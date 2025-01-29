#pragma once
#include "Application.h"

struct CBBlinnPhongData;

class BlinnPhongLight
{
public:
	BlinnPhongLight() {}
	~BlinnPhongLight() {}

public:
	BOOL Initialize() { return TRUE; }

public:
	void SetPosition(const XMFLOAT3& pos) { m_vlightPos = pos; }
	void SetDirection(const XMFLOAT3& dir) { m_vlightDir = dir; }
	void SetColor(const XMFLOAT4& color) { m_vlightColor = color; }

	XMFLOAT3 GetPosition() { return m_vlightPos; }
	XMFLOAT3 GetDirection() { return m_vlightDir; }
	XMFLOAT4 GetColor() { return m_vlightColor; }

	CBBlinnPhongData GetBlinnPhongCBData();

private:
	XMFLOAT3 m_vlightPos = XMFLOAT3(1.f, 1.f, 1.f);
	XMFLOAT3 m_vlightDir = XMFLOAT3(0.f, 0.f, 1.f);
	XMFLOAT4 m_vlightColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

};

class BlinnPhongDemo : public Application
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

	std::shared_ptr<BlinnPhongLight>& GetBlinnPhongLight() { return m_pBlinnPhongLight; }

private:
	std::shared_ptr<Object> m_pObj;
	std::shared_ptr<Object> m_pLightObj;
	std::shared_ptr<BlinnPhongLight> m_pBlinnPhongLight;


};

