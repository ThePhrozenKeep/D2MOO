#include "MISSILES/Missiles.h"

#include <algorithm>

#include <D2BitManip.h>

#include <D2PacketDef.h>

#include <DataTbls/MissilesTbls.h>
#include <D2StatList.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2States.h>
#include <D2DataTbls.h>

#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "ITEMS/Items.h"
#include "MISSILES/MissMode.h"
#include "MONSTER/MonsterUnique.h"
#include "PLAYER/Player.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FC552A0
void __fastcall MISSILES_RemoveAll(D2GameStrc* pGame)
{
    for (int32_t i = 0; i < 128; ++i)
    {
        D2UnitStrc* pUnit = pGame->pUnitList[4][i];
        while (pUnit)
        {
            D2UnitStrc* pNext = SUNIT_GetNextUnitFromList(pUnit);
            SUNIT_RemoveUnit(pGame, pUnit);
            pUnit = pNext;
        }
    }
}

//D2Game.0x6FC552F0
void __fastcall MISSILES_Initialize(D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nUnitGUID)
{
    if (!pMissile)
    {
        return;
    }

    MISSILE_AllocMissileData(pMissile);
    pMissile->dwUnitId = nUnitGUID;
    pMissile->dwFlags &= ~(UNITFLAG_TARGETABLE | UNITFLAG_ISVALIDTARGET);
    D2GAME_DeleteTimersOnUnit_6FC34A30(pGame, pMissile);
    sub_6FC35570(pGame, pMissile, EVENTTYPE_MODECHANGE, 0, 0);
}

//D2Game.0x6FC55340
void __fastcall MISSILES_Free(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return;
    }

    MISSILE_FreeMissileData(pMissile);
    sub_6FC349F0(pGame, pMissile);
}

