#pragma once

class Transform
{
public:
	Transform();
	~Transform();

public:
	BOOL Initialize();
	BOOL Update();


public:
	// Getter
	XMFLOAT3 GetLocalPosition() { return m_vLocalPosition; }
	XMFLOAT3 GetLocalScale() { return m_vLocalScale; }
	XMFLOAT3 GetLocalRotation() { return m_vLocalRotation; }
	XMFLOAT4X4 GetLocalMatrix() { return m_matLocal; }
	XMFLOAT4X4 GetLocalMatrixTransposed() 
	{ 
		XMMATRIX xmLocalTransposed = XMMatrixTranspose(XMLoadFloat4x4(&m_matLocal)); 
		XMFLOAT4X4 ret;
		XMStoreFloat4x4(&ret, xmLocalTransposed);
		return ret;
	}

	XMFLOAT3 GetWorldPosition() { return m_vWorldPosition; }
	XMFLOAT3 GetWorldScale() { return m_vWorldScale; }
	XMFLOAT3 GetWorldRotation() { return m_vWorldRotation; }
	XMFLOAT4X4 GetWorldMatrix() { return m_matWorld; }
	XMFLOAT4X4 GetWorldMatrixTransposed()
	{
		XMMATRIX xmWorldTransposed = XMMatrixTranspose(XMLoadFloat4x4(&m_matWorld));
		XMFLOAT4X4 ret;
		XMStoreFloat4x4(&ret, xmWorldTransposed);
		return ret;
	}

public:
	// Setter
	void SetLocalPosition(const XMFLOAT3& pos);
	void SetLocalScale(const XMFLOAT3& scale);
	void SetLocalRotation(const XMFLOAT3& rot);
	 
	void SetWorldPosition(const XMFLOAT3& pos);
	void SetWorldScale(const XMFLOAT3& scale);
	void SetWorldRotation(const XMFLOAT3& rot);

private:
	// Local Transform
	XMFLOAT3	m_vLocalPosition = { 0.f, 0.f, 0.f };
	XMFLOAT3	m_vLocalRotation = { 0.f, 0.f, 0.f };
	XMFLOAT3	m_vLocalScale = { 1.f, 1.f, 1.f };
	XMFLOAT4X4	m_matLocal;

	// World Transform
	XMFLOAT3	m_vWorldPosition = { 0.f, 0.f, 0.f };
	XMFLOAT3	m_vWorldRotation = { 0.f, 0.f, 0.f };
	XMFLOAT3	m_vWorldScale = { 1.f, 1.f, 1.f };
	XMFLOAT4X4	m_matWorld;

	BOOL m_bLocalUpdated = FALSE;
	BOOL m_bWorldUpdated = FALSE;


};

