#include "pch.h"
#include "ShaderManager.h"

using namespace std;

BOOL ShaderManager::Initialize()
{
	// ����� ���߿� �ܺ� ���Ϸ� ����ȭ���� ������ �ϴ°� ������
	std::vector<SHADER_COMPILE_TARGET_DESC> descs =
	{
		{SHADER_TYPE_VERTEX, L"../Shader/test.hlsl", "VS_Main", "testVS"},
		{SHADER_TYPE_PIXEL, L"../Shader/test.hlsl", "PS_Main", "testPS"}
	};

	for (const auto& desc : descs)
	{
		switch (desc.ShaderType)
		{
		case SHADER_TYPE_VERTEX:
			CompileAndAddShader<VertexShader>(desc.key, desc.wstrFilePath, desc.strEntryName);
			break;
		case SHADER_TYPE_GEOMETRY:
			CompileAndAddShader<GeometryShader>(desc.key, desc.wstrFilePath, desc.strEntryName);
			break;
		case SHADER_TYPE_HULL:
			CompileAndAddShader<HullShader>(desc.key, desc.wstrFilePath, desc.strEntryName);
			break;
		case SHADER_TYPE_DOMAIN:
			CompileAndAddShader<DomainShader>(desc.key, desc.wstrFilePath, desc.strEntryName);
			break;
		case SHADER_TYPE_PIXEL:
			CompileAndAddShader<PixelShader>(desc.key, desc.wstrFilePath, desc.strEntryName);
			break;
		case SHADER_TYPE_COMPUTE:
			CompileAndAddShader<ComputeShader>(desc.key, desc.wstrFilePath, desc.strEntryName);
			break;
		default:
			__debugbreak();
		}
	}

	return TRUE;
}

shared_ptr<VertexShader> ShaderManager::GetVertexShader(const string& key)
{
	auto shader = m_vsMap.find(key);

	if (shader == m_vsMap.end())
		__debugbreak();

	return shader->second;
}

shared_ptr<GeometryShader> ShaderManager::GetGeometryShader(const string& key)
{
	auto shader = m_gsMap.find(key);

	if (shader == m_gsMap.end())
		__debugbreak();

	return shader->second;
}

shared_ptr<HullShader> ShaderManager::GetHullShader(const string& key)
{
	auto shader = m_hsMap.find(key);

	if (shader == m_hsMap.end())
		__debugbreak();

	return shader->second;
}

shared_ptr<DomainShader> ShaderManager::GetDomainShader(const string& key)
{
	auto shader = m_dsMap.find(key);

	if (shader == m_dsMap.end())
		__debugbreak();

	return shader->second;
}

shared_ptr<PixelShader> ShaderManager::GetPixelShader(const string& key)
{
	auto shader = m_psMap.find(key);

	if (shader == m_psMap.end())
		__debugbreak();

	return shader->second;
}

shared_ptr<ComputeShader> ShaderManager::GetComputeShader(const string& key)
{
	auto shader = m_csMap.find(key);

	if (shader == m_csMap.end())
		__debugbreak();

	return shader->second;
}

// ���Ž� ��ü �Ҹ��� ������ ���� ���ͼ� �������
// ShaderManager ���� ��ü�� ����� �������ֹǷ� ��� �����پ����� ������ ref count �� 1�� ���� ��
// ���� ref count �� 1�� �ƴ� ��ü�� ����� �ϸ� �ȵ�
// ���⼭ key �� ������� ��ü�� ��ã���� �װ� �� �Ѿ(�ܼ� ������ �ѹ� ���°� ������)
void ShaderManager::RemoveVertexShader(const string& key)
{
	auto target = m_vsMap.find(key);

	// ref count �� 1�� �ƴϴ� -> ��𼱰� �����ִ�
	if (target->second.use_count() != 1)
		__debugbreak();

	m_vsMap.erase(target);
}

void ShaderManager::RemoveGeometryShader(const string& key)
{
	auto target = m_gsMap.find(key);

	// ref count �� 1�� �ƴϴ� -> ��𼱰� �����ִ�
	if (target->second.use_count() != 1)
		__debugbreak();

	m_gsMap.erase(target);
}

void ShaderManager::RemoveHullShader(const string& key)
{
	auto target = m_hsMap.find(key);

	// ref count �� 1�� �ƴϴ� -> ��𼱰� �����ִ�
	if (target->second.use_count() != 1)
		__debugbreak();

	m_hsMap.erase(target);
}

void ShaderManager::RemoveDomainShader(const string& key)
{
	auto target = m_dsMap.find(key);

	// ref count �� 1�� �ƴϴ� -> ��𼱰� �����ִ�
	if (target->second.use_count() != 1)
		__debugbreak();

	m_dsMap.erase(target);
}

void ShaderManager::RemovePixelShader(const string& key)
{
	auto target = m_psMap.find(key);

	// ref count �� 1�� �ƴϴ� -> ��𼱰� �����ִ�
	if (target->second.use_count() != 1)
		__debugbreak();

	m_psMap.erase(target);
}

void ShaderManager::RemoveComputeShader(const string& key)
{
	auto target = m_csMap.find(key);

	// ref count �� 1�� �ƴϴ� -> ��𼱰� �����ִ�
	if (target->second.use_count() != 1)
		__debugbreak();

	m_csMap.erase(target);
}

void ShaderManager::AddVertexShader(const string& key, shared_ptr<VertexShader> shader)
{
	m_vsMap.insert({ key, shader });
}

void ShaderManager::AddGeometryShader(const string& key, shared_ptr<GeometryShader> shader)
{
	m_gsMap.insert({ key, shader });
}

void ShaderManager::AddHullShader(const string& key, shared_ptr<HullShader> shader)
{
	m_hsMap.insert({ key, shader });
}

void ShaderManager::AddDomainShader(const string& key, shared_ptr<DomainShader> shader)
{
	m_dsMap.insert({ key, shader });
}

void ShaderManager::AddPixelShader(const string& key, shared_ptr<PixelShader> shader)
{
	m_psMap.insert({ key, shader });
}

void ShaderManager::AddComputeShader(const string& key, shared_ptr<ComputeShader> shader)
{
	m_csMap.insert({ key, shader });
}
