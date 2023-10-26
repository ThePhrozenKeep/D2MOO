#include "Texture.h"

#include "D2Gfx.h"


//D2Gfx.0x6FA74270 (#10071)
void __stdcall TEXTURE_CelFlatSpriteDraw(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, uint32_t dwGamma, DrawMode eDrawMode, int32_t nScreenMode, uint8_t* pPalette)
{
    return gpGraphicsInterface->pfCelFlatSpriteDraw(pData, nXPos, nYPos, dwGamma, eDrawMode, nScreenMode, pPalette);
}

//D2Gfx.0x6FA742A0 (#10072)
void __stdcall TEXTURE_CelDraw(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, uint32_t dwGamma, DrawMode eDrawMode, uint8_t* pPalette)
{
    return gpGraphicsInterface->pfCelDraw(pData, nXPos, nYPos, dwGamma, eDrawMode, pPalette);
}

//D2Gfx.0x6FA742D0 (#10073)
void __stdcall TEXTURE_CelDrawColor(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, uint32_t dwGamma, DrawMode eDrawMode, int32_t nGlobalPaletteShift)
{
    return gpGraphicsInterface->pfCelDrawColor(pData, nXPos, nYPos, dwGamma, eDrawMode, nGlobalPaletteShift);
}

//D2Gfx.0x6FA74300 (#10074)
void __stdcall TEXTURE_CelDrawEx(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, int32_t nSkipLines, int32_t nDrawLines, DrawMode eDrawMode)
{
    if (nDrawLines)
    {
        return gpGraphicsInterface->pfCelDrawEx(pData, nXPos, nYPos, nSkipLines, nDrawLines, eDrawMode);
    }
}

//D2Gfx.0x6FA74330 (#10077)
void __stdcall TEXTURE_CelDrawClipped(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, void* pCropRect, DrawMode eDrawMode)
{
    return gpGraphicsInterface->pfCelDrawClipped(pData, nXPos, nYPos, pCropRect, eDrawMode);
}

//D2Gfx.0x6FA74360 (#10075)
void __stdcall TEXTURE_CelDrawShadow(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos)
{
    return gpGraphicsInterface->pfCelDrawShadow(pData, nXPos + 2, nYPos);
}

//D2Gfx.0x6FA74380 (#10076)
void __stdcall TEXTURE_CelDrawHilight(D2GfxDataStrc* pData, int32_t nXPos, int32_t nYPos, uint8_t nPaletteIndex)
{
    return gpGraphicsInterface->pfCelDrawHilight(pData, nXPos, nYPos, nPaletteIndex);
}

//D2Gfx.0x6FA743A0 (#10078)
void __stdcall TEXTURE_DebugFillBackBuffer(int32_t nXPos, int32_t nYPos)
{
    return gpGraphicsInterface->pfDebugFillBackBuffer(nXPos, nYPos);
}
