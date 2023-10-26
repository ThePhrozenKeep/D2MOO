#pragma once

#include <cstdint>


struct D2GfxLightStrc;
struct D2TileLibraryEntryStrc;


//D2Gfx.0x6FA743C0 (#10080)
int32_t __stdcall TILE_TileDrawLit(D2TileLibraryEntryStrc* pTile, int32_t nXPos, int32_t nYPos, D2GfxLightStrc* pLight, int32_t nScreenPanels);
//D2Gfx.0x6FA743F0 (#10081)
int32_t __stdcall TILE_TileDrawTrans(D2TileLibraryEntryStrc* pTile, int32_t nXPos, int32_t nYPos, D2GfxLightStrc* pLight, int32_t nScreenPanels, uint8_t nAlpha);
//D2Gfx.0x6FA74420 (#10082)
int32_t __stdcall TILE_ShadowTileDraw(D2TileLibraryEntryStrc* pTile, int32_t nXPos, int32_t nYPos, int32_t nDrawMode, int32_t nScreenPanels);
