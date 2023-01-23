#include "UNIT/SUnitProxy.h"

#include <algorithm>

#include <Fog.h>
#include <D2BitManip.h>

#include <DataTbls/MonsterIds.h>
#include <DataTbls/LevelsIds.h>
#include "D2Dungeon.h"
#include "D2DataTbls.h"

#include "INVENTORY/InvMode.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT1/A1Q4.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitNpc.h"


#pragma warning(disable: 28159)


#pragma pack(push, 1)
struct D2UnitProxyArgStrc
{
    int32_t nLevelId;
    int32_t nCounter;
};

struct D2NpcInitStrc
{
    int32_t nNpcClassId;
    uint8_t bTrader;
    uint8_t nAct;
    uint8_t bForceVendor;
    uint8_t pad0x07;
};
#pragma pack(pop)


constexpr uint32_t MAX_NPC = 64;
constexpr uint32_t MAX_NPC_INVENTORY = 17;
uint8_t gbUnitProxyItemCacheInitialized[MAX_NPC_INVENTORY] = {};
D2UnitProxyStrc gUnitProxyItemCache[MAX_NPC_INVENTORY] = {};

int32_t dword_6FD4DD80;
int32_t dword_6FD4DD84;


//D2Game.0x6FCCB8A0
D2NpcRecordStrc* __fastcall SUNITPROXY_GetNpcRecordFromClassId(D2GameStrc* pGame, int32_t nNpcClassId, int32_t* pIndex)
{
    *pIndex = 0;

    for (int32_t nCounter = 0; nCounter < 64; ++nCounter)
    {
        if (pGame->pNpcControl->pFirstRecord[nCounter].nNPC == nNpcClassId)
        {
            *pIndex = nCounter;
            return &pGame->pNpcControl->pFirstRecord[nCounter];
        }
    }

    D2_ASSERT(nNpcClassId);

    return nullptr;
}

//D2Game.0x6FCCB910
D2NpcRecordStrc* __fastcall SUNITPROXY_GetNpcRecordFromUnit(D2GameStrc* pGame, D2UnitStrc* pNpc, int32_t* pIndex)
{
    return SUNITPROXY_GetNpcRecordFromClassId(pGame, pNpc ? pNpc->dwClassId : -1, pIndex);
}

//D2Game.0x6FCCB980
void __fastcall SUNITPROXY_AllocNpcEvent(D2UnitStrc* pNpc, D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t a4)
{
    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);
    if (pNpcRecord)
    {
        D2NpcEventStrc* pNpcEvent = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2NpcEventStrc);

        pNpcEvent->pNext = pNpcRecord->pEvent;
        pNpcRecord->pEvent = pNpcEvent;
        pNpcEvent->field_C = 1;
        pNpcEvent->pUnit = pPlayer;
        pNpcEvent->field_8 = a4;
    }
    else
    {
        ITEMS_RemoveFromAllPlayers(pGame, pPlayer);
    }
}

