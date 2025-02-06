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

BOOL Texture::Initialize(std::wstring wstrPath)
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
		// Create Shader Resource View
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.Format;

		DEVICE->CreateShaderResourceView(m_pTexResource.Get(), &srvDesc, m_SRV);
	}
	else
	{
		return bResult;
	}

	return bResult;
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

	return TRUE;
}

BOOL Texture::LoadFromWICFile(std::wstring wstrPath, D3D12_RESOURCE_DESC& outDesc)
{
	// In most cases, .dds files supports subresources(mipmap)
	// but, I'm going to check if WIC file has subresource, just in case

	std::unique_ptr<uint8_t[]> ddsData = nullptr;
	D3D12_SUBRESOURCE_DATA subResourceData = {};

	if (FAILED(LoadWICTextureFromFile(DEVICE.Get(), wstrPath.c_str(), m_pTexResource.GetAddressOf(), ddsData, subResourceData)))
	{
		__debugbreak();
		return FALSE;
	}

	D3D12_RESOURCE_DESC texDesc = m_pTexResource->GetDesc();
	UINT uiSubResourceSize = 0;
	UINT64 ui64UploadBufferSize = GetRequiredIntermediateSize(m_pTexResource.Get(), 0, uiSubResourceSize);
	if (subResourceData.pData)
	{
		uiSubResourceSize = 1;
	}
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

	return TRUE;
}

