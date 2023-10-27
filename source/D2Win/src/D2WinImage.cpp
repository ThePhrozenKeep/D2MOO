#include "D2WinImage.h"

#include <algorithm>

#include <D2Math.h>

#include <D2CMP.h>

#include <D2Gfx.h>
#include <Texture.h>

#include <Fog.h>
#include <D2BitManip.h>

#include "D2WinMain.h"


#pragma pack(push, 1)
struct D2CropRectStrc
{
	int32_t unk0x00;
	int32_t unk0x04;
	int32_t unk0x08;
	int32_t unk0x0C;
};
#pragma pack(pop)


//D2Win.0x6F8ABA90 (#10084)
D2WinImage2Strc* __fastcall IMAGE2_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, D2CellFileStrc* pCellFile, int32_t(__stdcall* a6)(SMSGHANDLER_PARAMS*), D2WinImageClickRectStrc* pClickRect, int32_t(__stdcall* pfHandleVirtualKeyInput)(SMSGHANDLER_PARAMS*))
{
	D2WinImage2Strc* pImage = D2_CALLOC_STRC(D2WinImage2Strc);

	pImage->controlHeader.nType = D2WIN_IMAGE;
	pImage->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pImage->controlHeader.pCellFile = pCellFile;
	pImage->controlHeader.nImageX = nX;
	pImage->controlHeader.nImageY = nY;
	pImage->controlHeader.nWidth = nWidth;
	pImage->controlHeader.nHeight = nHeight;
	pImage->controlHeader.pfDraw = IMAGE2_Draw;
	pImage->controlHeader.pfHandleMouseDown = IMAGE2_HandleMouseDown;
	pImage->controlHeader.pfHandleVirtualKeyInput = pfHandleVirtualKeyInput;
	pImage->controlHeader.field_34 = a6;
	pImage->eDrawMode = DRAWMODE_NORMAL;
	pImage->pClickRect = pClickRect;

	CONTROL_AppendToList(&pImage->controlHeader);

	return pImage;
}

//D2Win.0x6F8B1BF0 (#10085)
int32_t __fastcall IMAGE_Destroy(D2WinControlStrc* pControl)
{
	return CONTROL_RemoveFromList(pControl);
}

//D2Win.0x6F8ABB40
D2WinImageStrc* __fastcall IMAGE_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, D2CellFileStrc* pCellFile, int32_t a6, int32_t a7)
{
	D2WinImageStrc* pImage = D2_CALLOC_STRC(D2WinImageStrc);

	pImage->controlHeader.nType = D2WIN_IMAGE;
	pImage->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pImage->controlHeader.pCellFile = pCellFile;
	pImage->controlHeader.nImageX = nX;
	pImage->controlHeader.nImageY = nY;
	pImage->controlHeader.nWidth = nWidth;
	pImage->controlHeader.nHeight = nHeight;
	pImage->controlHeader.pfDraw = IMAGE_Draw;
	pImage->controlHeader.pfHandleMouseDown = IMAGE2_HandleMouseDown;
	pImage->eDrawMode = DRAWMODE_NORMAL;
	pImage->field_4C = a6;
	pImage->field_50 = a7;

	CONTROL_AppendToList(&pImage->controlHeader);

	return pImage;
}

//D2Win.0x6F8ABC00 (#10086)
int32_t __stdcall IMAGE_SetCellFile(D2WinImageStrc* pImage, D2CellFileStrc* pCellFile, D2WinImageClickRectStrc* pClickRect)
{
	D2_ASSERT(pImage->controlHeader.nType == D2WIN_IMAGE);

	pImage->controlHeader.pCellFile = pCellFile;
	pImage->pClickRect = pClickRect;
	return 1;
}

//D2Win.0x6F8ABC40 (#10087)
int32_t __stdcall IMAGE_SetCoordinates(D2WinImageStrc* pImage, int32_t nX, int32_t nY)
{
	D2_ASSERT(pImage->controlHeader.nType == D2WIN_IMAGE);

	pImage->controlHeader.nImageX = nX;
	pImage->controlHeader.nImageY = nY;
	return 1;
}

