#include "Drlg/D2DrlgOutSiege.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgDrlgVer.h"
#include "Drlg/D2DrlgOutdoors.h"
#include "Drlg/D2DrlgOutPlace.h"
#include "Drlg/D2DrlgTileSub.h"
#include "D2Seed.h"


//TODO: Find member names
struct D2DrlgOutSiegeInitStrc
{
	int nLevelId;
	int field_4;
	int field_8;
	int nLevelPrestId1;
	int nLevelPrestId2;
};

struct D2DrlgOutSiegeInitStrc2
{
	int nStyle;
	int field_4;
	int field_8;
	int nLevelPrestId1;
	int nLevelPrestId2;
};

struct D2DrlgOutSiegeInitStrc3
{
	int nLevelId;
	int nLevelPrestId1;
	int nLevelPrestId2;
	int field_C;
	int field_10;
	int field_14;
	BOOL bMustHave;
};


//D2Common.0x6FD84100
int __fastcall sub_6FD84100(D2DrlgLevelStrc* pLevel)
{
	return (pLevel->nLevelId == LEVEL_TUNDRAWASTELANDS) + 4;
}

//D2Common.0x6FD84110
void __fastcall DRLGOUTSIEGE_InitAct5OutdoorLevel(D2DrlgLevelStrc* pLevel)
{
	static const D2CoordStrc stru_6FDD09C8[] =
	{
		{ -1, 0 },
		{ 0, -1 },
		{ 1, 0 },
		{ 0, 1 },
		{ 0, -1 },
		{ 1, 0 },
		{ 0, 1 },
		{ -1, 0 },
		{ -1, 0 },
		{ 0, -1 },
		{ 1, 0 },
		{ 0, 1 },
	};

	D2LvlPrestTxt* pLvlPrestTxtRecord = NULL;
	D2DrlgVertexStrc* pPreviousVertex = NULL;
	D2DrlgVertexStrc* pDrlgVertex = NULL;
	int nPreviousDiffXAbs = 0;
	int nPreviousDiffYAbs = 0;
	int nPreviousDiffX = 0;
	int nPreviousDiffY = 0;
	int nLevelPrestId1 = 0;
	int nLevelPrestId2 = 0;
	int nLevelPrestId = 0;
	int nCurrentDiffX = 0;
	int nCurrentDiffY = 0;
	int nPreviousX = 0;
	int nPreviousY = 0;
	int nCurrentX = 0;
	int nCurrentY = 0;
	int nLookupId = 0;
	int nXCapped = 0;
	int nYCapped = 0;
	int nIndex = 0;
	int nSize = 0;
	int nRand = 0;
	int nX = 0;
	int nY = 0;

	if (pLevel->nLevelId == LEVEL_BLOODYFOOTHILLS)
	{
		D2_ASSERT(pLevel->pOutdoors);

		pLvlPrestTxtRecord = DATATBLS_GetLvlPrestTxtRecord(LVLPREST_ACT5_SIEGE_TO_TOWN);
		D2_ASSERT(pLvlPrestTxtRecord);

		nSize = pLevel->pOutdoors->nGridWidth - pLvlPrestTxtRecord->nSizeX / 8;
		for (int i = 0; i < 15; ++i)
		{
			D2_ASSERTM(nSize >= 0, "Siege Level is the wrong size");

			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nSize, 0, i + LVLPREST_ACT5_SIEGE_TO_TOWN, 0, 0);
			nSize -= pLvlPrestTxtRecord->nSizeX / 8;
		}
	}
	else
	{
		DRLGOUTPLACE_SetOutGridLinkFlags(pLevel);

		pPreviousVertex = pLevel->pOutdoors->pVertex;
		pDrlgVertex = pPreviousVertex->pNext;

		nLookupId = sub_6FD84100(pLevel);
		do
		{
			DRLGVER_GetCoordDiff(pPreviousVertex, &nPreviousDiffX, &nPreviousDiffY);
			DRLGVER_GetCoordDiff(pDrlgVertex, &nCurrentDiffX, &nCurrentDiffY);

			if (nPreviousDiffX < 0)
			{
				nPreviousDiffXAbs = -nPreviousDiffX;
			}
			else
			{
				nPreviousDiffXAbs = nPreviousDiffX;
			}

			if (nPreviousDiffY < 0)
			{
				nPreviousDiffYAbs = -nPreviousDiffY;
			}
			else
			{
				nPreviousDiffYAbs = nPreviousDiffY;
			}

			nPreviousX = pPreviousVertex->nPosX & 0xFFFFFFFE;
			nPreviousY = pPreviousVertex->nPosY & 0xFFFFFFFE;

			nCurrentX = pDrlgVertex->nPosX & 0xFFFFFFFE;
			nCurrentY = pDrlgVertex->nPosY & 0xFFFFFFFE;

			nLevelPrestId = sub_6FD80BE0(nPreviousDiffX, nPreviousDiffY, nLookupId);

			if (!(pPreviousVertex->dwFlags & 2))
			{
				while (nPreviousX != nCurrentX || nPreviousY != nCurrentY)
				{
					nPreviousX += 2 * nPreviousDiffX;
					nPreviousY += 2 * nPreviousDiffY;

					DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nPreviousX, nPreviousY, nLevelPrestId, -1, 0);
					DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nPreviousX, nPreviousY, 1, FLAG_OPERATION_OR);
				}
			}

			if (pPreviousVertex->dwFlags & 1)
			{
				if (pPreviousVertex->nPosX <= pDrlgVertex->nPosX)
				{
					nX = pDrlgVertex->nPosX;
				}
				else
				{
					nX = pPreviousVertex->nPosX;
				}
				nXCapped = (nX - 4 * nPreviousDiffXAbs) & 0xFFFFFFFE;

				if (pPreviousVertex->nPosY <= pDrlgVertex->nPosY)
				{
					nY = pDrlgVertex->nPosY;
				}
				else
				{
					nY = pPreviousVertex->nPosY;
				}
				nYCapped = (nY - 4 * nPreviousDiffYAbs) & 0xFFFFFFFE;

				DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nXCapped, nY - 4 * nPreviousDiffYAbs, 1024, FLAG_OPERATION_OR);
				DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nXCapped + 2 * nPreviousDiffXAbs, nYCapped + 2 * nPreviousDiffYAbs, 1024, FLAG_OPERATION_OR);
			}

			nRand = sub_6FD80C10(2 * nPreviousDiffX, 2 * nPreviousDiffY, 2 * nCurrentDiffX, 2 * nCurrentDiffY, nLookupId);
			if (nRand)
			{
				DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nCurrentX, nCurrentY, nRand, -1, 0);
				DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nCurrentX, nCurrentY, 1, FLAG_OPERATION_OR);
			}

			pPreviousVertex = pDrlgVertex;
			pDrlgVertex = pDrlgVertex->pNext;
		}
		while (pPreviousVertex != pLevel->pOutdoors->pVertex);

		if (pLevel->nLevelId == LEVEL_ID_ACT5_BARRICADE_1)
		{
			sub_6FD846C0(pLevel);
		}

		nX = pLevel->pOutdoors->nGridWidth - 2;
		nY = 0;

		nLevelPrestId1 = pLevel->nLevelId == LEVEL_TUNDRAWASTELANDS ? LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_1_SNOW : LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_1;
		nLevelPrestId2 = pLevel->nLevelId == LEVEL_TUNDRAWASTELANDS ? LVLPREST_ACT5_BARRICADE_RAVINE_BORDER_1_SNOW : LVLPREST_ACT5_BARRICADE_RAVINE_BORDER_1;

		while (nX != 0 || nY != pLevel->pOutdoors->nGridHeight - 2)
		{
			nIndex = DRLGGRID_GetGridFlags(pLevel->pOutdoors->pGrid, nX, nY) - nLevelPrestId1;
			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nX, nY, nIndex + nLevelPrestId2, -1, 0);

			nX += 2 * stru_6FDD09C8[nIndex].nX;
			nY += 2 * stru_6FDD09C8[nIndex].nY;
		}

		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, pLevel->pOutdoors->nGridWidth - 2, 0, pLevel->nLevelId == LEVEL_TUNDRAWASTELANDS ? LVLPREST_ACT5_BARRICADE_CLIFF_RAVINE_BORDER_7_SNOW : LVLPREST_ACT5_BARRICADE_CLIFF_RAVINE_BORDER_7, -1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, pLevel->pOutdoors->nGridHeight - 2, pLevel->nLevelId == LEVEL_TUNDRAWASTELANDS ? LVLPREST_ACT5_BARRICADE_RAVINE_CLIFF_BORDER_5_SNOW : LVLPREST_ACT5_BARRICADE_RAVINE_CLIFF_BORDER_5, -1, 0);

		DRLGOUTSIEGE_PlaceBarricadeEntrancesAndExits(pLevel);
		DRLGOUTSIEGE_PlaceCaves(pLevel);

		if (pLevel->nLevelId == LEVEL_ID_ACT5_BARRICADE_1)
		{
			DRLGOUTSIEGE_ConnectBarricadeAndSiege(pLevel);
		}

		DRLGOUTSIEGE_AddACt5SecondaryBorder(pLevel);
		DRLGOUTSIEGE_PlacePrisons(pLevel);
		DRLGOUTSIEGE_PlaceSpecialPresets(pLevel);
	}
}

