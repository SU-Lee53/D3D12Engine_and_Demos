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

		TREENODE_BEGIN(strTreeNodeName.c_str())
		{
			// Mesh
			string strMeshTreeNodeName = strNodeName + "'s Mesh"s;
			TREENODE_BEGIN(strTreeNodeName.c_str())
			{
				// Position
				for (int i = 0; i < pNode->mNumMeshes; i++)
				{
					UINT meshIndex = pNode->mMeshes[i];
					aiMesh* pMesh = m_rpScene->mMeshes[meshIndex];

					string strVtxTreeNodeName = strNodeName + "'s Vertices"s;
					TREENODE_BEGIN(strVtxTreeNodeName.c_str())
					{
						ImGui::Text("nVertices : %d", pMesh->mNumVertices);

						ImGui::Indent();

						for (int vtxIndex = 0; vtxIndex < pMesh->mNumVertices; vtxIndex++)
						{
							aiVector3D vtx = pMesh->mVertices[vtxIndex];
							ImGui::Text("#%d : (%f, %f, %f)", vtxIndex, vtx[0], vtx[1], vtx[2]);
						}
						ImGui::Unindent();
						ImGui::Separator();
					}
					TREENODE_END

					// UV : Can have multiple channels
					for (int nUVChannels = 0; nUVChannels < pMesh->GetNumUVChannels(); nUVChannels++)
					{
						string strUVTreeNodeName = strNodeName + "'s UV #" + to_string(nUVChannels);
						TREENODE_BEGIN(strUVTreeNodeName.c_str())
						{
							aiVector3D* uvChannel = pMesh->mTextureCoords[nUVChannels];
							unsigned int nUVComponents = pMesh->mNumUVComponents[nUVChannels];
							ImGui::Text("UV Channel #%d | Dimension : %d", nUVChannels, nUVComponents);	// In assimp, nUVs is also nVertices

							ImGui::Indent();

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

							ImGui::Unindent();
							ImGui::Separator();
						}
						TREENODE_END
					}

					// Normal
					string strNormalTreeNodeName = strNodeName + "'s Normals";
					TREENODE_BEGIN(strNormalTreeNodeName.c_str())
					{
						ImGui::Text("nNormals : %d", pMesh->mNumVertices);	// In assimp, nNormals is nVertices
						ImGui::Indent();
						for (int nIndex = 0; nIndex < pMesh->mNumVertices; nIndex++)
						{
							aiVector3D normal = pMesh->mNormals[nIndex];
							ImGui::Text("#%d : (%f, %f, %f)", nIndex, normal[0], normal[1], normal[2]);
						}

						ImGui::Unindent();
						ImGui::Separator();
					}
					TREENODE_END

					// BiNormal, Tangent
					string strTangentTreeNodeName = strNodeName + "'s Tangent";
					TREENODE_BEGIN(strTangentTreeNodeName.c_str())
					{
						aiVector3D* tangents = pMesh->mTangents;
						aiVector3D* normals = pMesh->mNormals;
						aiVector3D* biTangents = pMesh->mBitangents;	// ?? is this binormal?

						for (int nIndex = 0; nIndex < pMesh->mNumVertices; nIndex++)
						{
							aiVector3D tangent = tangents[nIndex];
							aiVector3D normal = normals[nIndex];
							aiVector3D biTangent = biTangents[nIndex];

							ImGui::Indent();
							ImGui::Text("#%d", nIndex);
							ImGui::Text("Tangent : (%f, %f, %f)", tangent[0], tangent[1], tangent[2]);
							ImGui::Text("Normal : (%f, %f, %f)", normal[0], normal[1], normal[2]);
							ImGui::Text("Bitangent : (%f, %f, %f)", biTangent[0], biTangent[1], biTangent[2]);
							ImGui::Unindent();
							ImGui::Separator();
						}
					}
					TREENODE_END


					// Face (index)
					string strFaceTreeNodeName = strNodeName + "'s Faces";
					TREENODE_BEGIN(strFaceTreeNodeName.c_str())
					{
						aiFace* pFace = pMesh->mFaces;
						UINT nFaces = pMesh->mNumFaces;
						ImGui::Text("nFaces : %d", pMesh->mNumFaces);
						for (int faceIndex = 0; faceIndex < nFaces; faceIndex++)
						{
							aiFace face = pFace[faceIndex];
							UINT nFaceIndices = face.mNumIndices;
							ImGui::Indent();
							for (int nIndices = 0; nIndices < nFaceIndices; nIndices++)
							{
								ImGui::Text("%d - #%d : %d", faceIndex, nIndices, face.mIndices[nIndices]);
							}
							ImGui::Unindent();
							ImGui::Separator();

						}
					}
					TREENODE_END

					string strPropTreeNodeName = strNodeName + "'s Properties"s;
					TREENODE_BEGIN(strPropTreeNodeName.c_str())
					{
						ImGui::Indent();
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
						ImGui::Unindent();
					}
					TREENODE_END

				}

			}
			TREENODE_END

			// MetaData?
			string strMetaTreeNodeName = strNodeName + "'s MetaData";
			TREENODE_BEGIN(strMetaTreeNodeName.c_str())
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
			}
			TREENODE_END

			// Transform
			// Something wrong in mTransformation
			string strTransformNodeName = strNodeName + "'s SRT";
			TREENODE_BEGIN(strTransformNodeName.c_str())
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
			}
			TREENODE_END

			// Material
			string strMaterialNodeName = strNodeName + "'s Material";
			TREENODE_BEGIN(strMaterialNodeName.c_str())
			{
				for (int i = 0; i < pNode->mNumMeshes; i++)
				{
					UINT meshIndex = pNode->mMeshes[i];
					aiMesh* pMesh = m_rpScene->mMeshes[meshIndex];
					UINT materialIndex = pMesh->mMaterialIndex;
					aiMaterial* pMaterial = m_rpScene->mMaterials[materialIndex];

					aiString aistrMaterialName;
					if (pMaterial->Get(AI_MATKEY_NAME, aistrMaterialName) == AI_SUCCESS)
					{
						ImGui::Text("Material Name : %s", aistrMaterialName.C_Str());
					}
					else
					{
						ImGui::TextColored(ImVec4(1.f,0.f,0.f,1.f), "Material Name : UNKNOWN");
					}

					ImGui::Separator();

					// Material Properties
					string strPropertyNodeName = strNodeName + "'s Properties";
					TREENODE_BEGIN(strPropertyNodeName.c_str())
					{
						ImGui::Text("nProperties : %d", pMaterial->mNumProperties);
						for (int propIndex = 0; propIndex < pMaterial->mNumProperties; propIndex++)
						{
							aiMaterialProperty* pProperty = pMaterial->mProperties[propIndex];

							ImGui::Text("Property #%d", propIndex);

							ImGui::Indent();
							string strPropKey = pProperty->mKey.C_Str();
							aiPropertyTypeInfo eType = pProperty->mType;
							UINT uiDataLength = pProperty->mDataLength;
							UINT uiIndex = pProperty->mIndex;
							UINT uiSemantic = pProperty->mSemantic;

							ImGui::Text("mKey : %s", strPropKey.c_str());
							ImGui::Text("mType : %s", GetPropertyTypeToString(eType).c_str());
							ImGui::Text("mType(raw) : %u", eType);
							ImGui::Text("mDataLength : %u bytes", uiDataLength);
							ImGui::Text("mIndex : %u", uiIndex);
							ImGui::Text("mSemantic : %u", uiSemantic);

							ImGui::Unindent();
							ImGui::Separator();
						}
					}
					TREENODE_END

					// Texture Paths
					string strTextureNodeName = strNodeName + "'s Textures";
					TREENODE_BEGIN(strTextureNodeName.c_str())
					{
						aiString pTexturePath;

						pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), pTexturePath);
						ImGui::Text("DIFFUSE : %s", pTexturePath.C_Str());

						pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), pTexturePath);
						ImGui::Text("SPECULAR : %s", pTexturePath.C_Str());

						pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_AMBIENT, 0), pTexturePath);
						ImGui::Text("AMBIENT : %s", pTexturePath.C_Str());

						pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), pTexturePath);
						ImGui::Text("EMISSIVE : %s", pTexturePath.C_Str());

						pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT, 0), pTexturePath);
						ImGui::Text("HEIGHT : %s", pTexturePath.C_Str());

						pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), pTexturePath);
						ImGui::Text("NORMALS : %s", pTexturePath.C_Str());

						pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_SHININESS, 0), pTexturePath);
						ImGui::Text("SHININESS : %s", pTexturePath.C_Str());

						pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DISPLACEMENT, 0), pTexturePath);
						ImGui::Text("DISPLACEMENT : %s", pTexturePath.C_Str());

						pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_LIGHTMAP, 0), pTexturePath);
						ImGui::Text("LIGHTMAP : %s", pTexturePath.C_Str());

						pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_REFLECTION, 0), pTexturePath);
						ImGui::Text("REFLECTION : %s", pTexturePath.C_Str());
					}
					TREENODE_END

					// Colors
					string strColorNodeName = strNodeName + "'s Colors";
					TREENODE_BEGIN(strColorNodeName.c_str())
					{
						aiColor4D aiColor;

						pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
						ImGui::Text("DIFFUSE : (%f, %f, %f, %f)", aiColor.r, aiColor.g, aiColor.b, aiColor.a);

						pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, aiColor);
						ImGui::Text("AMBIENT : (%f, %f, %f, %f)", aiColor.r, aiColor.g, aiColor.b, aiColor.a);

						pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aiColor);
						ImGui::Text("SPECULAR : (%f, %f, %f, %f)", aiColor.r, aiColor.g, aiColor.b, aiColor.a);

						pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiColor);
						ImGui::Text("EMISSIVE : (%f, %f, %f, %f)", aiColor.r, aiColor.g, aiColor.b, aiColor.a);

						pMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, aiColor);
						ImGui::Text("TRANSPARENT : (%f, %f, %f, %f)", aiColor.r, aiColor.g, aiColor.b, aiColor.a);

						pMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, aiColor);
						ImGui::Text("REFLECTIVE : (%f, %f, %f, %f)", aiColor.r, aiColor.g, aiColor.b, aiColor.a);


					}
					TREENODE_END
					





				}
			}
			TREENODE_END

		}
		TREENODE_END

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
