#include "UNIT/SUnitNpc.h"

#include <algorithm>

#include <Fog.h>
#include <D2BitManip.h>

#include <D2DataTbls.h>
#include <D2StatList.h>
#include <D2Skills.h>
#include <D2Monsters.h>
#include <D2Dungeon.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/LevelsIds.h>
#include <D2QuestRecord.h>
#include <D2States.h>
#include <D2Items.h>
#include <D2Waypoints.h>


#include "AI/AiGeneral.h"
#include "GAME/Clients.h"
#include "GAME/SCmd.h"
#include "GAME/Targets.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterSpawn.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlrMsg.h"
#include "PLAYER/PlrTrade.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT1/A1Q3.h"
#include "QUESTS/ACT5/A5Q1.h"
#include "QUESTS/ACT5/A5Q4.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitProxy.h"


#pragma warning(disable: 28159)


int32_t gnNpcGUID_6FD4DC48;

//D2Game.0x6FCC67D0
void __fastcall D2GAME_NPC_FirstFn_6FCC67D0(D2GameStrc* pGame, int32_t nVendorId, D2NpcRecordStrc* pNpcRecord)
{
    if (pNpcRecord->npcTrade.bHireInit)
    {
        return;
    }

    pNpcRecord->npcTrade.bHireInit = 1;

    if (pNpcRecord->pMercData)
    {
        return;
    }

    pNpcRecord->pMercData = (D2MercDataStrc*)D2_CALLOC_POOL(pGame->pMemoryPool, 0x450u);

    D2SeedStrc* pSeed = SUNITPROXY_GetSeedFromNpcControl(pGame);
    D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty(pGame->bExpansion, nVendorId, 0, 0);
    D2_ASSERT(pHirelingTxtRecord);

    const int32_t nAlternatives = pHirelingTxtRecord->wNameLast - pHirelingTxtRecord->wNameFirst + 1;
    for (int32_t i = 0; pHirelingTxtRecord->wNameFirst + i <= pHirelingTxtRecord->wNameLast; ++i)
    {
        if (i >= nAlternatives)
        {
            break;
        }

        pNpcRecord->pMercData[i].nMercName = pHirelingTxtRecord->wNameFirst + i;
        pNpcRecord->pMercData[i].dwSeed = ITEMS_RollRandomNumber(pSeed);
        pNpcRecord->pMercData[i].bHired = 0;
        pNpcRecord->pMercData[i].bAvailable = 0;
    }

    for (int32_t i = 0; i < 10; ++i)
    {
        const int32_t nStartIndex = ITEMS_RollLimitedRandomNumber(pSeed, nAlternatives);
        int32_t nCounter = nStartIndex;
        while (1)
        {
            if (!pNpcRecord->pMercData[nCounter].bAvailable && !pNpcRecord->pMercData[nCounter].bHired)
            {
                pNpcRecord->pMercData[nCounter].bAvailable = 1;
                break;
            }

            ++nCounter;

            if (nCounter >= nAlternatives)
            {
                nCounter = 0;
            }

            if (nCounter == nStartIndex)
            {
                return;
            }
        }
    }
}

//D2Game.0x6FCC6970
void __fastcall D2GAME_NPC_RepairItem_6FCC6970(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pPlayer)
{
    if (ITEMS_IsRepairable(pItem))
    {
        if (ITEMS_CheckIfThrowable(pItem) && ITEMS_CheckIfStackable(pItem))
        {
            const int32_t nMaxStack = ITEMS_GetTotalMaxStack(pItem);
            STATLIST_SetUnitStat(pItem, STAT_QUANTITY, nMaxStack, 0);

            if (pPlayer)
            {
                D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
                D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(pClient, pItem, 1, STAT_QUANTITY, nMaxStack, 0);
            }
        }

        D2GAME_RechargeItem_6FC4BD50(pGame, pPlayer, pItem);

        if (ITEMS_CheckItemFlag(pItem, IFLAG_BROKEN, __LINE__, __FILE__))
        {
            D2GAME_RepairBrokenItem_6FC4B630(pGame, pPlayer, pItem);
        }
        else
        {
            const int32_t nMaxDurability = STATLIST_GetMaxDurabilityFromUnit(pItem);
            if (nMaxDurability > 0)
            {
                STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nMaxDurability, 0);

                if (pPlayer)
                {
                    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
                    D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(pClient, pItem, 1, STAT_DURABILITY, nMaxDurability, 0);
                }
            }
        }
    }
}

//D2Game.0x6FCC6A60
D2UnitStrc* __fastcall D2GAME_NPC_GenerateStoreItem_6FCC6A60(D2UnitStrc* pNpc, int32_t szCode, D2GameStrc* pGame, int32_t a4, int32_t nQuality, int32_t nItemLevel, int32_t nPlayerLevel)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pNpc);

    int32_t nItemId = 0;
    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(szCode, &nItemId);
    
    int32_t nItemCode = szCode;
    if (pGame->nDifficulty != DIFFMODE_NORMAL && nPlayerLevel > 25)
    {
        if (pGame->nDifficulty == DIFFMODE_NIGHTMARE)
        {
            if (ITEMS_RollRandomNumber(SUNITPROXY_GetSeedFromNpcControl(pGame)) % 100000 < (nItemLevel << 6) + 4000 && pItemsTxtRecord->dwUberCode && pItemsTxtRecord->dwUberCode != '    ')
            {
                DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwUberCode, &nItemId);
                nItemCode = pItemsTxtRecord->dwUberCode;
            }
            else if (pItemsTxtRecord->dwNightmareUpgrade != ' xxx')
            {
                DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwNightmareUpgrade, &nItemId);
                nItemCode = pItemsTxtRecord->dwNightmareUpgrade;
            }
        }
        else
        {
            const uint32_t nRand = ITEMS_RollRandomNumber(SUNITPROXY_GetSeedFromNpcControl(pGame)) % 100000;
            if (pGame->bExpansion && nRand < 16 * nItemLevel + 1000 && pItemsTxtRecord->dwUltraCode && pItemsTxtRecord->dwUltraCode != '    ')
            {
                DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwUltraCode, &nItemId);
                nItemCode = pItemsTxtRecord->dwUltraCode;
            }
            else if (nRand < (nItemLevel << 7) + 5000 && pItemsTxtRecord->dwUberCode && pItemsTxtRecord->dwUberCode != '    ')
            {
                DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwUberCode, &nItemId);
                nItemCode = pItemsTxtRecord->dwUberCode;
            }

            if (pItemsTxtRecord->dwHellUpgrade != ' xxx')
            {
                DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwHellUpgrade, &nItemId);
                nItemCode = pItemsTxtRecord->dwHellUpgrade;
            }
        }
    }

    D2UnitStrc* pItem = nullptr;
    int32_t nItemQuality = nQuality;
    int32_t i = 0;
    do
    {
        int32_t j = 0;
        while (1)
        {
            int32_t bValidItem = 1;

            pItem = D2GAME_CreateItemUnit_6FC501A0(pNpc, nItemId, pGame, 4u, nItemQuality, 0, 1u, nItemLevel, 0, 0, 0);
            if (ITEMS_GetItemQuality(pItem) == ITEMQUAL_INFERIOR)
            {
                D2LowQualityItemsTxt* pLowQualityItemsTxtRecord = DATATBLS_GetLowQualityItemsTxtRecord(ITEMS_GetFileIndex(pItem));
                if (!_strcmpi(pLowQualityItemsTxtRecord->szName, "Cracked"))
                {
                    ITEMS_RemoveFromAllPlayers(pGame, pItem);
                    pItem = nullptr;
                    bValidItem = 0;
                }
            }

            if (bValidItem)
            {
                break;
            }

            ++j;
            if (j >= 5)
            {
                return 0;
            }
        }

        if (nItemCode == ITEMS_GetBaseCode(pItem))
        {
            i = 2;
        }
        else
        {
            ITEMS_RemoveFromAllPlayers(pGame, pItem);
            nItemQuality = ITEMQUAL_NORMAL;
            pItem = nullptr;
        }

        ++i;
    }
    while (i < 2);

    if (!pItem)
    {
        return nullptr;
    }

    const uint8_t nStorePage = ITEMS_GetStorePage(pItem);
    if (nStorePage == 0xFF)
    {
        ITEMS_RemoveFromAllPlayers(pGame, pItem);
        return nullptr;
    }

    ITEMS_SetInvPage(pItem, nStorePage);
    D2GAME_NPC_RepairItem_6FCC6970(pGame, pItem, 0);

    if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pNpc, pItem->dwUnitId, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 1, 1, 0))
    {
        if (nStorePage != 1)
        {
            SUNITPROXY_AllocNpcEvent(pNpc, pGame, pItem, 0);
            return nullptr;
        }

        ITEMS_SetInvPage(pItem, 2u);

        if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pNpc, pItem->dwUnitId, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 1, 1, 0))
        {
            SUNITPROXY_AllocNpcEvent(pNpc, pGame, pItem, 0);
            return nullptr;
        }
    }

    ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 1);

    if (a4)
    {
        pItem->dwFlagEx |= UNITFLAGEX_ISVENDORITEM;

        if (D2GAME_CheckHasFilledSockets_6FC4B2D0(pItem, __FILE__, __LINE__))
        {
            ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
        }

        UNITS_RefreshInventory(pNpc, 1);
        INVENTORY_AddItemToTradeInventory(pNpc->pInventory, pItem);
    }

    return pItem;
}

//D2Game.0x6FCC6F10
void __fastcall sub_6FCC6F10(D2UnitStrc* pNpc, D2UnitStrc* pItem, D2GameStrc* pGame, int32_t bInit)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pNpc);
    D2_ASSERT(pItem);

    int32_t nUnused = 0;
    sub_6FC446B0(pGame, pNpc, pItem->dwUnitId, &nUnused, 0, 0, 0, 0);

    if (bInit)
    {
        pItem->dwFlagEx |= UNITFLAGEX_ITEMINIT;
        UNITS_RefreshInventory(pNpc, 1);
        INVENTORY_AddItemToTradeInventory(pNpc->pInventory, pItem);
        ITEMS_SetItemCMDFlag(pItem, 1, 1);
    }
    else
    {
        ITEMS_RemoveFromAllPlayers(pGame, pItem);
    }
}

//D2Game.0x6FCC6FF0
void __fastcall D2GAME_NPC_BuildHirelingList_6FCC6FF0(D2GameStrc* pGame, D2ClientStrc* pClient, D2UnitStrc* pUnit, int32_t a4)
{
    if (!pUnit)
    {
        return;
    }

    switch (pUnit->dwClassId)
    {
    case MONSTER_KASHYA:
    case MONSTER_GREIZ:
    case MONSTER_ASHEARA:
    case MONSTER_QUAL_KEHK:
        break;
    default:
        return;
    }

    D2GAME_PACKETS_SendHeaderOnlyPacket(pClient, 0x4Fu);

    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(CLIENTS_GetGame(pClient), pUnit, &nUnused);
    if (!pNpcRecord)
    {
        return;
    }

    if (!pNpcRecord->npcTrade.bHireInit)
    {
        D2GAME_NPC_FirstFn_6FCC67D0(CLIENTS_GetGame(pClient), pUnit->dwClassId, pNpcRecord);
    }

    D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty(pGame->bExpansion, pUnit->dwClassId, 0, 0);
    D2_ASSERT(pHirelingTxtRecord);

    const int32_t nHirelingVariants = pHirelingTxtRecord->wNameLast - pHirelingTxtRecord->wNameFirst + 1;
    for (int32_t i = 0; i < nHirelingVariants; ++i)
    {
        D2MercDataStrc* pMercData = &pNpcRecord->pMercData[i];
        if (pMercData->bAvailable && !pMercData->bHired)
        {
            D2GSPacketSrv4E packet4E = {};

            packet4E.nHeader = 0x4Eu;
            packet4E.nMercName = pMercData->nMercName;
            packet4E.nSeed = pMercData->dwSeed;
            D2GAME_SendPacket0x4E_6FC3F410(pClient, &packet4E);
        }
    }
}

