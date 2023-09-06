#include "D2Collision.h"

#include "D2DataTbls.h"
#include "D2Dungeon.h"
#include <D2Lang.h>
#include <D2CMP.h>

//D2Common.0x6FD41000
void __fastcall D2Common_COLLISION_FirstFn_6FD41000(D2RoomStrc* pRoom, D2DrlgTileDataStrc* pTileData, D2TileLibraryEntryStrc* pTileLibraryEntry)
{
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;
	uint16_t* pCollisionMask = NULL;
	uint8_t* v17 = NULL;
	uint8_t* pTmp = NULL;
	int nCappedX = 0;
	int nCappedY = 0;
	int nIndex = 0;
	int nX = 0;
	int nY = 0;

	if (pRoom)
	{
		pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid)
		{
			nX = pTileData->nPosX + pCollisionGrid->pRoomCoords.dwTilesLeft;
			nY = pTileData->nPosY + pCollisionGrid->pRoomCoords.dwTilesTop;
			DUNGEON_GameTileToSubtileCoords(&nX, &nY);

			pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY);

			if (pRoom)
			{
				pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
				if (pCollisionGrid)
				{
					nX -= pCollisionGrid->pRoomCoords.dwSubtilesLeft;
					nY -= pCollisionGrid->pRoomCoords.dwSubtilesTop;

					if (pCollisionGrid->pCollisionMask)
					{
						v17 = D2CMP_10085_GetTileFlagArray(pTileData->pTile);

						if (nX > 0)
						{
							nCappedX = nX;
						}
						else
						{
							nCappedX = 0;
						}

						if (nY > 0)
						{
							nCappedY = nY;
						}
						else
						{
							nCappedY = 0;
						}

						pTmp = &v17[5 * (nY - nCappedY + 4) - nX];

						nIndex = pCollisionGrid->pRoomCoords.dwSubtilesWidth * nCappedY;

						for (int nYCounter = nCappedX; nYCounter < nY + 5; ++nYCounter)
						{
							pCollisionMask = &pCollisionGrid->pCollisionMask[nIndex];
							for (int nXCounter = nCappedX; nXCounter < nX + 5; ++nXCounter)
							{
								pCollisionMask[nXCounter] &= ~pTmp[nXCounter];
							}

							pTmp -= 5;
							nIndex += pCollisionGrid->pRoomCoords.dwSubtilesWidth;
						}
					}

					if (pTileLibraryEntry)
					{
						sub_6FD411F0(pCollisionGrid, pTileLibraryEntry, nX, nY);
					}
				}
			}
		}
	}
}

//D2Common.0x6FD411F0
void __fastcall sub_6FD411F0(D2RoomCollisionGridStrc* pCollisionGrid, D2TileLibraryEntryStrc* pTileLibraryEntry, int nX, int nY)
{
	uint16_t* pCollisionMask = NULL;
	uint8_t* v5 = NULL;
	uint8_t* pTmp = NULL;
	int nCappedX = 0;
	int nCappedY = 0;
	int nIndex = 0;

	if (pCollisionGrid && pCollisionGrid->pCollisionMask)
	{
		v5 = D2CMP_10085_GetTileFlagArray(pTileLibraryEntry);

		if (nX > 0)
		{
			nCappedX = nX;
		}
		else
		{
			nCappedX = 0;
		}

		if (nY > 0)
		{
			nCappedY = nY;
		}
		else
		{
			nCappedY = 0;
		}

		pTmp = &v5[5 * (nY - nCappedY + 4) - nX];

		nIndex = pCollisionGrid->pRoomCoords.dwSubtilesWidth * nCappedY;

		for (int nYCounter = nCappedY; nYCounter < nY + 5; ++nYCounter)
		{
			pCollisionMask = &pCollisionGrid->pCollisionMask[nIndex];
			for (int nXCounter = nCappedX; nXCounter < nX + 5; ++nXCounter)
			{
				pCollisionMask[nXCounter] |= pTmp[nXCounter];
			}

			pTmp -= 5;
			nIndex += pCollisionGrid->pRoomCoords.dwSubtilesWidth;
		}
	}
}

//D2Common.0x6FD412B0 (#10018)
int __stdcall D2COMMON_10018_Return0()
{
	REMOVE_LATER_Trace("D2COMMON_10018_Return0: Unused");
	return 0;
}

//D2Common.0x6FD412C0
void __fastcall COLLISION_AllocRoomCollisionGrid(void* pMemPool, D2RoomStrc* pRoom)
{
	D2RoomCollisionGridStrc* pAdjacentCollisionGrid = NULL;
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;
	D2DrlgTileDataStrc* pTileData = NULL;
	D2RoomStrc** ppRoomList = NULL;
	int nAdjacentRooms = 0;
	int nTiles = 0;
	D2DrlgCoordsStrc pDrlgCoords = {};

	if (pRoom)
	{
		DUNGEON_GetRoomCoordinates(pRoom, &pDrlgCoords);

		pCollisionGrid = (D2RoomCollisionGridStrc*)D2_ALLOC_POOL(pMemPool, sizeof(D2RoomCollisionGridStrc) + /*align*/ sizeof(uint16_t) * ((pDrlgCoords.dwSubtilesWidth + 2) * pDrlgCoords.dwSubtilesHeight));
		memcpy(&pCollisionGrid->pRoomCoords, &pDrlgCoords, sizeof(D2DrlgCoordsStrc));
		DUNGEON_SetCollisionGridInRoom(pRoom, pCollisionGrid);

		pCollisionGrid->pCollisionMask = (uint16_t*)&pCollisionGrid[1];
		memset(pCollisionGrid->pCollisionMask, 0x00, sizeof(uint16_t) * (pDrlgCoords.dwSubtilesWidth * pDrlgCoords.dwSubtilesHeight)); // TODO: (pDrlgCoords.dwSubtilesWidth + 2) as above?

		DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nAdjacentRooms);
		for (int i = 0; i < nAdjacentRooms; ++i)
		{
			pAdjacentCollisionGrid = DUNGEON_GetCollisionGridFromRoom(ppRoomList[i]);
			pTileData = DUNGEON_GetFloorTilesFromRoom(ppRoomList[i], &nTiles);
			sub_6FD413E0(pCollisionGrid, pAdjacentCollisionGrid, pTileData, nTiles, 0);
			pTileData = DUNGEON_GetWallTilesFromRoom(ppRoomList[i], &nTiles);
			sub_6FD413E0(pCollisionGrid, pAdjacentCollisionGrid, pTileData, nTiles, 0);
			pTileData = DUNGEON_GetRoofTilesFromRoom(ppRoomList[i], &nTiles);
			sub_6FD413E0(pCollisionGrid, pAdjacentCollisionGrid, pTileData, nTiles, 0);
		}
	}
}

//D2Common.0x6FD413E0
void __fastcall sub_6FD413E0(D2RoomCollisionGridStrc* pCollisionGrid, D2RoomCollisionGridStrc* pAdjacentCollisionGrid, D2DrlgTileDataStrc* pTiles, int nTiles, BOOL bRemoveOldFlags)
{
	uint16_t* pCollisionMask = NULL;
	uint8_t* v11 = NULL;
	uint8_t* pTmp = NULL;
	int nCappedX = 0;
	int nCappedY = 0;
	int nIndex = 0;
	int nFlags = 0;
	int nX = 0;
	int nY = 0;

	for (int i = 0; i < nTiles; ++i)
	{
		nX = pTiles[i].nPosX + pAdjacentCollisionGrid->pRoomCoords.dwTilesLeft;
		nY = pTiles[i].nPosY + pAdjacentCollisionGrid->pRoomCoords.dwTilesTop;

		DUNGEON_GameTileToSubtileCoords(&nX, &nY);

		if (DUNGEON_AreSubtileCoordinatesInsideRoom(&pCollisionGrid->pRoomCoords, nX, nY))
		{
			nX -= pCollisionGrid->pRoomCoords.dwSubtilesLeft;
			nY -= pCollisionGrid->pRoomCoords.dwSubtilesTop;

			if (bRemoveOldFlags)
			{
				if (pCollisionGrid->pCollisionMask)
				{
					v11 = D2CMP_10085_GetTileFlagArray(pTiles[i].pTile);

					if (nX > 0)
					{
						nCappedX = nX;
					}
					else
					{
						nCappedX = 0;
					}
					
					if (nY > 0)
					{
						nCappedY = nY;
					}
					else
					{
						nCappedY = 0;
					}

					pTmp = &v11[5 * (nY - nCappedY + 4) - nX];
					nIndex = pCollisionGrid->pRoomCoords.dwSubtilesWidth * nCappedY;

					for (int nYCounter = nCappedY; nYCounter < nY + 5; ++nYCounter)
					{
						pCollisionMask = &pCollisionGrid->pCollisionMask[nIndex];
						for (int nXCounter = nCappedX; nXCounter < nX + 5; ++nXCounter)
						{
							pCollisionMask[nXCounter] = pTmp[nXCounter];
						}
						pTmp -= 5;
						nIndex += pCollisionGrid->pRoomCoords.dwSubtilesWidth;
					}
				}
			}
			else
			{
				sub_6FD411F0(pCollisionGrid, pTiles[i].pTile, nX, nY);
			}

			nFlags = 0;
			if (pTiles[i].dwFlags & 2)
			{
				nFlags = COLLIDE_ALTERNATE_FLOOR;
			}
			if (pTiles[i].dwFlags & 0x40)
			{
				nFlags |= COLLIDE_BLOCK_PLAYER;
			}
			if (pTiles[i].dwFlags & 0x80)
			{
				nFlags |= COLLIDE_BARRIER;
			}

			if (nFlags && pCollisionGrid->pCollisionMask)
			{
				if (nX > 0)
				{
					nCappedX = nX;
				}
				else
				{
					nCappedX = 0;
				}
				
				if (nY > 0)
				{
					nCappedY = nY;
				}
				else
				{
					nCappedY = 0;
				}

				nIndex = pCollisionGrid->pRoomCoords.dwSubtilesWidth * nCappedY;

				for (int nYCounter = nCappedY; nYCounter < nY + 5; ++nYCounter)
				{
					pCollisionMask = &pCollisionGrid->pCollisionMask[nIndex];
					for (int nXCounter = nCappedX; nXCounter < nX + 5; ++nXCounter)
					{
						pCollisionMask[nXCounter] |= nFlags;
					}
					nIndex += pCollisionGrid->pRoomCoords.dwSubtilesWidth;
				}
			}
		}
	}
}

