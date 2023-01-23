#include "QUESTS/ACT2/A2Q0.h"

#include <DataTbls/MonsterIds.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT1/A1Q4.h"


//D2Game.0x6FD348E8
D2NPCMessageTableStrc gpAct2Q0NpcMessages[] =
{
	{
		{
			{ MONSTER_JERHYN, 253, 0, 0 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN2, 125, 0, 0 },
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


//D2Game.0x6FC9F350
void __fastcall ACT2Q0_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pQuestDataEx = nullptr;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT2Q0_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT2Q0_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT2Q0_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT2Q0_Callback13_PlayerStartedGame;
	pQuestData->pNPCMessages = gpAct2Q0NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->nQuest = QUESTSTATEFLAG_A2Q0;
	pQuestData->pfStatusFilter = ACT2Q0_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT2Q0_ActiveFilterCallback;

	D2Act2Quest0Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act2Quest0Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;

	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->tPlayerGUIDs);
}

//D2Game.0x6FC9F3F0
void __fastcall ACT2Q0_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNPCNo == MONSTER_JERHYN)
	{
		if (pQuestArg->nMessageIndex == 253)
		{
			QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A2Q0, QFLAG_REWARDGRANTED);
			QUESTS_SetGlobalState(pQuestArg->pGame, QUESTSTATEFLAG_A2Q0, QFLAG_PRIMARYGOALDONE);
		}
	}
	else if (pQuestArg->nNPCNo == MONSTER_CAIN2)
	{
		if (pQuestArg->nMessageIndex == 125)
		{
			D2Act2Quest0Strc* pQuestDataEx = (D2Act2Quest0Strc*)pQuestData->pQuestDataEx;
			if (pQuestArg->pPlayer)
			{
				QUESTS_AddPlayerGUID(&pQuestDataEx->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
			}
			else
			{
				QUESTS_AddPlayerGUID(&pQuestDataEx->tPlayerGUIDs, -1);
			}
		}
	}
}

//D2Game.0x6FC9F460
void __fastcall ACT2Q0_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (nNpcId == MONSTER_JERHYN)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q0, QFLAG_REWARDGRANTED))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_JERHYN, 0);
		}
	}
	else if (nNpcId == MONSTER_CAIN2)
	{
		D2Act2Quest0Strc* pQuestDataEx = (D2Act2Quest0Strc*)pQuestData->pQuestDataEx;
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_COMPLETEDNOW) != 1)
		{
			return;
		}

		int32_t nUnitId = -1;
		if (pQuestArg->pPlayer)
		{
			nUnitId = pQuestArg->pPlayer->dwUnitId;
		}

		if (QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->tPlayerGUIDs, nUnitId))
		{
			return;
		}

		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN2, 1);

		D2Act1Quest4Strc* pQuestDataExA1Q4 = (D2Act1Quest4Strc*)QUESTS_GetQuestData(pQuestData->pGame, QUEST_A1Q4_CAIN)->pQuestDataEx;
		QUESTS_FastRemovePlayerGUID(&pQuestDataExA1Q4->pQuestGUID, nUnitId);
	}
}

//D2Game.0x6FC9F510
bool __fastcall ACT2Q0_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	return nNpcId == MONSTER_JERHYN && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q0, QFLAG_REWARDGRANTED);
}

//D2Game.0x6FC9F540
void __fastcall ACT2Q0_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act2Quest0Strc* pQuestDataEx = (D2Act2Quest0Strc*)pQuestData->pQuestDataEx;

	if (pQuestArg->pPlayer)
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
	}
	else
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, -1);
	}
}

//D2Game.0x6FC9F560
void __fastcall ACT2Q0_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A2Q0, QFLAG_REWARDGRANTED) == 1)
	{
		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q0, QFLAG_PRIMARYGOALDONE);
	}
}

//
bool __fastcall ACT2Q0_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	return false;
}
