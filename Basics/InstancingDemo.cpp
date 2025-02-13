#include "pch.h"
#include "InstancingDemo.h"
#include "InstancingResources.h"
#include "Camera.h"

using namespace Instancing;
using namespace std;

void InstancingDemo::Initialize()
{
	m_pInstancingObj = make_shared<InstancingObject>();
	m_pInstancingObj->Initialize();

	// Cam
	m_pMainCamera = make_shared<Camera>();
	m_pMainCamera->Initialize();
	m_pMainCamera->SetPosition(XMFLOAT3(0.f, 0.f, -5.f));
	m_pMainCamera->SetPosition(XMFLOAT3(0.f, 0.f, -5.f));
}

void InstancingDemo::Update()
{
	m_pInstancingObj->Update();
	m_pMainCamera->Update();
}

void InstancingDemo::Render()
{
	RENDER.Add(m_pInstancingObj, m_pMainCamera);
}
