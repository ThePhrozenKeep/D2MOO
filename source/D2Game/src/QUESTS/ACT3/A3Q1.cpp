#include "QUESTS/ACT3/A3Q1.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2QuestRecord.h>
#include <D2StatList.h>

#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD37E38
D2NPCMessageTableStrc gpAct3Q1NpcMessages[] =
{
	{
		{
			{ MONSTER_ALKOR, 549, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_ALKOR, 550, 0, 2 },
			{ MONSTER_ORMUS, 551, 0, 2 },
			{ MONSTER_MESHIF2, 552, 0, 2 },
			{ MONSTER_ASHEARA, 553, 0, 2 },
			{ MONSTER_HRATLI, 554, 0, 2 },
			{ MONSTER_CAIN3, 555, 0, 2 },
			{ MONSTER_NATALYA, 556, 0, 2 },
		},
		7
	},
	{
		{
			{ MONSTER_ALKOR, 557, 0, 2 },
			{ MONSTER_ORMUS, 558, 0, 2 },
			{ MONSTER_MESHIF2, 559, 0, 2 },
			{ MONSTER_ASHEARA, 560, 0, 2 },
			{ MONSTER_HRATLI, 561, 0, 2 },
			{ MONSTER_CAIN3, 562, 0, 2 },
			{ MONSTER_NATALYA, 563, 0, 2 },
		},
		7
	},
	{
		{
			{ MONSTER_ALKOR, 564, 0, 1 },
			{ MONSTER_ORMUS, 565, 0, 2 },
			{ MONSTER_MESHIF2, 566, 0, 2 },
			{ MONSTER_ASHEARA, 567, 0, 2 },
			{ MONSTER_HRATLI, 568, 0, 2 },
			{ MONSTER_CAIN3, 569, 0, 2 },
			{ MONSTER_NATALYA, 570, 0, 2 },
		},
		7
	},
	{
		{
			{ MONSTER_CAIN3, 569, 0, 2 },
			{ MONSTER_NATALYA, 570, 0, 2 },
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


//D2Game.0x6FCA6E80
bool __fastcall ACT3Q1_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_ALKOR)
	{
		return false;
	}

	if (pQuest->fState == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_COMPLETEDBEFORE))
	{
		return true;
	}

	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' bbb'))
	{
		return true;
	}

	return false;
}

//D2Game.0x6FCA6F10
int32_t __fastcall OBJECTS_OperateFunction28_LamEsenTome(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q1_LAMESENTOME);
	if (!pQuestData || !pQuestData->bNotIntro || pObject->dwAnimMode != OBJMODE_NEUTRAL)
	{
		return 1;
	}


	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty], QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED) == 1)
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	pObject->dwDropItemCode = ' bbb';
	int32_t nItemLevel = 0;
	if (!D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pObject, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0))
	{
		return 1;
	}

	UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);

	D2Act3Quest1Strc* pQuestDataEx = (D2Act3Quest1Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bTomeActive = 1;
	pQuestDataEx->bTomeDropped = 1;
	++pQuestDataEx->nTomesInGame;
	pQuestDataEx->nTomeObjectMode = OBJMODE_OPENED;
	pQuestDataEx->nTomeGUID = pObject->dwUnitId;

	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT3Q1_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT3Q1_Callback11_ScrollMessage;

	if (pQuestData->fState != 3)
	{
		QUESTS_StateDebug(pQuestData, 3, "..\\D2Game/Quests/a3q1.cpp", 699);
	}

	if (pQuestData->fLastState != 1)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT3Q1_UnitIterate_StatusCyclerEx, 0);
	}

	return 1;
}

//D2Game.0x6FCA7040
int32_t __fastcall ACT3Q1_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A3Q1_LAMESENTOME);
	}

	return 0;
}

