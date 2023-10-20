#include "SKILLS/SkillPal.h"

#include <algorithm>

#include <D2BitManip.h>

#include <DataTbls/SkillsTbls.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Skills.h>
#include <D2StatList.h>
#include <D2States.h>
#include <D2Monsters.h>
#include <DataTbls/MonsterIds.h>

#include <UselessOrdinals.h>


#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Targets.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterUnique.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlrSave2.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillNec.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitMsg.h"


#pragma pack(push, 1)
struct D2AuraBaseStrc
{
    int32_t nAuraState;
    int32_t nSkillId;
    int32_t nSkillLevel;
    int32_t nDuration;
    int32_t nAuraStat[6];
    int32_t nAuraStatCalcValue[6];
    int32_t field_40;
    D2StatListStrc* pStatList;
    int32_t nPassiveState;

};

struct D2BasicAuraParamStrc : D2AuraBaseStrc
{
    StatListRemoveCallback pStateRemoveCallback;
};

struct D2DamageAuraParamStrc : D2AuraBaseStrc
{
    D2DamageStrc* pDamage;
};

struct D2UnkPalSkillStrc2
{
    int32_t nSkillId;
    int32_t nSkillLevel;
    int32_t nCounter;
};
#pragma pack(pop)


//D2Game.0x6FD18330
int32_t __fastcall SKILLS_SrvSt29_Sacrifice(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord && UNITS_GetUsedSkill(pUnit))
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (pTarget && UNITS_IsInMeleeRange(pUnit, pTarget, 0))
        {
            D2DamageStrc damage = {};

            damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0);
            if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
            {
                damage.dwPhysDamage = SUNITDMG_ApplyDamageBonuses(pUnit, 1, nullptr, 0, 0, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel), 0, pSkillsTxtRecord->nSrcDam);
                if (pSkillsTxtRecord->nEType)
                {
                    damage.dwConvPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
                    if (damage.dwConvPct > 0)
                    {
                        damage.nConvType = pSkillsTxtRecord->nEType;
                    }
                }

                D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
                damage.dwHitFlags = 1;
                damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
                damage.dwHitFlags |= 1;
            }

            SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD184B0
int32_t __fastcall SKILLS_SrvDo064_Sacrifice(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord)
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (pTarget)
        {
            D2DamageStrc* pDamage = SUNITDMG_GetDamageFromUnits(pUnit, pTarget);
            if (pDamage)
            {
                D2DamageStrc damage = {};
                damage.dwHitFlags |= 0x1000u;

                int32_t nBaseDamage = pDamage->dwPhysDamage;
                const int32_t nHitpoints = STATLIST_UnitGetStatValue(pTarget, STAT_HITPOINTS, 0);
                if (nBaseDamage > nHitpoints)
                {
                    nBaseDamage = nHitpoints;
                }

                const int32_t nDamage = MONSTERUNIQUE_CalculatePercentage(nBaseDamage, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel), 100);
                damage.dwPhysDamage = nDamage;
                damage.dwDmgTotal = nDamage;
                SUNITDMG_ExecuteEvents(pGame, pUnit, pUnit, 0, &damage);
                SUNITDMG_ExecuteMissileDamage(pGame, pUnit, pUnit, &damage);
                SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
            }

            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD18650
int32_t __fastcall SKILLS_SrvDo150_Smite(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || !UNITS_IsInMeleeRange(pUnit, pTarget, 0))
    {
        return 0;
    }

    D2DamageStrc damage = {};
    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
    {
        sub_6FC627B0(pUnit, MONMODE_ATTACK2);
        damage.dwStunLen = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, 0, 0);
    }
    else
    {
        D2UnitStrc* pShield = nullptr;
        if (!INVENTORY_GetEquippedShield(pUnit->pInventory, &pShield))
        {
            return 0;
        }

        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pShield ? pShield->dwClassId : -1);
        if (!pItemsTxtRecord)
        {
            return 0;
        }

        int32_t nMinDamage = pItemsTxtRecord->nMinDam << 8;
        int32_t nMaxDamage = pItemsTxtRecord->nMaxDam << 8;
        if (STATES_CheckState(pUnit, STATE_HOLYSHIELD))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_HOLYSHIELD);
            if (pStatList)
            {
                const int32_t nHolyShieldId = STATLIST_GetSkillId(pStatList);
                const int32_t nHolyShieldLevel = STATLIST_GetSkillLevel(pStatList);
                nMinDamage += SKILLS_GetMinPhysDamage(pUnit, nHolyShieldId, nHolyShieldLevel, 1);
                nMaxDamage += SKILLS_GetMaxPhysDamage(pUnit, nHolyShieldId, nHolyShieldLevel, 1);
            }
        }

        damage.dwPhysDamage = SUNITDMG_ApplyDamageBonuses(pUnit, 0, pShield, nMinDamage, nMaxDamage, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel), 0, 0x80u);
        
        const int32_t nStunLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        if (pSkillsTxtRecord->nEType)
        {
            damage.dwConvPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
            if (damage.dwConvPct > 0)
            {
                damage.nConvType = pSkillsTxtRecord->nEType;
            }
        }

        D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);

        damage.dwHitFlags = 2;
        damage.dwStunLen = nStunLength;
        damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, 0, 0) | 1;
    }

    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
    }

    damage.dwHitClass = 0x65u;
    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    return 1;
}

