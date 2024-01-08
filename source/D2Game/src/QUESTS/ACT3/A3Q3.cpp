#include "QUESTS/ACT3/A3Q3.h"

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
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitNpc.h"


//D2Game.0x6FD38CE0
D2NPCMessageTableStrc gpAct3Q3NpcMessages[] =
{
	{
		{
			{ MONSTER_ALKOR, 0x23B, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_ALKOR, 0x23C, 0, 2 },
			{ MONSTER_ORMUS, 0x23D, 0, 2 },
			{ MONSTER_MESHIF2, 0x23E, 0, 2 },
			{ MONSTER_ASHEARA, 0x23F, 0, 2 },
			{ MONSTER_HRATLI, 0x240, 0, 2 },
			{ MONSTER_CAIN3, 0x241, 0, 2 },
			{ MONSTER_NATALYA, 0x242, 0, 2 },
		},
		7
	},
	{
		{
			{ MONSTER_ALKOR, 0x243, 0, 2 },
			{ MONSTER_ORMUS, 0x244, 0, 2 },
			{ MONSTER_MESHIF2, 0x245, 0, 2 },
			{ MONSTER_ASHEARA, 0x246, 0, 2 },
			{ MONSTER_HRATLI, 0x247, 0, 2 },
			{ MONSTER_CAIN3, 0x248, 0, 2 },
			{ MONSTER_NATALYA, 0x249, 0, 2 },
		},
		7
	},
	{
		{
			{ MONSTER_ALKOR, 0x24A, 0, 2 },
			{ MONSTER_ORMUS, 0x24B, 0, 1 },
			{ MONSTER_MESHIF2, 0x24C, 0, 2 },
			{ MONSTER_HRATLI, 0x24E, 0, 2 },
			{ MONSTER_CAIN3, 0x24F, 0, 2 },
			{ MONSTER_NATALYA, 0x250, 0, 2 },
		},
		6
	},
	{
		{
			{ MONSTER_MESHIF2, 0x24C, 0, 2 },
			{ MONSTER_ALKOR, 0x24A, 0, 2 },
			{ MONSTER_CAIN3, 0x24F, 0, 2 },
			{ MONSTER_NATALYA, 0x250, 0, 2 },
		},
		4
	},
	{
		{
			{ MONSTER_MESHIF2, 0x24C, 0, 2 },
			{ MONSTER_ASHEARA, 0x24D, 0, 1 },
			{ MONSTER_ALKOR, 0x24A, 0, 2 },
			{ MONSTER_CAIN3, 0x24F, 0, 2 },
			{ MONSTER_NATALYA, 0x250, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_ORMUS, 0x251, 0, 1 },
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


//D2Game.0x6FCA8DC0
bool __fastcall ACT3Q3_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A2COMPLETED, QFLAG_REWARDGRANTED))
	{
		*pStatus = 0;
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED))
	{
		*pStatus = 2 * (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q3, QFLAG_PRIMARYGOALDONE) != 0) + 11;
		return true;
	}

	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuest->pQuestDataEx;

	*pStatus = 0;
	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 33g'))
	{
		*pStatus = 4;
		return true;
	}

	if (pQuestDataEx->bPlayerWithGidbinnDropped)
	{
		*pStatus = (pQuest->pGame->nGameType == 3) + 7;
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2))
	{
		if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4))
		{
			*pStatus = 6;
		}
		if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3))
		{
			*pStatus = 5;
		}
	}
	else if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q3, QFLAG_ENTERAREA))
	{
		*pStatus = (pQuestDataEx->bGidbinnDropped == 1) + 2;
	}
	else if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q3, QFLAG_LEAVETOWN))
	{
		*pStatus = 2;
	}
	else if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q3, QFLAG_STARTED))
	{
		*pStatus = 1;
	}

	return true;
}

//D2Game.0x6FCA8F10
bool __fastcall ACT3Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED))
	{
		return false;
	}

	if (nNpcId == MONSTER_HRATLI)
	{
		if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 33g') || pQuest->fState != 1)
		{
			return false;
		}

		return true;
	}
	else if (nNpcId == MONSTER_ORMUS)
	{
		if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 33g'))
		{
			return true;
		}

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2))
		{
			return false;
		}

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4))
		{
			return true;
		}
	}
	else if (nNpcId == MONSTER_ASHEARA)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2))
		{
			return false;
		}

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3))
		{
			return true;
		}
	}

	return false;
}