//D2Win.0x6F8ABC80 (#10088)
int32_t __stdcall IMAGE_SetFrame(D2WinImageStrc* pImage, int32_t nFrame)
{
	D2_ASSERT(pImage->controlHeader.nType == D2WIN_IMAGE);

	const int32_t nMaxFrames = D2CMP_CelFileGetCelsPerDirection(pImage->controlHeader.pCellFile) - 1;
	pImage->nFrame = D2Clamp(nFrame, 0, nMaxFrames);
	return 1;
}

//D2Win.0x6F8ABCD0 (#10089)
int32_t __stdcall IMAGE_ForceNormalDrawMode(D2WinImageStrc* pImage, int32_t nUnused)
{
	D2_ASSERT(pImage->controlHeader.nType == D2WIN_IMAGE);

	pImage->eDrawMode = DRAWMODE_NORMAL;
	return 1;
}

//D2Win.0x6F8ABD10 (#10090)
int32_t __stdcall IMAGE_GetFrameCount(D2WinImageStrc* pImage)
{
	D2_ASSERT(pImage->controlHeader.nType == D2WIN_IMAGE);

	return D2CMP_CelFileGetCelsPerDirection(pImage->controlHeader.pCellFile);
}

//D2Win.0x6F8ABD50
int32_t __fastcall IMAGE2_Draw(D2WinControlStrc* pControl)
{
	D2WinImageStrc* pImage = (D2WinImageStrc*)pControl;

	D2_ASSERT(pImage->controlHeader.nType == D2WIN_IMAGE);

	if (!(pImage->controlHeader.dwFlags & gdwBitMasks[2]) || !(pImage->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	if (!pImage->controlHeader.pCellFile)
	{
		return 1;
	}

	const int32_t nBaseY = pImage->controlHeader.nImageY - pImage->controlHeader.nHeight + 1;
	const int32_t nColumns = (pImage->controlHeader.nWidth + 255) >> 8;
	const int32_t nRows = ((pImage->controlHeader.nHeight + 255) >> 8) - 1;

	D2GfxDataStrc gfxData = {};

	gfxData.pCellFile = pImage->controlHeader.pCellFile;
	gfxData.nDirection = 0;
	gfxData.nFrame = pImage->nFrame;

	for (int32_t i = 0; i < nRows; ++i)
	{
		for (int32_t j = 0; j < nColumns; ++j)
		{
			TEXTURE_CelDraw(&gfxData, pImage->controlHeader.nImageX + 256 * j, nBaseY + 256 * (i + 1), -1, pImage->eDrawMode, 0);
			++gfxData.nFrame;
		}
	}

	if (nRows >= 0)
	{
		for (int32_t i = 0; i < nColumns; ++i)
		{
			TEXTURE_CelDraw(&gfxData, pImage->controlHeader.nImageX + 256 * i, pImage->controlHeader.nImageY, -1, pImage->eDrawMode, 0);
			++gfxData.nFrame;
		}
	}

	return 1;
}

//D2Win.0x6F8ABEA0
int32_t __fastcall IMAGE_Draw(D2WinControlStrc* pControl)
{
	D2WinImageStrc* pImage = (D2WinImageStrc*)pControl;

	//TODO: Names
	D2_ASSERT(pImage->controlHeader.nType == D2WIN_IMAGE);

	D2GfxDataStrc gfxData = {};
	gfxData.pCellFile = pImage->controlHeader.pCellFile;
	gfxData.nDirection = 0;
	gfxData.nFrame = 0;

	const int32_t v51[4] =
	{ 
		pImage->controlHeader.nImageY - pImage->controlHeader.nHeight,
		pImage->controlHeader.nImageY - pImage->controlHeader.nHeight + pImage->field_50,
		pImage->controlHeader.nImageY - pImage->field_50,
		pImage->controlHeader.nImageY
	};

	const int32_t v55[4] =
	{
		pImage->controlHeader.nImageX,
		pImage->controlHeader.nImageX + pImage->field_4C,
		pImage->controlHeader.nImageX + pImage->controlHeader.nWidth - pImage->field_4C,
		pImage->controlHeader.nImageX + pImage->controlHeader.nWidth
	};

	const int32_t v59[4] =
	{
		0,
		pImage->field_4C,
		256 - pImage->field_4C,
		256
	};

	const int32_t v63[4] =
	{
		0,
		pImage->field_50,
		256 - pImage->field_50,
		256
	};

	D2CropRectStrc v37 = {};
	D2CropRectStrc v41 = {};

	for (int32_t v10 = 0; v10 < 3; ++v10)
	{
		const int32_t v45 = v51[v10 + 1];
		const int32_t v47 = v51[v10];
		const int32_t v49 = v63[v10];
		const int32_t v50 = v63[v10 + 1];
		const int32_t v31 = v50 - v49;

		for (int32_t v6 = 0; v6 < 3; ++v6)
		{
			int32_t v15 = v55[v6];
			int32_t v16 = v47;
			const int32_t v48 = v59[v6];
			const int32_t v18 = v59[v6 + 1] - v48;
			v37.unk0x04 = v55[v6 + 1];
			const int32_t v19 = v37.unk0x04 - v15;
			const int32_t v20 = v45 - v47;
			v37.unk0x00 = v15;
			v37.unk0x08 = v47;
			v37.unk0x0C = v45;

			if (v18 == v19 && v31 == v20)
			{
				TEXTURE_CelDrawClipped(&gfxData, v15 - v48, v45 - v50 + 256, &v37, DRAWMODE_NORMAL);
			}
			else
			{
				const int32_t v46 = (v19 + v18 - 1) / v18;
				int32_t v21 = v47 - v49 + 256;
				const int32_t v36 = (v20 + v31 - 1) / v31;
				int32_t v30 = v49 + v21 - 256;
				for (int32_t j = 0; j < v36; ++j)
				{
					int32_t v22 = v15 - v48;
					int32_t v23 = v15;
					for (int32_t i = 0; i < v46; ++i)
					{
						if (v15 <= v23)
						{
							v15 = v23;
						}

						v41.unk0x00 = v15;
						v41.unk0x04 = std::min(v37.unk0x04, v15 + v18);
						v41.unk0x08 = std::max(v16, v30);
						v41.unk0x0C = std::min(v37.unk0x0C, v31 + v41.unk0x08);

						TEXTURE_CelDrawClipped(&gfxData, v22, v21, &v41, DRAWMODE_NORMAL);

						v16 = v37.unk0x08;
						v22 += v18;
						v23 += v18;
						v15 = v37.unk0x00;
					}

					v21 += v31;
					v30 += v31;
				}
			}
		}
	}

	return 1;
}

//D2Win.0x6F8AC170
int32_t __stdcall IMAGE2_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg)
{
	const D2WinImage2Strc* pImage = (const D2WinImage2Strc*)pMsg->hWindow;

	D2_ASSERT(pImage->controlHeader.nType == D2WIN_IMAGE);

	if (!(pImage->controlHeader.dwFlags & gdwBitMasks[2]) || !(pImage->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	const int32_t nX = gMousePosition_6F8FE234.x - pImage->controlHeader.nImageX;
	const int32_t nY = gMousePosition_6F8FE234.y + pImage->controlHeader.nHeight - pImage->controlHeader.nImageY;

	const D2WinImageClickRectStrc* pClickRect = pImage->pClickRect;
	while (pClickRect && pClickRect->pCallback)
	{
		if (nX >= pClickRect->nX && nY >= pClickRect->nY - pClickRect->nHeight && nX < pClickRect->nX + pClickRect->nWidth && nY < pClickRect->nY)
		{
			return pClickRect->pCallback(pMsg);
		}

		++pClickRect;
	}

	if (pImage->controlHeader.field_34)
	{
		return pImage->controlHeader.field_34(pMsg);
	}

	return 0;
}
