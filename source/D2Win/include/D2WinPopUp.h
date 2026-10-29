#pragma once

#include "D2WinControlHeader.h"
#include <D2Unicode.h>

#pragma pack(push, 1)
struct D2WinPopUpLineStrc
{
	wchar_t wszText[64];							//0x00
	void(__stdcall* pfHandleMouseInput)();			//0x80
};

struct D2WinPopUpStrc
{
	D2WinControlStrc controlHeader;					//0x00
	int32_t nLines;									//0x40
	D2WinPopUpLineStrc lines[8];					//0x44
	int32_t nCurrentLine;							//0x464
};
#pragma pack(pop)


//D2Win.0x6F8AEAE0
//1.14d: 0x005088C0
void __stdcall POPUP_Create();
//D2Win.0x6F8AEBA0
//1.14d: 0x005085A0
void __stdcall POPUP_Destroy();
//D2Win.0x6F8AEBC0
//1.14d: 0x005085C0
int32_t __stdcall POPUP_IsActive();
//D2Win.0x6F8AEC10
//1.14d: 0x00508610
int32_t __stdcall POPUP_AddLine(D2WinPopUpLineStrc* pPopUpLine);
//D2Win.0x6F8AECA0
//1.14d: 0x005086A0
int32_t __stdcall POPUP_HandleMouseButtonInput(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AED10
//1.14d: 0x00508710
int32_t __fastcall POPUP_UpdateHoveredLine(D2WinControlStrc* pControl);
//D2Win.0x6F8AEDB0
//1.14d: 0x005087A0
int32_t __fastcall POPUP_Draw(D2WinControlStrc* pControl);
