#include "QUESTS/ACT4/A4Q1.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2Monsters.h>
#include <D2QuestRecord.h>
#include <D2StatList.h>

#include "GAME/Game.h"
#include "MONSTER/MonsterSpawn.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD3A960
D2NPCMessageTableStrc gpAct4Q1NpcMessages[] =
{
	{
		{
			{ MONSTER_TYRAEL2, 670, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_TYRAEL2, 671, 0, 2 },
			{ MONSTER_CAIN4, 672, 0, 2},
		},
		2
	},
	{
		{
			{ MONSTER_TYRAEL2, 673, 0, 2 },
			{ MONSTER_CAIN4, 674, 0, 2 },
		},
		2
	},
	{
		{
			{ MONSTER_TYRAEL2, 676, 0, 1 },
			{ MONSTER_CAIN4, 677, 0, 2 },
			{ MONSTER_IZUALGHOST, 675, 0, 1 },
		},
		3
	},
	{
		{
			{ MONSTER_TYRAEL2, 676, 0, 2 },
			{ MONSTER_CAIN4, 677, 0, 2 },
		},
		2
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FCAD830
bool __fastcall ACT4Q1_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (!pQuest->bNotIntro || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDGRANTED))
	{
		return false;
	}

	if (nNpcId == MONSTER_TYRAEL2)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDPENDING) == 1 || pQuest->fState == 1)
		{
			return true;
		}
	}
	else if (nNpcId == MONSTER_IZUALGHOST)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_CUSTOM1))
		{
			return true;
		}
	}

	return false;
}

//D2Game.0x6FCAD8B0
void __fastcall ACT4Q1_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDPENDING))
	{
		return;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_IV)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDPENDING);
	QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, QUESTSTATEFLAG_A4Q1);
}

//D2Game.0x6FCAD930
void __fastcall ACT4Q1_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pNPCMessages = gpAct4Q1NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 1;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT4Q1_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT4Q1_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT4Q1_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT4Q1_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT4Q1_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT4Q1_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT4Q1_Callback13_PlayerStartedGame;
	pQuestData->nQuest = QUESTSTATEFLAG_A4Q1;
	pQuestData->nInitNo = 4;
	pQuestData->pfStatusFilter = 0;
	pQuestData->pfActiveFilter = ACT4Q1_ActiveFilterCallback;

	D2Act4Quest1Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act4Quest1Strc);
	memset(pQuestDataEx, 0x00, sizeof(D2Act4Quest1Strc));

	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->pfSeqFilter = ACT4Q1_SeqCallback;
	pQuestData->nSeqId = 24;
}

//D2Game.0x6FCADA00
bool __fastcall ACT4Q1_SeqCallback(D2QuestDataStrc* pQuestData)
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

	if (IsBadCodePtr((FARPROC)pQuest->pfSeqFilter))
	{
		FOG_DisplayAssert("pQuest->pSequence", __FILE__, __LINE__);
		exit(-1);
	}

	return pQuestData->pfSeqFilter(pQuest);
}

//D2Game.0x6FCADA70
void __fastcall ACT4Q1_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act4Quest1Strc* pQuestDataEx = (D2Act4Quest1Strc*)pQuestData->pQuestDataEx;
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

	if (pQuestArg->nNPCNo == MONSTER_TYRAEL2)
	{
		if (pQuestArg->nMessageIndex == 670)
		{
			pQuestDataEx->bTyraelActivated = 1;
			QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT4Q1_UnitIterate_UpdateQuestStateFlags);
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		}
		else if (pQuestArg->nMessageIndex == 676)
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
			{
				if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE))
				{
					if (pQuestData->fState != 5)
					{
						QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
						if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
						{
							FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
							exit(-1);
						}
						pQuestData->pfSeqFilter(pQuestData);
						pQuestData->dwFlags &= 0xFFFFFF00;
						QUESTS_UnitIterate(pQuestData, 13, 0, ACT4Q1_UnitIterate_StatusCyclerEx, 0);
					}

					pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
					QUESTS_SetGlobalState(pQuestArg->pGame, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE);
				}

				QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, 0);
				QUESTRECORD_ClearQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING);
				QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, pQuestData->nQuest);

				STATLIST_AddUnitStat(pQuestArg->pPlayer, STAT_SKILLPTS, 2, 0);
				QUESTS_SendLogUpdate(pQuestArg->pPlayer, QUEST_A4Q1_IZUAL);

				QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1);
				QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
			}
		}
	}
	else if (pQuestArg->nNPCNo == MONSTER_IZUALGHOST)
	{
		if (pQuestArg->nMessageIndex == 675)
		{
			pQuestDataEx->bIzualGhostActivated = 1;
			pQuestDataEx->unk0x0F = 1;
			QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_CUSTOM1);
			if (pQuestData->fLastState != 4)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 4, 0, ACT4Q1_UnitIterate_StatusCyclerEx, 1);
			}
		}
	}
}

