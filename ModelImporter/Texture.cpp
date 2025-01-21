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
	filesystem::path fsFilePath(wstrPath);
	string extension = fsFilePath.filename().extension().string();

	DirectX::TexMetadata texMetaData;

	if (extension == ".dds"s)
	{
		if (FAILED(LoadFromDDSFile(wstrPath.c_str(), DDS_FLAGS_FORCE_RGB, &texMetaData, m_Image)))
		{
			__debugbreak();
			return FALSE;
		}
	}
	else
	{
		if (FAILED(LoadFromWICFile(wstrPath.c_str(), WIC_FLAGS_FORCE_RGB, &texMetaData, m_Image)))
		{
			__debugbreak();
			return FALSE;
		}
	}


	if (FAILED(CreateShaderResourceView(DEVICE.Get(), m_Image.GetImages(), m_Image.GetImageCount(), texMetaData, m_pSRV.GetAddressOf())))
	{
		__debugbreak();
		return FALSE;
	}

	m_v2Size.x = texMetaData.width;
	m_v2Size.y = texMetaData.height;
}

ComPtr<ID3D11Texture2D> Texture::GetTexture2D()
{
	ComPtr<ID3D11Texture2D> texture;
	m_pSRV->GetResource((ID3D11Resource**)texture.GetAddressOf());
	return texture;
}