//D2Game.0x6FD18900
int32_t __fastcall SKILLS_SrvDo065_BasicAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    const int32_t nManaCost = SKILLS_GetManaCosts(nSkillId, nSkillLevel);
    const int32_t nMana = STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0);
    
    D2BasicAuraParamStrc args = {};
    args.nSkillId = nSkillId;
    args.nSkillLevel = nSkillLevel;
    args.nDuration = sub_6FD13220(pGame, pUnit, pSkillsTxtRecord, nSkillId, nSkillLevel) - pGame->dwGameFrame + 1;

    if (pSkillsTxtRecord->nPassiveState > 0)
    {
        args.nPassiveState = pSkillsTxtRecord->nPassiveState;
    }

    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER || nMana >= nManaCost)
    {
        for (int32_t i = 0; i < 6; ++i)
        {
            if (pSkillsTxtRecord->wAuraStat[i] < 0)
            {
                break;
            }

            args.nAuraStat[i] = pSkillsTxtRecord->wAuraStat[i];
            args.nAuraStatCalcValue[i] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], nSkillId, nSkillLevel);
        }
    }

    if (pSkillsTxtRecord->nAuraState > 0 && pSkillsTxtRecord->nAuraState < sgptDataTables->nStatesTxtRecordCount)
    {
        args.nAuraState = pSkillsTxtRecord->nAuraState;

        sub_6FD0FE50(pGame, pUnit, SKILLS_AuraCallback_BasicAura, &args);

        if (args.pStatList && nMana > nManaCost && pSkillsTxtRecord->nPassiveState <= 0)
        {
            for (int32_t i = 0; i < 5; ++i)
            {
                const int32_t nStat = pSkillsTxtRecord->nPassiveStat[i];
                const int32_t nValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[i], nSkillId, nSkillLevel);
                if (nValue && nStat > 0 && nStat < sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    STATLIST_SetStatIfListIsValid(args.pStatList, nStat, nValue, 0);
                }
            }
        }
    }

    if (pSkillsTxtRecord->wAuraTargetState >= 0 && pSkillsTxtRecord->wAuraTargetState < sgptDataTables->nStatesTxtRecordCount)
    {
        args.nAuraState = pSkillsTxtRecord->wAuraTargetState;

        const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);

        sub_6FD0FE80(pGame, pUnit, 0, 0, nAuraRange, pSkillsTxtRecord->dwAuraFilter, SKILLS_AuraCallback_BasicAura, &args, 1, __FILE__, __LINE__);

        if (nManaCost > 0)
        {
            if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
            {
                if (args.field_40 > 0)
                {
                    STATES_ToggleState(pUnit, STATE_NOMANAREGEN, 1);
                    D2GAME_SKILLMANA_AuraConsume_6FD10C90(pUnit, nManaCost);
                }
                else
                {
                    STATES_ToggleState(pUnit, STATE_NOMANAREGEN, 0);
                }
            }
        }
    }

    return 1;
}

//D2Game.0x6FD18BC0
int32_t __fastcall SKILLS_AuraCallback_BasicAura(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2BasicAuraParamStrc* pArgs = (D2BasicAuraParamStrc*)pAuraCallback->pArgs;

    D2CurseStrc curse = {};

    curse.pUnit = pAuraCallback->pOwner;
    curse.pTarget = pUnit;
    curse.nSkill = pArgs->nSkillId;
    curse.nSkillLevel = pArgs->nSkillLevel;
    curse.nDuration = pArgs->nDuration;
    curse.nStat = -1;
    curse.nStatValue = 0;
    curse.nState = pArgs->nAuraState;

    if (pArgs->pStateRemoveCallback)
    {
        curse.pStateRemoveCallback = pArgs->pStateRemoveCallback;
    }
    else if (pAuraCallback->pOwner == pUnit)
    {
        curse.pStateRemoveCallback = SKILLS_CurseStateCallback_BasicAura;
    }

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    if (!pStatList)
    {
        return 0;
    }

    pArgs->pStatList = pStatList;

    for (int32_t j = 0; j < 6; ++j)
    {
        if (pArgs->nAuraStatCalcValue[j])
        {
            const int32_t nStatId = pArgs->nAuraStat[j];
            if (nStatId > 0 && nStatId < sgptDataTables->nItemStatCostTxtRecordCount)
            {
                if (nStatId == STAT_ITEM_POISONLENGTHRESIST)
                {
                    int32_t bPoisonedOrCursed = 0;
                    if (STATES_CheckState(pUnit, STATE_POISON))
                    {
                        D2StatListStrc* pPoisonStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_POISON);
                        if (pPoisonStatList)
                        {
                            const int32_t nExpireFrame = pAuraCallback->pGame->dwGameFrame + MONSTERUNIQUE_CalculatePercentage(D2COMMON_10473(pPoisonStatList) - pAuraCallback->pGame->dwGameFrame, pArgs->nAuraStatCalcValue[j], 100);
                            D2COMMON_10476(pPoisonStatList, nExpireFrame);
                            EVENT_SetEvent(pAuraCallback->pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);
                            bPoisonedOrCursed = 1;
                        }
                    }

                    for (int32_t i = 0; i < sgptDataTables->nCurseStates; ++i)
                    {
                        const int32_t nCurseStateId = sgptDataTables->pCurseStates[i];
                        if (STATES_CheckState(pUnit, nCurseStateId))
                        {
                            D2StatListStrc* pCurseStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nCurseStateId);
                            if (pCurseStatList)
                            {
                                if (STATES_CheckStateMaskCurableByStateId(nCurseStateId))
                                {
                                    const int32_t nExpireFrame = pAuraCallback->pGame->dwGameFrame + MONSTERUNIQUE_CalculatePercentage(D2COMMON_10473(pCurseStatList) - pAuraCallback->pGame->dwGameFrame, pArgs->nAuraStatCalcValue[j], 100);
                                    D2COMMON_10476(pCurseStatList, nExpireFrame);
                                    EVENT_SetEvent(pAuraCallback->pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);
                                }
                                bPoisonedOrCursed = 1;
                            }
                        }
                    }

                    if (bPoisonedOrCursed)
                    {
                        ++pArgs->field_40;
                    }
                }
                else
                {
                    int32_t nBonus = sub_6FD0B3D0(pUnit, nStatId, pArgs->nAuraStatCalcValue[j]);
                    if (nBonus)
                    {
                        if (nStatId < sgptDataTables->nItemStatCostTxtRecordCount)
                        {
                            D2ItemStatCostTxt* pItemStatCostTxtRecord = &sgptDataTables->pItemStatCostTxt[nStatId];
                            if (pItemStatCostTxtRecord)
                            {
                                if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DIRECT])
                                {
                                    const int32_t nBaseValue = STATLIST_UnitGetStatValue(pUnit, nStatId, 0);
                                    int32_t nValue = nBaseValue + nBonus;
                                    if (pItemStatCostTxtRecord->wMaxStat < sgptDataTables->nItemStatCostTxtRecordCount)
                                    {
                                        const int32_t nMaxValue = STATLIST_UnitGetStatValue(pUnit, pItemStatCostTxtRecord->wMaxStat, 0);
                                        if (nValue >= nMaxValue)
                                        {
                                            nValue = nMaxValue;
                                        }
                                    }

                                    if (nValue != nBaseValue)
                                    {
                                        STATLIST_SetUnitStat(pUnit, nStatId, nValue, 0);
                                        ++pArgs->field_40;
                                    }
                                }
                                else
                                {
                                    if (STATLIST_GetStatValue(pStatList, nStatId, 0) != nBonus)
                                    {
                                        STATES_ToggleGfxStateFlag(pUnit, curse.nState, 1);
                                    }

                                    if (nStatId >= STAT_VELOCITYPERCENT && nStatId <= STAT_ATTACKRATE)
                                    {
                                        const int32_t nColdEffect = SUNITDMG_GetColdEffect(SUNIT_GetGameFromUnit(pUnit), pUnit);
                                        if (nBonus < nColdEffect)
                                        {
                                            nBonus = nColdEffect;
                                        }
                                    }

                                    STATLIST_SetStatIfListIsValid(pStatList, nStatId, nBonus, 0);

                                    if (nStatId == STAT_ATTACKRATE)
                                    {
                                        STATLIST_SetStatIfListIsValid(pStatList, STAT_OTHER_ANIMRATE, nBonus, 0);
                                    }

                                    ++pArgs->field_40;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, pArgs->nSkillId, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, pArgs->nSkillLevel, 0);

    if (pArgs->nPassiveState > 0)
    {
        D2StatListStrc* pPassiveStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pArgs->nPassiveState);
        if (pPassiveStatList)
        {
            D2Common_10474(pUnit, pPassiveStatList);
            STATLIST_FreeStatList(pPassiveStatList);
            STATES_ToggleGfxStateFlag(pUnit, pArgs->nPassiveState, 1);
        }
    }

    return 1;
}

//D2Game.0x6FD18FE0
void __fastcall SKILLS_CurseStateCallback_BasicAura(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    D2_MAYBE_UNUSED(pStatList);
    STATES_ToggleState(pUnit, nState, 0);
    if (STATES_CheckState(pUnit, STATE_NOMANAREGEN))
    {
        STATES_ToggleState(pUnit, STATE_NOMANAREGEN, 0);
    }

    D2GAME_RefreshPassiveSkills_6FD14CC0(pUnit);
    sub_6FD0FDD0(pUnit);
}

//D2Game.0x6FD19020
int32_t __fastcall SKILLS_SrvDo066_HolyFire_HolyShock_Sanctuary_Conviction(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    const int32_t nManaCost = SKILLS_GetManaCosts(nSkillId, nSkillLevel);
    const int32_t nMana = STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0);
    const int32_t nFrame = sub_6FD13220(pGame, pUnit, pSkillsTxtRecord, nSkillId, nSkillLevel) - pGame->dwGameFrame + 1;
    
    D2BasicAuraParamStrc args1 = {};
    args1.nSkillId = nSkillId;
    args1.nSkillLevel = nSkillLevel;
    args1.nDuration = nFrame;
    args1.nAuraState = pSkillsTxtRecord->nAuraState;

    for (int32_t i = 0; i < 5; ++i)
    {
        args1.nAuraStat[i] = pSkillsTxtRecord->nPassiveStat[i];
        if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER || nMana >= nManaCost)
        {
            args1.nAuraStatCalcValue[i] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[i], nSkillId, nSkillLevel);
        }
    }

    sub_6FD0FE50(pGame, pUnit, SKILLS_AuraCallback_BasicAura, &args1);
    
    if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
    {
        return 1;
    }

    D2DamageAuraParamStrc args2 = {};
    args2.nSkillId = nSkillId;
    args2.nSkillLevel = nSkillLevel;
    args2.nDuration = nFrame;
    args2.nAuraState = pSkillsTxtRecord->wAuraTargetState;

    if (pSkillsTxtRecord->wAuraTargetState >= 0 && pSkillsTxtRecord->wAuraTargetState < sgptDataTables->nStatesTxtRecordCount)
    {
        for (int32_t i = 0; i < 6; ++i)
        {
            args2.nAuraStat[i] = pSkillsTxtRecord->wAuraStat[i];
            if (nMana >= nManaCost)
            {
                args2.nAuraStatCalcValue[i] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], nSkillId, nSkillLevel);
                if (args2.nAuraStatCalcValue[i])
                {
                    args2.nPassiveState = 1;
                }
            }
        }
    }

    D2DamageStrc damage = {};
    if (D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel))
    {
        damage.dwHitClass |= 0xDu;
        if (pSkillsTxtRecord->dwHitClass)
        {
            damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
        }

        damage.wResultFlags |= pSkillsTxtRecord->wResultFlags | 0x20;
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
        args2.pDamage = &damage;
    }

    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);

    sub_6FD0FE80(pGame, pUnit, 0, 0, nAuraRange, pSkillsTxtRecord->dwAuraFilter, SKILLS_AuraCallback_HolyFire_HolyShock_Sanctuary_Conviction, &args2, 0, __FILE__, __LINE__);

    if (nManaCost > 0 && pUnit && !pUnit->dwUnitType)
    {
        if (args2.field_40 > 0)
        {
            STATES_ToggleState(pUnit, STATE_NOMANAREGEN, 1);
            D2GAME_SKILLMANA_AuraConsume_6FD10C90(pUnit, nManaCost);
        }
        else
        {
            STATES_ToggleState(pUnit, STATE_NOMANAREGEN, 0);
        }
    }

    return 1;
}

