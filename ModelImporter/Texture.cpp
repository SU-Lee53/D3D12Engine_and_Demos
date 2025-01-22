#include "stdafx.h"
#include "Texture.h"
#include <filesystem>

using namespace std;
using namespace std::literals;

Texture::Texture()
{
}

Texture::~Texture()
{
}

BOOL Texture::Initialize(const std::wstring& wstrPath)
{
	return LoadFromFile(wstrPath);
}

BOOL Texture::LoadFromFile(const std::wstring& wstrPath)
{
	DirectX::TexMetadata texMetaData;

	if (!wstrPath.empty())
	{
		filesystem::path fsFilePath(wstrPath);
		string extension = fsFilePath.filename().extension().string();

		// if failed to load file, create checkerboard error texture
		if (extension == ".dds"s)
		{
			if (FAILED(LoadFromDDSFile(wstrPath.c_str(), DDS_FLAGS_FORCE_RGB, &texMetaData, m_Image)))
			{
				CreateErrorTexture(texMetaData);
			}
		}
		else
		{
			if (FAILED(LoadFromWICFile(wstrPath.c_str(), WIC_FLAGS_FORCE_RGB, &texMetaData, m_Image)))
			{
				CreateErrorTexture(texMetaData);
			}
		}
	}
	else
	{
		CreateErrorTexture(texMetaData);
	}


	if (FAILED(CreateShaderResourceView(DEVICE.Get(), m_Image.GetImages(), m_Image.GetImageCount(), texMetaData, m_pSRV.GetAddressOf())))
	{
		__debugbreak();
		return FALSE;
	}

	m_v2Size.x = texMetaData.width;
	m_v2Size.y = texMetaData.height;
}

BOOL Texture::CreateErrorTexture(DirectX::TexMetadata& texMetaData)
{
	wstring errorPath = L"../Resource/error.jpg";

	if (FAILED(LoadFromWICFile(errorPath.c_str(), WIC_FLAGS_FORCE_RGB, &texMetaData, m_Image)))
	{
		__debugbreak();
		return FALSE;
	}

	m_bIsErrorTexture = TRUE;
	return TRUE;
}

ComPtr<ID3D11Texture2D> Texture::GetTexture2D()
{
	ComPtr<ID3D11Texture2D> texture;
	m_pSRV->GetResource((ID3D11Resource**)texture.GetAddressOf());
	return texture;
}
