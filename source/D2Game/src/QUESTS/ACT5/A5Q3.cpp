#include "QUESTS/ACT5/A5Q3.h"

#include <algorithm>

#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgPreset.h>
#include <Units/UnitRoom.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2QuestRecord.h>
#include <D2StatList.h>

#include "AI/AiGeneral.h"
#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/PlrMsg.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT5/A5Q4.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitInactive.h"


//D2Game.0x6FD3CE00
D2NPCMessageTableStrc gpAct5Q3NpcMessages[] =
{
	{
		{
			{ MONSTER_MALAH, 20116, 0, 0, },
		},
		1
	},
	{
		{
			{ MONSTER_LARZUK, 20119, 0, 2, },
			{ MONSTER_CAIN6, 20118, 0, 2, },
			{ MONSTER_MALAH, 20117, 0, 2, },
			{ MONSTER_NIHLATHAK, 20120, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20121, 0, 2, },
		},
		5
	},
	{
		{
			{ MONSTER_LARZUK, 20124, 0, 2, },
			{ MONSTER_CAIN6, 20123, 0, 2, },
			{ MONSTER_MALAH, 20122, 0, 2, },
			{ MONSTER_NIHLATHAK, 20125, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20126, 0, 2, },
		},
		5
	},
	{
		{
			{ MONSTER_LARZUK, 20129, 0, 2, },
			{ MONSTER_CAIN6, 20128, 0, 2, },
			{ MONSTER_DREHYAICED, 20131, 0, 0, },
			{ MONSTER_MALAH, 20127, 0, 0, },
			{ MONSTER_QUAL_KEHK, 20130, 0, 2, },
		},
		5
	},
	{
		{
			{ MONSTER_LARZUK, 20129, 0, 2, },
			{ MONSTER_CAIN6, 20128, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20130, 0, 2, },
		},
		3
	},
	{
		{
			{ MONSTER_LARZUK, 20134, 0, 2, },
			{ MONSTER_CAIN6, 20133, 0, 2, },
			{ MONSTER_DREHYA, 20136, 0, 0, },
			{ MONSTER_MALAH, 20132, 0, 0, },
			{ MONSTER_QUAL_KEHK, 20135, 0, 2, },
		},
		5
	},
	{
		{
			{ MONSTER_LARZUK, 20134, 0, 2, },
			{ MONSTER_CAIN6, 20133, 0, 2, },
			{ MONSTER_DREHYA, 20136, 0, 2, },
			{ MONSTER_MALAH, 20132, 0, 2, },
			{ MONSTER_QUAL_KEHK, 20135, 0, 2, },
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


//D2Game.0x6FCB3530
void __fastcall ACT5Q3_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDGRANTED) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING) == 1)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (nLevelId && DRLG_GetActNoFromLevelId(nLevelId) == ACT_V)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_PRIMARYGOALDONE);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING);
	}
}

//D2Game.0x6FCB35A0
void __fastcall ACT5Q3_RemoveNihlathakFromTown(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return;
	}

	D2UnitStrc* pNihlathak = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_MONSTER, ((D2Act5Quest3Strc*)pQuestData->pQuestDataEx)->NihlathakMonsterInTownGUID);
	if (!pNihlathak)
	{
		return;
	}

	D2MonsterInteractStrc* pMonInteract = nullptr;
	if (pNihlathak->dwUnitType == UNIT_MONSTER && pNihlathak->pMonsterData)
	{
		pMonInteract = pNihlathak->pMonsterData->pMonInteract;
	}

	if (MONSTERAI_HasInteractUnit(pMonInteract))
	{
		D2GAME_MONSTERAI_Last_6FC61F70(pQuestData->pGame, pNihlathak, 0);
	}

	D2Common_10153(pNihlathak->pDynamicPath);
	D2GAME_EVENTS_Delete_6FC34840(pQuestData->pGame, pNihlathak, EVENTTYPE_AITHINK, 0);
	STATLIST_SetUnitStat(pNihlathak, STAT_HITPOINTS, 0, 0);
	
	D2ModeChangeStrc modeChange = {};
	MONSTERMODE_GetModeChangeInfo(pNihlathak, MONMODE_DEAD, &modeChange);

	modeChange.nX = 0;
	modeChange.nY = 0;
	D2GAME_ModeChange_6FC65220(pQuestData->pGame, &modeChange, 1);

	UNITROOM_RefreshUnit(pNihlathak);

	pNihlathak->dwFlags |= UNITFLAG_DOUPDATE;
}

//D2Game.0x6FCB3670
bool __fastcall ACT5Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC)
{
	if (nNpcId == MONSTER_MALAH)
	{
		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuestFilter, QFLAG_REWARDPENDING) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM4))
		{
			return true;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuestFilter, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuestFilter, QFLAG_REWARDPENDING))
		{
			return false;
		}

		if (pQuest->fState == 1)
		{
			return true;
		}

		if (pQuest->fState == 4 && ((D2Act5Quest3Strc*)pQuest->pQuestDataEx)->nDefrostPotionsInGame == 0)
		{
			return true;
		}
	}
	else if (nNpcId == MONSTER_DREHYAICED)
	{
		if (pQuest->bNotIntro && pQuest->fState < 5 && !ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' eci'))
		{
			return true;
		}
	}
	else if (nNpcId == MONSTER_DREHYA)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuestFilter, QFLAG_REWARDPENDING))
		{
			return false;
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM5) == 0)
		{
			return true;
		}
	}

	return false;
}

//D2Game.0x6FCB3760
void __fastcall ACT5Q3_UpdateResistances(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue)
{
	int32_t nUnitType = 6;
	int32_t nUnitId = -1;
	if (pUnit)
	{
		nUnitType = pUnit->dwUnitType;
		nUnitId = pUnit->dwUnitId;
	}

	D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 0, 0, nUnitType, nUnitId);
	if (!pStatList)
	{
		return;
	}

	D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);

	STATLIST_SetStatIfListIsValid(pStatList, STAT_FIRERESIST, nValue, 0);
	STATLIST_SetStatIfListIsValid(pStatList, STAT_LIGHTRESIST, nValue, 0);
	STATLIST_SetStatIfListIsValid(pStatList, STAT_COLDRESIST, nValue, 0);
	STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONRESIST, nValue, 0);

	D2GAME_UpdateAttribute_6FC822D0(pUnit, STAT_FIRERESIST, nValue, pUnit);
	D2GAME_UpdateAttribute_6FC822D0(pUnit, STAT_LIGHTRESIST, nValue, pUnit);
	D2GAME_UpdateAttribute_6FC822D0(pUnit, STAT_COLDRESIST, nValue, pUnit);
	D2GAME_UpdateAttribute_6FC822D0(pUnit, STAT_POISONRESIST, nValue, pUnit);
}

//D2Game.0x6FCB3810
void __fastcall ACT5Q3_ApplyResistanceReward(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);

	int32_t nValue = 0;
	if (QUESTRECORD_GetQuestState(pPlayerData->pQuestData[0], QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM3))
	{
		nValue = 10;
	}

	if (QUESTRECORD_GetQuestState(pPlayerData->pQuestData[1], QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM3))
	{
		nValue += 10;
	}

	if (QUESTRECORD_GetQuestState(pPlayerData->pQuestData[2], QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM3))
	{
		nValue += 10;
	}

	if (nValue)
	{
		ACT5Q3_UpdateResistances(pGame, pUnit, nValue);
	}
}