//D2Game.0x6FD19390
int32_t __fastcall SKILLS_AuraCallback_HolyFire_HolyShock_Sanctuary_Conviction(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2DamageAuraParamStrc* pArgs = (D2DamageAuraParamStrc*)pAuraCallback->pArgs;
    if (pArgs->nPassiveState)
    {
        D2BasicAuraParamStrc args = {};
        args.nAuraState = pArgs->nAuraState;
        args.nSkillId = pArgs->nSkillId;
        args.nSkillLevel = pArgs->nSkillLevel;
        args.nDuration = pArgs->nDuration;

        for (int32_t i = 0; i < 6; ++i)
        {
            args.nAuraStat[i] = pArgs->nAuraStat[i];
            args.nAuraStatCalcValue[i] = pArgs->nAuraStatCalcValue[i];
        }

        D2AuraCallbackStrc auraCallback = {};
        auraCallback.pOwner = pAuraCallback->pOwner;
        auraCallback.pArgs = &args;
        auraCallback.pGame = pAuraCallback->pGame;
        SKILLS_AuraCallback_BasicAura(&auraCallback, pUnit);
    }
    
    if (pArgs->pDamage)
    {
        D2DamageStrc damage = {};
        memcpy(&damage, pArgs->pDamage, sizeof(D2DamageStrc));
        SUNITDMG_ExecuteEvents(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, 1, &damage);
        SUNITDMG_ExecuteMissileDamage(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, &damage);
    }

    return 1;
}

