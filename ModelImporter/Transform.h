#pragma once

struct CB_TRANSFORM
{
	XMMATRIX matLocal;
	XMMATRIX matWorld;
};

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
	XMFLOAT3 GetLocalScale() { return m_vLocalRotation; }
	XMFLOAT3 GetLocalRotation() { return m_vLocalScale; }
	XMMATRIX GetLocalMatrix() { return m_matLocal; }
	XMMATRIX GetLocalMatrixTransposed() { return XMMatrixTranspose(m_matLocal); }

	XMFLOAT3 GetWorldPosition() { return m_vWorldPosition; }
	XMFLOAT3 GetWorldScale() { return m_vWorldRotation; }
	XMFLOAT3 GetWorldRotation() { return m_vWorldScale; }
	XMMATRIX GetWorldMatrix() { return m_matWorld; }
	XMMATRIX GetWorldMatrixTransposed() { return XMMatrixTranspose(m_matWorld); }

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
	XMFLOAT3 m_vLocalPosition	= { 0.f, 0.f, 0.f };
	XMFLOAT3 m_vLocalRotation	= { 0.f, 0.f, 0.f };
	XMFLOAT3 m_vLocalScale		= { 1.f, 1.f, 1.f };
	XMMATRIX m_matLocal;

	// World Transform
	XMFLOAT3 m_vWorldPosition	= { 0.f, 0.f, 0.f };
	XMFLOAT3 m_vWorldRotation	= { 0.f, 0.f, 0.f };
	XMFLOAT3 m_vWorldScale		= { 1.f, 1.f, 1.f };
	XMMATRIX m_matWorld;

	BOOL m_bLocalUpdated = FALSE;
	BOOL m_bWorldUpdated = FALSE;


};

