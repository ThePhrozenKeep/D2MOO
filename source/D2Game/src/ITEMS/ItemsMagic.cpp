#include "ITEMS/ItemsMagic.h"

#include <algorithm>

#include <D2ItemMods.h>
#include <D2Items.h>
#include <D2Skills.h>
#include <D2StatList.h>
#include <D2DataTbls.h>
#include <D2Math.h>
#include <DataTbls/SkillsIds.h>

#include "ITEMS/Items.h"
#include "UNIT/SUnit.h"

#pragma pack(push, 1)
struct D2MagicAffixIndexAndPtrStrc
{
    int16_t nAffixId;
    int16_t pad0x02;
    const D2MagicAffixTxt* pMagicAffixRecord;
};

struct D2RareAffixIndexAndPtrStrc
{
    int16_t nIndex;
    int16_t pad0x02;
    D2RareAffixTxt* pRareAffixRecord;
};
#pragma pack(pop)


//D2Game.0x6FC52920
int16_t __fastcall ITEMS_RollMagicAffixes(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefix, int32_t nPreferredAffixIndex)
{
    if (ITEMS_GetItemFormat(pItem) < 1u)
    {
        return ITEMS_RollMagicAffixesOld(pItem, bRequireSpawnableAffix, bForceAffixRoll, bAssignProperties, bPrefix, nPreferredAffixIndex);
    }
    
    return ITEMS_RollMagicAffixesNew(pItem, bRequireSpawnableAffix, bForceAffixRoll, bAssignProperties, bPrefix, nPreferredAffixIndex, 0);
}

//D2Game.0x6FC52980
int16_t __fastcall ITEMS_RollMagicAffixesOld(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefix, int32_t nPreferredAffixIndex)
{
    D2MagicAffixDataTbl* pMagicAffixDataTbl = DATATBLS_GetMagicAffixDataTables();
    if (!pMagicAffixDataTbl)
    {
        return 0;
    }

    D2MagicAffixTxt* pMagicAffixTxtRecord = nullptr;
    int32_t nMagicAffixesCount = 0;
    if (bPrefix)
    {
        pMagicAffixTxtRecord = pMagicAffixDataTbl->pMagicPrefix;
        nMagicAffixesCount = pMagicAffixDataTbl->pAutoMagic - pMagicAffixTxtRecord;
    }
    else
    {
        pMagicAffixTxtRecord = pMagicAffixDataTbl->pMagicSuffix;
        nMagicAffixesCount = pMagicAffixDataTbl->pMagicPrefix - pMagicAffixTxtRecord;
    }

    if (!pMagicAffixTxtRecord)
    {
        return 0;
    }

    const int32_t nAffixTableOffset = pMagicAffixTxtRecord - pMagicAffixDataTbl->pMagicAffixTxt;
    const uint16_t nItemFormat = ITEMS_GetItemFormat(pItem);
    if (SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem),2) == 0 && !bForceAffixRoll)
    {
        return 0;
    }

    const int32_t nItemLevel = D2Clamp<int32_t>(ITEMS_GetItemLevel(pItem) + 2, 1, HARDCODEDCST_MAX_LEVEL);

    const int32_t nPotentialMagicAffixesSize = 512;
    D2MagicAffixIndexAndPtrStrc tPotentialMagicAffixes[nPotentialMagicAffixesSize];

    int32_t nPotentialMagicAffixesCount = 0;
    for (int32_t nMagicAffixIndex = 0; nMagicAffixIndex < nMagicAffixesCount; ++nMagicAffixIndex)
    {
        const D2MagicAffixTxt& rAffix = pMagicAffixTxtRecord[nMagicAffixIndex];
        if (!(bRequireSpawnableAffix && !rAffix.wSpawnable)
            && (rAffix.wVersion < 100u || nItemFormat >= 100u)
            && rAffix.dwLevel <= nItemLevel
            && ITEMMODS_CanItemHaveMagicAffix(pItem, &rAffix)
            && nPotentialMagicAffixesCount < (nPotentialMagicAffixesSize-1))
        {
            tPotentialMagicAffixes[nPotentialMagicAffixesCount].nAffixId = nAffixTableOffset + nMagicAffixIndex;
            tPotentialMagicAffixes[nPotentialMagicAffixesCount].pMagicAffixRecord = &rAffix;
            ++nPotentialMagicAffixesCount;
        }
    }

    if (nPotentialMagicAffixesCount <= 0)
    {
        return 0;
    }

    const int32_t nRolledAffixIndex = ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPotentialMagicAffixesCount);
    if (nRolledAffixIndex >= nPotentialMagicAffixesCount)
    {
        return 0;
    }

    D2MagicAffixIndexAndPtrStrc* pMagicItemAffixData = &tPotentialMagicAffixes[nRolledAffixIndex];

    if (nPreferredAffixIndex > 0 && nAffixTableOffset + nPreferredAffixIndex > 0)
    {
        for (int32_t i = 0; i < nPotentialMagicAffixesCount; ++i)
        {
            if (tPotentialMagicAffixes[i].nAffixId == nAffixTableOffset + nPreferredAffixIndex - 1)
            {
                pMagicItemAffixData = &tPotentialMagicAffixes[i];
                break;
            }
        }
    }

    if (bAssignProperties)
    {
        ITEMMODS_AssignProperty(0, 0, pItem, pMagicItemAffixData->pMagicAffixRecord, 0, 0);
    }
    return pMagicItemAffixData->nAffixId + 1;
}

