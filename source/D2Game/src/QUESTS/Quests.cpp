#include "QUESTS/Quests.h"

#include <algorithm>
#include <D2BitManip.h>

#include "D2Constants.h"
#include "D2PacketDef.h"

#include <Units/Units.h>

#include <DataTbls/MonsterIds.h>
#include <Units/UnitRoom.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2QuestRecord.h>
#include <D2Dungeon.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ItemsTbls.h>
#include <D2QuestRecord.h>
#include <DataTbls/ObjectsTbls.h>
#include <D2DataTbls.h>
#include <DataTbls/MonsterTbls.h>
#include <D2Monsters.h>
#include <DataTbls/LevelsIds.h>
#include <D2Items.h>
#include <D2StatList.h>
#include <D2Text.h>
#include <D2Environment.h>
#include <D2Collision.h>


#include "AI/AiGeneral.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/PlrIntro.h"
#include "PLAYER/PlrMsg.h"
#include "QUESTS/ACT1/A1Intro.h"
#include "QUESTS/ACT1/A1Q0.h"
#include "QUESTS/ACT1/A1Q1.h"
#include "QUESTS/ACT1/A1Q2.h"
#include "QUESTS/ACT1/A1Q3.h"
#include "QUESTS/ACT1/A1Q4.h"
#include "QUESTS/ACT1/A1Q5.h"
#include "QUESTS/ACT1/A1Q6.h"
#include "QUESTS/ACT1/A1Q7.h"
#include "QUESTS/ACT2/A2Intro.h"
#include "QUESTS/ACT2/A2Q0.h"
#include "QUESTS/ACT2/A2Q1.h"
#include "QUESTS/ACT2/A2Q2.h"
#include "QUESTS/ACT2/A2Q3.h"
#include "QUESTS/ACT2/A2Q4.h"
#include "QUESTS/ACT2/A2Q5.h"
#include "QUESTS/ACT2/A2Q6.h"
#include "QUESTS/ACT2/A2Q7.h"
#include "QUESTS/ACT2/A2Q8.h"
#include "QUESTS/ACT3/A3Intro.h"
#include "QUESTS/ACT3/A3Q0.h"
#include "QUESTS/ACT3/A3Q1.h"
#include "QUESTS/ACT3/A3Q2.h"
#include "QUESTS/ACT3/A3Q3.h"
#include "QUESTS/ACT3/A3Q4.h"
#include "QUESTS/ACT3/A3Q5.h"
#include "QUESTS/ACT3/A3Q6.h"
#include "QUESTS/ACT3/A3Q7.h"
#include "QUESTS/ACT4/A4Q0.h"
#include "QUESTS/ACT4/A4Q1.h"
#include "QUESTS/ACT4/A4Q2.h"
#include "QUESTS/ACT4/A4Q3.h"
#include "QUESTS/ACT4/A4Q4.h"
#include "QUESTS/ACT5/A5Intro.h"
#include "QUESTS/ACT5/A5Q1.h"
#include "QUESTS/ACT5/A5Q2.h"
#include "QUESTS/ACT5/A5Q3.h"
#include "QUESTS/ACT5/A5Q4.h"
#include "QUESTS/ACT5/A5Q5.h"
#include "QUESTS/ACT5/A5Q6.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"


uint8_t byte_6FD31270;


#define dword_6FD4DC34 1

constexpr D2QuestInitTableStrc gpQuestInitTable[] =
{
	{ ACT1Q0_InitQuestData, ACT_I,{ 0,0,0 }, 100, 1,{ 0,0,0 }, QUEST_A1Q0_WARRIVGOSSIP, QUESTSTATEFLAG_A1Q0 },
	{ ACT1Q1_InitQuestData, ACT_I,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A1Q1_DENOFEVIL, QUESTSTATEFLAG_A1Q1 },
	{ ACT1Q2_InitQuestData, ACT_I,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A1Q2_BLOODRAVEN, QUESTSTATEFLAG_A1Q2 },
	{ ACT1Q3_InitQuestData, ACT_I,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A1Q3_MALUS, QUESTSTATEFLAG_A1Q3 },
	{ ACT1Q4_InitQuestData, ACT_I,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A1Q4_CAIN, QUESTSTATEFLAG_A1Q4 },
	{ ACT1Q5_InitQuestData, ACT_I,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A1Q5_COUNTESS, QUESTSTATEFLAG_A1Q5 },
	{ ACT1Q6_InitQuestData, ACT_I,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A1Q6_ANDARIEL, QUESTSTATEFLAG_A1Q6 },

	{ ACT2Q0_InitQuestData, ACT_II,{ 0,0,0 }, 100, 1,{ 0,0,0 }, QUEST_A2Q0_JERHYNGOSSIP, QUESTSTATEFLAG_A2Q0 },
	{ ACT2Q1_InitQuestData, ACT_II,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A2Q1_RADAMENT, QUESTSTATEFLAG_A2Q1 },
	{ ACT2Q2_InitQuestData, ACT_II,{ 0,0,0 }, 100, 1,{ 0,0,0 }, QUEST_A2Q2_HORADRICSTAFF, QUESTSTATEFLAG_A2Q2 },
	{ ACT2Q3_InitQuestData, ACT_II,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A2Q3_TAINTEDSUN, QUESTSTATEFLAG_A2Q3 },
	{ ACT2Q4_InitQuestData, ACT_II,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A2Q4_HORAZONTOME, QUESTSTATEFLAG_A2Q4 },
	{ ACT2Q5_InitQuestData, ACT_II,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A2Q5_SUMMONER, QUESTSTATEFLAG_A2Q5 },
	{ ACT2Q6_InitQuestData, ACT_II,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A2Q6_DURIEL, QUESTSTATEFLAG_A2Q6 },

	{ ACT2Q7_InitQuestData, ACT_II,{ 0,0,0 }, 100, 1,{ 0,0,0 }, QUEST_A2Q7_UNUSED_GUARDGOSSIP, QUESTSTATEFLAG_A2Q7 },
	{ ACT2Q8_InitQuestData, ACT_II,{ 0,0,0 }, 100, 1,{ 0,0,0 }, QUEST_A2Q8_UNUSED_GUARDGOSSIP, QUESTSTATEFLAG_A2Q8 },

	{ ACT3Q0_InitQuestData, ACT_III,{ 0,0,0 }, 100, 1,{ 0,0,0 }, QUEST_A3Q0_HRATLIGOSSIP, QUESTSTATEFLAG_A3Q0 },
	{ ACT3Q1_InitQuestData, ACT_III,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A3Q1_LAMESENTOME, QUESTSTATEFLAG_A3Q1 },
	{ ACT3Q2_InitQuestData, ACT_III,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A3Q2_KHALIMFLAIL, QUESTSTATEFLAG_A3Q2 },
	{ ACT3Q3_InitQuestData, ACT_III,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A3Q3_GIDBINN, QUESTSTATEFLAG_A3Q3 },
	{ ACT3Q4_InitQuestData, ACT_III,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A3Q4_GOLDENBIRD, QUESTSTATEFLAG_A3Q4 },
	{ ACT3Q5_InitQuestData, ACT_III,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A3Q5_TRAVINCAL, QUESTSTATEFLAG_A3Q5 },
	{ ACT3Q6_InitQuestData, ACT_III,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A3Q6_MEPHISTO, QUESTSTATEFLAG_A3Q6 },

	{ ACT3Q7_InitQuestData, ACT_III,{ 0,0,0 }, 100, 0,{ 0,0,0 }, QUEST_A3Q7_DARKWANDERER, QUESTSTATEFLAG_A3Q7 },

	{ ACT4Q0_InitQuestData, ACT_IV,{ 0,0,0 }, 100, 1,{ 0,0,0 }, QUEST_A4Q0_TYRAELGOSSIP, QUESTSTATEFLAG_A4Q0 },
	{ ACT4Q1_InitQuestData, ACT_IV,{ 0,0,0 }, 0x0, 0,{ 0,0,0 }, QUEST_A4Q1_IZUAL, QUESTSTATEFLAG_A4Q1 },
	{ ACT4Q2_InitQuestData, ACT_IV,{ 0,0,0 }, 0x0, 0,{ 0,0,0 }, QUEST_A4Q2_DIABLO, QUESTSTATEFLAG_A4Q2 },
	{ ACT4Q3_InitQuestData, ACT_IV,{ 0,0,0 }, 0x0, 0,{ 0,0,0 }, QUEST_A4Q3_HELLFORGE, QUESTSTATEFLAG_A4Q3 },

	{ ACT1Q7_InitQuestData, ACT_I,{ 0,0,0 }, 100, 1,{ 0,0,0 }, QUEST_A1Q1EX_NAVI, QUESTSTATEFLAG_A1Q7 },

	{ ACT4Q4_InitQuestData, ACT_IV,{ 0,0,0 }, 100, 1,{ 0,0,0 }, QUEST_A4Q4_MALACHAI, QUESTSTATEFLAG_A4Q4 },

	{ ACT5Q1_InitQuestData, ACT_V,{ 0,0,0 }, 0x0, 0,{ 0,0,0 }, QUEST_A5Q1_SHENK, QUESTSTATEFLAG_A5Q1 },
	{ ACT5Q2_InitQuestData, ACT_V,{ 0,0,0 }, 0x0, 0,{ 0,0,0 }, QUEST_A5Q2_RESCUESOLDIERS, QUESTSTATEFLAG_A5Q2 },
	{ ACT5Q3_InitQuestData, ACT_V,{ 0,0,0 }, 0x0, 0,{ 0,0,0 }, QUEST_A5Q3_PRISONOFICE, QUESTSTATEFLAG_A5Q3 },
	{ ACT5Q4_InitQuestData, ACT_V,{ 0,0,0 }, 0x0, 0,{ 0,0,0 }, QUEST_A5Q4_NIHLATHAK, QUESTSTATEFLAG_A5Q4 },
	{ ACT5Q5_InitQuestData, ACT_V,{ 0,0,0 }, 0x0, 0,{ 0,0,0 }, QUEST_A5Q5_ANCIENTS, QUESTSTATEFLAG_A5Q5 },
	{ ACT5Q6_InitQuestData, ACT_V,{ 0,0,0 }, 0x0, 0,{ 0,0,0 }, QUEST_A5Q6_BAAL, QUESTSTATEFLAG_A5Q6 },
};

constexpr D2QuestIntroTableStrc gpQuestIntroTable[] =
{
	{ ACT1Intro_InitQuestData, ACT_I, { 0,0,0 } },
	{ ACT2Intro_InitQuestData, ACT_II, { 0,0,0 } },
	{ ACT3Intro_InitQuestData, ACT_III, { 0,0,0 } },
	{ ACT5Intro_InitQuestData, ACT_V, { 0,0,0 } }
};

constexpr int32_t gpAct1_NPCIds[] =
{
	MONSTER_AKARA,
	MONSTER_CHARSI,
	MONSTER_KASHYA,
	MONSTER_GHEED,
	MONSTER_WARRIV1,
	MONSTER_CAIN5
};

constexpr int32_t gpAct2_NPCIds[] =
{
	MONSTER_WARRIV2,
	MONSTER_ATMA,
	MONSTER_DROGNAN,
	MONSTER_FARA,
	MONSTER_LYSANDER,
	MONSTER_GEGLASH,
	MONSTER_MESHIF1,
	MONSTER_JERHYN,
	MONSTER_GREIZ,
	MONSTER_ELZIX,
	MONSTER_CAIN2
};

constexpr int32_t gpAct3_NPCIds[] =
{
	MONSTER_CAIN3,
	MONSTER_ASHEARA,
	MONSTER_HRATLI,
	MONSTER_ALKOR,
	MONSTER_ORMUS,
	MONSTER_MESHIF2,
	MONSTER_NATALYA
};

constexpr int32_t gpAct5_NPCIds[] =
{
	MONSTER_CAIN6,
	MONSTER_TYRAEL3,
	MONSTER_LARZUK,
	MONSTER_DREHYA,
	MONSTER_MALAH,
	MONSTER_NIHLATHAK,
	MONSTER_QUAL_KEHK
};



//D2Game.0x6FC93B60
void __fastcall QUESTS_FreeChainRecord(D2GameStrc* pGame, D2QuestChainStrc* pRecord)
{
	D2QuestChainStrc* pNext = nullptr;
	while (pRecord)
	{
		pNext = pRecord->pNext;
		D2_FREE_POOL(pGame ? pGame->pMemoryPool : nullptr, pRecord);
		pRecord = pNext;
	}
}

//D2Game.0x6FC93B90
D2QuestDataStrc* __fastcall QUESTS_GetQuestData(D2GameStrc* pGame, int32_t nId)
{
	D2_ASSERT(nId >= 0);

	if (!pGame || !pGame->pQuestControl)
	{
		return nullptr;
	}

	for (D2QuestDataStrc* pQuestData = pGame->pQuestControl->pLastQuest; pQuestData; pQuestData = pQuestData->pPrev)
	{
		if (pQuestData->nQuestNo == nId)
		{
			return pQuestData;
		}
	}
	
	return nullptr;
}

