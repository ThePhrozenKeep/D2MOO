#pragma once


#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#include <Windows.h>

#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>

#include <D2Constants.h>

#include <D2Gfx.h>
#include <DisplayType.h>
#include <Window.h>

#include <D2Gfx/D2GfxVisitorDefinitions.h>
#include <Fixtures/D2Gfx/D2GfxOriginalDll.h>
#include <Fixtures/D2Gfx/D2GraphicsInterfaceMockFixture.h>


// Globals of the D2MOO implementation that are not declared in a header
extern WNDPROC gpfWndProc;
extern HINSTANCE ghInstance;
extern HMODULE ghRenderModule;
extern DisplayType gnDisplayType;
extern int32_t gnViewMatrixAdjust;
extern int32_t gbInDebugBreak;
extern PALETTEENTRY gpPalette[256];
extern int32_t gnChangedScaleFactor;
extern int32_t gnScaleFactor;
extern int32_t gnPerspectiveScaleX;
extern int32_t gnPerspectiveScaleY;
extern HWND ghWnd;
extern int32_t gbIsWindowed;
extern D2GameResolutionMode gnResolutionMode;
extern int32_t gnWindowBorderHeight;
extern int32_t gbNoWindowCreated;
extern int32_t gbPaused;
extern int32_t gbWindowState;
extern int32_t gbCursorDisplayed;
extern D2WindowPlacementStrc gWindowPlacement[4];
extern char gszResolutionErrorBuffer[256];


// Copy of the globals of D2Gfx.cpp, Scale.cpp and Window.cpp, used to compare the state of both implementations
struct D2GfxStateStrc
{
	uint32_t nWndProc;						// Function pointers cannot be visited, the address is compared instead
	HINSTANCE hInstance;
	HMODULE hRenderModule;
	D2GraphicsInterfaceStrc* pGraphicsInterface;
	DisplayType nDisplayType;
	D2GfxSettingsStrc GfxSettings;
	int32_t nViewMatrixAdjust;
	int32_t bInDebugBreak;
	PALETTEENTRY Palette[256];
	int32_t nChangedScaleFactor;
	int32_t nScaleFactor;
	int32_t nPerspectiveScaleX;
	int32_t nPerspectiveScaleY;
	HWND hWnd;
	int32_t bIsWindowed;
	D2GameResolutionMode nResolutionMode;
	int32_t nWindowBorderHeight;
	int32_t bNoWindowCreated;
	int32_t bPaused;
	int32_t bWindowState;
	int32_t bCursorDisplayed;
	D2WindowPlacementStrc WindowPlacement[4];
	char szResolutionErrorBuffer[256];
};

BEGIN_VISIT(D2GfxStateStrc)
	FIELD(nWndProc)
	FIELD(hInstance)
	FIELD(hRenderModule)
	FIELD(pGraphicsInterface)
	FIELD(nDisplayType)
	OBJECT(GfxSettings)
	FIELD(nViewMatrixAdjust)
	FIELD(bInDebugBreak)
	ARRAY(Palette)
	FIELD(nChangedScaleFactor)
	FIELD(nScaleFactor)
	FIELD(nPerspectiveScaleX)
	FIELD(nPerspectiveScaleY)
	FIELD(hWnd)
	FIELD(bIsWindowed)
	FIELD(nResolutionMode)
	FIELD(nWindowBorderHeight)
	FIELD(bNoWindowCreated)
	FIELD(bPaused)
	FIELD(bWindowState)
	FIELD(bCursorDisplayed)
	ARRAY(WindowPlacement)
	ARRAY(szResolutionErrorBuffer)
END_VISIT()


// References to the globals of one implementation
struct D2GfxGlobals
{
	WNDPROC& pfWndProc;
	HINSTANCE& hInstance;
	HMODULE& hRenderModule;
	D2GraphicsInterfaceStrc*& pGraphicsInterface;
	DisplayType& nDisplayType;
	D2GfxSettingsStrc& GfxSettings;
	int32_t& nViewMatrixAdjust;
	int32_t& bInDebugBreak;
	PALETTEENTRY(&Palette)[256];
	int32_t& nChangedScaleFactor;
	int32_t& nScaleFactor;
	int32_t& nPerspectiveScaleX;
	int32_t& nPerspectiveScaleY;
	HWND& hWnd;
	int32_t& bIsWindowed;
	D2GameResolutionMode& nResolutionMode;
	int32_t& nWindowBorderHeight;
	int32_t& bNoWindowCreated;
	int32_t& bPaused;
	int32_t& bWindowState;
	int32_t& bCursorDisplayed;
	D2WindowPlacementStrc(&WindowPlacement)[4];
	char(&szResolutionErrorBuffer)[256];

