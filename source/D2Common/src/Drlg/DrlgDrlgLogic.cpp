#include <D2Seed.h>

#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgLogic.h"
#include "Drlg/D2DrlgDrlgAnim.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "D2CMP.h"

//D2Common.0x6FD76420
void __fastcall DRLGLOGIC_FreeDrlgCoordList(D2RoomExStrc* pRoomEx)
{
	D2RoomCoordListStrc* pRoomCoordList = NULL;
	D2RoomCoordListStrc* pNext = NULL;

	if (pRoomEx->pLogicalRoomInfo)
	{
		if (pRoomEx->pLogicalRoomInfo->dwFlags & 2)
		{
			DRLGGRID_FreeGrid(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pLogicalRoomInfo->pIndexX);
			DRLGGRID_FreeGrid(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pLogicalRoomInfo->pIndexY);
		}

		pRoomCoordList = pRoomEx->pLogicalRoomInfo->pCoordList;
		while (pRoomCoordList)
		{
			pNext = pRoomCoordList->pNext;
			D2_FREE_POOL(pRoomEx->pLevel->pDrlg->pMempool, pRoomCoordList);
			pRoomCoordList = pNext;
		}

		D2_FREE_POOL(pRoomEx->pLevel->pDrlg->pMempool, pRoomEx->pLogicalRoomInfo);
		pRoomEx->pLogicalRoomInfo = NULL;
	}
}

