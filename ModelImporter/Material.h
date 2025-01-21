#pragma once


// We will use forward rendering with no lighting in Importer
// Texture will be diffuse Only

class Texture;
class Shader;

class Material
{
public:
	Material();
	~Material();

public:
	BOOL Initialize(const std::wstring& wstrTexturePath);

public:
	void SetTexture(std::shared_ptr<Texture>& pTexture) { m_pTexture = pTexture; }
	void SetShader(std::shared_ptr<Shader>& pShader) { m_pShader = pShader; }
	
	std::shared_ptr<Texture>& GetTexture() { return m_pTexture; }
	std::shared_ptr<Shader>& GetShader() { return m_pShader; }

private:
	std::shared_ptr<Texture> m_pTexture = nullptr;
	std::shared_ptr<Shader> m_pShader = nullptr;


};

