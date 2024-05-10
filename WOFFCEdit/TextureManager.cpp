
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
TextureManager::TextureManager(ID3D11Device* device, std::vector<DisplayObject>	&displayList, std::shared_ptr<DX::DeviceResources> device_resource)
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

bool TextureManager::ApplyTextureToObject(const std::string& textureName , int objectIndex ) {
	// Check if the texture exists in the manager's map
	auto it = m_textures.find(textureName);
	//if (it == m_textures.end()) {
	//	// Texture not found
	//	return false;
	//}
	
	
	// Apply texture to each selected object
	for (size_t i = 0; i < m_displayList->size(); i++){
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
	}
	return true;
}

void TextureManager::MoveSelectedObjects(const InputCommands& input_commands) {
	// Iterate through the selected objects
	for (size_t i = 0; i < m_displayList->size(); i++) {
	
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
