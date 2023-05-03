#include "SKILLS/SkillSor.h"

#include <D2BitManip.h>

#include <D2StatList.h>
#include <D2States.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Skills.h>
#include <D2Items.h>
#include <D2Monsters.h>
#include <Path/PathMisc.h>

#include <UselessOrdinals.h>

#include "AI/AiGeneral.h"
#include "AI/AiThink.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MISSILES/MissMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterUnique.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/Player.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillDruid.h"
#include "SKILLS/SkillNec.h"
#include "SKILLS/SkillSor.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitEvent.h"
#include "UNIT/SUnitMsg.h"


#pragma pack(push, 1)
struct D2EnergyShieldDataStrc
{
    int32_t nDamageOffset;
    int32_t bPlayerOnly;
};
#pragma pack(pop)


//D2Game.0x6FD156F0
int32_t __fastcall SKILLS_DoInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nMissileId)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);

    int32_t nX = 0;
    int32_t nY = 0;
    if (!pSkillsTxtRecord || nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount || !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);

    if (!pSkill)
    {
        return 0;
    }

    if (SKILLS_GetParam1(pSkill))
    {
        int32_t nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
        if (nRange < 1)
        {
            nRange = 1;
        }

        D2MissileStrc missileParams = {};
        missileParams.nSkillLevel = nSkillLevel;
        missileParams.nMissile = nMissileId;
        missileParams.nTargetX = nX;
        missileParams.nTargetY = nY;
        missileParams.nRange = nRange;
        missileParams.dwFlags = 0x8020;
        missileParams.pOwner = pUnit;
        missileParams.pOrigin = pUnit;
        missileParams.nSkill = nSkillId;
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    SKILLS_SetParam1(pSkill, 1);

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        pUnit->dwGFXcurrentFrame = 0xB00;

        if (pGame->dwGameFrame < SKILLS_GetParam1(pSkill) && STATES_CheckState(pUnit, STATE_INFERNO))
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, 0);
            EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_MODECHANGE, pGame->dwGameFrame + 2, 4u, 0);
        }
        else
        {
            STATES_ToggleState(pUnit, STATE_INFERNO, 0);
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_MODECHANGE, 0);

            D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
            int32_t nInfernoLength = 1;
            if (pMonStats2TxtRecord)
            {
                nInfernoLength = pMonStats2TxtRecord->nInfernoLen;
            }

            EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, pGame->dwGameFrame + nInfernoLength, 0, 0);
        }
    }
    else
    {
        sub_6FCBD120(pGame, pUnit, 1);
    }

    return 1;
}

//D2Game.0x6FD15940
int32_t __fastcall SKILLS_StartInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t a5)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        int32_t nTimeout = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        if (nTimeout < 1)
        {
            nTimeout = 1;
        }

        SKILLS_SetParam1(UNITS_GetUsedSkill(pUnit), nTimeout + pGame->dwGameFrame);
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_INFERNO);
    if (pStatList)
    {
        sub_6FCBD120(pGame, pUnit, 1);
        D2COMMON_10476(pStatList, pGame->dwGameFrame + 6);
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, pGame->dwGameFrame + 6, 0, 0);
        SKILLS_DoInferno(pGame, pUnit, nSkillId, nSkillLevel, a5);
        return 1;
    }

    int32_t nUnitGUID = -1;
    int32_t nUnitType = 6;
    if (pUnit)
    {
        nUnitGUID = pUnit->dwUnitId;
        nUnitType = pUnit->dwUnitType;
    }

    pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, pGame->dwGameFrame + 20, nUnitType, nUnitGUID);
    if (!pStatList)
    {
        return 0;
    }

    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, pGame->dwGameFrame + 20, 0, 0);
    D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
    STATLIST_SetStatRemoveCallback(pStatList, SKILLS_StatRemoveCallback_RemoveState);
    STATLIST_SetState(pStatList, STATE_INFERNO);
    STATES_ToggleState(pUnit, STATE_INFERNO, 1);
    SKILLS_SetParam1(UNITS_GetUsedSkill(pUnit), 0);
    return 1;
}

//D2Game.0x6FD15AB0
int32_t __fastcall SKILLS_SrvSt11_Inferno_ArcticBlast(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord && (STATES_CheckState(pUnit, STATE_INFERNO) || STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0) >= pSkillsTxtRecord->wStartMana << 8) 
        && pSkillsTxtRecord->wSrvMissileA >= 0 && pSkillsTxtRecord->wSrvMissileA < sgptDataTables->nMissilesTxtRecordCount)
    {
        return SKILLS_StartInferno(pGame, pUnit, nSkillId, nSkillLevel, pSkillsTxtRecord->wSrvMissileA);
    }

    return 0;
}

