#include "pch.h"
#include "BasicForwardRenderResources.h"
#include "MeshHelper.h"

using namespace std;

/////////////////////////
//    RootSignature    //
/////////////////////////

BOOL BasicForwardRootSignature::Initialize()
{
	m_DescriptorRange.Resize(2);
	m_DescriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// b0 : TransformData
	m_DescriptorRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	// b1 : CameraData

	m_RootParameter.Resize(1);
	m_RootParameter[0].InitAsDescriptorTable(m_DescriptorRange.Size(), m_DescriptorRange.Get(), D3D12_SHADER_VISIBILITY_ALL);

	m_StaticSampler.Resize(1);
	{
		m_StaticSampler[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		m_StaticSampler[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_StaticSampler[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_StaticSampler[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_StaticSampler[0].MipLODBias = 0.0f;
		m_StaticSampler[0].MaxAnisotropy = 16;
		m_StaticSampler[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		m_StaticSampler[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		m_StaticSampler[0].MinLOD = -FLT_MAX;
		m_StaticSampler[0].MaxLOD = D3D12_FLOAT32_MAX;
		m_StaticSampler[0].ShaderRegister = 0;
		m_StaticSampler[0].RegisterSpace = 0;
		m_StaticSampler[0].ShaderRegister = D3D12_SHADER_VISIBILITY_PIXEL;
	}

	m_RootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init(m_RootParameter.Size(), m_RootParameter.Get(), m_StaticSampler.Size(), m_StaticSampler.Get(), m_RootSignatureFlags);

	ComPtr<ID3DBlob> pSignautreBlob = nullptr;

	ThrowIfFailed(
		D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, pSignautreBlob.GetAddressOf(), nullptr)
	);

	ThrowIfFailed(
		DEVICE->CreateRootSignature(0, pSignautreBlob->GetBufferPointer(), pSignautreBlob->GetBufferSize(), IID_PPV_ARGS(m_pRootSignature.GetAddressOf()))
	);


	return TRUE;
}

////////////////////
//    Pipeline    //
////////////////////

BOOL BasicForwardPipeline::Initialize(shared_ptr<RootSignature> rootSignature)
{
	auto vs = SHADER.GetShader<VertexShader>("testVS");
	auto ps = SHADER.GetShader<PixelShader>("testPS");

	{
		m_Desc.InputLayout = { BasicInput::descs.data(), (UINT)BasicInput::descs.size() };
		m_Desc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBlob()->GetBufferPointer(), vs->GetBlob()->GetBufferSize());
		m_Desc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBlob()->GetBufferPointer(), ps->GetBlob()->GetBufferSize());
		m_Desc.pRootSignature = rootSignature->Get();
		m_Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		m_Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		m_Desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		m_Desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		m_Desc.DepthStencilState.StencilEnable = FALSE;
		m_Desc.DepthStencilState.DepthEnable = TRUE;
		m_Desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		m_Desc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		m_Desc.RasterizerState.DepthClipEnable = TRUE;
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

////////////////////////
//    RenderMethod    //
////////////////////////

void ForwardRender::Initialize(std::shared_ptr<Object> owner)
{
	m_wpOwner = owner;

	m_dwPassCount = 1;
	m_dwRTVCount = 1;

	// Root Signature
	m_RootSignatures.resize(m_dwPassCount);
	m_RootSignatures[0] = make_shared<BasicForwardRootSignature>();
	m_RootSignatures[0]->Initialize();

	// Pipeline
	m_Pipelines.resize(m_dwPassCount);
	m_Pipelines[0] = make_shared<BasicForwardPipeline>();
	m_Pipelines[0]->Initialize(m_RootSignatures[0]);

	// Constant Buffers
	m_upTransformCBuffer = make_unique<ConstantBuffer<CBTranformData>>();
	m_upCameraCBuffer = make_unique<ConstantBuffer<CBCameraData>>();
	m_upTransformCBuffer->Initialize();
	m_upCameraCBuffer->Initialize();

	// Descriptor Heap
	m_HeapDesc.NumDescriptors = DESCRIPTOR_COUNT_FOR_DRAW;
	m_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	m_upDescriptorHeap = make_unique<DescriptorHeap>();
	m_upDescriptorHeap->Initialize(m_HeapDesc);

}

void ForwardRender::Render()
{
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();

	auto owner = m_wpOwner.lock();

	shared_ptr<Transform> pTransform = owner->GetTransform();
	shared_ptr<Mesh<BasicVertexType>> pMesh = owner->GetMesh();
	VertexBuffer& refVertexBuffer = pMesh->GetBuffer()->vertexBuffer;
	IndexBuffer& refIndexBuffer = pMesh->GetBuffer()->indexBuffer;

	// TODO : Think hard for better descriptor heap use
	// TODO2 : Render!!!!!!!! 

	// 1. Write data in Constant Buffer
	CBTranformData data;
	data.matWorld = pTransform->GetWorldMatrixTransposed();
	m_upTransformCBuffer->PushData(data);
	m_upCameraCBuffer->PushData(CORE.GetMainCameraCBData());

	// 2. Get Descriptor from DescriptorHeap(m_upDescriptorHeap)
	ComPtr<ID3D12DescriptorHeap> pDescriptorHeap = m_upDescriptorHeap->pDescriptorHeap;
	Descriptor TransformDescriptorHandle = m_upDescriptorHeap->Alloc();
	Descriptor CameraDescriptorHandle = m_upDescriptorHeap->Alloc();

	// 3. Copy Constant Buffer Data to Descriptor
	DEVICE->CopyDescriptorsSimple(1, TransformDescriptorHandle.cpuHandle, m_upTransformCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DEVICE->CopyDescriptorsSimple(1, CameraDescriptorHandle.cpuHandle, m_upCameraCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Current pDescriptorHeap layout (Probably)
	// | CBV(Transform): b0 | CBV(Camera): b1 |

	pCommandList->SetGraphicsRootSignature(m_RootSignatures[0]->Get());
	pCommandList->SetDescriptorHeaps(1, pDescriptorHeap.GetAddressOf());

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);

	pCommandList->SetPipelineState(m_Pipelines[0]->Get());
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0, 1, &refVertexBuffer.VertexBufferView);
	pCommandList->IASetIndexBuffer(&refIndexBuffer.IndexBufferView);
	pCommandList->DrawIndexedInstanced(pMesh->GetIndexCount(), 1, 0, 0, 0);

	m_upDescriptorHeap->Reset();
}

//////////////////
//    Object    //
//////////////////

BOOL BasicForwardObject::Initialize()
{
	BOOL bResult = FALSE;
	bResult = InitRenderMethod();

	vector<BasicVertexType> vtx;
	vector<UINT> idx;

	MeshHelper::CreateBoxMesh(vtx, idx);
	m_pMesh = make_shared<Mesh<BasicVertexType>>();
	bResult = m_pMesh->Initialize(vtx, idx);

	m_pTransform = make_shared<Transform>();
	m_pTransform->Initialize();
	m_pTransform->SetWorldPosition(Vec3(0.f, 0.f, 5.f));

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

BOOL BasicForwardObject::InitRenderMethod()
{
	m_pRenderMethod = make_shared<ForwardRender>();
	m_pRenderMethod->Initialize(shared_from_this());

	return TRUE;
}
