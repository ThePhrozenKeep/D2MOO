#include "QUESTS/ACT2/A2Intro.h"

#include <DataTbls/MonsterIds.h>

#include "PLAYER/PlrIntro.h"
#include "QUESTS/Quests.h"


//D2Game.0x6FD34698
D2NPCMessageTableStrc gpAct2IntroNpcMessages[] =
{
	{
		{
			{ MONSTER_ELZIX, 203, 0, 0, },
			{ MONSTER_GREIZ, 190, 0, 0, },
			{ MONSTER_ATMA, 221, 0, 0, },
			{ MONSTER_GEGLASH, 230, 0, 0, },
			{ MONSTER_MESHIF1, 241, 0, 0, },
			{ MONSTER_FARA, 263, 0, 0, },
			{ MONSTER_WARRIV2, 215, 0, 0, },
			{ MONSTER_LYSANDER, 274, 0, 0, },
			{ MONSTER_DROGNAN, 285, 0, 0, },
		},
		9
	},
	{
		{
			{ MONSTER_ELZIX, 204, 0, 0, },
			{ MONSTER_GREIZ, 190, 0, 0, },
			{ MONSTER_ATMA, 221, 0, 0, },
			{ MONSTER_GEGLASH, 231, 0, 0, },
			{ MONSTER_MESHIF1, 242, 0, 0, },
			{ MONSTER_FARA, 264, 0, 0, },
			{ MONSTER_WARRIV2, 215, 0, 0, },
			{ MONSTER_LYSANDER, 274, 0, 0, },
			{ MONSTER_DROGNAN, 286, 0, 0, },
		},
		9
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FC9F120
void __fastcall ACT2Intro_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT2Intro_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT2Intro_Callback11_ScrollMessage;
	pQuestData->pNPCMessages = gpAct2IntroNpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;
	pQuestData->fLastState = 0;
	pQuestData->pQuestDataEx = nullptr;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A2INTRO;
	pQuestData->pfStatusFilter = ACT2Intro_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT2Intro_ActiveFilterCallback;
}

//D2Game.0x6FC9F180
void __fastcall ACT2Intro_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	const int16_t nMessageIndex = pQuestArg->nMessageIndex;

	switch (pQuestArg->nNPCNo)
	{
	case MONSTER_ELZIX:
		if (nMessageIndex == 203 || nMessageIndex == 204)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_GEGLASH:
		if (nMessageIndex == 230 || nMessageIndex == 231)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_MESHIF1:
		if (nMessageIndex == 241 || nMessageIndex == 242)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_FARA:
		if (nMessageIndex == 263 || nMessageIndex == 264)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_DROGNAN:
		if (nMessageIndex == 285 || nMessageIndex == 286)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_GREIZ:
		if (nMessageIndex == 190)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_WARRIV2:
		if (nMessageIndex == 215)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	case MONSTER_LYSANDER:
		if (nMessageIndex == 274)
		{
			PLRINTRO_SetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, pQuestArg->nNPCNo);
		}
		break;

	default:
		return;
	}
}

//D2Game.0x6FC9F260
void __fastcall ACT2Intro_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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
	case MONSTER_ELZIX:
		nSpecialIntroClass = PCLASS_NECROMANCER;
		break;

	case MONSTER_GEGLASH:
		nSpecialIntroClass = PCLASS_BARBARIAN;
		break;

	case MONSTER_MESHIF1:
		nSpecialIntroClass = PCLASS_AMAZON;
		break;

	case MONSTER_FARA:
		nSpecialIntroClass = PCLASS_PALADIN;
		break;

	case MONSTER_DROGNAN:
		nSpecialIntroClass = PCLASS_SORCERESS;
		break;

	case MONSTER_WARRIV2:
	case MONSTER_GREIZ:
	case MONSTER_LYSANDER:
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

//
bool __fastcall ACT2Intro_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	*pStatus = 0;
	return false;
}

//
bool __fastcall ACT2Intro_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	return false;
}
