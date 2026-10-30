#include <D2GfxTestDefines.h>

#ifdef WINDOW_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstring>
#include <filesystem>
#include <vector>

#include <D2GfxTestDefinitions.h>
#include <TestUtilities.h>

#include <D2Constants.h>
#include <D2Gfx.h>
#include <DisplayType.h>
#include <Window.h>

#include <Fixtures/D2Gfx/D2GfxGlobalsFixture.h>
#include <Fixtures/D2Gfx/D2GfxWin32ApiMockFixture.h>


// The functions creating and managing the window are tested with the Win32 API mocked (see D2GfxWin32ApiMock),
// so no real window is created, and the display settings, the cursor and the taskbar are left untouched.
// Not tested, since they are fatal errors that would end the tests:
// - an unknown resolution in WINDOW_Create (#10023), WINDOW_Resize (#10030) and WINDOW_EndCutScene (#10033)
// - the assertions on the graphics interface and the window
// - failing to resize the window in WINDOW_EndCutScene (#10033)
// - pressing Cancel in the message box displayed when the window cannot be created, which breaks into the debugger


using GfxFixture = D2GfxGlobalsFixture<NoOpFixture>;
using WindowFixture = D2GfxWin32ApiMockFixture<GfxFixture>;


// D2GFX_SetPalette passes the palette of each implementation to the graphics interface, so its address is checked and removed before comparing calls
static void normalize_set_palette_calls(std::vector<D2GfxInterfaceCallStrc>& calls, D2GfxGlobals& globals)
{
	for (auto& call : calls)
	{
		if (call.nFunction == D2GFX_INTERFACE_FUNCTION_INDEX(pfSetPalette))
		{
			CHECK_EQ(call.nArgs[0], reinterpret_cast<uintptr_t>(&globals.Palette[0]));
			call.nArgs[0] = 0;
		}
	}
}

static void __cdecl dummy_cutscene_frame()
{
}

static HWND fake_window(uint32_t nValue)
{
	return reinterpret_cast<HWND>(static_cast<uintptr_t>(nValue));
}

// Fake auto hide bars hidden by WINDOW_UpdatePlacement (#10039) and shown again by WINDOW_ShowAll (#10040), some edges have none
static void generate_window_placements(D2WindowPlacementStrc(&pWindowPlacements)[4])
{
	std::memset(pWindowPlacements, 0, sizeof(pWindowPlacements));
	for (auto& windowPlacement : pWindowPlacements)
	{
		if (random_unsigned_integer(0, 2) != 0)
		{
			windowPlacement.hWnd = fake_window(random_unsigned_integer(0x100, 0xFFFF));
			windowPlacement.windowPlacement.length = sizeof(WINDOWPLACEMENT);
			windowPlacement.windowPlacement.showCmd = random_unsigned_integer(0, 10);
			windowPlacement.windowPlacement.rcNormalPosition = { 0, 0, 1024, 30 };
		}
	}
}

static void generate_auto_hide_bars()
{
	for (auto& hAutoHideBar : D2GfxWin32ApiMock::hAutoHideBars)
	{
		hAutoHideBar = random_unsigned_integer(0, 2) != 0 ? fake_window(random_unsigned_integer(0x100, 0xFFFF)) : nullptr;
	}
}


