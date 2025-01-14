#pragma once
#include "CB_types.h"

class Core
{
	DECLARE_SINGLE(Core)

public:
	BOOL Initialize(BOOL bEnableDebugLayer, BOOL bEnableGBV);
	BOOL UpdateWindowSize(DWORD dwBackBufferWidth, DWORD dwBackBufferHeight);

public:
	UINT64 Fence();
	void WaitForFenceValue(UINT64 ExpectedFenceValue);

public:
	// 빠른 시일 내에 렌더링부 분리
	BOOL BeginRender();
	BOOL EndRender();
	BOOL Present();

private:
	void CreateDescriptorHeapForRTV();
	void CreateDescriptorHeapForDSV();
	void CreateDepthStencilView(UINT width, UINT height);
	void CreateFence();
	void CreateMainCamera();

public:
	// Getter, Setter
	ComPtr<ID3D12Device5>& GetDevice() { return m_pD3DDevice; }
	ComPtr<IDXGISwapChain3>& GetSwapChain() { return m_pSwapChain; }
	ComPtr<ID3D12Resource>& GetCurrentRenderTarget() { return m_pRenderTargets[m_uiRenderTargetIndex]; }
	ComPtr<ID3D12CommandQueue>& GetCommandQueue() { return m_pCommandQueue; }

	ComPtr<ID3D12DescriptorHeap>& GetRTVHeap() { return m_pRTVHeap; }
	ComPtr<ID3D12DescriptorHeap>& GetDSVHeap() { return m_pDSVHeap; }
	UINT GetRTVDescriptorSize() { return m_uiRTVDescriptorSize; }
	UINT GetDSVDescriptorSize() { return m_uiDSVDescriptorSize; }

	DWORD GetCurrentContextIndex() { return m_dwCurContextIndex; }
	void SetCurrentContextIndex(DWORD index) { m_dwCurContextIndex = index; }
	
	UINT GetCurrentRenderTargetIndex() { return m_uiRenderTargetIndex; }
	void SetCurrentRenderTargetIndex(DWORD index) { m_uiRenderTargetIndex = index; }

	D3D12_VIEWPORT& GetViewport() { return m_Viewport; }
	D3D12_RECT& GetScissorRect() { return m_ScissorRect; }

	UINT64 GetFenceValue(DWORD contextIndex)
	{
		if (contextIndex >= MAX_PENDING_FRAME_COUNT)
			__debugbreak();

		return m_ui64LastFenceValues[contextIndex];
	}

	CBCameraData GetMainCameraCBData();

	std::shared_ptr<class Camera> GetMainCamera() { return m_pMainCamera; }

private:
	// Device
	ComPtr<ID3D12Device5> m_pD3DDevice = nullptr;

	// Command Queue
	ComPtr<ID3D12CommandQueue> m_pCommandQueue = nullptr;

	// Command List 와 Allocator 는 추후 RenderManager 같은곳에 구현예정ㅇㅇ

	// DXGI
	D3D_FEATURE_LEVEL m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	DXGI_ADAPTER_DESC1 m_AdapterDesc = {};
	ComPtr<IDXGISwapChain3> m_pSwapChain = nullptr;
	DWORD m_dwSwapChanFlags = 0;

	// Resources : RTV, DSV
	ComPtr<ID3D12Resource>			m_pRenderTargets[SWAP_CHAIN_FRAME_COUNT] = {};
	ComPtr<ID3D12Resource>			m_pDepthStencil = nullptr;
	ComPtr<ID3D12DescriptorHeap>	m_pRTVHeap = nullptr;
	ComPtr<ID3D12DescriptorHeap>	m_pDSVHeap = nullptr;
	UINT	m_uiRTVDescriptorSize = 0;
	UINT	m_uiDSVDescriptorSize = 0;
	UINT	m_uiRenderTargetIndex = 0;
	DWORD	m_dwCurContextIndex = 0;

	// Fence
	ComPtr<ID3D12Fence> m_pFence = nullptr;
	HANDLE m_hFenceEvent = nullptr;
	UINT64 m_ui64LastFenceValues[MAX_PENDING_FRAME_COUNT] = {};
	UINT64 m_ui64FenceValue = 0;

	// Viewport, Rect
	D3D12_VIEWPORT	m_Viewport = {};
	D3D12_RECT		m_ScissorRect = {};

	std::shared_ptr<class Camera> m_pMainCamera;
};

