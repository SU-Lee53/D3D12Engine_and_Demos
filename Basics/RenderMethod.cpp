#include "pch.h"
#include "RenderMethod.h"
#include "Pipeline.h"
#include "Object.h"

using namespace std;

RenderMethod::RenderMethod()
{
}

RenderMethod::~RenderMethod()
{
}

void RenderMethod::Initialize(shared_ptr<Object> owner, const vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC>& psoDescs)
{
}

void RenderMethod::Render()
{
}

ForwardRender::ForwardRender()
{
}

ForwardRender::~ForwardRender()
{
}

void ForwardRender::Initialize(std::shared_ptr<Object> owner, const std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC>& psoDescs)
{
	m_wpOwner = owner;

	m_dwPassCount = psoDescs.size();
	m_Pipelines.resize(m_dwPassCount);

	for (int i = 0; i < psoDescs.size(); i++)
	{
		m_Pipelines[i] = make_shared<Pipeline>();
		m_Pipelines[i]->Initialize(psoDescs[i]);
		m_dwRTVCount += psoDescs[i].NumRenderTargets;

	}

	// TODO : Create Render Targets -> No need to create on Forward Render
}

void ForwardRender::Render()
{
	ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();

	auto owner = m_wpOwner.lock();

	ComPtr<ID3D12RootSignature> pRootSignature = owner->GetRootSignature();
	shared_ptr<Transform> pTransform = owner->GetTransform();
	shared_ptr<Mesh<VertexType>> pMesh = owner->GetMesh();

	// TODO : Make Constant Buffer class and push transform data
	// TODO2 : Render!!!!!!!! 



}
