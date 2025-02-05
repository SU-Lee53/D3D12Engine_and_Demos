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

// DirectXTex
#include "../library/include/DirectXTex/DirectXTex.h"
#include "DDSTextureLoader12.h"
#include "WICTextureLoader12.h"
#ifdef _DEBUG
#pragma comment(lib, "../library/lib/DirectXTex/DirectXTex_debug.lib")
#else
#pragma comment(lib, "../library/lib/DirectXTex/DirectXTex.lib")
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
#include <array>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include <list>
#include <variant>
#include <filesystem>
#include <concepts>
using namespace std::literals;

// ImGui

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"

// Additional Headers
#include "define.h"
#include "Utils.h"
#include "constants.h"
#include "types.h"
#include "input_types.h"
#include "renderer_types.h"
#include "CB_types.h"
#include "HR_helper.h"

// Engine Cores
#include "Game.h"
#include "Core.h"

// Managers
#include "RenderManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ImGuiManager.h"

// Object Components
#include "Transform.h"
#include "Mesh.h"

// Descriptor
#include "Descriptor.h"

// Demo Resources
#include "Pipeline.h"
#include "RootSignature.h"
#include "RenderMethod.h"
#include "Object.h"
#include "StructuredBuffer.h"
#include "Mesh.h"

// Debugs
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif _DEBUG

