#include "SKILLS/SkillItem.h"

#include <D2BitManip.h>

#include <algorithm>

#include <D2Items.h>
#include <D2Dungeon.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2StatList.h>
#include <D2ItemMods.h>
#include <D2States.h>
#include <D2Collision.h>
#include <D2Monsters.h>
#include <D2Skills.h>
#include <DataTbls/SkillsIds.h>
#include <DataTbls/ObjectsIds.h>
#include <UselessOrdinals.h>


#include "AI/AiGeneral.h"
#include "AI/AiThink.h"
#include "AI/AiUtil.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "GAME/Targets.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "OBJECTS/Objects.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlrTrade.h"
#include "QUESTS/ACT5/A5Q5.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillNec.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitEvent.h"
#include "UNIT/SUnitMsg.h"


//D2Game.0x6FD02BA0
int32_t __fastcall SKILLITEM_pSpell01_Initializer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
{
    ITEMS_SetItemFlag(pItem, IFLAG_TARGETING, 1);

    const int32_t nSpellIcon = ITEMS_GetSpellIcon(pItem);
    if (nSpellIcon >= 0)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(pClient, pItem, nSpellIcon, 0, 1, nSkillId);
    }

    return 1;
}

//D2Game.0x6FD02BF0
int32_t __fastcall SKILLITEM_pSpell01_IdentifyItem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
{
    if (!D2GAME_PLRTRADE_IsInteractingWithPlayer(pGame, pUnit) && !ITEMS_CHECK_FLAG(pTarget,  IFLAG_IDENTIFIED) && pUnit->pInventory)
    {
        for (D2UnitStrc* pInvItem = INVENTORY_GetFirstItem(pUnit->pInventory); pInvItem; pInvItem = INVENTORY_GetNextItem(pInvItem))
        {
            if (INVENTORY_UnitIsItem(pInvItem) == pTarget)
            {
                D2GAME_ITEMS_Identify_6FC49670(pGame, pUnit, pTarget);
                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FD02C80
int32_t __fastcall SKILLITEM_pSpell02_CastPortal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
{
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;

        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);

        const int32_t nTownLevelId = DUNGEON_GetTownLevelIdFromActNo(DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom)));
        if (!DUNGEON_IsRoomInTown(pRoom))
        {
            sub_6FC7C170(pGame, pUnit);
            
            D2UnitStrc* pPortal = nullptr;

            const int32_t nResult = D2GAME_CreatePortalObject_6FD13DF0(pGame, pUnit, pRoom, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), nTownLevelId, &pPortal, 0x3Bu, 0);
            SUNIT_AttachSound(pUnit, 7u, 0);

            if (nResult)
            {
                PLAYER_SetUniqueIdInPlayerData(pUnit, pPortal ? pPortal->dwUnitId : -1);

                if (pPortal)
                {
                    OBJECTS_SetUnitIdInTimerArg(pPortal, pUnit->dwUnitId);

                    D2UnitStrc* pOwner = SUNIT_GetPortalOwner(pGame, pPortal);
                    if (pOwner)
                    {
                        OBJECTS_SetUnitIdInTimerArg(pOwner, pUnit->dwUnitId);
                        ACT5Q5_OnPortalOpened(pGame, pOwner);
                    }

                    ACT5Q5_OnPortalOpened(pGame, pPortal);
                }
            }

            if (pItem)
            {
                D2GAME_PACKETS_SendPacketSize06_6FC3C850(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x7C, pItem->dwUnitType, pItem->dwUnitId);
            }

            return nResult;
        }

        SUNIT_AttachSound(pUnit, 0x18u, pUnit);
    }

    return 0;
}

//D2Game.0x6FD02E10
int32_t __fastcall SKILLITEM_pSpell03_Potion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
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

    const int32_t nLength = ITEMMODS_EvaluateItemFormula(pUnit, pItem, pItemsTxtRecord->dwLen);
    const int32_t nState = pItemsTxtRecord->wState;
    
    D2StatListStrc* pStatList = nullptr;
    int32_t nRemainingDuration = 0;
    if (nLength > 0)
    {
        if (nState <= 0 || nState >= sgptDataTables->nStatesTxtRecordCount)
        {
            return 0;
        }

        pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
        if (pStatList)
        {
            nRemainingDuration = D2COMMON_10473(pStatList) - pGame->dwGameFrame;
        }
    }

    int32_t nResult = 0;
    int32_t bAffectsHitpoints = 0;
    for (int32_t i = 0; i < 3; ++i)
    {
        const int32_t nStatId = pItemsTxtRecord->wStat[i];
        D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
        if (!pItemStatCostTxtRecord)
        {
            break;
        }

        int32_t nValue = ITEMMODS_EvaluateItemFormula(pUnit, pItem, pItemsTxtRecord->dwCalc[i]);
        switch (nStatId)
        {
        case STAT_HITPOINTS:
        case STAT_HPREGEN:
        {
            if (nStatId == STAT_HPREGEN)
            {
                nValue <<= 8;
            }

            nValue = ITEMS_GetBonusLifeBasedOnClass(pUnit, nValue);
            const int32_t nVitality = STATLIST_UnitGetStatValue(pUnit, STAT_VITALITY, 0);
            if (nVitality > 0 && (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nVitality) / 2)
            {
                nValue *= 2;
            }
            break;
        }
        case STAT_MANA:
        case STAT_MANARECOVERY:
        {
            if (nStatId == STAT_MANARECOVERY)
            {
                nValue <<= 8;
            }

            nValue = ITEMS_GetBonusManaBasedOnClass(pUnit, nValue);
            const int32_t nEnergy = STATLIST_UnitGetStatValue(pUnit, STAT_ENERGY, 0);
            if (nEnergy > 0 && (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nEnergy) / 2)
            {
                nValue *= 2;
            }
            break;
        }
        default:
            break;
        }

        int32_t nAddValue = nValue << pItemStatCostTxtRecord->nValShift;
        if (nAddValue > 0)
        {
            if (nStatId == STAT_HITPOINTS)
            {
                bAffectsHitpoints = 1;
            }

            if (nStatId == STAT_HITPOINTS || nStatId == STAT_HPREGEN)
            {
                if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
                {
                    nAddValue = sub_6FD0F8B0(pGame, pUnit, nAddValue);
                }
            }

            if (nLength <= 0)
            {
                const int32_t nOldValue = STATLIST_UnitGetStatValue(pUnit, nStatId, 0);
                if (pItemStatCostTxtRecord->wMaxStat < sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    const int32_t nMaxValue = STATLIST_UnitGetStatValue(pUnit, pItemStatCostTxtRecord->wMaxStat, 0);
                    nAddValue = std::min(nAddValue, nMaxValue - nOldValue);
                }

                STATLIST_AddUnitStat(pUnit, nStatId, nAddValue, 0);
            }
            else
            {
                const int32_t nExpireFrame = nLength + nRemainingDuration + pGame->dwGameFrame;
                if (!pStatList)
                {
                    int32_t nUnitGUID = -1;
                    int32_t nUnitType = 6;
                    if (pUnit)
                    {
                        nUnitGUID = pUnit->dwUnitId;
                        nUnitType = pUnit->dwUnitType;
                    }

                    pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nExpireFrame, nUnitType, nUnitGUID);
                    if (pStatList)
                    {
                        STATLIST_SetState(pStatList, nState);
                        STATES_ToggleState(pUnit, nState, 1);

                        if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
                        {
                            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_STATREGEN, 0);
                            EVENT_SetEvent(pGame, pUnit, EVENTTYPE_STATREGEN, pGame->dwGameFrame + 1, 0, 0);
                        }

                        STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
                        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
                    }
                }

                if (pStatList)
                {
                    D2COMMON_10476(pStatList, nExpireFrame);

                    const int32_t nTimeDependentValue = nAddValue + nRemainingDuration * STATLIST_GetStatValue(pStatList, nStatId, 0);
                    if (!i)
                    {
                        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_REMOVESTATE, nExpireFrame, 0, 0);
                    }

                    if (nRemainingDuration + nLength > 0)
                    {
                        STATLIST_SetStatIfListIsValid(pStatList, nStatId, nTimeDependentValue / (nRemainingDuration + nLength), 0);
                    }
                }
            }

            nResult = 1;
        }
    }

    if (bAffectsHitpoints && pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        const uint8_t nLastSentHpPct = STATLIST_UnitGetStatValue(pUnit, STAT_LAST_SENT_HP_PCT, 0);
        const uint8_t nNewSentHpPct = sub_6FC62F50(pUnit);
        const int32_t nHpPctDiff = std::abs(nLastSentHpPct - nNewSentHpPct);
        if (nHpPctDiff > 4)
        {
            sub_6FCC6270(pUnit, nNewSentHpPct);
            STATLIST_SetUnitStat(pUnit, STAT_LAST_SENT_HP_PCT, nNewSentHpPct, 0);
        }
    }

    return nResult;
}

