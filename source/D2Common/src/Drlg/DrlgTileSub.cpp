#include "Drlg/D2DrlgTileSub.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlgAnim.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "Drlg/D2DrlgMaze.h"
#include "Drlg/D2DrlgPreset.h"
#include "Drlg/D2DrlgRoomTile.h"
#include "Drlg/D2DrlgOutdoors.h"
#include "Drlg/D2DrlgOutRoom.h"
#include "D2Dungeon.h"
#include "D2Seed.h"
#include <DataTbls/LevelsIds.h>


//D2Common.0x6FD8A460
void __fastcall DRLGTILESUB_AddSecondaryBorder(D2UnkOutdoorStrc* a1)
{
	D2LvlSubTxt* pLvlSubTxtRecord = NULL;
	D2DrlgSubstGroupStrc* pSubstGroup = NULL;
	BOOL bSmallWilderness = FALSE;
	BOOL bWilderness = FALSE;
	BOOL bBreak = FALSE;
	unsigned int nRand1 = 0;
	unsigned int nRand2 = 0;
	unsigned int nRand = 0;
	int nOffset = 0;
	int nHeight = 0;
	int nWidth = 0;
	int nTemp1 = 0;
	int nTemp2 = 0;
	int nArea = 0;
	int nX = 0;
	int nY = 0;
	D2CoordStrc pCoord[256] = {};

	pLvlSubTxtRecord = DATATBLS_GetLvlSubTxtRecord(a1->nLvlSubId);

	while (pLvlSubTxtRecord->dwType == a1->nLvlSubId)
	{
		DRLGTILESUB_InitializeDrlgFile(a1->pLevel->pDrlg->hArchive, pLvlSubTxtRecord);

		if (pLvlSubTxtRecord->pDrlgFile->nSubstGroups > 0)
		{
			if (a1->field_14 == -1)
			{
				a1->field_14 = 62;
			}

			if (!pLvlSubTxtRecord->dwBordType)
			{
				nRand = SEED_RollLimitedRandomNumber(&a1->pLevel->pSeed, pLvlSubTxtRecord->pDrlgFile->nSubstGroups);
			}
			else
			{
				nRand = 0;
			}

			for (int j = 0; j < pLvlSubTxtRecord->pDrlgFile->nSubstGroups; ++j)
			{
				pSubstGroup = &((D2DrlgSubstGroupStrc*)(pLvlSubTxtRecord->pDrlgFile->pSubstGroups))[(nRand + j) % pLvlSubTxtRecord->pDrlgFile->nSubstGroups];

				if (a1->pLevel->nLevelId >= LEVEL_BLOODMOOR && a1->pLevel->nLevelId <= LEVEL_TAMOEHIGHLAND)
				{
					bWilderness = TRUE;
				}
				else
				{
					bWilderness = FALSE;
				}

				nOffset = a1->nLvlSubId == 1 && a1->pLevel->pOutdoors->dwFlags & 12 ? -1 : 1;

				nWidth = nOffset + a1->field_4[2] - pLvlSubTxtRecord->dwGridSize * pSubstGroup->tBox.nWidth;
				nHeight = 1 + a1->field_4[3] - pLvlSubTxtRecord->dwGridSize * pSubstGroup->tBox.nHeight;

				nArea = nWidth * nHeight;
				if (nArea > 0)
				{
					if (a1->nLvlSubId == 1 && bWilderness && nWidth < 6 && nHeight < 6)
					{
						bSmallWilderness = TRUE;
					}
					else
					{
						bSmallWilderness = FALSE;
					}

					for (int i = 0; i < nArea; ++i)
					{
						pCoord[i].nX = i % nWidth;
						pCoord[i].nY = i / nWidth;
					}

					for (int i = 0; i < nArea; ++i)
					{
						nRand1 = SEED_RollLimitedRandomNumber(&a1->pLevel->pSeed, nArea);
						nRand2 = SEED_RollLimitedRandomNumber(&a1->pLevel->pSeed, nArea);

						nTemp1 = pCoord[nRand1].nX;
						nTemp2 = pCoord[nRand1].nY;

						pCoord[nRand1].nX = pCoord[nRand2].nX;
						pCoord[nRand1].nY = pCoord[nRand2].nY;
						pCoord[nRand2].nX = nTemp1;
						pCoord[nRand2].nY = nTemp2;
					}

					bBreak = FALSE;
					for (int i = 0; i < nArea; ++i)
					{
						nX = pCoord[i].nX;
						nY = pCoord[i].nY;
						if (!bSmallWilderness || nX != 2 || nY != 2)
						{
							if (DRLGTILESUB_TestReplaceSubPreset(nX, nY, a1, pSubstGroup, pLvlSubTxtRecord))
							{
								DRLGTILESUB_ReplaceSubPreset(nX, nY, a1, pSubstGroup, pLvlSubTxtRecord, (SEED_RollLimitedRandomNumber(&a1->pLevel->pSeed, pSubstGroup->field_14) + 1) * (pSubstGroup->tBox.nWidth + 1));

								if (pLvlSubTxtRecord->dwBordType == 0)
								{
									bBreak = TRUE;
									break;
								}
								else if (pLvlSubTxtRecord->dwBordType == 1)
								{
									break;
								}
							}
						}
					}

					if (bBreak)
					{
						break;
					}
				}
			}
		}

		++pLvlSubTxtRecord;
	}
}

