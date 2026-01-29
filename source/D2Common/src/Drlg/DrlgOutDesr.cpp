#include "Drlg/D2DrlgOutDesr.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgMaze.h"
#include "Drlg/D2DrlgOutdoors.h"
#include "Drlg/D2DrlgOutPlace.h"
#include "D2Seed.h"
#include <DataTbls/LevelsIds.h>


struct D2DrlgOutDesertInitStrc				// size: 0x10
{
	int nLvlPrestId;						//0x00
	int nRand;								//0x04
	int nX;									//0x08
	int nY;									//0x0C
};


//Inlined in D2Common.0x6FD7D430
void __fastcall DRLGOUTDESR_PlaceDesertTransitionToTown(D2DrlgLevelStrc* pLevel)
{
	D2DrlgOrthStrc* pTownRoomData = pLevel->pOutdoors->pRoomData;

	while (pTownRoomData && pTownRoomData->pLevel->nLevelId != LEVEL_LUTGHOLEIN)
	{
		pTownRoomData = pTownRoomData->pNext;
	}

	if (pTownRoomData)
	{
		if (pTownRoomData->nDirection == 3)
		{
			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, pLevel->pOutdoors->nGridHeight - 1, LVLPREST_ACT2_DESERT_TRANSITION_N, -1, 0);
		}
		else
		{
			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, pLevel->pOutdoors->nGridWidth - 1, 0, LVLPREST_ACT2_DESERT_TRANSITION_W, -1, 0);
		}
	}
}

//1.10: D2Common.0x6FD7D430
//1.14d: 0x0067F980
void __fastcall DRLGOUTDESR_InitAct2OutdoorLevel(D2DrlgLevelStrc* pLevel)
{
	const int nLevelPrestIds1[] = { LVLPREST_ACT2_DESERT_OASIS_1, LVLPREST_ACT2_DESERT_RUINS_08X08, LVLPREST_ACT2_DESERT_FILL_BONE_1, LVLPREST_ACT2_DESERT_FILL_BONE_2, LVLPREST_ACT2_DESERT_FILL_HEAD_1, LVLPREST_ACT2_DESERT_FILL_MESA_1, LVLPREST_ACT2_DESERT_FILL_WAGON_1 };
	const int nLevelPrestIds2[] = { LVLPREST_ACT2_DESERT_OASIS_1, LVLPREST_ACT2_DESERT_RUINS_08X08, LVLPREST_ACT2_DESERT_FILL_HEAD_2, LVLPREST_ACT2_DESERT_FILL_MESA_1 };
	const int nLevelPrestIds3[] = { LVLPREST_ACT2_DESERT_FILL_BERMS_1, LVLPREST_ACT2_DESERT_FILL_BERMS_2, LVLPREST_ACT2_DESERT_FILL_BERMS_3, LVLPREST_ACT2_DESERT_FILL_BERMS_4 };
	const int nLevelPrestIds4[] = { LVLPREST_ACT2_DESERT_OASIS_2, LVLPREST_ACT2_DESERT_OASIS_3 };

	DRLGOUTPLACE_SetOutGridLinkFlags(pLevel);
	DRLGOUTPLACE_PlaceAct1245OutdoorBorders(pLevel);

	switch (pLevel->nLevelId)
	{
	case LEVEL_ROCKYWASTE:
		DRLGOUTDESR_PlaceDesertTransitionToTown(pLevel);
		DRLGOUTDESR_PlaceBorders(pLevel);
		DRLGOUTDESR_AddExits(pLevel);
		DRLGOUTDOORS_SpawnAct12Shrines(pLevel, 5);
		DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds1, ARRAY_SIZE(nLevelPrestIds1), FALSE);
		break;

	case LEVEL_DRYHILLS:
		DRLGOUTDESR_PlaceCliffs(pLevel);
		DRLGOUTDESR_PlaceBorders(pLevel);
		DRLGOUTDESR_AddExits(pLevel);
		DRLGOUTDOORS_SpawnAct12Waypoint(pLevel);
		DRLGOUTDOORS_SpawnAct12Shrines(pLevel, 5);
		DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds2, ARRAY_SIZE(nLevelPrestIds2), FALSE);
		DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds3, ARRAY_SIZE(nLevelPrestIds3), TRUE);
		break;

	case LEVEL_FAROASIS:
		DRLGOUTDESR_PlaceCliffs(pLevel);
		DRLGOUTDESR_PlaceBorders(pLevel);
		DRLGOUTDESR_AddExits(pLevel);
		DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds4, ARRAY_SIZE(nLevelPrestIds4), FALSE);
		DRLGOUTDOORS_SpawnAct12Waypoint(pLevel);
		DRLGOUTDOORS_SpawnAct12Shrines(pLevel, 5);
		DRLGOUTDESR_PlaceFillsInFarOasis(pLevel);
		break;

	case LEVEL_LOSTCITY:
		DRLGOUTDESR_PlaceCliffs(pLevel);
		DRLGOUTDESR_PlaceBorders(pLevel);
		DRLGOUTDESR_AddExits(pLevel);
		DRLGOUTDESR_PlaceRuinsInLostCity(pLevel);
		DRLGOUTDOORS_SpawnAct12Waypoint(pLevel);
		DRLGOUTDOORS_SpawnAct12Shrines(pLevel, 5);
		DRLGOUTDESR_PlaceFillsInLostCity(pLevel);
		break;

	case LEVEL_VALLEYOFSNAKES:
		DRLGOUTDESR_AddExits(pLevel);
		break;

	case LEVEL_CANYONOFTHEMAGI:
		DRLGOUTDESR_PlaceTombEntriesInCanyon(pLevel);
		DRLGOUTDESR_PlaceBorders(pLevel);
		DRLGOUTDOORS_SpawnAct12Shrines(pLevel, 5);
		DRLGOUTDESR_PlaceFillsInCanyon(pLevel);
		break;

