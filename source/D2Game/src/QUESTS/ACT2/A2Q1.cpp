#include "QUESTS/ACT2/A2Q1.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD34B38
D2NPCMessageTableStrc gpAct2Q1NpcMessages[] =
{
	{
		{
			{ MONSTER_ATMA, 304, 0, 0 },
		},
		1
	},
	{
		{
			{ MONSTER_ATMA, 310, 0, 2, },
			{ MONSTER_WARRIV2, 307, 0, 2, },
			{ MONSTER_GREIZ, 305, 0, 2, },
			{ MONSTER_ELZIX, 306, 0, 2, },
			{ MONSTER_DROGNAN, 312, 0, 2, },
			{ MONSTER_LYSANDER, 313, 0, 2, },
			{ MONSTER_CAIN2, 314, 0, 2, },
			{ MONSTER_MESHIF1, 311, 0, 2, },
			{ MONSTER_GEGLASH, 308, 0, 2, },
			{ MONSTER_FARA, 309, 0, 2, },
		},
		10
	},
	{
		{
			{ MONSTER_ATMA, 317, 0, 2, },
			{ MONSTER_WARRIV2, 315, 0, 2, },
			{ MONSTER_GREIZ, 318, 0, 2, },
			{ MONSTER_ELZIX, 320, 0, 2, },
			{ MONSTER_DROGNAN, 322, 0, 2, },
			{ MONSTER_LYSANDER, 321, 0, 2, },
			{ MONSTER_CAIN2, 324, 0, 2, },
			{ MONSTER_MESHIF1, 316, 0, 2, },
			{ MONSTER_GEGLASH, 319, 0, 2, },
			{ MONSTER_FARA, 323, 0, 2, },
		},
		10
	},
	{
		{
			{ MONSTER_ATMA, 334, 0, 1, },
			{ MONSTER_GREIZ, 325, 0, 2, },
			{ MONSTER_ELZIX, 330, 0, 2, },
			{ MONSTER_DROGNAN, 326, 0, 2, },
			{ MONSTER_LYSANDER, 327, 0, 2, },
			{ MONSTER_CAIN2, 333, 0, 2, },
			{ MONSTER_MESHIF1, 328, 0, 2, },
			{ MONSTER_GEGLASH, 329, 0, 2, },
			{ MONSTER_FARA, 332, 0, 2, },
		},
		9
	},
	{
		{
			{ MONSTER_JERHYN, 331, 0, 2, },
			{ MONSTER_ATMA, 334, 0, 2, },
			{ MONSTER_GREIZ, 325, 0, 2, },
			{ MONSTER_ELZIX, 330, 0, 2, },
			{ MONSTER_DROGNAN, 326, 0, 2, },
			{ MONSTER_CAIN2, 333, 0, 2, },
			{ MONSTER_MESHIF1, 328, 0, 2, },
			{ MONSTER_GEGLASH, 329, 0, 2, },
			{ MONSTER_FARA, 332, 0, 2, },
		},
		9
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
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FC9F5A0
bool __fastcall ACT2Q1_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_ATMA || QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDGRANTED) == 1)
	{
		return false;
	}

	if (pQuest->bNotIntro && pQuest->fState == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		return true;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDPENDING) == 1)
	{
		return true;
	}

	return false;
}

//D2Game.0x6FC9F610
void __fastcall ACT2Q1_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_II)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDPENDING);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_CUSTOM1);
	QUESTS_UpdatePlayerFlags(pGame, pUnit);
}

//D2Game.0x6FC9F690
void __fastcall ACT2Q1_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT2Q1_Callback03_ChangedLevel;
	pQuestData->bActive = 1;
	pQuestData->fState = 1;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT2Q1_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT2Q1_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT2Q1_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT2Q1_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT2Q1_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT2Q1_Callback13_PlayerStartedGame;
	pQuestData->nSeqId = 13;
	pQuestData->pfSeqFilter = ACT2Q1_SeqCallback;
	pQuestData->pNPCMessages = gpAct2Q1NpcMessages;

	D2Act2Quest1Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act2Quest1Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->nQuest = QUESTSTATEFLAG_A2Q1;
	pQuestData->pfStatusFilter = nullptr;
	pQuestData->nInitNo = 4;
	pQuestData->pfActiveFilter = ACT2Q1_ActiveFilterCallback;

	memset(pQuestDataEx, 0x00, sizeof(D2Act2Quest1Strc));
}

