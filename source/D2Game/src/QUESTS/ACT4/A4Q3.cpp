#include "QUESTS/ACT4/A4Q3.h"

#include <algorithm>

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD3B380
D2NPCMessageTableStrc gpAct4Q3NpcMessages[] =
{
	{
		{
			{ MONSTER_CAIN4, 166, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN4, 167, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN4, 168, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN4, 168, 0, 2 },
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


//D2Game.0x6FCAFF50
void __fastcall OBJECTS_InitFunction48_HellForge(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A4Q3_HELLFORGE);

	if (!pQuestData || !pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_SPECIAL1);
		return;
	}

	UNITS_ChangeAnimMode(pOp->pObject, ((D2Act4Quest3Strc*)pQuestData->pQuestDataEx)->nHellforgeObjectMode);

	if (pQuestData->fLastState != 13 && pQuestData->fLastState != 2 && pQuestData->fLastState != 3)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 2, 0, ACT4Q3_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCAFFC0
int32_t __fastcall ACT4Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A4Q3_HELLFORGE);
	}

	return 0;
}

//D2Game.0x6FCB0030
void __fastcall ACT4Q3_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_IV)
	{
		return;
	}

	QUESTS_DeleteItem(pGame, pUnit, ' hfh');
	QUESTS_DeleteItem(pGame, pUnit, ' ssm');
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING);
	QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, QUESTSTATEFLAG_A4Q3);

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A4Q3_HELLFORGE);
	if (pQuestData)
	{
		++((D2Act4Quest3Strc*)pQuestData->pQuestDataEx)->nPlayersInAct;
	}
}

//D2Game.0x6FCB00E0
int32_t __fastcall OBJECTS_OperateFunction49_HellForge(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A4Q3_HELLFORGE);
	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		return 0;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING) == 1)
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	D2Act4Quest3Strc* pQuestDataEx = (D2Act4Quest3Strc*)pQuestData->pQuestDataEx;

	D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pObject->dwClassId);

	if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
	{
		if (ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' ssm'))
		{
			UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
			EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, pOp->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);

			pQuestDataEx->nHellforgeObjectMode = OBJMODE_OPENED;
			pQuestData->dwFlags &= 0xFFFFFF00;

			QUESTS_UnitIterate(pQuestData, 3, 0, ACT4Q3_UnitIterate_StatusCyclerEx, 1);
			QUESTS_DeleteItem(pOp->pGame, pOp->pPlayer, ' ssm');
			return 0;
		}

		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);

		if (pQuestData->bNotIntro && pQuestData->fState == 0)
		{
			QUESTS_StateDebug(pQuestData, 1, "..\\D2Game/Quests/a4q3.cpp", 241);
		}
	}
	else if (pObject->dwAnimMode == OBJMODE_OPENED)
	{
		D2UnitStrc* pItem = ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' hfh');
		if (!pItem || !pOp->pPlayer->pInventory)
		{
			SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
			return 0;
		}

		pItem = INVENTORY_GetLeftHandWeapon(pOp->pPlayer->pInventory);
		if (!pItem || DATATBLS_GetItemsTxtRecord(pItem->dwClassId)->dwCode != ' hfh')
		{
			SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
			return 0;
		}

		const int32_t nHits = pQuestDataEx->nHits + 1;
		pQuestDataEx->nHits = nHits;
		if (nHits <= 2)
		{
			return 0;
		}

		UNITS_ChangeAnimMode(pObject, OBJMODE_SPECIAL1);
		pQuestDataEx->nHellforgeObjectMode = OBJMODE_SPECIAL1;
		QUESTS_DeleteItem(pOp->pGame, pOp->pPlayer, ' hfh');
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_PRIMARYGOALDONE);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING);
		QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, pQuestData->nQuest);

		pQuestDataEx->bSoulstoneSmashed = 1;
		pQuestDataEx->bRewardDropsPending = 1;
		pQuestDataEx->nGemDropTier = 4;
		pQuestDataEx->nPlayersInAct = 1;

		const int16_t nPartyId = SUNIT_GetPartyId(pOp->pPlayer);
		if (nPartyId != -1)
		{
			PARTY_IteratePartyMembers(pOp->pGame, nPartyId, ACT4Q3_UnitIterate_SetPrimaryGoalDone, 0);
		}

		QUESTS_StateDebug(pQuestData, 4, "..\\D2Game/Quests/a4q3.cpp", 293);
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 13, 0, ACT4Q3_UnitIterate_StatusCyclerEx, 1);

		EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_QUESTFN, pOp->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[3] >> 8), 0, 0);
		SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pPlayer, ACT4Q3_UnitIterate_SetCompletionFlag);
		QUESTS_TriggerFX(pOp->pGame, 14);
	}

	return 1;
}

