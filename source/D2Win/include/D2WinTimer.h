#pragma once

#include "D2WinControlHeader.h"


#pragma pack(push, 1)
struct D2WinTimerStrc
{
	D2WinControlStrc controlHeader;					//0x00
	uint32_t nTimeout;								//0x40
	uint32_t dwTickCount;							//0x44
};
#pragma pack(pop)


//D2Win.0x6F8B1B70
D2WinTimerStrc* __fastcall TIMER_Create(uint32_t nTimeout, int32_t(__stdcall* a2)(D2WinMsgStrc*));
//D2Win.0x6F8B1BF0
int32_t __fastcall TIMER_Destroy(D2WinControlStrc* pControl);
//D2Win.0x6F8B1C00
void __fastcall D2Win_10180(D2WinTimerStrc* pTimer);
//D2Win.0x6F8B1C30
void __fastcall D2Win_10182(D2WinTimerStrc* pTimer);
//D2Win.0x6F8B1C60
int32_t __fastcall sub_6F8B1C60(D2WinControlStrc* pControl);
//D2Win.0x6F8B1CF0
int32_t __fastcall sub_6F8B1CF0(D2WinControlStrc* pControl);
//D2Win.0x6F8B1D70
int32_t __stdcall TIMER_HandleVirtualKeyInput(D2WinMsgStrc* pMsg);