//D2Game.0x6FC93BD0
void __fastcall QUESTS_AttachLevelChainRecord(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom, int32_t bDebug)
{
	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId)
	{
		return;
	}

	const int32_t nQuestId = DATATBLS_GetLevelsTxtRecord(nLevelId)->nQuest;
	if (!nQuestId || QUESTS_CreateChainRecord(pGame, pUnit, nQuestId) != (bDebug == 0) || !dword_6FD4DC34)
	{
		return;
	}

	sub_6FC84C70(pGame, "Error attaching to unit", STRCOLOR_DARK_GOLD);
}

//D2Game.0x6FC93C40
int32_t __fastcall QUESTS_CreateChainRecord(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nQuestId)
{
	if (!pGame)
	{
		FOG_Trace("Invalid questinfo for item\n");
		return 0;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, nQuestId);
	if (!pQuestData)
	{
		FOG_Trace("Invalid questinfo for item\n");
		return 0;
	}

	int32_t nUnitType = 6;
	if (pUnit)
	{
		nUnitType = pUnit->dwUnitType;
	}

	if (nQuestId == QUEST_A1Q4_CAIN)
	{
		if (nUnitType == UNIT_OBJECT && pUnit && pUnit->dwClassId == OBJECT_INVISIBLE_OBJECT)
		{
			D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
			if (pQuestDataEx)
			{
				pQuestDataEx->bInvisbleObjectInitialized = 1;
				pQuestDataEx->nInvisibleObjectGUID = pUnit ? pUnit->dwUnitId : -1;
			}
		}
	}
	//else if (nQuestId == QUEST_A2Q1_RADAMENT)
	//{
	//	if (nUnitType == UNIT_MONSTER && pUnit && pUnit->dwClassId == MONSTER_RADAMENT)
	//	{
	//		//D2Game_10034_Return((int32_t)pUnit);
	//	}
	//}
	//else if (nQuestId == QUEST_A2Q5_SUMMONER)
	//{
	//	if (nUnitType == UNIT_MONSTER && pUnit && pUnit->dwClassId == MONSTER_SUMMONER)
	//	{
	//		//D2Game_10034_Return((int32_t)pUnit);
	//	}
	//}

	for (D2QuestChainStrc* i = pUnit->pQuestEventList; i; i = i->pNext)
	{
		if (!i->pQuestData)
		{
			break;
		}

		if (i->pQuestData == pQuestData)
		{
			return 0;
		}
	}

	D2QuestChainStrc* pNew = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2QuestChainStrc);
	pNew->pQuestData = pQuestData;
	pNew->pNext = pUnit->pQuestEventList;
	pUnit->pQuestEventList = pNew;
	return 1;
}

//D2Game.0x6FC93D60
void __fastcall QUESTS_InitScrollTextChain(D2QuestDataStrc* pQuest, D2TextHeaderStrc* pTextControl, int32_t nNPCID, int32_t nIndex)
{
	D2NPCMessageTableStrc* pNpcMessageTable = &pQuest->pNPCMessages[nIndex];

	for (int32_t i = 0; i < pNpcMessageTable->nMessages; ++i)
	{
		if (pNpcMessageTable->pMessages[i].nNPCNo == nNPCID)
		{
			int32_t nMenu = pNpcMessageTable->pMessages[i].nMenu;
			if (nMenu == 1)
			{
				nMenu = 0;
			}

			TEXT_AddNodeToTextList(pTextControl, pNpcMessageTable->pMessages[i].nStringIndex, nMenu);
		}
	}
}

//D2Game.0x6FC93DC0
void __fastcall QUESTS_QuestInit(D2GameStrc* pGame)
{
	D2QuestDataStrc* pCurrent = nullptr;
	D2QuestDataStrc* pPrev = nullptr;

	for (int32_t i = 0; i < std::size(gpQuestInitTable); ++i)
	{
		pCurrent = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2QuestDataStrc);

		pCurrent->nQuestNo = gpQuestInitTable[i].nChainNo;
		pCurrent->pGame = pGame;
		pCurrent->bActive = false;
		pCurrent->fLastState = 0;
		pCurrent->bNotIntro = true;
		pCurrent->nActNo = gpQuestInitTable[i].nAct;
		pCurrent->tPlayerGUIDs.nPlayerCount = 0;
		pCurrent->pPrev = pPrev;
		pPrev = pCurrent;

		if (gpQuestInitTable[i].pfInit)
		{
			if (IsBadCodePtr((FARPROC)gpQuestInitTable[i].pfInit))
			{
				FOG_DisplayAssert("sgQuestInit[Index].pQuestInit", __FILE__, __LINE__);
				exit(-1);
			}

			gpQuestInitTable[i].pfInit(pCurrent);
		}
	}

	for (int32_t i = 0; i < std::size(gpQuestIntroTable); ++i)
	{
		pCurrent = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2QuestDataStrc);

		pCurrent->pGame = pGame;
		pCurrent->bActive = true;
		pCurrent->fLastState = 0;
		pCurrent->bNotIntro = false;
		pCurrent->nQuestNo = i + 37;
		pCurrent->nActNo = gpQuestIntroTable[i].nAct;
		pCurrent->pPrev = pPrev;
		pPrev = pCurrent;

		if (gpQuestIntroTable[i].pfInit)
		{
			if (IsBadCodePtr((FARPROC)gpQuestIntroTable[i].pfInit))
			{
				FOG_DisplayAssert("sgQuestIntro[Index].pQuestInit", __FILE__, __LINE__);
				exit(-1);
			}

			gpQuestIntroTable[i].pfInit(pCurrent);
		}
	}

	D2QuestInfoStrc* pQuestInfo = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2QuestInfoStrc);

	pQuestInfo->pLastQuest = pCurrent;

	SEED_InitSeed(&pQuestInfo->pSeed);
	SEED_InitLowSeed(&pQuestInfo->pSeed, (int32_t)ITEMS_RollRandomNumber(&pGame->pGameSeed));

	//pQuestInfo->bExecuting = HIDWORD(pGame->pGameSeed.nHighSeed);

	pQuestInfo->pQuestFlags = QUESTRECORD_AllocRecord(pGame->pMemoryPool);

	pGame->pQuestControl = pQuestInfo;
}

//D2Game.0x6FC93FD0
void __fastcall QUESTS_QuestFree(D2GameStrc* pGame)
{
	if (!pGame || !pGame->pQuestControl)
	{
		return;
	}

	D2QuestInfoStrc* pQuestControl = pGame->pQuestControl;

	D2QuestDataStrc* pQuestData = pQuestControl->pLastQuest;
	while (pQuestData)
	{
		D2QuestDataStrc* pPrevQuestData = pQuestData->pPrev;
		if (pQuestData->pQuestDataEx)
		{
			D2_FREE_POOL(pGame->pMemoryPool, pQuestData->pQuestDataEx);
		}
		D2_FREE_POOL(pGame->pMemoryPool, pQuestData);
		pQuestData = pPrevQuestData;
	}

	D2QuestTimerStrc* pTimer = pQuestControl->pTimer;
	while (pTimer)
	{
		D2QuestTimerStrc* pNextTimer = pTimer->pNext;
		D2_FREE_POOL(pGame->pMemoryPool, pTimer);
		pTimer = pNextTimer;
	}

	QUESTRECORD_FreeRecord(pGame->pMemoryPool, pQuestControl->pQuestFlags);
	D2_FREE_POOL(pGame->pMemoryPool, pQuestControl);
}

//D2Game.0x6FC94080
D2SeedStrc* __fastcall QUESTS_GetGlobalSeed(D2GameStrc* pGame)
{
	D2_ASSERT(pGame && pGame->pQuestControl);

	return &pGame->pQuestControl->pSeed;
}

//D2Game.0x6FC940B0
void __fastcall QUESTS_ParseKill(D2GameStrc* pGame, D2UnitStrc* pDefender, D2UnitStrc* pAttacker)
{
	if (!pDefender->pQuestEventList)
	{
		return;
	}

	if (pGame->nGameType == 3 && !pAttacker)
	{
		D2ClientStrc* pClient = pGame->pClientList;
		if (pClient)
		{
			pAttacker = CLIENTS_GetPlayerFromClient(pClient, 0);
		}
	}

	D2QuestArgStrc pArgs = {};
	pArgs.pGame = pGame;
	pArgs.nEvent = QUESTEVENT_MONSTERKILLED;
	pArgs.pTarget = pDefender;

	if (pAttacker && pAttacker->dwUnitType == UNIT_PLAYER)
	{
		pArgs.pPlayer = pAttacker;
	}
	else
	{
		pArgs.pPlayer = nullptr;
		if (pAttacker && pAttacker->dwUnitType == UNIT_MONSTER)
		{
			int32_t nOwnerType = 0;
			int32_t nOwnerId = 0;
			AIGENERAL_GetOwnerData(pAttacker, &nOwnerId, &nOwnerType);

			if (nOwnerType == UNIT_PLAYER)
			{
				pArgs.pPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nOwnerId);
			}
		}
	}

	bool bForceActive = false;
	if (pDefender && pDefender->dwUnitType == UNIT_MONSTER && pDefender->pMonsterData && (pDefender->pMonsterData->nTypeFlag & MONTYPEFLAG_SUPERUNIQUE))
	{
		const int32_t nSuperUniqueId = MONSTERUNIQUE_GetBossHcIdx(pDefender);
		if (nSuperUniqueId >= 0 && nSuperUniqueId < sgptDataTables->nSuperUniquesTxtRecordCount)
		{
			const int32_t nHcIdx = sgptDataTables->pSuperUniquesTxt[nSuperUniqueId].dwHcIdx;
			switch (nHcIdx)
			{
			case SUPERUNIQUE_ISMAIL_VILEHAND:
			case SUPERUNIQUE_GELEB_FLAMEFINGER:
			case SUPERUNIQUE_TOORC_ICEFIST:
			case SUPERUNIQUE_INFECTOR_OF_SOULS:
			case SUPERUNIQUE_LORD_DE_SEIS:
			case SUPERUNIQUE_GRAND_VIZIER_OF_CHAOS:
			case SUPERUNIQUE_THE_COW_KING:
			case SUPERUNIQUE_SIEGE_BOSS:
			case SUPERUNIQUE_ANCIENT_BARBARIAN_1:
			case SUPERUNIQUE_ANCIENT_BARBARIAN_2:
			case SUPERUNIQUE_ANCIENT_BARBARIAN_3:
			case SUPERUNIQUE_NIHLATHAK_BOSS:
				bForceActive = true;
				break;

			default:
				break;
			}
		}
	}

	switch (pDefender->dwClassId)
	{
	case MONSTER_MEPHISTO:
	case MONSTER_DIABLO:
	case MONSTER_HELLBOVINE:
	case MONSTER_BAALCRAB:
		bForceActive = true;
		break;

	default:
		break;
	}

	QUESTS_StatusCycler(&pArgs, bForceActive);
}

//D2Game.0x6FC94210
void __fastcall QUESTS_StatusCycler(D2QuestArgStrc* pArgs, bool bForceActive)
{
	for (D2QuestChainStrc* i = pArgs->pTarget->pQuestEventList; i; i = i->pNext)
	{
		if (i->pQuestData->pfCallback[pArgs->nEvent] && (i->pQuestData->bActive == true || bForceActive))
		{
			if (IsBadCodePtr((FARPROC)i->pQuestData->pfCallback[pArgs->nEvent]))
			{
				FOG_DisplayAssert("pHead->pQuest->pCallback[pqParam->eCallback]", __FILE__, __LINE__);
				exit(-1);
			}

			i->pQuestData->pfCallback[pArgs->nEvent](i->pQuestData, pArgs);
		}
	}
}

//D2Game.0x6FC94290
void __fastcall QUESTS_ChangeLevel(D2GameStrc* pGame, int32_t nOldLevelId, int32_t nTargetLevelId, D2UnitStrc* pUnit)
{
	D2QuestArgStrc pArgs = {};

	pArgs.pGame = pGame;
	pArgs.nOldLevel = nOldLevelId;
	pArgs.pPlayer = pUnit;
	pArgs.nEvent = QUESTEVENT_CHANGEDLEVEL;
	pArgs.nNewLevel = nTargetLevelId;
	QUESTS_EventCallback(&pArgs, true, false);
}

