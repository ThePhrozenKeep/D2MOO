#include "ITEMS/Items.h"

#include <algorithm>

#include <D2BitManip.h>

#include <D2Collision.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2ItemMods.h>
#include <D2Items.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <Units/UnitRoom.h>
#include <DataTbls/MonsterIds.h>

#include "AI/AiGeneral.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "INVENTORY/InvMode.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/ItemsMagic.h"
#include "MONSTER/MonsterMode.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlrModes.h"
#include "PLAYER/PlrTrade.h"
#include "QUESTS/Quests.h"
#include "SKILLS/SkillAss.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


#pragma pack(push, 1)
struct D2TCArrayStrc
{
    D2TCExShortStrc* pTC;
    int32_t nPicks;
    int16_t nMagic;
    int16_t nRare;
    int16_t nSet;
    int16_t nUnique;
    int16_t nSuperior;
    int16_t nNormal;
};
#pragma pack(pop)


bool gbGoldItemIdInitialized;
int32_t gnGoldItemId;
int32_t dword_6FD4DC18;


//D2Game.0x6FC4D470
D2UniqueItemsTxt* __fastcall ITEMS_GetUniqueItemsTxtRecord(int32_t nUniqueItemId)
{
    if (nUniqueItemId >= 0 && nUniqueItemId < sgptDataTables->nUniqueItemsTxtRecordCount)
    {
        return &sgptDataTables->pUniqueItemsTxt[nUniqueItemId];
    }

    return nullptr;
}

//D2Game.0x6FC4D4A0
uint32_t __fastcall ITEMS_HasUniqueBeenDroppedAlready(D2GameStrc* pGame, int32_t nUniqueItemId)
{
    if (!pGame)
    {
        return 0;
    }

    if (nUniqueItemId < 0 || nUniqueItemId > 4096)
    {
        return 1;
    }

    return pGame->dwUniqueFlags[nUniqueItemId >> 5] & gdwBitMasks[nUniqueItemId & 0x1F];
}

//D2Game.0x6FC4D4E0
int32_t __fastcall ITEMS_CanUniqueItemBeDropped(D2GameStrc* pGame, D2UnitStrc* pItem)
{
    if (!pItem)
    {
        return 0;
    }

    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
    if (!pItemsTxtRecord)
    {
        return 0;
    }
    
    const int32_t nFileIndex = ITEMS_GetFileIndex(pItem);
    D2UniqueItemsTxt* pUniqueItemsTxtRecord = ITEMS_GetUniqueItemsTxtRecord(nFileIndex);
    if (!pUniqueItemsTxtRecord)
    {
        return 0;
    }

    if (pUniqueItemsTxtRecord->dwUniqueItemFlags & gdwBitMasks[UNIQUEITEMSFLAGINDEX_NOLIMIT])
    {
        return 1;
    }

    if ((pItemsTxtRecord->nQuest || ITEMS_GetItemQuality(pItem) != ITEMQUAL_UNIQUE || !ITEMS_HasUniqueBeenDroppedAlready(pGame, nFileIndex)) && nFileIndex <= 4096)
    {
        pGame->dwUniqueFlags[nFileIndex >> 5] |= gdwBitMasks[nFileIndex & 0x1F];
        return 1;
    }

    return 0;
}

//D2Game.0x6FC4D5E0
int32_t __fastcall sub_6FC4D5E0(D2UnitStrc** ppItem, D2ItemDropStrc* pItemDrop)
{
    // TODO: v5
    int32_t v5 = 0;
    int32_t nPrefix = 0;
    int32_t nSuffix = 0;
    if (pItemDrop->nPrefix[0] >= 0)
    {
        nPrefix = ITEMS_RollMagicAffixes(*ppItem, 1, pItemDrop->nPrefix[0] > 0 ? 1 : 0, 1, 1, pItemDrop->nPrefix[0]);
        ITEMS_AssignPrefix(*ppItem, nPrefix, 0);

        if (nPrefix)
        {
            if (pItemDrop->nSuffix[0] < 0)
            {
                ITEMS_SetItemFlag(*ppItem, IFLAG_IDENTIFIED, 0);
                sub_6FC52410(*ppItem, pItemDrop);
                return 1;
            }
        }
        else
        {
            v5 = 1;
        }
    }
    else
    {
        v5 = 1;
    }

    if (pItemDrop->nSuffix[0] > 0)
    {
        v5 = 1;
    }

    nSuffix = ITEMS_RollMagicAffixes(*ppItem, 1, v5, 1, 0, pItemDrop->nSuffix[0]);
    ITEMS_AssignSuffix(*ppItem, nSuffix, 0);

    if (!nPrefix && !nSuffix)
    {
        return 0;
    }

    ITEMS_SetItemFlag(*ppItem, IFLAG_IDENTIFIED, 0);
    sub_6FC52410(*ppItem, pItemDrop);
    return 1;
}

//D2Game.0x6FC4D6B0
void __fastcall sub_6FC4D6B0(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    if (ITEMS_GetItemQuality(pItem) < 2 || !ITEMS_CheckIfSocketable(pItem) || ITEMS_CheckIfStackable(pItem))
    {
        return;
    }

    uint32_t nMaxSockets = ITEMS_GetMaxSockets(pItem);
    if (pGame->nDifficulty == DIFFMODE_NORMAL)
    {
        nMaxSockets = std::min(nMaxSockets, 3u);
    }
    else if (pGame->nDifficulty == DIFFMODE_NIGHTMARE)
    {
        nMaxSockets = std::min(nMaxSockets, 4u);
    }
    else if (pGame->nDifficulty == DIFFMODE_HELL)
    {
        nMaxSockets = std::min(nMaxSockets, 6u);
    }

    if (nMaxSockets <= 0)
    {
        return;
    }

    if (ITEMS_GetItemFormat(pItem) >= 100 || !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ARMOR))
    {
        uint32_t nChance = ITEMS_RollRandomNumber(ITEMS_GetItemSeed(pItem)) % 100;
        if (!(pItemDrop->dwFlags2 & 8))
        {
            if (pItemDrop->dwFlags2 & 0x10)
            {
                nChance = 0;
            }

            if (pItemDrop->dwFlags1 & 0x800 || nChance < 33)
            {
                ITEMS_SetItemFlag(pItem, IFLAG_SOCKETED, 1);

                if (ITEMS_GetItemFormat(pItem) < 1u)
                {
                    nMaxSockets = std::min(nMaxSockets, 3u);

                    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_HELM))
                    {
                        nMaxSockets = std::min(nMaxSockets, 2u);
                    }

                    ITEMS_AddSockets(pItem, nMaxSockets);
                }
                else
                {
                    ITEMS_AddSockets(pItem, ITEMS_GetItemStartSeed(pItem) % nMaxSockets + 1);
                }
            }
        }
    }
}

//D2Game.0x6FC4D800
void __fastcall ITEMS_MakeEthereal(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    if (pItemDrop->dwFlags2 & 2 || (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) && !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_ARMOR)) || !ITEMS_HasDurability(pItem))
    {
        return;
    }

    const int32_t nItemQuality = ITEMS_GetItemQuality(pItem);
    if (nItemQuality == ITEMQUAL_INFERIOR || nItemQuality == ITEMQUAL_SET || ITEMS_IsQuestItem(pItem))
    {
        return;
    }

    int32_t bApplyEthereality = 0;
    if (ITEMS_RollRandomNumber(ITEMS_GetItemSeed(pItem)) % 100 < 5)
    {
        bApplyEthereality = 1;
    }

    if (pItemDrop->dwFlags1 & 0x400000)
    {
        bApplyEthereality = 1;
    }

    if (pItemDrop->dwFlags2 & 4 || bApplyEthereality)
    {
        ITEMMODS_ApplyEthereality(pItem);

        if (ITEMS_HasDurability(pItem))
        {
            STATLIST_SetUnitStat(pItem, STAT_MAXDURABILITY, STATLIST_GetUnitBaseStat(pItem, STAT_MAXDURABILITY, 0) / 2 + 1, 0);
            STATLIST_SetUnitStat(pItem, STAT_DURABILITY, STATLIST_UnitGetStatValue(pItem, STAT_MAXDURABILITY, 0), 0);
        }
    }
}

//D2Game.0x6FC4D900
void __fastcall sub_6FC4D900(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    switch (ITEMS_GetItemType(pItem))
    {
    case ITEMTYPE_CHARM:
    {
        ITEMS_AssignCharmAffixes(pGame, pItem, pItemDrop);
        break;
    }
    case ITEMTYPE_BODY_PART:
    {
        int32_t nItemIndex = pItemDrop->nItemIndex;
        if (pItemDrop->pUnit)
        {
            nItemIndex = pItemDrop->pUnit->dwClassId;
        }

        ITEMS_SetFileIndex(pItem, nItemIndex);
        break;
    }
    case ITEMTYPE_PLAYER_BODY_PART:
    {
        int32_t nItemIndex = pItemDrop->nItemIndex;
        if (pItemDrop->pUnit)
        {
            nItemIndex = pItemDrop->pUnit->dwClassId;
        }

        ITEMS_SetFileIndex(pItem, nItemIndex);
        ITEMS_SetItemFlag(pItem, IFLAG_ISEAR, 1);
        break;
    }
    case ITEMTYPE_SCROLL:
    {
        ITEMS_AssignSuffix(pItem, sub_6FC54240(pItem, 1), 0);
        break;
    }
    case ITEMTYPE_BOOK:
    {
        ITEMS_AssignSuffix(pItem, sub_6FC54240(pItem, 0), 0);
        break;
    }
    default:
    {
        sub_6FC52410(pItem, pItemDrop);
        sub_6FC4D6B0(pGame, pItem, pItemDrop);
        break;
    }
    }
}

//D2Game.0x6FC4DA10
int32_t __fastcall ITEMS_AssignCharmAffixes(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    // TODO: nRand
    int16_t nSuffix = 0;
    int16_t nPrefix = 0;
    int32_t nRand = 0;
    if (pItemDrop->nPrefix[0] > 0)
    {
        nPrefix = ITEMS_RollMagicAffixes(pItem, 1, 1, 1, 1, pItemDrop->nPrefix[0]);
        ITEMS_AssignPrefix(pItem, nPrefix, 0);
    }
    else if (pItemDrop->nSuffix[0] <= 0)
    {
        nSuffix = ITEMS_RollMagicAffixes(pItem, 1, 0, 1, 1, pItemDrop->nSuffix[0]);
        ITEMS_AssignPrefix(pItem, nSuffix, 0);

        if (!nSuffix)
        {
            nRand = 1;
        }
    }

    if (pItemDrop->nSuffix[0] > 0)
    {
        nSuffix = ITEMS_RollMagicAffixes(pItem, 1, 1, 1, 0, pItemDrop->nSuffix[0]);
        ITEMS_AssignSuffix(pItem, nSuffix, 0);
    }
    else if (pItemDrop->nPrefix[0] <= 0)
    {
        nPrefix = ITEMS_RollMagicAffixes(pItem, 1, nRand, 1, 0, pItemDrop->nPrefix[0]);
        ITEMS_AssignSuffix(pItem, nPrefix, 0);
    }

    if (nPrefix || nSuffix)
    {
        ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 0);
        return 1;
    }

    char szCode[4] = {};

    const uint32_t nItemBaseCode = ITEMS_GetBaseCode(pItem);
    szCode[0] = (BYTE)nItemBaseCode == ' ' ? '\0' : (BYTE)nItemBaseCode;
    szCode[1] = BYTE1(nItemBaseCode) == ' ' ? '\0' : BYTE1(nItemBaseCode);
    szCode[2] = BYTE2(nItemBaseCode) == ' ' ? '\0' : BYTE2(nItemBaseCode);
    szCode[3] = '\0';

    FOG_Trace("Couldn't assign random magic modifiers to charm (code:%s, level:%d)\n", szCode, ITEMS_GetItemQlvl(pItem));
    FOG_DisplayAssert("wPrefix != 0 || wSuffix != 0", __FILE__, __LINE__);
    exit(-1);
}