#ifdef D2_VERSION_HAS_UBERS
	case LEVEL_PANDEMONIUMRUN2:
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 4, 4, LVLPREST_ACT2_DESERT_VALLEY_WARP, -1, 0);
		DRLGOUTDESR_PlaceBorders(pLevel);
		DRLGOUTDESR_PlaceFillsInCanyon(pLevel);
		DRLGOUTDOORS_SpawnAct12Shrines(pLevel, 5);
		break;
#endif

	default:
		return;
	}
}

//D2Common.0x6FD7D870
void __fastcall DRLGOUTDESR_PlacePresetVariants(D2DrlgLevelStrc* pLevel, const int* pLevelPrestIds, unsigned int nVariants, BOOL bIterateFiles)
{
	unsigned int nRand = SEED_RollLimitedRandomNumber(&pLevel->pSeed, nVariants);

	for (unsigned int i = 0; i < nVariants; ++i)
	{
		if (bIterateFiles)
		{
			for (unsigned int nFile = 0; nFile < DATATBLS_GetLvlPrestTxtRecord(pLevelPrestIds[nRand])->dwFiles; ++nFile)
			{
				DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, pLevelPrestIds[nRand], nFile, 0, 15);
			}
		}
		else
		{
			DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, pLevelPrestIds[nRand], -1, 0, 15);
		}

		nRand = (nRand + 1) % nVariants;
	}
}