// Helper functions
static int32_t ITEMS_ComputeCraftedMagicAffixLevel(D2UnitStrc* pItem, const D2ItemsTxt* pItemTxtRecord)
{
    const int32_t nItemQlvl = ITEMS_GetItemQlvl(pItem);
    const int32_t nItemLevel = std::max(ITEMS_GetItemLevel(pItem), nItemQlvl);

    int32_t nRequiredLevel;
    if (int32_t nItemMagicLevel = pItemTxtRecord->nMagicLevel)
    {
        nRequiredLevel = nItemMagicLevel + nItemLevel;
    }
    else
    {
        // Should be using DATATBLS_GetMaxLevel instead of HARDCODEDCST_MAX_LEVEL
        const int32_t nHalfItemQlvl = nItemQlvl / 2;
        const int32_t nHalfQLevelDistanceToLevel99 = HARDCODEDCST_MAX_LEVEL - nHalfItemQlvl;
        if (nItemLevel >= nHalfQLevelDistanceToLevel99)
        {
            nRequiredLevel = 2 * nItemLevel - nHalfItemQlvl - nHalfQLevelDistanceToLevel99;
        }
        else
        {
            nRequiredLevel = nItemLevel - nHalfItemQlvl;
        }
    }

    return D2Clamp<int32_t>(nRequiredLevel, 1, HARDCODEDCST_MAX_LEVEL);
}

static bool ITEMMODS_AffixPlayerClassisCompatible(D2UnitStrc* pItem, const D2MagicAffixTxt& rAffix)
{
    if (rAffix.nClassSpecific != uint8_t(-1))
    {
        const int32_t nClassOfClassSpecificItem = ITEMS_GetClassOfClassSpecificItem(pItem);
        return nClassOfClassSpecificItem == PCLASS_INVALID || nClassOfClassSpecificItem == rAffix.nClassSpecific;
    }

    return true;
}

static bool ITEMMODS_HasAffixWithGroup(D2UnitStrc* pItem, int32_t nGroup, bool bPrefixOrSuffix)
{
    if (!pItem || pItem->dwUnitType != UNIT_ITEM || !pItem->pItemData)
    {
        return false;
    }

    const uint16_t* pAffixes = (bPrefixOrSuffix ? pItem->pItemData->wMagicPrefix : pItem->pItemData->wMagicSuffix);
    for (int32_t nAffixIndex = 0; nAffixIndex < ITEMS_MAX_MODS; ++nAffixIndex)
    {
        D2MagicAffixTxt* pAffix = DATATBLS_GetMagicAffixTxtRecord(pAffixes[nAffixIndex]);
        if (!pAffix)
        {
            return false;
        }

        if (pAffix->dwGroup == nGroup)
        {
            return true;
        }
    }
    return false;
}

static int32_t ComputeAffixFrequency(int32_t nItemMagicLevel, const D2MagicAffixTxt* pMagicAffix)
{
    if (nItemMagicLevel)
    {
        return pMagicAffix->dwLevel * pMagicAffix->wFrequency;
    }

    return pMagicAffix->wFrequency;
}

static D2MagicAffixIndexAndPtrStrc* MagicItemRollAffix(
    D2UnitStrc* pItem, int32_t nItemMagicLevel,
    D2MagicAffixIndexAndPtrStrc* pPotentialMagicAffixes, int32_t nPotentialMagicAffixesCount, 
    int32_t nTotalMagicAffixesFrequency
)
{
    if (pPotentialMagicAffixes && nPotentialMagicAffixesCount > 0)
    {
        int32_t nRolledAffixValue = SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nTotalMagicAffixesFrequency);
        
        D2MagicAffixIndexAndPtrStrc* pRolledMagicAffixIdAndPtr = nullptr;
        for (int32_t nCurrentPotentialAffix = 0; nCurrentPotentialAffix < nPotentialMagicAffixesCount; ++nCurrentPotentialAffix)
        {
            pRolledMagicAffixIdAndPtr = &pPotentialMagicAffixes[nCurrentPotentialAffix];
            const int32_t nAffixFrequency = ComputeAffixFrequency(nItemMagicLevel, pRolledMagicAffixIdAndPtr->pMagicAffixRecord);
            nRolledAffixValue -= nAffixFrequency;
            if (nRolledAffixValue < 0)
            {
                break;
            }
        }
        return pRolledMagicAffixIdAndPtr;
    }
    return nullptr;
}