//D2Game.0x6FCC7100
void __fastcall D2GAME_NPC_FillStoreInventory_6FCC7100(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, D2NpcTradeStrc* pTrade)
{    
    D2SeedStrc* pSeed = SUNITPROXY_GetSeedFromNpcControl(pGame);

    pTrade->dwTicks = GetTickCount();

    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);
    D2_ASSERT(pNpcRecord);

    const int32_t nLevel = STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0);
    int32_t nItemLevel = nLevel + 5;
    if (pGame->nDifficulty == DIFFMODE_NORMAL)
    {
        constexpr int32_t npcLevels[5] = { 12, 20, 28, 36, 45 };
        if (pNpcRecord->npcTrade.nAct < std::size(npcLevels) && nItemLevel > npcLevels[pNpcRecord->npcTrade.nAct])
        {
            nItemLevel = npcLevels[pNpcRecord->npcTrade.nAct];
        }
    }

    int32_t nSpawnedItems = 0;
    for (int32_t i = 0; i < pNpcRecord->npcTrade.pProxy.nItems; ++i)
    {
        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(pTrade->pProxy.pItemCache[i].dwCode, &nUnused);
        if (pItemsTxtRecord->nLevel <= nItemLevel)
        {
            int32_t nNormalItems = 0;
            if (nItemLevel < 25)
            {
                nNormalItems = pNpcRecord->npcTrade.pProxy.pItemCache[i].nMin + ITEMS_RollLimitedRandomNumber(pSeed, pNpcRecord->npcTrade.pProxy.pItemCache[i].nMax - pNpcRecord->npcTrade.pProxy.pItemCache[i].nMin + 1);
            }

            if (pItemsTxtRecord->wVersion < 100u || pGame->wItemFormat >= 100u)
            {
                for (int32_t j = 0; j < nNormalItems; ++j)
                {
                    const uint32_t nQualityRoll = ITEMS_RollRandomNumber(pSeed) % 100;

                    int32_t nQuality = ITEMQUAL_NORMAL;
                    if (nItemLevel >= 5)
                    {
                        if (nItemLevel >= 10)
                        {
                            if (nQualityRoll >= 75)
                            {
                                nQuality = ITEMQUAL_SUPERIOR;
                            }
                        }
                        else
                        {
                            if (nQualityRoll > 85)
                            {
                                nQuality = ITEMQUAL_SUPERIOR;
                            }
                        }
                    }
                    else
                    {
                        if (nQualityRoll > 90)
                        {
                            nQuality = ITEMQUAL_INFERIOR;
                        }
                    }

                    if (!D2GAME_NPC_GenerateStoreItem_6FCC6A60(pNpc, pNpcRecord->npcTrade.pProxy.pItemCache[i].dwCode, pGame, 1, nQuality, nItemLevel, nLevel))
                    {
                        ++nSpawnedItems;
                    }

                    if (nSpawnedItems > 32)
                    {
                        return;
                    }
                }

                int32_t nItemId = 0;
                DATATBLS_GetItemRecordFromItemCode(pTrade->pProxy.pItemCache[i].dwCode, &nItemId);

                if (ITEMS_CheckBitField1Flag1(nItemId) == 1 && pTrade->pProxy.pItemCache[i].nMagicLevel <= nItemLevel)
                {
                    int32_t nMinItems = 1;
                    if (nItemLevel >= 25)
                    {
                        nMinItems = (ITEMS_RollRandomNumber(pSeed) & 1) + 2;
                    }

                    const int32_t nMagicItems = pNpcRecord->npcTrade.pProxy.pItemCache[i].nMagicMin + ITEMS_RollLimitedRandomNumber(pSeed, nMinItems + pNpcRecord->npcTrade.pProxy.pItemCache[i].nMagicMax - pNpcRecord->npcTrade.pProxy.pItemCache[i].nMagicMin);
                    for (int32_t j = 0; j < nMagicItems; ++j)
                    {
                        if (!D2GAME_NPC_GenerateStoreItem_6FCC6A60(pNpc, pNpcRecord->npcTrade.pProxy.pItemCache[i].dwCode, pGame, 1, ITEMQUAL_MAGIC, nItemLevel, nLevel))
                        {
                            ++nSpawnedItems;
                        }
                    }
                }
            }
        }
    }

    for (int32_t i = 0; i < pNpcRecord->npcTrade.pProxy.nPerms; ++i)
    {
        D2UnitStrc* pStoreItem = D2GAME_NPC_GenerateStoreItem_6FCC6A60(pNpc, pNpcRecord->npcTrade.pProxy.pPermCache[i], pGame, 1, ITEMQUAL_NORMAL, nItemLevel, nLevel);
        if (pStoreItem)
        {
            const int32_t nItemCode = DATATBLS_GetItemRecordFromItemCode(pTrade->pProxy.pPermCache[i], &nUnused)->dwCode;
            if (nItemCode == ' vqc' || nItemCode == ' vqa')
            {
                STATLIST_SetUnitStat(pStoreItem, STAT_QUANTITY, ITEMS_GetTotalMaxStack(pStoreItem), 0);
            }
        }
        else
        {
            ++nSpawnedItems;
        }

        if (nSpawnedItems > 32)
        {
            break;
        }
    }
}

//D2Game.0x6FCC74F0
void __fastcall D2GAME_STORES_CreateVendorCache_6FCC74F0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNPC, int32_t a4, int32_t a5)
{
    if (!pNPC)
    {
        return;
    }

    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNPC, &nUnused);
    if (!pNpcRecord)
    {
        return;
    }

    pNpcRecord->bTrading = 1;
    pNpcRecord->npcTrade.dwNPCGUID = pNPC->dwUnitId;
    D2GAME_STORES_CreateVendorCache_6FCCAE20(pGame, pPlayer, pNPC, a5);

    switch (pNPC->dwClassId)
    {
    case MONSTER_KASHYA:
    case MONSTER_GREIZ:
    case MONSTER_QUAL_KEHK:
        break;
    default:
        return;
    }

    if (a4 && !a5 && pNpcRecord->npcTrade.bRefreshInventory)
    {
        pNpcRecord->npcTrade.bRefreshInventory = 0;
        SUNITPROXY_AllocNpcInventory(pGame, pNpcRecord, pNPC);
        D2GAME_NPC_FillStoreInventory_6FCC7100(pGame, pPlayer, pNPC, &pNpcRecord->npcTrade);
        pNpcRecord->npcTrade.bVendorInit = 1;
    }

    UNITS_RefreshInventory(pNPC, 1);

    if (a5)
    {
        D2InventoryStrc* pGambleInventory = SUNITPROXY_GetGambleInventory(pGame, pPlayer, pNPC);
        if (pGambleInventory)
        {
            for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pGambleInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
            {
                if (INVENTORY_UnitIsItem(pItem))
                {
                    pItem->dwFlagEx |= UNITFLAGEX_ISVENDORITEM;

                    if (D2GAME_CheckHasFilledSockets_6FC4B2D0(pItem, __FILE__, __LINE__))
                    {
                        ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
                    }

                    INVENTORY_AddItemToTradeInventory(pNPC->pInventory, pItem);
                }
            }
        }
    }
    else
    {
        for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pNPC->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
        {
            if (INVENTORY_UnitIsItem(pItem))
            {
                pItem->dwFlagEx |= UNITFLAGEX_ISVENDORITEM;

                if (D2GAME_CheckHasFilledSockets_6FC4B2D0(pItem, __FILE__, __LINE__))
                {
                    ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
                }

                INVENTORY_AddItemToTradeInventory(pNPC->pInventory, pItem);
            }
        }
    }
}

//D2Game.0x6FCC7680
int32_t __fastcall D2GAME_STORES_SellItem_6FCC7680(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcGUID, int32_t nItemGUID, int16_t nItemMode, int32_t a6)
{
    D2_ASSERT(pGame);

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        return 1;
    }

    D2UnitStrc* pInteractUnit = SUNIT_GetInteractUnit(pGame, pPlayer);
    D2UnitStrc* pNpc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nNpcGUID);
    if (!pNpc || pInteractUnit != pNpc)
    {
        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
        return 1;
    }

    if (!ITEMS_IsInPlayersInventory(pPlayer, pItem, 0))
    {
        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 11, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
        return 3;
    }

    if (pItem->dwAnimMode != nItemMode)
    {
        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
        return 3;
    }

    if (!ITEMS_IsNotQuestItem(pItem))
    {
        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
        return 3;
    }

    const int32_t nTransactionCost = ITEMS_GetTransactionCost(pPlayer, pItem, (D2C_Difficulties)pGame->nDifficulty, UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty], pNpc->dwClassId, D2C_TransactionTypes::TRANSACTIONTYPE_SELL);

    int32_t bReSellAble = 1;

    if (ITEMS_GetItemQuality(pItem) == ITEMQUAL_INFERIOR)
    {
        D2LowQualityItemsTxt* pLowQualityItemsTxtRecord = DATATBLS_GetLowQualityItemsTxtRecord(ITEMS_GetFileIndex(pItem));
        if (!_strcmpi(pLowQualityItemsTxtRecord->szName, "Cracked"))
        {
            bReSellAble = 0;
        }
    }

    if (ITEMS_CheckItemFlag(pItem, IFLAG_BROKEN, __LINE__, __FILE__))
    {
        bReSellAble = 0;
    }

    if (ITEMS_GetItemType(pItem) == ITEMTYPE_PLAYER_BODY_PART)
    {
        bReSellAble = 0;
    }

    if (ITEMS_CheckItemFlag(pItem, IFLAG_PERSONALIZED, __LINE__, __FILE__))
    {
        bReSellAble = 0;
    }

    if (ITEMS_CheckItemFlag(pItem, IFLAG_ETHEREAL, __LINE__, __FILE__))
    {
        bReSellAble = 0;
    }

    if (D2GAME_CheckHasFilledSockets_6FC4B2D0(pItem, __FILE__, __LINE__))
    {
        bReSellAble = 0;
    }

    if (pItem->dwUnitType == UNIT_ITEM && ITEMS_GetItemQuality(pItem) == ITEMQUAL_UNIQUE)
    {
        D2UniqueItemsTxt* pUniqueItemsTxtRecord = ITEMS_GetUniqueItemsTxtRecord(ITEMS_GetFileIndex(pItem));
        if (pUniqueItemsTxtRecord && pUniqueItemsTxtRecord->dwUniqueItemFlags & gdwBitMasks[UNIQUEITEMSFLAGINDEX_CARRY1])
        {
            bReSellAble = 0;
        }
    }

    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);
    if (SUNITPROXY_GetVendorChain(pGame, pNpcRecord, pPlayer)->field_4)
    {
        bReSellAble = 0;
    }

    if (bReSellAble)
    {
        int32_t bCreateDupe = 1;
        if (pNpcRecord)
        {
            const uint32_t nBaseCode = ITEMS_GetBaseCode(pItem);
            if (pGame->nDifficulty != DIFFMODE_NORMAL && (nBaseCode == ' 4ph' || nBaseCode == ' 5ph' || nBaseCode == ' 4pm' || nBaseCode == ' 5pm'))
            {
                bCreateDupe = 0;
            }
            else
            {
                for (int32_t i = 0; i < pNpcRecord->npcTrade.pProxy.nPerms; ++i)
                {
                    if (nBaseCode == pNpcRecord->npcTrade.pProxy.pPermCache[i])
                    {
                        bCreateDupe = 0;
                        break;
                    }
                }
            }
        }

        if (bCreateDupe)
        {
            D2UnitStrc* pDupeItem = ITEMS_Duplicate(pGame, pItem, pNpc, 1);
            if (!pDupeItem)
            {
                D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
                return 3;
            }

            UNITS_ChangeAnimMode(pDupeItem, 4);

            const uint8_t nStorePage = ITEMS_GetStorePage(pDupeItem);
            if (nStorePage != 0xFF)
            {
                ITEMS_SetInvPage(pDupeItem, nStorePage);

                if (D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pNpc, pDupeItem->dwUnitId, CLIENTS_GetUnitX(pDupeItem), CLIENTS_GetUnitY(pDupeItem), 1, 1, 0))
                {
                    ITEMS_SetItemFlag(pDupeItem, IFLAG_IDENTIFIED, 1);

                    pDupeItem->dwFlagEx |= UNITFLAGEX_ISVENDORITEM;

                    if (D2GAME_CheckHasFilledSockets_6FC4B2D0(pDupeItem, __FILE__, __LINE__))
                    {
                        ITEMS_SetItemFlag(pDupeItem, IFLAG_NEWITEM, 1);
                    }

                    UNITS_RefreshInventory(pNpc, 1);

                    INVENTORY_AddItemToTradeInventory(pNpc->pInventory, pDupeItem);
                    STATLIST_SetUnitStat(pDupeItem, STAT_DURABILITY, STATLIST_GetMaxDurabilityFromUnit(pDupeItem), 0);
                    STATLIST_SetUnitStat(pDupeItem, STAT_QUANTITY, ITEMS_GetTotalMaxStack(pDupeItem), 0);
                }
                else if (nStorePage == 1)
                {
                    ITEMS_SetInvPage(pDupeItem, 2u);

                    if (D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pNpc, pDupeItem->dwUnitId, CLIENTS_GetUnitX(pDupeItem), CLIENTS_GetUnitY(pDupeItem), 1, 1, 0))
                    {
                        ITEMS_SetItemFlag(pDupeItem, IFLAG_IDENTIFIED, 1);

                        pDupeItem->dwFlagEx |= UNITFLAGEX_ISVENDORITEM;

                        if (D2GAME_CheckHasFilledSockets_6FC4B2D0(pDupeItem, __FILE__, __LINE__))
                        {
                            ITEMS_SetItemFlag(pDupeItem, IFLAG_NEWITEM, 1);
                        }

                        UNITS_RefreshInventory(pNpc, 1);

                        INVENTORY_AddItemToTradeInventory(pNpc->pInventory, pDupeItem);
                        STATLIST_SetUnitStat(pDupeItem, STAT_DURABILITY, STATLIST_GetMaxDurabilityFromUnit(pDupeItem), 0);
                        STATLIST_SetUnitStat(pDupeItem, STAT_QUANTITY, ITEMS_GetTotalMaxStack(pDupeItem), 0);
                    }
                    else
                    {
                        ITEMS_RemoveFromAllPlayers(pGame, pDupeItem);
                    }
                }
                else
                {
                    ITEMS_RemoveFromAllPlayers(pGame, pDupeItem);
                }
            }
            else
            {
                ITEMS_RemoveFromAllPlayers(pGame, pDupeItem);
            }
        }
    }

    if (nItemMode == IMODE_STORED)
    {
        if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
        {
            D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
            if (pBooksTxtRecord->dwScrollSkillId >= 0)
            {
                D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pPlayer, pBooksTxtRecord->dwScrollSkillId);
                if (pSkill)
                {
                    int32_t nSkillQuantity = SKILLS_GetQuantity(pSkill) - 1;
                    if (nSkillQuantity <= 0)
                    {
                        if (UNITS_GetRightSkill(pPlayer) == pSkill)
                        {
                            D2GAME_AssignSkill_6FD13800(pPlayer, 0, 0, -1);
                        }

                        nSkillQuantity = 0;
                    }

                    SKILLS_SetQuantity(pSkill, nSkillQuantity);

                    int32_t nUnitGUID = -1;
                    int32_t nUnitType = 6;
                    if (pPlayer)
                    {
                        nUnitGUID = pPlayer->dwUnitId;
                        nUnitType = pPlayer->dwUnitType;
                    }

                    D2GAME_PACKETS_SendPacket0x22_6FC3DBE0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), nUnitType, nUnitGUID, pBooksTxtRecord->dwScrollSkillId, nSkillQuantity);
                }
            }
        }
        else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
        {
            const int32_t nQuantity = STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0);
            if (nQuantity >= 0)
            {
                D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
                if (pBooksTxtRecord->dwScrollSkillId >= 0)
                {
                    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pPlayer, pBooksTxtRecord->dwScrollSkillId);
                    if (pSkill)
                    {
                        int32_t nSkillQuantity = SKILLS_GetQuantity(pSkill) - nQuantity;
                        if (nSkillQuantity <= 0)
                        {
                            if (UNITS_GetRightSkill(pPlayer) == pSkill)
                            {
                                D2GAME_AssignSkill_6FD13800(pPlayer, 0, 0, -1);
                            }

                            nSkillQuantity = 0;
                        }

                        SKILLS_SetQuantity(pSkill, nSkillQuantity);

                        int32_t nUnitGUID = -1;
                        int32_t nUnitType = 6;
                        if (pPlayer)
                        {
                            nUnitGUID = pPlayer->dwUnitId;
                            nUnitType = pPlayer->dwUnitType;
                        }

                        D2GAME_PACKETS_SendPacket0x22_6FC3DBE0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), nUnitType, nUnitGUID, pBooksTxtRecord->dwScrollSkillId, nSkillQuantity);
                    }
                }
            }
        }

        ITEMS_SetItemCell(pItem, ITEMS_GetInvPage(pItem));
        D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), pPlayer, pItem, 0x20);
        D2GAME_RemoveItem_6FC471F0(pGame, pPlayer, pItem, 0);
    }
    else if (nItemMode == IMODE_ONCURSOR)
    {
        if (!D2GAME_RemoveItemIfOnCursor_6FC49760(pGame, pPlayer, pItem))
        {
            D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
            return 1;
        }
    }
    else
    {
        sub_6FC45B30(pGame, pPlayer, ITEMS_GetBodyLocation(pItem), 1, &nUnused);
    }

    ITEMS_HandleGoldTransaction(pGame, pPlayer, nTransactionCost);
    D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 1, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), nItemGUID, 3);
    return 0;
}

