#include "QUESTS/ACT5/A5Q5.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Units/UnitRoom.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2QuestRecord.h>
#include <D2States.h>
#include <D2StatList.h>

#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Game.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/MonsterMode.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerStats.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT2/A2Q6.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitInactive.h"


//D2Game.0x6FD3DE60
D2NPCMessageTableStrc gpAct5Q5NpcMessages[] =
{
	{
		{
			{ MONSTER_QUAL_KEHK, 20153, 0, 0, },
		},
		1
	},
	{
		{
			{ MONSTER_LARZUK, 20157, 0, 2, },
			{ MONSTER_CAIN6, 20155, 0, 2, },
			{ MONSTER_MALAH, 20158, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20154, 0, 2, },
			{ MONSTER_NIHLATHAK, 20156, 0, 2, },
		},
		5
	},
	{
		{
			{ MONSTER_LARZUK, 20162, 0, 2, },
			{ MONSTER_CAIN6, 20160, 0, 2, },
			{ MONSTER_NIHLATHAK, 20161, 0, 2, },
			{ MONSTER_MALAH, 20163, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20159, 0, 2, },
		},
		5
	},
	{
		{
			{ MONSTER_LARZUK, 20167, 0, 2, },
			{ MONSTER_CAIN6, 20165, 0, 2, },
			{ MONSTER_NIHLATHAK, 20166, 0, 2, },
			{ MONSTER_MALAH, 20168, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20164, 0, 2, },
		},
		5
	},
	{
		{
			{ MONSTER_ANCIENTSTATUE1, 20002, 0, 0, },
			{ MONSTER_ANCIENTSTATUE2, 20002, 0, 0, },
			{ MONSTER_ANCIENTSTATUE3, 20002, 0, 0, },
		},
		3
	},
	{
		{
			{ MONSTER_LARZUK, 20167, 0, 0, },
			{ MONSTER_CAIN6, 20165, 0, 0, },
			{ MONSTER_DREHYA, 20166, 0, 0, },
			{ MONSTER_MALAH, 20168, 0, 0, },
			{ MONSTER_QUAL_KEHK, 20164, 0, 0, },
		},
		5
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


//D2Game.0x6FCB6910
void __fastcall ACT5Q5_SpawnAncientMonsters(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit)
{
	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;
	if ((pUnit && pUnit->dwAnimMode == OBJMODE_SPECIAL2) || pQuestDataEx->nAncientsSpawned >= 3)
	{
		return;
	}

	int32_t nObjectId = -1;
	if (pUnit)
	{
		nObjectId = pUnit->dwClassId;
	}

	int32_t nIndex = 0;
	int32_t nMonsterId = 0;
	switch (nObjectId)
	{
	case OBJECT_ANCIENTSTATUE3:
		nIndex = 1;
		nMonsterId = sgptDataTables->nSuperUniqueIds[SUPERUNIQUE_ANCIENT_BARBARIAN_3];
		break;

	case OBJECT_ANCIENTSTATUE1:
		nIndex = 0;
		nMonsterId = sgptDataTables->nSuperUniqueIds[SUPERUNIQUE_ANCIENT_BARBARIAN_1];
		break;

	case OBJECT_ANCIENTSTATUE2:
		nIndex = 2;
		nMonsterId = sgptDataTables->nSuperUniqueIds[SUPERUNIQUE_ANCIENT_BARBARIAN_2];
		break;

	default:
		return;
	}

	if (pQuestDataEx->nActivePortalCount > 0)
	{
		ACT5Q5_DeactivateAncientMonsters(pQuestData->pGame, pQuestData, pQuestDataEx);
		return;
	}

	D2CoordStrc pCoord = {};
	UNITS_GetCoords(pUnit, &pCoord);
	D2UnitStrc* pAncient = QUESTS_SpawnMonster(pQuestData->pGame, pUnit, &pCoord, 2, nMonsterId);
	if (!pAncient)
	{
		EVENT_SetEvent(pQuestData->pGame, pUnit, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 10, 0, 0);
		return;
	}

	if (!pUnit || pUnit->dwAnimMode != OBJMODE_SPECIAL2)
	{
		UNITS_ChangeAnimMode(pUnit, OBJMODE_SPECIAL2);
		pQuestDataEx->nAncientStatueObjectMode[nIndex] = OBJMODE_SPECIAL2;
	}

	pQuestDataEx->bAncientMonsterSpawned[pQuestDataEx->nAncientsSpawned] = 1;
	pQuestDataEx->nAncientMonsterGUID[pQuestDataEx->nAncientsSpawned] = pAncient->dwUnitId;

	++pQuestDataEx->nAncientsSpawned;
	++pQuestDataEx->nAncientsAlive;

	pQuestDataEx->bAncientsActivated = 1;
}

//D2Game.0x6FCB6A50
void __fastcall ACT5Q5_DeactivateAncientMonsters(D2GameStrc* pGame, D2QuestDataStrc* pQuestData, D2Act5Quest5Strc* pQuestDataEx)
{
	for (int32_t i = 0; i < 3; ++i)
	{
		if (pQuestDataEx->bAncientMonsterSpawned[i])
		{
			D2UnitStrc* pAncientMonster = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pQuestDataEx->nAncientMonsterGUID[i]);
			if (pAncientMonster)
			{
				if (STATES_CheckState(pAncientMonster, STATE_UNINTERRUPTABLE))
				{
					sub_6FCBDE90(pAncientMonster, 0);
				}

				if (UNITS_GetRoom(pAncientMonster))
				{
					D2ModeChangeStrc modeChange = {};
					MONSTERMODE_GetModeChangeInfo(pAncientMonster, MONMODE_DEAD, &modeChange);
					D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);

					DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pAncientMonster), UNIT_MONSTER, pQuestDataEx->nAncientMonsterGUID[i]);
					UNITS_FreeCollisionPath(pAncientMonster);
					UNITROOM_RemoveUnitFromRoom(pAncientMonster);
				}

				--pQuestDataEx->nAncientsSpawned;
				--pQuestDataEx->nAncientsAlive;
			}
			else
			{
				SUNITINACTIVE_DeleteSingleListNode(pGame, UNIT_MONSTER, i + MONSTER_ANCIENTBARB1, ACT_V);
			}

			pQuestDataEx->bAncientMonsterSpawned[i] = 0;
		}
	}

	pQuestDataEx->bAncientsActivated = 0;

	for (int32_t i = 0; i < 3; ++i)
	{
		pQuestDataEx->bAncienStatueRespawned[i] = 0;
		D2UnitStrc* pAncientStatue = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nAncientStatueGUID[i]);
		if (pAncientStatue)
		{
			UNITS_ChangeAnimMode(pAncientStatue, OBJMODE_NEUTRAL);
		}
		pQuestDataEx->nAncientStatueObjectMode[i] = OBJMODE_NEUTRAL;
	}

	pQuestDataEx->nAltarObjectMode = OBJMODE_NEUTRAL;
	D2UnitStrc* pAltar = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nAltarGUID);
	if (pAltar)
	{
		UNITS_ChangeAnimMode(pAltar, OBJMODE_NEUTRAL);
	}
}