//D2Game.0x6FD032D0
int32_t __fastcall SKILLITEM_pSpell04_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
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

    const int32_t nLength = ITEMMODS_EvaluateItemFormula(pUnit, pItem, pItemsTxtRecord->dwLen);
    const int32_t nState = pItemsTxtRecord->wState;

    D2StatListStrc* pStatList = nullptr;

    int32_t nRemainingDuration = 0;
    if (nLength > 0)
    {
        if (nState <= 0 || nState >= sgptDataTables->nStatesTxtRecordCount)
        {
            return 0;
        }

        pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
        if (pStatList)
        {
            nRemainingDuration = D2COMMON_10473(pStatList) - pGame->dwGameFrame;
        }
    }

    int32_t nResult = 0;
    int32_t bAffectsHitpoints = 0;

    for (int32_t i = 0; i < 3; ++i)
    {
        const int32_t nStatId = pItemsTxtRecord->wStat[i];
        D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
        if (!pItemStatCostTxtRecord)
        {
            break;
        }

        int32_t nValue = ITEMMODS_EvaluateItemFormula(pUnit, pItem, pItemsTxtRecord->dwCalc[i]);
        if (nStatId == STAT_MANARECOVERY || nStatId == STAT_HPREGEN)
        {
            nValue <<= 8;
        }

        int32_t nAddValue = nValue << pItemStatCostTxtRecord->nValShift;
        if (nAddValue > 0)
        {
            if (nStatId == STAT_HITPOINTS)
            {
                bAffectsHitpoints = 1;
            }

            if (nStatId == STAT_HITPOINTS || nStatId == STAT_HPREGEN)
            {
                if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
                {
                    nAddValue = sub_6FD0F8B0(pGame, pUnit, nAddValue);
                }
            }

            if (nLength <= 0)
            {
                const int32_t nOldValue = STATLIST_UnitGetStatValue(pUnit, nStatId, 0);
                if (pItemStatCostTxtRecord->wMaxStat < sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    const int32_t nMaxValue = STATLIST_UnitGetStatValue(pUnit, pItemStatCostTxtRecord->wMaxStat, 0);
                    nAddValue = std::min(nAddValue, nMaxValue - nOldValue);
                }

                STATLIST_AddUnitStat(pUnit, nStatId, nAddValue, 0);
            }
            else
            {
                const int32_t nExpireFrame = nLength + nRemainingDuration + pGame->dwGameFrame;
                if (!pStatList)
                {
                    int32_t nUnitGUID = -1;
                    int32_t nUnitType = 6;
                    if (pUnit)
                    {
                        nUnitGUID = pUnit->dwUnitId;
                        nUnitType = pUnit->dwUnitType;
                    }

                    pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nExpireFrame, nUnitType, nUnitGUID);
                    if (pStatList)
                    {
                        STATLIST_SetState(pStatList, nState);
                        STATES_ToggleState(pUnit, nState, 1);

                        if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
                        {
                            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_STATREGEN, 0);
                            EVENT_SetEvent(pGame, pUnit, EVENTTYPE_STATREGEN, pGame->dwGameFrame + 1, 0, 0);
                        }

                        STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
                        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
                    }
                }

                if (pStatList)
                {
                    D2COMMON_10476(pStatList, nExpireFrame);

                    const int32_t nTimeDependentValue = nAddValue + nRemainingDuration * STATLIST_GetStatValue(pStatList, nStatId, 0);
                    if (!i)
                    {
                        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_REMOVESTATE, nExpireFrame, 0, 0);
                    }

                    if (nRemainingDuration + nLength > 0)
                    {
                        STATLIST_SetStatIfListIsValid(pStatList, nStatId, nTimeDependentValue / (nRemainingDuration + nLength), 0);
                    }
                }
            }

            nResult = 1;
        }
    }

    if (bAffectsHitpoints && pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        const uint8_t nLastSentHpPct = STATLIST_UnitGetStatValue(pUnit, STAT_LAST_SENT_HP_PCT, 0);
        const uint8_t nNewSentHpPct = sub_6FC62F50(pUnit);
        const int32_t nHpPctDiff = std::abs(nLastSentHpPct - nNewSentHpPct);
        if (nHpPctDiff > 4)
        {
            sub_6FCC6270(pUnit, nNewSentHpPct);
            STATLIST_SetUnitStat(pUnit, STAT_LAST_SENT_HP_PCT, nNewSentHpPct, 0);
        }
    }

    return nResult;
}

//D2Game.0x6FD03610
int32_t __fastcall SKILLITEM_pSpell05_RejuvPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
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

    const int32_t nLength = ITEMMODS_EvaluateItemFormula(pUnit, pItem, pItemsTxtRecord->dwLen);
    const int32_t nState = pItemsTxtRecord->wState;

    D2StatListStrc* pStatList = nullptr;
    
    int32_t nRemainingDuration = 0;
    if (nLength > 0)
    {
        if (nState <= 0 || nState >= sgptDataTables->nStatesTxtRecordCount)
        {
            return 0;
        }

        pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
        if (pStatList)
        {
            nRemainingDuration = D2COMMON_10473(pStatList) - pGame->dwGameFrame;
        }
    }

    int32_t nResult = 0;
    int32_t bAffectsHitpoints = 0;

    for (int32_t i = 0; i < 3; ++i)
    {
        const int32_t nStatId = pItemsTxtRecord->wStat[i];
        D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
        if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->wMaxStat < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            const int32_t nPercentage = ITEMMODS_EvaluateItemFormula(pUnit, pItem, pItemsTxtRecord->dwCalc[i]);
            if (nPercentage > 0)
            {
                const int32_t nMaxValue = STATLIST_UnitGetStatValue(pUnit, pItemStatCostTxtRecord->wMaxStat, 0);
                int32_t nAddValue = MONSTERUNIQUE_CalculatePercentage(nMaxValue, nPercentage, 100);

                if (nStatId == STAT_HITPOINTS)
                {
                    bAffectsHitpoints = 1;
                }

                if (nStatId == STAT_HITPOINTS || nStatId == STAT_HPREGEN)
                {
                    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
                    {
                        nAddValue = sub_6FD0F8B0(pGame, pUnit, nAddValue);
                    }
                }

                if (nLength > 0)
                {
                    const int32_t nExpireFrame = nLength + nRemainingDuration + pGame->dwGameFrame;
                    if (!pStatList)
                    {
                        int32_t nUnitGUID = -1;
                        int32_t nUnitType = 6;
                        if (pUnit)
                        {
                            nUnitGUID = pUnit->dwUnitId;
                            nUnitType = pUnit->dwUnitType;
                        }

                        pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nExpireFrame, nUnitType, nUnitGUID);
                        if (pStatList)
                        {
                            STATLIST_SetState(pStatList, nState);
                            STATES_ToggleState(pUnit, nState, 1);

                            if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
                            {
                                D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_STATREGEN, 0);
                                EVENT_SetEvent(pGame, pUnit, EVENTTYPE_STATREGEN, pGame->dwGameFrame + 1, 0, 0);
                            }

                            STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
                            D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
                        }
                    }

                    if (pStatList)
                    {
                        D2COMMON_10476(pStatList, nExpireFrame);

                        const int32_t nTimeDependentValue = nAddValue + nRemainingDuration * STATLIST_GetStatValue(pStatList, nStatId, 0);
                        if (!i)
                        {
                            EVENT_SetEvent(pGame, pUnit, EVENTTYPE_REMOVESTATE, nExpireFrame, 0, 0);
                        }

                        if (nRemainingDuration + nLength > 0)
                        {
                            STATLIST_SetStatIfListIsValid(pStatList, nStatId, nTimeDependentValue / (nRemainingDuration + nLength), 0);
                        }
                    }
                }
                else
                {
                    const int32_t nOldValue = STATLIST_UnitGetStatValue(pUnit, nStatId, 0);
                    nAddValue = std::min(nAddValue, nMaxValue - nOldValue);

                    STATLIST_AddUnitStat(pUnit, nStatId, nAddValue, 0);
                }

                nResult = 1;
            }
        }
    }

    if (bAffectsHitpoints && pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        const uint8_t nNewSentHpPct = sub_6FC62F50(pUnit);
        sub_6FCC6270(pUnit, nNewSentHpPct);
        STATLIST_SetUnitStat(pUnit, STAT_LAST_SENT_HP_PCT, nNewSentHpPct, 0);
    }

    return nResult;
}

