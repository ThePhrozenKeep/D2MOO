#include "D2WinTextBox.h"

#include <algorithm>

#include <D2Math.h>

#include <D2CMP.h>
#include <D2Lang.h>
#include <D2StrTable.h>
#include <Storm.h>

#include <D2Gfx.h>
#include <Texture.h>

#include <Fog.h>
#include <D2BitManip.h>

#include "D2WinFont.h"
#include "D2WinMain.h"
#include "D2WinPalette.h"
#include "D2WinScrollBar.h"
#include "D2WinTimer.h"


int32_t dword_6F8BE238;
extern int32_t dword_6F96A8DC;


//D2Win.0x6F8AFAA0 (#10042)
D2WinTextBoxStrc* __fastcall D2Win_10042_TEXTBOX_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t a5, int32_t a6, D2CellFileStrc* pCellFile, int32_t(__stdcall* a8)(SMSGHANDLER_PARAMS*), uint32_t dwTextBoxFlags, int32_t a10, int32_t a11)
{
	D2WinTextBoxStrc* pTextBox = D2_CALLOC_STRC(D2WinTextBoxStrc);

	if (dwTextBoxFlags & 4 && !a11)
	{
		nWidth -= 26;
	}

	if (dwTextBoxFlags & 1)
	{
		dwTextBoxFlags |= 0x20;
	}

	pTextBox->controlHeader.nType = D2WIN_TEXTBOX;
	pTextBox->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pTextBox->controlHeader.pCellFile = pCellFile;
	pTextBox->controlHeader.nImageX = nX;
	pTextBox->controlHeader.nImageY = nY;
	pTextBox->controlHeader.nWidth = nWidth;
	pTextBox->controlHeader.nHeight = nHeight;
	pTextBox->dwTextBoxFlags = dwTextBoxFlags;
	pTextBox->field_40 = a5;
	pTextBox->field_44 = a6;
	pTextBox->controlHeader.pfDraw = TEXTBOX_Draw;
	pTextBox->controlHeader.pfHandleMouseDown = TEXTBOX_HandleMouseDown;

	if (dwTextBoxFlags & 1)
	{
		pTextBox->controlHeader.pfHandleVirtualKeyInput = TEXTBOX_HandleVirtualKeyInput;
	}

	pTextBox->controlHeader.field_34 = a8;
	pTextBox->nCurrentLine = -1;
	pTextBox->nNumColumns = 1;

	if (a10)
	{
		pTextBox->eFont = *(Font*)a10;
		pTextBox->field_98 = *(DWORD*)(a10 + 4);
	}
	else
	{
		pTextBox->eFont = D2FONT_FONT16;
		pTextBox->field_98 = 0;
	}

	sub_6F8AFCC0(pTextBox->eFont);

	pTextBox->field_A8 = dword_6F8BE238;

	if (dwTextBoxFlags & 4)
	{
		pTextBox->controlHeader.pCellFile = nullptr;

		if (a11)
		{
			pTextBox->pScrollBar = SCROLLBAR_Create(*(DWORD*)a11, *(DWORD*)(a11 + 4), 10, *(DWORD*)(a11 + 8), pCellFile, nullptr);
		}
		else
		{
			pTextBox->pScrollBar = SCROLLBAR_Create(nX + nWidth + 8, nY - 2, 10, nHeight - 16, pCellFile, nullptr);
		}

		SCROLLBAR_SetTextBox(pTextBox->pScrollBar, pTextBox);

		D2Win_10127_SetFont(pTextBox->eFont);
		sub_6F8AFCC0(pTextBox->eFont);

		const int32_t nTextHeight = D2Win_10125();
		const int32_t nPosition = std::max((pTextBox->controlHeader.nHeight - 2 * pTextBox->field_44 - (pTextBox->field_A8 + nTextHeight)) / (pTextBox->field_A8 + nTextHeight + pTextBox->field_A4), 1);

		D2Win_10202(pTextBox->pScrollBar, nPosition);
	}

	CONTROL_AppendToList(&pTextBox->controlHeader);

	return pTextBox;
}

//D2Win.0x6F8AFCC0
void __fastcall sub_6F8AFCC0(Font nFontSize)
{
	switch (STRTABLE_GetLanguage())
	{
	case LANGUAGE_JAPANESE:
	case LANGUAGE_POLISH:
	{
		dword_6F8BE238 = dword_6F96A8DC != 0 ? 0 : 4;
		break;
	}
	case LANGUAGE_KOREAN:
	{
		const Font nOldFont = D2Win_10127_SetFont(nFontSize);
		if (D2Win_10125() <= 13)
		{
			dword_6F8BE238 = (D2Win_10125() > 7) + 6;
		}
		else
		{
			dword_6F8BE238 = 8;
		}
		D2Win_10127_SetFont(nOldFont);
		break;
	}
	case LANGUAGE_CHINESETWN:
	{
		const Font nOldFont = D2Win_10127_SetFont(nFontSize);
		if (D2Win_10125() <= 13)
		{
			dword_6F8BE238 = (D2Win_10125() > 7) + 6;
		}
		else
		{
			dword_6F8BE238 = 8;
		}

		if (dword_6F96A8DC)
		{
			dword_6F8BE238 = 3;
		}

		D2Win_10127_SetFont(nOldFont);
		break;
	}
	default:
	{
		dword_6F8BE238 = 4;
		break;
	}
	}
}

