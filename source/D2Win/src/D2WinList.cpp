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


D2CellFileStrc* ghPentagramCellFile;
uint32_t dword_6F8FE220;


//D2Win.0x6F8AC270 (#10135)
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
	pList->field_44 = nFontHeight[pList->eFont];
	pList->field_48 = nFontHeight[pList->eFont] >> 2;
	pList->field_54 = -1;
	pList->field_58 = nHeight / (pList->field_44 + 2 * pList->field_48) - 1;

	D2Win_10127_SetFont(pList->eFont);

	if (*(DWORD*)a5)
	{
		char* nWidtha = (char*)a5 + 4;
		char* v24 = nullptr;
		do
		{
			D2WinListDataStrc* pData = D2_ALLOC_STRC(D2WinListDataStrc);
			const wchar_t* pText = D2LANG_10004_GetStringFromTblIndex(*((WORD*)nWidtha - 2));

			D2_ASSERT(D2LANG_strlen(pText) < std::size(pData->wszText));

			D2LANG_strcpy(pData->wszText, pText);

			int32_t nTextWidth = 0;
			int32_t nTextHeight = 0;
			D2Win_10131_GetTextDimensions(pText, &nTextWidth, &nTextHeight);

			nTextWidth += 8;
			if (pList->field_60 < nTextWidth)
			{
				pList->field_60 = nTextWidth;
			}

			v24 = nWidtha;
			pData->field_200 = nTextWidth;
			pData->field_208 = 0;
			pData->field_204 = (int32_t(__stdcall*)(D2WinMsgStrc*))*((DWORD*)nWidtha + 2);
			pData->field_20C = 0;
			pData->field_210 = 0;
			pData->field_214 = 0;
			pData->field_218 = *nWidtha;
			pData->field_21C = *((DWORD*)nWidtha + 1);
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

			++pList->field_50;
		}
		while (*((DWORD *)v24 + 3));
	}

	D2Win_10127_SetFont(D2FONT_FONT16);

	pList->pSelectedDataEntry = pList->pDataList;

	CONTROL_AppendToList(&pList->controlHeader);

	return pList;
}

//D2Win.0x6F8AC490 (#10136)
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
void __fastcall D2Win_10138(D2WinListStrc* pList, const char* szText, int32_t(__stdcall* a3)(D2WinMsgStrc*), char a4, int32_t a5, int32_t a6, int32_t a7)
{
	wchar_t wszText[256] = {};

	D2LANG_win2Unicode(wszText, szText, std::size(wszText));
	D2Win_10137(pList, wszText, a3, a4, a5, a6, a7);
}

//D2Win.0x6F8AC570 (#10137)
void __fastcall D2Win_10137(D2WinListStrc* pList, wchar_t* wszText, int32_t(__stdcall* a3)(D2WinMsgStrc*), char a4, int32_t a5, int32_t a6, int32_t a7)
{
	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	D2Win_10127_SetFont(pList->eFont);

	D2WinListDataStrc* pNewData = D2_ALLOC_STRC(D2WinListDataStrc);
	D2_ASSERT(D2LANG_strlen(wszText) < 256);

	D2LANG_strcpy(pNewData->wszText, wszText);

	int32_t nTextWidth = 0;
	int32_t nTextHeight = 0;
	D2Win_10131_GetTextDimensions(wszText, &nTextWidth, &nTextHeight);

	nTextWidth += 8;

	pList->field_60 = std::max(pList->field_60, nTextWidth);

	pNewData->field_200 = nTextWidth;
	pNewData->field_21C = a5;
	pNewData->field_204 = a3;
	pNewData->field_208 = 0;
	pNewData->field_20C = 0;
	pNewData->field_210 = 0;
	pNewData->field_214 = 0;
	pNewData->field_218 = a4;

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

	++pList->field_50;

	D2Win_10127_SetFont(D2FONT_FONT16);

	if (a7)
	{
		pList->pSelectedDataEntry = pNewData;
		pList->field_54 = pList->field_50 - 1;
	}
}

//D2Win.0x6F8AC6D0 (#10139)
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
int32_t __fastcall D2Win_10141(D2WinListStrc* pList)
{
	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	return pList->field_50;
}

//D2Win.0x6F8AC7A0
void __stdcall LIST_LoadPentspinCellfile()
{
	D2_ASSERT(ghPentagramCellFile == nullptr);

	char szFile[260] = {};
	wsprintfA(szFile, "%s\\UI\\CURSOR\\Pentspin", "DATA\\GLOBAL");
	ghPentagramCellFile = ARCHIVE_LoadCellFile(szFile, 0);
}

//D2Win.0x6F8AC800
void __stdcall LIST_UnloadPentspinCellfile()
{
	ARCHIVE_FreeCellFile(ghPentagramCellFile);
	ghPentagramCellFile = nullptr;
}

