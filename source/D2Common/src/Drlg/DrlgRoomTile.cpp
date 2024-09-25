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
	int nTileStyle;
	int nTileSequence;
	BOOL bIsRightWallWithDoor;
	int nUnitId;
	int nUnitType;
	int nX;
	int nY;
};



//D2Common.0x6FD88860
D2TileLibraryEntryStrc* __fastcall DRLGROOMTILE_GetTileCache(D2DrlgRoomStrc* pDrlgRoom, int nType, uint32_t nPackedTileInformation)
{
	D2TileLibraryEntryStrc* ppTileLibraryEntries[40] = {};

	D2C_PackedTileInformation nTileInformation{ nPackedTileInformation };
	const uint32_t nStyle = nTileInformation.nTileStyle;
	const uint32_t nSequence = nTileInformation.nTileSequence;

	if (int nEntries = D2CMP_10088_GetTiles(pDrlgRoom->pTiles, nType, nStyle, nSequence, ppTileLibraryEntries, ARRAY_SIZE(ppTileLibraryEntries)))
	{
		int nMax = 0;
		for (int i = 0; i < nEntries; ++i)
		{
			if (!ppTileLibraryEntries[i])
			{
				FOG_DisplayWarning("phTileArray[ii]", __FILE__, __LINE__);
			}

			nMax += D2CMP_10081_GetTileRarity(ppTileLibraryEntries[i]);
		}

		int nId = 0;
		int32_t nRand = SEED_RollLimitedRandomNumber(&pDrlgRoom->pSeed, nMax) + 1;
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
		if (!D2CMP_10088_GetTiles(pDrlgRoom->pTiles, TILETYPE_WALL_LEFT_EXIT, 0, 0, ppTileLibraryEntries, ARRAY_SIZE(ppTileLibraryEntries)))
		{
			FOG_DisplayWarning("nSize", __FILE__, __LINE__);
		}

		return ppTileLibraryEntries[0];
	}
}

// Helper function
static void DRLGROOMTILE_InitTileDataDefaults(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc* pTileData, int nX, int nY, uint32_t nPackedTileInformation, int nTileType, D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	pTileData->pTile = pTileLibraryEntry;
	pTileData->nTileType = nTileType;
	pTileData->dwFlags = MAPTILE_FLAGS_NONE;
	pTileData->unk0x24 = 0;
	pTileData->nGreen = 0xFF;
	pTileData->nBlue = 0xFF;
	pTileData->nRed = 0xFF;

	if (pDrlgRoom)
	{
		pTileData->nPosX = nX - pDrlgRoom->nTileXPos;
		pTileData->nPosY = nY - pDrlgRoom->nTileYPos;

		int nPosX = nX;
		int nPosY = nY + 1;

		DUNGEON_GameTileToClientCoords(&nPosX, &nPosY);

		pTileData->nWidth = nPosX;
		pTileData->nHeight = nPosY + DRLGROOMTILE_SUBTILES_SIZE;
	}

	DRLGROOMTILE_InitializeTileDataFlags(pDrlgRoom, pTileData, nPackedTileInformation, nTileType, nX, nY);
}

//D2Common.0x6FD889C0
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_InitWallTileData(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc** ppTileData, int nX, int nY, uint32_t nPackedTileInformation, D2TileLibraryEntryStrc* pTileLibraryEntry, int nTileType)
{
	D2DrlgTileDataStrc* pTileData = pTileData = &pDrlgRoom->pTileGrid->pTiles.pWallTiles[pDrlgRoom->pTileGrid->nWalls];
	if (ppTileData)
	{
		pTileData->unk0x20 = *ppTileData;
		*ppTileData = pTileData;
	}
	else
	{
		pTileData->unk0x20 = nullptr;
	}

	++pDrlgRoom->pTileGrid->nWalls;

	DRLGROOMTILE_InitTileDataDefaults(pDrlgRoom, pTileData, nX, nY, nPackedTileInformation, nTileType, pTileLibraryEntry);

	if (nTileType == TILETYPE_WALL_TOP_CORNER_RIGHT)
	{
		DRLGROOMTILE_InitializeTileDataFlags(pDrlgRoom, DRLGROOMTILE_InitWallTileData(pDrlgRoom, ppTileData, nX, nY, nPackedTileInformation, DRLGROOMTILE_GetTileCache(pDrlgRoom, TILETYPE_WALL_TOP_CORNER_LEFT, nPackedTileInformation), TILETYPE_WALL_TOP_CORNER_LEFT), nPackedTileInformation, TILETYPE_WALL_TOP_CORNER_LEFT, nX, nY);
	}

	return pTileData;
}

