#include "stdafx.h"
#include "Importer.h"

using namespace std;
using namespace std::literals;

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

	// Change to DirectX Axis System
	FbxAxisSystem fbxDirectXAxisSystem(FbxAxisSystem::eDirectX);
	if (m_rpfbxScene->GetGlobalSettings().GetAxisSystem() != fbxDirectXAxisSystem)
	{
		fbxDirectXAxisSystem.ConvertScene(m_rpfbxScene);
	}

}

void Importer::ShowFBXNodeToImGui()
{
	if (m_rpfbxRootNode)
	{
		for (int i = 0; i < m_rpfbxRootNode->GetChildCount(); i++) ProcessNode(m_rpfbxRootNode->GetChild(i), m_rpfbxRootNode->GetChild(i)->GetName());
	}

}

void Importer::PrintTabs()
{
	for (UINT i = 0; i < m_tabs; i++) ImGui::Text("\t");
}

void Importer::ProcessNode(FbxNode* pfbxNode, const char* nodeName)
{
	PrintTabs();
	
	const char* pStrNodeName = pfbxNode->GetName();
	FbxDouble3 fbxvTranslation = pfbxNode->LclTranslation.Get();
	FbxDouble3 fbxvRotation = pfbxNode->LclRotation.Get();
	FbxDouble3 fbxvScaling = pfbxNode->LclScaling.Get();

	ImGui::Text(
		"< Node name = '%s' Translation = '(%f, %f, %f)' Rotation = '(%f, %f, %f)' Scaling = '(%f, %f, %f)' >",
		pStrNodeName,
		fbxvTranslation[0], fbxvTranslation[1], fbxvTranslation[2],
		fbxvRotation[0], fbxvRotation[1], fbxvRotation[2],
		fbxvScaling[0], fbxvScaling[1], fbxvScaling[2]
	);

	m_tabs++;

	FbxMesh* pfbxMesh = pfbxNode->GetMesh();

	PrintMeshInfo(pfbxMesh, nodeName);
	PrintLayerInfo(pfbxMesh, nodeName);
	PrintSurfaceMaterialInfo(pfbxNode, nodeName);

	for (int i = 0; i < pfbxNode->GetNodeAttributeCount(); i++)
	{
		PrintAttribute(pfbxNode->GetNodeAttributeByIndex(i));
	}

	for (int i = 0; i < pfbxNode->GetChildCount(); i++)
	{
		ProcessNode(pfbxNode->GetChild(i), pfbxNode->GetChild(i)->GetName());
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

void Importer::PrintLayerInfo(FbxMesh* pfbxMesh, const char* nodeName)
{
	int layerCount = pfbxMesh->GetLayerCount();
	ImGui::Text("< Layer Count : %d >", layerCount);

	for (int i = 0; i < layerCount; i++)
	{
		string treeNodeName = "Layer Info : " + to_string(i) + " | Name : " + string(nodeName);
		if (ImGui::TreeNode(treeNodeName.c_str()))
		{
			FbxLayer* pfbxLayer = pfbxMesh->GetLayer(i);

			const FbxLayerElementPolygonGroup* polygons = pfbxLayer->GetPolygonGroups();
			if (polygons != nullptr) ImGui::Text("layer has polygons");

			const FbxLayerElementTemplate<FbxVector4>* normal = pfbxLayer->GetNormals();
			if (normal != nullptr) ImGui::Text("layer has normal");

			const FbxLayerElementTemplate<FbxVector4>* biNormal = pfbxLayer->GetBinormals();
			if (biNormal != nullptr) ImGui::Text("layer has binormal");

			const FbxLayerElementTemplate<FbxVector4>* tangent = pfbxLayer->GetTangents();
			if (tangent != nullptr) ImGui::Text("layer has tangent");

			FbxArray<const FbxLayerElementUV*> uvSet = pfbxLayer->GetUVSets();
			if (uvSet.GetCount() > 0) ImGui::Text("layer has uvs");

			const FbxLayerElementTemplate<FbxSurfaceMaterial*>* materials = pfbxLayer->GetMaterials();
			if (materials)
			{
				ImGui::Text("layer has materialElement");

				if (ImGui::TreeNode(string("Element Info : "s + nodeName).c_str()))
				{
					FbxLayerElementArrayTemplate<FbxSurfaceMaterial*> matArr = materials->GetDirectArray();
					int arrCount = matArr.GetCount();
					ImGui::Text("Material Array Count : %d", arrCount);

					for (int j = 0; j < arrCount; j++)
					{
						ImGui::Text("\tShadingModel : %s", matArr[j]->sShadingModel);
						ImGui::Text("\tMultiLayer : %s", matArr[j]->sMultiLayer);
						ImGui::Text("\tEmissive : %s", matArr[j]->sEmissive);
						ImGui::Text("\tEmissiveFactor : %s", matArr[j]->sEmissiveFactor);
						ImGui::Text("\tDiffuse : %s", matArr[j]->sDiffuse);
						ImGui::Text("\tDiffuseFactor : %s", matArr[j]->sDiffuseFactor);
						ImGui::Text("\tSpecular : %s", matArr[j]->sSpecular);
						ImGui::Text("\tSpecularFactor : %s", matArr[j]->sSpecularFactor);
						ImGui::Text("\tShininess : %s", matArr[j]->sShininess);
						ImGui::Text("\tBump : %s", matArr[j]->sBump);
						ImGui::Text("\tBumpFactor : %s", matArr[j]->sBumpFactor);
						ImGui::Text("\tNormalMap : %s", matArr[j]->sNormalMap);
						ImGui::Text("\tTransparentColor : %s", matArr[j]->sTransparentColor);
						ImGui::Text("\tTransparencyFactor : %s", matArr[j]->sTransparencyFactor);
						ImGui::Text("\tsReflection : %s", matArr[j]->sReflection);
						ImGui::Text("\tsReflectionFactor : %s", matArr[j]->sReflectionFactor);
						ImGui::Text("\tsDisplacementColor : %s", matArr[j]->sDisplacementColor);
						ImGui::Text("\tsDisplacementFactor : %s", matArr[j]->sDisplacementFactor);
						ImGui::Text("\tsVectorDisplacementColor : %s", matArr[j]->sVectorDisplacementColor);
						ImGui::Text("\tsVectorDisplacementFactor : %s", matArr[j]->sVectorDisplacementFactor);

					}


					ImGui::TreePop();
				}

			}
			const FbxLayerElementTemplate<FbxColor>* vertexColor = pfbxLayer->GetVertexColors();
			if (vertexColor != nullptr) ImGui::Text("layer has vertexColors");

			ImGui::TreePop();
		}
	}
	
}

void Importer::PrintMeshInfo(FbxMesh* pfbxMesh, const char* nodeName)
{
	string meshTreeNodeName = "Mesh Info | Name : " + string(nodeName);
	if (ImGui::TreeNode(meshTreeNodeName.c_str()))
	{
		int polygonCount = pfbxMesh->GetPolygonCount();
		ImGui::Text("Polygon Count : %d", polygonCount);

		std::string polygonTreeName = "polygon vertices"s + " | Name : " + string(nodeName);

		if (ImGui::TreeNode(polygonTreeName.c_str()))
		{
			for (int i = 0; i < polygonCount; i++)
			{
				ImGui::Text("polygon index : %d", i);
				int polygonSize = pfbxMesh->GetPolygonSize(i);
				ImGui::Text("polygon size : %d", polygonSize);

				ImGui::NewLine();
				for (int j = 0; j < polygonSize; j++)
				{
					int vtx = pfbxMesh->GetPolygonVertex(i, j);
					ImGui::SameLine();
					ImGui::Text("%d", vtx);
				}
			}

			ImGui::TreePop();
		}

		int controlPointCount = pfbxMesh->GetControlPointsCount();
		ImGui::Text("Vertex Count : %d", controlPointCount);

		string vertexTreeName = "Vertex(Control Point)"s + " | Name : " + string(nodeName);
		if (ImGui::TreeNode(vertexTreeName.c_str()))
		{
			for (int i = 0; i < controlPointCount; i++)
			{
				FbxVector4 pfbxControlPoints = pfbxMesh->GetControlPointAt(i);
				ImGui::Text("%d : (%f, %f, %f, %f)", i, pfbxControlPoints[0], pfbxControlPoints[1], pfbxControlPoints[2], pfbxControlPoints[3]);
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

}

void Importer::PrintSurfaceMaterialInfo(FbxNode* pfbxNode, const char* nodeName)
{
}

void Importer::ExportModelInSceneToModel(std::shared_ptr<Model>& pOutModel)
{
}

void Importer::CleanUp()
{
	m_rpfbxRootNode->Destroy();
	m_rpfbxScene->Destroy();
	m_rpfbxImporter->Destroy();
	m_rpfbxIOSettings->Destroy();
	m_rpfbxManager->Destroy();

}