//D2Game.0x6FCCBA30
void __fastcall SUNITPROXY_InitializeNpcControl(D2GameStrc* pGame)
{
    constexpr D2NpcInitStrc dword_6FD3F640[] =
    {
        { MONSTER_AKARA,        1, ACT_I,   1, 0 },
        { MONSTER_CAIN1,        0, ACT_I,   0, 0 },
        { MONSTER_CHARSI,       1, ACT_I,   0, 0 },
        { MONSTER_KASHYA,       0, ACT_I,   0, 0 },
        { MONSTER_GHEED,        1, ACT_I,   0, 0 },
        { MONSTER_WARRIV1,      0, ACT_I,   0, 0 },
        { MONSTER_NAVI,         0, ACT_I,   0, 0 },
        { MONSTER_WARRIV2,      0, ACT_II,  0, 0 },
        { MONSTER_ATMA,         0, ACT_II,  0, 0 },
        { MONSTER_DROGNAN,      1, ACT_II,  1, 0 },
        { MONSTER_FARA,         1, ACT_II,  0, 0 },
        { MONSTER_LYSANDER,     1, ACT_II,  0, 0 },
        { MONSTER_GEGLASH,      0, ACT_II,  0, 0 },
        { MONSTER_MESHIF1,      0, ACT_II,  0, 0 },
        { MONSTER_JERHYN,       0, ACT_II,  0, 0 },
        { MONSTER_GREIZ,        0, ACT_II,  0, 0 },
        { MONSTER_ELZIX,        1, ACT_II,  0, 0 },
        { MONSTER_ACT2GUARD4,   0, ACT_II,  0, 0 },
        { MONSTER_ACT2GUARD5,   0, ACT_II,  0, 0 },
        { MONSTER_CAIN5,        0, ACT_I,   0, 0 },
        { MONSTER_CAIN2,        0, ACT_II,  0, 0 },
        { MONSTER_CAIN3,        0, ACT_III, 0, 0 },
        { MONSTER_TYRAEL1,      0, ACT_III, 0, 0 },
        { MONSTER_ASHEARA,      1, ACT_III, 0, 0 },
        { MONSTER_HRATLI,       1, ACT_III, 0, 0 },
        { MONSTER_ALKOR,        1, ACT_III, 0, 0 },
        { MONSTER_ORMUS,        1, ACT_III, 1, 0 },
        { MONSTER_MESHIF2,      0, ACT_III, 0, 0 },
        { MONSTER_CAIN4,        0, ACT_IV,  0, 0 },
        { MONSTER_IZUALGHOST,   0, ACT_IV,  0, 0 },
        { MONSTER_TYRAEL2,      0, ACT_IV,  0, 0 },
        { MONSTER_MALACHAI,     0, ACT_IV,  0, 0 },
        { MONSTER_HALBU,        1, ACT_IV,  1, 0 },
        { MONSTER_JAMELLA,      1, ACT_IV,  1, 0 },
        { MONSTER_NATALYA,      0, ACT_III, 0, 0 },
        { MONSTER_ACT2GUARD2,   0, ACT_II,  0, 0 },
        { MONSTER_MALAH,        1, ACT_V,   1, 0 },
        { MONSTER_DREHYA,       1, ACT_V,   0, 0 },
        { MONSTER_LARZUK,       1, ACT_V,   0, 0 },
        { MONSTER_NIHLATHAK,    1, ACT_V,   0, 0 },
        { MONSTER_QUAL_KEHK,    0, ACT_V,   0, 0 },
        { MONSTER_TYRAEL3,      0, ACT_V,   0, 0 },
        { MONSTER_CAIN6,        0, ACT_V,   0, 0 },
    };

    D2NpcRecordStrc* pRecordArray = (D2NpcRecordStrc*)D2_CALLOC_POOL(pGame->pMemoryPool, MAX_NPC * sizeof(D2NpcRecordStrc));

    D2NpcControlStrc* pNpcControl = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2NpcControlStrc);
    pNpcControl->pFirstRecord = 0;
    pNpcControl->nArraySize = 0;
    pNpcControl->unk0x10 = 0;

    SEED_InitSeed(&pNpcControl->pSeed);
    SEED_InitLowSeed(&pNpcControl->pSeed, ITEMS_RollRandomNumber(&pGame->pGameSeed));

    if (!dword_6FD4DD80)
    {
        DATATBLS_GetItemRecordFromItemCode(' vqc', &dword_6FD4DD80);
    }

    if (!dword_6FD4DD84)
    {
        DATATBLS_GetItemRecordFromItemCode(' vqa', &dword_6FD4DD84);
    }

    D2ItemDataTbl* pItemDataTbl = DATATBLS_GetItemDataTables();
    uint32_t nIndex = 0;

    for (int32_t i = 0; i < sgptDataTables->nMonStatsTxtRecordCount; ++i)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(i);
        if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT])
        {
            D2NpcRecordStrc* pNpcRecord = &pRecordArray[nIndex];

            ++pNpcControl->unk0x10;

            pNpcRecord->nNPC = i;
            pNpcRecord->bTrading = 0;
            pNpcRecord->pEvent = nullptr;
            pNpcRecord->npcTrade.bVendorInit = 0;
            pNpcRecord->npcTrade.bHireInit = 0;
            pNpcRecord->npcTrade.dwTicks = 0;
            pNpcRecord->npcTrade.bTrader = 0;
            pNpcRecord->pInventory = INVENTORY_AllocInventory(pGame->pMemoryPool, 0);
            pNpcRecord->pVendorChain = nullptr;
            pNpcRecord->pGamble = 0;
            pNpcRecord->bGambleInit = 0;

            D2_ASSERT(nIndex < MAX_NPC);

            ++pNpcControl->nArraySize;

            for (size_t j = 0; j < std::size(dword_6FD3F640); ++j)
            {
                if (dword_6FD3F640[j].nNpcClassId == i)
                {
                    pNpcRecord->npcTrade.nAct = dword_6FD3F640[j].nAct;
                    pNpcRecord->npcTrade.bTrader = dword_6FD3F640[j].bTrader;
                    pNpcRecord->npcTrade.bLevelRefresh = 0;
                    pNpcRecord->npcTrade.bInited = 0;
                    pNpcRecord->npcTrade.bForceVendor = dword_6FD3F640[j].bForceVendor;
                    pNpcRecord->npcTrade.bRefreshInventory = 0;

                    if (pNpcRecord->npcTrade.bTrader == 1)
                    {
                        switch (pNpcRecord->nNPC)
                        {
                        case MONSTER_AKARA:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 0);
                            break;
                        case MONSTER_GHEED:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 1);
                            pNpcRecord->npcTrade.bLevelRefresh = 1;
                            pNpcRecord->npcTrade.bInited = 1;
                            pNpcRecord->bGambleInit = 1;
                            break;
                        case MONSTER_CHARSI:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 2);
                            pNpcRecord->npcTrade.bLevelRefresh = 1;
                            pNpcRecord->npcTrade.bInited = 1;
                            break;
                        case MONSTER_FARA:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 3);
                            pNpcRecord->npcTrade.bLevelRefresh = 1;
                            pNpcRecord->npcTrade.bInited = 1;
                            break;
                        case MONSTER_LYSANDER:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 4);
                            break;
                        case MONSTER_DROGNAN:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 5);
                            break;
                        case MONSTER_HRATLI:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 6);
                            pNpcRecord->npcTrade.bLevelRefresh = 1;
                            pNpcRecord->npcTrade.bInited = 1;
                            break;
                        case MONSTER_ALKOR:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 7);
                            pNpcRecord->bGambleInit = 1;
                            break;
                        case MONSTER_ORMUS:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 8);
                            break;
                        case MONSTER_ELZIX:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 9);
                            pNpcRecord->bGambleInit = 1;
                            break;
                        case MONSTER_ASHEARA:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 10);
                            pNpcRecord->npcTrade.bLevelRefresh = 1;
                            pNpcRecord->npcTrade.bInited = 1;
                            break;
                        case MONSTER_HALBU:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 12);
                            pNpcRecord->npcTrade.bLevelRefresh = 1;
                            pNpcRecord->npcTrade.bInited = 1;
                            break;
                        case MONSTER_JAMELLA:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 13);
                            pNpcRecord->npcTrade.bLevelRefresh = 1;
                            pNpcRecord->npcTrade.bInited = 1;
                            pNpcRecord->bGambleInit = 1;
                            break;
                        case MONSTER_MALAH:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 14);
                            break;
                        case MONSTER_LARZUK:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 15);
                            pNpcRecord->npcTrade.bLevelRefresh = 1;
                            pNpcRecord->npcTrade.bInited = 1;
                            break;
                        case MONSTER_NIHLATHAK:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 15); // TODO: 15? The same as LARZUK?
                            pNpcRecord->bGambleInit = 1;
                            break;
                        case MONSTER_DREHYA:
                            SUNITPROXY_InitializeItemCache(pGame, &pNpcRecord->npcTrade.pProxy, pItemDataTbl, 16);
                            pNpcRecord->npcTrade.bLevelRefresh = 1;
                            pNpcRecord->npcTrade.bInited = 1;
                            pNpcRecord->bGambleInit = 1;
                            break;
                        default:
                            break;
                        }
                    }
                    else
                    {
                        pNpcRecord->npcTrade.pProxy.pItemCache = nullptr;
                        pNpcRecord->npcTrade.pProxy.pPermCache = nullptr;
                    }

                    break;
                }
            }

            ++nIndex;
        }
    }

    pNpcControl->pFirstRecord = pRecordArray;
    pGame->pNpcControl = pNpcControl;
}

