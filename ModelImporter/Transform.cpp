#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
{
	m_matLocal = XMMatrixIdentity();
	m_matWorld = XMMatrixIdentity();

}

Transform::~Transform()
{

}

BOOL Transform::Initialize()
{
	return TRUE;
}

BOOL Transform::Update()
{
	if (m_bLocalUpdated)
	{
		XMMATRIX matScale = XMMatrixScalingFromVector(XMLoadFloat3(&m_vLocalScale));

		XMMATRIX matRotX = XMMatrixRotationX(m_vLocalRotation.x);
		XMMATRIX matRotY = XMMatrixRotationY(m_vLocalRotation.y);
		XMMATRIX matRotZ = XMMatrixRotationZ(m_vLocalRotation.z);
		XMMATRIX matRotation = XMMatrixMultiply(XMMatrixMultiply(matRotX, matRotY), matRotZ);

		XMMATRIX matTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&m_vLocalPosition));

		m_matLocal = XMMatrixMultiply(matScale, matRotation);
		m_matLocal = XMMatrixMultiply(m_matWorld, matTranslation);

		m_bLocalUpdated = FALSE;
	}

	if (m_bWorldUpdated)
	{
		XMMATRIX matScale = XMMatrixScalingFromVector(XMLoadFloat3(&m_vWorldScale));

		XMMATRIX matRotX = XMMatrixRotationX(m_vWorldRotation.x);
		XMMATRIX matRotY = XMMatrixRotationY(m_vWorldRotation.y);
		XMMATRIX matRotZ = XMMatrixRotationZ(m_vWorldRotation.z);
		XMMATRIX matRotation = XMMatrixMultiply(XMMatrixMultiply(matRotX, matRotY), matRotZ);

		XMMATRIX matTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&m_vWorldPosition));

		m_matWorld = XMMatrixMultiply(matScale, matRotation);
		m_matWorld = XMMatrixMultiply(m_matWorld, matTranslation);

		m_bWorldUpdated = FALSE;
	}

	return TRUE;
}

void Transform::SetLocalPosition(const XMFLOAT3& pos)
{
	m_vLocalPosition = pos;
	m_bLocalUpdated = TRUE;
}

void Transform::SetLocalScale(const XMFLOAT3& scale)
{
	m_vLocalRotation = scale;
	m_bLocalUpdated = TRUE;
}

void Transform::SetLocalRotation(const XMFLOAT3& rot)
{
	m_vLocalScale = rot;
	m_bLocalUpdated = TRUE;
}

void Transform::SetWorldPosition(const XMFLOAT3& pos)
{
	m_vWorldPosition = pos;
	m_bWorldUpdated = TRUE;
}

void Transform::SetWorldScale(const XMFLOAT3& scale)
{
	m_vWorldRotation = scale;
	m_bWorldUpdated = TRUE;
}

void Transform::SetWorldRotation(const XMFLOAT3& rot)
{
	m_vWorldScale = rot;
	m_bWorldUpdated = TRUE;
}
