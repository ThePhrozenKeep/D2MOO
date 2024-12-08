#include <Core/WNDPROC.h> 
#include <Fog.h>
#include <D2Client.h>
#include <Window.h>
#include <tracy/Tracy.hpp>

D2CLIENTSTUB(RECORD_sub, 6FAAE5B0, void, __fastcall, (UINT Msg, WPARAM wParam, LPARAM lParam));
D2CLIENTDWORDSTUB(6FBAB990);

D2VAR(D2CLIENT, dword_6FB9A954, HACCEL, 0x6FB9A954 - D2ClientImageBase);
D2VAR(D2CLIENT, hAcc_6FB9A710, HACCEL, 0x6FB9A710 - D2ClientImageBase);

int __fastcall sub_6FAAEB10()
{
	return *D2CLIENT_pdword_6FBAB990;
}

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

//1.00 :D2Client.0x
//1.10f:D2Client.0x6FAA25D0
//1.13c:D2Client.0x
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
				HWND v4; // eax
				HACCEL v8;
				if (!*D2CLIENT_hAcc_6FB9A710 || (v8 = *D2CLIENT_hAcc_6FB9A710, v4 = WINDOW_GetWindow(), !TranslateAcceleratorA(v4, v8, &Msg)))
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

//1.10f:D2Client.0x6FAA26C0
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

//1.10f:D2Client.0x6FAA2780
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
