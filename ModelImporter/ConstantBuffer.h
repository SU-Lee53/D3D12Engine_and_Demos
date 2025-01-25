#pragma once

template <typename T>
class ConstantBuffer
{
public:
	ConstantBuffer() {}
	~ConstantBuffer() {}

public:
	BOOL Initialize()
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, m_pBuffer.GetAddressOf())))
		{
			__debugbreak();
			return FALSE;
		}

		// Keep Constant Buffer to map
		// map - unmap is not efficient

		return TRUE;
	}

	void PushData(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		memset(&subResource, 0, sizeof(subResource));

		DC->Map(m_pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			memcpy(subResource.pData, &data, sizeof(data));
		}
		DC->Unmap(m_pBuffer.Get(), 0);
	}

	ComPtr<ID3D11Buffer>& GetBuffer() { return m_pBuffer; }

private:
	ComPtr<ID3D11Buffer>		m_pBuffer = nullptr;


};

