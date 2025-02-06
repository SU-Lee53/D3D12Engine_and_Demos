#pragma once

class Texture
{
public:
	Texture();
	~Texture();

public:
	BOOL Initialize(const std::wstring& wstrPath);

private:
	BOOL LoadFromFile(const std::wstring& wstrPath);

	BOOL CreateErrorTexture(DirectX::TexMetadata& texMetaData);

public:
	ComPtr<ID3D11ShaderResourceView>& GetComPtr() { return m_pSRV; }
	ComPtr<ID3D11Texture2D> GetTexture2D();

	XMFLOAT2 GetSize() { return m_v2Size; }

	const DirectX::ScratchImage& GetInfo() { return m_Image; }
	
	std::string GetFileName() { return m_strFileName; }

private:
	ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;
	XMFLOAT2 m_v2Size = { 0.f,0.f };
	DirectX::ScratchImage m_Image;

	BOOL m_bIsErrorTexture = FALSE;
	std::string m_strFileName = "";

public:
	friend std::ostream& operator<<(std::ostream& os, const Texture& tex)
	{
		os << "<Texture>" << std::endl;
		os << "<Texture Path>" << std::endl;
		os << tex.m_strFileName << std::endl;
		os << "</Texture Path>" << std::endl;
		os << "</Texture>" << std::endl;

		// Copy texture file to ../Model/Texture/
		{
			std::wstring wstrTextureSavePathAndName = Texture::m_wstrTextureSavePath + std::wstring(tex.m_strFileName.begin(), tex.m_strFileName.end());

			if (!std::filesystem::exists(Texture::m_wstrTextureSavePath))
			{
				std::filesystem::create_directories(Texture::m_wstrTextureSavePath);
			}

			// 1. find Texture file using std::filesystem

			std::filesystem::path fbxPath(L"../fbx/");

			for (const std::filesystem::directory_entry& e : std::filesystem::recursive_directory_iterator(fbxPath))
			{
				std::filesystem::path cur = e.path();
				if (cur.filename().string() == tex.m_strFileName)
				{
					std::filesystem::path save(wstrTextureSavePathAndName);
					if (std::filesystem::exists(save))
					{
						// file already copied
						break;
					}

					try {
						std::filesystem::copy_file(cur, save);
					}
					catch (std::filesystem::filesystem_error err)
					{
						OutputDebugStringA(err.what());
						__debugbreak();
					}
					break;
				}
			}

		}
		return os;
	}

	friend std::istream& operator>>(std::istream& is, Texture& tex)
	{
		std::string read;
		while (read != "</Texture>")
		{
			std::getline(is, read);
			if (read == "<Texture Path>")
			{
				std::getline(is, tex.m_strFileName);
			}
		}

		return is;
	}


public:
	static std::wstring m_wstrTextureSavePath;
};
