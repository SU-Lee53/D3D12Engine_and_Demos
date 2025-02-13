#include "pch.h"
#include "BasicForwardRenderDemo.h"
#include "BasicForwardRenderResources.h"
#include "Object.h"
#include "Camera.h"

using namespace BasicForward;
using namespace std;

void BasicForwardRenderDemo::Initialize()
{
	// Cam
	m_pMainCamera = make_shared<Camera>();
	m_pMainCamera->Initialize();

	m_pObj = make_shared<BasicForwardObject>();
	m_pObj->Initialize();

}

void BasicForwardRenderDemo::Update()
{
	shared_ptr<BasicForwardObject> forwardObj = static_pointer_cast<BasicForwardObject>(m_pObj);

	Vec3 pos = m_pMainCamera->GetTransform()->GetWorldPosition();
	Vec3 rot = forwardObj->GetTransform()->GetWorldRotation();

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

	forwardObj->GetTransform()->SetWorldRotation(rot);

	forwardObj->Update();

	m_pMainCamera->Update();
}

void BasicForwardRenderDemo::Render()
{
	RENDER.Add(m_pObj, m_pMainCamera);
}
