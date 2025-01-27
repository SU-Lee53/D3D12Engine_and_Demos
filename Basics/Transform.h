#pragma once

class Transform
{
public:
	Transform();
	~Transform();

	BOOL Initialize();
	BOOL Update();

public:
	// Getter, Setter
	void SetLocalPosition(const XMFLOAT3& pos);
	void SetLocalScale(const XMFLOAT3& scale);
	void SetLocalRotation(const XMFLOAT3& rot);
	
	void SetWorldPosition(const XMFLOAT3& pos);
	void SetWorldScale(const XMFLOAT3& scale);
	void SetWorldRotation(const XMFLOAT3& rot);

	XMFLOAT3 GetLocalPosition() { return m_vLocalPos; }
	XMFLOAT3 GetLocalRotation() { return m_vLocalRotation; }
	XMFLOAT3 GetLocalScale() { return m_vLocalScale; }
	
	XMFLOAT3 GetWorldPosition() { return m_vWorldPos; }
	XMFLOAT3 GetWorldRotation() { return m_vWorldRotation; }
	XMFLOAT3 GetWorldScale() { return m_vWorldScale; }

	XMFLOAT4X4 GetLocalMatrix() { return m_matLocal; }
	XMFLOAT4X4 GetLocalMatrixTransposed()
	{
		XMMATRIX xmLocalTransposed = XMMatrixTranspose(XMLoadFloat4x4(&m_matLocal));
		XMFLOAT4X4 ret;
		XMStoreFloat4x4(&ret, xmLocalTransposed);
		return ret;
	}
	
	XMFLOAT4X4 GetWorldMatrix() { return m_matWorld; }
	XMFLOAT4X4 GetWorldMatrixTransposed()
	{
		XMMATRIX xmWorldTransposed = XMMatrixTranspose(XMLoadFloat4x4(&m_matWorld));
		XMFLOAT4X4 ret;
		XMStoreFloat4x4(&ret, xmWorldTransposed);
		return ret;
	}

	CBTranformData GetTransformCBData() = delete;

public:
	// Basis Getter : With some matrix magics
	// Whole thing using LHS
	XMFLOAT3 GetLocalForward()
	{
		XMMATRIX xmLocalMatrix = XMLoadFloat4x4(&m_matLocal);
		XMVECTOR xmForward = xmLocalMatrix.r[2];
		XMFLOAT3 ret(0.f, 0.f, 0.f);
		XMStoreFloat3(&ret, xmForward);
		return ret;
	}

	XMFLOAT3 GetLocalRight()
	{
		XMMATRIX xmLocalMatrix = XMLoadFloat4x4(&m_matLocal);
		XMVECTOR xmRight = xmLocalMatrix.r[0];
		XMFLOAT3 ret(0.f, 0.f, 0.f);
		XMStoreFloat3(&ret, xmRight);
		return ret;
	}

	XMFLOAT3 GetLocalUp()
	{
		XMMATRIX xmLocalMatrix = XMLoadFloat4x4(&m_matLocal);
		XMVECTOR xmUp = xmLocalMatrix.r[1];
		XMFLOAT3 ret(0.f, 0.f, 0.f);
		XMStoreFloat3(&ret, xmUp);
		return ret;
	}
	
	XMFLOAT3 GetWorldForward()
	{
		XMMATRIX xmWorldMatrix = XMLoadFloat4x4(&m_matWorld);
		XMVECTOR xmForward = xmWorldMatrix.r[2];
		XMFLOAT3 ret(0.f, 0.f, 0.f);
		XMStoreFloat3(&ret, xmForward);
		return ret;
	}
	XMFLOAT3 GetWorldRight()
	{
		XMMATRIX xmWorldMatrix = XMLoadFloat4x4(&m_matWorld);
		XMVECTOR xmRight = xmWorldMatrix.r[0];
		XMFLOAT3 ret(0.f, 0.f, 0.f);
		XMStoreFloat3(&ret, xmRight);
		return ret;
	}
	XMFLOAT3 GetWorldUp()
	{
		XMMATRIX xmWorldMatrix = XMLoadFloat4x4(&m_matWorld);
		XMVECTOR xmUp = xmWorldMatrix.r[1];
		XMFLOAT3 ret(0.f, 0.f, 0.f);
		XMStoreFloat3(&ret, xmUp);
		return ret;
	}
	

private:
	XMFLOAT3 m_vLocalPos = { 0.f,0.f,0.f };
	XMFLOAT3 m_vLocalRotation = { 0.f,0.f,0.f };
	XMFLOAT3 m_vLocalScale = { 1.f,1.f,1.f };
	XMFLOAT4X4 m_matLocal;
	
	XMFLOAT3 m_vWorldPos = { 0.f,0.f,0.f };
	XMFLOAT3 m_vWorldRotation = { 0.f,0.f,0.f };
	XMFLOAT3 m_vWorldScale = { 1.f,1.f,1.f };
	XMFLOAT4X4 m_matWorld;

	BOOL m_bLocalUpdated = FALSE;
	BOOL m_bWorldUpdated = FALSE;

};

