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

private:
	ComPtr<ID3D11VertexShader>	m_pVertexShader = nullptr;
	ComPtr<ID3D11PixelShader>	m_pPixelShader = nullptr;

};