//D2Common.0x6FD8A750
//TODO: variable names
BOOL __fastcall DRLGTILESUB_TestReplaceSubPreset(int a1, int a2, D2UnkOutdoorStrc* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord)
{
	int nFloorFlags = 0;
	int nWallFlags = 0;
	int nX = 0;
	int nY = 0;
	int v8 = 0;
	int v9 = 0;
	int v15 = 0;
	int v18 = 0;

	v9 = a1 - a1 % pLvlSubTxtRecord->dwGridSize;
	v8 = a2 - a2 % pLvlSubTxtRecord->dwGridSize;

	for (int j = 0; j < pSubstGroup->tBox.nHeight; ++j)
	{
		for (int i = 0; i < pSubstGroup->tBox.nWidth; ++i)
		{
			if (pLvlSubTxtRecord->pDrlgFile->nFloorLayers)
			{
				nFloorFlags = DRLGGRID_GetGridEntry(&pLvlSubTxtRecord->pFloorGrid, i + pSubstGroup->tBox.nPosX, j + pSubstGroup->tBox.nPosY);
			}
			else
			{
				nFloorFlags = 0;
			}

			if (pLvlSubTxtRecord->pDrlgFile->nWallLayers)
			{
				nWallFlags = DRLGGRID_GetGridEntry(pLvlSubTxtRecord->pWallGrid, i + pSubstGroup->tBox.nPosX, j + pSubstGroup->tBox.nPosY);
			}
			else
			{
				nWallFlags = 0;
			}

			nX = v9 + i * pLvlSubTxtRecord->dwGridSize;
			nY = v8 + j * pLvlSubTxtRecord->dwGridSize;

			v15 = DRLGGRID_GetGridEntry(a3->pGrid1, nX, nY);
			if (a3->field_24)
			{
				if (!a3->field_24(a3->pLevel, nX, nY, v15, nFloorFlags, nWallFlags))
				{
					return 0;
				}
			}
			else if (nWallFlags & 1)
			{
				v18 = BYTE1(nWallFlags) - 1;
				if (v18 != a3->field_14 && v18 + a3->nLevelPrestId != v15)
				{
					return 0;
				}

				if (!a3->field_1C(a3->pLevel, nX, nY))
				{
					return 0;
				}
			}
			else if (nFloorFlags & 2)
			{
				if (!a3->field_20(a3->pLevel, nX, nY, 0, 0, 0))
				{
					return 0;
				}
			}
		}
	}

	return 1;
}

