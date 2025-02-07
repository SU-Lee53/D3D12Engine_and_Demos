#pragma once

class Model;
struct ModelNode;

struct MeshData
{
	void Clear()
	{
		positions.clear();
		uvs.clear();
		normals.clear();
		binormals.clear();
		tangents.clear();
		colors.clear();
		indices.clear();
	}

	std::vector<FbxVector4> positions;
	std::vector<FbxVector2> uvs;
	std::vector<FbxVector4> normals;
	std::vector<FbxVector4> binormals;
	std::vector<FbxVector4> tangents;
	std::vector<FbxColor>	colors;
	std::vector<int>		indices;
};

struct VertexData
{
	bool operator==(const VertexData& other) const
	{
		return 
			position == other.position &&
			uv == other.uv &&
			normal == other.normal &&
			binormal == other.binormal &&
			tangent == other.tangent &&
			color == other.color;
	}

	FbxVector4 position;
	FbxVector2 uv;
	FbxVector4 normal;
	FbxVector4 binormal;
	FbxVector4 tangent;
	FbxColor color;
};

// MSVC macro 
_STD_BEGIN

// hash for unordered_map
template<>
struct hash<VertexData>
{
	size_t operator()(const VertexData& v) const
	{
		size_t h1 = std::hash<double>()(v.position[0]) ^ std::hash<double>()(v.position[1]);
		size_t h2 = std::hash<double>()(v.position[2]) ^ std::hash<double>()(v.position[3]);
		size_t h3 = std::hash<double>()(v.uv[0]) ^ std::hash<double>()(v.uv[1]);
		size_t h4 = std::hash<double>()(v.normal[0]) ^ std::hash<double>()(v.normal[1]);
		size_t h5 = std::hash<double>()(v.normal[2]) ^ std::hash<double>()(v.normal[3]);
		size_t h6 = std::hash<double>()(v.tangent[0]) ^ std::hash<double>()(v.tangent[1]);
		size_t h7 = std::hash<double>()(v.tangent[2]) ^ std::hash<double>()(v.tangent[3]);
		return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5) ^ (h7 << 6);
	}
};

_STD_END

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

	template<typename elementType, typename vectorType = typename elementType::ArrayElementType>
	vectorType ExtractElement(elementType* pElement, int cpIndex, fbxsdk::FbxMesh* pfbxMesh, int polyIndex, int vtxIndex, const std::string& elementName);

#pragma endregion FBX_VIEWER


#pragma region FBX_TO_MODEL
public:
	void ExportModelInSceneToModel(std::shared_ptr<Model> pOutModel);

private:
	void ExportNode(std::shared_ptr<Model> pOutModel, FbxNode* pfbxNode, int uiParentIndex);

	void ExportMesh(std::shared_ptr<ModelNode> pOutModelNode, FbxMesh* pfbxMesh);
	void ExportMaterial(std::shared_ptr<ModelNode> pOutModelNode, FbxNode* pfbxNode);

#pragma endregion FBX_TO_MODEL

private:
	std::string ExtractFilename(const std::string& strTexturePath);

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

template<typename elementType, typename vectorType>
inline vectorType FbxLoader::ExtractElement(elementType* pElement, int cpIndex, fbxsdk::FbxMesh* pfbxMesh, int polyIndex, int vtxIndex, const std::string& elementName)
{
	vectorType v;

	if (pElement)
	{
		if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			int index = cpIndex;
			if (pElement->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				v = pElement->GetDirectArray().GetAt(index);
			}
			else if (pElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = pElement->GetIndexArray().GetAt(index);
				v = pElement->GetDirectArray().GetAt(id);
			}
			else
			{
				std::string debugString = elementName + ": Unsurpported reference mode\n"s;
				OutputDebugStringA(debugString.c_str());
			}
		}
		else if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int index = vtxIndex;
			if (pElement->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				v = pElement->GetDirectArray().GetAt(index);
			}
			else if(pElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int id = pElement->GetIndexArray().GetAt(index);
				v = pElement->GetDirectArray().GetAt(id);
			}
			else
			{
				std::string debugString = elementName + ": Unsurpported reference mode\n"s;
				OutputDebugStringA(debugString.c_str());
			}
		}
		else
		{
			std::string debugString = elementName + ": Unsurpported mapping mode\n"s;
			OutputDebugStringA(debugString.c_str());
		}
	}

	return v;


}
