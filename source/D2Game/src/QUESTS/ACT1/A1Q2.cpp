#include "QUESTS/ACT1/A1Q2.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitNpc.h"


//D2Game.0x6FD32570
D2NPCMessageTableStrc gpAct1Q2NpcMessages[] =
{
	{
		{
			{ MONSTER_KASHYA, 81, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_KASHYA, 82, 0, 2 },
			{ MONSTER_WARRIV1, 86, 0, 2 },
			{ MONSTER_CHARSI, 83, 0, 2 },
			{ MONSTER_AKARA, 85, 0, 2 },
			{ MONSTER_GHEED, 84, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_KASHYA, 87, 0, 2 },
			{ MONSTER_WARRIV1, 91, 0, 2 },
			{ MONSTER_CHARSI, 89, 0, 2 },
			{ MONSTER_AKARA, 88, 0, 2 },
			{ MONSTER_GHEED, 90, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_KASHYA, 92, 0, 0 },
			{ MONSTER_WARRIV1, 96, 0, 2 },
			{ MONSTER_CHARSI, 94, 0, 2 },
			{ MONSTER_AKARA, 93, 0, 2 },
			{ MONSTER_GHEED, 95, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_WARRIV1, 96, 0, 2 },
			{ MONSTER_KASHYA, 92, 0, 2 },
			{ MONSTER_AKARA, 93, 0, 2 },
			{ MONSTER_GHEED, 95, 0, 2 },
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


//D2Game.0x6FC98450
void __fastcall ACT1Q2_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (nLevelId && !DRLG_GetActNoFromLevelId(nLevelId))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_PRIMARYGOALDONE);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDPENDING);
	}
}

//D2Game.0x6FC984C0
bool __fastcall ACT1Q2_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_KASHYA || QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDGRANTED) == 1)
	{
		return false;
	}

	if (pQuest->fState == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDPENDING))
	{
		return true;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDPENDING) == 1)
	{
		return true;
	}

	return false;
}

//D2Game.0x6FC98530
void __fastcall ACT1Q2_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT1Q2_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT1Q2_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT1Q2_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT1Q2_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT1Q2_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT1Q2_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT1Q2_Callback13_PlayerStartedGame;
	pQuestData->pfSeqFilter = ACT1Q2_SeqCallback;
	pQuestData->pNPCMessages = gpAct1Q2NpcMessages;
	pQuestData->nQuest = QUESTSTATEFLAG_A1Q2;
	pQuestData->bActive = 1;

	D2Act1Quest2Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act1Quest2Strc);

	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->pfStatusFilter = 0;
	pQuestData->nInitNo = 4;
	pQuestData->nSeqId = 4;
	pQuestData->pfActiveFilter = ACT1Q2_ActiveFilterCallback;

	pQuestDataEx->unk0x01 = 0;
	pQuestDataEx->unk0x00 = 0;
	pQuestDataEx->bKashyaActivated = 0;
	pQuestDataEx->bBloodravenKilled = 0;
	pQuestDataEx->nBloodravenGUID = 0;

	pQuestData->fState = 0;
}

//D2Game.0x6FC98600
void __fastcall ACT1Q2_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_KASHYA)
	{
		return;
	}

	D2Act1Quest2Strc* pQuestDataEx = (D2Act1Quest2Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bKashyaActivated != 1)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 1);
	pQuestDataEx->bKashyaActivated = 0;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q2_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FC98660
int32_t __fastcall ACT1Q2_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A1Q2_BLOODRAVEN);
	}

	return 0;
}

//D2Game.0x6FC986D0
int32_t __fastcall ACT1Q2_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q2_BLOODRAVEN);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		if (pQuestData->fLastState == 1)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_LEAVETOWN);
		}
		else
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_ENTERAREA);
		}
	}

	return 0;
}

//D2Game.0x6FC98760
void __fastcall ACT1Q2_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (pQuestArg->nNPCNo != MONSTER_KASHYA)
	{
		return;
	}

	if (pQuestArg->nMessageIndex == 81)
	{
		((D2Act1Quest2Strc*)pQuestData->pQuestDataEx)->bKashyaActivated = 1;
		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q2_UnitIterate_UpdateQuestStateFlags);
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
	else if (pQuestArg->nMessageIndex == 92)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
		{
			return;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE) && pQuestData->fState != 5)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 13, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 0);
			QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);

			if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
			{
				FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
				exit(-1);
			}
			pQuestData->pfSeqFilter(pQuestData);
		}

		QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED);
		QUESTRECORD_ClearQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING);
		QUESTS_UpdatePlayerFlags(pQuestData->pGame, pQuestArg->pPlayer);

		int32_t nUnitId = -1;
		if (pQuestArg->pPlayer)
		{
			nUnitId = pQuestArg->pPlayer->dwUnitId;
		}

		QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, nUnitId);
		D2GAME_NPC_AssignMercenary_6FCCB520(pQuestData->pGame, pQuestArg->pPlayer, MONSTER_KASHYA);
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
}

