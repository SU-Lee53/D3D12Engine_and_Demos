#pragma once
#include "LightDesc.h"

class Light_Base
{
public:
	virtual BOOL Initialize() = 0;
	virtual void Update() = 0;

public:
	virtual RawLightData GetLightRawData() = 0;

	LIGHT_TYPE GetLightType() { return m_lightType; }

	virtual void ControlLightWithImGui(int index) = 0;
	void SetOnOFF(BOOL value) { m_bEnable = FALSE; }
	BOOL GetOnOFF() { return m_bEnable; }

protected:
	LIGHT_TYPE m_lightType;
	UnifiedLightData m_desc = {};
	BOOL m_bEnable = TRUE;
};

///////////////////////
// Directional Light //
///////////////////////

class DirectionalLight : public Light_Base
{
public:
	virtual BOOL Initialize() override;
	virtual void Update() override;

public:
	virtual RawLightData GetLightRawData() override;
	virtual void ControlLightWithImGui(int index) override;

public:
	void SetPosition(const XMFLOAT3& pos) { m_desc.DirectionalLight.lightPos = pos; }
	void SetDirection(const XMFLOAT3& dir) { m_desc.DirectionalLight.lightDir = dir; }
	void SetColor(const XMFLOAT3& color) { m_desc.DirectionalLight.lightColor = color; }

	XMFLOAT3 GetPosition() { return m_desc.DirectionalLight.lightPos; }
	XMFLOAT3 GetDirection() { return m_desc.DirectionalLight.lightDir; }
	XMFLOAT3 GetColor() { return m_desc.DirectionalLight.lightColor; }

};

/////////////////
// Point Light //
/////////////////

class PointLight : public Light_Base
{
public:
	virtual BOOL Initialize() override;
	virtual void Update() override;

public:
	virtual RawLightData GetLightRawData() override;
	virtual void ControlLightWithImGui(int index) override;

public:
	void SetPosition(const XMFLOAT3& pos) { m_desc.PointLight.lightPos = pos; }
	void SetColor(const XMFLOAT3& color) { m_desc.PointLight.lightColor = color; }

	void SetLightIntensity(float value) { m_desc.PointLight.lightIntensity = value; }
	void SetConstantAttenuation(float value) { m_desc.PointLight.constantAttenuation = value; }
	void SetLinearAttenuation(float value) { m_desc.PointLight.linearAttenuation = value; }
	void SetQuadraticAttenuation(float value) { m_desc.PointLight.QuadraticAttenuation = value; }

	XMFLOAT3 GetPosition() { return m_desc.PointLight.lightPos; }
	XMFLOAT3 GetColor() { return m_desc.PointLight.lightColor; }

	float GetLightIntensity() { return m_desc.PointLight.lightIntensity; }
	float GetConstantAttenuation() { return m_desc.PointLight.constantAttenuation; }
	float GetLinearAttenuation() { return m_desc.PointLight.linearAttenuation; }
	float GetQuadraticAttenuation() { return m_desc.PointLight.QuadraticAttenuation; }

};

////////////////
// Spot Light //
////////////////

class SpotLight : public Light_Base
{
public:
	virtual BOOL Initialize() override;
	virtual void Update() override;

public:
	virtual RawLightData GetLightRawData() override;
	virtual void ControlLightWithImGui(int index) override;

public:
	void SetPosition(const XMFLOAT3& pos) { m_desc.SpotLight.lightPos = pos; }
	void SetDirection(const XMFLOAT3& dir) { m_desc.SpotLight.lightDir = dir; }
	void SetColor(const XMFLOAT3& color) { m_desc.SpotLight.lightColor = color; }

	void SetLightIntensity(float value) { m_desc.SpotLight.lightIntensity = value; }
	void SetInnerCone(float value) { m_desc.SpotLight.innerCone = value; }
	void SetOuterCone(float value) { m_desc.SpotLight.outerCone = value; }
	void SetLightRange(float value) { m_desc.SpotLight.lightRange = value; }
	void SetAttenuation(float value) { m_desc.SpotLight.attenuation = value; }

	XMFLOAT3 GetPosition() { return m_desc.SpotLight.lightPos; }
	XMFLOAT3 GetDirection() { return m_desc.SpotLight.lightDir; }
	XMFLOAT3 GetColor() { return m_desc.SpotLight.lightColor; }

	float GetLightIntensity() { return m_desc.SpotLight.lightIntensity; }
	float GetInnerCone() { return m_desc.SpotLight.innerCone; }
	float GetOuterCone() { return m_desc.SpotLight.outerCone; }
	float GetLightRange() { return m_desc.SpotLight.lightRange; }
	float GetAttenuation() { return m_desc.SpotLight.attenuation; }

};