//D2Game.0x6FCB6BA0
void __fastcall ACT5Q5_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_REWARDGRANTED) || STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0) < 20 * (pGame->nDifficulty + 1))
	{
		return;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_V)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_REWARDGRANTED);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_PRIMARYGOALDONE);
	ACT5Q5_RewardPlayer(pGame, pUnit);
}

//D2Game.0x6FCB6C20
void __fastcall ACT5Q5_RewardPlayer(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	QUESTS_UpdatePlayerFlags(pGame, pUnit);

	int32_t nRewardExp = 1400000;
	if (pGame->nDifficulty == 1)
	{
		nRewardExp = 20000000;
	}
	else if (pGame->nDifficulty == 2)
	{
		nRewardExp = 40000000;
	}

	int32_t dwClassId = -1;
	if (pUnit)
	{
		dwClassId = pUnit->dwClassId;
	}

	const int32_t nMaxLevel = DATATBLS_GetMaxLevel(dwClassId);
	int32_t nCurrentLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);

	if (nCurrentLevel >= nMaxLevel)
	{
		return;
	}

	const uint32_t nNextThreshold = DATATBLS_GetLevelThreshold(dwClassId, nCurrentLevel + 1);
	const int32_t nDeltaExpThreshold = nNextThreshold - DATATBLS_GetLevelThreshold(dwClassId, nCurrentLevel);

	if (nRewardExp > (int32_t)nDeltaExpThreshold)
	{
		nRewardExp = nDeltaExpThreshold;
	}

	do
	{
		if (!nRewardExp)
		{
			break;
		}

		const int32_t nNextExp = STATLIST_GetUnitBaseStat(pUnit, STAT_NEXTEXP, 0);
		const int32_t nDeltaExp = nNextExp - STATLIST_GetUnitBaseStat(pUnit, STAT_EXPERIENCE, 0);

		if (nRewardExp < nDeltaExp)
		{
			const int32_t nExp = STATLIST_GetUnitBaseStat(pUnit, STAT_EXPERIENCE, 0);
			STATLIST_SetUnitStat(pUnit, STAT_EXPERIENCE, nExp + nRewardExp, 0);
			nRewardExp = 0;
		}
		else
		{
			STATLIST_SetUnitStat(pUnit, STAT_EXPERIENCE, nNextExp, 0);
			STATLIST_SetUnitStat(pUnit, STAT_LASTEXP, nDeltaExp, 0);
			PLAYERSTATS_LevelUp(pGame, pUnit);

			nRewardExp -= nDeltaExp;
			nCurrentLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
		}
	}
	while (nCurrentLevel < nMaxLevel);
}

