#include "pch.h"
#include "Terrain.h"
#include "Texture.h"
#include "MeshHelper.h"
using namespace std;

///////////////////
// RootSignature // 
///////////////////

BOOL TerrainRootSignature::Initialize()
{
	m_DescriptorRanges.resize(2);

	m_DescriptorRanges[0].Resize(2);
	m_DescriptorRanges[0][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// b0 : World Transform
	m_DescriptorRanges[0][1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	// b1 : Camera Data

	m_DescriptorRanges[1].Resize(1);
	m_DescriptorRanges[1][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// t1 : Diffuse

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

//////////////
// Pipeline // 
//////////////

BOOL TerrainPipeline::Initialize(std::shared_ptr<class RootSignature> rootSignature)
{
	SHADER.CompileAndAddShader<VertexShader>("TerrainVS", L"../Shader/Terrain.hlsl", "VSMain");
	SHADER.CompileAndAddShader<PixelShader>("TerrainPS", L"../Shader/Terrain.hlsl", "PSMain");

	VertexShader& vs = *SHADER.GetShader<VertexShader>("TerrainVS");
	PixelShader& ps = *SHADER.GetShader<PixelShader>("TerrainPS");

	// Set Pipeline State
	{
		m_Desc.InputLayout = { TerrainInput::descs.data(), (UINT)TerrainInput::descs.size() };
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

//////////////////
// RenderMethod // 
//////////////////

BOOL TerrainRender::Initialize(std::shared_ptr<Object> owner)
{
	m_wpOwner = owner;

	m_dwPassCount = 1;
	m_dwRTVCount = 1;

	// Root Signature
	m_RootSignatures.resize(m_dwPassCount);
	m_RootSignatures[0] = make_shared<TerrainRootSignature>();
	m_RootSignatures[0]->Initialize();

	// Pipeline
	m_Pipelines.resize(m_dwPassCount);
	m_Pipelines[0] = make_shared<TerrainPipeline>();
	m_Pipelines[0]->Initialize(m_RootSignatures[0]);

	// Constant Buffers
	m_upWorldTransformCBuffer = make_unique<ConstantBuffer<XMFLOAT4X4>>();
	m_upCameraCBuffer = make_unique<ConstantBuffer<CBCameraData>>();

	m_upWorldTransformCBuffer->Initialize();
	m_upCameraCBuffer->Initialize();

	// Descriptor Heap
	m_HeapDesc.NumDescriptors = m_RootSignatures[0]->GetDescriptorCount();
	m_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	m_upDescriptorHeap = make_unique<DescriptorHeap>();
	m_upDescriptorHeap->Initialize(m_HeapDesc);

	return TRUE;
}

void TerrainRender::Render(std::shared_ptr<Camera> pCamera)
{
	// TODO : RENDER
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();

	auto owner = m_wpOwner.lock();
	Terrain& originOwner = *static_pointer_cast<Terrain>(owner);

	Mesh<TerrainVertexType>& mesh = *originOwner.m_pTerrainMesh;
	XMFLOAT4X4& matWorld = originOwner.m_matWorld;
	VertexBuffer& vertexBuffer = originOwner.m_pTerrainMesh->GetBuffer()->vertexBuffer;
	IndexBuffer& indexBuffer = originOwner.m_pTerrainMesh->GetBuffer()->indexBuffer;

	Texture& texDiffuse = *originOwner.m_pDiffuseTexture;

	// 1. Write data in Constant Buffer
	m_upWorldTransformCBuffer->PushData(matWorld);
	m_upCameraCBuffer->PushData(pCamera->GetCameraCBData());

	// 2. Get Descriptor from DescriptorHeap(m_upDescriptorHeap)
	ComPtr<ID3D12DescriptorHeap> pDescriptorHeap = m_upDescriptorHeap->pDescriptorHeap;
	Descriptor TransformDescriptorHandle = m_upDescriptorHeap->Alloc();
	Descriptor CameraDescriptorHandle = m_upDescriptorHeap->Alloc();

	// 3. Copy Constant Buffer Data to Descriptor
	DEVICE->CopyDescriptorsSimple(1, TransformDescriptorHandle.cpuHandle, m_upWorldTransformCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DEVICE->CopyDescriptorsSimple(1, CameraDescriptorHandle.cpuHandle, m_upCameraCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 4. Get Descriptor for texture
	Descriptor TexDiffuseDescriptorHandle = m_upDescriptorHeap->Alloc();

	// 5. Copy texture data to Descriptor
	DEVICE->CopyDescriptorsSimple(1, TexDiffuseDescriptorHandle.cpuHandle, texDiffuse.GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

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

//////////////////////
// TerrainHeightmap //
//////////////////////

TerrainHeightmap::TerrainHeightmap()
{
}

TerrainHeightmap::~TerrainHeightmap()
{
}

BOOL TerrainHeightmap::Initialize(std::wstring wstrFilePath)
{
	DirectX::TexMetadata data;
	
	if (FAILED(::LoadFromWICFile(wstrFilePath.c_str(), WIC_FLAGS_FORCE_RGB, &data, m_HeightMapData)))
	{
		::OutputDebugStringA("Heightmap doesn't exist");
		__debugbreak();
		return FALSE;
	}

	m_uiWidth = data.width;
	m_uiHeight = data.height;

	return TRUE;
}

/////////////
// Terrain //
/////////////

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

BOOL Terrain::Initialize(std::wstring wstrDiffPath, std::wstring wstrHeightPath)
{
	// TODO : make MeshHelper::CreateGrid()

	// Create Terrain Heightmap
	m_pTerrainHeightmap = make_shared<TerrainHeightmap>();
	m_pTerrainHeightmap->Initialize(wstrHeightPath);

	// Create Grid
	vector<TerrainVertexType>	vtx = {};
	vector<UINT>				idx = {};

	MeshHelper::CreateTerrainGrid(vtx, idx, m_pTerrainHeightmap->m_uiWidth, m_pTerrainHeightmap->m_uiHeight);

	// Adjust Height
	AdjustTerrainHeight(vtx);

	// Create Mesh
	m_pTerrainMesh = make_unique<Mesh<TerrainVertexType>>();
	m_pTerrainMesh->Initialize(vtx, idx);

	// Create Diffuse Texture
	TEXTURE.CreateAndAddTexuture("Terrain_Diffuse", wstrDiffPath.c_str());
	m_pDiffuseTexture = TEXTURE.GetTexture("Terrain_Diffuse");

	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());

	// Init RenderMethod
	InitRenderMethod();

	return 0;
}

BOOL Terrain::Initialize()
{
	OutputDebugStringA("Wrong Initialize Function Called\n");
	__debugbreak();
	return FALSE;
}

void Terrain::Update()
{
	// PASS
}

void Terrain::Render(std::shared_ptr<Camera> pCamera)
{
	m_upRenderMethod->Render(pCamera);
}

BOOL Terrain::InitRenderMethod()
{
	m_upRenderMethod = make_unique<TerrainRender>();
	m_upRenderMethod->Initialize(shared_from_this());

	return TRUE;
}

void Terrain::AdjustTerrainHeight(std::vector<TerrainVertexType>& vtx)
{
	DirectX::ScratchImage& mapData = m_pTerrainHeightmap->m_HeightMapData;
	uint8_t* pixelData = mapData.GetPixels();

	UINT terrainWidth = m_pTerrainHeightmap->m_uiWidth;
	UINT terrainHeight = m_pTerrainHeightmap->m_uiHeight;

	for (int z = 0; z < terrainHeight; z++)
	{
		for (int x = 0; x < terrainWidth; x++)
		{
			UINT idx = terrainWidth * z + x;

			uint8_t height = pixelData[idx] / 255.f * 25.f;
			vtx[idx].Position.y = height;
		}
	}


}