//D2Game.0x6FC4DB60
void __fastcall D2GAME_ITEMS_AssignSpecial_6FC4DB60(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_CHARM))
    {
        ITEMS_AssignCharmAffixes(pGame, pItem, pItemDrop);
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BODY_PART))
    {
        int32_t nItemIndex = pItemDrop->nItemIndex;
        if (pItemDrop->pUnit)
        {
            nItemIndex = pItemDrop->pUnit->dwClassId;
        }

        ITEMS_SetFileIndex(pItem, nItemIndex);
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_PLAYER_BODY_PART))
    {
        int32_t nItemIndex = pItemDrop->nItemIndex;
        if (pItemDrop->pUnit)
        {
            nItemIndex = pItemDrop->pUnit->dwClassId;
        }

        ITEMS_SetFileIndex(pItem, nItemIndex);
        ITEMS_SetItemFlag(pItem, IFLAG_ISEAR, 1);
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
    {
        ITEMS_AssignSuffix(pItem, sub_6FC54240(pItem, 1), 0);
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
    {
        ITEMS_AssignSuffix(pItem, sub_6FC54240(pItem, 0), 0);
    }

    sub_6FC52410(pItem, pItemDrop);
}

//D2Game.0x6FC4DC20
void __fastcall sub_6FC4DC20(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    if (ITEMS_GetItemFormat(pItem))
    {
        D2GAME_ITEMS_AssignSpecial_6FC4DB60(pGame, pItem, pItemDrop);
    }
    else
    {
        sub_6FC4D900(pGame, pItem, pItemDrop);
    }
}

//D2Game.0x6FC4DE00
int32_t __fastcall ITEMS_RollItemQuality(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    constexpr int32_t nItemQualities[] =
    {
        ITEMQUAL_UNIQUE, ITEMQUAL_RARE, ITEMQUAL_SET, ITEMQUAL_MAGIC, ITEMQUAL_SUPERIOR, ITEMQUAL_NORMAL
    };

    if (ITEMS_GetItemFormat(pItem) >= 1u && pItemDrop->nQuality != 0)
    {
        return pItemDrop->nQuality;
    }

    D2_ASSERT(pItem->pGame);

    D2ItemRatioTxt* pItemRatioTxtRecord = DATATBLS_GetItemRatioTxtRecord(pItem ? pItem->dwClassId : -1, pItem->pGame->nDifficulty, ITEMS_GetItemFormat(pItem) < 1u ? 0 : 100);
    D2_ASSERT(pItemRatioTxtRecord);

    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
    D2_ASSERT(pItemsTxtRecord);

    if (pItemsTxtRecord->nQuest)
    {
        return ITEMQUAL_NORMAL;
    }

    int32_t nItemLevel = pItemDrop->nItemLvl;
    if (ITEMS_GetItemFormat(pItem) >= 1u)
    {
        if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_MISCELLANEOUS))
        {
            nItemLevel = 1;
        }
        else
        {
            nItemLevel = std::max(nItemLevel - pItemsTxtRecord->nLevel, 1);
        }
    }

    int32_t nItemQuality = ITEMQUAL_INFERIOR;

    int32_t nChance = 0;
    if (ITEMS_GetItemFormat(pItem) < 1u)
    {
        for (int32_t i = 0; i < std::size(nItemQualities); ++i)
        {
            switch (nItemQualities[i])
            {
            case ITEMQUAL_UNIQUE:
                nItemQuality = ITEMQUAL_UNIQUE;
                nChance = pItemRatioTxtRecord->dwUnique - nItemLevel;
                break;

            case ITEMQUAL_RARE:
                nItemQuality = ITEMQUAL_RARE;
                nChance = pItemRatioTxtRecord->dwRare - nItemLevel;
                break;

            case ITEMQUAL_SET:
                nItemQuality = ITEMQUAL_SET;
                nChance = pItemRatioTxtRecord->dwSet - nItemLevel;
                break;

            case ITEMQUAL_MAGIC:
                nItemQuality = ITEMQUAL_MAGIC;
                nChance = pItemRatioTxtRecord->dwMagic - nItemLevel / pItemRatioTxtRecord->dwMagicDivisor;
                break;

            case ITEMQUAL_SUPERIOR:
                nItemQuality = ITEMQUAL_SUPERIOR;
                nChance = pItemRatioTxtRecord->dwHiQuality - nItemLevel / pItemRatioTxtRecord->dwHiQualityDivisor;
                break;

            case ITEMQUAL_NORMAL:
                nItemQuality = ITEMQUAL_NORMAL;
                nChance = pItemRatioTxtRecord->dwNormal - nItemLevel / pItemRatioTxtRecord->dwNormalDivisor;
                break;

            default:
                break;
            }

            if (!ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), std::max(nChance, 1)))
            {
                return nItemQuality;
            }
        }
    }
    else
    {
        for (int32_t i = 0; i < std::size(nItemQualities); ++i)
        {
            switch (nItemQualities[i])
            {
            case ITEMQUAL_UNIQUE:
                nItemQuality = ITEMQUAL_UNIQUE;
                nChance = pItemRatioTxtRecord->dwUnique - nItemLevel / pItemRatioTxtRecord->dwUniqueDivisor;
                break;

            case ITEMQUAL_RARE:
                nItemQuality = ITEMQUAL_RARE;
                nChance = pItemRatioTxtRecord->dwRare - nItemLevel / pItemRatioTxtRecord->dwRareDivisor;
                break;

            case ITEMQUAL_SET:
                nItemQuality = ITEMQUAL_SET;
                nChance = pItemRatioTxtRecord->dwSet - nItemLevel / pItemRatioTxtRecord->dwSetDivisor;
                break;

            case ITEMQUAL_MAGIC:
                nItemQuality = ITEMQUAL_MAGIC;
                nChance = pItemRatioTxtRecord->dwMagic - nItemLevel / pItemRatioTxtRecord->dwMagicDivisor;
                break;

            case ITEMQUAL_SUPERIOR:
                nItemQuality = ITEMQUAL_SUPERIOR;
                nChance = pItemRatioTxtRecord->dwHiQuality - nItemLevel / pItemRatioTxtRecord->dwHiQualityDivisor;
                break;

            case ITEMQUAL_NORMAL:
                nItemQuality = ITEMQUAL_NORMAL;
                nChance = pItemRatioTxtRecord->dwNormal - nItemLevel / pItemRatioTxtRecord->dwNormalDivisor;
                break;

            default:
                break;
            }

            if (!ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nChance))
            {
                return nItemQuality;
            }
        }
    }

    return (pItemDrop->dwFlags2 & 0x40 | 0x20u) >> 5;
}

//D2Game.0x6FC4E1A0
void __fastcall sub_6FC4E1A0(D2UnitStrc** ppItem)
{
    for (int32_t i = 0; i < 3; ++i)
    {
        ITEMS_AssignPrefix(*ppItem, 0, i);
        ITEMS_AssignSuffix(*ppItem, 0, i);
    }

    ITEMS_AssignRarePrefix(*ppItem, 0);
    ITEMS_AssignRareSuffix(*ppItem, 0);
    ITEMS_SetFileIndex(*ppItem, -1);
}

//D2Game.0x6FC4E1F0
void __fastcall sub_6FC4E1F0(D2GameStrc* pGame, D2ItemDropStrc* pItemDrop, D2UnitStrc** ppItem, int32_t nLowSeed)
{
    sub_6FC4E1A0(ppItem);

    ITEMS_SetItemStartSeed(*ppItem, nLowSeed);
    SEED_InitLowSeed(ITEMS_GetItemSeed(*ppItem), nLowSeed);
    ITEMS_RollItemQuality(*ppItem, pItemDrop);
    ITEMS_SetItemQuality(*ppItem, ITEMQUAL_NORMAL);
    pItemDrop->nQuality = ITEMQUAL_NORMAL;

    sub_6FC4DC20(pGame, *ppItem, pItemDrop);
}

//D2Game.0x6FC4E430
int32_t __fastcall sub_6FC4E430(D2GameStrc* pGame, D2ItemDropStrc* pItemDrop, D2UnitStrc** ppItem, uint32_t* pLowSeed)
{
    D2_MAYBE_UNUSED(pGame);
    sub_6FC4E1A0(ppItem);

    ITEMS_SetItemStartSeed(*ppItem, *pLowSeed);
    SEED_InitLowSeed(ITEMS_GetItemSeed(*ppItem), *pLowSeed);
    ITEMS_RollItemQuality(*ppItem, pItemDrop);
    ITEMS_SetItemQuality(*ppItem, ITEMQUAL_SUPERIOR);
    pItemDrop->nQuality = ITEMQUAL_SUPERIOR;
    *pLowSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(*ppItem));
    return sub_6FC54690(*ppItem, pItemDrop);
}

//D2Game.0x6FC4E4D0
uint32_t __fastcall ITEMS_RollLimitedRandomNumber(D2SeedStrc* pSeed, int32_t nMax)
{
    return SEED_RollLimitedRandomNumber(pSeed, nMax);
}

//D2Game.0x6FC4E520
int32_t __fastcall D2GAME_InitItemStats_6FC4E520(D2GameStrc* pGame, D2UnitStrc** ppUnit, D2ItemDropStrc* pItemDrop, int32_t bQuestItem)
{
    if (ITEMS_GetItemType(*ppUnit) == ITEMTYPE_GOLD)
    {
        int32_t nQuantity = 1;
        if (pItemDrop)
        {
            nQuantity = std::max(ITEMS_RollLimitedRandomNumber(&(*ppUnit)->pSeed, 5 * pItemDrop->nItemLvl) + pItemDrop->nItemLvl, 1u);
            if (pItemDrop->nQtyOverride > 0)
            {
                nQuantity = pItemDrop->nQtyOverride;
            }
        }

        D2GAME_SetStatOrResetGold_6FC7CA70(*ppUnit, STAT_GOLD, nQuantity);
    }
    else if (ITEMS_GetQuiverType(*ppUnit))
    {
        const int32_t nMinStack = ITEMS_GetMinStack(*ppUnit);
        int32_t nQuantity = std::max(nMinStack + ITEMS_RollLimitedRandomNumber(&(*ppUnit)->pSeed, ITEMS_GetTotalMaxStack(*ppUnit) - nMinStack), 1u);
        if (pItemDrop && pItemDrop->nQtyOverride > 0)
        {
            nQuantity = pItemDrop->nQtyOverride;
        }

        STATLIST_SetUnitStat(*ppUnit, STAT_QUANTITY, nQuantity, 0);
    }
    else
    {
        int32_t nClassId = -1;
        if (*ppUnit)
        {
            nClassId = (*ppUnit)->dwClassId;
        }

        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nClassId);
        D2_ASSERT(pItemsTxtRecord);

        if (ITEMS_CheckItemTypeId(*ppUnit, ITEMTYPE_ANY_ARMOR))
        {
            STATLIST_SetUnitStat(*ppUnit, STAT_TOBLOCK, pItemsTxtRecord->nBlock, 0);
            STATLIST_SetUnitStat(*ppUnit, STAT_VELOCITYPERCENT, -pItemsTxtRecord->dwSpeed, 0);
            const uint32_t nDurability = (uint32_t)pItemsTxtRecord->nDurability >> 1;
            STATLIST_SetUnitStat(*ppUnit, STAT_DURABILITY, std::min(ITEMS_RollLimitedRandomNumber(&(*ppUnit)->pSeed, nDurability) + nDurability, 255u), 0);
            STATLIST_SetUnitStat(*ppUnit, STAT_MAXDURABILITY, std::min(pItemsTxtRecord->nDurability, 255ui8), 0);

            const int32_t nACValue = pItemsTxtRecord->dwMinAc + ITEMS_RollLimitedRandomNumber(&(*ppUnit)->pSeed, pItemsTxtRecord->dwMaxAc - pItemsTxtRecord->dwMinAc + 1);
            D2_ASSERT(nACValue <= pItemsTxtRecord->dwMaxAc);
            STATLIST_SetUnitStat(*ppUnit, STAT_ARMORCLASS, nACValue, 0);
        }
        else if (ITEMS_CheckItemTypeId(*ppUnit, ITEMTYPE_WEAPON))
        {
            if (ITEMS_CheckIfStackable(*ppUnit))
            {
                const int32_t nMinStack = ITEMS_GetMinStack(*ppUnit);
                int32_t nQuantity = ITEMS_RollLimitedRandomNumber(&(*ppUnit)->pSeed, ITEMS_GetTotalMaxStack(*ppUnit) - nMinStack) + nMinStack;
                if (pItemDrop && pItemDrop->nQtyOverride > 0)
                {
                    nQuantity = pItemDrop->nQtyOverride;
                }

                nQuantity = std::max(nQuantity, 1);
                if (ITEMS_IsMagSetRarUniCrfOrTmp(*ppUnit))
                {
                    nQuantity = 1;
                }

                STATLIST_SetUnitStat(*ppUnit, STAT_QUANTITY, nQuantity, 0);
            }

            const uint32_t nDurability = (uint32_t)pItemsTxtRecord->nDurability >> 1;
            STATLIST_SetUnitStat(*ppUnit, STAT_DURABILITY, std::min(ITEMS_RollLimitedRandomNumber(&(*ppUnit)->pSeed, nDurability) + nDurability, 255u), 0);
            STATLIST_SetUnitStat(*ppUnit, STAT_MAXDURABILITY, std::min(pItemsTxtRecord->nDurability, 255ui8), 0);

            if (pItemsTxtRecord->nMaxDam)
            {
                STATLIST_SetUnitStat(*ppUnit, STAT_MAXDAMAGE, pItemsTxtRecord->nMaxDam, 0);
            }

            if (pItemsTxtRecord->nMinDam)
            {
                STATLIST_SetUnitStat(*ppUnit, STAT_MINDAMAGE, pItemsTxtRecord->nMinDam, 0);
            }

            if (pItemsTxtRecord->n2HandMinDam)
            {
                STATLIST_SetUnitStat(*ppUnit, STAT_SECONDARY_MINDAMAGE, pItemsTxtRecord->n2HandMinDam, 0);
            }

            if (pItemsTxtRecord->n2HandMaxDam)
            {
                STATLIST_SetUnitStat(*ppUnit, STAT_SECONDARY_MAXDAMAGE, pItemsTxtRecord->n2HandMaxDam, 0);
            }

            if (pItemsTxtRecord->nMaxMisDam)
            {
                STATLIST_SetUnitStat(*ppUnit, STAT_ITEM_THROW_MINDAMAGE, pItemsTxtRecord->nMinMisDam, 0);
                STATLIST_SetUnitStat(*ppUnit, STAT_ITEM_THROW_MAXDAMAGE, pItemsTxtRecord->nMaxMisDam, 0);
            }

            STATLIST_SetUnitStat(*ppUnit, STAT_ATTACKRATE, -pItemsTxtRecord->dwSpeed, 0);
        }
        else
        {
            if (ITEMS_CheckIfStackable(*ppUnit))
            {
                const int32_t nMinStack = ITEMS_GetMinStack(*ppUnit);
                int32_t nSpawnStack = ITEMS_GetSpawnStack(*ppUnit);
                if (nSpawnStack < nMinStack || !nSpawnStack)
                {
                    nSpawnStack = std::max(nMinStack, ITEMS_GetTotalMaxStack(*ppUnit));
                }

                int32_t nQuantity = ITEMS_RollLimitedRandomNumber(&(*ppUnit)->pSeed, nSpawnStack - nMinStack) + nMinStack;
                if (pItemDrop && pItemDrop->nQtyOverride > 0)
                {
                    nQuantity = pItemDrop->nQtyOverride;
                }

                if (ITEMS_IsMagSetRarUniCrfOrTmp(*ppUnit) || nQuantity < 1)
                {
                    nQuantity = 1;
                }

                STATLIST_SetUnitStat(*ppUnit, STAT_QUANTITY, nQuantity, 0);
            }

            if (ITEMS_GetItemType(*ppUnit) == ITEMTYPE_ELIXIR)
            {
                ITEMMODS_SetRandomElixirFileIndex(*ppUnit);
            }
        }
    }

    if (ITEMS_GetVarInvGfxCount(*ppUnit))
    {
        ITEMS_SetVarGfxIndex(*ppUnit, ITEMS_RollLimitedRandomNumber(&(*ppUnit)->pSeed, ITEMS_GetVarInvGfxCount(*ppUnit)));
    }

    int32_t nResult = 1;
    if (bQuestItem && pItemDrop)
    {
        nResult = sub_6FC4C5F0(pGame, ppUnit, pItemDrop);

        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(*ppUnit ? (*ppUnit)->dwClassId : -1);
        if (pItemsTxtRecord && pItemsTxtRecord->nQuest && pItemsTxtRecord->nQuestDiffCheck)
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitStateOrFlag(*ppUnit, 0, 0x40u);
            if (!pStatList)
            {
                if (*ppUnit)
                {
                    pStatList = STATLIST_AllocStatList((*ppUnit)->pMemoryPool, 0x40u, 0, UNIT_ITEM, (*ppUnit)->dwUnitId);
                }
                else
                {
                    pStatList = STATLIST_AllocStatList(nullptr, 0x40u, 0, UNIT_ITEM, -1);
                }
                D2COMMON_10475_PostStatToStatList(*ppUnit, pStatList, 1);
            }

            STATLIST_SetStatIfListIsValid(pStatList, STAT_QUESTITEMDIFFICULTY, pGame->nDifficulty, 0);
            ITEMS_SetItemFlag(*ppUnit, IFLAG_IDENTIFIED, 1);
        }
    }

    return nResult;
}