//D2Game.0x6FCA8FC0
void __fastcall ACT3Q3_UnitIterate_SetGidbinnBroughtBackQuestState(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTS_GetQuestData(pGame, QUEST_A3Q3_GIDBINN))
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3))
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (DRLG_GetActNoFromLevelId(nLevelId) == ACT_III)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2);
	}
}

//D2Game.0x6FCA9050
void __fastcall ACT3Q3_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->pNPCMessages = gpAct3Q3NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A3Q3;
	pQuestData->pfActiveFilter = ACT3Q3_ActiveFilterCallback;
	pQuestData->nInitNo = 4;
	pQuestData->pfStatusFilter = ACT3Q3_StatusFilterCallback;
	pQuestData->pfSeqFilter = ACT3Q3_SeqCallback;
	pQuestData->nSeqId = 15;
	pQuestData->pfCallback[QUESTEVENT_ITEMPICKEDUP] = ACT3Q3_Callback04_ItemPickedUp;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT3Q3_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT3Q3_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT3Q3_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT3Q3_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT3Q3_Callback13_PlayerStartedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERJOINEDGAME] = ACT3Q3_Callback14_PlayerJoinedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM] = ACT3Q3_Callback09_PlayerDroppedWithQuestItem;

	D2Act3Quest3Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act3Quest3Strc);
	memset(pQuestDataEx, 0x00, sizeof(D2Act3Quest3Strc));
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestDataEx->nAltarObjectMode = 0;
}

//D2Game.0x6FCA9130
void __fastcall ACT3Q3_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;
	if (nNpcId == MONSTER_HRATLI)
	{
		if (pQuestDataEx->bHratliActivated == 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 2, 0, ACT3Q3_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->bHratliActivated = 0;
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q3_UnitIterate_UpdateQuestStateFlags);
		}
	}
	else if (nNpcId == MONSTER_ORMUS)
	{
		if (pQuestDataEx->bGidbinnBroughtToOrmus)
		{
			pQuestDataEx->bGidbinnBroughtToOrmus = 0;
			pQuestDataEx->bAltarCanBeActivated = 1;
		}
	}
}

//D2Game.0x6FCA91A0
int32_t __fastcall ACT3Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q3_GIDBINN);
	if (!pQuestData)
	{
		return 0;
	}

	if (ITEMS_FindQuestItem(pGame, pUnit, ' 33g'))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM1);
		return 0;
	}

	if (((D2Act3Quest3Strc*)pQuestData->pQuestDataEx)->bGidbinnDropped)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_ENTERAREA);

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_LEAVETOWN))
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_STARTED);
		}
	}
	else
	{
		if (pQuestData->fState == 2)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_LEAVETOWN);
		}
	}

	return 0;
}

//D2Game.0x6FCA9260
int32_t __fastcall ACT3Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A3Q3_GIDBINN);
	}

	return 0;
}

//D2Game.0x6FCA92D0
void __fastcall ACT3Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED) == 1)
	{
		if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
		}
		return;
	}

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 33g') && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM1) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2))
	{
		if (nNpcId == MONSTER_ASHEARA)
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3) == 0)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_ASHEARA, 5);
				return;
			}
		}
		else
		{
			if (nNpcId == MONSTER_ORMUS && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4) == 0)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_ORMUS, 6);
				return;
			}

			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3) == 0)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 5);
				return;
			}
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4) == 0)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 6);
		}
	}
	else
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM1))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
			return;
		}

		if (pQuestData->fState < 4 && !ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 33g'))
		{
			const int32_t nIndex = nIndices[pQuestData->fState];
			if (nIndex != -1 && nIndex < 8)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
			}
		}
	}
}

//D2Game.0x6FCA9490
void __fastcall ACT3Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel == LEVEL_FLAYERJUNGLE && pQuestData->bNotIntro && !pQuestData->fState)
	{
		pQuestData->fState = 1;
	}

	if (pQuestArg->nOldLevel != LEVEL_KURASTDOCKTOWN)
	{
		return;
	}

	QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);

	if (pQuestData->fState == 2)
	{
		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_COMPLETEDBEFORE) != 1)
		{
			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q3_UnitIterate_UpdateQuestStateFlags);
		}
	}
	else if (pQuestData->fState == 5)
	{
		pQuestData->bActive = 0;
	}
}

