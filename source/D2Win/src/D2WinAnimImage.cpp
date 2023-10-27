#include "D2WinAnimImage.h"

#include <algorithm>

#include <D2CMP.h>

#include <D2Gfx.h>
#include <Texture.h>

#include <D2BitManip.h>
#include <Fog.h>

#include "D2WinMain.h"


#pragma warning (disable : 28159)


//D2Win.0x6F8A53B0 (#10102)
D2WinAnimImageStrc* __fastcall ANIMIMAGE_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, D2CellFileStrc* pCellFile, uint32_t nAnimSpeed, int32_t(__stdcall* a7)(SMSGHANDLER_PARAMS*), D2AnimatedImageDescriptor* pDescriptor, DrawMode eDrawMode, void(__stdcall* a10)(SMSGHANDLER_PARAMS*))
{
	D2WinAnimImageStrc* pAnimImage = D2_CALLOC_STRC(D2WinAnimImageStrc);

	pAnimImage->controlHeader.nType = D2WIN_IMAGE;
	pAnimImage->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pAnimImage->controlHeader.pCellFile = pCellFile;
	pAnimImage->controlHeader.nImageX = nX;
	pAnimImage->controlHeader.nImageY = nY;
	pAnimImage->controlHeader.nWidth = nWidth;
	pAnimImage->controlHeader.nHeight = nHeight;
	pAnimImage->controlHeader.pfDraw = ANIMIMAGE_Draw;
	pAnimImage->controlHeader.pfHandleMouseDown = ANIMIMAGE_HandleMouseDown;
	pAnimImage->controlHeader.field_34 = a7;
	pAnimImage->pAnimatedImageDescriptor = pDescriptor;
	pAnimImage->nAnimSpeed = nAnimSpeed;
	pAnimImage->dwStartTickCount = GetTickCount();

	if (pDescriptor)
	{
		pAnimImage->controlHeader.pfShouldMouseInputBeHandled = ANIMIMAGE_ShouldMouseInputBeHandled;
	}

	if (eDrawMode)
	{
		pAnimImage->eDrawMode = eDrawMode;
	}
	else
	{
		pAnimImage->eDrawMode = DRAWMODE_NORMAL;
	}

	pAnimImage->field_58 = a10;
	pAnimImage->bIsRunning = 1;

	CONTROL_AppendToList(&pAnimImage->controlHeader);

	return pAnimImage;
}

//D2Win.0x6F8B1BF0 (#10103)
int32_t __fastcall ANIMIMAGE_Destroy(D2WinControlStrc* pControl)
{
	return CONTROL_RemoveFromList(pControl);
}

//D2Win.0x6F8A54A0 (#10106)
int32_t __fastcall D2Win_10106(D2WinAnimImageStrc* pAnimImage)
{
	D2_ASSERT(pAnimImage->controlHeader.nType == D2WIN_IMAGE);

	return pAnimImage->nAnimType;
}

//D2Win.0x6F8A54D0 (#10104)
int32_t __fastcall D2Win_10104(D2WinAnimImageStrc* pAnimImage)
{
	D2_ASSERT(pAnimImage->controlHeader.nType == D2WIN_IMAGE);

	if (pAnimImage->nAnimType == 3)
	{
		pAnimImage->nAnimType = 4;
		pAnimImage->nFrame = 0;
		pAnimImage->dwStartTickCount = GetTickCount();
		return 1;
	}
	else
	{
		return pAnimImage->nAnimType != 2 && pAnimImage->nAnimType != 4;
	}
}

//D2Win.0x6F8A5530
int32_t __fastcall ANIMIMAGE_ShouldMouseInputBeHandled(D2WinControlStrc* pControl)
{
	D2WinAnimImageStrc* pAnimImage = (D2WinAnimImageStrc*)pControl;

	D2_ASSERT(pAnimImage->controlHeader.nType == D2WIN_IMAGE);

	D2GfxDataStrc gfxData = {};
	
	D2CellFileStrc** ppCellFile = pAnimImage->pAnimatedImageDescriptor[pAnimImage->nAnimType].ppCellFile1;
	if (ppCellFile)
	{
		gfxData.pCellFile = *ppCellFile;
	}

	D2CellFileStrc* pGfxCell = D2CMP_CelGetHandle(&gfxData);

	const int32_t nX = gMousePosition_6F8FE234.x - D2CMP_CelGetOffsetX(pGfxCell);
	const int32_t nY = gMousePosition_6F8FE234.y - D2CMP_CelGetOffsetY(pGfxCell);

	return nX >= pAnimImage->controlHeader.nImageX && nY >= pAnimImage->controlHeader.nImageY - D2CMP_CelGetHeight(pGfxCell) 
		&& nX < pAnimImage->controlHeader.nImageX + D2CMP_CelGetWidth(pGfxCell) && nY < pAnimImage->controlHeader.nImageY;
}