//D2Game.0x6FC55360
D2UnitStrc* __fastcall MISSILES_CreateMissileFromParams(D2GameStrc* pGame, D2MissileStrc* missileParams)
{
    if (!missileParams->pOwner || (missileParams->pOwner->dwUnitType != UNIT_PLAYER && missileParams->pOwner->dwUnitType != UNIT_MONSTER))
    {
        return 0;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(missileParams->nMissile);
    if (!pMissilesTxtRecord)
    {
        return 0;
    }

    int32_t nX = 0;
    int32_t nY = 0;
    if (missileParams->dwFlags & UNITFLAG_DOUPDATE)
    {
        nX = missileParams->nX;
        nY = missileParams->nY;
    }
    else
    {
        if (!missileParams->pOrigin)
        {
            return 0;
        }

        nX = CLIENTS_GetUnitX(missileParams->pOrigin);
        nY = CLIENTS_GetUnitY(missileParams->pOrigin);
    }

    D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(missileParams->pOwner), nX, nY);
    if (!pRoom)
    {
        return 0;
    }

    int32_t nTargetX = nX;
    int32_t nTargetY = nY;
    if (missileParams->dwFlags & 2)
    {
        nTargetX += missileParams->nTargetX;
        nTargetY += missileParams->nTargetY;
    }
    else if (missileParams->dwFlags & 32)
    {
        nTargetX = missileParams->nTargetX;
        nTargetY = missileParams->nTargetY;
    }

    int32_t nVelocity = 0;
    if (!(missileParams->dwFlags & 4))
    {
        nVelocity = pMissilesTxtRecord->nVel + (missileParams->nSkillLevel * pMissilesTxtRecord->nVelLev) / 8;
        nVelocity <<= 8;
    }
    else
    {
        nVelocity = missileParams->nVelocity;
        if (!(missileParams->dwFlags & 0x10))
        {
            nVelocity <<= 8;
        }
    }

    if (pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_CANSLOW] && STATES_CheckState(missileParams->pOwner, STATE_SLOWMISSILES))
    {
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(missileParams->pOwner, STATE_SLOWMISSILES);
        if (pStatList)
        {
            nVelocity = STATLIST_GetStatValue(pStatList, STAT_SKILL_HANDOFATHENA, 0) * nVelocity / 100;
        }
    }

    if (nVelocity)
    {
        nVelocity = 75 * nVelocity / 100;
    }

    D2UnitStrc* pTarget = missileParams->pTarget;
    if (nVelocity)
    {
        if (pTarget)
        {
            if (pTarget != missileParams->pOwner
                && CLIENTS_GetUnitX(pTarget) == CLIENTS_GetUnitX(missileParams->pOwner)
                && CLIENTS_GetUnitY(pTarget) == CLIENTS_GetUnitY(missileParams->pOwner))
            {
                pTarget = nullptr;
                ++nTargetX;
                ++nTargetY;
            }
        }
        else if (nTargetX == nX && nTargetY == nY)
        {
            ++nTargetX;
            ++nTargetY;
        }

        
        int32_t nXDiff = 0;
        int32_t nYDiff = 0;
        if (pTarget)
        {
            nXDiff = CLIENTS_GetUnitX(pTarget) - nX;
            nYDiff = CLIENTS_GetUnitY(pTarget) - nY;
        }
        else
        {
            nXDiff = nTargetX - nX;
            nYDiff = nTargetY - nY;
        }

        nXDiff = std::abs(nXDiff);

        if (nXDiff >= 100)
        {
            return 0;
        }

        nYDiff = std::abs(nYDiff);

        if (nYDiff >= 100)
        {
            return 0;
        }
    }

    D2UnitStrc* pMissile = SUNIT_AllocUnitData(UNIT_MISSILE, missileParams->nMissile, nX, nY, pGame, pRoom, 1, pMissilesTxtRecord->nCollideType, 0);
    if (!pMissile)
    {
        return 0;
    }

    int32_t nRange = 0;
    if (!(missileParams->dwFlags & 0x8000))
    {
        nRange = pMissilesTxtRecord->wRange + missileParams->nSkillLevel * pMissilesTxtRecord->wLevRange;
        if (missileParams->dwFlags & 8 && pMissilesTxtRecord->nSubLoop)
        {
            nRange += missileParams->nLoops * (pMissilesTxtRecord->nSubStop - pMissilesTxtRecord->nSubStart);
        }
    }
    else
    {
        nRange = missileParams->nRange;
    }

    if (missileParams->dwFlags & 0x200)
    {
        nRange -= missileParams->nFrame;
        pMissile->nSeqCurrentFramePrecise = missileParams->nFrame << 8;
    }

    MISSILE_SetTotalFrames(pMissile, nRange);
    MISSILE_SetCurrentFrame(pMissile, nRange);

    int32_t nActivateFrame = 0;
    if (missileParams->dwFlags & 0x800)
    {
        nActivateFrame = missileParams->nActivateFrame;
    }
    else
    {
        nActivateFrame = pMissilesTxtRecord->nActivate;
    }
    MISSILE_SetActivateFrame(pMissile, nRange - nActivateFrame);

    if (!pMissile->pDynamicPath)
    {
        return 0;
    }

    PATH_SetVelocity(pMissile->pDynamicPath, 0, __FILE__, __LINE__);

    if (pTarget)
    {
        PATH_SetTargetUnit(pMissile->pDynamicPath, pTarget);
    }
    else
    {
        D2COMMON_10170_PathSetTargetPos(pMissile->pDynamicPath, nTargetX, nTargetY);
    }

    if (pMissilesTxtRecord->nCollision)
    {
        PATH_SetFootprintCollisionMask(pMissile->pDynamicPath, COLLIDE_MISSILE);
    }
    else
    {
        PATH_SetFootprintCollisionMask(pMissile->pDynamicPath, COLLIDE_NONE);
    }

    PATH_SetMoveTestCollisionMask(pMissile->pDynamicPath, stru_6FD2E5F8[pMissilesTxtRecord->nCollideType].nCollisionMask);

    if (pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_CANDESTROY])
    {
        pMissile->dwFlags |= UNITFLAG_CANBEATTACKED;
    }

    if (nVelocity)
    {
        PATH_SetVelocity(pMissile->pDynamicPath, nVelocity, __FILE__, __LINE__);
        D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
    }

    MISSILE_SetOwner(pMissile, missileParams->pOwner);
    PATH_SetAcceleration(pMissile->pDynamicPath, pMissilesTxtRecord->wAccel);
    PATH_SetMaxVelocity(pMissile->pDynamicPath, ((int32_t)pMissilesTxtRecord->nMaxVel) << 8);

    if (missileParams->dwFlags & 0x400)
    {
        int32_t nDistance = UNITS_GetDistanceToCoordinates(pMissile, nTargetX, nTargetY);
        if (!nDistance)
        {
            nDistance = 1;
        }

        int32_t nCurrentFrame = 0;
        if (nVelocity)
        {
            nCurrentFrame = (nDistance << 16) / (16 * nVelocity);
        }

        MISSILE_SetCurrentFrame(pMissile, nCurrentFrame);
    }

    STATLIST_AllocStatListEx(pMissile, 0, nullptr, nullptr);

    if (missileParams->pInitFunc)
    {
        missileParams->pInitFunc(pMissile, missileParams->pInitArgs);
    }

    MISSILE_SetSkill(pMissile, missileParams->nSkill);
    MISSILE_SetLevel(pMissile, missileParams->nSkillLevel);

    sub_6FC7C900(missileParams->pOwner, 0);
    D2MissileDamageDataStrc missileDamageData = {};
    MISSILE_CalculateDamageData(&missileDamageData, missileParams->pOwner, missileParams->pOrigin, pMissile, missileParams->nSkillLevel);
    MISSILE_SetDamageStats(missileParams->pOwner, pMissile, &missileDamageData, missileParams->nSkillLevel);

    sub_6FC7CA10(missileParams->pOwner, 0);
    UNITS_StoreOwner(pMissile, missileParams->pOwner);

    pMissile->dwFlags &= ~UNITFLAG_ISVALIDTARGET;

    if (missileParams->pOwner && pMissile->dwUnitType == UNIT_MISSILE && (missileParams->pOwner->dwUnitType == UNIT_PLAYER || missileParams->pOwner->dwUnitType == UNIT_MONSTER))
    {
        if (pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_PIERCE])
        {
            const int32_t nPierceValue = STATLIST_UnitGetItemStatOrSkillStatValue(missileParams->pOwner, STAT_ITEM_PIERCE, 0) + STATLIST_UnitGetStatValue(missileParams->pOwner, STAT_SKILL_PIERCE, 0);
            if (nPierceValue)
            {
                D2SeedStrc seed = {};
                SEED_InitLowSeed(&seed, STATLIST_GetUnitBaseStat(missileParams->pOwner, STAT_PIERCE_IDX, 0));
                
                int32_t nPierceIndex = 0;
                do
                {
                    if ((ITEMS_RollRandomNumber(&seed) % 100) >= nPierceValue)
                    {
                        break;
                    }
                    ++nPierceIndex;
                }
                while (nPierceIndex < 4);
                STATLIST_SetUnitStat(pMissile, STAT_PIERCE_IDX, nPierceIndex, 0);
            }
        }
    }

    if (missileParams->dwFlags & 0x1000)
    {
        STATLIST_SetUnitStat(pMissile, STAT_TOHIT, missileParams->nAttBonus, 0);
    }

    if (missileParams->dwFlags & UNITFLAG_ISDEAD)
    {
        MISSILE_SetFlags(pMissile, MISSILE_GetFlags(pMissile) | 2);
    }

    sub_6FC6E8B0(pGame, missileParams->pOwner, pMissile);
    STATLIST_SetUnitStat(pMissile, STAT_DAMAGE_FRAMERATE, pMissilesTxtRecord->dwDamageRate, 0);

    return pMissile;
}

