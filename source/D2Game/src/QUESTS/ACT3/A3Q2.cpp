#include "QUESTS/ACT3/A3Q2.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <D2Items.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/Items.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT2/A2Q6.h"
#include "QUESTS/ACT3/A3Q5.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD382E8
D2NPCMessageTableStrc gpAct3Q2NpcMessages[] =
{
	{
		{
			{ MONSTER_CAIN3, 543, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 545, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 544, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 546, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 547, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 548, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 543, 0, 2 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 545, 0, 2 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 544, 0, 2 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 546, 0, 2 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 547, 0, 2 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 548, 0, 2 },
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


//D2Game.0x6FCA7B70
bool __fastcall ACT3Q2_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_CAIN3)
	{
		return false;
	}

	if (pQuest->fState >= 1u && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_STARTED))
	{
		return true;
	}

	int32_t nFlailPartsAvailable = 0;
	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 1fq'))
	{
		++nFlailPartsAvailable;
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM1))
		{
			return true;
		}
	}

	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' yeq'))
	{
		++nFlailPartsAvailable;
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_LEAVETOWN))
		{
			return true;
		}
	}

	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' rhq'))
	{
		++nFlailPartsAvailable;
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM2))
		{
			return true;
		}
	}

	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' rbq'))
	{
		++nFlailPartsAvailable;
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_ENTERAREA))
		{
			return true;
		}
	}

	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 2fq') && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM3))
	{
		D2QuestDataStrc* pQuestData19 = QUESTS_GetQuestData(pQuest->pGame, QUEST_A3Q5_TRAVINCAL);
		if (!pQuestData19 || !((D2Act3Quest5Strc*)pQuestData19->pQuestDataEx)->bOrbSmashed)
		{
			return true;
		}
	}

	if (nFlailPartsAvailable == 4 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM1))
	{
		return true;
	}

	return false;
}

//D2Game.0x6FCA7CD0
void __fastcall ACT3Q2_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pNPCMessages = gpAct3Q2NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fLastState = 1;
	pQuestData->fState = 0;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A3Q2;
	pQuestData->nInitNo = 0;
	pQuestData->pfStatusFilter = ACT3Q2_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT3Q2_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT3Q2_SeqCallback;
	pQuestData->nSeqId = 15;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT3Q2_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT3Q2_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT3Q2_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT3Q2_Callback13_PlayerStartedGame;
	//pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = (QUESTCALLBACK)D2GAME_OBJECTS_InitFunction05_Door_6FCA6660;
	pQuestData->pfCallback[QUESTEVENT_ITEMPICKEDUP] = ACT3Q2_Callback04_ItemPickedUp;

	D2Act3Quest2Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act3Quest2Strc);
	memset(pQuestDataEx, 0x00, sizeof(D2Act3Quest2Strc));
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestDataEx->nSewerStairsObjectMode = 0;
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->tPlayerGUIDs);
}

//D2Game.0x6FCA7DA0
void __fastcall ACT3Q2_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_CAIN3)
	{
		return;
	}

	if (pQuestData->fState >= 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_STARTED))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 0);
		return;
	}

	bool bCubedFlailAvailable = false;
	if (ITEMS_FindQuestItem(pQuestData->pGame, pQuestArg->pPlayer, ' 2fq'))
	{
		bCubedFlailAvailable = true;
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM3))
		{
			D2QuestDataStrc* pQuestData19 = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A3Q5_TRAVINCAL);
			if (!pQuestData19 || !((D2Act3Quest5Strc*)pQuestData19->pQuestDataEx)->bOrbSmashed)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 5);
				return;
			}
		}
	}

	int32_t nFlailPartsAvailable = 0;
	bool bBaseFlailAvailable = false;
	if (ITEMS_FindQuestItem(pQuestData->pGame, pQuestArg->pPlayer, ' 1fq'))
	{
		++nFlailPartsAvailable;

		bBaseFlailAvailable = true;
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM1))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 4);
			return;
		}
	}

	bool bEyeAvailable = false;
	if (ITEMS_FindQuestItem(pQuestData->pGame, pQuestArg->pPlayer, ' yeq'))
	{
		++nFlailPartsAvailable;

		bEyeAvailable = true;
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_LEAVETOWN))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 1);
			return;
		}
	}

	bool bHeartAvailable = false;
	if (ITEMS_FindQuestItem(pQuestData->pGame, pQuestArg->pPlayer, ' rhq'))
	{
		++nFlailPartsAvailable;

		bHeartAvailable = true;
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM2))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 2);
			return;
		}
	}

	bool bBrainAvailable = false;
	if (ITEMS_FindQuestItem(pQuestData->pGame, pQuestArg->pPlayer, ' rbq'))
	{
		++nFlailPartsAvailable;

		bBrainAvailable = true;
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_ENTERAREA))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 3);
			return;
		}
	}

	if (nFlailPartsAvailable <= 0)
	{
		if (bCubedFlailAvailable)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 11);
		}
		else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_STARTED))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 6);
		}
		return;
	}

	if (bBaseFlailAvailable)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 10);
		return;
	}

	if (bHeartAvailable)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 8);
		return;
	}

	if (bEyeAvailable)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 7);
		return;
	}

	if (bBrainAvailable)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 9);
	}
	else
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 6);
	}
}

