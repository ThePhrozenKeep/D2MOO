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
			DUNGEON_ExpandCoords(&nX, &nY);

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

		pCollisionGrid = (D2RoomCollisionGridStrc*)FOG_AllocPool(pMemPool, sizeof(D2RoomCollisionGridStrc) + sizeof(uint16_t) * ((pDrlgCoords.dwSubtilesWidth + 2) * pDrlgCoords.dwSubtilesHeight), __FILE__, __LINE__, 0);
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

		DUNGEON_ExpandCoords(&nX, &nY);

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
				nFlags |= COLLIDE_WALL;
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
			FOG_FreePool(pMemPool, pCollisionGrid, __FILE__, __LINE__, 0);
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
	uint16_t* pCollisionMask = 0;
	uint16_t nResult = 0;
	int nX = 0;
	int nY = 0;

	pCollisionMask = &pCollisionGrid->pCollisionMask[(pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft) + (pBoundingBox->nBottom - pCollisionGrid->pRoomCoords.dwSubtilesTop) * pCollisionGrid->pRoomCoords.dwSubtilesWidth];

	if (pBoundingBox->nBottom <= pBoundingBox->nTop)
	{
		nY = (pBoundingBox->nTop - pCollisionGrid->pRoomCoords.dwSubtilesTop) - (pBoundingBox->nBottom - pCollisionGrid->pRoomCoords.dwSubtilesTop) + 1;
		do
		{
			if (pBoundingBox->nLeft <= pBoundingBox->nRight)
			{
				nX = (pBoundingBox->nRight - pCollisionGrid->pRoomCoords.dwSubtilesLeft) - (pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft) + 1;
				do
				{
					*pCollisionMask &= nMask;
					nResult |= *pCollisionMask;
					++pCollisionMask;

					--nX;
				}
				while (nX);
			}
			pCollisionMask += pCollisionGrid->pRoomCoords.dwSubtilesWidth + (pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft) - (pBoundingBox->nRight - pCollisionGrid->pRoomCoords.dwSubtilesLeft) - 1;

			--nY;
		}
		while (nY);

		return nResult;
	}

	return 0;
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
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;
	int nBoundingBoxes = 0;
	uint16_t nResult = 0;
	D2BoundingBoxStrc pBoundingBoxes[3] = {};

	pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, pBoundingBox->nLeft, pBoundingBox->nBottom);

	if (pRoom)
	{
		pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid)
		{
			nBoundingBoxes = COLLISION_AdaptBoundingBoxToGrid(pRoom, pBoundingBox, pBoundingBoxes);
			if (nBoundingBoxes > 0)
			{
				nResult = COLLISION_CheckCollisionMaskForBoundingBox(pCollisionGrid, &pBoundingBoxes[0], nMask);

				for (int i = 1; i < nBoundingBoxes; ++i)
				{
					nResult |= COLLISION_CheckCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBoxes[i], nMask);
				}

				return nResult;
			}
		}
	}

	return (COLLIDE_BLANK | COLLIDE_WALL | COLLIDE_BLOCK_MISSILE | COLLIDE_BLOCK_PLAYER);
}

//D2Common.0x6FD41DE0 (#10121)
// TODO: Better name (Find difference to #10122)
uint16_t __stdcall COLLISION_CheckMaskWithPattern1(D2RoomStrc* pRoom, int nX, int nY, int nCollisionPattern, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	switch (nCollisionPattern)
	{
	case 0:
		return COLLISION_CheckCollisionMask(pRoom, nX, nY, nMask);

	case 1:
	case 3:
	case 5:
		return COLLISION_CheckCollisionMaskWithAdjacentCells(pRoom, nX, nY, nMask);

	case 2:
	case 4:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
		return COLLISION_CheckCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);

	default:
		return -1;
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
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;

	pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY);

	if (pRoom)
	{
		pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid && pCollisionGrid->pCollisionMask)
		{
			return pCollisionGrid->pCollisionMask[nX + pCollisionGrid->pRoomCoords.dwSubtilesWidth * (nY - pCollisionGrid->pRoomCoords.dwSubtilesTop) - pCollisionGrid->pRoomCoords.dwSubtilesLeft] & nMask;
		}
	}

	return (COLLIDE_BLANK | COLLIDE_WALL | COLLIDE_BLOCK_MISSILE | COLLIDE_BLOCK_PLAYER);
}

//D2Common.0x6FD42740 (#10122)
// TODO: Better name (Find difference to #10121)
int __stdcall COLLISION_CheckMaskWithPattern2(D2RoomStrc* pRoom, int nX, int nY, int nCollisionPattern, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	switch (nCollisionPattern)
	{
	case 0:
		return COLLISION_CheckMask(pRoom, nX, nY, nMask);

	case 1:
	case 3:
		return sub_6FD42A30(pRoom, nX, nY, nMask);

	case 2:
	case 4:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
		return sub_6FD428D0(pRoom, &pBoundingBox, nMask);

	case 5:
		return sub_6FD42A30(pRoom, nX, nY, nMask);
	
	default:
		return 1;
	}
}

//D2Common.0x6FD428D0
int __fastcall sub_6FD428D0(D2RoomStrc* pRoom, D2BoundingBoxStrc* pBoundingBox, uint16_t nMask)
{
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;
	int nBoundingBoxes = 0;
	D2BoundingBoxStrc pBoundingBoxes[3] = {};

	pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, pBoundingBox->nLeft, pBoundingBox->nBottom);

	if (!pRoom)
	{
		return 1;
	}

	pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
	if (!pCollisionGrid)
	{
		return 1;
	}

	nBoundingBoxes = COLLISION_AdaptBoundingBoxToGrid(pRoom, pBoundingBox, pBoundingBoxes);
	if (nBoundingBoxes <= 0 || COLLISION_CheckCollisionMaskForBoundingBox(pCollisionGrid, &pBoundingBoxes[0], nMask))
	{
		return 1;
	}

	for (int i = 1; i < nBoundingBoxes; ++i)
	{
		if (sub_6FD428D0(pRoom, &pBoundingBoxes[i], nMask))
		{
			return 1;
		}
	}

	return 0;
}