//D2Game.0x6FD15B40
int32_t __fastcall SKILLS_SrvSt12_Telekinesis_DragonFlight(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    const int32_t nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    if (PATH_ComputeSquaredDistance(CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget)) > nRange * nRange)
    {
        return 0;
    }

    if (pTarget->dwUnitType == UNIT_PLAYER || pTarget->dwUnitType == UNIT_MONSTER)
    {
        if (!sub_6FCBD900(pGame, pUnit, pTarget) || DUNGEON_IsRoomInTown(UNITS_GetRoom(pTarget)) || DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
        {
            return 0;
        }
    }

    return 1;
}

//D2Game.0x6FD15CF0
int32_t __fastcall SKILLS_SrvSt13_ThunderStorm(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, -1);
    SKILLS_SetParam2(pSkill, 1);
    return 1;
}

//D2Game.0x6FD15D50
int32_t __fastcall SKILLS_SrvSt14_Hydra(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nX = 0;
    int32_t nY = 0;

    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom || !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    pRoom = D2GAME_GetRoom_6FC52070(pRoom, nX, nY);
    return pRoom && !DUNGEON_IsRoomInTown(pRoom);
}

//D2Game.0x6FD15E50
void __fastcall SKILLS_MissileInit_ChargedBolt(D2UnitStrc* pMissile, int32_t a2)
{
    if (!pMissile)
    {
        return;
    }

    uint8_t nFrames = MISSILE_GetTotalFrames(pMissile);
    if (nFrames >= 78)
    {
        nFrames = 77;
        MISSILE_SetTotalFrames(pMissile, 77);
        MISSILE_SetCurrentFrame(pMissile, 77);
    }

    SEED_InitLowSeed(&pMissile->pSeed, a2 + D2COMMON_10175_PathGetFirstPointX(pMissile->pDynamicPath));
    PATH_SetType(pMissile->pDynamicPath, 10);
    PATH_SetNewDistance(pMissile->pDynamicPath, nFrames);
    D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
}

//D2Game.0x6FD15EB0
int32_t __fastcall SKILLS_SrvDo017_ChargedBolt_BoltSentry(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x21;
    missileParams.pOwner = pUnit;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nMissile = nMissileId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nSkill = nSkillId;
    missileParams.pInitFunc = SKILLS_MissileInit_ChargedBolt;

    const int32_t nMissileCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    for (int32_t i = 0; i < nMissileCount; ++i)
    {
        missileParams.pInitArgs = i;
        if (D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &missileParams.nTargetX, &missileParams.nTargetY) && missileParams.nTargetX && missileParams.nTargetY)
        {
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
        }
    }

    return 1;
}

//D2Game.0x6FD16040
void __fastcall SKILLS_CurseStateCallback_DefensiveBuff(D2UnitStrc* pUnit, int32_t nState, int32_t nUnused)
{
    SUNITEVENT_FreeTimer(pUnit->pGame, pUnit, 1, nState);

    if (SUNIT_IsDead(pUnit) && STATES_CheckStateMaskStayDeathOnUnitByStateId(pUnit, nState))
    {
        return;
    }

    STATES_ToggleState(pUnit, nState, 0);
    D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
    sub_6FD14C30(pUnit);
}

//D2Game.0x6FD160A0
int32_t __fastcall SKILLS_SrvDo018_DefensiveBuff(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    sub_6FD11C90(pUnit, pSkillsTxtRecord->nAuraState, 1);

    D2CurseStrc curse = {};
    curse.pUnit = pUnit;
    curse.pTarget = pUnit;
    curse.nSkill = nSkillId;
    curse.nSkillLevel = nSkillLevel;
    curse.nStat = -1;
    curse.nDuration = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    curse.nState = pSkillsTxtRecord->nAuraState;
    curse.pStateFunc = SKILLS_CurseStateCallback_DefensiveBuff;

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    if (!pStatList)
    {
        return 0;
    }

    sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
    sub_6FCFE200(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);

    if (pSkillsTxtRecord->wAuraEvent[0] >= 0)
    {
        SUNITEVENT_FreeTimer(pGame, pUnit, 1, pSkillsTxtRecord->nAuraState);

        for (int32_t i = 0; i < 3; ++i)
        {
            if (pSkillsTxtRecord->wAuraEvent[i] < 0)
            {
                break;
            }

            sub_6FD156A0(pGame, pUnit, pSkillsTxtRecord->wAuraEvent[i], nSkillId, nSkillLevel, pSkillsTxtRecord->wAuraEventFunc[i], 1, pSkillsTxtRecord->nAuraState);
        }
    }

    STATES_ToggleGfxStateFlag(pUnit, pSkillsTxtRecord->nAuraState, 1);
    return 1;
}

