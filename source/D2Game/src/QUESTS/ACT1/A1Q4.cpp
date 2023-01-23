#include "QUESTS/ACT1/A1Q4.h"

#include <algorithm>

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2QuestRecord.h>

#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterSpawn.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitInactive.h"
#include "SKILLS/Skills.h"


//D2Game.0x6FD32EF0
D2NPCMessageTableStrc gpAct1Q4NpcMessages[] =
{
	{
		{
			{ MONSTER_AKARA, 97, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_AKARA, 99, 0, 2 },
			{ MONSTER_KASHYA, 98, 0, 2 },
			{ MONSTER_CHARSI, 100, 0, 2 },
			{ MONSTER_GHEED, 102, 0, 2 },
			{ MONSTER_WARRIV1, 101, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_KASHYA, 105, 0, 2 },
			{ MONSTER_WARRIV1, 107, 0, 2 },
			{ MONSTER_CHARSI, 103, 0, 2 },
			{ MONSTER_AKARA, 104, 0, 2 },
			{ MONSTER_GHEED, 106, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_KASHYA, 108, 0, 2 },
			{ MONSTER_WARRIV1, 111, 0, 2 },
			{ MONSTER_CHARSI, 109, 0, 2 },
			{ MONSTER_AKARA, 112, 0, 0 },
			{ MONSTER_GHEED, 110, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_KASHYA, 113, 0, 2 },
			{ MONSTER_WARRIV1, 116, 0, 2 },
			{ MONSTER_CHARSI, 114, 0, 2 },
			{ MONSTER_AKARA, 117, 0, 2 },
			{ MONSTER_GHEED, 115, 0, 2 },
		},
		5
	},
	{
		{
			{ MONSTER_KASHYA, 119, 0, 2 },
			{ MONSTER_WARRIV1, 122, 0, 2 },
			{ MONSTER_CHARSI, 121, 0, 2 },
			{ MONSTER_CAIN5, 123, 0, 1 },
			{ MONSTER_AKARA, 118, 0, 1 },
			{ MONSTER_GHEED, 120, 0, 2 },
		},
		6
	},
	{
		{
			{ MONSTER_CAIN5, 125, 0, 1 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN5, 123, 0, 2 },
			{ MONSTER_AKARA, 118, 0, 2 },
			{ MONSTER_GHEED, 120, 0, 2 },
		},
		3
	},
	{
		{
			{ MONSTER_CAIN5, 125, 0, 2 },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN1, 124, 0, 1 },
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


//D2Game.0x6FC99D00
void __fastcall ACT1Q4_UpdateObjects(D2QuestDataStrc* pQuestData, char a2, char a3)
{
	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	D2UnitStrc* pCage = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nCainGibbetGUID);

	if (pQuestDataEx->bCageModeChanged != 1 || pCage->dwClassId != OBJECT_CAINGIBBET)
	{
		pQuestDataEx->nCainGibbetObjectMode = OBJMODE_SPECIAL1;
	}
	else
	{
		if (pCage->dwAnimMode != OBJMODE_SPECIAL1)
		{
			UNITS_ChangeAnimMode(pCage, OBJMODE_SPECIAL1);
			pQuestDataEx->nCainGibbetObjectMode = OBJMODE_SPECIAL1;
		}
	}

	if (pQuestDataEx->bInifussTreeInitialized == 1)
	{
		D2UnitStrc* pInifussTree = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nInifussTreeGUID);
		if (pInifussTree)
		{
			UNITS_ChangeAnimMode(pInifussTree, OBJMODE_OPERATING);
		}
	}

	if (a2)
	{
		if (!pQuestDataEx->bTimerActive)
		{
			pQuestDataEx->bTimerActive = 1;
			QUESTS_CreateTimer(pQuestData, ACT1Q4_Timer_RemoveCainFromTristram, 1);
		}
	}
	else
	{
		if (!a3)
		{
			pQuestDataEx->unk0x50 = 1;
			return;
		}
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 1, 0, ACT1Q4_UnitIterate_RemoveCainFromTristram);
	}

	if (!a3)
	{
		pQuestDataEx->unk0x50 = 1;
		return;
	}

	if (!pQuestDataEx->bCainRemovedFromTristram)
	{
		SUNITINACTIVE_DeleteSingleListNode(pQuestData->pGame, UNIT_MONSTER, MONSTER_CAIN1, ACT_I);
	}

	pQuestDataEx->unk0x50 = 1;
	if (!pQuestDataEx->bCainInTownSpawned)
	{
		pQuestDataEx->unk0x52 = 1;
	}
}

//D2Game.0x6FC99DE0
int32_t __fastcall ACT1Q4_UnitIterate_RemoveCainFromTristram(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!pUnit || pUnit->dwClassId != MONSTER_CAIN1)
	{
		return 0;
	}

	D2MonsterInteractStrc* pMonInteract = nullptr;
	if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
	{
		pMonInteract = pUnit->pMonsterData->pMonInteract;
	}

	if (MONSTERAI_HasInteractUnit(pMonInteract))
	{
		D2GAME_MONSTERAI_Last_6FC61F70(pGame, pUnit, ACT1Q4_SendPacket0x5DToClient);
		return 1;
	}

	D2ModeChangeStrc modeChange = {};
	MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_DEATH, &modeChange);
	D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	if (pQuestData)
	{
		((D2Act1Quest4Strc*)pQuestData->pQuestDataEx)->bCainRemovedFromTristram = 1;
	}

	return 1;
}

//D2Game.0x6FC99E80
void __fastcall ACT1Q4_SendPacket0x5DToClient(D2UnitStrc* pUnit)
{
	D2GSPacketSrv5D packet5D = {};

	packet5D.nHeader = 0x5D;
	packet5D.nQuestId = QUEST_A1Q4_CAIN;
	packet5D.nFlags = 1;

	D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet5D, sizeof(packet5D));
}

//D2Game.0x6FC99EC0
void __fastcall ACT1Q4_SetMonolithOrder(D2QuestDataStrc* pQuestData)
{
	const int32_t nMonolithClassIds[5] = { 17, 18, 19, 20, 21 };

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;

	pQuestDataEx->nStoneOrder[0] = 0;
	pQuestDataEx->nStoneOrder[1] = 0;
	pQuestDataEx->nStoneOrder[2] = 0;
	pQuestDataEx->nStoneOrder[3] = 0;
	pQuestDataEx->nStoneOrder[4] = 0;

	pQuestDataEx->nCurrentMonolithNo = 0;

	D2SeedStrc* pSeed = QUESTS_GetGlobalSeed(pQuestData->pGame);
	for (int32_t i = 0; i < 5; ++i)
	{
		const uint32_t nRand = ITEMS_RollRandomNumber(pSeed) % 5;
		if (!pQuestDataEx->nStoneOrder[nRand])
		{
			char szMessage[128] = {};
			sprintf(szMessage, "stone %d is class %d", nRand, nMonolithClassIds[i]);
			QUESTS_DebugOutput(pQuestData->pGame, szMessage, __FILE__, __LINE__);
			pQuestDataEx->nStoneOrder[nRand] = nMonolithClassIds[i];
		}
	}
}

//D2Game.0x6FC99F90
void __fastcall ACT1Q4_SetInvisibleObjectGUID(D2GameStrc* pGame, D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit)
{
	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;

	pQuestDataEx->bInvisbleObjectInitialized = 1;
	if (pUnit)
	{
		pQuestDataEx->nInvisibleObjectGUID = pUnit->dwUnitId;
	}
	else
	{
		pQuestDataEx->nInvisibleObjectGUID = -1;
	}
}

