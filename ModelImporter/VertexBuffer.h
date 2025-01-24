#pragma once

struct VertexType
{
	XMFLOAT3 Position = { 0,0,0 };
	XMFLOAT4 Color = { 0,0,0,0 };
	XMFLOAT2 TexCoord = { 0,0 };
	XMFLOAT3 Normal = { 0,0,0 };
	XMFLOAT3 BiNormal = { 0,0,0 };
	XMFLOAT3 Tangent = { 0,0,0 };

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexBuffer
{
	std::vector<VertexType> m_Vertices;	// Cache

	ComPtr<ID3D11Buffer>	m_pBuffer;
	UINT	m_uiStride;
	UINT	m_uiOffset;
	size_t	m_uiCount;
	
	BOOL Initialize(const std::vector<VertexType> vertices);



};

