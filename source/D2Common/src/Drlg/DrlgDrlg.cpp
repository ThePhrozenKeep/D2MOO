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

static_assert(ROOMEXFLAG_HAS_WARP_0 == (1 << ROOMEXFLAG_HAS_WARP_FIRST_BIT), "Warp first bit must match of ROOMEXFLAG_HAS_WARP_0");
static_assert(ROOMEXFLAG_SUBSHRINE_ROW1 == (1 << ROOMEXFLAG_SUBSHRINE_ROWS_FIRST_BIT), "Subshrines first bit must match of ROOMEXFLAG_SUBSHRINE_ROW1");
static_assert(ROOMEXFLAG_HAS_WAYPOINT == (1 << ROOMEXFLAG_HAS_WAYPOINT_FIRST_BIT), "Waypoint first bit must match of ROOMEXFLAG_HAS_WAYPOINT");

//D2Common.0x6FD74120 (#10014)
D2DrlgStrc* __fastcall DRLG_AllocDrlg(D2DrlgActStrc* pAct, uint8_t nActNo, void* pDS1MemPool, uint32_t nInitSeed, int nTownLevelId, uint32_t nFlags, D2GameStrc* pGame, uint8_t nDifficulty, AUTOMAPFN pfAutoMap, TOWNAUTOMAPFN pfTownAutoMap)
{
	D2DrlgStrc* pDrlg = D2_CALLOC_STRC_SERVER(pAct->pMemPool, D2DrlgStrc);

	pDrlg->pAct = pAct;
	pDrlg->pMempool = pAct->pMemPool;
	D2_ASSERT(pDS1MemPool == nullptr);
	pDrlg->pDS1MemPool = pDS1MemPool; // Always nullptr in the game
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
			D2_FREE_SERVER(pDrlg->pMempool, pLevel);
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
			D2_FREE_SERVER(pDrlg->pMempool, pWarp);
			pWarp = pNextWarp;
		}
		while (pWarp);

		pDrlg->pWarp = NULL;
	}

	D2_FREE_SERVER(pDrlg->pMempool, pDrlg);
}