//D2Game.0x6FD16270
int32_t __fastcall SKILLS_SrvDo019_Inferno_ArcticBlast(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (pSkillsTxtRecord && pSkillsTxtRecord->wSrvMissileA >= 0 && pSkillsTxtRecord->wSrvMissileA < sgptDataTables->nMissilesTxtRecordCount)
    {
        return SKILLS_DoInferno(pGame, pUnit, nSkillId, nSkillLevel, pSkillsTxtRecord->wSrvMissileA);
    }

    return 0;
}

//D2Game.0x6FD162D0
int32_t __fastcall SKILLS_SrvDo020_StaticField(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
	if (!pDifficultyLevelsTxtRecord)
	{
		return 0;
	}

    D2StaticFieldCallbackArgStrc staticField = {};
    staticField.nMinDamage = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
	staticField.nDamagePct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

    if (pGame->bExpansion)
    {
        staticField.nStaticFieldMin = pDifficultyLevelsTxtRecord->dwStaticFieldMin;
    }

	staticField.nElementalLength = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
	staticField.nElementalType = pSkillsTxtRecord->nEType;

    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
	sub_6FD0FE80(pGame, pUnit, 0, 0, nAuraRange, pSkillsTxtRecord->dwAuraFilter, SKILLS_AuraCallback_StaticField, &staticField, 0, __FILE__, __LINE__);
	return 1;
}

//D2Game.0x6FD163E0
int32_t __fastcall SKILLS_AuraCallback_StaticField(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pDefender)
{
    D2StaticFieldCallbackArgStrc* pStaticFieldCallbackArg = (D2StaticFieldCallbackArgStrc*)pAuraCallback->pArgs;

    const int32_t nHitpoints = STATLIST_UnitGetStatValue(pDefender, STAT_HITPOINTS, 0) >> 8;
    if (nHitpoints < 1)
    {
        return 0;
    }

    if (pStaticFieldCallbackArg->nStaticFieldMin)
    {
        const int32_t nMinDamage = MONSTERUNIQUE_CalculatePercentage(STATLIST_GetMaxLifeFromUnit(pDefender) >> 8, pStaticFieldCallbackArg->nStaticFieldMin, 100);
        if (nHitpoints <= nMinDamage)
        {
            return 0;
        }
    }

    int32_t nShiftedDamage = MONSTERUNIQUE_CalculatePercentage(nHitpoints, pStaticFieldCallbackArg->nDamagePct, 100);
    if (nShiftedDamage > nHitpoints - 1)
    {
        nShiftedDamage = nHitpoints - 1;
    }

    int32_t nDamage = nShiftedDamage << 8;
    if (nDamage < pStaticFieldCallbackArg->nMinDamage)
    {
        nDamage = pStaticFieldCallbackArg->nMinDamage;
    }

    D2DamageStrc damage = {};
    int32_t nStatId = -1;
    int32_t nElementalType = pStaticFieldCallbackArg->nElementalType;
    sub_6FD11E40(pAuraCallback->pOwner, &damage, nElementalType, nDamage, pStaticFieldCallbackArg->nElementalLength, &nStatId, &nElementalType);

    if (nStatId >= 0)
    {
        const int32_t nValue = STATLIST_UnitGetStatValue(pDefender, nStatId, 0);
        if (nValue < 0)
        {
            nDamage = MONSTERUNIQUE_CalculatePercentage(100, nDamage, 100 - nValue);
        }
    }

    memset(&damage, 0, sizeof(damage));
    sub_6FD11E40(pAuraCallback->pOwner, &damage, nElementalType, nDamage, pStaticFieldCallbackArg->nElementalLength, nullptr, nullptr);

    damage.dwHitClass |= 0xD;
    damage.nHitClassActiveSet = 1;
    damage.wResultFlags = 0x4001u;

    SUNITDMG_ExecuteEvents(pAuraCallback->pGame, pAuraCallback->pOwner, pDefender, 1, &damage);
    SUNITDMG_ExecuteMissileDamage(pAuraCallback->pGame, pAuraCallback->pOwner, pDefender, &damage);

    return 1;
}

