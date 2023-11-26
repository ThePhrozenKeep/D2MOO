#include "Drlg/D2DrlgDrlg.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlgAnim.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "Drlg/D2DrlgDrlgWarp.h"
#include "Drlg/D2DrlgActivate.h"
#include "Drlg/D2DrlgMaze.h"
#include "Drlg/D2DrlgOutdoors.h"
#include "Drlg/D2DrlgOutPlace.h"
#include "Drlg/D2DrlgPreset.h"
#include "Drlg/D2DrlgRoomTile.h"
#include "D2Dungeon.h"
#include "D2Seed.h"
#include <D2CMP.h>
#include <DataTbls/LevelsIds.h>

static_assert(DRLGROOMFLAG_HAS_WARP_0 == (1 << DRLGROOMFLAG_HAS_WARP_FIRST_BIT), "Warp first bit must match of DRLGROOMFLAG_HAS_WARP_0");
static_assert(DRLGROOMFLAG_SUBSHRINE_ROW1 == (1 << DRLGROOMFLAG_SUBSHRINE_ROWS_FIRST_BIT), "Subshrines first bit must match of DRLGROOMFLAG_SUBSHRINE_ROW1");
static_assert(DRLGROOMFLAG_HAS_WAYPOINT == (1 << DRLGROOMFLAG_HAS_WAYPOINT_FIRST_BIT), "Waypoint first bit must match of DRLGROOMFLAG_HAS_WAYPOINT");

//D2Common.0x6FD74120 (#10014)
D2DrlgStrc* __fastcall DRLG_AllocDrlg(D2DrlgActStrc* pAct, uint8_t nActNo, HD2ARCHIVE hArchive, uint32_t nInitSeed, int nTownLevelId, uint32_t nFlags, D2GameStrc* pGame, uint8_t nDifficulty, AUTOMAPFN pfAutoMap, TOWNAUTOMAPFN pfTownAutoMap)
{
	D2DrlgStrc* pDrlg = D2_CALLOC_STRC_POOL(pAct->pMemPool, D2DrlgStrc);

	pDrlg->pAct = pAct;
	pDrlg->pMempool = pAct->pMemPool;
	D2_ASSERT(hArchive == nullptr);
	pDrlg->hArchive = hArchive; // Always nullptr in the game
	pDrlg->nAct = nActNo;

	SEED_InitLowSeed(&pDrlg->pSeed, nInitSeed);

	pDrlg->dwStartSeed = (unsigned int)SEED_RollRandomNumber(&pDrlg->pSeed);
	pDrlg->dwFlags = nFlags;
	pDrlg->pGame = pGame;
	pDrlg->dwGameLowSeed = nInitSeed;
	pDrlg->nDifficulty = nDifficulty;
	pDrlg->pfAutomap = pfAutoMap;
	pDrlg->pfTownAutomap = pfTownAutoMap;

	char szPath[MAX_PATH] = {};
	szPath[0] = sgptDataTables->szDefaultString;

	switch (nActNo)
	{
	case ACT_I:
		wsprintfA(szPath, "%s\\Tiles\\Act1\\Town\\Floor.dt1", "DATA\\GLOBAL");
		D2CMP_10087_LoadTileLibrarySlot(pDrlg->pTiles, szPath);
		break;
	
	case ACT_II:
	{
		unsigned int nStaffLevelOffset = 0;
		unsigned int nBossLevelOffset = 0;
		do
		{
			nStaffLevelOffset = (unsigned int)SEED_RollRandomNumber(&pDrlg->pSeed) % 7;
			nBossLevelOffset = (unsigned int)SEED_RollRandomNumber(&pDrlg->pSeed) % 7;
		} while (nStaffLevelOffset == nBossLevelOffset);

		pDrlg->nStaffTombLevel = LEVEL_TALRASHASTOMB1 + nStaffLevelOffset;
		pDrlg->nBossTombLevel = LEVEL_TALRASHASTOMB1 + nBossLevelOffset;

		wsprintfA(szPath, "%s\\Tiles\\Act2\\Town\\Ground.dt1", "DATA\\GLOBAL");
		D2CMP_10087_LoadTileLibrarySlot(pDrlg->pTiles, szPath);
		break;
	}
	case ACT_III:
		pDrlg->bJungleInterlink = SEED_RollRandomNumber(&pDrlg->pSeed) & 1;

		wsprintfA(szPath, "%s\\Tiles\\ACT3\\Kurast\\sets.dt1", "DATA\\GLOBAL");
		D2CMP_10087_LoadTileLibrarySlot(pDrlg->pTiles, szPath);
		break;
	}

	DRLGACTIVATE_InitializeRoomExStatusLists(pDrlg);
	DRLGOUTPLACE_CreateLevelConnections(pDrlg, nActNo);

	if (nTownLevelId != LEVEL_NONE)
	{
		DRLG_InitLevel(DRLG_GetLevel(pDrlg, nTownLevelId));
	}

	return pDrlg;
}

