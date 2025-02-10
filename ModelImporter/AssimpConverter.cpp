#include "stdafx.h"
#include "AssimpConverter.h"

using namespace std;
namespace fs = std::filesystem;

AssimpConverter::AssimpConverter()
{
}

AssimpConverter::~AssimpConverter()
{
}

BOOL AssimpConverter::Initialize(const std::wstring& wstrPath)
{
	m_pImporter = make_shared<Assimp::Importer>();
	m_wstrFilePath = wstrPath;

	fs::path p(wstrPath);
	if (!fs::exists(p))
	{
		OutputDebugStringA("File is invalid\n");
		__debugbreak();
		return FALSE;
	}

	string strPath(wstrPath.begin(), wstrPath.end());

	m_rpScene = m_pImporter->ReadFile(strPath,
		aiProcess_ConvertToLeftHanded |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_MakeLeftHanded 
	);

	if (m_rpScene == nullptr)
	{
		OutputDebugStringA("Scene is not imported\n");
		__debugbreak();
		return FALSE;
	}

	m_rpRootNode = m_rpScene->mRootNode;

	return TRUE;
}

void AssimpConverter::PrintModelDataToImGui()
{
	for (int i = 0; i < m_rpRootNode->mNumChildren; i++)
	{
		PrintNodeInfo(m_rpRootNode->mChildren[i]);
	}

	int nVertices = 0;
	for (int i = 0; i < m_rpScene->mNumMeshes; i++)
	{
		nVertices += m_rpScene->mMeshes[i]->mNumVertices;
	}

	ImGui::NewLine();
	ImGui::Text("Total Vertex Count : %d", nVertices);
}

