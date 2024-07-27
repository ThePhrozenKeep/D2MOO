#include "SKILLS/SkillAss.h"

#include <algorithm>

#include <D2BitManip.h>

#include <D2StatList.h>
#include <D2Dungeon.h>
#include <D2States.h>
#include <D2DataTbls.h>
#include <D2Monsters.h>
#include <D2Skills.h>
#include <D2Items.h>
#include <D2Combat.h>
#include <DataTbls/SkillsIds.h>
#include <DataTbls/MissilesIds.h>

#include <UselessOrdinals.h>

#include "AI/AiGeneral.h"
#include "AI/AiThink.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Targets.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterUnique.h"
#include "SKILLS/SkillDruid.h"
#include "SKILLS/SkillNec.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitInactive.h"
#include "UNIT/SUnitMsg.h"


#pragma pack(push, 1)
struct D2SrvDo47Strc
{
    int32_t nSkillId;
    int32_t nSkillLevel;
    int32_t nDuration;
    int32_t nAuraTargetState;
    int32_t nAuraStat[6];
    int32_t nAuraStatCalcValue[6];
};

struct D2SrvDo51Strc
{
    D2DamageStrc* pDamage;
    int32_t nMonCurseResSubstraction;
    int32_t nParam1;
    int32_t nParam2;
};

struct D2SrvDo142Strc
{
    D2DamageStrc* pDamage;
    int32_t nToHit;
    int16_t nResultFlags;
    int16_t unk0x0A;
    int32_t nHitFlags;
    uint8_t nSrcDam;
};
#pragma pack(pop)


//D2Game.0x6FCF5090
int32_t __fastcall SKILLS_SrvSt22_PsychicHammer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTargetUnit)
    {
        return 0;
    }

    D2ActiveRoomStrc* pUnitRoom = UNITS_GetRoom(pUnit);
    if (DUNGEON_IsRoomInTown(pUnitRoom))
    {
        return 0;
    }

    D2ActiveRoomStrc* pTargetRoom = UNITS_GetRoom(pTargetUnit);
    if (DUNGEON_IsRoomInTown(pTargetRoom))
    {
        return 0;
    }

    if (pTargetUnit->dwUnitType != UNIT_PLAYER && pTargetUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    return 1;
}

//D2Game.0x6FCF50E0
int32_t __fastcall SKILLS_SrvDo033_PsychicHammer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (!SKILLS_SrvSt22_PsychicHammer(pGame, pUnit, nSkillId, nSkillLevel))
    {
        return 0;
    }

    D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);

    D2DamageStrc damage = {};
    SUNITDMG_RollDamage(pUnit, nSkillId, nSkillLevel, &damage);
    SUNITDMG_ExecuteEvents(pGame, pUnit, pTargetUnit, 1, &damage);
    damage.wResultFlags |= 1u;

    if (nSkillLevel < 1)
    {
        nSkillLevel = 1;
    }

    int32_t nCalc = 0;
    if (pTargetUnit && pTargetUnit->dwUnitType == UNIT_PLAYER || MONSTERS_GetHirelingTypeId(pTargetUnit))
    {
        nCalc = pSkillsTxtRecord->dwCalc[3];
    }
    else if (MONSTERS_IsBoss(0, pTargetUnit))
    {
        nCalc = pSkillsTxtRecord->dwCalc[2];
    }
    else if (MONSTERUNIQUE_CheckMonTypeFlag(pTargetUnit, 8u))
    {
        nCalc = pSkillsTxtRecord->dwCalc[1];
    }
    else
    {
        nCalc = pSkillsTxtRecord->dwCalc[0];
    }

    const int32_t nChance = SKILLS_EvaluateSkillFormula(pUnit, nCalc, nSkillId, nSkillLevel);
    if (nChance > 0 && (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < nChance)
    {
        damage.wResultFlags |= 8u;
    }

    if (!(STATLIST_UnitGetStatValue(pTargetUnit, STAT_HITPOINTS, 0) & 0xFFFFFF00))
    {
        damage.wResultFlags |= 2u;
    }

    SUNITDMG_ExecuteMissileDamage(pGame, pUnit, pTargetUnit, &damage);
    return 1;
}

//D2Game.0x6FCF52C0
int32_t __fastcall SKILLS_SrvSt23_AssasinChargeStrikes(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTargetUnit)
    {
        return 0;
    }

    return UNITS_IsInMeleeRange(pUnit, pTargetUnit, 0);
}

//D2Game.0x6FCF52E0
void __fastcall SKILLS_StatRemoveCallback_ProgressiveStrike(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    D2_MAYBE_UNUSED(pStatList);
    STATES_ToggleState(pUnit, nState, 0);
}

//D2Game.0x6FCF52F0
int32_t __fastcall SKILLS_SrvDo034_TigerStrike_CobraStrike_RoyalStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{    
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    if (pSkillsTxtRecord->wAuraStat[0] < 0 || pSkillsTxtRecord->wAuraStat[0] >= sgptDataTables->nItemStatCostTxtRecordCount)
    {
        return 0;
    }

    D2DamageStrc damage = {};
    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0);

    uint8_t nSrcDam = pSkillsTxtRecord->nSrcDam;
    if (!pSkillsTxtRecord->nSrcDam)
    {
        nSrcDam = 0x80u;
    }
    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, nSrcDam);

    D2DamageStrc* pDamage = SUNITDMG_GetDamageFromUnits(pUnit, pTarget);
    if (pDamage && pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        const int32_t nExpireFrame = pGame->dwGameFrame + SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
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
            if (!pStatList)
            {
                SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
                return 1;
            }

            STATLIST_SetState(pStatList, pSkillsTxtRecord->nAuraState);
            STATLIST_SetStatRemoveCallback(pStatList, SKILLS_StatRemoveCallback_ProgressiveStrike);
            D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
            STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
            STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);
        }

        D2COMMON_10476(pStatList, nExpireFrame);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_REMOVESTATE, nExpireFrame, 0, 0);

        const int32_t nOldValue = STATLIST_GetStatValue(pStatList, pSkillsTxtRecord->wAuraStat[0], 0);

        int32_t nNewValue = nOldValue + 1;
        if (nNewValue > 3)
        {
            nNewValue = 3;
        }

        if (nOldValue != nNewValue)
        {
            STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxtRecord->wAuraStat[0], nNewValue, 0);

            if (pSkillsTxtRecord->wAuraStat[1] >= 0 && pSkillsTxtRecord->wAuraStat[1] < sgptDataTables->nItemStatCostTxtRecordCount)
            {
                STATLIST_AddStat(pStatList, pSkillsTxtRecord->wAuraStat[1], SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[1], nSkillId, nSkillLevel), 0);
            }

            STATES_ToggleState(pUnit, pSkillsTxtRecord->nAuraState, 1);
            STATES_ToggleGfxStateFlag(pUnit, pSkillsTxtRecord->nAuraState, 1);
        }
    }

    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    return 1;
}