//D2Game.0x6FCB6D30
bool __fastcall ACT5Q5_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId != MONSTER_QUAL_KEHK || pQuest->fState != 1)
	{
		return false;
	}

	return QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDGRANTED) == 0;
}

//D2Game.0x6FCB6D60
void __fastcall ACT5Q5_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT5Q5_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT5Q5_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT5Q5_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT5Q5_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT5Q5_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT5Q5_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT5Q5_Callback13_PlayerStartedGame;
	pQuestData->pNPCMessages = gpAct5Q5NpcMessages;
	pQuestData->bActive = 1;
	
	D2Act5Quest5Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act5Quest5Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->nQuest = QUESTSTATEFLAG_A5Q5;
	pQuestData->pfStatusFilter = 0;
	pQuestData->nInitNo = 4;
	pQuestData->pfActiveFilter = ACT5Q5_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT5Q5_SeqCallback;
	pQuestData->nSeqId = 36;
	memset(pQuestDataEx, 0x00, sizeof(D2Act5Quest5Strc));
}

//D2Game.0x6FCB6E30
void __fastcall ACT5Q5_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_QUAL_KEHK)
	{
		return;
	}

	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bQualKehkActivated != 1)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q5_UnitIterate_StatusCyclerEx, 1);
	pQuestDataEx->bQualKehkActivated = 0;
}

//D2Game.0x6FCB6E70
int32_t __fastcall ACT5Q5_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A5Q5_ANCIENTS);
	}

	return 0;
}

//D2Game.0x6FCB6EE0
void __fastcall ACT5Q5_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	const int16_t nMessageIndex = pQuestArg->nMessageIndex;
	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;
	if (nMessageIndex == 20153)
	{
		if (pQuestArg->nNPCNo == MONSTER_QUAL_KEHK)
		{
			if (!pQuestData->bNotIntro || pQuestData->fState != 1)
			{
				return;
			}

			QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			if (pQuestData->fLastState != 1)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q5_UnitIterate_StatusCyclerEx, 1);
			}
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q5_UnitIterate_UpdateQuestStateFlags);
			pQuestDataEx->bQualKehkActivated = 1;
			return;
		}
	}
	else if (nMessageIndex == 20002)
	{
		if (pQuestData->bNotIntro && pQuestData->fState < 4 && pQuestData->fLastState != 3)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 3, 0, ACT5Q5_UnitIterate_StatusCyclerEx, 1);
		}

		if (pQuestDataEx->nActivePortalCount > 0)
		{
			return;
		}

		D2UnitStrc* pUnit[3] = {};
		int32_t i = 0;
		int32_t j = 0; // TODO: Does j need to ne reset inside the loop?
		while (pQuestDataEx->nAncientStatueObjectMode[i] == OBJMODE_NEUTRAL)
		{
			++i;
			if (i >= 3)
			{
				while (1)
				{
					pUnit[j] = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nAncientStatueGUID[j]);
					if (!pUnit[j])
					{
						break;
					}

					DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pUnit[j]), 0);
					++j;
					if (j >= 3)
					{
						for (int32_t k = 0; k < 3; ++k)
						{
							UNITS_ChangeAnimMode(pUnit[k], OBJMODE_SPECIAL1);
							UNITS_FreeCollisionPath(pUnit[k]);
							EVENT_SetEvent(pQuestData->pGame, pUnit[k], UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 20, 0, 0);
							pQuestDataEx->nAncientStatueObjectMode[k] = OBJMODE_SPECIAL1;
						}

						pQuestDataEx->nAncientsSpawned = 0;
						pQuestDataEx->nAncientsAlive = 0;
						pQuestDataEx->unk0x10 = 1;
						pQuestDataEx->bActivatable = 1;
						return;
					}
				}
				return;
			}
		}
		return;
	}
	else if (nMessageIndex == 20169)
	{
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], QUESTSTATEFLAG_A5Q5, QFLAG_ENTERAREA);
		return;
	}

	switch (pQuestArg->nNPCNo)
	{
	case MONSTER_LARZUK:
		if (nMessageIndex == 20167)
		{
			QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuest, QFLAG_CUSTOM1);
		}
		break;

	case MONSTER_CAIN6:
		if (nMessageIndex == 20165)
		{
			QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuest, QFLAG_CUSTOM2);
		}
		break;

	case MONSTER_DREHYA:
		if (nMessageIndex == 20166)
		{
			QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuest, QFLAG_CUSTOM3);
		}
		break;

	case MONSTER_MALAH:
		if (nMessageIndex == 20168)
		{
			QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuest, QFLAG_CUSTOM4);
		}
		break;

	case MONSTER_QUAL_KEHK:
		if (nMessageIndex == 20164)
		{
			QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuest, QFLAG_CUSTOM5);
		}
		break;

	default:
		return;
	}
}

