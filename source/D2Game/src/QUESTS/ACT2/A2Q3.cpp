#include "QUESTS/ACT2/A2Q3.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2Environment.h>
#include <D2Items.h>
#include <D2QuestRecord.h>

#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/Items.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT2/A2Q2.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD35AA8
D2NPCMessageTableStrc gpAct2Q3NpcMessages[] =
{
	{
		{
			{ MONSTER_ATMA, 344, 0, 2, },
			{ MONSTER_WARRIV2, 343, 0, 2, },
			{ MONSTER_GREIZ, 341, 0, 2, },
			{ MONSTER_ELZIX, 342, 0, 2, },
			{ MONSTER_DROGNAN, 348, 0, 1, },
			{ MONSTER_LYSANDER, 347, 0, 2, },
			{ MONSTER_CAIN2, 350, 0, 2, },
			{ MONSTER_MESHIF1, 349, 0, 2, },
			{ MONSTER_GEGLASH, 345, 0, 2, },
			{ MONSTER_JERHYN, 340, 0, 2, },
			{ MONSTER_FARA, 346, 0, 2, },
		},
		11
	},
	{
		{
			{ MONSTER_ATMA, 344, 0, 2, },
			{ MONSTER_WARRIV2, 343, 0, 2, },
			{ MONSTER_GREIZ, 341, 0, 2, },
			{ MONSTER_ELZIX, 342, 0, 2, },
			{ MONSTER_DROGNAN, 348, 0, 2, },
			{ MONSTER_LYSANDER, 347, 0, 2, },
			{ MONSTER_CAIN2, 350, 0, 2, },
			{ MONSTER_MESHIF1, 349, 0, 2, },
			{ MONSTER_GEGLASH, 345, 0, 2, },
			{ MONSTER_JERHYN, 340, 0, 2, },
			{ MONSTER_FARA, 346, 0, 2, },
		},
		11
	},
	{
		{
			{ MONSTER_ATMA, 361, 0, 2, },
			{ MONSTER_WARRIV2, 353, 0, 2, },
			{ MONSTER_GREIZ, 352, 0, 2, },
			{ MONSTER_ELZIX, 359, 0, 2, },
			{ MONSTER_DROGNAN, 358, 0, 2, },
			{ MONSTER_LYSANDER, 357, 0, 2, },
			{ MONSTER_CAIN2, 360, 0, 2, },
			{ MONSTER_MESHIF1, 355, 0, 2, },
			{ MONSTER_JERHYN, 351, 0, 2, },
			{ MONSTER_GEGLASH, 354, 0, 2, },
			{ MONSTER_FARA, 356, 0, 2, },
		},
		11
	},
	{
		{
			{ MONSTER_ATMA, 368, 0, 1, },
			{ MONSTER_WARRIV2, 366, 0, 1, },
			{ MONSTER_GREIZ, 363, 0, 1, },
			{ MONSTER_ELZIX, 364, 0, 1, },
			{ MONSTER_DROGNAN, 371, 0, 1, },
			{ MONSTER_LYSANDER, 370, 0, 1, },
			{ MONSTER_CAIN2, 372, 0, 1, },
			{ MONSTER_MESHIF1, 367, 0, 1, },
			{ MONSTER_GEGLASH, 365, 0, 1, },
			{ MONSTER_JERHYN, 362, 0, 1, },
		},
		10
	},
	{
		{
			{ MONSTER_ATMA, 368, 0, 2, },
			{ MONSTER_WARRIV2, 366, 0, 2, },
			{ MONSTER_GREIZ, 363, 0, 2, },
			{ MONSTER_JERHYN, 362, 0, 2, },
			{ MONSTER_ELZIX, 364, 0, 2, },
			{ MONSTER_DROGNAN, 371, 0, 2, },
			{ MONSTER_LYSANDER, 370, 0, 2, },
			{ MONSTER_MESHIF1, 367, 0, 2, },
			{ MONSTER_GEGLASH, 365, 0, 2, },
			{ MONSTER_FARA, 369, 0, 2, },
		},
		10
	},
	{
		{
			{ -1, 0, 0, 2 },
		},
		0
	}
};


//D2Game.0x6FCA1440
void __fastcall ACT2Q3_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDPENDING) == 1)
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

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDPENDING);
}