//D2Game.0x6FCF55B0
int32_t __fastcall SKILLS_SrvDo035_FistsOfFire_ClawsOfThunder_BladesOfIce(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pRightWeapon = nullptr;
    D2UnitStrc* pLeftWeapon = nullptr;

    if (pUnit->pInventory)
    {
        pRightWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
        pLeftWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
    }

    if (!pUnit->pInventory || pRightWeapon != pLeftWeapon && pRightWeapon && pLeftWeapon && ITEMS_CanBeEquipped(pRightWeapon) && ITEMS_CanBeEquipped(pLeftWeapon) && ITEMS_CheckItemTypeId(pRightWeapon, ITEMTYPE_WEAPON) && ITEMS_CheckItemTypeId(pLeftWeapon, ITEMTYPE_WEAPON))
    {
        if (((pUnit->dwSeqFrame >> 8) % 2) != 0)
        {
            pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
        }
		else
		{
			pUnit->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;
		}
    }
    else
    {
		pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    SKILLS_SrvDo034_TigerStrike_CobraStrike_RoyalStrike(pGame, pUnit, nSkillId, nSkillLevel);
    return 1;
}

//D2Game.0x6FCF5680
void __fastcall sub_6FCF5680(D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pDamage || !(pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT))
    {
        return;
    }

    for (int32_t i = 0; i < sgptDataTables->nProgressiveStates; ++i)
    {
        if (STATES_CheckState(pUnit, sgptDataTables->pProgressiveStates[i]))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, sgptDataTables->pProgressiveStates[i]);
            if (pStatList)
            {
                const int32_t nSkillId = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);
                D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
                if (pSkillsTxtRecord && pSkillsTxtRecord->nPrgDamage == 1)
                {
                    const int32_t nMaxLevel = SKILLS_GetSkillLevel(pUnit, SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId), 1);
                    const int32_t nSkillLevel = std::max(STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0), nMaxLevel);
                    if (nSkillLevel > 0)
                    {
                        const int32_t nAuraStat = pSkillsTxtRecord->wAuraStat[0];
                        if (nAuraStat >= 0 && nAuraStat < sgptDataTables->nItemStatCostTxtRecordCount)
                        {
                            const int32_t nValue = STATLIST_GetStatValue(pStatList, nAuraStat, 0);
                            if (nValue > 0)
                            {
                                pDamage->dwEnDmgPct += nValue * SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

                                D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(SUNIT_GetGameFromUnit(pUnit), pUnit);
                                if (pTargetUnit && pSkillsTxtRecord->wTgtOverlay > 0 && pSkillsTxtRecord->wTgtOverlay < sgptDataTables->nOverlayTxtRecordCount)
                                {
                                    UNITS_SetOverlay(pTargetUnit, pSkillsTxtRecord->wTgtOverlay, 0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//D2Game.0x6FCF5870
void __fastcall sub_6FCF5870(D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pDamage || !(pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT))
    {
        return;
    }

    for (int32_t i = 0; i < sgptDataTables->nProgressiveStates; ++i)
    {
        if (STATES_CheckState(pUnit, sgptDataTables->pProgressiveStates[i]))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, sgptDataTables->pProgressiveStates[i]);
            if (pStatList)
            {
                const int32_t nSkillId = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);
                D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
                if (pSkillsTxtRecord)
                {
                    switch (pSkillsTxtRecord->nPrgDamage)
                    {
                    case 2:
                    {
                        const int32_t nMaxSkillLevel = SKILLS_GetSkillLevel(pUnit, SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId), 1);
                        const int32_t nSkillLevel = std::max(STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0), nMaxSkillLevel);
                        if (nSkillLevel > 0 && pSkillsTxtRecord->wAuraStat[0] >= 0 && pSkillsTxtRecord->wAuraStat[0] < sgptDataTables->nItemStatCostTxtRecordCount)
                        {
                            const int32_t nValue = STATLIST_GetStatValue(pStatList, pSkillsTxtRecord->wAuraStat[0], 0);
                            if (nValue > 0)
                            {
                                int32_t nLeech = pSkillsTxtRecord->dwParam[0] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[1];

                                switch (nValue)
                                {
                                case 1:
                                    pDamage->dwLifeLeech += nLeech;
                                    break;

                                case 2:
                                    pDamage->dwLifeLeech += nLeech;
                                    pDamage->dwManaLeech += nLeech;
                                    break;

                                case 3:
                                    nLeech *= 2;
                                    pDamage->dwLifeLeech += nLeech;
                                    pDamage->dwManaLeech += nLeech;
                                    break;

                                default:
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    case 3:
                    {
                        int32_t nSkillLevel = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0);
                        const int32_t nMaxSkillLevel = SKILLS_GetSkillLevel(pUnit, SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId), 1);
                        if (nMaxSkillLevel > nSkillLevel)
                        {
                            nSkillLevel = nMaxSkillLevel;
                        }

                        if (nSkillLevel > 0 && pSkillsTxtRecord->wAuraStat[0] >= 0 && pSkillsTxtRecord->wAuraStat[0] < sgptDataTables->nItemStatCostTxtRecordCount)
                        {
                            const int32_t nValue = STATLIST_GetStatValue(pStatList, pSkillsTxtRecord->wAuraStat[0], 0);
                            if (nValue > 0)
                            {
                                D2GAME_RollElementalDamage_6FD14DD0(pUnit, pDamage, nSkillId, nSkillLevel);
                                if (pSkillsTxtRecord->nEType == ELEMTYPE_COLD && nValue >= 2 && nValue <= 3 && pSkillsTxtRecord->dwParam[1])
                                {
                                    pDamage->dwFrzLen += pDamage->dwColdLen / pSkillsTxtRecord->dwParam[1];
                                }

                                pDamage->wResultFlags |= 0x4000u;
                            }
                        }
                        break;
                    }
                    case 4:
                    {
                        sub_6FCF5BC0(pUnit, pStatList, pDamage);
                        break;
                    }
					default:
						break;
                    }
                }
            }
        }
    }
}

//D2Game.0x6FCF5B90
D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId)
{
    if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
    {
        return &sgptDataTables->pSkillsTxt[nSkillId];
    }

    return nullptr;
}

//D2Game.0x6FCF5BC0
void __fastcall sub_6FCF5BC0(D2UnitStrc* pUnit, D2StatListStrc* pStatList, D2DamageStrc* pDamage)
{
    const int32_t nSkillId = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return;
    }

    int32_t nSkillLevel = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0);
    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    const int32_t nLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);
    if (nLevel > nSkillLevel)
    {
        nSkillLevel = nLevel;
    }

    if (nSkillLevel > 0)
    {
        const int32_t nAuraStat = pSkillsTxtRecord->wAuraStat[0];
        if (nAuraStat >= 0 && nAuraStat < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            const int32_t nValue = STATLIST_GetStatValue(pStatList, nAuraStat, 0);
            if (nValue > 0)
            {
                D2GAME_RollElementalDamage_6FD14DD0(pUnit, pDamage, nSkillId, nSkillLevel);

                if (pSkillsTxtRecord->nEType == ELEMTYPE_COLD && nValue == 3 && pSkillsTxtRecord->dwParam[4])
                {
                    pDamage->dwFrzLen += pDamage->dwColdLen / pSkillsTxtRecord->dwParam[4];
                }

                pDamage->wResultFlags |= DAMAGERESULTFLAG_SOFTHIT;

                int32_t nPercentage = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
                if (nPercentage > 0)
                {
                    nPercentage = std::min(nPercentage, 100);

                    const int32_t nBaseDamage = pDamage->dwPhysDamage;
                    const int32_t nDamage = std::min(MONSTERUNIQUE_CalculatePercentage(nBaseDamage, nPercentage, 100), nBaseDamage);
                    pDamage->dwPhysDamage = nBaseDamage - nDamage;
                    sub_6FD11E40(pUnit, pDamage, pSkillsTxtRecord->nEType, nDamage, 0, 0, 0);
                }
            }
        }
    }
}

//D2Game.0x6FCF5DE0
int32_t __fastcall sub_6FCF5DE0(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2DamageStrc damage = {};

    memcpy(&damage, (D2DamageStrc*)pAuraCallback->pArgs, sizeof(damage));
    sub_6FD10140(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, &damage, 0);

    return 1;
}

//D2Game.0x6FCF5E20
int32_t __fastcall SKILLS_GetProgressiveSkillMissileId(D2UnitStrc* pUnit, int32_t nSkillId)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return -1;
    }

    if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_PROGRESSIVE] && pSkillsTxtRecord->nAuraState >= 0 && pSkillsTxtRecord->nAuraState < sgptDataTables->nStatesTxtRecordCount)
    {
        const int32_t nAuraStat = pSkillsTxtRecord->wAuraStat[0];
        if (nAuraStat >= 0 && nAuraStat < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
            if (pStatList)
            {
                int32_t nIndex = STATLIST_GetStatValue(pStatList, nAuraStat, 0);
                if (nIndex > 1)
                {
                    if (nIndex >= 3)
                    {
                        nIndex = 3;
                    }

                    return *(&pSkillsTxtRecord->wSrvMissile + nIndex);
                }
            }
        }
    }

    return pSkillsTxtRecord->wSrvMissileA;
}

//D2Game.0x6FCF5EE0
int32_t __fastcall SKILLS_EvaluateProgressiveSkillCalc(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_PROGRESSIVE] && pSkillsTxtRecord->nAuraState >= 0 && pSkillsTxtRecord->nAuraState < sgptDataTables->nStatesTxtRecordCount)
    {
        const int32_t nAuraStat = pSkillsTxtRecord->wAuraStat[0];
        if (nAuraStat >= 0 && nAuraStat < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
            if (pStatList)
            {
                int32_t nIndex = STATLIST_GetStatValue(pStatList, nAuraStat, 0);
                if (nIndex > 1)
                {
                    if (nIndex >= 3)
                    {
                        nIndex = 3;
                    }

                    return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPrgCalc[nIndex - 1], nSkillId, nSkillLevel);
                }
            }
        }
    }

    return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPrgCalc[0], nSkillId, nSkillLevel);
}

//D2Game.0x6FCF6000
int32_t __fastcall SKILLS_SrvDo038_FistsOfFire_BladesOfIce_ProgressiveFn2(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    }

    int32_t nX = 0;
    int32_t nY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    int32_t nRange = SKILLS_EvaluateProgressiveSkillCalc(pUnit, nSkillId, nSkillLevel);
    if (!nRange)
    {
        nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    }

    D2DamageStrc damage = {};
    D2GAME_RollPhysicalDamage_6FD14EC0(pUnit, &damage, nSkillId, nSkillLevel);
    D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
    damage.wResultFlags |= 1u;
    damage.dwHitFlags |= 1;
    sub_6FD0FE80(pGame, pUnit, nX, nY, nRange, pSkillsTxtRecord->dwAuraFilter, sub_6FCF5DE0, &damage, 0, __FILE__, __LINE__);
    return 1;
}

//D2Game.0x6FCF6140
int32_t __fastcall SKILLS_SrvDo039_FistsOfFire_BladesOfIce_ProgressiveFn3(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    const uint32_t nRand = ITEMS_RollRandomNumber(&pUnit->pSeed);

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!pSkillsTxtRecord || !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId <= 0)
    {
        return 0;
    }

    int32_t nRange = SKILLS_EvaluateProgressiveSkillCalc(pUnit, nSkillId, nSkillLevel);
    if (!nRange)
    {
        nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    }

    D2MissileStrc missileParams = {};

    missileParams.nMissile = nMissileId;
    missileParams.dwFlags = 1;
    missileParams.pOwner = pUnit;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2SeedStrc seed = {};
    SEED_InitLowSeed(&seed, nRand);

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    const int32_t nArea = nRange * nRange;
    for (int32_t i = 0; i < nArea; ++i)
    {
        const int32_t nX = nRange - ITEMS_RollLimitedRandomNumber(&seed, 2 * nRange);
        const int32_t nY = nRange - ITEMS_RollLimitedRandomNumber(&seed, 2 * nRange);

        if (nX * nX + nY * nY <= nArea)
        {
            missileParams.nX = nTargetX + nX;
            missileParams.nY = nTargetY + nY;

            if (D2GAME_GetRoom_6FC52070(pRoom, missileParams.nX, missileParams.nY))
            {
                MISSILES_CreateMissileFromParams(pGame, &missileParams);
            }
        }
    }

    return 1;
}

