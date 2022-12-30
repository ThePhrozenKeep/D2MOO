#include "QUESTS/ACT2/A2Q5.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT2/A2Q4.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD368A8
D2NPCMessageTableStrc gpAct2Q5NpcMessages[] =
{
	{
		{
			{ MONSTER_ATMA, 414, 0, 2, },
			{ MONSTER_WARRIV2, 413, 0, 2, },
			{ MONSTER_GREIZ, 408, 0, 2, },
			{ MONSTER_ELZIX, 417, 0, 2, },
			{ MONSTER_DROGNAN, 410, 0, 2, },
			{ MONSTER_LYSANDER, 411, 0, 2, },
			{ MONSTER_CAIN2, 418, 0, 2, },
			{ MONSTER_MESHIF1, 412, 0, 2, },
			{ MONSTER_JERHYN, 409, 0, 2, },
			{ MONSTER_GEGLASH, 415, 0, 2, },
			{ MONSTER_FARA, 416, 0, 2, },
		},
		11
	},
	{
		{
			{ MONSTER_ATMA, 427, 0, 1, },
			{ MONSTER_WARRIV2, 424, 0, 1, },
			{ MONSTER_GREIZ, 419, 0, 1, },
			{ MONSTER_ELZIX, 423, 0, 1, },
			{ MONSTER_DROGNAN, 422, 0, 1, },
			{ MONSTER_LYSANDER, 426, 0, 1, },
			{ MONSTER_CAIN2, 429, 0, 1, },
			{ MONSTER_MESHIF1, 425, 0, 1, },
			{ MONSTER_JERHYN, 421, 0, 1, },
			{ MONSTER_GEGLASH, 420, 0, 1, },
			{ MONSTER_FARA, 428, 0, 1, },
		},
		11
	},
	{
		{
			{ MONSTER_ATMA, 427, 0, 2, },
			{ MONSTER_WARRIV2, 424, 0, 2, },
			{ MONSTER_GREIZ, 419, 0, 2, },
			{ MONSTER_ELZIX, 423, 0, 2, },
			{ MONSTER_DROGNAN, 422, 0, 2, },
			{ MONSTER_LYSANDER, 426, 0, 2, },
			{ MONSTER_CAIN2, 429, 0, 2, },
			{ MONSTER_MESHIF1, 425, 0, 2, },
			{ MONSTER_JERHYN, 421, 0, 2, },
			{ MONSTER_GEGLASH, 420, 0, 2, },
			{ MONSTER_FARA, 428, 0, 2, },
		},
		11
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FCA3DB0
bool __fastcall ACT2Q5_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	switch (nNpcId)
	{
	case MONSTER_ATMA:
	case MONSTER_WARRIV2:
	case MONSTER_ELZIX:
	case MONSTER_DROGNAN:
	case MONSTER_LYSANDER:
	case MONSTER_CAIN2:
	case MONSTER_MESHIF1:
	case MONSTER_JERHYN:
	case MONSTER_GEGLASH:
	case MONSTER_FARA:
		break;
	default:
		return false;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDPENDING) != 1)
	{
		return false;
	}

	return true;
}

//D2Game.0x6FCA3E20
void __fastcall ACT2Q5_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTS_GetQuestData(pGame, QUEST_A2Q5_SUMMONER))
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_II)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDPENDING);
}

//D2Game.0x6FCA3EA0
void __fastcall ACT2Q5_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pNPCMessages = gpAct2Q5NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;

	D2Act2Quest5Strc* pQuestDataEx = (D2Act2Quest5Strc*)FOG_AllocPool(pQuestData->pGame->pMemoryPool, sizeof(D2Act2Quest5Strc), __FILE__, __LINE__, 0);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->nQuest = QUESTSTATEFLAG_A2Q5;
	pQuestData->pfStatusFilter = nullptr;
	pQuestData->nInitNo = 2;
	pQuestData->pfActiveFilter = ACT2Q5_ActiveFilterCallback;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT2Q5_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT2Q5_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT2Q5_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT2Q5_Callback13_PlayerStartedGame;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT2Q5_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT2Q5_Callback00_NpcActivate;

	pQuestDataEx->bSummonerKilled = 0;
	pQuestDataEx->unk0x01 = 0;
	pQuestDataEx->bStatusTimerStarted = 0;
	pQuestDataEx->pRoom = nullptr;
}

//D2Game.0x6FCA3F50
void __fastcall ACT2Q5_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2
	};

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDPENDING))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 1);
	}
	else if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
	}
	else if (pQuestData->bNotIntro && pQuestData->fState)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDGRANTED) == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_PRIMARYGOALDONE))
		{
			return;
		}

		if (pQuestData->fState >= 2 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_PRIMARYGOALDONE))
		{
			return;
		}

		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && (uint32_t)nIndex < 4)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//D2Game.0x6FCA4040
