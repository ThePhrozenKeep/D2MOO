#include "SKILLS/SkillAma.h"

#include "D2DataTbls.h"
#include "DataTbls/SkillsTbls.h"
#include "D2Skills.h"
#include "D2Items.h"
#include "D2StatList.h"
#include "D2States.h"

#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "GAME/Targets.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterUnique.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlrModes.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillNec.h"
#include "SKILLS/Skills.h"
#include "SKILLS/SkillSor.h"
#include "SKILLS/Skills.h"


#pragma pack(push, 1)
struct D2SrvDo06Strc
{
    int32_t nSkillId;
    int32_t nSkillLevel;
    int32_t nDuration;
    int32_t nAuraTargetState;
    int32_t nAuraStatId;
    int32_t nAuraStatCalcValue;
};
#pragma pack(pop)


// Same as #10049
int32_t __stdcall D2Game_10048_Return1(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    return 1;
}

//D2Game.0x6FCF30D0
int32_t __stdcall D2Game_10049_Return1(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    return 1;
}

//D2Game.0x6FCF30E0
int32_t __fastcall SKILLS_SrvSt04_Arrow_Bolt(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (sub_6FD119C0(pUnit))
    {
        return 1;
    }

    sub_6FC80A30(pGame, pUnit);
    sub_6FC80E10(pGame, pUnit);
    return 0;
}

//D2Game.0x6FCF3120
int32_t __fastcall SKILLS_SrvSt05_Jab(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        return SUNIT_GetTargetUnit(pGame, pUnit) != nullptr;
    }

    return 0;
}

//D2Game.0x6FCF3170
int32_t __fastcall SKILLS_SrvSt06_PowerStrike_ChargedStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2DamageStrc damage = {};

    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, 0, 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
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
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, pSkillsTxtRecord->nSrcDam);
    return 1;
}

//D2Game.0x6FCF3280
int32_t __fastcall SKILLS_SrvSt07_Impale(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    if (!sub_6FCBD900(pGame, pUnit, pTarget))
    {
        return 0;
    }

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

        damage.dwHitFlags = DAMAGEHITFLAG_1;

        D2UnitStrc* pItem = sub_6FC7C7B0(pUnit);
        if (pItem && ITEMS_HasDurability(pItem))
        {
            const int32_t nChance = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
            const int32_t nAmount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
            if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
            {
                if (ITEMS_CheckIfStackable(pItem))
                {
                    if (STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0) > 0 && (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < nChance)
                    {
                        sub_6FD118C0(pGame, pUnit);
                    }
                }
                else if (ITEMS_HasDurability(pItem) && (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < nChance)
                {
                    const int32_t nNewDurability = STATLIST_GetUnitStatUnsigned(pItem, STAT_DURABILITY, 0) - nAmount;
                    if (nNewDurability <= 0)
                    {
                        sub_6FC4B580(pGame, pUnit, pItem);
                    }
                    else
                    {
                        STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nNewDurability, 0);
                        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_DURABILITY, nNewDurability, 0);
                    }
                }
            }
        }
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
    return 1;
}

//D2Game.0x6FCF3510
int32_t __fastcall SKILLS_SrvSt08_Strafe(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: v11, v12, v13
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

    if (!sub_6FD119C0(pUnit))
    {
        sub_6FC80A30(pGame, pUnit);
        sub_6FC80E10(pGame, pUnit);
        return 0;
    }

    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);

    int32_t v13 = 0;
    if (pTarget)
    {
        v13 = sub_6FD10790(pGame, pUnit, 0, 0, nAuraRange, 3);
    }
    else
    {
        pTarget = sub_6FD107F0(pGame, pUnit, 0, 0, nAuraRange, 3, -1, &v13);
    }

    int32_t v12 = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
    int32_t v11 = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (v12 > v11)
    {
        v12 = v11;
    }
    if (v13 > v12)
    {
        v12 = v13;
    }
    if (v12 < v11)
    {
        v11 = v12;
    }
    SKILLS_SetParam1(pSkill, v11);

    if (pTarget)
    {
        SKILLS_SetParam2(pSkill, pTarget->dwUnitType);
        SKILLS_SetParam3(pSkill, pTarget->dwUnitId);
    }
    else
    {
        SKILLS_SetParam2(pSkill, UNIT_MONSTER);
        SKILLS_SetParam3(pSkill, -1);
    }

    sub_6FD118C0(pGame, pUnit);
    return 1;
}