//D2Win.0x6F8A5610
int32_t __fastcall ANIMIMAGE_Draw(D2WinControlStrc* pControl)
{
	D2WinAnimImageStrc* pAnimImage = (D2WinAnimImageStrc*)pControl;

	//TODO: Names
	D2_ASSERT(pAnimImage->controlHeader.nType == D2WIN_IMAGE);

	if (!pAnimImage->controlHeader.pCellFile && !pAnimImage->pAnimatedImageDescriptor)
	{
		return 1;
	}

	D2GfxDataStrc gfxData = {};
	gfxData.nDirection = 0;

	int32_t nX = pAnimImage->controlHeader.nImageX;
	int32_t nY = pAnimImage->controlHeader.nImageY;

	if (pAnimImage->pAnimatedImageDescriptor)
	{
		const int32_t nAnimType = pAnimImage->nAnimType;
		if (nAnimType == 0 || nAnimType == 1)
		{
			const int32_t v5 = ANIMIMAGE_ShouldMouseInputBeHandled(pControl) != 0;
			pAnimImage->nAnimType = v5;
			if (nAnimType != v5)
			{
				SMSGHANDLER_PARAMS msg = {};
				msg.hWindow = (HWND)pAnimImage;
				msg.wParam = v5 == 1;

				if (pAnimImage->field_58)
				{
					pAnimImage->field_58(&msg);
				}
			}
		}

		int32_t nMaxFrames = 1;
		D2CellFileStrc** ppCellFile = pAnimImage->pAnimatedImageDescriptor[pAnimImage->nAnimType].ppCellFile1;
		if (ppCellFile && *ppCellFile)
		{
			nMaxFrames = D2CMP_CelFileGetCelsPerDirection(*ppCellFile);
		}

		//const int32_t v12 = pAnimImage->nAnimType;
		if (pAnimImage->nAnimType != 2)
		{
			if (pAnimImage->nAnimType == 4 && pAnimImage->nFrame >= nMaxFrames - 2)
			{
				pAnimImage->nAnimType = 1;
				pAnimImage->nFrame = 0;
				pAnimImage->dwStartTickCount = GetTickCount();
			}
		}
		else
		{
			if (pAnimImage->nFrame >= nMaxFrames - 2)
			{
				pAnimImage->nAnimType = 3;
				pAnimImage->nFrame = 0;
				pAnimImage->dwStartTickCount = GetTickCount();
			}
		}

		if (pAnimImage->nFrame > nMaxFrames)
		{
			pAnimImage->nFrame = 0;
			pAnimImage->dwStartTickCount = GetTickCount();
		}
	}

	D2CellFileStrc* pCellFile1 = nullptr;
	D2CellFileStrc* pCellFile2 = nullptr;
	DrawMode eDrawMode;
	uint32_t nAnimSpeed = 0;

	if (pAnimImage->pAnimatedImageDescriptor)
	{
		D2AnimatedImageDescriptor* pDescriptor = &pAnimImage->pAnimatedImageDescriptor[pAnimImage->nAnimType];

		if (pDescriptor->ppCellFile1)
		{
			pCellFile1 = *pDescriptor->ppCellFile1;
		}

		if (pDescriptor->ppCellFile2)
		{
			pCellFile2 = *pDescriptor->ppCellFile2;
		}

		eDrawMode = pDescriptor->eDrawMode;
		nAnimSpeed = pDescriptor->nAnimSpeed;
	}
	else
	{
		pCellFile1 = pAnimImage->controlHeader.pCellFile;
		gfxData.pCellFile = pCellFile1;
		gfxData.nDirection = 0;
		gfxData.nFrame = 0;

		D2CellFileStrc* v26 = D2CMP_CelGetHandle(&gfxData);
		nX -= D2CMP_CelGetOffsetX(v26);
		nY -= D2CMP_CelGetOffsetY(v26);
		eDrawMode = pAnimImage->eDrawMode;
		nAnimSpeed = pAnimImage->nAnimSpeed;
	}

	const uint32_t nFrameCount = D2CMP_CelFileGetCelsPerDirection(pCellFile1);

	if (pCellFile1)
	{
		gfxData.pCellFile = pCellFile1;
		gfxData.nFrame = pAnimImage->nFrame;
		TEXTURE_CelDraw(&gfxData, nX, nY, -1, pAnimImage->eDrawMode, 0); // TODO: Should probably be eDrawMode instead of pAnimImage->pDrawMode
	}

	if (pCellFile2)
	{
		gfxData.pCellFile = pCellFile2;
		gfxData.nFrame = pAnimImage->nFrame;
		TEXTURE_CelDraw(&gfxData, nX, nY, -1, eDrawMode, 0);
	}

	if (nAnimSpeed && nFrameCount > 1 && pAnimImage->bIsRunning)
	{
		const uint32_t nFrameDivisor = 1000 * nAnimSpeed / 25;
		if (pAnimImage->nAnimType != 2 && pAnimImage->nAnimType != 4)
		{
			pAnimImage->nFrame = (GetTickCount() - pAnimImage->dwStartTickCount) / nFrameDivisor % (nFrameCount - 1);
		}
		else
		{
			pAnimImage->nFrame = std::min(((uint32_t)GetTickCount() - pAnimImage->dwStartTickCount) / nFrameDivisor, nFrameCount - 1);
		}
	}

	return 1;
}

//D2Win.0x6F8A5900
int32_t __stdcall ANIMIMAGE_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinAnimImageStrc* pAnimImage = (D2WinAnimImageStrc*)pMsg->hWindow;

	D2_ASSERT(pAnimImage->controlHeader.nType == D2WIN_IMAGE);

	if (!pAnimImage->controlHeader.field_34)
	{
		return 0;
	}

	const int32_t nResult = pAnimImage->controlHeader.field_34(pMsg);
	if (!nResult)
	{
		return 0;
	}

	if (pAnimImage->nAnimType == 3)
	{
		pAnimImage->nAnimType = 4;
	}
	else if (pAnimImage->nAnimType == 1)
	{
		pAnimImage->nAnimType = 2;
	}

	pAnimImage->nFrame = 0;
	pAnimImage->dwStartTickCount = GetTickCount();

	return nResult;
}

//D2Win.0x6F8A5980 (#10105)
int32_t __stdcall ANIMIMAGE_SetIsRunning(D2WinAnimImageStrc* pAnimImage, int32_t bIsRunning)
{
	D2_ASSERT(pAnimImage->controlHeader.nType == D2WIN_IMAGE);

	pAnimImage->bIsRunning = bIsRunning;
	return 1;
}
