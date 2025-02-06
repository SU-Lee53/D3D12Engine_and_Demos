#pragma once

#define INSTANCED_COUNT 2000
#define MAX_POSITION 10.f

namespace Instancing
{
	class InstancingRootSignature : public RootSignature
	{
	public:
		virtual BOOL Initialize() override;

	};

	class InstancingPipeline : public Pipeline
	{
	public:
		virtual BOOL Initialize(std::shared_ptr<class RootSignature> rootSignature) override;

	};

	struct SBInstancingData
	{
		XMFLOAT4X4 matWorld;
		XMFLOAT4 colorDiffuse;
	};

	class InstancingRender : public RenderMethod
	{
	public:
		virtual ~InstancingRender() {}

	public:
		virtual BOOL Initialize(std::shared_ptr<Object> owner) override;
		virtual void Render() override;

	private:
		std::unique_ptr<ConstantBuffer<CBCameraData>> m_upCameraCBuffer = nullptr;
		std::unique_ptr<StructuredBuffer<SBInstancingData, INSTANCED_COUNT>> m_upInstancingSBuffer = nullptr;

		const UINT DESCRIPTOR_COUNT_FOR_DRAW = 2;
	};

#include "Mesh.h"

	class InstancingObject : public Object
	{
	public:
		virtual BOOL Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

	private:
		BOOL InitRenderMethod();

	public:
		std::unique_ptr<Transform>& GetTransform() { return m_upTransform; }
		std::unique_ptr<Mesh<VertexType>>& GetMesh() { return m_upMesh; }

		friend class InstancingRender;

	protected:
		std::unique_ptr<Transform> m_upTransform = nullptr;
		std::vector<SBInstancingData> m_InstancingDatas = {};

		std::unique_ptr<Mesh<VertexType>> m_upMesh = nullptr;

		std::unique_ptr<RenderMethod> m_upRenderMethod = nullptr;

		UINT m_nInstances = 0;

	};
}
