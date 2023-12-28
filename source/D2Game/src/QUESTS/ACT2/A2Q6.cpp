#include "QUESTS/ACT2/A2Q6.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2QuestRecord.h>

#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/Items.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlrTrade.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "SKILLS/Skills.h"


//D2Game.0x6FD36BC8
D2NPCMessageTableStrc gpAct2Q6NpcMessages[] =
{
	{
		{
			{ MONSTER_JERHYN, 430, 0, 1, },
		},
		1
	},
	{
		{
			{ MONSTER_ATMA, 434, 0, 2, },
			{ MONSTER_WARRIV2, 433, 0, 2, },
			{ MONSTER_GREIZ, 441, 0, 2, },
			{ MONSTER_ELZIX, 432, 0, 2, },
			{ MONSTER_JERHYN, 431, 0, 2, },
			{ MONSTER_DROGNAN, 439, 0, 2, },
			{ MONSTER_LYSANDER, 438, 0, 2, },
			{ MONSTER_CAIN2, 440, 0, 2, },
			{ MONSTER_MESHIF1, 436, 0, 2, },
			{ MONSTER_GEGLASH, 435, 0, 2, },
			{ MONSTER_FARA, 437, 0, 2, },
		},
		11
	},
	{
		{
			{ MONSTER_TYRAEL1, 302, 0, 1, },
		},
		1
	},
	{
		{
			{ MONSTER_ATMA, 445, 0, 2, },
			{ MONSTER_WARRIV2, 446, 0, 2, },
			{ MONSTER_GREIZ, 451, 0, 2, },
			{ MONSTER_ELZIX, 443, 0, 2, },
			{ MONSTER_DROGNAN, 449, 0, 2, },
			{ MONSTER_JERHYN, 442, 0, 1, },
			{ MONSTER_TYRAEL1, 302, 0, 1, },
			{ MONSTER_LYSANDER, 444, 0, 2, },
			{ MONSTER_CAIN2, 452, 0, 2, },
			{ MONSTER_MESHIF1, 450, 0, 2, },
			{ MONSTER_GEGLASH, 448, 0, 2, },
			{ MONSTER_FARA, 447, 0, 2, },
		},
		12
	},
	{
		{
			{ MONSTER_ATMA, 445, 0, 2, },
			{ MONSTER_WARRIV2, 446, 0, 2, },
			{ MONSTER_GREIZ, 451, 0, 2, },
			{ MONSTER_ELZIX, 443, 0, 2, },
			{ MONSTER_DROGNAN, 449, 0, 2, },
			{ MONSTER_JERHYN, 442, 0, 2, },
			{ MONSTER_TYRAEL1, 302, 0, 1, },
			{ MONSTER_LYSANDER, 444, 0, 2, },
			{ MONSTER_CAIN2, 452, 0, 2, },
			{ MONSTER_GEGLASH, 448, 0, 2, },
			{ MONSTER_FARA, 447, 0, 2, },
		},
		11
	},
	{
		{
			{ MONSTER_MESHIF1, 450, 0, 1, },
		},
		1
	},
	{
		{
			{ MONSTER_ATMA, 445, 0, 1, },
			{ MONSTER_WARRIV2, 446, 0, 1, },
			{ MONSTER_DROGNAN, 449, 0, 1, },
			{ MONSTER_LYSANDER, 444, 0, 1, },
			{ MONSTER_CAIN2, 452, 0, 1, },
			{ MONSTER_FARA, 447, 0, 1, },
		},
		6
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FCA4650
void __fastcall ACT2Q6_AttachCompletionSoundToPlayers(D2GameStrc* pGame)
{
	SUNIT_IterateUnitsOfType(pGame, 0, nullptr, ACT2Q6_UnitIterate_AttachCompletionSound);
}

//D2Game.0x6FCA4660
int32_t __fastcall ACT2Q6_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE))
	{
		SUNIT_AttachSound(pUnit, 54, pUnit);
	}

	return 0;
}

//D2Game.0x6FCA46A0
void __fastcall ACT2Q6_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL))
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) == 1
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN) == 1
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (DRLG_GetActNoFromLevelId(nLevelId) != ACT_II)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN);
	CLIENTS_UpdateCharacterProgression(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 2, pGame->nDifficulty);
}

