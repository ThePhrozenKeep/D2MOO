#include "D2WinAccountList.h"

#include <algorithm>

#include <D2Gfx.h>
#include <D2Lang.h>

#include <Fog.h>
#include <D2BitManip.h>

#include "D2WinFont.h"
#include "D2WinMain.h"


//D2Win.0x6F8A4E90 (#10162)
D2WinAccountListStrc* __fastcall ACCOUNTLIST_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t a5, void* a6)
{
	D2WinAccountListStrc* pAccountList = D2_CALLOC_STRC(D2WinAccountListStrc);

	pAccountList->controlHeader.nType = D2WIN_ACCOUNTLIST;
	pAccountList->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pAccountList->controlHeader.nImageX = nX;
	pAccountList->controlHeader.nImageY = nY;
	pAccountList->controlHeader.nWidth = nWidth;
	pAccountList->controlHeader.nHeight = nHeight;
	pAccountList->controlHeader.pfDraw = ACCOUNTLIST_Draw;
	pAccountList->controlHeader.pfHandleMouseDown = ACCOUNTLIST_HandleMouseDown;
	pAccountList->field_4C = a5;

	if (a6)
	{
		pAccountList->eFont = *(Font*)a6;
		pAccountList->field_54 = *((DWORD*)a6 + 1);
		CONTROL_AppendToList(&pAccountList->controlHeader);
	}
	else
	{
		pAccountList->eFont = D2FONT_FONT16;
		pAccountList->field_54 = 0;
		CONTROL_AppendToList(&pAccountList->controlHeader);
	}

	return pAccountList;
}

//D2Win.0x6F8A4F70 (#10163)
int32_t __fastcall ACCOUNTLIST_Destroy(D2WinAccountListStrc* pAccountList)
{
	D2_ASSERT(pAccountList->controlHeader.nType == D2WIN_ACCOUNTLIST);

	ACCOUNTLIST_FreeData(pAccountList);

	return CONTROL_RemoveFromList(&pAccountList->controlHeader);
}

//D2Win.0x6F8A4FC0 (#10164)
void __fastcall ACCOUNTLIST_FreeData(D2WinAccountListStrc* pAccountList)
{
	D2_ASSERT(pAccountList->controlHeader.nType == D2WIN_ACCOUNTLIST);

	D2WinAccountListDataStrc* pNext = nullptr;
	for (D2WinAccountListDataStrc* pData = pAccountList->pDataList; pData; pData = pNext)
	{
		pNext = pData->pNext;
		D2_FREE(pData);
	}
}

//D2Win.0x6F8A5010 (#10165)
void __fastcall ACCOUNTLIST_AllocData(D2WinAccountListStrc* pAccountList, const char* szText)
{
	D2_ASSERT(pAccountList->controlHeader.nType == D2WIN_ACCOUNTLIST);

	D2WinAccountListDataStrc* pData = D2_ALLOC_STRC(D2WinAccountListDataStrc);
	strncpy_s(pData->szText, szText, std::size(pData->szText));

	D2WinAccountListDataStrc* pList = pAccountList->pDataList;
	if (pList)
	{
		pData->pNext = pList;
		pAccountList->pDataList = pData;
	}
	else
	{
		pData->pNext = nullptr;
		pAccountList->pDataList = pData;
	}
}

//D2Win.0x6F8A5080 (#10166)
int32_t __fastcall ACCOUNTLIST_RemoveEntry(D2WinAccountListStrc* pAccountList, const char* szText)
{
	D2_ASSERT(pAccountList->controlHeader.nType == D2WIN_ACCOUNTLIST);

	D2WinAccountListDataStrc* pPrevious = nullptr;
	for (D2WinAccountListDataStrc* pData = pAccountList->pDataList; pData; pData = pData->pNext)
	{
		if (!strcmp(pData->szText, szText))
		{
			if (pAccountList->pSelectedDataEntry == pData)
			{
				pAccountList->pSelectedDataEntry = nullptr;
			}

			if (pPrevious)
			{
				pPrevious->pNext = pData->pNext;
			}
			else
			{
				pAccountList->pDataList = pData->pNext;
			}

			D2_FREE(pData);
			return 1;
		}

		pPrevious = pData;
	}

	return 0;
}