//D2Game.0x6FC4EBF0
uint32_t __fastcall ITEMS_RollRandomNumber(D2SeedStrc* pSeed)
{
    return SEED_RollRandomNumber(pSeed);
}

//D2Game.0x6FC4EC10
D2UnitStrc* __fastcall sub_6FC4EC10(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, BYTE* pBitstream, uint32_t nBufferSize, int32_t bCheckForHeader, D2ItemSaveStrc* pItemSave, uint32_t* pSize, uint32_t dwVersion)
{
    if (!pGame)
    {
        return nullptr;
    }

    if (pItemSave->nClassId < 0 || pItemSave->nClassId >= DATATBLS_GetItemDataTables()->nItemsTxtRecordCount)
    {
        *pSize = 0;
        return nullptr;
    }

    D2UnitStrc* pItem = SUNIT_AllocUnitData(UNIT_ITEM, pItemSave->nClassId, pItemSave->nX, pItemSave->nY, pGame, pRoom, 1, pItemSave->nAnimMode, 0);
    if (!pItem)
    {
        return nullptr;
    }

    int32_t bFail = 0;
    *pSize = ITEMS_DecodeItemFromBitstream(pItem, pBitstream, nBufferSize, bCheckForHeader, 0, dwVersion, &bFail);
    if (!*pSize || bFail)
    {
        SUNIT_RemoveUnit(pGame, pItem);
        return nullptr;
    }

    ITEMS_SetItemFlag(pItem, IFLAG_INIT, 1);
    ITEMS_SetItemFlag(pItem, IFLAG_INSTORE, 0);

    sub_6FC512C0(pGame, pItem);
    sub_6FC51310(pGame, pItem);

    return pItem;
}

//D2Game.0x6FC4ED80
D2UnitStrc* __fastcall D2GAME_CreateItemEx_6FC4ED80(D2GameStrc* pGame, D2ItemDropStrc* pItemDrop, int32_t a3)
{    
    if (!pGame->bExpansion)
    {
        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItemDrop->nId);
        if (!pItemsTxtRecord || pItemsTxtRecord->wVersion >= 100u)
        {
            return nullptr;
        }
    }
    
    D2ItemDataTbl* pItemDataTbl = DATATBLS_GetItemDataTables();
    D2_ASSERT(pItemDataTbl);

    const int32_t nItemId = pItemDrop->nId;
    if (nItemId < 0 || nItemId >= pItemDataTbl->nItemsTxtRecordCount)
    {
        return nullptr;
    }

    D2UnitStrc* pItem = SUNIT_AllocUnitData(UNIT_ITEM, nItemId, pItemDrop->nX, pItemDrop->nY, pItemDrop->pGame, pItemDrop->pRoom, pItemDrop->wUnitInitFlags, pItemDrop->nSpawnType, 0);
    D2_ASSERT(pItem);

    ITEMS_SetItemFlag(pItem, IFLAG_INIT, 1);

    ITEMS_SetItemFormat(pItem, pItemDrop->wItemFormat);

    if (a3 || pItemDrop->bForce)
    {
        pItem->dwInitSeed = pItemDrop->dwSeed;
        SEED_InitLowSeed(&pItem->pSeed, pItemDrop->dwSeed);
        ITEMS_SetItemStartSeed(pItem, pItemDrop->dwItemSeed);
        SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), pItemDrop->dwItemSeed);
    }

    if (pItemDrop->nItemLvl <= 0)
    {
        pItemDrop->nItemLvl = 1;
    }
    ITEMS_SetItemLevel(pItem, pItemDrop->nItemLvl);

    if (!pItemDrop->bForce)
    {
        ITEMS_SetItemFlag(pItem, IFLAG_INSTORE, 1);
    }

    ITEMS_SetInvPage(pItem, INVPAGE_NULL);

    if (!D2GAME_InitItemStats_6FC4E520(pGame, &pItem, pItemDrop, 1))
    {
        SUNIT_RemoveUnit(pGame, pItem);
        return nullptr;
    }

    if (pItemDrop->bForce)
    {
        ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, ITEMS_CheckIfFlagIsSet(pItemDrop->dwFlags1, IFLAG_IDENTIFIED));
        ITEMS_SetItemFlag(pItem, IFLAG_NOSELL, ITEMS_CheckIfFlagIsSet(pItemDrop->dwFlags1, IFLAG_NOSELL));

        if (!ITEMS_GetItemFormat(pItem) && !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ARMOR) && ITEMS_CheckIfFlagIsSet(pItemDrop->dwFlags1, IFLAG_SOCKETED) && !ITEMS_CheckIfFlagIsSet(ITEMS_GetItemFlags(pItem), IFLAG_SOCKETED))
        {
            if (ITEMS_GetMaxSockets(pItem))
            {
                ITEMS_AddSockets(pItem, ITEMS_GetItemStartSeed(pItem) % ITEMS_GetMaxSockets(pItem) + 1);
            }
            else
            {
                ITEMS_SetItemFlag(pItem, IFLAG_SOCKETED, 0);
            }
        }

        ITEMS_SetItemFlag(pItem, IFLAG_SOCKETED, ITEMS_CheckIfFlagIsSet(pItemDrop->dwFlags1, IFLAG_SOCKETED));
        ITEMS_SetItemFlag(pItem, IFLAG_BROKEN, ITEMS_CheckIfFlagIsSet(pItemDrop->dwFlags1, IFLAG_BROKEN));
        ITEMS_SetItemFlag(pItem, IFLAG_STARTITEM, ITEMS_CheckIfFlagIsSet(pItemDrop->dwFlags1, IFLAG_STARTITEM));

        if (ITEMS_GetItemType(pItem) == ITEMTYPE_GOLD)
        {
            D2GAME_SetStatOrResetGold_6FC7CA70(pItem, STAT_GOLD, pItemDrop->nQuantity);
        }
        else
        {
            STATLIST_SetUnitStat(pItem, STAT_QUANTITY, pItemDrop->nQuantity, 0);
        }

        pItemDrop->nMaxDur = std::min(pItemDrop->nMaxDur, 255);
        pItemDrop->nMinDur = std::min(pItemDrop->nMinDur, pItemDrop->nMaxDur);
        STATLIST_SetUnitStat(pItem, STAT_DURABILITY, pItemDrop->nMinDur, 0);
        STATLIST_SetUnitStat(pItem, STAT_MAXDURABILITY, pItemDrop->nMaxDur, 0);
    }

    if (ITEMS_GetItemType(pItem) == ITEMTYPE_PLAYER_BODY_PART)
    {
        if (pItemDrop->bForce)
        {
            ITEMS_SetEarName(pItem, pItemDrop->szName);
            ITEMS_SetEarLevel(pItem, pItemDrop->eEarLvl);
            ITEMS_SetItemFlag(pItem, IFLAG_NAMED, ITEMS_CheckIfFlagIsSet(pItemDrop->dwFlags1, IFLAG_NAMED));
        }
        else
        {
            if (!pItemDrop->pUnit || pItemDrop->pUnit->dwUnitType != UNIT_PLAYER)
            {
                SUNIT_RemoveUnit(pGame, pItem);
                return nullptr;
            }

            D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pItemDrop->pUnit);
            if (!pPlayerData)
            {
                SUNIT_RemoveUnit(pGame, pItem);
                return nullptr;
            }

            ITEMS_SetEarName(pItem, pPlayerData->szName);
            ITEMS_SetEarLevel(pItem, STATLIST_UnitGetStatValue(pItemDrop->pUnit, STAT_LEVEL, 0));

            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pItemDrop->pUnit, __FILE__, __LINE__);
            if (pClient)
            {
                ITEMS_SetItemFlag(pItem, IFLAG_NAMED, CLIENTS_CheckFlag(pClient, CLIENTSAVEFLAG_HARDCORE));
            }
        }
    }

    if (ITEMS_CheckIfFlagIsSet(ITEMS_GetItemFlags(pItem), IFLAG_PERSONALIZED))
    {
        if (pItemDrop->bForce)
        {
            ITEMS_SetEarName(pItem, pItemDrop->szName);
        }
        else
        {
            D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pItemDrop->pUnit);
            D2_ASSERT(pPlayerData);
            ITEMS_SetEarName(pItem, pPlayerData->szName);
        }
    }

    const int32_t nReplenishDurability = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_REPLENISH_DURABILITY, 0);
    if (nReplenishDurability && !EVENT_GetEventFrame(pGame, pItem, UNITEVENTCALLBACK_STATREGEN))
    {
        EVENT_SetEvent(pGame, pItem, UNITEVENTCALLBACK_STATREGEN, 2500 / nReplenishDurability + pGame->dwGameFrame + 1, 0, 0);
    }

    const int32_t nReplenishQuantity = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_REPLENISH_QUANTITY, 0);
    if (nReplenishQuantity && !EVENT_GetEventFrame(pGame, pItem, UNITEVENTCALLBACK_STATREGEN))
    {
        EVENT_SetEvent(pGame, pItem, UNITEVENTCALLBACK_STATREGEN, 2500 / nReplenishQuantity + pGame->dwGameFrame + 1, 0, 0);
    }

    return pItem;
}

//D2Game.0x6FC4F290
D2UnitStrc* __fastcall sub_6FC4F290(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2CoordStrc* pCoords, D2ItemDropStrc* pItemDrop)
{
    int32_t nItemLevel = DATATBLS_GetMonsterLevelInArea(DUNGEON_GetLevelIdFromRoom(pRoom), pGame->nDifficulty, pGame->bExpansion);
    if (nItemLevel > 1)
    {
        --nItemLevel;
    }

    nItemLevel = std::min(nItemLevel, 64);

    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    const int32_t nItemId = sub_6FC4F4A0(pGame, pSeed, nItemLevel, -1, 0, 0);

    D2CoordStrc coords1 = {};
    D2CoordStrc coords2 = {};

    coords2.nX = pCoords->nX;
    coords2.nY = pCoords->nY;

    if (D2GAME_GetRoom_6FC52070(pRoom, pCoords->nX + 2, pCoords->nY + 3))
    {
        coords1.nX = pCoords->nX + 2;
        coords1.nY = pCoords->nY + 3;
    }
    else
    {
        coords1.nX = coords2.nX;
        coords1.nY = coords2.nY;
    }

    D2ActiveRoomStrc* pTargetRoom = COLLISION_GetFreeCoordinatesWithField(pRoom, &coords1, &coords2, 1, 0x3E01u, 0x801u, 1);
    if (!pTargetRoom)
    {
        return 0;
    }

    D2ItemDropStrc itemDrop = {};

    itemDrop.wItemFormat = pGame->wItemFormat;
    itemDrop.pSeed = pSeed;
    itemDrop.nX = coords1.nX;
    itemDrop.nId = nItemId;
    itemDrop.pRoom = pTargetRoom;
    itemDrop.nItemLvl = nItemLevel;
    itemDrop.pUnit = nullptr;
    itemDrop.pGame = pGame;
    itemDrop.nSpawnType = 3;
    itemDrop.nY = coords1.nY;
    itemDrop.wUnitInitFlags = 1;
    itemDrop.nQuality = 0;

    D2UnitStrc* pItem = D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);

    if (pItemDrop)
    {
        memcpy(pItemDrop, &itemDrop, sizeof(itemDrop));
    }

    return pItem;
}

