#include "D2WRegister.h"

#include <Fog.h>
#include <Storm.h>


//D2Win.0x6F8B1DC0 (#10020)
void __fastcall WREGISTER_RegisterCommand(HWND hWnd, int32_t nType, int32_t nId, void(__stdcall* pCallback)(SMSGHANDLER_PARAMS*))
{
	D2_ASSERT(hWnd);

	switch (nType)
	{
	case 0:
		D2_ASSERT(nId != WM_COMMAND);
		D2_ASSERT(nId != WM_KEYUP);
		D2_ASSERT(nId != WM_KEYDOWN);

		SMsgRegisterMessage(hWnd, nId, pCallback);
		break;

	case 1:
		SMsgRegisterCommand(hWnd, nId, pCallback);
		break;

	case 2:
		SMsgRegisterKeyUp(hWnd, nId, pCallback);
		break;

	case 3:
		SMsgRegisterKeyDown(hWnd, nId, pCallback);
		break;

	default:
		return;
	}
}

//D2Win.0x6F8B1EC0 (#10021)
void __fastcall WREGISTER_UnregisterCommand(HWND hWnd, int32_t nType, int32_t nId, void(__stdcall* pCallback)(SMSGHANDLER_PARAMS*))
{
	D2_ASSERT(hWnd);

	switch (nType)
	{
	case 0:
		D2_ASSERT(nId != WM_COMMAND);
		D2_ASSERT(nId != WM_KEYUP);
		D2_ASSERT(nId != WM_KEYDOWN);

		SMsgUnregisterMessage(hWnd, nId, pCallback);
		break;

	case 1:
		SMsgUnregisterCommand(hWnd, nId, pCallback);
		break;

	case 2:
		SMsgUnregisterKeyUp(hWnd, nId, pCallback);
		break;

	case 3:
		SMsgUnregisterKeyDown(hWnd, nId, pCallback);
		break;

	default:
		return;
	}
}

//D2Win.0x6F8B1FC0 (#10022)
void __fastcall WREGISTER_RegisterCommands(HWND hWnd, D2WinCommandRegisterStrc* pCommandRegister, int32_t nCommands)
{
	D2_ASSERT(hWnd);

	for (int32_t i = 0; i < nCommands; ++i)
	{
		WREGISTER_RegisterCommand(hWnd, pCommandRegister[i].nType, pCommandRegister[i].nId, pCommandRegister[i].pCallback);
	}
}

//D2Win.0x6F8B2010
void __fastcall WREGISTER_UnregisterCommands(HWND hWnd, D2WinCommandRegisterStrc* pCommandRegister, int32_t nCommands)
{
	D2_ASSERT(hWnd);

	for (int32_t i = 0; i < nCommands; ++i)
	{
		WREGISTER_UnregisterCommand(hWnd, pCommandRegister[i].nType, pCommandRegister[i].nId, pCommandRegister[i].pCallback);
	}
}
