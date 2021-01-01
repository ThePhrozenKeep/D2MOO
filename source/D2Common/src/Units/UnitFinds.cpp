#include "Units/UnitFinds.h"

#include "D2Collision.h"
#include "D2DataTbls.h"
#include "D2Dungeon.h"
#include "D2Monsters.h"
#include "Units/Units.h"
#include <D2Math.h>


//D2Common.0x6FDBC680 (#10408)
BOOL __stdcall UNITFINDS_AreUnitsInNeighboredRooms(D2UnitStrc* pDestUnit, D2UnitStrc* pSrcUnit)
{
	D2RoomStrc** ppRoomList = NULL;
	D2RoomStrc* pDestRoom = NULL;
	D2RoomStrc* pSrcRoom = NULL;
	int nNumRooms = 0;

	D2_ASSERT(pDestUnit);
	D2_ASSERT(pSrcUnit);

	pSrcRoom = UNITS_GetRoom(pSrcUnit);
	pDestRoom = UNITS_GetRoom(pDestUnit);

	if (pSrcRoom && pDestRoom)
	{
		DUNGEON_GetAdjacentRoomsListFromRoom(pSrcRoom, &ppRoomList, &nNumRooms);

		for (int i = 0; i < nNumRooms; ++i)
		{
			if (ppRoomList[i] == pDestRoom)
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

//D2Common.0x6FDBC720 (#11087)
D2UnitStrc* __stdcall UNITFINDS_FindUnitInNeighboredRooms(D2RoomStrc* pRoom, int nUnitType, int nClassId)
{
	D2RoomStrc** ppRoomList = NULL;
	D2UnitStrc* pUnit = NULL;
	int nNumRooms = 0;

	D2_ASSERT(pRoom);

	DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

	for (int i = 0; i < nNumRooms; ++i)
	{
		pUnit = ppRoomList[i]->pUnitFirst;

		while (pUnit)
		{
			if (pUnit->dwUnitType == nUnitType && pUnit->dwClassId == nClassId)
			{
				return pUnit;
			}

			pUnit = pUnit->pRoomNext;
		}
	}

	return NULL;
}

//D2Common.0x6FDBC7B0 (#10405)
int __stdcall UNITFINDS_GetTestedUnitsFromRoom(D2RoomStrc* pRoom, D2UnitStrc** ppUnits, UNITFINDTEST pfnUnitTest, D2UnitFindArgStrc* pUnitFindArg)
{	
	int nUnitIndex = 0;

	D2_ASSERT(!IsBadCodePtr((FARPROC)pfnUnitTest));

	for (D2UnitStrc* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pRoomNext)
	{
		if (pfnUnitTest(pUnit, pUnitFindArg))
		{
			ppUnits[nUnitIndex] = pUnit;
			++nUnitIndex;

			D2_ASSERT(nUnitIndex < UNIT_FIND_ARRAY_SIZE);
		}
	}

	return nUnitIndex;
}

//D2Common.0x6FDBC840 (#11088)
D2UnitStrc* __stdcall UNITFINDS_GetNearestTestedUnit(D2UnitStrc* pUnit, int nX, int nY, int nSize, int(__fastcall* pfnUnitTest)(D2UnitStrc*, D2UnitStrc*))
{
	D2RoomStrc** ppRoomList = NULL;
	D2RoomStrc* pRoom = NULL;
	D2UnitStrc* pResult = NULL;
	int nSmallestDistance = 0;
	int nDistance = 0;
	int nNumRooms = 0;
	D2DrlgCoordsStrc pRoomCoord = {};

	pRoom = UNITS_GetRoom(pUnit);
	D2_ASSERT(pRoom);

	DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

	if (!nSize)
	{
		nSize = 65536;
	}

	D2_ASSERT(!IsBadCodePtr((FARPROC)pfnUnitTest));

	nSmallestDistance = 65535;

	for (int i = 0; i < nNumRooms; ++i)
	{
		DUNGEON_GetRoomCoordinates(ppRoomList[i], &pRoomCoord);

		if ((nX + nSize >= pRoomCoord.dwSubtilesLeft || nX - nSize <= pRoomCoord.dwSubtilesLeft + pRoomCoord.dwSubtilesWidth)
			&& (nSize + nY >= pRoomCoord.dwSubtilesTop || nY - nSize <= pRoomCoord.dwSubtilesTop + pRoomCoord.dwSubtilesHeight))
		{
			for (D2UnitStrc* j = ppRoomList[i]->pUnitFirst; j != NULL; j = j->pRoomNext)
			{
				nDistance = UNITS_GetDistanceToCoordinates(j, nX, nY);
				if (nDistance < nSize && nDistance < nSmallestDistance && pfnUnitTest(j, pUnit))
				{
					pResult = j;
					nSmallestDistance = nDistance;
				}
			}
		}
	}

	return pResult;
}

//D2Common.0x6FDBC990 (#10401)
void __stdcall UNITFINDS_InitializeUnitFindData(void* pMemPool, D2UnitFindDataStrc* pUnitFindData, D2RoomStrc* pRoom, int nX, int nY, int nSize, UNITFINDTEST pfnUnitTest, D2UnitFindArgStrc* pUnitFindArg)
{
	D2_ASSERT(pUnitFindData);

	pUnitFindData->pUnitsArray = (D2UnitStrc**)FOG_AllocServerMemory(pMemPool, sizeof(D2UnitStrc*) * UNIT_FIND_ARRAY_SIZE, __FILE__, __LINE__, 0);
	D2_ASSERT(pUnitFindData->pUnitsArray);
	memset(pUnitFindData->pUnitsArray, 0x00, sizeof(D2UnitStrc*) * UNIT_FIND_ARRAY_SIZE);

	pUnitFindData->nIndex = 0;
	pUnitFindData->nFlags = 0;
	pUnitFindData->pRoom = pRoom;
	pUnitFindData->nX = nX;
	pUnitFindData->nY = nY;
	pUnitFindData->pMemPool = pMemPool;
	pUnitFindData->nMaxArrayEntries = UNIT_FIND_ARRAY_SIZE;
	pUnitFindData->nSize = nSize;
	pUnitFindData->pfnUnitTest = pfnUnitTest;
	pUnitFindData->pUnitFindArg = pUnitFindArg;
}

//D2Common.0x6FDBCA50 (#10402)
void __stdcall UNITFINDS_FreeUnitFindData(D2UnitFindDataStrc* pUnitFindData)
{
	if (pUnitFindData && pUnitFindData->pUnitsArray)
	{
		FOG_FreeServerMemory(pUnitFindData->pMemPool, pUnitFindData->pUnitsArray, __FILE__, __LINE__, 0);
		pUnitFindData->pUnitsArray = NULL;
	}
}

//D2Common.0x6FDBCA80 (#10403)
void __stdcall UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(D2UnitFindDataStrc* pUnitFindData)
{
	D2RoomStrc** ppRoomList = NULL;
	D2RoomStrc* pRoom = NULL;
	D2UnitStrc* pNextUnit = NULL;
	int nUnitTest = 0;
	int nNumRooms = 0;
	int nIndex = 0;
	int nSize = 0;
	int nX = 0;
	int nY = 0;
	D2DrlgCoordsStrc pRoomCoord = {};

	D2_ASSERT(pUnitFindData);

	pRoom = pUnitFindData->pRoom;
	if (!pRoom)
	{
		pUnitFindData->nIndex = 0;
		return;
	}

	nX = pUnitFindData->nX;
	nY = pUnitFindData->nY;
	nSize = pUnitFindData->nSize;

	DUNGEON_GetRoomCoordinates(pRoom, &pRoomCoord);

	if (nX - nSize <= pRoomCoord.dwSubtilesLeft || nY - nSize <= pRoomCoord.dwSubtilesTop || nSize + nX >= pRoomCoord.dwSubtilesLeft + pRoomCoord.dwSubtilesWidth || nSize + nY >= pRoomCoord.dwSubtilesTop + pRoomCoord.dwSubtilesHeight)
	{
		DUNGEON_GetAdjacentRoomsListFromRoom(pUnitFindData->pRoom, &ppRoomList, &nNumRooms);
	}
	else
	{
		ppRoomList = &pUnitFindData->pRoom;
		nNumRooms = 1;
	}

	for (int i = 0; i < nNumRooms; ++i)
	{
		if (!(pUnitFindData->nFlags & 0x2000) || !DUNGEON_IsRoomInTown(ppRoomList[i]))
		{
			DUNGEON_GetRoomCoordinates(ppRoomList[i], &pRoomCoord);

			if ((nX + nSize >= pRoomCoord.dwSubtilesLeft || nX - nSize <= pRoomCoord.dwSubtilesLeft + pRoomCoord.dwSubtilesWidth)
				&& (nY + nSize >= pRoomCoord.dwSubtilesTop || nY - nSize <= pRoomCoord.dwSubtilesTop + pRoomCoord.dwSubtilesHeight))
			{
				
				for (D2UnitStrc* pUnit = ppRoomList[i]->pUnitFirst; pUnit; pUnit = pNextUnit)
				{
					pNextUnit = pUnit->pRoomNext;
					if (pUnitFindData->pfnUnitTest)
					{
						D2_ASSERT(!IsBadCodePtr((FARPROC)pUnitFindData->pfnUnitTest));

						nUnitTest = pUnitFindData->pfnUnitTest(pUnit, pUnitFindData->pUnitFindArg);
					}
					else
					{
						nUnitTest = UNITFINDS_TestUnit(pUnit, pUnitFindData->pUnitFindArg);
					}

					if (nUnitTest)
					{
						pUnitFindData->pUnitsArray[nIndex] = pUnit;
						++nIndex;

						if (nIndex == pUnitFindData->nMaxArrayEntries)
						{
							pUnitFindData->nMaxArrayEntries += UNIT_FIND_ARRAY_SIZE;
							pUnitFindData->pUnitsArray = (D2UnitStrc**)FOG_ReallocServerMemory(pUnitFindData->pMemPool, pUnitFindData->pUnitsArray, sizeof(D2UnitStrc*) * pUnitFindData->nMaxArrayEntries, __FILE__, __LINE__, 0);
						}
					}
				}
			}
		}
	}

	pUnitFindData->nIndex = nIndex;
}

//D2Common.0x6FDBCCA0 (#10404)
int __stdcall UNITFINDS_TestUnit(D2UnitStrc* pUnit, D2UnitFindArgStrc* pUnitFindArg)
{
	D2MissilesTxt* pMissilesTxtRecord = NULL;
	D2RoomStrc* pRoom = NULL;
	int nX = 0;
	int nY = 0;
	D2CoordStrc pCoords = {};

	if (pUnitFindArg && (!(pUnitFindArg->nFlags & 0x40) || pUnitFindArg->nIndex < pUnitFindArg->nMaxArrayEntries))
	{
		UNITS_GetCoords(pUnit, &pCoords);

		if ((pCoords.nX - pUnitFindArg->nX) * (pCoords.nX - pUnitFindArg->nX) + (pCoords.nY - pUnitFindArg->nY) * (pCoords.nY - pUnitFindArg->nY) <= pUnitFindArg->nSize * pUnitFindArg->nSize)
		{
			switch (pUnit->dwUnitType)
			{
			case UNIT_PLAYER:
				if (!(pUnitFindArg->nFlags & 1))
				{
					return 0;
				}

				if (pUnitFindArg->nFlags & 0x1000)
				{
					if (pUnit->dwAnimMode != PLRMODE_DEAD)
					{
						return 0;
					}
				}
				else if (pUnit->dwAnimMode == PLRMODE_DEAD || pUnit->dwAnimMode == PLRMODE_DEATH)
				{
					return 0;
				}

				if (pUnit == pUnitFindArg->pUnit)
				{
					return 0;
				}

				break;

			case UNIT_MONSTER:
				if (!(pUnitFindArg->nFlags & 2))
				{
					return 0;
				}

				if (pUnitFindArg->nFlags & 0x1000)
				{
					if (pUnit->dwAnimMode != MONMODE_DEAD)
					{
						return 0;
					}
				}
				else if (pUnit->dwAnimMode == MONMODE_DEAD || pUnit->dwAnimMode == MONMODE_DEATH)
				{
					return 0;
				}

				if (pUnitFindArg->nFlags & 4 && !MONSTERS_IsUndead(pUnit))
				{
					return 0;
				}

				break;

			case UNIT_OBJECT:
				if (!(pUnitFindArg->nFlags & 0x10))
				{
					return 0;
				}

				break;

			case UNIT_MISSILE:
				if (!(pUnitFindArg->nFlags & 8))
				{
					return 0;
				}
				
				pMissilesTxtRecord = DATATBLS_GetMissilesTxtRecord(pUnit->dwClassId);
				if(!pMissilesTxtRecord || pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_EXPLOSION])
				{
					return 0;
				}

				break;

			case UNIT_ITEM:
				if (!(pUnitFindArg->nFlags & 0x20))
				{
					return 0;
				}

				break;

			default:
				return 0;
			}
		}
	}

	if (!(pUnitFindArg->nFlags & 0x80) || pUnit->dwFlags & UNITFLAG_CANBEATTACKED)
	{
		if (!(pUnitFindArg->nFlags & 0x400) || pUnit->dwFlags & UNITFLAG_ISVALIDTARGET)
		{
			if (!(pUnitFindArg->nFlags & 0x100) || !DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
			{
				if (pUnitFindArg->nFlags & 0x200)
				{
					pRoom = UNITS_GetRoom(pUnit);
					if (pRoom)
					{
						while (D2Common_11098(pUnitFindArg->pField, &nX, &nY))
						{
							if (COLLISION_CheckMask(pRoom, nX, nY, 4) == 4)
							{
								return 0;
							}
						}
					}
				}

				if (!(pUnitFindArg->nFlags & 0x800) || !pUnitFindArg->pfnUnitTest(pUnit, pUnitFindArg))
				{
					++pUnitFindArg->nIndex;
					return 1;
				}
			}
		}
	}

	return 0;
}
