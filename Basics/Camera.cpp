#include "pch.h"
#include "Camera.h"

using namespace std;

Camera::Camera()
{
}

Camera::~Camera()
{
}

BOOL Camera::Initialize()
{
	m_pTransform = make_shared<Transform>();

	// EYE
	m_vCamEYE = m_pTransform->GetWorldPosition();

	// AT
	XMVECTOR xmCamEYE = XMLoadFloat3(&m_vCamEYE);
	XMVECTOR xmCamForward = XMLoadFloat3(&m_pTransform->GetWorldForward());
	XMVECTOR xmCamAT = XMVectorAdd(xmCamEYE, xmCamForward);
	XMStoreFloat3(&m_vCamAT, xmCamAT);

	// UP
	m_vCamUP = m_pTransform->GetWorldUp();

	return SetCamera(m_vCamEYE, m_vCamAT, m_vCamUP);
}

BOOL Camera::Update()
{
	m_pTransform->Update();

	if (m_bViewUpdated)
	{
		// EYE
		m_vCamEYE = m_pTransform->GetWorldPosition();

		// AT
		XMVECTOR xmCamEYE = XMLoadFloat3(&m_vCamEYE);
		XMVECTOR xmCamForward = XMLoadFloat3(&m_pTransform->GetWorldForward());
		XMVECTOR xmCamAT = XMVectorAdd(xmCamEYE, xmCamForward);
		XMStoreFloat3(&m_vCamAT, xmCamAT);

		// UP
		m_vCamUP = m_pTransform->GetWorldUp();

		// Load XMVECTOR for params
		xmCamEYE = XMLoadFloat3(&m_vCamEYE);
		xmCamAT = XMLoadFloat3(&m_vCamAT);
		XMVECTOR xmCamUP = XMLoadFloat3(&m_vCamUP);

		XMMATRIX xmMatView = XMMatrixLookAtLH(xmCamEYE, xmCamAT, xmCamUP);
		XMStoreFloat4x4(&m_matView, xmMatView);
		m_bViewUpdated = FALSE;
	}

	if (m_bProjUpdated)
	{
		XMMATRIX xmMatProj = XMMatrixPerspectiveFovLH(m_ffovY, m_fAspectRatio, m_fNear, m_fFar);
		XMStoreFloat4x4(&m_matProjection, xmMatProj);
		m_bProjUpdated = FALSE;
	}


	// Moving script
	{
		XMVECTOR xmPos = XMLoadFloat3(&m_pTransform->GetWorldPosition());
		XMFLOAT3 CamFront, CamRight, CamUp;
		DecomposeCameraUVN(CamFront, CamRight, CamUp);

		XMVECTOR xmFront = XMLoadFloat3(&CamFront);
		XMVECTOR xmRight = XMLoadFloat3(&CamRight);
		XMVECTOR xmUp = XMLoadFloat3(&CamUp);

		float fCamSpeed = 5.0f;

		if (INPUT.GetButton(KEY_TYPE::LSHIFT))
		{
			fCamSpeed *= 2;
		}

		if (INPUT.GetButton(KEY_TYPE::W))
		{
			xmFront = XMVectorScale(xmFront, fCamSpeed * DT);
			xmPos = XMVectorAdd(xmPos, xmFront);
		}
		if (INPUT.GetButton(KEY_TYPE::S))
		{
			xmFront = XMVectorScale(xmFront, -fCamSpeed * DT);
			xmPos = XMVectorAdd(xmPos, xmFront);
		}

		if (INPUT.GetButton(KEY_TYPE::D))
		{
			xmRight = XMVectorScale(xmRight, fCamSpeed * DT);
			xmPos = XMVectorAdd(xmPos, xmRight);
		}
		if (INPUT.GetButton(KEY_TYPE::A))
		{
			xmRight = XMVectorScale(xmRight, -fCamSpeed * DT);
			xmPos = XMVectorAdd(xmPos, xmRight);
		}

		if (INPUT.GetButton(KEY_TYPE::SPACE))
		{
			xmUp = XMVectorScale(xmUp, fCamSpeed * DT);
			xmPos = XMVectorAdd(xmPos, xmUp);
		}
		if (INPUT.GetButton(KEY_TYPE::CTRL))
		{
			xmUp = XMVectorScale(xmUp, -fCamSpeed * DT);
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
		XMVECTOR xmRot = XMLoadFloat3(&m_pTransform->GetWorldRotation());

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

		SetPosition(pos);
		SetRotation(rot);
	}
	return TRUE;
}

void Camera::Resize(DWORD dwWidth, DWORD dwHeight)
{
	m_fAspectRatio = (float)dwWidth / (float)dwHeight;
	m_bProjUpdated = TRUE;
}

void Camera::SetPosition(const XMFLOAT3& pos)
{
	XMVECTOR xmPosOrigin = XMLoadFloat3(&m_pTransform->GetWorldPosition());
	XMVECTOR xmPos = XMLoadFloat3(&pos);

	if(!XMVector3Equal(xmPosOrigin, xmPos))
	{
		m_pTransform->SetWorldPosition(pos);
		m_bViewUpdated = TRUE;
	}
}

void Camera::SetRotation(const XMFLOAT3& rot)
{
	XMVECTOR xmRotOrigin = XMLoadFloat3(&m_pTransform->GetWorldRotation());
	XMVECTOR xmRot = XMLoadFloat3(&rot);

	if (!XMVector3Equal(xmRotOrigin, xmRot))
	{
		m_pTransform->SetWorldRotation(rot);
		m_bViewUpdated = TRUE;
	}
}

void Camera::SetFovY(float fFovY)
{
	if (m_ffovY != fFovY)
	{
		m_ffovY = fFovY;
		m_bProjUpdated = TRUE;
	}
}

void Camera::SetAspect(float fWidth, float fHeight)
{
	m_fAspectRatio = fWidth / fHeight;
	m_bProjUpdated = TRUE;
}

void Camera::SetAspectRatio(float fRatio)
{
	if (m_fAspectRatio != fRatio)
	{
		m_fAspectRatio = fRatio;
		m_bProjUpdated = TRUE;
	}
}

void Camera::SetNear(float fNear)
{
	if (m_fNear != fNear)
	{
		m_fNear = fNear;
		m_bProjUpdated = TRUE;
	}
}

void Camera::SetFar(float fFar)
{
	if (m_fFar != fFar)
	{
		m_fFar = fFar;
		m_bProjUpdated = TRUE;
	}
}

BOOL Camera::SetCamera(const XMFLOAT3& camEYE, const XMFLOAT3& camAT, const XMFLOAT3& camUP)
{
	XMVECTOR xmCamEYE = XMLoadFloat3(&camEYE);
	XMVECTOR xmCamAT = XMLoadFloat3(&camAT);
	XMVECTOR xmCamUP = XMLoadFloat3(&camUP);

	XMMATRIX xmMatView = XMMatrixLookAtLH(xmCamEYE, xmCamAT, xmCamUP);
	XMStoreFloat4x4(&m_matView, xmMatView);

	m_ffovY = XM_PIDIV4;

	m_fAspectRatio = (float)GAME.GetGameDesc().width / (float)GAME.GetGameDesc().height;
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	XMMATRIX xmMatProj = XMMatrixPerspectiveFovLH(m_ffovY, m_fAspectRatio, m_fNear, m_fFar);
	XMStoreFloat4x4(&m_matProjection, xmMatProj);

	return TRUE;
}
