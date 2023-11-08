#include "QUESTS/ACT5/A5Q2.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2Monsters.h>
#include <D2QuestRecord.h>

#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "MONSTER/MonsterSpawn.h"
#include "OBJECTS/Objects.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD3C430
D2NPCMessageTableStrc gpAct5Q2NpcMessages[] =
{
	{
		{
			{ MONSTER_QUAL_KEHK, 20096, 0, 0, },
		},
		1
	},
	{
		{
			{ MONSTER_LARZUK, 20100, 0, 2, },
			{ MONSTER_CAIN6, 20098, 0, 2, },
			{ MONSTER_DREHYA, 20099, 0, 2, },
			{ MONSTER_MALAH, 20101, 0, 2, },
			{ MONSTER_NIHLATHAK, 20102, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20097, 0, 2, },
		},
		6
	},
	{
		{
			{ MONSTER_LARZUK, 20107, 0, 2, },
			{ MONSTER_CAIN6, 20105, 0, 2, },
			{ MONSTER_DREHYA, 20106, 0, 2, },
			{ MONSTER_MALAH, 20108, 0, 2, },
			{ MONSTER_NIHLATHAK, 20109, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20103, 0, 2, },
		},
		6
	},
	{
		{
			{ MONSTER_LARZUK, 20113, 0, 2, },
			{ MONSTER_CAIN6, 20111, 0, 2, },
			{ MONSTER_DREHYA, 20112, 0, 2, },
			{ MONSTER_MALAH, 20114, 0, 2, },
			{ MONSTER_NIHLATHAK, 20115, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20110, 0, 0, },
		},
		6
	},
	{
		{
			{ MONSTER_LARZUK, 20113, 0, 2, },
			{ MONSTER_CAIN6, 20111, 0, 2, },
			{ MONSTER_DREHYA, 20112, 0, 2, },
			{ MONSTER_MALAH, 20114, 0, 2, },
			{ MONSTER_NIHLATHAK, 20115, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20110, 0, 2, },
		},
		6
	},
	{
		{
			{ MONSTER_QUAL_KEHK, 20104, 0, 2, },
		},
		1
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


//D2Game.0x6FCB1F30
void __fastcall ACT5Q2_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_V)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDPENDING);

	const int32_t nFreedWussies = ((D2Act5Quest2Strc*)QUESTS_GetQuestData(pGame, QUEST_A5Q2_RESCUESOLDIERS)->pQuestDataEx)->nFreedWussies;
	if (nFreedWussies == 15)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_CUSTOM1);
	}
	else if (nFreedWussies == 14)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_CUSTOM2);
	}
	else
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_CUSTOM3);
	}
}

//D2Game.0x6FCB1FF0
bool __fastcall ACT5Q2_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId == MONSTER_QUAL_KEHK)
	{
		if (pQuest->fState != 1)
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDPENDING))
			{
				return true;
			}

			return false;
		}

		if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDPENDING))
		{
			return true;
		}
	}
	else if (nNpcId == MONSTER_ACT5POW)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDPENDING))
		{
			return true;
		}
	}

	return false;
}

//D2Game.0x6FCB2080
void __fastcall ACT5Q2_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT5Q2_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT5Q2_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT5Q2_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT5Q2_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT5Q2_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT5Q2_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT5Q2_Callback13_PlayerStartedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERJOINEDGAME] = ACT5Q2_Callback14_PlayerJoinedGame;
	pQuestData->pNPCMessages = gpAct5Q2NpcMessages;
	pQuestData->bActive = 1;

	D2Act5Quest2Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act5Quest2Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->nQuest = QUESTSTATEFLAG_A5Q2;
	pQuestData->pfStatusFilter = 0;
	pQuestData->nInitNo = 4;
	pQuestData->pfActiveFilter = ACT5Q2_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT5Q2_SeqCallback;
	pQuestData->nSeqId = 33;
	memset(pQuestDataEx, 0x00, sizeof(D2Act5Quest2Strc));
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->tPlayerGUIDs);
}

//D2Game.0x6FCB2160
void __fastcall ACT5Q2_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_QUAL_KEHK)
	{
		return;
	}

	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bQualKehkActivated != 1 || pQuestDataEx->nKilledWussies >= 5)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q2_UnitIterate_StatusCyclerEx, 1);
	pQuestDataEx->bQualKehkActivated = 0;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
}

