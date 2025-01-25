#include "stdafx.h"
#include "Application.h"
#include "Importer.h"
#include "Model.h"
#include "Camera.h"

using namespace std;

Application::Application()
{
}

Application::~Application()
{
}

void Application::Initialize()
{
	m_pCamera = make_shared<Camera>();
	m_pCamera->Initialize();

	m_pImporter = make_unique<FbxLoader>();
	m_pImporter->Initialize();

	m_pImporter->LoadFBXFile("../fbx/Gunship.fbx");

	m_pModel = make_shared<Model>();
	m_pImporter->ExportModelInSceneToModel(m_pModel);
	//m_pModel->ScaleModel(XMFLOAT3(0.01f, 0.01f, 0.01f));
}

void Application::Update()
{
	if (ImGui::Begin("Node"))
	{
		m_pImporter->ShowFBXNodeToImGui();
	}
	ImGui::End();

	// Model
	m_pModel->Update();

	// Cam
	{
		XMVECTOR xmPos = XMLoadFloat3(&m_pCamera->GetPosition());
		XMFLOAT3 CamFront, CamRight, CamUp;
		m_pCamera->DecomposeCameraUVN(CamFront, CamRight, CamUp);

		XMVECTOR xmFront = XMLoadFloat3(&CamFront);
		XMVECTOR xmRight = XMLoadFloat3(&CamRight);
		XMVECTOR xmUp = XMLoadFloat3(&CamUp);

		if (INPUT.GetButton(KEY_TYPE::W))
		{
			xmFront = XMVectorScale(xmFront, 0.1f);
			xmPos = XMVectorAdd(xmPos, xmFront);
		}
		if (INPUT.GetButton(KEY_TYPE::S))
		{
			xmFront = XMVectorScale(xmFront, -0.1f);
			xmPos = XMVectorAdd(xmPos, xmFront);
		}

		if (INPUT.GetButton(KEY_TYPE::D))
		{
			xmRight = XMVectorScale(xmRight, 0.1f);
			xmPos = XMVectorAdd(xmPos, xmRight);
		}
		if (INPUT.GetButton(KEY_TYPE::A))
		{
			xmRight = XMVectorScale(xmRight, -0.1f);
			xmPos = XMVectorAdd(xmPos, xmRight);
		}
		
		if (INPUT.GetButton(KEY_TYPE::SPACE))
		{
			xmUp = XMVectorScale(xmUp, 0.1f);
			xmPos = XMVectorAdd(xmPos, xmUp);
		}
		if (INPUT.GetButton(KEY_TYPE::CTRL))
		{
			xmUp = XMVectorScale(xmUp, -0.1f);
			xmPos = XMVectorAdd(xmPos, xmUp);
		}

		RECT rMyRect;
		GetClientRect(CORE.GetHwnd(), (LPRECT)&rMyRect);
		ClientToScreen(CORE.GetHwnd(), (LPPOINT)&rMyRect.left);
		ClientToScreen(CORE.GetHwnd(), (LPPOINT)&rMyRect.right);
		int screenCenterX = 0, screenCenterY = 0;
		screenCenterX = rMyRect.left + CORE.GetScreenWidth() / 2;
		screenCenterY = rMyRect.top + CORE.GetScreenHeight() / 2;

		GetCursorPos(&m_MousePoint);
		XMVECTOR xmRot = XMLoadFloat3(&m_pCamera->GetRotation());

		if (INPUT.GetButton(KEY_TYPE::RBUTTON))
		{
			{
				m_fMouseDeltaX = (m_MousePoint.x - screenCenterX);	// Yaw
				m_fMouseDeltaY = (m_MousePoint.y - screenCenterY);	// Pitch

				xmRot.m128_f32[0] += m_fMouseDeltaY * 0.001f;
				xmRot.m128_f32[1] += m_fMouseDeltaX * 0.001f;

				if (xmRot.m128_f32[0] >= 90.f) xmRot.m128_f32[0] = 90.f;
				if (xmRot.m128_f32[1] <= -90.f) xmRot.m128_f32[1] = -90.f;

			}
			SetCursorPos(screenCenterX, screenCenterY);

		}

		XMFLOAT3 pos, rot;
		XMStoreFloat3(&pos, xmPos);
		XMStoreFloat3(&rot, xmRot);

		m_pCamera->SetPosition(pos);
		m_pCamera->SetRotation(rot);
		m_pCamera->Update();
	}
}

void Application::Render()
{
	m_pModel->Render(m_pCamera);
}
