#include "Drlg/D2DrlgRoomTile.h"

#include "D2Collision.h"
#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgDrlgLogic.h"
#include "Drlg/D2DrlgDrlgWarp.h"
#include "Drlg/D2DrlgOutPlace.h"
#include "Drlg/D2DrlgOutRoom.h"
#include "Drlg/D2DrlgMaze.h"
#include "Drlg/D2DrlgPreset.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "D2Dungeon.h"
#include "D2Monsters.h"
#include "D2Seed.h"
#include <D2CMP.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <Units/Units.h>

struct D2UnkDrlgRoomTileStrc
{
	int nLevelId;
	int nArrayStartId;
	int nArrayEndId;
};

struct D2UnkDrlgRoomTileStrc2
{
	int field_0;
	int field_4;
	int field_8;
	int nUnitId;
	int nUnitType;
	int nX;
	int nY;
};



//D2Common.0x6FD88860
D2TileLibraryEntryStrc* __fastcall DRLGROOMTILE_GetTileCache(D2RoomExStrc* pRoomEx, int nType, unsigned int nTileFlags)
{
	D2TileLibraryEntryStrc* ppTileLibraryEntries[40] = {};
	unsigned int nMax = 0;
	int nEntries = 0;
	int nRand = 0;
	int nId = 0;
	uint8_t nSequence = 0;
	uint8_t nStyle = 0;

	if (nTileFlags)
	{
		nStyle = (nTileFlags >> 20) & 63;
		nSequence = BYTE1(nTileFlags);
	}
	else
	{
		nStyle = 0;
		nSequence = 0;
	}

	nEntries = D2CMP_10088_GetTiles(pRoomEx->pTiles, nType, nStyle, nSequence, ppTileLibraryEntries, ARRAY_SIZE(ppTileLibraryEntries));
	if (nEntries)
	{
		for (int i = 0; i < nEntries; ++i)
		{
			if (!ppTileLibraryEntries[i])
			{
				FOG_10025("phTileArray[ii]", __FILE__, __LINE__);
			}

			nMax += D2CMP_10081_GetTileRarity(ppTileLibraryEntries[i]);
		}

		nRand = SEED_RollLimitedRandomNumber(&pRoomEx->pSeed, nMax) + 1;
		if (nMax)
		{
			while (nEntries > 1 && nRand > 0)
			{
				nRand -= D2CMP_10081_GetTileRarity(ppTileLibraryEntries[nId]);
				++nId;
			}

			if (nId)
			{
				--nId;
			}
		}

		return ppTileLibraryEntries[nId];
	}
	else
	{
		if (!D2CMP_10088_GetTiles(pRoomEx->pTiles, 10, 0, 0, ppTileLibraryEntries, ARRAY_SIZE(ppTileLibraryEntries)))
		{
			FOG_10025("nSize", __FILE__, __LINE__);
		}

		return ppTileLibraryEntries[0];
	}
}

//D2Common.0x6FD889C0
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_InitWallTileData(D2RoomExStrc* pRoomEx, D2DrlgTileDataStrc** ppTileData, int nX, int nY, unsigned int nTileFlags, D2TileLibraryEntryStrc* pTileLibraryEntry, int nTileType)
{
	D2DrlgTileDataStrc* pTileData = NULL;
	int nPosX = 0;
	int nPosY = 0;

	pTileData = &pRoomEx->pTileGrid->pTiles.pWallTiles[pRoomEx->pTileGrid->nWalls];
	if (ppTileData)
	{
		pTileData->unk0x20 = *ppTileData;
		*ppTileData = pTileData;
	}
	else
	{
		pTileData->unk0x20 = NULL;
	}

	++pRoomEx->pTileGrid->nWalls;

	pTileData->nPosX = nX - pRoomEx->nTileXPos;
	pTileData->nPosY = nY - pRoomEx->nTileYPos;

	nPosX = nX;
	nPosY = nY + 1;

	DUNGEON_ExpandTileCoords(&nPosX, &nPosY);

	pTileData->nWidth = nPosX;
	pTileData->nHeight = nPosY + 40;

	pTileData->dwFlags = 0;
	pTileData->unk0x24 = 0;
	pTileData->nTileType = nTileType;

	pTileData->pTile = pTileLibraryEntry;
	pTileData->nGreen = -1;
	pTileData->nBlue = -1;
	pTileData->nRed = -1;

	DRLGROOMTILE_InitializeTileDataFlags(pRoomEx, pTileData, nTileFlags, nTileType, nX, nY);

	if (nTileType == TILETYPE_RIGHTPARTOFNORTHCORNERWALL)
	{
		DRLGROOMTILE_InitializeTileDataFlags(pRoomEx, DRLGROOMTILE_InitWallTileData(pRoomEx, ppTileData, nX, nY, nTileFlags, DRLGROOMTILE_GetTileCache(pRoomEx, 4, nTileFlags), 4), nTileFlags, 4, nX, nY);
	}

	return pTileData;
}

//D2Common.0x6FD88AC0
void __fastcall DRLGROOMTILE_InitializeTileDataFlags(D2RoomExStrc* pRoomEx, D2DrlgTileDataStrc* pTileData, uint32_t nTileFlags, int nType, int nX, int nY)
{
	const D2C_PackedTileInformation nTileInfo{ nTileFlags };

	if (pRoomEx && (nType == TILETYPE_RIGHTWALLWITHDOOR || nType == TILETYPE_LEFTWALLWITHDOOR))
	{
		DRLGROOMTILE_AddTilePresetUnits(pRoomEx, pTileData, nTileFlags, nX, nY, 0);
	}

	if (nType != TILETYPE_SHADOWS)
	{
		pTileData->dwFlags |= (nTileInfo.nWallLayer + 1) << MAPTILE_WALL_LAYER_BIT;
	}

	if (nType == TILETYPE_TREES)
	{
		pTileData->dwFlags |= MAPTILE_TREES;
	}
	else if (nType == TILETYPE_SPECIALTILES_11 || nType == TILETYPE_SPECIALTILES_10 || nType == TILETYPE_RIGHTWALLWITHDOOR || nType == TILETYPE_LEFTWALLWITHDOOR)
	{
		pTileData->dwFlags |= MAPTILE_WALL_EXIT;
	}

	if (nTileInfo.bLayerAbove)
	{
		pTileData->dwFlags |= MAPTILE_UNK_0x1;
	}

	if (nTileInfo.bLinkage)
	{
		pTileData->dwFlags |= MAPTILE_FLOOR_LINKER_PATH | MAPTILE_WALL_EXIT;
	}

	if (nTileInfo.bUnwalkable)
	{
		pTileData->dwFlags |= MAPTILE_UNWALKABLE;
	}

	if (nTileInfo.bFillLOS)
	{
		pTileData->dwFlags |= MAPTILE_FILL_LOS;
	}

	if (nTileInfo.bEnclosed)
	{
		pTileData->dwFlags |= MAPTILE_TREES;
	}

	if (nTileInfo.bHidden)
	{
		pTileData->dwFlags &= (~MAPTILE_HIDDEN);
	}
	else
	{
		pTileData->dwFlags |= MAPTILE_HIDDEN;
	}

	if (nTileInfo.bRevealHidden)
	{
		pTileData->dwFlags |= MAPTILE_UNK_0x200 | MAPTILE_TREES | MAPTILE_HIDDEN;
	}

	if (nTileInfo.bObjectWall)
	{
		pTileData->dwFlags |= MAPTILE_OBJECT_WALL;
	}

	if (nTileInfo.bLOS)
	{
		pTileData->dwFlags |= MAPTILE_LOS;
	}

	uint16_t nFlags = D2CMP_10079_GetTileFlags(pTileData->pTile);
	if (nFlags & TILE_FLAGS_OTHER)
	{
		pTileData->dwFlags |= MAPTILE_TREES;
	}

	if (nFlags & TILE_FLAGS_WOOD_OBJ)
	{
		pTileData->dwFlags |= MAPTILE_OBJECT_WALL;
	}
}