//D2Game.0x6FCB3880
void __fastcall ACT5Q3_UnitIterate_CheckForDefrostPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!ITEMS_FindQuestItem(pGame, pUnit, ' eci'))
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q3_MALUS); //TODO: Probably a bug!
	if (pQuestData)
	{
		((D2Act5Quest3Strc*)pQuestData->pQuestDataEx)->bDefrostPotionAcquired = 1;
	}
}

//D2Game.0x6FCB38B0
void __fastcall ACT5Q3_InitQuestData(D2QuestDataStrc* pQuestData)
{
	memset(pQuestData->pfCallback, 0x00, sizeof(pQuestData->pfCallback));
	pQuestData->fState = 0;
	pQuestData->pfCallback[QUESTEVENT_PLAYERLEAVESGAME] = ACT5Q3_Callback10_PlayerLeavesGame;
	pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL] = ACT5Q3_Callback03_ChangedLevel;
	pQuestData->pfCallback[QUESTEVENT_NPCACTIVATE] = ACT5Q3_Callback00_NpcActivate;
	pQuestData->pfCallback[QUESTEVENT_NPCDEACTIVATE] = ACT5Q3_Callback02_NpcDeactivate;
	pQuestData->pfCallback[QUESTEVENT_SCROLLMESSAGE] = ACT5Q3_Callback11_ScrollMessage;
	pQuestData->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME] = ACT5Q3_Callback13_PlayerStartedGame;
	pQuestData->pfCallback[QUESTEVENT_PLAYERJOINEDGAME] = ACT5Q3_Callback14_PlayerJoinedGame;
	pQuestData->pNPCMessages = gpAct5Q3NpcMessages;
	pQuestData->bActive = 1;
	
	D2Act5Quest3Strc* pQuestDataEx = D2_ALLOC_STRC_POOL(pQuestData->pGame->pMemoryPool, D2Act5Quest3Strc);
	pQuestData->pQuestDataEx = pQuestDataEx;
	pQuestData->nQuestFilter = 37;
	pQuestData->pfStatusFilter = ACT5Q3_StatusFilterCallback;
	pQuestData->nInitNo = 5;
	pQuestData->pfActiveFilter = ACT5Q3_ActiveFilterCallback;
	pQuestData->pfSeqFilter = ACT5Q3_SeqCallback;
	pQuestData->nSeqId = 34;
	memset(pQuestDataEx, 0x00, sizeof(D2Act5Quest3Strc));
	QUESTS_ResetPlayerGUIDCount(&pQuestDataEx->tPlayerGUIDs);
	pQuestDataEx->unk0x84 = 0;
	pQuestDataEx->unk0x88 = 0;
}

//D2Game.0x6FCB39A0
void __fastcall ACT5Q3_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (!pQuestArg->pTarget || pQuestArg->pTarget->dwClassId != MONSTER_MALAH)
	{
		return;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;

	if (pQuestDataEx->bMalahIntroActivated)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);
		pQuestDataEx->bMalahIntroActivated = 0;
	}

	if (pQuestData->bNotIntro && pQuestDataEx->bMalahActivated && pQuestData->fLastState < 4)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 4, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);
		pQuestDataEx->bMalahActivated = 0;
	}
}

//D2Game.0x6FCB3A20
int32_t __fastcall ACT5Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDGRANTED) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_COMPLETEDBEFORE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_PRIMARYGOALDONE)
		|| QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_COMPLETEDNOW))
	{
		QUESTS_StatusCyclerEx(pGame, pUnit, QUEST_A5Q3_PRISONOFICE);
	}

	return 0;
}

