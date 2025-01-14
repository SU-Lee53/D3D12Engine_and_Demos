#include "pch.h"
#include "TestApp.h"
#include "Object.h"
#include "Camera.h"
using namespace std;

void TestApp::Initialize()
{
	m_pObj = make_shared<BasicForwardObject>();
	m_pObj->Initialize();

}

void TestApp::Update()
{
	Vec3 pos = CORE.GetMainCamera()->GetTransform()->GetPosition();
	Vec3 rot = m_pObj->GetTransform()->GetRotation();

	if (INPUT.GetButton(KEY_TYPE::W))
	{
		rot.x += m_sensitivity * DT;
	}
	
	if (INPUT.GetButton(KEY_TYPE::S))
	{
		rot.x -= m_sensitivity * DT;
	}
	
	if (INPUT.GetButton(KEY_TYPE::A))
	{
		rot.y += m_sensitivity * DT;
	}
	
	if (INPUT.GetButton(KEY_TYPE::D))
	{
		rot.y -= m_sensitivity * DT;
	}
	
	if (INPUT.GetButton(KEY_TYPE::E))
	{
		rot.z += m_sensitivity * DT;
	}
	
	if (INPUT.GetButton(KEY_TYPE::Q))
	{
		rot.z -= m_sensitivity * DT;
	}

	if (INPUT.GetButton(KEY_TYPE::LEFT))
	{
		pos.x -= 0.01f * DT;
	}
	
	if (INPUT.GetButton(KEY_TYPE::RIGHT))
	{
		pos.x += 0.01f * DT;
	}
	
	if (INPUT.GetButton(KEY_TYPE::UP))
	{
		pos.z += 0.01f * DT;
	}
	
	if (INPUT.GetButton(KEY_TYPE::DOWN))
	{
		pos.z -= 0.01f * DT;
	}

	CORE.GetMainCamera()->SetPosition(pos);
	m_pObj->GetTransform()->SetRotation(rot);

	m_pObj->Update();


}

void TestApp::Render()
{
	RENDER.Add(m_pObj);
}