//D2Common.0x6FD88BE0
//TODO: v6, v10, v26
void __fastcall DRLGROOMTILE_AddTilePresetUnits(D2RoomExStrc* pRoomEx, D2DrlgTileDataStrc* pTileData, int nTileFlags, int nX, int nY, int nTileType)
{
	static const D2UnkDrlgRoomTileStrc stru_6FDD0DA8[] =
	{
		{ LEVEL_BARRACKS, 0, 3 },
		{ LEVEL_JAILLEV1, 0, 3 },
		{ LEVEL_JAILLEV2, 0, 3 },
		{ LEVEL_JAILLEV3, 0, 3 },
		{ LEVEL_MONASTERYGATE, 4, 6 },
		{ LEVEL_OUTERCLOISTER, 4, 6 },
		{ LEVEL_INNERCLOISTER, 5, 9 },
		{ LEVEL_CATHEDRAL, 5, 9 },
		{ LEVEL_CATACOMBSLEV1, 10, 11 },
		{ LEVEL_CATACOMBSLEV2, 10, 11 },
		{ LEVEL_CATACOMBSLEV3, 10, 11 },
		{ LEVEL_CATACOMBSLEV4, 10, 12 },
		{ LEVEL_HAREMLEV2, 13, 14 },
		{ LEVEL_PALACECELLARLEV1, 15, 18 },
		{ LEVEL_PALACECELLARLEV2, 15, 18 },
		{ LEVEL_PALACECELLARLEV3, 15, 18 },
		{ LEVEL_STONYTOMBLEV1, 19, 20 },
		{ LEVEL_HALLSOFTHEDEADLEV1, 19, 20 },
		{ LEVEL_HALLSOFTHEDEADLEV2, 19, 20 },
		{ LEVEL_CLAWVIPERTEMPLELEV1, 19, 20 },
		{ LEVEL_STONYTOMBLEV2, 19, 20 },
		{ LEVEL_HALLSOFTHEDEADLEV3, 19, 20 },
		{ LEVEL_CLAWVIPERTEMPLELEV2, 19, 20 },
		{ LEVEL_TALRASHASTOMB1, 19, 20 },
		{ LEVEL_TALRASHASTOMB2, 19, 20 },
		{ LEVEL_TALRASHASTOMB3, 19, 20 },
		{ LEVEL_TALRASHASTOMB4, 19, 20 },
		{ LEVEL_TALRASHASTOMB5, 19, 20 },
		{ LEVEL_TALRASHASTOMB6, 19, 20 },
		{ LEVEL_TALRASHASTOMB7, 19, 20 },
		{ LEVEL_MAGGOTLAIRLEV1, 21, 22 },
		{ LEVEL_MAGGOTLAIRLEV2, 21, 22 },
		{ LEVEL_MAGGOTLAIRLEV3, 21, 22 },
		{ LEVEL_HARROGATH, 23, 24 },
		{ LEVEL_ID_ACT5_BARRICADE_1, 24, 33 },
		{ LEVEL_ARREATPLATEAU, 24, 33 },
		{ LEVEL_TUNDRAWASTELANDS, 24, 33 },
	};

	static const D2UnkDrlgRoomTileStrc2 stru_6FDD0F68[] =
	{
		{ 7, 0, 1, OBJECT_DOORGATERIGHT, UNIT_OBJECT, 5, 0 },
		{ 7, 0, 0, OBJECT_DOORGATELEFT, UNIT_OBJECT, 0, 5 },
		{ 5, 0, 1, OBJECT_DOORWOODENRIGHT, UNIT_OBJECT, 0, 0 },
		{ 5, 0, 0, OBJECT_DOORWOODENLEFT, UNIT_OBJECT, 0, 0 },
		{ 6, 0, 1, OBJECT_DOORMONASTERYDOUBLERIGHT, UNIT_OBJECT, 5, -2 },
		{ 4, 0, 1, OBJECT_DOORCOURTYARDRIGHT, UNIT_OBJECT, 1, 2 },
		{ 4, 0, 0, OBJECT_DOORCOURTYARDLEFT, UNIT_OBJECT, 0, 0 },
		{ 4, 3, 1, OBJECT_DOORCATHEDRALDOUBLE, UNIT_OBJECT, 1, 0 },
		{ 1, 2, 0, OBJECT_DOORCATHEDRALLEFT, UNIT_OBJECT, 0, 3 },
		{ 1, 2, 1, OBJECT_DOORCATHEDRALRIGHT, UNIT_OBJECT, 3, 0 },
		{ 0, 0, 1, OBJECT_DOORWOODENRIGHT, UNIT_OBJECT, 0, 0 },
		{ 0, 0, 0, OBJECT_DOORWOODENLEFT2, UNIT_OBJECT, 0, 0 },
		{ 2, 0, 1, OBJECT_ANDARIELS_DOOR, UNIT_OBJECT, 5, 0 },
		{ 0, 1, 1, OBJECT_IRONGRATEDOORRIGHT, UNIT_OBJECT, 2, 0 },
		{ 0, 1, 0, OBJECT_IRONGRATEDOORLEFT, UNIT_OBJECT, 0, 2 },
		{ 5, 0, 1, OBJECT_WOODENGRATEDOORRIGHT, UNIT_OBJECT, 2, 0 },
		{ 4, 0, 0, OBJECT_WOODENGRATEDOORLEFT, UNIT_OBJECT, 0, 2 },
		{ 0, 0, 1, OBJECT_WOODENDOORRIGHT, UNIT_OBJECT, 2, 0 },
		{ 0, 0, 0, OBJECT_WOODENDOORLEFT, UNIT_OBJECT, 0, 2 },
		{ 2, 4, 1, OBJECT_TOMBDOORRIGHT, UNIT_OBJECT, 1, 0 },
		{ 2, 1, 0, OBJECT_TOMBDOORLEFT, UNIT_OBJECT, 0, 2 },
		{ 0, 1, 1, OBJECT_SLIMEDOOR1, UNIT_OBJECT, 0, 0 },
		{ 0, 1, 0, OBJECT_SLIMEDOOR2, UNIT_OBJECT, 0, 0 },
		{ 3, 3, 0, OBJECT_HARROGATH_TOWN_MAIN_GATE, UNIT_OBJECT, -2, 4 },

		{ 2, 1, 0, MONSTER_BARRICADETOWER, UNIT_MONSTER, 1, 2 },
		{ 2, 1, 1, MONSTER_BARRICADETOWER, UNIT_MONSTER, 2, 1 },
		{ 2, 6, 0, MONSTER_BARRICADETOWER, UNIT_MONSTER, 1, 1 },
		{ 2, 2, 0, MONSTER_BARRICADEDOOR2, UNIT_MONSTER, 0, 1 },
		{ 2, 3, 1, MONSTER_BARRICADEDOOR1, UNIT_MONSTER, 1, 0 },
		{ 26, 0, 0, MONSTER_PRISONDOOR, UNIT_MONSTER, 0, 1 },
		{ 2, 4, 1, MONSTER_BARRICADEWALL1, UNIT_MONSTER, 0, 0 },
		{ 2, 4, 0, MONSTER_BARRICADEWALL2, UNIT_MONSTER, 0, 0 },

		{ 29, 0, 1, OBJECT_PERMANENT_TOWN_PORTAL, UNIT_OBJECT, 2, 0 },
		{ 29, 0, 0, OBJECT_PERMANENT_TOWN_PORTAL, UNIT_OBJECT, 0, 2 },
	};

	int nUnitId = 0;
	int nPosX = 0;
	int nPosY = 0;
	int v26 = 0;
	int v10 = 0;
	int v6 = 0;

	if (pTileData)
	{
		if (pTileData->dwFlags & 0x20)
		{
			return;
		}

		v6 = pTileData->nTileType == TILETYPE_RIGHTWALLWITHDOOR;
	}
	else
	{
		v6 = nTileType == TILETYPE_RIGHTWALLWITHDOOR;
	}

	for (int i = 0; i < ARRAY_SIZE(stru_6FDD0DA8); ++i)
	{
		if (stru_6FDD0DA8[i].nLevelId == pRoomEx->pLevel->nLevelId)
		{
			v10 = ((unsigned int)nTileFlags >> 20) & 0x3F;
			v26 = BYTE1(nTileFlags);

			for (int j = stru_6FDD0DA8[i].nArrayStartId; j <= stru_6FDD0DA8[i].nArrayEndId; ++j)
			{
				if (v10 == stru_6FDD0F68[j].field_0 && v26 == stru_6FDD0F68[j].field_4 && v6 == stru_6FDD0F68[j].field_8)
				{
					nPosX = nX - pRoomEx->nTileXPos;
					nPosY = nY - pRoomEx->nTileYPos;

					DUNGEON_ExpandCoords(&nPosX, &nPosY);

					nPosX += stru_6FDD0F68[j].nX;
					nPosY += stru_6FDD0F68[j].nY;

					if (nPosX >= 0 && nPosY >= 0 && nPosX < 5 * pRoomEx->nTileWidth && nPosY < 5 * pRoomEx->nTileHeight)
					{
						nUnitId = stru_6FDD0F68[j].nUnitId;

						switch (stru_6FDD0F68[j].nUnitType)
						{
						case UNIT_MONSTER:
							DRLGROOM_AllocPresetUnit(pRoomEx, pRoomEx->pLevel->pDrlg->pMempool, UNIT_MONSTER, MONSTERS_ValidateMonsterId(nUnitId), MONMODE_NEUTRAL, nPosX, nPosY);
							break;

						case UNIT_OBJECT:
							if (nUnitId < 91 || nUnitId > 92 || (unsigned int)SEED_RollRandomNumber(&pRoomEx->pSeed) % 3)
							{
								DRLGROOM_AllocPresetUnit(pRoomEx, pRoomEx->pLevel->pDrlg->pMempool, UNIT_OBJECT, nUnitId, OBJMODE_NEUTRAL, nPosX, nPosY);
							}
							break;

						default:
							DRLGROOM_AllocPresetUnit(pRoomEx, pRoomEx->pLevel->pDrlg->pMempool, stru_6FDD0F68[j].nUnitType, nUnitId, 0, nPosX, nPosY);
							break;
						}

						if (pTileData)
						{
							pTileData->dwFlags |= 0x20;
						}
					}
					return;
				}
			}
		}
	}
}

