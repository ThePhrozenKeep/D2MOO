#include "D2WinPopUp.h"

#include <algorithm>

#include <D2Gfx.h>

#include <Fog.h>
#include <D2BitManip.h>

#include "D2WinFont.h"
#include "D2WinMain.h"
#include "D2WinPalette.h"


D2WinPopUpStrc* gpPopUp;


//D2Win.0x6F8AEAE0 (#10094)
void __stdcall POPUP_Create()
{
	POPUP_Destroy();

	D2WinPopUpStrc* pPopUp = D2_CALLOC_STRC(D2WinPopUpStrc);

	pPopUp->controlHeader.nType = D2WIN_POPUP;
	pPopUp->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2] | gdwBitMasks[3];
	pPopUp->controlHeader.nImageX = gMousePosition_6F8FE234.x;
	pPopUp->controlHeader.nImageY = gMousePosition_6F8FE234.y;
	pPopUp->controlHeader.pfDraw = POPUP_Draw;
	pPopUp->controlHeader.pfInitialize = POPUP_Initialize;
	pPopUp->nCurrentLine = -1;

	CONTROL_AppendToList(&pPopUp->controlHeader);

	gpPopUp = pPopUp;
}

//D2Win.0x6F8AEBA0 (#10095)
void __stdcall POPUP_Destroy()
{
	if (gpPopUp)
	{
		CONTROL_RemoveFromList(&gpPopUp->controlHeader);
		gpPopUp = nullptr;
	}
}

//D2Win.0x6F8AEBC0
int32_t __stdcall POPUP_IsActive()
{
	D2WinPopUpStrc* pPopUp = gpPopUp;

	if (!pPopUp)
	{
		return 0;
	}

	D2_ASSERT(pPopUp->controlHeader.nType == D2WIN_POPUP);

	if (!(pPopUp->controlHeader.dwFlags & gdwBitMasks[2]) || !(pPopUp->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	return 1;
}

//D2Win.0x6F8AEC10 (#10096)
int32_t __stdcall POPUP_AddLine(D2WinPopUpLineStrc* pPopUpLine)
{
	D2WinPopUpStrc* pPopUp = gpPopUp;

	D2_ASSERT(pPopUp->controlHeader.nType == D2WIN_POPUP);

	if (pPopUp->nLines >= 8)
	{
		return 0;
	}

	memcpy(&gpPopUp->lines[pPopUp->nLines], pPopUpLine, sizeof(*pPopUpLine));
	D2Win_10127_SetFont(D2FONT_FONT16);

	const int32_t nTextWidth = D2Win_10121_GetTextWidth(pPopUpLine->wszText);
	pPopUp->controlHeader.nWidth = std::max(pPopUp->controlHeader.nWidth, nTextWidth);

	++pPopUp->nLines;
	pPopUp->controlHeader.nHeight = 20 * pPopUp->nLines;

	return 1;
}

//D2Win.0x6F8AECA0
int32_t __stdcall POPUP_HandleMouseButtonInput(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinPopUpStrc* pPopUp = gpPopUp;

	D2_ASSERT(pPopUp->controlHeader.nType == D2WIN_POPUP);

	if (!(pPopUp->controlHeader.dwFlags & gdwBitMasks[2]) || !(pPopUp->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	const int32_t nCurrentLine = pPopUp->nCurrentLine;
	if (nCurrentLine == -1)
	{
		return 0;
	}

	void(__stdcall* pfHandleMouseInput)() = pPopUp->lines[nCurrentLine].pfHandleMouseInput;
	if (!pfHandleMouseInput)
	{
		return 0;
	}

	pfHandleMouseInput();
	return 1;
}

//D2Win.0x6F8AED10
int32_t __fastcall POPUP_Initialize(D2WinControlStrc* pControl)
{
	D2WinPopUpStrc* pPopUp = (D2WinPopUpStrc*)pControl;

	D2_ASSERT(pPopUp->controlHeader.nType == D2WIN_POPUP);

	if (!(pPopUp->controlHeader.dwFlags & gdwBitMasks[2]) || !(pPopUp->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	pPopUp->nCurrentLine = -1;

	const int32_t nX = gMousePosition_6F8FE234.x - pPopUp->controlHeader.nImageX;
	const int32_t nY = gMousePosition_6F8FE234.y - pPopUp->controlHeader.nImageY + pPopUp->controlHeader.nHeight;

	if (nX >= 0 && nX <= pPopUp->controlHeader.nWidth && nY >= 0 && nY <= pPopUp->controlHeader.nHeight)
	{
		pPopUp->nCurrentLine = nY / 20;
	}

	return 1;
}

//D2Win.0x6F8AEDB0
int32_t __fastcall POPUP_Draw(D2WinControlStrc* pControl)
{
	D2WinPopUpStrc* pPopUp = (D2WinPopUpStrc*)pControl;

	D2_ASSERT(pPopUp->controlHeader.nType == D2WIN_POPUP);

	if (!(pPopUp->controlHeader.dwFlags & gdwBitMasks[2]) || !(pPopUp->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	RECT rect = {};
	rect.left = pPopUp->controlHeader.nImageX - 5;
	rect.bottom = pPopUp->controlHeader.nImageY + 5;
	rect.top = pPopUp->controlHeader.nImageY - pPopUp->controlHeader.nHeight - 5;
	rect.right = pPopUp->controlHeader.nImageX + pPopUp->controlHeader.nWidth + 5;
	
	D2GFX_UtilRect(&rect, 0xFFu);
	D2GFX_DrawBox(rect.left, rect.bottom, rect.right, rect.top, D2Win_10034_MixRGB(0x3Fu, 0x3Fu, 0x3Fu), DRAWMODE_TRANS75);
	D2Win_10127_SetFont(D2FONT_FONT16);

	for (int32_t i = 0; i < pPopUp->nLines; ++i)
	{
		D2Win_10117_DrawText(
			pPopUp->lines[i].wszText,
			pPopUp->controlHeader.nImageX,
			pPopUp->controlHeader.nImageY + 20 * (i - pPopUp->nLines + 1),
			((i != pPopUp->nCurrentLine) - 1) & 3,
			0
		);
	}

	return 1;
}
