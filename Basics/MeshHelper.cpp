#include "pch.h"
#include "MeshHelper.h"

using namespace std;

void MeshHelper::CreateCube(vector<BasicVertexType>& vtx, vector<UINT>& idx)
{
	float fx = 0.5f;
	float fy = 0.5f;
	float fz = 0.5f;

	vtx.resize(8);

	Vec4 c0 = DirectX::Colors::Red;
	Vec4 c1 = DirectX::Colors::Orange;
	Vec4 c2 = DirectX::Colors::Yellow;
	Vec4 c3 = DirectX::Colors::Green;
	Vec4 c4 = DirectX::Colors::Blue;
	Vec4 c5 = DirectX::Colors::Indigo;
	Vec4 c6 = DirectX::Colors::Purple;
	Vec4 c7 = DirectX::Colors::White;

	// Front 
	vtx[0] = BasicInput(Input{}, XMFLOAT3{ -fx, +fy, -fz }, c0, XMFLOAT2{ 0.f, 1.f });
	vtx[1] = BasicInput(Input{}, XMFLOAT3{ +fx, +fy, -fz }, c1, XMFLOAT2{ 0.f, 1.f });
	vtx[2] = BasicInput(Input{}, XMFLOAT3{ +fx, +fy, +fz }, c2, XMFLOAT2{ 0.f, 1.f });
	vtx[3] = BasicInput(Input{}, XMFLOAT3{ -fx, +fy, +fz }, c3, XMFLOAT2{ 0.f, 1.f });
	// Back
	vtx[4] = BasicInput(Input{}, XMFLOAT3{ -fx, -fy, -fz }, c4, XMFLOAT2{ 1.0f, 1.0f });
	vtx[5] = BasicInput(Input{}, XMFLOAT3{ +fx, -fy, -fz }, c5, XMFLOAT2{ 0.0f, 1.0f });
	vtx[6] = BasicInput(Input{}, XMFLOAT3{ +fx, -fy, +fz }, c6, XMFLOAT2{ 0.0f, 0.0f });
	vtx[7] = BasicInput(Input{}, XMFLOAT3{ -fx, -fy, +fz }, c7, XMFLOAT2{ 1.0f, 0.0f });

	idx.resize(36);

	// Front
	idx[0] = 3; idx[1] = 1; idx[2] = 0;
	idx[3] = 2; idx[4] = 1; idx[5] = 3;
	// Upper
	idx[6] = 0; idx[7] = 5; idx[8] = 4;
	idx[9] = 1; idx[10] = 5; idx[11] = 0;
	// Back
	idx[12] = 3; idx[13] = 4; idx[14] = 7;
	idx[15] = 0; idx[16] = 4; idx[17] = 3;
	// Lower
	idx[18] = 1; idx[19] = 6; idx[20] = 5;
	idx[21] = 2; idx[22] = 6; idx[23] = 1;
	// Left
	idx[24] = 2; idx[25] = 7; idx[26] = 6;
	idx[27] = 3; idx[28] = 7; idx[29] = 2;
	// Right
	idx[30] = 6; idx[31] = 4; idx[32] = 5;
	idx[33] = 7; idx[34] = 4; idx[35] = 6;

}