//D2Game.0x6FCF6420
int32_t __fastcall SKILLS_SrvDo036_ClawsOfThunder_ProgressiveFn2(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || nSkillLevel <= 0)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId <= 0)
    {
        return 0;
    }

    const int32_t nVelocity = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel) + DATATBLS_GetMissileVelocityFromMissilesTxt(nMissileId, nSkillLevel);
    sub_6FD14170(pGame, pUnit, pTarget, nMissileId, nSkillId, nSkillLevel, nVelocity);
    return 1;
}

//D2Game.0x6FCF6500
int32_t __fastcall SKILLS_SrvDo037_ClawsOfThunder_ProgressiveFn3(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    int32_t nX = 0;
    int32_t nY = 0;
    if (nMissileId <= 0 || !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    int32_t nValue = SKILLS_EvaluateProgressiveSkillCalc(pUnit, nSkillId, nSkillLevel);
    if (!nValue)
    {
        nValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    }

    sub_6FCF6600(pGame, pUnit, ITEMS_RollRandomNumber(&pUnit->pSeed), nMissileId, nX, nY, nSkillId, nSkillLevel, nValue);
    return 1;
}

//D2Game.0x6FCF6600
void __fastcall sub_6FCF6600(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nLowSeed, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep)
{
    D2SeedStrc seed = {};
    SEED_InitLowSeed(&seed, nLowSeed);
    D2MissileStrc missileParams = {};
    missileParams.nX = nX;
    missileParams.nY = nY;
    missileParams.pOwner = pUnit;
    missileParams.nMissile = nMissileId;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.dwFlags = 3;
    missileParams.pInitFunc = sub_6FCF6C10;

    constexpr int32_t xOffsets[64] =
    {
        30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2, 0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29,
    };

    constexpr int32_t yOffsets[64] =
    {
        0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29, 30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2,
    };

    for (int32_t i = 0; i < 64; i += nStep)
    {
        missileParams.nTargetX = xOffsets[i];
        missileParams.nTargetY = yOffsets[i];
        missileParams.pInitArgs = ITEMS_RollRandomNumber(&seed);

        D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
        
        int32_t nParam = 0;
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(SKILL_ROYALSTRIKE);
        if (pSkillsTxtRecord)
        {
            nParam = pSkillsTxtRecord->dwParam[1];
        }

        MISSILE_SetTargetX(pMissile, nParam + 1);
    }
}

//D2Game.0x6FCF6C10
void __fastcall sub_6FCF6C10(D2UnitStrc* pMissile, int32_t nLowSeed)
{
    if (!pMissile)
    {
        return;
    }

    uint8_t nTotalFrames = MISSILE_GetTotalFrames(pMissile);
    if (nTotalFrames >= 78)
    {
        nTotalFrames = 77;
        MISSILE_SetTotalFrames(pMissile, 77);
        MISSILE_SetCurrentFrame(pMissile, 77);
    }

    SEED_InitLowSeed(&pMissile->pSeed, nLowSeed);
    PATH_SetType(pMissile->pDynamicPath, PATHTYPE_CHARGEDBOLT);
    PATH_SetNewDistance(pMissile->pDynamicPath, nTotalFrames);
    D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
}

//D2Game.0x6FCF6C70
int32_t __fastcall SKILLS_SrvDo143_FistsOfFire_RoyalStrike_ProgressiveFn(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    int32_t nX = 0;
    int32_t nY = 0;
    if (nMissileId <= 0 || !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    int32_t nAuraRange = SKILLS_EvaluateProgressiveSkillCalc(pUnit, nSkillId, nSkillLevel);
    if (!nAuraRange)
    {
        nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    }

    sub_6FCF6D70(pGame, pUnit, ITEMS_RollRandomNumber(&pUnit->pSeed), nMissileId, nX, nY, nSkillId, nSkillLevel, nAuraRange);
    return 1;
}

//D2Game.0x6FCF6D70
void __fastcall sub_6FCF6D70(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nLowSeed, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep)
{
    D2SeedStrc seed = {};
    SEED_InitLowSeed(&seed, nLowSeed);

    D2MissileStrc missileParams = {};
    missileParams.nX = nX;
    missileParams.nY = nY;
    missileParams.pOwner = pUnit;
    missileParams.nMissile = nMissileId;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.dwFlags = 3;
    missileParams.pInitFunc = sub_6FCF7390;

    constexpr int32_t xOffsets[64] =
    {
        30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2, 0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29,
    };

    constexpr int32_t yOffsets[64] =
    {
        0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29, 30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2,
    };

    for (int32_t i = 0; i < 64; i += nStep)
    {
        missileParams.nTargetX = xOffsets[i];
        missileParams.nTargetY = yOffsets[i];
        missileParams.pInitArgs = ITEMS_RollRandomNumber(&seed);

        D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
        if (pMissile && nMissileId == MISSILE_ROYALSTRIKECHAINLIGHTNING)
        {
            D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(SKILL_ROYALSTRIKE);
            int32_t nParam = 0;
            if (pSkillsTxtRecord)
            {
                nParam = pSkillsTxtRecord->dwParam[1];
            }

            MISSILE_SetTargetX(pMissile, nParam + 1);
        }
    }
}

//D2Game.0x6FCF7390
void __fastcall sub_6FCF7390(D2UnitStrc* pMissile, int32_t nInitSeed)
{
    // TODO: Names
    constexpr int32_t dword_6FD29668[] =
    {
        2, 2, 0, -2, -2, -2, 0, 2
    };
    
    constexpr int32_t dword_6FD29688[] =
    {
        0, 2, 2, 2, 0, -2, -2, -2
    };

    constexpr int32_t dword_6FD40504 = 15;
    constexpr int32_t dword_6FD40508 = 2;

    if (!pMissile)
    {
        return;
    }

    int32_t nFrames = MISSILE_GetTotalFrames(pMissile);
    if (nFrames >= 78)
    {
        nFrames = 77;
        MISSILE_SetTotalFrames(pMissile, 77);
        MISSILE_SetCurrentFrame(pMissile, 77);
    }

    SEED_InitLowSeed(&pMissile->pSeed, nInitSeed);
    PATH_SetType(pMissile->pDynamicPath, PATHTYPE_CHARGEDBOLT);
    PATH_SetNewDistance(pMissile->pDynamicPath, nFrames);

    D2PathPointStrc* ppPathPoints = nullptr;
    PATH_GetPathPoints(pMissile->pDynamicPath, &ppPathPoints);

    int32_t nX = CLIENTS_GetUnitX(pMissile);
    int32_t nY = CLIENTS_GetUnitY(pMissile);
    int32_t nDirection = UNITS_GetDirectionToCoords(pMissile, D2COMMON_10175_PathGetFirstPointX(pMissile->pDynamicPath), D2COMMON_10176_PathGetFirstPointY(pMissile->pDynamicPath));

    int32_t nSign = 2 * ((ITEMS_RollRandomNumber(&pMissile->pSeed) & 1) != 0) - 1;
    int32_t nStep = dword_6FD40508 + ITEMS_RollRandomNumber(&pMissile->pSeed) % 3;
    for (int32_t i = 0; i < nFrames; ++i)
    {
        nDirection += nSign * nStep;
        const uint8_t nIndex = (uint8_t)D2Common_11053(nDirection & 63);
        nX += dword_6FD29668[nIndex];
        ppPathPoints[i].X = nX;
        nY += dword_6FD29688[nIndex];
        ppPathPoints[i].Y = nY;

        if (!(i % dword_6FD40504))
        {
            nSign = -nSign;
            nStep = dword_6FD40508 + ITEMS_RollRandomNumber(&pMissile->pSeed) % 3;
        }
    }

    PATH_SetNumberOfPathPoints(pMissile->pDynamicPath, nFrames);
}

//D2Game.0x6FCF7590
int32_t __fastcall SKILLS_SrvDo040_RoyalStrike_ProgressiveFn1(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nX = 0;
    int32_t nY = 0;

    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    if (nSkillLevel <= 0)
    {
        return 0;
    }

    const int32_t v7 = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (v7 <= 0)
    {
        return 0;
    }

    D2GAME_CreateMissile_6FD115E0(pGame, pUnit, nSkillId, nSkillLevel, v7, nX, nY);
    return 1;
}

//D2Game.0x6FCF7610
int32_t __fastcall SKILLS_SrvDo041_RoyalStrike_ProgressiveFn3(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SeedStrc seed = {};
    SEED_InitLowSeed(&seed, ITEMS_RollRandomNumber(&pUnit->pSeed));

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!SKILLS_GetSkillsTxtRecord(nSkillId) || !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId <= 0)
    {
        return 0;
    }

    const int32_t nSubMissiles = SKILLS_EvaluateProgressiveSkillCalc(pUnit, nSkillId, nSkillLevel);
    if (!nSubMissiles)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};

    missileParams.nX = nTargetX;
    missileParams.dwFlags = 3;
    missileParams.nY = nTargetY;
    missileParams.pOwner = pUnit;
    missileParams.nMissile = nMissileId;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    for (int32_t i = 0; i < nSubMissiles; ++i)
    {
        missileParams.nTargetX = ITEMS_RollRandomNumber(&seed) % 40 - 20;
        missileParams.nTargetY = ITEMS_RollRandomNumber(&seed) % 40 - 20;

        if (!missileParams.nTargetX && !missileParams.nTargetY)
        {
            missileParams.nTargetX = 20;
        }

        D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
        if (pMissile)
        {
            MISSILE_SetTargetX(pMissile, SEED_GetLowSeed(&seed));
            MISSILE_SetTargetY(pMissile, (uint16_t)missileParams.nTargetX + (((uint16_t)missileParams.nTargetY) << 16));
        }
    }

    return 1;
}

