#include "pch.h"
#include "RenderManager.h"
#include "Core.h"
#include "Object.h"
#include "RenderMethod.h"
#include "Camera.h"

BOOL RenderManager::Initialize()
{
	BOOL bResult = CreateCommandList();
	return bResult;
}

BOOL RenderManager::CreateCommandList()
{
	for (DWORD i = 0; i < MAX_PENDING_FRAME_COUNT; i++)
	{
		HR_ASSERT(DEVICE->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_pCommandAllocators[i].GetAddressOf())
		));

		HR_ASSERT(DEVICE->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_pCommandAllocators[i].Get(),
			nullptr,
			IID_PPV_ARGS(m_pCommandLists[i].GetAddressOf())
		));

		m_pCommandLists[i]->Close();
	}

	return TRUE;
}

void RenderManager::RenderBegin()
{
	m_pRenderQueue.clear();

	DWORD dwCurContextIndex = CORE.GetCurrentContextIndex();
	ComPtr<ID3D12CommandAllocator> pCommandAllocator = m_pCommandAllocators[dwCurContextIndex];
	ComPtr<ID3D12GraphicsCommandList> pCommandList = m_pCommandLists[dwCurContextIndex];

	HR_ASSERT(pCommandAllocator->Reset());
	HR_ASSERT(pCommandList->Reset(pCommandAllocator.Get(), nullptr));

	UINT uiRenderTargetIndex = CORE.GetCurrentRenderTargetIndex();
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(CORE.GetRTVHeap()->GetCPUDescriptorHandleForHeapStart(), uiRenderTargetIndex, CORE.GetRTVDescriptorSize());
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(CORE.GetDSVHeap()->GetCPUDescriptorHandleForHeapStart());

	pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CORE.GetCurrentRenderTarget().Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));


	Color BackColor = GAME.GetGameDesc().BackColor;
	pCommandList->ClearRenderTargetView(rtvHandle, (float*)&BackColor, 0, nullptr);
	pCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);

	pCommandList->RSSetViewports(1, &CORE.GetViewport());
	pCommandList->RSSetScissorRects(1, &CORE.GetScissorRect());
	pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
}

void RenderManager::Render()
{
	// TODO : Need to think about good structure

	// DO NOT PUSH Camera data IN THIS FUNCTION
	// Reason : camera can be diffrent by each render passes

	for (const auto& p : m_pRenderQueue)
	{
		p.first->Render(p.second);
	}

}

void RenderManager::RenderEnd()
{
	DWORD dwCurContextIndex = CORE.GetCurrentContextIndex();
	ComPtr<ID3D12GraphicsCommandList> pCommandList = m_pCommandLists[dwCurContextIndex];

	pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CORE.GetCurrentRenderTarget().Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	pCommandList->Close();

	ID3D12CommandList* ppCommandLists[] = { pCommandList.Get() };
	COMMAND_QUEUE->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void RenderManager::Present()
{
	CORE.Fence();

	// Set VSync 
	UINT uiSyncInterval;
	UINT uiPresentFlags;

	if (bVsync)
		uiSyncInterval = 1;	// VSync ON
	else
		uiSyncInterval = 0;	// VSync OFF
	
	if (!uiSyncInterval)
		uiPresentFlags = DXGI_PRESENT_ALLOW_TEARING;
	else
		uiPresentFlags = 0;

	HRESULT hr = CORE.GetSwapChain()->Present(uiSyncInterval, uiPresentFlags);

	if (hr == DXGI_ERROR_DEVICE_REMOVED)
		__debugbreak();

	CORE.SetCurrentRenderTargetIndex(CORE.GetSwapChain()->GetCurrentBackBufferIndex());

	DWORD dwNextContextIndex = (CORE.GetCurrentContextIndex() + 1) % MAX_PENDING_FRAME_COUNT;

	CORE.WaitForFenceValue(CORE.GetFenceValue(dwNextContextIndex));

	CORE.SetCurrentContextIndex(dwNextContextIndex);
}
