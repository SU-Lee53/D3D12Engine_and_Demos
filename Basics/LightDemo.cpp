#include "pch.h"
#include "LightDemo.h"
#include "LightResources.h"
#include "BasicForwardRenderResources.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"

using namespace std;
using namespace BasicForward;

void LightDemo::Initialize()
{
	// Obj
	{
		m_pObjs.resize(8);
		for (auto& obj : m_pObjs)
		{
			obj = make_shared<Light::LightObject>();
			obj->Initialize();
		}

		shared_ptr<Light::LightObject> originObj = nullptr;
		{
			originObj = static_pointer_cast<Light::LightObject>(m_pObjs[0]);
			originObj->GetTransform()->SetWorldPosition(XMFLOAT3(2.0f, 0.0f, 0.0f));

			originObj = static_pointer_cast<Light::LightObject>(m_pObjs[1]);
			originObj->GetTransform()->SetWorldPosition(XMFLOAT3(-2.0f, 0.0f, 0.0f));

			originObj = static_pointer_cast<Light::LightObject>(m_pObjs[2]);
			originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, 2.0f));

			originObj = static_pointer_cast<Light::LightObject>(m_pObjs[3]);
			originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, -2.0f));

			originObj = static_pointer_cast<Light::LightObject>(m_pObjs[4]);
			originObj->GetTransform()->SetWorldPosition(XMFLOAT3(4.0f, 0.0f, 0.0f));

			originObj = static_pointer_cast<Light::LightObject>(m_pObjs[5]);
			originObj->GetTransform()->SetWorldPosition(XMFLOAT3(-4.0f, 0.0f, 0.0f));

			originObj = static_pointer_cast<Light::LightObject>(m_pObjs[6]);
			originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, 4.0f));

			originObj = static_pointer_cast<Light::LightObject>(m_pObjs[7]);
			originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, -4.0f));
		}
	}

	// Lights
	SetLights();

	// Cam
	CORE.GetMainCamera()->SetPosition(XMFLOAT3(0.f, 0.f, -6.f));
}

void LightDemo::Update()
{
	if(ImGui::Begin("Control Scene"))
	{
		for (int i = 0; i < m_pLightPairs.size(); i++)
		{
			m_pLightPairs[i].first->ControlLightWithImGui(i);

			LIGHT_TYPE ty = m_pLightPairs[i].first->GetLightType();

			switch (ty)
			{
			case LIGHT_TYPE_DIRECTIONAL:
				static_pointer_cast<BasicForwardObject>(m_pLightPairs[i].second)->GetTransform()->SetWorldPosition(static_pointer_cast<DirectionalLight>(m_pLightPairs[i].first)->GetPosition());
				break;
			case LIGHT_TYPE_POINT_LIGHT:
				static_pointer_cast<BasicForwardObject>(m_pLightPairs[i].second)->GetTransform()->SetWorldPosition(static_pointer_cast<PointLight>(m_pLightPairs[i].first)->GetPosition());
				break;
			case LIGHT_TYPE_SPOT_LIGHT:
				static_pointer_cast<BasicForwardObject>(m_pLightPairs[i].second)->GetTransform()->SetWorldPosition(static_pointer_cast<SpotLight>(m_pLightPairs[i].first)->GetPosition());
			break;

			default:
				break;
			}
		}

	}



	for (auto& pair : m_pLightPairs)
	{
		pair.first->Update();
		pair.second->Update();
	}

	for (auto& obj : m_pObjs)
	{
		obj->Update();
	}
}

void LightDemo::Render()
{
	std::for_each(m_pLightPairs.begin(), m_pLightPairs.end(), [](LightPair& p) { RENDER.Add(p.second); });
	std::for_each(m_pObjs.begin(), m_pObjs.end(), [](std::shared_ptr<Object> obj) { RENDER.Add(obj); });
}

void LightDemo::SetLights()
{
	m_pLightPairs.resize(6);

	XMFLOAT3 pos(0.f, 0.f, 0.f);

	pos = XMFLOAT3(0.f, 5.f, 0.f);
	m_pLightPairs[0].first = make_shared<SpotLight>();
	m_pLightPairs[0].second = make_shared<BasicForwardObject>();
	m_pLightPairs[0].first->Initialize();
	m_pLightPairs[0].second->Initialize();
	static_pointer_cast<SpotLight>(m_pLightPairs[0].first)->SetPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[0].second)->GetTransform()->SetWorldPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[0].second)->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	
	pos = XMFLOAT3(2.f, 0.f, 2.f);
	m_pLightPairs[1].first = make_shared<PointLight>();
	m_pLightPairs[1].second = make_shared<BasicForwardObject>();
	m_pLightPairs[1].first->Initialize();
	m_pLightPairs[1].second->Initialize();
	static_pointer_cast<PointLight>(m_pLightPairs[1].first)->SetPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[1].second)->GetTransform()->SetWorldPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[1].second)->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	
	pos = XMFLOAT3(2.f, 0.f, -2.f);
	m_pLightPairs[2].first = make_shared<PointLight>();
	m_pLightPairs[2].second = make_shared<BasicForwardObject>();
	m_pLightPairs[2].first->Initialize();
	m_pLightPairs[2].second->Initialize();
	static_pointer_cast<PointLight>(m_pLightPairs[2].first)->SetPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[2].second)->GetTransform()->SetWorldPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[2].second)->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	
	pos = XMFLOAT3(-2.f, 0.f, 2.f);
	m_pLightPairs[3].first = make_shared<PointLight>();
	m_pLightPairs[3].second = make_shared<BasicForwardObject>();
	m_pLightPairs[3].first->Initialize();
	m_pLightPairs[3].second->Initialize();
	static_pointer_cast<PointLight>(m_pLightPairs[3].first)->SetPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[3].second)->GetTransform()->SetWorldPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[3].second)->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	
	pos = XMFLOAT3(-2.f, 0.f, -2.f);
	m_pLightPairs[4].first = make_shared<PointLight>();
	m_pLightPairs[4].second = make_shared<BasicForwardObject>();
	m_pLightPairs[4].first->Initialize();
	m_pLightPairs[4].second->Initialize();
	static_pointer_cast<PointLight>(m_pLightPairs[4].first)->SetPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[4].second)->GetTransform()->SetWorldPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[4].second)->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	
	pos = XMFLOAT3(0.f, 0.f, 0.f);
	m_pLightPairs[5].first = make_shared<DirectionalLight>();
	m_pLightPairs[5].second = make_shared<BasicForwardObject>();
	m_pLightPairs[5].first->Initialize();
	m_pLightPairs[5].second->Initialize();
	static_pointer_cast<DirectionalLight>(m_pLightPairs[5].first)->SetPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[5].second)->GetTransform()->SetWorldPosition(pos);
	static_pointer_cast<BasicForwardObject>(m_pLightPairs[5].second)->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));

}

CBLightData LightDemo::GetLightCBDataInDemo()
{
	assert(m_pLightPairs.size() < MAX_LIGHT_COUNT);
	CBLightData data;

	int nLights = m_pLightPairs.size();
	for (int i = 0; i < nLights; i++)
	{
		data.lightData[i] = m_pLightPairs[i].first->GetLightRawData();
	}

	data.nLights = nLights;

	return data;
}
