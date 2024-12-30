#include "PLAYER/Player.h"

#include <algorithm>

#include <Fog.h>

#include <D2Chat.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <DataTbls/ObjectsIds.h>
#include <Path/PathMisc.h>
#include <Path/Step.h>

#include "AI/AiGeneral.h"
#include "GAME/Arena.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterAI.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/PlayerList.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlrIntro.h"
#include "PLAYER/PlrModes.h"
#include "PLAYER/PlrSave2.h"
#include "PLAYER/PlrTrade.h"
#include "QUESTS/ACT5/A5Q5.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitNpc.h"


#pragma pack(push, 1)
struct D2RosterIterateStrc
{
    int32_t bFlag;
    D2UnitStrc* pUnit;
};
#pragma pack(pop)


int32_t gnVirtualPlayerCount_6FD4DC1C;


//D2Game.0x6FC7B550
void __fastcall PLAYER_Create(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPlayerGUID)
{
    D2_ASSERT(pPlayer);

    pPlayer->dwFlags |= UNITFLAG_ISVALIDTARGET | UNITFLAG_CANBEATTACKED | UNITFLAG_TARGETABLE;

    pPlayer->dwUnitId = nPlayerGUID;

    INVENTORY_AllocInventory(nullptr, pPlayer);

    pPlayer->pSkills = SKILLS_AllocSkillList(pGame->pMemoryPool);
    SKILLS_InitSkillList(pPlayer);

    STATLIST_AllocStatListEx(pPlayer, 0, D2GAME_ITEMMODE_ServerStatlistCallback_6FC41910, pGame);
    UNITS_AllocPlayerData(pPlayer);

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (pPlayerData)
    {
        for (int32_t i = 0; i < 3; ++i)
        {
            pPlayerData->pPlayerIntro[i] = PLRINTRO_Create(pGame);
        }
        pPlayerData->dwBoughtItemId = 0;
    }

    SUNIT_InitClientInPlayerData(pPlayer, nullptr);
    D2GAME_DeleteTimersOnUnit_6FC34A30(pGame, pPlayer);
    ARENA_AllocArenaUnit(pGame, pPlayer);
    PLAYERPETS_AllocPetList(pGame, pPlayer);
    sub_6FCBDD30(pPlayer, 2u, 1);
}

//D2Game.0x6FC7B630
void __fastcall PLAYER_Destroy(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    PLAYERPETS_FreePetsFromPlayerData(pGame, pPlayer);
    PARTY_LeaveParty(pGame, pPlayer);
    PLAYERLIST_FreePlayerLists(pGame, pPlayer);
    SUNITDMG_FreeAttackerCombatList(pGame, pPlayer);

    for (D2CorpseStrc* pCorpse = INVENTORY_GetFirstCorpse(pPlayer->pInventory); pCorpse; pCorpse = INVENTORY_GetNextCorpse(pCorpse))
    {
        D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, INVENTORY_GetUnitGUIDFromCorpse(pCorpse));
        if (pUnit)
        {
            D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
            if (pRoom)
            {
                DUNGEON_AllocDrlgDelete(pRoom, pUnit->dwUnitType, pUnit->dwUnitId);
            }
            SUNIT_RemoveUnit(pGame, pUnit);
        }
    }

    SUNIT_RemoveAllItemsFromInventory(pGame, pPlayer);

    INVENTORY_FreeInventory(pPlayer->pInventory);
    ARENA_FreeArenaUnit(pGame, pPlayer);

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (pPlayerData)
    {
        // TODO: v11
        D2UnkPlayerDataStrc* v11 = pPlayerData->unk0x98;
        while (v11)
        {
            D2UnkPlayerDataStrc* pNext = v11->pNext;
            D2_FREE_POOL(pGame->pMemoryPool, v11);
            v11 = pNext;
        }
        pPlayerData->unk0x98 = nullptr;

        for (int32_t i = 0; i < 3; ++i)
        {
            PLRINTRO_Destroy(pGame, pPlayerData->pPlayerIntro[i]);
            pPlayerData->pPlayerIntro[i] = nullptr;
        }
    }

    UNITS_FreePlayerData(pGame->pMemoryPool, pPlayer);

    if (pPlayer->pHoverText)
    {
        CHAT_FreeHoverMsg(pGame->pMemoryPool, pPlayer->pHoverText);
    }

    sub_6FC349F0(pGame, pPlayer);
}

