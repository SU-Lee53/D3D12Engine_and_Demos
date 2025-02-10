#pragma once

class AssimpConverter
{
public:
	AssimpConverter();
	~AssimpConverter();

public:
	BOOL Initialize(const std::wstring& wstrPath);

#pragma region PRINT_DATAS
public:
	void PrintModelDataToImGui();

private:
	void PrintNodeInfo(aiNode* pNode);


#pragma endregion PRINT_DATAS

#pragma region EXPORT_TO_MODEL

public:
	void ExportMeshInConverterToModel();

private:
	void ExportNode();
	void ExportMesh();
	void ExportMaterial();

#pragma endregion EXPORT_TO_MODEL


public:
	void PrintTabs();
	BOOL IsNodeHelperNode(aiNode* pNode)
	{
		std::string strNodeName = pNode->mName.C_Str();

		if (strNodeName.find("AssimpFbx") == std::string::npos ||
			pNode->mNumMeshes > 0)
			return FALSE;

		return TRUE;
	}

private:
	std::shared_ptr<Assimp::Importer> m_pImporter = nullptr;
	const aiScene* m_rpScene = nullptr;
	aiNode* m_rpRootNode = nullptr;

	std::wstring m_wstrFilePath = L"";

	UINT m_tabs = 0;
};