//D2Game.0x6FC99FC0
bool __fastcall ACT1Q4_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId == MONSTER_AKARA)
	{
		if (pQuest->fState == 4 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING))
		{
			if (ITEMS_FindQuestItem(pPlayer->pGame, pPlayer, ' skb'))
			{
				return true;
			}
		}

		if (pQuest->fState == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING))
		{
			return true;
		}

		if (pQuest->fState == 6 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE) == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED))
		{
			return true;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING))
		{
			return true;
		}
	}
	else if (nNpcId == MONSTER_CAIN5)
	{
		int32_t nPlayerId = -1;
		if (pPlayer)
		{
			nPlayerId = pPlayer->dwUnitId;
		}

		D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuest->pQuestDataEx;
		if (QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->pQuestGUID, nPlayerId) != 1 && !QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->tPlayerGUIDs, nPlayerId) && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE))
		{
			return true;
		}
	}

	return false;
}

//D2Game.0x6FC9A0E0
void __fastcall ACT1Q4_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING) == 1)
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

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING);
	QUESTS_UpdatePlayerFlags(pGame, pUnit);
}

//D2Game.0x6FC9A160
bool __fastcall ACT1Q4_Timer_RemoveCainFromTristram(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	SUNIT_IterateUnitsOfType(pQuestData->pGame, 1, 0, ACT1Q4_UnitIterate_RemoveCainFromTristram);
	((D2Act1Quest4Strc*)pQuestData->pQuestDataEx)->bTimerActive = 0;
	return true;
}

//D2Game.0x6FC9A190
void __fastcall ACT1Q4_SpawnCainInTristram(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	if (!pQuestData)
	{
		FOG_DisplayWarning("0", __FILE__, __LINE__);
		return;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	if (!pQuestData->bNotIntro || pQuestDataEx->unk0x50 == 1)
	{
		return;
	}

	pQuestDataEx->nCainGibbetObjectMode = OBJMODE_SPECIAL1;
	UNITS_ChangeAnimMode(pUnit, OBJMODE_SPECIAL1);

	const int32_t nX = CLIENTS_GetUnitX(pUnit) + 3;
	const int32_t nY = CLIENTS_GetUnitY(pUnit) + 3;

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	D2UnitStrc* pCain = sub_6FC69F10(pGame, pRoom, nX, nY, MONSTER_CAIN1, 1, -1, 0);
	if (!pCain)
	{
		D2CoordStrc pCoord = {};
		pCoord.nX = nX;
		pCoord.nY = nY;

		D2RoomStrc* pFreeRoom = nullptr;
		QUESTS_GetFreePosition(pRoom, &pCoord, 2, 0x100, &pFreeRoom, 3);
		if (pFreeRoom)
		{
			pCain = sub_6FC69F10(pGame, pFreeRoom, pCoord.nX, pCoord.nY, MONSTER_CAIN1, 1, -1, 0);
		}
	}

	if (pCain)
	{
		pCain->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
		D2UnitStrc* pPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pQuestDataEx->nPlayerGUID);
		if (pPlayer)
		{
			SUNIT_AttachSound(pPlayer, 48, 0);
		}
	}
	else
	{
		QUESTS_DebugOutput(pGame, "Cain not created, look in town, dammit\n", __FILE__, __LINE__);
		pQuestDataEx->pPlayerInTristram = nullptr;
		SUNIT_IterateUnitsOfType(pGame, 0, pUnit, ACT1Q4_UnitIterate_FindPlayersInTristram);

		if (pQuestDataEx->pPlayerInTristram && !pQuestDataEx->bPortalFromTristramToTownSpawned)
		{
			if (D2GAME_CreatePortalObject_6FD13DF0(pGame, pQuestDataEx->pPlayerInTristram, pRoom, nX + 3, nY + 3, LEVEL_ROGUEENCAMPMENT, 0, OBJECT_TOWN_PORTAL, 0))
			{
				pQuestDataEx->bPortalFromTristramToTownSpawned = 1;
			}
		}

		if (!pQuestDataEx->bCainInTownSpawned)
		{
			pQuestDataEx->unk0x52 = 1;
		}

		pQuestDataEx->bCainNotCreated = 1;
	}

	SUNIT_IterateUnitsOfType(pGame, 0, pUnit, ACT1Q4_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
	SUNIT_IterateUnitsOfType(pGame, 0, pUnit, ACT1Q4_UnitIterate_SetCompletionFlag);
	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 6, 0, ACT1Q4_UnitIterate_StatusCyclerEx, 1);
}

//D2Game.0x6FC9A3D0
int32_t __fastcall ACT1Q4_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A1Q4_CAIN);
	}

	return 0;
}

//D2Game.0x6FC9A440
int32_t __fastcall ACT1Q4_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_TRISTRAM)
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING);
	QUESTS_UpdatePlayerFlags(pGame, pUnit);

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT1Q4_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FC9A4D0
int32_t __fastcall ACT1Q4_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_COMPLETEDNOW);
	QUESTS_SendLogUpdateEx(pUnit, QUEST_A1Q4_CAIN, 0);
	return 0;
}

//D2Game.0x6FC9A530
int32_t __fastcall ACT1Q4_UnitIterate_FindPlayersInTristram(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);

	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_TRISTRAM)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	if (!pQuestData)
	{
		return 0;
	}

	((D2Act1Quest4Strc*)pQuestData->pQuestDataEx)->pPlayerInTristram = pUnit;
	return 1;
}

//D2Game.0x6FC9A580
int32_t __fastcall OBJECTS_OperateFunction10_CainGibbet(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q4_CAIN);
	D2Act1Quest4Strc* pQuestDataEx = nullptr;
	if (pQuestData)
	{
		pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
		if (!pQuestData->bNotIntro || pQuestDataEx->unk0x50 == 1 || pQuestData->fState >= 6)
		{
			return 0;
		}
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED))
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);

	if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
	{
		return 0;
	}

	UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
	EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, pOp->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);

	if (pQuestDataEx)
	{
		pQuestDataEx->nCainGibbetObjectMode = OBJMODE_SPECIAL1;

		D2UnitStrc* pPlayer = pOp->pPlayer;
		int32_t nPlayerId = -1;
		if (pPlayer)
		{
			nPlayerId = pPlayer->dwUnitId;
		}
		pQuestDataEx->nPlayerGUID = nPlayerId;
	}

	EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_MONUMOD, pOp->pGame->dwGameFrame + 17, 0, 0);
	DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pObject), 0);

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING);

	QUESTS_UpdatePlayerFlags(pOp->pGame, pOp->pPlayer);

	const int16_t nPartyId = SUNIT_GetPartyId(pOp->pPlayer);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pOp->pGame, nPartyId, ACT1Q4_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FC9A6E0
