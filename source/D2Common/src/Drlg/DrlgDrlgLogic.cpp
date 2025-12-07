#include <D2Seed.h>

#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgLogic.h"
#include "Drlg/D2DrlgDrlgAnim.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "Drlg/D2DrlgRoomTile.h"
#include "D2CMP.h"

//D2Common.0x6FD76420
void __fastcall DRLGLOGIC_FreeDrlgCoordList(D2DrlgRoomStrc* pDrlgRoom)
{

	if (pDrlgRoom->pLogicalRoomInfo)
	{
		if (pDrlgRoom->pLogicalRoomInfo->dwFlags & DRLGLOGIC_ROOMINFO_HAS_GRID_CELLS)
		{
			DRLGGRID_FreeGrid(pDrlgRoom->pLevel->pDrlg->pMempool, &pDrlgRoom->pLogicalRoomInfo->pIndexX);
			DRLGGRID_FreeGrid(pDrlgRoom->pLevel->pDrlg->pMempool, &pDrlgRoom->pLogicalRoomInfo->pIndexY);
		}

		D2RoomCoordListStrc* pRoomCoordList = pRoomCoordList = pDrlgRoom->pLogicalRoomInfo->pCoordList;
		while (pRoomCoordList)
		{
			D2RoomCoordListStrc* pNext = pRoomCoordList->pNext;
			D2_FREE_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, pRoomCoordList);
			pRoomCoordList = pNext;
		}

		D2_FREE_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, pDrlgRoom->pLogicalRoomInfo);
		pDrlgRoom->pLogicalRoomInfo = NULL;
	}
}

