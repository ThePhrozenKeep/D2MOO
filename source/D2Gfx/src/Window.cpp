#include "Window.h"

#include <algorithm>
#include <iterator>

#include <shellapi.h>

#include <Fault.h>
#include <Fog.h>

#include "CmnSubtile.h"
#include "DisplayType.h"
#include "D2Gfx.h"


extern DisplayType gnDisplayType;
extern int32_t gbInDebugBreak;


//D2Gfx.0x6FA8D270
HWND ghWnd;
//D2Gfx.0x6FA8D268
int32_t gbIsWindowed;
//D2Gfx.0x6FA8D26C
D2GameResolutionMode gnResolutionMode;
//D2Gfx.0x6FA8D680
D2WindowPlacementStrc gWindowPlacement[4];
//D2Gfx.0x6FA8D740
int32_t gnWindowBorderHeight;
//D2Gfx.0x6FA8D748
char gszResolutionErrorBuffer[256];
//D2Gfx.0x6FA7E5F0
int32_t gbCursorDisplayed = 1;
//D2Gfx.0x6FA8D848
int32_t gbNoWindowCreated;
//D2Gfx.0x6FA8D84C
int32_t gbPaused;
//D2Gfx.0x6FA8D850
int32_t gbWindowState;
//D2Gfx.0x6FA8D25C
HINSTANCE ghInstance;


// Inlined in the original dll
static void WINDOW_GetResolutionRect(D2GameResolutionMode nResolution, RECT* pRect)
{
    *pRect = {};

    switch (nResolution)
    {
    case D2GAMERES_640x480:
        pRect->right = 640;
        pRect->bottom = 480;
        break;

    case D2GAMERES_800x600:
    case NUM_GAME_RESOLUTIONS:
        pRect->right = 800;
        pRect->bottom = 600;
        break;

    case D2GAMERES_1344x700:
        pRect->right = 1344;
        pRect->bottom = 700;
        break;

    default:
        FOG_DisplayHalt(FOG_csprintf(gszResolutionErrorBuffer, "Unknown resolution %d", nResolution), __FILE__, __LINE__);
        exit(-1);
    }
}