//D2Game.0x6FC52C00
int16_t __fastcall ITEMS_RollMagicAffixesNew(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefixes, int32_t nPreferredAffixIndex, int32_t nAutoMagicGroup)
{

    const D2MagicAffixDataTbl* pMagicAffixDataTables = DATATBLS_GetMagicAffixDataTables();
    if (!pMagicAffixDataTables)
    {
        return 0;
    }
    // Tables or ordered the following way:
    // - suffixes
    // - prefixes
    // - automagic

    // By default, use prefixes only
    const D2MagicAffixTxt* pMagicAffixTableBegin = pMagicAffixDataTables->pMagicPrefix;
    const D2MagicAffixTxt* pMagicAffixTableEnd = pMagicAffixDataTables->pAutoMagic;

    if (nAutoMagicGroup)
    {
        pMagicAffixTableBegin = pMagicAffixDataTables->pAutoMagic;
        pMagicAffixTableEnd   = &pMagicAffixDataTables->pMagicAffixTxt[pMagicAffixDataTables->nMagicAffixTxtRecordCount];
    }
    if (!bPrefixes)
    {
        pMagicAffixTableBegin = pMagicAffixDataTables->pMagicSuffix;
        pMagicAffixTableEnd = pMagicAffixDataTables->pMagicPrefix;
    }

    const int32_t nMagicAffixesCount = pMagicAffixTableEnd - pMagicAffixTableBegin;
    if (!pMagicAffixTableBegin)
    {
        return 0;
    }
    
    int16_t nRolledAffixId = -1; // We return nRolledAffixId + 1
    const int32_t nAffixTableOffset = pMagicAffixTableBegin - pMagicAffixDataTables->pMagicAffixTxt;

    const BOOL bItemRandomValueIsOdd = SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), 2);

    if (!bItemRandomValueIsOdd && !bForceAffixRoll)
    {
        return nRolledAffixId + 1;
    }

    const D2ItemsTxt* pItemTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
    if (!pItemTxtRecord)
    {
        return 0;
    }

    const int32_t nItemMagicLevel = pItemTxtRecord->nMagicLevel;
    const int32_t nItemAffixLevel = ITEMS_ComputeCraftedMagicAffixLevel(pItem, pItemTxtRecord);
    const uint16_t nItemFormat = ITEMS_GetItemFormat(pItem);

    int32_t nTotalMagicAffixesFrequency = 0;
    int32_t nPotentialMagicAffixesCount = 0;
    
    const int32_t nPotentialMagicAffixesSize = 512;
    D2MagicAffixIndexAndPtrStrc tPotentialMagicAffixes[nPotentialMagicAffixesSize];

    int32_t nMagicAffixIndex;
    for (nMagicAffixIndex = 0; nMagicAffixIndex < nMagicAffixesCount; nMagicAffixIndex++)
    {
        const D2MagicAffixTxt& rAffix = pMagicAffixTableBegin[nMagicAffixIndex];
        if ((bRequireSpawnableAffix && !rAffix.wSpawnable) || !(rAffix.wVersion < 100u || nItemFormat >= 100u))
        {
            continue;
        }

        bool bFoundPreferredAffix = false;
        if (nPreferredAffixIndex > 0 && nMagicAffixIndex == (nPreferredAffixIndex - 1))
        {
            bFoundPreferredAffix = true;
        }

        if (rAffix.dwLevel > nItemAffixLevel || (rAffix.dwMaxLevel != 0 && nItemAffixLevel > rAffix.dwMaxLevel))
        {
            continue;
        }

        if (!rAffix.nRare)
        {
            switch (ITEMS_GetItemQuality(pItem))
            {
            case ITEMQUAL_RARE:
            case ITEMQUAL_CRAFT:
            case ITEMQUAL_TEMPERED:
                // Ignore magic-only affixes for rare/crafted objects.
                continue;
            }
        }

        if (!ITEMMODS_CanItemHaveMagicAffix(pItem, &rAffix)
            || (nAutoMagicGroup && nAutoMagicGroup != rAffix.dwGroup)   // If automagic group provided, do not allow other groups
            || rAffix.wFrequency == 0                                   // No chance to drop this affix
            || !ITEMMODS_AffixPlayerClassisCompatible(pItem, rAffix)    // Affix needs to be compatible with weapon class
            || ITEMMODS_HasAffixWithGroup(pItem, rAffix.dwGroup, false) // May not spawn two suffixes of same group
            || ITEMMODS_HasAffixWithGroup(pItem, rAffix.dwGroup, true)  // May not spawn two prefixes of same group
            )
        {
            continue;
        }

        if (bFoundPreferredAffix)
        {
            if (nMagicAffixIndex + nAffixTableOffset >= 0)
            {
                if (bAssignProperties)
                {
                    ITEMMODS_AssignProperty(PROPMODE_AFFIX, 0, pItem, &rAffix, 0, 0);
                }
                return nMagicAffixIndex + nAffixTableOffset + 1;
            }
            break;
        }

        if (nPotentialMagicAffixesCount < (nPotentialMagicAffixesSize - 1))
        {
            tPotentialMagicAffixes[nPotentialMagicAffixesCount].nAffixId = nAffixTableOffset + nMagicAffixIndex;
            tPotentialMagicAffixes[nPotentialMagicAffixesCount].pMagicAffixRecord = &rAffix;
            ++nPotentialMagicAffixesCount;
        }
        nTotalMagicAffixesFrequency += ComputeAffixFrequency(nItemMagicLevel, &rAffix);
    }

    if (D2MagicAffixIndexAndPtrStrc* pRolledAffix = MagicItemRollAffix(pItem, nItemMagicLevel, tPotentialMagicAffixes, nPotentialMagicAffixesCount, nTotalMagicAffixesFrequency))
    {
        if (bAssignProperties)
        {
            ITEMMODS_AssignProperty(PROPMODE_AFFIX, 0, pItem, pRolledAffix->pMagicAffixRecord, 0, 0);
        }
        return pRolledAffix->nAffixId + 1;
    }

    return 0;
}

//D2Game.0x6FC53080
int16_t __fastcall ITEMS_RollMagicAffixes(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefixes, int32_t nPreferredAffixIndex, int32_t nAutoMagicGroup)
{
    if (ITEMS_GetItemFormat(pItem) < 1u)
    {
        return ITEMS_RollMagicAffixesOld(pItem, bRequireSpawnableAffix, bForceAffixRoll, bAssignProperties, bPrefixes, nPreferredAffixIndex);
    }
    
    return ITEMS_RollMagicAffixesNew(pItem, bRequireSpawnableAffix, bForceAffixRoll, bAssignProperties, bPrefixes, nPreferredAffixIndex, nAutoMagicGroup);
}

