#include "stdafx.h"
#include "Mesh.h"

using namespace std;

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

BOOL Mesh::Initialize(const std::vector<VertexType>& vertices, const std::vector<UINT>& indices)
{
	m_upVertexBuffer = make_unique<VertexBuffer>();
	m_upIndexBuffer = make_unique<IndexBuffer>();

	m_upVertexBuffer->Initialize(vertices);
	m_upIndexBuffer->Initialize(indices);

	return TRUE;
}