//D2Common.0x6FD41610
void __fastcall COLLISION_FreeRoomCollisionGrid(void* pMemPool, D2RoomStrc* pRoom)
{
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;

	if (pRoom)
	{
		pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid)
		{
			D2_FREE_POOL(pMemPool, pCollisionGrid);
			DUNGEON_SetCollisionGridInRoom(pRoom, NULL);
		}
	}
}

//D2Common.0x6FD41650 (#10118)
uint16_t __stdcall COLLISION_CheckMask(D2RoomStrc* pRoom, int nX, int nY, uint16_t nMask)
{
	return COLLISION_CheckCollisionMask(pRoom, nX, nY, nMask);
}

//D2Common.0x6FD41720 (#10127)
void __stdcall COLLISION_SetMask(D2RoomStrc* pRoom, int nX, int nY, uint16_t nMask)
{
	return COLLISION_SetCollisionMask(pRoom, nX, nY, nMask);
}

//D2Common.0x6FD417F0 (#10123)
void __stdcall COLLISION_ResetMask(D2RoomStrc* pRoom, int nX, int nY, uint16_t nMask)
{
	return COLLISION_ResetCollisionMask(pRoom, nX, nY, nMask);
}

//D2Common.0x6FD418C0 (#10120)
uint16_t __stdcall COLLISION_CheckMaskWithSizeXY(D2RoomStrc* pRoom, int nX, int nY, unsigned int nSizeX, unsigned int nSizeY, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	if ((int)nSizeX <= 1 && (int)nSizeY <= 1)
	{
		return COLLISION_CheckCollisionMask(pRoom, nX, nY, nMask);
	}

	COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, nSizeX, nSizeY);
	return COLLISION_CheckCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);
}

//D2Common.0x6FD41B40
uint16_t __fastcall COLLISION_CheckCollisionMaskForBoundingBox(D2RoomCollisionGridStrc* pCollisionGrid, D2BoundingBoxStrc* pBoundingBox, uint16_t nMask)
{
	const int32_t boxWidth = pBoundingBox->nRight - pBoundingBox->nLeft + 1;
	const int32_t boxHeight = pBoundingBox->nTop - pBoundingBox->nBottom + 1;
	const int32_t nCollisionMaskBeginX = pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft;
	const int32_t nCollisionMaskBeginY = pBoundingBox->nBottom - pCollisionGrid->pRoomCoords.dwSubtilesTop;
	
	uint16_t nResult = 0;
	const uint16_t* pCollisionMaskLine = &pCollisionGrid->pCollisionMask[nCollisionMaskBeginX + nCollisionMaskBeginY * pCollisionGrid->pRoomCoords.dwSubtilesWidth];
	for (int y = 0; y < boxHeight; y++)
	{
		for (int x = 0; x < boxWidth; x++) {
			nResult |= pCollisionMaskLine[x] & nMask;
		}
		pCollisionMaskLine += pCollisionGrid->pRoomCoords.dwSubtilesWidth;
	}
	return nResult;
}

//D2Common.0x6FD41BE0
int __fastcall COLLISION_AdaptBoundingBoxToGrid(D2RoomStrc* pRoom, D2BoundingBoxStrc* pBoundingBox, D2BoundingBoxStrc* pBoundingBoxes)
{
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;
	int nBoundingBoxes = 0;

	pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);

	if (pCollisionGrid && pBoundingBox->nLeft <= pBoundingBox->nRight && pBoundingBox->nBottom <= pBoundingBox->nTop)
	{
		pBoundingBoxes[0].nLeft = pBoundingBox->nLeft;
		pBoundingBoxes[0].nRight = pBoundingBox->nRight;
		pBoundingBoxes[0].nBottom = pBoundingBox->nBottom;
		pBoundingBoxes[0].nTop = pBoundingBox->nTop;

		++nBoundingBoxes;

		if (pBoundingBox->nRight >= pCollisionGrid->pRoomCoords.dwSubtilesLeft + pCollisionGrid->pRoomCoords.dwSubtilesWidth)
		{
			pBoundingBoxes[0].nRight = pCollisionGrid->pRoomCoords.dwSubtilesLeft + pCollisionGrid->pRoomCoords.dwSubtilesWidth - 1;

			pBoundingBoxes[1].nLeft = pBoundingBoxes[0].nRight + 1;
			pBoundingBoxes[1].nRight = pBoundingBox->nRight;
			pBoundingBoxes[1].nBottom = pBoundingBoxes[0].nBottom;
			pBoundingBoxes[1].nTop = pBoundingBoxes[0].nTop;

			++nBoundingBoxes;
		}

		if (pBoundingBox->nTop >= pCollisionGrid->pRoomCoords.dwSubtilesTop + pCollisionGrid->pRoomCoords.dwSubtilesHeight)
		{
			pBoundingBoxes[0].nTop = pCollisionGrid->pRoomCoords.dwSubtilesTop + pCollisionGrid->pRoomCoords.dwSubtilesHeight - 1;

			pBoundingBoxes[2].nLeft = pBoundingBoxes[0].nLeft;
			pBoundingBoxes[2].nRight = pBoundingBoxes[0].nRight;
			pBoundingBoxes[2].nBottom = pBoundingBoxes[0].nTop + 1;
			pBoundingBoxes[2].nTop = pBoundingBox->nTop;

			++nBoundingBoxes;
		}

		return nBoundingBoxes;
	}

	return 0;
}

//D2Common.0x6FD41CA0
uint16_t __fastcall COLLISION_CheckCollisionMaskForBoundingBoxRecursively(D2RoomStrc* pRoom, D2BoundingBoxStrc* pBoundingBox, uint16_t nMask)
{
	if (pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, pBoundingBox->nLeft, pBoundingBox->nBottom))
	{
		if (D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom))
		{
			D2BoundingBoxStrc pBoundingBoxes[3] = {};
			int nBoundingBoxes = COLLISION_AdaptBoundingBoxToGrid(pRoom, pBoundingBox, pBoundingBoxes);
			if (nBoundingBoxes > 0)
			{
				uint16_t nResult = COLLISION_CheckCollisionMaskForBoundingBox(pCollisionGrid, &pBoundingBoxes[0], nMask);

				for (int i = 1; i < nBoundingBoxes; ++i)
				{
					nResult |= COLLISION_CheckCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBoxes[i], nMask);
				}

				return nResult;
			}
		}
	}

	return COLLIDE_MASK_INVALID;
}

//D2Common.0x6FD41DE0 (#10121)
uint16_t __stdcall COLLISION_CheckMaskWithPattern(D2RoomStrc* pRoom, int nX, int nY, int nCollisionPattern, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	switch (nCollisionPattern)
	{
	case COLLISION_PATTERN_NONE:
		return COLLISION_CheckCollisionMask(pRoom, nX, nY, nMask);

	case COLLISION_PATTERN_SMALL_UNIT_PRESENCE:
	case COLLISION_PATTERN_SMALL_PET_PRESENCE:
	case COLLISION_PATTERN_SMALL_NO_PRESENCE:
		return COLLISION_CheckCollisionMaskWithAdjacentCells(pRoom, nX, nY, nMask);

	case COLLISION_PATTERN_BIG_UNIT_PRESENCE:
	case COLLISION_PATTERN_BIG_PET_PRESENCE:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
		return COLLISION_CheckCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);

	default:
		return COLLIDE_ALL_MASK;
	}
}