//D2Game.0x6FC4F4A0
int32_t __fastcall sub_6FC4F4A0(D2GameStrc* pGame, D2SeedStrc* pSeed, int32_t nLevelId, int32_t a4, int32_t a5, int32_t a6)
{
    constexpr int32_t BASE_GOLD_RAND = 65;

    if (!gbGoldItemIdInitialized)
    {
        gbGoldItemIdInitialized = true;
        gnGoldItemId = DATATBLS_GetItemIdFromItemCode(' dlg');
    }

    D2_ASSERT(gnGoldItemId != -1);
    D2_ASSERT(gnGoldItemId != 0);
    D2_ASSERT(nLevelId <= BASE_GOLD_RAND);

    const int32_t nBaseLayer = nLevelId >> 1;
    const int32_t nDiff = BASE_GOLD_RAND - nLevelId;
    
    int32_t nAdd = nBaseLayer + 10;
    if (nLevelId % 2)
    {
        nAdd += 1;
    }

    const int32_t nBaseLayerChance = nBaseLayer + nDiff + 5;

    const uint32_t nRand = ITEMS_RollRandomNumber(pSeed) % 100;
    if (nRand < nDiff)
    {
        return gnGoldItemId;
    }

    if (nRand < nBaseLayerChance)
    {
        return sub_6FC4BFF0(pGame, pSeed, nLevelId, a4, a5);
    }

    if (nRand < nBaseLayerChance + nAdd)
    {
        return sub_6FC4C1E0(pGame, pSeed, nLevelId, a4, a5);
    }

    if (nRand < 100)
    {
        return sub_6FC4C3D0(pGame, pSeed, nLevelId, a4, a5, a6);
    }

    FOG_DisplayAssert("0", __FILE__, __LINE__);
    exit(-1);
}

//D2Game.0x6FC4F640
void __fastcall sub_6FC4F640(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2CoordStrc* pCoords)
{
    int32_t nItemLevel = DATATBLS_GetMonsterLevelInArea(DUNGEON_GetLevelIdFromRoom(pRoom), pGame->nDifficulty, pGame->bExpansion);
    if (nItemLevel > 1)
    {
        --nItemLevel;
    }

    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    if (!dword_6FD4DC18)
    {
        DATATBLS_GetItemRecordFromItemCode(' dlg', &dword_6FD4DC18);
    }

    D2CoordStrc coords1 = {};
    D2CoordStrc coords2 = {};

    coords2.nX = pCoords->nX;
    coords2.nY = pCoords->nY;

    if (D2GAME_GetRoom_6FC52070(pRoom, pCoords->nX + 2, pCoords->nY + 3))
    {
        coords1.nX = pCoords->nX + 2;
        coords1.nY = pCoords->nY + 3;
    }
    else
    {
        coords1.nX = coords2.nX;
        coords1.nY = coords2.nY;
    }

    D2ActiveRoomStrc* pTargetRoom = COLLISION_GetFreeCoordinatesWithField(pRoom, &coords1, &coords2, 1, 0x3E01u, 0x801u, 1);
    if (!pTargetRoom)
    {
        return;
    }

    D2ItemDropStrc itemDrop = {};

    itemDrop.wItemFormat = pGame->wItemFormat;
    itemDrop.pSeed = pSeed;
    itemDrop.nX = coords1.nX;
    itemDrop.nId = dword_6FD4DC18;
    itemDrop.pRoom = pTargetRoom;
    itemDrop.nItemLvl = nItemLevel;
    itemDrop.pUnit = nullptr;
    itemDrop.pGame = pGame;
    itemDrop.nSpawnType = 3;
    itemDrop.nY = coords1.nY;
    itemDrop.wUnitInitFlags = 1;
    itemDrop.nQuality = 0;

    D2UnitStrc* pItem = D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);

    if (pItem)
    {
        ITEMS_SetItemFlag(pItem, IFLAG_INSTORE, 0);
    }
}

//D2Game.0x6FC4F830
D2UnitStrc* __fastcall D2GAME_DropArmor_6FC4F830(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2CoordStrc* pCoords, int32_t a4, int32_t a5, D2UnitStrc* pUnit)
{
    int32_t nItemLevel = DATATBLS_GetMonsterLevelInArea(DUNGEON_GetLevelIdFromRoom(pRoom), pGame->nDifficulty, pGame->bExpansion);
    if (nItemLevel > 1)
    { --nItemLevel;
    }

    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    const int32_t nItemId = sub_6FC4BFF0(pGame, pSeed, nItemLevel, a4, a5);
    if (nItemId < 0)
    {
        return 0;
    }

    D2CoordStrc coords1 = {};
    D2CoordStrc coords2 = {};

    coords2.nX = pCoords->nX;
    coords2.nY = pCoords->nY;

    if (D2GAME_GetRoom_6FC52070(pRoom, pCoords->nX + 2, pCoords->nY + 3))
    {
        coords1.nX = pCoords->nX + 2;
        coords1.nY = pCoords->nY + 3;
    }
    else
    {
        coords1.nX = coords2.nX;
        coords1.nY = coords2.nY;
    }

    D2ActiveRoomStrc* pTargetRoom = COLLISION_GetFreeCoordinatesWithField(pRoom, &coords1, &coords2, 1, 0x3E01u, 0x801u, 1);
    if (!pTargetRoom)
    {
        return nullptr;
    }

    D2ItemDropStrc itemDrop = {};

    itemDrop.pSeed = pSeed;
    itemDrop.nId = nItemId;
    itemDrop.nY = coords1.nY;
    itemDrop.pUnit = pUnit;
    itemDrop.pRoom = pTargetRoom;
    itemDrop.wItemFormat = pGame->wItemFormat;
    itemDrop.nItemLvl = nItemLevel;
    itemDrop.dwFlags2 |= 0x40u;
    itemDrop.pGame = pGame;
    itemDrop.nSpawnType = 3;
    itemDrop.nX = coords1.nX;
    itemDrop.wUnitInitFlags = 1;
    itemDrop.nQuality = 0;

    return D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
}

//D2Game.0x6FC4FA50
D2UnitStrc* __fastcall D2GAME_DropWeapon_6FC4FA50(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2CoordStrc* pCoords, int32_t a4, int32_t a5, D2UnitStrc* pUnit)
{    
    int32_t nItemLevel = DATATBLS_GetMonsterLevelInArea(DUNGEON_GetLevelIdFromRoom(pRoom), pGame->nDifficulty, pGame->bExpansion);
    if (nItemLevel > 1)
    { --nItemLevel;
    }

    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    int32_t nItemId = -1;
    for (int32_t i = 0; i < 6; ++i)
    {
        nItemId = sub_6FC4C1E0(pGame, pSeed, nItemLevel, a4, a5);
        if (nItemId >= 0 && ITEMS_IsMetalItem(nItemId))
        {
            break;
        }
    }

    if (nItemId < 0)
    {
        return 0;
    }

    D2CoordStrc coords1 = {};
    D2CoordStrc coords2 = {};

    coords2.nX = pCoords->nX;
    coords2.nY = pCoords->nY;

    if (D2GAME_GetRoom_6FC52070(pRoom, pCoords->nX + 2, pCoords->nY + 3))
    {
        coords1.nX = pCoords->nX + 2;
        coords1.nY = pCoords->nY + 3;
    }
    else
    {
        coords1.nX = coords2.nX;
        coords1.nY = coords2.nY;
    }

    D2ActiveRoomStrc* pTargetRoom = COLLISION_GetFreeCoordinatesWithField(pRoom, &coords1, &coords2, 1, 0x3E01u, 0x801u, 1);
    if (!pTargetRoom)
    {
        return nullptr;
    }

    D2ItemDropStrc itemDrop = {};

    itemDrop.pSeed = pSeed;
    itemDrop.nId = nItemId;
    itemDrop.nY = coords1.nY;
    itemDrop.pUnit = pUnit;
    itemDrop.pRoom = pTargetRoom;
    itemDrop.wItemFormat = pGame->wItemFormat;
    itemDrop.nItemLvl = nItemLevel;
    itemDrop.dwFlags2 |= 0x40u;
    itemDrop.pGame = pGame;
    itemDrop.nSpawnType = 3;
    itemDrop.nX = coords1.nX;
    itemDrop.wUnitInitFlags = 1;
    itemDrop.nQuality = 0;

    return D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
}

//D2Game.0x6FC4FCA0
D2UnitStrc* __fastcall sub_6FC4FCA0(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2CoordStrc* pCoords, int32_t a4, int32_t a5, D2UnitStrc* pUnit)
{
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    int32_t nItemLevel = 1;
    if (nLevelId && !DUNGEON_IsRoomInTown(pRoom))
    {
        nItemLevel = DATATBLS_GetMonsterLevelInArea(nLevelId, pGame->nDifficulty, pGame->bExpansion);
        if (nItemLevel > 1)
        { --nItemLevel;
        }
    }

    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    int32_t bIsMonster = 0;
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        bIsMonster = 1;
    }

    const int32_t nItemId = sub_6FC4C3D0(pGame, pSeed, nItemLevel, a4, a5, bIsMonster);

    D2CoordStrc coords1 = {};
    D2CoordStrc coords2 = {};

    coords2.nX = pCoords->nX;
    coords2.nY = pCoords->nY;

    if (D2GAME_GetRoom_6FC52070(pRoom, pCoords->nY + 2, pCoords->nY + 3))
    {
        coords1.nX = pCoords->nY + 2;
        coords1.nY = pCoords->nY + 3;
    }
    else
    {
        coords1.nX = coords2.nX;
        coords1.nY = coords2.nY;
    }
    
    D2ActiveRoomStrc* pTargetRoom = COLLISION_GetFreeCoordinatesWithField(pRoom, &coords1, &coords2, 1, 0x3E01u, 0x801u, 1);
    if (!pTargetRoom)
    {
        return nullptr;
    }

    D2ItemDropStrc itemDrop = {};

    itemDrop.pSeed = pSeed;
    itemDrop.pUnit = pUnit;
    itemDrop.nId = nItemId;
    itemDrop.pRoom = pTargetRoom;
    itemDrop.wItemFormat = pGame->wItemFormat;
    itemDrop.nY = coords1.nY;
    itemDrop.nItemLvl = nItemLevel;
    itemDrop.pGame = pGame;
    itemDrop.nSpawnType = 3;
    itemDrop.nX = coords1.nX;
    itemDrop.wUnitInitFlags = 1;
    itemDrop.nQuality = 0;

    return D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
}

//D2Game.0x6FC4FEC0
D2UnitStrc* __fastcall D2GAME_DropItemAtUnit_6FC4FEC0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nQuality, int32_t* pItemLevel, D2ItemDropStrc* pItemDrop, int32_t a6, int32_t a7)
{
    D2_ASSERT(pGame);

    D2SeedStrc* pSeed = &pUnit->pSeed;
    
    int32_t bIsMonster = 0;
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        bIsMonster = 1;
    }

    *pItemLevel = ITEMS_GetItemLevelForNewItem(pUnit, 0);

    int32_t nItemId = -1;
    if (pUnit->dwDropItemCode)
    {
        nItemId = DATATBLS_GetItemIdFromItemCode(pUnit->dwDropItemCode);
        D2_ASSERT(nItemId != -1);
    }
    else
    {
        nItemId = sub_6FC4F4A0(pGame, pSeed, *pItemLevel, a6, a7, bIsMonster);
        if (nQuality == ITEMQUAL_MAGIC)
        {
            int32_t nCounter = 0;
            D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);
            while (!pItemsTxtRecord || !(pItemsTxtRecord->dwBitField1 & 1))
            {
                if (nCounter <= 10)
                {
                    nItemId = sub_6FC4F4A0(pGame, pSeed, *pItemLevel, a6, a7, bIsMonster);
                }
                else
                {
                    nItemId = sub_6FC4C1E0(pGame, pSeed, *pItemLevel, a6, a7);
                }

                pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);
                ++nCounter;
            }
        }
    }

    D2CoordStrc coords1 = {};
    D2CoordStrc unitCoords = {};

    UNITS_GetCoords(pUnit, &unitCoords);

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (D2GAME_GetRoom_6FC52070(pRoom, unitCoords.nX + 2, unitCoords.nY + 3))
    {
        coords1.nX = unitCoords.nX + 2;
        coords1.nY = unitCoords.nY + 3;
    }
    else
    {
        coords1.nX = unitCoords.nX;
        coords1.nY = unitCoords.nY;
    }

    D2ActiveRoomStrc* pTargetRoom = COLLISION_GetFreeCoordinatesWithField(pRoom, &coords1, &unitCoords, 1, 0x3E01u, 0x801u, 1);
    if (!pTargetRoom)
    {
        return nullptr;
    }

    D2ItemDropStrc itemDrop = {};

    itemDrop.pSeed = 0;
    itemDrop.pRoom = pTargetRoom;
    itemDrop.nQuality = nQuality;
    itemDrop.wItemFormat = pGame->wItemFormat;
    itemDrop.nX = coords1.nX;
    itemDrop.nY = coords1.nY;
    itemDrop.pUnit = pUnit;
    itemDrop.pGame = pGame;
    itemDrop.nItemLvl = *pItemLevel;
    itemDrop.nId = nItemId;
    itemDrop.nSpawnType = 3;
    itemDrop.wUnitInitFlags = 1;
    
    D2UnitStrc* pItem = D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
    
    if (pItemDrop)
    {
        memcpy(pItemDrop, &itemDrop, sizeof(itemDrop));
    }

    return pItem;
}