//D2Common.0x6FD844F0
void __fastcall DRLGOUTSIEGE_PlaceCaves(D2DrlgLevelStrc* pLevel)
{
	static const D2DrlgOutSiegeInitStrc stru_6FDD0988[] =
	{
		{ LEVEL_ARREATPLATEAU,		0, 0, LVLPREST_ACT5_BARRICADE_TO_CAVE_32X16,		LVLPREST_ACT5_BARRICADE_TO_CAVE_16X32 },
		{ LEVEL_TUNDRAWASTELANDS,	0, 1, LVLPREST_ACT5_BARRICADE_FROM_CAVE_32X16_SNOW,	LVLPREST_ACT5_BARRICADE_FROM_CAVE_16X32_SNOW },
		{ LEVEL_TUNDRAWASTELANDS,	0, 0, LVLPREST_ACT5_BARRICADE_TO_CAVE_32X16_SNOW,	LVLPREST_ACT5_BARRICADE_TO_CAVE_16X32_SNOW },
	};

	int nLevelPrestId = 0;
	int nX = 0;
	int nY = 0;

	for (int i = 0; i < ARRAY_SIZE(stru_6FDD0988); ++i)
	{
		if (pLevel->nLevelId == stru_6FDD0988[i].nLevelId)
		{
			if (pLevel->nWidth <= pLevel->nHeight)
			{
				nLevelPrestId = stru_6FDD0988[i].nLevelPrestId1;
				//DATATBLS_GetLvlPrestTxtRecord(nLevelPrestId);
				nX = 2;
				if (stru_6FDD0988[i].field_8)
				{
					nY = pLevel->pOutdoors->nGridHeight - 2;
				}
				else
				{
					nY = 0;
				}
			}
			else
			{
				nLevelPrestId = stru_6FDD0988[i].nLevelPrestId2;
				//DATATBLS_GetLvlPrestTxtRecord(nLevelPrestId);
				nY = 2;
				if (stru_6FDD0988[i].field_8)
				{
					nX = pLevel->pOutdoors->nGridWidth - 2;
				}
				else
				{
					nX = 0;
				}
			}

			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nX, nY, nLevelPrestId, stru_6FDD0988[i].field_4, 0);
		}
	}
}

