#include "QUESTS/ACT2/A2Q4.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2QuestRecord.h>

#include "AI/AiTactics.h"
#include "AI/AiUtil.h"
#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterSpawn.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "SKILLS/Skills.h"


//D2Game.0x6FD35F58
D2NPCMessageTableStrc gpAct2Q4NpcMessages[] =
{
	{
		{
			{ MONSTER_DROGNAN, 373, 0, 1, },
		},
		1
	},
	{
		{
			{ MONSTER_DROGNAN, 373, 0, 2, },
			{ MONSTER_JERHYN, 377, 0, 1, },
		},
		2
	},
	{
		{
			{ MONSTER_ATMA, 380, 0, 2, },
			{ MONSTER_WARRIV2, 381, 0, 2, },
			{ MONSTER_GREIZ, 375, 0, 2, },
			{ MONSTER_ELZIX, 376, 0, 2, },
			{ MONSTER_DROGNAN, 383, 0, 2, },
			{ MONSTER_LYSANDER, 382, 0, 2, },
			{ MONSTER_CAIN2, 378, 0, 2, },
			{ MONSTER_JERHYN, 377, 0, 2, },
			{ MONSTER_MESHIF1, 384, 0, 2, },
			{ MONSTER_GEGLASH, 379, 0, 2, },
			{ MONSTER_FARA, 374, 0, 2, },
		},
		11
	},
	{
		{
			{ MONSTER_ATMA, 393, 0, 2, },
			{ MONSTER_WARRIV2, 394, 0, 2, },
			{ MONSTER_GREIZ, 387, 0, 2, },
			{ MONSTER_ELZIX, 385, 0, 2, },
			{ MONSTER_DROGNAN, 388, 0, 2, },
			{ MONSTER_JERHYN, 386, 0, 2, },
			{ MONSTER_LYSANDER, 389, 0, 2, },
			{ MONSTER_CAIN2, 395, 0, 2, },
			{ MONSTER_MESHIF1, 392, 0, 2, },
			{ MONSTER_GEGLASH, 391, 0, 2, },
			{ MONSTER_FARA, 390, 0, 2, },
		},
		11
	},
	{
		{
			{ MONSTER_ATMA, 406, 0, 1, },
			{ MONSTER_WARRIV2, 403, 0, 1, },
			{ MONSTER_GREIZ, 397, 0, 1, },
			{ MONSTER_ELZIX, 400, 0, 1, },
			{ MONSTER_JERHYN, 398, 0, 1, },
			{ MONSTER_DROGNAN, 399, 0, 1, },
			{ MONSTER_LYSANDER, 405, 0, 1, },
			{ MONSTER_CAIN2, 407, 0, 1, },
			{ MONSTER_MESHIF1, 402, 0, 1, },
			{ MONSTER_GEGLASH, 401, 0, 1, },
			{ MONSTER_FARA, 404, 0, 1, },
		},
		11
	},
	{
		{
			{ MONSTER_WARRIV2, 403, 0, 2, },
			{ MONSTER_GREIZ, 397, 0, 2, },
			{ MONSTER_ELZIX, 400, 0, 2, },
			{ MONSTER_JERHYN, 398, 0, 2, },
			{ MONSTER_DROGNAN, 399, 0, 2, },
			{ MONSTER_LYSANDER, 405, 0, 2, },
			{ MONSTER_CAIN2, 407, 0, 2, },
			{ MONSTER_MESHIF1, 402, 0, 2, },
			{ MONSTER_GEGLASH, 401, 0, 2, },
			{ MONSTER_FARA, 404, 0, 2, },
		},
		10
	},
	{
		{
			{ MONSTER_ACT2GUARD2, 185, 0, 1, },
		},
		1
	},
	{
		{
			{ MONSTER_ACT2GUARD2, 186, 0, 1, },
		},
		1
	},
	{
		{
			{ MONSTER_ACT2GUARD2, 187, 0, 1, },
		},
		1
	},
	{
		{
			{ MONSTER_ACT2GUARD2, 188, 0, 1, },
		},
		1
	},
	{
		{
			{ MONSTER_ACT2GUARD2, 189, 0, 1, },
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


//D2Game.0x6FCA25C0
bool __fastcall ACT2Q4_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED) == 1)
	{
		return false;
	}

	if (nNpcId == MONSTER_ACT2GUARD2)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING) == 1)
		{
			return false;
		}

		D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuest->pQuestDataEx;
		if (pQuestDataEx->nHaremBlockerObjectMode)
		{
			if (pQuestDataEx->nHaremBlockerObjectMode != OBJMODE_OPENED || !pQuestDataEx->bIsHaremBlockerNeutral)
			{
				return false;
			}

			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM4))
			{
				return true;
			}
		}
		else
		{
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM3))
			{
				return true;
			}
		}
	}
	else if (nNpcId == MONSTER_DROGNAN)
	{
		if (pQuest->fState != 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING))
		{
			return false;
		}

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED))
		{
			return true;
		}
	}
	else if (nNpcId == MONSTER_JERHYN)
	{
		if (pQuest->fState != 2 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING))
		{
			return false;
		}

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED))
		{
			return true;
		}
	}

	return false;
}

