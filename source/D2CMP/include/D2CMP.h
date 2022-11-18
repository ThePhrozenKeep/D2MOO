#pragma once

#include <D2Dll.h>
#include <cstdint>

#ifdef D2CMP_IMPL
#define D2CMP_DLL_DECL // We use .def files, not dllexport
#else
#define D2CMP_DLL_DECL __declspec( dllimport )
#endif

#pragma pack(1)

enum D2TileMaterialFlags : uint16_t 
{
	TILE_FLAGS_OTHER = 0x1,
	TILE_FLAGS_WATER = 0x2,
	TILE_FLAGS_WOOD_OBJ = 0x4, // Blocks reverberation
	TILE_FLAGS_ISTONE = 0x8,   // Indoor stone
	TILE_FLAGS_OSTONE = 0x10,  // Outdoor stone
	TILE_FLAGS_DIRT = 0x20,
	TILE_FLAGS_SAND = 0x40,
	TILE_FLAGS_WOOD = 0x80,
	TILE_FLAGS_LAVA = 0x100, // Special case: Always bright + animated
	TILE_FLAGS_SNOW = 0x400,
};

// Names come from this thread https://d2mods.info/forum/viewtopic.php?f=81&t=65163
// TODO: check those are all accurate or not misnamed
enum D2TileType
{
	TILETYPE_FLOORS = 0,
	TILETYPE_LEFTWALL = 1,
	TILETYPE_RIGHTWALL = 2,
	TILETYPE_RIGHTPARTOFNORTHCORNERWALL = 3,
	TILETYPE_LEFTPARTOFNORTHCORNERWALL = 4,
	TILETYPE_LEFTENDWALL = 5,
	TILETYPE_RIGHTENDWALL = 6,
	TILETYPE_SOUTHCORNERWALL = 7,
	TILETYPE_LEFTWALLWITHDOOR = 8,
	TILETYPE_RIGHTWALLWITHDOOR = 9,
	TILETYPE_SPECIALTILES_10 = 10, // Warps, Corpses, Visibility changes, Teleport ...
	TILETYPE_SPECIALTILES_11 = 11, // See 10
	TILETYPE_PILLARS_CCOLUMNS_STANDALONEOBJECTS = 12,
	TILETYPE_SHADOWS = 13,
	TILETYPE_TREES = 14,
	TILETYPE_ROOFS = 15,
	TILETYPE_LOWERWALLSEQUIVALENTTOTILETYPE_1 = 16,
	TILETYPE_LOWERWALLSEQUIVALENTTOTILETYPE_2 = 17,
	TILETYPE_LOWERWALLSEQUIVALENTTOTILETYPE_3_AND_4 = 18,
	TILETYPE_LOWERWALLSEQUIVALENTTOTILETYPE_7 = 19,
};

struct D2TileRecordStrc;

struct D2TileLibraryEntryStrc
{
	int32_t nLightDirection;					//0x00
	uint16_t nRoofHeight;						//0x04
	uint16_t nFlags;							//0x06
	int32_t nTotalHeight;						//0x08
	int32_t nWidth;								//0x0C
	int32_t nHeightToBottom;					//0x10
	int32_t nType;								//0x14 aka nOrientation
	int32_t nStyle;								//0x18 aka nIndex
	int32_t nSequence;							//0x1C aka nSubIndex
	int32_t nRarity_Frame;						//0x20 By default this is the rarity of the tile, for animated tiles this is the frame index.
	int32_t transparentColorRGB24;				//0x24
	uint8_t dwTileFlags[4];						//0x28
	int32_t dwBlockOffset_pBlock;				//0x2C
	int32_t nBlockSize;							//0x30
	int32_t nBlocks;							//0x34
	D2TileRecordStrc* pParent;					//0x38
	uint16_t unk0x3C;							//0x3C
	uint16_t nCacheIndex;						//0x3E
	uint32_t unk0x40[4];						//0x40
	//int32_t field_50;
	//int32_t field_54;
	//int32_t field_58;
	//int32_t field_5C;
};