//D2Game.0x6FC530E0
int16_t __fastcall ITEMS_RollTemperedItemAffix(D2UnitStrc* pItem, int32_t bPrefix)
{
    // Note: Original game checks D2COMMON_10875_GetItemFormat(pItem) != 0 but does the exact same things
    //       This is because tempered items were disabled in the final 1.10 version.
    D2RareAffixDataTbl* pRareAffixDataTbl = DATATBLS_GetRareAffixDataTables();
    D2RareAffixTxt* pRareAffixTxtRecordBegin = nullptr;
    D2RareAffixTxt* pRareAffixTxtRecordEnd = nullptr;
    if (bPrefix)
    {
        pRareAffixTxtRecordBegin = pRareAffixDataTbl->pRarePrefix;
        pRareAffixTxtRecordEnd = &pRareAffixDataTbl->pRareAffixTxt[pRareAffixDataTbl->nRareAffixTxtRecordCount];
    }
    else
    {
        pRareAffixTxtRecordBegin = pRareAffixDataTbl->pRareSuffix;
        pRareAffixTxtRecordEnd = pRareAffixDataTbl->pRarePrefix;
    }

    if (!pRareAffixTxtRecordBegin)
    {
        return 0;
    }

    const int32_t nAffixCount = pRareAffixTxtRecordEnd - pRareAffixTxtRecordBegin;
    const int32_t nAffixTableOffset = pRareAffixTxtRecordBegin - pRareAffixDataTbl->pRareAffixTxt;

    D2RareAffixIndexAndPtrStrc tPotentialRareAffixesPtrAndId[512] = {};
    int32_t nPotentialRareAffixes = 0;
    for (int32_t i = 0; i < nAffixCount; ++i)
    {
        if (ITEMMODS_CanItemHaveRareAffix(pItem, &pRareAffixTxtRecordBegin[i]) && nPotentialRareAffixes < 511)
        {
            tPotentialRareAffixesPtrAndId[nPotentialRareAffixes].nIndex = nAffixTableOffset + i;
            tPotentialRareAffixesPtrAndId[nPotentialRareAffixes].pRareAffixRecord = &pRareAffixTxtRecordBegin[i];
            ++nPotentialRareAffixes;
        }
    }

    if (!nPotentialRareAffixes)
    {
        return 0;
    }

    const int32_t nRolledAffixIndex = ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPotentialRareAffixes);
    if (nRolledAffixIndex >= nPotentialRareAffixes)
    {
        return 0;
    }

    return tPotentialRareAffixesPtrAndId[nRolledAffixIndex].nIndex + 1;
}

//D2Game.0x6FC53360
int32_t __fastcall D2GAME_RollRareItem_6FC53360(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    if (!ITEMS_CanBeRare(pItem))
    {
        return 0;
    }

    if (ITEMS_GetItemFormat(pItem) < 1u)
    {
        return sub_6FC53760(pItem, pItemDrop);
    }

    int16_t nPrefix = D2GAME_RollRareAffix_6FC53610(pItem, 1);
    int16_t nSuffix = D2GAME_RollRareAffix_6FC53610(pItem, 0);

    if (!nPrefix || !nSuffix)
    {
        return 0;
    }

    ITEMS_AssignRarePrefix(pItem, nPrefix);
    ITEMS_AssignRareSuffix(pItem, nSuffix);

    D2SeedStrc* pSeed = ITEMS_GetItemSeed(pItem);

    constexpr int32_t dword_6FD27E70[] =
    {
        3, 4, 4, 5, 5, 5, 6, 6
    };

    int32_t nAffixCount = 0;
    if (ITEMS_GetItemType(pItem) == ITEMTYPE_JEWEL)
    {
        nAffixCount = ITEMS_RollLimitedRandomNumber(pSeed, 2) + 3;
    }
    else
    {
        nAffixCount = dword_6FD27E70[ITEMS_RollRandomNumber(pSeed) & 7];
    }
    
    int32_t bSuffixAssigned = 0;
    int32_t nPrefixAssigned = 0;
    int32_t nPrefixIndex = 0;
    int32_t nSuffixIndex = 0;
    int32_t nCounter = 0;
    while (nCounter < nAffixCount)
    {
        if (nPrefixAssigned && bSuffixAssigned)
        {
            break;
        }

        if (!bSuffixAssigned && (nPrefixAssigned || ITEMS_RollRandomNumber(pSeed) & 1))
        {
            D2_ASSERT(nSuffixIndex < ITEMS_MAX_MODS);

            nSuffix = ITEMS_RollMagicAffixesNew(pItem, 1, 1, 0, 0, pItemDrop->nSuffix[nSuffixIndex], 0);
            if (nSuffix)
            {
                ITEMS_AssignSuffix(pItem, nSuffix, nSuffixIndex);
                ++nSuffixIndex;
                if (nSuffixIndex >= 3)
                {
                    bSuffixAssigned = 1;
                }
            }
            else
            {
                bSuffixAssigned = 1;
                --nCounter;
            }
        }
        else
        {
            D2_ASSERT(nPrefixIndex < ITEMS_MAX_MODS);

            nPrefix = ITEMS_RollMagicAffixesNew(pItem, 1, 1, 0, 1, pItemDrop->nPrefix[nPrefixIndex], 0);
            if (nPrefix)
            {
                ITEMS_AssignPrefix(pItem, nPrefix, nPrefixIndex);
                ++nPrefixIndex;
                if (nPrefixIndex >= 3)
                {
                    nPrefixAssigned = 1;
                }
            }
            else
            {
                nPrefixAssigned = 1;
                --nCounter;
            }
        }

        ++nCounter;
    }

    if (!nPrefixIndex && !nSuffixIndex)
    {
        return 0;
    }

    ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 0);

    for (int32_t i = 0; i < 3; ++i)
    {
        D2MagicAffixTxt* pMagicAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(ITEMS_GetPrefixId(pItem, i));
        if (pMagicAffixTxtRecord)
        {
            ITEMMODS_AssignProperty(0, nullptr, pItem, pMagicAffixTxtRecord, 0, 0);
        }

        pMagicAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(ITEMS_GetSuffixId(pItem, i));
        if (pMagicAffixTxtRecord)
        {
            ITEMMODS_AssignProperty(0, nullptr, pItem, pMagicAffixTxtRecord, 0, 0);
        }
    }

    sub_6FC52410(pItem, pItemDrop);
    return 1;
}