void __fastcall OBJECTS_InitFunction06_CairnStone(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q4_CAIN);
	if (pQuestData)
	{
		int32_t nClassId = -1;
		if (pOp->pObject)
		{
			nClassId = pOp->pObject->dwClassId;
		}

		D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
		if (!pQuestData->bNotIntro || pQuestDataEx->unk0x4C)
		{
			pQuestDataEx->unk0x4C = 0;

			if (!pQuestDataEx->bPortalToTristramOpened && nClassId == OBJECT_STONEALPHA)
			{
				if (pOp->pObject)
				{
					pQuestDataEx->nCairnStoneAlphaGUID = pOp->pObject->dwUnitId;
				}
				else
				{
					pQuestDataEx->nCairnStoneAlphaGUID = -1;
				}

				if (!pQuestDataEx->bOpenPortalTimerActive)
				{
					pQuestDataEx->bOpenPortalTimerActive = 1;
					QUESTS_CreateTimer(pQuestData, ACT1Q4_OpenPortalToTristram, 1);
				}
			}
			UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
		}
		else
		{
			if (pQuestDataEx->bActivateCairnStones)
			{
				UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
			}
			else if (pQuestDataEx->unk0x50)
			{
				UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
			}
			else if (pQuestDataEx->bCairnStonesActive[nClassId - 17] == 1)
			{
				pQuestDataEx->bCairnStonesActive[nClassId - 17] = 0;
				UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_NEUTRAL);
			}
		}
	}
	else
	{
		if (!pOp->pObject || pOp->pObject->dwAnimMode != OBJMODE_OPENED)
		{
			UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
		}
	}
}

//D2Game.0x6FC9A7C0
bool __fastcall ACT1Q4_OpenPortalToTristram(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	D2UnitStrc* pCairnStone = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nCairnStoneAlphaGUID);

	if (!pCairnStone)
	{
		pQuestDataEx->bOpenPortalTimerActive = 0;
		return 1;
	}

	if (D2GAME_CreatePortalObject_6FD13DF0(pGame, 0, UNITS_GetRoom(pCairnStone), CLIENTS_GetUnitX(pCairnStone) + 4, CLIENTS_GetUnitY(pCairnStone) + 4, LEVEL_TRISTRAM, 0, OBJECT_PERMANENT_TOWN_PORTAL, 1))
	{
		pQuestDataEx->bPortalToTristramOpened = 1;
		pQuestDataEx->bOpenPortalTimerActive = 0;
		return 1;
	}

	return 0;
}

//D2Game.0x6FC9A870
void __fastcall ACT1Q4_UnitIterate_SetEnterAreaFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (nLevelId && DRLG_GetActNoFromLevelId(nLevelId) == ACT_I)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_ENTERAREA);
	}
}

//D2Game.0x6FC9A8D0
int32_t __fastcall OBJECTS_OperateFunction09_Monolith(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q4_CAIN);
	D2_ASSERT(pQuestData);

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	if (!pQuestDataEx->bMonolithStonesOrdered)
	{
		pQuestDataEx->bMonolithStonesOrdered = 1;
		ACT1Q4_SetMonolithOrder(pQuestData);
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING) == 1)
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	if (!ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' dkb'))
	{
		if (!(pQuestDataEx->nCairnStoneInteractions % 64) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_LEAVETOWN) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_ENTERAREA))
		{
			SUNIT_AttachSound(pOp->pPlayer, 39, pOp->pPlayer);
		}

		++pQuestDataEx->nCairnStoneInteractions;
		return 0;
	}

	if (!pQuestData->bNotIntro || pQuestData->fState >= 6)
	{
		return 0;
	}

	D2UnitStrc* pInvisibleObject = nullptr;
	if (pQuestDataEx->bInvisbleObjectInitialized == 1)
	{
		pInvisibleObject = SUNIT_GetServerUnit(pOp->pGame, UNIT_OBJECT, pQuestDataEx->nInvisibleObjectGUID);
		if (!pQuestDataEx->bDecipheredScrollAcquired && pInvisibleObject && pInvisibleObject->dwClassId == OBJECT_INVISIBLE_OBJECT)
		{
			if (!pQuestData->fState && pQuestData->bNotIntro == 1)
			{
				QUESTS_DebugOutput(pQuestData->pGame, "Started quest", "..\\D2Game/Quests/a1q4.cpp", 599);
				QUESTS_StateDebug(pQuestData, 1, "..\\D2Game/Quests/a1q4.cpp", 1939);
			}

			if (!ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' dkb'))
			{
				return 0;
			}
		}
	}

	if (pQuestDataEx->bOperatedWithLastCairnStone == 1)
	{
		return 0;
	}

	if (pQuestData->fState != 5)
	{
		if (!pQuestData->fState && pQuestData->bNotIntro == 1)
		{
			QUESTS_DebugOutput(pQuestData->pGame, "Started quest", "..\\D2Game/Quests/a1q4.cpp", 599);
			QUESTS_StateDebug(pQuestData, 1, "..\\D2Game/Quests/a1q4.cpp", 1957);
		}

		if (!ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' dkb'))
		{
			return 0;
		}
		pQuestData->fState = 5;
	}

	if ((pOp->nObjectIdx & 0x0000FFFF) != pQuestDataEx->nStoneOrder[pQuestDataEx->nCurrentMonolithNo])
	{
		return 0;
	}

	pQuestDataEx->nMonolithGUIDs[pQuestDataEx->nCurrentMonolithNo] = pObject->dwUnitId;
	++pQuestDataEx->nCurrentMonolithNo;

	if (pObject->dwAnimMode)
	{
		return 0;
	}

	int32_t nCurrentMonolithNo = pQuestDataEx->nCurrentMonolithNo;
	if (nCurrentMonolithNo <= 4)
	{
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
		if (pInvisibleObject)
		{
			UNITS_ChangeAnimMode(pInvisibleObject, pQuestDataEx->nCurrentMonolithNo + 1);
		}
		return 0;
	}

	if (nCurrentMonolithNo != 5)
	{
		return 0;
	}

	UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

	if (pInvisibleObject)
	{
		UNITS_ChangeAnimMode(pInvisibleObject, OBJMODE_SPECIAL4);
	}

	pQuestDataEx->bOperatedWithLastCairnStone = 1;
	QUESTS_DeleteItem(pOp->pGame, pOp->pPlayer, ' dkb');

	--pQuestDataEx->nScrollsInGame;

	D2CoordStrc monolithCoords = {};

	for (int32_t i = 0; i < 5; ++i)
	{
		if (pQuestDataEx->nStoneOrder[i] == OBJECT_STONELAMBDA)
		{
			D2UnitStrc* pMonolith = SUNIT_GetServerUnit(pOp->pGame, UNIT_OBJECT, pQuestDataEx->nMonolithGUIDs[i]);
			if (pMonolith && pMonolith->dwClassId == OBJECT_STONELAMBDA)
			{
				UNITS_GetCoords(pMonolith, &monolithCoords);
			}
		}
	}

	if (monolithCoords.nX == 0)
	{
		D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
		D2RoomStrc** ppRoomList = nullptr;
		int32_t nNumRooms = 0;
		DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);

		for (int32_t i = 0; i < nNumRooms; ++i)
		{
			D2UnitStrc* pMonolith = ppRoomList[i]->pUnitFirst;
			while (pMonolith)
			{
				if (pMonolith->dwUnitType == UNIT_OBJECT && pMonolith->dwClassId == OBJECT_STONELAMBDA)
				{
					break;
				}
				pMonolith = pMonolith->pRoomNext;
			}

			if (pMonolith)
			{
				UNITS_GetCoords(pMonolith, &monolithCoords);
				break;
			}
		}
	}

	if (monolithCoords.nX)
	{
		D2UnitStrc* pCairnStonesMissile = D2GAME_CreateMissile_6FD115E0(pOp->pGame, pOp->pPlayer, 0, 1, 288, monolithCoords.nX + 6, monolithCoords.nY - 3);
		if (pCairnStonesMissile)
		{
			DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pCairnStonesMissile), 0);
		}
	}

	if (pQuestData->fLastState < 4)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 4, pOp->pPlayer, ACT1Q4_UnitIterate_StatusCyclerEx, 1);

		const int16_t nPartyId = SUNIT_GetPartyId(pOp->pPlayer);
		if (nPartyId != -1)
		{
			PARTY_IteratePartyMembers(pOp->pGame, nPartyId, ACT1Q4_UnitIterate_SetEnterAreaFlag, 0);
		}

		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_ENTERAREA);
	}

	QUESTS_TriggerFX(pQuestData->pGame, 1);

	return 0;
}