//D2Game.0x6FCB7160
int32_t __fastcall ACT5Q5_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_REWARDGRANTED) == 1 || STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0) < 20 * (pGame->nDifficulty + 1))
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_LEAVETOWN);
	}

	return 0;
}

//D2Game.0x6FCB71E0
void __fastcall ACT5Q5_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4, 0
	};

	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	if (!pQuestData->bNotIntro)
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED))
	{
		int32_t nFlag = 0;
		switch (nNpcId)
		{
		case MONSTER_LARZUK:
			nFlag = QFLAG_CUSTOM1;
			break;

		case MONSTER_CAIN6:
			nFlag = QFLAG_CUSTOM2;
			break;

		case MONSTER_DREHYA:
			nFlag = QFLAG_CUSTOM3;
			break;

		case MONSTER_MALAH:
			nFlag = QFLAG_CUSTOM4;
			break;

		case MONSTER_QUAL_KEHK:
			nFlag = QFLAG_CUSTOM5;
			break;

		default:
			return;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, nFlag))
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE))
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
			}
		}
		else
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 5);
		}
	}
	else
	{
		if (nNpcId >= 537 && nNpcId <= 539 && !pQuestDataEx->unk0x10)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
		}

		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < 9)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//
void __fastcall ACT5Q5_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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

//D2Game.0x6FCB7370
void __fastcall ACT5Q5_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	//D2Act5Quest5Strc* pQuestDataEx = nullptr;
	//D2BitBufferStrc* pQuestFlags = nullptr;
	//D2QuestDataStrc* pQuest = nullptr;
	//D2UnitStrc* pObject = nullptr;
	//D2UnitStrc* pMissile = nullptr;
	//D2MissileStrc missileParams = {};
	//int32_t nMonsterId = 0;
	//int32_t nObjectId = 0;

	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;

	if (!pQuestDataEx->bActivatable || !pQuestDataEx->unk0x10 || pQuestDataEx->nActivePortalCount)
	{
		return;
	}

	if (!pQuestDataEx->bTimerActive)
	{
		pQuestDataEx->bTimerActive = 1;
		QUESTS_CreateTimer(pQuestData, ACT5Q5_Timer_RespawnAncientStatueObjects, 2);
	}

	int32_t nMonsterId = -1;
	if (pQuestArg->pTarget)
	{
		nMonsterId = pQuestArg->pTarget->dwClassId;
	}

	int32_t nObjectId = 0;
	switch (nMonsterId)
	{
	case MONSTER_ANCIENTBARB1:
		pQuestDataEx->bAncienStatueRespawned[2] = 1;
		nObjectId = pQuestDataEx->nAncientStatueGUID[2];
		break;

	case MONSTER_ANCIENTBARB2:
		pQuestDataEx->bAncienStatueRespawned[0] = 1;
		nObjectId = pQuestDataEx->nAncientStatueGUID[0];
		break;

	case MONSTER_ANCIENTBARB3:
		pQuestDataEx->bAncienStatueRespawned[1] = 1;
		nObjectId = pQuestDataEx->nAncientStatueGUID[1];
		break;

	default:
		return;
	}

	D2UnitStrc* pObject = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, nObjectId);
	if (pObject)
	{
		D2MissileStrc missileParams = {};
		missileParams.dwFlags = 0x420;
		missileParams.nMissile = 541;
		missileParams.pOwner = pQuestArg->pTarget;
		missileParams.pOrigin = pQuestArg->pTarget;
		missileParams.nTargetX = CLIENTS_GetUnitX(pObject);
		missileParams.nTargetY = CLIENTS_GetUnitY(pObject);
		missileParams.nSkillLevel = 1;

		D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pQuestData->pGame, &missileParams);
		if (pMissile)
		{
			MISSILE_SetTargetX(pMissile, nObjectId);
		}
	}

	QUESTS_TriggerFX(pQuestData->pGame, 18);

	--pQuestDataEx->nAncientsAlive;

	if (pQuestDataEx->nAncientsAlive)
	{
		return;
	}

	pQuestDataEx->bAncientsDefeated = 1;
	if (!pQuestData->bNotIntro)
	{
		return;
	}

	if (pQuestArg->pPlayer)
	{
		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_REWARDGRANTED))
		{
			if (STATLIST_GetUnitBaseStat(pQuestArg->pPlayer, STAT_LEVEL, 0) >= 20 * (pQuestData->pGame->nDifficulty + 1))
			{
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_REWARDGRANTED);
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_PRIMARYGOALDONE);
				ACT5Q5_RewardPlayer(pQuestArg->pGame, pQuestArg->pPlayer);
			}
		}
	}

	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT5Q5_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT5Q5_UnitIterate_SetCompletionFlag);
	QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);

	SUNIT_AllocUnitData(UNIT_OBJECT, 561, CLIENTS_GetUnitX(pQuestArg->pTarget), CLIENTS_GetUnitY(pQuestArg->pTarget), pQuestArg->pGame, UNITS_GetRoom(pQuestArg->pTarget), 1, 0, 0);

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

	if (pQuestData->fLastState != 13)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 13, 0, ACT5Q5_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCB7680
