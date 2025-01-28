#include "pch.h"
#include "ImGuiManager.h"

void ImGuiManager::Initialize()
{
    m_pFontSrvDescriptorHeap = std::make_unique<DescriptorHeap>();
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 1;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    m_pFontSrvDescriptorHeap->Initialize(desc);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(GAME.GetGameDesc().hWnd);
    ImGui_ImplDX12_Init(DEVICE.Get(), MAX_PENDING_FRAME_COUNT,
        DXGI_FORMAT_R8G8B8A8_UNORM, m_pFontSrvDescriptorHeap->pDescriptorHeap.Get(),
        m_pFontSrvDescriptorHeap->pDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
        m_pFontSrvDescriptorHeap->pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiManager::Update()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::Render()
{
    ComPtr<ID3D12GraphicsCommandList>& pCommandList = RENDER.GetCurrentCommandList();

    // Rendering
    ImGui::Render();

    pCommandList->SetDescriptorHeaps(1, m_pFontSrvDescriptorHeap->pDescriptorHeap.GetAddressOf());

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), RENDER.GetCurrentCommandList().Get());

}
