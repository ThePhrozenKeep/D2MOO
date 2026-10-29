#include "D2WinMain.h"

#include <algorithm>
#include <cstdio>

#include <D2CMP.h>
#include <D2Gfx.h>
#include <D2Sound.h>
#include "JpegLibraryWrapper.h"
#include <Storm.h>

#include <Fog.h>
#include <D2BitManip.h>

#include <Texture.h>
#include <Window.h>

#include "D2WinAccountList.h"
#include "D2WinAnimImage.h"
#include "D2WinButton.h"
#include "D2WinComp.h"
#include "D2WinEditBox.h"
#include "D2WinFont.h"
#include "D2WinImage.h"
#include "D2WinList.h"
#include "D2WinPalette.h"
#include "D2WinPopUp.h"
#include "D2WinProgressBar.h"
#include "D2WinScrollBar.h"
#include "D2WinSmack.h"
#include "D2WinTextBox.h"
#include "D2WinTimer.h"
#include "D2WRegister.h"

#include "D2WinControlHeader.h"


#pragma warning (disable : 28159)
#pragma warning (disable : 6262)


#pragma pack(push, 1)
struct D2WinControlInitStrc
{
	int nType;
	int nX;
	int nY;
	int nWidth;
	int nHeight;
	int field_14;
	int nStringId;
	D2CellFileStrc** ppCellFile;
	int(__stdcall* pfCallback)(SMSGHANDLER_PARAMS*);
	void* field_24;
	void* field_28;
	void* field_2C;
};
#pragma pack(pop)


//1.10f: D2Win.0x6F8FE268
//1.14d: 0x007D55D8
WNDPROC gpfWndProc_6F8FE268;
//1.10f: D2Win.0x6F8FE23C
//1.14d: 0x007D55AC
HWND ghMainWindow_6F8FE23C;
//1.10f: D2Win.0x6F8FE26C
//1.14d: 0x007D55DC
int32_t dword_6F8FE26C;
int32_t(*gpfOnQuit)();
//1.10f: D2Win.0x6F8FE248
//1.14d: 0x007D55B8
int32_t gnResolution_6F8FE248;
//1.10f: D2Win.0x6F8FE240
//1.14d: 0x007D55B0
D2CellFileStrc* gpBackgroundCellFile;
//1.10f: D2Win.0x6F8FE224
//1.14d: 0x007D5594
D2CellFileStrc* gpCursorCellFile;
//1.10f: D2Win.0x6F8BDF78
//1.14d: 0x0072DDD4
int32_t gbRunMessageLoop;
//1.10f: D2Win.0x6F8FE24C
//1.14d: 0x007D55BC
D2WinControlStrc* gpControlList_6F8FE24C;
//1.10f: D2Win.0x6F8FE280
//1.14d: 0x007D55F4
int32_t dword_6F8FE280;
//1.10f: D2Win.0x6F8FE274
//1.14d: 0x007D55E4
int32_t dword_6F8FE274;
//1.10f: D2Win.0x6F8BDE18
//1.14d: 0x0072DC3C
int32_t dword_6F8BDE18;
//1.10f: D2Win.0x6F8FE260
//1.14d: 0x007D55D0
void* gpEscapeButton;
//1.10f: D2Win.0x6F8FE270
//1.14d: 0x007D55E0
int32_t gbIsMouseButtonPressed;
//1.10f: D2Win.0x6F8FE288
//1.14d: 0x007D55FC
int32_t dword_6F8FE288;
//1.10f: D2Win.0x6F8FE284
//1.14d: 0x007D55F8
void* dword_6F8FE284;
//1.10f: D2Win.0x6F8FE264
//1.14d: 0x007D55D4
D2WinControlStrc* dword_6F8FE264;
//1.10f: D2Win.0x6F8BDE20
//1.14d: 0x0072DC44
int32_t dword_6F8BDE20;
//1.10f: D2Win.0x6F8FE278
//1.14d: 0x007D55E8
int32_t dword_6F8FE278;
//1.10f: D2Win.0x6F8FE28C
//1.14d: 0x007D5600
DWORD gdwScreenshotTickCount_6F8FE28C;
//1.10f: D2Win.0x6F8BDE1C
//1.14d: 0x0072DC40
int32_t gnScreenshotCounter_6F8BDE1C;
//1.10f: D2Win.0x6F8FE258
//1.14d: 0x007D55C8
D2WinEditBoxStrc* gpNextFocusedEditBox;
//1.10f: D2Win.0x6F8FE25C
//1.14d: 0x007D55CC
D2WinEditBoxStrc* gpFocusedEditBox;
//1.10f: D2Win.0x6F8FE230
//1.14d: 0x007D55A0
int32_t gnCursorFrame;
//1.10f: D2Win.0x6F8FE234
//1.14d: 0x007D55A4
POINT gMousePosition_6F8FE234;
//1.10f: D2Win.0x6F8FE244
//1.14d: 0x007D55B4
int32_t gbWindowed_6F8FE244;
//1.10f: D2Win.0x6F8FE250
//1.14d: 0x007D55C0
D2WinControlStrc* dword_6F8FE250;
//Name in original .dll: sgtWindowData.deadChildrenLocked
//1.10f: D2Win.0x6F8FE254
//1.14d: 0x007D55C4
int32_t dword_6F8FE254;
//1.10f: D2Win.0x6F8FE228
//1.14d: 0x007D5598
int32_t dword_6F8FE228;
int32_t dword_6F8FE22C;



