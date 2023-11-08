#include "QUESTS/ACT1/A1Q5.h"

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <D2QuestRecord.h>

#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Game.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterSpawn.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "SKILLS/Skills.h"


//D2Game.0x6FD33760
D2NPCMessageTableStrc gpAct1Q5NpcMessages[] =
{
	{
		{
			{ MONSTER_CAIN5, 131, 0, 2 },
			{ MONSTER_CHARSI, 129, 0, 2 },
			{ MONSTER_AKARA, 130, 0, 2 },
			{ MONSTER_WARRIV1, 132, 0, 2 },
			{ MONSTER_KASHYA, 133, 0, 2 },
			{ MONSTER_GHEED, 128, 0 , 2 }
		},
		6
	},
	{
		{
			{ MONSTER_KASHYA, 134, 0, 2 },
			{ MONSTER_WARRIV1, 136, 0, 2 },
			{ MONSTER_CHARSI, 137, 0, 2 },
			{ MONSTER_AKARA, 138, 0, 2 },
			{ MONSTER_CAIN5, 135, 0, 2 },
			{ MONSTER_GHEED, 139, 0 , 2 }
		},
		6
	},
	{
		{
			{ MONSTER_KASHYA, 140, 0, 0 },
			{ MONSTER_WARRIV1, 141, 0, 0 },
			{ MONSTER_CAIN5, 145, 0, 0 },
			{ MONSTER_CHARSI, 144, 0, 0 },
			{ MONSTER_AKARA, 143, 0, 0 },
			{ MONSTER_GHEED, 142, 0 , 0 }
		},
		6
	},
	{
		{
			{ MONSTER_KASHYA, 140, 0, 2 },
			{ MONSTER_WARRIV1, 141, 0, 2 },
			{ MONSTER_CAIN5, 145, 0, 2 },
			{ MONSTER_CHARSI, 144, 0, 2 },
			{ MONSTER_AKARA, 143, 0, 2 },
			{ MONSTER_GHEED, 142, 0 , 2 }
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

//D2Game.0x6FC9CD00
int32_t __fastcall OBJECTS_OperateFunction06_TowerTome(D2ObjOperateFnStrc* pOp, int32_t nOperate)
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

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q5_COUNTESS);
	if (!pQuestData || !pQuestData->bNotIntro)
	{
		return 1;
	}

	QUESTS_SendScrollMessage(pOp->pPlayer, pObject, 127);

	if (pQuestData->fState > 1)
	{
		return 1;
	}

	QUESTS_StateDebug(pQuestData, 2, "..\\D2Game/Quests/a1q5.cpp", 514);
	if (pQuestData->fLastState < 1)
	{
		((D2Act1Quest5Strc*)pQuestData->pQuestDataEx)->bTowerTomeActivated = 1;
	}

	return 1;
}

//D2Game.0x6FC9CDA0
int32_t __fastcall ACT1Q5_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A1Q5_COUNTESS);
	}

	return 0;
}

//D2Game.0x6FC9CE00
int32_t __fastcall ACT1Q5_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q5_COUNTESS);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		switch (pQuestData->fLastState)
		{
		case 1:
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_LEAVETOWN);
			return 0;
		case 2:
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_ENTERAREA);
			return 0;
		case 3:
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_CUSTOM1);
			return 0;
		case 4:
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_CUSTOM2);
			return 0;
		}
	}

	return 0;
}

//D2Game.0x6FC9CED0
bool __fastcall ACT1Q5_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	D2Act1Quest5Strc* pQuestDataEx = (D2Act1Quest5Strc*)pQuest->pQuestDataEx;
	int32_t nUnitId = -1;
	if (pPlayer)
	{
		nUnitId = pPlayer->dwUnitId;
	}

	for (int32_t i = 0; i < pQuestDataEx->nUnitCount2; ++i)
	{
		if (pQuestDataEx->nUnitGUIDs2[i] == nUnitId)
		{
			if (nNpcId == MONSTER_WARRIV1 || nNpcId == MONSTER_GHEED)
			{
				return false;
			}

			return true;
		}
	}

	return false;
}

//D2Game.0x6FC9CF20
void __fastcall ACT1Q5_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (nLevelId && DRLG_GetActNoFromLevelId(nLevelId) == ACT_I)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_PRIMARYGOALDONE);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED);
	}
}

