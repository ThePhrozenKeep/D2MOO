#include "INVENTORY/InvMode.h"

#include <algorithm>

#include <D2Items.h>
#include <D2Inventory.h>
#include <D2Monsters.h>

#include "GAME/Clients.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitNpc.h"


//D2Game.0x6FC40FB0
void __fastcall D2GAME_INVMODE_First_6FC40FB0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, D2InventoryStrc* pInventoryArg)
{
    D2InventoryStrc* pInventory = pUnit->pInventory;
    D2_ASSERT(pInventory);

    if (pInventoryArg)
    {
        pInventory = pInventoryArg;
    }

    D2InventoryNodeStrc* pInventoryNode = INVENTORY_GetTradeInventory(pInventory);
    if (!pInventoryNode)
    {
        return;
    }

    do
    {
        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, INVENTORY_GetItemGUIDFromNode(pInventoryNode));
        if (pItem)
        {
            sub_6FC410E0(pUnit, pItem, nullptr, pClient);

            if (pItem->dwFlagEx & UNITFLAGEX_HASINV)
            {
                D2_ASSERT(pItem->pInventory);

                for (D2InventoryNodeStrc* i = INVENTORY_GetTradeInventory(pItem->pInventory); i; i = INVENTORY_GetNextNode(i))
                {
                    D2UnitStrc* pNestedItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, INVENTORY_GetItemGUIDFromNode(i));
                    if (pNestedItem)
                    {
                        sub_6FC410E0(pUnit, pItem, pNestedItem, pClient);
                    }
                }
            }
        }

        pInventoryNode = INVENTORY_GetNextNode(pInventoryNode);
    }
    while (pInventoryNode);

    if (MONSTERS_GetHirelingTypeId(pUnit))
    {
        D2GAME_ITEMS_UpdateInventoryItems_6FC45050(pGame, pUnit, 0, 1);
    }
}

