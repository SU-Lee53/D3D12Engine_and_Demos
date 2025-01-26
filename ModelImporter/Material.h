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

	friend std::ostream& operator<<(std::ostream& os, const MaterialData& data)
	{
		os << "<Color Data>" << std::endl;

		os << "<Diffuse>" << std::endl;
		os.write(reinterpret_cast<const char*>(&data.diffuseColor), sizeof(data.diffuseColor));
		os << "</Diffuse>" << std::endl;

		os << "<Specular>" << std::endl;
		os.write(reinterpret_cast<const char*>(&data.specularColor), sizeof(data.specularColor));
		os << "</Specular>" << std::endl;

		os << "<Emissive>" << std::endl;
		os.write(reinterpret_cast<const char*>(&data.emissiveColor), sizeof(data.emissiveColor));
		os << "</Emissive>" << std::endl;

		os << "<Ambient>:" << std::endl;
		os.write(reinterpret_cast<const char*>(&data.ambientColor), sizeof(data.ambientColor));
		os << "</Ambient>:" << std::endl;

		os << "</Color Data>" << std::endl;

		return os;
	}
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

	// This can be extend near future
	// Right now(01.26.2025), Export MaterialData, Texture(path)
	friend std::ostream& operator<<(std::ostream& os, const Material& material)
	{
		os << "<Material>" << std::endl;
		os << material.m_MaterialData;
		os << *material.m_pDiffuseTexture;
		return os;
	}
};

