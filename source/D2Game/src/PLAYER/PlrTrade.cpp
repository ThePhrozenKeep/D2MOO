#include "PLAYER/PlrTrade.h"

#include <algorithm>

#include <Fog.h>
#include <Storm.h>
#include <D2BitManip.h>

#include <D2DataTbls.h>
#include <DataTbls/ObjectsIds.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2ItemMods.h>
#include <D2StatList.h>


#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "INVENTORY/InvMode.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "ITEMS/ItemsMagic.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlrSave.h"
#include "QUESTS/ACT1/A1Q4.h"
#include "QUESTS/ACT2/A2Q2.h"
#include "QUESTS/ACT3/A3Q2.h"
#include "QUESTS/ACT4/A4Q2.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"


#pragma warning(disable: 28159)


int32_t dword_6FD4DC30;


//D2Game.0x6FC8F3D0
int32_t __fastcall OBJECTS_OperateFunction32_Bank(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pOp->nObjectIdx == OBJECT_STASH)
    {
        D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(pOp->pPlayer);
        if (pPlayerRoom && DUNGEON_IsRoomInTown(pPlayerRoom))
        {
            D2ActiveRoomStrc* pObjectRoom = UNITS_GetRoom(pObject);
            if (pObjectRoom && DUNGEON_IsRoomInTown(pObjectRoom))
            {
                SUNIT_SetInteractInfo(pOp->pPlayer, UNIT_OBJECT, pObject->dwUnitId);

                D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pOp->pPlayer, __FILE__, __LINE__);
                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(pClient, UPDATEUI_OPENSTASH);
                sub_6FC4B830(pOp->pGame, pOp->pPlayer);
            }
        }
    }

    return 1;
}

