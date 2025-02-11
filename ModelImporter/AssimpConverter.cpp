#include "stdafx.h"
#include "AssimpConverter.h"
#include "Model.h"

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
		aiProcess_MakeLeftHanded |
		aiProcess_FlipUVs |
		aiProcess_FlipWindingOrder |	// Convert to D3D
		//aiProcess_PreTransformVertices | 
		aiProcess_JoinIdenticalVertices | 
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
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
	PrintNodeInfo(m_rpRootNode);

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
	ImGui::Indent();

	string strNodeTreeName = "Node Name = " + string(pNode->mName.C_Str());
	if (ImGui::TreeNode(strNodeTreeName.c_str()))
	{
		PrintTransformInfo(pNode);
		PrintMeshInfo(pNode);
		PrintMaterialInfo(pNode);

		ImGui::Separator();
		ImGui::TreePop();
	}

	for (int i = 0; i < pNode->mNumChildren; i++)
	{
		PrintNodeInfo(pNode->mChildren[i]);
	}

	ImGui::Unindent();
}

void AssimpConverter::PrintTransformInfo(aiNode* pNode)
{
	aiMatrix4x4 aimatTransform = pNode->mTransformation;
	XMFLOAT4X4 matTransform(aimatTransform[0]);

	// Transpose
	XMMATRIX xmMat = XMLoadFloat4x4(&matTransform);
	XMMatrixTranspose(xmMat);
	XMStoreFloat4x4(&matTransform, xmMat);

	string strNodeName = pNode->mName.C_Str();
	string strTreeName = "Transfrom info" + strNodeName;

	if (ImGui::TreeNode(strTreeName.c_str()))
	{
		ImGui::Text("Transform Matrix : ");
		ImGui::Text("%f\t%f\t%f\t%f", matTransform(0, 0), matTransform(0, 1), matTransform(0, 2), matTransform(0, 3));
		ImGui::Text("%f\t%f\t%f\t%f", matTransform(1, 0), matTransform(1, 1), matTransform(1, 2), matTransform(1, 3));
		ImGui::Text("%f\t%f\t%f\t%f", matTransform(2, 0), matTransform(2, 1), matTransform(2, 2), matTransform(2, 3));
		ImGui::Text("%f\t%f\t%f\t%f", matTransform(3, 0), matTransform(3, 1), matTransform(3, 2), matTransform(3, 3));

		ImGui::TreePop();
	}


}

void AssimpConverter::PrintMeshInfo(aiNode* pNode)
{
	if (pNode->mNumMeshes < 1)
	{
		ImGui::Text("This node doesn't have mesh");
		return;
	}

	string strNodeName = pNode->mName.C_Str();

	string strTreeName = "Mesh Info : " + strNodeName;
	if (ImGui::TreeNode(strTreeName.c_str()))
	{
		for (int i = 0; i < pNode->mNumMeshes; i++)
		{
			UINT index = pNode->mMeshes[i];
			const aiMesh* pMesh = m_rpScene->mMeshes[index];

			ImGui::Text("Mesh's nVertices : %d", pMesh->mNumVertices);

			// Position
			strTreeName = "Vertices Info : " + strNodeName;
			if(ImGui::TreeNode(strTreeName.c_str()))
			{
				for (int vtxIndex = 0; vtxIndex < pMesh->mNumVertices; vtxIndex++)
				{
					XMFLOAT3 pos = XMFLOAT3(&pMesh->mVertices[vtxIndex][0]);
					ImGui::Text("#%d : (%f, %f, %f)", vtxIndex, pos.x, pos.y, pos.z);
				}

				ImGui::TreePop();
			}

			// UV (Channel 0)
			strTreeName = "UV Info (Channel 0) : " + strNodeName;
			if (ImGui::TreeNode(strTreeName.c_str()))
			{
				aiVector3D* UVChannel0 = pMesh->mTextureCoords[0];
				for (UINT uvIndex = 0; uvIndex < pMesh->mNumVertices; uvIndex++)
				{
					XMFLOAT2 UV = XMFLOAT2(&UVChannel0[uvIndex][0]);
					ImGui::Text("#%d : (%f, %f)", uvIndex, UV.x, UV.y);
				}

				ImGui::TreePop();
			}

			// Normal
			strTreeName = "Normals Info : " + strNodeName;
			if (ImGui::TreeNode(strTreeName.c_str()))
			{
				for (int vtxIndex = 0; vtxIndex < pMesh->mNumVertices; vtxIndex++)
				{
					XMFLOAT3 normal = XMFLOAT3(&pMesh->mNormals[vtxIndex][0]);
					ImGui::Text("#%d : (%f, %f, %f)", vtxIndex, normal.x, normal.y, normal.z);
				}

				ImGui::TreePop();
			}
			
			// BiTangent
			strTreeName = "Bitangent Info : " + strNodeName;
			if (ImGui::TreeNode(strTreeName.c_str()))
			{
				for (int vtxIndex = 0; vtxIndex < pMesh->mNumVertices; vtxIndex++)
				{
					XMFLOAT3 biTangent = XMFLOAT3(&pMesh->mBitangents[vtxIndex][0]);
					ImGui::Text("#%d : (%f, %f, %f)", vtxIndex, biTangent.x, biTangent.y, biTangent.z);
				}

				ImGui::TreePop();
			}
			
			// Tangent
			strTreeName = "Tangent Info : " + strNodeName;
			if (ImGui::TreeNode(strTreeName.c_str()))
			{
				for (int vtxIndex = 0; vtxIndex < pMesh->mNumVertices; vtxIndex++)
				{
					XMFLOAT3 tangent = XMFLOAT3(&pMesh->mTangents[vtxIndex][0]);
					ImGui::Text("#%d : (%f, %f, %f)", vtxIndex, tangent.x, tangent.y, tangent.z);
				}

				ImGui::TreePop();
			}



		}

		ImGui::TreePop();
	}
}

