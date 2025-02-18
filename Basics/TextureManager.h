#pragma once

class Texture;

class TextureManager
{
	DECLARE_SINGLE(TextureManager)

public:
	BOOL Initialize();

public:
	BOOL CreateAndAddTexuture(const std::string& strTextureName, const std::wstring strTexturePath);
	BOOL CreateBlankCubemapAndAddTexture(const std::string& strName, UINT uiCubeMapSize = 512, UINT uiMipLevels = 0, D3D12_RESOURCE_STATES initResourceState = D3D12_RESOURCE_STATE_RENDER_TARGET);

	std::shared_ptr<Texture> GetTexture(const std::string& strTextureName);
	BOOL RemoveTexture(const std::string& strTextureName);

private:
	BOOL CreateErrorTexture();

private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_pTextureMap;

};