	D2GfxStateStrc snapshot() const
	{
		D2GfxStateStrc state{};
		state.nWndProc = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(pfWndProc));
		state.hInstance = hInstance;
		state.hRenderModule = hRenderModule;
		state.pGraphicsInterface = pGraphicsInterface;
		state.nDisplayType = nDisplayType;
		state.GfxSettings = GfxSettings;
		state.nViewMatrixAdjust = nViewMatrixAdjust;
		state.bInDebugBreak = bInDebugBreak;
		std::memcpy(state.Palette, Palette, sizeof(state.Palette));
		state.nChangedScaleFactor = nChangedScaleFactor;
		state.nScaleFactor = nScaleFactor;
		state.nPerspectiveScaleX = nPerspectiveScaleX;
		state.nPerspectiveScaleY = nPerspectiveScaleY;
		state.hWnd = hWnd;
		state.bIsWindowed = bIsWindowed;
		state.nResolutionMode = nResolutionMode;
		state.nWindowBorderHeight = nWindowBorderHeight;
		state.bNoWindowCreated = bNoWindowCreated;
		state.bPaused = bPaused;
		state.bWindowState = bWindowState;
		state.bCursorDisplayed = bCursorDisplayed;
		std::memcpy(state.WindowPlacement, WindowPlacement, sizeof(state.WindowPlacement));
		std::memcpy(state.szResolutionErrorBuffer, szResolutionErrorBuffer, sizeof(state.szResolutionErrorBuffer));
		return state;
	}
};

inline D2GfxGlobals make_moo_d2gfx_globals()
{
	return D2GfxGlobals{
		gpfWndProc,
		ghInstance,
		ghRenderModule,
		gpGraphicsInterface,
		gnDisplayType,
		gGfxSettings,
		gnViewMatrixAdjust,
		gbInDebugBreak,
		gpPalette,
		gnChangedScaleFactor,
		gnScaleFactor,
		gnPerspectiveScaleX,
		gnPerspectiveScaleY,
		ghWnd,
		gbIsWindowed,
		gnResolutionMode,
		gnWindowBorderHeight,
		gbNoWindowCreated,
		gbPaused,
		gbWindowState,
		gbCursorDisplayed,
		gWindowPlacement,
		gszResolutionErrorBuffer,
	};
}

inline D2GfxSettingsStrc& d2gfx_original_gfx_settings()
{
	// D2GFX_CheckLowQuality, D2GFX_CheckBlendedShadows and D2GFX_CheckUnitPerspective all read a member of gGfxSettings
	auto& settings = d2gfx_original_global_from_function<D2GfxSettingsStrc>(0x00003BE0, offsetof(D2GfxSettingsStrc, bLowQuality));
	if (&settings != &d2gfx_original_global_from_function<D2GfxSettingsStrc>(0x00003BA0, offsetof(D2GfxSettingsStrc, bBlendedShadows))
		|| &settings != &d2gfx_original_global_from_function<D2GfxSettingsStrc>(0x00003AF0, offsetof(D2GfxSettingsStrc, bPerspectiveCapable)))
	{
		throw std::runtime_error("Inconsistent addresses for gGfxSettings in the original D2Gfx.dll");
	}

	return settings;
}

