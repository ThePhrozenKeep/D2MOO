#include "SKILLS/SkillMonst.h"

#include <algorithm>

#include <D2BitManip.h>
#include <D2Math.h>

#include <D2Monsters.h>
#include <D2Dungeon.h>
#include <D2StatList.h>
#include <D2DataTbls.h>
#include <D2Skills.h>
#include <D2Items.h>
#include <D2States.h>
#include <Units/UnitRoom.h>
#include <D2Collision.h>
#include <DataTbls/SkillsIds.h>
#include <DataTbls/MissilesIds.h>
#include <Path/Step.h>
#include <Units/UnitFinds.h>
#include <DataTbls/MonsterIds.h>


#include "AI/AiGeneral.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "QUESTS/Quests.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillBar.h"
#include "SKILLS/SkillNec.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"


//D2Game.0x6FD061C0
int32_t __fastcall SKILLS_SrvSt42_FireHit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        return SKILLS_SrvSt32_Conversion_Bash_Stun_Concentrate_BearSmite(pGame, pUnit, nSkillId, nSkillLevel);
    }

    D2DamageStrc damage = {};

    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
        if (pSkillsTxtRecord->dwHitClass)
        {
            damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
        }
        sub_6FC627B0(pUnit, MONMODE_SKILL1);
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);

    return 1;
}

//D2Game.0x6FD062E0
int32_t __fastcall SKILLS_SrvDo083_FireHit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);

    if (pTarget)
    {
        SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
        SUNITDMG_FreeAttackerDefenderCombatList(pGame, pUnit, pTarget);
        return sub_6FD12950(pGame, pUnit, nSkillId, nSkillLevel, 0);
    }

    return 0;
}

//D2Game.0x6FD06330
int32_t __fastcall SKILLS_SrvSt43_MaggotEgg(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit)
    {
        pUnit->dwFlags &= 0xFFFFFFF1;
    }

    PATH_SetUnitDeadCollision(pUnit, 1);
    return 1;
}

//D2Game.0x6FD06360
int32_t __fastcall SKILLS_SrvDo084_MaggotEgg(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 1, nSkillId, nSkillLevel, &nSpawnMode, nullptr, nullptr);
    if (nSummonId < 0 || !UNITS_GetUsedSkill(pUnit))
    {
        return 0;
    }

    pUnit->dwSeqSpeed = 0;

    const int32_t nCount = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nCount > 0)
    {
        int32_t nCounter1 = 0;
        D2UnitStrc* pSummon = sub_6FC68D70(pGame, pUnit, nSummonId, nSpawnMode, -1, 0);
        if (pSummon)
        {
            nCounter1 = 1;
            pSummon->dwFlags |= UNITFLAG_NOXP;
        }

        int32_t nCounter2 = 1;
        for (int32_t i = 0; nCounter1 < nCount; ++i)
        {
            if (nCounter2 >= 5 || i >= 2 * nCount)
            {
                break;
            }

            pSummon = sub_6FC68D70(pGame, pUnit, nSummonId, MONMODE_SKILL1, nCounter2, 0);
            if (pSummon)
            {
                ++nCounter1;
                pSummon->dwFlags |= UNITFLAG_NOXP;
            }
            else
            {
                ++nCounter2;
            }
        }
    }

    SUNITDMG_KillMonster(pGame, pUnit, pUnit, 1);

    return 1;
}

//D2Game.0x6FD064D0
int32_t __fastcall SKILLS_SrvDo085_UnholyBolt_ShamanFire(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{    
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA < 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    int32_t nClassId = -1;
    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
        nClassId = pUnit->dwClassId;
    }

    int32_t nMissileId = 0;
    DATATBLS_GetMonsterChainInfo(nClassId, nullptr, &nMissileId);
    nMissileId += pSkillsTxtRecord->wSrvMissileA;

    if (nMissileId < 0 && nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    sub_6FD11420(pGame, nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
    return 1;
}

//D2Game.0x6FD06590
int32_t __fastcall SKILLS_SrvSt44_MaggotUp(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (sub_6FCBDFE0(pGame, pUnit, UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 0, 0))
    {
        pUnit->dwFlags |= 0xEu;
        COLLISION_SetMaskWithPattern(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 1, 0x1000u);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD066A0
int32_t __fastcall SKILLS_SrvSt45_MaggotDown(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit)
    {
        pUnit->dwFlags &= 0xFFFFFFF1;
    }

    COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 5, 0x1000u);
    return 1;
}

//D2Game.0x6FD06730
int32_t __fastcall SKILLS_SrvDo086_MaggotDown(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;

    int32_t nFrameNo = 0;
    int32_t nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrameNo, &nFrameCount);

    if (nFrameNo <= 0)
    {
        pUnit->dwSeqSpeed = 0;
    }

    const int32_t nPercentage = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nPercentage > 0)
    {
        const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);

        int32_t nNewHp = MONSTERUNIQUE_CalculatePercentage(nPercentage, nHitpoints, 100) + nHitpoints;
        if (nNewHp >= nMaxHp)
        {
            nNewHp = nMaxHp;
        }

        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);
    }

    return 1;
}

//D2Game.0x6FD06880
int32_t __fastcall SKILLS_SrvDo087_MaggotLay(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: Name
    constexpr int8_t byte_6FD296C8[] =
    {
        10, 8, 22, 20, 18, 16, 14, 12
    };

    if (!SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 1, nSkillId, nSkillLevel, &nSpawnMode, 0, 0);
    if (nSummonId < 0)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    int8_t nIndex = D2Common_11053(UNITS_GetDirectionToCoords(pUnit, CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget))) & 7;
    if (nIndex < 0)
    {
        nIndex = ((uint8_t)(nIndex - 1) | 0xF8) + 1;
    }

    int32_t nOffsetX = 0;
    int32_t nOffsetY = 0;
    D2Common_11055(byte_6FD296C8[(uint8_t)nIndex], &nOffsetX, &nOffsetY);

    pUnit->nActionFrame = 0;

    D2UnitStrc* pSummon = D2GAME_SpawnMonster_6FC69F10(pGame, UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit) + nOffsetX, CLIENTS_GetUnitY(pUnit) + nOffsetY, nSummonId, nSpawnMode, -1, 0);
    if (pSummon)
    {
        pSummon->dwFlags |= UNITFLAG_NOXP;
        return 1;
    }

    return 0;
}

