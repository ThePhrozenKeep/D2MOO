#include "D2WinSmack.h"

#include <algorithm>

#include <D2Gfx.h>
#include <Fog.h>

#include <D2BitManip.h>

// D2Gfx
#include <Window.h>

#include "D2WinMain.h"


char gszSmackFileName[260];
int gnSmackVersion;
int32_t dword_6F96A8D8;
int32_t dword_6F96A8DC;


//D2Win.0x6F8AF710 (#10097)
D2WinSmackStrc* __fastcall SMACK_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t(__stdcall* a5)(D2WinMsgStrc*))
{
	D2WinSmackStrc* pSmack = D2_CALLOC_STRC(D2WinSmackStrc);

	pSmack->controlHeader.nType = D2WIN_SMACK;
	pSmack->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pSmack->controlHeader.field_34 = a5;
	pSmack->controlHeader.nImageX = nX;
	pSmack->controlHeader.nImageY = nY;
	pSmack->controlHeader.nWidth = nWidth;
	pSmack->controlHeader.nHeight = nHeight;
	pSmack->controlHeader.pfDraw = SMACK_Draw;
	pSmack->controlHeader.pfHandleMouseDown = SMACK_HandleMouseDown;
	pSmack->field_148 = nX + (nWidth - 468) / 2;
	pSmack->field_14C = nY + (nHeight - 60) / 2;

	CONTROL_AppendToList(&pSmack->controlHeader);

	return pSmack;
}

//D2Win.0x6F8AF7E0 (#10099)
int32_t __stdcall SMACK_Decode(D2WinSmackStrc* pSmack, const char* szFileName, int32_t nVersion, int32_t nUnused, int32_t nUnused2)
{
	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	strncpy_s(pSmack->szFileName, szFileName, std::size(pSmack->szFileName));
	WINDOW_CloseSmacker(&pSmack->field_140);
	WINDOW_DecodeSmacker(pSmack->szFileName, (uint8_t*)&pSmack->field_140, nVersion);
	strncpy_s(gszSmackFileName, szFileName, std::size(gszSmackFileName));
	gnSmackVersion = nVersion;
	return 1;
}

//D2Win.0x6F8AF860 (#10101)
int32_t __stdcall SMACK_DeleteFile(D2WinSmackStrc* pSmack)
{
	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	WINDOW_CloseSmacker(&pSmack->field_140);
	if (!dword_6F96A8D8)
	{
		DeleteFileA(pSmack->szFileName);
	}

	return 1;
}

//D2Win.0x6F8AF8B0 (#10100)
int32_t __stdcall SMACK_SetCoordinates(D2WinSmackStrc* pSmack, int32_t nX, int32_t nY)
{
	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	pSmack->controlHeader.nImageX = nX;
	pSmack->controlHeader.nImageY = nY;
	pSmack->field_148 = nX;
	pSmack->field_14C = nY;
	return 1;
}

//D2Win.0x6F8AF900 (#10098)
int32_t __fastcall SMACK_Destroy(D2WinSmackStrc* pSmack)
{
	SMACK_DeleteFile(pSmack);

	return CONTROL_RemoveFromList(&pSmack->controlHeader);
}

//D2Win.0x6F8AF950 (#10184)
void __stdcall D2Win_10184()
{
	dword_6F96A8D8 = 1;
}

//D2Win.0x6F8AF960
int32_t __fastcall SMACK_Draw(D2WinControlStrc* pControl)
{
	D2WinSmackStrc* pSmack = (D2WinSmackStrc*)pControl;

	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	if (!(pSmack->controlHeader.dwFlags & gdwBitMasks[2]) || !(pSmack->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	if (dword_6F96A8D8)
	{
		if (gszSmackFileName[0])
		{
			SMACK_Decode(pSmack, gszSmackFileName, gnSmackVersion, 0, 0);
		}

		dword_6F96A8D8 = 0;
	}

	WINDOW_PlaySmacker(&pSmack->field_140);
	return 1;
}

//D2Win.0x6F8AFA40
int32_t __stdcall SMACK_HandleMouseDown(D2WinMsgStrc* pMsg)
{
	D2WinSmackStrc* pSmack = (D2WinSmackStrc*)pMsg->hWnd;

	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	if (!(pSmack->controlHeader.dwFlags & gdwBitMasks[2]) || !(pSmack->controlHeader.dwFlags & gdwBitMasks[0]) || !pSmack->controlHeader.field_34)
	{
		return 0;
	}

	return pSmack->controlHeader.field_34(pMsg);
}

//D2Win.0x6F8AFA90 (#10208)
void __fastcall D2Win_10208(int32_t a1)
{
	dword_6F96A8DC = a1;
}