//D2Game.0x6FD039A0
int32_t __fastcall SKILLITEM_pSpell09_StaminaPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
{
    if (!pItem)
    {
        return 0;
    }

    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
    if (!pItemsTxtRecord || pItemsTxtRecord->wState <= 0 || pItemsTxtRecord->wState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }
    
    const int32_t nLength = ITEMMODS_EvaluateItemFormula(pUnit, pItem, pItemsTxtRecord->dwLen);
    if (nLength <= 0)
    {
        return 0;
    }

    sub_6FD11C90(pUnit, pItemsTxtRecord->wState, 0);
    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pItemsTxtRecord->wState);
    
    int32_t nEndFrame = nLength;
    if (pStatList)
    {
        nEndFrame += D2COMMON_10473(pStatList);
    }
    else
    {
        nEndFrame += pGame->dwGameFrame;
    }

    if (!pStatList)
    {
        int32_t nUnitGUID = -1;
        int32_t nUnitType = 6;
        if (pUnit)
        {
            nUnitGUID = pUnit->dwUnitId;
            nUnitType = pUnit->dwUnitType;
        }

        pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, 0, nUnitType, nUnitGUID);
        if (!pStatList)
        {
            return 0;
        }

        STATLIST_SetState(pStatList, pItemsTxtRecord->wState);
        STATES_ToggleState(pUnit, pItemsTxtRecord->wState, 1);
        STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
    }

    if (nEndFrame)
    {
        D2COMMON_10476(pStatList, nEndFrame);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_REMOVESTATE, nEndFrame, 0, 0);
    }

    for (int32_t i = 0; i < 3; ++i)
    {
        if (SKILLS_GetItemStatCostTxtRecord(pItemsTxtRecord->wStat[i]))
        {
            const int32_t nValue = ITEMMODS_EvaluateItemFormula(pUnit, pItem, pItemsTxtRecord->dwCalc[i]);
            if (nValue)
            {
                STATLIST_SetStatIfListIsValid(pStatList, pItemsTxtRecord->wStat[i], nValue, 0);
                if (pItemsTxtRecord->wStat[i] == STAT_STAMINARECOVERYBONUS)
                {
                    if (nValue > 0)
                    {
                        const int32_t nMaxStamina = STATLIST_UnitGetStatValue(pUnit, STAT_MAXSTAMINA, 0);
                        if (STATLIST_UnitGetStatValue(pUnit, STAT_STAMINA, 0) < nMaxStamina)
                        {
                            STATLIST_SetUnitStat(pUnit, STAT_STAMINA, nMaxStamina + 256, 0);
                        }
                    }
                }
            }
        }
    }

    return 1;
}

//D2Game.0x6FD03BB0
int32_t __fastcall SKILLITEM_pSpell09_AntidoteThawingPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
{
    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
    if (pItemsTxtRecord)
    {
        int32_t nResult = 0;
        for (int32_t i = 0; i < 2; ++i)
        {
            const int32_t nCurseState = pItemsTxtRecord->wCurseState[i];
            if (nCurseState > 0 && nCurseState < sgptDataTables->nStatesTxtRecordCount)
            {
                D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nCurseState);
                if (pStatList)
                {
                    D2Common_10474(pUnit, pStatList);
                    STATLIST_FreeStatList(pStatList);
                }

                if (STATES_CheckState(pUnit, nCurseState))
                {
                    STATES_ToggleState(pUnit, nCurseState, 0);
                }

                nResult = 1;
            }
        }

        if (SKILLITEM_pSpell09_StaminaPotion(pGame, pUnit, pItem, pTarget, nX, nY, nSkillId))
        {
            nResult = 1;
        }

        return nResult;
    }

    return 0;
}

//D2Game.0x6FD03C80
int32_t __fastcall SKILLITEM_pSpell08_ExperienceElixir(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
{
    const int32_t nStatId = ITEMS_GetFileIndex(pItem);
    if (nStatId >= 0 && nStatId < sgptDataTables->nItemStatCostTxtRecordCount)
    {
        const int32_t nValue = STATLIST_UnitGetStatValue(pItem, STAT_VALUE, 0);
        if (nValue)
        {
            STATLIST_AddUnitStat(pUnit, nStatId, nValue, 0);
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD03CD0
int32_t __fastcall SKILLITEM_pSpell07_OpenCube(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }

    if (PLAYER_IsBusy(pUnit))
    {
        D2UnitStrc* pInteractUnit = SUNIT_GetInteractUnit(pGame, pUnit);
        if (!pInteractUnit || pInteractUnit->dwUnitType != UNIT_OBJECT || pInteractUnit->dwClassId != OBJECT_STASH)
        {
            return 0;
        }

        SUNIT_ResetInteractInfo(pUnit);
        sub_6FC4B830(pGame, pUnit);
        D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 17u);
    }

    SUNIT_SetInteractInfo(pUnit, UNIT_ITEM, pItem ? pItem->dwUnitId : -1);
    D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), UPDATEUI_OPENCUBE);
    sub_6FC4B830(pGame, pUnit);
    return 0;
}

//D2Game.0x6FD03D80
int32_t __fastcall SKILLITEM_pSpell10_CastFireBallOnTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
{
    if (DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1))
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (pTarget)
        {
            return SKILLITEM_CastSkillOnTarget(pUnit, SKILL_FIREBALL, 1, pTarget, 0);
        }
        else
        {
            return SKILLITEM_CastSkillOnPosition(pUnit, SKILL_FIREBALL, 1, D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath), D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath), 0);
        }
    }

    return 0;
}

//D2Game.0x6FD03DF0
int32_t __fastcall SKILLITEM_pSpell11_CastFireBallToCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY, int32_t nSkillId)
{
    if (DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1))
    {
        return SKILLITEM_CastSkillOnPosition(pUnit, SKILL_FIREBALL, 1, nX, nY, 0);
    }

    return 0;
}