//D2Game.0x6FC942D0
void __fastcall QUESTS_EventCallback(D2QuestArgStrc* pArgs, bool bCheckActive, bool bCheckAct)
{
	if (!pArgs || !pArgs->pGame || !pArgs->pGame->pQuestControl)
	{
		return;
	}

	int8_t nAct = -1;
	if (bCheckAct)
	{
		D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pArgs->pPlayer);
		if (pRoom)
		{
			nAct = DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom));
		}
	}

	D2QuestDataStrc* pQuestData = pArgs->pGame->pQuestControl->pLastQuest;
	while(pQuestData)
	{
		if (pQuestData->pfCallback[pArgs->nEvent] && (bCheckActive || pQuestData->bActive) && (nAct == -1 || nAct == pQuestData->nActNo))
		{
			if (IsBadCodePtr((FARPROC)pQuestData->pfCallback[pArgs->nEvent]))
			{
				FOG_DisplayAssert("pQuest->pCallback[pqParam->eCallback]", __FILE__, __LINE__);
				exit(-1);
			}

			pQuestData->pfCallback[pArgs->nEvent](pQuestData, pArgs);
		}

		pQuestData = pQuestData->pPrev;
	}
}

//D2Game.0x6FC94390
void __fastcall QUESTS_PlayerDroppedWithQuestItem(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
	if (!pGame || !pPlayer)
	{
		return;
	}

	D2QuestArgStrc pArgs = {};
	pArgs.pPlayer = pPlayer;
	pArgs.pGame = pGame;
	pArgs.nEvent = QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM;

	for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pPlayer->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
	{
		pItem = INVENTORY_UnitIsItem(pItem);

		D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
		if (pItemsTxtRecord && pItemsTxtRecord->nQuest)
		{
			D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, pItemsTxtRecord->nQuest - 1);

			pArgs.pTarget = pItem;

			if (pQuestData && pQuestData->pfCallback[QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM])
			{
				if (IsBadCodePtr((FARPROC)pQuestData->pfCallback[QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM]))
				{
					FOG_DisplayAssert("pQuestInfo->pCallback[QUESTCALLBACK_PLAYERDROPPEDWITHQUESTITEM]", __FILE__, __LINE__);
					exit(-1);
				}
				pQuestData->pfCallback[QUESTEVENT_PLAYERDROPPEDWITHQUESTITEM](pQuestData, &pArgs);
			}
		}
	}

	pArgs.nEvent = QUESTEVENT_PLAYERLEAVESGAME;
	QUESTS_EventCallback(&pArgs, true, false);
}

//D2Game.0x6FC944B0
void __fastcall QUESTS_NPCActivate(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pTarget, D2TextHeaderStrc* pTextControl)
{
	if (!pClient)
	{
		return;
	}

	D2QuestArgStrc pArgs = {};
	pArgs.pGame = pClient->pGame;
	pArgs.pTextControl = pTextControl;
	pArgs.nEvent = QUESTEVENT_NPCACTIVATE;
	pArgs.pTarget = pTarget;
	pArgs.pPlayer = pPlayer;
	QUESTS_EventCallback(&pArgs, true, true);
}

//D2Game.0x6FC944F0
void __fastcall QUESTS_NPCDeactivate(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNPC)
{
	D2QuestArgStrc pArgs = {};

	pArgs.pGame = pGame;
	pArgs.pPlayer = pPlayer;
	pArgs.pTarget = pNPC;
	pArgs.nEvent = QUESTEVENT_NPCDEACTIVATE;
	QUESTS_EventCallback(&pArgs, true, true);
}

//D2Game.0x6FC94520
void __fastcall QUESTS_ItemPickedUp(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
	D2QuestArgStrc pArgs = {};

	pArgs.pGame = pGame;
	pArgs.pPlayer = pPlayer;
	pArgs.nEvent = QUESTEVENT_ITEMPICKEDUP;
	pArgs.pTarget = pItem;
	QUESTS_StatusCycler(&pArgs, true);
}

//D2Game.0x6FC94550
void __fastcall QUESTS_ItemDropped(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
	D2QuestArgStrc pArgs = {};

	pArgs.pGame = pGame;
	pArgs.pPlayer = pPlayer;
	pArgs.nEvent = QUESTEVENT_ITEMDROPPED;
	pArgs.pTarget = pItem;
	QUESTS_StatusCycler(&pArgs, true);
}

//D2Game.0x6FC94580
void __fastcall QUESTS_QuestUpdater(D2GameStrc* pGame)
{
	if (!pGame || !pGame->pQuestControl)
	{
		return;
	}

	++pGame->pQuestControl->dwTick;
	if (pGame->pQuestControl->dwTick == -1)
	{
		for (D2QuestTimerStrc* i = pGame->pQuestControl->pTimer; i; i = i->pNext)
		{
			i->dwTicks = -1 - i->dwTicks;
		}
	}

	pGame->pQuestControl->bExecuting = 1;

	D2QuestTimerStrc* pTimer = pGame->pQuestControl->pTimer;
	D2QuestTimerStrc* pPrev = nullptr;
	while (pTimer)
	{
		D2QuestTimerStrc* pNext = pTimer->pNext;
		if (pTimer->dwTicks < pGame->pQuestControl->dwTick)
		{
			if (IsBadCodePtr((FARPROC)pTimer->pfUpdate))
			{
				FOG_DisplayAssert("pTimer->pCallback", __FILE__, __LINE__);
				exit(-1);
			}

			if (pTimer->pfUpdate(pGame, pTimer->pQuest) == 1)
			{
				if (pPrev)
				{
					pPrev->pNext = pTimer->pNext;
					D2_FREE_POOL(pGame->pMemoryPool, pTimer);
					pTimer = pPrev;
				}
				else
				{
					pGame->pQuestControl->pTimer = pTimer->pNext;
					D2_FREE_POOL(pGame->pMemoryPool, pTimer);
					pTimer = 0;
				}
			}
			else
			{
				pTimer->dwTicks = pGame->pQuestControl->dwTick + pTimer->dwTimeout;
			}
		}
		pPrev = pTimer;
		pTimer = pNext;
	}

	pGame->pQuestControl->bExecuting = 0;
}

//D2Game.0x6FC94690
void __fastcall QUESTS_CreateTimer(D2QuestDataStrc* pQuest, QUESTUPDATE pfnCallback, int32_t nTicks)
{
	if (!pQuest || !pQuest->pGame)
	{
		return;
	}

	D2QuestInfoStrc* pQuestControl = pQuest->pGame->pQuestControl;
	if (!pQuestControl)
	{
		return;
	}

	D2_ASSERT(!pQuestControl->bExecuting);

	D2QuestTimerStrc* pQuestTimer = D2_ALLOC_STRC_POOL(pQuest->pGame->pMemoryPool, D2QuestTimerStrc);
	pQuestTimer->pNext = pQuestControl->pTimer;
	pQuestTimer->pQuest = pQuest;
	pQuestTimer->pfUpdate = pfnCallback;
	pQuestTimer->dwTimeout = nTicks;
	pQuestTimer->dwTicks = nTicks + pQuestControl->dwTick;

	pQuestControl->pTimer = pQuestTimer;
}

//D2Game.0x6FC94710
void __fastcall QUESTS_RefreshStatus(D2QuestDataStrc* pQuest, uint8_t* pQuestList, D2BitBufferStrc* pQuestFlags)
{
	const int32_t bCompletedNow = QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_COMPLETEDNOW);

	if (pQuest->fState < pQuest->nInitNo)
	{
		if (!bCompletedNow)
		{
			if (pQuest->nQuestNo != 4 || pQuest->fLastState != 6)
			{
				*pQuestList = pQuest->fLastState;
				return;
			}

			if (!QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_PRIMARYGOALDONE))
			{
				*pQuestList = 12;
				return;
			}

			*pQuestList = pQuest->fLastState;
			return;
		}

		*pQuestList = 12;
		return;
	}

	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, QFLAG_PRIMARYGOALDONE) != 1)
	{
		if (pQuest->nQuestNo == 4 && bCompletedNow)
		{
			if (pQuest->fState == 6)
			{
				*pQuestList = 12;
				return;
			}
			*pQuestList = pQuest->fLastState;
			return;
		}

		if (pQuest->nQuestNo == 10)
		{
			*pQuestList = 8 * (pQuest->fLastState != 4) + 4;
			return;
		}

		*pQuestList = 12;
		return;
	}

	//if (pQuest->nQuest >= 41)
	//{
	//	FOG_DisplayAssert("pQuest->eFilter < MAX_QUEST_STATUS", __FILE__, __LINE__);
	//	exit(-1);
	//}

	*pQuestList = pQuest->fLastState;
}

//D2Game.0x6FC947F0
void __fastcall QUESTS_StatusCallback(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);

	D2GAME_SendPacket0x28_6FC3F2F0(pClient, 0x28u, 6u, 0, pQuestFlags, 0);

	D2GSPacketSrv52 packet52 = {};
	packet52.nHeader = 0x52;

	if (!pGame->pQuestControl)
	{
		return;
	}

	for (D2QuestDataStrc* pQuest = pGame->pQuestControl->pLastQuest; pQuest; pQuest = pQuest->pPrev)
	{
		//if (pQuest->nQuestNo >= 41)
		//{
		//	FOG_DisplayAssert("pQuest->Id < MAX_QUEST_STATUS", __FILE__, __LINE__);
		//	exit(-1);
		//}

		if (pQuest->fLastState)
		{
			if (pQuest->pfStatusFilter)
			{
				if (IsBadCodePtr((FARPROC)pQuest->pfStatusFilter))
				{
					FOG_DisplayAssert("pQuest->pStatusFilter", __FILE__, __LINE__);
					exit(-1);
				}

				uint8_t a5 = 0;
				if (pQuest->pfStatusFilter(pQuest, pUnit, pGame->pQuestControl->pQuestFlags, pQuestFlags, &a5) == 1)
				{
					packet52.pQuestList[pQuest->nQuest] = a5;
				}
			}
			else
			{
				QUESTS_RefreshStatus(pQuest, &packet52.pQuestList[pQuest->nQuest], pQuestFlags);
			}
		}
	}

	bool bSendPacket0x50 = false;
	D2GSPacketSrv50 packet50 = {};
	if (packet52.pQuestList[1] && pGame->pQuestControl)
	{
		D2QuestDataStrc* pQuestData1 = pGame->pQuestControl->pLastQuest;
		while (pQuestData1 && pQuestData1->nQuestNo != 1)
		{
			pQuestData1 = pQuestData1->pPrev;
		}

		if (pQuestData1)
		{
			bSendPacket0x50 = true;
			packet50.QuestsStatusPayload.nMonstersToBeKilled = ACT1Q1_GetMonstersToBeKilled(pQuestData1);
		}
	}

	if (packet52.pQuestList[36] && pGame->pQuestControl)
	{
		D2QuestDataStrc* pQuestData32 = pGame->pQuestControl->pLastQuest;
		while (pQuestData32 && pQuestData32->nQuestNo != QUEST_A5Q2_RESCUESOLDIERS)
		{
			pQuestData32 = pQuestData32->pPrev;
		}

		if (pQuestData32)
		{
			bSendPacket0x50 = true;
			packet50.QuestsStatusPayload.nBarbsToBeRescued = ACT5Q2_GetBarbsToBeRescued(pQuestData32);
		}
	}

	if ((QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_REWARDGRANTED) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q4, QFLAG_PRIMARYGOALDONE)) && pGame->pAct[ACT_II])
	{
		packet50.QuestsStatusPayload.nStaffTombLevelOffset = DUNGEON_GetHoradricStaffTombLevelId(pGame->pAct[1]) - LEVEL_TALRASHASTOMB1;
	}
	else
	{
		if (!bSendPacket0x50)
		{
			D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet52, sizeof(packet52));
			return;
		}
	}

	packet50.nHeader = 0x50;
	packet50.nQuestId = QUEST_A1Q1_DENOFEVIL; // Used for the 3 progress quests at once.
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet50, sizeof(packet50));
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet52, sizeof(packet52));
}

//D2Game.0x6FC94A30
int32_t __fastcall QUESTS_DebugOutput(D2GameStrc* pGame, const char* szMessage, const char* szFile, int32_t nLine)
{
	if (!dword_6FD4DC34)
	{
		return 0;
	}

	sub_6FC84C70(pGame, szMessage, STRCOLOR_DARK_GOLD);
	return 1;
}

//D2Game.0x6FC94A50
void __fastcall QUESTS_DeleteItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint32_t dwItemCode)
{
	D2UnitStrc* pItem = ITEMS_FindQuestItem(pGame, pPlayer, dwItemCode);
	if (pItem)
	{
		QUESTS_DeleteItemEx(pGame, pPlayer, pPlayer->pInventory, pItem);
	}
}