//D2Game.0x6FCCBF50
void __fastcall SUNITPROXY_InitializeItemCache(D2GameStrc* pGame, D2UnitProxyStrc* pUnitProxy, void* pUnused, int32_t nNpcInventoryId)
{
    D2_ASSERT(nNpcInventoryId < MAX_NPC_INVENTORY);

    pUnitProxy->nItems = gUnitProxyItemCache[nNpcInventoryId].nItems;
    pUnitProxy->pItemCache = (D2ItemCacheStrc*)D2_ALLOC_POOL(pGame->pMemoryPool, sizeof(D2ItemCacheStrc) * gUnitProxyItemCache[nNpcInventoryId].nItems);
    memcpy(pUnitProxy->pItemCache, gUnitProxyItemCache[nNpcInventoryId].pItemCache, sizeof(D2ItemCacheStrc) * gUnitProxyItemCache[nNpcInventoryId].nItems);

    pUnitProxy->nPerms = gUnitProxyItemCache[nNpcInventoryId].nPerms;
    pUnitProxy->pPermCache = (uint32_t*)D2_ALLOC_POOL(pGame->pMemoryPool, sizeof(uint32_t) * gUnitProxyItemCache[nNpcInventoryId].nPerms);
    memcpy(pUnitProxy->pPermCache, gUnitProxyItemCache[nNpcInventoryId].pPermCache, sizeof(uint32_t) * gUnitProxyItemCache[nNpcInventoryId].nPerms);
}

