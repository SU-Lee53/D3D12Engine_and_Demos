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

	m_vCamEYE	= m_pTransform->GetPosition();
	m_vCamAT	= m_vCamEYE + m_pTransform->GetWorldMatrix().Backward(); // Simplemath Matrix is using RHS
	m_vCamUP	= m_pTransform->GetWorldMatrix().Up();

	return SetCamera(m_vCamEYE, m_vCamAT, m_vCamUP);
}

BOOL Camera::Update()
{
	m_pTransform->Update();

	if (m_bViewUpdated)
	{
		m_vCamEYE = m_pTransform->GetPosition();
		m_vCamAT = m_vCamEYE + m_pTransform->GetWorldMatrix().Backward(); // Simplemath Matrix is using RHS
		m_vCamUP = m_pTransform->GetWorldMatrix().Up();

		m_matView = XMMatrixLookAtLH(m_vCamEYE, m_vCamAT, m_vCamUP);
		m_bViewUpdated = FALSE;
	}

	if (m_bProjUpdated)
	{
		m_matProjection = XMMatrixPerspectiveFovLH(m_ffovY, m_fAspectRatio, m_fNear, m_fFar);
		m_bProjUpdated = FALSE;
	}


	return TRUE;
}

void Camera::Resize(DWORD dwWidth, DWORD dwHeight)
{
	m_fAspectRatio = (float)dwWidth / (float)dwHeight;
	m_bProjUpdated = TRUE;
}

void Camera::SetPosition(const Vec3& pos)
{
	if(m_pTransform->GetPosition() != pos)
	{
		m_pTransform->SetPosition(pos);
		m_bViewUpdated = TRUE;
	}
}

void Camera::SetRotation(const Vec3& rot)
{
	if(m_pTransform->GetRotation() != rot)
	{
		m_pTransform->SetRotation(rot);
		m_bViewUpdated = TRUE;
	}
}

void Camera::SetFovY(float fFovY)
{
	m_ffovY = fFovY;
	m_bProjUpdated = TRUE;
}

void Camera::SetAspect(float fWidth, float fHeight)
{
	m_fAspectRatio = fWidth / fHeight;
	m_bProjUpdated = TRUE;
}

void Camera::SetAspectRatio(float fRatio)
{
	m_fAspectRatio = fRatio;
	m_bProjUpdated = TRUE;
}

void Camera::SetNear(float fNear)
{
	m_fNear = fNear;
	m_bProjUpdated = TRUE;
}

void Camera::SetFar(float fFar)
{
	m_fFar = fFar;
	m_bProjUpdated = TRUE;
}

BOOL Camera::SetCamera(const Vec3& camEYE, const Vec3& camAT, const Vec3& camUP)
{
	m_matView = XMMatrixLookAtLH(camEYE, camAT, camUP);

	m_ffovY = XM_PIDIV4;

	m_fAspectRatio = (float)GAME.GetGameDesc().width / (float)GAME.GetGameDesc().height;
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	m_matProjection = XMMatrixPerspectiveFovLH(m_ffovY, m_fAspectRatio, m_fNear, m_fFar);

	return TRUE;
}
