#pragma once

// ��� �������Ұ��ΰ� 
// MRT �� ���� Shader-Pass �� RenderMethod Ŭ������ ����
// RenderMethod �� ��ӹ޾Ƽ� ���� ������ ��� �� ������� �����ϵ��� ��
// RenderManager �� RenderMethod::Render �� �̿��Ͽ� �������� ����

// �ϴ� owner �� Object �� ��
// �� Mesh �� �ٸ� ���𰡷� �ٲ�Ű��� ��

// TODO : ����

#include "ConstantBuffer.h"

class Object;
class Pipeline;
class RootSignature;
class Camera;

class RenderMethod
{
public:
	virtual ~RenderMethod() {}

public:
	virtual BOOL Initialize(std::shared_ptr<Object> pOwner) = 0;
	virtual void Render(std::shared_ptr<Camera> pCamera) = 0;

protected:
	std::weak_ptr<Object> m_wpOwner;
	
	std::vector<std::shared_ptr<RootSignature>>	m_RootSignatures = {};
	std::vector<std::shared_ptr<Pipeline>>		m_Pipelines = {};
	std::vector<ComPtr<ID3D12Resource>>			m_RTVs = {};
	DWORD m_dwPassCount = 0;
	DWORD m_dwRTVCount = 0;

	std::unique_ptr<DescriptorHeap> m_upDescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC		m_HeapDesc = {};	// Cache

};
