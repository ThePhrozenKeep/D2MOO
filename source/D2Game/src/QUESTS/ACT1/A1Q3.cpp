#include "QUESTS/ACT1/A1Q3.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <D2Items.h>
#include <D2QuestRecord.h>
#include <D2StatList.h>

#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT1/A1Q2.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD32A20
D2NPCMessageTableStrc gpAct1Q3NpcMessages[] =
{
	{
		{
			{ MONSTER_CHARSI, 146, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_AKARA, 148, 0, 2 },
			{ MONSTER_KASHYA, 149, 0, 2 },
			{ MONSTER_CAIN5, 147, 0, 2 },
			{ MONSTER_CHARSI, 150, 0, 0 },
			{ MONSTER_GHEED, 151, 0, 2 },
			{ MONSTER_WARRIV1, 153, 0, 2 },
		},
		6
	},
	{
		{
			{ MONSTER_KASHYA, 156, 0, 2 },
			{ MONSTER_WARRIV1, 159, 0, 2 },
			{ MONSTER_CHARSI, 157, 0, 2 },
			{ MONSTER_CAIN5, 154, 0, 2 },
			{ MONSTER_AKARA, 155, 0, 2 },
			{ MONSTER_GHEED, 158, 0, 2 },
		},
		6
	},
	{
		{
			{ MONSTER_KASHYA, 162, 0, 2 },
			{ MONSTER_WARRIV1, 165, 0, 2 },
			{ MONSTER_CHARSI, 163, 0, 0 },
			{ MONSTER_CAIN5, 160, 0, 2 },
			{ MONSTER_AKARA, 161, 0, 2 },
			{ MONSTER_GHEED, 164, 0, 2 },
		},
		6
	},
	{
		{
			{ MONSTER_KASHYA, 162, 0, 2 },
			{ MONSTER_WARRIV1, 165, 0, 2 },
			{ MONSTER_CAIN5, 160, 0, 2 },
			{ MONSTER_AKARA, 161, 0, 2 },
			{ MONSTER_GHEED, 164, 0, 2 },
		},
		5
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FC98EF0
void __fastcall ACT1Q3_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	if (STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0) < 8)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDPENDING);
}

//D2Game.0x6FC98F50
int32_t __fastcall ACT1Q3_SetRewardGranted(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED);
	QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDPENDING);

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_COMPLETEDBEFORE))
	{
		return 1;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q3_MALUS);
	D2_ASSERT(pQuestData);

	pQuestData->bActive = false;

	return QUESTS_DebugOutput(pGame, "deactivated tools of trade", __FILE__, __LINE__);
}

//D2Game.0x6FC98FD0
int32_t __fastcall OBJECTS_OperateFunction21_HoradrimMalus(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q3_MALUS);
	if (!pQuestData)
	{
		return 1;
	}

	if (!pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
	{
		return 1;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDPENDING) == 1)
	{
		return 1;
	}

	if (STATLIST_GetUnitBaseStat(pOp->pPlayer, STAT_LEVEL, 0) < 8)
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	int32_t nItemLevel = 0;
	pObject->dwDropItemCode = ' mdh';
	if (!D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pObject, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0))
	{
		return 1;
	}

	UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);

	D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bMalusInitialized = 1;
	pQuestDataEx->nMalusObjectMode = OBJMODE_OPENED;
	pQuestDataEx->nMalusGUID = pObject->dwUnitId;
	++pQuestDataEx->nMalusItemsInGame;

	if (pQuestData->fState != 4)
	{
		QUESTS_StateDebug(pQuestData, 4, "..\\D2Game/Quests/a1q3.cpp", 706);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q3_UnitIterate_UpdateQuestStateFlags);
	}

	if (pQuestData->fLastState != 1)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 0);
	}

	return 1;
}

//D2Game.0x6FC99150
int32_t __fastcall ACT1Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A1Q3_MALUS);
	}

	return 0;
}

//D2Game.0x6FC991C0
int32_t __fastcall ACT1Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q3_MALUS);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3 || pQuestData->fState == 4)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_LEAVETOWN);
	}

	return 0;
}

//D2Game.0x6FC99240
bool __fastcall ACT1Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_CHARSI || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED) == 1)
	{
		return false;
	}

	if (pQuest->fState == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDPENDING))
	{
		return true;
	}

	if (STATLIST_GetUnitBaseStat(pPlayer, STAT_LEVEL, 0) >= 8 && ITEMS_FindQuestItem(pPlayer->pGame, pPlayer, ' mdh'))
	{
		return true;
	}

	return false;
}

//D2Game.0x6FC992C0
void __fastcall ACT1Q3_UnitIterate_CheckForMalus(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!ITEMS_FindQuestItem(pGame, pUnit, ' mdh'))
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q3_MALUS);
	if (pQuestData)
	{
		((D2Act1Quest3Strc*)pQuestData->pQuestDataEx)->bMalusAcquired = 1;
	}
}