//D2Common.0x6FD42A30
// TODO: v21
int __fastcall sub_6FD42A30(D2RoomStrc* pRoom, int nX, int nY, uint16_t nMask)
{
	D2RoomCollisionGridStrc* pRoomCollisionGrid = NULL;
	uint16_t* pCollisionMask = NULL;
	int v21 = 0;

	pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY);

	if (!pRoom)
	{
		return 1;
	}

	pRoomCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);	
	if (!pRoomCollisionGrid || !pRoomCollisionGrid->pCollisionMask)
	{
		return 1;
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
			return 1;
		}

		return 0;

	case 1:
		if (nMask & *pCollisionMask || nMask & pCollisionMask[1] || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth] || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return 1;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX - 1, nY, nMask))
		{
			return 1;
		}

		return 0;

	case 2:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[1] || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return 1;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX, nY - 1, nMask))
		{
			return 1;
		}

		return 0;

	case 3:
		if (nMask & *pCollisionMask || nMask & pCollisionMask[1] || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return 1;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX, nY - 1, nMask) || COLLISION_CheckCollisionMask(pRoom, nX - 1, nY, nMask))
		{
			return 1;
		}

		return 0;

	case 4:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth] || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return 1;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX + 1, nY, nMask))
		{
			return 1;
		}

		return 0;

	case 6:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return 1;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX + 1, nY, nMask) || COLLISION_CheckCollisionMask(pRoom, nX, nY - 1, nMask))
		{
			return 1;
		}
		
		return 0;

	case 8:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[1] || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return 1;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX, nY + 1, nMask))
		{
			return 1;
		}

		return 0;

	case 9:
		if (nMask & *pCollisionMask || nMask & pCollisionMask[1] || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return 1;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX - 1, nY, nMask) || COLLISION_CheckCollisionMask(pRoom, nX, nY + 1, nMask))
		{
			return 1;
		}

		return 0;

	case 12:
		if (nMask & *pCollisionMask || nMask & *(pCollisionMask - 1) || nMask & pCollisionMask[-pRoomCollisionGrid->pRoomCoords.dwSubtilesWidth])
		{
			return 1;
		}

		if (COLLISION_CheckCollisionMask(pRoom, nX + 1, nY, nMask) || COLLISION_CheckCollisionMask(pRoom, nX, nY + 1, nMask))
		{
			return 1;
		}

		return 0;

	default:
		return 1;
	}
}

//D2Common.0x6FD43080 (#10119)
uint16_t __stdcall COLLISION_CheckMaskWithSize(D2RoomStrc* pRoom, int nX, int nY, int nUnitSize, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	switch (nUnitSize)
	{
	case 0:
	case 1:
		return COLLISION_CheckCollisionMask(pRoom, nX, nY, nMask);

	case 2:
		return COLLISION_CheckCollisionMaskWithAdjacentCells(pRoom, nX, nY, nMask);

	case 3:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
		return COLLISION_CheckCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);

	default:
		return -1;
	}
}

//D2Common.0x6FD432A0 (#10128)
void __stdcall COLLISION_SetMaskWithSize(D2RoomStrc* pRoom, int nX, int nY, int nUnitSize, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	switch (nUnitSize)
	{
	case 1:
		COLLISION_SetCollisionMask(pRoom, nX, nY, nMask);
		break;

	case 2:
		COLLISION_SetCollisionMask(pRoom, nX - 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX + 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY - 1, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY + 1, nMask);
		break;

	case 3:
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
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;

	pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY);

	if (pRoom)
	{
		pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid && pCollisionGrid->pCollisionMask)
		{
			pCollisionGrid->pCollisionMask[nX + pCollisionGrid->pRoomCoords.dwSubtilesWidth * (nY - pCollisionGrid->pRoomCoords.dwSubtilesTop) - pCollisionGrid->pRoomCoords.dwSubtilesLeft] |= nMask;
		}
	}
}

//D2Common.0x6FD43580
void __fastcall COLLISION_SetCollisionMaskForBoundingBoxRecursively(D2RoomStrc* pRoom, D2BoundingBoxStrc* pBoundingBox, uint16_t nMask)
{
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;
	int nBoundingBoxes = 0;
	D2BoundingBoxStrc pBoundingBoxes[3] = {};

	pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, pBoundingBox->nLeft, pBoundingBox->nBottom);

	if (pRoom)
	{
		pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid)
		{
			nBoundingBoxes = COLLISION_AdaptBoundingBoxToGrid(pRoom, pBoundingBox, pBoundingBoxes);
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
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;
	D2RoomStrc* pAdjacentRoom = NULL;
	D2BoundingBoxStrc pBoundingBox = {};

	switch (nCollisionPattern)
	{
	case 1:
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

	case 2:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
		COLLISION_SetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);

		if (nMask)
		{
			COLLISION_SetCollisionMask(pRoom, nX - 1, nY, COLLIDE_UNIT_RELATED);
			COLLISION_SetCollisionMask(pRoom, nX, nY, COLLIDE_UNIT_RELATED);
			COLLISION_SetCollisionMask(pRoom, nX + 1, nY, COLLIDE_UNIT_RELATED);
			COLLISION_SetCollisionMask(pRoom, nX, nY - 1, COLLIDE_UNIT_RELATED);
			COLLISION_SetCollisionMask(pRoom, nX, nY + 1, COLLIDE_UNIT_RELATED);
		}
		break;

	case 3:
		COLLISION_SetCollisionMask(pRoom, nX - 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX + 1, nY, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY - 1, nMask);
		COLLISION_SetCollisionMask(pRoom, nX, nY + 1, nMask);

		if (nMask)
		{
			pAdjacentRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY);
			if (pAdjacentRoom)
			{
				pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pAdjacentRoom);
				if (pCollisionGrid && pCollisionGrid->pCollisionMask)
				{
					pCollisionGrid->pCollisionMask[nX + pCollisionGrid->pRoomCoords.dwSubtilesWidth * (nY - pCollisionGrid->pRoomCoords.dwSubtilesTop) - pCollisionGrid->pRoomCoords.dwSubtilesLeft] |= COLLIDE_PET;
				}
			}
		}
		break;

	case 4:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX, nY, 3, 3);
		COLLISION_SetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);

		if (nMask)
		{
			COLLISION_SetCollisionMask(pRoom, nX - 1, nY, COLLIDE_PET);
			COLLISION_SetCollisionMask(pRoom, nX, nY, COLLIDE_PET);
			COLLISION_SetCollisionMask(pRoom, nX + 1, nY, COLLIDE_PET);
			COLLISION_SetCollisionMask(pRoom, nX, nY - 1, COLLIDE_PET);
			COLLISION_SetCollisionMask(pRoom, nX, nY + 1, COLLIDE_PET);
		}
		break;

	case 5:
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
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;

	pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, nX, nY);

	if (pRoom)
	{
		pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid && pCollisionGrid->pCollisionMask)
		{
			pCollisionGrid->pCollisionMask[nX + pCollisionGrid->pRoomCoords.dwSubtilesWidth * (nY - pCollisionGrid->pRoomCoords.dwSubtilesTop) - pCollisionGrid->pRoomCoords.dwSubtilesLeft] &= ~nMask;
		}
	}
}

