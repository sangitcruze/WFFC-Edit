#pragma once


#include <string>
#include <unordered_map>
#include <memory>
#include <d3d11.h>
#include <wrl/client.h>
#include "DisplayObject.h"
#include <stack>


class TextureManager
{
public:
    TextureManager();
    TextureManager(ID3D11Device* device, std::vector<DisplayObject> &displayList, std::shared_ptr<DX::DeviceResources> m_device_resource);
    ~TextureManager();


 

   /* void Initialise(std::vector<DisplayObject>* startingObjects,const std::shared_ptr<DX::DeviceResources>& device_resources);*/


    // Load texture from file
    bool LoadTexture(const std::wstring& filePath, const std::string& textureName);

    // Apply texture to object
    bool ApplyTextureToObject(const std::string& textureName = "", int objectIndex = -1);

    //Remove texture 
    bool RemoveTexture(int objectID = -1);

    void TextureManager::MoveSelectedObjects(const InputCommands& input_commands);

    void TextureManager::DeleteObject();

    void SpawnObject();

    std::vector<int> selectedObjects;
    const std::vector<int>& GetSelectedObjects() const { return selectedObjects; }
   

private:
    ID3D11Device* m_device;
    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

    std::vector<DisplayObject>* m_displayList;
    std::shared_ptr<DX::DeviceResources> m_device_resource;
    

};