//D2Common.0x6FD8A8E0
//TODO: variable names
void __fastcall DRLGTILESUB_ReplaceSubPreset(int a1, int a2, D2UnkOutdoorStrc* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord, int a6)
{
	int nFloorFlags = 0;
	int nWallFlags = 0;
	int v17 = 0;
	int v18 = 0;
	int v21 = 0;
	int v25 = 0;

	v21 = a1 - a1 % pLvlSubTxtRecord->dwGridSize;
	v25 = a2 - a2 % pLvlSubTxtRecord->dwGridSize;

	for (int j = 0; j < pSubstGroup->tBox.nHeight; ++j)
	{
		for (int i = 0; i < pSubstGroup->tBox.nWidth; ++i)
		{
			if (pLvlSubTxtRecord->pDrlgFile->nWallLayers)
			{
				nWallFlags = DRLGGRID_GetGridEntry(pLvlSubTxtRecord->pWallGrid, a6 + pSubstGroup->tBox.nPosX + i, pSubstGroup->tBox.nPosY + j);
			}
			else
			{
				nWallFlags = 0;
			}

			if (pLvlSubTxtRecord->pDrlgFile->nFloorLayers)
			{
				nFloorFlags = DRLGGRID_GetGridEntry(&pLvlSubTxtRecord->pFloorGrid, a6 + pSubstGroup->tBox.nPosX + i, pSubstGroup->tBox.nPosY + j);
			}
			else
			{
				nFloorFlags = 0;
			}

			if (nWallFlags & 1)
			{
				v17 = BYTE1(nWallFlags) - 1;
				if (a3->field_28)
				{
					v18 = a3->field_28(a3->pLevel, ((unsigned int)nWallFlags >> 20) & 0x3F, BYTE1(nWallFlags));
				}
				else
				{
					v18 = v17 + a3->nLevelPrestId;
				}

				if (v18 != -5 && v17 != a3->field_14)
				{
					a3->field_34(a3->pLevel, v21 + i * pLvlSubTxtRecord->dwGridSize, v25 + j * pLvlSubTxtRecord->dwGridSize, v18, 0, 1);
				}
			}
			else
			{
				if (nFloorFlags & 2)
				{
					a3->field_2C(a3->pLevel, v21 + i * pLvlSubTxtRecord->dwGridSize, v25 + j * pLvlSubTxtRecord->dwGridSize);
				}
				else
				{
					a3->field_30(a3->pLevel, v21 + i * pLvlSubTxtRecord->dwGridSize, v25 + j * pLvlSubTxtRecord->dwGridSize);
				}
			}
		}
	}
}