//D2Game.0x6FD06A60
int32_t __fastcall SKILLS_SrvSt46_AndrialSpray(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
        if (pSkill)
        {
            SKILLS_SetParam1(pSkill, CLIENTS_GetUnitX(pTarget));
            SKILLS_SetParam2(pSkill, CLIENTS_GetUnitY(pTarget));
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD06B20
int32_t __fastcall SKILLS_SrvDo088_AndrialSpray(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    // TODO: Names
    constexpr int8_t byte_6FD296D0[] =
    {
        27, 14, 15, 3, 99, 7, 21, 22, 31,
        26, 12, 13, 2, 99, 6, 19, 20, 30,
        25, 10, 11, 1, 99, 5, 17, 18, 29,
        24, 8, 9, 0, 99, 4, 15, 16, 28,
        31, 22, 23, 7, 99, 3, 13, 14, 27,
        30, 20, 7, 6, 99, 2, 1, 12, 26,
        29, 18, 19, 5, 99, 1, 9, 10, 25,
        28, 16, 17, 4, 99, 0, 23, 8, 24
    };

    constexpr int8_t byte_6FD29718[] =
    {
        29, 28, 27, 26, 25, 24, 31, 30
    };

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSrvMissileA < 0 || pSkillsTxtRecord->wSrvMissileA >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }
    
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    int32_t nX = SKILLS_GetParam1(pSkill);
    int32_t nY = SKILLS_GetParam2(pSkill);
    if (!nX || !nY)
    {
        if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
        {
            return 0;
        }
    }

    const uint8_t nIndex = D2Common_11053(UNITS_GetDirectionToCoords(pUnit, nX, nY)) & 7;

    nX = CLIENTS_GetUnitX(pUnit);
    nY = CLIENTS_GetUnitY(pUnit);

    int32_t nOffsetX = 0;
    int32_t nOffsetY = 0;
    D2Common_11055(byte_6FD29718[nIndex], &nOffsetX, &nOffsetY);
    nX += nOffsetX;
    nY += nOffsetY;

    int32_t nFrame = 0;
    int32_t nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrame, &nFrameCount);
    
    int32_t nOffset = nFrame - 4;
    if (nOffset < 0)
    {
        nOffset = 0;
    }
    else if (nOffset > 8)
    {
        nOffset = 8;
    }

    const int8_t a1 = byte_6FD296D0[9 * nIndex + nOffset];
    if (a1 != 99)
    {
        D2Common_11055(a1, &nOffsetX, &nOffsetY);
        nX += nOffsetX;
        nY += nOffsetY;
    }

    D2MissileStrc missileParams = {};
    missileParams.nTargetX = nX;
    missileParams.nTargetY = nY;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.dwFlags = 0x20u;
    missileParams.pOwner = pUnit;
    missileParams.pOrigin = pUnit;
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;

    MISSILES_CreateMissileFromParams(pGame, &missileParams);
    return 1;
}

//D2Game.0x6FD06D60
int32_t __fastcall SKILLS_SrvSt47_Jump(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || !pUnit->pDynamicPath || STATES_CheckState(pUnit, STATE_FREEZE))
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    int32_t nX = 0;
    int32_t nY = 0;
    if (pTarget)
    {
        nX = 2 * CLIENTS_GetUnitX(pTarget) - CLIENTS_GetUnitX(pUnit);
        nY = 2 * CLIENTS_GetUnitY(pTarget) - CLIENTS_GetUnitY(pUnit);
    }
    else
    {
        nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
        nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
        SKILLS_SetParam4(pSkill, -1);
    }

    D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
    if (!pRoom)
    {
        return 0;
    }

    if (pTarget)
    {
        D2DamageStrc damage = {};
        damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0);
        if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
        {
            damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
            damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
            if (pSkillsTxtRecord->dwHitClass)
            {
                damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
            }
            damage.dwEnDmgPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
            D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
        }

        SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
        SKILLS_SetParam3(pSkill, pTarget->dwUnitType);
        SKILLS_SetParam4(pSkill, pTarget->dwUnitId);
    }
    else
    {
        SKILLS_SetParam4(pSkill, -1);
    }

    COLLISION_SetMaskWithPattern(pRoom, nX, nY, PATH_GetUnitCollisionPattern(pUnit), 0x100u);
    sub_6FCBDE90(pUnit, 1);
    SKILLS_SetParam1(pSkill, nX);
    SKILLS_SetParam2(pSkill, nY);
    SKILLS_SetFlags(pSkill, 0x80u);
    return 1;
}

//D2Game.0x6FD070F0
int32_t __fastcall SKILLS_SrvDo089_Jump(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_ENDANIM, 0);

    if (pUnit->nActionFrame == 2)
    {
        pUnit->nActionFrame = 0;

        D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, SKILLS_GetParam3(pSkill), SKILLS_GetParam4(pSkill));
        if (pTarget)
        {
            SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
        }
        return 1;
    }

    const uint16_t nSkillFlags = SKILLS_GetFlags(pSkill);
    
    const int32_t nX = SKILLS_GetParam1(pSkill);
    const int32_t nY = SKILLS_GetParam2(pSkill);
    if (!(nSkillFlags & 0x100))
    {
        if (nSkillFlags & 0x80)
        {
            SKILLS_SetFlags(pSkill, 0x101u);
            PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, COLLIDE_NONE);
            PATH_SetFootprintCollisionMask(pUnit->pDynamicPath, COLLIDE_NONE);
            D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nX, nY);
            PATH_SetType(pUnit->pDynamicPath, PATHTYPE_LEAP);
            PATH_SetVelocity(pUnit->pDynamicPath, sub_6FD15500(pUnit), __FILE__, __LINE__);
            D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
        }
        else
        {
            SKILLS_SetFlags(pSkill, 0);
        }
        return 1;
    }

    if (CLIENTS_GetUnitX(pUnit) != nX || CLIENTS_GetUnitY(pUnit) != nY)
    {
        int32_t nFrame = 0;
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            int32_t nBaseId = pUnit->dwClassId;
            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nBaseId);
            if (pMonStatsTxtRecord)
            {
                nBaseId = pMonStatsTxtRecord->nBaseId;
            }

            if (nBaseId == MONSTER_SANDLEAPER1)
            {
                nFrame = 8;
            }
        }
        UNITS_SetAnimationFrame(pUnit, nFrame);

        pUnit->dwFrameCountPrecise = (pUnit->dwFrameCountPrecise & 0xFFFFFF00) + 256;
    }
    else
    {
        SKILLS_SetFlags(pSkill, 0);
        sub_6FCBDE90(pUnit, 0);
        COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pUnit), nX, nY, PATH_GetUnitCollisionPattern(pUnit), 0x100u);
        PATH_SetFootprintCollisionMask(pUnit->pDynamicPath, COLLIDE_MONSTER);
        PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, COLLIDE_MASK_MONSTER_PATH);
        PATH_SetType(pUnit->pDynamicPath, 101); // This is a bug in the original game, this value seems to be used by monmodes but should be converted to PATHTYPE_MON_OTHER_2?

        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            int32_t nBaseId = pUnit->dwClassId;
            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nBaseId);
            if (pMonStatsTxtRecord)
            {
                nBaseId = pMonStatsTxtRecord->nBaseId;
            }

            if (nBaseId == MONSTER_SANDLEAPER1)
            {
                D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, SKILLS_GetParam3(pSkill), SKILLS_GetParam4(pSkill));
                if (pTarget)
                {
                    SKILLS_SetFlags(pSkill, 1);
                    UNITS_SetAnimationFrame(pUnit, 12);
                    pUnit->dwFrameCountPrecise = (pUnit->dwFrameCountPrecise & 0xFFFFFF00) + 256;
                    PATH_RecacheRoomIfNeeded(pUnit->pDynamicPath);
                    PATH_SetNewDistance(pUnit->pDynamicPath, 5u);
                    PATH_SetStepNum(pUnit->pDynamicPath, 1u);
                    PATH_SetTargetUnit(pUnit->pDynamicPath, nullptr);
                    D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, 3 * nX - 2 * CLIENTS_GetUnitX(pTarget), 3 * nY - 2 * CLIENTS_GetUnitY(pTarget));
                    PATH_SetType(pUnit->pDynamicPath, PATHTYPE_KNOCKBACK_SERVER);
                    D2COMMON_10190_PATH_SetDistance(pUnit->pDynamicPath, 5u);
                    D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
                }
            }
        }
    }

    return 1;
}