//D2Game.0x6FCC7E20
void __fastcall sub_6FCC7E20(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pItem, D2UnitStrc* pUnit, int32_t a5)
{
    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);

    if (pNpcRecord->bGambleInit && a5 == 2)
    {
        D2NpcGambleStrc* pNpcGamble = pNpcRecord->pGamble;
        const int32_t nUnitGUID = pUnit ? pUnit->dwUnitId : -1;
        while (pNpcGamble)
        {
            if (pNpcGamble->dwGUID == nUnitGUID)
            {
                break;
            }

            pNpcGamble = pNpcGamble->pNext;
        }

        if (pNpcGamble && pNpcGamble->pInventory)
        {
            for (D2UnitStrc* i = INVENTORY_GetFirstItem(pNpcGamble->pInventory); i; i = INVENTORY_GetNextItem(i))
            {
                if (INVENTORY_UnitIsItem(i) == pItem)
                {
                    if (pItem && pNpc)
                    {
                        D2InventoryStrc* pGambleInventory = SUNITPROXY_GetGambleInventory(pGame, pUnit, pNpc);
                        if (pGambleInventory)
                        {
                            sub_6FC446B0(pGame, pNpc, pItem->dwUnitId, &nUnused, 0, 0, pGambleInventory, 0);

                            pItem->dwFlagEx |= 0x10u;

                            UNITS_RefreshInventory(pNpc, 1);
                            INVENTORY_AddItemToTradeInventory(pGambleInventory, pItem);
                            ITEMS_SetItemCMDFlag(pItem, 1, 1);
                        }
                        else
                        {
                            FOG_Trace("Error at line %d of file %s", __LINE__, __FILE__);
                        }
                    }
                    return;
                }
            }

        }
    }

    pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);
    if (pNpcRecord)
    {
        const uint32_t nBaseCode = ITEMS_GetBaseCode(pItem);
        if (pGame->nDifficulty && (nBaseCode == ' 4ph' || nBaseCode == ' 5ph' || nBaseCode == ' 4pm' || nBaseCode == ' 5pm'))
        {
            return;
        }

        for (int32_t i = 0; i < pNpcRecord->npcTrade.pProxy.nPerms; ++i)
        {
            if (nBaseCode == pNpcRecord->npcTrade.pProxy.pPermCache[i])
            {
                return;
            }
        }
    }

    sub_6FCC6F10(pNpc, pItem, pGame, 1);
    return;
}

//D2Game.0x6FCC7FA0
void __fastcall sub_6FCC7FA0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, uint16_t wName)
{
    int32_t nLevel = STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0);
    if (pGame->nDifficulty == DIFFMODE_NORMAL)
    {
        int32_t nUnused = 0;
        D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);
        if (pNpcRecord)
        {
            constexpr int32_t npcLevels[5] = { 12, 20, 28, 36, 45 };
            if (pNpcRecord->npcTrade.nAct < std::size(npcLevels) && nLevel > npcLevels[pNpcRecord->npcTrade.nAct])
            {
                nLevel = npcLevels[pNpcRecord->npcTrade.nAct];
            }
        }
    }

    if (pNpc)
    {
        if (pNpc->dwClassId == MONSTER_QUAL_KEHK && !QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty], QUEST_A5Q6_BAAL, QFLAG_REWARDGRANTED))
        {
            return D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 11, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
        }

        if (pNpc->dwClassId == MONSTER_KASHYA && nLevel < 8 && !QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty], QUEST_A1Q2_BLOODRAVEN, QFLAG_REWARDGRANTED))
        {
            return D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 11, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
        }
    }

    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);
    if (!pNpcRecord)
    {
        return D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
    }

    int32_t nVendorId = -1;
    if (pNpc)
    {
        nVendorId = pNpc->dwClassId;
    }

    D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty(pGame->bExpansion, nVendorId, 0, 0);
    D2_ASSERT(pHirelingTxtRecord);

    if (wName < pHirelingTxtRecord->wNameFirst || wName > pHirelingTxtRecord->wNameLast)
    {
        return D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
    }

    D2MercDataStrc* pMercDataRecord = &pNpcRecord->pMercData[wName - pHirelingTxtRecord->wNameFirst];
    if (pMercDataRecord->nMercName != wName)
    {
        return D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
    }

    if (pMercDataRecord->bHired)
    {
        return D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
    }

    D2HirelingInitStrc hirelingInit = {};
    if (!MONSTERS_HirelingInit(pGame->bExpansion, pPlayer, pMercDataRecord->dwSeed, MONSTERS_GetActFromHirelingTxt(pGame->bExpansion, 0, pMercDataRecord->nMercName), pGame->nDifficulty, &hirelingInit))
    {
        return;
    }

    int32_t nGold = STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0);
    if (nGold + STATLIST_UnitGetStatValue(pPlayer, STAT_GOLDBANK, 0) < hirelingInit.nGold)
    {
        return D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 12, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
    }

    if (nGold < hirelingInit.nGold)
    {
        PLRTRADE_AddGold(pPlayer, STAT_GOLD, -nGold);
        PLRTRADE_AddGold(pPlayer, STAT_GOLDBANK, nGold - hirelingInit.nGold);
    }
    else
    {
        PLRTRADE_AddGold(pPlayer, STAT_GOLD, -hirelingInit.nGold);
    }

    D2UnitStrc* pMerc = sub_6FC68D70(pGame, pNpc, pHirelingTxtRecord->dwClass, 1, 4, 0);
    if (!pMerc)
    {
        pMerc = sub_6FC68D70(pGame, pPlayer, pHirelingTxtRecord->dwClass, 1, 4, 0);
        if (!pMerc)
        {
            return D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 15, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
        }
    }

    pMercDataRecord->bHired = 1;

    sub_6FC61270(pGame, pPlayer, pMerc, -1, pMercDataRecord, 0);

    D2MonsterInteractStrc* pMonInteract = nullptr;
    if (pNpc && pNpc->dwUnitType == UNIT_MONSTER && pNpc->pMonsterData)
    {
        pMonInteract = pNpc->pMonsterData->pMonInteract;
    }

    D2GAME_NPC_BuildHirelingList_6FCC6FF0(pGame, SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), pNpc, MONSTERAI_GetInteractUnitCount(pMonInteract) <= 1);
    D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 5, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), pMerc ? pMerc->dwUnitId : -1, 0);

    pHirelingTxtRecord = DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty(pGame->bExpansion, nVendorId, 0, 0);
    D2_ASSERT(pHirelingTxtRecord);

    for (int32_t i = 0; i < pHirelingTxtRecord->wNameLast - pHirelingTxtRecord->wNameFirst + 1; ++i)
    {
        if (pNpcRecord->pMercData[i].bAvailable && !pNpcRecord->pMercData[i].bHired)
        {
            return;
        }
    }

    D2_FREE_POOL(pGame->pMemoryPool, pNpcRecord->pMercData);

    pNpcRecord->npcTrade.bHireInit = 0;
    pNpcRecord->pMercData = nullptr;

    D2GAME_NPC_FirstFn_6FCC67D0(pGame, nVendorId, pNpcRecord);
}

//D2Game.0x6FCC8430
void __fastcall sub_6FCC8430(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (sub_6FC7E8B0(pGame, pUnit, 7, 0))
    {
        sub_6FC7E310(pGame, pUnit, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
    }
}

//D2Game.0x6FCC84D0
void __fastcall sub_6FCC84D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet)
{
    if (!pPet || !pPlayer)
    {
        return;
    }

    D2UnitStrc* pHireling = sub_6FC7E8B0(pGame, pPlayer, 7, 0);
    if (pHireling)
    {
        AIGENERAL_SetOwnerData(pGame, pHireling, -1u, UNIT_MONSTER, 0, 0);
        sub_6FC7D260(pGame, pPlayer, pHireling->dwUnitId, 0);
        DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pHireling), UNIT_MONSTER, pHireling->dwUnitId);
        SUNIT_RemoveUnit(pGame, pHireling);
    }

    UNITS_ChangeAnimMode(pPet, MONMODE_NEUTRAL);
    STATLIST_SetUnitStat(pPet, STAT_HITPOINTS, STATLIST_GetMaxLifeFromUnit(pPet), 0);

    sub_6FC7DD90(pGame, pPlayer, pPet);
    D2GAME_UpdateSummonAI_6FC401F0(pGame, pPet, 0, pPlayer->dwNodeIndex);
    STATES_ToggleState(pPet, STATE_FREEZE, 0);
    STATES_ToggleState(pPet, STATE_SHATTER, 0);
    STATES_ToggleState(pPet, STATE_RECYCLED, 0);

    pPet->dwFlags |= UNITFLAG_NOXP | UNITFLAG_NOTC | UNITFLAG_ISVALIDTARGET | UNITFLAG_CANBEATTACKED | UNITFLAG_TARGETABLE;
    pPet->dwFlagEx &= 0xFFFBFFFF;

    sub_6FC7E310(pGame, pPlayer, CLIENTS_GetUnitX(pPlayer), CLIENTS_GetUnitY(pPlayer));
    MONSTERAI_SendMercStats(pGame, pPlayer, 0);
}

