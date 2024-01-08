#include "QUESTS/ACT1/A1Q0.h"

#include <DataTbls/MonsterIds.h>
#include <D2QuestRecord.h>
#include <Units/Units.h>

#include "GAME/Game.h"
#include "QUESTS/Quests.h"


//D2Game.0x6FD31CE8
D2NPCMessageTableStrc gpAct1Q0NpcMessages[] =
{
	{
		{
			{ MONSTER_WARRIV1, 0, 0, 0 },
		},
		1
	},
	{
		{
			{ MONSTER_WARRIV1, 1, 0, 0 },
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


//D2Game.0x6FC977D0
void __fastcall ACT1Q0_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT1Q0_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT1Q0_Callback11_ScrollMessage;
	pQuestData->pNPCMessages = gpAct1Q0NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pQuestDataEx = nullptr;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A1Q0;
	pQuestData->pfStatusFilter = ACT1Q0_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT1Q0_ActiveFilterCallback;
}

//D2Game.0x6FC97830
void __fastcall ACT1Q0_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNPCNo == MONSTER_WARRIV1 && (pQuestArg->nMessageIndex == 0 || pQuestArg->nMessageIndex == 1))
	{
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], QUESTSTATEFLAG_A1Q0, QFLAG_REWARDGRANTED);
	}
}

//D2Game.0x6FC97870
void __fastcall ACT1Q0_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_WARRIV1)
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], QUESTSTATEFLAG_A1Q0, QFLAG_REWARDGRANTED))
	{
		return;
	}

	if (pQuestArg->pPlayer && pQuestArg->pPlayer->dwClassId == PCLASS_PALADIN)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_WARRIV1, 1);
	}
	else
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_WARRIV1, 0);
	}
}

//D2Game.0x6FC978F0
bool __fastcall ACT1Q0_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	return nNpcId == MONSTER_WARRIV1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q0, QFLAG_REWARDGRANTED);
}

//
bool __fastcall ACT1Q0_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	return false;
}