//D2Game.0x6FD19460
int32_t __fastcall SKILLS_SrvSt31_Charge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTargetUnit && UNITS_IsInMeleeRange(pUnit, pTargetUnit, 0))
    {
        if (pUnit->dwUnitType != UNIT_PLAYER)
        {
            SUNIT_SetCombatMode(pGame, pUnit, 1);
            UNITS_SetUsedSkill(pUnit, 0);

            int32_t nMode = MONMODE_ATTACK1;
            if (pUnit->dwUnitType == UNIT_MONSTER)
            {
                int32_t nBaseId = pUnit->dwClassId;
                D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
                if (pMonStatsTxtRecord)
                {
                    nBaseId = pMonStatsTxtRecord->nBaseId;
                }

                if (nBaseId == MONSTER_CLAWVIPER1 || nBaseId == MONSTER_DURIEL)
                {
                    nMode = MONMODE_ATTACK2;
                }
            }

            D2ModeChangeStrc modeChange = {};
            MONSTERMODE_GetModeChangeInfo(pUnit, nMode, &modeChange);
            modeChange.pTargetUnit = pTargetUnit;
            return D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
        }

        return sub_6FD150A0(pGame, pUnit, pTargetUnit);
    }

    if (!pTargetUnit || STATES_CheckState(pTargetUnit, STATE_UNINTERRUPTABLE))
    {
        SKILLS_SetParam1(pSkill, 6);
        SKILLS_SetParam2(pSkill, 0);
    }
    else
    {
        SKILLS_SetParam1(pSkill, pTargetUnit->dwUnitType);
        SKILLS_SetParam2(pSkill, pTargetUnit->dwUnitId);
    }

    SKILLS_SetParam4(pSkill, 7);

    int32_t nBaseVelocity = 256;
    if (pUnit->dwUnitType)
    {
        D2MonStatsTxt* pMonstatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
        if (pMonstatsTxtRecord)
        {
            nBaseVelocity = pMonstatsTxtRecord->nRun << 8;
            if (pUnit->dwClassId == MONSTER_DURIEL)
            {
                SKILLS_SetParam4(pSkill, 15);
            }

            D2COMMON_10204_PATH_ClearPoint2(pUnit->pDynamicPath);
            PATH_SetNewDistance(pUnit->pDynamicPath, 20);
        }
    }
    else
    {
        D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pUnit->dwClassId);
        if (pCharStatsTxtRecord)
        {
            nBaseVelocity = ((int32_t)pCharStatsTxtRecord->nRunSpeed << 8);
        }
    }

    int32_t nVelocityPercent = STATLIST_UnitGetStatValue(pUnit, STAT_VELOCITYPERCENT, 0);
    if (nVelocityPercent <= 50)
    {
        nVelocityPercent = 50;
    }

    PATH_SetVelocity(pUnit->pDynamicPath, MONSTERUNIQUE_CalculatePercentage(nBaseVelocity, pSkillsTxtRecord->dwParam[0] + nVelocityPercent, 100), __FILE__, __LINE__);
    D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
    SKILLS_SetFlags(pSkill, 0x1001u);
    STATES_ToggleState(pUnit, STATE_SKILL_MOVE, 1);

    return 1;
}

//D2Game.0x6FD19780
int32_t __fastcall SKILLS_SrvDo067_Charge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    pUnit = pUnit;
    pGame = pGame;

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    //if (DATATBLS_GetMonSeqTxtRecordFromUnit(pUnit))
    //    DATATBLS_GetSeqFrameCount(pUnit);

    int32_t nEventFrame = 7;
    D2MonSeqTxt* pMonSeqTxtRecord = DATATBLS_GetMonSeqTxtRecordFromUnit(pUnit);
    if (pMonSeqTxtRecord)
    {
        const int32_t v8 = DATATBLS_GetSeqFrameCount(pUnit);
        for (int32_t i = 0; i < v8; ++i)
        {
            if (!pMonSeqTxtRecord->nEvent) // TODO: Why is this check inside a loop?
            {
                nEventFrame = i - 1;
                break;
            }
        }
    }

    const int32_t nUnitType = SKILLS_GetParam1(pSkill);
    const int32_t nUnitGUID = SKILLS_GetParam2(pSkill);
    D2UnitStrc* pTarget = nullptr;
    if (nUnitType != 6)
    {
        pTarget = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
    }

    const int32_t nSkillFlags = SKILLS_GetFlags(pSkill);
    if (!(nSkillFlags & 1))
    {
        SKILLS_SetFlags(pSkill, 0);
        SKILLS_SetParam1(pSkill, 0);
        SKILLS_SetParam2(pSkill, 0);
        sub_6FCC63D0(pUnit, nSkillId);
        D2GAME_SKILLS_RewindSkillEx_6FCBD260(pGame, pUnit, nEventFrame);

        if (pUnit)
        {
            pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
        }

        if (!pTarget)
        {
            pTarget = sub_6FD107F0(pGame, pUnit, 0, 0, 3, 3, -1, 0);
            if (!pTarget)
            {
                D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, 0);
                EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, pGame->dwGameFrame + 1, 0, 0);
                return 0;
            }
        }

        int32_t nRange = 0;
        if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
        {
            nRange = 3;
        }

        if (!UNITS_IsInMeleeRange(pUnit, pTarget, nRange) || SUNIT_IsDead(pUnit))
        {
            return 1;
        }

        D2DamageStrc damage = {};

        if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
        {
            damage.wResultFlags = 9;
        }
        else
        {
            damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0) | 8;
        }

        damage.dwEnDmgPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
        if (pSkillsTxtRecord->nEType)
        {
            damage.dwConvPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
            if (damage.dwConvPct > 0)
            {
                damage.nConvType = pSkillsTxtRecord->nEType;
            }
        }

        D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);

        if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
        {
            const int32_t nClassId = pUnit->dwClassId;
            int32_t nMonMode = MONMODE_ATTACK1;
            if (nClassId == MONSTER_CLAWVIPER1)
            {
                nMonMode = MONMODE_SKILL1;
            }
            else if (nClassId == MONSTER_DURIEL || nClassId == MONSTER_REANIMATEDHORDE1)
            {
                nMonMode = MONMODE_ATTACK2;
            }

            sub_6FC627B0(pUnit, nMonMode);
        }

        damage.dwHitClass = 0x70u;
        SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, pSkillsTxtRecord->nSrcDam);
        SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
        UNITS_SetOverlay(pTarget, 147, 0);
        return 1;
    }

    int32_t nRange = 0;
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && nSkillFlags & 2)
    {
        nRange = 3;
    }

    if (pTarget && UNITS_IsInMeleeRange(pUnit, pTarget, nRange))
    {
        SKILLS_SetFlags(pSkill, 0);
        D2GAME_SKILLS_RewindSkillEx_6FCBD260(pGame, pUnit, nEventFrame + 1);
        sub_6FCC63D0(pUnit, nSkillId);
        return 1;
    }

    if (nSkillFlags & 2)
    {
        SKILLS_SetFlags(pSkill, 0);
        sub_6FCC63D0(pUnit, nSkillId);

        D2UnitStrc* pNewTarget = sub_6FD107F0(pGame, pUnit, 0, 0, 3, 3, nUnitGUID, 0);
        if (pNewTarget && UNITS_IsInMeleeRange(pUnit, pNewTarget, 0))
        {
            SKILLS_SetParam1(pSkill, pNewTarget->dwUnitType);
            SKILLS_SetParam2(pSkill, pNewTarget->dwUnitId);
            return 1;
        }

        //if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
        //{
        //    int32_t v17 = pUnit->dwClassId;
        //    if (v17 >= 0 && v17 < sgptDataTables->nMonStatsTxtRecordCount && (v18 = &sgptDataTables->pMonStatsTxt[v17]) != 0)
        //    {
        //        v19 = v18->nBaseId;
        //    }
        //    else
        //    {
        //        v19 = pUnit->dwClassId;
        //    }

        //    if (/*v19 >= 0 && v19 < sgptDataTables->nMonStatsTxtRecordCount && */v19 == MONSTER_REANIMATEDHORDE1 && v17 >= 0)
        //    {
        //        if (v17 < sgptDataTables->nMonStatsTxtRecordCount)
        //        {
        //            v21 = &sgptDataTables->pMonStatsTxt[v17];
        //            if (v21)
        //            {
        //                v17 = v21->nBaseId;
        //            }
        //        }

        //        if (v17 == MONSTER_SIEGEBEAST1) // TODO: This check should never succeed if v19 == MONSTER_REANIMATEDHORDE1 and thus is useless
        //        {
        //            return 0;
        //        }
        //    }
        //}

        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, 0);
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, pGame->dwGameFrame + 1, 0, 0);

        return 0;
    }

    if (pGame->dwGameFrame - (pUnit->nSeqCurrentFramePrecise >> 8) >= nEventFrame)
    {
        D2GAME_SKILLS_RewindSkillEx_6FCBD260(pGame, pUnit, 0);
        SKILLS_SetFlags(pSkill, 1);
    }

    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_MODECHANGE, 0);
    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_MODECHANGE, pGame->dwGameFrame + 1, 1, 0);

    pUnit->dwFlags &= 0xFFFFFFBF;

    return 1;
}