//D2Common.0x6FD764A0
//TODO: v28, a1
void __fastcall DRLGLOGIC_InitializeDrlgCoordList(D2RoomExStrc* pRoomEx, D2DrlgGridStrc* pTileTypeGrid, D2DrlgGridStrc* pFloorGrid, D2DrlgGridStrc* pWallGrid)
{
	D2DrlgLogicalRoomInfoStrc* pDrlgCoordList = NULL;
	D2DrlgRoomTilesStrc* pDrlgRoomTiles = NULL;
	D2DrlgTileDataStrc* pWallTileData = NULL;
	D2RoomExStrc* pRoomExNear = NULL;
	D2DrlgGridStrc pDrlgGrid = {};
	D2UnkDrlgLogicStrc a1 = {};
	int nCellPositions[1024] = {};
	int nCellFlags[256] = {};
	int v28 = 0;
	int nLists = 0;
	int nX = 0;
	int nY = 0;

	pDrlgCoordList = D2_CALLOC_STRC_POOL(pRoomEx->pLevel->pDrlg->pMempool, D2DrlgLogicalRoomInfoStrc);
	pRoomEx->pLogicalRoomInfo = pDrlgCoordList;

	pDrlgRoomTiles = &pRoomEx->pTileGrid->pTiles;
	for (int i = 0; i < pDrlgRoomTiles->nWalls; ++i)
	{
		pWallTileData = &pDrlgRoomTiles->pWallTiles[i];

		if (pWallTileData->dwFlags & 4 && !(pWallTileData->dwFlags & 0x1C000))
		{
			DRLGGRID_AlterGridFlag(pWallGrid, pWallTileData->nPosX, pWallTileData->nPosY, 8, FLAG_OPERATION_OR);
		}
	}

	pDrlgCoordList->dwFlags |= 2;

	DRLGGRID_InitializeGridCells(pRoomEx->pLevel->pDrlg->pMempool, &pDrlgCoordList->pIndexX, pRoomEx->nTileWidth + 1, pRoomEx->nTileHeight + 1);

	if (!pRoomEx->pLevel->nCoordLists)
	{
		pRoomEx->pLevel->nCoordLists = 1;
	}

	nLists = pRoomEx->pLevel->nCoordLists;

	DRLGGRID_FillGrid(&pDrlgGrid, pRoomEx->nTileWidth + 1, pRoomEx->nTileHeight + 1, nCellPositions, nCellFlags);

	pDrlgRoomTiles = &pRoomEx->pTileGrid->pTiles;
	for (int i = 0; i < pDrlgRoomTiles->nWalls; ++i)
	{
		pWallTileData = &pDrlgRoomTiles->pWallTiles[i];
		if ((pWallTileData->dwFlags & 0x1C000) == 0x4000 && pWallTileData->nTileType != TILETYPE_ROOFS && ((unsigned int)~pWallTileData->dwFlags >> 11) & 1)
		{
			DRLGGRID_AlterGridFlag(&pDrlgGrid, pWallTileData->nPosX, pWallTileData->nPosY, 1, FLAG_OPERATION_OR);
		}
	}

	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		pRoomExNear = pRoomEx->ppRoomsNear[i];
		if (pRoomExNear != pRoomEx && pRoomExNear->pTileGrid)
		{
			for (D2DrlgTileLinkStrc* pDrlgTileLink = pRoomExNear->pTileGrid->pMapLinks; pDrlgTileLink; pDrlgTileLink = pDrlgTileLink->pNext)
			{
				if (!pDrlgTileLink->bFloor)
				{
					for (D2DrlgTileDataStrc* pDrlgTileData = pDrlgTileLink->pMapTile; pDrlgTileData; pDrlgTileData = pDrlgTileData->unk0x20)
					{
						if (DRLG_CheckLayer1ButNotWallObject(pDrlgTileData))
						{
							nX = pRoomExNear->nTileXPos + pDrlgTileData->nPosX;
							nY = pRoomExNear->nTileYPos + pDrlgTileData->nPosY;
							if (DRLGROOM_AreXYInsideCoordinatesOrOnBorder(&pRoomEx->pDrlgCoord, nX, nY))
							{
								DRLGGRID_AlterGridFlag(&pDrlgGrid, nX - pRoomEx->nTileXPos, nY - pRoomEx->nTileYPos, 1, FLAG_OPERATION_OR);
							}
						}
					}
				}
			}
		}
	}

	memset(&a1, 0x00, sizeof(a1));
	a1.field_4 = &pDrlgCoordList->pIndexX;
	a1.pTileTypeGrid = pTileTypeGrid;
	a1.pRoomEx = pRoomEx;
	a1.pWallGrid = pWallGrid;
	a1.pFloorGrid = pFloorGrid;
	a1.field_14 = &pDrlgGrid;
	a1.field_18 = nLists;

	for (int j = 0; j <= pRoomEx->nTileHeight; ++j)
	{
		for (int i = 0; i <= pRoomEx->nTileWidth; ++i)
		{
			if (!(DRLGGRID_GetGridEntry(a1.field_4, i, j) & 0x10000000))
			{
				++a1.field_18;
				a1.nFlags = a1.field_18 & 0xFFFFFFF | 0x10000000;

				v28 = DRLGGRID_GetGridEntry(a1.pFloorGrid, i, j);
				if ((v28 & 0x1E0FF00) == 0x1E00000 || (v28 & 0x80000000))
				{
					a1.nFlags |= 0x20000000;
				}

				DRLGLOGIC_SetTileGridFlags(&a1, i, j, -1);
			}
		}
	}

	DRLGGRID_ResetGrid(&pDrlgGrid);

	pDrlgCoordList->nLists = a1.field_18 - nLists + 1;

	pDrlgCoordList->pCoordList = (D2RoomCoordListStrc *)D2_CALLOC_POOL(pRoomEx->pLevel->pDrlg->pMempool, sizeof(D2RoomCoordListStrc) * pDrlgCoordList->nLists);
	
	pRoomEx->pLevel->nCoordLists += pDrlgCoordList->nLists;

	DRLGLOGIC_AssignCoordListsForGrids(pRoomEx, pDrlgCoordList, nLists);
	DRLGLOGIC_SetCoordListForTiles(pRoomEx);
	sub_6FD769B0(pRoomEx);
}

