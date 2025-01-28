#pragma once
#include "Application.h"
class HierarchyModelDemo : public Application
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	std::shared_ptr<Object> m_pObj;

	float m_fRotY = 0.f;

	// mouse
	POINT m_MousePoint;
	float m_fDeltaX = 0.f;
	float m_fDeltaY = 0.f;

	float m_sensitivity = 1.0f;
};

