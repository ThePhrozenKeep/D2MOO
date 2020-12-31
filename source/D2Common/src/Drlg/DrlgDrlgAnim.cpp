#include "Drlg/D2DrlgDrlgAnim.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgRoomTile.h"
#include <D2CMP.h>



//TODO: Variable names


//D2Common.0x6FD75480
void __fastcall DRLGANIM_InitCache(D2DrlgStrc* pDrlg, D2DrlgTileDataStrc* pTileData)
{
	D2TileLibraryEntryStrc* ppTileLibraryEntry[40] = {};
	int nSubIndex = 0;
	int nIndex = 0;

	D2_ASSERT(pDrlg);
	D2_ASSERT(pTileData);

	memset(pTileData, 0x00, sizeof(D2DrlgTileDataStrc));

	if (pDrlg->nAct != ACT_I)
	{
		if (pDrlg->nAct == ACT_II)
		{
			nSubIndex = 1;
		}
		else if (pDrlg->nAct == ACT_III)
		{
			nIndex = 29;
			nSubIndex = 12;
		}
		else
		{
			return;
		}
	}

	const int nSize = D2CMP_10088_GetTileSize(pDrlg->pTiles, 0, nIndex, nSubIndex, ppTileLibraryEntry, ARRAY_SIZE(ppTileLibraryEntry));
	D2_ASSERT(nSize);
	DRLGROOMTILE_InitTileData(NULL, pTileData, 0, 0, 0, ppTileLibraryEntry[0]);
}

//D2Common.0x6FD75560
void __fastcall DRLGANIM_TestLoadAnimatedRoomTiles(D2RoomExStrc* pRoomEx, D2DrlgGridStrc* pDrlgGrid, D2DrlgGridStrc* pOrientationGrid, int nOrientation, int nTileX, int nTileY)
{
	int v11; // ebx@3
	int nTiles; // esi@10
	unsigned __int8 nIndex; // [sp+24h] [bp-B4h]@8
	unsigned __int8 nSubIndex; // [sp+28h] [bp-B0h]@8
	D2TileLibraryEntryStrc* ppTileLibraryEntry[40] = {};

	for (int i = 0; i < pRoomEx->nTileHeight + (nTileY == 0); ++i)
	{
		for (int j = 0; j < pRoomEx->nTileWidth + (nTileX == 0); ++j)
		{
			v11 = DRLGGRID_GetGridFlags(pDrlgGrid, j, i);
			if (v11 & 0x8000003)
			{
				if (pOrientationGrid)
				{
					nOrientation = DRLGGRID_GetGridFlags(pOrientationGrid, j, i);
				}

				if (v11)
				{
					nIndex = ((unsigned int)v11 >> 20) & 63;
					nSubIndex = BYTE1(v11);
				}
				else
				{
					nIndex = 0;
					nSubIndex = 0;
				}

				nTiles = D2CMP_10088_GetTileSize(pRoomEx->pTiles, nOrientation, nIndex, nSubIndex, ppTileLibraryEntry, ARRAY_SIZE(ppTileLibraryEntry));
				if (nTiles && D2CMP_10079_GetTileFlags(ppTileLibraryEntry[0]) & 0x100)
				{
					if (v11 & 2)
					{
						pRoomEx->pTileGrid->pTiles.nFloors += (nTiles - 1);
					}
					else if (v11 & 1)
					{
						pRoomEx->pTileGrid->pTiles.nWalls += (nTiles - 1);
					}
					else
					{
						pRoomEx->pTileGrid->pTiles.nRoofs += (nTiles - 1);
					}

					pRoomEx->dwFlags |= ROOMEXFLAG_ANIMATED_FLOOR;
				}
			}
		}
	}
}

