#include "pch.h"
#include "NormalMappingDemo.h"
#include "NormalMappingResources.h"
#include "Light.h"
#include "Camera.h"

using namespace NormalMapping;
using namespace std;

void NormalMappingDemo::Initialize()
{
	m_pObj = make_shared<NormalMappingObject>();
	m_pObj->Initialize();

	m_pDirectionalLight = make_shared<DirectionalLight>();
	m_pDirectionalLight->Initialize();

	m_pSpotLight = make_shared<SpotLight>();
	m_pSpotLight->Initialize();

	// Cam
	CORE.GetMainCamera()->SetPosition(XMFLOAT3(0.f, 0.f, -5.f));
}

void NormalMappingDemo::Update()
{

	// Make spot light attach to Camera
	SpotLight& refSpotLight = *static_pointer_cast<SpotLight>(m_pSpotLight);
	Camera& refCamera = *CORE.GetMainCamera();

	refSpotLight.SetPosition(refCamera.GetPosition());

	XMFLOAT3 front, right, up;
	refCamera.DecomposeCameraUVN(front, right, up);
	refSpotLight.SetDirection(front);

	if (ImGui::Begin("Control SpotLight"))
	{
		m_pSpotLight->ControlLightWithImGui(0);
		m_pDirectionalLight->ControlLightWithImGui(1);

		ImGui::End();
	}
}

void NormalMappingDemo::Render()
{
	RENDER.Add(m_pObj);
}

CBLightData NormalMappingDemo::GetLightCBDataInDemo()
{
	CBLightData data;

	data.lightData[0] = m_pDirectionalLight->GetLightRawData();
	data.lightData[1] = m_pSpotLight->GetLightRawData();

	data.nLights = 2;

	return data;
}
