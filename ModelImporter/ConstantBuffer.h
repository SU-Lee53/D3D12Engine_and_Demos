#pragma once

template <typename T>
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

public:
	BOOL Initialize()
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, _buffer.GetAddressOf())))
		{
			__debugbreak();
			return FALSE;
		}

		// Keep Constant Buffer to map
		// map - unmap is not efficient
		::memset(&m_d3dSubresource, 0, sizeof(m_d3dSubresource));
		return TRUE;
	}

	void PushData(const T& data)
	{
		::memcpy(m_d3dSubresource.pData, &data, sizeof(data));
	}


private:
	ComPtr<ID3D11Buffer>		m_pBuffer = nullptr;
	D3D11_MAPPED_SUBRESOURCE	m_d3dSubresource = {};


};