//D2Game.0x6FC7B750
void __fastcall PLAYER_RemoveAllPlayers(D2GameStrc* pGame)
{
    for (int32_t i = 0; i < 128; ++i)
    {
        D2UnitStrc* pPlayer = pGame->pUnitList[UNIT_PLAYER][i];
        while (pPlayer)
        {
            D2UnitStrc* pNext = SUNIT_GetNextUnitFromList(pPlayer);
            SUNIT_RemoveUnit(pGame, pPlayer);
            pPlayer = pNext;
        }
    }
}

//D2Game.0x6FC7B7A0
void __fastcall sub_6FC7B7A0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY)
{
    sub_6FC817D0(pGame, pUnit, nullptr, PLRMODE_NEUTRAL, nX, nY, 1);
    EVENT_SetEvent(pGame, pUnit, EVENTTYPE_STATREGEN, pGame->dwGameFrame + 1, 0, 0);
    EVENT_SetEvent(pGame, pUnit, EVENTTYPE_DELAYEDPORTAL, pGame->dwGameFrame + 250, 0, 0);
}

//D2Game.0x6FC7B800
void __fastcall PLAYER_CreateStartItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2CharStatsTxt* pCharStatsTxt, int32_t nCount, D2CharItemStrc* pCharItem)
{
    int32_t nItemId = 0;
    if (!DATATBLS_GetItemRecordFromItemCode(pCharItem->dwItemCode, &nItemId))
    {
        return;
    }

    for (int32_t i = 0; i < pCharItem->nItemCount; ++i)
    {
        D2UnitStrc* pItem = nullptr;
        if (pPlayer)
        {
            pItem = D2GAME_CreateItemUnit_6FC501A0(pPlayer, nItemId, pGame, 4u, ITEMQUAL_NORMAL, 1u, 1u, ITEMS_GetItemLevelForNewItem(pPlayer, 0), 0, 0, 0);
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitStateAndFlag(pItem, 0, 0x40u);
            if (pStatList)
            {
                D2Common_10474(pItem, pStatList);
                STATLIST_FreeStatList(pStatList);
            }

            if (!nCount && pCharStatsTxt->wStartSkill >= 0 && pCharStatsTxt->wStartSkill < sgptDataTables->nSkillsTxtRecordCount)
            {
                D2StatListStrc* pStartItemStatList = STATLIST_GetStatListFromUnitStateAndFlag(pItem, 0, 0x40u);
                if (!pStartItemStatList)
                {
                    if (pItem)
                    {
                        pStartItemStatList = STATLIST_AllocStatList(pItem->pMemoryPool, 0x40u, 0, UNIT_ITEM, pItem->dwUnitId);
                    }
                    else
                    {
                        pStartItemStatList = STATLIST_AllocStatList(nullptr, 0x40u, 0, UNIT_ITEM, -1);
                    }

                    D2COMMON_10475_PostStatToStatList(pItem, pStartItemStatList, 1);
                }

                STATLIST_SetStatIfListIsValid(pStartItemStatList, STAT_ITEM_SINGLESKILL, 1, pCharStatsTxt->wStartSkill);
            }

            if (ITEMS_CheckIfStackable(pItem))
            {
                STATLIST_SetUnitStat(pItem, STAT_QUANTITY, ITEMS_GetTotalMaxStack(pItem), 0);
            }
        }

        ITEMS_SetItemFlag(pItem, IFLAG_STARTITEM, 1);
        ITEMS_SetBodyLocation(pItem, pCharItem->nBodyLoc);

        if (ITEMS_CheckIfBeltable(pItem))
        {
            D2_ASSERT(pItem);
            D2_ASSERT(pPlayer);

            int32_t nUnused = 0;
            if (!sub_6FC48940(pGame, pPlayer, pItem->dwUnitId, CLIENTS_GetUnitX(pItem), 1, &nUnused))
            {
                PLAYER_PlaceItemInInventory(pItem, pPlayer, pGame, 1);
            }
        }
        else if (pCharItem->nBodyLoc)
        {
            D2_ASSERT(pItem);
            D2_ASSERT(pPlayer);

            int32_t nUnused = 0;
            if (!sub_6FC45060(pGame, pPlayer, pItem->dwUnitId, ITEMS_GetBodyLocation(pItem), 1, &nUnused))
            {
                PLAYER_PlaceItemInInventory(pItem, pPlayer, pGame, 1);
            }

            if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOW_QUIVER))
            {
                STATLIST_SetUnitStat(pItem, STAT_QUANTITY, 250, 0);
            }
            else
            {
                STATLIST_SetUnitStat(pItem, STAT_DURABILITY, STATLIST_GetMaxDurabilityFromUnit(pItem), 0);
            }
        }
        else
        {
            PLAYER_PlaceItemInInventory(pItem, pPlayer, pGame, 1);
            if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOW_QUIVER))
            {
                STATLIST_SetUnitStat(pItem, STAT_QUANTITY, 250, 0);
            }
            else
            {
                STATLIST_SetUnitStat(pItem, STAT_DURABILITY, STATLIST_GetMaxDurabilityFromUnit(pItem), 0);
            }

            if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOW_QUIVER))
            {
                STATLIST_SetUnitStat(pItem, STAT_QUANTITY, 100, 0);
            }
        }
    }
}

