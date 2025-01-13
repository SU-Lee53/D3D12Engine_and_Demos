#include "pch.h"
#include "Object.h"
#include "Transform.h"
#include "RenderMethod.h"
#include "input_types.h"
#include "Pipeline.h"
#include "Model.h"
#include "Mesh.h"
#include "MeshHelper.h"

using namespace std;

////////////////////////////////////
///		BasicForwardObject
////////////////////////////

BOOL BasicForwardObject::Initialize()
{
	BOOL bResult = FALSE;
	bResult = InitRenderMethod();

	vector<VertexType> vtx;
	vector<UINT> idx;

	MeshHelper::CreateBoxMesh(vtx, idx);
	m_pMesh = make_shared<Mesh<VertexType>>();
	bResult = m_pMesh->Initialize(vtx, idx);
	
	m_pTransform = make_shared<Transform>();
	m_pTransform->Initialize();
	m_pTransform->SetPosition(Vec3(0.f, 0.f, 5.f));

	return bResult;
}

void BasicForwardObject::Update()
{
	m_pTransform->Update();
}

void BasicForwardObject::Render()
{
	m_pRenderMethod->Render();
}

BOOL BasicForwardObject::InitRenderMethod()
{
	m_pRenderMethod = make_shared<ForwardRender>();
	m_pRenderMethod->Initialize(shared_from_this());

	return TRUE;
}