//D2Game.0x6FCA4750
bool __fastcall ACT2Q6_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A1COMPLETED, QFLAG_REWARDGRANTED))
	{
		*pStatus = 0;
		return true;
	}

	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuest->pQuestDataEx;
	if (pQuestDataEx->bHoradricStaffIsMissing)
	{
		*pStatus = pQuestDataEx->nQuestStatus;
		return true;
	}

	*pStatus = 0;

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED))
	{
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN) == 1)
	{
		*pStatus = 5;
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA) == 1)
	{
		*pStatus = 6;
		return true;
	}

	if (!pQuest->bNotIntro)
	{
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM1) == 1)
	{
		*pStatus = pQuest->fLastState;
		return true;
	}

	if (pQuest->fState < 3)
	{
		*pStatus = pQuest->fLastState;
		return true;
	}

	*pStatus = 12;
	return true;
}

//D2Game.0x6FCA4820
void __fastcall ACT2Q6_UnitIterate_SetCustom1Flag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL))
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) == 1
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN) == 1
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA) == 1
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM1) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (DRLG_GetActNoFromLevelId(nLevelId) == ACT_II)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM1);
	}
}

//D2Game.0x6FCA48B0
bool __fastcall ACT2Q6_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	switch (nNpcId)
	{
	case MONSTER_JERHYN:
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) == 1)
		{
			return false;
		}

		if (pQuest->fState == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA))
		{
			return true;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN) == 1)
		{
			return true;
		}
		break;
	}
	case MONSTER_MESHIF1:
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA) == 1)
		{
			return true;
		}
		break;
	}
	case MONSTER_TYRAEL1:
	{
		D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuest->pQuestDataEx;
		if (pQuest->bNotIntro != 1 || pQuestDataEx->bDurielKilled != 1)
		{
			return false;
		}

		if (pQuestDataEx->bPortalToLutGholeinOpened == 0)
		{
			return true;
		}
		break;
	}
	default:
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE))
		{
			return false;
		}

		switch (nNpcId)
		{
		case MONSTER_ATMA:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM2))
			{
				return true;
			}
			break;

		case MONSTER_WARRIV2:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM3))
			{
				return true;
			}
			break;

		case MONSTER_DROGNAN:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM4))
			{
				return true;
			}
			break;

		case MONSTER_LYSANDER:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM5))
			{
				return true;
			}
			break;

		case MONSTER_CAIN2:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM6))
			{
				return true;
			}
			break;

		case MONSTER_FARA:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM7))
			{
				return true;
			}
			break;

		default:
			return false;
		}
		break;
	}
	}

	return false;
}

//D2Game.0x6FCA49C0
void __fastcall ACT2Q6_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT2Q6_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT2Q6_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT2Q6_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT2Q6_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT2Q6_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT2Q6_Callback13_PlayerStartedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERJOINEDGAME] = ACT2Q6_Callback14_PlayerJoinedGame;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT2Q6_Callback08_MonsterKilled;
	pQuestData->pNPCMessages = gpAct2Q6NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;
	
	D2Act2Quest6Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act2Quest6Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->nQuest = QUESTSTATEFLAG_A2Q6;
	pQuestData->pfStatusFilter = ACT2Q6_StatusFilterCallback;
	pQuestData->nInitNo = 4;
	pQuestData->pfActiveFilter = ACT2Q6_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT2Q6_SeqCallback;
	memset(pQuestDataEx, 0x00, sizeof(D2Act2Quest6Strc));
	pQuestDataEx->bDurielKilled = 0;
	pQuestDataEx->unk0x02 = 0;
	pQuestDataEx->bTimerActive = 0;
	pQuestDataEx->nTyraelsDoorObjectMode = 0;
	pQuestDataEx->unk0x1C = 0;
	pQuestDataEx->nStaffTombLevelId = 0;
}

