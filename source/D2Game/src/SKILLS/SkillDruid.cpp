#include "SKILLS/SkillDruid.h"

#include <D2BitManip.h>

#include <D2DataTbls.h>
#include <D2States.h>
#include <D2StatList.h>
#include <D2Skills.h>
#include <D2Dungeon.h>
#include <D2Monsters.h>
#include <D2Collision.h>
#include <Units/UnitRoom.h>
#include <DataTbls/MonsterIds.h>
#include <Units/UnitFinds.h>
#include <DataTbls/SkillsIds.h>
#include <Path/Step.h>
#include <D2Items.h>

#include <UselessOrdinals.h>

#include "AI/AiGeneral.h"
#include "AI/AiTactics.h"
#include "AI/AiThink.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Targets.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillMonst.h"
#include "SKILLS/SkillNec.h"
#include "SKILLS/Skills.h"
#include "SKILLS/SkillSor.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitInactive.h"
#include "UNIT/SUnitMsg.h"



//D2Game.0x6FCFDCF0
int32_t __fastcall SKILLS_SrvDo114_Raven(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, 0, 0);
    if (nSummonId < 0 || pSkillsTxtRecord->nPetType < 0 || pSkillsTxtRecord->nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2SummonArgStrc summonArg = {};
    summonArg.dwFlags = 0;
    summonArg.nAiSpecialState = AISPECIALSTATE_NONE;
    summonArg.nHcIdx = nSummonId;
    summonArg.pOwner = pUnit;
    summonArg.nMonMode = nSpawnMode;
    summonArg.nPetType = pSkillsTxtRecord->nPetType;
    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
    if (!pPet)
    {
        return 0;
    }

    pPet->dwFlags &= 0xFFFFFFFBu;
    STATLIST_SetUnitStat(pPet, STAT_HPREGEN, 0, 0);

    int32_t nLevel = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
    if (nLevel < 1)
    {
        nLevel = 1;
    }

    D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(pGame, pUnit, pPet, nLevel, nSkillLevel);
    D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pPet, nSkillId, nSkillLevel, 0);
    return 1;
}

//D2Game.0x6FCFDE90
int32_t __fastcall SKILLS_SrvDo115_Vines(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    if (pSkillsTxtRecord->nPetType < 0 || pSkillsTxtRecord->nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2SummonArgStrc summonArg = {};
    summonArg.nPetType = pSkillsTxtRecord->nPetType;
    summonArg.dwFlags = 0;
    summonArg.pOwner = pUnit;
    summonArg.nHcIdx = nSummonId;
    summonArg.nAiSpecialState = AISPECIALSTATE_NONE;
    summonArg.nMonMode = 8;
    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
    if (!pPet)
    {
        return 0;
    }

    D2GAME_UpdateSummonAI_6FC401F0(pGame, pPet, 0, pUnit->dwNodeIndex);
    STATES_ToggleState(pPet, STATE_VINE_BEAST, 1);
    int32_t nLevel = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
    if (nLevel < 1)
    {
        nLevel = 1;
    }

    STATLIST_SetUnitStat(pPet, STAT_LEVEL, nLevel, 0);
    D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pPet, nSkillId, nSkillLevel, 0);
    return 1;
}

//D2Game.0x6FCFE030
void __fastcall sub_6FCFE030(D2UnitStrc* pUnit, int32_t nStateId, D2StatListStrc* pStatList)
{
    sub_6FD11C90(pUnit, nStateId, 1);

    if (!pStatList)
    {
        return;
    }

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0));
    if (!pSkill)
    {
        return;
    }

    int32_t nUnitType = 6;
    int32_t nClassId = -1;
    int32_t nMode = 10;
    if (pUnit)
    {
        nUnitType = pUnit->dwUnitType;
        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            SKILLS_SetSkillMode(pSkill, PLRMODE_CAST);
            STATES_ToggleState(pUnit, nStateId, 0);
            return;
        }
        nClassId = pUnit->dwClassId;
    }

    D2COMMON_11013_ConvertMode(pUnit, &nUnitType, &nClassId, &nMode, __FILE__, __LINE__);
    SKILLS_SetSkillMode(pSkill, nMode);
    STATES_ToggleState(pUnit, nStateId, 0);
}

//D2Game.0x6FCFE0E0
void __fastcall sub_6FCFE0E0(D2UnitStrc* pUnit, D2StatListStrc* pStatList, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit || !pStatList)
    {
        return;
    }

    if (!pSkillsTxtRecord)
    {
        pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    }

    if (!pSkillsTxtRecord)
    {
        return;
    }

    for (int32_t i = 0; i < 6; ++i)
    {
        const int32_t nStatId = pSkillsTxtRecord->wAuraStat[i];
        if (SKILLS_GetItemStatCostTxtRecord(nStatId))
        {
            const int32_t nValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], nSkillId, nSkillLevel);
            if (nValue)
            {
                STATLIST_SetStat(pStatList, nStatId, nValue, 0);
                if (nStatId == STAT_ATTACKRATE)
                {
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_OTHER_ANIMRATE, nValue, 0);
                }
            }
        }
    }

    UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
}

//D2Game.0x6FCFE200
void __fastcall sub_6FCFE200(D2UnitStrc* pUnit, D2StatListStrc* pStatList, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit || !pStatList)
    {
        return;
    }

    if (!pSkillsTxtRecord)
    {
        pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    }

    if (!pSkillsTxtRecord)
    {
        return;
    }

    for (int32_t i = 0; i < 5; ++i)
    {
        const int32_t nStatId = pSkillsTxtRecord->nPassiveStat[i];
        if (nStatId >= 0 && nStatId < sgptDataTables->nItemStatCostTxtRecordCount && &sgptDataTables->pItemStatCostTxt[nStatId])
        {
            const int32_t nValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[i], nSkillId, nSkillLevel);
            if (nValue)
            {
                STATLIST_SetStat(pStatList, nStatId, nValue, 0);
                if (nStatId == STAT_ATTACKRATE)
                {
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_OTHER_ANIMRATE, nValue, 0);
                }
            }
        }
    }

    UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
}

//D2Game.0x6FCFE330
int32_t __fastcall SKILLS_SrvDo116_Wearwolf_Wearbear(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    if (sub_6FD11C90(pUnit, pSkillsTxtRecord->nAuraState, 1))
    {
        sub_6FD11BA0(pGame, pUnit, nSkillId, nSkillLevel);
        return 0;
    }

    const int32_t nLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    if (STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState))
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

    D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, pGame->dwGameFrame + nLength, nUnitType, nUnitGUID);
    if (!pStatList)
    {
        return 0;
    }

    STATLIST_SetState(pStatList, pSkillsTxtRecord->nAuraState);
    STATLIST_SetStatRemoveCallback(pStatList, sub_6FCFE030);
    D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
    STATES_ToggleState(pUnit, pSkillsTxtRecord->nAuraState, 1);
    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, pGame->dwGameFrame + nLength, 0, 0);
    sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (pSkill)
    {
        SKILLS_SetSkillMode(pSkill, 10);
    }

    return 1;
}

