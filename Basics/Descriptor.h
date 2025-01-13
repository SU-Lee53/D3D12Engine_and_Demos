#pragma once

struct Descriptor
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};

struct DescriptorHeap
{
	ComPtr<ID3D12DescriptorHeap> pDescriptorHeap;
	Descriptor DescriptorHandleFromStart;
	UINT uiDescriptorSize;
	UINT uiCurrentDescriptorCount;
	UINT uiAllocatedDescriptorCount;

	BOOL Initialize(const D3D12_DESCRIPTOR_HEAP_DESC& desc)
	{
		ThrowIfFailed(
			DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(pDescriptorHeap.GetAddressOf()))
		);

		uiAllocatedDescriptorCount = 0;
		uiCurrentDescriptorCount = desc.NumDescriptors;
		uiDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		DescriptorHandleFromStart.cpuHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

		if(desc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
			DescriptorHandleFromStart.gpuHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

		return TRUE;
	}

	Descriptor Alloc()
	{
		return (*this)[uiAllocatedDescriptorCount++];
	}

	Descriptor operator[](UINT index)
	{
		if (index >= uiCurrentDescriptorCount) __debugbreak();

		return Descriptor{
			CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHandleFromStart.cpuHandle, index, uiDescriptorSize),
			CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHandleFromStart.gpuHandle, index, uiDescriptorSize),
		};
	}

	void Reset()
	{
		uiAllocatedDescriptorCount = 0;
	}

};