//D2Game.0x6FCB3A90
bool __fastcall ACT5Q3_SeqCallback(D2QuestDataStrc* pQuestData)
{
	if (pQuestData->fState < 5 && pQuestData->bNotIntro)
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

//D2Game.0x6FCB3B00
void __fastcall ACT5Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const uint32_t gdwNormalAmazonRewardCodes[] =
	{
		' 1ma', ' 2ma', ' 3ma', ' 4ma', ' 5ma',
	};

	static const uint32_t gdwExceptionalAmazonRewardCodes[]
	{
		' 6ma', ' 7ma', ' 8ma', ' 9ma', ' ama',
	};

	static const uint32_t gdwEliteAmazonRewardCodes[] =
	{
		' bma', ' cma', ' dma', ' ema', ' fma',
	};

	static const uint32_t gdwNormalBarbarianRewardCodes[] =
	{
		' 1ab', ' 2ab', ' 3ab', ' 4ab', ' 5ab',
	};

	static const uint32_t gdwExceptionalBarbarianRewardCodes[]
	{
		' 6ab', ' 7ab', ' 8ab', ' 9ab', ' aab',
	};

	static const uint32_t gdwEliteBarbarianRewardCodes[] =
	{
		' bab', ' cab', ' dab', ' eab', ' fab',
	};

	static const uint32_t gdwNormalNecromancerRewardCodes[] =
	{
		' 1en', ' 2en', ' 3en', ' 4en', ' 5en',
	};

	static const uint32_t gdwExceptionalNecromancerRewardCodes[]
	{
		' 6en', ' 7en', ' 8en', ' 9en', ' aen',
	};

	static const uint32_t gdwEliteNecromancerRewardCodes[] =
	{
		' ben', ' cen', ' den', ' een', ' fen',
	};

	static const uint32_t gdwNormalPaladinRewardCodes[] =
	{
		' 1ap', ' 2ap', ' 3ap', ' 4ap', ' 5ap',
	};

	static const uint32_t gdwExceptionalPaladinRewardCodes[]
	{
		' 6ap', ' 7ap', ' 8ap', ' 9ap', ' aap',
	};

	static const uint32_t gdwElitePaladinRewardCodes[] =
	{
		' bap', ' cap', ' dap', ' eap', ' fap',
	};

	static const uint32_t gdwNormalSorceressRewardCodes[] =
	{
		' 1bo', ' 2bo', ' 3bo', ' 4bo', ' 5bo',
	};

	static const uint32_t gdwExceptionalSorceressRewardCodes[]
	{
		' 6bo', ' 7bo', ' 8bo', ' 9bo', ' abo',
	};

	static const uint32_t gdwEliteSorceressRewardCodes[] =
	{
		' bbo', ' cbo', ' dbo', ' ebo', ' fbo',
	};

	static const uint32_t gdwNormalDruidRewardCodes[] =
	{
		' 1rd', ' 2rd', ' 3rd', ' 4rd', ' 5rd',
	};

	static const uint32_t gdwExceptionalDruidRewardCodes[]
	{
		' 6rd', ' 7rd', ' 8rd', ' 9rd', ' ard',
	};

	static const uint32_t gdwEliteDruidRewardCodes[] =
	{
		' brd', ' crd', ' drd', ' erd', ' frd',
	};

	static const uint32_t gdwNormalAssasinRewardCodes[] =
	{
		' rtk', ' brw', ' fxa', ' sec', ' wlc', ' ltb', ' rks',
	};

	static const uint32_t gdwExceptionalAssasinRewardCodes[]
	{
		' ra9', ' bw9', ' fx9', ' sc9', ' wl9', ' wt9', ' rq9',
	};

	static const uint32_t gdwEliteAssasinRewardCodes[] =
	{
		' ra7', ' bw7', ' fx7', ' sc7', ' wl7', ' wt7', ' rq7',
	};

	static const D2PrisonOfIceRewardStrc pNormalRewards[] =
	{
		{ gdwNormalAmazonRewardCodes, std::size(gdwNormalAmazonRewardCodes) },
		{ gdwNormalSorceressRewardCodes, std::size(gdwNormalSorceressRewardCodes) },
		{ gdwNormalNecromancerRewardCodes, std::size(gdwNormalNecromancerRewardCodes) },
		{ gdwNormalPaladinRewardCodes, std::size(gdwNormalPaladinRewardCodes) },
		{ gdwNormalBarbarianRewardCodes, std::size(gdwNormalBarbarianRewardCodes) },
		{ gdwNormalDruidRewardCodes, std::size(gdwNormalDruidRewardCodes) },
		{ gdwNormalAssasinRewardCodes, std::size(gdwNormalAssasinRewardCodes) },
	};

	static const D2PrisonOfIceRewardStrc pExceptionalRewards[] =
	{
		{ gdwExceptionalAmazonRewardCodes, std::size(gdwExceptionalAmazonRewardCodes) },
		{ gdwExceptionalSorceressRewardCodes, std::size(gdwExceptionalSorceressRewardCodes) },
		{ gdwExceptionalNecromancerRewardCodes, std::size(gdwExceptionalNecromancerRewardCodes) },
		{ gdwExceptionalPaladinRewardCodes, std::size(gdwExceptionalPaladinRewardCodes) },
		{ gdwExceptionalBarbarianRewardCodes, std::size(gdwExceptionalBarbarianRewardCodes) },
		{ gdwExceptionalDruidRewardCodes, std::size(gdwExceptionalDruidRewardCodes) },
		{ gdwExceptionalAssasinRewardCodes, std::size(gdwExceptionalAssasinRewardCodes) },
	};

	static const D2PrisonOfIceRewardStrc pEliteRewards[] =
	{
		{ gdwEliteAmazonRewardCodes, std::size(gdwEliteAmazonRewardCodes) },
		{ gdwEliteSorceressRewardCodes, std::size(gdwEliteSorceressRewardCodes) },
		{ gdwEliteNecromancerRewardCodes, std::size(gdwEliteNecromancerRewardCodes) },
		{ gdwElitePaladinRewardCodes, std::size(gdwElitePaladinRewardCodes) },
		{ gdwEliteBarbarianRewardCodes, std::size(gdwEliteBarbarianRewardCodes) },
		{ gdwEliteDruidRewardCodes, std::size(gdwEliteDruidRewardCodes) },
		{ gdwEliteAssasinRewardCodes, std::size(gdwEliteAssasinRewardCodes) },
	};

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (pQuestArg->nNPCNo == MONSTER_MALAH)
	{
		if (pQuestArg->nMessageIndex == 20116)
		{
			pQuestDataEx->bMalahIntroActivated = 1;
			QUESTS_StateDebug(pQuestData, 2, __FILE__, __LINE__);
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q3_UnitIterate_UpdateQuestStateFlags);
			QUESTS_NPCActivateSpeeches(pQuestArg->pGame, pQuestArg->pPlayer, pQuestArg->pTarget);
			return;
		}

		if (pQuestArg->nMessageIndex == 20127)
		{
			if (!ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' eci') && !pQuestDataEx->nDefrostPotionsInGame)
			{
				if (QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, ' eci', 0, ITEMQUAL_NORMAL, 1))
				{
					++pQuestDataEx->nDefrostPotionsInGame;

					D2GSPacketSrv5D packet5D = {};
					packet5D.nHeader = 0x5D;
					packet5D.nQuestId = QUEST_A5Q3_PRISONOFICE;
					packet5D.nFlags = 1;
					packet5D.field_4 = 0;
					D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pQuestArg->pPlayer, __FILE__, __LINE__), &packet5D, sizeof(packet5D));

					pQuestDataEx->bMalahActivated = 1;
				}
			}
			return;
		}

		if (pQuestArg->nMessageIndex == 20132)
		{
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDGRANTED))
			{
				if ((QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_COMPLETEDBEFORE))
					&& !pQuestDataEx->bRewarded
					&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM4)
					&& !QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_CUSTOM3)
					&& !ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 2rt')
					&& QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 2rt', 0, ITEMQUAL_NORMAL, 1))
				{
					pQuestDataEx->bRewarded = 1;
				}
			}
			else
			{
				if (QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 2rt', 0, 2u, 1))
				{
					QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM4);

					D2GSPacketSrv5D packet5D = {};
					packet5D.nHeader = 0x5D;
					packet5D.nQuestId = QUEST_A5Q3_PRISONOFICE;
					packet5D.nFlags = 2;
					packet5D.field_4 = 0;
					D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pQuestArg->pPlayer, __FILE__, __LINE__), &packet5D, sizeof(packet5D));

					if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM5))
					{
						QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING);
						QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, 0);
					}
					else
					{
						if (pQuestData->bNotIntro && pQuestData->fLastState < 6)
						{
							pQuestData->dwFlags &= 0xFFFFFF00;
							QUESTS_UnitIterate(pQuestData, 6, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);
						}
					}

					if (pQuestDataEx->unk0x84 == 1)
					{
						pQuestDataEx->bDrehyaIcedRemoved = 0;
						SUNIT_IterateUnitsOfType(pQuestData->pGame, 1, 0, ACT5Q3_RemoveDrehyaIced);
						if (!pQuestDataEx->bDrehyaIcedRemoved)
						{
							SUNITINACTIVE_DeleteSingleListNode(pQuestData->pGame, UNIT_MONSTER, MONSTER_DREHYAICED, ACT_V);
						}

						pQuestDataEx->bChangeToSpecialObjectModeOutsideTown = 1;
						pQuestDataEx->unk0x84 = 2;
						ACT5Q3_SpawnDrehyaInTown(pQuestData, pQuestDataEx);
					}

					QUESTS_UpdatePlayerFlags(pQuestArg->pGame, pQuestArg->pPlayer);
				}
			}
			return;
		}
		if (pQuestArg->nMessageIndex == 20131 && pQuestData->bNotIntro)
		{
			if (pQuestData->fState <= 3)
			{
				QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
				sub_6FCB4400(pQuestData);
			}

			if (pQuestData->fLastState < 3)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 3, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);
			}
		}
		return;
	}

	if (pQuestArg->nNPCNo != MONSTER_DREHYA || pQuestArg->nMessageIndex != 20136)
	{
		if (pQuestArg->nMessageIndex == 20131 && pQuestData->bNotIntro)
		{
			if (pQuestData->fState <= 3)
			{
				QUESTS_StateDebug(pQuestData, 4, __FILE__, __LINE__);
				sub_6FCB4400(pQuestData);
			}

			if (pQuestData->fLastState < 3)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 3, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);
			}
		}
		return;
	}

	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDGRANTED))
	{
		return;
	}

	const int32_t nPlayerLevel = STATLIST_GetUnitBaseStat(pQuestArg->pPlayer, STAT_LEVEL, 0);

	const D2PrisonOfIceRewardStrc* pReward = pNormalRewards;
	if (pQuestArg->pGame->nDifficulty == DIFFMODE_NIGHTMARE)
	{
		if (pQuestArg->pGame->nGameType == 3 || nPlayerLevel > 45)
		{
			pReward = pExceptionalRewards;
		}
	}
	else if (pQuestArg->pGame->nDifficulty == DIFFMODE_HELL)
	{
		if (pQuestArg->pGame->nGameType == 3 || nPlayerLevel > 65)
		{
			pReward = pEliteRewards;
		}
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM6))
	{
		return;
	}

	const int32_t nPlayerClass = pQuestArg->pPlayer ? pQuestArg->pPlayer->dwClassId : -1;

	const uint32_t dwItemCode = pReward[nPlayerClass].dwItemCodes[ITEMS_RollLimitedRandomNumber(QUESTS_GetGlobalSeed(pQuestArg->pGame), pReward[nPlayerClass].nItemCount)];

	D2UnitStrc* pRewardItem = QUESTS_CreateItem(pQuestArg->pGame, pQuestArg->pPlayer, dwItemCode, ITEMS_GetItemLevelForNewItem(pQuestArg->pPlayer, 0), ITEMQUAL_RARE, 1);
	if (!pRewardItem)
	{
		return;
	}

	D2GSPacketSrv5D packet5D = {};
	packet5D.nHeader = 0x5D;
	packet5D.nQuestId = QUEST_A5Q3_PRISONOFICE;
	packet5D.nFlags = 16;
	packet5D.field_4 = 0;

	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pRewardItem->dwClassId);
	if (pItemsTxtRecord)
	{
		const int16_t nDropSound = pItemsTxtRecord->wDropSound;
		if (nDropSound > 0)
		{
			packet5D.field_4 = nDropSound;
		}
	}

	D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pQuestArg->pPlayer, __FILE__, __LINE__), &packet5D, sizeof(packet5D));

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM5);

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM4))
	{
		QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING);
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDGRANTED);
	}

	QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM6);
}

