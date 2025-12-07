#include "QUESTS/ACT3/A3Q5.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2QuestRecord.h>

#include "AI/AiTactics.h"
#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterSpawn.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/PlrTrade.h"
#include "QUESTS/Quests.h"
#include "QUESTS/QuestsFX.h"
#include "QUESTS/ACT3/A3Q2.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD39A00
D2NPCMessageTableStrc gpAct3Q5NpcMessages[] =
{
	{
		{
			{ MONSTER_ORMUS, 594, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_ALKOR, 595, 0, 2 },
			{ MONSTER_ORMUS, 597, 0, 2 },
			{ MONSTER_MESHIF2, 599, 0, 2 },
			{ MONSTER_ASHEARA, 601, 0, 2 },
			{ MONSTER_HRATLI, 603, 0, 2 },
			{ MONSTER_CAIN3, 605, 0, 2 },
			{ MONSTER_NATALYA, 607, 0, 2 },
		},
		7
	},
	{
		{
			{ MONSTER_ALKOR, 596, 0, 2 },
			{ MONSTER_ORMUS, 598, 0, 2 },
			{ MONSTER_MESHIF2, 600, 0, 2 },
			{ MONSTER_ASHEARA, 602, 0, 2 },
			{ MONSTER_HRATLI, 604, 0, 2 },
			{ MONSTER_CAIN3, 606, 0, 2 },
			{ MONSTER_NATALYA, 608, 0, 2 },
		},
		7
	},
	{
		{
			{ MONSTER_ALKOR, 609, 0, 2 },
			{ MONSTER_ORMUS, 611, 0, 2 },
			{ MONSTER_MESHIF2, 612, 0, 2 },
			{ MONSTER_ASHEARA, 614, 0, 2 },
			{ MONSTER_HRATLI, 616, 0, 2 },
			{ MONSTER_CAIN3, 618, 0, 2 },
			{ MONSTER_NATALYA, 619, 0, 2 },
		},
		7
	},
	{
		{
			{ MONSTER_ALKOR, 610, 0, 2 },
			{ MONSTER_MESHIF2, 613, 0, 2 },
			{ MONSTER_ASHEARA, 615, 0, 2 },
			{ MONSTER_HRATLI, 617, 0, 2 },
			{ MONSTER_NATALYA, 620, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_ALKOR, 621, 0, 2 },
			{ MONSTER_ORMUS, 622, 0, 2 },
			{ MONSTER_MESHIF2, 623, 0, 2 },
			{ MONSTER_ASHEARA, 624, 0, 2 },
			{ MONSTER_HRATLI, 625, 0, 2 },
			{ MONSTER_CAIN3, 626, 0, 1 },
			{ MONSTER_NATALYA, 627, 0, 2 },
		},
		7
	},
	{
		{
			{ MONSTER_ALKOR, 621, 0, 2 },
			{ MONSTER_ORMUS, 622, 0, 2 },
			{ MONSTER_MESHIF2, 623, 0, 2 },
			{ MONSTER_ASHEARA, 624, 0, 2 },
			{ MONSTER_HRATLI, 625, 0, 2 },
			{ MONSTER_CAIN3, 626, 0, 2 },
			{ MONSTER_NATALYA, 627, 0, 2 },
		},
		7
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FCAAFA0
bool __fastcall ACT3Q5_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED))
	{
		return false;
	}

	if (nNpcId == MONSTER_ORMUS)
	{
		if (pQuest->fState == 1)
		{
			return true;
		}
	}
	else if (nNpcId == MONSTER_CAIN3)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA) && !((D2Act3Quest5Strc*)pQuest->pQuestDataEx)->bOrbSmashed)
		{
			return true;
		}
	}

	return false;
}

