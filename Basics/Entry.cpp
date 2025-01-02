#include "pch.h"
#include "Game.h"
#include "TestApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GameDesc desc;
	{
		desc.width = 1600;
		desc.height = 900;
		desc.BackColor = Color{ 1.f, 0.f, 0.f, 1.f };
		desc.windowName = L"Billards";
		desc.hInstance = hInstance;
		desc.hWnd = NULL;
		desc.app = std::make_shared<TestApp>();

	}

	GAME.Run(desc);
}