//D2Game.0x6FCB4040
int32_t __fastcall ACT5Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, 0) == 1 || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING) == 1)
	{
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return 0;
	}

	if (pQuestData->fState == 2)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_STARTED);
	}
	else if (pQuestData->fState == 3)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_LEAVETOWN);
	}

	return 0;
}

//D2Game.0x6FCB40B0
int32_t __fastcall ACT5Q3_RemoveDrehyaIced(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!pUnit || pUnit->dwClassId != MONSTER_DREHYAICED)
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
		D2GAME_MONSTERAI_Last_6FC61F70(pGame, pUnit, ACT5Q3_SendPacket0x5DToClient);
		return 1;
	}

	D2ModeChangeStrc modeChange = {};
	MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_DEAD, &modeChange);
	D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
	if (pQuestData)
	{
		((D2Act5Quest3Strc*)pQuestData->pQuestDataEx)->bDrehyaIcedRemoved = 1;
	}

	return 1;
}

//D2Game.0x6FCB4150
void __fastcall ACT5Q3_SendPacket0x5DToClient(D2UnitStrc* pUnit)
{
	D2GSPacketSrv5D packet5D = {};

	packet5D.nHeader = 0x5D;
	packet5D.nQuestId = QUEST_A5Q3_PRISONOFICE;
	packet5D.nFlags = 32;
	packet5D.field_4 = 0;

	D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet5D, sizeof(packet5D));
}

//D2Game.0x6FCB4190
void __fastcall ACT5Q3_SpawnDrehyaInTown(D2QuestDataStrc* pQuestData, D2Act5Quest3Strc* pQuestDataEx)
{
	if (pQuestDataEx->bDrehyaInTownInitialized != 1)
	{
		return;
	}

	D2UnitStrc* pDrehyaInTown = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nDrehyaObjectInTownGUID);
	if (!pDrehyaInTown || pQuestDataEx->bDrehyaMonsterInTownSpawned)
	{
		return;
	}

	D2UnitStrc* pDrehya = QUESTS_SpawnCriticalMonster(pQuestData->pGame, CLIENTS_GetUnitX(pDrehyaInTown), CLIENTS_GetUnitY(pDrehyaInTown), UNITS_GetRoom(pDrehyaInTown), 1, MONSTER_DREHYA);
	if (!pDrehya)
	{
		return;
	}

	pQuestDataEx->nDrehyaMonsterInTownGUID = pDrehya->dwUnitId;
	pQuestDataEx->bDrehyaMonsterInTownSpawned = 1;

	if (pQuestDataEx->pDrehyaMapAI && pQuestDataEx->pDrehyaMapAI->pPosition && !pQuestDataEx->bDrehyaMapAIChanged)
	{
		pQuestDataEx->bDrehyaMapAIChanged = 1;
		DRLGPRESET_ChangeMapAI(&pQuestDataEx->pDrehyaMapAI, AIGENERAL_GetMapAiFromUnit(pDrehya));
	}

	D2CoordStrc pCoord = {};
	UNITS_GetCoords(pDrehya, &pCoord);

	const int32_t nX = pCoord.nX;
	const int32_t nY = pCoord.nY;

	D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pDrehya), nX, nY);
	if (pRoom)
	{
		D2UnitStrc* pDrehyaPortal = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_CAINPORTAL, pCoord.nX, pCoord.nY, pQuestData->pGame, pRoom, 1, 1, 0);
		if (pDrehyaPortal)
		{
			pQuestDataEx->bDrehyaPortalInTownSpawned = 1;
			pQuestDataEx->nDrehyaPortalInTownGUID = pDrehyaPortal->dwUnitId;
		}
	}

	if (pQuestData->fState < 5 && pQuestData->bNotIntro)
	{
		if (!pQuestData->fState)
		{
			pQuestData->fState = 1;
		}
		return;
	}

	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, pQuestData->nSeqId);
	if (!pQuest)
	{
		return;
	}

	if (IsBadCodePtr((FARPROC)pQuestData->pfSeqFilter))
	{
		FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
		exit(-1);
	}
	pQuestData->pfSeqFilter(pQuest);
}

//D2Game.0x6FCB4400
//TODO: Find a name
void __fastcall sub_6FCB4400(D2QuestDataStrc* pQuestData)
{
	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bDrehyaMonsterInTownSpawned && pQuestDataEx->unk0x84 != 2)
	{
		D2UnitStrc* pDrehya = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_MONSTER, pQuestDataEx->nDrehyaMonsterInTownGUID);
		if (pDrehya)
		{
			D2MonsterInteractStrc* pMonInteract = nullptr;
			if (pDrehya->dwUnitType == UNIT_MONSTER && pDrehya->pMonsterData)
			{
				pMonInteract = pDrehya->pMonsterData->pMonInteract;
			}

			if (MONSTERAI_HasInteractUnit(pMonInteract))
			{
				D2GAME_MONSTERAI_Last_6FC61F70(pQuestData->pGame, pDrehya, 0);
			}

			D2ModeChangeStrc modeChange = {};
			MONSTERMODE_GetModeChangeInfo(pDrehya, MONMODE_DEAD, &modeChange);
			D2GAME_ModeChange_6FC65220(pQuestData->pGame, &modeChange, 1);

			DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pDrehya), pDrehya->dwUnitType, pQuestDataEx->nDrehyaMonsterInTownGUID);
			UNITS_FreeCollisionPath(pDrehya);
			UNITROOM_RemoveUnitFromRoom(pDrehya);
		}
		else
		{
			SUNITINACTIVE_DeleteSingleListNode(pQuestData->pGame, UNIT_MONSTER, MONSTER_DREHYA, ACT_V);
		}

		pQuestDataEx->bDrehyaMonsterInTownSpawned = 0;
	}

	if (pQuestDataEx->bNihlathakMonsterInTownSpawned)
	{
		D2UnitStrc* pNihlathak = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_MONSTER, pQuestDataEx->NihlathakMonsterInTownGUID);
		if (pNihlathak)
		{
			D2MonsterInteractStrc* pMonInteract = nullptr;
			if (pNihlathak->dwUnitType == UNIT_MONSTER && pNihlathak->pMonsterData)
			{
				pMonInteract = pNihlathak->pMonsterData->pMonInteract;
			}

			if (MONSTERAI_HasInteractUnit(pMonInteract))
			{
				D2GAME_MONSTERAI_Last_6FC61F70(pQuestData->pGame, pNihlathak, 0);
			}

			D2Common_10153(pNihlathak->pDynamicPath);
			D2GAME_EVENTS_Delete_6FC34840(pQuestData->pGame, pNihlathak, EVENTTYPE_AITHINK, 0);
			STATLIST_SetUnitStat(pNihlathak, STAT_HITPOINTS, 0, 0);

			D2ModeChangeStrc modeChange = {};
			MONSTERMODE_GetModeChangeInfo(pNihlathak, MONMODE_DEAD, &modeChange);

			modeChange.nX = 0;
			modeChange.nY = 0;
			D2GAME_ModeChange_6FC65220(pQuestData->pGame, &modeChange, 1);
			UNITROOM_RefreshUnit(pNihlathak);

			pNihlathak->dwFlags |= UNITFLAG_DOUPDATE;

			D2UnitStrc* pNihlathakInTown = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pQuestDataEx->nNihlathakObjectInTownGUID);
			if (pNihlathakInTown)
			{
				EVENT_SetEvent(pQuestData->pGame, pNihlathakInTown, EVENTTYPE_QUESTFN, pQuestData->pGame->dwGameFrame + 1, 0, 0);
			}
		}
		else
		{
			SUNITINACTIVE_DeleteSingleListNode(pQuestData->pGame, UNIT_MONSTER, MONSTER_NIHLATHAK, ACT_V);
		}

		pQuestDataEx->bNihlathakMonsterInTownSpawned = 0;
	}

	if (!pQuestDataEx->unk0x88)
	{
		pQuestDataEx->unk0x88 = 1;
	}
}

