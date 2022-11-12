#include "Drlg/D2DrlgDrlgRoom.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgLogic.h"
#include "Drlg/D2DrlgDrlgWarp.h"
#include "Drlg/D2DrlgOutRoom.h"
#include "Drlg/D2DrlgPreset.h"
#include "Drlg/D2DrlgRoomTile.h"
#include "D2Dungeon.h"
#include "D2Seed.h"


//D2Common.0x6FD771C0
D2RoomExStrc* __fastcall DRLGROOM_AllocRoomEx(D2DrlgLevelStrc* pLevel, int nType)
{
	D2RoomExStrc* pRoomEx = D2_CALLOC_STRC_SERVER(pLevel->pDrlg->pMempool, D2RoomExStrc);

	pRoomEx->pLevel = pLevel;
	pRoomEx->nType = nType;
	pRoomEx->fRoomStatus = 4;
	SEED_InitLowSeed(&pRoomEx->pSeed, (int)SEED_RollRandomNumber(&pLevel->pSeed));
	pRoomEx->dwInitSeed = (unsigned int)SEED_RollRandomNumber(&pRoomEx->pSeed);

	if (pLevel->dwFlags & DRLGLEVELFLAG_AUTOMAP_REVEAL)
	{
		pRoomEx->dwFlags |= ROOMEXFLAG_AUTOMAP_REVEAL;
	}

	if (nType == DRLGTYPE_MAZE)
	{
		DRLGOUTROOM_AllocDrlgOutdoorRoom(pRoomEx);
	}
	else if (nType == DRLGTYPE_PRESET)
	{
		DRLGPRESET_AllocPresetRoomData(pRoomEx);
	}

	return pRoomEx;
}

//D2Common.0x6FD77280
void __fastcall sub_6FD77280(D2RoomExStrc* pRoomEx, BOOL bClient, uint32_t nFlags)
{
	pRoomEx->pRoom = NULL;

	pRoomEx->dwOtherFlags = nFlags & 1;

	if (pRoomEx->dwFlags & ROOMEXFLAG_HAS_ROOM)
	{
		sub_6FD8A2E0(pRoomEx, bClient == 0);
	}
}

//D2Common.0x6FD772B0
void __fastcall DRLGROOM_FreeRoomTiles(void* pMemPool, D2RoomExStrc* pRoomEx)
{
	D2RoomTileStrc* pRoomTile = NULL;
	D2RoomTileStrc* pNext = NULL;

	pRoomTile = pRoomEx->pRoomTiles;
	while (pRoomTile)
	{
		pNext = pRoomTile->pNext;
		D2_FREE_SERVER(pMemPool, pRoomTile);
		pRoomTile = pNext;
	}
	pRoomEx->pRoomTiles = NULL;
}