//D2Game.0x6FCA2660
int32_t __fastcall ACT2Q4_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A2Q4_HORAZONTOME);
	}
	return 0;
}

//D2Game.0x6FCA26D0
int32_t __fastcall ACT2Q4_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_LEAVETOWN);
	}
	else if (pQuestData->fState == 4)
	{
		if (pQuestData->fLastState == 2 || pQuestData->fLastState == 3)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_ENTERAREA);
		}
		else if (pQuestData->fLastState == 4)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM1);
		}
	}

	return 0;
}

//D2Game.0x6FCA2780
int32_t __fastcall ACT2Q4_IsHaremBlockerObjectOpened(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q4_HORAZONTOME);

	return pQuestData && ((D2Act2Quest4Strc*)pQuestData->pQuestDataEx)->nHaremBlockerObjectMode == OBJMODE_OPENED;
}

//D2Game.0x6FCA27B0
void __fastcall ACT2Q4_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_II)
	{
		return;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED);
	QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, QUESTSTATEFLAG_A2Q4);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM4);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM3);
}

//D2Game.0x6FCA2840
void __fastcall ACT2Q4_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pNPCMessages = gpAct2Q4NpcMessages;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT2Q4_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT2Q4_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT2Q4_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT2Q4_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT2Q4_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT2Q4_Callback13_PlayerStartedGame;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;
	pQuestData->pQuestDataEx = FOG_AllocPool(pQuestData->pGame->pMemoryPool, sizeof(D2Act2Quest4Strc), __FILE__, __LINE__, 0);
	pQuestData->nQuest = QUESTSTATEFLAG_A2Q4;
	pQuestData->pfStatusFilter = 0;
	pQuestData->nInitNo = 5;
	pQuestData->pfActiveFilter = ACT2Q4_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT2Q4_SeqCallback;
	pQuestData->nSeqId = 13;
	QUESTS_ResetPlayerGUIDCount(&pQuestData->tPlayerGUIDs);

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	memset(pQuestDataEx, 0x00, sizeof(D2Act2Quest4Strc));
	pQuestDataEx->bDrognanActivated = 0;
}

//D2Game.0x6FCA2920
void __fastcall ACT2Q4_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_DROGNAN)
	{
		return;
	}

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bDrognanActivated != 1)
	{
		return;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 2, 0, ACT2Q4_UnitIterate_StatusCyclerEx, 1);
	pQuestDataEx->bDrognanActivated = 0;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q4_UnitIterate_UpdateQuestStateFlags);
}