//D2Game.0x6FC8F450
int32_t __fastcall PLRTRADE_CheckCubeInput(D2GameStrc* pGame, D2InventoryStrc* pInventory, D2CubeMainTxt* pCubeMainTxt, int32_t nInputIndex, D2CubeItemStrc* pCubeItem, int32_t* a6)
{
    if (!pInventory)
    {
        return 0;
    }

    const uint16_t nInputFlags = pCubeMainTxt->pInputItem[nInputIndex].wInputFlags;
    if (!(nInputFlags & (CUBEFLAG_IN_ITEMCODE | CUBEFLAG_IN_USEANY)))
    {
        return 1;
    }

    int32_t nQuantity = pCubeMainTxt->pInputItem[nInputIndex].nQuantity;
    if (nQuantity <= 0)
    {
        nQuantity = 1;
    }

    D2ItemsTxt* pInputItemTxtRecord = nullptr;
    if (nInputFlags & CUBEFLAG_IN_USEANY && pCubeMainTxt->pInputItem[nInputIndex].wItem != uint16_t(-1))
    {
        pInputItemTxtRecord = DATATBLS_GetItemsTxtRecord(pCubeMainTxt->pInputItem[nInputIndex].wItem);
    }

    int32_t nItemsInCube = 0;
    int32_t nItemCountWithStacks = 0;
    int32_t bContainsStackableItem = 0;

    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
    {
        if (INVENTORY_UnitIsItem(pItem) && ITEMS_GetInvPage(pItem) == INVPAGE_CUBE)
        {
            ++nItemsInCube;

            D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
            if (pItemsTxtRecord)
            {
                const uint32_t dwItemCode = ITEMS_GetBaseCode(pItem);

                if (nInputFlags & CUBEFLAG_IN_USEANY)
                {
                    if (pCubeMainTxt->pInputItem[nInputIndex].wItem != uint16_t(-1))
                    {
                        if (nInputFlags & CUBEFLAG_IN_UPGRADED)
                        {
                            if (dwItemCode == pItemsTxtRecord->dwNormCode)
                            {
                                if (pInputItemTxtRecord->dwCode != pItemsTxtRecord->dwNormCode)
                                {
                                    continue;
                                }
                            }
                            else if (dwItemCode == pItemsTxtRecord->dwUberCode)
                            {
                                if (pInputItemTxtRecord->dwCode != pItemsTxtRecord->dwNormCode && pInputItemTxtRecord->dwCode != pItemsTxtRecord->dwUberCode)
                                {
                                    continue;
                                }
                            }
                            else if (dwItemCode == pItemsTxtRecord->dwUltraCode)
                            {
                                if (pInputItemTxtRecord->dwCode != pItemsTxtRecord->dwNormCode && pInputItemTxtRecord->dwCode != pItemsTxtRecord->dwUberCode && pInputItemTxtRecord->dwCode != pItemsTxtRecord->dwUltraCode)
                                {
                                    continue;
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else if (pCubeMainTxt->pInputItem[nInputIndex].wItem != pItem->dwClassId)
                        {
                            continue;
                        }
                    }
                }
                else if (nInputFlags & CUBEFLAG_IN_ITEMCODE && !ITEMS_CheckItemTypeId(pItem, pCubeMainTxt->pInputItem[nInputIndex].wItem))
                {
                    continue;
                }

                if (pCubeMainTxt->pInputItem[nInputIndex].nQuality && ITEMS_GetItemQuality(pItem) != pCubeMainTxt->pInputItem[nInputIndex].nQuality)
                {
                    //if (pCubeMainTxt->pInputItem[nInputIndex].nQuality == ITEMQUAL_TEMPERED)
                    //{
                    //    STATLIST_GetFullStatsCountFromUnit(pItem);
                    //}
                    continue;
                }

                if (!(nInputFlags & CUBEFLAG_IN_SPECIAL) || ITEMS_GetFileIndex(pItem) == pCubeMainTxt->pInputItem[nInputIndex].wItemID - 1)
                {
                    if (nInputFlags & CUBEFLAG_IN_NOSOCKET)
                    {
                        if (ITEMS_GetSockets(pItem))
                        {
                            continue;
                        }
                    }
                    else if (nInputFlags & CUBEFLAG_IN_SOCKETED)
                    {
                        if (!ITEMS_GetSockets(pItem))
                        {
                            continue;
                        }
                    }

                    if (nInputFlags & CUBEFLAG_IN_NOETHEREAL)
                    {
                        if (ITEMS_IsEthereal(pItem))
                        {
                            continue;
                        }
                    }
                    else if (nInputFlags & CUBEFLAG_IN_ETHEREAL)
                    {
                        if (!ITEMS_IsEthereal(pItem))
                        {
                            continue;
                        }
                    }

                    if (nInputFlags & CUBEFLAG_IN_NORMAL)
                    {
                        if (dwItemCode != pItemsTxtRecord->dwNormCode)
                        {
                            continue;
                        }
                    }
                    else if (nInputFlags & CUBEFLAG_IN_EXCEPTIONAL)
                    {
                        if (dwItemCode != pItemsTxtRecord->dwUberCode)
                        {
                            continue;
                        }
                    }
                    else if (nInputFlags & CUBEFLAG_IN_ELITE)
                    {
                        if (dwItemCode != pItemsTxtRecord->dwUltraCode)
                        {
                            continue;
                        }
                    }

                    if (nInputFlags & CUBEFLAG_IN_NORUNES && ITEMS_GetItemFlags(pItem) & IFLAG_RUNEWORD)
                    {
                        continue;
                    }
                    
                    if (pCubeMainTxt->nOp == 28 && pItemsTxtRecord->nQuest && pItemsTxtRecord->nQuestDiffCheck && STATLIST_UnitGetStatValue(pItem, 356, 0) < pGame->nDifficulty)
                    {
                        continue;
                    }

                    if (!nInputIndex && (pCubeMainTxt->pOutputItem[0].nType == 255 || pCubeMainTxt->pOutputItem[0].nType == 254) && pCubeMainTxt->pOutputItem[0].wItemFlags & CUBEFLAG_OUT_SOCKET && !ITEMS_GetMaxSockets(pItem))
                    {
                        continue;
                    }

                    if (nItemsInCube < 48 && a6[nItemsInCube - 1])
                    {
                        continue;
                    }

                    if (nInputIndex)
                    {
                        if (nItemsInCube < 48)
                        {
                            a6[nItemsInCube - 1] = 1;
                        }

                        ++nItemCountWithStacks;

                        if (ITEMS_CheckIfStackable(pItem))
                        {
                            bContainsStackableItem = 1;
                        }

                        continue;
                    }

                    if (pCubeMainTxt->nOp)
                    {
                        switch (pCubeMainTxt->nOp)
                        {
                        case 15u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_UnitGetStatValue(pItem, pCubeMainTxt->nParam, 0) < (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 16u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_UnitGetStatValue(pItem, pCubeMainTxt->nParam, 0) > (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 17u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_UnitGetStatValue(pItem, pCubeMainTxt->nParam, 0) == (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 18u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_UnitGetStatValue(pItem, pCubeMainTxt->nParam, 0) != (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 19u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_GetUnitBaseStat(pItem, pCubeMainTxt->nParam, 0) < (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 20u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_GetUnitBaseStat(pItem, pCubeMainTxt->nParam, 0) > (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 21u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_GetUnitBaseStat(pItem, pCubeMainTxt->nParam, 0) == (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 22u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_GetUnitBaseStat(pItem, pCubeMainTxt->nParam, 0) != (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 23u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_GetUnitStatBonus(pItem, pCubeMainTxt->nParam, 0) < (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 24u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_GetUnitStatBonus(pItem, pCubeMainTxt->nParam, 0) > (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 25u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_GetUnitStatBonus(pItem, pCubeMainTxt->nParam, 0) == (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 26u:
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pCubeMainTxt->nParam);
                            if (!pItemStatCostTxtRecord || STATLIST_GetUnitStatBonus(pItem, pCubeMainTxt->nParam, 0) != (pCubeMainTxt->nValue >> pItemStatCostTxtRecord->nValShift))
                            {
                                continue;
                            }
                            break;
                        }
                        case 27u:
                        {
                            if (ITEMS_GetFileIndex(pItem) == pCubeMainTxt->nValue)
                            {
                                continue;
                            }
                            break;
                        }
                        default:
                            break;
                        }
                    }


                    for (int32_t i = 0; i < 3; ++i)
                    {
                        pCubeItem[i].nItemLevel = ITEMS_GetItemLevel(pItem);

                        if (pCubeMainTxt->pOutputItem[0].wItemFlags & CUBEFLAG_OUT_COPYMODS)
                        {
                            pCubeItem[i].pItem = pItem;
                        }

                        bool bContinue = false;
                        if (pCubeMainTxt->pOutputItem[0].nType == 255)
                        {
                            pCubeItem[i].pItem = pItem;
                            pCubeItem[i].nClassId = pItem->dwClassId;
                            bContinue = true;
                        }
                        else if (pCubeMainTxt->pOutputItem[0].nType == 254)
                        {
                            pCubeItem[i].pItem = pItem;
                            pCubeItem[i].nClassId = -1;
                            bContinue = true;
                        }

                        if (bContinue)
                        {
                            if (pCubeMainTxt->pOutputItem[0].wItemFlags & 0x80)
                            {
                                if (pItemsTxtRecord->dwUberCode != '    ')
                                {
                                    const int32_t nClassId = FOG_GetLinkIndex(sgptDataTables->pItemsLinker, pItemsTxtRecord->dwUberCode, 0);
                                    if (nClassId >= 0)
                                    {
                                        D2ItemsTxt* pLinkItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nClassId);
                                        if (pLinkItemsTxtRecord && (pGame->bExpansion || pLinkItemsTxtRecord->wVersion < 100u))
                                        {
                                            pCubeItem[i].nClassId = nClassId;
                                        }
                                    }
                                }
                            }
                            else if (pCubeMainTxt->pOutputItem[0].wItemFlags & 0x100)
                            {
                                if (pItemsTxtRecord->dwUltraCode != '    ')
                                {
                                    const int32_t nClassId = FOG_GetLinkIndex(sgptDataTables->pItemsLinker, pItemsTxtRecord->dwUltraCode, 0);
                                    if (nClassId >= 0)
                                    {
                                        D2ItemsTxt* pLinkItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nClassId);
                                        if (pLinkItemsTxtRecord && (pGame->bExpansion || pLinkItemsTxtRecord->wVersion < 100u))
                                        {
                                            pCubeItem[i].nClassId = nClassId;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (nItemsInCube < 48)
                    {
                        a6[nItemsInCube - 1] = 1;
                    }

                    ++nItemCountWithStacks;
                    if (ITEMS_CheckIfStackable(pItem))
                    {
                        bContainsStackableItem = 1;
                    }
                }
            }
        }
    }

    if (bContainsStackableItem)
    {
        return nItemCountWithStacks >= nQuantity;
    }

    return nItemCountWithStacks == nQuantity;
}

//D2Game.0x6FC8FE40
int16_t __fastcall PLRTRADE_RollRandomItemClassOfSameType(D2GameStrc* pGame, int32_t nItemLevel, int32_t nItemId)
{
    D2ItemDataTbl* pItemDataTbl = DATATBLS_GetItemDataTables();
    D2_ASSERT(pItemDataTbl);

    int32_t nIndex = ITEMS_RollLimitedRandomNumber(&pGame->pGameSeed, pItemDataTbl->nItemsTxtRecordCount);

    int32_t nMax = nIndex - 1;
    if (nMax < 0)
    {
        nMax = pItemDataTbl->nItemsTxtRecordCount - 1;
    }

    if (nIndex == nMax)
    {
        return 0;
    }

    int16_t nIndices[256] = {};
    int32_t nCounter = 0;
    do
    {
        if (nCounter >= std::size(nIndices))
        {
            break;
        }

        if (ITEMS_CheckItemTypeIdByItemId(nIndex, nItemId))
        {
            D2ItemsTxt* pItemsTxtRecord = &pItemDataTbl->pItemsTxt[nIndex];
            if (pItemsTxtRecord->nSpawnable && (pItemsTxtRecord->wVersion < 100u || pGame->wItemFormat >= 100u) && pItemsTxtRecord->nLevel <= nItemLevel)
            {
                nIndices[nCounter] = nIndex;
                ++nCounter;
            }
        }

        ++nIndex;
        if (nIndex >= pItemDataTbl->nItemsTxtRecordCount)
        {
            nIndex = 0;
        }
    }
    while (nIndex != nMax);

    if (nCounter)
    {
        return nIndices[ITEMS_RollLimitedRandomNumber(&pGame->pGameSeed, nCounter)];
    }

    return 0;
}

//D2Game.0x6FC90000
int32_t __fastcall PLRTRADE_CreateCowPortal(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    return ACT1Q4_CreateCowPortal(pGame, pUnit);
}

//D2Game.0x6FC90010
void __fastcall PLRTRADE_CreateCubeOutputs(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CubeMainTxt* pCubeMainTxt, D2CubeItemStrc* pCubeItem)
{
    using CubeOutputFunc = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*);
    constexpr CubeOutputFunc dword_6FD2933C[] =
    {
        nullptr,
        PLRTRADE_CreateCowPortal
    };

    int32_t nSocketables = 0;
    int32_t bSuccess = 0;
    int32_t bAddCraftStats = 1;

    D2UnitStrc* pSocketables[18] = {};
    D2UnitStrc* pOutputs[3] = {};
    int32_t bRemove[3] = {};

    for (int32_t nCounter = 0; nCounter < 3; ++nCounter)
    {
        D2CubeOutputItem* pCubeOutput = &pCubeMainTxt->pOutputItem[nCounter];
        if (pCubeOutput->wItemFlags & (CUBEFLAG_OUT_REMOVE | CUBEFLAG_OUT_UNSOCKET))
        {
            bRemove[nCounter] = 1;
        }

        int32_t nOutputLevel = 0;
        if (pCubeOutput->nLvl)
        {
            nOutputLevel = pCubeOutput->nLvl;
        }
        else
        {
            if (pCubeOutput->nPLvl)
            {
                nOutputLevel = pCubeOutput->nPLvl * STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0) / 100;
            }

            if (pCubeOutput->nILvl)
            {
                nOutputLevel += pCubeOutput->nILvl * pCubeItem[nCounter].nItemLevel / 100;
            }
        }

        if (nOutputLevel <= 1)
        {
            nOutputLevel = 1;
        }

        const int32_t nMaxLevel = DATATBLS_GetMaxLevel(0);
        if (nOutputLevel >= nMaxLevel)
        {
            nOutputLevel = nMaxLevel;
        }

        int32_t nLevel = nOutputLevel;

        const int32_t nType = pCubeOutput->nType;
        if (nType && nType < std::size(dword_6FD2933C))
        {
            CubeOutputFunc pOperateFn = dword_6FD2933C[nType];
            if (pOperateFn)
            {
                bSuccess = pOperateFn(pGame, pUnit);
            }
        }

        if (pCubeOutput->wItemFlags & CUBEFLAG_OUT_COPYMODS)
        {
            D2UnitStrc* pItem = pCubeItem[nCounter].pItem;
            ITEMS_SetInvPage(pItem, INVPAGE_NULL);
            UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);

            D2UnitStrc* pDupeItem = ITEMS_Duplicate(pGame, pItem, 0, bRemove[nCounter] == 0);
            
            int32_t nDupeClassId = 0;
            switch (nType)
            {
            case CUBEOP_ITEMCODE:
            {
                nDupeClassId = pCubeOutput->wBaseItemId;
                break;
            }
            case CUBEOP_ITEMTYPE:
            {
                nDupeClassId = PLRTRADE_RollRandomItemClassOfSameType(pGame, nLevel, pCubeOutput->wBaseItemId);
                break;
            }
            case CUBEOP_USEITEM:
            {
                const int32_t nOriginalClassId = pCubeItem[nCounter].nClassId;
                if (nOriginalClassId >= 0)
                {
                    nDupeClassId = nOriginalClassId;
                }
                break;
            }
            case CUBEOP_USETYPE:
            {
                nDupeClassId = pCubeItem[nCounter].nClassId;
                break;
            }
			default:
				break;
            }

            if (pDupeItem)
            {
                pDupeItem->dwClassId = nDupeClassId;
            }

            D2UnitStrc* pTempItem = pDupeItem;
            D2GAME_InitItemStats_6FC4E520(pGame, &pTempItem, 0, 0);
            pOutputs[nCounter] = pTempItem;
            UNITS_ChangeAnimMode(pTempItem, IMODE_ONCURSOR);
            ITEMS_SetInvPage(pItem, INVPAGE_CUBE);
        }
        else
        {
            if (nType == CUBEOP_USEITEM)
            {
                D2UnitStrc* pItem = pCubeItem[nCounter].pItem;
                ITEMS_SetInvPage(pItem, INVPAGE_NULL);
                UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);

                D2UnitStrc* pDupeItem = ITEMS_Duplicate(pGame, pItem, 0, bRemove[nCounter] == 0);
                pOutputs[nCounter] = pDupeItem;

                UNITS_ChangeAnimMode(pDupeItem, IMODE_ONCURSOR);
                ITEMS_SetInvPage(pItem, INVPAGE_CUBE);

                if (pCubeOutput->nQuality == ITEMQUAL_TEMPERED)
                {
                    const uint16_t nPrefix = ITEMS_RollTemperedItemAffix(pDupeItem, 1);
                    const uint16_t nSuffix = ITEMS_RollTemperedItemAffix(pDupeItem, 0);
                    if (nPrefix && nSuffix)
                    {
                        ITEMS_SetItemQuality(pDupeItem, ITEMQUAL_TEMPERED);
                        ITEMS_AssignRarePrefix(pDupeItem, nPrefix);
                        ITEMS_AssignRareSuffix(pDupeItem, nSuffix);
                    }
                    else
                    {
                        bAddCraftStats = 0;
                    }
                }
            }
            else
            {
                int32_t bCreateItem = 0;
                int32_t nQuality = pCubeOutput->nQuality;
                int32_t nUniqueItemId = -1;
                int32_t nItemId = pCubeOutput->wItemID;
                int32_t nClassId = 0;
                if (nType == CUBEOP_USETYPE)
                {
                    bCreateItem = 1;
                    nClassId = pCubeItem[nCounter].nClassId;
                    if (pCubeOutput->wItemFlags & CUBEFLAG_OUT_NORMAL)
                    {
                        D2UnitStrc* pItem = pCubeItem[nCounter].pItem;
                        if (pItem)
                        {
                            const int32_t nItemFileIndex = ITEMS_GetFileIndex(pItem);
                            nItemId = nItemFileIndex + 1;
                            nLevel = ITEMS_GetItemLevel(pItem);
                            nQuality = ITEMS_GetItemQuality(pItem);
                            if (nQuality == ITEMQUAL_UNIQUE && !(pGame->dwUniqueFlags[nItemFileIndex >> 5] & gdwBitMasks[nItemFileIndex & 0x1F]))
                            {
                                nUniqueItemId = nItemFileIndex;
                            }
                        }
                    }
                }
                else if (nType == CUBEOP_ITEMCODE)
                {
                    bCreateItem = 1;
                    nClassId = pCubeOutput->wBaseItemId;
                }
                else if (nType == CUBEOP_ITEMTYPE)
                {
                    bCreateItem = 1;
                    nClassId = PLRTRADE_RollRandomItemClassOfSameType(pGame, nLevel, pCubeOutput->wBaseItemId);
                }

                if (bCreateItem)
                {
                    D2ItemDropStrc pItemDrop = {};
                    pItemDrop.pGame = pGame;
                    pItemDrop.nItemIndex = nItemId;
                    pItemDrop.wItemFormat = pGame->wItemFormat;
                    pItemDrop.pUnit = pUnit;
                    pItemDrop.nQuality = nQuality;
                    pItemDrop.nItemLvl = nLevel;
                    pItemDrop.nId = nClassId;
                    pItemDrop.nSpawnType = 4;
                    pItemDrop.nX = 0;
                    pItemDrop.nY = 0;
                    pItemDrop.pRoom = 0;
                    pItemDrop.wUnitInitFlags = 1;

                    // TODO: Check offset of pItemDrop.nPrefix & pItemDrop.nSuffix

                    for (int32_t i = 0; i < 3; ++i)
                    {
                        pItemDrop.nPrefix[i] = pCubeOutput->wPrefixId[i];
                    }

                    for (int32_t i = 0; i < 3; ++i)
                    {
                        pItemDrop.nSuffix[i] = pCubeOutput->wSuffixId[i];
                    }

                    //nPrefixCounter = 0;
                    //ptPrefix = pCubeOutput->wPrefixId;
                    //do
                    //{
                    //    ++nPrefixCounter;
                    //    nPrefix = *ptPrefix;
                    //    ++ptPrefix;
                    //    *(DWORD*)&pItemDrop.szName[4 * nPrefixCounter + 12] = nPrefix;
                    //}
                    //while (nPrefixCounter < 3);

                    //nSuffixCounter = 0;
                    //ptSuffix = pCubeOutput->wSuffixId;
                    //do
                    //{
                    //    ++nSuffixCounter;
                    //    nSuffix = *ptSuffix;
                    //    ++ptSuffix;
                    //    pItemDrop.nPrefix[nSuffixCounter + 2] = nSuffix;
                    //}
                    //while (nSuffixCounter < 3);

                    if (!(pCubeOutput->wItemFlags & CUBEFLAG_OUT_SOCKET) || pCubeOutput->nQuantity)
                    {
                        pItemDrop.dwFlags2 |= 8;
                    }
                    else
                    {
                        pItemDrop.dwFlags2 |= 0x10;
                    }

                    if (!(pCubeOutput->wItemFlags & CUBEFLAG_OUT_ETHEREAL))
                    {
                        pItemDrop.dwFlags2 |= 2;
                    }
                    else
                    {
                        pItemDrop.dwFlags2 |= 4;
                    }

                    pOutputs[nCounter] = D2GAME_CreateItemEx_6FC4ED80(pGame, &pItemDrop, 0);

                    if (nUniqueItemId >= 0)
                    {
                        pGame->dwUniqueFlags[nUniqueItemId >> 5] &= gdwInvBitMasks[nUniqueItemId & 0x1F];
                    }
                }
            }
        }

        D2UnitStrc* pCurrentOutput = pOutputs[nCounter];
        if (pCurrentOutput)
        {
            bSuccess = 1;
            if (bRemove[nCounter])
            {
                ITEMS_DestroyRunewordStatList(pGame, pCurrentOutput);
                if (pCubeOutput->wItemFlags & CUBEFLAG_OUT_REMOVE)
                {
                    D2UnitStrc* pItem = pCubeItem[nCounter].pItem;
                    if (pItem && pItem->pInventory)
                    {
                        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pItem->pInventory); i; i = INVENTORY_GetNextItem(i))
                        {
                            D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(i);
                            if (pCheckedItem)
                            {
                                D2UnitStrc* pDupeItem = ITEMS_Duplicate(pGame, pCheckedItem, 0, 1);
                                if (pDupeItem)
                                {
                                    ITEMS_SetInvPage(pDupeItem, INVPAGE_NULL);
                                    UNITS_ChangeAnimMode(pDupeItem, IMODE_ONCURSOR);
                                    pSocketables[nSocketables] = pDupeItem;
                                    ++nSocketables;
                                }
                            }
                        }
                    }
                }
            }

            if (bAddCraftStats)
            {
                for (int32_t i = 0; i < 5; ++i)
                {
                    if (pCubeOutput->pParam[i].nMod >= 0)
                    {
                        if (pCubeOutput->pParam[i].nModChance == 0 || pCubeOutput->pParam[i].nModChance >= 100 || (ITEMS_RollRandomNumber(&pCurrentOutput->pSeed) % 100) <= pCubeOutput->pParam[i].nModChance)
                        {
                            D2PropertyStrc itemProperty = {};
                            itemProperty.nProperty = pCubeOutput->pParam[i].nMod;
                            itemProperty.nLayer = pCubeOutput->pParam[i].nModParam;
                            itemProperty.nMin = pCubeOutput->pParam[i].nModMin;
                            itemProperty.nMax = pCubeOutput->pParam[i].nModMax;
                            ITEMMODS_AddCraftPropertyList(pCurrentOutput, &itemProperty, pGame->bExpansion);
                        }
                    }
                }
            }

            if (pCubeOutput->wItemFlags & CUBEFLAG_OUT_REPAIR)
            {
                if (ITEMS_CheckIfStackable(pCurrentOutput) && pCubeOutput->nQuantity)
                {
                    int32_t nMaxStack = ITEMS_GetTotalMaxStack(pCurrentOutput);
                    if (nMaxStack >= pCubeOutput->nQuantity)
                    {
                        nMaxStack = pCubeOutput->nQuantity;
                    }

                    STATLIST_SetUnitStat(pCurrentOutput, STAT_QUANTITY, nMaxStack, 0);
                }

                if (ITEMS_CheckItemFlag(pCurrentOutput, IFLAG_BROKEN, __LINE__, __FILE__))
                {
                    D2GAME_RepairBrokenItem_6FC4B630(pGame, pUnit, pCurrentOutput);
                }
                else
                {
                    const int32_t nMaxDurability = STATLIST_UnitGetStatValue(pCurrentOutput, STAT_MAXDURABILITY, 0);
                    if (STATLIST_GetUnitBaseStat(pCurrentOutput, STAT_DURABILITY, 0) < nMaxDurability)
                    {
                        STATLIST_SetUnitStat(pCurrentOutput, STAT_DURABILITY, nMaxDurability, 0);
                    }
                }
            }

            if (pCubeOutput->wItemFlags & CUBEFLAG_OUT_RECHARGE)
            {
                D2GAME_RechargeItem_6FC4BD50(pGame, pUnit, pCurrentOutput);
            }

            const int32_t nQuantity = pCubeOutput->nQuantity;
            if (pCubeOutput->wItemFlags & CUBEFLAG_OUT_SOCKET)
            {
                if (nQuantity && !STATLIST_UnitGetStatValue(pCurrentOutput, STAT_ITEM_NUMSOCKETS, 0))
                {
                    if (!(ITEMS_GetItemFlags(pCurrentOutput) & 0x800))
                    {
                        int32_t nMaxSockets = ITEMS_GetMaxSockets(pCurrentOutput);
                        if (nMaxSockets >= pCubeOutput->nQuantity)
                        {
                            nMaxSockets = pCubeOutput->nQuantity;
                        }

                        switch (ITEMS_GetItemQuality(pCurrentOutput))
                        {
                        case ITEMQUAL_MAGIC:
                        case ITEMQUAL_TEMPERED:
                            if (nMaxSockets > 3)
                            {
                                nMaxSockets = 3;
                            }
                            break;
                        case ITEMQUAL_SET:
                        case ITEMQUAL_RARE:
                        case ITEMQUAL_UNIQUE:
                        case ITEMQUAL_CRAFT:
                            if (nMaxSockets > 1)
                            {
                                nMaxSockets = 1;
                            }
                            break;
                        default:
                            break;
                        }

                        if (nMaxSockets > 0)
                        {
                            ITEMS_SetItemFlag(pCurrentOutput, IFLAG_SOCKETED, IFLAG_NEWITEM);
                            ITEMS_AddSockets(pCurrentOutput, nMaxSockets);
                        }
                    }
                }
            }
            else
            {
                if (nQuantity && ITEMS_CheckIfStackable(pCurrentOutput))
                {
                    int32_t nMaxStack = ITEMS_GetTotalMaxStack(pCurrentOutput);
                    if (nMaxStack >= pCubeOutput->nQuantity)
                    {
                        nMaxStack = pCubeOutput->nQuantity;
                    }

                    STATLIST_SetUnitStat(pCurrentOutput, STAT_QUANTITY, nMaxStack, 0);
                }
            }
        }
    }

    if (!bSuccess)
    {
        return;
    }

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    if (pUnit->pInventory)
    {
		D2UnitStrc* pNextItem = nullptr;
        for (D2UnitStrc* pCurrentItem = INVENTORY_GetFirstItem(pUnit->pInventory); pCurrentItem != nullptr; pCurrentItem = pNextItem)
        {
			pNextItem = INVENTORY_GetNextItem(pCurrentItem); // Needs to be retrieved before deleting current item

            D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(pCurrentItem);
            if (pCheckedItem && ITEMS_GetInvPage(pCheckedItem) == INVPAGE_CUBE)
            {
                int32_t nFlags = 0;
                ITEMS_SetOrRemoveFlag(&nFlags, 32, 1);
                ITEMS_SetItemCell(pCheckedItem, INVPAGE_CUBE);
                D2GAME_UpdateClientItem_6FC3E9D0(pClient, pUnit, pCheckedItem, nFlags);
                D2GAME_RemoveItem_6FC471F0(pGame, pUnit, pCheckedItem, 0);
            }
        }
    }

    SUNIT_AttachSound(pUnit, 4, pUnit);

    for (int32_t i = 0; i < 3; ++i)
    {
        D2UnitStrc* pOutputItem = pOutputs[i];
        if (pOutputItem)
        {
            ITEMS_SetInvPage(pOutputItem, INVPAGE_CUBE);

            if (D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pUnit, pOutputItem->dwUnitId, 0, 0, 1, 1, 0))
            {
                ITEMS_SetItemFlag(pOutputItem, IFLAG_IDENTIFIED, 1);

                D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pOutputItem->dwClassId);
                if (pItemsTxtRecord->nQuest)
                {
                    switch (pItemsTxtRecord->dwCode)
                    {
                    case ' tsh':
                        ACT2Q2_UpdateHoradricItemCounts(pGame, pUnit);
                        break;
                    case ' 2fq':
                        ACT3Q2_UpdateKhalimItemCounts(pGame, pUnit);
                        break;
					default:
						break;
                    }
                }
            }
            else
            {
                SUNIT_RemoveUnit(pGame, pOutputItem);
            }
        }
    }

    for (int32_t i = 0; i < nSocketables; ++i)
    {
        D2UnitStrc* pSocketable = pSocketables[i];
        if (pSocketable)
        {
            ITEMS_SetInvPage(pSocketable, INVPAGE_CUBE);

            if (D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pUnit, pSocketable->dwUnitId, 0, 0, 1, 1, 0))
            {
                ITEMS_SetItemFlag(pSocketable, IFLAG_IDENTIFIED, 1);
            }
            else
            {
                SUNIT_RemoveUnit(pGame, pSocketable);
            }
        }
    }
}

//D2Game.0x6FC90A60
void __fastcall PLRTRADE_Free(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    if (pPlayerData->pTrade)
    {
        D2_FREE_POOL(pGame->pMemoryPool, pPlayerData->pTrade->pSaveData);

        D2ItemTradeStrc* pItemTrade = pPlayerData->pTrade->pItemTrade;
        while (pItemTrade)
        {
            D2ItemTradeStrc* pNext = pItemTrade->pNext;
            D2_FREE_POOL(pGame->pMemoryPool, pItemTrade);
            pItemTrade = pNext;
        }

        pPlayerData->pTrade->pItemTrade = nullptr;
        D2_FREE_POOL(pGame->pMemoryPool, pPlayerData->pTrade);
        pPlayerData->pTrade = nullptr;
    }
}

//D2Game.0x6FC90AE0
void __fastcall sub_6FC90AE0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3)
{
    if (!pUnit || !UNITS_GetPlayerData(pUnit)->pTrade)
    {
        return;
    }

    D2GAME_PACKETS_SendPacketSize06_6FC3C850(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x92, 0, pUnit ? pUnit->dwUnitId : -1);
    SUNIT_IterateLivingPlayers(pGame, sub_6FC90BE0, pUnit);

    for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
    {
        D2UnitStrc* pItem = INVENTORY_UnitIsItem(i);
        if (pItem)
        {
            if (pItem->dwAnimMode == IMODE_STORED)
            {
                sub_6FC4B740(pUnit, pItem);
            }

            INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
            ITEMS_SetItemCMDFlag(pItem, 1, 1);
        }
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pUnit->pInventory);
    if (pCursorItem)
    {
        if (pCursorItem->dwUnitType == UNIT_ITEM)
        {
            INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pCursorItem);
            ITEMS_SetItemCMDFlag(pCursorItem, 1, 1);
        }
        else
        {
            FOG_TraceF(gszEmptyString_6FD447EC, "[sPlayerRestoreInventoryCopy] Deleted/non-item in hand");
        }
    }

    sub_6FC4BA50(pGame, pUnit);
    sub_6FC90C20(pGame, pUnit);
}

//D2Game.0x6FC90BE0
void __fastcall sub_6FC90BE0(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg)
{
    D2UnitStrc* pOtherPlayer = (D2UnitStrc*)pArg;
    if (pPlayer != pOtherPlayer)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        D2GAME_PACKETS_SendPacketSize06_6FC3C850(pClient, 0x92, 0, pOtherPlayer ? pOtherPlayer->dwUnitId : -1);
    }
}

//D2Game.0x6FC90C20
void __fastcall sub_6FC90C20(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    D2PlayerTradeStrc* pTrade = pPlayerData->pTrade;
    D2_ASSERT(pTrade);

    int32_t nUnused = 0;
    sub_6FC8BC70(pGame, pPlayer, pTrade->pSaveData, 0x60u, 0x2000u, 0, &nUnused);

    const int32_t nGold = STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0);
    if (nGold > 0 && pPlayer->dwAnimMode != PLRMODE_DEATH && pPlayer->dwAnimMode != PLRMODE_DEAD)
    {
        const int32_t nGoldDiff = STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0) - nGold;
        D2GAME_SetStatOrResetGold_6FC7CA70(pPlayer, STAT_GOLD, nGoldDiff);

        const int32_t nGoldSum = pTrade->unk0x0C[1] + STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0);
        D2GAME_SetStatOrResetGold_6FC7CA70(pPlayer, STAT_GOLD, nGoldSum);
    }
}

//D2Game.0x6FC90D70
void __fastcall PLRTRADE_SendEventPacketToPlayer(D2UnitStrc* pPlayer, D2C_SRV2CLT5A_TYPES nType, char* szSource)
{
    D2GSPacketSrv5A packet5A = {};
    packet5A.nHeader = 0x5A;
    packet5A.nType = nType;
    packet5A.nColor = STRCOLOR_WHITE;
    packet5A.dwParam = 0;

    if (szSource)
    {
        SStrCopy(packet5A.szText, szSource, INT_MAX);
        packet5A.szText[15] = 0;
    }
    else
    {
        packet5A.szText[0] = 0;
    }

    D2GAME_PACKETS_SendPacket0x5A_6FC3DEC0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), &packet5A);
}

