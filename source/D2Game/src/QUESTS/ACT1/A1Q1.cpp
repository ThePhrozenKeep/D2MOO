#include "QUESTS/ACT1/A1Q1.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2QuestRecord.h>
#include <D2StatList.h>
#include <Units/Units.h>

#include "GAME/Game.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "MONSTER/MonsterRegion.h"

//D2Game.0x6FD31F38
D2NPCMessageTableStrc gpAct1Q1NpcMessages[] =
{
	{
		{
			{ MONSTER_AKARA, 64, 0, 0 },
		},
		1
	},
	{
		{
			{ MONSTER_AKARA, 65, 0, 2 },
			{ MONSTER_WARRIV1, 70, 0, 2 },
			{ MONSTER_GHEED, 69, 0, 2 },
			{ MONSTER_KASHYA, 66, 0, 2 },
			{ MONSTER_CHARSI, 67, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_KASHYA, 72, 0, 2 },
			{ MONSTER_WARRIV1, 75, 0, 2 },
			{ MONSTER_CHARSI, 73, 0, 2 },
			{ MONSTER_AKARA, 71, 0, 2 },
			{ MONSTER_GHEED, 74, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_KASHYA, 77, 0, 2 },
			{ MONSTER_WARRIV1, 80, 0, 2 },
			{ MONSTER_CHARSI, 78, 0, 2 },
			{ MONSTER_AKARA, 76, 0, 0 },
			{ MONSTER_GHEED, 79, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_WARRIV1, 80, 0, 2 },
			{ MONSTER_CHARSI, 78, 0, 2 },
			{ MONSTER_GHEED, 79, 0, 2 },
		},
		3
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	},
	{
		{
			{ 1, 0, 0, 2 },
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


//D2Game.0x6FC97920
void __fastcall ACT1Q1_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (nLevelId && DRLG_GetActNoFromLevelId(nLevelId) == ACT_I)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_PRIMARYGOALDONE);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDPENDING);
	}
}

//D2Game.0x6FC97990
int32_t __fastcall ACT1Q1_GetMonstersToBeKilled(D2QuestDataStrc* pQuestData)
{
	return ((D2Act1Quest1Strc*)pQuestData->pQuestDataEx)->nMonstersLeft;
}

//D2Game.0x6FC979A0
bool __fastcall ACT1Q1_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_AKARA || QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDGRANTED) == 1)
	{
		return false;
	}

	if (pQuest->bNotIntro == 1 && pQuest->fState == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDPENDING))
	{
		return true;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDPENDING) == 1)
	{
		return true;
	}

	return false;
}

//D2Game.0x6FC97A10
void __fastcall ACT1Q1_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));

	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT1Q1_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT1Q1_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT1Q1_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT1Q1_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT1Q1_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT1Q1_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT1Q1_Callback13_PlayerStartedGame;

	pQuestData->pNPCMessages = gpAct1Q1NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 1;
	D2Act1Quest1Strc* pQuestDataEx = (D2Act1Quest1Strc*)FOG_AllocPool(pQuestData->pGame->pMemoryPool, sizeof(D2Act1Quest1Strc), __FILE__, __LINE__, 0);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->nQuest = QUESTSTATEFLAG_A1Q1;
	pQuestData->pfStatusFilter = 0;
	pQuestData->nInitNo = 4;
	pQuestData->pfActiveFilter = ACT1Q1_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT1Q1_SeqCallback;
	pQuestData->nSeqId = 2;

	memset(pQuestDataEx, 0x00, sizeof(D2Act1Quest1Strc));
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->pQuestGUID);
}

//D2Game.0x6FC97AE0
void __fastcall ACT1Q1_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg || !pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_AKARA)
	{
		return;
	}

	D2Act1Quest1Strc* pQuestDataEx = (D2Act1Quest1Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bAkaraActivated != 1)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q1_UnitIterate_StatusCyclerEx, 1);
	pQuestDataEx->bAkaraActivated = 0;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
}

//D2Game.0x6FC97B30
int32_t __fastcall ACT1Q1_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A1Q1_DENOFEVIL);
	}

	return 0;
}