//D2Game.0x6FC988F0
void __fastcall ACT1Q2_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4
	};

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
		return;
	}

	int32_t nUnitId = -1;
	if (pQuestArg->pPlayer)
	{
		nUnitId = pQuestArg->pPlayer->dwUnitId;
	}

	if (QUESTS_CheckPlayerGUID(pQuestData, nUnitId) == 1)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
	}
	else if (pQuestData->fState && QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) != 1 && pQuestData->fState < 4)
	{
		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < 6)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//D2Game.0x6FC989D0
void __fastcall ACT1Q2_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestData->bNotIntro)
	{
		return;
	}

	QUESTS_DebugOutput(pQuestData->pGame, "killed boss for quest", __FILE__, __LINE__);
	QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);

	D2Act1Quest2Strc* pQuestDataEx = (D2Act1Quest2Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->unk0x01 = 1;
	pQuestDataEx->bBloodravenKilled = 1;

	if (pQuestArg->pTarget)
	{
		pQuestDataEx->nBloodravenGUID = pQuestArg->pTarget->dwUnitId;
	}
	else
	{
		pQuestDataEx->nBloodravenGUID = -1;
	}

	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q2_UnitIterate_SetRewardPending);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q2_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q2_UnitIterate_SetCompletionFlag);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q2_UnitIterate_AttachCompletionSound);
	QUESTS_CreateTimer(pQuestData, ACT1Q2_Timer_StatusCycler, 15);
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	pQuestDataEx->unk0x00 = 1;
	pQuestDataEx->unk0x04 = 1;
	QUESTS_SetGlobalState(pQuestArg->pGame, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE);
}

//D2Game.0x6FC98AB0
bool __fastcall ACT1Q2_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 3, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 1);

	return true;
}

//D2Game.0x6FC98AD0
int32_t __fastcall ACT1Q2_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(pUnit);
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q2_BLOODRAVEN);

	D2_ASSERT(pQuestData);

	D2Act1Quest2Strc* pQuestDataEx = (D2Act1Quest2Strc*)pQuestData->pQuestDataEx;
	if (!pQuestDataEx->bBloodravenKilled)
	{
		return 0;
	}

	D2UnitStrc* pMonster = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pQuestDataEx->nBloodravenGUID);
	D2ActiveRoomStrc* pMonsterRoom = UNITS_GetRoom(pMonster);
	if (!pPlayerRoom)
	{
		return 0;
	}

	if (pPlayerRoom != pMonsterRoom)
	{
		D2_ASSERT(pPlayerRoom);

		D2ActiveRoomStrc** ppRoomList = nullptr;
		int32_t nRooms = 0;
		DUNGEON_GetAdjacentRoomsListFromRoom(pPlayerRoom, &ppRoomList, &nRooms);

		int32_t nCounter = 0;
		while (nCounter < nRooms)
		{
			if (ppRoomList[nCounter] == pMonsterRoom)
			{
				break;
			}
			++nCounter;
		}

		if (nCounter >= nRooms)
		{
			return 0;
		}
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDPENDING))
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDPENDING);
	return 0;
}

//D2Game.0x6FC98C00
int32_t __fastcall ACT1Q2_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_REWARDPENDING) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A1Q2_BLOODRAVEN, 0);
	}

	return 0;
}

//D2Game.0x6FC98C60
int32_t __fastcall ACT1Q2_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A1Q2, QFLAG_PRIMARYGOALDONE) == 1)
	{
		SUNIT_AttachSound(pUnit, 34, pUnit);
	}

	return 0;
}

//D2Game.0x6FC98CA0
int32_t __fastcall ACT1Q2_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A1Q2, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT1Q2_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FC98CF0
void __fastcall ACT1Q2_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel == LEVEL_BURIALGROUNDS && pQuestData->bNotIntro == 1)
	{
		if (pQuestData->fState < 3)
		{
			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
			pQuestData->dwFlags &= 0xFFFFFF00;
		}

		if (pQuestData->fLastState == 1 || !pQuestData->fLastState)
		{
			QUESTS_UnitIterate(pQuestData, 2, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 1);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q2_UnitIterate_UpdateQuestStateFlags);
			return;
		}

		if (pQuestData->fState < 3)
		{
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q2_UnitIterate_UpdateQuestStateFlags);
			return;
		}
	}

	if (pQuestArg->nOldLevel == LEVEL_ROGUEENCAMPMENT)
	{
		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);
		if (pQuestData->fState != 2)
		{
			return;
		}

		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, 1) == 1)
		{
			return;
		}

		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q2_UnitIterate_UpdateQuestStateFlags);
	}
}

//D2Game.0x6FC98DD0
bool __fastcall ACT1Q2_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (!pQuestData->fState && pQuestData->bNotIntro == 1)
	{
		QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
		return true;
	}

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
	return pQuest->pfSeqFilter(pQuest);
}

//D2Game.0x6FC98E60
void __fastcall ACT1Q2_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_ENTERAREA) == 1)
	{
		pQuestData->fLastState = 2;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q2, QFLAG_STARTED) == 1)
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 1;
	}
}

//
void __fastcall ACT1Q2_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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