//D2Game.0x6FCA2980
void __fastcall ACT2Q4_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	const int16_t nMessageIndex = pQuestArg->nMessageIndex;

	if (!pQuestData)
	{
		return;
	}

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (pQuestArg->nNPCNo == MONSTER_ACT2GUARD2)
	{
		if (nMessageIndex == 186)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM3);
		}
		else if (nMessageIndex > 186 && nMessageIndex <= 189)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM4);
		}
	}
	else
	{
		if (nMessageIndex == 406 || nMessageIndex == 403 || nMessageIndex == 397 || nMessageIndex == 400 || nMessageIndex == 398 || nMessageIndex == 399
			|| nMessageIndex == 405 || nMessageIndex == 407 || nMessageIndex == 402 || nMessageIndex == 401 || nMessageIndex == 404)
		{
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING))
			{
				QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING);

				QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));
			}
		}

		if (nMessageIndex == 396)
		{
			if (pQuestData->bNotIntro && pQuestData->fLastState < 5)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 5, 0, ACT2Q4_UnitIterate_StatusCyclerEx, 0);
			}

			if (!pQuestDataEx->bPortalToCanyonOpen && UNITS_GetRoom(pQuestArg->pPlayer) == pQuestDataEx->pRoom)
			{
				D2RoomStrc* pRoom = nullptr;
				D2CoordStrc pCoord = {};
				UNITS_GetCoords(pQuestArg->pPlayer, &pCoord);
				QUESTS_GetFreePosition(pQuestDataEx->pRoom, &pCoord, 2, 0xBE11, &pRoom, 8);
				if (pRoom && D2GAME_CreatePortalObject_6FD13DF0(pQuestData->pGame, pQuestArg->pPlayer, pRoom, pCoord.nX, pCoord.nY, LEVEL_CANYONOFTHEMAGI, 0, OBJECT_PERMANENT_TOWN_PORTAL, 0))
				{
					pQuestDataEx->bPortalToCanyonOpen = 1;
					QUESTS_SetGlobalState(pQuestArg->pGame, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE);
				}
			}
		}

		if (pQuestArg->nNPCNo == MONSTER_DROGNAN)
		{
			if (nMessageIndex == 373)
			{
				QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);

				pQuestDataEx->bDrognanActivated = 1;
				pQuestDataEx->bPalaceOpen = 1;

				if (pQuestDataEx->nHaremBlockerObjectMode == OBJMODE_NEUTRAL)
				{
					pQuestDataEx->bIsHaremBlockerNeutral = 1;
				}

				pQuestDataEx->nHaremBlockerObjectMode = OBJMODE_OPENED;
				SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q4_UnitIterate_UpdateQuestStateFlags);

				if (pQuestDataEx->bHaremBlockerInitialized == 1)
				{
					D2UnitStrc* pHaremBlocker = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nHaremBlockerGUID);
					if (pHaremBlocker)
					{
						UNITS_ChangeAnimMode(pHaremBlocker, OBJMODE_OPENED);
						UNITS_FreeCollisionPath(pHaremBlocker);
					}
				}
				QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
			}
		}
		else if (pQuestArg->nNPCNo == MONSTER_JERHYN)
		{
			if (nMessageIndex == 377)
			{
				QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
				QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 3, 0, ACT2Q4_UnitIterate_StatusCyclerEx, 1);
				SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q4_UnitIterate_UpdateQuestStateFlags);
			}
		}
	}
}

//D2Game.0x6FCA2C50
void __fastcall ACT2Q4_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4, 0, 0
	};

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
		if (nNpcId == MONSTER_ACT2GUARD2)
		{
			D2SeedStrc* pSeed = QUESTS_GetGlobalSeed(pQuestData->pGame);
			if (((D2Act2Quest4Strc*)pQuestData->pQuestDataEx)->bPalaceOpen)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_ACT2GUARD2, (uint32_t)SEED_RollRandomNumber(pSeed) % 3 + 8);
			}
			else
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_ACT2GUARD2, 7);
			}
			return;
		}
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
	}
	else if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 5);
	}
	else if (pQuestData->fState && pQuestData->bNotIntro && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED) != 1
		&& (pQuestData->fState < 5 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_PRIMARYGOALDONE)))
	{
		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < 12)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//D2Game.0x6FCA2D90