//1.10f: D2Win.0x6F8AFDC0 (#10043)
//1.13c: D2Win.0x? (#10178)
int32_t __fastcall D2Win_10043_TEXTBOX_Destroy(D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	D2WinScrollBarStrc* pScrollBar = pTextBox->pScrollBar;
	if (pScrollBar)
	{
		SCROLLBAR_Destroy(&pScrollBar->controlHeader);
	}

	D2Win_10047(pTextBox);

	return CONTROL_RemoveFromList(&pTextBox->controlHeader);
}

//D2Win.0x6F8AFE30 (#10047)
int32_t __fastcall D2Win_10047(D2WinTextBoxStrc* pTextBox)
{
	if (!pTextBox)
	{
		return 0;
	}

	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	D2WinTextBoxLineStrc* pNext = nullptr;
	for (D2WinTextBoxLineStrc* pLine = pTextBox->pFirstLine; pLine; pLine = pNext)
	{
		pNext = pLine->pNext;

		for (int32_t i = 0; i < std::size(pLine->pColumns); ++i)
		{
			if (pLine->pColumns[i])
			{
				D2_FREE(pLine->pColumns[i]);
			}
		}

		D2_FREE(pLine);
	}

	pTextBox->pCurrentLine = nullptr;
	pTextBox->pFirstLine = nullptr;
	pTextBox->pLastLine = nullptr;
	pTextBox->nNumLines = 0;
	pTextBox->nLine = 0;
	return 1;
}

//D2Win.0x6F8AFED0 (#10062)
int32_t __fastcall D2Win_10062(D2WinTextBoxStrc* pTextBox, int32_t a2, int32_t a3)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	pTextBox->field_A0 = a2;
	pTextBox->field_A4 = a3;
	return 1;
}

//D2Win.0x6F8AFF30 (#10045)
void __fastcall D2Win_10045(D2WinTextBoxStrc* pTextBox, const char* szText)
{
	Unicode wszText[1024] = {};

	D2_ASSERT(pTextBox);

	Unicode::win2Unicode(wszText, szText, std::size(wszText));
	D2Win_10051(pTextBox, wszText, 4, 0, 0);
}

//D2Win.0x6F8AFFB0 (#10046)
int32_t __fastcall D2Win_10046(D2WinTextBoxStrc* pTextBox, const char* szText, int32_t a3)
{
	Unicode wszText[1024] = {};

	D2_ASSERT(pTextBox);

	Unicode::utf8ToUnicode(wszText, szText, std::size(wszText));
	return D2Win_10051(pTextBox, wszText, a3, 0, 0);
}

//D2Win.0x6F8B0030 (#10044)
int32_t __fastcall D2Win_10044(D2WinTextBoxStrc* pTextBox, const Unicode* wszText)
{
	return D2Win_10051(pTextBox, wszText, 4, 0, 0);
}