//D2Game.0x6FCFE4C0
int32_t __fastcall sub_6FCFE4C0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSubMissiles, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t a7)
{
    if (a7)
    {
        sub_6FD11420(pGame, nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
        --nSubMissiles;
        if (nSubMissiles <= 0)
        {
            return 1;
        }
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 33;
    missileParams.pOwner = pUnit;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nMissile = nMissileId;
    missileParams.pInitFunc = SKILLS_MissileInit_ChargedBolt;

    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &missileParams.nTargetX, &missileParams.nTargetY))
    {
        return 1;
    }

    for (int32_t i = 0; i < nSubMissiles; ++i)
    {
        missileParams.pInitArgs = i;
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return 1;
}

//D2Game.0x6FCFE5F0
int32_t __fastcall SKILLS_SrvDo117_Firestorm(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nParam >= 0)
    {
        return 0;
    }

    return sub_6FCFE4C0(pGame, pUnit, nParam, nMissileId, nSkillId, nSkillLevel, 1);
}

//D2Game.0x6FCFE6A0
int32_t __fastcall SKILLS_SrvDo118_Twister_Tornado(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nParam <= 0)
    {
        return 0;
    }

    return sub_6FCFE4C0(pGame, pUnit, nParam, nMissileId, nSkillId, nSkillLevel, 0);
}

//D2Game.0x6FCFE750
int32_t __fastcall SKILLS_SrvDo119_DruidSummon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, 0, 0);
    if (nSummonId < 0 || pSkillsTxtRecord->nPetType < 0 || pSkillsTxtRecord->nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
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

    D2SummonArgStrc summonArg = {};
    summonArg.dwFlags = 0;
    summonArg.nAiSpecialState = AISPECIALSTATE_NONE;
    summonArg.nPetType = pSkillsTxtRecord->nPetType;
    summonArg.pOwner = pUnit;
    summonArg.nHcIdx = nSummonId;
    summonArg.nMonMode = nSpawnMode;
    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
    if (!pPet)
    {
        return 0;
    }

    D2GAME_UpdateSummonAI_6FC401F0(pGame, pPet, 0, pUnit->dwNodeIndex);

    int32_t nLevel = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
    if (nLevel < 1)
    {
        nLevel = 1;
    }

    D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(pGame, pUnit, pPet, nLevel, nSkillLevel);
    D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pPet, nSkillId, nSkillLevel, 0);
    return 1;
}

//D2Game.0x6FCFE900
int32_t __fastcall SKILLS_SrvSt56_FeralRage_Maul(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    D2DamageStrc damage = {};

    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        if (pSkillsTxtRecord->nEType)
        {
            damage.dwConvPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
            if (damage.dwConvPct > 0)
            {
                damage.nConvType = pSkillsTxtRecord->nEType;
            }
        }
        damage.dwEnDmgPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    }

    uint8_t nSrcDam = pSkillsTxtRecord->nSrcDam;
    if (!nSrcDam)
    {
        nSrcDam = 0x80u;
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, nSrcDam);
    
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        SKILLS_SetParam1(pSkill, 1);
    }
    else
    {
        SKILLS_SetParam1(pSkill, 0);
    }

    return 1;
}

//D2Game.0x6FCFEA60
int32_t __fastcall SKILLS_SrvDo120_FeralRage_Maul(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pSkillsTxtRecord->nAuraState < 0 && pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__) != nSkillId)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    }

    if (!SKILLS_GetParam1(pSkill))
    {
        return 1;
    }

    const int32_t nExpireFrame = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel) + pGame->dwGameFrame;
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
            return 1;
        }

        STATLIST_SetState(pStatList, pSkillsTxtRecord->nAuraState);
        STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
        STATES_ToggleState(pUnit, pSkillsTxtRecord->nAuraState, 1);
    }

    D2COMMON_10476(pStatList, nExpireFrame);
    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);
    STATES_ToggleGfxStateFlag(pUnit, pSkillsTxtRecord->nAuraState, 1);

    int32_t nParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
    const int32_t nMax = STATLIST_GetStatValue(pStatList, STAT_SKILL_FRENZY, 0) + 1;
    if (nParam >= nMax)
    {
        nParam = nMax;
    }

    STATLIST_SetStatIfListIsValid(pStatList, STAT_SKILL_FRENZY, nParam, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);
    sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nParam);
    return 1;
}

//D2Game.0x6FCFEC90
int32_t __fastcall SKILLS_SrvSt57_Rabies(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__) != nSkillId)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    if (pSkillsTxtRecord->nAuraState >= 0 && pSkillsTxtRecord->nAuraState < sgptDataTables->nStatesTxtRecordCount && !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, 0);

    const int32_t nToHit = SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);
    if (!(SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, nToHit, 0) & 1))
    {
        return 0;
    }

    SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (pSkillsTxtRecord->nEType)
    {
        SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
    }

    SKILLS_SetParam1(pSkill, 1);
    return 1;
}

//D2Game.0x6FCFEDD0
int32_t __fastcall sub_6FCFEDD0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nTimeout, int32_t nSkillId, int32_t nSkillLevel)
{
    if (nSkillLevel <= 0)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState > sgptDataTables->nStatesTxtRecordCount)
    {
        return 1;
    }

    const int32_t nExpireFrame = nTimeout + pGame->dwGameFrame;
    if (STATLIST_GetStatListFromUnitAndState(pTarget, pSkillsTxtRecord->wAuraTargetState))
    {
        return 1;
    }

    int32_t nTargetUnitGUID = -1;
    int32_t nTargetUnitType = 6;
    if (pTarget)
    {
        nTargetUnitGUID = pTarget->dwUnitId;
        nTargetUnitType = pTarget->dwUnitType;
    }

    D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nExpireFrame, nTargetUnitType, nTargetUnitGUID);
    if (!pStatList)
    {
        return 1;
    }

    STATLIST_SetState(pStatList, pSkillsTxtRecord->wAuraTargetState);
    STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
    D2COMMON_10475_PostStatToStatList(pTarget, pStatList, 1);
    STATES_ToggleState(pTarget, pSkillsTxtRecord->wAuraTargetState, 1);
    D2COMMON_10476(pStatList, nExpireFrame);
    EVENT_SetEvent(pGame, pTarget, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);
    sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId <= 0)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.nMissile = nMissileId;
    missileParams.nRange = nTimeout;
    missileParams.dwFlags = 32768;
    missileParams.pOwner = pTarget;
    missileParams.pOrigin = pTarget;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
    if (!pMissile)
    {
        return 1;
    }

    int32_t nUnitType = 6;
    int32_t nUnitGUID = -1;
    if (pUnit)
    {
        nUnitType = pUnit->dwUnitType;
        nUnitGUID = pUnit->dwUnitId;
    }

    MISSILE_SetTargetX(pMissile, nUnitType);
    MISSILE_SetTargetY(pMissile, nUnitGUID);
    return 1;
}