D2WinCommandRegisterStrc gpCommandRegister_6F8BDE28[28] =
{
	{ 0, WM_MOUSEMOVE, D2Win_COMMANDS_MouseMove_6F8AD670 },
	{ 0, WM_LBUTTONDOWN, D2Win_COMMANDS_MouseButton_6F8AE070 },
	{ 0, WM_LBUTTONUP, D2Win_COMMANDS_MouseButton_6F8AE070 },
	{ 0, WM_RBUTTONDOWN, D2Win_COMMANDS_MouseButton_6F8AE070 },
	{ 0, WM_RBUTTONUP, D2Win_COMMANDS_MouseButton_6F8AE070 },
	{ 0, WM_CHAR, D2Win_COMMANDS_Char_6F8ADA80 },
	{ 3, VK_BACK, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_LEFT, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_RIGHT, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_HOME, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_END, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_DELETE, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_UP, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_DOWN, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_PRIOR, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_NEXT, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_RETURN, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_ESCAPE, D2Win_COMMANDS_Escape_6F8ADB10 },
	{ 3, VK_TAB, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_SPACE, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, VK_CONTROL, D2Win_COMMANDS_Control_6F8ADBE0 },
	{ 3, VK_F1, D2Win_COMMANDS_VirtualKey_6F8ADB50 },
	{ 3, 0x4D, D2Win_COMMANDS_ToggleMusic_6F8AE020 },
	{ 0, WM_ACTIVATEAPP, D2Win_COMMANDS_ActivateApp_6F8ADC40 },
	{ 0, WM_SYSCOMMAND, D2Win_COMMANDS_SysCommand_6F8ADC00 },
	{ 2, VK_SNAPSHOT, D2Win_COMMANDS_Snapshot_6F8ADE70 },
	{ 2, VK_CONTROL, D2Win_COMMANDS_Control_6F8ADBF0 },
	{ 0, WM_MOUSEWHEEL, D2Win_COMMANDS_MouseWheel_6F8AE220 },
};


//D2Win.0x6F8ACC60 (#10000)
//1.14d: 0x004FA520
BOOL __stdcall D2Win_CreateWindow(HINSTANCE hInstance, DisplayType nRenderMode, BOOL bWindowed, BOOL bCompress)
{
	D2CMP_SetCompressedDataMode(bCompress);
	return D2GFX_Initialize(hInstance, D2Win_MAINWINDOW_WndProc_6F8AD9B0, nRenderMode, bWindowed);
}

//D2Win.0x6F8ACC90 (#10001)
//1.14d: 0x004F8F10
BOOL __stdcall D2Win_InitializeSpriteCache(BOOL bWindowed, D2GameResolutionMode nResolution)
{
	dword_6F8FE26C = 1;

	if (!WINDOW_Create(bWindowed, nResolution))
	{
		return 0;
	}

	D2CMP_InitSpriteCache(0, 0x800000u, 0x7D000u, 0);
	gpBackgroundCellFile = 0;
	gpControlList_6F8FE24C = 0;
	dword_6F8FE250 = 0;
	dword_6F8FE254 = 0;
	gpFocusedEditBox = 0;
	gpEscapeButton = 0;
	gpNextFocusedEditBox = 0;
	gpfWndProc_6F8FE268 = 0;
	dword_6F8FE264 = 0;
	dword_6F8FE278 = 0;
	gnCursorFrame = 0;
	gpfOnQuit = 0;
	gnResolution_6F8FE248 = nResolution;
	ghMainWindow_6F8FE23C = WINDOW_GetWindow();
	gbWindowed_6F8FE244 = bWindowed;

	return 1;
}

//D2Win.0x6F8ACD40 (#10014)
//1.14d: 0x004F8FC0
int __stdcall D2Win_10014_SetWndProc(WNDPROC wndProc)
{
	gpfWndProc_6F8FE268 = wndProc;
	return 1;
}

//D2Win.0x6F8ACD60 (#10015)
//1.14d: 0x004F8FE0
int __fastcall D2Win_10015()
{
	D2Win_10115_FONT();
	D2Win_10127_SetFont(D2FONT_FONT16);
	LIST_LoadPentspinCellfile();
	D2Win_10157();
	gpfWndProc_6F8FE268 = nullptr;
	WREGISTER_RegisterCommands(WINDOW_GetWindow(), gpCommandRegister_6F8BDE28, std::size(gpCommandRegister_6F8BDE28));
	D2Win_10177(0);
	gpfOnQuit = nullptr;
	return 1;
}

//D2Win.0x6F8ACDB0 (#10002)
//1.14d: 0x004F9030
int __stdcall D2Win_CloseSpriteCache()
{
	dword_6F8FE26C = 0;
	ghMainWindow_6F8FE23C = 0;
	D2CMP_FlushSpriteCache(1);
	WINDOW_Destroy();
	return 1;
}

//D2Win.0x6F8ACDD0 (#10003)
//1.14d: 0x004F9050
void __stdcall D2Win_10003()
{
	gnResolution_6F8FE248 = 0;
	WINDOW_PlayCutscene();
}

//D2Win.0x6F8ACDE0 (#10004)
//1.14d: 0x004F9060
void __stdcall D2Win_10004(D2GameResolutionMode bForceResize)
{
	gnResolution_6F8FE248 = bForceResize;

	if (bForceResize == 1)
	{
		D2CMP_FlushSpriteCache(1);
	}

	WINDOW_EndCutScene(bForceResize);

	if (bForceResize == 1)
	{
		D2CMP_InitSpriteCache(0, 0x800000u, 0x7D000u, 0);
	}
}

//D2Win: inlined (D2Win_10016 0x6F8ACE20, D2Win_UpdateAndDraw 0x6F8AD6F0)
//1.14d: 0x004F9290
void __stdcall CONTROL_FreeDeadList()
{
	D2WinControlStrc* pNext = nullptr;

	D2WinControlStrc* pCurrent = dword_6F8FE250;
	while (pCurrent)
	{
		if (pCurrent->dwFlags & gdwBitMasks[0])
		{
			break;
		}

		pNext = pCurrent->pNext;
		D2_FREE(pCurrent);
		pCurrent = pNext;
		dword_6F8FE250 = pNext;
	}

	while (pCurrent)
	{
		if (pCurrent->dwFlags & gdwBitMasks[1])
		{
			break;
		}

		pNext = pCurrent->pNext;
		D2_FREE(pCurrent);
		pCurrent = pNext;
		dword_6F8FE250 = pNext;
	}
}

//D2Win.0x6F8ACE20 (#10016)
//1.14d: 0x004FA550
int __stdcall D2Win_10016()
{
	dword_6F8FE254 = 0;

	CONTROL_FreeDeadList();

	LIST_UnloadPentspinCellfile();
	D2Win_10116();
	WREGISTER_UnregisterCommands(WINDOW_GetWindow(), gpCommandRegister_6F8BDE28, std::size(gpCommandRegister_6F8BDE28));
	return 1;
}

//D2Win.0x6F8ACEC0 (#10005)
//1.14d: 0x004F90A0
int __stdcall D2Win_10005(D2GameResolutionMode nResolution)
{
	gnResolution_6F8FE248 = nResolution;

	D2CMP_FlushSpriteCache(1);

	if (!WINDOW_Resize(nResolution, 0))
	{
		FOG_Trace("Failed to resize window... this is fatal!");
		exit(-1);
	}

	D2Win_10176(0);
	D2CMP_InitSpriteCache(0, 0x800000u, 0x7D000u, 0);
	return 1;
}