int32_t __fastcall D2Win_10051_Helper(D2WinTextBoxStrc* pTextBox, const Unicode* v9, int32_t a3, char a4, int32_t a5)
{
	D2WinTextBoxLineStrc* v23 = D2_ALLOC_STRC(D2WinTextBoxLineStrc);
	Unicode* v26 = nullptr;
	if (a5 == 2)
	{
		v23->pColumns[0] = (Unicode*)D2_ALLOC(sizeof(Unicode) * (Unicode::strlen(v9) + 6));
		v23->pColumns[0][4] = ' ';
		v23->pColumns[0][3] = v23->pColumns[0][4];
		v23->pColumns[0][2] = v23->pColumns[0][3];
		v23->pColumns[0][1] = v23->pColumns[0][2];
		v23->pColumns[0][0] = v23->pColumns[0][1];

		v26 = (Unicode*)(v23->pColumns[0] + 5);
	}
	else
	{
		v23->pColumns[0] = (Unicode*)D2_ALLOC(sizeof(Unicode) * (Unicode::strlen(v9) + 1));

		v26 = v23->pColumns[0];
	}
	Unicode::strcpy(v26, v9);

	int32_t v29 = 0;

	v23->pColumns[1] = nullptr;
	v23->pColumns[2] = nullptr;
	v23->pColumns[3] = nullptr;
	v23->pColumns[4] = nullptr;
	v23->field_14 = a3;
	v23->field_18 = a4;
	v23->pNext = nullptr;

	if (pTextBox->pFirstLine)
	{
		pTextBox->pLastLine->pNext = v23;
		pTextBox->pLastLine = v23;
		++pTextBox->nNumLines;

		D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

		D2WinTextBoxLineStrc* v33 = pTextBox->pCurrentLine;
		int32_t i = 0;
		while (v33)
		{
			v33 = v33->pNext;
			++i;
		}

		D2Win_10127_SetFont(pTextBox->eFont);

		const int32_t v35 = pTextBox->controlHeader.nHeight - 2 * pTextBox->field_44;
		sub_6F8AFCC0(pTextBox->eFont);

		const int32_t v36 = D2Win_10125();
		if (i > (v35 - (pTextBox->field_A8 + v36)) / (pTextBox->field_A8 + v36 + pTextBox->field_A4) + 1)
		{
			D2WinScrollBarStrc* pScrollBar = pTextBox->pScrollBar;
			if (!pScrollBar || SCROLLBAR_GetCurrentStep(pScrollBar) == SCROLLBAR_GetMaxSteps(pTextBox->pScrollBar))
			{
				v29 = 1;
				++pTextBox->nLine;
				pTextBox->pCurrentLine = pTextBox->pCurrentLine->pNext;
			}
		}
	}
	else
	{
		pTextBox->pFirstLine = v23;
		pTextBox->pLastLine = v23;
		pTextBox->pCurrentLine = v23;
		++pTextBox->nNumLines;
	}

	D2WinScrollBarStrc* pScrollBar = pTextBox->pScrollBar;
	if (pScrollBar)
	{
		const int32_t v41 = SCROLLBAR_GetCurrentStep(pScrollBar);

		D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

		D2Win_10127_SetFont(pTextBox->eFont);

		const int32_t v42 = pTextBox->controlHeader.nHeight - 2 * pTextBox->field_44;
		sub_6F8AFCC0(pTextBox->eFont);

		const int32_t v43 = D2Win_10125();
		SCROLLBAR_SetMaxSteps(pTextBox->pScrollBar, pTextBox->nNumLines - (v42 - (pTextBox->field_A8 + v43)) / (pTextBox->field_A8 + v43 + pTextBox->field_A4) - 1);

		if (v29)
		{
			SCROLLBAR_SetCurrentStep(pTextBox->pScrollBar, pTextBox->nLine);
		}
		else
		{
			SCROLLBAR_SetCurrentStep(pTextBox->pScrollBar, v41);
		}
	}

	return 1;
}

//D2Win.0x6F8B0040) --------------------------------------------------------
// Tooltip ?
int32_t __fastcall D2Win_10051(D2WinTextBoxStrc* pTextBox, const Unicode* a2, int32_t a3, char a4, int32_t a5)
{
	Unicode* v14; // ebp@27
	int v15; // edi@27
	int v16; // eax@27
	Unicode* v17; // ecx@27
	Unicode* v18; // ebx@37
	int v19; // ebp@37
	Unicode* v20; // edi@37
	signed int v21; // ebp@37
	signed int v29; // ebp@44
	Unicode* v46; // [sp+40h] [bp-824h]@27
	int v48; // [sp+44h] [bp-820h]@23
	Unicode* v49; // [sp+48h] [bp-81Ch]@27
	Unicode* a2a; // [sp+58h] [bp-80Ch]@32
	int v52; // [sp+5Ch] [bp-808h]@27
	Unicode* v53; // [sp+60h] [bp-804h]@27


	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	Unicode v58[1024] = {};

	Unicode::strcpy(v58, a2);

	const Unicode* v9 = v58;
	while (*v9)
	{
		if (*v9 >= 0x100u || !isspace(*v9))
		{
			break;
		}
		++v9;
	}

	D2Win_10127_SetFont(pTextBox->eFont);
	
	if (pTextBox->dwTextBoxFlags & 0x20)
	{
		return D2Win_10051_Helper(pTextBox, v9, a3, a4, a5);
	}

	int32_t v10 = pTextBox->controlHeader.nWidth - 2 * pTextBox->field_40;
	if (a5 == 2)
	{
		v10 -= D2Win_10121_GetTextWidth((const Unicode*)L"     ");
	}

	if (D2Win_10122(v9, 255) < v10)
	{
		return D2Win_10051_Helper(pTextBox, v9, a3, a4, a5);
	}

	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	v48 = pTextBox->controlHeader.nWidth - 2 * pTextBox->field_40;
	if (a5 == 2)
	{
		v48 -= D2Win_10121_GetTextWidth((const Unicode*)L"     ");
	}

	v14 = (Unicode*)SMemAlloc(2048, __FILE__, __LINE__, 0);
	v49 = v14;
	Unicode::strcpy(v14, v9);

	v15 = 0;
	v16 = Unicode::strlen(v14);
	v17 = v14;
	v52 = v16;
	v46 = v14;
	v53 = 0;
	if (v16 > 0)
	{
		while (1)
		{
			if (*v17 == ' ')
			{
				v53 = v17;
			}

			a2a = (Unicode*)(v15 + 1);
			if (D2Win_10122(v9, v15 + 1) >= v48)
			{
				break;
			}

			v15 = (int)a2a;
			++v46;

			if ((signed int)a2a >= v52)
			{
				goto LABEL_37;
			}

			v17 = v46;
		}

		if (v53)
		{
			v46 = v53;
			v15 = v53 - v14;
		}
	}

LABEL_37:
	v18 = (Unicode*)SMemAlloc(2048, __FILE__, __LINE__, 0);
	a2a = (Unicode*)SMemAlloc(2048, __FILE__, __LINE__, 0);
	Unicode::strncpy(v18, v14, v15);

	v18[v15] = 0;
	v19 = v52 - v15;
	v20 = a2a;
	Unicode::strncpy(a2a, v46, v19);
	v20[v19] = 0;

	SMemFree(v49, __FILE__, __LINE__, 0);
	D2Win_10051(pTextBox, v18, a3, 1, a5);
	
	if (a5 == 1)
	{
		D2Win_10051(pTextBox, v20, a3, 0, 2);
	}
	else
	{
		D2Win_10051(pTextBox, v20, a3, 0, a5);
	}

	SMemFree(v18, __FILE__, __LINE__, 0);
	SMemFree(v20, __FILE__, __LINE__, 0);
	return 1;
}