//D2Game.0x6FCAB000
void __fastcall ACT3Q5_SpawnCouncil(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q5_TRAVINCAL);
	if (!pQuestData || !pQuestData->bNotIntro)
	{
		return;
	}

	D2Act3Quest5Strc* pQuestDataEx = (D2Act3Quest5Strc*)pQuestData->pQuestDataEx;

	pQuestDataEx->bMonstersSpawned = 1;

	if (pQuestDataEx->nSpawnCount >= 6)
	{
		return;
	}

	int32_t nUnitId = -1;
	if (pUnit)
	{
		nUnitId = pUnit->dwUnitId;
	}

	if (pQuestDataEx->nSpawnCount > 0)
	{
		int32_t nCounter = 0;
		while (pQuestDataEx->nMonsterGUIDs[nCounter] != nUnitId)
		{
			++nCounter;
			if (nCounter >= pQuestDataEx->nSpawnCount)
			{
				break;
			}
		}
		if (nCounter < pQuestDataEx->nSpawnCount)
		{
			return;
		}
	}

	pQuestDataEx->nMonsterGUIDs[pQuestDataEx->nSpawnCount] = nUnitId;
	++pQuestDataEx->nSpawnCount;
	pQuestDataEx->nMonstersLeftToKill = pQuestDataEx->nSpawnCount;

	if (pQuestData->fState <= 1)
	{
		if (pQuestData->fLastState != 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 1, 0, ACT3Q5_UnitIterate_StatusCyclerEx, 1);
		}
	}
	else
	{
		if (pQuestData->fLastState != 3)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 3, 0, ACT3Q5_UnitIterate_StatusCyclerEx, 1);
		}
	}

	if (QUESTS_GetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A3Q1, QFLAG_PRIMARYGOALDONE) == 1)
	{
		QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
	}
	else
	{
		QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
	}

	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q5_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FCAB0F0
int32_t __fastcall ACT3Q5_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q5_TRAVINCAL);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState >= 2 && pQuestData->fState <= 3)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_STARTED);
	}

	if (((D2Act3Quest5Strc*)pQuestData->pQuestDataEx)->bMonstersSpawned)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_LEAVETOWN);
	}

	return 0;
}

//D2Game.0x6FCAB160
int32_t __fastcall ACT3Q5_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A3Q5_TRAVINCAL);
	}

	return 0;
}

//D2Game.0x6FCAB1D0
void __fastcall ACT3Q5_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, 0) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_III)
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED);
	}
	else
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA);
	}
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_PRIMARYGOALDONE);
}

//D2Game.0x6FCAB250
void __fastcall ACT3Q5_UnitIterate_DeleteKhalimItems(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED) == 1)
	{
		QUESTS_DeleteItem(pGame, pUnit, ' yeq');
		QUESTS_DeleteItem(pGame, pUnit, ' rhq');
		QUESTS_DeleteItem(pGame, pUnit, ' rbq');
		QUESTS_DeleteItem(pGame, pUnit, ' 1fq');
		QUESTS_DeleteItem(pGame, pUnit, ' 2fq');
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (DRLG_GetActNoFromLevelId(nLevelId) != ACT_III)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_PRIMARYGOALDONE);

	if (!D2GAME_PLRTRADE_IsInteractingWithPlayer(pGame, pUnit))
	{
		QUESTS_DeleteItem(pGame, pUnit, ' yeq');
		QUESTS_DeleteItem(pGame, pUnit, ' rhq');
		QUESTS_DeleteItem(pGame, pUnit, ' rbq');
		QUESTS_DeleteItem(pGame, pUnit, ' 1fq');
		QUESTS_DeleteItem(pGame, pUnit, ' 2fq');
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED);
	}
}

