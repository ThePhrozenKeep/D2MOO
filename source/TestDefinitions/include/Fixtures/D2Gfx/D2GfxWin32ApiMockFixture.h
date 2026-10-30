#pragma once


#include <cstdint>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <Windows.h>
#include <shellapi.h>

#include <doctest.h>

#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>

#include <DynamicArray.h>
#include <EqualityMacros.h>
#include <InlineHook.h>

#include <D2Gfx.h>

#include <Fixtures/D2Gfx/D2GraphicsInterfaceMockFixture.h>


// A call to a function of the Win32 API made by D2Gfx, all arguments are stored as 32 bits values.
// Strings and structures passed by pointer have different addresses in both implementations, their content is stored instead.
struct D2GfxWin32ApiCallStrc
{
	char szFunction[32];
	uint32_t nArgs[12];
	char szStrings[2][128];
};

BEGIN_VISIT(D2GfxWin32ApiCallStrc)
	ARRAY(szFunction)
	ARRAY(nArgs)
	ARRAY(szStrings)
END_VISIT()

DYNAMIC_ARRAY_TYPE(D2GfxWin32ApiCallStrc)


// Replaces the functions of the Win32 API used by D2Gfx to create and manage its window, so that no real window is created,
// the display settings and the taskbar are left untouched and no message box blocks the tests.
// Every call is recorded in `calls`, the return values and outputs of the functions are set by the members below.
struct D2GfxWin32ApiMock
{
	// Fake handles returned by the functions creating or finding objects
	static constexpr uint32_t DESKTOP_WINDOW = 0x0000DE5C;
	static constexpr uint32_t ICON_BASE = 0x001C0000;
	static constexpr uint32_t CURSOR_BASE = 0x000C0000;
	static constexpr uint32_t STOCK_OBJECT_BASE = 0x00570000;

	static inline std::vector<D2GfxWin32ApiCallStrc> calls;

	static inline HWND hFoundWindow = nullptr;							// FindWindowA
	static inline int32_t nMessageBoxResult = IDOK;						// MessageBoxA
	static inline int32_t nScreenWidth = 1024;							// GetSystemMetrics(SM_CXSCREEN)
	static inline int32_t nScreenHeight = 768;							// GetSystemMetrics(SM_CYSCREEN)
	static inline RECT DesktopRect = { 0, 0, 1024, 768 };				// GetClientRect
	static inline HWND hCreatedWindow = nullptr;						// CreateWindowExA
	static inline DWORD nLastError = ERROR_INVALID_PARAMETER;			// Set by CreateWindowExA and RegisterClassA when they fail
	static inline BOOL bDestroyWindowResult = TRUE;						// DestroyWindow
	static inline BOOL bIsIconic = FALSE;								// IsIconic
	static inline int32_t nCursorDisplayCount = 0;						// ShowCursor
	static inline HWND hAutoHideBars[4] = {};							// SHAppBarMessage(ABM_GETAUTOHIDEBAR), for each edge
	static inline ATOM nRegisterClassResult = 0xC0DE;					// RegisterClassA
	static inline HMODULE hLoadedLibrary = nullptr;						// LoadLibraryA
	static inline FARPROC pfProcAddress = nullptr;						// GetProcAddress
	static inline BOOL bFreeLibraryResult = TRUE;						// FreeLibrary

	static void reset()
	{
		calls.clear();
		hFoundWindow = nullptr;
		nMessageBoxResult = IDOK;
		nScreenWidth = 1024;
		nScreenHeight = 768;
		DesktopRect = { 0, 0, 1024, 768 };
		hCreatedWindow = nullptr;
		nLastError = ERROR_INVALID_PARAMETER;
		bDestroyWindowResult = TRUE;
		bIsIconic = FALSE;
		nCursorDisplayCount = 0;
		std::memset(hAutoHideBars, 0, sizeof(hAutoHideBars));
		nRegisterClassResult = 0xC0DE;
		hLoadedLibrary = nullptr;
		pfProcAddress = nullptr;
		bFreeLibraryResult = TRUE;
	}

	static std::vector<D2GfxWin32ApiCallStrc> take_calls()
	{
		return std::exchange(calls, {});
	}