void __fastcall ACT2Q4_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel == LEVEL_ARCANESANCTUARY)
	{
		if (pQuestData->fState < 4)
		{
			QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
		}

		if (pQuestData->fLastState >= 4)
		{
			if (pQuestData->fState >= 4)
			{
				return;
			}
		}
		else
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 4, 0, ACT2Q4_UnitIterate_StatusCyclerEx, 1);
		}

		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q4_UnitIterate_UpdateQuestStateFlags);
		return;
	}
	else if (pQuestArg->nNewLevel == LEVEL_HAREMLEV1)
	{
		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM4);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM3);
	}

	if (pQuestArg->nOldLevel == LEVEL_LUTGHOLEIN)
	{
		D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
		if (pQuestDataEx->bJerhynStartInitialized == 1)
		{
			D2UnitStrc* pJerhyn = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_MONSTER, pQuestDataEx->nJerhynGUID);
			if (pJerhyn)
			{
				D2MonsterInteractStrc* pMonInteract = nullptr;
				if (pJerhyn->dwUnitType == UNIT_MONSTER && pJerhyn->pMonsterData)
				{
					pMonInteract = pJerhyn->pMonsterData->pMonInteract;
				}

				if (MONSTERAI_HasInteractUnit(pMonInteract))
				{
					D2GAME_MONSTERAI_Last_6FC61F70(pQuestData->pGame, pJerhyn, 0);
				}
				else
				{
					sub_6FC3B2B0(pJerhyn, pQuestData->pGame);
				}
			}
			else
			{
				pQuestDataEx->bJerhynStartInitialized = 0;
			}
		}

		if (!pQuestDataEx->bJerhynPalaceInitialized)
		{
			if (pQuestDataEx->bHaremBlockerInitialized)
			{
				D2UnitStrc* pHaremBlocker = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nHaremBlockerGUID);
				if (pHaremBlocker)
				{
					D2CoordStrc pCoord = {};
					UNITS_GetCoords(pHaremBlocker, &pCoord);
					ACT2Q4_InitializeJerhynMonster(pQuestData, pHaremBlocker, UNITS_GetRoom(pHaremBlocker), &pCoord);
				}
			}
		}

		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);

		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING) != 1 && pQuestData->fState == 3)
		{
			QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q4_UnitIterate_UpdateQuestStateFlags);
		}
	}
}

//D2Game.0x6FCA2F60
void __fastcall ACT2Q4_InitializeJerhynMonster(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit, D2RoomStrc* pRoom, D2CoordStrc* pCoord)
{
	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;

	if (pQuestDataEx->bJerhynStartInitialized == 1)
	{
		D2UnitStrc* pJerhyn = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_MONSTER, pQuestDataEx->nJerhynGUID);
		if (pJerhyn)
		{
			D2MonsterInteractStrc* pMonInteract = nullptr;
			if (pJerhyn->dwUnitType == UNIT_MONSTER && pJerhyn->pMonsterData)
			{
				pMonInteract = pJerhyn->pMonsterData->pMonInteract;
			}

			if (MONSTERAI_HasInteractUnit(pMonInteract))
			{
				D2GAME_MONSTERAI_Last_6FC61F70(pQuestData->pGame, pJerhyn, 0);
				return;
			}

			sub_6FC3B2B0(pJerhyn, pQuestData->pGame);
			pQuestDataEx->bJerhynStartInitialized = 0;
		}
	}

	if (pQuestDataEx->bJerhynPalaceInitialized == 1)
	{
		return;
	}

	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A2Q6_DURIEL);

	if (pQuest && pQuest->bNotIntro && pQuest->fState < 2)
	{
		pCoord->nX = pCoord->nX - 10;
	}
	else
	{
		pCoord->nX = pCoord->nX + 15;
	}
	pCoord->nY = pCoord->nY - 3;

	D2RoomStrc* pFoundRoom = D2GAME_GetRoom_6FC52070(pRoom, pCoord->nX, pCoord->nY);
	QUESTS_GetFreePosition(pRoom, pCoord, 3, 0x100, &pFoundRoom, 9);

	D2UnitStrc* pJerhyn = sub_6FC69F10(pQuestData->pGame, pFoundRoom, pCoord->nX, pCoord->nY, MONSTER_JERHYN, 1, -1, 0);
	if (!pJerhyn)
	{
		pJerhyn = sub_6FC69F10(pQuestData->pGame, pFoundRoom, pCoord->nX, pCoord->nY, MONSTER_JERHYN, 1, 2, 0);
	}

	if (!pJerhyn)
	{
		return;
	}

	pJerhyn->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;

	pQuestDataEx->bJerhynPalaceInitialized = 1;
	if (!pQuestDataEx->bJerhynCoordinatesStored)
	{
		pQuestDataEx->bJerhynCoordinatesStored = 1;
		pQuestDataEx->pJerhynCoords.nX = pCoord->nX;
		pQuestDataEx->pJerhynCoords.nY = pCoord->nY;
	}
}

