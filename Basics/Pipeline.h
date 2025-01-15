#pragma once

// �⺻���� ID3D12Pipeline �� Wrapping �ϴ� ��ü
// �ʿ信 ���� ��ӹ޾� �ٸ� ������ Pipeline �� �����ϸ� ������

// TODO : ����

class Pipeline
{
public:
	virtual BOOL Initialize(std::shared_ptr<class RootSignature> rootSignature) = 0;

	ID3D12PipelineState* Get()
	{
		return m_PipelineState.Get();
	}

protected:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_Desc = {};
	ComPtr<ID3D12PipelineState> m_PipelineState = nullptr;
};