//D2Game.0x6FD03E40
int32_t __fastcall SKILLITEM_pSpell_Handler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t nX, int32_t nY)
{
    constexpr D2pSpellTblStrc gPSpellTable[31] = {
        { nullptr, nullptr },
        { SKILLITEM_pSpell01_Initializer, SKILLITEM_pSpell01_IdentifyItem },
        { nullptr, SKILLITEM_pSpell02_CastPortal },
        { nullptr, SKILLITEM_pSpell03_Potion },
        { nullptr, SKILLITEM_pSpell04_Unused },
        { nullptr, SKILLITEM_pSpell05_RejuvPotion },
        { nullptr, SKILLITEM_pSpell09_AntidoteThawingPotion },
        { nullptr, SKILLITEM_pSpell07_OpenCube },
        { nullptr, SKILLITEM_pSpell08_ExperienceElixir },
        { nullptr, SKILLITEM_pSpell09_StaminaPotion },
        { nullptr, SKILLITEM_pSpell10_CastFireBallOnTarget },
        { nullptr, SKILLITEM_pSpell11_CastFireBallToCoordinates },
    };

    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER || !pItem || pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    int32_t nPSpell = -1;
    int32_t nSkillId = -1;

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
    {
        D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
        if (pBooksTxtRecord)
        {
            nPSpell = pBooksTxtRecord->dwPSpell;
            nSkillId = pBooksTxtRecord->dwBookSkillId;
        }
    }

    if (nPSpell <= 0)
    {
        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
        if (pItemsTxtRecord)
        {
            nPSpell = pItemsTxtRecord->dwPspell;
        }

        if (nPSpell <= 0)
        {
            return 0;
        }
    }

    if (nPSpell >= std::size(gPSpellTable))
    {
        return 0;
    }

    if (gPSpellTable[nPSpell].pfSpellPrepare && !ITEMS_CHECK_FLAG(pItem,  IFLAG_TARGETING))
    {
        if (!gPSpellTable[nPSpell].pfSpellPrepare(pGame, pUnit, pItem, 0, nX, nY, nSkillId))
        {
            if (pUnit->pInventory)
            {
                for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
                {
                    if (INVENTORY_UnitIsItem(i) && ITEMS_CHECK_FLAG(i,  IFLAG_TARGETING))
                    {
                        ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                        D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                    }
                }
            }

            D2GAME_PACKETS_SendPacketSize06_6FC3C850(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x7C, pItem->dwUnitType, pItem->dwUnitId);
        }

        return 0;
    }

    SPELLDO pfSpellDo = gPSpellTable[nPSpell].pfSpellDo;
    if (!pfSpellDo)
    {
        return 0;
    }

    ITEMS_SetItemFlag(pItem, IFLAG_TARGETING, 1);

    const int32_t nResult = pfSpellDo(pGame, pUnit, pItem, pTarget, nX, nY, nSkillId);
    if (!nResult)
    {
        if (pUnit->pInventory)
        {
            for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
            {
                if (INVENTORY_UnitIsItem(i) && ITEMS_CHECK_FLAG(i,  IFLAG_TARGETING))
                {
                    ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }

        D2GAME_PACKETS_SendPacketSize06_6FC3C850(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x7C, pItem->dwUnitType, pItem->dwUnitId);
    }

    return nResult;
}

//D2Game.0x6FD040B0
int32_t __fastcall SKILLS_SrvDo113_Scroll_Book(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit->pInventory)
    {
        return 0;
    }

    int32_t bIsValidBookOrScroll = 0;
    
    for (D2UnitStrc* pInvItem = INVENTORY_GetFirstItem(pUnit->pInventory); pInvItem; pInvItem = INVENTORY_GetNextItem(pInvItem))
    {
        if (INVENTORY_UnitIsItem(pInvItem))
        {
            if (ITEMS_CheckItemTypeId(pInvItem, ITEMTYPE_BOOK))
            {
                const int32_t nSuffixId = ITEMS_GetSuffixId(pInvItem, 0);
                D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(nSuffixId);
                if (pBooksTxtRecord && pBooksTxtRecord->dwBookSkillId == nSkillId && (int32_t)STATLIST_UnitGetStatValue(pInvItem, STAT_QUANTITY, 0) > 0)
                {
                    bIsValidBookOrScroll = 1;
                }
            }
            else if (ITEMS_CheckItemTypeId(pInvItem, ITEMTYPE_SCROLL))
            {
                const int32_t nSuffixId = ITEMS_GetSuffixId(pInvItem, 0);
                D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(nSuffixId);
                if (pBooksTxtRecord && pBooksTxtRecord->dwScrollSkillId == nSkillId)
                {
                    bIsValidBookOrScroll = 1;
                }
            }

            if (bIsValidBookOrScroll)
            {
                switch (INVENTORY_GetItemNodePage(pInvItem))
                {
                case 1:
                {
                    int32_t nUnused = 0;
                    sub_6FC47D30(pGame, pUnit, pInvItem->dwUnitId, CLIENTS_GetUnitX(pInvItem), CLIENTS_GetUnitY(pInvItem), &nUnused);
                    return 1;
                }
                case 2:
                {
                    int32_t nUnused = 0;
                    sub_6FC49220(pGame, pUnit, pInvItem->dwUnitId, CLIENTS_GetUnitX(pInvItem), CLIENTS_GetUnitY(pInvItem), &nUnused, 0);
                    return 1;
                }
				default:
					break;
                }

                return 0;
            }
        }
    }

    return 0;
}

//D2Game.0x6FD04270
void __fastcall SKILLITEM_ActivateAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a5, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit)
    {
        return;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_AURA]) || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return;
    }

    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_PERIODICSTATS, a5);

    EVENT_SetEvent(pGame, pUnit, EVENTTYPE_PERIODICSTATS, SKILL_ComputePeriodicRate(pGame, pUnit, pSkillsTxtRecord, nSkillId, nSkillLevel), a5, nSkillId);

    if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_IMMEDIATE])
    {
        D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillId, nSkillLevel, 1, 1, 0);
    }
}

//D2Game.0x6FD04340
void __fastcall SKILLITEM_DeactivateAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a4, int32_t nSkillId)
{
    if (!pUnit)
    {
        return;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_AURA]) || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return;
    }

    STATES_ToggleState(pUnit, pSkillsTxtRecord->nAuraState, 0);

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
    if (pStatList)
    {
        D2Common_10474(pUnit, pStatList);
        STATLIST_FreeStatList(pStatList);
    }

    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_PERIODICSTATS, a4);
}