//D2Common.0x6FD8AA80
void __fastcall sub_6FD8AA80(D2UnkOutdoorStrc2* a1)
{
	D2DrlgSubstGroupStrc* pSubstGroup = NULL;
	D2LvlSubTxt* pLvlSubTxtRecord = NULL;
	unsigned int nThemeFlag = 0;
	int nHeight = 0;
	int nWidth = 0;
	int nRand = 0;

	if (a1->nSubWaypoint_Shrine != -1)
	{
		pLvlSubTxtRecord = DATATBLS_GetLvlSubTxtRecord(a1->nSubWaypoint_Shrine);
		nThemeFlag = a1->nSubThemePicked;
		while (nThemeFlag)
		{
			if (nThemeFlag & 1)
			{
				DRLGTILESUB_InitializeDrlgFile(a1->pDrlgRoom->pLevel->pDrlg->hArchive, pLvlSubTxtRecord);

				if (pLvlSubTxtRecord->dwCheckAll)
				{
					for (int nCurSubstGroupIndex = 0; nCurSubstGroupIndex < pLvlSubTxtRecord->pDrlgFile->nSubstGroups; ++nCurSubstGroupIndex)
					{
						pSubstGroup = &((D2DrlgSubstGroupStrc*)pLvlSubTxtRecord->pDrlgFile->pSubstGroups)[nCurSubstGroupIndex];
						nWidth = a1->pDrlgRoom->nTileWidth - pSubstGroup->tBox.nWidth + 1;
						nHeight = a1->pDrlgRoom->nTileHeight - pSubstGroup->tBox.nHeight + 1;
						if (nWidth > 0 && nHeight > 0)
						{
							if (pLvlSubTxtRecord->pDrlgFile->nSubstMethod == DRLGSUBST_FIXED)
							{
								for (int j = 1; j < nHeight; ++j)
								{
									for (int i = 1; i < nWidth; ++i)
									{
										if (sub_6FD8B010(i, j, a1, pSubstGroup, pLvlSubTxtRecord))
										{
											sub_6FD8ACE0(a1->pDrlgRoom->pLevel->pDrlg->pMempool, i, j, a1, pSubstGroup, pLvlSubTxtRecord, 0);
										}
									}
								}
							}
							else if (pLvlSubTxtRecord->pDrlgFile->nSubstMethod == DRLGSUBST_RANDOM)
							{
								for (int j = 0; j < nHeight; ++j)
								{
									for (int i = 0; i < nWidth; ++i)
									{
										if (sub_6FD8B130(i, j, a1, pSubstGroup, pLvlSubTxtRecord)
											&& pLvlSubTxtRecord->nProb[a1->nSubTheme] < (unsigned int)SEED_RollRandomNumber(&a1->pDrlgRoom->pSeed) % 100)
										{
											nRand = SEED_RollLimitedRandomNumber(&a1->pDrlgRoom->pSeed, pSubstGroup->field_14);
											sub_6FD8ACE0(a1->pDrlgRoom->pLevel->pDrlg->pMempool, i, j, a1, pSubstGroup, pLvlSubTxtRecord, (nRand + 1) * (pSubstGroup->tBox.nWidth + 1));
										}
									}
								}
							}
						}
					}
				}
				else
				{
					DRLGTILESUB_DoSubstitutions(a1, pLvlSubTxtRecord);
				}
			}

			nThemeFlag >>= 1;
			++pLvlSubTxtRecord;
		}
	}
}