//D2Game.0x6FC501A0
D2UnitStrc* __fastcall D2GAME_CreateItemUnit_6FC501A0(D2UnitStrc* pPlayer, int32_t nItemId, D2GameStrc* pGame, int32_t nSpawnTarget, int32_t nQuality, int32_t us1, int32_t alw1, int32_t nItemLevel, int32_t us0, int32_t a1, int32_t alw0)
{
    nItemLevel = std::max(nItemLevel, 1);

    D2ItemDropStrc itemDrop = {};
    itemDrop.nSpawnType = nSpawnTarget;
    itemDrop.nId = nItemId;
    itemDrop.wItemFormat = pGame->wItemFormat;
    itemDrop.nQuality = nQuality;
    itemDrop.pUnit = pPlayer;
    itemDrop.pSeed = nullptr;
    itemDrop.pGame = pGame;
    itemDrop.nX = 0;
    itemDrop.nY = 0;
    itemDrop.pRoom = nullptr;
    itemDrop.wUnitInitFlags = 1;
    itemDrop.dwFlags2 |= (alw1 != 0 ? 2 : 0) | (us1 != 0 ? 8 : 0);
    itemDrop.dwSeed = a1;
    itemDrop.dwItemSeed = alw0;

    if (nItemLevel == -1)
    {
        itemDrop.nItemLvl = ITEMS_GetItemLevelForNewItem(pPlayer, 0);
    }
    else
    {
        itemDrop.nItemLvl = nItemLevel;
    }

    itemDrop.nItemLvl = std::max(itemDrop.nItemLvl, 1);

    D2UnitStrc* pItem = D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, us0);
    if (pItem)
    {
        ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 1);
    }

    return pItem;
}

//D2Game.0x6FC502B0
void __fastcall sub_6FC502B0(D2GameStrc* pGame, D2UnitStrc* pItem)
{
    if (ITEMS_CheckIfSocketable(pItem))
    {
        SUNIT_RemoveAllItemsFromInventory(pGame, pItem);
        INVENTORY_FreeInventory(pItem->pInventory);
    }

    sub_6FC349F0(pGame, pItem);
}

//D2Game.0x6FC502E0
void __fastcall ITEMS_RemoveAll(D2GameStrc* pGame)
{
    D2UnitStrc** ppUnit = pGame->pUnitList[3];
    for (int32_t i = 0; i < 128; ++i)
    {
        D2UnitStrc* pUnit = *ppUnit;
        while (pUnit)
        {
            if (pUnit->dwAnimMode == IMODE_SOCKETED)
            {
                pUnit = SUNIT_GetNextUnitFromList(pUnit);
            }
            else
            {
                SUNIT_RemoveUnit(pGame, pUnit);
                pUnit = *ppUnit;
            }
        }
        ++ppUnit;
    }
}

//D2Game.0x6FC50320
void __fastcall ITEMS_RemoveFromAllPlayers(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    SUNIT_IterateLivingPlayers(pGame, ITEMS_PlayerIterateCallback_RemoveItem, pUnit);
    SUNIT_RemoveUnit(pGame, pUnit);
}

//D2Game.0x6FC50340
void __fastcall ITEMS_PlayerIterateCallback_RemoveItem(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg)
{
    D2UnitStrc* pItem = (D2UnitStrc*)pArg;

    for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
    {
        if (INVENTORY_UnitIsItem(i) == pItem)
        {
            INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pItem);
            return;
        }
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pUnit->pInventory);
    if (pCursorItem && pCursorItem == pItem)
    {
        INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);
    }
}

//D2Game.0x6FC503A0
int32_t __fastcall ITEMS_IsInPlayersInventory(D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2InventoryStrc* pInventoryArg)
{
    D2InventoryStrc* pInventory = pPlayer->pInventory;

    D2_ASSERT(pInventory);

    if (pInventoryArg)
    {
        pInventory = pInventoryArg;
    }

    if (pItem == INVENTORY_GetCursorItem(pInventory))
    {
        return 1;
    }

    for (D2UnitStrc* i = INVENTORY_GetFirstItem(pInventory); i; i = INVENTORY_GetNextItem(i))
    {
        if (pItem == INVENTORY_UnitIsItem(i))
        {
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC50410
D2UnitStrc* __fastcall ITEMS_FindQuestItem(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t dwItemCode)
{
    if (!pUnit->pInventory)
    {
        return nullptr;
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pUnit->pInventory);
    if (pCursorItem && dwItemCode == ITEMS_GetBaseCode(pCursorItem))
    {
        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pCursorItem->dwClassId);
        if (!pItemsTxtRecord || !pItemsTxtRecord->nQuest || pGame->nDifficulty <= STATLIST_UnitGetStatValue(pCursorItem, STAT_QUESTITEMDIFFICULTY, 0))
        {
            return pCursorItem;
        }
    }

    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
    {
        if (INVENTORY_UnitIsItem(pItem) && dwItemCode == ITEMS_GetBaseCode(pItem) && ITEMS_GetInvPage(pItem) != INVPAGE_EQUIP)
        {
            D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
            if (!pItemsTxtRecord || !pItemsTxtRecord->nQuest || !pItemsTxtRecord->nQuestDiffCheck || pGame->nDifficulty <= STATLIST_UnitGetStatValue(pItem, STAT_QUESTITEMDIFFICULTY, 0))
            {
                return pItem;
            }
        }
    }

    return 0;
}

//D2Game.0x6FC504F0
int32_t __fastcall ITEMS_GetItemLevelForNewItem(D2UnitStrc* pUnit, int32_t nLevelId)
{
    if (pUnit)
    {
        int32_t nLevel = 0;
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            nLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
        }
        else if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            nLevel = STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0);
        }

        return std::max(nLevel, 1);
    }

    if (!nLevelId)
    {
        return 1;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    return std::max(DATATBLS_GetMonsterLevelInArea(nLevelId, pGame->nDifficulty, pGame->bExpansion), 1);
}

//D2Game.0x6FC50560
void __fastcall ITEMS_UpdateDurability(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    const int32_t bIsArmor = ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_ARMOR);
    const int32_t bIsWeapon = ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON);
    if (!bIsArmor && !bIsWeapon || !ITEMS_HasDurability(pItem))
    {
        return;
    }

    uint32_t nChance = 4;
    if (bIsArmor)
    {
        nChance = 10;
    }
    else if (ITEMS_CheckIfThrowable(pItem))
    {
        if (!pGame->bExpansion)
        {
            return;
        }

        nChance = 10;
    }

    if (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100 >= nChance)
    {
        return;
    }

    int32_t nDurability = STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0) - 1;
    if (nDurability > 0)
    {
        nDurability = std::min(nDurability, STATLIST_GetMaxDurabilityFromUnit(pItem));

        const int32_t nReplenishDurability = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_REPLENISH_DURABILITY, 0);
        if (nReplenishDurability && !EVENT_GetEventFrame(pGame, pItem, UNITEVENTCALLBACK_STATREGEN))
        {
            EVENT_SetEvent(pGame, pItem, UNITEVENTCALLBACK_STATREGEN, 2500 / nReplenishDurability + pGame->dwGameFrame + 1, 0, 0);
        }

        STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nDurability, 0);
        
        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_DURABILITY, nDurability, 0);
        return;
    }

    if (!bIsWeapon && !ITEMS_CheckItemFlag(pItem, IFLAG_BROKEN, __LINE__, __FILE__))
    {
        sub_6FC4B580(pGame, pUnit, pItem);
        return;
    }
    
    if (ITEMS_GetItemFormat(pItem) < 1u)
    {
        STATLIST_SetUnitStat(pItem, STAT_DURABILITY, 0, 0);
        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_DURABILITY, 0, 0);
        return;
    }
    
    const int32_t nQuantity = STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0);
    if (nQuantity <= 1)
    {
        STATLIST_SetUnitStat(pItem, STAT_DURABILITY, 0, 0);
        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_DURABILITY, 0, 0);
        
        STATLIST_SetUnitStat(pItem, STAT_QUANTITY, 0, 0);
        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_QUANTITY, nQuantity, 0);

        sub_6FC80A30(pGame, pUnit);
    }
    else
    {
        STATLIST_SetUnitStat(pItem, STAT_QUANTITY, nQuantity - 1, 0);
        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_QUANTITY, nQuantity - 1, 0);
        
        const int32_t nMaxDurability = STATLIST_GetMaxDurabilityFromUnit(pItem);
        STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nMaxDurability, 0);
        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_DURABILITY, nMaxDurability, 0);
    }

    if (ITEMS_GetTotalMaxStack(pItem) > 0)
    {
        const int32_t nReplenishDurability = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_REPLENISH_DURABILITY, 0);
        if (nReplenishDurability && !EVENT_GetEventFrame(pGame, pItem, UNITEVENTCALLBACK_STATREGEN))
        {
            EVENT_SetEvent(pGame, pItem, UNITEVENTCALLBACK_STATREGEN, 2500 / nReplenishDurability + pGame->dwGameFrame + 1, 0, 0);
        }
    }
}

//D2Game.0x6FC50820
void __fastcall ITEMS_FillItemDrop(D2GameStrc* pGame, D2ItemDropStrc* pItemDrop, D2UnitStrc* pItem)
{
    memset(pItemDrop, 0, sizeof(D2ItemDropStrc));

    pItemDrop->pGame = pGame;
    pItemDrop->wItemFormat = ITEMS_GetItemFormat(pItem);
    pItemDrop->nItemLvl = std::max(ITEMS_GetItemLevel(pItem), 1);
    pItemDrop->nId = pItem ? pItem->dwClassId : -1;
    pItemDrop->nSpawnType = pItem ? pItem->dwAnimMode : 0;
    pItemDrop->nQuality = ITEMS_GetItemQuality(pItem);
    pItemDrop->nX = CLIENTS_GetUnitX(pItem);
    pItemDrop->nY = CLIENTS_GetUnitY(pItem);

    if (ITEMS_GetItemType(pItem) == ITEMTYPE_GOLD)
    {
        pItemDrop->nQuantity = STATLIST_UnitGetStatValue(pItem, STAT_GOLD, 0);
    }
    else if (ITEMS_CheckIfStackable(pItem))
    {
        const int32_t nQuantity = STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0);

        D2_ASSERT(nQuantity >= 0);

        pItemDrop->nQuantity = nQuantity;
    }

    pItemDrop->wUnitInitFlags = 1;
    pItemDrop->pSeed = &pItem->pSeed;
    pItemDrop->dwSeed = pItem->dwInitSeed;
    pItemDrop->dwItemSeed = ITEMS_GetItemStartSeed(pItem);
    pItemDrop->nItemIndex = ITEMS_GetFileIndex(pItem);
    pItemDrop->dwFlags1 = ITEMS_GetItemFlags(pItem);
    pItemDrop->nMinDur = STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0);
    pItemDrop->nMaxDur = STATLIST_GetUnitBaseStat(pItem, STAT_MAXDURABILITY, 0);
    pItemDrop->nItemIndex = ITEMS_GetFileIndex(pItem);
    pItemDrop->bForce = 0;

    if (ITEMS_GetItemType(pItem) == ITEMTYPE_PLAYER_BODY_PART)
    {
        strcpy_s(pItemDrop->szName, ITEMS_GetEarName(pItem));
        pItemDrop->eEarLvl = ITEMS_GetEarLevel(pItem);
    }
    else if (ITEMS_CheckItemFlag(pItem, IFLAG_PERSONALIZED, __LINE__, __FILE__))
    {
        strcpy_s(pItemDrop->szName, ITEMS_GetEarName(pItem));
    }
}