//D2Game.0x6FC9CFD0
void __fastcall ACT1Q5_SpawnTowerChestMissiles(D2QuestDataStrc* pQuestData, D2Act1Quest5Strc* pQuestDataEx)
{
	if (!pQuestDataEx->bCountessKilled || pQuestDataEx->bDeathMissilesCreated)
	{
		return;
	}

	D2UnitStrc* pTrapFirebolt = nullptr;

	for (int32_t i = 0; i < pQuestDataEx->nObjectCount; ++i)
	{
		D2UnitStrc* pObject = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nObjectGUIDs[i]);
		if (pObject)
		{
			D2CoordStrc pCoord = {};
			pCoord.nX = pQuestDataEx->pCoord.nX;
			pCoord.nY = pQuestDataEx->pCoord.nY;

			if (!pTrapFirebolt)
			{
				D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pObject), pCoord.nX, pCoord.nY);

				if (pRoom)
				{
					pTrapFirebolt = D2GAME_SpawnMonster_6FC69F10(pQuestData->pGame, pRoom, pCoord.nX, pCoord.nY, MONSTER_TRAP_FIREBOLT, 12, -1, 8);
				}

				if (!pTrapFirebolt)
				{
					UNITS_GetCoords(pObject, &pCoord);

					pCoord.nX += 5;
					pCoord.nY += 5;

					pTrapFirebolt = D2GAME_SpawnMonster_6FC69F10(pQuestData->pGame, D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pObject), pCoord.nX, pCoord.nY), pCoord.nX, pCoord.nY, MONSTER_TRAP_FIREBOLT, 12, -1, 8);
				}
			}

			if (pTrapFirebolt)
			{
				pQuestDataEx->bDeathMissilesCreated = 1;

				D2UnitStrc* pMissile = D2GAME_CreateMissile_6FD115E0(pQuestData->pGame, pTrapFirebolt, 0, 1, 332, CLIENTS_GetUnitX(pObject), CLIENTS_GetUnitY(pObject));
				if (pMissile)
				{
					MISSILE_SetTargetX(pMissile, pQuestDataEx->nObjectGUIDs[i]);
					MISSILE_SetTargetY(pMissile, 0);
					DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pMissile), 0);
				}
			}
		}
	}
}

//D2Game.0x6FC9D2E0
void __fastcall ACT1Q5_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fLastState = 0;
	pQuestData->fState = 0;
	pQuestData->pfStatusFilter = 0;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT1Q5_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT1Q5_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT1Q5_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT1Q5_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT1Q5_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT1Q5_Callback13_PlayerStartedGame;
	pQuestData->nQuest = QUESTSTATEFLAG_A1Q5;
	pQuestData->pNPCMessages = gpAct1Q5NpcMessages;
	pQuestData->bActive = 1;
	pQuestData->nInitNo = 4;
	pQuestData->pfActiveFilter = ACT1Q5_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT1Q5_SeqCallback;

	D2Act1Quest5Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act1Quest5Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->nSeqId = 3;

	memset(pQuestDataEx, 0x00, sizeof(D2Act1Quest5Strc));
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->tPlayerGUIDs);
}