//D2Game.0x6FCFEFD0
int32_t __fastcall SKILLS_SrvDo121_Rabies(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__) != nSkillId || !SKILLS_GetParam1(pSkill))
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, 0);
    sub_6FD00370(pGame, pUnit, nSkillLevel);
    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);

    int32_t nLength = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
    if (nSkillLevel > 0 && pSkillsTxtRecord->wAuraTargetState >= 0 && pSkillsTxtRecord->wAuraTargetState <= sgptDataTables->nStatesTxtRecordCount && !STATLIST_GetStatListFromUnitAndState(pTarget, pSkillsTxtRecord->wAuraTargetState))
    {
        D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nLength + pGame->dwGameFrame, pTarget->dwUnitType, pTarget->dwUnitId);
        if (pStatList)
        {
            STATLIST_SetState(pStatList, pSkillsTxtRecord->wAuraTargetState);
            STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
            D2COMMON_10475_PostStatToStatList(pTarget, pStatList, 1);
            STATES_ToggleState(pTarget, pSkillsTxtRecord->wAuraTargetState, 1);
            D2COMMON_10476(pStatList, nLength + pGame->dwGameFrame);
            EVENT_SetEvent(pGame, pTarget, UNITEVENTCALLBACK_REMOVESTATE, nLength + pGame->dwGameFrame, 0, 0);
            sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
            sub_6FCFF2E0(pGame, pUnit, pTarget, nLength, nSkillId, nSkillLevel);
        }
    }

    if (nLength < 10)
    {
        nLength = 10;
    }

    D2DamageStrc damage = {};
    sub_6FD155E0(pGame, pUnit, pTarget, pSkillsTxtRecord, nSkillId, nSkillLevel, &damage, 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
        if (pSkillsTxtRecord->dwHitClass)
        {
            damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
        }

        damage.dwEnDmgPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
        D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
    }

    sub_6FD11D90(pUnit, &damage, nLength, nSkillId);
    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    return 1;
}

//D2Game.0x6FCFF2E0
void __fastcall sub_6FCFF2E0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nRange, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        return;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId <= 0)
    {
        return;
    }

    D2MissileStrc missileParams = {};
    missileParams.pOwner = pTarget;
    missileParams.pOrigin = pTarget;
    missileParams.nMissile = nMissileId;
    missileParams.nRange = nRange;
    missileParams.dwFlags = 32768;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
    if (!pMissile)
    {
        return;
    }

    int32_t nUnitGUID = -1;
    int32_t nUnitType = 6;
    if (pUnit)
    {
        nUnitGUID = pUnit->dwUnitId;
        nUnitType = pUnit->dwUnitType;
    }

    MISSILE_SetTargetX(pMissile, nUnitType);
    MISSILE_SetTargetY(pMissile, nUnitGUID);
}

//D2Game.0x6FCFF3C0
int32_t __fastcall SKILLS_SrvSt58_FireClaws(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2DamageStrc damage = {};
    sub_6FD155E0(pGame, pUnit, pTarget, pSkillsTxtRecord, nSkillId, nSkillLevel, &damage, 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.dwHitFlags |= 2;
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
        if (pSkillsTxtRecord->dwHitClass)
        {
            damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
        }

        damage.dwEnDmgPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
        SUNITDMG_FillDamageValues(pGame, pUnit, pTarget, &damage, 0, pSkillsTxtRecord->nSrcDam);
        D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
        SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
    }

    return 1;
}

//D2Game.0x6FCFF4E0
int32_t __fastcall SKILLS_SrvDo122_Hunger(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2DamageStrc damage = {};
    sub_6FD155E0(pGame, pUnit, pTarget, pSkillsTxtRecord, nSkillId, nSkillLevel, &damage, 0);

    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.dwHitFlags |= 2;
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
        damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
        if (pSkillsTxtRecord->dwHitClass)
        {
            damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
        }

        SUNITDMG_FillDamageValues(pGame, pUnit, pTarget, &damage, 0, pSkillsTxtRecord->nSrcDam);
        D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
        damage.dwPhysDamage += MONSTERUNIQUE_CalculatePercentage(damage.dwPhysDamage, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel), 100);
        damage.dwLifeLeech += SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        damage.dwManaLeech += SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
        SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
        SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    }

    return 1;
}

//D2Game.0x6FCFF710
int32_t __fastcall SKILLS_SrvDo123_Volcano(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (nMissileId <= 0 || !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY) || !sub_6FD15340(pGame, pUnit, nMissileId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const uint32_t nRand = ITEMS_RollRandomNumber(&pUnit->pSeed);

    D2MissileStrc missileParams = {};
    missileParams.nMissile = nMissileId;
    missileParams.nX = nTargetX;
    missileParams.nY = nTargetY;
    missileParams.dwFlags = 1;
    missileParams.pOwner = pUnit;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
    if (!pMissile)
    {
        return 0;
    }

    MISSILE_SetTargetX(pMissile, nRand);
    sub_6FCC6300(pUnit, 0, nSkillId, nSkillLevel, nTargetX, nTargetY, nRand);
    return 1;
}

//D2Game.0x6FCFF870
int32_t __fastcall SKILLS_SrvDo124_Armageddon_Hurricane(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }
    
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__) != nSkillId)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const uint16_t nParam = (uint16_t)ITEMS_RollRandomNumber(&pUnit->pSeed);

    int32_t nLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    if (nLength < 1)
    {
        nLength = 1;
    }

    const int32_t nEndFrame = pGame->dwGameFrame + nLength;
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


        pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nEndFrame, nUnitType, nUnitGUID);
        if (!pStatList)
        {
            return 0;
        }

        STATLIST_SetState(pStatList, pSkillsTxtRecord->nAuraState);
        STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
        STATES_ToggleState(pUnit, pSkillsTxtRecord->nAuraState, 1);
    }

    D2COMMON_10476(pStatList, nEndFrame);
    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, nEndFrame, 0, 0);
    sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, pGame->dwGameFrame + pSkillsTxtRecord->dwParam[3], nSkillId, nSkillLevel);
    SKILLS_SetParam1(pSkill, nParam);
    return 1;
}

//D2Game.0x6FCFFAB0
int32_t __fastcall SKILLS_SrvDo145_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || nSkillLevel <= 0 || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount || !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
    {
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
        return 0;
    }

    if (!(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[8]))
    {
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
        if (pRoom && DUNGEON_IsRoomInTown(pRoom))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
            if (pStatList)
            {
                D2Common_10474(pUnit, pStatList);
                STATLIST_FreeStatList(pStatList);
            }

            STATES_ToggleState(pUnit, pSkillsTxtRecord->nAuraState, 0);
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
            return 0;
        }
    }

    const int32_t nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    if (nRange <= 0)
    {
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
        return 0;
    }

    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, pSkillsTxtRecord->dwParam[3] + pGame->dwGameFrame, nSkillId, nSkillLevel);

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
    {
        return 0;
    }

    D2DamageStrc damage = {};
    D2GAME_RollPhysicalDamage_6FD14EC0(pUnit, &damage, nSkillId, nSkillLevel);
    D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
    damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
    damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
    sub_6FD10200(pGame, pUnit, 0, 0, nRange, &damage, pSkillsTxtRecord->dwAuraFilter);
    return 1;
}

