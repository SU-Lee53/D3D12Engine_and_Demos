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
}
