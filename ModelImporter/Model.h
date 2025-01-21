#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

struct ModelNode
{
	std::unique_ptr<Mesh> pMesh;
	std::unique_ptr<Transform> pTransform;
	std::unique_ptr<Material> pMaterial;

	UINT uiParentIndex = -1;
	std::vector<UINT> uiChildrenIndices = {};
};

class Model
{
public:
	Model();
	~Model();

public:
	BOOL Initialize();

public:
	std::vector<std::shared_ptr<ModelNode>>& GetModelNodes() { return m_pModelNodes; }

	void AddModelNode(std::shared_ptr<ModelNode> pNode) { m_pModelNodes.push_back(pNode); }
	std::shared_ptr<ModelNode>& GetModelNode(UINT index) { return m_pModelNodes[index]; }

private:
	std::vector<std::shared_ptr<ModelNode>> m_pModelNodes = {};

};

