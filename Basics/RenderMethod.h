#pragma once

// 어떻게 렌더링할것인가 
// MRT 와 다중 Shader-Pass 를 RenderMethod 클래스로 묶음
// RenderMethod 를 상속받아서 여러 렌더링 방법 및 기술들을 구현하도록 함
// RenderManager 는 RenderMethod::Render 를 이용하여 렌더링을 수행

// 일단 owner 를 Object 로 둠
// 곧 Mesh 등 다른 무언가로 바뀔거같긴 함

// TODO : 구현

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
