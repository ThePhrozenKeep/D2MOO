#pragma once

#include <Windows.h>

#include <cstdint>
#include <D2Constants.h>
#include <DisplayType.h>

struct D2CellFileStrc;
struct D2WinEditBoxStrc;
struct D2WinControlInitStrc;
struct D2WinControlStrc;
struct SMSGHANDLER_PARAMS;

//1.10f: D2Win.0x6F8FE234
//1.14d: 0x007D55A4
extern POINT gMousePosition_6F8FE234;
//1.10f: D2Win.0x6F8FE254
//1.14d: 0x007D55C4
extern int32_t dword_6F8FE254;


//D2Win.0x6F8ACC60 (#10000)
//1.14d: 0x004FA520
D2WIN_DLL_DECL BOOL __stdcall D2Win_CreateWindow(HINSTANCE hInstance, DisplayType nRenderMode, BOOL bWindowed, BOOL bCompress);
//D2Win.0x6F8ACC90 (#10001)
//1.14d: 0x004F8F10
D2WIN_DLL_DECL BOOL __stdcall D2Win_InitializeSpriteCache(BOOL bWindowed, D2GameResolutionMode nResolution);
//D2Win.0x6F8ACD40 (#10014)
//1.14d: 0x004F8FC0
D2WIN_DLL_DECL int __stdcall D2Win_10014_SetWndProc(WNDPROC wndProc);
//D2Win.0x6F8ACD60 (#10015)
//1.14d: 0x004F8FE0
D2WIN_DLL_DECL int __fastcall D2Win_10015();
//D2Win.0x6F8ACDB0 (#10002)
//1.14d: 0x004F9030
D2WIN_DLL_DECL int __stdcall D2Win_CloseSpriteCache();
//D2Win.0x6F8ACDD0 (#10003)
//1.14d: 0x004F9050
D2WIN_DLL_DECL void __stdcall D2Win_10003();
//D2Win.0x6F8ACDE0 (#10004)
//1.14d: 0x004F9060
D2WIN_DLL_DECL void __stdcall D2Win_10004(BOOL bForceResize);
//D2Win.0x6F8ACE20 (#10016)
//1.14d: 0x004FA550
D2WIN_DLL_DECL int __stdcall D2Win_10016();
//D2Win.0x6F8ACEC0 (#10005)
//1.14d: 0x004F90A0
D2WIN_DLL_DECL int __stdcall D2Win_10005(BOOL bForceResize);
//D2Win.0x6F8ACF20 (#10006)
//1.14d: 0x004F9130
D2WIN_DLL_DECL void __stdcall D2Win_10006_ClearDrawCaches();
//D2Win.0x6F8ACF30 (#10007)
//1.14d: 0x004F9140
D2WIN_DLL_DECL int __stdcall BACKGROUND_SetCellFile(D2CellFileStrc* pCellFile);
//D2Win.0x6F8ACF50 (#10011)
//1.14d: 0x004F9160
D2WIN_DLL_DECL int __stdcall CURSOR_SetCellFile(D2CellFileStrc* pCellFile);
//D2Win.0x6F8ACF70 (#10010)
//1.14d: 0x004F9180
D2WIN_DLL_DECL void __stdcall D2Win_SetQuitCallback(int(*pfOnQuit)());
//D2Win.0x6F8ACF80 (#10009)
//1.14d: 0x004F9190
D2WIN_DLL_DECL int __stdcall D2Win_StopMessageLoop();
//D2Win.0x6F8ACF90 (#10008)
//1.14d: 0x004FA590
D2WIN_DLL_DECL int __stdcall D2Win_RunMessageLoop(void(__stdcall* pfOnFrame)(DWORD));
//D2Win.0x6F8AD0B0 (#10013)
//1.14d: 0x004F91A0
D2WIN_DLL_DECL int __stdcall CONTROL_SetCellFile(D2WinControlStrc* pControl, D2CellFileStrc* pCellFile);
//D2Win.0x6F8AD0D0 (#10012)
//1.14d: 0x004F91C0
D2WIN_DLL_DECL int __stdcall CONTROL_SetFocusedEditBox(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8AD110 (#10023)
//1.14d: 0x004F9200
D2WIN_DLL_DECL D2WinEditBoxStrc* __stdcall CONTROL_GetFocusedEditBox();
//D2Win.0x6F8AD120
//1.14d: 0x004F9210
int __stdcall CONTROL_AppendToList(D2WinControlStrc* pControl);
//D2Win.0x6F8AD160
//1.14d: 0x004F9310
signed int __stdcall CONTROL_RemoveFromList(D2WinControlStrc* pControl);
//D2Win.0x6F8AD260 (#10017)
//1.14d: 0x004F93C0
D2WIN_DLL_DECL D2WinControlStrc* __stdcall CONTROL_Create(D2WinControlInitStrc* pControlInit);
//D2Win.0x6F8AD450 (#10018)
//1.14d: 0x004F95C0
D2WIN_DLL_DECL int __stdcall CONTROL_Destroy(void*ppControl);
//D2Win.0x6F8AD570 (#10025)
//1.14d: 0x004F96F0
D2WIN_DLL_DECL void __fastcall CONTROL_SetEnabled(D2WinControlStrc* pControl, int bEnabled);
//D2Win.0x6F8AD5A0 (#10024)
//1.14d: 0x004F9710
D2WIN_DLL_DECL int __fastcall CONTROL_IsEnabled(D2WinControlStrc* pControl);
//D2Win.0x6F8AD5D0 (#10027)
//1.14d: 0x004F9740
D2WIN_DLL_DECL void __fastcall CONTROL_SetVisible(D2WinControlStrc* pControl, int bVisible);
//D2Win.0x6F8AD620 (#10026)
//1.14d: 0x004F9780
D2WIN_DLL_DECL int __fastcall CONTROL_IsVisible(D2WinControlStrc* pControl);
//D2Win.0x6F8AD650 (#10170)
//1.14d: 0x004F97B0
D2WIN_DLL_DECL BOOL __stdcall CONTROL_MouseInsideRect(const RECT* pRect);
//D2Win.0x6F8AD670
//1.14d: 0x004FA6B0
void __stdcall D2Win_COMMANDS_MouseMove_6F8AD670(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AD6F0 (#10019)
//1.14d: 0x004F98E0
D2WIN_DLL_DECL void __stdcall D2Win_UpdateAndDraw();
//D2Win.0x6F8AD9B0
//1.14d: 0x004F9A80
LRESULT __stdcall D2Win_MAINWINDOW_WndProc_6F8AD9B0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
//D2Win.0x6F8ADA80
//1.14d: 0x004F9B60
void __stdcall D2Win_COMMANDS_Char_6F8ADA80(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADB10
//1.14d: 0x004FA760
void __stdcall D2Win_COMMANDS_Escape_6F8ADB10(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADB50
//1.14d: 0x004F9BF0
void __stdcall D2Win_COMMANDS_VirtualKey_6F8ADB50(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADBE0
//1.14d: 0x004F9C80
void __stdcall D2Win_COMMANDS_Control_6F8ADBE0(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADBF0
//1.14d: 0x004F9CA0
void __stdcall D2Win_COMMANDS_Control_6F8ADBF0(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADC00
//1.14d: 0x004F9CC0
void __stdcall D2Win_COMMANDS_SysCommand_6F8ADC00(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADC40
//1.14d: 0x004F9F20
void __stdcall D2Win_COMMANDS_ActivateApp_6F8ADC40(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADCD0 (#10168)
//1.14d: 0x004FA7A0
D2WIN_DLL_DECL void __stdcall D2Win_10168_WINMAIN_CreateScreenshot();
//D2Win.0x6F8ADE70
//1.14d: 0x004FA840
void __stdcall D2Win_COMMANDS_Snapshot_6F8ADE70(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE020
//1.14d: 0x004FA160
void __stdcall D2Win_COMMANDS_ToggleMusic_6F8AE020(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE070
//1.14d: 0x004FA860
void __stdcall D2Win_COMMANDS_MouseButton_6F8AE070(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE220
//1.14d: 0x004FA340
void __stdcall D2Win_COMMANDS_MouseWheel_6F8AE220(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE4A0 (#10029)
//1.14d: 0x004FA410
D2WIN_DLL_DECL void __stdcall D2Win_10029(int a1);
//D2Win.0x6F8AE4B0 (#10030)
//1.14d: 0x004FA420
D2WIN_DLL_DECL void __stdcall D2Win_10030_CONTROL_ToggleFourthFlag(D2WinControlStrc* pControl, int bSet);
//D2Win.0x6F8AE4F0 (#10031)
//1.14d: 0x004FA450
D2WIN_DLL_DECL void __stdcall CONTROL_SetEscapeButton(void* pButton);
//D2Win.0x6F8AE500 (#10032)
//1.14d: 0x004FA460
D2WIN_DLL_DECL int __stdcall D2Win_IsMouseButtonPressed();
//D2Win.0x6F8AE510 (#10033)
//1.14d: 0x004FA470
D2WIN_DLL_DECL signed int __stdcall D2Win_10033(D2WinControlStrc* a1, int a2);
//D2Win.0x6F8AE530 (#10035)
//1.14d: 0x004FA490
D2WIN_DLL_DECL int __stdcall CONTROL_GetType(D2WinControlStrc* pControl);
//D2Win.0x6F8AE540
//1.14d: 0x004FA4B0
D2WinControlStrc* __stdcall sub_6F8AE540();