//D2Game.0x6FCB21C0
int32_t __fastcall ACT5Q2_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A5Q2_RESCUESOLDIERS);
	}

	return 0;
}

//D2Game.0x6FCB2230
void __fastcall ACT5Q2_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	constexpr uint32_t gdwAct5Q2RuneCodes[] =
	{
		' 70r', ' 80r', ' 90r'
	};

	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->nNPCNo != MONSTER_QUAL_KEHK)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

	if (pQuestArg->nMessageIndex == 20096)
	{
		pQuestDataEx->bQualKehkActivated = 1;
		QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q2_UnitIterate_UpdateQuestStateFlags);
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
	}
	else if (pQuestArg->nMessageIndex == 20110)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING) == 1)
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
					QUESTS_UnitIterate(pQuestData, 13, 0, ACT5Q2_UnitIterate_StatusCyclerEx, 0);
				}
				pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
			}

			int32_t nRewards = 0;
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_CUSTOM2))
			{
				nRewards = 2;
			}
			else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_CUSTOM3))
			{
				nRewards = 1;
			}
			else
			{
				nRewards = 3;
			}

			if (nRewards > 0)
			{
				bool bRewardGranted = false;
				for (int32_t i = 0; i < nRewards; ++i)
				{
					if (QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, gdwAct5Q2RuneCodes[i], 0, ITEMQUAL_NORMAL, 1))
					{
						bRewardGranted = true;
					}
				}

				if (bRewardGranted)
				{
					QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED);
					QUESTRECORD_ClearQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING);
					QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, pQuestData->nQuest);

					QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));

					D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pQuestArg->pPlayer, __FILE__, __LINE__);

					D2GSPacketSrv5D packet5D = {};
					packet5D.nHeader = 0x5D;
					packet5D.nQuestId = QUEST_A5Q2_RESCUESOLDIERS;
					packet5D.nFlags = 2;
					D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5D, sizeof(packet5D));
				}
			}

			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		}
	}
}

//D2Game.0x6FCB2460
int32_t __fastcall ACT5Q2_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q2_RESCUESOLDIERS);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_LEAVETOWN);
	}

	return 0;
}

//D2Game.0x6FCB24D0
void __fastcall ACT5Q2_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4
	};

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
		return;
	}

	if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)) == 1)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) == 1
		|| (pQuestData->fState >= 4 && !QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE))
		|| !pQuestData->bNotIntro)
	{
		return;
	}

	const int32_t nIndex = nIndices[pQuestData->fState];
	if (nIndex == 2 && nNpcId == MONSTER_QUAL_KEHK)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_ENTERAREA))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_QUAL_KEHK, 5);
		}
		else
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
	else if (nIndex != -1 && nIndex < 12)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
	}
}

//D2Game.0x6FCB2610
void __fastcall ACT5Q2_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;

	if (pQuestArg->pPlayer)
	{
		QUESTS_FastRemovePlayerGUID(&pQuestData->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
	}
	else
	{
		QUESTS_FastRemovePlayerGUID(&pQuestData->tPlayerGUIDs, -1);
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, -1);
	}
}

