#pragma once
#include "Mesh.h"
#include "input_types.h"

class MeshHelper
{
public:
	static void CreateBoxMesh(std::vector<BasicVertexType>& vertices, std::vector<UINT>& indices);
	static void CreateSphere(std::vector<VertexType>& vertices, std::vector<UINT>& indices);

};

