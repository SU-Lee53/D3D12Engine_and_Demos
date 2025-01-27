#pragma once

#include "Mesh.h"

class Transform;
class RenderMethod;
class Model;

// �ʿ�� ��쿡 ���� ��ӹ޾� ���
class Object : public std::enable_shared_from_this<Object>
{
public:
	virtual BOOL Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

public:
	std::shared_ptr<Transform>& GetTransform() { return m_pTransform; }
	std::shared_ptr<Mesh<BasicVertexType>>& GetMesh() { return m_pMesh; }


protected:
	std::shared_ptr<Transform> m_pTransform = nullptr;

	// TODO : add mesh(model), material, etc...
	//std::shared_ptr<Model> m_pModel;	-> for future
	std::shared_ptr<Mesh<BasicVertexType>> m_pMesh = nullptr;
	std::shared_ptr<RenderMethod> m_pRenderMethod = nullptr;


};