//D2Game.0x6FC509F0
void __fastcall ITEMS_DropGoldPile(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t nValue)
{
    int32_t nGoldItemId = 0;
    if (!DATATBLS_GetItemRecordFromItemCode(' dlg', &nGoldItemId) || !nValue)
    {
        return;
    }
    
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    uint32_t nRemainingGold = nValue;
    
    uint32_t nDroppedGold = 0;
    while (nDroppedGold < nValue)
    {
        const uint32_t nPileValue = std::min(nRemainingGold, 2'000'000'000u);

        nDroppedGold += nPileValue;
        nRemainingGold -= nPileValue;

        D2CoordStrc unitCoords = {};
        UNITS_GetCoords(pUnit, &unitCoords);

        const int32_t nX = unitCoords.nX + 2;
        const int32_t nY = unitCoords.nY + 3;

        D2CoordStrc spawnCoords = {};
        if (D2GAME_GetRoom_6FC52070(pRoom, nX, nY))
        {
            spawnCoords.nX = nX;
            spawnCoords.nY = nY;
        }
        else
        {
            spawnCoords.nX = unitCoords.nX;
            spawnCoords.nY = unitCoords.nY;
        }

        pRoom = COLLISION_GetFreeCoordinatesWithField(pRoom, &spawnCoords, &unitCoords, 1, 0x3E01u, 0x801u, 1);
        if (pRoom)
        {
            D2UnitStrc* pGoldPile = D2GAME_CreateItemUnit_6FC501A0(pUnit, nGoldItemId, pGame, 4u, ITEMQUAL_NORMAL, 1u, 1u, 1u, 0, 0, 0);
            if (pGoldPile)
            {
                if (nPileValue >= 0 && (pGoldPile->dwUnitType != UNIT_PLAYER || nPileValue <= UNITS_GetInventoryGoldLimit(pGoldPile)))
                {
                    STATLIST_SetUnitStat(pGoldPile, STAT_GOLD, nPileValue, 0);
                }
                else
                {
                    STATLIST_SetUnitStat(pGoldPile, STAT_GOLD, 0, 0);
                }

                D2GAME_DropItem_6FC52260(pGame, pUnit, pGoldPile, pRoom, spawnCoords.nX, spawnCoords.nY);
            }
        }
    }
}

//D2Game.0x6FC50C50
void __fastcall ITEMS_HandleGoldTransaction(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t nValue)
{
    const int32_t nGoldLimit = UNITS_GetInventoryGoldLimit(pUnit);
    const int32_t nGold = STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0);
    if (nGold == nGoldLimit)
    {
        ITEMS_DropGoldPile(pGame, pUnit, nValue);
    }
    else if (nGold + nValue <= nGoldLimit)
    {
        PLRTRADE_AddGold(pUnit, STAT_GOLD, nValue);
    }
    else
    {
        PLRTRADE_AddGold(pUnit, STAT_GOLD, nGoldLimit - nGold);
        ITEMS_DropGoldPile(pGame, pUnit, nValue + nGold - nGoldLimit);
    }
}

//D2Game.0x6FC50D80
void __fastcall ITEMS_DropPlayerEar(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    int32_t nItemId = 0;
    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER || !DATATBLS_GetItemRecordFromItemCode(' rae', &nItemId))
    {
        return;
    }

    D2CoordStrc coords1 = {};
    D2CoordStrc unitCoords = {};

    UNITS_GetCoords(pUnit, &unitCoords);
    
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (D2GAME_GetRoom_6FC52070(pRoom, unitCoords.nX + 2, unitCoords.nY + 3))
    {
        coords1.nX = unitCoords.nX + 2;
        coords1.nY = unitCoords.nY + 3;
    }
    else
    {
        coords1.nX = unitCoords.nX;
        coords1.nY = unitCoords.nY;
    }

    D2ActiveRoomStrc* pTargetRoom = COLLISION_GetFreeCoordinatesWithField(pRoom, &coords1, &unitCoords, 1, 0x3E01u, 0x801u, 1);
    if (!pTargetRoom)
    {
        return;
    }

    D2UnitStrc* pItem = D2GAME_CreateItemUnit_6FC501A0(pUnit, nItemId, pGame, 4u, 2u, 1u, 1u, STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0), 0, 0, 0);
    if (!pItem)
    {
        return;
    }

    UNITS_InitializeStaticPath(pItem, pTargetRoom, coords1.nX, coords1.nY);
    UNITS_BlockCollisionPath(pItem, pTargetRoom, coords1.nX, coords1.nY);
    UNITROOM_AddUnitToRoom(pItem, pTargetRoom);
    pItem->dwFlags |= UNITFLAG_SENDREFRESHMSG | UNITFLAG_TARGETABLE;
    UNITS_ChangeAnimMode(pItem, IMODE_ONGROUND);
    ITEMS_SetInvPage(pItem, INVPAGE_NULL);
    pItem->dwFlags |= IFLAG_LOWQUALITY;
    pItem->pItemData->dwActionStamp = ITEMS_GetGroundRemovalTime(pGame, pItem);
}

//D2Game.0x6FC50F70
int32_t __fastcall ITEMS_GetHealthPotionDropCode(D2UnitStrc* pUnit)
{
    constexpr int32_t healthPotionCodes[] =
    {
        ' 1ph', ' 2ph', ' 3ph', ' 4ph'
    };

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    D2_ASSERT(pRoom);

    int32_t nIndex = DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom));
    if (pUnit->pGame && pUnit->pGame->nDifficulty != DIFFMODE_NORMAL)
    {
        nIndex += 2;
    }

    if (nIndex < std::size(healthPotionCodes))
    {
        return healthPotionCodes[nIndex];
    }

    return healthPotionCodes[3];
}

//D2Game.0x6FC50FF0
int32_t __fastcall ITEMS_GetManaPotionDropCode(D2UnitStrc* pUnit)
{
    constexpr int32_t manaPotionCodes[] =
    {
        ' 1pm', ' 2pm', ' 3pm', ' 4pm'
    };

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    D2_ASSERT(pRoom);

    int32_t nIndex = DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom));
    if (pUnit->pGame && pUnit->pGame->nDifficulty != DIFFMODE_NORMAL)
    {
        nIndex += 2;
    }

    if (nIndex < std::size(manaPotionCodes))
    {
        return manaPotionCodes[nIndex];
    }

    return manaPotionCodes[3];
}

//D2Game.0x6FC51070
D2UnitStrc* __fastcall ITEMS_Duplicate(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pOwner, int32_t bDuplicateSocketFillers)
{
    D2ItemSaveStrc itemSave = {};
    uint8_t bitstream[1024] = {};

    const uint32_t nNextFreePos = ITEMS_SerializeItemToBitstream(pItem, bitstream, sizeof(bitstream), 1, 1, 0);
    ITEMS_GetCompactItemDataFromBitstream(bitstream, nNextFreePos, 1, &itemSave);

    uint32_t nSize = 0;
    D2UnitStrc* pDupe = sub_6FC4EC10(pGame, UNITS_GetRoom(pItem), bitstream, nNextFreePos, 1, &itemSave, &nSize, 96u);
    if (!pDupe)
    {
        return 0;
    }

    uint8_t* pBitstream = &bitstream[nSize];
    uint32_t nRemainingSize = nNextFreePos - nSize;

    ITEMS_SetItemFlag(pDupe, IFLAG_INIT, 1);
    ITEMS_SetItemFlag(pDupe, IFLAG_INSTORE, 0);

    if (bDuplicateSocketFillers && itemSave.nItemFileIndex)
    {
        for (int32_t i = 0; i < itemSave.nItemFileIndex; ++i)
        {
            ITEMS_GetCompactItemDataFromBitstream(pBitstream, nRemainingSize, 1, &itemSave);
            
            D2UnitStrc* pDupedSocketable = sub_6FC4EC10(pGame, 0, pBitstream, nRemainingSize, 1, &itemSave, &nSize, 96);
            if (!pDupedSocketable)
            {
                return nullptr;
            }

            pBitstream += nSize;
            nRemainingSize -= nSize;

            UNITS_ChangeAnimMode(pDupedSocketable, IMODE_ONCURSOR);

            int32_t bPlaced = 0;
            if (!D2GAME_ITEMSOCKET_PlaceItem_6FC497E0(pGame, pDupe, pDupedSocketable->dwUnitId, pDupe->dwUnitId, &bPlaced, 0, 1u, 0))
            {
                FOG_DisplayAssert("0", __FILE__, __LINE__);
                exit(-1);
            }

            ITEMS_SetItemFlag(pDupedSocketable, IFLAG_INIT, 1);
            ITEMS_SetItemFlag(pDupedSocketable, IFLAG_INSTORE, 0);
            ITEMS_SetItemCMDFlag(pDupedSocketable, 1, 0);
        }
    }

    ITEMS_SetItemFlag(pItem, IFLAG_ITEM, 1);

    sub_6FC512C0(pGame, pDupe);
    sub_6FC51310(pGame, pDupe);

    D2GAME_ResetSomeItemFlags_6FC415F0(pGame, pDupe);
    ITEMS_SetItemCMDFlag(pDupe, 1, 0);

    return pDupe;
}

//D2Game.0x6FC512C0
void __fastcall sub_6FC512C0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    const int32_t nReplenishDurability = STATLIST_UnitGetStatValue(pUnit, STAT_ITEM_REPLENISH_DURABILITY, 0);
    if (nReplenishDurability && !EVENT_GetEventFrame(pGame, pUnit, UNITEVENTCALLBACK_STATREGEN))
    {
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_STATREGEN, 2500 / nReplenishDurability + pGame->dwGameFrame + 1, 0, 0);
    }
}

//D2Game.0x6FC51310
void __fastcall sub_6FC51310(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    const int32_t nReplenishQuantity = STATLIST_UnitGetStatValue(pUnit, STAT_ITEM_REPLENISH_QUANTITY, 0);
    if (nReplenishQuantity && !EVENT_GetEventFrame(pGame, pUnit, UNITEVENTCALLBACK_STATREGEN))
    {
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_STATREGEN, 2500 / nReplenishQuantity + pGame->dwGameFrame + 1, 0, 0);
    }
}

//Inlined in D2Game.0x6FC51360
D2ItemTypesTxt* __fastcall ITEMS_GetItemTypesTxtRecord(int32_t nItemType)
{
    if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
    {
        return &sgptDataTables->pItemTypesTxt[nItemType];
    }

    return nullptr;
}