//D2Common.0x6FD43CE0
void __fastcall COLLISION_ResetCollisionMaskForBoundingBoxRecursively(D2RoomStrc* pRoom, D2BoundingBoxStrc* pBoundingBox, uint16_t nMask)
{
	D2RoomCollisionGridStrc* pCollisionGrid = NULL;
	int nBoundingBoxes = 0;
	D2BoundingBoxStrc pBoundingBoxes[3] = {};

	pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, pBoundingBox->nLeft, pBoundingBox->nBottom);

	if (pRoom)
	{
		pCollisionGrid = DUNGEON_GetCollisionGridFromRoom(pRoom);
		if (pCollisionGrid)
		{
			nBoundingBoxes = COLLISION_AdaptBoundingBoxToGrid(pRoom, pBoundingBox, pBoundingBoxes);
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
		case 1:
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

		case 2:
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

		case 3:
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

		case 4:
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

		case 5:
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
	uint16_t* pCollisionMask = NULL;
	int nX = 0;
	int nY = 0;

	pCollisionMask = &pCollisionGrid->pCollisionMask[(pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft) + (pBoundingBox->nBottom - pCollisionGrid->pRoomCoords.dwSubtilesTop) * pCollisionGrid->pRoomCoords.dwSubtilesWidth];

	if (pBoundingBox->nBottom <= pBoundingBox->nTop)
	{
		nY = (pBoundingBox->nTop - pCollisionGrid->pRoomCoords.dwSubtilesTop) - (pBoundingBox->nBottom - pCollisionGrid->pRoomCoords.dwSubtilesTop) + 1;
		do
		{
			if (pBoundingBox->nLeft <= pBoundingBox->nRight)
			{
				nX = (pBoundingBox->nRight - pCollisionGrid->pRoomCoords.dwSubtilesLeft) - (pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft) + 1;
				do
				{
					*pCollisionMask &= ~nMask;
					++pCollisionMask;

					--nX;
				}
				while (nX);
			}
			pCollisionMask += pCollisionGrid->pRoomCoords.dwSubtilesWidth + (pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft) - (pBoundingBox->nRight - pCollisionGrid->pRoomCoords.dwSubtilesLeft) - 1;

			--nY;
		}
		while (nY);
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
	uint16_t* pCollisionMask = NULL;
	int nX = 0;
	int nY = 0;

	pCollisionMask = &pCollisionGrid->pCollisionMask[(pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft) + (pBoundingBox->nBottom - pCollisionGrid->pRoomCoords.dwSubtilesTop) * pCollisionGrid->pRoomCoords.dwSubtilesWidth];

	if (pBoundingBox->nBottom <= pBoundingBox->nTop)
	{
		nY = (pBoundingBox->nTop - pCollisionGrid->pRoomCoords.dwSubtilesTop) - (pBoundingBox->nBottom - pCollisionGrid->pRoomCoords.dwSubtilesTop) + 1;
		do
		{
			if (pBoundingBox->nLeft <= pBoundingBox->nRight)
			{
				nX = (pBoundingBox->nRight - pCollisionGrid->pRoomCoords.dwSubtilesLeft) - (pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft) + 1;
				do
				{
					*pCollisionMask |= nMask;
					++pCollisionMask;

					--nX;
				}
				while (nX);
			}

			pCollisionMask += pCollisionGrid->pRoomCoords.dwSubtilesWidth + (pBoundingBox->nLeft - pCollisionGrid->pRoomCoords.dwSubtilesLeft) - (pBoundingBox->nRight - pCollisionGrid->pRoomCoords.dwSubtilesLeft) - 1;
			--nY;
		}
		while (nY);
	}
}

//D2Common.0x6FD44660 (#10131)
uint16_t __fastcall D2Common_10131(D2RoomStrc* pRoom, int nX1, int nY1, int nX2, int nY2, int nCollisionPattern, int nCollisionType, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};
	uint16_t v10 = 0;

	if (pRoom)
	{
		switch (nCollisionPattern)
		{
		case 0:
			break;
		
		case 1:
			COLLISION_ResetCollisionMask(pRoom, nX1, nY1, nCollisionType);
			break;

		case 2:
			COLLISION_ResetCollisionMask(pRoom, nX1 - 1, nY1, nCollisionType);
			COLLISION_ResetCollisionMask(pRoom, nX1, nY1, nCollisionType);
			COLLISION_ResetCollisionMask(pRoom, nX1 + 1, nY1, nCollisionType);
			COLLISION_ResetCollisionMask(pRoom, nX1, nY1 - 1, nCollisionType);
			COLLISION_ResetCollisionMask(pRoom, nX1, nY1 + 1, nCollisionType);
			break;

		case 3:
			COLLISION_CreateBoundingBox(&pBoundingBox, nX1, nY1, 3, 3);
			COLLISION_ResetCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nCollisionType);
			break;

		default:
			break;
		}

		v10 = COLLISION_CheckMaskWithSize(pRoom, nX2, nY2, nCollisionPattern, nMask);
	}
	else
	{
		v10 = COLLISION_CheckMaskWithSize(pRoom, nX2, nY2, nCollisionPattern, nMask);
	}

	if (v10 & 5)
	{
		COLLISION_SetMaskWithSize(pRoom, nX1, nY1, nCollisionPattern, nCollisionType);
	}
	else
	{
		COLLISION_SetMaskWithSize(pRoom, nX2, nY2, nCollisionPattern, nCollisionType);
	}

	return v10;
}