//D2Game.0x6FCC8630
D2UnitStrc* __fastcall D2GAME_MERCS_Create_6FCC8630(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint16_t wName, uint32_t nSeed, int16_t wVersion, int32_t nBaseMonster, int32_t bDead)
{
    uint8_t nAct = pPlayer->nAct;
    const uint8_t nMercAct = MONSTERS_GetActFromHirelingTxt(pGame->bExpansion, 0, wName);

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    if (pClient)
    {
        nAct = CLIENTS_GetActNo(pClient);
    }

    if (!pGame->bExpansion && (nMercAct < 0 || nAct != nMercAct))
    {
        return nullptr;
    }

    D2HirelingTxt* pHirelingTxtRecord = nullptr;
    if (wVersion == -1)
    {
        pHirelingTxtRecord = DATATBLS_GetNextHirelingTxtRecordFromActAndDifficulty(pGame->bExpansion, nMercAct, pGame->nDifficulty, nullptr);
    }
    else
    {
        pHirelingTxtRecord = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, wVersion, 1);
    }

    D2_ASSERT(pHirelingTxtRecord);

    int32_t nClassId = nBaseMonster;
    if (!nBaseMonster)
    {
        nClassId = pHirelingTxtRecord->dwClass;
    }

    uint16_t nMercName = wName;
    if (wName < pHirelingTxtRecord->wNameFirst || wName > pHirelingTxtRecord->wNameLast)
    {
        nMercName = pHirelingTxtRecord->wNameFirst;
    }

    int32_t nUnused = 0;
    D2MercDataStrc* pMercData = SUNITPROXY_GetNpcRecordFromClassId(pGame, pHirelingTxtRecord->dwSeller, &nUnused)->pMercData;
    if (pMercData)
    {
        D2MercDataStrc* pMercDataRecord = &pMercData[nMercName - pHirelingTxtRecord->wNameFirst];
        if (!pMercDataRecord->bAvailable)
        {
            pMercDataRecord->bHired = 1;
        }
    }

    D2UnitStrc* pMerc = SUNIT_AllocUnitData(UNIT_MONSTER, nClassId, 0, 0, pGame, nullptr, 1, bDead ? MONMODE_DEAD : MONMODE_NEUTRAL, 0);
    if (pMerc)
    {
        D2MercDataStrc mercData = {};
        mercData.dwSeed = nSeed;
        mercData.nMercName = nMercName;
        sub_6FC61270(pGame, pPlayer, pMerc, wVersion, &mercData, bDead);
    }

    return pMerc;
}

//D2Game.0x6FCC87C0
D2UnitStrc* __fastcall sub_6FCC87C0(D2GameStrc* pPlayer, D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t* a4)
{
    *a4 = 1;

    if (!pUnit->pInventory || !ITEMS_CheckIfStackable(pItem) || !ITEMS_GetAutoStack(pItem))
    {
        return nullptr;
    }

    D2UnitStrc* pStackItem = nullptr;
    int32_t nMaxStack = ITEMS_GetTotalMaxStack(pItem);
    while (1)
    {
        pStackItem = INVENTORY_FindEquippedItemForStack(pUnit->pInventory, pItem, pStackItem);
        if (!pStackItem)
        {
            break;
        }

        if (ITEMS_AreStackablesEqual(pItem, pStackItem))
        {
            const int32_t nQuantity = STATLIST_UnitGetStatValue(pStackItem, STAT_QUANTITY, 0);
            nMaxStack = ITEMS_GetTotalMaxStack(pStackItem);
            if (nQuantity < nMaxStack)
            {
                *a4 = nMaxStack - nQuantity;
                return pStackItem;
            }
        }
    }

    int32_t nQuantity = 0;
    do
    {
        do
        {
            pStackItem = INVENTORY_FindBackPackItemForStack(pUnit->pInventory, pItem, pStackItem);
            if (!pStackItem)
            {
                *a4 = nMaxStack;
                return nullptr;
            }
        }
        while (!ITEMS_AreStackablesEqual(pItem, pStackItem));

        nQuantity = STATLIST_UnitGetStatValue(pStackItem, STAT_QUANTITY, 0);
        nMaxStack = ITEMS_GetTotalMaxStack(pStackItem);
    }
    while (nQuantity >= nMaxStack);

    *a4 = nMaxStack - nQuantity;
    return pStackItem;
}

//D2Game.0x6FCC88B0) --------------------------------------------------------
int32_t __fastcall sub_6FCC88B0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, int32_t nItemGUID, int32_t nItemMode, uint16_t nTransactionType, int32_t nCost, int32_t bMultibuy)
{
    return 0;

//    D2GameStrc* v8; // ebx@1
//    D2UnitStrc* v9; // esi@1
//    D2UnitStrc* v10; // ebp@4
//    D2InventoryStrc* v11; // eax@9
//    int32_t v12; // edi@16
//    D2PlayerDataStrc* v13; // eax@16
//    int32_t v14; // edi@16
//    D2NpcRecordStrc* v15; // edi@20
//    int32_t v16; // eax@26
//    int32_t v17; // ecx@26
//    uint32_t* v18; // edx@27
//    int32_t v19; // edi@36
//    int32_t v20; // ebx@36
//    int32_t v21; // edi@38
//    int32_t v22; // ebx@38
//    int32_t v23; // edi@49
//    int32_t v24; // ebx@51
//    D2PlayerDataStrc* v25; // eax@51
//    int32_t v26; // eax@51
//    uint32_t v27; // edi@51
//    D2UnitStrc* v28; // ebx@51
//    int32_t v29; // eax@51
//    int32_t v30; // ecx@51
//    int32_t v31; // edi@58
//    int32_t v32; // edi@60
//    int32_t v33; // ST158_4@67
//    D2ClientStrc* v34; // eax@67
//    uint32_t v35; // ST15C_4@67
//    D2UnitStrc* v36; // edi@75
//    int32_t v37; // ebp@80
//    int32_t v38; // edx@80
//    int32_t v39; // eax@80
//    int32_t v40; // ebp@82
//    int32_t v41; // eax@91
//    D2InventoryStrc* v42; // ebx@95
//    int32_t v43; // eax@96
//    D2UnitStrc* v44; // ebx@100
//    int32_t v45; // eax@100
//    int32_t v46; // ST158_4@117
//    D2ClientStrc* v47; // eax@117
//    int32_t v48; // eax@120
//    int32_t v49; // ebx@121
//    int32_t v50; // eax@127
//    int32_t v51; // ebx@128
//    D2ClientStrc* v52; // eax@136
//    D2ClientStrc* v54; // eax@139
//    int32_t v55; // [sp+150h] [bp-4Ch]@7
//    int32_t v56; // [sp+150h] [bp-4Ch]@17
//    int32_t v57; // [sp+154h] [bp-48h]@7
//    int32_t v58; // [sp+154h] [bp-48h]@17
//    int32_t v59; // [sp+158h] [bp-44h]@7
//    int32_t v60; // [sp+158h] [bp-44h]@17
//    int32_t v61; // [sp+15Ch] [bp-40h]@7
//    int32_t v62; // [sp+15Ch] [bp-40h]@17
//    int32_t v63; // [sp+160h] [bp-3Ch]@7
//    int32_t v64; // [sp+160h] [bp-3Ch]@17
//    D2GameStrc* v65; // [sp+174h] [bp-28h]@1
//    D2UnitStrc* v66; // [sp+178h] [bp-24h]@32
//    int32_t v67; // [sp+178h] [bp-24h]@46
//    int32_t v68; // [sp+17Ch] [bp-20h]@32
//    int32_t v69; // [sp+180h] [bp-1Ch]@5
//    int32_t v70; // [sp+184h] [bp-18h]@16
//    D2InventoryStrc* v71; // [sp+188h] [bp-14h]@18
//    D2UnitStrc* v72; // [sp+18Ch] [bp-10h]@4
//    int32_t v73; // [sp+190h] [bp-Ch]@75
//    int32_t v74; // [sp+194h] [bp-8h]@20
//    int32_t v75; // [sp+198h] [bp-4h]@96
//    int32_t v76; // [sp+1A0h] [bp+4h]@121
//    int32_t v77; // [sp+1A0h] [bp+4h]@128
//    int32_t v78; // [sp+1A4h] [bp+8h]@14
//    int32_t v79; // [sp+1A4h] [bp+8h]@21
//    int32_t v80; // [sp+1A4h] [bp+8h]@33
//    int32_t v81; // [sp+1A4h] [bp+8h]@48
//    int32_t v82; // [sp+1A4h] [bp+8h]@51
//    int32_t v83; // [sp+1A4h] [bp+8h]@56
//    int32_t v84; // [sp+1A4h] [bp+8h]@71
//    char v85; // [sp+1A4h] [bp+8h]@100
//    int32_t v86; // [sp+1ACh] [bp+10h]@16
//    int32_t v87; // [sp+1ACh] [bp+10h]@51
//    int32_t v88; // [sp+1B4h] [bp+18h]@58
//    int32_t v89; // [sp+1B4h] [bp+18h]@60
//
//    v8 = pGame;
//    v9 = pPlayer;
//    v65 = pGame;
//    
//    D2_ASSERT(pGame);
//
//    v10 = SUNIT_GetServerUnit(pGame, 4, nItemGUID);
//    v72 = v10;
//    if (!v10)
//    {
//        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 7, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), nItemGUID, 0);
//        return 1;
//    }
//
//    v69 = nTab;
//    if (nTab)
//    {
//        if (nTab == 2)
//        {
//            v11 = SUNITPROXY_GetGambleInventory(v8, v9, pNpc);
//            if (!v11)
//            {
//                D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 7, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), nItemGUID, 0);
//                return 1;
//            }
//
//            if (ITEMS_IsInPlayersInventory(pNpc, v10, v11) != 1)
//            {
//                D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 7, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), nItemGUID, 0);
//                return 1;
//            }
//        }
//    }
//    else
//    {
//        if (!ITEMS_IsInPlayersInventory(pNpc, v10, 0))
//        {
//            D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 7, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), nItemGUID, 0);
//            return 1;
//        }
//    }
//
//    if (pNpc)
//        v78 = pNpc->dwClassId;
//    else
//        v78 = -1;
//
//    v12 = v8->nDifficulty;
//    v13 = UNITS_GetPlayerData(v9);
//    v86 = ITEMS_GetTransactionCost(v9, v10, v12, v13->pQuestData[v12], v78, v69);
//    v14 = STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0);
//    v70 = v14 + STATLIST_UnitGetStatValue(v9, STAT_GOLDBANK, 0);
//    if (v86 > v70)
//    {
//        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 12, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), -1, 0);
//        return 0;
//    }
//
//    v71 = v9->pInventory;
//    if (INVENTORY_GetCursorItem(v71))
//    {
//        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 7, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), -1, 0);
//        return 1;
//    }
//
//    v15 = UNITPROXY_GetNpcRecordFromUnit(v8, pNpc, &v74);
//    if (!v15)
//        goto LABEL_30;
//
//    v79 = ITEMS_GetBaseCode(v10);
//    if (v8->nDifficulty && (v79 == ' 4ph' || v79 == ' 5ph' || v79 == ' 4pm' || v79 == ' 5pm'))
//        goto LABEL_31;
//
//    v16 = v15->npcTrade.pProxy.nPerms;
//    v17 = 0;
//    if (v16 <= 0)
//    {
//LABEL_30:
//        a8 = 0;
//        goto LABEL_31;
//    }
//
//    v18 = v15->npcTrade.pProxy.pPermCache;
//    while (v79 != *v18)
//    {
//        ++v17;
//        ++v18;
//        if (v17 >= v16)
//            goto LABEL_30;
//    }
//
//LABEL_31:
//    if (ITEMS_CheckItemTypeId(v10, 22))
//    {
//        v66 = sub_6FC4B430(v8, v9, v10, &v68);
//        if (v66)
//        {
//            v80 = 1;
//            if (a8)
//            {
//                v80 = v70 / (uint32_t)v86;
//                if ((int32_t)(v70 / (uint32_t)v86) > v68)
//                    v80 = v68;
//            }
//
//            v19 = v86 * v80;
//            v20 = STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0);
//            if (v20 + STATLIST_UnitGetStatValue(v9, STAT_GOLDBANK, 0) < v86 * v80)
//            {
//                D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 12, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), -1, 0);
//                return 0;
//            }
//            else
//            {
//                if (v19 > v20)
//                {
//                    PLRTRADE_AddGold(v9, STAT_GOLD, -v20);
//                    PLRTRADE_AddGold(v9, STAT_GOLDBANK, v20 - v19);
//                }
//                else
//                {
//                    PLRTRADE_AddGold(v9, STAT_GOLD, -v19);
//                }
//
//                sub_6FC4B520(v65, v9, v66, v80);
//                sub_6FCC7E20(v65, pNpc, v10, v9, v69);
//                D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 0, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), v66->dwUnitId, 5);
//                return 0;
//            }
//        }
//    }
//
//    v67 = STATLIST_UnitGetStatValue(v10, STAT_QUANTITY, 0);
//    if (!a8 || !ITEMS_GetAutoStack(v10))
//        goto LABEL_71;
//
//    v81 = STATLIST_UnitGetStatValue(v10, STAT_QUANTITY, 0);
//    STATLIST_SetUnitStat(v10, STAT_QUANTITY, 1, 0);
//    if (pNpc)
//        v23 = pNpc->dwClassId;
//    else
//        v23 = -1;
//
//    v24 = v65->nDifficulty;
//    v25 = UNITS_GetPlayerData(v9);
//    v26 = ITEMS_GetTransactionCost(v9, v10, v24, v25->pQuestData[v24], v23, 0);
//    v27 = v26;
//    v87 = v26;
//    STATLIST_SetUnitStat(v10, STAT_QUANTITY, v81, 0);
//    v82 = v70 / v27;
//    v28 = sub_6FCC87C0(v65, v9, v10, &v70);
//    v29 = v70;
//    v30 = v70;
//    if (v70 >= v82)
//        v30 = v82;
//
//    if (!v28)
//    {
//        v67 = v30;
//        if (v30 <= 1)
//            v67 = 1;
//
//        v8 = v65;
//        v86 = v27 * v67;
//LABEL_71:
//        v84 = 0;
//        v70 = 0;
//        if (!ITEMS_CheckIfAutoBeltable(v71, v10))
//            a8 = 0;
//
//        while (1)
//        {
//            if (v70 && !a8)
//                return 0;
//
//            v73 = STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0);
//            v74 = STATLIST_UnitGetStatValue(v9, STAT_GOLDBANK, 0);
//            v36 = ITEMS_Duplicate(v8, v10, pNpc, 1);
//            if (!v36)
//            {
//                D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), -1, 0);
//                return 1;
//            }
//
//            if (a8 && ITEMS_CheckIfStackable(v36) && v67)
//                STATLIST_SetUnitStat(v36, STAT_QUANTITY, v67, 0);
//
//            v37 = STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0);
//            v38 = STATLIST_UnitGetStatValue(v9, STAT_GOLDBANK, 0);
//            v39 = nCost;
//            if (v37 + v38 < nCost)
//            {
//                D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 12, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), -1, 0);
//                return 0;
//            }
//
//            if (nCost > v37)
//            {
//                PLRTRADE_AddGold(v9, STAT_GOLD, -v37);
//                PLRTRADE_AddGold(v9, STAT_GOLDBANK, v37 - v86);
//            }
//            else
//            {
//                PLRTRADE_AddGold(v9, STAT_GOLD, -nCost);
//            }
//
//            UNITS_GetPlayerData(v9)->dwBoughtItemId = v36->dwUnitId;
//            UNITS_ChangeAnimMode(v36, 4);
//            v42 = v71;
//            v68 = v36->dwClassId;
//            if (ITEMS_CheckIfAutoBeltable(v71, v36))
//            {
//                v43 = sub_6FC48940(v65, v9, v36->dwUnitId, 0, 1, &v75);
//                v84 = v43;
//                if (v43)
//                    goto LABEL_117;
//
//                a8 = 0;
//            }
//            else
//            {
//                v43 = v84;
//            }
//
//            if (!v43)
//                break;
//
//LABEL_117:
//            sub_6FCC7E20(v65, pNpc, v72, v9, v69);
//            ITEMS_SetItemFlag(v36, IFLAG_TARGET, 1);
//            D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 0, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), 2, 1);
//            v10 = v72;
//            v8 = v65;
//            v70 = 1;
//        }
//
//        v85 = 1;
//        v44 = INVENTORY_GetLeftHandWeapon(v42);
//        v45 = UNITS_GetWeaponClass(v9);
//        if (v68 == dword_6FD4DD80 && v44 && v45 != 7)
//            v85 = 0;
//
//        if (v68 == dword_6FD4DD84)
//        {
//            if (!v44)
//                goto LABEL_111;
//
//            if (v45 != 1)
//                v85 = 0;
//        }
//
//        if (v44)
//        {
//            if (!ITEMS_CheckItemTypeId(v36, 45))
//                goto LABEL_113;
//
//            goto LABEL_115;
//        }
//
//LABEL_111:
//        if (v68 != dword_6FD4DD80 && v68 != dword_6FD4DD84)
//        {
//LABEL_113:
//            if (v85)
//            {
//                v84 = sub_6FC4A9B0(v65, v9, v36, 0);
//                if (v84)
//                    goto LABEL_117;
//            }
//        }
//
//LABEL_115:
//        if (v70 || (ITEMS_SetInvPage(v36, 0), (v84 = D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, v65, v9, v36->dwUnitId, 0, 0, 1, 1, 0)) == 0))
//        {
//            v48 = STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0);
//            if (v73 != v48)
//            {
//                v76 = v73 - STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0);
//                PLRTRADE_AddGold(v9, STAT_GOLD, v76);
//            }
//
//            v50 = STATLIST_UnitGetStatValue(v9, STAT_GOLDBANK, 0);
//            if (v74 != v50)
//            {
//                v77 = v74 - STATLIST_UnitGetStatValue(v9, STAT_GOLDBANK, 0);
//                PLRTRADE_AddGold(v9, STAT_GOLDBANK, v77);
//            }
//
//            ITEMS_RemoveFromAllPlayers(v65, v36);
//            if (!v70)
//            {
//                D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__), 0x2Au, 10, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), -1, 0);
//                return 0;
//            }
//
//            return 0;
//        }
//        goto LABEL_117;
//    }
//
//    if (v70 >= v82)
//        v29 = v82;
//
//    v83 = v29;
//    v64 = 0;
//    if (v29 > 0)
//    {
//        v31 = v87 * v29;
//        v88 = STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0);
//        if (v88 + STATLIST_UnitGetStatValue(v9, STAT_GOLDBANK, 0) < v87 * v83)
//        {
//            v64 = 0;
//            v62 = -1;
//            v60 = STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0);
//            v58 = 12;
//            v56 = 2076;
//        }
//        else
//        {
//            if (v31 > v88)
//            {
//                PLRTRADE_AddGold(v9, STAT_GOLD, -v88);
//                PLRTRADE_AddGold(v9, STAT_GOLDBANK, v88 - v31);
//            }
//            else
//            {
//                PLRTRADE_AddGold(v9, STAT_GOLD, -v31);
//            }
//
//            sub_6FCC7E20(v65, pNpc, v10, v9, v69);
//            STATLIST_AddUnitStat(v28, STAT_QUANTITY, v83, 0);
//            v33 = STATLIST_UnitGetStatValue(v28, STAT_QUANTITY, 0);
//            v34 = SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__);
//            D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(v34, v28, 1, STAT_QUANTITY, v33, v35, v64);
//            v62 = 5;
//            v60 = STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0);
//            v58 = 0;
//            v56 = 2085;
//        }
//    }
//    else
//    {
//        v62 = -1;
//        v60 = STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0);
//        v58 = 12;
//        v56 = 2071;
//    }
//
//    D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(v9, __FILE__, __LINE__v56), 0x2Au, v58, STATLIST_UnitGetStatValue(v9, STAT_GOLD, 0), v62, v64);
//    return 0;
}