//D2Game.0x6FCF3690
int32_t __fastcall SKILLS_SrvSt09_Fend(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: v10, v11
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

    const int32_t nAuraRange = sub_6FD15460(pUnit);
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);

    int32_t v11 = 0;
    if (pTarget)
    {
        v11 = sub_6FD10790(pGame, pUnit, 0, 0, nAuraRange, 0x20003);
    }
    else
    {
        pTarget = sub_6FD107F0(pGame, pUnit, 0, 0, nAuraRange, 0x20003, -1, &v11);
        if (!pTarget)
        {
            SKILLS_SetParam1(pSkill, 0);
            return 1;
        }
    }

    const int32_t v10 = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (v11 >= v10)
    {
        v11 = v10;
    }

    SKILLS_SetParam1(pSkill, v11);
    SKILLS_SetParam2(pSkill, pTarget->dwUnitType);
    SKILLS_SetParam3(pSkill, pTarget->dwUnitId);
    return 1;
}

//D2Game.0x6FCF37B0
int32_t __fastcall SKILLS_SrvSt37_Zeal_Fury_BloodLordFrenzy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    const int32_t nAuraRange = sub_6FD15460(pUnit);
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        pTarget = sub_6FD107F0(pGame, pUnit, 0, 0, nAuraRange, 0x20003, -1, 0);
    }

    if (!pTarget)
    {
        SKILLS_SetParam1(pSkill, 0);
        return 0;
    }

    SKILLS_SetParam1(pSkill, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel));
    SKILLS_SetParam2(pSkill, pTarget->dwUnitType);
    SKILLS_SetParam3(pSkill, pTarget->dwUnitId);

    return 1;
}

//D2Game.0x6FCF38C0
int32_t __fastcall SKILLS_SrvSt10_LightningStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !UNITS_GetUsedSkill(pUnit))
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    D2DamageStrc damage = {};

    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, 0, 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        const int32_t nMinDamage = SKILLS_GetMinElemDamage(pUnit, nSkillId, nSkillLevel, 1);
        const int32_t nMaxDamage = SKILLS_GetMaxElemDamage(pUnit, nSkillId, nSkillLevel, 1);
        damage.dwLtngDamage = nMinDamage + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDamage - nMinDamage);
        damage.dwEnDmgPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

        if (pSkillsTxtRecord->nEType)
        {
            damage.dwConvPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
            if (damage.dwConvPct > 0)
            {
                damage.nConvType = pSkillsTxtRecord->nEType;
            }
        }
    }

    uint8_t nSrcDam = pSkillsTxtRecord->nSrcDam;
    if (!nSrcDam)
    {
        nSrcDam = 0x80u;
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, nSrcDam);
    return 1;
}

//D2Game.0x6FCF3A70
int32_t __fastcall SKILLS_SrvDo006_InnerSight_SlowMissiles(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount || pSkillsTxtRecord->wAuraStat[0] < 0 || pSkillsTxtRecord->wAuraStat[0] >= sgptDataTables->nItemStatCostTxtRecordCount)
    {
        return 0;
    }

    D2SrvDo06Strc args = {};

    args.nSkillId = nSkillId;
    args.nSkillLevel = nSkillLevel;
    args.nDuration = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    args.nAuraTargetState = pSkillsTxtRecord->wAuraTargetState;
    args.nAuraStatId = pSkillsTxtRecord->wAuraStat[0];
    args.nAuraStatCalcValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[0], nSkillId, nSkillLevel);

    sub_6FD0FE80(pGame, pUnit, 0, 0, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel), pSkillsTxtRecord->dwAuraFilter, SKILLS_AuraCallback_InnerSight_SlowMissiles, &args, 1, __FILE__, __LINE__);
    return 1;
}

//D2Game.0x6FCF3B90
int32_t __fastcall SKILLS_AuraCallback_InnerSight_SlowMissiles(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2SrvDo06Strc* pArgs = (D2SrvDo06Strc*)pAuraCallback->pArgs;

    D2CurseStrc curse = {};

    curse.pTarget = pUnit;
    curse.pUnit = pAuraCallback->pOwner;
    curse.nSkill = pArgs->nSkillId;
    curse.nSkillLevel = pArgs->nSkillLevel;
    curse.nDuration = pArgs->nDuration;
    curse.nStat = pArgs->nAuraStatId;
    curse.nStatValue = pArgs->nAuraStatCalcValue;
    curse.nState = pArgs->nAuraTargetState;

    sub_6FD10EC0(&curse);

    return 1;
}