//D2Game.0x6FC90DE0
void __fastcall PLRTRADE_TryToTrade(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2)
{
    if (PLAYER_IsBusy(pPlayer1) || ACT4Q2_HasDiabloBeenKilled(pGame))
    {
        return;
    }

    const uint32_t nTickCount = GetTickCount();

    D2PlayerDataStrc* pPlayerData1 = UNITS_GetPlayerData(pPlayer1);
    D2PlayerDataStrc* pPlayerData2 = UNITS_GetPlayerData(pPlayer2);
    if (PLAYER_IsBusy(pPlayer2))
    {
        if (nTickCount - pPlayerData1->unk0x54 <= 1000)
        {
            return;
        }

        PLRTRADE_SendEventPacketToPlayer(pPlayer1, EVENTTYPE_PLAYERISBUSY, pPlayerData2->szName);
        pPlayerData1->unk0x54 = nTickCount;
        return;
    }

    if (nTickCount - pPlayerData2->dwAcceptTradeTick >= 0x1388)
    {
        if (!pPlayerData1->dwTradeState && !pPlayerData2->dwTradeState)
        {
            if (nTickCount - pPlayerData1->dwAcceptTradeTick < 5000)
            {
                if (nTickCount > pPlayerData1->unk0x54 + 1000)
                {
                    PLRTRADE_SendEventPacketToPlayer(pPlayer1, EVENTTYPE_PLEASEWAIT, nullptr);
                    pPlayerData1->unk0x54 = nTickCount;
                }
                return;
            }

            if (DUNGEON_IsTownLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pPlayer1))) && DUNGEON_IsTownLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pPlayer2))))
            {
                SUNIT_SetInteractInfo(pPlayer1, 0, pPlayer2 ? pPlayer2->dwUnitId : -1);
                SUNIT_SetInteractInfo(pPlayer2, 0, pPlayer1 ? pPlayer1->dwUnitId : -1);

                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__), 0);
                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer2, __FILE__, __LINE__), 1u);

                pPlayerData1->dwTradeState = 1;
                pPlayerData2->dwTradeState = 2;
            }
            else
            {
                SUNIT_ResetInteractInfo(pPlayer1);
                SUNIT_ResetInteractInfo(pPlayer2);
            }
        }
    }
    else
    {
        if (pPlayerData1->unk0x54 + 1000 < nTickCount)
        {
            PLRTRADE_SendEventPacketToPlayer(pPlayer1, EVENTTYPE_PLEASEWAIT, pPlayerData2->szName);
            pPlayerData1->unk0x54 = nTickCount;
        }
    }
}

