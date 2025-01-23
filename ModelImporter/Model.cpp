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

void Model::Render(shared_ptr<Camera> pCamera)
{
	// What we need
	// 1. Input Layout : in VertexType::desc
	// 2. Shaders : in node->pMaterial
	// 3. Rasterizer / Sampler / Blend State : TODO : DO IT
	// 4. Constant Buffer

	ConstantBuffer<CB_TRANSFORM> cbTransform;
	ConstantBuffer<CB_CAMERA> cbCamera;
	cbTransform.Initialize();
	cbCamera.Initialize();

	for (const auto& node : m_pModelNodes)
	{
		CB_TRANSFORM transformData = {};
		::ZeroMemory(&transformData, sizeof(transformData));
		{
			transformData.matLocal = XMMatrixTranspose(node->pTransform->GetLocalMatrix());
			transformData.matWorld = XMMatrixTranspose(node->pTransform->GetWorldMatrix());
		}
		cbTransform.PushData(transformData);

		CB_CAMERA cameraData = {};
		::ZeroMemory(&cameraData, sizeof(cameraData));
		{
			cameraData.matView = XMMatrixTranspose(pCamera->GetViewMatrix());
			cameraData.matProj = XMMatrixTranspose(pCamera->GetProjectionMatrix());
		}

		DC->IASetInputLayout(node->pMaterial->GetShader()->GetInputLayout().Get());
		DC->VSSetShader(node->pMaterial->GetShader()->GetVertexShader().Get(), nullptr, 0);
		// RSSetState
		DC->PSSetShader(node->pMaterial->GetShader()->GetPixelShader().Get(), nullptr, 0);
		// OMSetBlendState

		UINT stride = node->pMesh->GetVertexBuffer()->m_uiStride;
		UINT offset = node->pMesh->GetVertexBuffer()->m_uiOffset;
		DC->IASetVertexBuffers(0, 1, node->pMesh->GetVertexBuffer()->m_pBuffer.GetAddressOf(), &stride, &offset);
		DC->IASetIndexBuffer(node->pMesh->GetIndexBuffer()->m_pBuffer.Get(), DXGI_FORMAT_R32_UINT, node->pMesh->GetIndexBuffer()->m_uiOffset);

		DC->VSSetConstantBuffers(0, 1, cbTransform.GetBuffer().GetAddressOf());
		DC->VSSetConstantBuffers(1, 1, cbCamera.GetBuffer().GetAddressOf());

		// PSSetSamplers

		// DrawIndexed
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
