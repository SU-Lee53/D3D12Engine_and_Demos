#pragma once

// Window
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <initguid.h>

// D3D12
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <d2d1_3helper.h>
#include <dwrite_3.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
using namespace DirectX;

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#if defined(_M_ARM64EC)
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = (const char*)u8".\\D3D12\\arm64\\"; }
#elif defined(_M_ARM64)
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = (const char*)u8".\\D3D12\\arm64\\"; }
#elif defined(_M_AMD64)
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = (const char*)u8".\\D3D12\\x64\\"; }
#endif

// Windows Header Files
#include <windows.h>
#include <wrl.h>
using namespace Microsoft::WRL;

// C librarys
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

// STL
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <list>
#include <variant>
using namespace std::literals;

// Additional Headers
#include "define.h"
#include "constants.h"
#include "types.h"

// Engine Cores
#include "Game.h"
#include "Core.h"

// Managers
#include "RenderManager.h"

// Debugs
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif _DEBUG

