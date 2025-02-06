#pragma once

class Texture;

class TextureManager
{
	DECLARE_SINGLE(TextureManager)

public:
	BOOL Initialize();

public:
	BOOL CreateAndAddTexuture(const std::string& strTextureName, const std::wstring strTexturePath);
	std::shared_ptr<Texture> GetTexture(const std::string& strTextureName);
	BOOL RemoveTexture(const std::string& strTextureName);


private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_pTextureMap;

};