	template<typename T>
	static uint32_t to_argument(T value)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(value));
		}
		else
		{
			return static_cast<uint32_t>(value);
		}
	}

	static D2GfxWin32ApiCallStrc& record(const char* szFunction, std::initializer_list<uint32_t> args)
	{
		D2GfxWin32ApiCallStrc call{};
		std::snprintf(call.szFunction, sizeof(call.szFunction), "%s", szFunction);

		uint32_t nArg = 0;
		for (const auto arg : args)
		{
			call.nArgs[nArg++] = arg;
		}

		calls.push_back(call);
		return calls.back();
	}

	// Strings can also be resources or atoms, which are stored as arguments
	static uint32_t string_argument(D2GfxWin32ApiCallStrc& call, int32_t nString, const char* szString)
	{
		if (szString == nullptr || IS_INTRESOURCE(szString))
		{
			return to_argument(szString);
		}

		std::snprintf(call.szStrings[nString], sizeof(call.szStrings[nString]), "%s", szString);
		return 0;
	}

	static HWND WINAPI FindWindowA_mock(LPCSTR lpClassName, LPCSTR lpWindowName)
	{
		auto& call = record("FindWindowA", {});
		call.nArgs[0] = string_argument(call, 0, lpClassName);
		call.nArgs[1] = string_argument(call, 1, lpWindowName);
		return hFoundWindow;
	}

	static int WINAPI MessageBoxA_mock(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
	{
		auto& call = record("MessageBoxA", { to_argument(hWnd), 0, 0, uType });
		call.nArgs[1] = string_argument(call, 0, lpText);
		call.nArgs[2] = string_argument(call, 1, lpCaption);
		return nMessageBoxResult;
	}

	// Pretends that windows with a caption have a 3 pixels border and a 22 pixels caption, and that other windows have no border
	static BOOL WINAPI AdjustWindowRectEx_mock(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle)
	{
		record("AdjustWindowRectEx", { to_argument(lpRect->left), to_argument(lpRect->top), to_argument(lpRect->right), to_argument(lpRect->bottom), dwStyle, to_argument(bMenu), dwExStyle });

		if ((dwStyle & WS_CAPTION) == WS_CAPTION)
		{
			lpRect->left -= 3;
			lpRect->top -= 25;
			lpRect->right += 3;
			lpRect->bottom += 3;
		}
		return TRUE;
	}

	static HWND WINAPI GetDesktopWindow_mock()
	{
		record("GetDesktopWindow", {});
		return reinterpret_cast<HWND>(static_cast<uintptr_t>(DESKTOP_WINDOW));
	}

	static BOOL WINAPI GetClientRect_mock(HWND hWnd, LPRECT lpRect)
	{
		record("GetClientRect", { to_argument(hWnd) });
		*lpRect = DesktopRect;
		return TRUE;
	}

	static int WINAPI GetSystemMetrics_mock(int nIndex)
	{
		record("GetSystemMetrics", { to_argument(nIndex) });
		switch (nIndex)
		{
		case SM_CXSCREEN:
			return nScreenWidth;

		case SM_CYSCREEN:
			return nScreenHeight;

		default:
			return 0;
		}
	}

	static LONG WINAPI ChangeDisplaySettingsA_mock(DEVMODEA* lpDevMode, DWORD dwFlags)
	{
		if (lpDevMode)
		{
			record("ChangeDisplaySettingsA", { 1, lpDevMode->dmSize, lpDevMode->dmFields, lpDevMode->dmBitsPerPel, lpDevMode->dmPelsWidth, lpDevMode->dmPelsHeight, dwFlags });
		}
		else
		{
			record("ChangeDisplaySettingsA", { 0, 0, 0, 0, 0, 0, dwFlags });
		}
		return DISP_CHANGE_SUCCESSFUL;
	}

	static HWND WINAPI CreateWindowExA_mock(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
	{
		auto& call = record("CreateWindowExA", { dwExStyle, 0, 0, dwStyle, to_argument(X), to_argument(Y), to_argument(nWidth), to_argument(nHeight), to_argument(hWndParent), to_argument(hMenu), to_argument(hInstance), to_argument(lpParam) });
		call.nArgs[1] = string_argument(call, 0, lpClassName);
		call.nArgs[2] = string_argument(call, 1, lpWindowName);

		if (!hCreatedWindow)
		{
			SetLastError(nLastError);
		}
		return hCreatedWindow;
	}

	static BOOL WINAPI ShowWindow_mock(HWND hWnd, int nCmdShow)
	{
		record("ShowWindow", { to_argument(hWnd), to_argument(nCmdShow) });
		return TRUE;
	}

	static BOOL WINAPI UpdateWindow_mock(HWND hWnd)
	{
		record("UpdateWindow", { to_argument(hWnd) });
		return TRUE;
	}

	static HWND WINAPI SetFocus_mock(HWND hWnd)
	{
		record("SetFocus", { to_argument(hWnd) });
		return nullptr;
	}

	static int WINAPI ShowCursor_mock(BOOL bShow)
	{
		record("ShowCursor", { to_argument(bShow) });
		nCursorDisplayCount += bShow ? 1 : -1;
		return nCursorDisplayCount;
	}

	static BOOL WINAPI DestroyWindow_mock(HWND hWnd)
	{
		record("DestroyWindow", { to_argument(hWnd) });
		return bDestroyWindowResult;
	}

	static BOOL WINAPI SetWindowPos_mock(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
	{
		record("SetWindowPos", { to_argument(hWnd), to_argument(hWndInsertAfter), to_argument(X), to_argument(Y), to_argument(cx), to_argument(cy), uFlags });
		return TRUE;
	}

	static BOOL WINAPI IsIconic_mock(HWND hWnd)
	{
		record("IsIconic", { to_argument(hWnd) });
		return bIsIconic;
	}

	// The placement is made up from the window handle
	static BOOL WINAPI GetWindowPlacement_mock(HWND hWnd, WINDOWPLACEMENT* lpwndpl)
	{
		record("GetWindowPlacement", { to_argument(hWnd), lpwndpl->length });

		const auto nValue = static_cast<LONG>(reinterpret_cast<uintptr_t>(hWnd));
		lpwndpl->flags = WPF_SETMINPOSITION;
		lpwndpl->showCmd = static_cast<UINT>(nValue % 11);
		lpwndpl->ptMinPosition = { nValue & 0xFF, (nValue >> 8) & 0xFF };
		lpwndpl->ptMaxPosition = { -1, -1 };
		lpwndpl->rcNormalPosition = { nValue & 0x7F, (nValue >> 4) & 0x7F, 800 + (nValue & 0x7F), 600 + ((nValue >> 4) & 0x7F) };
		return TRUE;
	}

	static ATOM WINAPI RegisterClassA_mock(const WNDCLASSA* lpWndClass)
	{
		auto& call = record("RegisterClassA", {
			lpWndClass->style, to_argument(lpWndClass->lpfnWndProc), to_argument(lpWndClass->cbClsExtra), to_argument(lpWndClass->cbWndExtra), to_argument(lpWndClass->hInstance),
			to_argument(lpWndClass->hIcon), to_argument(lpWndClass->hCursor), to_argument(lpWndClass->hbrBackground), 0, 0
		});
		call.nArgs[8] = string_argument(call, 0, lpWndClass->lpszMenuName);
		call.nArgs[9] = string_argument(call, 1, lpWndClass->lpszClassName);

		if (!nRegisterClassResult)
		{
			SetLastError(nLastError);
		}
		return nRegisterClassResult;
	}

	static BOOL WINAPI UnregisterClassA_mock(LPCSTR lpClassName, HINSTANCE hInstance)
	{
		auto& call = record("UnregisterClassA", { 0, to_argument(hInstance) });
		call.nArgs[0] = string_argument(call, 0, lpClassName);
		return TRUE;
	}

	static HANDLE WINAPI LoadImageA_mock(HINSTANCE hInst, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad)
	{
		auto& call = record("LoadImageA", { to_argument(hInst), 0, type, to_argument(cx), to_argument(cy), fuLoad });
		call.nArgs[1] = string_argument(call, 0, name);
		return reinterpret_cast<HANDLE>(static_cast<uintptr_t>(ICON_BASE | call.nArgs[1]));
	}

	static HCURSOR WINAPI LoadCursorA_mock(HINSTANCE hInstance, LPCSTR lpCursorName)
	{
		auto& call = record("LoadCursorA", { to_argument(hInstance), 0 });
		call.nArgs[1] = string_argument(call, 0, lpCursorName);
		return reinterpret_cast<HCURSOR>(static_cast<uintptr_t>(CURSOR_BASE | (call.nArgs[1] & 0xFFFF)));
	}

	static HGDIOBJ WINAPI GetStockObject_mock(int i)
	{
		record("GetStockObject", { to_argument(i) });
		return reinterpret_cast<HGDIOBJ>(static_cast<uintptr_t>(STOCK_OBJECT_BASE | i));
	}

	static DWORD WINAPI GdiSetBatchLimit_mock(DWORD dw)
	{
		record("GdiSetBatchLimit", { dw });
		return 20;
	}

	static UINT_PTR WINAPI SHAppBarMessage_mock(DWORD dwMessage, PAPPBARDATA pData)
	{
		record("SHAppBarMessage", { dwMessage, pData->cbSize, to_argument(pData->hWnd), pData->uCallbackMessage, pData->uEdge,
			to_argument(pData->rc.left), to_argument(pData->rc.top), to_argument(pData->rc.right), to_argument(pData->rc.bottom), to_argument(pData->lParam) });

		if (dwMessage == ABM_GETAUTOHIDEBAR && pData->uEdge < 4)
		{
			return reinterpret_cast<UINT_PTR>(hAutoHideBars[pData->uEdge]);
		}
		return 0;
	}

	static HMODULE WINAPI LoadLibraryA_mock(LPCSTR lpLibFileName)
	{
		auto& call = record("LoadLibraryA", {});
		call.nArgs[0] = string_argument(call, 0, lpLibFileName);
		return hLoadedLibrary;
	}

	static FARPROC WINAPI GetProcAddress_mock(HMODULE hModule, LPCSTR lpProcName)
	{
		auto& call = record("GetProcAddress", { to_argument(hModule) });
		call.nArgs[1] = string_argument(call, 0, lpProcName);
		return pfProcAddress;
	}

	static BOOL WINAPI FreeLibrary_mock(HMODULE hLibModule)
	{
		record("FreeLibrary", { to_argument(hLibModule) });
		return bFreeLibraryResult;
	}
};


// Installs the mocks of D2GfxWin32ApiMock while it exists.
// The functions are hooked where they are implemented, so the calls of both implementations are replaced.
// LoadLibraryA, GetProcAddress and FreeLibrary are only hooked when asked, since nothing else may load a library while they are.
class D2GfxWin32ApiHooks
{
public:
	explicit D2GfxWin32ApiHooks(bool bHookLibraryFunctions = false)
	{
		m_hooks.reserve(32);

		hook("user32.dll", "FindWindowA", D2GfxWin32ApiMock::FindWindowA_mock);
		hook("user32.dll", "MessageBoxA", D2GfxWin32ApiMock::MessageBoxA_mock);
		hook("user32.dll", "AdjustWindowRectEx", D2GfxWin32ApiMock::AdjustWindowRectEx_mock);
		hook("user32.dll", "GetDesktopWindow", D2GfxWin32ApiMock::GetDesktopWindow_mock);
		hook("user32.dll", "GetClientRect", D2GfxWin32ApiMock::GetClientRect_mock);
		hook("user32.dll", "GetSystemMetrics", D2GfxWin32ApiMock::GetSystemMetrics_mock);
		hook("user32.dll", "ChangeDisplaySettingsA", D2GfxWin32ApiMock::ChangeDisplaySettingsA_mock);
		hook("user32.dll", "CreateWindowExA", D2GfxWin32ApiMock::CreateWindowExA_mock);
		hook("user32.dll", "ShowWindow", D2GfxWin32ApiMock::ShowWindow_mock);
		hook("user32.dll", "UpdateWindow", D2GfxWin32ApiMock::UpdateWindow_mock);
		hook("user32.dll", "SetFocus", D2GfxWin32ApiMock::SetFocus_mock);
		hook("user32.dll", "ShowCursor", D2GfxWin32ApiMock::ShowCursor_mock);
		hook("user32.dll", "DestroyWindow", D2GfxWin32ApiMock::DestroyWindow_mock);
		hook("user32.dll", "SetWindowPos", D2GfxWin32ApiMock::SetWindowPos_mock);
		hook("user32.dll", "IsIconic", D2GfxWin32ApiMock::IsIconic_mock);
		hook("user32.dll", "GetWindowPlacement", D2GfxWin32ApiMock::GetWindowPlacement_mock);
		hook("user32.dll", "RegisterClassA", D2GfxWin32ApiMock::RegisterClassA_mock);
		hook("user32.dll", "UnregisterClassA", D2GfxWin32ApiMock::UnregisterClassA_mock);
		hook("user32.dll", "LoadImageA", D2GfxWin32ApiMock::LoadImageA_mock);
		hook("user32.dll", "LoadCursorA", D2GfxWin32ApiMock::LoadCursorA_mock);
		hook("gdi32.dll", "GetStockObject", D2GfxWin32ApiMock::GetStockObject_mock);
		hook("gdi32.dll", "GdiSetBatchLimit", D2GfxWin32ApiMock::GdiSetBatchLimit_mock);
		hook("shell32.dll", "SHAppBarMessage", D2GfxWin32ApiMock::SHAppBarMessage_mock);

		if (bHookLibraryFunctions)
		{
			hook("kernel32.dll", "LoadLibraryA", D2GfxWin32ApiMock::LoadLibraryA_mock);
			hook("kernel32.dll", "FreeLibrary", D2GfxWin32ApiMock::FreeLibrary_mock);
			// Last, since it is used to find the other functions
			hook("kernel32.dll", "GetProcAddress", D2GfxWin32ApiMock::GetProcAddress_mock);
		}
	}

	D2GfxWin32ApiHooks(const D2GfxWin32ApiHooks&) = delete;
	D2GfxWin32ApiHooks& operator=(const D2GfxWin32ApiHooks&) = delete;

private:
	template<typename Function>
	void hook(const char* szModule, const char* szFunction, Function* pfReplacement)
	{
		const auto hModule = GetModuleHandleA(szModule);
		const auto pfTarget = hModule ? reinterpret_cast<void*>(GetProcAddress(hModule, szFunction)) : nullptr;
		if (pfTarget == nullptr)
		{
			throw std::runtime_error(std::string("D2GfxWin32ApiHooks: cannot find ") + szModule + "!" + szFunction);
		}

		for (const auto pfHookedTarget : m_targets)
		{
			if (pfHookedTarget == pfTarget)
			{
				throw std::runtime_error(std::string("D2GfxWin32ApiHooks: ") + szFunction + " shares its address with another hooked function");
			}
		}

		m_targets.push_back(pfTarget);
		m_hooks.emplace_back(pfTarget, reinterpret_cast<const void*>(pfReplacement));
	}

	std::vector<void*> m_targets;
	std::vector<InlineHook> m_hooks;
};


// Compares the calls made to the Win32 API by both implementations
inline void check_win32_api_calls_eq(std::vector<D2GfxWin32ApiCallStrc>& moo_calls, std::vector<D2GfxWin32ApiCallStrc>& original_calls, const char* context_title)
{
	CHECK_MESSAGE(moo_calls.size() == original_calls.size(), context_title, " - Different number of calls to the Win32 API");
	if (moo_calls.size() == original_calls.size() && !moo_calls.empty())
	{
		DynamicArray<D2GfxWin32ApiCallStrc> moo_array{ moo_calls.data(), static_cast<int>(moo_calls.size()) };
		DynamicArray<D2GfxWin32ApiCallStrc> original_array{ original_calls.data(), static_cast<int>(original_calls.size()) };
		MOO_CHECK_EQ(moo_array, original_array, context_title);
	}
}


// Calls made by one implementation to the graphics interface and to the Win32 API, and the cursor display count it left
struct D2GfxMockedCallsStrc
{
	std::vector<D2GfxInterfaceCallStrc> interface_calls;
	std::vector<D2GfxWin32ApiCallStrc> api_calls;
	int32_t nCursorDisplayCount = 0;
};

// Calls one implementation with the Win32 API mocked, starting with the given cursor display count, and returns its result
template<typename Function>
auto call_with_mocked_win32_api(Function&& function, D2GfxMockedCallsStrc& calls, int32_t nCursorDisplayCount = 0, bool bHookLibraryFunctions = false)
{
	D2GraphicsInterfaceMock::take_calls();
	D2GfxWin32ApiMock::take_calls();
	D2GfxWin32ApiMock::nCursorDisplayCount = nCursorDisplayCount;

	const auto take_calls = [&calls]()
	{
		calls.interface_calls = D2GraphicsInterfaceMock::take_calls();
		calls.api_calls = D2GfxWin32ApiMock::take_calls();
		calls.nCursorDisplayCount = D2GfxWin32ApiMock::nCursorDisplayCount;
	};

	if constexpr (std::is_void_v<decltype(function())>)
	{
		{
			D2GfxWin32ApiHooks hooks(bHookLibraryFunctions);
			function();
		}
		take_calls();
	}
	else
	{
		decltype(function()) result{};
		{
			D2GfxWin32ApiHooks hooks(bHookLibraryFunctions);
			result = function();
		}
		take_calls();
		return result;
	}
}


// Resets D2GfxWin32ApiMock for each test. The hooks are installed by the tests with D2GfxWin32ApiHooks around the calls to D2Gfx.
template<class Fixture>
struct D2GfxWin32ApiMockFixture : Fixture
{
	D2GfxWin32ApiMockFixture()
	{
		D2GfxWin32ApiMock::reset();
	}

	~D2GfxWin32ApiMockFixture()
	{
		D2GfxWin32ApiMock::reset();
	}

	// Compares the calls to the graphics interface and to the Win32 API, and the cursor display count
	void check_mocked_calls_eq(D2GfxMockedCallsStrc& moo_calls, D2GfxMockedCallsStrc& original_calls)
	{
		check_interface_calls_eq(moo_calls.interface_calls, original_calls.interface_calls, "Comparing calls to the graphics interface");
		check_win32_api_calls_eq(moo_calls.api_calls, original_calls.api_calls, "Comparing calls to the Win32 API");
		MOO_CHECK_EQ(moo_calls.nCursorDisplayCount, original_calls.nCursorDisplayCount, "Comparing cursor display counts");
	}
};
