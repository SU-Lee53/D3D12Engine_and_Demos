#pragma once
#include "Application.h"

class InstancingDemo : public Application
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	std::shared_ptr<Object> m_pInstancingObj = nullptr;

};