//D2Game.0x6FCA3160
bool __fastcall ACT2Q4_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (pQuestData->fState || pQuestData->bNotIntro != 1)
	{
		return true;
	}

	QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, (D2Quests)pQuestData->nSeqId);
	if (!pQuest)
	{
		return true;
	}

	if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
	{
		FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
		exit(-1);
	}
	pQuestData->pfSeqFilter(pQuest);
	return true;
}

//D2Game.0x6FCA31E0
void __fastcall ACT2Q4_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDGRANTED))
	{
		pQuestDataEx->bPalaceOpen = 1;
		pQuestDataEx->nHaremBlockerObjectMode = OBJMODE_OPENED;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING) == 1)
	{
		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q4, QFLAG_PRIMARYGOALDONE);
		pQuestDataEx->bPalaceOpen = 1;
		pQuestDataEx->nHaremBlockerObjectMode = OBJMODE_OPENED;
		return;
	}

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' tsh'))
	{
		pQuestData->fLastState = 1;
		pQuestData->fState = 1;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM1) == 1)
	{
		pQuestData->fLastState = 4;
		pQuestData->fState = 4;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_ENTERAREA) == 1)
	{
		pQuestData->fLastState = 3;
		pQuestData->fState = 4;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fLastState = 3;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_STARTED) == 1)
	{
		pQuestData->fLastState = 2;
		pQuestData->fState = 2;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED) != 1)
	{
		return;
	}

	pQuestDataEx->bPalaceOpen = 1;
	pQuestDataEx->nHaremBlockerObjectMode = OBJMODE_OPENED;
}

//D2Game.0x6FCA3320
void __fastcall ACT2Q4_InitializeJerhynStartObject(D2QuestDataStrc* pQuestData, D2ObjInitFnStrc* pOp)
{
	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bJerhynPalaceInitialized || QUESTS_GetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q0, QFLAG_PRIMARYGOALDONE) || QUESTS_GetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q4, QFLAG_PRIMARYGOALDONE))
	{
		return;
	}

	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A2Q6_DURIEL);
	if (!pQuest || !pQuest->bNotIntro || pQuest->fState >= 2)
	{
		return;
	}

	D2RoomStrc* pRoom = pOp->pRoom;
	D2CoordStrc pCoord = {};
	pCoord.nX = pOp->nX;
	pCoord.nY = pOp->nY;

	QUESTS_GetFreePosition(pRoom, &pCoord, 2, 0x100, &pRoom, 10);

	D2UnitStrc* pJerhyn = sub_6FC69F10(pQuestData->pGame, pRoom, pCoord.nX, pCoord.nY, MONSTER_JERHYN, 1, -1, 0);
	if (pJerhyn)
	{
		pQuestDataEx->bJerhynStartInitialized = 1;
		pQuestDataEx->nJerhynGUID = pJerhyn->dwUnitId;
	}
}

//D2Game.0x6FCA33F0
void __fastcall ACT2Q4_InitializeJerhynPalaceObject(D2QuestDataStrc* pQuestData, D2ObjInitFnStrc* pOp)
{
	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	if (QUESTS_GetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE) != 1)
	{
		pQuestDataEx->pGuardCoords.nX = pOp->nX + 1;
		pQuestDataEx->pGuardCoords.nY = pOp->nY;

		sub_6FC6A090(pQuestData->pGame, pOp->pRoom, pQuestDataEx->pGuardCoords.nX, pQuestDataEx->pGuardCoords.nY, MONSTER_ACT2GUARD2, 1, 0);
	}

	EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_QUESTFN, pOp->pGame->dwGameFrame + 1, 0, 0);

	D2CoordStrc pCoord = {};
	pCoord.nX = pOp->nX;
	pCoord.nY = pOp->nY;

	if (pQuestDataEx->bJerhynPalaceInitialized)
	{
		return;
	}

	if (!QUESTS_GetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q0, QFLAG_PRIMARYGOALDONE) && !QUESTS_GetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q1, QFLAG_PRIMARYGOALDONE))
	{
		D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A2Q6_DURIEL);
		if (pQuest && pQuest->bNotIntro && pQuest->fState < 2)
		{
			return;
		}
	}

	ACT2Q4_InitializeJerhynMonster(pQuestData, pOp->pObject, pOp->pRoom, &pCoord);
}