//D2Game.0x6FC94B20
void __fastcall QUESTS_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t nQuestId)
{
	if (!pGame)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, nQuestId);
	if (!pQuestData)
	{
		return;
	}

	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayer);
	if (!pRoom)
	{
		return;
	}

	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (!nLevelId || pQuestData->nActNo != DRLG_GetActNoFromLevelId(nLevelId))
	{
		return;
	}

	D2GSPacketSrv5D packet5D = {};
	packet5D.nHeader = 0x5D;
	packet5D.nQuestId = nQuestId;
	packet5D.nFlags = (uint8_t)pQuestData->dwFlags;
	packet5D.nState = 0;
	packet5D.field_4 = 0;

	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty];
	if (pQuestData->pfStatusFilter)
	{
		if (IsBadCodePtr((FARPROC)pQuestData->pfStatusFilter))
		{
			FOG_DisplayAssert("pQuestInfo->pStatusFilter", __FILE__, __LINE__);
			exit(-1);
		}

		uint8_t a5 = 0;
		if (pQuestData->pfStatusFilter(pQuestData, pPlayer, pGame->pQuestControl->pQuestFlags, pQuestFlags, &a5) == 1)
		{
			packet5D.nState = a5;
		}
	}
	else
	{
		QUESTS_RefreshStatus(pQuestData, &packet5D.nState, pQuestFlags);
	}

	if (pQuestData->nQuest == 1)
	{
		packet5D.field_4 = ACT1Q1_GetMonstersToBeKilled(pQuestData);
	}
	else if (pQuestData->nQuest == 36)
	{
		packet5D.field_4 = ACT5Q2_GetBarbsToBeRescued(pQuestData);
	}

	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5D, sizeof(packet5D));
}

//D2Game.0x6FC94CA0
void __fastcall QUESTS_UnitIterate(D2QuestDataStrc* pQuest, int32_t nIterateState, D2UnitStrc* pUnit, int32_t(__fastcall* pfIterate)(D2GameStrc*, D2UnitStrc*, void*), bool bIterate)
{
	pQuest->fLastState = nIterateState;

	if (bIterate == 1)
	{
		D2_ASSERT(pfIterate);

		SUNIT_IterateUnitsOfType(pQuest->pGame, 0, pUnit, pfIterate);
	}
}

//D2Game.0x6FC94CF0
int32_t __fastcall QUESTS_StateDebug(D2QuestDataStrc* pQuestData, int32_t nState, const char* szFile, int32_t nLine)
{
	pQuestData->fState = nState;

	if (!dword_6FD4DC34)
	{
		return 0;
	}

	char szMsg[256] = {};
	wsprintfA(szMsg, "changing state to %d\n", nState);
	sub_6FC84C70(pQuestData->pGame, szMsg, STRCOLOR_DARK_GOLD);
	return 1;
}

//D2Game.0x6FC94D40
void __fastcall QUESTS_NPCMessage(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t dwNPCGUID, uint16_t nMessage)
{
	D2QuestArgStrc pArgs = {};

	pArgs.pGame = pGame;
	pArgs.nEvent = QUESTEVENT_SCROLLMESSAGE;
	pArgs.pPlayer = pPlayer;
	pArgs.nNPCNo = 0;

	if (dwNPCGUID == -1)
	{
		pArgs.pTarget = 0;
	}
	else
	{
		pArgs.pTarget = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, dwNPCGUID);
		if (pArgs.pTarget)
		{
			pArgs.nNPCNo = (int16_t)pArgs.pTarget->dwClassId;
		}
	}

	pArgs.nMessageIndex = nMessage;
	QUESTS_EventCallback(&pArgs, true, true);
}

//D2Game.0x6FC94DB0
void __fastcall QUESTS_SequenceCycler(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t bGameEnter)
{
	if (!pGame || !pGame->pQuestControl)
	{
		return;
	}

	D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
	D2BitBufferStrc* pQuestFlags = pPlayerData->pQuestData[pGame->nDifficulty];

	if (!pGame->pQuestControl->bPickedSet)
	{
		pGame->pQuestControl->bPickedSet = 1;

		if (!bGameEnter)
		{
			if (byte_6FD31270)
			{
				for (int32_t i = 0; i < std::size(gpQuestInitTable); ++i)
				{
					D2_ASSERT(gpQuestInitTable[i].nAct < NUM_ACTS);

					if (!gpQuestInitTable[i].bNoSetState
						&& (QUESTRECORD_GetQuestState(pQuestFlags, gpQuestInitTable[i].nQuestNo, QFLAG_REWARDGRANTED) 
							|| QUESTRECORD_GetQuestState(pQuestFlags, gpQuestInitTable[i].nQuestNo, QFLAG_COMPLETEDBEFORE)))
					{
						D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pGame, gpQuestInitTable[i].nChainNo);

						D2_ASSERT(pQuest);

						pQuest->bNotIntro = 0;
						pQuest->bActive = 0;
						pQuest->fLastState = 0;

						//TODO: ???
						//++*((DWORD*)&pArgs.pGame + gpQuestInitTable[nCounter].nAct);
						QUESTRECORD_SetQuestState(pGame->pQuestControl->pQuestFlags, gpQuestInitTable[i].nQuestNo, QFLAG_COMPLETEDBEFORE);
					}
				}
			}

			D2QuestDataStrc* pQuest = pGame->pQuestControl->pLastQuest;
			while (pQuest)
			{
				if (pQuest->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME])
				{
					if (IsBadCodePtr((FARPROC)pQuest->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME]))
					{
						FOG_DisplayAssert("pQuest->pCallback[QUESTCALLBACK_PLAYERSTARTEDGAME]", __FILE__, __LINE__);
						exit(-1);
					}

					D2QuestArgStrc pArgs = {};
					pArgs.pPlayer = pPlayer;
					pArgs.pGame = pGame;
					pArgs.nEvent = QUESTEVENT_PLAYERSTARTEDGAME;
					pQuest->pfCallback[QUESTEVENT_PLAYERSTARTEDGAME](pQuest, &pArgs);
				}

				pQuest = pQuest->pPrev;
			}
		}

		D2QuestDataStrc* pQuestData1 = pGame->pQuestControl->pLastQuest;
		while (pQuestData1 && pQuestData1->nQuestNo != 1)
		{
			pQuestData1 = pQuestData1->pPrev;
		}
		if (pQuestData1)
		{
			if (IsBadCodePtr((FARPROC)pQuestData1->pfSeqFilter))
			{
				FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
				exit(-1);
			}
			((bool(__fastcall*)(D2QuestDataStrc*))pQuestData1->pfSeqFilter)(pQuestData1);
		}

		D2QuestDataStrc* pQuestData8 = pGame->pQuestControl->pLastQuest;
		while (pQuestData8 && pQuestData8->nQuestNo != 8)
		{
			pQuestData8 = pQuestData8->pPrev;
		}
		if (pQuestData8)
		{
			if (IsBadCodePtr((FARPROC)pQuestData8->pfSeqFilter))
			{
				FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
				exit(-1);
			}
			((bool(__fastcall*)(D2QuestDataStrc*))pQuestData8->pfSeqFilter)(pQuestData8);
		}

		D2QuestDataStrc* pQuestData18 = pGame->pQuestControl->pLastQuest;
		while (pQuestData18 && pQuestData18->nQuestNo != 18)
		{
			pQuestData18 = pQuestData18->pPrev;
		}
		if (pQuestData18)
		{
			if (IsBadCodePtr((FARPROC)pQuestData18->pfSeqFilter))
			{
				FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
				exit(-1);
			}
			((bool(__fastcall*)(D2QuestDataStrc*))pQuestData18->pfSeqFilter)(pQuestData18);
		}

		D2QuestDataStrc* pQuestData22 = pGame->pQuestControl->pLastQuest;
		while (pQuestData22 && pQuestData22->nQuestNo != 22)
		{
			pQuestData22 = pQuestData22->pPrev;
		}
		if (pQuestData22)
		{
			if (IsBadCodePtr((FARPROC)pQuestData22->pfSeqFilter))
			{
				FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
				exit(-1);
			}
			((bool(__fastcall*)(D2QuestDataStrc*))pQuestData22->pfSeqFilter)(pQuestData22);
		}

		D2QuestDataStrc* pQuestData31 = pGame->pQuestControl->pLastQuest;
		while (pQuestData31 && pQuestData31->nQuestNo != 31)
		{
			pQuestData31 = pQuestData31->pPrev;
		}
		if (pQuestData31)
		{
			if (IsBadCodePtr((FARPROC)pQuestData31->pfSeqFilter))
			{
				FOG_DisplayAssert("pQuestInfo->pSequence", __FILE__, __LINE__);
				exit(-1);
			}
			((bool(__fastcall*)(D2QuestDataStrc*))pQuestData31->pfSeqFilter)(pQuestData31);
		}
	}
	else
	{
		D2QuestDataStrc* pQuest = pGame->pQuestControl->pLastQuest;
		while (pQuest)
		{
			if (pQuest->pfCallback[QUESTEVENT_PLAYERJOINEDGAME])
			{
				if (IsBadCodePtr((FARPROC)pQuest->pfCallback[QUESTEVENT_PLAYERJOINEDGAME]))
				{
					FOG_DisplayAssert("pQuest->pCallback[QUESTCALLBACK_PLAYERJOINEDGAME]", __FILE__, __LINE__);
					exit(-1);
				}

				D2QuestArgStrc pArgs = {};
				pArgs.pPlayer = pPlayer;
				pArgs.pGame = pGame;
				pArgs.nEvent = QUESTEVENT_PLAYERJOINEDGAME;
				pQuest->pfCallback[QUESTEVENT_PLAYERJOINEDGAME](pQuest, &pArgs);
			}
			pQuest = pQuest->pPrev;
		}
	}

	D2GSPacketSrv5E packet5E = {};

	for (int32_t i = 0; i < std::size(gpQuestInitTable); ++i)
	{
		D2QuestDataStrc* pQuest = QUESTS_GetQuestData(pGame, gpQuestInitTable[i].nChainNo);

		D2_ASSERT(pQuest);

		packet5E.unk0x001[i] = pQuest->bNotIntro;
	}

	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);

	packet5E.nHeader = 0x5E;
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5E, sizeof(packet5E));

	D2GAME_SendPacket0x28_6FC3F2F0(pClient, 0x28u, 6u, 0, pQuestFlags, 0);

	D2_ASSERT(pGame->pQuestControl->bPickedSet);

	sub_6FC3F370(pClient, pGame->pQuestControl->pQuestFlags);

	D2QuestDataStrc* pQuestData1 = pGame->pQuestControl->pLastQuest;
	while (pQuestData1 && pQuestData1->nQuestNo != 1)
	{
		pQuestData1 = pQuestData1->pPrev;
	}
	if (pQuestData1 && ACT1Q1_CanClientFXBeTriggered(pQuestData1))
	{
		D2GSPacketSrv89 packet89 = {};
		packet89.nHeader = 0x89;
		D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet89, sizeof(packet89));
	}
}

//D2Game.0x6FC95360 (#10037)
int32_t __stdcall QUESTS_CheckFirstPickedSet(D2GameStrc* pGame)
{
	D2_ASSERT(pGame);

	if (!pGame->pQuestControl)
	{
		return 0;
	}

	return pGame->pQuestControl->bPickedSet;
}

//D2Game.0x6FC953A0 (#10038)
int32_t __stdcall QUESTS_CheckNotIntroQuest(D2GameStrc* pGame, int32_t nQuestId)
{
	D2_ASSERT(pGame && pGame->pQuestControl && pGame->pQuestControl->bPickedSet);

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, nQuestId);
	if (!pQuestData)
	{
		return 1;
	}

	return pQuestData->bNotIntro == 1;
}

//D2Game.0x6FC95430
void __fastcall QUESTS_SendCurrentFlags(D2GameStrc* pGame, D2ClientStrc* pClient)
{
	D2_ASSERT(pGame && pGame->pQuestControl && pGame->pQuestControl->bPickedSet);

	sub_6FC3F370(pClient, pGame->pQuestControl->pQuestFlags);
}

//D2Game.0x6FC95490
int32_t __fastcall QUESTS_ActiveCycler(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNPC)
{
	D2_ASSERT(pPlayer && pPlayer->dwUnitType == UNIT_PLAYER);
	D2_ASSERT(pNPC && pNPC->dwUnitType == UNIT_MONSTER);

	D2MonStatsTxt* pMonstatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pNPC->dwClassId);
	if (!pMonstatsTxtRecord || !(pMonstatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT]))
	{
		return 0;
	}

	D2_ASSERT(pGame->pQuestControl->bPickedSet);

	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
	if (!pClient)
	{
		return 0;
	}

	for (D2QuestDataStrc* pQuestData = pGame->pQuestControl->pLastQuest; pQuestData; pQuestData = pQuestData->pPrev)
	{
		if (pClient->nAct == pQuestData->nActNo)
		{
			D2_ASSERT(pQuestData->pfActiveFilter);

			if (IsBadCodePtr((FARPROC)pQuestData->pfActiveFilter))
			{
				FOG_DisplayAssert("pQuest->pActiveFilter", __FILE__, __LINE__);
				exit(-1);
			}

			D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
			D2_ASSERT(pPlayerData);

			D2BitBufferStrc* pQuestFlags = pPlayerData->pQuestData[pGame->nDifficulty];
			D2_ASSERT(pQuestFlags);

			if (pQuestData->pfActiveFilter(pQuestData, pNPC->dwClassId, pPlayer, pQuestFlags, pNPC))
			{
				D2GSPacketSrv8A packet8A = {};
				packet8A.nHeader = 0x8A;
				packet8A.nUnitType = UNIT_MONSTER;
				packet8A.dwUnitGUID = pNPC->dwUnitId;
				D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet8A, sizeof(packet8A));
				return 1;
			}
		}
	}

	return 0;
}