//D2Game.0x6FCB2650
void __fastcall ACT5Q2_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;

	if (!pQuestData->bNotIntro)
	{
		return;
	}

	int32_t nUnitGUID = -1;
	if (pQuestArg->pTarget)
	{
		nUnitGUID = pQuestArg->pTarget->dwUnitId;
	}

	if (pQuestArg->pTarget && pQuestArg->pTarget->dwClassId == MONSTER_PRISONDOOR)
	{
		D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pQuestArg->pTarget);
		DUNGEON_ToggleHasPortalFlag(pRoom, 0);
		if (pRoom)
		{
			D2ActiveRoomStrc** ppRoomList = nullptr;
			int32_t nNumRooms = 0;
			DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

			for (int32_t j = 0; j < nNumRooms; ++j)
			{
				for (D2UnitStrc* i = ppRoomList[j]->pUnitFirst; i; i = i->pRoomNext)
				{
					if (i->dwUnitType == UNIT_MONSTER && i->dwClassId == MONSTER_ACT5POW && i->dwAnimMode != MONMODE_DEAD && i->dwAnimMode != MONMODE_DEATH && UNITS_GetDistanceToOtherUnit(i, pQuestArg->pTarget) < 15)
					{
						pQuestDataEx->nFreedWussieUnitGUIDs[pQuestDataEx->nWussiesInRangeToDoor] = i->dwUnitId;
						++pQuestDataEx->nWussiesInRangeToDoor;
						++pQuestDataEx->nFreedWussies;
					}
				}
			}
		}

		if (pQuestDataEx->nKilledWussies >= 5 && pQuestData->fLastState != 12)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 12, 0, ACT5Q2_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->bQualKehkActivated = 0;
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, pQuestArg->pPlayer, ACT5Q2_UnitIterate_SetCompletionFlag);
		}
	}
	else
	{
		if (pQuestDataEx->nWussiesInRangeToDoor > 0)
		{
			int32_t nCounter = 0;
			while (nUnitGUID != pQuestDataEx->nFreedWussieUnitGUIDs[nCounter])
			{
				++nCounter;
				if (nCounter >= pQuestDataEx->nWussiesInRangeToDoor)
				{
					++pQuestDataEx->nKilledWussies;

					if (pQuestDataEx->nKilledWussies >= 5 && pQuestData->fLastState != 12)
					{
						pQuestData->dwFlags &= 0xFFFFFF00;
						QUESTS_UnitIterate(pQuestData, 12, 0, ACT5Q2_UnitIterate_StatusCyclerEx, 1);
						pQuestDataEx->bQualKehkActivated = 0;
						SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, pQuestArg->pPlayer, ACT5Q2_UnitIterate_SetCompletionFlag);
					}
					break;
				}
			}
		}
	}

	int32_t* pGUIDs = pQuestDataEx->unk0xB0;
	int32_t nCounter = 0;
	int32_t nIndex = 0;
	while (nUnitGUID != pGUIDs[nCounter])
	{
		++nCounter;
		if (nCounter >= 5)
		{
			++nIndex;

			if (nIndex >= 3)
			{
				return;
			}

			pGUIDs += 5;
			nCounter = 0;
		}
	}

	if (nIndex >= 0)
	{
		++pQuestDataEx->unk0x114[nIndex];
		if (pQuestDataEx->unk0x114[nIndex] >= 5)
		{
			pQuestDataEx->unk0x108[nIndex] = 1;
		}
	}
}

//D2Game.0x6FCB2860
int32_t __fastcall ACT5Q2_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDGRANTED) != 1
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDPENDING) != 1
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_PRIMARYGOALDONE) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A5Q2_RESCUESOLDIERS, 0);
	}

	return 0;
}

//D2Game.0x6FCB28D0
void __fastcall ACT5Q2_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel >= LEVEL_ID_ACT5_BARRICADE_1 && pQuestArg->nNewLevel <= LEVEL_ARREATPLATEAU && pQuestData->bNotIntro && ((D2Act5Quest2Strc*)pQuestData->pQuestDataEx)->nKilledWussies < 5)
	{
		if (pQuestData->fState == 1 || pQuestData->fState == 2)
		{
			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		}

		if (pQuestData->fLastState)
		{
			if (pQuestData->fState != 1 && pQuestData->fState != 2)
			{
				return;
			}
		}
		else
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 1, nullptr, ACT5Q2_UnitIterate_StatusCyclerEx, 1);
			pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
		}
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q2_UnitIterate_UpdateQuestStateFlags);
	}
	else if (pQuestArg->nOldLevel == LEVEL_HARROGATH)
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
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q2_UnitIterate_UpdateQuestStateFlags);
		if (pQuestData->fLastState || !pQuestData->bNotIntro || ((D2Act5Quest2Strc*)pQuestData->pQuestDataEx)->nKilledWussies >= 5)
		{
			return;
		}

		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, nullptr, ACT5Q2_UnitIterate_StatusCyclerEx, 1);
		pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	}
}

//D2Game.0x6FCB2A20
bool __fastcall ACT5Q2_SeqCallback(D2QuestDataStrc* pQuestData)
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

