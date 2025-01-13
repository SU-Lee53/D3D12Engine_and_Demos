#include "pch.h"
#include "Pipeline.h"
#include "Shader.h"
#include "input_types.h"
#include "RootSignature.h"

using namespace std;

BOOL BasicForwardPipeline::Initialize(shared_ptr<RootSignature> rootSignature)
{
	auto vs = SHADER.GetShader<VertexShader>("testVS");
	auto ps = SHADER.GetShader<PixelShader>("testPS");

	{
		m_Desc.InputLayout = { DefaultInput::descs.data(), (UINT)DefaultInput::descs.size() };
		m_Desc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBlob()->GetBufferPointer(), vs->GetBlob()->GetBufferSize());
		m_Desc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBlob()->GetBufferPointer(), ps->GetBlob()->GetBufferSize());
		m_Desc.pRootSignature = rootSignature->Get();
		m_Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		m_Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		m_Desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		m_Desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		m_Desc.DepthStencilState.StencilEnable = FALSE;
		m_Desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		m_Desc.SampleMask = UINT_MAX;
		m_Desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		m_Desc.NumRenderTargets = 1;
		m_Desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_Desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		m_Desc.SampleDesc.Count = 1;
	}

	ThrowIfFailed(
		DEVICE->CreateGraphicsPipelineState(&m_Desc, IID_PPV_ARGS(m_PipelineState.GetAddressOf()))
	);

	return TRUE;
}
