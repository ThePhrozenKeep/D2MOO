#include "Drlg/D2DrlgActivate.h"

#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "Drlg/D2DrlgPreset.h"
#include "Drlg/D2DrlgRoomTile.h"
#include <DataTbls/LevelsIds.h>

static int gStatsClientFreedRooms;
static int gStatsClientAllocatedRooms;
static int gStatsFreedRooms;
static int gStatsAllocatedRooms;

// D2Common.0x6FDE07B0
static void (__fastcall* gRoomExSetStatus[ROOMSTATUS_COUNT])(D2RoomExStrc*) =
{
	DRLGACTIVATE_RoomExSetStatus_ClientInRoom,
	DRLGACTIVATE_RoomExSetStatus_ClientInSight,
	DRLGACTIVATE_RoomExSetStatus_ClientOutOfSight,
	DRLGACTIVATE_RoomExSetStatus_Untile
};

// D2Common.0x6FDE07C0
static void (__fastcall* gRoomExUnsetStatus[ROOMSTATUS_COUNT])(D2RoomExStrc*) =
{
	DRLGACTIVATE_RoomExIdentifyRealStatus,
	DRLGACTIVATE_RoomExIdentifyRealStatus,
	DRLGACTIVATE_RoomExIdentifyRealStatus,
	DRLGACTIVATE_RoomExStatusUnset_Untile
};

//Helper functions
static void DRLGACTIVATE_RoomExStatuslink(D2RoomExStrc* pStatusRoomsListHead, D2RoomExStrc* pRoomEx)
{
	pRoomEx->pStatusNext = pStatusRoomsListHead;
	pRoomEx->pStatusPrev = pStatusRoomsListHead->pStatusPrev;

	pStatusRoomsListHead->pStatusPrev->pStatusNext = pRoomEx;
	pStatusRoomsListHead->pStatusPrev = pRoomEx;
}
static void DRLGACTIVATE_RoomExStatusUnlink(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->pStatusPrev && pRoomEx->pStatusNext)
	{
		pRoomEx->pStatusPrev->pStatusNext = pRoomEx->pStatusNext;
		pRoomEx->pStatusNext->pStatusPrev = pRoomEx->pStatusPrev;
		pRoomEx->pStatusPrev = nullptr;
		pRoomEx->pStatusNext = nullptr;
	}
}
// Returns true if status changed
static bool DRLGACTIVATE_UpdateRoomExStatusImpl(D2RoomExStrc* pRoomEx, D2DrlgRoomStatus nStatus)
{
	// Note: Lower value has priority over others
	if (pRoomEx->fRoomStatus > nStatus)
	{
		DRLGACTIVATE_RoomExStatusUnlink(pRoomEx);
		if (nStatus < ROOMSTATUS_COUNT)
		{
			DRLGACTIVATE_RoomExStatuslink(&pRoomEx->pLevel->pDrlg->tStatusRoomsLists[nStatus], pRoomEx);
		}
		pRoomEx->fRoomStatus = nStatus;
		return true;
	}
	return false;
}
static D2DrlgRoomStatus DRLGACTIVATE_RoomExFindFirstStatusWithRefCount(D2RoomExStrc* pRoomEx, D2DrlgRoomStatus nMaxStatus)
{
	for (int nFirstNonEmptyListStatus = 0; nFirstNonEmptyListStatus <= nMaxStatus && nFirstNonEmptyListStatus < ROOMSTATUS_COUNT; nFirstNonEmptyListStatus++)
	{
		if (pRoomEx->wRoomsInList[nFirstNonEmptyListStatus] != 0)
		{
			return (D2DrlgRoomStatus)nFirstNonEmptyListStatus;
		}
	}
	return nMaxStatus;
}

//D2Common.0x6FD733D0
void __fastcall DRLGACTIVATE_RoomExSetStatus_ClientInRoom(D2RoomExStrc* pRoomEx)
{
	DRLGACTIVATE_UpdateRoomExStatusImpl(pRoomEx, ROOMSTATUS_CLIENT_IN_ROOM);
}

