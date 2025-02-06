#include "pch.h"
#include "BasicForwardRenderDemo.h"
#include "BasicForwardRenderResources.h"
#include "Object.h"
#include "Camera.h"

using namespace BasicForward;
using namespace std;

void BasicForwardRenderDemo::Initialize()
{
	m_pObj = make_shared<BasicForwardObject>();
	m_pObj->Initialize();

}

void BasicForwardRenderDemo::Update()
{
	shared_ptr<BasicForwardObject> forwardObj = static_pointer_cast<BasicForwardObject>(m_pObj);

	Vec3 pos = CORE.GetMainCamera()->GetTransform()->GetWorldPosition();
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

	if (INPUT.GetButton(KEY_TYPE::LEFT))
	{
		pos.x -= m_sensitivity * DT;
	}

	if (INPUT.GetButton(KEY_TYPE::RIGHT))
	{
		pos.x += m_sensitivity * DT;
	}

	if (INPUT.GetButton(KEY_TYPE::UP))
	{
		pos.z += m_sensitivity * DT;
	}

	if (INPUT.GetButton(KEY_TYPE::DOWN))
	{
		pos.z -= m_sensitivity * DT;
	}

	CORE.GetMainCamera()->SetPosition(pos);
	forwardObj->GetTransform()->SetWorldRotation(rot);

	forwardObj->Update();


}

void BasicForwardRenderDemo::Render()
{
	RENDER.Add(m_pObj);
}
