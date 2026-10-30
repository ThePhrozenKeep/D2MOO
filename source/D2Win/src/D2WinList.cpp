#include "D2WinList.h"

#include <algorithm>

#include <D2Math.h>

#include <D2CMP.h>
#include <D2Lang.h>

#include <D2Gfx.h>
#include <Texture.h>

#include <Fog.h>
#include <D2BitManip.h>

#include "D2WinArchive.h"
#include "D2WinFont.h"
#include "D2WinMain.h"


#pragma warning (disable : 28159)


//1.10f: D2Win.0x6F8FE21C
//1.14d: 0x0087E96C
D2CellFileStrc* ghPentagramCellFile;
//1.10f: D2Win.0x6F8FE220
//1.14d: 0x0087E970
uint32_t dword_6F8FE220;


//D2Win.0x6F8AC270 (#10135)
//1.14d: 0x005075E0
D2WinListStrc* __fastcall LIST_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, void* a5, Font* pFont)
{
	// TODO: a5, pFont, nWidtha, v24
	constexpr int32_t nFontHeight[] =
	{
		8, 16, 30, 42, 0, 0, 0, 0, 0, 0, 0
	};

	D2WinListStrc* pList = D2_CALLOC_STRC(D2WinListStrc);

	pList->controlHeader.nType = D2WIN_LIST;
	pList->controlHeader.nImageX = nY;
	pList->controlHeader.nImageY = nY;
	pList->controlHeader.nWidth = nWidth;
	pList->controlHeader.nHeight = nHeight;
	pList->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pList->controlHeader.pfDraw = LIST_Draw;
	pList->controlHeader.pfHandleMouseDown = LIST_HandleMouseDown;
	pList->controlHeader.pfHandleCharInput = LIST_HandleCharInput;
	pList->controlHeader.pfHandleVirtualKeyInput = LIST_HandleVirtualKeyInput;
	pList->eFont = *pFont;
	pList->nLineHeight = nFontHeight[pList->eFont];
	pList->nLinePadding = nFontHeight[pList->eFont] >> 2;
	pList->nSelectedIndex = -1;
	pList->nLastVisibleOffset = nHeight / (pList->nLineHeight + 2 * pList->nLinePadding) - 1;

	D2Win_10127_SetFont(pList->eFont);

	if (*(DWORD*)a5)
	{
		char* nWidtha = (char*)a5 + 4;
		char* v24 = nullptr;
		do
		{
			D2WinListDataStrc* pData = D2_ALLOC_STRC(D2WinListDataStrc);
			const Unicode* pText = D2LANG_GetStringFromTblIndex(*((WORD*)nWidtha - 2));

			D2_ASSERT(Unicode::strlen(pText) < std::size(pData->wszText));

			Unicode::strcpy(pData->wszText, pText);

			int32_t nTextWidth = 0;
			int32_t nTextHeight = 0;
			D2Win_10131_GetTextDimensions(pText, &nTextWidth, &nTextHeight);

			nTextWidth += 8;
			if (pList->nWidestEntry < nTextWidth)
			{
				pList->nWidestEntry = nTextWidth;
			}

			v24 = nWidtha;
			pData->nTextWidth = nTextWidth;
			pData->nHitRectLeft = 0;
			pData->pfOnActivate = (int32_t(__stdcall*)(SMSGHANDLER_PARAMS*))*((DWORD*)nWidtha + 2);
			pData->nHitRectRight = 0;
			pData->nHitRectTop = 0;
			pData->nHitRectBottom = 0;
			pData->nHotKey = *nWidtha;
			pData->nVirtualKey = *((DWORD*)nWidtha + 1);
			pData->pNext = nullptr;

			if (pList->pDataList)
			{
				D2WinListDataStrc* pLast = pList->pDataList;
				for (D2WinListDataStrc* i = pLast->pNext; i; i = i->pNext)
				{
					pLast = i;
				}
				pLast->pNext = pData;
			}
			else
			{
				pList->pDataList = pData;
			}

			nWidtha += 16;

			++pList->nEntryCount;
		}
		while (*((DWORD *)v24 + 3));
	}

	D2Win_10127_SetFont(D2FONT_FONT16);

	pList->pSelectedDataEntry = pList->pDataList;

	CONTROL_AppendToList(&pList->controlHeader);

	return pList;
}