//D2Game.0x6FCF3C00
int32_t __fastcall SKILLS_SrvDo007_Jab(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    const int32_t nToHit = SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        sub_6FC627B0(pUnit, 8);
    }

    D2DamageStrc damage = {};
    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, nToHit, 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
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
    }

    uint8_t nSrcDam = pSkillsTxtRecord->nSrcDam;
    if (!nSrcDam)
    {
        nSrcDam = 0x80;
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, nSrcDam);
    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    return 1;
}

//D2Game.0x6FCF3D70
int32_t __fastcall SKILLS_SrvDo008_MultipleShot_Teeth_ShockWave(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: v9, v21, v24, v31
    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
    {
        return 1;
    }

    const int32_t v9 = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

    int32_t nDiffX = nTargetX - CLIENTS_GetUnitX(pUnit);
    int32_t nDiffY = nTargetY - CLIENTS_GetUnitY(pUnit);
    sub_6FD140D0(&nDiffX, &nDiffY);
    sub_6FD14120(&nDiffX, &nDiffY);

    nTargetX += v9 * nDiffX / -2;
    nTargetY += v9 * nDiffY / -2;

    int32_t nMissileId = pSkillsTxtRecord->wSrvMissileA;
    if (UNITS_GetWeaponClass(pUnit) != 1 && pSkillsTxtRecord->wSrvMissileB >= 0)
    {
        nMissileId = pSkillsTxtRecord->wSrvMissileB;
    }

    if (nMissileId >= 0 && nMissileId < sgptDataTables->nMissilesTxtRecordCount)
    {
        int32_t v31 = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
        if (!v31)
        {
            v31 = v9;
        }

        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 2080;
        missileParams.nSkillLevel = nSkillLevel;
        missileParams.pOwner = pUnit;
        missileParams.pOrigin = pUnit;
        missileParams.nSkill = nSkillId;
        missileParams.nMissile = nMissileId;
        missileParams.nActivateFrame = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        missileParams.dwFlags |= 0x10000;

        const int32_t v21 = (v9 - v31) / 2;
        for (int32_t i = 0; i < v21; ++i)
        {
            missileParams.nTargetX = nTargetX;
            missileParams.nTargetY = nTargetY;
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
            nTargetX += nDiffX;
            nTargetY += nDiffY;
        }

        missileParams.dwFlags &= 0xFFFEFFFF;

        for (int32_t i = 0; i < v31; ++i)
        {
            missileParams.nTargetX = nTargetX;
            missileParams.nTargetY = nTargetY;
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
            nTargetX += nDiffX;
            nTargetY += nDiffY;
        }

        const int32_t v24 = v9 - v21 - v31;
        missileParams.dwFlags |= 0x10000;
        for (int32_t i = 0; i < v24; ++i)
        {
            missileParams.nTargetX = nTargetX;
            missileParams.nTargetY = nTargetY;
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
            nTargetX += nDiffX;
            nTargetY += nDiffY;
        }

        return 1;
    }

    return 0;
}

//D2Game.0x6FCF4080
int32_t __fastcall SKILLS_SrvDo010_GuidedArrow_BoneSpirit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int32_t nParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

    int32_t nMissileId = pSkillsTxtRecord->wSrvMissileA;
    if (UNITS_GetWeaponClass(pUnit) != 1 && pSkillsTxtRecord->wSrvMissileB >= 0)
    {
        nMissileId = pSkillsTxtRecord->wSrvMissileB;
    }

    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.dwFlags = 0x20;
    missileParams.pOwner = pUnit;
    missileParams.pOrigin = pUnit;
    missileParams.pTarget = pTarget;
    missileParams.nSkill = nSkillId;
    missileParams.nTargetX = nTargetX;
    missileParams.nTargetY = nTargetY;
    missileParams.nMissile = nMissileId;

    if (nParam)
    {
        missileParams.pInitFunc = SKILLS_AddDamagePercentBonus;
    }
    missileParams.pInitArgs = nParam;

    if (!pTarget)
    {
        missileParams.dwFlags = 0x420;
    }

    D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
    if (pMissile)
    {
        if (pTarget)
        {
            MISSILE_SetTargetX(pMissile, 1);
        }
        else
        {
            MISSILE_SetTargetX(pMissile, 2);
        }

        const uint16_t nDiffX = nTargetX - CLIENTS_GetUnitX(pUnit);
        const uint16_t nDiffY = nTargetY - CLIENTS_GetUnitY(pUnit);
        MISSILE_SetTargetY(pMissile, nDiffX + (nDiffY << 16));
    }

    return 1;
}