bool __fastcall ACT5Q5_Timer_RespawnAncientStatueObjects(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;

	if (pQuestDataEx->nActivePortalCount || !pQuestDataEx->bActivatable)
	{
		pQuestDataEx->bTimerActive = 0;
		return true;
	}

	bool bStatueNotFound = false;
	for (int32_t i = 0; i < 3; ++i)
	{
		if (pQuestDataEx->bAncienStatueRespawned[i])
		{
			D2UnitStrc* pAncientStatue = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nAncientStatueGUID[i]);

			if (pAncientStatue)
			{
				UNITS_ChangeAnimMode(pAncientStatue, OBJMODE_OPERATING);

				D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pAncientStatue->dwClassId);
				EVENT_SetEvent(pGame, pAncientStatue, UNITEVENTCALLBACK_ENDANIM, pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);

				pQuestDataEx->bAncienStatueRespawned[i] = 0;
				pQuestDataEx->nAncientStatueObjectMode[i] = OBJMODE_OPENED;
			}
			else
			{
				bStatueNotFound = true;
			}
		}
	}

	if (bStatueNotFound)
	{
		return false;
	}

	pQuestDataEx->bTimerActive = 0;
	return true;
}

//D2Game.0x6FCB7760
int32_t __fastcall ACT5Q5_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_PRIMARYGOALDONE) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A5Q5_ANCIENTS, 0);
	}

	return 0;
}

//D2Game.0x6FCB77C0
int32_t __fastcall ACT5Q5_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS);
	D2_ASSERT(pQuestData);

	if (STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0) < 20 * (pGame->nDifficulty + 1) || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_ROCKYSUMMIT)
	{
		return 0;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED))
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED);
	ACT5Q5_RewardPlayer(pGame, pUnit);

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT5Q5_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCB78A0
void __fastcall ACT5Q5_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->nOldLevel == LEVEL_HARROGATH)
	{
		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);
		if (!pQuestData->bNotIntro)
		{
			return;
		}

		if (pQuestData->fState == 2)
		{
			if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], pQuestData->nQuest, QFLAG_REWARDGRANTED) == 1)
			{
				return;
			}

			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);

			if (pQuestData->fLastState != 1)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q5_UnitIterate_StatusCyclerEx, 1);
			}

			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q5_UnitIterate_UpdateQuestStateFlags);
		}
	}

	if (pQuestArg->nNewLevel == LEVEL_ROCKYSUMMIT)
	{
		if (pQuestData->fLastState < 2 && pQuestData->fState > 2)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 2, 0, ACT5Q5_UnitIterate_StatusCyclerEx, 1);
		}

		if (pQuestData->fState >= 3)
		{
			if (pQuestData->fLastState >= 2 || pQuestData->fState <= 2)
			{
				if (!pQuestDataEx->bAncientsDefeated && pQuestDataEx->bSummitDoorInitialized)
				{
					D2UnitStrc* pSummitDoor = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nSummitDoorGUID);
					if (pSummitDoor && pQuestDataEx->nSummitDoorObjectMode == OBJMODE_NEUTRAL)
					{
						pQuestDataEx->nSummitDoorObjectMode = OBJMODE_OPENED;
						UNITS_ChangeAnimMode(pSummitDoor, OBJMODE_OPERATING);
					}
				}
				return;
			}
		}
		else
		{
			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		}

		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q5_UnitIterate_UpdateQuestStateFlags);

		if (!pQuestDataEx->bAncientsDefeated && pQuestDataEx->bSummitDoorInitialized)
		{
			D2UnitStrc* pSummitDoor = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nSummitDoorGUID);
			if (pSummitDoor && pQuestDataEx->nSummitDoorObjectMode == OBJMODE_NEUTRAL)
			{
				pQuestDataEx->nSummitDoorObjectMode = OBJMODE_OPENED;
				UNITS_ChangeAnimMode(pSummitDoor, OBJMODE_OPERATING);
			}
		}
	}
}

