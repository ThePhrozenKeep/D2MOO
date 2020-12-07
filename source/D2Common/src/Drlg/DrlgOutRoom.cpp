#include "Drlg/D2DrlgOutRoom.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgDrlgLogic.h"
#include "Drlg/D2DrlgDrlgVer.h"
#include "Drlg/D2DrlgRoomTile.h"
#include "D2Seed.h"


//TODO: Check calls with DrlgGridStrc args

//D2Common.0x6FD83D20
void __fastcall DRLGOUTROOM_FreeDrlgOutdoorRoom(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->pOutdoor)
	{
		DRLGOUTROOM_FreeDrlgOutdoorRoomData(pRoomEx);

		FOG_FreeServerMemory(pRoomEx->pLevel->pDrlg->pMempool, pRoomEx->pOutdoor, __FILE__, __LINE__, 0);
		pRoomEx->pOutdoor = NULL;
	}
}

//D2Common.0x6FD83D90
void __fastcall DRLGOUTROOM_FreeDrlgOutdoorRoomData(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->pOutdoor)
	{
		DRLGGRID_FreeGrid(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pOutdoor->pOrientationGrid);
		DRLGGRID_FreeGrid(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pOutdoor->pWallGrid);
		DRLGGRID_FreeGrid(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pOutdoor->pFloorGrid);
		DRLGGRID_FreeGrid(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pOutdoor->pCellGrid);
		DRLGVER_FreeVertices(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pOutdoor->pVertex);
	}
}

//D2Common.0x6FD83DE0
void __fastcall DRLGOUTROOM_AllocDrlgOutdoorRoom(D2RoomExStrc* pRoomEx)
{
	pRoomEx->pOutdoor = (D2DrlgOutdoorRoomStrc*)FOG_AllocServerMemory(pRoomEx->pLevel->pDrlg->pMempool, sizeof(D2DrlgOutdoorRoomStrc), __FILE__, __LINE__, 0);
	memset(pRoomEx->pOutdoor, 0x00, sizeof(D2DrlgOutdoorRoomStrc));
}

//D2Common.6FD83E20
void __fastcall DRLGOUTROOM_InitializeDrlgOutdoorRoom(D2RoomExStrc* pRoomEx)
{
	DRLGROOMTILE_AllocTileGrid(pRoomEx);
	DRLGROOMTILE_CountWallWarpTiles(pRoomEx, &pRoomEx->pOutdoor->pWallGrid, &pRoomEx->pOutdoor->pOrientationGrid, 0, 0);
	DRLGROOMTILE_CountAllTileTypes(pRoomEx, &pRoomEx->pOutdoor->pWallGrid, 0, 0, 0);
	DRLGROOMTILE_CountAllTileTypes(pRoomEx, &pRoomEx->pOutdoor->pFloorGrid, 0, 0, 0);
	DRLGROOMTILE_AllocTileData(pRoomEx);
	DRLGROOMTILE_LoadInitRoomTiles(pRoomEx, &pRoomEx->pOutdoor->pWallGrid, &pRoomEx->pOutdoor->pOrientationGrid, 0, FALSE, FALSE);
	DRLGROOMTILE_LoadInitRoomTiles(pRoomEx, &pRoomEx->pOutdoor->pFloorGrid, NULL, 0, FALSE, FALSE);

	pRoomEx->pTileGrid->pTiles.nWalls = pRoomEx->pTileGrid->nWalls;
	pRoomEx->pTileGrid->pTiles.nFloors = pRoomEx->pTileGrid->nFloors;
	pRoomEx->pTileGrid->pTiles.nRoofs = pRoomEx->pTileGrid->nShadows;

	DRLGLOGIC_AllocCoordLists(pRoomEx);
}

//D2Common.0x6FD83EC0
BOOL __fastcall DRLGOUTROOM_LinkLevelsByLevelCoords(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	int nRand = SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 1;

	pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = nRand;
	pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = nRand;

	if (nRand == 0)
	{
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nWidth = 64;
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nHeight = 160;
	}
	else if (nRand == 1)
	{
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nWidth = 160;
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nHeight = 64;
	}

	pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nPosX = pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink].nPosX - pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nWidth;
	pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nPosY = pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink].nHeight - pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nHeight + pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink].nPosY - 16;
	
	return TRUE;
}

//D2Common.0x6FD83F70
BOOL __fastcall DRLGOUTROOM_LinkLevelsByLevelDef(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	D2LevelDefBin* pLevelDefBin = NULL;
	int nRand = SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 1;

	pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = nRand;
	pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = nRand;

	if (nRand == 0)
	{
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nWidth = 64;
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nHeight = 160;
	}
	else if (nRand == 1)
	{
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nWidth = 160;
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nHeight = 64;
	}

	pLevelDefBin = DATATBLS_GetLevelDefRecord(pLevelLinkData->nCurrentLevel);

	pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nPosX = pLevelDefBin->dwOffsetX;
	pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nPosY = pLevelDefBin->dwOffsetY;

	return TRUE;
}

//D2Common.0x6FD84010
BOOL __fastcall DRLGOUTROOM_LinkLevelsByOffsetCoords(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	static const D2CoordStrc pOffsetCoords[4] =
	{
		{ 0, -160 },
		{ -96, -64 },
		{ -64, -96 },
		{ -160, 0 }
	};

	int nIndex = 0;

	if (pLevelLinkData->nRand[1][pLevelLinkData->nIteration] == -1)
	{
		pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 1;
		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = pLevelLinkData->nRand[1][pLevelLinkData->nIteration];
	}
	else
	{
		if (!pLevelLinkData->nRand[0][pLevelLinkData->nIteration] == pLevelLinkData->nRand[1][pLevelLinkData->nIteration])
		{
			return 0;
		}

		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = !pLevelLinkData->nRand[0][pLevelLinkData->nIteration];
	}

	if (pLevelLinkData->nRand[0][pLevelLinkData->nIteration] == 0)
	{
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nWidth = 64;
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nHeight = 160;
	}
	else if (pLevelLinkData->nRand[0][pLevelLinkData->nIteration] == 1)
	{
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nWidth = 160;
		pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nHeight = 64;
	}

	nIndex = pLevelLinkData->nRand[0][pLevelLinkData->nIteration] + 2 * pLevelLinkData->nRand[0][pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink];

	pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nPosX = pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink].nPosX + pOffsetCoords[nIndex].nX;
	pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nPosY = pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink].nPosY + pOffsetCoords[nIndex].nY;
	
	return TRUE;
}
