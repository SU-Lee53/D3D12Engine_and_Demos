// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

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

// Debugs
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif _DEBUG

#endif //PCH_H
