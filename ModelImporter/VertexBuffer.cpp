#include "stdafx.h"
#include "VertexBuffer.h"

using namespace std;

vector<D3D11_INPUT_ELEMENT_DESC> VertexType::descs =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

BOOL VertexBuffer::Initialize(const std::vector<VertexType> vertices)
{
	m_Vertices = vertices;
	m_uiStride = sizeof(VertexType);
	m_uiCount = vertices.size();

	D3D11_BUFFER_DESC desc;
	::ZeroMemory(&desc, sizeof(desc));
	{
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = m_uiStride * m_uiCount;
	}

	D3D11_SUBRESOURCE_DATA data;
	::ZeroMemory(&data, sizeof(data));
	data.pSysMem = m_Vertices.data();

	if (FAILED(DEVICE->CreateBuffer(&desc, &data, m_pBuffer.GetAddressOf())))
	{
		__debugbreak();
		return FALSE;
	}

	return TRUE;
}