//D2Game.0x6FC410E0
int32_t __fastcall sub_6FC410E0(D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2UnitStrc* pUnused, D2ClientStrc* pClient)
{
    D2UnitStrc* pClientPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    D2GameStrc* pGame = CLIENTS_GetGame(pClient);
    D2UnitStrc* pInteractUnit = SUNIT_GetInteractUnit(pGame, pClientPlayer);

    if (pItem)
    {
        if (pItem->dwFlagEx & UNITFLAGEX_ISVENDORITEM)
        {
            if (pInteractUnit == pPlayer && D2GAME_NPC_IsItemInNpcInventory_6FCCB7D0(pGame, pClientPlayer, pInteractUnit, pItem, 0))
            {
                D2GAME_SendP0x9C_ItemAction_AddOrRemoveFromShop_6FC3F260(pClient, pPlayer, pItem, 56);
            }
            return 1;
        }

        if (pItem->dwFlagEx & UNITFLAGEX_ITEMINIT)
        {
            if (pInteractUnit == pPlayer && D2GAME_NPC_IsItemInNpcInventory_6FCCB7D0(pGame, pClientPlayer, pInteractUnit, pItem, 1))
            {
                D2GAME_SendP0x9C_ItemAction_AddOrRemoveFromShop_6FC3F260(pClient, pPlayer, pItem, 57);
            }
            return 1;
        }
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x40u) && pPlayer == pClientPlayer)
    {
        D2GAME_SendP0x9C_ItemAction_GroundToCursor_6FC3E870(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x80000u) && pPlayer == pClientPlayer)
    {
        D2GAME_SendP0x9D_ItemAction_Unknown0x16_6FC3F040(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if ((ITEMS_CheckItemCMDFlag(pItem, 2) || ITEMS_CheckItemCMDFlag(pItem, 0x80u)) && pPlayer == pClientPlayer)
    {
        D2GAME_SendP0x9C_ItemAction_PutInContainer_6FC3E980(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 4) && pPlayer == pClientPlayer)
    {
        D2GAME_UpdateClientItem_6FC3E9D0(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 8) || ITEMS_CheckItemCMDFlag(pItem, 0x200u))
    {
        D2GAME_SendP0x9D_ItemAction_Equip_6FC3EA50(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x10000u))
    {
        D2GAME_SendP0x9D_ItemAction_IndirectlySwapBodyItem_6FC3EA70(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x10u))
    {
        D2GAME_SendP0x9D_ItemAction_Unequip_6FC3EA90(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x20u))
    {
        D2GAME_SendP0x9D_ItemAction_SwapBodyItem_6FC3EAB0(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x100u) && pPlayer == pClientPlayer)
    {
        D2GAME_SendP0x9C_ItemAction_AddQuantity_6FC3EAF0(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x40000u) && pPlayer == pClientPlayer)
    {
        D2GAME_SendP0x9C_ItemAction_SwapInContainer_6FC3EB40(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if ((ITEMS_CheckItemCMDFlag(pItem, 0x400u) || ITEMS_CheckItemCMDFlag(pItem, 0x2000u)) && pPlayer == pClientPlayer)
    {
        D2GAME_SendP0x9C_ItemAction_PutInBelt_6FC3EB90(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x800u) && pPlayer == pClientPlayer)
    {
        D2GAME_SendP0x9C_ItemAction_RemoveFromBelt_6FC3EBE0(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x1000u) && pPlayer == pClientPlayer)
    {
        D2GAME_SendP0x9C_ItemAction_SwapInBelt_6FC3EC00(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x100000u) && pPlayer == pClientPlayer)
    {
        D2GAME_SendP0x9C_ItemAction_ToCursor_6FC3EEA0(pClient, pPlayer, pItem);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x200000u))
    {
        D2GAME_SendP0x9D_ItemAction_WeaponSwitch_6FC3EAD0(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x4000u))
    {
        D2GAME_SendP0x9D_ItemAction_AutoUnequip_6FC3EE20(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x8000u))
    {
        D2GAME_SendP0x9D_ItemAction_Unknown0x14_6FC3EEC0(pClient, pPlayer, pItem, 0);
        return 1;
    }

    if (ITEMS_CHECK_FLAG(pItem, IFLAG_BROKEN))
    {
        if (pPlayer == pClientPlayer || pItem && pItem->dwAnimMode == 1)
        {
            sub_6FC3EFC0(pClient, pPlayer, pItem, IFLAG_BROKEN, ITEMS_CHECK_FLAG(pItem,  IFLAG_BROKEN));
            return 1;
        }
    }
    else if (ITEMS_CHECK_FLAG(pItem, IFLAG_REPAIRED))
    {
        if (pPlayer == pClientPlayer || pItem && pItem->dwAnimMode == 1)
        {
            sub_6FC3EFC0(pClient, pPlayer, pItem, IFLAG_REPAIRED, ITEMS_CHECK_FLAG(pItem,  IFLAG_REPAIRED));
            return 1;
        }
    }
    else if (ITEMS_CHECK_FLAG(pItem, IFLAG_NEWITEM))
    {
        int32_t nAnimMode = 0;
        if (pItem)
        {
            nAnimMode = pItem->dwAnimMode;
        }

        if (pPlayer != pClientPlayer && (nAnimMode == IMODE_STORED || nAnimMode == IMODE_INBELT))
        {
            return 0;
        }

        if (nAnimMode != IMODE_STORED && nAnimMode != IMODE_EQUIP && nAnimMode != IMODE_INBELT)
        {
            return 0;
        }

        if (pPlayer != pClientPlayer || !ITEMS_CHECK_FLAG(pItem,  IFLAG_UNK2))
        {
            D2GAME_SendP0x9D_ItemAction_UpdateStats_6FC3EFA0(pClient, pPlayer, pItem, 0);
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC415F0
void __fastcall D2GAME_ResetSomeItemFlags_6FC415F0(D2GameStrc* pGame, D2UnitStrc* pItem)
{
    constexpr uint32_t nItemCmdFlags[21] =
    {
        0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x100, 0x40000, 0x400, 0x800, 0x1000, 0x2000, 0x200, 0x4000, 0x8000, 0x10000, 0x20000, 0x80000, 0x100000, 0x200000
    };

    constexpr uint32_t nItemFlags[8] =
    {
        IFLAG_QUANTITY, IFLAG_TARGET, IFLAG_DELETED, IFLAG_SWITCHOUT, IFLAG_SWITCHIN, IFLAG_NEWITEM, IFLAG_REPAIRED, IFLAG_UNK2
    };

    if (pItem)
    {
        pItem->dwFlagEx &= ~(UNITFLAGEX_ITEMINIT | UNITFLAGEX_ISVENDORITEM);
    }
    
    for (size_t i = 0; i < std::size(nItemCmdFlags); ++i)
    {
        ITEMS_SetItemCMDFlag(pItem, nItemCmdFlags[i], 0);
    }

    for (size_t i = 0; i < std::size(nItemFlags); ++i)
    {
        ITEMS_SetItemFlag(pItem, nItemFlags[i], 0);
    }
}

//D2Game.0x6FC41660
int32_t __fastcall sub_6FC41660(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (pUnit && ((pUnit->dwFlagEx & 0x400000) || (pUnit->dwFlagEx & 0x800000)))
    {
        return 1;
    }

    for (D2InventoryNodeStrc* pInventoryNode = INVENTORY_GetTradeInventory(pUnit->pInventory); pInventoryNode; pInventoryNode = INVENTORY_GetNextNode(pInventoryNode))
    {
        if (SUNIT_GetServerUnit(pGame, UNIT_ITEM, INVENTORY_GetItemGUIDFromNode(pInventoryNode)))
        {
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC416D0
void __fastcall D2GAME_INVMODE_Last_6FC416D0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit->pInventory)
    {
        return;
    }

    UNITS_RefreshInventory(pUnit, 0);

    for (D2InventoryNodeStrc* pInventoryNode = INVENTORY_GetTradeInventory(pUnit->pInventory); pInventoryNode; pInventoryNode = INVENTORY_GetNextNode(pInventoryNode))
    {
        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, INVENTORY_GetItemGUIDFromNode(pInventoryNode));
        if (pItem)
        {
            if (ITEMS_CheckItemCMDFlag(pItem, 0x10u) || ITEMS_CheckItemCMDFlag(pItem, 0x4000u))
            {
                ITEMS_SetBodyLocation(pItem, BODYLOC_NONE);
            }

            if (ITEMS_CheckItemCMDFlag(pItem, 0x20u) && ITEMS_CHECK_FLAG(pItem, IFLAG_SWITCHOUT))
            {
                ITEMS_SetBodyLocation(pItem, BODYLOC_NONE);
            }

            D2GAME_ResetSomeItemFlags_6FC415F0(pGame, pItem);

            if (pItem->dwFlagEx & UNITFLAGEX_HASINV)
            {
                UNITS_RefreshInventory(pItem, 0);

                D2_ASSERT(pItem->pInventory);

                for (D2InventoryNodeStrc* i = INVENTORY_GetTradeInventory(pItem->pInventory); i; i = INVENTORY_GetNextNode(i))
                {
                    D2UnitStrc* pNestedItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, INVENTORY_GetItemGUIDFromNode(i));
                    if (pNestedItem)
                    {
                        D2GAME_ResetSomeItemFlags_6FC415F0(pGame, pNestedItem);
                    }
                }

                INVENTORY_FreeTradeInventory(pItem->pInventory);
            }

            if (ITEMS_CheckItemCMDFlag(pItem, 1))
            {
                ITEMS_RemoveFromAllPlayers(pGame, pItem);
            }
        }
    }

    INVENTORY_FreeTradeInventory(pUnit->pInventory);
}