//D2Game.0x6FCB79F0
bool __fastcall ACT5Q5_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (pQuestData->fState != 5 && pQuestData->bNotIntro)
	{
		if (!pQuestData->fState)
		{
			pQuestData->fState = 1;
		}
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

//D2Game.0x6FCB7A60
void __fastcall ACT5Q5_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		((D2Act5Quest5Strc*)pQuestData->pQuestDataEx)->bQuestCompletedBefore = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fLastState = 1;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_STARTED) == 1)
	{
		pQuestData->fLastState = 1;
		pQuestData->fState = 2;
	}
}

//D2Game.0x6FCB7AF0
void __fastcall ACT5Q5_OnPortalOpened(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_ROCKYSUMMIT)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS);
	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bAncientsDefeated)
	{
		return;
	}

	++pQuestDataEx->nActivePortalCount;
	pQuestDataEx->bActivatable = 0;
	pQuestDataEx->unk0x10 = 0;
	ACT5Q5_DeactivateAncientMonsters(pGame, pQuestData, pQuestDataEx);
}

//D2Game.0x6FCB7B40
void __fastcall ACT5Q5_OnPortalClosed(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_ROCKYSUMMIT)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS);
	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;

	--pQuestDataEx->nActivePortalCount;
	if (!pQuestDataEx->nActivePortalCount && pQuestDataEx->unk0x10)
	{
		pQuestDataEx->bActivatable = 1;
	}
}

//D2Game.0x6FCB7B90
int32_t __fastcall ACT5Q5_IsNotActivatable(D2GameStrc* pGame)
{
	return ((D2Act5Quest5Strc*)QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS)->pQuestDataEx)->bActivatable == 0;
}

//D2Game.0x6FCB7BB0
int32_t __fastcall ACT5Q5_GetPortalCount(D2GameStrc* pGame)
{
	return ((D2Act5Quest5Strc*)QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS)->pQuestDataEx)->nActivePortalCount;
}

//D2Game.0x6FCB7BD0
int32_t __fastcall ACT5Q5_IsActivatable(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS);

	return pQuestData && ((D2Act5Quest5Strc*)pQuestData->pQuestDataEx)->bActivatable;
}

//D2Game.0x6FCB7C00
int32_t __fastcall ACT5Q5_IsArreatSummitClosed(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTargetLevel)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS);

	if (pQuestData && pQuestData->bNotIntro && pQuestData->pQuestDataEx)
	{
		return ((D2Act5Quest5Strc*)pQuestData->pQuestDataEx)->bAncientsDefeated == 0;
	}

	return 0;
}

//D2Game.0x6FCB7C30
void __fastcall OBJECTS_InitFunction65_AncientStatue2(D2ObjInitFnStrc* pOp)
{
	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q5_ANCIENTS)->pQuestDataEx;

	if (pOp->pObject)
	{
		pQuestDataEx->nAncientStatueGUID[0] = pOp->pObject->dwUnitId;

		switch (pOp->pObject->dwClassId)
		{
		case OBJECT_ANCIENTSTATUE3:
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAncientStatueObjectMode[1]);
			break;
		case OBJECT_ANCIENTSTATUE1:
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAncientStatueObjectMode[0]);
			break;
		case OBJECT_ANCIENTSTATUE2:
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAncientStatueObjectMode[2]);
			break;
		}
	}
	else
	{
		pQuestDataEx->nAncientStatueGUID[0] = -1;
	}
}

