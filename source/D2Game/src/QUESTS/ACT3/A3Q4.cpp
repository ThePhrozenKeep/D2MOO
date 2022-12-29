#include "QUESTS/ACT3/A3Q4.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <D2Items.h>
#include <D2QuestRecord.h>

#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FD39318
D2NPCMessageTableStrc gpAct3Q4NpcMessages[] =
{
	{
		{
			{ MONSTER_CAIN3, 527, 0, 1 },
			{ MONSTER_ASHEARA, 528, 0, 2 },
		},
		2
	},
	{
		{
			{ MONSTER_MESHIF2, 529, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN3, 531, 0, 2 },
			{ MONSTER_ALKOR, 534, 0, 1 },
			{ MONSTER_NATALYA, 530, 0, 2 },
			{ MONSTER_HRATLI, 532, 0, 2 },
			{ MONSTER_ASHEARA, 533, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_CAIN3, 531, 0, 1 },
			{ MONSTER_HRATLI, 532, 0, 2 },
			{ MONSTER_ASHEARA, 533, 0, 2 },
		},
		3
	},
	{
		{
			{ MONSTER_ALKOR, 534, 0, 2 },
			{ MONSTER_ORMUS, 535, 0, 2 },
			{ MONSTER_HRATLI, 536, 0, 2 },
			{ MONSTER_NATALYA, 537, 0, 2 },
		},
		4
	},
	{
		{
			{ MONSTER_ALKOR, 538, 0, 1 },
			{ MONSTER_MESHIF2, 539, 0, 2 },
			{ MONSTER_CAIN3, 540, 0, 2 },
			{ MONSTER_ORMUS, 541, 0, 2 },
		},
		4
	},
	{
		{
			{ MONSTER_ALKOR, 538, 0, 2 },
			{ MONSTER_MESHIF2, 539, 0, 2 },
			{ MONSTER_CAIN3, 540, 0, 2 },
			{ MONSTER_ORMUS, 541, 0, 2 },
			{ MONSTER_NATALYA, 542, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_CAIN3, 527, 0, 2 },
			{ MONSTER_ASHEARA, 528, 0, 2 },
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


//D2Game.0x6FCA9FF0
bool __fastcall ACT3Q4_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING))
	{
		return false;
	}

	switch (nNpcId)
	{
	case MONSTER_ALKOR:
		if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 43g'))
		{
			return true;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING))
		{
			return true;
		}
		break;

	case MONSTER_MESHIF2:
		if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 43j'))
		{
			return true;
		}
		break;

	case MONSTER_CAIN3:
		if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 43j') && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_STARTED))
		{
			return true;
		}

		if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 43g') && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_ENTERAREA))
		{
			return true;
		}
		break;
	}

	return false;
}

//D2Game.0x6FCAA0F0
bool __fastcall ACT3Q4_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	if (!QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A2COMPLETED, QFLAG_REWARDGRANTED))
	{
		*pStatus = 0;
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING))
	{
		*pStatus = 5;
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED))
	{
		*pStatus = 2 * (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q4, QFLAG_PRIMARYGOALDONE) != 0) + 11;
		return true;
	}

	*pStatus = 0;

	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 43g'))
	{
		*pStatus = (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q4, QFLAG_ENTERAREA) != 0) + 3;
		return true;
	}

	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' 43j'))
	{
		*pStatus = (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A3Q4, QFLAG_STARTED) != 0) + 1;
		return true;
	}

	if (!pQuest->bNotIntro)
	{
		return true;
	}

	if (pQuest->fLastState >= 6)
	{
		*pStatus = (pQuest->pGame->nGameType != 3) + 6;
	}
	else
	{
		*pStatus = pQuest->fLastState;
	}

	return true;
}

//D2Game.0x6FCAA210
void __fastcall ACT3Q4_UnitIterate_UpdateCurrentQuestState(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q4_GOLDENBIRD);
	if (!pQuestData)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, ((D2Act3Quest4Strc*)pQuestData->pQuestDataEx)->nCurrentQuestFlag);
	}
}