//D2Game.0x6FC9ACD0
int32_t __fastcall OBJECTS_OperateFunction12_InifussTree(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q4_CAIN);
	D2_ASSERT(pQuestData);

	if (!pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
		return 0;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];
	if (pQuestData->fState >= 6 || pObject->dwAnimMode != OBJMODE_NEUTRAL
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) == 1
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	if (ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' dkb') || ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' skb'))
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
		return 0;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	SUNIT_AttachSound(pOp->pPlayer, 45, 0);

	pOp->pObject->dwDropItemCode = ' skb';
	QUESTS_DebugOutput(pQuestData->pGame, "Started quest", "..\\D2Game/Quests/a1q4.cpp", 599);
	QUESTS_StateDebug(pQuestData, 4, "..\\D2Game/Quests/a1q4.cpp", 2123);

	int32_t nItemLevel = 0;
	D2UnitStrc* pItem = D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pObject, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0);
	if (pItem)
	{
		pQuestData->pfCallback[QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM] = ACT1Q4_Callback09_PlayerDroppedWithQuestItem;
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 2, pOp->pPlayer, ACT1Q4_UnitIterate_StatusCyclerEx, 1);
		pQuestDataEx->unk0x4B = 1;
		pQuestDataEx->nDecipheredScrollGUID = pItem->dwUnitId;
		pQuestDataEx->unk0x58 = 1;
		pQuestDataEx->bScrollAcquired = 1;
		++pQuestDataEx->nScrollsInGame;
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
	}

	pQuestDataEx->bInifussTreeInitialized = 1;
	pQuestDataEx->nInifussTreeGUID = pObject->dwUnitId;

	return 0;
}

//D2Game.0x6FC9AEA0
void __fastcall ACT1Q4_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestData->bNotIntro || pQuestData->fState == 6)
	{
		return;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	const uint32_t dwItemCode = ITEMS_GetBaseCode(pQuestArg->pTarget);
	if (dwItemCode == ' dkb' || dwItemCode == ' skb')
	{
		--pQuestDataEx->nScrollsInGame;
	}

	if (!pQuestDataEx->nScrollsInGame && pQuestData->fState <= 5 && pQuestDataEx->bOperatedWithLastCairnStone != 1 && pQuestDataEx->bInifussTreeInitialized == 1)
	{
		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		ACT1Q4_ResetInifussTree(pQuestData);
	}
}

//D2Game.0x6FC9AF10
void __fastcall ACT1Q4_ResetInifussTree(D2QuestDataStrc* pQuestData)
{
	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;

	if (pQuestDataEx->bInifussTreeInitialized == 1)
	{
		D2UnitStrc* pInifussTree = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nInifussTreeGUID);
		if (pInifussTree && pInifussTree->dwClassId == OBJECT_INIFUSS_TREE)
		{
			UNITS_ChangeAnimMode(pInifussTree, OBJMODE_NEUTRAL);
		}

		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q4_UnitIterate_StatusCyclerEx, 1);
		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		pQuestDataEx->unk0x58 = 0;
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q4_UnitIterate_UpdateQuestStateFlags);
	}
	else
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q4_UnitIterate_StatusCyclerEx, 1);
		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		pQuestDataEx->unk0x58 = 0;
	}
}

//D2Game.0x6FC9AFD0
int32_t __fastcall ACT1Q4_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_STARTED);
	}
	else if (pQuestData->fState >= 3 && pQuestData->fState <= 5)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_LEAVETOWN);
	}

	return 0;
}

//D2Game.0x6FC9B050
void __fastcall ACT1Q4_SendStoneOrderToClient(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	if (!pQuestData)
	{
		return;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	if (!pQuestDataEx->bMonolithStonesOrdered)
	{
		pQuestDataEx->bMonolithStonesOrdered = 1;
		ACT1Q4_SetMonolithOrder(pQuestData);
	}

	D2GSPacketSrv50 packet50 = {};
	packet50.nHeader = 0x50;
	packet50.nQuestId = QUEST_A1Q4_CAIN;

	for (int32_t i = 0; i < 5; ++i)
	{
		packet50.A1Q4_CainPayload.nStoneOrder[i] = pQuestDataEx->nStoneOrder[i] - 17;
		D2_ASSERT(packet50.A1Q4_CainPayload.nStoneOrder[i] < 5);
	}

	D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet50, sizeof(packet50));
}

//D2Game.0x6FC9B0F0
void __fastcall ACT1Q4_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT1Q4_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_ITEMPICKEDUP] = ACT1Q4_Callback04_ItemPickedUp;
	pQuestData->pfCallback[6] = ACT1Q4_Callback06_6FC9BFB0;
	pQuestData->pfCallback[QUESTEVENT_PLAYERJOINEDGAME] = ACT1Q4_Callback14_PlayerJoinedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM] = ACT1Q4_Callback09_PlayerDroppedWithQuestItem;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT1Q4_Callback08_MonsterKilled;
	pQuestData->pNPCMessages = gpAct1Q4NpcMessages;
	pQuestData->nQuest = 4;
	pQuestData->fLastState = 0;
	pQuestData->bActive = 1;
	pQuestData->pfStatusFilter = 0;
	pQuestData->nInitNo = 6;
	pQuestData->pfActiveFilter = ACT1Q4_ActiveFilterCallback;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT1Q4_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT1Q4_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT1Q4_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT1Q4_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT1Q4_Callback13_PlayerStartedGame;
	pQuestData->pfSeqFilter = ACT1Q4_SeqCallback;
	pQuestData->nSeqId = 3;

	D2Act1Quest4Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act1Quest4Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	memset(pQuestDataEx, 0x00, sizeof(D2Act1Quest4Strc));
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->pQuestGUID);
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->tPlayerGUIDs);
}

//D2Game.0x6FC9B250
void __fastcall ACT1Q4_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget)
	{
		return;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->pTarget->dwClassId == MONSTER_AKARA)
	{
		if (pQuestDataEx->bAkaraIntroActivated == 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 1, pQuestArg->pPlayer, ACT1Q4_UnitIterate_StatusCyclerEx, 1);
			pQuestDataEx->bAkaraIntroActivated = 0;
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q4_UnitIterate_UpdateQuestStateFlags);
		}

		if (pQuestDataEx->bAkaraActivatedForScroll == 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 3, pQuestArg->pPlayer, ACT1Q4_UnitIterate_StatusCyclerEx, 1);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q4_UnitIterate_UpdateQuestStateFlags);
			pQuestDataEx->bAkaraActivatedForScroll = 0;
		}
	}
	else if (pQuestArg->pTarget->dwClassId == MONSTER_CAIN1)
	{
		pQuestDataEx->bCainInTristramDeactivated = 1;
	}
}