//D2Common.0x6FD84580
void __fastcall DRLGOUTSIEGE_PlaceBarricadeEntrancesAndExits(D2DrlgLevelStrc* pLevel)
{
	for (int i = 0; i < pLevel->pOutdoors->nGridHeight; ++i)
	{
		if (DRLGGRID_GetGridFlags(&pLevel->pOutdoors->pGrid[2], i, 0) & 0x400)
		{
			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, i, 0, LVLPREST_ACT5_BARRICADE_EXIT_32X16, 2 * (pLevel->nLevelId == LEVEL_BLOODYFOOTHILLS) - 1, 0);
			break;
		}
	}

	for (int i = 0; i < pLevel->pOutdoors->nGridHeight; ++i)
	{
		if (DRLGGRID_GetGridFlags(&pLevel->pOutdoors->pGrid[2], i, pLevel->pOutdoors->nGridHeight - 2) & 0x400)
		{
			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, i, pLevel->pOutdoors->nGridHeight - 2, LVLPREST_ACT5_BARRICADE_ENTRANCE_32X16, 2 * (pLevel->nLevelId == LEVEL_BLOODYFOOTHILLS) - 1, 0);
			break;
		}
	}

	for (int i = 0; i < pLevel->pOutdoors->nGridHeight; ++i)
	{
		if (DRLGGRID_GetGridFlags(&pLevel->pOutdoors->pGrid[2], 0, i) & 0x400)
		{
			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, i, LVLPREST_ACT5_BARRICADE_EXIT_16X32, 2 * (pLevel->nLevelId == LEVEL_BLOODYFOOTHILLS) - 1, 0);
			break;
		}
	}

	for (int i = 0; i < pLevel->pOutdoors->nGridHeight; ++i)
	{
		if (DRLGGRID_GetGridFlags(&pLevel->pOutdoors->pGrid[2], pLevel->pOutdoors->nGridWidth - 2, i) & 0x400)
		{
			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, pLevel->pOutdoors->nGridWidth - 2, i, LVLPREST_ACT5_BARRICADE_ENTRANCE_16X32, 2 * (pLevel->nLevelId == LEVEL_BLOODYFOOTHILLS) - 1, 0);
			break;
		}
	}
}

