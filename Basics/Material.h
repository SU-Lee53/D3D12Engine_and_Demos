#pragma once

class Material
{
public:
	Material();
	~Material();

private:
	std::shared_ptr<class Texture> m_pTexture = nullptr;
	std::shared_ptr<class RenderMethod> m_pRenderMethod = nullptr;

};

