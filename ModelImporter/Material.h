#pragma once


// We will use forward rendering with no lighting in Importer
// Texture will be diffuse Only

#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	Material();
	~Material();

public:
	BOOL Initialize(const std::wstring& wstrTexturePath);

public:
	std::unique_ptr<Texture>& GetTexture() { return m_pTexture; }
	std::unique_ptr<Shader>& GetShader() { return m_pShader; }

private:
	std::unique_ptr<Texture> m_pTexture = nullptr;
	std::unique_ptr<Shader> m_pShader = nullptr;


};

