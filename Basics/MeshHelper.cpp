#include "pch.h"
#include "MeshHelper.h"

using namespace std;

void MeshHelper::CreateBoxMesh(vector<VertexType>& vtx, vector<UINT>& idx)
{
	float w2 = 1.f;
	float h2 = 1.f;
	float d2 = 1.f;

	vtx.resize(24);

	// Front 
	vtx[0] = DefaultInput(Input{}, Vec3{ -w2, -h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.f, 1.f });
	vtx[1] = DefaultInput(Input{}, Vec3{ -w2, -h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.f, 1.f });
	vtx[2] = DefaultInput(Input{}, Vec3{ -w2, -h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.f, 1.f });
	vtx[3] = DefaultInput(Input{}, Vec3{ -w2, -h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.f, 1.f });
	// Back
	vtx[4] = DefaultInput(Input{}, Vec3{ -w2, -h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 1.0f, 1.0f });
	vtx[5] = DefaultInput(Input{}, Vec3{ +w2, -h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.0f, 1.0f });
	vtx[6] = DefaultInput(Input{}, Vec3{ +w2, +h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.0f, 0.0f });
	vtx[7] = DefaultInput(Input{}, Vec3{ -w2, +h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 1.0f, 0.0f });
	// Upper
	vtx[8] = DefaultInput(Input{}, Vec3{ -w2, +h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.0f, 1.0f });
	vtx[9] = DefaultInput(Input{}, Vec3{ -w2, +h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.0f, 0.0f });
	vtx[10] = DefaultInput(Input{}, Vec3{ +w2, +h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 1.0f, 0.0f });
	vtx[11] = DefaultInput(Input{}, Vec3{ +w2, +h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 1.0f, 1.0f });
	// Lower			  										  
	vtx[12] = DefaultInput(Input{}, Vec3{ -w2, -h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.0f, 1.0f });
	vtx[13] = DefaultInput(Input{}, Vec3{ +w2, -h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.0f, 0.0f });
	vtx[14] = DefaultInput(Input{}, Vec3{ +w2, -h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 1.0f, 0.0f });
	vtx[15] = DefaultInput(Input{}, Vec3{ -w2, -h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 1.0f, 1.0f });
	// Left				  										  
	vtx[16] = DefaultInput(Input{}, Vec3{ -w2, -h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.0f, 1.0f });
	vtx[17] = DefaultInput(Input{}, Vec3{ -w2, +h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.0f, 0.0f });
	vtx[18] = DefaultInput(Input{}, Vec3{ -w2, +h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 1.0f, 0.0f });
	vtx[19] = DefaultInput(Input{}, Vec3{ -w2, -h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 1.0f, 1.0f });
	// Right
	vtx[20] = DefaultInput(Input{}, Vec3{ +w2, -h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.0f, 1.0f });
	vtx[21] = DefaultInput(Input{}, Vec3{ +w2, +h2, -d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 0.0f, 0.0f });
	vtx[22] = DefaultInput(Input{}, Vec3{ +w2, +h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 1.0f, 0.0f });
	vtx[23] = DefaultInput(Input{}, Vec3{ +w2, -h2, +d2 }, Vec4{ 1.f, 0.f, 0.f, 1.f }, Vec2{ 1.0f, 1.0f });

	idx.resize(36);

	// Front
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// Back
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// Upper
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// Lower
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// Left
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// Right
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

}
