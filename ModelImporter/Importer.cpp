#include "stdafx.h"
#include "Importer.h"
#include "Model.h"
#include <filesystem>

using namespace std;
using namespace std::literals;

FbxLoader::FbxLoader()
{
}

FbxLoader::~FbxLoader()
{
}

BOOL FbxLoader::Initialize()
{
	m_rpfbxManager = FbxManager::Create();
	
	m_rpfbxIOSettings = FbxIOSettings::Create(m_rpfbxManager, IOSROOT);
	m_rpfbxManager->SetIOSettings(m_rpfbxIOSettings);

	m_rpfbxImporter = FbxImporter::Create(m_rpfbxManager, "");

	m_rpfbxScene = FbxScene::Create(m_rpfbxManager, "");


	return TRUE;
}

void FbxLoader::LoadFBXFile(std::string strFilename)
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

void FbxLoader::ShowFBXNodeToImGui()
{
	if (m_rpfbxRootNode)
	{
		for (int i = 0; i < m_rpfbxRootNode->GetChildCount(); i++) ProcessNode(m_rpfbxRootNode->GetChild(i), m_rpfbxRootNode->GetChild(i)->GetName());
	}

}

void FbxLoader::PrintTabs()
{
	for (UINT i = 0; i < m_tabs; i++)
	{
		ImGui::Text("\t");
		ImGui::SameLine();
	}
}

void FbxLoader::ProcessNode(FbxNode* pfbxNode, const char* cstrNodeName)
{
	FbxDouble3 fbxvTranslation = pfbxNode->LclTranslation.Get();
	FbxDouble3 fbxvRotation = pfbxNode->LclRotation.Get();
	FbxDouble3 fbxvScaling = pfbxNode->LclScaling.Get();

	ImGui::NewLine();
	for (int i = 0; i < m_tabs; i++)
	{
		ImGui::Text("\t"); ImGui::SameLine();
	}

	ImGui::Text(
		"< Node name = '%s' Translation = '(%f, %f, %f)' Rotation = '(%f, %f, %f)' Scaling = '(%f, %f, %f)' >",
		cstrNodeName,
		fbxvTranslation[0], fbxvTranslation[1], fbxvTranslation[2],
		fbxvRotation[0], fbxvRotation[1], fbxvRotation[2],
		fbxvScaling[0], fbxvScaling[1], fbxvScaling[2]
	);

	m_tabs++;

	FbxMesh* pfbxMesh = pfbxNode->GetMesh();

	if (pfbxMesh)
	{
		PrintMeshInfo(pfbxMesh, cstrNodeName);
		PrintLayerInfo(pfbxMesh, cstrNodeName);
	}
	PrintSurfaceMaterialInfo(pfbxNode, cstrNodeName);

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

void FbxLoader::PrintAttribute(FbxNodeAttribute* pfbxAttribute)
{
	FbxString strType = GetAttributeTypeName(pfbxAttribute->GetAttributeType());
	FbxString strName = pfbxAttribute->GetName();
	PrintTabs();
	ImGui::Text("< Attribute type = '%s' name = '%s'/>\n", strType.Buffer(), strName.Buffer());
}

FbxString FbxLoader::GetAttributeTypeName(FbxNodeAttribute::EType fbxType)
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
		return ("Error");
	}

	return ("Error");
}

string FbxLoader::GetFbxMappingNodeName(FbxGeometryElement::EMappingMode mappingMode)
{
	switch (mappingMode)
	{
	case fbxsdk::FbxLayerElement::eNone:
		return ("eNone");
	case fbxsdk::FbxLayerElement::eByControlPoint:
		return ("eByControlPoint");
	case fbxsdk::FbxLayerElement::eByPolygonVertex:
		return ("eByPolygonVertex");
	case fbxsdk::FbxLayerElement::eByPolygon:
		return ("eByPolygon");
	case fbxsdk::FbxLayerElement::eByEdge:
		return ("eByEdge");
	case fbxsdk::FbxLayerElement::eAllSame:
		return ("eAllSame");
	default:
		__debugbreak();
		return ("Error");
	}

	return ("Error");
}

string FbxLoader::GetFbxReferenceNodeName(FbxGeometryElement::EReferenceMode referenceMode)
{
	switch (referenceMode)
	{
	case fbxsdk::FbxLayerElement::eDirect:
		return ("eDirect");
	case fbxsdk::FbxLayerElement::eIndex:
		return ("eIndex");
	case fbxsdk::FbxLayerElement::eIndexToDirect:
		return ("eIndexToDirect");
	default:
		__debugbreak();
		return ("Error");
	}

	return ("Error");
}

void FbxLoader::PrintLayerInfo(FbxMesh* pfbxMesh, const char* cstrNodeName)
{
	int layerCount = pfbxMesh->GetLayerCount();
	//ImGui::Text("< Layer Count : %d >", layerCount);

	for (int i = 0; i < layerCount; i++)
	{
		string treecstrNodeName = "Layer Info : " + to_string(i) + " | Name : " + string(cstrNodeName);
		if (ImGui::TreeNode(treecstrNodeName.c_str()))
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

			const FbxLayerElementTemplate<FbxSurfaceMaterial*>* materials = pfbxLayer->GetMaterials();
			if (materials)
			{
				ImGui::Text("layer has materialElement");
			}
			const FbxLayerElementTemplate<FbxColor>* vertexColor = pfbxLayer->GetVertexColors();
			if (vertexColor != nullptr) ImGui::Text("layer has vertexColors");

			ImGui::TreePop();
		}
	}
	
}

