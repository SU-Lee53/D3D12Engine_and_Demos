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
		os << std::endl;
		os << "</Diffuse>" << std::endl;
		
		os << "<Specular>" << std::endl;
		os.write(reinterpret_cast<const char*>(&data.specularColor), sizeof(data.specularColor));
		os << std::endl;
		os << "</Specular>" << std::endl;
		
		os << "<Emissive>" << std::endl;
		os.write(reinterpret_cast<const char*>(&data.emissiveColor), sizeof(data.emissiveColor));
		os << std::endl;
		os << "</Emissive>" << std::endl;
		
		os << "<Ambient>" << std::endl;
		os.write(reinterpret_cast<const char*>(&data.ambientColor), sizeof(data.ambientColor));
		os << std::endl;
		os << "</Ambient>" << std::endl;

		//os.write(reinterpret_cast<const char*>(&data), sizeof(data));

		os << "</Color Data>" << std::endl;

		return os;
	}

	friend std::istream& operator>>(std::istream& is, MaterialData data)
	{
		std::string read;
		while (read != "</Color Data>")
		{
			std::getline(is, read);
			if (read == "<Diffuse>")
			{
				is.read(reinterpret_cast<char*>(&data.diffuseColor), sizeof(data.diffuseColor));
			}
			if (read == "<Specular>")
			{
				is.read(reinterpret_cast<char*>(&data.specularColor), sizeof(data.specularColor));
			}
			if (read == "<Emissive>")
			{
				is.read(reinterpret_cast<char*>(&data.emissiveColor), sizeof(data.emissiveColor));
			}
			if (read == "<Ambient>")
			{
				is.read(reinterpret_cast<char*>(&data.ambientColor), sizeof(data.ambientColor));
			}
		
		}

		return is;
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
	// TODO : Copy texture file to ../Model/Texture (02.06.2025)
	friend std::ostream& operator<<(std::ostream& os, const Material& material)
	{
		os << "<Material>" << std::endl;
		os << material.m_MaterialData;
		os << *material.m_pDiffuseTexture;
		os << "</Material>" << std::endl;


		return os;
	}

	friend std::istream& operator>>(std::istream& is, Material& material)
	{
		std::string read;
		std::string strTexturePath;
		while (read != "</Material>")
		{
			std::getline(is, read);
			if (read == "<Color Data>")
			{
				is >> material.m_MaterialData;
			}

			if (read == "<Texture>")
			{
				while (read != "</Texture>")
				{
					std::getline(is, read);
					if (read == "<Texture Path>")
					{
						std::getline(is, strTexturePath);
					}
				}
			}
		}

		std::wstring wstrInitParameter(strTexturePath.begin(), strTexturePath.end());

		material.Initialize(wstrInitParameter);

		return is;
	}

};

