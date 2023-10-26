#pragma once

#include <cstdint>

#include <Windows.h>


struct D2TileLibraryEntryStrc;
struct D2GfxLightExStrc;


#pragma pack(push, 1)
struct D2PaletteStrc
{
    uint8_t unk0x00[256];
};

struct D2TileLibraryBlockStrc
{
    uint16_t nPosX;
    uint16_t nPosY;
    uint16_t unk0x04;
    uint8_t nGridX;
    uint8_t nGridY;
    uint16_t nFormat;
    uint32_t nLength;
    uint16_t unk0x0E;
    uint8_t* dwOffset_pData;
};
#pragma pack(pop)


//D2Gfx.0x6FA71000
void __fastcall D2GFX_InitGouraudCache_6FA71000();

//D2Gfx.0x6FA71010
void __fastcall D2GFX_FillYBufferTable_6FA71010(uint8_t* a1, int32_t a2, int32_t a3, int32_t a4);

//D2Gfx.0x6FA71070
void __fastcall sub_6FA71070(PALETTEENTRY** pPaletteTable);

//D2Gfx.0x6FA710C0
int32_t __fastcall D2GFX_SetContrastAndGamma_6FA710C0();

//D2Gfx.0x6FA71130
void __fastcall sub_6FA71130(uint8_t* a1, uint8_t* a2, uint8_t* a3);

//D2Gfx.0x6FA71340) --------------------------------------------------------
void __fastcall sub_6FA71340(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6);

//D2Gfx.0x6FA71720
void __fastcall sub_6FA71720(uint8_t* a1, uint8_t* a2, uint8_t* a3, int32_t a4);

//D2Gfx.0x6FA71970) --------------------------------------------------------
void __fastcall sub_6FA71970(uint8_t* a1, uint8_t* a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, uint8_t a8);

//D2Gfx.0x6FA71D90) --------------------------------------------------------
void __fastcall sub_6FA71D90(int32_t a1, int32_t a2, uint8_t* a3, uint8_t* a4, uint8_t* a5, int32_t a6);

//D2Gfx.0x6FA72090) --------------------------------------------------------
void __fastcall sub_6FA72090(int32_t a1, int32_t a2, uint8_t* a3, uint8_t* a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8, int32_t a9, uint8_t a10);

//D2Gfx.0x6FA72570
void __fastcall DGFX_InitGouraudCache_6FA72570();

//D2Gfx.0x6FA72630
void __fastcall sub_6FA72630(int32_t a1, int32_t a2);

//D2Gfx.0x6FA72700
void __fastcall sub_6FA72700();

//D2Gfx.0x6FA72730) --------------------------------------------------------
void __fastcall D2GFX_DrawVisTile_6FA72730(uint8_t* a1, int32_t nX, int32_t nY);

//D2Gfx.0x6FA72900
void __fastcall sub_6FA72900(uint8_t* a1, int32_t nX, int32_t nY, uint8_t a4);

//D2Gfx.0x6FA729D0
void __fastcall TILE_TileDrawLit_6FA729D0(uint8_t* a1, int32_t nX, int32_t nY, int32_t* pIntensities);

//D2Gfx.0x6FA72B80) --------------------------------------------------------
void __fastcall D2GFX_DrawBlendedVisTile_6FA72B80(int32_t a1, int32_t nX, int32_t nY, uint8_t nAlpha);

//D2Gfx.0x6FA72DC0) --------------------------------------------------------
void __fastcall D2GFX_DrawRoofTile_6FA72DC0(int32_t a1, int32_t nX, int32_t nY, int32_t* pIntensities, uint8_t nAlpha);

//D2Gfx.0x6FA73130) --------------------------------------------------------
void __fastcall sub_6FA73130(D2TileLibraryBlockStrc* a1, int32_t a2, int32_t a3, uint8_t* a4, uint8_t a5);

//D2Gfx.0x6FA73270) --------------------------------------------------------
void __fastcall sub_6FA73270(D2TileLibraryBlockStrc* a1, int32_t a2, int32_t a3, uint8_t* a4, int32_t* a5);

//D2Gfx.0x6FA73410) --------------------------------------------------------
void __fastcall D2GFX_FloorTileDraw_6FA73410(D2TileLibraryEntryStrc* pTileLibraryEntry, int32_t nX, int32_t nY, D2GfxLightExStrc* pLightEx);