//D2Gfx.0x6FA74450 (#10023)
int32_t __stdcall WINDOW_Create(int32_t bWindowed, D2GameResolutionMode nResolution)
{
    if (FindWindowA("Diablo II", 0))
    {
        MessageBoxA(nullptr, "Only one copy of Diablo II may run at a time.", "Diablo II Critical Error", MB_ICONEXCLAMATION);
        return 0;
    }

    D2_ASSERT(gpGraphicsInterface);
    D2_ASSERT(!ghWnd);

    gbIsWindowed = bWindowed;
    gnResolutionMode = nResolution;

    RECT rect = {};
    WINDOW_GetResolutionRect(nResolution, &rect);
    gnWindowBorderHeight = rect.bottom;

    uint32_t dwStyle = 0;
    if (bWindowed == 1)
    {
        dwStyle = WS_CAPTION;
    }
    else if (gnDisplayType == DISPLAYTYPE_OPENGL)
    {
        dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
    }
    else
    {
        dwStyle = WS_POPUP | WS_SYSMENU;
    }

    AdjustWindowRectEx(&rect, dwStyle, 0, WS_EX_APPWINDOW);

    int32_t nWidth = rect.right - rect.left;
    int32_t nHeight = rect.bottom - rect.top;
    gnWindowBorderHeight = nHeight - gnWindowBorderHeight;

    int32_t nX = 0;
    int32_t nY = 0;

    if (gbIsWindowed == 1)
    {
        RECT desktopRect = {};
        GetClientRect(GetDesktopWindow(), &desktopRect);

        nX = std::max((desktopRect.right - nWidth) >> 1, 0l);
        nY = std::max((desktopRect.bottom - nHeight) >> 1, 0l);
    }
    else if (gnDisplayType == DISPLAYTYPE_GLIDE)
    {
        nWidth = std::max(GetSystemMetrics(SM_CXSCREEN), 800);
        nHeight = std::max(GetSystemMetrics(SM_CYSCREEN), 600);
    }

    if (GetSystemMetrics(SM_CXSCREEN) < 800)
    {
        DEVMODEA deviceMode = {};
        deviceMode.dmSize = sizeof(DEVMODEA);
        deviceMode.dmBitsPerPel = 8;
        deviceMode.dmPelsWidth = 800;
        deviceMode.dmPelsHeight = 600;
        deviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        ChangeDisplaySettingsA(&deviceMode, 0);
    }

    ghWnd = CreateWindowExA(WS_EX_APPWINDOW | WS_EX_TOPMOST, "Diablo II", "Diablo II", dwStyle, nX, nY, nWidth, nHeight, nullptr, nullptr, ghInstance, nullptr);
    if (!ghWnd)
    {
        if (XFaultBreakableError(GetLastError()))
        {
            gbInDebugBreak = 1;
            __debugbreak();
            gbInDebugBreak = 0;
        }
        return 0;
    }

    ShowWindow(ghWnd, SW_SHOWNORMAL);
    UpdateWindow(ghWnd);
    SetFocus(ghWnd);
    GdiSetBatchLimit(1u);

    if (gbCursorDisplayed)
    {
        while (ShowCursor(0) >= 0);
        gbCursorDisplayed = 0;
    }

    const int32_t bResult = gpGraphicsInterface->pfCreateSurface(ghWnd, gnResolutionMode);
    if (!bResult)
    {
        gbNoWindowCreated = 1;
        if (!gbCursorDisplayed)
        {
            while (ShowCursor(1) < 0);
            gbCursorDisplayed = 1;
        }

        if (ghWnd)
        {
            DestroyWindow(ghWnd);
            ghWnd = nullptr;
        }
    }

    CMNSUBTILE_SetContrastAndGamma();
    SetWindowPos(ghWnd, HWND_NOTOPMOST, 0, 0, nWidth, nHeight, SWP_NOMOVE);
    return bResult;
}

//D2Gfx.0x6FA74820 (#10024)
int32_t __stdcall WINDOW_Destroy()
{
    D2_ASSERT(gpGraphicsInterface);

    if (!gbCursorDisplayed)
    {
        while (ShowCursor(1) < 0);
        gbCursorDisplayed = 1;
    }

    int32_t bWindowDestroyed = 1;
    if (!gpGraphicsInterface->pfCloseSurface())
    {
        bWindowDestroyed = 0;
    }

    if (ghWnd)
    {
        if (!DestroyWindow(ghWnd))
        {
            bWindowDestroyed = 0;
        }
        ghWnd = nullptr;
    }

    ChangeDisplaySettingsA(nullptr, 0);
    return bWindowDestroyed;
}

//D2Gfx.0x6FA748B0 (#10025)
void __stdcall WINDOW_SetPaused(int32_t bPaused)
{
    gbPaused = bPaused;
}

//D2Gfx.0x6FA748C0 (#10026)
int32_t __stdcall WINDOW_GetState()
{
    if (gbWindowState == gbPaused || !ghWnd)
    {
        return gbWindowState;
    }

    D2_ASSERT(gpGraphicsInterface);

    int32_t bPaused = gbPaused;
    if (!gbPaused)
    {
        if (IsIconic(ghWnd))
        {
            return gbWindowState;
        }
        bPaused = gbPaused;
    }

    gbWindowState = bPaused;
    if (bPaused)
    {
        ShowWindow(ghWnd, SW_MINIMIZE);
        WINDOW_ShowAll();
        while (ShowCursor(1) < 0);
    }
    else
    {
        WINDOW_UpdatePlacement();
        while (ShowCursor(0) >= 0);
    }

    gpGraphicsInterface->pfPauseSurface(ghWnd, gnResolutionMode, gbWindowState);

    if (!gbWindowState)
    {
        D2GFX_SetPalette(nullptr);
        CMNSUBTILE_SetContrastAndGamma();
    }

    return gbWindowState;
}

