#include "QUESTS/ACT2/A2Q8.h"

#include <DataTbls/MonsterIds.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "QUESTS/Quests.h"


//D2Game.0x6FD376B8
D2NPCMessageTableStrc gpAct2Q8NpcMessages[] =
{
	{
		{
			{ MONSTER_ACT2GUARD5, 303, 0, 1, },
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


//D2Game.0x6FCA66F0
void __fastcall ACT2Q8_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT2Q8_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT2Q8_Callback00_NpcActivate;
	pQuestData->pNPCMessages = gpAct2Q8NpcMessages;
	pQuestData->bActive = 1;

	D2Act2Quest8Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act2Quest8Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestDataEx->unk0x00[0] = 0;
	pQuestDataEx->unk0x00[1] = 0;

	pQuestData->nQuest = QUESTSTATEFLAG_A2Q8;
	pQuestData->pfStatusFilter = ACT2Q8_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT2Q8_ActiveFilterCallback;
}

//D2Game.0x6FCA6770
void __fastcall ACT2Q8_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNPCNo == MONSTER_ACT2GUARD5 && pQuestArg->nMessageIndex == 303)
	{
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A2Q8, 0);
	}
}

//D2Game.0x6FCA67B0
void __fastcall ACT2Q8_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_ACT2GUARD5)
	{
		return;
	}

	D2QuestDataStrc* pQuestData13 = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A2Q6_DURIEL);
	if (!pQuestData13 || !pQuestData13->bNotIntro || pQuestData13->fState >= 2)
	{
		return;
	}

	D2QuestDataStrc* pQuestData10 = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A2Q3_TAINTEDSUN);
	if (pQuestData10 && pQuestData10->bNotIntro && pQuestData10->fState < 4)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];
	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDPENDING) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_ACT2GUARD5, 0);
	}
}

//D2Game.0x6FCA6850
bool __fastcall ACT2Q8_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_ACT2GUARD5)
	{
		return false;
	}

	D2BitBufferStrc* pFlags = UNITS_GetPlayerData(pPlayer)->pQuestData[pQuest->pGame->nDifficulty];
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pQuest->pGame, 13);
	if (!pQuestData)
	{
		return false;
	}

	return pQuestData->bNotIntro && pQuestData->fState < 2
		&& QUESTS_GetGlobalState(pQuest->pGame, QUESTSTATEFLAG_A2Q3, QFLAG_PRIMARYGOALDONE)
		&& !QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDPENDING)
		&& !QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED);
}

//
bool __fastcall ACT2Q8_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	return false;
}