//D2Common.0x6FD8ACE0
void __fastcall sub_6FD8ACE0(void* pMemPool, int nX, int nY, D2UnkOutdoorStrc2* a4, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord, int a7)
{
	int nCounter = 0;
	int nFlags = 0;
	int nMinX = 0;
	int nMinY = 0;
	int nMaxX = 0;
	int nMaxY = 0;

	for (int j = 0; j < pSubstGroup->tBox.nHeight; ++j)
	{
		for (int i = 0; i < pSubstGroup->tBox.nWidth; ++i)
		{
			if (DRLGGRID_GetGridEntry(&pLvlSubTxtRecord->pShadowGrid, i + pSubstGroup->tBox.nPosX + a7, j + pSubstGroup->tBox.nPosY) & 0x8000000)
			{
				++nCounter;
			}
		}
	}

	DRLGROOMTILE_ReallocRoofTileGrid(pMemPool, a4->pDrlgRoom->pTileGrid, nCounter);

	for (int j = 0; j < pSubstGroup->tBox.nHeight; ++j)
	{
		for (int i = 0; i < pSubstGroup->tBox.nWidth; ++i)
		{
			if (pLvlSubTxtRecord->pDrlgFile->nFloorLayers)
			{
				nFlags = DRLGGRID_GetGridEntry(&pLvlSubTxtRecord->pFloorGrid, i + pSubstGroup->tBox.nPosX + a7, j + pSubstGroup->tBox.nPosY);
			}
			else
			{
				nFlags = 0;
			}

			if (nFlags & 2)
			{
				nFlags |= 0x80;
				DRLGGRID_AlterGridFlag(a4->pFloorGrid, nX + i, nY + j, nFlags, FLAG_OPERATION_OVERWRITE);
			}

			D2_ASSERT(pLvlSubTxtRecord->pDrlgFile->nWallLayers <= DRLG_MAX_WALL_LAYERS);
			for (int nLayer = 0; nLayer < pLvlSubTxtRecord->pDrlgFile->nWallLayers; ++nLayer)
			{
				nFlags = DRLGGRID_GetGridEntry(&pLvlSubTxtRecord->pWallGrid[nLayer], i + pSubstGroup->tBox.nPosX + a7, j + pSubstGroup->tBox.nPosY);
				if (nFlags & 1 && a4->pWallsGrids[nLayer])
				{
					DRLGGRID_AlterGridFlag(a4->pWallsGrids[nLayer], nX + i, nY + j, nFlags, FLAG_OPERATION_OVERWRITE);
				}

				nFlags = DRLGGRID_GetGridEntry(&pLvlSubTxtRecord->pTileTypeGrid[nLayer], i + pSubstGroup->tBox.nPosX + a7, j + pSubstGroup->tBox.nPosY);
				if (nFlags && a4->pOutdoorRooms[nLayer])
				{
					DRLGGRID_AlterGridFlag(&a4->pOutdoorRooms[nLayer]->pTileTypeGrid, nX + i, nY + j, nFlags, FLAG_OPERATION_OVERWRITE);
				}
			}

			nFlags = DRLGGRID_GetGridEntry(&pLvlSubTxtRecord->pShadowGrid, i + pSubstGroup->tBox.nPosX + a7, j + pSubstGroup->tBox.nPosY);
			if (nFlags & 0x8000000)
			{
				DRLGROOMTILE_InitTileShadow(a4->pDrlgRoom, nX + i + a4->pDrlgRoom->nTileXPos, nY + j + a4->pDrlgRoom->nTileYPos, nFlags);
			}
		}
	}

	nMinX = pSubstGroup->tBox.nPosX;
	nMinY = pSubstGroup->tBox.nPosY;
	nMaxX = pSubstGroup->tBox.nWidth;
	nMaxY = pSubstGroup->tBox.nHeight;

	DUNGEON_GameTileToSubtileCoords(&nX, &nY);
	DUNGEON_GameTileToSubtileCoords(&nMinX, &nMinY);
	DUNGEON_GameTileToSubtileCoords(&nMaxX, &nMaxY);
	
	for (D2PresetUnitStrc* pPresetUnit = pLvlSubTxtRecord->pDrlgFile->pPresetUnit; pPresetUnit; pPresetUnit = pPresetUnit->pNext)
	{
		if (pPresetUnit->nXpos > nMinX && pPresetUnit->nXpos < nMinX + nMaxX && pPresetUnit->nYpos > nMinY && pPresetUnit->nYpos < nMinY + nMaxY)
		{
			DRLGROOM_AllocPresetUnit(a4->pDrlgRoom, pMemPool, pPresetUnit->nUnitType, pPresetUnit->nIndex, pPresetUnit->nMode, nX + pPresetUnit->nXpos - nMinX, nY + pPresetUnit->nYpos - nMinY);
		}
	}
}