//D2Game.0x6FCADC80
int32_t __fastcall ACT4Q1_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDGRANTED) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A4Q1_IZUAL);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act4Quest1Strc* pQuestDataEx = (D2Act4Quest1Strc*)pQuestData->pQuestDataEx;
	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		if (pQuestDataEx->unk0x0C)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_ENTERAREA);
		}
		else
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_LEAVETOWN);
		}
	}

	return 0;
}

//D2Game.0x6FCADD00
int32_t __fastcall ACT4Q1_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pPlayer, QUEST_A4Q1_IZUAL);
	}

	return 0;
}

//D2Game.0x6FCADD70
void __fastcall ACT4Q1_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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

		if (nNpcId == MONSTER_IZUALGHOST && !QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_CUSTOM1))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_IZUALGHOST, 3);
			return;
		}
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
	}
	else if (QUESTS_CheckPlayerGUID(pQuestData, pQuestArg->pTarget ? pQuestArg->pTarget->dwUnitId : -1) == 1)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) != 1 && (pQuestData->fState < 4 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE)))
	{
		if (!pQuestData->bNotIntro)
		{
			return;
		}

		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < 6)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//D2Game.0x6FCADEA0
void __fastcall ACT4Q1_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act4Quest1Strc* pQuestDataEx = (D2Act4Quest1Strc*)pQuestData->pQuestDataEx;
	QUESTS_DebugOutput(pQuestData->pGame, "Finished quest", __FILE__, __LINE__);

	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;

	QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);

	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = nullptr;

	pQuestDataEx->pTargetUnit = pQuestArg->pTarget;
	if (pQuestData->bNotIntro)
	{
		if (pQuestArg->pPlayer)
		{
			D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDPENDING))
			{
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_PRIMARYGOALDONE);
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDPENDING);
				QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, pQuestData->nQuest);
			}
		}
		pQuestDataEx->bNeedToSpawnIzualGhost = 1;
		UNITS_GetCoords(pQuestArg->pTarget, &pQuestDataEx->pCoords);

		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT4Q1_UnitIterate_SetRewardPending);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT4Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT4Q1_UnitIterate_SetCompletionFlag);
	}

	if (!pQuestDataEx->bIzualGhostIsSpawning)
	{
		pQuestDataEx->bIzualGhostIsSpawning = 1;
		QUESTS_CreateTimer(pQuestData, ACT4Q1_SpawnIzualGhost, 3);
	}
}

//D2Game.0x6FCADFC0
int32_t __fastcall ACT4Q1_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDPENDING) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A4Q1_IZUAL, 0);
	}

	return 0;
}

//D2Game.0x6FCAE020
int32_t __fastcall ACT4Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A4Q1, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT4Q1_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCAE070
bool __fastcall ACT4Q1_SpawnIzualGhost(D2GameStrc* pGame, D2QuestDataStrc* pQuest)
{
	D2Act4Quest1Strc* pQuestDataEx = (D2Act4Quest1Strc*)pQuest->pQuestDataEx;
	if (pQuest->fState == 4 && pQuest->fLastState != 4 && pQuest->fLastState != 13)
	{
		pQuest->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuest, 3, 0, ACT4Q1_UnitIterate_StatusCyclerEx, 1);
	}

	if (pQuestDataEx->bNeedToSpawnIzualGhost)
	{
		pQuestDataEx->bNeedToSpawnIzualGhost = 0;

		D2RoomStrc* pRoom = DUNGEON_FindRoomBySubtileCoordinates(pGame->pAct[ACT_IV], pQuestDataEx->pCoords.nX, pQuestDataEx->pCoords.nY);
		if (pRoom)
		{
			if (!sub_6FC69F10(pGame, pRoom, pQuestDataEx->pCoords.nX, pQuestDataEx->pCoords.nY, MONSTER_IZUALGHOST, MONMODE_SKILL1, -1, 0)
				&& !sub_6FC69F10(pGame, pRoom, pQuestDataEx->pCoords.nX, pQuestDataEx->pCoords.nY, MONSTER_IZUALGHOST, MONMODE_SKILL1, 3, 0))
			{
				sub_6FC69F10(pGame, pRoom, pQuestDataEx->pCoords.nX, pQuestDataEx->pCoords.nY, MONSTER_IZUALGHOST, MONMODE_SKILL1, 5, 0);
			}
		}
	}

	pQuestDataEx->bIzualGhostIsSpawning = 0;
	return 1;
}