//D2Game.0x6FCCC030
void __fastcall SUNITPROXY_FreeNpcControl(D2GameStrc* pGame)
{
    D2NpcControlStrc* pNpcControl = pGame->pNpcControl;
    for (int32_t i = 0; i < pNpcControl->nArraySize; ++i)
    {
        D2NpcRecordStrc* pRecord = &pNpcControl->pFirstRecord[i];

        D2NpcVendorChainStrc* pVendorChain = pRecord->pVendorChain;
        while (pVendorChain)
        {
            D2NpcVendorChainStrc* pNext = pVendorChain->pNext;
            D2_FREE_POOL(pGame->pMemoryPool, pVendorChain);
            pVendorChain = pNext;
        }
        pRecord->pVendorChain = nullptr;

        SUNITPROXY_ClearNpcRecordData(pGame, pRecord);

        if (pRecord->npcTrade.pProxy.pItemCache)
        {
            D2_FREE_POOL(pGame->pMemoryPool, pRecord->npcTrade.pProxy.pItemCache);
            pRecord->npcTrade.pProxy.pItemCache = nullptr;
        }

        if (pRecord->npcTrade.pProxy.pPermCache)
        {
            D2_FREE_POOL(pGame->pMemoryPool, pRecord->npcTrade.pProxy.pPermCache);
            pRecord->npcTrade.pProxy.pPermCache = nullptr;
        }

        if (pRecord->pMercData)
        {
            D2_FREE_POOL(pGame->pMemoryPool, pRecord->pMercData);
            pRecord->pMercData = nullptr;
        }

        pRecord->pEvent = nullptr;
    }

    D2_FREE_POOL(pGame->pMemoryPool, pNpcControl->pFirstRecord);
    D2_FREE_POOL(pGame->pMemoryPool, pNpcControl);
    pGame->pNpcControl = nullptr;
}

//D2Game.0x6FCCC140
void __fastcall SUNITPROXY_ClearNpcRecordData(D2GameStrc* pGame, D2NpcRecordStrc* pNpcRecord)
{
    if (pNpcRecord->bGambleInit)
    {
        D2NpcGambleStrc* pGamble = pNpcRecord->pGamble;
        while (pGamble)
        {
            D2NpcGambleStrc* pNextGamble = pGamble->pNext;
            if (pGamble->pInventory)
            {
                D2UnitStrc* pItem = INVENTORY_GetFirstItem(pGamble->pInventory);
                while (pItem)
                {
                    D2UnitStrc* pNextItem = INVENTORY_GetNextItem(pItem);
                    D2_ASSERT(INVENTORY_UnitIsItem(pItem));
                    //INVENTORY_GetItemNodePage(pItem);
                    SUNIT_RemoveUnit(pGame, pItem);
                    pItem = pNextItem;
                }
                INVENTORY_FreeInventory(pGamble->pInventory);
                pGamble->pInventory = nullptr;
            }
            D2_FREE_POOL(pGame->pMemoryPool, pGamble);
            pGamble = pNextGamble;
        }
        pNpcRecord->pGamble = nullptr;
    }

    if (pNpcRecord->pInventory)
    {
        D2UnitStrc* pItem = INVENTORY_GetFirstItem(pNpcRecord->pInventory);
        while (pItem)
        {
            D2UnitStrc* pNextItem = INVENTORY_GetNextItem(pItem);
            D2_ASSERT(INVENTORY_UnitIsItem(pItem));
            //INVENTORY_GetItemNodePage(pItem);
            SUNIT_RemoveUnit(pGame, pItem);
            pItem = pNextItem;
        }
    }
    INVENTORY_FreeInventory(pNpcRecord->pInventory);
    pNpcRecord->pInventory = nullptr;

    D2NpcEventStrc* pNpcEvent = pNpcRecord->pEvent;
    while (pNpcEvent)
    {
        D2NpcEventStrc* pNextEvent = pNpcEvent->pNext;
        if (pNpcEvent->field_C == 1 && pNpcEvent->pUnit)
        {
            ITEMS_RemoveFromAllPlayers(pGame, pNpcEvent->pUnit);
        }
        D2_FREE_POOL(pGame->pMemoryPool, pNpcEvent);
        pNpcEvent = pNextEvent;
    }
    pNpcRecord->pEvent = nullptr;
}

//D2Game.0x6FCCC2E0
void __fastcall SUNITPROXY_UpdateNpcsOnActChange(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCurrentLevelId, int32_t nDestLevelId)
{
    D2UnitProxyArgStrc unitProxyArg = {};
    unitProxyArg.nLevelId = nCurrentLevelId;
    unitProxyArg.nCounter = 0;

    switch (nCurrentLevelId)
    {
    case LEVEL_ROGUEENCAMPMENT:
        SUNIT_IterateLivingPlayers(pGame, SUNITPROXY_CountPlayersInLevel, &unitProxyArg);
        SUNITPROXY_UpdateVendorInventory(pGame, pUnit, ACT_I, unitProxyArg.nCounter == 0);
        break;

    case LEVEL_LUTGHOLEIN:
        SUNIT_IterateLivingPlayers(pGame, SUNITPROXY_CountPlayersInLevel, &unitProxyArg);
        SUNITPROXY_UpdateVendorInventory(pGame, pUnit, ACT_II, unitProxyArg.nCounter == 0);
        break;

    case LEVEL_KURASTDOCKTOWN:
        SUNIT_IterateLivingPlayers(pGame, SUNITPROXY_CountPlayersInLevel, &unitProxyArg);
        SUNITPROXY_UpdateVendorInventory(pGame, pUnit, ACT_III, unitProxyArg.nCounter == 0);
        break;

    case LEVEL_THEPANDEMONIUMFORTRESS:
        SUNIT_IterateLivingPlayers(pGame, SUNITPROXY_CountPlayersInLevel, &unitProxyArg);
        SUNITPROXY_UpdateVendorInventory(pGame, pUnit, ACT_IV, unitProxyArg.nCounter == 0);
        break;

    case LEVEL_HARROGATH:
        SUNIT_IterateLivingPlayers(pGame, SUNITPROXY_CountPlayersInLevel, &unitProxyArg);
        SUNITPROXY_UpdateVendorInventory(pGame, pUnit, ACT_V, unitProxyArg.nCounter == 0);
        break;

    default:
        break;
    }

    switch (nDestLevelId)
    {
    case LEVEL_ROGUEENCAMPMENT:
        QUESTS_UpdateNPCIntros(pGame, pUnit, 0);
        break;

    case LEVEL_LUTGHOLEIN:
        QUESTS_UpdateNPCIntros(pGame, pUnit, 1);
        ACT1Q4_UpdateQuestStateOnActChange(pGame, pUnit);
        break;

    case LEVEL_KURASTDOCKTOWN:
        QUESTS_UpdateNPCIntros(pGame, pUnit, 2);
        break;

    case LEVEL_HARROGATH:
        QUESTS_UpdateNPCIntros(pGame, pUnit, 4);
        break;

    default:
        return;
    }
}