//D2Win.0x6F8ACF20 (#10006)
//1.14d: 0x004F9130
void __stdcall D2Win_10006_ClearDrawCaches()
{
	WINDOW_ClearCaches();
}

//D2Win.0x6F8ACF30 (#10007)
//1.14d: 0x004F9140
int __stdcall BACKGROUND_SetCellFile(D2CellFileStrc* pCellFile)
{
	gpBackgroundCellFile = pCellFile;
	return 1;
}

//D2Win.0x6F8ACF50 (#10011)
//1.14d: 0x004F9160
int __stdcall CURSOR_SetCellFile(D2CellFileStrc* pCellFile)
{
	gpCursorCellFile = pCellFile;
	return 1;
}

//D2Win.0x6F8ACF70 (#10010)
//1.14d: 0x004F9180
void __stdcall D2Win_SetQuitCallback(int(*pfOnQuit)())
{
	gpfOnQuit = pfOnQuit;
}

//D2Win.0x6F8ACF80 (#10009)
//1.14d: 0x004F9190
int __stdcall D2Win_StopMessageLoop()
{
	gbRunMessageLoop = 0;
	return 1;
}

//D2Win.0x6F8ACF90 (#10008)
//1.14d: 0x004FA590
int __stdcall D2Win_RunMessageLoop(void(__stdcall* pfOnFrame)(DWORD))
{
	int nSleepMillis = 0;
	DWORD dwFrame = 0;
	
	DWORD dwTickCount = GetTickCount();

	while (gbRunMessageLoop)
	{
		struct tagMSG Msg = {};

		if (PeekMessageA(&Msg, 0, 0, 0, 0))
		{
			gbRunMessageLoop = GetMessageA(&Msg, 0, 0, 0);
			if (!gbRunMessageLoop && gpfOnQuit)
			{
				gpfOnQuit();
			}

			TranslateMessage(&Msg);
			DispatchMessageA(&Msg);
		}
		else
		{
			DWORD dwTickDiff = GetTickCount() - dwTickCount;

			dwTickCount = GetTickCount();
			if (dwTickDiff > 1000)
			{
				dwTickDiff = 1000;
			}

			nSleepMillis -= dwTickDiff;
			if (nSleepMillis <= 0)
			{
				nSleepMillis += 40;
				if (nSleepMillis < -1000)
				{
					nSleepMillis = 0;
				}

				if (pfOnFrame)
				{
					pfOnFrame(dwFrame);
					++dwFrame;
				}

				D2Win_UpdateAndDraw();
			}

			gpFocusedEditBox = gpNextFocusedEditBox;

			DWORD dwSleepMillis = std::min(nSleepMillis, 20);

			if (dword_6F8BDE20)
			{
				dwSleepMillis = 0;
			}

			Sleep(dwSleepMillis);
		}
	}

	D2Win_UpdateAndDraw();

	if (!dword_6F8FE280)
	{
		gbRunMessageLoop = 1;
	}

	return 0;
}

//D2Win.0x6F8AD0B0 (#10013)
//1.14d: 0x004F91A0
int __stdcall CONTROL_SetCellFile(D2WinControlStrc* pControl, D2CellFileStrc* pCellFile)
{
	pControl->pCellFile = pCellFile;
	return 1;
}

//D2Win.0x6F8AD0D0 (#10012)
//1.14d: 0x004F91C0
int __stdcall CONTROL_SetFocusedEditBox(D2WinEditBoxStrc* pEditBox)
{
	if (gpNextFocusedEditBox)
	{
		if (pEditBox != gpNextFocusedEditBox)
		{
			EDITBOX_ClearSelection(gpNextFocusedEditBox);
		}
	}

	gpNextFocusedEditBox = pEditBox;


	return 1;
}

//D2Win.0x6F8AD110 (#10023)
//1.14d: 0x004F9200
D2WinEditBoxStrc* __stdcall CONTROL_GetFocusedEditBox()
{
	return gpFocusedEditBox;
}

//D2Win.0x6F8AD120
//1.14d: 0x004F9210
int __stdcall CONTROL_AppendToList(D2WinControlStrc* pControl)
{
	if (gpControlList_6F8FE24C)
	{
		D2WinControlStrc* pLast = gpControlList_6F8FE24C;
		for (D2WinControlStrc* i = gpControlList_6F8FE24C->pNext; i; i = i->pNext)
		{
			pLast = i;
		}
		pLast->pNext = pControl;
	}
	else
	{
		gpControlList_6F8FE24C = pControl;
	}

	return 1;
}

//D2Win.0x6F8AD160
//1.14d: 0x004F9310
int __stdcall CONTROL_RemoveFromList(D2WinControlStrc* pControl)
{
	if (gpFocusedEditBox == (D2WinEditBoxStrc*)pControl)
	{
		gpFocusedEditBox = nullptr;
	}

	if (gpNextFocusedEditBox == (D2WinEditBoxStrc*)pControl)
	{
		gpNextFocusedEditBox = nullptr;
	}

	if (dword_6F8FE264 == pControl)
	{
		dword_6F8FE264 = nullptr;
		dword_6F8FE278 = 0;
	}

	if (gpEscapeButton == pControl)
	{
		gpEscapeButton = nullptr;
	}

	if (!gpControlList_6F8FE24C)
	{
		return 0;
	}

	D2WinControlStrc* pPrevious = nullptr;
	for (D2WinControlStrc* pCurrent = gpControlList_6F8FE24C; pCurrent; pCurrent = pCurrent->pNext)
	{
		if (pCurrent == pControl)
		{
			if (pPrevious)
			{
				pPrevious->pNext = pCurrent->pNext;
			}
			else
			{
				gpControlList_6F8FE24C = pControl->pNext;
			}
			pControl->pNext = nullptr;

			if (dword_6F8FE250)
			{
				D2WinControlStrc* pLast = dword_6F8FE250;
				for (D2WinControlStrc* i = dword_6F8FE250->pNext; i; i = i->pNext)
				{
					pLast = i;
				}

				pLast->pNext = pControl;
			}
			else
			{
				dword_6F8FE250 = pControl;
			}

			return 1;
		}

		pPrevious = pCurrent;
	}

	return 1;
}