//D2Game.0x6FC9D3B0
void __fastcall ACT1Q5_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	const int16_t nMsgId = pQuestArg->nMessageIndex;
	const int16_t nNpcId = pQuestArg->nNPCNo;

	D2Act1Quest5Strc* pQuestDataEx = (D2Act1Quest5Strc*)pQuestData->pQuestDataEx;

	if (nMsgId == 127 && pQuestData->bNotIntro)
	{
		bool v6 = 0;
		if (pQuestDataEx->bTowerTomeActivated == 1)
		{
			if (pQuestData->fLastState < 1)
			{
				v6 = 1;
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 1, 0, ACT1Q5_UnitIterate_StatusCyclerEx, 1);
			}

			if (pQuestData->fLastState == 3)
			{
				v6 = 1;
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 2, 0, ACT1Q5_UnitIterate_StatusCyclerEx, 1);
				if (pQuestData->fState < 3)
				{
					QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
				}
			}
		}

		if (pQuestData->fState < 2)
		{
			QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q5_UnitIterate_UpdateQuestStateFlags);
		}
		else if (v6)
		{
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q5_UnitIterate_UpdateQuestStateFlags);
		}
	}

	if ((nNpcId == MONSTER_CHARSI || nNpcId == MONSTER_KASHYA || nNpcId == MONSTER_CAIN5 || nNpcId == MONSTER_WARRIV1 || nNpcId == MONSTER_AKARA || nNpcId == MONSTER_GHEED)
		&& (nMsgId == 144 || nMsgId == 140 || nMsgId == 145 || nMsgId == 141 || nMsgId == 143 || nMsgId == 142))
	{
		if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A1Q5, QFLAG_PRIMARYGOALDONE) && pQuestDataEx->bTriggerSeqFilter)
		{
			pQuestDataEx->bTriggerSeqFilter = 0;
			QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
			if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
			{
				FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
				exit(-1);
			}
			pQuestData->pfSeqFilter(pQuestData);
		}

		const int32_t nUnitId = pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1;

		for (int32_t i = 0; i < pQuestDataEx->nUnitCount2; ++i)
		{
			if (pQuestDataEx->nUnitGUIDs2[i] == nUnitId)
			{
				if (ACT1Q5_UpdateUnitGUIDLists(pQuestDataEx, nUnitId, 1) && pQuestDataEx->nUnitCount1 < 12)
				{
					pQuestDataEx->nUnitGUIDs1[pQuestDataEx->nUnitCount1] = nUnitId;
					++pQuestDataEx->nUnitCount1;
				}
				break;
			}
		}
	}
}

//D2Game.0x6FC9D5D0
int32_t __fastcall ACT1Q5_UpdateUnitGUIDLists(D2Act1Quest5Strc* pQuestDataEx, int32_t nUnitGUID, int32_t a3)
{
	if (a3)
	{
		for (int32_t i = 0; i < pQuestDataEx->nUnitCount2; ++i)
		{
			if (pQuestDataEx->nUnitGUIDs2[i] == nUnitGUID)
			{
				if (i != pQuestDataEx->nUnitCount2 - 1)
				{
					pQuestDataEx->nUnitGUIDs2[i] = pQuestDataEx->nUnitGUIDs2[pQuestDataEx->nUnitCount2 - 1];
				}
				--pQuestDataEx->nUnitCount2;

				return 1;
			}
		}
	}
	else
	{
		for (int32_t i = 0; i < pQuestDataEx->nUnitCount1; ++i)
		{
			if (pQuestDataEx->nUnitGUIDs1[i] == nUnitGUID)
			{
				if (i != pQuestDataEx->nUnitCount1 - 1)
				{
					pQuestDataEx->nUnitGUIDs1[i] = pQuestDataEx->nUnitGUIDs1[pQuestDataEx->nUnitCount1 - 1];
				}
				--pQuestDataEx->nUnitCount1;

				return 1;
			}
		}
	}

	return 0;
}

//D2Game.0x6FC9D650
void __fastcall ACT1Q5_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest5Strc* pQuestDataEx = (D2Act1Quest5Strc*)pQuestData->pQuestDataEx;
	int32_t nUnitId = -1;
	if (pQuestArg->pPlayer)
	{
		nUnitId = pQuestArg->pPlayer->dwUnitId;
	}

	QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, nUnitId);
	ACT1Q5_UpdateUnitGUIDLists(pQuestDataEx, nUnitId, 1);
	ACT1Q5_UpdateUnitGUIDLists(pQuestDataEx, nUnitId, 0);
}

