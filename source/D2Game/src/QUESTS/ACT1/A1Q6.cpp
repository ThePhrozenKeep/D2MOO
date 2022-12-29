#include "QUESTS/ACT1/A1Q6.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2QuestRecord.h>

#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "SKILLS/Skills.h"


//D2Game.0x6FD33B68
D2NPCMessageTableStrc gpAct1Q6NpcMessages[] =
{
	{
		{
			{ MONSTER_CAIN5, 166, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_AKARA, 168, 0, 2 },
			{ MONSTER_KASHYA, 172, 0, 2 },
			{ MONSTER_CHARSI, 169, 0, 2 },
			{ MONSTER_CAIN5, 167, 0, 2 },
			{ MONSTER_GHEED, 170, 0, 2 },
			{ MONSTER_WARRIV1, 171, 0, 2 },
		},
		6
	},
	{
		{
			{ MONSTER_KASHYA, 178, 0, 2 },
			{ MONSTER_WARRIV1, 177, 0, 2 },
			{ MONSTER_GHEED, 175, 0, 2 },
			{ MONSTER_CAIN5, 173, 0, 2 },
			{ MONSTER_CHARSI, 176, 0, 2 },
			{ MONSTER_AKARA, 174, 0, 2 },
		},
		6
	},
	{
		{
			{ MONSTER_KASHYA, 181, 0, 0 },
			{ MONSTER_CAIN5, 184,0, 0 },
			{ MONSTER_CHARSI, 180, 0, 2 },
			{ MONSTER_GHEED, 182, 0, 2 },
			{ MONSTER_WARRIV1, 183, 0, 0 },
			{ MONSTER_AKARA, 179, 0, 0 },
		},
		6
	},
	{
		{
			{ MONSTER_KASHYA, 181, 0, 2 },
			{ MONSTER_CAIN5, 184,0, 2 },
			{ MONSTER_CHARSI, 180, 0, 2 },
			{ MONSTER_GHEED, 182, 0, 2 },
			{ MONSTER_WARRIV1, 183, 0, 2 },
			{ MONSTER_AKARA, 179, 0, 2 },
		},
		6
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FC9DFF0
void __fastcall ACT1Q6_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_I)
	{
		return;
	}

	D2Act1Quest6Strc* pQuestDataEx = (D2Act1Quest6Strc*)QUESTS_GetQuestData(pGame, QUEST_A1Q6_ANDARIEL)->pQuestDataEx;
	int32_t nUnitId = -1;
	if (pUnit)
	{
		nUnitId = pUnit->dwUnitId;
	}

	QUESTS_AddPlayerGUID(&pQuestDataEx->pGUID1, nUnitId);
	QUESTS_AddPlayerGUID(&pQuestDataEx->pGUID2, nUnitId);
	QUESTS_AddPlayerGUID(&pQuestDataEx->pGUID3, nUnitId);

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING);

	CLIENTS_UpdateCharacterProgression(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 1, pGame->nDifficulty);
}

//D2Game.0x6FC9E0E0
bool __fastcall ACT1Q6_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	int32_t nUnitId = -1;
	if (pPlayer)
	{
		nUnitId = pPlayer->dwUnitId;
	}

	D2Act1Quest6Strc* pQuestDataEx = (D2Act1Quest6Strc*)pQuest->pQuestDataEx;
	if (nNpcId == MONSTER_CAIN5)
	{
		if (QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->pGUID1, nUnitId))
		{
			return true;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) != 1 && pQuest->fState == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING))
		{
			return true;
		}
	}
	else if (nNpcId == MONSTER_WARRIV1)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) != 1)
		{
			return QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING) == 1;
		}
	}
	else if (nNpcId == MONSTER_AKARA)
	{
		return QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->pGUID2, nUnitId) != 0;
	}
	if (nNpcId == MONSTER_KASHYA)
	{
		return QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->pGUID3, nUnitId) != 0;
	}

	return false;
}