//D2Game.0x6FD074B0
int32_t __fastcall SKILLS_SrvSt48_SwarmMove(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!SUNIT_GetTargetUnit(pGame, pUnit) || !pUnit->pDynamicPath)
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    PATH_SetNewDistance(pUnit->pDynamicPath, 5u);

    PATH_SetType(pUnit->pDynamicPath, PATHTYPE_TOWARD);
    D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
    if (PATH_GetNumberOfPathPoints(pUnit->pDynamicPath))
    {
        SKILLS_SetFlags(pSkill, 1);
        return 1;
    }
    
    PATH_SetType(pUnit->pDynamicPath, PATHTYPE_ASTAR);
    D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
    if (PATH_GetNumberOfPathPoints(pUnit->pDynamicPath))
    {
        SKILLS_SetFlags(pSkill, 1);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD07540
int32_t __fastcall SKILLS_SrvDo090_SwarmMove(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    if (SKILLS_GetFlags(pSkill) & 2)
    {
        SKILLS_SetFlags(pSkill, 0);

        UNITS_SetAnimationFrame(pUnit, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel));
    }
    else
    {
        UNITS_SetAnimationFrame(pUnit, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel));
    }

    return 1;
}

//D2Game.0x6FD075E0
int32_t __fastcall SKILLS_SrvSt49_Nest_EvilHutSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    int32_t nX = 0;
    int32_t nY = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 1, nSkillId, nSkillLevel, &nSpawnMode, &nX, &nY);
    if (nSummonId < 0)
    {
        return 0;
    }

    SKILLS_SetParam1(pSkill, nSummonId);
    SKILLS_SetParam2(pSkill, nX);
    SKILLS_SetParam3(pSkill, nY);
    SKILLS_SetParam4(pSkill, nSpawnMode);

    D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);

    COLLISION_SetMaskWithPattern(pRoom, nX, nY, 1, 0x100u);
    sub_6FCBDE90(pUnit, 1);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_ENDANIM, 0);
    return 1;
}

//D2Game.0x6FD07770
int32_t __fastcall SKILLS_SrvDo091_Nest_EvilHutSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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
    
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_ENDANIM, 0);
    sub_6FCBDE90(pUnit, 0);
    const int32_t nMonsterId = SKILLS_GetParam1(pSkill);
    const int32_t nX = SKILLS_GetParam2(pSkill);
    const int32_t nY = SKILLS_GetParam3(pSkill);
    const int32_t nAnimMode = SKILLS_GetParam4(pSkill);

    D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
    if (!pRoom || !nX || !nY)
    {
        return 0;
    }
    
    COLLISION_ResetMaskWithPattern(pRoom, nX, nY, 1, 0x100u);

    D2UnitStrc* pSummon = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, nAnimMode, 1, 0);
    if (!pSummon)
    {
        return 0;
    }

    pSummon->dwFlags |= UNITFLAG_NOXP | UNITFLAG_NOTC;

    if (pSkillsTxtRecord->wSumOverlay >= 0 && pSkillsTxtRecord->wSumOverlay < sgptDataTables->nOverlayTxtRecordCount)
    {
        UNITS_SetOverlay(pSummon, pSkillsTxtRecord->wSumOverlay, 0);
    }

    return 1;
}

//D2Game.0x6FD07920
int32_t __fastcall SKILLS_SrvSt50_QuickStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        D2DamageStrc damage = {};
        SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD07970
int32_t __fastcall SKILLS_SrvDo092_QuickStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
    SUNITDMG_FreeAttackerDefenderCombatList(pGame, pUnit, pTarget);
    
    int32_t nMissileId = pSkillsTxtRecord->wSrvMissileA;
    if (nMissileId < 0)
    {
        if (pUnit && pUnit->dwUnitType == 1)
        {
            nMissileId = D2Common_11050(pUnit, 1);
        }

        if (nMissileId < 0)
        {
            return 0;
        }
    }

    if (nMissileId < sgptDataTables->nMissilesTxtRecordCount)
    {
        sub_6FD11420(pGame, nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 1);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD07A30
int32_t __fastcall SKILLS_SrvDo093_GargoyleTrap(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        return 0;
    }

    const int32_t nTargetX = CLIENTS_GetUnitX(pTarget);
    const int32_t nTargetY = CLIENTS_GetUnitY(pTarget);
    const int32_t nUnitX = CLIENTS_GetUnitX(pUnit);
    const int32_t nUnitY = CLIENTS_GetUnitY(pUnit);

    int32_t nXDiff = nTargetX - nUnitX;
    nXDiff = std::abs(nXDiff);

    int32_t nYDiff = nTargetY - nUnitY;
    nYDiff = std::abs(nYDiff);

    int32_t nX = nTargetX;
    int32_t nY = nTargetY;
    if (nXDiff < nYDiff)
    {
        nX = nUnitX;
        for (int32_t i = 0; i < 4; ++i)
        {
            if (nX < nTargetX)
            {
                ++nX;
            }
            else if (nX > nTargetX)
            {
                --nX;
            }
        }
    }
    else
    {
        nY = nUnitY;
        for (int32_t i = 0; i < 4; ++i)
        {
            if (nY < nTargetY)
            {
                ++nY;
            }
            else if (nY > nTargetY)
            {
                --nY;
            }
        }
    }

    D2MissileStrc missileParams = {};
    missileParams.pOwner = pUnit;
    missileParams.nTargetY = nY - nUnitY;
    missileParams.dwFlags = 3;
    missileParams.nTargetX = nX - nUnitX;
    missileParams.nX = (nX - nUnitX) / 6 + nUnitX - 1;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nY = (nY - nUnitY) / 6 + nUnitY - 1;
    missileParams.nSkill = nSkillId;
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;
    MISSILES_CreateMissileFromParams(pGame, &missileParams);

    return 1;
}

//D2Game.0x6FD07C50
int32_t __fastcall SKILLS_SrvSt51_Submerge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit)
    {
        pUnit->dwFlags &= 0xFFFFFFF1;
    }

    return 1;
}

//D2Game.0x6FD07C70
int32_t __fastcall SKILLS_SrvDo094_Submerge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t pFrameNo; // [sp+Ch] [bp-8h]@1
    int32_t pFrameCount; // [sp+10h] [bp-4h]@1

    UNITS_GetShiftedFrameMetrics(pUnit, &pFrameNo, &pFrameCount);
    pUnit->nActionFrame = 0;
    if (pFrameNo <= 0)
    {
        pUnit->dwSeqSpeed = 0;
    }

    return 1;
}

//D2Game.0x6FD07CB0
int32_t __fastcall SKILLS_SrvSt52_Emerge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit)
    {
        pUnit->dwFlags |= 0x0E;
    }

    return 1;
}

//D2Game.0x6FD07CD0
int32_t __fastcall SKILLS_SrvSt53_MonInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    int32_t nDelay = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
    if (nDelay < 1)
    {
        nDelay = 1;
    }

    SKILLS_SetParam1(pSkill, nDelay + pGame->dwGameFrame);

    if (!STATES_CheckState(pUnit, STATE_INFERNO))
    {
        STATES_ToggleState(pUnit, STATE_INFERNO, 1);
    }

    return 1;
}

