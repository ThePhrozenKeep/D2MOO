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

extern POINT gMousePosition_6F8FE234;
extern int32_t dword_6F8FE254;


//D2Win.0x6F8ACC60 (#10000)
D2WIN_DLL_DECL BOOL __stdcall D2Win_CreateWindow(HINSTANCE hInstance, DisplayType nRenderMode, BOOL bWindowed, BOOL bCompress);
//D2Win.0x6F8ACC90 (#10001)
D2WIN_DLL_DECL BOOL __stdcall D2Win_InitializeSpriteCache(BOOL bWindowed, D2GameResolutionMode nResolution);
//D2Win.0x6F8ACD40 (#10014)
D2WIN_DLL_DECL int __stdcall D2Win_10014_SetWndProc(WNDPROC wndProc);
//D2Win.0x6F8ACD60 (#10015)
D2WIN_DLL_DECL int __fastcall D2Win_10015();
//D2Win.0x6F8ACDB0 (#10002)
D2WIN_DLL_DECL int __stdcall D2Win_CloseSpriteCache();
//D2Win.0x6F8ACDD0 (#10003)
D2WIN_DLL_DECL void __stdcall D2Win_10003();
//D2Win.0x6F8ACDE0 (#10004)
D2WIN_DLL_DECL void __stdcall D2Win_10004(BOOL bForceResize);
//D2Win.0x6F8ACE20 (#10016)
D2WIN_DLL_DECL int __stdcall D2Win_10016();
//D2Win.0x6F8ACEC0 (#10005)
D2WIN_DLL_DECL int __stdcall D2Win_10005(BOOL bForceResize);
//D2Win.0x6F8ACF20 (#10006)
D2WIN_DLL_DECL void __stdcall D2Win_10006_ClearDrawCaches();
//D2Win.0x6F8ACF30 (#10007)
D2WIN_DLL_DECL int __stdcall BACKGROUND_SetCellFile(D2CellFileStrc* pCellFile);
//D2Win.0x6F8ACF50 (#10011)
D2WIN_DLL_DECL int __stdcall CURSOR_SetCellFile(D2CellFileStrc* pCellFile);
//D2Win.0x6F8ACF70 (#10010)
D2WIN_DLL_DECL void __stdcall D2Win_10010(int(*a1)());
//D2Win.0x6F8ACF80 (#10009)
D2WIN_DLL_DECL int __stdcall D2Win_10009();
//D2Win.0x6F8ACF90 (#10008)
D2WIN_DLL_DECL int __stdcall D2Win_10008(void(__stdcall* pCallback)(DWORD));
//D2Win.0x6F8AD0B0 (#10013)
D2WIN_DLL_DECL int __stdcall CONTROL_SetCellFile(D2WinControlStrc* pControl, D2CellFileStrc* pCellFile);
//D2Win.0x6F8AD0D0 (#10012)
D2WIN_DLL_DECL int __stdcall D2Win_10012(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8AD110 (#10023)
D2WIN_DLL_DECL D2WinEditBoxStrc* __stdcall D2Win_10023();
//D2Win.0x6F8AD120
int __stdcall CONTROL_AppendToList(D2WinControlStrc* pControl);
//D2Win.0x6F8AD160
signed int __stdcall CONTROL_RemoveFromList(D2WinControlStrc* pControl);
//D2Win.0x6F8AD260 (#10017)
D2WIN_DLL_DECL D2WinControlStrc* __stdcall CONTROL_Create(D2WinControlInitStrc* pControlInit);
//D2Win.0x6F8AD450 (#10018)
D2WIN_DLL_DECL int __stdcall CONTROL_Destroy(void*a1);
//D2Win.0x6F8AD570 (#10025)
D2WIN_DLL_DECL void __fastcall D2Win_10025_CONTROL_ToggleFirstFlag(D2WinControlStrc* pControl, int bSet);
//D2Win.0x6F8AD5A0 (#10024)
D2WIN_DLL_DECL int __fastcall D2Win_10024_CONTROL_CheckFirstFlag(D2WinControlStrc* pControl);
//D2Win.0x6F8AD5D0 (#10027)
D2WIN_DLL_DECL void __fastcall D2Win_10027_CONTROL_ToggleThirdFlag(D2WinControlStrc* pControl, int bSet);
//D2Win.0x6F8AD620 (#10026)
D2WIN_DLL_DECL int __fastcall D2Win_10026_CONTROL_CheckThirdFlag(D2WinControlStrc* pControl);
//D2Win.0x6F8AD650 (#10170)
D2WIN_DLL_DECL BOOL __stdcall CONTROL_MouseInsideRect(const RECT* pRect);
//D2Win.0x6F8AD670
void __stdcall D2Win_COMMANDS_MouseMove_6F8AD670(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AD6F0 (#10019)
D2WIN_DLL_DECL void __stdcall D2Win_10019();
//D2Win.0x6F8AD9B0
LRESULT __stdcall D2Win_MAINWINDOW_WndProc_6F8AD9B0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
//D2Win.0x6F8ADA80
void __stdcall D2Win_COMMANDS_Char_6F8ADA80(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADB10
void __stdcall D2Win_COMMANDS_Escape_6F8ADB10(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADB50
void __stdcall D2Win_COMMANDS_VirtualKey_6F8ADB50(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADBE0
void __stdcall D2Win_COMMANDS_Control_6F8ADBE0(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADBF0
void __stdcall D2Win_COMMANDS_Control_6F8ADBF0(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADC00
void __stdcall D2Win_COMMANDS_SysCommand_6F8ADC00(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADC40
void __stdcall D2Win_COMMANDS_ActivateApp_6F8ADC40(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ADCD0 (#10168)
D2WIN_DLL_DECL void __stdcall D2Win_10168_WINMAIN_CreateScreenshot();
//D2Win.0x6F8ADE70
void __stdcall D2Win_COMMANDS_Snapshot_6F8ADE70(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE020
void __stdcall D2Win_COMMANDS_KeyF1_6F8AE020(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE070
void __stdcall D2Win_COMMANDS_MouseButton_6F8AE070(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE220
void __stdcall D2Win_COMMANDS_MouseWheel_6F8AE220(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE4A0 (#10029)
D2WIN_DLL_DECL void __stdcall D2Win_10029(int a1);
//D2Win.0x6F8AE4B0 (#10030)
D2WIN_DLL_DECL void __stdcall D2Win_10030_CONTROL_ToggleFourthFlag(D2WinControlStrc* pControl, int bSet);
//D2Win.0x6F8AE4F0 (#10031)
D2WIN_DLL_DECL void __stdcall D2Win_10031(void* a1);
//D2Win.0x6F8AE500 (#10032)
D2WIN_DLL_DECL int __stdcall D2Win_IsMouseButtonPressed();
//D2Win.0x6F8AE510 (#10033)
D2WIN_DLL_DECL signed int __stdcall D2Win_10033(D2WinControlStrc* a1, int a2);
//D2Win.0x6F8AE530 (#10035)
D2WIN_DLL_DECL int __stdcall CONTROL_GetType(D2WinControlStrc* pControl);
//D2Win.0x6F8AE540
D2WinControlStrc* __stdcall sub_6F8AE540();