//D2Game.0x6FCA14B0
bool __fastcall ACT2Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDGRANTED) == 1)
	{
		return false;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_REWARDPENDING) == 1)
	{
		switch (nNpcId)
		{
		case MONSTER_ATMA:
		case MONSTER_WARRIV2:
		case MONSTER_GREIZ:
		case MONSTER_ELZIX:
		case MONSTER_DROGNAN:
		case MONSTER_LYSANDER:
		case MONSTER_CAIN2:
		case MONSTER_MESHIF1:
		case MONSTER_GEGLASH:
		case MONSTER_JERHYN:
			return true;
		}
	}

	if (nNpcId == MONSTER_DROGNAN && pQuest->fState == 1)
	{
		return true;
	}

	return false;
}

//D2Game.0x6FCA1640
int32_t __fastcall ACT2Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A2Q3_TAINTEDSUN);
	}

	return 0;
}

//D2Game.0x6FCA16B0
int32_t __fastcall ACT2Q3_UnitIterate_UpdateClient(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
	if (!pClient || pClient->nAct != ACT_II)
	{
		return 0;
	}

	D2GSPacketSrv53 packet53 = {};
	packet53.nHeader = 0x53;
	packet53.unk0x01 = 5;
	packet53.unk0x05 = 0;
	packet53.unk0x09 = 1;
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet53, sizeof(packet53));

	D2GSPacketSrv5D packet5D = {};
	packet5D.field_4 = 0;
	packet5D.nHeader = 0x5D;
	packet5D.nQuestId = QUEST_A2Q3_TAINTEDSUN;
	packet5D.nFlags = 16;
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5D, sizeof(packet5D));
	return 0;
}

//D2Game.0x6FCA1730
void __fastcall ACT2Q3_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT2Q3_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT2Q3_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT2Q3_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT2Q3_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT2Q3_Callback13_PlayerStartedGame;
	pQuestData->pNPCMessages = gpAct2Q3NpcMessages;
	pQuestData->bActive = 1;

	D2Act2Quest3Strc* pQuestDataEx = (D2Act2Quest3Strc*)FOG_AllocPool(pQuestData->pGame->pMemoryPool, sizeof(D2Act2Quest3Strc), __FILE__, __LINE__, 0);
	pQuestData->nQuest = QUESTSTATEFLAG_A2Q3;
	pQuestData->nSeqId = 11;
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->pfStatusFilter = 0;
	pQuestData->nInitNo = 4;
	pQuestData->pfActiveFilter = ACT2Q3_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT2Q3_SeqCallback;

	memset(pQuestDataEx, 0x00, sizeof(D2Act2Quest3Strc));
	pQuestDataEx->nAltarMode = 0;
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->tPlayerGUIDs);
}

//D2Game.0x6FCA17F0
void __fastcall ACT2Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nMessageIndex == 348)
	{
		if (pQuestData->bNotIntro && pQuestArg->nNPCNo == MONSTER_DROGNAN)
		{
			if (pQuestData->fLastState == 1)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 2, 0, ACT2Q3_UnitIterate_StatusCyclerEx, 0);
			}

			if (pQuestData->fState == 1)
			{
				QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
				SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q3_UnitIterate_UpdateQuestStateFlags);
			}
		}
	}
	else if (pQuestArg->nMessageIndex > 361 && pQuestArg->nMessageIndex <= 372)
	{
		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
		if (pQuestData->fState != 5 && pQuestData->bNotIntro == 1 && QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE))
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 13, 0, ACT2Q3_UnitIterate_StatusCyclerEx, 0);
			QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
			if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
			{
				FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
				exit(-1);
			}
			pQuestData->pfSeqFilter(pQuestData);
		}

		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);

		if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING))
		{
			return;
		}

		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDGRANTED);
		QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDPENDING);

		QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));
		if (!pQuestData->bNotIntro)
		{
			QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q3, QFLAG_PRIMARYGOALDONE);
		}
	}
}

//D2Game.0x6FCA19B0
void __fastcall ACT2Q3_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_RemovePlayerGUID(pQuestData, pQuestArg);

	D2Act2Quest3Strc* pQuestDataEx = (D2Act2Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->pPlayer)
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
	}
	else
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, -1);
	}
}

//D2Game.0x6FCA19E0
void __fastcall ACT2Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 0
	};

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, 1))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
	}
	else if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, 14))
	{
		if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)) == 1)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
		}
		else if (pQuestData->fState && pQuestData->bNotIntro && QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) != 1 && pQuestData->fState < 4)
		{
			const int32_t nIndex = nIndices[pQuestData->fState];
			if (nIndex != -1 && (uint32_t)nIndex < 6)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
			}
		}
	}
}

