#pragma once

#include <cstdint>

#include <Windows.h>


#include "DrawMode.h"


#pragma pack(push, 1)
struct D2GfxDataStrc;
struct D2GfxLightStrc;
struct D2GfxLightExStrc;
struct D2TileLibraryEntryStrc;


struct D2GfxSettingsStrc
{
	int32_t bPerspectiveEnabled;				//0x00
	int32_t bPerspectiveCapable;				//0x04
	int32_t bLowQuality;						//0x08
	int32_t nGamma;								//0x0C
	int32_t bVSync;								//0x10
	int32_t bBlendedShadows;					//0x14
};

struct D2GfxHelperStrc
{
	void(__fastcall* pfFillYBufferTable)(uint8_t* ppvBits, int32_t nWidth, int32_t nHeight, int32_t a4);
	void(__fastcall* pfDrawVisTile)(uint8_t* a1, int32_t a2, int32_t a3);
	void(__fastcall* pfUnknown)(uint8_t* a1, int32_t a2, int32_t a3, uint8_t a4);
	void(__fastcall* pfDrawGroundTile)(D2TileLibraryEntryStrc* pTileLibraryEntry, int32_t nX, int32_t nY, D2GfxLightExStrc* pLightEx);
	void(__fastcall* pfDrawWallTile)(uint8_t* a1, int32_t a2, int32_t a3, int32_t* a4);
	void(__fastcall* pfDrawBlendedVisTile)(int32_t a1, int32_t a2, int32_t a3, uint8_t a4);
	void(__fastcall* pfDrawRoofTile)(int32_t a1, int32_t a2, int32_t a3, int32_t* a4, uint8_t a5);
};