//D2Game.0x6FCB2A90
void __fastcall ACT5Q2_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTRECORD_ClearQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A5Q2, QFLAG_ENTERAREA);
}

//D2Game.0x6FCB2AC0
void __fastcall ACT5Q2_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	QUESTRECORD_ClearQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_ENTERAREA);

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_STARTED) == 1)
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 1;
	}
}

//D2Game.0x6FCB2B60
int32_t __fastcall ACT5Q2_SpawnCagedWussies(D2QuestDataStrc* pQuestData, D2ObjInitFnStrc* pOp, int32_t nCount)
{
	if (!pQuestData || !pQuestData->bNotIntro)
	{
		return 0;
	}

	int32_t nWussiesSpawned = 0;
	for (int32_t i = 0; i < 25; ++i)
	{
		D2UnitStrc* pWussies = D2GAME_SpawnMonster_6FC69F10(pQuestData->pGame, pOp->pRoom, pOp->nX, pOp->nY, MONSTER_ACT5POW, 1, 5, 0);
		if (pWussies)
		{
			pWussies->dwFlags |= (UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT);

			((D2Act5Quest2Strc*)pQuestData->pQuestDataEx)->unk0xB0[nWussiesSpawned + 5 * nCount] = pWussies->dwUnitId;
			++nWussiesSpawned;

			QUESTS_CreateChainRecord(pQuestData->pGame, pWussies, QUEST_A5Q2_RESCUESOLDIERS);
			if (nWussiesSpawned >= 5)
			{
				return nWussiesSpawned;
			}
		}
	}

	return nWussiesSpawned;
}

//D2Game.0x6FCB2C00
void __fastcall OBJECTS_InitFunction62_CagedWussie(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q2_RESCUESOLDIERS);
	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;

	if (!pQuestData->bNotIntro)
	{
		return;
	}

	if (pQuestData->fState < 2)
	{
		QUESTS_StateDebug(pQuestData, 2, "..\\D2Game/Quests/a5q2.cpp", 860);
	}

	if (!pQuestData->fLastState && pQuestDataEx->nKilledWussies < 5)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q2_UnitIterate_StatusCyclerEx, 1);
	}

	int32_t nCounter = 0;
	while (pQuestDataEx->bWussiesSpawned[nCounter])
	{
		++nCounter;
		if (nCounter >= 3)
		{
			return;
		}
	}

	if (nCounter <= 0)
	{
		pQuestDataEx->pWussieCoords[nCounter].nX = pOp->nX;
		pQuestDataEx->pWussieCoords[nCounter].nY = pOp->nY;
		pQuestDataEx->nSpawnedWussies += ACT5Q2_SpawnCagedWussies(pQuestData, pOp, nCounter);
		pQuestDataEx->bWussiesSpawned[nCounter] = 1;
		pQuestDataEx->unk0x10E[nCounter] = 0;
		return;
	}

	int32_t i = 0;
	while (pQuestDataEx->pWussieCoords[i].nX != pOp->nX || pQuestDataEx->pWussieCoords[i].nY != pOp->nY)
	{
		++i;
		if (i >= nCounter)
		{
			pQuestDataEx->pWussieCoords[nCounter].nX = pOp->nX;
			pQuestDataEx->pWussieCoords[nCounter].nY = pOp->nY;
			pQuestDataEx->nSpawnedWussies += ACT5Q2_SpawnCagedWussies(pQuestData, pOp, nCounter);
			pQuestDataEx->bWussiesSpawned[nCounter] = 1;
			pQuestDataEx->unk0x10E[nCounter] = 0;
			return;
		}
	}
}

//D2Game.0x6FCB2CF0
int32_t __fastcall ACT5Q2_CheckWussieCounters(D2GameStrc* pGame, D2UnitStrc* pWussie)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q2_RESCUESOLDIERS);
	if (!pQuestData || !pQuestData->bNotIntro)
	{
		return 0;
	}

	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;

	int32_t nWussieGUID = -1;
	if (pWussie)
	{
		nWussieGUID = pWussie->dwUnitId;
	}

	int32_t* pGUIDs = pQuestDataEx->unk0xB0;
	int32_t nCounter = 0;
	int32_t nIndex = 0;
	while (nWussieGUID != pGUIDs[nCounter])
	{
		++nCounter;
		if (nCounter >= 5)
		{
			++nIndex;

			if (nIndex >= 3)
			{
				return 0;
			}

			pGUIDs += 5;
			nCounter = 0;
		}
	}

	if (nIndex >= 0 && pQuestDataEx->unk0x114[nIndex])
	{
		return 1;
	}

	return 0;
}

