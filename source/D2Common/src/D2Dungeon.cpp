#include "D2Dungeon.h"

#include "D2Collision.h"
#include "D2DataTbls.h"
#include <DataTbls/LevelsIds.h>
#include "Drlg/D2DrlgActivate.h"
#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgAnim.h"
#include "Drlg/D2DrlgDrlgLogic.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "Drlg/D2DrlgDrlgWarp.h"
#include "Drlg/D2DrlgPreset.h"
#include "D2Environment.h"
#include "Units/UnitRoom.h"
#include "Units/Units.h"
#include "D2Seed.h"


//D2Common.0x6FD8B8A0 (#10038)
D2DrlgActStrc* __stdcall DUNGEON_AllocAct(uint8_t nAct, uint32_t nInitSeed, BOOL bClient, D2GameStrc* pGame, uint8_t nDifficulty, void* pMemPool, int nTownLevelId, AUTOMAPFN pfAutoMap, TOWNAUTOMAPFN pfTownAutoMap)
{
	D2DrlgActStrc* pAct = D2_CALLOC_STRC_POOL(pMemPool, D2DrlgActStrc);

	pAct->dwInitSeed = nInitSeed;
	pAct->bClient = bClient;
	pAct->nAct = nAct;
	pAct->pMemPool = pMemPool;

	if (!bClient)
	{
		pAct->nTownId = nTownLevelId;
	}

	pAct->pDrlg = DRLG_AllocDrlg(pAct, nAct, 0, nInitSeed, (bClient ? LEVEL_NONE : nTownLevelId), (bClient ? DRLGFLAG_ONCLIENT : 0), pGame, nDifficulty, pfAutoMap, pfTownAutoMap);
	pAct->pEnvironment = ENVIRONMENT_AllocDrlgEnvironment(pMemPool);

	DRLGANIM_InitCache(pAct->pDrlg, &pAct->pTileData);

	return pAct;
}

//D2Common.0x6FD8B950 (#10039)
void __stdcall DUNGEON_FreeAct(D2DrlgActStrc* pAct)
{
	D2RoomStrc* pRoom = NULL;
	D2RoomStrc* pNext = NULL;

	D2_ASSERT(pAct);

	if (pAct->pDrlg)
	{
		DRLG_FreeDrlg(pAct->pDrlg);
		pAct->pDrlg = NULL;
	}

	pRoom = pAct->pRoom;
	while (pRoom)
	{
		pNext = pRoom->pRoomNext;
		DUNGEON_FreeRoom(pAct->pMemPool, pRoom);
		pRoom = pNext;
	}

	if (pAct->pEnvironment)
	{
		ENVIRONMENT_FreeDrlgEnvironment(pAct->pMemPool, pAct->pEnvironment);
		pAct->pEnvironment = NULL;
	}

	D2_FREE_POOL(pAct->pMemPool, pAct);
}

//D2Common.0x6FD8B9D0
void* __fastcall DUNGEON_GetMemPoolFromAct(D2DrlgActStrc* pAct)
{
	return pAct->pMemPool;
}

//D2Common.0x6FD8B9E0 (#10026)
void __stdcall DUNGEON_ToggleRoomTilesEnableFlag(D2RoomStrc* pRoom, BOOL bEnabled)
{
	D2_ASSERT(pRoom);

	DRLGWARP_ToggleRoomTilesEnableFlag(pRoom->pRoomEx, bEnabled);
}

//D2Common.0x6FD8BA20 (#10027)
D2UnitStrc* __stdcall DUNGEON_GetWarpTileFromRoomAndSourceLevelId(D2RoomStrc* pRoom, int nSourceLevel, D2LvlWarpTxt** ppLvlWarpTxtRecord)
{
	D2RoomStrc* pDestRoom = NULL;
	int nDestinationLevel = 0;

	D2_ASSERT(pRoom);

	pDestRoom = DRLGWARP_GetDestinationRoom(pRoom->pRoomEx, nSourceLevel, &nDestinationLevel, ppLvlWarpTxtRecord);
	D2_ASSERT(pDestRoom);

	for (D2UnitStrc* pTile = pDestRoom->pUnitFirst; pTile; pTile = pTile->pRoomNext)
	{
		if (pTile->dwUnitType == UNIT_TILE && pTile->dwClassId == nDestinationLevel)
		{
			return pTile;
		}
	}

	return NULL;
}

//D2Common.0x6FD8BAB0 (#10028)
D2LvlWarpTxt* __stdcall DUNGEON_GetLvlWarpTxtRecordFromRoomAndUnit(D2RoomStrc* pRoom, D2UnitStrc* pUnit)
{
	D2_ASSERT(pRoom);

	return DRLGWARP_GetLvlWarpTxtRecordFromUnit(pRoom->pRoomEx, pUnit);
}

//D2Common.0x6FD8BAF0 (#10030)
D2DrlgTileDataStrc* __stdcall DUNGEON_GetFloorTilesFromRoom(D2RoomStrc* pRoom, int* pFloorCount)
{
	if (pRoom && pRoom->pRoomTiles)
	{
		*pFloorCount = pRoom->pRoomTiles->nFloors;
		return pRoom->pRoomTiles->pFloorTiles;
	}

	*pFloorCount = 0;
	return NULL;
}

//D2Common.0x6FD8BB20 (#10031)
D2DrlgTileDataStrc* __stdcall DUNGEON_GetWallTilesFromRoom(D2RoomStrc* pRoom, int* pWallCount)
{
	D2_ASSERT(pRoom);

	*pWallCount = pRoom->pRoomTiles->nWalls;
	return pRoom->pRoomTiles->pWallTiles;
}

//D2Common.0x6FD8BB60 (#10032)
D2DrlgTileDataStrc* __stdcall DUNGEON_GetRoofTilesFromRoom(D2RoomStrc* pRoom, int* pRoofCount)
{
	D2_ASSERT(pRoom);

	*pRoofCount = pRoom->pRoomTiles->nRoofs;
	return pRoom->pRoomTiles->pRoofTiles;
}

//D2Common.0x6FD8BBA0 (#10033)
D2DrlgTileDataStrc* __stdcall DUNGEON_GetTileDataFromAct(D2DrlgActStrc* pAct)
{
	return &pAct->pTileData;
}

