#include "pch.h"
#include "HierarchyModelDemo.h"
#include "HierarchyModelResource.h"
#include "Object.h"
#include "Camera.h"

using namespace std;

void HierarchyModelDemo::Initialize()
{
	m_pObj = make_shared<HierarchyModelObject>();
	m_pObj->Initialize();

	shared_ptr<HierarchyModelObject> originObj = static_pointer_cast<HierarchyModelObject>(m_pObj);
	originObj->LoadFromBinaryFile(L"../Models/Binaries/Hummer.bin");
	originObj->GetTransform()->SetWorldScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
}

void HierarchyModelDemo::Update()
{
	m_pObj->Update();

}

void HierarchyModelDemo::Render()
{
	RENDER.Add(m_pObj);
}