struct D2RenderCallbackStrc
{
	int32_t(__fastcall* pfInitialize)(HINSTANCE);
	int32_t(__fastcall* pfInitPerspective)(D2GfxSettingsStrc* pSettings, const D2GfxHelperStrc* pHelpers);
	int32_t(__fastcall* pfRelease)();
	int32_t(__fastcall* pfCreateWindow)(HWND hWnd, int32_t nResolutionMode);
	int32_t(__fastcall* pfDestroyWindow)();
	void(__fastcall* pfEndCutScene)(HWND hWnd, int32_t nResolutionMode, int32_t nWindowState);
	int32_t(__fastcall* pfBeginScene)(int32_t bClear, uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
	int32_t(__fastcall* pfEndScene1)();
	int32_t(__fastcall* pfEndScene2)();
	int32_t(__fastcall* pfResizeWindow)(HWND hWnd, int32_t bForceResize);
	int32_t(__fastcall* pfGetBackBuffer)(uint8_t* pBuffer);
	int32_t(__fastcall* pfActivateWindow)();
	int32_t(__fastcall* pfSetOption)(int32_t nOption, int32_t nValue);
	int32_t(__fastcall* pfBeginCutScene)();
	void(__fastcall* pfPlayCutScene)(const char* szFile, int32_t nResolutionMode, void* pfFrame);
	int32_t(__fastcall* pfCheckCutScene)();
	void(__fastcall* pfDecodeSmacker)(const char* szSmacker, uint8_t* pBuffer, int32_t nVersion);
	void(__fastcall* pfPlaySmacker)(void* pContext);
	void(__fastcall* pfCloseSmacker)(void* pContext);
	int32_t* (__fastcall* pfGetRenderStatistics)();
	int32_t(__fastcall* pfGetScreenSize)(int32_t* pWidth, int32_t* pHeight);
	void(__fastcall* pfUpdateScaleFactor)(int32_t nScaleFactor);
	int32_t(__fastcall* pfSetGamma)(int32_t nGamma);
	int32_t(__fastcall* pfCheckGamma)();
	void(__fastcall* pfSetPerspectiveScale)(int32_t nScaleX, int32_t nScaleY);
	void(__fastcall* pfAdjustPerspectivePosition)(int32_t nPosX, int32_t nPosY, int32_t nBais, int32_t* pXAdjust, int32_t* pYAdjust);
	void(__fastcall* pfScalePerspectivePosition)(int32_t nPosX, int32_t nPosY, int32_t nAngle, int32_t* pXAdjust, int32_t* pYAdjust, int32_t bOrder);
	void(__fastcall* pfSetDefaultPerspectiveFactor)();
	void(__fastcall* pfSetPalette)(LPPALETTEENTRY pPalette);
	void(__fastcall* pfSetPaletteTable)(LPPALETTEENTRY* pPaletteTable);
	void(__fastcall* pfSetGlobalLight)(uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
	int32_t(__fastcall* pfDrawGroundTile)(D2TileLibraryEntryStrc* pTile, D2GfxLightExStrc* pLight, int32_t nPosX, int32_t nPosY, int32_t nWorldXpos, int32_t nWorldYpos, uint8_t nAlpha, int32_t nScreenPanels, void* pTileData);
	void(__fastcall* pfDrawPerspectiveImage)(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, DrawMode eDrawMode, int32_t nScreenMode, uint8_t* pPalette);
	void(__fastcall* pfDrawImage)(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, DrawMode eDrawMode, uint8_t* pPalette);
	void(__fastcall* pfDrawShiftedImage)(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint32_t dwGamma, DrawMode eDrawMode, int32_t nGlobalPaletteShift);
	void(__fastcall* pfDrawVerticalCropImage)(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, int32_t nSkipLines, int32_t nDrawLines, DrawMode eDrawMode);
	void(__fastcall* pfDrawShadow)(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY);
	void(__fastcall* pfDrawImageFast)(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, uint8_t nPaletteIndex);
	void(__fastcall* pfDrawClippedImage)(D2GfxDataStrc* pData, int32_t nPosX, int32_t nPosY, void* pCropRect, DrawMode eDrawMode);
	int32_t(__fastcall* pfDrawWallTile)(D2TileLibraryEntryStrc* pTile, int32_t nPosX, int32_t nPosY, D2GfxLightStrc* pLight, int32_t nScreenPanels);
	int32_t(__fastcall* pfDrawTransWallTile)(D2TileLibraryEntryStrc* pTile, int32_t nPosX, int32_t nPosY, D2GfxLightStrc* pLight, int32_t nScreenPanels, uint8_t nAlpha);
	int32_t(__fastcall* pfDrawShadowTile)(D2TileLibraryEntryStrc* pTile, int32_t nPosX, int32_t nPosY, DrawMode eDrawMode, int32_t nScreenPanels);
	void(__fastcall* pfDrawRect)(RECT* pRect, uint8_t nPaletteIndex);
	void(__fastcall* pfDrawRectEx)(RECT* pRect, uint8_t nPaletteIndex);
	void(__fastcall* pfDrawSolidRect)(RECT* pRect, uint8_t nPaletteIndex);
	void(__fastcall* pfDrawSolidSquare)(POINT* pPoint, uint8_t nSize, uint8_t nPaletteIndex);
	void(__fastcall* pfDrawSolidRectEx)(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, DrawMode eDrawMode);
	void(__fastcall* pfDrawSolidRectAlpha)(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, uint8_t nAlpha);
	void(__fastcall* pfDrawLine)(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, uint8_t nAlpha);
	void(__fastcall* pfClearScreen)(int32_t bPartial);
	void(__fastcall* pfDrawString)(int32_t nPosX, int32_t nPosY, const char* szFormat, va_list va);
	void(__fastcall* pfDrawLight)(uint32_t* pLight, uint32_t* pPlayerLight, int32_t nPosX, int32_t nPosY);
	void(__fastcall* pfDebugFillBackBuffer)(int32_t nPosX, int32_t nPosY);
	void(__fastcall* pfClearCaches)();
};
#pragma pack(pop)


extern D2RenderCallbackStrc* gpRenderCallbacks;
extern D2GfxSettingsStrc gGfxSettings;


//D2Gfx.0x6FA73750 (#10000)
int32_t __stdcall D2GFX_Initialize(HINSTANCE hInstance, WNDPROC pfWndProc, int32_t nDisplayType, int32_t bWindowed);
//D2Gfx.0x6FA73980
void __cdecl j_D2GFX_10040_ShowAllWindows();
//D2Gfx.0x6FA73990 (#10002)
int32_t __stdcall D2GFX_ReturnFalse();
//D2Gfx.0x6FA739A0 (#10001)
int32_t __stdcall D2GFX_Release();
//D2Gfx.0x6FA73AC0 (#10003)
int32_t __stdcall D2GFX_GetRenderMode();
//D2Gfx.0x6FA73AD0 (#10004)
int32_t __stdcall D2GFX_CheckWindowed();
//D2Gfx.0x6FA73AE0 (#10005)
int32_t __stdcall D2GFX_GetResolutionMode();
//D2Gfx.0x6FA73AF0 (#10007)
int32_t __stdcall D2GFX_CheckUnitPerspective();
//D2Gfx.0x6FA73B00 (#10008)
void __stdcall D2GFX_SetUnitPerspective(int32_t bPerspective);
//D2Gfx.0x6FA73B20 (#10047)
int32_t __stdcall D2GFX_HardwareAcceleratedRenderMode();
//D2Gfx.0x6FA73B30 (#10009)
int32_t __stdcall D2GFX_TogglePerspective();
//D2Gfx.0x6FA73B50 (#10010)
int32_t __stdcall D2GFX_CheckPerspective();
//D2Gfx.0x6FA73B70 (#10011)
void __stdcall D2GFX_SetPerspective(int32_t bPerspective);
//D2Gfx.0x6FA73B80 (#10012)
int32_t __stdcall D2GFX_ToggleBlendedShadows();
//D2Gfx.0x6FA73BA0 (#10013)
int32_t __stdcall D2GFX_CheckBlendedShadows();
//D2Gfx.0x6FA73BB0 (#10014)
void __stdcall D2GFX_SetBlendShadows(int32_t bBlendedShadows);
//D2Gfx.0x6FA73BC0 (#10015)
int32_t __stdcall D2GFX_ToggleLowQuality();
//D2Gfx.0x6FA73BE0 (#10016)
int32_t __stdcall D2GFX_CheckLowQuality();
//D2Gfx.0x6FA73BF0 (#10017)
void __stdcall D2GFX_SetLowQuality(int32_t bLowQuality);
//D2Gfx.0x6FA73C00 (#10018)
int32_t __stdcall D2GFX_SetGamma(uint32_t dwGamma);
//D2Gfx.0x6FA73C20 (#10019)
int32_t __stdcall D2GFX_CheckGamma();
//D2Gfx.0x6FA73C30 (#10020)
void __stdcall D2GFX_EnableVSync();
//D2Gfx.0x6FA73C40 (#10021)
uint32_t* __stdcall D2GFX_GetRenderStatistics();
//D2Gfx.0x6FA73C50 (#10022)
void __fastcall D2GFX_SetViewMatrixAdjust(int32_t nAdjust);
//D2Gfx.0x6FA73C60 (#10042)
int32_t __fastcall D2GFX_SetOption(int32_t nOption, int32_t nValue);
//D2Gfx.0x6FA73C90 (#10044)
int32_t __stdcall D2GFX_BeginScene(int32_t bClear, uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
//D2Gfx.0x6FA73CB0 (#10045)
int32_t __stdcall D2GFX_EndScene();
//D2Gfx.0x6FA73CD0 (#10046)
int32_t __stdcall D2GFX_GetBackBuffer(uint8_t* pBuffer);
//D2Gfx.0x6FA73CE0 (#10051)
void __stdcall D2GFX_DrawRect(RECT* pRect, uint8_t nPaletteIndex);
//D2Gfx.0x6FA73D00 (#10052)
void __stdcall D2GFX_DrawRectEx(RECT* pRect, uint8_t nPaletteIndex);
//D2Gfx.0x6FA73D20 (#10053)
void __stdcall D2GFX_DrawSolidRect(RECT* pRect, uint8_t nPaletteIndex);
//D2Gfx.0x6FA73D40 (#10054)
void __stdcall D2GFX_DrawSolidSquare(POINT* pPoint, uint8_t nSize, uint8_t nPaletteIndex);
//D2Gfx.0x6FA73D60 (#10055)
void __stdcall D2GFX_DrawSolidRectEx(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, DrawMode eDrawMode);
//D2Gfx.0x6FA73DA0 (#10056)
void __stdcall D2GFX_DrawSolidRectAlpha(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint32_t dwColor, uint8_t nAlpha);
//D2Gfx.0x6FA73DE0 (#10057)
void __stdcall D2GFX_DrawLine(int32_t nXStart, int32_t nYStart, int32_t nXEnd, int32_t nYEnd, uint8_t nColor, uint8_t nAlpha);
//D2Gfx.0x6FA73E10 (#10058)
void __stdcall D2GFX_ClearScreen(int32_t bPartial);
//D2Gfx.0x6FA73E30 (#10079)
int32_t __stdcall D2GFX_DrawGroundTile(D2TileLibraryEntryStrc* pTile, D2GfxLightExStrc* pLight, int32_t nXPos, int32_t nYPos, int32_t nWorldXpos, int32_t nWorldYpos, uint8_t nAlpha, int32_t nScreenPanels, void* pTileData);
//D2Gfx.0x6FA73E70 (#10048)
void __stdcall D2GFX_SetPalette(PALETTEENTRY* pPalette);
//D2Gfx.0x6FA73EB0 (#10049)
void __stdcall D2GFX_SetPaletteTable(PALETTEENTRY** pPaletteTable);
//D2Gfx.0x6FA73ED0 (#10050)
void __stdcall D2GFX_SetGlobalLight(uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
//D2Gfx.0x6FA73EF0 (#10069)
void __cdecl D2GFX_DrawString(int32_t nXPos, int32_t nYPos, const char* szFormat, ...);
//D2Gfx.0x6FA73F10 (#10070)
void __stdcall D2GFX_DrawLight(uint32_t* pLight, uint32_t* pPlayerLight, int32_t nXPos, int32_t nYPos);
