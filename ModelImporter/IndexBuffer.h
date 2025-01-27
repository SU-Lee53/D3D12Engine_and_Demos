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
		os << "<Index Data>" << std::endl;
		{
			os << "<Index Size>" << std::endl;
			size_t nIndices = buf.m_Indices.size();
			os.write(reinterpret_cast<const char*>(&nIndices), sizeof(nIndices));
			os << std::endl;
			os << "</Index Size>" << std::endl;

			os << "<Index>" << std::endl;
			os.write(reinterpret_cast<const char*>(buf.m_Indices.data()), sizeof(buf.m_Indices[0]) * nIndices);
			os << std::endl;
			os << "\n</Index>" << std::endl;
		}
		os << "</Index Data>" << std::endl;

		return os;
	}

	friend std::istream& operator>>(std::istream& is, IndexBuffer& buf)
	{
		std::string read;
		size_t nIndices = 0;
		std::vector<UINT> idx = {};

		while (read != "</Index>")
		{
			std::getline(is, read);

			if (read == "<Index Size>")
			{
				is.read(reinterpret_cast<char*>(&nIndices), sizeof(nIndices));
				idx.resize(nIndices);
			}

			if (read == "<Index>")
			{
				is.read(reinterpret_cast<char*>(idx.data()), sizeof(idx[0]) * nIndices);
			}

		}

		buf.Initialize(idx);

		return is;
	}
};