//D2Game.0x6FC9E1B0
void __fastcall ACT1Q6_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));

	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT1Q6_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT1Q6_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT1Q6_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT1Q6_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT1Q6_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT1Q6_Callback13_PlayerStartedGame;
	pQuestData->pfSeqFilter = ACT1Q6_SeqCallback;

	pQuestData->nQuest = QUESTSTATEFLAG_A1Q6;
	pQuestData->pNPCMessages = gpAct1Q6NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fLastState = 0;
	pQuestData->fState = 0;

	D2Act1Quest6Strc* pQuestDataEx = (D2Act1Quest6Strc*)FOG_AllocPool(pQuestData->pGame->pMemoryPool, sizeof(D2Act1Quest6Strc), __FILE__, __LINE__, 0);

	pQuestData->nInitNo = 4;
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->pfStatusFilter = 0;
	pQuestData->nSeqId = 37;
	pQuestData->pfActiveFilter = ACT1Q6_ActiveFilterCallback;

	pQuestDataEx->nAndarielGUID = 0;
	pQuestDataEx->unk0x190 = 0;
	pQuestDataEx->nTimerInvocations = 0;
	pQuestDataEx->bAndarielKilled = 0;
	pQuestDataEx->bCainActivated = 0;

	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->pGUID1);
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->pGUID2);
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->pGUID3);
}

//D2Game.0x6FC9E2B0
void __fastcall ACT1Q6_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_CAIN5)
	{
		return;
	}

	D2Act1Quest6Strc* pQuestDataEx = (D2Act1Quest6Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bCainActivated != 1)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q6_UnitIterate_StatusCyclerEx, 1);
	pQuestDataEx->bCainActivated = 0;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
}

//D2Game.0x6FC9E300
int32_t __fastcall ACT1Q6_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A1Q6_ANDARIEL);
	}

	return 0;
}

//D2Game.0x6FC9E370
void __fastcall ACT1Q6_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	int32_t nUnitId = -1;
	if (pQuestArg->pPlayer)
	{
		nUnitId = pQuestArg->pPlayer->dwUnitId;
	}

	D2Act1Quest6Strc* pQuestDataEx = (D2Act1Quest6Strc*)pQuestData->pQuestDataEx;

	switch (pQuestArg->nNPCNo)
	{
	case MONSTER_CAIN5:
	{
		if (pQuestArg->nMessageIndex == 166)
		{
			QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			pQuestDataEx->bCainActivated = 1;
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q6_UnitIterate_UpdateQuestStateFlags);
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		}
		else if (pQuestArg->nMessageIndex == 184)
		{
			QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pGUID1, nUnitId);
		}
		return;
	}
	case MONSTER_WARRIV1:
	{
		if (pQuestArg->nMessageIndex != 183)
		{
			return;
		}

		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);

		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING))
		{
			return;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE))
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 13, 0, ACT1Q6_UnitIterate_StatusCyclerEx, 0);
			QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
			QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE);
		}

		QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED);

		QUESTS_AddPlayerGUID(&pQuestData->pGUID, nUnitId);
		QUESTS_UpdatePlayerFlags(pQuestData->pGame, pQuestArg->pPlayer);
		return;
	}
	case MONSTER_AKARA:
	{
		if (pQuestArg->nMessageIndex == 179)
		{
			QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pGUID2, nUnitId);
		}
		return;
	}
	case MONSTER_KASHYA:
	{
		if (pQuestArg->nMessageIndex == 181)
		{
			QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pGUID3, nUnitId);
		}
		return;
	}
	}
}

//D2Game.0x6FC9E510
int32_t __fastcall ACT1Q6_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q6_ANDARIEL);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		if (pQuestData->fLastState == 1)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_LEAVETOWN);
		}
		else
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_ENTERAREA);
		}
	}

	return 0;
}

//D2Game.0x6FC9E5A0
void __fastcall ACT1Q6_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
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

	D2Act1Quest6Strc* pQuestDataEx = (D2Act1Quest6Strc*)pQuestData->pQuestDataEx;

	int32_t nUnitId = -1;
	if (pQuestArg->pPlayer)
	{
		nUnitId = pQuestArg->pPlayer->dwUnitId;
	}

	if (nNpcId == MONSTER_AKARA)
	{
		if (QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->pGUID2, nUnitId))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_AKARA, 3);
			return;
		}
	}
	else if (nNpcId == MONSTER_KASHYA)
	{
		if (QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->pGUID3, nUnitId))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_KASHYA, 3);
			return;
		}
	}
	else if (nNpcId == MONSTER_CAIN5)
	{
		if (QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->pGUID1, nUnitId))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN5, 3);
			return;
		}
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING))
	{
		if (nNpcId == MONSTER_CAIN5 || nNpcId == MONSTER_AKARA || nNpcId == MONSTER_KASHYA)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
		}
		else
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
		}
	}
	else if (QUESTS_CheckPlayerGUID(pQuestData, nUnitId) == 1)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
	}
	else if (pQuestData->fState != 1 || nNpcId != MONSTER_CAIN5 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED))
	{
		if (!pQuestData->fState || (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE)))
		{
			return;
		}

		if (pQuestData->fState >= 4 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE))
		{
			return;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) == 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE) == 1)
		{
			return;
		}

		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < 6)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
	else
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN5, 0);
	}
}