void AssimpConverter::PrintMaterialInfo(aiNode* pNode)
{
}


void AssimpConverter::ExportMeshInConverterToModel(shared_ptr<Model> pOutModel)
{
	std::filesystem::path p(m_wstrFilePath);
	pOutModel->SetName(p.stem().string());

	if (m_rpScene)
	{
		for (int i = 0; i < m_rpRootNode->mNumChildren; i++)
		{
			aiNode* pNode = m_rpRootNode->mChildren[i];
			ExportNode(pNode, pOutModel, -1, -1);
		}
	}

}

void AssimpConverter::ExportNode(aiNode* pNode, shared_ptr<Model> pOutModel, int curindex, int parentIndex)
{
	// Export node with mesh only
	BOOL bIsHelperNode = IsNodeHelperNode(pNode);

	if (bIsHelperNode == FALSE)
	{
		for (int i = 0; i < pNode->mNumMeshes; i++)
		{
			UINT meshIndex = pNode->mMeshes[i];
			aiMesh* pMesh = m_rpScene->mMeshes[meshIndex];
			UINT materialIndex = pMesh->mMaterialIndex;
			aiMaterial* pMaterial = m_rpScene->mMaterials[materialIndex];

			shared_ptr<ModelNode> pModelNode = make_shared<ModelNode>();

			pModelNode->strName = pNode->mName.C_Str();
			pModelNode->parentIndex = parentIndex;

			ExportTransform(pNode, pModelNode);
			ExportMesh(pMesh, pModelNode);
			ExportMaterial(pMaterial, pModelNode);

			pOutModel->AddModelNode(pModelNode);
		}
	}

	for (int i = 0; i < pNode->mNumChildren; i++)
	{
		ExportNode(pNode->mChildren[i], pOutModel, pOutModel->GetModelNodes().size(), curindex);
	}

}