//D2Game.0x6FCB45E0
void __fastcall ACT5Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	static const int32_t nIndices[] =
	{
		-1, 0, 1, 2, 3, 4, 5
	};

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestData->pGame->nDifficulty];

	int32_t nNpcId = -1;
	if (pQuestArg->pTarget)
	{
		nNpcId = pQuestArg->pTarget->dwClassId;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	if (nNpcId == MONSTER_DREHYAICED)
	{
		if (pQuestData->fState == 6)
		{
			D2ModeChangeStrc modeChange = {};
			MONSTERMODE_GetModeChangeInfo(pQuestArg->pTarget, MONMODE_DEAD, &modeChange);
			D2GAME_ModeChange_6FC65220(pQuestArg->pGame, &modeChange, 1);
		}
		return;
	}

	if (nNpcId == MONSTER_MALAH)
	{
		if (pQuestData->fState == 4)
		{
			if (!ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' eci') && !pQuestDataEx->nDefrostPotionsInGame)
			{
				QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_MALAH, 3);
			}
			return;
		}

		if ((QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_COMPLETEDBEFORE))
			&& !pQuestDataEx->bRewarded
			&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM4)
			&& !QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_CUSTOM3)
			&& !ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' 2rt'))
		{
			QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, MONSTER_MALAH, 5);
		}
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDPENDING))
	{
		if (nNpcId == MONSTER_DREHYA)
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM5))
			{
				return;
			}
		}
		else if (nNpcId == MONSTER_MALAH)
		{
			if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM4))
			{
				return;
			}
		}

		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 5);
		return;
	}

	if (QUESTS_CheckPlayerGUID(pQuestData, (pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1)) == 1)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, 6);
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED) == 1 || (pQuestData->fState >= 5 && !QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_PRIMARYGOALDONE)) || !pQuestData->bNotIntro)
	{
		return;
	}

	const int32_t nIndex = nIndices[pQuestData->fState];
	if (nIndex != -1 && nIndex < 10)
	{
		QUESTS_InitScrollTextChain(pQuestData, pQuestArg->pTextControl, nNpcId, nIndex);
	}
}

//D2Game.0x6FCB4810
void __fastcall ACT5Q3_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	int32_t nUnitId = -1;
	if (pQuestArg->pPlayer)
	{
		nUnitId = pQuestArg->pPlayer->dwUnitId;
	}

	QUESTS_FastRemovePlayerGUID(&pQuestData->tPlayerGUIDs, nUnitId);

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	QUESTS_FastRemovePlayerGUID(&pQuestDataEx->tPlayerGUIDs, nUnitId);

	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' eci'))
	{
		--pQuestDataEx->nDefrostPotionsInGame;
	}
}

//D2Game.0x6FCB4870
void __fastcall ACT5Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (pQuestArg->nNewLevel == LEVEL_ARREATPLATEAU)
	{
		if (pQuestData->bNotIntro && !pQuestData->fState)
		{
			pQuestData->fState = 1;
			sub_6FCB4400(pQuestData);
		}
	}
	else
	{
		if ((pQuestArg->nNewLevel == LEVEL_CRYSTALIZEDCAVERNLEV1 || pQuestArg->nNewLevel == LEVEL_CELLAROFPITY) && pQuestData->bNotIntro)
		{
			if (pQuestData->fState <= 2)
			{
				QUESTS_StateDebug(pQuestData, 3, __FILE__, __LINE__);
			}

			if (pQuestData->fLastState)
			{
				if (pQuestData->fState > 2)
				{
					sub_6FCB4400(pQuestData);
				}
				else
				{
					SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q3_UnitIterate_UpdateQuestStateFlags);
					sub_6FCB4400(pQuestData);
				}
			}
			else
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);

				SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q3_UnitIterate_UpdateQuestStateFlags);
				sub_6FCB4400(pQuestData);
			}
		}
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
			if (!pQuestData->fLastState)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 1, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);
			}
			SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT5Q3_UnitIterate_UpdateQuestStateFlags);
		}
	}

	if (pQuestArg->nNewLevel < LEVEL_NIHLATHAKSTEMPLE || pQuestArg->nNewLevel > LEVEL_HALLSOFVAUGHT || !pQuestData->bNotIntro || pQuestData->fState >= 5)
	{
		return;
	}

	SUNIT_IterateUnitsOfType(pQuestArg->pGame, 0, pQuestArg->pTarget, ACT5Q3_UnitIterate_SetCompletionFlag);
	pQuestData->fState = 6;

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->unk0x84 = 2;
	if (!pQuestDataEx->unk0x88)
	{
		pQuestDataEx->unk0x88 = 1;
	}
}

//D2Game.0x6FCB4A10
int32_t __fastcall ACT5Q3_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDGRANTED) != 1
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING) != 1
		&& QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_PRIMARYGOALDONE) != 1)
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_COMPLETEDNOW);
		QUESTS_SendLogUpdateEx(pUnit, QUEST_A5Q3_PRISONOFICE, 0);
	}

	return 0;
}

//D2Game.0x6FCB4A80
void __fastcall ACT5Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	if (ITEMS_FindQuestItem(pQuestData->pGame, pQuestArg->pPlayer, ' eci'))
	{
		pQuestDataEx->nDefrostPotionsInGame = 1;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_COMPLETEDBEFORE))
	{
		pQuestDataEx->unk0x84 = 2;
		pQuestDataEx->unk0x88 = 1;
		return;
	}

	if (!pQuestData->bNotIntro)
	{
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_LEAVETOWN))
	{
		pQuestData->fState = 3;
	}
	else
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_STARTED))
		{
			if (pQuestDataEx->nDefrostPotionsInGame)
			{
				pQuestData->fState = 3;
				pQuestData->fLastState = 4;
			}
			return;
		}
		pQuestData->fState = 2;
	}

	pQuestData->fLastState = 1;

	if (pQuestDataEx->nDefrostPotionsInGame)
	{
		pQuestData->fState = 3;
		pQuestData->fLastState = 4;
	}
}