void FbxLoader::PrintMeshInfo(FbxMesh* pfbxMesh, const char* cstrNodeName)
{
	string meshTreecstrNodeName = "Mesh Info | Name : " + string(cstrNodeName);
	if (ImGui::TreeNode(meshTreecstrNodeName.c_str()))
	{
		// Polygons
		// Some models has 4 vertices per polygons
		// But, D3D supports triangle primitive only
		// So, We need to Triangulate into 3 Vertices when Export
		int nPolygons = pfbxMesh->GetPolygonCount();

		std::string strPolygonTreeName = "polygon vertices"s + " | Name : " + string(cstrNodeName) + " | Count : "s + to_string(nPolygons);

		if (ImGui::TreeNode(strPolygonTreeName.c_str()))
		{
			for (int i = 0; i < nPolygons; i++)
			{
				ImGui::NewLine();
				ImGui::Text("polygon index : %d", i);
				int nPolygonSize = pfbxMesh->GetPolygonSize(i);
				ImGui::Text("polygon size : %d", nPolygonSize);

				for (int j = 0; j < nPolygonSize; j++)
				{
					int vtxIdx = pfbxMesh->GetPolygonVertex(i, j);
					FbxVector4 vtx = pfbxMesh->GetControlPointAt(vtxIdx);
					ImGui::Text("%d : (%f, %f, %f, %f)", vtxIdx, vtx[0], vtx[1], vtx[2], vtx[3]);
				}

				// if Polygon Size is bigger then 4, Do Triangulate
				if (nPolygonSize > 3)
				{
					string strTriangulateNodeName = "Triangulate #"s + to_string(i);
					if (ImGui::TreeNode(strTriangulateNodeName.c_str()))
					{
						for (int j = 1; j < nPolygonSize - 1; j++)
						{
							ImGui::Text("(%d, %d, %d)", pfbxMesh->GetPolygonVertex(i, 0), pfbxMesh->GetPolygonVertex(i, j), pfbxMesh->GetPolygonVertex(i, j + 1));
							ImGui::SameLine();
						}
						ImGui::NewLine();

						ImGui::TreePop();
					}
				}
			}

			ImGui::TreePop();
		}

		// Vertices
		int nControlPoints = pfbxMesh->GetControlPointsCount();

		string strVertexTreeName = "Vertex(Control Point) | Name : "s + string(cstrNodeName) + " | Count : "s + to_string(nControlPoints);
		if (ImGui::TreeNode(strVertexTreeName.c_str()))
		{
			ImGui::Text("Vertices");
			for (int i = 0; i < nControlPoints; i++)
			{
				FbxVector4 fbxControlPoints = pfbxMesh->GetControlPointAt(i);
				ImGui::Text("%d : (%f, %f, %f, %f)", i, fbxControlPoints[0], fbxControlPoints[1], fbxControlPoints[2], fbxControlPoints[3]);

				int nUVElements = pfbxMesh->GetElementUVCount();
				int nNormalElements = pfbxMesh->GetElementNormalCount();
				int nBinormalElements = pfbxMesh->GetElementBinormalCount();
				int nTangentElements = pfbxMesh->GetElementTangentCount();

				FbxGeometryElementUV* uvElement = nullptr;
				FbxGeometryElementNormal* normalElement = nullptr;
				FbxGeometryElementBinormal* binormalElement = nullptr;
				FbxGeometryElementTangent* tangentElement = nullptr;

				if (nUVElements > 0)
					uvElement = pfbxMesh->GetElementUV();

				if (nNormalElements > 0)
					normalElement = pfbxMesh->GetElementNormal();

				if (nBinormalElements > 0)
					binormalElement = pfbxMesh->GetElementBinormal();

				if (nTangentElements > 0)
					tangentElement = pfbxMesh->GetElementTangent();

				if (uvElement)
				{
					ImGui::SameLine();
					FbxGeometryElement::EMappingMode mappingMode = uvElement->GetMappingMode();
					FbxGeometryElement::EReferenceMode referenceMode = uvElement->GetReferenceMode();

					string strMappingMode = GetFbxMappingNodeName(mappingMode);
					string strReferenceMode = GetFbxReferenceNodeName(referenceMode);

					if (mappingMode != FbxGeometryElement::eByPolygonVertex)
					{
						ImGui::Text("%s : Unsupported mapping mode", strMappingMode.c_str());
						continue;	// if mappingMode is not eByPolygonVertex, Do not print UV data
					}

					if (referenceMode == FbxGeometryElement::eIndex)
					{
						ImGui::Text("%s : Unsupported reference mode", strReferenceMode.c_str());
						continue; // if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
					}

					FbxVector2 uv;

					switch (referenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					{
						uv = uvElement->GetDirectArray().GetAt(i);
						break;
					}
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						int uvIndex = uvElement->GetIndexArray().GetAt(i);
						uv = uvElement->GetDirectArray().GetAt(uvIndex);
						break;
					}
					case fbxsdk::FbxLayerElement::eIndex:
						__debugbreak();
						break;
					default:
						__debugbreak();
						break;
					}

					ImGui::Text("\t| UV : (%f, %f)", uv[0], uv[1]);
				}

				if (normalElement)
				{
					ImGui::SameLine();

					FbxGeometryElement::EMappingMode mappingMode = normalElement->GetMappingMode();
					FbxGeometryElement::EReferenceMode referenceMode = normalElement->GetReferenceMode();

					string strMappingMode = GetFbxMappingNodeName(mappingMode);
					string strReferenceMode = GetFbxReferenceNodeName(referenceMode);

					if (mappingMode != FbxGeometryElement::eByPolygonVertex)
					{
						ImGui::Text("%s : Unsupported mapping mode", strMappingMode.c_str());
						continue;	// if mappingMode is not eByPolygonVertex, Do not print UV data
					}

					if (referenceMode == FbxGeometryElement::eIndex)
					{
						ImGui::Text("%s : Unsupported reference mode", strReferenceMode.c_str());
						continue; // if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
					}

					FbxVector4 normal;

					switch (referenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					{
						normal = normalElement->GetDirectArray().GetAt(i);
						break;
					}
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						int normalIndex = normalElement->GetIndexArray().GetAt(i);
						normal = normalElement->GetDirectArray().GetAt(normalIndex);
						break;
					}
					case fbxsdk::FbxLayerElement::eIndex:
						__debugbreak();
						break;
					default:
						__debugbreak();
						break;
					}

					ImGui::Text("\t| Normal : (%f, %f, %f, %f)", normal[0], normal[1], normal[2], normal[3]);
				}

				if (binormalElement)
				{
					ImGui::SameLine();

					FbxGeometryElement::EMappingMode mappingMode = binormalElement->GetMappingMode();
					FbxGeometryElement::EReferenceMode referenceMode = binormalElement->GetReferenceMode();

					string strMappingMode = GetFbxMappingNodeName(mappingMode);
					string strReferenceMode = GetFbxReferenceNodeName(referenceMode);

					if (mappingMode != FbxGeometryElement::eByPolygonVertex)
					{
						ImGui::Text("%s : Unsupported mapping mode", strMappingMode.c_str());
						continue;	// if mappingMode is not eByPolygonVertex, Do not print UV data
					}

					if (referenceMode == FbxGeometryElement::eIndex)
					{
						ImGui::Text("%s : Unsupported reference mode", strReferenceMode.c_str());
						continue; // if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
					}

					FbxVector4 binormal;

					switch (referenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					{
						binormal = binormalElement->GetDirectArray().GetAt(i);
						break;
					}
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						int binormalIndex = binormalElement->GetIndexArray().GetAt(i);
						binormal = binormalElement->GetDirectArray().GetAt(binormalIndex);
						break;
					}
					case fbxsdk::FbxLayerElement::eIndex:
						__debugbreak();
						break;
					default:
						__debugbreak();
						break;
					}

					ImGui::Text("\t| Binormal : (%f, %f, %f, %f)", binormal[0], binormal[1], binormal[2], binormal[3]);
				}
				
				if (tangentElement)
				{
					ImGui::SameLine();

					FbxGeometryElement::EMappingMode mappingMode = tangentElement->GetMappingMode();
					FbxGeometryElement::EReferenceMode referenceMode = tangentElement->GetReferenceMode();

					string strMappingMode = GetFbxMappingNodeName(mappingMode);
					string strReferenceMode = GetFbxReferenceNodeName(referenceMode);

					if (mappingMode != FbxGeometryElement::eByPolygonVertex)
					{
						ImGui::Text("%s : Unsupported mapping mode", strMappingMode.c_str());
						continue;	// if mappingMode is not eByPolygonVertex, Do not print UV data
					}

					if (referenceMode == FbxGeometryElement::eIndex)
					{
						ImGui::Text("%s : Unsupported reference mode", strReferenceMode.c_str());
						continue; // if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
					}

					FbxVector4 tangent;

					switch (referenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					{
						tangent = tangentElement->GetDirectArray().GetAt(i);
						break;
					}
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						int tangentIndex = tangentElement->GetIndexArray().GetAt(i);
						tangent = tangentElement->GetDirectArray().GetAt(tangentIndex);
						break;
					}
					case fbxsdk::FbxLayerElement::eIndex:
						__debugbreak();
						break;
					default:
						__debugbreak();
						break;
					}

					ImGui::Text("\t| Tangent : (%f, %f, %f, %f)", tangent[0], tangent[1], tangent[2], tangent[3]);
				}

			}

			ImGui::TreePop();
		}

		ImGui::Text("<Additional Datas from FbxMesh>");
		ImGui::NewLine();

		// UVs
		int nUVElements = pfbxMesh->GetElementUVCount();
		ImGui::Text("UV Element Count : %d", nUVElements);
		if (nUVElements > 0)	// if UV is valid
		{
			for (int i = 0; i < nUVElements; i++)
			{
				FbxGeometryElementUV* uvElement = pfbxMesh->GetElementUV(0);

				FbxGeometryElement::EMappingMode mappingMode = uvElement->GetMappingMode();
				FbxGeometryElement::EReferenceMode referenceMode = uvElement->GetReferenceMode();

				string strMappingMode = GetFbxMappingNodeName(mappingMode);
				string strReferenceMode = GetFbxReferenceNodeName(referenceMode);

				if (mappingMode != FbxGeometryElement::eByPolygonVertex)
				{
					ImGui::Text("%s : Unsupported mapping mode", strMappingMode.c_str());
					continue;	// if mappingMode is not eByPolygonVertex, Do not print UV data
				}

				if (referenceMode == FbxGeometryElement::eIndex)
				{
					ImGui::Text("%s : Unsupported reference mode", strReferenceMode.c_str());
					continue; // if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
				}

				string strUVTreeName = "UV | Name : "s + string(cstrNodeName) + " #"s + to_string(i);
				if (ImGui::TreeNode(strUVTreeName.c_str()))
				{
					ImGui::Text("Mapping Mode : %s\nReference Mode : %s", strMappingMode.c_str(), strReferenceMode.c_str());
					for (int polygonIdx = 0; polygonIdx < nPolygons; polygonIdx++)
					{
						int polygonSize = pfbxMesh->GetPolygonSize(polygonIdx);
						for (int vertexIdx = 0; vertexIdx < polygonSize; vertexIdx++)
						{
							int controlPointIndex = pfbxMesh->GetPolygonVertex(polygonIdx, vertexIdx);
							FbxVector2 uv;

							switch (referenceMode)
							{
							case fbxsdk::FbxLayerElement::eDirect:
							{
								uv = uvElement->GetDirectArray().GetAt(controlPointIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndexToDirect:
							{
								int uvIndex = uvElement->GetIndexArray().GetAt(controlPointIndex);
								uv = uvElement->GetDirectArray().GetAt(uvIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndex:
								__debugbreak();
								break;
							default:
								__debugbreak();
								break;
							}

							FbxVector4 fbxControlPoints = pfbxMesh->GetControlPointAt(controlPointIndex);
							ImGui::Text("Polygon : %d\t", polygonIdx); ImGui::SameLine();
							ImGui::Text("Vertex : %d (#%d : {%f, %f, %f})\t", vertexIdx, controlPointIndex, fbxControlPoints[0], fbxControlPoints[1], fbxControlPoints[2]); ImGui::SameLine();
							ImGui::Text("UV : (%f, %f)", uv[0], uv[1]);
						}
						ImGui::NewLine();
					}
					ImGui::TreePop();
				}
			}

		}

		// Normals
		int nNormalElements = pfbxMesh->GetElementNormalCount();
		ImGui::Text("Normal Element Count : %d", nNormalElements);
		if (nNormalElements > 0)
		{
			for (int i = 0; i < nNormalElements; i++)
			{
				FbxGeometryElementNormal* pNormalElement = pfbxMesh->GetElementNormal(i);
				int nNormals = pNormalElement->GetDirectArray().GetCount();

				FbxGeometryElement::EMappingMode mappingMode = pNormalElement->GetMappingMode();
				FbxGeometryElement::EReferenceMode referenceMode = pNormalElement->GetReferenceMode();

				string strMappingMode = GetFbxMappingNodeName(mappingMode);
				string strReferenceMode = GetFbxReferenceNodeName(referenceMode);

				if (mappingMode != FbxGeometryElement::eByPolygonVertex)
				{
					ImGui::Text("%s : Unsupported mapping mode", strMappingMode.c_str());
					continue;	// if mappingMode is not eByPolygonVertex, Do not print UV data
				}

				if (referenceMode == FbxGeometryElement::eIndex)
				{
					ImGui::Text("%s : Unsupported reference mode", strReferenceMode.c_str());
					continue; // if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
				}

				string strNormalTreeName = "Normal | Name : "s + string(cstrNodeName) + " #"s + to_string(i) + " | Count : "s + to_string(nNormals);
				if (ImGui::TreeNode(strNormalTreeName.c_str()))
				{
					ImGui::Text("Mapping Mode : %s\nReference Mode : %s", strMappingMode.c_str(), strReferenceMode.c_str());
					for (int polygonIdx = 0; polygonIdx < nPolygons; polygonIdx++)
					{
						int polygonSize = pfbxMesh->GetPolygonSize(polygonIdx);
						for (int vertexIdx = 0; vertexIdx < polygonSize; vertexIdx++)
						{
							int controlPointIndex = pfbxMesh->GetPolygonVertex(polygonIdx, vertexIdx);
							FbxVector4 normal;

							switch (referenceMode)
							{
							case fbxsdk::FbxLayerElement::eDirect:
							{
								normal = pNormalElement->GetDirectArray().GetAt(controlPointIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndexToDirect:
							{
								int uvIndex = pNormalElement->GetIndexArray().GetAt(controlPointIndex);
								normal = pNormalElement->GetDirectArray().GetAt(uvIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndex:
								__debugbreak();
								break;
							default:
								__debugbreak();
								break;
							}

							FbxVector4 fbxControlPoints = pfbxMesh->GetControlPointAt(controlPointIndex);
							ImGui::Text("Polygon : %d\t", polygonIdx); ImGui::SameLine();
							ImGui::Text("Vertex : %d (#%d : {%f, %f, %f})\t", vertexIdx, controlPointIndex, fbxControlPoints[0], fbxControlPoints[1], fbxControlPoints[2]); ImGui::SameLine();
							ImGui::Text("Normal : (%f, %f, %f, %f)", normal[0], normal[1], normal[2], normal[3]);
						}
						ImGui::NewLine();
					}
					ImGui::TreePop();
				}
			}
		}

		// Binormals
		int nBinormalElements = pfbxMesh->GetElementBinormalCount();
		ImGui::Text("Binormals Count : %d", nBinormalElements);
		if (nBinormalElements)
		{
			for (int i = 0; i < nBinormalElements; i++)
			{
				FbxGeometryElementBinormal* pBinormalElement = pfbxMesh->GetElementBinormal(i);
				int nBinormals = pBinormalElement->GetDirectArray().GetCount();

				FbxGeometryElement::EMappingMode mappingMode = pBinormalElement->GetMappingMode();
				FbxGeometryElement::EReferenceMode referenceMode = pBinormalElement->GetReferenceMode();

				string strMappingMode = GetFbxMappingNodeName(mappingMode);
				string strReferenceMode = GetFbxReferenceNodeName(referenceMode);

				if (mappingMode != FbxGeometryElement::eByPolygonVertex)
				{
					ImGui::Text("%s : Unsupported mapping mode", strMappingMode.c_str());
					continue;	// if mappingMode is not eByPolygonVertex, Do not print UV data
				}

				if (referenceMode == FbxGeometryElement::eIndex)
				{
					ImGui::Text("%s : Unsupported reference mode", strReferenceMode.c_str());
					continue; // if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
				}

				string strBinormalTreeName = "Binormal | Name : "s + string(cstrNodeName) + " #"s + to_string(i) + " | Count : "s + to_string(nBinormals);
				if (ImGui::TreeNode(strBinormalTreeName.c_str()))
				{
					ImGui::Text("Mapping Mode : %s\nReference Mode : %s", strMappingMode.c_str(), strReferenceMode.c_str());
					for (int polygonIdx = 0; polygonIdx < nPolygons; polygonIdx++)
					{
						int polygonSize = pfbxMesh->GetPolygonSize(polygonIdx);
						for (int vertexIdx = 0; vertexIdx < polygonSize; vertexIdx++)
						{
							int controlPointIndex = pfbxMesh->GetPolygonVertex(polygonIdx, vertexIdx);
							FbxVector4 Binormal;

							switch (referenceMode)
							{
							case fbxsdk::FbxLayerElement::eDirect:
							{
								Binormal = pBinormalElement->GetDirectArray().GetAt(controlPointIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndexToDirect:
							{
								int uvIndex = pBinormalElement->GetIndexArray().GetAt(controlPointIndex);
								Binormal = pBinormalElement->GetDirectArray().GetAt(uvIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndex:
								__debugbreak();
								break;
							default:
								__debugbreak();
								break;
							}

							FbxVector4 fbxControlPoints = pfbxMesh->GetControlPointAt(controlPointIndex);
							ImGui::Text("Polygon : %d\t", polygonIdx); ImGui::SameLine();
							ImGui::Text("Vertex : %d (#%d : {%f, %f, %f})\t", vertexIdx, controlPointIndex, fbxControlPoints[0], fbxControlPoints[1], fbxControlPoints[2]); ImGui::SameLine();
							ImGui::Text("Binormal : (%f, %f, %f, %f)", Binormal[0], Binormal[1], Binormal[2], Binormal[3]);
						}
						ImGui::NewLine();
					}
					ImGui::TreePop();
				}
			}
		}

		// Tangents
		int nTangentElements = pfbxMesh->GetElementTangentCount();
		ImGui::Text("Tangents Count : %d", nTangentElements);
		if (nTangentElements > 0)
		{
			for (int i = 0; i < nTangentElements; i++)
			{
				FbxGeometryElementTangent* pTangentElement = pfbxMesh->GetElementTangent(i);
				int nTangents = pTangentElement->GetDirectArray().GetCount();

				FbxGeometryElement::EMappingMode mappingMode = pTangentElement->GetMappingMode();
				FbxGeometryElement::EReferenceMode referenceMode = pTangentElement->GetReferenceMode();

				string strMappingMode = GetFbxMappingNodeName(mappingMode);
				string strReferenceMode = GetFbxReferenceNodeName(referenceMode);

				if (mappingMode != FbxGeometryElement::eByPolygonVertex)
				{
					ImGui::Text("%s : Unsupported mapping mode", strMappingMode.c_str());
					continue;	// if mappingMode is not eByPolygonVertex, Do not print UV data
				}

				if (referenceMode == FbxGeometryElement::eIndex)
				{
					ImGui::Text("%s : Unsupported reference mode", strReferenceMode.c_str());
					continue; // if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
				}

				string strTangentTreeName = "Tangent | Name : "s + string(cstrNodeName) + " #"s + to_string(i) + " | Count : "s + to_string(nTangents);
				if (ImGui::TreeNode(strTangentTreeName.c_str()))
				{
					ImGui::Text("Mapping Mode : %s\nReference Mode : %s", strMappingMode.c_str(), strReferenceMode.c_str());
					for (int polygonIdx = 0; polygonIdx < nPolygons; polygonIdx++)
					{
						int polygonSize = pfbxMesh->GetPolygonSize(polygonIdx);
						for (int vertexIdx = 0; vertexIdx < polygonSize; vertexIdx++)
						{
							int controlPointIndex = pfbxMesh->GetPolygonVertex(polygonIdx, vertexIdx);
							FbxVector4 Tangent;

							switch (referenceMode)
							{
							case fbxsdk::FbxLayerElement::eDirect:
							{
								Tangent = pTangentElement->GetDirectArray().GetAt(controlPointIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndexToDirect:
							{
								int uvIndex = pTangentElement->GetIndexArray().GetAt(controlPointIndex);
								Tangent = pTangentElement->GetDirectArray().GetAt(uvIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndex:
								__debugbreak();
								break;
							default:
								__debugbreak();
								break;
							}

							FbxVector4 fbxControlPoints = pfbxMesh->GetControlPointAt(controlPointIndex);
							ImGui::Text("Polygon : %d\t", polygonIdx); ImGui::SameLine();
							ImGui::Text("Vertex : %d (#%d : {%f, %f, %f})\t", vertexIdx, controlPointIndex, fbxControlPoints[0], fbxControlPoints[1], fbxControlPoints[2]); ImGui::SameLine();
							ImGui::Text("Tangent : (%f, %f, %f, %f)", Tangent[0], Tangent[1], Tangent[2], Tangent[3]);
						}
						ImGui::NewLine();
					}
					ImGui::TreePop();
				}
			}
		}

		// Vertex Color
		int nVertexColorElements = pfbxMesh->GetElementVertexColorCount();
		ImGui::Text("Vertex Color Count : %d", nVertexColorElements);
		if (nVertexColorElements > 0)
		{
			for (int i = 0; i < nVertexColorElements; i++)
			{
				FbxGeometryElementVertexColor* pVtxColorElement = pfbxMesh->GetElementVertexColor(i);
				int nVertexColors = pVtxColorElement->GetDirectArray().GetCount();

				FbxGeometryElement::EMappingMode mappingMode = pVtxColorElement->GetMappingMode();
				FbxGeometryElement::EReferenceMode referenceMode = pVtxColorElement->GetReferenceMode();

				string strMappingMode = GetFbxMappingNodeName(mappingMode);
				string strReferenceMode = GetFbxReferenceNodeName(referenceMode);

				if (mappingMode != FbxGeometryElement::eByPolygonVertex)
				{
					ImGui::Text("%s : Unsupported mapping mode", strMappingMode.c_str());
					continue;	// if mappingMode is not eByPolygonVertex, Do not print UV data
				}

				if (referenceMode == FbxGeometryElement::eIndex)
				{
					ImGui::Text("%s : Unsupported reference mode", strReferenceMode.c_str());
					continue; // if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
				}

				string strVtxColorTreeName = "VtxColor | Name : "s + string(cstrNodeName) + " #"s + to_string(i);
				if (ImGui::TreeNode(strVtxColorTreeName.c_str()))
				{
					ImGui::Text("Mapping Mode : %s\nReference Mode : %s", strMappingMode.c_str(), strReferenceMode.c_str());
					for (int polygonIdx = 0; polygonIdx < nPolygons; polygonIdx++)
					{
						int polygonSize = pfbxMesh->GetPolygonSize(polygonIdx);
						for (int vertexIdx = 0; vertexIdx < polygonSize; vertexIdx++)
						{
							int controlPointIndex = pfbxMesh->GetPolygonVertex(polygonIdx, vertexIdx);
							FbxColor VtxColor;

							switch (referenceMode)
							{
							case fbxsdk::FbxLayerElement::eDirect:
							{
								VtxColor = pVtxColorElement->GetDirectArray().GetAt(controlPointIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndexToDirect:
							{
								int VtxColorIndex = pVtxColorElement->GetIndexArray().GetAt(controlPointIndex);
								VtxColor = pVtxColorElement->GetDirectArray().GetAt(VtxColorIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndex:
								__debugbreak();
								break;
							default:
								__debugbreak();
								break;
							}

							FbxVector4 fbxControlPoints = pfbxMesh->GetControlPointAt(controlPointIndex);
							ImGui::Text("Polygon : %d\t", polygonIdx); ImGui::SameLine();
							ImGui::Text("Vertex : %d (#%d : {%f, %f, %f})\t", vertexIdx, controlPointIndex, fbxControlPoints[0], fbxControlPoints[1], fbxControlPoints[2]); ImGui::SameLine();
							ImGui::Text("VtxColor : (%f, %f, %f, %f)", VtxColor.mRed, VtxColor.mGreen, VtxColor.mBlue, VtxColor.mAlpha);
						}
						ImGui::NewLine();
					}
					ImGui::TreePop();
				}
			}
		}

		// Smoothing : dunno this works -> Probably used in animation
		/*
		int nSmoothingElements = pfbxMesh->GetElementSmoothingCount();
		ImGui::Text("Smoothing Count : %d", nSmoothingElements);
		if (nSmoothingElements > 0)
		{
			for (int i = 0; i < nSmoothingElements; i++)
			{
				FbxGeometryElementSmoothing* pSmoothingElement = pfbxMesh->GetElementSmoothing(i);

				int nSmoothings = pSmoothingElement->GetDirectArray().GetCount();

				FbxGeometryElement::EMappingMode mappingMode = pSmoothingElement->GetMappingMode();
				FbxGeometryElement::EReferenceMode referenceMode = pSmoothingElement->GetReferenceMode();

				string strMappingMode = GetFbxMappingNodeName(mappingMode);
				string strReferenceMode = GetFbxReferenceNodeName(referenceMode);

				if (mappingMode != FbxGeometryElement::eByPolygonVertex)
				{
					ImGui::Text("%s : Unsupported mapping mode", strMappingMode.c_str());
					continue;	// if mappingMode is not eByPolygonVertex, Do not print UV data
				}

				if (referenceMode == FbxGeometryElement::eIndex)
				{
					ImGui::Text("%s : Unsupported reference mode", strReferenceMode.c_str());
					continue; // if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
				}

				string strSmoothingTreeName = "Smoothing | Name : "s + string(cstrNodeName) + " #"s + to_string(i);
				if (ImGui::TreeNode(strSmoothingTreeName.c_str()))
				{
					ImGui::Text("Mapping Mode : %s\nReference Mode : %s", strMappingMode.c_str(), strReferenceMode.c_str());
					for (int polygonIdx = 0; polygonIdx < nPolygons; polygonIdx++)
					{
						int polygonSize = pfbxMesh->GetPolygonSize(polygonIdx);
						for (int vertexIdx = 0; vertexIdx < polygonSize; vertexIdx++)
						{
							int controlPointIndex = pfbxMesh->GetPolygonVertex(polygonIdx, vertexIdx);
							int Smoothing;

							switch (referenceMode)
							{
							case fbxsdk::FbxLayerElement::eDirect:
							{
								Smoothing = pSmoothingElement->GetDirectArray().GetAt(controlPointIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndexToDirect:
							{
								int SmoothingIndex = pSmoothingElement->GetIndexArray().GetAt(controlPointIndex);
								Smoothing = pSmoothingElement->GetDirectArray().GetAt(SmoothingIndex);
								break;
							}
							case fbxsdk::FbxLayerElement::eIndex:
								__debugbreak();
								break;
							default:
								__debugbreak();
								break;
							}

							FbxVector4 fbxControlPoints = pfbxMesh->GetControlPointAt(controlPointIndex);
							ImGui::Text("Polygon : %d\t", polygonIdx); ImGui::SameLine();
							ImGui::Text("Vertex : %d (#%d : {%f, %f, %f})\t", vertexIdx, controlPointIndex, fbxControlPoints[0], fbxControlPoints[1], fbxControlPoints[2]); ImGui::SameLine();
							ImGui::Text("Smoothing : %d", Smoothing);
						}
						ImGui::NewLine();
					}
					ImGui::TreePop();
				}
			}
		}
		*/

		ImGui::TreePop();
	}

}

void FbxLoader::PrintSurfaceMaterialInfo(FbxNode* pfbxNode, const char* cstrNodeName)
{
	int nMaterials = pfbxNode->GetMaterialCount();

	if (ImGui::TreeNode(string("Materials in Node : "s + cstrNodeName).c_str()))
	{
		if (nMaterials != 0)
		{
			for (int i = 0; i < nMaterials; i++)
			{
				if (ImGui::TreeNode(string("Material #"s + to_string(i)).c_str()))
				{
					FbxSurfaceMaterial* pfbxSurfaceMaterial = pfbxNode->GetMaterial(i);

					if (pfbxSurfaceMaterial)
					{
						if (ImGui::TreeNode("All Properties"))
						{
							PrintMaterialPropertyInfoAll(pfbxSurfaceMaterial, cstrNodeName);

							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Basic Properties"))
						{
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sShadingModel, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sMultiLayer, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sEmissive, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sEmissiveFactor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sAmbient, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sAmbientFactor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sDiffuse, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sDiffuseFactor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sSpecular, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sSpecularFactor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sShininess, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sBump, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sNormalMap, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sBumpFactor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sTransparentColor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sTransparencyFactor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sReflection, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sReflectionFactor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sDisplacementColor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sDisplacementFactor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sVectorDisplacementColor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sVectorDisplacementFactor, cstrNodeName);

							ImGui::TreePop();
						}
						
						// PBR Materials
						if (ImGui::TreeNode("PBR Properties"))
						{
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sDiffuse, cstrNodeName);				// sDiffuse				== Base Color/albedo
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sReflectionFactor, cstrNodeName);	// sReflectionFactor	== Metallic
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sShininess, cstrNodeName);			// sShininess			== Roughness
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sNormalMap, cstrNodeName);			// sNormalMap			== NormalMap
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sDisplacementColor, cstrNodeName);	// sDisplacementColor	== Height/Displacement
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sEmissive, cstrNodeName);			// sEmissive			== Emission
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sEmissiveFactor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sTransparencyFactor, cstrNodeName);	// sTransparencyFactor	== Transparency/Opacity
							ImGui::Text("AO is not supported");
							ImGui::TreePop();
						}
						
						// Texture Materials
						if (ImGui::TreeNode("Possible Texture Properties"))
						{
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sDiffuse, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sAmbient, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sSpecular, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sReflection, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sEmissive, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sNormalMap, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sBump, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sDisplacementColor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sTransparentColor, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sShininess, cstrNodeName);
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sReflectionFactor, cstrNodeName);
							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Our Target Properties"))
						{
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sDiffuse, cstrNodeName);				// AlbedoColor
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sEmissive, cstrNodeName);			// EmissiveColor
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sSpecular, cstrNodeName);			// SpecularColor
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sSpecularFactor, cstrNodeName);		// SpecularHighlight
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sReflection, cstrNodeName);			// Glossiness?
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sReflectionFactor, cstrNodeName);	// Metallic?
							PrintMaterialPropertyInfo(pfbxSurfaceMaterial, FbxSurfaceMaterial::sShininess, cstrNodeName);			// Roughness? -> (1.0 - Roughness) = Smoothness?
							ImGui::TreePop();
						}

					}

					ImGui::TreePop();
				}
			}
		}
		else
		{
			ImGui::Text("This Node has no Material");
		}
		
		ImGui::TreePop();
	}



}

void FbxLoader::PrintMaterialPropertyInfo(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName, const char* cstrNodeName)
{
	FbxProperty& prop = pfbxSurfaceMaterial->FindProperty(cstrPropertyName);

	if (prop.IsValid())
	{
		FbxDataType dataType = prop.GetPropertyDataType();
		EFbxType typeEnum = dataType.GetType();

		ImGui::Text("<%s>", cstrPropertyName);
		ImGui::Text("\tProperty : %s", cstrPropertyName);
		ImGui::Text("\tProperty Data type : %s", dataType.GetName());
		
		switch (typeEnum)
		{
		case fbxsdk::eFbxUndefined:
			break;
		case fbxsdk::eFbxUChar:
		case fbxsdk::eFbxUShort:
		case fbxsdk::eFbxUInt:
		case fbxsdk::eFbxULongLong:
		{
			FbxULongLong value = prop.Get<FbxULongLong>();
			ImGui::Text("\tValue : %llu", value);
			break;
		}
		case fbxsdk::eFbxChar:
		case fbxsdk::eFbxShort:
		case fbxsdk::eFbxInt:
		case fbxsdk::eFbxLongLong:
		{
			FbxLongLong value = prop.Get<FbxLongLong>();
			ImGui::Text("\tValue : %lli", value);
			break;
		}
		case fbxsdk::eFbxHalfFloat:
		case fbxsdk::eFbxFloat:
		case fbxsdk::eFbxDouble:
		{
			FbxDouble value = prop.Get<FbxDouble>();
			ImGui::Text("\tValue : %llf", value);
			break;
		}
		case fbxsdk::eFbxDouble2:
		{
			FbxDouble2 value = prop.Get<FbxDouble2>();
			ImGui::Text("\tValue : (%f, %f)", value[0], value[1]);
			break;
		}
		case fbxsdk::eFbxDouble3:
		{
			FbxDouble3 value = prop.Get<FbxDouble3>();
			ImGui::Text("\tValue : (%f, %f, %f)", value[0], value[1], value[2]);
			break;
		}
		case fbxsdk::eFbxDouble4:
		{
			FbxDouble4 value = prop.Get<FbxDouble4>();
			ImGui::Text("\tValue : (%f, %f, %f, %f)", value[0], value[1], value[2], value[3]);
			break;
		}
		case fbxsdk::eFbxDouble4x4:
		{
			FbxDouble4x4 value = prop.Get<FbxDouble4x4>();
			ImGui::Text("\tValue : ");
			ImGui::Text("\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f", 
				value[0][1], value[0][1], value[0][2], value[0][3],
				value[1][1], value[1][1], value[1][2], value[1][3],
				value[2][1], value[2][1], value[2][2], value[2][3],
				value[3][1], value[3][1], value[3][2], value[3][3]
			);
			break;
		}
		case fbxsdk::eFbxString:
		{
			FbxString value = prop.Get<FbxString>();
			ImGui::Text("\tValue : %s", (const char*)value.Buffer());
			break;
		}
		case fbxsdk::eFbxBool:
		{
			FbxBool value = prop.Get<FbxBool>();

			const char* boolalphaValue = value ? "True" : "False";

			ImGui::Text("\tValue : %s", boolalphaValue);
			break;
		}
		case fbxsdk::eFbxEnum:
		case fbxsdk::eFbxEnumM:
		case fbxsdk::eFbxTime:
		case fbxsdk::eFbxReference:
		case fbxsdk::eFbxBlob:
		case fbxsdk::eFbxDistance:
		case fbxsdk::eFbxDateTime:
		case fbxsdk::eFbxTypeCount:
		default:
			ImGui::Text("\tNot Now");
			break;
		}

		BOOL bHasTexure = IsTexture(pfbxSurfaceMaterial, prop.GetName());
		bHasTexure ? ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "\tHas File Texture Texture") : ImGui::Text("This is Not Texture");
		if (bHasTexure)
		{
			PrintTextureInfo(pfbxSurfaceMaterial, prop.GetName());
		}

		ImGui::Text("</%s>\n", prop.GetName());

	}
	else
	{
		ImGui::Text("%s is Not Valid", cstrPropertyName);
	}

	ImGui::NewLine();
}

void FbxLoader::PrintMaterialPropertyInfoAll(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrNodeName)
{
	FbxProperty prop = pfbxSurfaceMaterial->GetFirstProperty();

	while (prop.IsValid())
	{
		prop.GetName();
		FbxDataType dataType = prop.GetPropertyDataType();
		EFbxType typeEnum = dataType.GetType();
		const char* cstrDataTypeName = dataType.GetName();

		ImGui::Text("<%s>", prop.GetName());
		ImGui::Text("\tProperty Name : %s", prop.GetName());
		ImGui::Text("\tProperty Data type : %s", dataType.GetName());

		switch (typeEnum)
		{
		case fbxsdk::eFbxUndefined:
			break;
		case fbxsdk::eFbxUChar:
		case fbxsdk::eFbxUShort:
		case fbxsdk::eFbxUInt:
		case fbxsdk::eFbxULongLong:
		{
			FbxULongLong value = prop.Get<FbxULongLong>();
			ImGui::Text("\tValue : %llu", value);
			break;
		}
		case fbxsdk::eFbxChar:
		case fbxsdk::eFbxShort:
		case fbxsdk::eFbxInt:
		case fbxsdk::eFbxLongLong:
		{
			FbxLongLong value = prop.Get<FbxLongLong>();
			ImGui::Text("\tValue : %lli", value);
			break;
		}
		case fbxsdk::eFbxHalfFloat:
		case fbxsdk::eFbxFloat:
		case fbxsdk::eFbxDouble:
		{
			FbxDouble value = prop.Get<FbxDouble>();
			ImGui::Text("\tValue : %llf", value);
			break;
		}
		case fbxsdk::eFbxDouble2:
		{
			FbxDouble2 value = prop.Get<FbxDouble2>();
			ImGui::Text("\tValue : (%f, %f)", value[0], value[1]);
			break;
		}
		case fbxsdk::eFbxDouble3:
		{
			FbxDouble3 value = prop.Get<FbxDouble3>();
			ImGui::Text("\tValue : (%f, %f, %f)", value[0], value[1], value[2]);
			break;
		}
		case fbxsdk::eFbxDouble4:
		{
			FbxDouble4 value = prop.Get<FbxDouble4>();
			ImGui::Text("\tValue : (%f, %f, %f, %f)", value[0], value[1], value[2], value[3]);
			break;
		}
		case fbxsdk::eFbxDouble4x4:
		{
			FbxDouble4x4 value = prop.Get<FbxDouble4x4>();
			ImGui::Text("\tValue : ");
			ImGui::Text("\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f",
				value[0][1], value[0][1], value[0][2], value[0][3],
				value[1][1], value[1][1], value[1][2], value[1][3],
				value[2][1], value[2][1], value[2][2], value[2][3],
				value[3][1], value[3][1], value[3][2], value[3][3]
			);
			break;
		}
		case fbxsdk::eFbxString:
		{
			FbxString value = prop.Get<FbxString>();
			ImGui::Text("\tValue : %s", (const char*)value.Buffer());
			break;
		}
		case fbxsdk::eFbxBool:
		{
			FbxBool value = prop.Get<FbxBool>();

			const char* boolalphaValue = value ? "True" : "False";

			ImGui::Text("\tValue : %s", boolalphaValue);
			break;
		}
		case fbxsdk::eFbxEnum:
		case fbxsdk::eFbxEnumM:
		case fbxsdk::eFbxTime:
		case fbxsdk::eFbxReference:
		case fbxsdk::eFbxBlob:
		case fbxsdk::eFbxDistance:
		case fbxsdk::eFbxDateTime:
		case fbxsdk::eFbxTypeCount:
		default:
			ImGui::Text("\tNot Now");
			break;
		}

		BOOL bHasTexure = IsTexture(pfbxSurfaceMaterial, prop.GetName());
		bHasTexure ? ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "\tHas File Texture Texture") : ImGui::Text("This is Not Texture");
		if (bHasTexure)
		{
			PrintTextureInfo(pfbxSurfaceMaterial, prop.GetName());
		}

		ImGui::Text("</%s>\n", prop.GetName());

		prop = pfbxSurfaceMaterial->GetNextProperty(prop);
	}
}

void FbxLoader::PrintTextureInfo(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName)
{
	FbxProperty& reffbxProperty = pfbxSurfaceMaterial->FindProperty(cstrPropertyName);

	string strcstrNodeName = cstrPropertyName + " Info"s;

	if (reffbxProperty.IsValid())
	{
		if (ImGui::TreeNode(strcstrNodeName.c_str()))
		{
			ImGui::Text("Texture Property : %s -> ", cstrPropertyName);
			ImGui::SameLine();

			int nLayeredTextures = reffbxProperty.GetSrcObjectCount<FbxLayeredTexture>();
			if (nLayeredTextures > 0)
			{
				ImGui::Text("has Layered Texture");

				for (int i = 0; i < nLayeredTextures; i++)
				{
					FbxLayeredTexture* pfbxLayeredTexture = reffbxProperty.GetSrcObject<FbxLayeredTexture>();
					int nTextures = pfbxLayeredTexture->GetSrcObjectCount<FbxTexture>();
					for (int j = 0; j < nTextures; j++)
					{
						const char* cstrLayeredTextureName = pfbxLayeredTexture->GetName();
						ImGui::Text("Texture Name : %s", cstrLayeredTextureName);
					}
				}
			}
			else
			{
				int nTextures = reffbxProperty.GetSrcObjectCount<FbxTexture>();
				if (nTextures > 0)
				{
					ImGui::Text("has Non-Layered Texture");

					for (int i = 0; i < nTextures; i++)
					{
						FbxTexture* pfbxTexture = reffbxProperty.GetSrcObject<FbxTexture>();
						if (pfbxTexture)
						{
							const char* cstrTextureName = pfbxTexture->GetName();
							ImGui::Text("Texture Name : %s", cstrTextureName);
						}

						FbxFileTexture* pfbxFileTexture = FbxCast<FbxFileTexture>(pfbxTexture);
						if (pfbxFileTexture)
						{
							ImGui::Text("This Texture is File Texture");
							ImGui::Text("Texture Path : %s", pfbxFileTexture->GetFileName());
						}
						else
						{
							ImGui::Text("This Texture Cannot be File Texture");
						}
					}
				}
				else
				{
					ImGui::Text("has no texture");
				}
			}

			ImGui::TreePop();
		}
	}
	else
	{
		ImGui::Text("%s property don't have Texture", cstrPropertyName);
	}
}

BOOL FbxLoader::IsTexture(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName)
{
	BOOL bResult = FALSE;
	FbxProperty& reffbxProperty = pfbxSurfaceMaterial->FindProperty(cstrPropertyName);

	if (reffbxProperty.IsValid())
	{
		int nLayeredTextures = reffbxProperty.GetSrcObjectCount<FbxLayeredTexture>();
		int nNonLayeredTextures = reffbxProperty.GetSrcObjectCount<FbxTexture>();

		if (nLayeredTextures != 0 || nNonLayeredTextures != 0)
			bResult = TRUE;
	} 

	return bResult;
}

void FbxLoader::ExportModelInSceneToModel(std::shared_ptr<Model> pOutModel)
{
	if (m_rpfbxRootNode)
	{
		for (int i = 0; i < m_rpfbxRootNode->GetChildCount(); i++)
			ExportNode(pOutModel, m_rpfbxRootNode->GetChild(i), -1);
	}

	Model::UpdateChildsInModelNodes(pOutModel->GetModelNodes());
}

void FbxLoader::ExportNode(std::shared_ptr<Model> pOutModel, FbxNode* pfbxNode, int uiParentIndex)
{
	FbxNodeAttribute* fbxAttribute = pfbxNode->GetNodeAttributeByIndex(0);
	FbxNodeAttribute::EType fbxeType = fbxAttribute->GetAttributeType();

	if (fbxeType == FbxNodeAttribute::eMesh)
	{
		shared_ptr<ModelNode> pModelNode = make_shared<ModelNode>();

		// Name
		pModelNode->strName = pfbxNode->GetName();

		// Transform
		FbxDouble3 fbxvTranslation = pfbxNode->LclTranslation.Get();
		FbxDouble3 fbxvRotation = pfbxNode->LclRotation.Get();
		FbxDouble3 fbxvScaling = pfbxNode->LclScaling.Get();

		pModelNode->pTransform->SetLocalPosition(XMFLOAT3(fbxvTranslation[0], fbxvTranslation[1], fbxvTranslation[2]));
		pModelNode->pTransform->SetLocalRotation(XMFLOAT3(fbxvRotation[0], fbxvRotation[1], fbxvRotation[2]));
		pModelNode->pTransform->SetLocalScale(XMFLOAT3(fbxvScaling[0], fbxvScaling[1], fbxvScaling[2]));

		// Mesh, Material
		if (pfbxNode->GetMesh())
		{
			ExportMesh(pModelNode, pfbxNode->GetMesh());
		}

		if (pfbxNode->GetMaterial(0))
		{
			ExportMaterial(pModelNode, pfbxNode);
		}

		pModelNode->parentIndex = uiParentIndex;
		pOutModel->AddModelNode(pModelNode);
	}

	UINT uiCurrentNodeIndex = pOutModel->GetModelNodes().size() - 1;
	for (int i = 0; i < pfbxNode->GetChildCount(); i++)
	{
		ExportNode(pOutModel, pfbxNode->GetChild(i), uiCurrentNodeIndex);
	}
}

void FbxLoader::ExportMesh(std::shared_ptr<ModelNode> pOutModelNode, FbxMesh* pfbxMesh)
{
	vector<VertexType>	vertices = {};
	vector<UINT>		indices = {};

	// Indices first
	int nPolygons = pfbxMesh->GetPolygonCount();
	size_t nIndices = nPolygons * 3;
	for (int i = 0; i < nPolygons; i++)
	{
		int nPolygonSize = pfbxMesh->GetPolygonSize(i);
		for (int j = 1; j < nPolygonSize - 1; j++)
		{
			UINT index0 = pfbxMesh->GetPolygonVertex(i, 0);
			UINT index1 = pfbxMesh->GetPolygonVertex(i, j);
			UINT index2 = pfbxMesh->GetPolygonVertex(i, j + 1);
			indices.push_back(index0);
			indices.push_back(index1);
			indices.push_back(index2);
		}
	}

	// Vertices Next
	// Other Input Varibles are loaded in same time
	int nVertices = pfbxMesh->GetControlPointsCount();

	for (int i = 0; i < nVertices; i++)
	{
		VertexType vtx = {};

		// Vertex
		FbxVector4 position = pfbxMesh->GetControlPointAt(i);
		vtx.Position = XMFLOAT3(position[0], position[1], position[2]);

		// UV, Normal, BiNormal, Tangent
		int nUVElements = pfbxMesh->GetElementUVCount();
		int nNormalElements = pfbxMesh->GetElementNormalCount();
		int nBinormalElements = pfbxMesh->GetElementBinormalCount();
		int nTangentElements = pfbxMesh->GetElementTangentCount();
		int nvtxColorElements = pfbxMesh->GetElementVertexColorCount();

		FbxGeometryElementUV* uvElement = nullptr;
		FbxGeometryElementNormal* normalElement = nullptr;
		FbxGeometryElementBinormal* binormalElement = nullptr;
		FbxGeometryElementTangent* tangentElement = nullptr;
		FbxGeometryElementVertexColor* vtxColorElement = nullptr;

		uvElement = nUVElements > 0 ? pfbxMesh->GetElementUV() : nullptr;
		normalElement = nNormalElements > 0 ? pfbxMesh->GetElementNormal() : nullptr;
		binormalElement = nBinormalElements > 0 ? pfbxMesh->GetElementBinormal() : nullptr;
		tangentElement = nBinormalElements > 0 ? pfbxMesh->GetElementTangent() : nullptr;
		vtxColorElement = nvtxColorElements > 0 ? pfbxMesh->GetElementVertexColor() : nullptr;

		// UV
		if (uvElement)
		{
			FbxVector2 uv;
			if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				switch (uvElement->GetReferenceMode())
				{
				case fbxsdk::FbxLayerElement::eDirect:
				{
					uv = uvElement->GetDirectArray().GetAt(i);
					break;
				}
				case fbxsdk::FbxLayerElement::eIndexToDirect:
				{
					int uvIndex = uvElement->GetIndexArray().GetAt(i);
					uv = uvElement->GetDirectArray().GetAt(uvIndex);
					break;
				}
				case fbxsdk::FbxLayerElement::eIndex:
				{
					uv = { 0.f, 0.f };
					break;
				}
				default:
					__debugbreak();
					break;
				}
			}
			else
			{
				uv = { 0.f, 0.f };
			}

			vtx.TexCoord = XMFLOAT2(uv[0], uv[1]);
		}
		else
		{
			vtx.TexCoord = XMFLOAT2(0.f, 0.f);
		}
		
		// Normal
		if (normalElement)
		{
			FbxVector4 normal;
			if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				switch (normalElement->GetReferenceMode())
				{
				case fbxsdk::FbxLayerElement::eDirect:
				{
					normal = normalElement->GetDirectArray().GetAt(i);
					break;
				}
				case fbxsdk::FbxLayerElement::eIndexToDirect:
				{
					int normalIndex = normalElement->GetIndexArray().GetAt(i);
					normal = normalElement->GetDirectArray().GetAt(normalIndex);
					break;
				}
				case fbxsdk::FbxLayerElement::eIndex:
				{
					normal = { 0.f, 0.f, 0.f, 0.f };
					break;
				}
				default:
					__debugbreak();
					break;
				}
			}
			else
			{
				normal = { 0.f, 0.f, 0.f, 0.f };
			}

			vtx.Normal = XMFLOAT3(normal[0], normal[1], normal[2]);
		}
		else
		{
			vtx.Normal = XMFLOAT3(0.f, 0.f, 0.f);
		}
		
		// Binormal
		if (binormalElement)
		{
			FbxVector4 binormal;
			if (binormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				switch (binormalElement->GetReferenceMode())
				{
				case fbxsdk::FbxLayerElement::eDirect:
				{
					binormal = binormalElement->GetDirectArray().GetAt(i);
					break;
				}
				case fbxsdk::FbxLayerElement::eIndexToDirect:
				{
					int binormalIndex = binormalElement->GetIndexArray().GetAt(i);
					binormal = binormalElement->GetDirectArray().GetAt(binormalIndex);
					break;
				}
				case fbxsdk::FbxLayerElement::eIndex:
				{
					binormal = { 0.f, 0.f, 0.f, 0.f };
					break;
				}
				default:
					__debugbreak();
					break;
				}
			}
			else
			{
				binormal = { 0.f, 0.f, 0.f, 0.f };
			}

			vtx.BiNormal = XMFLOAT3(binormal[0], binormal[1], binormal[2]);
		}
		else
		{
			vtx.BiNormal = XMFLOAT3(0.f, 0.f, 0.f);
		}
		
		// Tangent
		if (tangentElement)
		{
			FbxVector4 tangent;
			if (tangentElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				switch (tangentElement->GetReferenceMode())
				{
				case fbxsdk::FbxLayerElement::eDirect:
				{
					tangent = tangentElement->GetDirectArray().GetAt(i);
					break;
				}
				case fbxsdk::FbxLayerElement::eIndexToDirect:
				{
					int tangentIndex = tangentElement->GetIndexArray().GetAt(i);
					tangent = tangentElement->GetDirectArray().GetAt(tangentIndex);
					break;
				}
				case fbxsdk::FbxLayerElement::eIndex:
				{
					tangent = { 0.f, 0.f, 0.f, 0.f };
					break;
				}
				default:
					__debugbreak();
					break;
				}
			}
			else
			{
				tangent = { 0.f, 0.f, 0.f, 0.f };
			}

			vtx.Tangent = XMFLOAT3(tangent[0], tangent[1], tangent[2]);
		}
		else
		{
			vtx.Tangent = XMFLOAT3(0.f, 0.f, 0.f);
		}
		
		// vtxColor
		if (vtxColorElement)
		{
			FbxColor vtxColor;
			if (vtxColorElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				switch (vtxColorElement->GetReferenceMode())
				{
				case fbxsdk::FbxLayerElement::eDirect:
				{
					vtxColor = vtxColorElement->GetDirectArray().GetAt(i);
					break;
				}
				case fbxsdk::FbxLayerElement::eIndexToDirect:
				{
					int vtxColorIndex = vtxColorElement->GetIndexArray().GetAt(i);
					vtxColor = vtxColorElement->GetDirectArray().GetAt(vtxColorIndex);
					break;
				}
				case fbxsdk::FbxLayerElement::eIndex:
				{
					vtxColor = FbxColor(1.f, 0.f, 0.f, 1.f);
					break;
				}
				default:
					__debugbreak();
					break;
				}
			}
			else
			{
				vtxColor = FbxColor(1.f, 0.f, 0.f, 1.f);
			}

			vtx.Color = XMFLOAT4(vtxColor.mRed, vtxColor.mGreen, vtxColor.mBlue, vtxColor.mAlpha);
		}
		else
		{
			vtx.Color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
		}

		vertices.push_back(vtx);

	}

	pOutModelNode->pMesh->Initialize(vertices, indices);

}

