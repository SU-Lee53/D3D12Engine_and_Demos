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
	void PrintInfoToImGui();

private:
	std::vector<std::shared_ptr<ModelNode>> m_pModelNodes = {};

	// ImGui Variables
	int m_ItemSelected = 0;
	int m_ItemHighlightedIndex = 0;
	BOOL m_ItemHighlighted = FALSE;

};