//D2Game.0x6FD07D80
void __fastcall SKILLS_SetInfernoFrame(D2SkillsTxt* pSkillsTxtRecord, D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
        if (pMonStats2TxtRecord)
        {
            if (pSkillsTxtRecord->nMonAnim == MONMODE_SEQUENCE)
            {
                UNITS_SetAnimationFrame(pUnit, pMonStats2TxtRecord->nInfernoAnim - 1);
                pUnit->dwFrameCountPrecise = (pMonStats2TxtRecord->nInfernoAnim + 1) << 8;
            }
            else
            {
                pUnit->nSeqCurrentFramePrecise = pMonStats2TxtRecord->nInfernoAnim << 8;
            }
        }
    }
    else
    {
        UNITS_SetAnimationFrame(pUnit, 8);
        pUnit->dwFrameCountPrecise = 0x500u;
    }
}

//D2Game.0x6FD07E30
int32_t __fastcall SKILLS_SrvDo095_MonInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pSkillsTxtRecord->wSrvMissileA);
    if (!pMissilesTxtRecord)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};

    missileParams.dwFlags = 0x25u;
    missileParams.pOwner = pUnit;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;
    missileParams.nVelocity = pMissilesTxtRecord->nVel + nSkillLevel * pMissilesTxtRecord->nVelLev / 8;

    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &missileParams.nTargetX, &missileParams.nTargetY))
    {
        int32_t nAnimLength = 11;
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
            if (pMonStats2TxtRecord)
            {
                nAnimLength = pMonStats2TxtRecord->nInfernoLen;
            }
        }
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_MODECHANGE, 0);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_ENDANIM, pGame->dwGameFrame + nAnimLength, 0, 0);
        return 0;
    }

    D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
    if (!pMissile)
    {
        int32_t nAnimLength = 11;
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
            if (pMonStats2TxtRecord)
            {
                nAnimLength = pMonStats2TxtRecord->nInfernoLen;
            }
        }
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_MODECHANGE, 0);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_ENDANIM, pGame->dwGameFrame + nAnimLength, 0, 0);
        return 0;
    }

    SKILLS_UpdateInfernoAnimationParameters(pUnit, pMissile, nSkillId, nSkillLevel);
    SKILLS_SetInfernoFrame(pSkillsTxtRecord, pUnit);

    if (pGame->dwGameFrame < SKILLS_GetParam1(pSkill) && STATES_CheckState(pUnit, STATE_INFERNO))
    {
        int32_t nAnimLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
        if (nAnimLength < 1)
        {
            nAnimLength = 1;
        }

        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_ENDANIM, 0);
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_MODECHANGE, 0);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_MODECHANGE, pGame->dwGameFrame + nAnimLength, 0, 0);
    }
    else
    {
        int32_t nAnimLength = 11;
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
            if (pMonStats2TxtRecord)
            {
                nAnimLength = pMonStats2TxtRecord->nInfernoLen;
            }
        }
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_MODECHANGE, 0);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_ENDANIM, pGame->dwGameFrame + nAnimLength, 0, 0);
    }

    return 1;
}

//D2Game.0x6FD081E0
int32_t __fastcall SKILLS_UpdateInfernoAnimationParameters(D2UnitStrc* pUnit, D2UnitStrc* pMissile, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !pMissile)
    {
        return 0;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 0;
    }

    int32_t nFrame = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nFrame <= 0)
    {
        nFrame = pMissilesTxtRecord->dwParam[1] + nSkillLevel - 1;
    }

    nFrame = D2Clamp(nFrame, 1, 255);

    PATH_SetNewDistance(pMissile->pDynamicPath, nFrame);
    UNITS_SetAnimationSpeed(pMissile, (pMissilesTxtRecord->nAnimLen << 8) / nFrame);
    MISSILE_SetTotalFrames(pMissile, nFrame);
    MISSILE_SetCurrentFrame(pMissile, nFrame);

    return 1;
}

//D2Game.0x6FD082D0
int32_t __fastcall SKILLS_SrvDo152_DiabLight(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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
    
    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pSkillsTxtRecord->wSrvMissileA);
    if (!pMissilesTxtRecord)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x25u;
    missileParams.pOwner = pUnit;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;
    missileParams.nVelocity = pMissilesTxtRecord->nVel + nSkillLevel * pMissilesTxtRecord->nVelLev / 8;

    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &missileParams.nTargetX, &missileParams.nTargetY))
    {
        int32_t nAnimLength = 11;
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
            if (pMonStats2TxtRecord)
            {
                nAnimLength = pMonStats2TxtRecord->nInfernoLen;
            }
        }
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_MODECHANGE, 0);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_ENDANIM, pGame->dwGameFrame + nAnimLength, 0, 0);
        return 0;
    }

    D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
    if (!pMissile)
    {
        int32_t nAnimLength = 11;
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
            if (pMonStats2TxtRecord)
            {
                nAnimLength = pMonStats2TxtRecord->nInfernoLen;
            }
        }
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_MODECHANGE, 0);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_ENDANIM, pGame->dwGameFrame + nAnimLength, 0, 0);
        return 0;
    }

    SKILLS_UpdateInfernoAnimationParameters(pUnit, pMissile, nSkillId, nSkillLevel);
    SKILLS_SetInfernoFrame(pSkillsTxtRecord, pUnit);

    if (pGame->dwGameFrame < SKILLS_GetParam1(pSkill) && STATES_CheckState(pUnit, STATE_INFERNO))
    {
        int32_t nAnimLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
        if (nAnimLength < 1)
        {
            nAnimLength = 1;
        }

        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_ENDANIM, 0);
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_MODECHANGE, 0);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_MODECHANGE, pGame->dwGameFrame + nAnimLength, 0, 0);
    }
    else
    {
        int32_t nAnimLength = 11;
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
            if (pMonStats2TxtRecord)
            {
                nAnimLength = pMonStats2TxtRecord->nInfernoLen;
            }
        }

        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_MODECHANGE, 0);
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_ENDANIM, pGame->dwGameFrame + nAnimLength, 0, 0);
    }

    return 1;
}

//D2Game.0x6FD08680
int32_t __fastcall SKILLS_SrvDo096_ZakarumHeal_Bestow(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    int32_t nMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
    if (nMax < 0)
    {
        nMax = 0;
    }
    else if (nMax > 100)
    {
        nMax = 100;
    }

    int32_t nMin = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    if (nMin < 0)
    {
        nMin = 0;
    }
    else if (nMin > nMax)
    {
        nMin = nMax;
    }

    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pTarget);
    const int32_t nPercentage = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMax - nMin) + nMin;
    int32_t nNewHp = MONSTERUNIQUE_CalculatePercentage(nMaxHp, nPercentage, 100) + STATLIST_UnitGetStatValue(pTarget, STAT_HITPOINTS, 0);
    if (nNewHp < 1)
    {
        nNewHp = 1;
    }
    else if (nNewHp > nMaxHp)
    {
        nNewHp = nMaxHp;
    }

    STATLIST_SetUnitStat(pTarget, STAT_HITPOINTS, nNewHp, 0);

    return 1;
}

//D2Game.0x6FD08850
int32_t __fastcall SKILLS_ResurrectUnit(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return 0;
    }

    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, STATLIST_UnitGetStatValue(pUnit, STAT_MAXHP, 0), 0);

    if (pUnit)
    {
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            pUnit->dwFlags |= UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET | UNITFLAG_NOTC | UNITFLAG_NOXP;

            MONSTER_UpdateAiCallbackEvent(pGame, pUnit);
            sub_6FC68280(pGame->pMonReg, pUnit);
            QUESTS_AttachLevelChainRecord(pGame, pUnit, pRoom, 0);
            PATH_SetFootprintCollisionMask(pUnit->pDynamicPath, COLLIDE_MONSTER);
            D2Common_10214(pUnit);
            COLLISION_SetMaskWithPattern(pRoom, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), PATH_GetUnitCollisionPattern(pUnit), 0x100);
        }
        else
        {
            pUnit->dwFlags |= UNITFLAG_TARGETABLE | UNITFLAG_CANBEATTACKED | UNITFLAG_ISVALIDTARGET;

            PATH_SetFootprintCollisionMask(pUnit->pDynamicPath, COLLIDE_PLAYER);
            D2Common_10214(pUnit);
            COLLISION_SetMaskWithPattern(pRoom, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), PATH_GetUnitCollisionPattern(pUnit), 0x80);
        }
    }

    return 1;
}

