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

	// mouse
	POINT _point;
	float m_deltaX = 0.f;
	float m_deltaY = 0.f;

	float m_sensitivity = 1.0f;
};

