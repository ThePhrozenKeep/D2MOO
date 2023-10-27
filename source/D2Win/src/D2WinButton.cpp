#include "D2WinButton.h"

#include <algorithm>

#include <D2CMP.h>

#include <D2Lang.h>

#include <D2Sound.h>

#include <D2Gfx.h>
#include <Texture.h>

#include <Fog.h>
#include <D2BitManip.h>

#include "D2WinFont.h"
#include "D2WinMain.h"


//D2Win.0x6F8A61A0 (#10107)
D2WinButtonStrc* __fastcall BUTTON_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, D2CellFileStrc* pCellFile, int32_t(__stdcall* pCallback)(D2WinMsgStrc*), int32_t nActivationVirtualKey, int32_t nBaseFrame, uint32_t nFlags, uint16_t nStringId, int32_t(__stdcall* pfUpdateHoverState)(D2WinMsgStrc*))
{
	D2WinButtonStrc* pButton = D2_CALLOC_STRC(D2WinButtonStrc);

	pButton->controlHeader.nType = D2WIN_BUTTON;
	pButton->controlHeader.nImageX = nX;
	pButton->controlHeader.nImageY = nY;
	pButton->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pButton->controlHeader.pCellFile = pCellFile;
	pButton->controlHeader.nWidth = nWidth;
	pButton->controlHeader.nHeight = nHeight;
	pButton->controlHeader.pfDraw = BUTTON_Draw;
	pButton->controlHeader.pfHandleMouseDown = BUTTON_HandleMouseDown;
	pButton->controlHeader.pfHandleMouseUp = BUTTON_HandleMouseUp;

	if (nActivationVirtualKey)
	{
		pButton->controlHeader.pfHandleVirtualKeyInput = BUTTON_HandleVirtualKeyInput;
	}

	pButton->controlHeader.field_34 = pCallback;
	pButton->nActivationVirtualKey = nActivationVirtualKey;
	pButton->dwButtonFlags = nFlags;
	pButton->nBaseFrame = nBaseFrame;

	if (nStringId)
	{
		D2LANG_strcpy(pButton->wszText, D2LANG_10004_GetStringFromTblIndex(nStringId));
	}

	const int32_t v18 = ((nWidth + 255) >> 8) * ((nHeight + 255) >> 8);
	pButton->nIsPressedFrameOffset = v18;
	if (pButton->dwButtonFlags & 0x20)
	{
		pButton->field_58 = 2 * v18;
	}

	pButton->pfUpdateHoverState = pfUpdateHoverState;

	CONTROL_AppendToList(&pButton->controlHeader);

	return pButton;
}

//D2Win.0x6F8A62C0 (#10108)
int32_t __fastcall BUTTON_Destroy(D2WinButtonStrc* pButton)
{
	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	return CONTROL_RemoveFromList(&pButton->controlHeader);
}

//D2Win.0x6F8A62F0 (#10109)
int32_t __fastcall BUTTON_IsPressed(D2WinButtonStrc* pButton)
{
	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	return pButton->bIsPressed && pButton->bIsCallbackPending ? 1 : 0;
}

//D2Win.0x6F8A6330 (#10110)
void __fastcall BUTTON_SetIsPressed(D2WinButtonStrc* pButton, int32_t bIsPressed)
{
	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	pButton->bIsPressed = bIsPressed;
	pButton->bIsCallbackPending = bIsPressed;
}

//D2Win.0x6F8A6360 (#10186)
int32_t __fastcall BUTTON_SetText(D2WinButtonStrc* pButton, Font eFont, const wchar_t* wszText, int32_t nTextColor)
{
	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	pButton->eFont = eFont;
	pButton->nTextColor = nTextColor;

	return D2LANG_strcpy(pButton->wszText, wszText);
}

//D2Win.0x6F8A63A0 (#10111)
int32_t __fastcall BUTTON_SetTextWithStringId(D2WinButtonStrc* pButton, Font eFont, uint16_t nStringId, int32_t nTextColor)
{
	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	pButton->eFont = eFont;
	pButton->nTextColor = nTextColor;

	return D2LANG_strcpy(pButton->wszText, D2LANG_10004_GetStringFromTblIndex(nStringId));
}