//D2Game.0x6FCB4B50
void __fastcall ACT5Q3_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg)
{
	if (ITEMS_FindQuestItem(pQuestArg->pGame, pQuestArg->pPlayer, ' eci'))
	{
		++((D2Act5Quest3Strc*)pQuestData->pQuestDataEx)->nDefrostPotionsInGame;
	}

	if (pQuestData->bNotIntro && pQuestData->fState < 5)
	{
		return;
	}

	D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData->pGame, QUEST_A5Q4_NIHLATHAK);
	if (!pQuest)
	{
		return;
	}

	if (!pQuest->bNotIntro)
	{
		return;
	}

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pQuestArg->pPlayer)->pQuestData[pQuestArg->pGame->nDifficulty];
	if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING)
		&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM2)
		&& !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDGRANTED))
	{
		QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q4, QFLAG_COMPLETEDNOW);
	}
}

//D2Game.0x6FCB4BF0
bool __fastcall ACT5Q3_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus)
{
	*pStatus = 0;
	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDGRANTED))
	{
		return true;
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING) || QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A5Q3, QFLAG_PRIMARYGOALDONE))
	{
		if (QUESTRECORD_GetQuestState(pFlags, pQuest->nQuestFilter, QFLAG_CUSTOM4) && !QUESTRECORD_GetQuestState(pFlags, pQuest->nQuestFilter, QFLAG_CUSTOM5))
		{
			*pStatus = 6;
		}
		else if (!QUESTRECORD_GetQuestState(pFlags, pQuest->nQuestFilter, QFLAG_CUSTOM4))
		{
			*pStatus = 5;
		}
		else
		{
			*pStatus = 13;
		}
		return true;
	}

	if (!pQuest->bNotIntro)
	{
		return true;
	}

	if (ITEMS_FindQuestItem(pQuest->pGame, pPlayer, ' eci'))
	{
		*pStatus = 4;
		return true;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuest->pQuestDataEx;
	const int16_t nPartyId = SUNIT_GetPartyId(pPlayer);
	if (nPartyId != -1)
	{
		pQuestDataEx->bDefrostPotionAcquired = 0;
		PARTY_IteratePartyMembers(pQuest->pGame, nPartyId, ACT5Q3_UnitIterate_CheckForDefrostPotion, 0);

		if (pQuestDataEx->bDefrostPotionAcquired)
		{
			*pStatus = 4;
			return true;
		}
	}

	if (QUESTRECORD_GetQuestState(pFlags, QUESTSTATEFLAG_A5Q3, QFLAG_COMPLETEDNOW))
	{
		*pStatus = 12;
	}
	else if (pQuest->fState < 5)
	{
		*pStatus = pQuest->fLastState;
	}

	return true;
}

//D2Game.0x6FCB4D30
void __fastcall OBJECTS_InitFunction66_DrehyaStartInTown(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData33 = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q3_PRISONOFICE);
	if (pQuestData33)
	{
		D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData33->pQuestDataEx;
		pQuestDataEx->nDrehyaObjectInTownGUID = pOp->pObject ? pOp->pObject->dwUnitId : -1;
		pQuestDataEx->bDrehyaInTownInitialized = 1;
		pQuestDataEx->pDrehyaObjectInTownCoords.nX = pOp->nX;
		pQuestDataEx->pDrehyaObjectInTownCoords.nY = pOp->nY;

		if (pQuestDataEx->unk0x84 == 2)
		{
			if (pQuestData33->fState < 5 && pQuestData33->bNotIntro)
			{
				if (!pQuestData33->fState)
				{
					pQuestData33->fState = 1;
				}
			}
			else
			{
				D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pQuestData33->pGame, pQuestData33->nSeqId);
				if (pQuest)
				{
					if (IsBadCodePtr((FARPROC)pQuest->pfSeqFilter))
					{
						FOG_DisplayAssert("pQuestInfo->pSequence", "..\\D2Game/Quests/a5q3.cpp", 331);
						exit(-1);
					}
					pQuest->pfSeqFilter(pQuest);
				}
			}

			if (!pQuestDataEx->bDrehyaMonsterInTownSpawned)
			{
				D2UnitStrc* pDrehya = QUESTS_SpawnCriticalMonster(pOp->pGame, pOp->nX, pOp->nY, pOp->pRoom, 1, MONSTER_DREHYA);
				if (pDrehya)
				{
					pQuestDataEx->nDrehyaMonsterInTownGUID = pDrehya->dwUnitId;
					pQuestDataEx->bDrehyaMonsterInTownSpawned = 1;

					if (pQuestDataEx->pDrehyaMapAI && pQuestDataEx->pDrehyaMapAI->pPosition && !pQuestDataEx->bDrehyaMapAIChanged)
					{
						DRLGPRESET_ChangeMapAI(&pQuestDataEx->pDrehyaMapAI, AIGENERAL_GetMapAiFromUnit(pDrehya));
						pQuestDataEx->bDrehyaMapAIChanged = 1;
					}
				}
			}
		}
	}

	D2QuestDataStrc* pQuestData34 = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q4_NIHLATHAK);
	if (pQuestData34 && ((D2Act5Quest4Strc*)pQuestData34->pQuestDataEx)->bNeedsToOpenPortal)
	{
		EVENT_SetEvent(pOp->pGame, pOp->pObject, EVENTTYPE_QUESTFN, pOp->pGame->dwGameFrame + 12, 0, 0);
	}
}

//D2Game.0x6FCB4EA0
void __fastcall ACT5Q3_SpawnFrozenDrehya(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return;
	}

	if (!pQuestData->bNotIntro)
	{
		return;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bFrozenAnyaObjectSpawned)
	{
		return;
	}

	D2UnitStrc* pFrozenDrehya = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_FROZEN_ANYA, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), pGame, UNITS_GetRoom(pUnit), 1, 0, 0);
	if (pFrozenDrehya)
	{
		pFrozenDrehya->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
		pQuestDataEx->bFrozenAnyaObjectSpawned = 1;
		pQuestDataEx->nFrozenDrehyaObjectGUID = pFrozenDrehya->dwUnitId;
	}
	else
	{
		EVENT_SetEvent(pQuestData->pGame, pUnit, EVENTTYPE_QUESTFN, pQuestData->pGame->dwGameFrame + 25, 0, 0);
	}
}

//D2Game.0x6FCB4FB0
void __fastcall OBJECTS_InitFunction67_DrehyaStartOutsideTown(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData || !pQuestData->bNotIntro)
	{
		return;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}
	pQuestDataEx->nDrehyaObjectOutsideTownGUID = nUnitId;

	if (!pQuestDataEx->bFrozenAnyaObjectSpawned)
	{
		EVENT_SetEvent(pQuestData->pGame, pOp->pObject, EVENTTYPE_QUESTFN, pQuestData->pGame->dwGameFrame + 25, 0, 0);
	}
}

