#include "pch.h"
#include "NormalMappingResources.h"
#include "MeshHelper.h"
// For Lights
#include "Light.h"
#include "NormalMappingDemo.h"

using namespace std;
using namespace NormalMapping;

////////////////////////////////
// NormalMappingRootSignature //
////////////////////////////////

BOOL NormalMappingRootSignature::Initialize()
{
	m_DescriptorRanges.resize(2);

	m_DescriptorRanges[0].Resize(4);
	m_DescriptorRanges[0][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// b0 : Transform Data
	m_DescriptorRanges[0][1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	// b1 : Camera Data
	m_DescriptorRanges[0][2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);	// b2 : Color Data
	m_DescriptorRanges[0][3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 3);	// b3 : Light Data

	m_DescriptorRanges[1].Resize(2);
	m_DescriptorRanges[1][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// t1 : Diffuse
	m_DescriptorRanges[1][1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	// t1 : Normal

	m_RootParameter.Resize(2);
	m_RootParameter[0].InitAsDescriptorTable(m_DescriptorRanges[0].Size(), m_DescriptorRanges[0].Get(), D3D12_SHADER_VISIBILITY_ALL);
	m_RootParameter[1].InitAsDescriptorTable(m_DescriptorRanges[1].Size(), m_DescriptorRanges[1].Get(), D3D12_SHADER_VISIBILITY_ALL);

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
	ComPtr<ID3DBlob> pErrorBlob = nullptr;

	if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, pSignautreBlob.GetAddressOf(), pErrorBlob.GetAddressOf())))
	{
		OutputDebugStringA((const char*)pErrorBlob->GetBufferPointer());
		__debugbreak();
	}

	ThrowIfFailed(
		DEVICE->CreateRootSignature(0, pSignautreBlob->GetBufferPointer(), pSignautreBlob->GetBufferSize(), IID_PPV_ARGS(m_pRootSignature.GetAddressOf()))
	);


	return TRUE;
}

///////////////////////////
// NormalMappingPipeline //
///////////////////////////

BOOL NormalMappingPipeline::Initialize(std::shared_ptr<class RootSignature> rootSignature)
{
	SHADER.CompileAndAddShader<VertexShader>("NormalMappingVS", L"../Shader/NormalMapping.hlsl", "VSMain");
	SHADER.CompileAndAddShader<PixelShader>("NormalMappingPS", L"../Shader/NormalMapping.hlsl", "PSMain");

	VertexShader& vs = *SHADER.GetShader<VertexShader>("NormalMappingVS");
	PixelShader& ps = *SHADER.GetShader<PixelShader>("NormalMappingPS");

	// Set Pipeline State
	{
		m_Desc.InputLayout = { DefaultInput::descs.data(), (UINT)DefaultInput::descs.size() };
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

/////////////////////////
// NormalMappingRender //
/////////////////////////

BOOL NormalMappingRender::Initialize(std::shared_ptr<Object> owner)
{
	m_wpOwner = owner;

	m_dwPassCount = 1;
	m_dwRTVCount = 1;

	// Root Signature
	m_RootSignatures.resize(m_dwPassCount);
	m_RootSignatures[0] = make_shared<NormalMappingRootSignature>();
	m_RootSignatures[0]->Initialize();

	// Pipeline
	m_Pipelines.resize(m_dwPassCount);
	m_Pipelines[0] = make_shared<NormalMappingPipeline>();
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
	m_HeapDesc.NumDescriptors = m_RootSignatures[0]->GetDescriptorCount();
	m_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	m_upDescriptorHeap = make_unique<DescriptorHeap>();
	m_upDescriptorHeap->Initialize(m_HeapDesc);

	return TRUE;
}

void NormalMappingRender::Render(std::shared_ptr<Camera> pCamera)
{
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();

	auto owner = m_wpOwner.lock();
	NormalMappingObject& originOwner = *static_pointer_cast<NormalMappingObject>(owner);

	Transform& transform = *originOwner.m_upTransform;
	Mesh<VertexType>& mesh = *originOwner.m_upMesh;
	ColorData& color = *originOwner.m_upColorData;
	VertexBuffer& vertexBuffer = originOwner.m_upMesh->GetBuffer()->vertexBuffer;
	IndexBuffer& indexBuffer = originOwner.m_upMesh->GetBuffer()->indexBuffer;

	Texture& texDiffuse = *originOwner.m_upDiffuseTexture;
	Texture& texNormal = *originOwner.m_upNormalTexture;

	// 1. Write data in Constant Buffer
	m_upTransformCBuffer->PushData({ transform.GetLocalMatrixTransposed(), transform.GetWorldMatrixTransposed() });
	m_upCameraCBuffer->PushData(pCamera->GetCameraCBData());
	m_upColorCBuffer->PushData(color.GetMaterialCBData());

	CBLightData lightData = static_pointer_cast<NormalMappingDemo>(GAME.GetGameDesc().app)->GetLightCBDataInDemo();
	m_upLightCBuffer->PushData(lightData);

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

	// 4. Get Descriptor for texture
	Descriptor TexDiffuseDescriptorHandle = m_upDescriptorHeap->Alloc();
	Descriptor TexNormalDescriptorHandle = m_upDescriptorHeap->Alloc();

	// 5. Copy texture data to Descriptor
	DEVICE->CopyDescriptorsSimple(1, TexDiffuseDescriptorHandle.cpuHandle, texDiffuse.GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DEVICE->CopyDescriptorsSimple(1, TexNormalDescriptorHandle.cpuHandle, texNormal.GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 6. Root Signature first
	pCommandList->SetGraphicsRootSignature(m_RootSignatures[0]->Get());

	// 7. Set Descriptor heap
	pCommandList->SetDescriptorHeaps(1, pDescriptorHeap.GetAddressOf());

	// 8. Set Root Parameter[0]
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);

	// 9. Set Root Parameter[1]
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandleForTexture(gpuHandle, DESCRIPTOR_COUNT_FOR_TABLE_0, DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	pCommandList->SetGraphicsRootDescriptorTable(1, gpuHandleForTexture);

	// 10. Set pipeline and draw
	pCommandList->SetPipelineState(m_Pipelines[0]->Get());
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0, 1, &vertexBuffer.VertexBufferView);
	pCommandList->IASetIndexBuffer(&indexBuffer.IndexBufferView);
	pCommandList->DrawIndexedInstanced(mesh.GetIndexCount(), 1, 0, 0, 0);

	m_upDescriptorHeap->Reset();


}

/////////////////////////
// NormalMappingObject //
/////////////////////////

BOOL NormalMappingObject::Initialize()
{
	m_upTransform = make_unique<Transform>();
	m_upTransform->Initialize();

	vector<VertexType> vtx;
	vector<UINT> idx;

	MeshHelper::CreatePlane(vtx, idx);

	m_upMesh = make_unique<Mesh<VertexType>>();
	m_upMesh->Initialize(vtx, idx);

	m_upColorData = make_unique<ColorData>();
	{
		m_upColorData->colorDiffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		m_upColorData->colorSpecular = XMFLOAT4(1.f, 1.f, 1.f, 100.f);
		m_upColorData->colorAmbient = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		m_upColorData->colorEmissive = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	}

	TEXTURE.CreateAndAddTexuture("Brick_Diffuse", L"../Models/Texture/brick_wall_001_diffuse_4k.png");
	TEXTURE.CreateAndAddTexuture("Brick_Normal", L"../Models/Texture/brick_wall_001_nor_gl_4k.png");

	m_upDiffuseTexture = TEXTURE.GetTexture("Brick_Diffuse");
	m_upNormalTexture = TEXTURE.GetTexture("Brick_Normal");

	InitRenderMethod();

	return TRUE;
}

void NormalMappingObject::Update()
{
	m_upTransform->Update();
}

void NormalMappingObject::Render(std::shared_ptr<Camera> pCamera)
{
	m_upRenderMethod->Render(pCamera);
}

BOOL NormalMappingObject::InitRenderMethod()
{
	m_upRenderMethod = make_unique<NormalMappingRender>();
	m_upRenderMethod->Initialize(shared_from_this());

	return TRUE;
}