//D2Game.0x6FC9B2F0
void __fastcall ACT1Q4_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	const int16_t nMessageIndex = pQuestArg->nMessageIndex;

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (pQuestArg->nNPCNo == MONSTER_AKARA)
	{
		if (nMessageIndex == 97)
		{
			pQuestDataEx->bAkaraIntroActivated = 1;
			QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
			return;
		}

		if (nMessageIndex == 112)
		{
			if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' skb'))
			{
				QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' skb');
				D2UnitStrc* pDecipheredScroll = QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, ' dkb', 0, ITEMQUAL_NORMAL, 1);
				if (pDecipheredScroll)
				{
					pQuestDataEx->bAkaraActivatedForScroll = 1;
					pQuestDataEx->unk0x4B = 1;
					pQuestDataEx->bDecipheredScrollAcquired = 1;
					pQuestDataEx->nDecipheredScrollGUID = pDecipheredScroll->dwUnitId;
					QUESTS_DebugOutput(pQuestData->pGame, "deciphered scroll", __FILE__, __LINE__);
					QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
					pQuestData->dwFlags &= 0xFFFFFF00;
					QUESTS_UnitIterate(pQuestData, 3, pQuestArg->pPlayer, ACT1Q4_UnitIterate_StatusCyclerEx, 0);
				}
				else
				{
					--pQuestDataEx->nScrollsInGame;
				}
			}
			return;
		}

		if (nMessageIndex != 118 || !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING))
		{
			return;
		}

		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED);
		QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING);

		QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));
		QUESTS_UpdatePlayerFlags(pQuestData->pGame, pQuestArg->pPlayer);

		int32_t nItemQuality = 0;
		int32_t nItemLevel = 0;
		switch (pQuestArg->pGame->nDifficulty)
		{
		case DIFFMODE_NIGHTMARE:
			nItemLevel = 30;
			nItemQuality = ITEMQUAL_RARE;
			break;

		case DIFFMODE_HELL:
			nItemLevel = 60;
			nItemQuality = ITEMQUAL_RARE;
			break;

		default:
			nItemLevel = 7;
			nItemQuality = ITEMQUAL_MAGIC;
			break;
		}

		QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, ' nir', nItemLevel, nItemQuality, 1);
		QUESTS_SendLogUpdate(pQuestArg->pPlayer, QUEST_A1Q4_CAIN);
		QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE))
		{
			if (pQuestData->fLastState != 13)
			{
				pQuestData->fLastState = 13;
			}

			if (pQuestData->fState != 6)
			{
				QUESTS_StateDebug(pQuestData, 6, __FILE__, __LINE__);
			}

			if (!QUESTS_GetGlobalState(pQuestArg->pGame, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE))
			{
				QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE);
				if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
				{
					FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
					exit(-1);
				}
				pQuestData->pfSeqFilter(pQuestData);
			}
		}

		if (pQuestDataEx->bQuestCompleteBefore == 1)
		{
			QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE);
		}
		return;
	}
	else if (pQuestArg->nNPCNo == MONSTER_CAIN5)
	{
		int32_t nPlayerId = -1;
		if (pQuestArg->pPlayer)
		{
			nPlayerId = pQuestArg->pPlayer->dwUnitId;
		}

		if (nMessageIndex == 125)
		{
			QUESTS_AddPlayerGUID(&pQuestData->tPlayerGUIDs, nPlayerId);
			QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pQuestGUID, nPlayerId);
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		}
		else if (nMessageIndex == 126 || nMessageIndex == 123)
		{
			QUESTS_AddPlayerGUID(&pQuestDataEx->tPlayerGUIDs, nPlayerId);
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
		}
	}
}

//D2Game.0x6FC9B610
void __fastcall ACT1Q4_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4, 5, 6
	};

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (nNpcId == MONSTER_CAIN1)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN1, 9);
	}

	if (pQuestDataEx->bOperatedWithLastCairnStone == 1 && ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' dkb'))
	{
		QUESTS_DeleteItem(pQuestArg->pGame, pQuestArg->pPlayer, ' dkb');
		--pQuestDataEx->nScrollsInGame;
	}

	int32_t nPlayerId = -1;
	if (pQuestArg->pPlayer)
	{
		nPlayerId = pQuestArg->pPlayer->dwUnitId;
	}

	if (nNpcId == MONSTER_CAIN5 && !QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->tPlayerGUIDs, nPlayerId) && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN5, 5);
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING))
	{
		if (nNpcId != MONSTER_CAIN5)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 5);
			return;
		}

		if (QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->tPlayerGUIDs, nPlayerId))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN5, 7);
		}
		else
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN5, 5);
		}
		return;
	}

	if (QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->pQuestGUID, nPlayerId) == 1)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 6);
		return;
	}

	if (QUESTS_CheckPlayerGUID(pQuestData, nPlayerId) == 1 && nNpcId == MONSTER_CAIN5 && !QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->tPlayerGUIDs, nPlayerId))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_CAIN5, 5);
		return;
	}

	if (QUESTS_CheckPlayerGUID(pQuestData, nPlayerId) == 1)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_COMPLETEDNOW))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 8);
		}
		else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 7);
		}
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_COMPLETEDNOW) || !pQuestData->fState || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_COMPLETEDBEFORE))
	{
		return;
	}

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, 0x20736B62u))
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
		return;
	}

	if (pQuestData->fState == 4)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
	}
	else if (pQuestData->fState < 6)
	{
		const int32_t nIndex = nIndices[pQuestData->fState];
		if (nIndex != -1 && nIndex < 11)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
		}
	}
}

//D2Game.0x6FC9B930
void __fastcall ACT1Q4_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' dkb'))
	{
		++pQuestDataEx->nScrollsInGame;
	}

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' skb'))
	{
		++pQuestDataEx->nScrollsInGame;
	}
}

//D2Game.0x6FC9B970
void __fastcall ACT1Q4_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;

	QUESTS_RemovePlayerGUID(pQuestData, pQuestArg);

	if (pQuestArg->pPlayer)
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pQuestGUID, pQuestArg->pPlayer->dwUnitId);
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
	}
	else
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, -1);
	}
}

//D2Game.0x6FC9B9D0
void __fastcall ACT1Q4_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;

	if (pQuestArg->nNewLevel == LEVEL_TRISTRAM && !pQuestDataEx->bCainInTownSpawned && !pQuestDataEx->unk0x50 && pQuestData->fState >= 6)
	{
		QUESTS_DebugOutput(pQuestData->pGame, "Started quest", __FILE__, __LINE__);
		QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 4, pQuestArg->pPlayer, ACT1Q4_UnitIterate_StatusCyclerEx, 0);
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q4_UnitIterate_UpdateQuestStateFlags);
	}

	if (pQuestArg->nOldLevel == LEVEL_ROGUEENCAMPMENT)
	{
		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);

		int32_t nPlayerId = -1;
		if (pQuestArg->pPlayer)
		{
			nPlayerId = pQuestArg->pPlayer->dwUnitId;
		}
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->pQuestGUID, nPlayerId);

		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING) == 1)
		{
			return;
		}

		if (pQuestData->fState == 2)
		{
			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		}
	}

	if (pQuestArg->nNewLevel == LEVEL_ROGUEENCAMPMENT)
	{
		if (pQuestDataEx->pPlayerInTristram)
		{
			D2UnitStrc* pCain = SUNIT_GetServerUnit(pQuestArg->pGame, UNIT_OBJECT, pQuestDataEx->nCainObjectGUID);
			if (pCain && pQuestDataEx->unk0x52 == 1 && !pQuestDataEx->bCainInTownSpawned)
			{
				ACT1Q4_SpawnCainInTown(pQuestArg->pGame, UNITS_GetRoom(pCain), CLIENTS_GetUnitX(pCain), CLIENTS_GetUnitY(pCain));
			}
		}
	}
	else if (pQuestArg->nNewLevel == LEVEL_LUTGHOLEIN)
	{
		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING) != 1 && !pQuestDataEx->unk0x50 && pQuestData->fState < 6)
		{
			QUESTS_DebugOutput(pQuestData->pGame, "Started quest", __FILE__, __LINE__);
			ACT1Q4_UpdateObjects(pQuestData, 0, 1);
			QUESTS_StateDebug(pQuestData, 7, __FILE__, __LINE__);
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 5, pQuestArg->pPlayer, ACT1Q4_UnitIterate_StatusCyclerEx, 1);
			QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q4_UnitIterate_AddPlayerGUIDs);
			pQuestDataEx->unk0x58 = 1;
		}
	}
}

