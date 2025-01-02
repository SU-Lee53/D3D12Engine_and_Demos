#pragma once

struct GameDesc
{
	DWORD width;
	DWORD height;
	Color BackColor = Color{ 0.f,1.f,0.f,1.f };
	std::wstring windowName = L"";
	HINSTANCE hInstance = 0;
	HWND hWnd = 0;
	std::shared_ptr<class Application> app;
};

class Game
{
	DECLARE_SINGLE(Game);

public:
	WPARAM Run(const GameDesc& desc);
	ATOM MyRegisterClass();
	BOOL InitInstance(int cmdShow);
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

public:
	void Update();
	void Render();

public:
	void UpdateWindowSize(DWORD width, DWORD height)
	{
		m_Desc.width = width;
		m_Desc.height = height;
	}

public:
	GameDesc& GetGameDesc() { return m_Desc; }

private:
	GameDesc m_Desc;

};

