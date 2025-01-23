#include "stdafx.h"
#include "Application.h"
#include "Importer.h"
#include "Model.h"

using namespace std;

Application::Application()
{
}

Application::~Application()
{
}

void Application::Initialize()
{
	m_pImporter = make_unique<FbxLoader>();
	m_pImporter->Initialize();

	m_pImporter->LoadFBXFile("../fbx/Gunship.fbx");

	model = make_shared<Model>();
	m_pImporter->ExportModelInSceneToModel(model);

}

void Application::Update()
{
	if (ImGui::Begin("Node"))
	{
		m_pImporter->ShowFBXNodeToImGui();
	}
	ImGui::End();
}
