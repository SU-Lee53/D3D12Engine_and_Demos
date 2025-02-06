#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

using namespace std;

BOOL TextureManager::Initialize()
{
    return 0;
}

BOOL TextureManager::CreateAndAddTexuture(const std::string& strTextureName, const std::wstring strTexturePath)
{
    BOOL bResult = TRUE;
    if (m_pTextureMap.find(strTextureName) != m_pTextureMap.end())
    {
        return TRUE;    // Texture already exist
    }

    shared_ptr<Texture> pTexture = make_shared<Texture>();
    bResult = pTexture->Initialize(strTexturePath);

    if (bResult == FALSE)
        return bResult;

    m_pTextureMap.insert(make_pair(strTextureName, pTexture));
    
    return bResult;
}

std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& strTextureName)
{
    auto it = m_pTextureMap.find(strTextureName);

    if (it == m_pTextureMap.end())
    {
        return nullptr;
    }

    return it->second;
}

BOOL TextureManager::RemoveTexture(const std::string& strTextureName)
{
    auto target = m_pTextureMap.find(strTextureName);

    // ref count != 1 -> Still in used
    if (target->second.use_count() != 1)
        __debugbreak();

    m_pTextureMap.erase(target);
}