//D2Game.0x6FCA9540
void __fastcall ACT3Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED) == 1)
	{
		return;
	}

	if (pQuestArg->nNPCNo == MONSTER_ORMUS)
	{
		if (pQuestArg->nMessageIndex == 587)
		{
			if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 33g') && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM1) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4))
			{
				((D2Act3Quest3Strc*)pQuestData->pQuestDataEx)->bGidbinnBroughtToOrmus = 1;
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 33g');
				--((D2Act3Quest3Strc*)pQuestData->pQuestDataEx)->nGidbinnsInGame;
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2);

				const int16_t nPartyId = SUNIT_GetPartyId(pQuestArg->pPlayer);
				if (nPartyId != -1)
				{
					PARTY_IteratePartyMembers(pQuestData->pGame, nPartyId, ACT3Q3_UnitIterate_SetGidbinnBroughtBackQuestState, 0);
				}

				SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q3_UnitIterate_SetCompletionFlag);
			}

			if (pQuestData->bNotIntro)
			{
				QUESTS_SetGlobalState(pQuestArg->pGame, QUESTSTATEFLAG_A3Q3, QFLAG_PRIMARYGOALDONE);
			}

			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 13, 0, ACT3Q3_UnitIterate_StatusCyclerEx, 0);

			if (!pQuestData->bNotIntro || pQuestData->fState == 5)
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
		else if (pQuestArg->nMessageIndex == 593 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4))
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4);

			int32_t nItemLevel = 21;
			if (pQuestArg->pGame->nDifficulty == DIFFMODE_NIGHTMARE)
			{
				nItemLevel = 35;
			}
			else if (pQuestArg->pGame->nDifficulty == DIFFMODE_HELL)
			{
				nItemLevel = 75;
			}

			QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, ' nir', nItemLevel, ITEMQUAL_RARE, 1);

			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3))
			{
				return;
			}

			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_PRIMARYGOALDONE);
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED);

			if (pQuestArg->pPlayer)
			{
				QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
			}
			else
			{
				QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, -1);
			}
		}
	}
	else if (pQuestArg->nNPCNo == MONSTER_HRATLI)
	{
		if (pQuestArg->nMessageIndex == 571)
		{
			QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			((D2Act3Quest3Strc*)pQuestData->pQuestDataEx)->bHratliActivated = 1;
		}
	}
	else if (pQuestArg->nNPCNo == MONSTER_ASHEARA)
	{
		if (pQuestArg->nMessageIndex != 589 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3))
		{
			return;
		}

		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3);
		D2GAME_NPC_AssignMercenary_6FCCB520(pQuestData->pGame, pQuestArg->pPlayer, MONSTER_ASHEARA);

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4))
		{
			return;
		}

		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_PRIMARYGOALDONE);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED);

		if (pQuestArg->pPlayer)
		{
			QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
		}
		else
		{
			QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, -1);
		}
	}
}

//D2Game.0x6FCA9820
int32_t __fastcall ACT3Q3_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED)
		&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2)
		&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4)
		&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3)
		&& !ITEMS_FindQuestItem(pGame, pUnit, ' 33g'))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_COMPLETEDNOW);
	}

	return 0;
}

//D2Game.0x6FCA98A0
void __fastcall ACT3Q3_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_DebugOutput(pQuestData->pGame, "ginbinn acquired", __FILE__, __LINE__);

	int32_t nItemId = -1;
	if (pQuestArg->pTarget)
	{
		nItemId = pQuestArg->pTarget->dwClassId;
	}

	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);
	if (pQuestData->bNotIntro && pItemsTxtRecord->dwCode == ' 33g')
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 4, 0, ACT3Q3_UnitIterate_StatusCyclerEx, 1);

		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM5))
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM5);
			SUNIT_AttachSound(pQuestArg->pPlayer, 65, pQuestArg->pPlayer);
		}

		QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 3, 0, ACT3Q3_UnitIterate_StatusCyclerEx, 1);
	}

	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q3_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FCA9990
bool __fastcall ACT3Q3_SeqCallback(D2QuestDataStrc* pQuestData)
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

//D2Game.0x6FCA9A00
void __fastcall ACT3Q3_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 33g'))
	{
		++pQuestDataEx->nGidbinnsInGame;
	}

	if (pQuestDataEx->bPlayerWithGidbinnDropped && pQuestDataEx->nGidbinnsInGame == 1)
	{
		pQuestDataEx->bPlayerWithGidbinnDropped = 0;
	}
}

