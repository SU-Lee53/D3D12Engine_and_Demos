#include "pch.h"
#include "Game.h"
#include "BasicForwardRenderDemo.h"
#include "HierarchyModelDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GameDesc desc;
	{
		desc.width = 1600;
		desc.height = 900;
		desc.BackColor = Color{ 0.f, 0.f, 0.f, 1.f };
		desc.windowName = L"D3D12Engine";
		desc.hInstance = hInstance;
		desc.hWnd = NULL;
		desc.app = std::make_shared<HierarchyModelDemo>();

	}

	GAME.Run(desc);
}

