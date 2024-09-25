#pragma once

#include "CommonDefinitions.h"
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"

#pragma pack(1)

union D2C_PackedTileInformation
{
	uint32_t nPackedValue;
	struct {
		// If not wall nor floor, then it is a roof.
		uint32_t bIsWall           : 1; // BIT(0)     Is this a wall.
		uint32_t bIsFloor          : 1; // BIT(1)     Is this a floor.
		uint32_t bLOS              : 1; // BIT(2)     code-generated; added on edges
		uint32_t bEnclosed         : 1; // BIT(3)     seems to delimit an enclosure inside another area, or trees
		uint32_t bExit             : 1; // BIT(4)     arch or doorway or gateway in a wall
		uint32_t bUnk0x20          : 1; // BIT(5)     Unknown flag
		uint32_t bLayerBelow       : 1; // BIT(6)     only floor
		uint32_t bLayerAbove       : 1; // BIT(7)     wall & floor
		uint32_t nTileSequence     : 8; // BIT(8-15)  AKA tile subindex
		uint32_t bFillLOS          : 1; // BIT(16)    all tiles will get wall collision
		uint32_t bUnwalkable       : 1; // BIT(17) 
		uint32_t nWallLayer        : 2; // BIT(18-19) code-generated; set when at least Floor2 or Wall2  // (nLayer - 1) << 0x12
		//uint32_t bOverlappedLayer3 : 1; // BIT(19)    code-generated; set when has more walls, incl' of other orientations than usual; obj, shd, tree, roof, lower
		uint32_t nTileStyle        : 6; // BIT(20-25) AKA tile index
		uint32_t bRevealHidden     : 1; // BIT(26)    looks like an upper wall brought to a layer in front
		uint32_t bShadow           : 1; // BIT(27)    this layer is a shadow layer | Lectem's note: seems to be roof instead ? Or are shadow tiles interpreted as roof tiles ?
		uint32_t bLinkage          : 1; // BIT(28)    near wp, lvl links, paths // will never get hidden
		uint32_t bObjectWall       : 1; // BIT(29)    wall tiles with props; may be block reverb / other sounds (crates, barrels, tables etc.)
		uint32_t bUnk0x40000000    : 1; // BIT(30)    Unknown flag
		uint32_t bHidden           : 1; // BIT(31) 
	};
};

enum D2C_RoomTileConstants
{
	DRLGROOMTILE_TILES_SIZE = 8,
	DRLGROOMTILE_SUBTILES_SIZE = DRLGROOMTILE_TILES_SIZE * 5,
};

#pragma pack()

