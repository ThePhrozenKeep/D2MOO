#pragma once  

#include <D2BasicTypes.h>
#include <Storm.h>

#pragma pack(push, 1)

enum D2WindowProcCallbackType : uint32_t {
	D2_WINPROC_MESSAGE,
	D2_WINPROC_COMMAND,
	D2_WINPROC_KEYUP,
	D2_WINPROC_KEYDOWN,
};

struct D2WindowProcCallbackStrc
{
	D2WindowProcCallbackType nType; // 0x00
	UINT uMessage;					// 0x04 Message, Command or Key (WM_*, ?, VK_*)
	SMSGHANDLER pfCallback;			// 0x08
};
#pragma pack(pop)


//1.10f:D2Client.0x6FAB5750
int32_t __fastcall D2CLIENT_GetGlobalUiStateNbPanels();

//1.10f:D2Client.0x6FAA2550
LRESULT __stdcall WNDPROC_WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

//1.00 :D2Client.0x
//1.10f:D2Client.0x6FAA25D0
//1.13c:D2Client.0x
int __fastcall ExecuteMessageLoop_6FAA25D0(int(__stdcall* pLoopBody)(int));

//1.10f:D2Client.0x6FAA26C0
void __fastcall D2CLIENT_INPUT_RegisterCallbacks(HWND hWnd, D2WindowProcCallbackStrc* pCallbacks, size_t nCallbacks);

//1.10f:D2Client.0x6FAA2780
void __fastcall D2CLIENT_INPUT_UnregisterCallbacks(HWND hWnd, D2WindowProcCallbackStrc* pCallbacks, size_t nCallbacks);