struct D2TileStrc
{
	D2TileStrc* pNext;						//0x00
	uint16_t unk0x04;							//0x04
	uint16_t nFlags;							//0x06
	int32_t unk0x08;							//0x08
	int32_t nTile;								//0x0C
	int32_t unk0x10;							//0x10
	int32_t nAct;								//0x14
	int32_t nLevel;								//0x18
	int32_t nTileType;							//0x1C
	uint32_t unk0x20[7];						//0x20
	D2TileRecordStrc* pParent;				//0x3C
	uint32_t unk0x40[33];						//0x40
};

struct D2TileLibraryBlockStrc
{
	int16_t nPosX;							//0x00
	int16_t nPosY;							//0x02
	int16_t unk0x04;							//0x04
	uint8_t nGridX;							//0x06
	uint8_t nGridY;							//0x07
	uint16_t nFormat;							//0x08
	int32_t nLength;							//0x0A
	int16_t unk0x0E;							//0x0E
	int32_t dwOffset_pData;						//0x10
};

struct D2TileLibraryHashRefStrc
{
	D2TileLibraryEntryStrc* pTile;			//0x00
	D2TileLibraryHashRefStrc* pPrev;		//0x04
};

struct D2TileLibraryHashNodeStrc
{
	int32_t nStyle;							//0x00 aka nIndex;								
	int32_t nSequence;						//0x04 aka nSequence;							
	int32_t nType;							//0x08 aka nOrientation;						
	D2TileLibraryHashRefStrc* pRef;			//0x0C
	D2TileLibraryHashNodeStrc* pPrev;		//0x10
};

struct D2TileLibraryHashStrc
{
	D2TileLibraryHashNodeStrc* pNodes[128];	//0x00
};

struct D2TileLibraryHeaderStrc
{
	int32_t dwVersion;							//0x00
	int32_t dwFlags;							//0x04
	char szLibraryName[260];				//0x08
	int32_t nTiles;								//0x10C
	int32_t dwTileStart_pFirst;					//0x110
};

struct D2TileRecordStrc
{
	char szLibraryName[260];				//0x00
	void* pLibrary;							//0x104
	D2TileLibraryHashStrc* pHashBlock;		//0x108
	D2TileRecordStrc* pPrev;				//0x10C
};

#pragma pack()

// Helper function
inline bool TileTypeIsAWallWithDoor(int32_t nTileType)
{
	return nTileType == TILETYPE_RIGHTWALLWITHDOOR || nTileType == TILETYPE_LEFTWALLWITHDOOR;
}


D2FUNC_DLL(D2CMP, 10077_GetTileType, int, __stdcall, (D2TileLibraryEntryStrc * pTileLibraryEntry), 0xFFF0)											//D2Cmp.#10078
D2FUNC_DLL(D2CMP, 10078_GetTileStyle, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFF30)											//D2Cmp.#10078
// Material flags
D2FUNC_DLL(D2CMP, 10079_GetTileFlags, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFF60)											//D2Cmp.#10079
D2FUNC_DLL(D2CMP, 10081_GetTileRarity, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFFC0)											//D2Cmp.#10081
D2FUNC_DLL(D2CMP, 10082_GetTileSequence, int, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0xFFF0)										//D2Cmp.#10082
D2FUNC_DLL(D2CMP, 10085_GetTileFlagArray, uint8_t*, __stdcall, (D2TileLibraryEntryStrc* pTileLibraryEntry), 0x10080)								//D2Cmp.#10085
D2FUNC_DLL(D2CMP, 10087_LoadTileLibrarySlot, void, __stdcall, (D2TileLibraryHashStrc** ppTileLibraryHash, const char* szFileName), 0xFDE0)			//D2Cmp.#10087
D2FUNC_DLL(D2CMP, 10088_GetTiles, int, __stdcall, (D2TileLibraryHashStrc** ppTileLibraryHash, int nType, int nStyle, int nSequence, D2TileLibraryEntryStrc** pTileList, int nTileListSize), 0xFE70)//D2Cmp.#10088
D2FUNC_DLL(D2CMP, MixPalette, uint8_t*, __stdcall, (uint8_t nTrans, int nColor), 0xB760)															//D2Cmp.#10098