//D2Game.0x6FCAA270
void __fastcall ACT3Q4_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pNPCMessages = gpAct3Q4NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->fState = 0;
	pQuestData->nQuest = QUESTSTATEFLAG_A3Q4;
	pQuestData->pfActiveFilter = ACT3Q4_ActiveFilterCallback;
	pQuestData->nInitNo = 0;
	pQuestData->pfStatusFilter = ACT3Q4_StatusFilterCallback;
	pQuestData->pfSeqFilter = ACT3Q4_SeqCallback;
	pQuestData->nSeqId = 16;
	pQuestData->pfCallback[QUESTEVENT_ITEMPICKEDUP] = ACT3Q4_Callback04_ItemPickedUp;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT3Q4_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT3Q4_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT3Q4_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT3Q4_Callback13_PlayerStartedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT3Q4_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERJOINEDGAME] = ACT3Q4_Callback14_PlayerJoinedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM] = ACT3Q4_Callback09_PlayerDroppedWithQuestItem;
	QUESTS_ResetPlayerGUIDCount(&pQuestData->pGUID);

	D2Act3Quest4Strc* pQuestDataEx = (D2Act3Quest4Strc*)FOG_AllocPool(pQuestData->pGame->pMemoryPool, sizeof(D2Act3Quest4Strc), __FILE__, __LINE__, 0);
	memset(pQuestDataEx, 0x00, sizeof(D2Act3Quest4Strc));
	pQuestData->pQuestDataEx = pQuestDataEx;

	pQuestDataEx->unk0x0C = 1;
	pQuestDataEx->unk0x01 = 1;
}

//D2Game.0x6FCAA360
void __fastcall ACT3Q4_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED) == 1)
	{
		if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 6);
		}
		return;
	}

	if (nNpcId == MONSTER_ALKOR && ((D2Act3Quest4Strc*)pQuestData->pQuestDataEx)->bGoldenBirdBroughtToAlkor == 1)
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 5);
		return;
	}

	if (!ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43g'))
	{
		if (!ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43j'))
		{
			return;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_STARTED))
		{
			if (nNpcId == MONSTER_CAIN3 || nNpcId == MONSTER_ASHEARA)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 7);
				return;
			}
		}
		else
		{
			if (nNpcId == MONSTER_CAIN3 || nNpcId == MONSTER_ASHEARA)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 0);
				return;
			}
		}
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 1);
		return;
	}

	if (nNpcId == MONSTER_CAIN3)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_ENTERAREA))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN3, 3);
			return;
		}
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
		return;
	}

	if (nNpcId != MONSTER_ALKOR)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
		return;
	}

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_ALKOR, 2);
	}
}

//D2Game.0x6FCAA510
void __fastcall ACT3Q4_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nOldLevel == LEVEL_KURASTDOCKTOWN)
	{
		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);
	}
}