//D2Common.0x6FD88AC0
void __fastcall DRLGROOMTILE_InitializeTileDataFlags(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc* pTileData, uint32_t nTileFlags, int nType, int nX, int nY)
{
	const D2C_PackedTileInformation nTileInfo{ nTileFlags };

	if (pDrlgRoom && (nType == TILETYPE_WALL_RIGHT_DOOR || nType == TILETYPE_WALL_LEFT_DOOR))
	{
		DRLGROOMTILE_AddTilePresetUnits(pDrlgRoom, pTileData, nTileFlags, nX, nY, 0);
	}

	if (nType != TILETYPE_SHADOW)
	{
		pTileData->dwFlags |= (nTileInfo.nWallLayer + 1) << MAPTILE_WALL_LAYER_BIT;
	}

	if (nType == TILETYPE_TREE)
	{
		pTileData->dwFlags |= MAPTILE_TREES;
	}
	else if (nType == TILETYPE_WALL_RIGHT_EXIT || nType == TILETYPE_WALL_LEFT_EXIT || nType == TILETYPE_WALL_RIGHT_DOOR || nType == TILETYPE_WALL_LEFT_DOOR)
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

	if (!nTileInfo.bHidden)
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
void __fastcall DRLGROOMTILE_AddTilePresetUnits(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc* pTileData, uint32_t nPackedTileInformation, int nX, int nY, int nTileType)
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
		{ 7, 0, true,  OBJECT_DOORGATERIGHT, UNIT_OBJECT, 5, 0 },
		{ 7, 0, false, OBJECT_DOORGATELEFT, UNIT_OBJECT, 0, 5 },
		{ 5, 0, true,  OBJECT_DOORWOODENRIGHT, UNIT_OBJECT, 0, 0 },
		{ 5, 0, false, OBJECT_DOORWOODENLEFT, UNIT_OBJECT, 0, 0 },
		{ 6, 0, true,  OBJECT_DOORMONASTERYDOUBLERIGHT, UNIT_OBJECT, 5, -2 },
		{ 4, 0, true,  OBJECT_DOORCOURTYARDRIGHT, UNIT_OBJECT, 1, 2 },
		{ 4, 0, false, OBJECT_DOORCOURTYARDLEFT, UNIT_OBJECT, 0, 0 },
		{ 4, 3, true,  OBJECT_DOORCATHEDRALDOUBLE, UNIT_OBJECT, 1, 0 },
		{ 1, 2, false, OBJECT_DOORCATHEDRALLEFT, UNIT_OBJECT, 0, 3 },
		{ 1, 2, true,  OBJECT_DOORCATHEDRALRIGHT, UNIT_OBJECT, 3, 0 },
		{ 0, 0, true,  OBJECT_DOORWOODENRIGHT, UNIT_OBJECT, 0, 0 },
		{ 0, 0, false, OBJECT_DOORWOODENLEFT2, UNIT_OBJECT, 0, 0 },
		{ 2, 0, true,  OBJECT_ANDARIELS_DOOR, UNIT_OBJECT, 5, 0 },
		{ 0, 1, true,  OBJECT_IRONGRATEDOORRIGHT, UNIT_OBJECT, 2, 0 },
		{ 0, 1, false, OBJECT_IRONGRATEDOORLEFT, UNIT_OBJECT, 0, 2 },
		{ 5, 0, true,  OBJECT_WOODENGRATEDOORRIGHT, UNIT_OBJECT, 2, 0 },
		{ 4, 0, false, OBJECT_WOODENGRATEDOORLEFT, UNIT_OBJECT, 0, 2 },
		{ 0, 0, true,  OBJECT_WOODENDOORRIGHT, UNIT_OBJECT, 2, 0 },
		{ 0, 0, false, OBJECT_WOODENDOORLEFT, UNIT_OBJECT, 0, 2 },
		{ 2, 4, true,  OBJECT_TOMBDOORRIGHT, UNIT_OBJECT, 1, 0 },
		{ 2, 1, false, OBJECT_TOMBDOORLEFT, UNIT_OBJECT, 0, 2 },
		{ 0, 1, true,  OBJECT_SLIMEDOOR1, UNIT_OBJECT, 0, 0 },
		{ 0, 1, false, OBJECT_SLIMEDOOR2, UNIT_OBJECT, 0, 0 },
		{ 3, 3, false, OBJECT_HARROGATH_TOWN_MAIN_GATE, UNIT_OBJECT, -2, 4 },

		{ 2,  1, false, MONSTER_BARRICADETOWER, UNIT_MONSTER, 1, 2 },
		{ 2,  1, true,  MONSTER_BARRICADETOWER, UNIT_MONSTER, 2, 1 },
		{ 2,  6, false, MONSTER_BARRICADETOWER, UNIT_MONSTER, 1, 1 },
		{ 2,  2, false, MONSTER_BARRICADEDOOR2, UNIT_MONSTER, 0, 1 },
		{ 2,  3, true,  MONSTER_BARRICADEDOOR1, UNIT_MONSTER, 1, 0 },
		{ 26, 0, false, MONSTER_PRISONDOOR, UNIT_MONSTER, 0, 1 },
		{ 2,  4, true,  MONSTER_BARRICADEWALL1, UNIT_MONSTER, 0, 0 },
		{ 2,  4, false, MONSTER_BARRICADEWALL2, UNIT_MONSTER, 0, 0 },

		{ 29, 0, true,  OBJECT_PERMANENT_TOWN_PORTAL, UNIT_OBJECT, 2, 0 },
		{ 29, 0, false, OBJECT_PERMANENT_TOWN_PORTAL, UNIT_OBJECT, 0, 2 },
	};

	BOOL bIsRightWallWithDoor = 0;

	if (pTileData)
	{
		if (pTileData->dwFlags & MAPTILE_HASPRESETUNITS)
		{
			return;
		}

		bIsRightWallWithDoor = pTileData->nTileType == TILETYPE_WALL_RIGHT_DOOR;
	}
	else
	{
		bIsRightWallWithDoor = nTileType == TILETYPE_WALL_RIGHT_DOOR;
	}

	for (int i = 0; i < ARRAY_SIZE(stru_6FDD0DA8); ++i)
	{
		if (stru_6FDD0DA8[i].nLevelId == pDrlgRoom->pLevel->nLevelId)
		{
			D2C_PackedTileInformation nTileInformation{ nPackedTileInformation };
			const uint32_t nTileStyle = nTileInformation.nTileStyle;
			const uint32_t nTileSequence = nTileInformation.nTileSequence;

			for (int j = stru_6FDD0DA8[i].nArrayStartId; j <= stru_6FDD0DA8[i].nArrayEndId; ++j)
			{
				if (nTileStyle == stru_6FDD0F68[j].nTileStyle && nTileSequence == stru_6FDD0F68[j].nTileSequence && bIsRightWallWithDoor == stru_6FDD0F68[j].bIsRightWallWithDoor)
				{
					int nPosX = nX - pDrlgRoom->nTileXPos;
					int nPosY = nY - pDrlgRoom->nTileYPos;

					DUNGEON_GameTileToSubtileCoords(&nPosX, &nPosY);

					nPosX += stru_6FDD0F68[j].nX;
					nPosY += stru_6FDD0F68[j].nY;

					if (nPosX >= 0 && nPosY >= 0 && nPosX < 5 * pDrlgRoom->nTileWidth && nPosY < 5 * pDrlgRoom->nTileHeight)
					{
						const int nUnitId = stru_6FDD0F68[j].nUnitId;

						switch (stru_6FDD0F68[j].nUnitType)
						{
						case UNIT_MONSTER:
							DRLGROOM_AllocPresetUnit(pDrlgRoom, pDrlgRoom->pLevel->pDrlg->pMempool, UNIT_MONSTER, MONSTERS_ValidateMonsterId(nUnitId), MONMODE_NEUTRAL, nPosX, nPosY);
							break;

						case UNIT_OBJECT:
							if (nUnitId < 91 || nUnitId > 92 || (unsigned int)SEED_RollRandomNumber(&pDrlgRoom->pSeed) % 3)
							{
								DRLGROOM_AllocPresetUnit(pDrlgRoom, pDrlgRoom->pLevel->pDrlg->pMempool, UNIT_OBJECT, nUnitId, OBJMODE_NEUTRAL, nPosX, nPosY);
							}
							break;

						default:
							DRLGROOM_AllocPresetUnit(pDrlgRoom, pDrlgRoom->pLevel->pDrlg->pMempool, stru_6FDD0F68[j].nUnitType, nUnitId, 0, nPosX, nPosY);
							break;
						}

						if (pTileData)
						{
							pTileData->dwFlags |= MAPTILE_HASPRESETUNITS;
						}
					}
					return;
				}
			}
		}
	}
}

//D2Common.0x6FD88DD0
void __fastcall DRLGROOMTILE_InitTileData(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc* pTileData, int nX, int nY, uint32_t nPackedTileInformation, D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	DRLGROOMTILE_InitTileDataDefaults(pDrlgRoom, pTileData, nX, nY, nPackedTileInformation, TILETYPE_FLOOR, pTileLibraryEntry);
}

//D2Common.0x6FD88E60
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_InitFloorTileData(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc** ppTileData, int nX, int nY, uint32_t nPackedTileInformation, D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2DrlgTileDataStrc* pTileData = &pDrlgRoom->pTileGrid->pTiles.pFloorTiles[pDrlgRoom->pTileGrid->nFloors];
	if (ppTileData)
	{
		pTileData->unk0x20 = *ppTileData;
		*ppTileData = pTileData;
	}
	else
	{
		pTileData->unk0x20 = nullptr;
	}

	++pDrlgRoom->pTileGrid->nFloors;

	DRLGROOMTILE_InitTileData(pDrlgRoom, pTileData, nX, nY, nPackedTileInformation, pTileLibraryEntry);
	return pTileData;
}

