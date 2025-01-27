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
		os << *mesh.m_upVertexBuffer;
		os << *mesh.m_upIndexBuffer;
		os << "</Mesh>" << std::endl;

		return os;
	}

	friend std::istream& operator>>(std::istream& is, Mesh& mesh)
	{
		std::string read;
		while (read != "</Mesh>")
		{
			std::getline(is, read);

			if (read == "<Vertex Data>")
			{
				mesh.m_upVertexBuffer = std::make_unique<VertexBuffer>();
				is >> *mesh.m_upVertexBuffer;
			}

			if (read == "<Index Data>")
			{
				mesh.m_upIndexBuffer = std::make_unique<IndexBuffer>();
				is >> *mesh.m_upIndexBuffer;
			}
		}

		return is;
	}

};

