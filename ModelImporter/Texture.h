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

public:
	ComPtr<ID3D11ShaderResourceView>& GetComPtr() { return m_pSRV; }
	ComPtr<ID3D11Texture2D> GetTexture2D();

	XMFLOAT2 GetSize() { return m_v2Size; }

	const DirectX::ScratchImage& GetInfo() { return m_Image; }

private:
	ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;
	XMFLOAT2 m_v2Size = { 0.f,0.f };
	DirectX::ScratchImage m_Image;


};

