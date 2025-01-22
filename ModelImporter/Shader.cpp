#include "stdafx.h"
#include "Shader.h"

using namespace std;

Shader::Shader()
{
}

Shader::~Shader()
{
}

BOOL Shader::Initialize(const std::wstring& wstrfilePath, const std::string& wstrVSEntryName, const std::string& wstrPSEntryName)
{
	ComPtr<ID3DBlob> pVSByteCode;
	ComPtr<ID3DBlob> pPSByteCode;
	ComPtr<ID3DBlob> pError;

	string strVSVersion = "vs_5_0"s;
	string strPSVersion = "ps_5_0"s;

	const UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;


	// Vertex Shader Compile
	if (FAILED(::D3DCompileFromFile(
			wstrfilePath.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			wstrVSEntryName.c_str(),
			strVSVersion.c_str(),
			compileFlag,
			0,
			pVSByteCode.GetAddressOf(),
			pError.GetAddressOf()
	)))
	{
		__debugbreak();
	}
	
	// Pixel Shader Compile
	if (FAILED(::D3DCompileFromFile(
			wstrfilePath.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			wstrPSEntryName.c_str(),
			strPSVersion.c_str(),
			compileFlag,
			0,
			pPSByteCode.GetAddressOf(),
			pError.GetAddressOf()
	)))
	{
		__debugbreak();
	}
	
	// Create Vertex Shader
	if (FAILED(DEVICE->CreateVertexShader(pVSByteCode->GetBufferPointer(), pVSByteCode->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf())))
	{
		__debugbreak();
	}

	// Create Pixel Shader
	if (FAILED(DEVICE->CreatePixelShader(pPSByteCode->GetBufferPointer(), pPSByteCode->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf())))
	{
		__debugbreak();
	}

	return TRUE;
}

BOOL Shader::InitializeAsDefault()
{
	return Initialize(L"./Shader/ImporterShader.hlsl", "VSMain"s, "PSMain"s);
}
