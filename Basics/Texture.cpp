#include "pch.h"
#include "Texture.h"
#include <filesystem>

using namespace std;

Texture::Texture()
{

}

Texture::~Texture()
{

}

BOOL Texture::InitializeAsDefault(std::wstring wstrPath)
{
	std::filesystem::path p(wstrPath);
	m_strPath = p.string();

	D3D12_RESOURCE_DESC desc = {};
	BOOL bResult = FALSE;

	if (p.extension().string() == ".dds"s)
	{
		bResult = LoadFromDDSFile(wstrPath, desc);
	}
	else
	{
		bResult = LoadFromWICFile(wstrPath, desc);
	}

	if (bResult)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		{
			heapDesc.NumDescriptors = 1;
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		}

		// Create Descriptor Heap
		m_upDescriptorHeap = make_unique<DescriptorHeap>();
		m_upDescriptorHeap->Initialize(heapDesc);

		// Create Shader Resource View
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;

		DEVICE->CreateShaderResourceView(m_pTexResource.Get(), &srvDesc, m_upDescriptorHeap->DescriptorHandleFromStart.cpuHandle);

		m_SRV = srvDesc.Buffer;
	}
	else
	{
		return bResult;
	}

	return bResult;
}

BOOL Texture::InitializeAsBlankCubemap(UINT uiCubeMapSize, UINT uiMipLevels)
{
	D3D12_RESOURCE_DESC cubeDesc = {};
	cubeDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	cubeDesc.Alignment = 0;
	cubeDesc.Width = uiCubeMapSize;
	cubeDesc.Height = uiCubeMapSize;
	cubeDesc.DepthOrArraySize = 6;	// Cube map has 6 sides
	cubeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	cubeDesc.SampleDesc.Count = 1;
	cubeDesc.SampleDesc.Quality = 0;
	cubeDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	cubeDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	cubeDesc.MipLevels = 1;

	// Clear value
	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = cubeDesc.Format;
	clearValue.Color[0] = 0.f;
	clearValue.Color[1] = 0.f;
	clearValue.Color[2] = 0.f;
	clearValue.Color[3] = 1.f;

	// Create Default heap (Not Upload Heap)
	HRESULT hr = DEVICE->CreateCommittedResource(
	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		& cubeDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,
		IID_PPV_ARGS(&m_pTexResource));

	if (FAILED(hr))
	{
		OutputDebugStringA("Failed to create cubemap");
		__debugbreak();
		return FALSE;
	}
	else
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		{
			heapDesc.NumDescriptors = 1;
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		}

		// Create Descriptor Heap
		m_upDescriptorHeap = make_unique<DescriptorHeap>();
		m_upDescriptorHeap->Initialize(heapDesc);

		// Create Shader Resource View For Pass 2
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = cubeDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = cubeDesc.MipLevels;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.f;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		DEVICE->CreateShaderResourceView(m_pTexResource.Get(), &srvDesc, m_upDescriptorHeap->DescriptorHandleFromStart.cpuHandle);

		m_SRV = srvDesc.Buffer;

		return TRUE;
	}

	__debugbreak();
	return FALSE;
}

BOOL Texture::LoadFromDDSFile(std::wstring wstrPath, D3D12_RESOURCE_DESC& outDesc)
{
	std::unique_ptr<uint8_t[]> upDDSData;
	std::vector<D3D12_SUBRESOURCE_DATA> subResourceDatas = {};

	if (FAILED(LoadDDSTextureFromFile(DEVICE.Get(), wstrPath.c_str(), m_pTexResource.GetAddressOf(), upDDSData, subResourceDatas)))
	{
		__debugbreak();
		return FALSE;
	}

	D3D12_RESOURCE_DESC texDesc = m_pTexResource->GetDesc();
	UINT uiSubResourceSize = (UINT)subResourceDatas.size();
	UINT64 ui64UploadBufferSize = GetRequiredIntermediateSize(m_pTexResource.Get(), 0, uiSubResourceSize);

	ComPtr<ID3D12Resource> pUploadBuffer = nullptr;

	// Create Upload heap
	DEVICE->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(ui64UploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(pUploadBuffer.GetAddressOf())
	);

	// Get Command list & allocator from ResourceManager
	ComPtr<ID3D12GraphicsCommandList>& cmdList = RESOURCE.GetCommandList();
	ComPtr<ID3D12CommandAllocator>& cmdAllocator = RESOURCE.GetCommandAllocator();

	ThrowIfFailed(cmdAllocator->Reset());

	ThrowIfFailed(cmdList->Reset(cmdAllocator.Get(), nullptr));

	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pTexResource.Get(), D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST));
	{
		// Functions from D3DX
		// Do CopyTextureRegion() for each subresources
		::UpdateSubresources(cmdList.Get(), m_pTexResource.Get(), pUploadBuffer.Get(), 0, 0, uiSubResourceSize, subResourceDatas.data());
	}
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pTexResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE));

	RESOURCE.CloseAndExcuteCommandList();
	RESOURCE.FenceAndWait();

	outDesc = texDesc;
	m_TextureSize.x = texDesc.Width;
	m_TextureSize.y = texDesc.Height;

	return TRUE;
}

BOOL Texture::LoadFromWICFile(std::wstring wstrPath, D3D12_RESOURCE_DESC& outDesc)
{
	// Non .dds file dosen't have subresources(mipmap)
	// So, MipLevels of non-.dds file is 1

	std::unique_ptr<uint8_t[]> ddsData = nullptr;
	D3D12_SUBRESOURCE_DATA subResourceData = {};

	if (FAILED(LoadWICTextureFromFile(DEVICE.Get(), wstrPath.c_str(), m_pTexResource.GetAddressOf(), ddsData, subResourceData)))
	{
		__debugbreak();
		return FALSE;
	}

	D3D12_RESOURCE_DESC texDesc = m_pTexResource->GetDesc();
	UINT uiSubResourceSize = 1;	// MipLevel is 1
	UINT64 ui64UploadBufferSize = GetRequiredIntermediateSize(m_pTexResource.Get(), 0, uiSubResourceSize);

	ComPtr<ID3D12Resource> pUploadBuffer = nullptr;

	// Create Upload heap
	DEVICE->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(ui64UploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(pUploadBuffer.GetAddressOf())
	);

	// Get Command list & allocator from ResourceManager
	ComPtr<ID3D12GraphicsCommandList>& cmdList = RESOURCE.GetCommandList();
	ComPtr<ID3D12CommandAllocator>& cmdAllocator = RESOURCE.GetCommandAllocator();

	ThrowIfFailed(cmdAllocator->Reset());

	ThrowIfFailed(cmdList->Reset(cmdAllocator.Get(), nullptr));

	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pTexResource.Get(), D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST));
	{
		// Functions from D3DX
		// Do CopyTextureRegion() for each subresources
		::UpdateSubresources(cmdList.Get(), m_pTexResource.Get(), pUploadBuffer.Get(), 0, 0, uiSubResourceSize, &subResourceData);
	}
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pTexResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE));

	RESOURCE.CloseAndExcuteCommandList();
	RESOURCE.FenceAndWait();

	outDesc = texDesc;
	m_TextureSize.x = texDesc.Width;
	m_TextureSize.y = texDesc.Height;


	return TRUE;
}

