#pragma once

#include <Windows.h>

#include <cstdint>
#include <D2Constants.h>

struct D2CellFileStrc;
struct D2WinEditBoxStrc;
struct D2WinControlInitStrc;
struct D2WinControlStrc;
struct SMSGHANDLER_PARAMS;

extern POINT gMousePosition_6F8FE234;
extern int32_t dword_6F8FE254;


//D2Win.0x6F8ACC60
BOOL __stdcall D2Win_CreateWindow(HINSTANCE hInstance, int nRenderMode, BOOL bWindowed, BOOL bCompress);
//D2Win.0x6F8ACC90
BOOL __stdcall D2Win_InitializeSpriteCache(BOOL bWindowed, D2GameResolutionMode nResolution);
//D2Win.0x6F8ACD40
int __stdcall D2Win_10014_SetWndProc(WNDPROC wndProc);
//D2Win.0x6F8ACD60
int __fastcall D2Win_10015();
//D2Win.0x6F8ACDB0
int __stdcall D2Win_CloseSpriteCache();
//D2Win.0x6F8ACDD0
void __stdcall D2Win_10003();
//D2Win.0x6F8ACDE0
void __stdcall D2Win_10004(BOOL bForceResize);
//D2Win.0x6F8ACE20
int __stdcall D2Win_10016();
//D2Win.0x6F8ACEC0
int __stdcall D2Win_10005(BOOL bForceResize);
//D2Win.0x6F8ACF20
void __stdcall D2Win_10006_ClearDrawCaches();
//D2Win.0x6F8ACF30
int __stdcall BACKGROUND_SetCellFile(D2CellFileStrc* pCellFile);
//D2Win.0x6F8ACF50
int __stdcall CURSOR_SetCellFile(D2CellFileStrc* pCellFile);
//D2Win.0x6F8ACF70
void __stdcall D2Win_10010(int(*a1)());
//D2Win.0x6F8ACF80
int __stdcall D2Win_10009();
//D2Win.0x6F8ACF90
int __stdcall D2Win_10008(void(__stdcall* pCallback)(DWORD));
//D2Win.0x6F8AD0B0
int __stdcall CONTROL_SetCellFile(D2WinControlStrc* pControl, D2CellFileStrc* pCellFile);
//D2Win.0x6F8AD0D0
int __stdcall D2Win_10012(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8AD110
D2WinEditBoxStrc* __stdcall D2Win_10023();
//D2Win.0x6F8AD120
int __stdcall CONTROL_AppendToList(D2WinControlStrc* pControl);
//D2Win.0x6F8AD160
signed int __stdcall CONTROL_RemoveFromList(D2WinControlStrc* pControl);
//D2Win.0x6F8AD260
D2WinControlStrc* __stdcall CONTROL_Create(D2WinControlInitStrc* pControlInit);
//D2Win.0x6F8AD450
int __stdcall CONTROL_Destroy(void*a1);
//D2Win.0x6F8AD570
void __fastcall D2Win_10025_CONTROL_ToggleFirstFlag(D2WinControlStrc* pControl, int bSet);
//D2Win.0x6F8AD5A0
int __fastcall D2Win_10024_CONTROL_CheckFirstFlag(D2WinControlStrc* pControl);
//D2Win.0x6F8AD5D0
void __fastcall D2Win_10027_CONTROL_ToggleThirdFlag(D2WinControlStrc* pControl, int bSet);
//D2Win.0x6F8AD620
int __fastcall D2Win_10026_CONTROL_CheckThirdFlag(D2WinControlStrc* pControl);
//D2Win.0x6F8AD650
BOOL __stdcall CONTROL_MouseInsideRect(const RECT* pRect);
//D2Win.0x6F8AD670
void __stdcall D2Win_COMMANDS_MouseMove_6F8AD670(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AD6F0
void __stdcall D2Win_10019();
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
//D2Win.0x6F8ADCD0
void __stdcall D2Win_10168_WINMAIN_CreateScreenshot();
//D2Win.0x6F8ADE70
void __stdcall D2Win_COMMANDS_Snapshot_6F8ADE70(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE020
void __stdcall D2Win_COMMANDS_KeyF1_6F8AE020(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE070
void __stdcall D2Win_COMMANDS_MouseButton_6F8AE070(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE220
void __stdcall D2Win_COMMANDS_MouseWheel_6F8AE220(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AE4A0
void __stdcall D2Win_10029(int a1);
//D2Win.0x6F8AE4B0
void __stdcall D2Win_10030_CONTROL_ToggleFourthFlag(D2WinControlStrc* pControl, int bSet);
//D2Win.0x6F8AE4F0
void __stdcall D2Win_10031(void* a1);
//D2Win.0x6F8AE500
int __stdcall D2Win_IsMouseButtonPressed();
//D2Win.0x6F8AE510
signed int __stdcall D2Win_10033(D2WinControlStrc* a1, int a2);
//D2Win.0x6F8AE530
int __stdcall CONTROL_GetType(D2WinControlStrc* pControl);
//D2Win.0x6F8AE540
D2WinControlStrc* __stdcall sub_6F8AE540();