//D2Common.0x6FD76830
void __fastcall DRLGLOGIC_SetTileGridFlags(D2UnkDrlgLogicStrc* a1, int nX, int nY, int a4)
{
	static const D2CoordStrc stru_6FDCE5C8[] =
	{
		{ 1, 0 },
		{ 0, 1 },
		{ -1, 0 },
		{ 0, -1 },
	};

	static const int dword_6FDCE5EC[] =
	{
		23, 0, 5, 21, 17, 15, 3, 0, 9, 7, 39, 0, 0, 5, 3, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 0
	};

	static const int dword_6FDCE650[] =
	{
		-1, 0, 1, 2, 2, 0, 1, 3, 0, 1, 0, 1, 4, -1, 4, 0, 0, 0, 0, 0
	};

	int nPosX = nX;
	int nPosY = nY;
	int nIndex = 0;
	int nFlags = 0;

	while (DRLGROOM_AreXYInsideCoordinatesOrOnBorder(&a1->pRoomEx->pDrlgCoord, nPosX + a1->pRoomEx->nTileXPos, nPosY + a1->pRoomEx->nTileYPos))
	{
		if (DRLGGRID_GetGridEntry(a1->field_4, nPosX, nPosY) & 0x10000000)
		{
			return;
		}

		if (!DRLGGRID_GetGridEntry(a1->field_14, nPosX, nPosY))
		{
			DRLGGRID_AlterGridFlag(a1->field_4, nPosX, nPosY, a1->nFlags, FLAG_OPERATION_OR);

			for (int i = 0; i < 4; ++i)
			{
				DRLGLOGIC_SetTileGridFlags(a1, nPosX + stru_6FDCE5C8[i].nX, nPosY + stru_6FDCE5C8[i].nY, i);
			}
			return;
		}

		nIndex = 0;
		if (DRLGGRID_IsGridValid(a1->pTileTypeGrid))
		{
			nIndex = DRLGGRID_GetGridEntry(a1->pTileTypeGrid, nPosX, nPosY);
		}

		nFlags = dword_6FDCE5EC[a4 + 5 * dword_6FDCE650[nIndex] + 1];
		if (nFlags & 1)
		{
			DRLGGRID_AlterGridFlag(a1->field_4, nPosX, nPosY, a1->nFlags, FLAG_OPERATION_OR);
		}

		if (nFlags & 2 && a4 != 2)
		{
			DRLGLOGIC_SetTileGridFlags(a1, nPosX + 1, nPosY, 0);
		}

		if (nFlags & 4 && a4 != 3)
		{
			DRLGLOGIC_SetTileGridFlags(a1, nPosX, nPosY + 1, 1);
		}

		if (nFlags & 8 && a4 != 0)
		{
			DRLGLOGIC_SetTileGridFlags(a1, nPosX - 1, nPosY, 2);
		}

		if (nFlags & 16 && a4 != 1)
		{
			DRLGLOGIC_SetTileGridFlags(a1, nPosX, nPosY - 1, 3);
		}

		if (!(nFlags & 32))
		{
			return;
		}

		++nPosY;
		++nPosX;

		a4 = -1;
	}
}

//D2Common.0x6FD769B0
void __fastcall sub_6FD769B0(D2RoomExStrc* pRoomEx)
{
	D2RoomExStrc* pCurrentRoomEx = NULL;

	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		pCurrentRoomEx = pRoomEx->ppRoomsNear[i];
		if (pCurrentRoomEx != pRoomEx)
		{
			if (pCurrentRoomEx->pLogicalRoomInfo)
			{
				if (!DRLG_ComputeRectanglesManhattanDistance(&pRoomEx->pDrlgCoord, &pCurrentRoomEx->pDrlgCoord, 1))
				{
					for (int j = pRoomEx->pDrlgCoord.nPosX; j <= pRoomEx->pDrlgCoord.nPosX + pRoomEx->nTileWidth; ++j)
					{
						sub_6FD76A90(pRoomEx, pCurrentRoomEx, j, pRoomEx->pDrlgCoord.nPosY);
						sub_6FD76A90(pRoomEx, pCurrentRoomEx, j, pRoomEx->nTileHeight + pRoomEx->pDrlgCoord.nPosY);
					}

					for (int j = pRoomEx->pDrlgCoord.nPosY; j <= pRoomEx->pDrlgCoord.nPosY + pRoomEx->nTileHeight; ++j)
					{
						sub_6FD76A90(pRoomEx, pCurrentRoomEx, pRoomEx->pDrlgCoord.nPosX, j);
						sub_6FD76A90(pRoomEx, pCurrentRoomEx, pRoomEx->pDrlgCoord.nPosX + pRoomEx->nTileWidth, j);
					}
				}
			}
		}
	}
}