//D2Common.0x6FD88DD0
void __fastcall DRLGROOMTILE_InitTileData(D2RoomExStrc* pRoomEx, D2DrlgTileDataStrc* pTileData, int nX, int nY, int nTileFlags, D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	int nPosX = 0;
	int nPosY = 0;

	pTileData->pTile = pTileLibraryEntry;
	pTileData->nTileType = TILETYPE_FLOORS;
	pTileData->dwFlags = 0;
	pTileData->unk0x24 = 0;
	pTileData->nGreen = 0xFF;
	pTileData->nBlue = 0xFF;
	pTileData->nRed = 0xFF;

	if (pRoomEx)
	{
		pTileData->nPosX = nX - pRoomEx->nTileXPos;
		pTileData->nPosY = nY - pRoomEx->nTileYPos;

		nPosX = nX;
		nPosY = nY + 1;

		DUNGEON_ExpandTileCoords(&nPosX, &nPosY);

		pTileData->nWidth = nPosX;
		pTileData->nHeight = nPosY + 40;
	}

	DRLGROOMTILE_InitializeTileDataFlags(pRoomEx, pTileData, nTileFlags, 0, nX, nY);
}

//D2Common.0x6FD88E60
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_InitFloorTileData(D2RoomExStrc* pRoomEx, D2DrlgTileDataStrc** ppTileData, int nX, int nY, int nTileFlags, D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2DrlgTileDataStrc* pTileData = NULL;

	int nPosX = 0;
	int nPosY = 0;

	pTileData = &pRoomEx->pTileGrid->pTiles.pFloorTiles[pRoomEx->pTileGrid->nFloors];
	if (ppTileData)
	{
		pTileData->unk0x20 = *ppTileData;
		*ppTileData = pTileData;
	}
	else
	{
		pTileData->unk0x20 = NULL;
	}

	++pRoomEx->pTileGrid->nFloors;

	pTileData->pTile = pTileLibraryEntry;
	pTileData->nTileType = TILETYPE_FLOORS;
	pTileData->dwFlags = 0;
	pTileData->unk0x24 = 0;
	pTileData->nGreen = -1;
	pTileData->nBlue = -1;
	pTileData->nRed = -1;


	pTileData->nPosX = nX - pRoomEx->nTileXPos;
	pTileData->nPosY = nY - pRoomEx->nTileYPos;

	nPosX = nX;
	nPosY = nY + 1;

	DUNGEON_ExpandTileCoords(&nPosX, &nPosY);

	pTileData->nWidth = nPosX;
	pTileData->nHeight = nPosY + 40;

	DRLGROOMTILE_InitializeTileDataFlags(pRoomEx, pTileData, nTileFlags, 0, nX, nY);

	return pTileData;
}

//D2Common.0x6FD88F10
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_InitShadowTileData(D2RoomExStrc* pRoomEx, D2DrlgTileDataStrc** ppTileData, int nX, int nY, int nTileFlags, D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2DrlgTileDataStrc* pTileData = NULL;

	int nPosX = 0;
	int nPosY = 0;

	pTileData = &pRoomEx->pTileGrid->pTiles.pRoofTiles[pRoomEx->pTileGrid->nShadows];
	if (ppTileData)
	{
		pTileData->unk0x20 = *ppTileData;
		*ppTileData = pTileData;
	}
	else
	{
		pTileData->unk0x20 = 0;
	}

	++pRoomEx->pTileGrid->nShadows;
	pTileData->nPosX = nX - pRoomEx->nTileXPos;
	pTileData->nPosY = nY - pRoomEx->nTileYPos;

	nPosX = nX;
	nPosY = nY + 1;

	DUNGEON_ExpandTileCoords(&nPosX, &nPosY);

	pTileData->nTileType = TILETYPE_SHADOWS;

	pTileData->nWidth = nPosX;
	pTileData->nHeight = nPosY + 40;

	pTileData->dwFlags = 0;
	pTileData->unk0x24 = 0;
	pTileData->nGreen = -1;
	pTileData->nBlue = -1;
	pTileData->nRed = -1;
	pTileData->pTile = pTileLibraryEntry;

	DRLGROOMTILE_InitializeTileDataFlags(pRoomEx, pTileData, nTileFlags, 13, nX, nY);

	return pTileData;
}

//D2Common.0x6FD88FD0
void __fastcall sub_6FD88FD0(D2RoomExStrc* pRoomEx, int nX, int nY, int nTileFlags)
{
	DRLGROOMTILE_InitShadowTileData(pRoomEx, 0, nX, nY, nTileFlags, DRLGROOMTILE_GetTileCache(pRoomEx, 13, nTileFlags));
}