//D2Game.0x6FCC92A0
int32_t __fastcall D2GAME_NPC_BuyItemHandler_6FCC92A0(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcUnitId, int32_t nItemId, int32_t nItemMode, uint16_t nTransactionType, int32_t nCost, int32_t bMultibuy)
{
    D2_ASSERT(pGame);

    D2UnitStrc* pInteractUnit = SUNIT_GetInteractUnit(pGame, pPlayer);
    D2UnitStrc* pNpc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nNpcUnitId);
    if (pNpc && pInteractUnit == pNpc)
    {
        return sub_6FCC88B0(pGame, pPlayer, pNpc, nItemId, nItemMode, nTransactionType, nCost, bMultibuy);
    }

    const int32_t nGold = STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0);
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2Au, 9, nGold, -1, 0);
    return 1;
}

//D2Game.0x6FCC9350
void __fastcall D2GAME_NPC_ResurrectMerc_6FCC9350(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcUnitId)
{
    D2UnitStrc* pInteractUnit = SUNIT_GetInteractUnit(pGame, pPlayer);
    D2UnitStrc* pNpc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nNpcUnitId);
    if (!pNpc || pInteractUnit != pNpc)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        return D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
    }

    switch (pNpc->dwClassId)
    {
    case MONSTER_KASHYA:
    case MONSTER_GREIZ:
    case MONSTER_ASHEARA:
    case MONSTER_TYRAEL2:
    case MONSTER_QUAL_KEHK:
        break;
    default:
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        return D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
    }
    }

    D2UnitStrc* pHireling = sub_6FC7E8B0(pGame, pPlayer, 7, 1);
    if (!pHireling)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        return D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
    }

    const int32_t nHirelingResurrectionCost = MONSTERS_GetHirelingResurrectionCost(pHireling);
    const int32_t nGold = STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0);
    if (nGold + STATLIST_UnitGetStatValue(pPlayer, STAT_GOLDBANK, 0) < nHirelingResurrectionCost)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        return D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2Au, 12, nGold, -1, 0);
    }

    if (nHirelingResurrectionCost > nGold)
    {
        PLRTRADE_AddGold(pPlayer, STAT_GOLD, -nGold);
        PLRTRADE_AddGold(pPlayer, STAT_GOLDBANK, nGold - nHirelingResurrectionCost);
    }
    else
    {
        PLRTRADE_AddGold(pPlayer, STAT_GOLD, -nHirelingResurrectionCost);
    }

    pHireling->dwFlags &= 0xFFFEFFFFu;

    UNITS_ChangeAnimMode(pHireling, 1);

    STATLIST_SetUnitStat(pHireling, STAT_HITPOINTS, STATLIST_GetMaxLifeFromUnit(pHireling), 0);
    sub_6FCC84D0(pGame, pPlayer, pHireling);

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket0x9B_6FC3FB30(pClient, -1, 0);

    return D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2Au, 5, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), pHireling->dwUnitId, 0);
}

//D2Game.0x6FCC9540
void __fastcall D2GAME_NPC_HireMerc_6FCC9540(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcUnitId, uint16_t a4)
{
    D2UnitStrc* pInteractUnit = SUNIT_GetInteractUnit(pGame, pPlayer);
    D2UnitStrc* pNpc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nNpcUnitId);
    if (pNpc && pInteractUnit == pNpc)
    {
        sub_6FCC7FA0(pGame, pPlayer, pNpc, a4);
    }
    else
    {
        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
    }
}

