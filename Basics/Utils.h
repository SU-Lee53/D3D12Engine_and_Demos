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

};