//D2Game.0x6FC9D6A0
void __fastcall ACT1Q5_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, -1, 0, 1, 2, 3
	};

	if (!pQuestData->bNotIntro)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED) == 1 && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_PRIMARYGOALDONE))
	{
		return;
	}

	const int32_t nNpcId = pQuestArg->pTarget ? pQuestArg->pTarget->dwClassId : -1;
	const int32_t nUnitId = pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1;

	D2Act1Quest5Strc* pQuestDataEx = (D2Act1Quest5Strc*)pQuestData->pQuestDataEx;

	if (pQuestData->fState >= 4)
	{
		if (pQuestDataEx->nUnitCount2 > 0)
		{
			int32_t nCounter1 = 0;
			while (pQuestDataEx->nUnitGUIDs2[nCounter1] != nUnitId)
			{
				++nCounter1;

				if (nCounter1 >= pQuestDataEx->nUnitCount2)
				{
					if (pQuestDataEx->nUnitCount1 <= 0)
					{
						return;
					}

					int32_t nCounter2 = 0;
					while (pQuestDataEx->nUnitGUIDs1[nCounter2] != nUnitId)
					{
						++nCounter2;

						if (nCounter2 >= pQuestDataEx->nUnitCount1)
						{
							return;
						}
					}

					break;
				}
			}
		}
		else
		{
			if (pQuestDataEx->nUnitCount1 <= 0)
			{
				return;
			}

			int32_t nCounter2 = 0;
			while (pQuestDataEx->nUnitGUIDs1[nCounter2] != nUnitId)
			{
				++nCounter2;

				if (nCounter2 >= pQuestDataEx->nUnitCount1)
				{
					return;
				}
			}
		}
	}

	int32_t nCounter1 = 0;
	if (pQuestDataEx->nUnitCount2 > 0)
	{
		while (pQuestDataEx->nUnitGUIDs2[nCounter1] != nUnitId)
		{
			++nCounter1;

			if (nCounter1 >= pQuestDataEx->nUnitCount2)
			{
				if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED) != 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_PRIMARYGOALDONE) != 1)
				{
					const int32_t nIndex = nIndices[pQuestData->fState];
					if (nIndex != -1 && nIndex < 5)
					{
						QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
					}
				}
				else if (pQuestDataEx->nUnitCount1 > 0)
				{
					int32_t nCounter2 = 0;
					while (pQuestDataEx->nUnitGUIDs1[nCounter2] != nUnitId)
					{
						++nCounter2;

						if (nCounter2 >= pQuestDataEx->nUnitCount1)
						{
							return;
						}
					}

					QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
				}

				return;
			}
		}

		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
	}
	else
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED) != 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_PRIMARYGOALDONE) != 1)
		{
			const int32_t nIndex = nIndices[pQuestData->fState];
			if (nIndex != -1 && nIndex < 5)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
			}
		}
		else if (pQuestDataEx->nUnitCount1 > 0)
		{
			int32_t nCounter2 = 0;
			while (pQuestDataEx->nUnitGUIDs1[nCounter2] != nUnitId)
			{
				++nCounter2;

				if (nCounter2 >= pQuestDataEx->nUnitCount1)
				{
					return;
				}
			}

			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
		}
	}
}

//D2Game.0x6FC9D830
void __fastcall ACT1Q5_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest5Strc* pQuestDataEx = (D2Act1Quest5Strc*)pQuestData->pQuestDataEx;

	if (!pQuestData->bNotIntro)
	{
		if (pQuestArg->nOldLevel == LEVEL_ROGUEENCAMPMENT)
		{
			if (pQuestData->fState == 2)
			{
				if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], pQuestData->nQuest, 0) != 1)
				{
					QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
				}
			}
			else if (pQuestData->fState == 5 && pQuestData->bNotIntro)
			{
				if (ACT1Q5_UpdateUnitGUIDLists(pQuestDataEx, pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1, 0) && !pQuestDataEx->nUnitCount1 && !pQuestDataEx->nUnitCount2)
				{
					pQuestData->bActive = 0;
				}
			}
		}
	}
	else
	{
		if (pQuestArg->nNewLevel != LEVEL_FORGOTTENTOWER)
		{
			if (pQuestArg->nNewLevel == LEVEL_TOWERCELLARLEV5)
			{
				if (pQuestData->fState < 4 && pQuestData->fLastState != 2)
				{
					if (pQuestData->fState != 3)
					{
						QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
					}
					pQuestData->dwFlags &= 0xFFFFFF00;
					QUESTS_UnitIterate(pQuestData, 2, 0, ACT1Q5_UnitIterate_StatusCyclerEx, 1);
					SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q5_UnitIterate_UpdateQuestStateFlags);
				}
			}
			else if (pQuestArg->nOldLevel == LEVEL_ROGUEENCAMPMENT)
			{
				if (pQuestData->fState == 2)
				{
					if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], pQuestData->nQuest, 0) != 1)
					{
						QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
					}
				}
				else if (pQuestData->fState == 5 && pQuestData->bNotIntro)
				{
					if (ACT1Q5_UpdateUnitGUIDLists(pQuestDataEx, pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1, 0) && !pQuestDataEx->nUnitCount1 && !pQuestDataEx->nUnitCount2)
					{
						pQuestData->bActive = 0;
					}
				}
			}
		}
		else
		{
			if (!pQuestData->fState)
			{
				QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 3, 0, ACT1Q5_UnitIterate_StatusCyclerEx, 1);
				SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q5_UnitIterate_UpdateQuestStateFlags);
			}
			else if (pQuestData->fState <= 3 && pQuestData->fLastState == 1)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 4, 0, ACT1Q5_UnitIterate_StatusCyclerEx, 1);
				SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q5_UnitIterate_UpdateQuestStateFlags);
			}
		}
	}
}