//D2Game.0x6FD19C80
int32_t __fastcall SKILLS_SrvSt35_Vengeance(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTargetUnit)
    {
        return 0;
    }

    D2DamageStrc damage = {};
    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTargetUnit, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0);

    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.dwHitFlags = 1;
        damage.dwPhysDamage = SUNITDMG_ApplyDamageBonuses(pUnit, 1, nullptr, 0, 0, 0, 0, pSkillsTxtRecord->nSrcDam);
        damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
        damage.dwHitFlags = pSkillsTxtRecord->dwHitFlags | 1;

        int32_t nShiftedMinDamage = 0;
        int32_t nShiftedMaxDamage = 0;
        if (sub_6FC7C7B0(pUnit))
        {
            if (INVENTORY_GetWieldType(pUnit, pUnit->pInventory) == 2)
            {
                nShiftedMinDamage = STATLIST_UnitGetStatValue(pUnit, STAT_SECONDARY_MINDAMAGE, 0);
                nShiftedMaxDamage = STATLIST_UnitGetStatValue(pUnit, STAT_SECONDARY_MAXDAMAGE, 0);
            }
            else
            {
                nShiftedMinDamage = STATLIST_UnitGetStatValue(pUnit, STAT_MINDAMAGE, 0);
                nShiftedMaxDamage = STATLIST_UnitGetStatValue(pUnit, STAT_MAXDAMAGE, 0);
            }
        }
        else
        {
            nShiftedMinDamage = STATLIST_UnitGetStatValue(pUnit, STAT_MINDAMAGE, 0);
            if (nShiftedMinDamage <= 0)
            {
                nShiftedMinDamage = 1;
            }

            nShiftedMaxDamage = STATLIST_UnitGetStatValue(pUnit, STAT_MAXDAMAGE, 0);
            if (nShiftedMaxDamage <= 1)
            {
                nShiftedMaxDamage = 2;
            }
        }

        int32_t nMinDamage = nShiftedMinDamage << 8;
        int32_t nMaxDamage = nShiftedMaxDamage << 8;
        if (nMinDamage < 1)
        {
            nMinDamage = 256;
        }

        if (nMaxDamage <= nMinDamage)
        {
            nMaxDamage = nMinDamage + 256;
        }

        const int32_t nBaseDamage = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDamage - nMinDamage) + nMinDamage;

        int32_t nFireDamagePercent = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
        if (nFireDamagePercent)
        {
            const int32_t nFireMastery = STATLIST_UnitGetStatValue(pUnit, STAT_PASSIVE_FIRE_MASTERY, 0);
            if (nFireMastery)
            {
                nFireDamagePercent += MONSTERUNIQUE_CalculatePercentage(nFireDamagePercent, nFireMastery, 100);
            }
        }

        damage.dwFireDamage += MONSTERUNIQUE_CalculatePercentage(nBaseDamage, nFireDamagePercent, 100);

        int32_t nColdDamagePercent = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        if (nColdDamagePercent)
        {
            const int32_t nColdMastery = STATLIST_UnitGetStatValue(pUnit, STAT_PASSIVE_COLD_MASTERY, 0);
            if (nColdMastery)
            {
                nColdDamagePercent += MONSTERUNIQUE_CalculatePercentage(nColdDamagePercent, nColdMastery, 100);
            }
        }

        damage.dwColdDamage += MONSTERUNIQUE_CalculatePercentage(nBaseDamage, nColdDamagePercent, 100);

        int32_t nLightningDamagePercent = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
        if (nLightningDamagePercent)
        {
            const int32_t nLightningMastery = STATLIST_UnitGetStatValue(pUnit, STAT_PASSIVE_LTNG_MASTERY, 0);
            if (nLightningMastery)
            {
                nLightningDamagePercent += MONSTERUNIQUE_CalculatePercentage(nLightningDamagePercent, nLightningMastery, 100);
            }
        }

        damage.dwLtngDamage += MONSTERUNIQUE_CalculatePercentage(nBaseDamage, nLightningDamagePercent, 100);

        damage.dwColdLen += SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);

        int32_t nElemType = SKILLS_GetParam1(pSkill);
        switch (nElemType)
        {
        case 0:
            damage.dwHitClass = 32;
            break;
        case 1:
            damage.dwHitClass = 48;
            break;
        case 2:
            damage.dwHitClass = 64;
            break;
        }

        SKILLS_SetParam1(pSkill, (nElemType + 1) % 3);
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTargetUnit, &damage, 128);

    return 1;
}

