#include "stdafx.h"
#include "IndexBuffer.h"

BOOL IndexBuffer::Initialize(const std::vector<UINT> indices)
{
	m_Indices = indices;
	m_uiStride = sizeof(UINT);
	m_uiCount = indices.size();

	D3D11_BUFFER_DESC desc;
	::ZeroMemory(&desc, sizeof(desc));
	{
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = m_uiStride * m_uiCount;
	}

	D3D11_SUBRESOURCE_DATA data;
	::ZeroMemory(&data, sizeof(data));
	data.pSysMem = m_Indices.data();

	if (FAILED(DEVICE->CreateBuffer(&desc, &data, m_pBuffer.GetAddressOf())))
	{
		__debugbreak();
		return FALSE;
	}

	return TRUE;

}
