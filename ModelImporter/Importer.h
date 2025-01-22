#pragma once

class Model;
struct ModelNode;

class FbxLoader
{
public:
	FbxLoader();
	~FbxLoader();

	BOOL Initialize();

public:
	void LoadFBXFile(std::string strFilename);

#pragma region FBX_VIEWER
public:
	void ShowFBXNodeToImGui();

private:
	void PrintTabs();
	void ProcessNode(FbxNode* pFbxNode, const char* cstrNodeName);
	void PrintAttribute(FbxNodeAttribute* pfbxAttribute);
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType fbxType);
	std::string GetFbxMappingNodeName(FbxGeometryElement::EMappingMode mappingMode);
	std::string GetFbxReferenceNodeName(FbxGeometryElement::EReferenceMode referenceMode);

	void PrintLayerInfo(FbxMesh* pfbxMesh, const char* cstrNodeName);
	void PrintMeshInfo(FbxMesh* pfbxMesh, const char* cstrNodeName);
	void PrintSurfaceMaterialInfo(FbxNode* pfbxNode, const char* cstrNodeName);
	void PrintMaterialPropertyInfo(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName, const char* cstrNodeName);
	void PrintMaterialPropertyInfoAll(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrNodeName);
	void PrintTextureInfo(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName);

	BOOL IsTexture(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName);

#pragma endregion FBX_VIEWER


#pragma region FBX_TO_MODEL
public:
	void ExportModelInSceneToModel(std::shared_ptr<Model> pOutModel);

private:
	void ExportNode(std::shared_ptr<Model> pOutModel, FbxNode* pfbxNode);

	void ExportMesh(std::shared_ptr<ModelNode> pOutModelNode, FbxMesh* pfbxMesh);
	void ExportMaterial(std::shared_ptr<ModelNode> pOutModelNode, FbxNode* pfbxNode);


#pragma endregion FBX_TO_MODEL


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

