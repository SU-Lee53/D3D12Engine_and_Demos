#pragma once

#include <d3dcompiler.h>

// 나중에 Mesh Shader 가 필요하다면 추가
enum SHADER_TYPE
{
	SHADER_TYPE_VERTEX = 0,
	SHADER_TYPE_GEOMETRY,
	SHADER_TYPE_HULL,
	SHADER_TYPE_DOMAIN,
	SHADER_TYPE_PIXEL,
	SHADER_TYPE_COMPUTE,
	SHADER_TYPE_COUNT
};

template <SHADER_TYPE ty>
class Shader
{
public:
	Shader();
	~Shader();

	void Initialize(const std::wstring& filePath, const std::string& entryName);

public:
	ComPtr<ID3DBlob>& GetBlob() { return m_ShaderByteCode; }

private:
	ComPtr<ID3DBlob> m_ShaderByteCode = nullptr;

	std::string m_strFileName = "";
	std::string m_strEntryName = "";
	std::wstring m_wstrFilePath = L"";
};

template<SHADER_TYPE ty>
inline Shader<ty>::Shader()
{
}

template<SHADER_TYPE ty>
inline Shader<ty>::~Shader()
{
}

template<SHADER_TYPE ty>
inline void Shader<ty>::Initialize(const std::wstring& filePath, const std::string& entryName)
{
	m_strEntryName = entryName;
	m_wstrFilePath = filePath;
	
	std::filesystem::path p(filePath);
	m_strFileName = p.filename().string();


	// Set shader target version for D3DCompileFromFile
	std::string shaderVersion;
	switch (ty)
	{
	case SHADER_TYPE_VERTEX:
		shaderVersion = "vs_5_0";
		break;
	case SHADER_TYPE_GEOMETRY:
		shaderVersion = "gs_5_0";
		break;
	case SHADER_TYPE_HULL:
		shaderVersion = "hs_5_0";
		break;
	case SHADER_TYPE_DOMAIN:
		shaderVersion = "ds_5_0";
		break;
	case SHADER_TYPE_PIXEL:
		shaderVersion = "ps_5_0";
		break;
	case SHADER_TYPE_COMPUTE:
		shaderVersion = "cs_5_0";
		break;
	default:
		__debugbreak();
	}

#ifdef _DEBUG
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif
	ComPtr<ID3DBlob> pError;

	HRESULT hr = D3DCompileFromFile(m_wstrFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_strEntryName.c_str(), shaderVersion.c_str(), compileFlags, 0, m_ShaderByteCode.GetAddressOf(), pError.GetAddressOf());

	if (FAILED(hr))
	{
		if (pError)
		{
			OutputDebugStringA((char*)pError->GetBufferPointer());
			__debugbreak();
		}
	}

}

using VertexShader = Shader<SHADER_TYPE_VERTEX>;
using GeometryShader = Shader<SHADER_TYPE_GEOMETRY>;
using HullShader = Shader<SHADER_TYPE_HULL>;
using DomainShader = Shader<SHADER_TYPE_DOMAIN>;
using PixelShader = Shader<SHADER_TYPE_PIXEL>;
using ComputeShader = Shader<SHADER_TYPE_COMPUTE>;

template<typename C>
concept ShaderType = requires (C c)
{
	std::same_as<C, VertexShader> ||
		std::same_as<C, GeometryShader> ||
		std::same_as<C, HullShader> ||
		std::same_as<C, DomainShader> ||
		std::same_as<C, PixelShader> ||
		std::same_as<C, ComputeShader>;

};
