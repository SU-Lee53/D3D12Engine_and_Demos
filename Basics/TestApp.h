#pragma once
#include "Application.h"
class TestApp : public Application
{
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
};

