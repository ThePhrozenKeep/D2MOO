#include "QUESTS/ACT5/A5Q6.h"

#include <D2BitManip.h>

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2QuestRecord.h>

#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/Level.h"
#include "GAME/SCmd.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterSpawn.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/Player.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT5/A5Q5.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "SKILLS/Skills.h"


//D2Game.0x6FD3E560
D2NPCMessageTableStrc gpAct5Q6NpcMessages[] =
{
	{
		{
			{ MONSTER_LARZUK, 20171, 0, 2, },
			{ MONSTER_CAIN6, 20170, 0, 2, },
			{ MONSTER_MALAH, 20172, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20174, 0, 2, },
			{ MONSTER_NIHLATHAK, 20173, 0, 2, },
		},
		5
	},
	{
		{
			{ MONSTER_LARZUK, 20178, 0, 0, },
			{ MONSTER_CAIN6, 20177, 0, 0, },
			{ MONSTER_MALAH, 20179, 0, 0, },
			{ MONSTER_TYRAEL3, 20175, 0, 0, },
			{ MONSTER_QUAL_KEHK, 20180, 0, 0, },
			{ MONSTER_DREHYA, 20176, 0, 0, },
		},
		6
	},
	{
		{
			{ MONSTER_LARZUK, 20178, 0, 0, },
			{ MONSTER_MALAH, 20179, 0, 0, },
			{ MONSTER_TYRAEL3, 20175, 0, 0, },
			{ MONSTER_QUAL_KEHK, 20180, 0, 0, },
			{ MONSTER_DREHYA, 20176, 0, 0, },
		},
		5
	},
	{
		{
			{ MONSTER_LARZUK, 20178, 0, 2, },
			{ MONSTER_MALAH, 20179, 0, 2, },
			{ MONSTER_TYRAEL3, 20175, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20180, 0, 2, },
			{ MONSTER_NIHLATHAK, 20176, 0, 2, },
		},
		5
	},
	{
		{
			{ MONSTER_CAIN6, 20177, 0, 0, },
		},
		1
	},
	{
		{
			{ MONSTER_CAIN6, 20177, 0, 2, },
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
	}
};


//D2Game.0x6FCB8320
void __fastcall ACT5Q6_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDPENDING) == 1)
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

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_PRIMARYGOALDONE);

	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
	CLIENTS_UpdateCharacterProgression(pClient, 5, pGame->nDifficulty);
}

//D2Game.0x6FCB83B0
bool __fastcall ACT5Q6_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId >= MONSTER_ANCIENTSTATUE1 && nNpcId <= MONSTER_ANCIENTSTATUE3)
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pQuest->pGame, QUEST_A5Q5_ANCIENTS);
		if (pQuestData)
		{
			D2Act5Quest5Strc* pQuestDataEx = (D2Act5Quest5Strc*)pQuestData->pQuestDataEx;
			if (pQuestDataEx->bAncientsActivated && !pQuestDataEx->nAncientsAlive && pQuest->fState == 1 && !((D2Act5Quest6Strc*)pQuest->pQuestDataEx)->bQuestStarted)
			{
				return true;
			}
		}
	}

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED))
	{
		return false;
	}

	int32_t nFlag = 0;
	switch (nNpcId)
	{
	case MONSTER_LARZUK:
		nFlag = QFLAG_ENTERAREA;
		break;

	case MONSTER_CAIN6:
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_CUSTOM6))
		{
			return false;
		}
		nFlag = QFLAG_CUSTOM1;
		break;

	case MONSTER_MALAH:
		nFlag = QFLAG_CUSTOM2;
		break;

	case MONSTER_TYRAEL3:
		nFlag = QFLAG_CUSTOM3;
		break;

	case MONSTER_QUAL_KEHK:
		nFlag = QFLAG_CUSTOM4;
		break;

	case MONSTER_DREHYA:
		nFlag = QFLAG_CUSTOM5;
		break;

	default:
		return false;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, nFlag))
	{
		return false;
	}

	return true;
}