//D2Game.0x6FC51360
void __fastcall D2GAME_DropTC_6FC51360(D2GameStrc* pGame, D2UnitStrc* pMonster, D2UnitStrc* pPlayer, D2TCExShortStrc* pTCTxtRecord, int32_t nQuality, int32_t nLvl, int32_t a7, D2UnitStrc** ppItems, int32_t* pnItemsDropped, int32_t nMaxItems)
{
    D2_ASSERT(pTCTxtRecord);

    int32_t nItemsDropped = 0;
    if (!pnItemsDropped)
    {
        pnItemsDropped = &nItemsDropped;
    }
    *pnItemsDropped = 0;

    D2_ASSERT(!ppItems || nMaxItems);
    
    if (!ppItems && nMaxItems <= 0)
    {
        nMaxItems = 6;
    }

    D2TCArrayStrc tcArray[64] = {};

    tcArray[0].pTC = pTCTxtRecord;
    tcArray[0].nPicks = std::max(std::abs(pTCTxtRecord->nPicks), 1);
    tcArray[0].nMagic = pTCTxtRecord->nMagic;
    tcArray[0].nRare = pTCTxtRecord->nRare;
    tcArray[0].nSet = pTCTxtRecord->nSet;
    tcArray[0].nUnique = pTCTxtRecord->nUnique;
    tcArray[0].nSuperior = pTCTxtRecord->nSuperior;
    tcArray[0].nNormal = pTCTxtRecord->nNormal;

    int32_t nThrowableCounter = 0;
    int32_t nCounter = 1;
    while (nCounter)
    {
        --nCounter;

        while (tcArray[nCounter].pTC)
        {
            D2TCExShortStrc* pTC = tcArray[nCounter].pTC;
            int32_t nChance = pTC->nClassic;
            if (pGame->bExpansion)
            {
                nChance = pTC->nProb;
            }

            if (!nChance || !tcArray[nCounter].nPicks)
            {
                break;
            }

            int32_t nRand = 0;
            if (pTC->nPicks < 0)
            {
                nRand = -pTC->nPicks - tcArray[nCounter].nPicks;
                if (nRand >= nChance)
                {
                    break;
                }
            }
            else
            {
                int32_t nNoDrop = pTC->nNoDrop;
                if (pTC->nNoDrop)
                {
                    int32_t nPartyMembers = 1;
                    if (pPlayer)
                    {
                        D2UnitStrc* pOwner = pPlayer;
                        if (pPlayer->dwUnitType != UNIT_PLAYER)
                        {
                            pOwner = AIGENERAL_GetMinionOwner(pPlayer);
                        }

                        if (pOwner && pOwner->dwUnitType == UNIT_PLAYER)
                        {
                            nPartyMembers = std::max(nPartyMembers, std::min(PARTY_GetLivingPartyMemberCountInSameLevel(pGame, pOwner), 8));
                        }
                    }

                    uint32_t nPlayerCount = (PLAYER_GetPlayerCount(pGame) - nPartyMembers) / 2 + nPartyMembers;
                    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER)
                    {
                        nPlayerCount = std::min<uint32_t>(nPlayerCount, std::max(STATLIST_UnitGetStatValue(pMonster, STAT_MONSTER_PLAYERCOUNT, 0), 1));
                    }

                    if (nPlayerCount > 1)
                    {
                        double noDropRatio = (double)nNoDrop / (double)(nChance + nNoDrop);
                        const double baseRatio = noDropRatio;

                        for (int32_t i = nPlayerCount - 1; i > 0; --i)
                        {
                            noDropRatio *= baseRatio;
                        }

                        const double inverseRatio = 1.0 - noDropRatio;
                        if (inverseRatio == 0.0)
                        {
                            nNoDrop = 0;
                        }
                        else
                        {
                            nNoDrop = (int32_t)((double)nChance / inverseRatio * (1.0 - inverseRatio));
                        }
                    }
                }

                if (!a7)
                {
                    nChance += nNoDrop;
                }

                nRand = ITEMS_RollLimitedRandomNumber(&pMonster->pSeed, nChance);

                if (!a7)
                {
                    if (nRand < nNoDrop)
                    {
                        --tcArray[nCounter].nPicks;
                        continue;
                    }

                    nRand -= nNoDrop;
                }
            }

            --tcArray[nCounter].nPicks;

            int32_t nIndex = 0;
            if (!pGame->bExpansion)
            {
                while (nIndex < pTC->nTypes)
                {
                    if (!(pTC->pInfo[nIndex].nFlags & 0x10) && (nIndex >= pTC->nTypes - 1 || pTC->pInfo[nIndex + 1].nClassic > nRand))
                    {
                        break;
                    }

                    ++nIndex;
                }
            }
            else
            {
                int32_t nLowerBound = 0;
                int32_t nUpperBound = pTC->nTypes;
                while (nLowerBound < nUpperBound)
                {
                    nIndex = nLowerBound + (nUpperBound - nLowerBound) / 2;
                    if (nRand > pTC->pInfo[nIndex].nProb)
                    {
                        nLowerBound = nIndex + 1;
                    }
                    else if (nRand < pTC->pInfo[nIndex].nProb)
                    {
                        nUpperBound = nIndex;
                    }
                    else // nRand == pTC->pInfo[nIndex].nProb
                    {
                        break;
                    }
                }

                nIndex = std::max(nLowerBound - 1, 0);
            }

            if (nIndex < pTC->nTypes)
            {
                D2TCExInfoStrc* pTCExInfo = &pTC->pInfo[nIndex];
                if (pTCExInfo->nFlags & 4)
                {
                    D2TCExShortStrc* pTCExTxtRecord = DATATBLS_GetTreasureClassExRecordFromIdAndLevel(pTCExInfo->nItemId, 0);
                    const int32_t nOldCounter = nCounter;
                    if (tcArray[nCounter].nPicks > 0)
                    {
                        ++nCounter;
                    }

                    int32_t nTCStackTop = nCounter;
                    D2_ASSERT(nTCStackTop < 64);

                    tcArray[nTCStackTop].pTC = pTCExTxtRecord;
                    tcArray[nTCStackTop].nPicks = std::max(std::abs(pTCExTxtRecord->nPicks), 1);

                    int16_t* pCurrent = &tcArray[nTCStackTop].nMagic;
                    int16_t* pTCValue = &pTCExTxtRecord->nMagic;
                    int16_t* pOld = &tcArray[nOldCounter].nMagic;

                    for (int32_t i = 0; i < 6; ++i)
                    {
                        int16_t nVal = *pOld;
                        if (!*pOld || *pTCValue > *pOld)
                        {
                            nVal = *pTCValue;
                        }
                        *pCurrent = nVal;

                        ++pTCValue;
                        ++pOld;
                        ++pCurrent;
                    }
                }

                const int16_t nItemId = pTCExInfo->nItemId;
                if (nItemId != -1)
                {
                    if (!pGame->bExpansion)
                    {
                        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);
                        if (!pItemsTxtRecord || pItemsTxtRecord->wVersion >= 100u)
                        {
                            continue;
                        }

                        int16_t* pCurrent = &tcArray[nCounter].nMagic;
                        int16_t* pTCValue = &pTCExInfo->nMagic;
                        for (int32_t i = 0; i < 6; ++i)
                        {
                            if (*pTCValue > *pCurrent)
                            {
                                *pCurrent = *pTCValue;
                            }

                            ++pTCValue;
                            ++pCurrent;
                        }
                    }

                    int32_t nItemQuality = 0;
                    int32_t nItemIndex = 0;
                    if (pTCExInfo->nFlags & 1)
                    {
                        nItemQuality = ITEMQUAL_UNIQUE;
                        nItemIndex = pTCExInfo->nTxtRow + 1;
                    }
                    else if (pTCExInfo->nFlags & 2)
                    {
                        nItemQuality = ITEMQUAL_SET;
                        nItemIndex = pTCExInfo->nTxtRow + 1;
                    }
                    else
                    {
                        nItemQuality = nQuality;
                        if (!nItemQuality)
                        {
                            D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);
                            if (!pItemsTxtRecord)
                            {
                                nItemQuality = 0;
                            }
                            else
                            {
                                D2ItemTypesTxt* pItemTypesTxtRecord = ITEMS_GetItemTypesTxtRecord(ITEMS_GetItemTypeFromItemId(nItemId));
                                if (!pItemTypesTxtRecord)
                                {
                                    nItemQuality = 0;
                                }
                                else
                                {
                                    if (pItemTypesTxtRecord->nNormal)
                                    {
                                        nItemQuality = ITEMQUAL_NORMAL;
                                    }
                                    else if (pItemsTxtRecord->nUnique || (pItemTypesTxtRecord->nMagic && pItemsTxtRecord->nQuest))
                                    {
                                        nItemQuality = ITEMQUAL_UNIQUE;
                                    }
                                    else
                                    {
                                        D2ItemRatioTxt* pItemRatioTxtRecord = DATATBLS_GetItemRatioTxtRecord(nItemId, pGame->nDifficulty, 100u);
                                        D2_ASSERT(pItemRatioTxtRecord);

                                        const int32_t nLevelDiff = nLvl - pItemsTxtRecord->nLevel;
                                        int32_t nMagicBonus = 0;
                                        if (pPlayer && (pPlayer->dwUnitType == UNIT_PLAYER || pPlayer->dwUnitType == UNIT_MONSTER))
                                        {
                                            nMagicBonus = STATLIST_UnitGetItemStatOrSkillStatValue(pPlayer, STAT_ITEM_MAGICBONUS, 0);
                                            D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pPlayer);
                                            if (pOwner)
                                            {
                                                nMagicBonus += STATLIST_UnitGetItemStatOrSkillStatValue(pOwner, STAT_ITEM_MAGICBONUS, 0);
                                            }
                                        }

                                        if (nMagicBonus == 0 || nMagicBonus > -100)
                                        {
                                            const int32_t nMagicDivisor = nMagicBonus + 100;
                                            int32_t nUniqueBase = (pItemRatioTxtRecord->dwUnique - nLevelDiff / pItemRatioTxtRecord->dwUniqueDivisor) << 7;
                                            if (nMagicBonus != 0)
                                            {
                                                const int32_t nDivisor = nMagicDivisor > 110 ? 50 * (5 * nMagicDivisor - 500) / (nMagicDivisor + 150) + 100 : nMagicDivisor;
                                                if (nDivisor)
                                                {
                                                    nUniqueBase = 12800 * (pItemRatioTxtRecord->dwUnique - nLevelDiff / pItemRatioTxtRecord->dwUniqueDivisor) / nDivisor;
                                                }
                                            }

                                            nUniqueBase = std::max(nUniqueBase, pItemRatioTxtRecord->dwUniqueMin);

                                            const int32_t nUniqueChance = nUniqueBase - nUniqueBase * tcArray[nCounter].nUnique / 1024;
                                            if (nUniqueChance <= 0 || ITEMS_RollLimitedRandomNumber(&pMonster->pSeed, nUniqueChance) < 128)
                                            {
                                                nItemQuality = ITEMQUAL_UNIQUE;
                                            }
                                            else
                                            {
                                                int32_t nSetBase = (pItemRatioTxtRecord->dwSet - nLevelDiff / pItemRatioTxtRecord->dwSetDivisor) << 7;
                                                if (nMagicBonus != 0)
                                                {
                                                    const int32_t nDivisor = nMagicDivisor > 110 ? 100 * (5 * nMagicDivisor - 500) / (nMagicDivisor + 400) + 100 : nMagicDivisor;
                                                    if (nDivisor)
                                                    {
                                                        nSetBase = 12800 * (pItemRatioTxtRecord->dwSet - nLevelDiff / pItemRatioTxtRecord->dwSetDivisor) / nDivisor;
                                                    }
                                                }

                                                nSetBase = std::max(nSetBase, pItemRatioTxtRecord->dwSetMin);

                                                const int32_t nSetChance = nSetBase - nSetBase * tcArray[nCounter].nSet / 1024;
                                                if (nSetChance <= 0 || ITEMS_RollLimitedRandomNumber(&pMonster->pSeed, nSetChance) < 128)
                                                {
                                                    nItemQuality = ITEMQUAL_SET;
                                                }
                                                else
                                                {
                                                    if (pItemTypesTxtRecord->nRare)
                                                    {
                                                        int32_t nRareBase = (pItemRatioTxtRecord->dwRare - nLevelDiff / pItemRatioTxtRecord->dwRareDivisor) << 7;
                                                        if (nMagicBonus != 0)
                                                        {
                                                            const int32_t nDivisor = nMagicDivisor > 110 ? 200 * (3 * nMagicDivisor - 300) / (nMagicDivisor + 500) + 100 : nMagicDivisor;
                                                            if (nDivisor)
                                                            {
                                                                nRareBase = 12800 * (pItemRatioTxtRecord->dwRare - nLevelDiff / pItemRatioTxtRecord->dwRareDivisor) / nDivisor;
                                                            }
                                                        }

                                                        nRareBase = std::max(nRareBase, pItemRatioTxtRecord->dwRareMin);

                                                        const int32_t nRareChance = nRareBase - nRareBase * tcArray[nCounter].nRare / 1024;
                                                        if (nRareChance <= 0 || (uint32_t)ITEMS_RollLimitedRandomNumber(&pMonster->pSeed, nRareChance) < 128)
                                                        {
                                                            nItemQuality = ITEMQUAL_RARE;
                                                        }
                                                    }

                                                    if (!nItemQuality)
                                                    {
                                                        if (pItemTypesTxtRecord->nMagic)
                                                        {
                                                            nItemQuality = ITEMQUAL_MAGIC;
                                                        }
                                                        else
                                                        {
                                                            int32_t nMagicBase = (pItemRatioTxtRecord->dwMagic - nLevelDiff / pItemRatioTxtRecord->dwMagicDivisor) << 7;
                                                            if (nMagicBonus != 0)
                                                            {
                                                                nMagicBase = 12800 * (pItemRatioTxtRecord->dwMagic - nLevelDiff / pItemRatioTxtRecord->dwMagicDivisor) / nMagicDivisor;
                                                            }

                                                            nMagicBase = std::max(nMagicBase, pItemRatioTxtRecord->dwMagicMin);

                                                            const int32_t nMagicChance = nMagicBase - nMagicBase * tcArray[nCounter].nMagic / 1024;
                                                            if (nMagicChance <= 0 || ITEMS_RollLimitedRandomNumber(&pMonster->pSeed, nMagicChance) < 128)
                                                            {
                                                                nItemQuality = ITEMQUAL_MAGIC;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        
                                        if (!nItemQuality)
                                        {
                                            const int32_t nHighQualityChance = (pItemRatioTxtRecord->dwHiQuality - nLevelDiff / pItemRatioTxtRecord->dwHiQualityDivisor) << 7;
                                            if (nHighQualityChance <= 0 || (uint32_t)ITEMS_RollLimitedRandomNumber(&pMonster->pSeed, nHighQualityChance) < 128)
                                            {
                                                nItemQuality = ITEMQUAL_SUPERIOR;
                                            }
                                            else
                                            {
                                                const int32_t nNormalChance = (pItemRatioTxtRecord->dwNormal - nLevelDiff / pItemRatioTxtRecord->dwNormalDivisor) << 7;
                                                if (nNormalChance <= 0 || ITEMS_RollLimitedRandomNumber(&pMonster->pSeed, nNormalChance) < 128)
                                                {
                                                    nItemQuality = ITEMQUAL_NORMAL;
                                                }
                                                else
                                                {
                                                    nItemQuality = ITEMQUAL_INFERIOR;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    uint32_t nFlags = 0;
                    const int32_t nSuperiorChance = tcArray[nCounter].nSuperior;
                    if (nSuperiorChance && (ITEMS_RollRandomNumber(&pMonster->pSeed) & 1023u) < nSuperiorChance)
                    {
                        nFlags = 4;
                    }

                    const int32_t nNormalChance = tcArray[nCounter].nNormal;
                    if (nNormalChance && (ITEMS_RollRandomNumber(&pMonster->pSeed) & 1023u) < nNormalChance)
                    {
                        nFlags |= 0x10;
                    }

                    int32_t nId = nItemId;
                    if (!pGame->bExpansion && ITEMS_CheckItemTypeIfThrowable(ITEMS_GetItemTypeFromItemId(nItemId)))
                    {
                        ++nThrowableCounter;
                        if (nThrowableCounter <= 10)
                        {
                            ++tcArray[nCounter].nPicks;
                            nId = -1;
                        }
                        else
                        {
                            nId = DATATBLS_GetItemIdFromItemCode(' dsl');
                        }
                    }

                    if (nId >= 0)
                    {
                        D2CoordStrc coords = {};
                        UNITS_GetCoords(pMonster, &coords);

                        D2CoordStrc itemCoords = {};
                        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMonster);
                        if (D2GAME_GetRoom_6FC52070(pRoom, coords.nX + 2, coords.nY + 3))
                        {
                            itemCoords.nX = coords.nX + 2;
                            itemCoords.nY = coords.nY + 3;
                        }
                        else
                        {
                            itemCoords.nX = coords.nX;
                            itemCoords.nY = coords.nY;
                        }

                        D2ActiveRoomStrc* pFreeCoordsRoom = COLLISION_GetFreeCoordinatesWithField(pRoom, &itemCoords, &coords, 1, 0x3E01u, 0x801u, 1);
                        if (pFreeCoordsRoom)
                        {
                            D2ItemDropStrc itemDrop = {};
                            itemDrop.pSeed = nullptr;
                            itemDrop.pRoom = pFreeCoordsRoom;
                            itemDrop.nId = nId;
                            itemDrop.wItemFormat = pGame->wItemFormat;
                            itemDrop.nX = itemCoords.nX;
                            itemDrop.nY = itemCoords.nY;
                            itemDrop.nQuality = nItemQuality;
                            itemDrop.pUnit = pMonster;
                            itemDrop.pGame = pGame;
                            itemDrop.nSpawnType = 3;
                            itemDrop.wUnitInitFlags = 1;
                            itemDrop.nItemLvl = ITEMS_GetItemLevelForNewItem(pMonster, 0);
                            itemDrop.nItemIndex = nItemIndex;

                            if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->dwClassId == MONSTER_HELLBOVINE)
                            {
                                itemDrop.dwFlags2 = 1;
                            }

                            itemDrop.dwFlags2 |= nFlags;

                            D2UnitStrc* pItem = D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
                            if (pItem)
                            {
                                if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_GOLD) && pTCExInfo->nTxtRow)
                                {
                                    D2GAME_SetStatOrResetGold_6FC7CA70(pItem, STAT_GOLD, pTCExInfo->nTxtRow * STATLIST_UnitGetStatValue(pItem, STAT_GOLD, 0) >> 8);
                                }

                                if (ppItems)
                                {
                                    ppItems[*pnItemsDropped] = pItem;
                                }

                                ++*pnItemsDropped;
                                if (*pnItemsDropped >= nMaxItems)
                                {
                                    return;
                                }

                                if (pPlayer && ITEMS_GetItemType(pItem) == ITEMTYPE_GOLD)
                                {
                                    int32_t nGoldBonus = STATLIST_UnitGetItemStatOrSkillStatValue(pPlayer, STAT_ITEM_GOLDBONUS, 0) + 100;
                                    D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pPlayer);
                                    if (pOwner)
                                    {
                                        nGoldBonus += STATLIST_UnitGetItemStatOrSkillStatValue(pOwner, STAT_ITEM_GOLDBONUS, 0);
                                    }

                                    D2GAME_SetStatOrResetGold_6FC7CA70(pItem, STAT_GOLD, nGoldBonus * STATLIST_UnitGetStatValue(pItem, STAT_GOLD, 0) / 100);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//D2Game.0x6FC52070
D2ActiveRoomStrc* __fastcall D2GAME_GetRoom_6FC52070(D2ActiveRoomStrc* pRoom, int32_t nSubtileX, int32_t nSubtileY)
{
    return DUNGEON_GetRoomAtPosition(pRoom, nSubtileX, nSubtileY);
}

//D2Game.0x6FC52110
void __fastcall sub_6FC52110(D2GameStrc* pGame, D2UnitStrc* pMonster, D2UnitStrc* pPlayer, int32_t nTCId, int32_t nQuality, int32_t nItemLevel, int32_t a7, D2UnitStrc** ppItems, int32_t* pnItemsDropped, int32_t nMaxItems)
{
    int32_t nLevel = 0;
    if (pGame->bExpansion && pGame->nDifficulty != DIFFMODE_NORMAL && pMonster && pMonster->dwUnitType == UNIT_MONSTER)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pMonster->dwClassId);
        if (pMonStatsTxtRecord && !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO]) && !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_BOSS]))
        {
            nLevel = STATLIST_UnitGetStatValue(pMonster, STAT_LEVEL, 0);
        }
    }

    D2TCExShortStrc* pTCRecord = DATATBLS_GetTreasureClassExRecordFromIdAndLevel(nTCId, nLevel);
    if (pTCRecord)
    {
        D2GAME_DropTC_6FC51360(pGame, pMonster, pPlayer, pTCRecord, nQuality, nItemLevel, a7, ppItems, pnItemsDropped, nMaxItems);
    }
}

//D2Game.0x6FC521D0
int32_t __fastcall ITEMS_GetGroundRemovalTime(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (ITEMS_IsQuestItem(pUnit))
    {
        return 0;
    }

	const int32_t nItemQuality = ITEMS_GetItemQuality(pUnit);
    if (nItemQuality > ITEMQUAL_MAGIC && nItemQuality <= ITEMQUAL_TEMPERED)
    {
        return pGame->dwGameFrame + 45000;
    }

    if (ITEMS_GetItemType(pUnit) == ITEMTYPE_GOLD)
    {
		const int32_t nGoldQuantity = STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0);
        if (nGoldQuantity <= 10000)
        {
            return pGame->dwGameFrame + 15000;
        }

		return pGame->dwGameFrame + 45000;
    }

    if (!ITEMS_IsSocketFiller(pUnit))
    {
        return pGame->dwGameFrame + 15000;
    }

    return pGame->dwGameFrame + 30000;
}

//D2Game.0x6FC52260
void __fastcall D2GAME_DropItem_6FC52260(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY)
{
    if (!pRoom)
    {
        return;
    }

    UNITS_InitializeStaticPath(pItem, pRoom, nX, nY);
    UNITS_BlockCollisionPath(pItem, pRoom, nX, nY);
    UNITROOM_AddUnitToRoom(pItem, pRoom);

    if (pItem)
    {
        pItem->dwFlags |= UNITFLAG_SENDREFRESHMSG | UNITFLAG_TARGETABLE;
    }

    UNITS_ChangeAnimMode(pItem, IMODE_ONGROUND);
    ITEMS_SetInvPage(pItem, INVPAGE_NULL);

    if (pItem)
    {
        pItem->dwFlags |= UNITFLAG_ISRESURRECT;
    }

    pItem->pItemData->dwActionStamp = ITEMS_GetGroundRemovalTime(pGame, pItem);

    if (pUnit)
    {
        QUESTS_ItemDropped(pGame, pUnit, pItem);
    }
}

//D2Game.0x6FC52300
void __fastcall ITEMS_DeleteInactiveItems(D2GameStrc* pGame, int32_t nAct)
{
    if (!pGame->pAct[nAct])
    {
        return;
    }

    for (D2ActiveRoomStrc* i = DUNGEON_GetRoomFromAct(pGame->pAct[nAct]); i; i = i->pRoomNext)
    {
        D2UnitStrc* pItem = i->pUnitFirst;
        while (pItem)
        {
            D2UnitStrc* pNext = pItem->pRoomNext;
            if (pItem->dwUnitType == UNIT_ITEM)
            {
                const int32_t nDeletionFrame = pItem->pItemData->dwActionStamp;
                if (nDeletionFrame && pGame->dwGameFrame >= nDeletionFrame)
                {
                    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pItem);
                    if (pRoom)
                    {
                        DUNGEON_AllocDrlgDelete(pRoom, pItem->dwUnitType, pItem->dwUnitId);
                        UNITS_FreeCollisionPath(pItem);
                        UNITROOM_RemoveUnitFromRoom(pItem);
                    }

                    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
                    ITEMS_RemoveFromAllPlayers(pGame, pItem);
                }
            }
            pItem = pNext;
        }
    }
}

//D2Game.0x6FC523B0
void __fastcall ITEMS_DestroyRunewordStatList(D2GameStrc* pGame, D2UnitStrc* pItem)
{
    if (!pItem || pItem->dwUnitType != UNIT_ITEM || !(ITEMS_GetItemFlags(pItem) & IFLAG_RUNEWORD))
    {
        return;
    }

    ITEMS_SetItemFlag(pItem, IFLAG_RUNEWORD, 0);
    ITEMS_AssignPrefix(pItem, 0, 0);

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitStateOrFlag(pItem, STATE_RUNEWORD, 0x40);
    if (pStatList)
    {
        D2Common_10474(pItem, pStatList);
        STATLIST_FreeStatList(pStatList);
    }
}

//D2Game.0x6FC52410
void __fastcall sub_6FC52410(D2UnitStrc* pUnit, D2ItemDropStrc* pItemDrop)
{
    const int32_t nClassId = ITEMS_GetStaffMods(pUnit);
    if (nClassId < 0 || nClassId >= NUMBER_OF_PLAYERCLASSES)
    {
        return;
    }
    
    const int32_t nSkillCount = SKILLS_GetPlayerSkillCount(nClassId);
    if (nSkillCount <= 0)
    {
        return;
    }

    const int32_t nSkillId = SKILLS_GetClassSkillId(nClassId, 0);
    const int32_t nItemLevel = pItemDrop->nItemLvl;
    if (ITEMS_GetItemFormat(pUnit) != 0)
    {
        sub_6FC52650(pUnit, nItemLevel, nSkillId, nSkillCount, pItemDrop->dwFlags2 & 0x20 ? pItemDrop->nItemLvl : 0);
    }
    else
    {
        sub_6FC55060(pUnit, nItemLevel, nSkillId, 0);
    }
}

//D2Game.0x6FC52650
void __fastcall sub_6FC52650(D2UnitStrc* pUnit, int32_t nItemLvl, int32_t nClassFirstSkillId, int32_t nSkillCount, int32_t nItemDropLvl)
{
    D2_MAYBE_UNUSED(nSkillCount);
    int32_t nRand = nItemDropLvl + ITEMS_RollRandomNumber(ITEMS_GetItemSeed(pUnit)) % 100;
    int32_t nCount = 0;
    if (nRand > 90)
    {
        nCount = 3;
    }
    else if (nRand > 70)
    {
        nCount = 2;
    }
    else if (nRand > 30 || nItemDropLvl)
    {
        nCount = 1;
    }
    else
    {
        return;
    }

    int32_t nBase = 1;
    if (nItemLvl > 36 && ITEMS_GetItemFormat(pUnit) >= 100u)
    {
        nBase = 5;
    }
    else if (nItemLvl > 24)
    {
        nBase = 4;
    }
    else if (nItemLvl > 18)
    {
        nBase = 3;
    }
    else if (nItemLvl > 11)
    {
        nBase = 2;
    }

    int32_t skillIds[3] = { -1, -1, -1 };

    for (int32_t j = 0; j < nCount; ++j)
    {
        nRand = ITEMS_RollRandomNumber(ITEMS_GetItemSeed(pUnit)) % 100;
        int32_t nBaseLayer = 0;
        if (nRand > 80)
        {
            nBaseLayer = nBase + 1;
        }
        else if (nRand > 30)
        {
            nBaseLayer = nBase;
        }
        else if (nRand > 10)
        {
            nBaseLayer = nBase - 1;
        }
        else
        {
            nBaseLayer = nBase - 2;
        }

        nBaseLayer = std::max(nBaseLayer, 1);

        if (ITEMS_GetItemQuality(pUnit) == ITEMQUAL_INFERIOR && nBaseLayer >= 4)
        {
            nBaseLayer = 4;
        }

        nBaseLayer = nClassFirstSkillId + 5 * (nBaseLayer - 1);
        int32_t nSkillId = 0;
        for (int32_t i = 0; i < 6; ++i)
        {
            nSkillId = nBaseLayer + ITEMS_RollRandomNumber(ITEMS_GetItemSeed(pUnit)) % 5;

            D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
            if (!pSkillsTxtRecord || pSkillsTxtRecord->nITypeA[0] <= 0 || ITEMS_CheckItemTypeId(pUnit, pSkillsTxtRecord->nITypeA[0]))
            {
                if (nSkillId != skillIds[0] && nSkillId != skillIds[1] && nSkillId != skillIds[2])
                {
                    skillIds[j] = nSkillId;
                    break;
                }
            }
        }

        int32_t nValue = 1;
        if (ITEMS_GetItemFormat(pUnit) < 100u || ITEMS_GetItemQuality(pUnit) != ITEMQUAL_INFERIOR)
        {
            nRand = nItemDropLvl / 2 + ITEMS_RollRandomNumber(ITEMS_GetItemSeed(pUnit)) % 100;
            if (nRand >= 90)
            {
                nValue = 3;
            }
            else if (nRand >= 60)
            {
                nValue = 2;
            }
        }

        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitStateAndFlag(pUnit, 0, 0x40u);
        if (!pStatList)
        {
            pStatList = STATLIST_AllocStatList(pUnit ? pUnit->pMemoryPool : nullptr, 0x40u, 0, UNIT_ITEM, pUnit ? pUnit->dwUnitId : -1);
            D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
        }

        if (pStatList)
        {
            STATLIST_SetStatIfListIsValid(pStatList, STAT_ITEM_SINGLESKILL, nValue, nSkillId);
        }
    }
}
