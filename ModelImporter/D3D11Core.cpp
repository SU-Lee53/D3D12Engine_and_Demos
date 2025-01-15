#include "stdafx.h"
#include "D3D11Core.h"

BOOL D3D11Core::Initialize(HWND hWnd, UINT windowWidth, UINT windowHeight)
{
	m_hWnd = hWnd;
	m_uiScreenWidth = windowWidth;
	m_uiScreenHeight = windowHeight;
	CreateDeviceAndSwapChain();
	CreateRTV();
	CreateDSV();

	m_Viewport.Width = (float)m_uiScreenWidth;
	m_Viewport.Height = (float)m_uiScreenHeight;
	m_Viewport.MinDepth = 0.f;
	m_Viewport.MaxDepth = 1.f;

	m_ScissorRect.left = 0;
	m_ScissorRect.top = 0;
	m_ScissorRect.right = m_uiScreenWidth;
	m_ScissorRect.bottom = m_uiScreenHeight;

	return TRUE;
}

BOOL D3D11Core::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	memset(&desc, 0, sizeof(desc));
	{
		desc.BufferDesc.Width = m_uiScreenWidth;
		desc.BufferDesc.Height = m_uiScreenHeight;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = m_hWnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		m_pSwapChain.GetAddressOf(),
		m_pDevice.GetAddressOf(),
		nullptr,
		m_pDeviceContext.GetAddressOf()
	);

	HR_ASSERT(hr);

	return TRUE;
}

BOOL D3D11Core::CreateRTV()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	HR_ASSERT(hr);

	hr = m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRTV.GetAddressOf());
	HR_ASSERT(hr);

	return TRUE;
}

BOOL D3D11Core::CreateDSV()
{
	{
		D3D11_TEXTURE2D_DESC desc;
		memset(&desc, 0, sizeof(desc));

		desc.Width = m_uiScreenWidth;
		desc.Height = m_uiScreenHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		HRESULT hr = m_pDevice->CreateTexture2D(
			&desc,
			nullptr,
			m_pDSVTexture.GetAddressOf()
		);
		HR_ASSERT(hr);
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		memset(&desc, 0, sizeof(desc));

		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = m_pDevice->CreateDepthStencilView(
			m_pDSVTexture.Get(),
			&desc,
			m_pDSV.GetAddressOf()
		);
		HR_ASSERT(hr);
	}

	return TRUE;
}

void D3D11Core::RenderBegin()
{
	XMFLOAT4 clearColor(Colors::Black);

	m_pDeviceContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSV.Get());
	m_pDeviceContext->ClearRenderTargetView(m_pRTV.Get(), (float*)&clearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);
	m_pDeviceContext->RSSetScissorRects(1, &m_ScissorRect);
}

void D3D11Core::RenderEnd()
{
	HRESULT hr = m_pSwapChain->Present(1, 0);
	HR_ASSERT(hr);
}

BOOL D3D11Core::UpdateWindowSize(UINT windowWidth, UINT windowHeight)
{
	if (!(windowWidth * windowHeight))
		return FALSE;

	if (m_uiScreenWidth == windowWidth and m_uiScreenHeight == windowHeight)
		return FALSE;

	if (m_pSwapChain)
	{
		m_pDeviceContext->OMSetRenderTargets(0, 0, 0);

		// Release all outstanding references to the swap chain's buffers.
		m_pRTV.Reset();

		HRESULT hr;
		// Preserve the existing buffer count and format.
		// Automatically choose the width and height to match the client rect for HWNDs.
		hr = m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		// Perform error handling here!

		// Get buffer and create a render-target-view.
		ID3D11Texture2D* pBuffer;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(void**)&pBuffer);
		// Perform error handling here!

		hr = m_pDevice->CreateRenderTargetView(pBuffer, NULL,
			&m_pRTV);
		// Perform error handling here!
		pBuffer->Release();

		m_pDeviceContext->OMSetRenderTargets(1, &m_pRTV, NULL);

		// Set up the viewport.
		m_Viewport.Width = windowWidth;
		m_Viewport.Height = windowHeight;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_pDeviceContext->RSSetViewports(1, &m_Viewport);

		m_ScissorRect.left = 0;
		m_ScissorRect.top = 0;
		m_ScissorRect.right = windowWidth;
		m_ScissorRect.bottom = windowHeight;
		m_pDeviceContext->RSSetScissorRects(1, &m_ScissorRect);

		m_uiScreenWidth = windowWidth;
		m_uiScreenHeight = windowHeight;
	}

}