//D2Game.0x6FCFFCA0
int32_t __fastcall SKILLS_SrvDo146_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || nSkillLevel <= 0 || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount || !STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
    {
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
        return 0;
    }

    if (!(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_INTOWN]))
    {
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
        if (pRoom && DUNGEON_IsRoomInTown(pRoom))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nAuraState);
            if (pStatList)
            {
                D2Common_10474(pUnit, pStatList);
                STATLIST_FreeStatList(pStatList);
            }

            STATES_ToggleState(pUnit, pSkillsTxtRecord->nAuraState, 0);
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
            return 0;
        }
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId <= 0)
    {
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
        return 0;
    }

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
        return 0;
    }
    
    const int32_t nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    if (nRange <= 0)
    {
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
        return 0;
    }

    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, pGame->dwGameFrame + pSkillsTxtRecord->dwParam[3], nSkillId, nSkillLevel);
    
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
    {
        return 0;
    }

    SEED_InitLowSeed(&pUnit->pSeed, SKILLS_GetParam1(pSkill));
    SKILLS_SetParam1(pSkill, ITEMS_RollRandomNumber(&pUnit->pSeed) % 1000000);

    for (int32_t i = 0; i < 5; ++i)
    {
        const int32_t nX = CLIENTS_GetUnitX(pUnit) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 2 * nRange + 1) - nRange;
        const int32_t nY = CLIENTS_GetUnitY(pUnit) + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 2 * nRange + 1) - nRange;
        if (D2Common_11026(nX, nY, pUnit, 0x805u))
        {
            D2ActiveRoomStrc* pTargetRoom = D2GAME_GetRoom_6FC52070(pRoom, nX, nY);
            if (pTargetRoom && !COLLISION_CheckMask(pTargetRoom, nX, nY, 1u))
            {
                D2GAME_CreateMissile_6FD115E0(pGame, pUnit, nSkillId, nSkillLevel, nMissileId, nX, nY);
                sub_6FCC6300(pUnit, 0, nSkillId, nSkillLevel, nX, nY, 0);
                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FD00140
int32_t __fastcall D2GAME_EventFunc25_6FD00140(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pDamage)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pAttacker, pSkillsTxtRecord->nAuraState);
    if (!pStatList || pSkillsTxtRecord->wAuraStat[1] < 0 || pSkillsTxtRecord->wAuraStat[1] >= sgptDataTables->nItemStatCostTxtRecordCount)
    {
        return 0;
    }

    const int32_t nValue1 = STATLIST_GetStatValue(pStatList, pSkillsTxtRecord->wAuraStat[1], 0);
    if (nValue1 <= 0 || pSkillsTxtRecord->wAuraStat[0] < 0 || pSkillsTxtRecord->wAuraStat[0] >= sgptDataTables->nItemStatCostTxtRecordCount)
    {
        return 0;
    }

    int32_t nValue0 = STATLIST_GetStatValue(pStatList, pSkillsTxtRecord->wAuraStat[0], 0);
    if (nValue0)
    {
        if (pDamage->dwFireDamage > 0)
        {
            if (nValue0 > pDamage->dwFireDamage)
            {
                nValue0 -= pDamage->dwFireDamage;
                pDamage->dwFireDamage = 0;
            }
            else
            {
                pDamage->dwFireDamage -= nValue0;
                nValue0 = 0;
            }
        }

        if (pDamage->dwColdDamage > 0)
        {
            if (nValue0 > pDamage->dwColdDamage)
            {
                nValue0 -= pDamage->dwColdDamage;
                pDamage->dwColdDamage = 0;
            }
            else
            {
                pDamage->dwColdDamage -= nValue0;
                nValue0 = 0;
            }
        }

        if (pDamage->dwLtngDamage > 0)
        {
            if (nValue0 > pDamage->dwLtngDamage)
            {
                nValue0 -= pDamage->dwLtngDamage;
                pDamage->dwLtngDamage = 0;
            }
            else
            {
                pDamage->dwLtngDamage -= nValue0;
                nValue0 = 0;
            }
        }

        STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxtRecord->wAuraStat[0], nValue0, 0);
    }

    if (nValue0 <= 0)
    {
        STATES_ToggleState(pAttacker, pSkillsTxtRecord->nAuraState, 0);
        D2Common_10474(pAttacker, pStatList);
        STATLIST_FreeStatList(pStatList);
        return 1;
    }

    const int32_t nOldParamValue = STATLIST_GetStatValue(pStatList, STAT_UNSENTPARAM1, 0);
    const int32_t nNewParamValue = MONSTERUNIQUE_CalculatePercentage(100, nValue0, nValue1);
    if (nNewParamValue - nOldParamValue >= 5)
    {
        STATES_ToggleGfxStateFlag(pAttacker, pSkillsTxtRecord->nAuraState, 1);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_UNSENTPARAM1, nNewParamValue, 0);
    }

    return 1;
}

//D2Game.0x6FD00370
int32_t __fastcall sub_6FD00370(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    int32_t nToHit = 0;
    uint8_t nSrcDam = 0x80u;
    for (int32_t i = 0; i < sgptDataTables->nTransformStates; ++i)
    {
        const int32_t nState = sgptDataTables->pTransformStates[i];
        if (STATES_CheckState(pUnit, nState) && SKILLS_GetStatesTxtRecord(nState))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
            if (pStatList)
            {
                const int32_t nSkillId = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);
                D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
                if (pSkillsTxtRecord)
                {
                    nToHit += SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel);
                    if (pSkillsTxtRecord->nSrcDam)
                    {
                        nSrcDam = pSkillsTxtRecord->nSrcDam;
                    }
                }
            }
        }
    }

    D2DamageStrc damage = {};
    damage.dwHitClass = 1;
    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, nToHit, 0);
    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, nSrcDam);
    return 1;
}

//D2Game.0x6FD004E0
int32_t __fastcall SKILLS_SrvDo125_WakeOfDestruction(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount || !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 2;
    missileParams.nMissile = nMissileId;
    missileParams.pOwner = pUnit;
    missileParams.pOrigin = pUnit;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nTargetX = nTargetX - CLIENTS_GetUnitX(pUnit);
    missileParams.nTargetY = nTargetY - CLIENTS_GetUnitY(pUnit);

    D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
    if (!pMissile)
    {
        return 0;
    }

    MISSILE_SetTargetX(pMissile, -missileParams.nTargetY);
    MISSILE_SetTargetY(pMissile, missileParams.nTargetX);
    return 1;
}

//D2Game.0x6FD00660
int32_t __fastcall SKILLS_SrvSt59_ImpInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    int32_t nTimeout = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nTimeout < 1)
    {
        nTimeout = 1;
    }

    SKILLS_SetParam1(pSkill, nTimeout + pGame->dwGameFrame);

    if (!STATES_CheckState(pUnit, STATE_INFERNO))
    {
        STATES_ToggleState(pUnit, STATE_INFERNO, 1);
    }

    return 1;
}

