#pragma once

class Utils
{
public:
    static void SetDebugLayerInfo(ComPtr<ID3D12Device> pDevice);
	static inline size_t AlignConstantBuffersize(size_t size)
	{
		size_t alligned_size = (size + 255) & (~255);
		return alligned_size;
	}

	static void DecomposeMatrixToBasis(IN XMFLOAT4X4 matrix, OUT XMFLOAT3& outFront, OUT XMFLOAT3& outRight, OUT XMFLOAT3& outUp)
	{
		XMMATRIX xmWorld = XMLoadFloat4x4(&matrix);
		XMStoreFloat3(&outFront, xmWorld.r[2]); // Front is in 3rd row
		XMStoreFloat3(&outRight, xmWorld.r[0]); // Right is in 1st row
		XMStoreFloat3(&outUp, xmWorld.r[1]); // UP is in 2nd row
	}

	static void SetDefaultSamplerDesc(D3D12_STATIC_SAMPLER_DESC& outSamplerDesc, UINT registerIndex)
	{
		outSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		outSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		outSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		outSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		outSamplerDesc.MipLODBias = 0.f;
		outSamplerDesc.MaxAnisotropy = 16;
		outSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		outSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		outSamplerDesc.MinLOD = -FLT_MAX;
		outSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		outSamplerDesc.ShaderRegister = registerIndex;
		outSamplerDesc.RegisterSpace = 0;
		outSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}

};

