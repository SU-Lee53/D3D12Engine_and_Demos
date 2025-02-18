#include "pch.h"
#include "Skybox.h"
#include "Texture.h"
#include "MeshHelper.h"

using namespace std;

/////////////////////////
// Pass 1 RootSignature //
/////////////////////////

BOOL SkyboxRootSignaturePass1::Initialize()
{
	m_DescriptorRanges.resize(1);

	m_DescriptorRanges[0].Resize(1);
	m_DescriptorRanges[0][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// t0 : Skybox original texture

	m_RootParameter.Resize(m_DescriptorRanges.size());
	m_RootParameter[0].InitAsDescriptorTable(m_DescriptorRanges[0].Size(), m_DescriptorRanges[0].Get(), D3D12_SHADER_VISIBILITY_ALL);

	m_StaticSampler.Resize(1);
	{
		m_StaticSampler[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		m_StaticSampler[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		m_StaticSampler[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		m_StaticSampler[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		m_StaticSampler[0].MipLODBias = 0.0f;
		m_StaticSampler[0].MaxAnisotropy = 1;
		m_StaticSampler[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		m_StaticSampler[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		m_StaticSampler[0].MinLOD = 0.0f;
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

/////////////////////
// Pass 1 Pipeline //
/////////////////////

BOOL SkyboxPipelinePass1::Initialize(std::shared_ptr<class RootSignature> rootSignature)
{
	SHADER.CompileAndAddShader<VertexShader>("SkyboxPass1VS", L"../Shader/Skybox_Pass1.hlsl", "VSMain");
	SHADER.CompileAndAddShader<PixelShader>("SkyboxPass1PS", L"../Shader/Skybox_Pass1.hlsl", "PSMain");

	VertexShader& vs = *SHADER.GetShader<VertexShader>("SkyboxPass1VS");
	PixelShader& ps = *SHADER.GetShader<PixelShader>("SkyboxPass1PS");

	// Set Pipeline State
	{
		m_Desc.InputLayout = { CubemapInput::descs.data(), (UINT)CubemapInput::descs.size() };
		m_Desc.VS = CD3DX12_SHADER_BYTECODE(vs.GetBlob()->GetBufferPointer(), vs.GetBlob()->GetBufferSize());
		m_Desc.PS = CD3DX12_SHADER_BYTECODE(ps.GetBlob()->GetBufferPointer(), ps.GetBlob()->GetBufferSize());
		m_Desc.pRootSignature = rootSignature->Get();
		m_Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		m_Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		m_Desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		m_Desc.BlendState.RenderTarget[1].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		m_Desc.BlendState.RenderTarget[2].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		m_Desc.BlendState.RenderTarget[3].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		m_Desc.BlendState.RenderTarget[4].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		m_Desc.BlendState.RenderTarget[5].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		m_Desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		m_Desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		m_Desc.DepthStencilState.StencilEnable = FALSE;
		m_Desc.DepthStencilState.DepthEnable = TRUE;
		m_Desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		m_Desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_Desc.RasterizerState.DepthClipEnable = TRUE;
		m_Desc.SampleMask = UINT_MAX;
		m_Desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		m_Desc.NumRenderTargets = 6;
		m_Desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;	// Fortmat from Skybox::m_pCubeMapTexture
		m_Desc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_Desc.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_Desc.RTVFormats[3] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_Desc.RTVFormats[4] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_Desc.RTVFormats[5] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_Desc.DSVFormat = DXGI_FORMAT_UNKNOWN;				// DSV is Unused
		m_Desc.SampleDesc.Count = 1;
	}

	ThrowIfFailed(
		DEVICE->CreateGraphicsPipelineState(&m_Desc, IID_PPV_ARGS(m_PipelineState.GetAddressOf()))
	);

	return TRUE;
}

/////////////////////////
// Pass 2 RootSignature //
/////////////////////////

BOOL SkyboxRootSignaturePass2::Initialize()
{
	m_DescriptorRanges.resize(2);

	m_DescriptorRanges[0].Resize(1);
	m_DescriptorRanges[0][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// b0 : view/proj per each faces (except translation infos)

	m_DescriptorRanges[1].Resize(1);
	m_DescriptorRanges[1][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// t0 : Cube Map texture

	m_RootParameter.Resize(m_DescriptorRanges.size());
	m_RootParameter[0].InitAsDescriptorTable(m_DescriptorRanges[0].Size(), m_DescriptorRanges[0].Get(), D3D12_SHADER_VISIBILITY_VERTEX);
	m_RootParameter[1].InitAsDescriptorTable(m_DescriptorRanges[1].Size(), m_DescriptorRanges[1].Get(), D3D12_SHADER_VISIBILITY_PIXEL);

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

/////////////////////
// Pass 2 Pipeline //
/////////////////////

BOOL SkyboxPipelinePass2::Initialize(std::shared_ptr<class RootSignature> rootSignature)
{
	SHADER.CompileAndAddShader<VertexShader>("SkyboxPass2VS", L"../Shader/Skybox_Pass2.hlsl", "VSMain");
	SHADER.CompileAndAddShader<PixelShader>("SkyboxPass2PS", L"../Shader/Skybox_Pass2.hlsl", "PSMain");

	VertexShader& vs = *SHADER.GetShader<VertexShader>("SkyboxPass2VS");
	PixelShader& ps = *SHADER.GetShader<PixelShader>("SkyboxPass2PS");

	// Set Pipeline State
	{
		m_Desc.InputLayout = { SkyboxVertexType::descs.data(), (UINT)SkyboxVertexType::descs.size() };
		m_Desc.VS = CD3DX12_SHADER_BYTECODE(vs.GetBlob()->GetBufferPointer(), vs.GetBlob()->GetBufferSize());
		m_Desc.PS = CD3DX12_SHADER_BYTECODE(ps.GetBlob()->GetBufferPointer(), ps.GetBlob()->GetBufferSize());
		m_Desc.pRootSignature = rootSignature->Get();
		m_Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		m_Desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		m_Desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		m_Desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		m_Desc.DepthStencilState.StencilEnable = FALSE;
		m_Desc.DepthStencilState.DepthEnable = TRUE;
		m_Desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		m_Desc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;	// Skybox needs cull front
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

BOOL SkyboxRender::Initialize(std::shared_ptr<Object> pOwner)
{
	m_wpOwner = pOwner;

	m_dwPassCount = 2;
	m_dwRTVCount = 2;

	// Root Signature
	m_RootSignatures.resize(m_dwPassCount);
	m_RootSignatures[0] = make_shared<SkyboxRootSignaturePass1>();
	m_RootSignatures[0]->Initialize();
	m_RootSignatures[1] = make_shared<SkyboxRootSignaturePass2>();
	m_RootSignatures[1]->Initialize();

	// Pipeline
	m_Pipelines.resize(m_dwPassCount);
	m_Pipelines[0] = make_shared<SkyboxPipelinePass1>();
	m_Pipelines[0]->Initialize(m_RootSignatures[0]);
	m_Pipelines[1] = make_shared<SkyboxPipelinePass2>();
	m_Pipelines[1]->Initialize(m_RootSignatures[1]);

	// Constant Buffers
	m_upPass2CameraCBuffer = make_unique<ConstantBuffer<CBCameraData>>();

	m_upPass2CameraCBuffer->Initialize();

	// Descriptor Heap Pass 1
	m_HeapDescForPass1.NumDescriptors = DESCRIPTOR_COUNT_FOR_PASS_1;	// Need to valid while draw 6 faces
	m_HeapDescForPass1.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_HeapDescForPass1.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	m_upDescriptorHeapForPass1 = make_unique<DescriptorHeap>();
	m_upDescriptorHeapForPass1->Initialize(m_HeapDescForPass1);


	// Descriptor Heap Pass 2
	m_HeapDesc.NumDescriptors = DESCRIPTOR_COUNT_FOR_PASS_2;
	m_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	m_upDescriptorHeap = make_unique<DescriptorHeap>();
	m_upDescriptorHeap->Initialize(m_HeapDesc);

	return TRUE;
}

void SkyboxRender::Render(std::shared_ptr<Camera> pCamera)
{

	D3D12_RESOURCE_STATES& currentState = static_pointer_cast<Skybox>(m_wpOwner.lock())->m_pCubeMap->m_CurrentResourceState;
	if (currentState != D3D12_RESOURCE_STATE_RENDER_TARGET)
	{
		ChangeResourceStateToRenderTarget(currentState);
	}
	Pass1();

	ChangeResourceStateToShaderResource(currentState);
	Pass2(pCamera);
}

void SkyboxRender::ChangeResourceStateToRenderTarget(D3D12_RESOURCE_STATES currentState)
{
	SkyboxCubeMap& cubeMap = *static_pointer_cast<Skybox>(m_wpOwner.lock())->m_pCubeMap;
	cubeMap.ChangeResourceStateToRenderTarget(currentState);
}

void SkyboxRender::Pass1(std::shared_ptr<Camera> pCamera)
{
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();

	shared_ptr<Skybox> owner = static_pointer_cast<Skybox>(m_wpOwner.lock());
	SkyboxCubeMap& cubeMap = *owner->m_pCubeMap;
	Texture& texSkybox = *owner->m_pSkyboxTexture;
	Mesh<CubemapVertexType>& mesh = *owner->m_pCubeMap->m_upQuadMesh;
	VertexBuffer& vertexBuffer = mesh.GetBuffer()->vertexBuffer;
	IndexBuffer& indexBuffer = mesh.GetBuffer()->indexBuffer;

	D3D12_VIEWPORT viewport;
	{
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;
		viewport.Width = (float)cubeMap.m_uiCubeMapSize;
		viewport.Height = (float)cubeMap.m_uiCubeMapSize;
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;
	}

	D3D12_RECT scissorRect;
	{
		scissorRect.left = 0;
		scissorRect.top = 0;
		scissorRect.right = (LONG)cubeMap.m_uiCubeMapSize;
		scissorRect.bottom = (LONG)cubeMap.m_uiCubeMapSize;
	}

	// 1. Set Render Target at once (MRT) 
	// ++ Set CameraData for Pass 1 ConstantBuffer here
	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandles[6];
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(cubeMap.m_pRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	for (int i = 0; i < 6; i++)
	{
		RTVHandles[i] = rtvHandle;
		pCommandList->ClearRenderTargetView(RTVHandles[i], clearColor, 0, nullptr);

		rtvHandle.Offset(1, cubeMap.m_uiRTVDescriptorSize);
	}

	pCommandList->RSSetViewports(1, &viewport);
	pCommandList->RSSetScissorRects(1, &scissorRect);

	// 2. Push CameraData to ConstantBuffer
	pCommandList->OMSetRenderTargets(6, RTVHandles, FALSE, nullptr);

	// 3. Set Root Signature
	pCommandList->SetGraphicsRootSignature(m_RootSignatures[0]->Get());

	// 4. Get Descriptor from DescriptorHeap(m_upDescriptorHeap)
	ComPtr<ID3D12DescriptorHeap> pDescriptorHeap = m_upDescriptorHeapForPass1->pDescriptorHeap;
	Descriptor TextureDescriptorHandle = m_upDescriptorHeapForPass1->Alloc();

	// 5. Copy ConstantBuffer & Texture to Descriptor
	DEVICE->CopyDescriptorsSimple(1, TextureDescriptorHandle.cpuHandle, texSkybox.GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 6. Set Descriptor Heap
	pCommandList->SetDescriptorHeaps(1, pDescriptorHeap.GetAddressOf());

	// 7. Set Root Parameters
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);

	// 8. Set Pipeline and draw
	pCommandList->SetPipelineState(m_Pipelines[0]->Get());
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0, 1, &vertexBuffer.VertexBufferView);
	pCommandList->IASetIndexBuffer(&indexBuffer.IndexBufferView);

	// 9. Instancing 6 Times -> InstancedID will be direction index
	pCommandList->DrawInstanced(mesh.GetVertexCount(), 1, 0, 0);

	m_upDescriptorHeapForPass1->Reset();

}

void SkyboxRender::ChangeResourceStateToShaderResource(D3D12_RESOURCE_STATES currentState)
{
	SkyboxCubeMap& cubeMap = *static_pointer_cast<Skybox>(m_wpOwner.lock())->m_pCubeMap;
	cubeMap.ChangeResourceStateToShaderResource(currentState);
}

void SkyboxRender::Pass2(std::shared_ptr<Camera> pCamera)
{
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();

	// Set Render Target as Back Buffer
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(CORE.GetRTVHeap()->GetCPUDescriptorHandleForHeapStart(), CORE.GetCurrentRenderTargetIndex(), CORE.GetRTVDescriptorSize());
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(CORE.GetDSVHeap()->GetCPUDescriptorHandleForHeapStart());

	// RTV's Resource State is maybe already changed in RenderManager::RenderBegin
	Color BackColor = GAME.GetGameDesc().BackColor;
	pCommandList->ClearRenderTargetView(rtvHandle, (float*)&BackColor, 0, nullptr);
	pCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);

	pCommandList->RSSetViewports(1, &CORE.GetViewport());
	pCommandList->RSSetScissorRects(1, &CORE.GetScissorRect());
	pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

	shared_ptr<Skybox> owner = static_pointer_cast<Skybox>(m_wpOwner.lock());
	SkyboxCubeMap& texSkybox = *owner->m_pCubeMap;
	Mesh<SkyboxVertexType>& mesh = *owner->m_upSkyboxMesh;
	VertexBuffer& vertexBuffer = owner->m_upSkyboxMesh->GetBuffer()->vertexBuffer;
	IndexBuffer& indexBuffer = owner->m_upSkyboxMesh->GetBuffer()->indexBuffer;

	m_upPass2CameraCBuffer->PushData(pCamera->GetCameraCBData());

	ComPtr<ID3D12DescriptorHeap> pDescriptorHeap = m_upDescriptorHeap->pDescriptorHeap;

	Descriptor CameraDescriptorHandle = m_upDescriptorHeap->Alloc();
	DEVICE->CopyDescriptorsSimple(1, CameraDescriptorHandle.cpuHandle, m_upPass2CameraCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	Descriptor TextureDescriptorHandle = m_upDescriptorHeap->Alloc();
	DEVICE->CopyDescriptorsSimple(1, TextureDescriptorHandle.cpuHandle, texSkybox.m_pCubeMapTexture->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 6. Root Signature first
	pCommandList->SetGraphicsRootSignature(m_RootSignatures[1]->Get());

	// 7. Set Descriptor heap
	pCommandList->SetDescriptorHeaps(1, pDescriptorHeap.GetAddressOf());

	// 8. Set Root Parameter[0]
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);

	// 9. Set Root Parameter[1]
	gpuHandle.Offset(1, DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	pCommandList->SetGraphicsRootDescriptorTable(1, gpuHandle);

	// 10. Set pipeline and draw
	pCommandList->SetPipelineState(m_Pipelines[1]->Get());
	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0, 1, &vertexBuffer.VertexBufferView);
	pCommandList->IASetIndexBuffer(&indexBuffer.IndexBufferView);
	pCommandList->DrawIndexedInstanced(mesh.GetIndexCount(), 1, 0, 0, 0);

	m_upDescriptorHeap->Reset();

}
///////////////////
// SkyboxCubeMap //
///////////////////

SkyboxCubeMap::SkyboxCubeMap()
{
}

SkyboxCubeMap::~SkyboxCubeMap()
{
}

BOOL SkyboxCubeMap::Initialize(const std::string& strTextureName, UINT uiCubeMapSize, UINT uiMipLevels)
{
	m_uiCubeMapSize = uiCubeMapSize;
	m_uiMipLevels = uiMipLevels;

	m_CurrentResourceState = D3D12_RESOURCE_STATE_RENDER_TARGET;

	TEXTURE.CreateBlankCubemapAndAddTexture(strTextureName, uiCubeMapSize, uiMipLevels, m_CurrentResourceState);
	m_pCubeMapTexture = TEXTURE.GetTexture(strTextureName);

	// Create Desriptor Heap for RTV
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	{
		rtvHeapDesc.NumDescriptors = 6;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	}

	DEVICE->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_pRTVDescriptorHeap.GetAddressOf()));
	m_uiRTVDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// Get Descriptor Handle from start
	CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(m_pRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	// Create Render Target View per face
	for (UINT face = 0; face < 6; face++)
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = {};
		RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;	// 2d array of texture
		RTVDesc.Texture2DArray.FirstArraySlice = face;	// face index
		RTVDesc.Texture2DArray.ArraySize = 1;	// one face per usage
		RTVDesc.Texture2DArray.MipSlice = 0;

		// Create RTV for current face
		DEVICE->CreateRenderTargetView(m_pCubeMapTexture->GetResource().Get(), &RTVDesc, RTVHandle);

		// Move rtv handle using offset
		RTVHandle.Offset(1, m_uiRTVDescriptorSize);
	}

	// Create Mesh for cube mapping
	vector<CubemapVertexType> vtx;
	vector<UINT> idx;
	MeshHelper::CreateQuadMeshForCubeMapping(vtx, idx);

	m_upQuadMesh = make_unique<Mesh<CubemapVertexType>>();
	m_upQuadMesh->Initialize(vtx, idx);

	// Create View 
	XMVECTOR vAts[6] =
	{
		XMVectorSet(1,0,0,0),	// (+x)
		XMVectorSet(-1,0,0,0),	// (-x)
		XMVectorSet(0,1,0,0),	// (+y)
		XMVectorSet(0,-1,0,0),	// (-y)
		XMVectorSet(0,0,1,0),	// (+z)
		XMVectorSet(0,0,-1,0)	// (-z)
	};

	XMVECTOR vUps[6] =
	{
		XMVectorSet(0,1,0,0),	// (+x)
		XMVectorSet(0,1,0,0),	// (-x)
		XMVectorSet(0,0,-1,0),	// (+y)
		XMVectorSet(0,0,1,0),	// (-y)
		XMVectorSet(0,1,0,0),	// (+z)
		XMVectorSet(0,1,0,0),	// (-z)
	};

	XMVECTOR vEye = XMVectorSet(0, 0, 0, 1);

	for (int i = 0; i < 6; i++)
	{
		XMMATRIX view = XMMatrixLookAtLH(vEye, vAts[i], vUps[i]);
		XMStoreFloat4x4(&m_ViewMatrices[i], view);
	}

	XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.f, 0.1f, 100.f);	// Fov 90, Ratio 1:1
	XMStoreFloat4x4(&m_ProjMatrix, projMatrix);

	return TRUE;
}

void SkyboxCubeMap::ChangeResourceStateToRenderTarget(D3D12_RESOURCE_STATES currentState)
{
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();
	ComPtr<ID3D12Resource>& refTarget = m_pCubeMapTexture->GetResource();

	D3D12_RESOURCE_STATES nextState = D3D12_RESOURCE_STATE_RENDER_TARGET;
	pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(refTarget.Get(), currentState, nextState));

	m_CurrentResourceState = nextState;
}

void SkyboxCubeMap::ChangeResourceStateToShaderResource(D3D12_RESOURCE_STATES currentState)
{
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();
	ComPtr<ID3D12Resource>& refTarget = m_pCubeMapTexture->GetResource();

	D3D12_RESOURCE_STATES nextState = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
	pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(refTarget.Get(), currentState, nextState));

	m_CurrentResourceState = nextState;
}

////////////
// Skybox //
////////////

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}

BOOL Skybox::Initialize(std::wstring wstrTexturePath)
{
	TEXTURE.CreateAndAddTexuture("Skybox", wstrTexturePath);
	m_pSkyboxTexture = TEXTURE.GetTexture("Skybox");

	UINT cubemapHeight = (UINT)m_pSkyboxTexture->GetTextureSize().y;

	m_pCubeMap = make_shared<SkyboxCubeMap>();
	m_pCubeMap->Initialize("CubeMap", cubemapHeight);

	vector<SkyboxVertexType> vtx = {};
	vector<UINT> idx = {};
	MeshHelper::CreateSkyboxCube(vtx, idx);
	
	m_upSkyboxMesh = make_unique<Mesh<SkyboxVertexType>>();
	m_upSkyboxMesh->Initialize(vtx, idx);

	InitRenderMethod();

	return TRUE;
}

BOOL Skybox::Initialize()
{
	OutputDebugStringA("Wrong Initialize Function Called\n");
	__debugbreak();
	return FALSE;
}

void Skybox::Update()
{
	// PASS
}

void Skybox::Render(std::shared_ptr<Camera> pCamera)
{
	m_upRenderMethod->Render(pCamera);
}

BOOL Skybox::InitRenderMethod()
{
	m_upRenderMethod = make_unique<SkyboxRender>();
	m_upRenderMethod->Initialize(shared_from_this());

	return TRUE;
}