//D2Game.0x6FCAB380
int32_t __fastcall OBJECTS_OperateFunction53_CompellingOrb(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
	{
		return 0;
	}

	D2UnitStrc* pItem = INVENTORY_GetLeftHandWeapon(pOp->pPlayer->pInventory);
	if (!pItem || ITEMS_GetBaseCode(pItem) != ' 2fq')
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q5_TRAVINCAL);
	D2Act3Quest5Strc* pQuestDataEx = (D2Act3Quest5Strc*)pQuestData->pQuestDataEx;

	const int32_t nHits = pQuestDataEx->nHits + 1;
	pQuestDataEx->nHits = nHits;
	if (nHits < 2)
	{
		return 0;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_PRIMARYGOALDONE);

	QUESTS_DeleteItem(pOp->pGame, pOp->pPlayer, ' 2fq');

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED);
	}

	D2UnitStrc* pCompellingOrb = SUNIT_GetServerUnit(pOp->pGame, UNIT_MONSTER, pQuestDataEx->nCompellingOrbGUID);
	if (pCompellingOrb)
	{
		AITACTICS_ChangeModeAndTargetCoordinates(pOp->pGame, pCompellingOrb, 0, 0, 0);
		QUESTSFX_SpawnCompellingOrbMissile(pOp->pGame, pCompellingOrb);
	}

	UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
	const int32_t nObjectId = pObject ? pObject->dwClassId : -1;
	D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nObjectId);
	EVENT_SetEvent(pOp->pGame, pObject, EVENTTYPE_ENDANIM, pOp->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);
	pQuestDataEx->bOrbSmashed = 1;

	QUESTS_TriggerFX(pQuestData->pGame, 10);
	if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
	{
		FOG_DisplayAssert("pQuestInfo->pSequence", "..\\D2Game/Quests/a3q5.cpp", 851);
		exit(-1);
	}
	pQuestData->pfSeqFilter(pQuestData);

	const int16_t nPartyId = SUNIT_GetPartyId(pOp->pPlayer);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pOp->pGame, nPartyId, ACT3Q5_UnitIterate_DeleteKhalimItems, 0);
	}

	return 0;
}

//D2Game.0x6FCAB580
void __fastcall OBJECTS_InitFunction53_StairsR(D2ObjInitFnStrc* pOp)
{
	if (((D2Act3Quest5Strc*)QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q5_TRAVINCAL)->pQuestDataEx)->bOrbSmashed)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
}

//D2Game.0x6FCAB5B0
void __fastcall OBJECTS_InitFunction60_CompellingOrb(D2ObjInitFnStrc* pOp)
{
	D2Act3Quest5Strc* pQuestDataEx = (D2Act3Quest5Strc*)QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q5_TRAVINCAL)->pQuestDataEx;
	if (pQuestDataEx->bOrbSmashed)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}

	if (pQuestDataEx->bCompellingOrbSpawned)
	{
		return;
	}

	D2UnitStrc* pCompellingOrb = sub_6FC6A090(pOp->pGame, pOp->pRoom, pOp->nX, pOp->nY, MONSTER_COMPELLINGORB, 1, 0);
	if (!pCompellingOrb)
	{
		return;
	}

	pCompellingOrb->dwFlags |= UNITFLAG_NOTC;
	pQuestDataEx->nCompellingOrbGUID = pCompellingOrb->dwUnitId;
	pQuestDataEx->bCompellingOrbSpawned = 1;
}

//D2Game.0x6FCAB620
void __fastcall ACT3Q5_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->pfStatusFilter = 0;
	pQuestData->pNPCMessages = gpAct3Q5NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT3Q5_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT3Q5_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT3Q5_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT3Q5_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT3Q5_Callback13_PlayerStartedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT3Q5_Callback10_PlayerLeavesGame;
	pQuestData->pfSeqFilter = ACT3Q5_SeqCallback;
	pQuestData->nSeqId = 20;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A3Q5;
	pQuestData->pfActiveFilter = ACT3Q5_ActiveFilterCallback;
	pQuestData->nInitNo = 6;

	D2Act3Quest5Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act3Quest5Strc);
	memset(pQuestDataEx, 0x00, sizeof(D2Act3Quest5Strc));
	pQuestData->pQuestDataEx = pQuestDataEx;
}