//D2Game.0x6FCA4AB0
void __fastcall ACT2Q6_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4
	};

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
		if (nNpcId == MONSTER_TYRAEL1)
		{
			if (((D2Act2Quest6Strc*)pQuestData->pQuestDataEx)->nTyraelsDoorObjectMode == OBJMODE_OPENED)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_TYRAEL1, 2);
			}
			return;
		}
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE))
	{
		switch (nNpcId)
		{
		case MONSTER_ATMA:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM2))
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_ATMA, 6);
				return;
			}
			break;

		case MONSTER_WARRIV2:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM3))
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_WARRIV2, 6);
				return;
			}
			break;

		case MONSTER_DROGNAN:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM4))
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_DROGNAN, 6);
				return;
			}
			break;

		case MONSTER_LYSANDER:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM5))
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_LYSANDER, 6);
				return;
			}
			break;

		case MONSTER_CAIN2:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM6))
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN2, 6);
				return;
			}
			break;

		case MONSTER_FARA:
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM7))
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_FARA, 6);
				return;
			}
			break;

		default:
			break;
		}
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA))
	{
		if (nNpcId == MONSTER_MESHIF1)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_MESHIF1, 5);
		}
		else
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
		}
	}
	else if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
	}
	else if (pQuestData->fState)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE))
		{
			return;
		}

		if (pQuestData->fState >= 4 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE))
		{
			return;
		}

		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex == 1 && nNpcId == MONSTER_DROGNAN)
		{
			if (QUESTS_GetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q4, QFLAG_PRIMARYGOALDONE) != 1)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
			}
		}
		else if (nIndex != -1 && nIndex < 8)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//D2Game.0x6FC96C80
void __fastcall ACT2Q6_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_RemovePlayerGUID(pQuestData, pQuestArg);
}

//D2Game.0x6FCA4D50
void __fastcall ACT2Q6_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget)
	{
		return;
	}

	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->pTarget->dwClassId == MONSTER_TYRAEL1)
	{
		if (pQuestDataEx->bTyraelActivated == 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 4, 0, ACT2Q6_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->bTyraelActivated = 0;
			pQuestData->pfCallback[2] = 0;
		}
	}
	else if (pQuestArg->pTarget->dwClassId == MONSTER_JERHYN)
	{
		if (pQuestDataEx->bInitJerhynActivated == 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 1, 0, ACT2Q6_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->bInitJerhynActivated = 0;
		}
		else
		{
			if (pQuestDataEx->bEndJerhynActivated != 1)
			{
				return;
			}

			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 6, 0, ACT2Q6_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->bEndJerhynActivated = 0;
		}

		pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q6_UnitIterate_UpdateQuestStateFlags);
	}
}

//D2Game.0xFCA4E00
int32_t __fastcall ACT2Q6_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A2Q6_DURIEL);
	}

	return 0;
}

//D2Game.0x6FCA4E70
int32_t __fastcall ACT2Q6_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL);
	if (pQuestData && pQuestData->fState >= 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_STARTED);
	}

	return 0;
}

//D2Game.0x6FCA4EC0
void __fastcall ACT2Q6_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (pQuestArg->nNPCNo == MONSTER_TYRAEL1)
	{
		if (pQuestData->bNotIntro && pQuestArg->nMessageIndex == 302 && !pQuestDataEx->bPortalToLutGholeinOpened)
		{
			pQuestDataEx->bPortalIsOpening = 1;

			if (D2GAME_CreatePortalObject_6FD13DF0(pQuestData->pGame, pQuestArg->pPlayer, UNITS_GetRoom(pQuestArg->pPlayer), CLIENTS_GetUnitX(pQuestArg->pPlayer), CLIENTS_GetUnitY(pQuestArg->pPlayer), LEVEL_LUTGHOLEIN, 0, OBJECT_TOWN_PORTAL, 0))
			{
				QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
				SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT2Q6_UnitIterate_SetLeaveTownFlag);
				SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT2Q6_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
				SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT2Q6_UnitIterate_SetCompletionFlag);

				pQuestDataEx->bPortalToLutGholeinOpened = 1;
				pQuestDataEx->bTyraelActivated = 1;
				pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT2Q6_Callback02_NpcDeactivate;
			}

			pQuestDataEx->bPortalIsOpening = 0;
		}
	}
	else if (pQuestArg->nNPCNo == MONSTER_JERHYN)
	{
		if (pQuestArg->nMessageIndex == 430)
		{
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
			QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A2Q3_TAINTEDSUN);
			pQuestDataEx->bInitJerhynActivated = 1;
			pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT2Q6_Callback02_NpcDeactivate;
			if (pQuest)
			{
				if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
				{
					FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
					exit(-1);
				}
				pQuestData->pfSeqFilter(pQuest);
			}
		}
		else
		{
			if (pQuestArg->nMessageIndex == 442 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN))
			{
				if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE))
				{
					QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
				}
				pQuestDataEx->bEndJerhynActivated = 1;
				pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT2Q6_Callback02_NpcDeactivate;
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA);
				QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN);
				QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
			}
		}
		return;
	}
	else if (pQuestArg->nNPCNo == MONSTER_MESHIF1)
	{
		if (pQuestArg->nMessageIndex == 450 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA))
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED))
			{
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' tsh');
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' piv');
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' fsm');
			}

			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE))
			{
				QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE);
				QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 13, 0, ACT2Q6_UnitIterate_StatusCyclerEx, 1);
			}

			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED);
			QUESTS_UpdatePlayerFlags(pQuestData->pGame, pQuestArg->pPlayer);
			QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA);

			if (pQuestArg->pPlayer)
			{
				QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
			}
			else
			{
				QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, -1);
			}
		}

		return;
	}

	switch (pQuestArg->nMessageIndex)
	{
	case 445:
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM2);
		break;

	case 446:
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM3);
		break;

	case 449:
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM4);
		break;

	case 444:
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM5);
		break;

	case 452:
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM6);
		break;

	case 447:
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM7);
		break;

	default:
		return;
	}
}