//D2Game.0x6FD166A0
int32_t __fastcall SKILLS_SrvDo021_Telekinesis(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !pUnit || pUnit->dwUnitType != UNIT_PLAYER || PLAYER_IsBusy(pUnit))
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;

    switch (pTarget->dwUnitType)
    {
    case UNIT_PLAYER:
    case UNIT_MONSTER:
    {
        if (!sub_6FCBD900(pGame, pUnit, pTarget) || DUNGEON_IsRoomInTown(UNITS_GetRoom(pTarget)) || DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
        {
            return 0;
        }

        D2DamageStrc damage = {};
        D2GAME_RollPhysicalDamage_6FD14EC0(pUnit, &damage, nSkillId, nSkillLevel);
        D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);

        damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;

        if (pSkillsTxtRecord->dwHitClass)
        {
            damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
        }

        if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < pSkillsTxtRecord->dwParam[1])
        {
            damage.wResultFlags |= 9u;
        }

        SUNITDMG_ExecuteEvents(pGame, pUnit, pTarget, 1, &damage);
        SUNITDMG_ExecuteMissileDamage(pGame, pUnit, pTarget, &damage);
        break;
    }
    case UNIT_ITEM:
    {
        if (ITEMS_CheckItemTypeId(pTarget, ITEMTYPE_SCROLL) || ITEMS_CheckItemTypeId(pTarget, ITEMTYPE_GOLD) || ITEMS_CheckItemTypeId(pTarget, ITEMTYPE_MISSILE_POTION)
            || ITEMS_CheckItemTypeId(pTarget, ITEMTYPE_MISSILE) || ITEMS_CheckItemTypeId(pTarget, ITEMTYPE_POTION) || ITEMS_CheckItemTypeId(pTarget, ITEMTYPE_KEY))
        {
            int32_t a4 = 0;
            D2GAME_PickupItem_6FC43340(pGame, pUnit, pTarget->dwUnitId, &a4);
        }
        else
        {
            DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pTarget), pTarget->dwUnitType, pTarget->dwUnitId);
            ITEMS_SetInvPage(pTarget, 0xFFu);
            pTarget->dwFlags |= UNITFLAG_SENDREFRESHMSG;
            UNITS_ChangeAnimMode(pTarget, IMODE_ONGROUND);
            SUNIT_AttachSound(pUnit, 0x13u, pUnit);
        }
        break;
    }
    case UNIT_OBJECT:
    {
        int32_t bResult = 0;
        OBJECTS_OperateHandler(pGame, pUnit, pTarget->dwUnitType, pTarget->dwUnitId, &bResult);
        break;
    }
    default:
    {
        break;
    }
    }

    return 1;
}

//D2Game.0x6FD169A0
int32_t __fastcall SKILLS_SrvDo022_NovaAttack(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    const int32_t nMissileId = SKILLS_GetProgressiveSkillMissileId(pUnit, nSkillId);
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    const int32_t nVelocity = DATATBLS_GetMissileVelocityFromMissilesTxt(nMissileId, nSkillLevel);
    const int32_t nParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    sub_6FD14170(pGame, pUnit, pUnit, nMissileId, nSkillId, nSkillLevel, nParam + nVelocity);
    return 1;
}

//D2Game.0x6FD16A60
int32_t __fastcall SKILLS_SrvDo023_Blaze_EnergyShield_SpiderLay(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wAuraStat[0] < -1 || pSkillsTxtRecord->wAuraStat[0] >= sgptDataTables->nItemStatCostTxtRecordCount || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    D2CurseStrc curse = {};
    curse.pUnit = pUnit;
    curse.pTarget = pUnit;
    curse.nSkill = nSkillId;
    curse.nSkillLevel = nSkillLevel;
    curse.nStat = -1;
    curse.nStatValue = 0;
    curse.nDuration = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    curse.nState = pSkillsTxtRecord->nAuraState;

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    if (!pStatList)
    {
        return 0;
    }

    sub_6FCFE200(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nSkillLevel);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);

    if (pSkillsTxtRecord->wAuraEvent[0] >= 0)
    {
        SUNITEVENT_FreeTimer(pGame, pUnit, 1, pSkillsTxtRecord->nAuraState);
        for (int32_t i = 0; i < 3; ++i)
        {
            if (pSkillsTxtRecord->wAuraEvent[i] < 0)
            {
                break;
            }

            sub_6FD156A0(pGame, pUnit, pSkillsTxtRecord->wAuraEvent[i], nSkillId, nSkillLevel, pSkillsTxtRecord->wAuraEventFunc[i], 1, pSkillsTxtRecord->nAuraState);
        }
    }

    return 1;
}

