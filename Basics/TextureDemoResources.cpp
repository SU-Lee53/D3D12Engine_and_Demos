#include "pch.h"
#include "TextureDemoResources.h"
#include <fstream>

using namespace TextureDemoResource;
using namespace std;

BOOL TextureDemoRootSignature::Initialize()
{
	m_DescriptorRanges.resize(1);

	m_DescriptorRanges[0].Resize(4);
	m_DescriptorRanges[0][0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// b0 : Transform Data
	m_DescriptorRanges[0][1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	// b1 : Camera Data
	m_DescriptorRanges[0][2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);	// b2 : Color Data
	m_DescriptorRanges[0][3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

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

BOOL TextureDemoPipeline::Initialize(std::shared_ptr<class RootSignature> rootSignature)
{
	SHADER.CompileAndAddShader<VertexShader>("TextureVS", L"../Shader/Texture.hlsl", "VSMain");
	SHADER.CompileAndAddShader<PixelShader>("TexturePS", L"../Shader/Texture.hlsl", "PSMain");

	VertexShader& vs = *SHADER.GetShader<VertexShader>("TextureVS");
	PixelShader& ps = *SHADER.GetShader<PixelShader>("TexturePS");

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

BOOL TextureDemoRender::Initialize(std::shared_ptr<Object> owner)
{
	m_wpOwner = owner;

	m_dwPassCount = 1;
	m_dwRTVCount = 1;

	// Root Signature
	m_RootSignatures.resize(m_dwPassCount);
	m_RootSignatures[0] = make_shared<TextureDemoRootSignature>();
	m_RootSignatures[0]->Initialize();

	// Pipeline
	m_Pipelines.resize(m_dwPassCount);
	m_Pipelines[0] = make_shared<TextureDemoPipeline>();
	m_Pipelines[0]->Initialize(m_RootSignatures[0]);

	// Constant Buffers
	m_upTransformCBuffer = make_unique<ConstantBuffer<CBModelTransformData>>();
	m_upCameraCBuffer = make_unique<ConstantBuffer<CBCameraData>>();
	m_upColorCBuffer = make_unique<ConstantBuffer<CBColorData>>();
	m_upTransformCBuffer->Initialize();
	m_upCameraCBuffer->Initialize();
	m_upColorCBuffer->Initialize();

	// Descriptor Heap
	m_HeapDesc.NumDescriptors = DESCRIPTOR_COUNT_FOR_DRAW;
	m_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	m_upDescriptorHeap = make_unique<DescriptorHeap>();
	m_upDescriptorHeap->Initialize(m_HeapDesc);

	return TRUE;
}

void TextureDemoRender::Render()
{
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();

	auto owner = m_wpOwner.lock();
	shared_ptr<TextureDemoObject> originOwner = static_pointer_cast<TextureDemoObject>(owner);

	for (const auto& node : originOwner->m_pModelNodes)
	{
		Transform& transform = *node->pTransform;
		Mesh<VertexType>& mesh = *node->pMesh;
		VertexBuffer& vertexBuffer = node->pMesh->GetBuffer()->vertexBuffer;
		IndexBuffer& indexBuffer = node->pMesh->GetBuffer()->indexBuffer;
		Texture& texture = *node->pTexture;
		ColorData& color = *node->pColorData;

		// Set Root Signature first
		pCommandList->SetGraphicsRootSignature(m_RootSignatures[0]->Get());

		// Wirte data in ConstantBuffer
		m_upTransformCBuffer->PushData(CBModelTransformData{ transform.GetLocalMatrixTransposed(), transform.GetWorldMatrixTransposed() });
		m_upCameraCBuffer->PushData(CORE.GetMainCameraCBData());
		m_upColorCBuffer->PushData(color.GetMaterialCBData());

		// Get Descriptor Heap
		ComPtr<ID3D12DescriptorHeap> pDescriptorHeap = m_upDescriptorHeap->pDescriptorHeap;
		Descriptor TransformDescriptorHandle = m_upDescriptorHeap->Alloc();
		Descriptor CameraDescriptorHandle = m_upDescriptorHeap->Alloc();
		Descriptor ColorDescriptorHandle = m_upDescriptorHeap->Alloc();
		Descriptor TexDescriptorHandle = m_upDescriptorHeap->Alloc();

		// Copy Constant Buffer data to descriptor
		DEVICE->CopyDescriptorsSimple(1, TransformDescriptorHandle.cpuHandle, m_upTransformCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		DEVICE->CopyDescriptorsSimple(1, CameraDescriptorHandle.cpuHandle, m_upCameraCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		DEVICE->CopyDescriptorsSimple(1, ColorDescriptorHandle.cpuHandle, m_upColorCBuffer->GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		DEVICE->CopyDescriptorsSimple(1, TexDescriptorHandle.cpuHandle, texture.GetDescriptorHeap()->DescriptorHandleFromStart.cpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// Set Descriptor Heap to cmdList
		pCommandList->SetDescriptorHeaps(1, pDescriptorHeap.GetAddressOf());

		// Current pDescriptorHeap layout (Probably)
		// | CBV(Transform): b0 | CBV(Camera): b1 | CBV(Color): b2 | SRV(Texutre): t0 |

		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);
		
		pCommandList->SetPipelineState(m_Pipelines[0]->Get());
		pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pCommandList->IASetVertexBuffers(0, 1, &vertexBuffer.VertexBufferView);
		pCommandList->IASetIndexBuffer(&indexBuffer.IndexBufferView);
		pCommandList->DrawIndexedInstanced(mesh.GetIndexCount(), 1, 0, 0, 0);

		m_upDescriptorHeap->Reset();

	}


}

BOOL TextureDemoObject::Initialize()
{
	m_upTransform = make_unique<Transform>();
	m_upTransform->Initialize();

	InitRenderMethod();

	return TRUE;
}

void TextureDemoObject::Update()
{
	for (auto& node : m_pModelNodes)
	{
		node->pTransform->SetWorldPosition(m_upTransform->GetWorldPosition());
		node->pTransform->SetWorldRotation(m_upTransform->GetWorldRotation());
		node->pTransform->SetWorldScale(m_upTransform->GetWorldScale());
		node->Update();
	}
}

void TextureDemoObject::Render()
{
	m_upRenderMethod->Render();
}

BOOL TextureDemoObject::InitRenderMethod()
{
	m_upRenderMethod = make_unique<TextureDemoRender>();
	m_upRenderMethod->Initialize(shared_from_this());

	return TRUE;
}

void TextureDemoObject::LoadFromBinaryFile(std::wstring filePath)
{
	fstream fs;
	fs.open(filePath, ios::in | ios::binary);

	if (!fs.is_open()) __debugbreak();
	if (!fs.good()) __debugbreak();

	string read;
	UINT nodeIndex = 0;
	while (read != "</Model>")
	{
		// TODO : READ
		::getline(fs, read);

		if (read == "<Model Name>")
		{
			::getline(fs, m_strName);
		}

		if (read == "<Node Count>")
		{
			size_t nNodes = 0;
			fs.read(reinterpret_cast<char*>(&nNodes), sizeof(nNodes));
			m_pModelNodes.resize(nNodes);
		}

		if (read == "<Node>")
		{
			shared_ptr<ModelNode> pModelNode = make_shared<ModelNode>();
			fs >> *pModelNode;
			m_pModelNodes[nodeIndex++] = pModelNode;
		}
	}
}