//D2Game.0x6FCF77E0
void __fastcall sub_6FCF77E0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pDamage || !(pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT))
    {
        return;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return;
    }

    //UNITS_IsInMovingMode(pTarget);
    if (!UNITS_IsInMeleeRange(pUnit, pTarget, 0))
    {
        return;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    for (int32_t i = 0; i < sgptDataTables->nProgressiveStates; ++i)
    {
        if (STATES_CheckState(pUnit, sgptDataTables->pProgressiveStates[i]))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, sgptDataTables->pProgressiveStates[i]);
            if (pStatList)
            {
                const int32_t nSkillId = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);

                D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
                if (pSkillsTxtRecord)
                {
                    int32_t nSkillLevel = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0);
                    const int32_t nMaxSkillLevel = SKILLS_GetSkillLevel(pUnit, SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId), 1);
                    if (nSkillLevel < nMaxSkillLevel)
                    {
                        nSkillLevel = nMaxSkillLevel;
                    }

                    if (nSkillLevel > 0 && pSkillsTxtRecord->wAuraStat[0] >= 0 && pSkillsTxtRecord->wAuraStat[0] < sgptDataTables->nItemStatCostTxtRecordCount)
                    {
                        int32_t nValue = STATLIST_GetStatValue(pStatList, pSkillsTxtRecord->wAuraStat[0], 0);
                        if (nValue < 1)
                        {
                            nValue = 1;
                        }
                        else if (nValue > 3)
                        {
                            nValue = 3;
                        }

                        const int32_t nX = ITEMS_RollRandomNumber(&pUnit->pSeed);

                        if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_PRGSTACK])
                        {
                            for (int32_t i = 0; i < nValue - 1; ++i)
                            {
                                const int32_t nPrgFunc = pSkillsTxtRecord->wSrvPrgFunc[i];
                                if (nPrgFunc > 0 && nPrgFunc < gnSkillSrvDoFns)
                                {
                                    STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxtRecord->wAuraStat[0], i + 1, 0);

                                    const SkillDoFunc pfSkillDo = gpSkillSrvDoFnTable_6FD40A20[nPrgFunc];
                                    if (pfSkillDo)
                                    {
                                        pfSkillDo(pGame, pUnit, nSkillId, nSkillLevel);
                                    }
                                }
                            }

                            STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxtRecord->wAuraStat[0], nValue, 0);
                        }

                        const int32_t nPrgFunc = pSkillsTxtRecord->wSrvPrgFunc[nValue - 1];
                        if (nPrgFunc > 0 && nPrgFunc < gnSkillSrvDoFns)
                        {
                            const SkillDoFunc pfSkillDo = gpSkillSrvDoFnTable_6FD40A20[nPrgFunc];
                            if (pfSkillDo)
                            {
                                pfSkillDo(pGame, pUnit, nSkillId, nSkillLevel);
                            }
                        }

                        sub_6FCC6300(pUnit, pTarget, nSkillId, nSkillLevel, nX, 0, nValue);
                    }
                }

                D2Common_10474(pUnit, pStatList);
                STATLIST_FreeStatList(pStatList);
            }
        }
    }

    STATES_UpdatePgsvFlags(pUnit);
}

//D2Game.0x6FCF7AD0
int32_t __fastcall SKILLS_SrvSt24_DragonTalon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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
    if (pTargetUnit)
    {
        if (UNITS_IsInMeleeRange(pUnit, pTargetUnit, 0))
        {
            const int32_t nParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel) - 1;
            SKILLS_SetParam1(pSkill, nParam);
            sub_6FCF7BC0(pGame, pUnit, pTargetUnit, pSkillsTxtRecord, nSkillId, nSkillLevel, nParam == 0);
            return 1;
        }
    }
    else
    {
        SKILLS_SetParam1(pSkill, 0);
    }

    return 0;
}

//D2Game.0x6FCF7BC0
void __fastcall sub_6FCF7BC0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2SkillsTxt* pSkillsTxtRecordArg, int32_t nSkillId, int32_t nSkillLevel, int32_t bKnockback)
{
    D2DamageStrc damage = {};

    const int32_t nToHit = STATLIST_UnitGetStatValue(pUnit, STAT_PROGRESSIVE_TOHIT, 0) + SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);
    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, nToHit, 0);

    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        if (bKnockback)
        {
            damage.wResultFlags |= (DAMAGERESULTFLAG_KNOCKBACK | DAMAGERESULTFLAG_GETHIT);
        }

        damage.dwEnDmgPct = 0;

        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (pSkillsTxtRecord && nSkillLevel > 0)
        {
            damage.dwEnDmgPct = pSkillsTxtRecord->dwParam[0] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[1];
        }

        sub_6FCF7CE0(pGame, &damage, pUnit, pTarget, nSkillId, nSkillLevel);
    }

    damage.dwHitClass = 1;

    uint8_t nSrcDam = pSkillsTxtRecordArg->nSrcDam;
    if (!nSrcDam)
    {
        nSrcDam = 0x80u;
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, nSrcDam);
}

//D2Game.0x6FCF7CE0
void __fastcall sub_6FCF7CE0(D2GameStrc* pGame, D2DamageStrc* pDamage, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel)
{
    sub_6FCF5680(pUnit, pDamage);
    pDamage->dwHitFlags |= 3u;

    const int32_t nMinBasePhysDamage = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 0);
    const int32_t nMaxBasePhysDamage = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 0);

    const int32_t nMinBaseDamage = nMinBasePhysDamage + MONSTERUNIQUE_CalculatePercentage(nMinBasePhysDamage, pDamage->dwEnDmgPct, 100);
    const int32_t nMaxBaseDamage = nMaxBasePhysDamage + MONSTERUNIQUE_CalculatePercentage(nMaxBasePhysDamage, pDamage->dwEnDmgPct, 100);

    int32_t nMinKickDamage = 0;
    int32_t nMaxKickDamage = 0;
    int32_t nDamagePercent = pDamage->dwEnDmgPct;
    SKILLS_CalculateKickDamage(pUnit, &nMinKickDamage, &nMaxKickDamage, &nDamagePercent);

    const int32_t nMinDamage = nMinBaseDamage + MONSTERUNIQUE_CalculatePercentage(nMinKickDamage << 8, nDamagePercent, 100) + (nMinKickDamage << 8);
    const int32_t nMaxDamage = nMaxBaseDamage + MONSTERUNIQUE_CalculatePercentage(nMaxKickDamage << 8, nDamagePercent, 100) + (nMaxKickDamage << 8);
    pDamage->dwPhysDamage += ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDamage - nMinDamage) + nMinDamage;

    D2GAME_RollElementalDamage_6FD14DD0(pUnit, pDamage, nSkillId, nSkillLevel);

    if (!pUnit->pInventory)
    {
        return;
    }

    D2UnitStrc* pMainWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
    D2UnitStrc* pSecondaryWeapon = nullptr;
    if (pMainWeapon)
    {
        pSecondaryWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, (ITEMS_GetBodyLocation(pMainWeapon) == BODYLOC_RARM) ? BODYLOC_LARM : BODYLOC_RARM);
    }

    if (ITEMS_CheckItemTypeId(pMainWeapon, ITEMTYPE_WEAPON) && ITEMS_CanBeEquipped(pMainWeapon) && STATLIST_GetOwner(pMainWeapon, 0))
    {
        STATLIST_MergeStatLists(pUnit, pMainWeapon, 0);
    }
    else
    {
        pMainWeapon = nullptr;
    }

    if (pSecondaryWeapon && pSecondaryWeapon != pMainWeapon && ITEMS_CheckItemTypeId(pSecondaryWeapon, ITEMTYPE_WEAPON) && ITEMS_CanBeEquipped(pSecondaryWeapon) && STATLIST_GetOwner(pSecondaryWeapon, 0))
    {
        STATLIST_MergeStatLists(pUnit, pSecondaryWeapon, 0);
    }

    SUNITDMG_FillDamageValues(pGame, pUnit, pTarget, pDamage, 1, 128);
    sub_6FCF5870(pUnit, pDamage);

    if (pMainWeapon)
    {
        STATLIST_MergeStatLists(pUnit, pMainWeapon, 1);
    }
}

//D2Game.0x6FCF8110
int32_t __fastcall SKILLS_SrvDo042_DragonTalon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    sub_6FCF77E0(pGame, pUnit, SUNITDMG_GetDamageFromUnits(pUnit, pTarget));
    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    
    const int32_t nParam = SKILLS_GetParam1(pSkill) - 1;
    if (nParam < 0)
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, nParam);

    int32_t bKnockback = 0;
    if (nParam <= 0)
    {
        int32_t nLevel = nSkillLevel + STATLIST_UnitGetStatValue(pTarget, STAT_LEVEL, 0) - STATLIST_UnitGetStatValue(pTarget, STAT_LEVEL, 0);
        if (nLevel < 1)
        {
            nLevel = 1;
        }
        else if (nLevel > nSkillLevel)
        {
            nLevel = nSkillLevel;
        }

        int32_t nChance = 100;
        if (pTarget->dwUnitType == UNIT_PLAYER || MONSTERS_GetHirelingTypeId(pTarget))
        {
            nChance = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nLevel);
        }
        else if (MONSTERS_IsBoss(nullptr, pTarget))
        {
            nChance = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nLevel);
        }
        else if (MONSTERUNIQUE_CheckMonTypeFlag(pTarget, MONTYPEFLAG_UNIQUE))
        {
            nChance = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nLevel);
        }

        if (nChance > 0 && ITEMS_RollRandomNumber(&pUnit->pSeed) % 100 < nChance)
        {
            bKnockback = 1;
        }
    }

    sub_6FCF7BC0(pGame, pUnit, pTarget, pSkillsTxtRecord, nSkillId, nSkillLevel, bKnockback);
    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);

    if (nParam > 0)
    {
        if (pUnit)
        {
            pUnit->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;
        }

        sub_6FD15080(pUnit, 100);
    }

    return 1;
}

