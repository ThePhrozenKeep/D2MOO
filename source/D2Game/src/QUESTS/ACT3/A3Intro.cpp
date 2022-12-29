#include "QUESTS/ACT3/A3Intro.h"

#include <DataTbls/MonsterIds.h>

#include "PLAYER/PlrIntro.h"
#include "QUESTS/Quests.h"


//D2Game.0x6FD37998
D2NPCMessageTableStrc gpAct3IntroNpcMessages[] =
{
	{
		{
			{ MONSTER_NATALYA, 453, 0, 0 },
			{ MONSTER_CAIN3, 458, 0, 0 },
			{ MONSTER_MESHIF2, 478, 0, 0 },
			{ MONSTER_ASHEARA, 490, 0, 0 },
			{ MONSTER_ALKOR, 501, 0, 0 },
			{ MONSTER_ORMUS, 514, 0, 0 },
		},
		6
	},
	{
		{
			{ MONSTER_NATALYA, 453, 0, 0 },
			{ MONSTER_MESHIF2, 479, 0, 0 },
			{ MONSTER_ASHEARA, 491, 0, 0 },
			{ MONSTER_ALKOR, 502, 0, 0 },
			{ MONSTER_ORMUS, 515, 0, 0 },
		},
		5
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FCA68D0
void __fastcall ACT3Intro_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT3Intro_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT3Intro_Callback11_ScrollMessage;
	pQuestData->pNPCMessages = gpAct3IntroNpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pQuestDataEx = nullptr;
	pQuestData->nQuest = QUESTSTATEFLAG_A3INTRO;
	pQuestData->pfStatusFilter = ACT3Intro_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT3Intro_ActiveFilterCallback;
}

//D2Game.0x6FCA6930
void __fastcall ACT3Intro_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	const int16_t nMessageIndex = pQuestArg->nMessageIndex;

	switch (pQuestArg->nNPCNo)
	{
	case MONSTER_ALKOR:
		if (nMessageIndex == 501 || nMessageIndex == 502)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_MESHIF2:
		if (nMessageIndex == 478 || nMessageIndex == 479)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_ASHEARA:
		if (nMessageIndex == 490 || nMessageIndex == 491)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_ORMUS:
		if (nMessageIndex == 514 || nMessageIndex == 515)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_NATALYA:
		if (nMessageIndex == 453)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_CAIN3:
		if (nMessageIndex == 458)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	default:
		return;
	}
}

//D2Game.0x6FCA6A00
void __fastcall ACT3Intro_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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

	int32_t nSpecialIntroClass = 0;
	switch (nNpcId)
	{
	case MONSTER_ALKOR:
		nSpecialIntroClass = PCLASS_NECROMANCER;
		break;

	case MONSTER_MESHIF2:
		nSpecialIntroClass = PCLASS_BARBARIAN;
		break;

	case MONSTER_ASHEARA:
		nSpecialIntroClass = PCLASS_AMAZON;
		break;

	case MONSTER_ORMUS:
		nSpecialIntroClass = PCLASS_PALADIN;
		break;

	case MONSTER_CAIN3:
	case MONSTER_NATALYA:
		nSpecialIntroClass = PCLASS_INVALID;
		break;

	default:
		return;
	}

	if (PLRINTRO_GetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, nNpcId))
	{
		return;
	}

	if (nPlayerClass == nSpecialIntroClass)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 1);
	}
	else
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 0);
	}
}

//D2Game.0x6FCA6B00
bool __fastcall ACT3Intro_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	*pStatus = 0;
	return 0;
}

//D2Game.0x6FCA6B10
bool __fastcall ACT3Intro_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	return nNpcId == MONSTER_CAIN3 && !PLRINTRO_GetQuestIntroFlag(pPlayer, pQuest->pGame, MONSTER_CAIN3);
}