//D2Game.0x6FCA7FB0
void __fastcall ACT3Q2_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel == LEVEL_GREATMARSH && pQuestData->bNotIntro && pQuestData->fState < 3)
	{
		QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
	}
}

//D2Game.0x6FCA7FE0
bool __fastcall ACT3Q2_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	*pStatus = 0;

	if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A2COMPLETED, QFLAG_REWARDGRANTED))
	{
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q2, QFLAG_STARTED) || pQuest->bNotIntro && pQuest->fState >= 2)
	{
		*pStatus = 1;
	}

	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 2fq'))
	{
		D2QuestDataStrc* pQuestData19 = QUESTS_GetQuestData(pQuest->pGame, QUEST_A3Q5_TRAVINCAL);
		if (pQuestData19 && ((D2Act3Quest5Strc*)pQuestData19->pQuestDataEx)->bOrbSmashed)
		{
			*pStatus = 12;
		}
		else
		{
			*pStatus = 6;
		}

		return true;
	}

	int32_t nFlailPartsAvailable = 0;
	bool bBaseFlailAvailable = false;
	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 1fq'))
	{
		++nFlailPartsAvailable;
		bBaseFlailAvailable = true;
	}

	bool bEyeAvailable = false;
	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' yeq'))
	{
		++nFlailPartsAvailable;
		bEyeAvailable = true;
	}

	bool bHeartAvailable = false;
	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' rhq'))
	{
		++nFlailPartsAvailable;
		bHeartAvailable = true;
	}

	bool bBrainAvailable = false;
	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' rbq'))
	{
		++nFlailPartsAvailable;
		bBrainAvailable = true;
	}

	if (nFlailPartsAvailable == 4)
	{
		*pStatus = 2 * (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM1) == 0) + 5;
		return true;
	}

	if (!nFlailPartsAvailable)
	{
		if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q2, QFLAG_STARTED))
		{
			*pStatus = 1;
			return true;
		}

		*pStatus = pQuest->fState > 1;
		return true;
	}

	if (!bEyeAvailable)
	{
		*pStatus = 1;
		return true;
	}

	if (!bBrainAvailable)
	{
		*pStatus = 2;
		return true;
	}

	if (!bHeartAvailable)
	{
		*pStatus = 4;
		return true;
	}

	*pStatus = 4 * (bBaseFlailAvailable != 0) + 3;
	return true;

}

//D2Game.0x6FCA81E0
void __fastcall ACT3Q2_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (pQuestArg->nNPCNo == MONSTER_CAIN3)
	{
		switch (pQuestArg->nMessageIndex)
		{
		case 543:
			QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_STARTED);
			((D2Act3Quest2Strc*)pQuestData->pQuestDataEx)->bInitiallyTalkedToCain = 1;
			pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT3Q2_Callback02_NpcDeactivate;
			break;

		case 545:
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_LEAVETOWN);
			break;

		case 544:
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM2);
			break;

		case 546:
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_ENTERAREA);
			break;

		case 547:
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM1);
			break;

		case 548:
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM3);
			break;

		default:
			return;
		}
	}
}

