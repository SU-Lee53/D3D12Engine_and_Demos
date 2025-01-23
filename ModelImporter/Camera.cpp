#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"

// Whole DirectXMath thing is messed up
// TODO : NEED TO CHECK IF THIS IS ALRIGHT!!!

Camera::Camera()
{
	m_vCamPosition = XMFLOAT3(0.f, 0.f, 0.f);
	m_vCamRotation = XMFLOAT3(0.f, 0.f, 0.f);

	m_vCamEYE = XMFLOAT3(0.f, 0.f, 0.f);
	m_vCamAT = XMFLOAT3(0.f, 0.f, 0.f);
	m_vCamUP = XMFLOAT3(0.f, 0.f, 0.f);

	m_matView = XMMatrixIdentity();
	m_matProjection = XMMatrixIdentity();
}

Camera::~Camera()
{

}

BOOL Camera::Initialize()
{
	// Get Camera Forward
	XMMATRIX world = CreateWorldMatrix(m_vCamPosition, m_vCamRotation);
	XMFLOAT3 camForward;
	XMStoreFloat3(&camForward, world.r[3]);

	m_vCamEYE = m_vCamPosition;
	m_vCamAT = XMFLOAT3(m_vCamEYE.x + camForward.x, m_vCamEYE.y + camForward.y, m_vCamEYE.z + camForward.z);
	XMStoreFloat3(&m_vCamUP, world.r[2]);

	return SetCamera(m_vCamEYE, m_vCamAT, m_vCamUP);
}

XMMATRIX CreateWorldMatrix(const XMFLOAT3& pos, const XMFLOAT3& rot)
{
	// S * R * T
	// Scale
	XMVECTOR vScale{ 1.f, 1.f, 1.f };
	XMMATRIX matScale = XMMatrixScalingFromVector(vScale);

	// Rotation
	XMMATRIX matRotX = XMMatrixRotationX(rot.x);
	XMMATRIX matRotY = XMMatrixRotationY(rot.y);
	XMMATRIX matRotZ = XMMatrixRotationZ(rot.z);
	XMMATRIX matRotation = XMMatrixMultiply(XMMatrixMultiply(matRotX, matRotY), matRotZ);

	// Translation
	XMVECTOR vPos{ pos.x, pos.y, pos.z };
	XMMATRIX matTranslation = XMMatrixTranslationFromVector(vPos);

	XMMATRIX ret = XMMatrixIdentity();
	ret = XMMatrixMultiply(matScale, matRotation);
	ret = XMMatrixMultiply(ret, matTranslation);

	return ret;
}

BOOL Camera::Update()
{
	if (m_bViewUpdated)
	{
		// Get Camera Forward
		XMMATRIX world = CreateWorldMatrix(m_vCamPosition, m_vCamRotation);
		XMFLOAT3 camForward;
		XMStoreFloat3(&camForward, world.r[3]);

		m_vCamEYE = m_vCamPosition;
		m_vCamAT = XMFLOAT3(m_vCamEYE.x + camForward.x, m_vCamEYE.y + camForward.y, m_vCamEYE.z + camForward.z);
		XMStoreFloat3(&m_vCamUP, world.r[2]);

		m_matView = XMMatrixLookAtLH(XMLoadFloat3(&m_vCamEYE), XMLoadFloat3(&m_vCamAT), XMLoadFloat3(&m_vCamUP));
		m_bViewUpdated = FALSE;
	}

	if (m_bProjUpdated)
	{
		m_matProjection = XMMatrixPerspectiveFovLH(m_ffovY, m_fAspectRatio, m_fNear, m_fFar);
		m_bProjUpdated = FALSE;
	}

}

void Camera::Resize(DWORD dwWidth, DWORD dwHeight)
{
	m_fAspectRatio = (float)dwWidth / (float)dwHeight;
	m_bProjUpdated = TRUE;
}

void Camera::SetPosition(const XMFLOAT3& pos)
{
	if (m_vCamPosition.x != pos.x || m_vCamPosition.y != pos.y || m_vCamPosition.z != pos.z)
	{
		m_vCamPosition = pos;
		m_bViewUpdated = TRUE;
	}
}

void Camera::SetRotation(const XMFLOAT3& rot)
{
	if (m_vCamRotation.x != rot.x || m_vCamRotation.y != rot.y || m_vCamRotation.z != rot.z)
	{
		m_vCamRotation = rot;
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
	m_matView = XMMatrixLookAtLH(XMLoadFloat3(&camEYE), XMLoadFloat3(&camAT), XMLoadFloat3(&camUP));

	m_ffovY = XM_PIDIV4;

	m_fAspectRatio = (float)CORE.GetScreenWidth() / (float)CORE.GetScreenHeight();
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	m_matProjection = XMMatrixPerspectiveFovLH(m_ffovY, m_fAspectRatio, m_fNear, m_fFar);

	return TRUE;
}