//Helper function
static void DRLGACTIVATE_InitRoomsInitTimeout(D2DrlgStrc* pDrlg)
{
	pDrlg->nRoomsInitTimeout = 2 * (DRLG_IsOnClient(pDrlg) == 0) + 5;
}
void DRLGACTIVATE_RoomEx_EnsureHasRoom(D2RoomExStrc* pRoomEx, bool bInitTimeoutCounter)
{
	if (pRoomEx->pRoom == nullptr && !(pRoomEx->dwFlags & ROOMEXFLAG_HAS_ROOM))
	{
		D2DrlgStrc* pDrlg = pRoomEx->pLevel->pDrlg;
		if (!pRoomEx->nRoomsNear)
		{
			sub_6FD77BB0(pDrlg->pMempool, pRoomEx);
		}

		DRLGROOMTILE_InitRoomGrids(pRoomEx);
		DRLGROOMTILE_AddRoomMapTiles(pRoomEx);
		DRLG_CreateRoomForRoomEx(pDrlg, pRoomEx);
		++pDrlg->nRoomsInitSinceLastUpdate;
		++pDrlg->nAllocatedRooms;
		if (bInitTimeoutCounter)
		{
			DRLGACTIVATE_InitRoomsInitTimeout(pDrlg);
		}
	}
}

//D2Common.0x6FD73450
void __fastcall DRLGACTIVATE_RoomExSetStatus_ClientInSight(D2RoomExStrc* pRoomEx)
{
	DRLGACTIVATE_RoomEx_EnsureHasRoom(pRoomEx, true);
	DRLGACTIVATE_UpdateRoomExStatusImpl(pRoomEx, ROOMSTATUS_CLIENT_IN_SIGHT);
}

//D2Common.0x6FD73550
void __fastcall DRLGACTIVATE_RoomExSetStatus_ClientOutOfSight(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->dwFlags & ROOMEXFLAG_TILELIB_LOADED 
		&& (pRoomEx->nType != DRLGTYPE_PRESET || (pRoomEx->dwFlags & ROOMEXFLAG_PRESET_UNITS_ADDED) != 0))
	{

		if (DRLGACTIVATE_UpdateRoomExStatusImpl(pRoomEx, ROOMSTATUS_CLIENT_OUT_OF_SIGHT)
			&& pRoomEx->wRoomsInList[ROOMSTATUS_CLIENT_IN_SIGHT] != 0)
		{
			DRLGACTIVATE_RoomExSetStatus_ClientInSight(pRoomEx);
		}
	}
}

//D2Common.0x6FD736F0
void __fastcall DRLGACTIVATE_RoomExSetStatus_Untile(D2RoomExStrc* pRoomEx)
{
	if (!(pRoomEx->dwFlags & ROOMEXFLAG_TILELIB_LOADED))
	{
		DRLGROOMTILE_LoadDT1FilesForRoom(pRoomEx);
	}

	if (pRoomEx->nType == DRLGTYPE_PRESET && !(pRoomEx->dwFlags & ROOMEXFLAG_PRESET_UNITS_ADDED))
	{
		DRLGPRESET_SpawnHardcodedPresetUnits(pRoomEx);
	}
	DRLGACTIVATE_UpdateRoomExStatusImpl(pRoomEx, ROOMSTATUS_UNTILE);
}

//D2Common.0x6FD73790
void __fastcall DRLGACTIVATE_RoomExIdentifyRealStatus(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->fRoomStatus >= ROOMSTATUS_COUNT || pRoomEx->wRoomsInList[pRoomEx->fRoomStatus] == 0)
	{
		const D2DrlgRoomStatus nFirstStatusWithRefCount = DRLGACTIVATE_RoomExFindFirstStatusWithRefCount(pRoomEx, ROOMSTATUS_COUNT);
		if (pRoomEx->fRoomStatus != nFirstStatusWithRefCount)
		{
			DRLGACTIVATE_UpdateRoomExStatusImpl(pRoomEx, nFirstStatusWithRefCount);
		}
	}
}

//D2Common.0x6FD73880
void __fastcall DRLGACTIVATE_RoomExStatusUnset_Untile(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->fRoomStatus != ROOMSTATUS_COUNT)
	{
		DRLGACTIVATE_RoomExIdentifyRealStatus(pRoomEx);
		
		// We may unload the room if no status is now set
		if (pRoomEx->fRoomStatus == ROOMSTATUS_COUNT)
		{
			if (DRLG_IsOnClient(pRoomEx->pLevel->pDrlg))
			{
				sub_6FD8A2E0(pRoomEx, false);
			}
		}
	}
}

