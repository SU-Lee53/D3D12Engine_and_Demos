#pragma once

enum class KEY_TYPE
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,

	SPACE = VK_SPACE,
	CTRL = VK_CONTROL,
	ENTER = VK_RETURN,
	ESC = VK_ESCAPE,

	LSHIFT = VK_LSHIFT,
	RSHIFT = VK_RSHIFT,

	A = 'A',
	B = 'B',
	C = 'C',
	D = 'D',
	E = 'E',
	F = 'F',
	G = 'G',
	H = 'H',
	I = 'I',
	J = 'J',
	K = 'K',
	L = 'L',
	M = 'M',
	N = 'N',
	O = 'O',
	P = 'P',
	Q = 'Q',
	R = 'R',
	S = 'S',
	T = 'T',
	U = 'U',
	V = 'V',
	W = 'W',
	X = 'X',
	Y = 'Y',
	Z = 'Z',

	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',

	LBUTTON = VK_LBUTTON,
	RBUTTON = VK_RBUTTON,

	end = -1
};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	END
};

enum
{
	KEY_TYPE_COUNT = static_cast<int>(UINT8_MAX + 1),
	KEY_STATE_COUNT = static_cast<int>(KEY_STATE::END),
};

class InputManager
{
	DECLARE_SINGLE(InputManager)

public:
	void Initialize();
	void Update();

public:
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }

private:
	void UpdateKeyboard();
	void UpdateMouse();

public:
	const POINT& GetMousePos() { return _mousePos; }

private:
	inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<UINT8>(key)]; }

private:
	HWND _hWnd = nullptr;
	std::vector<KEY_STATE> _states;
	POINT _mousePos = {};
};