//D2Common.0x6FD42000
// TODO: v4, v21
uint16_t __fastcall COLLISION_CheckCollisionMaskWithAdjacentCells(D2RoomStrc* pRoom, int nX, int nY, uint16_t nMask)
{
	D2RoomCollisionGridStrc* pRoomCollisionGrid = NULL;
	D2RoomStrc* v4 = NULL;
	uint16_t* pCollisionMask = NULL;
	int v21 = 0;

	v4 = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY);
	if (!v4)
	{
		return nMask;
	}

	pRoomCollisionGrid = DUNGEON_GetCollisionGridFromRoom(v4);
	if (!pRoomCollisionGrid || !pRoomCollisionGrid->pCollisionMask)
	{
		return nMask;
	}

	if (pRoomCollisionGrid->pRoomCoords.dwSubtilesLeft >= nX)
	{
		v21 += 1;
	}

	if (pRoomCollisionGrid->pRoomCoords.dwSubtilesTop >= nY)
	{
		v21 += 2;
	}

	if (pRoomCollisionGrid->pRoomCoords.dwSubtilesLeft + pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth <= nX + 1)
	{
		v21 += 4;
	}

	if (pRoomCollisionGrid->pRoomCoords.dwSubtilesTop + pRoomCollisionGrid->pRoomCoords.dwSubtilesHeight <= nY + 1)
	{
		v21 += 8;
	}

	pCollisionMask = &pRoomCollisionGrid->pCollisionMask[nX + pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth * (nY - pRoomCollisionGrid->pRoomCoords.dwSubtilesTop) - pRoomCollisionGrid->pRoomCoords.dwSubtilesLeft];

	switch (v21)
	{
	case 0:
		return nMask & (*pCollisionMask | pCollisionMask[1] | *(pCollisionMask - 1) | pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth] | pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth]);

	case 1:
		return COLLISION_CheckCollisionMask(v4, nX - 1, nY, nMask) | (nMask & (*pCollisionMask | pCollisionMask[1] | pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth] | pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth]));

	case 2:
		return COLLISION_CheckCollisionMask(v4, nX, nY - 1, nMask) | (nMask & (*pCollisionMask | pCollisionMask[1] | *(pCollisionMask - 1) | pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth]));

	case 3:
		return COLLISION_CheckCollisionMask(v4, nX - 1, nY, nMask) | COLLISION_CheckCollisionMask(v4, nX, nY - 1, nMask) | (nMask & (*pCollisionMask | pCollisionMask[1] | pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth]));

	case 4:
		return COLLISION_CheckCollisionMask(v4, nX + 1, nY, nMask) | nMask & (*pCollisionMask | *(pCollisionMask - 1) | pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth] | pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth]);

	case 6:
		return COLLISION_CheckCollisionMask(v4, nX + 1, nY, nMask) | COLLISION_CheckCollisionMask(v4, nX, nY - 1, nMask) | (nMask & (*pCollisionMask | *(pCollisionMask - 1) | pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth]));

	case 8:
		return COLLISION_CheckCollisionMask(v4, nX, nY + 1, nMask) | (nMask & (*pCollisionMask | pCollisionMask[1] | *(pCollisionMask - 1) | pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth]));

	case 9:
		return COLLISION_CheckCollisionMask(v4, nX - 1, nY, nMask) | COLLISION_CheckCollisionMask(v4, nX, nY + 1, nMask) | (nMask & (*pCollisionMask | pCollisionMask[1] | pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth]));

	case 12:
		return COLLISION_CheckCollisionMask(v4, nX + 1, nY, nMask) | COLLISION_CheckCollisionMask(v4, nX, nY + 1, nMask) | (nMask & (*pCollisionMask | *(pCollisionMask - 1) | pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth]));

	default:
		return nMask;
	}
}

//D2Common.0x6FD42670
uint16_t __fastcall COLLISION_CheckCollisionMask(D2RoomStrc* pRoom, int nX, int nY, uint16_t nMask)
{
	if (pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY))
	{
		D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid && pCollisionGrid->pCollisionMask)
		{
			return pCollisionGrid->pCollisionMask[nX + pCollisionGrid->pRoomCoords.dwSubtilesWidth * (nY - pCollisionGrid->pRoomCoords.dwSubtilesTop) - pCollisionGrid->pRoomCoords.dwSubtilesLeft] & nMask;
		}
	}

	return COLLIDE_MASK_INVALID;
}

//D2Common.0x6FD42740 (#10122)
int __stdcall COLLISION_CheckAnyCollisionWithPattern(D2RoomStrc* pRoom, int nX, int nY, int nCollisionPattern, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	switch (nCollisionPattern)
	{
	case COLLISION_PATTERN_NONE:
		return COLLISION_CheckMask(pRoom, nX, nY, nMask) != 0;

	case COLLISION_PATTERN_SMALL_UNIT_PRESENCE:
	case COLLISION_PATTERN_SMALL_PET_PRESENCE:
	case COLLISION_PATTERN_SMALL_NO_PRESENCE:
		return COLLISION_CheckAnyCollisionWithAdjacentCells(pRoom, nX, nY, nMask);

	case COLLISION_PATTERN_BIG_UNIT_PRESENCE:
	case COLLISION_PATTERN_BIG_PET_PRESENCE:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
		return COLLISION_CheckAnyCollisionForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);

	default:
		return TRUE;
	}
}

//D2Common.0x6FD428D0
BOOL __fastcall COLLISION_CheckAnyCollisionForBoundingBoxRecursively(D2RoomStrc* pRoom, D2BoundingBoxStrc* pBoundingBox, uint16_t nMask)
{
	if (pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, pBoundingBox->nLeft, pBoundingBox->nBottom))
	{
		if (D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom))
		{
			D2BoundingBoxStrc pBoundingBoxes[3] = {};
			int nBoundingBoxes = COLLISION_AdaptBoundingBoxToGrid(pRoom, pBoundingBox, pBoundingBoxes);
			if (nBoundingBoxes > 0 && COLLISION_CheckCollisionMaskForBoundingBox(pCollisionGrid, &pBoundingBoxes[0], nMask) == 0)
			{
				for (int i = 1; i < nBoundingBoxes; ++i)
				{
					if(COLLISION_CheckAnyCollisionForBoundingBoxRecursively(pRoom, &pBoundingBoxes[i], nMask) != 0)
					{
						return TRUE;
					}
				}
				// No collision found!
				return FALSE;
			}
		}
	}
	return TRUE;
}

//D2Common.0x6FD42A30
// TODO: v21
BOOL __fastcall COLLISION_CheckAnyCollisionWithAdjacentCells(D2RoomStrc* pRoom, int nX, int nY, uint16_t nMask)
{
	D2RoomCollisionGridStrc* pRoomCollisionGrid = NULL;
	uint16_t* pCollisionMask = NULL;
	int v21 = 0;

	pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY);

	if (!pRoom)
	{
		return TRUE;
	}

	pRoomCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);	
	if (!pRoomCollisionGrid || !pRoomCollisionGrid->pCollisionMask)
	{
		return TRUE;
	}

	if (pRoomCollisionGrid->pRoomCoords.dwSubtilesLeft >= nX)
	{
		v21 = 1;
	}

	if (pRoomCollisionGrid->pRoomCoords.dwSubtilesTop >= nY)
	{
		v21 += 2;
	}

	if (pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth + pRoomCollisionGrid->pRoomCoords.dwSubtilesLeft <= nX + 1)
	{
		v21 += 4;
	}

	if (pRoomCollisionGrid->pRoomCoords.dwSubtilesTop + pRoomCollisionGrid->pRoomCoords.dwSubtilesHeight <= nY + 1)
	{
		v21 += 8;
	}

	pCollisionMask = &pRoomCollisionGrid->pCollisionMask[nX + pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth * (nY - pRoomCollisionGrid->pRoomCoords.dwSubtilesTop) - pRoomCollisionGrid->pRoomCoords.dwSubtilesLeft];

	switch (v21)
	{
	case 0:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[1] || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth] || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return TRUE;
		}

		return FALSE;

	case 1:
		if (nMask & *pCollisionMask || nMask & pCollisionMask[1] || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth] || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return TRUE;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX - 1, nY, nMask))
		{
			return TRUE;
		}

		return FALSE;

	case 2:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[1] || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return TRUE;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX, nY - 1, nMask))
		{
			return TRUE;
		}

		return FALSE;

	case 3:
		if (nMask & *pCollisionMask || nMask & pCollisionMask[1] || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return TRUE;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX, nY - 1, nMask) || COLLISION_CheckCollisionMask(pRoom, nX - 1, nY, nMask))
		{
			return TRUE;
		}

		return FALSE;

	case 4:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth] || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return TRUE;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX + 1, nY, nMask))
		{
			return TRUE;
		}

		return FALSE;

	case 6:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return TRUE;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX + 1, nY, nMask) || COLLISION_CheckCollisionMask(pRoom, nX, nY - 1, nMask))
		{
			return TRUE;
		}
		
		return FALSE;

	case 8:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[1] || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return TRUE;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX, nY + 1, nMask))
		{
			return TRUE;
		}

		return FALSE;

	case 9:
		if (nMask & *pCollisionMask || nMask & pCollisionMask[1] || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return TRUE;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX - 1, nY, nMask) || COLLISION_CheckCollisionMask(pRoom, nX, nY + 1, nMask))
		{
			return TRUE;
		}

		return FALSE;

	case 12:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return TRUE;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX + 1, nY, nMask) || COLLISION_CheckCollisionMask(pRoom, nX, nY + 1, nMask))
		{
			return TRUE;
		}

		return FALSE;

	default:
		return TRUE;
	}
}

//D2Common.0x6FD43080 (#10119)
uint16_t __stdcall COLLISION_CheckMaskWithSize(D2RoomStrc* pRoom, int nX, int nY, int nUnitSize, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	switch (nUnitSize)
	{
	case COLLISION_UNIT_SIZE_NONE:
	case COLLISION_UNIT_SIZE_POINT:
		return COLLISION_CheckCollisionMask(pRoom, nX, nY, nMask);

	case COLLISION_UNIT_SIZE_SMALL:
		return COLLISION_CheckCollisionMaskWithAdjacentCells(pRoom, nX, nY, nMask);

	case COLLISION_UNIT_SIZE_BIG:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
		return COLLISION_CheckCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);

	default:
		return COLLIDE_ALL_MASK;
	}
}

//D2Common.0x6FD432A0 (#10128)
void __stdcall COLLISION_SetMaskWithSize(D2RoomStrc* pRoom, int nX, int nY, int nUnitSize, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	switch (nUnitSize)
	{
	case COLLISION_UNIT_SIZE_POINT:
		COLLISION_SetCollisionMask(pRoom, nX, nY, nMask);
		break;

	case COLLISION_UNIT_SIZE_SMALL:
		COLLISION_SetCollisionMask(pRoom, nX - 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX + 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY - 1, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY + 1, nMask);
		break;

	case COLLISION_UNIT_SIZE_BIG:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
		COLLISION_SetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);
		break;

	default:
		break;
	}
}

