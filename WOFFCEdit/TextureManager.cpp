
#include <string>
#include <unordered_map>
#include <memory>
#include <d3d11.h>
#include <wrl/client.h>
// Include the header for DDSTextureLoader
#include <DDSTextureLoader.h>
#include "InputCommands.h"
#include "Game.h"
#include "TextureManager.h"





TextureManager::TextureManager()
{


}
TextureManager::TextureManager(ID3D11Device* device, std::vector<DisplayObject>& displayList, std::shared_ptr<DX::DeviceResources> device_resource)
	: m_device(device)
{
	m_displayList = &displayList;
	m_device_resource = device_resource;



}

TextureManager::~TextureManager()
{
}


bool TextureManager::LoadTexture(const std::wstring& filePath, const std::string& textureName) {
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	HRESULT hr = DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), filePath.c_str(), nullptr, texture.GetAddressOf());
	if (FAILED(hr)) {
		// Failed to load texture
		return false;
	}
	// Add texture to map
	m_textures[textureName] = texture;
	return true;
}

bool TextureManager::ApplyTextureToObject(const std::string& textureName, int objectIndex) {
	// Check if the texture exists in the manager's map
	auto it = m_textures.find(textureName);



	// Apply texture to each selected object
	for (size_t i = 0; i < m_displayList->size(); i++) {
		// Check if the object index is valid
		if (std::find(selectedObjects.begin(), selectedObjects.end(), i) != selectedObjects.end()) {
			// Load texture for the object
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/error.dds",
				nullptr, &(*m_displayList)[i].m_texture_diffuse);
			// Update the object's effect with the new texture
			(*m_displayList)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect) {
				// Set the texture 
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect) {
					basic_effect->SetTexture((*m_displayList)[i].m_texture_diffuse);
				}
				});
		}
		else {
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/placeholder.dds",
				nullptr, &(*m_displayList)[i].m_texture_diffuse);
			// Update the object's effect with the new texture
			(*m_displayList)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect) {
				// Set the texture  
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect) {
					basic_effect->SetTexture((*m_displayList)[i].m_texture_diffuse);
				}
				});


		}
		RecordState(); // Record state after applying textures
	}
	return true;
}





bool TextureManager::RemoveTexture(int objectID)
{
	if (objectID == 1)
	{

		// Apply texture to each selected object
		for (size_t i = 0; i < m_displayList->size(); ++i) {
			// Check if the object index is valid
			if (std::find(selectedObjects.begin(), selectedObjects.end(), i) != selectedObjects.end()) {
				// Load texture for the object
				DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/rock.dds",
					nullptr, &(*m_displayList)[i].m_texture_diffuse);
				// Update the object's effect with the new texture
				(*m_displayList)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect) {
					// Set the texture 
					const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
					if (basic_effect) {
						basic_effect->SetTexture((*m_displayList)[i].m_texture_diffuse);
					}
					});
			}


		
		}
		RecordState(); // Record state after removing texture
	}	
	return false;
}


void TextureManager::MoveSelectedObjects(const InputCommands& input_commands) {
	// Iterate through the selected objects
	for (size_t i = 0; i < m_displayList->size(); i++)
	{

		// Check if the object index is valid
		if (std::find(selectedObjects.begin(), selectedObjects.end(), i) != selectedObjects.end()) {
			if (input_commands.objectMoveForward) {
				// Update the position of the object
				(*m_displayList)[i].m_position.x += 0.1f;
			}
			if (input_commands.objectMoveBackward) {
				(*m_displayList)[i].m_position.x -= 0.1f;
			}
			
		}
	}
	RecordState(); // Record state after moving objects
}

void TextureManager::DeleteObject() {
	// look through the selected objects to delete
	for (auto it = selectedObjects.rbegin(); it != selectedObjects.rend(); ++it) {
		int objectId = *it;
		// Find the object with the specified ID
		auto obj = std::remove_if(m_displayList->begin(), m_displayList->end(),
			[objectId](const DisplayObject& obj) { return obj.m_ID == objectId; });
		// delete objects from the display list
		m_displayList->erase(obj, m_displayList->end());
	}

	// After deletion, reassign IDs
	for (size_t i = 0; i < m_displayList->size(); ++i) {
		(*m_displayList)[i].m_ID = static_cast<int>(i);
	}
	// Clear the list of selected objects 
	selectedObjects.clear();
	// Record state after deleting objects
	RecordState();
}

void TextureManager::SpawnObject() {

	DisplayObject newObject;

	// Create a new object with parameters

	newObject.m_model = (*m_displayList)[0].m_model;
	newObject.m_texture_diffuse = (*m_displayList)[0].m_texture_diffuse;

	newObject.m_position = DirectX::SimpleMath::Vector3::Vector3(2, 1, 2);
	newObject.m_scale = DirectX::SimpleMath::Vector3::Vector3(1, 1, 1);
	newObject.m_orientation = DirectX::SimpleMath::Vector3::Vector3(0, 0, 0);
	// Add the new object to the end of the display list
	m_displayList->push_back(newObject);

	// Assign a unique ID to the newly spawned object
	int newID = (m_displayList->size()) / 2;
	(*m_displayList)[newID].m_ID = newID;
	// Record the current state for undo
	RecordState();

}

void TextureManager::ScaleObject(int objectIndex, bool isVisible) {
	for (size_t i = 0; i < m_displayList->size(); i++) {
		// Check if the object index is valid
		if (std::find(selectedObjects.begin(), selectedObjects.end(), i) != selectedObjects.end()) {
			// scale object
			(*m_displayList)[i].m_scale = DirectX::SimpleMath::Vector3::Vector3(10, 10, 10);

		}
	}
	
}



// Function to record the current state for undo
void TextureManager::RecordState() {
	// Push the current state to the undo stack
	m_undoStack.push(*m_displayList);
	// Clear the redo stack 
	while (!m_redoStack.empty()) {
		m_redoStack.pop();
	}
}
// Function to undo the last operation
void TextureManager::Undo() {
	// Check if there is a previous state in the undo stack
	if (!m_undoStack.empty()) {
		// Push the current state to the redo stack
		m_redoStack.push(*m_displayList);
		// Restore the previous state 
		*m_displayList = m_undoStack.top();
		// Remove the previous state from the undo stack
		m_undoStack.pop();
	}
}
// Function to redo the last undone operation
void TextureManager::Redo() {
	// Check if there is a state in the redo stack to redo
	if (!m_redoStack.empty()) {
		// Push the current state to the undo stack
		m_undoStack.push(*m_displayList);
		// Restore the state from the redo stack
		*m_displayList = m_redoStack.top();
		// Remove the state from the redo stack
		m_redoStack.pop();
	}
}


