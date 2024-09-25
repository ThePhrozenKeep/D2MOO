#include "QUESTS/ACT5/A5Q1.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgPreset.h>
#include <D2Dungeon.h>
#include <D2QuestRecord.h>

#include "AI/AiGeneral.h"
#include "GAME/Game.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "OBJECTS/Objects.h"
#include "PLAYER/PlrIntro.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD3BDF8
D2NPCMessageTableStrc gpAct5Q1NpcMessages[] =
{
	{
		{
			{ MONSTER_LARZUK, 20077, 0, 0, },
		},
		1
	},
	{
		{
			{ MONSTER_LARZUK, 20078, 0, 2, },
			{ MONSTER_CAIN6, 20079, 0, 2, },
			{ MONSTER_DREHYA, 20080, 0, 2, },
			{ MONSTER_MALAH, 20081, 0, 2, },
			{ MONSTER_NIHLATHAK, 20082, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20083, 0, 2, },
		},
		6
	},
	{
		{
			{ MONSTER_LARZUK, 20084, 0, 2, },
			{ MONSTER_CAIN6, 20085, 0, 2, },
			{ MONSTER_DREHYA, 20086, 0, 2, },
			{ MONSTER_MALAH, 20087, 0, 2, },
			{ MONSTER_NIHLATHAK, 20088, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20089, 0, 2, },
		},
		6
	},
	{
		{
			{ MONSTER_LARZUK, 20090, 0, 0, },
			{ MONSTER_CAIN6, 20091, 0, 2, },
			{ MONSTER_DREHYA, 20092, 0, 2, },
			{ MONSTER_MALAH, 20093, 0, 2, },
			{ MONSTER_NIHLATHAK, 20094, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20095, 0, 2, },
		},
		6
	},
	{
		{
			{ MONSTER_LARZUK, 20090, 0, 2, },
			{ MONSTER_CAIN6, 20091, 0, 2, },
			{ MONSTER_DREHYA, 20092, 0, 2, },
			{ MONSTER_MALAH, 20093, 0, 2, },
			{ MONSTER_NIHLATHAK, 20094, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20095, 0, 2, },
		},
		6
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FCB1200
void __fastcall ACT5Q1_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDGRANTED) == 1
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDPENDING) == 1
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_PRIMARYGOALDONE) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_V)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDPENDING);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_PRIMARYGOALDONE);
}

//D2Game.0x6FCB1280
bool __fastcall ACT5Q1_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_LARZUK)
	{
		return false;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuestFilter, QFLAG_REWARDPENDING) && !QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuestFilter, QFLAG_CUSTOM1))
	{
		return true;
	}

	if (pQuest->bNotIntro && pQuest->fState == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuestFilter, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuestFilter, QFLAG_REWARDPENDING))
	{
		return true;
	}

	return false;
}

//D2Game.0x6FCB1300
void __fastcall ACT5Q1_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT5Q1_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT5Q1_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT5Q1_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT5Q1_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT5Q1_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT5Q1_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT5Q1_Callback13_PlayerStartedGame;
	pQuestData->pNPCMessages = gpAct5Q1NpcMessages;
	pQuestData->bActive = 1;

	D2Act5Quest1Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act5Quest1Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A5Q1;
	pQuestData->pfStatusFilter = ACT5Q1_StatusFilterCallback;
	pQuestData->nInitNo = 4;
	pQuestData->pfActiveFilter = ACT5Q1_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT5Q1_SeqCallback;
	pQuestData->nSeqId = 32;
	memset(pQuestDataEx, 0x00, sizeof(D2Act5Quest1Strc));
}

//D2Game.0x6FCB13D0
bool __fastcall ACT5Q1_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	*pStatus = 0;

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDPENDING) || QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A5Q1, QFLAG_PRIMARYGOALDONE))
	{
		if (QUESTRECORD_GetQuestState(pFlags, pQuest->nQuestFilter, QFLAG_CUSTOM1))
		{
			*pStatus = 4;
		}
		else
		{
			*pStatus = 3;
		}
	}
	else if (pQuest->bNotIntro && !QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDGRANTED))
	{
		if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A5Q1, QFLAG_COMPLETEDNOW))
		{
			*pStatus = 12;
		}
		else if (pQuest->fState < 5)
		{
			*pStatus = pQuest->fLastState;
		}
	}

	return true;
}

//D2Game.0x6FCB1470
void __fastcall ACT5Q1_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_LARZUK)
	{
		return;
	}

	D2Act5Quest1Strc* pQuestDataEx = (D2Act5Quest1Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bLarzukStartActivated == 1)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q1_UnitIterate_StatusCyclerEx, 1);
		pQuestDataEx->bLarzukStartActivated = 0;
	}

	if (pQuestDataEx->bLarzukEndActivated == 1)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 4, 0, ACT5Q1_UnitIterate_StatusCyclerEx, 1);
		pQuestDataEx->bLarzukEndActivated = 0;
	}
}