//D2Common.0x6FD73A30
void __fastcall DRLGACTIVATE_RoomExPropagateSetStatus(void* pMemPool, D2RoomExStrc* pRoomEx, uint8_t nStatus)
{
	if (pRoomEx->nRoomsNear == 0)
	{
		sub_6FD77BB0(pMemPool, pRoomEx);
	}

	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		D2RoomExStrc* pNearRoom = pRoomEx->ppRoomsNear[i];
		if (nStatus + 1 < ROOMSTATUS_COUNT)
		{
			DRLGACTIVATE_RoomExPropagateSetStatus(pMemPool, pNearRoom, nStatus + 1);
		}

		if (pNearRoom->fRoomStatus >= nStatus)
		{
			const D2DrlgRoomStatus nFirstStatusWithRefCount = DRLGACTIVATE_RoomExFindFirstStatusWithRefCount(pNearRoom, D2DrlgRoomStatus(nStatus));
			if (nFirstStatusWithRefCount == nStatus)
			{
				gRoomExSetStatus[nStatus](pNearRoom);
			}
		}
		++pNearRoom->wRoomsInList[nStatus];
	}
}

//D2Common.0x6FD73BE0
void __fastcall DRLGACTIVATE_RoomExPropagateUnsetStatus(D2RoomExStrc* pRoomEx, uint8_t nStatus)
{
	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		if (pRoomEx->ppRoomsNear[i])
		{
			--pRoomEx->ppRoomsNear[i]->wRoomsInList[nStatus];

			gRoomExUnsetStatus[nStatus](pRoomEx->ppRoomsNear[i]);

			if (nStatus + 1 < ROOMSTATUS_COUNT)
			{
				DRLGACTIVATE_RoomExPropagateUnsetStatus(pRoomEx->ppRoomsNear[i], nStatus + 1);
			}
		}
	}
}

// Helper functions
static void DRLGACTIVATE_RoomSetAndPropagateStatus(D2RoomExStrc* pRoomEx, D2DrlgRoomStatus nStatus)
{
	DRLGACTIVATE_RoomExPropagateSetStatus(pRoomEx->pLevel->pDrlg->pMempool, pRoomEx, nStatus + 1);
	if (pRoomEx->fRoomStatus >= nStatus)
	{
		const D2DrlgRoomStatus nFirstStatusWithRefCount = DRLGACTIVATE_RoomExFindFirstStatusWithRefCount(pRoomEx, nStatus);
		if (nFirstStatusWithRefCount == nStatus)
		{
			gRoomExSetStatus[nStatus](pRoomEx);
		}
	}
	++pRoomEx->wRoomsInList[nStatus];
}

static void DRLGACTIVATE_RoomUnsetAndPropagateStatus(D2RoomExStrc* pRoomEx, D2DrlgRoomStatus nStatus)
{
	if (pRoomEx->wRoomsInList[nStatus] != 0)
	{
		--pRoomEx->wRoomsInList[nStatus];

		gRoomExUnsetStatus[nStatus](pRoomEx);

		for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
		{
			if (D2RoomExStrc * pNearRoom = pRoomEx->ppRoomsNear[i])
			{
				--pNearRoom->wRoomsInList[nStatus + 1];
				gRoomExUnsetStatus[nStatus + 1](pNearRoom);
				DRLGACTIVATE_RoomExPropagateUnsetStatus(pNearRoom, nStatus + 2);
			}
		}
	}
}

//D2Common.0x6FD739A0
void __fastcall DRLGACTIVATE_SetClientIsInSight(D2DrlgStrc* pDrlg, int nLevelId, int nX, int nY, D2RoomExStrc* pRoomExHint)
{
	D2DrlgLevelStrc* pLevel = DRLG_GetLevel(pDrlg, nLevelId);
	if (pRoomExHint && pRoomExHint->pLevel->nLevelId != nLevelId)
	{
		pRoomExHint = nullptr;
	}
	D2RoomExStrc* pRoomEx = DRLG_GetRoomExFromCoordinates(nX, nY, pDrlg, pRoomExHint, pLevel);

	if (pRoomEx->wRoomsInList[ROOMSTATUS_CLIENT_IN_SIGHT] == 0)
	{
		DRLGACTIVATE_RoomSetAndPropagateStatus(pRoomEx, ROOMSTATUS_CLIENT_IN_SIGHT);
	}
}

//D2Common.0x6FD73B40
void __fastcall DRLGACTIVATE_UnsetClientIsInSight(D2DrlgStrc* pDrlg, int nLevelId, int nX, int nY, D2RoomExStrc* pRoomExHint)
{
	D2DrlgLevelStrc* pLevel = DRLG_GetLevel(pDrlg, nLevelId);
	if (pRoomExHint && pRoomExHint->pLevel->nLevelId != nLevelId)
	{
		pRoomExHint = nullptr;
	}

	if (D2RoomExStrc* pRoomEx = DRLG_GetRoomExFromCoordinates(nX, nY, pDrlg, pRoomExHint, pLevel))
	{
		DRLGACTIVATE_RoomUnsetAndPropagateStatus(pRoomEx, ROOMSTATUS_CLIENT_IN_SIGHT);
	}
}