//D2Game.0x6FCA70B0
void __fastcall ACT3Q1_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED) == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_PRIMARYGOALDONE))
	{
		return;
	}

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' bbb'))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED) == 1)
	{
		if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
		}
	}
	else if (pQuestData->bNotIntro && pQuestData->fState < 4)
	{
		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < 6)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//D2Game.0x6FCA7190
void __fastcall ACT3Q1_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	D2Act3Quest1Strc* pQuestDataEx = (D2Act3Quest1Strc*)pQuestData->pQuestDataEx;

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED) == 1 || pQuestArg->nNPCNo != MONSTER_ALKOR)
	{
		return;
	}

	if (pQuestArg->nMessageIndex == 549)
	{
		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT3Q1_UnitIterate_StatusCyclerEx, 1);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q1_UnitIterate_UpdateQuestStateFlags);
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
	else if (pQuestArg->nMessageIndex == 564)
	{
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED))
		{
			return;
		}

		if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' bbb'))
		{
			QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' bbb');
			pQuestDataEx->bTomeBroughtToAlkor = 1;

			pQuestDataEx->nPlayerWithTomeGUID = pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1;

			pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT3Q1_Callback02_NpcDeactivate;
		}

		if (pQuestDataEx->bCanGetReward == 1)
		{
			SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT3Q1_UnitIterate_SetPrimaryGoalDone);
			pQuestDataEx->bCanGetReward = 0;
			SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT3Q1_UnitIterate_AddStatPointReward);
			SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT3Q1_UnitIterate_SetCompletionFlag);
		}

		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 13, 0, ACT3Q1_UnitIterate_StatusCyclerEx, 0);

		QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_PRIMARYGOALDONE) || !pQuestData->bNotIntro)
		{
			return;
		}

		QUESTS_SetGlobalState(pQuestArg->pGame, QUESTSTATEFLAG_A3Q1, QFLAG_PRIMARYGOALDONE);
		if (pQuestData->fState == 5)
		{
			return;
		}

		QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
		if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
		{
			FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
			exit(-1);
		}
		pQuestData->pfSeqFilter(pQuestData);
	}
}

//D2Game.0x6FCA73B0
int32_t __fastcall ACT3Q1_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q1_LAMESENTOME);
	if (pQuestData && (pQuestData->fState == 2 || pQuestData->fState == 3))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_STARTED);
	}

	return 0;
}

//D2Game.0x6FCA7420
int32_t __fastcall ACT3Q1_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return 0;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDPENDING))
	{
		return 0;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (DRLG_GetActNoFromLevelId(nLevelId) != ACT_III)
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDPENDING);
	QUESTS_UpdatePlayerFlags(pGame, pUnit);
	return 0;
}

//D2Game.0x6FCA74A0
int32_t __fastcall ACT3Q1_UnitIterate_AddStatPointReward(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDPENDING) == 1)
	{
		STATLIST_AddUnitStat(pUnit, STAT_STATPTS, 5, 0);
		QUESTS_SendLogUpdate(pUnit, QUEST_A3Q1_LAMESENTOME);

		QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDPENDING);
	}

	return 0;
}

//D2Game.0x6FCA74F0
int32_t __fastcall ACT3Q1_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED) != 1
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDPENDING) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_COMPLETEDNOW);
	}

	return 0;
}

//D2Game.0x6FCA7530
void __fastcall ACT3Q1_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_ALKOR)
	{
		return;
	}

	D2Act3Quest1Strc* pQuestDataEx = (D2Act3Quest1Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bTomeBroughtToAlkor != 1)
	{
		return;
	}

	int32_t nUnitGUID = -1;
	if (pQuestArg->pPlayer)
	{
		nUnitGUID = pQuestArg->pPlayer->dwUnitId;
	}

	if (nUnitGUID != pQuestDataEx->nPlayerWithTomeGUID)
	{
		return;
	}

	if (!pQuestArg->pPlayer || pQuestArg->pPlayer->dwClassId != PCLASS_ASSASSIN && pQuestArg->pPlayer->dwClassId != PCLASS_DRUID)
	{
		SUNIT_AttachSound(pQuestArg->pPlayer, 67, pQuestArg->pPlayer);
	}

	pQuestDataEx->bTomeBroughtToAlkor = 0;
}