//D2Game.0x6FD043F0
int32_t __fastcall SKILLITEM_EventFunc06_AttackerTakesDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pAttacker && pUnit && pUnit->dwFlags & UNITFLAG_CANBEATTACKED)
    {
        const int32_t nDamage = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
        if (nDamage > 0)
        {
            D2DamageStrc damage = {};
            damage.wResultFlags |= 0x4021u;
            damage.dwPhysDamage = nDamage << 8;
            damage.dwHitClass = 0x8D;
            SUNITDMG_ExecuteEvents(pGame, pAttacker, pUnit, 1, &damage);
            SUNITDMG_ExecuteMissileDamage(pGame, pAttacker, pUnit, &damage);
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD044B0
int32_t __fastcall SKILLITEM_EventFunc10_AttackerTakesLightDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pAttacker && pUnit && pUnit->dwFlags & UNITFLAG_CANBEATTACKED)
    {
        const int32_t nDamage = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
        if (nDamage > 0)
        {
            D2DamageStrc damage = {};
            damage.wResultFlags |= 0x4021u;
            damage.dwLtngDamage = nDamage << 8;
            damage.dwHitClass = 0x4D;
            SUNITDMG_ExecuteEvents(pGame, pAttacker, pUnit, 1, &damage);
            SUNITDMG_ExecuteMissileDamage(pGame, pAttacker, pUnit, &damage);
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD04570
int32_t __fastcall SKILLITEM_EventFunc11_ApplyFireDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit || !(pUnit->dwFlags & UNITFLAG_CANBEATTACKED))
    {
        return 0;
    }

    const int32_t nDamage = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
    if (nDamage <= 0)
    {
        return 0;
    }

    D2DamageStrc damage = {};
    damage.wResultFlags |= 0x4021u;
    damage.dwFireDamage = nDamage << 8;
    damage.dwHitClass = 0x2D;
    SUNITDMG_ExecuteEvents(pGame, pAttacker, pUnit, 1, &damage);
    SUNITDMG_ExecuteMissileDamage(pGame, pAttacker, pUnit, &damage);
    return 1;
}

//D2Game.0x6FD04630
int32_t __fastcall SKILLITEM_EventFunc12_ApplyColdDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit || !(pUnit->dwFlags & UNITFLAG_CANBEATTACKED))
    {
        return 0;
    }

    const int32_t nDamage = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
    if (nDamage <= 0)
    {
        return 0;
    }

    const int32_t nDefenderLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
    const int32_t nAttackerLevel = STATLIST_UnitGetStatValue(pAttacker, STAT_LEVEL, 0);
    int32_t nLength = 25;
    if (nAttackerLevel > nDefenderLevel)
    {
        nLength += 10 * (nAttackerLevel - nDefenderLevel);
    }

    D2DamageStrc damage = {};
    damage.wResultFlags |= 0x4021u;
    damage.dwColdLen = nLength;
    damage.dwColdDamage = nDamage << 8;
    damage.dwHitClass = 0x3D;
    SUNITDMG_ExecuteEvents(pGame, pAttacker, pUnit, 1, &damage);
    SUNITDMG_ExecuteMissileDamage(pGame, pAttacker, pUnit, &damage);
    return 1;
}

//D2Game.0x6FD04720
int32_t __fastcall SKILLITEM_EventFunc07_Knockback(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit || !pDamage || STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId) <= 0)
    {
        return 0;
    }
    
    int32_t nChance = 64;
    if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
        if (pMonStats2TxtRecord)
        {
            if (pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_LARGE])
            {
                nChance = 32;
            }
            else if (pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_SMALL])
            {
                nChance = 128;
            }
        }
    }

    if ((ITEMS_RollRandomNumber(&pAttacker->pSeed) & 127) < nChance)
    {
        pDamage->wResultFlags |= 8u;
        return 1;
    }

    return 0;
}

//D2Game.0x6FD04820
int32_t __fastcall SKILLITEM_EventFunc08_Howl(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pAttacker && pUnit && pUnit->dwUnitType == UNIT_MONSTER && !MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_UNIQUE | MONTYPEFLAG_CHAMPION))
    {
        const int32_t nChance = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
        if (nChance > 0 && (ITEMS_RollRandomNumber(&pAttacker->pSeed) & 127) < nChance)
        {
            AIUTIL_ApplyTerrorCurseState(pGame, pAttacker, pUnit, SKILL_HOWL, 20, 20);
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD048B0
int32_t __fastcall SKILLITEM_EventFunc09_Stupidity(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }
    
    const int32_t nValue = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
    if (!nValue)
    {
        return 0;
    }
    
    int32_t nChance = 5 * (STATLIST_UnitGetStatValue(pAttacker, STAT_LEVEL, 0) + 4 * nValue - STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0) + 6);
    if (nEvent == 6 && nChance > 0)
    {
        nChance /= 3;
    }

    if (nChance < 1)
    {
        nChance = 1;
    }
    else if (nChance > 99)
    {
        nChance = 99;
    }

    const int32_t nBaseValue = nChance - (ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100);
    if (nBaseValue > 0)
    {
        int32_t nSkillLevel = nBaseValue / 5 + 1;
        if (nSkillLevel < 1)
        {
            nSkillLevel = 1;
        }
        else if (nSkillLevel >= 20)
        {
            nSkillLevel = 20;
        }

        sub_6FD0B9F0(pGame, pAttacker, pUnit, nSkillLevel);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD049D0
int32_t __fastcall SKILLITEM_EventFunc13_DamageToMana(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pAttacker && pAttacker->dwUnitType == UNIT_PLAYER && pDamage && pDamage->dwDmgTotal > 0)
    {
        const int32_t nPercentage = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
        if (nPercentage > 0)
        {
            const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pAttacker);
            const int32_t nMana = STATLIST_UnitGetStatValue(pAttacker, STAT_MANA, 0);
            if (nMana < nMaxMana)
            {
                int32_t nNewMana = nMana + MONSTERUNIQUE_CalculatePercentage(pDamage->dwDmgTotal, nPercentage, 100);
                if (nNewMana < 0)
                {
                    nNewMana = 0;
                }
                else if (nNewMana > nMaxMana)
                {
                    nNewMana = nMaxMana;
                }

                STATLIST_SetUnitStat(pAttacker, STAT_MANA, nNewMana, 0);
                UNITS_SetOverlay(pAttacker, 152, 0);
                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FD04B10
int32_t __fastcall SKILLITEM_EventFunc14_Freeze(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit)
    {
        return 0;
    }
    
    int32_t nValue = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
    if (nValue <= 0)
    {
        return 0;
    }
    
    int32_t nAttackerLevel = STATLIST_UnitGetStatValue(pAttacker, STAT_LEVEL, 0);
    const int32_t nDefenderLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);

    if (nEvent == 6)
    {
        nAttackerLevel -= 6;
    }

    int32_t nChance = 5 * (4 * (nValue - 1) - nDefenderLevel + nAttackerLevel + 10);
    if (nEvent == 6)
    {
        nChance /= 3;
    }

    if (nChance < 0)
    {
        nChance = 0;
    }
    else if (nChance > 100)
    {
        nChance = 100;
    }

    const int32_t nBaseLength = nChance - ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100;
    if (nBaseLength > 0)
    {
        int32_t nLength = 2 * nBaseLength + 25;
        if (nLength < 25)
        {
            nLength = 25;
        }
        else if (nLength > 250)
        {
            nLength = 250;
        }

        D2DamageStrc damage = {};
        damage.dwFrzLen = nLength;
        SUNITDMG_ExecuteEvents(pGame, pAttacker, pUnit, 1, &damage);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD04C40
int32_t __fastcall SKILLITEM_CalculateOpenWoundsHpRegen(int32_t nLevel, const int32_t* pValues)
{
    if (nLevel <= 1 || !pValues)
    {
        return 0;
    }

    if (nLevel > 60)
    {
        return (pValues[4] * (nLevel - 60)) + 15 * (pValues[1] + pValues[2] + pValues[3]) + 14 * pValues[0];
    }

    if (nLevel > 45)
    {
        return (pValues[3] * (nLevel - 45)) + 15 * (pValues[1] + pValues[2]) + 14 * pValues[0];
    }

    if (nLevel > 30)
    {
        return 15 * pValues[1] + pValues[2] * (nLevel - 30) + 14 * pValues[0];
    }

    if (nLevel > 15)
    {
        return pValues[1] * (nLevel - 15) + 14 * pValues[0];
    }

    return pValues[0] * (nLevel - 1);
}

//D2Game.0x6FD04CF0
int32_t __fastcall SKILLITEM_EventFunc15_OpenWounds(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit)
    {
        return 0;
    }

    const int32_t nChance = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
    if (nChance <= 0 || (ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100) >= nChance)
    {
        return 0;
    }

    const int32_t nLevel = std::max(STATLIST_UnitGetStatValue(pAttacker, STAT_LEVEL, 0), 1);
    const int32_t nValues[] = { 9, 18, 27, 36, 45 };

    int32_t nHpRegen = SKILLITEM_CalculateOpenWoundsHpRegen(nLevel, nValues) + 40;
    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        nHpRegen /= 4;

        if (nEvent == 6)
        {
            nHpRegen /= 2;
        }
    }
    else if (pUnit->dwUnitType == UNIT_MONSTER && MONSTERUNIQUE_CheckMonTypeFlag(pUnit, 0xCu))
    {
        nHpRegen /= 2;
    }

    D2CurseStrc pCurse = {};
    pCurse.pUnit = pAttacker;
    pCurse.pTarget = pUnit;
    pCurse.nSkill = 0;
    pCurse.nSkillLevel = 1;
    pCurse.nDuration = 200;
    pCurse.nStat = STAT_HPREGEN;
    pCurse.nStatValue = -nHpRegen;
    pCurse.nState = STATE_OPENWOUNDS;
    sub_6FD10EC0(&pCurse);
    return 1;
}

//D2Game.0x6FD04E50
int32_t __fastcall SKILLITEM_EventFunc16_CrushingBlow(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit)
    {
        return 0;
    }
    
    const int32_t nChance = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId); 
    if (nChance <= 0 || (ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100) >= nChance)
    {
        return 0;
    }

    int32_t nDivisor = 4;
    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        nDivisor = 10;
    }
    else if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        if (MONSTERS_GetHirelingTypeId(pUnit))
        {
            nDivisor = 10;
        }
        else
        {
            if (MONSTERS_IsBoss(0, pUnit) || MONSTERUNIQUE_CheckMonTypeFlag(pUnit, 2u))
            {
                nDivisor = 8;
            }

            int32_t nPlayerCount = STATLIST_UnitGetStatValue(pUnit, STAT_MONSTER_PLAYERCOUNT, 0);
            if (nPlayerCount < 1)
            {
                nPlayerCount = 1;
            }

            const int32_t nHpBonus = MONSTER_GetHpBonus(nPlayerCount);
            if (nHpBonus)
            {
                nDivisor += MONSTERUNIQUE_CalculatePercentage(nDivisor, nHpBonus, 100);
            }
        }
    }

    if (nEvent == 6)
    {
        nDivisor *= 2;
    }

    const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
    int32_t nCrushingBlowHp = nHitpoints / nDivisor;

    int32_t nDamageResist = STATLIST_UnitGetStatValue(pUnit, STAT_DAMAGERESIST, 0);
    if (nDamageResist >= 100)
    {
        nDamageResist = 100;
    }

    if (nDamageResist > 0)
    {
        nCrushingBlowHp -= MONSTERUNIQUE_CalculatePercentage(nCrushingBlowHp, nDamageResist, 100);
    }

    int32_t nNewHp = nHitpoints - nCrushingBlowHp;
    if (nNewHp <= 0)
    {
        nNewHp = 0;
    }

    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);

    if (nNewHp <= 0 && pDamage)
    {
        pDamage->wResultFlags |= 2u;
    }

    if (nCrushingBlowHp > 0)
    {
        UNITS_SetOverlay(pUnit, 147, 0);
    }

    return 1;
}