//D2Common.0x6FD756B0
void __fastcall DRLGANIM_AnimateTiles(D2RoomExStrc* pRoomEx)
{
	D2DrlgTileDataStrc* pTileData = NULL;
	D2RoomExStrc* pCurrentRoomEx = NULL;

	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		pCurrentRoomEx = pRoomEx->ppRoomsNear[i];

		if (pCurrentRoomEx->dwFlags & ROOMEXFLAG_ANIMATED_FLOOR && pCurrentRoomEx->pTileGrid)
		{
			for (D2DrlgAnimTileGridStrc* j = pCurrentRoomEx->pTileGrid->pAnimTiles; j; j = j->pNext)
			{
				pTileData = j->ppMapTileData[j->nCurrentFrame >> 8];
				if (pTileData)
				{
					pTileData->dwFlags |= 8;
				}

				j->nCurrentFrame = (j->nCurrentFrame + j->nAnimationSpeed) % (j->nFrames << 8);

				pTileData = j->ppMapTileData[j->nCurrentFrame >> 8];
				if (pTileData)
				{
					pTileData->dwFlags &= 0xFFFFFFF7;
				}
			}
		}
	}
}

//D2Common.0x6FD75740
void __fastcall DRLGANIM_AllocAnimationTileGrids(D2RoomExStrc* pRoomEx, int nAnimationSpeed, D2DrlgGridStrc* pWallGrid, int nWalls, D2DrlgGridStrc* pFloorGrid, int nFloors, D2DrlgGridStrc* pShadowGrid)
{
	DRLGANIM_AllocAnimationTileGrid(pRoomEx, nAnimationSpeed, pRoomEx->pTileGrid->pTiles.pWallTiles, pRoomEx->pTileGrid->nWalls, pWallGrid, nWalls);
	DRLGANIM_AllocAnimationTileGrid(pRoomEx, nAnimationSpeed, pRoomEx->pTileGrid->pTiles.pFloorTiles, pRoomEx->pTileGrid->nFloors, pFloorGrid, nFloors);
	DRLGANIM_AllocAnimationTileGrid(pRoomEx, nAnimationSpeed, pRoomEx->pTileGrid->pTiles.pRoofTiles, pRoomEx->pTileGrid->nShadows, pShadowGrid, 1);
}