//D2Common.0x6FD44910
void __fastcall COLLISION_CreateBoundingBox(D2BoundingBoxStrc* pBoundingBox, int nCenterX, int nCenterY, unsigned int nSizeX, unsigned int nSizeY)
{
	pBoundingBox->nLeft = nCenterX - (nSizeX >> 1);
	pBoundingBox->nRight = nCenterX - (nSizeX >> 1) + nSizeX - 1;
	pBoundingBox->nBottom = nCenterY - (nSizeY >> 1);
	pBoundingBox->nTop = nCenterY - (nSizeY >> 1) + nSizeY - 1;
}

//D2Common.0x6FD44950 (#10132)
uint16_t __fastcall D2Common_10132(D2RoomStrc* pRoom, int nX1, int nY1, int nX2, int nY2, int nCollisionPattern, int nCollisionType, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};
	uint16_t nResult = 0;

	COLLISION_ResetMaskWithPattern(pRoom, nX1, nY1, nCollisionPattern, nCollisionType);

	switch (nCollisionPattern)
	{
	case 0:
		nResult = COLLISION_CheckCollisionMask(pRoom, nX2, nY2, nMask);
		if (nResult)
		{
			COLLISION_SetMaskWithPattern(pRoom, nX1, nY1, nCollisionPattern, nCollisionType);
			return nResult;
		}
		else
		{
			COLLISION_SetMaskWithPattern(pRoom, nX2, nY2, nCollisionPattern, nCollisionType);
			return 0;
		}

	case 1:
	case 3:
	case 5:
		nResult = COLLISION_CheckCollisionMaskWithAdjacentCells(pRoom, nX2, nY2, nMask);
		if (nResult)
		{
			COLLISION_SetMaskWithPattern(pRoom, nX1, nY1, nCollisionPattern, nCollisionType);
			return nResult;
		}
		else
		{
			COLLISION_SetMaskWithPattern(pRoom, nX2, nY2, nCollisionPattern, nCollisionType);
			return 0;
		}

	case 2:
	case 4:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX2, nY2, 3, 3);
		nResult = COLLISION_CheckCollisionMaskForBoundingBoxRecursively(pRoom, &pBoundingBox, nMask);
		if (nResult)
		{
			COLLISION_SetMaskWithPattern(pRoom, nX1, nY1, nCollisionPattern, nCollisionType);
			return nResult;
		}
		else
		{
			COLLISION_SetMaskWithPattern(pRoom, nX2, nY2, nCollisionPattern, nCollisionType);
			return 0;
		}

	default:
		COLLISION_SetMaskWithPattern(pRoom, nX1, nY1, nCollisionPattern, nCollisionType);
		return -1;
	}
}

//D2Common.0x6FD44BB0
uint16_t __fastcall sub_6FD44BB0(D2RoomStrc* pRoom1, int nX1, int nY1, D2RoomStrc* pRoom2, int nX2, int nY2, int nCollisionPattern, uint16_t nMask1, uint16_t nMask2)
{
	D2BoundingBoxStrc pBoundingBox = {};
	uint16_t nResult = 0;

	if (pRoom1)
	{
		switch (nCollisionPattern)
		{
		case 0:
			break;

		case 1:
			COLLISION_ResetCollisionMask(pRoom1, nX1, nY1, nMask1);
			break;

		case 2:
			COLLISION_ResetCollisionMask(pRoom1, nX1 - 1, nY1, nMask1);
			COLLISION_ResetCollisionMask(pRoom1, nX1, nY1, nMask1);
			COLLISION_ResetCollisionMask(pRoom1, nX1 + 1, nY1, nMask1);
			COLLISION_ResetCollisionMask(pRoom1, nX1, nY1 - 1, nMask1);
			COLLISION_ResetCollisionMask(pRoom1, nX1, nY1 + 1, nMask1);
			break;

		case 3:
			COLLISION_CreateBoundingBox(&pBoundingBox, nX1, nY1, 3, 3);
			COLLISION_ResetCollisionMaskForBoundingBoxRecursively(pRoom1, &pBoundingBox, nMask1);
			break;

		default:
			return -1;
		}
	}

	switch (nCollisionPattern)
	{
	case 0:
	case 1:
		nResult = COLLISION_CheckCollisionMask(pRoom2, nX2, nY2, nMask2);
		break;

	case 2:
		nResult = COLLISION_CheckCollisionMaskWithAdjacentCells(pRoom2, nX2, nY2, nMask2);
		break;

	case 3:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX2, nY2, 3, 3);
		nResult = COLLISION_CheckCollisionMaskForBoundingBoxRecursively(pRoom2, &pBoundingBox, nMask2);
		break;

	default:
		return -1;
	}

	switch (nCollisionPattern)
	{
	case 0:
		break;

	case 1:
		COLLISION_SetCollisionMask(pRoom2, nX2, nY2, nMask1);
		break;

	case 2:
		COLLISION_SetCollisionMask(pRoom2, nX2 - 1, nY2, nMask1);
		COLLISION_SetCollisionMask(pRoom2, nX2, nY2, nMask1);
		COLLISION_SetCollisionMask(pRoom2, nX2 + 1, nY2, nMask1);
		COLLISION_SetCollisionMask(pRoom2, nX2, nY2 - 1, nMask1);
		COLLISION_SetCollisionMask(pRoom2, nX2, nY2 + 1, nMask1);
		break;

	case 3:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX2, nY2, 3, 3);
		COLLISION_SetCollisionMaskForBoundingBoxRecursively(pRoom2, &pBoundingBox, nMask1);
		break;

	default:
		return -1;
	}

	return nResult;
}

