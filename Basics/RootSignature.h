#pragma once

struct DescriptorRange
{
private:
	std::vector<CD3DX12_DESCRIPTOR_RANGE> DescriptorRanges;

public:
	void Resize(size_t size)
	{
		DescriptorRanges.resize(size);
	}

	size_t Size()
	{ 
		return DescriptorRanges.size(); 
	}

	void Add(const CD3DX12_DESCRIPTOR_RANGE* descriptorRange)
	{
		DescriptorRanges.push_back(*descriptorRange);
	}

	CD3DX12_DESCRIPTOR_RANGE* Get(UINT offset = 0)
	{
		return &DescriptorRanges[offset];
	}

	CD3DX12_DESCRIPTOR_RANGE& operator[](UINT index)
	{
		if (index >= DescriptorRanges.size()) __debugbreak();
		return DescriptorRanges[index];
	}

};

struct RootParameter
{
private:
	std::vector<CD3DX12_ROOT_PARAMETER> RootParameters;

public:
	void Resize(size_t size)
	{
		RootParameters.resize(size);
	}

	size_t Size()
	{ 
		return RootParameters.size(); 
	}

	void Add(const CD3DX12_ROOT_PARAMETER* rootParam)
	{
		RootParameters.push_back(*rootParam);
	}

	CD3DX12_ROOT_PARAMETER* Get(UINT offset = 0)
	{
		return &RootParameters[offset];
	}

	CD3DX12_ROOT_PARAMETER& operator[](UINT index)
	{
		if (index >= RootParameters.size()) __debugbreak();
		return RootParameters[index];
	}
};

struct StaticSampler
{
private:
	std::vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers;

public:
	void Resize(size_t size)
	{
		StaticSamplers.resize(size);
	}
	size_t Size() 
	{
		return StaticSamplers.size();
	}

	void Add(const D3D12_STATIC_SAMPLER_DESC* sampler)
	{
		StaticSamplers.push_back(*sampler);
	}

	D3D12_STATIC_SAMPLER_DESC* Get(UINT offset = 0)
	{
		return &StaticSamplers[offset];
	}
	
	D3D12_STATIC_SAMPLER_DESC& operator[](UINT index)
	{
		if (index >= StaticSamplers.size()) __debugbreak();
		return StaticSamplers[index];
	}
};

// Derive it when need

class RootSignature
{
public:
	virtual BOOL Initialize() = 0;

	ID3D12RootSignature* Get()
	{
		return m_pRootSignature.Get();
	}

protected:
	ComPtr<ID3D12RootSignature> m_pRootSignature = nullptr;
	D3D12_ROOT_SIGNATURE_FLAGS m_RootSignatureFlags;

	// In real situation, variables below are useless after create root signature
	// but, this renderer is for demo and example so im going to cache it for debug
	DescriptorRange m_DescriptorRange;
	RootParameter m_RootParameter;
	StaticSampler m_StaticSampler;

};