//D2Common.6FD7D950
void __fastcall DRLGOUTDESR_PlaceCliffs(D2DrlgLevelStrc* pLevel)
{
	static const D2DrlgOutDesertInitStrc pOutDesertInit[8][5] =
	{
		{
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_ENDS, 1, 0, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_PATH, -1, 2, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_WALL, -1, 4, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_WALL, -1, 6, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_ENDS, 2, 8, 4 },
		},
		{
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_ENDS, 1, 0, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_WALL, -1, 2, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_PATH, -1, 4, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_WALL, -1, 6, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_ENDS, 2, 8, 4 },
		},
		{
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_ENDS, 1, 0, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_WALL, -1, 2, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_WALL, -1, 4, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_PATH, -1, 6, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_ENDS, 2, 8, 4 },
		},
		{
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_ENDS, 2, 8, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_WALL, -1, 6, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_TOP, -1, 4, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_PATH, -1, 4, 6 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_ENDS, 2, 4, 8 },
		},
		{
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_ENDS, 2, 8, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_PATH, -1, 6, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_TOP, -1, 4, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_WALL, -1, 4, 6 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_ENDS, 2, 4, 8 },
		},
		{
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_ENDS, 1, 4, 0 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_PATH, -1, 4, 2 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_WALL, -1, 4, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_WALL, -1, 4, 6 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_ENDS, 2, 4, 8 },
		},
		{
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_ENDS, 1, 4, 0 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_WALL, -1, 4, 2 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_PATH, -1, 4, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_WALL, -1, 4, 6 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_ENDS, 2, 4, 8 },
		},
		{
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_ENDS, 1, 4, 0 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_WALL, -1, 4, 2 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_WALL, -1, 4, 4 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_PATH, -1, 4, 6 },
			{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_ENDS, 2, 4, 8 },
		},
	};

	unsigned int nRand = SEED_RollRandomNumber(&pLevel->pSeed) & 7;

	for (int i = 0; i < 5; ++i)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, pOutDesertInit[nRand][i].nX, pOutDesertInit[nRand][i].nY, pOutDesertInit[nRand][i].nLvlPrestId, pOutDesertInit[nRand][i].nRand, 0);
	}
}

//D2Common.0x6FD7D9B0
void __fastcall DRLGOUTDESR_PlaceBorders(D2DrlgLevelStrc* pLevel)
{
	DRLGOUTDOORS_AddAct124SecondaryBorder(pLevel, 2, LVLPREST_ACT2_DESERT_BORDER_1);
	DRLGOUTDOORS_AddAct124SecondaryBorder(pLevel, 1, LVLPREST_ACT2_DESERT_BORDER_1);
	DRLGOUTDOORS_AddAct124SecondaryBorder(pLevel, 3, LVLPREST_ACT2_DESERT_BORDER_1);
}

//D2Common.0x6FD7D9F0
void __fastcall DRLGOUTDESR_AddExits(D2DrlgLevelStrc* pLevel)
{
	int nLevelPrestId = 0;

	switch (pLevel->nLevelId)
	{
	case LEVEL_ROCKYWASTE:
	case LEVEL_DRYHILLS:
		nLevelPrestId = LVLPREST_ACT2_DESERT_TOMB_1;
		break;

	case LEVEL_FAROASIS:
		nLevelPrestId = LVLPREST_ACT2_DESERT_LAIR_1;
		break;

	case LEVEL_LOSTCITY:
		nLevelPrestId = LVLPREST_ACT2_DESERT_RUINS_SEWER;
		break;

	case LEVEL_VALLEYOFSNAKES:
		nLevelPrestId = LVLPREST_ACT2_DESERT_TOMB_2;
		break;

	default:
		FOG_DisplayAssert("sAddTheExitsAndStuff() - Could not add the exit!", __FILE__, __LINE__);
		exit(-1);
	}

	if (!DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, nLevelPrestId, -1, 0, 15))
	{
		FOG_DisplayAssert("sAddTheExitsAndStuff() - Could not add the exit!", __FILE__, __LINE__);
		exit(-1);
	}
}

//D2Common.0x6FD7DA60
void __fastcall DRLGOUTDESR_PlaceFillsInFarOasis(D2DrlgLevelStrc* pLevel)
{
	const int nLevelPrestIds1[4] = { LVLPREST_ACT2_DESERT_RUINS_08X08, LVLPREST_ACT2_DESERT_FILL_HEAD_1, LVLPREST_ACT2_DESERT_FILL_MESA_1, LVLPREST_ACT2_DESERT_FILL_WAGON_1 };
	const int nLevelPrestIds2[1] = { LVLPREST_ACT2_DESERT_OASIS_1 };

	DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds1, ARRAY_SIZE(nLevelPrestIds1), FALSE);
	DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds2, ARRAY_SIZE(nLevelPrestIds2), TRUE);
	DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds2, ARRAY_SIZE(nLevelPrestIds2), TRUE);
}

