#pragma once
#include "Application.h"

class Light;

using LightPair = std::pair<std::shared_ptr<Light>, std::shared_ptr<Object>>;

class LightDemo : public Application
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

public:
	void SetLights();

public:
	CBLightData GetLightCBDataInDemo();

private:
	std::vector<LightPair> m_pLightPairs = {};
	std::vector<std::shared_ptr<Object>> m_pObjs = {};

};