//D2Game.0x6FC53610
int16_t __fastcall D2GAME_RollRareAffix_6FC53610(D2UnitStrc* pItem, int32_t bPrefix)
{
    D2RareAffixDataTbl* pRareAffixDataTbl = DATATBLS_GetRareAffixDataTables();
    D2RareAffixTxt* pRareAffixTxtRecord = nullptr;
    if (bPrefix)
    {
        pRareAffixTxtRecord = pRareAffixDataTbl->pRarePrefix;
    }
    else
    {
        pRareAffixTxtRecord = pRareAffixDataTbl->pRareSuffix;
    }

    if (!pRareAffixTxtRecord)
    {
        return 0;
    }

    const int32_t nAffixIndex = pRareAffixTxtRecord - pRareAffixDataTbl->pRareAffixTxt;
    int32_t nCount = 0;
    if (bPrefix)
    {
        nCount = &pRareAffixDataTbl->pRareAffixTxt[pRareAffixDataTbl->nRareAffixTxtRecordCount] - pRareAffixDataTbl->pRarePrefix;
    }
    else
    {
        nCount = pRareAffixDataTbl->pRarePrefix - pRareAffixDataTbl->pRareSuffix;
    }

    D2RareAffixIndexAndPtrStrc data[512] = {};
    int32_t nMax = 0;
    for (int32_t i = 0; i < nCount; ++i)
    {
        if (ITEMMODS_CanItemHaveRareAffix(pItem, &pRareAffixTxtRecord[i]) && nMax < 511)
        {
            data[nMax].nIndex = nAffixIndex + i;
            data[nMax].pRareAffixRecord = &pRareAffixTxtRecord[i];
            ++nMax;
        }
    }

    if (!nMax)
    {
        return 0;
    }

    const uint32_t nIndex = ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nMax);
    if (nIndex >= nMax)
    {
        return 0;
    }

    return data[nIndex].nIndex + 1;
}

// Helper functions
static bool D2GAME_DoesItemHaveAffixOrAffixGroup(D2UnitStrc* pItem, uint16_t nAffixId, uint32_t nAffixGroup, bool bPrefix)
{
    for (int32_t nAffixSlot = 0; nAffixSlot < ITEMS_MAX_MODS; ++nAffixSlot)
    {
        const uint16_t nSlotAffixId = bPrefix ? ITEMS_GetPrefixId(pItem, nAffixSlot) : ITEMS_GetSuffixId(pItem, nAffixSlot);
        if (const D2MagicAffixTxt* pSlotAffixRecord = DATATBLS_GetMagicAffixTxtRecord(nSlotAffixId))
        {
            if (nSlotAffixId == nAffixId || pSlotAffixRecord->dwGroup == nAffixGroup)
            {
                return true;
            }
        }
    }
    return false;
}

static void D2GAME_RollMagicAffixForRareItem(D2UnitStrc* pItem, bool bPrefix, int* pAffixSlot)
{
    const int32_t nMaxAttemptsForNewAffix = 250;
    uint16_t nAffixIdToAssign = 0;
    for (int32_t nAttempt = 0; nAttempt <= nMaxAttemptsForNewAffix && nAffixIdToAssign == 0; ++nAttempt)
    {

        uint16_t nRolledAffixId = ITEMS_RollMagicAffixes(pItem, TRUE, TRUE, FALSE, bPrefix, 0);
        if (!nRolledAffixId)
        {
            break;
        }
        const uint32_t nRolledAffixGroup = DATATBLS_GetMagicAffixTxtRecord(nRolledAffixId)->dwGroup;

        if (!D2GAME_DoesItemHaveAffixOrAffixGroup(pItem, nRolledAffixId, nRolledAffixGroup, bPrefix))
        {
            nAffixIdToAssign = nRolledAffixId;
        }
    }

    if (bPrefix)
    {
        ITEMS_AssignPrefix(pItem, nAffixIdToAssign, *pAffixSlot);
    }
    else
    {
        ITEMS_AssignSuffix(pItem, nAffixIdToAssign, *pAffixSlot);
    }

    if (nAffixIdToAssign != 0)
    {
        (*pAffixSlot)++;
    }
}

static BOOL __fastcall D2GAME_AssignMagicAffixesForRareItem(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop, int32_t nMaxAffixesToSpawn)
{
    D2SeedStrc* pItemSeed = ITEMS_GetItemSeed(pItem);

    int32_t nSuffixSlotToAssign = 0;
    int32_t nPrefixSlotToAssign = 0;

    for (int32_t nSpawnedAffixes = 0; nSpawnedAffixes < nMaxAffixesToSpawn; ++nSpawnedAffixes)
    {
        const bool bPreferSuffixRoll = SEED_RollLimitedRandomNumber(pItemSeed, 2) != 0;
        if ((nSuffixSlotToAssign != ITEMS_MAX_MODS && bPreferSuffixRoll) || nPrefixSlotToAssign == ITEMS_MAX_MODS)
        {
            D2GAME_RollMagicAffixForRareItem(pItem, false, &nSuffixSlotToAssign);
        }
        else
        {
            D2GAME_RollMagicAffixForRareItem(pItem, true, &nPrefixSlotToAssign);
        }
    }

    // Early out if we didn't assign any affix
    if (nPrefixSlotToAssign == 0 && nSuffixSlotToAssign == 0)
    {
        return FALSE;
    }

    ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 0);

    for (int32_t nAffixSlot = 0; nAffixSlot < ITEMS_MAX_MODS; ++nAffixSlot)
    {
        if (D2MagicAffixTxt* pSlotPrefixRecord = DATATBLS_GetMagicAffixTxtRecord(ITEMS_GetPrefixId(pItem, nAffixSlot)))
        {
            ITEMMODS_AssignProperty(PROPMODE_AFFIX, nullptr, pItem, pSlotPrefixRecord, 0, 0);
        }

        if (D2MagicAffixTxt* pSlotSuffixRecord = DATATBLS_GetMagicAffixTxtRecord(ITEMS_GetSuffixId(pItem, nAffixSlot)))
        {
            ITEMMODS_AssignProperty(PROPMODE_AFFIX, nullptr, pItem, pSlotSuffixRecord, 0, 0);
        }
    }
    
    sub_6FC52410(pItem, pItemDrop);
    return TRUE;
}