//D2Game.0x6FCCC540
void __fastcall SUNITPROXY_UpdateVendorInventory(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nAct, int32_t bNoMorePlayersInLevel)
{
    const uint32_t nTickCount = GetTickCount();
    
    for (int32_t i = 0; i < pGame->pNpcControl->nArraySize; ++i)
    {
        D2NpcRecordStrc* pNpcRecord = &pGame->pNpcControl->pFirstRecord[i];

        if (pNpcRecord && pNpcRecord->npcTrade.nAct == nAct && pNpcRecord->npcTrade.bTrader == 1 && pNpcRecord->bTrading == 1 && pNpcRecord->npcTrade.bVendorInit == 1)
        {
            if (bNoMorePlayersInLevel)
            {
                D2UnitStrc* pNpc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pNpcRecord->npcTrade.dwNPCGUID);
                if (pNpc && pNpc->dwClassId == pNpcRecord->nNPC)
                {
                    D2MonsterInteractStrc* pMonInteract = nullptr;
                    if (pNpc->dwUnitType == UNIT_MONSTER && pNpc->pMonsterData)
                    {
                        pMonInteract = pNpc->pMonsterData->pMonInteract;
                    }

                    if (!MONSTERAI_GetInteractUnitCount(pMonInteract))
                    {
                        SUNITPROXY_ClearNpcRecordData(pGame, pNpcRecord);
                        pNpcRecord->npcTrade.bVendorInit = 0;
                        pNpcRecord->npcTrade.dwTicks = 0;
                        pNpcRecord->pInventory = INVENTORY_AllocInventory(pGame->pMemoryPool, nullptr);
                        pNpc->pInventory = pNpcRecord->pInventory;
                    }
                    else
                    {
                        pNpcRecord->npcTrade.bRefreshInventory = 1;
                    }
                }
                else
                {
                    SUNITPROXY_ClearNpcRecordData(pGame, pNpcRecord);
                    pNpcRecord->npcTrade.bVendorInit = 0;
                    pNpcRecord->npcTrade.dwTicks = 0;
                    pNpcRecord->pInventory = INVENTORY_AllocInventory(pGame->pMemoryPool, nullptr);
                }
            }
            else
            {
                if (nTickCount > pNpcRecord->npcTrade.dwTicks + 240000)
                {
                    pNpcRecord->npcTrade.dwTicks = nTickCount;
                    pNpcRecord->npcTrade.bRefreshInventory = 1;
                }
            }
        }
    }
}

//D2Game.0x6FCCC690
void __fastcall SUNITPROXY_CountPlayersInLevel(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg)
{
    D2UnitProxyArgStrc* pUnitProxyArg = (D2UnitProxyArgStrc*)pArg;

    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom && pUnitProxyArg->nLevelId == DUNGEON_GetLevelIdFromRoom(pRoom))
    {
        ++pUnitProxyArg->nCounter;
    }
}

//D2Game.0x6FCCC6B0
void __fastcall SUNITPROXY_OnClientRemovedFromGame(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (pGame->nGameType == 3)
    {
        return;
    }

    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return;
    }

    uint8_t nAct = 0;

    D2UnitProxyArgStrc unitProxyArg = {};
    unitProxyArg.nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    unitProxyArg.nCounter = 0;

    switch (unitProxyArg.nLevelId)
    {
    case LEVEL_ROGUEENCAMPMENT:
        nAct = ACT_I;
        break;

    case LEVEL_LUTGHOLEIN:
        nAct = ACT_II;
        break;

    case LEVEL_KURASTDOCKTOWN:
        nAct = ACT_III;
        break;

    case LEVEL_THEPANDEMONIUMFORTRESS:
        nAct = ACT_IV;
        break;

    default:
        return;
    }

    SUNIT_IterateLivingPlayers(pGame, SUNITPROXY_CountPlayersInLevel, &unitProxyArg);
    --unitProxyArg.nCounter;
    SUNITPROXY_UpdateVendorInventory(pGame, pUnit, nAct, unitProxyArg.nCounter == 0);
}