//D2Common.0x6FD757B0
void __fastcall DRLGANIM_AllocAnimationTileGrid(D2RoomExStrc* pRoomEx, int nAnimationSpeed, D2DrlgTileDataStrc* pTiles, int nTiles, D2DrlgGridStrc* pDrlgGrid, int nUnused)
{
	D2TileLibraryEntryStrc* pTileLibraryEntries[40] = {};
	D2DrlgAnimTileGridStrc* pDrlgAnimTileGrid = NULL;
	unsigned int v9 = 0;
	unsigned int nFrames = 0;
	unsigned int nIndex = 0;
	int nX = 0;
	int nY = 0;
	uint8_t nStyle = 0;
	uint8_t nSeq = 0;
	char szBuffer[100] = {};
	
	if (pTiles)
	{
		if (!nAnimationSpeed)
		{
			nAnimationSpeed = 80;
		}

		for (int i = 0; i < nTiles; ++i)
		{
			if (pTiles[i].pTile && D2CMP_10079_GetTileFlags(pTiles[i].pTile) & 0x100)
			{
				v9 = DRLGGRID_GetGridFlags(&pDrlgGrid[((pTiles[i].dwFlags >> 14) & 7) - 1], pTiles[i].nPosX, pTiles[i].nPosY);
				if (v9)
				{
					nStyle = (v9 >> 20) & 0x3F;
					nSeq = BYTE1(v9);
				}
				else
				{
					nStyle = 0;
					nSeq = 0;
				}

				nFrames = D2CMP_10088_GetTileSize(pRoomEx->pTiles, pTiles[i].unk0x1C, nStyle, nSeq, pTileLibraryEntries, ARRAY_SIZE(pTileLibraryEntries));

				pDrlgAnimTileGrid = (D2DrlgAnimTileGridStrc*)FOG_AllocServerMemory(pRoomEx->pLevel->pDrlg->pMempool, sizeof(D2DrlgAnimTileGridStrc), __FILE__, __LINE__, 0);
				memset(pDrlgAnimTileGrid, 0x00, sizeof(D2DrlgAnimTileGridStrc));

				pDrlgAnimTileGrid->ppMapTileData = (D2DrlgTileDataStrc**)FOG_AllocServerMemory(pRoomEx->pLevel->pDrlg->pMempool, sizeof(D2DrlgTileDataStrc*) * nFrames, __FILE__, __LINE__, 0);
				memset(pDrlgAnimTileGrid->ppMapTileData, 0x00, sizeof(D2DrlgTileDataStrc*) * nFrames);

				pDrlgAnimTileGrid->nFrames = nFrames;
				pDrlgAnimTileGrid->nAnimationSpeed = nAnimationSpeed;
				pDrlgAnimTileGrid->pNext = pRoomEx->pTileGrid->pAnimTiles;

				pRoomEx->pTileGrid->pAnimTiles = pDrlgAnimTileGrid;

				nIndex = 0;
				while (nIndex < nFrames)
				{
					if (!D2CMP_10081_GetTileFrame(pTileLibraryEntries[nIndex]))
					{
						break;
					}

					++nIndex;
				}

				memset(szBuffer, 0x00, sizeof(szBuffer));
				szBuffer[0] = sgptDataTables->szDefaultString;

				if (nIndex >= nFrames)
				{
					FOG_10025(FOG_10018(szBuffer, "Animating tiles missing Rarity 0 Tile - Style %d Seq %d", nStyle, nSeq), __FILE__, __LINE__);
					nIndex = 0;
				}


				pTiles[i].pTile = pTileLibraryEntries[nIndex];
				*pDrlgAnimTileGrid->ppMapTileData = &pTiles[i];

				nX = pTiles[i].nPosX + pRoomEx->nTileXPos;
				nY = pTiles[i].nPosY + pRoomEx->nTileYPos;

				for (unsigned int nRarity = 1; nRarity < nFrames; ++nRarity)
				{
					nIndex = 0;
					while (nIndex < nFrames)
					{
						if (D2CMP_10081_GetTileFrame(pTileLibraryEntries[nIndex]) == nRarity)
						{
							break;
						}

						++nIndex;
					}

					if (nIndex >= nFrames)
					{
						FOG_10025(FOG_10018(szBuffer, "Animating tiles missing Rarity %d Tile - Style %d Seq %d", nRarity, nStyle, nSeq), __FILE__, __LINE__);
						nIndex = 0;
					}

					if (pTiles[i].unk0x1C == 0)
					{
						pDrlgAnimTileGrid->ppMapTileData[nRarity] = DRLGROOMTILE_InitFloorTileData(pRoomEx, NULL, nX, nY, v9, pTileLibraryEntries[nIndex]);
					}
					else if (pTiles[i].unk0x1C == 13)
					{
						pDrlgAnimTileGrid->ppMapTileData[nRarity] = DRLGROOMTILE_InitShadowTileData(pRoomEx, NULL, nX, nY, v9, pTileLibraryEntries[nIndex]);
					}
					else
					{
						pDrlgAnimTileGrid->ppMapTileData[nRarity] = DRLGROOMTILE_InitWallTileData(pRoomEx, NULL, nX, nY, v9, pTileLibraryEntries[nIndex], pTiles[i].unk0x1C);
					}

					pDrlgAnimTileGrid->ppMapTileData[nRarity]->dwFlags |= 8;
				}
			}
		}
	}
}

//D2Common.0x6FD75B00
void __fastcall DRLGANIM_UpdateFrameInAdjacentRooms(D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2)
{
	int nCurrentFrame = 0;

	if (pRoomEx2)
	{
		if (pRoomEx1)
		{
			for (int i = 0; i < pRoomEx1->nRoomsNear; ++i)
			{
				if (pRoomEx1->ppRoomsNear[i]->pTileGrid && pRoomEx1->ppRoomsNear[i]->pTileGrid->pAnimTiles)
				{
					nCurrentFrame = pRoomEx1->ppRoomsNear[i]->pTileGrid->pAnimTiles->nCurrentFrame;
					break;
				}
			}
		}

		for (int i = 0; i < pRoomEx2->nRoomsNear; ++i)
		{
			if (pRoomEx2->ppRoomsNear[i]->pTileGrid)
			{
				for (D2DrlgAnimTileGridStrc* j = pRoomEx2->ppRoomsNear[i]->pTileGrid->pAnimTiles; j; j = j->pNext)
				{
					j->nCurrentFrame = nCurrentFrame;
				}
			}
		}
	}
}
