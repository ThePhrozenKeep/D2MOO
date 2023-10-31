#include "Window.h"

#include <algorithm>

#include <shellapi.h>

#include <Fog.h>

#include "CmnSubtile.h"
#include "DisplayType.h"
#include "D2Gfx.h"


extern DisplayType gnDisplayType;


HWND ghWnd;
int32_t gbIsWindowed;
D2GameResolutionMode gnResolutionMode;
//int32_t dword_6FA8D740;
D2WindowPlacementStrc gWindowPlacement[4];
int32_t gbCursorDisplayed;
int32_t gbNoWindowCreated_6FA8D848;
int32_t gbPaused_6FA8D84C;
int32_t gbWindowState_6FA8D850;
HINSTANCE ghInstance;


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

    switch (nResolution)
    {
    case D2GAMERES_640x480:
        rect.right = 640;
        rect.bottom = 480;
        break;

    case D2GAMERES_800x600:
    case NUM_GAME_RESOLUTIONS:
        rect.right = 800;
        rect.bottom = 600;
        break;

    case D2GAMERES_1344x700:
        rect.right = 1344;
        rect.bottom = 700;
        break;

    default:
        static char szLocalBuffer[256];
        FOG_DisplayHalt(FOG_csprintf(szLocalBuffer, "Unknown resolution %d", nResolution), __FILE__, __LINE__);
        exit(-1);
    }

    //dword_6FA8D740 = Rect.bottom;

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
    
    //dword_6FA8D740 = nHeight - dword_6FA8D740;

    int32_t nX = 0;
    int32_t nY = 0;

    if (gbIsWindowed == 1)
    {
        RECT desktopRect = {};
        GetClientRect(GetDesktopWindow(), &desktopRect);

        nX = std::max((desktopRect.right - nWidth) / 2, 0l);
        nY = std::max((desktopRect.bottom - nHeight)  / 2, 0l);
    }
    else if (gnDisplayType == DISPLAYTYPE_GLIDE)
    {
        nWidth = std::max(GetSystemMetrics(0), 800);
        nHeight = std::max(GetSystemMetrics(1), 600);
    }

    if (GetSystemMetrics(0) < 800)
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
        //if (!sub_6FA750A7(GetLastError()))
        //    return 0;
        //__debugbreak();
        //dword_6FA8D854 = 0;
        return 0;
    }

    ShowWindow(ghWnd, 1);
    UpdateWindow(ghWnd);
    SetFocus(ghWnd);
    GdiSetBatchLimit(1u);

    if (gbCursorDisplayed)
    {
        while (ShowCursor(0) >= 0);
        gbCursorDisplayed = 0;
    }

    if (gpGraphicsInterface->pfCreateSurface(ghWnd, gnResolutionMode))
    {
        D2GFX_SetContrastAndGamma_6FA710C0();
        SetWindowPos(ghWnd, HWND_NOTOPMOST, 0, 0, nWidth, nHeight, SWP_NOMOVE);
        return 1;
    }

    gbNoWindowCreated_6FA8D848 = 1;
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

    return 0;
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
    gbPaused_6FA8D84C = bPaused;
}

//D2Gfx.0x6FA748C0 (#10026)
int32_t __stdcall WINDOW_GetState()
{
    if (gbWindowState_6FA8D850 == gbPaused_6FA8D84C || !ghWnd)
    {
        return gbWindowState_6FA8D850;
    }

    D2_ASSERT(gpGraphicsInterface);

    int32_t bPaused = gbPaused_6FA8D84C;
    if (!gbPaused_6FA8D84C)
    {
        if (IsIconic(ghWnd))
        {
            return gbWindowState_6FA8D850;
        }
        bPaused = gbPaused_6FA8D84C;
    }

    gbWindowState_6FA8D850 = bPaused;
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

    gpGraphicsInterface->pfPauseSurface(ghWnd, gnResolutionMode, gbWindowState_6FA8D850);

    if (!gbWindowState_6FA8D850)
    {
        D2GFX_SetPalette(nullptr);
        D2GFX_SetContrastAndGamma_6FA710C0();
    }

    return gbWindowState_6FA8D850;
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

        switch (nResolution)
        {
        case 0:
            Rect.right = 640;
            Rect.bottom = 480;
            break;

        case 1:
        case 2:
            Rect.right = 800;
            Rect.bottom = 600;
            break;

        case 3:
            Rect.right = 1344;
            Rect.bottom = 700;
            break;

        default:
            //FOG_DisplayHalt(Fog_10018(&unk_6FA8D748, "Unknown resolution %d", nResolution), __FILE__, __LINE__);
            exit(-1);
        }

        //dword_6FA8D740 = Rect.bottom;

        AdjustWindowRectEx(&Rect, WS_CAPTION, 0, WS_EX_APPWINDOW);

        const int32_t nWidth = Rect.right - Rect.left;
        const int32_t nHeight = Rect.bottom - Rect.top;
        //dword_6FA8D740 = nHeight - dword_6FA8D740;

        RECT ClientRect = {};
        GetClientRect(GetDesktopWindow(), &ClientRect);

        SetWindowPos(ghWnd, HWND_NOTOPMOST, 0, 0, nWidth, nHeight, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
    }

    const int32_t bResult = gpGraphicsInterface->pfChangeRes(ghWnd, nResolution);
    D2GFX_SetContrastAndGamma_6FA710C0();
    return bResult;
}

//D2Gfx.0x6FA74C60 (#10031)
void __stdcall WINDOW_ClearCaches()
{
    gpGraphicsInterface->pfClearCaches();
}

//D2Gfx.0x6FA74C70 (#10032)
void __stdcall WINDOW_PlayCutscene()
{
    D2_ASSERT(gpGraphicsInterface);

    gpGraphicsInterface->pfPlayCutscene();
}

//D2Gfx.0x6FA74CA0 (#10033)
void __stdcall WINDOW_EndCutScene(D2GameResolutionMode nResolution)
{
    D2_ASSERT(gpGraphicsInterface);

    if (gbWindowState_6FA8D850 != gbPaused_6FA8D84C && ghWnd && (gbPaused_6FA8D84C || !IsIconic(ghWnd)))
    {
        gbWindowState_6FA8D850 = gbPaused_6FA8D84C;
        if (gbPaused_6FA8D84C)
        {
            ShowWindow(ghWnd, 6);
            WINDOW_ShowAll();
            while (ShowCursor(1) < 0);
        }
        else
        {
            WINDOW_UpdatePlacement();
            while (ShowCursor(0) >= 0);
        }

        gpGraphicsInterface->pfPauseSurface(ghWnd, gnResolutionMode, gbWindowState_6FA8D850);

        if (gbWindowState_6FA8D850)
        {
            gnResolutionMode = nResolution;
            D2GFX_SetPalette(nullptr);
            return;
        }

        D2GFX_SetPalette(nullptr);
        D2GFX_SetContrastAndGamma_6FA710C0();
    }

    if (gbWindowState_6FA8D850)
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

    return gpGraphicsInterface->pfOpenSmackCutscene(szFile, nResolution, pfFrame);
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

        pWindowPlacement->hWnd = (HWND)SHAppBarMessage(7u, &pData);

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