//D2Win.0x6F8AC490 (#10136)
//1.14d: 0x00506E60
int32_t __fastcall LIST_Destroy(D2WinListStrc* pList)
{
	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	D2WinListDataStrc* pNext = nullptr;
	for (D2WinListDataStrc* pData = pList->pDataList; pData; pData = pNext)
	{
		pNext = pData->pNext;
		D2_FREE(pData);
	}

	return CONTROL_RemoveFromList(&pList->controlHeader);
}

//D2Win.0x6F8AC4F0 (#10138)
//1.14d: 0x005077F0
void __fastcall D2Win_10138(D2WinListStrc* pList, const char* szText, int32_t(__stdcall* a3)(SMSGHANDLER_PARAMS*), char a4, int32_t a5, int32_t a6, int32_t a7)
{
	Unicode wszText[256] = {};

	Unicode::win2Unicode(wszText, szText, std::size(wszText));
	D2Win_10137(pList, wszText, a3, a4, a5, a6, a7);
}

//D2Win.0x6F8AC570 (#10137)
//1.14d: 0x00506EC0
void __fastcall D2Win_10137(D2WinListStrc* pList, Unicode* wszText, int32_t(__stdcall* a3)(SMSGHANDLER_PARAMS*), char a4, int32_t a5, int32_t a6, int32_t a7)
{
	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	D2Win_10127_SetFont(pList->eFont);

	D2WinListDataStrc* pNewData = D2_ALLOC_STRC(D2WinListDataStrc);
	D2_ASSERT(Unicode::strlen(wszText) < 256);

	Unicode::strcpy(pNewData->wszText, wszText);

	int32_t nTextWidth = 0;
	int32_t nTextHeight = 0;
	D2Win_10131_GetTextDimensions(wszText, &nTextWidth, &nTextHeight);

	nTextWidth += 8;

	pList->nWidestEntry = std::max(pList->nWidestEntry, nTextWidth);

	pNewData->nTextWidth = nTextWidth;
	pNewData->nVirtualKey = a5;
	pNewData->pfOnActivate = a3;
	pNewData->nHitRectLeft = 0;
	pNewData->nHitRectRight = 0;
	pNewData->nHitRectTop = 0;
	pNewData->nHitRectBottom = 0;
	pNewData->nHotKey = a4;

	if (a6)
	{
		pNewData->pNext = pList->pDataList;
		pList->pDataList = pNewData;
	}
	else
	{
		D2WinListDataStrc* pLast = pList->pDataList;
		if (pLast)
		{
			for (D2WinListDataStrc* i = pLast->pNext; i; i = i->pNext)
			{
				pLast = i;
			}

			pLast->pNext = pNewData;
		}
		else
		{
			pList->pDataList = pNewData;
		}

		pNewData->pNext = nullptr;
	}

	++pList->nEntryCount;

	D2Win_10127_SetFont(D2FONT_FONT16);

	if (a7)
	{
		pList->pSelectedDataEntry = pNewData;
		pList->nSelectedIndex = pList->nEntryCount - 1;
	}
}

//D2Win.0x6F8AC6D0 (#10139)
//1.14d: 0x00507000
int32_t __fastcall LIST_GetSelectedDataIndex(D2WinListStrc* pList)
{
	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	int32_t nIndex = 0;
	for (D2WinListDataStrc* pData = pList->pDataList; pData; pData = pData->pNext)
	{
		if (pList->pSelectedDataEntry == pData)
		{
			return nIndex;
		}
		
		++nIndex;
	}

	return -1;
}

//D2Win.0x6F8AC720 (#10140)
//1.14d: 0x00507050
D2WinListDataStrc* __fastcall LIST_GetDataFromIndex(D2WinListStrc* pList, int32_t nIndex)
{
	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	int32_t nCounter = 0;
	for (D2WinListDataStrc* pData = pList->pDataList; pData; pData = pData->pNext)
	{
		if (nCounter == nIndex)
		{
			return pData;
		}

		++nCounter;
	}

	return nullptr;
}

