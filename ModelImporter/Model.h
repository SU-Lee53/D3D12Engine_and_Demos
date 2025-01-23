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

	void Render();

public:
	std::vector<std::shared_ptr<ModelNode>>& GetModelNodes() { return m_pModelNodes; }

	void AddModelNode(std::shared_ptr<ModelNode> pNode) { m_pModelNodes.push_back(pNode); }
	std::shared_ptr<ModelNode>& GetModelNode(UINT index) { return m_pModelNodes[index]; }

public:
	static void UpdateChildsInModelNodes(std::vector<std::shared_ptr<ModelNode>>& pModelNodes);

private:
	std::vector<std::shared_ptr<ModelNode>> m_pModelNodes = {};

};