//D2Game.0x6FCA7580
void __fastcall ACT3Q1_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->pNPCMessages = gpAct3Q1NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->nQuest = QUESTSTATEFLAG_A3Q1;
	pQuestData->nInitNo = 4;
	pQuestData->pfStatusFilter = (QUESTSTATUS)ACT3Q1_StatusFilterCallback;
	pQuestData->pfActiveFilter = ACT3Q1_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT3Q1_SeqCallback;
	pQuestData->nSeqId = 19;
	pQuestData->pfCallback[QUESTEVENT_ITEMPICKEDUP] = ACT3Q1_Callback04_ItemPickedUp;
	pQuestData->pfCallback[QUESTEVENT_ITEMDROPPED] = ACT3Q1_Callback05_ItemDropped;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT3Q1_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT3Q1_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT3Q1_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT3Q1_Callback13_PlayerStartedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERJOINEDGAME] = ACT3Q1_Callback14_PlayerJoinedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT3Q1_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM] = ACT3Q1_Callback09_PlayerDroppedWithQuestItem;

	D2Act3Quest1Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act3Quest1Strc);
	memset(pQuestDataEx, 0x00, sizeof(D2Act3Quest1Strc));
	pQuestData->pQuestDataEx = pQuestDataEx;

	pQuestDataEx->bTomeDropped = 0;
	pQuestDataEx->bTomeActive = 0;
	pQuestDataEx->bCanGetReward = 1;
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->tPlayerGUIDs);
}

//D2Game.0x6FCA7670
void __fastcall ACT3Q1_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel == LEVEL_LOWERKURAST && pQuestData->bNotIntro && pQuestData->fState < 1)
	{
		pQuestData->fState = 1;
	}

	if (pQuestArg->nOldLevel != LEVEL_KURASTDOCKTOWN)
	{
		return;
	}

	QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);
	if (pQuestData->fState != 2)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) == 1)
	{
		return;
	}

	if (pQuestData->fLastState != 1)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT3Q1_UnitIterate_StatusCyclerEx, 1);
	}
	QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q1_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FCA7740
void __fastcall ACT3Q1_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act3Quest1Strc* pQuestDataEx = (D2Act3Quest1Strc*)pQuestData->pQuestDataEx;

	if (!pQuestData->bNotIntro)
	{
		return;
	}

	QUESTS_DebugOutput(pQuestData->pGame, "tome acquired", __FILE__, __LINE__);
	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 2, 0, ACT3Q1_UnitIterate_StatusCyclerEx, 1);
	QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);

	if (pQuestArg->pPlayer)
	{
		QUESTS_AddPlayerGUID(&pQuestDataEx->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
	}
	else
	{
		QUESTS_AddPlayerGUID(&pQuestDataEx->tPlayerGUIDs, -1);
	}
}