//D2Common.0x6FD764A0
void __fastcall DRLGLOGIC_InitializeDrlgCoordList(D2DrlgRoomStrc* pDrlgRoom, D2DrlgGridStrc* pTileTypeGrid, D2DrlgGridStrc* pFloorGrid, D2DrlgGridStrc* pWallGrid)
{
	int nCellPositions[1024] = {};
	int nCellFlags[256] = {};
	
	D2DrlgLogicalRoomInfoStrc* pDrlgCoordList = D2_CALLOC_STRC_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, D2DrlgLogicalRoomInfoStrc);
	pDrlgRoom->pLogicalRoomInfo = pDrlgCoordList;

	D2DrlgRoomTilesStrc* pDrlgRoomTiles = &pDrlgRoom->pTileGrid->pTiles;
	for (int i = 0; i < pDrlgRoomTiles->nWalls; ++i)
	{
		D2DrlgTileDataStrc* pWallTileData = &pDrlgRoomTiles->pWallTiles[i];

		if (pWallTileData->dwFlags & MAPTILE_TREES && !HasMapTileLayer(pWallTileData->dwFlags))
		{
			DRLGGRID_AlterGridFlag(pWallGrid, pWallTileData->nPosX, pWallTileData->nPosY, 8, FLAG_OPERATION_OR);
		}
	}

	pDrlgCoordList->dwFlags |= DRLGLOGIC_ROOMINFO_HAS_GRID_CELLS;

	DRLGGRID_InitializeGridCells(pDrlgRoom->pLevel->pDrlg->pMempool, &pDrlgCoordList->pIndexX, pDrlgRoom->nTileWidth + 1, pDrlgRoom->nTileHeight + 1);

	if (!pDrlgRoom->pLevel->nCoordLists)
	{
		pDrlgRoom->pLevel->nCoordLists = 1;
	}

	int nLists = pDrlgRoom->pLevel->nCoordLists;

	D2DrlgGridStrc pDrlgGrid = {};
	DRLGGRID_FillGrid(&pDrlgGrid, pDrlgRoom->nTileWidth + 1, pDrlgRoom->nTileHeight + 1, nCellPositions, nCellFlags);

	pDrlgRoomTiles = &pDrlgRoom->pTileGrid->pTiles;
	for (int i = 0; i < pDrlgRoomTiles->nWalls; ++i)
	{
		D2DrlgTileDataStrc* pWallTileData = &pDrlgRoomTiles->pWallTiles[i];
		if (GetMapTileLayer(pWallTileData->dwFlags) == 1 && pWallTileData->nTileType != TILETYPE_ROOF && !(pWallTileData->dwFlags &MAPTILE_OBJECT_WALL))
		{
			DRLGGRID_AlterGridFlag(&pDrlgGrid, pWallTileData->nPosX, pWallTileData->nPosY, 1, FLAG_OPERATION_OR);
		}
	}

	for (int i = 0; i < pDrlgRoom->nRoomsNear; ++i)
	{
		D2DrlgRoomStrc* pDrlgRoomNear = pDrlgRoom->ppRoomsNear[i];
		if (pDrlgRoomNear != pDrlgRoom && pDrlgRoomNear->pTileGrid)
		{
			for (D2DrlgTileLinkStrc* pDrlgTileLink = pDrlgRoomNear->pTileGrid->pMapLinks; pDrlgTileLink; pDrlgTileLink = pDrlgTileLink->pNext)
			{
				if (!pDrlgTileLink->bFloor)
				{
					for (D2DrlgTileDataStrc* pDrlgTileData = pDrlgTileLink->pMapTile; pDrlgTileData; pDrlgTileData = pDrlgTileData->unk0x20)
					{
						if (DRLG_CheckLayer1ButNotWallObject(pDrlgTileData))
						{
							const int nX = pDrlgRoomNear->nTileXPos + pDrlgTileData->nPosX;
							const int nY = pDrlgRoomNear->nTileYPos + pDrlgTileData->nPosY;
							if (DRLGROOM_AreXYInsideCoordinatesOrOnBorder(&pDrlgRoom->pDrlgCoord, nX, nY))
							{
								DRLGGRID_AlterGridFlag(&pDrlgGrid, nX - pDrlgRoom->nTileXPos, nY - pDrlgRoom->nTileYPos, 1, FLAG_OPERATION_OR);
							}
						}
					}
				}
			}
		}
	}

	D2UnkDrlgLogicStrc tDRLGLogicUnkStrc = {};
	memset(&tDRLGLogicUnkStrc, 0x00, sizeof(tDRLGLogicUnkStrc));
	tDRLGLogicUnkStrc.field_4 = &pDrlgCoordList->pIndexX;
	tDRLGLogicUnkStrc.pTileTypeGrid = pTileTypeGrid;
	tDRLGLogicUnkStrc.pDrlgRoom = pDrlgRoom;
	tDRLGLogicUnkStrc.pWallGrid = pWallGrid;
	tDRLGLogicUnkStrc.pFloorGrid = pFloorGrid;
	tDRLGLogicUnkStrc.field_14 = &pDrlgGrid;
	tDRLGLogicUnkStrc.field_18 = nLists;

	for (int j = 0; j <= pDrlgRoom->nTileHeight; ++j)
	{
		for (int i = 0; i <= pDrlgRoom->nTileWidth; ++i)
		{
			if (!(DRLGGRID_GetGridEntry(tDRLGLogicUnkStrc.field_4, i, j) & 0x10000000))
			{
				++tDRLGLogicUnkStrc.field_18;
				tDRLGLogicUnkStrc.nFlags = (tDRLGLogicUnkStrc.field_18 & 0xFFFFFFF) | 0x10000000;

				D2C_PackedTileInformation nFloorPackedInfo = { (uint32_t)DRLGGRID_GetGridEntry(tDRLGLogicUnkStrc.pFloorGrid, i, j) };
				if((nFloorPackedInfo.nTileStyle == 30 && nFloorPackedInfo.nWallLayer == 0) || nFloorPackedInfo.bHidden)
				{
					tDRLGLogicUnkStrc.nFlags |= 0x20000000;
				}

				DRLGLOGIC_SetTileGridFlags(&tDRLGLogicUnkStrc, i, j, -1);
			}
		}
	}

	DRLGGRID_ResetGrid(&pDrlgGrid);

	pDrlgCoordList->nLists = tDRLGLogicUnkStrc.field_18 - nLists + 1;

	pDrlgCoordList->pCoordList = (D2RoomCoordListStrc *)D2_CALLOC_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, sizeof(D2RoomCoordListStrc) * pDrlgCoordList->nLists);
	
	pDrlgRoom->pLevel->nCoordLists += pDrlgCoordList->nLists;

	DRLGLOGIC_AssignCoordListsForGrids(pDrlgRoom, pDrlgCoordList, nLists);
	DRLGLOGIC_SetCoordListForTiles(pDrlgRoom);
	sub_6FD769B0(pDrlgRoom);
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

	while (DRLGROOM_AreXYInsideCoordinatesOrOnBorder(&a1->pDrlgRoom->pDrlgCoord, nPosX + a1->pDrlgRoom->nTileXPos, nPosY + a1->pDrlgRoom->nTileYPos))
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

		int nIndex = 0;
		if (DRLGGRID_IsGridValid(a1->pTileTypeGrid))
		{
			nIndex = DRLGGRID_GetGridEntry(a1->pTileTypeGrid, nPosX, nPosY);
		}

		const int nFlags = dword_6FDCE5EC[a4 + 5 * dword_6FDCE650[nIndex] + 1];
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
void __fastcall sub_6FD769B0(D2DrlgRoomStrc* pDrlgRoom)
{
	for (int i = 0; i < pDrlgRoom->nRoomsNear; ++i)
	{
		D2DrlgRoomStrc* pCurrentRoomEx = pDrlgRoom->ppRoomsNear[i];
		if (pCurrentRoomEx != pDrlgRoom)
		{
			if (pCurrentRoomEx->pLogicalRoomInfo)
			{
				if (!DRLG_CheckNotOverlappingUsingManhattanDistance(&pDrlgRoom->pDrlgCoord, &pCurrentRoomEx->pDrlgCoord, 1))
				{
					for (int j = pDrlgRoom->pDrlgCoord.nPosX; j <= pDrlgRoom->pDrlgCoord.nPosX + pDrlgRoom->nTileWidth; ++j)
					{
						sub_6FD76A90(pDrlgRoom, pCurrentRoomEx, j, pDrlgRoom->pDrlgCoord.nPosY);
						sub_6FD76A90(pDrlgRoom, pCurrentRoomEx, j, pDrlgRoom->nTileHeight + pDrlgRoom->pDrlgCoord.nPosY);
					}

					for (int j = pDrlgRoom->pDrlgCoord.nPosY; j <= pDrlgRoom->pDrlgCoord.nPosY + pDrlgRoom->nTileHeight; ++j)
					{
						sub_6FD76A90(pDrlgRoom, pCurrentRoomEx, pDrlgRoom->pDrlgCoord.nPosX, j);
						sub_6FD76A90(pDrlgRoom, pCurrentRoomEx, pDrlgRoom->pDrlgCoord.nPosX + pDrlgRoom->nTileWidth, j);
					}
				}
			}
		}
	}
}