//D2Game.0x6FC9F770
void __fastcall ACT2Q1_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_ATMA)
	{
		return;
	}

	D2Act2Quest1Strc* pQuestDataEx = (D2Act2Quest1Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bAtmaActivated != 1)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 1, 0, ACT2Q1_UnitIterate_StatusCyclerEx, 1);
	pQuestDataEx->bAtmaActivated = 0;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
}

//D2Game.0x6FC9F7C0
int32_t __fastcall ACT2Q1_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A2Q1_RADAMENT);
	}

	return 0;
}

//D2Game.0x6FC9F830
void __fastcall ACT2Q1_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (pQuestArg->nNPCNo != MONSTER_ATMA)
	{
		return;
	}

	D2Act2Quest1Strc* pQuestDataEx = (D2Act2Quest1Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->nMessageIndex == 304)
	{
		pQuestDataEx->bAtmaActivated = 1;
		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q1_UnitIterate_UpdateQuestStateFlags);
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
	else if (pQuestArg->nMessageIndex == 334)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
		{
			return;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE))
		{
			if (pQuestData->fState != 5)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 13, 0, ACT2Q1_UnitIterate_StatusCyclerEx, 1);
				pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
				QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);

				if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
				{
					FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
					exit(-1);
				}
				pQuestData->pfSeqFilter(pQuestData);
			}

			if (!pQuestData->bNotIntro)
			{
				QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q1, QFLAG_PRIMARYGOALDONE);
			}
		}
		else
		{
			if (pQuestDataEx->bRewardPending)
			{
				D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, pQuestData->nSeqId);
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
		}

		QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED);
		QUESTRECORD_ClearQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING);

		QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
}

//D2Game.0x6FC9FA20
int32_t __fastcall ACT2Q1_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q1_RADAMENT);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		if (pQuestData->fLastState == 1)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_LEAVETOWN);
		}
		else
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_ENTERAREA);
		}
	}

	return 0;
}

//D2Game.0x6FC9FAB0
void __fastcall ACT2Q1_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4, 0
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
	}
	else if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) != 1)
	{
		if (!pQuestData->fState || (pQuestData->fState >= 4 && !QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE)))
		{
			return;
		}

		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < 9)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//D2Game.0x6FC9FBB0
void __fastcall ACT2Q1_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestData->bNotIntro)
	{
		return;
	}

	QUESTS_DebugOutput(pQuestData->pGame, "killed radament for quest", __FILE__, __LINE__);
	QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
	QUESTS_SetGlobalState(pQuestArg->pGame, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE);

	D2Act2Quest1Strc* pQuestDataEx = (D2Act2Quest1Strc*)pQuestData->pQuestDataEx;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	pQuestDataEx->bRadamentKilled = 1;
	pQuestDataEx->pRoom = UNITS_GetRoom(pQuestArg->pTarget);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT2Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT2Q1_UnitIterate_SetCompletionFlag);
	pQuestDataEx->nSkillBookDropCount = 0;
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT2Q1_UnitIterate_DetermineSkillBookDropCount);

	if (!pQuestDataEx->bStatusTimerStarted)
	{
		pQuestDataEx->bStatusTimerStarted = 1;
		QUESTS_CreateTimer(pQuestData, ACT2Q1_Timer_StatusCycler, 12);
	}

	if (!pQuestDataEx->nSkillBookDropCount)
	{
		return;
	}

	pQuestArg->pTarget->dwDropItemCode = ' ssa';
	for (int32_t i = 0; i < pQuestDataEx->nSkillBookDropCount; ++i)
	{
		int32_t nItemLevel = 0;
		D2GAME_DropItemAtUnit_6FC4FEC0(pQuestData->pGame, pQuestArg->pTarget, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0);
	}
}