//D2Game.0x6FCA52D0
int32_t __fastcall ACT2Q6_UnitIterate_SetLeaveTownFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return 0;
	}

	if (!QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL))
	{
		FOG_DisplayAssert("pQuestInfo != nullptr", __FILE__, __LINE__);
		exit(-1);
	}
	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE)
		&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN)
		&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA)
		&& nLevelId == LEVEL_DURIELSLAIR)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN);
		CLIENTS_UpdateCharacterProgression(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 2, pGame->nDifficulty);
	}

	return 0;
}

//D2Game.0x6FCA53B0
int32_t __fastcall ACT2Q6_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT2Q6_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCA5400
int32_t __fastcall ACT2Q6_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) != 1
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN) != 1
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A2Q6_DURIEL, 0);
	}

	return 0;
}

//D2Game.0x6FCA5470
void __fastcall ACT2Q6_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_DebugOutput(pQuestData->pGame, "killed duriel for quest", __FILE__, __LINE__);
	if (pQuestData->bNotIntro)
	{
		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		QUESTS_CreateTimer(pQuestData, ACT2Q6_TImer_StatusCycler, 8);

		if (pQuestArg->pPlayer)
		{
			D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED))
			{
				if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN)
					&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA)
					&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM1))
				{
					QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM1);
					
					const int16_t nPartyId = SUNIT_GetPartyId(pQuestArg->pPlayer);
					if (nPartyId != -1)
					{
						PARTY_IteratePartyMembers(pQuestArg->pGame, nPartyId, ACT2Q6_UnitIterate_SetCustom1Flag, 0);
					}
					//D2Game_10034_Return(61);
				}
			}
		}
	}

	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = nullptr;
	pQuestDataEx->bDurielKilled = 1;
	pQuestDataEx->pDurielRoom = UNITS_GetRoom(pQuestArg->pTarget);
	QUESTS_TriggerFX(pQuestData->pGame, 8);
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q6_UnitIterate_SetCustom1FlagForPartyMembers);

	if (pQuestDataEx->bTyraelsDoorInitialized)
	{
		D2UnitStrc* pTyraelsDoor = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nTyraelsDoorGUID);
		if (pTyraelsDoor)
		{
			UNITS_ChangeAnimMode(pTyraelsDoor, OBJMODE_OPERATING);
			D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pTyraelsDoor->dwClassId);
			EVENT_SetEvent(pQuestData->pGame, pTyraelsDoor, UNITEVENTCALLBACK_ENDANIM, pQuestData->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);
		}
	}
	pQuestDataEx->nTyraelsDoorObjectMode = OBJMODE_OPENED;
}

//D2Game.0x6FCA55E0
bool __fastcall ACT2Q6_TImer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;

	if (pQuestData->fLastState != 5 && pQuestData->fLastState != 4 && pQuestData->fLastState != 3)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 3, 0, ACT2Q6_UnitIterate_StatusCyclerEx, 1);
	}

	pQuestDataEx->unk0x00 = 0;

	return true;
}

//D2Game.0x6FCA5620
int32_t __fastcall ACT2Q6_UnitIterate_SetCustom1FlagForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_DURIELSLAIR)
	{
		return 0;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM1))
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_CUSTOM1);
	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT2Q6_UnitIterate_SetCustom1Flag, 0);
	}

	return 0;
}

