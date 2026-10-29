#include "D2WinSmack.h"

#include <algorithm>

#include <D2Gfx.h>
#include <Fog.h>

#include <D2BitManip.h>

// D2Gfx
#include <Window.h>

#include "D2WinMain.h"


//1.10f: D2Win.0x6F96A7D0
//1.14d: 0x0087E860
char gszSmackFileName[260];
//1.10f: D2Win.0x6F96A8D4
//1.14d: 0x0087E964
int gnSmackVersion;
//1.10f: D2Win.0x6F96A8D8
//1.14d: 0x0087E968
int32_t dword_6F96A8D8;
//1.10f: D2Win.0x6F96A8DC
//1.14d: 0x00841B70
int32_t dword_6F96A8DC;


//D2Win.0x6F8AF710 (#10097)
//1.14d: 0x00506B70
D2WinSmackStrc* __fastcall SMACK_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t(__stdcall* pfCallback)(SMSGHANDLER_PARAMS*))
{
	D2WinSmackStrc* pSmack = D2_CALLOC_STRC(D2WinSmackStrc);

	pSmack->controlHeader.nType = D2WIN_SMACK;
	pSmack->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pSmack->controlHeader.pfCallback = pfCallback;
	pSmack->controlHeader.nImageX = nX;
	pSmack->controlHeader.nImageY = nY;
	pSmack->controlHeader.nWidth = nWidth;
	pSmack->controlHeader.nHeight = nHeight;
	pSmack->controlHeader.pfDraw = SMACK_Draw;
	pSmack->controlHeader.pfHandleMouseDown = SMACK_HandleMouseDown;
	pSmack->nBannerX = nX + (nWidth - 468) / 2;
	pSmack->nBannerY = nY + (nHeight - 60) / 2;

	CONTROL_AppendToList(&pSmack->controlHeader);

	return pSmack;
}

//D2Win.0x6F8AF7E0 (#10099)
int32_t __stdcall SMACK_Decode(D2WinSmackStrc* pSmack, const char* szFileName, int32_t nVersion, int32_t nUnused, int32_t nUnused2)
{
	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	strncpy_s(pSmack->szFileName, szFileName, std::size(pSmack->szFileName));
	WINDOW_CloseSmacker(&pSmack->smackerContext);
	WINDOW_DecodeSmacker(pSmack->szFileName, (uint8_t*)&pSmack->smackerContext, nVersion);
	strncpy_s(gszSmackFileName, szFileName, std::size(gszSmackFileName));
	gnSmackVersion = nVersion;
	return 1;
}

//D2Win.0x6F8AF860 (#10101)
//1.14d: 0x00506990
int32_t __stdcall SMACK_DeleteFile(D2WinSmackStrc* pSmack)
{
	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	WINDOW_CloseSmacker(&pSmack->smackerContext);
	if (!dword_6F96A8D8)
	{
		DeleteFileA(pSmack->szFileName);
	}

	return 1;
}

//D2Win.0x6F8AF8B0 (#10100)
//1.14d: 0x005069F0
int32_t __stdcall SMACK_SetCoordinates(D2WinSmackStrc* pSmack, int32_t nX, int32_t nY)
{
	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	pSmack->controlHeader.nImageX = nX;
	pSmack->controlHeader.nImageY = nY;
	pSmack->nBannerX = nX;
	pSmack->nBannerY = nY;
	return 1;
}

//D2Win.0x6F8AF900 (#10098)
//1.14d: 0x00506A40
int32_t __fastcall SMACK_Destroy(D2WinSmackStrc* pSmack)
{
	SMACK_DeleteFile(pSmack);

	return CONTROL_RemoveFromList(&pSmack->controlHeader);
}

//D2Win.0x6F8AF950 (#10184)
//1.14d: 0x00506A80
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

	WINDOW_PlaySmacker(&pSmack->smackerContext);
	return 1;
}

//D2Win.0x6F8AFA40
//1.14d: 0x00506B10
int32_t __stdcall SMACK_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinSmackStrc* pSmack = (D2WinSmackStrc*)pMsg->hWindow;

	D2_ASSERT(pSmack->controlHeader.nType == D2WIN_SMACK);

	if (!(pSmack->controlHeader.dwFlags & gdwBitMasks[2]) || !(pSmack->controlHeader.dwFlags & gdwBitMasks[0]) || !pSmack->controlHeader.pfCallback)
	{
		return 0;
	}

	return pSmack->controlHeader.pfCallback(pMsg);
}

//D2Win.0x6F8AFA90 (#10208)
void __fastcall D2Win_10208(int32_t a1)
{
	dword_6F96A8DC = a1;
}
