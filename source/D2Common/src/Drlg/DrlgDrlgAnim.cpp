#include "Drlg/D2DrlgDrlgAnim.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgRoomTile.h"
#include <D2CMP.h>



//TODO: Variable names


//D2Common.0x6FD75480
void __fastcall DRLGANIM_InitCache(D2DrlgStrc* pDrlg, D2DrlgTileDataStrc* pTileData)
{
	D2TileLibraryEntryStrc* ppTileLibraryEntry[40] = {};
	int nSequence = 0;
	int nStyle = 0;

	D2_ASSERT(pDrlg);
	D2_ASSERT(pTileData);

	memset(pTileData, 0x00, sizeof(D2DrlgTileDataStrc));

	if (pDrlg->nAct != ACT_I)
	{
		if (pDrlg->nAct == ACT_II)
		{
			nSequence = 1;
		}
		else if (pDrlg->nAct == ACT_III)
		{
			nStyle = 29;
			nSequence = 12;
		}
		else
		{
			return;
		}
	}

	const int nSize = D2CMP_10088_GetTiles(pDrlg->pTiles, 0, nStyle, nSequence, ppTileLibraryEntry, ARRAY_SIZE(ppTileLibraryEntry));
	D2_ASSERT(nSize);
	DRLGROOMTILE_InitTileData(NULL, pTileData, 0, 0, 0, ppTileLibraryEntry[0]);
}

//D2Common.0x6FD75560
void __fastcall DRLGANIM_TestLoadAnimatedRoomTiles(D2RoomExStrc* pRoomEx, D2DrlgGridStrc* pDrlgGrid, D2DrlgGridStrc* pTileTypeGrid, int nTileType, int nTileX, int nTileY)
{
	D2TileLibraryEntryStrc* ppTileLibraryEntry[40] = {};

	for (int nY = 0; nY < pRoomEx->nTileHeight + (nTileY == 0); ++nY)
	{
		for (int nX = 0; nX < pRoomEx->nTileWidth + (nTileX == 0); ++nX)
		{
			const uint32_t nGridEntry = DRLGGRID_GetGridEntry(pDrlgGrid, nX, nY);
			const D2C_PackedTileInformation nTileInfo{ nGridEntry };
			if (nTileInfo.bShadow || nTileInfo.bIsWall || nTileInfo.bIsFloor)
			{
				if (pTileTypeGrid)
				{
					nTileType = DRLGGRID_GetGridEntry(pTileTypeGrid, nX, nY);
				}

				const uint8_t nStyle = nTileInfo.nTileStyle;
				const uint8_t nSequence = nTileInfo.nTileSequence;

				const int nTilesCount = D2CMP_10088_GetTiles(pRoomEx->pTiles, nTileType, nStyle, nSequence, ppTileLibraryEntry, ARRAY_SIZE(ppTileLibraryEntry));
				if (nTilesCount && D2CMP_10079_GetTileFlags(ppTileLibraryEntry[0]) & TILE_FLAGS_LAVA)
				{
					if (nTileInfo.bIsFloor)
					{
						pRoomEx->pTileGrid->pTiles.nFloors += (nTilesCount - 1);
					}
					else if (nTileInfo.bIsWall)
					{
						pRoomEx->pTileGrid->pTiles.nWalls += (nTilesCount - 1);
					}
					else
					{
						pRoomEx->pTileGrid->pTiles.nRoofs += (nTilesCount - 1);
					}

					pRoomEx->dwFlags |= ROOMEXFLAG_ANIMATED_FLOOR;
				}
			}
		}
	}
}

// Helper functions
static int AnimationFrameFixedPointToInteger(int nFixedPointFrame)
{
	return nFixedPointFrame >> 8;
}

static int AnimationFrameIntegerToFixedPoint(int nIntegerPart)
{
	return nIntegerPart << 8;
}

