#include "pch.h"
#include "SpotlightDemo.h"
#include "SpotlightResources.h"
#include "BasicForwardRenderResources.h"
#include "Object.h"
#include "Camera.h"

using namespace std;

///////////////////
// SpitlightDemo //
///////////////////

void SpotlightDemo::Initialize()
{
	m_pObjs.resize(9);
	for (auto& obj : m_pObjs)
	{
		obj = make_shared<SpotlightObject>();
		obj->Initialize();
	}

	m_pLightObj = make_shared<BasicForwardObject>();
	m_pLightObj->Initialize();

	m_pSpotlight = make_shared<DemoSpotLight>();
	m_pSpotlight->Initialize();
	{
		m_pSpotlight->SetPosition(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_pSpotlight->SetDirection(XMFLOAT3(0.0f, -1.0f, 0.0f));
		m_pSpotlight->SetColor(XMFLOAT3(1.0f, 1.0f, 1.0f));

		m_pSpotlight->SetLightIntensity(1.0f);
		m_pSpotlight->SetInnerCone(10.f);
		m_pSpotlight->SetOuterCone(45.f);
		m_pSpotlight->SetLightRange(20.0f);
		m_pSpotlight->SetAttenuation(1.0f);
	}

	shared_ptr<SpotlightObject> originObj = nullptr;
	{
		originObj = static_pointer_cast<SpotlightObject>(m_pObjs[0]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(2.0f, 0.0f, 2.0f));

		originObj = static_pointer_cast<SpotlightObject>(m_pObjs[1]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, 2.0f));
		
		originObj = static_pointer_cast<SpotlightObject>(m_pObjs[2]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(-2.0f, 0.0f, 2.0f));
		
		originObj = static_pointer_cast<SpotlightObject>(m_pObjs[3]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(2.0f, 0.0f, 0.0f));

		originObj = static_pointer_cast<SpotlightObject>(m_pObjs[4]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
		
		originObj = static_pointer_cast<SpotlightObject>(m_pObjs[5]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(-2.0f, 0.0f, 0.0f));
		
		originObj = static_pointer_cast<SpotlightObject>(m_pObjs[6]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(2.0f, 0.0f, -2.0f));

		originObj = static_pointer_cast<SpotlightObject>(m_pObjs[7]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, -2.0f));
		
		originObj = static_pointer_cast<SpotlightObject>(m_pObjs[8]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(-2.0f, 0.0f, -2.0f));
	}

	shared_ptr<BasicForwardObject> originLightObj = static_pointer_cast<BasicForwardObject>(m_pLightObj);
	originLightObj->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));

	CORE.GetMainCamera()->SetPosition(XMFLOAT3(0.f, 0.f, -6.f));

}

void SpotlightDemo::Update()
{
	if (ImGui::Begin("Spotlight Control"s.c_str()))
	{
		ImGui::Text("Light Control");
		{
			XMFLOAT3 pos = m_pSpotlight->GetPosition();
			XMFLOAT3 dir = m_pSpotlight->GetDirection();
			XMFLOAT3 color = m_pSpotlight->GetColor();
			
			float lightIntensity = m_pSpotlight->GetLightIntensity();
			float innerCone = m_pSpotlight->GetInnerCone();
			float outerCone = m_pSpotlight->GetOuterCone();
			float lightRange = m_pSpotlight->GetLightRange();
			float attenuation = m_pSpotlight->GetAttenuation();

			ImGui::DragFloat3("Position"s.c_str(), (float*)&pos, 0.01f, -10.f, 10.f);
			ImGui::DragFloat3("Direction"s.c_str(), (float*)&dir, 0.01f, -1.f, 1.f);
			ImGui::DragFloat3("Color"s.c_str(), (float*)&color, 0.01f, 0.f, 1.f);
			
			ImGui::DragFloat("LightIntensity"s.c_str(), &lightIntensity, 0.1f, 0.f, 10.f);
			ImGui::DragFloat("InnerCone"s.c_str(), &innerCone, 0.1f, 0.f, 90.f);
			ImGui::DragFloat("OuterCone"s.c_str(), &outerCone, 0.1f, 0.f, 90.f);
			ImGui::DragFloat("LightRange"s.c_str(), &lightRange, 0.1f, 0.f, 50.f);
			ImGui::DragFloat("Attenuation"s.c_str(), &attenuation, 0.1f, 0.f, 1.f);

			m_pSpotlight->SetPosition(pos);
			m_pSpotlight->SetDirection(dir);
			m_pSpotlight->SetColor(color);

			m_pSpotlight->SetLightIntensity(lightIntensity);
			m_pSpotlight->SetInnerCone(innerCone);
			m_pSpotlight->SetOuterCone(outerCone);
			m_pSpotlight->SetLightRange(lightRange);
			m_pSpotlight->SetAttenuation(attenuation);
		}

		ImGui::Text("Object Material Control");
		{
			shared_ptr<SpotlightObject> originObj = static_pointer_cast<SpotlightObject>(m_pObjs[0]);
			XMFLOAT3 diffuse = originObj->GetDiffuseColor();

			ImGui::DragFloat3("Diffuse"s.c_str(), (float*)&diffuse, 0.01f, 0.f, 1.f);
			
			for (auto& obj : m_pObjs)
			{
				shared_ptr<SpotlightObject> target = static_pointer_cast<SpotlightObject>(obj);
				target->SetDiffuseColor(diffuse);
			}
		}

	}
	ImGui::End();

	shared_ptr<BasicForwardObject> originLightObj = static_pointer_cast<BasicForwardObject>(m_pLightObj);
	originLightObj->GetTransform()->SetWorldPosition(m_pSpotlight->GetPosition());

	m_pLightObj->Update();

	for (auto& obj : m_pObjs)
	{
		obj->Update();
	}
	

}

void SpotlightDemo::Render()
{
	for (auto& obj : m_pObjs)
	{
		RENDER.Add(obj);
	}

	RENDER.Add(m_pLightObj);
}

///////////////
// SpotLight //
///////////////

CBSpotlightData DemoSpotLight::GetSpotlightCBData()
{
	return CBSpotlightData{
		m_vLightPos,
		1.f,	// pad1
		m_vLightDir,
		1.f,	// pad2
		m_vLightColor,
		1.f,	// pad3
		m_fLightIntensity,
		std::cosf(XMConvertToRadians(m_fInnerCone)),
		std::cosf(XMConvertToRadians(m_fOuterCone)),
		m_fLightRange,
		m_fAttenuation
	};
}
