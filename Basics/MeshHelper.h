#pragma once
#include "Mesh.h"
#include "input_types.h"

class MeshHelper
{
public:
	static void CreateCube(std::vector<BasicVertexType>& vertices, std::vector<UINT>& indices);
	static void CreateSphere(std::vector<VertexType>& vertices, std::vector<UINT>& indices);

	static void CreatePlane(std::vector<VertexType>& vertices, std::vector<UINT>& indices);

	static void CreateTerrainGrid(std::vector<TerrainVertexType>& vertices, std::vector<UINT>& indices, UINT width, UINT height);

	static void CreateSkyboxCube(std::vector<SkyboxVertexType>& vertices, std::vector<UINT>& indices);
	static void CreateQuadMeshForCubeMapping(std::vector<CubemapVertexType>& vertices, std::vector<UINT>& indices);
};