//D2Game.0x6FCA34D0
int32_t __fastcall ACT2Q4_IsGuardMoving(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->unk0x0F != 1 || pQuestDataEx->unk0x10)
	{
		return 0;
	}

	pQuestDataEx->unk0x10 = 1;
	return 1;
}

//D2Game.0x6FCA3500
void __fastcall ACT2Q4_InitializeHaremBlockerObject(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit)
{
	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	D2CoordStrc pCoord = {};
	UNITS_GetCoords(pUnit, &pCoord);
	if (pQuestDataEx->bPalaceOpen || pQuestDataEx->bHaremBlockerInitialized)
	{
		return;
	}

	pQuestDataEx->nHaremBlockerObjectMode = OBJMODE_NEUTRAL;
	pQuestDataEx->pHaremBlockerCoords.nX = pCoord.nX - 2;
	pQuestDataEx->pHaremBlockerCoords.nY = pCoord.nY - 1;

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);

	D2UnitStrc* pHaremBlocker = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_EUNUCH_HAREM_BLOCKER, pQuestDataEx->pHaremBlockerCoords.nX, pQuestDataEx->pHaremBlockerCoords.nY, pQuestData->pGame, pRoom, 1, 0, 0);

	if (!pHaremBlocker)
	{
		pQuestDataEx->pHaremBlockerCoords.nY = pCoord.nY;
		pHaremBlocker = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_EUNUCH_HAREM_BLOCKER, pQuestDataEx->pHaremBlockerCoords.nX, pQuestDataEx->pHaremBlockerCoords.nY, pQuestData->pGame, UNITS_GetRoom(pUnit), 1, 0, 0);
	}

	if (!pHaremBlocker)
	{
		return;
	}

	pHaremBlocker->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
	pQuestDataEx->bHaremBlockerInitialized = 1;
	pQuestDataEx->nHaremBlockerGUID = pHaremBlocker->dwUnitId;
}

//D2Game.0x6FCA35D0
void __fastcall OBJECTS_InitFunction30_HaremBlocker(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData)
	{
		return;
	}

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nHaremBlockerObjectMode);

	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}
	pQuestDataEx->nHaremBlockerGUID = nUnitId;

	if (pQuestDataEx->nHaremBlockerObjectMode == OBJMODE_OPENED)
	{
		UNITS_FreeCollisionPath(pOp->pObject);
	}
}

//D2Game.0x6FCA3620
int32_t __fastcall ACT2Q4_IsJerhynPalaceActivated(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData11 = QUESTS_GetQuestData(pGame, QUEST_A2Q4_HORAZONTOME);
	D2QuestDataStrc* pQuestData8 = QUESTS_GetQuestData(pQuestData11->pGame, QUEST_A2Q1_RADAMENT);
	D2QuestDataStrc* pQuestData13 = QUESTS_GetQuestData(pQuestData11->pGame, QUEST_A2Q6_DURIEL);

	return !pQuestData11 || !((D2Act2Quest4Strc*)pQuestData11->pQuestDataEx)->bJerhynPalaceInitialized || !pQuestData8 || !pQuestData8->bNotIntro || pQuestData8->fState == 5
		|| !pQuestData13 || !pQuestData13->bNotIntro || pQuestData13->fState >= 4 || pQuestData11->bNotIntro != 1 || pQuestData11->fState >= 2;
}

//D2Game.0x6FCA36A0
int32_t __fastcall ACT2Q4_HasGuardMovedToEndPosition(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData)
	{
		return 1;
	}

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	if (!pQuestDataEx->unk0x18 && !pQuestDataEx->unk0x19)
	{
		return pQuestDataEx->nHaremBlockerObjectMode == OBJMODE_OPENED;
	}

	return 0;
}

//D2Game.0x6FCA36E0
int32_t __fastcall ACT2Q4_GetGuardCoordinates(D2GameStrc* pGame, D2CoordStrc* pCoord)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->unk0x18)
	{
		pCoord->nX = pQuestDataEx->pGuardCoords.nX;
		pCoord->nY = pQuestDataEx->pGuardCoords.nY;
		return 1;
	}

	if (pQuestDataEx->unk0x19)
	{
		pCoord->nX = pQuestDataEx->pGuardCoords.nX;
		pCoord->nY = pQuestDataEx->pGuardCoords.nY - 4;
		return 1;
	}

	return 0;
}