//D2Common.0x6FD44E00
uint16_t __fastcall sub_6FD44E00(D2RoomStrc* pRoom1, int nX1, int nY1, D2RoomStrc* pRoom2, int nX2, int nY2, int nCollisionPattern, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};

	if (pRoom1)
	{
		switch (nCollisionPattern)
		{
		case 1:
			COLLISION_ResetCollisionMask(pRoom1, nX1, nY1, nMask);
			break;

		case 2:
			COLLISION_ResetCollisionMask(pRoom1, nX1 - 1, nY1, nMask);
			COLLISION_ResetCollisionMask(pRoom1, nX1, nY1, nMask);
			COLLISION_ResetCollisionMask(pRoom1, nX1 + 1, nY1, nMask);
			COLLISION_ResetCollisionMask(pRoom1, nX1, nY1 - 1, nMask);
			COLLISION_ResetCollisionMask(pRoom1, nX1, nY1 + 1, nMask);
			break;

		case 3:
			COLLISION_CreateBoundingBox(&pBoundingBox, nX1, nY1, 3, 3);
			COLLISION_ResetCollisionMaskForBoundingBoxRecursively(pRoom1, &pBoundingBox, nMask);
			break;

		default:
			break;
		}
	}

	switch(nCollisionPattern)
	{
	case 1:
		COLLISION_SetCollisionMask(pRoom2, nX2, nY2, nMask);
		break;

	case 2:
		COLLISION_SetCollisionMask(pRoom2, nX2 - 1, nY2, nMask);
		COLLISION_SetCollisionMask(pRoom2, nX2, nY2, nMask);
		COLLISION_SetCollisionMask(pRoom2, nX2 + 1, nY2, nMask);
		COLLISION_SetCollisionMask(pRoom2, nX2, nY2 - 1, nMask);
		COLLISION_SetCollisionMask(pRoom2, nX2, nY2 + 1, nMask);
		break;

	case 3:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX2, nY2, 3, 3);
		COLLISION_SetCollisionMaskForBoundingBoxRecursively(pRoom2, &pBoundingBox, nMask);
		break;

	default:
		break;
	}

	return 0;
}

//D2Common.0x6FD44FF0
int __fastcall sub_6FD44FF0(D2RoomStrc* pRoom1, int nX1, int nY1, D2RoomStrc* pRoom2, int nX2, int nY2, int nCollisionPattern, int nCollisionType, uint16_t nMask)
{
	D2BoundingBoxStrc pBoundingBox = {};
	int nResult = 0;

	if (pRoom1)
	{
		COLLISION_ResetMaskWithPattern(pRoom1, nX1, nY1, nCollisionPattern, nCollisionType);
	}

	switch (nCollisionPattern)
	{
	case 0:
		nResult = COLLISION_CheckCollisionMask(pRoom2, nX2, nY2, nMask);
		if (nResult)
		{
			if (pRoom1)
			{
				COLLISION_SetMaskWithPattern(pRoom1, nX1, nY1, nCollisionPattern, nCollisionType);
			}
			return nResult;
		}
		else
		{
			COLLISION_SetMaskWithPattern(pRoom2, nX2, nY2, nCollisionPattern, nCollisionType);
			return 0;
		}

	case 1:
	case 3:
	case 5:
		nResult = sub_6FD42A30(pRoom2, nX2, nY2, nMask);
		if (nResult)
		{
			if (pRoom1)
			{
				COLLISION_SetMaskWithPattern(pRoom1, nX1, nY1, nCollisionPattern, nCollisionType);
			}
			return nResult;
		}
		else
		{
			COLLISION_SetMaskWithPattern(pRoom2, nX2, nY2, nCollisionPattern, nCollisionType);
			return 0;
		}

	case 2:
	case 4:
		COLLISION_CreateBoundingBox(&pBoundingBox, nX2, nY2, 3, 3);
		nResult = sub_6FD428D0(pRoom2, &pBoundingBox, nMask);
		if (nResult)
		{
			if (pRoom1)
			{
				COLLISION_SetMaskWithPattern(pRoom1, nX1, nY1, nCollisionPattern, nCollisionType);
			}
			return nResult;
		}
		else
		{
			COLLISION_SetMaskWithPattern(pRoom2, nX2, nY2, nCollisionPattern, nCollisionType);
			return 0;
		}

	default:
		if (pRoom1)
		{
			COLLISION_SetMaskWithPattern(pRoom1, nX1, nY1, nCollisionPattern, nCollisionType);
		}
		return 1;
	}
}

//D2Common.0x6FD451D0 (#10133)
// TODO: Name
void __fastcall D2Common_10133(D2RoomStrc* pRoom1, int nX1, int nY1, D2RoomStrc* pRoom2, int nX2, int nY2, int nCollisionPattern, int nCollisionType)
{
	if (pRoom1)
	{
		COLLISION_ResetMaskWithPattern(pRoom1, nX1, nY1, nCollisionPattern, nCollisionType);
		COLLISION_SetMaskWithPattern(pRoom2, nX2, nY2, nCollisionPattern, nCollisionType);
	}
}

