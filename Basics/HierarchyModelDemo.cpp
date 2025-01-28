#include "pch.h"
#include "HierarchyModelDemo.h"
#include "HierarchyModelResource.h"
#include "Object.h"
#include "Camera.h"

using namespace std;

void HierarchyModelDemo::Initialize()
{
	m_pObj = make_shared<HierarchyModelObject>();
	m_pObj->Initialize();

	shared_ptr<HierarchyModelObject> originObj = static_pointer_cast<HierarchyModelObject>(m_pObj);
	originObj->LoadFromBinaryFile(L"../Models/Binaries/Hummer.bin");
	originObj->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
}

void HierarchyModelDemo::Update()
{
	Camera& cam = *CORE.GetMainCamera();

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

void HierarchyModelDemo::Render()
{
	RENDER.Add(m_pObj);
}
