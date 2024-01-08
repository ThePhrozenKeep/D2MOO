#include "QUESTS/ACT3/A3Q7.h"

#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <D2Dungeon.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterSpawn.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FCAD210
int32_t __fastcall ACT3Q7_GetWandererCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pCoord)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q7_DARKWANDERER);
	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		return 0;
	}

	D2Act3Quest7Strc* pQuestDataEx = (D2Act3Quest7Strc*)pQuestData->pQuestDataEx;

	if (!pQuestDataEx->bDarkWandererInitialized)
	{
		return 0;
	}

	pCoord->nX = pQuestDataEx->nDarkWandererX;
	pCoord->nY = pQuestDataEx->nDarkWandererY;

	if (pQuestDataEx->bWandererCoordsCalculated)
	{
		return 1;
	}

	pCoord->nY = pQuestDataEx->nDarkWandererY - 20;

	pQuestDataEx->bWandererCoordsCalculated = 1;

	if (!UNITS_TestCollisionByCoordinates(pUnit, pCoord->nX, pCoord->nY, 0x3C01))
	{
		pQuestDataEx->nDarkWandererX = pCoord->nX;
		pQuestDataEx->nDarkWandererY = pCoord->nY;
		return 1;
	}

	pCoord->nY += 9;
	if (!UNITS_TestCollisionByCoordinates(pUnit, pCoord->nX, pCoord->nY, 0x3C01))
	{
		pQuestDataEx->nDarkWandererX = pCoord->nX;
		pQuestDataEx->nDarkWandererY = pCoord->nY;
		return 1;
	}

	pCoord->nX += 2;
	if (!UNITS_TestCollisionByCoordinates(pUnit, pCoord->nX, pCoord->nY, 0x3C01))
	{
		pQuestDataEx->nDarkWandererX = pCoord->nX;
		pQuestDataEx->nDarkWandererY = pCoord->nY;
		return 1;
	}

	pCoord->nX -= 4;
	if (!UNITS_TestCollisionByCoordinates(pUnit, pCoord->nX, pCoord->nY, 0x3C01))
	{
		pQuestDataEx->nDarkWandererX = pCoord->nX;
		pQuestDataEx->nDarkWandererY = pCoord->nY;
		return 1;
	}

	pCoord->nX = pQuestDataEx->nDarkWandererX;
	pCoord->nY = pQuestDataEx->nDarkWandererY - 8;
	if (!UNITS_TestCollisionByCoordinates(pUnit, pCoord->nX, pCoord->nY, 0x3C01))
	{
		pQuestDataEx->nDarkWandererX = pCoord->nX;
		pQuestDataEx->nDarkWandererY = pCoord->nY;
		return 1;
	}

	pCoord->nY += 5;
	pQuestDataEx->nDarkWandererX = pCoord->nX;
	pQuestDataEx->nDarkWandererY = pCoord->nY;
	return 1;
}

//D2Game.0x6FCAD360
void __fastcall OBJECTS_InitFunction43_DarkWanderer(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q7_DARKWANDERER);
	if (!pQuestData)
	{
		return;
	}

	D2Act3Quest7Strc* pQuestDataEx = (D2Act3Quest7Strc*)pQuestData->pQuestDataEx;
	if (!pQuestDataEx->bPrimaryGoalOpen)
	{
		pQuestDataEx->bDarkWandererInitialized = 1;
		return;
	}

	pQuestDataEx->nDarkWandererX = pOp->nX + 7;
	pQuestDataEx->nDarkWandererY = pOp->nY;

	D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(pOp->pRoom, pOp->nX + 7, pOp->nY);
	if (D2GAME_SpawnMonster_6FC69F10(pOp->pGame, pRoom, pQuestDataEx->nDarkWandererX, pQuestDataEx->nDarkWandererY, MONSTER_DARKWANDERER, 1, -1, 0))
	{
		pQuestDataEx->bPrimaryGoalOpen = 0;
	}
	pQuestDataEx->bDarkWandererInitialized = 1;
}

