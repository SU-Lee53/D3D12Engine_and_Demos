#include "pch.h"
#include "LambertDemo.h"
#include "LambertResources.h"
#include "Object.h"
#include "Camera.h"

using namespace std;

void LambertDemo::Initialize()
{
	m_pObj = make_shared<LambertObject>();
	m_pObj->Initialize();
	
	m_pLambertLight = make_shared<LambertLight>();
	m_pLambertLight->Initialize();

	shared_ptr<LambertObject> originObj = static_pointer_cast<LambertObject>(m_pObj);
	//originObj->LoadFromBinaryFile(L"../Models/Binaries/Hummer.bin");
	//originObj->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));

	XMFLOAT4 colorKhaki;
	XMStoreFloat4(&colorKhaki, Colors::DarkOliveGreen);
	//originObj->SetColor(colorKhaki);

}

void LambertDemo::Update()
{
	if(ImGui::Begin("Lambert"s.c_str()))
	{
		XMFLOAT3 dir = m_pLambertLight->GetDirection();
		XMFLOAT4 color = m_pLambertLight->GetColor();

		ImGui::DragFloat3("Direction"s.c_str(), (float*)&dir, 0.2f);
		ImGui::DragFloat4("Color"s.c_str(), (float*)&color, 0.2f);

		m_pLambertLight->SetDirection(dir);
		m_pLambertLight->SetColor(color);
	}
	ImGui::End();


	m_pObj->Update();

}

void LambertDemo::Render()
{
	RENDER.Add(m_pObj);
}


//////////////////
// LambertLight //
//////////////////

CBLambertData LambertLight::GetLambertCBData()
{
	return CBLambertData{ m_vlightDir, m_vlightColor };
}
