#pragma once

// �̰� ��� �ؾ����� ������ �غ����ҵ�
// ���������� ���⿡�� �ణ ��ƴ� ����

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