////D2Common.0x6FD45210) --------------------------------------------------------
//int __stdcall D2Common_11263(D2RoomStrc* pRoom, D2CoordStrc* a2, D2CoordStrc* a3, uint16_t nCollisionType)
//{
////	int v4; // ebp@1
////	int v5; // edx@1
////	int v6; // eax@1
////	int v7; // ecx@1
////	D2RoomStrc* v8; // esi@1
////	int v9; // eax@1
////	int v10; // ecx@1
////	int v11; // edi@6
////	int v12; // ebx@6
////	int v13; // eax@6
////	int v14; // ecx@6
////	bool v15; // sf@6
////	unsigned __int8 v16; // of@6
////	unsigned int v17; // edx@14
////	D2RoomStrc* v18; // eax@15
////	int v19; // ecx@18
////	D2RoomCollisionGridStrc* v20; // eax@30
////	int v22; // edx@32
////	D2RoomCollisionGridStrc* v23; // eax@38
////	__int16* v24; // eax@38
////	__int16* v25; // edx@38
////	int v26; // edi@40
////	int v27; // eax@46
////	unsigned int v28; // esi@48
////	int v29; // eax@49
////	int v30; // ecx@52
////	int v31; // edx@64
////	int v32; // eax@67
////	D2RoomCollisionGridStrc* v33; // eax@71
////	int v34; // eax@71
////	int v35; // ecx@71
////	int v36; // eax@77
////	unsigned int v37; // esi@79
////	int v38; // eax@80
////	int v39; // ecx@83
////	int v40; // ecx@95
////	D2RoomCollisionGridStrc* v41; // eax@103
////	uint16_t* v42; // eax@103
////	int v43; // ecx@105
////	int v44; // ecx@106
////	int v45; // eax@115
////	unsigned int v46; // esi@117
////	int v47; // eax@118
////	int v48; // ecx@121
////	int v49; // ecx@132
////	D2RoomCollisionGridStrc* v50; // eax@140
////	int v51; // eax@140
////	int v52; // ebx@142
////	int v53; // ecx@142
////	int v54; // eax@151
////	unsigned int v55; // esi@153
////	int v56; // eax@154
////	int v57; // ecx@157
////	signed int v58; // [sp+10h] [bp-2Ch]@1
////	int v59; // [sp+14h] [bp-28h]@1
////	int v60; // [sp+14h] [bp-28h]@32
////	int v61; // [sp+18h] [bp-24h]@1
////	int i; // [sp+18h] [bp-24h]@64
////	signed int v63; // [sp+1Ch] [bp-20h]@1
////	int v64; // [sp+20h] [bp-1Ch]@1
////	int v65; // [sp+24h] [bp-18h]@1
////	int pNumRooms; // [sp+28h] [bp-14h]@14
////	unsigned int v67; // [sp+2Ch] [bp-10h]@6
////	D2RoomStrc **pppRoom; // [sp+30h] [bp-Ch]@14
////	int v69; // [sp+34h] [bp-8h]@117
////	int v70; // [sp+38h] [bp-4h]@117
////	int pRooma; // [sp+40h] [bp+4h]@33
////	int pRoomb; // [sp+40h] [bp+4h]@65
////	int pRoomc; // [sp+40h] [bp+4h]@94
////	int a2a; // [sp+44h] [bp+8h]@1
////
////	v4 = a2->nX;
////	v5 = a2->nY;
////	v6 = a3->nX;
////	v7 = a3->nY;
////	v63 = 1;
////	v58 = 1;
////	v8 = pRoom;
////	v64 = v6;
////	v65 = v7;
////	v9 = v6 - v4;
////	v10 = v7 - v5;
////	a2a = v5;
////	v61 = v9;
////	v59 = v10;
////	if (!pRoom)
////		goto LABEL_167;
////	if (v9 < 0)
////	{
////		v61 = -v9;
////		v63 = -1;
////	}
////	if (v10 < 0)
////	{
////		v59 = -v10;
////		v58 = -1;
////	}
////	v11 = pRoom->pRoomCoords.dwSubtilesLeft;
////	v12 = pRoom->pRoomCoords.dwSubtilesTop;
////	v13 = pRoom->pRoomCoords.dwSubtilesLeft + pRoom->pRoomCoords.dwSubtilesWidth;
////	v14 = v12 + pRoom->pRoomCoords.dwSubtilesHeight;
////	v16 = __OFSUB__(v4, pRoom->pRoomCoords.dwSubtilesLeft);
////	v15 = v4 - pRoom->pRoomCoords.dwSubtilesLeft < 0;
////	v67 = v12 + pRoom->pRoomCoords.dwSubtilesHeight;
////	if (v15 ^ v16)
////		goto LABEL_170;
////	if (v4 < v13 && v5 >= v12 && v5 < v14)
////		goto LABEL_28;
////	if (v4 < v11 || v4 >= v13 || v5 < v12 || v5 >= v14)
////	{
////LABEL_170:
////		pppRoom = 0;
////		pNumRooms = 0;
////		DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &pppRoom, &pNumRooms);
////		v17 = 0;
////		if ((unsigned int)pNumRooms <= 0)
////			goto LABEL_167;
////		while (1)
////		{
////			v18 = pppRoom[v17];
////			if (v18)
////			{
////				if (v4 >= v18->pRoomCoords.dwSubtilesLeft)
////				{
////					if (v4 < v18->pRoomCoords.dwSubtilesLeft + v18->pRoomCoords.dwSubtilesWidth)
////					{
////						v19 = v18->pRoomCoords.dwSubtilesTop;
////						if (a2a >= v19)
////						{
////							if (a2a < v19 + v18->pRoomCoords.dwSubtilesHeight)
////								break;
////						}
////					}
////				}
////			}
////			++v17;
////			if (v17 >= pNumRooms)
////				goto LABEL_167;
////		}
////		v8 = pppRoom[v17];
////		v5 = a2a;
////	}
////	if (!v8
////		|| (v11 = v8->pRoomCoords.dwSubtilesLeft,
////			v12 = v8->pRoomCoords.dwSubtilesTop,
////			v13 = v8->pRoomCoords.dwSubtilesLeft + v8->pRoomCoords.dwSubtilesWidth,
////			v14 = v12 + v8->pRoomCoords.dwSubtilesHeight,
////			v16 = __OFSUB__(v4, v8->pRoomCoords.dwSubtilesLeft),
////			v15 = v4 - v8->pRoomCoords.dwSubtilesLeft < 0,
////			v67 = v12 + v8->pRoomCoords.dwSubtilesHeight,
////			v15 ^ v16)
////		|| v4 >= v13
////		|| v5 < v12
////		|| v5 >= v14)
////		goto LABEL_167;
////LABEL_28:
////	if (!v61)
////	{
////		if (v59)
////		{
////			v22 = v59 + 1;
////			v60 = v59 + 1;
////			while (1)
////			{
////				pRooma = 0;
////				if (v58 <= 0)
////				{
////					if (v65 < v12)
////					{
////						v60 = a2a - v12 + 1;
////						pRooma = v22 - (a2a - v12 + 1);
////					}
////				}
////				else
////				{
////					if (v65 >= v14)
////					{
////						v60 = v14 - a2a;
////						pRooma = v22 - (v14 - a2a);
////					}
////				}
////				v23 = DUNGEON_GetCollisionGridFromRoom(v8);
////				v24 = (__int16*)&v23->pCollisionMask[v4 + v23->pRoomCoords.dwSubtilesWidth * (a2a - v12) - v11];
////				v25 = &v24[v60 * v58 * v8->pRoomCoords.dwSubtilesWidth];
////				if (v24 != v25)
////					break;
////				v26 = a2a;
////LABEL_43:
////				if (!pRooma)
////					return 0;
////				if (v4 < v8->pRoomCoords.dwSubtilesLeft
////					|| v4 >= v8->pRoomCoords.dwSubtilesLeft + v8->pRoomCoords.dwSubtilesWidth
////					|| (v27 = v8->pRoomCoords.dwSubtilesTop, v26 < v27)
////					|| v26 >= v27 + v8->pRoomCoords.dwSubtilesHeight)
////				{
////					v64 = 0;
////					v67 = 0;
////					DUNGEON_GetAdjacentRoomsListFromRoom(v8, (D2RoomStrc ***)&v64, (int*)&v67);
////					v28 = 0;
////					if (!v67)
////						goto LABEL_167;
////					while (1)
////					{
////						v29 = *(_DWORD*)(v64 + 4 * v28);
////						if (v29)
////						{
////							if (v4 >= *(_DWORD*)v29)
////							{
////								if (v4 < *(_DWORD*)v29 + *(_DWORD*)(v29 + 8))
////								{
////									v30 = *(_DWORD*)(v29 + 4);
////									if (a2a >= v30)
////									{
////										if (a2a < v30 + *(_DWORD*)(v29 + 12))
////											break;
////									}
////								}
////							}
////						}
////						++v28;
////						if (v28 >= v67)
////							goto LABEL_167;
////					}
////					v8 = *(D2RoomStrc **)(v64 + 4 * v28);
////				}
////				if (!v8
////					|| (v12 = v8->pRoomCoords.dwSubtilesTop, v11 = v8->pRoomCoords.dwSubtilesLeft, v14 = v12 + v8->pRoomCoords.dwSubtilesHeight, v4 < v8->pRoomCoords.dwSubtilesLeft)
////					|| v4 >= v11 + v8->pRoomCoords.dwSubtilesWidth
////					|| a2a < v12
////					|| a2a >= v14)
////					goto LABEL_167;
////				v60 = pRooma;
////				v22 = pRooma;
////			}
////			while (!((unsigned __int16)*v24 & nCollisionType))
////			{
////				v24 += v58 * v8->pRoomCoords.dwSubtilesWidth;
////				v26 = v58 + a2a;
////				a2a += v58;
////				if (v24 == v25)
////					goto LABEL_43;
////			}
////			goto LABEL_167;
////		}
////		v20 = DUNGEON_GetCollisionGridFromRoom(v8);
////		if (nCollisionType & v20->pCollisionMask[v4 + v20->pRoomCoords.dwSubtilesWidth * (a2a - v12) - v11])
////			goto LABEL_167;
////		return 0;
////	}
////	if (!v59)
////	{
////		v31 = v61 + 1;
////		for (i = v61 + 1; ; i = pRoomb)
////		{
////			pRoomb = 0;
////			if (v63 <= 0)
////			{
////				if (v64 >= v11)
////					goto LABEL_71;
////				v32 = v4 - v11 + 1;
////			}
////			else
////			{
////				if (v64 < v13)
////					goto LABEL_71;
////				v32 = v13 - v4;
////			}
////			i = v32;
////			pRoomb = v31 - v32;
////LABEL_71:
////			v33 = DUNGEON_GetCollisionGridFromRoom(v8);
////			v34 = (int)&v33->pCollisionMask[v4 + v33->pRoomCoords.dwSubtilesWidth * (a2a - v12) - v11];
////			v35 = v34 + 2 * i * v63;
////			if (v34 != v35)
////			{
////				while (!(*(_WORD*)v34 & nCollisionType))
////				{
////					v4 += v63;
////					v34 += 2 * v63;
////					if (v34 == v35)
////						goto LABEL_74;
////				}
////				goto LABEL_167;
////			}
////LABEL_74:
////			if (!pRoomb)
////				return 0;
////			if (v4 < v8->pRoomCoords.dwSubtilesLeft
////				|| v4 >= v8->pRoomCoords.dwSubtilesLeft + v8->pRoomCoords.dwSubtilesWidth
////				|| (v36 = v8->pRoomCoords.dwSubtilesTop, a2a < v36)
////				|| a2a >= v36 + v8->pRoomCoords.dwSubtilesHeight)
////			{
////				v65 = 0;
////				v67 = 0;
////				DUNGEON_GetAdjacentRoomsListFromRoom(v8, (D2RoomStrc ***)&v65, (int*)&v67);
////				v37 = 0;
////				if (v67 <= 0)
////					goto LABEL_167;
////				while (1)
////				{
////					v38 = *(_DWORD*)(v65 + 4 * v37);
////					if (v38)
////					{
////						if (v4 >= *(_DWORD*)v38)
////						{
////							if (v4 < *(_DWORD*)v38 + *(_DWORD*)(v38 + 8))
////							{
////								v39 = *(_DWORD*)(v38 + 4);
////								if (a2a >= v39)
////								{
////									if (a2a < v39 + *(_DWORD*)(v38 + 12))
////										break;
////								}
////							}
////						}
////					}
////					++v37;
////					if (v37 >= v67)
////						goto LABEL_167;
////				}
////				v8 = *(D2RoomStrc **)(v65 + 4 * v37);
////			}
////			if (!v8
////				|| (v11 = v8->pRoomCoords.dwSubtilesLeft, v12 = v8->pRoomCoords.dwSubtilesTop, v13 = v8->pRoomCoords.dwSubtilesLeft + v8->pRoomCoords.dwSubtilesWidth, v4 < v8->pRoomCoords.dwSubtilesLeft)
////				|| v4 >= v13
////				|| a2a < v12
////				|| a2a >= v12 + v8->pRoomCoords.dwSubtilesHeight)
////				goto LABEL_167;
////			v31 = pRoomb;
////		}
////	}
////	pRoomc = 0;
////	if (v61 < v59)
////	{
////		v49 = v67;
////		v65 += v58;
////		while (1)
////		{
////			if (v63 >= 0)
////				v70 = v13;
////			else
////				v70 = v11 - 1;
////			if (v58 >= 0)
////				v69 = v49;
////			else
////				v69 = v12 - 1;
////			if (a2a >= v49)
////				goto LABEL_167;
////			v50 = DUNGEON_GetCollisionGridFromRoom(v8);
////			v51 = (int)&v50->pCollisionMask[v4 + v50->pRoomCoords.dwSubtilesWidth * (a2a - v12) - v11];
////			while (1)
////			{
////				if (*(_WORD*)v51 & nCollisionType)
////					goto LABEL_167;
////				v52 = v58 + a2a;
////				a2a += v58;
////				v53 = v61 + pRoomc;
////				pRoomc += v61;
////				if (pRoomc >= v59)
////				{
////					pRoomc = v53 - v59;
////					v4 += v63;
////					v51 += 2 * v63;
////				}
////				v51 += 2 * v58 * v8->pRoomCoords.dwSubtilesWidth;
////				if (v4 == v70)
////					break;
////				if (v52 == v69)
////					break;
////				if (v52 == v65)
////					return 0;
////			}
////			if (v52 == v65)
////				break;
////			if (v4 < v8->pRoomCoords.dwSubtilesLeft
////				|| v4 >= v8->pRoomCoords.dwSubtilesWidth + v8->pRoomCoords.dwSubtilesLeft
////				|| (v54 = v8->pRoomCoords.dwSubtilesTop, v52 < v54)
////				|| v52 >= v54 + v8->pRoomCoords.dwSubtilesHeight)
////			{
////				v64 = 0;
////				v67 = 0;
////				DUNGEON_GetAdjacentRoomsListFromRoom(v8, (D2RoomStrc ***)&v64, (int*)&v67);
////				v55 = 0;
////				if (v67 <= 0)
////					goto LABEL_167;
////				while (1)
////				{
////					v56 = *(_DWORD*)(v64 + 4 * v55);
////					if (v56)
////					{
////						if (v4 >= *(_DWORD*)v56)
////						{
////							if (v4 < *(_DWORD*)v56 + *(_DWORD*)(v56 + 8))
////							{
////								v57 = *(_DWORD*)(v56 + 4);
////								if (a2a >= v57)
////								{
////									if (a2a < v57 + *(_DWORD*)(v56 + 12))
////										break;
////								}
////							}
////						}
////					}
////					++v55;
////					if (v55 >= v67)
////						goto LABEL_167;
////				}
////				v8 = *(D2RoomStrc **)(v64 + 4 * v55);
////			}
////			if (v8)
////			{
////				v11 = v8->pRoomCoords.dwSubtilesLeft;
////				v12 = v8->pRoomCoords.dwSubtilesTop;
////				v13 = v8->pRoomCoords.dwSubtilesLeft + v8->pRoomCoords.dwSubtilesWidth;
////				v49 = v12 + v8->pRoomCoords.dwSubtilesHeight;
////				if (v4 >= v8->pRoomCoords.dwSubtilesLeft)
////				{
////					if (v4 < v13 && a2a >= v12 && a2a < v49)
////						continue;
////				}
////			}
////LABEL_167:
////			a3->nX = v4;
////			a3->nY = a2a;
////			return 1;
////		}
////	}
////	else
////	{
////		v40 = v67;
////		v64 += v63;
////		while (1)
////		{
////			if (v63 >= 0)
////				v67 = v13;
////			else
////				v67 = v11 - 1;
////			if (v58 >= 0)
////				v65 = v40;
////			else
////				v65 = v12 - 1;
////			if (a2a >= v40)
////				goto LABEL_167;
////			v41 = DUNGEON_GetCollisionGridFromRoom(v8);
////			v42 = &v41->pCollisionMask[v4 + v41->pRoomCoords.dwSubtilesWidth * (a2a - v12) - v11];
////			while (1)
////			{
////				if (*v42 & nCollisionType)
////					goto LABEL_167;
////				v43 = v59 + pRoomc;
////				v4 += v63;
////				pRoomc += v59;
////				if (pRoomc < v61)
////				{
////					v44 = a2a;
////				}
////				else
////				{
////					pRoomc = v43 - v61;
////					v44 = v58 + a2a;
////					v42 += v58 * v8->pRoomCoords.dwSubtilesWidth;
////					a2a += v58;
////				}
////				v42 += v63;
////				if (v4 == v67)
////					break;
////				if (v44 == v65)
////					break;
////				if (v4 == v64)
////					return 0;
////			}
////			if (v4 == v64)
////				break;
////			if (v4 < v8->pRoomCoords.dwSubtilesLeft
////				|| v4 >= v8->pRoomCoords.dwSubtilesWidth + v8->pRoomCoords.dwSubtilesLeft
////				|| (v45 = v8->pRoomCoords.dwSubtilesTop, v44 < v45)
////				|| v44 >= v45 + v8->pRoomCoords.dwSubtilesHeight)
////			{
////				v70 = 0;
////				v69 = 0;
////				DUNGEON_GetAdjacentRoomsListFromRoom(v8, (D2RoomStrc ***)&v70, &v69);
////				v46 = 0;
////				if ((unsigned int)v69 <= 0)
////					goto LABEL_167;
////				while (1)
////				{
////					v47 = *(_DWORD*)(v70 + 4 * v46);
////					if (v47)
////					{
////						if (v4 >= *(_DWORD*)v47)
////						{
////							if (v4 < *(_DWORD*)v47 + *(_DWORD*)(v47 + 8))
////							{
////								v48 = *(_DWORD*)(v47 + 4);
////								if (a2a >= v48)
////								{
////									if (a2a < v48 + *(_DWORD*)(v47 + 12))
////										break;
////								}
////							}
////						}
////					}
////					++v46;
////					if (v46 >= v69)
////						goto LABEL_167;
////				}
////				v8 = *(D2RoomStrc **)(v70 + 4 * v46);
////			}
////			if (v8)
////			{
////				v11 = v8->pRoomCoords.dwSubtilesLeft;
////				v12 = v8->pRoomCoords.dwSubtilesTop;
////				v13 = v8->pRoomCoords.dwSubtilesLeft + v8->pRoomCoords.dwSubtilesWidth;
////				v40 = v12 + v8->pRoomCoords.dwSubtilesHeight;
////				if (v4 >= v8->pRoomCoords.dwSubtilesLeft)
////				{
////					if (v4 < v13 && a2a >= v12 && a2a < v40)
////						continue;
////				}
////			}
////			goto LABEL_167;
////		}
////	}
////	return 0;
//return 0;
//}


//TODO:Remove
int __stdcall D2Common_11263(D2RoomStrc* pRoom, D2CoordStrc* a2, D2CoordStrc* a3, uint16_t nMask)
{
	return 1;
}
/////////////


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


static bool PathPosIsInRoom(const D2RoomStrc& room, int posX, int posY)
{
	return posX >= room.nSubtileX && posX < (room.nSubtileX + room.nSubtileWidth)
		&& posY >= room.nSubtileY && posY < (room.nSubtileY + room.nSubtileHeight);
}

//D2Common.0x6FD46620
D2RoomStrc* __fastcall COLLISION_GetRoomBySubTileCoordinates(D2RoomStrc* pRoom, int nX, int nY)
{
	if (pRoom)
	{
		if (PathPosIsInRoom(*pRoom, nX,nY))
		{
			return pRoom;
		}

		D2RoomStrc** ppRoomList = nullptr;
		int nAdjacentRooms = 0;
		DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nAdjacentRooms);

		for (int i = 0; i < nAdjacentRooms; ++i)
		{
			if (ppRoomList[i] && PathPosIsInRoom(*ppRoomList[i], nX, nY))
			{
				return ppRoomList[i];
			}
		}
	}

	return nullptr;
}