//D2Game.0x6FCB8490
void __fastcall ACT5Q6_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT5Q6_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT5Q6_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_MONSTERKILLED] = ACT5Q6_Callback08_MonsterKilled;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT5Q6_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT5Q6_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT5Q6_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT5Q6_Callback13_PlayerStartedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERJOINEDGAME] = ACT5Q6_Callback14_PlayerJoinedGame;
	pQuestData->pNPCMessages = gpAct5Q6NpcMessages;
	pQuestData->bActive = 1;
	
	D2Act5Quest6Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act5Quest6Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->nQuestFilter = QUESTSTATEFLAG_A5Q6;
	pQuestData->pfStatusFilter = 0;
	pQuestData->nInitNo = 4;
	pQuestData->pfActiveFilter = ACT5Q6_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT5Q6_SeqCallback;
	pQuestData->nSeqId = 37;
	memset(pQuestDataEx, 0x00, sizeof(D2Act5Quest6Strc));
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->tPlayerGUIDs);
	pQuestDataEx->nBaalPortalObjectMode = OBJMODE_OPERATING;
	pQuestDataEx->nLastPortalObjectMode = OBJMODE_OPERATING;
}

//D2Game.0x6FCB8580
void __fastcall ACT5Q6_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act5Quest6Strc* pQuestDataEx = (D2Act5Quest6Strc*)pQuestData->pQuestDataEx;

	if (pQuestArg->pTarget && pQuestArg->pTarget->dwClassId == MONSTER_TYRAEL3 && !pQuestDataEx->bLastPortalOpened)
	{
		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, pQuestArg->pPlayer, ACT5Q6_UnitIterate_CreateLastPortal);
		pQuestDataEx->bLastPortalOpened = 1;
	}
}

//D2Game.0x6FCB85C0
int32_t __fastcall ACT5Q6_UnitIterate_CreateLastPortal(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_THEWORLDSTONECHAMBER)
	{
		return 0;
	}

	D2CoordStrc pCoord = {};
	UNITS_GetCoords(pUnit, &pCoord);
	pCoord.nX += 5;
	pRoom = UNITS_GetRoom(pUnit);
	QUESTS_GetFreePosition(pRoom, &pCoord, 5, 0x400, &pRoom, 18);
	if (pRoom)
	{
		SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_LAST_PORTAL, pCoord.nX, pCoord.nY, pGame, pRoom, 1, 1, 0);
	}

	return 1;
}

//D2Game.0x6FCB8660
void __fastcall ACT5Q6_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	switch (pQuestArg->nMessageIndex)
	{
	case 20178:
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuestFilter, QFLAG_ENTERAREA);
		break;

	case 20177:
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuestFilter, QFLAG_CUSTOM1);
		break;

	case 20179:
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuestFilter, QFLAG_CUSTOM2);
		break;

	case 20175:
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuestFilter, QFLAG_CUSTOM3);
		break;

	case 20180:
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuestFilter, QFLAG_CUSTOM4);
		break;

	case 20176:
		QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty], pQuestData->nQuestFilter, QFLAG_CUSTOM5);
		break;

	default:
		return;
	}
}