//D2Game.0x6FCF8330
int32_t __fastcall sub_6FCF8330(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSubMissiles, int32_t nRange, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pMissile, &nTargetX, &nTargetY))
    {
        return 0;
    }

    SEED_InitLowSeed(&pMissile->pSeed, nTargetX);

    D2MissileStrc missileParams = {};

    missileParams.nSkill = nSkillId;
    missileParams.pOwner = pUnit;
    missileParams.dwFlags = 0x420u;
    missileParams.pOrigin = pMissile;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nMissile = nMissileId;

    if (nSubMissiles <= 1 || nRange < 2)
    {
        missileParams.nTargetY = nTargetY;
        missileParams.nTargetX = nTargetX;
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
        return 1;
    }

    for (int32_t i = 0; i < nSubMissiles; ++i)
    {
        missileParams.nTargetX = nTargetX + ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, 2 * nRange) - nRange;
        missileParams.nTargetY = nTargetY + ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, 2 * nRange) - nRange;

        if (PATH_ComputeSquaredDistance(CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), missileParams.nTargetX, missileParams.nTargetY) >= 4)
        {
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
        }
    }

    return 1;
}

//D2Game.0x6FCF8550
int32_t __fastcall SKILLS_SrvDo043_ShockField(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    const int32_t nSubMissiles = SKILLS_EvaluateProgressiveSkillCalc(pUnit, nSkillId, nSkillLevel);
    if (nSubMissiles <= 0)
    {
        return 0;
    }

    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    return sub_6FCF8330(pGame, pUnit, pUnit, nMissileId, nSubMissiles, nAuraRange, nSkillId, nSkillLevel);
}

//D2Game.0x6FCF8610
D2UnitStrc* __fastcall sub_6FCF8610(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, 0, 0);
    if (nSummonId < 0 || nSummonId >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        return 0;
    }

    int32_t nPetType = pSkillsTxtRecord->nPetType;
    if (nPetType < 0 || nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        nPetType = 0;
    }

    const int32_t nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);
    if (!nX || !nY)
    {
        nX = CLIENTS_GetUnitX(pUnit);
        nY = CLIENTS_GetUnitY(pUnit);
    }

    D2UnitStrc* pOwner = pUnit;
    if (pOwner)
    {
        while (pOwner->dwUnitType == UNIT_MONSTER)
        {
            pOwner = AIGENERAL_GetMinionOwner(pOwner);
            if (!pOwner)
            {
                return 0;
            }
        }
    }

    if ((!nX || !nY) && !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pOwner, &nX, &nY))
    {
        return 0;
    }

    D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pOwner), nX, nY);
    if (!pRoom || !(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_INTOWN]) && DUNGEON_IsRoomInTown(pRoom))
    {
        return 0;
    }
    
    D2SummonArgStrc summonArg = {};
    summonArg.nMonMode = nSpawnMode;
    summonArg.nPetType = nPetType;
    summonArg.nHcIdx = nSummonId;
    summonArg.pPosition.nX = nX;
    summonArg.pPosition.nY = nY;
    summonArg.dwFlags = 1;
    summonArg.pOwner = pOwner;
    summonArg.nAiSpecialState = AISPECIALSTATE_NONE;
    summonArg.nPetMax = nPetMax;

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
    if (!pPet)
    {
        return 0;
    }

    D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(pGame, pOwner, pPet, 0, nSkillLevel);
    D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pOwner, pPet, nSkillId, nSkillLevel, 0);
    sub_6FCBDD30(pPet, 2u, 1);
    
    D2ModeChangeStrc modeChange = {};
    MONSTERMODE_GetModeChangeInfo(pPet, nSpawnMode, &modeChange);
    D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
    return pPet;
}

//D2Game.0x6FCF88E0
int32_t __fastcall SKILLS_SrvDo044_BladeSentinel(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    int32_t nSpawnMode = 0;
    if (!pSkillsTxtRecord || D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, 0, 0) < 0)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    D2UnitStrc* pMonster = sub_6FCF8610(pGame, pUnit, 0, 0, pSkillsTxtRecord, nSkillId, nSkillLevel);
    if (!pMonster)
    {
        return 0;
    }

    STATLIST_SetUnitStat(pMonster, STAT_LEVEL, STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0), 0);

    const int32_t nX = CLIENTS_GetUnitX(pUnit);
    const int32_t nY = CLIENTS_GetUnitY(pUnit);
    sub_6FCBDFE0(pGame, pMonster, UNITS_GetRoom(pUnit), nX, nY, 0, 0);

    D2AiCmdStrc aiCmd = {};

    aiCmd.nCmdParam[0] = 0;
    aiCmd.nCmdParam[1] = nX;
    aiCmd.nCmdParam[2] = nY;
    aiCmd.nCmdParam[3] = nTargetX;
    aiCmd.nCmdParam[4] = nTargetY;

    AIGENERAL_CopyAiCommand(pGame, pMonster, &aiCmd);

    D2GAME_EVENTS_Delete_6FC34840(pGame, pMonster, EVENTTYPE_AITHINK, 0);
    EVENT_SetEvent(pGame, pMonster, EVENTTYPE_AITHINK, pGame->dwGameFrame + 1, 0, 0);
    pMonster->dwFlags &= 0xFFFFFFF7;

    return 1;
}

//D2Game.0x6FCF8B00
int32_t __fastcall SKILLS_SrvDo045_Sentry(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nX = 0;
    int32_t nY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    return sub_6FCF8610(pGame, pUnit, nX, nY, pSkillsTxtRecord, nSkillId, nSkillLevel) != 0;
}

//D2Game.0x6FCF8BA0
int32_t __fastcall SKILLS_SrvDo046_DragonClaw(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    const bool bSetFlag = ((pUnit->dwSeqFrame >> 8) % 2) != 0;

    sub_6FCF8C70(pGame, pUnit, pTarget, nSkillId, nSkillLevel);

    D2DamageStrc* pDamage = SUNITDMG_GetDamageFromUnits(pUnit, pTarget);
    if (pDamage)
    {
        sub_6FCF77E0(pGame, pUnit, pDamage);
    }

    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);

    if (bSetFlag)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }
    else
    {
        pUnit->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;
    }

    return 1;
}

//D2Game.0x6FCF8C70
void __fastcall sub_6FCF8C70(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return;
    }

    D2DamageStrc damage = {};

    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, STATLIST_UnitGetStatValue(pUnit, 325, 0) + SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.dwHitFlags |= 2;
        sub_6FCF5680(pUnit, &damage);
        damage.dwEnDmgPct += SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
        if (pSkillsTxtRecord->nEType)
        {
            damage.dwConvPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
            if (damage.dwConvPct > 0)
            {
                damage.nConvType = pSkillsTxtRecord->nEType;
            }

            D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
        }

        SUNITDMG_FillDamageValues(pGame, pUnit, pTarget, &damage, 0, pSkillsTxtRecord->nSrcDam);
        sub_6FCF5870(pUnit, &damage);
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
}

//D2Game.0x6FCF8DD0
void __fastcall sub_6FCF8DD0(D2UnitStrc* pDefender)
{
    if (!STATES_CheckStateMaskRemHitOnUnit(pDefender))
    {
        return;
    }

    const int32_t nRecords = (sgptDataTables->nStatesTxtRecordCount + 31) / 32;

    uint32_t nFlags[8] = {};
    memcpy(nFlags, D2COMMON_10494_STATES_GetStatFlags(pDefender), sizeof(uint32_t) * nRecords);

    for (int32_t i = 0; i < nRecords; ++i)
    {
        int32_t nFlag = nFlags[i];

        for (int32_t nBit = FOG_LeadingZeroesCount(nFlag); nBit >= 0; nBit = FOG_LeadingZeroesCount(nFlag))
        {
            const int32_t nStateId = nBit + 32 * i;

            nFlag &= ~(1 << nBit);

            if (sgptDataTables->fStateMasks[6][nStateId >> 5] & gdwBitMasks[nStateId & 31])
            {
                D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pDefender, nStateId);
                if (pStatList)
                {
                    D2Common_10474(pDefender, pStatList);
                    STATLIST_FreeStatList(pStatList);
                }

                STATES_ToggleState(pDefender, nStateId, 0);
            }
        }

        nFlags[i] = nFlag;
    }
}

