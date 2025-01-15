#pragma once

class D3D11Core
{
	DECLARE_SINGLE(D3D11Core);

public:
	BOOL Initialize(HWND hWnd, UINT windowWidth, UINT windowHeight);
	BOOL UpdateWindowSize(UINT windowWidth, UINT windowHeight);
	
private:
	BOOL CreateDeviceAndSwapChain();
	BOOL CreateRTV();
	BOOL CreateDSV();

public:
	void RenderBegin();
	void RenderEnd();

public:
	ComPtr<ID3D11Device>& GetDevice() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext>& GetDeviceContext() { return m_pDeviceContext; }

	HWND GetHwnd() { return m_hWnd; }

private:
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
	
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;

	// RTV
	ComPtr<ID3D11RenderTargetView> m_pRTV;

	// DSV
	ComPtr<ID3D11Texture2D> m_pDSVTexture;
	ComPtr<ID3D11DepthStencilView> m_pDSV;

	D3D11_VIEWPORT	m_Viewport = {};
	D3D11_RECT		m_ScissorRect = {};

	UINT m_uiScreenWidth = 0;
	UINT m_uiScreenHeight = 0;

	HWND m_hWnd = nullptr;
};

