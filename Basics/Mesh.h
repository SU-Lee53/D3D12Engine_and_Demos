#pragma once

#include "input_types.h"

template<ShaderInputType T>
class Mesh
{
public:
	Mesh();
	~Mesh();

public:
	BOOL Initialize(const std::vector<T>& vertices, const std::vector<UINT> indices);

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
inline BOOL Mesh<T>::Initialize(const std::vector<T>& vertices, const std::vector<UINT> indices)
{
	// TODO : Create VTX/IDX buffer
}