//D2Game.0x6FD1A200
int32_t __fastcall SKILLS_SrvDo073_BlessedHammer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
        int32_t nTargetX = 0;
        int32_t nTargetY = 0;
        if (nMissileId > 0 && D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
        {
            if (pUnit)
            {
                pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
            }

            D2MissileStrc missileParams = {};
            missileParams.dwFlags = 32;
            missileParams.pOwner = pUnit;
            missileParams.pOrigin = pUnit;
            missileParams.nMissile = nMissileId;
            missileParams.nTargetX = nTargetX;
            missileParams.nTargetY = nTargetY;
            missileParams.nSkill = nSkillId;
            missileParams.nSkillLevel = nSkillLevel;

            D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
            if (pMissile)
            {
                PATH_SetType(pMissile->pDynamicPath, PATHTYPE_BLESSEDHAMMER);
                D2Common_10142(pMissile->pDynamicPath, pMissile, 0);

                const int32_t nPercentage = SKILLS_GetConcentrationDamageBonus(pUnit, nSkillId) + 100;
                if (nPercentage != 100)
                {
                    const int32_t nMinDamage = MONSTERUNIQUE_CalculatePercentage(STATLIST_GetUnitBaseStat(pMissile, STAT_MAGICMINDAM, 0), nPercentage, 100);
                    STATLIST_SetUnitStat(pMissile, STAT_MAGICMINDAM, nMinDamage, 0);
                    const int32_t nMaxDamage = MONSTERUNIQUE_CalculatePercentage(STATLIST_GetUnitBaseStat(pMissile, STAT_MAGICMAXDAM, 0), nPercentage, 100);
                    STATLIST_SetUnitStat(pMissile, STAT_MAGICMAXDAM, nMaxDamage, 0);
                }

                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FD1A480
int32_t __fastcall SKILLS_SrvSt36_HolyShield(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit->pInventory)
    {
        return INVENTORY_GetEquippedShield(pUnit->pInventory, 0) != 0;
    }

    return 0;
}

//D2Game.0x6FD1A4A0
int32_t __fastcall SKILLS_SrvDo079_Conversion(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: v8
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    int32_t v8 = 0;
    if (SUNITDMG_GetDamageFromUnits(pUnit, pTarget) && !MONSTERS_GetHirelingTypeId(pTarget) && pTarget->dwUnitType == UNIT_MONSTER && STATLIST_GetUnitAlignment(pTarget) == UNIT_ALIGNMENT_EVIL && sub_6FD15190(pTarget, 11))
    {
        v8 = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 100) < SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    }

    if (pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount || !v8)
    {
        SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
        return 1;
    }

    int32_t nExpireFrame = pGame->dwGameFrame + SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    if (nExpireFrame < pGame->dwGameFrame + 1)
    {
        nExpireFrame = pGame->dwGameFrame + 1;
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pTarget, pSkillsTxtRecord->wAuraTargetState);
    if (!pStatList)
    {
        int32_t nUnitGUID = -1;
        int32_t nUnitType = 6;
        if (pUnit)
        {
            nUnitGUID = pUnit->dwUnitId;
            nUnitType = pUnit->dwUnitType;
        }

        pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 0x802u, nExpireFrame, nUnitType, nUnitGUID);
        if (!pStatList)
        {
            return 0;
        }

        STATLIST_SetState(pStatList, pSkillsTxtRecord->wAuraTargetState);
        STATLIST_SetStatRemoveCallback(pStatList, SKILLS_StatRemoveCallback_Conversion);
        D2COMMON_10475_PostStatToStatList(pTarget, pStatList, 1);
        STATES_ToggleState(pTarget, pSkillsTxtRecord->wAuraTargetState, 1);
    }

    D2COMMON_10476(pStatList, nExpireFrame);
    EVENT_SetEvent(pGame, pTarget, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);
    sub_6FD154D0(pTarget);
    sub_6FCBDD30(pTarget, 2u, 1);
    D2GAME_UpdateSummonAI_6FC401F0(pGame, pTarget, 0, pUnit->dwNodeIndex);
    UNITS_SetTargetUnitForDynamicUnit(pUnit, 0);

    for (D2StatListStrc* i = STATLIST_GetStatListFromUnitAndFlag(pTarget, 8); i; i = STATLIST_GetStatListFromUnitAndFlag(pTarget, 8))
    {
        D2Common_10474(pTarget, i);
        STATLIST_FreeStatList(i);
    }

    const int32_t nTargetLevel = STATLIST_UnitGetStatValue(pTarget, STAT_LEVEL, 0);
    const int32_t nLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
    if (nTargetLevel && nLevel < nTargetLevel && !STATLIST_GetStatListFromUnitAndState(pTarget, STATE_CONVERSION_SAVE))
    {
        D2StatListStrc* pConversionStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 0, 0, pTarget->dwUnitType, pTarget->dwUnitId);
        if (pConversionStatList)
        {
            STATLIST_SetState(pConversionStatList, STATE_CONVERSION_SAVE);
            D2COMMON_10475_PostStatToStatList(pTarget, pConversionStatList, 1);

            const int32_t nTargetHp = STATLIST_UnitGetStatValue(pTarget, STAT_HITPOINTS, 0) >> 8;
            const int32_t nTargetMaxHp = STATLIST_GetMaxLifeFromUnit(pTarget) >> 8;
            STATLIST_SetStatIfListIsValid(pConversionStatList, STAT_CONVERSION_LEVEL, nTargetLevel, 0);
            STATLIST_SetStatIfListIsValid(pConversionStatList, STAT_CONVERSION_MAXHP, nTargetMaxHp, 0);

            int32_t nNewTargetHp = MONSTERUNIQUE_CalculatePercentage(nTargetHp, nLevel, nTargetLevel) << 8;
            int32_t nNewTargetMaxHp = MONSTERUNIQUE_CalculatePercentage(nTargetMaxHp, nLevel, nTargetLevel) << 8;

            if (nNewTargetMaxHp < 1)
            {
                nNewTargetMaxHp = 1;
            }

            if (nNewTargetHp < 1)
            {
                nNewTargetHp = 1;
            }

            if (nNewTargetHp >= nNewTargetMaxHp)
            {
                nNewTargetHp = nNewTargetMaxHp;
            }

            STATLIST_SetUnitStat(pTarget, STAT_LEVEL, nLevel, 0);
            STATLIST_SetUnitStat(pTarget, STAT_HITPOINTS, nNewTargetHp, 0);
            STATLIST_SetUnitStat(pTarget, STAT_MAXHP, nNewTargetMaxHp, 0);
        }
    }

    return 1;
}

