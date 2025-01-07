#include "pch.h"
#include "Pipeline.h"
#include "Shader.h"
#include "input_types.h"

Pipeline::Pipeline()
{
}

Pipeline::~Pipeline()
{
}

BOOL Pipeline::Initialize(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc)
{
	m_Desc = desc;

	ThrowIfFailed(
		DEVICE->CreateGraphicsPipelineState(&m_Desc, IID_PPV_ARGS(m_PipelineState.GetAddressOf()))
	);

	return TRUE;
}