//D2Game.0x6FD089E0
int32_t __fastcall SKILLS_SrvDo097_Resurrect(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    if (!MONSTERS_IsDead(pTarget) || STATES_CheckStateMaskHideOnUnit(pTarget))
    {
        return 0;
    }

    const int32_t nX = CLIENTS_GetUnitX(pTarget);
    const int32_t nY = CLIENTS_GetUnitY(pTarget);

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pTarget);
    if (pRoom && !COLLISION_CheckAnyCollisionWithPattern(pRoom, nX, nY, PATH_GetUnitCollisionPattern(pTarget), pTarget->dwUnitType != UNIT_PLAYER ? COLLIDE_MASK_MONSTER_PATH : 0x1C09u))
    {
        PATH_SetTargetUnit(pUnit->pDynamicPath, pTarget);
        SKILLS_ResurrectUnit(pGame, pTarget);

        if (pTarget->dwUnitType == UNIT_MONSTER)
        {
            const int32_t nMode = MONSTERSPAWN_GetResurrectMode(pTarget, 1);
            D2ModeChangeStrc modeChange = {};
            MONSTERMODE_GetModeChangeInfo(pTarget, nMode, &modeChange);
            MONSTERSPAWN_GetResurrectMode(pTarget, 1);
            D2GAME_ModeChange_6FC65220(pGame, &modeChange, nMode != MONMODE_SEQUENCE);
        }

        if (pSkillsTxtRecord->wSrvOverlay >= 0 && pSkillsTxtRecord->wSrvOverlay < sgptDataTables->nOverlayTxtRecordCount)
        {
            UNITS_SetOverlay(pTarget, pSkillsTxtRecord->wSrvOverlay, 0);
        }

        return 1;
    }

    return 0;
}

//D2Game.0x6FD08BB0
int32_t __fastcall SKILLS_SrvDo098_MonTeleport(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    const int32_t nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
    const int32_t nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);

    D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
    if (!pRoom)
    {
        D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
        if (pOwner)
        {
            pRoom = UNITS_GetRoom(pOwner);
        }
    }

    if (sub_6FCBDFE0(pGame, pUnit, pRoom, nX, nY, 0, 0))
    {
        UNITROOM_RefreshUnit(pUnit);
        pUnit->dwFlagEx |= UNITFLAGEX_TELEPORTED;
        return 1;
    }

    return 0;
}

//D2Game.0x6FD08CD0
int32_t __fastcall SKILLS_SrvDo099_PrimePoisonNova(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    constexpr int32_t dword_6FD29720[] =
    {
        0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1
    };

    constexpr int32_t dword_6FD29760[] =
    {
        2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2, 2
    };

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

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x1F;
    missileParams.pOwner = pUnit;
    missileParams.pOrigin = pUnit;
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nLoops = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    missileParams.nVelocity = pMissilesTxtRecord->dwParam[0] << 6;

    int32_t nParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
    if (nParam < 1)
    {
        nParam = 1;
    }

    for (int32_t i = 0; i < 16; i+= 2)
    {
        missileParams.nTargetX = dword_6FD29720[i];
        missileParams.nTargetY = dword_6FD29760[i];
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    missileParams.nVelocity = pMissilesTxtRecord->dwParam[1] << 6;
    if (nParam > 1)
    {
        for (int32_t i = 0; i < 15; i += nParam)
        {
            missileParams.nTargetX = dword_6FD29720[i + 1];
            missileParams.nTargetY = dword_6FD29760[i + 1];
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
        }
    }

    return 1;
}

//D2Game.0x6FD08EB0
int32_t __fastcall SKILLS_SrvDo100_DiabCold(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        D2DamageStrc damage = {};

        damage.wResultFlags |= pSkillsTxtRecord->wResultFlags | 1;
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
        if (pSkillsTxtRecord->dwHitClass)
        {
            damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
        }

        damage.dwFrzLen = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
        D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
        SUNITDMG_ExecuteEvents(pGame, pUnit, pTarget, 1, &damage);
        if (!STATLIST_UnitGetStatValue(pTarget, 6, 0))
        {
            damage.wResultFlags |= 2u;
        }

        SUNITDMG_ExecuteMissileDamage(pGame, pUnit, pTarget, &damage);

        if (pSkillsTxtRecord->wSrvOverlay > 0 && pSkillsTxtRecord->wSrvOverlay <= sgptDataTables->nOverlayTxtRecordCount)
        {
            UNITS_SetOverlay(pTarget, pSkillsTxtRecord->wSrvOverlay, 0);
        }
    }

    return 1;
}

//D2Game.0x6FD09000
int32_t __fastcall SKILLS_SrvDo101_FingerMageSpider(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (!SKILLS_GetMissilesTxtRecord(pSkillsTxtRecord->wSrvMissileA))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 3;
    missileParams.pOwner = pUnit;
    missileParams.pOrigin = pUnit;
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nLoops = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        const int32_t nX = CLIENTS_GetUnitX(pTarget);
        const int32_t nY = CLIENTS_GetUnitY(pTarget);
        D2Common_10216(pUnit->pDynamicPath, nX, nY, 0);

        missileParams.nTargetX = nX - missileParams.nX;
        missileParams.nTargetY = nY - missileParams.nY;
    }
    else
    {
        missileParams.nTargetX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath) - missileParams.nX;
        missileParams.nTargetY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath) - missileParams.nY;
    }

    MISSILES_CreateMissileFromParams(pGame, &missileParams);

    return 1;
}

//D2Game.0x6FD092B0
int32_t __fastcall SKILLS_SrvDo102_DiabWall(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    const int32_t nSubMissiles = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x21;
    missileParams.pOwner = pUnit;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.pInitFunc = SKILLS_MissileInit_DiabWall;

    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &missileParams.nTargetX, &missileParams.nTargetY))
    {
        return 0;
    }

    for (int32_t i = 0; i < nSubMissiles; ++i)
    {
        missileParams.pInitArgs = i;
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return 1;
}

//D2Game.0x6FD09410
void __fastcall SKILLS_MissileInit_DiabWall(D2UnitStrc* pMissile, int32_t nInitSeed)
{
    if (pMissile)
    {
        int32_t nFrames = MISSILE_GetTotalFrames(pMissile);
        if (nFrames >= 78)
        {
            nFrames = 77;
            MISSILE_SetTotalFrames(pMissile, 77);
            MISSILE_SetCurrentFrame(pMissile, 77);
        }

        SEED_InitLowSeed(&pMissile->pSeed, nInitSeed + D2COMMON_10175_PathGetFirstPointX(pMissile->pDynamicPath));

        if ((ITEMS_RollRandomNumber(&pMissile->pSeed) % 100) >= 20)
        {
            PATH_SetType(pMissile->pDynamicPath, PATHTYPE_CHARGEDBOLT);
            PATH_SetNewDistance(pMissile->pDynamicPath, nFrames);
            D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
        }
    }
}

