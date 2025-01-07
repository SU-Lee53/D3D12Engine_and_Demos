#pragma once

// 이건 어떻게 해야할지 생각좀 해봐야할듯
// 범용적으로 쓰기에는 약간 어렵다 싶음

class RootSignature
{
public:
	RootSignature();
	~RootSignature();

	BOOL Initialize(std::vector<D3D12_ROOT_PARAMETER> rootParameters);

private:
	ComPtr<ID3D12RootSignature> m_pRootSignature;
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags;

};

