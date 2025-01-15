#pragma once


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
	virtual void Initialize(std::shared_ptr<Object> owner) override;
	virtual void Render() override;

private:
	std::unique_ptr<ConstantBuffer<CBTranformData>> m_upTransformCBuffer = nullptr;
	std::unique_ptr<ConstantBuffer<CBCameraData>> m_upCameraCBuffer = nullptr;

	const UINT DESCRIPTOR_COUNT_FOR_DRAW = 2;
};

class BasicForwardObject : public Object
{
public:
	virtual BOOL Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	BOOL InitRenderMethod();

};