void MeshHelper::CreateSphere(std::vector<VertexType>& vertices, std::vector<UINT>& indices)
{

	float radius = 0.5f; // Radius of Sphere
	UINT stackCount = 20; // Column Divide
	UINT sliceCount = 20; // Row Divide

	// Vertices
	VertexType v;

	// North(Top)
	v.Position = XMFLOAT3(0.0f, radius, 0.0f);
	v.TexCoord = XMFLOAT2(0.5f, 0.0f);
	v.Normal = v.Position;
	XMStoreFloat3(&v.Normal, XMVector3Normalize(XMLoadFloat3(&v.Normal)));
	v.Tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMStoreFloat3(&v.Tangent, XMVector3Normalize(XMLoadFloat3(&v.Tangent)));

	XMVECTOR xmNormal = XMLoadFloat3(&v.Normal);
	XMVECTOR xmTangent = XMLoadFloat3(&v.Tangent);
	XMVECTOR xmBinormal = XMVector3Dot(xmNormal, xmTangent);
	XMStoreFloat3(&v.BiNormal, xmBinormal);

	vertices.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// Compute vertex for each ring
	for (UINT y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// Vertex on ring
		for (UINT x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;
			v.Position.x = radius * sinf(phi) * cosf(theta);
			v.Position.y = radius * cosf(phi);
			v.Position.z = radius * sinf(phi) * sinf(theta);

			v.TexCoord = XMFLOAT2(deltaU * x, deltaV * y);

			v.Normal = v.Position;
			XMStoreFloat3(&v.Normal, XMVector3Normalize(XMLoadFloat3(&v.Normal)));

			v.Tangent.x = -radius * sinf(phi) * sinf(theta);
			v.Tangent.y = 0.0f;
			v.Tangent.z = radius * sinf(phi) * cosf(theta);
			XMStoreFloat3(&v.Tangent, XMVector3Normalize(XMLoadFloat3(&v.Tangent)));

			xmNormal = XMLoadFloat3(&v.Normal);
			xmTangent = XMLoadFloat3(&v.Tangent);
			xmBinormal = XMVector3Dot(xmNormal, xmTangent);
			XMStoreFloat3(&v.BiNormal, xmBinormal);

			vertices.push_back(v);
		}
	}

	// South(Bottom)
	v.Position = XMFLOAT3(0.0f, -radius, 0.0f);
	v.TexCoord = XMFLOAT2(0.5f, 0.0f);
	v.Normal = v.Position;
	XMStoreFloat3(&v.Normal, XMVector3Normalize(XMLoadFloat3(&v.Normal)));
	v.Tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMStoreFloat3(&v.Tangent, XMVector3Normalize(XMLoadFloat3(&v.Tangent)));

	xmNormal = XMLoadFloat3(&v.Normal);
	xmTangent = XMLoadFloat3(&v.Tangent);
	xmBinormal = XMVector3Dot(xmNormal, xmTangent);
	XMStoreFloat3(&v.BiNormal, xmBinormal);

	vertices.push_back(v);

	// Indices
	// North index
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		// [0]
		//  |  \
		// [i+1]-[i+2]
		indices.push_back(0);
		indices.push_back(i + 2);
		indices.push_back(i + 1);
	}

	// Body index
	UINT ringVertexCount = sliceCount + 1;
	for (UINT y = 0; y < stackCount - 2; ++y)
	{
		for (UINT x = 0; x < sliceCount; ++x)
		{
			// [y, x]-[y, x+1]
			//  |       /
			// [y+1, x]
			indices.push_back(1 + (y)*ringVertexCount + (x));
			indices.push_back(1 + (y)*ringVertexCount + (x + 1));
			indices.push_back(1 + (y + 1) * ringVertexCount + (x));
			//          [y, x+1]
			//         /      |
			// [y+1, x]-[y+1,x+1]
			indices.push_back(1 + (y + 1) * ringVertexCount + (x));
			indices.push_back(1 + (y)*ringVertexCount + (x + 1));
			indices.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// South
	UINT bottomIndex = static_cast<UINT>(vertices.size()) - 1;
	UINT lastRingStartIndex = bottomIndex - ringVertexCount;
	for (UINT i = 0; i < sliceCount; ++i)
	{
		// [last+i]-[last+i+1]
		// |        /
		// [bottom]
		indices.push_back(bottomIndex);
		indices.push_back(lastRingStartIndex + i);
		indices.push_back(lastRingStartIndex + i + 1);
	}
}

void MeshHelper::CreatePlane(std::vector<VertexType>& vertices, std::vector<UINT>& indices)
{
	vertices.resize(4);

	vertices[0].Position = XMFLOAT3(-0.5f, -0.5f, 0.f);
	vertices[0].TexCoord = XMFLOAT2(0.f, 1.f);
	vertices[0].Normal = XMFLOAT3(0.f, 0.f, -1.f);
	vertices[0].Tangent = XMFLOAT3(1.f, 0.f, 0.f);

	vertices[1].Position = XMFLOAT3(-0.5f, 0.5f, 0.f);
	vertices[1].TexCoord = XMFLOAT2(0.f, 0.f);
	vertices[1].Normal = XMFLOAT3(0.f, 0.f, -1.f);
	vertices[1].Tangent = XMFLOAT3(1.f, 0.f, 0.f);

	vertices[2].Position = XMFLOAT3(0.5f, -0.5f, 0.f);
	vertices[2].TexCoord = XMFLOAT2(1.f, 1.f);
	vertices[2].Normal = XMFLOAT3(0.f, 0.f, -1.f);
	vertices[2].Tangent = XMFLOAT3(1.f, 0.f, 0.f);

	vertices[3].Position = XMFLOAT3(0.5f, 0.5f, 0.f);
	vertices[3].TexCoord = XMFLOAT2(1.f, 0.f);
	vertices[3].Normal = XMFLOAT3(0.f, 0.f, -1.f);
	vertices[3].Tangent = XMFLOAT3(1.f, 0.f, 0.f);

	// Calculate Binormal
	for (int i = 0; i < 4; i++)
	{
		XMVECTOR xmNormal = XMLoadFloat3(&vertices[i].Normal);
		XMVECTOR xmTangent = XMLoadFloat3(&vertices[i].Tangent);

		XMVECTOR xmBinormal = XMVector3Cross(xmNormal, xmTangent);
		XMStoreFloat3(&vertices[i].BiNormal, xmBinormal);
	}


	indices = { 0, 1, 2, 2, 1, 3 };
}

void MeshHelper::CreateTerrainGrid(std::vector<TerrainVertexType>& vertices, std::vector<UINT>& indices, UINT width, UINT height)
{
	for (int z = 0; z < height + 1; z++)
	{
		for (int x = 0; x < width + 1; x++)
		{
			TerrainVertexType v;
			v.Position = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.TexCoord = Vec2(static_cast<float>(x), static_cast<float>(z));


			
			// UV
			// z 1.0
			// |
			// |
			// |
			// |
			// |
			// |
			// 0.0 ------------- x 1.0


			vertices.push_back(v);
		}
	}

	for (int z = 0; z < height; z++)
	{
		for (int x = 0; x < width; x++)
		{
			// [0]
			//  |  \
			// [2] - [1]
			indices.push_back((width + 1) * (z + 1) + (x));
			indices.push_back((width + 1) * (z)+ (x + 1));
			indices.push_back((width + 1) * (z)+ (x));
			// [1] - [2]
			//     \  |
			//       [0]
			indices.push_back((width + 1) * (z) + (x + 1));
			indices.push_back((width + 1) * (z + 1) + (x));
			indices.push_back((width + 1) * (z + 1) + (x + 1));
		}
	}
}

void MeshHelper::CreateSkyboxCube(std::vector<SkyboxVertexType>& vertices, std::vector<UINT>& indices)
{
	float fx = 1.0f;
	float fy = 1.0f;
	float fz = 1.0f;

	vertices.resize(8);

	// Front 
	vertices[0] = SkyboxVertexType(Input{}, XMFLOAT3{ -fx, +fy, -fz }, XMFLOAT3{ -fx, +fy, -fz });
	vertices[1] = SkyboxVertexType(Input{}, XMFLOAT3{ +fx, +fy, -fz }, XMFLOAT3{ +fx, +fy, -fz });
	vertices[2] = SkyboxVertexType(Input{}, XMFLOAT3{ +fx, +fy, +fz }, XMFLOAT3{ +fx, +fy, +fz });
	vertices[3] = SkyboxVertexType(Input{}, XMFLOAT3{ -fx, +fy, +fz }, XMFLOAT3{ -fx, +fy, +fz });
	// Back		  
	vertices[4] = SkyboxVertexType(Input{}, XMFLOAT3{ -fx, -fy, -fz }, XMFLOAT3{ -fx, +fy, -fz });
	vertices[5] = SkyboxVertexType(Input{}, XMFLOAT3{ +fx, -fy, -fz }, XMFLOAT3{ +fx, +fy, -fz });
	vertices[6] = SkyboxVertexType(Input{}, XMFLOAT3{ +fx, -fy, +fz }, XMFLOAT3{ +fx, +fy, +fz });
	vertices[7] = SkyboxVertexType(Input{}, XMFLOAT3{ -fx, -fy, +fz }, XMFLOAT3{ -fx, +fy, +fz });

	indices.resize(36);

	// Front
	indices[0] = 3; indices[1] = 1; indices[2] = 0;
	indices[3] = 2; indices[4] = 1; indices[5] = 3;
	// Upper
	indices[6] = 0; indices[7] = 5; indices[8] = 4;
	indices[9] = 1; indices[10] = 5; indices[11] = 0;
	// Back
	indices[12] = 3; indices[13] = 4; indices[14] = 7;
	indices[15] = 0; indices[16] = 4; indices[17] = 3;
	// Lower
	indices[18] = 1; indices[19] = 6; indices[20] = 5;
	indices[21] = 2; indices[22] = 6; indices[23] = 1;
	// Left
	indices[24] = 2; indices[25] = 7; indices[26] = 6;
	indices[27] = 3; indices[28] = 7; indices[29] = 2;
	// Right
	indices[30] = 6; indices[31] = 4; indices[32] = 5;
	indices[33] = 7; indices[34] = 4; indices[35] = 6;

}

void MeshHelper::CreateQuadMeshForCubeMapping(std::vector<CubemapVertexType>& vertices, std::vector<UINT>& indices)
{
	// Triangle 1
	vertices.resize(6);
	vertices[0].Position = { -1.f, 1.f, 0.f };
	vertices[1].Position = { 1.f, -1.f, 0.f };
	vertices[2].Position = { -1.f, -1.f, 0.f };

	// Triangle 2
	vertices[3].Position = { -1.f, 1.f, 0.f };
	vertices[4].Position = { 1.f, 1.f, 0.f };
	vertices[5].Position = { 1.f, -1.f, 0.f };

	// Indices
	indices.resize(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;

}
