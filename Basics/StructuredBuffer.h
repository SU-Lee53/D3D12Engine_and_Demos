#pragma once

template <typename T, size_t size>
class StructuredBuffer
{
public:
	using ElementType = T;

public:
	StructuredBuffer();
	~StructuredBuffer();

	BOOL Initialize();
	void PushData(const T* data, const size_t nData);

public:
	std::unique_ptr<DescriptorHeap>& GetDescriptorHeap() { return m_upDescriptorHeap; }
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return m_GPUAddr; }

private:
	ComPtr<ID3D12Resource> m_pResource = nullptr;
	std::unique_ptr<DescriptorHeap> m_upDescriptorHeap = nullptr;

	D3D12_BUFFER_SRV m_SRVBuffer = 0;
	D3D12_GPU_VIRTUAL_ADDRESS m_GPUAddr = 0;
	ElementType* m_rpSysMemAddr = nullptr;

	UINT m_uiSRVSize = 0;	// Structured Buffer can be SRV or UAV
	UINT m_uiDescriptorSize = 0;
	UINT m_size = size;
};

template<typename T, size_t size>
inline StructuredBuffer<T, size>::StructuredBuffer()
{
}

template<typename T, size_t size>
inline StructuredBuffer<T, size>::~StructuredBuffer()
{
}

template<typename T, size_t size>
inline BOOL StructuredBuffer<T, size>::Initialize()
{
	D3D12_RESOURCE_DESC bufferDesc = {};
	{
		bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		bufferDesc.Width = sizeof(ElementType) * size;
		bufferDesc.Height = 1;
		bufferDesc.DepthOrArraySize = 1;
		bufferDesc.MipLevels = 1;
		bufferDesc.SampleDesc.Count = 1;
		bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}

	ThrowIfFailed(
		DEVICE->CreateCommittedResource
		(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_pResource.GetAddressOf())
		)
	);

	// Create ID3D12DescriptorHeap
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	{
		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	}

	m_upDescriptorHeap = std::make_unique<DescriptorHeap>();
	m_upDescriptorHeap->Initialize(heapDesc);

	// Write cache datas
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	{
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;	// This is just structure, not texture
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = size;
		srvDesc.Buffer.StructureByteStride = sizeof(ElementType);
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	}

	DEVICE->CreateShaderResourceView(m_pResource.Get(), &srvDesc, m_upDescriptorHeap->DescriptorHandleFromStart.cpuHandle);
	m_SRVBuffer = srvDesc.Buffer;
	m_uiDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_uiSRVSize = sizeof(ElementType) * size;
	m_GPUAddr = m_pResource->GetGPUVirtualAddress();

	// Map data pointer
	CD3DX12_RANGE writeRange(0, 0);
	m_pResource->Map(0, &writeRange, reinterpret_cast<void**>(&m_rpSysMemAddr));

	return TRUE;
}

template<typename T, size_t size>
inline void StructuredBuffer<T, size>::PushData(const T* data, const size_t nData)
{
	::memcpy(m_rpSysMemAddr, data, sizeof(T) * nData);
}