//D2Common.0x6FD434B0
void __fastcall COLLISION_SetCollisionMask(D2RoomStrc* pRoom, int nX, int nY, uint16_t nMask)
{
	if (pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY))
	{
		D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid && pCollisionGrid->pCollisionMask)
		{
			pCollisionGrid->pCollisionMask[nX + pCollisionGrid->pRoomCoords.dwSubtilesWidth * (nY - pCollisionGrid->pRoomCoords.dwSubtilesTop) - pCollisionGrid->pRoomCoords.dwSubtilesLeft] |= nMask;
		}
	}
}

//D2Common.0x6FD43580
void __fastcall COLLISION_SetCollisionMaskForBoundingBoxRecursively(D2RoomStrc* pRoom, D2BoundingBoxStrc* pBoundingBox, uint16_t nMask)
{
	if (pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, pBoundingBox->nLeft, pBoundingBox->nBottom))
	{
		if (D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom))
		{
			D2BoundingBoxStrc pBoundingBoxes[3] = {};
			int nBoundingBoxes = COLLISION_AdaptBoundingBoxToGrid(pRoom, pBoundingBox, pBoundingBoxes);
			if (nBoundingBoxes > 0)
			{
				COLLISION_SetCollisionMaskForBoundingBox(pCollisionGrid, &pBoundingBoxes[0], nMask);

				for (int i = 1; i < nBoundingBoxes; ++i)
				{
					COLLISION_SetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBoxes[i], nMask);
				}
			}
		}
	}
}

//D2Common.0x6FD436F0 (#10130)
void __stdcall COLLISION_SetMaskWithPattern(D2RoomStrc* pRoom, int nX, int nY, int nCollisionPattern, uint16_t nMask)
{
	D2BoundingBoxStrc tBoundingBox = {};

	switch (nCollisionPattern)
	{
	case COLLISION_PATTERN_SMALL_UNIT_PRESENCE:
		COLLISION_SetCollisionMask(pRoom, nX - 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX + 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY - 1, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY + 1, nMask);

		if (nMask)
		{
			COLLISION_SetCollisionMask(pRoom, nX, nY, COLLIDE_UNIT_RELATED);
		}
		break;

	case COLLISION_PATTERN_BIG_UNIT_PRESENCE:
		COLLISION_CreateBoundingBox(&tBoundingBox, nX, nY, 3, 3);
		COLLISION_SetCollisionMaskForBoundingBoxRecursively(pRoom, &tBoundingBox, nMask);

		if (nMask)
		{
			COLLISION_SetCollisionMask(pRoom, nX - 1, nY, COLLIDE_UNIT_RELATED);
			COLLISION_SetCollisionMask(pRoom, nX, nY, COLLIDE_UNIT_RELATED);
			COLLISION_SetCollisionMask(pRoom, nX + 1, nY, COLLIDE_UNIT_RELATED);
			COLLISION_SetCollisionMask(pRoom, nX, nY - 1, COLLIDE_UNIT_RELATED);
			COLLISION_SetCollisionMask(pRoom, nX, nY + 1, COLLIDE_UNIT_RELATED);
		}
		break;

	case COLLISION_PATTERN_SMALL_PET_PRESENCE:
		COLLISION_SetCollisionMask(pRoom, nX - 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX + 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY - 1, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY + 1, nMask);

		if (nMask)
		{
			COLLISION_SetCollisionMask(pRoom, nX, nY, COLLIDE_PET);
		}
		break;

	case COLLISION_PATTERN_BIG_PET_PRESENCE:
		COLLISION_CreateBoundingBox(&tBoundingBox, nX, nY, 3, 3);
		COLLISION_SetCollisionMaskForBoundingBoxRecursively(pRoom, &tBoundingBox, nMask);

		if (nMask)
		{
			COLLISION_SetCollisionMask(pRoom, nX - 1, nY, COLLIDE_PET);
			COLLISION_SetCollisionMask(pRoom, nX, nY, COLLIDE_PET);
			COLLISION_SetCollisionMask(pRoom, nX + 1, nY, COLLIDE_PET);
			COLLISION_SetCollisionMask(pRoom, nX, nY - 1, COLLIDE_PET);
			COLLISION_SetCollisionMask(pRoom, nX, nY + 1, COLLIDE_PET);
		}
		break;

	case COLLISION_PATTERN_SMALL_NO_PRESENCE:
		COLLISION_SetCollisionMask(pRoom, nX - 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX + 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY - 1, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY + 1, nMask);
		break;

	default:
		return;
	}
}

//D2Common.0x6FD439D0 (#10124)
void __stdcall COLLISION_ResetMaskWithSize(D2RoomStrc* pRoom, int nX, int nY, int nUnitSize, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	if (pRoom)
	{
		switch (nUnitSize)
		{
		case 1:
			COLLISION_ResetCollisionMask(pRoom, nX, nY, nMask);
			break;

		case 2:
			COLLISION_ResetCollisionMask(pRoom, nX - 1, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX + 1, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY - 1, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY + 1, nMask);
			break;

		case 3:
			COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
			COLLISION_ResetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);
			break;

		default:
			break;
		}
	}
}

//D2Common.0x6FD43C10
void __fastcall COLLISION_ResetCollisionMask(D2RoomStrc* pRoom, int nX, int nY, uint16_t nMask)
{
	if (pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY))
	{
		D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid && pCollisionGrid->pCollisionMask)
		{
			pCollisionGrid->pCollisionMask[nX + pCollisionGrid->pRoomCoords.dwSubtilesWidth * (nY - pCollisionGrid->pRoomCoords.dwSubtilesTop) - pCollisionGrid->pRoomCoords.dwSubtilesLeft] &= ~nMask;
		}
	}
}

//D2Common.0x6FD43CE0
void __fastcall COLLISION_ResetCollisionMaskForBoundingBoxRecursively(D2RoomStrc* pRoom, D2BoundingBoxStrc* pBoundingBox, uint16_t nMask)
{
	if (pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, pBoundingBox->nLeft, pBoundingBox->nBottom))
	{
		if (D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom))
		{
			D2BoundingBoxStrc pBoundingBoxes[3] = {};
			int nBoundingBoxes = COLLISION_AdaptBoundingBoxToGrid(pRoom, pBoundingBox, pBoundingBoxes);
			if (nBoundingBoxes > 0)
			{
				COLLISION_ResetCollisionMaskForBoundingBox(pCollisionGrid, &pBoundingBoxes[0], nMask);

				for (int i = 1; i < nBoundingBoxes; ++i)
				{
					COLLISION_ResetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBoxes[i], nMask);
				}
			}
		}
	}
}

//D2Common.0x6FD43E60 (#10126)
void __stdcall COLLISION_ResetMaskWithPattern(D2RoomStrc* pRoom, int nX, int nY, int nCollisionPattern, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	if (pRoom)
	{
		switch (nCollisionPattern)
		{
		case COLLISION_PATTERN_SMALL_UNIT_PRESENCE:
			COLLISION_ResetCollisionMask(pRoom, nX - 1, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX + 1, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY - 1, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY + 1, nMask);

			if (nMask)
			{
				COLLISION_ResetCollisionMask(pRoom, nX, nY, COLLIDE_UNIT_RELATED);
			}
			break;

		case COLLISION_PATTERN_BIG_UNIT_PRESENCE:
			COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
			COLLISION_ResetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);

			if (nMask)
			{
				COLLISION_ResetCollisionMask(pRoom, nX - 1, nY, COLLIDE_UNIT_RELATED);
				COLLISION_ResetCollisionMask(pRoom, nX, nY, COLLIDE_UNIT_RELATED);
				COLLISION_ResetCollisionMask(pRoom, nX + 1, nY, COLLIDE_UNIT_RELATED);
				COLLISION_ResetCollisionMask(pRoom, nX, nY - 1, COLLIDE_UNIT_RELATED);
				COLLISION_ResetCollisionMask(pRoom, nX, nY + 1, COLLIDE_UNIT_RELATED);
			}
			break;

		case COLLISION_PATTERN_SMALL_PET_PRESENCE:
			COLLISION_ResetCollisionMask(pRoom, nX - 1, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX + 1, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY - 1, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY + 1, nMask);

			if (nMask)
			{
				COLLISION_ResetCollisionMask(pRoom, nX, nY, COLLIDE_PET);
			}
			break;

		case COLLISION_PATTERN_BIG_PET_PRESENCE:
			COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
			COLLISION_ResetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);

			if (nMask)
			{
				COLLISION_ResetCollisionMask(pRoom, nX - 1, nY, COLLIDE_PET);
				COLLISION_ResetCollisionMask(pRoom, nX, nY, COLLIDE_PET);
				COLLISION_ResetCollisionMask(pRoom, nX + 1, nY, COLLIDE_PET);
				COLLISION_ResetCollisionMask(pRoom, nX, nY - 1, COLLIDE_PET);
				COLLISION_ResetCollisionMask(pRoom, nX, nY + 1, COLLIDE_PET);
			}
			break;

		case COLLISION_PATTERN_SMALL_NO_PRESENCE:
			COLLISION_ResetCollisionMask(pRoom, nX - 1, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX + 1, nY, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY - 1, nMask);
			COLLISION_ResetCollisionMask(pRoom, nX, nY + 1, nMask);
			break;

		default:
			return;
		}
	}
}

//D2Common.0x6FD44140 (#10125)
void __stdcall COLLISION_ResetMaskWithSizeXY(D2RoomStrc* pRoom, int nX, int nY, unsigned int nSizeX, unsigned int nSizeY, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	if ((int)nSizeX > 0 && (int)nSizeY > 0)
	{
		if (nSizeX != 1 || nSizeY != 1)
		{
			COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, nSizeX, nSizeY);
			COLLISION_ResetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);
		}
		else
		{
			COLLISION_ResetCollisionMask(pRoom, nX, nY, nMask);
		}
	}
}

