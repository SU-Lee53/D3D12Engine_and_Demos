#pragma once

// STL
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <string>


// WIN
#include <windows.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <initguid.h>
#include <WinUser.h>
#include <assert.h>
#include <optional>

// DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <wrl.h>
#include <DirectXMath.h>
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

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