//D2Game.0x6FCA1AC0
void __fastcall ACT2Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act2Quest3Strc* pQuestDataEx = (D2Act2Quest3Strc*)pQuestData->pQuestDataEx;

	if ((pQuestArg->nNewLevel == LEVEL_LOSTCITY || pQuestArg->nNewLevel == LEVEL_VALLEYOFSNAKES) && !pQuestData->fState && pQuestData->bNotIntro == 1 && !pQuestDataEx->bDarkenTimerCreated)
	{
		QUESTS_CreateTimer(pQuestData, ACT2Q3_DarkenEnvironment, (SEED_RollRandomNumber(QUESTS_GetGlobalSeed(pQuestData->pGame)) & 1) + 15);
		pQuestDataEx->bDarkenTimerCreated = 1;
	}

	if (pQuestArg->nNewLevel == LEVEL_LUTGHOLEIN && pQuestDataEx->bBlackened == 1)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT2Q3_UnitIterate_StatusCyclerEx, 1);
		if (!pQuestData->fState)
		{
			QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
		}

		if (pQuestData->pGame->pAct[ACT_II])
		{
			ENVIRONMENT_TaintedSunBegin(pQuestData->pGame->pAct[ACT_II]);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q3_UnitIterate_UpdateClient);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q3_UnitIterate_UpdateQuestStateFlags);
			pQuestDataEx->bBlackened = 0;
		}
		else
		{
			pQuestDataEx->bBlackened = 1;
		}
	}

	if (pQuestArg->nOldLevel == LEVEL_LUTGHOLEIN)
	{
		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);
		if (pQuestData->fState == 2)
		{
			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		}
	}
}

//D2Game.0x6FCA1C00
bool __fastcall ACT2Q3_DarkenEnvironment(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act2Quest3Strc* pQuestDataEx = (D2Act2Quest3Strc*)pQuestData->pQuestDataEx;

	pQuestDataEx->bDarkenTimerCreated = 0;
	if (pQuestDataEx->bRewarded)
	{
		return true;
	}

	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 1, 0, ACT2Q3_UnitIterate_StatusCyclerEx, 1);

	if (!pQuestData->fState)
	{
		QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
	}

	if (pQuestData->pGame->pAct[ACT_II])
	{
		ENVIRONMENT_TaintedSunBegin(pQuestData->pGame->pAct[ACT_II]);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q3_UnitIterate_UpdateClient);
		pQuestDataEx->bRewarded = 1;
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q3_UnitIterate_UpdateQuestStateFlags);
		return true;
	}

	pQuestDataEx->bBlackened = 1;
	return true;
}

//D2Game.0x6FCA1CA0
bool __fastcall ACT2Q3_SeqCallback(D2QuestDataStrc* pQuestData)
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

//D2Game.0x6FCA1D10
int32_t __fastcall ACT2Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q3_TAINTEDSUN);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState >= 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_STARTED);
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_LEAVETOWN);
	}
	else if (pQuestData->fState == 3)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_ENTERAREA);
	}

	return 0;
}

//D2Game.0x6FCA1D90
void __fastcall ACT2Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDPENDING) == 1)
	{
		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A2Q3, QFLAG_PRIMARYGOALDONE);
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_STARTED) == 1)
	{
		D2Act2Quest3Strc* pQuestDataEx = (D2Act2Quest3Strc*)pQuestData->pQuestDataEx;

		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT2Q3_UnitIterate_StatusCyclerEx, 1);
		if (!pQuestData->fState)
		{
			QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
		}

		pQuestData->pGame = pQuestData->pGame;
		if (pQuestData->pGame->pAct[ACT_II])
		{
			ENVIRONMENT_TaintedSunBegin(pQuestData->pGame->pAct[ACT_II]);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q3_UnitIterate_UpdateClient);
			pQuestDataEx->bRewarded = 1;
		}
		else
		{
			D2GSPacketSrv5D packet5D = {};
			packet5D.field_4 = 0;
			pQuestDataEx->bBlackened = 1;
			packet5D.nHeader = 0x5D;
			packet5D.nQuestId = QUEST_A2Q3_TAINTEDSUN;
			packet5D.nFlags = 1;
			D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pQuestArg->pPlayer, __FILE__, __LINE__), &packet5D, sizeof(packet5D));
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_ENTERAREA))
		{
			pQuestData->fLastState = 2;
			pQuestData->fState = 3;
		}
		else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_LEAVETOWN))
		{
			pQuestData->fLastState = 2;
			pQuestData->fState = 2;
		}
		else
		{
			pQuestData->fLastState = 1;
			pQuestData->fState = 1;
		}
	}
}

