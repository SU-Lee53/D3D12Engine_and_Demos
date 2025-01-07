#pragma once

class Transform
{
public:
	Transform();
	~Transform();

	BOOL Initialize();
	void Update();

	void SetPosition(Vec3 pos);
	void SetScale(Vec3 scale);
	void SetRotation(Vec3 rot);

	Vec3 GetPosition() { return m_vPos; }
	Vec3 GetScale() { return m_vScale; }
	Vec3 GetRotation() { return m_vRotation; }

	Matrix GetWorldMatrix() { return m_matWorld; }
	Matrix GetWorldMatrixTransposed() { return XMMatrixTranspose(m_matWorld); }

private:
	Vec3 m_vPos = { 0.f,0.f,0.f };
	Vec3 m_vScale = { 1.f,1.f,1.f };
	Vec3 m_vRotation = { 0.f,0.f,0.f };
	Matrix m_matWorld = Matrix::Identity;

	BOOL m_bUpdated = FALSE;

};

