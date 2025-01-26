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

	friend std::ostream& operator<<(std::ostream& os, const VertexBuffer& buf)
	{
		os << "<Vertex>" << std::endl;
		for (const auto& vtx : buf.m_Vertices)
		{
			os.write(reinterpret_cast<const char*>(&vtx), sizeof(vtx));
		}
		os << "</Vertex>" << std::endl;

		return os;
	}

};