//D2Common.0x6FD76A90
void __fastcall sub_6FD76A90(D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2, int nX, int nY)
{
	D2RoomCoordListStrc* pRoomCoordList1 = NULL;
	D2RoomCoordListStrc* pRoomCoordList2 = NULL;
	int nIndex1 = 0;
	int nIndex2 = 0;

	if (DRLGROOM_AreXYInsideCoordinatesOrOnBorder(&pRoomEx2->pDrlgCoord, nX, nY))
	{
		pRoomCoordList1 = sub_6FD77110(pRoomEx1, nX * 5, nY * 5);
		pRoomCoordList2 = sub_6FD77110(pRoomEx2, nX * 5, nY * 5);

		nIndex1 = pRoomCoordList1->nIndex;
		nIndex2 = pRoomCoordList2->nIndex;

		if (nIndex2 && nIndex1)
		{
			if (pRoomEx2->pLevel->nLevelId == pRoomEx1->pLevel->nLevelId && nIndex2 != nIndex1 && pRoomCoordList2->bNode == pRoomCoordList1->bNode)
			{
				sub_6FD76B90(pRoomEx1, nIndex1, nIndex2, pRoomCoordList2->bNode);
			}
		}
	}
}

//D2Common.0x6FD76B90
void __fastcall sub_6FD76B90(D2RoomExStrc* pRoomEx, int nIndex1, int nIndex2, BOOL bNode)
{
	D2RoomCoordListStrc* pRoomCoordList = NULL;
	BOOL bContinue = FALSE;

	if (pRoomEx->pLogicalRoomInfo && !(pRoomEx->pLogicalRoomInfo->dwFlags & 1))
	{
		pRoomCoordList = pRoomEx->pLogicalRoomInfo->pCoordList;

		if (pRoomCoordList)
		{
			do
			{
				if (pRoomCoordList->nIndex == nIndex1)
				{
					bContinue = TRUE;
					pRoomCoordList->nIndex = nIndex2;
				}
				pRoomCoordList = pRoomCoordList->pNext;
			}
			while (pRoomCoordList);

			if (bContinue)
			{
				for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
				{
					if (pRoomEx->ppRoomsNear[i] != pRoomEx && pRoomEx->ppRoomsNear[i]->pLevel->nLevelId == pRoomEx->pLevel->nLevelId)
					{
						sub_6FD76B90(pRoomEx->ppRoomsNear[i], nIndex1, nIndex2, bNode);
					}
				}
			}
		}
	}
}

//D2Common.0x6FD76C20
BOOL __fastcall DRLG_CheckLayer1ButNotWallObject(D2DrlgTileDataStrc* pTileData)
{
	if (GetMapTileLayer(pTileData->dwFlags) == 1)
	{
		if (pTileData->nTileType != TILETYPE_ROOFS)
		{
			return (pTileData->dwFlags & MAPTILE_OBJECT_WALL) == 0;
		}
	}

	return FALSE;
}

//D2Common.0x6FD76C50
void __fastcall DRLGLOGIC_SetCoordListForTiles(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->pTileGrid->pTiles.nWalls)
	{
		for (int i = 0; i < pRoomEx->pTileGrid->pTiles.nWalls; ++i)
		{
			if (pRoomEx->pLogicalRoomInfo && !(pRoomEx->pLogicalRoomInfo->dwFlags & 1))
			{
				pRoomEx->pTileGrid->pTiles.pWallTiles[i].unk0x10 = DRLGGRID_GetGridEntry(&pRoomEx->pLogicalRoomInfo->pIndexY, pRoomEx->pTileGrid->pTiles.pWallTiles[i].nPosX, pRoomEx->pTileGrid->pTiles.pWallTiles[i].nPosY);
			}
			else
			{
				pRoomEx->pTileGrid->pTiles.pWallTiles[i].unk0x10 = 0;
			}
		}
	}
}