//D2Game.0x6FD16C00
void __fastcall SKILLS_CreateBlazeMissile(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit->pDynamicPath || !D2Common_10234(pUnit->pDynamicPath))
    {
        return;
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_BLAZE);
    if (!pStatList)
    {
        return;
    }

    const int32_t nSkillId = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);
    int32_t nSkillLevel = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0);
    if (nSkillLevel < 0)
    {
        nSkillLevel = 1;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return;
    }

    const int32_t nMissileId = pSkillsTxtRecord->wSrvMissileA;
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount || DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)))
    {
        return;
    }

    D2MissileStrc missileParams = {};
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nMissile = nMissileId;
    missileParams.dwFlags = 0x21;
    missileParams.pOwner = pUnit;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    MISSILES_CreateMissileFromParams(pGame, &missileParams);
}

//D2Game.0x6FD16D70
int32_t __fastcall SKILLS_SrvDo024_FireWall(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pSkillsTxtRecord->wSrvMissileA < 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount || !sub_6FD15340(pGame, pUnit, pSkillsTxtRecord->wSrvMissileA))
    {
        return 0;
    }

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
        nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
        nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
    }

    if (!nX || !nY)
    {
        return 0;
    }

    D2RoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
    if (DUNGEON_IsRoomInTown(pRoom))
    {
        return 0;
    }

    D2MissileStrc missileParams = {};

    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;
    missileParams.dwFlags = 0x21;
    missileParams.pOwner = pUnit;
    missileParams.nX = nX;
    missileParams.nY = nY;
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;

    const int32_t nDiffX = CLIENTS_GetUnitX(pUnit) - nX;
    const int32_t nDiffY = CLIENTS_GetUnitY(pUnit) - nY;

    missileParams.nTargetY = nY + nDiffX;
    missileParams.nTargetX = nX - nDiffY;
    MISSILES_CreateMissileFromParams(pGame, &missileParams);

    missileParams.nTargetY = nY - nDiffX;
    missileParams.nTargetX = nX + nDiffY;
    MISSILES_CreateMissileFromParams(pGame, &missileParams);

    if (pSkillsTxtRecord->wSrvMissileB >= 0 && pSkillsTxtRecord->wSrvMissileB < sgptDataTables->nMissilesTxtRecordCount)
    {
        missileParams.nTargetX = 0;
        missileParams.nTargetY = 0;
        missileParams.nMissile = pSkillsTxtRecord->wSrvMissileB;
        missileParams.dwFlags = 1;
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return 1;
}

//D2Game.0x6FD17040
int32_t __fastcall SKILLS_SrvDo025_Enchant(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wAuraStat[0] < -1 || pSkillsTxtRecord->wAuraStat[0] >= sgptDataTables->nItemStatCostTxtRecordCount || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || !SUNIT_AreUnitsAligned(pGame, pUnit, pTarget))
    {
        pTarget = pUnit;
    }

    D2CurseStrc curse = {};
    curse.pUnit = pUnit;
    curse.pTarget = pTarget;
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

    for (int32_t i = 0; i < 6; ++i)
    {
        if (pSkillsTxtRecord->wAuraStat[i] >= 0 && pSkillsTxtRecord->wAuraStat[i] < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            const int32_t nValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], nSkillId, nSkillLevel);
            if (nValue)
            {
                STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxtRecord->wAuraStat[i], nValue, 0);
            }
        }
    }

    STATES_ToggleGfxStateFlag(pTarget, pSkillsTxtRecord->nAuraState, 1);
    return 1;
}

