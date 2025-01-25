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

	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matProjection, XMMatrixIdentity());
}

Camera::~Camera()
{

}

BOOL Camera::Initialize()
{
	// Get Camera Forward
	m_matCamWorld = CreateWorldMatrix();
	XMMATRIX world = XMLoadFloat4x4(&m_matCamWorld);
	XMVECTOR camForward = world.r[2];

	// EYE
	m_vCamEYE = m_vCamPosition;

	// AT
	XMVECTOR CamEYE = XMLoadFloat3(&m_vCamEYE);
	XMVECTOR CamAT = XMVectorAdd(CamEYE, camForward);

	// UP
	XMVECTOR CamUP = world.r[1];

	XMStoreFloat3(&m_vCamEYE, CamEYE);
	XMStoreFloat3(&m_vCamAT, CamAT);
	XMStoreFloat3(&m_vCamUP, CamUP);

	return SetCamera(m_vCamEYE, m_vCamAT, m_vCamUP);
}

XMFLOAT4X4 Camera::CreateWorldMatrix()
{
	// Load XMVECTOR
	XMVECTOR CamEYE = XMLoadFloat3(&m_vCamEYE);
	XMVECTOR CamAT = XMLoadFloat3(&m_vCamAT);
	XMVECTOR CamUP = XMLoadFloat3(&m_vCamUP);

	// S * R * T
	// Scale
	XMVECTOR vScale{ 1.f, 1.f, 1.f };
	XMMATRIX matScale = XMMatrixScalingFromVector(vScale);

	// Rotation
	XMMATRIX matRotX = XMMatrixRotationX(m_vCamRotation.x);
	XMMATRIX matRotY = XMMatrixRotationY(m_vCamRotation.y);
	XMMATRIX matRotZ = XMMatrixRotationZ(m_vCamRotation.z);
	XMMATRIX matRotation = XMMatrixMultiply(XMMatrixMultiply(matRotX, matRotY), matRotZ);

	// Translation
	XMVECTOR vPos{ m_vCamPosition.x, m_vCamPosition.y, m_vCamPosition.z };
	XMMATRIX matTranslation = XMMatrixTranslationFromVector(vPos);

	XMMATRIX retMatrix = XMMatrixIdentity();
	retMatrix = XMMatrixMultiply(matScale, matRotation);
	retMatrix = XMMatrixMultiply(retMatrix, matTranslation);

	XMFLOAT4X4 ret;
	XMStoreFloat4x4(&ret, retMatrix);

	return ret;
}

BOOL Camera::Update()
{
	if (m_bViewUpdated)
	{
		// Get Camera Forward
		m_matCamWorld = CreateWorldMatrix();
		XMMATRIX world = XMLoadFloat4x4(&m_matCamWorld);
		XMVECTOR camForward = world.r[2];

		// EYE
		m_vCamEYE = m_vCamPosition;

		// AT
		XMVECTOR CamEYE = XMLoadFloat3(&m_vCamEYE);
		XMVECTOR CamAT = XMVectorAdd(CamEYE, camForward);

		// UP
		XMVECTOR CamUP = world.r[1];

		XMMATRIX xmMatView = XMMatrixLookAtLH(CamEYE, CamAT, CamUP);
		XMStoreFloat4x4(&m_matView, xmMatView);
		m_bViewUpdated = FALSE;
	}

	if (m_bProjUpdated)
	{
		XMMATRIX xmMatProj = XMMatrixPerspectiveFovLH(m_ffovY, m_fAspectRatio, m_fNear, m_fFar);
		XMStoreFloat4x4(&m_matProjection, xmMatProj);
		m_bProjUpdated = FALSE;
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
	XMVECTOR xmPosOrigin = XMLoadFloat3(&m_vCamPosition);
	XMVECTOR xmPos = XMLoadFloat3(&pos);

	if (!XMVector3Equal(xmPosOrigin, xmPos))
	{
		m_vCamPosition = pos;
		m_bViewUpdated = TRUE;
	}
}

void Camera::SetRotation(const XMFLOAT3& rot)
{
	XMVECTOR xmRotOrigin = XMLoadFloat3(&m_vCamRotation);
	XMVECTOR xmRot = XMLoadFloat3(&rot);
	
	if (!XMVector3Equal(xmRotOrigin, xmRot))
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
	// XMVECTOR Load
	XMVECTOR xmCamEYE = XMLoadFloat3(&camEYE);
	XMVECTOR xmCamAT = XMLoadFloat3(&camAT);
	XMVECTOR xmCamUP = XMLoadFloat3(&camUP);

	XMMATRIX xmMatView = XMMatrixLookAtLH(xmCamEYE, xmCamAT, xmCamUP);
	XMStoreFloat4x4(&m_matView, xmMatView);

	m_ffovY = XM_PIDIV4;

	m_fAspectRatio = (float)CORE.GetScreenWidth() / (float)CORE.GetScreenHeight();
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	XMMATRIX xmMatProj = XMMatrixPerspectiveFovLH(m_ffovY, m_fAspectRatio, m_fNear, m_fFar);
	XMStoreFloat4x4(&m_matProjection, xmMatProj);

	return TRUE;
}