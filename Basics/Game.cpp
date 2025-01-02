#include "pch.h"
#include "Game.h"



#pragma region Win32Callbacks
WPARAM Game::Run(const GameDesc& desc)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif _DEBUG

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

        RENDER.Initialize();


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

#ifdef _DEBUG
    _ASSERT(_CrtCheckMemory());
#endif _DEBUG

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
void Game::Update()
{
}
void Game::Render()
{
    RENDER.RenderBegin();
    RENDER.Render();
    RENDER.RenderEnd();
    RENDER.Present();
}
#pragma endregion Win32Callbacks
