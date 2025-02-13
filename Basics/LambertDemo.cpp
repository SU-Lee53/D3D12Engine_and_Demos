#include "pch.h"
#include "LambertDemo.h"
#include "LambertResources.h"
#include "Object.h"
#include "Camera.h"

using namespace std;

void LambertDemo::Initialize()
{
	// Cam
	m_pMainCamera = make_shared<Camera>();
	m_pMainCamera->Initialize();

	m_pObj = make_shared<Lambert::LambertObject>();
	m_pObj->Initialize();
	
	m_pLambertLight = make_shared<LambertLight>();
	m_pLambertLight->Initialize();

	shared_ptr<Lambert::LambertObject> originObj = static_pointer_cast<Lambert::LambertObject>(m_pObj);
	//originObj->LoadFromBinaryFile(L"../Models/Binaries/Hummer.bin");
	originObj->GetTransform()->SetWorldPosition(XMFLOAT3(0.0f, 0.0f, 3.0f));

}

void LambertDemo::Update()
{
	if(ImGui::Begin("Lambert Control"s.c_str()))
	{
		XMFLOAT3 dir = m_pLambertLight->GetDirection();
		XMFLOAT4 color = m_pLambertLight->GetColor();

		ImGui::DragFloat3("Direction"s.c_str(), (float*)&dir, 0.01f, -1.f, 1.f);
		ImGui::DragFloat4("Color"s.c_str(), (float*)&color, 0.01f, 0.f, 1.f);

		m_pLambertLight->SetDirection(dir);
		m_pLambertLight->SetColor(color);
	}
	ImGui::End();


	m_pObj->Update();
	m_pMainCamera->Update();
}

void LambertDemo::Render()
{
	RENDER.Add(m_pObj, m_pMainCamera);
}


//////////////////
// LambertLight //
//////////////////

Lambert::CBLambertData LambertLight::GetLambertCBData()
{
	return Lambert::CBLambertData{ m_vlightDir, 0.f/*padding*/, m_vlightColor };
}