//D2Game.0x6FCF8ED0
int32_t __fastcall SKILLS_SrvDo047_CloakOfShadows(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount || STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
    {
        return 0;
    }

    D2CurseStrc curse = {};

    curse.pUnit = pUnit;
    curse.pTarget = pUnit;
    curse.nSkill = nSkillId;
    curse.nSkillLevel = nSkillLevel;
    curse.nDuration = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    curse.nStat = pSkillsTxtRecord->nPassiveStat[0];
    curse.nStatValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[0], nSkillId, nSkillLevel);
    curse.nState = pSkillsTxtRecord->nAuraState;
    curse.pStateRemoveCallback = sub_6FD10E50;

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    if (!pStatList)
    {
        return 0;
    }

    for (int32_t i = 1; i < 5; ++i)
    {
        if (pSkillsTxtRecord->nPassiveStat[i] >= 0)
        {
            const int32_t nValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[i], nSkillId, nSkillLevel);
            if (nValue)
            {
                STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxtRecord->nPassiveStat[i], nValue, 0);
            }
        }
    }

    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);
    STATES_ToggleGfxStateFlag(pUnit, pSkillsTxtRecord->nAuraState, 1);

    if (pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    D2SrvDo47Strc args = {};

    args.nSkillId = nSkillId;
    args.nSkillLevel = nSkillLevel;
    args.nDuration = curse.nDuration;
    args.nAuraTargetState = pSkillsTxtRecord->wAuraTargetState;
    
    for (int32_t i = 0; i < 6; ++i)
    {
        args.nAuraStat[i] = pSkillsTxtRecord->wAuraStat[i];
        args.nAuraStatCalcValue[i] = 0;
        if (pSkillsTxtRecord->wAuraStat[i] >= 0)
        {
            args.nAuraStatCalcValue[i] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], nSkillId, nSkillLevel);
        }
    }

    sub_6FD0FE80(pGame, pUnit, 0, 0, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel), pSkillsTxtRecord->dwAuraFilter, SKILLS_AuraCallback_CloakOfShadows, &args, 1, __FILE__, __LINE__);

    return 1;
}

//D2Game.0x6FCF9160
int32_t __fastcall SKILLS_AuraCallback_CloakOfShadows(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2SrvDo47Strc* pArgs = (D2SrvDo47Strc*)pAuraCallback->pArgs;
    if (SUNIT_IsDead(pUnit))
    {
        return 0;
    }

    D2CurseStrc curse = {};

    curse.pTarget = pUnit;
    curse.pUnit = pAuraCallback->pOwner;
    curse.nSkill = pArgs->nSkillId;
    curse.nSkillLevel = pArgs->nSkillLevel;
    curse.nDuration = pArgs->nDuration;
    curse.nStat = pArgs->nAuraStat[0];
    if (curse.nStat <= 0)
    {
        curse.nStat = -1;
    }
    curse.nStatValue = pArgs->nAuraStatCalcValue[0];
    curse.nState = pArgs->nAuraTargetState;
    curse.pStateRemoveCallback = sub_6FD0B250;

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    if (!pStatList)
    {
        return 0;
    }

    for (int32_t i = 1; i < 6; ++i)
    {
        if (pArgs->nAuraStat[i] >= 0)
        {
            STATLIST_SetStatIfListIsValid(pStatList, pArgs->nAuraStat[i], pArgs->nAuraStatCalcValue[i], 0);
        }
    }

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && sub_6FD15190(pUnit, AISPECIALSTATE_DIMVISION))
    {
        AITHINK_ExecuteAiFn(pAuraCallback->pGame, pUnit, AIGENERAL_GetAiControlFromUnit(pUnit), AISPECIALSTATE_DIMVISION);
    }

    return 1;
}

//D2Game.0x6FCF9260
int32_t __fastcall SKILLS_SrvDo048_BladeFury(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId < 0)
    {
        return 0;
    }

    const int32_t nDuration = SKILLS_EvaluateProgressiveSkillCalc(pUnit, nSkillId, nSkillLevel) - 1;
    if (nDuration <= 0)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags &= 0xFFFFFFBF;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    int32_t bMissileCreated = 0;
    if (SKILLS_GetParam1(pSkill) < pGame->dwGameFrame)
    {
        D2MissileStrc missileParams = {};
        missileParams.nSkill = nSkillId;
        missileParams.nMissile = nMissileId;
        missileParams.nSkillLevel = nSkillLevel;
        missileParams.nTargetX = nTargetX;
        missileParams.nTargetY = nTargetY;
        missileParams.dwFlags = 32;
        missileParams.pOwner = pUnit;
        missileParams.pOrigin = pUnit;
        MISSILES_CreateMissileFromParams(pGame, &missileParams);

        bMissileCreated = 1;

        SKILLS_SetParam1(pSkill, nDuration + pGame->dwGameFrame);
    }

    if (STATES_CheckState(pUnit, STATE_INFERNO))
    {
        sub_6FCBD120(pGame, pUnit, 1);
    }

    return bMissileCreated;
}

//D2Game.0x6FCF93B0
int32_t __fastcall SKILLS_SrvSt26_BladeFury(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId) < 0)
    {
        return 0;
    }

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        return 0;
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_INFERNO);
    if (pStatList)
    {
        D2COMMON_10476(pStatList, pGame->dwGameFrame + 7);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_REMOVESTATE, pGame->dwGameFrame + 7, 0, 0);

        if (SKILLS_GetParam1(pSkill) <= pGame->dwGameFrame)
        {
            const int32_t nResult = SKILLS_SrvDo048_BladeFury(pGame, pUnit, nSkillId, nSkillLevel);
            if (nResult)
            {
                D2GAME_SKILLMANA_Consume_6FD10A50(pGame, pUnit, nSkillId, nSkillLevel);
            }

            return nResult;
        }

        sub_6FCBD120(pGame, pUnit, 1);
        return 1;
    }

    if (pSkillsTxtRecord->wStartMana > 0 && STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0) < (int32_t)pSkillsTxtRecord->wStartMana << 8)
    {
        return 0;
    }

    int32_t nUnitGUID = -1;
    int32_t nUnitType = 6;
    if (pUnit)
    {
        nUnitGUID = pUnit->dwUnitId;
        nUnitType = pUnit->dwUnitType;
    }

    pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, pGame->dwGameFrame + 21, nUnitType, nUnitGUID);
    if (!pStatList)
    {
        return 0;
    }

    EVENT_SetEvent(pGame, pUnit, EVENTTYPE_REMOVESTATE, pGame->dwGameFrame + 21, 0, 0);
    D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
    STATLIST_SetStatRemoveCallback(pStatList, SKILLS_StatRemoveCallback_RemoveState);
    STATLIST_SetState(pStatList, STATE_INFERNO);
    STATES_ToggleState(pUnit, STATE_INFERNO, 1);
    SKILLS_SetParam1(pSkill, 0);

    return 1;
}

//D2Game.0x6FCF9550
void __fastcall SKILLS_StatRemoveCallback_RemoveState(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    D2_MAYBE_UNUSED(pStatList);
    STATES_ToggleState(pUnit, nState, 0);

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }
}

//D2Game.0x6FCF9580
void __fastcall sub_6FCF9580(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nSkillId, int32_t nSkillLevel, int32_t nItemLevel, int32_t bCheckOnInit)
{
    // TODO: v28
    if (!pPet || pPet->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pPet->dwClassId);
    if (!pMonStatsTxtRecord || pMonStatsTxtRecord->nMonEquipTxtRecordId < 0)
    {
        return;
    }

    D2MonEquipTxt* pMonEquipTxtRecord = &sgptDataTables->pMonEquipTxt[pMonStatsTxtRecord->nMonEquipTxtRecordId];
    if (pMonEquipTxtRecord && (!bCheckOnInit || pMonEquipTxtRecord->nOnInit) && pPet->pInventory)
    {
        while (pMonEquipTxtRecord->nMonster == pPet->dwClassId && pMonEquipTxtRecord->wLevel > nSkillLevel)
        {
            ++pMonEquipTxtRecord;
        }

        while (pMonEquipTxtRecord->nMonster == pPet->dwClassId)
        {
            int32_t nMax = 0;
            while (nMax < 3 && pMonEquipTxtRecord->nLoc[nMax] > 0 && pMonEquipTxtRecord->nLoc[nMax] < 11)
            {
                ++nMax;
            }

            const uint32_t nIndex = ITEMS_RollLimitedRandomNumber(&pPet->pSeed, nMax);
            const uint8_t nBodyLoc = pMonEquipTxtRecord->nLoc[nIndex];
            if (!INVENTORY_GetItemFromBodyLoc(pPet->pInventory, nBodyLoc))
            {
                int32_t nMod = pMonEquipTxtRecord->nMod[nIndex];
                if (nMod < 0 || nMod >= 8)
                {
                    nMod = 0;
                }

                int32_t nItemCode = pMonEquipTxtRecord->dwItem[nIndex];
                if (nItemCode != '    ' || !pUnit)
                {
                    Unk v28 = {};
                    v28.nItemCode = nItemCode;
                    v28.nBodyLoc = nBodyLoc;
                    v28.unk0x05 = 1;
                    sub_6FC61190(pGame, pPet, &v28, nItemLevel, nMod);
                }
                else if (pUnit->pInventory)
                {
                    D2UnitStrc* pItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
                    if (pItem)
                    {
                        Unk v28 = {};
                        v28.nItemCode = ITEMS_GetBaseCode(pItem);
                        v28.nBodyLoc = nBodyLoc;
                        v28.unk0x05 = 1;
                        sub_6FC61190(pGame, pPet, &v28, nItemLevel, nMod);
                    }
                }
            }

            ++pMonEquipTxtRecord;
        }
    }
}