//D2Game.0x6FC91050
void __fastcall sub_6FC91050(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, D2PlayerDataStrc* pPlayerData1, D2PlayerDataStrc* pPlayerData2, int32_t nTradeState)
{
    UNITS_RefreshInventory(pPlayer1, 1);
    UNITS_RefreshInventory(pPlayer2, 1);

    if (pPlayer1)
    {
        pPlayer1->dwFlagEx |= 0x800000u;
    }

    if (pPlayer2)
    {
        pPlayer2->dwFlagEx |= 0x800000u;
    }

    SUNIT_ResetInteractInfo(pPlayer1);
    SUNIT_ResetInteractInfo(pPlayer2);

    sub_6FC90AE0(pGame, pPlayer1, 1);
    sub_6FC90AE0(pGame, pPlayer2, 1);

    PLRTRADE_Free(pGame, pPlayer1);
    PLRTRADE_Free(pGame, pPlayer2);

    pPlayerData1->dwTradeState = nTradeState;
    pPlayerData2->dwTradeState = 9;

    sub_6FC4B830(pGame, pPlayer1);
    sub_6FC4B830(pGame, pPlayer2);

    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__), 0xCu);
    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer2, __FILE__, __LINE__), 0xCu);

    D2GAME_EVENTS_Delete_6FC34840(pGame, pPlayer1, UNITEVENTCALLBACK_UPDATETRADE, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pPlayer2, UNITEVENTCALLBACK_UPDATETRADE, 0);

    EVENT_SetEvent(pGame, pPlayer1, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
    EVENT_SetEvent(pGame, pPlayer2, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
}

//D2Game.0x6FC91250
int32_t __fastcall sub_6FC91250(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint16_t nButton, int32_t nGoldAmount)
{
    if (!pPlayer)
    {
        return 1;
    }

    int32_t nInteractUnitType = 0;
    int32_t nInteractUnitGUID = 0;
    if (!SUNIT_GetInteractInfo(pPlayer, &nInteractUnitType, &nInteractUnitGUID))
    {
        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), UPDATEUI_CLOSETRADE);
        return 0;
    }

    if (nButton >= TRADEBTN_CLOSESTASH && nButton <= TRADEBTN_STASHDEPOSIT)
    {
        if (nInteractUnitType != UNIT_OBJECT)
        {
            return 1;
        }

        D2UnitStrc* pInteractObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, nInteractUnitGUID);
        if (!pInteractObject || pInteractObject->dwClassId != OBJECT_STASH)
        {
            return 0;
        }
        
        D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(pPlayer);
        if (!pPlayerRoom || !DUNGEON_IsRoomInTown(pPlayerRoom))
        {
            return 0;
        }
        
        D2ActiveRoomStrc* pObjectRoom = UNITS_GetRoom(pInteractObject);    
        if (!pObjectRoom || !DUNGEON_IsRoomInTown(pObjectRoom))
        {
            return 0;
        }

        switch (nButton)
        {
        case TRADEBTN_CLOSESTASH:
        {
            if (SUNIT_GetInteractInfo(pPlayer, &nInteractUnitType, &nInteractUnitGUID) && nInteractUnitType == UNIT_OBJECT)
            {
                SUNIT_ResetInteractInfo(pPlayer);
            }

            sub_6FC4B830(pGame, pPlayer);
            break;
        }
        case TRADEBTN_STASHWITHDRAW:
        {
            if (nGoldAmount > 0 && nGoldAmount <= STATLIST_UnitGetStatValue(pPlayer, STAT_GOLDBANK, 0))
            {
                if (nGoldAmount + STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0) <= UNITS_GetInventoryGoldLimit(pPlayer))
                {
                    ITEMS_HandleGoldTransaction(pGame, pPlayer, nGoldAmount);
                    PLRTRADE_AddGold(pPlayer, STAT_GOLDBANK, -nGoldAmount);
                }
                else
                {
                    SUNIT_AttachSound(pPlayer, 19, pPlayer);
                }
            }
            break;
        }
        case TRADEBTN_STASHDEPOSIT:
        {
            if (nGoldAmount > 0 && nGoldAmount <= STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0))
            {
                const int32_t nStashGoldLimit = UNITS_GetStashGoldLimit(pPlayer);
                const int32_t nGoldBank = STATLIST_UnitGetStatValue(pPlayer, STAT_GOLDBANK, 0);
                if (nGoldBank + nGoldAmount <= nStashGoldLimit)
                {
                    PLRTRADE_AddGold(pPlayer, STAT_GOLD, -nGoldAmount);
                    PLRTRADE_AddGold(pPlayer, STAT_GOLDBANK, nGoldAmount);
                }
                else if (nGoldBank < nStashGoldLimit)
                {
                    PLRTRADE_AddGold(pPlayer, STAT_GOLDBANK, nStashGoldLimit - nGoldBank);
                    PLRTRADE_AddGold(pPlayer, STAT_GOLD, nGoldBank - nStashGoldLimit);
                }
            }
            break;
        }
		default:
			break;
        }
    }
    else if (nButton >= TRADEBTN_CLOSECUBE && nButton <= TRADEBTN_TRANSMUTE)
    {
        if (nInteractUnitType != UNIT_ITEM)
        {
            return 3;
        }

        switch (nButton)
        {
        case TRADEBTN_CLOSECUBE:
        {
            if (SUNIT_GetInteractInfo(pPlayer, &nInteractUnitType, &nInteractUnitGUID) && nInteractUnitType == UNIT_ITEM)
            {
                SUNIT_ResetInteractInfo(pPlayer);
            }

            sub_6FC4B830(pGame, pPlayer);
            break;
        }
        case TRADEBTN_TRANSMUTE:
        {
            PLRTRADE_HandleCubeInteraction(pGame, pPlayer);
            break;
        }
		default:
			break;
        }
    }
    else
    {
        if (nInteractUnitType != UNIT_PLAYER)
        {
            D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 13);
            return 3;
        }

        D2UnitStrc* pInteractPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nInteractUnitGUID);
        if (pInteractPlayer)
        {
            D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
            D2PlayerDataStrc* pInteractPlayerData = UNITS_GetPlayerData(pInteractPlayer);

            switch (nButton)
            {
            case TRADEBTN_CANCEL:
            {
                if (pPlayerData->dwTradeState != 2 && pPlayerData->dwTradeState != 1 || pInteractPlayerData->dwTradeState != 2 && pInteractPlayerData->dwTradeState != 1)
                {
                    if (sub_6FC41660(pGame, pPlayer) || sub_6FC41660(pGame, pInteractPlayer))
                    {
                        return 0;
                    }
                }

                sub_6FC91050(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData, 9u);
                
                const uint32_t nTickCount = GetTickCount();
                pInteractPlayerData->dwAcceptTradeTick = nTickCount;
                pPlayerData->dwAcceptTradeTick = nTickCount;
                break;
            }
            case TRADEBTN_PERFORM:
            {
                if (sub_6FC41660(pGame, pPlayer) || sub_6FC41660(pGame, pInteractPlayer))
                {
                    if (pPlayerData->dwTradeState == 7)
                    {
                        pPlayerData->dwTradeState = 14;
                    }
                    else if (pPlayerData->dwTradeState == 8)
                    {
                        pPlayerData->dwTradeState = 15;
                    }
                    else
                    {
                        sub_6FC91050(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData, 12);
                    }
                    return 0;
                }

                if (ACT4Q2_HasDiabloBeenKilled(pGame))
                {
                    sub_6FC91050(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData, 9u);
                    return 0;
                }

                if (pPlayerData->dwTradeState != 2 || pInteractPlayerData->dwTradeState != 1)
                {
                    sub_6FC91050(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData, 12);
                    return 0;
                }

                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 6u);
                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), 6u);

                pPlayerData->dwTradeState = 3;
                pInteractPlayerData->dwTradeState = 3;

                if (!PLRTRADE_AllocPlayerTrade(pGame, pPlayer))
                {
                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), UI_NPCSHOP);
                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), UPDATEUI_CLOSETRADE);
                    return 0;
                }

                if (PLRTRADE_AllocPlayerTrade(pGame, pInteractPlayer))
                {
                    D2GSPacketSrv78 packet78 = {};

                    packet78.nHeader = 0x78u;
                    strcpy_s(packet78.szName, CLIENTS_GetName(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__)));
                    packet78.dwPlayerGUID = pInteractPlayer->dwUnitId;
                    sub_6FC3E0D0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), &packet78);

                    strcpy_s(packet78.szName, CLIENTS_GetName(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__)));
                    packet78.dwPlayerGUID = pPlayer->dwUnitId;
                    sub_6FC3E0D0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), &packet78);
                    return 0;
                }

                PLRTRADE_Free(pGame, pPlayer);

                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), UI_NPCSHOP);
                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), UPDATEUI_CLOSETRADE);
                break;
            }
            case TRADEBTN_ACCEPT:
            {
                if (sub_6FC41660(pGame, pPlayer) || sub_6FC41660(pGame, pInteractPlayer))
                {
                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 6u);
                    return 0;
                }

                if (ACT4Q2_HasDiabloBeenKilled(pGame))
                {
                    sub_6FC91050(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData, 9);
                    return 0;
                }

                if (pPlayerData->dwTradeState != 3)
                {
                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 6u);
                    return 0;
                }

                if (INVENTORY_GetCursorItem(pPlayer->pInventory))
                {
                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 6u);
                    return 0;
                }

                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), 5u);

                if (pPlayerData->dwTradeState != 3)
                {
                    return 0;
                }
                
                pPlayerData->dwTradeState = 4;
                
                if (pInteractPlayerData->dwTradeState != 4)
                {
                    return 0;
                }

                if (pPlayerData->pTrade->unk0x0C[0])
                {
                    if (STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0) < pPlayerData->pTrade->unk0x0C[0])
                    {
                        sub_6FC92920(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData);

                        const uint32_t nTickCount = GetTickCount();
                        pInteractPlayerData->dwAcceptTradeTick = nTickCount;
                        pPlayerData->dwAcceptTradeTick = nTickCount;
                        return 0;
                    }
                    else if (pPlayerData->pTrade->unk0x0C[0] + STATLIST_UnitGetStatValue(pInteractPlayer, STAT_GOLD, 0) > UNITS_GetInventoryGoldLimit(pInteractPlayer))
                    {
                        sub_6FC92920(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData);

                        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 10);
                        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), 9u);

                        const uint32_t nTickCount = GetTickCount();
                        pInteractPlayerData->dwAcceptTradeTick = nTickCount;
                        pPlayerData->dwAcceptTradeTick = nTickCount;
                        return 0;
                    }
                }

                if (pInteractPlayerData->pTrade->unk0x0C[0])
                {
                    if (STATLIST_UnitGetStatValue(pInteractPlayer, STAT_GOLD, 0) < pInteractPlayerData->pTrade->unk0x0C[0])
                    {
                        sub_6FC92920(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData);

                        const uint32_t nTickCount = GetTickCount();
                        pInteractPlayerData->dwAcceptTradeTick = nTickCount;
                        pPlayerData->dwAcceptTradeTick = nTickCount;
                        return 0;
                    }

                    if (pInteractPlayerData->pTrade->unk0x0C[0] + STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0) > UNITS_GetInventoryGoldLimit(pPlayer))
                    {
                        sub_6FC92920(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData);

                        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 9u);
                        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), 10);

                        const uint32_t nTickCount = GetTickCount();
                        pInteractPlayerData->dwAcceptTradeTick = nTickCount;
                        pPlayerData->dwAcceptTradeTick = nTickCount;
                        return 0;
                    }
                }

                D2TradeStates nTradeState = TRADESTATE_OTHERNOROOM;
                if (!INVENTORY_CanItemsBeTraded(pGame->pMemoryPool, pPlayer, pInteractPlayer, &nTradeState))
                {
                    pPlayerData->pTrade->unk0x0C[0] = 0;
                    pInteractPlayerData->pTrade->unk0x0C[0] = 0;

                    if (nTradeState == TRADESTATE_OTHERNOROOM)
                    {
                        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), UPDATEUI_BUYERNOROOM);
                        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), UPDATEUI_SELLERNOROOM);
                        sub_6FC92920(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData);
                    }
                    else
                    {
                        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), UPDATEUI_SELLERNOROOM);
                        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), UPDATEUI_BUYERNOROOM);
                        sub_6FC92920(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData);
                    }

                    const uint32_t nTickCount = GetTickCount();
                    pInteractPlayerData->dwAcceptTradeTick = nTickCount;
                    pPlayerData->dwAcceptTradeTick = nTickCount;
                    return 0;
                }

                for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pPlayer->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
                {
                    D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(pItem);
                    if (ITEMS_GetInvPage(pCheckedItem) == 2 && !sub_6FC425F0(pGame, pInteractPlayer, pCheckedItem))
                    {
                        sub_6FC92920(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData);

                        const uint32_t nTickCount = GetTickCount();
                        pInteractPlayerData->dwAcceptTradeTick = nTickCount;
                        pPlayerData->dwAcceptTradeTick = nTickCount;
                        return 0;
                    }
                }

                for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pInteractPlayer->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
                {
                    D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(pItem);
                    if (ITEMS_GetInvPage(pCheckedItem) == 2 && !sub_6FC425F0(pGame, pPlayer, pCheckedItem))
                    {
                        sub_6FC92920(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData);

                        const uint32_t nTickCount = GetTickCount();
                        pInteractPlayerData->dwAcceptTradeTick = nTickCount;
                        pPlayerData->dwAcceptTradeTick = nTickCount;
                        return 0;
                    }
                }

                pPlayer->dwFlagEx |= 0x800000u;
                pInteractPlayer->dwFlagEx |= 0x800000u;

                pPlayerData->dwTradeState = 9;
                pInteractPlayerData->dwTradeState = 9;

                EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
                EVENT_SetEvent(pGame, pInteractPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);

                if (sub_6FC92A90(pGame, pPlayer, pInteractPlayer) && sub_6FC92A90(pGame, pInteractPlayer, pPlayer))
                {
                    if (pPlayerData->pTrade->unk0x0C[0])
                    {
                        if (STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0) >= pPlayerData->pTrade->unk0x0C[0])
                        {
                            PLRTRADE_AddGold(pPlayer, STAT_GOLD, -pPlayerData->pTrade->unk0x0C[0]);
                        }

                        ITEMS_HandleGoldTransaction(pGame, pInteractPlayer, pPlayerData->pTrade->unk0x0C[0]);
                    }

                    if (pInteractPlayerData->pTrade->unk0x0C[0])
                    {
                        if (STATLIST_UnitGetStatValue(pInteractPlayer, STAT_GOLD, 0) >= pInteractPlayerData->pTrade->unk0x0C[0])
                        {
                            PLRTRADE_AddGold(pInteractPlayer, STAT_GOLD, -pInteractPlayerData->pTrade->unk0x0C[0]);
                        }

                        ITEMS_HandleGoldTransaction(pGame, pPlayer, pInteractPlayerData->pTrade->unk0x0C[0]);
                    }

                    SUNIT_ResetInteractInfo(pPlayer);
                    SUNIT_ResetInteractInfo(pInteractPlayer);

                    PLRTRADE_Free(pGame, pPlayer);
                    PLRTRADE_Free(pGame, pInteractPlayer);

                    sub_6FC4B830(pGame, pPlayer);
                    sub_6FC4B830(pGame, pInteractPlayer);

                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 13);
                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), 13);
                }
                else
                {
                    sub_6FC92920(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData);
                }

                const uint32_t nTickCount = GetTickCount();
                pInteractPlayerData->dwAcceptTradeTick = nTickCount;
                pPlayerData->dwAcceptTradeTick = nTickCount;
                break;
            }
            case TRADEBTN_GREENCHECK:
            {
                if (pPlayerData->dwTradeState == 4)
                {
                    pPlayerData->dwTradeState = 3;
                }

                if (pInteractPlayerData->dwTradeState == 4)
                {
                    pInteractPlayerData->dwTradeState = 3;
                }

                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 6u);
                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), 6u);
                break;
            }
            case TRADEBTN_SENDGOLD:
            {
                if (nGoldAmount <= STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0))
                {
                    const uint32_t nInventoryGoldLimit = UNITS_GetInventoryGoldLimit(pInteractPlayer);
                    const uint32_t nGold = STATLIST_UnitGetStatValue(pInteractPlayer, STAT_GOLD, 0);
                    
                    uint32_t nAddedGold = nGoldAmount;
                    if (nGold + nGoldAmount > nInventoryGoldLimit)
                    {
                        nAddedGold = nInventoryGoldLimit - nGold;
                    }

                    if (pPlayerData->pTrade)
                    {
                        pPlayerData->pTrade->unk0x0C[0] = nAddedGold;
                        D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(SUNIT_GetClientFromPlayer(pInteractPlayer, __FILE__, __LINE__), nAddedGold, 0);
                        D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), nAddedGold, 1);
                    }
                }
                else
                {
                    if (!sub_6FC41660(pGame, pPlayer) && !sub_6FC41660(pGame, pInteractPlayer))
                    {
                        sub_6FC91050(pGame, pPlayer, pInteractPlayer, pPlayerData, pInteractPlayerData, 9u);
                    }
                }
                break;
            }
			default:
				break;
            }
        }
        else
        {
            if (!sub_6FC41660(pGame, pPlayer))
            {
                D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), UPDATEUI_CLOSETRADE);
            }
        }
    }

    return 0;
}