//D2Common.0x6FD743B0 (#10012)
void __fastcall DRLG_FreeDrlg(D2DrlgStrc* pDrlg)
{
	D2DrlgLevelStrc* pNextLevel = NULL;
	D2DrlgLevelStrc* pLevel = NULL;
	D2DrlgWarpStrc* pNextWarp = NULL;
	D2DrlgWarpStrc* pWarp = NULL;

	if (pDrlg->pLevel)
	{
		pLevel = pDrlg->pLevel;

		do
		{
			pNextLevel = pLevel->pNextLevel;
			DRLG_FreeLevel(pDrlg->pMempool, pLevel, FALSE);
			D2_FREE_POOL(pDrlg->pMempool, pLevel);
			pLevel = pNextLevel;
		}
		while (pLevel);

		pDrlg->pLevel = NULL;
	}

	if (pDrlg->pWarp)
	{
		pWarp = pDrlg->pWarp;

		do
		{
			pNextWarp = pWarp->pNext;
			D2_FREE_POOL(pDrlg->pMempool, pWarp);
			pWarp = pNextWarp;
		}
		while (pWarp);

		pDrlg->pWarp = NULL;
	}

	D2_FREE_POOL(pDrlg->pMempool, pDrlg);
}

//D2Common.0x6FD74440
void __fastcall DRLG_FreeLevel(void* pMemPool, D2DrlgLevelStrc* pLevel, BOOL bAlloc)
{
	D2DrlgBuildStrc* pNextDrlgBuild = NULL;
	D2DrlgBuildStrc* pDrlgBuild = NULL;
	D2DrlgRoomStrc* pNextRoomEx = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	int nCounter = 0;

	if (bAlloc)
	{
		if (!pLevel->pPresetMaps && pLevel->nRooms)
		{
			pLevel->pPresetMaps = (int*)D2_CALLOC_POOL(pMemPool, sizeof(int) * pLevel->nRooms);
		}
	}
	else
	{
		if (pLevel->pPresetMaps)
		{
			D2_FREE_POOL(pMemPool, pLevel->pPresetMaps);
			pLevel->pPresetMaps = NULL;
		}
	}

	pNextRoomEx = pLevel->pFirstRoomEx;
	if (pNextRoomEx)
	{
		nCounter = 0;
		do
		{
			pDrlgRoom = pNextRoomEx;
			pNextRoomEx = pNextRoomEx->pDrlgRoomNext;
			if (pLevel->pPresetMaps)
			{
				pLevel->pPresetMaps[nCounter] = pDrlgRoom->dwOtherFlags & 1;
				++nCounter;
			}

			DRLGROOM_FreeRoomEx(pDrlgRoom);
		}
		while (pNextRoomEx);

		pLevel->pFirstRoomEx = NULL;
		pLevel->nRooms = 0;
	}

	if (pLevel->pCurrentMap)
	{
		DRLGPRESET_FreeDrlgMap(pMemPool, pLevel->pCurrentMap);
		pLevel->pCurrentMap = NULL;
	}

	switch (pLevel->nDrlgType)
	{
	case DRLGTYPE_MAZE:
		DRLGMAZE_ResetMazeRecord(pLevel, bAlloc);
		break;

	case DRLGTYPE_PRESET:
		DRLGPRESET_ResetDrlgMap(pLevel, bAlloc);
		break;

	case DRLGTYPE_OUTDOOR:
		DRLGOUTDOORS_FreeOutdoorInfo(pLevel, bAlloc);
		break;
	}

	memset(pLevel->pTileInfo, 0x00, sizeof(pLevel->pTileInfo));
	pLevel->nTileInfo = 0;

	memset(pLevel->nRoom_Center_Warp_X, 0x00, sizeof(pLevel->nRoom_Center_Warp_X) + sizeof(pLevel->nRoom_Center_Warp_Y));
	pLevel->nRoomCoords = 0;

	if (!bAlloc && pLevel->pJungleDefs)
	{
		D2_FREE_POOL(pMemPool, pLevel->pJungleDefs);
		pLevel->pJungleDefs = NULL;
	}

	pNextDrlgBuild = pLevel->pBuild;
	if (pNextDrlgBuild)
	{
		do
		{
			pDrlgBuild = pNextDrlgBuild;
			pNextDrlgBuild = pNextDrlgBuild->pNext;
			D2_FREE_POOL(pMemPool, pDrlgBuild);
		}
		while (pNextDrlgBuild);

		pLevel->pBuild = NULL;
	}
}