//D2Game.0x6FD094B0
int32_t __fastcall SKILLS_SrvSt54_DiabRun(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (pSkill)
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (pTarget)
        {
            SKILLS_SetFlags(pSkill, 0);
            SKILLS_SetParam1(pSkill, pTarget->dwUnitId);
            SKILLS_SetParam2(pSkill, pTarget->dwUnitType);
            PATH_SetTargetUnit(pUnit->pDynamicPath, 0);
            D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget));
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD09570
int32_t __fastcall SKILLS_SrvDo103_DiabRun(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    if (SKILLS_GetFlags(pSkill) & 2)
    {
        SKILLS_SetFlags(pSkill, 0);
        pUnit->dwFrameCountPrecise = pSkillsTxtRecord->dwParam[0] << 8;
        UNITS_SetAnimationFrame(pUnit, pSkillsTxtRecord->dwParam[1]);
        return 1;
    }

    int32_t nFrame = 0;
    int32_t nFrameCount = 0;
    UNITS_GetShiftedFrameMetrics(pUnit, &nFrame, &nFrameCount);

    if (nFrame == pSkillsTxtRecord->dwParam[2])
    {
        if (!pUnit->pDynamicPath)
        {
            return 0;
        }

        int32_t nBaseVelocity = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel) << 8;
        if (nBaseVelocity <= 256)
        {
            nBaseVelocity = 256;
        }

        PATH_SetVelocity(pUnit->pDynamicPath, MONSTERUNIQUE_CalculatePercentage(nBaseVelocity, STATLIST_UnitGetStatValue(pUnit, STAT_VELOCITYPERCENT, 0), 100), __FILE__, __LINE__);
        PATH_SetType(pUnit->pDynamicPath, PATHTYPE_ASTAR);
        D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
        SKILLS_SetFlags(pSkill, 1);
        return 1;
    }

    if (nFrame == pSkillsTxtRecord->dwParam[3])
    {
        pUnit->dwFrameCountPrecise = pSkillsTxtRecord->dwParam[4] << 8;
        UNITS_SetAnimationFrame(pUnit, pSkillsTxtRecord->dwParam[5]);
        return 1;
    }

    if (pUnit->nActionFrame == 1)
    {
        D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, SKILLS_GetParam2(pSkill), SKILLS_GetParam1(pSkill));
        if (pTarget)
        {
            if (UNITS_IsInMeleeRange(pUnit, pTarget, 0))
            {
                D2DamageStrc damage = {};
                pUnit->nActionFrame = 0;
                sub_6FD155E0(pGame, pUnit, pTarget, pSkillsTxtRecord, nSkillId, nSkillLevel, &damage, 0);

                if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
                {
                    damage.dwHitFlags = pSkillsTxtRecord->dwHitFlags | 0x20;
                    if (pSkillsTxtRecord->dwHitClass)
                    {
                        damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
                    }

                    D2GAME_RollPhysicalDamage_6FD14EC0(pUnit, &damage, nSkillId, nSkillLevel);
                    D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
                    SUNITDMG_ExecuteEvents(pGame, pUnit, pTarget, 1, &damage);

                    if (!STATLIST_UnitGetStatValue(pTarget, STAT_HITPOINTS, 0))
                    {
                        damage.wResultFlags |= 2u;
                    }

                    SUNITDMG_ExecuteMissileDamage(pGame, pUnit, pTarget, &damage);
                }
            }
        }
    }

    return 1;
}

//D2Game.0x6FD09890
int32_t __fastcall SKILLS_SrvDo104_DiabPrison(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        return 0;
    }
    
    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, nullptr, nullptr);
    if (nSummonId < 0)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget)
    {
        if (!pUnit->pDynamicPath || D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath) != 2)
        {
            return 0;
        }
                
        D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath));
        if (!pObject)
        {
            return 0;
        }

        pTarget = pObject;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pTarget);
    if (pRoom && !DUNGEON_IsRoomInTown(pRoom))
    {
        sub_6FC6A810(pGame, pRoom, 0, 0, pTarget, nSummonId, 0);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD09970
int32_t __fastcall SKILLS_SrvDo105_DesertTurret(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    const int32_t nX = CLIENTS_GetUnitX(pUnit);
    const int32_t nY = CLIENTS_GetUnitY(pUnit);
    const int32_t nXDiff = nTargetX - nX;
    const int32_t nYDiff = nTargetY - nY;
    if (!nXDiff && !nYDiff)
    {
        return 0;
    }

    int32_t nBaseX = 0;
    int32_t nBaseY = 0;

    if (nYDiff < 0)
    {
        nBaseX = 2;
    }
    else if (nYDiff > 0)
    {
        nBaseX = -2;
    }

    if (nXDiff < 0)
    {
        nBaseY = -2;
    }
    else if (nXDiff > 0)
    {
        nBaseY = 2;
    }

    D2MissileStrc missileParams = {};
    missileParams.pOwner = pUnit;
    missileParams.dwFlags = 3;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;

    const int32_t nSubMissiles = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    int32_t nOffsetX = nXDiff - nBaseX * nSubMissiles / 2;
    int32_t nOffsetY = nYDiff - nBaseY * nSubMissiles / 2;

    for (int32_t i = 0; i < nSubMissiles; ++i)
    {
        missileParams.nX = nX + nOffsetX / 6;
        missileParams.nY = nY + nOffsetX / 6; // TODO: Maybe nOffsetY?
        missileParams.nTargetX = nOffsetX;
        missileParams.nTargetY = nOffsetY;
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
        nOffsetX += nBaseX;
        nOffsetY += nBaseY;
    }

    return 1;
}

//D2Game.0x6FD09BF0
int32_t __fastcall SKILLS_SrvDo106_ArcaneTower(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    if (!SUNIT_GetTargetUnit(pGame, pUnit))
    {
        return 0;
    }

    sub_6FD14170(pGame, pUnit, pUnit, pSkillsTxtRecord->wSrvMissileA, nSkillId, nSkillLevel, DATATBLS_GetMissileVelocityFromMissilesTxt(pSkillsTxtRecord->wSrvMissileA, nSkillLevel));
    return 1;
}

//D2Game.0x6FD09C90
int32_t __fastcall SKILLS_SrvSt55_Mosquito(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2SeedStrc seed = {};
    SEED_InitSeed(&seed);
    int32_t nUnitGUID = -1;
    if (pUnit)
    {
        nUnitGUID = pUnit->dwUnitId;
    }
    SEED_InitLowSeed(&seed, pTarget->dwUnitId + nUnitGUID);

    const int32_t nMin = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    int32_t nParam = nMin + ITEMS_RollLimitedRandomNumber(&seed, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel) - nMin);
    if (nParam < 1)
    {
        nParam = 1;
    }

    SKILLS_SetParam1(pSkill, nParam);
    SKILLS_SetParam2(pSkill, pTarget->dwUnitId);
    SKILLS_SetParam3(pSkill, pTarget->dwUnitType);

    return 1;
}

