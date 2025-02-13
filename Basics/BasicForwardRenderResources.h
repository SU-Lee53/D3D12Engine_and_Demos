#pragma once

namespace BasicForward
{
	// Basic Forward Rendering Root Signature (Without Texture)
	class BasicForwardRootSignature : public RootSignature
	{
	public:
		virtual BOOL Initialize() override;

	};

	class BasicForwardPipeline : public Pipeline
	{
	public:
		virtual BOOL Initialize(std::shared_ptr<class RootSignature> rootSignature) override;

	};

	// Basic Forward rendering RenderMethod
	// PassCount = 0, RTVCount = 0
	class ForwardRender : public RenderMethod
	{
	public:
		virtual BOOL Initialize(std::shared_ptr<Object> owner) override;
		virtual void Render(std::shared_ptr<Camera> pCamera) override;

	private:
		std::unique_ptr<ConstantBuffer<CBTransformData>> m_upTransformCBuffer = nullptr;
		std::unique_ptr<ConstantBuffer<CBCameraData>> m_upCameraCBuffer = nullptr;

		const UINT DESCRIPTOR_COUNT_FOR_DRAW = 2;
	};

	class BasicForwardObject : public Object
	{
	public:
		virtual BOOL Initialize() override;
		virtual void Update() override;
		virtual void Render(std::shared_ptr<Camera> pCamera) override;

	private:
		BOOL InitRenderMethod();

	public:
		std::shared_ptr<Transform>& GetTransform() { return m_pTransform; }
		std::shared_ptr<Mesh<BasicVertexType>>& GetMesh() { return m_pMesh; }


	protected:
		std::shared_ptr<Transform> m_pTransform = nullptr;

		std::shared_ptr<Mesh<BasicVertexType>> m_pMesh = nullptr;
		std::shared_ptr<RenderMethod> m_pRenderMethod = nullptr;

	};

}
