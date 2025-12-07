#include "QUESTS/ACT4/A4Q0.h"

#include <DataTbls/MonsterIds.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "QUESTS/Quests.h"


//D2Game.0x6FD3A7D8
D2NPCMessageTableStrc gpAct4Q0NpcMessages[] =
{
	{
		{
			{ MONSTER_TYRAEL2, 664, 0, 1, },
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


//D2Game.0x6FCAD6F0
void __fastcall ACT4Q0_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT4Q0_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT4Q0_Callback11_ScrollMessage;
	pQuestData->pNPCMessages = gpAct4Q0NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pQuestDataEx = nullptr;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A4Q0;
	pQuestData->pfStatusFilter = ACT4Q0_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT4Q0_ActiveFilterCallback;
}

//D2Game.0x6FCAD750
void __fastcall ACT4Q0_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNPCNo == MONSTER_TYRAEL2 && pQuestArg->nMessageIndex == 664)
	{
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A4Q0, QFLAG_REWARDGRANTED);
	}
}

//D2Game.0x6FCAD790
void __fastcall ACT4Q0_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2UnitStrc* pTarget = pQuestArg->pTarget;

	if (!pTarget || pTarget->dwClassId != MONSTER_TYRAEL2)
	{
		return;
	}

	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A4Q0, QFLAG_REWARDGRANTED))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, pTarget->dwClassId, 0);
	}
}

//D2Game.0x6FCAD800
bool __fastcall ACT4Q0_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	return nNpcId == MONSTER_TYRAEL2 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q0, QFLAG_REWARDGRANTED);
}

//
bool __fastcall ACT4Q0_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	return false;
}
