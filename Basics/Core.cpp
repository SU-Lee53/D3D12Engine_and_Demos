#include "pch.h"
#include "Core.h"
#include "Utils.h"
#include "Camera.h"

BOOL Core::Initialize(BOOL bEnableDebugLayer, BOOL bEnableGBV)
{
    ComPtr<ID3D12Debug> pDebugController;
    ComPtr<IDXGIFactory4> pFactory;
    ComPtr<IDXGIAdapter1> pAdapter = nullptr;
    DXGI_ADAPTER_DESC1 AdapterDesc = {};

    DWORD dwCreateFlags = 0;
    DWORD dwCreateFactoryFlags = 0;

    // Debug Layer 설정
    if (bEnableDebugLayer)
    {
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController))))
        {
            pDebugController->EnableDebugLayer();
        }
        
        dwCreateFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
        if (bEnableGBV)
        {
            ComPtr<ID3D12Debug5> pDebugController5;
            if (SUCCEEDED(pDebugController->QueryInterface(IID_PPV_ARGS(&pDebugController5))))
            {
                pDebugController5->SetEnableGPUBasedValidation(TRUE);
                pDebugController5->SetEnableAutoName(TRUE);
            }
        }
        
    }

    // DXGI Factory
    HR_ASSERT(::CreateDXGIFactory2(dwCreateFactoryFlags, IID_PPV_ARGS(&pFactory)));
    
    // 가장 메모리가 많은 GPU 를 Adapter 로 선택
    size_t bestMemory = 0;
    for (DWORD adapterIndex = 0; ; adapterIndex++)
    {
        ComPtr<IDXGIAdapter1> pCurAdapter = nullptr;
        if (pFactory->EnumAdapters1(adapterIndex, pCurAdapter.GetAddressOf()) == DXGI_ERROR_NOT_FOUND)
            break;

        DXGI_ADAPTER_DESC1 CurAdapterDesc;
        pCurAdapter->GetDesc1(&CurAdapterDesc);

        size_t memory = CurAdapterDesc.DedicatedVideoMemory / (1024 * 1024);

        if (memory > bestMemory)
        {
            bestMemory = memory;
            pAdapter = pCurAdapter;
            AdapterDesc = CurAdapterDesc;
        }
    }

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_2,
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };

    DWORD FeatureLevelNum = _countof(featureLevels);

    // Device 생성
    for (DWORD featureLevelIndex = 0; featureLevelIndex < FeatureLevelNum; featureLevelIndex++)
    {
        if (pAdapter)
        {
            HR_ASSERT(::D3D12CreateDevice(pAdapter.Get(), featureLevels[featureLevelIndex], IID_PPV_ARGS(m_pD3DDevice.GetAddressOf())));
            m_FeatureLevel = featureLevels[featureLevelIndex];
            pAdapter->GetDesc1(&m_AdapterDesc);
            break;
        }
    }

    if (!m_pD3DDevice)
        __debugbreak();

    if (pDebugController)
    {
        Utils::SetDebugLayerInfo(m_pD3DDevice);
    }

    // Command Queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    HR_ASSERT(
        m_pD3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_pCommandQueue.GetAddressOf()))
    );

    
    // SwapChain
    HWND hWnd = GAME.GetGameDesc().hWnd;

    RECT rect;
    ::GetClientRect(hWnd, &rect);
    DWORD dwWndWidth = rect.right - rect.left;
    DWORD dwWndHeight = rect.bottom - rect.top;
    DWORD dwBackBufferWidth = dwWndWidth;
    DWORD dwBackBufferHeight = dwWndHeight;
    {
        DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
        {
            SwapChainDesc.Width = dwBackBufferWidth;
            SwapChainDesc.Height = dwBackBufferHeight;
            SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            SwapChainDesc.BufferCount = SWAP_CHAIN_FRAME_COUNT;
            SwapChainDesc.SampleDesc.Count = 1;
            SwapChainDesc.SampleDesc.Quality = 0;
            SwapChainDesc.Scaling = DXGI_SCALING_NONE;
            SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
            SwapChainDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
            m_dwSwapChanFlags = SwapChainDesc.Flags;
        }

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        ComPtr<IDXGISwapChain1> pSwapChain1;
        HR_ASSERT(pFactory->CreateSwapChainForHwnd(m_pCommandQueue.Get(), hWnd, &SwapChainDesc, &fsSwapChainDesc, nullptr, pSwapChain1.GetAddressOf()));

        HR_ASSERT(pSwapChain1->QueryInterface(IID_PPV_ARGS(m_pSwapChain.GetAddressOf())));
        m_uiRenderTargetIndex = m_pSwapChain->GetCurrentBackBufferIndex();
    }

    // RTVHeap 에 SwapChain 의 BackBuffer 연결
    CreateDescriptorHeapForRTV();
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRTVHeap->GetCPUDescriptorHandleForHeapStart());

    for (UINT n = 0; n < SWAP_CHAIN_FRAME_COUNT; n++)
    {
        m_pSwapChain->GetBuffer(n, IID_PPV_ARGS(m_pRenderTargets[n].GetAddressOf()));
        m_pD3DDevice->CreateRenderTargetView(m_pRenderTargets[n].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, m_uiRTVDescriptorSize);
    }

    // Depth Stencil Buffer 생성
    CreateDescriptorHeapForDSV();
    CreateDepthStencilView(dwWndWidth, dwWndHeight);

    // Viewport, Rect
    m_Viewport.Width = (float)dwWndWidth;
    m_Viewport.Height = (float)dwWndHeight;
    m_Viewport.MinDepth = 0.f;
    m_Viewport.MaxDepth = 1.f;

    m_ScissorRect.left = 0;
    m_ScissorRect.top = 0;
    m_ScissorRect.right = dwWndWidth;
    m_ScissorRect.bottom = dwWndHeight;

    // Fence
    CreateFence();

    // Main Camera
    CreateMainCamera();

    // TODO : Commamd List, Allocator 는 Render로 뺄지 말지 결정하고 생성

    return TRUE;
}

