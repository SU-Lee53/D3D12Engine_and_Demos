#include "stdafx.h"
#include "Model.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;

Model::Model()
{
}

Model::~Model()
{
}

BOOL Model::Initialize()
{
	return TRUE;
}

void Model::Update()
{
	for (const auto& node : m_pModelNodes)
	{
		node->pTransform->Update();
	}
}

void Model::Render(shared_ptr<Camera> pCamera)
{
	// What we need
	// 1. Input Layout : in VertexType::desc
	// 2. Shaders : in node->pMaterial
	// 3. Rasterizer / Sampler / Blend State : TODO : DO IT
	// 4. Constant Buffer

	unique_ptr<ConstantBuffer<CB_TRANSFORM>> cbTransform = make_unique<ConstantBuffer<CB_TRANSFORM>>();
	unique_ptr<ConstantBuffer<CB_CAMERA>> cbCamera = make_unique<ConstantBuffer<CB_CAMERA>>();
	unique_ptr<ConstantBuffer<XMFLOAT4>> cbShowColor = make_unique<ConstantBuffer<XMFLOAT4>>();

	cbTransform->Initialize();
	cbCamera->Initialize();
	cbShowColor->Initialize();

	for (UINT i = 0; i < m_pModelNodes.size(); i++)
	{
		shared_ptr<ModelNode>& node = m_pModelNodes[i];

		CB_TRANSFORM transformData = {};
		::ZeroMemory(&transformData, sizeof(transformData));
		{
			transformData.matLocal = node->pTransform->GetLocalMatrixTransposed();
			transformData.matWorld = node->pTransform->GetWorldMatrixTransposed();
		}
		cbTransform->PushData(transformData);

		CB_CAMERA cameraData = {};
		::ZeroMemory(&cameraData, sizeof(cameraData));
		{
			cameraData.matView = pCamera->GetViewMatrixTransposed();
			cameraData.matProj = pCamera->GetProjectionMatrixTransposed();
		}
		cbCamera->PushData(cameraData);

		XMFLOAT4 ShowColor = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
		if (i == m_ItemSelected)
		{
			ShowColor = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
		}
		cbShowColor->PushData(ShowColor);

		DC->PSSetShaderResources(0, 1, node->pMaterial->GetTexture()->GetComPtr().GetAddressOf());

		DC->IASetInputLayout(node->pMaterial->GetShader()->GetInputLayout().Get());
		DC->VSSetShader(node->pMaterial->GetShader()->GetVertexShader().Get(), nullptr, 0);
		// RSSetState
		DC->RSSetState(CORE.GetDefaultRasterizerState().Get());
		DC->PSSetShader(node->pMaterial->GetShader()->GetPixelShader().Get(), nullptr, 0);
		// OMSetBlendState
		const float blendFactor = 0.f;
		DC->OMSetBlendState(CORE.GetDefaultBlendState().Get(), &blendFactor, 0xFFFFFFFF);	// Need to check

		UINT stride = node->pMesh->GetVertexBuffer()->m_uiStride;
		UINT offset = node->pMesh->GetVertexBuffer()->m_uiOffset;
		DC->IASetVertexBuffers(0, 1, node->pMesh->GetVertexBuffer()->m_pBuffer.GetAddressOf(), &stride, &offset);
		DC->IASetIndexBuffer(node->pMesh->GetIndexBuffer()->m_pBuffer.Get(), DXGI_FORMAT_R32_UINT, node->pMesh->GetIndexBuffer()->m_uiOffset);

		DC->VSSetConstantBuffers(0, 1, cbTransform->GetBuffer().GetAddressOf());
		DC->VSSetConstantBuffers(1, 1, cbCamera->GetBuffer().GetAddressOf());
		DC->PSSetConstantBuffers(2, 1, cbShowColor->GetBuffer().GetAddressOf());

		// PSSetSamplers
		DC->PSSetSamplers(0, 1, CORE.GetDefaultSamplerState().GetAddressOf());
		CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT);
		// DrawIndexed
		DC->DrawIndexed(node->pMesh->GetIndexBuffer()->m_Indices.size(), 0, 0);
	}

}

void Model::UpdateChildsInModelNodes(std::vector<std::shared_ptr<ModelNode>>& pModelNodes)
{
	for (int i = 0; i < pModelNodes.size(); i++)
	{
		for (int j = 0; j < pModelNodes.size(); j++)
		{
			if (pModelNodes[j]->parentIndex == i)
			{
				pModelNodes[i]->uiChildrenIndices.push_back(j);
			}
		}
	}

}

void Model::TranslateModel(const XMFLOAT3& pos)
{
	for (const auto& node : m_pModelNodes)
	{
		node->pTransform->SetWorldPosition(pos);
	}
}