//D2Win.0x6F8A5140 (#10167)
int32_t __fastcall ACCOUNTLIST_GetTextFromSelectedEntry(D2WinAccountListStrc* pAccountList, char* szBuffer)
{
	D2_ASSERT(pAccountList->controlHeader.nType == D2WIN_ACCOUNTLIST);

	const char* szText = pAccountList->pSelectedDataEntry->szText;
	if (!szText)
	{
		return 0;
	}

	strcpy_s(szBuffer, std::size(pAccountList->pSelectedDataEntry->szText), szText);

	return 1;
}

//D2Win.0x6F8A5180
int32_t __fastcall ACCOUNTLIST_Draw(D2WinControlStrc* pControl)
{
	D2WinAccountListStrc* pAccountList = (D2WinAccountListStrc*)pControl;

	D2_ASSERT(pAccountList->controlHeader.nType == D2WIN_ACCOUNTLIST);

	if (!(pAccountList->controlHeader.dwFlags & gdwBitMasks[2]) || !(pAccountList->controlHeader.dwFlags & gdwBitMasks[0]) || !pAccountList->pDataList)
	{
		return 0;
	}

	D2Win_10127_SetFont(pAccountList->eFont);

	const int32_t nBaseX = pAccountList->controlHeader.nImageX;
	const int32_t nBaseY = pAccountList->controlHeader.nImageY - pAccountList->controlHeader.nHeight + D2Win_10125();

	int32_t nCounter = 0;
	for (D2WinAccountListDataStrc* pData = pAccountList->pDataList; pData; pData = pData->pNext)
	{
		if (nCounter >= pAccountList->field_48 && nCounter < pAccountList->field_48 + 24)
		{
			const int32_t nX = nBaseX + 120 * nCounter % 6;
			const int32_t nY = nBaseY + 20 * nCounter / 6;
			D2GFX_DrawBox(nX, nY - 14, nX + 28, nY, 0x55u, DRAWMODE_NORMAL);

			wchar_t wszText[256] = {};
			D2LANG_win2Unicode(wszText, pData->szText, std::size(wszText));
			D2Win_10117_DrawText(wszText, nX + 32, nY, 0, 0);

			if (pData == pAccountList->pSelectedDataEntry)
			{
				D2GFX_DrawBox(nX - 1, nY - 16, nX + 118, nY + 1, 0xFFu, DRAWMODE_TRANS50);
			}
		}

		++nCounter;
	}

	return 1;
}

//D2Win.0x6F8A52E0
int32_t __stdcall ACCOUNTLIST_HandleMouseDown(D2WinMsgStrc* pMsg)
{
	D2WinAccountListStrc* pAccountList = (D2WinAccountListStrc*)pMsg->hWnd;

	D2_ASSERT(pAccountList->controlHeader.nType == D2WIN_ACCOUNTLIST);

	if (!(pAccountList->controlHeader.dwFlags & gdwBitMasks[2]) || !(pAccountList->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	int32_t nCounter = 0;
	for (D2WinAccountListDataStrc* pData = pAccountList->pDataList; pData; pData = pData->pNext)
	{
		if (nCounter == (gMousePosition_6F8FE234.x - pAccountList->controlHeader.nImageX) / 120 + 6 * (pAccountList->controlHeader.nHeight - pAccountList->controlHeader.nImageY + gMousePosition_6F8FE234.y + 4) / 20)
		{
			pAccountList->pSelectedDataEntry = pData;
			return 1;
		}

		++nCounter;
	}

	pAccountList->pSelectedDataEntry = nullptr;
	return 1;
}