//D2Game.0x6FD17200
int32_t __fastcall SKILLS_SrvDo026_ChainLightning(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA < 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

    D2UnitStrc* pMissile = sub_6FD11420(pGame, pSkillsTxtRecord->wSrvMissileA, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
    if (!pMissile)
    {
        return 0;
    }

    MISSILE_SetTargetX(pMissile, nParam);
    return 1;
}

//D2Game.0x6FD172B0
int32_t __fastcall SKILLS_SrvDo151_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA < 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nDiffX = 0;
    int32_t nDiffY = 0;

    const int32_t nParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        nDiffX = CLIENTS_GetUnitX(pTarget) - CLIENTS_GetUnitX(pUnit);
        nDiffY = CLIENTS_GetUnitY(pTarget) - CLIENTS_GetUnitY(pUnit);
    }

    D2UnitStrc* pMissile = sub_6FD11420(pGame, pSkillsTxtRecord->wSrvMissileA, pUnit, nSkillId, nSkillLevel, nDiffX, nDiffY, 0, 0, 0);
    if (!pMissile)
    {
        return 0;
    }

    MISSILE_SetTargetX(pMissile, nParam);
    return 1;
}

//D2Game.0x6FD17450
int32_t __fastcall SKILLS_SrvDo027_Teleport(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY);

    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return 0;
    }

    D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(DUNGEON_GetLevelIdFromRoom(pRoom));
    if (pLevelsTxtRecord && pLevelsTxtRecord->nTeleport != 0 && (pLevelsTxtRecord->nTeleport != 2 || !UNITS_TestCollisionByCoordinates(pUnit, nTargetX, nTargetY, 0x804)))
    {
        return sub_6FCBDFE0(pGame, pUnit, 0, nTargetX, nTargetY, 0, 0);
    }

    return 0;
}

//D2Game.0x6FD174E0
int32_t __fastcall SKILLS_SrvDo028_Meteor_Blizzard_Eruption_BaalTaunt_Catapult(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA < 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    if (!sub_6FD15340(pGame, pUnit, pSkillsTxtRecord->wSrvMissileA))
    {
        return 0;
    }

    return D2GAME_CreateMissile_6FD115E0(pGame, pUnit, nSkillId, nSkillLevel, pSkillsTxtRecord->wSrvMissileA, 0, 0) != 0;
}

//D2Game.0x6FD17570
int32_t __fastcall SKILLS_SrvDo029_ThunderStorm(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA < 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount || pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
    if (!pSkill)
    {
        return 0;
    }

    if (!STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState) || SKILLS_GetParam2(pSkill))
    {
        D2CurseStrc curse = {};
        curse.pUnit = pUnit;
        curse.pTarget = pUnit;
        curse.nSkill = nSkillId;
        curse.nSkillLevel = nSkillLevel;
        curse.nDuration = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
        curse.nStat = -1;
        curse.nStatValue = 0;
        curse.nState = pSkillsTxtRecord->nAuraState;

        D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
        if (pStatList)
        {
            STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
            STATLIST_SetStatIfListIsValid(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);
        }
    }
    else
    {
        D2UnitStrc* pTarget = sub_6FD107F0(pGame, pUnit, 0, 0, pSkillsTxtRecord->dwParam[6], 3, SKILLS_GetParam1(pSkill), 0);
        if (pTarget)
        {
            if (pUnit && !DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)) && !DUNGEON_IsRoomInTown(UNITS_GetRoom(pTarget)))
            {
                D2UnitStrc* pMissile = D2GAME_CreateMissile_6FD115E0(pGame, pUnit, nSkillId, nSkillLevel, pSkillsTxtRecord->wSrvMissileA, CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget));
                if (pMissile)
                {
                    MISSMODE_SrvDmgHitHandler(pGame, pMissile, pTarget, 1);
                    SUNIT_RemoveUnit(pGame, pMissile);
                    sub_6FCC6300(pUnit, pTarget, nSkillId, nSkillLevel, 0, 0, 0);
                }
            }

            SKILLS_SetParam1(pSkill, pTarget->dwUnitId);
        }
        else
        {
            SKILLS_SetParam1(pSkill, -1);
        }
    }

    SKILLS_SetParam2(pSkill, 0);
    return 1;
}