//D2Game.0x6FD00710
int32_t __fastcall SKILLS_SrvDo126_ImpInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    
    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pSkillsTxtRecord->wSrvMissileA);
    if (!pMissilesTxtRecord)
    {
        return 0;
    }
    
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget && pUnit->pDynamicPath)
    {
        D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget));
        UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x25u;
    missileParams.pOwner = pUnit;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;
    missileParams.nVelocity = pMissilesTxtRecord->nVel + nSkillLevel * pMissilesTxtRecord->nVelLev / 8;
    missileParams.nTargetX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
    missileParams.nTargetY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);

    D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
    if (pMissile)
    {
        const int32_t nFrame = pMissilesTxtRecord->dwParam[1] + SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        PATH_SetNewDistance(pMissile->pDynamicPath, nFrame);
        MISSILE_SetTotalFrames(pMissile, nFrame);
        MISSILE_SetCurrentFrame(pMissile, nFrame);
    }

    SKILLS_SetInfernoFrame(pSkillsTxtRecord, pUnit);

    if (pGame->dwGameFrame < SKILLS_GetParam1(pSkill) && STATES_CheckState(pUnit, STATE_INFERNO))
    {
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, 0);
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_MODECHANGE, pGame->dwGameFrame + 3, 0, 0);
    }
    else
    {
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_MODECHANGE, 0);
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, pGame->dwGameFrame + 13, 0, 0);
    }

    return 1;
}

//D2Game.0x6FD009F0
int32_t __fastcall SKILLS_SrvSt60_SuckBlood(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2DamageStrc damage = {};
    sub_6FD155E0(pGame, pUnit, pTarget, pSkillsTxtRecord, nSkillId, nSkillLevel, &damage, 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.dwHitFlags |= 2;
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
        if (pSkillsTxtRecord->dwHitClass)
        {
            damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
        }

        SUNITDMG_RollSuckBloodDamage(pGame, pUnit, pTarget, nSkillId, nSkillLevel, &damage);
        damage.dwPhysDamage += MONSTERUNIQUE_CalculatePercentage(damage.dwPhysDamage, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel), 100);
        damage.dwLifeLeech += SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        damage.dwManaLeech += SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
        SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
    }

    return 1;
}

//D2Game.0x6FD00BE0
int32_t __fastcall SKILLS_SrvDo127_SuckBlood(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2DamageStrc* pDamage = SUNITDMG_GetDamageFromUnits(pUnit, pTarget);
    if (!pDamage)
    {
        return 0;
    }

    if (pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        D2UnitStrc* pOwner = nullptr;
        if (pUnit)
        {
            if (pUnit->dwUnitType == UNIT_MONSTER)
            {
                pOwner = AIGENERAL_GetMinionOwner(pUnit);
                if (!pOwner)
                {
                    pOwner = pUnit;
                }
            }
        }

        const int32_t nOwnerHitpoints = STATLIST_UnitGetStatValue(pOwner, STAT_HITPOINTS, 0);
        const int32_t nOwnerMaxHp = STATLIST_GetMaxLifeFromUnit(pOwner);
        const int32_t nTargetHitpoints = STATLIST_UnitGetStatValue(pTarget, STAT_HITPOINTS, 0);

        int32_t nDamage = pDamage->dwPhysDamage;
        if (nDamage >= nTargetHitpoints)
        {
            nDamage = nTargetHitpoints;
        }

        int32_t nNewHp = nOwnerHitpoints + MONSTERUNIQUE_CalculatePercentage(nDamage, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel), 100);
        if (nNewHp < 1)
        {
            nNewHp = 1;
        }
        else if (nNewHp >= nOwnerMaxHp)
        {
            nNewHp = nOwnerMaxHp;
        }

        STATLIST_SetUnitStat(pOwner, STAT_HITPOINTS, nNewHp, 0);
    }

    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    return 1;
}

//D2Game.0x6FD00DC0
int32_t __fastcall SKILLS_SrvDo128_CryHelp(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || !pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    D2AiCmdStrc aiCmd = {};
    aiCmd.nCmdParam[0] = 1;
    aiCmd.nCmdParam[1] = pTarget->dwUnitType;
    aiCmd.nCmdParam[2] = pTarget->dwUnitId;

    int32_t nTimeout = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nTimeout < 1)
    {
        nTimeout = 1;
    }

    aiCmd.nCmdParam[3] = nTimeout + pGame->dwGameFrame;
    AIGENERAL_AllocCommandsForMinions(pGame, pUnit, &aiCmd);

    if (pSkillsTxtRecord->wSrvOverlay > 0 && pSkillsTxtRecord->wSrvOverlay < sgptDataTables->nOverlayTxtRecordCount)
    {
        UNITS_SetOverlay(pTarget, pSkillsTxtRecord->wSrvOverlay, 0);
    }

    return 1;
}

//D2Game.0x6FD00EC0
void __fastcall sub_6FD00EC0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit && pTarget && pUnit->dwUnitType == UNIT_MONSTER && pTarget->dwUnitType == UNIT_MONSTER)
    {
        int32_t nUnitBaseId = pUnit->dwClassId;
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
        if (pMonStatsTxtRecord)
        {
            nUnitBaseId = pMonStatsTxtRecord->nBaseId;
        }

        if (nUnitBaseId == MONSTER_IMP1)
        {
            int32_t nTargetBaseId = pTarget->dwClassId;
            pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
            if (pMonStatsTxtRecord)
            {
                nTargetBaseId = pMonStatsTxtRecord->nBaseId;
            }

            if (nTargetBaseId == MONSTER_BARRICADETOWER || nTargetBaseId == MONSTER_SIEGEBEAST1)
            {
                STATES_ToggleState(pUnit, STATE_ATTACHED, 1);
                UNITS_StoreOwner(pUnit, pTarget);
                UNITS_StoreOwner(pTarget, pUnit);
                D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId);
                sub_6FC35570(pGame, pUnit, UNITEVENTCALLBACK_ACTIVESTATE, nSkillId, nSkillLevel);
                pUnit->dwFlags &= 0xFFFFFFF1u;
                if (pUnit->pMonsterData)
                {
                    AITHINK_ExecuteAiFn(pGame, pUnit, pUnit->pMonsterData->pAiControl, AISPECIALSTATE_MOUNTING_UNIT);
                }
                else
                {
                    AITHINK_ExecuteAiFn(pGame, pUnit, nullptr, AISPECIALSTATE_MOUNTING_UNIT);
                }
            }
        }
    }
}

//D2Game.0x6FD01010
void __fastcall sub_6FD01010(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit || !pOwner || pUnit->dwUnitType != UNIT_MONSTER || pOwner->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    int32_t nBaseId = pUnit->dwClassId;
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nBaseId);
    if (pMonStatsTxtRecord)
    {
        nBaseId = pMonStatsTxtRecord->nBaseId;
    }

    if (nBaseId == MONSTER_IMP1)
    {
        int32_t nOwnerBaseId = pOwner->dwClassId;
        pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nOwnerBaseId);
        if (pMonStatsTxtRecord)
        {
            nOwnerBaseId = pMonStatsTxtRecord->nBaseId;
        }

        if (nOwnerBaseId == MONSTER_BARRICADETOWER || nOwnerBaseId == MONSTER_SIEGEBEAST1)
        {
            STATES_ToggleState(pUnit, STATE_ATTACHED, 0);
            UNITS_StoreOwner(pUnit, 0);
            UNITS_StoreOwner(pOwner, 0);

            if (UNITS_GetCurrentLifePercentage(pUnit) >= 10)
            {
                pUnit->dwFlags |= UNITFLAG_ISVALIDTARGET | UNITFLAG_CANBEATTACKED | UNITFLAG_TARGETABLE;
                D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 5, nSkillId);

                if (pUnit->pMonsterData)
                {
                    AITHINK_ExecuteAiFn(pGame, pUnit, pUnit->pMonsterData->pAiControl, AISPECIALSTATE_NONE);
                }
                else
                {
                    AITHINK_ExecuteAiFn(pGame, pUnit, nullptr, AISPECIALSTATE_NONE);
                }
            }
            else
            {
                SUNITDMG_KillMonster(pGame, pUnit, 0, 1);
            }
        }
    }
}

