#pragma once

#include "Mesh.h"
class Material;

class Model
{
public:
	Model();
	~Model();

public:
	BOOL Initialize();

private:
	BOOL InitMesh();
	BOOL InitMaterial();


private:
	std::shared_ptr<Mesh<BasicVertexType>> m_pMesh = nullptr;
	std::shared_ptr<Material> m_pMaterial = nullptr;
};

