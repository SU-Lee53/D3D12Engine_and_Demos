#pragma once

class Shader
{
public:
	Shader();
	~Shader();

public:
	BOOL Initialize(const std::wstring& wstrfilePath, const std::string& wstrVSEntryName, const std::string& wstrPSEntryName);

public:
	BOOL InitializeAsDefault();

public:
	// Getter
	ComPtr<ID3D11VertexShader>& GetVertexShader() { return m_pVertexShader; }
	ComPtr<ID3D11PixelShader>&	GetPixelShader() { return m_pPixelShader; }
	ComPtr<ID3D11InputLayout>&	GetInputLayout() { return m_pInputLayout; }

private:
	ComPtr<ID3D11VertexShader>	m_pVertexShader = nullptr;
	ComPtr<ID3D11PixelShader>	m_pPixelShader = nullptr;
	ComPtr<ID3D11InputLayout>	m_pInputLayout = nullptr;

};