//D2Game.0x6FD1A900
void __fastcall SKILLS_StatRemoveCallback_Conversion(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    STATES_ToggleState(pUnit, nState, 0);
    sub_6FCBDD30(pUnit, 0, 1);
    D2GAME_TARGETS_Last_6FC40380(SUNIT_GetGameFromUnit(pUnit), pUnit);

    D2StatListStrc* pConversionStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_CONVERSION_SAVE);
    if (!pConversionStatList)
    {
        return;
    }

    const int32_t nConversionLevel = STATLIST_GetStatValue(pConversionStatList, STAT_CONVERSION_LEVEL, 0);
    const int32_t nConversionMaxHp = STATLIST_GetStatValue(pConversionStatList, STAT_CONVERSION_MAXHP, 0);
    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit) >> 8;
    const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);

    int32_t nNewHp = 1;
    if (nMaxHp)
    {
        nNewHp = MONSTERUNIQUE_CalculatePercentage(nConversionMaxHp, nHitpoints >> 8, nMaxHp);
    }

    STATLIST_SetUnitStat(pUnit, STAT_LEVEL, nConversionLevel, 0);
    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp << 8, 0);
    STATLIST_SetUnitStat(pUnit, STAT_MAXHP, nConversionMaxHp, 0);
    D2Common_10474(pUnit, pConversionStatList);
    STATLIST_FreeStatList(pConversionStatList);

    for (D2StatListStrc* i = STATLIST_GetStatListFromUnitAndFlag(pUnit, 8); i; i = STATLIST_GetStatListFromUnitAndFlag(pUnit, 8))
    {
        D2Common_10474(pUnit, i);
        STATLIST_FreeStatList(i);
    }
}

//D2Game.0x6FD1AA50
int32_t __fastcall SKILLS_SrvDo080_FistOfTheHeavens(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord)
    {
        const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
        if (nMissileId > 0)
        {
            D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
            if (pTarget)
            {
                D2MissileStrc missileParams = {};
                missileParams.dwFlags = 1;
                missileParams.pOwner = pUnit;
                missileParams.nX = CLIENTS_GetUnitX(pTarget);
                missileParams.nY = CLIENTS_GetUnitY(pTarget);
                missileParams.nSkillLevel = nSkillLevel;
                missileParams.nSkill = nSkillId;
                missileParams.nMissile = nMissileId;

                D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
                if (pMissile)
                {
                    MISSILE_SetTargetX(pMissile, pTarget->dwUnitType);
                    MISSILE_SetTargetY(pMissile, pTarget->dwUnitId);

                    if (pSkillsTxtRecord->wSrvOverlay >= 0 && pSkillsTxtRecord->wSrvOverlay < sgptDataTables->nOverlayTxtRecordCount)
                    {
                        UNITS_SetOverlay(pTarget, pSkillsTxtRecord->wSrvOverlay, 0);
                    }

                    return 1;
                }
            }
        }
    }

    return 0;
}

//D2Game.0x6FD1ABC0
int32_t __fastcall SKILLS_SrvDo081_HolyFreeze(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    const int32_t nManaCost = SKILLS_GetManaCosts(nSkillId, nSkillLevel);
    const int32_t nMana = STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0);
    const int32_t nFrame = sub_6FD13220(pGame, pUnit, pSkillsTxtRecord, nSkillId, nSkillLevel) - pGame->dwGameFrame + 1;
    
    D2BasicAuraParamStrc args1 = {};
    args1.nSkillId = nSkillId;
    args1.nSkillLevel = nSkillLevel;
    args1.nDuration = nFrame;
    args1.nAuraState = pSkillsTxtRecord->nAuraState;
    args1.pStateRemoveCallback = SKILLS_CurseStateCallback_HolyFreeze;

    for (int32_t i = 0; i < 5; ++i)
    {
        args1.nAuraStat[i] = pSkillsTxtRecord->nPassiveStat[i];
        if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER || nMana >= nManaCost)
        {
            args1.nAuraStatCalcValue[i] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[i], nSkillId, nSkillLevel);
        }
    }

    sub_6FD0FE50(pGame, pUnit, SKILLS_AuraCallback_BasicAura, &args1);

    if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
    {
        return 1;
    }

    D2DamageAuraParamStrc args2 = {};
    args2.nSkillId = nSkillId;
    args2.nSkillLevel = nSkillLevel;
    args2.nDuration = nFrame;
    args2.nAuraState = pSkillsTxtRecord->wAuraTargetState;

    if (pSkillsTxtRecord->wAuraTargetState >= 0 && pSkillsTxtRecord->wAuraTargetState < sgptDataTables->nStatesTxtRecordCount)
    {
        for (int32_t i = 0; i < 6; ++i)
        {
            args2.nAuraStat[i] = pSkillsTxtRecord->wAuraStat[i];
            if (nMana >= nManaCost)
            {
                args2.nAuraStatCalcValue[i] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], nSkillId, nSkillLevel);
                if (args2.nAuraStatCalcValue[i])
                {
                    args2.nPassiveState = 1;
                }
            }
        }
    }

    D2DamageStrc damage = {};
    if (D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel))
    {
        damage.dwHitClass |= 0xDu;
        if (pSkillsTxtRecord->dwHitClass)
        {
            damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
        }

        damage.wResultFlags |= pSkillsTxtRecord->wResultFlags | 0x20;
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
        args2.pDamage = &damage;
    }
    
    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    
    sub_6FD0FE80(pGame, pUnit, 0, 0, nAuraRange, pSkillsTxtRecord->dwAuraFilter, SKILLS_AuraCallback_HolyFreeze, &args2, 0, __FILE__, __LINE__);

    if (nManaCost > 0 && pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        if (args2.field_40 > 0)
        {
            STATES_ToggleState(pUnit, STATE_NOMANAREGEN, 1);
            D2GAME_SKILLMANA_AuraConsume_6FD10C90(pUnit, nManaCost);
        }
        else
        {
            STATES_ToggleState(pUnit, STATE_NOMANAREGEN, 0);
        }
    }

    return 1;
}

