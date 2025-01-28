#include "pch.h"
#include "LambertDemo.h"
#include "LambertResources.h"
#include "Object.h"
#include "Camera.h"

using namespace std;

void LambertDemo::Initialize()
{
	m_pObj = make_shared<LambertObject>();
	m_pObj->Initialize();
	
	m_pLambertLight = make_shared<LambertLight>();
	m_pLambertLight->Initialize();

	m_pLambertLight->SetDirection(XMFLOAT3(1.f, 0.f, 0.f));
	m_pLambertLight->SetColor(XMFLOAT3(1.f, 1.f, 1.f));

	shared_ptr<LambertObject> originObj = static_pointer_cast<LambertObject>(m_pObj);
	originObj->LoadFromBinaryFile(L"../Models/Binaries/Hummer.bin");
	originObj->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));

	XMFLOAT4 colorKhaki;
	XMStoreFloat4(&colorKhaki, Colors::Khaki);
	originObj->SetColor(colorKhaki);

}

void LambertDemo::Update()
{
	Camera& cam = *CORE.GetMainCamera();

	if(ImGui::Begin("TEST"))
	{
		ImGui::Text("YES");
	}
	ImGui::End();

	// Cam
	{
		XMVECTOR xmPos = XMLoadFloat3(&cam.GetPosition());
		XMFLOAT3 CamFront, CamRight, CamUp;
		cam.DecomposeCameraUVN(CamFront, CamRight, CamUp);

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
		GetClientRect(GAME.GetGameDesc().hWnd, (LPRECT)&rMyRect);
		ClientToScreen(GAME.GetGameDesc().hWnd, (LPPOINT)&rMyRect.left);
		ClientToScreen(GAME.GetGameDesc().hWnd, (LPPOINT)&rMyRect.right);
		int screenCenterX = 0, screenCenterY = 0;
		screenCenterX = rMyRect.left + GAME.GetGameDesc().width / 2;
		screenCenterY = rMyRect.top + GAME.GetGameDesc().height / 2;

		GetCursorPos(&m_MousePoint);
		XMVECTOR xmRot = XMLoadFloat3(&cam.GetRotation());

		if (INPUT.GetButton(KEY_TYPE::RBUTTON))
		{
			{
				m_fDeltaX = (m_MousePoint.x - screenCenterX);	// Yaw
				m_fDeltaY = (m_MousePoint.y - screenCenterY);	// Pitch

				xmRot.m128_f32[0] += m_fDeltaY * 0.001f;
				xmRot.m128_f32[1] += m_fDeltaX * 0.001f;

				if (xmRot.m128_f32[0] >= 90.f) xmRot.m128_f32[0] = 90.f;
				if (xmRot.m128_f32[1] <= -90.f) xmRot.m128_f32[1] = -90.f;

			}
			SetCursorPos(screenCenterX, screenCenterY);

		}

		XMFLOAT3 pos, rot;
		XMStoreFloat3(&pos, xmPos);
		XMStoreFloat3(&rot, xmRot);

		cam.SetPosition(pos);
		cam.SetRotation(rot);
		cam.Update();
	}

	m_pObj->Update();

}

void LambertDemo::Render()
{
	RENDER.Add(m_pObj);
}


//////////////////
// LambertLight //
//////////////////

CBLambertData LambertLight::GetLambertCBData()
{
	return CBLambertData{ m_vlightDir, m_vlightColor };
}