//D2Game.0x6FCAD460
void __fastcall ACT3Q7_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pNPCMessages = nullptr;
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT3Q7_Callback13_PlayerStartedGame;
	pQuestData->bActive = 1;

	D2Act3Quest7Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act3Quest7Strc);
	memset(pQuestDataEx, 0x00, sizeof(D2Act3Quest7Strc));
	pQuestData->pQuestDataEx = pQuestDataEx;

	pQuestData->nQuestFilter = 16;
	pQuestData->pfStatusFilter = ACT3Q7_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT3Q7_ActiveFilterCallback;

	pQuestDataEx->bPrimaryGoalOpen = 1;
}

//D2Game.0x6FCAD4F0
void __fastcall ACT3Q7_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A3Q7, QFLAG_REWARDGRANTED) == 1)
	{
		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A3Q7, QFLAG_PRIMARYGOALDONE);
		((D2Act3Quest7Strc*)pQuestData->pQuestDataEx)->bPrimaryGoalOpen = 0;
	}
}

//D2Game.0x6FCAD530
void __fastcall ACT3Q7_CreateVileDogSpawnTimer(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q7_DARKWANDERER);
	if (!pQuestData)
	{
		return;
	}

	D2Act3Quest7Strc* pQuestDataEx = (D2Act3Quest7Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bVileDogsSpawned || pQuestDataEx->bVileDogSpawnTimerCreated)
	{
		return;
	}

	pQuestDataEx->bVileDogSpawnTimerCreated = 1;
	if (pUnit)
	{
		pQuestDataEx->nDarkWandererGUID = pUnit->dwUnitId;
	}
	else
	{
		pQuestDataEx->nDarkWandererGUID = -1;
	}

	QUESTS_CreateTimer(pQuestData, ACT3Q7_SpawnVileDogs, 2);
}

//D2Game.0x6FCAD590
bool __fastcall ACT3Q7_SpawnVileDogs(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	static const D2CoordStrc pAdjustCoords[] =
	{
		{ -3, -3 },
		{ -3,  0 },
		{ -3,  3 },
		{  0, -3 },
		{  0,  3 },
		{  3, -3 },
		{  3,  0 },
		{ -3,  3 },
	};

	D2Act3Quest7Strc* pQuestDataEx = (D2Act3Quest7Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bVileDogsSpawned)
	{
		return 1;
	}

	SUNIT_IterateUnitsOfType(pGame, 0, 0, ACT3Q7_UnitIterate_SetRewardGranted);

	pQuestDataEx->bVileDogsSpawned = 1;
	pQuestDataEx->bVileDogSpawnTimerCreated = 0;

	D2UnitStrc* pDarkWanderer = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pQuestDataEx->nDarkWandererGUID);
	if (!pDarkWanderer)
	{
		return 1;
	}

	D2SeedStrc* pSeed = QUESTS_GetGlobalSeed(pGame);

	D2ActiveRoomStrc* pCurrentRoom = UNITS_GetRoom(pDarkWanderer);
	//UNITS_GetCoords(pDarkWanderer, &pCoord); // TODO: Was this meant to be used on pCoords?

	D2CoordStrc pCoords = {};

	for (int32_t i = SEED_RollRandomNumber(pSeed) & 1; i < ARRAY_SIZE(pAdjustCoords); ++i)
	{
		pCoords.nY += pAdjustCoords[i].nY;
		pCoords.nX += pAdjustCoords[i].nX;
		D2ActiveRoomStrc* pRoom = nullptr;
		QUESTS_GetFreePosition(pCurrentRoom, &pCoords, 3, 0x3F11, &pRoom, 11);

		if (pRoom)
		{
			SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_VILE_DOG_AFTERGLOW, pCoords.nX, pCoords.nY, pGame, pRoom, 1, 0, 0);
		}
	}

	return 1;
}

//D2Game.0x6FCAD690
int32_t __fastcall ACT3Q7_UnitIterate_SetRewardGranted(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q7, QFLAG_REWARDGRANTED) || !UNITS_GetRoom(pUnit))
	{
		return 0;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (nLevelId && DRLG_GetActNoFromLevelId(nLevelId) == ACT_III)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q7, QFLAG_REWARDGRANTED);
	}

	return 0;
}

//
bool __fastcall ACT3Q7_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	return false;
}

//
bool __fastcall ACT3Q7_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	return false;
}