//D2Common.0x6FD8B010
BOOL __fastcall sub_6FD8B010(int a1, int a2, D2UnkOutdoorStrc2* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord)
{
	int nFlags = 0;

	for (int j = 0; j < pSubstGroup->tBox.nHeight; ++j)
	{
		for (int i = 0; i < pSubstGroup->tBox.nWidth; ++i)
		{
			if (DRLGGRID_GetGridEntry(&pLvlSubTxtRecord->pFloorGrid, i + pSubstGroup->tBox.nPosX, j + pSubstGroup->tBox.nPosY) & 2 || pLvlSubTxtRecord->pWallGrid[0].nWidth && DRLGGRID_GetGridEntry(pLvlSubTxtRecord->pWallGrid, i + pSubstGroup->tBox.nPosX, j + pSubstGroup->tBox.nPosY) & 1)
			{
				nFlags = DRLGGRID_GetGridEntry(a3->pFloorGrid, a1 + i, a2 + j);
				if (nFlags & 0x3F0FF00 || !(nFlags & 2))
				{
					return FALSE;
				}

				for (int nLayer = 0; nLayer < a3->field_2C; ++nLayer)
				{
					if (a3->pWallsGrids[nLayer] && DRLGGRID_GetGridEntry(a3->pWallsGrids[nLayer], a1 + i, a2 + j) & 1)
					{
						return FALSE;
					}
				}
			}
		}
	}

	return TRUE;
}

