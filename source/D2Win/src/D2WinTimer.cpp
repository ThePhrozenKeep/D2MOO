#include "D2WinTimer.h"

#include <Fog.h>
#include <D2BitManip.h>

#include "D2WinMain.h"

#pragma warning (disable : 28159)


uint32_t gdwTimeoutTickCount;


//D2Win.0x6F8B1B70
D2WinTimerStrc* __fastcall TIMER_Create(uint32_t nTimeout, int32_t(__stdcall* a2)(SMSGHANDLER_PARAMS*))
{
	D2WinTimerStrc* pTimer = D2_CALLOC_STRC(D2WinTimerStrc);

	pTimer->controlHeader.nType = D2WIN_TIMER;
	pTimer->nTimeout = nTimeout;
	pTimer->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pTimer->dwTickCount = GetTickCount();
	pTimer->controlHeader.pfInitialize = sub_6F8B1C60;
	pTimer->controlHeader.field_34 = a2;
	pTimer->controlHeader.pfHandleVirtualKeyInput = TIMER_HandleVirtualKeyInput;

	CONTROL_AppendToList(&pTimer->controlHeader);

	return pTimer;
}

//D2Win.0x6F8B1BF0 (#10181)
int32_t __fastcall TIMER_Destroy(D2WinControlStrc* pControl)
{
	return CONTROL_RemoveFromList(pControl);
}

//D2Win.0x6F8B1C00 (#10180)
void __fastcall D2Win_10180(D2WinTimerStrc* pTimer)
{
	D2_ASSERT(pTimer->controlHeader.nType == D2WIN_TIMER);
	pTimer->controlHeader.pfHandleVirtualKeyInput = nullptr;
}

//D2Win.0x6F8B1C30 (#10182)
void __fastcall D2Win_10182(D2WinTimerStrc* pTimer)
{
	D2_ASSERT(pTimer->controlHeader.nType == D2WIN_TIMER);
	pTimer->controlHeader.pfInitialize = sub_6F8B1CF0;
}

//D2Win.0x6F8B1C60
int32_t __fastcall sub_6F8B1C60(D2WinControlStrc* pControl)
{
	D2WinTimerStrc* pTimer = (D2WinTimerStrc*)pControl;

	D2_ASSERT(pTimer->controlHeader.nType == D2WIN_TIMER);

	if (!(pTimer->controlHeader.dwFlags & gdwBitMasks[2]) || !(pTimer->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	if (GetTickCount() / 1000 - pTimer->dwTickCount / 1000 <= pTimer->nTimeout)
	{
		return 1;
	}

	SMSGHANDLER_PARAMS msg = {};
	msg.hWindow = (HWND)pTimer;

	if (pTimer->controlHeader.field_34)
	{
		pTimer->controlHeader.field_34(&msg);
	}

	return 1;
}

//D2Win.0x6F8B1CF0
int32_t __fastcall sub_6F8B1CF0(D2WinControlStrc* pControl)
{
	D2WinTimerStrc* pTimer = (D2WinTimerStrc*)pControl;

	D2_ASSERT(pTimer->controlHeader.nType == D2WIN_TIMER);

	if (!(pTimer->controlHeader.dwFlags & gdwBitMasks[2]) || !(pTimer->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	const uint32_t dwTickCount = GetTickCount();
	if (dwTickCount < gdwTimeoutTickCount)
	{
		return 1;
	}

	SMSGHANDLER_PARAMS pMsg = {};
	pMsg.hWindow = (HWND)pTimer;

	if (pTimer->controlHeader.field_34)
	{
		pTimer->controlHeader.field_34(&pMsg);
	}

	gdwTimeoutTickCount = dwTickCount + pTimer->nTimeout;
	return 1;
}

//D2Win.0x6F8B1D70
int32_t __stdcall TIMER_HandleVirtualKeyInput(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinTimerStrc* pTimer = (D2WinTimerStrc*)pMsg->hWindow;

	D2_ASSERT(pTimer->controlHeader.nType == D2WIN_TIMER);

	if (pMsg->nMessage == WM_CHAR || pMsg->wParam != VK_SPACE)
	{
		return 0;
	}

	if (pTimer->controlHeader.field_34)
	{
		pTimer->controlHeader.field_34(pMsg);
	}

	return 1;
}