//D2Game.0x6FCAB6F0
void __fastcall ACT3Q5_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4, 5, 6
	};

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, (D2QuestStateFlagIds)pQuestData->nQuestFilter, QFLAG_ENTERAREA))
	{
		if (!((D2Act3Quest5Strc*)pQuestData->pQuestDataEx)->bOrbSmashed)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 5);
		}
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED) != 1 || QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)))
	{
		if (pQuestData->fState < 6u)
		{
			if (pQuestData->bNotIntro)
			{
				const int32_t nIndex = nIndices[pQuestData->fState];
				if (nIndex != -1 && nIndex < 8)
				{
					QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
				}
			}
		}
		else
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_PRIMARYGOALDONE) == 1)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 6);
			}
		}
	}
}

//D2Game.0x6FCAB800
void __fastcall ACT3Q5_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel == LEVEL_KURASTCAUSEWAY && pQuestData->bNotIntro && !pQuestData->fState)
	{
		pQuestData->fState = 1;
		pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT3Q5_Callback02_NpcDeactivate;
	}

	if (pQuestArg->nOldLevel != LEVEL_KURASTDOCKTOWN)
	{
		return;
	}

	QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);

	if (pQuestData->fState <= 1 || pQuestData->fState > 3)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_ENTERAREA) == 1)
	{
		return;
	}

	if (pQuestData->fLastState < 3)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 2, 0, ACT3Q5_UnitIterate_StatusCyclerEx, 0);
	}

	if (QUESTS_GetGlobalState(pQuestArg->pGame, QUESTSTATEFLAG_A3Q1, QFLAG_PRIMARYGOALDONE) == 1)
	{
		QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
	}
	else
	{
		QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
	}
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q5_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FCAB900
void __fastcall ACT3Q5_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_ORMUS)
	{
		return;
	}

	D2Act3Quest5Strc* pQuestDataEx = (D2Act3Quest5Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bOrmusActivated != 1)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 2, 0, ACT3Q5_UnitIterate_StatusCyclerEx, 1);
	pQuestDataEx->bOrmusActivated = 0;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q5_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FCAB960
void __fastcall ACT3Q5_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED) == 1)
	{
		return;
	}

	if (pQuestArg->nNPCNo == MONSTER_ORMUS)
	{
		if (pQuestArg->nMessageIndex == 594)
		{
			if (QUESTS_GetGlobalState(pQuestArg->pGame, QUESTSTATEFLAG_A3Q1, QFLAG_PRIMARYGOALDONE) == 1)
			{
				QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			}
			else
			{
				QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
			}

			((D2Act3Quest5Strc*)pQuestData->pQuestDataEx)->bOrmusActivated = 1;
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		}
	}
	else if (pQuestArg->nNPCNo == MONSTER_CAIN3)
	{
		if (pQuestArg->nMessageIndex == 626 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA))
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED))
			{
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' yeq');
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' rhq');
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' rbq');
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 1fq');
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 2fq');
			}

			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_PRIMARYGOALDONE) && pQuestData->fState != 7)
			{
				QUESTS_SetGlobalState(pQuestArg->pGame, QUESTSTATEFLAG_A3Q5, QFLAG_PRIMARYGOALDONE);
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 13, 0, ACT3Q5_UnitIterate_StatusCyclerEx, 0);
				QUESTS_StateDebug(pQuestData, 7, __FILE__, __LINE__);
			}

			if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
			{
				FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
				exit(-1);
			}
			pQuestData->pfSeqFilter(pQuestData);

			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED);
			QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA);

			QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		}
	}
}

//D2Game.0x6FCABB50
bool __fastcall ACT3Q5_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (pQuestData->fState != 7 && pQuestData->bNotIntro)
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

