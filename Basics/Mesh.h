#pragma once

#include "input_types.h"
#include "MeshHelper.h"

template<ShaderInputType T>
class Mesh
{
public:
	Mesh();
	~Mesh();

public:
	// Currently test function 
	BOOL Initialize(const std::vector<T>& vertices, const std::vector<UINT>& indices);


public:
	void SetVertices(const std::vector<T>& vertices) { m_Vertices = vertices; }
	void SetIndices(const std::vector<UINT>& indices) { m_Indices = indices; }

	std::shared_ptr<MeshBuffer>& GetBuffer() { return m_pMeshBuffer; }


private:
	std::shared_ptr<MeshBuffer> m_pMeshBuffer = nullptr;
	
	// Buffer Cache
	std::vector<T> m_Vertices = {};
	std::vector<UINT> m_Indices = {};

};

template<ShaderInputType T>
inline Mesh<T>::Mesh()
{
}

template<ShaderInputType T>
inline Mesh<T>::~Mesh()
{
}

template<ShaderInputType T>
inline BOOL Mesh<T>::Initialize(const std::vector<T>& vertices, const std::vector<UINT>& indices)
{
	// TODO : Create VTX/IDX buffer

	// cube mesh for test
	MeshHelper::CreateBoxMesh(vertices, indices);

	ThrowIfFailed(RESOURCE.CreateVertexBuffer(vertices, m_pMeshBuffer->vertexBuffer));
	ThrowIfFailed(RESOURCE.CreateIndexBuffer(indices, m_pMeshBuffer->indexBuffer));


	return TRUE;

}