void __fastcall ACT2Q5_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	switch (pQuestArg->nMessageIndex)
	{
	case 419:
	case 420:
	case 421:
	case 422:
	case 423:
	case 424:
	case 425:
	case 426:
	case 427:
	case 428:
	case 429:
		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDPENDING))
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_PRIMARYGOALDONE))
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 13, 0, ACT2Q5_UnitIterate_StatusCyclerEx, 1);
				QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q5, QFLAG_PRIMARYGOALDONE);
				QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
			}

			QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDGRANTED);
			QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDPENDING);
		}
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		break;
	}
}

//D2Game.0x6FCA4140
int32_t __fastcall ACT2Q5_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A2Q5_SUMMONER);
	}

	return 0;
}

//D2Game.0x6FCA41B0
void __fastcall ACT2Q5_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestData->bNotIntro)
	{
		QUESTS_DebugOutput(pQuestData->pGame, "killed summoner for quest", __FILE__, __LINE__);
		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);

		D2Act2Quest5Strc* pQuestDataEx = (D2Act2Quest5Strc*)pQuestData->pQuestDataEx;
		if (!pQuestDataEx->bStatusTimerStarted)
		{
			pQuestDataEx->bStatusTimerStarted = 1;
			QUESTS_CreateTimer(pQuestData, ACT2Q5_Timer_StatusCycler, 3);
		}

		pQuestDataEx->unk0x09 = 0;
		pQuestDataEx->bSummonerKilled = 1;
		pQuestDataEx->pRoom = UNITS_GetRoom(pQuestArg->pTarget);

		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT2Q5_UnitIterate_SetRewardPending);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT2Q4_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT2Q5_UnitIterate_SetCompletionFlag);
	}

	QUESTS_TriggerFX(pQuestData->pGame, 7);
}

//D2Game.0x6FCA4270
int32_t __fastcall ACT2Q5_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q5_SUMMONER);
	D2_ASSERT(pQuestData);

	D2Act2Quest5Strc* pQuestDataEx = (D2Act2Quest5Strc*)pQuestData->pQuestDataEx;

	if (pRoom != pQuestDataEx->pRoom)
	{
		D2RoomStrc** ppRoomList = nullptr;
		int32_t nNumRooms = 0;
		DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

		int32_t nCounter = 0;
		while (nCounter < nNumRooms)
		{
			if (ppRoomList[nCounter] != pQuestDataEx->pRoom)
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

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDPENDING))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_PRIMARYGOALDONE);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDPENDING);
	}

	return 0;
}

//D2Game.0x6FCA4380
bool __fastcall ACT2Q5_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act2Quest5Strc* pQuestDataEx = (D2Act2Quest5Strc*)pQuestData->pQuestDataEx;

	if (pQuestDataEx->unk0x09)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 4, 0, ACT2Q5_UnitIterate_StatusCyclerEx, 1);
		pQuestDataEx->bStatusTimerStarted = 0;
		return true;
	}

	pQuestDataEx->unk0x09 = 1;
	SUNIT_IterateUnitsOfType(pGame, 0, 0, ACT2Q5_UnitIterate_AttachCompletionSound);
	return false;
}

//D2Game.0x6FCA43D0
int32_t __fastcall ACT2Q5_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q5, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_ARCANESANCTUARY)
	{
		return 0;
	}

	SUNIT_AttachSound(pUnit, 51, pUnit);
	return 0;
}

//D2Game.0x6FCA4420
int32_t __fastcall ACT2Q5_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q5, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT2Q4_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCA4470
int32_t __fastcall ACT2Q5_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDPENDING) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A2Q5_SUMMONER, 0);
	}

	return 0;
}

//D2Game.0x6FCA44D0
void __fastcall ACT2Q5_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestData->bNotIntro && pQuestArg->nNewLevel >= LEVEL_LUTGHOLEIN && pQuestArg->nOldLevel == LEVEL_LUTGHOLEIN)
	{
		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);
	}
}

//D2Game.0x6FCA4500
void __fastcall ACT2Q5_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_STARTED) != 1)
	{
		return;
	}

	pQuestData->fLastState = 2;
	pQuestData->fState = 1;
}

//D2Game.0x6FCA4560
void __fastcall ACT2Q5_OnSummonerActivated(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q5_SUMMONER);
	if (!pQuestData || pQuestData->bNotIntro != 1)
	{
		return;
	}

	D2Act2Quest5Strc* pQuestDataEx = (D2Act2Quest5Strc*)pQuestData->pQuestDataEx;
	if (!pQuestDataEx->unk0x01)
	{
		pQuestDataEx->unk0x01 = 1;
	}

	if (!pQuestData->fState)
	{
		QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
	}

	if (pQuestData->fLastState < 2)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 2, 0, ACT2Q5_UnitIterate_StatusCyclerEx, 1);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q5_UnitIterate_UpdateQuestStateFlags);
	}
}

//D2Game.0x6FCA45E0
int32_t __fastcall ACT2Q5_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q5_SUMMONER);
	if (pQuestData && pQuestData->fState == 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q5, QFLAG_STARTED);
	}

	return 0;
}

//
void __fastcall ACT2Q5_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_RemovePlayerGUID(pQuestData, pQuestArg);
}
