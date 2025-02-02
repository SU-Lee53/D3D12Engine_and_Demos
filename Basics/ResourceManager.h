#pragma once

#include "input_types.h"

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager)

public:
	BOOL Initialize();

private:
	// Internal Init functions
	BOOL CreateCommandList();
	BOOL CreateFence();

public:
	template<ShaderInputType T>
	HRESULT CreateVertexBuffer(const std::vector<T>& initData, VertexBuffer& refOutBuffer);
	HRESULT CreateIndexBuffer(const std::vector<UINT>& initData, IndexBuffer& refOutBuffer);

private:
	UINT64 Fence();
	void WaitForFenceValue();

public:
	ComPtr<ID3D12GraphicsCommandList>& GetCommandList() { return m_pCommandList; }
	
	BOOL CloseAndExcuteCommandList()
	{
		m_pCommandList->Close();

		ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
		m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	}

private:
	ComPtr<ID3D12GraphicsCommandList>	m_pCommandList = nullptr;
	ComPtr<ID3D12CommandAllocator>		m_pCommandAllocator = nullptr;
	ComPtr<ID3D12CommandQueue>			m_pCommandQueue = nullptr;

	ComPtr<ID3D12Fence>		m_pFence = nullptr;
	HANDLE					m_FenceEventHandle = nullptr;
	UINT64					m_ui64FenceValue = 0;

};

template<ShaderInputType T>
inline HRESULT ResourceManager::CreateVertexBuffer(const std::vector<T>& initData, VertexBuffer& refOutBuffer)
{
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
	ComPtr<ID3D12Resource> pVertexBuffer = nullptr;
	ComPtr<ID3D12Resource> pUploadBuffer = nullptr;
	UINT VertexBufferSize = sizeof(T) * initData.size();

	ThrowIfFailed(
		DEVICE->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(VertexBufferSize),
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&pVertexBuffer)
		)
	);

	if (!initData.empty())
	{
		ThrowIfFailed(m_pCommandAllocator->Reset());
		ThrowIfFailed(m_pCommandList->Reset(m_pCommandAllocator.Get(), nullptr));

		ThrowIfFailed(
			DEVICE->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(VertexBufferSize),
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&pUploadBuffer)
			)
		);


		UINT8* pVertexDataBegin = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		pUploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
		{
			memcpy(pVertexDataBegin, initData.data(), VertexBufferSize);
		}
		pUploadBuffer->Unmap(0, nullptr);

		m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pVertexBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		{
			m_pCommandList->CopyBufferRegion(pVertexBuffer.Get(), 0, pUploadBuffer.Get(), 0, VertexBufferSize);
		}
		m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pVertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
		m_pCommandList->Close();

		ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
		m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		Fence();
		WaitForFenceValue();
	}
	
	VertexBufferView.BufferLocation = pVertexBuffer->GetGPUVirtualAddress();
	VertexBufferView.StrideInBytes = sizeof(T);
	VertexBufferView.SizeInBytes = VertexBufferSize;

	refOutBuffer.pVertexBuffer = pVertexBuffer;
	refOutBuffer.VertexBufferView = VertexBufferView;

	return S_OK;

}
