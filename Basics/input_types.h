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

struct DefaultInput
{
	Vec3 Position = { 0,0,0 };
	Vec4 Color = { 0,0,0,0 };
	Vec2 TexCoord = { 0,0 };
	Vec3 Normal = { 0,0,0 };
	Vec3 BiNormal = { 0,0,0 };
	Vec3 Tangent = { 0,0,0 };
};

using BasicVertexType = BasicInput;
using VertexType = DefaultInput;

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