//D2Game.0x6FC956C0
void __fastcall QUESTS_SetGlobalState(D2GameStrc* pGame, int32_t nQuestId, int32_t nState)
{
	D2_ASSERT(pGame && pGame->pQuestControl && pGame->pQuestControl->bPickedSet);

	QUESTRECORD_SetQuestState(pGame->pQuestControl->pQuestFlags, nQuestId, nState);
}

//D2Game.0x6FC95700
int32_t __fastcall QUESTS_GetGlobalState(D2GameStrc* pGame, int32_t nQuestId, int32_t nState)
{
	D2_ASSERT(pGame && pGame->pQuestControl && pGame->pQuestControl->bPickedSet);

	return QUESTRECORD_GetQuestState(pGame->pQuestControl->pQuestFlags, nQuestId, nState);
}

//D2Game.0x6FC95740
void __fastcall QUESTS_WarrivSpawn(D2GameStrc* pGame, D2UnitStrc* pWarriv, int32_t nXpos, int32_t nYpos, int32_t nWarrivID)
{
	if (nWarrivID >= 0 && nWarrivID < sgptDataTables->nMonStatsTxtRecordCount)
	{
		if (nWarrivID == MONSTER_JERHYN || nWarrivID == MONSTER_ACT2GUARD2)
		{
			//D2GAME_Return_6FCA3310(nXpos, nYpos);
		}
	}
}

//D2Game.0x6FC95790
void __fastcall QUESTS_UseClueItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUnitId)
{
	D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nUnitId);

	if (!pItem || !ITEMS_IsInPlayersInventory(pUnit, pItem, nullptr))
	{
		return;
	}

	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	if (!pItemsTxtRecord)
	{
		return;
	}

	if (pItemsTxtRecord->dwCode == ' dkb')
	{
		ACT1Q4_SendStoneOrderToClient(pGame, pUnit);
	}
	else if (pItemsTxtRecord->dwCode == ' srt')
	{
		ACT2Q6_SendTombIdToClient(pGame, pUnit);
	}
}

//D2Game.0x6FC95800
void __fastcall OBJECTS_InitFunction18_JerhynPosition(D2ObjInitFnStrc* pOp)
{
	if (!pOp || !pOp->pGame)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData)
	{
		return;
	}

	ACT2Q4_InitializeJerhynStartObject(pQuestData, pOp);
}

//D2Game.0x6FC95830
void __fastcall OBJECTS_InitFunction19_JerhynPositionEx(D2ObjInitFnStrc* pOp)
{
	if (!pOp || !pOp->pGame)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q4_HORAZONTOME);
	if (!pQuestData)
	{
		return;
	}

	ACT2Q4_InitializeJerhynPalaceObject(pQuestData, pOp);
}

//D2Game.0x6FC95860
void __fastcall OBJECTS_InitFunction20_TaintedAltar(D2ObjInitFnStrc* pOp)
{
	if (pOp && pOp->pGame)
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A2Q3_TAINTEDSUN);
		if (pQuestData)
		{
			D2Act2Quest3Strc* pQuestDataEx = (D2Act2Quest3Strc*)pQuestData->pQuestDataEx;
			pQuestDataEx->bAltarInitialized = 1;

			int32_t nObjectId = -1;
			if (pOp->pObject)
			{
				nObjectId = pOp->pObject->dwUnitId;
			}

			pQuestDataEx->nAltarGUID = nObjectId;

			if (pQuestData->bNotIntro != 1)
			{

				UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAltarMode);
				return;
			}

			if (pQuestData->fState <= 1)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 1, 0, ACT2Q3_UnitIterate_StatusCyclerEx, 1);
				if (!pQuestData->fState)
				{
					QUESTS_StateDebug(pQuestData, 1, "..\\D2Game/Quests/a2q3.cpp", 529);
				}

				if (pQuestData->pGame->pAct[ACT_II])
				{
					ENVIRONMENT_TaintedSunBegin(pQuestData->pGame->pAct[ACT_II]);
					SUNIT_IterateUnitsOfType(pQuestData->pGame, 0, 0, ACT2Q3_UnitIterate_UpdateClient);
					pQuestDataEx->bRewarded = 1;
				}
				else
				{
					pQuestDataEx->bBlackened = 1;
				}
				pQuestData->fState = 3;
				pQuestData->dwFlags &= 0xFFFFFF00;
			}

			if (!pQuestData->fLastState)
			{
				pQuestData->dwFlags &= 0xFFFFFF00;
				QUESTS_UnitIterate(pQuestData, 2, 0, ACT2Q3_UnitIterate_StatusCyclerEx, 1);
			}

			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nAltarMode);
			return;
		}
	}

	if (!pOp->pObject || pOp->pObject->dwAnimMode != OBJMODE_OPENED)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
}

//D2Game.0x6FC958A0
void __fastcall OBJECTS_InitFunction15_MalusStand(D2ObjInitFnStrc* pOp)
{
	if (pOp && pOp->pGame)
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q3_MALUS);
		if (pQuestData)
		{
			D2Act1Quest3Strc* pQuestDataEx = (D2Act1Quest3Strc*)pQuestData->pQuestDataEx;
			pQuestDataEx->bMalusInitialized = 1;

			int32_t nObjectId = -1;
			if (pOp->pObject)
			{
				nObjectId = pOp->pObject->dwUnitId;
			}

			pQuestDataEx->nMalusGUID = nObjectId;
			if (!pQuestData->bNotIntro)
			{
				pQuestDataEx->nMalusObjectMode = OBJMODE_OPENED;
			}
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nMalusObjectMode);
			return;
		}
	}

	if (!pOp->pObject || pOp->pObject->dwAnimMode != OBJMODE_OPENED)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
}

//D2Game.0x6FC958E0
void __fastcall OBJECTS_InitFunction07_CainGibbet(D2ObjInitFnStrc* pOp)
{
	if (pOp && pOp->pGame)
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A1Q4_CAIN);

		if (pQuestData)
		{
			D2Act1Quest4Strc* pQuestDataEx = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
			UNITS_ChangeAnimMode(pOp->pObject, pQuestDataEx->nCainGibbetObjectMode);
			pQuestDataEx->bCageModeChanged = 1;

			if (pOp->pObject)
			{
				pQuestDataEx->nCainGibbetGUID = pOp->pObject->dwUnitId;
			}
			else
			{
				pQuestDataEx->nCainGibbetGUID = -1;
			}

			return;
		}
	}

	if (!pOp->pObject || pOp->pObject->dwAnimMode != OBJMODE_OPENED)
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
}

//D2Game.0x6FC95920
void __fastcall QUESTS_DisableSequences()
{
	byte_6FD31270 = 0;
}

//D2Game.0x6FC95930
void __fastcall QUESTS_ObjectEvents(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	if (!pGame)
	{
		return;
	}

	int32_t nUnitId = -1;
	if (pUnit)
	{
		nUnitId = pUnit->dwClassId;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);

	switch (nUnitId)
	{
	case OBJECT_HELLFORGE:
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A4Q3_HELLFORGE);
		if (pQuestData)
		{
			ACT4Q3_CreateReward(pQuestData, pUnit);
		}
		break;
	}
	case OBJECT_ANCIENTSTATUE3:
	case OBJECT_ANCIENTSTATUE1:
	case OBJECT_ANCIENTSTATUE2:
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q5_ANCIENTS);
		if (pQuestData)
		{
			ACT5Q5_SpawnAncientMonsters(pQuestData, pUnit);
		}
		break;
	}
	case OBJECT_NIHLATHAK_START_IN_TOWN:
	{
		ACT5Q3_RemoveNihlathakFromTown(pGame);
		break;
	}
	case OBJECT_DREHYA_START_IN_TOWN:
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q4_NIHLATHAK);
		if (!pQuestData)
		{
			return;
		}

		D2Act5Quest4Strc* pAct5Quest4 = (D2Act5Quest4Strc*)pQuestData->pQuestDataEx;
		if (!pAct5Quest4 || !pAct5Quest4->bNeedsToOpenPortal)
		{
			return;
		}

		if (pAct5Quest4->bPortalCreated)
		{
			EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_QUESTFN, pGame->dwGameFrame + 12, 0, 0);
			return;
		}

		const int32_t nX = CLIENTS_GetUnitX(pUnit);
		const int32_t nY = CLIENTS_GetUnitY(pUnit);

		if (D2GAME_CreatePortalObject_6FD13DF0(pQuestData->pGame, nullptr, pRoom, nX + 10, nY + 5, LEVEL_NIHLATHAKSTEMPLE, nullptr, OBJECT_PERMANENT_TOWN_PORTAL, 1))
		{
			pAct5Quest4->bPortalCreated = 1;
			pAct5Quest4->bNeedsToOpenPortal = 0;
		}
		else
		{
			EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_QUESTFN, pGame->dwGameFrame + 12, 0, 0);
		}
		break;
	}
	case OBJECT_DREHYA_START_OUTSIDE_TOWN:
	{
		ACT5Q3_SpawnFrozenDrehya(pGame, pUnit);
		break;
	}
	case OBJECT_CHEST:
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q5_COUNTESS);
		if (pQuestData)
		{
			ACT1Q5_SpawnTowerChestMissiles(pQuestData, (D2Act1Quest5Strc*)pQuestData->pQuestDataEx);
			D2Act1Quest5Strc* pAct1Quest5 = (D2Act1Quest5Strc*)pQuestData->pQuestDataEx;
			if (pAct1Quest5->bCountessKilled && !pAct1Quest5->bDeathMissilesCreated)
			{
				EVENT_SetEvent(pQuestData->pGame, pUnit, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 10, 0, 0);
			}
		}
		break;
	}
	case OBJECT_ACT3SEWERLEVER:
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q2_KHALIMFLAIL);
		if (pQuestData)
		{
			D2Act3Quest2Strc* pAct3Quest2 = (D2Act3Quest2Strc*)pQuestData->pQuestDataEx;
			if (pAct3Quest2->bSewerStairsInitialized == 1)
			{
				D2UnitStrc* pFoundUnit = SUNIT_GetServerUnit(pQuestData->pGame, UNIT_OBJECT, pAct3Quest2->nSewerStairsObjectGUID);
				if (pFoundUnit)
				{
					UNITS_ChangeAnimMode(pFoundUnit, pAct3Quest2->nSewerStairsObjectMode);
					if (pAct3Quest2->nSewerStairsObjectMode != OBJMODE_OPENED)
					{
						pAct3Quest2->nSewerStairsObjectMode = OBJMODE_OPENED;
						D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pFoundUnit->dwClassId);
						if (pObjectsTxtRecord)
						{
							EVENT_SetEvent(pQuestData->pGame, pFoundUnit, UNITEVENTCALLBACK_ENDANIM, pQuestData->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);
						}
					}
				}
				else
				{
					pAct3Quest2->nSewerStairsObjectMode = OBJMODE_OPENED;
				}
			}
		}
		break;
	}
	case OBJECT_WIRTSBODY:
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q2_KHALIMFLAIL);
		if (pQuestData)
		{
			D2Act1Quest4Strc* pAct1Quest4 = (D2Act1Quest4Strc*)pQuestData->pQuestDataEx;
			if (!pAct1Quest4->bWirtsBodyInitialized)
			{
				pAct1Quest4->bWirtsBodyInitialized = 1;
				pAct1Quest4->nGoldPilesToDropFromWirtsBody = ITEMS_RollRandomNumber(QUESTS_GetGlobalSeed(pQuestData->pGame)) % 10 + 10;
				pUnit->dwDropItemCode = ' dlg';
			}

			int32_t nItemLevel = 0;
			if (pAct1Quest4->nGoldPilesToDropFromWirtsBody && D2GAME_DropItemAtUnit_6FC4FEC0(pQuestData->pGame, pUnit, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0))
			{
				--pAct1Quest4->nGoldPilesToDropFromWirtsBody;
				if (pAct1Quest4->nGoldPilesToDropFromWirtsBody > 0)
				{
					EVENT_SetEvent(pQuestData->pGame, pUnit, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 10, 0, 0);
				}
			}
		}
		break;
	}
	case OBJECT_CAINGIBBET:
	{
		ACT1Q4_SpawnCainInTristram(pGame, pUnit);
		break;
	}
	case OBJECT_CAINPORTAL:
	{
		const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
		if (DRLG_GetActNoFromLevelId(nLevelId))
		{
			if (nLevelId == LEVEL_HARROGATH || nLevelId >= 113)
			{
				D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q3_PRISONOFICE);
				if (pQuestData)
				{
					ACT5Q3_UpdateDrehyaPortalMode(pQuestData, pUnit);
				}
			}
			else
			{
				D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A5Q2_RESCUESOLDIERS);
				if (pQuestData)
				{
					ACT5Q2_UpdatePortalMode(pQuestData, pUnit);
				}
			}
		}
		else
		{
			D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q4_CAIN);
			if (pQuestData)
			{
				ACT1Q4_UpdatePortalInTown(pQuestData, pUnit);
			}
		}
		break;
	}
	case OBJECT_VILE_DOG_AFTERGLOW:
	{
		if (pRoom)
		{
			if (pUnit && pUnit->dwAnimMode == OBJMODE_OPERATING)
			{
				UNITS_ChangeAnimMode(pUnit, OBJMODE_OPENED);
			}

			const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
			if (nLevelId == LEVEL_SPIDERFOREST)
			{
				sub_6FC68D70(pGame, pUnit, MONSTER_VILECHILD1, 1, -1, 0);
			}
			else if (nLevelId == LEVEL_CHAOSSANCTUM)
			{
				ACT4Q2_SpawnSealBoss(pGame, pUnit);
			}
		}
		break;
	}
	case OBJECT_JERHYN_PLACEHOLDER2:
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A2Q4_HORAZONTOME);
		if (pQuestData)
		{
			ACT2Q4_InitializeHaremBlockerObject(pQuestData, pUnit);
		}
		break;
	}
	case OBJECT_MEPHISTOBRIDGE:
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q6_MEPHISTO);
		if (pQuestData)
		{
			D2Act3Quest6Strc* pAct3Quest6 = (D2Act3Quest6Strc*)pQuestData->pQuestDataEx;

			if (pAct3Quest6->nHellGatePortalObjectMode == OBJMODE_OPENED)
			{
				int32_t nAnimMode = OBJMODE_NEUTRAL;
				if (pUnit)
				{
					nAnimMode = pUnit->dwAnimMode;
					if (nAnimMode == OBJMODE_OPENED)
					{
						return;
					}
				}

				D2UnitStrc* pFoundUnit = UNITS_GetRoom(pUnit)->pUnitFirst;
				if (pFoundUnit)
				{
					while (pFoundUnit->dwUnitType || (uint32_t)D2Common_10399(pFoundUnit, pUnit) >= 18)
					{
						pFoundUnit = pFoundUnit->pRoomNext;
						if (!pFoundUnit)
						{
							EVENT_SetEvent(pQuestData->pGame, pUnit, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 24, 0, 0);
							return;
						}
					}

					if (nAnimMode == OBJMODE_NEUTRAL)
					{
						pAct3Quest6->nMephistoBridgeObjectMode = OBJMODE_OPENED;
						UNITS_ChangeAnimMode(pUnit, OBJMODE_OPERATING);
					}
					else if (nAnimMode == OBJMODE_OPERATING)
					{
						pAct3Quest6->nMephistoBridgeObjectMode = OBJMODE_OPENED;
						UNITS_ChangeAnimMode(pUnit, OBJMODE_OPENED);
						UNITS_FreeCollisionPath(pUnit);
					}
				}
			}
			EVENT_SetEvent(pQuestData->pGame, pUnit, UNITEVENTCALLBACK_QUESTFN, pQuestData->pGame->dwGameFrame + 24, 0, 0);
		}
		break;
	}
	}
}