//D2Game.0x6FC7BB50
void __fastcall PLAYER_PlaceItemInInventory(D2UnitStrc* pItem, D2UnitStrc* pPlayer, D2GameStrc* pGame, int32_t a8)
{
    D2_ASSERT(pItem);
    D2_ASSERT(pPlayer);

    ITEMS_SetInvPage(pItem, INVPAGE_INVENTORY);

    D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pPlayer, pItem->dwUnitId, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), a8, 1, nullptr);
}

//D2Game.0x6FC7BC30
void __fastcall PLAYER_CreateStartItemsFromCharStatsTxt(D2UnitStrc* pPlayer, D2GameStrc* pGame, void* pUnused)
{
    if (!pPlayer)
    {
        return;
    }

    D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pPlayer->dwClassId);
    if (!pCharStatsTxtRecord)
    {
        return;
    }

    for (int32_t i = 0; i < std::size(pCharStatsTxtRecord->pItems); ++i)
    {
        PLAYER_CreateStartItem(pGame, pPlayer, pCharStatsTxtRecord, i, &pCharStatsTxtRecord->pItems[i]);
    }
}

//D2Game.0x6FC7BC90
void __fastcall PLAYER_SynchronizeItemsToClient(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    D2_ASSERT(pUnit->pInventory);

    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
    {
        D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(pItem);
        if (INVENTORY_GetItemNodePage(pItem) == 3)
        {
            D2GAME_SendP0x9D_ItemAction_Equip_6FC3EA50(pClient, pUnit, pCheckedItem, 0);
        }
        else if (pUnit == CLIENTS_GetPlayerFromClient(pClient, 0) && INVENTORY_GetItemNodePage(pItem) == 1)
        {
            D2GAME_SendP0x9C_ItemAction_PutInContainer_6FC3E980(pClient, pUnit, pCheckedItem, 0);
        }
        else if (pUnit == CLIENTS_GetPlayerFromClient(pClient, 0) && INVENTORY_GetItemNodePage(pItem) == 2)
        {
            D2GAME_SendP0x9C_ItemAction_PutInBelt_6FC3EB90(pClient, pUnit, pCheckedItem, 0);
        }
    }
}

//D2Game.0x6FC7BD50
int32_t __fastcall PLAYER_IsBusy(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }

    int32_t nUnitType = 0;
    int32_t nUnitGUID = 0;
    if (SUNIT_GetInteractInfo(pUnit, &nUnitType, &nUnitGUID) || INVENTORY_GetCursorItem(pUnit->pInventory))
    {
        return 1;
    }

    return UNITS_GetPlayerData(pUnit)->bBusy;
}

//D2Game.0x6FC7BDB0
void __fastcall PLAYER_ResetBusyState(D2UnitStrc* pUnit)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    D2_ASSERT(pPlayerData->bBusy != 0);

    pPlayerData->bBusy = 0;
}