void AssimpConverter::PrintNodeInfo(aiNode* pNode)
{
	// Let't print nodes with mesh only

	BOOL bIsHelperNode = IsNodeHelperNode(pNode);

	if (bIsHelperNode == FALSE)
	{
		m_tabs++;

		string strNodeName = pNode->mName.C_Str();
		string strTreeNodeName = "Node Name : " + strNodeName;
		PrintTabs();
		if (ImGui::TreeNode(strTreeNodeName.c_str()))
		{
			// Mesh
			string strMeshTreeNodeName = strNodeName + "'s Mesh"s;
			if(ImGui::TreeNode(strMeshTreeNodeName.c_str()))
			{
				// Position
				for (int i = 0; i < pNode->mNumMeshes; i++)
				{
					UINT meshIndex = *pNode->mMeshes;
					aiMesh* pMesh = m_rpScene->mMeshes[meshIndex];
					string strVtxTreeNodeName = strNodeName + "'s Vertices"s;
					if (ImGui::TreeNode(strVtxTreeNodeName.c_str()))
					{
						ImGui::Text("nVertices : %d", pMesh->mNumVertices);
						for (int vtxIndex = 0; vtxIndex < pMesh->mNumVertices; vtxIndex++)
						{
							aiVector3D vtx = pMesh->mVertices[vtxIndex];
							ImGui::Text("#%d : (%f, %f, %f)", vtxIndex, vtx[0], vtx[1], vtx[2]);
						}
						ImGui::TreePop();
					}

					// UV : Can have multiple channels
					for (int nUVChannels = 0; nUVChannels < pMesh->GetNumUVChannels(); nUVChannels++)
					{
						string strUVTreeNodeName = strNodeName + "'s UV #" + to_string(nUVChannels);
						if (ImGui::TreeNode(strUVTreeNodeName.c_str()))
						{
							aiVector3D* uvChannel = pMesh->mTextureCoords[nUVChannels];
							unsigned int nUVComponents = pMesh->mNumUVComponents[nUVChannels];
							ImGui::Text("UV Channel #%d | Dimension : %d", nUVChannels, nUVComponents);	// In assimp, nUVs is also nVertices
							for (int uvIndex = 0; uvIndex < pMesh->mNumVertices; uvIndex++)
							{
								aiVector3D uv = uvChannel[uvIndex];
								if (nUVComponents == 1)
									ImGui::Text("#%d : %f", uvIndex, uv[0]);
								else if (nUVComponents == 2)
									ImGui::Text("#%d : (%f, %f)", uvIndex, uv[0], uv[1]);
								else if (nUVComponents == 3)
									ImGui::Text("#%d : (%f, %f, %f)", uvIndex, uv[0], uv[1], uv[2]);
							}

							ImGui::TreePop();
						}
					}

					// Normal
					string strNormalTreeNodeName = strNodeName + "'s Normals";
					if (ImGui::TreeNode(strNormalTreeNodeName.c_str()))
					{
						ImGui::Text("nNormals : %d", pMesh->mNumVertices);	// In assimp, nNormals is nVertices
						for (int nIndex = 0; nIndex < pMesh->mNumVertices; nIndex++)
						{
							aiVector3D normal = pMesh->mNormals[nIndex];
							ImGui::Text("#%d : (%f, %f, %f)", nIndex, normal[0], normal[1], normal[2]);
						}

						ImGui::TreePop();
					}

					// BiNormal, Tangent
					string strTangentTreeNodeName = strNodeName + "'s Tangent";
					if (ImGui::TreeNode(strTangentTreeNodeName.c_str()))
					{
						aiVector3D* tangents = pMesh->mTangents;
						aiVector3D* normals = pMesh->mNormals;
						aiVector3D* biTangents = pMesh->mBitangents;	// ?? is this binormal?

						for (int nIndex = 0; nIndex < pMesh->mNumVertices; nIndex++)
						{
							aiVector3D tangent = tangents[nIndex];
							aiVector3D normal = normals[nIndex];
							aiVector3D biTangent = biTangents[nIndex];

							ImGui::NewLine();
							ImGui::Text("#%d", nIndex);
							ImGui::Text("Tangent : (%f, %f, %f)", tangent[0], tangent[1], tangent[2]);
							ImGui::Text("Normal : (%f, %f, %f)", normal[0], normal[1], normal[2]);
							ImGui::Text("Bitangent : (%f, %f, %f)", biTangent[0], biTangent[1], biTangent[2]);
						}


						ImGui::TreePop();
					}



					// Face (index)
					string strFaceTreeNodeName = strNodeName + "'s Faces";
					if (ImGui::TreeNode(strFaceTreeNodeName.c_str()))
					{
						aiFace* pFace = pMesh->mFaces;
						UINT nFaces = pMesh->mNumFaces;
						ImGui::Text("nFaces : %d", pMesh->mNumFaces);
						for (int faceIndex = 0; faceIndex < nFaces; faceIndex++)
						{
							aiFace face = pFace[faceIndex];
							UINT nFaceIndices = face.mNumIndices;
							ImGui::NewLine();
							for (int nIndices = 0; nIndices < nFaceIndices; nIndices++)
							{
								ImGui::Text("%d - #%d : %d", faceIndex, nIndices, face.mIndices[nIndices]);
							}

						}

						ImGui::TreePop();
					}


					string strPropTreeNodeName = strNodeName + "'s Properties"s;
					if (ImGui::TreeNode(strPropTreeNodeName.c_str()))
					{
						ImGui::Text("Mesh Properties");
						ImGui::Text("HasBones? : %s", pMesh->HasBones() ? "TRUE" : "FALSE");
						ImGui::Text("HasFaces? : %s", pMesh->HasFaces() ? "TRUE" : "FALSE");
						ImGui::Text("HasNormals? : %s", pMesh->HasNormals() ? "TRUE" : "FALSE");
						ImGui::Text("HasPositions? : %s", pMesh->HasPositions() ? "TRUE" : "FALSE");
						ImGui::Text("HasTangentsAndBitangents? : %s", pMesh->HasTangentsAndBitangents() ? "TRUE" : "FALSE");
						for (UINT i = 0; i < pMesh->GetNumUVChannels(); i++)
						{
							ImGui::Text("HasTextureCoords of Channel #%d? : %s", i, pMesh->HasTextureCoords(i) ? "TRUE" : "FALSE");
						}
						for (UINT i = 0; i < pMesh->GetNumColorChannels(); i++)
						{
							ImGui::Text("HasVertexColors of Channel #%d? : %s", i, pMesh->HasVertexColors(i) ? "TRUE" : "FALSE");
						}
						ImGui::TreePop();
					}
					

				}

				ImGui::TreePop();
			}

			// MetaData?
			string strMetaTreeNodeName = strNodeName + "'s MetaData";
			if (ImGui::TreeNode(strMetaTreeNodeName.c_str()))
			{
				aiMetadata* pMetaData = pNode->mMetaData;
				ImGui::Text("mNumProperties : %d", pMetaData->mNumProperties);

				for (int i = 0; i < pMetaData->mNumProperties; i++)
				{
					ImGui::NewLine();
					ImGui::Text("MetaData #%d", i);
					string key = pMetaData->mKeys[i].C_Str();
					aiMetadataEntry mdEntry = pMetaData->mValues[i];
					aiMetadataType mdType = mdEntry.mType;
					void* pData = mdEntry.mData;

					ImGui::Text("Key : %s", key.c_str());
					ImGui::Text("MetaData Type : "); ImGui::SameLine();
					switch (mdType)
					{
					case AI_BOOL:
					{
						BOOL metaData = *(bool*)pData;
						ImGui::Text("AI_BOOL");
						ImGui::Text(metaData ? "MetaData : TRUE" : "MetaData : FALSE");
						break;
					}
					case AI_INT32:
					{
						INT32 metaData = *(INT32*)pData;
						ImGui::Text("AI_INT32");
						ImGui::Text("MetaData : %d", metaData);
						break;
					}
					case AI_UINT64:
					{
						UINT64 metaData = *(UINT64*)pData;
						ImGui::Text("AI_UINT64");
						ImGui::Text("MetaData : %l", metaData);
						break;
					}
					case AI_FLOAT:
					{
						float metaData = *(float*)pData;
						ImGui::Text("AI_FLOAT");
						ImGui::Text("MetaData : %f", metaData);
						break;
					}
					case AI_DOUBLE:
					{
						double metaData = *(double*)pData;
						ImGui::Text("AI_DOUBLE");
						ImGui::Text("MetaData : %lf", metaData);
						break;
					}
					case AI_AISTRING:
					{
						string metaData = ((aiString*)pData)->C_Str();
						ImGui::Text("AI_AISTRING");
						ImGui::Text("MetaData : %s", metaData.c_str());
						break;
					}
					case AI_AIVECTOR3D:
					{
						aiVector3D metaData = *(aiVector3D*)pData;
						ImGui::Text("AI_AIVECTOR3D");
						ImGui::Text("MetaData : (%f, %f, %f)", metaData.x, metaData.y, metaData.z);
						break;
					}
					case FORCE_32BIT:
						ImGui::Text("FORCE_32BIT");
						break;
					default:
						__debugbreak();
						break;
					}
				}
				ImGui::TreePop();
			}

			// Transform
			// Something wrong in mTransformation
			string strTransformNodeName = strNodeName + "'s SRT";
			if (ImGui::TreeNode(strTransformNodeName.c_str()))
			{
				aiMatrix4x4 aiMattransform = pNode->mTransformation;
				XMFLOAT4X4 xmMatTransform(aiMattransform[0]);

				aiVector3D aivTranslate;
				aiVector3D aivRotate;
				aiVector3D aivScale;

				aiMattransform.Decompose(aivScale, aivRotate, aivTranslate);

				ImGui::Text("Translate : (%f, %f, %f)", aivTranslate.x, aivTranslate.y, aivTranslate.z);
				ImGui::Text("Rotate : (%f, %f, %f)", aivRotate.x, aivRotate.y, aivRotate.z);
				ImGui::Text("Scale : (%f, %f, %f)", aivScale.x, aivScale.y, aivScale.z);

				ImGui::Text("Matrix : ");
				ImGui::Text("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
					xmMatTransform.m[0][0], xmMatTransform.m[0][1], xmMatTransform.m[0][2], xmMatTransform.m[0][3],
					xmMatTransform.m[1][0], xmMatTransform.m[1][1], xmMatTransform.m[1][2], xmMatTransform.m[1][3],
					xmMatTransform.m[2][0], xmMatTransform.m[2][1], xmMatTransform.m[2][2], xmMatTransform.m[2][3],
					xmMatTransform.m[3][0], xmMatTransform.m[3][1], xmMatTransform.m[3][2], xmMatTransform.m[3][3]
					);

				ImGui::TreePop();
			}



			ImGui::TreePop();
		}

	}

	for (int i = 0; i < pNode->mNumChildren; i++)
	{
		PrintNodeInfo(pNode->mChildren[i]);
	}

	if (bIsHelperNode == FALSE)
	{
		m_tabs--;
	}
}

void AssimpConverter::ExportMeshInConverterToModel()
{
}

void AssimpConverter::ExportNode()
{
}

void AssimpConverter::ExportMesh()
{
}

void AssimpConverter::ExportMaterial()
{
}

void AssimpConverter::PrintTabs()
{
	for (UINT i = 0; i < m_tabs; i++)
	{
		ImGui::Text("\t");
		ImGui::SameLine();
	}
}