//D2Win.0x6F8AD260 (#10017)
//1.14d: 0x004F93C0
D2WinControlStrc* __stdcall CONTROL_Create(D2WinControlInitStrc* pControlInit)
{
	D2CellFileStrc* pCellFile = nullptr;
	if (pControlInit->ppCellFile)
	{
		pCellFile = *pControlInit->ppCellFile;
	}

	switch (pControlInit->nType)
	{
	case D2WIN_TEXTBOX:
		return (D2WinControlStrc*)D2Win_10042_TEXTBOX_Create(
			pControlInit->nX,
			pControlInit->nY,
			pControlInit->nWidth,
			pControlInit->nHeight,
			pControlInit->field_14,
			pControlInit->nStringId,
			pCellFile,
			pControlInit->pfCallback,
			(int)pControlInit->field_2C,
			(int)pControlInit->field_28,
			(int)pControlInit->field_24);

	case D2WIN_EDITBOX:
		return (D2WinControlStrc*)D2Win_10070_EDITBOX_Create(
			pControlInit->nX,
			pControlInit->nY,
			pControlInit->nWidth,
			pControlInit->nHeight,
			pControlInit->field_14,
			pControlInit->nStringId,
			pCellFile,
			(int)pControlInit->field_24,
			pControlInit->pfCallback,
			(int)pControlInit->field_2C,
			(int)pControlInit->field_28);

	case D2WIN_IMAGE:
		return (D2WinControlStrc*)IMAGE2_Create(
			pControlInit->nX,
			pControlInit->nY,
			pControlInit->nWidth,
			pControlInit->nHeight,
			pCellFile,
			(int(__stdcall*)(SMSGHANDLER_PARAMS*))pControlInit->pfCallback,
			(D2WinImageClickRectStrc*)pControlInit->field_24,
			(int(__stdcall*)(SMSGHANDLER_PARAMS*))pControlInit->field_28);

	case D2WIN_ANIMIMAGE:
		return (D2WinControlStrc*)ANIMIMAGE_Create(
			pControlInit->nX,
			pControlInit->nY,
			pControlInit->nWidth,
			pControlInit->nHeight,
			pCellFile,
			pControlInit->field_14,
			pControlInit->pfCallback,
			(D2AnimatedImageDescriptor*)pControlInit->field_24,
			(DrawMode)pControlInit->nStringId,
			(void(__stdcall*)(SMSGHANDLER_PARAMS*))pControlInit->field_28);

	case D2WIN_IMAGE2:
		return (D2WinControlStrc*)IMAGE_Create(pControlInit->nX, pControlInit->nY, pControlInit->nWidth, pControlInit->nHeight, pCellFile, pControlInit->field_14, pControlInit->nStringId);

	case D2WIN_SCROLLBAR:
		return (D2WinControlStrc*)SCROLLBAR_Create(pControlInit->nX, pControlInit->nY, pControlInit->nWidth, pControlInit->nHeight, pCellFile, pControlInit->pfCallback);

	case D2WIN_BUTTON:
		return (D2WinControlStrc*)BUTTON_Create(
			pControlInit->nX,
			pControlInit->nY,
			pControlInit->nWidth,
			pControlInit->nHeight,
			pCellFile,
			pControlInit->pfCallback,
			pControlInit->field_14,
			(int)pControlInit->field_24,
			(int)pControlInit->field_2C,
			pControlInit->nStringId,
			(int(__stdcall*)(SMSGHANDLER_PARAMS*))pControlInit->field_28);

	case D2WIN_LIST:
		return (D2WinControlStrc*)LIST_Create(pControlInit->nX, pControlInit->nY, pControlInit->nWidth, pControlInit->nHeight, pControlInit->field_24, (Font*)pControlInit->field_28);

	case D2WIN_TIMER:
		return (D2WinControlStrc*)TIMER_Create(pControlInit->nX, pControlInit->pfCallback);

	case D2WIN_SMACK:
		return (D2WinControlStrc*)SMACK_Create(pControlInit->nX, pControlInit->nY, pControlInit->nWidth, pControlInit->nHeight, pControlInit->pfCallback);

	case D2WIN_PROGRESSBAR:
		return (D2WinControlStrc*)PROGRESSBAR_Create(pControlInit->nX, pControlInit->nY, pControlInit->nWidth, pControlInit->nHeight);

	case D2WIN_ACCOUNTLIST:
		return (D2WinControlStrc*)ACCOUNTLIST_Create(pControlInit->nX, pControlInit->nY, pControlInit->nWidth, pControlInit->nHeight, (int)pControlInit->field_2C, pControlInit->field_28);

	default:
		break;
	}

	return nullptr;
}

//D2Win.0x6F8AD450 (#10018)
//1.14d: 0x004F95C0
int __stdcall CONTROL_Destroy(void* ppControl)
{
	//TODO: result
	int result = 0;

	if (ppControl)
	{
		D2WinControlStrc* v2 = *(D2WinControlStrc**)ppControl;
		if (*(DWORD*)ppControl)
		{
			switch (v2->nType)
			{
			case D2WIN_IMAGE:
			case D2WIN_IMAGE2:
				result = IMAGE_Destroy(v2);
				*(DWORD*)ppControl = 0;
				break;

			case D2WIN_ANIMIMAGE:
				result = ANIMIMAGE_Destroy(v2);
				*(DWORD*)ppControl = 0;
				break;

			case D2WIN_SCROLLBAR:
				result = SCROLLBAR_Destroy(v2);
				*(DWORD*)ppControl = 0;
				break;

			case D2WIN_TIMER:
				result = TIMER_Destroy(v2);
				*(DWORD*)ppControl = 0;
				break;

			case D2WIN_PROGRESSBAR:
				result = PROGRESSBAR_Destroy(v2);
				*(DWORD*)ppControl = 0;
				break;

			case D2WIN_TEXTBOX:
				result = D2Win_10043_TEXTBOX_Destroy((D2WinTextBoxStrc*)v2);
				*(DWORD*)ppControl = 0;
				break;

			case D2WIN_EDITBOX:
				result = D2Win_10071_EDITBOX_Destroy((D2WinEditBoxStrc*)v2);
				*(DWORD*)ppControl = 0;
				break;

			case D2WIN_BUTTON:
				result = BUTTON_Destroy((D2WinButtonStrc*)v2);
				*(DWORD*)ppControl = 0;
				break;

			case D2WIN_LIST:
				result = LIST_Destroy((D2WinListStrc*)v2);
				*(DWORD*)ppControl = 0;
				break;

			case D2WIN_SMACK:
				result = SMACK_Destroy((D2WinSmackStrc*)v2);
				*(DWORD*)ppControl = 0;
				break;

			case D2WIN_ACCOUNTLIST:
				result = ACCOUNTLIST_Destroy((D2WinAccountListStrc*)v2);
				*(DWORD*)ppControl = 0;
				break;

			default:
				result = 0;
				*(DWORD*)ppControl = 0;
				break;
			}
		}
	}

	return 0;
}