//D2Game.0x6FCB8710
void __fastcall ACT5Q6_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, -1, -1, 0, 1, 2
	};

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	D2Act5Quest6Strc* pQuestDataEx = (D2Act5Quest6Strc*)pQuestData->pQuestDataEx;

	if (nNpcId < MONSTER_ANCIENTSTATUE1 || nNpcId > MONSTER_ANCIENTSTATUE3)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED))
		{
			switch (nNpcId)
			{
			case MONSTER_LARZUK:
				if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_ENTERAREA))
				{
					QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
					return;
				}
				break;

			case MONSTER_CAIN6:
				if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_CUSTOM6))
				{
					if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_CUSTOM1) && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_PRIMARYGOALDONE))
					{
						QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 5);
					}
					else
					{
						QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 4);
					}
				}
				return;

			case MONSTER_MALAH:
				if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_CUSTOM2))
				{
					QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
					return;
				}
				break;

			case MONSTER_TYRAEL3:
				if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_PRIMARYGOALDONE))
				{
					QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
				}
				return;

			case MONSTER_QUAL_KEHK:
				if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_CUSTOM4))
				{
					QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
					return;
				}
				break;

			case MONSTER_DREHYA:
				if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_CUSTOM5))
				{
					QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 2);
					return;
				}
				break;

			default:
				return;
			}

			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_PRIMARYGOALDONE))
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
			}
			return;
		}

		if (pQuestData->fState < 4)
		{
			const int32_t nIndex = nIndices[pQuestData->fState];
			if (nIndex != -1 && nIndex < 9)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
			}
		}
	}
	else
	{
		D2QuestDataStrc* pQuestData35 = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A5Q5_ANCIENTS);
		if (!pQuestData35)
		{
			return;
		}

		D2Act5Quest5Strc* pQuestDataEx35 = (D2Act5Quest5Strc*)pQuestData35->pQuestDataEx;
		if (!pQuestDataEx35->bAncientsActivated || pQuestDataEx35->nAncientsAlive)
		{
			return;
		}

		if (pQuestData->fState == 1 && !pQuestDataEx->bQuestStarted)
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
			return;
		}

		if (pQuestData->fState != 2 || pQuestDataEx->bQuestStarted == 0)
		{
			return;
		}

		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 3);
	}
}

//D2Game.0x6FCB8910
void __fastcall ACT5Q6_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act5Quest6Strc* pQuestDataEx = (D2Act5Quest6Strc*)pQuestData->pQuestDataEx;

	QUESTS_TriggerFX(pQuestData->pGame, 19);

	if (pQuestData->bNotIntro)
	{
		pQuestDataEx->pRoom = UNITS_GetRoom(pQuestArg->pTarget);
		if (!pQuestDataEx->pRoom)
		{
			return;
		}

		QUESTS_UnitIterate(pQuestData, 4, 0, ACT5Q6_UnitIterate_StatusCyclerEx, 1);

		if (pQuestArg->pPlayer)
		{
			pQuestDataEx->nPlayersInWorldStoneChamber = 0;

			SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT5Q6_UnitIterate_SetRewardGranted);
			SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT5Q6_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
			SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT5Q6_UnitIterate_SetCompletionFlag);
			SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT5Q6_UnitIterate_AttachCompletionSound);

			if (pQuestData->bNotIntro && !QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty], QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED))
			{
				const int32_t nMin = 6000 * pQuestData->pGame->nDifficulty + 1500;
				int32_t nMax = 6000 * pQuestData->pGame->nDifficulty + 3000;
				if (nMax >= 65535)
				{
					nMax = 65535;
				}

				for (int32_t i = 0; i < pQuestDataEx->nPlayersInWorldStoneChamber; ++i)
				{
					//D2Game_10034_Return(95);
					ITEMS_DropGoldPile(pQuestData->pGame, pQuestArg->pTarget, nMin + ITEMS_RollLimitedRandomNumber(QUESTS_GetGlobalSeed(pQuestData->pGame), nMax - nMin));
				}
			}
			sub_6FC37B10(pQuestData->pGame);
		}

		if (pQuestData->fState != 5)
		{
			pQuestData->fState = 5;
		}
	}

	D2UnitStrc* pMissile = D2GAME_CreateMissile_6FD115E0(pQuestArg->pGame, pQuestArg->pTarget, 0, 1, 625, CLIENTS_GetUnitX(pQuestArg->pTarget), CLIENTS_GetUnitY(pQuestArg->pTarget));
	if (pMissile)
	{
		DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pMissile), 0);
	}
}

//D2Game.0x6FCB8B80
int32_t __fastcall ACT5Q6_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A5Q6_BAAL);
	}

	return 0;
}

