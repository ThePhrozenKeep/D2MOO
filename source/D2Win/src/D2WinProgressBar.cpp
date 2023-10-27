#include "D2WinProgressBar.h"

#include <D2Gfx.h>

#include <Fog.h>
#include <D2BitManip.h>

#include "D2WinMain.h"
#include "D2WinPalette.h"


//D2Win.0x6F8AEED0 (#10091)
D2WinProgressBarStrc* __fastcall PROGRESSBAR_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight)
{
	D2WinProgressBarStrc* pProgressBar = D2_CALLOC_STRC(D2WinProgressBarStrc);

	pProgressBar->controlHeader.nType = D2WIN_PROGRESSBAR;
	pProgressBar->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pProgressBar->controlHeader.nImageX = nX;
	pProgressBar->controlHeader.nImageY = nY;
	pProgressBar->controlHeader.nWidth = nWidth;
	pProgressBar->controlHeader.nHeight = nHeight;
	pProgressBar->controlHeader.pfDraw = PROGRESSBAR_Draw;

	CONTROL_AppendToList(&pProgressBar->controlHeader);

	return pProgressBar;
}

//D2Win.0x6F8B1BF0 (#10092)
int32_t __fastcall PROGRESSBAR_Destroy(D2WinControlStrc* pControl)
{
	return CONTROL_RemoveFromList(pControl);
}

//D2Win.0x6F8AEF70 (#10093)
int32_t __stdcall PROGRESSBAR_SetProgress(D2WinProgressBarStrc* pProgressBar, float fProgress)
{
	D2_ASSERT(pProgressBar->controlHeader.nType == D2WIN_PROGRESSBAR);

	pProgressBar->fProgress = fProgress;
	return 1;
}

//D2Win.0x6F8AEFB0
int32_t __fastcall PROGRESSBAR_Draw(D2WinControlStrc* pControl)
{
	D2WinProgressBarStrc* pProgressBar = (D2WinProgressBarStrc*)pControl;

	D2_ASSERT(pProgressBar->controlHeader.nType == D2WIN_PROGRESSBAR);

	if (!(pProgressBar->controlHeader.dwFlags & gdwBitMasks[2]) || !(pProgressBar->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	D2GFX_DrawBox(
		pProgressBar->controlHeader.nImageX,
		pProgressBar->controlHeader.nImageY,
		pProgressBar->controlHeader.nImageX + (int32_t)(pProgressBar->controlHeader.nWidth * pProgressBar->fProgress),
		pProgressBar->controlHeader.nImageY + pProgressBar->controlHeader.nHeight,
		D2Win_10034_MixRGB(0xFFu, 0, 0),
		DRAWMODE_NORMAL
	);
	return 1;
}