//D2Game.0x6FC55B70
void __fastcall MISSILES_SyncToClient(D2ClientStrc* pClient, D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nVelocity)
{
    if (!pMissile || pMissile->dwUnitType != UNIT_MISSILE)
    {
        return;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || !(pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_CLIENTSEND]))
    {
        return;
    }

    D2GSPacketSrv73 packet73 = {};
    packet73.nHeader = 0x73;
    packet73.nMissileId = MISSILE_GetClassId(pMissile);
    packet73.nX = CLIENTS_GetUnitX(pMissile);
    packet73.nY = CLIENTS_GetUnitY(pMissile);

    if (nVelocity)
    {
        packet73.nFirstX = D2COMMON_10175_PathGetFirstPointX(pMissile->pDynamicPath);
        packet73.nFirstY = D2COMMON_10176_PathGetFirstPointY(pMissile->pDynamicPath);
    }

    packet73.nCurrentFrame = MISSILE_GetCurrentFrame(pMissile);
    packet73.nUnitType = pOwner->dwUnitType;
    packet73.nUnitGUID = pOwner->dwUnitId;
    packet73.nLevel = MISSILE_GetLevel(pMissile);
    packet73.nPierce = STATLIST_UnitGetStatValue(pMissile, STAT_PIERCE_IDX, 0);

    sub_6FC3F610(pClient, &packet73);
}