//D2Game.0x6FCA9A30
void __fastcall ACT3Q3_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;

	if (ITEMS_GetBaseCode(pQuestArg->pTarget) != ' 33g')
	{
		return;
	}

	--pQuestDataEx->nGidbinnsInGame;
	if (!pQuestDataEx->nGidbinnsInGame && pQuestDataEx->bGidbinnDropped && pQuestData->bNotIntro && pQuestData->fState < 5)
	{
		pQuestDataEx->bPlayerWithGidbinnDropped = 1;
	}
}

//D2Game.0x6FCA9A70
void __fastcall ACT3Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 33g'))
	{
		++pQuestDataEx->nGidbinnsInGame;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED))
	{
		pQuestDataEx->nAltarObjectMode = OBJMODE_OPENED;
	}
	else if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 33g'))
	{
		pQuestData->fLastState = 4;
		pQuestData->fState = 4;
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q3_UnitIterate_UpdateQuestStateFlags);
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM2) == 1)
	{
		pQuestDataEx->nAltarObjectMode = OBJMODE_OPENED;
		pQuestData->fState = 5;

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3))
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4))
			{
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED);
				pQuestData->bNotIntro = 0;
			}
			else
			{
				pQuestData->fLastState = 6;
			}
		}
		else
		{
			pQuestData->fLastState = 5;
		}
	}
	else
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM1) == 1)
		{
			pQuestData->fState = 3;
			pQuestData->fLastState = 2;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_ENTERAREA) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_LEAVETOWN) == 1)
		{
			pQuestData->fState = 2;
			pQuestData->fLastState = 2;
		}
		else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_STARTED) == 1)
		{
			pQuestData->fState = 3;
			pQuestData->fLastState = 1;
		}
	}
}

//D2Game.0x6FCA9BA0
void __fastcall OBJECTS_InitFunction25_Gidbinn(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q3_GIDBINN);
	if (!pQuestData)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
		return;
	}

	if (!pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
		return;
	}

	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bGidbinnObjectInitialized = 1;
	pQuestDataEx->nGidbinnX = pOp->nX;
	pQuestDataEx->nGidbinnY = pOp->nY;

	if (!pQuestDataEx->bGidbinnObjectActivated || pQuestDataEx->bBossHasSpawned)
	{
		return;
	}

	pQuestDataEx->bBossIsSpawning = 1;
	D2UnitStrc* pFetishBoss = sub_6FC6E8D0(pOp->pGame, pOp->pRoom, 0, MONSTER_FETISH11, 1, 0, 0, 1);
	if (pFetishBoss)
	{
		pQuestDataEx->bGidbinnObjectActivated = 0;
		pQuestDataEx->bBossHasSpawned = 1;
		pQuestDataEx->nFetishBossGUID = pFetishBoss->dwUnitId;
	}
	pQuestDataEx->bBossIsSpawning = 0;
}

//D2Game.0x6FCA9C30
int32_t __fastcall OBJECTS_OperateFunction31_GidbinnDecoy(D2ObjOperateFnStrc* pOp, int32_t nOperate)
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

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_REWARDGRANTED)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4))
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q3_GIDBINN);
	if (!pQuestData || !pQuestData->bNotIntro)
	{
		return 1;
	}

	if (!pQuestData->fState)
	{
		pQuestData->fState = 1;
	}

	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;

	UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

	D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
	EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, pOp->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);

	pQuestDataEx->bGidbinnObjectActivated = 1;
	if (!pQuestDataEx->bSpawnTimerStarted)
	{
		pQuestDataEx->bSpawnTimerStarted = 1;
		QUESTS_CreateTimer(pQuestData, ACT3Q3_SpawnGidbinnBoss, 7);
	}

	return 1;
}