//D2Game.0x6FCC95B0
int32_t __fastcall D2GAME_NPC_Repair_6FCC95B0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nNpcGUID, int32_t nItemGUID, int32_t nUnused, int32_t a6)
{
    D2_ASSERT(pGame);

    D2UnitStrc* pInteractUnit = SUNIT_GetInteractUnit(pGame, pUnit);
    D2UnitStrc* pNpc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nNpcGUID);
    if (!pNpc || pInteractUnit != pNpc)
    {
        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 0);
        return 1;
    }

    switch (pNpc->dwClassId)
    {
    case MONSTER_CHARSI:
    case MONSTER_FARA:
    case MONSTER_HRATLI:
    case MONSTER_HALBU:
    case MONSTER_LARZUK:
        break;
    default:
        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 0);
        return 1;
    }

    if (a6 < 0)
    {
        const int32_t nRepairCosts = ITEMS_GetAllRepairCosts(pGame, pUnit, pNpc->dwClassId, (D2C_Difficulties)pGame->nDifficulty, UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], nullptr);
        if (nRepairCosts)
        {
            const int32_t nGold = STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0);
            if (nGold + STATLIST_UnitGetStatValue(pUnit, STAT_GOLDBANK, 0) < nRepairCosts)
            {
                D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x2Au, 12, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 0);
                return 1;
            }

            if (nRepairCosts > nGold)
            {
                PLRTRADE_AddGold(pUnit, STAT_GOLD, -nGold);
                PLRTRADE_AddGold(pUnit, STAT_GOLDBANK, nGold - nRepairCosts);
            }
            else
            {
                PLRTRADE_AddGold(pUnit, STAT_GOLD, -nRepairCosts);
            }

            ITEMS_GetAllRepairCosts(pGame, pUnit, pNpc->dwClassId, (D2C_Difficulties)pGame->nDifficulty, UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], D2GAME_NPC_RepairItem_6FCC6970);
        }

        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x2Au, 2, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 1);
        return 0;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem || !ITEMS_IsInPlayersInventory(pUnit, pItem, nullptr))
    {
        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 0);
        return 3;
    }

    const int32_t nMaxDurability = STATLIST_GetMaxDurabilityFromUnit(pItem);
    const int32_t nDurability = STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0);
    if (!ITEMS_IsRepairable(pItem))
    {
        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 0);
        return 1;
    }

    if (!ITEMS_CheckIfStackable(pItem) || !ITEMS_CheckIfThrowable(pItem) || ITEMS_IsEthereal(pItem) || STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0) >= ITEMS_GetTotalMaxStack(pItem))
    {
        int32_t bHasChargedSkills = 0;
        if ((!ITEMS_HasUsedCharges(pItem, &bHasChargedSkills) || bHasChargedSkills) && (!nMaxDurability || nDurability >= nMaxDurability))
        {
            D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x2Au, 9, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 0);
            return 1;
        }
    }

    const int32_t nTransactionCosts = ITEMS_GetTransactionCost(pUnit, pItem, (D2C_Difficulties)pGame->nDifficulty, UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], pNpc->dwClassId, D2C_TransactionTypes::TRANSACTIONTYPE_REPAIR);
    const int32_t nGold = STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0);
    if (nGold + STATLIST_UnitGetStatValue(pUnit, STAT_GOLDBANK, 0) >= nTransactionCosts)
    {
        if (nTransactionCosts > nGold)
        {
            PLRTRADE_AddGold(pUnit, STAT_GOLD, -nGold);
            PLRTRADE_AddGold(pUnit, STAT_GOLDBANK, nGold - nTransactionCosts);
        }
        else
        {
            PLRTRADE_AddGold(pUnit, STAT_GOLD, -nTransactionCosts);
        }

        D2GAME_NPC_RepairItem_6FCC6970(pGame, pItem, pUnit);
        D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x2Au, 2, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 1);
        return 0;
    }

    if (nDurability < nMaxDurability)
    {
        const uint32_t nDivisor = (nTransactionCosts << 10) / (uint32_t)(nMaxDurability - nDurability);
        if (nGold > 0 && nDivisor < nGold << 10 && nDivisor > 0)
        {
            const uint32_t nRepairDurability = (nGold << 10) / nDivisor;
            if (!ITEMS_CheckItemFlag(pItem, IFLAG_BROKEN, __LINE__, __FILE__))
            {
                PLRTRADE_AddGold(pUnit, STAT_GOLD, -nGold);

                int32_t nNewDurability = nMaxDurability;
                if (nNewDurability > nRepairDurability + nDurability)
                {
                    nNewDurability = nRepairDurability + nDurability;
                }

                STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nNewDurability, 0);
                D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_DURABILITY, nNewDurability, 0);
                D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x2Au, 2, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 1);
                return 0;
            }
        }
    }

    D2GAME_SendPacket0x2A_6FC3F3B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x2Au, 12, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 0);
    return 0;
}

//D2Game.0x6FCC9C90
void __fastcall D2GAME_NPC_IdentifyAllItems_6FCC9C90(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcGUID)
{
    if (!pPlayer)
    {
        return;
    }

    D2UnitStrc* pInteractUnit = SUNIT_GetInteractUnit(pGame, pPlayer);
    D2UnitStrc* pNpc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nNpcGUID);
    if (!pNpc || pInteractUnit != pNpc)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        return D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2A, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
    }

    switch (pNpc->dwClassId)
    {
    case MONSTER_CAIN2:
    case MONSTER_CAIN3:
    case MONSTER_CAIN4:
    case MONSTER_CAIN5:
    case MONSTER_CAIN6:
    {
        const int32_t nUnidentifiedItems = ITEMS_GetNoOfUnidItems(pPlayer);
        if (!nUnidentifiedItems)
        {
            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
            return D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 42, 9, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
        }

        D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
        if (QUESTRECORD_GetQuestState(pPlayerData->pQuestData[pGame->nDifficulty], QUEST_A1Q4_CAIN, QFLAG_REWARDGRANTED) != 1 && QUESTRECORD_GetQuestState(pPlayerData->pQuestData[pGame->nDifficulty], QUEST_A1Q4_CAIN, QFLAG_REWARDPENDING) != 1)
        {
            const int32_t nIdentifyCost = 100 * nUnidentifiedItems;
            const int32_t nGold = STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0);
            if (nGold + STATLIST_UnitGetStatValue(pPlayer, STAT_GOLDBANK, 0) < nIdentifyCost)
            {
                D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
                return D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2A, 12, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
            }

            if (nIdentifyCost > nGold)
            {
                PLRTRADE_AddGold(pPlayer, STAT_GOLD, -nGold);
                PLRTRADE_AddGold(pPlayer, STAT_GOLDBANK, nGold - nIdentifyCost);
            }
            else
            {
                PLRTRADE_AddGold(pPlayer, STAT_GOLD, -nIdentifyCost);
            }
        }

        for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pPlayer->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
        {
            if (INVENTORY_UnitIsItem(pItem))
            {
                switch (INVENTORY_GetItemNodePage(pItem))
                {
                case 1:
                {
                    if ((ITEMS_GetInvPage(pItem) == 0 || ITEMS_GetInvPage(pItem) == 3) && !ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, __FILE__))
                    {
                        D2GAME_ITEMS_Identify_6FC49670(pGame, pPlayer, pItem);
                    }
                    break;
                }
                case 3:
                {
                    if (!ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, __FILE__))
                    {
                        D2GAME_ITEMS_Identify_6FC49670(pGame, pPlayer, pItem);
                    }
                    break;
                }
				default:
					break;
                }
            }

            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
            D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2A, 3, STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0), -1, 0);
        }
        break;
    }
	default:
		break;
    }
}

//D2Game.0x6FCC9F40
int32_t __fastcall NPC_HandleDialogMessage(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nType, int32_t nNpcGUID, int32_t nItemGUID)
{
    D2UnitStrc* pMonster = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nNpcGUID);
    if (!pPlayer || !pMonster || pMonster->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pMonster->dwClassId);
    if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT]) || !pMonster->pMonsterData || !pMonster->pMonsterData->pMonInteract)
    {
        return 0;
    }

    int32_t a4 = 0;
    if (MONSTERAI_GetInteractUnitCount(pMonster->pMonsterData->pMonInteract) == 1)
    {
        a4 = 1;
    }

    const int32_t nClassId = pMonster->dwClassId;
    switch (nType)
    {
    case 1:
    {
        switch (nClassId)
        {
        case MONSTER_GHEED:
        case MONSTER_AKARA:
        case MONSTER_CHARSI:
        case MONSTER_DROGNAN:
        case MONSTER_FARA:
        case MONSTER_ELZIX:
        case MONSTER_LYSANDER:
        case MONSTER_ASHEARA:
        case MONSTER_HRATLI:
        case MONSTER_ALKOR:
        case MONSTER_ORMUS:
        case MONSTER_HALBU:
        case MONSTER_JAMELLA:
        case MONSTER_LARZUK:
        case MONSTER_DREHYA:
        case MONSTER_MALAH:
            D2GAME_NPCS_SetInteractTrading_6FC61AF0(pMonster, pPlayer);
            D2GAME_STORES_CreateVendorCache_6FCC74F0(pGame, pPlayer, pMonster, a4, 0);
            return 0;
        default:
            return 0;
        }
    }
    case 2:
    {
        switch (nClassId)
        {
        case MONSTER_GHEED:
        case MONSTER_ELZIX:
        case MONSTER_ALKOR:
        case MONSTER_JAMELLA:
        case MONSTER_DREHYA:
        case MONSTER_NIHLATHAK:
            D2GAME_NPCS_SetInteractTrading_6FC61AF0(pMonster, pPlayer);
            D2GAME_STORES_CreateVendorCache_6FCC74F0(pGame, pPlayer, pMonster, a4, 1);
            return 0;
        default:
            return 0;
        }
    }
    case 3:
    {
        D2GAME_NPC_BuildHirelingList_6FCC6FF0(pGame, SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), pMonster, MONSTERAI_GetInteractUnitCount(pMonster->pMonsterData->pMonInteract) <= 1);
        return 0;
    }
    default:
        break;
    }

    switch (nClassId)
    {
    case MONSTER_CHARSI:
    {
        D2GSPacketSrv58 packet58 = {};
        packet58.nHeader = 0x58;
        packet58.nUnitId = nNpcGUID;

        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        if (!pPlayer->pInventory)
        {
            return 3;
        }

        D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pPlayer->pInventory);
        if (!pCursorItem || pCursorItem->dwUnitId != nItemGUID)
        {
            return 3;
        }

        D2UnitStrc* pInput = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A1Q3, QFLAG_REWARDPENDING))
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            return 0;
        }

        if (!pInput)
        {
            return 0;
        }

        if (!ITEMS_IsImbueable(pInput))
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            return 0;
        }

        D2ItemDropStrc itemDrop = {};
        ITEMS_FillItemDrop(pGame, &itemDrop, pInput);

        itemDrop.dwFlags2 |= (ITEMS_IsEthereal(pInput) != 0 ? 0x24 : 0x22);
        itemDrop.wItemFormat = pGame->wItemFormat;

        bool bInputIsPersonalized = false;
        char szName[16] = {};
        if (ITEMS_CheckIfFlagIsSet(ITEMS_GetItemFlags(pInput), IFLAG_PERSONALIZED))
        {
            bInputIsPersonalized = true;
            strcpy_s(szName, ITEMS_GetEarName(pInput));
        }

        if (!D2GAME_RemoveItemIfOnCursor_6FC49760(pGame, pPlayer, pInput))
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            return 0;
        }

        itemDrop.nQuality = ITEMQUAL_RARE;
        itemDrop.nItemLvl = ITEMS_GetItemLevelForNewItem(pPlayer, 0);
        if (itemDrop.nItemLvl > 5)
        {
            itemDrop.nItemLvl += 4;
        }

        D2UnitStrc* pOutput = D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
        if (!pOutput)
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            return 0;
        }

        D2GAME_NPC_RepairItem_6FCC6970(pGame, pOutput, 0);
        sub_6FC4BBB0(pGame, pOutput, pPlayer);
        ITEMS_SetInvPage(pOutput, INVPAGE_INVENTORY);

        if (bInputIsPersonalized)
        {
            ITEMS_SetItemFlag(pOutput, IFLAG_PERSONALIZED, 1);
            ITEMS_SetEarName(pOutput, szName);
        }

        if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pPlayer, pOutput->dwUnitId, 0, 0, 1, 1, 0))
        {
            UNITS_ChangeAnimMode(pOutput, 3);
            D2CoordStrc coords = {};
            UNITS_GetCoords(pPlayer, &coords);
            D2CoordStrc returnCoords = {};
            D2ActiveRoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pPlayer), &coords, &returnCoords, 1);
            D2GAME_DropItem_6FC52260(pGame, 0, pOutput, pRoom, returnCoords.nX, returnCoords.nY);
        }

        ACT1Q3_SetRewardGranted(pGame, pPlayer);
        packet58.unk0x05 = 6;
        D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);

        return 0;
    }
    case MONSTER_WARRIV1:
    {
        if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A1Q6, QFLAG_REWARDGRANTED))
        {
            return 0;
        }

        D2GAME_PlayerChangeAct_6FC867C0(pGame, pPlayer, LEVEL_LUTGHOLEIN, 0);
        QUESTS_ActChange_HirelingChangeAct(pGame, pPlayer, pMonster, LEVEL_LUTGHOLEIN, LEVEL_ROGUEENCAMPMENT);
        
        int16_t nWpNo = 0;
        if (WAYPOINTS_GetWaypointNoFromLevelId(LEVEL_LUTGHOLEIN, &nWpNo))
        {
            WAYPOINTS_ActivateWaypoint(UNITS_GetPlayerData(pPlayer)->pWaypointData[pGame->nDifficulty], nWpNo);
        }

        return 0;
    }
    case MONSTER_WARRIV2:
    {
        D2GAME_PlayerChangeAct_6FC867C0(pGame, pPlayer, LEVEL_ROGUEENCAMPMENT, 5);
        return 0;
    }
    case MONSTER_MESHIF1:
    {
        if (nType || !QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q6, QFLAG_REWARDGRANTED))
        {
            return 0;
        }

        D2GAME_PlayerChangeAct_6FC867C0(pGame, pPlayer, LEVEL_KURASTDOCKTOWN, 0);
        QUESTS_ActChange_HirelingChangeAct(pGame, pPlayer, pMonster, LEVEL_KURASTDOCKTOWN, LEVEL_LUTGHOLEIN);
        
        int16_t nWpNo = 0;
        if (WAYPOINTS_GetWaypointNoFromLevelId(LEVEL_KURASTDOCKTOWN, &nWpNo))
        {
            WAYPOINTS_ActivateWaypoint(UNITS_GetPlayerData(pPlayer)->pWaypointData[pGame->nDifficulty], nWpNo);
        }

        return 0;
    }
    case MONSTER_MESHIF2:
    {
        if (!nType)
        {
            D2GAME_PlayerChangeAct_6FC867C0(pGame, pPlayer, LEVEL_LUTGHOLEIN, 5);
        }
        return 0;
    }
    case MONSTER_TYRAEL2:
    {
        if (nType || !pGame->bExpansion || !QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A4Q2, QFLAG_REWARDGRANTED))
        {
            return 0;
        }

        D2GAME_PlayerChangeAct_6FC867C0(pGame, pPlayer, LEVEL_HARROGATH, 0);
        QUESTS_ActChange_HirelingChangeAct(pGame, pPlayer, pMonster, LEVEL_HARROGATH, LEVEL_THEPANDEMONIUMFORTRESS);
        
        int16_t nWpNo = 0;
        if (WAYPOINTS_GetWaypointNoFromLevelId(LEVEL_HARROGATH, &nWpNo))
        {
            WAYPOINTS_ActivateWaypoint(UNITS_GetPlayerData(pPlayer)->pWaypointData[pGame->nDifficulty], nWpNo);
        }

        return 0;
    }
    case MONSTER_LARZUK:
    {
        D2GSPacketSrv58 packet58 = {};
        packet58.nHeader = 0x58;
        packet58.nUnitId = nNpcGUID;

        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        if (!pPlayer->pInventory)
        {
            return 3;
        }

        D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pPlayer->pInventory);
        if (!pCursorItem || pCursorItem->dwUnitId != nItemGUID)
        {
            return 3;
        }

        D2UnitStrc* pInput = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q1, QFLAG_REWARDPENDING))
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            return 0;
        }

        if (!pInput)
        {
            return 0;
        }

        if (!ITEMS_IsSocketable(pInput))
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            return 0;
        }
        
        D2UnitStrc* pOutput = ITEMS_Duplicate(pGame, pInput, pPlayer, 1);     
        if (!pOutput)
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            return 0;
        }
        
        if (!D2GAME_RemoveItemIfOnCursor_6FC49760(pGame, pPlayer, pInput))
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            return 0;
        }

        ITEMS_SetItemFlag(pOutput, IFLAG_SOCKETED, 1);

        int32_t nSockets = ITEMS_GetMaxSockets(pOutput);
        const int32_t nQuality = ITEMS_GetItemQuality(pOutput);
        if (nQuality == ITEMQUAL_MAGIC)
        {
            if (nSockets > 2)
            {
                nSockets = 2;
            }

            nSockets = ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pOutput), nSockets) + 1;
        }
        else if (nQuality > ITEMQUAL_MAGIC && nQuality <= ITEMQUAL_TEMPERED && nSockets > 1)
        {
            nSockets = 1;
        }

        ITEMS_AddSockets(pOutput, nSockets);

        D2GAME_NPC_RepairItem_6FCC6970(pGame, pOutput, 0);
        sub_6FC4BBB0(pGame, pOutput, pPlayer);
        ITEMS_SetInvPage(pOutput, 0);

        if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pPlayer, pOutput->dwUnitId, 0, 0, 1, 1, 0))
        {
            UNITS_ChangeAnimMode(pOutput, IMODE_ONGROUND);
            D2CoordStrc coords = {};
            UNITS_GetCoords(pPlayer, &coords);
            D2CoordStrc returnCoords = {};
            D2ActiveRoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pPlayer), &coords, &returnCoords, 1);
            D2GAME_DropItem_6FC52260(pGame, 0, pOutput, pRoom, returnCoords.nX, returnCoords.nY);
        }

        ACT5Q1_SetRewardGranted(pGame, pPlayer);
        packet58.unk0x05 = 6;
        D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
        return 0;
    }
    case MONSTER_DREHYA:
    {
        D2GSPacketSrv58 packet58 = {};
        packet58.nHeader = 0x58;
        packet58.nUnitId = nNpcGUID;

        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        if (!pPlayer->pInventory)
        {
            return 3;
        }

        D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pPlayer->pInventory);
        if (!pCursorItem || pCursorItem->dwUnitId != nItemGUID)
        {
            return 3;
        }

        D2UnitStrc* pInput = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q4, QFLAG_REWARDPENDING))
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            return 0;
        }

        if (!pInput)
        {
            return 0;
        }

        if (!ITEMS_IsPersonalizable(pInput))
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            return 0;
        }

        D2UnitStrc* pOutput = ITEMS_Duplicate(pGame, pInput, pPlayer, 1);
        if (!pOutput)
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            // TODO: Missing return 0?
        }

        if (!D2GAME_RemoveItemIfOnCursor_6FC49760(pGame, pPlayer, pInput))
        {
            packet58.unk0x05 = 7;
            D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
            sub_6FC44030(pGame, pPlayer, nItemGUID);
            return 0;
        }

        D2GAME_NPC_RepairItem_6FCC6970(pGame, pOutput, 0);
        sub_6FC4BBB0(pGame, pOutput, pPlayer);
        ITEMS_SetInvPage(pOutput, INVPAGE_INVENTORY);

        if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pPlayer, (pOutput ? pOutput->dwUnitId : -1), 0, 0, 1, 1, 0))
        {
            UNITS_ChangeAnimMode(pOutput, IMODE_ONGROUND);
            D2CoordStrc coords = {};
            UNITS_GetCoords(pPlayer, &coords);
            D2CoordStrc returnCoords = {};
            D2ActiveRoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pPlayer), &coords, &returnCoords, 1);
            D2GAME_DropItem_6FC52260(pGame, 0, pOutput, pRoom, returnCoords.nX, returnCoords.nY);
        }

        ITEMS_SetItemFlag(pOutput, IFLAG_PERSONALIZED, 1);
        ITEMS_SetEarName(pOutput, UNITS_GetPlayerData(pPlayer)->szName);
        ACT5Q4_SetRewardGranted(pGame, pPlayer);

        packet58.unk0x05 = 6;
        D2GAME_SendPacket0x58_6FC3F520(pClient, &packet58);
        return 0;
    }
    case MONSTER_CAIN6:
    {
        if (!nType)
        {
            D2GAME_PlayerChangeAct_6FC867C0(pGame, pPlayer, LEVEL_THEPANDEMONIUMFORTRESS, 5);
        }
        return 0;
    }
	default:
		return 0;
    }

}