//D2Game.0x6FC9E7A0
void __fastcall ACT1Q6_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel >= LEVEL_CATACOMBSLEV1 && pQuestArg->nNewLevel <= LEVEL_CATACOMBSLEV4 && pQuestData->bNotIntro)
	{
		if (pQuestData->fState < 3)
		{
			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		}

		if (pQuestArg->nNewLevel == LEVEL_CATACOMBSLEV4)
		{
			if (pQuestData->fLastState < 2)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 2, 0, ACT1Q6_UnitIterate_StatusCyclerEx, 1);
				SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q6_UnitIterate_UpdateQuestStateFlags);
				QUESTS_DebugOutput(pQuestArg->pGame, "started quest by jumping to level", __FILE__, __LINE__);
				return;
			}
		}
		else
		{
			if (!pQuestData->fLastState)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q6_UnitIterate_StatusCyclerEx, 0);
				SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q6_UnitIterate_UpdateQuestStateFlags);
				QUESTS_DebugOutput(pQuestArg->pGame, "started quest by jumping to level", __FILE__, __LINE__);
				return;
			}
		}

		if (pQuestData->fState >= 3)
		{
			QUESTS_DebugOutput(pQuestArg->pGame, "started quest by jumping to level", __FILE__, __LINE__);
			return;
		}

		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q6_UnitIterate_UpdateQuestStateFlags);
		QUESTS_DebugOutput(pQuestArg->pGame, "started quest by jumping to level", __FILE__, __LINE__);
		return;
	}

	if (pQuestData->fState == 4 && pQuestArg->nNewLevel == LEVEL_LUTGHOLEIN)
	{
		QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
		return;
	}

	if (pQuestArg->nOldLevel != LEVEL_ROGUEENCAMPMENT)
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
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q6_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FC9E900
void __fastcall ACT1Q6_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const uint32_t dwChippedGemCodes[] =
	{
		' vcg', ' rcg', ' bcg', ' ycg', ' gcg', ' wcg', ' cks'
	};

	static const uint32_t dwNormalGemCodes[] =
	{
		' vsg', ' rsg', ' bsg', ' ysg', ' gsg', ' wsg', ' uks'
	};

	D2Act1Quest6Strc* pQuestDataEx = (D2Act1Quest6Strc*)pQuestData->pQuestDataEx;

	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;

	if (pQuestData->bNotIntro)
	{
		if (pQuestArg->pPlayer)
		{
			D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING))
			{
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE);
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING);

				CLIENTS_UpdateCharacterProgression(SUNIT_GetClientFromPlayer(pQuestArg->pPlayer, __FILE__, __LINE__), 1, pQuestData->pGame->nDifficulty);

				D2SeedStrc* pSeed = QUESTS_GetGlobalSeed(pQuestData->pGame);
				int32_t nItemLevel = 0;
				for (int32_t i = 0; i < 2; ++i)
				{
					pQuestArg->pTarget->dwDropItemCode = dwChippedGemCodes[ITEMS_RollRandomNumber(pSeed) % ARRAY_SIZE(dwChippedGemCodes)];
					D2GAME_DropItemAtUnit_6FC4FEC0(pQuestData->pGame, pQuestArg->pTarget, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0);
				}

				pQuestArg->pTarget->dwDropItemCode = dwNormalGemCodes[ITEMS_RollRandomNumber(pSeed) % ARRAY_SIZE(dwNormalGemCodes)];
				D2GAME_DropItemAtUnit_6FC4FEC0(pQuestData->pGame, pQuestArg->pTarget, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0);

				pQuestArg->pTarget->dwDropItemCode = 0;
			}
		}

		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q6_UnitIterate_SetRewardPending);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q6_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q6_UnitIterate_SetCompletionFlag);
	}

	pQuestDataEx->nAndarielGUID = pQuestArg->pTarget ? pQuestArg->pTarget->dwUnitId : -1;
	pQuestDataEx->nTimerInvocations = 1;

	if (pQuestData->bNotIntro)
	{
		QUESTS_CreateTimer(pQuestData, ACT1Q6_Timer_StatusCycler, 1);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q6_UnitIterate_AttachCompletionSound);
	}

	pQuestDataEx->bAndarielKilled = 1;
	QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT1Q6_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = nullptr;
}

