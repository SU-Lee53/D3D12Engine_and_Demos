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
    {
        assert(CreateErrorTexture());
    }

    m_pTextureMap.insert(make_pair(strTextureName, pTexture));
    
    return bResult;
}

std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& strTextureName)
{
    auto it = m_pTextureMap.find(strTextureName);

    if (it == m_pTextureMap.end())
    {
        // 1. find error texture
        it = m_pTextureMap.find("Error");

        // 2. if error is also not available : return nullptr
        if (it == m_pTextureMap.end())
        {
            return nullptr;
        }
    }

    return it->second;  // return value will be correct texture OR error texture
}

BOOL TextureManager::RemoveTexture(const std::string& strTextureName)
{
    auto target = m_pTextureMap.find(strTextureName);

    // ref count != 1 -> Still in used
    if (target->second.use_count() != 1)
    {
        __debugbreak();
        return FALSE;
    }

    m_pTextureMap.erase(target);

    return TRUE;
}

BOOL TextureManager::CreateErrorTexture()
{
    // if error texture is already created -> return immediatly
    if (m_pTextureMap.find("Error") != m_pTextureMap.end())
    {
        return TRUE;
    }

    BOOL bResult = TRUE;
    shared_ptr<Texture> pTexture = make_shared<Texture>();
    bResult = pTexture->Initialize(L"../Models/Texture/error.jpg");
    if (!bResult)
    {
        __debugbreak();
    }

    m_pTextureMap.insert(make_pair("Error", pTexture));

    return TRUE;
}