//D2Common.0x6FD8BBB0 (#10034)
void __stdcall DUNGEON_GetRoomCoordinates(D2RoomStrc* pRoom, D2DrlgCoordsStrc* pCoords)
{
	D2_ASSERT(pCoords);

	if (pRoom)
	{
		memcpy(pCoords, &pRoom->pCoords, sizeof(D2DrlgCoordsStrc));
	}
	else
	{
		FOG_Trace(" ----- RJS NOTE: Would have crashed, see code");
		memset(pCoords, 0x00, sizeof(D2DrlgCoordsStrc));
	}
}

//D2Common.0x6FD8BC10 (#10035)
void __stdcall DUNGEON_GetAdjacentRoomsListFromRoom(D2RoomStrc* pRoom, D2RoomStrc*** pppRoomList, int* pNumRooms)
{
	D2_ASSERT(pRoom);

	*pNumRooms = pRoom->nNumRooms;
	*pppRoomList = pRoom->ppRoomList;
}

//D2Common.0x6FD8BC50
D2RoomStrc* __fastcall DUNGEON_AllocRoom(D2DrlgActStrc* pAct, D2RoomExStrc* pRoomEx, D2DrlgCoordsStrc* pDrlgCoords, D2DrlgRoomTilesStrc* pRoomTiles, int nLowSeed, uint32_t dwFlags)
{
	D2_ASSERT(pAct);

	D2RoomStrc* pRoom = D2_CALLOC_STRC_POOL(pAct->pMemPool, D2RoomStrc);

	pRoom->dwFlags = dwFlags;

	SEED_InitLowSeed(&pRoom->pSeed, nLowSeed);

	pRoom->pRoomEx = pRoomEx;
	memcpy(&pRoom->pCoords, pDrlgCoords, sizeof(D2DrlgCoordsStrc));
	pRoom->pRoomTiles = pRoomTiles;
	pRoom->pRoomNext = pAct->pRoom;
	pAct->pRoom = pRoom;
	pAct->unk0x20 = 1;
	pRoom->pAct = pAct;

	DRLGROOM_SetRoom(pRoomEx, pRoom);

	pRoom->ppRoomList = (D2RoomStrc**)D2_ALLOC_POOL(pAct->pMemPool, sizeof(D2RoomStrc*) * pRoom->pRoomEx->nRoomsNear);
	pRoom->nNumRooms = DRLGROOM_ReorderNearRoomList(pRoom->pRoomEx, pRoom->ppRoomList);

	for (int i = 0; i < pRoom->nNumRooms; ++i)
	{
		D2RoomStrc* pAdjacentRoom = pAdjacentRoom = pRoom->ppRoomList[i];
		D2_ASSERT(pAdjacentRoom);

		if (pAdjacentRoom != pRoom)
		{
			pAdjacentRoom->nNumRooms = DRLGROOM_ReorderNearRoomList(pAdjacentRoom->pRoomEx, pAdjacentRoom->ppRoomList);
		}
	}

	COLLISION_AllocRoomCollisionGrid(pAct->pMemPool, pRoom);

	if (pAct->pfnActCallBack)
	{
		pAct->pfnActCallBack(pRoom);
	}

	return pRoom;
}

