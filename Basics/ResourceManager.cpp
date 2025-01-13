#include "pch.h"
#include "ResourceManager.h"

BOOL ResourceManager::Initialize()
{

	D3D12_COMMAND_QUEUE_DESC desc = {};
	{
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	}

	ThrowIfFailed(
		DEVICE->CreateCommandQueue(&desc, IID_PPV_ARGS(m_pCommandQueue.GetAddressOf()))
	);

	CreateCommandList();
	CreateFence();

	return TRUE;
}

BOOL ResourceManager::CreateCommandList()
{
	ThrowIfFailed(
		DEVICE->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_pCommandAllocator.GetAddressOf()))
	);

	ThrowIfFailed(
		DEVICE->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocator.Get(), nullptr, IID_PPV_ARGS(m_pCommandList.GetAddressOf()))
	);

	m_pCommandList->Close();

	return TRUE;
}

BOOL ResourceManager::CreateFence()
{
	ThrowIfFailed(
		DEVICE->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_pFence.GetAddressOf()))
		);

	m_ui64FenceValue = 0;

	m_FenceEventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return m_ui64FenceValue;
}

HRESULT ResourceManager::CreateIndexBuffer(const std::vector<UINT>& initData, IndexBuffer& refOutBuffer)
{
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
	ComPtr<ID3D12Resource> pIndexBuffer = nullptr;
	ComPtr<ID3D12Resource> pUploadBuffer = nullptr;
	UINT IndexBufferSize = sizeof(UINT) * initData.size();

	ThrowIfFailed(
		DEVICE->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&pIndexBuffer)
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
				&CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize),
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&pUploadBuffer)
			)
		);

		UINT8* pIndexDataBegin = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		pUploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin));
		{
			memcpy(pIndexDataBegin, initData.data(), IndexBufferSize);
		}
		pUploadBuffer->Unmap(0, nullptr);

		m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pIndexBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		{
			m_pCommandList->CopyBufferRegion(pIndexBuffer.Get(), 0, pUploadBuffer.Get(), 0, IndexBufferSize);
		}
		m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(pIndexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));
		m_pCommandList->Close();

		ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
		m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		Fence();
		WaitForFenceValue();
	}

	IndexBufferView.BufferLocation = pIndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	IndexBufferView.SizeInBytes = IndexBufferSize;

	refOutBuffer.pIndexBuffer = pIndexBuffer;
	refOutBuffer.IndexBufferView = IndexBufferView;

	return S_OK;

}

UINT64 ResourceManager::Fence()
{
	m_ui64FenceValue++;
	m_pCommandQueue->Signal(m_pFence.Get(), m_ui64FenceValue);
	return m_ui64FenceValue;
}

void ResourceManager::WaitForFenceValue()
{
	const UINT64 ExpectedFenceValue = m_ui64FenceValue;

	// Wait until the previous frame is finished.
	if (m_pFence->GetCompletedValue() < ExpectedFenceValue)
	{
		m_pFence->SetEventOnCompletion(ExpectedFenceValue, m_FenceEventHandle);
		WaitForSingleObject(m_FenceEventHandle, INFINITE);
	}
}