BOOL AssimpConverter::ExportMesh(aiMesh* pMesh, shared_ptr<ModelNode> pOutModelNode)
{
	if (!pMesh) return FALSE;

	VertexType v;
	vector<VertexType> vtx = {};
	vector<UINT> idx = {};

	UINT nVertices = pMesh->mNumVertices;
	vtx.resize(nVertices);

	for (int vtxIndex = 0; vtxIndex < nVertices; vtxIndex++)
	{
		// Position
		vtx[vtxIndex].Position = XMFLOAT3(&pMesh->mVertices[vtxIndex][0]);

		// UV : Only export UV in channel[0]
		aiVector3D* uvChannel0 = pMesh->mTextureCoords[0];
		unsigned int nUVComponents = pMesh->mNumUVComponents[0];
		if (uvChannel0)
		{
			if (nUVComponents != 2)
			{
				OutputDebugStringA("UV doesn't have 2 components");
				__debugbreak();
				return FALSE;
			}
			else
			{
				vtx[vtxIndex].TexCoord = XMFLOAT2(&uvChannel0[vtxIndex][0]);
			}
		}
		else
		{
			vtx[vtxIndex].TexCoord = XMFLOAT2(0.f, 0.f);
		}

		// Normal
		if (pMesh->mNormals)
			vtx[vtxIndex].Normal = XMFLOAT3(&pMesh->mNormals[vtxIndex][0]);
		else
			vtx[vtxIndex].Normal = XMFLOAT3(0.f, 0.f, 0.f);

		// Binormal (BiTangent)
		if(pMesh->mBitangents)
			vtx[vtxIndex].BiNormal = XMFLOAT3(&pMesh->mBitangents[vtxIndex][0]);
		else
			vtx[vtxIndex].BiNormal = XMFLOAT3(0.f, 0.f, 0.f);

		// Tangent
		if(pMesh->mTangents)
			vtx[vtxIndex].Tangent = XMFLOAT3(&pMesh->mTangents[vtxIndex][0]);
		else
			vtx[vtxIndex].Tangent = XMFLOAT3(0.f, 0.f, 0.f);

		// Color : Only export color in channel[0]
		aiColor4D* colorChannel0 = pMesh->mColors[0];
		if (colorChannel0)
		{
			vtx[vtxIndex].Color = XMFLOAT4(&colorChannel0[vtxIndex][0]);
		}
		else
		{
			vtx[vtxIndex].Color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		}

	}

	UINT nIndices = pMesh->mNumFaces * 3;
	idx.resize(nIndices);

	for (int faceIndex = 0; faceIndex < pMesh->mNumFaces; faceIndex++)
	{
		aiFace face = pMesh->mFaces[faceIndex];
		UINT nFaceComponents = face.mNumIndices;

		// We already triangulated so, __debugbreak() must not excuted
		// if __debugbreak() occured, It means serious problems in this program
		if (nFaceComponents != 3)
		{
			OutputDebugStringA("Face is not triangle primitive type");
			__debugbreak();
			return FALSE;
		}

		for (int i = 0; i < nFaceComponents; i++)
		{
			UINT indexPos = (faceIndex * 3) + i;
			idx[indexPos] = face.mIndices[i];
		}
	}

	pOutModelNode->pMesh->Initialize(vtx, idx);

	return TRUE;
}

BOOL AssimpConverter::ExportTransform(aiNode* pNode, std::shared_ptr<ModelNode> pOutModelNode)
{
	if (!pNode) return FALSE;

	// Current aiNode::mTransformation is somewhat weird
	// but Decomposed data looks legit. so let's use it
	aiMatrix4x4 transform = pNode->mTransformation;

	aiVector3D vPosition(0.f, 0.f, 0.f);
	aiVector3D vRotation(0.f, 0.f, 0.f);
	aiVector3D vScale(0.f, 0.f, 0.f);

	transform.Decompose(vScale, vRotation, vPosition);

	pOutModelNode->pTransform->SetLocalPosition(XMFLOAT3(vPosition.x, vPosition.y, vPosition.z));
	pOutModelNode->pTransform->SetLocalRotation(XMFLOAT3(vRotation.x, vRotation.y, vRotation.z));
	pOutModelNode->pTransform->SetLocalScale(XMFLOAT3(vScale.x, vScale.y, vScale.z));

	return TRUE;
}

BOOL AssimpConverter::ExportMaterial(aiMaterial* pMaterial, shared_ptr<ModelNode> pOutModelNode)
{
	if (!pMaterial) return FALSE;

	// We will only export (02.11.2025) :
	// diffuse color
	// specular color
	// ambient color
	// emissive color
	// diffuse texture

	// Colors
	aiColor4D color;
	MaterialData data;

	pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	data.diffuseColor = XMFLOAT4(color.r, color.g, color.b, color.a);

	pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
	data.specularColor = XMFLOAT4(color.r, color.g, color.b, color.a);

	pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
	data.ambientColor = XMFLOAT4(color.r, color.g, color.b, color.a);

	pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
	data.emissiveColor = XMFLOAT4(color.r, color.g, color.b, color.a);

	// Diffuse texture
	// only save filename
	aiString texturePath;

	pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePath);
	string strTexturePath = texturePath.C_Str();
	fs::path fsTexturePath(strTexturePath);

	wstring wstrTextureFilename = fsTexturePath.filename().wstring();

	pOutModelNode->pMaterial->Initialize(wstrTextureFilename);
	pOutModelNode->pMaterial->SetMaterialData(data);

	return TRUE;
}

void AssimpConverter::PrintTabs()
{
	for (UINT i = 0; i < m_tabs; i++)
	{
		ImGui::Text("\t");
		ImGui::SameLine();
	}
}
