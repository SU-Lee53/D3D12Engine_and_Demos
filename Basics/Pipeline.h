#pragma once

// �⺻���� ID3D12Pipeline �� Wrapping �ϴ� ��ü
// �ʿ信 ���� ��ӹ޾� �ٸ� ������ Pipeline �� �����ϸ� ������

// TODO : ����

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	BOOL Initialize(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc);

protected:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_Desc = {};
	ComPtr<ID3D12PipelineState> m_PipelineState = nullptr;
};