//D2Common.0x6FD88860
D2TileLibraryEntryStrc* __fastcall DRLGROOMTILE_GetTileCache(D2DrlgRoomStrc* pDrlgRoom, int nType, uint32_t nPackedTileInformation);
//D2Common.0x6FD889C0
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_InitWallTileData(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc** ppTileData, int nX, int nY, uint32_t nPackedTileInformation, D2TileLibraryEntryStrc* pTileLibraryEntry, int nTileType);
//D2Common.0x6FD88AC0
void __fastcall DRLGROOMTILE_InitializeTileDataFlags(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc* pTileData, uint32_t nTileFlags, int nType, int nX, int nY);
//D2Common.0x6FD88BE0
void __fastcall DRLGROOMTILE_AddTilePresetUnits(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc* pTileData, uint32_t nPackedTileInformation, int nX, int nY, int nTileType);
//D2Common.0x6FD88DD0
void __fastcall DRLGROOMTILE_InitTileData(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc* pTileData, int nX, int nY, uint32_t nPackedTileInformation, D2TileLibraryEntryStrc* pTileLibraryEntry);
//D2Common.0x6FD88E60
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_InitFloorTileData(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc** ppTileData, int nX, int nY, uint32_t nPackedTileInformation, D2TileLibraryEntryStrc* pTileLibraryEntry);
//D2Common.0x6FD88F10
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_InitShadowTileData(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc** ppTileData, int nX, int nY, uint32_t nPackedTileInformation, D2TileLibraryEntryStrc* pTileLibraryEntry);
//D2Common.0x6FD88FD0
void __fastcall DRLGROOMTILE_InitTileShadow(D2DrlgRoomStrc* pDrlgRoom, int nX, int nY, uint32_t nPackedTileInformation);
//D2Common.0x6FD89000
void __fastcall DRLGROOMTILE_LoadInitRoomTiles(D2DrlgRoomStrc* pDrlgRoom, D2DrlgGridStrc* pTilePackedInfoGrid, D2DrlgGridStrc* pTileTypeGrid, BOOL bFillBlanks, BOOL bKillEdgeX, BOOL bKillEdgeY);
//D2Common.0x6FD89360
BOOL __fastcall DRLGROOMTILE_AddWarp(D2DrlgRoomStrc* pDrlgRoom, int nX, int nY, uint32_t nPackedTileInformation, int nTileType);
//D2Common.0x6FD89410
void __fastcall DRLGROOMTILE_LoadWallWarpTiles(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc* pTileData, uint32_t nPackedTileInformation, int nTileType);
//D2Common.0x6FD89590
void __fastcall DRLGROOMTILE_LoadFloorWarpTiles(D2DrlgRoomStrc* pDrlgRoom, int nX, int nY, uint32_t nPackedTileInformation, int nTileType);
//D2Common.0x6FD897E0
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_GetLinkedTileData(D2DrlgRoomStrc* pDrlgRoom, BOOL bFloor, uint32_t nPackedTileInformation, int nX, int nY, D2DrlgRoomStrc** ppDrlgRoom);
//D2Common.0x6FD89930
void __fastcall DRLGROOMTILE_AddLinkedTileData(void* pMemPool, D2DrlgRoomStrc* pDrlgRoom, int nTileType, uint32_t nPackedTileInformation, int nX, int nY);
//D2Common.0x6FD89AF0
void __fastcall DRLGROOMTILE_LinkedTileDataManager(void* pMemPool, D2DrlgRoomStrc* pDrlgRoom1, D2DrlgRoomStrc* pDrlgRoom2, D2DrlgTileDataStrc* pTileData, int nTileType, uint32_t nPackedTileInformation, int nX, int nY);
//D2Common.0x6FD89CC0
void __fastcall DRLGROOMTILE_GetCreateLinkedTileData(void* pMemPool, D2DrlgRoomStrc* pDrlgRoom, int nTileType, uint32_t nPackedTileInformation, int nX, int nY);
//D2Common.0x6FD89E30
void __fastcall DRLGROOMTILE_CountAllTileTypes(D2DrlgRoomStrc* pDrlgRoom, D2DrlgGridStrc* pTileInfoGrid, BOOL bCheckCoordinatesValidity, BOOL bKillEdgeX, BOOL bKillEdgeY);
//D2Common.0x6FD89F00
void __fastcall DRLGROOMTILE_CountWallWarpTiles(D2DrlgRoomStrc* pDrlgRoom, D2DrlgGridStrc* pTileInfoGrid, D2DrlgGridStrc* pTileTypeGrid, BOOL bKillEdgeX, BOOL bKillEdgeY);
//D2Common.0x6FD89FA0
void __fastcall DRLGROOMTILE_InitRoomGrids(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD89FD0
void __fastcall DRLGROOMTILE_AddRoomMapTiles(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD8A010
void __fastcall DRLGROOMTILE_AllocTileGrid(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD8A050
void __fastcall DRLGROOMTILE_AllocTileData(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD8A130
void __fastcall DRLGROOMTILE_ReallocRoofTileGrid(void* pMemPool, D2DrlgTileGridStrc* pTileGrid, int nAdditionalRoofs);
//D2Common.0x6FD8A1B0 (#10017)
D2COMMON_DLL_DECL int __fastcall DRLGROOMTILE_GetNumberOfShadowsFromRoom(D2ActiveRoomStrc* pRoom);
//D2Common.0x6FD8A1D0
void __fastcall DRLGROOMTILE_FreeTileGrid(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD8A2E0
void __fastcall DRLGROOMTILE_FreeRoom(D2DrlgRoomStrc* pDrlgRoom, BOOL bKeepRoom);
//D2Common.0x6FD8A380
void __fastcall DRLGROOMTILE_LoadDT1FilesForRoom(D2DrlgRoomStrc* pDrlgRoom);
