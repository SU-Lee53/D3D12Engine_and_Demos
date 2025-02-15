#pragma once

#include "types.h"

struct Input {};

struct BasicInput : public Input
{
	Vec3 pos;
	Vec4 color;
	Vec2 texCoord;

	static std::vector<D3D12_INPUT_ELEMENT_DESC> descs;
};

struct DefaultInput : public Input
{
	XMFLOAT3 Position = { 0,0,0 };
	XMFLOAT4 Color = { 0,0,0,0 };
	XMFLOAT2 TexCoord = { 0,0 };
	XMFLOAT3 Normal = { 0,0,0 };
	XMFLOAT3 BiNormal = { 0,0,0 };
	XMFLOAT3 Tangent = { 0,0,0 };

	static std::vector<D3D12_INPUT_ELEMENT_DESC> descs;
};

struct TerrainInput : public Input
{
	XMFLOAT3 Position = { 0,0,0 };
	XMFLOAT2 TexCoord = { 0,0 };

	static std::vector<D3D12_INPUT_ELEMENT_DESC> descs;
};

using BasicVertexType = BasicInput;
using VertexType = DefaultInput;
using TerrainVertexType = TerrainInput;

/////////////
// Concept //
/////////////

template<typename C>
concept IsShaderInputType = requires(C c)
{
	std::derived_from<C, Input>;
};

template<typename C>
concept ShaderInputType = IsShaderInputType<C>;