//D2Game.0x6FC92130
void __fastcall PLRTRADE_HandleCubeInteraction(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return;
    }

    const int32_t nCubeMainTxtRecordCount = DATATBLS_GetCubemainTxtRecordCount();

    int32_t nItemsInCube = 0;
    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
    {
        if (ITEMS_GetInvPage(INVENTORY_UnitIsItem(pItem)) == INVPAGE_CUBE)
        {
            ++nItemsInCube;
        }
    }

    if (nItemsInCube <= 0)
    {
        return;
    }

    _SYSTEMTIME systemTime = {};
    GetLocalTime(&systemTime);
    const uint16_t wDayOfWeek = systemTime.wDayOfWeek + 1;
    const uint16_t wDay = systemTime.wDay;

    for (int32_t nId = 0; nId < nCubeMainTxtRecordCount; ++nId)
    {
        D2CubeMainTxt* pCubeMainTxtRecord = DATATBLS_GetCubemainTxtRecord(nId);
        if (!pCubeMainTxtRecord)
        {
            return;
        }

        if ((pCubeMainTxtRecord->nEnabled || dword_6FD4DC30)
            && (pGame->bExpansion || pCubeMainTxtRecord->wVersion < 100)
            && (pGame->nGameType || pGame->dwGameType || !pCubeMainTxtRecord->nLadder)
            && pCubeMainTxtRecord->nMinDiff <= pGame->nDifficulty
            && (pCubeMainTxtRecord->nClass == ((uint8_t)(-1)) || pCubeMainTxtRecord->nClass == pUnit->dwClassId)
            && pCubeMainTxtRecord->nNumInputs == nItemsInCube)
        {
            bool bOpConditionsFulfilled = false;

            switch (pCubeMainTxtRecord->nOp)
            {
            case 1:
            {
                if (wDay >= pCubeMainTxtRecord->nParam && wDay <= pCubeMainTxtRecord->nValue)
                {
                    bOpConditionsFulfilled = true;
                }
                break;
            }
            case 2:
            {
                if (pCubeMainTxtRecord->nValue == wDayOfWeek)
                {
                    bOpConditionsFulfilled = true;
                }
                break;
            }
            case 3:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && (int32_t)STATLIST_UnitGetStatValue(pUnit, nStatId, 0) >= (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 4:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && (int32_t)STATLIST_UnitGetStatValue(pUnit, nStatId, 0) <= (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 5:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && STATLIST_UnitGetStatValue(pUnit, nStatId, 0) != (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 6:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && STATLIST_UnitGetStatValue(pUnit, nStatId, 0) == (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 7:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && STATLIST_GetUnitBaseStat(pUnit, nStatId, 0) >= (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 8:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && STATLIST_GetUnitBaseStat(pUnit, nStatId, 0) <= (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 9:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && STATLIST_GetUnitBaseStat(pUnit, nStatId, 0) != (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 10:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && STATLIST_GetUnitBaseStat(pUnit, nStatId, 0) == (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 11:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && STATLIST_GetUnitStatBonus(pUnit, nStatId, 0) >= (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 12:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && STATLIST_GetUnitStatBonus(pUnit, nStatId, 0) <= (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 13:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && STATLIST_GetUnitStatBonus(pUnit, nStatId, 0) != (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            case 14:
            {
                const int32_t nStatId = pCubeMainTxtRecord->nParam;
                if (nStatId < 0 || nStatId > sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    bOpConditionsFulfilled = true;
                }
                else
                {
                    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                    if (pItemStatCostTxtRecord && STATLIST_GetUnitStatBonus(pUnit, nStatId, 0) == (pCubeMainTxtRecord->nValue >> pItemStatCostTxtRecord->nValShift))
                    {
                        bOpConditionsFulfilled = true;
                    }
                }
                break;
            }
            default:
                bOpConditionsFulfilled = true;
                break;
            }

            if (bOpConditionsFulfilled && pUnit->pInventory)
            {
                D2CubeItemStrc a5[3] = {};
                int32_t a6[48] = {};

                int32_t nInputs = 0;
                while (PLRTRADE_CheckCubeInput(pGame, pUnit->pInventory, pCubeMainTxtRecord, nInputs, a5, a6))
                {
                    ++nInputs;
                    if (nInputs >= 7)
                    {
                        PLRTRADE_CreateCubeOutputs(pGame, pUnit, pCubeMainTxtRecord, a5);
                        return;
                    }
                }
            }
        }
    }
}

//D2Game.0x6FC927D0
int32_t __fastcall PLRTRADE_AllocPlayerTrade(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (pPlayerData->pTrade)
    {
        return 0;
    }

    D2PlayerTradeStrc* pTradeRecord = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2PlayerTradeStrc);
    pTradeRecord->pSaveData = (BYTE*)D2_ALLOC_POOL(pGame->pMemoryPool, 0x2000u);
    pTradeRecord->nSaveLength = sub_6FC8A0F0(pGame, pPlayer, pTradeRecord->pSaveData, 0x2000u, 0, 0);

    D2_ASSERT(pTradeRecord->nSaveLength > 0);

    pTradeRecord->pItemTrade = nullptr;
    pTradeRecord->unk0x0C[0] = 0;
    pTradeRecord->unk0x0C[1] = STATLIST_UnitGetStatValue(pPlayer, STAT_GOLD, 0);
    pTradeRecord->unk0x0C[2] = 0;
    pPlayerData->pTrade = pTradeRecord;

    return 1;
}

//D2Game.0x6FC92890
void __fastcall PLRTRADE_AddGold(D2UnitStrc* pUnit, int32_t nStat, int32_t nValue)
{
    if (!pUnit)
    {
        return;
    }

    const int32_t nNewValue = nValue + STATLIST_UnitGetStatValue(pUnit, nStat, 0);
    if (nNewValue < 0)
    {
        STATLIST_SetUnitStat(pUnit, nStat, 0, 0);
        return;
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        if (nStat == STAT_GOLD)
        {
            if (nNewValue > UNITS_GetInventoryGoldLimit(pUnit))
            {
                STATLIST_SetUnitStat(pUnit, STAT_GOLD, 0, 0);
                return;
            }
        }
        else if (nStat == STAT_GOLDBANK)
        {
            if (nNewValue > UNITS_GetStashGoldLimit(pUnit))
            {
                STATLIST_SetUnitStat(pUnit, STAT_GOLDBANK, 0, 0);
                return;
            }
        }
    }

    STATLIST_AddUnitStat(pUnit, nStat, nValue, 0);
}

//D2Game.0x6FC92920
void __fastcall sub_6FC92920(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, D2PlayerDataStrc* pPlayerData1, D2PlayerDataStrc* pPlayerData2)
{
    sub_6FC4B830(pGame, pPlayer1);
    sub_6FC4B830(pGame, pPlayer2);

    UNITS_RefreshInventory(pPlayer1, 1);
    UNITS_RefreshInventory(pPlayer2, 1);

    if (pPlayer1)
    {
        pPlayer1->dwFlagEx |= 0x800000u;
    }

    if (pPlayer2)
    {
        pPlayer2->dwFlagEx |= 0x800000u;
    }

    pPlayerData1->pTrade->unk0x0C[0] = 0;
    pPlayerData2->pTrade->unk0x0C[0] = 0;

    sub_6FC90AE0(pGame, pPlayer1, 1);
    sub_6FC90AE0(pGame, pPlayer2, 1);

    D2ClientStrc* pClient1 = SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient1, 0, 0);
    D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient1, 0, 1);

    D2ClientStrc* pClient2 = SUNIT_GetClientFromPlayer(pPlayer2, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient2, 0, 0);
    D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient2, 0, 1);

    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(pClient1, 6u);
    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(pClient2, 6u);

    pPlayerData1->dwTradeState = 5;
    pPlayerData2->dwTradeState = 5;

    sub_6FC4B830(pGame, pPlayer1);
    sub_6FC4B830(pGame, pPlayer2);

    EVENT_SetEvent(pGame, pPlayer1, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
    EVENT_SetEvent(pGame, pPlayer2, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
}

//D2Game.0x6FC92A90
int32_t __fastcall sub_6FC92A90(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__);
    const int32_t bSet = CLIENTS_CheckFlag(pClient, CLIENTSAVEFLAG_HARDCORE);

    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pPlayer1->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
    {
        D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(pItem);
        if (ITEMS_GetInvPage(pCheckedItem) == 2)
        {
            D2UnitStrc* pDupeItem = ITEMS_Duplicate(pGame, pCheckedItem, 0, 0);
            if (pDupeItem)
            {
                if (ITEMS_GetItemType(pDupeItem) == ITEMTYPE_PLAYER_BODY_PART)
                {
                    ITEMS_SetEarName(pDupeItem, ITEMS_GetEarName(pCheckedItem));
                    ITEMS_SetEarLevel(pDupeItem, ITEMS_GetEarLevel(pCheckedItem));
                    ITEMS_SetItemFlag(pDupeItem, IFLAG_NAMED, bSet);
                }
                else if (ITEMS_CheckItemFlag(pDupeItem, IFLAG_PERSONALIZED, __LINE__, __FILE__))
                {
                    ITEMS_SetEarName(pDupeItem, ITEMS_GetEarName(pCheckedItem));
                }

                UNITS_ChangeAnimMode(pDupeItem, 4);
                ITEMS_SetInvPage(pDupeItem, 0);

                if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pPlayer2, pDupeItem->dwUnitId, 0, 0, 1, 1, 0))
                {
                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__), UPDATEUI_BUYERNOROOM);
                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer2, __FILE__, __LINE__), UPDATEUI_SELLERNOROOM);
                    return 0;
                }

                if (D2GAME_CheckHasFilledSockets_6FC4B2D0(pCheckedItem, __FILE__, __LINE__) && !sub_6FC4B310(pGame, pPlayer2, pCheckedItem, pDupeItem, 1u))
                {
                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__), UPDATEUI_BUYERNOROOM);
                    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer2, __FILE__, __LINE__), UPDATEUI_SELLERNOROOM);
                    return 0;
                }
            }
        }
    }

    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pPlayer1->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
    {
        D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(pItem);

        const uint8_t nInvPage = ITEMS_GetInvPage(pCheckedItem);
        switch (nInvPage)
        {
        case 1:
        {
            int32_t nFlags = 0;
            ITEMS_SetOrRemoveFlag(&nFlags, 32, 1);
            ITEMS_SetItemCell(pCheckedItem, nInvPage);
            D2GAME_UpdateClientItem_6FC3E9D0(pClient, pPlayer1, pCheckedItem, nFlags);
            D2GAME_RemoveItem_6FC471F0(pGame, pPlayer1, pCheckedItem, 0);
            break;
        }
        case 2:
        {
            sub_6FC4B740(pPlayer1, pCheckedItem);

            int32_t nFlags = 0;
            ITEMS_SetOrRemoveFlag(&nFlags, 32, 1);
            ITEMS_SetItemCell(pCheckedItem, nInvPage);
            D2GAME_UpdateClientItem_6FC3E9D0(pClient, pPlayer1, pCheckedItem, nFlags);
            D2GAME_RemoveItem_6FC471F0(pGame, pPlayer1, pCheckedItem, 0);
            break;
        }
		default:
			break;
        }
    }

    return 1;
}