//D2Win.0x6F8AD570 (#10025)
//1.14d: 0x004F96F0
void __fastcall CONTROL_SetEnabled(D2WinControlStrc* pControl, int bEnabled)
{
	if (!pControl)
	{
		return;
	}

	if (bEnabled)
	{
		pControl->dwFlags |= gdwBitMasks[0];
	}
	else
	{
		pControl->dwFlags &= gdwInvBitMasks[0];
	}
}

//D2Win.0x6F8AD5A0 (#10024)
//1.14d: 0x004F9710
int __fastcall CONTROL_IsEnabled(D2WinControlStrc* pControl)
{
	D2_ASSERT(pControl);

	return pControl->dwFlags & gdwBitMasks[0];
}

//D2Win.0x6F8AD5D0 (#10027)
//1.14d: 0x004F9740
void __fastcall CONTROL_SetVisible(D2WinControlStrc* pControl, int bVisible)
{
	D2_ASSERT(pControl);

	if (bVisible)
	{
		pControl->dwFlags |= gdwBitMasks[2];
	}
	else
	{
		pControl->dwFlags &= gdwInvBitMasks[2];
	}
}

//D2Win.0x6F8AD620 (#10026)
//1.14d: 0x004F9780
int __fastcall CONTROL_IsVisible(D2WinControlStrc* pControl)
{
	D2_ASSERT(pControl);

	return pControl->dwFlags & gdwBitMasks[2];
}

//D2Win.0x6F8AD650 (#10170)
//1.14d: 0x004F97B0
BOOL __stdcall CONTROL_MouseInsideRect(const RECT* pRect)
{
	return PtInRect(pRect, gMousePosition_6F8FE234);
}

//D2Win.0x6F8AD670
//1.14d: 0x004FA6B0
void __stdcall D2Win_COMMANDS_MouseMove_6F8AD670(SMSGHANDLER_PARAMS* pMsg)
{
	int nX = (int)(int16_t)LOWORD(pMsg->lParam); // GET_X_LPARAM
	int nY = (int)(int16_t)HIWORD(pMsg->lParam); // GET_Y_LPARAM

	int nScreenWidth = 0;
	int nScreenHeight = 0;
	WINDOW_GetDimensions(&nScreenWidth, &nScreenHeight);
	--nScreenWidth;
	--nScreenHeight;

	if (D2GFX_HardwareAcceleratedRenderMode())
	{
		bool bSetCursorPos = false;
		if (nX > nScreenWidth)
		{
			nX = nScreenWidth;
			bSetCursorPos = true;
		}

		if (nY > nScreenHeight)
		{
			nY = nScreenHeight;
			bSetCursorPos = true;
		}

		if (bSetCursorPos)
		{
			SetCursorPos(nX, nY);
		}
	}

	gMousePosition_6F8FE234.x = nX;
	gMousePosition_6F8FE234.y = nY;
}

//D2Win.0x6F8AD6F0 (#10019)
//1.14d: 0x004F98E0
void __stdcall D2Win_UpdateAndDraw()
{
	if (WINDOW_GetState())
	{
		return;
	}

	for (D2WinControlStrc* pControl = gpControlList_6F8FE24C; pControl; pControl = pControl->pNext)
	{
		if (pControl->pfUpdate)
		{
			pControl->pfUpdate(pControl);
		}
	}

	if (gpfWndProc_6F8FE268)
	{
		return;
	}

	int nScreenWidth = 0;
	int nScreenHeight = 0;
	WINDOW_GetDimensions(&nScreenWidth, &nScreenHeight);

	if (D2GFX_StartDraw(0, 0, 0, 0))
	{
		D2GFX_ClearScreen(0);

		if (gpBackgroundCellFile)
		{
			D2GfxDataStrc gfxData = {};
			gfxData.pCellFile = gpBackgroundCellFile;
			gfxData.nDirection = 0;
			gfxData.nFrame = 0;

			for (int y = 256; y < 768; y += 256)
			{
				for (int x = 0; x < 1024; x += 256)
				{
					TEXTURE_CelDraw(&gfxData, x, y, -1, DRAWMODE_NORMAL, 0);
					++gfxData.nFrame;
				}
			}

			for (int x = 0; x < 1024; x += 256)
			{
				TEXTURE_CelDraw(&gfxData, x, nScreenHeight - 1, -1, DRAWMODE_NORMAL, 0);
				++gfxData.nFrame;
			}
		}

		for (D2WinControlStrc* pControl = gpControlList_6F8FE24C; pControl; pControl = pControl->pNext)
		{
			if (pControl->pfDraw && !(pControl->dwFlags & gdwBitMasks[3]))
			{
				pControl->pfDraw(pControl);
			}
		}

		for (void* i = D2Win_10147(); i; i = D2Win_10148(i))
		{
			sub_6F8A15E0(i);
		}

		for (void* i = D2Win_10147(); i; i = D2Win_10148(i))
		{
			sub_6F8A1890(i, -1);
		}

		for (void* i = D2Win_10147(); i; i = D2Win_10148(i))
		{
			sub_6F8A1D10(i);
			sub_6F8A2070(i);
		}

		if (dword_6F8FE278)
		{
			D2GFX_DrawBoxAlpha(0, 0, nScreenWidth, nScreenHeight, 0, 127u);
		}

		for (D2WinControlStrc* pControl = gpControlList_6F8FE24C; pControl; pControl = pControl->pNext)
		{
			if (pControl->pfDraw && pControl->dwFlags & gdwBitMasks[3])
			{
				pControl->pfDraw(pControl);
			}
		}

		if (gpCursorCellFile && dword_6F8BDE18)
		{
			int nX = gMousePosition_6F8FE234.x;
			int nY = gMousePosition_6F8FE234.y;
			dword_6F8FE228 = gMousePosition_6F8FE234.x;
			dword_6F8FE22C = gMousePosition_6F8FE234.y;

			if (gMousePosition_6F8FE234.x <= 0)
			{
				nX = 0;
			}

			if (gMousePosition_6F8FE234.y <= 0)
			{
				nY = 0;
			}

			D2GfxDataStrc gfxData = {};
			gfxData.nDirection = 0;
			gfxData.pCellFile = gpCursorCellFile;
			gfxData.nFrame = gnCursorFrame;
			TEXTURE_CelDraw(&gfxData, nX, nY - 1, -1, DRAWMODE_NORMAL, 0);
		}

		D2GFX_EndScene();
	}

	if (dword_6F8FE254 <= 0)
	{
		CONTROL_FreeDeadList();
	}
}