//D2Common.0x6FD89000
void __fastcall DRLGROOMTILE_LoadInitRoomTiles(D2RoomExStrc* pRoomEx, D2DrlgGridStrc* pDrlgCoordIndex, D2DrlgGridStrc* pTileTypeGrid, BOOL bFillBlanks, BOOL bKillEdgeX, BOOL bKillEdgeY)
{
	unsigned int v18 = 0;
	unsigned int v31 = 0;
	BOOL bContinue = FALSE;

	void* pMemPool = pRoomEx->pLevel->pDrlg->pMempool;

	const int nTileCountX = pRoomEx->nTileWidth + (bKillEdgeX == 0);
	const int nTileCountY = pRoomEx->nTileHeight + (bKillEdgeY == 0);

	const int nX = pRoomEx->nTileXPos;
	const int nY = pRoomEx->nTileYPos;

	for (int nTileY = 0; nTileY < nTileCountY; ++nTileY)
	{
		for (int nTileX = 0; nTileX < nTileCountX; ++nTileX)
		{
			unsigned int nGridFlags = DRLGGRID_GetGridEntry(pDrlgCoordIndex, nTileX, nTileY);

			const int nTileType = pTileTypeGrid ? DRLGGRID_GetGridEntry(pTileTypeGrid, nTileX, nTileY) : 0;

			const uint8_t v17 = (nGridFlags >> 20) & 0x3F;
			v18 = nGridFlags >> 8;
			if ((nTileType != TILETYPE_SPECIALTILES_11 && nTileType != TILETYPE_SPECIALTILES_10) || v17 < 8)
			{
				if (nTileType == TILETYPE_FLOORS && v17 == 30 && (!BYTE1(nGridFlags) || BYTE1(nGridFlags) == 1))
				{
					nGridFlags |= 0x80000000;
				}

				bContinue = FALSE;
				if (nGridFlags & 0x80000000)
				{
					switch (nTileType)
					{
					// Warps
					case TILETYPE_SPECIALTILES_10:
					case TILETYPE_SPECIALTILES_11:
						if (v17 < 8)
						{
							DRLGROOMTILE_AddWarp(pRoomEx, nX + nTileX, nY + nTileY, nGridFlags, nTileType);
							DRLGROOMTILE_LoadFloorWarpTiles(pRoomEx, nX + nTileX, nY + nTileY, nGridFlags, nTileType);
						}
						bContinue = TRUE;
						break;
					// Doors
					case TILETYPE_LEFTWALLWITHDOOR:
					case TILETYPE_RIGHTWALLWITHDOOR:
						if (pRoomEx->pLevel->nLevelId < LEVEL_ID_ACT5_BARRICADE_1 || (pRoomEx->pLevel->nLevelId > LEVEL_ARREATPLATEAU && pRoomEx->pLevel->nLevelId != LEVEL_TUNDRAWASTELANDS))
						{
							DRLGROOMTILE_AddTilePresetUnits(pRoomEx, 0, nGridFlags, nX + nTileX, nY + nTileY, nTileType);
							bContinue = TRUE;
						}
						break;

					default:
						break;
					}
				}

				if (!bContinue)
				{
					if (nGridFlags & 4)
					{
						if (nGridFlags & 2)
						{
							if (v17 == 30 && (!(uint8_t)v18 || (uint8_t)v18 == 1))
							{
								nGridFlags &= 0xFFFFFF7F;
							}


							D2RoomExStrc* pLinkedRoom = nullptr;
							if (D2DrlgTileDataStrc* pTileData = DRLGROOMTILE_GetLinkedTileData(pRoomEx, 1, nGridFlags, nX + nTileX, nY + nTileY, &pLinkedRoom))
							{
								DRLGROOMTILE_LinkedTileDataManager(pMemPool, pRoomEx, pLinkedRoom, pTileData, 0, nGridFlags, nX + nTileX, nY + nTileY);
							}
							else
							{
								DRLGROOMTILE_AddLinkedTileData(pMemPool, pRoomEx, 0, nGridFlags, nX + nTileX, nY + nTileY);
							}
						}
						else if (nGridFlags & 1)
						{
							D2RoomExStrc* pLinkedRoom = nullptr;
							if (D2DrlgTileDataStrc* pTileData = DRLGROOMTILE_GetLinkedTileData(pRoomEx, nTileType == 0, nGridFlags, nX + nTileX, nY + nTileY, &pLinkedRoom))
							{
								DRLGROOMTILE_LinkedTileDataManager(pMemPool, pRoomEx, pLinkedRoom, pTileData, nTileType, nGridFlags, nX + nTileX, nY + nTileY);
							}
							else
							{
								DRLGROOMTILE_AddLinkedTileData(pMemPool, pRoomEx, nTileType, nGridFlags, nX + nTileX, nY + nTileY);
							}
						}
						else if (nGridFlags & 0x8000000 && !(nGridFlags & 0x80000000))
						{
							DRLGROOMTILE_GetCreateLinkedTileData(pMemPool, pRoomEx, 13, nGridFlags, nX + nTileX, nY + nTileY);
						}
					}
					else
					{
						if (nGridFlags & 2)
						{
							DRLGROOMTILE_InitFloorTileData(pRoomEx, 0, nX + nTileX, nY + nTileY, nGridFlags, DRLGROOMTILE_GetTileCache(pRoomEx, 0, nGridFlags));
						}
						else
						{
							if (bFillBlanks && DRLGROOM_AreXYInsideCoordinates(&pRoomEx->pDrlgCoord, nX + nTileX, nY + nTileY))
							{
								v31 = 0x1E00000;
								if (pRoomEx->pLevel->nLevelId == LEVEL_ARCANESANCTUARY)
								{
									v31 |= 0x100;
								}

								DRLGROOMTILE_InitFloorTileData(pRoomEx, 0, nX + nTileX, nY + nTileY, (nGridFlags & 0xFFFFFF7F) | 0x80000000, DRLGROOMTILE_GetTileCache(pRoomEx, 0, v31));
							}
						}

						if (nGridFlags & 1)
						{
							D2TileLibraryEntryStrc* pTileCache = DRLGROOMTILE_GetTileCache(pRoomEx, nTileType, nGridFlags);
							D2DrlgTileDataStrc* pTileData = DRLGROOMTILE_InitWallTileData(pRoomEx, 0, nX + nTileX, nY + nTileY, nGridFlags, pTileCache, nTileType);
							if (nTileType == TILETYPE_SPECIALTILES_11 || nTileType == TILETYPE_SPECIALTILES_10)
							{
								DRLGROOMTILE_LoadWallWarpTiles(pRoomEx, pTileData, nGridFlags, nTileType);
							}
						}

						if (nGridFlags & 0x8000000)
						{
							D2TileLibraryEntryStrc* pTileCache = DRLGROOMTILE_GetTileCache(pRoomEx, 13, nGridFlags);
							DRLGROOMTILE_InitShadowTileData(pRoomEx, 0, nX + nTileX, nY + nTileY, nGridFlags, pTileCache);
						}
					}
				}
			}
		}
	}
}