//D2Game.0x6FCB7CB0
void __fastcall OBJECTS_InitFunction63_AncientStatue3(D2ObjInitFnStrc* pOp)
{
	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q5_ANCIENTS)->pQuestDataEx;

	if (pOp->pObject)
	{
		pQuestDataEx->nAncientStatueGUID[1] = pOp->pObject->dwUnitId;

		switch (pOp->pObject->dwClassId)
		{
		case OBJECT_ANCIENTSTATUE3:
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAncientStatueObjectMode[1]);
			break;

		case OBJECT_ANCIENTSTATUE1:
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAncientStatueObjectMode[0]);
			break;

		case OBJECT_ANCIENTSTATUE2:
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAncientStatueObjectMode[2]);
			break;
		}
	}
	else
	{
		pQuestDataEx->nAncientStatueGUID[1] = -1;
	}
}

//D2Game.0x6FCB7D30
void __fastcall OBJECTS_InitFunction64_AncientStatue1(D2ObjInitFnStrc* pOp)
{
	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q5_ANCIENTS)->pQuestDataEx;

	if (pOp->pObject)
	{
		pQuestDataEx->nAncientStatueGUID[2] = pOp->pObject->dwUnitId;

		switch (pOp->pObject->dwClassId)
		{
		case OBJECT_ANCIENTSTATUE3:
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAncientStatueObjectMode[1]);
			break;

		case OBJECT_ANCIENTSTATUE1:
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAncientStatueObjectMode[0]);
			break;

		case OBJECT_ANCIENTSTATUE2:
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAncientStatueObjectMode[2]);
			break;
		}
	}
	else
	{
		pQuestDataEx->nAncientStatueGUID[2] = -1;
	}
}

//D2Game.0x6FCB7DB0
int32_t __fastcall OBJECTS_OperateFunction62_63_64_AncientStatue(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (pOp && pOp->pPlayer)
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
	}

	return 0;
}

//D2Game.0x6FCB7DD0
void __fastcall OBJECTS_InitFunction72_AncientsAltar(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q5_ANCIENTS);
	if (!pQuestData)
	{
		return;
	}

	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}
	((D2Act5Quest5Strc*)pQuestData->pQuestDataEx)->nAltarGUID = nUnitId;

	UNITS_ChangeAnimMode(pOp->pObject, ((D2Act5Quest5Strc*)pQuestData->pQuestDataEx)->nAltarObjectMode);
}

//D2Game.0x6FCB7E10
void __fastcall OBJECTS_InitFunction73_AncientsDoor(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q5_ANCIENTS);

	if (!pQuestData)
	{
		return;
	}

	if (((D2Act5Quest5Strc*)pQuestData->pQuestDataEx)->bQuestCompletedBefore)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
	else
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_NEUTRAL);
	}
}

//D2Game.0x6FCB7E50
int32_t __fastcall OBJECTS_OperateFunction65_AncientsAltar(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q5_ANCIENTS);

	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		return 1;
	}

	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;

	if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
	{
		return 1;
	}

	if (pQuestData->bNotIntro)
	{
		if (pQuestData->fState < 4)
		{
			if (pQuestDataEx->nActivePortalCount > 0)
			{
				SUNIT_IterateUnitsOfType(pOp->pGame, 0, 0, ACT5Q5_UnitIterate_ClosePortals);
			}

			QUESTS_SendScrollMessage(pOp->pPlayer, pObject, 20002);

			if (pQuestData->fState < 2)
			{
				QUESTS_StateDebug(pQuestData, 2, "..\\D2Game/Quests/a5q5.cpp", 1439);
			}

			if (pQuestData->fLastState != 3)
			{
				QUESTS_UnitIterate(pQuestData, 3, 0, ACT5Q5_UnitIterate_StatusCyclerEx, 1);
			}
		}
	}
	else
	{
		if (pQuestDataEx->nActivePortalCount > 0)
		{
			SUNIT_IterateUnitsOfType(pOp->pGame, 0, 0, ACT5Q5_UnitIterate_ClosePortals);
		}

		QUESTS_SendScrollMessage(pOp->pPlayer, pObject, 20002);
	}

	UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
	pQuestDataEx->nAltarObjectMode = OBJMODE_OPENED;
	return 1;
}

//D2Game.0x6FCB7F40
int32_t __fastcall ACT5Q5_UnitIterate_ClosePortals(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2UnitStrc* pPortal = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, PLAYER_GetUniqueIdFromPlayerData(pUnit));
	if (!pPortal)
	{
		return 0;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pPortal);
	if (DUNGEON_GetLevelIdFromRoom(pRoom) == LEVEL_ROCKYSUMMIT)
	{
		sub_6FC7C170(pGame, pUnit);
	}

	return 0;
}

