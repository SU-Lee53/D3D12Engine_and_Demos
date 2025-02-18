#pragma once
#include "Application.h"

class Skybox;

class SkyboxDemo : public Application
{
public:
	void Initialize() override;
	void Update() override;
	void Render() override;

private:
	std::shared_ptr<Skybox> m_pSkybox = nullptr;

};