//D2Common.0x6FD88F10
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_InitShadowTileData(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc** ppTileData, int nX, int nY, uint32_t nPackedTileInformation, D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2DrlgTileDataStrc* pTileData = &pDrlgRoom->pTileGrid->pTiles.pRoofTiles[pDrlgRoom->pTileGrid->nShadows];
	if (ppTileData)
	{
		pTileData->unk0x20 = *ppTileData;
		*ppTileData = pTileData;
	}
	else
	{
		pTileData->unk0x20 = 0;
	}

	++pDrlgRoom->pTileGrid->nShadows;

	DRLGROOMTILE_InitTileDataDefaults(pDrlgRoom, pTileData, nX, nY, nPackedTileInformation, TILETYPE_SHADOW, pTileLibraryEntry);

	return pTileData;
}

//D2Common.0x6FD88FD0
void __fastcall DRLGROOMTILE_InitTileShadow(D2DrlgRoomStrc* pDrlgRoom, int nX, int nY, uint32_t nPackedTileInformation)
{
	DRLGROOMTILE_InitShadowTileData(pDrlgRoom, nullptr, nX, nY, nPackedTileInformation, DRLGROOMTILE_GetTileCache(pDrlgRoom, TILETYPE_SHADOW, nPackedTileInformation));
}

//D2Common.0x6FD89000
void __fastcall DRLGROOMTILE_LoadInitRoomTiles(D2DrlgRoomStrc* pDrlgRoom, D2DrlgGridStrc* pTilePackedInfoGrid, D2DrlgGridStrc* pTileTypeGrid, BOOL bFillBlanks, BOOL bKillEdgeX, BOOL bKillEdgeY)
{
	void* pMemPool = pDrlgRoom->pLevel->pDrlg->pMempool;

	const int nTileCountX = pDrlgRoom->nTileWidth + (bKillEdgeX == 0);
	const int nTileCountY = pDrlgRoom->nTileHeight + (bKillEdgeY == 0);

	const int nRoomX = pDrlgRoom->nTileXPos;
	const int nRoomY = pDrlgRoom->nTileYPos;

	for (int nTileOffsetY = 0; nTileOffsetY < nTileCountY; ++nTileOffsetY)
	{
		for (int nTileOffsetX = 0; nTileOffsetX < nTileCountX; ++nTileOffsetX)
		{
			const uint32_t nPackedTileInformation = DRLGGRID_GetGridEntry(pTilePackedInfoGrid, nTileOffsetX, nTileOffsetY);
			D2C_PackedTileInformation nTileInformation{ nPackedTileInformation };
			const uint8_t nTileStyle = nTileInformation.nTileStyle;
			const uint8_t nTileSequence = nTileInformation.nTileSequence;

			const int nTileType = pTileTypeGrid ? DRLGGRID_GetGridEntry(pTileTypeGrid, nTileOffsetX, nTileOffsetY) : TILETYPE_FLOOR;
			
			if ((nTileType == TILETYPE_WALL_LEFT_EXIT || nTileType == TILETYPE_WALL_RIGHT_EXIT) && nTileStyle >= 8)
			{
				continue;
			}

			if (nTileType == TILETYPE_FLOOR && nTileStyle == 30 && (nTileSequence == 0 || nTileSequence == 1))
			{
				nTileInformation.bHidden = true;
			}

			const int nTilePosX = nRoomX + nTileOffsetX;
			const int nTilePosY = nRoomY + nTileOffsetY;

			if (nTileInformation.bHidden)
			{
				bool bTileAdded = false;
				switch (nTileType)
				{
				// Doors
				case TILETYPE_WALL_LEFT_DOOR:
				case TILETYPE_WALL_RIGHT_DOOR:
				{
					const int nLevelId = pDrlgRoom->pLevel->nLevelId;
					if (nLevelId == LEVEL_ID_ACT5_BARRICADE_1 
						|| nLevelId == LEVEL_ARREATPLATEAU 
						|| nLevelId == LEVEL_TUNDRAWASTELANDS)
					{
						break; // ignore doors for those levels
					}
					DRLGROOMTILE_AddTilePresetUnits(pDrlgRoom, nullptr, nTileInformation.nPackedValue, nTilePosX, nTilePosY, nTileType);
					bTileAdded = true;
					break;
				}
				// Warps
				case TILETYPE_WALL_LEFT_EXIT:
				case TILETYPE_WALL_RIGHT_EXIT:
					if (nTileStyle < 8)
					{
						DRLGROOMTILE_AddWarp(pDrlgRoom, nTilePosX, nTilePosY, nTileInformation.nPackedValue, nTileType);
						DRLGROOMTILE_LoadFloorWarpTiles(pDrlgRoom, nTilePosX, nTilePosY, nTileInformation.nPackedValue, nTileType);
					}
					bTileAdded = true;
					break;

				default:
					break;
				}
				if (bTileAdded)
				{
					continue;
				}
			}

			bool bTileAdded = false;
			if (nTileInformation.bLOS)
			{
				bTileAdded = true;
				if (nTileInformation.bIsFloor)
				{
					if (nTileStyle == 30 && (nTileSequence == 0 || nTileSequence == 1))
					{
						nTileInformation.bLayerAbove = false;
					}


					D2DrlgRoomStrc* pLinkedRoom = nullptr;
					if (D2DrlgTileDataStrc* pLinkedTileData = DRLGROOMTILE_GetLinkedTileData(pDrlgRoom, TRUE, nTileInformation.nPackedValue, nTilePosX, nTilePosY, &pLinkedRoom))
					{
						DRLGROOMTILE_LinkedTileDataManager(pMemPool, pDrlgRoom, pLinkedRoom, pLinkedTileData, TILETYPE_FLOOR, nTileInformation.nPackedValue, nTilePosX, nTilePosY);
					}
					else
					{
						DRLGROOMTILE_AddLinkedTileData(pMemPool, pDrlgRoom, TILETYPE_FLOOR, nTileInformation.nPackedValue, nTilePosX, nTilePosY);
					}
				}
				else if (nTileInformation.bIsWall)
				{
					D2DrlgRoomStrc* pLinkedRoom = nullptr;
					if (D2DrlgTileDataStrc* pLinkedTileData = DRLGROOMTILE_GetLinkedTileData(pDrlgRoom, nTileType == TILETYPE_FLOOR, nTileInformation.nPackedValue, nTilePosX, nTilePosY, &pLinkedRoom))
					{
						DRLGROOMTILE_LinkedTileDataManager(pMemPool, pDrlgRoom, pLinkedRoom, pLinkedTileData, nTileType, nTileInformation.nPackedValue, nTilePosX, nTilePosY);
					}
					else
					{
						DRLGROOMTILE_AddLinkedTileData(pMemPool, pDrlgRoom, nTileType, nTileInformation.nPackedValue, nTilePosX, nTilePosY);
					}
				}
				else if (nTileInformation.bShadow && !nTileInformation.bHidden)
				{
					DRLGROOMTILE_GetCreateLinkedTileData(pMemPool, pDrlgRoom, TILETYPE_SHADOW, nTileInformation.nPackedValue, nTilePosX, nTilePosY);
				}
				else
				{
					bTileAdded = false;
				}
			}

			if(!bTileAdded)
			{
				if (nTileInformation.bIsFloor)
				{
					D2TileLibraryEntryStrc* pTileCache = DRLGROOMTILE_GetTileCache(pDrlgRoom, TILETYPE_FLOOR, nTileInformation.nPackedValue);
					DRLGROOMTILE_InitFloorTileData(pDrlgRoom, nullptr, nTilePosX, nTilePosY, nTileInformation.nPackedValue, pTileCache);
				}
				else if(bFillBlanks && DRLGROOM_AreXYInsideCoordinates(&pDrlgRoom->pDrlgCoord, nTilePosX, nTilePosY))
				{
					D2C_PackedTileInformation nCachedTileInfo{ 0 };
					nCachedTileInfo.nTileStyle = 30;
					nCachedTileInfo.nTileSequence = uint32_t(pDrlgRoom->pLevel->nLevelId == LEVEL_ARCANESANCTUARY);
					D2TileLibraryEntryStrc* pTileCache = DRLGROOMTILE_GetTileCache(pDrlgRoom, 0, nCachedTileInfo.nPackedValue);


					D2C_PackedTileInformation nArcaneFloorTileInfo = nTileInformation;
					nArcaneFloorTileInfo.bLayerAbove = false;
					nArcaneFloorTileInfo.bHidden = true;
					DRLGROOMTILE_InitFloorTileData(pDrlgRoom, 0, nTilePosX, nTilePosY, nArcaneFloorTileInfo.nPackedValue, pTileCache);
				}

				if (nTileInformation.bIsWall)
				{
					D2TileLibraryEntryStrc* pTileCache = DRLGROOMTILE_GetTileCache(pDrlgRoom, nTileType, nTileInformation.nPackedValue);
					D2DrlgTileDataStrc* pWallTileData = DRLGROOMTILE_InitWallTileData(pDrlgRoom, nullptr, nTilePosX, nTilePosY, nTileInformation.nPackedValue, pTileCache, nTileType);
					if (nTileType == TILETYPE_WALL_RIGHT_EXIT || nTileType == TILETYPE_WALL_LEFT_EXIT)
					{
						DRLGROOMTILE_LoadWallWarpTiles(pDrlgRoom, pWallTileData, nTileInformation.nPackedValue, nTileType);
					}
				}

				if (nTileInformation.bShadow)
				{
					D2TileLibraryEntryStrc* pTileCache = DRLGROOMTILE_GetTileCache(pDrlgRoom, TILETYPE_SHADOW, nTileInformation.nPackedValue);
					DRLGROOMTILE_InitShadowTileData(pDrlgRoom, nullptr, nTilePosX, nTilePosY, nTileInformation.nPackedValue, pTileCache);
				}
			}
		}
	}
}

