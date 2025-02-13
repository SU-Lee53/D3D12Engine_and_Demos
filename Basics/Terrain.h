#pragma once


class TerrainRender : public RenderMethod
{
public:
	virtual ~TerrainRender() {}

public:
	virtual BOOL Initialize(std::shared_ptr<Object> owner) override;
	virtual void Render(std::shared_ptr<Camera> pCamera) override;

private:
	std::unique_ptr<ConstantBuffer<CBCameraData>> m_upCameraCBuffer = nullptr;
	std::unique_ptr<ConstantBuffer<CBLightData>> m_upLightCBuffer = nullptr;

	const UINT DESCRIPTOR_COUNT_FOR_TABLE_0 = 4;
	const UINT DESCRIPTOR_COUNT_FOR_TABLE_1 = 2;	// Diffuse, Normal

};

class Terrain
{
public:
	Terrain();
	~Terrain();

public:
	BOOL Initialize(std::wstring wstrDiffPath, std::wstring wstrHeightPath);


private:
	std::shared_ptr<Texture> m_pDiffuseTexture = nullptr;
	std::shared_ptr<Texture> m_pHeightTexture = nullptr;

	std::unique_ptr<Mesh<VertexType>> m_pTerrainMesh = nullptr;

	

};

