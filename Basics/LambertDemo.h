#pragma once

#include "Application.h"

struct CBLambertData;

class LambertLight
{
public:
	LambertLight() {}
	~LambertLight() {}

public:
	BOOL Initialize() { return TRUE; }

public:
	void SetDirection(const XMFLOAT3& dir) { m_vlightDir = dir; }
	void SetColor(const XMFLOAT4& color) { m_vlightColor = color; }
	
	XMFLOAT3 GetDirection() { return m_vlightDir; }
	XMFLOAT4 GetColor() { return m_vlightColor; }

	CBLambertData GetLambertCBData();

private:
	XMFLOAT3 m_vlightDir = XMFLOAT3(0.f, 0.f, 1.f);
	XMFLOAT4 m_vlightColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

};

class LambertDemo : public Application
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

	std::shared_ptr<LambertLight>& GetLambertLight() { return m_pLambertLight; }

private:
	std::shared_ptr<Object> m_pObj;
	std::shared_ptr<LambertLight> m_pLambertLight;


};