//D2Win.0x6F8AD9B0
//1.14d: 0x004F9A80
LRESULT __stdcall D2Win_MAINWINDOW_WndProc_6F8AD9B0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (!dword_6F8FE26C)
	{
		return 0;
	}

	if (gpfWndProc_6F8FE268)
	{
		return gpfWndProc_6F8FE268(hWnd, Msg, wParam, lParam);
	}

	if (Msg == WM_CLOSE)
	{
		dword_6F8FE280 = 1;
		PostQuitMessage(0);
		return 0;
	}

	int bUseResult = 0;
	LRESULT lResult = 0;

	if (SMsgDispatchMessage(hWnd, Msg, wParam, lParam, &bUseResult, &lResult) && bUseResult)
	{
		return lResult;
	}

	return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

//D2Win.0x6F8ADA80
//1.14d: 0x004F9B60
void __stdcall D2Win_COMMANDS_Char_6F8ADA80(SMSGHANDLER_PARAMS* pMsg)
{

	D2WinControlStrc* pControl = dword_6F8FE264;
	if (gpFocusedEditBox)
	{
		if (dword_6F8FE264)
		{
			for (D2WinControlStrc* i = dword_6F8FE264; i; i = i->pNext)
			{
				if ((D2WinEditBoxStrc*)i == gpFocusedEditBox)
				{
					if (D2Win_10074(gpFocusedEditBox, pMsg->wParam))
					{
						SMsgBreakHandlerChain(pMsg);
						// Should we return SMsgBreakHandlerChain's result?
						return;

					}

					break;
				}
			}
		}
		else
		{
			if (D2Win_10074(gpFocusedEditBox, pMsg->wParam))
			{
				SMsgBreakHandlerChain(pMsg);
				// Should we return SMsgBreakHandlerChain's result?
				return;
			}
		}
	}

	if (!pControl)
	{
		pControl = gpControlList_6F8FE24C;
	}

	while (pControl)
	{
		if (pControl->pfHandleCharInput)
		{
			pMsg->hWindow = (HWND)pControl;

			++dword_6F8FE254;
			const int bHandled = pControl->pfHandleCharInput(pMsg);
			--dword_6F8FE254;

			if (bHandled)
			{
				break;
			}
		}

		pControl = pControl->pNext;
	}

	SMsgBreakHandlerChain(pMsg);
}

//D2Win.0x6F8ADB10
//1.14d: 0x004FA760
void __stdcall D2Win_COMMANDS_Escape_6F8ADB10(SMSGHANDLER_PARAMS* pMsg)
{
	if (gpEscapeButton)
	{
		for (D2WinControlStrc* pControl = dword_6F8FE264; pControl; pControl = pControl->pNext)
		{
			if (pControl == gpEscapeButton)
			{
				pMsg->hWindow = (HWND)gpEscapeButton;
				BUTTON_SimulateClick(pMsg);
				SMsgBreakHandlerChain(pMsg);
				// Should we return SMsgBreakHandlerChain's result?
				return;
			}
		}
	}

	D2Win_COMMANDS_VirtualKey_6F8ADB50(pMsg);
	// Should we return D2Win_COMMANDS_VirtualKey_6F8ADB50's result?
	return;
}

//D2Win.0x6F8ADB50
//1.14d: 0x004F9BF0
void __stdcall D2Win_COMMANDS_VirtualKey_6F8ADB50(SMSGHANDLER_PARAMS* pMsg)
{
	//TODO: v4
	D2WinControlStrc* pControl = dword_6F8FE264;
	if (gpFocusedEditBox)
	{
		if (dword_6F8FE264)
		{
			for (D2WinControlStrc* i = dword_6F8FE264; i; i = i->pNext)
			{
				if ((D2WinEditBoxStrc*)i == gpFocusedEditBox)
				{
					if (EDITBOX_HandleKeyDown(gpFocusedEditBox, pMsg->wParam))
					{
						SMsgBreakHandlerChain(pMsg);
						// Should we return SMsgBreakHandlerChain's result?$
						return;
					}

					break;
				}
			}
		}
		else
		{
			if (EDITBOX_HandleKeyDown(gpFocusedEditBox, pMsg->wParam))
			{
				SMsgBreakHandlerChain(pMsg);
				// Should we return SMsgBreakHandlerChain's result?
				return;
			}
		}
	}

	if (!pControl)
	{
		pControl = gpControlList_6F8FE24C;
	}

	while (pControl)
	{
		if (pControl->pfHandleVirtualKeyInput)
		{
			pMsg->hWindow = (HWND)pControl;
			++dword_6F8FE254;
			const int v4 = pControl->pfHandleVirtualKeyInput(pMsg);
			--dword_6F8FE254;
			if (v4)
			{
				SMsgBreakHandlerChain(pMsg);
				// Should we return SMsgBreakHandlerChain's result?
				return;
			}
		}

		pControl = pControl->pNext;
	}

	SMsgBreakHandlerChain(pMsg);
	// Should we return SMsgBreakHandlerChain's result?
	return;
}

//D2Win.0x6F8ADBE0
//1.14d: 0x004F9C80
void __stdcall D2Win_COMMANDS_Control_6F8ADBE0(SMSGHANDLER_PARAMS* pMsg)
{
	dword_6F8FE274 = 1;
	SMsgBreakHandlerChain(pMsg);
	// Should we return SMsgBreakHandlerChain's result?
}