//D2Game.0x6FCCC7C0
void __fastcall SUNITPROXY_InitializeNpcEventChain(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    //GetTickCount();
    for (int32_t i = 0; i < pGame->pNpcControl->nArraySize; ++i)
    {
        D2NpcRecordStrc* pRecord = &pGame->pNpcControl->pFirstRecord[i];

        if (pRecord->npcTrade.bTrader == 1 && pRecord->npcTrade.bVendorInit == 1 && pRecord->bTrading == 1)
        {
            D2NpcEventStrc* pEvent = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2NpcEventStrc);
            pEvent->field_C = 0;
            pEvent->pNext = pRecord->pEvent;
            pEvent->pUnit = pUnit;
            pEvent->field_8 = 1;
            pEvent->field_4 = 4;
            pRecord->pEvent = pEvent;
        }
    }
}

//D2Game.0x6FCCC860
D2InventoryStrc* __fastcall SUNITPROXY_GetNpcInventory(D2GameStrc* pGame, int32_t nNpc)
{
    int32_t nUnused = 0;
    return SUNITPROXY_GetNpcRecordFromClassId(pGame, nNpc, &nUnused)->pInventory;
}

//D2Game.0x6FCCC8B0
D2SeedStrc* __fastcall SUNITPROXY_GetSeedFromNpcControl(D2GameStrc* pGame)
{
    return &pGame->pNpcControl->pSeed;
}

//D2Game.0x6FCCC8C0
void __fastcall SUNITPROXY_FreeVendorChain(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc)
{
    int32_t nNpcId = -1;
    if (pNpc)
    {
        nNpcId = pNpc->dwClassId;
    }

    for (int32_t nCounter = 0; nCounter < 64; ++nCounter)
    {
        D2NpcRecordStrc* pNpcRecord = &pGame->pNpcControl->pFirstRecord[nCounter];
        if (pNpcRecord && pNpcRecord->nNPC == nNpcId)
        {
            int32_t nPlayerGUID = -1;
            if (pPlayer)
            {
                nPlayerGUID = pPlayer->dwUnitId;
            }

            if (pNpcRecord->bGambleInit)
            {
                D2NpcGambleStrc* pPreviousGamble = nullptr;
                for (D2NpcGambleStrc* pGamble = pNpcRecord->pGamble; pGamble; pGamble = pGamble->pNext)
                {
                    if (pGamble->dwGUID == nPlayerGUID)
                    {
                        if (pPreviousGamble)
                        {
                            pPreviousGamble->pNext = pGamble->pNext;
                        }
                        else
                        {
                            pNpcRecord->pGamble = pGamble->pNext;
                        }

                        if (pGamble->pInventory)
                        {
                            D2UnitStrc* pItem = INVENTORY_GetFirstItem(pGamble->pInventory);
                            while (pItem)
                            {
                                D2_ASSERT(INVENTORY_UnitIsItem(pItem));

                                D2UnitStrc* pNext = INVENTORY_GetNextItem(pItem);
                                //INVENTORY_GetItemNodePage(pItem);
                                SUNIT_RemoveUnit(pGame, pItem);
                                pItem = pNext;
                            }

                            INVENTORY_FreeInventory(pGamble->pInventory);
                            pGamble->pInventory = nullptr;
                        }

                        D2_FREE_POOL(pGame->pMemoryPool, pGamble);
                        break;
                    }

                    pPreviousGamble = pGamble;
                }
            }

            
            D2NpcVendorChainStrc* pPreviousVendorChain = nullptr;
            for (D2NpcVendorChainStrc* pVendorChain = pNpcRecord->pVendorChain; pVendorChain; pVendorChain = pVendorChain->pNext)
            {
                if (pVendorChain->dwGUID == nPlayerGUID)
                {
                    if (pPreviousVendorChain)
                    {
                        pPreviousVendorChain->pNext = pVendorChain->pNext;
                    }
                    else
                    {
                        pNpcRecord->pVendorChain = pVendorChain->pNext;
                    }
                    D2_FREE_POOL(pGame->pMemoryPool, pVendorChain);
                    return;
                }
                pPreviousVendorChain = pVendorChain;
            }
            return;
        }
    }

    if (!nNpcId)
    {
        FOG_DisplayAssert("mc != 0", __FILE__, __LINE__);
        exit(-1);
    }
}

//D2Game.0x6FCCCA70
void __fastcall SUNITPROXY_UpdateGambleInventory(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pPlayer, D2ClientStrc* pClient)
{
    D2GAME_INVMODE_First_6FC40FB0(pGame, pNpc, pClient, nullptr);

    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);
    if (pNpcRecord && pNpcRecord->bGambleInit)
    {
        const int32_t nPlayerGUID = pPlayer ? pPlayer->dwUnitId : -1;
        for (D2NpcGambleStrc* pGamble = pNpcRecord->pGamble; pGamble; pGamble = pGamble->pNext)
        {
            if (pGamble->dwGUID == nPlayerGUID)
            {
                if (pGamble->pInventory)
                {
                    D2GAME_INVMODE_First_6FC40FB0(pGame, pNpc, pClient, pGamble->pInventory);
                }
                return;
            }
        }
    }
}

