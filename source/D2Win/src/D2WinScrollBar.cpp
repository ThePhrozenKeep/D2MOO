#include "D2WinScrollBar.h"

#include <algorithm>

#include <D2Math.h>

#include <D2CMP.h>

#include <D2Gfx.h>
#include <Texture.h>

#include <Fog.h>
#include <D2BitManip.h>

#include "D2WinMain.h"
#include "D2WinTextBox.h"


//D2Win.0x6F8AF040 (#10063)
D2WinScrollBarStrc* __fastcall SCROLLBAR_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, D2CellFileStrc* pCellFile, int32_t(__stdcall* a6)(SMSGHANDLER_PARAMS*))
{
	D2WinScrollBarStrc* pScrollBar = D2_CALLOC_STRC(D2WinScrollBarStrc);

	pScrollBar->controlHeader.nType = D2WIN_SCROLLBAR;
	pScrollBar->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pScrollBar->controlHeader.pCellFile = pCellFile;
	pScrollBar->controlHeader.nImageX = nX;
	pScrollBar->controlHeader.nImageY = nY;
	pScrollBar->controlHeader.nWidth = nWidth;
	pScrollBar->controlHeader.nHeight = nHeight;
	pScrollBar->controlHeader.pfDraw = SCROLLBAR_Draw;
	pScrollBar->controlHeader.pfHandleMouseDown = SCROLLBAR_HandleMouseDown;
	pScrollBar->controlHeader.pfHandleMouseUp = SCROLLBAR_HandleMouseUp;
	pScrollBar->controlHeader.field_34 = a6;

	CONTROL_AppendToList(&pScrollBar->controlHeader);

	return pScrollBar;
}

//D2Win.0x6F8B1BF0 (#10064)
int32_t __fastcall SCROLLBAR_Destroy(D2WinControlStrc* pControl)
{
	return CONTROL_RemoveFromList(pControl);
}

//D2Win.0x6F8AF100 (#10065)
int32_t __fastcall SCROLLBAR_GetMaxSteps(D2WinScrollBarStrc* pScrollBar)
{
	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	return pScrollBar->nMaxSteps;
}

//D2Win.0x6F8AF130 (#10066)
int32_t __fastcall SCROLLBAR_SetMaxSteps(D2WinScrollBarStrc* pScrollBar, int32_t nMaxSteps)
{
	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	pScrollBar->nMaxSteps = std::max(nMaxSteps, 0);

	return 1;
}

//D2Win.0x6F8AF170 (#10068)
int32_t __fastcall SCROLLBAR_SetCurrentStep(D2WinScrollBarStrc* pScrollBar, int32_t nCurrentStep)
{
	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	pScrollBar->nCurrentStep = std::min(nCurrentStep, pScrollBar->nMaxSteps);

	return 1;
}

//D2Win.0x6F8AF1B0 (#10067)
int32_t __fastcall SCROLLBAR_GetCurrentStep(D2WinScrollBarStrc* pScrollBar)
{
	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	return pScrollBar->nCurrentStep;
}

//D2Win.0x6F8AF1E0 (#10069)
int32_t __fastcall SCROLLBAR_SetTextBox(D2WinScrollBarStrc* pScrollBar, D2WinTextBoxStrc* pTextBox)
{
	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	pScrollBar->pTextBox = pTextBox;
	return 1;
}

//D2Win.0x6F8AF240
int32_t __fastcall sub_6F8AF240(SMSGHANDLER_PARAMS* pMsg, D2WinScrollBarStrc* pScrollBar, int32_t a3)
{
	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	if (pScrollBar->bIsStepIndicatorDragged)
	{
		return 0;
	}

	if (a3 > 0)
	{
		pScrollBar->bIsUpperArrowPressed = 1;
		--pScrollBar->nCurrentStep;
	}
	else if (a3 < 0)
	{
		pScrollBar->bIsLowerArrowPressed = 1;
		++pScrollBar->nCurrentStep;
	}

	pScrollBar->nCurrentStep = D2Clamp(pScrollBar->nCurrentStep, 0, pScrollBar->nMaxSteps);

	if (pScrollBar->pTextBox)
	{
		D2Win_10053(pScrollBar->pTextBox, pScrollBar->nCurrentStep);
	}

	if (pScrollBar->field_5C)
	{
		pScrollBar->field_5C(pMsg);
	}

	return 0;
}

//D2Win.0x6F8AF2D0 (#10202)
void __fastcall D2Win_10202(D2WinScrollBarStrc* pScrollBar, int32_t nPosition)
{
	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	pScrollBar->field_50 = nPosition;
}

//D2Win.0x6F8AF300 (#10201)
int32_t __fastcall D2Win_10201(D2WinScrollBarStrc* pScrollBar)
{
	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	return pScrollBar->field_50;
}