//D2Game.0x6FC95CC0
void __fastcall OBJECTS_InitFunction23_LamEsenTomeStand(D2ObjInitFnStrc* pOp)
{
	if (!pOp || !pOp->pGame)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pOp->pGame, QUEST_A3Q1_LAMESENTOME);
	if (!pQuestData)
	{
		return;
	}

	if (pQuestData->bNotIntro)
	{
		UNITS_ChangeAnimMode(pOp->pObject, ((D2Act3Quest1Strc*)pQuestData->pQuestDataEx)->nTomeObjectMode);
	}
	else
	{
		UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
	}
}

//D2Game.0x6FC95D10
void __fastcall QUESTS_SetJadeFigurineBoss(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (DRLG_GetActNoFromLevelId(nLevelId) != ACT_III)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q4_GOLDENBIRD);

	if (!pUnit)
	{
		return;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
	if (pMonStatsTxtRecord && pQuestData && pQuestData->bNotIntro == 1 && pUnit->dwClassId != MONSTER_FETISH11 && !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_FLYING]))
	{
		ACT3Q4_UnitIterate_SetGoldenBirdBoss(pQuestData, pUnit);
	}
}

//D2Game.0x6FC95DB0
void __fastcall QUESTS_SetGoldenBirdSpawn(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2QuestChainStrc* pQuestChain = pUnit->pQuestEventList;
	if (!pQuestChain)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
	if (DRLG_GetActNoFromLevelId(nLevelId) != ACT_III)
	{
		return;
	}

	D2QuestDataStrc* pQuestData = pQuestChain->pQuestData;
	if (pQuestData->nQuest != 20 || !pQuestData->bActive)
	{
		return;
	}

	D2Act3Quest4Strc* pQuestDataEx = (D2Act3Quest4Strc*)pQuestData->pQuestDataEx;
	if (pQuestDataEx->unk0x02 != 1)
	{
		return;
	}

	int32_t nUnitGUID = -1;
	if (pUnit)
	{
		nUnitGUID = pUnit->dwUnitId;
	}

	if (pQuestDataEx->nBossGUID == nUnitGUID)
	{
		pQuestDataEx->unk0x02 = 0;
		pQuestDataEx->unk0x01 = 1;
	}
}

//D2Game.0x6FC95DF0
D2UnitStrc* __fastcall QUESTS_CreateItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint32_t dwCode, int32_t nLevel, uint8_t nQuality, int32_t bDroppable)
{
	int32_t nItemId = 0;
	if (!DATATBLS_GetItemRecordFromItemCode(dwCode, &nItemId))
	{
		return nullptr;
	}

	int32_t nItemLevel = ITEMS_GetItemLevelForNewItem(pPlayer, 0);
	if (nLevel)
	{
		nItemLevel = nLevel;
	}

	D2UnitStrc* pItem = D2GAME_CreateItemUnit_6FC501A0(pPlayer, nItemId, pGame, 4u, nQuality, 0, 1u, nItemLevel, 0, 0, 0);
	if (!pItem)
	{
		return nullptr;
	}

	const int32_t nMaxDurability = STATLIST_GetMaxDurabilityFromUnit(pItem);
	if (nMaxDurability > 0)
	{
		STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nMaxDurability, 0);
	}

	ITEMS_SetInvPage(pItem, INVPAGE_INVENTORY);

	if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pPlayer, pItem->dwUnitId, 0, 0, 1, 1, 0))
	{
		if (bDroppable)
		{
			D2CoordStrc pCoord = {};
			UNITS_GetCoords(pPlayer, &pCoord);
			D2ActiveRoomStrc* pRoom = nullptr;
			QUESTS_GetFreePosition(UNITS_GetRoom(pPlayer), &pCoord, 1, 0x3E01, &pRoom, 5);
			D2GAME_DropItem_6FC52260(pGame, nullptr, pItem, pRoom, pCoord.nX, pCoord.nY);
			return pItem;
		}

		ITEMS_RemoveFromAllPlayers(pGame, pItem);
		return nullptr;
	}

	if (!ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, __FILE__))
	{
		D2GAME_ITEMS_Identify_6FC49670(pGame, pPlayer, pItem);
	}

	return pItem;
}

//D2Game.0x6FC95F30
void __fastcall QUESTS_UpdateNPCIntros(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nAct)
{
	D2GSPacketSrv91 packet91 = {};

	D2PlrIntroStrc* pIntroFlags = UNITS_GetPlayerData(pUnit)->pPlayerIntro[pGame->nDifficulty];

	bool bUpdate = false;
	switch (nAct)
	{
	case ACT_I:
	{
		memset(packet91.pNPCFlags, -1, sizeof(packet91.pNPCFlags));
		for (int32_t i = 0; i < std::size(gpAct1_NPCIds); ++i)
		{
			if (PLRINTRO_GetNpcIntroFlag(pIntroFlags, gpAct1_NPCIds[i]))
			{
				bUpdate = true;
				packet91.pNPCFlags[i] = (uint16_t)gpAct1_NPCIds[i];
			}
		}
		break;
	}

	case ACT_II:
	{
		memset(packet91.pNPCFlags, -1, sizeof(packet91.pNPCFlags));
		for (int32_t i = 0; i < std::size(gpAct2_NPCIds); ++i)
		{
			if (PLRINTRO_GetNpcIntroFlag(pIntroFlags, gpAct2_NPCIds[i]))
			{
				bUpdate = true;
				packet91.pNPCFlags[i] = (uint16_t)gpAct2_NPCIds[i];
			}
		}
		break;
	}

	case ACT_III:
	{
		memset(packet91.pNPCFlags, -1, sizeof(packet91.pNPCFlags));
		for (int32_t i = 0; i < std::size(gpAct3_NPCIds); ++i)
		{
			if (PLRINTRO_GetNpcIntroFlag(pIntroFlags, gpAct3_NPCIds[i]))
			{
				bUpdate = true;
				packet91.pNPCFlags[i] = (uint16_t)gpAct3_NPCIds[i];
			}
		}
		break;
	}

	case ACT_V:
	{
		memset(packet91.pNPCFlags, -1, sizeof(packet91.pNPCFlags));
		for (int32_t i = 0; i < std::size(gpAct5_NPCIds); ++i)
		{
			if (PLRINTRO_GetNpcIntroFlag(pIntroFlags, gpAct5_NPCIds[i]))
			{
				bUpdate = true;
				packet91.pNPCFlags[i] = (uint16_t)gpAct5_NPCIds[i];
			}
		}
		break;
	}

	default:
		return;
	}

	if (!bUpdate)
	{
		return;
	}

	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
	if (!pClient)
	{
		return;
	}

	packet91.nHeader = 0x91;
	packet91.nAct = nAct;
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet91, sizeof(packet91));
}

//D2Game.0x6FC960C0
void __fastcall QUESTS_ActChange_HirelingChangeAct(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pVendor, uint32_t dwDestLvl, uint32_t unk)
{
	D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
	D2BitBufferStrc* pQuestFlags = pPlayerData->pQuestData[pGame->nDifficulty];

	if (!pVendor || pVendor->dwUnitType != UNIT_MONSTER)
	{
		return;
	}

	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);

	if (pVendor->dwClassId == MONSTER_WARRIV1)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1COMPLETED, QFLAG_REWARDGRANTED))
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1COMPLETED, QFLAG_REWARDGRANTED);
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A1COMPLETED, QFLAG_PRIMARYGOALDONE);

			D2GAME_SendPacket0x28_6FC3F2F0(pClient, 0x28u, 6u, 0, pQuestFlags, 0);

			if (pPlayerData->bBusy != 1)
			{
				pPlayerData->bBusy = 1;
				D2GAME_PACKETS_SendPacket0x61_6FC3F590(pClient, 2);
			}

			QUESTS_SetIntroFlags(pGame, pUnit, gpAct1_NPCIds, std::size(gpAct1_NPCIds));
		}

		if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_PRIMARYGOALDONE) == 1 && QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED) == 1)
		{
			D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A1Q6_ANDARIEL);
			if (pQuestData && pQuestData->bNotIntro)
			{
				D2QuestArgStrc pArgs = {};
				pArgs.pGame = pGame;
				pArgs.nEvent = QUESTEVENT_CHANGEDLEVEL;
				pArgs.nOldLevel = LEVEL_ROGUEENCAMPMENT;
				pArgs.nNewLevel = LEVEL_LUTGHOLEIN;
				pArgs.pPlayer = pUnit;

				if (pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL])
				{
					if (IsBadCodePtr((FARPROC)pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL]))
					{
						FOG_DisplayAssert("pQuestInfo->pCallback[QUESTCALLBACK_CHANGEDLEVEL]", __FILE__, __LINE__);
						exit(-1);
					}
					pQuestData->pfCallback[QUESTEVENT_CHANGEDLEVEL](pQuestData, &pArgs);
				}
			}
		}

		ACT1Q4_UpdateQuestStateOnActChange(pGame, pUnit);
	}
	else if (pVendor->dwClassId == MONSTER_MESHIF1)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2COMPLETED, QFLAG_REWARDGRANTED))
		{
			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED))
			{
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED);
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q2, QFLAG_PRIMARYGOALDONE);

				D2UnitStrc* pItem = ITEMS_FindQuestItem(pGame, pUnit, ' fsm');
				if (pItem)
				{
					QUESTS_DeleteItemEx(pGame, pUnit, pUnit->pInventory, pItem);
				}

				pItem = ITEMS_FindQuestItem(pGame, pUnit, ' piv');
				if (pItem)
				{
					QUESTS_DeleteItemEx(pGame, pUnit, pUnit->pInventory, pItem);
				}
			}

			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2COMPLETED, QFLAG_REWARDGRANTED);
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A2COMPLETED, QFLAG_PRIMARYGOALDONE);

			QUESTS_SetIntroFlags(pGame, pUnit, gpAct2_NPCIds, std::size(gpAct2_NPCIds));

			D2GAME_SendPacket0x28_6FC3F2F0(pClient, 0x28u, 6u, 0, pQuestFlags, 0);

			if (pPlayerData->bBusy != 1)
			{
				pPlayerData->bBusy = 1;
				D2GAME_PACKETS_SendPacket0x61_6FC3F590(pClient, 3);
			}
		}
	}
	else if (pVendor->dwClassId == MONSTER_TYRAEL2)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A4COMPLETED, QFLAG_REWARDGRANTED) && pGame->bExpansion)
		{
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4COMPLETED, QFLAG_REWARDGRANTED);
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A4COMPLETED, QFLAG_PRIMARYGOALDONE);

			QUESTS_SetIntroFlags(pGame, pUnit, gpAct2_NPCIds, std::size(gpAct2_NPCIds)); // TODO: Act2??

			D2GAME_SendPacket0x28_6FC3F2F0(pClient, 0x28u, 6u, 0, pQuestFlags, 0);

			if (pPlayerData->bBusy != 1)
			{
				pPlayerData->bBusy = 1;

				D2GSPacketSrv5D packet5D = {};
				packet5D.nHeader = 0x5D;
				packet5D.nQuestId = QUEST_A4Q2_DIABLO;
				packet5D.nFlags = 2;
				D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5D, sizeof(packet5D));

				D2GAME_PACKETS_SendPacket0x61_6FC3F590(pClient, 5);
			}
		}
	}
}

