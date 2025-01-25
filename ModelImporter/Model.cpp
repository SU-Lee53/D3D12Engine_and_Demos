#include "stdafx.h"
#include "Model.h"
#include "ConstantBuffer.h"
#include "Camera.h"

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
	cbTransform->Initialize();
	cbCamera->Initialize();

	for (const auto& node : m_pModelNodes)
	{
		CB_TRANSFORM transformData = {};
		::ZeroMemory(&transformData, sizeof(transformData));
		{
			XMStoreFloat4x4(&transformData.matLocal, XMMatrixTranspose(XMLoadFloat4x4(&node->pTransform->GetLocalMatrix())));
			XMStoreFloat4x4(&transformData.matWorld, XMMatrixTranspose(XMLoadFloat4x4(&node->pTransform->GetWorldMatrix())));
		}
		cbTransform->PushData(transformData);

		CB_CAMERA cameraData = {};
		::ZeroMemory(&cameraData, sizeof(cameraData));
		{
			XMStoreFloat4x4(&cameraData.matView, XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix())));
			XMStoreFloat4x4(&cameraData.matProj, XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetProjectionMatrix())));
		}
		cbCamera->PushData(cameraData);

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
