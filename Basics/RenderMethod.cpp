#include "pch.h"
#include "RenderMethod.h"
#include "Pipeline.h"

using namespace std;

RenderMethod::RenderMethod()
{
}

RenderMethod::~RenderMethod()
{
}

void RenderMethod::Initialize(shared_ptr<Object> owner, const vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC>& psoDescs)
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

	// TODO : Create Render Targets
}

void RenderMethod::Render()
{
}