//D2Game.0x6FCB5010
void __fastcall OBJECTS_InitFunction68_NihlathakStartInTown(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	if (pOp->pObject)
	{
		pQuestDataEx->nNihlathakObjectInTownGUID = pOp->pObject->dwUnitId;
	}
	else
	{
		pQuestDataEx->nNihlathakObjectInTownGUID = -1;
	}

	if (pQuestDataEx->unk0x88 || pQuestDataEx->bNihlathakMonsterInTownSpawned)
	{
		return;
	}

	D2UnitStrc* pNihlathak = QUESTS_SpawnCriticalMonster(pOp->pGame, pOp->nX, pOp->nY, pOp->pRoom, 1, MONSTER_NIHLATHAK);
	if (!pNihlathak)
	{
		return;
	}

	pQuestDataEx->NihlathakMonsterInTownGUID = pNihlathak->dwUnitId;
	pQuestDataEx->bNihlathakMonsterInTownSpawned = 1;

	if (pQuestDataEx->pNilathakMapAI && pQuestDataEx->pNilathakMapAI->pPosition && !pQuestDataEx->bNilathakMapAIChanged)
	{
		DRLGPRESET_ChangeMapAI(&pQuestDataEx->pNilathakMapAI, AIGENERAL_GetMapAiFromUnit(pNihlathak));
		pQuestDataEx->bNilathakMapAIChanged = 1;
	}
}

//D2Game.0x6FCB50C0
void __fastcall OBJECTS_InitFunction69_NihlathakStartOutsideTown(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->unk0x88 != 1 || pQuestDataEx->bNilathakOutsideTownSpawned)
	{
		return;
	}

	const int32_t nMonsterId = DATATBLS_MapSuperUniqueId(2, sgptDataTables->nSuperUniqueIds[SUPERUNIQUE_NIHLATHAK_BOSS]);
	D2UnitStrc* pNihlathak = D2GAME_SpawnPresetMonster_6FC66560(pOp->pGame, pOp->pRoom, nMonsterId, pOp->nX, pOp->nY, 1);
	if (pNihlathak)
	{
		pQuestDataEx->bNilathakOutsideTownSpawned = 1;
		pQuestDataEx->nNihlathakOutsideTownGUID = pNihlathak->dwUnitId;
	}
}

//D2Game.0x6FCB5130
void __fastcall ACT5Q3_UpdateDrehyaPortalMode(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit)
{
	if (!pUnit)
	{
		return;
	}

	if (pUnit->dwAnimMode == OBJMODE_OPERATING)
	{
		UNITS_ChangeAnimMode(pUnit, OBJMODE_OPENED);
	}
	else if (pUnit->dwAnimMode == OBJMODE_OPENED)
	{
		D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
		D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
		if (DUNGEON_GetLevelIdFromRoom(pRoom) == LEVEL_HARROGATH)
		{
			++pQuestDataEx->nObjectUpdateInvocations;

			if (pQuestDataEx->nObjectUpdateInvocations > 5)
			{
				pQuestDataEx->bChangeToSpecialObjectModeInTown = 1;
			}

			if (pQuestDataEx->bChangeToSpecialObjectModeInTown)
			{
				UNITS_ChangeAnimMode(pUnit, OBJMODE_SPECIAL1);
			}
		}
		else
		{
			if (pQuestDataEx->bChangeToSpecialObjectModeOutsideTown)
			{
				UNITS_ChangeAnimMode(pUnit, OBJMODE_SPECIAL1);
			}
		}
	}
	else if (pUnit->dwAnimMode == OBJMODE_SPECIAL1)
	{
		UNITS_ChangeAnimMode(pUnit, OBJMODE_SPECIAL2);
	}

	EVENT_SetEvent(pQuestData->pGame, pUnit, EVENTTYPE_QUESTFN, pQuestData->pGame->dwGameFrame + 25, 0, 0);
}

//D2Game.0x6FCB51C0
int32_t __fastcall ACT5Q3_SpawnDrehyaPortalOutsideTown(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bDrehyaPortalOutsideTownSpawned)
	{
		return 1;
	}

	if (!pQuestDataEx->unk0x84)
	{
		return 0;
	}

	const int32_t nX = pQuestDataEx->pDrehyaPortalCoords.nX;
	const int32_t nY = pQuestDataEx->pDrehyaPortalCoords.nY;

	D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
	if (!pRoom)
	{
		pQuestDataEx->pDrehyaPortalCoords.nX += 3;
		pQuestDataEx->pDrehyaPortalCoords.nY += 3;
		return 0;
	}

	D2UnitStrc* pDrehyaPortal = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_CAINPORTAL, pQuestDataEx->pDrehyaPortalCoords.nX, pQuestDataEx->pDrehyaPortalCoords.nY, pGame, pRoom, 1, 1, 0);
	if (!pDrehyaPortal)
	{
		return 0;
	}

	pQuestDataEx->bDrehyaPortalOutsideTownSpawned = 1;
	pQuestDataEx->nDrehyaPortalOutsideTownGUID = pDrehyaPortal->dwUnitId;

	return 1;
}

//D2Game.0x6FCB5320
int32_t __fastcall ACT5Q3_GetDrehyaPortalCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pCoord)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	if (!pQuestDataEx->unk0x84)
	{
		return 0;
	}

	if (pQuestDataEx->bDrehyaPortalOutsideTownSpawned)
	{
		pCoord->nX = pQuestDataEx->pDrehyaPortalCoords.nX;
		pCoord->nY = pQuestDataEx->pDrehyaPortalCoords.nY;
	}
	else
	{
		UNITS_GetCoords(pUnit, pCoord);
	}

	return 1;
}

//D2Game.0x6FCB5390
int32_t __fastcall ACT5Q3_InitializeDrehyaPortalCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return 0;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	UNITS_GetCoords(pUnit, &pQuestDataEx->pDrehyaPortalCoords);
	pQuestDataEx->bDrehyaPortalCoordsInitialized = 1;

	return 1;
}

//D2Game.0x6FCB53D0
//TODO: Find a name
void __fastcall sub_6FCB53D0(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->bChangeToSpecialObjectModeOutsideTown = 1;
	pQuestDataEx->unk0x84 = 2;
	ACT5Q3_SpawnDrehyaInTown(pQuestData, pQuestDataEx);
}

//D2Game.0x6FCB5400
//TODO: Find a name
int32_t __fastcall sub_6FCB5400(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);

	if (pQuestData)
	{
		return ((D2Act5Quest3Strc*)pQuestData->pQuestDataEx)->unk0x84 == 0;
	}

	return 0;
}

//D2Game.0x6FCB5430
//TODO: Find a name
void __fastcall sub_6FCB5430(D2GameStrc* pGame)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);

	if (pQuestData && pQuestData->bNotIntro && pQuestData->fLastState < 2)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 2, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCB5470
void __fastcall OBJECTS_InitFunction74_FrozenAnya(D2ObjInitFnStrc* pOp)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return;
	}

	int32_t nUnitId = -1;
	if (pOp->pObject)
	{
		nUnitId = pOp->pObject->dwUnitId;
	}

	((D2Act5Quest3Strc*)pQuestData->pQuestDataEx)->nFrozenAnyaObjectGUID = nUnitId;

	if (pQuestData->bNotIntro && pQuestData->fLastState < 2)
	{
		pQuestData->dwFlags &= 0xFFFFFF00;
		QUESTS_UnitIterate(pQuestData, 2, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);
	}
}