//D2Win.0x6F8AF330
int32_t __fastcall SCROLLBAR_Draw(D2WinControlStrc* pControl)
{
	D2WinScrollBarStrc* pScrollBar = (D2WinScrollBarStrc*)pControl;

	D2_ASSERT(pScrollBar);
	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	if (!(pScrollBar->controlHeader.dwFlags & gdwBitMasks[2]) || !(pScrollBar->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	if (!pScrollBar->nMaxSteps)
	{
		return 1;
	}

	if (!D2Win_IsMouseButtonPressed())
	{
		pScrollBar->bIsStepIndicatorDragged = 0;
	}

	if (pScrollBar->bIsStepIndicatorDragged)
	{
		SMSGHANDLER_PARAMS msg = {};

		msg.hWindow = (HWND)pScrollBar;
		msg.nMessage = WM_LBUTTONDOWN;
		SCROLLBAR_HandleMouseDown(&msg);
	}

	D2GfxDataStrc gfxData = {};
	gfxData.nDirection = 0;
	gfxData.pCellFile = pScrollBar->controlHeader.pCellFile;
	gfxData.nFrame = 2 * (pScrollBar->bIsLowerArrowPressed != 0) + 1;

	int32_t nY = pScrollBar->controlHeader.nImageY - 1;
	TEXTURE_CelDraw(&gfxData, pScrollBar->controlHeader.nImageX, nY, -1, DRAWMODE_NORMAL, 0);

	gfxData.nFrame = 5;

	for (int32_t i = 10; i < pScrollBar->controlHeader.nHeight - 10; i += 10)
	{
		nY -= 10;
		TEXTURE_CelDraw(&gfxData, pScrollBar->controlHeader.nImageX, nY, -1, DRAWMODE_NORMAL, 0);
	}

	gfxData.nFrame = pScrollBar->bIsUpperArrowPressed != 0 ? 2 : 0;
	TEXTURE_CelDraw(&gfxData, pScrollBar->controlHeader.nImageX, pScrollBar->controlHeader.nImageY - pScrollBar->controlHeader.nHeight + 9, -1, DRAWMODE_NORMAL, 0);

	gfxData.nFrame = 4;
	TEXTURE_CelDraw(
		&gfxData,
		pScrollBar->controlHeader.nImageX,
		pScrollBar->controlHeader.nImageY + pScrollBar->nCurrentStep * (pScrollBar->controlHeader.nHeight - 30) / pScrollBar->nMaxSteps - pScrollBar->controlHeader.nHeight + 19,
		-1,
		DRAWMODE_NORMAL,
		0
	);

	return 1;
}

//D2Win.0x6F8AF4D0
int32_t __stdcall SCROLLBAR_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinScrollBarStrc* pScrollBar = (D2WinScrollBarStrc*)pMsg->hWindow;

	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	if (pMsg->nMessage != WM_LBUTTONDOWN || !(pScrollBar->controlHeader.dwFlags & gdwBitMasks[2]) || !(pScrollBar->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 1;
	}

	if (!pScrollBar->nMaxSteps)
	{
		return 1;
	}

	const int32_t nMouseX = gMousePosition_6F8FE234.x - pScrollBar->controlHeader.nImageX;
	if (nMouseX < 0 || nMouseX > pScrollBar->controlHeader.nWidth)
	{
		return 0;
	}

	// TODO: v9, v12
	const int32_t v9 = pScrollBar->controlHeader.nHeight - 30;
	const int32_t v12 = (v9 * pScrollBar->nCurrentStep) / pScrollBar->nMaxSteps + 10;

	const int32_t nMouseY = gMousePosition_6F8FE234.y + pScrollBar->controlHeader.nHeight - pScrollBar->controlHeader.nImageY;
	if (nMouseY >= v12 && nMouseY <= v12 + 10)
	{
		pScrollBar->bIsStepIndicatorDragged = 1;
	}

	if (nMouseY < 11 && !pScrollBar->bIsLowerArrowPressed && !pScrollBar->bIsStepIndicatorDragged)
	{
		pScrollBar->bIsUpperArrowPressed = 1;
		--pScrollBar->nCurrentStep;
	}
	else if (nMouseY > pScrollBar->controlHeader.nHeight - 10 && !pScrollBar->bIsUpperArrowPressed && !pScrollBar->bIsStepIndicatorDragged)
	{
		pScrollBar->bIsLowerArrowPressed = 1;
		++pScrollBar->nCurrentStep;
	}
	else if (!pScrollBar->bIsLowerArrowPressed && !pScrollBar->bIsUpperArrowPressed)
	{
		if (pScrollBar->bIsStepIndicatorDragged)
		{
			pScrollBar->nCurrentStep = pScrollBar->nMaxSteps * (nMouseY - 11) / v9;
		}
		else
		{
			int32_t nStep = pScrollBar->field_50;
			if (!nStep)
			{
				int32_t nDivisor = v9 / 10;
				if (!nDivisor)
				{
					nDivisor = 1;
				}

				nStep = pScrollBar->nMaxSteps / nDivisor;
				if (!nStep)
				{
					nStep = 1;
				}
			}

			if (nMouseY < v12)
			{
				pScrollBar->nCurrentStep -= nStep;
			}
			else if (nMouseY > v12 + 10)
			{
				pScrollBar->nCurrentStep += nStep;
			}
		}
	}

	pScrollBar->nCurrentStep = D2Clamp(pScrollBar->nCurrentStep, 0, pScrollBar->nMaxSteps);

	if (pScrollBar->pTextBox)
	{
		D2Win_10053(pScrollBar->pTextBox, pScrollBar->nCurrentStep);
	}

	if (pScrollBar->field_5C)
	{
		pScrollBar->field_5C(pMsg);
	}

	if (pScrollBar->controlHeader.field_34)
	{
		return pScrollBar->controlHeader.field_34(pMsg);
	}

	return 0;
}

//D2Win.0x6F8AF6B0
int32_t __stdcall SCROLLBAR_HandleMouseUp(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinScrollBarStrc* pScrollBar = (D2WinScrollBarStrc*)pMsg->hWindow;

	D2_ASSERT(pScrollBar->controlHeader.nType == D2WIN_SCROLLBAR);

	if (pScrollBar->bIsUpperArrowPressed || pScrollBar->bIsLowerArrowPressed || pScrollBar->bIsStepIndicatorDragged)
	{
		pScrollBar->bIsUpperArrowPressed = 0;
		pScrollBar->bIsLowerArrowPressed = 0;
		pScrollBar->bIsStepIndicatorDragged = 0;
		
		return 1;
	}

	return 0;
}
