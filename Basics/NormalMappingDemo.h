#pragma once
#include "Application.h"

class Object;
class Light_Base;

class NormalMappingDemo : public Application
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

public:
	CBLightData GetLightCBDataInDemo();


private:
	std::shared_ptr<Light_Base> m_pDirectionalLight = nullptr;
	std::shared_ptr<Light_Base> m_pSpotLight = nullptr;

	std::shared_ptr<Object> m_pObj = nullptr;

};

