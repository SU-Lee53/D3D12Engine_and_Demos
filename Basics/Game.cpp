#include "pch.h"
#include "Game.h"
#include "Application.h"
#include "Camera.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma region Win32Callbacks
WPARAM Game::Run(const GameDesc& desc)
{
    m_Desc = desc;

    MyRegisterClass();

    if (!InitInstance(SW_SHOW))
    {
        return FALSE;
    }

    {
        // TODO : Init
#ifdef _DEBUG
        CORE.Initialize(TRUE, TRUE);
#else
        CORE.Initialize(FALSE, FALSE);
#endif 
        SHADER.Initialize();
        INPUT.Initialize();
        TIME.Initialize();
        RENDER.Initialize();
        RESOURCE.Initialize();
        GUI.Initialize();
        TEXTURE.Initialize();

        m_Desc.app->Initialize();
    }

    MSG msg;

    // Main message loop:
    while (1)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // TODO: Run
            Update();
            Render();
        }
    }

    // Wait for gpu command complete before terminate
    CORE.Fence();

    for (DWORD i = 0; i < MAX_PENDING_FRAME_COUNT; i++)
    {
        CORE.WaitForFenceValue(CORE.GetFenceValue(i));
    }

    // ImGui Clean Up
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    return (int)msg.wParam;
}

ATOM Game::MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_Desc.hInstance;
    wcex.hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
    wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = m_Desc.windowName.c_str();
    wcex.hIconSm = wcex.hIcon;

    return RegisterClassExW(&wcex);
}

BOOL Game::InitInstance(int cmdShow)
{
    RECT windowRect = { 0, 0, m_Desc.width, m_Desc.height };
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

    m_Desc.hWnd = CreateWindowW(m_Desc.windowName.c_str(), m_Desc.windowName.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, m_Desc.hInstance, nullptr);

    if (!m_Desc.hWnd)
        return FALSE;

    ::ShowWindow(m_Desc.hWnd, cmdShow);
    ::UpdateWindow(m_Desc.hWnd);

    return TRUE;
}

LRESULT Game::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_SIZE: 
    {
        RECT	rect;
        HWND hWnd = ::GetActiveWindow();    // 이래도 되는지 모르겠음...
        GetClientRect(hWnd, &rect);
        DWORD	dwWndWidth = rect.right - rect.left;
        DWORD	dwWndHeight = rect.bottom - rect.top;
        CORE.UpdateWindowSize(dwWndWidth, dwWndHeight);
    }
        break;
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return ::DefWindowProc(handle, message, wParam, lParam);
    }
}
#pragma endregion Win32Callbacks

void Game::Update()
{
    // Update Managers
    INPUT.Update();
    TIME.Update();
    GUI.Update();

    // Update Game Logic
    CORE.GetMainCamera()->Update();
    m_Desc.app->Update();

    // Update FPS
    ShowFps();
}

void Game::Render()
{
    RENDER.RenderBegin();

    {
        m_Desc.app->Render();
        RENDER.Render();
        GUI.Render();
    }

    RENDER.RenderEnd();
    RENDER.Present();
}

void Game::ShowFps()
{
    UINT fps = TIME.GetFps();

    WCHAR text[100] = L"";
    wsprintf(text, L"FPS : %d", fps);

    SetWindowText(m_Desc.hWnd, text);
}
