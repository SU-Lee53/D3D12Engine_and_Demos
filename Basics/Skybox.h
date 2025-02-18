#pragma once
#include "Object.h"

class SkyboxRootSignaturePass1 : public RootSignature
{
public:
	BOOL Initialize() override;
};

class SkyboxRootSignaturePass2 : public RootSignature
{
public:
	BOOL Initialize() override;
};

class SkyboxPipelinePass1 : public Pipeline
{
public:
	BOOL Initialize(std::shared_ptr<class RootSignature> rootSignature) override;
};

class SkyboxPipelinePass2 : public Pipeline
{
public:
	BOOL Initialize(std::shared_ptr<class RootSignature> rootSignature) override;
};

class SkyboxRender : public RenderMethod
{
public:
	BOOL Initialize(std::shared_ptr<Object> pOwner) override;
	void Render(std::shared_ptr<Camera> pCamera) override;

private:
	void ChangeResourceStateToRenderTarget(D3D12_RESOURCE_STATES currentState);
	void Pass1(std::shared_ptr<Camera> pCamera = nullptr);	// We dont need Camera
	void ChangeResourceStateToShaderResource(D3D12_RESOURCE_STATES currentState);
	void Pass2(std::shared_ptr<Camera> pCamera);

private:
	std::unique_ptr<ConstantBuffer<CBCameraData>> m_upPass2CameraCBuffer = nullptr;

	std::unique_ptr<DescriptorHeap> m_upDescriptorHeapForPass1 = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC		m_HeapDescForPass1 = {};	// Cache

	const UINT DESCRIPTOR_COUNT_FOR_PASS_1 = 1;	// origin
	const UINT DESCRIPTOR_COUNT_FOR_PASS_2 = 2;	// Camera + cubemap

};

struct SkyboxCubeMap
{
	// Functions
	SkyboxCubeMap();
	~SkyboxCubeMap();

	BOOL Initialize(const std::string& strTextureName, UINT uiCubeMapSize = 512, UINT uiMipLevels = 0);

	void ChangeResourceStateToRenderTarget(D3D12_RESOURCE_STATES currentState);
	void ChangeResourceStateToShaderResource(D3D12_RESOURCE_STATES currentState = D3D12_RESOURCE_STATE_RENDER_TARGET);

	// Variables
	std::unique_ptr<Mesh<CubemapVertexType>> m_upQuadMesh = nullptr;
	XMFLOAT4X4 m_ViewMatrices[6] = {};
	XMFLOAT4X4 m_ProjMatrix = {};

	std::shared_ptr<Texture> m_pCubeMapTexture = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_pRTVDescriptorHeap = nullptr;
	UINT m_uiRTVDescriptorSize = 0;

	UINT m_uiCubeMapSize = 0;
	UINT m_uiMipLevels = 0;

	// For ResourceBarrier
	D3D12_RESOURCE_STATES m_CurrentResourceState;
};

class Skybox : public Object
{
public:
	Skybox();
	virtual ~Skybox();

public:
	BOOL Initialize(std::wstring wstrTexturePath);

	BOOL Initialize() override;
	void Update() override;
	void Render(std::shared_ptr<Camera> pCamera) override;

private:
	BOOL InitRenderMethod();

	friend class SkyboxRender;

private:
	std::shared_ptr<Texture> m_pSkyboxTexture = nullptr;
	std::shared_ptr<SkyboxCubeMap> m_pCubeMap = nullptr;

	std::unique_ptr<Mesh<SkyboxVertexType>> m_upSkyboxMesh = nullptr;

	std::unique_ptr<RenderMethod> m_upRenderMethod = nullptr;
};