//D2Game.0x6FCB7F80
int32_t __fastcall OBJECTS_OperateFunction66_AncientsDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q5_ANCIENTS);

	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		return 1;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;
	if (!pQuestDataEx->bAncientsDefeated)
	{
		if (pQuestData->bNotIntro || pQuestDataEx->bAncientsActivated)
		{
			SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
			return 0;
		}
	}

	D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);

	if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
	{
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
		DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pObject), 0);
		EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, pOp->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);
	}
	else if (pObject->dwAnimMode == OBJMODE_OPENED)
	{
		D2GAME_WarpPlayerToTile_6FCA5B20(pOp);
		DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pObject), 1);
		return 0;
	}

	return 1;
}

//D2Game.0x6FCB80B0
void __fastcall ACT5Q5_OnPlayerDied(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_ROCKYSUMMIT)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS);
	if (!pQuestData || !pQuestData->bNotIntro)
	{
		return;
	}

	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->nActivePortalCount || pQuestDataEx->bAncientsDefeated || !pQuestDataEx->bActivatable)
	{
		return;
	}

	pQuestDataEx->nLivingPlayersOnSummit = 0;
	SUNIT_IterateUnitsOfType(pGame, 0, 0, ACT5Q5_UnitIterate_CountLivingPlayers);

	if (!pQuestDataEx->nLivingPlayersOnSummit)
	{
		pQuestDataEx->bActivatable = 0;
		pQuestDataEx->unk0x10 = 0;
		ACT5Q5_DeactivateAncientMonsters(pGame, pQuestData, pQuestDataEx);
	}
}

//D2Game.0x6FCB8140
int32_t __fastcall ACT5Q5_UnitIterate_CountLivingPlayers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_ROCKYSUMMIT)
	{
		return 0;
	}

	if (!pUnit || pUnit->dwAnimMode == PLRMODE_DEATH || pUnit->dwAnimMode == PLRMODE_DEAD)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS);
	if (pQuestData && pQuestData->pQuestDataEx)
	{
		++((D2Act5Quest5Strc*)pQuestData->pQuestDataEx)->nLivingPlayersOnSummit;
	}

	return 0;
}

//D2Game.0x6FCB8190
int32_t __fastcall OBJECTS_OperateFunction69_InvisibleAncient(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q5, QFLAG_ENTERAREA))
	{
		QUESTS_SendScrollMessage(pOp->pPlayer, pOp->pObject, 20169);
	}

	return 1;
}

//D2Game.0x6FCB81F0
void __fastcall OBJECTS_InitFunction76_SummitDoor(D2ObjInitFnStrc* pOp)
{
	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q5_ANCIENTS)->pQuestDataEx;
	pQuestDataEx->bSummitDoorInitialized = 1;

	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}
	pQuestDataEx->nSummitDoorGUID = nUnitId;

	UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nSummitDoorObjectMode);
	if (!pQuestDataEx->bAncientsDefeated && pQuestDataEx->nSummitDoorObjectMode != OBJMODE_OPENED)
	{
		pQuestDataEx->nSummitDoorObjectMode = OBJMODE_OPENED;
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPERATING);
	}
}

//D2Game.0x6FCB8250
int32_t __fastcall OBJECTS_OperateFunction71_SummitDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q5_ANCIENTS);
	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		return 0;
	}

	D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;

	if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
	{
		if (!pQuestDataEx->bAncientsDefeated)
		{
			UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
			pQuestDataEx->nSummitDoorObjectMode = OBJMODE_OPENED;
			return 0;
		}

		D2GAME_WarpPlayerToTile_6FCA5B20(pOp);
		return 0;
	}

	if (pObject->dwAnimMode != OBJMODE_OPERATING)
	{
		if (pObject->dwAnimMode == OBJMODE_OPENED)
		{
			if (pQuestDataEx->bAncientsDefeated)
			{
				if (pQuestDataEx->bAncientsActivated)
				{
					UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
					pQuestDataEx->nSummitDoorObjectMode = OBJMODE_NEUTRAL;
				}
			}
			else
			{
				SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
			}

			if (pQuestDataEx->bAncientsDefeated)
			{
				pQuestDataEx->nSummitDoorObjectMode = OBJMODE_NEUTRAL;
				UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
			}
			else
			{
				SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
				UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
			}
		}
		return 0;
	}

	if (pQuestDataEx->bAncientsDefeated)
	{
		pQuestDataEx->nSummitDoorObjectMode = OBJMODE_NEUTRAL;
		UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
	}
	else
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
	}

	return 1;
}