//D2Game.0x6FCB54D0
int32_t __fastcall OBJECTS_OperateFunction67_FrozenAnya(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
	if (!pOp || !pOp->pObject)
	{
		return 0;
	}

	D2UnitStrc* pObject = pOp->pObject;

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A5Q3_PRISONOFICE);

	if (!pQuestData || !pQuestData->pQuestDataEx)
	{
		return 1;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;

	if (ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' eci'))
	{
		D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];

		if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDGRANTED)
			&& !QUESTRECORD_GetQuestState(pQuestFlags, pQuestData->nQuestFilter, QFLAG_REWARDPENDING)
			&& ITEMS_FindQuestItem(pOp->pGame, pOp->pPlayer, ' eci'))
		{
			--pQuestDataEx->nDefrostPotionsInGame;
			QUESTS_DeleteItem(pOp->pGame, pOp->pPlayer, ' eci');
			QUESTS_StateDebug(pQuestData, 5, "..\\D2Game/Quests/a5q3.cpp", 1999);

			pQuestDataEx->unk0x84 = 1;
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_PRIMARYGOALDONE);
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_REWARDPENDING);

			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 5, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);
			QUESTS_TriggerFX(pQuestData->pGame, 16);

			SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pPlayer, ACT5Q3_UnitIterate_SetPrimaryGoalDoneForPartyMembers);
			SUNIT_IterateUnitsOfType(pOp->pGame, 0, pOp->pPlayer, ACT5Q3_UnitIterate_SetCompletionFlag);

			if (!pQuestDataEx->bTimerActive)
			{
				pQuestDataEx->bTimerActive = 1;
				pQuestDataEx->pDrehyaObjectOutsideTownCoords.nX = CLIENTS_GetUnitX(pObject);
				pQuestDataEx->pDrehyaObjectOutsideTownCoords.nY = CLIENTS_GetUnitY(pObject);

				QUESTS_CreateTimer(pQuestData, ACT5Q3_SpawnDrehyaIcedMonsterOutsideTown, 1);
				DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pObject), 0);
			}
		}
	}
	else
	{
		QUESTS_SendScrollMessage(pOp->pPlayer, pObject, 20131);

		if (pQuestData->fLastState == 1)
		{
			pQuestData->dwFlags &= 0xFFFFFF00;
			QUESTS_UnitIterate(pQuestData, 3, 0, ACT5Q3_UnitIterate_StatusCyclerEx, 1);
		}
	}

	return 1;
}

//D2Game.0x6FCB56D0
int32_t __fastcall ACT5Q3_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData)
{
	if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q3, QFLAG_PRIMARYGOALDONE))
	{
		return 0;
	}

	const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
	if (nPartyId != -1)
	{
		PARTY_IteratePartyMembers(pGame, nPartyId, ACT5Q3_UnitIterate_SetPrimaryGoalDone, 0);
	}

	return 0;
}

//D2Game.0x6FCB5720
bool __fastcall ACT5Q3_SpawnDrehyaIcedMonsterOutsideTown(D2GameStrc* pGame, D2QuestDataStrc* pQuestData)
{
	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->bDrehyaIcedMonsterSpawned)
	{
		return false;
	}

	D2UnitStrc* pFrozenDrehya = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nFrozenAnyaObjectGUID);

	if (!pQuestDataEx->nTimerInvocations)
	{
		if (pFrozenDrehya)
		{
			UNITS_ChangeAnimMode(pFrozenDrehya, OBJMODE_OPENED);
			UNITS_FreeCollisionPath(pFrozenDrehya);
		}

		pQuestDataEx->nTimerInvocations = 1;
		return false;
	}

	if (pQuestDataEx->nTimerInvocations != 1)
	{
		return false;
	}

	pQuestDataEx->nTimerInvocations = 2;

	if (!pFrozenDrehya)
	{
		return false;
	}

	D2UnitStrc* pDrehya = QUESTS_SpawnCriticalMonster(pGame, pQuestDataEx->pDrehyaObjectOutsideTownCoords.nX, pQuestDataEx->pDrehyaObjectOutsideTownCoords.nY, UNITS_GetRoom(pFrozenDrehya), 1, MONSTER_DREHYAICED);
	if (!pDrehya)
	{
		return false;
	}

	DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pFrozenDrehya), pFrozenDrehya->dwUnitType, pFrozenDrehya->dwUnitId);
	UNITROOM_RemoveUnitFromRoom(pFrozenDrehya);

	if (pQuestDataEx->pDrehyaMapAI && pQuestDataEx->pDrehyaMapAI->pPosition && !pQuestDataEx->bDrehyaMapAIChanged)
	{
		DRLGPRESET_ChangeMapAI(&pQuestDataEx->pDrehyaMapAI, AIGENERAL_GetMapAiFromUnit(pDrehya));
		pQuestDataEx->bDrehyaMapAIChanged = 1;
	}

	pQuestDataEx->bDrehyaIcedMonsterSpawned = 1;
	pQuestDataEx->nDrehyaIcedMonsterGUID = pDrehya->dwUnitId;

	if (pQuestData->fState < 5 && pQuestData->bNotIntro)
	{
		if (!pQuestData->fState)
		{
			pQuestData->fState = 1;
			pQuestDataEx->bTimerActive = 0;
			return true;
		}
	}
	else
	{
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
	}

	pQuestDataEx->bTimerActive = 0;
	return true;
}

//D2Game.0x6FCB5890
void __fastcall ACT5Q3_ChangeDrehyaMapAI(D2GameStrc* pGame, D2UnitStrc* pUnit, D2MapAIStrc* pMapAi)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData)
	{
		return;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	if (!pMapAi)
	{
		return;
	}

	pQuestDataEx->pDrehyaMapAI = DRLGPRESET_CreateCopyOfMapAI(pQuestData->pGame->pMemoryPool, pMapAi);
	if (!pQuestDataEx->bDrehyaMonsterInTownSpawned)
	{
		return;
	}

	D2UnitStrc* pDrehya = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_MONSTER, pQuestDataEx->nDrehyaMonsterInTownGUID);
	if (pDrehya && pQuestDataEx->pDrehyaMapAI && pQuestDataEx->pDrehyaMapAI->pPosition && !pQuestDataEx->bDrehyaMapAIChanged)
	{
		DRLGPRESET_ChangeMapAI(&pQuestDataEx->pDrehyaMapAI, AIGENERAL_GetMapAiFromUnit(pDrehya));
		pQuestDataEx->bDrehyaMapAIChanged = 1;
	}
}

//D2Game.0x6FCB5920
void __fastcall ACT5Q3_ChangeNihlathakMapAI(D2GameStrc* pGame, D2UnitStrc* pUnit, D2MapAIStrc* pMapAi)
{
	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
	if (!pQuestData || !pMapAi)
	{
		return;
	}

	D2Act5Quest3Strc* pQuestDataEx = (D2Act5Quest3Strc*)pQuestData->pQuestDataEx;
	pQuestDataEx->pNilathakMapAI = DRLGPRESET_CreateCopyOfMapAI(pQuestData->pGame->pMemoryPool, pMapAi);
	if (!pQuestDataEx->bNihlathakMonsterInTownSpawned)
	{
		return;
	}

	D2UnitStrc* pNihlathak = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_MONSTER, pQuestDataEx->NihlathakMonsterInTownGUID);
	if (pNihlathak && pQuestDataEx->pNilathakMapAI && pQuestDataEx->pNilathakMapAI->pPosition && !pQuestDataEx->bNilathakMapAIChanged)
	{
		DRLGPRESET_ChangeMapAI(&pQuestDataEx->pNilathakMapAI, AIGENERAL_GetMapAiFromUnit(pNihlathak));
		pQuestDataEx->bNilathakMapAIChanged = 1;
	}
}