//D2Game.0x6FCAE140
int32_t __fastcall ACT4Q1_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	D2_ASSERT(pRoom);

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A4Q1_IZUAL);
	D2_ASSERT(pQuestData);

	D2Act4Quest1Strc* pQuestDataEx = (D2Act4Quest1Strc*)pQuestData->pQuestDataEx;

	D2RoomStrc* pTargetRoom = UNITS_GetRoom(pQuestDataEx->pTargetUnit);
	if (!pRoom)
	{
		return 0;
	}

	if (pRoom != pTargetRoom)
	{
		D2RoomStrc** ppRoomList = nullptr;
		int32_t nNumRooms = 0;
		DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

		int32_t nCounter = 0;
		while (nCounter < nNumRooms)
		{
			if (ppRoomList[nCounter] == pTargetRoom)
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

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDPENDING))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_PRIMARYGOALDONE);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_REWARDPENDING);
	}

	return 0;
}

//D2Game.0x6FCAE250
void __fastcall ACT4Q1_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nOldLevel != LEVEL_THEPANDEMONIUMFORTRESS)
	{
		return;
	}

	QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);

	if (pQuestData->fState != 2 || QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], pQuestData->nQuest, 0) == 1)
	{
		return;
	}

	QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT4Q1_UnitIterate_UpdateQuestStateFlags);

	if (!pQuestData->fLastState)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT4Q1_UnitIterate_StatusCyclerEx, 0);
	}
}

//D2Game.0x6FCAE2E0
void __fastcall ACT4Q1_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_TYRAEL2)
	{
		return;
	}

	D2Act4Quest1Strc* pQuestDataEx = (D2Act4Quest1Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bTyraelActivated != 1)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 1, 0, ACT4Q1_UnitIterate_StatusCyclerEx, 1);

	pQuestDataEx->bTyraelActivated = 0;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
}

//D2Game.0x6FCAE330
void __fastcall ACT4Q1_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_ENTERAREA) == 1)
	{
		D2Act4Quest1Strc* pQuestDataEx = (D2Act4Quest1Strc*)pQuestData->pQuestDataEx;
		pQuestDataEx->unk0x0C = 1;
		pQuestData->fLastState = 2;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q1, QFLAG_STARTED) == 1)
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 1;
	}
}

//D2Game.0x6FCAE3C0
void __fastcall ACT4Q1_OnIzualActivated(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A4Q1_IZUAL);

	if (pQuestData && pQuestData->bNotIntro && pQuestData->fLastState < 2)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 2, 0, ACT4Q1_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCAE400
int32_t __fastcall ACT4Q1_IsAnyPlayerInRangeOfIzualGhost(D2GameStrc* pGame, D2UnitStrc* pTargetUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A4Q1_IZUAL);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act4Quest1Strc* pQuestDataEx = (D2Act4Quest1Strc*)pQuestData->pQuestDataEx;
	if (!pQuestDataEx || !pQuestDataEx->bIzualGhostActivated)
	{
		return 0;
	}

	pQuestDataEx->bCloseToIzualGhost = 0;
	pQuestDataEx->pTargetUnit = pTargetUnit;
	SUNIT_IterateLivingPlayers(pGame, ACT4Q1_UnitIterate_CheckDistanceToIzualGhost, pQuestDataEx);

	return !pQuestDataEx->bCloseToIzualGhost;
}

//D2Game.0x6FCAE450
void __fastcall ACT4Q1_UnitIterate_CheckDistanceToIzualGhost(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2Act4Quest1Strc* pQuestDataEx = (D2Act4Quest1Strc*)pData;

	const int32_t nDistance = UNITS_GetDistanceToOtherUnit(pUnit, pQuestDataEx->pTargetUnit);
	if (nDistance < 5)
	{
		pQuestDataEx->bCloseToIzualGhost = 1;
	}
}

//D2Game.0x6FCAE470
void __fastcall ACT4Q1_AttachCompletionSoundToPlayers(D2GameStrc* pGame)
{
	SUNIT_IterateUnitsOfType(pGame, 0, 0, ACT4Q1_UnitIterate_AttachCompletionSound);
}

//D2Game.0x6FCAE480
int32_t __fastcall ACT4Q1_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A4Q1, QFLAG_PRIMARYGOALDONE))
	{
		SUNIT_AttachSound(pUnit, 74, pUnit);
	}

	return 0;
}

//
void __fastcall ACT4Q1_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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
