#pragma once

class Texture
{
public:
	Texture();
	~Texture();

public:
	BOOL Initialize(std::wstring wstrPath);

private:
	BOOL LoadFromDDSFile(std::wstring wstrPath, D3D12_RESOURCE_DESC& outDesc);
	BOOL LoadFromWICFile(std::wstring wstrPath, D3D12_RESOURCE_DESC& outDesc);

private:
	ComPtr<ID3D12Resource> m_pTexResource = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE m_SRV = {};

	std::string m_strPath = "";
	std::string m_strName = "";
};

