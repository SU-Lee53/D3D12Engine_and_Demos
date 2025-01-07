#include "pch.h"
#include "RootSignature.h"

using namespace std;

RootSignature::RootSignature()
{
}

RootSignature::~RootSignature()
{
}

BOOL RootSignature::Initialize(vector<D3D12_ROOT_PARAMETER> rootParameters)
{
	return TRUE;
}
