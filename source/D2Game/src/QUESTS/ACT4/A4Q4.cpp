#include "QUESTS/ACT4/A4Q4.h"

#include <DataTbls/MonsterIds.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT4/A4Q2.h"


//D2Game.0x6FD3B830
D2NPCMessageTableStrc gpAct4Q4NpcMessages[] =
{
	{
		{
			{ MONSTER_MALACHAI, 668, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_MALACHAI, 669, 0, 1 },
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


//D2Game.0x6FCB0E50
void __fastcall ACT4Q4_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT4Q4_Callback00_NpcActivate;
	pQuestData->pNPCMessages = gpAct4Q4NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pQuestDataEx = 0;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A4Q4;
	pQuestData->pfStatusFilter = ACT4Q4_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT4Q4_ActiveFilterCallback;
}

//D2Game.0x6FCB0EB0
void __fastcall ACT4Q4_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_MALACHAI)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_PRIMARYGOALDONE))
	{
		D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A4Q2_DIABLO);
		if (!pQuest || !((D2Act4Quest2Strc*)pQuest->pQuestDataEx)->bDiabloKilled)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_MALACHAI, 1);
		}
	}
	else
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_MALACHAI, 0);
	}
}

//D2Game.0x6FCB0F50
bool __fastcall ACT4Q4_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_MALACHAI)
	{
		return false;
	}

	D2BitBufferStrc* pFlags = UNITS_GetPlayerData(pPlayer)->pQuestData[pQuest->pGame->nDifficulty];
	if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING) && !QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A4Q3, QFLAG_PRIMARYGOALDONE))
	{
		return true;
	}

	if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A4Q2, QFLAG_PRIMARYGOALDONE) && !QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A4Q2, QFLAG_REWARDGRANTED))
	{
		return true;
	}

	return false;
}

//
bool __fastcall ACT4Q4_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	return false;
}