//D2Common.0x6FD44370
void __fastcall COLLISION_ResetCollisionMaskForBoundingBox(D2RoomCollisionGridStrc* pCollisionGrid, D2BoundingBoxStrc* pBoundingBox, uint16_t nMask)
{
	const int32_t boxWidth = pBoundingBox->nRight - pBoundingBox->nLeft + 1;
	const int32_t boxHeight = pBoundingBox->nTop - pBoundingBox->nBottom + 1;
	const int32_t nCollisionMaskBeginX = pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft;
	const int32_t nCollisionMaskBeginY = pBoundingBox->nBottom - pCollisionGrid->pRoomCoords.dwSubtilesTop;

	uint16_t* pCollisionMaskLine = &pCollisionGrid->pCollisionMask[nCollisionMaskBeginX + nCollisionMaskBeginY * pCollisionGrid->pRoomCoords.dwSubtilesWidth];
	for (int y = 0; y < boxHeight; y++)
	{
		for (int x = 0; x < boxWidth; x++) {
			pCollisionMaskLine[x] &= ~nMask;
		}
		pCollisionMaskLine += pCollisionGrid->pRoomCoords.dwSubtilesWidth;
	}
}

//D2Common.0x6FD443E0 (#10129)
void __stdcall COLLISION_SetMaskWithSizeXY(D2RoomStrc* pRoom, int nX, int nY, unsigned int nSizeX, unsigned int nSizeY, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	if (nSizeX != 1 || nSizeY != 1)
	{
		COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, nSizeX, nSizeY);
		COLLISION_SetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);
	}
	else
	{
		COLLISION_SetCollisionMask(pRoom, nX, nY, nMask);
	}
}

//D2Common.0x6FD44600
void __fastcall COLLISION_SetCollisionMaskForBoundingBox(D2RoomCollisionGridStrc* pCollisionGrid, D2BoundingBoxStrc* pBoundingBox, uint16_t nMask)
{
	const int32_t boxWidth = pBoundingBox->nRight - pBoundingBox->nLeft + 1;
	const int32_t boxHeight = pBoundingBox->nTop - pBoundingBox->nBottom + 1;
	const int32_t nCollisionMaskBeginX = pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft;
	const int32_t nCollisionMaskBeginY = pBoundingBox->nBottom - pCollisionGrid->pRoomCoords.dwSubtilesTop;

	uint16_t* pCollisionMaskLine = &pCollisionGrid->pCollisionMask[nCollisionMaskBeginX + nCollisionMaskBeginY * pCollisionGrid->pRoomCoords.dwSubtilesWidth];
	for (int y = 0; y < boxHeight; y++)
	{
		for (int x = 0; x < boxWidth; x++) {
			pCollisionMaskLine[x] |= nMask;
		}
		pCollisionMaskLine += pCollisionGrid->pRoomCoords.dwSubtilesWidth;
	}
}

//D2Common.0x6FD44660 (#10131)
uint16_t __fastcall COLLISION_TryMoveUnitCollisionMask(D2RoomStrc* pRoom, int nX1, int nY1, int nX2, int nY2, int nUnitSize, uint16_t nCollisionMask, uint16_t nMoveConditionMask)
{
	COLLISION_ResetMaskWithSize(pRoom, nX1, nY1, nUnitSize, nCollisionMask);
	
	const uint16_t nCollidedWithMask = COLLISION_CheckMaskWithSize(pRoom, nX2, nY2, nUnitSize, nMoveConditionMask);

	if (nCollidedWithMask & (COLLIDE_BLOCK_PLAYER|COLLIDE_BARRIER))
	{
		COLLISION_SetMaskWithSize(pRoom, nX1, nY1, nUnitSize, nCollisionMask);
	}
	else
	{
		COLLISION_SetMaskWithSize(pRoom, nX2, nY2, nUnitSize, nCollisionMask);
	}

	return nCollidedWithMask;
}

//D2Common.0x6FD44910
void __fastcall COLLISION_CreateBoundingBox(D2BoundingBoxStrc* pBoundingBox, int nCenterX, int nCenterY, unsigned int nSizeX, unsigned int nSizeY)
{
	pBoundingBox->nLeft = nCenterX - (nSizeX / 2);
	pBoundingBox->nRight = pBoundingBox->nLeft + nSizeX - 1;
	pBoundingBox->nBottom = nCenterY - (nSizeY / 2);
	pBoundingBox->nTop = pBoundingBox->nBottom + nSizeY - 1;
}

//D2Common.0x6FD44950 (#10132)
uint16_t __fastcall COLLISION_TryTeleportUnitCollisionMask(D2RoomStrc* pRoom, int nX1, int nY1, int nX2, int nY2, int nCollisionPattern, uint16_t nFootprintCollisionMask, uint16_t nMoveConditionMask)
{
	COLLISION_ResetMaskWithPattern(pRoom, nX1, nY1, nCollisionPattern, nFootprintCollisionMask);

	if (const uint16_t nCollidedWithMask = COLLISION_CheckMaskWithPattern(pRoom, nX2, nY2, nCollisionPattern, nMoveConditionMask))
	{
		COLLISION_SetMaskWithPattern(pRoom, nX1, nY1, nCollisionPattern, nFootprintCollisionMask);
		return nCollidedWithMask;
	}
	else
	{
		COLLISION_SetMaskWithPattern(pRoom, nX2, nY2, nCollisionPattern, nFootprintCollisionMask);
		return 0;
	}

}

//D2Common.0x6FD44BB0
uint16_t __fastcall COLLISION_ForceTeleportUnitCollisionMaskAndGetCollision(D2RoomStrc* pRoom1, int nX1, int nY1, D2RoomStrc* pRoom2, int nX2, int nY2, int nUnitSize, uint16_t nFootprintCollisionMask, uint16_t nMoveConditionMask)
{
	COLLISION_ResetMaskWithSize(pRoom1, nX1, nY1, nUnitSize, nFootprintCollisionMask);

	const uint16_t nCollidedWithMask = COLLISION_CheckMaskWithSize(pRoom2, nX2, nY2, nUnitSize, nMoveConditionMask);

	COLLISION_SetMaskWithSize(pRoom2, nX2, nY2, nUnitSize, nFootprintCollisionMask);

	return nCollidedWithMask;
}

//D2Common.0x6FD44E00
uint16_t __fastcall COLLISION_TeleportUnitCollisionMask(D2RoomStrc* pRoom1, int nX1, int nY1, D2RoomStrc* pRoom2, int nX2, int nY2, int nUnitSize, uint16_t nMask)
{
	COLLISION_ResetMaskWithSize(pRoom1, nX1, nY1, nUnitSize, nMask);
	COLLISION_SetMaskWithSize(pRoom2, nX2, nY2, nUnitSize, nMask);
	return 0;
}

//1.10f: D2Common.0x6FD44FF0
//1.13c: D2Common.0x6FD9D5C0
int __fastcall COLLISION_TrySetUnitCollisionMask(D2RoomStrc* pRoom1, int nX1, int nY1, D2RoomStrc* pRoom2, int nX2, int nY2, int nCollisionPattern, uint16_t nFootprintCollisionMask, uint16_t nMoveConditionMask)
{
	if (pRoom1)
	{
		COLLISION_ResetMaskWithPattern(pRoom1, nX1, nY1, nCollisionPattern, nFootprintCollisionMask);
	}

	if (const uint16_t nCollidedWithMask = COLLISION_CheckMaskWithPattern(pRoom2, nX2, nY2, nCollisionPattern, nMoveConditionMask))
	{
		if (pRoom1)
		{
			COLLISION_SetMaskWithPattern(pRoom1, nX1, nY1, nCollisionPattern, nFootprintCollisionMask);
		}
		return nCollidedWithMask;
	}
	else
	{
		COLLISION_SetMaskWithPattern(pRoom2, nX2, nY2, nCollisionPattern, nFootprintCollisionMask);
		return COLLIDE_NONE;
	}
}

//D2Common.0x6FD451D0 (#10133)
void __fastcall COLLISION_SetUnitCollisionMask(D2RoomStrc* pRoom1, int nX1, int nY1, D2RoomStrc* pRoom2, int nX2, int nY2, int nCollisionPattern, uint16_t nCollisionMask)
{
	if (pRoom1)
	{
		COLLISION_ResetMaskWithPattern(pRoom1, nX1, nY1, nCollisionPattern, nCollisionMask);
		COLLISION_SetMaskWithPattern(pRoom2, nX2, nY2, nCollisionPattern, nCollisionMask);
	}
}