//D2Game.0x6FD050D0
int32_t __fastcall SKILLITEM_EventFunc17_ManaAfterKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pAttacker && pUnit && pAttacker->dwUnitType == UNIT_PLAYER)
    {
        const int32_t nValue = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
        if (nValue)
        {
            const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pAttacker);
            const int32_t nMana = STATLIST_UnitGetStatValue(pAttacker, STAT_MANA, 0);

            if (nMana < nMaxMana)
            {
                int32_t nNewMana = (nValue << 8) + nMana;
                if (nNewMana < 0)
                {
                    nNewMana = 0;
                }
                else if (nNewMana > nMaxMana)
                {
                    nNewMana = nMaxMana;
                }

                STATLIST_SetUnitStat(pAttacker, STAT_MANA, nNewMana, 0);
                UNITS_SetOverlay(pAttacker, 152, 0);
                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FD05160
int32_t __fastcall SKILLITEM_EventFunc28_HealAfterKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pAttacker && pUnit)
    {
        const int32_t nValue = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
        if (nValue)
        {
            const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pAttacker);
            const int32_t nHitpoints = STATLIST_UnitGetStatValue(pAttacker, STAT_HITPOINTS, 0);

            if (nHitpoints < nMaxHp)
            {
                int32_t nNewHp = (nValue << 8) + nHitpoints;
                if (nNewHp < 0)
                {
                    nNewHp = 0;
                }
                else if (nNewHp > nMaxHp)
                {
                    nNewHp = nMaxHp;
                }

                STATLIST_SetUnitStat(pAttacker, STAT_HITPOINTS, nNewHp, 0);
                UNITS_SetOverlay(pAttacker, 151, 0);
                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FD051E0
int32_t __fastcall SKILLITEM_EventFunc18_HealAfterDemonKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pAttacker && pUnit && MONSTERS_IsDemon(pUnit))
    {
        const int32_t nValue = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
        if (nValue)
        {
            const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pAttacker);
            const int32_t nHitpoints = STATLIST_UnitGetStatValue(pAttacker, STAT_HITPOINTS, 0);

            if (nHitpoints < nMaxHp)
            {
                int32_t nNewHp = (nValue << 8) + nHitpoints;
                if (nNewHp < 0)
                {
                    nNewHp = 0;
                }
                else if (nNewHp > nMaxHp)
                {
                    nNewHp = nMaxHp;
                }

                STATLIST_SetUnitStat(pAttacker, STAT_HITPOINTS, nNewHp, 0);
                UNITS_SetOverlay(pAttacker, 151, 0);
                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FD05270
int32_t __fastcall SKILLITEM_EventFunc19_Slow(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit)
    {
        return 0;
    }

    int32_t nSlowValue = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
    if (nSlowValue)
    {
        return 0;
    }

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    {
        if (nSlowValue >= 50)
        {
            nSlowValue = 50;
        }
    }
    case UNIT_MONSTER:
    {
        if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, 0xCu) || MONSTERS_IsBoss(0, pUnit) || MONSTERS_GetHirelingTypeId(pUnit))
        {
            if (nSlowValue >= 50)
            {
                nSlowValue = 50;
            }
        }
        else if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, 2u))
        {
            if (nSlowValue >= 75)
            {
                nSlowValue = 75;
            }
        }
        else
        {
            if (nSlowValue >= 90)
            {
                nSlowValue = 90;
            }
        }
    }
    default:
        return 0;
    }

    D2CurseStrc pCurse = {};
    pCurse.pUnit = pAttacker;
    pCurse.pTarget = pUnit;
    pCurse.nSkill = 0;
    pCurse.nSkillLevel = 1;
    pCurse.nDuration = 750;
    pCurse.nStat = STAT_VELOCITYPERCENT;
    pCurse.nStatValue = -nSlowValue;
    pCurse.nState = STATE_SLOWED;

    D2StatListStrc* pStatList = sub_6FD10EC0(&pCurse);
    if (pStatList)
    {
        STATLIST_SetStat(pStatList, STAT_ATTACKRATE, -nSlowValue, 0);
        STATLIST_SetStat(pStatList, STAT_OTHER_ANIMRATE, -nSlowValue, 0);
        UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD053D0
int32_t __fastcall SKILLITEM_EventFunc20_SkillOnAttackHitKill(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: v9, v10
    if (!pAttacker)
    {
        return 0;
    }

    const int32_t nChance = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
    if (nChance <= 0 || pDamage && !(pDamage->dwHitFlags & 0x20))
    {
        return 0;
    }

    if ((ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100) >= nChance)
    {
        return 0;
    }

    const int32_t v9 = (uint16_t)nSkillId & sgptDataTables->nShiftedStuff;
    const int32_t v10 = (uint16_t)nSkillId >> sgptDataTables->nStuff;

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(v10);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_ITEMTGTDO])
    {
        if (pUnit)
        {
            SKILLITEM_CastSkillOnTarget(pUnit, v10, v9, pUnit, 0);
            return 1;
        }
    }
    else
    {
        if (pUnit)
        {
            SKILLITEM_CastSkillOnTarget(pAttacker, v10, v9, pUnit, 1);
            return 1;
        }

        SKILLITEM_CastSkillOnPosition(pAttacker, v10, v9, D2COMMON_10175_PathGetFirstPointX(pAttacker->pDynamicPath), D2COMMON_10176_PathGetFirstPointY(pAttacker->pDynamicPath), 1);
    }

    return 1;
}

