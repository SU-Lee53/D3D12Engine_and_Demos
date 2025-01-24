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

	BOOL CreateDefaultRasterizerState();
	BOOL CreateDefaultBlendState();
	BOOL CreateDefaultSamplerState();

public:
	void RenderBegin();
	void RenderEnd();

public:
	ComPtr<ID3D11Device>& GetDevice() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext>& GetDeviceContext() { return m_pDeviceContext; }

	HWND GetHwnd() { return m_hWnd; }

	UINT GetScreenWidth() { return m_uiScreenWidth; }
	UINT GetScreenHeight() { return m_uiScreenHeight; }

	ComPtr<ID3D11RasterizerState>& GetDefaultRasterizerState() { return m_pRasterizerStateDefault; }
	ComPtr<ID3D11BlendState>& GetDefaultBlendState() { return m_pBlendStateDefault; }
	ComPtr<ID3D11SamplerState>& GetDefaultSamplerState() { return m_pSamplerStateDefault; }
	

private:
	ComPtr<ID3D11Device>		m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
	
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;

	// RTV
	ComPtr<ID3D11RenderTargetView> m_pRTV;

	// DSV
	ComPtr<ID3D11DepthStencilView>	m_pDSV;
	ComPtr<ID3D11Texture2D>			m_pDSVTexture;

	// Default Rasterizer, Blend, SamplerState
	ComPtr<ID3D11RasterizerState>	m_pRasterizerStateDefault = nullptr;
	ComPtr<ID3D11BlendState>		m_pBlendStateDefault = nullptr;
	ComPtr<ID3D11SamplerState>		m_pSamplerStateDefault = nullptr;

	D3D11_VIEWPORT	m_Viewport = {};
	D3D11_RECT		m_ScissorRect = {};

	UINT m_uiScreenWidth = 0;
	UINT m_uiScreenHeight = 0;

	HWND m_hWnd = nullptr;
};