//D2Game.0x6FC992F0
bool __fastcall ACT1Q3_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	*pStatus = 0;

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDPENDING))
	{
		*pStatus = 10;
		return true;
	}

	if (ITEMS_FindQuestItem(pPlayer->pGame, pPlayer, ' mdh'))
	{
		if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED))
		{
			*pStatus = 2;
		}

		return true;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pPlayer);
	if (nPartyId != -1)
	{
		D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)pQuest->pQuestDataEx;
		pQuestDataEx->bMalusAcquired = 0;
		PARTY_IteratePartyMembers(pQuest->pGame, nPartyId, ACT1Q3_UnitIterate_CheckForMalus, 0);

		if (pQuestDataEx->bMalusAcquired)
		{
			if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED))
			{
				*pStatus = 2;
			}

			return true;
		}
	}

	if (!pQuest->bNotIntro)
	{
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A1Q3, QFLAG_PRIMARYGOALDONE))
	{
		*pStatus = QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDPENDING) != 0 ? 10 : 13;
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A1Q3, QFLAG_COMPLETEDNOW))
	{
		*pStatus = 12;
		return true;
	}

	if (pQuest->fState < 5)
	{
		*pStatus = pQuest->fLastState;
		return true;
	}

	if (QUESTS_GetGlobalState(pQuest->pGame, QUESTSTATEFLAG_A1Q3, QFLAG_PRIMARYGOALDONE))
	{
		*pStatus = 8 * (STATLIST_GetUnitBaseStat(pPlayer, STAT_LEVEL, 0) >= 8) + 4;
	}

	return true;
}

//D2Game.0x6FC99430
void __fastcall ACT1Q3_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT1Q3_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT1Q3_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM] = ACT1Q3_Callback09_PlayerDroppedWithQuestItem;
	pQuestData->pfCallback[QUESTEVENT_PLAYERJOINEDGAME] = ACT1Q3_Callback14_PlayerJoinedGame;
	pQuestData->pfCallback[QUESTEVENT_ITEMPICKEDUP] = ACT1Q3_Callback04_ItemPickedUp;
	pQuestData->pfCallback[6] = ACT1Q3_Callback06;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT1Q3_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT1Q3_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT1Q3_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT1Q3_Callback13_PlayerStartedGame;
	pQuestData->pfSeqFilter = ACT1Q3_SeqCallback;
	pQuestData->pNPCMessages = gpAct1Q3NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->nQuest = QUESTSTATEFLAG_A1Q3;

	D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)FOG_AllocPool(pQuestData->pGame->pMemoryPool, sizeof(D2Act1Quest3Strc), __FILE__, __LINE__, 0);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->fState = 0;
	pQuestData->pfStatusFilter = ACT1Q3_StatusFilterCallback;
	pQuestData->nInitNo = 5;
	pQuestData->pfActiveFilter = ACT1Q3_ActiveFilterCallback;
	pQuestData->nSeqId = 6;

	memset(pQuestDataEx, 0x00, sizeof(D2Act1Quest3Strc));
	pQuestDataEx->unk0x00 = 0;
	pQuestDataEx->nMalusGUID = 0;
	pQuestDataEx->unused0x09[7] = 0;
	pQuestDataEx->unk0x12 = 0;
	pQuestDataEx->bMalusInitialized = 0;
	pQuestDataEx->unk0x08 = 0;
	pQuestDataEx->bCharsiIntroActivated = 0;
	pQuestDataEx->bCharsiEndActivated = 0;
	pQuestDataEx->nMalusObjectMode = 0;
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->tPlayerGUIDs);
}

//D2Game.0x6FC99540
void __fastcall ACT1Q3_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_CHARSI)
	{
		return;
	}

	D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bCharsiIntroActivated == 1)
	{
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q3_UnitIterate_UpdateQuestStateFlags);
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 1);
		pQuestDataEx->bCharsiIntroActivated = 0;
	}
	else if (pQuestDataEx->bCharsiEndActivated == 1)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 13, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 1);
		pQuestDataEx->bCharsiEndActivated = 0;
	}
}

//D2Game.0x6FC995C0
void __fastcall ACT1Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (pQuestArg->nNPCNo != MONSTER_CHARSI)
	{
		return;
	}

	D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->nMessageIndex == 163)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED))
		{
			return;
		}

		int32_t nUnitId = -1;
		if (pQuestArg->pPlayer)
		{
			nUnitId = pQuestArg->pPlayer->dwUnitId;
		}

		QUESTS_AddPlayerGUID(&pQuestDataEx->tPlayerGUIDs, nUnitId);
		if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' mdh'))
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_PRIMARYGOALDONE);
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDPENDING);
			QUESTS_UpdatePlayerFlags(pQuestData->pGame, pQuestArg->pPlayer);
			QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' mdh');
			--pQuestDataEx->nMalusItemsInGame;

			const int16_t nPartyId = SUNIT_GetPartyId(pQuestArg->pPlayer);
			if (nPartyId != -1)
			{
				PARTY_IteratePartyMembers(pQuestArg->pGame, nPartyId, ACT1Q2_UnitIterate_SetPrimaryGoalDone, 0);
			}

			if (pQuestData->bNotIntro)
			{
				if (pQuestData->fState == 4)
				{
					QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
					pQuestDataEx->bCharsiEndActivated = 1;
					QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A1Q3, QFLAG_PRIMARYGOALDONE);
					if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
					{
						FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
						exit(-1);
					}
					pQuestData->pfSeqFilter(pQuestData);
					QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
					return;
				}

				if (pQuestDataEx->bStartingPlayerHasMalus)
				{
					D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, pQuestData->nSeqId);
					if (pQuest)
					{
						if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
						{
							FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
							exit(-1);
						}
						pQuest->pfSeqFilter(pQuest);
					}
				}
			}
		}

		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
	else if (pQuestArg->nMessageIndex == 146)
	{
		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
		pQuestDataEx->bCharsiIntroActivated = 1;
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
}