//D2Game.0x6FCB14D0
int32_t __fastcall ACT5Q1_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A5Q1_SHENK);
	}

	return 0;
}

//D2Game.0x6FCB1540
void __fastcall ACT5Q1_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act5Quest1Strc* pQuestDataEx = (D2Act5Quest1Strc*)pQuestData->pQuestDataEx;

	if (pQuestArg->nNPCNo != MONSTER_LARZUK)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];
	if (pQuestArg->nMessageIndex == 20077)
	{
		pQuestDataEx->bLarzukStartActivated = 1;
		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q1_UnitIterate_UpdateQuestStateFlags);
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
	else if (pQuestArg->nMessageIndex == 20090)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDPENDING) != 1)
		{
			return;
		}

		pQuestDataEx->bLarzukEndActivated = 1;
		QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, pQuestData->nQuestFilter);
		QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_CUSTOM1);
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_PRIMARYGOALDONE) && pQuestData->fState != 5)
		{
			QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
			if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
			{
				FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
				exit(-1);
			}
			pQuestData->pfSeqFilter(pQuestData);

			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 13, 0, ACT5Q1_UnitIterate_StatusCyclerEx, 1);
		}

		if (pQuestArg->pPlayer)
		{
			QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
		}
		else
		{
			QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, -1);
		}
	}
}

//D2Game.0x6FCB16B0
int32_t __fastcall ACT5Q1_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q1_SHENK);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		if (pQuestData->fLastState == 2)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_ENTERAREA);
		}
		else
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_LEAVETOWN);
		}
	}

	return 0;
}

//D2Game.0x6FCB1740
void __fastcall ACT5Q1_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4
	};

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDPENDING))
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_CUSTOM1))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
		}
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED) != 1 && (pQuestData->fState < 4 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_PRIMARYGOALDONE)))
	{
		if (!pQuestData->bNotIntro)
		{
			return;
		}

		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < 8)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//D2Game.0x6FCB1830
void __fastcall ACT5Q1_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestData->bNotIntro)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pQuestArg->pTarget);
	if (!pRoom)
	{
		return;
	}

	((D2Act5Quest1Strc*)pQuestData->pQuestDataEx)->pRoom = pRoom;

	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, nullptr, ACT5Q1_UnitIterate_SetRewardPending);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT5Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT5Q1_UnitIterate_SetCompletionFlag);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT5Q1_UnitIterate_AttachCompletionSound);

	QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A5Q1, QFLAG_PRIMARYGOALDONE);
	QUESTS_TriggerFX(pQuestData->pGame, 15);

	if (pQuestData->fLastState < 3)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 3, 0, ACT5Q1_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCB18E0
int32_t __fastcall ACT5Q1_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);

	if (!pRoom || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDGRANTED))
	{
		return 0;
	}

	D2Act5Quest1Strc* pQuestDataEx = (D2Act5Quest1Strc*)QUESTS_GetQuestData(pGame, QUEST_A5Q1_SHENK)->pQuestDataEx;

	if (pRoom != pQuestDataEx->pRoom)
	{
		D2ActiveRoomStrc** ppRoomList = nullptr;
		int32_t nNumRooms = 0;
		DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

		int32_t nCounter = 0;
		while (nCounter < nNumRooms)
		{
			if (ppRoomList[nCounter] == pQuestDataEx->pRoom)
			{
				break;
			}

			++nCounter;
		}

		if (nCounter >= nNumRooms)
		{
			return 0;
		}
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDPENDING);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_PRIMARYGOALDONE);
	return 0;
}

//D2Game.0x6FCB19A0
int32_t __fastcall ACT5Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q1, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT5Q1_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCB19F0
int32_t __fastcall ACT5Q1_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDGRANTED) != 1
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDPENDING) != 1
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_PRIMARYGOALDONE) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A5Q1_SHENK, 0);
	}

	return 0;
}

//D2Game.0x6FCB1A60
int32_t __fastcall ACT5Q1_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q1, QFLAG_PRIMARYGOALDONE))
	{
		SUNIT_AttachSound(pUnit, 80, pUnit);
	}

	return 0;
}

//D2Game.0x6FCB1AA0
void __fastcall ACT5Q1_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel < LEVEL_BLOODYFOOTHILLS || pQuestArg->nNewLevel > LEVEL_ARREATPLATEAU)
	{
		if (pQuestArg->nOldLevel != LEVEL_HARROGATH)
		{
			return;
		}

		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);
		if (pQuestData->fState != 2)
		{
			return;
		}

		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDPENDING) == 1)
		{
			return;
		}

		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		if (!pQuestData->fLastState)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q1_UnitIterate_StatusCyclerEx, 1);
		}
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q1_UnitIterate_UpdateQuestStateFlags);
	}
	else
	{
		if (!pQuestData->bNotIntro)
		{
			return;
		}

		if (pQuestData->fState == 1 || pQuestData->fState == 2)
		{
			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q1_UnitIterate_UpdateQuestStateFlags);
		}
	}
}

