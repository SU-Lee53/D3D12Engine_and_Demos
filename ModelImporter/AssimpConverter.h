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
	void PrintNodeInfo(aiNode* node);


#pragma endregion PRINT_DATAS

private:
	std::shared_ptr<Assimp::Importer> m_pImporter = nullptr;
	const aiScene* m_rpaiScene = nullptr;
	aiNode* m_rpRootNode = nullptr;

	std::wstring m_wstrFilePath = L"";

};

