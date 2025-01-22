#include "stdafx.h"
#include "Material.h"

using namespace std;

Material::Material()
{

}

Material::~Material()
{

}

BOOL Material::Initialize(const std::wstring& wstrTexturePath)
{
	m_pDiffuseTexture = make_unique<Texture>();
	m_pShader = make_unique<Shader>();

	m_pDiffuseTexture->Initialize(wstrTexturePath);
	m_pShader->InitializeAsDefault();

	return TRUE;
}
