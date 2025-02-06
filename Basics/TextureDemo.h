#pragma once
#include "Application.h"

class TextureDemo : public Application
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	std::shared_ptr<Object> m_pObj;

};