//D2Game.0x6FD01170
int32_t __fastcall SKILLS_SrvDo129_ImpTeleport(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pUnit);
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pOwner)
    {
        int32_t nTargetX = 0;
        int32_t nTargetY = 0;
        D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY);
        sub_6FCBDFE0(pGame, pUnit, 0, nTargetX, nTargetY, 0, 0);
        sub_6FD01010(pGame, pUnit, pOwner, nSkillId, nSkillLevel);
        return 1;
    }

    if (!pTarget)
    {
        return SKILLS_SrvDo098_MonTeleport(pGame, pUnit, nSkillId, nSkillLevel);
    }

    if (SUNIT_GetOwner(pGame, pTarget) || SUNIT_IsDead(pTarget) || !sub_6FCBDFE0(pGame, pUnit, UNITS_GetRoom(pTarget), CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget), 1, 0))
    {
        return 0;
    }

    sub_6FD00EC0(pGame, pUnit, pTarget, nSkillId, nSkillLevel);
    return 1;
}

//D2Game.0x6FD01320
int32_t __fastcall SKILLS_SrvSt61_SelfResurrect(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    SKILLS_ResurrectUnit(pGame, pUnit);
    pUnit->dwFlags &= 0xFFFFFFF1;
    PATH_SetFootprintCollisionMask(pUnit->pDynamicPath, COLLIDE_MONSTER);
    return 1;
}

//D2Game.0x6FD01360
int32_t __fastcall SKILLS_SrvDo130_VineAttack(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    constexpr int32_t xOffsets[] =
    {
        0, 0, 1, -1
    };

    constexpr int32_t yOffsets[] =
    {
        1, -1, 0, 0
    };

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA <= 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nSubMissiles = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nSubMissiles <= 0)
    {
        return 0;
    }

    D2MissileStrc pMissile = {};
    pMissile.nSkillLevel = nSkillLevel;
    pMissile.nMissile = pSkillsTxtRecord->wSrvMissileA;
    pMissile.dwFlags = 3;
    pMissile.pOwner = pUnit;
    pMissile.nSkill = nSkillId;
    pMissile.pInitFunc = SKILLS_MissileInit_ChargedBolt;

    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &pMissile.nX, &pMissile.nY))
    {
        return 1;
    }

    for (int32_t i = 0; i < nSubMissiles; ++i)
    {
        const int32_t nIndex = i % 4;
        pMissile.pInitArgs = i;
        pMissile.nTargetX = xOffsets[nIndex];
        pMissile.nTargetY = yOffsets[nIndex];
        MISSILES_CreateMissileFromParams(pGame, &pMissile);
    }

    return 1;
}

//D2Game.0x6FD014A0
int32_t __fastcall SKILLS_SrvDo131_OverseerWhip(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || SUNIT_IsDead(pTarget))
    {
        return 0;
    }

    if (pTarget->dwUnitType == UNIT_MONSTER)
    {
        int32_t nBaseId = pTarget->dwClassId;
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
        if (pMonStatsTxtRecord)
        {
            nBaseId = pMonStatsTxtRecord->nBaseId;
        }

        if (nBaseId == MONSTER_MINION1)
        {
            if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) >= SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel) && !STATES_CheckState(pTarget, pSkillsTxtRecord->wAuraTargetState))
            {
                int32_t nSpawnMode = 0;
                const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, 0, 0);
                if (nSummonId >= 0)
                {
                    int32_t nChainId = 0;
                    DATATBLS_GetMonsterChainInfo(pTarget->dwClassId, 0, &nChainId);
                    const int32_t nClassId = sub_6FD017F0(nSummonId, nChainId);
                    MONSTER_Reinitialize(pGame, pTarget, nClassId, nSpawnMode);
                    AITHINK_ExecuteAiFn(pGame, pTarget, AIGENERAL_GetAiControlFromUnit(pTarget), AISPECIALSTATE_WHIPPED);

                    D2ModeChangeStrc modeChange = {};
                    MONSTERMODE_GetModeChangeInfo(pTarget, nSpawnMode, &modeChange);
                    D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
                    STATES_ToggleState(pTarget, STATE_CHANGECLASS, 1);

                    int32_t nUnitGUID = -1;
                    int32_t nUnitType = 6;
                    if (pUnit)
                    {
                        nUnitGUID = pUnit->dwUnitId;
                        nUnitType = pUnit->dwUnitType;
                    }

                    D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 0, 0, nUnitType, nUnitGUID);
                    if (pStatList)
                    {
                        D2COMMON_10475_PostStatToStatList(pTarget, pStatList, 1);
                        STATLIST_SetState(pStatList, STATE_CHANGECLASS);
                        STATLIST_SetStatIfListIsValid(pStatList, STAT_SHORTPARAM1, nClassId, 0);
                    }

                    if (pSkillsTxtRecord->wSumUMod > 0 && pSkillsTxtRecord->wSumUMod < 43)
                    {
                        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pTarget, pSkillsTxtRecord->wSumUMod, 0);
                    }
                }

                return 1;
            }
        }
    }

    D2CurseStrc curse = {};
    curse.pUnit = pUnit;
    curse.pTarget = pTarget;
    curse.nSkill = nSkillId;
    curse.nSkillLevel = nSkillLevel;
    curse.nStat = -1;
    curse.nDuration = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    curse.nState = pSkillsTxtRecord->wAuraTargetState;
    
    D2StatListStrc* pCurseStatList = sub_6FD10EC0(&curse);
    if (pCurseStatList)
    {
        sub_6FCFE0E0(pUnit, pCurseStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
    }

    return 1;
}

//D2Game.0x6FD017F0
int32_t __fastcall sub_6FD017F0(int32_t nMonsterId, int32_t nNoInChain)
{
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
    if (!pMonStatsTxtRecord)
    {
        return -1;
    }

    int32_t nClassId = pMonStatsTxtRecord->nBaseId;
    for (int32_t i = 0; i < nNoInChain; ++i)
    {
        nClassId = pMonStatsTxtRecord->nNextInClass;
        pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
        if (!pMonStatsTxtRecord)
        {
            break;
        }
    }

    return nClassId;
}

//D2Game.0x6FD01860
int32_t __fastcall SKILLS_SrvDo132_ImpFireMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA <= 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    int32_t nClassId = -1;
    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
        nClassId = pUnit->dwClassId;
    }

    int32_t nChainId = 0;
    DATATBLS_GetMonsterChainInfo(nClassId, 0, &nChainId);
    sub_6FD11420(pGame, nChainId + pSkillsTxtRecord->wSrvMissileA, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
    return 1;
}

