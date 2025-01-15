#include "stdafx.h"
#include "Importer.h"
using namespace std;

Importer::Importer()
{
}

Importer::~Importer()
{
}

BOOL Importer::Initialize()
{
	m_rpfbxManager = FbxManager::Create();
	
	m_rpfbxIOSettings = FbxIOSettings::Create(m_rpfbxManager, IOSROOT);
	m_rpfbxManager->SetIOSettings(m_rpfbxIOSettings);

	m_rpfbxImporter = FbxImporter::Create(m_rpfbxManager, "");

	m_rpfbxScene = FbxScene::Create(m_rpfbxManager, "");

	return TRUE;
}

void Importer::LoadFBXFile(std::string strFilename)
{
	m_strFilename = strFilename;
	m_rpfbxImporter->Initialize(strFilename.c_str(), -1, m_rpfbxManager->GetIOSettings());
	m_rpfbxImporter->Import(m_rpfbxScene);

	m_rpfbxRootNode = m_rpfbxScene->GetRootNode();
}

void Importer::ShowFBXNodeToImGui()
{
	if (m_rpfbxRootNode)
	{
		for (int i = 0; i < m_rpfbxRootNode->GetChildCount(); i++) ProcessNode(m_rpfbxRootNode->GetChild(i));
	}
}

void Importer::PrintTabs()
{
	for (UINT i = 0; i < m_tabs; i++) ImGui::Text("\t");
}

void Importer::ProcessNode(FbxNode* pFbxNode)
{
	PrintTabs();
	
	const char* pStrNodeName = pFbxNode->GetName();
	FbxDouble3 fbxvTranslation = pFbxNode->LclTranslation.Get();
	FbxDouble3 fbxvRotation = pFbxNode->LclRotation.Get();
	FbxDouble3 fbxvScaling = pFbxNode->LclScaling.Get();

	ImGui::Text(
		"< Node name = '%s' Translation = '(%f, %f, %f)' Rotation = '(%f, %f, %f)' Scaling = '(%f, %f, %f)' >\n",
		pStrNodeName,
		fbxvTranslation[0], fbxvTranslation[1], fbxvTranslation[2],
		fbxvRotation[0], fbxvRotation[1], fbxvRotation[2],
		fbxvScaling[0], fbxvScaling[1], fbxvScaling[2]
	);

	m_tabs++;

	for (int i = 0; i < pFbxNode->GetNodeAttributeCount(); i++)
	{
		PrintAttribute(pFbxNode->GetNodeAttributeByIndex(i));
	}

	for (int i = 0; i < pFbxNode->GetChildCount(); i++)
	{
		ProcessNode(pFbxNode->GetChild(i));
	}

	m_tabs--;
	PrintTabs();
	ImGui::Text("</Node>\n");

}

void Importer::PrintAttribute(FbxNodeAttribute* pfbxAttribute)
{
	FbxString strType = GetAttributeTypeName(pfbxAttribute->GetAttributeType());
	FbxString strName = pfbxAttribute->GetName();
	PrintTabs();
	ImGui::Text("< Attribute type = '%s' name = '%s'/>\n", strType.Buffer(), strName.Buffer());
}

FbxString Importer::GetAttributeTypeName(FbxNodeAttribute::EType fbxType)
{
	switch (fbxType)
	{
	case fbxsdk::FbxNodeAttribute::eUnknown:
		return("Unknown");

	case fbxsdk::FbxNodeAttribute::eNull:
		return("Null");

	case fbxsdk::FbxNodeAttribute::eMarker:
		return("Marker");

	case fbxsdk::FbxNodeAttribute::eSkeleton:
		return("Skeleton");

	case fbxsdk::FbxNodeAttribute::eMesh:
		return("Mesh");

	case fbxsdk::FbxNodeAttribute::eNurbs:
		return("Nurbs");

	case fbxsdk::FbxNodeAttribute::ePatch:
		return("Patch");

	case fbxsdk::FbxNodeAttribute::eCamera:
		return("Camera");

	case fbxsdk::FbxNodeAttribute::eCameraStereo:
		return("CameraStereo");

	case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
		return("CameraSwitcher");

	case fbxsdk::FbxNodeAttribute::eLight:
		return("Light");

	case fbxsdk::FbxNodeAttribute::eOpticalReference:
		return("OpticalReference");

	case fbxsdk::FbxNodeAttribute::eOpticalMarker:
		return("OpticalMarker");

	case fbxsdk::FbxNodeAttribute::eNurbsCurve:
		return("NurbsCurve");

	case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
		return("TrimNurbsSurface");

	case fbxsdk::FbxNodeAttribute::eBoundary:
		return("Boundary");

	case fbxsdk::FbxNodeAttribute::eNurbsSurface:
		return("NurbsSurface");

	case fbxsdk::FbxNodeAttribute::eShape:
		return("Shape");

	case fbxsdk::FbxNodeAttribute::eLODGroup:
		return("LODGroup");

	case fbxsdk::FbxNodeAttribute::eSubDiv:
		return("SubDiv");

	case fbxsdk::FbxNodeAttribute::eCachedEffect:
		return("CachedEffect");

	case fbxsdk::FbxNodeAttribute::eLine:
		return("Line");

	default:
		__debugbreak();
	}

	return ("Error");
}

void Importer::CleanUp()
{
	m_rpfbxRootNode->Destroy();
	m_rpfbxScene->Destroy();
	m_rpfbxImporter->Destroy();
	m_rpfbxIOSettings->Destroy();
	m_rpfbxManager->Destroy();

}