//D2Game.0x6FC7BDF0
void __fastcall PLAYER_StopInteractions(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    int32_t nUnitType = 0;
    int32_t nUnitGUID = 0;
    if (!SUNIT_GetInteractInfo(pUnit, &nUnitType, &nUnitGUID))
    {
        return ACT5Q5_OnPlayerDied(pGame, pUnit);
    }

    D2UnitStrc* pInteractUnit = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
    if (!pInteractUnit)
    {
        return ACT5Q5_OnPlayerDied(pGame, pUnit);
    }

    switch (nUnitType)
    {
    case UNIT_PLAYER:
    {
        sub_6FC92EE0(pGame, pUnit);
        break;
    }
    case UNIT_MONSTER:
    {
        D2GAME_NPC_ResetInteract_6FCCB4D0(pGame, pUnit, pInteractUnit);
        break;
    }
    case UNIT_OBJECT:
    {
        sub_6FC79B50(pGame, pUnit, pInteractUnit);
        break;
    }
    case UNIT_ITEM:
    {
        if (DATATBLS_GetItemsTxtRecord(pInteractUnit->dwClassId)->dwCode == ' xob')
        {
            sub_6FC92EF0(pGame, pUnit);
        }
        break;
    }
    default:
        break;
    }

    return ACT5Q5_OnPlayerDied(pGame, pUnit);
}

//D2Game.0x6FC7BEC0
void __fastcall sub_6FC7BEC0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    int32_t nUnitType = 0;
    int32_t nUnitGUID = 0;
    if (!SUNIT_GetInteractInfo(pUnit, &nUnitType, &nUnitGUID))
    {
        return;
    }

    D2UnitStrc* pInteractUnit = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
    if (!pInteractUnit)
    {
        return;
    }

    switch (nUnitType)
    {
    case UNIT_PLAYER:
    {
        sub_6FC91250(pGame, pUnit, 6, 0);
        return;
    }
    case UNIT_MONSTER:
    {
        D2GAME_NPC_ResetInteract_6FCCB4D0(pGame, pUnit, pInteractUnit);
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        sub_6FC3F340(pClient, 0x62u, UNIT_MONSTER, pInteractUnit->dwUnitId, 0);
        return;
    }
    case UNIT_OBJECT:
    {
        sub_6FC79B50(pGame, pUnit, pInteractUnit);
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        sub_6FC3F340(pClient, 0x62u, UNIT_OBJECT, pInteractUnit->dwUnitId, 0);
        return;
    }
    case UNIT_ITEM:
    {
        if (DATATBLS_GetItemsTxtRecord(pInteractUnit->dwClassId)->dwCode == ' xob')
        {
            sub_6FC92EF0(pGame, pUnit);
            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
            sub_6FC3F340(pClient, 0x62u, UNIT_ITEM, pInteractUnit->dwUnitId, 0);
        }
        return;
    }
    default:
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        sub_6FC3F340(pClient, 0x62u, 6, 0, 0);
        return;
    }
    }
}

//1.10f: D2Game.0x6FC7BFC0
//1.13c: D2Game.0x6FC57B10
void __fastcall sub_6FC7BFC0(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nPlayerGUID, D2CoordStrc* pCoord)
{
    D2_ASSERT(pRoom);

    COLLISION_GetFreeCoordinatesEx(pRoom, pCoord, 2, 0x9C19u, 4);

    D2UnitStrc* pPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nPlayerGUID);
    if (!pPlayer)
    {
        return;
    }

    D2Common_10231(pPlayer->pDynamicPath, pPlayer, pRoom, pCoord->nX, pCoord->nY);
    pPlayer->dwFlags |= UNITFLAG_TARGETABLE;

    UNITS_ChangeAnimMode(pPlayer, PLRMODE_DEAD);
    pPlayer->dwFlags &= ~UNITFLAG_ISVALIDTARGET;

    STATES_ToggleState(pPlayer, STATE_PLAYERBODY, 1);
    pPlayer->dwFlags |= UNITFLAG_INITSEEDSET;

    D2GAME_EVENTS_Delete_6FC34840(pGame, pPlayer, EVENTTYPE_STATREGEN, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pPlayer, EVENTTYPE_TRAP, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pPlayer, EVENTTYPE_FREEHOVER, 0);

    const int32_t nOwnerGUID = INVENTORY_GetOwnerId(pPlayer->pInventory);
    if (nOwnerGUID != -1)
    {
        D2GAME_SendPacket0x8E_CorpseAssign_ToAllPlayers_6FC3F9B0(pGame, nOwnerGUID, nPlayerGUID, 1u);
    }
}

//D2Game.0x6FC7C0C0
void __fastcall PLAYER_SetUniqueIdInPlayerData(D2UnitStrc* pPlayer, int32_t nUnitGUID)
{
    D2_ASSERT(pPlayer);

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    pPlayerData->dwUniqueId = nUnitGUID;
}