//D2Win.0x6F8B05E0 (#10198)
int32_t __fastcall D2Win_10198(D2WinTextBoxStrc* pTextBox, const Unicode* pText, int32_t a3, int32_t a4, char a5)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	for (int32_t i = 0; i < a3; ++i)
	{
		D2WinTextBoxLineStrc* pLine = D2_CALLOC_STRC(D2WinTextBoxLineStrc);
		pLine->pColumns[0] = (Unicode*)D2_ALLOC(sizeof(Unicode) * (Unicode::strlen(pText) + 1));
		Unicode::strcpy(pLine->pColumns[0], pText);
		pLine->pColumns[1] = nullptr;
		pLine->pColumns[2] = nullptr;
		pLine->pColumns[3] = nullptr;
		pLine->pColumns[4] = nullptr;
		pLine->field_14 = a4;
		pLine->field_18 = a5;
		pLine->pNext = nullptr;

		if (pTextBox->pFirstLine)
		{
			pTextBox->pLastLine->pNext = pLine;
		}
		else
		{
			pTextBox->pFirstLine = pLine;
		}

		pTextBox->pLastLine = pLine;
		++pTextBox->nNumLines;
	}

	if (pTextBox->pScrollBar)
	{
		D2Win_10127_SetFont(pTextBox->eFont);
		sub_6F8AFCC0(pTextBox->eFont);
		const int32_t nTextHeight = D2Win_10125();
		SCROLLBAR_SetMaxSteps(pTextBox->pScrollBar, pTextBox->nNumLines - (pTextBox->controlHeader.nHeight - 2 * pTextBox->field_44 - (pTextBox->field_A8 + nTextHeight)) / (pTextBox->field_A8 + nTextHeight + pTextBox->field_A4) - 1);
	}

	return 1;
}

//D2Win.0x6F8B0750 (#10057)
int32_t __fastcall D2Win_10057(D2WinTextBoxStrc* pTextBox, const char* a2, int32_t a3, int32_t a4)
{
	Unicode wszText[1024] = {};

	Unicode::win2Unicode(wszText, a2, std::size(wszText));
	return D2Win_10058_0(pTextBox, wszText, a3, a4);
}

//D2Win.0x6F8B07C0 (#10058)
int32_t __fastcall D2Win_10058_0(D2WinTextBoxStrc* pTextBox, const Unicode* pText, int32_t a3, int32_t nColumn)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);
	D2_ASSERT(pTextBox->nNumColumns > nColumn);

	Unicode wszText[1024] = {};

	Unicode::strcpy(wszText, pText);

	D2WinTextBoxLineStrc* pLine = pTextBox->pFirstLine;
	
	for (int32_t i = 0; i < a3; ++i)
	{
		if (!pLine)
		{
			return 1;
		}

		pLine = pLine->pNext;	
	}

	if (pLine)
	{
		D2_ASSERT(pLine->pColumns[nColumn] == nullptr);

		pLine->pColumns[nColumn] = (Unicode*)D2_ALLOC(sizeof(Unicode) * (Unicode::strlen(wszText) + 1));
		Unicode::strcpy(pLine->pColumns[nColumn], wszText);
	}

	return 1;
}