//D2Common.0x6FD89360
BOOL __fastcall DRLGROOMTILE_AddWarp(D2DrlgRoomStrc* pDrlgRoom, int nX, int nY, uint32_t nPackedTileInformation, int nTileType)
{
	D2C_PackedTileInformation nTileInformation{ nPackedTileInformation };
	D2LvlWarpTxt* pLvlWarpTxtRecord = DRLGWARP_GetLvlWarpTxtRecordFromWarpIdAndDirection(pDrlgRoom->pLevel, nTileInformation.nTileStyle, nTileType == TILETYPE_WALL_RIGHT_EXIT ? 'r' : 'l');
	
	if (pLvlWarpTxtRecord)
	{
		int nPosX = nX - pDrlgRoom->nTileXPos;
		int nPosY = nY - pDrlgRoom->nTileYPos;

		if (nPosX != pDrlgRoom->nTileWidth && nPosY != pDrlgRoom->nTileHeight)
		{
			DUNGEON_GameTileToSubtileCoords(&nPosX, &nPosY);

			nPosX += pLvlWarpTxtRecord->dwOffsetX;
			nPosY += pLvlWarpTxtRecord->dwOffsetY;

			DRLGROOM_AllocPresetUnit(pDrlgRoom, pDrlgRoom->pLevel->pDrlg->pMempool, UNIT_TILE, pLvlWarpTxtRecord->dwLevelId, OBJMODE_NEUTRAL, nPosX, nPosY);

			return TRUE;
		}
	}

	return FALSE;
}

// Helper functions
static D2RoomTileStrc* DRLGROOMTILE_FindDestinationWarpTile(D2DrlgRoomStrc* pDrlgRoom, D2C_PackedTileInformation nTileInformation)
{
	const int nDestinationLevelId = DRLGWARP_GetWarpDestinationFromArray(pDrlgRoom->pLevel, nTileInformation.nTileStyle);
	D2RoomTileStrc* pWarpTile = pDrlgRoom->pRoomTiles;
	while (pWarpTile && pWarpTile->pLvlWarpTxtRecord->dwLevelId != nDestinationLevelId)
	{
		pWarpTile = pWarpTile->pNext;
	}
	return pWarpTile;
}

static D2LvlWarpTxt* DRLGROOMTILE_UpdateAndGetLvlWarpTxtRecord(D2RoomTileStrc* pWarpTile, int nTileType)
{
	D2LvlWarpTxt* pLvlWarpTxtRecord = pWarpTile->pLvlWarpTxtRecord;
	D2_ASSERT(pLvlWarpTxtRecord);

	if (pLvlWarpTxtRecord->szDirection[0] != 'b')
	{
		const char szDirection = nTileType == TILETYPE_WALL_RIGHT_EXIT ? 'r' : 'l';

		if (pLvlWarpTxtRecord->szDirection[0] != szDirection)
		{
			pWarpTile->pLvlWarpTxtRecord = DATATBLS_GetLvlWarpTxtRecordFromLevelIdAndDirection(pLvlWarpTxtRecord->dwLevelId, szDirection);
		}
	}
	return pWarpTile->pLvlWarpTxtRecord;
}

//D2Common.0x6FD89410
void __fastcall DRLGROOMTILE_LoadWallWarpTiles(D2DrlgRoomStrc* pDrlgRoom, D2DrlgTileDataStrc* pTileData, uint32_t nPackedTileInformation, int nTileType)
{
	D2C_PackedTileInformation nTileInformation{ nPackedTileInformation };
	
	D2RoomTileStrc* pWarpTile = DRLGROOMTILE_FindDestinationWarpTile(pDrlgRoom, nTileInformation);

	if (!pWarpTile)
	{
		FOG_DisplayWarning("ptWarp", __FILE__, __LINE__);
		return;
	}

	if (nTileInformation.nTileSequence != 0 && nTileInformation.nTileSequence != 4 || DRLGROOMTILE_AddWarp(pDrlgRoom, pDrlgRoom->nTileXPos + pTileData->nPosX, pTileData->nPosY + pDrlgRoom->nTileYPos, nTileInformation.nPackedValue, nTileType))
	{
		D2_ASSERT(pWarpTile);

		pTileData->unk0x20 = pWarpTile->unk0x10;
		pWarpTile->unk0x10 = pTileData;

		const D2LvlWarpTxt* pWarpDef = DRLGROOMTILE_UpdateAndGetLvlWarpTxtRecord(pWarpTile, nTileType);
		if (!pWarpDef)
		{
			FOG_DisplayWarning("pctWarpDef", __FILE__, __LINE__);
		}

		if (pWarpDef->dwLitVersion)
		{
			nTileInformation.nTileSequence |= pWarpDef->dwTiles;
			D2TileLibraryEntryStrc* pTileCache = DRLGROOMTILE_GetTileCache(pDrlgRoom, nTileType, nTileInformation.nPackedValue);
			if (D2DrlgTileDataStrc* pDrlgTileData = DRLGROOMTILE_InitWallTileData(pDrlgRoom, &pWarpTile->unk0x0C, pDrlgRoom->nTileXPos + pTileData->nPosX, pTileData->nPosY + pDrlgRoom->nTileYPos, nTileInformation.nPackedValue, pTileCache, nTileType))
			{
				pDrlgTileData->dwFlags |= MAPTILE_HIDDEN;
			}
		}
	}
}

