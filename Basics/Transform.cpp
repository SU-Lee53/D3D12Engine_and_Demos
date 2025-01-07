#include "pch.h"
#include "Transform.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

BOOL Transform::Initialize()
{
	return 0;
}

void Transform::Update()
{
	if (m_bUpdated)
	{
		// S * R * T
		// Scale
		Matrix matScale = XMMatrixScalingFromVector(m_vScale);

		// Rotation
		Matrix matRotX = XMMatrixRotationX(m_vRotation.x);
		Matrix matRotY = XMMatrixRotationY(m_vRotation.y);
		Matrix matRotZ = XMMatrixRotationZ(m_vRotation.z);
		Matrix matRotation = XMMatrixMultiply(XMMatrixMultiply(matRotX, matRotY), matRotZ);

		// Translation
		Matrix matTranslation = XMMatrixTranslationFromVector(m_vPos);

		m_matWorld = XMMatrixMultiply(matScale, matRotation);
		m_matWorld = XMMatrixMultiply(m_matWorld, matTranslation);
		
		m_bUpdated = FALSE;
	}
}

void Transform::SetPosition(Vec3 pos)
{
	m_vPos = XMLoadFloat3(&pos);
	m_bUpdated = TRUE;
}

void Transform::SetScale(Vec3 scale)
{
	m_vScale = XMLoadFloat3(&scale);
	m_bUpdated = TRUE;
}

void Transform::SetRotation(Vec3 rot)
{
	m_vRotation = XMLoadFloat3(&rot);
	m_bUpdated = TRUE;
}