//D2Win.0x6F8AC770 (#10141)
//1.14d: 0x005070A0
int32_t __fastcall D2Win_10141(D2WinListStrc* pList)
{
	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	return pList->nEntryCount;
}

//D2Win.0x6F8AC7A0
//1.14d: 0x005070D0
void __stdcall LIST_LoadPentspinCellfile()
{
	D2_ASSERT(ghPentagramCellFile == nullptr);

	char szFile[260] = {};
	wsprintfA(szFile, "%s\\UI\\CURSOR\\Pentspin", "DATA\\GLOBAL");
	ghPentagramCellFile = ARCHIVE_LoadCellFile(szFile, 0);
}

//D2Win.0x6F8AC800
//1.14d: 0x00507140
void __stdcall LIST_UnloadPentspinCellfile()
{
	ARCHIVE_FreeCellFile(ghPentagramCellFile);
	ghPentagramCellFile = nullptr;
}

//D2Win.0x6F8AC820
//1.14d: 0x00507160
int32_t __fastcall LIST_Draw(D2WinControlStrc* pControl)
{
	D2WinListStrc* pList = (D2WinListStrc*)pControl;

	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	D2Win_10127_SetFont(pList->eFont);

	D2WinListDataStrc* pData = pList->pDataList;
	for (int32_t i = 0; i < pList->nFirstVisibleEntry; ++i)
	{
		if (pData)
		{
			pData = pData->pNext;
		}
	}

	D2GfxDataStrc gfxData = {};
	gfxData.pCellFile = ghPentagramCellFile;
	gfxData.nDirection = 0;
	gfxData.nFrame = pList->nPentagramFrame;

	int32_t nTextY = pList->controlHeader.nImageY + pList->nLineHeight + pList->nLinePadding - pList->controlHeader.nHeight;

	while (pData)
	{
		if (nTextY >= pList->controlHeader.nImageY)
		{
			break;
		}

		const int32_t nTextX = pList->controlHeader.nImageX + ((pList->controlHeader.nWidth - pData->nTextWidth) >> 1);

		if (pData == pList->pSelectedDataEntry)
		{
			int32_t nImageY = nTextY + 3;
			if (pList->eFont == D2FONT_FONT30)
			{
				nImageY += 6;
			}

			const int32_t nOffsetX = (pList->controlHeader.nWidth - pList->nWidestEntry) >> 2;
			const int32_t nImageX1 = pList->controlHeader.nImageX + nOffsetX - 25;
			const int32_t nImageX2 = pList->controlHeader.nImageX - nOffsetX + pList->controlHeader.nWidth - 25;
			TEXTURE_CelDraw(&gfxData, nImageX1, nImageY, -1, DRAWMODE_NORMAL, 0);
			TEXTURE_CelDraw(&gfxData, nImageX2, nImageY, -1, DRAWMODE_NORMAL, 0);
		}

		D2Win_10117_DrawText(pData->wszText, nTextX, nTextY, 0, 0);

		pData->nHitRectLeft = pList->controlHeader.nImageX;
		pData->nHitRectRight = pList->controlHeader.nImageX + pList->controlHeader.nWidth;
		pData->nHitRectTop = nTextY - pList->nLinePadding - pList->nLineHeight;
		pData->nHitRectBottom = nTextY + pList->nLinePadding;

		nTextY += pList->nLineHeight + 2 * pList->nLinePadding;

		pData = pData->pNext;
	}

	++pList->nPentagramFrame;
	if (pList->nPentagramFrame >= 8)
	{
		pList->nPentagramFrame = 0;
	}

	D2Win_10127_SetFont(D2FONT_FONT16);

	return 1;
}

