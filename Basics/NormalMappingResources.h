#pragma once

#include "Mesh.h"
#include "Texture.h"

// namespace Texture can overlap class name
namespace NormalMapping
{
	class NormalMappingRootSignature : public RootSignature
	{
	public:
		virtual BOOL Initialize() override;

	};

	class NormalMappingPipeline : public Pipeline
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


	class NormalMappingRender : public RenderMethod
	{
	public:
		virtual ~NormalMappingRender() {}

	public:
		virtual BOOL Initialize(std::shared_ptr<Object> owner) override;
		virtual void Render() override;

	private:
		std::unique_ptr<ConstantBuffer<CBModelTransformData>> m_upTransformCBuffer = nullptr;
		std::unique_ptr<ConstantBuffer<CBCameraData>> m_upCameraCBuffer = nullptr;
		std::unique_ptr<ConstantBuffer<CBColorData>> m_upColorCBuffer = nullptr;
		std::unique_ptr<ConstantBuffer<CBLightData>> m_upLightCBuffer = nullptr;

		const UINT DESCRIPTOR_COUNT_FOR_TABLE_0 = 4;
		const UINT DESCRIPTOR_COUNT_FOR_TABLE_1 = 2;	// Diffuse, Normal
	};

	struct ColorData
	{
		XMFLOAT4 colorDiffuse;
		XMFLOAT4 colorSpecular;
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

	class NormalMappingObject : public Object
	{
	public:
		virtual BOOL Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

	private:
		BOOL InitRenderMethod();

	public:
		std::unique_ptr<Transform>& GetTransform() { return m_upTransform; }

		friend class NormalMappingRender;

	protected:
		std::unique_ptr<Transform> m_upTransform = nullptr;
		std::unique_ptr<ColorData> m_upColorData = nullptr;
		std::unique_ptr<Mesh<VertexType>> m_upMesh = nullptr;
		std::unique_ptr<RenderMethod> m_upRenderMethod = nullptr;

		std::shared_ptr<Texture> m_upDiffuseTexture = nullptr;
		std::shared_ptr<Texture> m_upNormalTexture = nullptr;

		std::string m_strName = "";

		UINT m_nInstances = 0;

	};
}