//D2Game.0x6FC7C120
int32_t __fastcall PLAYER_GetUniqueIdFromPlayerData(D2UnitStrc* pPlayer)
{
    D2_ASSERT(pPlayer);

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    return pPlayerData->dwUniqueId;
}

//D2Game.0x6FC7C170
void __fastcall sub_6FC7C170(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2_ASSERT(pPlayer);

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    D2UnitStrc* pPortal = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pPlayerData->dwUniqueId);
    if (!pPortal || pPortal->dwClassId != OBJECT_TOWN_PORTAL)
    {
        return;
    }

    ACT5Q5_OnPortalClosed(pGame, pPortal);

    D2UnitStrc* pOwner = SUNIT_GetPortalOwner(pGame, pPortal);
    D2ActiveRoomStrc* pPortalRoom = UNITS_GetRoom(pPortal);
    if (pPortalRoom)
    {
        DUNGEON_AllocDrlgDelete(pPortalRoom, UNIT_OBJECT, pPortal->dwUnitId);
    }

    SUNIT_RemoveUnit(pGame, pPortal);
    DUNGEON_ToggleHasPortalFlag(pPortalRoom, 1);

    if (!pOwner)
    {
        return;
    }

    ACT5Q5_OnPortalClosed(pGame, pOwner);

    D2ActiveRoomStrc* pOwnerRoom = UNITS_GetRoom(pOwner);
    if (pOwnerRoom)
    {
        DUNGEON_AllocDrlgDelete(pOwnerRoom, UNIT_OBJECT, pOwner->dwUnitId);
    }

    SUNIT_RemoveUnit(pGame, pOwner);
    DUNGEON_ToggleHasPortalFlag(pOwnerRoom, 1);
}

//D2Game.0x6FC7C260
int32_t __fastcall sub_6FC7C260(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUnitGUID, uint32_t nValue)
{
    if (SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID) != pUnit)
    {
        return 0;
    }

    if (nValue > STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0) || nValue < 0 || nValue > UNITS_GetInventoryGoldLimit(pUnit))
    {
        return 0;
    }

    if (!nValue)
    {
        return 1;
    }

    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pUnit, &pCoord);

    uint32_t nGoldValue = nValue;
    if (nValue > 2'000'000'000)
    {
        nGoldValue = 2'000'000'000;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    D2UnitStrc* pItem = sub_6FC4FCA0(pGame, pRoom, &pCoord, 4, 1, pUnit);
    if (!pItem)
    {
        return 1;
    }

    D2GAME_SetStatOrResetGold_6FC7CA70(pItem, STAT_GOLD, nGoldValue);

    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        UNITS_StoreOwner(pItem, pUnit);
    }

    const uint32_t nDiff = STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0) - nValue;
    D2GAME_SetStatOrResetGold_6FC7CA70(pUnit, STAT_GOLD, nDiff);

    return 1;
}

//D2Game.0x6FC7C3A0
int32_t __fastcall sub_6FC7C3A0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* a3)
{
    int32_t nUnitType = 0;
    int32_t nUnitGUID = 0;
    if (SUNIT_GetInteractInfo(pPlayer, &nUnitType, &nUnitGUID))
    {
        D2UnitStrc* pInteractUnit = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
        if (!pInteractUnit)
        {
            SUNIT_ResetInteractInfo(pPlayer);
            return 0;
        }

        if (nUnitType == UNIT_PLAYER)
        {
            return sub_6FC93430(pGame, pPlayer, a3);
        }
        else if (nUnitType == UNIT_MONSTER)
        {
            return sub_6FC61F20(pInteractUnit, pPlayer);
        }
    }
    else
    {
        if (UNITS_GetPlayerData(pPlayer)->bBusy)
        {
            return 0;
        }
    }

    return sub_6FC93430(pGame, pPlayer, a3);
}

//D2Game.0x6FC7C450
int32_t __fastcall sub_6FC7C450(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nHotkeyId, int16_t nSkillId, char a4, int32_t nFlags)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    if (!pClient)
    {
        return 0;
    }

    CLIENTS_SetSkillHotKey(pClient, nHotkeyId, nSkillId, a4, nFlags);
    return 1;
}

//D2Game.0x6FC7C490 (#10059)
void __fastcall PLAYER_SetVirtualPlayerCount(int32_t nPlayers)
{
    if (nPlayers >= 0 && nPlayers <= 8)
    {
        gnVirtualPlayerCount_6FD4DC1C = nPlayers;
    }
}

