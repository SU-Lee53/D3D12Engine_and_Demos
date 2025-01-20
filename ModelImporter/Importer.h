#pragma once

class Model;

class Importer
{
public:
	Importer();
	~Importer();

	BOOL Initialize();

public:
	void LoadFBXFile(std::string strFilename);
	void ShowFBXNodeToImGui();

private:
	void PrintTabs();
	void ProcessNode(FbxNode* pFbxNode, const char* cstrNodeName);
	void PrintAttribute(FbxNodeAttribute* pfbxAttribute);
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType fbxType);

	void PrintLayerInfo(FbxMesh* pfbxMesh, const char* cstrNodeName);
	void PrintMeshInfo(FbxMesh* pfbxMesh, const char* cstrNodeName);
	void PrintSurfaceMaterialInfo(FbxNode* pfbxNode, const char* cstrNodeName);
	void PrintMaterialPropertyInfo(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName, const char* cstrNodeName);
	void PrintMaterialPropertyInfoAll(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrNodeName);
	void PrintTextureInfo(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName);

	BOOL HasTexture(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName);

	void ExportModelInSceneToModel(std::shared_ptr<Model>& pOutModel);

private:
	void CleanUp();


private:
	// FBX SDK memory management object
	FbxManager* m_rpfbxManager = nullptr;

	// For .fbx import
	FbxIOSettings* m_rpfbxIOSettings = nullptr;
	FbxImporter* m_rpfbxImporter = nullptr;
	FbxScene* m_rpfbxScene = nullptr;

	// Root node
	FbxNode* m_rpfbxRootNode = nullptr;

	// File metadata cache
	std::string m_strFilename = "";

	UINT m_tabs = 0;


};