//D2Game.0x6FCA56C0
void __fastcall ACT2Q6_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel < LEVEL_LUTGHOLEIN || pQuestArg->nNewLevel >= LEVEL_KURASTDOCKTOWN)
	{
		return;
	}

	if (pQuestArg->nOldLevel == LEVEL_LUTGHOLEIN)
	{
		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);
	}

	if (!pQuestData->bNotIntro)
	{
		return;
	}

	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bDurielKilled)
	{
		if (pQuestArg->nNewLevel != LEVEL_DURIELSLAIR || pQuestData->fState > 1)
		{
			return;
		}

		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);

		if (pQuestData->fLastState > 1)
		{
			return;
		}

		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 2, 0, ACT2Q6_UnitIterate_StatusCyclerEx, 0);

		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) == 1)
		{
			return;
		}

		D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestArg->pGame, QUEST_A2Q6_DURIEL);
		if (pQuest && pQuest->fState >= 2)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_STARTED);
		}
		return;
	}

	if (!pQuestDataEx->nStaffTombLevelId)
	{
		pQuestDataEx->nStaffTombLevelId = DUNGEON_GetHoradricStaffTombLevelId(pQuestArg->pGame->pAct[ACT_II]);
	}

	if (!pQuestDataEx->nStaffTombLevelId)
	{
		return;
	}

	if (pQuestArg->nNewLevel == pQuestDataEx->nStaffTombLevelId)
	{
		if (pQuestData->fState == 2)
		{
			return;
		}

		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);

		if (pQuestData->fLastState <= 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 2, 0, ACT2Q6_UnitIterate_StatusCyclerEx, 0);
		}

		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) == 1)
		{
			return;
		}

		D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestArg->pGame, QUEST_A2Q6_DURIEL);
		if (pQuest && pQuest->fState >= 2)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_STARTED);
		}
		return;
	}
	else if (pQuestArg->nNewLevel == LEVEL_CANYONOFTHEMAGI)
	{
		if (!pQuestData->fState)
		{
			QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);

			D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) != 1)
			{
				D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestArg->pGame, QUEST_A2Q6_DURIEL);
				if (pQuest && pQuest->fState >= 2)
				{
					QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_STARTED);
				}
			}
		}

		if (!pQuestData->fLastState)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 1, 0, ACT2Q6_UnitIterate_StatusCyclerEx, 1);
		}

		return;
	}

	if (pQuestArg->nNewLevel != LEVEL_DURIELSLAIR || pQuestData->fState > 1)
	{
		return;
	}

	QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);

	if (pQuestData->fLastState > 1)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 2, 0, ACT2Q6_UnitIterate_StatusCyclerEx, 0);

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) == 1)
	{
		return;
	}

	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestArg->pGame, QUEST_A2Q6_DURIEL);
	if (pQuest && pQuest->fState >= 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_STARTED);
	}
}

//D2Game.0x6FCA5880
bool __fastcall ACT2Q6_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (!pQuestData->fState && pQuestData->bNotIntro == 1)
	{
		QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
	}

	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A2Q3_TAINTEDSUN);
	if (!pQuest)
	{
		return true;
	}

	if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
	{
		FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
		exit(-1);
	}
	return pQuestData->pfSeqFilter(pQuest);
}

//D2Game.0x6FCA5900
void __fastcall ACT2Q6_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTRECORD_ClearQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A2Q2, QFLAG_CUSTOM5);
}

//D2Game.0x6FCA5930
void __fastcall ACT2Q6_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_CUSTOM5);

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED))
	{
		((D2Act2Quest6Strc*)pQuestData->pQuestDataEx)->bDurielLairPortalNeedsToOpen = 1;
		((D2Act2Quest6Strc*)pQuestData->pQuestDataEx)->bObjectsNeedUpdate = 1;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		((D2Act2Quest6Strc*)pQuestData->pQuestDataEx)->bRewardedOrCompletedBefore = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_STARTED) == 1)
	{
		pQuestData->fLastState = 1;
		pQuestData->fState = 2;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fLastState = 5;
		pQuestData->fState = 5;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_ENTERAREA) == 1)
	{
		pQuestData->fLastState = 6;
		pQuestData->fState = 5;
	}
}