//D2Game.0x6FC7C4A0
int32_t __fastcall PLAYER_GetPlayerCount(D2GameStrc* pGame)
{
    int32_t nLivingPlayers = 0;
    SUNIT_IterateLivingPlayers(pGame, PLAYER_CountLivingPlayers, &nLivingPlayers);

    if (pGame->nGameType == 0 || pGame->nGameType > 3 || gnVirtualPlayerCount_6FD4DC1C <= nLivingPlayers)
    {
        return nLivingPlayers;
    }

    return gnVirtualPlayerCount_6FD4DC1C;
}

//D2Game.0x6FC7C4E0
void __fastcall PLAYER_CountLivingPlayers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pLivingPlayers)
{
    if (!SUNIT_IsDead(pUnit))
    {
        ++*((int32_t*)pLivingPlayers);
    }
}

//D2Game.0x6FC7C500
void __fastcall PLAYER_ApplyDeathPenalty(D2GameStrc* pGame, D2UnitStrc* pDefender, D2UnitStrc* pAttacker)
{
    const int32_t nDefenderLevel = STATLIST_UnitGetStatValue(pDefender, STAT_LEVEL, 0);
    const int32_t nDefenderInventoryGold = STATLIST_UnitGetStatValue(pDefender, STAT_GOLD, 0);
    const int32_t nDefenderBankGold = STATLIST_UnitGetStatValue(pDefender, STAT_GOLDBANK, 0);
    const int32_t nDefenderTotalGold = nDefenderBankGold + nDefenderInventoryGold;

    const int32_t nPenaltyPercentage = std::min(nDefenderLevel, 20);

    int32_t nGoldPenalty = nDefenderTotalGold * nPenaltyPercentage / 100;
    
    int32_t bAttackedByAnotherPlayer = 0;
    if (pAttacker && pAttacker != pDefender)
    {
        if (pAttacker->dwUnitType == UNIT_PLAYER)
        {
            bAttackedByAnotherPlayer = 1;
        }
        else if (pAttacker->dwUnitType == UNIT_MONSTER)
        {
            D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pAttacker);
            if (pOwner && pOwner->dwUnitType == UNIT_PLAYER)
            {
                bAttackedByAnotherPlayer = 1;
            }
        }
    }

    if (pGame->nGameType == 3)
    {
        if (nGoldPenalty > nDefenderTotalGold - 500 * nDefenderLevel)
        {
            nGoldPenalty = std::max(nDefenderTotalGold - 500 * nDefenderLevel, 0);
        }

        nGoldPenalty = std::min(nGoldPenalty, nDefenderInventoryGold);
    }

    if (bAttackedByAnotherPlayer)
    {
        if (nGoldPenalty > nDefenderInventoryGold)
        {
            D2GAME_SetStatOrResetGold_6FC7CA70(pDefender, STAT_GOLDBANK, nDefenderTotalGold - nGoldPenalty);
            D2GAME_SetStatOrResetGold_6FC7CA70(pDefender, STAT_GOLD, nGoldPenalty);
        }

        if (pDefender)
        {
            sub_6FC7C260(pGame, pDefender, pDefender->dwUnitId, nGoldPenalty);
        }
        else
        {
            sub_6FC7C260(pGame, nullptr, -1, nGoldPenalty);
        }
    }
    else
    {
        if (nGoldPenalty > nDefenderInventoryGold)
        {
            if (pGame->nGameType != 3)
            {
                D2GAME_SetStatOrResetGold_6FC7CA70(pDefender, STAT_GOLDBANK, nDefenderInventoryGold + nDefenderBankGold - nGoldPenalty);
            }
        }
        else
        {
            if (pDefender)
            {
                sub_6FC7C260(pGame, pDefender, pDefender->dwUnitId, nDefenderInventoryGold - nGoldPenalty);
            }
            else
            {
                sub_6FC7C260(pGame, nullptr, -1, nDefenderInventoryGold - nGoldPenalty);
            }
        }

        D2GAME_SetStatOrResetGold_6FC7CA70(pDefender, STAT_GOLD, 0);
    }

    D2GAME_SetStatOrResetGold_6FC7CA70(pDefender, STAT_GOLDLOST, nGoldPenalty);

    if (pAttacker)
    {
        if (pAttacker->dwUnitType == UNIT_PLAYER)
        {
            return;
        }

        D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pAttacker);
        if (pOwner && pOwner->dwUnitType == UNIT_PLAYER)
        {
            return;
        }
    }

    if (nDefenderLevel > 1)
    {
        const int32_t nClassId = pDefender ? pDefender->dwClassId : -1;
        const int32_t nPreviousExpThreshold = DATATBLS_GetLevelThreshold(nClassId, nDefenderLevel - 1);
        const int32_t nCurrentExpThreshold = DATATBLS_GetLevelThreshold(nClassId, nDefenderLevel);
        D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
        int32_t nExperiencePenalty = (nCurrentExpThreshold - nPreviousExpThreshold) * pDifficultyLevelsTxtRecord->dwDeathExpPenalty / 100;
        if (nExperiencePenalty)
        {
            int32_t nNewExperienceValue = STATLIST_UnitGetStatValue(pDefender, STAT_EXPERIENCE, 0) - nExperiencePenalty;
            if (nNewExperienceValue <= nPreviousExpThreshold)
            {
                nExperiencePenalty += nNewExperienceValue - nPreviousExpThreshold;
                nExperiencePenalty = std::max(nExperiencePenalty, 0);
                nNewExperienceValue = nPreviousExpThreshold + 1;
            }

            CLIENTS_SetExperienceLoss(SUNIT_GetClientFromPlayer(pDefender, __FILE__, __LINE__), nExperiencePenalty);
            STATLIST_SetUnitStat(pDefender, STAT_EXPERIENCE, nNewExperienceValue, 0);
        }
    }
}