//D2Game.0x6FD05520
int32_t __fastcall SKILLITEM_EventFunc21_SkillOnGetHit(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: v9, v10
    if (pAttacker)
    {
        const int32_t nChance = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
        if (nChance > 0)
        {
            const int32_t v9 = (uint16_t)nSkillId & sgptDataTables->nShiftedStuff;
            const int32_t v10 = (uint16_t)nSkillId >> sgptDataTables->nStuff;
            if (pDamage && pDamage->wResultFlags & 4 && (ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100) < nChance)
            {
                if (SKILLS_GetSkillsTxtRecord(v10))
                {
                    if (pUnit)
                    {
                        SKILLITEM_CastSkillOnTarget(pAttacker, v10, v9, pUnit, 0);
                    }
                    else
                    {
                        SKILLITEM_CastSkillOnPosition(pAttacker, v10, v9, D2COMMON_10175_PathGetFirstPointX(pAttacker->pDynamicPath), D2COMMON_10176_PathGetFirstPointY(pAttacker->pDynamicPath), 0);
                    }

                    return 1;
                }
            }
        }
    }

    return 0;
}

//D2Game.0x6FD05640
int32_t __fastcall SKILLITEM_EventFunc30_SkillOnDeathLevelup(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: v9, v10
    if (pAttacker)
    {
        const int32_t nChance = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
        if (nChance > 0)
        {
            const int32_t v9 = (uint16_t)nSkillId & sgptDataTables->nShiftedStuff;
            const int32_t v10 = (uint16_t)nSkillId >> sgptDataTables->nStuff;

            if ((ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100) < nChance)
            {
                if (SKILLS_GetSkillsTxtRecord(v10))
                {
                    if (pUnit)
                    {
                        SKILLITEM_CastSkillOnTarget(pAttacker, v10, v9, pUnit, 0);
                    }
                    else
                    {
                        SKILLITEM_CastSkillOnPosition(pAttacker, v10, v9, D2COMMON_10175_PathGetFirstPointX(pAttacker->pDynamicPath), D2COMMON_10176_PathGetFirstPointY(pAttacker->pDynamicPath), 0);
                    }

                    return 1;
                }
            }
        }
    }

    return 0;
}

//D2Game.0x6FD05750
int32_t __fastcall SKILLITEM_EventFunc29_RestInPeace(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pAttacker && pUnit)
    {
        STATES_ToggleState(pUnit, STATE_RESTINPEACE, 1);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD05780
int32_t __fastcall SKILLITEM_TimerCallback_ReanimateMonster(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pSource, D2UnitStrc* pTarget, D2DamageStrc* pDamage, int32_t nMonId, int32_t nOwnerId)
{
    if (!pSource || pSource->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }
    
    D2UnitStrc* pOwner = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nOwnerId);
    if (!pOwner || !D2COMMON_11017_CheckUnitIfConsumeable(pSource, 0) || nMonId < 0 || nMonId >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        return 0;
    }

    const int32_t nX = CLIENTS_GetUnitX(pSource);
    const int32_t nY = CLIENTS_GetUnitY(pSource);

    D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomAtPosition(UNITS_GetRoom(pSource), nX, nY);
    if (!pRoom)
    {
        return 0;
    }
    
    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(nMonId);
    if (!pMonStats2TxtRecord)
    {
        return 0;
    }
     
    COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pSource), nX, nY, PATH_GetUnitCollisionPattern(pSource), 0x8000u);

    D2UnitStrc* pRevivedMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonId, MONMODE_NEUTRAL, -1, 74);
    if (!pRevivedMonster)
    {
        return 0;
    }

    pRevivedMonster->dwFlags |= UNITFLAG_NOXP | UNITFLAG_NOTC | UNITFLAG_ISVALIDTARGET | UNITFLAG_CANBEATTACKED | UNITFLAG_TARGETABLE;

    D2AiControlStrc* pAiControl = nullptr;
    if (pRevivedMonster->dwUnitType == UNIT_MONSTER && pRevivedMonster->pMonsterData)
    {
        pAiControl = pRevivedMonster->pMonsterData->pAiControl;
    }

    AITHINK_ExecuteAiFn(pGame, pRevivedMonster, pAiControl, AISPECIALSTATE_NONE);
    MONSTER_UpdateAiCallbackEvent(pGame, pRevivedMonster);
    AIGENERAL_SetOwnerData(pGame, pRevivedMonster, pOwner->dwUnitId, pOwner->dwUnitType, 0, 0);
    AIUTIL_SetOwnerGUIDAndType(pRevivedMonster, pOwner);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pRevivedMonster, EVENTTYPE_AITHINK, 0);
    EVENT_SetEvent(pGame, pRevivedMonster, EVENTTYPE_AITHINK, pGame->dwGameFrame + 25, 0, 0);
    sub_6FCBDD30(pRevivedMonster, 2u, 0);
    pRevivedMonster->dwFlags |= UNITFLAG_ISREVIVE;
    STATES_ToggleState(pRevivedMonster, STATE_REVIVE, 1);
    D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pRevivedMonster, 21, 0);
    EVENT_SetEvent(pGame, pRevivedMonster, EVENTTYPE_MONUMOD, pGame->dwGameFrame + 1500, 0, 0);
    D2GAME_UpdateSummonAI_6FC401F0(pGame, pRevivedMonster, 0, pOwner->dwNodeIndex);
    D2Common_10214(pRevivedMonster);

    if (pMonStats2TxtRecord->nResurrectMode == MONMODE_SEQUENCE)
    {
        D2ModeChangeStrc pModeChange = {};
        MONSTERMODE_GetModeChangeInfo(pRevivedMonster, MONMODE_SEQUENCE, &pModeChange);
        UNITS_SetUsedSkill(pRevivedMonster, SKILLS_GetSkillById(pRevivedMonster, SKILL_SKELETONRAISE, -1));
        PATH_SetStepNum(pRevivedMonster->pDynamicPath, 1u);
        pModeChange.pTargetUnit = 0;
        pModeChange.nX = nX;
        pModeChange.nY = nY;
        D2GAME_ModeChange_6FC65220(pGame, &pModeChange, 0);
    }

    STATES_ToggleState(pRevivedMonster, STATE_CORPSE_NOSELECT, 1);
    STATES_ToggleState(pSource, STATE_CORPSEEXP, 1);

    return 1;
}

