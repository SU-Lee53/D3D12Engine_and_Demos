#include "pch.h"
#include "TerrainDemo.h"
#include "Terrain.h"

using namespace std;

void TerrainDemo::Initialize()
{
	// Cam
	m_pMainCamera = make_shared<Camera>();
	m_pMainCamera->Initialize();

	m_pTerrain = make_shared<Terrain>();
	shared_ptr<Terrain>& pTerrain = static_pointer_cast<Terrain>(m_pTerrain);
	pTerrain->Initialize(L"../Models/Texture/grass.jpg", L"../Models/Texture/height.png");

}

void TerrainDemo::Update()
{
	m_pTerrain->Update();
	m_pMainCamera->Update();
}

void TerrainDemo::Render()
{
	RENDER.Add(m_pTerrain, m_pMainCamera);
}
