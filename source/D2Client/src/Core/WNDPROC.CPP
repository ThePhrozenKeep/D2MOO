#include <Core/WNDPROC.h> 
#include <Fog.h>
#include <D2Client.h>
#include <D2Gfx.h>
#include <D2WinMain.h>
#include <Window.h>
#include <Core/Lng.h>
#include <tracy/Tracy.hpp>
#include <CGAME/scroll.h>

#ifdef D2_VERSION_110F
D2CLIENTDWORDSTUB(6FBAB990);
D2VAR(D2CLIENT, gpbWindowHasFocus, DWORD, 0x6FB747C0 - D2ClientImageBase);

D2VAR(D2CLIENT, dword_6FB9A954, HACCEL, 0x6FB9A954 - D2ClientImageBase);
D2VAR(D2CLIENT, pgnDrawMaxWidth, int32_t, 0x6FB9A704 - D2ClientImageBase); // 1.13c: D2Client.0x6FBA9E14
D2VAR(D2CLIENT, pgnDrawMaxHeight, int32_t, 0x6FB9A700 - D2ClientImageBase);// 1.13c: D2Client.0x6FBA9E18

D2VAR(D2CLIENT, pgnScreenWidth, uint32_t, 0x6FB740EC - D2ClientImageBase);
D2VAR(D2CLIENT, pgnScreenHeight, uint32_t, 0x6FB740F0 - D2ClientImageBase);
D2VAR(D2CLIENT, pgbIsHigherResolution, BOOL, 0x6FB9A708 - D2ClientImageBase);
D2VAR(D2CLIENT, pgSomeObject, uint32_t, 0x6FBB5D4F - D2ClientImageBase);
D2VAR(D2CLIENT, pgpSomeDialog_6FBB5D43, uint32_t, 0x6FBB5D43 - D2ClientImageBase);
//1.10f: D2Client.0x6FBAB9C4
D2VAR(D2CLIENT, pgdwUiNbPanels, D2C_GlobalUiStates, 0x6FBAB9C4 - D2ClientImageBase);

D2CLIENTSTUB(RECORD_sub, 6FAAE5B0, void, __fastcall, (UINT Msg, WPARAM wParam, LPARAM lParam));
D2CLIENTSTUB(sub, 6FACB6E0, int, __fastcall, (BOOL));
void D2CLIENT_Return_6FAA1500();
D2CLIENTSTUB(UI_ReopenMsgLog, 6FB1F320, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAD6DC0, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAF65C0, void, __fastcall, ());

//1.10f: D2Client.0x6FB740F4
int32_t gnDrawMaxWidth_Unk; // unused

int __fastcall sub_6FAAEB10()
{
	return *D2CLIENT_pdword_6FBAB990;
}

//1.10f: D2Client.0x6FAB5750
D2C_GlobalUiStates __fastcall D2CLIENT_GetGlobalUiStateNbPanels()
{
	return *D2CLIENT_pgdwUiNbPanels;
}

//1.10f: D2Client.0x6FAA23B0
void __fastcall D2CLIENT_UI_ChangeResolution(D2GameResolutionMode nResolutionMode)
{
	if (nResolutionMode != D2GFX_GetResolutionMode())
	{
		switch (nResolutionMode)
		{
		case D2GAMERES_640x480:
			*D2CLIENT_pgnScreenWidth = 640;
			*D2CLIENT_pgnScreenHeight = 480;
			*D2CLIENT_pgbIsHigherResolution = 0;
			break;
		case D2GAMERES_800x600:
			*D2CLIENT_pgnScreenWidth = 800;
			*D2CLIENT_pgnScreenHeight = 600;
			*D2CLIENT_pgbIsHigherResolution = 1;
			break;
		default:
			break;
		}
		*D2CLIENT_pgnDrawMaxHeight = *D2CLIENT_pgnScreenHeight - 40;
		*D2CLIENT_pgnDrawMaxWidth = *D2CLIENT_pgnScreenWidth;
		gnDrawMaxWidth_Unk = *D2CLIENT_pgnScreenWidth;

		D2Win_10005(nResolutionMode);
		UI_AdjustViewMatrix(D2CLIENT_GetGlobalUiStateNbPanels());
		sub_6FACB6E0(1);
		D2CLIENT_Return_6FAA1500();
		D2CLIENT_UI_ReopenMsgLog_6FB1F320();
		if (*D2CLIENT_pgSomeObject)
		{
			if (*D2CLIENT_pgpSomeDialog_6FBB5D43)
			{
				sub_6FAD6DC0();
				if (*D2CLIENT_pgSomeObject)
					(**(void(__thiscall***)(uintptr_t, int))*D2CLIENT_pgSomeObject)(*D2CLIENT_pgSomeObject, 1);
				*D2CLIENT_pgSomeObject = 0;
				*D2CLIENT_pgpSomeDialog_6FBB5D43 = 0;
			}
			sub_6FAF65C0();
		}
	}
}

//1.10f: D2Client.0x6FAA2460
void __stdcall sub_6FAA2460(SMSGHANDLER_PARAMS* pParams)
{
	PostQuitMessage(0);
	pParams->bUseResult = TRUE;
	pParams->lResult = 0;
}

//1.10f: D2Client.0x6FAA2480
void __stdcall sub_6FAA2480(SMSGHANDLER_PARAMS* pParams)
{
	*D2CLIENT_gpbWindowHasFocus = pParams->wParam != 0;
	pParams->bUseResult = 0;
}

//1.10f: D2Client.0x6FAA24A0
void __stdcall sub_6FAA24A0(SMSGHANDLER_PARAMS* pParams)
{
	pParams->bUseResult = TRUE;
	pParams->lResult = DefWindowProcA(pParams->hWindow, pParams->nMessage, pParams->wParam, pParams->lParam);
}