void FbxLoader::ExportMaterial(std::shared_ptr<ModelNode> pOutModelNode, FbxNode* pfbxNode)
{
	FbxSurfaceMaterial* pfbxSurfaceMaterial = pfbxNode->GetMaterial(0);
	MaterialData materialData = {};

	// This time, We only export Phong shading variables
	// Phong shading variables : Diffuse, Specular, Ambient, Emissive

	if (pfbxSurfaceMaterial)
	{
		// Diffuse (+ Texture)
		FbxProperty pfbxDiffuseProp = pfbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		wstring wstrDiffuseTexturePath = L"";
		if (pfbxDiffuseProp.IsValid())
		{
			FbxColor fbxDiffuseColor = pfbxDiffuseProp.Get<FbxColor>();
			materialData.diffuseColor = XMFLOAT4(fbxDiffuseColor.mRed, fbxDiffuseColor.mGreen, fbxDiffuseColor.mBlue, fbxDiffuseColor.mAlpha);

			if (IsTexture(pfbxSurfaceMaterial, FbxSurfaceMaterial::sDiffuse))
			{
				FbxTexture* pfbxTexture = pfbxDiffuseProp.GetSrcObject<FbxTexture>();
				FbxFileTexture* pfbxFileTexture = FbxCast<FbxFileTexture>(pfbxTexture);

				try
				{
					string strTexturePath = pfbxFileTexture->GetFileName();
					filesystem::path fsTexturePath(strTexturePath);
					wstrDiffuseTexturePath = fsTexturePath.wstring();
				}
				catch (::exception e)
				{
					wstrDiffuseTexturePath = L"";	// Error Texture
				}
			}
		}
		else
		{
			materialData.diffuseColor = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		}

		// Specular
		FbxProperty pfbxSpecularProp = pfbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
		if (pfbxSpecularProp.IsValid())
		{
			FbxColor fbxSpecularColor = pfbxSpecularProp.Get<FbxColor>();
			materialData.specularColor = XMFLOAT4(fbxSpecularColor.mRed, fbxSpecularColor.mGreen, fbxSpecularColor.mBlue, fbxSpecularColor.mAlpha);
		}
		else
		{
			materialData.specularColor = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		}

		// Ambient
		FbxProperty pfbxAmbientProp = pfbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
		if (pfbxAmbientProp.IsValid())
		{
			FbxColor fbxAmbientColor = pfbxAmbientProp.Get<FbxColor>();
			materialData.ambientColor = XMFLOAT4(fbxAmbientColor.mRed, fbxAmbientColor.mGreen, fbxAmbientColor.mBlue, fbxAmbientColor.mAlpha);
		}
		else
		{
			materialData.ambientColor = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		}

		// Emissive
		FbxProperty pfbxEmissiveProp = pfbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
		if (pfbxEmissiveProp.IsValid())
		{
			FbxColor fbxEmissiveColor = pfbxEmissiveProp.Get<FbxColor>();
			materialData.emissiveColor = XMFLOAT4(fbxEmissiveColor.mRed, fbxEmissiveColor.mGreen, fbxEmissiveColor.mBlue, fbxEmissiveColor.mAlpha);
		}
		else
		{
			materialData.emissiveColor = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		}


		pOutModelNode->pMaterial->Initialize(wstrDiffuseTexturePath);
		pOutModelNode->pMaterial->SetMaterialData(materialData);
	}

}

void FbxLoader::CleanUp()
{
	m_rpfbxRootNode->Destroy();
	m_rpfbxScene->Destroy();
	m_rpfbxImporter->Destroy();
	m_rpfbxIOSettings->Destroy();
	m_rpfbxManager->Destroy();

}