//D2Game.0x6FC97BA0
void __fastcall ACT1Q1_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest1Strc* pQuestDataEx = (D2Act1Quest1Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->nNPCNo != MONSTER_AKARA)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

	if (pQuestArg->nMessageIndex == 64)
	{
		pQuestDataEx->bAkaraActivated = 1;
		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q1_UnitIterate_UpdateQuestStateFlags);
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
	else if (pQuestArg->nMessageIndex == 76)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING) != 1)
		{
			return;
		}

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
				QUESTS_UnitIterate(pQuestData, 13, 0, ACT1Q1_UnitIterate_StatusCyclerEx, 0);
			}
			pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
		}

		QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED);
		QUESTRECORD_ClearQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING);
		QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, pQuestData->nQuest);
		STATLIST_AddUnitStat(pQuestArg->pPlayer, STAT_SKILLPTS, 1, 0);

		QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
}

//D2Game.0x6FC97D30
int32_t __fastcall ACT1Q1_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q1_DENOFEVIL);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		if (pQuestData->fLastState == 1)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_LEAVETOWN);
		}
		else
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_ENTERAREA);
		}
	}

	return 0;
}

//D2Game.0x6FC97DC0
void __fastcall ACT1Q1_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4
	};

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, (pQuestArg->pTarget ? pQuestArg->pTarget->dwClassId : -1), 3);
	}
	else if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)) == 1)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, (pQuestArg->pTarget ? pQuestArg->pTarget->dwClassId : -1), 4);
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) != 1 && (pQuestData->fState < 4 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE)))
	{
		if (!pQuestData->bNotIntro)
		{
			return;
		}

		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < 8)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, (pQuestArg->pTarget ? pQuestArg->pTarget->dwClassId : -1), nIndex);
		}
	}
}

//D2Game.0x6FC97ED0
void __fastcall ACT1Q1_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestData->bNotIntro)
	{
		return;
	}

	D2Act1Quest1Strc* pQuestDataEx = (D2Act1Quest1Strc*)pQuestData->pQuestDataEx;
	D2MonsterRegionStrc* pMonRegion = pQuestArg->pGame->pMonReg[LEVEL_DENOFEVIL];

	D2_ASSERT(pMonRegion);

	pQuestDataEx->nMonstersLeft = pMonRegion->dwMonSpawnCount - pMonRegion->dwMonKillCount;

	if (pQuestArg->pPlayer)
	{
		if (!QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->pQuestGUID, pQuestArg->pPlayer->dwUnitId))
		{
			QUESTS_AddPlayerGUID(&pQuestDataEx->pQuestGUID, pQuestArg->pPlayer->dwUnitId);
		}
	}

	const int32_t nPopulatedRooms = DUNGEON_GetNumberOfPopulatedRoomsInLevel(pQuestArg->pGame->pAct[DRLG_GetActNoFromLevelId(LEVEL_DENOFEVIL)], LEVEL_DENOFEVIL);

	if (nPopulatedRooms > pMonRegion->unk0x04 || pMonRegion->dwMonKillCount != pMonRegion->dwMonSpawnCount)
	{
		if (nPopulatedRooms > pMonRegion->unk0x04 || pQuestDataEx->nMonstersLeft > 5)
		{
			if (pQuestData->fLastState == 4 && pQuestDataEx->nMonstersLeft > 5)
			{
				pQuestData->dwFlags |= 0x00000020;
				QUESTS_UnitIterate(pQuestData, 4, 0, ACT1Q1_UnitIterate_StatusCyclerEx, 1);
			}
		}
		else
		{
			pQuestData->dwFlags |= 0x00000020;
			QUESTS_UnitIterate(pQuestData, 4, 0, ACT1Q1_UnitIterate_StatusCyclerEx, 1);
			pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
		}
	}
	else
	{
		QUESTS_DebugOutput(pQuestData->pGame, "Finished quest", __FILE__, __LINE__);
		pQuestDataEx->bFinished = 1;
		pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
		QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
		pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = nullptr;
		QUESTS_SetGlobalState(pQuestArg->pGame, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE);
		QUESTS_GUIDUpdate(pQuestArg->pGame, &pQuestDataEx->pQuestGUID, pQuestData->nQuest, 0);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q1_UnitIterate_SetCompletionFlag);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q1_UnitIterate_AttachCompletionSound);
		QUESTS_TriggerFX(pQuestData->pGame, 0);

		if (!pQuestDataEx->bTimerActive)
		{
			pQuestDataEx->bTimerActive = 1;
			QUESTS_CreateTimer(pQuestData, ACT1Q1_Timer_StatusCycler, 8);
		}
	}
}

