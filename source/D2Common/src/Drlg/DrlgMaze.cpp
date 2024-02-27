#include "Drlg/D2DrlgMaze.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "Drlg/D2DrlgPreset.h"
#include "D2Seed.h"
#include <DataTbls/LevelsIds.h>

// Helper functions
static bool DRLGMAZE_CheckIfRoomOverlapsOrth(D2DrlgRoomStrc* pNewRoomEx, D2DrlgOrthStrc* pDrlgOrth)
{
	while (pDrlgOrth)
	{
		if (DRLG_ComputeRectanglesManhattanDistance(&pNewRoomEx->pDrlgCoord, pDrlgOrth->pBox, 0) == 0)
		{
			return true;
		}
		pDrlgOrth = pDrlgOrth->pNext;
	}
	return false;
}
static bool DRLGMAZE_CheckIfRoomOverlapsAythingOtherThanParent(D2DrlgRoomStrc* pNewRoomEx, D2DrlgRoomStrc* pParentRoomEx)
{
	return DRLGMAZE_CheckIfRoomOverlapsOrth(pNewRoomEx, pParentRoomEx->pDrlgOrth) || !DRLGMAZE_CheckRoomNotOverlaping(pNewRoomEx->pLevel, pNewRoomEx, pParentRoomEx, 0);
}
static bool DRLGMAZE_OrthWithDirectionExists(D2DrlgRoomStrc* pDrlgRoom, int nDirection)
{
	D2DrlgOrthStrc* pDrlgOrth = pDrlgRoom->pDrlgOrth;
	while (pDrlgOrth != nullptr)
	{
		if (pDrlgOrth->nDirection == nDirection)
		{
			return true;
		}
		pDrlgOrth = pDrlgOrth->pNext;
	}
	return false;
}