//D2Win.0x6F8A63F0 (#10112)
void __fastcall BUTTON_SetCoordinates(D2WinButtonStrc* pButton, int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight)
{
	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	pButton->controlHeader.nImageX = nX;
	pButton->controlHeader.nImageY = nY;
	pButton->controlHeader.nWidth = nWidth;
	pButton->controlHeader.nHeight = nHeight;
}

//D2Win.0x6F8A6430 (#10113)
void __fastcall BUTTON_SimulateClick(D2WinMsgStrc* pMsg)
{
	D2WinButtonStrc* pButton = (D2WinButtonStrc*)pMsg->hWnd;

	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	if (!pButton->controlHeader.pfHandleMouseDown)
	{
		return;
	}

	pButton->nActivationCounter = 1;
	pButton->controlHeader.pfHandleMouseDown(pMsg);

	if (!pButton->controlHeader.pfHandleMouseUp)
	{
		return;
	}

	D2WinMsgStrc msg = {};
	msg.hWnd = pMsg->hWnd;

	++dword_6F8FE254;
	pButton->controlHeader.pfHandleMouseUp(&msg);
	--dword_6F8FE254;
}

//D2Win.0x6F8A64A0 (#10114)
void __fastcall BUTTON_SetBaseFrame(D2WinButtonStrc* pButton, int32_t nBaseFrame)
{
	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	pButton->nBaseFrame = nBaseFrame;
}

//D2Win.0x6F8A64D0
int32_t __fastcall BUTTON_OnMouseWheelScrolled(D2WinButtonStrc* pButton, int32_t nMouseWheelIncrement)
{
	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	if (!(pButton->controlHeader.dwFlags & gdwBitMasks[0]) || !(pButton->controlHeader.dwFlags & gdwBitMasks[2]) || pButton->bIsPressed || pButton->bIsCallbackPending)
	{
		return 0;
	}

	if ((nMouseWheelIncrement <= 0 || !(pButton->dwButtonFlags & 8)) && (nMouseWheelIncrement >= 0 || !(pButton->dwButtonFlags & 0x10)))
	{
		return 0;
	}

	D2WinMsgStrc msg = {};
	msg.hWnd = (HWND)pButton;

	if (pButton->controlHeader.field_34)
	{
		++dword_6F8FE254;
		pButton->controlHeader.field_34(&msg);
		--dword_6F8FE254;
	}

	return 0;
}

//D2Win.0x6F8A6570 (#10189)
void __fastcall BUTTON_SetStringId(D2WinButtonStrc* pButton, uint16_t nStringId)
{
	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	pButton->nStringId = nStringId;
}

