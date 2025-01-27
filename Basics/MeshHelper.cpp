#include "pch.h"
#include "MeshHelper.h"

using namespace std;

void MeshHelper::CreateBoxMesh(vector<BasicVertexType>& vtx, vector<UINT>& idx)
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
	vtx[0] = BasicInput(Input{}, Vec3{ -fx, +fy, -fz }, c0, Vec2{ 0.f, 1.f });
	vtx[1] = BasicInput(Input{}, Vec3{ +fx, +fy, -fz }, c1, Vec2{ 0.f, 1.f });
	vtx[2] = BasicInput(Input{}, Vec3{ +fx, +fy, +fz }, c2, Vec2{ 0.f, 1.f });
	vtx[3] = BasicInput(Input{}, Vec3{ -fx, +fy, +fz }, c3, Vec2{ 0.f, 1.f });
	// Back
	vtx[4] = BasicInput(Input{}, Vec3{ -fx, -fy, -fz }, c4, Vec2{ 1.0f, 1.0f });
	vtx[5] = BasicInput(Input{}, Vec3{ +fx, -fy, -fz }, c5, Vec2{ 0.0f, 1.0f });
	vtx[6] = BasicInput(Input{}, Vec3{ +fx, -fy, +fz }, c6, Vec2{ 0.0f, 0.0f });
	vtx[7] = BasicInput(Input{}, Vec3{ -fx, -fy, +fz }, c7, Vec2{ 1.0f, 0.0f });

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