//D2Game.0x6FCAA520
void __fastcall ACT3Q4_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED) == 1)
	{
		return;
	}

	D2Act3Quest4Strc* pQuestDataEx = (D2Act3Quest4Strc*)pQuestData->pQuestDataEx;
	switch (pQuestArg->nNPCNo)
	{
	case MONSTER_CAIN3:
	{
		if (pQuestArg->nMessageIndex == 527)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_STARTED);
			if (pQuestData->fState == 1)
			{
				QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
				pQuestDataEx->bTalkedToCainOnce = 1;
				pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT3Q4_Callback02_NpcDeactivate;
			}
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
			pQuestDataEx->nCurrentQuestFlag = QFLAG_STARTED;

			const int16_t nPartyId = SUNIT_GetPartyId(pQuestArg->pPlayer);
			if (nPartyId != -1)
			{
				PARTY_IteratePartyMembers(pQuestArg->pGame, nPartyId, ACT3Q4_UnitIterate_UpdateCurrentQuestState, 0);
			}
		}
		else if (pQuestArg->nMessageIndex == 531)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_ENTERAREA);
			pQuestDataEx->bTalkedToCainTwice = 1;
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
			pQuestDataEx->nCurrentQuestFlag = QFLAG_ENTERAREA;
			
			const int16_t nPartyId = SUNIT_GetPartyId(pQuestArg->pPlayer);
			if (nPartyId != -1)
			{
				PARTY_IteratePartyMembers(pQuestArg->pGame, nPartyId, ACT3Q4_UnitIterate_UpdateCurrentQuestState, 0);
			}
		}
		break;
	}
	case MONSTER_ALKOR:
	{
		if (pQuestArg->nMessageIndex == 534)
		{
			if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43g')
				&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING)
				&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED))
			{
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43g');
				--pQuestDataEx->nGoldenBirdsInGame;
				if (pQuestData->bNotIntro && pQuestData->fState != 4)
				{
					QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
				}

				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING);
				pQuestDataEx->bTalkedToAlkor = 1;
				pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT3Q4_Callback02_NpcDeactivate;
				pQuestDataEx->nCurrentQuestFlag = QFLAG_REWARDPENDING;
				
				const int16_t nPartyId = SUNIT_GetPartyId(pQuestArg->pPlayer);
				if (nPartyId != -1)
				{
					PARTY_IteratePartyMembers(pQuestArg->pGame, nPartyId, ACT3Q4_UnitIterate_UpdateCurrentQuestState, 0);
				}
			}
		}
		else if (pQuestArg->nMessageIndex == 538)
		{
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED))
			{
				if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING))
				{
					QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING);
					QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED);
					QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_PRIMARYGOALDONE);
					QUESTRECORD_ResetIntermediateStateFlags(pQuestFlags, pQuestData->nQuest);
					QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_CUSTOM1);
					if (pQuestData->bNotIntro)
					{
						QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
					}

					QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, ' zyx', 0, ITEMQUAL_NORMAL, 1);

					QUESTS_UpdatePlayerFlags(pQuestArg->pGame, pQuestArg->pPlayer);

					QUESTS_AddPlayerGUID(&pQuestData->pGUID, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));
				}

				if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_PRIMARYGOALDONE))
				{
					QUESTS_SetGlobalState(pQuestArg->pGame, QUESTSTATEFLAG_A3Q4, QFLAG_PRIMARYGOALDONE);
					if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
					{
						FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
						exit(-1);
					}
					pQuestData->pfSeqFilter(pQuestData);
				}
			}
		}
		break;
	}
	case MONSTER_MESHIF2:
	{
		if (pQuestArg->nMessageIndex == 529)
		{
			if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43j'))
			{
				pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT3Q4_Callback02_NpcDeactivate;
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43j');
				--pQuestDataEx->nGoldenBirdsInGame;
				if (QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43g', 0, ITEMQUAL_NORMAL, 1))
				{
					++pQuestDataEx->nGoldenBirdsInGame;
					QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
					pQuestDataEx->bTalkedToMeshif = 1;
					pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT3Q4_Callback02_NpcDeactivate;
				}
			}
		}
		break;
	}
	}
}

//D2Game.0x6FCAA8D0
void __fastcall ACT3Q4_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	D2Act3Quest4Strc* pQuestDataEx = (D2Act3Quest4Strc*)pQuestData->pQuestDataEx;

	if (nNpcId == MONSTER_ALKOR)
	{
		if (pQuestDataEx->bTalkedToAlkor == 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 5, 0, ACT3Q4_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->bTalkedToAlkor = 0;
			pQuestDataEx->bGoldenBirdBroughtToAlkor = 1;
		}
	}
	else if (nNpcId == MONSTER_CAIN3)
	{
		if (pQuestDataEx->bTalkedToCainOnce == 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 2, 0, ACT3Q4_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->bTalkedToCainOnce = 0;
		}

		if (pQuestDataEx->bTalkedToCainTwice == 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 4, 0, ACT3Q4_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->bTalkedToCainTwice = 0;
		}
	}
	else if (nNpcId == MONSTER_MESHIF2)
	{
		if (pQuestDataEx->bTalkedToMeshif == 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 3, 0, ACT3Q4_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->bTalkedToMeshif = 0;
		}
	}
}