//D2Common.0x6FD846C0
void __fastcall sub_6FD846C0(D2DrlgLevelStrc* pLevel)
{
	DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], pLevel->pOutdoors->nGridWidth - 2, pLevel->pOutdoors->nGridHeight - 4, 1024, FLAG_OPERATION_OR);
	DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], pLevel->pOutdoors->nGridWidth - 2, pLevel->pOutdoors->nGridHeight - 3, 1024, FLAG_OPERATION_OR);
}

//D2Common.0x6FD84700
//TODO: a1
void __fastcall DRLGOUTSIEGE_AddACt5SecondaryBorder(D2DrlgLevelStrc* pLevel)
{
	D2UnkOutdoorStrc a1 = {};

	if (pLevel->nLevelId != LEVEL_BLOODYFOOTHILLS)
	{
		a1.pLevel = pLevel;
		a1.field_4 = &pLevel->pOutdoors->nWidth;
		a1.pGrid1 = &pLevel->pOutdoors->pGrid[0];
		a1.pGrid2 = &pLevel->pOutdoors->pGrid[2];
		a1.field_14 = -1;
		a1.nLvlSubId = LVLSUB_ACT5_BARRICADE;
		a1.field_24 = sub_6FD84820;
		a1.field_28 = sub_6FD84780;
		a1.field_2C = DRLGOUTDOORS_AlterAdjacentPresetGridCells;
		a1.field_30 = DRLGOUTDOORS_SetBlankGridCell;
		a1.field_34 = DRLGOUTDOORS_SpawnOutdoorLevelPresetEx;

		DRLGTILESUB_AddSecondaryBorder(&a1);
	}
}

