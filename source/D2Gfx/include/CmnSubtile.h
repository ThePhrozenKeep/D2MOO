#pragma once


#include <cstdint>


struct D2TileLibraryEntryStrc;
struct D2GfxLightExStrc;
struct D2PaletteTableStrc;


enum D2TileBlockFormatFlags : uint16_t
{
	TILEBLOCKFORMAT_VISIBLE = 0x1,               // Block is drawn by the software renderer
	TILEBLOCKFORMAT_RLE = 0x4,                   // RLE encoded data, otherwise 32x15 isometric diamond
};


#pragma pack(push, 1)
// Maps each palette index to another palette index
struct D2PaletteStrc
{
	uint8_t nRemap[256];						//0x00
};

struct D2TileLibraryBlockStrc
{
	int16_t nPosX;								//0x00
	int16_t nPosY;								//0x02
	uint16_t unk0x04;							//0x04
	uint8_t nGridX;								//0x06
	uint8_t nGridY;								//0x07
	uint16_t nFormat;							//0x08 D2TileBlockFormatFlags
	uint32_t nLength;							//0x0A
	uint16_t unk0x0E;							//0x0E
	uint8_t* dwOffset_pData;					//0x10
};

// Fields of D2TileLibraryEntryStrc (D2CMP) that are only used by the software renderer
struct D2GfxTileLibraryEntryStrc
{
	uint8_t unk0x00[80];						//0x00
	int32_t nBlocks;							//0x50
	D2TileLibraryBlockStrc* pBlocks;			//0x54
};
#pragma pack(pop)


// Software renderer of tiles drawn to an 8 bits back buffer.
// Tiles are either RLE encoded or 32x15 isometric diamonds (floor blocks).
// RLE data is a list of (skip, length, pixels[length]) spans, a (0, 0) span ends the current line.
// "Flat" functions remap all pixels through a single palette, "Gouraud" ones interpolate the light intensity
// between the left and right sides of each line using the gouraud cache.

//D2Gfx.0x6FA71000
void __fastcall CMNSUBTILE_InitGouraudCache();

//D2Gfx.0x6FA71010
void __fastcall CMNSUBTILE_FillYBufferTable(uint8_t* pBuffer, int32_t nPitch, int32_t nHeight, int32_t nUnknown);

//D2Gfx.0x6FA71070
void __fastcall CMNSUBTILE_SetPaletteTables(D2PaletteTableStrc* pPaletteTable);

//D2Gfx.0x6FA710C0
int32_t __fastcall CMNSUBTILE_SetContrastAndGamma();

//D2Gfx.0x6FA71130
void __fastcall CMNSUBTILE_DrawIsoBlockFlat(uint8_t* pSrc, uint8_t* pDst, uint8_t* pPalette);

//D2Gfx.0x6FA71340
void __fastcall CMNSUBTILE_DrawIsoBlockGouraud(uint8_t* pSrc, uint8_t* pDst, int32_t nLeftIntensity, int32_t nLeftIntensityStep, int32_t nRightIntensity, int32_t nRightIntensityStep);

//D2Gfx.0x6FA71720
void __fastcall CMNSUBTILE_DrawRleFlat(uint8_t* pSrc, uint8_t* pDst, uint8_t* pPalette, int32_t nLines);

//D2Gfx.0x6FA71970
void __fastcall CMNSUBTILE_DrawRleGouraud(uint8_t* pSrc, uint8_t* pDst, int32_t nLeftIntensity, int32_t nLeftIntensityStep, int32_t nRightIntensity, int32_t nRightIntensityStep, int32_t nLines, uint8_t nIntensityShift);

//D2Gfx.0x6FA71D90
void __fastcall CMNSUBTILE_DrawRleFlatClipped(int32_t nX, int32_t nY, uint8_t* pSrc, uint8_t* pDst, uint8_t* pPalette, int32_t nLines);

//D2Gfx.0x6FA72090
void __fastcall CMNSUBTILE_DrawRleGouraudClipped(int32_t nX, int32_t nY, uint8_t* pSrc, uint8_t* pDst, int32_t nLeftIntensity, int32_t nLeftIntensityStep, int32_t nRightIntensity, int32_t nRightIntensityStep, int32_t nLines, uint8_t nIntensityShift);

//D2Gfx.0x6FA72570
void __fastcall CMNSUBTILE_BuildGouraudCache();

//D2Gfx.0x6FA72630
void __fastcall CMNSUBTILE_SetViewport(int32_t nWidth, int32_t nHeight);

//D2Gfx.0x6FA72700
void __fastcall CMNSUBTILE_BuildLineOffsets();

//D2Gfx.0x6FA72730
void __fastcall CMNSUBTILE_DrawVisTile(uint8_t* pSrc, int32_t nX, int32_t nY);

//D2Gfx.0x6FA72900
void __fastcall CMNSUBTILE_DrawFlatLitVisTile(uint8_t* pSrc, int32_t nX, int32_t nY, uint8_t nIntensity);

//D2Gfx.0x6FA729D0
void __fastcall CMNSUBTILE_TileDrawLit(uint8_t* pSrc, int32_t nX, int32_t nY, int32_t* pIntensities);

//D2Gfx.0x6FA72B80
void __fastcall CMNSUBTILE_DrawBlendedVisTile(uint8_t* pSrc, int32_t nX, int32_t nY, uint8_t nAlpha);

//D2Gfx.0x6FA72DC0
void __fastcall CMNSUBTILE_DrawRoofTile(uint8_t* pSrc, int32_t nX, int32_t nY, int32_t* pIntensities, uint8_t nAlpha);

//D2Gfx.0x6FA73130
void __fastcall CMNSUBTILE_DrawFloorBlockFlatClipped(D2TileLibraryBlockStrc* pBlock, int32_t nX, int32_t nY, uint8_t* pDst, uint8_t nIntensity);

//D2Gfx.0x6FA73270
void __fastcall CMNSUBTILE_DrawFloorBlockGouraudClipped(D2TileLibraryBlockStrc* pBlock, int32_t nX, int32_t nY, uint8_t* pDst, int32_t* pIntensities);

//D2Gfx.0x6FA73410
void __fastcall CMNSUBTILE_FloorTileDraw(D2TileLibraryEntryStrc* pTileLibraryEntry, int32_t nX, int32_t nY, D2GfxLightExStrc* pLightEx);