//D2Game.0x6FCAA990
int32_t __fastcall ACT3Q4_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A3Q4_GOLDENBIRD);
	}

	return 0;
}

//D2Game.0x6FCAAA00
void __fastcall ACT3Q4_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING))
	{
		return;
	}

	QUESTS_DebugOutput(pQuestData->pGame, "something acquired", __FILE__, __LINE__);

	uint32_t dwItemId = -1;
	if (pQuestArg->pTarget)
	{
		dwItemId = pQuestArg->pTarget->dwClassId;
	}

	const uint32_t dwItemCode = DATATBLS_GetItemsTxtRecord(dwItemId)->dwCode;
	if (dwItemCode == ' 43g')
	{
		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		QUESTS_UnitIterate(pQuestData, 3, 0, ACT3Q4_UnitIterate_StatusCyclerEx, 1);
	}
	else if (dwItemCode == ' 43j')
	{
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT3Q4_UnitIterate_StatusCyclerEx, 1);
		QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_CUSTOM2))
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_CUSTOM2);
			SUNIT_AttachSound(pQuestArg->pPlayer, 72, pQuestArg->pPlayer);
		}
	}
}

//D2Game.0x6FCAAB10
bool __fastcall ACT3Q4_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (pQuestData->fState != 5 && pQuestData->bNotIntro)
	{
		return true;
	}

	bool bResult = false;
	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, pQuestData->nSeqId);
	if (pQuest)
	{
		if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
		{
			FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
			exit(-1);
		}
		bResult = pQuestData->pfSeqFilter(pQuest);
	}

	D2QuestDataStrc* pQuestData17 = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A3Q3_GIDBINN);
	if (pQuestData17)
	{
		if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
		{
			FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
			exit(-1);
		}
		return pQuestData17->pfSeqFilter(pQuestData17) || bResult;
	}
	
	return bResult;
}

//D2Game.0x6FCAABE0
void __fastcall ACT3Q4_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	D2Act3Quest4Strc* pQuestDataEx = (D2Act3Quest4Strc*)pQuestData->pQuestDataEx;

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_REWARDPENDING))
	{
		pQuestData->bNotIntro = 0;
		pQuestDataEx->unk0x01 = 0;
		pQuestDataEx->unk0x0C = 0;
		return;
	}

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43g'))
	{
		pQuestDataEx->unk0x01 = 0;
		pQuestDataEx->unk0x0C = 0;
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_ENTERAREA))
		{
			pQuestData->fState = 3;
			pQuestData->fLastState = 4;
		}
		else
		{
			pQuestData->fState = 3;
			pQuestData->fLastState = 3;
		}
	}
	else if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43j'))
	{
		pQuestDataEx->unk0x01 = 0;
		pQuestDataEx->unk0x0C = 0;
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_STARTED))
		{
			pQuestData->fState = 2;
			pQuestData->fLastState = 2;
		}
		else
		{
			pQuestData->fState = 1;
			pQuestData->fLastState = 1;
		}
	}
}