//D2Win.0x6F8B08F0 (#10197)
void __fastcall D2Win_10197(D2WinTextBoxStrc* pTextBox, int32_t nLineNum)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);
	D2_ASSERT(nLineNum <= pTextBox->nNumLines);

	D2WinTextBoxLineStrc* pLine = pTextBox->pFirstLine;
	int32_t i = 0;
	while (i < nLineNum)
	{
		if (!pLine->pNext)
		{
			break;
		}
		pLine = pLine->pNext;
		++i;
	}

	D2_ASSERT(pLine);
	D2_ASSERT(i == nLineNum);

	for (int32_t j = 0; j < std::size(pLine->pColumns); ++j)
	{
		if (pLine->pColumns[j])
		{
			D2_FREE(pLine->pColumns[j]);
			pLine->pColumns[j] = nullptr;
		}
	}
}

//D2Win.0x6F8B09E0 (#10052)
int32_t __fastcall D2Win_10052_TEXTBOX_GetNumLines(D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	return pTextBox->nNumLines;
}

//D2Win.0x6F8B0A30 (#10054)
int32_t __fastcall D2Win_10054(D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	return pTextBox->nLine;
}

//D2Win.0x6F8B0A80 (#10049)
int32_t __fastcall D2Win_10049(D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	return pTextBox->nCurrentLine;
}

//D2Win.0x6F8B0AD0 (#10050)
void __fastcall D2Win_10050_TEXTBOX_SetLine(D2WinTextBoxStrc* pTextBox, int32_t nLine)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	pTextBox->nCurrentLine = D2Clamp(nLine, -1, pTextBox->nNumLines - 1);
}

//D2Win.0x6F8B0B30 (#10053)
int32_t __fastcall D2Win_10053(D2WinTextBoxStrc* pTextBox, int32_t a2)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	a2 = std::min(a2, pTextBox->nNumLines);

	D2WinTextBoxLineStrc* pLine = pTextBox->pFirstLine;
	for (int32_t i = 0; i < a2; ++i)
	{
		if (!pLine->pNext)
		{
			break;
		}

		pLine = pLine->pNext;
	}

	if (pLine)
	{
		pTextBox->pCurrentLine = pLine;
		pTextBox->nLine = a2;
	}

	if (pTextBox->pScrollBar)
	{
		SCROLLBAR_SetCurrentStep(pTextBox->pScrollBar, a2);
	}

	return 1;
}

//D2Win.0x6F8B0BC0 (#10055)
int32_t __fastcall D2Win_10055(D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	D2Win_10127_SetFont(pTextBox->eFont);
	sub_6F8AFCC0(pTextBox->eFont);

	const int32_t nTextHeight = D2Win_10125();
	return (pTextBox->controlHeader.nHeight - 2 * pTextBox->field_44 - (pTextBox->field_A8 + nTextHeight)) / (pTextBox->field_A8 + nTextHeight + pTextBox->field_A4) + 1;
}

//D2Win.0x6F8B0C50 (#10048)
Unicode* __fastcall D2Win_10048(D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	if (pTextBox->nCurrentLine == -1 || !(pTextBox->dwTextBoxFlags & 1))
	{
		return 0;
	}

	int32_t nLine = 0;
	for (D2WinTextBoxLineStrc* pLine = pTextBox->pFirstLine; pLine; pLine = pLine->pNext)
	{
		if (pTextBox->nCurrentLine == nLine)
		{
			return pLine->pColumns[0];
		}

		++nLine;
	}

	return 0;
}

//D2Win.0x6F8B0CC0 (#10196)
Unicode* __fastcall D2Win_10196(D2WinTextBoxStrc* pTextBox, int32_t nLineNum, int32_t nColumnNum)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);
	D2_ASSERT(nLineNum <= pTextBox->nNumLines);
	D2_ASSERT(nColumnNum < std::size(pTextBox->unk0x68));

	D2WinTextBoxLineStrc* pLine = pTextBox->pFirstLine;
	int32_t i = 0;
	while (i < nLineNum)
	{
		if (!pLine->pNext)
		{
			break;
		}

		pLine = pLine->pNext;
		++i;
	}

	D2_ASSERT(pLine);
	D2_ASSERT(i == nLineNum);

	return pLine->pColumns[nColumnNum];
}

//D2Win.0x6F8B0DC0 (#10056)
void __fastcall D2Win_10056(D2WinTextBoxStrc* pTextBox, int32_t nNumColumns)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);
	D2_ASSERT((nNumColumns > 0) && (nNumColumns <= std::size(pTextBox->unk0x68)));
	D2_ASSERT(pTextBox->nNumColumns <= nNumColumns);

	pTextBox->nNumColumns = nNumColumns;

	if (nNumColumns > 1)
	{
		pTextBox->dwTextBoxFlags |= 0x20;
	}
}

//D2Win.0x6F8B0E60 (#10059)
void __fastcall D2Win_10059(D2WinTextBoxStrc* pTextBox, int32_t nColumn, int32_t a3)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);
	D2_ASSERT((nColumn >= 0) && (nColumn < pTextBox->nNumColumns));

	pTextBox->unk0x68[nColumn] = a3;
}