//D2Game.0x6FCB8BF0
int32_t __fastcall ACT5Q6_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q6, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT5Q6_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCB8C40
int32_t __fastcall ACT5Q6_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED) == 1)
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_COMPLETEDNOW);
	QUESTS_SendLogUpdateEx(pUnit, QUEST_A5Q6_BAAL, 0);
	return 0;
}

//D2Game.0x6FCB8C90
int32_t __fastcall ACT5Q6_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q6, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	D2GSPacketSrv5D packet5D = {};
	packet5D.nHeader = 0x5D;
	packet5D.nQuestId = QUEST_A5Q6_BAAL;
	packet5D.nFlags = 2;
	packet5D.field_4 = 0;
	D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet5D, sizeof(packet5D));

	SUNIT_AttachSound(pUnit, 83, pUnit);
	return 0;
}

//D2Game.0x6FCB8D10
int32_t __fastcall ACT5Q6_UnitIterate_SetRewardGranted(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED) == 1)
	{
		return 0;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom || DUNGEON_GetLevelIdFromRoom(pRoom) != LEVEL_THEWORLDSTONECHAMBER)
	{
		return 0;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDPENDING))
	{
		return 0;
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_PRIMARYGOALDONE);
	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED);

	CLIENTS_UpdateCharacterProgression(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 5, pGame->nDifficulty);

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q6_BAAL);
	if (pQuestData)
	{
		++((D2Act5Quest6Strc*)pQuestData->pQuestDataEx)->nPlayersInWorldStoneChamber;
	}

	return 0;
}

//D2Game.0x6FCB8DE0
void __fastcall ACT5Q6_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel == LEVEL_HARROGATH)
	{
		D2GSPacketSrv50 packet50 = {};
		packet50.nHeader = 0x50;
		packet50.nQuestId = QUEST_A5Q6_BAAL;
		packet50.A5Q6_BaalPayload.nZooMonsterId = ((D2Act5Quest6Strc*)QUESTS_GetQuestData(pQuestArg->pGame, QUEST_A5Q6_BAAL)->pQuestDataEx)->nZooMonsterId;
		D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pQuestArg->pPlayer, __FILE__, __LINE__), &packet50, sizeof(packet50));
	}

	if (pQuestArg->nOldLevel == LEVEL_HARROGATH)
	{
		QUESTS_QuickRemovePlayerGUID(pQuestData, pQuestArg);

		if (pQuestData->fState == 2)
		{
			D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

			if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDPENDING) == 1)
			{
				return;
			}

			QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);

			if (pQuestData->fLastState != 1)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q6_UnitIterate_StatusCyclerEx, 1);
			}

			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q6_UnitIterate_UpdateQuestStateFlags);
		}
	}

	if (pQuestArg->nNewLevel >= LEVEL_THRONEOFDESTRUCTION)
	{
		if (pQuestData->bNotIntro && pQuestData->fLastState < 2)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 2, 0, ACT5Q6_UnitIterate_StatusCyclerEx, 1);
		}

		SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q6_UnitIterate_UpdateQuestStateFlags);
	}
}

//D2Game.0x6FCB8F30
int32_t __fastcall ACT5Q6_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return 0;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || DRLG_GetActNoFromLevelId(nLevelId) != ACT_V)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q6_BAAL);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_LEAVETOWN);
	}

	return 0;
}

//D2Game.0x6FCB8FD0
bool __fastcall ACT5Q6_SeqCallback(D2QuestDataStrc* pQuestData)
{
	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A5Q6_BAAL);

	if (pQuest->bNotIntro && !pQuest->fState)
	{
		((D2Act5Quest6Strc*)pQuest->pQuestDataEx)->bQuestStarted = 1;
		pQuest->fState = 2;
		pQuest->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuest, 1, 0, ACT5Q6_UnitIterate_StatusCyclerEx, 1);
		SUNIT_IterateUnitsOfType(pQuest->pGame, 0, 0, ACT5Q6_UnitIterate_UpdateQuestStateFlags);
	}

	return true;
}