//D2Common.0x6FD89590
void __fastcall DRLGROOMTILE_LoadFloorWarpTiles(D2DrlgRoomStrc* pDrlgRoom, int nX, int nY, uint32_t nPackedTileInformation, int nTileType)
{
	static const D2CoordStrc gWarpTileOffsets_6FDD1320[] =
	{
		{ 0, 0 },
		{ 1, 0 },
		{ 0, 1 },
		{ 1, 1 }
	};

	D2C_PackedTileInformation nTileInformation{ nPackedTileInformation };
	
	pDrlgRoom->dwFlags |= DRLGROOMFLAG_POPULATION_ZERO;
	
	if (D2RoomTileStrc* pWarpTile = DRLGROOMTILE_FindDestinationWarpTile(pDrlgRoom, nTileInformation))
	{
		const D2LvlWarpTxt* pWarpDef = DRLGROOMTILE_UpdateAndGetLvlWarpTxtRecord(pWarpTile, nTileType);

		if (pWarpDef->dwLitVersion)
		{
			for (int nDestinationTileSequence = 0; nDestinationTileSequence < 4 ; ++nDestinationTileSequence)
			{
				D2C_PackedTileInformation nDestinationTileInformation{ 0 };
				nDestinationTileInformation.nTileStyle = nTileInformation.nTileSequence; // This is how we reference links between tiles to hide/show
				nDestinationTileInformation.nTileSequence = nDestinationTileSequence | 4;
				D2TileLibraryEntryStrc* pTileCache = DRLGROOMTILE_GetTileCache(pDrlgRoom, TILETYPE_FLOOR, nDestinationTileInformation.nPackedValue);


				const int nDestinationTilePosX = nX - 1 + gWarpTileOffsets_6FDD1320[nDestinationTileSequence].nX;
				const int nDestinationTilePosY = nY - 1 + gWarpTileOffsets_6FDD1320[nDestinationTileSequence].nY;
				D2DrlgTileDataStrc* pDestTileData = DRLGROOMTILE_InitFloorTileData(pDrlgRoom, &pWarpTile->unk0x0C, nDestinationTilePosX, nDestinationTilePosY, nDestinationTileInformation.nPackedValue, pTileCache);
				pDestTileData->dwFlags |= MAPTILE_HIDDEN;
			}

			for (int i = 0; i < pDrlgRoom->pTileGrid->nFloors; ++i)
			{
				D2DrlgTileDataStrc* pFloorTileData = &pDrlgRoom->pTileGrid->pTiles.pFloorTiles[i];
				if (D2CMP_10078_GetTileStyle(pFloorTileData->pTile) == nTileInformation.nTileSequence
					&& (unsigned int)D2CMP_10082_GetTileSequence(pFloorTileData->pTile) < 4)
				{
					pFloorTileData->unk0x20 = pWarpTile->unk0x10;
					pWarpTile->unk0x10 = pFloorTileData;
				}
			}
		}
	}
}

//D2Common.0x6FD897E0
D2DrlgTileDataStrc* __fastcall DRLGROOMTILE_GetLinkedTileData(D2DrlgRoomStrc* pDrlgRoom, BOOL bFloor, uint32_t nPackedTileInformation, int nX, int nY, D2DrlgRoomStrc** ppDrlgRoom)
{
	D2C_PackedTileInformation nTileInformation{ nPackedTileInformation };

	for (int i = 0; i < pDrlgRoom->nRoomsNear; ++i)
	{
		D2DrlgRoomStrc* pNearRoomEx = pDrlgRoom->ppRoomsNear[i];

		if (pNearRoomEx != pDrlgRoom && pNearRoomEx->pTileGrid && DRLGROOM_AreXYInsideCoordinatesOrOnBorder(&pNearRoomEx->pDrlgCoord, nX, nY))
		{
			for (D2DrlgTileLinkStrc* pTileLink = pNearRoomEx->pTileGrid->pMapLinks; pTileLink; pTileLink = pTileLink->pNext)
			{
				if ((!pTileLink->bFloor && !bFloor) || (pTileLink->bFloor && bFloor))
				{
					for (D2DrlgTileDataStrc* pTileData = pTileLink->pMapTile; pTileData; pTileData = pTileData->unk0x20)
					{
						if (pNearRoomEx->nTileXPos + pTileData->nPosX == nX && pNearRoomEx->nTileYPos + pTileData->nPosY == nY)
						{
							if (pTileData->nTileType != TILETYPE_WALL_TOP_CORNER_LEFT && (pTileData->nTileType == TILETYPE_SHADOW || !nTileInformation.bShadow))
							{
								if (!HasMapTileLayer(pTileData->dwFlags) || GetMapTileLayer(pTileData->dwFlags) == nTileInformation.nWallLayer)
								{
									*ppDrlgRoom = pDrlgRoom->ppRoomsNear[i];
									return pTileData;
								}
							}
						}
					}
				}
			}
		}
	}

	*ppDrlgRoom = nullptr;
	return nullptr;
}

//D2Common.0x6FD89930
void __fastcall DRLGROOMTILE_AddLinkedTileData(void* pMemPool, D2DrlgRoomStrc* pDrlgRoom, int nTileType, uint32_t nPackedTileInformation, int nX, int nY)
{
	if (nTileType != TILETYPE_WALL_LEFT_EXIT && nTileType != TILETYPE_WALL_RIGHT_EXIT || DRLGROOM_AreXYInsideCoordinates(&pDrlgRoom->pDrlgCoord, nX, nY))
	{
		D2DrlgTileLinkStrc* pCurLink;
		for (pCurLink = pDrlgRoom->pTileGrid->pMapLinks; pCurLink; pCurLink = pCurLink->pNext)
		{
			if ((pCurLink->bFloor && nTileType == TILETYPE_FLOOR) || (nTileType != TILETYPE_FLOOR))
			{
				// Use current tile
				break;
			}
		}
		if (pCurLink == nullptr)
		{
			D2DrlgTileLinkStrc* pTileLink = D2_ALLOC_STRC_POOL(pMemPool, D2DrlgTileLinkStrc);
			pTileLink->bFloor = nTileType == TILETYPE_FLOOR;
			pTileLink->pMapTile = nullptr;
			pTileLink->pNext = pDrlgRoom->pTileGrid->pMapLinks;
			pDrlgRoom->pTileGrid->pMapLinks = pTileLink;
			pCurLink = pTileLink;
		}

		D2TileLibraryEntryStrc* pTileCache = DRLGROOMTILE_GetTileCache(pDrlgRoom, nTileType, nPackedTileInformation);

		switch (nTileType)
		{
		case TILETYPE_FLOOR:
			DRLGROOMTILE_InitFloorTileData(pDrlgRoom, &pCurLink->pMapTile, nX, nY, nPackedTileInformation, pTileCache);
			break;
		case TILETYPE_SHADOW:
			DRLGROOMTILE_InitShadowTileData(pDrlgRoom, &pCurLink->pMapTile, nX, nY, nPackedTileInformation, pTileCache);
			break;
		default:
			DRLGROOMTILE_InitWallTileData(pDrlgRoom, &pCurLink->pMapTile, nX, nY, nPackedTileInformation, pTileCache, nTileType);
			if (nTileType == TILETYPE_WALL_LEFT_EXIT || nTileType == TILETYPE_WALL_RIGHT_EXIT)
			{
				DRLGROOMTILE_LoadWallWarpTiles(pDrlgRoom, pCurLink->pMapTile, nPackedTileInformation, nTileType);
			}
			break;
		}
	}
}

