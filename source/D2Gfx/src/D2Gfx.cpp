#include "D2Gfx.h"

#include <algorithm>

#include <Fog.h>

#include "CmnSubtile.h"
#include "DisplayType.h"
#include "Window.h"


extern int32_t gbIsWindowed;
extern int32_t gnResolutionMode;
extern int32_t gbNoWindowCreated_6FA8D848;
extern HINSTANCE ghInstance;


constexpr const char* gszDriverDllNames[NUMBER_OF_DISPLAYTYPES] = { "", "D2Gdi.dll", "", "D2DDraw.dll", "D2Glide.dll", "D2OpenGL.dll", "D2Direct3D.dll", "D2Rave.dll" };

WNDPROC gpfWndProc;
PALETTEENTRY gpPalette_6FA8D278[256];

HMODULE ghRenderModule;
D2RenderCallbackStrc* gpRenderCallbacks;
int32_t gnDisplayType;
int32_t gnViewMatrixAdjust_6FA8D254;

D2GfxSettingsStrc gGfxSettings = { 0, 1, 0, 155, 1, 1 };

constexpr D2GfxHelperStrc gpGfxHelpers =
{
    D2GFX_FillYBufferTable_6FA71010,
    D2GFX_DrawVisTile_6FA72730,
    sub_6FA72900,
    D2GFX_DrawGroundTile_6FA73410,
    TILE_DrawWallTile_6FA729D0,
    D2GFX_DrawBlendedVisTile_6FA72B80,
    D2GFX_DrawRoofTile_6FA72DC0
};


//D2Gfx.0x6FA73750 (#10000)
int32_t __stdcall D2GFX_Initialize(HINSTANCE hInstance, WNDPROC pfWndProc, int32_t nDisplayType, int32_t bWindowed)
{
    gpfWndProc = pfWndProc;
    ghInstance = hInstance;

    WNDCLASSA wndClass = {};
    if (nDisplayType == DISPLAYTYPE_OPENGL)
    {
        wndClass.style = CS_OWNDC;
    }
    wndClass.lpfnWndProc = pfWndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = (HICON)LoadImageA(ghInstance, (LPCSTR)(FOG_IsExpansion() ? 103 : 102), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
    wndClass.hCursor = LoadCursorA(nullptr, (LPCSTR)0x7F00);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = "Diablo II";
    wndClass.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW);

    if (!RegisterClassA(&wndClass))
    {
    	//if (sub_6FA750A7(GetLastError()))
    	//{
    	//	__debugbreak();
    	//	dword_6FA8D854 = 0;
    	//}
        return 0;
    }

    D2GFX_InitGouraudCache_6FA71000();

    atexit(j_D2GFX_10040_ShowAllWindows);

    gnDisplayType = nDisplayType;
    gbIsWindowed = bWindowed;

    if (nDisplayType < 0 || nDisplayType >= std::size(gszDriverDllNames))
    {
        FOG_Trace("Unsupported video mode - %d", nDisplayType);
        return 0;
    }

    ghRenderModule = LoadLibraryA(gszDriverDllNames[nDisplayType]);
    if (!ghRenderModule)
    {
        char szErrorMessage[256] = {};
        wsprintfA(szErrorMessage, "Can't find DLL: %s", gszDriverDllNames[nDisplayType]);
        FOG_DisplayHalt(szErrorMessage, __FILE__, __LINE__);
        exit(-1);
    }

    FOG_10233(gszDriverDllNames[nDisplayType], 1);

    D2RenderCallbackStrc* (__fastcall * pfGetRenderCallbacks)() = (D2RenderCallbackStrc * (__fastcall*)())GetProcAddress(ghRenderModule, (LPCSTR)10000);

    if (!pfGetRenderCallbacks)
    {
        char szErrorMessage[256] = {};
        wsprintfA(szErrorMessage, "Error interfacing with Gfx DLL #%i", GetLastError());
        FOG_DisplayHalt(szErrorMessage, __FILE__, __LINE__);
        exit(-1);
    }

    gpRenderCallbacks = pfGetRenderCallbacks();
    gpRenderCallbacks->pfSetOption(8, bWindowed == 1);

    if (!gpRenderCallbacks->pfInitialize(hInstance))
    {
        //Fog_10028(9, 0, 0, "%s", "Error 1: Diablo II is unable to proceed.  Please run D2VidTst and try again.");
        return 0;
    }

    int32_t bPerspectiveCapable = gGfxSettings.bPerspectiveCapable;
    if (!D2GFX_HardwareAcceleratedRenderMode())
    {
        bPerspectiveCapable = 0;
    }
    gGfxSettings.bPerspectiveCapable = bPerspectiveCapable;

    return gpRenderCallbacks->pfInitPerspective(&gGfxSettings, &gpGfxHelpers);
}