//D2Game.0x6FC9D970
bool __fastcall ACT1Q5_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (pQuestData->fState < 2 && pQuestData->bNotIntro == 1)
	{
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

	if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
	{
		FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
		exit(-1);
	}
	return pQuest->pfSeqFilter(pQuest);
}

//D2Game.0x6FC9D9F0
void __fastcall ACT1Q5_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act1Quest5Strc* pQuestDataEx = (D2Act1Quest5Strc*)pQuestData->pQuestDataEx;

	UNITS_GetCoords(pQuestArg->pTarget, &pQuestDataEx->pCoord);

	if (pQuestData->bNotIntro)
	{
		QUESTS_DebugOutput(pQuestData->pGame, "Finished quest", __FILE__, __LINE__);
		pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = nullptr;
		QUESTS_StateDebug(pQuestData, 5, __FILE__, __LINE__);
		pQuestDataEx->bTriggerSeqFilter = 1;
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT1Q5_UnitIterate_AttachCompletionSound);
		QUESTS_SetGlobalState(pQuestData->pGame, QUESTSTATEFLAG_A1Q5, QFLAG_PRIMARYGOALDONE);

		if (pQuestDataEx->nUnitCount2)
		{
			pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT1Q5_Callback10_PlayerLeavesGame;
		}
		else
		{
			pQuestData->bActive = 0;
		}

		pQuestDataEx->bCountessKilled = 1;
		pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = nullptr;
		QUESTS_SetGlobalState(pQuestArg->pGame, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q5_UnitIterate_SetRewardGranted);

		for (int32_t i = 0; i < pQuestDataEx->tPlayerGUIDs.nPlayerCount; ++i)
		{
			D2UnitStrc* pUnit = SUNIT_GetServerUnit(pQuestArg->pGame, UNIT_PLAYER, pQuestDataEx->tPlayerGUIDs.nPlayerGUIDs[i]);
			if (pUnit)
			{
				D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pQuestArg->pGame->nDifficulty];
				if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) != 1)
				{
					QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_PRIMARYGOALDONE);
					QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED);
					SUNIT_AttachSound(pUnit, 37, pUnit);
				}
			}
		}

		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q5_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
		SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT1Q5_UnitIterate_SetCompletionFlag);
		QUESTS_CreateTimer(pQuestData, ACT1Q5_Timer_StatusCycler, 7);
	}

	ACT1Q5_SpawnTowerChestMissiles(pQuestData, pQuestDataEx);

	if (!pQuestDataEx->bDeathMissilesCreated)
	{
		EVENT_SetEvent(pQuestData->pGame, pQuestArg->pTarget, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 10, 0, 0);
	}
}

//D2Game.0x6FC9DBE0
int32_t __fastcall ACT1Q5_UnitIterate_SetRewardGranted(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDPENDING))
	{
		return 0;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_TOWERCELLARLEV5)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q5_COUNTESS);
	if (!pQuestData)
	{
		return 0;
	}

	int32_t nUnitId = -1;
	if (pUnit)
	{
		nUnitId = pUnit->dwUnitId;
	}

	D2Act1Quest5Strc* pQuestDataEx = (D2Act1Quest5Strc*)pQuestData->pQuestDataEx;
	if (!QUESTS_QuickCheckPlayerGUID(&pQuestDataEx->tPlayerGUIDs, nUnitId))
	{
		QUESTS_AddPlayerGUID(&pQuestDataEx->tPlayerGUIDs, nUnitId);
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_PRIMARYGOALDONE);
	return 0;
}

