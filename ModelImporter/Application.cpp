#include "stdafx.h"
#include "Application.h"
#include "Importer.h"
#include "Model.h"
#include "Camera.h"

using namespace std;

Application::Application()
{
}

Application::~Application()
{
}

void Application::Initialize()
{
	m_pCamera = make_shared<Camera>();
	m_pCamera->Initialize();

	m_pImporter = make_unique<FbxLoader>();
	m_pImporter->Initialize();

	m_pImporter->LoadFBXFile("../fbx/Gunship.fbx");

	m_pModel = make_shared<Model>();
	m_pImporter->ExportModelInSceneToModel(m_pModel);

}

void Application::Update()
{
	if (ImGui::Begin("Node"))
	{
		m_pImporter->ShowFBXNodeToImGui();
	}
	ImGui::End();

	XMFLOAT3 pos = m_pCamera->GetPosition();
	if (INPUT.GetButton(KEY_TYPE::W))
	{
		pos.z += 1.0f;
	}
	if (INPUT.GetButton(KEY_TYPE::S))
	{
		pos.z -= 1.0f;
	}
	if (INPUT.GetButton(KEY_TYPE::D))
	{
		pos.x += 1.0f;
	}
	if (INPUT.GetButton(KEY_TYPE::A))
	{
		pos.x -= 1.0f;
	}

	m_pCamera->SetPosition(pos);
	m_pCamera->Update();
}

void Application::Render()
{
	m_pModel->Render(m_pCamera);
}