//D2Game.0x6FCA59F0
void __fastcall ACT2Q6_SendTombIdToClient(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL);
	D2GSPacketSrv50 packet50 = {};
	packet50.nHeader = 0x50;
	packet50.nQuestId = QUEST_A2Q6_DURIEL;

	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
	const int32_t nTombStaffLevel = DUNGEON_GetHoradricStaffTombLevelId(pGame->pAct[1]);
	if (nTombStaffLevel)
	{
		packet50.TombOffsetPayload.nStaffTombLevelOffset = nTombStaffLevel - LEVEL_TALRASHASTOMB1;

		((D2Act2Quest6Strc*)pQuestData->pQuestDataEx)->nStaffTombLevelId = nTombStaffLevel;
	}
	else
	{
		packet50.TombOffsetPayload.nStaffTombLevelOffset = 0;
	}
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet50, sizeof(packet50));
}

//D2Game.0x6FCA5B00
int32_t __fastcall ACT2Q6_GetObjectIdForArcaneThing(D2GameStrc* pGame)
{
	static const int32_t nArcaneThingObjectIds[] =
	{
		313, 312, 308, 310, 311, 309, 307, 0
	};

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL);
	if (!pQuestData)
	{
		return 307;
	}

	const int32_t nTombStaffLevel = DUNGEON_GetHoradricStaffTombLevelId(pQuestData->pGame->pAct[ACT_II]);
	if (!nTombStaffLevel)
	{
		return 307;
	}

	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;

	if (!pQuestDataEx->bObjectIdsInitialized)
	{
		pQuestDataEx->bObjectIdsInitialized = 1;

		int32_t i = 0;
		for (int32_t j = 0; j < 7; ++j)
		{
			if (j != nTombStaffLevel - LEVEL_TALRASHASTOMB1 && i < 6)
			{
				pQuestDataEx->nArcaneThingObjectIds[i] = nArcaneThingObjectIds[j];
				++i;
			}
		}
	}

	if (pQuestDataEx->nArcaneThings == 6)
	{
		pQuestDataEx->nArcaneThings = 0;
	}

	return pQuestDataEx->nArcaneThingObjectIds[pQuestDataEx->nArcaneThings++];
}

//D2Game.0x6FCA5B20
void __fastcall D2GAME_WarpPlayerToTile_6FCA5B20(D2ObjOperateFnStrc* pOp)
{
	for (D2UnitStrc* i = UNITS_GetRoom(pOp->pObject)->pUnitFirst; i; i = i->pRoomNext)
	{
		if (i->dwUnitType == UNIT_TILE)
		{
			SUNIT_WarpPlayer(pOp->pGame, pOp->pPlayer, i);
		}
	}
}

//D2Game.0x6FCA5B60
void __fastcall OBJECTS_InitFunction37_Unused(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q6_DURIEL);
	if (!pQuestData)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
		return;
	}

	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bUnusedObjectInitialized = 1;

	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}
	pQuestDataEx->nUnusedObjectGUID = nUnitId;

	if (!pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
		return;
	}

	if (pQuestDataEx->bDurielLairTombOpen != 1)
	{
		if (pQuestDataEx->bDurielLairPortalNeedsToOpen == 1)
		{
			UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
		}
		else
		{
			UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_NEUTRAL);
		}
	}
	else
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
}

//D2Game.0x6FCA5BE0
void __fastcall OBJECTS_InitFunction38_TyraelsDoor(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q6_DURIEL);
	if (!pQuestData)
	{
		return;
	}

	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bTyraelsDoorInitialized = 1;

	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}
	pQuestDataEx->nTyraelsDoorGUID = nUnitId;

	if (pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nTyraelsDoorObjectMode);
	}
	else
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
}

//D2Game.0x6FCA5C30
int32_t __fastcall ACT2Q6_IsDurielsLairClosed(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL);

	if (pQuestData && pQuestData->bNotIntro)
	{
		return ((D2Act2Quest6Strc*)pQuestData->pQuestDataEx)->bDurielLairTombOpen == 0;
	}

	return 0;
}

