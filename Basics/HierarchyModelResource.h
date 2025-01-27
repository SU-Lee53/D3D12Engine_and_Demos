#pragma once


class HierarchyModelRootSignature : public RootSignature
{
public:
	virtual BOOL Initialize() override;

};

class HierarchyModelPipeline : public Pipeline
{
public:
	virtual BOOL Initialize(std::shared_ptr<class RootSignature> rootSignature) override;

};

struct CBModelTransformData
{
	Matrix matLocal;
	Matrix matWorld;
};

class HierarchyModelRender : public RenderMethod
{
public:
	virtual void Initialize(std::shared_ptr<Object> owner) override;
	virtual void Render() override;

private:
	std::unique_ptr<ConstantBuffer<CBModelTransformData>> m_upTransformCBuffer = nullptr;
	std::unique_ptr<ConstantBuffer<CBCameraData>> m_upCameraCBuffer = nullptr;

	const UINT DESCRIPTOR_COUNT_FOR_DRAW = 2;
};

class HierarchyModelObject : public Object
{
public:
	virtual BOOL Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	BOOL InitRenderMethod();

};