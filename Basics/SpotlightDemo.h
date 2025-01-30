#pragma once
#include "Application.h"

struct CBSpotlightData;

class SpotLight
{
public:
	SpotLight() {}
	~SpotLight() {}

public:
	BOOL Initialize() { return TRUE; }

public:
	void SetPosition(const XMFLOAT3& pos) { m_vLightPos = pos; }
	void SetDirection(const XMFLOAT3& dir) { m_vLightDir = dir; }
	void SetColor(const XMFLOAT3& color) { m_vLightColor = color; }

	void SetLightIntensity(float value) { m_fLightIntensity = value; }
	void SetInnerCone(float value) { m_fInnerCone = value; }
	void SetOuterCone(float value) { m_fOuterCone = value; }
	void SetLightRange(float value) { m_fLightRange = value; }
	void SetAttenuation(float value) { m_fAttenuation = value; }

	XMFLOAT3 GetPosition() { return m_vLightPos; }
	XMFLOAT3 GetDirection() { return m_vLightDir; }
	XMFLOAT3 GetColor() { return m_vLightColor; }

	float GetLightIntensity() { return m_fLightIntensity; }
	float GetInnerCone() { return m_fInnerCone; }
	float GetOuterCone() { return m_fOuterCone; }
	float GetLightRange() { return m_fLightRange; }
	float GetAttenuation() { return m_fAttenuation; }

	CBSpotlightData GetSpotlightCBData();

private:
	XMFLOAT3 m_vLightPos	= XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 m_vLightDir	= XMFLOAT3(0.f, 0.f, 1.f);
	XMFLOAT3 m_vLightColor	= XMFLOAT3(1.f, 1.f, 1.f);

	float m_fLightIntensity = 0.f;
	float m_fInnerConeCos	= 0.f;	// еш of inner cone 
	float m_fOuterConeCos	= 0.f;	// еш of outer cone 
	float m_fLightRange		= 0.f;
	float m_fAttenuation	= 0.f;

	float m_fInnerCone		= 0.f;
	float m_fOuterCone		= 0.f;
};

class SpotlightDemo : public Application
{
public:
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Render() override;


	std::shared_ptr<SpotLight>& GetSpotLight() { return m_pSpotlight; }

private:
	std::vector<std::shared_ptr<Object>> m_pObjs;
	std::shared_ptr<Object> m_pLightObj;
	std::shared_ptr<SpotLight> m_pSpotlight;


};