//D2Gfx.0x6FA73980
void __cdecl j_D2GFX_10040_ShowAllWindows()
{
    WINDOW_ShowAll();
}

//D2Gfx.0x6FA73990 (#10002)
int32_t __stdcall D2GFX_ReturnFalse()
{
    return 0;
}

//D2Gfx.0x6FA739A0 (#10001)
int32_t __stdcall D2GFX_Release()
{
    gpRenderCallbacks->pfRelease();
    UnregisterClassA("Diablo II", ghInstance);

    if (gbNoWindowCreated_6FA8D848)
    {
        const char* szMsg = nullptr;
        switch (gnDisplayType)
        {
        case DISPLAYTYPE_GDI:
            szMsg = "Error 21: A critical error has occurred while initializing windowed mode.";
            break;

        case DISPLAYTYPE_DDRAW:
            szMsg = "Error 22: A critical error has occurred while initializing DirectDraw.";
            break;

        case DISPLAYTYPE_GLIDE:
            szMsg = "Error 23: A critical error has occurred while initializing Glide.";
            break;

        case DISPLAYTYPE_OPENGL:
            szMsg = "Error 24: A critical error has occurred while initializing OpenGL.";
            break;

        case DISPLAYTYPE_DIRECT3D:
            szMsg = "Error 25: A critical error has occurred while initializing Direct3D.";
            break;

        default:
            break;
        }

        if (szMsg)
        {
            MessageBoxA(nullptr, szMsg, "Diablo II Critical Error", MB_ICONEXCLAMATION);
        }
    }

    D2_ASSERT(ghRenderModule != nullptr);

    const int32_t bResult = FreeLibrary(ghRenderModule);
    FOG_10233(gszDriverDllNames[gnDisplayType], 0);
    ghRenderModule = nullptr;
    gpRenderCallbacks = nullptr;
    return bResult;
}

//D2Gfx.0x6FA73AC0 (#10003)
int32_t __stdcall D2GFX_GetRenderMode()
{
    return gnDisplayType;
}

//D2Gfx.0x6FA73AD0 (#10004)
int32_t __stdcall D2GFX_CheckWindowed()
{
    return gbIsWindowed;
}

//D2Gfx.0x6FA73AE0 (#10005)
int32_t __stdcall D2GFX_GetResolutionMode()
{
    return gnResolutionMode;
}

//D2Gfx.0x6FA73AF0 (#10007)
int32_t __stdcall D2GFX_CheckUnitPerspective()
{
    return gGfxSettings.bPerspectiveCapable;
}

//D2Gfx.0x6FA73B00 (#10008)
void __stdcall D2GFX_SetUnitPerspective(int32_t bPerspective)
{
    if (D2GFX_HardwareAcceleratedRenderMode())
    {
        gGfxSettings.bPerspectiveCapable = bPerspective;
    }
    else
    {
        gGfxSettings.bPerspectiveCapable = 0;
    }
}

//D2Gfx.0x6FA73B20 (#10047)
int32_t __stdcall D2GFX_HardwareAcceleratedRenderMode()
{
    return gnDisplayType >= 4;
}

//D2Gfx.0x6FA73B30 (#10009)
int32_t __stdcall D2GFX_TogglePerspective()
{
    gGfxSettings.bPerspectiveEnabled = gGfxSettings.bPerspectiveEnabled == 0;
    return gGfxSettings.bPerspectiveEnabled;
}

//D2Gfx.0x6FA73B50 (#10010)
int32_t __stdcall D2GFX_CheckPerspective()
{
    return gGfxSettings.bPerspectiveCapable != 0 ? gGfxSettings.bPerspectiveEnabled : 0;
}

