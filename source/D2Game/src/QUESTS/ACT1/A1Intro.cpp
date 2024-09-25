#include "QUESTS/ACT1/A1Intro.h"

#include <DataTbls/MonsterIds.h>
#include <D2QuestRecord.h>

#include "PLAYER/PlrIntro.h"
#include "QUESTS/Quests.h"


//D2Game.0x6FD31A98
D2NPCMessageTableStrc gpAct1IntroNpcMessages[] =
{
	{
		{
			{ MONSTER_AKARA, 11, 0, 0 },
			{ MONSTER_GHEED, 45, 0, 0 },
			{ MONSTER_CHARSI, 36, 0, 0 },
			{ MONSTER_KASHYA, 24, 0, 0 },
		},
		4
	},
	{
		{
			{ MONSTER_AKARA, 12, 0, 0 },
			{ MONSTER_GHEED, 46, 0, 0 },
			{ MONSTER_CHARSI, 37, 0, 0 },
			{ MONSTER_KASHYA, 25, 0, 0 },
		},
		4
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FC975E0
void __fastcall ACT1Intro_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT1Intro_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT1Intro_Callback11_ScrollMessage;
	pQuestData->pNPCMessages = gpAct1IntroNpcMessages;

	pQuestData->bActive = true;
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pQuestDataEx = nullptr;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A1INTRO;

	pQuestData->pfStatusFilter = ACT1Intro_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT1Intro_ActiveFilterCallback;
}

//D2Game.0x6FC97640
void __fastcall ACT1Intro_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg)
	{
		return;
	}

	const int16_t nMessageIndex = pQuestArg->nMessageIndex;

	switch (pQuestArg->nNPCNo)
	{
	case MONSTER_AKARA:
		if (nMessageIndex != 11 && nMessageIndex != 12)
		{
			return;
		}
		break;

	case MONSTER_GHEED:
		if (nMessageIndex != 45 && nMessageIndex != 46)
		{
			return;
		}
		break;

	case MONSTER_CHARSI:
		if (nMessageIndex != 36 && nMessageIndex != 37)
		{
			return;
		}
		break;

	case MONSTER_KASHYA:
		if (nMessageIndex != 24 && nMessageIndex != 25)
		{
			return;
		}
		break;

	default:
		return;
	}

	PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
}

//D2Game.0x6FC976C0
void __fastcall ACT1Intro_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	int32_t nPlayerClass = -1;
	if (pQuestArg->pPlayer)
	{
		nPlayerClass = pQuestArg->pPlayer->dwClassId;
	}

	int32_t nSpecialClass = 0;
	switch (nNpcId)
	{
	case MONSTER_KASHYA:
		nSpecialClass = PCLASS_AMAZON;
		break;

	case MONSTER_AKARA:
		nSpecialClass = PCLASS_SORCERESS;
		break;

	case MONSTER_GHEED:
		nSpecialClass = PCLASS_NECROMANCER;
		break;

	case MONSTER_CHARSI:
		nSpecialClass = PCLASS_BARBARIAN;
		break;

	default:
		return;
	}

	if (PLRINTRO_GetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, nNpcId))
	{
		return;
	}

	if (nPlayerClass == nSpecialClass)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 1);
	}
	else
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 0);
	}
}

//D2Game.0x6FC97770
bool __fastcall ACT1Intro_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	return QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pPlayer)->pQuestData[pQuest->pGame->nDifficulty], QUESTSTATEFLAG_A1Q1, QFLAG_REWARDGRANTED) != 1
		&& nNpcId == MONSTER_AKARA
		&& !PLRINTRO_GetQuestIntroFlag(pPlayer, pQuest->pGame, MONSTER_AKARA);
}

//
bool __fastcall ACT1Intro_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	*pStatus = 0;
	return false;
}