//D2Game.0x6FC53760
BOOL __fastcall sub_6FC53760(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    // Note: original game calls ITEMS_GetItemFormat twice, it was probably trying to handle different paths dependending on the format but ended up calling the same functions

    const uint16_t nItemFormat = ITEMS_GetItemFormat(pItem);
    const int16_t nRolledPrefixId = D2GAME_RollRareAffix_6FC53610(pItem, TRUE);
    const int16_t nRolledSuffixId = D2GAME_RollRareAffix_6FC53610(pItem, FALSE);

    if (!nRolledPrefixId && !nRolledSuffixId)
    {
        return FALSE;
    }

    ITEMS_AssignRarePrefix(pItem, nRolledPrefixId);
    ITEMS_AssignRareSuffix(pItem, nRolledSuffixId);

    D2SeedStrc* pItemSeed = ITEMS_GetItemSeed(pItem);

    const bool bIsJewel = ITEMS_GetItemType(pItem) == ITEMTYPE_JEWEL;

    int32_t nMaxAffixesToSpawn;
    if (bIsJewel)
    {
        nMaxAffixesToSpawn = SEED_RollLimitedRandomNumber(pItemSeed, ITEMS_MAX_AFFIXES_RARE_JEWEL - ITEMS_MIN_AFFIXES_RARE_JEWEL + 1) + ITEMS_MIN_AFFIXES_RARE_JEWEL;
    }
    else
    {
        nMaxAffixesToSpawn = SEED_RollLimitedRandomNumber(pItemSeed, ITEMS_MAX_AFFIXES_RARE_ITEM - ITEMS_MIN_AFFIXES_RARE_ITEM + 1) + ITEMS_MIN_AFFIXES_RARE_ITEM;
    }
    return D2GAME_AssignMagicAffixesForRareItem(pItem, pItemDrop, nMaxAffixesToSpawn);
}


//D2Game.0x6FC53CD0) --------------------------------------------------------
int32_t __fastcall sub_6FC53CD0(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    // Note: original game calls ITEMS_GetItemFormat twice, it was probably trying to handle different paths dependending on the format but ended up calling the same functions

    const uint16_t nItemFormat = ITEMS_GetItemFormat(pItem);
    const int16_t nRolledPrefixId = D2GAME_RollRareAffix_6FC53610(pItem, TRUE);
    const int16_t nRolledSuffixId = D2GAME_RollRareAffix_6FC53610(pItem, FALSE);

    if (!nRolledPrefixId && !nRolledSuffixId)
    {
        return FALSE;
    }

    ITEMS_AssignRarePrefix(pItem, nRolledPrefixId);
    ITEMS_AssignRareSuffix(pItem, nRolledSuffixId);

    const int32_t nItemLvl = pItemDrop->nItemLvl;
    int32_t nMinAffixesForItemLvl = 1;
    if (nItemLvl > 30)
    {
        nMinAffixesForItemLvl = 2;
    }
    if (nItemLvl > 50)
    {
        nMinAffixesForItemLvl = 3;
    }
    if (nItemLvl > 70)
    {
        nMinAffixesForItemLvl = 4;
    }
    
    D2SeedStrc* pItemSeed = ITEMS_GetItemSeed(pItem);
    int32_t nMaxAffixesToSpawn = SEED_RollLimitedRandomNumber(pItemSeed, 5);
    if (nMaxAffixesToSpawn < nMinAffixesForItemLvl)
    {
        nMaxAffixesToSpawn = nMinAffixesForItemLvl;
    }

    return D2GAME_AssignMagicAffixesForRareItem(pItem, pItemDrop, nMaxAffixesToSpawn);
}

//D2Game.0x6FC54240
int32_t __fastcall sub_6FC54240(D2UnitStrc* pItem, int32_t bScroll)
{
    D2BookDataTbl* pBookDataTbl = DATATBLS_GetBookDataTables();
    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
    if (!pBookDataTbl)
    {
        return -1;
    }

    if (!pItemsTxtRecord || !pBookDataTbl->pBooksTxt)
    {
        return pBookDataTbl->nBooksTxtRecordCount;
    }

    for (int32_t i = 0; i < pBookDataTbl->nBooksTxtRecordCount; ++i)
    {
        const int32_t nCode = bScroll ? pBookDataTbl->pBooksTxt[i].dwScrollSpellCode : pBookDataTbl->pBooksTxt[i].dwBookSpellCode;
        if (pItemsTxtRecord->dwCode == nCode)
        {
            return i;
        }
    }

    return pBookDataTbl->nBooksTxtRecordCount;
}

//D2Game.0x6FC542C0
int32_t __fastcall sub_6FC542C0(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    if (ITEMS_GetItemFormat(pItem) < 1u)
    {
        return sub_6FC544A0(pItem);
    }

    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
    if (!pItemsTxtRecord)
    {
        return 0;
    }

    int32_t nLegibleSets = 0;
    int32_t nAccumulatedRarity = 0;
    int32_t nIndex = -1;
    const int32_t nItemFormat = ITEMS_GetItemFormat(pItem);
    const int32_t nItemLevel = ITEMS_GetItemLevel(pItem);
    int32_t rarityData[512] = {};

    for (int32_t i = 0; i < sgptDataTables->nSetItemsTxtRecordCount; ++i)
    {
        D2SetItemsTxt* pSetItemsTxtRecord = &sgptDataTables->pSetItemsTxt[i];
        if ((pSetItemsTxtRecord->wVersion < 100 || nItemFormat >= 100) && pSetItemsTxtRecord->wLvl <= nItemLevel && pItemsTxtRecord->dwCode == pSetItemsTxtRecord->szItemCode && (pSetItemsTxtRecord->nSetId != 29 || pItemDrop->dwFlags2 & 1))
        {
            if (pItemDrop->nItemIndex && pItemDrop->nItemIndex - 1 == i)
            {
                nIndex = i;
            }

            rarityData[2 * nLegibleSets] = i;
            rarityData[2 * nLegibleSets + 1] = pSetItemsTxtRecord->dwRarity;
            if (!pSetItemsTxtRecord->dwRarity)
            {
                rarityData[2 * nLegibleSets + 1] = 1;
            }

            nAccumulatedRarity += rarityData[2 * nLegibleSets + 1];
            ++nLegibleSets;
        }
    }

    int32_t nFileIndex = nIndex;
    if (nIndex < 0)
    {
        if (!nAccumulatedRarity)
        {
            return 0;
        }

        int32_t nChance = ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nAccumulatedRarity);
        int32_t nCounter = 0;
        while (nCounter < nLegibleSets)
        {
            if (nChance < rarityData[2 * nCounter + 1])
            {
                break;
            }

            nChance -= rarityData[2 * nCounter + 1];
            ++nCounter;
        }

        if (nCounter < 0 || nCounter >= nLegibleSets)
        {
            return 0;
        }

        nFileIndex = rarityData[2 * nCounter];
    }

    ITEMS_SetFileIndex(pItem, nFileIndex);
    ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 0);
    ITEMMODS_AssignProperty(4, nullptr, pItem, nullptr, 0, 0);

    return 1;
}

