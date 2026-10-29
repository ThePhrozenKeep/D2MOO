#include "D2WinTimer.h"

#include <Fog.h>
#include <D2BitManip.h>

#include "D2WinMain.h"

#pragma warning (disable : 28159)


//1.10f: D2Win.0x6F96A8E0
//1.14d: 0x00841B74
uint32_t gdwNextRepeatTickCount;


//D2Win.0x6F8B1B70
//1.14d: 0x004FD9D0
D2WinTimerStrc* __fastcall TIMER_Create(uint32_t nTimeout, int32_t(__stdcall* pfCallback)(SMSGHANDLER_PARAMS*))
{
	D2WinTimerStrc* pTimer = D2_CALLOC_STRC(D2WinTimerStrc);

	pTimer->controlHeader.nType = D2WIN_TIMER;
	pTimer->nTimeout = nTimeout;
	pTimer->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pTimer->dwTickCount = GetTickCount();
	pTimer->controlHeader.pfUpdate = TIMER_UpdateDefault;
	pTimer->controlHeader.pfCallback = pfCallback;
	pTimer->controlHeader.pfHandleVirtualKeyInput = TIMER_HandleVirtualKeyInput;

	CONTROL_AppendToList(&pTimer->controlHeader);

	return pTimer;
}

//D2Win.0x6F8B1BF0 (#10181)
//1.14d: 0x004FD820
int32_t __fastcall TIMER_Destroy(D2WinControlStrc* pControl)
{
	return CONTROL_RemoveFromList(pControl);
}

//D2Win.0x6F8B1C00 (#10180)
//1.14d: 0x004FD830
void __fastcall TIMER_DisableSpaceKey(D2WinTimerStrc* pTimer)
{
	D2_ASSERT(pTimer->controlHeader.nType == D2WIN_TIMER);
	pTimer->controlHeader.pfHandleVirtualKeyInput = nullptr;
}

//D2Win.0x6F8B1C30 (#10182)
//1.14d: 0x004FDA50
void __fastcall TIMER_SetRepeating(D2WinTimerStrc* pTimer)
{
	D2_ASSERT(pTimer->controlHeader.nType == D2WIN_TIMER);
	pTimer->controlHeader.pfUpdate = TIMER_UpdateRepeating;
}

//D2Win.0x6F8B1C60
//1.14d: 0x004FD860
int32_t __fastcall TIMER_UpdateDefault(D2WinControlStrc* pControl)
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

	if (pTimer->controlHeader.pfCallback)
	{
		pTimer->controlHeader.pfCallback(&msg);
	}

	return 1;
}

//D2Win.0x6F8B1CF0
//1.14d: 0x004FD8F0
int32_t __fastcall TIMER_UpdateRepeating(D2WinControlStrc* pControl)
{
	D2WinTimerStrc* pTimer = (D2WinTimerStrc*)pControl;

	D2_ASSERT(pTimer->controlHeader.nType == D2WIN_TIMER);

	if (!(pTimer->controlHeader.dwFlags & gdwBitMasks[2]) || !(pTimer->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	const uint32_t dwTickCount = GetTickCount();
	if (dwTickCount < gdwNextRepeatTickCount)
	{
		return 1;
	}

	SMSGHANDLER_PARAMS msg = {};
	msg.hWindow = (HWND)pTimer;

	if (pTimer->controlHeader.pfCallback)
	{
		pTimer->controlHeader.pfCallback(&msg);
	}

	gdwNextRepeatTickCount = dwTickCount + pTimer->nTimeout;
	return 1;
}

//D2Win.0x6F8B1D70
//1.14d: 0x004FD970
int32_t __stdcall TIMER_HandleVirtualKeyInput(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinTimerStrc* pTimer = (D2WinTimerStrc*)pMsg->hWindow;

	D2_ASSERT(pTimer->controlHeader.nType == D2WIN_TIMER);

	if (pMsg->nMessage == WM_CHAR || pMsg->wParam != VK_SPACE)
	{
		return 0;
	}

	if (pTimer->controlHeader.pfCallback)
	{
		pTimer->controlHeader.pfCallback(pMsg);
	}

	return 1;
}