//D2Common.0x6FD89360
BOOL __fastcall DRLGROOMTILE_AddWarp(D2RoomExStrc* pRoomEx, int nX, int nY, int nTileFlags, int nType)
{
	D2LvlWarpTxt* pLvlWarpTxtRecord = DRLGWARP_GetLvlWarpTxtRecordFromWarpIdAndDirection(pRoomEx->pLevel, ((unsigned int)nTileFlags >> 20) & 0x3F, (((nType != 11) - 1) & 6) + 108);
	int nPosX = 0;
	int nPosY = 0;

	if (pLvlWarpTxtRecord)
	{
		nPosX = nX - pRoomEx->nTileXPos;
		nPosY = nY - pRoomEx->nTileYPos;

		if (nPosX != pRoomEx->nTileWidth && nPosY != pRoomEx->nTileHeight)
		{
			DUNGEON_ExpandCoords(&nPosX, &nPosY);

			nPosX += pLvlWarpTxtRecord->dwOffsetX;
			nPosY += pLvlWarpTxtRecord->dwOffsetY;

			DRLGROOM_AllocPresetUnit(pRoomEx, pRoomEx->pLevel->pDrlg->pMempool, UNIT_TILE, pLvlWarpTxtRecord->dwLevelId, 0, nPosX, nPosY);

			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD89410
void __fastcall DRLGROOMTILE_LoadWallWarpTiles(D2RoomExStrc* pRoomEx, D2DrlgTileDataStrc* pTileData, int nTileFlags, int nType)
{
	D2DrlgTileDataStrc* pDrlgTileData = NULL;
	D2RoomTileStrc* pRoomTile = NULL;
	int nLevelId = 0;
	int nFlags = 0;
	char szDirection = 0;

	pRoomTile = pRoomEx->pRoomTiles;
	nLevelId = DRLGWARP_GetWarpDestinationFromArray(pRoomEx->pLevel, ((unsigned int)nTileFlags >> 20) & 0x3F);
	if (pRoomTile)
	{
		while (pRoomTile->pLvlWarpTxtRecord->dwLevelId != nLevelId)
		{
			pRoomTile = pRoomTile->pNext;
			if (!pRoomTile)
			{
				FOG_10025("ptWarp", __FILE__, __LINE__);
				return;
			}
		}
	}
	else
	{
		FOG_10025("ptWarp", __FILE__, __LINE__);
		return;
	}

	if (BYTE1(nTileFlags) && BYTE1(nTileFlags) != 4 || DRLGROOMTILE_AddWarp(pRoomEx, pRoomEx->nTileXPos + pTileData->nPosX, pTileData->nPosY + pRoomEx->nTileYPos, nTileFlags, nType))
	{
		D2_ASSERT(pRoomTile);
		D2_ASSERT(pRoomTile->pLvlWarpTxtRecord);

		if (pRoomTile->pLvlWarpTxtRecord->szDirection[0] != 'b')
		{
			szDirection = (((nType != 11) - 1) & 6) + 108;
			if (pRoomTile->pLvlWarpTxtRecord->szDirection[0] != szDirection)
			{
				pRoomTile->pLvlWarpTxtRecord = DATATBLS_GetLvlWarpTxtRecordFromLevelIdAndDirection(pRoomTile->pLvlWarpTxtRecord->dwLevelId, szDirection);
			}
		}

		pTileData->unk0x20 = pRoomTile->unk0x10;
		pRoomTile->unk0x10 = pTileData;

		if (!pRoomTile->pLvlWarpTxtRecord)
		{
			FOG_10025("pctWarpDef", __FILE__, __LINE__);
		}

		if (pRoomTile->pLvlWarpTxtRecord->dwLitVersion)
		{
			nFlags = nTileFlags | (pRoomTile->pLvlWarpTxtRecord->dwTiles << 8);
			pDrlgTileData = DRLGROOMTILE_InitWallTileData(pRoomEx, &pRoomTile->unk0x0C, pRoomEx->nTileXPos + pTileData->nPosX, pTileData->nPosY + pRoomEx->nTileYPos, nFlags, DRLGROOMTILE_GetTileCache(pRoomEx, nType, nFlags), nType);

			if (pDrlgTileData)
			{
				pDrlgTileData->dwFlags |= 8;
			}
		}
	}
}

//D2Common.0x6FD89590
//TODO: Rename variables
void __fastcall DRLGROOMTILE_LoadFloorWarpTiles(D2RoomExStrc* pRoomEx, int nX, int nY, unsigned int dwFlags, int nType)
{
	static const D2CoordStrc stru_6FDD1320[] =
	{
		{ 0, 0 },
		{ 1, 0 },
		{ 0, 1 },
		{ 1, 1 }
	};

	D2TileLibraryEntryStrc* pTileLibraryEntry = NULL;
	D2DrlgTileDataStrc* pDrlgTileData = NULL;
	D2RoomTileStrc* pRoomTile = NULL;
	int nLevelId = 0;
	int nHeight = 0;
	int nWidth = 0;
	char szDirection = 0;
	int v13 = 0;
	int v15 = 0;
	int v16 = 0;
	uint8_t v5 = 0;


	v5 = BYTE1(dwFlags);
	pRoomEx->dwFlags |= ROOMEXFLAG_POPULATION_ZERO;
	pRoomTile = pRoomEx->pRoomTiles;

	nLevelId = DRLGWARP_GetWarpDestinationFromArray(pRoomEx->pLevel, (dwFlags >> 20) & 63);
	if (pRoomTile)
	{
		while (pRoomTile->pLvlWarpTxtRecord->dwLevelId != nLevelId)
		{
			pRoomTile = pRoomTile->pNext;
			if (!pRoomTile)
			{
				return;
			}
		}

		D2_ASSERT(pRoomTile->pLvlWarpTxtRecord);

		if (pRoomTile->pLvlWarpTxtRecord->szDirection[0] != 'b')
		{
			szDirection = (nType != 11) ? 'l' : 'r';

			if (pRoomTile->pLvlWarpTxtRecord->szDirection[0] != szDirection)
			{
				pRoomTile->pLvlWarpTxtRecord = DATATBLS_GetLvlWarpTxtRecordFromLevelIdAndDirection(pRoomTile->pLvlWarpTxtRecord->dwLevelId, szDirection);
			}
		}

		if (pRoomTile->pLvlWarpTxtRecord->dwLitVersion)
		{
			v13 = v5;
			v15 = v13 << 12;
			for (int i = 0; i < 4 ; ++i)
			{
				v16 = (i | v15 | 4) << 8;
				pTileLibraryEntry = DRLGROOMTILE_GetTileCache(pRoomEx, 0, v16);

				nWidth = nX - 1 + stru_6FDD1320[i].nX;
				nHeight = nY - 1 + stru_6FDD1320[i].nY;

				pDrlgTileData = &pRoomEx->pTileGrid->pTiles.pFloorTiles[pRoomEx->pTileGrid->nFloors];
				if (!pRoomTile)
				{
					pDrlgTileData->unk0x20 = 0;
				}
				else
				{
					pDrlgTileData->unk0x20 = pRoomTile->unk0x0C;
					pRoomTile->unk0x0C = pDrlgTileData;
				}

				++pRoomEx->pTileGrid->nFloors;

				pDrlgTileData->nTileType = TILETYPE_FLOORS;
				pDrlgTileData->dwFlags = 0;
				pDrlgTileData->unk0x24 = 0;
				pDrlgTileData->pTile = pTileLibraryEntry;
				pDrlgTileData->nGreen = -1;
				pDrlgTileData->nBlue = -1;
				pDrlgTileData->nRed = -1;
				pDrlgTileData->nPosX = nWidth - pRoomEx->nTileXPos;
				pDrlgTileData->nPosY = nHeight - pRoomEx->nTileYPos;

				pDrlgTileData->nWidth = nWidth;
				pDrlgTileData->nHeight = nHeight + 1;
				DUNGEON_ExpandTileCoords(&pDrlgTileData->nWidth, &pDrlgTileData->nHeight);
				pDrlgTileData->nHeight += 40;

				DRLGROOMTILE_InitializeTileDataFlags(pRoomEx, pDrlgTileData, v16, 0, nWidth, nHeight);

				pDrlgTileData->dwFlags |= 8;
			}

			for (int i = 0; i < pRoomEx->pTileGrid->nFloors; ++i)
			{
				if (D2CMP_10078_GetTileStyle(pRoomEx->pTileGrid->pTiles.pFloorTiles[i].pTile) == v13 && (unsigned int)D2CMP_10082_GetTileSequence(pRoomEx->pTileGrid->pTiles.pFloorTiles[i].pTile) < 4)
				{
					pRoomEx->pTileGrid->pTiles.pFloorTiles[i].unk0x20 = pRoomTile->unk0x10;
					pRoomTile->unk0x10 = &pRoomEx->pTileGrid->pTiles.pFloorTiles[i];
				}
			}
		}
	}
}

//D2Common.0x6FD897E0
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_GetLinkedTileData(D2RoomExStrc* pRoomEx, BOOL bFloor, int nTileFlags, int nX, int nY, D2RoomExStrc** ppRoomEx)
{
	D2RoomExStrc* pNearRoomEx = NULL;

	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		pNearRoomEx = pRoomEx->ppRoomsNear[i];

		if (pNearRoomEx != pRoomEx && pNearRoomEx->pTileGrid && DRLGROOM_AreXYInsideCoordinatesOrOnBorder(&pNearRoomEx->pDrlgCoord, nX, nY))
		{
			for (D2DrlgTileLinkStrc* pTileLink = pNearRoomEx->pTileGrid->pMapLinks; pTileLink; pTileLink = pTileLink->pNext)
			{
				if ((!pTileLink->bFloor && !bFloor) || (pTileLink->bFloor && bFloor))
				{
					for (D2DrlgTileDataStrc* pTileData = pTileLink->pMapTile; pTileData; pTileData = pTileData->unk0x20)
					{
						if (pNearRoomEx->nTileXPos + pTileData->nPosX == nX && pNearRoomEx->nTileYPos + pTileData->nPosY == nY)
						{
							if (pTileData->nTileType != TILETYPE_LEFTPARTOFNORTHCORNERWALL && (pTileData->nTileType == TILETYPE_SHADOWS || !(nTileFlags & 0x8000000)))
							{
								if (!(pTileData->dwFlags & 0x1C000) || ((((unsigned int)pTileData->dwFlags >> 14) & 7) - 1 == (((unsigned int)nTileFlags >> 18) & 3)))
								{
									*ppRoomEx = pRoomEx->ppRoomsNear[i];
									return pTileData;
								}
							}
						}
					}
				}
			}
		}
	}

	*ppRoomEx = NULL;
	return NULL;
}

//D2Common.0x6FD89930
void __fastcall DRLGROOMTILE_AddLinkedTileData(void* pMemPool, D2RoomExStrc* pRoomEx, int nType, unsigned int nTileFlags, int nX, int nY)
{
	D2TileLibraryEntryStrc* pTileLibraryEntry = NULL;
	D2DrlgTileDataStrc** ppTileData = NULL;
	D2DrlgTileDataStrc* pTileData = NULL;
	D2DrlgTileLinkStrc* pTileLink = NULL;
	D2DrlgTileLinkStrc* pCurLink = NULL;

	int nPosX = 0;
	int nPosY = 0;

	if (nType != 11 && nType != 10 || DRLGROOM_AreXYInsideCoordinates(&pRoomEx->pDrlgCoord, nX, nY))
	{
		for (pCurLink = pRoomEx->pTileGrid->pMapLinks; pCurLink; pCurLink = pCurLink->pNext)
		{
			if ((pCurLink->bFloor && nType == 0) || (nType != 0))
			{
				// Use current tile
				break;
			}
		}
		if (pCurLink == nullptr)
		{
			pTileLink = D2_ALLOC_STRC_SERVER(pMemPool, D2DrlgTileLinkStrc);
			pTileLink->bFloor = !nType;
			pTileLink->pMapTile = NULL;
			pTileLink->pNext = pRoomEx->pTileGrid->pMapLinks;
			pRoomEx->pTileGrid->pMapLinks = pTileLink;
			pCurLink = pTileLink;
		}

		pTileLibraryEntry = DRLGROOMTILE_GetTileCache(pRoomEx, nType, nTileFlags);

		if (nType)
		{
			if (nType == 13)
			{
				DRLGROOMTILE_InitShadowTileData(pRoomEx, &pCurLink->pMapTile, nX, nY, nTileFlags, pTileLibraryEntry);
			}
			else
			{
				DRLGROOMTILE_InitWallTileData(pRoomEx, &pCurLink->pMapTile, nX, nY, nTileFlags, pTileLibraryEntry, nType);

				if (nType == 11 || nType == 10)
				{
					DRLGROOMTILE_LoadWallWarpTiles(pRoomEx, pCurLink->pMapTile, nTileFlags, nType);
				}
			}
		}
		else
		{
			pTileData = &pRoomEx->pTileGrid->pTiles.pFloorTiles[pRoomEx->pTileGrid->nFloors];

			ppTileData = &pCurLink->pMapTile;
			if (ppTileData)
			{
				pTileData->unk0x20 = *ppTileData;
				*ppTileData = pTileData;
			}
			else
			{
				pTileData->unk0x20 = NULL;
			}

			++pRoomEx->pTileGrid->nFloors;

			pTileData->pTile = pTileLibraryEntry;

			pTileData->nGreen = -1;
			pTileData->nBlue = -1;
			pTileData->nRed = -1;
			pTileData->nTileType = TILETYPE_FLOORS;
			pTileData->dwFlags = 0;
			pTileData->unk0x24 = 0;

			pTileData->nPosX = nX - pRoomEx->nTileXPos;
			pTileData->nPosY = nY - pRoomEx->nTileYPos;

			nPosX = nX;
			nPosY = nY + 1;
			DUNGEON_ExpandTileCoords(&nPosX, &nPosY);

			pTileData->nWidth = nPosX;
			pTileData->nHeight = nPosY + 40;

			DRLGROOMTILE_InitializeTileDataFlags(pRoomEx, pTileData, nTileFlags, 0, nX, nY);
		}
	}
}

//D2Common.0x6FD89AF0
//TODO: Rename variables
void __fastcall DRLGROOMTILE_LinkedTileDataManager(void* pMemPool, D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2, D2DrlgTileDataStrc* pTileData, int nTileType, unsigned int nTileFlags, int nX, int nY)
{
	static const int dword_6FDD1340[] =
	{
		-1, 0, 1, 2, -1, 3, 4, 5, -2, -2, -1, -1, -1, -2, -1, -1, -1, -1, -1
	};
	// Maps tile type < 7 (walls) to ???
	static const int dword_6FDD138C[] =
	{
		-1, 1, 3, 3, 4, 1, 3, 1, 1, 2, 3, 4, 3, 2, 2, 3, 3, 3, 4, 3, 3, 3, 1, 3, 3, 4, 5, 6, 1, 3, 2, 3, 4, 3, 6, 2, 1, 2, 3, 4, 1, 2, 7
	};

	D2TileLibraryEntryStrc* v16 = NULL;
	int v10 = 0;
	BOOL v15 = FALSE;
	BOOL b = FALSE;

	v10 = dword_6FDD1340[nTileType];

	if (pTileData->dwFlags & 1)
	{
		if (TileTypeIsAWallWithDoor(pTileData->nTileType))
		{
			DRLGROOMTILE_InitializeTileDataFlags(pRoomEx1, pTileData, nTileFlags, pTileData->nTileType, nX, nY);
		}

		return;
	}

	if (!(nTileFlags & 0x80))
	{
		if (!TileTypeIsAWallWithDoor(nTileType))
		{
			if (TileTypeIsAWallWithDoor(pTileData->nTileType) && (nX == pRoomEx2->nTileXPos || nY == pRoomEx2->nTileYPos))
			{
				return;
			}

			b = TRUE;
		}

		if (b || nX != pRoomEx1->nTileXPos && nY != pRoomEx1->nTileYPos)
		{
			bool bIsTileTypeANormalWallOrFloor = pTileData->nTileType <= TILETYPE_SOUTHCORNERWALL;
			if (v10 < 0 || !bIsTileTypeANormalWallOrFloor)
			{
				if (v10 != -1)
				{
					return;
				}
			}
			else
			{
				nTileType = dword_6FDD138C[pTileData->nTileType + 7 * v10];
			}
		}
	}

	if (pTileData->nTileType == TILETYPE_RIGHTPARTOFNORTHCORNERWALL)
	{
		if (nTileType != TILETYPE_RIGHTPARTOFNORTHCORNERWALL)
		{
			pTileData->unk0x20->dwFlags |= 8;
			D2Common_COLLISION_FirstFn_6FD41000(pRoomEx2->pRoom, pTileData->unk0x20, 0);
		}
	}
	else
	{
		if (nTileType == TILETYPE_RIGHTPARTOFNORTHCORNERWALL)
		{
			pTileData->dwFlags |= 0xC008;
			D2Common_COLLISION_FirstFn_6FD41000(pRoomEx2->pRoom, pTileData, 0);
			DRLGROOMTILE_AddLinkedTileData(pMemPool, pRoomEx1, 3, nTileFlags, nX, nY);
		}
	}

	v15 = pTileData->nTileType == TILETYPE_FLOORS && D2CMP_10078_GetTileStyle(pTileData->pTile) == 30 && !D2CMP_10082_GetTileSequence(pTileData->pTile);
	if (nTileType != pTileData->nTileType || v15)
	{
		v16 = DRLGROOMTILE_GetTileCache(pRoomEx2, nTileType, nTileFlags);
		pTileData->nTileType = nTileType;

		if (v16 != pTileData->pTile && pRoomEx2->pRoom)
		{
			D2Common_COLLISION_FirstFn_6FD41000(pRoomEx2->pRoom, pTileData, v16);
		}

		pTileData->pTile = v16;
	}

	DRLGROOMTILE_InitializeTileDataFlags(pRoomEx1, pTileData, nTileFlags, pTileData->nTileType, nX, nY);
}

//D2Common.0x6FD89CC0
void __fastcall DRLGROOMTILE_GetCreateLinkedTileData(void* pMemPool, D2RoomExStrc* pRoomEx, int nTileType, unsigned int nTileFlags, int nX, int nY)
{
	D2DrlgTileLinkStrc* pTileLink = NULL;
	D2DrlgTileDataStrc* pTileData = NULL;
	D2RoomExStrc* pNearRoomEx = NULL;

	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		pNearRoomEx = pRoomEx->ppRoomsNear[i];
		if (pNearRoomEx != pRoomEx && pNearRoomEx->pTileGrid && DRLGROOM_AreXYInsideCoordinatesOrOnBorder(&pNearRoomEx->pDrlgCoord, nX, nY))
		{
			pTileLink = pNearRoomEx->pTileGrid->pMapLinks;
			while (pTileLink)
			{
				pTileData = pTileLink->pMapTile;

				if ((!pTileLink->bFloor && nTileType != TILETYPE_FLOORS) || (pTileLink->bFloor && nTileType == TILETYPE_FLOORS))
				{
					while (pTileData)
					{
						if (pNearRoomEx->nTileXPos + pTileData->nPosX == nX && pNearRoomEx->nTileYPos + pTileData->nPosY == nY)
						{
							if (pTileData->nTileType != TILETYPE_LEFTPARTOFNORTHCORNERWALL && (pTileData->nTileType == TILETYPE_SHADOWS || !(nTileFlags & 0x8000000)))
							{
								if (!(pTileData->dwFlags & 0x1C000))
								{
									return DRLGROOMTILE_LinkedTileDataManager(pMemPool, pRoomEx, pRoomEx->ppRoomsNear[i], pTileData, nTileType, nTileFlags, nX, nY);
								}

								if ((((unsigned int)pTileData->dwFlags >> 14) & 7) - 1 == ((nTileFlags >> 18) & 3))
								{
									return DRLGROOMTILE_LinkedTileDataManager(pMemPool, pRoomEx, pRoomEx->ppRoomsNear[i], pTileData, nTileType, nTileFlags, nX, nY);
								}
							}
						}

						pTileData = pTileData->unk0x20;
					}
				}

				pTileLink = pTileLink->pNext;
			}
		}
	}

	DRLGROOMTILE_AddLinkedTileData(pMemPool, pRoomEx, nTileType, nTileFlags, nX, nY);
}

//D2Common.0x6FD89E30
void __fastcall DRLGROOMTILE_CountAllTileTypes(D2RoomExStrc* pRoomEx, D2DrlgGridStrc* pDrlgCoordIndex, BOOL bCheckCoordinatesValidity, BOOL bKillEdgeX, BOOL bKillEdgeY)
{
	int* pFlags = 0;
	int nHeight = 0;
	int nWidth = 0;

	nWidth = pRoomEx->nTileWidth + (bKillEdgeX == 0);
	nHeight = pRoomEx->nTileHeight + (bKillEdgeY == 0);

	for (int nY = 0; nY < nHeight; ++nY)
	{
		for (int nX = 0; nX < nWidth; ++nX)
		{
			pFlags = DRLGGRID_GetGridFlagsPointer(pDrlgCoordIndex, nX, nY);
			if (*pFlags & 1)
			{
				++pRoomEx->pTileGrid->pTiles.nWalls;
			}

			if (*pFlags & 2 || bCheckCoordinatesValidity && DRLGROOM_AreXYInsideCoordinates(&pRoomEx->pDrlgCoord, nX + pRoomEx->nTileXPos, nY + pRoomEx->nTileYPos))
			{
				++pRoomEx->pTileGrid->pTiles.nFloors;
			}

			if (*pFlags & 0x8000000)
			{
				++pRoomEx->pTileGrid->pTiles.nRoofs;
			}
		}
	}
}

//D2Common.0x6FD89F00
void __fastcall DRLGROOMTILE_CountWallWarpTiles(D2RoomExStrc* pRoomEx, D2DrlgGridStrc* pDrlgCoordIndex, D2DrlgGridStrc* pOutdoorRoom, BOOL bKillEdgeX, BOOL bKillEdgeY)
{
	int v9 = 0;

	for (int j = 0; j < pRoomEx->nTileHeight + (bKillEdgeY == 0); ++j)
	{
		for (int i = 0; i < pRoomEx->nTileWidth + (bKillEdgeX == 0); ++i)
		{
			v9 = DRLGGRID_GetGridEntry(pOutdoorRoom, i, j);

			if (v9 == 3)
			{
				++pRoomEx->pTileGrid->pTiles.nWalls;
			}
			else
			{
				if (v9 > 9 && v9 <= 11)
				{
					if (DRLGGRID_GetGridEntry(pDrlgCoordIndex, i, j) < 0)
					{
						pRoomEx->pTileGrid->pTiles.nFloors += 6;
					}
					else
					{
						++pRoomEx->pTileGrid->pTiles.nWalls;
					}
				}
			}
		}
	}
}

//D2Common.0x6FD89FA0
void __fastcall DRLGROOMTILE_InitRoomGrids(D2RoomExStrc* pRoomEx)
{
	SEED_InitLowSeed(&pRoomEx->pSeed, pRoomEx->dwInitSeed);

	if (pRoomEx->nType == DRLGTYPE_MAZE)
	{
		DRLGOUTPLACE_InitOutdoorRoomGrids(pRoomEx);
	}
	else if (pRoomEx->nType == DRLGTYPE_PRESET)
	{
		DRLGPRESET_InitPresetRoomGrids(pRoomEx);
	}
}

//D2Common.0x6FD89FD0
void __fastcall DRLGROOMTILE_AddRoomMapTiles(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->nType == DRLGTYPE_MAZE)
	{
		DRLGOUTROOM_InitializeDrlgOutdoorRoom(pRoomEx);
	}
	else if (pRoomEx->nType == DRLGTYPE_PRESET)
	{
		DRLGPRESET_AddPresetRoomMapTiles(pRoomEx);
	}

	pRoomEx->dwFlags |= ROOMEXFLAG_HAS_ROOM;
}