//D2Game.0x6FC544A0
int32_t __fastcall sub_6FC544A0(D2UnitStrc* pItem)
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

    const int32_t nLowSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
    const int32_t nItemFormat = ITEMS_GetItemFormat(pItem);
    const int32_t nDurability = std::min(2 * STATLIST_GetUnitStatUnsigned(pItem, STAT_DURABILITY, 0), 255ui32);
    STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nDurability, 0);

    const int32_t nMaxDurability = std::min(2 * STATLIST_GetUnitBaseStat(pItem, STAT_MAXDURABILITY, 0), 255);
    STATLIST_SetUnitStat(pItem, STAT_MAXDURABILITY, nMaxDurability, 0);

    int32_t nCounter = 0;
    while (nCounter < sgptDataTables->nSetsTxtRecordCount)
    {
        if (sgptDataTables->pSetsTxt[nCounter].wVersion >= 100)
        {
            break;
        }
        ++nCounter;
    }

    const uint32_t nRandom = ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nCounter);
    int32_t nSetItemId = -1;
    for (int32_t j = 0; j < nCounter; ++j)
    {
        const int32_t nIndex = (nRandom + j) % nCounter;
        D2SetsTxt* pSetsTxtRecord = &sgptDataTables->pSetsTxt[nIndex];
        if (pSetsTxtRecord->wVersion < 100u || nItemFormat >= 100u)
        {
            for (int32_t i = 0; i < pSetsTxtRecord->nSetItems; ++i)
            {
                if (pItemsTxtRecord->dwCode == pSetsTxtRecord->pSetItem[i]->szItemCode)
                {
                    nSetItemId = pSetsTxtRecord->pSetItem[i]->wSetItemId;
                    break;
                }
            }
        }
    }

    if (nSetItemId == -1)
    {
        SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nLowSeed);
        return 0;
    }

    ITEMS_SetFileIndex(pItem, nSetItemId);
    ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 0);
    ITEMMODS_AssignProperty(4, nullptr, pItem, nullptr, 0, 0);
    return 1;
}

//D2Game.0x6FC54690
int32_t __fastcall sub_6FC54690(D2UnitStrc* pUnit, D2ItemDropStrc* pItemDrop)
{
    D2QualityItemDataTbl* pLowQualityItemDataTbl = DATATBLS_GetQualityItemDataTables();
    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pUnit ? pUnit->dwClassId : -1);
    if (!pLowQualityItemDataTbl || !pItemsTxtRecord)
    {
        return 0;
    }

    int32_t nCount = pLowQualityItemDataTbl->nQualityItemsTxtRecordCount; 
    if (ITEMS_CheckIfThrowable(pUnit) || pItemsTxtRecord->nNoDurability)
    {
        nCount = 4;
    }

    uint8_t bRecordChecked[12] = {}; // TODO: There's no overflow check here, if the record count of QualityItems.txt is > 12
    int32_t nFileIndex = 0;

    while (1)
    {
        do
        {
            nFileIndex = ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pUnit), nCount);
        }
        while (bRecordChecked[nFileIndex]);

        D2QualityItemsTxt* pQualityItemsTxtRecord = DATATBLS_GetQualityItemsTxtRecord(nFileIndex);
        if (!pQualityItemsTxtRecord)
        {
            return 0;
        }

        if (ITEMMODS_CanItemBeHighQuality(pUnit, pQualityItemsTxtRecord))
        {
            ITEMS_SetFileIndex(pUnit, nFileIndex);

            ITEMMODS_AssignProperty(1, 0, pUnit, pQualityItemsTxtRecord, 0, 0);

            sub_6FC52410(pUnit, pItemDrop);
            return 1;
        }

        bRecordChecked[nFileIndex] = 1;

        int32_t i = 0;
        if (nCount <= 0)
        {
            return 0;
        }

        while (bRecordChecked[i])
        {
            ++i;

            if (i >= nCount)
            {
                return 0;
            }
        }
    }
}

