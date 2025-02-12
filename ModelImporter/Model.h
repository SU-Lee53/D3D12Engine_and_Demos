#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

struct ModelNode
{
	ModelNode()
	{
		pMesh = std::make_unique<Mesh>();
		pTransform = std::make_unique<Transform>();
		pMaterial = std::make_unique<Material>();
	}

	friend std::ostream& operator<<(std::ostream& os, const ModelNode& node)
	{
		os << "<Node>" << std::endl;

		os << "<Frame Name>" << std::endl;
		os << node.strName << std::endl;
		os << "</Frame Name>" << std::endl;
		
		os << "<Parent Index>" << std::endl;
		os.write(reinterpret_cast<const char*>(&node.parentIndex), sizeof(node.parentIndex));
		os << std::endl;
		os << "</Parent Index>" << std::endl;

		os << "<Children Count>" << std::endl;
		size_t nChildrens = node.uiChildrenIndices.size();
		os.write(reinterpret_cast<const char*>(&nChildrens), sizeof(nChildrens));
		os << std::endl;
		os << "</Children Count>" << std::endl;
		
		os << "<Children Index>" << std::endl; 
		for (const UINT& idx : node.uiChildrenIndices)
		{
			os.write(reinterpret_cast<const char*>(&idx), sizeof(idx));
		}
		os << std::endl;
		os << "</Children Index>" << std::endl;

		os << *node.pTransform;
		os << *node.pMesh;
		os << *node.pMaterial;

		os << "</Node>" << std::endl;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, ModelNode& node)
	{
		std::string read;
		while (read != "</Node>")
		{
			std::getline(is, read);

			if (read == "<Frame Name>")
			{
				std::getline(is, node.strName);
			}
			
			if (read == "<Parent Index>")
			{
				is.read(reinterpret_cast<char*>(&node.parentIndex), sizeof(node.parentIndex));
			}

			if (read == "<Children Count>")
			{
				size_t nChildren = 0;
				is.read(reinterpret_cast<char*>(&nChildren), sizeof(nChildren));
				node.uiChildrenIndices.resize(nChildren);
			}
			
			if (read == "<Children Index>")
			{
				is.read(reinterpret_cast<char*>(node.uiChildrenIndices.data()), sizeof(node.uiChildrenIndices[0]) * node.uiChildrenIndices.size());
			}

			if (read == "<Transform>")
			{
				is >> *node.pTransform;
			}

			if (read == "<Mesh>")
			{
				is >> *node.pMesh;
			}

			if (read == "<Material>")
			{
				is >> *node.pMaterial;
			}

		}

		return is;
	}

	std::string strName;

	std::unique_ptr<Mesh> pMesh;
	std::unique_ptr<Transform> pTransform;
	std::unique_ptr<Material> pMaterial;

	int parentIndex = -1;	// -1 means Root is Parent
	std::vector<UINT> uiChildrenIndices = {};
};

class Model
{
public:
	Model();
	~Model();

public:
	BOOL Initialize();

	void Update();
	void Render(std::shared_ptr<class Camera> pCamera);

public:
	std::vector<std::shared_ptr<ModelNode>>& GetModelNodes() { return m_pModelNodes; }

	void AddModelNode(std::shared_ptr<ModelNode> pNode) { m_pModelNodes.push_back(pNode); }
	std::shared_ptr<ModelNode>& GetModelNode(UINT index) { return m_pModelNodes[index]; }

public:
	static void UpdateChildsInModelNodes(std::vector<std::shared_ptr<ModelNode>>& pModelNodes);

public:
	void TranslateModel(const XMFLOAT3& pos);
	void RotateModel(const XMFLOAT3& rot);
	void ScaleModel(const XMFLOAT3& scale);

public:
	void ExportModelToBinary();
	void ImportModelFromBinary(std::string binaryFilePath);

public:
	void PrintInfoToImGui();

public:
	void SetName(const std::string& name) { m_strName = name; }
	std::string GetName() { return m_strName; }

private:
	XMFLOAT4X4 CalculateRelativeTranforms(std::shared_ptr<ModelNode> pNode);

private:
	std::string m_strName = "";
	std::vector<std::shared_ptr<ModelNode>> m_pModelNodes = {};

	// ImGui Variables
	int m_ItemSelected = 0;
	int m_ItemHighlightedIndex = 0;
	BOOL m_ItemHighlighted = FALSE;

};