void Model::RotateModel(const XMFLOAT3& rot)
{
	for (const auto& node : m_pModelNodes)
	{
		node->pTransform->SetWorldRotation(rot);
	}
}

void Model::ScaleModel(const XMFLOAT3& scale)
{
	for (const auto& node : m_pModelNodes)
	{
		node->pTransform->SetWorldScale(scale);
	}
}

void Model::ExportModelToBinary()
{
	filesystem::path exportPath("../Models/Binaries/");
	if (!filesystem::exists(exportPath))
	{
		filesystem::create_directories(exportPath);
	}

	std::string exportName = exportPath.string() + m_strName + ".bin"s;
	fstream fs;
	fs.open(exportName, ios::out | ios::binary);

	if (!fs.is_open()) __debugbreak();
	if (!fs.good()) __debugbreak();

	fs << "<Model>" << std::endl;

	fs << "<Model Name>" << std::endl;
	fs << m_strName << std::endl;
	fs << "</Model Name>" << std::endl;

	fs << "<Node Count>" << std::endl;
	size_t nNodes = m_pModelNodes.size();
	fs.write(reinterpret_cast<const char*>(&nNodes), sizeof(nNodes));
	fs << "</Node Count>" << std::endl;

	for (const auto& node : m_pModelNodes)
	{
		fs << *node;
	}

	fs << "</Model>" << std::endl;

}

void Model::PrintInfoToImGui()
{
	if (ImGui::BeginListBox("ModelNodes"))
	{
		for (int i = 0; i < m_pModelNodes.size(); i++)
		{
			const bool bSelected = (m_ItemSelected == i);
			if (ImGui::Selectable(m_pModelNodes[i]->strName.c_str(), bSelected))
				m_ItemSelected = i;

			if (m_ItemHighlighted && ImGui::IsItemHovered())
				m_ItemHighlightedIndex = i;

			if (bSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	// Transform Manipulator
	shared_ptr<ModelNode> pSelectedNode = m_pModelNodes[m_ItemSelected];

	if (ImGui::BeginTabBar("Transform", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("World"))
		{
			XMFLOAT3 pos = pSelectedNode->pTransform->GetWorldPosition();
			XMFLOAT3 rot = pSelectedNode->pTransform->GetWorldRotation();
			XMFLOAT3 scale = pSelectedNode->pTransform->GetWorldScale();

			XMVECTOR xmPos = XMLoadFloat3(&pos);
			XMVECTOR xmRot = XMLoadFloat3(&rot);
			XMVECTOR xmScale = XMLoadFloat3(&scale);

			ImGui::DragFloat("pos.x", &xmPos.m128_f32[0], 0.01f, -100.f, 100.f);
			ImGui::DragFloat("pos.y", &xmPos.m128_f32[1], 0.01f, -100.f, 100.f);
			ImGui::DragFloat("pos.z", &xmPos.m128_f32[2], 0.01f, -100.f, 100.f);

			ImGui::DragFloat("rot.x", &xmRot.m128_f32[0], 0.01f, -360.f, 360.f);
			ImGui::DragFloat("rot.y", &xmRot.m128_f32[1], 0.01f, -360.f, 360.f);
			ImGui::DragFloat("rot.z", &xmRot.m128_f32[2], 0.01f, -360.f, 360.f);

			ImGui::DragFloat("scale.x", &xmScale.m128_f32[0], 0.01f, 0.f, 10.f);
			ImGui::DragFloat("scale.y", &xmScale.m128_f32[1], 0.01f, 0.f, 10.f);
			ImGui::DragFloat("scale.z", &xmScale.m128_f32[2], 0.01f, 0.f, 10.f);

			XMStoreFloat3(&pos, xmPos);
			XMStoreFloat3(&rot, xmRot);
			XMStoreFloat3(&scale, xmScale);

			pSelectedNode->pTransform->SetWorldPosition(pos);
			pSelectedNode->pTransform->SetWorldRotation(rot);
			pSelectedNode->pTransform->SetWorldScale(scale);

			ImGui::EndTabItem();
		}
		
		if (ImGui::BeginTabItem("Local"))
		{
			XMFLOAT3 pos = pSelectedNode->pTransform->GetLocalPosition();
			XMFLOAT3 rot = pSelectedNode->pTransform->GetLocalRotation();
			XMFLOAT3 scale = pSelectedNode->pTransform->GetLocalScale();

			ImGui::Text("Local Transforms");
			ImGui::Text("Translation : (%f, %f, %f)", pos.x, pos.y, pos.z);
			ImGui::Text("Rotation : (%f, %f, %f)", rot.x, rot.y, rot.z);
			ImGui::Text("Scale : (%f, %f, %f)", scale.x, scale.y, scale.z);

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}


}