//D2Game.0x6FC9BC80
void __fastcall ACT1Q4_SpawnCainInTown(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nX, int32_t nY)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	if (!pQuestData)
	{
		return;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;

	D2DrlgCoordsStrc pRoomCoord = {};
	DUNGEON_GetRoomCoordinates(pRoom, &pRoomCoord);

	int32_t nTmpX = nX;
	int32_t nTmpY = nY;

	D2CoordStrc pCoord = {};
	pCoord.nX = nX;
	pCoord.nY = nY;

	int32_t nCounter = 0;
	while (nTmpX < pRoomCoord.dwSubtilesLeft || nTmpY < pRoomCoord.dwSubtilesTop || nTmpX >= pRoomCoord.dwSubtilesWidth + pRoomCoord.dwSubtilesLeft - 1 || nTmpY >= pRoomCoord.dwSubtilesTop + pRoomCoord.dwSubtilesHeight - 1)
	{
		++nTmpX;
		++nTmpY;

		++nCounter;
		if (nCounter > 20)
		{
			pCoord.nX = nY;
			break;
		}
	}
	if (nCounter <= 20)
	{
		pCoord.nX = nTmpX;
	}
	pCoord.nY = nTmpY;

	D2RoomStrc* pFreeRoom = pRoom;
	QUESTS_GetFreePosition(pRoom, &pCoord, 2, 0x100, &pFreeRoom, 1);

	if (!pFreeRoom)
	{
		pFreeRoom = pRoom;
		pCoord.nX = nX;
		pCoord.nY = nY;
	}

	D2UnitStrc* pCain = sub_6FC69F10(pGame, pFreeRoom, pCoord.nX, pCoord.nY, MONSTER_CAIN5, 1, 5, 0);
	if (pCain)
	{
		pCain->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
		pQuestDataEx->bCainInTownSpawned = 1;
		pQuestDataEx->unk0x52 = 0;
		pQuestDataEx->nCainMonsterInTownGUID = pCain->dwUnitId;
		return;
	}

	nCounter = 0;
	do
	{
		++pCoord.nX;
		++pCoord.nY;

		pFreeRoom = DUNGEON_GetRoomAtPosition(pFreeRoom, pCoord.nX, pCoord.nY);

		if (!pFreeRoom)
		{
			pCoord.nX = nX;
			pFreeRoom = pRoom;
			pCoord.nY = nY;
		}

		QUESTS_GetFreePosition(pFreeRoom, &pCoord, 2, 0x100, &pFreeRoom, 2);

		if (!pFreeRoom)
		{
			pCoord.nX = nX;
			pFreeRoom = pRoom;
			pCoord.nY = nY;
		}

		++nCounter;

		pCain = sub_6FC69F10(pGame, pFreeRoom, pCoord.nX, pCoord.nY, MONSTER_CAIN5, 1, 10, 0);
		if (pCain)
		{
			pCain->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
			pQuestDataEx->bCainInTownSpawned = 1;
			pQuestDataEx->unk0x52 = 0;
			pQuestDataEx->nCainMonsterInTownGUID = pCain->dwUnitId;
			return;
		}
	}
	while (nCounter < 20);

	pCain = sub_6FC69F10(pGame, pRoom, nX, nY, MONSTER_CAIN5, 1, 15, 0);
	if (pCain)
	{
		pCain->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
		pQuestDataEx->bCainInTownSpawned = 1;
		pQuestDataEx->unk0x52 = 0;
		pQuestDataEx->nCainMonsterInTownGUID = pCain->dwUnitId;
	}
}

//D2Game.0x6FC9BEF0
int32_t __fastcall ACT1Q4_UnitIterate_AddPlayerGUIDs(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDPENDING))
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_COMPLETEDNOW);

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN)->pQuestDataEx;
	if (pUnit)
	{
		QUESTS_AddPlayerGUID(&pQuestDataEx->pQuestGUID, pUnit->dwUnitId);
	}
	else
	{
		QUESTS_AddPlayerGUID(&pQuestDataEx->pQuestGUID, -1);
	}

	return 0;
}

//D2Game.0x6FC9BF70
void __fastcall ACT1Q4_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_DebugOutput(pQuestData->pGame, "cairn stone item acquired", __FILE__, __LINE__);
	if (pQuestData->bNotIntro)
	{
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q4_UnitIterate_UpdateQuestStateFlags);
	}
}

//D2Game.0x6FC9BFB0
void __fastcall ACT1Q4_Callback06_6FC9BFB0(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (((D2Act1Quest4Strc*)pQuestData->pQuestDataEx)->bOperatedWithLastCairnStone != 1 && pQuestData->fState <= 5)
	{
		QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
		ACT1Q4_ResetInifussTree(pQuestData);
	}
}

//D2Game.0x6FC9BFF0
bool __fastcall ACT1Q4_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (!pQuestData->fState && pQuestData->bNotIntro == 1)
	{
		QUESTS_DebugOutput(pQuestData->pGame, "Started quest", __FILE__, __LINE__);
		QUESTS_StateDebug(pQuestData, 1, __FILE__, __LINE__);
		return true;
	}

	if (pQuestData->fState != 6 && pQuestData->bNotIntro)
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