//D2Common.0x6FD45210 (#11263)
//This is basicly an implementation of Bresenham's line algorithm over rooms.
BOOL __stdcall COLLISION_RayTrace(D2RoomStrc* pRoom, D2CoordStrc* pBeginCoord, D2CoordStrc* pEndCoord, uint16_t nCollisionMask)
{
	const int nBeginX = pBeginCoord->nX;
	const int nBeginY = pBeginCoord->nY;
	const int nLineEndX = pEndCoord->nX;
	const int nLineEndY = pEndCoord->nY;

	D2CoordStrc tCurrentCoord;
	tCurrentCoord.nX = nBeginX;
	tCurrentCoord.nY = nBeginY;

	if (!pRoom)
	{
		*pEndCoord = tCurrentCoord;
		return TRUE;
	}
	
	int nXDirection = 1;
	int nYDirection = 1;
	int nDeltaX = nLineEndX - nBeginX;
	int nDeltaY = nLineEndY - nBeginY;

	if (nLineEndX - tCurrentCoord.nX < 0)
	{
		nDeltaX = tCurrentCoord.nX - nLineEndX;
		nXDirection = -1;
	}
	if (nDeltaY < 0)
	{
		nDeltaY = -nDeltaY;
		nYDirection = -1;
	}

	pRoom = DUNGEON_GetRoomAtPosition(pRoom, tCurrentCoord.nX, tCurrentCoord.nY);

	if (!(pRoom && DungeonTestRoomGame(pRoom, tCurrentCoord.nX, tCurrentCoord.nY)))
	{
		*pEndCoord = tCurrentCoord;
		return TRUE;
	}
	
	int nDeviation = 0;

	if (nDeltaX == 0 && nDeltaY == 0) // coords Begin == End
	{
		D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		const size_t nMaskIdx = (tCurrentCoord.nX - pRoom->nSubtileX) + (tCurrentCoord.nY - pRoom->nSubtileY) * pCollisionGrid->pRoomCoords.dwSubtilesWidth;
		if ((nCollisionMask & pCollisionGrid->pCollisionMask[nMaskIdx]) != 0)
		{
			*pEndCoord = tCurrentCoord;
			return TRUE;
		}
		return FALSE;
	}
	else if (nDeltaX == 0 && nDeltaY != 0) // Vertical path
	{
		int nRemainingStepsUntilNextRoomY = nDeltaY + 1;
		int nRemainingStepsInRoomY = nRemainingStepsUntilNextRoomY;

		while (1)
		{
			if (nYDirection <= 0)
			{
				if (pEndCoord->nY < pRoom->nSubtileY)
				{
					nRemainingStepsInRoomY = tCurrentCoord.nY - pRoom->nSubtileY + 1;
					nRemainingStepsUntilNextRoomY -= nRemainingStepsInRoomY;
				}
				else
				{
					nRemainingStepsUntilNextRoomY = 0;
				}
			}
			else
			{
				const int nRoomEndSubtileY = pRoom->nSubtileY + pRoom->nSubtileHeight;
				if (pEndCoord->nY >= nRoomEndSubtileY)
				{
					nRemainingStepsInRoomY = nRoomEndSubtileY - tCurrentCoord.nY;
					nRemainingStepsUntilNextRoomY -= nRemainingStepsInRoomY;
				}
				else
				{
					nRemainingStepsUntilNextRoomY = 0;
				}
			}

			D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
			const size_t nMaskIdx = (tCurrentCoord.nX - pRoom->nSubtileX) + (tCurrentCoord.nY - pRoom->nSubtileY) * pCollisionGrid->pRoomCoords.dwSubtilesWidth;
			const uint16_t* pMaskIt = &pCollisionGrid->pCollisionMask[nMaskIdx];
			const uint16_t* pMaskEnd = &pCollisionGrid->pCollisionMask[nMaskIdx + nRemainingStepsInRoomY * nYDirection * pRoom->nSubtileWidth];

			if (pMaskIt != pMaskEnd)
			{
				for (; pMaskIt != pMaskEnd; pMaskIt += nYDirection * pRoom->nSubtileWidth)
				{
					if ((*pMaskIt & nCollisionMask) != 0)
					{
						*pEndCoord = tCurrentCoord;
						return TRUE;
					}
					tCurrentCoord.nY += nYDirection;
				}
			}

			if (nRemainingStepsUntilNextRoomY == 0)
				return FALSE;

			// Go to next room
			pRoom = DUNGEON_GetRoomAtPosition(pRoom, tCurrentCoord.nX, tCurrentCoord.nY);
			if (!(pRoom && DungeonTestRoomGame(pRoom, tCurrentCoord.nX, tCurrentCoord.nY)))
			{
				*pEndCoord = tCurrentCoord;
				return TRUE;
			}

			nRemainingStepsInRoomY = nRemainingStepsUntilNextRoomY;
		}
	}
	else if (nDeltaX != 0 && nDeltaY == 0) // Horizontal path
	{
		int nRemainingStepsUntilNextRoomX = nDeltaX + 1;
		int nRemainingStepsInRoomX = nRemainingStepsUntilNextRoomX;
		while(true)
		{
			if (nXDirection <= 0)
			{
				if (nLineEndX < pRoom->nSubtileX)
				{
					nRemainingStepsInRoomX = tCurrentCoord.nX - pRoom->nSubtileX + 1;
					nRemainingStepsUntilNextRoomX -= nRemainingStepsInRoomX;
				}
				else
				{
					nRemainingStepsUntilNextRoomX = 0;
				}
			}
			else
			{
				const int nRoomEndSubtileX = pRoom->nSubtileX + pRoom->nSubtileWidth;
				if (nLineEndX >= nRoomEndSubtileX)
				{
					nRemainingStepsInRoomX = nRoomEndSubtileX - tCurrentCoord.nX;
					nRemainingStepsUntilNextRoomX -= nRemainingStepsInRoomX;
				}
				else
				{
					nRemainingStepsUntilNextRoomX = 0;
				}
			}

			D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
			const size_t nMaskIdx = (tCurrentCoord.nX - pRoom->nSubtileX) + (tCurrentCoord.nY - pRoom->nSubtileY) * pCollisionGrid->pRoomCoords.dwSubtilesWidth;
			const uint16_t* pMaskIt = &pCollisionGrid->pCollisionMask[nMaskIdx];
			const uint16_t* pMaskEnd = &pCollisionGrid->pCollisionMask[nMaskIdx + nRemainingStepsInRoomX * nXDirection];

			for (; pMaskIt != pMaskEnd; pMaskIt += nXDirection)
			{
				if ((*pMaskIt & nCollisionMask) != 0)
				{
					*pEndCoord = tCurrentCoord;
					return TRUE;
				}
				tCurrentCoord.nX += nXDirection;
			}

			if (!nRemainingStepsUntilNextRoomX)
				return FALSE;

			// Go to next room
			pRoom = DUNGEON_GetRoomAtPosition(pRoom, tCurrentCoord.nX, tCurrentCoord.nY);
			if (!(pRoom && DungeonTestRoomGame(pRoom, tCurrentCoord.nX, tCurrentCoord.nY)))
			{
				*pEndCoord = tCurrentCoord;
				return TRUE;
			}

			nRemainingStepsInRoomX = nRemainingStepsUntilNextRoomX;
		}
	}
	else if (nDeltaX < nDeltaY)
	{
		const int nStopY = nLineEndY + nYDirection;
		while (1)
		{
			const int nRoomEndSubtileX = pRoom->nSubtileX + pRoom->nSubtileWidth;
			const int nRoomEndSubtileY = pRoom->nSubtileY + pRoom->nSubtileHeight;

			const int nRoomLastX = nXDirection >= 0 ? nRoomEndSubtileX : (pRoom->nSubtileX - 1);
			const int nRoomLastY = nYDirection >= 0 ? nRoomEndSubtileY : (pRoom->nSubtileY - 1);

			if (tCurrentCoord.nY >= nRoomEndSubtileY)
			{
				*pEndCoord = tCurrentCoord;
				return TRUE;
			}

			D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
			const size_t nMaskIdx = (tCurrentCoord.nX - pRoom->nSubtileX) + (tCurrentCoord.nY - pRoom->nSubtileY) * pCollisionGrid->pRoomCoords.dwSubtilesWidth;
			const uint16_t* pMaskIt = &pCollisionGrid->pCollisionMask[nMaskIdx];
			
			while (1)
			{
				if ((*pMaskIt & nCollisionMask) != 0)
				{
					*pEndCoord = tCurrentCoord;
					return TRUE;
				}
				tCurrentCoord.nY += nYDirection;
				nDeviation += nDeltaX;
				if (nDeviation >= nDeltaY)
				{
					nDeviation = nDeviation - nDeltaY;
					tCurrentCoord.nX += nXDirection;
					pMaskIt += nXDirection;
				}
				pMaskIt += nYDirection * pRoom->nSubtileWidth;
				if (tCurrentCoord.nX == nRoomLastX || tCurrentCoord.nY == nRoomLastY)
					break;
				if (tCurrentCoord.nY == nStopY)
					return FALSE;
			}
			if (tCurrentCoord.nY == nStopY)
				return FALSE;

			// Go to next room
			pRoom = DUNGEON_GetRoomAtPosition(pRoom, tCurrentCoord.nX, tCurrentCoord.nY);
			if (!(pRoom && DungeonTestRoomGame(pRoom, tCurrentCoord.nX, tCurrentCoord.nY)))
			{
				*pEndCoord = tCurrentCoord;
				return TRUE;
			}
		}
	}
	else // nDeltaX > nDeltaY
	{
		const int nStopX = nLineEndX + nXDirection;
		while (1)
		{
			const int nRoomEndSubtileX = pRoom->nSubtileX + pRoom->nSubtileWidth;
			const int nRoomEndSubtileY = pRoom->nSubtileY + pRoom->nSubtileHeight;

			const int nRoomLastX = nXDirection >= 0 ? nRoomEndSubtileX : (pRoom->nSubtileX - 1);
			const int nRoomLastY = nYDirection >= 0 ? nRoomEndSubtileY : (pRoom->nSubtileY - 1);

			if (tCurrentCoord.nY >= nRoomEndSubtileY)
			{
				*pEndCoord = tCurrentCoord;
				return TRUE;
			}

			D2RoomCollisionGridStrc* pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
			const size_t nMaskIdx =  (tCurrentCoord.nX - pRoom->nSubtileX) + (tCurrentCoord.nY - pRoom->nSubtileY) * pCollisionGrid->pRoomCoords.dwSubtilesWidth;
			const uint16_t* pMaskIt = &pCollisionGrid->pCollisionMask[nMaskIdx];

			while (1)
			{
				if ((*pMaskIt & nCollisionMask) != 0)
				{
					*pEndCoord = tCurrentCoord;
					return TRUE;
				}

				tCurrentCoord.nX += nXDirection;
				nDeviation += nDeltaY;
				if (nDeviation >= nDeltaX)
				{
					nDeviation -= nDeltaX;
					pMaskIt += nYDirection * pRoom->nSubtileWidth;
					tCurrentCoord.nY += nYDirection;
				}
				pMaskIt += nXDirection;
				if (tCurrentCoord.nX == nRoomLastX || tCurrentCoord.nY == nRoomLastY)
					break;
				if (tCurrentCoord.nX == nStopX)
					return FALSE;
			}
			if (tCurrentCoord.nX == nStopX)
				return FALSE;

			// Go to next room
			pRoom = DUNGEON_GetRoomAtPosition(pRoom, tCurrentCoord.nX, tCurrentCoord.nY);
			if (!(pRoom && DungeonTestRoomGame(pRoom, tCurrentCoord.nX, tCurrentCoord.nY)))
			{
				*pEndCoord = tCurrentCoord;
				return TRUE;
			}
		}
	}
	return FALSE;

}

