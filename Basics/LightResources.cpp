#include "pch.h"
#include "LightResources.h"
#include "Mesh.h"
#include "LightDemo.h"
#include "MeshHelper.h"

using namespace Light;
using namespace std;

////////////////////////
// LightRootSignature //
////////////////////////

BOOL LightRootSignature::Initialize()
{
	m_DescriptorRanges.resize(1);
	m_DescriptorRanges[0].Resize(4);
	m_DescriptorRanges[0][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// b0 : Transform Data
	m_DescriptorRanges[0][1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	// b1 : Camera Data
	m_DescriptorRanges[0][2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);	// b2 : Color Data
	m_DescriptorRanges[0][3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 3);	// b3 : Light Data

	m_RootParameter.Resize(1);
	m_RootParameter[0].InitAsDescriptorTable(m_DescriptorRanges[0].Size(), m_DescriptorRanges[0].Get(), D3D12_SHADER_VISIBILITY_ALL);

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
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

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

///////////////////
// LightPipeline //
///////////////////

BOOL LightPipeline::Initialize(std::shared_ptr<class RootSignature> rootSignature)
{
	SHADER.CompileAndAddShader<VertexShader>("LightVS", L"../Shader/Light.hlsl", "VSMain");
	SHADER.CompileAndAddShader<PixelShader>("LightPS", L"../Shader/Light.hlsl", "PSMain");

	VertexShader& vs = *SHADER.GetShader<VertexShader>("LightVS");
	PixelShader& ps = *SHADER.GetShader<PixelShader>("LightPS");

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

/////////////////
// LightRender //
/////////////////

BOOL LightRender::Initialize(std::shared_ptr<Object> owner)
{
	m_wpOwner = owner;

	m_dwPassCount = 1;
	m_dwRTVCount = 1;

	// Root Signature
	m_RootSignatures.resize(m_dwPassCount);
	m_RootSignatures[0] = make_shared<LightRootSignature>();
	m_RootSignatures[0]->Initialize();

	// Pipeline
	m_Pipelines.resize(m_dwPassCount);
	m_Pipelines[0] = make_shared<LightPipeline>();
	m_Pipelines[0]->Initialize(m_RootSignatures[0]);

	// Constant Buffers
	m_upTransformCBuffer = make_unique<ConstantBuffer<CBModelTransformData>>();
	m_upCameraCBuffer = make_unique<ConstantBuffer<CBCameraData>>();
	m_upColorCBuffer = make_unique<ConstantBuffer<CBColorData>>();
	m_upLightCBuffer = make_unique<ConstantBuffer<CBLightData>>();

	m_upTransformCBuffer->Initialize();
	m_upCameraCBuffer->Initialize();
	m_upColorCBuffer->Initialize();
	m_upLightCBuffer->Initialize();

	// Descriptor Heap
	m_HeapDesc.NumDescriptors = DESCRIPTOR_COUNT_FOR_DRAW;
	m_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	m_upDescriptorHeap = make_unique<DescriptorHeap>();
	m_upDescriptorHeap->Initialize(m_HeapDesc);

	return TRUE;
}

void LightRender::Render()
{
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();

	LightObject& originOwner = *static_pointer_cast<LightObject>(m_wpOwner.lock());

	Transform& transform = *originOwner.GetTransform();
	Mesh<VertexType>& mesh = *originOwner.m_upMesh;
	ColorData& color = *originOwner.m_upColorData;
	VertexBuffer& vertexBuffer = originOwner.m_upMesh->GetBuffer()->vertexBuffer;
	IndexBuffer& indexBuffer = originOwner.m_upMesh->GetBuffer()->indexBuffer;

	// 1. Write data in Constant Buffer
	CBModelTransformData transformData;
	{
		transformData.matLocal = transform.GetLocalMatrixTransposed();
		transformData.matWorld = transform.GetWorldMatrixTransposed();
	}
	m_upTransformCBuffer->PushData(transformData);
	m_upCameraCBuffer->PushData(CORE.GetMainCameraCBData());
	m_upColorCBuffer->PushData(color.GetMaterialCBData());

	// Light Data is in Application(LightDemo)
	CBLightData data = static_pointer_cast<LightDemo>(GAME.GetGameDesc().app)->GetLightCBDataInDemo();
	m_upLightCBuffer->PushData(data);

	// 2. Get Descriptor from DescriptorHeap(m_upDescriptorHeap)
	ComPtr<ID3D12DescriptorHeap> pDescriptorHeap = m_upDescriptorHeap->pDescriptorHeap;
	Descriptor TransformDescriptorHandle = m_upDescriptorHeap->Alloc();
	Descriptor CameraDescriptorHandle = m_upDescriptorHeap->Alloc();
	Descriptor ColorDescriptorHandle = m_upDescriptorHeap->Alloc();
	Descriptor LightDescriptorHandle = m_upDescriptorHeap->Alloc();

	// 3. Copy Constant Buffer Data to Descriptor
	DEVICE->CopyDescriptorsSimple(1, TransformDescriptorHandle.cpuHandle, m_upTransformCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DEVICE->CopyDescriptorsSimple(1, CameraDescriptorHandle.cpuHandle, m_upCameraCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DEVICE->CopyDescriptorsSimple(1, ColorDescriptorHandle.cpuHandle, m_upColorCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DEVICE->CopyDescriptorsSimple(1, LightDescriptorHandle.cpuHandle, m_upLightCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Current pDescriptorHeap layout (Probably)
	// | CBV(Transform): b0 | CBV(Camera): b1 | CBV(Color): b2 | CBV(Light): b3 |

	pCommandList->SetGraphicsRootSignature(m_RootSignatures[0]->Get());
	pCommandList->SetDescriptorHeaps(1, pDescriptorHeap.GetAddressOf());

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);

	pCommandList->SetPipelineState(m_Pipelines[0]->Get());
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0, 1, &vertexBuffer.VertexBufferView);
	pCommandList->IASetIndexBuffer(&indexBuffer.IndexBufferView);
	pCommandList->DrawIndexedInstanced(mesh.GetIndexCount(), 1, 0, 0, 0);

	m_upDescriptorHeap->Reset();

}

/////////////////
// LightObject //
/////////////////

BOOL LightObject::Initialize()
{
	vector<VertexType> vtx;
	vector<UINT> idx;

	MeshHelper::CreateSphere(vtx, idx);

	m_upMesh = make_unique<Mesh<VertexType>>();
	m_upMesh->Initialize(vtx, idx);

	m_upTransform = make_unique<Transform>();
	m_upTransform->Initialize();

	m_upColorData = make_unique<ColorData>();
	{
		m_upColorData->colorDiffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		m_upColorData->colorSpecular = XMFLOAT4(1.f, 1.f, 1.f, 100.f);
		m_upColorData->colorAmbient = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		m_upColorData->colorEmissive = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	}

	InitRenderMethod();

	return TRUE;
}

void LightObject::Update()
{
	m_upTransform->Update();
}

void LightObject::Render()
{
	m_upRenderMethod->Render();
}

BOOL LightObject::InitRenderMethod()
{
	m_upRenderMethod = make_unique<LightRender>();
	m_upRenderMethod->Initialize(shared_from_this());

	return TRUE;
}
