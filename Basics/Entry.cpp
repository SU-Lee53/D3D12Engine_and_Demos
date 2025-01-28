#include "pch.h"
#include "Game.h"
#include "BasicForwardRenderDemo.h"
#include "HierarchyModelDemo.h"
#include "LambertDemo.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetBreakAlloc(326);
#endif _DEBUG

	GameDesc desc;
	{
		desc.width = 1600;
		desc.height = 900;
		desc.BackColor = Color{ 0.f, 0.f, 0.f, 1.f };
		desc.windowName = L"D3D12Engine";
		desc.hInstance = hInstance;
		desc.hWnd = NULL;
		desc.app = std::make_shared<LambertDemo>();

	}

	GAME.Run(desc);

#ifdef _DEBUG
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();
#endif _DEBUG

}

