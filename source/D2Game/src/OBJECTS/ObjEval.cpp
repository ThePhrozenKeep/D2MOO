#include "OBJECTS/ObjEval.h"

#include <algorithm>

#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2StatList.h>
#include <D2States.h>
#include <DataTbls/ObjectsTbls.h>

#include "ITEMS/Items.h"
#include "UNIT/SUnitDmg.h"


//D2Game.0x6FC745D0
void __fastcall OBJEVAL_ApplyTrapObjectDamage(D2GameStrc* pGame, D2UnitStrc* pSrcUnit, D2UnitStrc* pTargetUnit, int32_t nDamageType)
{
    D2ActiveRoomStrc* pTargetRoom = UNITS_GetRoom(pTargetUnit);

    if (DUNGEON_IsRoomInTown(pTargetRoom) || UNITS_TestCollisionBetweenInteractingUnits(pTargetUnit, pSrcUnit, COLLIDE_MASK_PLAYER_FLYING))
    {
        return;
    }

    D2_ASSERT(pTargetUnit);

    const int32_t nHitpoints = STATLIST_UnitGetStatValue(pTargetUnit, STAT_HITPOINTS, 0);
    const int32_t nMinDamage = std::max(nHitpoints >> 5, 1);
    const int32_t nMaxDamage = std::max(nHitpoints >> 3, nMinDamage + 1);

    const int32_t nLevel = STATLIST_UnitGetStatValue(pTargetUnit, STAT_LEVEL, 0);

    const int32_t nParam = 2 * ((uint8_t)(nLevel + (uint8_t)ITEMS_RollLimitedRandomNumber(&pSrcUnit->pSeed, nLevel >> 2)) - 5 * ((int32_t)STATLIST_UnitGetStatValue(pTargetUnit, STAT_DEXTERITY, 0) >> 1) - nLevel);

    const int32_t nChanceToHit = std::max(nParam - (int32_t)STATLIST_UnitGetStatValue(pTargetUnit, STAT_ARMORCLASS, 0) + 125, 65);

    if ((ITEMS_RollRandomNumber(&pSrcUnit->pSeed) % 100) >= nChanceToHit)
    {
        return;
    }

    D2_ASSERT(nMinDamage <= nMaxDamage);

    const uint8_t nBaseDamage = DATATBLS_GetObjectsTxtRecord(pSrcUnit ? pSrcUnit->dwClassId : -1)->nDamage;

    const int32_t nDamage = (ITEMS_RollLimitedRandomNumber(&pSrcUnit->pSeed, nMaxDamage - nMinDamage + 256) + nMinDamage) * (uint32_t)nBaseDamage / 100;
    if (!nBaseDamage)
    {
        FOG_DisplayWarning("ptObjectData->bDamage", __FILE__, __LINE__);
    }

    if (!nDamage)
    {
        return;
    }

    D2DamageStrc damage = {};
    switch (nDamageType)
    {
    case 1:
        damage.dwFireDamage = nDamage;
        damage.dwHitClass = 0x20;
        break;

    case 2:
        damage.dwLtngDamage = nDamage;
        damage.dwHitClass = 0x40;
        break;

    case 3:
        damage.dwColdDamage = nDamage;
        damage.dwHitClass = 0x30;
        break;

    default:
        damage.dwPhysDamage = nDamage;
        break;
    }

    damage.wResultFlags = DAMAGERESULTFLAG_SUCCESSFULHIT;

    if (!STATES_CheckState(pTargetUnit, STATE_UNINTERRUPTABLE))
    {
        damage.wResultFlags |= DAMAGERESULTFLAG_GETHIT;
    }

    SUNITDMG_CalculateTotalDamage(pGame, pSrcUnit, pTargetUnit, &damage);
    SUNITDMG_ExecuteEvents(pGame, pSrcUnit, pTargetUnit, 0, &damage);
    SUNITDMG_ExecuteMissileDamage(pGame, pSrcUnit, pTargetUnit, &damage);
}