//D2Common.0x6FD8B130
BOOL __fastcall sub_6FD8B130(int a1, int a2, D2UnkOutdoorStrc2* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord)
{
	int nTileType = 0;
	int nFloorFlags1 = 0;
	int nFloorFlags2 = 0;
	int nWallFlags1 = 0;
	int nWallFlags2 = 0;

	for (int j = 0; j < pSubstGroup->tBox.nHeight; ++j)
	{
		for (int i = 0; i < pSubstGroup->tBox.nWidth; ++i)
		{
			nTileType = DRLGGRID_GetGridEntry(pLvlSubTxtRecord->pTileTypeGrid, i + pSubstGroup->tBox.nPosX, j + pSubstGroup->tBox.nPosY);
			if (nTileType != DRLGGRID_GetGridEntry(&a3->pOutdoorRooms[0]->pTileTypeGrid, i + a1, j + a2))
			{
				return FALSE;
			}

			if (pLvlSubTxtRecord->pDrlgFile->nFloorLayers)
			{
				nFloorFlags1 = DRLGGRID_GetGridEntry(&pLvlSubTxtRecord->pFloorGrid, i + pSubstGroup->tBox.nPosX, j + pSubstGroup->tBox.nPosY);
			}
			else
			{
				nFloorFlags1 = 0;
			}

			if (nFloorFlags1 & 2)
			{
				nFloorFlags2 = DRLGGRID_GetGridEntry(a3->pFloorGrid, i + a1, j + a2);
				if (!(nFloorFlags2 & 2))
				{
					return FALSE;
				}

				if ((nFloorFlags1 ^ nFloorFlags2) & 0x3F0FF00)
				{
					return FALSE;
				}
			}

			if (pLvlSubTxtRecord->pDrlgFile->nWallLayers)
			{
				nWallFlags1 = DRLGGRID_GetGridEntry(pLvlSubTxtRecord->pWallGrid, i + pSubstGroup->tBox.nPosX, j + pSubstGroup->tBox.nPosY);
			}
			else
			{
				nWallFlags1 = 0;
			}

			if (nWallFlags1 & 1)
			{
				nWallFlags2 = DRLGGRID_GetGridEntry(a3->pWallsGrids[0], i + a1, j + a2);
				if (!(nWallFlags2 & 1))
				{
					return FALSE;
				}

				if ((nWallFlags1 ^ nWallFlags2) & 0x3F0FF00)
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

//D2Common.0x6FD8B290
void __fastcall DRLGTILESUB_DoSubstitutions(D2UnkOutdoorStrc2* pOutdoorLevel, D2LvlSubTxt* pLvlSubTxtRecord)
{
	if (!pLvlSubTxtRecord->pDrlgFile->nSubstGroups)
	{
		return;
	}
	for (int xyz = 0; xyz < pLvlSubTxtRecord->nMax[pOutdoorLevel->nSubTheme]; ++xyz)
	{
		const int substGroupIdx = SEED_RollLimitedRandomNumber(&pOutdoorLevel->pDrlgRoom->pSeed, pLvlSubTxtRecord->pDrlgFile->nSubstGroups);
		D2DrlgSubstGroupStrc* pSubstGroup = &pLvlSubTxtRecord->pDrlgFile->pSubstGroups[substGroupIdx];
		
		const int nAvailableSpaceX = pOutdoorLevel->pDrlgRoom->nTileWidth - pSubstGroup->tBox.nWidth;
		const int nAvailableSpaceY = pOutdoorLevel->pDrlgRoom->nTileHeight - pSubstGroup->tBox.nHeight;
		if (nAvailableSpaceX > 0 && nAvailableSpaceY > 0)
		{
			const int nTrials = pLvlSubTxtRecord->nTrials[pOutdoorLevel->nSubTheme];
			if (nTrials != -1)
			{
				for (int i = 0; i < nTrials; ++i)
				{
					int nX = SEED_RollLimitedRandomNumber(&pOutdoorLevel->pDrlgRoom->pSeed, nAvailableSpaceX) + 1;
					int nY = SEED_RollLimitedRandomNumber(&pOutdoorLevel->pDrlgRoom->pSeed, nAvailableSpaceY) + 1;
					if (sub_6FD8B010(nX, nY, pOutdoorLevel, pSubstGroup, pLvlSubTxtRecord))
					{
						sub_6FD8ACE0(pOutdoorLevel->pDrlgRoom->pLevel->pDrlg->pMempool, nX, nY, pOutdoorLevel, pSubstGroup, pLvlSubTxtRecord, 0);
						break;
					}
				}
			}
			else
			{
				D2CoordStrc tCoords[256] = {};

				const int nPotentialPositionsArea = nAvailableSpaceX * nAvailableSpaceY;
				for (int i = 0; i < nPotentialPositionsArea; ++i)
				{
					tCoords[i].nX = i % nAvailableSpaceX;
					tCoords[i].nY = i / nAvailableSpaceX;
				}

				// Randomize positions
				for (int i = 0; i < nPotentialPositionsArea; ++i)
				{
					uint32_t nRand1 = SEED_RollLimitedRandomNumber(&pOutdoorLevel->pDrlgRoom->pSeed, nPotentialPositionsArea);
					uint32_t nRand2 = SEED_RollLimitedRandomNumber(&pOutdoorLevel->pDrlgRoom->pSeed, nPotentialPositionsArea);

					int nTemp1 = tCoords[nRand1].nX;
					int nTemp2 = tCoords[nRand1].nY;
					tCoords[nRand1].nX = tCoords[nRand2].nX;
					tCoords[nRand1].nY = tCoords[nRand2].nY;
					tCoords[nRand2].nX = nTemp1;
					tCoords[nRand2].nY = nTemp2;
				}

				for (int i = 0; i < nPotentialPositionsArea; ++i)
				{
					const int nX = tCoords[i].nX + 1;
					const int nY = tCoords[i].nY + 1;
					if (sub_6FD8B010(nX, nY, pOutdoorLevel, pSubstGroup, pLvlSubTxtRecord))
					{
						sub_6FD8ACE0(pOutdoorLevel->pDrlgRoom->pLevel->pDrlg->pMempool, nX, nY, pOutdoorLevel, pSubstGroup, pLvlSubTxtRecord, 0);
						break;
					}
				}
			}
		}
	}
	
}

//D2Common.0x6FD8B640
void __fastcall DRLGTILESUB_InitializeDrlgFile(HD2ARCHIVE hArchive, D2LvlSubTxt* pLvlSubTxtRecord)
{
	D2DrlgFileStrc** ppDrlgFile = NULL;
	D2DrlgCoordStrc pDrlgCoord = {};

	ppDrlgFile = &pLvlSubTxtRecord->pDrlgFile;

	if (!pLvlSubTxtRecord->pDrlgFile)
	{
		DRLGPRESET_LoadDrlgFile(ppDrlgFile, hArchive, pLvlSubTxtRecord->szFile);

		pDrlgCoord.nPosX = 0;
		pDrlgCoord.nPosY = 0;
		pDrlgCoord.nWidth = (*ppDrlgFile)->nWidth + 1;
		pDrlgCoord.nHeight = (*ppDrlgFile)->nHeight + 1;

		if (!(*ppDrlgFile)->nSubstGroups)
		{
			FOG_DisplayWarning("Substitution File was not saved with substitution groups", __FILE__, __LINE__);
		}

		for (int i = 0; i < (*ppDrlgFile)->nWallLayers; ++i)
		{
			DRLGGRID_FillNewCellFlags(NULL, &pLvlSubTxtRecord->pWallGrid[i], (int*)(*ppDrlgFile)->pWallLayer[i], &pDrlgCoord, pDrlgCoord.nWidth);
			DRLGGRID_FillNewCellFlags(NULL, &pLvlSubTxtRecord->pTileTypeGrid[i], (int*)(*ppDrlgFile)->pTileTypeLayer[i], &pDrlgCoord, pDrlgCoord.nWidth);
		}

		for (int i = 1; i < (*ppDrlgFile)->nWallLayers; ++i)
		{
			DRLGGRID_AlterAllGridFlags(&pLvlSubTxtRecord->pWallGrid[i], i << 18, FLAG_OPERATION_OR);
		}

		if ((*ppDrlgFile)->nFloorLayers)
		{
			DRLGGRID_FillNewCellFlags(NULL, &pLvlSubTxtRecord->pFloorGrid, (int*)(*ppDrlgFile)->pFloorLayer[0], &pDrlgCoord, pDrlgCoord.nWidth);
		}

		if ((*ppDrlgFile)->pShadowLayer)
		{
			DRLGGRID_FillNewCellFlags(NULL, &pLvlSubTxtRecord->pShadowGrid, (int*)(*ppDrlgFile)->pShadowLayer, &pDrlgCoord, pDrlgCoord.nWidth);
		}
	}
}

//D2Common.0x6FD8B770
void __fastcall DRLGTILESUB_FreeDrlgFile(D2LvlSubTxt* pLvlSubTxtRecord)
{
	if (pLvlSubTxtRecord->pDrlgFile)
	{
		for (int i = 0; i < pLvlSubTxtRecord->pDrlgFile->nWallLayers; ++i)
		{
			DRLGGRID_FreeGrid(NULL, &pLvlSubTxtRecord->pWallGrid[i]);
			DRLGGRID_FreeGrid(NULL, &pLvlSubTxtRecord->pTileTypeGrid[i]);
		}

		DRLGGRID_FreeGrid(NULL, &pLvlSubTxtRecord->pFloorGrid);
		DRLGGRID_FreeGrid(NULL, &pLvlSubTxtRecord->pShadowGrid);

		DRLGPRESET_FreeDrlgFile(&pLvlSubTxtRecord->pDrlgFile);
	}
}

//D2Common.0x6FD8B7E0
int __fastcall DRLGTILESUB_PickSubThemes(D2DrlgRoomStrc* pDrlgRoom, int nSubType, int nSubTheme)
{
	D2LvlSubTxt* pLvlSubTxtRecord = NULL;
	int nCounter = 0;
	int nMask = 0;

	if (nSubType != -1 && nSubTheme != -1)
	{
		pLvlSubTxtRecord = DATATBLS_GetLvlSubTxtRecord(nSubType);
		while (pLvlSubTxtRecord->dwType == nSubType)
		{
			if (((unsigned int)SEED_RollRandomNumber(&pDrlgRoom->pSeed) % 100) < pLvlSubTxtRecord->nProb[nSubTheme])
			{
				nMask |= 1 << nCounter;
				pDrlgRoom->dwDT1Mask |= pLvlSubTxtRecord->dwDt1Mask;
			}

			++pLvlSubTxtRecord;
			++nCounter;
		}

		return nMask;
	}

	return 0;
}
