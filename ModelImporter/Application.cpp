#include "stdafx.h"
#include "Application.h"
#include "Importer.h"

using namespace std;

Application::Application()
{
}

Application::~Application()
{
}

void Application::Initialize()
{
	m_pImporter = make_unique<Importer>();
	m_pImporter->Initialize();

	m_pImporter->LoadFBXFile("../fbx/Gunship.fbx");
}

void Application::Update()
{
	if (ImGui::Begin("Node"))
	{
		m_pImporter->ShowFBXNodeToImGui();
	}
	ImGui::End();
}
