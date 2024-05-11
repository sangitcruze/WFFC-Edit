
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
	//if (it == m_textures.end()) {
	//	// Texture not found
	//	return false;
	//}


	// Apply texture to each selected object
	for (size_t i = 0; i < m_displayList->size(); i++) {
		// Check if the object index is valid
		if (std::find(selectedObjects.begin(), selectedObjects.end(), i) != selectedObjects.end()) {
			// Load texture for the object
			DirectX::CreateDDSTextureFromFile(m_device_resource->GetD3DDevice(), L"database/data/error.dds",
				nullptr, &(*m_displayList)[i].m_texture_diffuse);
			// Update the object's effect with the new texture
			(*m_displayList)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect) {
				// Set the texture for the basic effect (assumed to be used)
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
				// Set the texture for the basic effect (assumed to be used)
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect) {
					basic_effect->SetTexture((*m_displayList)[i].m_texture_diffuse);
				}
				});


		}
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
					// Set the texture for the basic effect (assumed to be used)
					const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
					if (basic_effect) {
						basic_effect->SetTexture((*m_displayList)[i].m_texture_diffuse);
					}
					});
			}


			return false;
		}
	}
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
			// Add more movement controls as needed
		}
	}
}

void TextureManager::DeleteObject() {
	// Iterate through the selected objects to delete
	for (auto it = selectedObjects.rbegin(); it != selectedObjects.rend(); ++it) {
		int objectId = *it;
		// Find the object with the specified ID
		auto obj = std::remove_if(m_displayList->begin(), m_displayList->end(),
			[objectId](const DisplayObject& obj) { return obj.m_ID == objectId; });
		// Erase the marked objects from the display list
		m_displayList->erase(obj, m_displayList->end());
	}

	// After deletion, reassign IDs
	for (size_t i = 0; i < m_displayList->size(); ++i) {
		(*m_displayList)[i].m_ID = static_cast<int>(i);
	}
	// Clear the list of selected objects since they have been deleted
	selectedObjects.clear();
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



	
}