#include "QUESTS/ACT1/A1Q7.h"

#include <DataTbls/MonsterIds.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "QUESTS/Quests.h"


//D2Game.0x6FD34038
D2NPCMessageTableStrc gpAct1Q7NpcMessages[] =
{
	{
		{
			{ MONSTER_NAVI, 59, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_NAVI, 60, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_NAVI, 61, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_NAVI, 62, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_NAVI, 63, 0, 1 },
		},
		1
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FC9EF40
void __fastcall ACT1Q7_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT1Q7_Callback00_NpcActivate;
	//pQuestData->pfCallback[8] = (QUESTCALLBACK)D2GAME_OBJECTS_InitFunction05_6FCA6660;
	pQuestData->pNPCMessages = gpAct1Q7NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A1Q7;
	pQuestData->pfStatusFilter = ACT1Q7_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT1Q7_ActiveFilterCallback;
}

//D2Game.0x6FC9EFA0
void __fastcall ACT1Q7_Callback00_NpcActivate(D2QuestDataStrc* pQuest, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_NAVI)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pQuest->pGame, QUEST_A1Q1_DENOFEVIL);
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTS_GetGlobalState(pQuestArg->pGame, QUESTSTATEFLAG_A1Q1, QFLAG_PRIMARYGOALDONE)
		|| !pQuestData->bNotIntro
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDGRANTED)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDPENDING))
	{
		QUESTS_InitScrollTextChain(pQuest, pQuestArg->pTextControl, MONSTER_NAVI, ITEMS_RollRandomNumber(&pQuestArg->pPlayer->pSeed) % 3 + 2);
	}
	else
	{
		QUESTS_InitScrollTextChain(pQuest, pQuestArg->pTextControl, MONSTER_NAVI, ITEMS_RollRandomNumber(&pQuestArg->pPlayer->pSeed) & 1);
	}
}

//D2Game.0x6FC9F0A0
bool __fastcall ACT1Q7_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_NAVI || !QUESTS_GetQuestData(pQuest->pGame, QUEST_A1Q1_DENOFEVIL)->bNotIntro || QUESTS_GetGlobalState(pQuest->pGame, QUESTSTATEFLAG_A1Q1, QFLAG_PRIMARYGOALDONE))
	{
		return false;
	}

	D2BitBufferStrc* pFlags = UNITS_GetPlayerData(pPlayer)->pQuestData[pQuest->pGame->nDifficulty];

	return !QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDGRANTED)
		&& !QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A1Q1, QFLAG_PRIMARYGOALDONE)
		&& !QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDPENDING);
}

//
bool __fastcall ACT1Q7_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	return false;
}