//D2Gfx.0x6FA749C0 (#10027)
HWND __stdcall WINDOW_GetWindow()
{
    return ghWnd;
}

//D2Gfx.0x6FA749D0 (#10041)
int32_t __stdcall WINDOW_GetDimensions(int32_t* pWidth, int32_t* pHeight)
{
    D2_ASSERT(gpGraphicsInterface);

    return gpGraphicsInterface->pfGetDimensions(pWidth, pHeight);
}

//D2Gfx.0x6FA74A10 (#10029)
void __stdcall WINDOW_ShowCursor(int32_t bShow)
{
    if (!gbIsWindowed || gnDisplayType == DISPLAYTYPE_GLIDE || gnDisplayType == DISPLAYTYPE_OPENGL || gnDisplayType == DISPLAYTYPE_DIRECT3D)
    {
        return;
    }

    if (bShow)
    {
        if (gbCursorDisplayed)
        {
            while (ShowCursor(0) >= 0);
            gbCursorDisplayed = 0;
        }
    }
    else
    {
        if (!gbCursorDisplayed)
        {
            while (ShowCursor(1) < 0);
            gbCursorDisplayed = 1;
        }
    }
}

//D2Gfx.0x6FA74A80 (#10028)
int32_t __stdcall WINDOW_IsFullScreen()
{
    if (gpGraphicsInterface)
    {
        return gbIsWindowed == 0;
    }

    return 0;
}

//D2Gfx.0x6FA74AA0 (#10043)
int32_t __stdcall WINDOW_Activate(int32_t bActive)
{
    D2_ASSERT(gpGraphicsInterface);

    gbIsWindowed = bActive == 0;
    return gpGraphicsInterface->pfActivateWindow();
}