//D2Game.0x6FCB2D60
void __fastcall ACT5Q2_UpdateWussieCounters(D2GameStrc* pGame, int32_t nUnused, D2UnitStrc* pWussie)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q2_RESCUESOLDIERS);
	if (!pQuestData || !pQuestData->bNotIntro)
	{
		return;
	}

	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;
	int32_t nUnitGUID = -1;
	if (pWussie)
	{
		nUnitGUID = pWussie->dwUnitId;
	}

	for (int32_t i = 0; i < 3; ++i)
	{
		for (int32_t j = 0; j < 5; ++j)
		{
			if (nUnitGUID == pQuestDataEx->unk0xB0[j + 5 * i])
			{
				++pQuestDataEx->unk0x114[i];
				if (pQuestDataEx->unk0x114[i] >= 5)
				{
					pQuestDataEx->unk0x108[i] = 1;
				}

				return;
			}
		}
	}
}

//D2Game.0x6FCB2DE0
void __fastcall ACT5Q2_UpdateQuestState(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pWussie)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q2_RESCUESOLDIERS);
	if (!pQuestData)
	{
		return;
	}

	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;

	int32_t nWussieGUID = -1;
	if (pWussie)
	{
		nWussieGUID = pWussie->dwUnitId;
	}

	for (int32_t i = 0; i < 3; ++i)
	{
		for (int32_t j = 0; j < 5; ++j)
		{
			if (nWussieGUID == pQuestDataEx->unk0xB0[j + 5 * i])
			{
				if (pQuestDataEx->unk0x10E[i] == 1)
				{
					return;
				}

				D2CoordStrc pCoord = {}; pCoord.nX = pQuestDataEx->pWussieCoords[i].nX + 12;
				pCoord.nY = pQuestDataEx->pWussieCoords[i].nY;
				D2ActiveRoomStrc* pTargetRoom = UNITS_GetRoom(pWussie);
				D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(pPlayer);

				if (!pPlayerRoom)
				{
					return;
				}

				D2ActiveRoomStrc** ppRoomList = nullptr;
				int32_t nNumRooms = 0;
				DUNGEON_GetAdjacentRoomsListFromRoom(pPlayerRoom, &ppRoomList, &nNumRooms);
				if (!nNumRooms)
				{
					return;
				}

				bool bDeadPrisonDoorFound = false;
				for (int32_t k = 0; k < nNumRooms; ++k)
				{
					for (D2UnitStrc* pMonster = ppRoomList[k]->pUnitFirst; pMonster; pMonster = pMonster->pRoomNext)
					{
						if (pMonster->dwUnitType == UNIT_MONSTER && pMonster->dwClassId == MONSTER_PRISONDOOR && pMonster->dwAnimMode == MONMODE_DEAD && UNITS_GetDistanceToOtherUnit(pMonster, pWussie) < 15)
						{
							pTargetRoom = UNITS_GetRoom(pMonster);
							UNITS_GetCoords(pMonster, &pCoord);
							bDeadPrisonDoorFound = true;
							pCoord.nX += 2;
							break;
						}
					}
				}

				if (!bDeadPrisonDoorFound)
				{
					return;
				}

				pQuestDataEx->unk0x10E[i] = 1;

				pTargetRoom = DUNGEON_GetRoomAtPosition(pTargetRoom, pCoord.nX, pCoord.nY);

				D2UnitStrc* pPortal = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_CAINPORTAL, pCoord.nX, pCoord.nY, pQuestData->pGame, pTargetRoom, 1, 1, 0);
				if (!pPortal)
				{
					pPortal = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_CAINPORTAL, pCoord.nX - 2, pCoord.nY, pQuestData->pGame, pTargetRoom, 1, 1, 0);
					if (!pPortal && pTargetRoom)
					{
						D2ActiveRoomStrc* pTemp = pTargetRoom;
						QUESTS_GetFreePosition(pTargetRoom, &pCoord, 2, 0x8000u, &pTemp, 17);

						if (pTemp)
						{
							pPortal = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_CAINPORTAL, pCoord.nX, pCoord.nY, pQuestData->pGame, pTemp, 1, 1, 0);
						}
						else
						{
							pPortal = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_CAINPORTAL, pCoord.nX, pCoord.nY, pQuestData->pGame, pTargetRoom, 1, 1, 0);
						}
					}
				}

				if (pPortal)
				{
					pQuestDataEx->bPortalSpawned[i] = 1;
					pQuestDataEx->nPortalGUIDs[i] = pPortal->dwUnitId;

					EVENT_SetEvent(pQuestData->pGame, pPortal, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 25, 0, 0);
					DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pPortal), 0);
				}

				D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty];

				if (pQuestDataEx->nSpawnedWussies == pQuestDataEx->nFreedWussies + pQuestDataEx->nKilledWussies && pQuestDataEx->bWussiesSpawned[0] && pQuestDataEx->bWussiesSpawned[1] && pQuestDataEx->bWussiesSpawned[2])
				{
					if (pQuestDataEx->nFreedWussies <= 11)
					{
						SUNIT_IterateUnitsOfType(pGame, 0, pPlayer, ACT5Q2_UnitIterate_SetCompletionFlag);
					}
					else
					{
						pQuestData->dwFlags &= 0xFFFFFF00;
						QUESTS_UnitIterate(pQuestData, 3, 0, ACT5Q2_UnitIterate_StatusCyclerEx, 1);

						if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, 0) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDPENDING))
						{
							QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_PRIMARYGOALDONE);
							QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_REWARDPENDING);

							switch (pQuestDataEx->nFreedWussies)
							{
							case 15:
								QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_CUSTOM1);
								break;

							case 14:
								QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_CUSTOM2);
								break;

							default:
								QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_CUSTOM3);
								break;
							}
						}

						SUNIT_IterateUnitsOfType(pGame, 0, pPlayer, ACT5Q2_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
						SUNIT_IterateUnitsOfType(pGame, 0, pPlayer, ACT5Q2_UnitIterate_SetCompletionFlag);
						SUNIT_IterateUnitsOfType(pGame, 0, pPlayer, ACT5Q2_UnitIterate_AttachCompletionSound);
					}
				}
				else
				{
					QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q2, QFLAG_ENTERAREA);
					if (pQuestDataEx->nKilledWussies < 5)
					{
						pQuestData->dwFlags |= 0x00000020;
						QUESTS_UnitIterate(pQuestData, 2, 0, ACT5Q2_UnitIterate_StatusCyclerEx, 1);
					}
					pQuestDataEx->bQualKehkActivated = 0;
				}
				return;
			}
		}
	}
}