//D2Common.0x6FD74440
void __fastcall DRLG_FreeLevel(void* pMemPool, D2DrlgLevelStrc* pLevel, BOOL bAlloc)
{
	D2DrlgBuildStrc* pNextDrlgBuild = NULL;
	D2DrlgBuildStrc* pDrlgBuild = NULL;
	D2RoomExStrc* pNextRoomEx = NULL;
	D2RoomExStrc* pRoomEx = NULL;
	int nCounter = 0;

	if (bAlloc)
	{
		if (!pLevel->pPresetMaps && pLevel->nRooms)
		{
			pLevel->pPresetMaps = (int*)D2_CALLOC_SERVER(pMemPool, sizeof(int) * pLevel->nRooms);
		}
	}
	else
	{
		if (pLevel->pPresetMaps)
		{
			D2_FREE_SERVER(pMemPool, pLevel->pPresetMaps);
			pLevel->pPresetMaps = NULL;
		}
	}

	pNextRoomEx = pLevel->pFirstRoomEx;
	if (pNextRoomEx)
	{
		nCounter = 0;
		do
		{
			pRoomEx = pNextRoomEx;
			pNextRoomEx = pNextRoomEx->pRoomExNext;
			if (pLevel->pPresetMaps)
			{
				pLevel->pPresetMaps[nCounter] = pRoomEx->dwOtherFlags & 1;
				++nCounter;
			}

			DRLGROOM_FreeRoomEx(pRoomEx);
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
		FOG_FreeServerMemory(pMemPool, pLevel->pJungleDefs, __FILE__, __LINE__, 0);
		pLevel->pJungleDefs = NULL;
	}

	pNextDrlgBuild = pLevel->pBuild;
	if (pNextDrlgBuild)
	{
		do
		{
			pDrlgBuild = pNextDrlgBuild;
			pNextDrlgBuild = pNextDrlgBuild->pNext;
			D2_FREE_SERVER(pMemPool, pDrlgBuild);
		}
		while (pNextDrlgBuild);

		pLevel->pBuild = NULL;
	}
}

//D2Common.0x6FD745C0
//TODO: Name
void __fastcall sub_6FD745C0(D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2)
{
	D2DrlgLevelStrc* pLevel1 = NULL;
	D2DrlgLevelStrc* pLevel2 = NULL;
	D2DrlgLevelStrc* pLevel = NULL;
	int* pVisLevelIds = NULL;
	int nVisLevelId = 0;

	if (pRoomEx1)
	{
		pLevel1 = pRoomEx1->pLevel;
	}

	if (pRoomEx2)
	{
		pLevel2 = pRoomEx2->pLevel;
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
//TODO: Name
void __fastcall sub_6FD74700(D2DrlgStrc* pDrlg)
{
	D2DrlgLevelStrc* pCurrentLevel = NULL;
	D2RoomExStrc* pRoomEx = NULL;
	D2RoomExStrc* k = NULL;
	int* pLevelIds = NULL;
	int* pLevels = NULL;
	int dwFlags = 0;
	
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
				pRoomEx = pLevel->pFirstRoomEx;

				while (pRoomEx && pRoomEx->fRoomStatus > 3 && !(pRoomEx->dwFlags & ROOMEXFLAG_HAS_ROOM))
				{
					pRoomEx = pRoomEx->pRoomExNext;
				}

				if (pRoomEx)
				{
					pLevel->dwInactiveFrames = 10;
				}
				else
				{
					pLevelIds = DRLGROOM_GetVisArrayFromLevelId(pLevel->pDrlg, pLevel->nLevelId);

					for (int j = 0; j < 8; ++j)
					{
						if (pLevelIds[j])
						{
							pCurrentLevel = DRLG_GetLevel(pLevel->pDrlg, pLevelIds[j]);

							if (pCurrentLevel->pFirstRoomEx)
							{
								pLevels = DRLGROOM_GetVisArrayFromLevelId(pCurrentLevel->pDrlg, pCurrentLevel->nLevelId);
								dwFlags = 0;

								for (int i = 0; i < 8; ++i)
								{
									if (pLevels[i] == pLevel->nLevelId)
									{
										dwFlags |= (1 << (i + 4));
									}
								}

								for (k = pCurrentLevel->pFirstRoomEx; k; k = k->pRoomExNext)
								{
									if (k->dwFlags & dwFlags && (k->fRoomStatus <= 3 || k->dwFlags & ROOMEXFLAG_HAS_ROOM))
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
									for (D2RoomExStrc* i = pCurrentLevel->pFirstRoomEx; i; i = i->pRoomExNext)
									{
										if (i->dwFlags & dwFlags)
										{
											DRLGROOM_FreeRoomTiles(pLevel->pDrlg->pMempool, i);

											if (i->ppRoomsNear)
											{
												D2_FREE_SERVER(pLevel->pDrlg->pMempool, i->ppRoomsNear);
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
	D2DrlgLevelStrc* pLevel = D2_CALLOC_STRC_SERVER(pDrlg->pMempool, D2DrlgLevelStrc);

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
void __fastcall DRLG_CreateRoomForRoomEx(D2DrlgStrc* pDrlg, D2RoomExStrc* pRoomEx)
{
	D2DrlgCoordsStrc pDrlgCoords = {};
	uint32_t dwFlags = 0;

	pDrlgCoords.dwTilesLeft = pRoomEx->nTileXPos;
	pDrlgCoords.dwSubtilesLeft = pRoomEx->nTileXPos;
	pDrlgCoords.dwTilesTop = pRoomEx->nTileYPos;
	pDrlgCoords.dwSubtilesTop = pRoomEx->nTileYPos;
	pDrlgCoords.dwTilesWidth = pRoomEx->nTileWidth;
	pDrlgCoords.dwSubtilesWidth = pRoomEx->nTileWidth;
	pDrlgCoords.dwTilesHeight = pRoomEx->nTileHeight;
	pDrlgCoords.dwSubtilesHeight = pRoomEx->nTileHeight;

	DUNGEON_ExpandCoords(&pDrlgCoords.dwSubtilesLeft, &pDrlgCoords.dwSubtilesTop);
	DUNGEON_ExpandCoords(&pDrlgCoords.dwSubtilesWidth, &pDrlgCoords.dwSubtilesHeight);

	if (pRoomEx->pTileGrid->pTiles.nWalls || pRoomEx->pTileGrid->pTiles.nFloors)
	{
		if (pRoomEx->dwFlags & ROOMEXFLAG_AUTOMAP_REVEAL)
		{
			dwFlags = 4;
		}
		else if (pRoomEx->dwOtherFlags & 1)
		{
			dwFlags = 1;
		}
		else
		{
			dwFlags = 0;
		}

		pRoomEx->pRoom = DUNGEON_AllocRoom(pDrlg->pAct, pRoomEx, &pDrlgCoords, &pRoomEx->pTileGrid->pTiles, (int)SEED_RollRandomNumber(&pRoomEx->pSeed), dwFlags);
	}
}

//D2Common.0x6FD74B30
int* __fastcall DRLG_GetRoomCenterX_RoomWarpXFromRoom(D2RoomExStrc* pRoomEx)
{
	return pRoomEx->pLevel->nRoom_Center_Warp_X;
}

//D2Common.0x6FD74B40
void __fastcall DRLG_ComputeLevelWarpInfo(D2DrlgLevelStrc* pLevel)
{
	for (D2RoomExStrc* pRoomEx = pLevel->pFirstRoomEx; pRoomEx; pRoomEx = pRoomEx->pRoomExNext)
	{
		// First check if we have a waypoint
		bool bHasWarp = (pRoomEx->dwFlags & ROOMEXFLAG_HAS_WAYPOINT_MASK) != 0;

		// Then check for additional warps
		if ((pRoomEx->dwFlags & ROOMEXFLAG_HAS_WARP_MASK) != 0 && !bHasWarp)
		{
			int nWarpIndex = 0;
			for (int warpMask = ROOMEXFLAG_HAS_WARP_0; (warpMask & ROOMEXFLAG_HAS_WARP_MASK) != 0; warpMask <<= 1)
			{
				if (pRoomEx->dwFlags & warpMask && DRLGWARP_GetWarpDestinationFromArray(pLevel, nWarpIndex) != -1)
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
			*pX = pRoomEx->nTileXPos + pRoomEx->nTileWidth / 2;
			*pY = pRoomEx->nTileYPos + pRoomEx->nTileHeight / 2;

			DUNGEON_ExpandCoords(pX, pY);

			++pLevel->nRoomCoords;
		}
	}
}

//D2Common.0x6FD74C10 (#10006)
void __stdcall DRLG_InitLevel(D2DrlgLevelStrc* pLevel)
{
	int* pX = NULL;
	int* pY = NULL;
	int nCounter = 0;
	int nFlag = 0;
	BOOL bFlagSet = FALSE;

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
		for (D2RoomExStrc* pRoomEx = pLevel->pFirstRoomEx; pRoomEx; pRoomEx = pRoomEx->pRoomExNext)
		{
			if (pLevel->pPresetMaps[nCounter])
			{
				pRoomEx->dwOtherFlags |= 1;
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

	for (D2RoomExStrc* i = pLevel->pFirstRoomEx; i; i = i->pRoomExNext)
	{
		if (!(i->dwFlags & ROOMEXFLAG_POPULATION_ZERO))
		{
			++nCounter;
		}
	}

	return nCounter;
}

//D2Common.0x6FD74D90
void __fastcall DRLG_GetMinAndMaxCoordinatesFromLevel(D2DrlgLevelStrc* pLevel, int* pTileMinX, int* pTileMinY, int* pTileMaxX, int* pTileMaxY)
{
	D2RoomExStrc* pRoomEx = pLevel->pFirstRoomEx;

	*pTileMinX = pRoomEx->nTileXPos;
	*pTileMinY = pRoomEx->nTileYPos;
	*pTileMaxX = pRoomEx->nTileXPos + pRoomEx->nTileWidth;
	*pTileMaxY = pRoomEx->nTileYPos + pRoomEx->nTileHeight;

	while (pRoomEx)
	{
		if (pRoomEx->nTileXPos + pRoomEx->nTileWidth > *pTileMaxX)
		{
			*pTileMaxX = pRoomEx->nTileXPos + pRoomEx->nTileWidth;
		}

		if (pRoomEx->nTileXPos < *pTileMinX)
		{
			*pTileMinX = pRoomEx->nTileXPos;
		}

		if (pRoomEx->nTileYPos + pRoomEx->nTileHeight > *pTileMaxY)
		{
			*pTileMaxY = pRoomEx->nTileYPos + pRoomEx->nTileHeight;
		}

		if (pRoomEx->nTileYPos < *pTileMinY)
		{
			*pTileMinY = pRoomEx->nTileYPos;
		}

		pRoomEx = pRoomEx->pRoomExNext;
	}
}

//D2Common.0x6FD74E10
void __fastcall DRLG_UpdateRoomExCoordinates(D2DrlgLevelStrc* pLevel)
{
	D2RoomExStrc* pRoomEx = NULL;
	int nTileMaxX = 0;
	int nTileMinX = 0;
	int nTileMaxY = 0;
	int nTileMinY = 0;

	if (!pLevel)
	{
		FOG_10025("ptDrlgLevel", __FILE__, __LINE__);
	}

	if (!pLevel->pFirstRoomEx)
	{
		FOG_10025("ptDrlgLevel->ptRoomFirst", __FILE__, __LINE__);
	}

	DRLG_GetMinAndMaxCoordinatesFromLevel(pLevel, &nTileMinX, &nTileMinY, &nTileMaxX, &nTileMaxY);

	if (pLevel->nWidth < nTileMaxX - nTileMinX)
	{
		FOG_10025("ptCoordsLevel->nSizeTileX >= nTileMaxX - nTileMinX", __FILE__, __LINE__);
	}

	if (pLevel->nHeight < nTileMaxY - nTileMinY)
	{
		FOG_10025("ptCoordsLevel->nSizeTileY >= nTileMaxY - nTileMinY", __FILE__, __LINE__);
	}

	pRoomEx = pLevel->pFirstRoomEx;
	while (pRoomEx)
	{
		pRoomEx->nTileXPos += pLevel->nPosX - nTileMinX;
		pRoomEx->nTileYPos += pLevel->nPosY - nTileMinY;

		pRoomEx = pRoomEx->pRoomExNext;
	}
}

//D2Common.0x6FD74EF0
D2RoomExStrc* __fastcall DRLG_GetRoomExFromLevelAndCoordinates(D2DrlgLevelStrc* pLevel, int nX, int nY)
{
	D2RoomExStrc* pRoomEx = NULL;

	if (!pLevel)
	{
		FOG_10025("ptDrlgLevel", __FILE__, __LINE__);
	}

	if (!pLevel->pFirstRoomEx)
	{
		FOG_10025("ptDrlgLevel->ptRoomFirst", __FILE__, __LINE__);
	}

	pRoomEx = pLevel->pFirstRoomEx;
	while (pRoomEx)
	{
		if (DRLGROOM_AreXYInsideCoordinates(&pRoomEx->pDrlgCoord, nX, nY))
		{
			return pRoomEx;
		}

		pRoomEx = pRoomEx->pRoomExNext;
	}

	return NULL;
}

//D2Common.0x6FD74F70
D2RoomExStrc* __fastcall DRLG_GetRoomExFromCoordinates(int nX, int nY, D2DrlgStrc* pDrlg, D2RoomExStrc* pRoomExHint, D2DrlgLevelStrc* pLevel)
{

	if (pRoomExHint)
	{
		if (DRLGROOM_AreXYInsideCoordinates(&pRoomExHint->pDrlgCoord, nX, nY))
		{
			return pRoomExHint;
		}

		for (int i = 0; i < pRoomExHint->nRoomsNear; ++i)
		{
			if (pRoomExHint != pRoomExHint->ppRoomsNear[i])
			{
				if (DRLGROOM_AreXYInsideCoordinates(&pRoomExHint->ppRoomsNear[i]->pDrlgCoord, nX, nY))
				{
					return pRoomExHint->ppRoomsNear[i];
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

//D2Common.0x6FD751C0
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

	pDrlgWarp = D2_ALLOC_STRC_SERVER(pDrlg->pMempool, D2DrlgWarpStrc);
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