//D2Game.0x6FD01910
int32_t __fastcall sub_6FD01910(D2UnitStrc* pUnit, D2UnitStrc* pTarget)
{
    if (!pTarget || pTarget->dwUnitType != UNIT_MONSTER || SUNIT_IsDead(pTarget))
    {
        return 0;
    }
    
    int32_t nBaseId = pTarget->dwClassId;
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nBaseId);
    if (pMonStatsTxtRecord)
    {
        nBaseId = pMonStatsTxtRecord->nBaseId;
    }

    if (nBaseId == MONSTER_PUTRIDDEFILER1 || nBaseId == MONSTER_PAINWORM1 || STATES_CheckState(pTarget, STATE_PREGNANT))
    {
        return 0;
    }

    return STATLIST_GetUnitAlignment(pTarget) != UNIT_ALIGNMENT_GOOD;
}

//D2Game.0x6FD019B0
int32_t __fastcall SKILLS_SrvDo133_Impregnate(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || !sub_6FD01910(pUnit, pTarget))
    {
        return 0;
    }

    D2CurseStrc curse = {};
    curse.pUnit = pUnit;
    curse.pTarget = pTarget;
    curse.nSkill = nSkillId;
    curse.nSkillLevel = nSkillLevel;
    curse.nStat = -1;
    curse.nState = STATE_PREGNANT;
    curse.pStateRemoveCallback = sub_6FD01B00;
    sub_6FD10EC0(&curse);
    return 1;
}

//D2Game.0x6FD01B00
void __fastcall sub_6FD01B00(D2UnitStrc* pUnit, int32_t nStateId, D2StatListStrc* pStatList)
{
    STATES_ToggleState(pUnit, nStateId, 0);

    int32_t nSummonId = MONSTER_PAINWORM1;
    int32_t nMonMode = MONMODE_NEUTRAL;
    if (pStatList)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(STATLIST_GetSkillId(pStatList));
        if (pSkillsTxtRecord)
        {
            if (pSkillsTxtRecord->wSummon > 0 && pSkillsTxtRecord->wSummon < sgptDataTables->nMonStatsTxtRecordCount)
            {
                nSummonId = pSkillsTxtRecord->wSummon;
            }

            if (pSkillsTxtRecord->nSumMode >= 0 && pSkillsTxtRecord->nSumMode < 16)
            {
                nMonMode = pSkillsTxtRecord->nSumMode;
            }
        }
    }

    if (SUNIT_IsDead(pUnit))
    {
        sub_6FC6A150(SUNIT_GetGameFromUnit(pUnit), pUnit, nSummonId, nMonMode, 1, 0);
    }
}

//D2Game.0x6FD01BB0
int32_t __fastcall SKILLS_SrvDo134_SiegeBeastStomp(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA <= 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2DamageStrc damage = {};
    damage.dwHitFlags |= 1;
    damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
    damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;

    if (pSkillsTxtRecord->dwHitClass)
    {
        damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
    }

    D2GAME_RollPhysicalDamage_6FD14EC0(pUnit, &damage, nSkillId, nSkillLevel);
    D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
    sub_6FD10200(pGame, pUnit, 0, 0, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel), &damage, 0);
    return 1;
}

//D2Game.0x6FD01CC0
int32_t __fastcall SKILLS_SrvSt62_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    int32_t nX = 0;
    int32_t nY = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 1, nSkillId, nSkillLevel, &nSpawnMode, &nX, &nY);
    SKILLS_SetParam1(pSkill, nSummonId);
    SKILLS_SetParam2(pSkill, nX);
    SKILLS_SetParam3(pSkill, nY);
    SKILLS_SetParam4(pSkill, nSpawnMode);
    return 1;
}

//D2Game.0x6FD01D40
int32_t __fastcall SKILLS_SrvDo135_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    const int32_t nMonsterId = SKILLS_GetParam1(pSkill);
    const int32_t nX = SKILLS_GetParam2(pSkill);
    const int32_t nY = SKILLS_GetParam3(pSkill);
    const int32_t nAnimMode = SKILLS_GetParam4(pSkill);

    D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
    if (!pRoom)
    {
        return 0;
    }

    D2UnitStrc* pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, nAnimMode, -1, 0);
    if (!pMonster)
    {
        pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, nAnimMode, 4, 0);
    }

    if (!pMonster)
    {
        return 0;
    }

    pMonster->dwFlags |= UNITFLAG_NOXP | UNITFLAG_NOTC;
    UNITS_StoreOwner(pMonster, pUnit);

    if (pSkillsTxtRecord->wSumOverlay >= 0 && pSkillsTxtRecord->wSumOverlay < sgptDataTables->nOverlayTxtRecordCount)
    {
        UNITS_SetOverlay(pMonster, pSkillsTxtRecord->wSumOverlay, 0);
    }

    return 1;
}

//D2Game.0x6FD01F10
int32_t __fastcall SKILLS_SrvDo136_DeathMaul(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nX = 0;
    int32_t nY = 0;
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA <= 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount || !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pMissile = D2GAME_CreateMissile_6FD115E0(pGame, pUnit, nSkillId, nSkillLevel, pSkillsTxtRecord->wSrvMissileA, 0, 0);
    if (!pMissile)
    {
        return 0;
    }

    const int32_t nAnimationSpeed = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nAnimationSpeed > 0)
    {
        UNITS_SetAnimationSpeed(pMissile, nAnimationSpeed);
    }

    const int32_t nTotalFrames = MISSILE_GetTotalFrames(pMissile);
    const int32_t nDistance = UNITS_GetDistanceToCoordinates(pUnit, nX, nY);
    int32_t nFrame = 0;
    if (nDistance < 1)
    {
        nFrame = nTotalFrames / 2 + nTotalFrames / 24;
    }
    else if (nDistance >= 10)
    {
        nFrame = nTotalFrames * nDistance / 12;
    }
    else
    {
        nFrame = nTotalFrames / 2 + nTotalFrames * nDistance / 24;
    }

    MISSILE_SetTotalFrames(pMissile, nFrame);
    MISSILE_SetCurrentFrame(pMissile, nFrame);
    return 1;
}

//D2Game.0x6FD02070
int32_t __fastcall SKILLS_SrvDo137_FenrisRage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || !D2COMMON_11018(pTarget))
    {
        return 0;
    }

    STATES_ToggleState(pTarget, STATE_CORPSE_NODRAW, 1);
    UNITROOM_RefreshUnit(pTarget);
    sub_6FD10250(pGame, pUnit, pUnit, pSkillsTxtRecord->nAuraState, nSkillId, nSkillLevel);
    return 1;
}

//D2Game.0x6FD02140
int32_t __fastcall SKILLS_SrvDo138_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA <= 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    int32_t nX = 0;
    int32_t nY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 1;
    }

    int32_t nXDiff = nX - CLIENTS_GetUnitX(pUnit);
    int32_t nYDiff = nY - CLIENTS_GetUnitY(pUnit);
    sub_6FD140D0(&nXDiff, &nYDiff);
    sub_6FD14120(&nXDiff, &nYDiff);
    const int32_t nSubMissiles = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    nX += nXDiff * nSubMissiles / -2;
    nY += nYDiff * nSubMissiles / -2;
    const int32_t nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x20;
    missileParams.pOwner = pUnit;
    missileParams.pOrigin = pUnit;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;
    missileParams.nRange = nRange;

    for (int32_t i = 0; i < nSubMissiles; ++i)
    {
        missileParams.nTargetY = nX;
        missileParams.nTargetX = nY;
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
        nX += nXDiff;
        nY += nYDiff;
    }

    if (STATES_CheckState(pUnit, STATE_INFERNO))
    {
        UNITS_SetAnimationFrame(pUnit, 8);
        pUnit->dwFrameCountPrecise = 0x600u;
    }

    return 1;
}