//D2Common.0x6FD84780
int __fastcall sub_6FD84780(D2DrlgLevelStrc* pLevel, int nStyle, int a3)
{
	static const D2DrlgOutSiegeInitStrc2 stru_6FDD0A28[] =
	{
		{ 49,	1,	16,	LVLPREST_ACT5_BARRICADE_1,					LVLPREST_ACT5_BARRICADE_1_SNOW },
		{ 49,	31,	46,	LVLPREST_ACT5_BARRICADE_1,					LVLPREST_ACT5_BARRICADE_1_SNOW },
		{ 48,	1,	1,	LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_3,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_3_SNOW },
		{ 48,	2,	3,	LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_1,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_1_SNOW },
		{ 48,	4,	4,	LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_4,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_4_SNOW },
		{ 48,	5,	5,	LVLPREST_ACT5_BARRICADE_RAVINE_BORDER_3,	LVLPREST_ACT5_BARRICADE_RAVINE_BORDER_3_SNOW },
		{ 48,	6,	7,	LVLPREST_ACT5_BARRICADE_RAVINE_BORDER_1,	LVLPREST_ACT5_BARRICADE_RAVINE_BORDER_1_SNOW },
		{ 48,	8,	8,	LVLPREST_ACT5_BARRICADE_RAVINE_BORDER_4,	LVLPREST_ACT5_BARRICADE_RAVINE_BORDER_4_SNOW },
		{ 48,	30,	30,	0,											0 },
		{ 48,	31,	31,	-5,											-5 },
	};

	D2_ASSERT((nStyle == 48) || (nStyle == 49));

	for (int i = 0; i < ARRAY_SIZE(stru_6FDD0A28); ++i)
	{
		if (nStyle == stru_6FDD0A28[i].nStyle && a3 >= stru_6FDD0A28[i].field_4 && a3 <= stru_6FDD0A28[i].field_8)
		{
			if (pLevel->nLevelId == LEVEL_TUNDRAWASTELANDS)
			{
				return a3 + stru_6FDD0A28[i].nLevelPrestId2 - stru_6FDD0A28[i].field_4;
			}
			else
			{
				return a3 + stru_6FDD0A28[i].nLevelPrestId1 - stru_6FDD0A28[i].field_4;
			}
		}
	}

	D2_UNREACHABLE;
}

//D2Common.0x6FD84820
//TODO: v8
BOOL __fastcall sub_6FD84820(D2DrlgLevelStrc* pLevel, int nX, int nY, int a4, int a5, unsigned int a6)
{
	int v8 = sub_6FD84780(pLevel, (a6 >> 20) & 0x3F, BYTE1(a6));

	if (v8 == -5)
	{
		return TRUE;
	}
	else if (v8 == a4)
	{
		return DRLGOUTDOORS_TestGridCellNonLvlLink(pLevel, nX, nY) != 0;
	}

	return FALSE;
}

