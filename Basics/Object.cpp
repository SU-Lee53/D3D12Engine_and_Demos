#include "pch.h"
#include "Object.h"
#include "Transform.h"
#include "RenderMethod.h"
#include "input_types.h"
#include "Pipeline.h"
#include "Model.h"
#include "Mesh.h"

Object::Object()
{
}

Object::~Object()
{
}

BOOL Object::Initialize()
{
	return 0;
}

void Object::Update()
{
}

void Object::Render()
{
}

////////////////////////////////////
///		BasicForwardObject
////////////////////////////

BasicForwardObject::BasicForwardObject()
{
}

BasicForwardObject::~BasicForwardObject()
{
}

BOOL BasicForwardObject::Initialize()
{
	BOOL bResult = FALSE;
	bResult = InitRootSignatrue();
	bResult = InitRenderMethod();

	std::vector<VertexType> vtx;
	std::vector<UINT> idx;

	bResult = m_pMesh->Initialize(vtx, idx);

	return bResult;
}

void BasicForwardObject::Update()
{
	m_pTransform->Update();
}

void BasicForwardObject::Render()
{
	m_pRenderMethod->Render();
}

BOOL BasicForwardObject::InitRootSignatrue()
{
	ComPtr<ID3DBlob> pSignature = nullptr;
	ComPtr<ID3DBlob> pErrorBlob = nullptr;

	/*
		|   CBV   |   SRV   |
		CBV : world, view, proj
		SRV : Diffuse Texture
	*/

	CD3DX12_DESCRIPTOR_RANGE ranges[2] = {};
	ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootParameters[1] = {};
	rootParameters[0].InitAsDescriptorTable(_countof(ranges), ranges, D3D12_SHADER_VISIBILITY_ALL);

	D3D12_STATIC_SAMPLER_DESC sampler = {};
	{
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.MipLODBias = 0.0f;
		sampler.MaxAnisotropy = 16;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		sampler.MinLOD = -FLT_MAX;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderRegister = D3D12_SHADER_VISIBILITY_PIXEL;
	}

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 1, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, pSignature.GetAddressOf(), pErrorBlob.GetAddressOf()));
	
	ThrowIfFailed(DEVICE->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(m_pRootSignature.GetAddressOf())));

	return TRUE;
}

BOOL BasicForwardObject::InitRenderMethod()
{
	auto vs = SHADER.GetShader<VertexShader>("testVS");
	auto ps = SHADER.GetShader<VertexShader>("testPS");
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	{
		desc.InputLayout = { DefaultInput::descs.data(), (UINT)DefaultInput::descs.size() };
		desc.pRootSignature = m_pRootSignature.Get();
		desc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBlob()->GetBufferPointer(), vs->GetBlob()->GetBufferSize());
		desc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBlob()->GetBufferPointer(), ps->GetBlob()->GetBufferSize());
		desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		desc.DepthStencilState.StencilEnable = FALSE;
		desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		desc.SampleMask = UINT_MAX;
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		desc.NumRenderTargets = 1;
		desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count = 1;
	}

	std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC> descs = { desc };
	m_pRenderMethod->Initialize(shared_from_this(), descs);

	return TRUE;
}