//D2Game.0x6FC7C750
int32_t __fastcall D2GAME_IteratePlayers_6FC7C750(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pGame || !pUnit || pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }

    D2RosterIterateStrc rosterIterate = {};

    rosterIterate.bFlag = 0;
    rosterIterate.pUnit = pUnit;

    SUNIT_IterateLivingPlayers(pGame, sub_6FC7C790, &rosterIterate);
    return rosterIterate.bFlag;
}

//D2Game.0x6FC7C790
void __fastcall sub_6FC7C790(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArgs)
{
    D2RosterIterateStrc* pRosterIterate = (D2RosterIterateStrc*)pArgs;
    if (PLAYERLIST_CheckFlag(pRosterIterate->pUnit, pUnit, 8))
    {
        pRosterIterate->bFlag = 1;
    }
}

//D2Game.0x6FC7C7B0
D2UnitStrc* __fastcall sub_6FC7C7B0(D2UnitStrc* pUnit)
{
    if (!pUnit || !pUnit->pInventory)
    {
        return nullptr;
    }

    D2UnitStrc* pEquippedWeapon = nullptr;
	D2C_PlayerBodyLocs nBodyLoc = BODYLOC_NONE;
    int32_t bIsLeftHandItem = 0;
    INVENTORY_GetEquippedWeapon(pUnit->pInventory, &pEquippedWeapon, &nBodyLoc, &bIsLeftHandItem);

    if (!UNITS_CanDualWield(pUnit))
    {
        return pEquippedWeapon;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return pEquippedWeapon;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
    if (!pSkillsTxtRecord)
    {
        return pEquippedWeapon;
    }

    D2UnitStrc* pWeapon = nullptr;
    switch (pSkillsTxtRecord->nWeapSel)
    {
    case 2u:
    {
        pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, 5);
        D2UnitStrc* pTemp = INVENTORY_GetCompositItem(pUnit->pInventory, 6);
        if (!pWeapon || !ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_WEAPON) || pTemp && ITEMS_CheckItemTypeId(pTemp, ITEMTYPE_WEAPON) && SKILLS_GetFlags(pSkill) & 0x2000)
        {
            pWeapon = pTemp;
        }
        break;
    }
    case 3u:
    {
        if (((pUnit->dwSeqFrame >> 8) % 2) != 0)
        {
            pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, 6);
        }
        else
        {
            pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, 5);
        }
        break;
    }
    case 4u:
    {
        pEquippedWeapon = nullptr;
        break;
    }
    case 1u:
    {
        pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, 6);
        break;
    }
    default:
        pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, 5);
        if (!pWeapon || !ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_WEAPON))
        {
            pWeapon = INVENTORY_GetCompositItem(pUnit->pInventory, 6);
        }
        break;
    }

    if (ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_WEAPON) && ITEMS_CanBeEquipped(pWeapon))
    {
        return pWeapon;
    }

    return pEquippedWeapon;
}