//D2Game.0x6FC9EB50
int32_t __fastcall ACT1Q6_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING) && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE))
	{
		SUNIT_AttachSound(pUnit, 33, pUnit);
	}

	return 0;
}

//D2Game.0x6FC9EBA0
void __fastcall ACT1Q6_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pArgs)
{
	QUESTS_RemovePlayerGUID(pQuestData, pArgs);
	D2Act1Quest6Strc* pQuestDataEx = (D2Act1Quest6Strc*)pQuestData->pQuestDataEx;

	int32_t nUnitId = -1;
	if (pArgs->pPlayer)
	{
		nUnitId = pArgs->pPlayer->dwUnitId;
	}

	QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pGUID1, nUnitId);
	QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pGUID2, nUnitId);
	QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pGUID3, nUnitId);
}

//D2Game.0x6FC9EBF0
int32_t __fastcall ACT1Q6_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_COMPLETEDBEFORE) == 1)
	{
		return 0;
	}

	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_CATACOMBSLEV4)
	{
		return 0;
	}

	D2Act1Quest6Strc* pQuestDataEx = (D2Act1Quest6Strc*)QUESTS_GetQuestData(pGame, QUEST_A1Q6_ANDARIEL)->pQuestDataEx;
	int32_t nUnitId = -1;
	if (pUnit)
	{
		nUnitId = pUnit->dwUnitId;
	}

	QUESTS_AddPlayerGUID(&pQuestDataEx->pGUID1, nUnitId);
	QUESTS_AddPlayerGUID(&pQuestDataEx->pGUID2, nUnitId);
	QUESTS_AddPlayerGUID(&pQuestDataEx->pGUID3, nUnitId);

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING);

	CLIENTS_UpdateCharacterProgression(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 1, pGame->nDifficulty);
	return 0;
}

//D2Game.0x6FC9ECD0
int32_t __fastcall ACT1Q6_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDPENDING) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A1Q6_ANDARIEL, 0);
	}

	return 0;
}

//D2Game.0x6FC9ED30
int32_t __fastcall ACT1Q6_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT1Q6_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FC9ED80
bool __fastcall ACT1Q6_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuest)
{
	D2Act1Quest6Strc* pQuestDataEx = (D2Act1Quest6Strc*)pQuest->pQuestDataEx;
	++pQuestDataEx->nTimerInvocations;

	if (pQuestDataEx->nTimerInvocations == 10)
	{
		SUNIT_IterateUnitsOfType(pGame, 0, SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pQuestDataEx->nAndarielGUID), ACT1Q6_UnitIterate_CreatePortalToTown);
		return false;
	}

	if (pQuestDataEx->nTimerInvocations != 12)
	{
		return false;
	}

	if (pQuest->fLastState != 3 && pQuest->fLastState != 13)
	{
		pQuest->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuest, 3, 0, ACT1Q6_UnitIterate_StatusCyclerEx, 1);
	}

	return true;
}

//D2Game.0x6FC9EE00
int32_t __fastcall ACT1Q6_UnitIterate_CreatePortalToTown(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);

	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_CATACOMBSLEV4)
	{
		return 0;
	}

	D2CoordStrc pCoord = {};
	UNITS_GetCoords(pUnit, &pCoord);
	D2GAME_CreatePortalObject_6FD13DF0(pGame, pUnit, pRoom, pCoord.nX, pCoord.nY, LEVEL_ROGUEENCAMPMENT, 0, 59, 0);
	return 1;
}

//D2Game.0x6FC9EE70
bool __fastcall ACT1Q6_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (!pQuestData->fState && pQuestData->bNotIntro == 1)
	{
		QUESTS_CreateTimer(pQuestData, ACT1Q6_Timer_StateDebug, 20);
	}

	return true;
}

//D2Game.0x6FC9EE90
bool __fastcall ACT1Q6_Timer_StateDebug(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	if (!pQuestData->fState)
	{
		QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
	}

	return true;
}

//D2Game.0x6FC9EEB0
void __fastcall ACT1Q6_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_ENTERAREA) == 1)
	{
		pQuestData->fLastState = 2;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_STARTED) == 1)
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 1;
	}
}