//D2Game.0x6FD09DE0
int32_t __fastcall SKILLS_SrvDo107_Mosquito(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
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

    D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, SKILLS_GetParam3(pSkill), SKILLS_GetParam2(pSkill));
    if (!pTarget || !UNITS_IsInMeleeRange(pUnit, pTarget, 0))
    {
        return 0;
    }

    const int32_t nParam = SKILLS_GetParam1(pSkill);

    D2DamageStrc damage = {};

    const int32_t nMinDamage = SKILLS_GetMinPhysDamage(pUnit, nSkillId, nSkillLevel, 1) >> 8;
    const int32_t nMaxDamage = SKILLS_GetMaxPhysDamage(pUnit, nSkillId, nSkillLevel, 1) >> 8;
    D2GAME_RollPhysicalDamage_6FD14EC0(pUnit, &damage, nSkillId, nSkillLevel);
    D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
    damage.dwPoisDamage = 2 * (nMinDamage + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDamage - nMinDamage));
    damage.dwPoisLen = SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);
    damage.dwManaLeech = (nMinDamage + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDamage - nMinDamage)) << 8;
    damage.dwStamLeech = (nMinDamage + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDamage - nMinDamage)) << 8;
    damage.wResultFlags = 1;

    const int32_t nPercentage = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
    int32_t nNewHp = MONSTERUNIQUE_CalculatePercentage(damage.dwPhysDamage, nPercentage, 100) + STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, STAT_STRENGTH);
    if (nNewHp > nMaxHp)
    {
        nNewHp = nMaxHp;
    }
    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);

    SUNITDMG_ExecuteEvents(pGame, pUnit, pTarget, 1, &damage);
    if (!STATLIST_UnitGetStatValue(pTarget, STAT_HITPOINTS, STAT_STRENGTH))
    {
        damage.wResultFlags |= 2u;
    }

    SUNITDMG_ExecuteMissileDamage(pGame, pUnit, pTarget, &damage);
    if (nParam - 1 > 0)
    {
        SKILLS_SetParam1(pSkill, nParam - 1);
        UNITS_SetAnimationFrame(pUnit, pSkillsTxtRecord->dwParam[0]);
        pUnit->dwFrameCountPrecise = (pUnit->dwFrameCountPrecise & 0xFFFFFF00) + 256;
    }

    return 1;
}

//D2Game.0x6FD0A190
int32_t __fastcall SKILLS_SrvDo108_RegurgitatorEat(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget && pTarget->dwUnitType == UNIT_MONSTER && !MONSTERS_GetHirelingTypeId(pTarget) && pTarget->dwAnimMode == MONMODE_DEAD)
    {
        const int32_t nTargetMaxHp = STATLIST_GetMaxLifeFromUnit(pTarget);
        DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pTarget), UNIT_MONSTER, pTarget->dwUnitId);
        SUNIT_RemoveUnit(pGame, pTarget);

        const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
        if (nTargetMaxHp > 0)
        {
            int32_t nNewHp = MONSTERUNIQUE_CalculatePercentage(nTargetMaxHp, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel), 100) + nHitpoints;
            if (nNewHp < 1)
            {
                nNewHp = 1;
            }
            else if (nNewHp > nMaxHp)
            {
                nNewHp = nMaxHp;
            }

            STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);
        }
        
        return 1;
    }

    return 0;
}

//D2Game.0x6FD0A340
int32_t __fastcall SKILLS_GetMonFrenzySequenceFrame(D2UnitStrc* pUnit)
{
    if (DATATBLS_GetMonSeqTxtRecordFromUnit(pUnit))
    {
        const int32_t nMax = DATATBLS_GetSeqFramePointsCount(pUnit) >> 8;
        const int32_t nCurrent = pUnit->dwSeqFrame >> 8;
        for (int32_t i = nCurrent + 1; i < nMax; ++i)
        {
            int32_t nEvent = 0;
            DATATBLS_GetSequenceEvent((D2AnimSeqTxt*)pUnit->pAnimSeq, nCurrent << 8, &nEvent);
            if (nEvent)
            {
                return nEvent;
            }
        }
    }
    else
    {
        const int32_t nMax = pUnit->dwFrameCountPrecise >> 8;
        const int32_t nCurrent = pUnit->nSeqCurrentFramePrecise >> 8;
        if (nMax > pUnit->pAnimData->dwFrames || nCurrent + 1 >= nMax)
        {
            return 0;
        }

        // TODO: Why is this inside a loop?
        for (int32_t i = 0; i < nMax; ++i)
        {
            if (pUnit->pAnimData->pFrameFlags[nCurrent])
            {
                return pUnit->pAnimData->pFrameFlags[nCurrent];
            }
        }
    }

    return 0;
}

//D2Game.0x6FD0A3D0
int32_t __fastcall SKILLS_RollMonFrenzyDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !pTargetUnit)
    {
        return 0;
    }

    UNITS_SetTargetUnitForDynamicUnit(pUnit, pTargetUnit);

    D2DamageStrc damage = {};

    damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTargetUnit, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0);
    if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        damage.wResultFlags |= pSkillsTxtRecord->wResultFlags;
        damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;

        if (pSkillsTxtRecord->dwHitClass)
        {
            damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
        }

        damage.dwEnDmgPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
    }

    SUNITDMG_AllocCombat(pGame, pUnit, pTargetUnit, &damage, 128);
    SUNITDMG_DrainItemDurability(pGame, pUnit, pTargetUnit, 0);

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (pSkill)
    {
        if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
        {
            SKILLS_SetParam1(pSkill, 1);
            return 1;
        }

        SKILLS_SetParam1(pSkill, 0);
    }

    return 1;
}

//D2Game.0x6FD0A520
int32_t __fastcall SKILLS_SrvSt25_64_DragonClaw_MonFrenzy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    return SUNIT_GetTargetUnit(pGame, pUnit) != 0;
}

//D2Game.0x6FD0A530
int32_t __fastcall SKILLS_SrvDo109_MonFrenzy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (pSkill)
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (pTarget)
        {
            SUNITDMG_FreeAttackerDefenderCombatList(pGame, pUnit, pTarget);

            if (pUnit)
            {
                pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
            }

            if (SKILLS_GetMonFrenzySequenceFrame(pUnit))
            {
                sub_6FD12950(pGame, pUnit, nSkillId, nSkillLevel, 0);
            }

            SKILLS_RollMonFrenzyDamage(pGame, pUnit, pTarget, nSkillId, nSkillLevel);

            if (SKILLS_GetParam1(pSkill))
            {
                SKILLS_ApplyFrenzyStats(pGame, pUnit, nSkillId, nSkillLevel);
            }

            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FD0A5E0
int32_t __fastcall SKILLS_SrvDo148_DoomKnightMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !SUNIT_GetTargetUnit(pGame, pUnit))
    {
        return 0;
    }
    
    int32_t nMissileId = pSkillsTxtRecord->wSrvMissileA;
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        nMissileId += pUnit->pMonsterData->nComponent[10];
    }

    if (SKILLS_GetMissilesTxtRecord(nMissileId))
    {
        if (pUnit)
        {
            pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
        }

        if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[1])
        {
            sub_6FD11710(pGame, nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
        }
        else
        {
            sub_6FD11420(pGame, nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
        }

        return 1;
    }

    return 0;
}

//D2Game.0x6FD0A720
int32_t __fastcall SKILLS_SrvDo149_NecromageMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{    
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !SUNIT_GetTargetUnit(pGame, pUnit))
    {
        return 0;
    }
    
    int32_t nMissileId = pSkillsTxtRecord->wSrvMissileA;
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        nMissileId += pUnit->pMonsterData->nComponent[11];
    }

    if (SKILLS_GetMissilesTxtRecord(nMissileId))
    {
        if (pUnit)
        {
            pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
        }

        if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[1])
        {
            sub_6FD11710(pGame, nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
        }
        else
        {
            sub_6FD11420(pGame, nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
        }

        return 1;
    }

    return 0;
}