//D2Game.0x6FC7C900
void __fastcall sub_6FC7C900(D2UnitStrc* pAttacker, int32_t a2)
{
    if (a2)
    {
        D2UnitStrc* pLeftHandWeapon = INVENTORY_GetLeftHandWeapon(pAttacker->pInventory);
        if (pLeftHandWeapon && STATLIST_GetOwner(pLeftHandWeapon, 0))
        {
            STATLIST_MergeStatLists(pAttacker, pLeftHandWeapon, 0);
        }

        D2UnitStrc* pSecondaryWeapon = INVENTORY_GetSecondaryWeapon(pAttacker->pInventory);
        if (pSecondaryWeapon && pSecondaryWeapon != pLeftHandWeapon && STATLIST_GetOwner(pSecondaryWeapon, 0))
        {
            STATLIST_MergeStatLists(pAttacker, pSecondaryWeapon, 0);
        }
    }

    if (UNITS_CanDualWield(pAttacker))
    {
        D2UnitStrc* pWeapon = sub_6FC7C7B0(pAttacker);
        D2UnitStrc* pLeftHandWeapon = INVENTORY_GetLeftHandWeapon(pAttacker->pInventory);
        if (pWeapon)
        {
            if (pWeapon == pLeftHandWeapon)
            {
                D2UnitStrc* pSecondaryWeapon = INVENTORY_GetSecondaryWeapon(pAttacker->pInventory);
                if (STATLIST_GetOwner(pLeftHandWeapon, 0))
                {
                    STATLIST_MergeStatLists(pAttacker, pLeftHandWeapon, 1);
                }

                if (pSecondaryWeapon && STATLIST_GetOwner(pSecondaryWeapon, 0))
                {
                    STATLIST_MergeStatLists(pAttacker, pSecondaryWeapon, 0);
                }
            }
            else
            {
                if (STATLIST_GetOwner(pWeapon, 0))
                {
                    STATLIST_MergeStatLists(pAttacker, pWeapon, 1);
                }

                if (pLeftHandWeapon && STATLIST_GetOwner(pLeftHandWeapon, 0))
                {
                    STATLIST_MergeStatLists(pAttacker, pLeftHandWeapon, 0);
                }
            }
        }
        else
        {
            if (pLeftHandWeapon)
            {
                if (STATLIST_GetOwner(pLeftHandWeapon, 0))
                {
                    STATLIST_MergeStatLists(pAttacker, pLeftHandWeapon, 1);
                }
            }
        }
    }
}

//D2Game.0x6FC7CA10
void __fastcall sub_6FC7CA10(D2UnitStrc* pTarget, int32_t a2)
{
    if (!a2)
    {
        return;
    }

    D2UnitStrc* pLeftHandWeapon = INVENTORY_GetLeftHandWeapon(pTarget->pInventory);
    if (pLeftHandWeapon && STATLIST_GetOwner(pLeftHandWeapon, 0))
    {
        STATLIST_MergeStatLists(pTarget, pLeftHandWeapon, 1);
    }

    D2UnitStrc* pSecondaryWeapon = INVENTORY_GetSecondaryWeapon(pTarget->pInventory);
    if (pSecondaryWeapon && pSecondaryWeapon != pLeftHandWeapon)
    {
        if (STATLIST_GetOwner(pSecondaryWeapon, 0))
        {
            STATLIST_MergeStatLists(pTarget, pSecondaryWeapon, 0);
        }
    }
}

//D2Game.0x6FC7CA70
void __fastcall D2GAME_SetStatOrResetGold_6FC7CA70(D2UnitStrc* pUnit, int32_t nStat, int32_t nValue)
{
    if (!pUnit)
    {
        return;
    }

    if (nValue < 0)
    {
        STATLIST_SetUnitStat(pUnit, nStat, 0, 0);
        return;
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        if (nStat == STAT_GOLD)
        {
            if (nValue > UNITS_GetInventoryGoldLimit(pUnit))
            {
                STATLIST_SetUnitStat(pUnit, STAT_GOLD, 0, 0);
                return;
            }
        }
        else if (nStat == STAT_GOLDBANK)
        {
            if (nValue > (int32_t)UNITS_GetStashGoldLimit(pUnit))
            {
                STATLIST_SetUnitStat(pUnit, STAT_GOLDBANK, 0, 0);
                return;
            }
        }
    }

    STATLIST_SetUnitStat(pUnit, nStat, nValue, 0);
}