inline D2GfxGlobals make_original_d2gfx_globals()
{
	return D2GfxGlobals{
		d2gfx_original_global<WNDPROC>(0x0001D260),								// gpfWndProc
		d2gfx_original_global<HINSTANCE>(0x0001D25C),							// ghInstance
		d2gfx_original_global<HMODULE>(0x0001D274),								// ghRenderModule
		d2gfx_original_global_from_function<D2GraphicsInterfaceStrc*>(0x00003C20),		// D2GFX_GammaCanBeControlled
		d2gfx_original_global_from_function<DisplayType>(0x00003AC0),					// D2GFX_GetDisplayType
		d2gfx_original_gfx_settings(),
		d2gfx_original_global<int32_t>(0x0001D254),								// gnViewMatrixAdjust
		d2gfx_original_global<int32_t>(0x0001D854),								// gbInDebugBreak
		d2gfx_original_global<PALETTEENTRY[256]>(0x0001D278),							// gpPalette
		d2gfx_original_global<int32_t>(0x0000E4A8),								// gnChangedScaleFactor
		d2gfx_original_global_from_function<int32_t>(0x00003F50),						// SCALE_GetFactor
		d2gfx_original_global<int32_t>(0x0001D678),								// gnPerspectiveScaleX
		d2gfx_original_global<int32_t>(0x0001D67C),								// gnPerspectiveScaleY
		d2gfx_original_global_from_function<HWND>(0x000049C0),							// WINDOW_GetWindow
		d2gfx_original_global_from_function<int32_t>(0x00003AD0),						// D2GFX_CheckWindowed
		d2gfx_original_global_from_function<D2GameResolutionMode>(0x00003AE0),			// D2GFX_GetResolutionMode
		d2gfx_original_global<int32_t>(0x0001D740),								// gnWindowBorderHeight
		d2gfx_original_global<int32_t>(0x0001D848),								// gbNoWindowCreated
		d2gfx_original_global<int32_t>(0x0001D84C),								// gbPaused
		d2gfx_original_global<int32_t>(0x0001D850),								// gbWindowState
		d2gfx_original_global<int32_t>(0x0000E5F0),								// gbCursorDisplayed
		d2gfx_original_global<D2WindowPlacementStrc[4]>(0x0001D680),				// gWindowPlacement
		d2gfx_original_global<char[256]>(0x0001D748),							// gszResolutionErrorBuffer
	};
}


// Makes both implementations use the graphics interface mock and gives access to their globals.
// The globals are reset to the same values for both implementations.
template<class Fixture>
struct D2GfxGlobalsFixture : Fixture
{
	D2GraphicsInterfaceStrc graphics_interface = make_graphics_interface_mock();
	D2GfxGlobals moo_globals = make_moo_d2gfx_globals();
	D2GfxGlobals original_globals = make_original_d2gfx_globals();

	D2GfxGlobalsFixture()
	{
		const auto setup_data = [this](
			D2GfxGlobals& globals
		) {
			globals.pfWndProc = nullptr;
			globals.hInstance = nullptr;
			globals.hRenderModule = nullptr;
			globals.pGraphicsInterface = &graphics_interface;
			globals.nDisplayType = DISPLAYTYPE_DDRAW;
			globals.GfxSettings = { 0, 1, 0, 155, 1, 1 };
			globals.nViewMatrixAdjust = 0;
			globals.bInDebugBreak = 0;
			std::memset(globals.Palette, 0, sizeof(globals.Palette));
			globals.nChangedScaleFactor = 256;
			globals.nScaleFactor = 256;
			globals.nPerspectiveScaleX = 0;
			globals.nPerspectiveScaleY = 0;
			globals.hWnd = nullptr;
			globals.bIsWindowed = 0;
			globals.nResolutionMode = D2GAMERES_640x480;
			globals.nWindowBorderHeight = 0;
			globals.bNoWindowCreated = 0;
			globals.bPaused = 0;
			globals.bWindowState = 0;
			globals.bCursorDisplayed = 1;
			std::memset(globals.WindowPlacement, 0, sizeof(globals.WindowPlacement));
			std::memset(globals.szResolutionErrorBuffer, 0, sizeof(globals.szResolutionErrorBuffer));
		};

		setup_data(moo_globals);
		setup_data(original_globals);

		D2GraphicsInterfaceMock::return_value = 0;
		D2GraphicsInterfaceMock::calls.clear();
		D2GraphicsInterfaceMock::output_strings.clear();
	}

	~D2GfxGlobalsFixture()
	{
		const auto reset_data = [](
			D2GfxGlobals& globals
		) {
			// Do not leave a dangling pointer to the mock
			globals.pGraphicsInterface = nullptr;
			globals.hRenderModule = nullptr;
			globals.hWnd = nullptr;
			// WINDOW_ShowAll (#10040) is called when the process exits if D2GFX_Initialize (#10000) was tested, do not let it show fake windows
			std::memset(globals.WindowPlacement, 0, sizeof(globals.WindowPlacement));
		};

		reset_data(moo_globals);
		reset_data(original_globals);
	}

	// Snapshots the globals of both implementations and compares them
	void check_globals_eq(const char* context_title)
	{
		auto moo_state = moo_globals.snapshot();
		auto original_state = original_globals.snapshot();
		MOO_CHECK_EQ(moo_state, original_state, context_title);
	}

	// Compares the calls to the graphics interface since the last call to take_calls()
	void check_calls_eq(std::vector<D2GfxInterfaceCallStrc>& moo_calls, std::vector<D2GfxInterfaceCallStrc>& original_calls)
	{
		check_interface_calls_eq(moo_calls, original_calls, "Comparing calls to the graphics interface");
	}
};
