#pragma once
#include "Descriptor.h"
#include "CB_types.h"

/*
	- how this run
		- This ConstantBuffer<T> is not directly bind to pipelines(in RenderMethod)
			- So, Descriptor Heap's type in this class will be "D3D12_DESCRIPTOR_FLAG_NONE"
		- Instead, Copy data in this class to another Descriptor Heap
			- This process will be done in RenderMethod::Render
			- Copy Destination will be other Descriptor Heap in RenderMethod(Base)
			- Near future, Copy Dest will be replace to Descriptor Heap from Descriptor Pool

*/


template <typename T>
class ConstantBuffer
{
public:
	using BufferType = T;

public:
	ConstantBuffer();
	~ConstantBuffer();

	BOOL Initialize();
	void PushData(const T& data);

public:
	std::unique_ptr<DescriptorHeap>& GetDescriptorHeap() { return m_pDescriptorHeap; }

private:
	ComPtr<ID3D12Resource>			m_pResource = nullptr;
	std::unique_ptr<DescriptorHeap> m_pDescriptorHeap = nullptr;

	// cbv data cache
	D3D12_GPU_VIRTUAL_ADDRESS		m_GPUAddr;
	BufferType*						m_rpSysMemAddr = nullptr;

	UINT m_uiCBVSize = 0;
	UINT m_uiDescriptorSize = 0;
};

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer()
{
}

template<typename T>
inline ConstantBuffer<T>::~ConstantBuffer()
{
}

template<typename T>
inline BOOL ConstantBuffer<T>::Initialize()
{
	m_uiCBVSize = Utils::AlignConstantBuffersize(sizeof(T));

	// Create ID3D12Resource
	ThrowIfFailed(
		DEVICE->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(m_uiCBVSize),
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

	m_pDescriptorHeap = std::make_unique<DescriptorHeap>();
	m_pDescriptorHeap->Initialize(heapDesc);

	// Write cache datas
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	{
		cbvDesc.BufferLocation = m_pResource->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = m_uiCBVSize;
	}
	
	DEVICE->CreateConstantBufferView(&cbvDesc, m_pDescriptorHeap->DescriptorHandleFromStart.cpuHandle);
	m_GPUAddr = cbvDesc.BufferLocation;
	m_uiDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Map data pointer
	CD3DX12_RANGE writeRange(0, 0);
	m_pResource->Map(0, &writeRange, reinterpret_cast<void**>(&m_rpSysMemAddr));

	return TRUE;
}

template<typename T>
inline void ConstantBuffer<T>::PushData(const T& data)
{
	*m_rpSysMemAddr = data;
}