//D2Game.0x6FC92CF0
void __fastcall PLRTRADE_StopAllPlayerInteractions(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    int32_t nUnitType = 0;
    int32_t nUnitGUID = 0;
    if (!SUNIT_GetInteractInfo(pPlayer, &nUnitType, &nUnitGUID) || nUnitType != UNIT_PLAYER)
    {
        return;
    }

    D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID);
    if (!pOtherPlayer)
    {
        return;
    }

    SUNIT_ResetInteractInfo(pPlayer);

    if (!SUNIT_GetInteractInfo(pOtherPlayer, &nUnitType, &nUnitGUID) || nUnitType != UNIT_PLAYER)
    {
        return;
    }

    const int32_t nPlayerGUID = pPlayer ? pPlayer->dwUnitId : -1;
    if (nUnitGUID != nPlayerGUID)
    {
        return;
    }

    UNITS_GetPlayerData(pPlayer)->dwTradeState = 9;
    EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);

    sub_6FC90AE0(pGame, pPlayer, 0);
    sub_6FC90AE0(pGame, pOtherPlayer, 1);

    PLRTRADE_Free(pGame, pPlayer);
    PLRTRADE_Free(pGame, pOtherPlayer);

    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pOtherPlayer, __FILE__, __LINE__), 0xCu);
    SUNIT_ResetInteractInfo(pOtherPlayer);

    UNITS_GetPlayerData(pOtherPlayer)->dwTradeState = 9;
    EVENT_SetEvent(pGame, pOtherPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
}