//D2Game.0x6FCB03A0
int32_t __fastcall ACT4Q3_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_COMPLETEDNOW);
	QUESTS_SendLogUpdateEx(pUnit, QUEST_A4Q3_HELLFORGE, 0);
	return 0;
}

//D2Game.0x6FCB0400
bool __fastcall ACT4Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	return nNpcId == MONSTER_CAIN4 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDGRANTED)
		&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING)
		&& pQuest->bNotIntro && pQuest->fState == 1 && !((D2Act4Quest3Strc*)pQuest->pQuestDataEx)->bSoulstoneAcquired;
}

//D2Game.0x6FCB0460
void __fastcall ACT4Q3_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pQuestDataEx = 0;
	pQuestData->fLastState = 0;
	pQuestData->fState = 0;
	pQuestData->pfStatusFilter = 0;
	pQuestData->pfActiveFilter = ACT4Q3_ActiveFilterCallback;
	pQuestData->pNPCMessages = gpAct4Q3NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->pfSeqFilter = ACT4Q3_SeqCallback;
	pQuestData->nSeqId = QUEST_A4Q2_DIABLO;
	pQuestData->nQuest = QUESTSTATEFLAG_A4Q3;
	pQuestData->nInitNo = 4;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT4Q3_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT4Q3_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT4Q3_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT4Q3_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT4Q3_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT4Q3_Callback13_PlayerStartedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERJOINEDGAME] = ACT4Q3_Callback14_PlayerJoinedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM] = ACT4Q3_Callback09_PlayerDroppedWithQuestItem;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT4Q3_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_ITEMPICKEDUP] = ACT4Q3_Callback04_ItemPickedUp;

	D2Act4Quest3Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act4Quest3Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	memset(pQuestDataEx, 0x00, sizeof(D2Act4Quest3Strc));
}

//D2Game.0x6FCB0550
void __fastcall ACT4Q3_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDGRANTED)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING)
		|| !pQuestData->bNotIntro || pQuestData->fLastState == 13)
	{
		return;
	}

	if (!pQuestData->fState)
	{
		QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
	}
}

//D2Game.0x6FCB05C0
bool __fastcall ACT4Q3_SeqCallback(D2QuestDataStrc* pQuestData)
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

	if (IsBadCodePtr((FARPROC)pQuest->pfSeqFilter))
	{
		FOG_DisplayAssert("pQuest->pSequence", __FILE__, __LINE__);
		exit(-1);
	}
	return pQuestData->pfSeqFilter(pQuest);
}

//D2Game.0x6FCB0650
void __fastcall ACT4Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nOldLevel != LEVEL_THEPANDEMONIUMFORTRESS)
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
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT4Q3_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FCB06D0
int32_t __fastcall ACT4Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A4Q3_HELLFORGE);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState != 2)
	{
		if (pQuestData->fState == 3)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_LEAVETOWN);
		}
		return 0;
	}

	if (((D2Act4Quest3Strc*)pQuestData->pQuestDataEx)->bSoulstoneAcquired == 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_CUSTOM1);
	}
	else
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_STARTED);
	}

	return 0;
}

