#pragma once

#include "D2WinControlHeader.h"


#pragma pack(push, 1)
struct D2WinTimerStrc
{
	D2WinControlStrc controlHeader;					//0x00
	uint32_t nTimeout;								//0x40 seconds; milliseconds after TIMER_SetRepeating
	uint32_t dwTickCount;							//0x44
};
#pragma pack(pop)


//D2Win.0x6F8B1B70
//1.14d: 0x004FD9D0
D2WinTimerStrc* __fastcall TIMER_Create(uint32_t nTimeout, int32_t(__stdcall* pfCallback)(SMSGHANDLER_PARAMS*));
//D2Win.0x6F8B1BF0
//1.14d: 0x004FD820
int32_t __fastcall TIMER_Destroy(D2WinControlStrc* pControl);
//D2Win.0x6F8B1C00
//1.14d: 0x004FD830
void __fastcall TIMER_DisableSpaceKey(D2WinTimerStrc* pTimer);
//D2Win.0x6F8B1C30
//1.14d: 0x004FDA50
void __fastcall TIMER_SetRepeating(D2WinTimerStrc* pTimer);
//D2Win.0x6F8B1C60
//1.14d: 0x004FD860
int32_t __fastcall TIMER_UpdateDefault(D2WinControlStrc* pControl);
//D2Win.0x6F8B1CF0
//1.14d: 0x004FD8F0
int32_t __fastcall TIMER_UpdateRepeating(D2WinControlStrc* pControl);
//D2Win.0x6F8B1D70
//1.14d: 0x004FD970
int32_t __stdcall TIMER_HandleVirtualKeyInput(SMSGHANDLER_PARAMS* pMsg);
