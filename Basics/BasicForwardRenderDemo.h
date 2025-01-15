#pragma once
#include "Application.h"

class BasicForwardRenderDemo : public Application
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	std::shared_ptr<Object> m_pObj;

	float m_fRotY = 0.f;

	// mouse
	POINT _point;
	float m_deltaX = 0.f;
	float m_deltaY = 0.f;

	float m_sensitivity = 1.0f;
};

