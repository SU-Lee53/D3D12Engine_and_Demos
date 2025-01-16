#pragma once

struct IndexBuffer
{
	std::vector<UINT> m_Indices;	// Cache

	ComPtr<ID3D11Buffer>	m_pBuffer;
	UINT	m_uiStride;
	UINT	m_uiOffset;
	size_t	m_uiCount;


	BOOL Initialize(const std::vector<UINT> indices);
};