//D2Game.0x6FC9C0A0
void __fastcall ACT1Q4_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) == 1)
	{
		D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A1Q4_CAIN);
		if (pQuest)
		{
			((D2Act1Quest4Strc*)pQuest->pQuestDataEx)->unk0x52 = 1;
		}

		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE);
		pQuestDataEx->nCainGibbetObjectMode = OBJMODE_SPECIAL1;
		pQuestDataEx->unk0x4C = 1;
		pQuestDataEx->bActivateCairnStones = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_COMPLETEDBEFORE) == 1)
	{
		D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A1Q4_CAIN);
		if (pQuest)
		{
			((D2Act1Quest4Strc*)pQuest->pQuestDataEx)->unk0x52 = 1;
		}

		pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A1Q4_CAIN);
		if (pQuest)
		{
			((D2Act1Quest4Strc*)pQuest->pQuestDataEx)->bQuestCompleteBefore = 1;
		}

		pQuestDataEx->nCainGibbetObjectMode = OBJMODE_SPECIAL1;
		pQuestDataEx->unk0x4C = 1;
		pQuestDataEx->bActivateCairnStones = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_ENTERAREA) == 1)
	{
		pQuestData->fLastState = 4;
		pQuestData->fState = 5;
		pQuestDataEx->unk0x4C = 1;
		pQuestDataEx->bDecipheredScrollAcquired = 1;
		pQuestDataEx->bActivateCairnStones = 1;
		pQuestDataEx->bScrollAcquired = 1;
		pQuestDataEx->unk0x58 = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_STARTED) == 1)
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 1;
	}

	D2UnitStrc* pDecipheredScroll = ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' dkb');
	D2UnitStrc* pBarkScroll = ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' skb');
	if (pDecipheredScroll)
	{
		++pQuestDataEx->nScrollsInGame;
	}

	if (pBarkScroll)
	{
		++pQuestDataEx->nScrollsInGame;
	}

	if (pDecipheredScroll)
	{
		pQuestData->fState = 5;
		pQuestData->fLastState = 3;
		pQuestDataEx->bDecipheredScrollAcquired = 1;
		pQuestDataEx->bScrollAcquired = 1;
		pQuestDataEx->unk0x58 = 1;
	}
	else if (pBarkScroll)
	{
		pQuestDataEx->bScrollAcquired = 1;
		pQuestData->fState = 4;
		pQuestData->fLastState = 2;
		pQuestDataEx->unk0x58 = 1;
	}
}

//D2Game.0x6FC9C220
void __fastcall ACT1Q4_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	int32_t nItemLevel = 0;

	if (!pQuestArg->pPlayer)
	{
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q4_UnitIterate_SetCustom6Flag);
		//D2Game_10034_Return(64);
		pQuestArg->pTarget->dwDropItemCode = ' spv';
		for (int32_t i = 0; i < 8; ++i)
		{
			D2GAME_DropItemAtUnit_6FC4FEC0(pQuestArg->pGame, pQuestArg->pTarget, 0, &nItemLevel, 0, -1, 0);
		}
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_CUSTOM6) && QUESTRECORD_GetQuestState(pQuestFlags, (pQuestArg->pGame->bExpansion ? QUESTSTATEFLAG_A5Q6 : QUESTSTATEFLAG_A4Q2), QFLAG_REWARDGRANTED))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_CUSTOM6);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q4_UnitIterate_SetCustom6Flag);
		//D2Game_10034_Return(64);
		pQuestArg->pTarget->dwDropItemCode = ' spv';
		for (int32_t i = 0; i < 8; ++i)
		{
			D2GAME_DropItemAtUnit_6FC4FEC0(pQuestArg->pGame, pQuestArg->pTarget, 0, &nItemLevel, 0, -1, 0);
		}
	}
}

//D2Game.0x6FC9C300
int32_t __fastcall ACT1Q4_UnitIterate_SetCustom6Flag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);

	if (pRoom && DUNGEON_GetLevelIdFromRoom(pRoom) == LEVEL_MOOMOOFARM)
	{
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A1Q4, QFLAG_CUSTOM6);
	}

	return 0;
}

//D2Game.0x6FC9C340
void __fastcall OBJECTS_InitFunction09_InifussTree(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q4_CAIN);
	if (pQuestData)
	{
		D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
		pQuestDataEx->bInifussTreeInitialized = 1;
		if (!pQuestData->bNotIntro || pQuestDataEx->unk0x50)
		{
			pQuestDataEx->unk0x58 = OBJMODE_OPERATING;
		}

		UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->unk0x58);
	}
	else
	{
		if (!pOp->pObject || pOp->pObject->dwAnimMode != OBJMODE_OPENED)
		{
			UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
		}
	}
}

//D2Game.0x6FC9C3A0
void __fastcall OBJECTS_InitFunction13_InvisibleObject(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q4_CAIN);
	if (pQuestData)
	{
		QUESTS_CreateChainRecord(pQuestData->pGame, pOp->pObject, QUEST_A1Q4_CAIN);
	}
	else
	{
		if (!pOp->pObject || pOp->pObject->dwAnimMode != OBJMODE_OPENED)
		{
			UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
		}
	}
}

//D2Game.0x6FC9C460
void __fastcall ACT1Q4_UpdateQuestStateOnActChange(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDPENDING) == 1 || ((D2Act1Quest4Strc*)pQuestData->pQuestDataEx)->unk0x50)
	{
		return;
	}

	if (pQuestData->fState >= 6)
	{
		return;
	}

	QUESTS_DebugOutput(pQuestData->pGame, "Started quest", __FILE__, __LINE__);
	ACT1Q4_UpdateObjects(pQuestData, 0, 1);
	QUESTS_StateDebug(pQuestData, 7, __FILE__, __LINE__);
	pQuestData->dwFlags &= 0xFFFFFF00;
	QUESTS_UnitIterate(pQuestData, 5, pUnit, ACT1Q4_UnitIterate_StatusCyclerEx, 1);
	QUESTS_SetGlobalState(pGame, QUESTSTATEFLAG_A1Q4, QFLAG_PRIMARYGOALDONE);
	SUNIT_IterateUnitsOfType(pGame, 0, 0, ACT1Q4_UnitIterate_AddPlayerGUIDs);
}

//D2Game.0x6FC9C540
void __fastcall OBJECTS_InitFunction54_CainStartPosition(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q4_CAIN);
	if (!pQuestData)
	{
		return;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}
	pQuestDataEx->nCainObjectGUID = nUnitId;

	pQuestDataEx->bCainObjectInitialized = 1;
	pQuestDataEx->pCainStartPositionCoords.nX = pOp->nX;
	pQuestDataEx->pCainStartPositionCoords.nY = pOp->nY;

	if (pQuestDataEx->unk0x52 == 1 && !pQuestDataEx->bCainInTownSpawned)
	{
		ACT1Q4_SpawnCainInTown(pOp->pGame, pOp->pRoom, pOp->nX, pOp->nY);
	}
}

//D2Game.0x6FC9C5B0
int32_t __fastcall ACT1Q4_CreateCowPortal(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTS_GetGlobalState(pGame, QUESTSTATEFLAG_A1Q4, QFLAG_CUSTOM7) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q4, QFLAG_CUSTOM6))
	{
		SUNIT_AttachSound(pUnit, 20, pUnit);
		return 0;
	}

	if (pGame->bExpansion)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED))
		{
			SUNIT_AttachSound(pUnit, 20, pUnit);
			return 0;
		}
	}
	else if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4Q2, QFLAG_REWARDGRANTED))
	{
		if (!pGame->bExpansion)
		{
			SUNIT_AttachSound(pUnit, 20, pUnit);
			return 0;
		}

		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED))
		{
			SUNIT_AttachSound(pUnit, 20, pUnit);
			return 0;
		}
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_ROGUEENCAMPMENT)
	{
		SUNIT_AttachSound(pUnit, 20, pUnit);
		return 0;
	}

	D2CoordStrc pCoord = {};
	UNITS_GetCoords(pUnit, &pCoord);
	D2RoomStrc* pFreeRoom = nullptr;
	QUESTS_GetFreePosition(pRoom, &pCoord, 3, 0x400, &pFreeRoom, 4);
	if (pFreeRoom && D2GAME_CreatePortalObject_6FD13DF0(pGame, pUnit, pFreeRoom, pCoord.nX, pCoord.nY, LEVEL_MOOMOOFARM, 0, OBJECT_PERMANENT_TOWN_PORTAL, 0))
	{
		QUESTS_SetGlobalState(pGame, QUESTSTATEFLAG_A1Q4, QFLAG_CUSTOM7);
		return 1;
	}

	SUNIT_AttachSound(pUnit, 20, pUnit);
	return 0;
}