//D2Game.0x6FCCA990
void __fastcall D2GAME_NPC_IdentifyBoughtItem_6FCCA990(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nItemGUID)
{
    if (UNITS_GetPlayerData(pPlayer)->dwBoughtItemId != nItemGUID)
    {
        return;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem || ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, __FILE__))
    {
        return;
    }

    D2GAME_ITEMS_Identify_6FC49670(pGame, pPlayer, pItem);
}

//D2Game.0x6FCCA9F0
void __fastcall D2GAME_STORES_FillGamble_6FCCA9F0(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pUnit, D2NpcRecordStrc* pNpcRecord)
{
    static int32_t gnRingItemId = 0;
    static int32_t gnAmuItemId = 0;

    D2GambleDataTbl* pGambleDataTbl = DATATBLS_GetGambleDataTables();
    if (!pGambleDataTbl)
    {
        return;
    }

    D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
    if (!pDifficultyLevelsTxtRecord)
    {
        return;
    }

    const uint32_t nHighQualityChance = pDifficultyLevelsTxtRecord->dwGambleUniq + pDifficultyLevelsTxtRecord->dwGambleSet + pDifficultyLevelsTxtRecord->dwGambleRare;

    if (!gnRingItemId)
    {
        DATATBLS_GetItemRecordFromItemCode(' nir', &gnRingItemId);
    }

    if (!gnAmuItemId)
    {
        DATATBLS_GetItemRecordFromItemCode(' uma', &gnAmuItemId);
    }

    D2NpcGambleStrc* pGamble = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2NpcGambleStrc);
    if (!pGamble)
    {
        return;
    }

    pGamble->pInventory = INVENTORY_AllocInventory(pGame->pMemoryPool, 0);
    pGamble->dwGUID = pUnit ? pUnit->dwUnitId : -1;
    pGamble->pNext = pNpcRecord->pGamble;
    pNpcRecord->pGamble = pGamble;

    const int32_t nLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
    D2SeedStrc* pSeed = SUNITPROXY_GetSeedFromNpcControl(pGame);
    
    int32_t nCounter = 0;
    do
    {
        int32_t nItemLevel = nLevel + ITEMS_RollRandomNumber(pSeed) % 10 - 5;
        if (nItemLevel < 5)
        {
            nItemLevel = 5;
        }
        else if (nItemLevel > 99)
        {
            nItemLevel = 99;
        }

        int32_t nItemId = pGambleDataTbl->pGambleSelection[ITEMS_RollLimitedRandomNumber(pSeed, pGambleDataTbl->pGambleChooseLimit[nItemLevel])];
        if (!pGame->bExpansion)
        {
            D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);
            if (!pItemsTxtRecord)
            {
                return;
            }

            if (pItemsTxtRecord->wVersion >= 100u)
            {
                continue;
            }
        }

        if (nCounter < 2)
        {
            if (nCounter)
            {
                nItemId = gnAmuItemId;
            }
            else
            {
                nItemId = gnRingItemId;
            }
        }

        if (pGame->bExpansion)
        {
            D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);
            if (pItemsTxtRecord && pItemsTxtRecord->dwUberCode && pItemsTxtRecord->dwUberCode != '    ')
            {
                int32_t nUberItemId = 0;
                D2ItemsTxt* pUberItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwUberCode, &nUberItemId);
                if (pUberItemsTxtRecord)
                {
                    const uint32_t nUberItemChance = pDifficultyLevelsTxtRecord->dwGambleUber * (nItemLevel - pUberItemsTxtRecord->nLevel) + 1;
                    if (nUberItemChance > 0)
                    {
                        if (ITEMS_RollLimitedRandomNumber(pSeed, 10000) < nUberItemChance)
                        {
                            nItemId = nUberItemId;
                        }
                        else
                        {
                            if (pItemsTxtRecord->dwUltraCode && pItemsTxtRecord->dwUltraCode != '    ')
                            {
                                int32_t nUltraItemId = 0;
                                D2ItemsTxt* pUltraItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwUltraCode, &nUltraItemId);
                                if (pUltraItemsTxtRecord)
                                {
                                    const uint32_t nUltraItemChance = pDifficultyLevelsTxtRecord->dwGambleUltra * (nItemLevel - pUltraItemsTxtRecord->nLevel) + 1;
                                    if (nUltraItemChance > 0 && (ITEMS_RollRandomNumber(pSeed) % 10000) < nUltraItemChance)
                                    {
                                        nItemId = nUltraItemId;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        int32_t nItemQuality = ITEMQUAL_MAGIC;
        if (nHighQualityChance > 0)
        {
            const uint32_t nQualityRoll = ITEMS_RollRandomNumber(pSeed) % 100000;
            if (nQualityRoll < nHighQualityChance)
            {
                if (nQualityRoll >= pDifficultyLevelsTxtRecord->dwGambleUniq)
                {
                    nItemQuality = (nQualityRoll >= (pDifficultyLevelsTxtRecord->dwGambleUniq + pDifficultyLevelsTxtRecord->dwGambleSet)) ? ITEMQUAL_RARE : ITEMQUAL_SET;
                }
                else
                {
                    nItemQuality = ITEMQUAL_UNIQUE;
                }
            }
        }

        ++nCounter;

        D2UnitStrc* pItem = D2GAME_CreateItemUnit_6FC501A0(pNpc, nItemId, pGame, 4u, nItemQuality, 0, 1u, nItemLevel, 0, 0, 0);
        if (pItem)
        {
            ITEMS_SetInvPage(pItem, 0);
            D2GAME_NPC_RepairItem_6FCC6970(pGame, pItem, 0);
            ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 0);

            if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pNpc, pItem->dwUnitId, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 1, 0, pGamble->pInventory))
            {
                ITEMS_RemoveFromAllPlayers(pGame, pItem);
                return;
            }
        }
    }
    while (nCounter < 14);
}

//D2Game.0x6FCCAE20
void __fastcall D2GAME_STORES_CreateVendorCache_6FCCAE20(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, int32_t bGamble)
{
    if (!pNpc)
    {
        return;
    }

    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);
    if (!pNpcRecord)
    {
        return;
    }

    if (bGamble)
    {
        if (pNpcRecord->bGambleInit)
        {
            SUNITPROXY_GetVendorChain(pGame, pNpcRecord, pPlayer)->field_4 = 1;

            const int32_t nPlayerGUID = pPlayer ? pPlayer->dwUnitId : -1;
            int32_t bHasGamble = 0;
            for (D2NpcGambleStrc* pNpcGamble = pNpcRecord->pGamble; pNpcGamble; pNpcGamble = pNpcGamble->pNext)
            {
                if (pNpcGamble->dwGUID == nPlayerGUID)
                {
                    bHasGamble = 1;
                }
            }

            if (!bHasGamble)
            {
                D2GAME_STORES_FillGamble_6FCCA9F0(pGame, pNpc, pPlayer, pNpcRecord);
            }
        }
    }
    else
    {
        if (pNpcRecord->npcTrade.bTrader)
        {
            SUNITPROXY_GetVendorChain(pGame, pNpcRecord, pPlayer)->field_4 = 0;

            if (!pNpcRecord->npcTrade.bVendorInit)
            {
                D2GAME_NPC_FillStoreInventory_6FCC7100(pGame, pPlayer, pNpc, &pNpcRecord->npcTrade);
                pNpcRecord->npcTrade.bVendorInit = 1;
            }

            if (!pNpcRecord->npcTrade.bHireInit)
            {
                const int32_t nNpcClassId = pNpc->dwClassId;
                switch (pNpc->dwClassId)
                {
                case MONSTER_KASHYA:
                case MONSTER_GREIZ:
                case MONSTER_ASHEARA:
                case MONSTER_QUAL_KEHK:
                    D2GAME_NPC_FirstFn_6FCC67D0(pGame, nNpcClassId, pNpcRecord);
                    break;
				default:
					break;
                }

                pNpcRecord->npcTrade.bHireInit = 1;
            }
        }
    }
}

