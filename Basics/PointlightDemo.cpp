#include "pch.h"
#include "PointlightDemo.h"
#include "PointlightResources.h"
#include "BasicForwardRenderResources.h"
#include "Object.h"
#include "Camera.h"

using namespace std;

////////////////////
// PointlightDemo //
////////////////////

void PointlightDemo::Initialize()
{
	m_pObjs.resize(8);
	for (auto& obj : m_pObjs)
	{
		obj = make_shared<PointlightObject>();
		obj->Initialize();
	}

	m_pLightObj = make_shared<BasicForwardObject>();
	m_pLightObj->Initialize();

	m_pPointlight = make_shared<Pointlight>();
	m_pPointlight->Initialize();
	{
		m_pPointlight->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_pPointlight->SetColor(XMFLOAT3(1.0f, 1.0f, 1.0f));

		m_pPointlight->SetLightIntensity(1.0f);
		m_pPointlight->SetConstantAttenuation(1.0f);
		m_pPointlight->SetLinearAttenuation(0.09f);
		m_pPointlight->SetQuadraticAttenuation(0.032f);
	}

	shared_ptr<PointlightObject> originObj = nullptr;
	{
		originObj = static_pointer_cast<PointlightObject>(m_pObjs[0]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(2.0f, 0.0f, 2.0f));

		originObj = static_pointer_cast<PointlightObject>(m_pObjs[1]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, 2.0f));

		originObj = static_pointer_cast<PointlightObject>(m_pObjs[2]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(-2.0f, 0.0f, 2.0f));

		originObj = static_pointer_cast<PointlightObject>(m_pObjs[3]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(2.0f, 0.0f, 0.0f));

		originObj = static_pointer_cast<PointlightObject>(m_pObjs[4]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(-2.0f, 0.0f, 0.0f));

		originObj = static_pointer_cast<PointlightObject>(m_pObjs[5]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(2.0f, 0.0f, -2.0f));

		originObj = static_pointer_cast<PointlightObject>(m_pObjs[6]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, -2.0f));

		originObj = static_pointer_cast<PointlightObject>(m_pObjs[7]);
		originObj->GetTransform()->SetWorldPosition(XMFLOAT3(-2.0f, 0.0f, -2.0f));
	}

	shared_ptr<BasicForwardObject> originLightObj = static_pointer_cast<BasicForwardObject>(m_pLightObj);
	originLightObj->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));

	CORE.GetMainCamera()->SetPosition(XMFLOAT3(0.f, 0.f, -6.f));

}

void PointlightDemo::Update()
{
	if (ImGui::Begin("Pointlight Control"s.c_str()))
	{
		ImGui::Text("Light Control");
		{
			XMFLOAT3 pos = m_pPointlight->GetPosition();
			XMFLOAT3 color = m_pPointlight->GetColor();

			float intensity = m_pPointlight->GetLightIntensity();
			float constant = m_pPointlight->GetConstantAttenuation();
			float linear = m_pPointlight->GetLinearAttenuation();
			float quadratic = m_pPointlight->GetQuadraticAttenuation();

			ImGui::DragFloat3("Position"s.c_str(), (float*)&pos, 0.01f, -10.f, 10.f);
			ImGui::DragFloat3("Color"s.c_str(), (float*)&color, 0.01f, 0.f, 1.f);

			ImGui::DragFloat("LightIntensity"s.c_str(), &intensity, 0.01f, 0.f, 5.f);
			ImGui::DragFloat("Constant Attenuation"s.c_str(), &constant, 0.01f, 0.f, 5.f);
			ImGui::DragFloat("Linear Attenuation"s.c_str(), &linear, 0.01f, 0.f, 5.f);
			ImGui::DragFloat("Quadratic Attenuation"s.c_str(), &quadratic, 0.01f, 0.f, 5.f);

			m_pPointlight->SetPosition(pos);
			m_pPointlight->SetColor(color);

			m_pPointlight->SetLightIntensity(intensity);
			m_pPointlight->SetConstantAttenuation(constant);
			m_pPointlight->SetLinearAttenuation(linear);
			m_pPointlight->SetQuadraticAttenuation(quadratic);
		}

		ImGui::Text("Object Material Control");
		{
			shared_ptr<PointlightObject> originObj = static_pointer_cast<PointlightObject>(m_pObjs[0]);
			XMFLOAT3 diffuse = originObj->GetDiffuseColor();

			ImGui::DragFloat3("Diffuse"s.c_str(), (float*)&diffuse, 0.01f, 0.f, 1.f);

			for (auto& obj : m_pObjs)
			{
				shared_ptr<PointlightObject> target = static_pointer_cast<PointlightObject>(obj);
				target->SetDiffuseColor(diffuse);
			}
		}

	}
	ImGui::End();

	shared_ptr<BasicForwardObject> originLightObj = static_pointer_cast<BasicForwardObject>(m_pLightObj);
	originLightObj->GetTransform()->SetWorldPosition(m_pPointlight->GetPosition());

	m_pLightObj->Update();

	for (auto& obj : m_pObjs)
	{
		obj->Update();
	}
}

void PointlightDemo::Render()
{
	for (auto& obj : m_pObjs)
	{
		RENDER.Add(obj);
	}

	RENDER.Add(m_pLightObj);
}

////////////////
// Pointlight //
////////////////

CBPointlightData Pointlight::GetPointlightCBData()
{
	return CBPointlightData{
		m_vLightPos,
		1.f,	// pad1
		m_vLightColor,
		1.f,	// pad2
		m_fLightIntensity,
		m_fConstantAttenuation,
		m_fLinearAttenuation,
		m_fQuadraticAttenuation
	};
}