//D2Game.0x6FD02360
int32_t __fastcall SKILLS_SrvDo139_BaalColdMissiles(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA <= 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
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

    const int32_t nDiffX = nTargetX - CLIENTS_GetUnitX(pUnit);
    const int32_t nDiffY = nTargetY - CLIENTS_GetUnitY(pUnit);
    D2UnitStrc* pMissile = sub_6FD11420(pGame, pSkillsTxtRecord->wSrvMissileA, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
    if (!pMissile)
    {
        return 0;
    }

    MISSILE_SetTargetX(pMissile, -nDiffY);
    MISSILE_SetTargetY(pMissile, nDiffX);
    return 1;
}

//D2Game.0x6FD024A0
int32_t __fastcall SKILLS_SrvSt63_Corpse_VineCycler(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA <= 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
    if (!pOwner)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || STATES_CheckState(pTarget, STATE_CORPSE_NOSELECT))
    {
        return 0;
    }

    STATES_ToggleState(pTarget, STATE_CORPSE_NOSELECT, 1);
    UNITROOM_RefreshUnit(pTarget);

    D2MissileStrc missileParams = {};
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pTarget;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    MISSILES_CreateMissileFromParams(pGame, &missileParams);
    sub_6FCC6300(pUnit, pTarget, nSkillId, 1, 0, 0, 0);
    return 1;
}

//D2Game.0x6FD025C0
int32_t __fastcall SKILLS_SrvDo147_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pUnit);
    if (pOwner)
    {
        D2Common_10235_PATH_UpdateRiderPath(pUnit, pOwner);
    }

    return 1;
}

//D2Game.0x6FD025E0
int32_t __fastcall D2GAME_SKILLS_BloodMana_6FD025E0(D2UnitStrc* pUnit, int32_t nManaCost)
{
    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_BLOOD_MANA);
    if (STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) < nManaCost)
    {
        if (pStatList)
        {
            D2Common_10474(pUnit, pStatList);
            STATLIST_FreeStatList(pStatList);
        }

        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, 256, 0);
        return 0;
    }

    STATLIST_AddUnitStat(pUnit, STAT_HITPOINTS, -nManaCost, 0);

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(STATLIST_GetSkillId(pStatList));
    if (STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) < (pSkillsTxtRecord->dwParam[4] << 8) && pStatList)
    {
        D2Common_10474(pUnit, pStatList);
        STATLIST_FreeStatList(pStatList);
    }

    return 1;
}

//D2Game.0x6FD026B0
int32_t __fastcall SKILLS_SrvDo140_BaalTentacle(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit)
    {
        return 0;
    }

    const int32_t nMinionCount = pGame->nDifficulty + ITEMS_RollRandomNumber(&pUnit->pSeed) % 3 + 2;

    int32_t nMinionSpawnMode = 0;
    int32_t nMinionId = 0;
    int32_t nMinionX = 0;
    int32_t nMinionY = 0;
    MONSTERS_GetMinionSpawnInfo(pUnit, &nMinionId, &nMinionX, &nMinionY, &nMinionSpawnMode, pGame->nDifficulty, 0);
    
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);

    int32_t nX = 0;
    int32_t nY = 0;
    if (pTarget)
    {
        nX = CLIENTS_GetUnitX(pTarget);
        nY = CLIENTS_GetUnitY(pTarget);
    }
    else
    {
        nX = CLIENTS_GetUnitX(pUnit);
        nY = CLIENTS_GetUnitY(pUnit);
    }

    for (int32_t i = 0; i < nMinionCount; ++i)
    {
        nMinionX = nX + ITEMS_RollRandomNumber(&pUnit->pSeed) % 18 - 9;
        nMinionY = nY + ITEMS_RollRandomNumber(&pUnit->pSeed) % 18 - 9;
        
        D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nMinionX, nMinionY);
        if (pRoom)
        {
            D2UnitStrc* pMinion = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nMinionX, nMinionY, nMinionId, nMinionSpawnMode, -1, 0);
            if (pMinion)
            {
                MONSTERUNIQUE_ToggleUnitFlag(pMinion, UNITFLAG_NOXP, 1);
                MONSTERUNIQUE_ToggleUnitFlag(pMinion, UNITFLAG_NOTC, 1);
                AITACTICS_Idle(pGame, pMinion, 15);
                UNITS_StoreOwner(pMinion, pUnit);
            }
        }
    }

    return 1;
}

//D2Game.0x6FD02950
int32_t __fastcall SKILLS_SrvDo141_BaalCorpseExplode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2EffectStrc effect = {};

    effect.nFlags = 0x3002u;
    effect.nX = CLIENTS_GetUnitX(pUnit);
    effect.nY = CLIENTS_GetUnitY(pUnit);
    effect.pUnit = pUnit;
    effect.nSkillId = SKILL_BAALCORPSEEXPLODE;
    effect.nSkillLevel = nSkillLevel;
    effect.pfAreaEffect = SKILLS_AreaEffect_BaalCorpseExplode;

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord && nSkillLevel > 0)
    {
        effect.nParam = pSkillsTxtRecord->dwParam[4] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[5];
    }
    else
    {
        effect.nParam = 0;
    }

    sub_6FD149B0(pGame, &effect);
    return 1;
}

//D2Game.0x6FD02A50
int32_t __fastcall SKILLS_AreaEffect_BaalCorpseExplode(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillLevel, int32_t nUnused)
{
    UNITS_SetTargetUnitForDynamicUnit(pUnit, pTarget);
    return SKILLS_SrvDo055_CorpseExplosion(pGame, pUnit, SKILL_BAALCORPSEEXPLODE, nSkillLevel);
}

//D2Game.0x6FD02A80
D2UnitStrc* __fastcall SKILLS_FindUseableCorpse(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pOwner, int32_t nUnitSize)
{
    D2UnitFindArgStrc unitFindArg = {};
    unitFindArg.nX = CLIENTS_GetUnitX(pUnit);
    unitFindArg.nY = CLIENTS_GetUnitY(pUnit);
    unitFindArg.nSize = nUnitSize;
    unitFindArg.nFlags = 0x1002;
    unitFindArg.pUnit = pUnit;
    
    D2UnitFindDataStrc unitFindData = {};
    UNITFINDS_InitializeUnitFindData(pGame->pMemoryPool, &unitFindData, UNITS_GetRoom(pUnit), unitFindArg.nX, unitFindArg.nY, nUnitSize, 0, &unitFindArg);
    UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(&unitFindData);

    for (int32_t i = 0; i < unitFindData.nIndex; ++i)
    {
        D2UnitStrc* pResult = unitFindData.pUnitsArray[i];
        if (UNITS_IsCorpseUseable(pResult) && sub_6FCBD900(pGame, pUnit, pResult))
        {
            UNITFINDS_FreeUnitFindData(&unitFindData);
            return pResult;
        }
    }

    UNITFINDS_FreeUnitFindData(&unitFindData);
    return nullptr;
}