//D2Game.0x6FCCCB20
D2InventoryStrc* __fastcall SUNITPROXY_GetGambleInventory(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc)
{
    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);

    if (!pNpcRecord || !pNpcRecord->bGambleInit)
    {
        return nullptr;
    }

    const int32_t nPlayerGUID = pPlayer ? pPlayer->dwUnitId : -1;
    for (D2NpcGambleStrc* pGamble = pNpcRecord->pGamble; pGamble; pGamble = pGamble->pNext)
    {
        if (pGamble->dwGUID == nPlayerGUID)
        {
            return pGamble->pInventory;
        }
    }

    return nullptr;
}

//D2Game.0x6FCCCBB0
D2NpcVendorChainStrc* __fastcall SUNITPROXY_GetVendorChain(D2GameStrc* pGame, D2NpcRecordStrc* pNpcRecord, D2UnitStrc* pNpc)
{
    const int32_t nNpcGUID = pNpc ? pNpc->dwUnitId : -1;

    for (D2NpcVendorChainStrc* pVendorChain = pNpcRecord->pVendorChain; pVendorChain; pVendorChain = pVendorChain->pNext)
    {
        if (pVendorChain->dwGUID == nNpcGUID)
        {
            return pVendorChain;
        }
    }

    D2NpcVendorChainStrc* pVendorChain = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2NpcVendorChainStrc);
    pVendorChain->dwGUID = nNpcGUID;
    pVendorChain->pNext = pNpcRecord->pVendorChain;
    pNpcRecord->pVendorChain = pVendorChain;
    return pVendorChain;
}

//D2Game.0x6FCCCC00
void __fastcall SUNITPROXY_AllocNpcInventory(D2GameStrc* pGame, D2NpcRecordStrc* pNpcRecord, D2UnitStrc* pNpc)
{
    SUNITPROXY_ClearNpcRecordData(pGame, pNpcRecord);

    pNpcRecord->npcTrade.bVendorInit = 0;
    pNpcRecord->npcTrade.dwTicks = 0;
    pNpcRecord->pInventory = INVENTORY_AllocInventory(pGame->pMemoryPool, 0);

    if (pNpc)
    {
        pNpc->pInventory = pNpcRecord->pInventory;
    }
}

//D2Game.0x6FCCCC40
void __fastcall SUNITPROXY_FillGlobalItemCache()
{
    for (int32_t i = 0; i < sgptDataTables->nMonStatsTxtRecordCount; ++i)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(i);
        if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT])
        {
            switch (i)
            {
            case MONSTER_AKARA:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(0);
                break;

            case MONSTER_GHEED:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(1);
                break;

            case MONSTER_CHARSI:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(2);
                break;

            case MONSTER_FARA:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(3);
                break;

            case MONSTER_LYSANDER:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(4);
                break;

            case MONSTER_DROGNAN:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(5);
                break;

            case MONSTER_HRATLI:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(6);
                break;

            case MONSTER_ALKOR:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(7);
                break;

            case MONSTER_ORMUS:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(8);
                break;

            case MONSTER_ELZIX:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(9);
                break;

            case MONSTER_ASHEARA:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(10);
                break;

            case MONSTER_HALBU:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(12);
                break;

            case MONSTER_JAMELLA:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(13);
                break;

            case MONSTER_MALAH:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(14);
                break;

            case MONSTER_LARZUK:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(15);
                break;

            case MONSTER_DREHYA:
            case MONSTER_NIHLATHAK:
                SUNITPROXY_FillIGlobaltemCacheRecordForNpc(16);
                break;

            default:
                break;
            }
        }
    }
}

