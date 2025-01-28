#include "pch.h"
#include "input_types.h"

using namespace std;

std::vector<D3D12_INPUT_ELEMENT_DESC> BasicInput::descs =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, 
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // 4 * 3 = 12
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } // 12 + 4 * 4 = 28
};

std::vector<D3D12_INPUT_ELEMENT_DESC> DefaultInput::descs =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, 
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // 4 * 3 = 12
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // 12 + (4 * 4) = 28
	{ "NORMAL", 0, DXGI_FORMAT_R32G32_FLOAT,	0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // 28 + (4 * 2) = 36
	{ "BINORMAL", 0, DXGI_FORMAT_R32G32_FLOAT,	0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // 36 + (4 * 3) = 48
	{ "TANGENT", 0, DXGI_FORMAT_R32G32_FLOAT,	0, 60, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } // 48 + (4 * 3) = 60
};