//D2Game.0x6FCF4280
void __fastcall SKILLS_AddDamagePercentBonus(D2UnitStrc* pUnit, int32_t nBonus)
{
    if (nBonus)
    {
        STATLIST_SetUnitStat(pUnit, STAT_DAMAGEPERCENT, nBonus + STATLIST_GetUnitStatUnsigned(pUnit, STAT_DAMAGEPERCENT, 0), 0);
    }
}

//D2Game.0x6FCF42B0
int32_t __fastcall SKILLS_SrvDo011_ChargedStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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
    
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);

    const int32_t nMissileCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

    const int32_t nTargetX = CLIENTS_GetUnitX(pTarget);
    const int32_t nTargetY = CLIENTS_GetUnitY(pTarget);

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 33;
    missileParams.pOwner = pUnit;
    missileParams.nMissile = nMissileId;
    missileParams.nX = nTargetX;
    missileParams.nY = nTargetY;
    missileParams.nTargetX = 2 * nTargetX - CLIENTS_GetUnitX(pUnit);
    missileParams.nTargetY = 2 * nTargetY - CLIENTS_GetUnitY(pUnit);
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.pInitFunc = SKILLS_MissileInit_ChargedBolt;

    for (int32_t i = 0; i < nMissileCount; ++i)
    {
        missileParams.pInitArgs = i;
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return 1;
}

//D2Game.0x6FCF44D0
int32_t __fastcall SKILLS_SrvDo012_Strafe(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: v15, v23
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

    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);

    const int32_t nParam = SKILLS_GetParam1(pSkill);
    if (!nParam)
    {
        return 0;
    }

    int32_t nTargetGUID = SKILLS_GetParam3(pSkill);
    D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, SKILLS_GetParam2(pSkill), nTargetGUID);
    if (!pTarget)
    {
        pTarget = sub_6FD107F0(pGame, pUnit, 0, 0, nAuraRange, 3, nTargetGUID, 0);
        if (!pTarget)
        {
            return 1;
        }
    }
    UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY);

    int32_t nMissileId = pSkillsTxtRecord->wSrvMissileA;
    if (UNITS_GetWeaponClass(pUnit) != 1 && pSkillsTxtRecord->wSrvMissileB >= 0)
    {
        nMissileId = pSkillsTxtRecord->wSrvMissileB;
    }

    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    const int32_t v15 = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x21;
    missileParams.pOwner = pUnit;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nTargetX = nTargetX;
    missileParams.nTargetY = nTargetY;
    missileParams.nSkill = nSkillId;
    missileParams.nMissile = nMissileId;
    missileParams.pInitFunc = SKILLS_AddDamagePercentBonus;
    missileParams.pInitArgs = v15;
    MISSILES_CreateMissileFromParams(pGame, &missileParams);

    SKILLS_SetParam1(pSkill, nParam - 1);
    if (nParam - 1 <= 0)
    {
        return 0;
    }

    D2UnitStrc* v23 = sub_6FD107F0(pGame, pUnit, 0, 0, nAuraRange, 3, pTarget->dwUnitId, 0);
    if (v23)
    {
        SKILLS_SetParam2(pSkill, v23->dwUnitType);
        SKILLS_SetParam3(pSkill, v23->dwUnitId);
        sub_6FD15080(pUnit, pSkillsTxtRecord->dwParam[5]);
    }

    return 1;
}

