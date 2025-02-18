#include "pch.h"
#include "SkyboxDemo.h"
#include "Skybox.h"

using namespace std;

void SkyboxDemo::Initialize()
{
	// Cam
	m_pMainCamera = make_shared<Camera>();
	m_pMainCamera->Initialize();

	m_pSkybox = make_shared<Skybox>();
	m_pSkybox->Initialize(L"../Models/Texture/skybox.png");
}

void SkyboxDemo::Update()
{
	m_pMainCamera->Update();
}

void SkyboxDemo::Render()
{
	RENDER.Add(m_pSkybox, m_pMainCamera);
}