//D2Win.0x6F8B0EE0 (#10060)
void __fastcall D2Win_10060(D2WinTextBoxStrc* pTextBox, int32_t nColumn, int32_t a3)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);
	D2_ASSERT(nColumn >= 0);
	
	pTextBox->unk0x7C[nColumn] = a3;
}

//D2Win.0x6F8B0F60 (#10061)
int32_t __fastcall D2Win_10061(D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	int32_t nLineCount = 0;
	D2WinTextBoxLineStrc* pLine = pTextBox->pFirstLine;
	while (pLine)
	{
		pLine = pLine->pNext;
		if (!pLine)
		{
			return nLineCount;
		}

		++nLineCount;
	}

	return nLineCount;
}

//D2Win.0x6F8B0FC0 (#10187)
void __fastcall D2Win_10187(D2WinTextBoxStrc* pTextBox, int32_t bSet)
{
	D2Win_10027_CONTROL_ToggleThirdFlag(&pTextBox->controlHeader, bSet);

	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	if (pTextBox->pScrollBar)
	{
		D2Win_10027_CONTROL_ToggleThirdFlag(&pTextBox->pScrollBar->controlHeader, bSet);
	}
}

//D2Win.0x6F8B1030 (#10188)
int __fastcall D2Win_10188(D2WinTextBoxStrc* pTextBox, int32_t nLineNum, int32_t a3)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	if (nLineNum != -1)
	{
		D2_ASSERT(nLineNum <= pTextBox->nNumLines);

		D2WinTextBoxLineStrc* pLine = pTextBox->pFirstLine;
		int32_t i = 0;
		while (i < nLineNum)
		{
			if (!pLine->pNext)
			{
				break;
			}

			pLine = pLine->pNext;
			++i;
		}

		D2_ASSERT(pLine);
		D2_ASSERT(i == nLineNum);

		pLine->field_14 = a3;
		return 1;
	}

	for (D2WinTextBoxLineStrc* pLine = pTextBox->pFirstLine; pLine; pLine = pLine->pNext)
	{
		pLine->field_14 = a3;
	}

	return 1;
}