//D2Common.0x6FD772F0
void __fastcall DRLGROOM_FreeRoomEx(D2RoomExStrc* pRoomEx)
{
	D2PresetUnitStrc* pNextPresetUnit = NULL;
	D2RoomExStrc* pCurrentRoomEx = NULL;
	D2RoomExStrc* pNextRoomEx = NULL;
	D2DrlgOrthStrc* pNextDrlgOrth = NULL;
	D2DrlgOrthStrc* pNextOrth = NULL;
	D2DrlgOrthStrc* pOrth = NULL;
	void* pMemPool = NULL;

	pMemPool = pRoomEx->pLevel->pDrlg->pMempool;

	DRLGROOM_FreeRoomTiles(pMemPool, pRoomEx);

	if (pRoomEx->ppRoomsNear)
	{
		D2_FREE_SERVER(pMemPool, pRoomEx->ppRoomsNear);
		pRoomEx->nRoomsNear = 0;
		pRoomEx->ppRoomsNear = NULL;
	}

	if (pRoomEx->nType == DRLGTYPE_MAZE)
	{
		DRLGOUTROOM_FreeDrlgOutdoorRoom(pRoomEx);
	}
	else if (pRoomEx->nType == DRLGTYPE_PRESET)
	{
		DRLGPRESET_FreePresetRoomData(pRoomEx);
	}

	
	for (D2PresetUnitStrc* pPresetUnit = pRoomEx->pPresetUnits; pPresetUnit; pPresetUnit = pNextPresetUnit)
	{
		pNextPresetUnit = pPresetUnit->pNext;
		DRLGPRESET_FreePresetUnit(pMemPool, pPresetUnit);
	}

	for (D2DrlgOrthStrc* pDrlgOrth = pRoomEx->pDrlgOrth; pDrlgOrth; pDrlgOrth = pNextDrlgOrth)
	{
		pNextDrlgOrth = pDrlgOrth->pNext;
		if (pDrlgOrth->bInit == 1)
		{
			pCurrentRoomEx = pDrlgOrth->pRoomEx;
			pOrth = pRoomEx->pDrlgOrth;
			pNextOrth = pOrth->pNext;

			if (pOrth->bInit == 1 && pOrth->pRoomEx == pCurrentRoomEx)
			{
				pRoomEx->pDrlgOrth = pNextOrth;
				pNextOrth = pOrth;

				D2_FREE_SERVER(pMemPool, pNextOrth);
			}
			else
			{
				while (pNextOrth)
				{
					if (pNextOrth->bInit == 1 && pNextOrth->pRoomEx == pCurrentRoomEx)
					{
						pOrth->pNext = pNextOrth->pNext;
						D2_FREE_SERVER(pMemPool, pNextOrth);
						break;
					}

					pOrth = pNextOrth;
					pNextOrth = pNextOrth->pNext;
				}
			}

			pOrth = pCurrentRoomEx->pDrlgOrth;
			pNextOrth = pOrth->pNext;
			if (pOrth->bInit == 1 && pOrth->pRoomEx == pRoomEx)
			{
				pCurrentRoomEx->pDrlgOrth = pNextOrth;
				pNextOrth = pOrth;
				D2_FREE_SERVER(pMemPool, pNextOrth);
			}
			else
			{
				while (pNextOrth)
				{
					if (pNextOrth->bInit == 1 && pNextOrth->pRoomEx == pRoomEx)
					{
						pOrth->pNext = pNextOrth->pNext;
						D2_FREE_SERVER(pMemPool, pNextOrth);
						break;
					}

					pOrth = pNextOrth;
					pNextOrth = pNextOrth->pNext;
				}
			}
		}
	}

	for (D2DrlgOrthStrc* pDrlgOrth = pRoomEx->pDrlgOrth; pDrlgOrth; pDrlgOrth = pNextDrlgOrth)
	{
		pNextDrlgOrth = pDrlgOrth->pNext;
		D2_FREE_SERVER(pMemPool, pDrlgOrth);
	}

	pCurrentRoomEx = pRoomEx->pLevel->pFirstRoomEx;
	if (pCurrentRoomEx == pRoomEx)
	{
		pRoomEx->pLevel->pFirstRoomEx = pRoomEx->pRoomExNext;
		--pRoomEx->pLevel->nRooms;
	}
	else
	{
		pNextRoomEx = pCurrentRoomEx->pRoomExNext;
		while (pNextRoomEx)
		{
			if (pNextRoomEx == pRoomEx)
			{
				pCurrentRoomEx->pRoomExNext = pRoomEx->pRoomExNext;
				--pRoomEx->pLevel->nRooms;
				break;
			}

			pCurrentRoomEx = pNextRoomEx;
			pNextRoomEx = pCurrentRoomEx->pRoomExNext;
		}
	}

	DRLGROOMTILE_FreeTileGrid(pRoomEx);
	DRLGLOGIC_FreeDrlgCoordList(pRoomEx);
	D2_FREE_SERVER(pMemPool, pRoomEx);
}

//D2Common.0x6FD774F0
void __fastcall DRLGROOM_FreeRoomData(void* pMemPool, D2DrlgOrthStrc* pDrlgRoomData)
{
	D2DrlgOrthStrc* pNext = NULL;
	
	for (D2DrlgOrthStrc* pRoomData = pDrlgRoomData; pRoomData; pRoomData = pNext)
	{
		pNext = pRoomData->pNext;
		D2_FREE_SERVER(pMemPool, pRoomData);
	}
}