//D2Common.0x6FD84870
void __fastcall DRLGOUTSIEGE_PlaceSpecialPresets(D2DrlgLevelStrc* pLevel)
{
	static const D2DrlgOutSiegeInitStrc3 stru_6FDD0AF8[] =
	{
		{ LEVEL_ID_ACT5_BARRICADE_1,		LVLPREST_ACT5_BARRICADE_HELL_PORTAL_N,		LVLPREST_ACT5_BARRICADE_HELL_PORTAL_W,		0, 25, 1, 1 },
		{ LEVEL_ARREATPLATEAU,		LVLPREST_ACT5_BARRICADE_HELL_PORTAL_N,		LVLPREST_ACT5_BARRICADE_HELL_PORTAL_W,		0, 25, 1, 1 },
		{ LEVEL_TUNDRAWASTELANDS,	LVLPREST_ACT5_BARRICADE_HELL_PORTAL_N,		LVLPREST_ACT5_BARRICADE_HELL_PORTAL_W,		1, 25, 1, 1 },
		{ LEVEL_ARREATPLATEAU,		LVLPREST_ACT5_BARRICADE_WAYPOINT_DIRT,		LVLPREST_ACT5_BARRICADE_WAYPOINT_DIRT,		-1, 25, 1, 1 },
		{ LEVEL_TUNDRAWASTELANDS,	LVLPREST_ACT5_BARRICADE_WAYPOINT_SNOW,		LVLPREST_ACT5_BARRICADE_WAYPOINT_SNOW,		-1, 25, 1, 1 },
		{ LEVEL_ID_ACT5_BARRICADE_1,		LVLPREST_ACT5_BARRICADE_RUINS_N_TREASURE,	LVLPREST_ACT5_BARRICADE_RUINS_W_TREASURE,	-1, 10, 1, 0 },
		{ LEVEL_ID_ACT5_BARRICADE_1,		LVLPREST_ACT5_BARRICADE_RUINS_N_1,			LVLPREST_ACT5_BARRICADE_RUINS_W_1,			-1, 20, 4, 0 },
		{ LEVEL_ID_ACT5_BARRICADE_1,		LVLPREST_ACT5_BARRICADE_RUINS_N_2,			LVLPREST_ACT5_BARRICADE_RUINS_W_2,			-1, 20, 4, 0 },
		{ LEVEL_ARREATPLATEAU,		LVLPREST_ACT5_BARRICADE_FILLER_TREASURE,	LVLPREST_ACT5_BARRICADE_FILLER_TREASURE,	-1, 10, 1, 0 },
		{ LEVEL_ARREATPLATEAU,		LVLPREST_ACT5_BARRICADE_BUILDING,			LVLPREST_ACT5_BARRICADE_BUILDING,			-1, 8, 1, 0 },
		{ LEVEL_ARREATPLATEAU,		LVLPREST_ACT5_BARRICADE_FILLER,				LVLPREST_ACT5_BARRICADE_FILLER,				-1, 15, 5, 0 },
		{ LEVEL_TUNDRAWASTELANDS,	LVLPREST_ACT5_BARRICADE_SNOW_LAKE__1,		LVLPREST_ACT5_BARRICADE_SNOW_LAKE__1,		-1, 9, 4, 0 },
		{ LEVEL_TUNDRAWASTELANDS,	LVLPREST_ACT5_BARRICADE_SNOW_LAKE__2,		LVLPREST_ACT5_BARRICADE_SNOW_LAKE__2,		-1, 9, 4, 0 },
		{ LEVEL_TUNDRAWASTELANDS,	LVLPREST_ACT5_BARRICADE_SNOW_OTHER,			LVLPREST_ACT5_BARRICADE_SNOW_OTHER,			-1, 9, 4, 0 },
		{ LEVEL_TUNDRAWASTELANDS,	LVLPREST_ACT5_BARRICADE_SNOW_TREASURE,		LVLPREST_ACT5_BARRICADE_SNOW_TREASURE,		-1, 5, 3, 0 },
	};

	int nLevelPrestId = 0;
	BOOL bAdded = FALSE;

	for (int i = 0; i < ARRAY_SIZE(stru_6FDD0AF8); ++i)
	{
		if (pLevel->nLevelId == stru_6FDD0AF8[i].nLevelId)
		{
			if (pLevel->nWidth < pLevel->nHeight)
			{
				nLevelPrestId = stru_6FDD0AF8[i].nLevelPrestId1;
			}
			else
			{
				nLevelPrestId = stru_6FDD0AF8[i].nLevelPrestId2;
			}

			bAdded = FALSE;

			for (int j = 0; j < stru_6FDD0AF8[i].field_14; ++j)
			{
				bAdded = DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, nLevelPrestId, stru_6FDD0AF8[i].field_C, 0, 15);
			}

			D2_ASSERT(bAdded || !stru_6FDD0AF8[i].bMustHave);
		}
	}
}