//D2Win.0x6F8B1130
int32_t __fastcall TEXTBOX_Draw(D2WinControlStrc* pControl)
{
	D2WinTextBoxStrc* pTextBox = (D2WinTextBoxStrc*)pControl;

	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	if (!(pTextBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pTextBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	const uint8_t nColor = D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u);

	D2CellFileStrc* pCellFile = pTextBox->controlHeader.pCellFile;
	if (pCellFile)
	{
		D2GfxDataStrc gfxData = {};
		gfxData.pCellFile = pCellFile;
		gfxData.nDirection = 0;
		gfxData.nFrame = 0;
		TEXTURE_CelDraw(&gfxData, pTextBox->controlHeader.nImageX, pTextBox->controlHeader.nImageY - 1, -1u, DRAWMODE_NORMAL, 0);
	}

	sub_6F8AFCC0(pTextBox->eFont);
	D2Win_10127_SetFont(pTextBox->eFont);

	// TODO: v51, v8, v49, v48, v16, v34
	const int32_t v51 = pTextBox->field_40 + pTextBox->controlHeader.nImageX;
	int32_t nY = pTextBox->field_44 + pTextBox->controlHeader.nImageY - pTextBox->controlHeader.nHeight + D2Win_10125();

	sub_6F8AFCC0(pTextBox->eFont);

	const int32_t v8 = pTextBox->field_A8 + D2Win_10125();
	int32_t v49 = pTextBox->controlHeader.nHeight - 2 * pTextBox->field_44;
	
	const int32_t nBoxWidth = pTextBox->controlHeader.nWidth - 2 * pTextBox->field_40;

	if (pTextBox->dwTextBoxFlags & 0x40)
	{
		D2WinTextBoxLineStrc* pCurrentLine = pTextBox->pCurrentLine;
		if (pCurrentLine)
		{
			pTextBox->field_9C -= 2;

			if (pTextBox->field_9C < -D2Win_10121_GetTextWidth(pCurrentLine->pColumns[0]))
			{
				pTextBox->field_9C = nBoxWidth;
			}
		}
	}

	if (pTextBox->dwTextBoxFlags & 0x80)
	{
		D2WinTextBoxLineStrc* pCurrentLine = pTextBox->pCurrentLine;
		if (pCurrentLine)
		{
			pTextBox->field_9C -= 2;

			if (pTextBox->field_9C < -v8)
			{
				pTextBox->field_9C = 0;
				pTextBox->pCurrentLine = pCurrentLine->pNext;
			}
		}
	}

	int32_t nCounter = 0;
	for (D2WinTextBoxLineStrc* pCurrentLine = pTextBox->pCurrentLine; pCurrentLine; pCurrentLine = pCurrentLine->pNext)
	{
		for (int32_t i = 0; i < pTextBox->nNumColumns; ++i)
		{
			const Unicode* wszColumn = pCurrentLine->pColumns[i];
			if (!wszColumn)
			{
				break;
			}

			int32_t v48 = pTextBox->controlHeader.nWidth;
			if (i < pTextBox->nNumColumns - 1)
			{
				v48 = pTextBox->unk0x68[i + 1];
			}

			const int32_t v16 = pTextBox->unk0x68[i];
			int32_t nX = 0;
			if (v16)
			{
				nX = v16 + v51;

				int32_t v18 = 0;
				if (i + 1 >= pTextBox->nNumColumns)
				{
					v18 = pTextBox->controlHeader.nWidth - v16;
					v48 = pTextBox->controlHeader.nWidth - v16;
				}
				else
				{
					v18 = pTextBox->unk0x68[i + 1] - v16;
					v48 = pTextBox->unk0x68[i + 1] - v16;
				}

				if (pTextBox->unk0x7C[i] & 2)
				{
					nX += std::max((v18 - D2Win_10121_GetTextWidth(wszColumn)) / 2, 0);
				}
				else if (pTextBox->unk0x7C[i] & 0x10)
				{
					nX += v48 - std::max(D2Win_10121_GetTextWidth(wszColumn), 0);
				}
			}
			else
			{
				if (pTextBox->dwTextBoxFlags & 2)
				{
					nX = pTextBox->controlHeader.nImageX + std::max((pTextBox->controlHeader.nWidth - D2Win_10121_GetTextWidth(pCurrentLine->pColumns[0])) / 2, 0);
				}
				else if (pTextBox->dwTextBoxFlags & 0x10)
				{
					nX = pTextBox->controlHeader.nImageX - std::max(D2Win_10121_GetTextWidth(pCurrentLine->pColumns[0]), 0);
				}
				else
				{
					nX = v51;
				}
			}

			Unicode wszText[1024] = {};
			Unicode::strcpy(wszText, pCurrentLine->pColumns[i]);

			int32_t j = 0;
			while (j < Unicode::strlen(wszText))
			{
				if (D2Win_10122(wszText, j + 1) >= v48)
				{
					break;
				}
				++j;
			}
			wszText[j] = L'\0';

			if (pTextBox->nCurrentLine == nCounter + pTextBox->nLine && pTextBox->dwTextBoxFlags & 1)
			{
				if (i)
				{
					D2Win_10117_DrawText(wszText, nX, nY, pCurrentLine->field_14 != 4 ? pCurrentLine->field_14 : 0, 0);
				}
				else
				{
					sub_6F8AB360(wszText, nX, nY, nBoxWidth, nColor, DRAWMODE_NORMAL, pCurrentLine->field_14 != 4 ? pCurrentLine->field_14 : 0);
				}
			}
			else if (pTextBox->dwTextBoxFlags & 0x40)
			{
				sub_6F8AA510(pCurrentLine->pColumns[i], nX, nY, pCurrentLine->field_14, pTextBox->field_9C, nBoxWidth);
			}
			else if (pTextBox->dwTextBoxFlags & 0x80)
			{
				D2Win_10117_DrawText(wszText, nX, nY + pTextBox->field_9C, pCurrentLine->field_14, 0);
			}
			else
			{
				D2Win_10117_DrawText(wszText, nX, nY, pCurrentLine->field_14, 0);
			}
		}

		const int32_t v34 = v8 + (pCurrentLine->field_18 ? pTextBox->field_A0 : pTextBox->field_A4);
		nY += v34;
		v49 -= v34;

		++nCounter;

		if (v49 < v34)
		{
			break;
		}
	}

	D2Win_10127_SetFont(D2FONT_FONT16);
	return 1;
}

//D2Win.0x6F8B15A0
int32_t __stdcall TEXTBOX_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinTextBoxStrc* pTextBox = (D2WinTextBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	if (!(pTextBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pTextBox->controlHeader.dwFlags & gdwBitMasks[0]) || pTextBox->nNumLines < 1)
	{
		return 0;
	}

	if (pTextBox->dwTextBoxFlags & 1)
	{
		D2Win_10127_SetFont(pTextBox->eFont);
		sub_6F8AFCC0(pTextBox->eFont);

		pTextBox->nCurrentLine = (gMousePosition_6F8FE234.y + pTextBox->controlHeader.nHeight - pTextBox->controlHeader.nImageY - pTextBox->field_44) / (pTextBox->field_A8 + D2Win_10125()) + pTextBox->nLine;

		if (pTextBox->dwTextBoxFlags & 8)
		{
			pTextBox->nCurrentLine = D2Clamp(pTextBox->nCurrentLine, 0, pTextBox->nNumLines - 1);
		}
		else if (pTextBox->nCurrentLine < 0 || pTextBox->nCurrentLine >= pTextBox->nNumLines)
		{
			pTextBox->nCurrentLine = -1;
		}
	}

	if (pTextBox->controlHeader.field_34)
	{
		return pTextBox->controlHeader.field_34(pMsg);
	}

	return 0;
}

//D2Win.0x6F8B16B0
int32_t __stdcall TEXTBOX_HandleVirtualKeyInput(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinTextBoxStrc* pTextBox = (D2WinTextBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	if (!(pTextBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pTextBox->controlHeader.dwFlags & gdwBitMasks[0]) || pTextBox->nNumLines < 1 || pTextBox->nCurrentLine == -1 || D2Win_10023())
	{
		return 0;
	}

	// TODO: v3, v4, v9, v10
	int32_t v3 = pTextBox->nLine;
	int32_t v4 = pTextBox->nCurrentLine;

	const int32_t nNumLines = pTextBox->nNumLines;
	const int32_t v9 = D2Win_10055(pTextBox);

	switch (pMsg->wParam)
	{
	case VK_PRIOR:
	{
		if (v4 > v3)
		{
			v4 = v3;
		}
		else if (v3 > 0)
		{
			v3 = std::max(v3 - v9 + 1, 0);
			v4 = v3;
		}
		break;
	}
	case VK_NEXT:
	{
		const int32_t v10 = v9 + v3 - 1;
		if (v4 < v10)
		{
			v4 = std::min(nNumLines - 1, v10);
		}
		else if (v3 < nNumLines - v9)
		{
			v3 = std::min(v10, nNumLines - v9);
			v4 = std::min(v10, nNumLines - 1);
		}
		break;
	}
	case VK_UP:
	{
		if (v4 > 0)
		{
			--v4;

			v3 = D2Clamp(v3, v4 - v9 + 1, v4);
		}
		break;
	}
	case VK_DOWN:
	{
		if (v4 < nNumLines - 1)
		{
			++v4;

			v3 = D2Clamp(v3, v4 - v9 + 1, v4);
		}
		break;
	}
	case VK_HOME:
	{
		v4 = 0;
		v3 = 0;
		break;
	}
	case VK_END:
	{
		v4 = nNumLines - 1;
		v3 = nNumLines - v9;
		break;
	}
	default:
	{
		break;
	}
	}

	const bool hasChanged = v4 != pTextBox->nCurrentLine;
	pTextBox->nCurrentLine = v4;

	if (nNumLines > v9 && v3 != pTextBox->nLine)
	{
		D2Win_10053(pTextBox, v3);
	}

	if (hasChanged && pTextBox->controlHeader.field_34)
	{
		pTextBox->controlHeader.field_34(pMsg);
	}

	return 0;
}

//D2Win.0x6F8B18F0 (#10190)
int32_t __fastcall D2Win_10190(D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	return SCROLLBAR_GetMaxSteps(pTextBox->pScrollBar);
}

//D2Win.0x6F8B1940 (#10191)
int32_t __fastcall D2Win_10191(D2WinTextBoxStrc* pTextBox, int32_t a2)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	return SCROLLBAR_SetMaxSteps(pTextBox->pScrollBar, a2);
}

//D2Win.0x6F8B1990 (#10192)
int32_t __fastcall D2Win_10192(D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	return SCROLLBAR_GetCurrentStep(pTextBox->pScrollBar);
}

//D2Win.0x6F8B19E0 (#10193)
int32_t __fastcall D2Win_10193(D2WinTextBoxStrc* pTextBox, int32_t a2)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	return SCROLLBAR_SetCurrentStep(pTextBox->pScrollBar, a2);
}

//D2Win.0x6F8B1A30 (#10194)
int32_t __fastcall D2Win_10194(D2WinTextBoxStrc* pTextBox, D2WinTextBoxStrc* a2)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	return SCROLLBAR_SetTextBox(pTextBox->pScrollBar, a2);
}

//D2Win.0x6F8B1A80 (#10195)
int32_t __fastcall D2Win_10195(D2WinTextBoxStrc* pTextBox, void(__stdcall* a2)(SMSGHANDLER_PARAMS*))
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	D2WinScrollBarStrc* pScrollBar = pTextBox->pScrollBar;

	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	pScrollBar->field_5C = a2;
	return 1;
}

//D2Win.0x6F8B1AD0 (#10204)
void __fastcall D2Win_10204(D2WinTextBoxStrc* pTextBox, int32_t a2)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	D2Win_10202(pTextBox->pScrollBar, a2);
}

//D2Win.0x6F8B1B20 (#10203)
int32_t __fastcall D2Win_10203_TEXTBOX(D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pTextBox);
	D2_ASSERT(pTextBox->controlHeader.nType == D2WIN_TEXTBOX);

	return D2Win_10201(pTextBox->pScrollBar);
}