//D2Common.0x6FD89AF0
void __fastcall DRLGROOMTILE_LinkedTileDataManager(void* pMemPool, D2DrlgRoomStrc* pDrlgRoom1, D2DrlgRoomStrc* pDrlgRoom2, D2DrlgTileDataStrc* pTileData, int nTileType, uint32_t nPackedTileInformation, int nX, int nY)
{
	//D2Common.0x6FDD1340
	// -1 Means stop and do nothing
	// -2 Means ignore remapping but still init tile data
	static const int nRemapIndices[] =
	{
		-1, 
		0, // [TILETYPE_WALL_LEFT            ]
		1, // [TILETYPE_WALL_RIGHT           ]
		2, // [TILETYPE_WALL_TOP_CORNER_RIGHT]
		-1,// [TILETYPE_WALL_TOP_CORNER_LEFT ] This one doesn't remap somehow
		3, // [TILETYPE_WALL_TOP_RIGHT       ]
		4, // [TILETYPE_WALL_BOTTOM_LEFT     ]
		5, // [TILETYPE_WALL_BOTTOM_RIGHT    ]
		-2, -2, -1, -1, -1, -2, -1, -1, -1, -1, -1
	};
	//D2Common.0x6FDD1390
	static const int nWallTileTypeRemap[6][7] =
	{
		/* [parameter nTileType value     ]    [pTileData->nTileType - 1] */
		/* [TILETYPE_WALL_LEFT            ] */ { TILETYPE_WALL_LEFT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_LEFT,TILETYPE_WALL_LEFT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_LEFT},
		/* [TILETYPE_WALL_RIGHT           ] */ { TILETYPE_WALL_LEFT,TILETYPE_WALL_RIGHT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_LEFT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_RIGHT,TILETYPE_WALL_RIGHT},
		/* [TILETYPE_WALL_TOP_CORNER_RIGHT] */ { TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_LEFT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_RIGHT},
		/* [TILETYPE_WALL_TOP_RIGHT       ] */ { TILETYPE_WALL_LEFT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_LEFT,TILETYPE_WALL_TOP_RIGHT,TILETYPE_WALL_BOTTOM_LEFT,TILETYPE_WALL_LEFT},
		/* [TILETYPE_WALL_BOTTOM_LEFT     ] */ { TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_RIGHT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_LEFT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_BOTTOM_LEFT,TILETYPE_WALL_RIGHT},
		/* [TILETYPE_WALL_BOTTOM_RIGHT    ] */ { TILETYPE_WALL_LEFT,TILETYPE_WALL_RIGHT,TILETYPE_WALL_TOP_CORNER_RIGHT,TILETYPE_WALL_TOP_CORNER_LEFT,TILETYPE_WALL_LEFT,TILETYPE_WALL_RIGHT,TILETYPE_WALL_BOTTOM_RIGHT	},
	};
	
	D2C_PackedTileInformation nTileInformation{ nPackedTileInformation };
	int v10 = nRemapIndices[nTileType];

	if (pTileData->dwFlags & MAPTILE_UNK_0x1)
	{
		if (TileTypeIsAWallWithDoor(pTileData->nTileType))
		{
			DRLGROOMTILE_InitializeTileDataFlags(pDrlgRoom1, pTileData, nPackedTileInformation, pTileData->nTileType, nX, nY);
		}

		return;
	}

	if (!nTileInformation.bLayerAbove)
	{
		BOOL b = FALSE;
		if (!TileTypeIsAWallWithDoor(nTileType))
		{
			if (TileTypeIsAWallWithDoor(pTileData->nTileType) && (nX == pDrlgRoom2->nTileXPos || nY == pDrlgRoom2->nTileYPos))
			{
				return;
			}

			b = TRUE;
		}

		if (b || nX != pDrlgRoom1->nTileXPos && nY != pDrlgRoom1->nTileYPos)
		{
			bool bIsTileTypeANormalWallOrFloor = pTileData->nTileType <= TILETYPE_WALL_BOTTOM_RIGHT;
			if (v10 < 0 || !bIsTileTypeANormalWallOrFloor)
			{
				if (v10 != -1)
				{
					return;
				}
			}
			else
			{
				D2_ASSERT(v10 < 6);
				D2_ASSERT(pTileData->nTileType >= 0);
				const int nWallIdx = pTileData->nTileType - TILETYPE_WALL_LEFT; // From 0 to 6
				nTileType = nWallTileTypeRemap[v10][nWallIdx];
			}
		}
	}

	if (pTileData->nTileType == TILETYPE_WALL_TOP_CORNER_RIGHT)
	{
		if (nTileType != TILETYPE_WALL_TOP_CORNER_RIGHT)
		{
			pTileData->unk0x20->dwFlags |= MAPTILE_HIDDEN;
			D2Common_COLLISION_FirstFn_6FD41000(pDrlgRoom2->pRoom, pTileData->unk0x20, nullptr);
		}
	}
	else
	{
		if (nTileType == TILETYPE_WALL_TOP_CORNER_RIGHT)
		{
			pTileData->dwFlags |= (3 << MAPTILE_WALL_LAYER_BIT) | MAPTILE_HIDDEN;
			D2Common_COLLISION_FirstFn_6FD41000(pDrlgRoom2->pRoom, pTileData, nullptr);
			DRLGROOMTILE_AddLinkedTileData(pMemPool, pDrlgRoom1, TILETYPE_WALL_TOP_CORNER_RIGHT, nPackedTileInformation, nX, nY);
		}
	}

	if (nTileType != pTileData->nTileType || 
		pTileData->nTileType == TILETYPE_FLOOR && D2CMP_10078_GetTileStyle(pTileData->pTile) == 30 && D2CMP_10082_GetTileSequence(pTileData->pTile) == 0)
	{
		D2TileLibraryEntryStrc* pTileCache = DRLGROOMTILE_GetTileCache(pDrlgRoom2, nTileType, nPackedTileInformation);
		pTileData->nTileType = nTileType;

		if (pTileCache != pTileData->pTile && pDrlgRoom2->pRoom)
		{
			D2Common_COLLISION_FirstFn_6FD41000(pDrlgRoom2->pRoom, pTileData, pTileCache);
		}

		pTileData->pTile = pTileCache;
	}

	DRLGROOMTILE_InitializeTileDataFlags(pDrlgRoom1, pTileData, nPackedTileInformation, pTileData->nTileType, nX, nY);
}

