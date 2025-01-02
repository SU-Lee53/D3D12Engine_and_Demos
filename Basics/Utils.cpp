#include "pch.h"
#include "Utils.h"

void Utils::SetDebugLayerInfo(ComPtr<ID3D12Device> pDevice)
{
    ComPtr<ID3D12InfoQueue> pInfoQueue;
    pDevice->QueryInterface(IID_PPV_ARGS(pInfoQueue.GetAddressOf()));

    if (pInfoQueue)
    {
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);

        D3D12_MESSAGE_ID hide[] =
        {
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_WRONGSWAPCHAINBUFFERREFERENCE,
            D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
        };
        D3D12_INFO_QUEUE_FILTER filter = {};
        filter.DenyList.NumIDs = _countof(hide);
        filter.DenyList.pIDList = hide;
        pInfoQueue->AddStorageFilterEntries(&filter);
    }
}
