#pragma once

struct UploadBuffer
{

};

struct Descriptor
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};


struct VertexBuffer
{
	ComPtr<ID3D12Resource> pVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
};

struct IndexBuffer
{
	ComPtr<ID3D12Resource> pIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
};

struct MeshBuffer
{
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	UINT numElements;
};