//D2Game.0x6FCA1F00
int32_t __fastcall OBJECTS_OperateFunction24_TaintedSunAltar(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q3_TAINTEDSUN);
	if (!pQuestData)
	{
		return 0;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];
	if ((QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDPENDING) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDGRANTED) == 1)
		&& (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED) == 1 || ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' piv') || ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' tsh')))
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
	{
		return 0;
	}

	D2Act2Quest3Strc* pQuestDataEx = (D2Act2Quest3Strc*)pQuestData->pQuestDataEx;

	if (!pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
		pQuestDataEx->nAltarMode = OBJMODE_OPENED;
		pQuestDataEx->nViperAmuletDropCount = 0;
		pQuestDataEx->bAltarDestroyed = 1;
		SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pPlayer, ACT2Q3_UnitIterate_DetermineViperAmuletDropCount);

		D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
		int32_t nItemLevel = DUNGEON_GetLevelIdFromRoom(pRoom);

		int32_t nAmuletsDropped = 0;
		pObject->dwDropItemCode = ' piv';
		for (int32_t i = 0; i < pQuestDataEx->nViperAmuletDropCount; ++i)
		{
			D2UnitStrc* pItem = D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pObject, ITEMQUAL_UNIQUE, &nItemLevel, 0, -1, 0);
			if (pItem)
			{
				ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 1);
				++nAmuletsDropped;
			}
		}
		OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);

		D2SeedStrc* pSeed = QUESTS_GetGlobalSeed(pOp->pGame);

		for (uint32_t i = 0; i < ITEMS_RollRandomNumber(pSeed) % 5 + 5; ++i)
		{
			OBJMODE_DropItemWithCodeAndQuality(pOp->pGame, pOp->pObject, ' dlg', 2);
		}

		D2QuestDataStrc* pQuestDataStaff = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A2Q2_HORADRICSTAFF);
		if (pQuestDataStaff)
		{
			((D2Act2Quest2Strc*)pQuestDataStaff->pQuestDataEx)->nAmulets += nAmuletsDropped;
		}
		return 0;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pOp->pObject);
	pQuestDataEx->pRoom = pRoom;
	pQuestDataEx->nAltarLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPERATING);

	pQuestDataEx->nAltarMode = OBJMODE_OPENED;
	pQuestDataEx->bAltarInitialized = 1;
	pQuestDataEx->nAltarGUID = pObject->dwUnitId;
	pQuestDataEx->bAltarDestroyed = 1;

	if (pQuestData->bNotIntro)
	{
		if (pQuestData->pGame->pAct[ACT_II])
		{
			pQuestDataEx->bRewarded = 0;
			ENVIRONMENT_TaintedSunEnd(pQuestData->pGame->pAct[ACT_II]);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q3_UnitIterate_SendPacket0x53ToClient);
		}

		QUESTS_DebugOutput(pQuestData->pGame, "Finished quest", "..\\D2Game/Quests/a2q3.cpp", 991);
		QUESTS_StateDebug(pQuestData, 4, "..\\D2Game/Quests/a2q3.cpp", 992);
		QUESTS_TriggerFX(pQuestData->pGame, 6);
	}

	pQuestDataEx->nViperAmuletDropCount = 0;
	SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pPlayer, ACT2Q3_UnitIterate_DetermineViperAmuletDropCount);

	if (pQuestData->bNotIntro)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDGRANTED))
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDPENDING);
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_PRIMARYGOALDONE);
			QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_COMPLETEDNOW);
		}

		SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pObject, ACT2Q3_UnitIterate_SetRewardPending);
		SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pObject, ACT2Q3_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
		SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pObject, ACT2Q3_UnitIterate_AttachCompletionSound);
		QUESTS_SetGlobalState(pOp->pGame, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE);
	}

	int32_t nItemLevel = DUNGEON_GetLevelIdFromRoom(pQuestDataEx->pRoom);

	int32_t nAmuletsDropped = 0;
	pObject->dwDropItemCode = ' piv';
	for (int32_t i = 0; i < pQuestDataEx->nViperAmuletDropCount; ++i)
	{
		D2UnitStrc* pItem = D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pObject, ITEMQUAL_UNIQUE, &nItemLevel, 0, -1, 0);
		if (pItem)
		{
			ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 1);
			++nAmuletsDropped;
		}
	}

	D2QuestDataStrc* pQuestDataStaff = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A2Q2_HORADRICSTAFF);
	if (pQuestDataStaff)
	{
		((D2Act2Quest2Strc*)pQuestDataStaff->pQuestDataEx)->nAmulets += nAmuletsDropped;
	}

	OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);

	D2SeedStrc* pSeed = QUESTS_GetGlobalSeed(pOp->pGame);

	for (uint32_t i = 0; i < ITEMS_RollRandomNumber(pSeed) % 5 + 5; ++i)
	{
		OBJMODE_DropItemWithCodeAndQuality(pOp->pGame, pObject, ' dlg', 2);
	}

	SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q3_UnitIterate_SetCompletionFlag);
	QUESTS_CreateTimer(pQuestData, ACT2Q3_Timer_StatusCycler, 10);

	return 0;
}