//D2Game.0x6FD0A860
int32_t __fastcall SKILLS_SrvDo110_Hireable_RogueMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !SUNIT_GetTargetUnit(pGame, pUnit))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t nMissileId = D2COMMON_11039_CheckWeaponIsMissileBased(pUnit, &nSkillLevel);
    if (nMissileId && nMissileId != MISSILE_BOLT)
    {
        if (nMissileId >= 0 && nMissileId < sgptDataTables->nMissilesTxtRecordCount && &sgptDataTables->pMissilesTxt[nMissileId])
        {
            sub_6FD11420(pGame, nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
            return 1;
        }

        return 0;
    }

    nMissileId = pSkillsTxtRecord->wSrvMissileA;
    if (nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount || !&sgptDataTables->pMissilesTxt[nMissileId])
    {
        return 0;
    }

    if (!(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[1]))
    {
        sub_6FD11420(pGame, nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
        return 1;
    }

    sub_6FD11710(pGame, nMissileId, pUnit, nSkillId, nSkillLevel, 0, 0, 0, 0, 0);
    return 1;
}

//D2Game.0x6FD0A9B0
int32_t __fastcall SKILLS_SrvDo112_MonCurseCast(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    constexpr int32_t monCurseIds[] = {
        SKILL_AMPLIFYDAMAGE, SKILL_WEAKEN, SKILL_IRONMAIDEN, SKILL_LIFETAP, SKILL_DECREPIFY, SKILL_LOWERRESIST
    };

    const uint32_t nIndex = ITEMS_RollRandomNumber(&pUnit->pSeed) % std::size(monCurseIds);
    const int32_t nCurseSkillId = monCurseIds[nIndex];

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nCurseSkillId, nSkillLevel);

    D2UnkNecSkillStrc args = {};
    args.pGame = pGame;
    args.pUnit = pUnit;
    args.unk0x08 = 0;
    args.nSkillId = nCurseSkillId;
    args.nSkillLevel = nSkillLevel;
    args.nDuration = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nCurseSkillId, nSkillLevel);
    args.bUpdateAnimRate = 0;
    args.nAuraTargetState = pSkillsTxtRecord->wAuraTargetState;

    for (int32_t i = 0; i < 6; ++i)
    {
        args.nAuraStat[i] = -1;
        args.nAuraStatCalc[i] = 0;
    }

    switch (nIndex)
    {
    case 0u:
    {
        args.nAuraStat[0] = STAT_DAMAGERESIST;
        args.nAuraStatCalc[0] = -100;
        break;
    }
    case 1u:
    {
        args.nAuraStat[0] = STAT_DAMAGEPERCENT;
        args.nAuraStatCalc[0] = -50;
        break;
    }
    case 2u:
    {
        args.nAuraEvent[0] = 5;
        args.nAuraEventFunc[0] = 4;
        break;
    }
    case 3u:
    {
        args.nAuraEvent[0] = 1;
        args.nAuraEventFunc[0] = 5;
        args.nAuraEvent[1] = 2;
        args.nAuraEventFunc[1] = 5;
        break;
    }
    case 4u:
    {
        args.bUpdateAnimRate = 1;
        args.nAuraStat[0] = STAT_VELOCITYPERCENT;
        args.nAuraStatCalc[0] = pSkillsTxtRecord->dwParam[6];
        args.nAuraStat[1] = STAT_DAMAGEPERCENT;
        args.nAuraStatCalc[1] = -50;
        args.nAuraStat[2] = STAT_DAMAGERESIST;
        args.nAuraStatCalc[2] = -50;
        args.nAuraStat[3] = STAT_ATTACKRATE;
        args.nAuraStatCalc[3] = pSkillsTxtRecord->dwParam[6];
        break;
    }
    default:
    {
        const int32_t nResSubstraction = D2COMMON_11036_GetMonCurseResistanceSubtraction(nSkillLevel, nCurseSkillId);
        args.nAuraStat[0] = STAT_MAGICRESIST;
        args.nAuraStatCalc[0] = -nResSubstraction;
        args.nAuraStat[1] = STAT_FIRERESIST;
        args.nAuraStatCalc[1] = -nResSubstraction;
        args.nAuraStat[2] = STAT_LIGHTRESIST;
        args.nAuraStatCalc[2] = -nResSubstraction;
        args.nAuraStat[3] = STAT_COLDRESIST;
        args.nAuraStatCalc[3] = -nResSubstraction;
        args.nAuraStat[4] = STAT_POISONRESIST;
        args.nAuraStatCalc[4] = -nResSubstraction;
        break;
    }
    }

    return sub_6FD10360(pGame, 3, pUnit, nRange, sub_6FD0B450, &args);
}

//D2Game.0x6FD0AC20
D2UnitStrc* __fastcall SKILLS_CreateSpiderLayMissile(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit->pDynamicPath || !D2Common_10234(pUnit->pDynamicPath))
    {
        return nullptr;
    }

    // TODO: Check this calculation, Names    
    uint8_t v22[8] = { 10, 8, 22, 20, 18, 16, 14, 12, };
    uint32_t a1a = v22[D2Common_11053(PATH_GetNewDirection(pUnit->pDynamicPath))];
    int32_t a2a = 0;
    int32_t a3 = 0;
    D2Common_11055((uint8_t)a1a, &a2a, &a3);

    D2MissileStrc missileParams = {};
    missileParams.nX = CLIENTS_GetUnitY(pUnit) + a2a;
    missileParams.nY = CLIENTS_GetUnitX(pUnit) + a3;
    missileParams.dwFlags = 0x21;
    missileParams.pOwner = pUnit;
    missileParams.nTargetX = missileParams.nX + 2 * a2a;
    missileParams.nTargetY = missileParams.nY + 2 * a3;
    missileParams.nSkillLevel = SKILLS_GetSkillLevel(pUnit, SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, SKILL_SPIDERLAY), 1);
    missileParams.nVelocity = 3;
    missileParams.nSkill = SKILL_SPIDERLAY;
    missileParams.nMissile = MISSILE_SPIDERGOOLAY;

    return MISSILES_CreateMissileFromParams(pGame, &missileParams);
}

//D2Game.0x6FD0AE10
int32_t __fastcall SKILLS_SrvDo111_FetishAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nX = 0;
    int32_t nY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nX, &nY))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    D2UnitFindDataStrc* pUnitFindData = sub_6FD10D90(pGame, pUnit, pSkillsTxtRecord->dwParam[3] + 2 * nSkillLevel, nX, nY);
    for (int32_t i = 0; i < pUnitFindData->nIndex; ++i)
    {
        D2UnitStrc* pFoundUnit = pUnitFindData->pUnitsArray[i];
        if (!sub_6FCBD900(pGame, pUnit, pFoundUnit) && pFoundUnit
            && (pFoundUnit->dwClassId >= MONSTER_FETISHBLOW1 && pFoundUnit->dwClassId <= MONSTER_FETISHBLOW5 || pFoundUnit->dwClassId >= MONSTER_FETISH1 && pFoundUnit->dwClassId <= MONSTER_FETISH5)
            && sub_6FD0B2B0(pGame, pUnit, pFoundUnit))
        {
            sub_6FD11260(pUnit, pFoundUnit, nSkillId, nSkillLevel, STAT_ATTACKRATE, STATE_FETISHAURA, 0);
        }
    }

    sub_6FD10E20(pGame, pUnitFindData);

    return 1;
}