//D2Game.0x6FCF47D0
int32_t __fastcall SKILLS_SrvDo013_Fend_Zeal_Fury(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: v14, v16
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

    const int32_t nAuraRange = sub_6FD15460(pUnit);
    const int32_t nTargetGUID = SKILLS_GetParam3(pSkill);
    D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, SKILLS_GetParam2(pSkill), nTargetGUID);
    if (!pTarget || !UNITS_IsInMeleeRange(pUnit, pTarget, 0))
    {
        pTarget = sub_6FD107F0(pGame, pUnit, 0, 0, nAuraRange, 0x20003, nTargetGUID, 0);
    }

    if (!pTarget)
    {
        return 0;
    }

    D2DamageStrc damage = {};

    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.dwEnDmgPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        if (pSkillsTxtRecord->nEType)
        {
            damage.dwConvPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
            if (damage.dwConvPct > 0)
            {
                damage.nConvType = pSkillsTxtRecord->nEType;
            }

            D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
        }
    }

    uint8_t nSrcDam = pSkillsTxtRecord->nSrcDam;
    if (!nSrcDam)
    {
        nSrcDam = 0x80;
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, nSrcDam);
    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 4);

    const int32_t v14 = SKILLS_GetParam1(pSkill) - 1;
    SKILLS_SetParam1(pSkill, v14);
    if (v14 <= 0)
    {
        return 0;
    }

    D2UnitStrc* v16 = sub_6FD107F0(pGame, pUnit, 0, 0, nAuraRange, 131075, pTarget ? pTarget->dwUnitId : -1, 0);
    if (!v16)
    {
        return 0;
    }

    SKILLS_SetParam2(pSkill, v16->dwUnitType);
    SKILLS_SetParam3(pSkill, v16->dwUnitId);
    sub_6FD15080(pUnit, pSkillsTxtRecord->dwParam[1]);
    return 0;
}

//D2Game.0x6FCF4A20
int32_t __fastcall SKILLS_SrvDo014_LightningStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: v13
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

    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    const int32_t nParam0 = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

    D2UnitStrc* v13 = sub_6FD107F0(pGame, pUnit, CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget), nParam0, 3, pTarget->dwUnitId, 0);
    if (!v13)
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    const int32_t nParam1 = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x20;
    missileParams.pOwner = pUnit;
    missileParams.pOrigin = pTarget;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nMissile = nMissileId;
    missileParams.nTargetX = CLIENTS_GetUnitY(v13);
    missileParams.nTargetY = CLIENTS_GetUnitY(v13);

    D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
    if (pMissile)
    {
        MISSILE_SetTargetX(pMissile, nParam1);
    }

    return 1;
}

//D2Game.0x6FCF4C50
int32_t __fastcall SKILLS_SrvDo015_Dopplezon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
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

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }


    D2SummonArgStrc summonArg = {};
    summonArg.nSpecialAiState = 0;
    summonArg.nPetType = nPetType;
    summonArg.nHcIdx = nSummonId;
    summonArg.pOwner = pUnit;
    summonArg.nMonMode = nSpawnMode;
    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
    if (!pPet)
    {
        return 0;
    }

    UNITS_StoreOwner(pPet, pUnit);
    const int32_t nHitpoints = MONSTERUNIQUE_CalculatePercentage(STATLIST_GetMaxLifeFromUnit(pUnit), SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel), 100);

    STATLIST_SetUnitStat(pPet, STAT_HITPOINTS, nHitpoints, 0);
    STATLIST_SetUnitStat(pPet, STAT_MAXHP, nHitpoints, 0);
    STATLIST_SetUnitStat(pPet, STAT_LEVEL, STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 0);
    D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(pGame, pUnit, pPet, 0, nSkillLevel);
    D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pPet, nSkillId, nSkillLevel, 0);
    EVENT_SetEvent(pGame, pPet, UNITEVENTCALLBACK_MONUMOD, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel) + pGame->dwGameFrame, 0, 0);
    D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pPet, 21, 0);
    UNITS_SetOverlay(pPet, 171, 0);
    D2GAME_UpdateSummonAI_6FC401F0(pGame, pPet, 0, pUnit->dwNodeIndex);
    return 1;
}

//D2Game.0x6FCF4EE0
int32_t __fastcall SKILLS_SrvDo016_Valkyrie(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
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

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2SummonArgStrc summonArg = {};
    summonArg.nSpecialAiState = 0;
    summonArg.nPetType = nPetType;
    summonArg.nHcIdx = nSummonId;
    summonArg.pOwner = pUnit;
    summonArg.nMonMode = nSpawnMode;
    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
    if (!pPet)
    {
        return 0;
    }

    D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(pGame, pUnit, pPet, 0, nSkillLevel);
    D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pPet, nSkillId, nSkillLevel, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel));
    D2GAME_EVENTS_Delete_6FC34840(pGame, pPet, UNITEVENTCALLBACK_AITHINK, 0);
    EVENT_SetEvent(pGame, pPet, UNITEVENTCALLBACK_AITHINK, pGame->dwGameFrame + 20, 0, 0);
    STATES_ToggleState(pPet, STATE_VALKYRIE, 1);
    UNITS_StoreOwner(pPet, pUnit);
    D2GAME_UpdateSummonAI_6FC401F0(pGame, pPet, 0, pUnit->dwNodeIndex);
    return 1;
}