BOOL Core::UpdateWindowSize(DWORD dwBackBufferWidth, DWORD dwBackBufferHeight)
{
    if (!(dwBackBufferWidth * dwBackBufferHeight))
        return FALSE;

    DWORD dwWidth = GAME.GetGameDesc().width;
    DWORD dwHeight = GAME.GetGameDesc().height;

    if (dwWidth == dwBackBufferWidth && dwHeight == dwBackBufferHeight)
        return FALSE;

    Fence();

    for (DWORD i = 0; i < MAX_PENDING_FRAME_COUNT; i++)
    {
        WaitForFenceValue(m_ui64LastFenceValues[i]);
    }

    DXGI_SWAP_CHAIN_DESC1 desc;
    HR_ASSERT(m_pSwapChain->GetDesc1(&desc));

    for (UINT n = 0; n < SWAP_CHAIN_FRAME_COUNT; n++)
    {
        m_pRenderTargets[n].Reset();
    }

    if (m_pDepthStencil)
    {
        m_pDepthStencil.Reset();
    }

    HR_ASSERT(
        m_pSwapChain->ResizeBuffers(SWAP_CHAIN_FRAME_COUNT, dwBackBufferWidth, dwBackBufferHeight, DXGI_FORMAT_R8G8B8A8_UNORM, m_dwSwapChanFlags)
    );

    m_uiRenderTargetIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRTVHeap->GetCPUDescriptorHandleForHeapStart());

    for (UINT n = 0; n < SWAP_CHAIN_FRAME_COUNT; n++)
    {
        m_pSwapChain->GetBuffer(n, IID_PPV_ARGS(m_pRenderTargets[n].GetAddressOf()));
        m_pD3DDevice->CreateRenderTargetView(m_pRenderTargets[n].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(m_uiRTVDescriptorSize);
    }

    CreateDepthStencilView(dwBackBufferWidth, dwBackBufferHeight);

    GAME.UpdateWindowSize(dwBackBufferWidth, dwBackBufferHeight);
    m_Viewport.Width = (float)dwBackBufferWidth;
    m_Viewport.Height = (float)dwBackBufferHeight;
    m_ScissorRect.left = 0;
    m_ScissorRect.top = 0;
    m_ScissorRect.right = dwBackBufferWidth;
    m_ScissorRect.bottom = dwBackBufferHeight;

    // TODO : Reset Camera

    return FALSE;
}

UINT64 Core::Fence()
{
    m_ui64FenceValue++;
    m_pCommandQueue->Signal(m_pFence.Get(), m_ui64FenceValue);
    m_ui64LastFenceValues[m_dwCurContextIndex] = m_ui64FenceValue;
    return m_ui64FenceValue;
}

void Core::WaitForFenceValue(UINT64 ExpectedFenceValue)
{
    if (m_pFence->GetCompletedValue() < ExpectedFenceValue)
    {
        m_pFence->SetEventOnCompletion(ExpectedFenceValue, m_hFenceEvent);
        WaitForSingleObject(m_hFenceEvent, INFINITE);
    }
}

BOOL Core::BeginRender()
{
    return 0;
}

BOOL Core::EndRender()
{
    return 0;
}

BOOL Core::Present()
{
    return 0;
}

void Core::CreateDescriptorHeapForRTV()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    {
        rtvHeapDesc.NumDescriptors = SWAP_CHAIN_FRAME_COUNT;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    }
    HR_ASSERT(m_pD3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_pRTVHeap.GetAddressOf())));
    m_uiRTVDescriptorSize = m_pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

void Core::CreateDescriptorHeapForDSV()
{
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    {
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    }
    HR_ASSERT(m_pD3DDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(m_pDSVHeap.GetAddressOf())));
    m_uiDSVDescriptorSize = m_pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void Core::CreateDepthStencilView(UINT width, UINT height)
{
    // 추후 필요시 DESC1, 2로 업그레이드
    D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
    depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;    // Stencil 버퍼를 지금은 사용 안함
    depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

    // Clear Value 설정
    D3D12_CLEAR_VALUE depthOptimizeClearValue = {};
    depthOptimizeClearValue.Format = DXGI_FORMAT_D32_FLOAT;
    depthOptimizeClearValue.DepthStencil.Depth = 1.f;
    depthOptimizeClearValue.DepthStencil.Stencil = 0;

    D3D12_RESOURCE_DESC depthDesc;
    depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthDesc.Alignment = 0;
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.DepthOrArraySize = 1;
    depthDesc.MipLevels = 1;
    depthDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    HR_ASSERT(
        m_pD3DDevice->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &depthDesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &depthOptimizeClearValue,
            IID_PPV_ARGS(m_pDepthStencil.GetAddressOf())
        ));

    m_pDepthStencil->SetName(L"Core::m_pDepthStencil");

    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_pDSVHeap->GetCPUDescriptorHandleForHeapStart());
    m_pD3DDevice->CreateDepthStencilView(m_pDepthStencil.Get(), &depthStencilDesc, dsvHandle);

}

void Core::CreateFence()
{
    HR_ASSERT(
        m_pD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_pFence.GetAddressOf()))
    );

    m_hFenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

}

void Core::CreateMainCamera()
{
    m_pMainCamera = std::make_shared<Camera>();
    m_pMainCamera->Initialize();
}

CBCameraData Core::GetMainCameraData()
{
    return m_pMainCamera->GetCameraCBData();
}