//D2Game.0x6FCB3240
int32_t __fastcall ACT5Q2_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q2, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT5Q2_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCB3290
int32_t __fastcall ACT5Q2_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q2, QFLAG_PRIMARYGOALDONE))
	{
		SUNIT_AttachSound(pUnit, 81, pUnit);
	}

	return 0;
}

//D2Game.0x6FCB32D0
int32_t __fastcall ACT5Q2_GetBarbsToBeRescued(D2QuestDataStrc* pQuestData)
{
	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;

	int32_t nBarbs = 0;
	for (int32_t i = 0; i < 3; ++i)
	{
		if (!pQuestDataEx->bWussiesSpawned[i])
		{
			nBarbs += 5;
		}
	}

	if (pQuestDataEx->nSpawnedWussies - pQuestDataEx->nFreedWussies - pQuestDataEx->nKilledWussies + nBarbs < 0)
	{
		return 0;
	}

	return pQuestDataEx->nSpawnedWussies - pQuestDataEx->nFreedWussies - pQuestDataEx->nKilledWussies + nBarbs;
}

//D2Game.0x6FCB3310
void __fastcall ACT5Q2_UpdatePortalMode(D2QuestDataStrc* pQuestData, D2UnitStrc* pPortal)
{
	int32_t nUnitGUID = -1;
	int32_t nAnimMode = OBJMODE_NEUTRAL;
	if (pPortal)
	{
		nUnitGUID = pPortal->dwUnitId;
		nAnimMode = pPortal->dwAnimMode;
	}

	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;

	int32_t nCounter = 0;
	while (nUnitGUID != pQuestDataEx->nPortalGUIDs[nCounter])
	{
		++nCounter;
		if (nCounter >= 3)
		{
			return;
		}
	}

	switch (nAnimMode)
	{
	case OBJMODE_OPERATING:
		UNITS_ChangeAnimMode(pPortal, OBJMODE_OPENED);
		break;

	case OBJMODE_OPENED:
		if (pQuestDataEx->unk0x108[nCounter] == 1)
		{
			++pQuestDataEx->nPortalUpdateInvocations[nCounter];
		}

		if (pQuestDataEx->nPortalUpdateInvocations[nCounter] > 5)
		{
			pQuestDataEx->bChangeToSpecialObjectMode[nCounter] = 1;
		}

		if (pQuestDataEx->bChangeToSpecialObjectMode[nCounter])
		{
			UNITS_ChangeAnimMode(pPortal, OBJMODE_SPECIAL1);
		}
		break;

	case OBJMODE_SPECIAL1:
		UNITS_ChangeAnimMode(pPortal, OBJMODE_SPECIAL2);
		break;
	}

	EVENT_SetEvent(pQuestData->pGame, pPortal, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 25, 0, 0);
}

