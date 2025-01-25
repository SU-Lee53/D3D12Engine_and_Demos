#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
{
	XMStoreFloat4x4(&m_matLocal, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());
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

		XMMATRIX xmLocal = XMMatrixIdentity();
		xmLocal = XMMatrixMultiply(matScale, matRotation);
		xmLocal = XMMatrixMultiply(xmLocal, matTranslation);
		XMStoreFloat4x4(&m_matLocal, xmLocal);

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

		XMMATRIX xmWorld = XMMatrixIdentity();
		xmWorld = XMMatrixMultiply(matScale, matRotation);
		xmWorld = XMMatrixMultiply(xmWorld, matTranslation);
		XMStoreFloat4x4(&m_matWorld, xmWorld);

		m_bWorldUpdated = FALSE;
	}

	return TRUE;
}

void Transform::SetLocalPosition(const XMFLOAT3& pos)
{
	XMVECTOR xmPosOrigin = XMLoadFloat3(&m_vLocalPosition);
	XMVECTOR xmPosNew = XMLoadFloat3(&pos);

	if(!XMVector3Equal(xmPosOrigin, xmPosNew))
	{
		m_vLocalPosition = pos;
		m_bLocalUpdated = TRUE;
	}
}

void Transform::SetLocalScale(const XMFLOAT3& scale)
{
	XMVECTOR xmScaleOrigin = XMLoadFloat3(&m_vLocalScale);
	XMVECTOR xmScaleNew = XMLoadFloat3(&scale);

	if (!XMVector3Equal(xmScaleOrigin, xmScaleNew))
	{
		m_vLocalScale = scale;
		m_bLocalUpdated = TRUE;
	}
}

void Transform::SetLocalRotation(const XMFLOAT3& rot)
{
	XMVECTOR xmRotationOrigin = XMLoadFloat3(&m_vLocalRotation);
	XMVECTOR xmRotationNew = XMLoadFloat3(&rot);

	if (!XMVector3Equal(xmRotationOrigin, xmRotationNew))
	{
		m_vLocalRotation = rot;
		m_bLocalUpdated = TRUE;
	}
}

void Transform::SetWorldPosition(const XMFLOAT3& pos)
{
	XMVECTOR xmPosOrigin = XMLoadFloat3(&m_vWorldPosition);
	XMVECTOR xmPosNew = XMLoadFloat3(&pos);

	if (!XMVector3Equal(xmPosOrigin, xmPosNew))
	{
		m_vWorldPosition = pos;
		m_bWorldUpdated = TRUE;
	}
}

void Transform::SetWorldScale(const XMFLOAT3& scale)
{
	XMVECTOR xmScaleOrigin = XMLoadFloat3(&m_vWorldScale);
	XMVECTOR xmScaleNew = XMLoadFloat3(&scale);

	if (!XMVector3Equal(xmScaleOrigin, xmScaleNew))
	{
		m_vWorldScale = scale;
		m_bWorldUpdated = TRUE;
	}
}

void Transform::SetWorldRotation(const XMFLOAT3& rot)
{
	XMVECTOR xmRotationOrigin = XMLoadFloat3(&m_vWorldRotation);
	XMVECTOR xmRotationNew = XMLoadFloat3(&rot);

	if (!XMVector3Equal(xmRotationOrigin, xmRotationNew))
	{
		m_vWorldRotation = rot;
		m_bWorldUpdated = TRUE;
	}
}