//D2Game.0x6FCABBC0
void __fastcall ACT3Q5_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	D2Act3Quest5Strc* pQuestDataEx = (D2Act3Quest5Strc*)pQuestData->pQuestDataEx;

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED))
	{
		pQuestDataEx->bOrbSmashed = true;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA) == 1)
	{
		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A3Q5, QFLAG_PRIMARYGOALDONE);
		pQuestData->bNotIntro = 0;
	}
	else
	{
		pQuestDataEx->bQuest1RewardGranted = QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q1, QFLAG_REWARDGRANTED);
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_STARTED))
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_LEAVETOWN) != 0)
			{
				pQuestData->fLastState = 3;

				if (pQuestDataEx->bQuest1RewardGranted)
				{
					pQuestData->fState = 4;
				}
				else
				{
					pQuestData->fState = 5;
				}
			}
			else
			{
				pQuestData->fLastState = 2;

				if (pQuestDataEx->bQuest1RewardGranted)
				{
					pQuestData->fState = 2;
				}
				else
				{
					pQuestData->fState = 3;
				}
			}
		}
		else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_LEAVETOWN))
		{
			pQuestData->fLastState = 3;
			if (!pQuestDataEx->bQuest1RewardGranted)
			{
				pQuestData->fState = 5;
			}
			else
			{
				pQuestData->fState = 4;
			}
		}
	}
}

//D2Game.0x6FCABCB0
void __fastcall ACT3Q5_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_DebugOutput(pQuestData->pGame, "killed boss for quest", __FILE__, __LINE__);

	D2Act3Quest5Strc* pQuestDataEx = (D2Act3Quest5Strc*)pQuestData->pQuestDataEx;

	if (pQuestArg->pTarget && pQuestArg->pTarget->dwClassId == MONSTER_COMPELLINGORB)
	{
		pQuestArg->pTarget->dwFlags |= UNITFLAG_NOTC;
		return;
	}

	if (!pQuestDataEx->bFlailDropped || !pQuestDataEx->bCubeDropped)
	{
		pQuestDataEx->nFlailsMissing = 0;
		pQuestDataEx->nHoradricCubesMissing = 0;
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q5_UnitIterate_DetermineFlailAndCubeDropCounts);
	}

	if (pQuestDataEx->bFlailDropped)
	{
		if (!pQuestDataEx->bCubeDropped)
		{
			pQuestDataEx->bCubeDropped = 1;
			pQuestArg->pTarget->dwDropItemCode = ' xob';
			for (int32_t i = 0; i < pQuestDataEx->nHoradricCubesMissing; ++i)
			{
				int32_t nItemLevel = 0;
				D2GAME_DropItemAtUnit_6FC4FEC0(pQuestData->pGame, pQuestArg->pTarget, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0);
			}
		}
	}
	else
	{
		int32_t nFlailsDropped = 0;
		pQuestArg->pTarget->dwDropItemCode = ' 1fq';
		for (int32_t i = 0; i < pQuestDataEx->nFlailsMissing; ++i)
		{
			int32_t nItemLevel = 0;
			if (D2GAME_DropItemAtUnit_6FC4FEC0(pQuestData->pGame, pQuestArg->pTarget, ITEMQUAL_UNIQUE, &nItemLevel, 0, -1, 0))
			{
				++nFlailsDropped;
			}
		}

		if (nFlailsDropped)
		{
			pQuestDataEx->bFlailDropped = 1;

			D2QuestDataStrc* pQuestData16 = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A3Q2_KHALIMFLAIL);
			if (pQuestData16)
			{
				D2Act3Quest2Strc* pQuestDataEx16 = (D2Act3Quest2Strc*)pQuestData16->pQuestDataEx;
				pQuestDataEx16->bFlailDropped = 1;
				pQuestDataEx16->nFlailsDropped += nFlailsDropped;
			}
		}
	}

	if (!pQuestData->bNotIntro || pQuestDataEx->nMonstersLeftToKill <= 0)
	{
		return;
	}

	--pQuestDataEx->nMonstersLeftToKill;

	if (pQuestDataEx->nMonstersLeftToKill)
	{
		return;
	}

	int32_t nUnitId = -1;
	if (pQuestArg->pTarget)
	{
		nUnitId = pQuestArg->pTarget->dwUnitId;
	}
	pQuestDataEx->nLastKilledMonsterGUID = nUnitId;

	pQuestData->dwFlags &= 0xFFFFFF00;
	if (pQuestDataEx->bOrbSmashed)
	{
		QUESTS_StateDebug(pQuestData, 7, __FILE__, __LINE__);
		QUESTS_UnitIterate(pQuestData, 13, 0, ACT3Q5_UnitIterate_StatusCyclerEx, 1);
	}
	else
	{
		QUESTS_StateDebug(pQuestData, 6, __FILE__, __LINE__);
		QUESTS_UnitIterate(pQuestData, 4, 0, ACT3Q5_UnitIterate_StatusCyclerEx, 1);
	}
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q5_UnitIterate_UpdateQuestStateAfterMonsterKill);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT3Q5_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT3Q5_UnitIterate_SetCompletionFlag);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT3Q5_UnitIterate_AttachCompletionSound);
	QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A3Q5, QFLAG_PRIMARYGOALDONE);
}