//D2Win.0x6F8AC9B0
//1.14d: 0x005072F0
int32_t __stdcall LIST_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinListStrc* pList = (D2WinListStrc*)pMsg->hWindow;

	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	const uint32_t dwTickCount = GetTickCount();

	for (D2WinListDataStrc* pData = pList->pDataList; pData; pData = pData->pNext)
	{
		if (gMousePosition_6F8FE234.x > pData->nHitRectLeft && gMousePosition_6F8FE234.x < pData->nHitRectRight && gMousePosition_6F8FE234.y > pData->nHitRectTop && gMousePosition_6F8FE234.y < pData->nHitRectBottom)
		{
			if (pData != pList->pSelectedDataEntry || dwTickCount - dword_6F8FE220 >= 300)
			{
				pList->pSelectedDataEntry = pData;
			}
			else if (pData->pfOnActivate)
			{
				return pData->pfOnActivate(pMsg);
			}
		}
	}

	dword_6F8FE220 = dwTickCount;

	if (pList->controlHeader.pfCallback)
	{
		return pList->controlHeader.pfCallback(pMsg);
	}

	return 0;
}

//D2Win.0x6F8ACA70
//1.14d: 0x005073B0
int32_t __stdcall LIST_HandleCharInput(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinListStrc* pList = (D2WinListStrc*)pMsg->hWindow;

	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	for (D2WinListDataStrc* pData = pList->pDataList; pData; pData = pData->pNext)
	{
		if (pData->nHotKey == pMsg->wParam && pData->pfOnActivate)
		{
			return pData->pfOnActivate(pMsg);
		}
	}

	return 0;
}

//D2Win.0x6F8ACAE0
//1.14d: 0x00507420
int32_t __stdcall LIST_HandleVirtualKeyInput(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinListStrc* pList = (D2WinListStrc*)pMsg->hWindow;

	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	if (pMsg->nMessage == WM_CHAR)
	{
		return 0;
	}

	if (pMsg->wParam == VK_RETURN)
	{
		CONTROL_SetFocusedEditBox(nullptr);

		if (pList->pSelectedDataEntry->pfOnActivate)
		{
			return pList->pSelectedDataEntry->pfOnActivate(pMsg);
		}
	}
	else if (pMsg->wParam == VK_UP)
	{	
		D2WinListDataStrc* pTemp = pList->pDataList;
		
		if (pList->pDataList == pList->pSelectedDataEntry)
		{
			for (D2WinListDataStrc* pData = pList->pDataList->pNext; pData; pData = pData->pNext)
			{
				pTemp = pData;
			}
		}
		else
		{
			for (D2WinListDataStrc* pData = pList->pDataList->pNext; pData != pList->pSelectedDataEntry; pData = pData->pNext)
			{
				pTemp = pData;
			}
		}

		pList->pSelectedDataEntry = pTemp;

		pList->nSelectedIndex = 0;
		for (D2WinListDataStrc* pData = pList->pDataList; pData != pTemp; pData = pData->pNext)
		{
			++pList->nSelectedIndex;
		}

		pList->nFirstVisibleEntry = D2Clamp(pList->nFirstVisibleEntry, pList->nSelectedIndex - pList->nLastVisibleOffset, pList->nSelectedIndex);

		CONTROL_SetFocusedEditBox(nullptr);
		return 1;
	}
	else if (pMsg->wParam == VK_DOWN)
	{
		pList->pSelectedDataEntry = pList->pSelectedDataEntry->pNext;
		if (!pList->pSelectedDataEntry)
		{
			pList->pSelectedDataEntry = pList->pDataList;
		}

		pList->nSelectedIndex = 0;
		for (D2WinListDataStrc* pData = pList->pDataList; pData != pList->pSelectedDataEntry; pData = pData->pNext)
		{
			++pList->nSelectedIndex;
		}

		pList->nFirstVisibleEntry = D2Clamp(pList->nFirstVisibleEntry, pList->nSelectedIndex - pList->nLastVisibleOffset, pList->nSelectedIndex);

		CONTROL_SetFocusedEditBox(nullptr);
		return 1;
	}

	for (D2WinListDataStrc* pData = pList->pDataList; pData; pData = pData->pNext)
	{
		if (pData->nVirtualKey == pMsg->wParam && pData->pfOnActivate)
		{
			return pData->pfOnActivate(pMsg);
		}
	}

	return 0;
}