//D2Gfx.0x6FA73B70 (#10011)
void __stdcall D2GFX_SetPerspective(int32_t bPerspective)
{
    gGfxSettings.bPerspectiveEnabled = bPerspective;
}

//D2Gfx.0x6FA73B80 (#10012)
int32_t __stdcall D2GFX_ToggleBlendedShadows()
{
    gGfxSettings.bBlendedShadows = gGfxSettings.bBlendedShadows == 0;
    return gGfxSettings.bBlendedShadows;
}

//D2Gfx.0x6FA73BA0 (#10013)
int32_t __stdcall D2GFX_CheckBlendedShadows()
{
    return gGfxSettings.bBlendedShadows;
}

//D2Gfx.0x6FA73BB0 (#10014)
void __stdcall D2GFX_SetBlendShadows(int32_t bBlendedShadows)
{
    gGfxSettings.bBlendedShadows = bBlendedShadows;
}

//D2Gfx.0x6FA73BC0 (#10015)
int32_t __stdcall D2GFX_ToggleLowQuality()
{
    gGfxSettings.bLowQuality = gGfxSettings.bLowQuality == 0;
    return gGfxSettings.bLowQuality;
}

//D2Gfx.0x6FA73BE0 (#10016)
int32_t __stdcall D2GFX_CheckLowQuality()
{
    return gGfxSettings.bLowQuality;
}

//D2Gfx.0x6FA73BF0 (#10017)
void __stdcall D2GFX_SetLowQuality(int32_t bLowQuality)
{
    gGfxSettings.bLowQuality = bLowQuality;
}

//D2Gfx.0x6FA73C00 (#10018)
int32_t __stdcall D2GFX_SetGamma(uint32_t dwGamma)
{
    if (dwGamma)
    {
        gGfxSettings.nGamma = dwGamma;
    }

    return gpRenderCallbacks->pfSetGamma(gGfxSettings.nGamma);
}

//D2Gfx.0x6FA73C20 (#10019)
int32_t __stdcall D2GFX_CheckGamma()
{
    return gpRenderCallbacks->pfCheckGamma();
}

//D2Gfx.0x6FA73C30 (#10020)
void __stdcall D2GFX_EnableVSync()
{
    gGfxSettings.bVSync = 1;
}

//D2Gfx.0x6FA73C40 (#10021)
uint32_t* __stdcall D2GFX_GetRenderStatistics()
{
    return (uint32_t*)gpRenderCallbacks->pfGetRenderStatistics();
}

//D2Gfx.0x6FA73C50 (#10022)
void __fastcall D2GFX_SetViewMatrixAdjust(int32_t nAdjust)
{
    gnViewMatrixAdjust_6FA8D254 = nAdjust;
}

//D2Gfx.0x6FA73C60 (#10042)
int32_t __fastcall D2GFX_SetOption(int32_t nOption, int32_t nValue)
{
    return gpRenderCallbacks->pfSetOption(nOption, nValue);
}

//D2Gfx.0x6FA73C90 (#10044)
int32_t __stdcall D2GFX_BeginScene(int32_t bClear, uint8_t nRed, uint8_t nGreen, uint8_t nBlue)
{
    return gpRenderCallbacks->pfBeginScene(bClear, nRed, nGreen, nBlue);
}

//D2Gfx.0x6FA73CB0 (#10045)
int32_t __stdcall D2GFX_EndScene()
{
    gpRenderCallbacks->pfEndScene1();
    gpRenderCallbacks->pfEndScene2();
    return 1;
}

//D2Gfx.0x6FA73CD0 (#10046)
int32_t __stdcall D2GFX_GetBackBuffer(uint8_t* pBuffer)
{
    return gpRenderCallbacks->pfGetBackBuffer(pBuffer);
}

//D2Gfx.0x6FA73CE0 (#10051)
void __stdcall D2GFX_DrawRect(RECT* pRect, uint8_t nPaletteIndex)
{
    return gpRenderCallbacks->pfDrawRect(pRect, nPaletteIndex);
}

//D2Gfx.0x6FA73D00 (#10052)
void __stdcall D2GFX_DrawRectEx(RECT* pRect, uint8_t nPaletteIndex)
{
    return gpRenderCallbacks->pfDrawRectEx(pRect, nPaletteIndex);
}