//D2Game.0x6FC9DC80
int32_t __fastcall ACT1Q5_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED) == 1)
	{
		return 0;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return 0;
	}

	if (DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_TOWERCELLARLEV5)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_COMPLETEDNOW);
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED);
	QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDPENDING);
	SUNIT_AttachSound(pUnit, 37, pUnit);

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q5_COUNTESS);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act1Quest5Strc* pQuestDataEx = (D2Act1Quest5Strc*)pQuestData->pQuestDataEx;
	const int32_t nUnitId = pUnit ? pUnit->dwUnitId : -1;

	if (pQuestDataEx->nUnitCount2 < 12)
	{
		pQuestDataEx->nUnitGUIDs2[pQuestDataEx->nUnitCount2] = nUnitId;
		++pQuestDataEx->nUnitCount2;
	}
	return 0;
}

//D2Game.0x6FC9DD40
int32_t __fastcall ACT1Q5_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_REWARDGRANTED) == 1)
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_COMPLETEDNOW);
	QUESTS_SendLogUpdateEx(pUnit, QUEST_A1Q5_COUNTESS, 0);
	return 0;
}

//D2Game.0x6FC9DD90
int32_t __fastcall ACT1Q5_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A1Q5, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT1Q5_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FC9DDE0
bool __fastcall ACT1Q5_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act1Quest5Strc* pQuestDataEx = (D2Act1Quest5Strc*)pQuestData->pQuestDataEx;

	if (pQuestData->fState == 5)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 13, 0, ACT1Q5_UnitIterate_StatusCyclerEx, 1);
	}

	pQuestDataEx->unk0x11C = 0;

	return true;
}

//D2Game.0x6FC9DE10
void __fastcall ACT1Q5_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuest, QFLAG_COMPLETEDBEFORE))
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_ENTERAREA) == 1)
	{ // NOLINT(bugprone-branch-clone)
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_CUSTOM2) == 1)
	{
		pQuestData->fLastState = 4;
		pQuestData->fState = 3;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_CUSTOM1) == 1)
	{
		pQuestData->fLastState = 3;
		pQuestData->fState = 2;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_LEAVETOWN) == 1)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 1;
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q5, QFLAG_STARTED) == 1)
	{
		pQuestData->fState = 2;
		pQuestData->fLastState = 1;
	}
}

//D2Game.0x6FC9DEE0
void __fastcall OBJECTS_InitFunction04_TowerTome(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q5_COUNTESS);
	D2_ASSERT(pQuestData);

	if (!pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_SPECIAL1);
	}
}

//D2Game.0x6FC9DF30
void __fastcall OBJECTS_InitFunction47_CountessChest(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q5_COUNTESS);
	if (!pQuestData)
	{
		return;
	}

	D2Act1Quest5Strc* pQuestDataEx = (D2Act1Quest5Strc*)pQuestData->pQuestDataEx;
	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}

	if (pQuestDataEx->nObjectCount <= 0)
	{
		if (pQuestDataEx->nObjectCount < 8)
		{
			pQuestDataEx->nObjectGUIDs[pQuestDataEx->nObjectCount] = nUnitId;
			++pQuestDataEx->nObjectCount;
		}

		ACT1Q5_SpawnTowerChestMissiles(pQuestData, pQuestDataEx);

		if (pQuestDataEx->bCountessKilled && !pQuestDataEx->bDeathMissilesCreated)
		{
			EVENT_SetEvent(pQuestData->pGame, pOp->pObject, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 10, 0, 0);
		}
	}
	else
	{
		int32_t nCounter = 0;
		while (pQuestDataEx->nObjectGUIDs[nCounter] != nUnitId)
		{
			++nCounter;
			if (nCounter >= pQuestDataEx->nObjectCount)
			{
				if (pQuestDataEx->nObjectCount < 8)
				{
					pQuestDataEx->nObjectGUIDs[pQuestDataEx->nObjectCount] = nUnitId;
					++pQuestDataEx->nObjectCount;
				}

				ACT1Q5_SpawnTowerChestMissiles(pQuestData, pQuestDataEx);

				if (pQuestDataEx->bCountessKilled && !pQuestDataEx->bDeathMissilesCreated)
				{
					EVENT_SetEvent(pQuestData->pGame, pOp->pObject, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 10, 0, 0);
				}
				return;
			}
		}
	}
}
