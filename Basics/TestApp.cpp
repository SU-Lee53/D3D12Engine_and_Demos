#include "pch.h"
#include "TestApp.h"
#include "Object.h"

using namespace std;

void TestApp::Initialize()
{
	m_pObj = make_shared<BasicForwardObject>();
	m_pObj->Initialize();
}

void TestApp::Update()
{
	m_pObj->GetTransform()->SetRotation(Vec3(0.f, m_fRotY, 0.f));
	m_fRotY += 1.f * DT;

	m_pObj->Update();
}

void TestApp::Render()
{
	RENDER.Add(m_pObj);
}