//D2Common.0x6FD77520
void __fastcall DRLGROOM_AllocDrlgOrthsForRooms(D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2, int nDirection)
{
	D2DrlgOrthStrc* pDrlgOrth = NULL;
	D2DrlgOrthStrc* pNew = NULL;

	pDrlgOrth = pRoomEx1->pDrlgOrth;
	while (pDrlgOrth)
	{
		if (pDrlgOrth->pRoomEx == pRoomEx2)
		{
			break;
		}
		pDrlgOrth = pDrlgOrth->pNext;
	}

	if (!pDrlgOrth)
	{
		pNew = D2_CALLOC_STRC_SERVER(pRoomEx1->pLevel->pDrlg->pMempool, D2DrlgOrthStrc);
		pNew->pNext = pRoomEx1->pDrlgOrth;
		pRoomEx1->pDrlgOrth = pNew;
		pNew->pRoomEx = pRoomEx2;
		pNew->nDirection = nDirection;
		pNew->bInit = 1;
		pNew->pBox = &pRoomEx2->pDrlgCoord;
	}

	pDrlgOrth = pRoomEx2->pDrlgOrth;
	while (pDrlgOrth)
	{
		if (pDrlgOrth->pRoomEx == pRoomEx1)
		{
			break;
		}
		pDrlgOrth = pDrlgOrth->pNext;
	}

	if (!pDrlgOrth)
	{
		pNew = D2_CALLOC_STRC_SERVER(pRoomEx2->pLevel->pDrlg->pMempool, D2DrlgOrthStrc);
		pNew->pNext = pRoomEx2->pDrlgOrth;
		pRoomEx2->pDrlgOrth = pNew;
		pNew->pRoomEx = pRoomEx1;
		pNew->nDirection = ((uint8_t)nDirection - 2) & 3;
		pNew->bInit = 1;
		pNew->pBox = &pRoomEx1->pDrlgCoord;
	}
}

//D2Common.0x6FD77600
void __fastcall DRLGROOM_AddOrth(D2DrlgOrthStrc** ppDrlgOrth, D2DrlgLevelStrc* pLevel, int nDirection, BOOL bIsPreset)
{
	D2DrlgOrthStrc* pPrevious = NULL;
	D2DrlgOrthStrc* pNext = NULL;
	D2DrlgOrthStrc* pNew = NULL;

	pNew = D2_CALLOC_STRC_SERVER(pLevel->pDrlg->pMempool, D2DrlgOrthStrc);

	pNew->pLevel = pLevel;
	pNew->nDirection = nDirection;
	pNew->bPreset = bIsPreset;
	pNew->bInit = 0;
	pNew->pBox = &pLevel->pLevelCoords;

	if (!*ppDrlgOrth)
	{
		*ppDrlgOrth = pNew;
	}
	else
	{
		pNext = (*ppDrlgOrth)->pNext;
		pPrevious = *ppDrlgOrth;
		if (pNext)
		{
			do
			{
				if (sub_6FD776B0(pNext, pNew))
				{
					break;
				}

				pPrevious = pNext;
				pNext = pNext->pNext;
			}
			while (pNext);
		}
		else
		{
			if (sub_6FD776B0(*ppDrlgOrth, pNew))
			{
				pNew->pNext = *ppDrlgOrth;
				*ppDrlgOrth = pNew;
				return;
			}
		}

		pNew->pNext = pNext;
		pPrevious->pNext = pNew;
	}
}

