#pragma once

#include "types.h"

struct Input {};

struct DefaultInput : public Input
{
	Vec3 pos;
	Vec4 color;
	Vec2 texCoord;

	static std::vector<D3D12_INPUT_ELEMENT_DESC> descs;
};

using VertexType = DefaultInput;

template<typename C>
concept IsShaderInputType = requires(C c)
{
	std::derived_from<C, Input>;
};

template<typename C>
concept ShaderInputType = IsShaderInputType<C>;