//D2Common.0x6FD89CC0
void __fastcall DRLGROOMTILE_GetCreateLinkedTileData(void* pMemPool, D2DrlgRoomStrc* pDrlgRoom, int nTileType, uint32_t nPackedTileInformation, int nX, int nY)
{
	D2C_PackedTileInformation nTileInformation{ nPackedTileInformation };

	for (int i = 0; i < pDrlgRoom->nRoomsNear; ++i)
	{
		D2DrlgRoomStrc* pNearRoomEx = pDrlgRoom->ppRoomsNear[i];
		if (pNearRoomEx != pDrlgRoom && pNearRoomEx->pTileGrid && DRLGROOM_AreXYInsideCoordinatesOrOnBorder(&pNearRoomEx->pDrlgCoord, nX, nY))
		{
			D2DrlgTileLinkStrc* pTileLink = pNearRoomEx->pTileGrid->pMapLinks;
			while (pTileLink)
			{
				D2DrlgTileDataStrc* pTileData = pTileLink->pMapTile;

				if ((!pTileLink->bFloor && nTileType != TILETYPE_FLOOR) || (pTileLink->bFloor && nTileType == TILETYPE_FLOOR))
				{
					while (pTileData)
					{
						if (pNearRoomEx->nTileXPos + pTileData->nPosX == nX && pNearRoomEx->nTileYPos + pTileData->nPosY == nY)
						{
							if (pTileData->nTileType != TILETYPE_WALL_TOP_CORNER_LEFT && (pTileData->nTileType == TILETYPE_SHADOW || !nTileInformation.bShadow))
							{
								if (!HasMapTileLayer(pTileData->dwFlags) || GetMapTileLayer(pTileData->dwFlags) == nTileInformation.nWallLayer)
								{
									return DRLGROOMTILE_LinkedTileDataManager(pMemPool, pDrlgRoom, pDrlgRoom->ppRoomsNear[i], pTileData, nTileType, nPackedTileInformation, nX, nY);
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

	DRLGROOMTILE_AddLinkedTileData(pMemPool, pDrlgRoom, nTileType, nPackedTileInformation, nX, nY);
}

//D2Common.0x6FD89E30
void __fastcall DRLGROOMTILE_CountAllTileTypes(D2DrlgRoomStrc* pDrlgRoom, D2DrlgGridStrc* pTileInfoGrid, BOOL bCheckCoordinatesValidity, BOOL bKillEdgeX, BOOL bKillEdgeY)
{
	const int nTileCountX = pDrlgRoom->nTileWidth + (bKillEdgeX == 0);
	const int nTileCountY = pDrlgRoom->nTileHeight + (bKillEdgeY == 0);

	for (int nY = 0; nY < nTileCountY; ++nY)
	{
		for (int nX = 0; nX < nTileCountX; ++nX)
		{
			const int* pFlags = DRLGGRID_GetGridFlagsPointer(pTileInfoGrid, nX, nY);
			D2C_PackedTileInformation nTileInformation{ (uint32_t)*pFlags };
			if (nTileInformation.bIsWall)
			{
				++pDrlgRoom->pTileGrid->pTiles.nWalls;
			}

			if (nTileInformation.bIsFloor || bCheckCoordinatesValidity && DRLGROOM_AreXYInsideCoordinates(&pDrlgRoom->pDrlgCoord, nX + pDrlgRoom->nTileXPos, nY + pDrlgRoom->nTileYPos))
			{
				++pDrlgRoom->pTileGrid->pTiles.nFloors;
			}

			if (nTileInformation.bShadow)
			{
				++pDrlgRoom->pTileGrid->pTiles.nRoofs;
			}
		}
	}
}

//D2Common.0x6FD89F00
void __fastcall DRLGROOMTILE_CountWallWarpTiles(D2DrlgRoomStrc* pDrlgRoom, D2DrlgGridStrc* pTileInfoGrid, D2DrlgGridStrc* pTileTypeGrid, BOOL bKillEdgeX, BOOL bKillEdgeY)
{
	const int nTileCountX = pDrlgRoom->nTileWidth + (bKillEdgeX == 0);
	const int nTileCountY = pDrlgRoom->nTileHeight + (bKillEdgeY == 0);

	for (int nY = 0; nY < nTileCountY; ++nY)
	{
		for (int nX = 0; nX < nTileCountX; ++nX)
		{
			switch (int nTileType = DRLGGRID_GetGridEntry(pTileTypeGrid, nX, nY))
			{
			case TILETYPE_WALL_TOP_CORNER_RIGHT:
				++pDrlgRoom->pTileGrid->pTiles.nWalls;
				break;
			case TILETYPE_WALL_LEFT_EXIT:
			case TILETYPE_WALL_RIGHT_EXIT:
				if (D2C_PackedTileInformation{ (uint32_t)DRLGGRID_GetGridEntry(pTileInfoGrid, nX, nY) }.bHidden)
				{
					pDrlgRoom->pTileGrid->pTiles.nFloors += 6;
				}
				else
				{
					++pDrlgRoom->pTileGrid->pTiles.nWalls;
				}
				break;
			default:
				break;
			}
		}
	}
}

//D2Common.0x6FD89FA0
void __fastcall DRLGROOMTILE_InitRoomGrids(D2DrlgRoomStrc* pDrlgRoom)
{
	SEED_InitLowSeed(&pDrlgRoom->pSeed, pDrlgRoom->dwInitSeed);

	if (pDrlgRoom->nType == DRLGTYPE_MAZE)
	{
		DRLGOUTPLACE_InitOutdoorRoomGrids(pDrlgRoom);
	}
	else if (pDrlgRoom->nType == DRLGTYPE_PRESET)
	{
		DRLGPRESET_InitPresetRoomGrids(pDrlgRoom);
	}
}

//D2Common.0x6FD89FD0
void __fastcall DRLGROOMTILE_AddRoomMapTiles(D2DrlgRoomStrc* pDrlgRoom)
{
	if (pDrlgRoom->nType == DRLGTYPE_MAZE)
	{
		DRLGOUTROOM_InitializeDrlgOutdoorRoom(pDrlgRoom);
	}
	else if (pDrlgRoom->nType == DRLGTYPE_PRESET)
	{
		DRLGPRESET_AddPresetRoomMapTiles(pDrlgRoom);
	}

	pDrlgRoom->dwFlags |= DRLGROOMFLAG_HAS_ROOM;
}

//D2Common.0x6FD8A010
void __fastcall DRLGROOMTILE_AllocTileGrid(D2DrlgRoomStrc* pDrlgRoom)
{
	if (!pDrlgRoom->pTileGrid)
	{
		pDrlgRoom->pTileGrid = D2_CALLOC_STRC_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, D2DrlgTileGridStrc);
	}
}

//D2Common.0x6FD8A050
void __fastcall DRLGROOMTILE_AllocTileData(D2DrlgRoomStrc* pDrlgRoom)
{
	void* pMemPool = pDrlgRoom->pLevel->pDrlg->pMempool;
	D2DrlgRoomTilesStrc* pRoomTiles = &pDrlgRoom->pTileGrid->pTiles;

	if (pRoomTiles->nFloors)
	{
		pRoomTiles->pFloorTiles = (D2DrlgTileDataStrc*)D2_CALLOC_POOL(pMemPool, sizeof(D2DrlgTileDataStrc) * pRoomTiles->nFloors);
	}

	if (pRoomTiles->nWalls)
	{
		pRoomTiles->pWallTiles = (D2DrlgTileDataStrc*)D2_CALLOC_POOL(pMemPool, sizeof(D2DrlgTileDataStrc) * pRoomTiles->nWalls);
	}

	// Something may already have allocated roof tiles with DRLGROOMTILE_ReallocRoofTileGrid!
	if (pRoomTiles->nRoofs && !pRoomTiles->pRoofTiles)
	{
		pRoomTiles->pRoofTiles = (D2DrlgTileDataStrc*)D2_CALLOC_POOL(pMemPool, sizeof(D2DrlgTileDataStrc) * pRoomTiles->nRoofs);
	}
}

//D2Common.0x6FD8A130
void __fastcall DRLGROOMTILE_ReallocRoofTileGrid(void* pMemPool, D2DrlgTileGridStrc* pTileGrid, int nAdditionalRoofs)
{
	if (nAdditionalRoofs)
	{
		pTileGrid->pTiles.pRoofTiles = (D2DrlgTileDataStrc*)D2_REALLOC_POOL(pMemPool, pTileGrid->pTiles.pRoofTiles, sizeof(D2DrlgTileDataStrc) * (nAdditionalRoofs + pTileGrid->pTiles.nRoofs));
		
		int nCounter = 0;
		while (nCounter < pTileGrid->nShadows - 1)
		{
			pTileGrid->pTiles.pRoofTiles[nCounter].unk0x20 = &pTileGrid->pTiles.pRoofTiles[nCounter + 1];
			++nCounter;
		}

		pTileGrid->pTiles.pRoofTiles[nCounter].unk0x20 = nullptr;
		pTileGrid->pTiles.nRoofs += nAdditionalRoofs;
	}
}

//D2Common.0x6FD8A1B0 (#10017)
int __fastcall DRLGROOMTILE_GetNumberOfShadowsFromRoom(D2ActiveRoomStrc* pRoom)
{
	if (pRoom)
	{
		return DUNGEON_GetRoomExFromRoom(pRoom)->pTileGrid->nShadows;
	}

	return 0;
}

//D2Common.0x6FD8A1D0
void __fastcall DRLGROOMTILE_FreeTileGrid(D2DrlgRoomStrc* pDrlgRoom)
{
	if (D2DrlgTileGridStrc* pTileGrid = pDrlgRoom->pTileGrid)
	{
		if (pTileGrid->pTiles.pWallTiles)
		{
			D2_FREE_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, pTileGrid->pTiles.pWallTiles);
		}

		if (pTileGrid->pTiles.pFloorTiles)
		{
			D2_FREE_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, pTileGrid->pTiles.pFloorTiles);
		}

		if (pTileGrid->pTiles.pRoofTiles)
		{
			D2_FREE_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, pTileGrid->pTiles.pRoofTiles);
		}

		D2DrlgTileLinkStrc* pTileLink = pTileGrid->pMapLinks;
		while (pTileLink)
		{
			D2DrlgTileLinkStrc* pNextTileLink = pTileLink->pNext;
			D2_FREE_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, pTileLink);
			pTileLink = pNextTileLink;
		}
		pTileGrid->pMapLinks = nullptr;

		D2DrlgAnimTileGridStrc* pAnimTileGrid = pTileGrid->pAnimTiles;
		while (pAnimTileGrid)
		{
			D2DrlgAnimTileGridStrc* pNextAnimTileGrid = pAnimTileGrid->pNext;
			D2_FREE_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, pAnimTileGrid->ppMapTileData);
			D2_FREE_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, pAnimTileGrid);
			pAnimTileGrid = pNextAnimTileGrid;
		}

		D2_FREE_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, pTileGrid);
		pDrlgRoom->pTileGrid = nullptr;
	}
}