//D2Game.0x6FD17820
int32_t __fastcall SKILLS_EventFunc24_EnergyShield(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pDamage)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pSkillsTxtRecord->nAuraState < 0 || pSkillsTxtRecord->nAuraState >= sgptDataTables->nStatesTxtRecordCount || !STATLIST_GetStatListFromUnitAndState(pAttacker, pSkillsTxtRecord->nAuraState))
    {
        return 0;
    }

    const int32_t nMultiplier = SKILLS_EvaluateSkillFormula(pAttacker, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nMultiplier <= 0)
    {
        return 0;
    }

    int32_t nMana = STATLIST_UnitGetStatValue(pAttacker, STAT_MANA, 0);
    int32_t nDivisor = SKILLS_EvaluateSkillFormula(pAttacker, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
    if (nDivisor <= 0)
    {
        nDivisor = 1;
    }

    int32_t bIsPlayer = 1;
    if (pUnit && (pUnit->dwUnitType != UNIT_MONSTER || MONSTERS_GetHirelingTypeId(pUnit)))
    {
        bIsPlayer = 0;
    }

    constexpr D2EnergyShieldDataStrc gEnergyShieldData_6FD29AD8[8] =
    {
        { offsetof(D2DamageStrc, dwPhysDamage), 0 },
        { offsetof(D2DamageStrc, dwFireDamage), 0 },
        { offsetof(D2DamageStrc, dwLtngDamage), 0 },
        { offsetof(D2DamageStrc, dwColdDamage), 0 },
        { offsetof(D2DamageStrc, dwMagDamage), 0 },
        { offsetof(D2DamageStrc, dwLifeLeech), 1 },
        { offsetof(D2DamageStrc, dwManaLeech), 1 },
        { offsetof(D2DamageStrc, dwStamLeech), 1 },
    };

    int32_t nAbsorbedDamage = 0;
    for (int32_t i = 0; i < 8; ++i)
    {
        const D2EnergyShieldDataStrc* pEnergyShieldData = &gEnergyShieldData_6FD29AD8[i];
        if (!pEnergyShieldData->bPlayerOnly || bIsPlayer)
        {
            int32_t* pDamageValue = (int32_t*)((char*)pDamage + pEnergyShieldData->nDamageOffset);
            if (*pDamageValue > 0)
            {
                int32_t nAbsorb = MONSTERUNIQUE_CalculatePercentage(*pDamageValue, nMultiplier, 100);
                const int32_t nMax = MONSTERUNIQUE_CalculatePercentage(nMana, 16, nDivisor);

                if (nAbsorb >= nMax)
                {
                    nAbsorb = nMax;
                }

                nMana -= MONSTERUNIQUE_CalculatePercentage(nAbsorb, nDivisor, 16);
                *pDamageValue -= nAbsorb;
                nAbsorbedDamage += nAbsorb;

                if (nMana <= 0)
                {
                    nMana = 0;
                }
            }
        }
    }

    if (nMana <= 0)
    {
        nMana = 0;
    }

    STATLIST_SetUnitStat(pAttacker, STAT_MANA, nMana, 0);

    if (pUnit && nAbsorbedDamage > 0 && pSkillsTxtRecord->wPrgOverlay >= 0 && pSkillsTxtRecord->wPrgOverlay <= sgptDataTables->nOverlayTxtRecordCount)
    {
        int32_t nOverlayData[2] = {};
        *(int64_t*)nOverlayData = D2Common_11053(UNITS_GetDirectionToCoords(pAttacker, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit))) + pSkillsTxtRecord->wPrgOverlay;
        UNITS_SetOverlay(pAttacker, nOverlayData[0], nOverlayData[1]);
    }

    if (nMana <= 0)
    {
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pAttacker, pSkillsTxtRecord->nAuraState);
        if (pStatList)
        {
            D2Common_10474(pAttacker, pStatList);
            STATLIST_FreeStatList(pStatList);
            return 1;
        }

        STATES_ToggleState(pAttacker, pSkillsTxtRecord->nAuraState, 0);
    }

    return 1;
}