//D2Game.0x6FCA2320
int32_t __fastcall ACT2Q3_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDPENDING) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A2Q3_TAINTEDSUN, 0);
	}

	return 0;
}

//D2Game.0x6FCA2380
int32_t __fastcall ACT2Q3_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q3, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT2Q3_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCA23D0
int32_t __fastcall ACT2Q3_UnitIterate_SendPacket0x53ToClient(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
	if (!pClient || pClient->nAct != ACT_II)
	{
		return 0;
	}

	D2GSPacketSrv53 pPacket53 = {};
	pPacket53.nHeader = 0x53;
	pPacket53.unk0x01 = 2;
	pPacket53.unk0x05 = 0;
	pPacket53.unk0x09 = 0;
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &pPacket53, sizeof(pPacket53));
	return 0;
}

//D2Game.0x6FCA2420
int32_t __fastcall ACT2Q3_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q3_TAINTEDSUN);
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);

	if (pRoom && pQuestData
		&& ((D2Act2Quest3Strc*)pQuestData->pQuestDataEx)->nAltarLevelId == DUNGEON_GetLevelIdFromRoom(pRoom)
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDGRANTED) != 1
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_COMPLETEDBEFORE) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_PRIMARYGOALDONE);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q3, QFLAG_REWARDPENDING);
	}

	return 0;
}

//D2Game.0x6FCA24A0
bool __fastcall ACT2Q3_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act2Quest3Strc* pQuestDataEx = (D2Act2Quest3Strc*)pQuestData->pQuestDataEx;

	if (pQuestData->fState == 4)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 3, 0, ACT2Q3_UnitIterate_StatusCyclerEx, 1);
	}
	pQuestDataEx->unk0x06 = 0;

	return 1;
}

//D2Game.0x6FCA24D0
int32_t __fastcall ACT2Q3_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q3, QFLAG_PRIMARYGOALDONE))
	{
		SUNIT_AttachSound(pUnit, 52, pUnit);
	}

	return 0;
}

//D2Game.0x6FCA2510
int32_t __fastcall ACT2Q3_UnitIterate_DetermineViperAmuletDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (ITEMS_FindQuestItem(pGame, pUnit, ' piv') || ITEMS_FindQuestItem(pGame, pUnit, ' tsh'))
	{
		return 0;
	}

	if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED))
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q3_TAINTEDSUN);
	if (pQuestData)
	{
		++((D2Act2Quest3Strc*)pQuestData->pQuestDataEx)->nViperAmuletDropCount;
	}

	return 0;
}

//D2Game.0x6FCA2580
void __fastcall ACT2Q3_BrightenEnvironment(D2GameStrc* pGame, uint8_t nActNo)
{
	if (nActNo != ACT_II)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q3_TAINTEDSUN);
	if (!pQuestData)
	{
		return;
	}

	D2Act2Quest3Strc* pQuestDataEx = (D2Act2Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bBlackened != 1)
	{
		return;
	}

	ENVIRONMENT_TaintedSunBegin(pGame->pAct[ACT_II]);
	pQuestDataEx->bBlackened = 0;
	pQuestDataEx->bRewarded = 1;
}
