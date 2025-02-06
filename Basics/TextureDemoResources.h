#pragma once

#include "Mesh.h"
#include "Texture.h"

// namespace Texture can overlap class name
namespace TextureDemoResource
{
	class TextureDemoRootSignature : public RootSignature
	{
	public:
		virtual BOOL Initialize() override;

	};

	class TextureDemoPipeline : public Pipeline
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


	class TextureDemoRender : public RenderMethod
	{
	public:
		virtual ~TextureDemoRender() {}

	public:
		virtual BOOL Initialize(std::shared_ptr<Object> owner) override;
		virtual void Render() override;

	private:
		std::unique_ptr<ConstantBuffer<CBModelTransformData>> m_upTransformCBuffer = nullptr;
		std::unique_ptr<ConstantBuffer<CBCameraData>> m_upCameraCBuffer = nullptr;
		std::unique_ptr<ConstantBuffer<CBColorData>> m_upColorCBuffer = nullptr;

		const UINT DESCRIPTOR_COUNT_FOR_DRAW = 4;	// Texture use one
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

	struct ModelNode
	{
		std::string strName;

		std::shared_ptr<Mesh<VertexType>> pMesh;
		std::shared_ptr<Transform> pTransform;
		std::shared_ptr<ColorData> pColorData;
		std::shared_ptr<Texture> pTexture;

		int parentIndex = -1;	// -1 means Root is Parent
		std::vector<UINT> uiChildrenIndices = {};


		ModelNode()
		{
			pMesh = std::make_shared<Mesh<VertexType>>();
			pTransform = std::make_shared<Transform>();
			pColorData = std::make_shared<ColorData>();
			pTexture = std::make_shared<Texture>();
		}

		void Update()
		{
			pTransform->Update();
		}

		friend std::istream& operator>>(std::istream& is, ModelNode& node)
		{
			std::string read;
			while (read != "</Node>")
			{
				std::getline(is, read);

				if (read == "<Frame Name>")
				{
					std::getline(is, node.strName);
				}

				if (read == "<Parent Index>")
				{
					is.read(reinterpret_cast<char*>(&node.parentIndex), sizeof(node.parentIndex));
				}

				if (read == "<Children Count>")
				{
					size_t nChildren = 0;
					is.read(reinterpret_cast<char*>(&nChildren), sizeof(nChildren));
					node.uiChildrenIndices.resize(nChildren);
				}

				if (read == "<Children Index>")
				{
					is.read(reinterpret_cast<char*>(node.uiChildrenIndices.data()), sizeof(node.uiChildrenIndices[0]) * node.uiChildrenIndices.size());
				}

				if (read == "<Transform>")
				{
					while (read != "</Transform>")
					{
						XMFLOAT3 in;
						std::getline(is, read);
						if (read == "<Local>")
						{
							is.read(reinterpret_cast<char*>(&in), sizeof(in));
							node.pTransform->SetLocalPosition(in);
							is.read(reinterpret_cast<char*>(&in), sizeof(in));
							node.pTransform->SetLocalRotation(in);
							is.read(reinterpret_cast<char*>(&in), sizeof(in));
							node.pTransform->SetLocalScale(in);
						}

						if (read == "<World>")
						{
							is.read(reinterpret_cast<char*>(&in), sizeof(in));
							node.pTransform->SetWorldPosition(in);
							is.read(reinterpret_cast<char*>(&in), sizeof(in));
							node.pTransform->SetWorldRotation(in);
							is.read(reinterpret_cast<char*>(&in), sizeof(in));
							node.pTransform->SetWorldScale(in);
						}
					}

					node.pTransform->Initialize();
					node.pTransform->Update();
				}

				if (read == "<Mesh>")
				{
					size_t nVertices = 0, nIndices = 0;
					std::vector<VertexType> vtx = {};
					std::vector<UINT> idx = {};

					while (read != "</Mesh>")
					{
						std::getline(is, read);
						if (read == "<Vertex Data>")
						{
							while (read != "</Vertex Data>")
							{
								std::getline(is, read);
								if (read == "<Vertex Size>")
								{
									is.read(reinterpret_cast<char*>(&nVertices), sizeof(nVertices));
									vtx.resize(nVertices);
								}
								if (read == "<Vertex>")
								{
									is.read(reinterpret_cast<char*>(vtx.data()), sizeof(vtx[0]) * nVertices);
								}
							}
						}

						if (read == "<Index Data>")
						{
							while (read != "</Index Data>")
							{
								std::getline(is, read);
								if (read == "<Index Size>")
								{
									is.read(reinterpret_cast<char*>(&nIndices), sizeof(nIndices));
									idx.resize(nIndices);
								}
								if (read == "<Index>")
								{
									is.read(reinterpret_cast<char*>(idx.data()), sizeof(idx[0]) * nIndices);
								}
							}
						}
					}

					node.pMesh->Initialize(vtx, idx);
				}

				if (read == "<Color Data>")
				{
					while (read != "</Color Data>")
					{
						std::getline(is, read);
						if (read == "<Diffuse>")
						{
							is.read(reinterpret_cast<char*>(&node.pColorData->colorDiffuse), sizeof(node.pColorData->colorDiffuse));
						}
						if (read == "<Specular>")
						{
							is.read(reinterpret_cast<char*>(&node.pColorData->colorSpecular), sizeof(node.pColorData->colorSpecular));
						}
						if (read == "<Ambient>")
						{
							is.read(reinterpret_cast<char*>(&node.pColorData->colorAmbient), sizeof(node.pColorData->colorAmbient));
						}
						if (read == "<Emissive>")
						{
							is.read(reinterpret_cast<char*>(&node.pColorData->colorEmissive), sizeof(node.pColorData->colorEmissive));
						}
					}
				}

				// Texture
				if (read == "<Texture>")
				{
					while (read != "</Texture>")
					{
						std::getline(is, read);
						if (read == "<Texture Path>")
						{
							std::string strFilename = "";
							std::getline(is, strFilename);
							std::string strSavePathBase = "../Models/Texture/"s;
							std::string strTextureFilePath = strSavePathBase + strFilename;

							std::filesystem::path fsFilePath(strTextureFilePath);

							TEXTURE.CreateAndAddTexuture(fsFilePath.stem().string(), std::wstring(strTextureFilePath.begin(), strTextureFilePath.end()));
							node.pTexture = TEXTURE.GetTexture(fsFilePath.stem().string());
						}
					}
				}

			}

			return is;
		}
	};

	class TextureDemoObject : public Object
	{
	public:
		virtual BOOL Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

	private:
		BOOL InitRenderMethod();

	public:
		void LoadFromBinaryFile(std::wstring filePath);

	public:
		std::unique_ptr<Transform>& GetTransform() { return m_upTransform; }

		friend class TextureDemoRender;

	protected:
		std::unique_ptr<Transform> m_upTransform = nullptr;

		std::unique_ptr<RenderMethod> m_upRenderMethod = nullptr;
		std::vector<std::shared_ptr<ModelNode>> m_pModelNodes = {};
		std::string m_strName = "";

		UINT m_nInstances = 0;

	};
}