//D2Game.0x6FC980C0
int32_t __fastcall ACT1Q1_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_COMPLETEDNOW);
	QUESTS_SendLogUpdateEx(pUnit, QUEST_A1Q1_DENOFEVIL, 0);
	QUESTS_UpdatePlayerFlags(pGame, pUnit);
	return 0;
}

//D2Game.0x6FC98120
int32_t __fastcall ACT1Q1_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A1Q1, QFLAG_PRIMARYGOALDONE))
	{
		SUNIT_AttachSound(pUnit, 35, pUnit);
	}

	return 0;
}

//D2Game.0x6FC98160
int32_t __fastcall ACT1Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A1Q1, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT1Q1_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FC981B0
bool __fastcall ACT1Q1_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act1Quest1Strc* pQuestDataEx = (D2Act1Quest1Strc*)pQuestData->pQuestDataEx;

	if (pQuestData->fState == 4)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 5, 0, ACT1Q1_UnitIterate_StatusCyclerEx, 1);
	}

	pQuestDataEx->bTimerActive = 0;

	return 1;
}

//D2Game.0x6FC981E0
void __fastcall ACT1Q1_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest1Strc* pQuestDataEx = (D2Act1Quest1Strc*)pQuestData->pQuestDataEx;

	if (pQuestArg->nNewLevel == LEVEL_DENOFEVIL)
	{
		if (!pQuestData->bNotIntro)
		{
			return;
		}

		if (pQuestData->fState == 1 || pQuestData->fState == 2)
		{
			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		}

		pQuestDataEx->bEnteredDen = 1;
		if (pQuestData->fLastState >= 2)
		{
			if (pQuestData->fState != 1 && pQuestData->fState != 2)
			{
				return;
			}
		}
		else
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 2, 0, ACT1Q1_UnitIterate_StatusCyclerEx, 1);
			pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
		}

		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q1_UnitIterate_UpdateQuestStateFlags);
	}
	else if (pQuestArg->nOldLevel == LEVEL_ROGUEENCAMPMENT)
	{
		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);
		if (pQuestData->fState != 2)
		{
			return;
		}

		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING) == 1)
		{
			return;
		}

		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q1_UnitIterate_UpdateQuestStateFlags);

		if (pQuestData->fLastState == 1)
		{
			return;
		}

		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q1_UnitIterate_StatusCyclerEx, 1);
		pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	}
}

//D2Game.0x6FC98330
bool __fastcall ACT1Q1_SeqCallback(D2QuestDataStrc* pQuestData)
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
		FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
		exit(-1);
	}

	return pQuest->pfSeqFilter(pQuest);
}

//D2Game.0x6FC983A0
void __fastcall ACT1Q1_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_ENTERAREA) == 1)
	{
		pQuestData->fLastState = 2;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q1, QFLAG_STARTED) == 1)
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 1;
	}
}

//D2Game.0x6FC98430
bool __fastcall ACT1Q1_CanClientFXBeTriggered(D2QuestDataStrc* pQuestData)
{
	return pQuestData->bNotIntro && pQuestData->fState >= 4;
}

//
void __fastcall ACT1Q1_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_FastRemovePlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));

	D2Act1Quest1Strc* pQuestDataEx = (D2Act1Quest1Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->pPlayer)
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pQuestGUID, pQuestArg->pPlayer->dwUnitId);
	}
	else
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pQuestGUID, -1);
	}
}
