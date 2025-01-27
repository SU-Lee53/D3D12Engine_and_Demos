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
		os << "<Vertex Data>" << std::endl;

		{
			os << "<Vertex Size>" << std::endl;
			size_t nVertices = buf.m_Vertices.size();
			os.write(reinterpret_cast<const char*>(&nVertices), sizeof(nVertices));
			os << std::endl;
			os << "</Vertex Size>" << std::endl;

			os << "<Vertex>" << std::endl;
			os.write(reinterpret_cast<const char*>(buf.m_Vertices.data()), sizeof(buf.m_Vertices[0]) * nVertices);
			os << std::endl;
			os << "\n</Vertex>" << std::endl;
		}

		os << "</Vertex Data>" << std::endl;

		return os;
	}

	friend std::istream& operator>>(std::istream& is, VertexBuffer& buf)
	{
		std::string read;
		size_t nVertices = 0;
		std::vector<VertexType> vtx = {};

		while (read != "</Vertex>")
		{
			std::getline(is, read);

			if (read == "<Vertex Size>")
			{
				is.read(reinterpret_cast<char*>(&nVertices), sizeof(nVertices));
				vtx.resize(nVertices);
			}

			if (read == "<Vertex>")
			{
				is.read(reinterpret_cast<char*>(vtx.data()), sizeof(vtx[0]) * nVertices);
			}

		}

		buf.Initialize(vtx);

		return is;
	}

};