//D2Common.0x6FD459D0 (#10135)
D2RoomStrc* __stdcall COLLISION_GetFreeCoordinatesWithMaxDistance(D2RoomStrc* pRoom, D2CoordStrc* pSpawnPoint, int nUnitSize, unsigned int nMask, BOOL bAllowNeighboorRooms, int nMaxDistance)
{
	return COLLISION_GetFreeCoordinatesImpl(pRoom, pSpawnPoint, NULL, nUnitSize, nMask, nMask, bAllowNeighboorRooms, nMaxDistance, 1);
}

//D2Common.0x6FD45A00
// TODO: Names, vars
D2RoomStrc* __fastcall COLLISION_GetFreeCoordinatesImpl(D2RoomStrc* pRoom, D2CoordStrc* ptSpawnPoint, D2CoordStrc* pFieldCoord, int nUnitSize, unsigned int nMask, unsigned int nFieldMask, BOOL bAllowNeighboorRooms, int nMaxDistance, int nPosIncrementValue)
{
	D2FieldStrc* pField = NULL;
	D2RoomStrc* v10 = NULL;
	D2RoomStrc* pTemp = NULL;
	int nSpawnX = 0;
	int nSpawnY = 0;
	int v15; // ebx@13
	int v17; // edi@15
	int v33; // ecx@68
	int v34; // eax@70
	int v35; // eax@72
	int v37; // ebx@77
	int v39; // edi@77
	int v48; // ecx@106
	int v49; // eax@108
	int v50; // eax@110
	int v59; // [sp+14h] [bp-8Ch]@16
	int v60; // [sp+14h] [bp-8Ch]@78
	int v61; // [sp+18h] [bp-88h]@14
	int v62; // [sp+18h] [bp-88h]@80
	int v63; // [sp+1Ch] [bp-84h]@11
	int v64; // [sp+20h] [bp-80h]@14
	int v65; // [sp+20h] [bp-80h]@80
	int v66; // [sp+24h] [bp-7Ch]@16
	int v67; // [sp+24h] [bp-7Ch]@78
	int v70; // [sp+30h] [bp-70h]@12
	int v71; // [sp+34h] [bp-6Ch]@12
	int v72; // [sp+38h] [bp-68h]@12
	int nX; // [sp+40h] [bp-60h]@12
	int nY; // [sp+48h] [bp-58h]@12
	int v77; // [sp+4Ch] [bp-54h]@12
	int v78; // [sp+50h] [bp-50h]@12

	v10 = pRoom;

	D2_ASSERT(ptSpawnPoint);

	nSpawnX = ptSpawnPoint->nX;
	nSpawnY = ptSpawnPoint->nY;

	if (pFieldCoord)
	{
		pField = DATATBLS_AllocField();
		D2_ASSERT(pField);

		DATATBLS_SetFieldCoordinates(pField, pFieldCoord->nX, pFieldCoord->nY);
	}

	v10 = COLLISION_GetRoomBySubTileCoordinates(v10, ptSpawnPoint->nX, ptSpawnPoint->nY);

	if (v10 && !COLLISION_CheckMaskWithSize(v10, ptSpawnPoint->nX, ptSpawnPoint->nY, nUnitSize, nMask))
	{
		if (!pFieldCoord || !D2Common_11099(pField, v10, ptSpawnPoint->nX, ptSpawnPoint->nY, nFieldMask))
		{
			if (pField)
			{
				DATATBLS_FreeField(pField);
			}

			return v10;
		}
	}

	v63 = -1;
	if (nMaxDistance > 1)
	{
		v78 = -2;
		v72 = 2;
		v70 = nSpawnY + 1;
		v71 = nSpawnX;
		nY = nSpawnY - 1;
		nX = nSpawnX;
		v77 = 1;

		do
		{
			v15 = nY;
			v63 = -1;
			if (nY <= v70)
			{
				v61 = nX - nSpawnX - 1;
				v64 = v77;
				do
				{
					v17 = nX - 1;
					if (v17 <= v71 + 1)
					{
						v59 = v17 - nSpawnX;
						v66 = nSpawnX - v17;
						do
						{
							if (v10)
							{
								pTemp = COLLISION_GetRoomBySubTileCoordinates(v10, v17, v15);

								if (pTemp)
								{
									v10 = pTemp;

									if (!COLLISION_CheckMaskWithSize(pTemp, v17, v15, nUnitSize, nMask) && (!pFieldCoord || !D2Common_11099(pField, pTemp, v17, v15, nFieldMask)))
									{
										if (v59 < 0)
										{
											v33 = v66;
										}
										else
										{
											v33 = v59;
										}

										if (v61 < 0)
										{
											v34 = v64;
										}
										else
										{
											v34 = v61;
										}

										v35 = v33 + v34;
										if (v63 == -1 || v35 < v63)
										{
											v63 = v35;
											ptSpawnPoint->nX = v17;
											ptSpawnPoint->nY = v15;
										}
									}
								}
							}

							v17 += v72;
							v59 += v72;
							v66 += v78;
						}
						while (v17 <= v71 + 1);
					}

					v15 += nPosIncrementValue;
					v61 += nPosIncrementValue;
					v64 -= nPosIncrementValue;
				}
				while (v15 <= v70);
			}

			v37 = nX;
			if (nX <= v71)
			{
				v67 = v77 - 1;
				v60 = -nSpawnX + nX;
				do
				{
					v39 = nY;
					if (v39 <= v70)
					{
						v62 = v39 - nSpawnY;
						v65 = nSpawnY - v39;
						do
						{
							if (v10)
							{
								pTemp = COLLISION_GetRoomBySubTileCoordinates(v10, v37, v39);

								if (pTemp)
								{
									v10 = pTemp;

									if (!COLLISION_CheckMaskWithSize(pTemp, v37, v39, nUnitSize, nMask) && (!pFieldCoord || !D2Common_11099(pField, pTemp, v37, v39, nFieldMask)))
									{
										if (v60 < 0)
										{
											v48 = v67;
										}
										else
										{
											v48 = v60;
										}

										if (v62 < 0)
										{
											v49 = v65;
										}
										else
										{
											v49 = v62;
										}

										v50 = v48 + v49;
										if (v63 == -1 || v50 < v63)
										{
											v63 = v50;
											ptSpawnPoint->nX = v37;
											ptSpawnPoint->nY = v39;
										}
									}
								}
							}

							v39 += v72;
							v65 += v78;
							v62 += v72;
						}
						while (v39 <= v70);
					}

					v37 += nPosIncrementValue;
					v60 += nPosIncrementValue;
					v67 -= nPosIncrementValue;
				}
				while (v37 <= v71);
			}

			if (v63 != -1)
			{
				break;
			}

			nX -= nPosIncrementValue;
			nY -= nPosIncrementValue;
			v71 += nPosIncrementValue;
			v72 += 2 * nPosIncrementValue;
			v78 -= 2 * nPosIncrementValue;
			v77 += nPosIncrementValue;
			v70 += nPosIncrementValue;
		}
		while (v71 + 1 - nSpawnX < nMaxDistance);
	}

	if (pField)
	{
		DATATBLS_FreeField(pField);
	}

	if (v63 == -1)
	{
		if (bAllowNeighboorRooms)
		{
			return COLLISION_GetRoomBySubTileCoordinates(v10, ptSpawnPoint->nX, ptSpawnPoint->nY);
		}

		return 0;
	}

	return COLLISION_GetRoomBySubTileCoordinates(v10, ptSpawnPoint->nX, ptSpawnPoint->nY);
}

//D2Common.0x6FD46280 (#10134)
D2RoomStrc* __stdcall COLLISION_GetFreeCoordinates(D2RoomStrc* pRoom, D2CoordStrc* pSpawnPoint, int nUnitSize, unsigned int nMask, BOOL bAllowNeighboorRooms)
{
	return COLLISION_GetFreeCoordinatesImpl(pRoom, pSpawnPoint, NULL, nUnitSize, nMask, nMask, bAllowNeighboorRooms, 50, 1);
}

//D2Common.0x6FD462B0 (#10137)
D2RoomStrc* __stdcall COLLISION_GetFreeCoordinatesEx(D2RoomStrc* pRoom, D2CoordStrc* pSpawnPoint, int nUnitSize, unsigned int nMask, int nPosIncrementValue)
{
	return COLLISION_GetFreeCoordinatesImpl(pRoom, pSpawnPoint, NULL, nUnitSize, nMask, nMask, FALSE, 50, nPosIncrementValue);
}

//D2Common.0x6FD462E0 (#10138)
D2RoomStrc* __stdcall COLLISION_GetFreeCoordinatesWithField(D2RoomStrc* pRoom, D2CoordStrc* pSpawnPoint, D2CoordStrc* pFieldCoord, int nUnitSize, unsigned int nMask, unsigned int nFieldMask, BOOL bAllowNeighboorRooms)
{
	return COLLISION_GetFreeCoordinatesImpl(pRoom, pSpawnPoint, pFieldCoord, nUnitSize, nMask, nFieldMask, bAllowNeighboorRooms, 50, 1);
}