//D2Game.0x6FC997E0
void __fastcall ACT1Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED) == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_PRIMARYGOALDONE))
	{
		return;
	}

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' mdh'))
	{
		if (STATLIST_GetUnitBaseStat(pQuestArg->pPlayer, STAT_LEVEL, 0) >= 8 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, 0))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
		}
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_REWARDGRANTED) != 1)
	{
		if (!pQuestData->fState || pQuestData->fState == 4)
		{
			return;
		}

		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < ARRAY_SIZE(nIndices))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//D2Game.0x6FC998D0
void __fastcall ACT1Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestData->bNotIntro)
	{
		pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = nullptr;
		return;
	}

	if (pQuestArg->nOldLevel != LEVEL_ROGUEENCAMPMENT)
	{
		return;
	}

	if (pQuestData->fState != 2)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	if (pQuestData->fLastState != 1)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 1);
	}

	QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q3_UnitIterate_UpdateQuestStateFlags);
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = nullptr;
}

//D2Game.0x6FC99990
void __fastcall ACT1Q3_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_CUSTOM2))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_CUSTOM2);
		SUNIT_AttachSound(pQuestArg->pPlayer, 36, pQuestArg->pPlayer);
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 2, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 1);
}

//D2Game.0x6FC999F0
void __fastcall ACT1Q3_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' mdh'))
	{
		return;
	}

	if (!pQuestData->bNotIntro || pQuestData->fState == 5)
	{
		return;
	}

	D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)pQuestData->pQuestDataEx;
	++pQuestDataEx->nMalusItemsInGame;

	if (pQuestDataEx->nMalusItemsInGame == 1 && pQuestDataEx->bMalusInitialized && pQuestDataEx->nMalusObjectMode == OBJMODE_OPENED)
	{
		pQuestData->fLastState = 2;
	}
}

//D2Game.0x6FC99A50
void __fastcall ACT1Q3_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestData->bNotIntro || pQuestData->fState == 5)
	{
		return;
	}

	D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)pQuestData->pQuestDataEx;
	--pQuestDataEx->nMalusItemsInGame;

	if (!pQuestDataEx->nMalusItemsInGame && pQuestDataEx->bMalusInitialized && pQuestDataEx->nMalusObjectMode == OBJMODE_OPENED)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 3, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FC99AA0
void __fastcall ACT1Q3_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->pPlayer)
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
	}
	else
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, -1);
	}
}

//D2Game.0x6FC99AD0
void __fastcall ACT1Q3_Callback06(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestData->bNotIntro)
	{
		return;
	}

	D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)pQuestData->pQuestDataEx;
	--pQuestDataEx->nMalusItemsInGame;

	if (pQuestDataEx->nMalusItemsInGame || pQuestDataEx->nMalusObjectMode != OBJMODE_OPENED || !pQuestData->bNotIntro)
	{
		return;
	}

	pQuestDataEx->nMalusObjectMode = OBJMODE_NEUTRAL;

	if (!pQuestDataEx->bMalusInitialized)
	{
		return;
	}

	QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
	pQuestData->dwFlags = 0x00000001;
	QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q2_UnitIterate_StatusCyclerEx, 1);
	pQuestDataEx->unk0x08 = 0;

	if (pQuestDataEx->bMalusInitialized != 1)
	{
		return;
	}

	D2UnitStrc* pMalus = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nMalusGUID);
	if (pMalus && pMalus->dwClassId == OBJECT_MALUS)
	{
		UNITS_ChangeAnimMode(pMalus, OBJMODE_NEUTRAL);
	}
	else
	{
		pQuestDataEx->bMalusInitialized = 0;
	}
}

//D2Game.0x6FC99B80
bool __fastcall ACT1Q3_SeqCallback(D2QuestDataStrc* pQuestData)
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

//D2Game.0x6FC99C10
void __fastcall ACT1Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' mdh'))
	{
		D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)pQuestData->pQuestDataEx;
		pQuestDataEx->bStartingPlayerHasMalus = 1;
		++pQuestDataEx->nMalusItemsInGame;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_STARTED) == 1)
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q3, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
}