//1.10f: D2Client.0x6FAA24D0
void __stdcall sub_6FAA24D0(SMSGHANDLER_PARAMS* pParams)
{
	SendMessageA(pParams->hWindow, 0x10u, 0, 0);
	pParams->bUseResult = 1;
	pParams->lResult = 0;
}

//1.10f: D2Client.0x6FAA2500
void __stdcall sub_6FAA2500(SMSGHANDLER_PARAMS* pParams)
{
	pParams->bUseResult = TRUE;
	pParams->lResult = 0;
}

//1.10f: D2Client.0x6FAA2520
void __stdcall sub_6FAA2520(SMSGHANDLER_PARAMS* pParams)
{
	SendMessageA(pParams->hWindow, 0x400u, 0, 0);
	pParams->bUseResult = TRUE;
	pParams->lResult = 0;
}

//1.10f: D2Client.0x6FAA2550
LRESULT __stdcall WNDPROC_WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	BOOL bUseResult = FALSE;
	LRESULT result = NULL;
	D2CLIENT_RECORD_sub_6FAAE5B0(Msg, wParam, lParam);
	if ((!sub_6FAAEB10() || Msg == WM_CLOSE) && SMsgDispatchMessage(hWnd, Msg, wParam, lParam, &bUseResult, &result) && bUseResult)
		return result;
	else
		return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

//1.00 : D2Client.0x
//1.10f: D2Client.0x6FAA25D0
//1.13c: D2Client.0x
int __fastcall ExecuteMessageLoop_6FAA25D0(int(__stdcall* pLoopBody)(int))
{
	int nIterationCount = 0;
	BOOL v3 = TRUE;
	do
	{
		struct tagMSG Msg;
		if (PeekMessageA(&Msg, 0, 0, 0, 0))
		{
			v3 = GetMessageA(&Msg, 0, 0, 0);
			if (v3)
			{
				if (!LNG_GetAccelerationTable() || (!TranslateAcceleratorA(WINDOW_GetWindow(), LNG_GetAccelerationTable(), &Msg)))
				{
					if (*D2CLIENT_dword_6FB9A954)
					{
						HACCEL v9 = *D2CLIENT_dword_6FB9A954;
						TranslateAcceleratorA(WINDOW_GetWindow(), v9, &Msg);
					}
				}
			}
			TranslateMessage(&Msg);
			DispatchMessageA(&Msg);
		}
		else
		{
			if (pLoopBody)
			{
				const int result = pLoopBody(nIterationCount++);
				if (result)
					return result;
			}
#define FORCE_CPU_SLEEP 1
#if FORCE_CPU_SLEEP
			{
				// Original game will loop only to dequeue packets without ever spinning. To avoid burning the CPU, we insert 1ms sleeps. (which is not a good way to fix it, but better than nothing)
				ZoneScopedNC("Sleep", 0xFFFFFFFF);
				Sleep(1u);
			}
#else
			if (!*D2CLIENT_gpbWindowHasFocus
				&& *D2CLIENT_pgnGameType_6FBA7960 != GAMETYPE_OBNET_HOST
				&& *D2CLIENT_pgnGameType_6FBA7960 != GAMETYPE_LAN_HOST)
			{
				ZoneScopedNC("Sleep", 0xFFFFFFFF);
				Sleep(10u);
			}
#endif
		}
	} while (v3);
	return 1;
}

//1.10f: D2Client.0x6FAA26C0
void __fastcall D2CLIENT_INPUT_RegisterCallbacks(HWND hWnd, D2WindowProcCallbackStrc* pCallbacks, size_t nCallbacks)
{
	D2_ASSERT(hWnd);
	for (int i = 0; i < nCallbacks; i++)
	{
		const auto& callback = pCallbacks[i];
		const UINT id = callback.uMessage;
		switch (callback.nType)
		{
		case D2_WINPROC_MESSAGE:
			D2_ASSERT(id != WM_COMMAND);
			SMsgRegisterMessage(hWnd, id, callback.pfCallback);
			break;
		case D2_WINPROC_COMMAND:
			SMsgRegisterCommand(hWnd, id, callback.pfCallback);
			break;
		case D2_WINPROC_KEYUP:
			SMsgRegisterKeyUp(hWnd, id, callback.pfCallback);
			break;
		case D2_WINPROC_KEYDOWN:
			SMsgRegisterKeyDown(hWnd, id, callback.pfCallback);
			break;
		default:
			break;
		}
	}
}

//1.10f: D2Client.0x6FAA2780
void __fastcall D2CLIENT_INPUT_UnregisterCallbacks(HWND hWnd, D2WindowProcCallbackStrc* pCallbacks, size_t nCallbacks)
{
	D2_ASSERT(hWnd);

	for (int i = 0; i < nCallbacks; i++)
	{
		const auto& callback = pCallbacks[i];
		const UINT id = callback.uMessage;
		switch (callback.nType)
		{
		case D2_WINPROC_MESSAGE:
			D2_ASSERT(id != WM_COMMAND);
			SMsgUnregisterMessage(hWnd, id, callback.pfCallback);
			break;
		case D2_WINPROC_COMMAND:
			SMsgUnregisterCommand(hWnd, id, callback.pfCallback);
			break;
		case D2_WINPROC_KEYUP:
			SMsgUnregisterKeyUp(hWnd, id, callback.pfCallback);
			break;
		case D2_WINPROC_KEYDOWN:
			SMsgUnregisterKeyDown(hWnd, id, callback.pfCallback);
			break;
		default:
			break;
		}
	}
}
#endif //D2_VERSION_110F
