#include "pch.h"
#include "InstancingDemo.h"
#include "InstancingResources.h"
#include "Camera.h"

using namespace std;

void InstancingDemo::Initialize()
{
	m_pInstancingObj = make_shared<InstancingObject>();
	m_pInstancingObj->Initialize();

	CORE.GetMainCamera()->SetPosition(XMFLOAT3(0.f, 0.f, -5.f));
}

void InstancingDemo::Update()
{
	m_pInstancingObj->Update();
}

void InstancingDemo::Render()
{
	RENDER.Add(m_pInstancingObj);
}