//D2Game.0x6FCB1BA0
bool __fastcall ACT5Q1_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (pQuestData->fState != 5 && pQuestData->bNotIntro)
	{
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

//D2Game.0x6FCB1C10
void __fastcall ACT5Q1_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED)
		|| QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDPENDING) == 1)
	{
		pQuestData->fLastState = 5;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_ENTERAREA) == 1)
	{
		pQuestData->fLastState = 2;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fLastState = 1;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_STARTED) == 1)
	{
		pQuestData->fLastState = 1;
		pQuestData->fState = 2;
	}
	else if (PLRINTRO_GetQuestIntroFlag(pQuestArg->pPlayer, pQuestArg->pGame, MONSTER_MALAH) && !pQuestData->fState && pQuestData->bNotIntro)
	{
		pQuestData->fState = 1;
	}
}

//D2Game.0x6FCB1D10
void __fastcall ACT5Q1_SetRewardGranted(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDGRANTED);
	QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_REWARDPENDING);

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q1, QFLAG_COMPLETEDBEFORE) && !QUESTS_GetQuestData(pGame, QUEST_A5Q1_SHENK))
	{
		FOG_DisplayAssert("pQuestInfo != nullptr", __FILE__, __LINE__);
		exit(-1);
	}
}

//D2Game.0x6FCB1D90
void __fastcall OBJECTS_InitFunction71_LarzukStandard(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q1_SHENK);
	if (!pQuestData)
	{
		return;
	}

	D2Act5Quest1Strc* pQuestDataEx = (D2Act5Quest1Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bLarzukSpawned)
	{
		return;
	}

	D2CoordStrc pCoord = {};
	pCoord.nY = pOp->nY;
	pCoord.nX = pOp->nX;
	D2ActiveRoomStrc* pRoom = nullptr;
	QUESTS_GetFreePosition(pOp->pRoom, &pCoord, 2, 0x100, &pRoom, 16);
	if (!pRoom)
	{
		return;
	}

	D2UnitStrc* pLarzuk = D2GAME_SpawnMonster_6FC69F10(pOp->pGame, pRoom, pCoord.nX, pCoord.nY, MONSTER_LARZUK, 1, 5, 0);
	if (!pLarzuk)
	{
		return;
	}

	pLarzuk->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;

	pQuestDataEx->bLarzukSpawned = 1;
	pQuestDataEx->nLarzukGUID = pLarzuk->dwUnitId;

	if (pQuestDataEx->pLarzukMapAI && pQuestDataEx->pLarzukMapAI->pPosition && !pQuestDataEx->bLarzukMapAIChanged)
	{
		DRLGPRESET_ChangeMapAI(&pQuestDataEx->pLarzukMapAI, AIGENERAL_GetMapAiFromUnit(pLarzuk));
		pQuestDataEx->bLarzukMapAIChanged = 1;
	}
}

//D2Game.0x6FCB1E60
void __fastcall ACT5Q1_OnSiegeBossActivated(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q1_SHENK);

	if (!pQuestData || !pQuestData->bNotIntro || pQuestData->fLastState >= 2 || MONSTERUNIQUE_GetBossHcIdx(pUnit) != SUPERUNIQUE_SIEGE_BOSS)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 2, 0, ACT5Q1_UnitIterate_StatusCyclerEx, 1);
}

//
void __fastcall ACT5Q1_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->pPlayer)
	{
		QUESTS_FastRemovePlayerGUID(&pQuestData->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
	}
	else
	{
		QUESTS_FastRemovePlayerGUID(&pQuestData->tPlayerGUIDs, -1);
	}
}

//Inlined in D2Game.0x6FC975A0
void __fastcall ACT5Q1_ChangeLarzukMapAI(D2GameStrc* pGame, D2UnitStrc* pUnit, D2MapAIStrc* pMapAi)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q1_SHENK);
	if (!pQuestData || !pMapAi)
	{
		return;
	}

	D2Act5Quest1Strc* pQuestDataEx = (D2Act5Quest1Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->pLarzukMapAI = DRLGPRESET_CreateCopyOfMapAI(pQuestData->pGame->pMemoryPool, pMapAi);
	if (!pQuestDataEx->bLarzukSpawned)
	{
		return;
	}

	D2UnitStrc* pLarzuk = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_MONSTER, pQuestDataEx->nLarzukGUID);
	if (pLarzuk && pQuestDataEx->pLarzukMapAI && pQuestDataEx->pLarzukMapAI->pPosition && !pQuestDataEx->bLarzukMapAIChanged)
	{
		DRLGPRESET_ChangeMapAI(&pQuestDataEx->pLarzukMapAI, AIGENERAL_GetMapAiFromUnit(pLarzuk));
		pQuestDataEx->bLarzukMapAIChanged = 1;
	}
}