//D2Game.0x6FC9C6E0
void __fastcall OBJECTS_InitFunction61_CainPortal(D2ObjInitFnStrc* pOp)
{
	UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPERATING);
	EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_QUESTFN, pOp->pGame->dwGameFrame + 25, 0, 0);
}

//D2Game.0x6FC9C710
void __fastcall ACT1Q4_UpdatePortalInTown(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit)
{
	int32_t nAnimMode = 0;
	if (pUnit)
	{
		nAnimMode = pUnit->dwAnimMode;
	}

	if (nAnimMode == OBJMODE_OPERATING)
	{
		UNITS_ChangeAnimMode(pUnit, OBJMODE_OPENED);
	}
	else if (nAnimMode == OBJMODE_OPENED)
	{
		D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
		if (DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit)) == LEVEL_ROGUEENCAMPMENT)
		{
			++pQuestDataEx->nObjectUpdateInvocations;
			if (pQuestDataEx->nObjectUpdateInvocations > 5)
			{
				pQuestDataEx->bChangePortalInTownToSpecialMode = 1;
			}

			if (pQuestDataEx->bChangePortalInTownToSpecialMode)
			{
				UNITS_ChangeAnimMode(pUnit, OBJMODE_SPECIAL1);
			}
		}
		else
		{
			if (pQuestDataEx->bChangePortalOutsideTownToSpecialMode)
			{
				UNITS_ChangeAnimMode(pUnit, OBJMODE_SPECIAL1);
			}
		}
	}
	else  if (nAnimMode == OBJMODE_SPECIAL1)
	{
		UNITS_ChangeAnimMode(pUnit, OBJMODE_SPECIAL2);
	}

	EVENT_SetEvent(pQuestData->pGame, pUnit, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 25, 0, 0);
}

//D2Game.0x6FC9C7A0
int32_t __fastcall ACT1Q4_GetCainPortalInTownCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pCoord)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;

	if (pQuestData->bNotIntro && pQuestDataEx->bCainPortalInTownSpawned && !pQuestDataEx->bChangePortalInTownToSpecialMode)
	{
		pCoord->nX = pQuestDataEx->pCainPortalInTownCoords.nX;
		pCoord->nY = pQuestDataEx->pCainPortalInTownCoords.nY;
		return 1;
	}

	return 0;
}

//D2Game.0x6FC9C7F0
int32_t __fastcall ACT1Q4_SpawnCainPortalOutsideTown(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		return 1;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bCainPortalOutsideTownSpawned)
	{
		return 1;
	}

	const int32_t nX = pQuestDataEx->pCainPortalOutsideTownCoords.nX;
	const int32_t nY = pQuestDataEx->pCainPortalOutsideTownCoords.nY;

	D2RoomStrc* pRoom = DUNGEON_GetRoomAtPosition(UNITS_GetRoom(pUnit), nX, nY);
	if (!pRoom)
	{
		pQuestDataEx->pCainPortalOutsideTownCoords.nX += 3;
		pQuestDataEx->pCainPortalOutsideTownCoords.nY += 3;
		return 1;
	}

	D2UnitStrc* pCainPortal = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_CAINPORTAL, pQuestDataEx->pCainPortalOutsideTownCoords.nX, pQuestDataEx->pCainPortalOutsideTownCoords.nY, pGame, pRoom, 1, 1, 0);
	if (pCainPortal)
	{
		pQuestDataEx->bCainPortalOutsideTownSpawned = 1;
		pQuestDataEx->nCainPortalOutsideTownGUID = pCainPortal->dwUnitId;
	}

	return 1;
}

//D2Game.0x6FC9C930
int32_t __fastcall ACT1Q4_GetCainPortalOutsideTownCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pCoord)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bCainPortalOutsideTownSpawned)
	{
		pCoord->nX = pQuestDataEx->pCainPortalOutsideTownCoords.nX;
		pCoord->nY = pQuestDataEx->pCainPortalOutsideTownCoords.nY;
	}
	else
	{
		UNITS_GetCoords(pUnit, pCoord);
	}

	return 1;
}

//D2Game.0x6FC9C990
int32_t __fastcall ACT1Q4_UpdateCainPortalCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bCainPortalCoordsInitialized)
	{
		return 0;
	}

	UNITS_GetCoords(pUnit, &pQuestDataEx->pCainPortalOutsideTownCoords);
	pQuestDataEx->bCainPortalCoordsInitialized = 1;
	return 1;
}

//D2Game.0x6FC9C9D0
void __fastcall ACT1Q4_SpawnCainPortalInTown(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
	if (!pQuestData)
	{
		return;
	}

	D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bChangePortalOutsideTownToSpecialMode = 1;
	pQuestDataEx->unk0x52 = 1;

	if (pQuestDataEx->bCainObjectInitialized != 1)
	{
		return;
	}

	D2UnitStrc* pCainObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nCainObjectGUID);
	if (!pCainObject)
	{
		return;
	}

	D2RoomStrc* pRoom = UNITS_GetRoom(pCainObject);

	const int32_t nX = CLIENTS_GetUnitX(pCainObject);
	const int32_t nY = CLIENTS_GetUnitY(pCainObject);

	pQuestDataEx->nCainObjectGUID = pCainObject->dwUnitId;
	pQuestDataEx->bCainObjectInitialized = 1;
	pQuestDataEx->pCainStartPositionCoords.nX = nX;
	pQuestDataEx->pCainStartPositionCoords.nY = nY;

	if (pQuestDataEx->unk0x52 == 1)
	{
		if (!pQuestDataEx->bCainInTownSpawned)
		{
			ACT1Q4_SpawnCainInTown(pGame, pRoom, nX, nY);
		}
	}

	if (pQuestDataEx->bCainInTownSpawned != 1)
	{
		return;
	}

	D2UnitStrc* pCainMonster = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pQuestDataEx->nCainMonsterInTownGUID);
	if (!pCainMonster)
	{
		return;
	}

	D2CoordStrc pCoord = {};
	UNITS_GetCoords(pCainMonster, &pCoord);

	pRoom = DUNGEON_GetRoomAtPosition(UNITS_GetRoom(pCainMonster), pCoord.nX, pCoord.nY);
	if (pRoom)
	{
		D2UnitStrc* pCainPortal = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_CAINPORTAL, pCoord.nX, pCoord.nY, pGame, pRoom, 1, 1, 0);
		if (pCainPortal)
		{
			pQuestDataEx->bCainPortalInTownSpawned = 1;
			pQuestDataEx->nCainPortalInTownGUID = pCainPortal->dwUnitId;
		}
	}
}

//D2Game.0x6FC9CBE0
void __fastcall ACT1Q4_OnCainInTownActivated(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);

	if (pQuestData)
	{
		((D2Act1Quest4Strc*)pQuestData->pQuestDataEx)->bChangePortalInTownToSpecialMode = 1;
	}
}

//D2Game.0x6FC9CC00
int32_t __fastcall ACT1Q4_IsCainInTristramDeactivated(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);

	if (pQuestData)
	{
		return ((D2Act1Quest4Strc*)pQuestData->pQuestDataEx)->bCainInTristramDeactivated != 0;
	}

	return false;
}