//D2Common.0x6FD76A90
void __fastcall sub_6FD76A90(D2DrlgRoomStrc* pDrlgRoom1, D2DrlgRoomStrc* pDrlgRoom2, int nX, int nY)
{
	if (DRLGROOM_AreXYInsideCoordinatesOrOnBorder(&pDrlgRoom2->pDrlgCoord, nX, nY))
	{
		D2RoomCoordListStrc* pRoomCoordList1 = sub_6FD77110(pDrlgRoom1, nX * 5, nY * 5);
		D2RoomCoordListStrc* pRoomCoordList2 = sub_6FD77110(pDrlgRoom2, nX * 5, nY * 5);

		const int nIndex1 = pRoomCoordList1->nIndex;
		const int nIndex2 = pRoomCoordList2->nIndex;

		if (nIndex2 && nIndex1)
		{
			if (pDrlgRoom2->pLevel->nLevelId == pDrlgRoom1->pLevel->nLevelId && nIndex2 != nIndex1 && pRoomCoordList2->bNode == pRoomCoordList1->bNode)
			{
				sub_6FD76B90(pDrlgRoom1, nIndex1, nIndex2, pRoomCoordList2->bNode);
			}
		}
	}
}

//D2Common.0x6FD76B90
void __fastcall sub_6FD76B90(D2DrlgRoomStrc* pDrlgRoom, int nIndex1, int nIndex2, BOOL bNode)
{
	if (pDrlgRoom->pLogicalRoomInfo ==nullptr || (pDrlgRoom->pLogicalRoomInfo->dwFlags & DRLGLOGIC_ROOMINFO_HAS_COORD_LIST))
	{
		return;
	}

	if (D2RoomCoordListStrc* pRoomCoordList = pDrlgRoom->pLogicalRoomInfo->pCoordList)
	{
		bool bContinue = false;
		do
		{
			if (pRoomCoordList->nIndex == nIndex1)
			{
				bContinue = true;
				pRoomCoordList->nIndex = nIndex2;
			}
			pRoomCoordList = pRoomCoordList->pNext;
		}
		while (pRoomCoordList);

		if (bContinue)
		{
			for (int i = 0; i < pDrlgRoom->nRoomsNear; ++i)
			{
				if (pDrlgRoom->ppRoomsNear[i] != pDrlgRoom && pDrlgRoom->ppRoomsNear[i]->pLevel->nLevelId == pDrlgRoom->pLevel->nLevelId)
				{
					sub_6FD76B90(pDrlgRoom->ppRoomsNear[i], nIndex1, nIndex2, bNode);
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
		if (pTileData->nTileType != TILETYPE_ROOF)
		{
			return (pTileData->dwFlags & MAPTILE_OBJECT_WALL) == 0;
		}
	}

	return FALSE;
}

//D2Common.0x6FD76C50
void __fastcall DRLGLOGIC_SetCoordListForTiles(D2DrlgRoomStrc* pDrlgRoom)
{
	if (pDrlgRoom->pTileGrid->pTiles.nWalls)
	{
		for (int i = 0; i < pDrlgRoom->pTileGrid->pTiles.nWalls; ++i)
		{
			if (pDrlgRoom->pLogicalRoomInfo && !(pDrlgRoom->pLogicalRoomInfo->dwFlags & DRLGLOGIC_ROOMINFO_HAS_COORD_LIST))
			{
				pDrlgRoom->pTileGrid->pTiles.pWallTiles[i].unk0x10 = DRLGGRID_GetGridEntry(&pDrlgRoom->pLogicalRoomInfo->pIndexY, pDrlgRoom->pTileGrid->pTiles.pWallTiles[i].nPosX, pDrlgRoom->pTileGrid->pTiles.pWallTiles[i].nPosY);
			}
			else
			{
				pDrlgRoom->pTileGrid->pTiles.pWallTiles[i].unk0x10 = 0;
			}
		}
	}
}

//D2Common.0x6FD76CF0
void __fastcall DRLGLOGIC_AssignCoordListsForGrids(D2DrlgRoomStrc* pDrlgRoom, D2DrlgLogicalRoomInfoStrc* pDrlgCoordList, int nLists)
{
	DRLGGRID_InitializeGridCells(pDrlgRoom->pLevel->pDrlg->pMempool, &pDrlgCoordList->pIndexY, pDrlgRoom->nTileWidth + 1, pDrlgRoom->nTileHeight + 1);
	const int nWidth = pDrlgRoom->nTileWidth + 1;
	const int nHeight = pDrlgRoom->nTileHeight + 1;

	for (int nY = 0; nY < nHeight; ++nY)
	{
		for (int nX = 0; nX < nWidth; ++nX)
		{
			const int nFlags = DRLGGRID_GetGridEntry(&pDrlgCoordList->pIndexX, nX, nY);
			const int nIndex = nFlags & 0xFFFFFFF;

			if (!DRLGGRID_GetGridEntry(&pDrlgCoordList->pIndexY, nX, nY))
			{
				D2RoomCoordListStrc* pRoomCoordList = D2_CALLOC_STRC_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, D2RoomCoordListStrc);

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

				pRoomCoordList->pBox[0].nPosY += pDrlgRoom->nTileYPos;
				pRoomCoordList->pBox[0].nHeight += pDrlgRoom->nTileYPos;
				pRoomCoordList->pBox[0].nPosX += pDrlgRoom->nTileXPos;
				pRoomCoordList->pBox[0].nWidth += pDrlgRoom->nTileXPos;

				pRoomCoordList->pBox[1].nPosX = pRoomCoordList->pBox[0].nPosX;
				pRoomCoordList->pBox[1].nPosY = pRoomCoordList->pBox[0].nPosY;
				pRoomCoordList->pBox[1].nWidth = pRoomCoordList->pBox[0].nWidth;
				pRoomCoordList->pBox[1].nHeight = pRoomCoordList->pBox[0].nHeight;

				nTmpWidth = pDrlgRoom->nTileXPos + pDrlgRoom->nTileWidth;
				if (pRoomCoordList->pBox[1].nWidth >= nTmpWidth)
				{
					pRoomCoordList->pBox[1].nWidth = nTmpWidth;
				}

				nTmpHeight = pDrlgRoom->nTileYPos + pDrlgRoom->nTileHeight;
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
void __fastcall DRLGLOGIC_AllocCoordLists(D2DrlgRoomStrc* pDrlgRoom)
{
	D2DrlgLogicalRoomInfoStrc* pLogicalRoomInfo = D2_CALLOC_STRC_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, D2DrlgLogicalRoomInfoStrc);
	pDrlgRoom->pLogicalRoomInfo = pLogicalRoomInfo;

	pLogicalRoomInfo->dwFlags |= DRLGLOGIC_ROOMINFO_HAS_COORD_LIST;
	pDrlgRoom->pLevel->nCoordLists = 1;
	pLogicalRoomInfo->nLists = 1;

	pLogicalRoomInfo->pCoordList = D2_CALLOC_STRC_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, D2RoomCoordListStrc);
	
	pLogicalRoomInfo->pCoordList->nIndex = pDrlgRoom->pLevel->nCoordLists;
	pLogicalRoomInfo->pCoordList->pBox[0].nPosY = pDrlgRoom->nTileYPos;
	pLogicalRoomInfo->pCoordList->pBox[0].nHeight = pDrlgRoom->nTileYPos + pDrlgRoom->nTileHeight;
	pLogicalRoomInfo->pCoordList->pBox[0].nPosX = pDrlgRoom->nTileXPos;
	pLogicalRoomInfo->pCoordList->pBox[0].nWidth = pDrlgRoom->nTileXPos + pDrlgRoom->nTileWidth;
	pLogicalRoomInfo->pCoordList->pBox[1].nPosY = pDrlgRoom->nTileYPos;
	pLogicalRoomInfo->pCoordList->pBox[1].nHeight = pDrlgRoom->nTileYPos + pDrlgRoom->nTileHeight;
	pLogicalRoomInfo->pCoordList->pBox[1].nPosX = pDrlgRoom->nTileXPos;
	pLogicalRoomInfo->pCoordList->pBox[1].nWidth = pDrlgRoom->nTileXPos + pDrlgRoom->nTileWidth;
}

//D2Common.0x6FD77080
int __fastcall DRLGLOGIC_GetRoomCoordListIndex(D2DrlgRoomStrc* pDrlgRoom, int nX, int nY)
{
	D2RoomCoordListStrc* pRoomCoordList = sub_6FD77110(pDrlgRoom, nX, nY);

	if (pRoomCoordList)
	{
		return pRoomCoordList->nIndex;
	}

	return -1;
}

//D2Common.0x6FD77110
D2RoomCoordListStrc* __fastcall sub_6FD77110(D2DrlgRoomStrc* pDrlgRoom, int nX, int nY)
{
	D2_ASSERT(pDrlgRoom->pLogicalRoomInfo);

	if (pDrlgRoom->pLogicalRoomInfo->dwFlags & DRLGLOGIC_ROOMINFO_HAS_COORD_LIST)
	{
		return pDrlgRoom->pLogicalRoomInfo->pCoordList;
	}
	else
	{
		return (D2RoomCoordListStrc*)DRLGGRID_GetGridEntry(&pDrlgRoom->pLogicalRoomInfo->pIndexY, nX / 5 - pDrlgRoom->nTileXPos, nY / 5 - pDrlgRoom->nTileYPos);
	}
}

//D2Common.0x6FD77190
D2RoomCoordListStrc* __fastcall DRLGLOGIC_GetRoomCoordList(D2DrlgRoomStrc* pDrlgRoom)
{
	D2_ASSERT(pDrlgRoom->pLogicalRoomInfo);
	return pDrlgRoom->pLogicalRoomInfo->pCoordList;
}