//D2Game.0x6FCF9750
int32_t __fastcall SKILLS_SrvDo049_ShadowWarrior_Master(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, 0, 0);
    if (nSummonId < 0)
    {
        return 0;
    }

    int32_t nPetType = pSkillsTxtRecord->nPetType;
    if (nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        nPetType = 0;
    }

    D2SummonArgStrc summonArg = {};
    summonArg.nHcIdx = nSummonId;
    summonArg.pOwner = pUnit;
    summonArg.nAiSpecialState = AISPECIALSTATE_NONE;
    summonArg.nMonMode = nSpawnMode;
    summonArg.nPetType = nPetType;
    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
    if (!pPet)
    {
        return 0;
    }

    STATLIST_SetUnitStat(pPet, STAT_LEVEL, STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0), 0);

    if (nSkillLevel > 1)
    {
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pPet);
        const int32_t nHitpoints = MONSTERUNIQUE_CalculatePercentage(nMaxHp, pSkillsTxtRecord->dwParam[0] * (nSkillLevel - 1), 100) + nMaxHp;
        STATLIST_SetUnitStat(pPet, STAT_MAXHP, nHitpoints, 0);
        STATLIST_SetUnitStat(pPet, STAT_HITPOINTS, nHitpoints, 0);

        D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 0, 0, pPet->dwUnitType, pPet->dwUnitId);
        if (pStatList)
        {
            D2COMMON_10475_PostStatToStatList(pPet, pStatList, 1);

            for (int32_t i = 0; i < 6; ++i)
            {
                if (pSkillsTxtRecord->wAuraStat[i] >= 0 && pSkillsTxtRecord->wAuraStat[i] < sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxtRecord->wAuraStat[i], SKILLS_EvaluateSkillFormula(pPet, pSkillsTxtRecord->dwAuraStatCalc[1], nSkillId, nSkillLevel), 0);
                }
            }

            for (int32_t i = 0; i < 5; ++i)
            {
                if (pSkillsTxtRecord->nPassiveStat[i] >= 0 && pSkillsTxtRecord->nPassiveStat[i] < sgptDataTables->nItemStatCostTxtRecordCount)
                {
                    STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxtRecord->nPassiveStat[i], SKILLS_EvaluateSkillFormula(pPet, pSkillsTxtRecord->dwPassiveCalc[1], nSkillId, nSkillLevel), 0);
                }
            }

            if (pSkillsTxtRecord->wSumUMod > 0 && pSkillsTxtRecord->wSumUMod < 43)
            {
                D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pPet, pSkillsTxtRecord->wSumUMod, 1);
            }
        }
    }

    int32_t nItemLevel = 0;
    if (nSkillLevel > 0)
    {
        nItemLevel = pSkillsTxtRecord->dwParam[4] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[5];
    }


    const int32_t nMaxLevel = DATATBLS_GetMaxLevel(0);
    if (nItemLevel < 1)
    {
        nItemLevel = 1;
    }
    else if (nItemLevel > nMaxLevel)
    {
        nItemLevel = nMaxLevel;
    }

    sub_6FCF9580(pGame, pUnit, pPet, nSkillId, nSkillLevel, nItemLevel, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pPet, EVENTTYPE_AITHINK, 0);
    EVENT_SetEvent(pGame, pPet, EVENTTYPE_AITHINK, pGame->dwGameFrame + 20, 0, 0);

    if (pSkillsTxtRecord->nAuraState > 0 && pSkillsTxtRecord->nAuraState < sgptDataTables->nStatesTxtRecordCount)
    {
        STATES_ToggleState(pPet, pSkillsTxtRecord->nAuraState, 1);
    }

    UNITS_StoreOwner(pPet, pUnit);

    const int32_t nLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    if (nLength > 0)
    {
        EVENT_SetEvent(pGame, pPet, EVENTTYPE_MONUMOD, pGame->dwGameFrame + nLength, 0, 0);
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pPet, MONUMOD_TEMPSUMMON, 0);
    }

    D2GAME_UpdateSummonAI_6FC401F0(pGame, pPet, 0, pUnit->dwNodeIndex);

    return 1;
}

//D2Game.0x6FCF9B50
int32_t __fastcall SKILLS_SrvSt27_DragonTail(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    sub_6FD15470(pUnit, pSkillsTxtRecord->dwParam[3]);

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    const int32_t nToHit = STATLIST_UnitGetStatValue(pUnit, STAT_PROGRESSIVE_TOHIT, 0) + SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);

    D2DamageStrc damage = {};
    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, nToHit, 0);
    if (!(damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT))
    {
        return 0;
    }

    sub_6FCF7CE0(pGame, &damage, pUnit, pTarget, nSkillId, nSkillLevel);

    uint8_t nSrcDam = pSkillsTxtRecord->nSrcDam;
    if (!pSkillsTxtRecord->nSrcDam)
    {
        nSrcDam = 0x80u;
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, nSrcDam);
    return 1;
}

//D2Game.0x6FCF9C70
int32_t __fastcall SKILLS_SrvDo050_DragonTail(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    D2DamageStrc* pDamage = SUNITDMG_GetDamageFromUnits(pUnit, pTarget);
    if (!pDamage)
    {
        return 0;
    }

    D2DamageStrc physDamage = {};
    memcpy(&physDamage, pDamage, sizeof(physDamage));

    sub_6FCF77E0(pGame, pUnit, pDamage);
    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);

    if (SUNIT_IsDead(pUnit))
    {
        return 1;
    }

    D2DamageStrc fireDamage = {};

    const int32_t nPercentage = STATLIST_UnitGetStatValue(pUnit, STAT_PASSIVE_FIRE_MASTERY, 0) + SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

    fireDamage.dwFireDamage = MONSTERUNIQUE_CalculatePercentage(physDamage.dwPhysDamage, nPercentage, 100);
    fireDamage.wResultFlags = 9;
    sub_6FD10200(pGame, pUnit, CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget), SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel), &fireDamage, 0);

    return 1;
}

//D2Game.0x6FCF9EA0
void __fastcall SKILLS_StatRemoveCallback_MindBlast(D2UnitStrc* pItem, int32_t nState, D2StatListStrc* pStatList)
{
    D2_MAYBE_UNUSED(pStatList);
    sub_6FCBDD30(pItem, 0, 1);
    STATES_ToggleState(pItem, nState, 0);
    D2GAME_TARGETS_Last_6FC40380(SUNIT_GetGameFromUnit(pItem), pItem);

    D2StatListStrc* pConversionStatList = STATLIST_GetStatListFromUnitAndState(pItem, STATE_CONVERSION_SAVE);
    if (pConversionStatList)
    {
        const int32_t nLevel = STATLIST_GetStatValue(pConversionStatList, STAT_CONVERSION_LEVEL, 0);
        const int32_t nConversionMaxHp = STATLIST_GetStatValue(pConversionStatList, STAT_CONVERSION_MAXHP, 0);
        int32_t nNewHp = STATLIST_GetMaxLifeFromUnit(pItem) >> 8;
        const int32_t nHitpoints = STATLIST_UnitGetStatValue(pItem, STAT_HITPOINTS, 0) >> 8;
        
        int32_t nMaxHp = 1;
        if (nNewHp)
        {
            nMaxHp = MONSTERUNIQUE_CalculatePercentage(nConversionMaxHp, nHitpoints, nNewHp);
            if (nMaxHp < 1)
            {
                nMaxHp = 1;
            }
        }

        if (nNewHp > nMaxHp)
        {
            nNewHp = nMaxHp;
        }

        STATLIST_SetUnitStat(pItem, STAT_LEVEL, nLevel, 0);
        STATLIST_SetUnitStat(pItem, STAT_HITPOINTS, nNewHp << 8, 0);
        STATLIST_SetUnitStat(pItem, STAT_MAXHP, nConversionMaxHp << 8, 0);
        D2Common_10474(pItem, pConversionStatList);
        STATLIST_FreeStatList(pConversionStatList);
    }

    for (D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndFlag(pItem, 8); pStatList; pStatList = STATLIST_GetStatListFromUnitAndFlag(pItem, 8))
    {
        D2Common_10474(pItem, pStatList);
        STATLIST_FreeStatList(pStatList);
    }
}

