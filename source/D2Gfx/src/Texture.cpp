#include "Texture.h"

#include "D2Gfx.h"


//D2Gfx.0x6FA74270 (#10071)
void __stdcall TEXTURE_DrawPerspectiveImage(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, uint32_t dwGamma, DrawMode eDrawMode, int32_t nScreenMode, uint8_t* pPalette)
{
    return gpRenderCallbacks->pfDrawPerspectiveImage(pData, nXPos, nYPos, dwGamma, eDrawMode, nScreenMode, pPalette);
}

//D2Gfx.0x6FA742A0 (#10072)
void __stdcall TEXTURE_DrawImage(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, uint32_t dwGamma, DrawMode eDrawMode, uint8_t* pPalette)
{
    return gpRenderCallbacks->pfDrawImage(pData, nXPos, nYPos, dwGamma, eDrawMode, pPalette);
}

//D2Gfx.0x6FA742D0 (#10073)
void __stdcall TEXTURE_DrawShiftedImage(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, uint32_t dwGamma, DrawMode eDrawMode, int32_t nGlobalPaletteShift)
{
    return gpRenderCallbacks->pfDrawShiftedImage(pData, nXPos, nYPos, dwGamma, eDrawMode, nGlobalPaletteShift);
}

//D2Gfx.0x6FA74300 (#10074)
void __stdcall TEXTURE_DrawVerticalCropImage(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, int32_t nSkipLines, int32_t nDrawLines, DrawMode eDrawMode)
{
    if (nDrawLines)
    {
        return gpRenderCallbacks->pfDrawVerticalCropImage(pData, nXPos, nYPos, nSkipLines, nDrawLines, eDrawMode);
    }
}

//D2Gfx.0x6FA74330 (#10077)
void __stdcall TEXTURE_DrawClippedImage(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, void* pCropRect, DrawMode eDrawMode)
{
    return gpRenderCallbacks->pfDrawClippedImage(pData, nXPos, nYPos, pCropRect, eDrawMode);
}

//D2Gfx.0x6FA74360 (#10075)
void __stdcall TEXTURE_DrawShadow(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos)
{
    return gpRenderCallbacks->pfDrawShadow(pData, nXPos + 2, nYPos);
}

//D2Gfx.0x6FA74380 (#10076)
void __stdcall TEXTURE_DrawImageFast(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, uint8_t nPaletteIndex)
{
    return gpRenderCallbacks->pfDrawImageFast(pData, nXPos, nYPos, nPaletteIndex);
}

//D2Gfx.0x6FA743A0 (#10078)
void __stdcall TEXTURE_DebugFillBackBuffer(int32_t nXPos, int32_t nYPos)
{
    return gpRenderCallbacks->pfDebugFillBackBuffer(nXPos, nYPos);
}