//D2Game.0x6FCAACE0
void __fastcall ACT3Q4_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act3Quest4Strc* pQuestDataEx = (D2Act3Quest4Strc*)pQuestData->pQuestDataEx;

	--pQuestDataEx->nGoldenBirdsInGame;
	if (!pQuestDataEx->nGoldenBirdsInGame && pQuestData->bNotIntro && pQuestDataEx->bJadeFigurineDropped && pQuestData->fState < 4)
	{
		pQuestDataEx->bPlayerDroppedWithItem = 1;
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 6, 0, ACT3Q4_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCAAD20
void __fastcall ACT3Q4_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act3Quest4Strc* pQuestDataEx = (D2Act3Quest4Strc*)pQuestData->pQuestDataEx;
	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43j'))
	{
		++pQuestDataEx->nGoldenBirdsInGame;
	}

	bool bGoldenBirdAcquired = false;
	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 43g'))
	{
		bGoldenBirdAcquired = true;
		++pQuestDataEx->nGoldenBirdsInGame;
	}

	if (pQuestDataEx->nGoldenBirdsInGame != 1 || !pQuestData->bNotIntro || !pQuestDataEx->bPlayerDroppedWithItem)
	{
		return;
	}

	pQuestDataEx->bPlayerDroppedWithItem = 0;
	pQuestData->dwFlags &= 0xFFFFFF00;

	if (bGoldenBirdAcquired)
	{
		QUESTS_UnitIterate(pQuestData, 3, 0, ACT3Q4_UnitIterate_StatusCyclerEx, 1);
	}
	else
	{
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT3Q4_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCAADA0
int32_t __fastcall ACT3Q4_UnitIterate_SetGoldenBirdBoss(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit)
{
	D2Act3Quest4Strc* pQuestDataEx = (D2Act3Quest4Strc*)pQuestData->pQuestDataEx;
	if (!pQuestData->bNotIntro || !pQuestDataEx->unk0x0C || pQuestDataEx->unk0x01 != 1)
	{
		return 0;
	}
	
	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A3Q3_GIDBINN);
	if (pQuest && ((D2Act3Quest3Strc*)pQuest->pQuestDataEx)->bBossIsSpawning)
	{
		return 0;
	}

	QUESTS_CreateChainRecord(pQuestData->pGame, pUnit, QUEST_A3Q4_GOLDENBIRD);
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT3Q4_Callback08_MonsterKilled;
	pQuestDataEx->unk0x01 = 0;
	pQuestDataEx->unk0x02 = 1;

	if (pUnit)
	{
		pQuestDataEx->nBossGUID = pUnit->dwUnitId;
	}
	else
	{
		pQuestDataEx->nBossGUID = -1;
	}
	return 1;
}

//D2Game.0x6FCAAE20
void __fastcall ACT3Q4_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_DebugOutput(pQuestData->pGame, "killed boss for quest", __FILE__, __LINE__);

	D2Act3Quest4Strc* pQuestDataEx = (D2Act3Quest4Strc*)pQuestData->pQuestDataEx;
	if (!pQuestData->bNotIntro || !pQuestDataEx->unk0x02 || !pQuestDataEx->unk0x0C)
	{
		return;
	}

	if (pQuestArg->pPlayer && QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuest, QFLAG_REWARDGRANTED))
	{
		return;
	}

	pQuestArg->pTarget->dwDropItemCode = ' 43j';
	int32_t nItemLevel = 0;
	if (D2GAME_DropItemAtUnit_6FC4FEC0(pQuestData->pGame, pQuestArg->pTarget, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0))
	{
		pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = nullptr;
		pQuestDataEx->unk0x0C = 0;
		pQuestDataEx->bJadeFigurineDropped = 1;
		++pQuestDataEx->nGoldenBirdsInGame;
		if (pQuestData->fState < 1)
		{
			QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
		}

		if (!pQuestData->fLastState)
		{
			QUESTS_UnitIterate(pQuestData, 1, 0, ACT3Q4_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->unk0x02 = 0;
			return;
		}
	}
	else
	{
		pQuestDataEx->unk0x01 = 1;
	}
	pQuestDataEx->unk0x02 = 0;
}

//D2Game.0x6FCAAF60
bool __fastcall ACT3Q4_GoldenBirdBroughtToAlkor(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q4_GOLDENBIRD);

	return pQuestData && ((D2Act3Quest4Strc*)pQuestData->pQuestDataEx)->bGoldenBirdBroughtToAlkor;
}

//D2Game.0x6FCAAF80
void __fastcall ACT3Q4_ResetAlkor(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q4_GOLDENBIRD);

	if (pQuestData)
	{
		((D2Act3Quest4Strc*)pQuestData->pQuestDataEx)->bGoldenBirdBroughtToAlkor = 0;
	}
}

//
void __fastcall ACT3Q4_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_RemovePlayerGUID(pQuestData, pQuestArg);
}