//D2Game.0x6FCA82D0
void __fastcall ACT3Q2_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_CAIN3)
	{
		return;
	}

	if (((D2Act3Quest2Strc*)pQuestData->pQuestDataEx)->bInitiallyTalkedToCain == 1)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT3Q2_UnitIterate_StatusCyclerEx, 1);
		pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	}
}

//D2Game.0x6FCA8310
int32_t __fastcall ACT3Q2_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_COMPLETEDBEFORE))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A3Q2_KHALIMFLAIL);
	}

	return 0;
}

//D2Game.0x6FCA8360
bool __fastcall ACT3Q2_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (pQuestData->fState != 5 && pQuestData->bNotIntro)
	{
		if (!pQuestData->fState)
		{
			pQuestData->fState = 1;
		}
		return true;
	}

	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, pQuestData->nSeqId);
	if (!pQuest)
	{
		return false;
	}

	if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
	{
		FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
		exit(-1);
	}
	return pQuestData->pfSeqFilter(pQuest);
}

//D2Game.0x6FCA83D0
void __fastcall ACT3Q2_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2COMPLETED, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED))
	{
		return;
	}

	const uint32_t dwItemCode = ITEMS_GetBaseCode(pQuestArg->pTarget);

	//QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_STARTED);
	//QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_LEAVETOWN);
	//QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_ENTERAREA);
	//QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM2);

	bool bBaseFlailAvailable = false;
	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 1fq'))
	{
		bBaseFlailAvailable = true;
	}

	//ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 2fq');

	bool bEyeAvailable = false;
	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' yeq'))
	{
		bEyeAvailable = true;
	}

	bool bBrainAvailable = false;
	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' rbq'))
	{
		bBrainAvailable = true;
	}

	bool bHeartAvailable = false;
	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' rhq'))
	{
		bHeartAvailable = true;
	}

	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pQuestArg->pPlayer, __FILE__, __LINE__);

	D2GSPacketSrv5D packet5D = {};
	packet5D.nHeader = 0x5D;
	packet5D.nQuestId = QUEST_A3Q2_KHALIMFLAIL;
	packet5D.nFlags = (uint8_t)pQuestData->dwFlags;
	packet5D.field_4 = 0;

	if (dwItemCode == ' 2fq')
	{
		if (pQuestData->fLastState)
		{
			pQuestData->fLastState = 7;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2COMPLETED, QFLAG_REWARDGRANTED))
		{
			return;
		}

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM3))
		{
			packet5D.nState = 5;
		}
		else
		{
			packet5D.nState = 6;
		}
	}
	else if (dwItemCode == ' 1fq')
	{
		if (!bEyeAvailable)
		{
			packet5D.nState = 1;
		}
		else if (!bBrainAvailable)
		{
			packet5D.nState = 2;
		}
		else if (!bHeartAvailable)
		{
			packet5D.nState = 4;
		}
		else if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM1))
		{
			packet5D.nState = 5;
		}
		else
		{
			packet5D.nState = 7;
		}
	}
	else if (dwItemCode == ' yeq')
	{
		if (!bBrainAvailable)
		{
			packet5D.nState = 2;
		}
		else if (!bBaseFlailAvailable)
		{
			packet5D.nState = 3;
		}
		else if (!bHeartAvailable)
		{
			packet5D.nState = 4;
		}
		else
		{
			packet5D.nState = 7;
		}
	}
	else if (dwItemCode == ' rhq')
	{
		if (!bBrainAvailable)
		{
			packet5D.nState = 2;
		}
		else if (!bBaseFlailAvailable)
		{
			packet5D.nState = 3;
		}
		else if (!bEyeAvailable)
		{
			packet5D.nState = 1;
		}
		else
		{
			packet5D.nState = 7;
		}
	}
	else if (dwItemCode == ' rbq')
	{
		if (!bHeartAvailable)
		{
			packet5D.nState = 4;
		}
		else if (!bBaseFlailAvailable)
		{
			packet5D.nState = 3;
		}
		else if (!bEyeAvailable)
		{
			packet5D.nState = 1;
		}
		else
		{
			packet5D.nState = 7;
		}
	}
	else
	{
		return;
	}

	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5D, sizeof(packet5D));
}

