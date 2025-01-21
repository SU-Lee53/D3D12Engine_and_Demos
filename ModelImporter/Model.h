#pragma once

class Mesh;
class Transform;
class Material;

struct ModelNode
{
	std::unique_ptr<Mesh> pMesh;
	std::unique_ptr<Transform> pTransform;
	std::unique_ptr<Material> pMaterial;

	UINT uiParentIndex;
	std::vector<UINT> uiChildrenIndices;
};

class Model
{
public:
	Model();
	~Model();

public:
	BOOL Initialize();

private:
	std::vector<std::shared_ptr<ModelNode>> m_pModelNodes = {};

};

