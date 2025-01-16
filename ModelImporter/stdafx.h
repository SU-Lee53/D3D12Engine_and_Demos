#pragma once

// STL
#include <iostream>
#include <print>
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <string>


// WIN
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <initguid.h>
#include <WinUser.h>
#include <assert.h>
#include <optional>

// DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// DirectXTex
#include "DirectXTex/DirectXTex.h"

// ImGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#ifdef _DEBUG
#pragma comment(lib, "../library/lib/DirectXTex/DirectXTex_debug.lib")
#else
#pragma comment(lib, "../library/lib/DirectXTex/DirectXTex.lib")
#endif

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


// fbx SDK
#if defined(_M_AMD64)
#if defined(_DEBUG)
#pragma comment(lib, "../library/lib/FBXSDK/x64/debug/libfbxsdk.lib")
#elif
#pragma comment(lib, "../library/lib/FBXSDK/x64/release/libfbxsdk.lib")
#endif
#elif defined(_M_IX86)
#if defined(_DEBUG)
#pragma comment(lib, "../library/lib/FBXSDK/x86/debug/libfbxsdk.lib")
#elif
#pragma comment(lib, "../library/lib/FBXSDK/x86/release/libfbxsdk.lib")
#endif
#elif defined(_M_ARM64EC)
#if defined(_DEBUG)
#pragma comment(lib, "../library/lib/FBXSDK/arm64/debug/libfbxsdk.lib")
#elif
#pragma comment(lib, "../library/lib/FBXSDK/arm64/release/libfbxsdk.lib")
#endif
#endif

#include "fbxsdk.h"
#define FBXSDK_SHARED

// headers
#include "define.h"

// Cores
#include "D3D11Core.h"

// Managers
//#include "InputManager.h"
#include "ImGuiManager.h"