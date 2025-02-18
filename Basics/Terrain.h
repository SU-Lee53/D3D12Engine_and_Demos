#pragma once

class TerrainRootSignature : public RootSignature
{
public:
	virtual BOOL Initialize() override;

};

class TerrainPipeline : public Pipeline
{
public:
	virtual BOOL Initialize(std::shared_ptr<class RootSignature> rootSignature) override;

};

class TerrainRender : public RenderMethod
{
public:
	virtual ~TerrainRender() {}

public:
	virtual BOOL Initialize(std::shared_ptr<Object> owner) override;
	virtual void Render(std::shared_ptr<Camera> pCamera) override;

private:
	std::unique_ptr<ConstantBuffer<XMFLOAT4X4>> m_upWorldTransformCBuffer = nullptr;
	std::unique_ptr<ConstantBuffer<CBCameraData>> m_upCameraCBuffer = nullptr;

	const UINT DESCRIPTOR_COUNT_FOR_TABLE_0 = 2;
	const UINT DESCRIPTOR_COUNT_FOR_TABLE_1 = 1;	// Diffuse, Normal

};

struct TerrainHeightmap
{
public:
	// functions
	TerrainHeightmap();
	~TerrainHeightmap();

	BOOL Initialize(std::wstring wstrFilePath);

public:
	// variables
	DirectX::ScratchImage m_HeightMapData = {};

	UINT m_uiWidth = 0;
	UINT m_uiHeight = 0;

};

class Terrain : public Object
{
public:
	Terrain();
	virtual ~Terrain();

public:
	BOOL Initialize(std::wstring wstrDiffPath, std::wstring wstrHeightPath);

	virtual BOOL Initialize() override;
	virtual void Update() override;
	virtual void Render(std::shared_ptr<Camera> pCamera) override;

private:
	BOOL InitRenderMethod();

private:
	void AdjustTerrainHeight(std::vector<TerrainVertexType>& vtx);

	friend class TerrainRender;

private:
	std::shared_ptr<Texture> m_pDiffuseTexture = nullptr;
	std::shared_ptr<TerrainHeightmap> m_pTerrainHeightmap = nullptr;
	std::unique_ptr<Mesh<TerrainVertexType>> m_pTerrainMesh = nullptr;
	std::unique_ptr<RenderMethod> m_upRenderMethod = nullptr;
	
	XMFLOAT4X4 m_matWorld;

};