//D2Game.0x6FC964B0
void __fastcall QUESTS_DeleteItemEx(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);

	int32_t nAnimMode = 0;
	if (pItem)
	{
		nAnimMode = pItem->dwAnimMode;
	}

	if (nAnimMode == IMODE_STORED)
	{
		ITEMS_SetItemCell(pItem, ITEMS_GetInvPage(pItem));
		D2GAME_UpdateClientItem_6FC3E9D0(pClient, pPlayer, pItem, 0x20);
		D2GAME_RemoveItem_6FC471F0(pGame, pPlayer, pItem, 0);
	}
	else if (nAnimMode == IMODE_EQUIP)
	{
		int32_t a5 = 0;
		sub_6FC45B30(pGame, pPlayer, INVENTORY_GetItemsXPosition(pInventory, pItem), 1, &a5);
	}
	else if (nAnimMode == IMODE_ONCURSOR)
	{
		D2GAME_RemoveItemIfOnCursor_6FC49760(pGame, pPlayer, pItem);
		INVENTORY_SetCursorItem(pInventory, nullptr);
	}
}

//D2Game.0x6FC96580
void __fastcall QUESTS_HellPortalActChange(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nLevel, D2UnitStrc* pObject)
{
	D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
	D2BitBufferStrc* pQuestFlags = pPlayerData->pQuestData[pGame->nDifficulty];

	if (nLevel == LEVEL_DURANCEOFHATELEV3)
	{
		if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3COMPLETED, QFLAG_REWARDGRANTED))
		{
			D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);

			if (!QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED))
			{
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_REWARDGRANTED);
				QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q2, QFLAG_PRIMARYGOALDONE);

				D2UnitStrc* pItem = ITEMS_FindQuestItem(pGame, pPlayer, ' yeq');
				if (pItem)
				{
					QUESTS_DeleteItemEx(pGame, pPlayer, pPlayer->pInventory, pItem);
				}

				pItem = ITEMS_FindQuestItem(pGame, pPlayer, ' rhq');
				if (pItem)
				{
					QUESTS_DeleteItemEx(pGame, pPlayer, pPlayer->pInventory, pItem);
				}

				pItem = ITEMS_FindQuestItem(pGame, pPlayer, ' rbq');
				if (pItem)
				{
					QUESTS_DeleteItemEx(pGame, pPlayer, pPlayer->pInventory, pItem);
				}

				pItem = ITEMS_FindQuestItem(pGame, pPlayer, ' 1fq');
				if (pItem)
				{
					QUESTS_DeleteItemEx(pGame, pPlayer, pPlayer->pInventory, pItem);
				}

				pItem = ITEMS_FindQuestItem(pGame, pPlayer, ' 2fq');
				if (pItem)
				{
					QUESTS_DeleteItemEx(pGame, pPlayer, pPlayer->pInventory, pItem);
				}
			}

			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3COMPLETED, QFLAG_REWARDGRANTED);
			QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A3COMPLETED, QFLAG_PRIMARYGOALDONE);
			QUESTS_SetIntroFlags(pGame, pPlayer, gpAct3_NPCIds, std::size(gpAct3_NPCIds));

			if (pPlayerData->bBusy != 1)
			{
				pPlayerData->bBusy = 1;
				D2GAME_PACKETS_SendPacket0x61_6FC3F590(pClient, 4);
			}
		}
	}
	else
	{
		D2QuestDataStrc* pQuestData = QUESTS_GetQuestData(pGame, QUEST_A3Q6_MEPHISTO);
		if (pQuestData)
		{
			D2Act3Quest6Strc* pQuestDataEx = (D2Act3Quest6Strc*)pQuestData->pQuestDataEx;
			pQuestDataEx->nHellGatePortalObjectMode = OBJMODE_OPENED;
			pQuestDataEx->nMephistoBridgeObjectMode = OBJMODE_OPENED;

			if (pQuestDataEx->bHellGatePortalInitialized)
			{
				D2UnitStrc* pHellGatePortal = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nHellGatePortalGUID);
				if (pHellGatePortal)
				{
					UNITS_ChangeAnimMode(pHellGatePortal, OBJMODE_OPERATING);
				}
			}

			if (pQuestDataEx->bMephistoBridgeInitialized)
			{
				D2UnitStrc* pMephistoBridge = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pQuestDataEx->nMephistoBridgeGUID);
				if (pMephistoBridge)
				{
					UNITS_ChangeAnimMode(pMephistoBridge, OBJMODE_OPENED);
				}
			}
		}
	}
}

//D2Game.0x6FC967D0
void __fastcall QUESTS_SetIntroFlags(D2GameStrc* pGame, D2UnitStrc* pPlayer, const int32_t* pNPCs, int32_t nNPCs)
{
	D2PlayerDataStrc* pPlayerData = pPlayerData = UNITS_GetPlayerData(pPlayer);
	for (int32_t i = 0; i < nNPCs; ++i)
	{
		PLRINTRO_SetNpcIntroFlag(pPlayerData->pPlayerIntro[pGame->nDifficulty], pNPCs[i]);
	}
}

//D2Game.0x6FC96810
void __fastcall QUESTS_AddPlayerGUID(D2QuestGUIDStrc* pQuestGUID, int32_t dwGUID)
{
	if (pQuestGUID->nPlayerCount >= 32)
	{
		return;
	}

	if (QUESTS_QuickCheckPlayerGUID(pQuestGUID, dwGUID))
	{
		return;
	}

	pQuestGUID->nPlayerGUIDs[pQuestGUID->nPlayerCount] = dwGUID;
	++pQuestGUID->nPlayerCount;
}

//D2Game.0x6FC96840
int32_t __fastcall QUESTS_FastRemovePlayerGUID(D2QuestGUIDStrc* pGUIDs, int32_t dwGUID)
{
	for (int32_t i = 0; i < pGUIDs->nPlayerCount; ++i)
	{
		if (pGUIDs->nPlayerGUIDs[i] == dwGUID)
		{
			if (i != pGUIDs->nPlayerCount - 1)
			{
				pGUIDs->nPlayerGUIDs[i] = pGUIDs->nPlayerGUIDs[pGUIDs->nPlayerCount - 1];
			}

			--pGUIDs->nPlayerCount;
			return 1;
		}
	}

	return 0;
}

//D2Game.0x6FC96880
int32_t __fastcall QUESTS_QuickCheckPlayerGUID(D2QuestGUIDStrc* pGUIDs, int32_t dwGUID)
{
	for (int32_t i = 0; i < pGUIDs->nPlayerCount; ++i)
	{
		if (pGUIDs->nPlayerGUIDs[i] == dwGUID)
		{
			return 1;
		}
	}

	return 0;
}

//D2Game.0x6FC968B0
int32_t __fastcall QUESTS_CheckPlayerGUID(D2QuestDataStrc* pQuest, int32_t dwGUID)
{
	for (int32_t i = 0; i < pQuest->tPlayerGUIDs.nPlayerCount; ++i)
	{
		if (pQuest->tPlayerGUIDs.nPlayerGUIDs[i] == dwGUID)
		{
			return 1;
		}
	}

	return 0;
}

//D2Game.0x6FC968E0
void __fastcall QUESTS_ResetPlayerGUIDCount(D2QuestGUIDStrc* pGUIDs)
{
	pGUIDs->nPlayerCount = 0;
}

//D2Game.0x6FC968F0
void __fastcall QUESTS_QuickRemovePlayerGUID(D2QuestDataStrc* pQuest, D2QuestArgStrc* pQuestArg)
{
	const int32_t nUnitGUID = pQuestArg->pPlayer ? pQuestArg->pPlayer->dwUnitId : -1;

	for (int32_t i = 0; i < pQuest->tPlayerGUIDs.nPlayerCount; ++i)
	{
		if (pQuest->tPlayerGUIDs.nPlayerGUIDs[i] == nUnitGUID)
		{
			if (i != pQuest->tPlayerGUIDs.nPlayerCount - 1)
			{
				pQuest->tPlayerGUIDs.nPlayerGUIDs[i] = pQuest->tPlayerGUIDs.nPlayerGUIDs[pQuest->tPlayerGUIDs.nPlayerCount - 1];
			}
			--pQuest->tPlayerGUIDs.nPlayerCount;
			return;
		}
	}
}

//D2Game.0x6FC96940
void __fastcall QUESTS_GetFreePosition(D2ActiveRoomStrc* pRoom, D2CoordStrc* pCoord, uint32_t nSize, uint16_t fCollision, D2ActiveRoomStrc** ppRoom, int32_t nRadius)
{
	const uint32_t nCenterX = pCoord->nX - (nSize >> 1);
	const uint32_t nCenterY = pCoord->nY - (nSize >> 1);

	D2DrlgCoordsStrc drlgCoords = {};
	DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

	int32_t nX = nCenterX;

	int32_t j = 1;
	for (int32_t i = -1; i > -100; --i)
	{
		for (int32_t y = j; y > i; y -= 2)
		{
			const int32_t nY = y + nCenterY;

			D2ActiveRoomStrc* pRoom1 = pRoom;
			if (nY < drlgCoords.nSubtileY || nY >= drlgCoords.nSubtileY + drlgCoords.nSubtileHeight)
			{
				pRoom1 = D2GAME_GetRoom_6FC52070(pRoom, nX, nY);
			}

			if (pRoom1)
			{
				for (int32_t x = i; x < j; x += 2)
				{
					DUNGEON_GetRoomCoordinates(pRoom1, &drlgCoords);

					nX = x + nCenterX;

					D2ActiveRoomStrc* pRoom2 = pRoom1;
					if (nX < drlgCoords.nSubtileX || nX /*TODO: was nY instead, but most likely wrong*/ >= drlgCoords.nSubtileX + drlgCoords.nSubtileWidth)
					{
						pRoom2 = D2GAME_GetRoom_6FC52070(pRoom1, nX, nY);
					}

					if (pRoom2 && !COLLISION_CheckMaskWithSizeXY(pRoom2, nX, nY, 2 * nSize + 1, 2 * nSize + 1, fCollision)
						&& nX >= drlgCoords.nSubtileX && nX < drlgCoords.nSubtileX + drlgCoords.nSubtileWidth
						&& nY >= drlgCoords.nSubtileY && nY < drlgCoords.nSubtileY + drlgCoords.nSubtileHeight)
					{
						pCoord->nX = (nSize >> 1) + nX;
						pCoord->nY = (nSize >> 1) + nY;
						*ppRoom = D2GAME_GetRoom_6FC52070(pRoom, pCoord->nX, pCoord->nY);
						return;
					}
				}
			}
		}

		++j;
	}

	*ppRoom = nullptr;
}

//D2Game.0x6FC96C80
void __fastcall QUESTS_RemovePlayerGUID(D2QuestDataStrc* pQuest, D2QuestArgStrc* pArgs)
{
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pArgs->pPlayer)->pQuestData[pQuest->pGame->nDifficulty];
	if (QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, 1) != 1 || !QUESTRECORD_GetQuestState(pQuestFlags, pQuest->nQuest, 0) || !pQuest->tPlayerGUIDs.nPlayerCount)
	{
		return;
	}

	const int32_t nUnitGUID = pArgs->pPlayer ? pArgs->pPlayer->dwUnitId : -1;

	for (int32_t i = 0; i < pQuest->tPlayerGUIDs.nPlayerCount; ++i)
	{
		if (pQuest->tPlayerGUIDs.nPlayerGUIDs[i] == nUnitGUID)
		{
			if (i != pQuest->tPlayerGUIDs.nPlayerCount - 1)
			{
				pQuest->tPlayerGUIDs.nPlayerGUIDs[i] = pQuest->tPlayerGUIDs.nPlayerGUIDs[pQuest->tPlayerGUIDs.nPlayerCount - 1];
			}
			--pQuest->tPlayerGUIDs.nPlayerCount;
			return;
		}
	}
}