//D2Game.0x6FC92EE0
void __fastcall sub_6FC92EE0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    PLRTRADE_StopAllPlayerInteractions(pGame, pUnit);
}

//D2Game.0x6FC92EF0
void __fastcall sub_6FC92EF0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    SUNIT_ResetInteractInfo(pUnit);
    sub_6FC4B830(pGame, pUnit);
}

//D2Game.0x6FC92F10
void __fastcall sub_6FC92F10(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
    int32_t nUnitType = 0;
    int32_t nUnitGUID = 0;
    if (SUNIT_GetInteractInfo(pPlayer, &nUnitType, &nUnitGUID) && nUnitType)
    {
        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0xCu);
        return;
    }

    D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID);
    if (!pOtherPlayer)
    {
        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), 0xCu);
        return;
    }

    D2UnitStrc* pDupeItem = ITEMS_Duplicate(pGame, pItem, 0, 0);
    UNITS_ChangeAnimMode(pDupeItem, 4);
    ITEMS_SetInvPage(pDupeItem, 1u);

    const int32_t nItemGUID = pDupeItem ? pDupeItem->dwUnitId : -1;

    D2PlayerDataStrc* pPlayerData1 = UNITS_GetPlayerData(pPlayer);
    D2PlayerDataStrc* pPlayerData2 = UNITS_GetPlayerData(pOtherPlayer);
    if (D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pOtherPlayer, nItemGUID, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 0, 1, 0))
    {
        if (D2GAME_CheckHasFilledSockets_6FC4B2D0(pItem, __FILE__, __LINE__))
        {
            sub_6FC4B310(pGame, pOtherPlayer, pItem, pDupeItem, 1u);
        }

        if (pPlayerData1 && pPlayerData1->pTrade)
        {
            D2ItemTradeStrc* pItemTrade = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2ItemTradeStrc);
            pItemTrade->nItemGUID1 = pItem->dwUnitId;
            pItemTrade->nItemGUID2 = nItemGUID;
            pItemTrade->pNext = pPlayerData1->pTrade->pItemTrade;
            pPlayerData1->pTrade->pItemTrade = pItemTrade;
        }

        if (pPlayerData1->dwTradeState == 4 || pPlayerData2->dwTradeState == 4)
        {
            D2ClientStrc* pClient1 = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
            D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient1, 0, 0);
            D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient1, 0, 1);
            
            D2ClientStrc* pClient2 = SUNIT_GetClientFromPlayer(pOtherPlayer, __FILE__, __LINE__);
            D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient2, 0, 0);
            D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient2, 0, 1);

            D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(pClient1, UPDATEUI_OPENTRADE);
            D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(pClient2, UPDATEUI_OPENTRADE);

            pPlayerData1->dwTradeState = 3;
            pPlayerData2->dwTradeState = 3;
        }
    }
    else
    {
        ITEMS_RemoveFromAllPlayers(pGame, pDupeItem);
        sub_6FC91050(pGame, pPlayer, pOtherPlayer, pPlayerData1, pPlayerData2, 9u);
    }
}