//D2Game.0x6FCB33B0
int32_t __fastcall ACT5Q2_FindPortal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc** ppPortal)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q2_RESCUESOLDIERS);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act5Quest2Strc* pQuestDataEx = (D2Act5Quest2Strc*)pQuestData->pQuestDataEx;

	int32_t nUnitGUID = -1;
	if (pUnit)
	{
		nUnitGUID = pUnit->dwUnitId;
	}

	int32_t* pGUIDs = pQuestDataEx->unk0xB0;
	int32_t nCounter = 0;
	int32_t nIndex = 0;
	while (nUnitGUID != pGUIDs[nCounter])
	{
		++nCounter;
		if (nCounter >= 5)
		{
			++nIndex;

			if (nIndex >= 3)
			{
				return 0;
			}

			pGUIDs += 5;
			nCounter = 0;
		}
	}

	if (!pQuestDataEx->bPortalSpawned[nIndex])
	{
		return 0;
	}

	D2UnitStrc* pPortal = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nPortalGUIDs[nIndex]);
	if (!pPortal)
	{
		return 0;
	}

	*ppPortal = pPortal;
	return 1;
}

//D2Game.0x6FCB3440
void __fastcall ACT5Q2_OnWussieActivated(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q2_RESCUESOLDIERS);

	if (!pQuestData || !pQuestData->bNotIntro || pQuestData->fLastState == 5)
	{
		return;
	}

	if (((D2Act5Quest2Strc*)pQuestData->pQuestDataEx)->nKilledWussies >= 5)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 5, 0, ACT5Q2_UnitIterate_StatusCyclerEx, 1);
}

//D2Game.0x6FCB3490
int32_t __fastcall ACT5Q2_IsPrisonDoorDead(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pWussie)
{
	D2UnitStrc* pUnit = pPlayer;
	if (!pPlayer)
	{
		if (pGame->nGameType != 3 || !pGame->pClientList)
		{
			pUnit = pWussie;
		}
		else
		{
			pUnit = CLIENTS_GetPlayerFromClient(pGame->pClientList, 0);
			if (!pUnit)
			{
				pUnit = pWussie;
			}
		}
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return 0;
	}

	D2ActiveRoomStrc** ppRoomList = nullptr;
	int32_t nNumRooms = 0;
	DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

	for (int32_t i = 0; i < nNumRooms; ++i)
	{
		for (D2UnitStrc* j = ppRoomList[i]->pUnitFirst; j; j = j->pRoomNext)
		{
			if (j->dwUnitType == UNIT_MONSTER && j->dwClassId == MONSTER_PRISONDOOR && j->dwAnimMode == MONMODE_DEAD)
			{
				return 1;
			}
		}
	}

	return 0;
}
