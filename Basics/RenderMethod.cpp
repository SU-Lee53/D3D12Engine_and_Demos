#include "pch.h"
#include "RenderMethod.h"
#include "Pipeline.h"
#include "RootSignature.h"
#include "Object.h"
#include "Camera.h"


using namespace std;

////////////////////////////////////
///		ForwardRender
////////////////////////////

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
	shared_ptr<Mesh<VertexType>> pMesh = owner->GetMesh();
	VertexBuffer& refVertexBuffer = pMesh->GetBuffer()->vertexBuffer;
	IndexBuffer& refIndexBuffer = pMesh->GetBuffer()->indexBuffer;

	// TODO : Think hard for better descriptor heap use
	// TODO2 : Render!!!!!!!! 

	// 1. Write data in Constant Buffer
	m_upTransformCBuffer->PushData(pTransform->GetTransformCBData());
	m_upCameraCBuffer->PushData(CORE.GetMainCameraData());

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
