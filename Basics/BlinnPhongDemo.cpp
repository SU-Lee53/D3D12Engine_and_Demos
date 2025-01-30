#include "pch.h"
#include "BlinnPhongDemo.h"
#include "BlinnPhongResource.h"
#include "BasicForwardRenderResources.h"
#include "Object.h"
#include "Camera.h"

using namespace std;


////////////////////
// BlinnPhongDemo //
////////////////////

void BlinnPhongDemo::Initialize()
{
	m_pObj = make_shared<BlinnPhongObject>();
	m_pObj->Initialize();
	
	m_pLightObj = make_shared<BasicForwardObject>();
	m_pLightObj->Initialize();

	m_pBlinnPhongLight = make_shared<BlinnPhongLight>();
	m_pBlinnPhongLight->Initialize();

	shared_ptr<BlinnPhongObject> originObj = static_pointer_cast<BlinnPhongObject>(m_pObj);
	originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, 3.0f));

	shared_ptr<BasicForwardObject> originLightObj = static_pointer_cast<BasicForwardObject>(m_pLightObj);
	originLightObj->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
}

void BlinnPhongDemo::Update()
{
	if (ImGui::Begin("BlinnPhong Control"s.c_str()))
	{
		ImGui::Text("Light Control");
		{
			XMFLOAT3 pos = m_pBlinnPhongLight->GetPosition();
			XMFLOAT3 dir = m_pBlinnPhongLight->GetDirection();
			XMFLOAT4 color = m_pBlinnPhongLight->GetColor();

			ImGui::DragFloat3("Position"s.c_str(), (float*)&pos, 0.01f, -10.f, 10.f);
			ImGui::DragFloat3("Direction"s.c_str(), (float*)&dir, 0.01f, -1.f, 1.f);
			ImGui::DragFloat3("Color"s.c_str(), (float*)&color, 0.01f, 0.f, 1.f);

			m_pBlinnPhongLight->SetPosition(pos);
			m_pBlinnPhongLight->SetDirection(dir);
			m_pBlinnPhongLight->SetColor(color);
		}

		ImGui::Text("Object Material Control");
		{
			shared_ptr<BlinnPhongObject> originObj = static_pointer_cast<BlinnPhongObject>(m_pObj);
			XMFLOAT3 diffuse = originObj->GetDiffuseColor();
			XMFLOAT3 specular = originObj->GetSpecularColor();
			XMFLOAT3 ambient = originObj->GetAmbientColor();
			float shiness = originObj->GetShiness();

			ImGui::DragFloat3("Diffuse"s.c_str(), (float*)&diffuse, 0.01f, 0.f, 1.f);
			ImGui::DragFloat3("Specular"s.c_str(), (float*)&specular, 0.01f, 0.f, 1.f);
			ImGui::DragFloat3("Ambient"s.c_str(), (float*)&ambient, 0.01f, 0.f, 1.f);
			ImGui::DragFloat("Shiness"s.c_str(), &shiness, 0.01f, 0.f, 100.f);

			originObj->SetDiffuseColor(diffuse);
			originObj->SetSpecularColor(specular);
			originObj->SetAmbientColor(ambient);
			originObj->SetShiness(shiness);
		}

	}
	ImGui::End();

	shared_ptr<BasicForwardObject> originLightObj = static_pointer_cast<BasicForwardObject>(m_pLightObj);
	originLightObj->GetTransform()->SetWorldPosition(m_pBlinnPhongLight->GetPosition());

	m_pLightObj->Update();

	m_pObj->Update();

}

void BlinnPhongDemo::Render()
{
	RENDER.Add(m_pObj);
	RENDER.Add(m_pLightObj);
}

/////////////////////
// BlinnPhongLight //
/////////////////////

CBBlinnPhongData BlinnPhongLight::GetBlinnPhongCBData()
{
	return CBBlinnPhongData{ m_vlightPos, m_vlightDir, 1/*padding*/, m_vlightColor };
}
