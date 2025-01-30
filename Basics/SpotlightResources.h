#pragma once

class SpotlightRootSignature : public RootSignature
{
public:
	virtual BOOL Initialize() override;

};

class SpotlightPipeline : public Pipeline
{
public:
	virtual BOOL Initialize(std::shared_ptr<class RootSignature> rootSignature) override;

};

struct CBModelTransformData
{
	XMFLOAT4X4 matLocal;
	XMFLOAT4X4 matWorld;
};

struct CBColorData
{
	XMFLOAT4 colorDiffuse;
	XMFLOAT4 colorSpecular;
	XMFLOAT4 colorAmbient;
	XMFLOAT4 colorEmissive;
};

struct CBSpotlightData
{
	XMFLOAT3 lightPos;
	float pad1;
	XMFLOAT3 lightDir;
	float pad2;
	XMFLOAT3 lightColor;
	float pad3;

	float lightIntensity;
	float innerCone;	// cos(еш) of inner cone 
	float outerCone;	// cos(еш) of outer cone 
	float lightRange;
	float attenuation;
};

class SpotlightRender : public RenderMethod
{
public:
	virtual ~SpotlightRender() {}

public:
	virtual BOOL Initialize(std::shared_ptr<Object> owner) override;
	virtual void Render() override;

private:
	std::unique_ptr<ConstantBuffer<CBModelTransformData>> m_upTransformCBuffer = nullptr;
	std::unique_ptr<ConstantBuffer<CBCameraData>> m_upCameraCBuffer = nullptr;
	std::unique_ptr<ConstantBuffer<CBColorData>> m_upColorCBuffer = nullptr;
	std::unique_ptr<ConstantBuffer<CBSpotlightData>> m_upSpotlightCBuffer = nullptr;

	const UINT DESCRIPTOR_COUNT_FOR_DRAW = 4;
};

#include "Mesh.h"

struct ColorData
{
	XMFLOAT4 colorDiffuse;
	XMFLOAT4 colorSpecular;	// a is for Specular Pow (Shiness)
	XMFLOAT4 colorAmbient;
	XMFLOAT4 colorEmissive;

	CBColorData GetMaterialCBData()
	{
		return CBColorData
		{
			colorDiffuse,
			colorSpecular,
			colorAmbient,
			colorEmissive
		};
	}
};

class SpotlightObject : public Object
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

	void SetDiffuseColor(const XMFLOAT3& color) { m_upColorData->colorDiffuse = XMFLOAT4(color.x, color.y, color.z, 1.f); }
	void SetSpecularColor(const XMFLOAT3& color) { m_upColorData->colorSpecular = XMFLOAT4(color.x, color.y, color.z, m_upColorData->colorSpecular.w); }
	void SetAmbientColor(const XMFLOAT3& color) { m_upColorData->colorAmbient = XMFLOAT4(color.x, color.y, color.z, 1.f); }
	void SetShiness(float shiness) { m_upColorData->colorSpecular.w = shiness; }

	XMFLOAT3 GetDiffuseColor()
	{
		XMVECTOR xmDiffuse = XMLoadFloat4(&m_upColorData->colorDiffuse);
		XMFLOAT3 ret;
		XMStoreFloat3(&ret, xmDiffuse);
		return ret;
	}

	XMFLOAT3 GetSpecularColor()
	{
		XMVECTOR xmSpecular = XMLoadFloat4(&m_upColorData->colorSpecular);
		XMFLOAT3 ret;
		XMStoreFloat3(&ret, xmSpecular);
		return ret;
	}

	XMFLOAT3 GetAmbientColor()
	{
		XMVECTOR xmAmbient = XMLoadFloat4(&m_upColorData->colorAmbient);
		XMFLOAT3 ret;
		XMStoreFloat3(&ret, xmAmbient);
		return ret;
	}

	float GetShiness() { return m_upColorData->colorSpecular.w; }

	friend class SpotlightRender;

protected:
	std::unique_ptr<Transform> m_upTransform = nullptr;
	std::unique_ptr<ColorData> m_upColorData = nullptr;
	std::unique_ptr<Mesh<VertexType>> m_upMesh = nullptr;
	std::unique_ptr<RenderMethod> m_upRenderMethod = nullptr;

};