//D2Common.0x6FD84910
void __fastcall DRLGOUTSIEGE_PlacePrisons(D2DrlgLevelStrc* pLevel)
{
	int nPrisonsPlaced = 0;
	int nLevelPrestId = 0;
	int nRandX = 0;
	int nRandY = 0;
	int nX = 0;
	int nY = 0;

	if (pLevel->nLevelId == LEVEL_ID_ACT5_BARRICADE_1)
	{
		D2_ASSERT(pLevel->pOutdoors);

		for (int i = 0; i < 90; ++i)
		{
			if (nPrisonsPlaced >= 3)
			{
				break;
			}

			nX = 2 * SEED_RollLimitedRandomNumber(&pLevel->pSeed, pLevel->pOutdoors->nGridWidth / 2);
			nY = 2 * SEED_RollLimitedRandomNumber(&pLevel->pSeed, pLevel->pOutdoors->nGridHeight / 2);

			nLevelPrestId = DRLGOUTDOORS_GetPresetIndexFromGridCell(pLevel, nX, nY);
			if (nLevelPrestId >= LVLPREST_ACT5_BARRICADE_1 && nLevelPrestId <= LVLPREST_ACT5_BARRICADE_8)
			{
				DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nX, nY, nLevelPrestId + 16, -1, 0);
				++nPrisonsPlaced;
			}
		}

		nRandX = SEED_RollLimitedRandomNumber(&pLevel->pSeed, pLevel->pOutdoors->nGridWidth / 2);
		nRandY = SEED_RollLimitedRandomNumber(&pLevel->pSeed, pLevel->pOutdoors->nGridHeight / 2);

		for (int j = 0; j < pLevel->pOutdoors->nGridHeight; ++j)
		{
			if (nPrisonsPlaced >= 3)
			{
				break;
			}

			for (int i = 0; i < pLevel->pOutdoors->nGridWidth; ++i)
			{
				if (nPrisonsPlaced >= 3)
				{
					break;
				}

				nX = (i + 2 * nRandX) % pLevel->pOutdoors->nGridWidth;
				nY = (j + 2 * nRandY) % pLevel->pOutdoors->nGridHeight;

				nLevelPrestId = DRLGOUTDOORS_GetPresetIndexFromGridCell(pLevel, nX, nY);
				if (nLevelPrestId >= LVLPREST_ACT5_BARRICADE_1 && nLevelPrestId <= LVLPREST_ACT5_BARRICADE_8)
				{
					DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nX, nY, nLevelPrestId + 16, -1, 0);
					++nPrisonsPlaced;
				}
			}
		}

		if (nPrisonsPlaced < 3)
		{
			FOG_10025("Could not place enough prisons for quest 2. Please include seed in bug report.", __FILE__, __LINE__);
		}
	}
}

//D2Common.0x6FD84BB0
void __fastcall DRLGOUTSIEGE_ConnectBarricadeAndSiege(D2DrlgLevelStrc* pLevel)
{
	D2LvlPrestTxt* pLvlPrestTxtRecord = NULL;
	int nX = 0;
	int nY = 0;

	D2_ASSERT(pLevel);

	D2_ASSERT(pLevel->nLevelId == LEVEL_ID_ACT5_BARRICADE_1);

	D2_ASSERT(pLevel->pOutdoors);

	pLvlPrestTxtRecord = DATATBLS_GetLvlPrestTxtRecord(LVLPREST_ACT5_BARRICADE_TO_SIEGE);
	D2_ASSERT(pLvlPrestTxtRecord);

	nX = pLevel->pOutdoors->nGridWidth - pLvlPrestTxtRecord->nSizeX / 8;
	nY = pLevel->pOutdoors->nGridHeight - pLvlPrestTxtRecord->nSizeY / 8;

	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nX, nY, LVLPREST_ACT5_BARRICADE_TO_SIEGE, -1, 0);
	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nX, nY - 2, LVLPREST_ACT5_BARRICADE_RAVINE_BORDER_4, -1, 0);
}

