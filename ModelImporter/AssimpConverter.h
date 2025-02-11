#pragma once

class Model;
struct ModelNode;

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
	void PrintTransformInfo(aiNode* pNode);
	void PrintMeshInfo(aiNode* pNode);
	void PrintMaterialInfo(aiNode* pNode);


#pragma endregion PRINT_DATAS

#pragma region EXPORT_TO_MODEL

public:
	void ExportMeshInConverterToModel(std::shared_ptr<Model> pOutModel);

private:
	void ExportNode(aiNode* pNode, std::shared_ptr<Model> pOutModel, int curindex, int parentIndex);
	BOOL ExportMesh(aiMesh* pMesh, std::shared_ptr<ModelNode> pOutModel);
	BOOL ExportTransform(aiNode* pNode, std::shared_ptr<ModelNode> pOutModel);
	BOOL ExportMaterial(aiMaterial* pMaterial, std::shared_ptr<ModelNode> pOutModel);

#pragma endregion EXPORT_TO_MODEL


public:
	// Utilities
	void PrintTabs();
	BOOL IsNodeHelperNode(aiNode* pNode)
	{
		std::string strNodeName = pNode->mName.C_Str();

		if (strNodeName.find("AssimpFbx") == std::string::npos ||
			pNode->mNumMeshes > 0)
			return FALSE;

		return TRUE;
	}

	std::string GetPropertyTypeToString(aiPropertyTypeInfo type)
	{
		std::string ret;

		switch (type)
		{
		case aiPTI_Float:
			ret = "aiPTI_Float"s;
			break;
		case aiPTI_Double:
			ret = "aiPTI_Double"s;
			break;
		case aiPTI_String:
			ret = "aiPTI_String"s;
			break;
		case aiPTI_Integer:
			ret = "aiPTI_Integer"s;
			break;
		case aiPTI_Buffer:
			ret = "aiPTI_Buffer"s;
			break;
		case _aiPTI_Force32Bit:
			ret = "_aiPTI_Force32Bit"s;
			break;
		default:
			ret = "UNKNOWN"s;
			break;
		}

		return ret;
	}

	template<typename T>
	std::optional<T> QueryDataByMaterialProperty(aiMaterial* pMaterial, aiMaterialProperty* pProp);

	std::string QueryRawData(aiMaterialProperty* pProp);

private:
	std::shared_ptr<Assimp::Importer> m_pImporter = nullptr;
	const aiScene* m_rpScene = nullptr;
	aiNode* m_rpRootNode = nullptr;

	std::wstring m_wstrFilePath = L"";

	UINT m_tabs = 0;
};

template<typename T>
inline std::optional<T> AssimpConverter::QueryDataByMaterialProperty(aiMaterial* pMaterial, aiMaterialProperty* pProp)
{
	T data;

	if (pMaterial->Get(pProp->mKey.C_Str(), data) == AI_SUCCESS)
	{
		return data;
	}
	else
	{
		return std::nullopt;
	}

}

inline std::string AssimpConverter::QueryRawData(aiMaterialProperty* pProp)
{
	std::string raw(reinterpret_cast<const char*>(pProp->mData), pProp->mDataLength);
	return raw;
}