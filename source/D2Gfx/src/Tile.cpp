#include "Tile.h"

#include "D2Gfx.h"


//D2Gfx.0x6FA743C0 (#10080)
int32_t __stdcall TILE_TileDrawLit(D2TileLibraryEntryStrc* pTile, int32_t nXPos, int32_t nYPos, D2GfxLightStrc* pLight, int32_t nScreenPanels)
{
    return gpGraphicsInterface->pfTileDrawLit(pTile, nXPos, nYPos, pLight, nScreenPanels);
}

//D2Gfx.0x6FA743F0 (#10081)
int32_t __stdcall TILE_TileDrawTrans(D2TileLibraryEntryStrc* pTile, int32_t nXPos, int32_t nYPos, D2GfxLightStrc* pLight, int32_t nScreenPanels, uint8_t nAlpha)
{
    return gpGraphicsInterface->pfTileDrawTrans(pTile, nXPos, nYPos, pLight, nScreenPanels, nAlpha);
}

//D2Gfx.0x6FA74420 (#10082)
int32_t __stdcall TILE_ShadowTileDraw(D2TileLibraryEntryStrc* pTile, int32_t nXPos, int32_t nYPos, DrawMode eDrawMode, int32_t nScreenPanels)
{
    return gpGraphicsInterface->pfShadowTileDraw(pTile, nXPos, nYPos, eDrawMode, nScreenPanels);
}