//D2Common.0x6FD745C0
//TODO: Name
void __fastcall sub_6FD745C0(D2DrlgRoomStrc* pDrlgRoom1, D2DrlgRoomStrc* pDrlgRoom2)
{
	D2DrlgLevelStrc* pLevel1 = NULL;
	D2DrlgLevelStrc* pLevel2 = NULL;
	D2DrlgLevelStrc* pLevel = NULL;
	int* pVisLevelIds = NULL;
	int nVisLevelId = 0;

	if (pDrlgRoom1)
	{
		pLevel1 = pDrlgRoom1->pLevel;
	}

	if (pDrlgRoom2)
	{
		pLevel2 = pDrlgRoom2->pLevel;
	}

	if (pLevel1 != pLevel2)
	{
		if (pLevel1)
		{
			--pLevel1->bActive;
			if (pLevel1->bActive <= 0)
			{
				pLevel1->bActive = 0;
			}
			pLevel1->dwInactiveFrames = 10;

			pVisLevelIds = DRLGROOM_GetVisArrayFromLevelId(pLevel1->pDrlg, pLevel1->nLevelId);

			for (int i = 0; i < 8; ++i)
			{
				nVisLevelId = pVisLevelIds[i];
				if (nVisLevelId)
				{
					pLevel = DRLG_GetLevel(pLevel1->pDrlg, nVisLevelId);

					pLevel->dwInactiveFrames = 10;
					--pLevel->bActive;
					if (pLevel->bActive <= 0)
					{
						pLevel->bActive = 0;
					}
				}
			}
		}

		if (pLevel2)
		{
			++pLevel2->bActive;
			if (pLevel2->bActive <= 0)
			{
				pLevel2->bActive = 0;
			}
			pLevel2->dwInactiveFrames = 10;

			pVisLevelIds = DRLGROOM_GetVisArrayFromLevelId(pLevel2->pDrlg, pLevel2->nLevelId);

			for (int i = 0; i < 8; ++i)
			{
				nVisLevelId = pVisLevelIds[i];
				if (nVisLevelId)
				{
					pLevel = DRLG_GetLevel(pLevel2->pDrlg, nVisLevelId);

					pLevel->dwInactiveFrames = 10;
					++pLevel->bActive;
					if (pLevel->bActive <= 0)
					{
						pLevel->bActive = 0;
					}
				}
			}
		}
	}
}