//D2Common.0x6FD8A010
void __fastcall DRLGROOMTILE_AllocTileGrid(D2RoomExStrc* pRoomEx)
{
	if (!pRoomEx->pTileGrid)
	{
		pRoomEx->pTileGrid = D2_CALLOC_STRC_SERVER(pRoomEx->pLevel->pDrlg->pMempool, D2DrlgTileGridStrc);
	}
}

//D2Common.0x6FD8A050
void __fastcall DRLGROOMTILE_AllocTileData(D2RoomExStrc* pRoomEx)
{
	void* pMemPool = pRoomEx->pLevel->pDrlg->pMempool;
	D2DrlgRoomTilesStrc* pRoomTiles = &pRoomEx->pTileGrid->pTiles;

	if (pRoomTiles->nFloors)
	{
		pRoomTiles->pFloorTiles = (D2DrlgTileDataStrc*)D2_CALLOC_SERVER(pMemPool, sizeof(D2DrlgTileDataStrc) * pRoomTiles->nFloors);
	}

	if (pRoomTiles->nWalls)
	{
		pRoomTiles->pWallTiles = (D2DrlgTileDataStrc*)D2_CALLOC_SERVER(pMemPool, sizeof(D2DrlgTileDataStrc) * pRoomTiles->nWalls);
	}

	// Something may already have allocated roof tiles with DRLGROOMTILE_ReallocRoofTileGrid!
	if (pRoomTiles->nRoofs && !pRoomTiles->pRoofTiles)
	{
		pRoomTiles->pRoofTiles = (D2DrlgTileDataStrc*)D2_CALLOC_SERVER(pMemPool, sizeof(D2DrlgTileDataStrc) * pRoomTiles->nRoofs);
	}
}

