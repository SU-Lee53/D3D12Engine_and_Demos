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
	for (UINT i = 0; i < m_tabs; i++)
	{
		ImGui::Text("\t");
		ImGui::SameLine();
	}
}

void Importer::ProcessNode(FbxNode* pfbxNode, const char* cstrNodeName)
{
	
	const char* pStrcstrNodeName = pfbxNode->GetName();
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
		pStrcstrNodeName,
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

void Importer::PrintLayerInfo(FbxMesh* pfbxMesh, const char* cstrNodeName)
{
	int layerCount = pfbxMesh->GetLayerCount();
	ImGui::Text("< Layer Count : %d >", layerCount);

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

void Importer::PrintMeshInfo(FbxMesh* pfbxMesh, const char* cstrNodeName)
{
	string meshTreecstrNodeName = "Mesh Info | Name : " + string(cstrNodeName);
	if (ImGui::TreeNode(meshTreecstrNodeName.c_str()))
	{
		// Polygons
		// Some models has 4 vertices per polygons
		// But, D3D supports triangle primitive only
		// So, We need to Triangulate into 3 Vertices when Export
		int nPolygons = pfbxMesh->GetPolygonCount();
		ImGui::Text("Polygon Count : %d", nPolygons);

		std::string strPolygonTreeName = "polygon vertices"s + " | Name : " + string(cstrNodeName);

		if (ImGui::TreeNode(strPolygonTreeName.c_str()))
		{
			for (int i = 0; i < nPolygons; i++)
			{
				ImGui::Text("polygon index : %d", i);
				int nPolygonSize = pfbxMesh->GetPolygonSize(i);
				ImGui::Text("polygon size : %d", nPolygonSize);

				ImGui::NewLine();
				for (int j = 0; j < nPolygonSize; j++)
				{
					int vtx = pfbxMesh->GetPolygonVertex(i, j);
					ImGui::SameLine();
					ImGui::Text("%d", vtx);
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
		ImGui::Text("Vertex Count : %d", nControlPoints);

		string strVertexTreeName = "Vertex(Control Point) | Name : "s + string(cstrNodeName);
		if (ImGui::TreeNode(strVertexTreeName.c_str()))
		{
			ImGui::Text("Vertices");
			for (int i = 0; i < nControlPoints; i++)
			{
				FbxVector4 fbxControlPoints = pfbxMesh->GetControlPointAt(i);
				ImGui::Text("%d : (%f, %f, %f, %f)", i, fbxControlPoints[0], fbxControlPoints[1], fbxControlPoints[2], fbxControlPoints[3]);
			}

			ImGui::TreePop();
		}

		// UVs
		int nUVs = pfbxMesh->GetElementUVCount();
		ImGui::Text("UV Element Count : %d", nUVs);
		if (nUVs > 0)	// if UV is valid
		{
			for (int i = 0; i < nUVs; i++)
			{
				FbxGeometryElementUV* uvElement = pfbxMesh->GetElementUV(0);

				FbxGeometryElement::EMappingMode mappingMode = uvElement->GetMappingMode();
				FbxGeometryElement::EReferenceMode referenceMode = uvElement->GetReferenceMode();

				if (mappingMode != FbxGeometryElement::eByPolygonVertex)
				{
					ImGui::Text("Unsupported UV mapping mode");
					goto lb_PrintUV_Break;	// if mappingMode is not eByPolygonVertex, Do not print UV data
				}

				string strUVTreeName = "UV | Name : "s + string(cstrNodeName) + " #"s + to_string(i);
				if (ImGui::TreeNode(strUVTreeName.c_str()))
				{
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
								ImGui::Text("Unsupported UV reference mode");
								goto lb_PrintUV_Break;	// if referenceMode is not eDirect or eIndexToDirect, Do not print UV data
							default:
								__debugbreak();
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

	lb_PrintUV_Break:


		// Normals
		int nNormals = pfbxMesh->GetElementNormalCount();
		ImGui::Text("Normals Count : %d", nNormals);

		// Binormals
		int nBinormals = pfbxMesh->GetElementBinormalCount();
		ImGui::Text("Binormals Count : %d", nBinormals);

		// Tangents
		int nTangents = pfbxMesh->GetElementTangentCount();
		ImGui::Text("Tangents Count : %d", nTangents);

		// Vertex Color
		int nVertexColor = pfbxMesh->GetElementVertexColorCount();
		ImGui::Text("Vertex Color Count : %d", nVertexColor);

		// Smoothing : dunno this works
		int nSmoothing = pfbxMesh->GetElementSmoothingCount();
		ImGui::Text("Smoothing Count : %d", nSmoothing);



		ImGui::TreePop();
	}

}

void Importer::PrintSurfaceMaterialInfo(FbxNode* pfbxNode, const char* cstrNodeName)
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

					if(pfbxSurfaceMaterial)
						PrintMaterialPropertyInfoAll(pfbxSurfaceMaterial, cstrNodeName);

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

void Importer::PrintMaterialPropertyInfo(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName, const char* cstrNodeName)
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

		ImGui::Text("</%s>", cstrPropertyName);
	}
	else
	{
		ImGui::Text("%s is Not Valid", cstrPropertyName);
	}

	ImGui::NewLine();
}

void Importer::PrintMaterialPropertyInfoAll(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrNodeName)
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
		ImGui::Text("\tIs Texture : %s", bHasTexure ? "True" : "False");
		if (bHasTexure)
		{
			PrintTextureInfo(pfbxSurfaceMaterial, prop.GetName());
		}

		ImGui::Text("</%s>\n", prop.GetName());

		prop = pfbxSurfaceMaterial->GetNextProperty(prop);
	}
}

void Importer::PrintTextureInfo(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName)
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

BOOL Importer::IsTexture(FbxSurfaceMaterial* pfbxSurfaceMaterial, const char* cstrPropertyName)
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