//D2Common.0x6FD73C40
void __fastcall DRLGACTIVATE_ChangeClientRoom(D2RoomExStrc* pPreviousRoom, D2RoomExStrc* pNewRoom)
{
	if (pPreviousRoom == pNewRoom)
	{
		return;
	}

	if (pNewRoom)
	{
		DRLGACTIVATE_RoomSetAndPropagateStatus(pNewRoom, ROOMSTATUS_CLIENT_IN_ROOM);
	}

	if (pPreviousRoom)
	{
		DRLGACTIVATE_RoomUnsetAndPropagateStatus(pPreviousRoom, ROOMSTATUS_CLIENT_IN_ROOM);
	}
}

//D2Common.0x6FD73CF0
void __fastcall DRLGACTIVATE_InitializeRoomEx(D2RoomExStrc* pRoomEx)
{
	if (!(pRoomEx->dwFlags & ROOMEXFLAG_TILELIB_LOADED))
	{
		DRLGROOMTILE_LoadDT1FilesForRoom(pRoomEx);
	}

	if (!(pRoomEx->dwFlags & ROOMEXFLAG_PRESET_UNITS_ADDED) && (pRoomEx->nType == DRLGTYPE_PRESET))
	{
		DRLGPRESET_SpawnHardcodedPresetUnits(pRoomEx);
	}

	DRLGACTIVATE_RoomEx_EnsureHasRoom(pRoomEx, false);
}

//D2Common.0x6FD73D80
D2RoomStrc* __fastcall DRLGACTIVATE_StreamRoomAtCoords(D2DrlgStrc* pDrlg, int nX, int nY)
{
	if (D2RoomExStrc* pRoomEx = DRLG_GetRoomExFromCoordinates(nX, nY, pDrlg, 0, 0))
	{
		DRLGACTIVATE_InitializeRoomEx(pRoomEx);
		return pRoomEx->pRoom;
	}
	return nullptr;
}

//D2Common.0x6FD73E30
void __fastcall DRLGACTIVATE_InitializeRoomExStatusLists(D2DrlgStrc* pDrlg)
{
	for (int nStatus = 0; nStatus < ROOMSTATUS_COUNT; ++nStatus)
	{
		D2RoomExStrc* pRoomExStatusList = &pDrlg->tStatusRoomsLists[nStatus];
		pRoomExStatusList->fRoomStatus = nStatus;
		pRoomExStatusList->pStatusNext = pRoomExStatusList;
		pRoomExStatusList->pStatusPrev = pRoomExStatusList;
	}
}

//Helper function
static D2RoomExStrc* DRLGACTIVATE_RoomExStatusList_GetFirst(D2DrlgStrc* pDrlg, D2DrlgRoomStatus nStatus)
{
	const D2RoomExStrc* pStatusListHead = &pDrlg->tStatusRoomsLists[nStatus];
	if (pStatusListHead != pStatusListHead->pStatusNext) // If not empty
	{
		return pStatusListHead->pStatusNext;
	}
	return nullptr;
}

//D2Common.0x6FD73E60
D2RoomStrc* __fastcall DRLGACTIVATE_GetARoomInClientSight(D2DrlgStrc* pDrlg)
{
	if (D2RoomExStrc* pRoomEx = DRLGACTIVATE_RoomExStatusList_GetFirst(pDrlg, ROOMSTATUS_CLIENT_IN_ROOM))
	{
		return pRoomEx->pRoom;
	}
	if (D2RoomExStrc* pRoomEx = DRLGACTIVATE_RoomExStatusList_GetFirst(pDrlg, ROOMSTATUS_CLIENT_IN_SIGHT))
	{
		return pRoomEx->pRoom;
	}
	return nullptr;
}

//D2Common.0x6FD73E90
D2RoomStrc* __fastcall DRLGACTIVATE_GetARoomInSightButWithoutClient(D2DrlgStrc* pDrlg, D2RoomExStrc* pRoomEx)
{
	if (D2RoomExStrc* pNextStatusRoom = pRoomEx->pStatusNext)
	{
		if (pRoomEx->fRoomStatus != ROOMSTATUS_CLIENT_IN_ROOM || pNextStatusRoom != &pDrlg->tStatusRoomsLists[ROOMSTATUS_CLIENT_IN_ROOM])
		{
			return pNextStatusRoom->pRoom;
		}

		if (D2RoomExStrc* pFirstRoomInSight = DRLGACTIVATE_RoomExStatusList_GetFirst(pDrlg, ROOMSTATUS_CLIENT_IN_SIGHT))
		{
			return pFirstRoomInSight->pRoom;
		}
	}

	return nullptr;
}

