#include <Core/WNDPROC.h> 
#include <Fog.h>

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
