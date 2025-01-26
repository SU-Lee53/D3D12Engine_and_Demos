#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

public:
	BOOL Initialize(const std::vector<VertexType>&vertices, const std::vector<UINT>&indices);

	std::unique_ptr<VertexBuffer>& GetVertexBuffer() { return m_upVertexBuffer; }
	std::unique_ptr<IndexBuffer>& GetIndexBuffer() { return m_upIndexBuffer; }

private:
	std::unique_ptr<VertexBuffer> m_upVertexBuffer;
	std::unique_ptr<IndexBuffer> m_upIndexBuffer;

public:
	friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh)
	{
		os << "<Mesh>" << std::endl;
		os << *mesh.m_upVertexBuffer << std::endl;
		os << *mesh.m_upIndexBuffer << std::endl;
		os << "</Mesh>" << std::endl;

		return os;
	}
};