//D2Common.0x6FD78E50
D2DrlgRoomStrc* __fastcall DRLGMAZE_GetFreeLocationForRoomEast(D2DrlgLevelStrc* pLevel)
{
	D2DrlgRoomStrc* pResult = nullptr;

	for (D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
	{
		if ((!pResult || pDrlgRoom->nTileXPos < pResult->nTileXPos))
		{
			if (DRLGMAZE_CheckIfMayPlaceAdjacentPresetRoom(pDrlgRoom, ALTDIR_WEST))
			{
				pResult = pDrlgRoom;
			}
		}
	}

	return pResult;
}

//D2Common.0x6FD79240
D2DrlgRoomStrc* __fastcall DRLGMAZE_GetFreeLocationForRoomWest(D2DrlgLevelStrc* pLevel)
{
	D2DrlgRoomStrc* pResult = nullptr;

	for (D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
	{
		if ((!pResult || pDrlgRoom->nTileXPos > pResult->nTileXPos))
		{
			if (DRLGMAZE_CheckIfMayPlaceAdjacentPresetRoom(pDrlgRoom, ALTDIR_EAST))
			{
				pResult = pDrlgRoom;
			}
		}
	}
	return pResult;
}

//D2Common.0x6FD79360
D2DrlgRoomStrc* __fastcall DRLGMAZE_GetFreeLocationForRoomNorth(D2DrlgLevelStrc* pLevel)
{
	D2DrlgRoomStrc* pResult = nullptr;

	for (D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
	{
		if ((!pResult || pDrlgRoom->nTileYPos > pResult->nTileYPos)
			&& DRLGMAZE_CheckIfMayPlaceAdjacentPresetRoom(pDrlgRoom, ALTDIR_SOUTH))
		{
			pResult = pDrlgRoom;
		}
	}
	return pResult;
}

// Helper function
D2DrlgRoomStrc* __fastcall DRLGMAZE_GetFreeLocationForRoomSouth(D2DrlgLevelStrc* pLevel)
{
	D2DrlgRoomStrc* pResult = nullptr;

	for (D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
	{
		if ((!pResult || pDrlgRoom->nTileYPos < pResult->nTileYPos)
			&& DRLGMAZE_CheckIfMayPlaceAdjacentPresetRoom(pDrlgRoom, ALTDIR_NORTH))
		{
			pResult = pDrlgRoom;
		}
	}
	return pResult;
}

//D2Common.0x6FD78F70
void __fastcall DRLGMAZE_PickRoomPreset(D2DrlgRoomStrc* pDrlgRoom, BOOL bResetFlag)
{
	static const int dword_6FDCE710[][3] =
	{
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_ACT2_CORRUPT_HAREM_SW,	LVLPREST_ACT2_BASEMENT_SW,	LVLPREST_ACT3_SPIDER_SW },
		{ LVLPREST_ACT2_CORRUPT_HAREM_SE,	LVLPREST_ACT2_BASEMENT_SE,	LVLPREST_ACT3_SPIDER_SE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_ACT2_CORRUPT_HAREM_NW,	LVLPREST_ACT2_BASEMENT_NW,	LVLPREST_ACT3_SPIDER_NW },
		{ LVLPREST_ACT2_CORRUPT_HAREM_NE,	LVLPREST_ACT2_BASEMENT_NE,	LVLPREST_ACT3_SPIDER_NE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,					LVLPREST_NONE,				LVLPREST_NONE },
	};

	static const int dword_6FDCE7D0[][2] =
	{
		{ LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,				LVLPREST_ACT5_LAVA_W },
		{ LVLPREST_NONE,				LVLPREST_ACT5_LAVA_E },
		{ LVLPREST_NONE,				LVLPREST_ACT5_LAVA_EW },
		{ LVLPREST_NONE,				LVLPREST_ACT5_LAVA_S },
		{ LVLPREST_ACT5_TEMPLE_SW,		LVLPREST_NONE },
		{ LVLPREST_ACT5_TEMPLE_SE_UP,	LVLPREST_NONE },
		{ LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,				LVLPREST_ACT5_LAVA_N },
		{ LVLPREST_ACT5_TEMPLE_NW,		LVLPREST_NONE },
		{ LVLPREST_ACT5_TEMPLE_NE,		LVLPREST_NONE },
		{ LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,				LVLPREST_ACT5_LAVA_NS },
		{ LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,				LVLPREST_NONE },
		{ LVLPREST_NONE,				LVLPREST_NONE }
	};

	int nPickedFile = -1;
	int nLevelPrest = 0;

	for (D2DrlgOrthStrc* pDrlgOrth = pDrlgRoom->pDrlgOrth; pDrlgOrth; pDrlgOrth = pDrlgOrth->pNext)
	{
		switch (pDrlgOrth->nDirection)
		{
		case 0:
			nLevelPrest |= 1;
			break;

		case 1:
			nLevelPrest |= 8;
			break;

		case 2:
			nLevelPrest |= 2;
			break;

		case 3:
			nLevelPrest |= 4;
			break;

		default:
			break;
		}
	}

	switch (pDrlgRoom->pLevel->nLevelType)
	{
	case LVLTYPE_ACT1_CAVE:
		nLevelPrest += LVLPREST_ACT1_DOE_ENTRANCE;
		break;

	case LVLTYPE_ACT1_CRYPT:
		nLevelPrest += LVLPREST_ACT1_GRAVEYARD;
		break;

	case LVLTYPE_ACT1_BARRACKS:
		nLevelPrest += LVLPREST_ACT1_BARRACKS_COURT_CONNECT;
		break;

	case LVLTYPE_ACT1_JAIL:
		nLevelPrest += LVLPREST_ACT1_BARRACKS_FORGE_N;
		break;

	case LVLTYPE_ACT1_CATACOMBS:
		nLevelPrest += LVLPREST_ACT1_CATHEDRAL;
		break;

	case LVLTYPE_ACT2_SEWER:
		nLevelPrest += LVLPREST_ACT2_TOWN;
		break;

	case LVLTYPE_ACT2_HAREM:
		nLevelPrest = dword_6FDCE710[nLevelPrest][0];
		break;

	case LVLTYPE_ACT2_BASEMENT:
		nLevelPrest = dword_6FDCE710[nLevelPrest][1];
		if (pDrlgRoom->pLevel->nLevelId == LEVEL_PALACECELLARLEV1 && nLevelPrest == LVLPREST_ACT2_BASEMENT_NW)
		{
			nPickedFile = 2;
		}

		if (pDrlgRoom->pLevel->nLevelId == LEVEL_PALACECELLARLEV3 && (nLevelPrest == LVLPREST_ACT2_BASEMENT_NW || nLevelPrest == LVLPREST_ACT2_BASEMENT_SE))
		{
			nPickedFile = 3;
		}
		break;

	case LVLTYPE_ACT2_TOMB:
		nLevelPrest += LVLPREST_ACT2_DESERT_RUINS_ELDER;
		break;

	case LVLTYPE_ACT2_LAIR:
		nLevelPrest += LVLPREST_ACT2_DURIELS_LAIR;
		break;

	case LVLTYPE_ACT2_ARCANE:
		nLevelPrest += LVLPREST_ACT2_LAIR_TIGHT_SPOT_S;
		break;

	case LVLTYPE_ACT3_KURAST:
		nLevelPrest += LVLPREST_ACT3_TEMPLE_6;
		break;

	case LVLTYPE_ACT3_SPIDER:
		nLevelPrest = dword_6FDCE710[nLevelPrest][2];
		if (pDrlgRoom->pLevel->nLevelId == LEVEL_SPIDERCAVE && nLevelPrest == LVLPREST_ACT3_SPIDER_NE)
		{
			nLevelPrest = LVLPREST_ACT3_SPIDER_CHEST_NE;
		}

		if (pDrlgRoom->pLevel->nLevelId == LEVEL_SPIDERCAVERN && nLevelPrest == LVLPREST_ACT3_SPIDER_NW)
		{
			nLevelPrest = LVLPREST_ACT3_SPIDER_CHEST_NW;
		}
		break;

	case LVLTYPE_ACT3_DUNGEON:
		nLevelPrest += LVLPREST_ACT3_SPIDER_CHEST_NE;
		break;

	case LVLTYPE_ACT3_SEWER:
		nLevelPrest += LVLPREST_ACT3_DUNGEON_TREASURE_2;
		break;

	case LVLTYPE_ACT4_LAVA:
		nLevelPrest += LVLPREST_ACT4_LAVA_X;
		break;

	case LVLTYPE_ACT5_ICE_CAVES:
		D2_ASSERT(pDrlgRoom->pLevel);
		D2_ASSERT(pDrlgRoom->pLevel->pMaze);

		if (pDrlgRoom->pLevel->pMaze->dwRooms[pDrlgRoom->pLevel->pDrlg->nDifficulty] != 1)
		{
			nLevelPrest += LVLPREST_ACT5_BARRICADE_16_SNOW;
		}
		break;

	case LVLTYPE_ACT5_TEMPLE:
		nLevelPrest = dword_6FDCE7D0[nLevelPrest][0];
		break;

	case LVLTYPE_ACT5_BAAL:
		nLevelPrest += LVLPREST_ACT5_LAVA_NS;
		break;

	case LVLTYPE_ACT5_LAVA:
		nLevelPrest = dword_6FDCE7D0[nLevelPrest][1];
		break;

	default:
		FOG_DisplayHalt("sSetChamberPreset() - Some really bad voodoo here!", __FILE__, __LINE__);
		exit(-1);
		return;
	}

	if (nLevelPrest)
	{
		DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, nLevelPrest, nPickedFile, bResetFlag);
	}
}


//D2Common.0x6FD79480
void __fastcall DRLGMAZE_InitLevelData(D2DrlgLevelStrc* pLevel)
{
	pLevel->pMaze = DATATBLS_GetLvlMazeTxtRecordFromLevelId(pLevel->nLevelId);
	DRLG_SetLevelPositionAndSize(pLevel->pDrlg, pLevel);
}

//D2Common.0x6FD794A0
void __fastcall DRLGMAZE_GenerateLevel(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct1CavePrevIds[] =
	{
		{ LVLPREST_ACT1_CAVE_N, LVLPREST_ACT1_CAVE_PREV_N, -1, 3 },
		{ LVLPREST_ACT1_CAVE_E, LVLPREST_ACT1_CAVE_PREV_E, -1, 0 },
		{ LVLPREST_ACT1_CAVE_S, LVLPREST_ACT1_CAVE_PREV_S, -1, 1 },
		{ LVLPREST_ACT1_CAVE_W, LVLPREST_ACT1_CAVE_PREV_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1CaveDOEIds[] =
	{
		{ LVLPREST_ACT1_CAVE_N, LVLPREST_ACT1_CAVE_DEN_OF_EVIL_N, -1, 3 },
		{ LVLPREST_ACT1_CAVE_E, LVLPREST_ACT1_CAVE_DEN_OF_EVIL_E, -1, 0 },
		{ LVLPREST_ACT1_CAVE_S, LVLPREST_ACT1_CAVE_DEN_OF_EVIL_S, -1, 1 },
		{ LVLPREST_ACT1_CAVE_W, LVLPREST_ACT1_CAVE_DEN_OF_EVIL_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1CaveDownIds[] =
	{
		{ LVLPREST_ACT1_CAVE_N, LVLPREST_ACT1_CAVE_DOWN_N, -1, 3 },
		{ LVLPREST_ACT1_CAVE_E, LVLPREST_ACT1_CAVE_DOWN_E, -1, 0 },
		{ LVLPREST_ACT1_CAVE_S, LVLPREST_ACT1_CAVE_DOWN_S, -1, 1 },
		{ LVLPREST_ACT1_CAVE_W, LVLPREST_ACT1_CAVE_DOWN_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1CaveColdcrowIds[] =
	{
		{ LVLPREST_ACT1_CAVE_N, LVLPREST_ACT1_CAVE_COLDCROW_N, -1, 3 },
		{ LVLPREST_ACT1_CAVE_E, LVLPREST_ACT1_CAVE_COLDCROW_E, -1, 0 },
		{ LVLPREST_ACT1_CAVE_S, LVLPREST_ACT1_CAVE_COLDCROW_S, -1, 1 },
		{ LVLPREST_ACT1_CAVE_W, LVLPREST_ACT1_CAVE_COLDCROW_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1CaveNextIds[] =
	{
		{ LVLPREST_ACT1_CAVE_N, LVLPREST_ACT1_CAVE_NEXT_N, -1, 3 },
		{ LVLPREST_ACT1_CAVE_E, LVLPREST_ACT1_CAVE_NEXT_E, -1, 0 },
		{ LVLPREST_ACT1_CAVE_S, LVLPREST_ACT1_CAVE_NEXT_S, -1, 1 },
		{ LVLPREST_ACT1_CAVE_W, LVLPREST_ACT1_CAVE_NEXT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1CryptPrevIds[] =
	{
		{ LVLPREST_ACT1_CRYPT_N, LVLPREST_ACT1_CRYPT_PREV_N, -1, 3 },
		{ LVLPREST_ACT1_CRYPT_E, LVLPREST_ACT1_CRYPT_PREV_E, -1, 0 },
		{ LVLPREST_ACT1_CRYPT_S, LVLPREST_ACT1_CRYPT_PREV_S, -1, 1 },
		{ LVLPREST_ACT1_CRYPT_W, LVLPREST_ACT1_CRYPT_PREV_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1CryptSpecialIds[] =
	{
		{ LVLPREST_ACT1_CRYPT_N, LVLPREST_ACT1_CRYPT_BONEBREAK_N, -1, 3 },
		{ LVLPREST_ACT1_CRYPT_E, LVLPREST_ACT1_CRYPT_BONEBREAK_E, -1, 0 },
		{ LVLPREST_ACT1_CRYPT_S, LVLPREST_ACT1_CRYPT_BONEBREAK_S, -1, 1 },
		{ LVLPREST_ACT1_CRYPT_W, LVLPREST_ACT1_CRYPT_BONEBREAK_W, -1, 2 },
		{ LVLPREST_ACT1_CRYPT_N, LVLPREST_ACT1_CRYPT_PORTAL_N, -1, 3 },
		{ LVLPREST_ACT1_CRYPT_E, LVLPREST_ACT1_CRYPT_PORTAL_E, -1, 0 },
		{ LVLPREST_ACT1_CRYPT_S, LVLPREST_ACT1_CRYPT_PORTAL_S, -1, 1 },
		{ LVLPREST_ACT1_CRYPT_W, LVLPREST_ACT1_CRYPT_PORTAL_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1CryptChestIds[] =
	{
		{ LVLPREST_ACT1_CRYPT_N, LVLPREST_ACT1_CRYPT_CHEST_N, -1, 3 },
		{ LVLPREST_ACT1_CRYPT_E, LVLPREST_ACT1_CRYPT_CHEST_E, -1, 0 },
		{ LVLPREST_ACT1_CRYPT_S, LVLPREST_ACT1_CRYPT_CHEST_S, -1, 1 },
		{ LVLPREST_ACT1_CRYPT_W, LVLPREST_ACT1_CRYPT_CHEST_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1CryptNextIds[] =
	{
		{ LVLPREST_ACT1_CRYPT_N, LVLPREST_ACT1_CRYPT_NEXT_N, -1, 3 },
		{ LVLPREST_ACT1_CRYPT_E, LVLPREST_ACT1_CRYPT_NEXT_E, -1, 0 },
		{ LVLPREST_ACT1_CRYPT_S, LVLPREST_ACT1_CRYPT_NEXT_S, -1, 1 },
		{ LVLPREST_ACT1_CRYPT_W, LVLPREST_ACT1_CRYPT_NEXT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1JailPrevIds[] =
	{
		{ LVLPREST_ACT1_JAIL_N, LVLPREST_ACT1_JAIL_PREV_N, -1, 3 },
		{ LVLPREST_ACT1_JAIL_E, LVLPREST_ACT1_JAIL_PREV_E, -1, 0 },
		{ LVLPREST_ACT1_JAIL_S, LVLPREST_ACT1_JAIL_PREV_S, -1, 1 },
		{ LVLPREST_ACT1_JAIL_W, LVLPREST_ACT1_JAIL_PREV_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1JailCathIds[] =
	{
		{ LVLPREST_ACT1_JAIL_N, LVLPREST_ACT1_JAIL_CATH_N, -1, 3 },
		{ LVLPREST_ACT1_JAIL_E, LVLPREST_ACT1_JAIL_CATH_E, -1, 0 },
		{ LVLPREST_ACT1_JAIL_S, LVLPREST_ACT1_JAIL_CATH_S, -1, 1 },
		{ LVLPREST_ACT1_JAIL_W, LVLPREST_ACT1_JAIL_CATH_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1JailNextIds[] =
	{
		{ LVLPREST_ACT1_JAIL_N, LVLPREST_ACT1_JAIL_NEXT_N, -1, 3 },
		{ LVLPREST_ACT1_JAIL_E, LVLPREST_ACT1_JAIL_NEXT_E, -1, 0 },
		{ LVLPREST_ACT1_JAIL_S, LVLPREST_ACT1_JAIL_NEXT_S, -1, 1 },
		{ LVLPREST_ACT1_JAIL_W, LVLPREST_ACT1_JAIL_NEXT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1JailWaypointIds[] =
	{
		{ LVLPREST_ACT1_JAIL_N, LVLPREST_ACT1_JAIL_WAYPOINT_N, -1, 3 },
		{ LVLPREST_ACT1_JAIL_E, LVLPREST_ACT1_JAIL_WAYPOINT_E, -1, 0 },
		{ LVLPREST_ACT1_JAIL_S, LVLPREST_ACT1_JAIL_WAYPOINT_S, -1, 1 },
		{ LVLPREST_ACT1_JAIL_W, LVLPREST_ACT1_JAIL_WAYPOINT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1JailPitspawnIds[] =
	{
		{ LVLPREST_ACT1_JAIL_N, LVLPREST_ACT1_JAIL_PITSPAWN_N, -1, 3 },
		{ LVLPREST_ACT1_JAIL_E, LVLPREST_ACT1_JAIL_PITSPAWN_E, -1, 0 },
		{ LVLPREST_ACT1_JAIL_S, LVLPREST_ACT1_JAIL_PITSPAWN_S, -1, 1 },
		{ LVLPREST_ACT1_JAIL_W, LVLPREST_ACT1_JAIL_PITSPAWN_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1CatacombsNextIds[] =
	{
		{ LVLPREST_ACT1_CATACOMBS_N, LVLPREST_ACT1_CATACOMBS_NEXT_N, -1, 3 },
		{ LVLPREST_ACT1_CATACOMBS_E, LVLPREST_ACT1_CATACOMBS_NEXT_E, -1, 0 },
		{ LVLPREST_ACT1_CATACOMBS_S, LVLPREST_ACT1_CATACOMBS_NEXT_S, -1, 1 },
		{ LVLPREST_ACT1_CATACOMBS_W, LVLPREST_ACT1_CATACOMBS_NEXT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1CatacombsWaypointIds[] =
	{
		{ LVLPREST_ACT1_CATACOMBS_N, LVLPREST_ACT1_CATACOMBS_WAYPOINT_N, -1, 3 },
		{ LVLPREST_ACT1_CATACOMBS_E, LVLPREST_ACT1_CATACOMBS_WAYPOINT_E, -1, 0 },
		{ LVLPREST_ACT1_CATACOMBS_S, LVLPREST_ACT1_CATACOMBS_WAYPOINT_S, -1, 1 },
		{ LVLPREST_ACT1_CATACOMBS_W, LVLPREST_ACT1_CATACOMBS_WAYPOINT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2ArcaneSummonerIds[] =
	{
		{ LVLPREST_ACT2_ARCANE_N, LVLPREST_ACT2_ARCANE_SUMMONER_N, -1, 3 },
		{ LVLPREST_ACT2_ARCANE_E, LVLPREST_ACT2_ARCANE_SUMMONER_E, -1, 0 },
		{ LVLPREST_ACT2_ARCANE_S, LVLPREST_ACT2_ARCANE_SUMMONER_S, -1, 1 },
		{ LVLPREST_ACT2_ARCANE_W, LVLPREST_ACT2_ARCANE_SUMMONER_W, -1, 2 },
	};

	D2DrlgRoomStrc* pRandomRoomEx = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	int nDirection = 0;
	int nRooms = 0;
	int nRand = 0;

	pDrlgRoom = DRLGROOM_AllocRoomEx(pLevel, DRLGTYPE_PRESET);

	pDrlgRoom->nTileWidth = pDrlgRoom->pLevel->pMaze->dwSizeX;
	pDrlgRoom->nTileHeight = pDrlgRoom->pLevel->pMaze->dwSizeY;

	pDrlgRoom->nTileXPos = pLevel->nPosX + (pLevel->nWidth - pDrlgRoom->nTileWidth) / 2;
	pDrlgRoom->nTileYPos = pLevel->nPosY + (pLevel->nHeight - pDrlgRoom->nTileHeight) / 2;

	DRLGROOM_AddRoomExToLevel(pLevel, pDrlgRoom);
	
	D2DrlgRoomStrc* pLevelFirstRoomEx = pLevel->pFirstRoomEx;

	switch (pLevel->nLevelType)
	{
	case LVLTYPE_ACT1_CAVE:
		nRooms = pLevel->pMaze->dwRooms[pLevel->pDrlg->nDifficulty];

		if (pLevel->nLevelId == pLevel->pDrlg->nStaffTombLevel)
		{
			nRooms *= 3;
		}
		else if (pLevel->nLevelId == pLevel->pDrlg->nBossTombLevel)
		{
			nRooms *= 2;
		}

		while (pLevel->nRooms < nRooms)
		{
			pRandomRoomEx = DRLGMAZE_GetRandomRoomExFromLevel(pLevel);
			nDirection = SEED_RollRandomNumber(&pRandomRoomEx->pSeed) & 3;
			if (!DRLGMAZE_HasMapDS1(pRandomRoomEx))
			{
				DRLGMAZE_AddAdjacentMazeRoom(pRandomRoomEx, nDirection, 1);
			}
		}

		nRand = SEED_RollRandomNumber(&pLevel->pSeed) & 3;
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CavePrevIds[nRand], &nRand);

		if (pLevel->nLevelId == LEVEL_DENOFEVIL)
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CaveDOEIds[nRand], &nRand);
		}
		else
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CaveDownIds[nRand], &nRand);
		}

		if (pLevel->nLevelId == LEVEL_CAVELEV1)
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CaveColdcrowIds[nRand], &nRand);
		}

		if (pLevel->nLevelId == LEVEL_UNDERGROUNDPASSAGELEV1)
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CaveNextIds[nRand], &nRand);
		}
		break;

	case LVLTYPE_ACT1_CRYPT:
		nRooms = pLevel->pMaze->dwRooms[pLevel->pDrlg->nDifficulty];

		if (pLevel->nLevelId == pLevel->pDrlg->nStaffTombLevel)
		{
			nRooms *= 3;
		}
		else if (pLevel->nLevelId == pLevel->pDrlg->nBossTombLevel)
		{
			nRooms *= 2;
		}

		while (pLevel->nRooms < nRooms)
		{
			pRandomRoomEx = DRLGMAZE_GetRandomRoomExFromLevel(pLevel);
			nDirection = SEED_RollRandomNumber(&pRandomRoomEx->pSeed) & 3;
			if (!DRLGMAZE_HasMapDS1(pRandomRoomEx))
			{
				DRLGMAZE_AddAdjacentMazeRoom(pRandomRoomEx, nDirection, 1);
			}
		}

		nRand = SEED_RollRandomNumber(&pLevel->pSeed) & 3;
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CryptPrevIds[nRand], &nRand);
		if (pLevel->nLevelId == LEVEL_CRYPT)
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CryptSpecialIds[nRand], &nRand);
		}

		if (pLevel->nLevelId == LEVEL_MAUSOLEUM)
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CryptChestIds[nRand], &nRand);
		}

		if (pLevel->nLevelId >= LEVEL_TOWERCELLARLEV1 && pLevel->nLevelId <= LEVEL_TOWERCELLARLEV4)
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CryptNextIds[nRand], &nRand);
		}
		break;

	case LVLTYPE_ACT1_BARRACKS:
		DRLGMAZE_InitBasicMazeLayout(pLevel, 2);

		nRooms = pLevel->pMaze->dwRooms[pLevel->pDrlg->nDifficulty];

		if (pLevel->nLevelId == pLevel->pDrlg->nStaffTombLevel)
		{
			nRooms *= 3;
		}
		else if (pLevel->nLevelId == pLevel->pDrlg->nBossTombLevel)
		{
			nRooms *= 2;
		}

		while (pLevel->nRooms < nRooms)
		{
			pRandomRoomEx = DRLGMAZE_GetRandomRoomExFromLevel(pLevel);
			nDirection = SEED_RollRandomNumber(&pRandomRoomEx->pSeed) & 3;
			if (!DRLGMAZE_HasMapDS1(pRandomRoomEx))
			{
				DRLGMAZE_AddAdjacentMazeRoom(pRandomRoomEx, nDirection, 1);
			}
		}
		break;

	case LVLTYPE_ACT1_JAIL:
		DRLGMAZE_InitBasicMazeLayout(pLevel, 2);
		
		nRooms = pLevel->pMaze->dwRooms[pLevel->pDrlg->nDifficulty];

		if (pLevel->nLevelId == pLevel->pDrlg->nStaffTombLevel)
		{
			nRooms *= 3;
		}
		else if (pLevel->nLevelId == pLevel->pDrlg->nBossTombLevel)
		{
			nRooms *= 2;
		}

		while (pLevel->nRooms < nRooms)
		{
			pRandomRoomEx = DRLGMAZE_GetRandomRoomExFromLevel(pLevel);
			nDirection = SEED_RollRandomNumber(&pRandomRoomEx->pSeed) & 3;
			if (!DRLGMAZE_HasMapDS1(pRandomRoomEx))
			{
				DRLGMAZE_AddAdjacentMazeRoom(pRandomRoomEx, nDirection, 1);
			}
		}

		nRand = SEED_RollRandomNumber(&pLevel->pSeed) & 3;
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1JailPrevIds[nRand], &nRand);

		if (pLevel->nLevelId == LEVEL_JAILLEV1)
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1JailWaypointIds[nRand], &nRand);
		}

		if (pLevel->nLevelId == LEVEL_JAILLEV2)
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1JailPitspawnIds[nRand], &nRand);
		}

		if (pLevel->nLevelId == LEVEL_JAILLEV3)
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1JailCathIds[nRand], &nRand);
		}
		else
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1JailNextIds[nRand], &nRand);
		}
		break;

	case LVLTYPE_ACT1_CATACOMBS:
		if (pLevel->nLevelId == LEVEL_CATACOMBSLEV1)
		{
			DRLGMAZE_AddAdjacentMazeRoom(pLevelFirstRoomEx, ALTDIR_NORTH, TRUE);
			DRLGMAZE_AddAdjacentMazeRoom(pLevelFirstRoomEx, ALTDIR_EAST, TRUE);
			DRLGMAZE_AddAdjacentMazeRoom(pLevelFirstRoomEx, ALTDIR_SOUTH, TRUE);
			DRLGMAZE_AddAdjacentMazeRoom(pLevelFirstRoomEx, ALTDIR_WEST, TRUE);
			DRLGMAZE_SetPickedFileAndPresetId(pLevelFirstRoomEx, LVLPREST_ACT1_CATACOMBS_PREV_NSEW, -1, FALSE);
		}
		else
		{
			if (SEED_RollRandomNumber(&pLevel->pSeed) & 1)
			{
				DRLGMAZE_AddAdjacentMazeRoom(pLevelFirstRoomEx, ALTDIR_WEST, TRUE);
				DRLGMAZE_AddAdjacentMazeRoom(pLevelFirstRoomEx, ALTDIR_EAST, TRUE);
				DRLGMAZE_SetPickedFileAndPresetId(pLevelFirstRoomEx, LVLPREST_ACT1_CATACOMBS_PREV_EW, -1, FALSE);
			}
			else
			{
				DRLGMAZE_AddAdjacentMazeRoom(pLevelFirstRoomEx, ALTDIR_NORTH, TRUE);
				DRLGMAZE_AddAdjacentMazeRoom(pLevelFirstRoomEx, ALTDIR_SOUTH, TRUE);
				DRLGMAZE_SetPickedFileAndPresetId(pLevelFirstRoomEx, LVLPREST_ACT1_CATACOMBS_PREV_NS, -1, FALSE);
			}
		}

		nRooms = pLevel->pMaze->dwRooms[pLevel->pDrlg->nDifficulty];

		if (pLevel->nLevelId == pLevel->pDrlg->nStaffTombLevel)
		{
			nRooms *= 3;
		}
		else if (pLevel->nLevelId == pLevel->pDrlg->nBossTombLevel)
		{
			nRooms *= 2;
		}

		while (pLevel->nRooms < nRooms)
		{
			pRandomRoomEx = DRLGMAZE_GetRandomRoomExFromLevel(pLevel);
			nDirection = SEED_RollRandomNumber(&pRandomRoomEx->pSeed) & 3;
			if (!DRLGMAZE_HasMapDS1(pRandomRoomEx))
			{
				DRLGMAZE_AddAdjacentMazeRoom(pRandomRoomEx, nDirection, 1);
			}
		}

		nRand = SEED_RollRandomNumber(&pLevel->pSeed) & 3;
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CatacombsNextIds[nRand], &nRand);

		if (pLevel->nLevelId == LEVEL_CATACOMBSLEV2)
		{
			DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1CatacombsWaypointIds[nRand], &nRand);
		}
		break;

	case LVLTYPE_ACT2_SEWER:
		DRLGMAZE_InitBasicMazeLayout(pLevel, 2);

		nRooms = pLevel->pMaze->dwRooms[pLevel->pDrlg->nDifficulty];

		if (pLevel->nLevelId == pLevel->pDrlg->nStaffTombLevel)
		{
			nRooms *= 3;
		}
		else if (pLevel->nLevelId == pLevel->pDrlg->nBossTombLevel)
		{
			nRooms *= 2;
		}

		while (pLevel->nRooms < nRooms)
		{
			pRandomRoomEx = DRLGMAZE_GetRandomRoomExFromLevel(pLevel);
			nDirection = SEED_RollRandomNumber(&pRandomRoomEx->pSeed) & 3;
			if (!DRLGMAZE_HasMapDS1(pRandomRoomEx))
			{
				DRLGMAZE_AddAdjacentMazeRoom(pRandomRoomEx, nDirection, 1);
			}
		}

		DRLGMAZE_ScanReplaceSpecialAct2SewersPresets(pLevel);
		break;

	case LVLTYPE_ACT2_TOMB:
		if (pLevel->nLevelId == LEVEL_CLAWVIPERTEMPLELEV2)
		{
			pDrlgRoom->pMaze->nLevelPrest = LVLPREST_ACT2_TOMB_TAINTED_SUN_X;
			pDrlgRoom->pMaze->nPickedFile = -1;
			pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
		}
		else
		{
			DRLGMAZE_PlaceAct2TombPrev_Act5BaalPrev(pLevel);
			DRLGMAZE_BuildBasicMaze(pLevel);
			DRLGMAZE_PlaceAct2TombStuff(pLevel);
		}
		break;

	case LVLTYPE_ACT2_LAIR:
		DRLGMAZE_InitBasicMazeLayout(pLevel, 2);
		DRLGMAZE_BuildBasicMaze(pLevel);
		DRLGMAZE_PlaceAct2LairStuff(pLevel);
		break;

	case LVLTYPE_ACT2_ARCANE:
		DRLGMAZE_PlaceArcaneSanctuary(pLevel);

		nRand = SEED_RollRandomNumber(&pLevel->pSeed) & 3;
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct2ArcaneSummonerIds[nRand], &nRand);
		break;

	case LVLTYPE_ACT2_HAREM:
	case LVLTYPE_ACT2_BASEMENT:
	case LVLTYPE_ACT3_SPIDER:
		DRLGMAZE_InitBasicMazeLayout(pLevel, 2);
		break;

	case LVLTYPE_ACT3_KURAST:
		DRLGMAZE_InitBasicMazeLayout(pLevel, 2);
		DRLGMAZE_BuildBasicMaze(pLevel);
		DRLGMAZE_PlaceAct3MephistoStuff(pLevel);
		break;

	case LVLTYPE_ACT3_DUNGEON:
		DRLGMAZE_InitBasicMazeLayout(pLevel, 2);
		DRLGMAZE_BuildBasicMaze(pLevel);
		DRLGMAZE_PlaceAct3DungeonStuff(pLevel);
		break;

	case LVLTYPE_ACT3_SEWER:
		if (pLevel->nLevelId == LEVEL_SEWERSA3LEV1)
		{
			DRLGMAZE_InitBasicMazeLayout(pLevel, 5);

			if (!DRLGMAZE_ReplaceRoomPreset(pLevel, LVLPREST_ACT3_SEWER_SW, LVLPREST_ACT3_SEWER_PREV_SW, -1, 0))
			{
				FOG_DisplayWarning("ptRoom", __FILE__, __LINE__);
			}

			if (!DRLGMAZE_ReplaceRoomPreset(pLevel, LVLPREST_ACT3_SEWER_SE, LVLPREST_ACT3_SEWER_PREV_SE, -1, 0))
			{
				FOG_DisplayWarning("ptRoom", __FILE__, __LINE__);
			}

			if (!DRLGMAZE_ReplaceRoomPreset(pLevel, LVLPREST_ACT3_SEWER_NW, LVLPREST_ACT3_SEWER_PREV_NW, -1, 0))
			{
				FOG_DisplayWarning("ptRoom", __FILE__, __LINE__);
			}

			if (!DRLGMAZE_ReplaceRoomPreset(pLevel, LVLPREST_ACT3_SEWER_NE, LVLPREST_ACT3_SEWER_PREV_NE, -1, 0))
			{
				FOG_DisplayWarning("ptRoom", __FILE__, __LINE__);
			}
		}
		else
		{
			DRLGMAZE_InitBasicMazeLayout(pLevel, 2);
		}

		DRLGMAZE_BuildBasicMaze(pLevel);
		DRLGMAZE_PlaceAct3SewerStuff(pLevel);
		break;

	case LVLTYPE_ACT4_LAVA:
		DRLGMAZE_BuildBasicMaze(pLevel);
		break;

	case LVLTYPE_ACT5_ICE_CAVES:
		switch (pLevel->nLevelId)
		{
		case LEVEL_CELLAROFPITY:
			D2_ASSERT(pLevelFirstRoomEx);

			if (SEED_RollLimitedRandomNumber(&pDrlgRoom->pLevel->pSeed, 2) != 0)
			{
				DRLGMAZE_SetPickedFileAndPresetId(pLevelFirstRoomEx, LVLPREST_ACT5_ICE_RIVER_A, -1, FALSE);
			}
			else
			{
				DRLGMAZE_SetPickedFileAndPresetId(pLevelFirstRoomEx, LVLPREST_ACT5_ICE_RIVER_B, -1, FALSE);
			}
			break;

		case LEVEL_ECHOCHAMBER:
			DRLGMAZE_SetPickedFileAndPresetId(pLevelFirstRoomEx, LVLPREST_ACT5_ICE_POOL_A, -1, FALSE);
			break;

		case LEVEL_GLACIALCAVESLEV2:
			DRLGMAZE_SetPickedFileAndPresetId(pLevelFirstRoomEx, LVLPREST_ACT5_ICE_POOL_B, -1, FALSE);
			break;

		default:
			DRLGMAZE_InitBasicMazeLayout(pLevel, 2);
			DRLGMAZE_BuildBasicMaze(pLevel);
			DRLGMAZE_PlaceAct5IceStuff(pLevel);
			break;
		}
		break;

	case LVLTYPE_ACT5_TEMPLE:
		DRLGMAZE_InitBasicMazeLayout(pLevel, 2);
		DRLGMAZE_PlaceAct5TempleStuff(pLevel);
		break;

	case LVLTYPE_ACT5_BAAL:
		DRLGMAZE_PlaceAct2TombPrev_Act5BaalPrev(pLevel);
		DRLGMAZE_BuildBasicMaze(pLevel);
		DRLGMAZE_PlaceAct5BaalStuff(pLevel);
		break;

	case LVLTYPE_ACT5_LAVA:
		DRLGMAZE_PlaceAct5LavaPresets(pLevel);
		break;

	default:
		FOG_DisplayHalt("MazeLevelGenerate() - Some really bad voodoo here!", __FILE__, __LINE__);
		exit(-1);
		return;
	}

	if (pLevel->nLevelId == LEVEL_BARRACKS)
	{
		DRLGMAZE_PlaceAct1Barracks(pLevel);
	}
	else if (pLevel->nLevelId == LEVEL_RIVEROFFLAME)
	{
		DRLGMAZE_PlaceAct4Lava(pLevel);
	}
	else
	{
		DRLG_UpdateRoomExCoordinates(pLevel);
	}

	DRLGMAZE_RollAct_1_2_3_BasicPresets(pLevel);
	DRLGMAZE_RollBasicPresets(pLevel);
}

//D2Common.0x6FD79E10
void __fastcall DRLGMAZE_SetPickedFileAndPresetId(D2DrlgRoomStrc* pDrlgRoom, int nLevelPrest, int nPickedFile, BOOL bResetFlag)
{
	pDrlgRoom->pMaze->nPickedFile = nPickedFile;
	pDrlgRoom->pMaze->nLevelPrest = nLevelPrest;

	if (bResetFlag)
	{
		pDrlgRoom->pMaze->dwFlags &= (~DRLGPRESETROOMFLAG_HAS_MAP_DS1);
	}
	else
	{
		pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
	}
}

//D2Common.0x6FD79E40
D2DrlgRoomStrc* __fastcall DRLGMAZE_ReplaceRoomPreset(D2DrlgLevelStrc* pLevel, int nLevelPrestId1, int nLevelPrestId2, int nPickedFile, BOOL bResetFlag)
{
	for (D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == nLevelPrestId1)
		{
			DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, nLevelPrestId2, nPickedFile, bResetFlag);
			return pDrlgRoom;
		}
	}

	return NULL;
}

//D2Common.0x6FD79EA0
D2DrlgRoomStrc* __fastcall DRLGMAZE_AddAdjacentMazeRoom(D2DrlgRoomStrc* pDrlgRoom, int nDirection, BOOL bMergeRooms)
{
	D2DrlgOrthStrc* pDrlgOrth = NULL;
	D2DrlgRoomStrc* pNewRoomEx = NULL;
	int nDir = 0;
	int nX = 0;
	int nY = 0;

	pNewRoomEx = DRLGROOM_AllocRoomEx(pDrlgRoom->pLevel, DRLGTYPE_PRESET);
	pNewRoomEx->nTileWidth = pNewRoomEx->pLevel->pMaze->dwSizeX;
	pNewRoomEx->nTileHeight = pNewRoomEx->pLevel->pMaze->dwSizeY;

	switch (nDirection)
	{
	case 0:
		pNewRoomEx->nTileXPos = pDrlgRoom->nTileXPos - pDrlgRoom->nTileWidth;
		pNewRoomEx->nTileYPos = pDrlgRoom->nTileYPos;
		break;

	case 1:
		pNewRoomEx->nTileXPos = pDrlgRoom->nTileXPos;
		pNewRoomEx->nTileYPos = pDrlgRoom->nTileYPos - pDrlgRoom->nTileHeight;
		break;

	case 2:
		pNewRoomEx->nTileXPos = pDrlgRoom->nTileXPos + pDrlgRoom->nTileWidth;
		pNewRoomEx->nTileYPos = pDrlgRoom->nTileYPos;
		break;

	case 3:
		pNewRoomEx->nTileXPos = pDrlgRoom->nTileXPos;
		pNewRoomEx->nTileYPos = pDrlgRoom->nTileYPos + pDrlgRoom->nTileHeight;
		break;

	case 4:
		pNewRoomEx->nTileXPos = pDrlgRoom->nTileXPos - pDrlgRoom->nTileWidth;
		pNewRoomEx->nTileYPos = pDrlgRoom->nTileYPos - pDrlgRoom->nTileHeight;
		break;

	case 5:
		pNewRoomEx->nTileXPos = pDrlgRoom->nTileXPos + pDrlgRoom->nTileWidth;
		pNewRoomEx->nTileYPos = pDrlgRoom->nTileYPos - pDrlgRoom->nTileHeight;
		break;

	case 6:
		pNewRoomEx->nTileXPos = pDrlgRoom->nTileXPos + pDrlgRoom->nTileWidth;
		pNewRoomEx->nTileYPos = pDrlgRoom->nTileYPos + pDrlgRoom->nTileHeight;
		break;

	case 7:
		pNewRoomEx->nTileXPos = pDrlgRoom->nTileXPos - pDrlgRoom->nTileWidth;
		pNewRoomEx->nTileYPos = pDrlgRoom->nTileYPos + pDrlgRoom->nTileHeight;
		break;

	default:
		break;
	}

	for (D2DrlgOrthStrc* i = pDrlgRoom->pDrlgOrth; i; i = i->pNext)
	{
		if (!DRLG_ComputeRectanglesManhattanDistance(&pNewRoomEx->pDrlgCoord, i->pBox, 0))
		{
			DRLGROOM_FreeRoomEx(pNewRoomEx);
			return 0;
		}
	}

	if (!DRLGMAZE_CheckRoomNotOverlaping(pNewRoomEx->pLevel, pNewRoomEx, pDrlgRoom, 0))
	{
		DRLGROOM_FreeRoomEx(pNewRoomEx);
		return 0;
	}

	DRLGROOM_AllocDrlgOrthsForRooms(pDrlgRoom, pNewRoomEx, nDirection);

	if (bMergeRooms && !DRLGMAZE_HasMapDS1(pDrlgRoom))
	{
		for (D2DrlgRoomStrc* i = pNewRoomEx->pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			if (i != pNewRoomEx && !DRLGMAZE_HasMapDS1(i))
			{
				nX = 0;
				nY = 0;

				if (!sub_6FD77740(&pNewRoomEx->pDrlgCoord, &i->pDrlgCoord, 1, &nX, &nY))
				{
					if (nX != nY)
					{
						pDrlgOrth = pNewRoomEx->pDrlgOrth;
						while (pDrlgOrth && pDrlgOrth->pDrlgRoom != i)
						{
							pDrlgOrth = pDrlgOrth->pNext;
						}

						if (!pDrlgOrth)
						{
							if ((int)((unsigned int)SEED_RollRandomNumber(&i->pSeed) % 1000) < (int)pNewRoomEx->pLevel->pMaze->dwMerge)
							{
								nDir = DRLG_GetDirectionFromCoordinates(&i->pDrlgCoord, &pNewRoomEx->pDrlgCoord);
								if (nDir != DIRECTION_INVALID)
								{
									DRLGROOM_AllocDrlgOrthsForRooms(i, pNewRoomEx, nDir);
									DRLGMAZE_PickRoomPreset(i, 1);
								}
							}
						}
					}
				}
			}
		}
	}

	DRLGROOM_AddRoomExToLevel(pDrlgRoom->pLevel, pNewRoomEx);
	if (bMergeRooms)
	{
		DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
	}

	DRLGMAZE_PickRoomPreset(pNewRoomEx, 1);
	return pNewRoomEx;
}

//D2Common.0x6FD7A110
void __fastcall DRLGMAZE_InitBasicMazeLayout(D2DrlgLevelStrc* pLevel, int nRoomsPerDirection)
{
	D2DrlgRoomStrc* pFirstRoomEx = NULL;
	D2DrlgRoomStrc* pNewRoomEx = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;

	pFirstRoomEx = pLevel->pFirstRoomEx;
	pDrlgRoom = pLevel->pFirstRoomEx;

	for (int i = nRoomsPerDirection - 1; i > 0; --i)
	{
		pNewRoomEx = DRLGROOM_AllocRoomEx(pDrlgRoom->pLevel, DRLGTYPE_PRESET);
		pNewRoomEx->nTileWidth = pNewRoomEx->pLevel->pMaze->dwSizeX;
		pNewRoomEx->nTileHeight = pNewRoomEx->pLevel->pMaze->dwSizeY;

		if (DRLGMAZE_LinkMazeRooms(pNewRoomEx, pDrlgRoom, 1))
		{
			DRLGROOM_AllocDrlgOrthsForRooms(pDrlgRoom, pNewRoomEx, 1);
			DRLGMAZE_MergeMazeRooms(pNewRoomEx);
			DRLGROOM_AddRoomExToLevel(pDrlgRoom->pLevel, pNewRoomEx);
			DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
			DRLGMAZE_PickRoomPreset(pNewRoomEx, 1);
		}
		else
		{
			DRLGROOM_FreeRoomEx(pNewRoomEx);
			pNewRoomEx = nullptr;
		}
		D2_ASSERT(pDrlgRoom != nullptr); // Not in original game. Could be nullptr if fails to link.
		pDrlgRoom = pNewRoomEx;
	}

	D2_ASSERT(pDrlgRoom != nullptr); // Not in original game. Could be nullptr if the last fails to link.
	for (int i = nRoomsPerDirection - 1; i > 0; --i)
	{
		pNewRoomEx = DRLGROOM_AllocRoomEx(pDrlgRoom->pLevel, DRLGTYPE_PRESET);
		pNewRoomEx->nTileWidth = pNewRoomEx->pLevel->pMaze->dwSizeX;
		pNewRoomEx->nTileHeight = pNewRoomEx->pLevel->pMaze->dwSizeY;

		if (DRLGMAZE_LinkMazeRooms(pNewRoomEx, pDrlgRoom, 0))
		{
			DRLGROOM_AllocDrlgOrthsForRooms(pDrlgRoom, pNewRoomEx, 0);
			DRLGMAZE_MergeMazeRooms(pNewRoomEx);
			DRLGROOM_AddRoomExToLevel(pDrlgRoom->pLevel, pNewRoomEx);
			DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
			DRLGMAZE_PickRoomPreset(pNewRoomEx, 1);
		}
		else
		{
			DRLGROOM_FreeRoomEx(pNewRoomEx);
			pNewRoomEx = NULL;
		}
		pDrlgRoom = pNewRoomEx;
	}

	for (int i = nRoomsPerDirection - 1; i > 0; --i)
	{
		pNewRoomEx = DRLGROOM_AllocRoomEx(pDrlgRoom->pLevel, DRLGTYPE_PRESET);
		pNewRoomEx->nTileWidth = pNewRoomEx->pLevel->pMaze->dwSizeX;
		pNewRoomEx->nTileHeight = pNewRoomEx->pLevel->pMaze->dwSizeY;

		if (DRLGMAZE_LinkMazeRooms(pNewRoomEx, pDrlgRoom, 3))
		{
			DRLGROOM_AllocDrlgOrthsForRooms(pDrlgRoom, pNewRoomEx, 3);
			DRLGMAZE_MergeMazeRooms(pNewRoomEx);
			DRLGROOM_AddRoomExToLevel(pDrlgRoom->pLevel, pNewRoomEx);
			DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
			DRLGMAZE_PickRoomPreset(pNewRoomEx, 1);
		}
		else
		{
			DRLGROOM_FreeRoomEx(pNewRoomEx);
			pNewRoomEx = NULL;
		}
		pDrlgRoom = pNewRoomEx;
	}

	for (int i = nRoomsPerDirection - 2; i > 0; --i)
	{
		pNewRoomEx = DRLGROOM_AllocRoomEx(pDrlgRoom->pLevel, DRLGTYPE_PRESET);
		pNewRoomEx->nTileWidth = pNewRoomEx->pLevel->pMaze->dwSizeX;
		pNewRoomEx->nTileHeight = pNewRoomEx->pLevel->pMaze->dwSizeY;

		if (DRLGMAZE_LinkMazeRooms(pNewRoomEx, pDrlgRoom, 2))
		{
			DRLGROOM_AllocDrlgOrthsForRooms(pDrlgRoom, pNewRoomEx, 2);
			DRLGMAZE_MergeMazeRooms(pNewRoomEx);
			DRLGROOM_AddRoomExToLevel(pDrlgRoom->pLevel, pNewRoomEx);
			DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
			DRLGMAZE_PickRoomPreset(pNewRoomEx, 1);
		}
		else
		{
			DRLGROOM_FreeRoomEx(pNewRoomEx);
			pNewRoomEx = NULL;
		}
		pDrlgRoom = pNewRoomEx;
	}

	DRLGROOM_AllocDrlgOrthsForRooms(pDrlgRoom, pFirstRoomEx, 2);
	DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
	DRLGMAZE_PickRoomPreset(pFirstRoomEx, 1);
}

//D2Common.0x6FD7A340
BOOL __fastcall DRLGMAZE_LinkMazeRooms(D2DrlgRoomStrc* pDrlgRoom1, D2DrlgRoomStrc* pDrlgRoom2, int nDirection)
{
	switch (nDirection)
	{
	case 0:
		pDrlgRoom1->nTileXPos = pDrlgRoom2->nTileXPos - pDrlgRoom2->nTileWidth;
		pDrlgRoom1->nTileYPos = pDrlgRoom2->nTileYPos;
		break;

	case 1:
		pDrlgRoom1->nTileXPos = pDrlgRoom2->nTileXPos;
		pDrlgRoom1->nTileYPos = pDrlgRoom2->nTileYPos - pDrlgRoom2->nTileHeight;
		break;

	case 2:
		pDrlgRoom1->nTileXPos = pDrlgRoom2->nTileXPos + pDrlgRoom2->nTileWidth;
		pDrlgRoom1->nTileYPos = pDrlgRoom2->nTileYPos;
		break;

	case 3:
		pDrlgRoom1->nTileXPos = pDrlgRoom2->nTileXPos;
		pDrlgRoom1->nTileYPos = pDrlgRoom2->nTileYPos + pDrlgRoom2->nTileHeight;
		break;

	case 4:
		pDrlgRoom1->nTileXPos = pDrlgRoom2->nTileXPos - pDrlgRoom2->nTileWidth;
		pDrlgRoom1->nTileYPos = pDrlgRoom2->nTileYPos - pDrlgRoom2->nTileHeight;
		break;

	case 5:
		pDrlgRoom1->nTileXPos = pDrlgRoom2->nTileXPos + pDrlgRoom2->nTileWidth;
		pDrlgRoom1->nTileYPos = pDrlgRoom2->nTileYPos - pDrlgRoom2->nTileHeight;
		break;

	case 6:
		pDrlgRoom1->nTileXPos = pDrlgRoom2->nTileXPos + pDrlgRoom2->nTileWidth;
		pDrlgRoom1->nTileYPos = pDrlgRoom2->nTileYPos + pDrlgRoom2->nTileHeight;
		break;

	case 7:
		pDrlgRoom1->nTileXPos = pDrlgRoom2->nTileXPos - pDrlgRoom2->nTileWidth;
		pDrlgRoom1->nTileYPos = pDrlgRoom2->nTileYPos + pDrlgRoom2->nTileHeight;
		break;

	default:
		break;
	}
	
	for (D2DrlgOrthStrc* pDrlgOrth = pDrlgRoom2->pDrlgOrth; pDrlgOrth; pDrlgOrth = pDrlgOrth->pNext)
	{
		if (!DRLG_ComputeRectanglesManhattanDistance(&pDrlgRoom1->pDrlgCoord, pDrlgOrth->pBox, 0))
		{
			return FALSE;
		}
	}

	return DRLGMAZE_CheckRoomNotOverlaping(pDrlgRoom1->pLevel, pDrlgRoom1, pDrlgRoom2, 0);
}

//D2Common.0x6FD7A450
void __fastcall DRLGMAZE_MergeMazeRooms(D2DrlgRoomStrc* pDrlgRoom)
{
	D2DrlgOrthStrc* pDrlgOrth = NULL;
	int nDirection = 0;
	int nX = 0;
	int nY = 0;

	if (!DRLGMAZE_HasMapDS1(pDrlgRoom))
	{
		for (D2DrlgRoomStrc* i = pDrlgRoom->pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			if (i != pDrlgRoom && !DRLGMAZE_HasMapDS1(i))
			{
				nX = 0;
				nY = 0;

				if (!sub_6FD77740(&pDrlgRoom->pDrlgCoord, &i->pDrlgCoord, 1, &nX, &nY))
				{
					if (nX != nY)
					{
						pDrlgOrth = pDrlgRoom->pDrlgOrth;
						while (pDrlgOrth)
						{
							if (pDrlgOrth->pDrlgRoom == i)
							{
								break;
							}

							pDrlgOrth = pDrlgOrth->pNext;
						}

						if (!pDrlgOrth && (int)((unsigned int)SEED_RollRandomNumber(&i->pSeed) % 1000) < (int)pDrlgRoom->pLevel->pMaze->dwMerge)
						{
							nDirection = DRLG_GetDirectionFromCoordinates(&i->pDrlgCoord, &pDrlgRoom->pDrlgCoord);
							if (nDirection != DIRECTION_INVALID)
							{
								DRLGROOM_AllocDrlgOrthsForRooms(i, pDrlgRoom, nDirection);
								DRLGMAZE_PickRoomPreset(i, 1);
							}
						}
					}
				}
			}
		}
	}
}

//D2Common.0x6FD7A570
D2DrlgRoomStrc* __fastcall DRLGMAZE_GetRandomRoomExFromLevel(D2DrlgLevelStrc* pLevel)
{
	D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx;

	for (int i = SEED_RollLimitedRandomNumber(&pLevel->pSeed, pLevel->nRooms); i; --i)
	{
		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}

	return pDrlgRoom;
}

//D2Common.0x6FD7A5D0
void __fastcall DRLGMAZE_BuildBasicMaze(D2DrlgLevelStrc* pLevel)
{
	D2DrlgRoomStrc* pRandomRoomEx = NULL;
	D2DrlgRoomStrc* pNewRoomEx = NULL;
	D2DrlgOrthStrc* pDrlgOrth = NULL;
	int nDirection = 0;
	int nRooms = 0;

	nRooms = pLevel->pMaze->dwRooms[pLevel->pDrlg->nDifficulty];

	if (pLevel->nLevelId == pLevel->pDrlg->nStaffTombLevel)
	{
		nRooms *= 3;
	}

	if (pLevel->nLevelId == pLevel->pDrlg->nBossTombLevel)
	{
		nRooms *= 2;
	}

	for (int i = pLevel->nRooms; i < nRooms; i = pLevel->nRooms)
	{
		pRandomRoomEx = DRLGMAZE_GetRandomRoomExFromLevel(pLevel);

		nDirection = SEED_RollRandomNumber(&pRandomRoomEx->pSeed) & 3;

		if (!DRLGMAZE_HasMapDS1(pRandomRoomEx))
		{
			pNewRoomEx = DRLGROOM_AllocRoomEx(pRandomRoomEx->pLevel, DRLGTYPE_PRESET);
			pNewRoomEx->nTileWidth = pNewRoomEx->pLevel->pMaze->dwSizeX;
			pNewRoomEx->nTileHeight = pNewRoomEx->pLevel->pMaze->dwSizeY;

			switch (nDirection)
			{
			case 0:
				pNewRoomEx->nTileXPos = pRandomRoomEx->nTileXPos - pRandomRoomEx->nTileWidth;
				pNewRoomEx->nTileYPos = pRandomRoomEx->nTileYPos;
				break;

			case 1:
				pNewRoomEx->nTileXPos = pRandomRoomEx->nTileXPos;
				pNewRoomEx->nTileYPos = pRandomRoomEx->nTileYPos - pRandomRoomEx->nTileHeight;
				break;

			case 2:
				pNewRoomEx->nTileXPos = pRandomRoomEx->nTileXPos + pRandomRoomEx->nTileWidth;
				pNewRoomEx->nTileYPos = pRandomRoomEx->nTileYPos;
				break;

			case 3:
				pNewRoomEx->nTileXPos = pRandomRoomEx->nTileXPos;
				pNewRoomEx->nTileYPos = pRandomRoomEx->nTileYPos + pRandomRoomEx->nTileHeight;
				break;

			case 4:
				pNewRoomEx->nTileXPos = pRandomRoomEx->nTileXPos - pRandomRoomEx->nTileWidth;
				pNewRoomEx->nTileYPos = pRandomRoomEx->nTileYPos - pRandomRoomEx->nTileHeight;
				break;

			case 5:
				pNewRoomEx->nTileXPos = pRandomRoomEx->nTileXPos + pRandomRoomEx->nTileWidth;
				pNewRoomEx->nTileYPos = pRandomRoomEx->nTileYPos - pRandomRoomEx->nTileHeight;
				break;

			case 6:
				pNewRoomEx->nTileXPos = pRandomRoomEx->nTileXPos + pRandomRoomEx->nTileWidth;
				pNewRoomEx->nTileYPos = pRandomRoomEx->nTileYPos + pRandomRoomEx->nTileHeight;
				break;

			case 7:
				pNewRoomEx->nTileXPos = pRandomRoomEx->nTileXPos - pRandomRoomEx->nTileWidth;
				pNewRoomEx->nTileYPos = pRandomRoomEx->nTileYPos + pRandomRoomEx->nTileHeight;
				break;

			default:
				break;
			}

			pDrlgOrth = pRandomRoomEx->pDrlgOrth;
			while (pDrlgOrth && DRLG_ComputeRectanglesManhattanDistance(&pNewRoomEx->pDrlgCoord, pDrlgOrth->pBox, 0))
			{
				pDrlgOrth = pDrlgOrth->pNext;
			}

			if (!pDrlgOrth)
			{
				if (DRLGMAZE_CheckRoomNotOverlaping(pNewRoomEx->pLevel, pNewRoomEx, pRandomRoomEx, 0))
				{
					DRLGROOM_AllocDrlgOrthsForRooms(pRandomRoomEx, pNewRoomEx, nDirection);
					DRLGMAZE_MergeMazeRooms(pNewRoomEx);
					DRLGROOM_AddRoomExToLevel(pRandomRoomEx->pLevel, pNewRoomEx);
					DRLGMAZE_PickRoomPreset(pRandomRoomEx, 1);
					DRLGMAZE_PickRoomPreset(pNewRoomEx, 1);
				}
				else
				{
					DRLGROOM_FreeRoomEx(pNewRoomEx);
				}
			}
			else
			{
				DRLGROOM_FreeRoomEx(pNewRoomEx);
			}
		}
	}
}

static const int dword_6FDCE850[][3] =
{
	{ LVLPREST_ACT5_LAVA_S, 1, 0 },
	{ LVLPREST_ACT5_LAVA_N, 3, 1 },
	{ LVLPREST_ACT5_LAVA_S, 1, 1 },
	{ LVLPREST_ACT5_LAVA_N, 3, 0 },
	{ LVLPREST_ACT5_LAVA_E, 0, 1 },
	{ LVLPREST_ACT5_LAVA_W, 2, 0 },
	{ LVLPREST_ACT5_LAVA_E, 0, 0 },
	{ LVLPREST_ACT5_LAVA_W, 2, 1 },
};

static void PlaceLavaPreset(D2DrlgRoomStrc* pFirstRoomEx, int nSet)
{

	int nDirection = dword_6FDCE850[nSet][1];

	D2DrlgRoomStrc* pNewRoomEx = DRLGROOM_AllocRoomEx(pFirstRoomEx->pLevel, DRLGTYPE_PRESET);
	pNewRoomEx->nTileWidth = pNewRoomEx->pLevel->pMaze->dwSizeX;
	pNewRoomEx->nTileHeight = pNewRoomEx->pLevel->pMaze->dwSizeY;

	if (DRLGMAZE_LinkMazeRooms(pNewRoomEx, pFirstRoomEx, nDirection))
	{
		DRLGROOM_AllocDrlgOrthsForRooms(pFirstRoomEx, pNewRoomEx, nDirection);
		DRLGROOM_AddRoomExToLevel(pFirstRoomEx->pLevel, pNewRoomEx);
		DRLGMAZE_PickRoomPreset(pFirstRoomEx, 1);

		pNewRoomEx->pMaze->nLevelPrest = dword_6FDCE850[nSet][0];
		pNewRoomEx->pMaze->nPickedFile = dword_6FDCE850[nSet][2];
		pNewRoomEx->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
	}
	else
	{
		DRLGROOM_FreeRoomEx(pNewRoomEx);
	}
}

//D2Common.0x6FD7A830
void __fastcall DRLGMAZE_PlaceAct5LavaPresets(D2DrlgLevelStrc* pLevel)
{		
	const int nSet = 2 * (SEED_RollRandomNumber(&pLevel->pFirstRoomEx->pSeed) & 3);
	
	const int scnLavaCombinations = ARRAY_SIZE(dword_6FDCE850);
	D2_ASSERT(nSet < (scnLavaCombinations - 1));

	// Note: pLevel->pFirstRoomEx may change when calling PlaceLavaPreset, using the same room for both presets is correct here.
	D2DrlgRoomStrc* pLevelFirstRoom = pLevel->pFirstRoomEx;
	PlaceLavaPreset(pLevelFirstRoom, nSet);
	PlaceLavaPreset(pLevelFirstRoom, nSet+1);

	DRLGMAZE_FillBlankMazeSpaces(pLevel, LVLPREST_ACT4_LAVA_X, nullptr);
}

//D2Common.0x6FD7A9B0
void __fastcall DRLGMAZE_FillBlankMazeSpaces(D2DrlgLevelStrc* pLevel, int nLevelPrest, D2DrlgRoomStrc* pIgnoreRoomEx)
{
	const int nRooms = pLevel->nRooms;
	if (nRooms > 0)
	{
		D2DrlgRoomStrc** pDrlgRoomArray = (D2DrlgRoomStrc**)D2_ALLOC_POOL(pLevel->pDrlg->pMempool, sizeof(D2DrlgRoomStrc*) * nRooms);

		// Copy rooms pointers
		{
			D2DrlgRoomStrc* pTemp = pLevel->pFirstRoomEx;
			for (int i = 0; i < nRooms; ++i)
			{
				pDrlgRoomArray[i] = pTemp;
				pTemp = pTemp->pDrlgRoomNext;
			}
		}

		for (int i = 0; i < nRooms; ++i)
		{
			D2DrlgRoomStrc* pCurrentRoomEx = pDrlgRoomArray[i];
			if (pCurrentRoomEx != pIgnoreRoomEx)
			{
				for (int j = 0; j < 8; ++j)
				{
					D2DrlgRoomStrc* pNewRoomEx = DRLGROOM_AllocRoomEx(pCurrentRoomEx->pLevel, DRLGTYPE_PRESET);
					pNewRoomEx->nTileWidth = pNewRoomEx->pLevel->pMaze->dwSizeX;
					pNewRoomEx->nTileHeight = pNewRoomEx->pLevel->pMaze->dwSizeY;

					if (DRLGMAZE_LinkMazeRooms(pNewRoomEx, pCurrentRoomEx, j))
					{
						DRLGROOM_AllocDrlgOrthsForRooms(pCurrentRoomEx, pNewRoomEx, j);
						DRLGROOM_AddRoomExToLevel(pCurrentRoomEx->pLevel, pNewRoomEx);
						DRLGMAZE_SetPickedFileAndPresetId(pNewRoomEx, nLevelPrest, -1, FALSE);
					}
					else
					{
						DRLGROOM_FreeRoomEx(pNewRoomEx);
					}
				}
			}
		}
		
		D2_FREE_POOL(pLevel->pDrlg->pMempool, pDrlgRoomArray);
	}
}

//D2Common.0x6FD7AAC0
void __fastcall DRLGMAZE_PlaceAct2TombPrev_Act5BaalPrev(D2DrlgLevelStrc* pLevel)
{
	static const int dword_6FDCE8B4[][2] =
	{
		{ LVLPREST_ACT2_TOMB_PREV_NSE, LVLPREST_ACT5_BAAL_PREV_NSE },
		{ LVLPREST_ACT2_TOMB_PREV_SEW, LVLPREST_ACT5_BAAL_PREV_SEW },
		{ LVLPREST_ACT2_TOMB_PREV_NSW, LVLPREST_ACT5_BAAL_PREV_NSW },
		{ LVLPREST_ACT2_TOMB_PREV_NEW, LVLPREST_ACT5_BAAL_PREV_NEW },
	};

	// Warning: pLevel->pFirstRoomEx may change, so keep a copy of the pointer to the room we are using
	D2DrlgRoomStrc* pLevelFirstRoomEx = pLevel->pFirstRoomEx;

	int nDirection = SEED_RollRandomNumber(&pLevel->pSeed) & 3;
	for (int i = 0; i < 3; ++i)
	{
		D2DrlgRoomStrc* pDrlgRoom = DRLGROOM_AllocRoomEx(pLevelFirstRoomEx->pLevel, DRLGTYPE_PRESET);

		DRLGMAZE_SetRoomSize(pDrlgRoom);

		if (DRLGMAZE_LinkMazeRooms(pDrlgRoom, pLevelFirstRoomEx, nDirection))
		{
			DRLGROOM_AllocDrlgOrthsForRooms(pLevelFirstRoomEx, pDrlgRoom, nDirection);
			DRLGMAZE_MergeMazeRooms(pDrlgRoom);
			DRLGROOM_AddRoomExToLevel(pLevelFirstRoomEx->pLevel, pDrlgRoom);
			DRLGMAZE_PickRoomPreset(pLevelFirstRoomEx, 1);
			DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
		}
		else
		{
			DRLGROOM_FreeRoomEx(pDrlgRoom);
		}

		nDirection = (nDirection + 1) % 4;
	}

	if (pLevel->nLevelType == LVLTYPE_ACT2_TOMB)
	{
		pLevelFirstRoomEx->pMaze->nLevelPrest = dword_6FDCE8B4[nDirection][0];
		pLevelFirstRoomEx->pMaze->nPickedFile = -1;
		pLevelFirstRoomEx->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
	}
	else if (pLevel->nLevelType == LVLTYPE_ACT5_BAAL)
	{
		pLevelFirstRoomEx->pMaze->nLevelPrest = dword_6FDCE8B4[nDirection][1];
		pLevelFirstRoomEx->pMaze->nPickedFile = -1;
		pLevelFirstRoomEx->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
	}
}

//Helper function
static D2DrlgRoomStrc* DRLGMAZE_PlaceRoomForArcaneBranch(D2DrlgRoomStrc* pParentRoom, int nArcaneMapBranch)
{
	if (D2DrlgRoomStrc* pDrlgRoom = DRLGMAZE_PlaceAdjacentPresetRoom(pParentRoom, nArcaneMapBranch % 4, TRUE))
	{
		DRLGMAZE_PickRoomPreset(pParentRoom, 1);
		DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
		return pDrlgRoom;
	}
	return nullptr;
}

/*
Order of generation of the arcane sanctuary rooms for a given branch:

                 2 →  3 →  4 →  5 →  6
                 ↑                   ↓
         W → 0 → 1        12         7 →  8
                           ↑         ↓
                14 ← 13 ← 11 ← 10  ← 9
*/
// Helper function
static int DRLGMAZE_ArcaneSanctuaryDirectionFromRoomIdx(int nBranchDirection, int nBranchRoomIdx)
{
	switch (nBranchRoomIdx)
	{
	default:
		return nBranchDirection;     // Forward when facing branch direction
	case 2:
	case 12:
		return nBranchDirection + 3; // Left when facing branch direction
	case 7:
	case 9:
		return nBranchDirection + 1; // Right when facing branch direction
	case 10:
	case 11:
	case 13:
	case 14:
		return nBranchDirection + 2; // Backwards when facing branch direction
	}
}

//D2Common.0x6FD7ABC0
// This function has been rewritten using a loop to avoid having macros / 15 calls to the same functions.
void __fastcall DRLGMAZE_PlaceArcaneSanctuary(D2DrlgLevelStrc* pLevel)
{
	D2DrlgRoomStrc* pLevelFirstRoomEx = pLevel->pFirstRoomEx;
	
	const int nRand = SEED_RollRandomNumber(&pLevel->pSeed) & 3;

	static const int nRoomsPerBranch = 15;
	static const int nBranchDirections = 4;
	D2DrlgRoomStrc* pDrlgRoomArray[nRoomsPerBranch * nBranchDirections] = {};

	for (int nBranchDirection = 0; nBranchDirection < nBranchDirections; ++nBranchDirection)
	{
		D2DrlgRoomStrc* pParentRoom = pLevelFirstRoomEx;
		for (int nBranchRoomIdx = 0; nBranchRoomIdx < nRoomsPerBranch; nBranchRoomIdx++)
		{
			const int nRoomDirection = DRLGMAZE_ArcaneSanctuaryDirectionFromRoomIdx(nBranchDirection, nBranchRoomIdx);
			D2DrlgRoomStrc* pNewRoom = DRLGMAZE_PlaceRoomForArcaneBranch(pParentRoom, nRoomDirection);
			// See diagram above, those rooms do not have "child" rooms. So the next room will start from current parent.
			// The game also didn't keep it in the room array, which means its pMaze->nPickedFile will be left untouched. Is this a mistake ?
			// In any case we reproduce the same behaviour here
			if (nBranchRoomIdx != 8 && nBranchRoomIdx != 12)
			{
				pDrlgRoomArray[nBranchRoomIdx + nBranchDirection * nRoomsPerBranch] = pNewRoom;
				pParentRoom = pNewRoom;
			}
		}
	}

	for (int i = 0; i < ARRAY_SIZE(pDrlgRoomArray); ++i)
	{
		if (pDrlgRoomArray[i])
		{
			pDrlgRoomArray[i]->pMaze->nPickedFile = (nRand + i / 15) % 4;
		}
	}

	pLevelFirstRoomEx->pMaze->nPickedFile = 4;
}

//D2Common.0x6FD7AFD0
D2DrlgRoomStrc* __fastcall DRLGMAZE_PlaceAdjacentPresetRoom(D2DrlgRoomStrc* pParentRoomEx, int nDirection, BOOL bMergeRooms)
{
	D2DrlgRoomStrc* pNewRoomEx = DRLGROOM_AllocRoomEx(pParentRoomEx->pLevel, DRLGTYPE_PRESET);
	pNewRoomEx->nTileWidth = pNewRoomEx->pLevel->pMaze->dwSizeX;
	pNewRoomEx->nTileHeight = pNewRoomEx->pLevel->pMaze->dwSizeY;

	switch (nDirection)
	{
	case ALTDIR_WEST:
		pNewRoomEx->nTileXPos = pParentRoomEx->nTileXPos - pParentRoomEx->nTileWidth;
		pNewRoomEx->nTileYPos = pParentRoomEx->nTileYPos;
		break;

	case ALTDIR_NORTH:
		pNewRoomEx->nTileXPos = pParentRoomEx->nTileXPos;
		pNewRoomEx->nTileYPos = pParentRoomEx->nTileYPos - pParentRoomEx->nTileHeight;
		break;

	case ALTDIR_EAST:
		pNewRoomEx->nTileXPos = pParentRoomEx->nTileXPos + pParentRoomEx->nTileWidth;
		pNewRoomEx->nTileYPos = pParentRoomEx->nTileYPos;
		break;

	case ALTDIR_SOUTH:
		pNewRoomEx->nTileXPos = pParentRoomEx->nTileXPos;
		pNewRoomEx->nTileYPos = pParentRoomEx->nTileYPos + pParentRoomEx->nTileHeight;
		break;

	case ALTDIR_NORTHWEST:
		pNewRoomEx->nTileXPos = pParentRoomEx->nTileXPos - pParentRoomEx->nTileWidth;
		pNewRoomEx->nTileYPos = pParentRoomEx->nTileYPos - pParentRoomEx->nTileHeight;
		break;

	case ALTDIR_NORTHEAST:
		pNewRoomEx->nTileXPos = pParentRoomEx->nTileXPos + pParentRoomEx->nTileWidth;
		pNewRoomEx->nTileYPos = pParentRoomEx->nTileYPos - pParentRoomEx->nTileHeight;
		break;

	case ALTDIR_SOUTHEAST:
		pNewRoomEx->nTileXPos = pParentRoomEx->nTileXPos + pParentRoomEx->nTileWidth;
		pNewRoomEx->nTileYPos = pParentRoomEx->nTileYPos + pParentRoomEx->nTileHeight;
		break;

	case ALTDIR_SOUTHWEST:
		pNewRoomEx->nTileXPos = pParentRoomEx->nTileXPos - pParentRoomEx->nTileWidth;
		pNewRoomEx->nTileYPos = pParentRoomEx->nTileYPos + pParentRoomEx->nTileHeight;
		break;

	default:
		break;
	}

	if(DRLGMAZE_CheckIfRoomOverlapsAythingOtherThanParent(pNewRoomEx, pParentRoomEx))
	{
		DRLGROOM_FreeRoomEx(pNewRoomEx);
		return nullptr;
	}

	DRLGROOM_AllocDrlgOrthsForRooms(pParentRoomEx, pNewRoomEx, nDirection);

	if (bMergeRooms)
	{
		DRLGMAZE_MergeMazeRooms(pNewRoomEx);
	}

	DRLGROOM_AddRoomExToLevel(pParentRoomEx->pLevel, pNewRoomEx);
	return pNewRoomEx;
}

//D2Common.0x6FD7B230
void __fastcall DRLGMAZE_ScanReplaceSpecialPreset(D2DrlgLevelStrc* pLevel, const D2MazeLevelIdStrc* pMazeInit, int* pRand)
{
	D2DrlgRoomStrc* pNewRoomEx = NULL;

	for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
	{
		if (!DRLGMAZE_HasMapDS1(i) && i->pMaze->nLevelPrest == pMazeInit->nLevelPrestId1)
		{
			i->pMaze->nLevelPrest = pMazeInit->nLevelPrestId2;
			i->pMaze->nPickedFile = pMazeInit->nPickedFile;
			i->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
			if (pRand)
			{
				*pRand = (*pRand + 1) % 4;
			}
			return;
		}
	}

	for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
	{
		if (!DRLGMAZE_HasMapDS1(i))
		{
			pNewRoomEx = DRLGROOM_AllocRoomEx(i->pLevel, DRLGTYPE_PRESET);

			pNewRoomEx->nTileWidth = pNewRoomEx->pLevel->pMaze->dwSizeX;
			pNewRoomEx->nTileHeight = pNewRoomEx->pLevel->pMaze->dwSizeY;

			if (!DRLGMAZE_LinkMazeRooms(pNewRoomEx, i, pMazeInit->nDirection))
			{
				DRLGROOM_FreeRoomEx(pNewRoomEx);
			}
			else
			{
				DRLGROOM_AllocDrlgOrthsForRooms(i, pNewRoomEx, pMazeInit->nDirection);
				DRLGROOM_AddRoomExToLevel(i->pLevel, pNewRoomEx);

				DRLGMAZE_PickRoomPreset(i, 1);

				pNewRoomEx->pMaze->nLevelPrest = pMazeInit->nLevelPrestId2;
				pNewRoomEx->pMaze->nPickedFile = pMazeInit->nPickedFile;
				pNewRoomEx->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
				break;
			}
		}
	}

	if (pRand)
	{
		*pRand = (*pRand + 1) % 4;
	}
}

//D2Common.0x6FD7B330
void __fastcall DRLGMAZE_ScanReplaceSpecialAct2SewersPresets(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct2SewerPrevIds[] =
	{
		{ LVLPREST_ACT2_SEWER_N, LVLPREST_ACT2_SEWER_PREV_N, -1, 3 },
		{ LVLPREST_ACT2_SEWER_E, LVLPREST_ACT2_SEWER_PREV_E, -1, 0 },
		{ LVLPREST_ACT2_SEWER_S, LVLPREST_ACT2_SEWER_PREV_S, -1, 1 },
		{ LVLPREST_ACT2_SEWER_W, LVLPREST_ACT2_SEWER_PREV_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2SewerNextIds[] =
	{
		{ LVLPREST_ACT2_SEWER_N, LVLPREST_ACT2_SEWER_NEXT_N, -1, 3 },
		{ LVLPREST_ACT2_SEWER_E, LVLPREST_ACT2_SEWER_NEXT_E, -1, 0 },
		{ LVLPREST_ACT2_SEWER_S, LVLPREST_ACT2_SEWER_NEXT_S, -1, 1 },
		{ LVLPREST_ACT2_SEWER_W, LVLPREST_ACT2_SEWER_NEXT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2SewerWaypointIds[] =
	{
		{ LVLPREST_ACT2_SEWER_N, LVLPREST_ACT2_SEWER_WAYPOINT_N, -1, 3 },
		{ LVLPREST_ACT2_SEWER_E, LVLPREST_ACT2_SEWER_WAYPOINT_E, -1, 0 },
		{ LVLPREST_ACT2_SEWER_S, LVLPREST_ACT2_SEWER_WAYPOINT_S, -1, 1 },
		{ LVLPREST_ACT2_SEWER_W, LVLPREST_ACT2_SEWER_WAYPOINT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2SewerRadamentIds[] =
	{
		{ LVLPREST_ACT2_SEWER_N, LVLPREST_ACT2_SEWER_RADAMENTS_LAIR_N, -1, 3 },
		{ LVLPREST_ACT2_SEWER_E, LVLPREST_ACT2_SEWER_RADAMENTS_LAIR_E, -1, 0 },
		{ LVLPREST_ACT2_SEWER_S, LVLPREST_ACT2_SEWER_RADAMENTS_LAIR_S, -1, 1 },
		{ LVLPREST_ACT2_SEWER_W, LVLPREST_ACT2_SEWER_RADAMENTS_LAIR_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2SewerChestIds[] =
	{
		{ LVLPREST_ACT2_SEWER_N, LVLPREST_ACT2_SEWER_CHEST_N, -1, 3 },
		{ LVLPREST_ACT2_SEWER_E, LVLPREST_ACT2_SEWER_CHEST_E, -1, 0 },
		{ LVLPREST_ACT2_SEWER_S, LVLPREST_ACT2_SEWER_CHEST_S, -1, 1 },
		{ LVLPREST_ACT2_SEWER_W, LVLPREST_ACT2_SEWER_CHEST_W, -1, 2 },
	};

	const D2MazeLevelIdStrc* pMazeLevelIds = NULL;
	D2DrlgRoomStrc* pDrlgRoom1 = NULL;
	D2DrlgRoomStrc* pDrlgRoom2 = NULL;
	D2DrlgRoomStrc* pDrlgRoom3 = NULL;
	D2DrlgRoomStrc* pDrlgRoom4 = NULL;
	D2DrlgRoomStrc* pDrlgRoom5 = NULL;
	int nDirection = 0;
	int nRand = 0;

	nDirection = (2 * (SEED_RollRandomNumber(&pLevel->pSeed) & 1)) | 1;
	nRand = SEED_RollRandomNumber(&pLevel->pSeed) & 3;

	switch (pLevel->nLevelId)
	{
	case LEVEL_SEWERSLEV1:
		pDrlgRoom1 = NULL;
		for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			if ((!pDrlgRoom1 || i->nTileYPos < pDrlgRoom1->nTileYPos) && DRLGMAZE_CheckIfMayPlaceAdjacentPresetRoom(i, ALTDIR_NORTH))
			{
				pDrlgRoom1 = i;
			}
		}

		pDrlgRoom2 = DRLGMAZE_PlaceAdjacentPresetRoom(pDrlgRoom1, ALTDIR_NORTH, TRUE);
		if (pDrlgRoom2)
		{
			DRLGMAZE_PickRoomPreset(pDrlgRoom1, 1);
			DRLGMAZE_PickRoomPreset(pDrlgRoom2, 1);
		}

		pDrlgRoom3 = DRLGMAZE_PlaceAdjacentPresetRoom(pDrlgRoom2, ALTDIR_NORTH, TRUE);
		if (pDrlgRoom3)
		{
			DRLGMAZE_PickRoomPreset(pDrlgRoom2, 1);
			DRLGMAZE_PickRoomPreset(pDrlgRoom3, 1);
		}

		pDrlgRoom4 = DRLGMAZE_PlaceAdjacentPresetRoom(pDrlgRoom3, ALTDIR_WEST, FALSE);
		if (pDrlgRoom4)
		{
			DRLGMAZE_PickRoomPreset(pDrlgRoom3, 1);
			DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom4, LVLPREST_ACT2_SEWER_PREV_E, 0, FALSE);
		}

		pDrlgRoom1 = NULL;
		for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			if ((!pDrlgRoom1 || i->nTileXPos > pDrlgRoom1->nTileXPos) && DRLGMAZE_CheckIfMayPlaceAdjacentPresetRoom(i, ALTDIR_EAST))
			{
				pDrlgRoom1 = i;
			}
		}

		pDrlgRoom2 = DRLGMAZE_PlaceAdjacentPresetRoom(pDrlgRoom1, ALTDIR_EAST, TRUE);
		if (pDrlgRoom2)
		{
			DRLGMAZE_PickRoomPreset(pDrlgRoom1, 1);
			DRLGMAZE_PickRoomPreset(pDrlgRoom2, 1);
		}

		pDrlgRoom3 = DRLGMAZE_PlaceAdjacentPresetRoom(pDrlgRoom2, ALTDIR_EAST, TRUE);
		if (pDrlgRoom3)
		{
			DRLGMAZE_PickRoomPreset(pDrlgRoom2, 1);
			DRLGMAZE_PickRoomPreset(pDrlgRoom3, 1);
		}

		pDrlgRoom4 = DRLGMAZE_PlaceAdjacentPresetRoom(pDrlgRoom3, nDirection, FALSE);
		if (pDrlgRoom4)
		{
			DRLGMAZE_PickRoomPreset(pDrlgRoom3, 1);
			DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom4, LVLPREST_ACT2_SEWER_PREV_NS, 0, FALSE);
		}

		pDrlgRoom5 = DRLGMAZE_PlaceAdjacentPresetRoom(pDrlgRoom4, nDirection, FALSE);
		if (pDrlgRoom5)
		{
			DRLGMAZE_PickRoomPreset(pDrlgRoom5, 1);
		}

		pMazeLevelIds = &nAct2SewerNextIds[nRand];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}
		break;

	case LEVEL_SEWERSLEV2:
		pMazeLevelIds = &nAct2SewerPrevIds[nRand];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		nRand = (nRand + 1) % 4;
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct2SewerWaypointIds[nRand], &nRand);
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct2SewerNextIds[nRand], &nRand);
		break;

	case LEVEL_SEWERSLEV3:
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct2SewerPrevIds[nRand], &nRand);
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct2SewerRadamentIds[nRand], &nRand);
		break;

	case LEVEL_ANCIENTTUNNELS:
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct2SewerPrevIds[nRand], &nRand);
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct2SewerChestIds[nRand], &nRand);
		break;

	default:
		return;
	}
}

//D2Common.0x6FD7B660
void __fastcall DRLGMAZE_AddSpecialPreset(D2DrlgLevelStrc* pLevel, int nDirection, int nLvlPrestId, int nFile)
{
	for (D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom))
		{
			if (DRLGMAZE_InitRoomFixedPreset(pDrlgRoom, nDirection, nLvlPrestId, nFile, TRUE))
			{
				break;
			}
		}
	}
}