//D2Win.0x6F8A65A0
int32_t __fastcall BUTTON_Draw(D2WinControlStrc* pControl)
{
	D2WinButtonStrc* pButton = (D2WinButtonStrc*)pControl;

	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	int32_t nBaseFrame = pButton->nBaseFrame;
	if (!(pButton->controlHeader.dwFlags & gdwBitMasks[2]) || !(pButton->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		nBaseFrame += pButton->field_58;
	}
	else if (pButton->bIsPressed)
	{
		nBaseFrame += pButton->nIsPressedFrameOffset;
	}

	if (pButton->controlHeader.pCellFile && pButton->controlHeader.dwFlags & gdwBitMasks[2])
	{
		DrawMode eDrawMode;
		if (pButton->controlHeader.dwFlags & gdwBitMasks[0] || pButton->dwButtonFlags & 0x20)
		{
			eDrawMode = DRAWMODE_NORMAL;
		}
		else
		{
			eDrawMode = DRAWMODE_TRANS50;
		}

		const int32_t nBaseY = pButton->controlHeader.nImageY - pButton->controlHeader.nHeight + 1;
		const int32_t nColumns = (pButton->controlHeader.nWidth + 255) >> 8;
		const int32_t nRows = ((pButton->controlHeader.nHeight + 255) >> 8) - 1;

		D2GfxDataStrc gfxData = {};
		gfxData.pCellFile = pButton->controlHeader.pCellFile;
		gfxData.nDirection = 0;

		for (int32_t i = 0; i < nRows; ++i)
		{
			for (int32_t j = 0; j < nColumns; ++j)
			{
				gfxData.nFrame = nBaseFrame + j + 4 * i;
				TEXTURE_CelDraw(&gfxData, pButton->controlHeader.nImageX + 256 * j, nBaseY + 256 * (i + 1), -1, eDrawMode, 0);
			}
		}

		if (nRows >= 0)
		{
			for (int32_t i = 0; i < nColumns; ++i)
			{
				gfxData.nFrame = nBaseFrame + i + 4 * nRows;
				TEXTURE_CelDraw(&gfxData, pButton->controlHeader.nImageX + 256 * i, pButton->controlHeader.nImageY, -1, eDrawMode, 0);
			}
		}

		pButton->eFont = D2FONT_FONTRIDICULOUS;

		int32_t nOffsetY = 0;
		if (pButton->controlHeader.nHeight >= 35)
		{
			pButton->eFont = D2FONT_FONTEXOCET10;
			nOffsetY = 4;
		}
		else if (pButton->controlHeader.nHeight == 32)
		{
			nOffsetY = 3;
		}
		else if (pButton->controlHeader.nHeight > 20)
		{
			nOffsetY = 2;
		}
		else
		{
			nOffsetY = 1;
		}

		pButton->nTextColor = 0;

		if (pButton->wszText[0])
		{
			D2Win_10127_SetFont(pButton->eFont);

			const int32_t nTextHeight = D2Win_10125();

			const int32_t nOffsetX = std::max((pButton->controlHeader.nWidth - D2Win_10121_GetTextWidth(pButton->wszText)) / 2, 0);

			int32_t nX = nOffsetX + pButton->controlHeader.nImageX;
			int32_t nY = nOffsetY + pButton->controlHeader.nImageY - (pButton->controlHeader.nHeight - nTextHeight) / 2;
			if (pButton->bIsPressed && pButton->bIsCallbackPending || pButton->nActivationCounter)
			{
				nX -= 2;
				nY += 2;
			}

			if (pButton->dwButtonFlags & 0x40)
			{
				nX = nOffsetX + pButton->controlHeader.nImageX;

				const wchar_t* wszText = D2LANG_10004_GetStringFromTblIndex(pButton->nStringId);

				int32_t nTextX = std::max((pButton->controlHeader.nWidth - D2Win_10121_GetTextWidth(wszText)) / 2, 0) + pButton->controlHeader.nImageX;

				nY = nOffsetY + pButton->controlHeader.nImageY - 2 * (pButton->controlHeader.nHeight - nTextHeight) / 3;
				int32_t nTextY = nOffsetY + pButton->controlHeader.nImageY - (pButton->controlHeader.nHeight - nTextHeight) / 3;
				if (pButton->bIsPressed && pButton->bIsCallbackPending || pButton->nActivationCounter)
				{
					nX -= 2;
					nY += 2;
					nTextX -= 2;
					nTextY += 2;
				}

				D2Win_10118_DrawBlendedText(wszText, nTextX, nTextY, pButton->nTextColor, 0, DRAWMODE_BURN);
			}

			D2Win_10118_DrawBlendedText(pButton->wszText, nX, nY, pButton->nTextColor, 0, DRAWMODE_BURN);
		}
	}

	if (pButton->nActivationCounter == 2)
	{
		pButton->nActivationCounter = 0;
		pButton->bIsCallbackPending = 1;
	}
	else if (pButton->nActivationCounter != 0)
	{
		++pButton->nActivationCounter;
	}

	if (pButton->pfUpdateHoverState)
	{
		if (sub_6F8AE540())
		{
			pButton->isButtonHovered = 0;
		}
		else
		{
			pButton->isButtonHovered = gMousePosition_6F8FE234.x >= pButton->controlHeader.nImageX
				&& gMousePosition_6F8FE234.y >= pButton->controlHeader.nImageY - pButton->controlHeader.nHeight
				&& gMousePosition_6F8FE234.x < pButton->controlHeader.nImageX + pButton->controlHeader.nWidth
				&& gMousePosition_6F8FE234.y < pButton->controlHeader.nImageY;
		}

		D2WinMsgStrc msg = {};
		msg.hWnd = (HWND)pButton;
		msg.lParam = pButton->isButtonHovered;

		++dword_6F8FE254;
		pButton->pfUpdateHoverState(&msg);
		--dword_6F8FE254;
	}

	return 1;
}

//D2Win.0x6F8A6A10
int32_t __stdcall BUTTON_HandleMouseDown(D2WinMsgStrc* pMsg)
{
	D2WinButtonStrc* pButton = (D2WinButtonStrc*)pMsg->hWnd;

	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	if (!(pButton->controlHeader.dwFlags & gdwBitMasks[2]) || !(pButton->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	if (pButton->dwButtonFlags & BUTTONFLAG_IS_TOGGLE)
	{
		pButton->bIsPressed = pButton->bIsPressed == 0;
		pButton->bIsCallbackPending = pButton->bIsCallbackPending == 0;
	}
	else
	{
		if (pButton->bIsPressed && pButton->bIsCallbackPending)
		{
			return 1;
		}

		pButton->bIsPressed = 1;
		pButton->bIsCallbackPending = 1;
	}

	if (!(pButton->dwButtonFlags & BUTTONFLAG_SKIP_SOUND))
	{
		D2SOUND_10039("data\\global\\sfx\\cursor\\button.wav");
	}

	if (pButton->dwButtonFlags & (BUTTONFLAG_IS_TOGGLE | 1) && pButton->controlHeader.field_34)
	{
		D2_ASSERT(dword_6F8FE254 > 0);

		pButton->controlHeader.field_34(pMsg);
	}

	return 1;
}

//D2Win.0x6F8A6AF0
int32_t __stdcall BUTTON_HandleMouseUp(D2WinMsgStrc* pMsg)
{
	D2WinButtonStrc* pButton = (D2WinButtonStrc*)pMsg->hWnd;

	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	if (!(pButton->controlHeader.dwFlags & gdwBitMasks[2]) || !(pButton->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}


	if (pButton->dwButtonFlags & (BUTTONFLAG_IS_TOGGLE | 1))
	{
		return 0;
	}

	pButton->controlHeader.dwFlags |= gdwBitMasks[1];

	pButton->bIsPressed = 0;

	int32_t nResult = 0;
	if (pButton->bIsCallbackPending && pButton->controlHeader.field_34)
	{
		D2_ASSERT(dword_6F8FE254 > 0);

		nResult = pButton->controlHeader.field_34(pMsg);
	}

	pButton->bIsCallbackPending = 0;

	pButton->controlHeader.dwFlags &= gdwInvBitMasks[1];
	return nResult;
}

//D2Win.0x6F8A6BB0
int32_t __stdcall BUTTON_HandleVirtualKeyInput(D2WinMsgStrc* pMsg)
{
	D2WinButtonStrc* pButton = (D2WinButtonStrc*)pMsg->hWnd;

	D2_ASSERT(pButton->controlHeader.nType == D2WIN_BUTTON);

	if (pMsg->uMessage == WM_CHAR || pButton->nActivationVirtualKey != pMsg->wParam)
	{
		return 0;
	}

	if (!pButton->controlHeader.pfHandleMouseDown)
	{
		return 0;
	}

	pButton->nActivationCounter = 1;

	D2_ASSERT(dword_6F8FE254 > 0);

	const int32_t nResult = pButton->controlHeader.pfHandleMouseDown(pMsg);

	if (!pButton->controlHeader.pfHandleMouseUp)
	{
		return nResult;
	}

	D2WinMsgStrc msg = {};
	msg.hWnd = pMsg->hWnd;

	++dword_6F8FE254;
	pButton->controlHeader.pfHandleMouseUp(&msg);
	--dword_6F8FE254;
	return nResult;
}
