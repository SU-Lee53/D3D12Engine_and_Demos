#include "stdafx.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

using namespace std;

Material::Material()
{

}

Material::~Material()
{

}

BOOL Material::Initialize(const std::wstring& wstrTexturePath)
{
	m_pTexture = make_shared<Texture>();
	m_pShader = make_shared<Shader>();

	m_pTexture->Initialize(wstrTexturePath);
	m_pShader->InitializeAsDefault();

	return TRUE;
}