//D2Game.0x6FCB0760
void __fastcall ACT4Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (pQuestArg->nNPCNo != MONSTER_CAIN4)
	{
		return;
	}

	D2Act4Quest3Strc* pQuestDataEx = (D2Act4Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->nMessageIndex == 678)
	{
		pQuestDataEx->bCainActivated = 1;
		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
	else if (pQuestArg->nMessageIndex == 679)
	{
		if (QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, ' ssm', 0, ITEMQUAL_NORMAL, 1))
		{
			pQuestDataEx->bCainActivated = 1;
			if (pQuestData->fState < 2)
			{
				QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			}
			pQuestDataEx->bSoulstoneAcquired = 1;
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		}
	}
	else if (pQuestArg->nMessageIndex == 680)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE) && pQuestData->fState != 5)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 13, 0, ACT4Q3_UnitIterate_StatusCyclerEx, 0);
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

			int32_t nUnitId = -1;
			if (pQuestArg->pPlayer)
			{
				nUnitId = pQuestArg->pPlayer->dwUnitId;
			}

			QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, nUnitId);
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		}
	}
}

//D2Game.0x6FCB0920
void __fastcall ACT4Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
	}
	else if (QUESTS_CheckPlayerGUID(pQuestData, pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1) == 1)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
	}
	else if (pQuestData->fState && QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) != 1
		&& (pQuestData->fState < 4 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE))
		&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_REWARDPENDING) && pQuestData->bNotIntro)
	{
		if (pQuestData->fState == 1)
		{
			if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' ssm'))
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 0);
			}
			else if (!((D2Act4Quest3Strc*)pQuestData->pQuestDataEx)->bSoulstoneAcquired && pQuestData->fLastState < 3)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 1);
			}
		}
		else
		{
			if (!ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' ssm') && pQuestData->fLastState < 3 && !((D2Act4Quest3Strc*)pQuestData->pQuestDataEx)->bSoulstoneAcquired)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 1);
			}
		}
	}
}

//D2Game.0x6FCB0A80
void __fastcall ACT4Q3_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_CAIN4)
	{
		return;
	}

	D2Act4Quest3Strc* pQuestDataEx = (D2Act4Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bCainActivated != 1)
	{
		return;
	}

	int32_t nState = 1;
	if (pQuestDataEx->bSoulstoneAcquired)
	{
		nState = 4;
	}
	QUESTS_UnitIterate(pQuestData, nState, 0, ACT4Q3_UnitIterate_StatusCyclerEx, 1);
	pQuestData->dwFlags &= 0xFFFFFF00;
	pQuestDataEx->bCainActivated = 0;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT4Q3_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FCB0AE0
void __fastcall ACT4Q3_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act4Quest3Strc* pQuestDataEx = (D2Act4Quest3Strc*)pQuestData->pQuestDataEx;

	if (ITEMS_GetBaseCode(pQuestArg->pTarget) == ' hfh')
	{
		--pQuestDataEx->nHellforgeHammersInGame;
	}
}

//D2Game.0x6FCB0B00
void __fastcall ACT4Q3_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act4Quest3Strc* pQuestDataEx = (D2Act4Quest3Strc*)pQuestData->pQuestDataEx;

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' hfh'))
	{
		++pQuestDataEx->nHellforgeHammersInGame;
	}
}

//D2Game.0x6FCB0B20
void __fastcall ACT4Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act4Quest3Strc* pQuestDataEx = (D2Act4Quest3Strc*)pQuestData->pQuestDataEx;
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' hfh'))
	{
		++pQuestDataEx->nHellforgeHammersInGame;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		return;
	}

	if (!ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' ssm'))
	{
		pQuestData->fState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_CUSTOM1) == 1)
	{
		pQuestDataEx->bSoulstoneAcquired = 1;
		pQuestData->fLastState = 4;
		pQuestData->fState = 2;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fLastState = 1;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q3, QFLAG_STARTED) == 1)
	{
		pQuestData->fLastState = 1;
		pQuestData->fState = 2;
	}
}

