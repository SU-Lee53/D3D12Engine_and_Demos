#pragma once
#include "Shader.h"

struct SHADER_COMPILE_TARGET_DESC
{
	SHADER_TYPE ShaderType;
	std::wstring wstrFilePath;
	std::string strEntryName;
	std::string key;
};

// Shader 들을 미리 컴파일하고 저장, 필요할때 꺼내씀
class ShaderManager
{
	DECLARE_SINGLE(ShaderManager)

public:
	BOOL Initialize();

public:
	template<ShaderType T>
	void CompileAndAddShader(const std::string& key, const std::wstring& filePath, const std::string& entryName);

	template<ShaderType T>
	std::shared_ptr<T> GetShader(const std::string& key);

	template<ShaderType T>
	void RemoveShader(const std::string& key);



private:
	// To prevent ref count problem, Allocate Shader Object in Add functions
	// This will make shaders ref count to 1 in shadermaps 
	void AddVertexShader(const std::string& key, std::shared_ptr<VertexShader> shader);
	void AddGeometryShader(const std::string& key, std::shared_ptr<GeometryShader> shader);
	void AddHullShader(const std::string& key, std::shared_ptr<HullShader> shader);
	void AddDomainShader(const std::string& key, std::shared_ptr<DomainShader> shader);
	void AddPixelShader(const std::string& key, std::shared_ptr<PixelShader> shader);
	void AddComputeShader(const std::string& key, std::shared_ptr<ComputeShader> shader);

	std::shared_ptr<VertexShader> GetVertexShader(const std::string& key);
	std::shared_ptr<GeometryShader> GetGeometryShader(const std::string& key);
	std::shared_ptr<HullShader> GetHullShader(const std::string& key);
	std::shared_ptr<DomainShader> GetDomainShader(const std::string& key);
	std::shared_ptr<PixelShader> GetPixelShader(const std::string& key);
	std::shared_ptr<ComputeShader> GetComputeShader(const std::string& key);

	void RemoveVertexShader(const std::string& key);
	void RemoveGeometryShader(const std::string& key);
	void RemoveHullShader(const std::string& key);
	void RemoveDomainShader(const std::string& key);
	void RemovePixelShader(const std::string& key);
	void RemoveComputeShader(const std::string& key);
private:
	std::map<std::string, std::shared_ptr<VertexShader>> m_vsMap;
	std::map<std::string, std::shared_ptr<GeometryShader>> m_gsMap;
	std::map<std::string, std::shared_ptr<HullShader>> m_hsMap;
	std::map<std::string, std::shared_ptr<DomainShader>> m_dsMap;
	std::map<std::string, std::shared_ptr<PixelShader>> m_psMap;
	std::map<std::string, std::shared_ptr<ComputeShader>> m_csMap;


};

template <ShaderType T>
inline void ShaderManager::CompileAndAddShader(const std::string& key, const std::wstring& filePath, const std::string& entryName)
{
	if constexpr (std::is_same_v<T, VertexShader>)
	{
		std::shared_ptr<VertexShader> shader = std::make_shared<VertexShader>();
		shader->Initialize(filePath, entryName);
		AddVertexShader(key, shader);
	}
	else if constexpr (std::is_same_v<T, GeometryShader>)
	{
		std::shared_ptr<GeometryShader> shader = std::make_shared<GeometryShader>();
		shader->Initialize(filePath, entryName);
		AddGeometryShader(key, shader);
	}
	else if constexpr (std::is_same_v<T, HullShader>)
	{
		std::shared_ptr<HullShader> shader = std::make_shared<HullShader>();
		shader->Initialize(filePath, entryName);
		AddHullShader(key, shader);
	}
	else if constexpr (std::is_same_v<T, DomainShader>)
	{
		std::shared_ptr<DomainShader> shader = std::make_shared<DomainShader>();
		shader->Initialize(filePath, entryName);
		AddDomainShader(key, shader);
	}
	else if constexpr (std::is_same_v<T, PixelShader>)
	{
		std::shared_ptr<PixelShader> shader = std::make_shared<PixelShader>();
		shader->Initialize(filePath, entryName);
		AddPixelShader(key, shader);
	}
	else if constexpr (std::is_same_v<T, ComputeShader>)
	{
		std::shared_ptr<ComputeShader> shader = std::make_shared<ComputeShader>();
		shader->Initialize(filePath, entryName);
		AddComputeShader(key, shader);
	}

}

template<ShaderType T>
inline std::shared_ptr<T> ShaderManager::GetShader(const std::string& key)
{
	if constexpr (std::is_same_v<T, VertexShader>)
	{
		return GetVertexShader(key);
	}
	else if constexpr (std::is_same_v<T, GeometryShader>)
	{
		return GetGeometryShader(key);
	}
	else if constexpr (std::is_same_v<T, HullShader>)
	{
		return GetHullShader(key);
	}
	else if constexpr (std::is_same_v<T, DomainShader>)
	{
		return GetDomainShader(key);
	}
	else if constexpr (std::is_same_v<T, PixelShader>)
	{
		return GetPixelShader(key);
	}
	else if constexpr (std::is_same_v<T, ComputeShader>)
	{
		return GetComputeShader(key);
	}

	__debugbreak();
	return nullptr;
}

template<ShaderType T>
inline void ShaderManager::RemoveShader(const std::string& key)
{
	if constexpr (std::is_same_v<T, VertexShader>)
	{
		return RemoveVertexShader(key);
	}
	else if constexpr (std::is_same_v<T, GeometryShader>)
	{
		return RemoveGeometryShader(key);
	}
	else if constexpr (std::is_same_v<T, HullShader>)
	{
		return RemoveHullShader(key);
	}
	else if constexpr (std::is_same_v<T, DomainShader>)
	{
		return RemoveDomainShader(key);
	}
	else if constexpr (std::is_same_v<T, PixelShader>)
	{
		return RemovePixelShader(key);
	}
	else if constexpr (std::is_same_v<T, ComputeShader>)
	{
		return RemoveComputeShader(key);
	}
}
