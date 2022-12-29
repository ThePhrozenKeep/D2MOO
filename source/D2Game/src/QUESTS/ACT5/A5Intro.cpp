#include "QUESTS/ACT5/A5Intro.h"

#include <DataTbls/MonsterIds.h>

#include "PLAYER/PlrIntro.h"
#include "QUESTS/Quests.h"


//D2Game.0x6FD3BAE8
D2NPCMessageTableStrc gpAct5IntroNpcMessages[] =
{
	{
		{
			{ MONSTER_DREHYA, 20014, 0, 0, },
			{ MONSTER_MALAH, 20037, 0, 0, },
			{ MONSTER_NIHLATHAK, 20053, 0, 0, },
			{ MONSTER_QUAL_KEHK, 20065, 0, 0, },
			{ MONSTER_CAIN6, 20003, 0, 0, },
		},
		5
	},
	{
		{
			{ MONSTER_DREHYA, 20014, 0, 0, },
			{ MONSTER_MALAH, 20039, 0, 0, },
			{ MONSTER_NIHLATHAK, 20054, 0, 0, },
			{ MONSTER_QUAL_KEHK, 20067, 0, 0, },
			{ MONSTER_CAIN6, 20003, 0, 0, },
		},
		5
	},
	{
		{
			{ MONSTER_DREHYA, 20014, 0, 0, },
			{ MONSTER_MALAH, 20038, 0, 0, },
			{ MONSTER_NIHLATHAK, 20055, 0, 0, },
			{ MONSTER_QUAL_KEHK, 20066, 0, 0, },
			{ MONSTER_CAIN6, 20003, 0, 0, },
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


//D2Game.0x6FCB0FD0
void __fastcall ACT5Intro_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT5Intro_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT5Intro_Callback11_ScrollMessage;
	pQuestData->pNPCMessages = gpAct5IntroNpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pQuestDataEx = 0;
	pQuestData->nQuest = 41;
	pQuestData->pfStatusFilter = ACT5Intro_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT5Intro_ActiveFilterCallback;
}

//D2Game.0x6FCB1030
void __fastcall ACT5Intro_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	const int16_t nMessageIndex = pQuestArg->nMessageIndex;

	switch (pQuestArg->nNPCNo)
	{
	case MONSTER_CAIN6:
	{
		if (nMessageIndex == 20003)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		return;
	}
	case MONSTER_DREHYA:
	{
		if (nMessageIndex == 20014)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		return;
	}
	case MONSTER_MALAH:
	{
		if (nMessageIndex == 20039 || nMessageIndex == 20038 || nMessageIndex == 20037)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);

			D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestArg->pGame, QUEST_A5Q1_SHENK);
			if (!pQuest || pQuest->fState)
			{
				return;
			}

			if (!pQuest->bNotIntro)
			{
				return;
			}

			pQuest->fState = 1;
		}
		return;
	}
	case MONSTER_NIHLATHAK:
	{
		if (nMessageIndex == 20054 || nMessageIndex == 20053 || nMessageIndex == 20055)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		return;
	}
	case MONSTER_QUAL_KEHK:
	{
		if (nMessageIndex == 20067 || nMessageIndex == 20066 || nMessageIndex == 20065)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		return;
	}
	}
}

//D2Game.0x6FCB1100
void __fastcall ACT5Intro_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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

	int32_t nSpecialIntroClass1 = 0;
	int32_t nSpecialIntroClass2 = 0;
	switch (nNpcId)
	{
	case MONSTER_MALAH:
		nSpecialIntroClass1 = PCLASS_BARBARIAN;
		nSpecialIntroClass2 = PCLASS_SORCERESS;
		break;
	case MONSTER_NIHLATHAK:
		nSpecialIntroClass1 = PCLASS_ASSASSIN;
		nSpecialIntroClass2 = PCLASS_NECROMANCER;
		break;
	case MONSTER_QUAL_KEHK:
		nSpecialIntroClass1 = PCLASS_DRUID;
		nSpecialIntroClass2 = PCLASS_PALADIN;
		break;
	case MONSTER_DREHYA:
	case MONSTER_CAIN6:
		nSpecialIntroClass1 = PCLASS_INVALID;
		nSpecialIntroClass2 = PCLASS_INVALID;
		break;
	default:
		return;
	}

	if (PLRINTRO_GetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, nNpcId))
	{
		return;
	}

	int32_t nIndex = 0;
	if (nPlayerClass == nSpecialIntroClass1)
	{
		nIndex = 1;
	}
	else if (nPlayerClass == nSpecialIntroClass2)
	{
		nIndex = 2;
	}

	QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
}

//D2Game.0x6FCB11D0
bool __fastcall ACT5Intro_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	return nNpcId == MONSTER_MALAH && !PLRINTRO_GetQuestIntroFlag(pPlayer, pQuest->pGame, MONSTER_MALAH);
}

//
bool __fastcall ACT5Intro_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	*pStatus = 0;
	return 0;
}