//D2Game.0x6FCB0C00
void __fastcall ACT4Q3_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act4Quest3Strc* pQuestDataEx = (D2Act4Quest3Strc*)pQuestData->pQuestDataEx;
	if (!pQuestData->bNotIntro)
	{
		return;
	}

	pQuestArg->pTarget->dwDropItemCode = ' hfh';
	int32_t nItemLevel = 0;
	if (D2GAME_DropItemAtUnit_6FC4FEC0(pQuestArg->pGame, pQuestArg->pTarget, ITEMQUAL_UNIQUE, &nItemLevel, 0, -1, 0))
	{
		++pQuestDataEx->nHellforgeHammersInGame;
	}
}

//D2Game.0x6FCB0C40
void __fastcall ACT4Q3_CreateReward(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit)
{
	constexpr int32_t runeRewardCodes[3][11] =
	{
		{ ' 10r', ' 20r', ' 30r', ' 40r', ' 50r', ' 60r', ' 70r', ' 80r', ' 90r', ' 01r', ' 11r' },
		{ ' 21r', ' 31r', ' 41r', ' 51r', ' 61r', ' 71r', ' 81r', ' 91r', ' 02r', ' 12r', ' 22r' },
		{ ' 51r', ' 61r', ' 71r', ' 81r', ' 91r', ' 02r', ' 12r', ' 22r', ' 32r', ' 42r', ' 52r' },
	};

	constexpr int32_t gemRewardCodes[3][7] =
	{
		{ ' vpg', ' rpg', ' bpg', ' ypg', ' gpg', ' wpg', ' zks' },
		{ ' vzg', ' rlg', ' blg', ' ylg', ' glg', ' wlg', ' lks' },
		{ ' vsg', ' rsg', ' bsg', ' ysg', ' gsg', ' wsg', ' uks' },
	};

	D2Act4Quest3Strc* pQuestDataEx = (D2Act4Quest3Strc*)pQuestData->pQuestDataEx;

	if (pQuestDataEx->bSoulstoneSmashed)
	{
		UNITS_ChangeAnimMode(pUnit, OBJMODE_SPECIAL2);
	}

	if (!pQuestDataEx->bRewardDropsPending || pQuestDataEx->nPlayersInAct <= 0)
	{
		return;
	}

	int32_t nGemsDropped = 0;
	int32_t nCounter = 0;
	int32_t nId = 0;
	while (1)
	{
		D2SeedStrc* pSeed = QUESTS_GetGlobalSeed(pQuestData->pGame);

		switch (pQuestDataEx->nGemDropTier)
		{
		case 4:
			nId = 0;
			break;

		case 2:
		case 3:
			nId = 1;
			break;

		case 1:
			nId = 2;
			break;

		default:
			return;
		}

		pUnit->dwDropItemCode = gemRewardCodes[nId][ITEMS_RollRandomNumber(pSeed) % std::size(gemRewardCodes[0])];
		int32_t nItemLevel = 50;
		if (D2GAME_DropItemAtUnit_6FC4FEC0(pQuestData->pGame, pUnit, ITEMQUAL_NORMAL, &nItemLevel, nullptr, -1, 0))
		{
			++nGemsDropped;
		}

		++nCounter;

		if (nCounter >= pQuestDataEx->nPlayersInAct)
		{
			if (nGemsDropped)
			{
				--pQuestDataEx->nGemDropTier;
				if (pQuestDataEx->nGemDropTier > 0)
				{
					EVENT_SetEvent(pQuestData->pGame, pUnit, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 20, 0, 0);
				}
				else
				{
					pQuestDataEx->bRewardDropsPending = 0;
					if (pQuestData->pGame->bExpansion)
					{
						pUnit->dwDropItemCode = runeRewardCodes[pQuestData->pGame->nDifficulty][ITEMS_RollRandomNumber(pSeed) % std::size(runeRewardCodes[0])];
						D2GAME_DropItemAtUnit_6FC4FEC0(pQuestData->pGame, pUnit, ITEMQUAL_NORMAL, &nItemLevel, nullptr, -1, 0);
					}
				}
			}

			break;
		}
	}
}

//
void __fastcall ACT4Q3_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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
