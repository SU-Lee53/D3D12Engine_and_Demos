#pragma once

// 기본적인 ID3D12Pipeline 을 Wrapping 하는 객체
// 필요에 따라 상속받아 다른 형태의 Pipeline 을 구현하면 좋을듯

// TODO : 구현

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