//D2Game.0x6FC9FCB0
int32_t __fastcall ACT2Q1_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDPENDING) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A2Q1_RADAMENT, 0);
	}

	return 0;
}

//D2Game.0x6FC9FD10
int32_t __fastcall ACT2Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q1_RADAMENT);
	D2_ASSERT(pQuestData);

	D2Act2Quest1Strc* pQuestDataEx = (D2Act2Quest1Strc*)pQuestData->pQuestDataEx;

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

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_CUSTOM1);
	QUESTS_UpdatePlayerFlags(pQuestData->pGame, pUnit);
	
	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT2Q1_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FC9FE70
bool __fastcall ACT2Q1_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act2Quest1Strc* pQuestDataEx = (D2Act2Quest1Strc*)pQuestData->pQuestDataEx;

	if (pQuestData->fState == 4)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 3, 0, ACT2Q1_UnitIterate_StatusCyclerEx, 1);
	}
	pQuestDataEx->bStatusTimerStarted = 0;

	return 1;
}

//D2Game.0x6FC9FEA0
int32_t __fastcall ACT2Q1_UnitIterate_DetermineSkillBookDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q1, QFLAG_CUSTOM1))
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q1_RADAMENT);
		if (pQuestData && !ITEMS_FindQuestItem(pGame, pUnit, ' ssa'))
		{
			++((D2Act2Quest1Strc*)pQuestData->pQuestDataEx)->nSkillBookDropCount;
		}
	}

	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q1, QFLAG_PRIMARYGOALDONE))
	{
		SUNIT_AttachSound(pUnit, 50, pUnit);
	}

	return 0;
}

//D2Game.0x6FC9FF20
void __fastcall ACT2Q1_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nOldLevel != LEVEL_LUTGHOLEIN)
	{
		return;
	}

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
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q1_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FC9FFA0
bool __fastcall ACT2Q1_SeqCallback(D2QuestDataStrc* pQuestData)
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

//D2Game.0x6FCA0010
void __fastcall ACT2Q1_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDGRANTED) == 1)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_CUSTOM1) == 1 && !ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' ssa'))
		{
			QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDGRANTED);
			QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDPENDING);
			QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_COMPLETEDBEFORE);
			QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_CUSTOM1);
		}
		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q1, QFLAG_PRIMARYGOALDONE);
		pQuestData->bNotIntro = 0;
		pQuestData->fState = 0;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_COMPLETEDBEFORE) == QFLAG_REWARDPENDING)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_REWARDPENDING) == 1)
		{
			((D2Act2Quest1Strc*)pQuestData->pQuestDataEx)->bRewardPending = 1;
		}
		pQuestData->fState = 0;
		pQuestData->bNotIntro = 0;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_ENTERAREA) == 1)
	{
		pQuestData->fLastState = 2;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_STARTED) == 1)
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 1;
	}
}

//D2Game.0x6FCA0120
void __fastcall ACT2Q1_OnRadamentActivated(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q1_RADAMENT);
	if (!pQuestData || pQuestData->bNotIntro != 1 || (pQuestData->fState >= 3u && pQuestData->fLastState >= 2u))
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_SEWERSLEV3)
	{
		return;
	}

	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	
	bool bReturn = true;
	if (pQuestData->fState < 3)
	{
		bReturn = false;
		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
	}

	if (pQuestData->fLastState >= 2)
	{
		if (bReturn)
		{
			return;
		}
	}
	else
	{
		D2Act2Quest1Strc* pQuestDataEx = (D2Act2Quest1Strc*)pQuestData->pQuestDataEx;
		pQuestData->dwFlags &= 0xFFFFFF00;

		bool bIterate = false;
		if (!pQuestDataEx->unk0x09)
		{
			pQuestDataEx->unk0x09 = 1;
			bIterate = true;
		}
		QUESTS_UnitIterate(pQuestData, 2, 0, ACT2Q1_UnitIterate_StatusCyclerEx, bIterate);
	}
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q1_UnitIterate_UpdateQuestStateFlags);
}

//
void __fastcall ACT2Q1_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_RemovePlayerGUID(pQuestData, pQuestArg);
}
