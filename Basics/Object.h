#pragma once

#include "Mesh.h"

class Transform;
class RenderMethod;
class Model;

// 필요와 경우에 따라 상속받아 사용
class Object : public std::enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

	virtual BOOL Initialize();
	virtual void Update();
	virtual void Render();

public:
	ComPtr<ID3D12RootSignature>& GetRootSignature() { return m_pRootSignature; }
	std::shared_ptr<Transform>& GetTransform() { return m_pTransform; }
	std::shared_ptr<Mesh<VertexType>>& GetMesh() { return m_pMesh; }


protected:
	ComPtr<ID3D12RootSignature> m_pRootSignature = nullptr;

	std::shared_ptr<Transform> m_pTransform = nullptr;

	// TODO : add mesh(model), material, etc...
	//std::shared_ptr<Model> m_pModel;
	std::shared_ptr<Mesh<VertexType>> m_pMesh = nullptr;
	std::shared_ptr<RenderMethod> m_pRenderMethod = nullptr;


};


class BasicForwardObject : public Object
{
public:
	BasicForwardObject();
	virtual ~BasicForwardObject();

	virtual BOOL Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	BOOL InitRootSignatrue();
	BOOL InitRenderMethod();

};