//D2Game.0x6FCCCED0
void __fastcall SUNITPROXY_FillIGlobaltemCacheRecordForNpc(int32_t nNpcId)
{
    D2ItemDataTbl* pItemDataTbl = DATATBLS_GetItemDataTables();

    if (gbUnitProxyItemCacheInitialized[nNpcId])
    {
        return;
    }

    gbUnitProxyItemCacheInitialized[nNpcId] = 1;

    int32_t nItemCount = 0;
    int32_t nPermStoreItemCount = 0;

    for (int32_t i = 0; i < pItemDataTbl->nItemsTxtRecordCount; ++i)
    {
        D2ItemsTxt* pItemsTxtRecord = &pItemDataTbl->pItemsTxt[i];
        if (pItemsTxtRecord->nSpawnable && (pItemsTxtRecord->nVendorMax[nNpcId] || pItemsTxtRecord->nVendorMagicMax[nNpcId]))
        {
            if (pItemsTxtRecord->nPermStoreItem)
            {
                ++nPermStoreItemCount;
            }
            else
            {
                ++nItemCount;
            }
        }
    }

    gUnitProxyItemCache[nNpcId].pItemCache = (D2ItemCacheStrc*)D2_CALLOC_POOL(nullptr, sizeof(D2ItemCacheStrc) * nItemCount);
    gUnitProxyItemCache[nNpcId].nItems = nItemCount;

    gUnitProxyItemCache[nNpcId].pPermCache = (uint32_t*)D2_CALLOC_POOL(nullptr, sizeof(uint32_t) * nPermStoreItemCount);
    gUnitProxyItemCache[nNpcId].nPerms = nPermStoreItemCount;

    int32_t nItemCacheIndex = 0;
    int32_t nPermCacheIndex = 0;
    for (int32_t i = 0; i < pItemDataTbl->nItemsTxtRecordCount; ++i)
    {
        D2ItemsTxt* pItemsTxtRecord = &pItemDataTbl->pItemsTxt[i];
        if (pItemsTxtRecord->nSpawnable && (pItemsTxtRecord->nVendorMax[nNpcId] || pItemsTxtRecord->nVendorMagicMax[nNpcId]))
        {
            if (pItemsTxtRecord->nPermStoreItem)
            {
                gUnitProxyItemCache[nNpcId].pPermCache[nPermCacheIndex] = pItemsTxtRecord->dwCode;
                ++nPermCacheIndex;
            }
            else
            {
                gUnitProxyItemCache[nNpcId].pItemCache[nItemCacheIndex].nMin = pItemsTxtRecord->nVendorMin[nNpcId];
                gUnitProxyItemCache[nNpcId].pItemCache[nItemCacheIndex].nMax = pItemsTxtRecord->nVendorMax[nNpcId];
                gUnitProxyItemCache[nNpcId].pItemCache[nItemCacheIndex].nMagicMin = pItemsTxtRecord->nVendorMagicMin[nNpcId];
                gUnitProxyItemCache[nNpcId].pItemCache[nItemCacheIndex].nMagicMax = pItemsTxtRecord->nVendorMagicMax[nNpcId];
                gUnitProxyItemCache[nNpcId].pItemCache[nItemCacheIndex].dwCode = pItemsTxtRecord->dwCode;
                gUnitProxyItemCache[nNpcId].pItemCache[nItemCacheIndex].nMagicLevel = pItemsTxtRecord->nVendorMagicLvl[nNpcId];
                ++nItemCacheIndex;
            }
        }
    }
}

//D2Game.0x6FCCD120
void __fastcall SUNITPROXY_ClearGlobalItemCache()
{
    for (int32_t i = 0; i < MAX_NPC_INVENTORY; ++i)
    {
        if (gbUnitProxyItemCacheInitialized[i] == 1)
        {
            gbUnitProxyItemCacheInitialized[i] = 0;

            if (gUnitProxyItemCache[i].pItemCache)
            {
                D2_FREE_POOL(nullptr, gUnitProxyItemCache[i].pItemCache);
                gUnitProxyItemCache[i].pItemCache = nullptr;
            }

            if (gUnitProxyItemCache[i].pPermCache)
            {
                D2_FREE_POOL(nullptr, gUnitProxyItemCache[i].pPermCache);
                gUnitProxyItemCache[i].pPermCache = nullptr;
            }
        }
    }
}

//D2Game.0x6FCCD190
void __fastcall SUNITPROXY_FreeNpcGamble(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pPlayer)
{
    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);

    if (pNpcRecord && pNpcRecord->bGambleInit)
    {
        const int32_t nPlayerGUID = pPlayer ? pPlayer->dwUnitId : -1;
        D2NpcGambleStrc* pPreviousGamble = nullptr;
        for (D2NpcGambleStrc* pGamble = pNpcRecord->pGamble; pGamble; pGamble = pGamble->pNext)
        {
            if (pGamble->dwGUID == nPlayerGUID)
            {
                if (pPreviousGamble)
                {
                    pPreviousGamble->pNext = pGamble->pNext;
                }
                else
                {
                    pNpcRecord->pGamble = pGamble->pNext;
                }

                if (pGamble->pInventory)
                {
                    D2UnitStrc* pInventoryItem = INVENTORY_GetFirstItem(pGamble->pInventory);
                    while (pInventoryItem)
                    {
                        D2UnitStrc* pNextItem = INVENTORY_GetNextItem(pInventoryItem);
                        D2_ASSERT(INVENTORY_UnitIsItem(pInventoryItem));
                        //INVENTORY_GetItemNodePage(pInventoryItem);
                        SUNIT_RemoveUnit(pGame, pInventoryItem);
                        pInventoryItem = pNextItem;
                    }
                    INVENTORY_FreeInventory(pGamble->pInventory);
                    pGamble->pInventory = nullptr;
                }

                D2_FREE_POOL(pGame->pMemoryPool, pGamble);
                return;
            }

            pPreviousGamble = pGamble;
        }
    }
}
