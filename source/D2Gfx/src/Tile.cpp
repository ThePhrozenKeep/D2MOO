#include "Tile.h"

#include "D2Gfx.h"


//D2Gfx.0x6FA743C0 (#10080)
int32_t __stdcall TILE_DrawWallTile(D2TileLibraryEntryStrc* pTile, int32_t nXPos, int32_t nYPos, D2GfxLightStrc* pLight, int32_t nScreenPanels)
{
    return gpRenderCallbacks->pfDrawWallTile(pTile, nXPos, nYPos, pLight, nScreenPanels);
}

//D2Gfx.0x6FA743F0 (#10081)
int32_t __stdcall TILE_DrawTransparentWallTile(D2TileLibraryEntryStrc* pTile, int32_t nXPos, int32_t nYPos, D2GfxLightStrc* pLight, int32_t nScreenPanels, uint8_t nAlpha)
{
    return gpRenderCallbacks->pfDrawTransWallTile(pTile, nXPos, nYPos, pLight, nScreenPanels, nAlpha);
}

//D2Gfx.0x6FA74420 (#10082)
int32_t __stdcall TILE_DrawShadowTile(D2TileLibraryEntryStrc* pTile, int32_t nXPos, int32_t nYPos, DrawMode eDrawMode, int32_t nScreenPanels)
{
    return gpRenderCallbacks->pfDrawShadowTile(pTile, nXPos, nYPos, eDrawMode, nScreenPanels);
}
