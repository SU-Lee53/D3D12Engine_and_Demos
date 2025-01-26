#pragma once

struct IndexBuffer
{
	std::vector<UINT> m_Indices;	// Cache

	ComPtr<ID3D11Buffer>	m_pBuffer;
	UINT	m_uiStride;
	UINT	m_uiOffset;
	size_t	m_uiCount;


	BOOL Initialize(const std::vector<UINT> indices);


	friend std::ostream& operator<<(std::ostream& os, const IndexBuffer& buf)
	{
		os << "<Index>" << std::endl;
		for (const auto& idx : buf.m_Indices)
		{
			os.write(reinterpret_cast<const char*>(&idx), sizeof(idx));
		}
		os << "</Index>" << std::endl;

		return os;
	}
};

