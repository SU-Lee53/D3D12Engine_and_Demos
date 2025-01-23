#include "stdafx.h"
#include "Model.h"
#include "algorithm"
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

void Model::Render()
{
	// What we need
	// 1. Input Layout : in VertexType::desc
	// 2. Shaders : in node->pMaterial
	// 3. Rasterizer / Sampler / Blend State : TODO : DO IT
	// 4. Constant Buffer
	for (const auto& node : m_pModelNodes)
	{

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