//D2Win.0x6F8ADBF0
//1.14d: 0x004F9CA0
void __stdcall D2Win_COMMANDS_Control_6F8ADBF0(SMSGHANDLER_PARAMS* pMsg)
{
	dword_6F8FE274 = 0;
	SMsgBreakHandlerChain(pMsg);
	// Should we return SMsgBreakHandlerChain's result?
}

//D2Win.0x6F8ADC00
//1.14d: 0x004F9CC0
void __stdcall D2Win_COMMANDS_SysCommand_6F8ADC00(SMSGHANDLER_PARAMS* pMsg)
{
	uint16_t v1 = pMsg->wParam & 0xFFF0;
	if (v1 == 0xF130 || v1 == 0xF100 || v1 == 0xF010 && WINDOW_IsFullScreen())
	{
		pMsg->bUseResult = 1;
		pMsg->lResult = 0;
	}
}

//D2Win.0x6F8ADC40
//1.14d: 0x004F9F20
void __stdcall D2Win_COMMANDS_ActivateApp_6F8ADC40(SMSGHANDLER_PARAMS* pMsg)
{
	dword_6F8BDE20 = pMsg->wParam != 0;
	if (pMsg->wParam)
	{
		if (!FOG_IsHandlingError())
		{
			WINDOW_SetPaused(0);
			D2Win_10176(1);
		}

		D2SOUND_10049(dword_6F8FE284);
		dword_6F8FE288 = 0;
	}
	else
	{
		if (!dword_6F8FE288)
		{
			dword_6F8FE284 = D2SOUND_10048();
		}

		dword_6F8FE288 = 1;
		D2SOUND_10049(0);
		WINDOW_SetPaused(1);
	}

	if (WINDOW_CheckCutScene())
	{
		WINDOW_GetState();
	}

	pMsg->bUseResult = 0;
}

//D2Win.0x6F8ADCD0 (#10168)
//1.14d: 0x004FA7A0
void __stdcall D2Win_10168_WINMAIN_CreateScreenshot()
{
	const uint32_t dwTickCount = GetTickCount();

	if (dwTickCount - gdwScreenshotTickCount_6F8FE28C < 1000)
	{
		return;
	}

	int32_t nWidth = 0;
	int32_t nHeight = 0;
	WINDOW_GetDimensions(&nWidth, &nHeight);

	uint8_t* pBuffer = (uint8_t*)D2_ALLOC(3 * nWidth * nHeight);

	if (D2GFX_GetBackBuffer(pBuffer))
	{
		char szFilename[256] = {};
		while (1)
		{
			sprintf_s(szFilename, "Screenshot%03d.jpg", gnScreenshotCounter_6F8BDE1C++);
			const HANDLE v3 = FOG_CreateFileA(szFilename, 0x80000000u, 0, 0, 3, 128, 0);
			if (v3 == INVALID_HANDLE_VALUE)
			{
				break;
			}
			FOG_CloseFile(v3);
		}

		D2MooJpegProperties tJpegProperties; //  ijl's JPEG_CORE_PROPERTIESin in original code
		D2MooJpegLibInit(&tJpegProperties);
		tJpegProperties.nWidth = nWidth;
		tJpegProperties.nHeight = nHeight;
		tJpegProperties.pBuffer = pBuffer;
		tJpegProperties.szFileName = szFilename;
		tJpegProperties.nJQuality = 100;
		D2MooJpegLibWrite(&tJpegProperties);
		D2MooJpegLibFree(&tJpegProperties);
	}

	gdwScreenshotTickCount_6F8FE28C = dwTickCount;
	D2_FREE(pBuffer);
}

//D2Win.0x6F8ADE70
//1.14d: 0x004FA840
void __stdcall D2Win_COMMANDS_Snapshot_6F8ADE70(SMSGHANDLER_PARAMS* pMsg)
{
	D2Win_10168_WINMAIN_CreateScreenshot();

	pMsg->bUseResult = 1;
	pMsg->lResult = 0;
}

//D2Win.0x6F8AE020
//1.14d: 0x004FA160
void __stdcall D2Win_COMMANDS_ToggleMusic_6F8AE020(SMSGHANDLER_PARAMS* pMsg)
{
	if (!dword_6F8FE274 || !D2SOUND_10066())
	{
		return;
	}

	if (D2SOUND_10069())
	{
		D2SOUND_10065();
		D2SOUND_10070(0);
	}
	else
	{
		D2SOUND_10034(1);
		D2SOUND_10070(1);
	}

	pMsg->bUseResult = 1;
	pMsg->lResult = 0;
}

//D2Win.0x6F8AE070
//1.14d: 0x004FA860
void __stdcall D2Win_COMMANDS_MouseButton_6F8AE070(SMSGHANDLER_PARAMS* pMsg)
{
	//TODO: v10
	D2Win_COMMANDS_MouseMove_6F8AD670(pMsg);

	if (POPUP_IsActive())
	{
		const int32_t bInputHandled = POPUP_HandleMouseButtonInput(pMsg);
		POPUP_Destroy();
		if (bInputHandled)
		{
			return;
		}
	}

	D2WinControlStrc* pControl = dword_6F8FE264;
	if (!dword_6F8FE264)
	{
		pControl = gpControlList_6F8FE24C;
	}

	switch (pMsg->nMessage)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		gbIsMouseButtonPressed = 1;

		while (pControl)
		{
			if ((pControl->pfShouldMouseInputBeHandled && pControl->pfShouldMouseInputBeHandled(pControl))
				|| (gMousePosition_6F8FE234.x >= pControl->nImageX && gMousePosition_6F8FE234.y >= pControl->nImageY - pControl->nHeight
					&& gMousePosition_6F8FE234.x < pControl->nImageX + pControl->nWidth && gMousePosition_6F8FE234.y < pControl->nImageY))
			{
				pMsg->hWindow = (HWND)pControl;
				if (pControl->pfHandleMouseDown)
				{
					++dword_6F8FE254;
					const int v10 = pControl->pfHandleMouseDown(pMsg);
					--dword_6F8FE254;
					if (v10)
					{
						SMsgBreakHandlerChain(pMsg);
						// Should we return SMsgBreakHandlerChain's result?
						return;
					}
				}
			}

			pControl = pControl->pNext;
		}

		SMsgBreakHandlerChain(pMsg);
		// Should we return SMsgBreakHandlerChain's result?
		return;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		gbIsMouseButtonPressed = 0;

		while (pControl)
		{
			pMsg->hWindow = (HWND)pControl;
			if (pControl->pfHandleMouseUp)
			{
				++dword_6F8FE254;
				pControl->pfHandleMouseUp(pMsg);
				--dword_6F8FE254;
			}
			pControl = pControl->pNext;
		}

		SMsgBreakHandlerChain(pMsg);
		// Should we return SMsgBreakHandlerChain's result?
		return;

	default:
		SMsgBreakHandlerChain(pMsg);
		// Should we return SMsgBreakHandlerChain's result?
		return;
	}
}