////D2Common.0x6FD46310) --------------------------------------------------------
//void __fastcall D2Common_10136(D2RoomStrc* pRoom, D2CoordStrc* pCoord, int a3, int nCollisionType, D2RoomStrc** ppRoom)
//{
////	int v6; // eax@1
////	int v8; // ecx@1
////	signed int v9; // edx@1
////	signed int v10; // ecx@1
////	int v11; // ebx@3
////	int v12; // eax@4
////	D2RoomStrc *v13; // edi@4
////	bool v14; // sf@4
////	unsigned __int8 v15; // of@4
////	int v16; // esi@6
////	int v17; // eax@9
////	unsigned int v18; // edx@12
////	D2RoomStrc *v19; // eax@13
////	int v20; // ecx@16
////	int v21; // ebp@24
////	D2RoomStrc *v22; // ebx@25
////	int v24; // eax@25
////	int v25; // esi@27
////	int v26; // eax@29
////	unsigned int v27; // edx@32
////	int v28; // eax@33
////	int v29; // ecx@36
////	int v30; // esi@42
////	int v31; // edi@42
////	unsigned int v32; // eax@42
////	D2RoomStrc *v33; // eax@43
////	D2RoomCollisionGridStrc *v34; // eax@44
////	uint16_t *v35; // ecx@45
////	signed __int16 v36; // ax@46
////	int v37; // edi@48
////	int v38; // esi@48
////	D2RoomStrc *v39; // [sp+10h] [bp-68h]@4
////	signed int v41; // [sp+18h] [bp-60h]@1
////	int pNumRooms; // [sp+1Ch] [bp-5Ch]@12
////	unsigned int v43; // [sp+20h] [bp-58h]@32
////	int v44; // [sp+24h] [bp-54h]@32
////	int v46; // [sp+2Ch] [bp-4Ch]@1
////	D2RoomStrc **pppRoom; // [sp+30h] [bp-48h]@12
////	int v48; // [sp+34h] [bp-44h]@24
////	int v49; // [sp+38h] [bp-40h]@2
////	int v50; // [sp+3Ch] [bp-3Ch]@3
////	int v51; // [sp+40h] [bp-38h]@1
////	int v52; // [sp+44h] [bp-34h]@1
////	D2DrlgCoordStrc pBoundingBox; // [sp+48h] [bp-30h]@48
////	D2DrlgCoordsStrc pDrlgCoords; // [sp+58h] [bp-20h]@1
////
////	v6 = pCoord->nX;
////	v8 = pCoord->nY;
////	v51 = v6;
////	v52 = v8;
////
////	DUNGEON_GetRoomCoordinates(pRoom, &pDrlgCoords);
////
////	v9 = 1;
////	v10 = -1;
////	v46 = 1;
////	v41 = -1;
////
////	while (v10 > -50)
////	{
////		v49 = v10;
////		if (v10 < v9)
////		{
////			v11 = v10 + v52;
////			v50 = v10 + v52;
////			while (1)
////			{
////				v12 = pDrlgCoords.dwSubtilesTop;
////				v13 = pRoom;
////				v15 = __OFSUB__(v11, pDrlgCoords.dwSubtilesTop);
////				v14 = v11 - pDrlgCoords.dwSubtilesTop < 0;
////				v39 = pRoom;
////				pCoord->nY = v11;
////
////				if (!(v14 ^ v15) && v11 < v12 + pDrlgCoords.dwSubtilesHeight)
////				{
////					goto LABEL_23;
////				}
////
////				v16 = pCoord->nX;
////				if (pRoom)
////				{
////					v39 = COLLISION_GetRoomBySubTileCoordinates(pRoom, v16, v11);
////					v10 = v41;
////					v13 = v39;
////LABEL_23:
////					if (v13)
////					{
////						v48 = v10;
////						v21 = v10 + v51;
////						while (1)
////						{
////							v22 = v13;
////							DUNGEON_GetRoomCoordinates(v13, &pDrlgCoords);
////							v24 = pDrlgCoords.dwSubtilesLeft;
////							v15 = __OFSUB__(v21, pDrlgCoords.dwSubtilesLeft);
////							v14 = v21 - pDrlgCoords.dwSubtilesLeft < 0;
////							pCoord->nX = v21;
////							if (!(v14 ^ v15))
////							{
////								if (v21 < v24 + pDrlgCoords.dwSubtilesWidth)
////								{
////									if (!v22)
////									{
////										goto LABEL_51;
////									}
////
////									goto LABEL_END;
////								}
////							}
////							v25 = pCoord->nY;
////
////
////							v22 = COLLISION_GetRoomBySubTileCoordinates(v13, v21, v25);
////							if (v22)
////							{
////LABEL_END:
////								v30 = pCoord->nY;
////								v31 = pCoord->nX;
////								v32 = a3 + 2;
////								if (a3 + 2 > 1)
////								{
////									v37 = v31 - (v32 >> 1);
////									v38 = v30 - (v32 >> 1);
////									pBoundingBox.nRight = v37 + v32 - 1;
////									pBoundingBox.nTop = v38 + v32 - 1;
////									pBoundingBox.nLeft = v37;
////									pBoundingBox.nBottom = v38;
////									v36 = COLLISION_CheckCollisionMaskForBoundingBoxRecursively(v22, &pBoundingBox, nCollisionType);
////								}
////								else
////								{
////									v36 = COLLISION_CheckCollisionMask(v22, pCoord->nX, pCoord->nY, nCollisionType);
////								}
////
////								if (!v36)
////								{
////									*ppRoom = v22;
////									return;
////								}
////
////								v13 = v39;
////								goto LABEL_51;
////							}
////
////LABEL_51:
////							v21 += 2;
////							v48 += 2;
////							if (v48 >= v46)
////							{
////								v11 = v50;
////								v10 = v41;
////								goto LABEL_54;
////							}
////						}
////					}
////				}
////
////LABEL_54:
////				v9 = v46;
////				v11 += 2;
////				v15 = __OFSUB__(v49 + 2, v46);
////				v14 = v49 + 2 - v46 < 0;
////				v49 += 2;
////				v50 = v11;
////				if (!(v14 ^ v15))
////				{
////					goto LABEL_55;
////				}
////			}
////		}
////
////LABEL_55:
////		++v9;
////		--v10;
////		v46 = v9;
////		v41 = v10;
////	}
////
////	*ppRoom = NULL;
//}


////TODO:Remove
//D2FUNC(D2COMMON, 10136, void, __fastcall, (D2RoomStrc* pRoom, D2CoordStrc* pCoord, int a3, int nCollisionType, D2RoomStrc** ppRoom), 0x6310)
//void __fastcall D2Common_10136(D2RoomStrc* pRoom, D2CoordStrc* pCoord, int a3, int nCollisionType, D2RoomStrc** ppRoom)
//{
//	return D2COMMON_10136(pRoom, pCoord, a3, nCollisionType, ppRoom);
//}
///////////////



void __fastcall D2Common_10136(D2RoomStrc* pRoom, D2CoordStrc* pCoord, int a3, uint16_t nMask, D2RoomStrc** ppRoom)
{
	int v6; // eax@1
	int v8; // ecx@1
	signed int v9; // edx@1
	signed int v10; // ecx@1
	D2RoomStrc *v13; // edi@4
	D2RoomStrc *v22; // ebx@25
	int v48; // [sp+34h] [bp-44h]@24
	int v49; // [sp+38h] [bp-40h]@2
	D2DrlgCoordsStrc pDrlgCoords; // [sp+58h] [bp-20h]@1

	v6 = pCoord->nX;
	v8 = pCoord->nY;

	DUNGEON_GetRoomCoordinates(pRoom, &pDrlgCoords);

	v9 = 1;
	v10 = -1;

	while (v10 > -50)
	{
		if (v10 < v9)
		{
			pCoord->nY = v10 + v8;
			v49 = v10;

			while (v49 < v9)
			{
				v13 = pRoom;

				if (pCoord->nY < pDrlgCoords.dwSubtilesTop || pCoord->nY >= pDrlgCoords.dwSubtilesTop + pDrlgCoords.dwSubtilesHeight)
				{
					v13 = COLLISION_GetRoomBySubTileCoordinates(pRoom, pCoord->nX, pCoord->nY);
				}

				if (v13)
				{
					v48 = v10;
					pCoord->nX = v10 + v6;
					while (v48 < v9)
					{
						v22 = v13;

						DUNGEON_GetRoomCoordinates(v13, &pDrlgCoords);

						if (pCoord->nX < pDrlgCoords.dwSubtilesLeft || pCoord->nX >= pDrlgCoords.dwSubtilesLeft + pDrlgCoords.dwSubtilesWidth)
						{
							v22 = COLLISION_GetRoomBySubTileCoordinates(v13, pCoord->nX, pCoord->nY);
						}

						if (v22 && !COLLISION_CheckMaskWithSizeXY(v22, pCoord->nX, pCoord->nY, a3 + 2, a3 + 2, nMask))
						{
							*ppRoom = v22;
							return;
						}

						pCoord->nX += 2;
						v48 += 2;
					}
				}

				pCoord->nY += 2;
				v49 += 2;
			}
		}

		++v9;
		--v10;
	}

	*ppRoom = NULL;
}

//D2Common.0x6FD46620
//Named DungeonFindRoomGame in the original game
D2RoomStrc* __fastcall COLLISION_GetRoomBySubTileCoordinates(D2RoomStrc* pRoom, int nX, int nY)
{
	return DUNGEON_GetRoomAtPosition(pRoom, nX, nY);
}