//D2Game.0x6FD1AF40
void __fastcall SKILLS_CurseStateCallback_HolyFreeze(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    D2_MAYBE_UNUSED(pStatList);
    if (!SUNIT_IsDead(pUnit) || !STATES_CheckStateMaskStayDeathOnUnitByStateId(pUnit, nState))
    {
        STATES_ToggleState(pUnit, nState, 0);

        if (!SUNIT_IsDead(pUnit))
        {
            STATES_ToggleState(pUnit, STATE_SHATTER, 0);
        }

        UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
    }
}

//D2Game.0x6FD1AF90
int32_t __fastcall SKILLS_AuraCallback_HolyFreeze(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2BasicAuraParamStrc* pArgs = (D2BasicAuraParamStrc*)pAuraCallback->pArgs;
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
        if (!pMonStatsTxtRecord || pMonStatsTxtRecord->nColdEffect[pAuraCallback->pGame->nDifficulty] >= 0)
        {
            return 0;
        }
    }

    if (pArgs->nPassiveState)
    {
        D2BasicAuraParamStrc args = {};

        args.nAuraState = pArgs->nAuraState;
        args.nSkillId = pArgs->nSkillId;
        args.nSkillLevel = pArgs->nSkillLevel;
        args.nDuration = pArgs->nDuration;

        for (int32_t i = 0; i < 6; ++i)
        {
            args.nAuraStat[i] = pArgs->nAuraStat[i];
            args.nAuraStatCalcValue[i] = pArgs->nAuraStatCalcValue[i];
        }

        D2AuraCallbackStrc auraCallback = {};
        auraCallback.pArgs = &args;
        auraCallback.pOwner = pAuraCallback->pOwner;
        auraCallback.pGame = pAuraCallback->pGame;
        SKILLS_AuraCallback_BasicAura(&auraCallback, pUnit);
    }

    D2DamageStrc* pDamage = (D2DamageStrc*)pArgs->pStateRemoveCallback;
    if (pDamage)
    {
        D2DamageStrc damage = {};
        memcpy(&damage, pDamage, sizeof(D2DamageStrc));
        SUNITDMG_ExecuteEvents(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, 1, &damage);
        SUNITDMG_ExecuteMissileDamage(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, &damage);
    }

    STATES_ToggleState(pUnit, STATE_SHATTER, (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < 20);

    return 1;
}

//D2Game.0x6FD1B100
int32_t __fastcall SKILLS_ApplyRedemptionEffect(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pCorpse, int32_t nSkillId, int32_t nSkillLevel, int32_t bSkipChanceRoll)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord && pCorpse && SKILLS_CanUnitCorpseBeSelected(pCorpse) && (bSkipChanceRoll || (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel)))
    {
        const int32_t nNewHp = std::min((SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel) << 8) + STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0), STATLIST_GetMaxLifeFromUnit(pUnit));
        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);

        const int32_t nNewMana = std::min((SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel) << 8) + STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0), STATLIST_GetMaxManaFromUnit(pUnit));
        STATLIST_SetUnitStat(pUnit, STAT_MANA, nNewMana, 0);

        STATES_ToggleState(pCorpse, STATE_REDEEMED, 1);
        pCorpse->dwFlags &= ~(UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD1B260
int32_t __fastcall SKILLS_SrvDo082_Redemption(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    const int32_t nManaCost = SKILLS_GetManaCosts(nSkillId, nSkillLevel);
    const int32_t nMana = STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0);

    D2BasicAuraParamStrc args1 = {};
    args1.nSkillId = nSkillId;
    args1.nSkillLevel = nSkillLevel;
    args1.nDuration = sub_6FD13220(pGame, pUnit, pSkillsTxtRecord, nSkillId, nSkillLevel) - pGame->dwGameFrame + 1;
    args1.nAuraState = pSkillsTxtRecord->nAuraState;

    for (int32_t i = 0; i < 5; ++i)
    {
        args1.nAuraStat[i] = pSkillsTxtRecord->nPassiveStat[i];
        if (args1.nAuraStat[i] >= 0 && (!pUnit || pUnit->dwUnitType != UNIT_PLAYER || nMana >= nManaCost))
        {
            args1.nAuraStatCalcValue[i] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[i], nSkillId, nSkillLevel);
        }
    }

    sub_6FD0FE50(pGame, pUnit, SKILLS_AuraCallback_BasicAura, &args1);

    if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
    {
        return 1;
    }

    D2UnkPalSkillStrc2 args2 = {};
    args2.nSkillId = nSkillId;
    args2.nSkillLevel = nSkillLevel;
    
    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);

    sub_6FD0FE80(pGame, pUnit, 0, 0, nAuraRange, pSkillsTxtRecord->dwAuraFilter, SKILLS_AuraCallback_Redemption, &args2, 1, __FILE__, __LINE__);

    if (nManaCost > 0 && pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        if (args2.nCounter > 0)
        {
            STATES_ToggleState(pUnit, STATE_NOMANAREGEN, 1);
            D2GAME_SKILLMANA_AuraConsume_6FD10C90(pUnit, nManaCost);
        }
        else
        {
            STATES_ToggleState(pUnit, STATE_NOMANAREGEN, 0);
        }
    }

    return 1;
}

//D2Game.0x6FD1B490
int32_t __fastcall SKILLS_AuraCallback_Redemption(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2UnkPalSkillStrc2* pArgs = (D2UnkPalSkillStrc2*)pAuraCallback->pArgs;

    const int32_t nResult = SKILLS_ApplyRedemptionEffect(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, pArgs->nSkillId, pArgs->nSkillLevel, 0);
    if (nResult)
    {
        ++pArgs->nCounter;
    }

    return nResult;
}

//D2Game.0x6FD1B4C0
void __fastcall SKILLS_ApplyThornsDamage(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage)
{
    int32_t nThornsPercent = STATLIST_UnitGetStatValue(pDefender, STAT_THORNS_PERCENT, 0);
    if (nThornsPercent <= 0)
    {
        return;
    }

    if (pAttacker && pAttacker->dwUnitType == UNIT_PLAYER || MONSTERS_GetHirelingTypeId(pAttacker))
    {
        nThornsPercent = (nThornsPercent + 4) / 8;
    }

    if (nThornsPercent <= 0)
    {
        return;
    }

    D2DamageStrc damage = {};
    damage.dwPhysDamage = MONSTERUNIQUE_CalculatePercentage(pDamage->dwPhysDamage, nThornsPercent, 100);
    damage.dwHitClass = 0x8Du;
    damage.wResultFlags = 0x4021u;
    SUNITDMG_ExecuteEvents(pGame, pDefender, pAttacker, 1, &damage);
    SUNITDMG_ExecuteMissileDamage(pGame, pDefender, pAttacker, &damage);
}