//D2Game.0x6FD05B60
int32_t __fastcall SKILLITEM_EventFunc31_Reanimate(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit || pUnit->dwUnitType != UNIT_MONSTER || MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_UNIQUE | MONTYPEFLAG_CHAMPION))
    {
        return 0;
    }

    const int32_t nChance = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, (uint32_t)nSkillId >> 16, nSkillId);
    if (nChance <= 0 || (ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100) >= nChance)
    {
        return 0;
    }

    for (D2UnitStrc* i = pAttacker; i; i = AIGENERAL_GetMinionOwner(i))
    {
        if (i->dwUnitType == UNIT_PLAYER)
        {
			SUNITEVENT_Register(pGame, pUnit, UNITEVENT_DEATH, (uint16_t)nSkillId, i->dwUnitId, SKILLITEM_TimerCallback_ReanimateMonster, 0, i->dwUnitId);
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD05C20
int32_t __fastcall SKILLITEM_FindTargetPosition(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit)
    {
        return 0;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return 0;
    }

    const int32_t nUnitX = CLIENTS_GetUnitX(pUnit);
    const int32_t nUnitY = CLIENTS_GetUnitY(pUnit);
    const int32_t nCollisionPattern = PATH_GetUnitCollisionPattern(pUnit);
    const uint16_t nMask = PATH_GetFootprintCollisionMask(pUnit->pDynamicPath);

    for (int32_t i = 0; i < 10; ++i)
    {
        const int32_t nX = nUnitX + ITEMS_RollRandomNumber(&pUnit->pSeed) % 40 - 20;
        const int32_t nY = nUnitY + ITEMS_RollRandomNumber(&pUnit->pSeed) % 40 - 20;
        if (!COLLISION_CheckMaskWithPattern(pRoom, nX, nY, nCollisionPattern, nMask))
        {
            D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nX, nY);
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD05D70
int32_t __fastcall SKILLITEM_HandleItemEffectSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pTargetUnit, int32_t nX, int32_t nY, int32_t* pUnitType, int32_t* pUnitGUID, int32_t* pX, int32_t* pY, int32_t a7)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !pSkillsTxtRecord->wItemEffect)
    {
        return 0;
    }

    D2UnitStrc* pCurrentTarget = SUNIT_GetTargetUnit(pGame, pUnit);

    const int32_t nTargetX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
    const int32_t nTargetY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
    const int32_t bFlagSet = pUnit->dwFlags & UNITFLAG_SKSRVDOFUNC;

    if (pTargetUnit)
    {
        UNITS_SetTargetUnitForDynamicUnit(pUnit, pTargetUnit);
    }
    else
    {
        D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nX, nY);
    }

    if (pUnitType)
    {
        *pUnitType = 6;
        *pUnitGUID = -1;
    }

    if (pX)
    {
        *pX = 0;
        *pY = 0;
    }

    int32_t nResult = 0;

    switch (pSkillsTxtRecord->nItemTarget)
    {
    case SKILLSITEMTARGET_CASTER:
    {
        UNITS_SetTargetUnitForDynamicUnit(pUnit, pUnit);
        if (pUnitType)
        {
            *pUnitType = pUnit->dwUnitType;
            *pUnitGUID = pUnit->dwUnitId;
        }

        if (!(pSkillsTxtRecord->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_ITEMCHECKSTART]) || sub_6FD12950(pGame, pUnit, nSkillId, nSkillLevel, 1))
        {
            nResult = D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillId, nSkillLevel, 0, 1, a7);
        }
        break;
    }
    case SKILLSITEMTARGET_RANDOM:
    {
        if (SKILLITEM_FindTargetPosition(pGame, pUnit, nSkillId, nSkillLevel))
        {
            if (pX)
            {
                *pX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
            }

            if (pY)
            {
                *pY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
            }

            if (!(pSkillsTxtRecord->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_ITEMCHECKSTART]) || sub_6FD12950(pGame, pUnit, nSkillId, nSkillLevel, 1))
            {
                nResult = D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillId, nSkillLevel, 0, 1, a7);
            }
        }
        break;
    }
    case SKILLSITEMTARGET_RANDOM_CORPSE:
    {
        D2UnitStrc* pTarget = sub_6FD15210(pUnit, SUNIT_GetTargetUnit(pGame, pUnit), nSkillId, nSkillLevel);
        if (pTarget)
        {
            UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);
            if (pUnitType)
            {
                *pUnitType = pTarget->dwUnitType;
                *pUnitGUID = pTarget->dwUnitId;
            }

            if (!(pSkillsTxtRecord->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_ITEMCHECKSTART]) || sub_6FD12950(pGame, pUnit, nSkillId, nSkillLevel, 1))
            {
                nResult = D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillId, nSkillLevel, 0, 1, a7);
            }
        }
        break;
    }
    case SKILLSITEMTARGET_LAST_ATTACKER:
    {
        if (!SUNIT_GetTargetUnit(pGame, pUnit))
        {
            D2UnitStrc* pTarget = SUNIT_GetLastAttacker(pGame, pUnit);
            UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);

            if (pUnitType)
            {
                if (pTarget)
                {
                    *pUnitType = pTarget->dwUnitType;
                    *pUnitGUID = pTarget->dwUnitId;
                }
                else
                {
                    *pUnitType = 6;
                    *pUnitGUID = -1;
                }
            }
        }

        if (!(pSkillsTxtRecord->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_ITEMCHECKSTART]) || sub_6FD12950(pGame, pUnit, nSkillId, nSkillLevel, 1))
        {
            nResult = D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillId, nSkillLevel, 0, 1, a7);
        }
        break;
    }
    default:
        if (!(pSkillsTxtRecord->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_ITEMCHECKSTART]) || sub_6FD12950(pGame, pUnit, nSkillId, nSkillLevel, 1))
        {
            nResult = D2GAME_SKILLS_Handler_6FD12BA0(pGame, pUnit, nSkillId, nSkillLevel, 0, 1, a7);
        }
        break;
    }

    if (pCurrentTarget)
    {
        UNITS_SetTargetUnitForDynamicUnit(pUnit, pCurrentTarget);
    }
    else
    {
        D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nTargetX, nTargetY);
    }

    if (bFlagSet)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }
    else
    {
        pUnit->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;
    }

    return nResult;
}

//D2Game.0x6FD06020
int32_t __fastcall SKILLITEM_CastSkillOnTarget(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, D2UnitStrc* pTargetUnit, int32_t a5)
{
    if (!pUnit || !pTargetUnit || !pUnit->pGame || STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
    {
        return 0;
    }

    int32_t nTargetUnitType = 6;
    int32_t nTargetUnitGUID = -1;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;

    if (!SKILLITEM_HandleItemEffectSkill(pUnit->pGame, pUnit, nSkillId, nSkillLevel, pTargetUnit, 0, 0, &nTargetUnitType, &nTargetUnitGUID, &nTargetX, &nTargetY, a5))
    {
        return 0;
    }

    if (nTargetUnitType == 6)
    {
        sub_6FCC60D0(pUnit, nSkillId, nSkillLevel, pTargetUnit->dwUnitType, pTargetUnit->dwUnitId, a5);
    }
    else
    {
        sub_6FCC60D0(pUnit, nSkillId, nSkillLevel, nTargetUnitType, nTargetUnitGUID, a5);
    }

    return 1;
}

//D2Game.0x6FD060F0
int32_t __fastcall SKILLITEM_CastSkillOnPosition(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t a0)
{
    if (!pUnit || !pUnit->pGame || STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
    {
        return 0;
    }

    int32_t nTargetUnitType = 0;
    int32_t nTargetUnitGUID = 0;
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;

    if (!SKILLITEM_HandleItemEffectSkill(pUnit->pGame, pUnit, nSkillId, nSkillLevel, 0, nX, nY, &nTargetUnitType, &nTargetUnitGUID, &nTargetX, &nTargetY, a0))
    {
        return 0;
    }

    if (nTargetX)
    {
        sub_6FCC6150(pUnit, nSkillId, nSkillLevel, nTargetX, nTargetY, a0);
    }
    else
    {
        sub_6FCC6150(pUnit, nSkillId, nSkillLevel, nX, nY, a0);
    }

    return 1;
}