//D2Win.0x6F8AE220
//1.14d: 0x004FA340
void __stdcall D2Win_COMMANDS_MouseWheel_6F8AE220(SMSGHANDLER_PARAMS* pMsg)
{
	//TODO: Names
	const int nIncrement = (int)(int16_t)HIWORD(pMsg->wParam) / 120; // GET_WHEEL_DELTA_WPARAM

	D2Win_COMMANDS_MouseMove_6F8AD670(pMsg);

	if (POPUP_IsActive())
	{
		return;
	}

	D2WinControlStrc* pFirst = dword_6F8FE264;
	D2WinControlStrc* pScrollBar = nullptr;

	if (!dword_6F8FE264)
	{
		pFirst = gpControlList_6F8FE24C;
	}

	D2WinControlStrc* pControl = pFirst;

	if (pFirst)
	{
		do
		{
			if (pControl->nType == D2WIN_SCROLLBAR && pControl->dwFlags & gdwBitMasks[2] && pControl->dwFlags & gdwBitMasks[0] && SCROLLBAR_GetMaxSteps((D2WinScrollBarStrc*)pControl) > 0)
			{
				if (pScrollBar)
				{
					int nScrollBarX = 0;
					if (gMousePosition_6F8FE234.x < pScrollBar->nImageX)
					{
						nScrollBarX = gMousePosition_6F8FE234.x - pScrollBar->nImageX;
					}
					else if (gMousePosition_6F8FE234.x > pScrollBar->nWidth + pScrollBar->nImageX)
					{
						nScrollBarX = gMousePosition_6F8FE234.x - pScrollBar->nWidth - pScrollBar->nImageX;
					}

					int nScrollBarY = 0;
					if (gMousePosition_6F8FE234.y < pScrollBar->nImageY - pScrollBar->nHeight)
					{
						nScrollBarY = gMousePosition_6F8FE234.y + pScrollBar->nHeight - pScrollBar->nImageY;
					}
					else if (gMousePosition_6F8FE234.y > pScrollBar->nImageY)
					{
						nScrollBarY = gMousePosition_6F8FE234.y - pScrollBar->nImageY;
					}

					int nControlX = 0;
					if (gMousePosition_6F8FE234.x < pControl->nImageX)
					{
						nControlX = gMousePosition_6F8FE234.x - pControl->nImageX;
					}
					else if (gMousePosition_6F8FE234.x > pControl->nWidth + pControl->nImageX)
					{
						nControlX = gMousePosition_6F8FE234.x - pControl->nWidth - pControl->nImageX;
					}

					int nControlY = 0;
					if (gMousePosition_6F8FE234.y < pControl->nImageY - pControl->nHeight)
					{
						nControlY = gMousePosition_6F8FE234.y + pControl->nHeight - pControl->nImageY;
					}
					else if (gMousePosition_6F8FE234.y > pControl->nImageY)
					{
						nControlY = gMousePosition_6F8FE234.y - pControl->nImageY;
					}

					if (!nScrollBarY && nScrollBarX <= 0)
					{
						if (nControlY == 0 && nControlX <= 0 && nScrollBarX <= nControlX)
						{
							pScrollBar = pControl;
						}
					}
					else
					{
						if ((nControlY == 0 && nControlX <= 0)
							|| (nScrollBarX * nScrollBarX + nScrollBarY * nScrollBarY >= nControlX * nControlX + nControlY * nControlY))
						{
							pScrollBar = pControl;
						}
					}
				}
				else
				{
					pScrollBar = pControl;
				}
			}

			pControl = pControl->pNext;
		}
		while (pControl);

		if (pScrollBar && sub_6F8AF240(pMsg, (D2WinScrollBarStrc*)pScrollBar, nIncrement))
		{
			SMsgBreakHandlerChain(pMsg);
			// Should we return SMsgBreakHandlerChain's result?
			return;
		}
	}

	for (D2WinControlStrc* i = pFirst; i; i = i->pNext)
	{
		if (i->nType == D2WIN_BUTTON && BUTTON_OnMouseWheelScrolled((D2WinButtonStrc*)i, nIncrement))
		{
			break;
		}
	}

	SMsgBreakHandlerChain(pMsg);
	// Should we return SMsgBreakHandlerChain's result?
	return;
}

//D2Win.0x6F8AE4A0 (#10029)
//1.14d: 0x004FA410
void __stdcall D2Win_10029(int a1)
{
	dword_6F8BDE18 = a1;
}

//D2Win.0x6F8AE4B0 (#10030)
//1.14d: 0x004FA420
void __stdcall D2Win_10030_CONTROL_ToggleFourthFlag(D2WinControlStrc* pControl, int bSet)
{
	if (bSet)
	{
		pControl->dwFlags |= gdwBitMasks[3];
	}
	else
	{
		pControl->dwFlags &= gdwInvBitMasks[3];
	}
}

//D2Win.0x6F8AE4F0 (#10031)
//1.14d: 0x004FA450
void __stdcall CONTROL_SetEscapeButton(void* pButton)
{
	gpEscapeButton = pButton;
}

//D2Win.0x6F8AE500 (#10032)
//1.14d: 0x004FA460
int __stdcall D2Win_IsMouseButtonPressed()
{
	return gbIsMouseButtonPressed;
}

//D2Win.0x6F8AE510 (#10033)
//1.14d: 0x004FA470
int __stdcall D2Win_10033(D2WinControlStrc* a1, int a2)
{
	dword_6F8FE264 = a1;
	dword_6F8FE278 = a2;
	return 1;
}

//D2Win.0x6F8AE530 (#10035)
//1.14d: 0x004FA490
int __stdcall CONTROL_GetType(D2WinControlStrc* pControl)
{
	if (pControl)
	{
		return pControl->nType;
	}

	return 0;
}

//D2Win.0x6F8AE540
//1.14d: 0x004FA4B0
D2WinControlStrc* __stdcall sub_6F8AE540()
{
	return dword_6F8FE264;
}
