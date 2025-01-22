#pragma once


// We will use forward rendering with no lighting in Importer
// Texture will be diffuse Only

#include "Texture.h"
#include "Shader.h"

struct MaterialData
{
	XMFLOAT4 diffuseColor;
	XMFLOAT4 specularColor;
	XMFLOAT4 emissiveColor;
	XMFLOAT4 ambientColor;
};

class Material
{
public:
	Material();
	~Material();

public:
	BOOL Initialize(const std::wstring& wstrTexturePath);

public:
	std::unique_ptr<Texture>& GetTexture() { return m_pDiffuseTexture; }
	std::unique_ptr<Shader>& GetShader() { return m_pShader; }

	MaterialData& GetMaterialData() { return m_MaterialData; }
	void SetMaterialData(const MaterialData& data) { m_MaterialData = data; }

private:
	std::unique_ptr<Texture> m_pDiffuseTexture = nullptr;
	std::unique_ptr<Shader> m_pShader = nullptr;

	MaterialData m_MaterialData;


};

