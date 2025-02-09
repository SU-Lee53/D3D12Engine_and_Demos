#include "stdafx.h"
#include "AssimpConverter.h"

using namespace std;
namespace fs = std::filesystem;

AssimpConverter::AssimpConverter()
{
}

AssimpConverter::~AssimpConverter()
{
}

BOOL AssimpConverter::Initialize(const std::wstring& wstrPath)
{
	m_pImporter = make_shared<Assimp::Importer>();
	m_wstrFilePath = wstrPath;

	fs::path p(wstrPath);
	if (!fs::exists(p))
	{
		OutputDebugStringA("File is invalid\n");
		__debugbreak();
		return FALSE;
	}

	string strPath(wstrPath.begin(), wstrPath.end());

	m_rpaiScene = m_pImporter->ReadFile(strPath,
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	if (m_rpaiScene == nullptr)
	{
		OutputDebugStringA("Scene is not imported\n");
		__debugbreak();
		return FALSE;
	}

	m_rpRootNode = m_rpaiScene->mRootNode;

	return TRUE;
}

void AssimpConverter::PrintModelDataToImGui()
{
	PrintNodeInfo(m_rpRootNode);
	for (int i = 0; i < m_rpRootNode->mNumChildren; i++)
	{
		PrintNodeInfo(m_rpRootNode->mChildren[i]);
	}
}

void AssimpConverter::PrintNodeInfo(aiNode* node)
{
	ImGui::Text("Node name : %s", node->mName.C_Str());

	if (node->mNumChildren > 0)
	{
		for (int i = 0; i < node->mNumChildren; i++)
		{
			PrintNodeInfo(node->mChildren[i]);
		}
	}
}