//D2Common.0x6FD776B0
BOOL __fastcall sub_6FD776B0(D2DrlgOrthStrc* pDrlgOrth1, D2DrlgOrthStrc* pDrlgOrth2)
{
	if (pDrlgOrth1->nDirection <= pDrlgOrth2->nDirection)
	{
		if (pDrlgOrth1->nDirection == pDrlgOrth2->nDirection)
		{
			switch (pDrlgOrth2->nDirection)
			{
			case 0:
				if (pDrlgOrth1->pBox->nPosY <= pDrlgOrth2->pBox->nPosY)
				{
					return FALSE;
				}
				break;

			case 1:
				if (pDrlgOrth1->pBox->nPosX >= pDrlgOrth2->pBox->nPosX)
				{
					return FALSE;
				}
				break;

			case 2:
				if (pDrlgOrth1->pBox->nPosY >= pDrlgOrth2->pBox->nPosY)
				{
					return FALSE;
				}
				break;

			case 3:
				if (pDrlgOrth1->pBox->nPosX <= pDrlgOrth2->pBox->nPosX)
				{
					return FALSE;
				}
				break;

			default:
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

//D2Common.0x6FD77740
BOOL __fastcall sub_6FD77740(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int nMaxDistance, int* pX, int* pY)
{
	if (pDrlgCoord1->nPosX >= pDrlgCoord2->nPosX)
	{
		*pX = pDrlgCoord1->nPosX - pDrlgCoord2->nWidth - pDrlgCoord2->nPosX;
	}
	else
	{
		*pX = pDrlgCoord2->nPosX - pDrlgCoord1->nWidth - pDrlgCoord1->nPosX;
	}

	if (pDrlgCoord1->nPosY >= pDrlgCoord2->nPosY)
	{
		*pY = pDrlgCoord1->nPosY - pDrlgCoord2->nHeight - pDrlgCoord2->nPosY;
	}
	else
	{
		*pY = pDrlgCoord2->nPosY - pDrlgCoord1->nHeight - pDrlgCoord1->nPosY;
	}

	return *pX >= nMaxDistance || *pY >= nMaxDistance;
}

//D2Common.0x6FD777B0
// Compute manhattan distance between rectangles and returns true if distance is greater or equal than nMaxDistance
BOOL __fastcall DRLG_ComputeRectanglesManhattanDistance(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int nMaxDistanceToAssumeCollision)
{
	int nSignedDistanceX = 0;
	int nSignedDistanceY = 0;

	// Negative distance means we are "inside" the other rectangle
	if (pDrlgCoord1->nPosX >= pDrlgCoord2->nPosX)
	{
		nSignedDistanceX = pDrlgCoord1->nPosX - pDrlgCoord2->nWidth - pDrlgCoord2->nPosX;
	}
	else
	{
		nSignedDistanceX = pDrlgCoord2->nPosX - pDrlgCoord1->nWidth - pDrlgCoord1->nPosX;
	}

	if (pDrlgCoord1->nPosY >= pDrlgCoord2->nPosY)
	{
		nSignedDistanceY = pDrlgCoord1->nPosY - pDrlgCoord2->nHeight - pDrlgCoord2->nPosY;
	}
	else
	{
		nSignedDistanceY = pDrlgCoord2->nPosY - pDrlgCoord1->nHeight - pDrlgCoord1->nPosY;
	}

	return nSignedDistanceX >= nMaxDistanceToAssumeCollision || nSignedDistanceY >= nMaxDistanceToAssumeCollision;
}

//D2Common.0x6FD77800
BOOL __fastcall sub_6FD77800(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int nMaxDistance)
{
	int nX = 0;
	int nY = 0;

	if (pDrlgCoord1->nPosX > pDrlgCoord2->nPosX)
	{
		nX = pDrlgCoord1->nPosX - pDrlgCoord2->nWidth - pDrlgCoord2->nPosX;
	}
	else
	{
		nX = pDrlgCoord2->nPosX - pDrlgCoord1->nWidth - pDrlgCoord1->nPosX;
	}

	if (pDrlgCoord1->nPosY > pDrlgCoord2->nPosY)
	{
		nY = pDrlgCoord1->nPosY - pDrlgCoord2->nHeight - pDrlgCoord2->nPosY;
	}
	else
	{
		nY = pDrlgCoord2->nPosY - pDrlgCoord1->nHeight - pDrlgCoord1->nPosY;
	}

	if (nMaxDistance)
	{
		if (!nX && nY <= nMaxDistance)
		{
			return TRUE;
		}

		if (!nY && nX <= nMaxDistance)
		{
			return TRUE;
		}
	}
	else
	{
		if (nX <= 0 && nY <= 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD77890
BOOL __fastcall sub_6FD77890(D2DrlgLevelStrc* pLevel, D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2, int nMaxDistance)
{
	int nX = 0;
	int nY = 0;

	for (D2RoomExStrc* pRoomEx = pLevel->pFirstRoomEx; pRoomEx; pRoomEx = pRoomEx->pRoomExNext)
	{
		if (pRoomEx != pRoomEx1 && pRoomEx != pRoomEx2)
		{
			if (pRoomEx1->nTileXPos >= pRoomEx->nTileXPos)
			{
				nX = pRoomEx1->nTileXPos - pRoomEx->nTileWidth - pRoomEx->nTileXPos;
			}
			else
			{
				nX = pRoomEx->nTileXPos - pRoomEx1->nTileWidth - pRoomEx1->nTileXPos;
			}

			if (pRoomEx1->nTileYPos >= pRoomEx->nTileYPos)
			{
				nY = pRoomEx1->nTileYPos - pRoomEx->nTileHeight - pRoomEx->nTileYPos;
			}
			else
			{
				nY = pRoomEx->nTileYPos - pRoomEx1->nTileHeight - pRoomEx1->nTileYPos;
			}

			if (nX < nMaxDistance && nY < nMaxDistance)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//D2Common.0x6FD77910
void __fastcall DRLGROOM_AddRoomExToLevel(D2DrlgLevelStrc* pLevel, D2RoomExStrc* pRoomEx)
{
	pRoomEx->pRoomExNext = pLevel->pFirstRoomEx;
	pLevel->pFirstRoomEx = pRoomEx;
	++pLevel->nRooms;
}

//D2Common.0x6FD77930
BOOL __fastcall DRLGROOM_AreXYInsideCoordinates(D2DrlgCoordStrc* pDrlgCoord, int nX, int nY)
{
	if (nX >= pDrlgCoord->nPosX && nY >= pDrlgCoord->nPosY && nX < pDrlgCoord->nPosX + pDrlgCoord->nWidth && nY < pDrlgCoord->nPosY + pDrlgCoord->nHeight)
	{
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD77980
BOOL __fastcall DRLGROOM_AreXYInsideCoordinatesOrOnBorder(D2DrlgCoordStrc* pDrlgCoord, int nX, int nY)
{
	if (nX >= pDrlgCoord->nPosX && nY >= pDrlgCoord->nPosY && nX <= pDrlgCoord->nPosX + pDrlgCoord->nWidth && nY <= pDrlgCoord->nPosY + pDrlgCoord->nHeight)
	{
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD779D0
BOOL __fastcall DRLGROOM_CheckLOSDraw(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->nType == DRLGTYPE_MAZE)
	{
		return TRUE;
	}
	else if (pRoomEx->nType == DRLGTYPE_PRESET)
	{
		return pRoomEx->dwFlags & ROOMEXFLAG_NO_LOS_DRAW;
	}
		
	return FALSE;
}

//D2Common.0x6FD779F0
int __fastcall sub_6FD779F0(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->nType == DRLGTYPE_MAZE)
	{
		return pRoomEx->pOutdoor->dwFlags & 0x80;
	}

	return 0;
}

//D2Common.0x6FD77A00
int __fastcall DRLGROOM_CheckWaypointFlags(D2RoomExStrc* pRoomEx)
{
	return pRoomEx->dwFlags & (ROOMEXFLAG_HAS_WAYPOINT | ROOMEXFLAG_HAS_WAYPOINT_SMALL);
}

//D2Common.0x6FD77A10
int __fastcall DRLGROOM_GetLevelId(D2RoomExStrc* pRoomEx)
{
	return pRoomEx->pLevel->nLevelId;
}

//D2Common.0x6FD77A20
int __fastcall DRLGROOM_GetWarpDestinationLevel(D2RoomExStrc* pRoomEx, int nSourceLevel)
{
	D2LvlWarpTxt* pLvlWarpTxtRecord = 0;
	D2RoomStrc* pRoom = NULL;
	int nDestinationLevel = 0;

	pRoom = DRLGWARP_GetDestinationRoom(pRoomEx, nSourceLevel, &nDestinationLevel, &pLvlWarpTxtRecord);
	D2_ASSERT(pRoom);
	
	pRoomEx = DUNGEON_GetRoomExFromRoom(pRoom);
	D2_ASSERT(pRoomEx);
	
	D2_ASSERT(pRoomEx->pLevel);
	return pRoomEx->pLevel->nLevelId;
}

//D2Common.0x6FD77AB0
int __fastcall DRLGROOM_GetLevelIdFromPopulatedRoom(D2RoomExStrc* pRoomEx)
{
	if (!pRoomEx)
	{
		FOG_10025("ptRoom", __FILE__, __LINE__);
	}

	if (pRoomEx->dwFlags & ROOMEXFLAG_POPULATION_ZERO)
	{
		return 0;
	}
	else
	{
		return pRoomEx->pLevel->nLevelId;
	}
}

//D2Common.0x6FD77AF0
BOOL __fastcall DRLGROOM_HasWaypoint(D2RoomExStrc* pRoomEx)
{
	if (!pRoomEx)
	{
		FOG_10025("ptRoom", __FILE__, __LINE__);
	}

	return DRLGROOM_CheckWaypointFlags(pRoomEx) != 0;
}

//D2Common.0x6FD77B20
char* __fastcall DRLGROOM_GetPickedLevelPrestFilePathFromRoomEx(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->nType == DRLGTYPE_PRESET)
	{
		return DRLGPRESET_GetPickedLevelPrestFilePathFromRoomEx(pRoomEx);
	}
	else
	{
		return "None";
	}
}

//D2Common.0x6FD77B50
int __fastcall DRLGROOM_ReorderNearRoomList(D2RoomExStrc* pRoomEx, D2RoomStrc** ppRoomList)
{
	int nRooms = 0;

	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		if (pRoomEx->ppRoomsNear[i]->pRoom)
		{
			ppRoomList[nRooms] = pRoomEx->ppRoomsNear[i]->pRoom;
			++nRooms;
		}
	}

	for (int i = nRooms; i < pRoomEx->nRoomsNear; ++i)
	{
		ppRoomList[i] = NULL;
	}

	return nRooms;
}

//D2Common.0x6FD77BB0
void __fastcall sub_6FD77BB0(void* pMemPool, D2RoomExStrc* pRoomEx)
{
	D2RoomExStrc* ppNearRooms[30] = {};
	D2DrlgLevelStrc* pLevel = NULL;
	int* pDestinationVisArray = 0;
	int* pSourceVisArray = 0;
	int nWarpDestination = 0;
	int nDestinationVisCount = 0;
	int nSourceVisCount = 0;
	int nFlags = 0;
	int nX = 0;
	int nY = 0;
	BOOL bSkip = FALSE;
	uint8_t nWarpId = 0;

	pRoomEx->nRoomsNear = 0;

	for (D2RoomExStrc* pCurrentRoomEx = pRoomEx->pLevel->pFirstRoomEx; pCurrentRoomEx; pCurrentRoomEx = pCurrentRoomEx->pRoomExNext)
	{
		if (pRoomEx->nTileXPos >= pCurrentRoomEx->nTileXPos)
		{
			nX = pRoomEx->nTileXPos - pCurrentRoomEx->nTileWidth - pCurrentRoomEx->nTileXPos;
		}
		else
		{
			nX = pCurrentRoomEx->nTileXPos - pRoomEx->nTileWidth - pRoomEx->nTileXPos;
		}

		if (pRoomEx->nTileYPos >= pCurrentRoomEx->nTileYPos)
		{
			nY = pRoomEx->nTileYPos - pCurrentRoomEx->nTileHeight - pCurrentRoomEx->nTileYPos;
		}
		else
		{
			nY = pCurrentRoomEx->nTileYPos - pRoomEx->nTileHeight - pRoomEx->nTileYPos;
		}

		if (nX < 6 && nY < 6)
		{
			ppNearRooms[pRoomEx->nRoomsNear] = pCurrentRoomEx;
			++pRoomEx->nRoomsNear;
		}
	}

	DRLGROOM_SortRoomListByPosition(ppNearRooms, pRoomEx->nRoomsNear);

	pRoomEx->ppRoomsNear = (D2RoomExStrc**)D2_ALLOC_SERVER(pMemPool, sizeof(D2RoomExStrc*) * pRoomEx->nRoomsNear);

	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		pRoomEx->ppRoomsNear[i] = ppNearRooms[i];
	}

	if (pRoomEx->dwFlags & ROOMEXFLAG_HAS_WARP_MASK)
	{
		nFlags = ROOMEXFLAG_HAS_WARP_0;
		nWarpId = 0;

		do
		{
			if (nFlags & pRoomEx->dwFlags)
			{
				if (nWarpId >= 8)
				{
					FOG_10025("ptRoom1 && bExit1 < LEVEL_VIS_MAX", __FILE__, __LINE__);
				}

				pSourceVisArray = DRLGROOM_GetVisArrayFromLevelId(pRoomEx->pLevel->pDrlg, pRoomEx->pLevel->nLevelId);

				pLevel = DRLG_GetLevel(pRoomEx->pLevel->pDrlg, pSourceVisArray[nWarpId]);

				pDestinationVisArray = DRLGROOM_GetVisArrayFromLevelId(pRoomEx->pLevel->pDrlg, pSourceVisArray[nWarpId]);
				nWarpDestination = DRLGWARP_GetWarpDestinationFromArray(pRoomEx->pLevel, nWarpId);

				if (!pLevel->pFirstRoomEx)
				{
					DRLG_InitLevel(pLevel);
				}

				if (nWarpDestination != -1)
				{
					nSourceVisCount = 0;
					nDestinationVisCount = 0;
					for (int i = 0; i < nWarpId; ++i)
					{
						if (pSourceVisArray[i] == pSourceVisArray[nWarpId])
						{
							++nSourceVisCount;
						}
					}

					for (int i = 0; i < 8; ++i)
					{
						if (pDestinationVisArray[i] == pRoomEx->pLevel->nLevelId)
						{
							if (nSourceVisCount == nDestinationVisCount)
							{
								if (sub_6FD77F00(pMemPool, pRoomEx, nWarpId, pLevel->pFirstRoomEx, i, nWarpDestination))
								{
									bSkip = TRUE;
								}
								break;
							}

							++nDestinationVisCount;
						}
					}
				}

				if (!bSkip)
				{
					for (int i = 0; i < 8; ++i)
					{
						if (pDestinationVisArray[i] == pRoomEx->pLevel->nLevelId && sub_6FD77F00(pMemPool, pRoomEx, nWarpId, pLevel->pFirstRoomEx, i, nWarpDestination))
						{
							break;
						}
					}
				}
			}

			nFlags <<= 1;

			++nWarpId;
		}
		while (nFlags & ROOMEXFLAG_HAS_WARP_MASK);
	}

	if (!DRLG_IsTownLevel(pRoomEx->pLevel->nLevelId))
	{
		for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
		{
			if (DRLG_IsTownLevel(pRoomEx->ppRoomsNear[i]->pLevel->nLevelId))
			{
				pRoomEx->dwFlags |= ROOMEXFLAG_POPULATION_ZERO;
				return;
			}
		}
	}
}

//D2Common.0x6FD77EB0
void __fastcall DRLGROOM_SortRoomListByPosition(D2RoomExStrc** ppRoomList, int nListSize)
{
	D2RoomExStrc* pRoomEx1 = NULL;
	D2RoomExStrc* pRoomEx2 = NULL;

	for (int j = nListSize - 1; j > 0; --j)
	{
		for (int i = 0; i < nListSize - 1; ++i)
		{
			pRoomEx1 = ppRoomList[i + 1];
			pRoomEx2 = ppRoomList[i];

			if (pRoomEx2->nTileXPos >= pRoomEx1->nTileXPos + pRoomEx1->nTileWidth || pRoomEx2->nTileYPos >= pRoomEx1->nTileYPos + pRoomEx1->nTileHeight)
			{
				ppRoomList[i] = pRoomEx1;
				ppRoomList[i + 1] = pRoomEx2;
			}
		}
	}
}

//D2Common.0x6FD77F00
BOOL __fastcall sub_6FD77F00(void* pMemPool, D2RoomExStrc* pRoomEx1, uint8_t nWarpId, D2RoomExStrc* pRoomEx2, char nWarpFlag, int nDirection)
{
	D2RoomTileStrc* pRoomTile = NULL;
	int nX = 0;
	int nY = 0;
	BOOL bResult = FALSE;

	while (pRoomEx2)
	{
		if (((1 << (nWarpFlag + 4)) & pRoomEx2->dwFlags))
		{
			if (nDirection == -1)
			{
				if (pRoomEx1->nTileXPos >= pRoomEx2->nTileXPos)
				{
					nX = pRoomEx1->nTileXPos - pRoomEx2->nTileWidth - pRoomEx2->nTileXPos;
				}
				else
				{
					nX = pRoomEx2->nTileXPos - pRoomEx1->nTileWidth - pRoomEx1->nTileXPos;
				}

				if (pRoomEx1->nTileYPos >= pRoomEx2->nTileYPos)
				{
					nY = pRoomEx1->nTileYPos - pRoomEx2->nTileHeight - pRoomEx2->nTileYPos;
				}
				else
				{
					nY = pRoomEx2->nTileYPos - pRoomEx1->nTileHeight - pRoomEx1->nTileYPos;
				}

				if (nX < 6 && nY < 6)
				{
					++pRoomEx1->nRoomsNear;
					pRoomEx1->ppRoomsNear = (D2RoomExStrc**)D2_REALLOC_SERVER(pMemPool, pRoomEx1->ppRoomsNear, sizeof(D2RoomExStrc*) * pRoomEx1->nRoomsNear);

					D2_ASSERT(pRoomEx1->ppRoomsNear);

					pRoomEx1->ppRoomsNear[pRoomEx1->nRoomsNear - 1] = pRoomEx2;
					DRLGROOM_SortRoomListByPosition(pRoomEx1->ppRoomsNear, pRoomEx1->nRoomsNear);

					bResult = TRUE;
				}
			}
			else
			{
				++pRoomEx1->nRoomsNear;
				pRoomEx1->ppRoomsNear = (D2RoomExStrc**)FOG_ReallocServerMemory(pMemPool, pRoomEx1->ppRoomsNear, sizeof(D2RoomExStrc*) * pRoomEx1->nRoomsNear, __FILE__, __LINE__, 0);

				D2_ASSERT(pRoomEx1->ppRoomsNear);
				pRoomEx1->ppRoomsNear[pRoomEx1->nRoomsNear - 1] = pRoomEx2;
				DRLGROOM_SortRoomListByPosition(pRoomEx1->ppRoomsNear, pRoomEx1->nRoomsNear);

				pRoomTile = D2_CALLOC_STRC_SERVER(pRoomEx1->pLevel->pDrlg->pMempool, D2RoomTileStrc);
				pRoomTile->pRoomEx = pRoomEx2;
				pRoomTile->pLvlWarpTxtRecord = DRLGWARP_GetLvlWarpTxtRecordFromWarpIdAndDirection(pRoomEx1->pLevel, nWarpId, 'b');
				pRoomTile->bEnabled = 1;
				pRoomTile->pNext = pRoomEx1->pRoomTiles;
				pRoomEx1->pRoomTiles = pRoomTile;

				return TRUE;
			}
		}

		pRoomEx2 = pRoomEx2->pRoomExNext;
	}

	return bResult;
}

//D2Common.0x6FD780E0
D2PresetUnitStrc* __fastcall DRLGROOM_AllocPresetUnit(D2RoomExStrc* pRoomEx, void* pMemPool, int nUnitType, int nIndex, int nMode, int nX, int nY)
{
	D2PresetUnitStrc* pPresetUnit = D2_CALLOC_STRC_SERVER(pMemPool, D2PresetUnitStrc);

	pPresetUnit->nUnitType = nUnitType;
	pPresetUnit->nMode = nMode;
	pPresetUnit->nIndex = nIndex;
	pPresetUnit->nYpos = nY;
	pPresetUnit->nXpos = nX;

	if (pRoomEx)
	{
		pPresetUnit->pNext = pRoomEx->pPresetUnits;
		pRoomEx->pPresetUnits = pPresetUnit;
	}
	else
	{
		pPresetUnit->pNext = NULL;
	}

	return pPresetUnit;
}

//D2Common.0x6FD78160
D2PresetUnitStrc* __fastcall DRLGROOM_GetPresetUnits(D2RoomExStrc* pRoomEx)
{
	if (!(pRoomEx->dwFlags & ROOMEXFLAG_AUTOMAP_REVEAL))
	{
		if (pRoomEx->dwFlags & ROOMEXFLAG_PRESET_UNITS_SPAWNED)
		{
			return NULL;
		}

		pRoomEx->dwFlags |= ROOMEXFLAG_PRESET_UNITS_SPAWNED;
	}

	return pRoomEx->pPresetUnits;
}

//D2Common.0x6FD78190
void __fastcall DRLGROOM_SetRoom(D2RoomExStrc* pRoomEx, D2RoomStrc* pRoom)
{
	pRoomEx->pRoom = pRoom;
}

//D2Common.0x6FD781A0
void __fastcall DRLGROOM_GetRGB_IntensityFromRoomEx(D2RoomExStrc* pRoomEx, uint8_t* pIntensity, uint8_t* pRed, uint8_t* pGreen, uint8_t* pBlue)
{
	D2LevelDefBin* pLevelDefRecord = DATATBLS_GetLevelDefRecord(pRoomEx->pLevel->nLevelId);

	*pIntensity = pLevelDefRecord->nIntensity;
	*pRed = pLevelDefRecord->nRed;
	*pGreen = pLevelDefRecord->nGreen;
	*pBlue = pLevelDefRecord->nBlue;
}

//D2Common.0x6FD781E0
int* __fastcall DRLGROOM_GetVisArrayFromLevelId(D2DrlgStrc* pDrlg, int nLevelId)
{
	D2DrlgWarpStrc* pDrlgWarp = pDrlg->pWarp;

	while (pDrlgWarp)
	{
		if (!pDrlgWarp->nLevel)
		{
			FOG_10025("ptVisInfo->eLevelId != LEVEL_ID_NONE", __FILE__, __LINE__);
		}

		if (nLevelId == pDrlgWarp->nLevel)
		{
			return pDrlgWarp->nVis;
		}

		pDrlgWarp = pDrlgWarp->pNext;
	}

	return DATATBLS_GetLevelDefRecord(nLevelId)->dwVis;
}

//D2Common.0x6FD78230
D2DrlgStrc* __fastcall DRLGROOM_GetDrlgFromRoomEx(D2RoomExStrc* pRoom)
{
	D2_ASSERT(pRoom);
	D2_ASSERT(pRoom->pLevel);
	D2_ASSERT(pRoom->pLevel->pDrlg);
	return pRoom->pLevel->pDrlg;
}
