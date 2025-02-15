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
	D3D12_BUFFER_SRV GetSRV() { return m_SRV; }
	std::unique_ptr<DescriptorHeap>& GetDescriptorHeap() { return m_upDescriptorHeap; }
	ComPtr<ID3D12Resource>& GetResource() { return m_pTexResource; }

private:
	ComPtr<ID3D12Resource> m_pTexResource = nullptr;
	std::unique_ptr<DescriptorHeap> m_upDescriptorHeap = nullptr;

	D3D12_BUFFER_SRV m_SRV = {};

	std::string m_strPath = "";
};