//D2Common.0x6FD8A2E0
void __fastcall DRLGROOMTILE_FreeRoom(D2DrlgRoomStrc* pDrlgRoom, BOOL bKeepRoom)
{
	if (!bKeepRoom && pDrlgRoom->pRoom)
	{
		DUNGEON_RemoveRoomFromAct(pDrlgRoom->pLevel->pDrlg->pAct, pDrlgRoom->pRoom);
	}

	pDrlgRoom->pRoom = nullptr;

	DRLGLOGIC_FreeDrlgCoordList(pDrlgRoom);

	if (pDrlgRoom->dwFlags & DRLGROOMFLAG_HAS_ROOM)
	{
		pDrlgRoom->dwFlags ^= DRLGROOMFLAG_HAS_ROOM;
		++pDrlgRoom->pLevel->pDrlg->nFreedRooms;
		DRLGROOMTILE_FreeTileGrid(pDrlgRoom);

		if (pDrlgRoom->nType == DRLGTYPE_MAZE)
		{
			DRLGOUTROOM_FreeDrlgOutdoorRoomData(pDrlgRoom);
		}
		else if (pDrlgRoom->nType == DRLGTYPE_PRESET)
		{
			DRLGPRESET_FreeDrlgGridsFromPresetRoom(pDrlgRoom);
		}

		for (D2RoomTileStrc* i = pDrlgRoom->pRoomTiles; i; i = i->pNext)
		{
			i->unk0x0C = nullptr;
			i->unk0x10 = nullptr;
		}

		if (bKeepRoom)
		{
			pDrlgRoom->dwFlags |= DRLGROOMFLAG_ROOM_FREED_SRV;
		}
	}
}

//D2Common.0x6FD8A380
void __fastcall DRLGROOMTILE_LoadDT1FilesForRoom(D2DrlgRoomStrc* pDrlgRoom)
{
	D2LvlTypesTxt* pLvlTypesTxtRecord = DATATBLS_GetLevelTypesTxtRecord(pDrlgRoom->pLevel->nLevelType);

	static_assert(ARRAY_SIZE(pLvlTypesTxtRecord->szFile) <= 32, "DT1Mask is 32bits, needs to match the number of file records");
	
	uint32_t dwDT1Mask = pDrlgRoom->dwDT1Mask;
	for (int nFileIndex = 0; nFileIndex < ARRAY_SIZE(pLvlTypesTxtRecord->szFile) && dwDT1Mask != 0; ++nFileIndex, dwDT1Mask >>= 1)
	{
		if (dwDT1Mask & 1)
		{
			D2CMP_10087_LoadTileLibrarySlot(pDrlgRoom->pTiles, pLvlTypesTxtRecord->szFile[nFileIndex]);
		}
	}

	char szPath[MAX_PATH] = { sgptDataTables->szDefaultString };

	wsprintfA(szPath, "%s\\Tiles\\Act1\\Outdoors\\Blank.dt1", "DATA\\GLOBAL");
	D2CMP_10087_LoadTileLibrarySlot(pDrlgRoom->pTiles, szPath);

	wsprintfA(szPath, "%s\\Tiles\\Act1\\Barracks\\InvisWal.dt1", "DATA\\GLOBAL");
	D2CMP_10087_LoadTileLibrarySlot(pDrlgRoom->pTiles, szPath);

	wsprintfA(szPath, "%s\\Tiles\\Act1\\Barracks\\Warp.dt1", "DATA\\GLOBAL");
	D2CMP_10087_LoadTileLibrarySlot(pDrlgRoom->pTiles, szPath);

	pDrlgRoom->dwFlags |= DRLGROOMFLAG_TILELIB_LOADED;
}