//D2Common.0x6FD8A130
void __fastcall DRLGROOMTILE_ReallocRoofTileGrid(void* pMemPool, D2DrlgTileGridStrc* pTileGrid, int nRoofs)
{
	int nCounter = 0;

	if (nRoofs)
	{
		pTileGrid->pTiles.pRoofTiles = (D2DrlgTileDataStrc*)D2_REALLOC_SERVER(pMemPool, pTileGrid->pTiles.pRoofTiles, sizeof(D2DrlgTileDataStrc) * (nRoofs + pTileGrid->pTiles.nRoofs));
		
		while (nCounter < pTileGrid->nShadows - 1)
		{
			pTileGrid->pTiles.pRoofTiles[nCounter].unk0x20 = &pTileGrid->pTiles.pRoofTiles[nCounter + 1];
			++nCounter;
		}

		pTileGrid->pTiles.pRoofTiles[nCounter].unk0x20 = NULL;
		pTileGrid->pTiles.nRoofs += nRoofs;
	}
}

//D2Common.0x6FD8A1B0 (#10017)
int __fastcall DRLGROOMTILE_GetNumberOfShadowsFromRoom(D2RoomStrc* pRoom)
{
	if (pRoom)
	{
		return DUNGEON_GetRoomExFromRoom(pRoom)->pTileGrid->nShadows;
	}

	return 0;
}