//D2Gfx.0x6FA73D20 (#10053)
void __stdcall D2GFX_DrawSolidRect(RECT* pRect, uint8_t nPaletteIndex)
{
    return gpRenderCallbacks->pfDrawSolidRect(pRect, nPaletteIndex);
}

//D2Gfx.0x6FA73D40 (#10054)
void __stdcall D2GFX_DrawSolidSquare(POINT* pPoint, uint8_t nSize, uint8_t nPaletteIndex)
{
    return gpRenderCallbacks->pfDrawSolidSquare(pPoint, nSize, nPaletteIndex);
}

//D2Gfx.0x6FA73D60 (#10055)
void __stdcall D2GFX_DrawSolidRectEx(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, DrawMode eDrawMode)
{
    if (nXEnd >= nXStart && nYEnd >= nYStart)
    {
        gpRenderCallbacks->pfDrawSolidRectEx(nXStart, nYStart, nXEnd, nYEnd, dwColor, eDrawMode);
    }
}

//D2Gfx.0x6FA73DA0 (#10056)
void __stdcall D2GFX_DrawSolidRectAlpha(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, uint8_t nAlpha)
{
    if (nXEnd >= nXStart && nYEnd >= nYStart)
    {
        gpRenderCallbacks->pfDrawSolidRectAlpha(nXStart, nYStart, nXEnd, nYEnd, dwColor, nAlpha);
    }
}

//D2Gfx.0x6FA73DE0 (#10057)
void __stdcall D2GFX_DrawLine(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint8_t nColor, uint8_t nAlpha)
{
    return gpRenderCallbacks->pfDrawLine(nXStart, nYStart, nXEnd, nYEnd, nColor, nAlpha);
}

//D2Gfx.0x6FA73E10 (#10058)
void __stdcall D2GFX_ClearScreen(int32_t bPartial)
{
    return gpRenderCallbacks->pfClearScreen(bPartial);
}

//D2Gfx.0x6FA73E30 (#10079)
int32_t __stdcall D2GFX_DrawGroundTile(D2TileLibraryEntryStrc* pTile, D2GfxLightExStrc* pLight, int32_t nXPos, int32_t nYPos, int32_t nWorldXpos, int32_t nWorldYpos, uint8_t nAlpha, int32_t nScreenPanels, void* pTileData)
{
    return gpRenderCallbacks->pfDrawGroundTile(pTile, pLight, nXPos, nYPos, nWorldXpos, nWorldYpos, nAlpha, nScreenPanels, pTileData);
}

//D2Gfx.0x6FA73E70 (#10048)
void __stdcall D2GFX_SetPalette(PALETTEENTRY* pPalette)
{
    if (pPalette)
    {
        memcpy(gpPalette_6FA8D278, pPalette, sizeof(gpPalette_6FA8D278));
        gpRenderCallbacks->pfSetPalette(pPalette);
    }
    else
    {
        gpRenderCallbacks->pfSetPalette(gpPalette_6FA8D278);
    }
}

//D2Gfx.0x6FA73EB0 (#10049)
void __stdcall D2GFX_SetPaletteTable(PALETTEENTRY** pPaletteTable)
{
    gpRenderCallbacks->pfSetPaletteTable(pPaletteTable);
    sub_6FA71070(pPaletteTable);
}

//D2Gfx.0x6FA73ED0 (#10050)
void __stdcall D2GFX_SetGlobalLight(uint8_t nRed, uint8_t nGreen, uint8_t nBlue)
{
    return gpRenderCallbacks->pfSetGlobalLight(nRed, nGreen, nBlue);
}

//D2Gfx.0x6FA73EF0 (#10069)
void __cdecl D2GFX_DrawString(int32_t nXPos, int32_t nYPos, const char* szFormat, ...)
{
    va_list va;

    va_start(va, szFormat);
    return gpRenderCallbacks->pfDrawString(nXPos, nYPos, szFormat, va);
}

//D2Gfx.0x6FA73F10 (#10070)
void __stdcall D2GFX_DrawLight(uint32_t* pLight, uint32_t* pPlayerLight, int32_t nXPos, int32_t nYPos)
{
    return gpRenderCallbacks->pfDrawLight(pLight, pPlayerLight, nXPos, nYPos);
}