//D2Game.0x6FCA8780
void __fastcall ACT3Q2_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED))
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_CUSTOM3))
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 7;
	}
	else
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_STARTED))
		{
			pQuestData->fState = 2;
			pQuestData->fLastState = 7;
		}

		D2Act3Quest2Strc* pQuestDataEx = (D2Act3Quest2Strc*)pQuestData->pQuestDataEx;

		if (pQuestDataEx->nEyesDropped + pQuestDataEx->nBrainsDropped + pQuestDataEx->nHeartsDropped + pQuestDataEx->nFlailsDropped)
		{
			pQuestData->fState = 2;
			pQuestData->fLastState = 7;
		}
	}
}

//D2Game.0x6FCA87F0
int32_t __fastcall OBJECTS_OperateFunction44_SewerStairs(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	if (pObject->dwAnimMode == OBJMODE_OPENED)
	{
		D2GAME_WarpPlayerToTile_6FCA5B20(pOp);
	}

	return 1;
}

//D2Game.0x6FCA8810
int32_t __fastcall OBJECTS_OperateFunction45_SewerLever(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
	{
		return 1;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q2_KHALIMFLAIL);
	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		return 1;
	}

	D2Act3Quest2Strc* pQuestDataEx = (D2Act3Quest2Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bSewerStairsInitialized != 1 || !SUNIT_GetServerUnit(pOp->pGame, UNIT_OBJECT, pQuestDataEx->nSewerStairsObjectGUID))
	{
		return 1;
	}

	UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

	D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
	EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, pOp->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);

	pQuestDataEx->nSewerStairsObjectMode = OBJMODE_OPENED;
	EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_QUESTFN, pOp->pGame->dwGameFrame + 30, 0, 0);

	QUESTS_TriggerFX(pQuestData->pGame, 9);
	return 1;
}

//D2Game.0x6FCA8940
void __fastcall OBJECTS_InitFunction41_SewerStairs(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q2_KHALIMFLAIL);
	if (!pQuestData)
	{
		return;
	}

	D2Act3Quest2Strc* pQuestDataEx = (D2Act3Quest2Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bSewerStairsInitialized = 1;

	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}
	pQuestDataEx->nSewerStairsObjectGUID = nUnitId;

	if (pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nSewerStairsObjectMode);
	}
	else
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
}

//D2Game.0x6FCA8990
void __fastcall OBJECTS_InitFunction42_SewerLever(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q2_KHALIMFLAIL);
	if (pQuestData && !pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
}

//D2Game.0x6FCA89C0
void __fastcall ACT3Q2_UpdateKhalimItemCounts(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	//D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	//if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED))
	//{
	//	QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDPENDING); // TODO: Maybe meant to be set?
	//}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q2_KHALIMFLAIL);
	if (!pQuestData)
	{
		return;
	}

	D2Act3Quest2Strc* pQuestDataEx = (D2Act3Quest2Strc*)pQuestData->pQuestDataEx;

	--pQuestDataEx->nEyesDropped;
	--pQuestDataEx->nHeartsDropped;
	--pQuestDataEx->nBrainsDropped;
	--pQuestDataEx->nFlailsDropped;
	++pQuestDataEx->nCubedFlails;
}

//D2Game.0x6FCA8A30
int32_t __fastcall OBJECTS_OperateFunction57_KhalimChest(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	if (!QUESTS_SetObjectSelection(pOp))
	{
		return 1;
	}

	D2SeedStrc* pSeed = QUESTS_GetGlobalSeed(pOp->pGame);

	const int32_t nCounter = ITEMS_RollRandomNumber(pSeed) % 5 + 5;
	for (int32_t i = 0; i < nCounter; ++i)
	{
		OBJMODE_DropItemWithCodeAndQuality(pOp->pGame, pObject, ' dlg', 2);
	}
	pObject->dwDropItemCode = ' rhq';

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q2_KHALIMFLAIL);
	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
		return 1;
	}

	D2Act3Quest2Strc* pQuestDataEx = (D2Act3Quest2Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->nItemDropCount = 0;
	SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pPlayer, ACT3Q2_UnitIterate_DetermineKhalimHeartDropCount);

	for (int32_t i = 0; i < pQuestDataEx->nItemDropCount; ++i)
	{
		int32_t nItemLevel = 0;
		if (D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pObject, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 1))
		{
			pQuestDataEx->bHeartDropped = 1;
			++pQuestDataEx->nHeartsDropped;
		}
	}

	OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
	return 1;
}