//D2Gfx.0x6FA74AE0 (#10030)
int32_t __stdcall WINDOW_Resize(D2GameResolutionMode nResolution, int32_t bForceResize)
{
    D2_ASSERT(gpGraphicsInterface);

    if (!bForceResize && nResolution == gnResolutionMode)
    {
        return 1;
    }

    gnResolutionMode = nResolution;

    if (gbIsWindowed == 1)
    {
        RECT Rect = {};
        WINDOW_GetResolutionRect(nResolution, &Rect);
        gnWindowBorderHeight = Rect.bottom;

        AdjustWindowRectEx(&Rect, WS_CAPTION, 0, WS_EX_APPWINDOW);

        const int32_t nWidth = Rect.right - Rect.left;
        const int32_t nHeight = Rect.bottom - Rect.top;
        gnWindowBorderHeight = nHeight - gnWindowBorderHeight;

        RECT ClientRect = {};
        GetClientRect(GetDesktopWindow(), &ClientRect);

        SetWindowPos(ghWnd, HWND_NOTOPMOST, 0, 0, nWidth, nHeight, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
    }

    const int32_t bResult = gpGraphicsInterface->pfChangeRes(ghWnd, nResolution);
    CMNSUBTILE_SetContrastAndGamma();
    return bResult;
}

//D2Gfx.0x6FA74C60 (#10031)
void __stdcall WINDOW_ClearCaches()
{
    gpGraphicsInterface->pfClearCaches();
}

//D2Gfx.0x6FA74C70 (#10032)
BOOL __stdcall WINDOW_PlayCutscene()
{
    D2_ASSERT(gpGraphicsInterface);

    return gpGraphicsInterface->pfPlayCutscene();
}

//D2Gfx.0x6FA74CA0 (#10033)
void __stdcall WINDOW_EndCutScene(D2GameResolutionMode nResolution)
{
    D2_ASSERT(gpGraphicsInterface);

    if (gbWindowState != gbPaused && ghWnd && (gbPaused || !IsIconic(ghWnd)))
    {
        gbWindowState = gbPaused;
        if (gbPaused)
        {
            ShowWindow(ghWnd, SW_MINIMIZE);
            WINDOW_ShowAll();
            while (ShowCursor(1) < 0);
        }
        else
        {
            WINDOW_UpdatePlacement();
            while (ShowCursor(0) >= 0);
        }

        gpGraphicsInterface->pfPauseSurface(ghWnd, gnResolutionMode, gbWindowState);

        if (gbWindowState)
        {
            gnResolutionMode = nResolution;
            D2GFX_SetPalette(nullptr);
            return;
        }

        D2GFX_SetPalette(nullptr);
        CMNSUBTILE_SetContrastAndGamma();
    }

    if (gbWindowState)
    {
        gnResolutionMode = nResolution;
    }
    else if (!WINDOW_Resize(nResolution, 1))
    {
        FOG_Trace("Failed to resize window after cutscenes... this is fatal!");
        exit(-1);
    }

    D2GFX_SetPalette(nullptr);
}

//D2Gfx.0x6FA74DE0 (#10034)
void __stdcall WINDOW_OpenSmackCutscene(const char* szFile, D2GameResolutionMode nResolution, void(__cdecl* pfFrame)())
{
    D2_ASSERT(gpGraphicsInterface);

    return gpGraphicsInterface->pfOpenSmackCutscene(szFile, nResolution, (void*)pfFrame);
}

//D2Gfx.0x6FA74E20 (#10035)
int32_t __stdcall WINDOW_CheckCutScene()
{
    D2_ASSERT(gpGraphicsInterface);

    return gpGraphicsInterface->pfCheckCutScene();
}

//D2Gfx.0x6FA74E50 (#10036)
void __stdcall WINDOW_DecodeSmacker(const char* szSmacker, uint8_t* pBuffer, int32_t nVersion)
{
    D2_ASSERT(gpGraphicsInterface);

    return gpGraphicsInterface->pfDecodeSmacker(szSmacker, pBuffer, nVersion);
}

//D2Gfx.0x6FA74E90 (#10037)
void __stdcall WINDOW_PlaySmacker(void* pContext)
{
    D2_ASSERT(gpGraphicsInterface);

    return gpGraphicsInterface->pfPlaySmacker(pContext);
}

//D2Gfx.0x6FA74ED0 (#10038)
void __stdcall WINDOW_CloseSmacker(void* pContext)
{
    D2_ASSERT(gpGraphicsInterface);

    return gpGraphicsInterface->pfCloseSmacker(pContext);
}

//D2Gfx.0x6FA74F10 (#10039)
void __stdcall WINDOW_UpdatePlacement()
{
    if (gbIsWindowed == 1)
    {
        return;
    }

    _AppBarData pData = {};
    pData.cbSize = sizeof(pData);
    pData.hWnd = ghWnd;

    for (int32_t i = 0; i < std::size(gWindowPlacement); ++i)
    {
        pData.uEdge = i;

        D2WindowPlacementStrc* pWindowPlacement = &gWindowPlacement[i];

        pWindowPlacement->hWnd = (HWND)SHAppBarMessage(ABM_GETAUTOHIDEBAR, &pData);

        if (pWindowPlacement->hWnd)
        {
            pWindowPlacement->windowPlacement.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(pWindowPlacement->hWnd, &pWindowPlacement->windowPlacement);
            ShowWindow(pWindowPlacement->hWnd, SW_HIDE);
        }
    }
}

//D2Gfx.0x6FA74FA0 (#10040)
void __stdcall WINDOW_ShowAll()
{
    if (gbIsWindowed == 1)
    {
        return;
    }

    for (int32_t i = 0; i < std::size(gWindowPlacement); ++i)
    {
        D2WindowPlacementStrc* pWindowPlacement = &gWindowPlacement[i];

        if (pWindowPlacement->hWnd)
        {
            ShowWindow(pWindowPlacement->hWnd, pWindowPlacement->windowPlacement.showCmd);
        }
    }
}