//D2Game.0x6FCA5C60
void __fastcall OBJECTS_InitFunction21_HoradricOrifice(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q6_DURIEL);
	if (!pQuestData)
	{
		return;
	}

	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bHoradricOrificeInitialized = 1;

	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}
	pQuestDataEx->nHoradricOrificeGUID = nUnitId;

	if (!pQuestDataEx->bDurielLairPortalNeedsToOpen || pQuestDataEx->bTimerActive || pQuestDataEx->bDurielLairTombOpen)
	{
		if (pQuestDataEx->bDurielLairTombOpen)
		{
			UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
		}
	}
	else
	{
		QUESTS_CreateTimer(pQuestData, ACT2Q6_Timer_UpdateObjects, 1);
		pQuestDataEx->bTimerActive = 1;
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
}

//D2Game.0x6FCA5CE0
bool __fastcall ACT2Q6_Timer_UpdateObjects(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bObjectsNeedUpdate != 1)
	{
		pQuestDataEx->bTimerActive = 0;
		return true;
	}

	int32_t nObjects = 0;
	if (pQuestDataEx->bUnusedObjectInitialized)
	{
		D2UnitStrc* pUnusedObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nUnusedObjectGUID);
		if (pUnusedObject)
		{
			nObjects = 1;
			if (!pUnusedObject->dwAnimMode)
			{
				if (pQuestDataEx->bDurielLairPortalNeedsToOpen)
				{
					UNITS_ChangeAnimMode(pUnusedObject, OBJMODE_OPENED);
				}
				else
				{
					UNITS_ChangeAnimMode(pUnusedObject, OBJMODE_OPERATING);
					D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pUnusedObject->dwClassId);
					EVENT_SetEvent(pQuestData->pGame, pUnusedObject, UNITEVENTCALLBACK_ENDANIM, pQuestData->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);
				}
			}
		}
	}

	if (pQuestDataEx->bHoradricOrificeInitialized)
	{
		D2UnitStrc* pHoradricOrifice = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nHoradricOrificeGUID);
		if (pHoradricOrifice)
		{
			const int32_t nX = CLIENTS_GetUnitX(pHoradricOrifice) - 13;
			const int32_t nY = CLIENTS_GetUnitY(pHoradricOrifice) + 3;

			D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pHoradricOrifice), nX, nY);
			if (pRoom)
			{
				D2UnitStrc* pDurielLairPortal = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_PORTAL_TO_DURIELS_LAIR, nX, nY, pGame, pRoom, 1, 1, 0);
				if (pDurielLairPortal)
				{
					++nObjects;
					if (pQuestDataEx->bDurielLairPortalNeedsToOpen)
					{
						UNITS_ChangeAnimMode(pDurielLairPortal, OBJMODE_OPENED);
					}
					else
					{
						UNITS_ChangeAnimMode(pDurielLairPortal, OBJMODE_OPERATING);
						DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pHoradricOrifice), 1);
						DUNGEON_ToggleHasPortalFlag(pRoom, 1);
					}
				}
			}
		}
	}

	if (!nObjects)
	{
		return false;
	}

	pQuestDataEx->bDurielLairTombOpen = 1;
	pQuestDataEx->bObjectsNeedUpdate = 0;
	pQuestDataEx->bTimerActive = 0;
	return true;
}

//D2Game.0x6FCA5F20
void __fastcall ACT2Q6_UnitIterate_DeleteHoradricItems(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (DRLG_GetActNoFromLevelId(nLevelId) != ACT_II)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_PRIMARYGOALDONE);
	if (D2GAME_PLRTRADE_IsInteractingWithPlayer(pGame, pUnit))
	{
		return;
	}

	QUESTS_DeleteItem(pGame, pUnit, ' tsh');
	QUESTS_DeleteItem(pGame, pUnit, ' piv');
	QUESTS_DeleteItem(pGame, pUnit, ' fsm');
}

//D2Game.0x6FCA5FB0
int32_t __fastcall OBJECTS_OperateFunction25_StaffOrifice(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	if (!QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q6_DURIEL))
	{
		return 1;
	}

	if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
	{
		if (pObject->dwAnimMode == OBJMODE_OPERATING && SUNIT_GetInteractUnit(pOp->pGame, pOp->pPlayer) == pObject)
		{
			SUNIT_ResetInteractInfo(pOp->pPlayer);
			UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
			return 0;
		}
	}
	else
	{
		if (PLAYER_IsBusy(pOp->pPlayer))
		{
			return 1;
		}

		if (!ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' tsh'))
		{
			SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
			return 1;
		}

		SUNIT_SetInteractInfo(pOp->pPlayer, UNIT_OBJECT, pObject->dwUnitId);
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

		D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pOp->pPlayer, "..\\D2Game/Quests/a2q6.cpp", 1589);

		D2GSPacketSrv58 packet58 = {};
		packet58.nHeader = 0x58;
		packet58.nUnitId = pObject->dwUnitId;
		D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet58, sizeof(D2GSPacketSrv58));
	}

	return 1;
}