//D2Common.0x6FD756B0
void __fastcall DRLGANIM_AnimateTiles(D2RoomExStrc* pRoomEx)
{

	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		D2RoomExStrc* pCurrentRoomEx = pRoomEx->ppRoomsNear[i];

		if (pCurrentRoomEx->dwFlags & ROOMEXFLAG_ANIMATED_FLOOR && pCurrentRoomEx->pTileGrid)
		{
			for (D2DrlgAnimTileGridStrc* j = pCurrentRoomEx->pTileGrid->pAnimTiles; j; j = j->pNext)
			{
				D2DrlgTileDataStrc* pTileData = j->ppMapTileData[AnimationFrameFixedPointToInteger(j->nCurrentFrame)];
				if (pTileData)
				{
					pTileData->dwFlags |= MAPTILE_HIDDEN;
				}

				j->nCurrentFrame = (j->nCurrentFrame + j->nAnimationSpeed) % AnimationFrameIntegerToFixedPoint(j->nFrames);

				pTileData = j->ppMapTileData[AnimationFrameFixedPointToInteger(j->nCurrentFrame)];
				if (pTileData)
				{
					pTileData->dwFlags &= ~MAPTILE_HIDDEN;
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

// Helper function
D2TileLibraryEntryStrc* DRLGANIM_FindAnimatedTileFrame(D2TileLibraryEntryStrc** pTileLibraryEntries, int nbTileEntries, int nRarity, int nStyle, int nSeq)
{
	for (int nEntryIndex = 0; nEntryIndex < nbTileEntries; nEntryIndex++)
	{
		if (D2CMP_10081_GetTileRarity(pTileLibraryEntries[nEntryIndex]) == nRarity)
		{
			return pTileLibraryEntries[nEntryIndex];
		}
	}
	char szBuffer[100] = { sgptDataTables->szDefaultString };
	FOG_DisplayWarning(FOG_csprintf(szBuffer, "Animating tiles missing Rarity %d Tile - Style %d Seq %d", nRarity, nStyle, nSeq), __FILE__, __LINE__);
	// Return first tile if nothing found.
	return pTileLibraryEntries[0];
}

//D2Common.0x6FD757B0
void __fastcall DRLGANIM_AllocAnimationTileGrid(D2RoomExStrc* pRoomEx, int nAnimationSpeed, D2DrlgTileDataStrc* pTiles, int nTiles, D2DrlgGridStrc* pDrlgGrid, int nUnused)
{
	if (!pTiles)
	{
		return;
	}
	if (nAnimationSpeed == 0)
	{
		nAnimationSpeed = 80;
	}

	D2TileLibraryEntryStrc* pTileLibraryEntries[40] = {};

	for (int i = 0; i < nTiles; ++i)
	{
		D2DrlgTileDataStrc& pCurrentTileData = pTiles[i];
		if (pCurrentTileData.pTile && D2CMP_10079_GetTileFlags(pCurrentTileData.pTile) & TILE_FLAGS_LAVA)
		{

			const int32_t nGridIdx = GetMapTileLayer(pCurrentTileData.dwFlags);
			const uint32_t nGridEntry = DRLGGRID_GetGridEntry(&pDrlgGrid[nGridIdx], pCurrentTileData.nPosX, pCurrentTileData.nPosY);
			const D2C_PackedTileInformation nPackedTileInformation{ nGridEntry };

			const uint8_t nStyle = nPackedTileInformation.nTileStyle;
			const uint8_t nSeq = nPackedTileInformation.nTileSequence;

			int nFrames = D2CMP_10088_GetTiles(pRoomEx->pTiles, pCurrentTileData.nTileType, nStyle, nSeq, pTileLibraryEntries, ARRAY_SIZE(pTileLibraryEntries));

			D2DrlgAnimTileGridStrc* pDrlgAnimTileGrid = D2_CALLOC_STRC_POOL(pRoomEx->pLevel->pDrlg->pMempool, D2DrlgAnimTileGridStrc);

			pDrlgAnimTileGrid->ppMapTileData = (D2DrlgTileDataStrc**)D2_CALLOC_POOL(pRoomEx->pLevel->pDrlg->pMempool, sizeof(D2DrlgTileDataStrc*) * nFrames);

			pDrlgAnimTileGrid->nFrames = nFrames;
			pDrlgAnimTileGrid->nAnimationSpeed = nAnimationSpeed;
			pDrlgAnimTileGrid->pNext = pRoomEx->pTileGrid->pAnimTiles;

			pRoomEx->pTileGrid->pAnimTiles = pDrlgAnimTileGrid;

			// Animated tiles use rarity as frame index
			pCurrentTileData.pTile = DRLGANIM_FindAnimatedTileFrame(pTileLibraryEntries, nFrames, 0, nStyle, nSeq);
			pDrlgAnimTileGrid->ppMapTileData[0] = &pCurrentTileData;

			int nX = pCurrentTileData.nPosX + pRoomEx->nTileXPos;
			int nY = pCurrentTileData.nPosY + pRoomEx->nTileYPos;

			for (unsigned int nRarity = 1; nRarity < nFrames; ++nRarity)
			{
				D2TileLibraryEntryStrc* pTileEntry = DRLGANIM_FindAnimatedTileFrame(pTileLibraryEntries, nFrames, 0, nStyle, nSeq);

				if (pCurrentTileData.nTileType == TILETYPE_FLOORS)
				{
					pDrlgAnimTileGrid->ppMapTileData[nRarity] = DRLGROOMTILE_InitFloorTileData(pRoomEx, nullptr, nX, nY, nPackedTileInformation.nPackedValue, pTileEntry);
				}
				else if (pCurrentTileData.nTileType == TILETYPE_SHADOWS)
				{
					pDrlgAnimTileGrid->ppMapTileData[nRarity] = DRLGROOMTILE_InitShadowTileData(pRoomEx, nullptr, nX, nY, nPackedTileInformation.nPackedValue, pTileEntry);
				}
				else
				{
					pDrlgAnimTileGrid->ppMapTileData[nRarity] = DRLGROOMTILE_InitWallTileData(pRoomEx, nullptr, nX, nY, nPackedTileInformation.nPackedValue, pTileEntry, pCurrentTileData.nTileType);
				}
				// We start by displaying the first frame, hide all other frames tiles.
				pDrlgAnimTileGrid->ppMapTileData[nRarity]->dwFlags |= MAPTILE_HIDDEN;
			}
		}
	}
}

//D2Common.0x6FD75B00
void __fastcall DRLGANIM_UpdateFrameInAdjacentRooms(D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2)
{
	if (!pRoomEx2)
	{
		return;
	}
	int nCurrentFrame = 0;
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
		if (D2DrlgTileGridStrc* pAdjacentRoomTileGrid = pRoomEx2->ppRoomsNear[i]->pTileGrid)
		{
			for (D2DrlgAnimTileGridStrc* pAdjacentRoomAnimData = pAdjacentRoomTileGrid->pAnimTiles; pAdjacentRoomAnimData; pAdjacentRoomAnimData = pAdjacentRoomAnimData->pNext)
			{
				pAdjacentRoomAnimData->nCurrentFrame = nCurrentFrame;
			}
		}
	}

}