//D2Game.0x6FC549F0
int32_t __fastcall sub_6FC549F0(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    D2_MAYBE_UNUSED(pItemDrop);

    D2LowQualityItemDataTbl* pLowQualityItemDataTbl = DATATBLS_GetLowQualityItemDataTables();
    if (!pLowQualityItemDataTbl)
    {
        return 0;
    }

    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
    if (!pItemsTxtRecord)
    {
        return 0;
    }

    ITEMS_SetFileIndex(pItem, ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), pLowQualityItemDataTbl->nLowQualityItemsTxtRecordCount));

    if (ITEMS_HasDurability(pItem))
    {
        const int32_t nMaxDurability = std::max(pItemsTxtRecord->nDurability / 3, 1);
        const int32_t nDurability = std::max(ITEMS_RollLimitedRandomNumber(&pItem->pSeed, nMaxDurability / 2) + nMaxDurability / 2, 1u);
        STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nDurability, 0);
        STATLIST_SetUnitStat(pItem, STAT_MAXDURABILITY, nMaxDurability, 0);
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
    {
        const int32_t nMaxDamage = std::max(75 * STATLIST_GetUnitBaseStat(pItem, STAT_MAXDAMAGE, 0) / 100, 2);
        STATLIST_SetUnitStat(pItem, STAT_MAXDAMAGE, nMaxDamage, 0);

        const int32_t nMinDamage = std::max(75 * STATLIST_GetUnitBaseStat(pItem, STAT_MINDAMAGE, 0) / 100, 1);
        STATLIST_SetUnitStat(pItem, STAT_MINDAMAGE, nMinDamage, 0);

        const int32_t nMaxSecondaryDamage = std::max(75 * STATLIST_GetUnitBaseStat(pItem, STAT_SECONDARY_MAXDAMAGE, 0) / 100, 2);
        STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MAXDAMAGE, nMaxSecondaryDamage, 0);

        const int32_t nMinSecondaryDamage = std::max(75 * STATLIST_GetUnitBaseStat(pItem, STAT_SECONDARY_MINDAMAGE, 0) / 100, 1);
        STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MINDAMAGE, nMinSecondaryDamage, 0);

        if (!ITEMS_CheckIfThrowable(pItem))
        {
            return 1;
        }

        const int32_t nMinThrowDamage = std::max(75 * STATLIST_GetUnitStatSigned(pItem, STAT_ITEM_THROW_MINDAMAGE, 0) / 100, 2);
        STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MINDAMAGE, nMinThrowDamage, 0);

        const int32_t nMaxThrowDamage = std::max(75 * STATLIST_GetUnitStatSigned(pItem, STAT_ITEM_THROW_MAXDAMAGE, 0) / 100, 1);
        STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MAXDAMAGE, nMaxThrowDamage, 0);
        return 1;
    }
    else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_ARMOR))
    {
        const int32_t nArmorClass = std::max(75 * STATLIST_GetUnitBaseStat(pItem, STAT_ARMORCLASS, 0) / 100, 1);
        STATLIST_SetUnitStat(pItem, STAT_ARMORCLASS, nArmorClass, 0);
        return 1;
    }

    return 0;
}

//D2Game.0x6FC54D00
int32_t __fastcall sub_6FC54D00(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    if (!pItem)
    {
        return 0;
    }

    const int32_t nResult = sub_6FC549F0(pItem, pItemDrop);
    sub_6FC52410(pItem, pItemDrop);

    return nResult;
}

//D2Game.0x6FC55060
void __fastcall sub_6FC55060(D2UnitStrc* pUnit, int32_t nItemLevel, int32_t nClassFirstSkillId, int32_t a4 /*unused?*/)
{
    const int32_t nCountRoll = ITEMS_RollRandomNumber(ITEMS_GetItemSeed(pUnit)) % 100;
    int32_t nCount = 0;
    if (nCountRoll > 90)
    {
        nCount = 3;
    }
    else if (nCountRoll > 70)
    {
        nCount = 2;
    }
    else if (nCountRoll > 30)
    {
        nCount = 1;
    }
    else
    {
        return;
    }

    int32_t nBaseOffset = 1;
    if (nItemLevel > 24)
    {
        nBaseOffset = 4;
    }
    else if (nItemLevel > 18)
    {
        nBaseOffset = 3;
    }
    else if (nItemLevel > 11)
    {
        nBaseOffset = 2;
    }

    for (int32_t i = 0; i < nCount; ++i)
    {
        const int32_t nOffsetRoll = ITEMS_RollRandomNumber(ITEMS_GetItemSeed(pUnit)) % 100;
        int32_t nOffset = 0;
        if (nOffsetRoll > 80)
        {
            nOffset = nBaseOffset + 1;
        }
        else if (nOffsetRoll > 30)
        {
            nOffset = nBaseOffset;
        }
        else if (nOffsetRoll > 10)
        {
            nOffset = nBaseOffset - 1;
        }
        else
        {
            nOffset = nBaseOffset - 2;
        }

        nOffset = std::max(nOffset, 1);

        int32_t layers[3] = { -1, -1, -1 };
        int32_t nLayer = 0;

        const int32_t nBaseLayer = nClassFirstSkillId + 5 * (nOffset - 1);
        do
        {
            do
            {
                nLayer = nBaseLayer + ITEMS_RollRandomNumber(ITEMS_GetItemSeed(pUnit)) % 5;
            }
            while (nLayer == SKILL_POISONDAGGER); // Why poison dagger ? Is it banned from being a character skill ?
        }
        while (nLayer == layers[0] || nLayer == layers[1] || nLayer == layers[2]);

        layers[i] = nLayer;

        const int32_t nValueRoll = ITEMS_RollRandomNumber(ITEMS_GetItemSeed(pUnit)) % 100;
        int32_t nValue = 1;
        if (nValueRoll >= 90)
        {
            nValue = 3;
        }
        else if (nValueRoll >= 60)
        {
            nValue = 2;
        }

        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitStateAndFlag(pUnit, 0, 0x40);
        if (!pStatList)
        {
            pStatList = STATLIST_AllocStatList(pUnit ? pUnit->pMemoryPool : nullptr, 0x40u, 0, 4, pUnit ? pUnit->dwUnitId : -1);
            D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
        }

        if (pStatList)
        {
            STATLIST_SetStatIfListIsValid(pStatList, STAT_ITEM_SINGLESKILL, nValue, nLayer);
        }
    }
}

//D2Game.0x6FC55270
int32_t __fastcall sub_6FC55270(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop)
{
    if (ITEMS_GetItemFormat(pItem) < 1u)
    {
        return sub_6FC549F0(pItem, pItemDrop);
    }

    return sub_6FC54D00(pItem, pItemDrop);
}