//D2Common.0x6FD76CF0
void __fastcall DRLGLOGIC_AssignCoordListsForGrids(D2RoomExStrc* pRoomEx, D2DrlgLogicalRoomInfoStrc* pDrlgCoordList, int nLists)
{
	DRLGGRID_InitializeGridCells(pRoomEx->pLevel->pDrlg->pMempool, &pDrlgCoordList->pIndexY, pRoomEx->nTileWidth + 1, pRoomEx->nTileHeight + 1);
	const int nWidth = pRoomEx->nTileWidth + 1;
	const int nHeight = pRoomEx->nTileHeight + 1;

	for (int nY = 0; nY < nHeight; ++nY)
	{
		for (int nX = 0; nX < nWidth; ++nX)
		{
			const int nFlags = DRLGGRID_GetGridEntry(&pDrlgCoordList->pIndexX, nX, nY);
			const int nIndex = nFlags & 0xFFFFFFF;

			if (!DRLGGRID_GetGridEntry(&pDrlgCoordList->pIndexY, nX, nY))
			{
				D2RoomCoordListStrc* pRoomCoordList = D2_CALLOC_STRC_POOL(pRoomEx->pLevel->pDrlg->pMempool, D2RoomCoordListStrc);

				pRoomCoordList->nIndex = nIndex;
				pRoomCoordList->bNode = (nFlags & 0x20000000) == 0x20000000;
				pRoomCoordList->pNext = pDrlgCoordList->pCoordList;

				pDrlgCoordList->pCoordList = pRoomCoordList;

				pRoomCoordList->pBox[0].nPosX = nX;
				pRoomCoordList->pBox[0].nPosY = nY;
				int nTmpWidth;
				for (nTmpWidth = nX; nTmpWidth < nWidth; ++nTmpWidth)
				{
					if (nIndex != (DRLGGRID_GetGridEntry(&pDrlgCoordList->pIndexX, nTmpWidth, nY) & 0xFFFFFFF))
					{
						break;
					}

					if (DRLGGRID_GetGridEntry(&pDrlgCoordList->pIndexY, nTmpWidth, nY))
					{
						break;
					}
				}
				pRoomCoordList->pBox[0].nWidth = nTmpWidth;

				BOOL bBreak = FALSE;
				int nTmpHeight;
				for (nTmpHeight = nY; nTmpHeight < nHeight; ++nTmpHeight)
				{
					for (int i = nX; i < pRoomCoordList->pBox[0].nWidth; ++i)
					{
						if (nIndex != (DRLGGRID_GetGridEntry(&pDrlgCoordList->pIndexX, i, nTmpHeight) & 0xFFFFFFF) || DRLGGRID_GetGridEntry(&pDrlgCoordList->pIndexY, i, nTmpHeight))
						{
							bBreak = TRUE;
							break;
						}
					}

					if (bBreak)
					{
						break;
					}
				}
				pRoomCoordList->pBox[0].nHeight = nTmpHeight;

				for (int j = pRoomCoordList->pBox[0].nPosY; j < pRoomCoordList->pBox[0].nHeight; ++j)
				{
					for (int i = pRoomCoordList->pBox[0].nPosX; i < pRoomCoordList->pBox[0].nWidth; ++i)
					{
						DRLGGRID_AlterGridFlag(&pDrlgCoordList->pIndexY, i, j, (int)pRoomCoordList, FLAG_OPERATION_OVERWRITE);
					}
				}

				pRoomCoordList->pBox[0].nPosY += pRoomEx->nTileYPos;
				pRoomCoordList->pBox[0].nHeight += pRoomEx->nTileYPos;
				pRoomCoordList->pBox[0].nPosX += pRoomEx->nTileXPos;
				pRoomCoordList->pBox[0].nWidth += pRoomEx->nTileXPos;

				pRoomCoordList->pBox[1].nPosX = pRoomCoordList->pBox[0].nPosX;
				pRoomCoordList->pBox[1].nPosY = pRoomCoordList->pBox[0].nPosY;
				pRoomCoordList->pBox[1].nWidth = pRoomCoordList->pBox[0].nWidth;
				pRoomCoordList->pBox[1].nHeight = pRoomCoordList->pBox[0].nHeight;

				nTmpWidth = pRoomEx->nTileXPos + pRoomEx->nTileWidth;
				if (pRoomCoordList->pBox[1].nWidth >= nTmpWidth)
				{
					pRoomCoordList->pBox[1].nWidth = nTmpWidth;
				}

				nTmpHeight = pRoomEx->nTileYPos + pRoomEx->nTileHeight;
				if (pRoomCoordList->pBox[1].nHeight >= nTmpHeight)
				{
					pRoomCoordList->pBox[1].nHeight = nTmpHeight;
				}

				if (pRoomCoordList->pBox[1].nPosX >= nTmpWidth || pRoomCoordList->pBox[1].nPosY >= nTmpHeight)
				{
					pRoomCoordList->pBox[1].nPosX = 0;
					pRoomCoordList->pBox[1].nPosY = 0;
					pRoomCoordList->pBox[1].nWidth = 0;
					pRoomCoordList->pBox[1].nHeight = 0;
				}
			}
		}
	}
}