//D2Game.0x6FC96D20
void __fastcall QUESTS_UpdatePlayerFlags(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
	D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];

	D2GAME_SendPacket0x28_6FC3F2F0(pClient, 0x28u, 6u, 0, pQuestFlags, 0);
}

//D2Game.0x6FC96D60
void __fastcall QUESTS_GUIDUpdate(D2GameStrc* pGame, D2QuestGUIDStrc* pQuestGUID, int32_t nQuest, uint16_t wSoundCMD)
{
	for (int32_t i = 0; i < pQuestGUID->nPlayerCount; ++i)
	{
		D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pQuestGUID->nPlayerGUIDs[i]);
		if (pUnit)
		{
			D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
			if (QUESTRECORD_GetQuestState(pQuestFlags, nQuest, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pQuestFlags, nQuest, QFLAG_REWARDPENDING) != 1)
			{
				QUESTRECORD_SetQuestState(pQuestFlags, nQuest, QFLAG_PRIMARYGOALDONE);
				QUESTRECORD_SetQuestState(pQuestFlags, nQuest, QFLAG_REWARDPENDING);

				if (wSoundCMD)
				{
					SUNIT_AttachSound(pUnit, wSoundCMD, pUnit);
				}
			}
		}
	}
}

//D2Game.0x6FC96E20
void __fastcall QUESTS_SendScrollMessage(D2UnitStrc* pPlayer, D2UnitStrc* pUnit, uint16_t nMessage)
{
	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);

	D2GSPacketSrv27 packet27 = {};
	packet27.nHeader = 0x27;
	packet27.nUnitType = UNIT_OBJECT;
	packet27.dwUnitGUID = pUnit ? pUnit->dwUnitId : -1;
	packet27.pMessageList.nCount = 1;
	packet27.pMessageList.pMessages[0].nMenu = 0;
	packet27.pMessageList.pMessages[0].nStringId = nMessage;

	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet27, sizeof(packet27));
}

//D2Game.0x6FC96E80
void __fastcall QUESTS_TriggerFX(D2GameStrc* pGame, uint8_t a2)
{
	pGame->pQuestControl->unk0x20 = a2;
	SUNIT_IterateUnitsOfType(pGame, 0, 0, QUESTS_FXIterate);
}

//D2Game.0x6FC96EA0
int32_t __fastcall QUESTS_FXIterate(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg)
{
	QUESTS_UpdatePlayerFlags(pGame, pUnit);

	D2GSPacketSrv89 packet89 = {};

	packet89.nHeader = 0x89;
	packet89.nEvent = pGame->pQuestControl->unk0x20;
	D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet89, sizeof(packet89));

	return 0;
}

//D2Game.0x6FC96F10
void __fastcall QUESTS_NPCActivateSpeeches(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pTarget)
{
	D2TextHeaderStrc* pTextHeader = TEXT_AllocTextHeader(pGame->pMemoryPool);
	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);

	D2QuestArgStrc pArgs = {};
	pArgs.pGame = pClient ? pClient->pGame : nullptr;
	pArgs.nEvent = 0;
	pArgs.pTarget = pTarget;
	pArgs.pPlayer = pPlayer;
	pArgs.pTextControl = pTextHeader;

	QUESTS_EventCallback(&pArgs, true, true);

	D2GSPacketSrv27 packet27 = {};
	packet27.nHeader = 0x27;

	if (pTarget)
	{
		packet27.dwUnitGUID = pTarget->dwUnitId;
		packet27.nUnitType = (uint8_t)pTarget->dwUnitType;
	}
	else
	{
		packet27.dwUnitGUID = -1;
		packet27.nUnitType = 6;
	}

	TEXT_CreateMessageListFromTextHeader(pTextHeader, &packet27.pMessageList);

	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet27, sizeof(packet27));
	TEXT_FreeTextHeader(pTextHeader);

	D2QuestInfoStrc* pQuestInfo = pGame->pQuestControl;

	D2_ASSERT(pQuestInfo->bPickedSet);

	D2GSPacketSrv29 packet29 = {};
	packet29.nHeader = 0x29u;
	QUESTRECORD_CopyRecordToBuffer(pQuestInfo->pQuestFlags, packet29.pData, sizeof(packet29.pData), 0);
	sub_6FC3F370(pClient, &packet29);
}

//D2Game.0x6FC97020
int32_t __fastcall QUESTS_PortalCheck(D2GameStrc* pGame, D2CoordStrc* pCoord, int32_t nLevel, D2ActiveRoomStrc** ppRoom)
{
	if (nLevel == LEVEL_DURIELSLAIR)
	{
		return ACT2Q6_ActivateTomb(pGame, pCoord, ppRoom);
	}
	
	return 0;
}

//D2Game.0x6FC97040
int32_t __fastcall QUESTS_SetObjectSelection(D2ObjOperateFnStrc* pOp)
{
	D2UnitStrc* pObject = pOp->pObject;
	if (pObject && pObject->dwAnimMode != OBJMODE_NEUTRAL)
	{
		return 0;
	}

	//D2CoordStrc pCoord = {};
	//UNITS_GetCoords(pObject, &pCoord);

	D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
	if (!pObjectsTxtRecord->nMode[1])
	{
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
	}
	else
	{
		UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
		EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_ENDANIM, pOp->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);
	}

	pObject->dwFlags &= ~UNITFLAG_TARGETABLE;

	return 1;
}

//D2Game.0x6FC970E0
void __fastcall QUESTS_SendLogUpdate(D2UnitStrc* pUnit, uint8_t nQuestId)
{
	D2GSPacketSrv5D packet5D = {};

	packet5D.nHeader = 0x5D;
	packet5D.nQuestId = nQuestId;
	packet5D.nFlags = 2;
	packet5D.nState = 0;
	packet5D.field_4 = 0;

	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5D, sizeof(packet5D));
}

//D2Game.0x6FC97120
void __fastcall QUESTS_SendLogUpdateEx(D2UnitStrc* pPlayer, uint8_t nQuestId, uint8_t nAct)
{
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayer);
	if (pRoom)
	{
		const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
		if (nLevelId == 0 || DRLG_GetActNoFromLevelId(nLevelId) < nAct)
		{
			return;
		}
	}

	D2GSPacketSrv5D packet5D = {};
	packet5D.nHeader = 0x5D;
	packet5D.nQuestId = nQuestId;
	packet5D.nFlags = 0;
	packet5D.nState = 12;
	packet5D.field_4 = 0;
	D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
	D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5D, sizeof(packet5D));
}

//D2Game.0x6FC97190
D2UnitStrc* __fastcall QUESTS_SpawnCriticalMonster(D2GameStrc* pGame, int32_t nXpos, int32_t nYpos, D2ActiveRoomStrc* pRoom, int32_t bSpecialInit, int32_t nMonster)
{
	D2ActiveRoomStrc* ppRoom = pRoom;
	D2DrlgCoordsStrc drlgCoords = {};
	DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

	int32_t nX = nYpos;
	int32_t nY = nXpos;

	D2CoordStrc coord = {};
	coord.nX = nXpos;
	coord.nY = nYpos;

	int32_t nCounter = 0;
	while (1)
	{
		if (nY >= drlgCoords.nSubtileX && nX >= drlgCoords.nSubtileY && nY < drlgCoords.nSubtileX + drlgCoords.nSubtileWidth - 1 && nX < drlgCoords.nSubtileHeight + drlgCoords.nSubtileY - 1)
		{
			coord.nX = nY;
			break;
		}

		++nY;
		++nX;

		++nCounter;
		if (nCounter > 20)
		{
			coord.nX = nYpos;
			break;
		}
	}

	coord.nY = nX;

	QUESTS_GetFreePosition(pRoom, &coord, 2, 0x100u, &ppRoom, 6);

	D2ActiveRoomStrc* pTargetRoom = ppRoom;
	int32_t nTargetX = 0;
	int32_t nTargetY = 0;
	if (ppRoom)
	{
		nTargetX = coord.nX;
		nTargetY = coord.nY;
	}
	else
	{
		pTargetRoom = pRoom;
		nTargetX = nXpos;
		nTargetY = nYpos;
	}

	D2UnitStrc* pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pTargetRoom, nTargetX, nTargetY, nMonster, 1, -1, 0);

	if (!pMonster)
	{
		pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pTargetRoom, nTargetX, nTargetY, nMonster, 1, 5, 0);
	}

	if (pMonster)
	{
		if (bSpecialInit)
		{
			pMonster->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
		}
		return pMonster;
	}

	nCounter = 0;
	do
	{
		coord.nX = nTargetX + 1;
		coord.nY = nTargetY + 1;

		pTargetRoom = D2GAME_GetRoom_6FC52070(pTargetRoom, coord.nX, coord.nY);

		ppRoom = pTargetRoom;

		if (!pTargetRoom)
		{
			pTargetRoom = pRoom;
			ppRoom = pRoom;
			coord.nX = nXpos;
			coord.nY = nYpos;
		}

		QUESTS_GetFreePosition(pTargetRoom, &coord, 2, 0x100u, &ppRoom, 7);

		pTargetRoom = ppRoom;
		if (ppRoom)
		{
			nTargetY = coord.nY;
			nTargetX = coord.nX;
		}
		else
		{
			pTargetRoom = pRoom;
			nTargetX = nXpos;
			nTargetY = nYpos;
		}

		++nCounter;

		pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pTargetRoom, nTargetX, nTargetY, nMonster, 1, 10, 0);
		if (pMonster)
		{
			if (bSpecialInit)
			{
				pMonster->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
			}
			return pMonster;
		}
	}
	while (nCounter < 20);

	pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nXpos, nYpos, nMonster, 1, 15, 0);
	if (pMonster)
	{
		if (bSpecialInit)
		{
			pMonster->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
		}
	}
	return pMonster;
}

//D2Game.0x6FC973D0
void __fastcall QUESTS_MonsterSpawn(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	if (!pUnit)
	{
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (DUNGEON_GetLevelIdFromRoom(pRoom) < 108)
	{
		ACT3Q5_SpawnCouncil(pGame, pUnit);
	}
}

//D2Game.0x6FC97400
int32_t __fastcall QUESTS_LevelWarpCheck(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nSourceLevel, int32_t nTargetLevel)
{
	switch (nTargetLevel)
	{
	case LEVEL_DURIELSLAIR:
		return ACT2Q6_IsDurielsLairClosed(pGame, pPlayer);

	case LEVEL_DURANCEOFHATELEV1:
		return ACT3Q5_IsDuranceOfHateClosed(pGame, pPlayer);

	case LEVEL_GLACIALCAVESLEV1:
	case LEVEL_THEWORLDSTONEKEEPLEV1:
		if (nSourceLevel != LEVEL_ROCKYSUMMIT)
		{
			return 0;
		}
		return ACT5Q5_IsArreatSummitClosed(pGame, pPlayer, nTargetLevel);

	case LEVEL_THEWORLDSTONECHAMBER:
		return ACT5Q6_IsWorldStoneChamberClosed(pGame, pPlayer);

	default:
		return 0;
	}
}

//D2Game.0x6FC974B0
D2UnitStrc* __fastcall QUESTS_SpawnMonster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pPosition, int32_t nType, int32_t nIndex)
{
	D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), pPosition->nX, pPosition->nY);

	if (pRoom)
	{
		return D2GAME_SpawnPresetMonster_6FC66560(pGame, pRoom, DATATBLS_MapSuperUniqueId(nType, nIndex), pPosition->nX, pPosition->nY, 1);
	}

	return nullptr;
}

//D2Game.0x6FC975A0
void __fastcall QUESTS_MapPresetSpawn(D2GameStrc* pGame, D2UnitStrc* pUnit, D2MapAIStrc* pMapAI)
{
	if (!pUnit || pUnit->dwUnitType != UNIT_OBJECT)
	{
		return;
	}

	switch (pUnit->dwClassId)
	{
	case OBJECT_DREHYA_START_IN_TOWN:
		ACT5Q3_ChangeDrehyaMapAI(pGame, pUnit, pMapAI);
		break;

	case OBJECT_NIHLATHAK_START_IN_TOWN:
		ACT5Q3_ChangeNihlathakMapAI(pGame, pUnit, pMapAI);
		break;

	case OBJECT_LARZUK_STANDARD:
		ACT5Q1_ChangeLarzukMapAI(pGame, pUnit, pMapAI);
		break;
	}
}