//D2Game.0x6FCA3740
void __fastcall ACT2Q4_GetAndUpdatePalaceNpcState(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* a3, int32_t* pIdle)
{
	D2QuestDataStrc* pQuestData11 = QUESTS_GetQuestData(pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData11 || !pQuestData11->bNotIntro)
	{
		*a3 = 1;
		*pIdle = 0;
		return;
	}

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData11->pQuestDataEx;
	if (!pQuestDataEx->bJerhynPalaceInitialized)
	{
		*a3 = 1;
		*pIdle = 0;
		return;
	}

	D2QuestDataStrc* pQuestData13 = QUESTS_GetQuestData(pGame, QUEST_A2Q6_DURIEL);
	if (QUESTS_GetGlobalState(pGame, QUESTSTATEFLAG_A2Q4, QFLAG_PRIMARYGOALDONE) || QUESTS_GetGlobalState(pGame, QUESTSTATEFLAG_A2Q6, QFLAG_PRIMARYGOALDONE) || !pQuestData13 || !pQuestData13->bNotIntro || pQuestData13->fState >= 2 || pQuestDataEx->unk0x14)
	{
		*a3 = 1;
		*pIdle = 0;
		return;
	}

	if (pQuestData13 && pQuestData13->bNotIntro && pQuestData13->fState == 1)
	{
		pQuestDataEx->unk0x12 = 1;
	}

	if (pQuestDataEx->unk0x12 != 1)
	{
		*a3 = 1;
		*pIdle = 0;
		return;
	}

	if (!pQuestDataEx->unk0x13 || pQuestDataEx->unk0x0F)
	{
		pQuestDataEx->bPlayerCloseToHaremBlocker = 0;
		pQuestDataEx->nPlayerCloseToHaremBlockerGUID = 0;
		SUNIT_IterateUnitsOfType(pQuestData11->pGame, 0, 0, ACT2Q4_UnitIterate_CheckDistanceToHaremBlocker);

		if (pQuestDataEx->bPlayerCloseToHaremBlocker)
		{
			AITACTICS_WalkToTargetCoordinates(pGame, pUnit, pQuestDataEx->pHaremBlockerCoords.nX, pQuestDataEx->pHaremBlockerCoords.nY);
			pQuestDataEx->unk0x13 = 1;
			pQuestDataEx->unk0x12 = 0;

			*a3 = 0;
			*pIdle = 0;
		}
		else
		{
			*a3 = 0;
			*pIdle = 1;
		}

		return;
	}

	*a3 = 0;
	*pIdle = 1;

	if (pQuestDataEx->bHaremBlockerInitialized != 1)
	{
		return;
	}

	if (AIUTIL_GetDistanceToCoordinates(pUnit, pQuestDataEx->pHaremBlockerCoords.nX, pQuestDataEx->pHaremBlockerCoords.nY) > 2)
	{
		pQuestDataEx->unk0x19 = 1;
		pQuestDataEx->unk0x18 = 0;
		AITACTICS_WalkToTargetCoordinates(pGame, pUnit, pQuestDataEx->pHaremBlockerCoords.nX, pQuestDataEx->pHaremBlockerCoords.nY);
		*pIdle = 0;
	}
	else
	{
		D2UnitStrc* pHaremBlocker = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nHaremBlockerGUID);
		if (!pHaremBlocker)
		{
			return;
		}

		D2CoordStrc pCoord = {};
		UNITS_GetCoords(pHaremBlocker, &pCoord);
		pCoord.nX += 4;
		D2RoomStrc* pRoom = UNITS_GetRoom(pHaremBlocker);

		if ((COLLISION_GetFreeCoordinates(pRoom, &pCoord, 1, 0x3C01, 0) || COLLISION_GetFreeCoordinates(pRoom, &pCoord, 2, 0x3C01, 0) || COLLISION_GetFreeCoordinates(pRoom, &pCoord, 3, 0x3C01, 0)) && sub_6FCBDFE0(pGame, pUnit, pRoom, pCoord.nX, pCoord.nY, 0, 0))
		{
			pQuestDataEx->unk0x0F = 1;
			pQuestDataEx->unk0x14 = 1;
		}
	}
}