//D2Game.0x6FCA8B10
int32_t __fastcall ACT3Q2_UnitIterate_DetermineKhalimHeartDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (ITEMS_FindQuestItem(pGame, pUnit, ' rhq') || ITEMS_FindQuestItem(pGame, pUnit, ' 2fq'))
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q2_KHALIMFLAIL);
	if (pQuestData)
	{
		++((D2Act3Quest2Strc*)pQuestData->pQuestDataEx)->nItemDropCount;
	}
	return 0;
}

//D2Game.0x6FCA8B60
int32_t __fastcall OBJECTS_OperateFunction58_KhalimChest(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	if (!QUESTS_SetObjectSelection(pOp))
	{
		return 1;
	}

	D2SeedStrc* pSeed = QUESTS_GetGlobalSeed(pOp->pGame);

	const int32_t nCounter = ITEMS_RollRandomNumber(pSeed) % 5 + 5;
	for (int32_t i = 0; i < nCounter; ++i)
	{
		OBJMODE_DropItemWithCodeAndQuality(pOp->pGame, pObject, ' dlg', 2);
	}
	pObject->dwDropItemCode = ' yeq';

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q2_KHALIMFLAIL);
	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
		return 1;
	}

	D2Act3Quest2Strc* pQuestDataEx = (D2Act3Quest2Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->nItemDropCount = 0;
	SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pPlayer, ACT3Q2_UnitIterate_DetermineKhalimEyeDropCount);

	for (int32_t i = 0; i < pQuestDataEx->nItemDropCount; ++i)
	{
		int32_t nItemLevel = 0;
		if (D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pObject, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 1))
		{
			pQuestDataEx->bEyeDropped = 1;
			++pQuestDataEx->nEyesDropped;
		}
	}

	OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
	return 1;
}

//D2Game.0x6FCA8C40
int32_t __fastcall ACT3Q2_UnitIterate_DetermineKhalimEyeDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (ITEMS_FindQuestItem(pGame, pUnit, ' yeq') || ITEMS_FindQuestItem(pGame, pUnit, ' 2fq'))
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q2_KHALIMFLAIL);
	if (pQuestData)
	{
		++((D2Act3Quest2Strc*)pQuestData->pQuestDataEx)->nItemDropCount;
	}

	return 0;
}

//D2Game.0x6FCA8C90
int32_t __fastcall OBJECTS_OperateFunction59_KhalimChest(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	if (!QUESTS_SetObjectSelection(pOp))
	{
		return 1;
	}

	D2SeedStrc* pSeed = QUESTS_GetGlobalSeed(pOp->pGame);

	const int32_t nCounter = ITEMS_RollRandomNumber(pSeed) % 5 + 5;
	for (int32_t i = 0; i < nCounter; ++i)
	{
		OBJMODE_DropItemWithCodeAndQuality(pOp->pGame, pObject, ' dlg', 2);
	}
	pObject->dwDropItemCode = ' rbq';

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q2_KHALIMFLAIL);
	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
		return 1;
	}

	D2Act3Quest2Strc* pQuestDataEx = (D2Act3Quest2Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->nItemDropCount = 0;
	SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pPlayer, ACT3Q2_UnitIterate_DetermineKhalimBrainDropCount);

	for (int32_t i = 0; i < pQuestDataEx->nItemDropCount; ++i)
	{
		int32_t nItemLevel = 0;
		if (D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pObject, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 1))
		{
			pQuestDataEx->bBrainDropped = 1;
			++pQuestDataEx->nBrainsDropped;
		}
	}

	OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
	return 1;
}

//D2Game.0x6FCA8D70
int32_t __fastcall ACT3Q2_UnitIterate_DetermineKhalimBrainDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (ITEMS_FindQuestItem(pGame, pUnit, ' rbq') || ITEMS_FindQuestItem(pGame, pUnit, ' 2fq'))
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q2_KHALIMFLAIL);
	if (pQuestData)
	{
		++((D2Act3Quest2Strc*)pQuestData->pQuestDataEx)->nItemDropCount;
	}

	return 0;
}