//D2Win.0x6F8AC820
int32_t __fastcall LIST_Draw(D2WinControlStrc* pControl)
{
	D2WinListStrc* pList = (D2WinListStrc*)pControl;

	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	D2Win_10127_SetFont(pList->eFont);

	D2WinListDataStrc* pData = pList->pDataList;
	for (int32_t i = 0; i < pList->field_4C; ++i)
	{
		if (pData)
		{
			pData = pData->pNext;
		}
	}

	D2GfxDataStrc gfxData = {};
	gfxData.pCellFile = ghPentagramCellFile;
	gfxData.nDirection = 0;
	gfxData.nFrame = pList->field_5C;

	int32_t nTextY = pList->controlHeader.nImageY + pList->field_44 + pList->field_48 - pList->controlHeader.nHeight;

	while (pData)
	{
		if (nTextY >= pList->controlHeader.nImageY)
		{
			break;
		}

		const int32_t nTextX = pList->controlHeader.nImageX + ((pList->controlHeader.nWidth - pData->field_200) >> 1);

		if (pData == pList->pSelectedDataEntry)
		{
			int32_t nImageY = nTextY + 3;
			if (pList->eFont == D2FONT_FONT30)
			{
				nImageY += 6;
			}

			const int32_t nOffsetX = (pList->controlHeader.nWidth - pList->field_60) >> 2;
			const int32_t nImageX1 = pList->controlHeader.nImageX + nOffsetX - 25;
			const int32_t nImageX2 = pList->controlHeader.nImageX - nOffsetX + pList->controlHeader.nWidth - 25;
			TEXTURE_CelDraw(&gfxData, nImageX1, nImageY, -1, DRAWMODE_NORMAL, 0);
			TEXTURE_CelDraw(&gfxData, nImageX2, nImageY, -1, DRAWMODE_NORMAL, 0);
		}

		D2Win_10117_DrawText(pData->wszText, nTextX, nTextY, 0, 0);

		pData->field_208 = pList->controlHeader.nImageX;
		pData->field_20C = pList->controlHeader.nImageX + pList->controlHeader.nWidth;
		pData->field_210 = nTextY - pList->field_48 - pList->field_44;
		pData->field_214 = nTextY + pList->field_48;

		nTextY += pList->field_44 + 2 * pList->field_48;

		pData = pData->pNext;
	}

	++pList->field_5C;
	if (pList->field_5C >= 8)
	{
		pList->field_5C = 0;
	}

	D2Win_10127_SetFont(D2FONT_FONT16);

	return 1;
}

//D2Win.0x6F8AC9B0
int32_t __stdcall LIST_HandleMouseDown(D2WinMsgStrc* pMsg)
{
	D2WinListStrc* pList = (D2WinListStrc*)pMsg->hWnd;

	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	const uint32_t dwTickCount = GetTickCount();

	for (D2WinListDataStrc* pData = pList->pDataList; pData; pData = pData->pNext)
	{
		if (gMousePosition_6F8FE234.x > pData->field_208 && gMousePosition_6F8FE234.x < pData->field_20C && gMousePosition_6F8FE234.y > pData->field_210 && gMousePosition_6F8FE234.y < pData->field_214)
		{
			if (pData != pList->pSelectedDataEntry || dwTickCount - dword_6F8FE220 >= 300)
			{
				pList->pSelectedDataEntry = pData;
			}
			else if (pData->field_204)
			{
				return pData->field_204(pMsg);
			}
		}
	}

	dword_6F8FE220 = dwTickCount;

	if (pList->controlHeader.field_34)
	{
		return pList->controlHeader.field_34(pMsg);
	}

	return 0;
}

//D2Win.0x6F8ACA70
int32_t __stdcall LIST_HandleCharInput(D2WinMsgStrc* pMsg)
{
	D2WinListStrc* pList = (D2WinListStrc*)pMsg->hWnd;

	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	for (D2WinListDataStrc* pData = pList->pDataList; pData; pData = pData->pNext)
	{
		if (pData->field_218 == pMsg->wParam && pData->field_204)
		{
			return pData->field_204(pMsg);
		}
	}

	return 0;
}

//D2Win.0x6F8ACAE0
int32_t __stdcall LIST_HandleVirtualKeyInput(D2WinMsgStrc* pMsg)
{
	D2WinListStrc* pList = (D2WinListStrc*)pMsg->hWnd;

	D2_ASSERT(pList->controlHeader.nType == D2WIN_LIST);

	if (pMsg->uMessage == WM_CHAR)
	{
		return 0;
	}

	if (pMsg->wParam == VK_RETURN)
	{
		D2Win_10012(nullptr);

		if (pList->pSelectedDataEntry->field_204)
		{
			return pList->pSelectedDataEntry->field_204(pMsg);
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

		pList->field_54 = 0;
		for (D2WinListDataStrc* pData = pList->pDataList; pData != pTemp; pData = pData->pNext)
		{
			++pList->field_54;
		}

		pList->field_4C = D2Clamp(pList->field_4C, pList->field_54 - pList->field_58, pList->field_54);

		D2Win_10012(nullptr);
		return 1;
	}
	else if (pMsg->wParam == VK_DOWN)
	{
		pList->pSelectedDataEntry = pList->pSelectedDataEntry->pNext;
		if (!pList->pSelectedDataEntry)
		{
			pList->pSelectedDataEntry = pList->pDataList;
		}

		pList->field_54 = 0;
		for (D2WinListDataStrc* pData = pList->pDataList; pData != pList->pSelectedDataEntry; pData = pData->pNext)
		{
			++pList->field_54;
		}

		pList->field_4C = D2Clamp(pList->field_4C, pList->field_54 - pList->field_58, pList->field_54);

		D2Win_10012(nullptr);
		return 1;
	}

	for (D2WinListDataStrc* pData = pList->pDataList; pData; pData = pData->pNext)
	{
		if (pData->field_21C == pMsg->wParam && pData->field_204)
		{
			return pData->field_204(pMsg);
		}
	}

	return 0;
}
