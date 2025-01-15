#pragma once

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
	void ProcessNode(FbxNode* pFbxNode);
	void PrintAttribute(FbxNodeAttribute* pfbxAttribute);
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType fbxType);

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

	UINT m_tabs;

};