//D2Game.0x6FCCAF30
int32_t __fastcall D2GAME_NPC_RemoveStates_6FCCAF30(D2UnitStrc* pUnit)
{
    int32_t bStateRemoved = 0;
    for (int32_t nState = 0; nState < sgptDataTables->nStatesTxtRecordCount; ++nState)
    {
        if (STATES_CheckState(pUnit, nState) && STATES_CheckStateMaskCurableByStateId(nState))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
            if (pStatList)
            {
                D2Common_10474(pUnit, pStatList);
                STATLIST_FreeStatList(pStatList);
                bStateRemoved = 1;
            }
        }
    }

    return bStateRemoved;
}

//D2Game.0x6FCCAFA0
void __fastcall SUNITNPC_PetIterate_Heal(D2GameStrc* pGame, D2UnitStrc* a2, D2UnitStrc* pUnit, void* a4)
{
    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
    if (STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) < nMaxHp)
    {
        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nMaxHp, 0);
        *(uint8_t*)a4 = 1;
    }

    for (int32_t nState = 0; nState < sgptDataTables->nStatesTxtRecordCount; ++nState)
    {
        if (STATES_CheckState(pUnit, nState) && STATES_CheckStateMaskCurableByStateId(nState))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
            if (pStatList)
            {
                D2Common_10474(pUnit, pStatList);
                STATLIST_FreeStatList(pStatList);
                *(uint8_t*)a4 = 1;
            }
        }
    }

    D2StatListStrc* pStatListPoison = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_POISON);
    if (pStatListPoison)
    {
        D2Common_10474(pUnit, pStatListPoison);
        STATLIST_FreeStatList(pStatListPoison);
        *(uint8_t*)a4 = 1;
    }

    D2StatListStrc* pStatListFreeze = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_FREEZE);
    if (pStatListFreeze)
    {
        D2Common_10474(pUnit, pStatListFreeze);
        STATLIST_FreeStatList(pStatListFreeze);
        *(uint8_t*)a4 = 1;
    }
}

//D2Game.0x6FCCB080
void __fastcall D2GAME_NPC_HealPlayer_6FCCB080(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pNpc)
{
    if (SUNIT_GetInteractUnit(pGame, pUnit) != pNpc)
    {
        return;
    }

    int32_t bPlaySound = 0;

    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
    if (STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) < nMaxHp)
    {
        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nMaxHp, 0);
        D2GAME_UpdateAttribute_6FC822D0(pUnit, STAT_HITPOINTS, nMaxHp, pUnit);
        bPlaySound = 1;
    }

    const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pUnit);
    if (STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0) < nMaxMana)
    {
        STATLIST_SetUnitStat(pUnit, STAT_MANA, nMaxMana, 0);
        D2GAME_UpdateAttribute_6FC822D0(pUnit, STAT_MANA, nMaxMana, pUnit);
        bPlaySound = 1;
    }

    const int32_t nMaxStamina = STATLIST_GetMaxStaminaFromUnit(pUnit);
    if (STATLIST_UnitGetStatValue(pUnit, STAT_STAMINA, 0) < nMaxStamina)
    {
        STATLIST_SetUnitStat(pUnit, STAT_STAMINA, nMaxStamina, 0);
        D2GAME_UpdateAttribute_6FC822D0(pUnit, STAT_STAMINA, nMaxStamina, pUnit);
        bPlaySound = 1;
    }

    D2StatListStrc* pStatListPoison = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_POISON);
    if (pStatListPoison)
    {
        D2Common_10474(pUnit, pStatListPoison);
        STATLIST_FreeStatList(pStatListPoison);
        bPlaySound = 1;
    }

    D2StatListStrc* pStatListFreeze = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_FREEZE);
    if (pStatListFreeze)
    {
        D2Common_10474(pUnit, pStatListFreeze);
        STATLIST_FreeStatList(pStatListFreeze);
        bPlaySound = 1;
    }

    for (int32_t nState = 0; nState < sgptDataTables->nStatesTxtRecordCount; ++nState)
    {
        if (STATES_CheckState(pUnit, nState) && STATES_CheckStateMaskCurableByStateId(nState))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
            if (pStatList)
            {
                D2Common_10474(pUnit, pStatList);
                STATLIST_FreeStatList(pStatList);
                bPlaySound = 1;
            }
        }
    }

    PLAYERPETS_IteratePets(pGame, pUnit, SUNITNPC_PetIterate_Heal, &bPlaySound);

    if (bPlaySound)
    {
        SUNIT_AttachSound(pNpc, 10u, 0);
    }
}

//D2Game.0x6FCCB220
void __fastcall D2GAME_NPC_Heal_6FCCB220(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pNpc)
{
    if (pNpc)
    {
        gnNpcGUID_6FD4DC48 = pNpc->dwUnitId;

        switch (pNpc->dwClassId)
        {
        case MONSTER_AKARA:
        case MONSTER_ATMA:
        case MONSTER_FARA:
        case MONSTER_ORMUS:
        case MONSTER_JAMELLA:
        case MONSTER_MALAH:
            D2GAME_NPC_HealPlayer_6FCCB080(pGame, pUnit, pNpc);
            break;
		default:
			break;
        }
    }
    else
    {
        gnNpcGUID_6FD4DC48 = -1;
    }
}

//D2Game.0x6FCCB280
void __fastcall D2GAME_NPC_PurchaseHeal_6FCCB280(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nNpcGUID)
{
    if (!pUnit)
    {
        return;
    }

    D2UnitStrc* pNpc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nNpcGUID);
    D2UnitStrc* pInteractUnit = SUNIT_GetInteractUnit(pGame, pUnit);
    if (!pNpc || pInteractUnit != pNpc)
    {
        return;
    }

    int32_t nStatRestored = 0;

    if (STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) != STATLIST_GetMaxLifeFromUnit(pUnit))
    {
        nStatRestored = 1;
    }

    int32_t nHealingCost = UNITS_GetHealingCost(pUnit);
    if (!nHealingCost)
    {
        nHealingCost = 1;
    }

    if (STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0) != STATLIST_GetMaxManaFromUnit(pUnit))
    {
        nStatRestored = 1;
    }

    if (STATLIST_UnitGetStatValue(pUnit, STAT_STAMINA, 0) == STATLIST_GetMaxStaminaFromUnit(pUnit) && !nStatRestored)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        return D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2A, 14, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 0);
    }

    const int32_t nGold = STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0);
    if (nGold + STATLIST_UnitGetStatValue(pUnit, STAT_GOLDBANK, 0) < nHealingCost)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        return D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2A, 12, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 0);
    }

    if (nHealingCost > nGold)
    {
        PLRTRADE_AddGold(pUnit, STAT_GOLD, -nGold);
        PLRTRADE_AddGold(pUnit, STAT_GOLDBANK, nGold - nHealingCost);
    }
    else
    {
        PLRTRADE_AddGold(pUnit, STAT_GOLD, -nHealingCost);
    }

    D2GAME_NPC_HealPlayer_6FCCB080(pGame, pUnit, pNpc);

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    D2GAME_SendPacket0x2A_6FC3F3B0(pClient, 0x2A, 6, STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0), -1, 0);
}

//D2Game.0x6FCCB4D0
void __fastcall D2GAME_NPC_ResetInteract_6FCCB4D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc)
{
    SUNIT_ResetInteractInfo(pPlayer);
    if (pNpc && pNpc->dwUnitType == UNIT_MONSTER && pNpc->pMonsterData)
    {
        MONSTERAI_RemoveInteractInfoFor(pGame, pPlayer, pNpc->pMonsterData->pMonInteract);
    }
    else
    {
        MONSTERAI_RemoveInteractInfoFor(pGame, pPlayer, 0);
    }
}

//D2Game.0x6FCCB520
void __fastcall D2GAME_NPC_AssignMercenary_6FCCB520(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMonster)
{
    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromClassId(pGame, nMonster, &nUnused);
    if (!pNpcRecord || !pNpcRecord->pMercData)
    {
        return;
    }

    if (!pGame->bExpansion || !sub_6FC7E8B0(pGame, pPlayer, 7, 1))
    {
        if (!pGame->bExpansion && sub_6FC7E8B0(pGame, pPlayer, 7, 0))
        {
            return;
        }

        D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty(pGame->bExpansion, nMonster, pGame->nDifficulty, 0);
        D2_ASSERT(pHirelingTxtRecord);

        for (int32_t i = 0; i < pHirelingTxtRecord->wNameLast - pHirelingTxtRecord->wNameFirst + 1; ++i)
        {
            if (pNpcRecord->pMercData[i].bHired != 1 && pNpcRecord->pMercData[i].bAvailable == 1)
            {
                D2MercDataStrc* pMercDataRecord = &pNpcRecord->pMercData[i];

                D2GSPacketSrv50 packet50 = {};
                packet50.nHeader = 0x50u;
                packet50.nQuestId = 2;
                packet50.HirelingPayload.nHireling = pNpcRecord->pMercData[i].nMercName;

                pNpcRecord->pMercData[i].bHired = 1;

                D2GAME_SendPacket0x50_6FC3F440(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), &packet50);

                D2UnitStrc* pMerc = sub_6FC68D70(pGame, pPlayer, pHirelingTxtRecord->dwClass, MONMODE_NEUTRAL, 4, 0);
                if (!pMerc)
                {
                    pMerc = sub_6FC68D70(pGame, pPlayer, pHirelingTxtRecord->dwClass, MONMODE_NEUTRAL, 6, 0);
                    if (!pMerc)
                    {
                        pMerc = sub_6FC68D70(pGame, pPlayer, pHirelingTxtRecord->dwClass, MONMODE_NEUTRAL, 12, 0);
                    }
                }

                if (!pMerc)
                {
                    return;
                }

                sub_6FC61270(pGame, pPlayer, pMerc, -1, pMercDataRecord, 0);
                break;
            }
        }
    }

    D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty(pGame->bExpansion, nMonster, 0, 0);
    D2_ASSERT(pHirelingTxtRecord);

    for (int32_t i = 0; i < pHirelingTxtRecord->wNameLast - pHirelingTxtRecord->wNameFirst + 1; ++i)
    {
        if (pNpcRecord->pMercData[i].bAvailable && !pNpcRecord->pMercData[i].bHired)
        {
            return;
        }
    }

    D2_FREE_POOL(pGame->pMemoryPool, pNpcRecord->pMercData);

    pNpcRecord->npcTrade.bHireInit = 0;
    pNpcRecord->pMercData = nullptr;

    D2GAME_NPC_FirstFn_6FCC67D0(pGame, nMonster, pNpcRecord);
}

//D2Game.0x6FCCB7D0
int32_t __fastcall D2GAME_NPC_IsItemInNpcInventory_6FCCB7D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, D2UnitStrc* pItem, int32_t a4)
{
    int32_t nUnused = 0;
    D2NpcRecordStrc* pNpcRecord = SUNITPROXY_GetNpcRecordFromUnit(pGame, pNpc, &nUnused);
    if (!pNpcRecord)
    {
        return 1;
    }

    D2NpcVendorChainStrc* pNpcVendorChain = SUNITPROXY_GetVendorChain(pGame, pNpcRecord, pPlayer);
    if (!pNpcVendorChain || !pNpcVendorChain->field_4)
    {
        if (ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, __FILE__))
        {
            return 1;
        }

        return 0;
    }

    if (a4)
    {
        return 1;
    }

    const int32_t nPlayerGUID = pPlayer ? pPlayer->dwUnitId : -1;
    for (D2NpcGambleStrc* pGamble = pNpcRecord->pGamble; pGamble; pGamble = pGamble->pNext)
    {
        if (pGamble->dwGUID == nPlayerGUID)
        {
            for (D2UnitStrc* pInventoryItem = INVENTORY_GetFirstItem(pGamble->pInventory); pInventoryItem; pInventoryItem = INVENTORY_GetNextItem(pInventoryItem))
            {
                if (INVENTORY_UnitIsItem(pInventoryItem) == pItem)
                {
                    return 1;
                }
            }
            break;
        }
    }

    return 0;
}
