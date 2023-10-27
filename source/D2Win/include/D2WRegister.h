#pragma once

#include "D2WinControlHeader.h"


#pragma pack(push, 1)
struct D2WinCommandRegisterStrc
{
	int32_t nType;									//0x00
	int32_t nId;									//0x04
	void(__stdcall* pCallback)(SMSGHANDLER_PARAMS*);		//0x08
};
#pragma pack(pop)


//D2Win.0x6F8B1DC0
void __fastcall WREGISTER_RegisterCommand(HWND hWnd, int32_t nType, int32_t nId, void(__stdcall* pCallback)(SMSGHANDLER_PARAMS*));
//D2Win.0x6F8B1EC0
void __fastcall WREGISTER_UnregisterCommand(HWND hWnd, int32_t nType, int32_t nId, void(__stdcall* pCallback)(SMSGHANDLER_PARAMS*));
//D2Win.0x6F8B1FC0
void __fastcall WREGISTER_RegisterCommands(HWND hWnd, D2WinCommandRegisterStrc* pCommandRegister, int32_t nCommands);
//D2Win.0x6F8B2010
void __fastcall WREGISTER_UnregisterCommands(HWND hWnd, D2WinCommandRegisterStrc* pCommandRegister, int32_t nCommands);