//D2Game.0x6FCA77D0
void __fastcall ACT3Q1_Callback05_ItemDropped(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act3Quest1Strc* pQuestDataEx = (D2Act3Quest1Strc*)pQuestData->pQuestDataEx;

	QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));

	if (pQuestData->bNotIntro)
	{
		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT3Q1_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCA7830
void __fastcall ACT3Q1_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act3Quest1Strc* pQuestDataEx = (D2Act3Quest1Strc*)pQuestData->pQuestDataEx;

	--pQuestDataEx->nTomesInGame;

	if (!pQuestDataEx->nTomesInGame && pQuestData->bNotIntro && pQuestDataEx->bTomeActive && pQuestData->fState <= 3)
	{
		pQuestDataEx->bPlayerWithTomeDropped = 1;
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 8, 0, ACT3Q1_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCA7870
void __fastcall ACT3Q1_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act3Quest1Strc* pQuestDataEx = (D2Act3Quest1Strc*)pQuestData->pQuestDataEx;

	if (!ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' bbb'))
	{
		return;
	}

	++pQuestDataEx->nTomesInGame;
	if (pQuestDataEx->nTomesInGame == 1 && pQuestData->bNotIntro && pQuestDataEx->bPlayerWithTomeDropped)
	{
		pQuestDataEx->bPlayerWithTomeDropped = 0;
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 2, 0, ACT3Q1_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCA78D0
void __fastcall ACT3Q1_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED) == 1)
	{
		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A3Q1, QFLAG_PRIMARYGOALDONE);
		return;
	}

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' bbb'))
	{
		pQuestData->fState = 4;
		pQuestData->fLastState = 2;

		D2Act3Quest1Strc* pQuestDataEx = (D2Act3Quest1Strc*)pQuestData->pQuestDataEx;
		pQuestDataEx->nTomeObjectMode = OBJMODE_OPENED;
		pQuestDataEx->bTomeActive = 1;
		++pQuestDataEx->nTomesInGame;

		if (pQuestArg->pPlayer)
		{
			QUESTS_AddPlayerGUID(&pQuestDataEx->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
		}
		else
		{
			QUESTS_AddPlayerGUID(&pQuestDataEx->tPlayerGUIDs, -1);
		}
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_STARTED) == 1)
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 1;
	}
}

//D2Game.0x6FCA79A0
bool __fastcall ACT3Q1_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (pQuestData->fState != 5 && pQuestData->bNotIntro)
	{
		return true;
	}

	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, (D2Quests)pQuestData->nSeqId);
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

//D2Game.0x6FCA7A10
bool __fastcall ACT3Q1_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A2COMPLETED, QFLAG_REWARDGRANTED))
	{
		*pStatus = 0;
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED))
	{
		*pStatus = 2 * (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q1, QFLAG_PRIMARYGOALDONE) != 0) + 11;
		return true;
	}

	*pStatus = 0;
	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' bbb'))
	{
		*pStatus = 2;
		return true;
	}

	D2Act3Quest1Strc* pQuestDataEx = (D2Act3Quest1Strc*)pQuest->pQuestDataEx;
	const int16_t nPartyId = SUNIT_GetPartyId(pPlayer);
	if (nPartyId != -1)
	{
		pQuestDataEx->bTomeAcquired = 0;
		PARTY_IteratePartyMembers(pQuest->pGame, nPartyId, ACT3Q1_UnitIterate_CheckForTomeItem, 0);
		if (pQuestDataEx->bTomeAcquired)
		{
			*pStatus = 2;
			return true;
		}
	}

	if (!pQuest->bNotIntro)
	{
		return true;
	}

	if (pQuest->fState > 3)
	{
		*pStatus = 12;

		D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayer);
		if (!pRoom)
		{
			return true;
		}

		const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
		if (DRLG_GetActNoFromLevelId(nLevelId) == ACT_III && pQuestDataEx->nTomesInGame)
		{
			*pStatus = 1;
		}
		return true;
	}

	*pStatus = 1;
	if (pQuestDataEx->bTomeActive == 1 && !pQuestDataEx->nTomesInGame)
	{
		*pStatus = (pQuest->pGame->nGameType != 3) + 8;
	}

	return true;
}

//D2Game.0x6FCA7B40
void __fastcall ACT3Q1_UnitIterate_CheckForTomeItem(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (ITEMS_FindQuestItem(pGame, pUnit, ' bbb'))
	{
		((D2Act3Quest1Strc*)QUESTS_GetQuestData(pGame, QUEST_A3Q1_LAMESENTOME)->pQuestDataEx)->bTomeAcquired = 1;
	}
}

//
void __fastcall ACT3Q1_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_RemovePlayerGUID(pQuestData, pQuestArg);
}