//D2Common.0x6FD73EF0 (#10015)
void __fastcall DRLGACTIVATE_GetRoomsAllocationStats(int* pOutStatsClientAllocatedRooms, int* pOutStatsClientFreedRooms, int* pOutStatsAllocatedRooms, int* pOutStatsFreedRooms)
{
	*pOutStatsClientAllocatedRooms = gStatsClientAllocatedRooms;
	*pOutStatsClientFreedRooms = gStatsClientFreedRooms;
	*pOutStatsAllocatedRooms = gStatsAllocatedRooms;
	*pOutStatsFreedRooms = gStatsFreedRooms;
}

//D2Common.0x6FD73F20 (#10003)
void __stdcall DRLGACTIVATE_Update(D2DrlgStrc* pDrlg)
{
	if (DRLG_IsOnClient(pDrlg))
	{
		gStatsClientAllocatedRooms = pDrlg->nAllocatedRooms;
		gStatsClientFreedRooms = pDrlg->nFreedRooms;
	}
	else
	{
		gStatsAllocatedRooms = pDrlg->nAllocatedRooms;
		gStatsFreedRooms = pDrlg->nFreedRooms;
	}

	if (pDrlg->nRoomsInitSinceLastUpdate > 1)
	{
		pDrlg->nRoomsInitSinceLastUpdate = 0;
		return;
	}

	--pDrlg->nRoomsInitTimeout;

	if (pDrlg->nRoomsInitTimeout == 0)
	{
		DRLGACTIVATE_InitRoomsInitTimeout(pDrlg);

		if (!pDrlg->pRoomEx || pDrlg->pRoomEx->fRoomStatus != ROOMSTATUS_CLIENT_OUT_OF_SIGHT)
		{
			pDrlg->pRoomEx = pDrlg->tStatusRoomsLists[ROOMSTATUS_CLIENT_OUT_OF_SIGHT].pStatusNext;
		}


		if (D2RoomExStrc* pDrlgRoomExListHead = pDrlg->pRoomEx)
		{
			D2RoomExStrc* pCurRoomEx;
			for (pCurRoomEx = pDrlgRoomExListHead ; pCurRoomEx != pDrlgRoomExListHead ; pCurRoomEx = pCurRoomEx->pStatusNext)
			{
				if (pCurRoomEx != &pDrlg->tStatusRoomsLists[ROOMSTATUS_CLIENT_OUT_OF_SIGHT])
				{
					DRLGACTIVATE_RoomEx_EnsureHasRoom(pCurRoomEx, false);
				}
				if (pDrlg->nRoomsInitSinceLastUpdate != 0)
				{
					break;
				}
			}

			pDrlg->pRoomEx = pCurRoomEx;
			pDrlg->nRoomsInitSinceLastUpdate = 0;
		}
	}
}

//D2Common.0x6FD74060
BOOL __fastcall DRLGACTIVATE_TestRoomCanUnTile(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->dwFlags & ROOMEXFLAG_HASPORTAL)
	{
		return FALSE;
	}

	D2_ASSERT(!DRLG_IsOnClient(DRLGROOM_GetDrlgFromRoomEx(pRoomEx)));

	if (pRoomEx->fRoomStatus <= 1)
	{
		return FALSE;
	}

	if (DRLG_IsTownLevel(pRoomEx->pLevel->nLevelId) || pRoomEx->pLevel->nLevelId == LEVEL_ROCKYSUMMIT)
	{
		for (D2RoomExStrc* pCurrentRoomEx = pRoomEx->pLevel->pFirstRoomEx; pCurrentRoomEx; pCurrentRoomEx = pCurrentRoomEx->pRoomExNext)
		{
			if (pCurrentRoomEx->fRoomStatus <= 1)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//D2Common.0x6FD740F0
void __fastcall DRLGACTIVATE_ToggleHasPortalFlag(D2RoomExStrc* pRoomEx, BOOL bReset)
{
	if (bReset)
	{
		pRoomEx->dwFlags &= ~ROOMEXFLAG_HASPORTAL;
	}
	else
	{
		pRoomEx->dwFlags |= ROOMEXFLAG_HASPORTAL;
	}
}

//D2Common.0x6FD74110
uint8_t __fastcall DRLGACTIVATE_GetRoomStatusFlags(D2RoomExStrc* pRoomEx)
{
	return pRoomEx->fRoomStatus;
}