//D2Common.0x6FD8A1D0
void __fastcall DRLGROOMTILE_FreeTileGrid(D2RoomExStrc* pRoomEx)
{
	D2DrlgAnimTileGridStrc* pNextAnimTileGrid = NULL;
	D2DrlgAnimTileGridStrc* pAnimTileGrid = NULL;
	D2DrlgTileLinkStrc* pNextTileLink = NULL;
	D2DrlgTileLinkStrc* pTileLink = NULL;
	D2DrlgTileGridStrc* pTileGrid = NULL;

	pTileGrid = pRoomEx->pTileGrid;

	if (pTileGrid)
	{
		if (pTileGrid->pTiles.pWallTiles)
		{
			D2_FREE_SERVER(pRoomEx->pLevel->pDrlg->pMempool, pTileGrid->pTiles.pWallTiles);
		}

		if (pTileGrid->pTiles.pFloorTiles)
		{
			D2_FREE_SERVER(pRoomEx->pLevel->pDrlg->pMempool, pTileGrid->pTiles.pFloorTiles);
		}

		if (pTileGrid->pTiles.pRoofTiles)
		{
			D2_FREE_SERVER(pRoomEx->pLevel->pDrlg->pMempool, pTileGrid->pTiles.pRoofTiles);
		}

		pTileLink = pTileGrid->pMapLinks;
		while (pTileLink)
		{
			pNextTileLink = pTileLink->pNext;
			D2_FREE_SERVER(pRoomEx->pLevel->pDrlg->pMempool, pTileLink);
			pTileLink = pNextTileLink;
		}
		pTileGrid->pMapLinks = NULL;

		pAnimTileGrid = pTileGrid->pAnimTiles;
		while (pAnimTileGrid)
		{
			pNextAnimTileGrid = pAnimTileGrid->pNext;
			D2_FREE_SERVER(pRoomEx->pLevel->pDrlg->pMempool, pAnimTileGrid->ppMapTileData);
			D2_FREE_SERVER(pRoomEx->pLevel->pDrlg->pMempool, pAnimTileGrid);
			pAnimTileGrid = pNextAnimTileGrid;
		}

		D2_FREE_SERVER(pRoomEx->pLevel->pDrlg->pMempool, pTileGrid);
		pRoomEx->pTileGrid = NULL;
	}
}

//D2Common.0x6FD8A2E0
void __fastcall sub_6FD8A2E0(D2RoomExStrc* pRoomEx, BOOL bKeepRoom)
{
	if (!bKeepRoom && pRoomEx->pRoom)
	{
		DUNGEON_RemoveRoomFromAct(pRoomEx->pLevel->pDrlg->pAct, pRoomEx->pRoom);
	}

	pRoomEx->pRoom = NULL;

	DRLGLOGIC_FreeDrlgCoordList(pRoomEx);

	if (pRoomEx->dwFlags & ROOMEXFLAG_HAS_ROOM)
	{
		pRoomEx->dwFlags ^= ROOMEXFLAG_HAS_ROOM;
		++pRoomEx->pLevel->pDrlg->nFreedRooms;
		DRLGROOMTILE_FreeTileGrid(pRoomEx);

		if (pRoomEx->nType == DRLGTYPE_MAZE)
		{
			DRLGOUTROOM_FreeDrlgOutdoorRoomData(pRoomEx);
		}
		else if (pRoomEx->nType == DRLGTYPE_PRESET)
		{
			DRLGPRESET_FreeDrlgGridsFromPresetRoom(pRoomEx);
		}

		for (D2RoomTileStrc* i = pRoomEx->pRoomTiles; i; i = i->pNext)
		{
			i->unk0x0C = NULL;
			i->unk0x10 = NULL;
		}

		if (bKeepRoom)
		{
			pRoomEx->dwFlags |= ROOMEXFLAG_ROOM_FREED_SRV;
		}
	}
}

//D2Common.0x6FD8A380
void __fastcall DRLGROOMTILE_LoadDT1FilesForRoom(D2RoomExStrc* pRoomEx)
{
	D2LvlTypesTxt* pLvlTypesTxtRecord = NULL;
	unsigned int dwDT1Mask = 0;
	int nCounter = 0;
	char szPath[MAX_PATH] = {};

	pLvlTypesTxtRecord = DATATBLS_GetLevelTypesTxtRecord(pRoomEx->pLevel->nLevelType);

	dwDT1Mask = pRoomEx->dwDT1Mask;

	while (dwDT1Mask && nCounter < 32)
	{
		if (dwDT1Mask & 1)
		{
			D2CMP_10087_LoadTileLibrarySlot(pRoomEx->pTiles, pLvlTypesTxtRecord->szFile[nCounter]);
		}

		dwDT1Mask >>= 1;

		++nCounter;
	}

	szPath[0] = sgptDataTables->szDefaultString;

	wsprintfA(szPath, "%s\\Tiles\\Act1\\Outdoors\\Blank.dt1", "DATA\\GLOBAL");
	D2CMP_10087_LoadTileLibrarySlot(pRoomEx->pTiles, szPath);

	wsprintfA(szPath, "%s\\Tiles\\Act1\\Barracks\\InvisWal.dt1", "DATA\\GLOBAL");
	D2CMP_10087_LoadTileLibrarySlot(pRoomEx->pTiles, szPath);

	wsprintfA(szPath, "%s\\Tiles\\Act1\\Barracks\\Warp.dt1", "DATA\\GLOBAL");
	D2CMP_10087_LoadTileLibrarySlot(pRoomEx->pTiles, szPath);

	pRoomEx->dwFlags |= ROOMEXFLAG_TILELIB_LOADED;
}