//D2Common.0x6FD8BD90 (#10040)
BOOL __stdcall DUNGEON_DoRoomsTouchOrOverlap(D2RoomStrc* pRoom1, D2RoomStrc* pRoom2)
{
	D2_ASSERT(pRoom1);
	D2_ASSERT(pRoom2);

	if (pRoom1->nTileXPos <= pRoom2->nTileXPos + pRoom2->nTileWidth && pRoom1->nTileXPos + pRoom1->nTileWidth >= pRoom2->nTileXPos)
	{
		if (pRoom1->nTileYPos <= pRoom2->nTileYPos + pRoom2->nTileHeight && pRoom1->nTileYPos + pRoom1->nTileHeight >= pRoom2->nTileYPos)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD8BE30 (#10043)
BOOL __stdcall DUNGEON_AreTileCoordinatesInsideRoom(D2RoomStrc* pRoom, int nX, int nY)
{
	D2_ASSERT(pRoom);

	return nX >= pRoom->nTileXPos && nX < pRoom->nTileXPos + pRoom->nTileWidth && nY >= pRoom->nTileYPos && nY < pRoom->nTileYPos + pRoom->nTileHeight;
}

//D2Common.0x6FD8BE90 (#10048)
int __stdcall D2Common_10048(D2RoomStrc* pRoom, int nUnused)
{
	D2_MAYBE_UNUSED(nUnused);
	D2_ASSERT(pRoom);

	if (pRoom->nTileWidth + pRoom->nTileXPos >= pRoom->nTileXPos && pRoom->nTileYPos + pRoom->nTileHeight >= pRoom->nTileYPos)
	{
		if (pRoom->nTileXPos == pRoom->nTileWidth + pRoom->nTileXPos)
		{
			return 1;
		}
		else if (pRoom->nTileYPos == pRoom->nTileYPos + pRoom->nTileHeight)
		{
			return 3;
		}
		else
		{
			return 4;
		}
	}

	return 0;
}

//D2Commmon.0x6FD8BF00 (#10051)
D2RoomStrc* __stdcall DUNGEON_FindRoomByTileCoordinates(D2DrlgActStrc* pAct, int nX, int nY)
{
	for (D2RoomStrc* pRoom = pAct->pRoom; pRoom; pRoom = pRoom->pRoomNext)
	{
		if (nX >= pRoom->nTileXPos && nX < pRoom->nTileXPos + pRoom->nTileWidth && nY >= pRoom->nTileYPos && nY < pRoom->nTileYPos + pRoom->nTileHeight)
		{
			return pRoom;
		}
	}

	return NULL;
}

//D2Common.0x6FD8BF50 (#10050)
D2RoomStrc* __stdcall DUNGEON_GetAdjacentRoomByCoordinates(D2RoomStrc* pRoom, int nX, int nY)
{
	D2RoomStrc* pTemp = NULL;

	D2_ASSERT(pRoom);

	for (int i = 0; i < pRoom->nNumRooms; ++i)
	{
		pTemp = pRoom->ppRoomList[i];
		D2_ASSERT(pTemp);

		if (nX >= pTemp->nTileXPos && nX < pTemp->nTileXPos + pTemp->nTileWidth && nY >= pTemp->nTileYPos && nY < pTemp->nTileYPos + pTemp->nTileHeight)
		{
			return pTemp;
		}
	}

	return NULL;
}

//D2Common.0x6FD8BFF0 (#10049)
void __stdcall DUNGEON_CallRoomCallback(D2RoomStrc* pRoom, ROOMCALLBACKFN pfnRoomCallback, void* pArgs)
{
	D2_ASSERT(pRoom);

	D2_ASSERT(!IsBadCodePtr((FARPROC)pfnRoomCallback));

	for (int i = 0; i < pRoom->nNumRooms; ++i)
	{
		if (!pfnRoomCallback(pRoom->ppRoomList[i], pArgs))
		{
			break;
		}
	}
}

//D2Common.0x6FD8C080 (#10052)
void __stdcall D2Common_10052(D2RoomStrc* pRoom, RECT* pRect)
{
	int nTemp = 0;

	D2_ASSERT(pRoom);

	D2Common_10115(pRoom->nTileXPos, pRoom->nTileYPos, &nTemp, (int*)&pRect->top);
	D2Common_10115(pRoom->nTileXPos, pRoom->nTileHeight + pRoom->nTileYPos, (int*)&pRect->left, &nTemp);
	D2Common_10115(pRoom->nTileXPos + pRoom->nTileWidth, pRoom->nTileYPos, (int*)&pRect->right, &nTemp);
	D2Common_10115(pRoom->nTileXPos + pRoom->nTileWidth, pRoom->nTileHeight + pRoom->nTileYPos, &nTemp, (int*)&pRect->bottom);

	D2_ASSERT(pRect->top <= pRect->bottom);
	D2_ASSERT(pRect->left <= pRect->right);
}

//D2Common.0x6FD8C170 (#10053)
void __stdcall DUNGEON_GetSubtileRect(D2RoomStrc* pRoom, RECT* pRect)
{
	D2_ASSERT(pRoom);

	pRect->top = pRoom->nSubtileY;
	pRect->bottom = pRoom->nSubtileY + pRoom->nSubtileHeight;
	pRect->left = pRoom->nSubtileX;
	pRect->right = pRoom->nSubtileWidth + pRoom->nSubtileX;

	D2_ASSERT(pRect->top <= pRect->bottom);
	D2_ASSERT(pRect->left <= pRect->right);
}

//D2Common.0x6FD8C210 (#10054)
void __stdcall DUNGEON_GetRGB_IntensityFromRoom(D2RoomStrc* pRoom, uint8_t* pIntensity, uint8_t* pRed, uint8_t* pGreen, uint8_t* pBlue)
{
	return DRLGROOM_GetRGB_IntensityFromRoomEx(pRoom->pRoomEx, pIntensity, pRed, pGreen, pBlue);
}

//D2Common.0x6FD8C240 (#10041)
D2RoomStrc* __stdcall DUNGEON_FindRoomBySubtileCoordinates(D2DrlgActStrc* pAct, int nX, int nY)
{
	if (pAct)
	{
		for (D2RoomStrc* pRoom = pAct->pRoom; pRoom; pRoom = pRoom->pRoomNext)
		{
			if (DungeonTestRoomGame(pRoom, nX, nY))
			{
				return pRoom;
			}
		}
	}

	return NULL;
}

//D2Common.0x6FD8C290
BOOL __fastcall DUNGEON_AreSubtileCoordinatesInsideRoom(D2DrlgCoordsStrc* pDrlgCoords, int nX, int nY)
{
	D2_ASSERT(pDrlgCoords);

	return nX >= pDrlgCoords->dwSubtilesLeft && nX < pDrlgCoords->dwSubtilesLeft + pDrlgCoords->dwSubtilesWidth && nY >= pDrlgCoords->dwSubtilesTop && nY < pDrlgCoords->dwSubtilesTop + pDrlgCoords->dwSubtilesHeight;
}

//D2Common.0x6FD8C2F0 (#10046)
D2RoomStrc* __stdcall DUNGEON_FindActSpawnLocation(D2DrlgActStrc* pAct, int nLevelId, int nTileIndex, int* pX, int* pY)
{
	D2_ASSERT(pAct);

	return sub_6FD788D0(pAct->pDrlg, nLevelId, nTileIndex, pX, pY);
}

//D2Common.0x6FD8C340 (#10045)
D2RoomStrc* __stdcall DUNGEON_FindActSpawnLocationEx(D2DrlgActStrc* pAct, int nLevelId, int nTileIndex, int* pX, int* pY, int nUnitSize)
{
	D2RoomStrc* pNearRoom = NULL;
	D2RoomStrc* pRoom = NULL;
	D2CoordStrc pCoord = {};
	int nX = 0;
	int nY = 0;

	D2_ASSERT(pAct);

	pRoom = sub_6FD788D0(pAct->pDrlg, nLevelId, nTileIndex, &nX, &nY);
	if (pRoom)
	{
		DUNGEON_ExpandCoords(&nX, &nY);

		pCoord.nX = nX + 3;
		pCoord.nY = nY + 3;

		if (!COLLISION_GetFreeCoordinates(pRoom, &pCoord, nUnitSize, 0x1C09, 0))
		{
			FOG_DisplayAssert("Unable to find spawn location for act start", __FILE__, __LINE__);
			exit(-1);
		}

		*pX = pCoord.nX;
		*pY = pCoord.nY;

		if (DungeonTestRoomGame(pRoom, *pX, *pY))
		{
			return pRoom;
		}

		for (int i = 0; i < pRoom->nNumRooms; ++i)
		{
			pNearRoom = pRoom->ppRoomList[i];
			if (DungeonTestRoomGame(pNearRoom, *pX, *pY))
			{
				return pNearRoom;
			}
		}
	}

	return NULL;
}

//D2Common.0x6FD8C4A0 (#10029)
D2UnitStrc* __stdcall DUNGEON_GetFirstUnitInRoom(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	UNITROOM_SortUnitListByTargetY(pRoom);

	return pRoom->pUnitFirst;
}

//D2Common.0x6FD8C4E0 (#10100)
void __stdcall DUNGEON_IncreaseAlliedCountOfRoom(D2RoomStrc* pRoom)
{
	if (pRoom)
	{
		++pRoom->nAllies;
	}
}

//D2Comon.0x6FD8C4F0 (#10036)
int __stdcall DUNGEON_GetAlliedCountFromRoom(D2RoomStrc* pRoom)
{
	if (pRoom)
	{
		return pRoom->nAllies;
	}

	return 0;
}

//D2Common.0x6FD8C510 (#10101)
void __stdcall DUNGEON_DecreaseAlliedCountOfRoom(D2RoomStrc* pRoom)
{
	if (pRoom)
	{
		D2_ASSERT(pRoom->nAllies > 0);

		--pRoom->nAllies;
	}
}

//D2Common.0x6FD8C550
D2UnitStrc** __fastcall DUNGEON_GetUnitListFromRoom(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	return &pRoom->pUnitFirst;
}

//D2Common.0x6FD8C580
D2UnitStrc** __fastcall DUNGEON_GetUnitUpdateListFromRoom(D2RoomStrc* pRoom, BOOL bUpdate)
{
	D2_ASSERT(pRoom);

	if (bUpdate)
	{
		pRoom->pAct->bUpdate = TRUE;
	}

	return &pRoom->pUnitUpdate;
}

//D2Common.0x6FD8C5C0 (#10055)
D2PresetUnitStrc* __stdcall DUNGEON_GetPresetUnitsFromRoom(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	return DRLGROOM_GetPresetUnits(pRoom->pRoomEx);
}

//D2Common.0x6FD8C600
D2RoomCollisionGridStrc* __fastcall DUNGEON_GetCollisionGridFromRoom(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	return pRoom->pCollisionGrid;
}

//D2Common.0x6FD8C630
void __fastcall DUNGEON_SetCollisionGridInRoom(D2RoomStrc* pRoom, D2RoomCollisionGridStrc* pCollisionGrid)
{
	D2_ASSERT(pRoom);

	pRoom->pCollisionGrid = pCollisionGrid;
}

//D2Common.0x6FD8C660 (#10063)
void __stdcall DUNGEON_SetClientIsInSight(D2DrlgActStrc* pAct, int nLevelId, int nX, int nY, D2RoomStrc* pRoom)
{
	D2_ASSERT(pAct);

	DRLGACTIVATE_SetClientIsInSight(pAct->pDrlg, nLevelId, nX, nY, pRoom ? pRoom->pRoomEx : nullptr);
}

//D2Common.0x6FD8C6B0 (#10064)
void __stdcall DUNGEON_UnsetClientIsInSight(D2DrlgActStrc* pAct, int nLevelId, int nX, int nY, D2RoomStrc* pRoom)
{
	D2_ASSERT(pAct);

	DRLGACTIVATE_UnsetClientIsInSight(pAct->pDrlg, nLevelId, nX, nY, pRoom ? pRoom->pRoomEx : nullptr);
}

//D2Common.0x6FD8C700 (#10062)
void __stdcall DUNGEON_ChangeClientRoom(D2RoomStrc* pRoom1, D2RoomStrc* pRoom2)
{
	DRLGACTIVATE_ChangeClientRoom(pRoom1 ? pRoom1->pRoomEx : nullptr, pRoom2 ? pRoom2->pRoomEx : nullptr);
}

//D2Common.0x6FD8C730 (#10065)
D2RoomStrc* __stdcall DUNGEON_StreamRoomAtCoords(D2DrlgActStrc* pAct, int nX, int nY)
{
	D2_ASSERT(pAct);

	return DRLGACTIVATE_StreamRoomAtCoords(pAct->pDrlg, nX, nY);
}

//D2Common.0x6FD8C770 (#10056)
D2RoomStrc* __stdcall DUNGEON_GetRoomFromAct(D2DrlgActStrc* pAct)
{
	D2_ASSERT(pAct);

	return pAct->pRoom;
}

//D2Common.0x6FD8C7A0 (#10057)
int __stdcall DUNGEON_GetLevelIdFromRoom(D2RoomStrc* pRoom)
{
	if (pRoom)
	{
		return DRLGROOM_GetLevelId(pRoom->pRoomEx);
	}

	return 0;
}

//D2Common.0x6FD8C7C0 (#10058)
int __stdcall DUNGEON_GetWarpDestinationLevel(D2RoomStrc* pRoom, int nSourceLevel)
{
	if (pRoom)
	{
		return DRLGROOM_GetWarpDestinationLevel(pRoom->pRoomEx, nSourceLevel);
	}

	return 0;
}

//D2Common.0x6FD8C7E0 (#10059)
int __stdcall DUNGEON_GetLevelIdFromPopulatedRoom(D2RoomStrc* pRoom)
{
	if (pRoom)
	{
		return DRLGROOM_GetLevelIdFromPopulatedRoom(pRoom->pRoomEx);
	}

	return 0;
}

//D2Common.0x6FD8C800 (#10060)
BOOL __stdcall DUNGEON_HasWaypoint(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	return DRLGROOM_HasWaypoint(pRoom->pRoomEx);
}

//D2Common.0x6FD8C840 (#10061)
char* __stdcall DUNGEON_GetPickedLevelPrestFilePathFromRoom(D2RoomStrc* pRoom)
{
	if (pRoom)
	{
		return DRLGROOM_GetPickedLevelPrestFilePathFromRoomEx(pRoom->pRoomEx);
	}

	return NULL;
}

//D2Common.0x6FD8C860 (#10066)
void __stdcall DUNGEON_AllocDrlgDelete(D2RoomStrc* pRoom, int nUnitType, D2UnitGUID nUnitGUID)
{
	D2DrlgDeleteStrc* pDrlgDelete = NULL;

	if (pRoom)
	{
		pDrlgDelete = D2_ALLOC_STRC_POOL(pRoom->pAct->pMemPool, D2DrlgDeleteStrc);
		pDrlgDelete->nUnitType = nUnitType;
		pDrlgDelete->nUnitGUID = nUnitGUID;
		pDrlgDelete->pNext = pRoom->pDrlgDelete;
		pRoom->pDrlgDelete = pDrlgDelete;
		pRoom->pAct->bUpdateEx = TRUE;
	}
}

//D2Common.0x6FD8C8B0 (#10067)
void __stdcall DUNGEON_FreeDrlgDelete(D2RoomStrc* pRoom)
{
	D2DrlgDeleteStrc* pNext = NULL;

	D2_ASSERT(pRoom);

	for (D2DrlgDeleteStrc* pDrlgDelete = pRoom->pDrlgDelete; pDrlgDelete; pDrlgDelete = pNext)
	{
		pNext = pDrlgDelete->pNext;
		D2_FREE_POOL(pRoom->pAct->pMemPool, pDrlgDelete);
	}

	pRoom->pDrlgDelete = NULL;
}

//D2Common.0x6FD8C910 (#10068)
D2DrlgDeleteStrc* __stdcall DUNGEON_GetDrlgDeleteFromRoom(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	return pRoom->pDrlgDelete;
}

//D2Common.0x6FD8C940 (#10069)
D2RoomStrc* __stdcall DUNGEON_GetARoomInClientSight(D2DrlgActStrc* pAct)
{
	D2_ASSERT(pAct);

	return DRLGACTIVATE_GetARoomInClientSight(pAct->pDrlg);
}

//D2Common.0x6FD8C980 (#10070)
D2RoomStrc* __stdcall DUNGEON_GetARoomInSightButWithoutClient(D2DrlgActStrc* pAct, D2RoomStrc* pRoom)
{
	D2_ASSERT(pAct);

	D2_ASSERT(pRoom);

	return DRLGACTIVATE_GetARoomInSightButWithoutClient(pAct->pDrlg, pRoom->pRoomEx);
}

//D2Common.0x6FD8C9E0 (#10071)
BOOL __stdcall DUNGEON_TestRoomCanUnTile(D2DrlgActStrc* pAct, D2RoomStrc* pRoom)
{
	D2_ASSERT(pAct);
	D2_ASSERTM(!pAct->bClient, "Only servers should use DungeonTestRoomCanUnTile.");
	D2_ASSERT(pRoom);

	return DRLGACTIVATE_TestRoomCanUnTile(pRoom->pRoomEx);
}

//D2Common.0x6FD8CA60 (#10072)
bool __stdcall DUNGEON_GetRoomStatusFlags(D2RoomStrc* pRoom)
{
	return DRLGACTIVATE_GetRoomStatusFlags(pRoom->pRoomEx) >= 3;
}

//D2Common.0x6FD8CA80 (#10073)
BOOL __stdcall D2Common_10073(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	if (pRoom->nNumRooms == pRoom->pRoomEx->nRoomsNear)
	{
		for (int i = 0; i < pRoom->nNumRooms; ++i)
		{
			if (!(pRoom->ppRoomList[i]->dwFlags & 1))
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD8CAE0 (#10074)
BOOL __stdcall D2Common_10074(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	return pRoom->dwFlags & 2;
}

//D2Common.0x6FD8CB10 (#10075)
void __stdcall D2Common_10075(D2RoomStrc* pRoom, BOOL bSet)
{
	D2_ASSERT(pRoom);

	if (bSet)
	{
		pRoom->dwFlags |= 2;
	}
	else
	{
		pRoom->dwFlags &= 0xFFFFFFFD;
	}
}

//D2Common.0x6FD8CB60 (#10079)
void __stdcall DUNGEON_AddClientToRoom(D2RoomStrc* pRoom, D2ClientStrc* pClient)
{
	D2_ASSERT(pRoom);

	D2_ASSERT(pClient);

	for (int i = 0; i < pRoom->nNumClients; ++i)
	{
		D2_ASSERT(pRoom->ppClients[i]);
	}

	D2_ASSERT(pRoom->nNumClients <= pRoom->nMaxClients);

	if (pRoom->nNumClients == pRoom->nMaxClients)
	{
		pRoom->nMaxClients += 4;
		pRoom->ppClients = (D2ClientStrc**)D2_REALLOC_POOL(pRoom->pAct->pMemPool, pRoom->ppClients, sizeof(D2ClientStrc*) * pRoom->nMaxClients);
	}

	pRoom->ppClients[pRoom->nNumClients] = pClient;
	++pRoom->nNumClients;

	DUNGEON_UpdateClientListOfRoom(pRoom);
}

//D2Common.0x6FD8CC50
void __fastcall DUNGEON_UpdateClientListOfRoom(D2RoomStrc* pRoom)
{
	D2ClientStrc* pClient1 = NULL;
	D2ClientStrc* pClient2 = NULL;
	BOOL bContinue = FALSE;

	for (int i = 0; i < pRoom->nNumClients; ++i)
	{
		D2_ASSERT(pRoom->ppClients[i]);
	}

	for (int nClients = pRoom->nNumClients - 1; nClients > 0; --nClients)
	{
		bContinue = FALSE;

		for (int i = 0; i < nClients; ++i)
		{
			pClient1 = pRoom->ppClients[i];
			pClient2 = pRoom->ppClients[i + 1];

			if (pClient1 > pClient2)
			{
				pRoom->ppClients[i] = pClient2;
				pRoom->ppClients[i + 1] = pClient1;
				bContinue = TRUE;
			}
		}

		if (!bContinue)
		{
			return;
		}
	}

	for (int i = 0; i < pRoom->nNumClients; ++i)
	{
		D2_ASSERT(pRoom->ppClients[i]);
	}
}

//D2Common.0x6FD8CD10 (#10080)
void __stdcall DUNGEON_RemoveClientFromRoom(D2RoomStrc* pRoom, D2ClientStrc* pClient)
{
	int i = 0;

	D2_ASSERT(pRoom);

	D2_ASSERT(pClient);

	for (i = 0; i < pRoom->nNumClients; ++i)
	{
		D2_ASSERT(pRoom->ppClients[i]);
	}

	for (i = 0; i < pRoom->nNumClients; ++i)
	{
		if (pRoom->ppClients[i] == pClient)
		{
			break;
		}
	}

	D2_ASSERT(i < pRoom->nNumClients);

	--pRoom->nNumClients;
	pRoom->ppClients[i] = pRoom->ppClients[pRoom->nNumClients];

	DUNGEON_UpdateClientListOfRoom(pRoom);
}

//D2Common.0x6FD8CDF0 (#10081)
int __stdcall D2Common_10081_GetTileCountFromRoom(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	if (pRoom->nNumClients)
	{
		pRoom->nTileCount = 0;
		return 0;
	}
	else
	{
		++pRoom->nTileCount;
		return pRoom->nTileCount;
	}
}

//D2Common.0x6FD8CE40
void __fastcall DUNGEON_FreeRoom(void* pMemPool, D2RoomStrc* pRoom)
{
	for (D2UnitStrc* i = pRoom->pUnitFirst; i; i = pRoom->pUnitFirst)
	{
		if (!(i->dwFlags & UNITFLAG_ISCLIENTUNIT))
		{
			i->dwFlagEx |= 0x20;
		}

		i->dwFlags |= 0x800000;
		UNITROOM_UpdatePath(i);
	}

	COLLISION_FreeRoomCollisionGrid(pMemPool, pRoom);

	DUNGEON_FreeDrlgDelete(pRoom);

	if (pRoom->ppClients)
	{
		D2_FREE_POOL(pRoom->pAct->pMemPool, pRoom->ppClients);
	}

	D2_FREE_POOL(pMemPool, pRoom->ppRoomList);
	D2_FREE_POOL(pMemPool, pRoom);
}

//D2Common.0x6FD8CF10 (#10076)
void __stdcall DUNGEON_RemoveRoomFromAct(D2DrlgActStrc* pAct, D2RoomStrc* pRoom)
{
	D2RoomStrc* pPreviousRoom = NULL;
	D2RoomStrc* pCurrentRoom = NULL;
	D2RoomStrc* pNearRoom = NULL;
	int nCounter = 0;

	D2_ASSERT(pRoom);
	D2_ASSERT(pAct->pRoom);

	pCurrentRoom = pAct->pRoom;
	while (pCurrentRoom != pRoom)
	{
		pPreviousRoom = pCurrentRoom;
		pCurrentRoom = pCurrentRoom->pRoomNext;
		if (!pCurrentRoom)
		{
			break;
		}
	}

	D2_ASSERT(pCurrentRoom);

	if (pPreviousRoom)
	{
		pPreviousRoom->pRoomNext = pCurrentRoom->pRoomNext;
	}
	else
	{
		pAct->pRoom = pCurrentRoom->pRoomNext;
	}

	for (int i = 0; i < pRoom->nNumRooms; ++i)
	{
		pNearRoom = pRoom->ppRoomList[i];
		if (pNearRoom != pRoom)
		{
			nCounter = 0;

			while (nCounter < pNearRoom->nNumRooms && pNearRoom->ppRoomList[nCounter] != pRoom)
			{
				++nCounter;
			}

			if (nCounter < pNearRoom->nNumRooms)
			{
				--pNearRoom->nNumRooms;
				pNearRoom->ppRoomList[nCounter] = pNearRoom->ppRoomList[pNearRoom->nNumRooms];
			}
		}
	}

	sub_6FD77280(pRoom->pRoomEx, pAct->bClient, pRoom->dwFlags);
	DUNGEON_FreeRoom(pAct->pMemPool, pRoom);
}

//D2Common.0x6FD8D000 (#10077)
void __stdcall D2Common_10077(D2RoomStrc* pRoom1, D2RoomStrc* pRoom2)
{
	sub_6FD745C0(pRoom1 ? pRoom1->pRoomEx : NULL, pRoom2 ? pRoom2->pRoomEx : NULL);
}

//D2Common.0x6FD8D030 (#10078)
void __stdcall DUNGEON_UpdateAndFreeInactiveRooms(D2DrlgActStrc* pAct)
{
	DRLG_UpdateAndFreeInactiveRooms(pAct->pDrlg);
}

//D2Common.0x6FD8D040 (#10044)
int __stdcall DUNGEON_CheckLOSDraw(D2RoomStrc* pRoom)
{
	if (pRoom)
	{
		return DRLGROOM_CheckLOSDraw(pRoom->pRoomEx);
	}

	return 0;
}

//D2Common.0x6FD8D060
D2DrlgEnvironmentStrc* __fastcall DUNGEON_GetEnvironmentFromAct(D2DrlgActStrc* pAct)
{
	D2_ASSERT(pAct);

	return pAct->pEnvironment;
}

//D2Common.0x6FD8D090 (#10088)
D2DrlgStrc* __stdcall DUNGEON_GetDrlgFromAct(D2DrlgActStrc* pAct)
{
	D2_ASSERT(pAct);

	return pAct->pDrlg;
}

//D2Common.0x6FD912D0 (#10089)
int __stdcall DUNGEON_GetInitSeedFromAct(D2DrlgActStrc* pAct)
{
	if (pAct)
	{
		return pAct->dwInitSeed;
	}

	return 0;
}

//D2Common.0x6FD8D0C0 (#10007)
D2RoomExStrc* __fastcall DUNGEON_GetRoomExFromRoom(D2RoomStrc* pRoom)
{
	return pRoom->pRoomEx;
}

//D2Common.0x6FD8D0D0 (#10086)
BOOL __stdcall DUNGEON_IsTownLevelId(int nLevelId)
{
	return DRLG_IsTownLevel(nLevelId);
}

//D2Common.0x6FD8D0E0 (#10082)
BOOL __stdcall DUNGEON_IsRoomInTown(D2RoomStrc* pRoom)
{
	if (pRoom)
	{
		return DRLG_IsTownLevel(DRLGROOM_GetLevelId(pRoom->pRoomEx));
	}

	return FALSE;
}

//D2Common.0x6FD8D100 (#10083)
int __stdcall D2COMMON_10083_Return0(D2RoomStrc* pRoom)
{
	//if (pRoom)
	//{
	//	DRLGROOM_GetLevelId(pRoom->pRoomEx);
	//}

	//return D2COMMON_10018_Return0();

	return 0;
}

//D2Common.0x6FD8D130 (#10084)
int __stdcall D2Common_10084(D2RoomStrc* pRoom)
{
	return pRoom->dwFlags & 4;
}

//D2Common.0x6FD8D140 (#10085)
int __stdcall DUNGEON_GetTownLevelIdFromActNo(uint8_t nAct)
{
	static const int gnTownLevelIds[] = { LEVEL_ROGUEENCAMPMENT, LEVEL_LUTGHOLEIN, LEVEL_KURASTDOCKTOWN, LEVEL_THEPANDEMONIUMFORTRESS, LEVEL_HARROGATH };

	D2_ASSERT(nAct < ARRAY_SIZE(gnTownLevelIds));

	return gnTownLevelIds[nAct];
}

//D2Common.0x6FD8D180 (#10087)
int __stdcall D2Common_10087(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	return sub_6FD779F0(pRoom->pRoomEx);
}

//D2Common.0x6FD8D1C0 (#10090)
int __stdcall DUNGEON_GetNumberOfPopulatedRoomsInLevel(D2DrlgActStrc* pAct, int nLevelId)
{
	return DRLG_GetNumberOfPopulatedRoomsInLevel(pAct->pDrlg, nLevelId);
}

//D2Common.0x6FD8D1E0 (#10025)
int* __stdcall DUNGEON_GetWarpCoordinatesFromRoom(D2RoomStrc* pRoom)
{
	D2_ASSERT(pRoom);

	return DRLG_GetRoomCenterX_RoomWarpXFromRoom(pRoom->pRoomEx);
}

//D2Common.0x6FD8D220 (#10091)
void __stdcall DUNGEON_UpdateWarpRoomSelect(D2RoomStrc* pRoom, int nLevelId)
{
	D2_ASSERT(pRoom);

	DRLGWARP_UpdateWarpRoomSelect(pRoom->pRoomEx, nLevelId);
}

//D2Common.0x6FD8D260 (#10092)
void __stdcall DUNGEON_UpdateWarpRoomDeselect(D2RoomStrc* pRoom, int nLevelId)
{
	D2_ASSERT(pRoom);

	DRLGWARP_UpdateWarpRoomDeselect(pRoom->pRoomEx, nLevelId);
}

//D2Common.0x6FD8D2A0 (#10093)
void __stdcall DUNGEON_UpdatePops(D2RoomStrc* pRoom, int nX, int nY, BOOL bOtherRoom)
{
	D2_ASSERT(pRoom);

	DRLGPRESET_UpdatePops(pRoom->pRoomEx, nX, nY, bOtherRoom);
}

//D2Common.0x6FD8D2E0 (#10094)
void __stdcall DUNGEON_GetTombStoneTileCoords(D2RoomStrc* pRoom, D2CoordStrc** ppTombStoneTiles, int* pnTombStoneTiles)
{
	return DRLGPRESET_GetTombStoneTileCoords(pRoom->pRoomEx, ppTombStoneTiles, pnTombStoneTiles);
}

//D2Common.0x6FD8D300 (#10095)
int __stdcall D2Common_10095(D2RoomStrc* pRoom, int nX, int nY)
{
	D2RoomStrc* pNearRoom = NULL;

	if (!pRoom)
	{
		return 0;
	}

	if (DungeonTestRoomGame(pRoom, nX, nY))
	{
		return DRLGLOGIC_GetRoomCoordListIndex(pRoom->pRoomEx, nX, nY);
	}

	for (int i = 0; i < pRoom->nNumRooms; ++i)
	{
		pNearRoom = pRoom->ppRoomList[i];
		if (DungeonTestRoomGame(pNearRoom, nX, nY))
		{
			return DRLGLOGIC_GetRoomCoordListIndex(pNearRoom->pRoomEx, nX, nY);
		}
	}

	return 0;
}

//D2Common.0x6FD8D3A0 (#10096)
D2RoomCoordListStrc* __stdcall D2Common_10096(D2RoomStrc* pRoom, int nX, int nY)
{
	return sub_6FD77110(pRoom->pRoomEx, nX, nY);
}

//D2Common.0x6FD8D3C0 (#10097)
D2RoomCoordListStrc* __stdcall DUNGEON_GetRoomCoordList(D2RoomStrc* pRoom)
{
	return DRLGLOGIC_GetRoomCoordList(pRoom->pRoomEx);
}

//D2Common.0x6FD8D3D0 (#10098)
void __stdcall DUNGEON_GetPortalLevelArrayFromPortalFlags(void* pMemPool, int nFlags, int** ppLevels, int* pnLevels)
{
	int* pPortalLevels = NULL;
	int nPortalLevels = 0;
	int nCurrentFlag = 0;
	int pLevels[32] = {};

	nCurrentFlag = 1;

	pPortalLevels = DATATBLS_GetPortalLevels(&nPortalLevels);
	D2_ASSERT(nPortalLevels <= ARRAY_SIZE(pLevels));

	*ppLevels = NULL;
	*pnLevels = 0;

	for (int i = 0; i < nPortalLevels; ++i)
	{
		if (nFlags & nCurrentFlag)
		{
			pLevels[*pnLevels] = pPortalLevels[i];
			++*pnLevels;
		}

		nCurrentFlag <<= 1;
	}		
	
	if (*pnLevels)
	{
		*ppLevels = (int*)D2_ALLOC_POOL(pMemPool, sizeof(int) * *pnLevels);
		memcpy(*ppLevels, pLevels, sizeof(int) * *pnLevels);
	}
}

//D2Common.0x6FD8D4B0 (#10099)
int __stdcall DUNGEON_GetPortalFlagFromLevelId(int nPortalLevelId)
{
	int* pPortalLevels = NULL;
	int nPortalLevels = 0;

	pPortalLevels = DATATBLS_GetPortalLevels(&nPortalLevels);

	for (int i = 0; i < nPortalLevels; ++i)
	{
		if (nPortalLevelId == pPortalLevels[i])
		{
			return 1 << i;
		}
	}

	return 0;
}

//D2Common.0x6FD8D4F0 (#10037)
int __stdcall DUNGEON_GetTownLevelIdFromAct(D2DrlgActStrc* pAct)
{
	D2_ASSERT(pAct);

	return pAct->nTownId;
}

//D2Common.0x6FD8D520 (#10047)
int __stdcall DUNGEON_GetHoradricStaffTombLevelId(D2DrlgActStrc* pAct)
{
	if (pAct)
	{
		return DRLG_GetHoradricStaffTombLevelId(pAct->pDrlg);
	}

	return 0;
}

//D2Common.0x6FD8D540 (#10102)
void __stdcall DUNGEON_ToggleHasPortalFlag(D2RoomStrc* pRoom, BOOL bReset)
{
	if (pRoom)
	{
		DRLGACTIVATE_ToggleHasPortalFlag(pRoom->pRoomEx, bReset);
	}
}

//D2Common.0x6FD8D560 (#10104)
void __stdcall DUNGEON_AnimateTiles(D2RoomStrc* pRoom)
{
	if (pRoom)
	{
		DRLGANIM_AnimateTiles(pRoom->pRoomEx);
	}
}

//D2Common.0x6FD8D580 (#10105)
void __stdcall DUNGEON_InitRoomTileAnimation(D2DrlgActStrc* pAct, D2RoomStrc* pRoom1, D2RoomStrc* pRoom2)
{
	D2RoomExStrc* pRoomEx1 = NULL;
	D2RoomStrc* pRoom = NULL;

	if (pRoom1)
	{
		pRoom = pAct->pRoom;
		if (pRoom)
		{
			while (pRoom != pRoom1)
			{
				pRoom = pRoom->pRoomNext;
				if (!pRoom)
				{
					DRLGANIM_UpdateFrameInAdjacentRooms(NULL, pRoom2 ? pRoom2->pRoomEx : NULL);
					return;
				}
			}

			pRoomEx1 = pRoom1->pRoomEx;
		}
	}

	DRLGANIM_UpdateFrameInAdjacentRooms(pRoomEx1, pRoom2 ? pRoom2->pRoomEx : NULL);
}

//D2Common.0x6FD8D5C0 (#10103)
void __stdcall DUNGEON_SetActCallbackFunc(D2DrlgActStrc* pAct, ACTCALLBACKFN pActCallbackFunction)
{
	if (pAct)
	{
		pAct->pfnActCallBack = pActCallbackFunction;
	}
	else
	{
		FOG_DisplayWarning("ptAct", __FILE__, __LINE__);
	}
}

//D2Common.0x6FD8D600 (#10106)
void __stdcall DUNGEON_SaveKilledUnitGUID(D2RoomStrc* pRoom, D2UnitGUID nUnitGUID)
{
	if (pRoom)
	{
		pRoom->nLastDeadGUIDs[pRoom->nCurrentDeathIndex] = nUnitGUID;
		pRoom->nCurrentDeathIndex = (pRoom->nCurrentDeathIndex + 1) & 3;
	}
}

//D2Common.0x6FD8D630 (#10111)
void __stdcall DUNGEON_WorldToScreenCoords(int* pX, int* pY)
{
	int nTemp = 8 * (*pX + *pY);

	*pX = 16 * (*pX - *pY);
	*pY = nTemp;
}

//D2Common.0x6FD8D660 (#10112)
void __stdcall DUNGEON_FlattenCoords_IsoToCartesian(int* pX, int* pY)
{
	int nTemp = (*pX + *pY) >> 2;

	*pX = (*pX - *pY) >> 1;
	*pY = nTemp;
}

//D2Common.0x6FD8D690 (#10107)
void __stdcall D2Common_10107(int* pX, int* pY)
{
	int nX = 2 * *pY + *pX;
	int nY = 2 * *pY - *pX;

	*pX = nX / 160;
	*pY = nY / 160;
}

//D2Common.0x6FD8D6E0 (#10110)
void __stdcall DUNGEON_ExpandTileCoords(int* pX, int* pY)
{
	int nX = *pX;
	int nY = *pY;

	*pX = 80 * (nX - nY);
	*pY = 80 * (nX + nY) >> 1;
}

//D2Common.0x6FD8D710 (#10114)
void __stdcall D2Common_10114(int nX, int nY, int* pX, int* pY)
{
	*pX = 2 * nY + nX;
	*pY = 2 * nY - nX;

	if (*pX >= 0)
	{
		*pX /= 160;
	}
	else
	{
		*pX = *pX / 160 - 1;
	}

	if (*pY >= 0)
	{
		*pY /= 160;
	}
	else
	{
		*pY = *pY / 160 - 1;
	}
}

//D2Common.0x6FD8D790 (#10115)
void __stdcall D2Common_10115(int nX, int nY, int* pX, int* pY)
{
	*pX = 80 * (nX - nY);
	*pY = 40 * (nX + nY);
	*pX -= 80;
	*pY += 80;
}

//D2Common.0x6FD8D7D0 (#10116)
void __stdcall D2Common_10116(int nX, int nY, int* pX, int* pY)
{
	*pX = 2 * nY + nX;
	*pY = 2 * nY - nX;

	if (*pX >= 0)
	{
		*pX >>= 5;
	}
	else
	{
		*pX = (*pX >> 5) - 1;
	}

	if (*pY >= 0)
	{
		*pY >>= 5;
	}
	else
	{
		*pY = (*pY >> 5) - 1;
	}
}

//D2Common.0x6FD8D830 (#10117)
void __stdcall D2Common_10117(int nX, int nY, int* pX, int* pY)
{
	*pX = 16 * (nX - nY);
	*pY = 8 * (nX + nY);
	*pX -= 16;
	*pY += 16;
}

//D2Common.0x6FD8D870 (#10108)
void __stdcall DUNGEON_ScreenToWorldCoords(int* pX, int* pY)
{
	int nTemp = (2 * *pY - *pX) >> 5;

	*pX = (2 * *pY + *pX) >> 5;
	*pY = nTemp;
}

//D2Common.0x6FD8D8A0 (#10109)
void __stdcall D2Common_10109(int* pX, int* pY)
{
	int nX = 2 * *pY + *pX;
	int nY = 2 * *pY - *pX;

	*pX = nX;
	*pY = nY;
}

//D2Common.0x6FD8D8C0 (#10113)
void __stdcall DUNGEON_ExpandCoords(int* pX, int* pY)
{
	*pX *= 5;
	*pY *= 5;
}