//D2Game.0x6FC931D0
void __fastcall sub_6FC931D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
    int32_t nUnitType = 0;
    int32_t nUnitGUID = 0;
    if (SUNIT_GetInteractInfo(pPlayer, &nUnitType, &nUnitGUID) && nUnitType)
    {
        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), UPDATEUI_CLOSETRADE);
        return;
    }

    D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID);
    if (!pOtherPlayer)
    {
        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), UPDATEUI_CLOSETRADE);
        return;
    }

    const int32_t nItemGUID = pItem ? pItem->dwUnitId : -1;

    D2PlayerDataStrc* pPlayerData1 = UNITS_GetPlayerData(pPlayer);
    if (pPlayerData1 && pPlayerData1->pTrade)
    {
        for (D2ItemTradeStrc* pItemTrade = pPlayerData1->pTrade->pItemTrade; pItemTrade; pItemTrade = pItemTrade->pNext)
        {
            if (pItemTrade->nItemGUID1 == nItemGUID)
            {
                int32_t nUnused = 0;
                if (sub_6FC446B0(pGame, pOtherPlayer, pItemTrade->nItemGUID2, &nUnused, 1, 1, 0, 1))
                {
                    //for (pCurrentItem = pItemTrade; pCurrentItem; pCurrentItem = pCurrentItem->pNext)
                    //    ;
                }
                break;
            }
        }
    }

    D2PlayerDataStrc* pPlayerData2 = UNITS_GetPlayerData(pOtherPlayer);
    const uint32_t nTickCount = GetTickCount() + 10000;
    pPlayerData1->pTrade->unk0x0C[2] = nTickCount;
    pPlayerData2->pTrade->unk0x0C[2] = nTickCount;

    D2ClientStrc* pClient1 = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(pClient1, UPDATEUI_CHECKBOX);
    D2ClientStrc* pClient2 = SUNIT_GetClientFromPlayer(pOtherPlayer, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(pClient2, UPDATEUI_CHECKBOX);

    if (pPlayerData1->dwTradeState == 4 || pPlayerData2->dwTradeState == 4)
    {
        D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient1, 0, 0);
        D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient1, 0, 1);

        D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient2, 0, 0);
        D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(pClient2, 0, 1);

        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(pClient1, UPDATEUI_OPENTRADE);
        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(pClient2, UPDATEUI_OPENTRADE);

        pPlayerData1->dwTradeState = 3;
        pPlayerData2->dwTradeState = 3;
    }
}

//D2Game.0x6FC933F0
int32_t __fastcall PLRTRADE_CopyTradeSaveDataToBuffer(D2UnitStrc* pUnit, uint8_t* pBuffer, int32_t nBufferSize)
{
    D2PlayerTradeStrc* pTrade = UNITS_GetPlayerData(pUnit)->pTrade;
    if (pTrade && nBufferSize >= pTrade->nSaveLength)
    {
        memcpy(pBuffer, pTrade->pSaveData, pTrade->nSaveLength);
        return pTrade->nSaveLength;
    }

    return 0;
}

//D2Game.0x6FC93430
int32_t __fastcall sub_6FC93430(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
    D2PlayerDataStrc* pPlayerData1 = UNITS_GetPlayerData(pPlayer);
    int32_t nUnitType = 0;
    int32_t nUnitGUID = 0;
    if (!SUNIT_GetInteractInfo(pPlayer, &nUnitType, &nUnitGUID))
    {
        return pPlayerData1->dwTradeState <= 4;
    }

    if (nUnitType != UNIT_PLAYER)
    {
        return 1;
    }

    D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID);
    if (!pOtherPlayer)
    {
        return pPlayerData1->dwTradeState <= 4;
    }

    D2PlayerDataStrc* pPlayerData2 = UNITS_GetPlayerData(pOtherPlayer);
    if (!pPlayerData2->pTrade)
    {
        return 0;
    }

    if (ACT4Q2_HasDiabloBeenKilled(pGame))
    {
        return 0;
    }

    if (pPlayer && (pPlayer->dwFlagEx >> 23) & 1 || (pOtherPlayer->dwFlagEx >> 23) & 1)
    {
        return 0;
    }

    D2_ASSERT(pPlayerData1->pTrade);

    int32_t nResult = 1;
    if (pPlayerData1->dwTradeState == 4)
    {
        nResult = 0;
    }

    if (pItem && pItem->dwUnitType == UNIT_ITEM)
    {
        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
        if (pItemsTxtRecord->nQuest)
        {
            if (pItemsTxtRecord->dwCode == ' xob')
            {
                for (D2UnitStrc* i = INVENTORY_GetFirstItem(pPlayer->pInventory); i; i = INVENTORY_GetNextItem(i))
                {
                    if (ITEMS_GetInvPage(INVENTORY_UnitIsItem(i)) == INVPAGE_CUBE)
                    {
                        nResult = 0;
                        PLRTRADE_SendEventPacketToPlayer(pPlayer, EVENTTYPE_HASITEMSINBOX, nullptr);
                        break;
                    }
                }
            }

            if (ITEMS_FindQuestItem(pGame, pOtherPlayer, pItemsTxtRecord->dwCode))
            {
                nResult = 0;
            }
        }
        else
        {
            if (ITEMS_CheckItemFlag(pItem, IFLAG_NOSELL, __LINE__, __FILE__))
            {
                nResult = 0;
            }
            else
            {
                if (pItem->dwUnitType == UNIT_ITEM && ITEMS_GetItemQuality(pItem) == ITEMQUAL_UNIQUE)
                {
                    D2UniqueItemsTxt* pUniqueItemsTxtRecord = ITEMS_GetUniqueItemsTxtRecord(ITEMS_GetFileIndex(pItem));
                    if (pUniqueItemsTxtRecord && pUniqueItemsTxtRecord->dwUniqueItemFlags & gdwBitMasks[UNIQUEITEMSFLAGINDEX_CARRY1])
                    {
                        nResult = 0;
                    }
                }
            }
        }
    }

    if (pPlayer)
    {
        pPlayer->dwFlagEx |= 0x400000u;
    }

    pPlayerData1->dwTradeState = 7;
    pPlayerData2->dwTradeState = 7;

    EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
    EVENT_SetEvent(pGame, pOtherPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);

    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), UPDATEUI_OPENTRADE);
    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pOtherPlayer, __FILE__, __LINE__), UPDATEUI_OPENTRADE);

    return nResult;
}

//D2Game.0x6FC93740
int32_t __fastcall sub_6FC93740(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    int32_t nUnitType = 0;
    int32_t nUnitGUID = 0;

    if (!SUNIT_GetInteractInfo(pUnit, &nUnitType, &nUnitGUID) || nUnitType != UNIT_PLAYER)
    {
        return 1;
    }

    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
    {
        if (INVENTORY_GetItemNodePage(pItem) == INVPAGE_TRADE)
        {
            return 0;
        }
    }

    return 1;
}

//D2Game.0x6FC937A0
int32_t __fastcall D2GAME_PLRTRADE_IsInteractingWithPlayer(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    int32_t nUnitType = 0;
    int32_t nUnitGUID = 0;

    return SUNIT_GetInteractInfo(pUnit, &nUnitType, &nUnitGUID) && nUnitType == UNIT_PLAYER && SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID);
}

//D2Game.0x6FC937F0
void __fastcall D2GAME_PLRTRADE_Last_6FC937F0(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t a3, int32_t a4)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2PlayerTradeStrc* pTrade = pPlayerData->pTrade;

    switch (pPlayerData->dwTradeState)
    {
    case 5:
    {
        pPlayerData->dwTradeState = 6;
        EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
        return;
    }
    case 6:
    {
        pPlayerData->dwTradeState = 3;
        if (pPlayer)
        {
            pPlayer->dwFlagEx &= 0xFF3FFFFFu;
        }
        return;
    }
    case 7:
    {
        pPlayerData->dwTradeState = 8;
        EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
        return;
    }
    case 8:
    {
        if (pPlayer)
        {
            pPlayer->dwFlagEx &= 0xFFBFFFFFu;
        }

        if (pTrade->unk0x0C[2])
        {
            pPlayerData->dwTradeState = 11;
            EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 125, 0, 0);
        }
        else
        {
            pPlayerData->dwTradeState = 3;
        }
        return;
    }
    case 9:
    {
        pPlayerData->dwTradeState = 10;
        EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
        return;
    }
    case 10:
    {
        pPlayerData->dwTradeState = 0;
        if (pPlayer)
        {
            pPlayer->dwFlagEx &= 0xFF3FFFFFu;
        }
        return;
    }
    case 11:
    {
        if (pTrade->unk0x0C[2] >= GetTickCount())
        {
            EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 25, 0, 0);
            return;
        }

        pPlayerData->dwTradeState = 3;
        if (pPlayer)
        {
            pPlayer->dwFlagEx &= 0xFF3FFFFFu;
        }

        pTrade->unk0x0C[2] = 0;
        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), UPDATEUI_NORMCHECKBOX);
        return;
    }
    case 12:
    {
        FOG_TraceF("TRADE", "  time: %d", pGame->dwGameFrame);
        pPlayerData->dwTradeState = 13;
        EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 4, 0, 0);
        return;
    }
    case 13:
    {
        FOG_TraceF("TRADE", "  time: %d", pGame->dwGameFrame);
        pPlayerData->dwTradeState = 0;
        if (pPlayer)
        {
            pPlayer->dwFlagEx &= 0xFF3FFFFFu;
        }

        GAME_LogMessage(3, "[HACKLIST]  PlayerTradeUpdate: Player %s caught in bad trade state", pPlayerData->szName);
        sub_6FC36C20(pGame, pPlayerData->pClient->dwClientId, __FILE__, __LINE__);
        return;
    }
    case 14:
    {
        pPlayerData->dwTradeState = 15;
        EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 1, 0, 0);
        return;
    }
    case 15:
    {
        if (pPlayer)
        {
            pPlayer->dwFlagEx &= 0xFFBFFFFFu;
        }

        pPlayerData->dwTradeState = 16;
        EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 5, 0, 0);
        return;
    }
    case 16:
    {
        int32_t nUnitType = 0;
        int32_t nUnitGUID = 0;
        if (SUNIT_GetInteractInfo(pPlayer, &nUnitType, &nUnitGUID) && nUnitType == UNIT_PLAYER)
        {
            D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID);
            if (pOtherPlayer)
            {
                D2PlayerDataStrc* pPlayerData1 = UNITS_GetPlayerData(pPlayer);
                D2PlayerDataStrc* pPlayerData2 = UNITS_GetPlayerData(pOtherPlayer);
                if ((pPlayerData1->dwTradeState == 2 || pPlayerData1->dwTradeState == 1) && (pPlayerData2->dwTradeState == 2 || pPlayerData2->dwTradeState == 1))
                {
                    sub_6FC91050(pGame, pPlayer, pOtherPlayer, pPlayerData1, pPlayerData2, 12);
                    const uint32_t nTickCount = GetTickCount();
                    pPlayerData2->dwAcceptTradeTick = nTickCount;
                    pPlayerData1->dwAcceptTradeTick = nTickCount;
                    return;
                }

                if (!sub_6FC41660(pGame, pPlayer) && !sub_6FC41660(pGame, pOtherPlayer))
                {
                    sub_6FC91050(pGame, pPlayer, pOtherPlayer, pPlayerData1, pPlayerData2, 12);
                    const uint32_t nTickCount = GetTickCount();
                    pPlayerData2->dwAcceptTradeTick = nTickCount;
                    pPlayerData1->dwAcceptTradeTick = nTickCount;
                    return;
                }
            }
        }

        EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_UPDATETRADE, pGame->dwGameFrame + 5, 0, 0);
        return;
    }
    default:
        return;
    }
}