//D2Game.0x6FCB9030
void __fastcall ACT5Q6_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_CUSTOM6);
	}
}

//D2Game.0x6FCB9070
void __fastcall ACT5Q6_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_COMPLETEDBEFORE))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_CUSTOM6);
		D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pQuestArg->pPlayer, __FILE__, __LINE__);
		CLIENTS_UpdateCharacterProgression(pClient, 5, pQuestData->pGame->nDifficulty);
	}
	else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDPENDING) != 1)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_LEAVETOWN) == 1)
		{
			pQuestData->fLastState = 1;
			pQuestData->fState = 3;
		}
		else if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_STARTED) == 1)
		{
			((D2Act5Quest6Strc*)pQuestData->pQuestDataEx)->bQuestStarted = 1;
			pQuestData->fLastState = 1;
			pQuestData->fState = 3;
		}
	}
}

//D2Game.0x6FCB9150
void __fastcall ACT5Q6_OpenWorldStoneChamber(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q6_BAAL);
	if (!pQuestData)
	{
		return;
	}

	((D2Act5Quest6Strc*)pQuestData->pQuestDataEx)->bWorldStoneChamberOpen = true;

	if (pQuestData->bNotIntro && pQuestData->fLastState < 3)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 3, 0, ACT5Q6_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCB9190
int32_t __fastcall ACT5Q6_IsWorldStoneChamberClosed(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q6_BAAL);
	if (!pQuestData)
	{
		return 1;
	}

	return ((D2Act5Quest6Strc*)pQuestData->pQuestDataEx)->bWorldStoneChamberOpen != 1;
}

//D2Game.0x6FCB91C0
void __fastcall OBJECTS_InitFunction75_BaalsPortal(D2ObjInitFnStrc* pOp)
{
	D2Act5Quest6Strc* pQuestDataEx = (D2Act5Quest6Strc*)QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q6_BAAL)->pQuestDataEx;

	UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nBaalPortalObjectMode);
	pQuestDataEx->nBaalPortalObjectMode = OBJMODE_OPENED;
}

//D2Game.0x6FCB91F0
int32_t __fastcall OBJECTS_OperateFunction70_BaalPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q6_BAAL);
	if (!pQuestData)
	{
		return 1;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pOp->pPlayer);
	if (!pRoom)
	{
		return 1;
	}

	if (DUNGEON_GetLevelIdFromRoom(pRoom) == LEVEL_THRONEOFDESTRUCTION)
	{
		if (((D2Act5Quest6Strc*)pQuestData->pQuestDataEx)->bWorldStoneChamberOpen == 1)
		{
			LEVEL_WarpUnit(pOp->pGame, pOp->pPlayer, LEVEL_THEWORLDSTONECHAMBER, 11);
		}
	}
	else
	{
		LEVEL_WarpUnit(pOp->pGame, pOp->pPlayer, LEVEL_THRONEOFDESTRUCTION, 0);
	}

	return 1;
}

//D2Game.0x6FCB9260
void __fastcall OBJECTS_InitFunction77_LastPortal(D2ObjInitFnStrc* pOp)
{
	D2Act5Quest6Strc* pQuestDataEx = (D2Act5Quest6Strc*)QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q6_BAAL)->pQuestDataEx;

	UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nLastPortalObjectMode);
	pQuestDataEx->nLastPortalObjectMode = OBJMODE_OPENED;
}