//D2Game.0x6FD17C30
int32_t __fastcall SKILLS_SrvDo144_Hydra(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, nullptr, nullptr);
    if (nSummonId < 0 || pSkillsTxtRecord->nPetType < 0 || pSkillsTxtRecord->nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        return 0;
    }
    
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    int32_t nX = 0;
    int32_t nY = 0;
    if (!pRoom || !D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    pRoom = D2GAME_GetRoom_6FC52070(pRoom, nX, nY);
    if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
    {
        return 0;
    }

    D2UnitStrc* pOwner = pUnit;
    if (pOwner && pOwner->dwUnitType == UNIT_MONSTER && MONSTERS_GetHirelingTypeId(pOwner))
    {
        D2UnitStrc* pTemp = AIGENERAL_GetMinionOwner(pOwner);
        if (pTemp)
        {
            pOwner = pTemp;
        }
    }

    int32_t nParam = pGame->dwGameFrame;
    if (nSkillLevel > 0)
    {
        nParam += pSkillsTxtRecord->dwParam[0] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[1];
    }

    D2SummonArgStrc summonArg = {};
    summonArg.nPetType = pSkillsTxtRecord->nPetType;
    summonArg.dwFlags |= 1;
    summonArg.nMonMode = nSpawnMode;
    summonArg.pOwner = pOwner;
    summonArg.nSpecialAiState = 0;
    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

    constexpr int32_t nXOffsets[] = { -1, 0,  1 };
    constexpr int32_t nYOffsets[] = { -1, 0, -1 };

    int32_t bResult = 0;
    for (int32_t i = 0; i < 3; ++i)
    {
        summonArg.pPosition.nX = nX + nXOffsets[i];
        summonArg.pPosition.nY = nY + nYOffsets[i];
        summonArg.nHcIdx = i + nSummonId;

        D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
        if (pPet)
        {
            D2GAME_SetSkills_6FD14C60(pPet, nSkillId, nSkillLevel, 1);
            D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pOwner, pPet, nSkillId, nSkillLevel, 0);

            bResult = 1;

            if (pPet->dwUnitType == UNIT_MONSTER && pPet->pMonsterData && pPet->pMonsterData->pAiControl)
            {
                AITHINK_SetAiControlParams(pPet->pMonsterData->pAiControl, nParam, -666, -666);
            }

            if (pOwner && pOwner->dwUnitType == UNIT_MONSTER)
            {
                sub_6FCBDD30(pPet, STATLIST_GetUnitAlignment(pOwner), 1);
            }
        }
    }

    return bResult;
}

//D2Game.0x6FD17F40
int32_t __fastcall SKILLS_EventFunc01_ChillingArmor(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit)
    {
        return 0;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pUnit);
    if (!pOwner || !sub_6FCBD900(pGame, pAttacker, pOwner))
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    const int32_t nMissileId = pSkillsTxtRecord->wSrvMissileA;
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    if (nSkillLevel < 0)
    {
        nSkillLevel = 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pUnit->dwClassId);
    if (!pMissilesTxtRecord || !(pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_RETURNFIRE]))
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.nSkill = nSkillId;
    missileParams.dwFlags = 0x20;
    missileParams.pOwner = pAttacker;
    missileParams.pOrigin = pAttacker;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nMissile = nMissileId;
    missileParams.nTargetX = CLIENTS_GetUnitX(pOwner);
    missileParams.nTargetY = CLIENTS_GetUnitY(pOwner);
    MISSILES_CreateMissileFromParams(pGame, &missileParams);
    return 1;
}

//D2Game.0x6FD180E0
int32_t __fastcall SKILLS_EventFunc02_FrozenArmor(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER) || (pDamage && pDamage->dwPhysDamage <= 0))
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (nSkillLevel < 0)
    {
        nSkillLevel = 1;
    }

    D2DamageStrc damage = {};
    damage.wResultFlags |= 0x20u;
    damage.dwFrzLen = SKILLS_EvaluateSkillFormula(pAttacker, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    SUNITDMG_ExecuteEvents(pGame, pAttacker, pUnit, 1, &damage);

    if (pSkillsTxtRecord->wCltOverlayA >= 0 && pSkillsTxtRecord->wCltOverlayA < sgptDataTables->nOverlayTxtRecordCount)
    {
        UNITS_SetOverlay(pUnit, pSkillsTxtRecord->wCltOverlayA, 0);
    }

    return 1;
}

//D2Game.0x6FD18200
int32_t __fastcall SKILLS_EventFunc03_ShiverArmor(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (nSkillLevel < 0)
    {
        nSkillLevel = 1;
    }

    D2DamageStrc damage = {};
    D2GAME_RollElementalDamage_6FD14DD0(pAttacker, &damage, nSkillId, nSkillLevel);
    damage.nHitClassActiveSet = 1;
    damage.dwHitClass |= 0xDu;
    damage.wResultFlags = 16417;
    SUNITDMG_ExecuteEvents(pGame, pAttacker, pUnit, 1, &damage);
    SUNITDMG_ExecuteMissileDamage(pGame, pAttacker, pUnit, &damage);

    if (pSkillsTxtRecord->wCltOverlayA >= 0 && pSkillsTxtRecord->wCltOverlayA < sgptDataTables->nOverlayTxtRecordCount)
    {
        UNITS_SetOverlay(pUnit, pSkillsTxtRecord->wCltOverlayA, 0);
    }

    return 1;
}
