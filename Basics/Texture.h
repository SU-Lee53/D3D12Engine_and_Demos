#pragma once

class Texture
{
public:
	Texture();
	~Texture();

	// only TextureManager can create Texture instance
	friend class TextureManager;

private:
	BOOL Initialize(std::wstring wstrPath);

private:
	BOOL LoadFromDDSFile(std::wstring wstrPath, D3D12_RESOURCE_DESC& outDesc);
	BOOL LoadFromWICFile(std::wstring wstrPath, D3D12_RESOURCE_DESC& outDesc);

public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRV() { return m_SRV; }

private:
	ComPtr<ID3D12Resource> m_pTexResource = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE m_SRV = {};

	std::string m_strPath = "";
};

