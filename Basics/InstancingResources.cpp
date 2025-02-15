#include "pch.h"
#include "InstancingResources.h"
#include "MeshHelper.h"
#include <random>

using namespace Instancing;
using namespace std;

/////////////////////////////
// InstancingRootSignature //
/////////////////////////////

BOOL InstancingRootSignature::Initialize()
{
	m_DescriptorRanges.resize(2);
	
	m_DescriptorRanges[0].Resize(1);
	m_DescriptorRanges[0][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// b0 : Camera Data

	m_DescriptorRanges[1].Resize(1);
	m_DescriptorRanges[1][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// t0 : Structured Buffer(Instancing Data)

	m_RootParameter.Resize(2);
	m_RootParameter[0].InitAsDescriptorTable(m_DescriptorRanges[0].Size(), m_DescriptorRanges[0].Get(), D3D12_SHADER_VISIBILITY_ALL);
	m_RootParameter[1].InitAsShaderResourceView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

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
		m_StaticSampler[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
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

////////////////////////
// InstancingPipeline //
////////////////////////

BOOL InstancingPipeline::Initialize(std::shared_ptr<class RootSignature> rootSignature)
{
	SHADER.CompileAndAddShader<VertexShader>("InstancingVS", L"../Shader/Instancing.hlsl", "VSMain");
	SHADER.CompileAndAddShader<PixelShader>("InstancingPS", L"../Shader/Instancing.hlsl", "PSMain");

	VertexShader& vs = *SHADER.GetShader<VertexShader>("InstancingVS");
	PixelShader& ps = *SHADER.GetShader<PixelShader>("InstancingPS");

	// Set Pipeline State
	{
		m_Desc.InputLayout = { VertexType::descs.data(), (UINT)VertexType::descs.size() };
		m_Desc.VS = CD3DX12_SHADER_BYTECODE(vs.GetBlob()->GetBufferPointer(), vs.GetBlob()->GetBufferSize());
		m_Desc.PS = CD3DX12_SHADER_BYTECODE(ps.GetBlob()->GetBufferPointer(), ps.GetBlob()->GetBufferSize());
		m_Desc.pRootSignature = rootSignature->Get();
		m_Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		m_Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		m_Desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		m_Desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		m_Desc.DepthStencilState.StencilEnable = FALSE;
		m_Desc.DepthStencilState.DepthEnable = TRUE;
		m_Desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		m_Desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
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

//////////////////////
// InstancingRender //
//////////////////////

BOOL InstancingRender::Initialize(std::shared_ptr<Object> owner)
{
	m_wpOwner = owner;

	m_dwPassCount = 1;
	m_dwRTVCount = 1;

	// Root Signature
	m_RootSignatures.resize(m_dwPassCount);
	m_RootSignatures[0] = make_shared<InstancingRootSignature>();
	m_RootSignatures[0]->Initialize();

	// Pipeline
	m_Pipelines.resize(m_dwPassCount);
	m_Pipelines[0] = make_shared<InstancingPipeline>();
	m_Pipelines[0]->Initialize(m_RootSignatures[0]);

	// Constant Buffers
	m_upCameraCBuffer = make_unique<ConstantBuffer<CBCameraData>>();
	m_upInstancingSBuffer = make_unique<StructuredBuffer<SBInstancingData, INSTANCED_COUNT>>();

	m_upCameraCBuffer->Initialize();
	m_upInstancingSBuffer->Initialize();

	// Descriptor Heap
	m_HeapDesc.NumDescriptors = DESCRIPTOR_COUNT_FOR_DRAW;
	m_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	m_upDescriptorHeap = make_unique<DescriptorHeap>();
	m_upDescriptorHeap->Initialize(m_HeapDesc);

	return TRUE;
}

void InstancingRender::Render(std::shared_ptr<Camera> pCamera)
{
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();

	InstancingObject& originOwner = *static_pointer_cast<InstancingObject>(m_wpOwner.lock());

	Transform& transform = *originOwner.GetTransform();
	Mesh<VertexType>& mesh = *originOwner.m_upMesh;
	VertexBuffer& vertexBuffer = originOwner.m_upMesh->GetBuffer()->vertexBuffer;
	IndexBuffer& indexBuffer = originOwner.m_upMesh->GetBuffer()->indexBuffer;

	pCommandList->SetGraphicsRootSignature(m_RootSignatures[0]->Get());

	// 1. Write data in Constant Buffer
	m_upCameraCBuffer->PushData(pCamera->GetCameraCBData());

	// 2. Get Descriptor from DescriptorHeap(m_upDescriptorHeap)
	ComPtr<ID3D12DescriptorHeap> pDescriptorHeap = m_upDescriptorHeap->pDescriptorHeap;
	Descriptor CameraDescriptorHandle = m_upDescriptorHeap->Alloc();

	// 3. Copy Constant Buffer Data to Descriptor
	DEVICE->CopyDescriptorsSimple(1, CameraDescriptorHandle.cpuHandle, m_upCameraCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	pCommandList->SetDescriptorHeaps(1, pDescriptorHeap.GetAddressOf());

	// Current pDescriptorHeap layout (Probably)
	// | CBV(Transform): b0 | 
	// -> If you write huge data like structuredbuffer to descriptor heap, your gpu might be completely mad!!
	// -> solution : use shader resource view bruh

	// 4. Set Structured Buffer and push data
	pCommandList->SetGraphicsRootShaderResourceView(1, m_upInstancingSBuffer->GetGPUVirtualAddress());
	m_upInstancingSBuffer->PushData(originOwner.m_InstancingDatas.data(), originOwner.m_InstancingDatas.size());

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);

	pCommandList->SetPipelineState(m_Pipelines[0]->Get());
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0, 1, &vertexBuffer.VertexBufferView);
	pCommandList->IASetIndexBuffer(&indexBuffer.IndexBufferView);
	pCommandList->DrawIndexedInstanced(mesh.GetIndexCount(), originOwner.m_nInstances, 0, 0, 0);

	m_upDescriptorHeap->Reset();

}

//////////////////////
// InstancingObject //
//////////////////////

BOOL InstancingObject::Initialize()
{
	m_upTransform = make_unique<Transform>();
	m_upTransform->Initialize();

	vector<VertexType> vtx;
	vector<UINT> idx;

	MeshHelper::CreateSphere(vtx, idx);

	m_upMesh = make_unique<Mesh<VertexType>>();
	m_upMesh->Initialize(vtx, idx);

	m_upTransform = make_unique<Transform>();
	m_upTransform->Initialize();

	m_InstancingDatas.clear();

	// std::random objs for random color
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis1(-MAX_POSITION, MAX_POSITION);
	std::uniform_real_distribution<float> dis2(0.0f, 1.0f);


	for (int x = 0; x < INSTANCED_COUNT; x++)
	{
		SBInstancingData data;
		
		array<float, 3> posValue = { 0.0f, 0.0f, 0.0f };
		std::for_each(posValue.begin(), posValue.end(), [&gen, &dis1](float& f) {f = dis1(gen); });
		XMFLOAT3 pos(posValue.data());
		XMVECTOR xmPos = XMLoadFloat3(&pos);

		XMMATRIX xmWorld = XMMatrixTranspose(XMMatrixTranslationFromVector(xmPos));
		XMStoreFloat4x4(&data.matWorld, xmWorld);
	
		array<float, 4> colorValues = {0.0f, 0.0f, 0.0f, 1.0f};
		std::for_each(colorValues.begin(), colorValues.end(), [&gen, &dis2](float& f) {f = dis2(gen); });
		data.colorDiffuse = XMFLOAT4(colorValues.data());
	
		m_InstancingDatas.push_back(data);
	}

	m_nInstances = m_InstancingDatas.size();

	InitRenderMethod();

	return TRUE;
}

void InstancingObject::Update()
{
	for (int i = 0; i < INSTANCED_COUNT; i++)
	{
		XMFLOAT4 color = m_InstancingDatas[i].colorDiffuse;
		
		color.x = (std::sinf(color.w) + 1.f) / 2.f;
		color.y = (std::cosf(color.w) + 1.f) / 2.f;
		color.z = (std::sinf(color.w + 3.14f / 2) + 1.f) / 2.f;

		color.w += 0.5f * DT;
		if (color.w > 3.14f) color.w = 0;

		m_InstancingDatas[i].colorDiffuse = color;
	}
}

void InstancingObject::Render(std::shared_ptr<Camera> pCamera)
{
	m_upRenderMethod->Render(pCamera);
}

BOOL InstancingObject::InitRenderMethod()
{
	m_upRenderMethod = make_unique<InstancingRender>();
	m_upRenderMethod->Initialize(shared_from_this());

	return TRUE;
}