//D2Common.0x6FD7B710
BOOL __fastcall DRLGMAZE_CheckIfMayPlaceAdjacentPresetRoom(D2DrlgRoomStrc* pDrlgRoom, int nDirection)
{
	if (DRLGMAZE_HasMapDS1(pDrlgRoom))
	{
		return FALSE;
	}

	// Check if there is already a Drlg Orth matching direction
	if(DRLGMAZE_OrthWithDirectionExists(pDrlgRoom, nDirection))
	{
		return FALSE;
	}

	D2DrlgRoomStrc* pNewRoomEx = DRLGMAZE_PlaceAdjacentPresetRoom(pDrlgRoom, nDirection, FALSE);

	if (pNewRoomEx)
	{
		DRLGMAZE_PickRoomPreset(pNewRoomEx, TRUE);
		DRLGROOM_FreeRoomEx(pNewRoomEx);
	}

	return pNewRoomEx != 0;
}

//D2Common.0x6FD7B8B0
void __fastcall DRLGMAZE_PlaceAct2TombStuff(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct2TombNextIds[] =
	{
		{ LVLPREST_ACT2_TOMB_N, LVLPREST_ACT2_TOMB_NEXT_N, -1, 3 },
		{ LVLPREST_ACT2_TOMB_E, LVLPREST_ACT2_TOMB_NEXT_E, -1, 0 },
		{ LVLPREST_ACT2_TOMB_S, LVLPREST_ACT2_TOMB_NEXT_S, -1, 1 },
		{ LVLPREST_ACT2_TOMB_W, LVLPREST_ACT2_TOMB_NEXT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2TombWaypointIds[] =
	{
		{ LVLPREST_ACT2_TOMB_N, LVLPREST_ACT2_TOMB_WAYPOINT_N, -1, 3 },
		{ LVLPREST_ACT2_TOMB_E, LVLPREST_ACT2_TOMB_WAYPOINT_E, -1, 0 },
		{ LVLPREST_ACT2_TOMB_S, LVLPREST_ACT2_TOMB_WAYPOINT_S, -1, 1 },
		{ LVLPREST_ACT2_TOMB_W, LVLPREST_ACT2_TOMB_WAYPOINT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2TombChestIds[] =
	{
		{ LVLPREST_ACT2_TOMB_N, LVLPREST_ACT2_TOMB_CHEST_N, -1, 3 },
		{ LVLPREST_ACT2_TOMB_E, LVLPREST_ACT2_TOMB_CHEST_E, -1, 0 },
		{ LVLPREST_ACT2_TOMB_S, LVLPREST_ACT2_TOMB_CHEST_S, -1, 1 },
		{ LVLPREST_ACT2_TOMB_W, LVLPREST_ACT2_TOMB_CHEST_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2TombLeatherarmIds[] =
	{
		{ LVLPREST_ACT2_TOMB_N, LVLPREST_ACT2_TOMB_LEATHERARM_N, -1, 3 },
		{ LVLPREST_ACT2_TOMB_E, LVLPREST_ACT2_TOMB_LEATHERARM_E, -1, 0 },
		{ LVLPREST_ACT2_TOMB_S, LVLPREST_ACT2_TOMB_LEATHERARM_S, -1, 1 },
		{ LVLPREST_ACT2_TOMB_W, LVLPREST_ACT2_TOMB_LEATHERARM_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2TombCubeIds[] =
	{
		{ LVLPREST_ACT2_TOMB_N, LVLPREST_ACT2_TOMB_CUBE_N, -1, 3 },
		{ LVLPREST_ACT2_TOMB_E, LVLPREST_ACT2_TOMB_CUBE_E, -1, 0 },
		{ LVLPREST_ACT2_TOMB_S, LVLPREST_ACT2_TOMB_CUBE_S, -1, 1 },
		{ LVLPREST_ACT2_TOMB_W, LVLPREST_ACT2_TOMB_CUBE_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2TombTreasureIds[] =
	{
		{ LVLPREST_ACT2_TOMB_N, LVLPREST_ACT2_TOMB_TREASURE_N, -1, 3 },
		{ LVLPREST_ACT2_TOMB_E, LVLPREST_ACT2_TOMB_TREASURE_E, -1, 0 },
		{ LVLPREST_ACT2_TOMB_S, LVLPREST_ACT2_TOMB_TREASURE_S, -1, 1 },
		{ LVLPREST_ACT2_TOMB_W, LVLPREST_ACT2_TOMB_TREASURE_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2TombTalRashaIds[] =
	{
		{ LVLPREST_ACT2_TOMB_N, LVLPREST_ACT2_TOMB_TALRASHA_N, -1, 3 },
		{ LVLPREST_ACT2_TOMB_E, LVLPREST_ACT2_TOMB_TALRASHA_E, -1, 0 },
		{ LVLPREST_ACT2_TOMB_S, LVLPREST_ACT2_TOMB_TALRASHA_S, -1, 1 },
		{ LVLPREST_ACT2_TOMB_W, LVLPREST_ACT2_TOMB_TALRASHA_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2TombKaaIds[] =
	{
		{ LVLPREST_ACT2_TOMB_N, LVLPREST_ACT2_TOMB_KAA_N, -1, 3 },
		{ LVLPREST_ACT2_TOMB_E, LVLPREST_ACT2_TOMB_KAA_E, -1, 0 },
		{ LVLPREST_ACT2_TOMB_S, LVLPREST_ACT2_TOMB_KAA_S, -1, 1 },
		{ LVLPREST_ACT2_TOMB_W, LVLPREST_ACT2_TOMB_KAA_W, -1, 2 },
	};

	const D2MazeLevelIdStrc* pMazeLevelIds = NULL;
	D2DrlgPresetRoomStrc* pPresetRoom = NULL;
	int nDirection = 0;

	pPresetRoom = NULL;
	for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
	{
		pPresetRoom = i->pMaze;
		if (pPresetRoom->nLevelPrest > 428)
		{
			break;
		}
	}

	switch (pPresetRoom->nLevelPrest)
	{
	case LVLPREST_ACT2_TOMB_PREV_NSW:
		nDirection = 0;
		break;

	case LVLPREST_ACT2_TOMB_PREV_NEW:
		nDirection = 1;
		break;

	case LVLPREST_ACT2_TOMB_PREV_NSE:
		nDirection = 2;
		break;

	case LVLPREST_ACT2_TOMB_PREV_SEW:
		nDirection = 3;
		break;

	default:
		FOG_DisplayAssert("sPlaceTombStuff() - Why isn't the first room the preset warp room?", __FILE__, __LINE__);
		exit(-1);
		return;
	}

	if (pLevel->nLevelId >= LEVEL_STONYTOMBLEV1 && pLevel->nLevelId <= LEVEL_CLAWVIPERTEMPLELEV1)
	{
		pMazeLevelIds = &nAct2TombNextIds[nDirection];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{			
			for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
			{
				if (!DRLGMAZE_HasMapDS1(i) && DRLGMAZE_InitRoomFixedPreset(i, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 1))
				{
					break;
				}
			}
		}
		nDirection = (nDirection + 1) % 4;
	}

	if (pLevel->nLevelId == LEVEL_HALLSOFTHEDEADLEV2)
	{
		pMazeLevelIds = &nAct2TombWaypointIds[nDirection];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		nDirection = (nDirection + 1) % 4;
	}

	if (pLevel->nLevelId == LEVEL_STONYTOMBLEV2 || pLevel->nLevelId == LEVEL_CLAWVIPERTEMPLELEV2)
	{
		pMazeLevelIds = &nAct2TombChestIds[nDirection];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		nDirection = (nDirection + 1) % 4;
	}

	if (pLevel->nLevelId >= LEVEL_TALRASHASTOMB1 && pLevel->nLevelId <= LEVEL_TALRASHASTOMB7 && pLevel->nLevelId != pLevel->pDrlg->nStaffTombLevel)
	{
		pMazeLevelIds = &nAct2TombChestIds[nDirection];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		nDirection = (nDirection + 1) % 4;
	}

	if (pLevel->nLevelId == LEVEL_STONYTOMBLEV2)
	{
		pMazeLevelIds = &nAct2TombLeatherarmIds[nDirection];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		nDirection = (nDirection + 1) % 4;
	}

	if (pLevel->nLevelId == LEVEL_HALLSOFTHEDEADLEV3)
	{
		pMazeLevelIds = &nAct2TombCubeIds[nDirection];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		nDirection = (nDirection + 1) % 4;
	}

	if (pLevel->nLevelId == LEVEL_STONYTOMBLEV2 || pLevel->nLevelId == LEVEL_CLAWVIPERTEMPLELEV2)
	{
		pMazeLevelIds = &nAct2TombTreasureIds[nDirection];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		nDirection = (nDirection + 1) % 4;
	}

	if (pLevel->nLevelId == pLevel->pDrlg->nStaffTombLevel)
	{
		pMazeLevelIds = &nAct2TombTalRashaIds[nDirection];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		nDirection = (nDirection + 1) % 4;
	}

	if (pLevel->nLevelId == pLevel->pDrlg->nBossTombLevel)
	{
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct2TombKaaIds[nDirection], &nDirection);
	}
}

//D2Common.0x6FD7BC40
D2DrlgRoomStrc* __fastcall DRLGMAZE_InitRoomFixedPreset(D2DrlgRoomStrc* pDrlgRoom, int nDirection, int nLvlPrestId, int nFile, BOOL bUseInitPreset)
{
	D2DrlgRoomStrc* pNewRoomEx = DRLGROOM_AllocRoomEx(pDrlgRoom->pLevel, DRLGTYPE_PRESET);

	DRLGMAZE_SetRoomSize(pNewRoomEx);

	if (DRLGMAZE_LinkMazeRooms(pNewRoomEx, pDrlgRoom, nDirection))
	{
		DRLGROOM_AllocDrlgOrthsForRooms(pDrlgRoom, pNewRoomEx, nDirection);
		DRLGROOM_AddRoomExToLevel(pDrlgRoom->pLevel, pNewRoomEx);

		if (bUseInitPreset)
		{
			DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
		}

		pNewRoomEx->pMaze->nLevelPrest = nLvlPrestId;
		pNewRoomEx->pMaze->nPickedFile = nFile;
		pNewRoomEx->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
		return pNewRoomEx;
	}
	else
	{
		DRLGROOM_FreeRoomEx(pNewRoomEx);
		return NULL;
	}
}

//D2Common.0x6FD7BCD0
void __fastcall DRLGMAZE_PlaceAct2LairStuff(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct2LairPrevIds[] =
	{
		{ LVLPREST_ACT2_LAIR_N, LVLPREST_ACT2_LAIR_PREV_N, -1, 3 },
		{ LVLPREST_ACT2_LAIR_E, LVLPREST_ACT2_LAIR_PREV_E, -1, 0 },
		{ LVLPREST_ACT2_LAIR_S, LVLPREST_ACT2_LAIR_PREV_S, -1, 1 },
		{ LVLPREST_ACT2_LAIR_W, LVLPREST_ACT2_LAIR_PREV_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct2LairSpecialIds[] =
	{
		{ LVLPREST_ACT2_LAIR_N, LVLPREST_ACT2_LAIR_NEXT_N, -1, 3 },
		{ LVLPREST_ACT2_LAIR_E, LVLPREST_ACT2_LAIR_NEXT_E, -1, 0 },
		{ LVLPREST_ACT2_LAIR_S, LVLPREST_ACT2_LAIR_NEXT_S, -1, 1 },
		{ LVLPREST_ACT2_LAIR_W, LVLPREST_ACT2_LAIR_NEXT_W, -1, 2 },
		{ LVLPREST_ACT2_LAIR_N, LVLPREST_ACT2_LAIR_TREASURE_N, -1, 3 },
		{ LVLPREST_ACT2_LAIR_E, LVLPREST_ACT2_LAIR_TREASURE_E, -1, 0 },
		{ LVLPREST_ACT2_LAIR_S, LVLPREST_ACT2_LAIR_TREASURE_S, -1, 1 },
		{ LVLPREST_ACT2_LAIR_W, LVLPREST_ACT2_LAIR_TREASURE_W, -1, 2 },
		{ LVLPREST_ACT2_LAIR_S, LVLPREST_ACT2_LAIR_TIGHT_SPOT_S, -1, 1 },
	};

	const D2MazeLevelIdStrc* pMazeLevelIds = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	int nIndex = 0;
	int nRand = 0;

	nRand = SEED_RollRandomNumber(&pLevel->pSeed) & 3;

	if (pLevel->nLevelId == LEVEL_MAGGOTLAIRLEV3)
	{
		pDrlgRoom = pLevel->pFirstRoomEx;
		while (pDrlgRoom)
		{
			if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == LVLPREST_ACT2_LAIR_S)
			{
				DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, LVLPREST_ACT2_LAIR_TIGHT_SPOT_S, -1, FALSE);
				break;
			}

			pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
		}

		if (!pDrlgRoom)
		{
			pDrlgRoom = pLevel->pFirstRoomEx;
			while (pDrlgRoom)
			{
				if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && DRLGMAZE_InitRoomFixedPreset(pDrlgRoom, 1, LVLPREST_ACT2_LAIR_TIGHT_SPOT_S, -1, 1))
				{
					break;
				}

				pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
			}
		}

		pDrlgRoom = pLevel->pFirstRoomEx;
		while (pDrlgRoom)
		{
			if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == LVLPREST_ACT2_LAIR_W)
			{
				DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, LVLPREST_ACT2_LAIR_TREASURE_W, -1, FALSE);
				break;
			}

			pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
		}

		if (!pDrlgRoom)
		{
			DRLGMAZE_AddSpecialPreset(pLevel, 2, LVLPREST_ACT2_LAIR_TREASURE_W, -1);
		}

		nIndex = 0;
	}
	else
	{
		pMazeLevelIds = &nAct2LairSpecialIds[nRand];
		pDrlgRoom = pLevel->pFirstRoomEx;
		while (pDrlgRoom)
		{
			if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
			{
				DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
				break;
			}

			pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
		}

		if (!pDrlgRoom)
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		nIndex = (nRand + 1) % 4;
	}

	pMazeLevelIds = &nAct2LairPrevIds[nIndex];
	if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
	{
		DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
	}
}

//D2Common.0x6FD7BE60
void __fastcall DRLGMAZE_PlaceAct3DungeonStuff(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct3DungeonPrevIds[] =
	{
		{ LVLPREST_ACT3_DUNGEON_N, LVLPREST_ACT3_DUNGEON_PREV_N, -1, 3 },
		{ LVLPREST_ACT3_DUNGEON_E, LVLPREST_ACT3_DUNGEON_PREV_E, -1, 0 },
		{ LVLPREST_ACT3_DUNGEON_S, LVLPREST_ACT3_DUNGEON_PREV_S, -1, 1 },
		{ LVLPREST_ACT3_DUNGEON_W, LVLPREST_ACT3_DUNGEON_PREV_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct3DungeonNextIds[] =
	{
		{ LVLPREST_ACT3_DUNGEON_N, LVLPREST_ACT3_DUNGEON_NEXT_N, -1, 3 },
		{ LVLPREST_ACT3_DUNGEON_E, LVLPREST_ACT3_DUNGEON_NEXT_E, -1, 0 },
		{ LVLPREST_ACT3_DUNGEON_S, LVLPREST_ACT3_DUNGEON_NEXT_S, -1, 1 },
		{ LVLPREST_ACT3_DUNGEON_W, LVLPREST_ACT3_DUNGEON_NEXT_W, -1, 2 },
	};

	const D2MazeLevelIdStrc* pMazeLevelIds = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	int nDirection = 0;

	nDirection = SEED_RollRandomNumber(&pLevel->pSeed) & 3;
	
	pMazeLevelIds = &nAct3DungeonPrevIds[nDirection];
	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
		{
			pDrlgRoom->pMaze->nLevelPrest = pMazeLevelIds->nLevelPrestId2;
			pDrlgRoom->pMaze->nPickedFile = pMazeLevelIds->nPickedFile;
			pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
			break;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}

	if (!pDrlgRoom)
	{	
		for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			if (!DRLGMAZE_HasMapDS1(i))
			{
				pDrlgRoom = DRLGMAZE_PlaceAdjacentPresetRoom(i, pMazeLevelIds->nDirection, FALSE);
				if (pDrlgRoom)
				{
					DRLGMAZE_PickRoomPreset(i, 1);
					DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
					break;
				}
			}
		}
	}

	pMazeLevelIds = &nAct3DungeonNextIds[(nDirection + 1) % 4];
	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
		{
			pDrlgRoom->pMaze->nLevelPrest = pMazeLevelIds->nLevelPrestId2;
			pDrlgRoom->pMaze->nPickedFile = pMazeLevelIds->nPickedFile;
			pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
			break;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}

	if (!pDrlgRoom)
	{
		for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			if (!DRLGMAZE_HasMapDS1(i))
			{
				pDrlgRoom = DRLGMAZE_PlaceAdjacentPresetRoom(i, pMazeLevelIds->nDirection, FALSE);
				if (pDrlgRoom)
				{
					DRLGMAZE_PickRoomPreset(i, 1);
					DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
					break;
				}
			}

			
		}
	}
}

//D2Common.0x6FD7C000
void __fastcall DRLGMAZE_PlaceAct3SewerStuff(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct3SewerDrainIds[] =
	{
		{ LVLPREST_ACT3_SEWER_N, LVLPREST_ACT3_SEWER_DRAIN_N, -1, 3 },
		{ LVLPREST_ACT3_SEWER_E, LVLPREST_ACT3_SEWER_DRAIN_E, -1, 0 },
		{ LVLPREST_ACT3_SEWER_S, LVLPREST_ACT3_SEWER_DRAIN_S, -1, 1 },
		{ LVLPREST_ACT3_SEWER_W, LVLPREST_ACT3_SEWER_DRAIN_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct3SewerChestIds[] =
	{
		{ LVLPREST_ACT3_SEWER_N, LVLPREST_ACT3_SEWER_CHEST_N, -1, 3 },
		{ LVLPREST_ACT3_SEWER_E, LVLPREST_ACT3_SEWER_CHEST_E, -1, 0 },
		{ LVLPREST_ACT3_SEWER_S, LVLPREST_ACT3_SEWER_CHEST_S, -1, 1 },
		{ LVLPREST_ACT3_SEWER_W, LVLPREST_ACT3_SEWER_CHEST_W, -1, 2 },
	};

	const D2MazeLevelIdStrc* pMazeLevelIds = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	D2DrlgRoomStrc* pTemp = NULL;
	int nDirection = 0;

	nDirection = SEED_RollRandomNumber(&pLevel->pSeed) & 3;
	
	pMazeLevelIds = &nAct3SewerDrainIds[nDirection];
	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
		{
			pDrlgRoom->pMaze->nLevelPrest = pMazeLevelIds->nLevelPrestId2;
			pDrlgRoom->pMaze->nPickedFile = pMazeLevelIds->nPickedFile;
			pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
			break;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}

	if (!pDrlgRoom)
	{
		for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			if (!DRLGMAZE_HasMapDS1(i))
			{
				pTemp = DRLGMAZE_PlaceAdjacentPresetRoom(i, pMazeLevelIds->nDirection, FALSE);
				if (pTemp)
				{
					DRLGMAZE_PickRoomPreset(i, 1);
					DRLGMAZE_SetPickedFileAndPresetId(pTemp, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
					break;
				}
			}
		}
	}

	pMazeLevelIds = &nAct3SewerChestIds[(nDirection + 1) % 4];
	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
		{
			pDrlgRoom->pMaze->nLevelPrest = pMazeLevelIds->nLevelPrestId2;
			pDrlgRoom->pMaze->nPickedFile = pMazeLevelIds->nPickedFile;
			pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
			break;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}

	if (!pDrlgRoom)
	{
		for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			if (!DRLGMAZE_HasMapDS1(i))
			{
				pTemp = DRLGMAZE_PlaceAdjacentPresetRoom(i, pMazeLevelIds->nDirection, FALSE);
				if (pTemp)
				{
					DRLGMAZE_PickRoomPreset(i, 1);
					DRLGMAZE_SetPickedFileAndPresetId(pTemp, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
					break;
				}
			}
		}
	}
}

//D2Common.0x6FD7C1A0
//TODO: v9, v14
void __fastcall DRLGMAZE_PlaceAct3MephistoStuff(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct3MephistoPrevIds[] =
	{
		{ LVLPREST_ACT3_MEPHISTO_N, LVLPREST_ACT3_MEPHISTO_PREV_N, -1, 3 },
		{ LVLPREST_ACT3_MEPHISTO_E, LVLPREST_ACT3_MEPHISTO_PREV_E, -1, 0 },
		{ LVLPREST_ACT3_MEPHISTO_S, LVLPREST_ACT3_MEPHISTO_PREV_S, -1, 1 },
		{ LVLPREST_ACT3_MEPHISTO_W, LVLPREST_ACT3_MEPHISTO_PREV_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct3MephistoWaypointIds[] =
	{
		{ LVLPREST_ACT3_MEPHISTO_N, LVLPREST_ACT3_MEPHISTO_WAYPOINT_N, -1, 3 },
		{ LVLPREST_ACT3_MEPHISTO_E, LVLPREST_ACT3_MEPHISTO_WAYPOINT_E, -1, 0 },
		{ LVLPREST_ACT3_MEPHISTO_S, LVLPREST_ACT3_MEPHISTO_WAYPOINT_S, -1, 1 },
		{ LVLPREST_ACT3_MEPHISTO_W, LVLPREST_ACT3_MEPHISTO_WAYPOINT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct3MephistoNextIds[] =
	{
		{ LVLPREST_ACT3_MEPHISTO_N, LVLPREST_ACT3_MEPHISTO_NEXT_N, -1, 3 },
		{ LVLPREST_ACT3_MEPHISTO_E, LVLPREST_ACT3_MEPHISTO_NEXT_E, -1, 0 },
		{ LVLPREST_ACT3_MEPHISTO_S, LVLPREST_ACT3_MEPHISTO_NEXT_S, -1, 1 },
		{ LVLPREST_ACT3_MEPHISTO_W, LVLPREST_ACT3_MEPHISTO_NEXT_W, -1, 2 },
	};

	const D2MazeLevelIdStrc* pMazeLevelIds = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	int v9 = 0;
	int v14 = 0;
	int nRand = 0;

	nRand = SEED_RollRandomNumber(&pLevel->pSeed) & 3;

	pMazeLevelIds = &nAct3MephistoPrevIds[nRand];
	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
		{
			DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
			break;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}

	if (!pDrlgRoom)
	{
		for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			if (!DRLGMAZE_HasMapDS1(i) && DRLGMAZE_InitRoomFixedPreset(i, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 1))
			{
				break;
			}
		}
	}

	v9 = (nRand + 1) % 4;
	if (pLevel->nLevelId == LEVEL_DURANCEOFHATELEV2)
	{
		pMazeLevelIds = &nAct3MephistoWaypointIds[v9];
		pDrlgRoom = pLevel->pFirstRoomEx;
		while (pDrlgRoom)
		{
			if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
			{
				DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
				break;
			}

			pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
		}

		if (!pDrlgRoom)
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		v14 = (v9 + 1) % 4;
		pMazeLevelIds = &nAct3MephistoNextIds[v14];
		pDrlgRoom = pLevel->pFirstRoomEx;
		while (pDrlgRoom)
		{
			if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
			{
				DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
				break;
			}

			pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
		}

		if (!pDrlgRoom)
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		v9 = (v14 + 1) % 4;
	}
	
	if (pLevel->nLevelId == LEVEL_DURANCEOFHATELEV1)
	{
		pMazeLevelIds = &nAct3MephistoNextIds[v9];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}
	}
}

//D2Common.0x6FD7C380
void __fastcall DRLGMAZE_PlaceAct5TempleStuff(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct5TempleSpecialIds[] =
	{
		{ LVLPREST_ACT5_TEMPLE_NE, LVLPREST_ACT5_TEMPLE_NE_DOWN, -1, 1 },
		{ LVLPREST_ACT5_TEMPLE_NW, LVLPREST_ACT5_TEMPLE_NW_DOWN, -1, 0 },
		{ LVLPREST_ACT5_TEMPLE_SW, LVLPREST_ACT5_TEMPLE_SW_DOWN, -1, 3 },
		{ LVLPREST_ACT5_TEMPLE_NE, LVLPREST_ACT5_TEMPLE_NE_WAYPOINT, -1, 1 },
		{ LVLPREST_ACT5_TEMPLE_NW, LVLPREST_ACT5_TEMPLE_NW_WAYPOINT, -1, 0 },
		{ LVLPREST_ACT5_TEMPLE_SW, LVLPREST_ACT5_TEMPLE_SW_WAYPOINT, -1, 3 },
		{ LVLPREST_ACT5_TEMPLE_SE_UP, LVLPREST_ACT5_TEMPLE_SE_WAYPOINT, -1, 2 },
	};

	const D2MazeLevelIdStrc* pMazeLevelIds = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	unsigned int nStartDirection = 0;
	unsigned int nDirection = 0;

	nDirection = (unsigned int)SEED_RollRandomNumber(&pLevel->pSeed) % 3;
	nStartDirection = nDirection;

	if (pLevel->nLevelId != LEVEL_HALLSOFVAUGHT)
	{
		pMazeLevelIds = &nAct5TempleSpecialIds[nDirection];
		pDrlgRoom = pLevel->pFirstRoomEx;
		while (pDrlgRoom)
		{
			if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
			{
				pDrlgRoom->pMaze->nLevelPrest = pMazeLevelIds->nLevelPrestId2;
				pDrlgRoom->pMaze->nPickedFile = pMazeLevelIds->nPickedFile;
				pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
				break;
			}

			pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
		}

		if (!pDrlgRoom)
		{
			for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
			{
				if (!DRLGMAZE_HasMapDS1(i) && DRLGMAZE_InitRoomFixedPreset(i, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 1))
				{
					break;
				}
			}
		}

		nDirection = (int)(nDirection + 1) % 4;
	}

	if (pLevel->nLevelId == LEVEL_HALLSOFDEATHSCALLING)
	{
		D2_ASSERT(nDirection != nStartDirection);

		pMazeLevelIds = &nAct5TempleSpecialIds[nDirection + 3];
		pDrlgRoom = pLevel->pFirstRoomEx;
		while (pDrlgRoom)
		{
			if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
			{
				pDrlgRoom->pMaze->nPickedFile = pMazeLevelIds->nPickedFile;
				pDrlgRoom->pMaze->nLevelPrest = pMazeLevelIds->nLevelPrestId2;
				pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
				break;
			}

			pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
		}
		
		if (!pDrlgRoom)
		{
			for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
			{
				if (!DRLGMAZE_HasMapDS1(i) && DRLGMAZE_InitRoomFixedPreset(i, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 1))
				{
					break;
				}
			}
		}
	}
}

//D2Common.0x6FD7C500
void __fastcall DRLGMAZE_PlaceAct5BaalStuff(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct5BaalNextIds[] =
	{
		{ LVLPREST_ACT5_BAAL_N, LVLPREST_ACT5_BAAL_NEXT_N, -1, 3 },
		{ LVLPREST_ACT5_BAAL_E, LVLPREST_ACT5_BAAL_NEXT_E, -1, 0 },
		{ LVLPREST_ACT5_BAAL_S, LVLPREST_ACT5_BAAL_NEXT_S, -1, 1 },
		{ LVLPREST_ACT5_BAAL_W, LVLPREST_ACT5_BAAL_NEXT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct5BaalWaypointIds[] =
	{
		{ LVLPREST_ACT5_BAAL_N, LVLPREST_ACT5_BAAL_WAYPOINT_N, -1, 3 },
		{ LVLPREST_ACT5_BAAL_E, LVLPREST_ACT5_BAAL_WAYPOINT_E, -1, 0 },
		{ LVLPREST_ACT5_BAAL_S, LVLPREST_ACT5_BAAL_WAYPOINT_S, -1, 1 },
		{ LVLPREST_ACT5_BAAL_W, LVLPREST_ACT5_BAAL_WAYPOINT_W, -1, 2 },
	};

	const D2MazeLevelIdStrc* pMazeLevelIds = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	int nDirection = 0;

	nDirection = SEED_RollRandomNumber(&pLevel->pSeed) & 3;
	pMazeLevelIds = &nAct5BaalNextIds[nDirection];
	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
		{
			pDrlgRoom->pMaze->nLevelPrest = pMazeLevelIds->nLevelPrestId2;
			pDrlgRoom->pMaze->nPickedFile = pMazeLevelIds->nPickedFile;
			pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
			break;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}

	if (!pDrlgRoom)
	{
		for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			if (!DRLGMAZE_HasMapDS1(i) && DRLGMAZE_InitRoomFixedPreset(i, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 1))
			{
				break;
			}
		}
	}

	if (pLevel->nLevelId == LEVEL_THEWORLDSTONEKEEPLEV2)
	{
		pMazeLevelIds = &nAct5BaalWaypointIds[(nDirection + 1) % 4];
		pDrlgRoom = pLevel->pFirstRoomEx;
		while (pDrlgRoom)
		{
			if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
			{
				pDrlgRoom->pMaze->nPickedFile = pMazeLevelIds->nPickedFile;
				pDrlgRoom->pMaze->nLevelPrest = pMazeLevelIds->nLevelPrestId2;
				pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;
				break;
			}

			pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
		}

		if (!pDrlgRoom)
		{
			for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
			{
				if (!DRLGMAZE_HasMapDS1(i) && DRLGMAZE_InitRoomFixedPreset(i, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 1))
				{
					break;
				}
			}
		}
	}
}

//D2Common.0x6FD7C660
void __fastcall DRLGMAZE_PlaceAct1Barracks(D2DrlgLevelStrc* pLevel)
{
	static D2DrlgRoomStrc* (__fastcall* pfGetBarracksEntryRoom[])(D2DrlgLevelStrc*) =
	{
		DRLGMAZE_GetFreeLocationForRoomWest,
		DRLGMAZE_GetFreeLocationForRoomNorth,
		DRLGMAZE_GetFreeLocationForRoomEast,
	};

	static const D2MazeLevelIdStrc nAct1BarracksNextIds[] =
	{
		{ LVLPREST_ACT1_BARRACKS_N, LVLPREST_ACT1_BARRACKS_NEXT_N, -1, 3 },
		{ LVLPREST_ACT1_BARRACKS_E, LVLPREST_ACT1_BARRACKS_NEXT_E, -1, 0 },
		{ LVLPREST_ACT1_BARRACKS_S, LVLPREST_ACT1_BARRACKS_NEXT_S, -1, 1 },
		{ LVLPREST_ACT1_BARRACKS_W, LVLPREST_ACT1_BARRACKS_NEXT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct1BarracksForgeIds[] =
	{
		{ LVLPREST_ACT1_BARRACKS_N, LVLPREST_ACT1_BARRACKS_FORGE_N, -1, 3 },
		{ LVLPREST_ACT1_BARRACKS_E, LVLPREST_ACT1_BARRACKS_FORGE_E, -1, 0 },
		{ LVLPREST_ACT1_BARRACKS_S, LVLPREST_ACT1_BARRACKS_FORGE_S, -1, 1 },
		{ LVLPREST_ACT1_BARRACKS_W, LVLPREST_ACT1_BARRACKS_FORGE_W, -1, 2 },
	};

	const D2MazeLevelIdStrc* pMazeLevelIds = NULL;
	D2DrlgLevelStrc* pOuterCloisterLevel = NULL;
	D2DrlgRoomStrc* pBarracksRoomEx = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	int nTileMinX = 0;
	int nTileMinY = 0;
	int nTileMaxX = 0;
	int nTileMaxY = 0;
	int nDirection = 0;
	int nRand = 0;
	int nX = 0;
	int nY = 0;

	pOuterCloisterLevel = DRLG_GetLevel(pLevel->pDrlg, LEVEL_OUTERCLOISTER);
	nDirection = pOuterCloisterLevel->pPreset->nDirection;

	pDrlgRoom = pfGetBarracksEntryRoom[nDirection](pLevel);

	nX = pOuterCloisterLevel->nPosX;
	nY = pOuterCloisterLevel->nPosY;

	switch (nDirection)
	{
	case 0:
		pBarracksRoomEx = DRLGROOM_AllocRoomEx(pDrlgRoom->pLevel, DRLGTYPE_PRESET);
		DRLGMAZE_SetRoomSize(pBarracksRoomEx);

		if (DRLGMAZE_LinkMazeRooms(pBarracksRoomEx, pDrlgRoom, 2))
		{
			DRLGROOM_AllocDrlgOrthsForRooms(pDrlgRoom, pBarracksRoomEx, 2);
			DRLGROOM_AddRoomExToLevel(pDrlgRoom->pLevel, pBarracksRoomEx);
			if (pBarracksRoomEx)
			{
				DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
				DRLGMAZE_SetPickedFileAndPresetId(pBarracksRoomEx, LVLPREST_ACT1_BARRACKS_COURT_CONNECT, 0, FALSE);
			}
		}
		else
		{
			DRLGROOM_FreeRoomEx(pBarracksRoomEx);
			pBarracksRoomEx = 0;
		}
		D2_ASSERT(pBarracksRoomEx); // Original game does not check for nullptr here, and may still dereference a nullptr...

		DRLGROOM_AddOrth(&pBarracksRoomEx->pDrlgOrth, pOuterCloisterLevel, 2, FALSE);
		nX -= pLevel->pMaze->dwSizeX + pBarracksRoomEx->nTileXPos;
		nY += pOuterCloisterLevel->nHeight / 2 - pBarracksRoomEx->nTileYPos;
		break;

	case 1:
		pBarracksRoomEx = DRLGROOM_AllocRoomEx(pDrlgRoom->pLevel, DRLGTYPE_PRESET);
		DRLGMAZE_SetRoomSize(pBarracksRoomEx);

		if (DRLGMAZE_LinkMazeRooms(pBarracksRoomEx, pDrlgRoom, 3))
		{
			DRLGROOM_AllocDrlgOrthsForRooms(pDrlgRoom, pBarracksRoomEx, 3);
			DRLGROOM_AddRoomExToLevel(pDrlgRoom->pLevel, pBarracksRoomEx);
			if (pBarracksRoomEx)
			{
				DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
				DRLGMAZE_SetPickedFileAndPresetId(pBarracksRoomEx, LVLPREST_ACT1_BARRACKS_COURT_CONNECT, 1, FALSE);
			}
		}
		else
		{
			DRLGROOM_FreeRoomEx(pBarracksRoomEx);
			pBarracksRoomEx = 0;
		}

		DRLGROOM_AddOrth(&pBarracksRoomEx->pDrlgOrth, pOuterCloisterLevel, 3, FALSE);
		nX += pOuterCloisterLevel->nWidth / 2 - pBarracksRoomEx->nTileXPos - 6;
		nY -= pLevel->pMaze->dwSizeY + pBarracksRoomEx->nTileYPos;
		break;

	case 2:
		pBarracksRoomEx = DRLGMAZE_PlaceAdjacentPresetRoom(pDrlgRoom, ALTDIR_WEST, FALSE);
		if (pBarracksRoomEx)
		{
			DRLGMAZE_PickRoomPreset(pDrlgRoom, 1);
			DRLGMAZE_SetPickedFileAndPresetId(pBarracksRoomEx, LVLPREST_ACT1_BARRACKS_COURT_CONNECT, 2, FALSE);
		}

		DRLGROOM_AddOrth(&pBarracksRoomEx->pDrlgOrth, pOuterCloisterLevel, 0, FALSE);
		nX += pOuterCloisterLevel->nWidth - pBarracksRoomEx->nTileXPos;
		nY += pOuterCloisterLevel->nHeight / 2 - pBarracksRoomEx->nTileYPos + 1;
		break;
	default:
		break;
	}

	if (SEED_RollRandomNumber(&pLevel->pSeed) & 1)
	{
		pMazeLevelIds = &nAct1BarracksNextIds[nDirection];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
			{
				if (!DRLGMAZE_HasMapDS1(i) && DRLGMAZE_InitRoomFixedPreset(i, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 1))
				{
					break;
				}
			}
		}

		pMazeLevelIds = &nAct1BarracksForgeIds[(nDirection + 1) % 4];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}
	}
	else
	{
		pMazeLevelIds = &nAct1BarracksForgeIds[nDirection];
		if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
		{
			DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
		}

		nRand = (nDirection + 1) % 4;
		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct1BarracksNextIds[nRand], &nRand);
	}

	for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
	{
		i->nTileXPos += nX;
		i->nTileYPos += nY;
	}

	DRLG_GetMinAndMaxCoordinatesFromLevel(pLevel, &nTileMinX, &nTileMinY, &nTileMaxX, &nTileMaxY);
	pLevel->nPosX = nTileMinX;
	pLevel->nPosY = nTileMinY;
	pLevel->nWidth = nTileMaxX - nTileMinX;
	pLevel->nHeight = nTileMaxY - nTileMinY;
}

//D2Common.0x6FD7CA20
void __fastcall DRLGMAZE_SetRoomSize(D2DrlgRoomStrc* pDrlgRoom)
{
	pDrlgRoom->nTileWidth = pDrlgRoom->pLevel->pMaze->dwSizeX;
	pDrlgRoom->nTileHeight = pDrlgRoom->pLevel->pMaze->dwSizeY;
}

//D2Common.0x6FD7CA40
void __fastcall DRLGMAZE_PlaceAct4Lava(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct4LavaForgeIds[] =
	{
		{ LVLPREST_ACT4_LAVA_W, LVLPREST_ACT4_LAVA_FORGE_W, -1, 2 },
		{ LVLPREST_ACT4_LAVA_E, LVLPREST_ACT4_LAVA_FORGE_E, -1, 0 },
	};

	if (pLevel->pDrlg->nAct != ACT_V)
	{
		D2DrlgLevelStrc* pChaosSanctum = DRLG_GetLevel(pLevel->pDrlg, LEVEL_CHAOSSANCTUM);

		D2DrlgRoomStrc* pParentRoom = DRLGMAZE_GetFreeLocationForRoomNorth(pLevel);

		D2DrlgRoomStrc* pNewRoomEx = DRLGROOM_AllocRoomEx(pParentRoom->pLevel, DRLGTYPE_PRESET);
		DRLGMAZE_SetRoomSize(pNewRoomEx);

		if (DRLGMAZE_LinkMazeRooms(pNewRoomEx, pParentRoom, ALTDIR_SOUTH))
		{
			DRLGROOM_AllocDrlgOrthsForRooms(pParentRoom, pNewRoomEx, ALTDIR_SOUTH);
			DRLGROOM_AddRoomExToLevel(pParentRoom->pLevel, pNewRoomEx);
			if (pNewRoomEx)
			{
				DRLGMAZE_PickRoomPreset(pParentRoom, TRUE);
				DRLGMAZE_SetPickedFileAndPresetId(pNewRoomEx, LVLPREST_ACT4_LAVA_WARP_N, -1, FALSE);
			}
		}
		else
		{
			DRLGROOM_FreeRoomEx(pNewRoomEx);
		}

		pParentRoom = DRLGMAZE_GetFreeLocationForRoomSouth(pLevel);

		// Add bridge between lava (where the statues are) and chaos sanctum 
		D2DrlgRoomStrc* pBridgeRoomEx1 = DRLGMAZE_PlaceAdjacentPresetRoom(pParentRoom, ALTDIR_NORTH, FALSE);
		if (pBridgeRoomEx1)
		{
			DRLGMAZE_PickRoomPreset(pParentRoom, 1);
			DRLGMAZE_SetPickedFileAndPresetId(pBridgeRoomEx1, LVLPREST_ACT4_BRIDGE_1, -1, FALSE);
		}

		D2DrlgRoomStrc* pBridgeRoomEx2 = DRLGMAZE_PlaceAdjacentPresetRoom(pBridgeRoomEx1, ALTDIR_NORTH, FALSE);
		if (pBridgeRoomEx2)
		{
			DRLGMAZE_SetPickedFileAndPresetId(pBridgeRoomEx2, LVLPREST_ACT4_BRIDGE_2, -1, FALSE);
		}

		D2DrlgRoomStrc* pBridgeRoomEx3 = DRLGMAZE_PlaceAdjacentPresetRoom(pBridgeRoomEx2, ALTDIR_NORTH, FALSE);
		if (pBridgeRoomEx3)
		{
			DRLGMAZE_SetPickedFileAndPresetId(pBridgeRoomEx3, LVLPREST_ACT4_BRIDGE_2, -1, FALSE);
		}

		DRLGROOM_AddOrth(&pBridgeRoomEx3->pDrlgOrth, pChaosSanctum, ALTDIR_NORTH, FALSE);

		const int nX = pChaosSanctum->nPosX + 2 * pBridgeRoomEx3->nTileWidth - pBridgeRoomEx3->nTileXPos;
		const int nY = pChaosSanctum->nPosY + pChaosSanctum->nHeight - pBridgeRoomEx3->nTileYPos;

		DRLGMAZE_ScanReplaceSpecialPreset(pLevel, &nAct4LavaForgeIds[SEED_RollRandomNumber(&pLevel->pSeed) & 1], 0);

		DRLGMAZE_FillBlankMazeSpaces(pLevel, LVLPREST_ACT4_LAVA_X, pBridgeRoomEx3);

		for (D2DrlgRoomStrc* i = pLevel->pFirstRoomEx; i; i = i->pDrlgRoomNext)
		{
			i->nTileXPos += nX;
			i->nTileYPos += nY;
		}

		int nMinX, nMinY, nMaxX, nMaxY;
		DRLG_GetMinAndMaxCoordinatesFromLevel(pLevel, &nMinX, &nMinY, &nMaxX, &nMaxY);

		pLevel->nPosX = nMinX;
		pLevel->nPosY = nMinY;
		pLevel->nWidth = nMaxX - nMinX;
		pLevel->nHeight = nMaxY - nMinY;
	}
}

//D2Common.0x6FD7CCB0
void __fastcall DRLGMAZE_PlaceAct5IceStuff(D2DrlgLevelStrc* pLevel)
{
	static const D2MazeLevelIdStrc nAct5IcePrevIds[] =
	{
		{ LVLPREST_ACT5_ICE_N, LVLPREST_ACT5_ICE_PREV_N, -1, 3 },
		{ LVLPREST_ACT5_ICE_E, LVLPREST_ACT5_ICE_PREV_E, -1, 0 },
		{ LVLPREST_ACT5_ICE_S, LVLPREST_ACT5_ICE_PREV_S, -1, 1 },
		{ LVLPREST_ACT5_ICE_W, LVLPREST_ACT5_ICE_PREV_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct5IceNextIds[] =
	{
		{ LVLPREST_ACT5_ICE_N, LVLPREST_ACT5_ICE_NEXT_N, -1, 3 },
		{ LVLPREST_ACT5_ICE_E, LVLPREST_ACT5_ICE_NEXT_E, -1, 0 },
		{ LVLPREST_ACT5_ICE_S, LVLPREST_ACT5_ICE_NEXT_S, -1, 1 },
		{ LVLPREST_ACT5_ICE_W, LVLPREST_ACT5_ICE_NEXT_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct5IceDownIds[] =
	{
		{ LVLPREST_ACT5_ICE_N, LVLPREST_ACT5_ICE_DOWN_N, -1, 3 },
		{ LVLPREST_ACT5_ICE_E, LVLPREST_ACT5_ICE_DOWN_E, -1, 0 },
		{ LVLPREST_ACT5_ICE_S, LVLPREST_ACT5_ICE_DOWN_S, -1, 1 },
		{ LVLPREST_ACT5_ICE_W, LVLPREST_ACT5_ICE_DOWN_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct5IceThemeIds[] =
	{
		{ LVLPREST_ACT5_ICE_N, LVLPREST_ACT5_ICE_THEME_N, -1, 3 },
		{ LVLPREST_ACT5_ICE_E, LVLPREST_ACT5_ICE_THEME_E, -1, 0 },
		{ LVLPREST_ACT5_ICE_S, LVLPREST_ACT5_ICE_THEME_S, -1, 1 },
		{ LVLPREST_ACT5_ICE_W, LVLPREST_ACT5_ICE_THEME_W, -1, 2 },
	};

	static const D2MazeLevelIdStrc nAct5IceWaypointIds[] =
	{
		{ LVLPREST_ACT5_ICE_N, LVLPREST_ACT5_ICE_WAYPOINT_N, -1, 3 },
		{ LVLPREST_ACT5_ICE_E, LVLPREST_ACT5_ICE_WAYPOINT_E, -1, 0 },
		{ LVLPREST_ACT5_ICE_S, LVLPREST_ACT5_ICE_WAYPOINT_S, -1, 1 },
		{ LVLPREST_ACT5_ICE_W, LVLPREST_ACT5_ICE_WAYPOINT_W, -1, 2 },
	};

	const D2MazeLevelIdStrc* pMazeLevelIds = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	int nDirection = 0;

	nDirection = SEED_RollRandomNumber(&pLevel->pSeed) & 3;
	pMazeLevelIds = &nAct5IcePrevIds[nDirection];

	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
		{
			DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
			break;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}
	
	if (!pDrlgRoom)
	{
		DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
	}

	nDirection = (nDirection + 1) % 4;
	pMazeLevelIds = &nAct5IceNextIds[nDirection];
	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
		{
			DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
			break;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}

	if (!pDrlgRoom)
	{
		DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
	}

	nDirection = (nDirection + 1) % 4;
	pMazeLevelIds = &nAct5IceDownIds[nDirection];
	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == pMazeLevelIds->nLevelPrestId1)
		{
			DRLGMAZE_SetPickedFileAndPresetId(pDrlgRoom, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, FALSE);
			break;
		}

		pDrlgRoom = pDrlgRoom->pDrlgRoomNext;
	}

	if (!pDrlgRoom)
	{
		DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
	}

	nDirection = (nDirection + 1) % 4;
	if (pLevel->nLevelId != LEVEL_CRYSTALIZEDCAVERNLEV1)
	{
		if (pLevel->nLevelId == LEVEL_CRYSTALIZEDCAVERNLEV2)
		{
			pMazeLevelIds = &nAct5IceThemeIds[nDirection];
			if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
			{
				DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
			}

			nDirection = (nDirection + 1) % 4;
		}
		else if (pLevel->nLevelId != LEVEL_GLACIALCAVESLEV1)
		{
			return;
		}
	}

	pMazeLevelIds = &nAct5IceWaypointIds[nDirection];
	if (!DRLGMAZE_ReplaceRoomPreset(pLevel, pMazeLevelIds->nLevelPrestId1, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile, 0))
	{
		DRLGMAZE_AddSpecialPreset(pLevel, pMazeLevelIds->nDirection, pMazeLevelIds->nLevelPrestId2, pMazeLevelIds->nPickedFile);
	}
}

//D2Common.0x6FD7CEA0
void __fastcall DRLGMAZE_RollAct_1_2_3_BasicPresets(D2DrlgLevelStrc* pLevel)
{
	int pOffsets[15] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	int nLevelPrestId = 0;
	int nBaseLvlPrest = 0;
	int nCounter1 = 0;
	int nCounter2 = 0;
	int nIndex1 = 0;
	int nIndex2 = 0;
	int nIndex = 0;
	int nTemp1 = 0;
	int nTemp2 = 0;

	switch (pLevel->nLevelType)
	{
	case LVLTYPE_ACT1_CAVE:
		nBaseLvlPrest = LVLPREST_ACT1_DOE_ENTRANCE;
		break;

	case LVLTYPE_ACT1_CRYPT:
		nBaseLvlPrest = LVLPREST_ACT1_GRAVEYARD;
		break;

	case LVLTYPE_ACT1_BARRACKS:
		nBaseLvlPrest = LVLPREST_ACT1_BARRACKS_COURT_CONNECT;
		break;

	case LVLTYPE_ACT1_JAIL:
		nBaseLvlPrest = LVLPREST_ACT1_BARRACKS_FORGE_N;
		break;

	case LVLTYPE_ACT1_CATACOMBS:
		nBaseLvlPrest = LVLPREST_ACT1_CATHEDRAL;
		break;

	case LVLTYPE_ACT2_SEWER:
		nBaseLvlPrest = LVLPREST_ACT2_TOWN;
		break;

	case LVLTYPE_ACT2_TOMB:
		nBaseLvlPrest = LVLPREST_ACT2_DESERT_RUINS_ELDER;
		break;

	case LVLTYPE_ACT3_KURAST:
		nBaseLvlPrest = LVLPREST_ACT3_TEMPLE_6;
		break;

	case LVLTYPE_ACT3_DUNGEON:
		nBaseLvlPrest = LVLPREST_ACT3_SPIDER_CHEST_NE;
		break;

	case LVLTYPE_ACT3_SEWER:
		nBaseLvlPrest = LVLPREST_ACT3_DUNGEON_TREASURE_2;
		break;

	default:
		return;
	}

	if (pLevel->nLevelId == LEVEL_DENOFEVIL)
	{
		return;
	}

	for (int i = 0; i < 15; ++i)
	{
		nIndex2 = (unsigned int)SEED_RollRandomNumber(&pLevel->pSeed) % 15;
		nIndex1 = (unsigned int)SEED_RollRandomNumber(&pLevel->pSeed) % 15;

		nTemp1 = pOffsets[nIndex1];
		nTemp2 = pOffsets[nIndex2];

		pOffsets[nIndex2] = nTemp1;
		pOffsets[nIndex1] = nTemp2;
	}

	nIndex = (unsigned int)SEED_RollRandomNumber(&pLevel->pSeed) % 15;

	nCounter1 = pLevel->nRooms / 5 + 1;
	if (nCounter1 <= 2)
	{
		nCounter1 = 2;
	}

	nCounter2 = 2 * pLevel->nRooms;

	while (nCounter1 && nCounter2)
	{
		nLevelPrestId = pOffsets[nIndex] + nBaseLvlPrest;
		
		for (D2DrlgRoomStrc* pDrlgRoom = pLevel->pFirstRoomEx; pDrlgRoom; pDrlgRoom = pDrlgRoom->pDrlgRoomNext)
		{
			if (!DRLGMAZE_HasMapDS1(pDrlgRoom) && pDrlgRoom->pMaze->nLevelPrest == nLevelPrestId)
			{
				pDrlgRoom->pMaze->nLevelPrest = nLevelPrestId + 15;
				pDrlgRoom->pMaze->nPickedFile = -1;
				pDrlgRoom->pMaze->dwFlags |= DRLGPRESETROOMFLAG_HAS_MAP_DS1;

				--nCounter1;
				break;
			}
		}

		--nCounter2;
		nIndex = (nIndex + 1) % 15;
	}
}

//D2Common.0x6FD7D130
void __fastcall DRLGMAZE_RollBasicPresets(D2DrlgLevelStrc* pLevel)
{
	D2DrlgBuildStrc* pDrlgBuild = NULL;
	D2DrlgRoomStrc* pNextRoomEx = NULL;
	D2DrlgRoomStrc* pDrlgRoom = NULL;
	D2DrlgRoomStrc* pTemp = NULL;
	D2DrlgMapStrc* pDrlgMap = NULL;
	int nLvlPrestId = 0;
	int nPickedFile = 0;
	int nLevelDef = 0;
	int nFiles = 0;

	pDrlgRoom = pLevel->pFirstRoomEx;
	while (pDrlgRoom)
	{
		pNextRoomEx = pDrlgRoom->pDrlgRoomNext;

		pDrlgMap = DRLGPRESET_AllocDrlgMap(pLevel, pDrlgRoom->pMaze->nLevelPrest, &pDrlgRoom->pDrlgCoord, &pLevel->pSeed);

		nPickedFile = pDrlgRoom->pMaze->nPickedFile;
		if (nPickedFile == -1)
		{
			nLevelDef = pDrlgMap->pLvlPrestTxtRecord->dwDef;
			nFiles = pDrlgMap->pLvlPrestTxtRecord->dwFiles;

			switch (pLevel->nLevelType)
			{
			case LVLTYPE_ACT1_CAVE:
				nLvlPrestId = LVLPREST_ACT1_DOE_ENTRANCE;
				break;

			case LVLTYPE_ACT1_CRYPT:
				nLvlPrestId = LVLPREST_ACT1_GRAVEYARD;
				break;

			case LVLTYPE_ACT1_BARRACKS:
				nLvlPrestId = LVLPREST_ACT1_BARRACKS_COURT_CONNECT;
				break;

			case LVLTYPE_ACT1_JAIL:
				nLvlPrestId = LVLPREST_ACT1_BARRACKS_FORGE_N;
				break;

			case LVLTYPE_ACT1_CATACOMBS:
				nLvlPrestId = LVLPREST_ACT1_CATHEDRAL;
				break;

			case LVLTYPE_ACT2_SEWER:
				nLvlPrestId = LVLPREST_ACT2_TOWN;
				break;

			case LVLTYPE_ACT2_TOMB:
				nLvlPrestId = LVLPREST_ACT2_DESERT_RUINS_ELDER;
				break;

			case LVLTYPE_ACT2_LAIR:
				nLvlPrestId = LVLPREST_ACT2_DURIELS_LAIR;
				break;

			case LVLTYPE_ACT3_KURAST:
				nLvlPrestId = LVLPREST_ACT3_TEMPLE_6;
				break;

			case LVLTYPE_ACT3_DUNGEON:
				nLvlPrestId = LVLPREST_ACT3_SPIDER_CHEST_NE;
				break;

			case LVLTYPE_ACT3_SEWER:
				nLvlPrestId = LVLPREST_ACT3_DUNGEON_TREASURE_2;
				break;

			case LVLTYPE_ACT4_LAVA:
				nLvlPrestId = LVLPREST_ACT4_LAVA_X;
				break;

			case LVLTYPE_ACT5_ICE_CAVES:
				nLvlPrestId = LVLPREST_ACT5_BARRICADE_16_SNOW;
				break;

			case LVLTYPE_ACT5_BAAL:
				nLvlPrestId = LVLPREST_ACT5_LAVA_NS;
				break;

			case LVLTYPE_ACT5_LAVA:
				nLvlPrestId = LVLPREST_ACT5_TEMPLE_SW_WAYPOINT;
				break;

			default:
				nLvlPrestId = nLevelDef;
				break;
			}

			if (nLevelDef > nLvlPrestId && nLevelDef < nLvlPrestId + 16)
			{
				pDrlgBuild = pLevel->pBuild;

				while (pDrlgBuild)
				{
					if (pDrlgBuild->nPreset == nLevelDef)
					{
						break;
					}

					pDrlgBuild = pDrlgBuild->pNext;
				}

				if (!pDrlgBuild)
				{
					pDrlgBuild = D2_ALLOC_STRC_POOL(pLevel->pDrlg->pMempool, D2DrlgBuildStrc);
					pDrlgBuild->nPreset = 0;
					pDrlgBuild->nDivisor = 0;
					pDrlgBuild->nRand = 0;
					pDrlgBuild->pNext = 0;
					pDrlgBuild->nPreset = nLevelDef;
					pDrlgBuild->nDivisor = nFiles;
					pDrlgBuild->nRand = SEED_RollLimitedRandomNumber(&pLevel->pSeed, nFiles);
					pDrlgBuild->pNext = pLevel->pBuild;
					pLevel->pBuild = pDrlgBuild;
				}

				pDrlgBuild->nRand = (pDrlgBuild->nRand + 1) % pDrlgBuild->nDivisor;
				pDrlgMap->nPickedFile = pDrlgBuild->nRand;
			}
		}
		else
		{
			pDrlgMap->nPickedFile = nPickedFile;
		}

		pTemp = DRLGPRESET_BuildArea(pLevel, pDrlgMap, 0, pDrlgRoom->nTileWidth <= 12 && pDrlgRoom->nTileHeight <= 12);

		for (D2DrlgOrthStrc* pDrlgOrth = pDrlgRoom->pDrlgOrth; pDrlgOrth; pDrlgOrth = pDrlgOrth->pNext)
		{
			if (pDrlgOrth->bInit == 1)
			{
				DRLGROOM_AllocDrlgOrthsForRooms(pTemp, pDrlgOrth->pDrlgRoom, pDrlgOrth->nDirection);
			}
		}

		DRLGROOM_FreeRoomEx(pDrlgRoom);

		pDrlgRoom = pNextRoomEx;
	}
}

//D2Common.0x6FD7D3D0
void __fastcall DRLGMAZE_ResetMazeRecord(D2DrlgLevelStrc* pLevel, BOOL bKeepMazeRecord)
{
	if (!bKeepMazeRecord)
	{
		pLevel->pMaze = NULL;
	}
}
