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
};