TEST_SUITE("WindowTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Gfx.dll").string().c_str()));


	TEST_CASE_FIXTURE(WindowFixture, "D2Gfx.0x6FA74450 (#10023)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_Create, dll_base + 0x00004450);

		const auto bWindowed = GENERATE(0, 1, 2);
		const auto nDisplayType = GENERATE(DISPLAYTYPE_GDI, DISPLAYTYPE_DDRAW, DISPLAYTYPE_GLIDE, DISPLAYTYPE_OPENGL, DISPLAYTYPE_DIRECT3D);
		const auto nResolution = GENERATE(D2GAMERES_640x480, D2GAMERES_800x600, NUM_GAME_RESOLUTIONS, D2GAMERES_1344x700);
		// 0: the window and its surface are created, 1: the surface cannot be created, 2: the window cannot be created, 3: the game is already running
		const auto nScenario = GENERATE(0, 1, 2, 3);
		// Screens smaller than 800x600 have their resolution changed
		const auto nScreenWidth = GENERATE(640, 1024);

		SUBCASE("")
		{
			const auto hInstance = reinterpret_cast<HINSTANCE>(static_cast<uintptr_t>(0x00400000));
			const auto bCursorDisplayed = static_cast<int32_t>(random_unsigned_integer(0, 1));
			const auto nCursorDisplayCount = random_signed_integer(-2, 2);

			const auto setup_data = [&](
				D2GfxGlobals& globals
			) {
				globals.hInstance = hInstance;
				globals.nDisplayType = nDisplayType;
				globals.bCursorDisplayed = bCursorDisplayed;
				globals.bIsWindowed = 3;
				globals.nResolutionMode = D2GAMERES_640x480;
				globals.nWindowBorderHeight = 1234;
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			// The window is centered on the desktop, but not outside of it
			D2GfxWin32ApiMock::nScreenWidth = nScreenWidth;
			D2GfxWin32ApiMock::nScreenHeight = static_cast<int32_t>(random_unsigned_integer(480, 1200));
			D2GfxWin32ApiMock::DesktopRect = { 0, 0, static_cast<LONG>(random_unsigned_integer(600, 2000)), static_cast<LONG>(random_unsigned_integer(400, 1200)) };
			D2GfxWin32ApiMock::hFoundWindow = nScenario == 3 ? fake_window(0xD2) : nullptr;
			D2GfxWin32ApiMock::hCreatedWindow = nScenario == 2 ? nullptr : fake_window(0x1234);
			D2GfxWin32ApiMock::nLastError = ERROR_NOT_ENOUGH_MEMORY;
			D2GraphicsInterfaceMock::return_value = nScenario == 1 ? 0 : 1;

			// Call both implementations
			D2GfxMockedCallsStrc moo_calls;
			const auto moo_result = call_with_mocked_win32_api([&]() { return sut(bWindowed, nResolution); }, moo_calls, nCursorDisplayCount);

			D2GfxMockedCallsStrc original_calls;
			const auto original_result = call_with_mocked_win32_api([&]() { return original(bWindowed, nResolution); }, original_calls, nCursorDisplayCount);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface and to the Win32 API
			check_mocked_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");

			// Check specific values
			CHECK_EQ(moo_globals.hWnd, nScenario == 0 ? fake_window(0x1234) : nullptr);
		}
	}

	TEST_CASE_FIXTURE(WindowFixture, "D2Gfx.0x6FA74820 (#10024)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_Destroy, dll_base + 0x00004820);

		const auto bCursorDisplayed = GENERATE(0, 1);
		const auto bHasWindow = GENERATE(0, 1);
		const auto bCloseSurfaceResult = GENERATE(0u, 1u);
		const BOOL bDestroyWindowResult = GENERATE(FALSE, TRUE);

		SUBCASE("")
		{
			const auto nCursorDisplayCount = random_signed_integer(-2, 2);

			const auto setup_data = [&](
				D2GfxGlobals& globals
			) {
				globals.bCursorDisplayed = bCursorDisplayed;
				globals.hWnd = bHasWindow ? fake_window(0x1234) : nullptr;
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			D2GraphicsInterfaceMock::return_value = bCloseSurfaceResult;
			D2GfxWin32ApiMock::bDestroyWindowResult = bDestroyWindowResult;

			// Call both implementations
			D2GfxMockedCallsStrc moo_calls;
			const auto moo_result = call_with_mocked_win32_api([&]() { return sut(); }, moo_calls, nCursorDisplayCount);

			D2GfxMockedCallsStrc original_calls;
			const auto original_result = call_with_mocked_win32_api([&]() { return original(); }, original_calls, nCursorDisplayCount);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface and to the Win32 API
			check_mocked_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA748B0 (#10025)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_SetPaused, dll_base + 0x000048B0);

		const auto bPaused = GENERATE(0, 1, static_cast<int32_t>(random_unsigned_integer()));

		SUBCASE("")
		{
			// Call both implementations
			sut(bPaused);
			original(bPaused);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(WindowFixture, "D2Gfx.0x6FA748C0 (#10026)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_GetState, dll_base + 0x000048C0);

		const auto bPaused = GENERATE(0, 1);
		const auto bWindowState = GENERATE(0, 1);
		const auto bHasWindow = GENERATE(0, 1);
		const BOOL bIsIconic = GENERATE(FALSE, TRUE);
		const auto bIsWindowed = GENERATE(0, 1);

		SUBCASE("")
		{
			D2WindowPlacementStrc pWindowPlacements[4];
			generate_window_placements(pWindowPlacements);
			const auto nCursorDisplayCount = random_signed_integer(-2, 2);

			const auto setup_data = [&](
				D2GfxGlobals& globals
			) {
				globals.bPaused = bPaused;
				globals.bWindowState = bWindowState;
				globals.hWnd = bHasWindow ? fake_window(0x1234) : nullptr;
				globals.bIsWindowed = bIsWindowed;
				std::memcpy(globals.WindowPlacement, pWindowPlacements, sizeof(pWindowPlacements));
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			generate_auto_hide_bars();
			D2GfxWin32ApiMock::bIsIconic = bIsIconic;
			D2GraphicsInterfaceMock::return_value = random_unsigned_integer();

			// Call both implementations
			D2GfxMockedCallsStrc moo_calls;
			const auto moo_result = call_with_mocked_win32_api([&]() { return sut(); }, moo_calls, nCursorDisplayCount);

			D2GfxMockedCallsStrc original_calls;
			const auto original_result = call_with_mocked_win32_api([&]() { return original(); }, original_calls, nCursorDisplayCount);

			normalize_set_palette_calls(moo_calls.interface_calls, moo_globals);
			normalize_set_palette_calls(original_calls.interface_calls, original_globals);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface and to the Win32 API
			check_mocked_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA749C0 (#10027)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_GetWindow, dll_base + 0x000049C0);

		SUBCASE("")
		{
			// The window is only returned, a fake one is enough
			const auto hWnd = reinterpret_cast<HWND>(static_cast<uintptr_t>(random_unsigned_integer()));

			const auto setup_data = [hWnd](
				D2GfxGlobals& globals
			) {
				globals.hWnd = hWnd;
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();

			// Compare return values
			CHECK_EQ(moo_result, original_result);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA749D0 (#10041)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_GetDimensions, dll_base + 0x000049D0);

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same pointers
			int32_t nWidth = 0;
			int32_t nHeight = 0;
			const auto nReturnValue = random_unsigned_integer();

			// Call both implementations
			D2GraphicsInterfaceMock::return_value = nReturnValue;
			const auto moo_result = sut(&nWidth, &nHeight);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			const auto original_result = original(&nWidth, &nHeight);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(WindowFixture, "D2Gfx.0x6FA74A10 (#10029)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_ShowCursor, dll_base + 0x00004A10);

		const auto nDisplayType = GENERATE(DISPLAYTYPE_GDI, DISPLAYTYPE_DDRAW, DISPLAYTYPE_GLIDE, DISPLAYTYPE_OPENGL, DISPLAYTYPE_DIRECT3D, DISPLAYTYPE_RAVE);
		const auto bIsWindowed = GENERATE(0, 1, 2);
		const auto bCursorDisplayed = GENERATE(0, 1);
		const auto bShow = GENERATE(0, 1, 2);

		SUBCASE("")
		{
			const auto nCursorDisplayCount = random_signed_integer(-3, 3);

			const auto setup_data = [&](
				D2GfxGlobals& globals
			) {
				globals.nDisplayType = nDisplayType;
				globals.bIsWindowed = bIsWindowed;
				globals.bCursorDisplayed = bCursorDisplayed;
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			// Call both implementations
			D2GfxMockedCallsStrc moo_calls;
			call_with_mocked_win32_api([&]() { sut(bShow); }, moo_calls, nCursorDisplayCount);

			D2GfxMockedCallsStrc original_calls;
			call_with_mocked_win32_api([&]() { original(bShow); }, original_calls, nCursorDisplayCount);

			// Compare calls to the graphics interface and to the Win32 API
			check_mocked_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74A80 (#10028)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_IsFullScreen, dll_base + 0x00004A80);

		const auto bHasGraphicsInterface = GENERATE(0, 1);
		const auto bIsWindowed = GENERATE(0, 1, static_cast<int32_t>(random_unsigned_integer()));

		SUBCASE("")
		{
			const auto setup_data = [bHasGraphicsInterface, bIsWindowed](
				D2GfxGlobals& globals
			) {
				if (!bHasGraphicsInterface)
				{
					globals.pGraphicsInterface = nullptr;
				}
				globals.bIsWindowed = bIsWindowed;
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			// Call both implementations
			const auto moo_result = sut();
			const auto original_result = original();

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74AA0 (#10043)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_Activate, dll_base + 0x00004AA0);

		const auto bActive = GENERATE(0, 1, static_cast<int32_t>(random_unsigned_integer()));
		const auto bIsWindowed = GENERATE(0, 1);

		SUBCASE("")
		{
			const auto nReturnValue = random_unsigned_integer();

			const auto setup_data = [bIsWindowed](
				D2GfxGlobals& globals
			) {
				globals.bIsWindowed = bIsWindowed;
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			// Call both implementations
			D2GraphicsInterfaceMock::return_value = nReturnValue;
			const auto moo_result = sut(bActive);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			const auto original_result = original(bActive);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(WindowFixture, "D2Gfx.0x6FA74AE0 (#10030)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_Resize, dll_base + 0x00004AE0);

		// Only the supported resolutions, an unknown resolution is a fatal error that would end the tests
		const auto nResolution = GENERATE(D2GAMERES_640x480, D2GAMERES_800x600, NUM_GAME_RESOLUTIONS, D2GAMERES_1344x700);
		const auto nCurrentResolution = GENERATE(D2GAMERES_640x480, D2GAMERES_800x600);
		const auto bForceResize = GENERATE(0, 1);
		const auto bIsWindowed = GENERATE(0, 1, 2);

		SUBCASE("")
		{
			const auto setup_data = [nCurrentResolution, bIsWindowed](
				D2GfxGlobals& globals
			) {
				globals.nResolutionMode = nCurrentResolution;
				globals.bIsWindowed = bIsWindowed;
				globals.hWnd = fake_window(0x1234);
				globals.nWindowBorderHeight = 1234;
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			D2GraphicsInterfaceMock::return_value = random_unsigned_integer();

			// Call both implementations
			D2GfxMockedCallsStrc moo_calls;
			const auto moo_result = call_with_mocked_win32_api([&]() { return sut(nResolution, bForceResize); }, moo_calls);

			D2GfxMockedCallsStrc original_calls;
			const auto original_result = call_with_mocked_win32_api([&]() { return original(nResolution, bForceResize); }, original_calls);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface and to the Win32 API
			check_mocked_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74C60 (#10031)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_ClearCaches, dll_base + 0x00004C60);

		SUBCASE("")
		{
			// Call both implementations
			sut();
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original();
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74C70 (#10032)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_PlayCutscene, dll_base + 0x00004C70);

		SUBCASE("")
		{
			// Call both implementations
			sut();
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original();
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(WindowFixture, "D2Gfx.0x6FA74CA0 (#10033)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_EndCutScene, dll_base + 0x00004CA0);

		// Only the supported resolutions, an unknown resolution is a fatal error that would end the tests
		const auto nResolution = GENERATE(D2GAMERES_640x480, D2GAMERES_800x600, NUM_GAME_RESOLUTIONS, D2GAMERES_1344x700);
		const auto bPaused = GENERATE(0, 1);
		const auto bWindowState = GENERATE(0, 1);
		const auto bHasWindow = GENERATE(0, 1);
		const BOOL bIsIconic = GENERATE(FALSE, TRUE);
		const auto bIsWindowed = GENERATE(0, 1);

		SUBCASE("")
		{
			D2WindowPlacementStrc pWindowPlacements[4];
			generate_window_placements(pWindowPlacements);
			const auto nCursorDisplayCount = random_signed_integer(-2, 2);

			const auto setup_data = [&](
				D2GfxGlobals& globals
			) {
				globals.bPaused = bPaused;
				globals.bWindowState = bWindowState;
				globals.bIsWindowed = bIsWindowed;
				globals.hWnd = bHasWindow ? fake_window(0x1234) : nullptr;
				globals.nResolutionMode = D2GAMERES_640x480;
				std::memcpy(globals.WindowPlacement, pWindowPlacements, sizeof(pWindowPlacements));
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			generate_auto_hide_bars();
			D2GfxWin32ApiMock::bIsIconic = bIsIconic;

			// Failing to resize the window is a fatal error
			D2GraphicsInterfaceMock::return_value = 1;

			// Call both implementations
			D2GfxMockedCallsStrc moo_calls;
			call_with_mocked_win32_api([&]() { sut(nResolution); }, moo_calls, nCursorDisplayCount);

			D2GfxMockedCallsStrc original_calls;
			call_with_mocked_win32_api([&]() { original(nResolution); }, original_calls, nCursorDisplayCount);

			normalize_set_palette_calls(moo_calls.interface_calls, moo_globals);
			normalize_set_palette_calls(original_calls.interface_calls, original_globals);

			// Compare calls to the graphics interface and to the Win32 API
			check_mocked_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74DE0 (#10034)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_OpenSmackCutscene, dll_base + 0x00004DE0);

		const auto nResolution = GENERATE(D2GAMERES_640x480, D2GAMERES_800x600);

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface
			const char* szFile = "data\\local\\video\\New_Bliz640x480.bik";

			// Call both implementations
			sut(szFile, nResolution, dummy_cutscene_frame);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(szFile, nResolution, dummy_cutscene_frame);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74E20 (#10035)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_CheckCutScene, dll_base + 0x00004E20);

		const auto nReturnValue = GENERATE(0u, 1u, random_unsigned_integer());

		SUBCASE("")
		{
			// Call both implementations
			D2GraphicsInterfaceMock::return_value = nReturnValue;
			const auto moo_result = sut();
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			const auto original_result = original();
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74E50 (#10036)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_DecodeSmacker, dll_base + 0x00004E50);

		REPEAT_5();

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface so both implementations get the same buffer
			const char* szSmacker = "data\\local\\video\\d2intro640x292.bik";
			uint8_t pBuffer[16] = {};
			const int32_t nVersion = random_signed_integer(0, 10);

			// Call both implementations
			sut(szSmacker, pBuffer, nVersion);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(szSmacker, pBuffer, nVersion);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74E90 (#10037)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_PlaySmacker, dll_base + 0x00004E90);

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface
			uint8_t pContext[16] = {};

			// Call both implementations
			sut(pContext);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(pContext);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(GfxFixture, "D2Gfx.0x6FA74ED0 (#10038)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_CloseSmacker, dll_base + 0x00004ED0);

		SUBCASE("")
		{
			// Input data, only forwarded to the graphics interface
			uint8_t pContext[16] = {};

			// Call both implementations
			sut(pContext);
			auto moo_calls = D2GraphicsInterfaceMock::take_calls();

			original(pContext);
			auto original_calls = D2GraphicsInterfaceMock::take_calls();

			// Compare calls to the graphics interface
			check_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(WindowFixture, "D2Gfx.0x6FA74F10 (#10039)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_UpdatePlacement, dll_base + 0x00004F10);

		const auto bIsWindowed = GENERATE(0, 1, 2);

		REPEAT_5();

		SUBCASE("")
		{
			D2WindowPlacementStrc pWindowPlacements[4];
			generate_window_placements(pWindowPlacements);

			const auto setup_data = [&](
				D2GfxGlobals& globals
			) {
				globals.bIsWindowed = bIsWindowed;
				globals.hWnd = fake_window(0x1234);
				std::memcpy(globals.WindowPlacement, pWindowPlacements, sizeof(pWindowPlacements));
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			generate_auto_hide_bars();

			// Call both implementations
			D2GfxMockedCallsStrc moo_calls;
			call_with_mocked_win32_api([&]() { sut(); }, moo_calls);

			D2GfxMockedCallsStrc original_calls;
			call_with_mocked_win32_api([&]() { original(); }, original_calls);

			// Compare calls to the graphics interface and to the Win32 API
			check_mocked_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}

	TEST_CASE_FIXTURE(WindowFixture, "D2Gfx.0x6FA74FA0 (#10040)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(WINDOW_ShowAll, dll_base + 0x00004FA0);

		const auto bIsWindowed = GENERATE(0, 1, 2);

		REPEAT_5();

		SUBCASE("")
		{
			D2WindowPlacementStrc pWindowPlacements[4];
			generate_window_placements(pWindowPlacements);

			const auto setup_data = [&](
				D2GfxGlobals& globals
			) {
				globals.bIsWindowed = bIsWindowed;
				std::memcpy(globals.WindowPlacement, pWindowPlacements, sizeof(pWindowPlacements));
			};

			setup_data(moo_globals);
			setup_data(original_globals);

			// Call both implementations
			D2GfxMockedCallsStrc moo_calls;
			call_with_mocked_win32_api([&]() { sut(); }, moo_calls);

			D2GfxMockedCallsStrc original_calls;
			call_with_mocked_win32_api([&]() { original(); }, original_calls);

			// Compare calls to the graphics interface and to the Win32 API
			check_mocked_calls_eq(moo_calls, original_calls);

			// Compare potentially modified global data
			check_globals_eq("Comparing globals");
		}
	}
}

#endif