//D2Common.0x6FD76F90
void __fastcall DRLGLOGIC_AllocCoordLists(D2RoomExStrc* pRoomEx)
{
	D2DrlgLogicalRoomInfoStrc* pLogicalRoomInfo = D2_CALLOC_STRC_POOL(pRoomEx->pLevel->pDrlg->pMempool, D2DrlgLogicalRoomInfoStrc);
	pRoomEx->pLogicalRoomInfo = pLogicalRoomInfo;

	pLogicalRoomInfo->dwFlags |= 1;
	pRoomEx->pLevel->nCoordLists = 1;
	pLogicalRoomInfo->nLists = 1;

	pLogicalRoomInfo->pCoordList = D2_CALLOC_STRC_POOL(pRoomEx->pLevel->pDrlg->pMempool, D2RoomCoordListStrc);
	
	pLogicalRoomInfo->pCoordList->nIndex = pRoomEx->pLevel->nCoordLists;
	pLogicalRoomInfo->pCoordList->pBox[0].nPosY = pRoomEx->nTileYPos;
	pLogicalRoomInfo->pCoordList->pBox[0].nHeight = pRoomEx->nTileYPos + pRoomEx->nTileHeight;
	pLogicalRoomInfo->pCoordList->pBox[0].nPosX = pRoomEx->nTileXPos;
	pLogicalRoomInfo->pCoordList->pBox[0].nWidth = pRoomEx->nTileXPos + pRoomEx->nTileWidth;
	pLogicalRoomInfo->pCoordList->pBox[1].nPosY = pRoomEx->nTileYPos;
	pLogicalRoomInfo->pCoordList->pBox[1].nHeight = pRoomEx->nTileYPos + pRoomEx->nTileHeight;
	pLogicalRoomInfo->pCoordList->pBox[1].nPosX = pRoomEx->nTileXPos;
	pLogicalRoomInfo->pCoordList->pBox[1].nWidth = pRoomEx->nTileXPos + pRoomEx->nTileWidth;
}

//D2Common.0x6FD77080
int __fastcall DRLGLOGIC_GetRoomCoordListIndex(D2RoomExStrc* pRoomEx, int nX, int nY)
{
	D2RoomCoordListStrc* pRoomCoordList = sub_6FD77110(pRoomEx, nX, nY);

	if (pRoomCoordList)
	{
		return pRoomCoordList->nIndex;
	}

	return -1;
}

//D2Common.0x6FD77110
D2RoomCoordListStrc* __fastcall sub_6FD77110(D2RoomExStrc* pRoomEx, int nX, int nY)
{
	D2_ASSERT(pRoomEx->pLogicalRoomInfo);

	if (pRoomEx->pLogicalRoomInfo->dwFlags & 1)
	{
		return pRoomEx->pLogicalRoomInfo->pCoordList;
	}
	else
	{
		return (D2RoomCoordListStrc*)DRLGGRID_GetGridEntry(&pRoomEx->pLogicalRoomInfo->pIndexY, nX / 5 - pRoomEx->nTileXPos, nY / 5 - pRoomEx->nTileYPos);
	}
}

//D2Common.0x6FD77190
D2RoomCoordListStrc* __fastcall DRLGLOGIC_GetRoomCoordList(D2RoomExStrc* pRoomEx)
{
	D2_ASSERT(pRoomEx->pLogicalRoomInfo);
	return pRoomEx->pLogicalRoomInfo->pCoordList;
}
