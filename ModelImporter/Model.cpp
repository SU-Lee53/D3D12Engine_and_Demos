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