//D2Common.0x6FD7DAC0
void __fastcall DRLGOUTDESR_PlaceRuinsInLostCity(D2DrlgLevelStrc* pLevel)
{
	const int nLevelPrestIds[4] = { LVLPREST_ACT2_DESERT_RUINS_ELDER, LVLPREST_ACT2_DESERT_RUINS_16X16, LVLPREST_ACT2_DESERT_RUINS_16X08, LVLPREST_ACT2_DESERT_RUINS_08X16 };

	DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds, ARRAY_SIZE(nLevelPrestIds), FALSE);
}

//D2Common.0x6FD7DB00
void __fastcall DRLGOUTDESR_PlaceFillsInLostCity(D2DrlgLevelStrc* pLevel)
{
	const int nLevelPrestIds1[5] = { LVLPREST_ACT2_DESERT_OASIS_1, LVLPREST_ACT2_DESERT_FILL_HEAD_2, LVLPREST_ACT2_DESERT_FILL_MESA_1, LVLPREST_ACT2_DESERT_FILL_BERMS_1, LVLPREST_ACT2_DESERT_FILL_BERMS_2 };
	const int nLevelPrestIds2[1] = { LVLPREST_ACT2_DESERT_RUINS_08X08 };

	DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds1, ARRAY_SIZE(nLevelPrestIds1), FALSE);
	DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds2, ARRAY_SIZE(nLevelPrestIds2), TRUE);
	DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds2, ARRAY_SIZE(nLevelPrestIds2), TRUE);
}

//D2Common.6FD7DB70
void __fastcall DRLGOUTDESR_PlaceTombEntriesInCanyon(D2DrlgLevelStrc* pLevel)
{
	static const D2DrlgOutDesertInitStrc pOutDesertInit[9] =
	{
		{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_KING_ENDS,	0, 8, 0 },
		{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_KING_TOMB,	2, 6, 0 },
		{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_KING_TOMB,	1, 4, 0 },
		{ LVLPREST_ACT2_DESERT_CLIFF_RIGHT_KING_TOMB,	0, 2, 0 },
		{ LVLPREST_ACT2_DESERT_CLIFF_TOP_KING_TOMB,		0, 0, 0 },
		{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_KING_TOMB,	0, 0, 2 },
		{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_KING_TOMB,	1, 0, 4 },
		{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_KING_TOMB,	2, 0, 6 },
		{ LVLPREST_ACT2_DESERT_CLIFF_LEFT_KING_ENDS,	0, 0, 8 },
	};

	for (int i = 0; i < ARRAY_SIZE(pOutDesertInit); ++i)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, pOutDesertInit[i].nX, pOutDesertInit[i].nY, pOutDesertInit[i].nLvlPrestId, pOutDesertInit[i].nRand, 0);
	}

	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 4, 4, LVLPREST_ACT2_DESERT_VALLEY_WARP, -1, 0);
}

//D2Common.0x6FD7DBC0
void __fastcall DRLGOUTDESR_PlaceFillsInCanyon(D2DrlgLevelStrc* pLevel)
{
	const int nLevelPrestIds1[5] = { LVLPREST_ACT2_DESERT_FILL_BONE_1, LVLPREST_ACT2_DESERT_FILL_BONE_2, LVLPREST_ACT2_DESERT_FILL_BERMS_3, LVLPREST_ACT2_DESERT_FILL_BERMS_4, LVLPREST_ACT2_DESERT_FILL_WAGON_1 };
	const int nLevelPrestIds2[2] = { LVLPREST_ACT2_DESERT_VALLEY_RUIN_1, LVLPREST_ACT2_DESERT_VALLEY_RUIN_2 };

	DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds1, ARRAY_SIZE(nLevelPrestIds1), FALSE);
	DRLGOUTDESR_PlacePresetVariants(pLevel, nLevelPrestIds2, ARRAY_SIZE(nLevelPrestIds2), TRUE);
}