//D2Game.0x6FCB9290
int32_t __fastcall OBJECTS_OperateFunction72_LastPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q6_BAAL);
	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		return 0;
	}

	if (((D2Act5Quest6Strc*)pQuestData->pQuestDataEx)->nLastPortalObjectMode != OBJMODE_OPENED)
	{
		return 1;
	}

	D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pOp->pPlayer);
	D2BitBufferStrc* pQuestFlags = pPlayerData->pQuestData[pOp->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q6, QFLAG_PRIMARYGOALDONE))
	{
		LEVEL_WarpUnit(pOp->pGame, pOp->pPlayer, LEVEL_HARROGATH, 0);

		D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pOp->pPlayer, "..\\D2Game/Quests/a5q6.cpp", 923);
		sub_6FC37B10(pQuestData->pGame);

		if (pClient && !PLAYER_IsBusy(pOp->pPlayer))
		{
			SUNIT_ResetInteractInfo(pOp->pPlayer);
			pPlayerData->bBusy = 1;

			D2GSPacketSrv61 packet61 = {};
			packet61.nHeader = 0x61;
			packet61.unk0x001 = 7;
			D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet61, sizeof(packet61));

			QUESTRECORD_SetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_CUSTOM6);
		}
	}
	else
	{
		SUNIT_AttachSound(pOp->pPlayer, 19, pOp->pPlayer);
	}

	return 1;
}

//D2Game.0x6FCB9370
void __fastcall OBJECTS_InitFunction79_Zoo(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q6_BAAL);
	D2Act5Quest6Strc* pQuestDataEx = (D2Act5Quest6Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bZooInitialized)
	{
		return;
	}

	for (int32_t i = 0; i < 10; ++i)
	{
		pQuestDataEx->nZooMonsterId = ITEMS_RollLimitedRandomNumber(QUESTS_GetGlobalSeed(pOp->pGame), sgptDataTables->nMonStatsTxtRecordCount - 1) + 1;

		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pQuestDataEx->nZooMonsterId);
		if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_ZOO])
		{
			pQuestDataEx->bZooInitialized = 1;
			break;
		}

		if (pQuestDataEx->bZooInitialized)
		{
			break;
		}
	}

	if (!pQuestDataEx->bZooInitialized)
	{
		pQuestDataEx->bZooInitialized = 1;
		pQuestDataEx->nZooMonsterId = 0;
	}

	if (pQuestDataEx->bZooInitialized)
	{
		SUNIT_IterateUnitsOfType(pOp->pGame, 0, 0, ACT5Q6_UnitIterate_SendZooMonsterIdToClient);
	}
}

//D2Game.0x6FCB94C0
int32_t __fastcall ACT5Q6_UnitIterate_SendZooMonsterIdToClient(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);

	D2GSPacketSrv50 packet50 = {};
	packet50.nHeader = 0x50u;
	packet50.nQuestId = QUEST_A5Q6_BAAL;
	packet50.A5Q6_BaalPayload.nZooMonsterId = ((D2Act5Quest6Strc*)QUESTS_GetQuestData(pGame, QUEST_A5Q6_BAAL)->pQuestDataEx)->nZooMonsterId;
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet50, sizeof(packet50));

	return 0;
}

//D2Game.0x6FCB9520
void __fastcall ACT5Q6_SpawnTyrael(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2UnitStrc* pUnit)
{
	D2CoordStrc coord = {};
	UNITS_GetCoords(pUnit, &coord);
	coord.nX -= 5;
	coord.nY -= 5;

	D2ActiveRoomStrc* pFreeRoom = nullptr;
	QUESTS_GetFreePosition(pRoom, &coord, 5, 0x400u, &pFreeRoom, 19);

	if (pFreeRoom)
	{
		D2GAME_SpawnMonster_6FC69F10(pGame, pFreeRoom, coord.nX, coord.nY, MONSTER_TYRAEL3, MONMODE_NEUTRAL, 4, 66);
	}
}

//
void __fastcall ACT5Q6_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	QUESTS_FastRemovePlayerGUID(&pQuestData->tPlayerGUIDs, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1));

	D2Act5Quest6Strc* pQuestDataEx = (D2Act5Quest6Strc*)pQuestData->pQuestDataEx;
	if (pQuestArg->pPlayer)
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, pQuestArg->pPlayer->dwUnitId);
	}
	else
	{
		QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, -1);
	}
}
