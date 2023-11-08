#include "Drlg/D2DrlgOutRoom.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgDrlgLogic.h"
#include "Drlg/D2DrlgDrlgVer.h"
#include "Drlg/D2DrlgRoomTile.h"
#include "D2Seed.h"


//TODO: Check calls with DrlgGridStrc args

//D2Common.0x6FD83D20
void __fastcall DRLGOUTROOM_FreeDrlgOutdoorRoom(D2DrlgRoomStrc* pDrlgRoom)
{
	if (pDrlgRoom->pOutdoor)
	{
		DRLGOUTROOM_FreeDrlgOutdoorRoomData(pDrlgRoom);

		D2_FREE_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, pDrlgRoom->pOutdoor);
		pDrlgRoom->pOutdoor = NULL;
	}
}

//D2Common.0x6FD83D90
void __fastcall DRLGOUTROOM_FreeDrlgOutdoorRoomData(D2DrlgRoomStrc* pDrlgRoom)
{
	if (pDrlgRoom->pOutdoor)
	{
		DRLGGRID_FreeGrid(pDrlgRoom->pLevel->pDrlg->pMempool, &pDrlgRoom->pOutdoor->pTileTypeGrid);
		DRLGGRID_FreeGrid(pDrlgRoom->pLevel->pDrlg->pMempool, &pDrlgRoom->pOutdoor->pWallGrid);
		DRLGGRID_FreeGrid(pDrlgRoom->pLevel->pDrlg->pMempool, &pDrlgRoom->pOutdoor->pFloorGrid);
		DRLGGRID_FreeGrid(pDrlgRoom->pLevel->pDrlg->pMempool, &pDrlgRoom->pOutdoor->pDirtPathGrid);
		DRLGVER_FreeVertices(pDrlgRoom->pLevel->pDrlg->pMempool, &pDrlgRoom->pOutdoor->pVertex);
	}
}

//D2Common.0x6FD83DE0
void __fastcall DRLGOUTROOM_AllocDrlgOutdoorRoom(D2DrlgRoomStrc* pDrlgRoom)
{
	pDrlgRoom->pOutdoor = D2_CALLOC_STRC_POOL(pDrlgRoom->pLevel->pDrlg->pMempool, D2DrlgOutdoorRoomStrc);
}

//D2Common.6FD83E20
void __fastcall DRLGOUTROOM_InitializeDrlgOutdoorRoom(D2DrlgRoomStrc* pDrlgRoom)
{
	DRLGROOMTILE_AllocTileGrid(pDrlgRoom);
	DRLGROOMTILE_CountWallWarpTiles(pDrlgRoom, &pDrlgRoom->pOutdoor->pWallGrid, &pDrlgRoom->pOutdoor->pTileTypeGrid, 0, 0);
	DRLGROOMTILE_CountAllTileTypes(pDrlgRoom, &pDrlgRoom->pOutdoor->pWallGrid, 0, 0, 0);
	DRLGROOMTILE_CountAllTileTypes(pDrlgRoom, &pDrlgRoom->pOutdoor->pFloorGrid, 0, 0, 0);
	DRLGROOMTILE_AllocTileData(pDrlgRoom);
	DRLGROOMTILE_LoadInitRoomTiles(pDrlgRoom, &pDrlgRoom->pOutdoor->pWallGrid, &pDrlgRoom->pOutdoor->pTileTypeGrid, 0, FALSE, FALSE);
	DRLGROOMTILE_LoadInitRoomTiles(pDrlgRoom, &pDrlgRoom->pOutdoor->pFloorGrid, nullptr, FALSE, FALSE, FALSE);

	pDrlgRoom->pTileGrid->pTiles.nWalls = pDrlgRoom->pTileGrid->nWalls;
	pDrlgRoom->pTileGrid->pTiles.nFloors = pDrlgRoom->pTileGrid->nFloors;
	pDrlgRoom->pTileGrid->pTiles.nRoofs = pDrlgRoom->pTileGrid->nShadows;

	DRLGLOGIC_AllocCoordLists(pDrlgRoom);
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
		const bool bRand0IsNull = pLevelLinkData->nRand[0][pLevelLinkData->nIteration] == 0;
		if (int(bRand0IsNull) == pLevelLinkData->nRand[1][pLevelLinkData->nIteration])
		{
			return 0;
		}

		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = bRand0IsNull;
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