//D2Game.0x6FCABED0
int32_t __fastcall ACT3Q5_UnitIterate_UpdateQuestStateAfterMonsterKill(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);

	if (!pRoom || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA))
	{
		return 0;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (DRLG_GetActNoFromLevelId(nLevelId) != ACT_III)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q5_TRAVINCAL);
	D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, ((D2Act3Quest5Strc*)pQuestData->pQuestDataEx)->nLastKilledMonsterGUID);
	if (!pTarget)
	{
		return 0;
	}

	D2ActiveRoomStrc* pTargetRoom = UNITS_GetRoom(pTarget);
	if (!pTargetRoom)
	{
		return 0;
	}

	if (pRoom != pTargetRoom)
	{
		D2ActiveRoomStrc** ppRoomList = nullptr;
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

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED);
	}
	else
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA);
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_PRIMARYGOALDONE);

	return 0;
}

//D2Game.0x6FCAC020
int32_t __fastcall ACT3Q5_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A3Q5, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT3Q5_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCAC070
int32_t __fastcall ACT3Q5_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_ENTERAREA) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q5, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A3Q5_TRAVINCAL, 0);
	}

	return 0;
}

//D2Game.0x6FCAC0D0
int32_t __fastcall ACT3Q5_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A3Q5, QFLAG_PRIMARYGOALDONE))
	{
		SUNIT_AttachSound(pUnit, 64, pUnit);
	}

	return 0;
}

//D2Game.0x6FCAC110
int32_t __fastcall ACT3Q5_UnitIterate_DetermineFlailAndCubeDropCounts(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return 0;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_III)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q5_TRAVINCAL);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act3Quest5Strc* pQuestDataEx = (D2Act3Quest5Strc*)pQuestData->pQuestDataEx;

	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED)
		&& !ITEMS_FindQuestItem(pGame, pUnit, ' 1fq') && !ITEMS_FindQuestItem(pGame, pUnit, ' 2fq'))
	{
		++pQuestDataEx->nFlailsMissing;
	}

	if (!ITEMS_FindQuestItem(pGame, pUnit, ' xob'))
	{
		++pQuestDataEx->nHoradricCubesMissing;
	}

	return 0;
}

//D2Game.0x6FCAC1B0
int32_t __fastcall ACT3Q5_IsDuranceOfHateClosed(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q5_TRAVINCAL);
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);

	if (pRoom && DUNGEON_GetLevelIdFromRoom(pRoom) == LEVEL_DURANCEOFHATELEV2 || !pQuestData)
	{
		return 0;
	}

	return ((D2Act3Quest5Strc*)pQuestData->pQuestDataEx)->bOrbSmashed == 0;
}

//
void __fastcall ACT3Q5_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_RemovePlayerGUID(pQuestData, pQuestArg);
}
