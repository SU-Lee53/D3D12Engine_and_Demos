#pragma once
#include "Application.h"

struct CBPointlightData;

class DemoPointlight
{
public:
	DemoPointlight() {}
	~DemoPointlight() {}

public:
	BOOL Initialize() { return TRUE; }

public:
	void SetPosition(const XMFLOAT3& pos) { m_vLightPos = pos; }
	void SetColor(const XMFLOAT3& color) { m_vLightColor = color; }

	void SetLightIntensity(float value) { m_fLightIntensity = value; }
	void SetConstantAttenuation(float value) { m_fConstantAttenuation = value; }
	void SetLinearAttenuation(float value) { m_fLinearAttenuation = value; }
	void SetQuadraticAttenuation(float value) { m_fQuadraticAttenuation = value; }

	XMFLOAT3 GetPosition() { return m_vLightPos; }
	XMFLOAT3 GetColor() { return m_vLightColor; }

	float GetLightIntensity() { return m_fLightIntensity; }
	float GetConstantAttenuation() { return m_fConstantAttenuation; }
	float GetLinearAttenuation() { return m_fLinearAttenuation; }
	float GetQuadraticAttenuation() { return m_fQuadraticAttenuation; }

	CBPointlightData GetPointlightCBData();

private:
	XMFLOAT3 m_vLightPos = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 m_vLightColor = XMFLOAT3(1.f, 1.f, 1.f);

	float m_fLightIntensity = 0.f;
	float m_fConstantAttenuation = 0.f;
	float m_fLinearAttenuation = 0.f;
	float m_fQuadraticAttenuation = 0.f;
};


class PointlightDemo :
    public Application
{
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Render() override;

public:
	std::shared_ptr<DemoPointlight>& GetPointlight() { return m_pPointlight; }

private:
	std::vector<std::shared_ptr<Object>> m_pObjs;
	std::shared_ptr<Object> m_pLightObj;
	std::shared_ptr<DemoPointlight> m_pPointlight;

};