//D2Game.0x6FCA3A10
int32_t __fastcall ACT2Q4_UnitIterate_CheckDistanceToHaremBlocker(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q6, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)QUESTS_GetQuestData(pGame, QUEST_A2Q4_HORAZONTOME)->pQuestDataEx;

	if (AIUTIL_GetDistanceToCoordinates(pUnit, pQuestDataEx->pHaremBlockerCoords.nX, pQuestDataEx->pHaremBlockerCoords.nY) > 30)
	{
		return 0;
	}

	pQuestDataEx->bPlayerCloseToHaremBlocker = 1;
	if (pUnit)
	{
		pQuestDataEx->nPlayerCloseToHaremBlockerGUID = pUnit->dwUnitId;
	}
	else
	{
		pQuestDataEx->nPlayerCloseToHaremBlockerGUID = -1;
	}

	return 1;
}

//D2Game.0x6FCA3AA0
int32_t __fastcall OBJECTS_OperateFunction42_SanctuaryTome(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
	{
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

		D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
		EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, pOp->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		return 1;
	}

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	QUESTS_SendScrollMessage(pOp->pPlayer, pObject, 396);

	pQuestDataEx->pRoom = UNITS_GetRoom(pObject);

	if (!pQuestData->bNotIntro || pQuestData->fState == 5)
	{
		return 1;
	}

	QUESTS_StateDebug(pQuestData, 5, "..\\D2Game/Quests/a2q4.cpp", 1517);
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q4_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q4_UnitIterate_SetCompletionFlag);
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q4_UnitIterate_UselessGoalCheck);
	return 1;
}

//D2Game.0x6FCA3B80
int32_t __fastcall ACT2Q4_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_COMPLETEDNOW);
	}

	return 0;
}

//D2Game.0x6FCA3BC0
int32_t __fastcall ACT2Q4_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_ARCANESANCTUARY)
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDPENDING);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED);
	QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, QUESTSTATEFLAG_A2Q4);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM4);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_CUSTOM3);

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT2Q4_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCA3C70
int32_t __fastcall ACT2Q4_UnitIterate_UselessGoalCheck(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q4, QFLAG_PRIMARYGOALDONE);//TODO: Meant to be set?

	return 0;
}

//D2Game.0x6FCA3CA0
void __fastcall OBJECTS_InitFunction29_ArcaneSanctuaryPortal(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pOp->pRoom);

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;

	if (nLevelId == LEVEL_ARCANESANCTUARY)
	{
		UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nPortalModeInSanctuary);
		if (pQuestDataEx->nPortalModeInSanctuary == OBJMODE_OPERATING)
		{
			pQuestDataEx->nPortalModeInSanctuary = OBJMODE_OPENED;

			if (pOp->pObject)
			{
				D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->pObject->dwClassId);
				EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
			}
		}
	}
	else if (nLevelId == LEVEL_PALACECELLARLEV3)
	{
		UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nPortalModeInCellar);
		if (pQuestDataEx->nPortalModeInCellar == OBJMODE_OPERATING)
		{
			pQuestDataEx->nPortalModeInCellar = OBJMODE_OPENED;

			if (pOp->pObject)
			{
				D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->pObject->dwClassId);
				EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
			}
		}
	}
}

//D2Game.0x6FCA3D60
void __fastcall ACT2Q4_SetPortalMode(D2GameStrc* pGame, int32_t nLevelId)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData)
	{
		return;
	}

	D2Act2Quest4Strc* pQuestDataEx = (D2Act2Quest4Strc*)pQuestData->pQuestDataEx;
	if (nLevelId == LEVEL_ARCANESANCTUARY)
	{
		if (!pQuestDataEx->nPortalModeInCellar)
		{
			pQuestDataEx->nPortalModeInCellar = OBJMODE_OPERATING;
			pQuestDataEx->nPortalModeInSanctuary = OBJMODE_OPENED;
		}
	}
	else if (nLevelId == LEVEL_PALACECELLARLEV3)
	{
		if (!pQuestDataEx->nPortalModeInSanctuary)
		{
			pQuestDataEx->nPortalModeInSanctuary = OBJMODE_OPERATING;
			pQuestDataEx->nPortalModeInCellar = OBJMODE_OPENED;
		}
	}
}

//
void __fastcall ACT2Q4_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_RemovePlayerGUID(pQuestData, pQuestArg);
}