//D2Game.0x6FCA9D50
bool __fastcall ACT3Q3_SpawnGidbinnBoss(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act3Quest3Strc* pQuestDataEx = nullptr;
	D2UnitStrc* pFetishBoss = nullptr;
	D2ActiveRoomStrc** ppRoomList = nullptr;
	D2ActiveRoomStrc* pRoom = nullptr;
	D2UnitStrc* pUnit = nullptr;
	int32_t nNumRooms = 0;
	int32_t nCounter = 0;

	pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bSpawnTimerStarted = 0;

	if (pQuestDataEx->bGidbinnObjectInitialized == 1 && pQuestDataEx->bGidbinnObjectActivated && !pQuestDataEx->bBossHasSpawned && pGame->pAct[ACT_III])
	{
		pRoom = DUNGEON_FindRoomBySubtileCoordinates(pGame->pAct[ACT_III], pQuestDataEx->nGidbinnX, pQuestDataEx->nGidbinnY);
		if (pRoom)
		{
			DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

			if (nNumRooms)
			{
				while (1)
				{
					pUnit = ppRoomList[nCounter]->pUnitFirst;

					while (pUnit && pUnit->dwUnitType != UNIT_PLAYER)
					{
						pUnit = pUnit->pRoomNext;
					}

					if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
					{
						break;
					}

					++nCounter;
					if (nCounter >= nNumRooms)
					{
						return true;
					}
				}

				pQuestDataEx->bBossIsSpawning = 1;
				pFetishBoss = sub_6FC6E8D0(pGame, pRoom, 0, MONSTER_FETISH11, 1, 0, 0, 1);
				if (pFetishBoss)
				{
					pQuestDataEx->bGidbinnObjectActivated = 0;
					pQuestDataEx->bBossHasSpawned = 1;
					pQuestDataEx->nFetishBossGUID = pFetishBoss->dwUnitId;
				}
				pQuestDataEx->bBossIsSpawning = 0;
				pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT3Q3_Callback08_MonsterKilled;
			}
		}
	}

	return true;
}

//D2Game.0x6FCA9E40
void __fastcall ACT3Q3_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_DebugOutput(pQuestData->pGame, "killed boss for quest", __FILE__, __LINE__);

	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;

	if (!pQuestDataEx->bBossHasSpawned || !pQuestData->bNotIntro || (!MONSTERUNIQUE_CheckMonTypeFlag(pQuestArg->pTarget, MONTYPEFLAG_UNIQUE | MONTYPEFLAG_CHAMPION | MONTYPEFLAG_SUPERUNIQUE) && !MONSTERS_IsBoss(nullptr, pQuestArg->pTarget)))
	{
		return;
	}

	pQuestArg->pTarget->dwDropItemCode = ' 33g';
	int32_t nItemLevel = 0;
	if (D2GAME_DropItemAtUnit_6FC4FEC0(pQuestData->pGame, pQuestArg->pTarget, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0))
	{
		pQuestDataEx->bGidbinnDropped = 1;
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT3Q3_UnitIterate_UpdateQuestStateFlags);
		pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = nullptr;
	}
	else
	{
		pQuestDataEx->bBossHasSpawned = 0;
	}
}

//D2Game.0x6FCA9EF0
int32_t __fastcall ACT3Q3_GetAltarCoordinates(D2GameStrc* pGame, D2CoordStrc* pCoord)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q3_GIDBINN);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;
	if (!pQuestDataEx->bAltarCanBeActivated)
	{
		return 0;
	}

	pCoord->nX = pQuestDataEx->nAltarX;
	pCoord->nY = pQuestDataEx->nAltarY;
	return 1;
}

//D2Game.0x6FCA9F30
void __fastcall ACT3Q3_SetAltarMode(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q3_GIDBINN);
	if (!pQuestData)
	{
		return;
	}

	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bAltarCanBeActivated = 0;

	D2UnitStrc* pAltar = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nAltarGUID);
	if (!pAltar)
	{
		return;
	}

	UNITS_ChangeAnimMode(pAltar, OBJMODE_OPERATING);
	D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pAltar->dwClassId);
	EVENT_SetEvent(pGame, pAltar, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pGame->dwGameFrame + 1, 0, 0);
	pQuestDataEx->nAltarObjectMode = OBJMODE_OPENED;
}

//D2Game.0x6FCA9FA0
void __fastcall OBJECTS_InitFunction39_GidbinnAltar(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q3_GIDBINN);
	if (!pQuestData)
	{
		return;
	}

	D2Act3Quest3Strc* pQuestDataEx = (D2Act3Quest3Strc*)pQuestData->pQuestDataEx;
	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}

	pQuestDataEx->nAltarGUID = nUnitId;
	pQuestDataEx->nAltarX = pOp->nX;
	pQuestDataEx->nAltarY = pOp->nY;

	UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAltarObjectMode);
}