//D2Game.0x6FCF9FF0
int32_t __fastcall SKILLS_AuraCallback_MindBlast(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2SrvDo51Strc* pArgs = (D2SrvDo51Strc*)pAuraCallback->pArgs;

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && !MONSTERS_GetHirelingTypeId(pUnit) && STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_EVIL && sub_6FD15190(pUnit, AISPECIALSTATE_TERROR) && ITEMS_RollLimitedRandomNumber(&pAuraCallback->pOwner->pSeed, 100) <= pArgs->nMonCurseResSubstraction)
    {
        const int32_t nExpireFrame = pAuraCallback->pGame->dwGameFrame + pArgs->nParam1 + ITEMS_RollLimitedRandomNumber(&pAuraCallback->pOwner->pSeed, pArgs->nParam2);
        D2StatListStrc* pConversionStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_CONVERSION);
        if (!pConversionStatList)
        {
            int32_t nUnitGUID = -1;
            int32_t nUnitType = 6;
            if (pAuraCallback->pOwner)
            {
                nUnitGUID = pAuraCallback->pOwner->dwUnitId;
                nUnitType = pAuraCallback->pOwner->dwUnitType;
            }

            pConversionStatList = STATLIST_AllocStatList(pAuraCallback->pGame->pMemoryPool, 0x802u, nExpireFrame, nUnitType, nUnitGUID);
            if (pConversionStatList)
            {
                STATLIST_SetState(pConversionStatList, STATE_CONVERSION);
                STATLIST_SetStatRemoveCallback(pConversionStatList, SKILLS_StatRemoveCallback_MindBlast);
                D2COMMON_10475_PostStatToStatList(pUnit, pConversionStatList, 1);
                STATES_ToggleState(pUnit, STATE_CONVERSION, STATE_FREEZE);
            }
        }

        if (pConversionStatList)
        {
            D2COMMON_10476(pConversionStatList, nExpireFrame);
            EVENT_SetEvent(pAuraCallback->pGame, pUnit, EVENTTYPE_REMOVESTATE, nExpireFrame, 0, 0);
            sub_6FD154D0(pUnit);
            sub_6FCBDD30(pUnit, 2u, 1);
            D2GAME_UpdateSummonAI_6FC401F0(pAuraCallback->pGame, pUnit, 0, pAuraCallback->pOwner->dwNodeIndex);

            for (D2StatListStrc* i = STATLIST_GetStatListFromUnitAndFlag(pUnit, 8); i; i = STATLIST_GetStatListFromUnitAndFlag(pUnit, 8))
            {
                D2Common_10474(pUnit, i);
                STATLIST_FreeStatList(i);
            }

            UNITS_SetTargetUnitForDynamicUnit(pAuraCallback->pOwner, nullptr);

            const int32_t nLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
            const int32_t nOwnerLevel = STATLIST_UnitGetStatValue(pAuraCallback->pOwner, STAT_LEVEL, 0);
            if (!nLevel || nOwnerLevel >= nLevel || STATLIST_GetStatListFromUnitAndState(pUnit, STATE_CONVERSION_SAVE))
            {
                return 1;
            }
            
            D2StatListStrc* pConversionSaveStatList = STATLIST_AllocStatList(pAuraCallback->pGame->pMemoryPool, 0, 0, pUnit->dwUnitType, pUnit->dwUnitId);
            if (!pConversionSaveStatList)
            {
                return 1;
            }

            STATLIST_SetState(pConversionSaveStatList, STATE_CONVERSION_SAVE);
            D2COMMON_10475_PostStatToStatList(pUnit, pConversionSaveStatList, 1);
            const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) >> 8;
            const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit) >> 8;
            STATLIST_SetStatIfListIsValid(pConversionSaveStatList, STAT_CONVERSION_LEVEL, nLevel, 0);
            STATLIST_SetStatIfListIsValid(pConversionSaveStatList, STAT_CONVERSION_MAXHP, nMaxHp, 0);

            int32_t nNewHitpoints = MONSTERUNIQUE_CalculatePercentage(nHitpoints, nOwnerLevel, nLevel) << 8;
            int32_t nNewMaxHp = MONSTERUNIQUE_CalculatePercentage(nMaxHp, nOwnerLevel, nLevel) << 8;
            if (nNewMaxHp < 1)
            {
                nNewMaxHp = 1;
            }

            if (nNewHitpoints < 1)
            {
                nNewHitpoints = 1;
            }
            else if (nNewHitpoints > nNewMaxHp)
            {
                nNewHitpoints = nNewMaxHp;
            }

            STATLIST_SetUnitStat(pUnit, STAT_LEVEL, nOwnerLevel, 0);
            STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHitpoints, 0);
            STATLIST_SetUnitStat(pUnit, STAT_MAXHP, nNewMaxHp, 0);
            return 1;
        }
    }

    D2DamageStrc damage = {};
    memcpy(&damage, pArgs->pDamage, sizeof(damage));
    SUNITDMG_ExecuteEvents(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, 1, &damage);
    SUNITDMG_ExecuteMissileDamage(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, &damage);
    return 1;
}

//D2Game.0x6FCFA350
int32_t __fastcall SKILLS_SrvDo051_MindBlast(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nX = 0;
    int32_t nY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    int32_t nRange = SKILLS_EvaluateProgressiveSkillCalc(pUnit, nSkillId, nSkillLevel);
    if (!nRange)
    {
        nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    }

    D2DamageStrc damage = {};
    D2GAME_RollPhysicalDamage_6FD14EC0(pUnit, &damage, nSkillId, nSkillLevel);
    D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
    damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
    damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;

    D2SrvDo51Strc args = {};
    args.pDamage = &damage;
    args.nMonCurseResSubstraction = D2COMMON_11036_GetMonCurseResistanceSubtraction(nSkillLevel, nSkillId);
    args.nParam1 = pSkillsTxtRecord->dwParam[2];
    args.nParam2 = pSkillsTxtRecord->dwParam[3];


    sub_6FD0FE80(pGame, pUnit, nX, nY, nRange, pSkillsTxtRecord->dwAuraFilter, SKILLS_AuraCallback_MindBlast, &args, 0, __FILE__, __LINE__);
    return 1;
}

//D2Game.0x6FCFA4F0
int32_t __fastcall SKILLS_SrvDo052_DragonFlight(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;

    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    if (((pUnit->dwSeqFrame >> 8) % 2) != 0)
    {
        D2DamageStrc damage = {};

        const int32_t nToHit = STATLIST_UnitGetStatValue(pUnit, STAT_PROGRESSIVE_TOHIT, 0) + SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);
        damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, nToHit, 0);
        if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
        {
            if (nSkillLevel > 0)
            {
                damage.dwEnDmgPct = pSkillsTxtRecord->dwParam[0] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[1];
            }
            else
            {
                damage.dwEnDmgPct = 0;
            }

            sub_6FCF7CE0(pGame, &damage, pUnit, pTarget, nSkillId, nSkillLevel);
        }

        uint8_t nSrcDam = pSkillsTxtRecord->nSrcDam;
        if (!nSrcDam)
        {
            nSrcDam = 0x80u;
        }

        SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, nSrcDam);
        sub_6FCF77E0(pGame, pUnit, SUNITDMG_GetDamageFromUnits(pUnit, pTarget));
        SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
        return 1;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return 0;
    }

    pUnit->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;

    int32_t nX = 0;
    int32_t nY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(DUNGEON_GetLevelIdFromRoom(pRoom));
    if (!pLevelsTxtRecord || pLevelsTxtRecord->nTeleport == 0 || (pLevelsTxtRecord->nTeleport == 2 && UNITS_TestCollisionByCoordinates(pUnit, nX, nY, 0x804)))
    {
        return 0;
    }

    sub_6FCBDFE0(pGame, pUnit, 0, nX, nY, 0, 0);
    return 1;
}

//D2Game.0x6FCFA720
int32_t __fastcall SKILLS_SrvSt28_BladeShield(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId) || SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId) <= 0 || SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel) <= 0)
    {
        return 0;
    }

    if (pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    sub_6FD11C90(pUnit, pSkillsTxtRecord->nAuraState, 0);

    D2CurseStrc curse = {};
    curse.pUnit = pUnit;
    curse.pTarget = pUnit;
    curse.nSkill = nSkillId;
    curse.nSkillLevel = nSkillLevel;
    curse.nStat = -1;
    curse.nDuration = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    curse.nState = pSkillsTxtRecord->nAuraState;

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    if (!pStatList)
    {
        return 0;
    }

    sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);
    STATES_ToggleGfxStateFlag(pUnit, pSkillsTxtRecord->nAuraState, 1);
    return 1;
}

//D2Game.0x6FCFA880
int32_t __fastcall SKILLS_SrvDo053_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
	if (!pSkillsTxtRecord)
	{
		return 0;
	}

	int32_t nAuraRange = SKILLS_EvaluateProgressiveSkillCalc(pUnit, nSkillId, nSkillLevel);
    if (!nAuraRange)
    {
        nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    }

    D2DamageStrc damage = {};
    damage.dwHitFlags = 2;
		
    D2GAME_RollPhysicalDamage_6FD14EC0(pUnit, &damage, nSkillId, nSkillLevel);
    D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);

    if (pSkillsTxtRecord->dwHitClass)
    {
        damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
    }

    sub_6FD0FE80(pGame, pUnit, 0, 0, nAuraRange, pSkillsTxtRecord->dwAuraFilter, sub_6FCF5DE0, &damage, 0, __FILE__, __LINE__);
	return 1;
}

//D2Game.0x6FCFA980
int32_t __fastcall SKILLS_AuraCallback_SrvDo142(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2SrvDo142Strc* pArgs = (D2SrvDo142Strc*)pAuraCallback->pArgs;
    D2DamageStrc damage = {};
    memcpy(&damage, pArgs->pDamage, sizeof(damage));

    sub_6FD15650(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, pArgs->nResultFlags, pArgs->nToHit, &damage, 1);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.dwHitFlags |= pArgs->nHitFlags;
        if (pArgs->nSrcDam)
        {
            SUNITDMG_FillDamageValues(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, &damage, 0, pArgs->nSrcDam);
        }

        SUNITDMG_AllocCombat(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, &damage, 128);
        SUNITDMG_DrainItemDurability(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, 0);
    }

    return 1;
}

//D2Game.0x6FCFAA20
int32_t __fastcall SKILLS_SrvDo142_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nRange = SKILLS_EvaluateProgressiveSkillCalc(pUnit, nSkillId, nSkillLevel);
    if (!nRange)
    {
        nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    }

    D2DamageStrc damage = {};
    damage.dwHitFlags = 2;
    D2GAME_RollPhysicalDamage_6FD14EC0(pUnit, &damage, nSkillId, nSkillLevel);
    D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);

    if (pSkillsTxtRecord->dwHitClass)
    {
        damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
    }

    D2SrvDo142Strc args = {};
    args.pDamage = &damage;
    args.nResultFlags = pSkillsTxtRecord->wResultFlags;
    args.nHitFlags = pSkillsTxtRecord->dwHitFlags;
    args.nSrcDam = pSkillsTxtRecord->nSrcDam;
    if (!(pSkillsTxtRecord->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT))
    {
        args.nToHit = SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);
    }

    sub_6FD0FE80(pGame, pUnit, 0, 0, nRange, pSkillsTxtRecord->dwAuraFilter, SKILLS_AuraCallback_SrvDo142, &args, 0, __FILE__, __LINE__);
    return 1;
}

//D2Game.0x6FCFAB60
int32_t __fastcall SKILLS_SrvDo054_BladeShield(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount || !SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId))
    {
        return 0;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!DUNGEON_IsRoomInTown(pRoom))
    {
        SKILLS_SrvDo142_Unused(pGame, pUnit, nSkillId, nSkillLevel);
    }

    return 1;
}
