#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

BOOL Camera::Initialize()
{
	m_vCamPos = Vec3(0.f, 0.f, 0.f);
	m_vCamDir = Vec3(0.f, 0.f, 1.f);
	m_vCamUp = Vec3(0.f, 1.f, 0.f);

	return SetCamera(m_vCamPos, m_vCamDir, m_vCamUp);
}

BOOL Camera::SetCamera(const Vec3& camPos, const Vec3& camDir, const Vec3& camUp)
{
	m_matView = Matrix::CreateLookAt(camPos, camDir, camUp);

	m_ffovY = XM_PIDIV4;

	m_fAspectRatio = (float)GAME.GetGameDesc().width / (float)GAME.GetGameDesc().height;
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	m_matProjection = Matrix::CreatePerspectiveFieldOfView(m_ffovY, m_fAspectRatio, m_fNear, m_fFar);

	return TRUE;
}