//D2Game.0x6FCA60D0
void __fastcall ACT2Q6_DeleteAllHoradricItemsAndOpenTomb(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pObject)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_PRIMARYGOALDONE);

	QUESTS_DeleteItem(pGame, pUnit, ' tsh');
	QUESTS_DeleteItem(pGame, pUnit, ' piv');
	QUESTS_DeleteItem(pGame, pUnit, ' fsm');

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT2Q6_UnitIterate_DeleteHoradricItems, 0);
	}

	QUESTS_TriggerFX(pGame, 3);

	D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(338);

	const int32_t nRange = (int32_t)((uint64_t)(1717986919i64 * ((int32_t)pMissilesTxtRecord->wRange - 75)) >> 32) >> 3;
	const int32_t nTicks = ((uint32_t)nRange >> 31) + nRange;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL);
	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bObjectsNeedUpdate = 1;
	pQuestDataEx->bStaffItemsRemoved = 1;
	if (!pQuestDataEx->bTimerActive)
	{
		QUESTS_CreateTimer(pQuestData, ACT2Q6_Timer_UpdateObjects, nTicks);
		pQuestDataEx->bTimerActive = 1;
	}

	UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

	D2ActiveRoomStrc* pObjectRoom = UNITS_GetRoom(pObject);
	D2_ASSERT(pObjectRoom);

	const int32_t nX = CLIENTS_GetUnitX(pObject) - 13;
	const int32_t nY = CLIENTS_GetUnitY(pObject) + 3;
	
	DUNGEON_ToggleHasPortalFlag(pObjectRoom, 0);
	if (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomAtPosition(pObjectRoom, nX, nY))
	{
		DUNGEON_ToggleHasPortalFlag(pRoom, 0);
	}
}

//D2Game.0x6FCA6310
int32_t __fastcall ACT2Q6_IsInRangeOfTyrael(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bRewardedOrCompletedBefore)
	{
		return 1;
	}

	if (!pQuestDataEx->bPortalToLutGholeinOpened)
	{
		return 0;
	}

	pQuestDataEx->bCloseToTyrael = 0;
	pQuestDataEx->pTyrael = pUnit;
	SUNIT_IterateLivingPlayers(pGame, ACT2Q6_UnitIterate_CheckDistanceToTyrael, pQuestDataEx);
	return pQuestDataEx->bCloseToTyrael == 0;
}

//D2Game.0x6FCA6370
void __fastcall ACT2Q6_UnitIterate_CheckDistanceToTyrael(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2Act2Quest6Strc* pQuestDataEx = (D2Act2Quest6Strc*)pData;

	const int32_t nDistance = UNITS_GetDistanceToOtherUnit(pUnit, pQuestDataEx->pTyrael);
	if (nDistance < 12)
	{
		pQuestDataEx->bCloseToTyrael = 1;
	}
}

//D2Game.0x6FCA6390
int32_t __fastcall ACT2Q6_ActivateTomb(D2GameStrc* pGame, D2CoordStrc* pCoord, D2ActiveRoomStrc** ppRoom)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL);
	if (!pQuestData || ((D2Act2Quest6Strc*)pQuestData->pQuestDataEx)->bPortalIsOpening != 1)
	{
		return 0;
	}

	int32_t nX = 0;
	int32_t nY = 0;
	D2ActiveRoomStrc* pRoom = DUNGEON_FindActSpawnLocationEx(pGame->pAct[ACT_II], LEVEL_LUTGHOLEIN, 12, &nX, &nY, 3);
	if (!pRoom)
	{
		return 0;
	}

	sub_6FC385A0(pGame, pRoom);

	pRoom = COLLISION_GetFreeCoordinatesEx(pRoom, pCoord, 3, 0xBE11, 7);
	if (!pRoom)
	{
		return 0;
	}

	pCoord->nX = nX;
	pCoord->nY = nY;
	*ppRoom = pRoom;
	return 1;
}
