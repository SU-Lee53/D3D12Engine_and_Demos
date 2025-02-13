#include "pch.h"
#include "NormalMappingDemo.h"
#include "NormalMappingResources.h"
#include "Light.h"
#include "Camera.h"

using namespace NormalMapping;
using namespace std;

void NormalMappingDemo::Initialize()
{
	// Cam
	m_pMainCamera = make_shared<Camera>();
	m_pMainCamera->Initialize();

	m_pObj = make_shared<NormalMappingObject>();
	m_pObj->Initialize();

	m_pDirectionalLight = make_shared<DirectionalLight>();
	m_pDirectionalLight->Initialize();

	m_pSpotLight = make_shared<SpotLight>();
	m_pSpotLight->Initialize();

	// Cam
	m_pMainCamera->SetPosition(XMFLOAT3(0.f, 0.f, -5.f));
}

void NormalMappingDemo::Update()
{
	// Make spot light attach to Camera
	SpotLight& refSpotLight = *static_pointer_cast<SpotLight>(m_pSpotLight);

	refSpotLight.SetPosition(m_pMainCamera->GetPosition());

	XMFLOAT3 front, right, up;
	m_pMainCamera->DecomposeCameraUVN(front, right, up);
	refSpotLight.SetDirection(front);

	if (ImGui::Begin("Control SpotLight"))
	{
		m_pSpotLight->ControlLightWithImGui(0);
		m_pDirectionalLight->ControlLightWithImGui(1);

		ImGui::End();
	}

	m_pMainCamera->Update();
}

void NormalMappingDemo::Render()
{
	RENDER.Add(m_pObj, m_pMainCamera);
}

CBLightData NormalMappingDemo::GetLightCBDataInDemo()
{
	CBLightData data;

	data.lightData[0] = m_pDirectionalLight->GetLightRawData();
	data.lightData[1] = m_pSpotLight->GetLightRawData();

	data.nLights = 2;

	return data;
}
