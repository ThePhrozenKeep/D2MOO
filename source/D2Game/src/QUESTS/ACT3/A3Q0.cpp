#include "QUESTS/ACT3/A3Q0.h"

#include <DataTbls/MonsterIds.h>
#include <Drlg/D2DrlgPreset.h>
#include <D2QuestRecord.h>

#include "AI/AiGeneral.h"
#include "GAME/Game.h"
#include "MONSTER/MonsterSpawn.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD37BE8
D2NPCMessageTableStrc gpAct3Q0NpcMessages[] =
{
	{
		{
			{ MONSTER_HRATLI, 465, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_HRATLI, 466, 0, 1 },
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


//D2Game.0x6FCA6B40
void __fastcall ACT3Q0_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT3Q0_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT3Q0_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT3Q0_Callback13_PlayerStartedGame;
	pQuestData->pNPCMessages = gpAct3Q0NpcMessages;
	pQuestData->bActive = 1;

	D2Act3Quest0Strc* pQuestDataEx = (D2Act3Quest0Strc*)FOG_AllocPool(pQuestData->pGame->pMemoryPool, sizeof(D2Act3Quest0Strc), __FILE__, __LINE__, 0);
	memset(pQuestDataEx, 0x00, sizeof(D2Act3Quest0Strc));
	pQuestData->pQuestDataEx = pQuestDataEx;

	pQuestData->nQuest = QUESTSTATEFLAG_A3Q0;
	pQuestData->pfStatusFilter = ACT3Q0_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT3Q0_ActiveFilterCallback;
}

//D2Game.0x6FCA6BD0
void __fastcall ACT3Q0_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNPCNo != MONSTER_HRATLI)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];
	if (pQuestArg->nMessageIndex == 465 || pQuestArg->nMessageIndex == 466)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q0, QFLAG_REWARDGRANTED);
		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A3Q0, QFLAG_PRIMARYGOALDONE);
	}
}

//D2Game.0x6FCA6C30
void __fastcall ACT3Q0_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_HRATLI)
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A3Q0, QFLAG_REWARDGRANTED))
	{
		return;
	}

	if (pQuestArg->pPlayer && pQuestArg->pPlayer->dwClassId == PCLASS_SORCERESS)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_HRATLI, 1);
	}
	else
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_HRATLI, 0);
	}
}

//D2Game.0x6FCA6CB0
bool __fastcall ACT3Q0_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	return nNpcId == MONSTER_HRATLI && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q0, QFLAG_REWARDGRANTED);
}

//D2Game.0x6FCA6CE0
void __fastcall ACT3Q0_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A3Q0, QFLAG_REWARDGRANTED) == 1)
	{
		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A3Q0, QFLAG_PRIMARYGOALDONE);
	}
}

//D2Game.0x6FCA6D20
void __fastcall OBJECTS_InitFunction49_HratliStart(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q0_HRATLIGOSSIP);
	if (!pQuestData || QUESTS_GetGlobalState(pOp->pGame, QUESTSTATEFLAG_A3Q0, QFLAG_PRIMARYGOALDONE))
	{
		return;
	}

	D2Act3Quest0Strc* pQuestDataEx = (D2Act3Quest0Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bHratliStartCreated && SUNIT_GetServerUnit(pOp->pGame, UNIT_MONSTER, pQuestDataEx->nHratliGUID))
	{
		return;
	}

	D2UnitStrc* pHratli = sub_6FC69F10(pOp->pGame, pOp->pRoom, pOp->nX, pOp->nY, MONSTER_HRATLI, 1, -1, 0);
	if (pHratli)
	{
		pQuestDataEx->bHratliStartCreated = 1;
		pQuestDataEx->nHratliGUID = pHratli->dwUnitId;
	}
}

//D2Game.0x6FCA6DA0
void __fastcall OBJECTS_InitFunction50_HratliEnd(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q0_HRATLIGOSSIP);
	if (!pQuestData)
	{
		return;
	}

	D2Act3Quest0Strc* pQuestDataEx = (D2Act3Quest0Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bHratliEndObjectInitialized = 1;
	pQuestDataEx->nHratliX = pOp->nX;
	pQuestDataEx->nHratliY = pOp->nY;

	if (!QUESTS_GetGlobalState(pOp->pGame, QUESTSTATEFLAG_A3Q0, QFLAG_PRIMARYGOALDONE) || pQuestDataEx->bHratliEndCreated)
	{
		return;
	}

	if (pQuestDataEx->bHratliStartCreated && SUNIT_GetServerUnit(pOp->pGame, UNIT_MONSTER, pQuestDataEx->nHratliGUID))
	{
		pQuestDataEx->bHratliStartIsPresent = 1;
		return;
	}

	D2UnitStrc* pHratli = sub_6FC69F10(pOp->pGame, pOp->pRoom, pOp->nX, pOp->nY, MONSTER_HRATLI, 1, -1, 0);
	if (!pHratli)
	{
		return;
	}

	pQuestDataEx->bHratliEndCreated = 1;
	pQuestDataEx->nHratliGUID = pHratli->dwUnitId;
	pHratli->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;

	if (pQuestDataEx->pHratliMapAI && pQuestDataEx->pHratliMapAI->pPosition && !pQuestDataEx->bHratliMapAIChanged)
	{
		DRLGPRESET_ChangeMapAI(&pQuestDataEx->pHratliMapAI, AIGENERAL_GetMapAiFromUnit(pHratli));
		pQuestDataEx->bHratliMapAIChanged = 1;
	}
}

//
bool __fastcall ACT3Q0_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	return false;
}