//D2Common.0x6FD74700
//TODO: Clean loops
void __fastcall DRLG_UpdateAndFreeInactiveRooms(D2DrlgStrc* pDrlg)
{
	for (D2DrlgLevelStrc* pLevel = pDrlg->pLevel; pLevel; pLevel = pLevel->pNextLevel)
	{
		if (!pLevel->bActive && pLevel->pFirstRoomEx)
		{
			if (pLevel->dwInactiveFrames)
			{
				--pLevel->dwInactiveFrames;
			}
			else
			{
				D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx;

				while (pDrlgRoom && pDrlgRoom->fRoomStatus > 3 && !(pDrlgRoom->dwFlags & DRLGROOMFLAG_HAS_ROOM))
				{
					pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
				}

				if (pDrlgRoom)
				{
					pLevel->dwInactiveFrames = 10;
				}
				else
				{
					int* pLevelIds = DRLGROOM_GetVisArrayFromLevelId(pLevel->pDrlg, pLevel->nLevelId);

					D2DrlgRoomStrc* k = nullptr;
					for (int j = 0; j < 8; ++j)
					{
						if (pLevelIds[j])
						{
							D2DrlgLevelStrc* pCurrentLevel = DRLG_GetLevel(pLevel->pDrlg, pLevelIds[j]);

							if (pCurrentLevel->pFirstRoomEx)
							{
								int* pLevels = DRLGROOM_GetVisArrayFromLevelId(pCurrentLevel->pDrlg, pCurrentLevel->nLevelId);
								int dwFlags = 0;

								for (int i = 0; i < 8; ++i)
								{
									if (pLevels[i] == pLevel->nLevelId)
									{
										dwFlags |= (1 << (i + 4));
									}
								}

								for (k = pCurrentLevel->pFirstRoomEx; k; k = k->pDrlgRoomNext)
								{
									if (k->dwFlags & dwFlags && (k->fRoomStatus <= 3 || k->dwFlags & DRLGROOMFLAG_HAS_ROOM))
									{
										break;
									}
								}

								if (k)
								{
									pLevel->dwInactiveFrames = 10;
									break;
								}
								else
								{
									for (D2DrlgRoomStrc* i = pCurrentLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
									{
										if (i->dwFlags & dwFlags)
										{
											DRLGROOM_FreeRoomTiles(pLevel->pDrlg->pMempool, i);

											if (i->ppRoomsNear)
											{
												D2_FREE_POOL(pLevel->pDrlg->pMempool, i->ppRoomsNear);
												i->ppRoomsNear = NULL;
												i->nRoomsNear = 0;
											}
										}
									}
								}
							}
						}
					}

					if (!k)
					{
						DRLG_FreeLevel(pDrlg->pMempool, pLevel, TRUE);
					}
				}
			}
		}
	}
}

//D2Common.0x6FD748D0 (#10013)
D2DrlgLevelStrc* __fastcall DRLG_AllocLevel(D2DrlgStrc* pDrlg, int nLevelId)
{
	D2DrlgLevelStrc* pLevel = D2_CALLOC_STRC_POOL(pDrlg->pMempool, D2DrlgLevelStrc);

	pLevel->pDrlg = pDrlg;
	pLevel->nLevelId = nLevelId;
	pLevel->nLevelType = DATATBLS_GetLevelDefRecord(nLevelId)->dwLevelType;
	pLevel->nDrlgType = DATATBLS_GetLevelDefRecord(nLevelId)->dwDrlgType;

	if (pDrlg->dwFlags & DRLGFLAG_ONCLIENT)
	{
		pLevel->dwFlags |= DRLGLEVELFLAG_AUTOMAP_REVEAL;
	}

	SEED_InitLowSeed(&pLevel->pSeed, pLevel->nLevelId + pLevel->pDrlg->dwStartSeed);

	switch (pLevel->nDrlgType)
	{
	case DRLGTYPE_MAZE:
		DRLGMAZE_InitLevelData(pLevel);
		break;
	
	case DRLGTYPE_PRESET:
		DRLGPRESET_InitLevelData(pLevel);
		break;
		
	case DRLGTYPE_OUTDOOR:
		DRLGOUTDOORS_AllocOutdoorInfo(pLevel);
		break;
	}

	pLevel->pNextLevel = pDrlg->pLevel;
	pDrlg->pLevel = pLevel;

	return pLevel;
}

//D2Common.0x6FD749A0 (#10005)
D2DrlgLevelStrc* __stdcall DRLG_GetLevel(D2DrlgStrc* pDrlg, int nLevelId)
{
	for (D2DrlgLevelStrc* pLevel = pDrlg->pLevel; pLevel; pLevel = pLevel->pNextLevel)
	{
		if (pLevel->nLevelId == nLevelId)
		{
			return pLevel;
		}
	}

	return DRLG_AllocLevel(pDrlg, nLevelId);
}

//D2Common.0x6FD749D0
int __fastcall DRLG_GetHoradricStaffTombLevelId(D2DrlgStrc* pDrlg)
{
	if (pDrlg)
	{
		return pDrlg->nStaffTombLevel;
	}

	return 0;
}

//D2Common.0x6FD749E0
int __fastcall DRLG_GetDirectionFromCoordinates(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2)
{
	if (pDrlgCoord1->nPosX <= pDrlgCoord2->nPosX)
	{
		if (pDrlgCoord2->nPosX == pDrlgCoord1->nPosX + pDrlgCoord1->nWidth)
		{
			return DIRECTION_SOUTHEAST;
		}
	}
	else
	{
		if (pDrlgCoord1->nPosX == pDrlgCoord2->nPosX + pDrlgCoord2->nWidth)
		{
			return DIRECTION_SOUTHWEST;
		}
	}

	if (pDrlgCoord1->nPosY <= pDrlgCoord2->nPosY)
	{
		if (pDrlgCoord2->nPosY == pDrlgCoord1->nPosY + pDrlgCoord1->nHeight)
		{
			return DIRECTION_NORTHEAST;
		}
	}
	else
	{
		if (pDrlgCoord1->nPosY == pDrlgCoord2->nPosY + pDrlgCoord2->nHeight)
		{
			return DIRECTION_NORTHWEST;
		}
	}

	return DIRECTION_INVALID;
}

//D2Common.0x6FD74A40
void __fastcall DRLG_CreateRoomForRoomEx(D2DrlgStrc* pDrlg, D2DrlgRoomStrc* pDrlgRoom)
{
	D2DrlgCoordsStrc pDrlgCoords = {};
	uint32_t dwFlags = 0;

	pDrlgCoords.nTileXPos = pDrlgRoom->nTileXPos;
	pDrlgCoords.nSubtileX = pDrlgRoom->nTileXPos;
	pDrlgCoords.nTileYPos = pDrlgRoom->nTileYPos;
	pDrlgCoords.nSubtileY = pDrlgRoom->nTileYPos;
	pDrlgCoords.nTileWidth = pDrlgRoom->nTileWidth;
	pDrlgCoords.nSubtileWidth = pDrlgRoom->nTileWidth;
	pDrlgCoords.nTileHeight = pDrlgRoom->nTileHeight;
	pDrlgCoords.nSubtileHeight = pDrlgRoom->nTileHeight;

	DUNGEON_GameTileToSubtileCoords(&pDrlgCoords.nSubtileX, &pDrlgCoords.nSubtileY);
	DUNGEON_GameTileToSubtileCoords(&pDrlgCoords.nSubtileWidth, &pDrlgCoords.nSubtileHeight);

	if (pDrlgRoom->pTileGrid->pTiles.nWalls || pDrlgRoom->pTileGrid->pTiles.nFloors)
	{
		if (pDrlgRoom->dwFlags & DRLGROOMFLAG_AUTOMAP_REVEAL)
		{
			dwFlags = 4;
		}
		else if (pDrlgRoom->dwOtherFlags & 1)
		{
			dwFlags = 1;
		}
		else
		{
			dwFlags = 0;
		}

		pDrlgRoom->pRoom = DUNGEON_AllocRoom(pDrlg->pAct, pDrlgRoom, &pDrlgCoords, &pDrlgRoom->pTileGrid->pTiles, (int)SEED_RollRandomNumber(&pDrlgRoom->pSeed), dwFlags);
	}
}

//D2Common.0x6FD74B30
int* __fastcall DRLG_GetRoomCenterX_RoomWarpXFromRoom(D2DrlgRoomStrc* pDrlgRoom)
{
	return pDrlgRoom->pLevel->nRoom_Center_Warp_X;
}

//D2Common.0x6FD74B40
void __fastcall DRLG_ComputeLevelWarpInfo(D2DrlgLevelStrc* pLevel)
{
	for (D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
	{
		// First check if we have a waypoint
		bool bHasWarp = (pDrlgRoom->dwFlags & DRLGROOMFLAG_HAS_WAYPOINT_MASK) != 0;

		// Then check for additional warps
		if ((pDrlgRoom->dwFlags & DRLGROOMFLAG_HAS_WARP_MASK) != 0 && !bHasWarp)
		{
			int nWarpIndex = 0;
			for (int warpMask = DRLGROOMFLAG_HAS_WARP_0; (warpMask & DRLGROOMFLAG_HAS_WARP_MASK) != 0; warpMask <<= 1)
			{
				if (pDrlgRoom->dwFlags & warpMask && DRLGWARP_GetWarpDestinationFromArray(pLevel, nWarpIndex) != -1)
				{
					bHasWarp = true;
				}
				nWarpIndex++;
			}
		}

		if (bHasWarp)
		{
			int* pX = &pLevel->nRoom_Center_Warp_X[pLevel->nRoomCoords];
			int* pY = &pLevel->nRoom_Center_Warp_Y[pLevel->nRoomCoords];

			// Put warp in the center of the tile
			*pX = pDrlgRoom->nTileXPos + pDrlgRoom->nTileWidth / 2;
			*pY = pDrlgRoom->nTileYPos + pDrlgRoom->nTileHeight / 2;

			DUNGEON_GameTileToSubtileCoords(pX, pY);

			++pLevel->nRoomCoords;
		}
	}
}

//D2Common.0x6FD74C10 (#10006)
void __stdcall DRLG_InitLevel(D2DrlgLevelStrc* pLevel)
{
	SEED_InitLowSeed(&pLevel->pSeed, pLevel->nLevelId + pLevel->pDrlg->dwStartSeed);

	switch (pLevel->nDrlgType)
	{
	case DRLGTYPE_MAZE:
		DRLGMAZE_GenerateLevel(pLevel);
		break;

	case DRLGTYPE_PRESET:
		DRLGPRESET_GenerateLevel(pLevel);
		break;

	case DRLGTYPE_OUTDOOR:
		DRLGOUTDOORS_GenerateLevel(pLevel);
		break;

	default:
		return;
	}

	if (pLevel->nRooms && pLevel->pPresetMaps)
	{
		int nCounter = 0;
		for (D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
		{
			if (pLevel->pPresetMaps[nCounter])
			{
				pDrlgRoom->dwOtherFlags |= 1;
			}

			++nCounter;
		}
	}

	DRLG_ComputeLevelWarpInfo(pLevel);
}

//D2Common.0x6FD74D50
int __fastcall DRLG_GetNumberOfPopulatedRoomsInLevel(D2DrlgStrc* pDrlg, int nLevelId)
{
	D2DrlgLevelStrc* pLevel = DRLG_GetLevel(pDrlg, nLevelId);
	int nCounter = 0;

	for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
	{
		if (!(i->dwFlags & DRLGROOMFLAG_POPULATION_ZERO))
		{
			++nCounter;
		}
	}

	return nCounter;
}

//D2Common.0x6FD74D90
void __fastcall DRLG_GetMinAndMaxCoordinatesFromLevel(D2DrlgLevelStrc* pLevel, int* pTileMinX, int* pTileMinY, int* pTileMaxX, int* pTileMaxY)
{
	D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx;

	*pTileMinX = pDrlgRoom->nTileXPos;
	*pTileMinY = pDrlgRoom->nTileYPos;
	*pTileMaxX = pDrlgRoom->nTileXPos + pDrlgRoom->nTileWidth;
	*pTileMaxY = pDrlgRoom->nTileYPos + pDrlgRoom->nTileHeight;

	while (pDrlgRoom)
	{
		if (pDrlgRoom->nTileXPos + pDrlgRoom->nTileWidth > *pTileMaxX)
		{
			*pTileMaxX = pDrlgRoom->nTileXPos + pDrlgRoom->nTileWidth;
		}

		if (pDrlgRoom->nTileXPos < *pTileMinX)
		{
			*pTileMinX = pDrlgRoom->nTileXPos;
		}

		if (pDrlgRoom->nTileYPos + pDrlgRoom->nTileHeight > *pTileMaxY)
		{
			*pTileMaxY = pDrlgRoom->nTileYPos + pDrlgRoom->nTileHeight;
		}

		if (pDrlgRoom->nTileYPos < *pTileMinY)
		{
			*pTileMinY = pDrlgRoom->nTileYPos;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}
}

//D2Common.0x6FD74E10
void __fastcall DRLG_UpdateRoomExCoordinates(D2DrlgLevelStrc* pLevel)
{
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	int nTileMaxX = 0;
	int nTileMinX = 0;
	int nTileMaxY = 0;
	int nTileMinY = 0;

	if (!pLevel)
	{
		FOG_DisplayWarning("ptDrlgLevel", __FILE__, __LINE__);
	}

	if (!pLevel->pFirstRoomEx)
	{
		FOG_DisplayWarning("ptDrlgLevel->ptRoomFirst", __FILE__, __LINE__);
	}

	DRLG_GetMinAndMaxCoordinatesFromLevel(pLevel, &nTileMinX, &nTileMinY, &nTileMaxX, &nTileMaxY);

	if (pLevel->nWidth < nTileMaxX - nTileMinX)
	{
		FOG_DisplayWarning("ptCoordsLevel->nSizeTileX >= nTileMaxX - nTileMinX", __FILE__, __LINE__);
	}

	if (pLevel->nHeight < nTileMaxY - nTileMinY)
	{
		FOG_DisplayWarning("ptCoordsLevel->nSizeTileY >= nTileMaxY - nTileMinY", __FILE__, __LINE__);
	}

	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		pDrlgRoom->nTileXPos += pLevel->nPosX - nTileMinX;
		pDrlgRoom->nTileYPos += pLevel->nPosY - nTileMinY;

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}
}

//D2Common.0x6FD74EF0
D2DrlgRoomStrc* __fastcall DRLG_GetRoomExFromLevelAndCoordinates(D2DrlgLevelStrc* pLevel, int nX, int nY)
{
	D2DrlgRoomStrc* pDrlgRoom = NULL;

	if (!pLevel)
	{
		FOG_DisplayWarning("ptDrlgLevel", __FILE__, __LINE__);
	}

	if (!pLevel->pFirstRoomEx)
	{
		FOG_DisplayWarning("ptDrlgLevel->ptRoomFirst", __FILE__, __LINE__);
	}

	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		if (DRLGROOM_AreXYInsideCoordinates(&pDrlgRoom->pDrlgCoord, nX, nY))
		{
			return pDrlgRoom;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}

	return NULL;
}

//D2Common.0x6FD74F70
D2DrlgRoomStrc* __fastcall DRLG_GetRoomExFromCoordinates(int nX, int nY, D2DrlgStrc* pDrlg, D2DrlgRoomStrc* pDrlgRoomHint, D2DrlgLevelStrc* pLevel)
{

	if (pDrlgRoomHint)
	{
		if (DRLGROOM_AreXYInsideCoordinates(&pDrlgRoomHint->pDrlgCoord, nX, nY))
		{
			return pDrlgRoomHint;
		}

		for (int i = 0; i < pDrlgRoomHint->nRoomsNear; ++i)
		{
			if (pDrlgRoomHint != pDrlgRoomHint->ppRoomsNear[i])
			{
				if (DRLGROOM_AreXYInsideCoordinates(&pDrlgRoomHint->ppRoomsNear[i]->pDrlgCoord, nX, nY))
				{
					return pDrlgRoomHint->ppRoomsNear[i];
				}
			}
		}
	}

	if (!pLevel)
	{
		int nLevelId = 0;
		if (pDrlg->pLevel)
		{
			D2DrlgLevelStrc* pCurrentLevel = pDrlg->pLevel;

			while (!DRLGROOM_AreXYInsideCoordinates(&pCurrentLevel->pLevelCoords, nX, nY))
			{
				pCurrentLevel = pCurrentLevel->pNextLevel;
				if (!pCurrentLevel)
				{
					break;
				}
			}

			if (pCurrentLevel)
			{
				nLevelId = pCurrentLevel->nLevelId;
			}
		}

		pLevel = DRLG_GetLevel(pDrlg, nLevelId);
	}

	if (!pLevel->pFirstRoomEx)
	{
		DRLG_InitLevel(pLevel);
	}

	return DRLG_GetRoomExFromLevelAndCoordinates(pLevel, nX, nY);
}

//1.10f: D2Common.0x6FD751C0
//1.13c: D2Common.0x6FD7D320
BOOL __fastcall DRLG_IsTownLevel(int nLevelId)
{
	switch (nLevelId)
	{
	case LEVEL_ROGUEENCAMPMENT:
	case LEVEL_LUTGHOLEIN:
	case LEVEL_KURASTDOCKTOWN:
	case LEVEL_THEPANDEMONIUMFORTRESS:
	case LEVEL_HARROGATH:
		return TRUE;

	default:
		return FALSE;
	}
}

//D2Common.0x6FD75260 (#10000)
int __stdcall DRLG_GetLevelTypeFromLevelId(int nLevelId)
{
	return DATATBLS_GetLevelDefRecord(nLevelId)->dwLevelType;
}

//D2Common.0x6FD75270
void __fastcall DRLG_SetLevelPositionAndSize(D2DrlgStrc* pDrlg, D2DrlgLevelStrc* pLevel)
{
	D2LevelDefBin* pLevelDefBin = DATATBLS_GetLevelDefRecord(pLevel->nLevelId);
	D2DrlgLevelStrc* pDependLevel = NULL;
	int nX = 0;
	int nY = 0;

	pLevel->nWidth = pLevelDefBin->dwSizeX[pDrlg->nDifficulty];
	pLevel->nHeight = pLevelDefBin->dwSizeY[pDrlg->nDifficulty];

	if (pLevelDefBin->dwDepend)
	{
		pDependLevel = DRLG_GetLevel(pDrlg, pLevelDefBin->dwDepend);

		nX = pDependLevel->nPosX;
		nY = pDependLevel->nPosY;
	}

	pLevel->nPosX = nX + pLevelDefBin->dwOffsetX;
	pLevel->nPosY = nY + pLevelDefBin->dwOffsetY;
}

//D2Common.0x6FD75300 (#10001)
uint8_t __stdcall DRLG_GetActNoFromLevelId(int nLevelId)
{
	//IMPROVEMENT: Lookup the act from Levels.txt

	static const int gnTownIds[] = { LEVEL_ROGUEENCAMPMENT, LEVEL_LUTGHOLEIN, LEVEL_KURASTDOCKTOWN, LEVEL_THEPANDEMONIUMFORTRESS, LEVEL_HARROGATH, 1024 };

	int nAct = 1;

	while (nLevelId >= gnTownIds[nAct])
	{
		++nAct;
		if (nAct > 5)
		{
			return 0;
		}
	}

	return nAct - 1;
}

//D2Common.0x6FD75330 (#10004)
int __stdcall DRLG_GetSaveMonstersFromLevelId(int nLevelId)
{
	return DATATBLS_GetLevelDefRecord(nLevelId)->dwSaveMonsters;
}

//D2Common.0x6FD75350 (#10002)
int __stdcall DRLG_GetLOSDrawFromLevelId(int nLevelId)
{
	return DATATBLS_GetLevelDefRecord(nLevelId)->dwLOSDraw;
}

//D2Common.0x6FD75370
D2DrlgWarpStrc* __fastcall DRLG_GetDrlgWarpFromLevelId(D2DrlgStrc* pDrlg, int nLevelId)
{
	D2LevelDefBin* pLevelDefBin = NULL;
	D2DrlgWarpStrc* pDrlgWarp = NULL;

	for (D2DrlgWarpStrc* i = pDrlg->pWarp; i; i = i->pNext)
	{
		if (i->nLevel == nLevelId)
		{
			return i;
		}
	}

	pDrlgWarp = D2_ALLOC_STRC_POOL(pDrlg->pMempool, D2DrlgWarpStrc);
	pDrlgWarp->nLevel = nLevelId;

	pLevelDefBin = DATATBLS_GetLevelDefRecord(nLevelId);
	for (int i = 0; i < 8; ++i)
	{
		pDrlgWarp->nVis[i] = pLevelDefBin->dwVis[i];
		pDrlgWarp->nWarp[i] = pLevelDefBin->dwWarp[i];
	}

	pDrlgWarp->pNext = pDrlg->pWarp;
	pDrlg->pWarp = pDrlgWarp;

	return pDrlgWarp;
}

//D2Common.0x6FD753F0
void __fastcall DRLG_SetWarpId(D2DrlgWarpStrc* pDrlgWarp, int nVis, int nWarp, int nId)
{
	for (int i = 0; i < 8; ++i)
	{
		if (pDrlgWarp->nVis[i] == nVis)
		{
			pDrlgWarp->nWarp[i] = nWarp;
			return;
		}
	}

	if (nId == -1)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (!pDrlgWarp->nVis[i] && pDrlgWarp->nWarp[i] == -1)
			{
				pDrlgWarp->nVis[i] = nVis;
				pDrlgWarp->nWarp[i] = nWarp;
				return;
			}
		}
		D2_UNREACHABLE;
	}
	D2_ASSERT(nId >= 0 && nId < 8);


	pDrlgWarp->nVis[nId] = nVis;
	pDrlgWarp->nWarp[nId] = nWarp;
}

//D2Common.0x6FD75450
int __fastcall DRLG_IsOnClient(D2DrlgStrc* pDrlg)
{
	D2_ASSERT(pDrlg);

	return pDrlg->dwFlags & DRLGFLAG_ONCLIENT;
}
