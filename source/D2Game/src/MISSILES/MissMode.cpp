#include "MISSILES/MissMode.h"

#include <algorithm>

#include <D2BitManip.h>
#include <D2Math.h>

#include <D2PacketDef.h>

#include <D2StatList.h>
#include <D2Dungeon.h>
#include <Units/Missile.h>
#include <DataTbls/MissilesIds.h>
#include <DataTbls/MissilesTbls.h>
#include <DataTbls/SkillsIds.h>
#include <DataTbls/SkillsTbls.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <D2States.h>
#include <D2StatList.h>
#include <D2DataTbls.h>
#include <D2Skills.h>
#include <D2Combat.h>
#include <Path/Path.h>
#include <Path/PathMisc.h>
#include <Path/Step.h>
#include <D2Monsters.h>
#include <D2Collision.h>
#include <UselessOrdinals.h>


#include "AI/AiGeneral.h"
#include "AI/AiUtil.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "GAME/Targets.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "OBJECTS/ObjMode.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT5/A5Q6.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillBar.h"
#include "SKILLS/SkillDruid.h"
#include "SKILLS/SkillNec.h"
#include "SKILLS/SkillPal.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitEvent.h"


#pragma pack(push, 1)
struct D2HitFunc22ParamStrc
{
    D2UnitStrc* pOwner;
    D2UnitStrc* pOrigin;
    int32_t nSkillId;
    int32_t nLevel;
    int32_t nParam;
    int32_t nHitSubMissile;
};
#pragma pack(pop)


D2MissileUnitFindTableStrc stru_6FD2E5F8[9] =
{
    /*[0]*/{ nullptr                                                  , COLLIDE_NONE                                       },
    /*[1]*/{ MISSMODE_UnitFindCallback_CanCollideWithGoodAlignmentUnit, COLLIDE_PLAYER | COLLIDE_BARRIER                   },
    /*[2]*/{ MISSMODE_UnitFindCallback_CanCollideWithMonster          , COLLIDE_MONSTER | COLLIDE_BARRIER                  },
    /*[3]*/{ MISSMODE_UnitFindCallback_CanCollideWithPlayerOrMonster  , COLLIDE_MONSTER | COLLIDE_PLAYER | COLLIDE_BARRIER },
    /*[4]*/{ nullptr                                                  , COLLIDE_NONE                                       },
    /*[5]*/{ MISSMODE_UnitFindCallback_CanCollideWithMonster          , COLLIDE_MONSTER | COLLIDE_BARRIER                  },
    /*[6]*/{ nullptr                                                  , COLLIDE_BARRIER                                    },
    /*[7]*/{ MISSMODE_UnitFindCallback_CanMissileDestroy              , COLLIDE_MISSILE                                    },
    /*[8]*/{ MISSMODE_UnitFindCallback_CanCollideWithPlayerOrMonster  , COLLIDE_MONSTER | COLLIDE_PLAYER | COLLIDE_BARRIER | COLLIDE_BLOCK_PLAYER },
};


//D2Game.0x6FC55CE0
int32_t __fastcall MISSMODE_UnitFindCallback_CanCollideWithMonster(D2UnitStrc* pUnit, void* pArgument)
{
    D2MissileUnitFindArgStrc* pArg = (D2MissileUnitFindArgStrc*)pArgument;
    if (!pArg || !pArg->pMissilesTxtRecord)
    {
        return 0;
    }
    
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    if (!(pUnit->dwFlags & UNITFLAG_ISVALIDTARGET) || !(pUnit->dwFlags & UNITFLAG_CANBEATTACKED) || (pArg->pMissilesTxtRecord->nNextHit && STATES_CheckState(pUnit, STATE_JUSTHIT)))
    {
        return 0;
    }

    if (MISSILE_CheckUnitIfOwner(pArg->pMissile, pUnit) || (pArg->pOwner && !sub_6FCBD900(pArg->pGame, pArg->pOwner, pUnit) && !pArg->pMissilesTxtRecord->nCollideFriend))
    {
        return 0;
    }

    return 1;
}

//D2Game.0x6FC55D90
int32_t __fastcall MISSMODE_UnitFindCallback_CanCollideWithGoodAlignmentUnit(D2UnitStrc* pUnit, void* pArgument)
{
    D2MissileUnitFindArgStrc* pArg = (D2MissileUnitFindArgStrc*)pArgument;
    if (!pArg || !pArg->pMissilesTxtRecord)
    {
        return 0;
    }

    if (!pUnit || (pUnit->dwUnitType != UNIT_PLAYER && (pUnit->dwUnitType != UNIT_MONSTER || STATLIST_GetUnitAlignment(pUnit) != UNIT_ALIGNMENT_GOOD)))
    {
        return 0;
    }

    if (!(pUnit->dwFlags & UNITFLAG_ISVALIDTARGET) || !(pUnit->dwFlags & UNITFLAG_CANBEATTACKED) || (pArg->pMissilesTxtRecord->nNextHit && STATES_CheckState(pUnit, STATE_JUSTHIT)))
    {
        return 0;
    }

    if (MISSILE_CheckUnitIfOwner(pArg->pMissile, pUnit) || (pArg->pOwner && !sub_6FCBD900(pArg->pGame, pArg->pOwner, pUnit) && !pArg->pMissilesTxtRecord->nCollideFriend))
    {
        return 0;
    }

    return 1;
}

//D2Game.0x6FC55E60
int32_t __fastcall MISSMODE_UnitFindCallback_CanCollideWithPlayerOrMonster(D2UnitStrc* pUnit, void* pArgument)
{
    D2MissileUnitFindArgStrc* pArg = (D2MissileUnitFindArgStrc*)pArgument;
    if (!pArg || !pArg->pMissilesTxtRecord)
    {
        return 0;
    }
    
    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    if (!(pUnit->dwFlags & UNITFLAG_ISVALIDTARGET) || !(pUnit->dwFlags & UNITFLAG_CANBEATTACKED) || (pArg->pMissilesTxtRecord->nNextHit && STATES_CheckState(pUnit, STATE_JUSTHIT)))
    {
        return 0;
    }

    if (MISSILE_CheckUnitIfOwner(pArg->pMissile, pUnit) || (pArg->pOwner && !sub_6FCBD900(pArg->pGame, pArg->pOwner, pUnit) && !pArg->pMissilesTxtRecord->nCollideFriend))
    {
        return 0;
    }

    return 1;
}

//D2Game.0x6FC55F20
int32_t __fastcall MISSMODE_UnitFindCallback_CanMissileDestroy(D2UnitStrc* pUnit, void* pArgument)
{
    D2MissileUnitFindArgStrc* pArg = (D2MissileUnitFindArgStrc*)pArgument;
    D2_MAYBE_UNUSED(pArg);

    if (!pUnit || pUnit->dwUnitType != UNIT_MISSILE)
    {
        return 0;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pUnit->dwClassId);
    if (!pMissilesTxtRecord || !(pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_CANDESTROY]))
    {
        return 0;
    }

    return 1;
}

//D2Game.0x6FC55F80
void __stdcall MISSMODE_FillDamageParams(D2UnitStrc* pMissile, D2UnitStrc* pTarget, D2DamageStrc* pDamage)
{
    memset(pDamage, 0x00, sizeof(*pDamage));

    pDamage->dwPhysDamage = MISSMODE_RollDamageValue(pMissile, STAT_MINDAMAGE, STAT_MAXDAMAGE, -1);
    pDamage->dwFireDamage = MISSMODE_RollDamageValue(pMissile, STAT_FIREMINDAM, STAT_FIREMAXDAM, STAT_PASSIVE_FIRE_MASTERY);
    pDamage->dwMagDamage = MISSMODE_RollDamageValue(pMissile, STAT_MAGICMINDAM, STAT_MAGICMAXDAM, STAT_PASSIVE_MAG_MASTERY);
    pDamage->dwLtngDamage = MISSMODE_RollDamageValue(pMissile, STAT_LIGHTMINDAM, STAT_LIGHTMAXDAM, STAT_PASSIVE_LTNG_MASTERY);
    pDamage->dwColdDamage = MISSMODE_RollDamageValue(pMissile, STAT_COLDMINDAM, STAT_COLDMAXDAM, STAT_PASSIVE_COLD_MASTERY);
    pDamage->dwColdLen = STATLIST_UnitGetStatValue(pMissile, STAT_COLDLENGTH, 0);
    pDamage->dwPoisDamage = MISSMODE_RollDamageValue(pMissile, STAT_POISONMINDAM, STAT_POISONMAXDAM, STAT_PASSIVE_POIS_MASTERY);
    pDamage->dwPoisLen = STATLIST_UnitGetStatValue(pMissile, STAT_POISONLENGTH, 0);

    const int32_t nPoisonCount = STATLIST_UnitGetStatValue(pMissile, STAT_POISON_COUNT, 0);
    if (nPoisonCount > 1)
    {
        pDamage->dwPoisLen /= nPoisonCount;
    }

    pDamage->dwManaLeech = STATLIST_UnitGetStatValue(pMissile, STAT_MANADRAINMINDAM, 0);
    pDamage->dwLifeLeech = STATLIST_UnitGetStatValue(pMissile, STAT_LIFEDRAINMINDAM, 0);
    pDamage->dwStamLeech = STATLIST_UnitGetStatValue(pMissile, STAT_STAMDRAINMINDAM, 0);
    pDamage->dwBurnDamage = MISSMODE_RollDamageValue(pMissile, STAT_BURNINGMIN, STAT_BURNINGMAX, STAT_PASSIVE_FIRE_MASTERY);
    pDamage->dwBurnLen = STATLIST_UnitGetStatValue(pMissile, STAT_FIRELENGTH, 0);
    pDamage->dwStunLen = STATLIST_UnitGetStatValue(pMissile, STAT_STUNLENGTH, 0);

    if (pTarget)
    {
        int32_t nDamagePercent = STATLIST_UnitGetStatValue(pMissile, STAT_DAMAGEPERCENT, 0);
        if (MONSTERS_IsDemon(pTarget))
        {
            nDamagePercent += STATLIST_UnitGetStatValue(pMissile, STAT_ITEM_DEMONDAMAGE_PERCENT, 0);
        }

        if (MONSTERS_IsUndead(pTarget))
        {
            nDamagePercent += STATLIST_UnitGetStatValue(pMissile, STAT_ITEM_UNDEADDAMAGE_PERCENT, 0);
        }

        if (pTarget->dwUnitType == UNIT_MONSTER)
        {
            D2MonStatsTxt* pMonStatsTxt = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
            if (pMonStatsTxt)
            {
                const int32_t nMonType = pMonStatsTxt->wMonType;
                if (nMonType > 0)
                {
                    D2StatStrc statBuffer[128] = {};
                    const int32_t nStats = STATLIST_CopyStats(pMissile, STAT_DAMAGE_VS_MONTYPE, statBuffer, std::size(statBuffer));

                    for (int32_t i = 0; i < nStats; ++i)
                    {
                        const int32_t nLayer = statBuffer[i].nLayer;
                        if (nLayer > 0 && nLayer < sgptDataTables->nMonTypeTxtRecordCount && nMonType > 0 && nMonType < sgptDataTables->nMonTypeTxtRecordCount)
                        {
                            if (gdwBitMasks[nLayer & 31] & sgptDataTables->pMonTypeNest[(nLayer >> 5) + nMonType * sgptDataTables->nMonTypeIndex])
                            {
                                nDamagePercent += statBuffer[i].nValue;
                            }
                        }
                    }
                }
            }
        }

        nDamagePercent = std::max(nDamagePercent, -90);

        pDamage->dwPhysDamage += (nDamagePercent * pDamage->dwPhysDamage) / 100;
    }

    if (STATLIST_UnitGetStatValue(pMissile, STAT_ITEM_DEADLYSTRIKE, 0))
    {
        pDamage->wResultFlags |= DAMAGERESULTFLAG_CRITICALSTRIKE;
        pDamage->dwPhysDamage *= 2;
    }

    if (STATLIST_UnitGetStatValue(pMissile, STAT_SKILL_BYPASS_UNDEAD, 0))
    {
        pDamage->dwHitFlags |= DAMAGEHITFLAG_BYPASSUNDEAD;
    }

    if (STATLIST_UnitGetStatValue(pMissile, STAT_SKILL_BYPASS_DEMONS, 0))
    {
        pDamage->dwHitFlags |= DAMAGEHITFLAG_BYPASSDEMON;
    }

    if (STATLIST_UnitGetStatValue(pMissile, STAT_SKILL_BYPASS_BEASTS, 0))
    {
        pDamage->dwHitFlags |= DAMAGEHITFLAG_BYPASSBEASTS;
    }
}

//D2Game.0x6FC56290
int32_t __fastcall MISSMODE_RollDamageValue(D2UnitStrc* pUnit, int32_t nMinDamStat, int32_t nMaxDamStat, int32_t nMasteryStat)
{
    int32_t nMaxDam = STATLIST_UnitGetStatValue(pUnit, nMaxDamStat, 0);
    if (nMaxDam <= 0)
    {
        return 0;
    }

    int32_t nMinDam = STATLIST_UnitGetStatValue(pUnit, nMinDamStat, 0);
    if (nMinDam <= 0)
    {
        return 0;
    }

    if (nMinDam > nMaxDam)
    {
        const int32_t nTemp = nMaxDam;

        nMaxDam = nMinDam;
        nMinDam = nTemp;
    }

    if (nMasteryStat != -1)
    {
        const int32_t nMastery = STATLIST_UnitGetStatValue(pUnit, nMasteryStat, 0);
        if (nMastery)
        {
            nMinDam += MONSTERUNIQUE_CalculatePercentage(nMinDam, nMastery, 100);
            nMaxDam += MONSTERUNIQUE_CalculatePercentage(nMaxDam, nMastery, 100);
        }
    }

    return nMinDam + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDam - nMinDam);
}

//D2Game.0x6FC56480
int32_t __fastcall MISSMODE_GetDamageValue(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage)
{
    if (!pAttacker)
    {
        return 0;
    }

    switch (MISSILE_GetElemTypeFromMissileId(pAttacker->dwClassId))
    {
    case ELEMTYPE_NONE:
    {
        pDamage->dwPhysDamage = MISSMODE_RollDamageValue(pAttacker, STAT_MINDAMAGE, STAT_MAXDAMAGE, -1);

        if (pDefender)
        {
            int32_t nDamagePercent = STATLIST_UnitGetStatValue(pAttacker, STAT_DAMAGEPERCENT, 0);
            if (MONSTERS_IsDemon(pDefender))
            {
                nDamagePercent += STATLIST_UnitGetStatValue(pAttacker, STAT_ITEM_DEMONDAMAGE_PERCENT, 0);
            }

            if (MONSTERS_IsUndead(pDefender))
            {
                nDamagePercent += STATLIST_UnitGetStatValue(pAttacker, STAT_ITEM_UNDEADDAMAGE_PERCENT, 0);
            }

            nDamagePercent = std::max(nDamagePercent, -90);

            pDamage->dwPhysDamage += (nDamagePercent * pDamage->dwPhysDamage) / 100;
        }

        if (STATLIST_UnitGetStatValue(pAttacker, STAT_ITEM_DEADLYSTRIKE, 0))
        {
            pDamage->wResultFlags |= DAMAGERESULTFLAG_CRITICALSTRIKE;
            pDamage->dwPhysDamage *= 2;
        }

        return pDamage->dwPhysDamage;
    }
    case ELEMTYPE_FIRE:
    {
        pDamage->dwFireDamage = MISSMODE_RollDamageValue(pAttacker, STAT_FIREMINDAM, STAT_FIREMAXDAM, STAT_PASSIVE_FIRE_MASTERY);
        return pDamage->dwFireDamage;
    }
    case ELEMTYPE_LTNG:
    {
        pDamage->dwLtngDamage = MISSMODE_RollDamageValue(pAttacker, STAT_LIGHTMINDAM, STAT_LIGHTMAXDAM, STAT_PASSIVE_LTNG_MASTERY);
        return pDamage->dwLtngDamage;
    }
    case ELEMTYPE_MAGIC:
    {
        pDamage->dwMagDamage = MISSMODE_RollDamageValue(pAttacker, STAT_MAGICMINDAM, STAT_MAGICMAXDAM, STAT_PASSIVE_MAG_MASTERY);
        return pDamage->dwMagDamage;
    }
    case ELEMTYPE_COLD:
    {
        pDamage->dwColdDamage = MISSMODE_RollDamageValue(pAttacker, STAT_COLDMINDAM, STAT_COLDMAXDAM, STAT_PASSIVE_COLD_MASTERY);
        pDamage->dwColdLen = STATLIST_UnitGetStatValue(pAttacker, STAT_COLDLENGTH, 0);
        return pDamage->dwColdDamage;
    }
    case ELEMTYPE_POIS:
    {
        pDamage->dwPoisDamage = MISSMODE_RollDamageValue(pAttacker, STAT_POISONMINDAM, STAT_POISONMAXDAM, STAT_PASSIVE_POIS_MASTERY);
        pDamage->dwPoisLen = STATLIST_UnitGetStatValue(pAttacker, STAT_POISONLENGTH, 0);

        const int32_t nPoisonCount = STATLIST_UnitGetStatValue(pAttacker, STAT_POISON_COUNT, 0);
        if (nPoisonCount > 1)
        {
            pDamage->dwPoisLen /= nPoisonCount;
        }
        return 25 * pDamage->dwPoisDamage;
    }
    case ELEMTYPE_LIFE:
    {
        pDamage->dwLifeLeech = STATLIST_UnitGetStatValue(pAttacker, STAT_LIFEDRAINMINDAM, 0);
        return 0;
    }
    case ELEMTYPE_MANA:
    {
        pDamage->dwManaLeech = STATLIST_UnitGetStatValue(pAttacker, STAT_MANADRAINMINDAM, 0);
        return 0;
    }
    case ELEMTYPE_STAMINA:
    {
        pDamage->dwStamLeech = STATLIST_UnitGetStatValue(pAttacker, STAT_STAMDRAINMINDAM, 0);
        return 0;
    }
    case ELEMTYPE_STUN:
    {
        pDamage->dwStunLen = STATLIST_UnitGetStatValue(pAttacker, STAT_STUNLENGTH, 0);
        return 0;
    }
    case ELEMTYPE_BURN:
    {
        pDamage->dwBurnDamage = MISSMODE_RollDamageValue(pAttacker, STAT_BURNINGMIN, STAT_BURNINGMAX, STAT_PASSIVE_FIRE_MASTERY);
        pDamage->dwBurnLen = STATLIST_UnitGetStatValue(pAttacker, STAT_FIRELENGTH, 0);
        return 25 * pDamage->dwBurnDamage;
    }
    case ELEMTYPE_FREEZE:
    {
        pDamage->dwColdDamage = MISSMODE_RollDamageValue(pAttacker, STAT_COLDMINDAM, STAT_COLDMAXDAM, STAT_PASSIVE_COLD_MASTERY);
        pDamage->dwFrzLen = STATLIST_UnitGetStatValue(pAttacker, STAT_COLDLENGTH, 0);
        return pDamage->dwColdDamage;
    }
	default:
		break;
    }

    return 0;
}

//D2Game.0x6FC56730
void __fastcall MISSMODE_ResetDamageParams(D2GameStrc* pGame, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pUnit)
    {
        return;
    }

    pDamage->dwPhysDamage = 0;
    pDamage->dwFireDamage = 0;
    pDamage->dwLtngDamage = 0;
    pDamage->dwMagDamage = 0;
    pDamage->dwColdDamage = 0;
    pDamage->dwPoisDamage = 0;
    pDamage->dwLifeLeech = 0;
    pDamage->dwManaLeech = 0;
    pDamage->dwStamLeech = 0;
    pDamage->dwStunLen = 0;
    pDamage->dwBurnDamage = 0;
    pDamage->dwHitFlags &= ~(DAMAGEHITFLAG_BYPASSUNDEAD | DAMAGEHITFLAG_BYPASSDEMON | DAMAGEHITFLAG_BYPASSBEASTS);

    switch (MISSILE_GetElemTypeFromMissileId(pUnit->dwClassId))
    {
    case ELEMTYPE_POIS:
        pDamage->dwColdLen = 0;
        pDamage->dwFrzLen = 0;
        pDamage->dwBurnLen = 0;
        break;

    case ELEMTYPE_BURN:
        pDamage->dwColdLen = 0;
        pDamage->dwPoisLen = 0;
        pDamage->dwFrzLen = 0;
        break;

    case ELEMTYPE_COLD:
    case ELEMTYPE_FREEZE:
        pDamage->dwPoisLen = 0;
        pDamage->dwBurnLen = 0;
        break;

    default:
        pDamage->dwColdLen = 0;
        pDamage->dwFrzLen = 0;
        pDamage->dwPoisLen = 0;
        pDamage->dwBurnLen = 0;
        break;
    }
}

//D2Game.0x6FC567E0
void __fastcall MISSMODE_AddDamageValue(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nDamage)
{
    if (!pMissile)
    {
        return;
    }

    switch (MISSILE_GetElemTypeFromMissileId(pMissile->dwClassId))
    {
    case ELEMTYPE_NONE:
        pDamage->dwPhysDamage += nDamage;
        break;
    case ELEMTYPE_FIRE:
        pDamage->dwFireDamage += nDamage;
        break;
    case ELEMTYPE_LTNG:
        pDamage->dwLtngDamage += nDamage;
        break;
    case ELEMTYPE_MAGIC:
        pDamage->dwMagDamage += nDamage;
        break;
    case ELEMTYPE_COLD:
        pDamage->dwColdDamage += nDamage;
        break;
    case ELEMTYPE_POIS:
        pDamage->dwPoisDamage += nDamage;
        break;
    case ELEMTYPE_LIFE:
        pDamage->dwLifeLeech += nDamage;
        break;
    case ELEMTYPE_MANA:
        pDamage->dwManaLeech += nDamage;
        break;
    case ELEMTYPE_STAMINA:
        pDamage->dwStamLeech += nDamage;
        break;
    case ELEMTYPE_STUN:
        pDamage->dwStunLen += nDamage;
        break;
    case ELEMTYPE_BURN:
        pDamage->dwBurnDamage += nDamage;
        break;
    case ELEMTYPE_FREEZE:
        pDamage->dwColdDamage = nDamage;
        break;
	default:
		break;
    }
}

//D2Game.0x6FC568F0
int32_t __fastcall MISSMODE_CreatePoisonCloudHitSubmissiles(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nSubStep, int32_t nMainStep, int32_t nLoops)
{
    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(nMissileId);
    if (!pMissilesTxtRecord || !pOwner || !pOrigin)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x17;
    if (nLoops > 0)
    {
        missileParams.dwFlags = 0x1F;
        missileParams.nLoops = nLoops;
    }

    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pOrigin;
    missileParams.nMissile = nMissileId;
    missileParams.nX = CLIENTS_GetUnitX(pOrigin);
    missileParams.nY = CLIENTS_GetUnitY(pOrigin);
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nVelocity = pMissilesTxtRecord->dwParam[0] << 7;

    nMainStep = std::max(nMainStep, 1);

    constexpr int32_t xOffsets[16] =
    {
        0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1
    };

    constexpr int32_t yOffsets[16] =
    {
        2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2, 2
    };

    for (int32_t i = 0; i < 16; i += nMainStep)
    {
        missileParams.nTargetX = xOffsets[i];
        missileParams.nTargetY = yOffsets[i];
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    if (nSubStep)
    {
        missileParams.nVelocity = pMissilesTxtRecord->dwParam[1] << 7;

        for (int32_t i = 0; i < 15; i += nSubStep)
        {
            missileParams.nTargetX = xOffsets[i + 1];
            missileParams.nTargetY = yOffsets[i + 1];
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
        }
    }

    return 1;
}

//D2Game.0x6FC56AB0
void __fastcall MISSMODE_CreateImmolationArrowHitSubmissiles(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t nMissileId, int32_t nRange)
{
    if (!UNITS_GetRoom(pUnit))
    {
        return;
    }

    nRange = std::max(nRange, 0);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 1;
    missileParams.pOwner = SUNIT_GetOwner(pGame, pUnit);
    missileParams.nSkill = MISSILE_GetSkill(pUnit);
    missileParams.nSkillLevel = MISSILE_GetLevel(pUnit);
    missileParams.nMissile = nMissileId;

    if (nRange)
    {
        missileParams.dwFlags |= 0x8000;
    }

    const int32_t nUnitX = CLIENTS_GetUnitX(pUnit);
    const int32_t nUnitY = CLIENTS_GetUnitY(pUnit);
    for (int32_t x = -a3; x <= a3; ++x)
    {
        for (int32_t y = -a3; y <= a3; ++y)
        {
            const int32_t nX = nUnitX + x;
            const int32_t nY = nUnitY + y;
            D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
            if (pRoom && x * x + y * y <= a3 * a3)
            {
                D2CoordStrc coord1 = {};
                coord1.nX = nX;
                coord1.nY = nY;

                D2CoordStrc coord2 = {};
                coord2.nX = nX + x;
                coord2.nY = nY + y;

                if (!COLLISION_RayTrace(pRoom, &coord1, &coord2, 4u))
                {
                    D2ActiveRoomStrc* pTargetRoom = D2GAME_GetRoom_6FC52070(pRoom, nX, nY);
                    if (pTargetRoom && !DUNGEON_IsRoomInTown(pTargetRoom))
                    {
                        if (nRange)
                        {
                            missileParams.nRange = nRange;
                        }

                        missileParams.nX = nX;
                        missileParams.nY = nY;
                        MISSILES_CreateMissileFromParams(pGame, &missileParams);
                    }
                }
            }
        }
    }
}

//D2Game.0x6FC56D50
int32_t __fastcall MISSMODE_HandleMissileCollision(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return MISSMODE_SrvDmgHitHandler(pGame, pMissile, nullptr, 1);
    }
    
    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return MISSMODE_SrvDmgHitHandler(pGame, pMissile, nullptr, 1);
    }

    if (PATH_GetVelocity(pMissile->pDynamicPath) && sub_6FCBC930(pGame, pMissile) == 2)
    {
        return MISSMODE_SrvDmgHitHandler(pGame, pMissile, nullptr, 1);
    }

    const int32_t nFrame = MISSILE_GetCurrentFrame(pMissile) - 1;
    MISSILE_SetCurrentFrame(pMissile, nFrame);

    if (nFrame <= 0)
    {
        return MISSMODE_SrvDmgHitHandler(pGame, pMissile, nullptr, 1);
    }

    const int32_t nAnimMode = pMissile->dwAnimMode;
    if (!nAnimMode)
    {
        return 1;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
    if (!pRoom || !pMissile->pDynamicPath)
    {
        return 2;
    }

    const int32_t nFlags = D2Common_10201(pMissile->pDynamicPath);
    if (nFlags & 5)
    {
        return 2;
    }

    if (nAnimMode == 6 || MISSILE_GetCurrentFrame(pMissile) > MISSILE_GetActivateFrame(pMissile) || !nFlags)
    {
        return 1;
    }

    if (!stru_6FD2E5F8[nAnimMode].pfUnitFindCallback)
    {
        return 2;
    }

    D2MissileUnitFindArgStrc unitFindArg = {};
    unitFindArg.pGame = pGame;
    unitFindArg.pMissile = pMissile;
    unitFindArg.pOwner = SUNIT_GetOwner(pGame, pMissile);
    unitFindArg.pMissilesTxtRecord = pMissilesTxtRecord;

    const int32_t nSize = UNITS_GetUnitSizeX(pMissile);

    D2PathPointStrc* pathPoints = nullptr;
    const int32_t nPathPoints = D2COMMON_10198_PathGetSaveStep(pMissile->pDynamicPath, &pathPoints);

    for (int32_t i = 0; i < nPathPoints; ++i)
    {
        const uint16_t nX = pathPoints[i].X;
        const uint16_t nY = pathPoints[i].Y;
        const uint16_t nCollisionMask = COLLISION_CheckMaskWithSize(pRoom, nX, nY, nSize, stru_6FD2E5F8[nAnimMode].nCollisionMask);
        if (nCollisionMask)
        {
            D2UnitStrc* pTarget = D2Common_10407(pRoom, nX, nY, stru_6FD2E5F8[nAnimMode].pfUnitFindCallback, &unitFindArg, nSize);
            if (pTarget)
            {
                return MISSMODE_SrvDmgHitHandler(pGame, pMissile, pTarget, 0);
            }

            if (nCollisionMask & 4)
            {
                MISSMODE_SrvDmgHitHandler(pGame, pMissile, nullptr, 1);
                return 2;
            }
        }
    }

    return 1;
}

//D2Game.0x6FC56FA0
D2UnitStrc* __fastcall MISSMODE_CreatePlagueJavelin_PoisonJavelin_PoisonTrapHitSubmissiles(D2GameStrc* pGame, D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nRange, int32_t nLoops)
{
    if (!D2Common_10234(pOrigin->pDynamicPath))
    {
        return nullptr;
    }

    D2MissileStrc missileParams = {};

    missileParams.dwFlags = 5;
    if (nRange > 0)
    {
        missileParams.nRange = nRange;
        missileParams.dwFlags = 0x8005;
    }

    if (nLoops > 0)
    {
        missileParams.nLoops = nLoops;
        missileParams.dwFlags |= 8;
    }

    missileParams.pOwner = SUNIT_GetOwner(pGame, pOrigin);
    if (!missileParams.pOwner)
    {
        return nullptr;
    }

    missileParams.pOrigin = pOrigin;
    missileParams.nMissile = nMissileId;
    missileParams.nX = CLIENTS_GetUnitX(pOrigin);
    missileParams.nY = CLIENTS_GetUnitY(pOrigin);
    missileParams.nSkill = MISSILE_GetSkill(pOrigin);
    const int32_t nLevel = MISSILE_GetLevel(pOrigin);
    missileParams.nLoops = 2 * (nLevel - 1);
    missileParams.nSkillLevel = nLevel;

    return MISSILES_CreateMissileFromParams(pGame, &missileParams);
}

//D2Game.0x6FC570B0
int32_t __fastcall MISSMODE_SrvDo02_PlagueJavelin_PoisonJavelin_PoisonTrap(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (pMissile)
    {
        D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
        if (pMissilesTxtRecord && pMissilesTxtRecord->wSubMissile[0] >= 0)
        {
            const int32_t nLevel = MISSILE_GetLevel(pMissile);
            D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
            const int32_t nValue = MISSILE_EvaluateMissileFormula(pMissile, pOwner, pMissilesTxtRecord->dwSrvCalc, pMissile->dwClassId, nLevel);
            MISSMODE_CreatePlagueJavelin_PoisonJavelin_PoisonTrapHitSubmissiles(pGame, pMissile, pMissilesTxtRecord->wSubMissile[0], 0, nValue);
        }
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC57140
int32_t __fastcall MISSMODE_SrvDo03_PoisonCloud_Blizzard_ThunderStorm_HandOfGod(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile->pDynamicPath || !PATH_GetVelocity(pMissile->pDynamicPath))
    {
        COLLISION_SetMask(UNITS_GetRoom(pMissile), CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), 0x40);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC571D0
int32_t __fastcall MISSMODE_SrvDo05_FireWall_ImmolationFire_MeteorFire_MoltenBoulderFirePath(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return MISSMODE_HandleMissileCollision(pGame, pMissile);
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return MISSMODE_HandleMissileCollision(pGame, pMissile);
    }

    const int32_t nCurrentFrame = pMissile->nSeqCurrentFramePrecise >> 8;
    COLLISION_SetMaskWithSize(UNITS_GetRoom(pMissile), CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), UNITS_GetUnitSizeX(pMissile), 0x40);

    if (nCurrentFrame == pMissilesTxtRecord->nSubStart - 1)
    {
        pMissile->nSeqCurrentFramePrecise = (nCurrentFrame + ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, pMissilesTxtRecord->nSubStop - pMissilesTxtRecord->nSubStart)) << 8;
    }
    else if (MISSILE_GetCurrentFrame(pMissile) == pMissilesTxtRecord->nSubStart)
    {
        pMissile->nSeqCurrentFramePrecise = (pMissilesTxtRecord->nSubStart - 3) << 8;
    }
    else if (MISSILE_GetCurrentFrame(pMissile) < pMissilesTxtRecord->nSubStart)
    {
        const int32_t nCappedFrame = std::max(nCurrentFrame - 2, 0);
        pMissile->nSeqCurrentFramePrecise = nCappedFrame << 8;
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC573B0
int32_t __fastcall MISSMODE_SrvDo06_MoltenBoulder_FireWallMaker(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner || !pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] < 0 || !pMissile->pDynamicPath)
    {
        return 2;
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        const int32_t nX = CLIENTS_GetUnitX(pMissile);
        const int32_t nY = CLIENTS_GetUnitY(pMissile);

        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 0x21;
        missileParams.pOwner = pOwner;
        missileParams.nX = nX;
        missileParams.nY = nY;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        missileParams.nTargetX = nX;
        missileParams.nTargetY = nY;
        missileParams.nMissile = pMissilesTxtRecord->wSubMissile[0];

        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC57510
int32_t __fastcall MISSMODE_SrvDo07_GuidedArrow_BoneSpirit(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (SUNIT_IsDead(pOwner) || DUNGEON_IsRoomInTown(UNITS_GetRoom(pMissile)))
    {
        return 2;
    }

    if (MISSILE_GetTargetX(pMissile) & 1)
    {
        sub_6FCBC7E0(pGame, pMissile);

        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pMissile);
        if (pTarget && (SUNIT_IsDead(pTarget) || !sub_6FCBD900(pGame, pOwner, pTarget)))
        {
            pTarget = nullptr;
        }

        int32_t nParam = pMissilesTxtRecord->dwParam[0];
        if (nParam <= 0)
        {
            nParam = 5;
        }

        if (pTarget && !(MISSILE_GetCurrentFrame(pMissile) % nParam))
        {
            const int32_t nDistance = UNITS_GetDistanceToOtherUnit(pMissile, pTarget);
            if (nDistance > 3 && nDistance < 25)
            {
                D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
            }
        }
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC57640
int32_t __fastcall MISSMODE_SrvDo08_MonBlizzCenter(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    const int32_t nDivisor = std::max(pMissilesTxtRecord->dwParam[2], 1);
    const int32_t nDividedLevel = nLevel / nDivisor;
    const int32_t nParam = std::max(nDividedLevel, 2);
    const int32_t nFrames = std::max(pMissilesTxtRecord->dwParam[1] - nDividedLevel, 3);

    MISSMODE_CreateMissileWithCollisionCheck(pGame, pMissile, nParam + pMissilesTxtRecord->dwParam[0], nFrames, pMissilesTxtRecord->wSubMissile[0], 5);
    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC576F0
D2UnitStrc* __fastcall MISSMODE_CreateMissileWithCollisionCheck(D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nRange, int32_t nFrames, int32_t nMissileId, uint16_t nCollisionMask)
{
    if (!nFrames || (MISSILE_GetRemainingFrames(pMissile) % nFrames) != 0)
    {
        return nullptr;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner || !UNITS_GetRoom(pMissile))
    {
        return nullptr;
    }

    SEED_InitLowSeed(&pMissile->pSeed, CLIENTS_GetUnitX(pMissile) + MISSILE_GetRemainingFrames(pMissile));

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 3;
    missileParams.pOwner = pOwner;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nMissile = nMissileId;

    const int32_t nMaxRange = nRange - 1;
    missileParams.nX = ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, 2 * nMaxRange) - nMaxRange + CLIENTS_GetUnitX(pMissile);
    missileParams.nY = ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, 2 * nMaxRange) - nMaxRange + CLIENTS_GetUnitY(pMissile);

    if (COLLISION_CheckMask(UNITS_GetRoom(pMissile), missileParams.nX, missileParams.nY, nCollisionMask))
    {
        return nullptr;
    }

    return MISSILES_CreateMissileFromParams(pGame, &missileParams);
}

//D2Game.0x6FC57910
int32_t __fastcall MISSMODE_SrvDo09_BatLightningBolt(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 1;
        missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
        missileParams.nX = CLIENTS_GetUnitX(pMissile);
        missileParams.nY = CLIENTS_GetUnitY(pMissile);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        missileParams.nMissile = pMissilesTxtRecord->wSubMissile[0];
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC57A40
int32_t __fastcall MISSMODE_SrvDo10_BlizzardCenter(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 2;
    }

    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    MISSMODE_CreateMissileWithCollisionCheck(pGame, pMissile, SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nLevel), SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[1], nSkillId, nLevel), pMissilesTxtRecord->wSubMissile[0], 5);
    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC57B60
int32_t __fastcall MISSMODE_SrvDo11_FingerMageSpider(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pMissile);
    if (!pTarget)
    {
        D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
        if (pOwner)
        {
            pTarget = SUNIT_GetTargetUnit(pGame, pOwner);
        }
    }

    if (pTarget)
    {
        int32_t nFrames = pMissilesTxtRecord->dwParam[0];
        if (nFrames <= 0)
        {
            nFrames = 5;
        }

        if (!(MISSILE_GetCurrentFrame(pMissile) % nFrames) && UNITS_GetDistanceToOtherUnit(pMissile, pTarget) <= pMissilesTxtRecord->dwParam[1])
        {
            const int32_t nMissileX = CLIENTS_GetUnitX(pMissile);
            const int32_t nMissileY = CLIENTS_GetUnitY(pMissile);
            const int32_t nTargetX = CLIENTS_GetUnitX(pTarget);
            const int32_t nTargetY = CLIENTS_GetUnitY(pTarget);
            const int32_t nParam = std::max(pMissilesTxtRecord->dwParam[2], 1);
            const int32_t nX = D2signum(nTargetX - nMissileX) * nParam;
            const int32_t nY = D2signum(nTargetY - nMissileY) * nParam;

            D2COMMON_10170_PathSetTargetPos(pMissile->pDynamicPath, nX + CLIENTS_GetUnitX(pMissile), nY + CLIENTS_GetUnitY(pMissile));
            D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
        }
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC57DA0
int32_t __fastcall MISSMODE_SrvDo12_DiabWallMaker(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] < 0)
    {
        return 2;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 33;
        missileParams.pOwner = pOwner;
        missileParams.nX = CLIENTS_GetUnitX(pMissile);
        missileParams.nY = CLIENTS_GetUnitY(pMissile);
        missileParams.nTargetX = missileParams.nX;
        missileParams.nTargetY = missileParams.nY;
        missileParams.nMissile = pMissilesTxtRecord->wSubMissile[0];
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC57F00
int32_t __fastcall MISSMODE_SrvDo13_BoneWallMaker(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!SUNIT_GetOwner(pGame, pMissile) || !MISSILE_GetTargetY(pMissile))
    {
        return 2;
    }

    if (!D2Common_10234(pMissile->pDynamicPath))
    {
        return MISSMODE_HandleMissileCollision(pGame, pMissile);
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 2;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 2;
    }

    D2SummonArgStrc summonArg = {};
    int32_t nSpawnMode;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pOwner, 0, nSkillId, nLevel, &nSpawnMode, 0, 0);
    if (nSummonId < 0)
    {
        return 0;
    }

    int32_t nPetType = pSkillsTxtRecord->nPetType;
    if (nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        nPetType = 0;
    }

    D2UnitStrc* pMonster = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, MISSILE_GetTargetX(pMissile));
    if (pMonster)
    {

        summonArg.nSpecialAiState = 0;
        summonArg.nMonMode = nSpawnMode;
        summonArg.dwFlags = 0xDu;
        summonArg.pOwner = pOwner;
        summonArg.nHcIdx = nSummonId;
        summonArg.pPosition.nX = CLIENTS_GetUnitX(pMissile);
        summonArg.pPosition.nY = CLIENTS_GetUnitY(pMissile);
        summonArg.nPetType = nPetType;

        D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
        if (pPet)
        {
            AIGENERAL_SetOwnerData(pGame, pPet, pMonster->dwUnitId, 1, 0, 0);
            AIGENERAL_AllocMinionList(pGame, pMonster, pPet);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pPet, 15, 0);
            D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pOwner, pPet, nSkillId, nLevel, 0);
            UNITS_StoreOwner(pPet, pOwner);
            sub_6FC40280(pGame, pPet, 0, 9);
            MISSILE_SetTargetY(pMissile, MISSILE_GetTargetY(pMissile) - 1);
        }
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC58140
int32_t __fastcall MISSMODE_SrvDo14_GrimWard(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    const int32_t nParam = std::max(pMissilesTxtRecord->dwParam[0], 1);

    if (MISSILE_GetRemainingFrames(pMissile) % nParam)
    {
        return MISSMODE_HandleMissileCollision(pGame, pMissile);
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    if (nSkillId > 0 && nLevel > 0)
    {
        sub_6FD14400(pGame, pMissile, pMissilesTxtRecord->dwParam[1], nSkillId, nLevel);
        return MISSMODE_HandleMissileCollision(pGame, pMissile);
    }

    return 2;
}

//D2Game.0x6FC581F0
int32_t __fastcall MISSMODE_SrvDo15_FrozenOrb(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    constexpr int32_t xPositions[] =
    {
        30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2, 0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29
    };
    
    constexpr int32_t yPositions[] =
    {
        0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29, 30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2
    };

    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] < 0)
    {
        return 2;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 2;
    }

    const int32_t nFrames = std::max(pMissilesTxtRecord->dwParam[0], 1);

    if (MISSILE_GetRemainingFrames(pMissile) % nFrames <= 0)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 2;
        missileParams.pOwner = pOwner;
        missileParams.pOrigin = pMissile;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        missileParams.nMissile = pMissilesTxtRecord->wSubMissile[0];

        const int32_t nIndex = std::abs(MISSILE_GetTargetX(pMissile) % 64);
        missileParams.nTargetX = xPositions[nIndex];
        missileParams.nTargetY = yPositions[nIndex];
        MISSILE_SetTargetX(pMissile, (nIndex + pMissilesTxtRecord->dwParam[1]) % 64);
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC58340
int32_t __fastcall MISSMODE_SrvDo16_FrozenOrbNova(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    const int32_t nParam = std::max(pMissilesTxtRecord->dwParam[1], 1);

    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
    if (nRemainingFrames < pMissilesTxtRecord->dwParam[0] && !(nRemainingFrames % nParam))
    {
        const int32_t nTargetX = MISSILE_GetTargetX(pMissile);
        const int32_t nTargetY = MISSILE_GetTargetY(pMissile);
        const int32_t nDiff = nTargetX - nTargetY;
        const int32_t nSum = nTargetX + nTargetY;

        D2COMMON_10170_PathSetTargetPos(pMissile->pDynamicPath, nDiff / 2 + CLIENTS_GetUnitX(pMissile), nSum / 2 + CLIENTS_GetUnitY(pMissile));
        D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
        MISSILE_SetTargetX(pMissile, nDiff / 2);
        MISSILE_SetTargetY(pMissile, nSum / 2);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC58480
int32_t __fastcall MISSMODE_SrvDo17_CairnStones(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    const int32_t nCurrentFrame = MISSILE_GetCurrentFrame(pMissile);
    if (pMissilesTxtRecord->wSubMissile[0] > 0 && nCurrentFrame < pMissilesTxtRecord->wRange - pMissilesTxtRecord->dwParam[0] && nCurrentFrame > pMissilesTxtRecord->dwParam[0])
    {
        MISSMODE_CreateMissileWithCollisionCheck(pGame, pMissile, pMissilesTxtRecord->dwParam[2], pMissilesTxtRecord->dwParam[1], pMissilesTxtRecord->wSubMissile[0], 5);
    }

    if (pMissilesTxtRecord->dwParam[3] >= 0 && !MISSILE_GetTargetX(pMissile) && nCurrentFrame <= (pMissilesTxtRecord->dwParam[0] + pMissilesTxtRecord->dwParam[4]))
    {
        MISSILE_SetTargetX(pMissile, 1);
        D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
        D2GAME_CreatePortalObject_6FD13DF0(pGame, pOwner, pRoom, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), pMissilesTxtRecord->dwParam[3], 0, 60u, 1);
        DUNGEON_ToggleHasPortalFlag(pRoom, 1);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC585E0
int32_t __fastcall MISSMODE_SrvDo18_TowerChestSpawner(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    int32_t nCurrentFrame = MISSILE_GetCurrentFrame(pMissile);
    if (nCurrentFrame == 1)
    {
        D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, MISSILE_GetTargetX(pMissile));
        if (pObject)
        {
            SUNIT_AttachSound(pObject, 0x5Cu, 0);
        }
    }

    if (nCurrentFrame == pMissilesTxtRecord->wRange - pMissilesTxtRecord->dwParam[0])
    {
        D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, MISSILE_GetTargetX(pMissile));
        if (pObject)
        {
            D2ObjOperateFnStrc pOp = {};
            pOp.pGame = pGame;
            pOp.pObject = pObject;
            pOp.pPlayer = nullptr;
            pOp.pObjectregion = (D2ObjectControlStrc*)&pObject->pSeed;
            pOp.nObjectIdx = pObject->dwClassId;
            sub_6FC75EB0(&pOp);
        }

        MISSILE_SetTargetY(pMissile, 1);
    }

    if (MISSILE_GetTargetY(pMissile))
    {
        const int32_t nFrames = std::max(4 * pMissilesTxtRecord->dwParam[1], 1);

        if (!(nCurrentFrame % nFrames))
        {
            D2CoordStrc pCoord = {};
            UNITS_GetCoords(pMissile, &pCoord);
            const int32_t nRange = pMissilesTxtRecord->dwParam[2];
            pCoord.nX += ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, 2 * nRange + 1) - nRange;
            pCoord.nY += ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, 2 * nRange + 1) - nRange;

            D2CoordStrc returnCoords = {};
            D2ActiveRoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pMissile), &pCoord, &returnCoords, 1);
            if (pRoom)
            {
                D2ItemDropStrc itemDrop = {};
                itemDrop.pSeed = 0;
                itemDrop.pUnit = pMissile;
                itemDrop.pGame = pGame;
                itemDrop.wItemFormat = pGame->wItemFormat;
                itemDrop.nSpawnType = 3;
                itemDrop.pRoom = pRoom;
                itemDrop.nId = DATATBLS_GetItemIdFromItemCode(' dlg');
                itemDrop.nX = returnCoords.nX;
                itemDrop.nY = returnCoords.nY;
                itemDrop.wUnitInitFlags = 1;
                itemDrop.nQuality = 2;
                itemDrop.nItemLvl = DATATBLS_GetMonsterLevelInArea(DUNGEON_GetLevelIdFromRoom(pRoom), pGame->nDifficulty, pGame->bExpansion);
                D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
            }
        }
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC58860
int32_t __fastcall MISSMODE_SrvDo20_BladeCreeper(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner || SUNIT_IsDead(pOwner))
    {
        MISSMODE_SrvDmgHitHandler(pGame, pMissile, nullptr, 1);
        return 2;
    }

    if (pOwner && pMissile->pDynamicPath)
    {
        D2Common_10229(pMissile->pDynamicPath, pMissile, UNITS_GetRoom(pOwner), CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner));
    }

    MISSILE_SetCurrentFrame(pMissile, 10);
    MISSMODE_HandleMissileCollision(pGame, pMissile);
    return 1;
}

//D2Game.0x6FC58940
int32_t __fastcall MISSMODE_SrvDo21_Distraction(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (pMissilesTxtRecord->wSubMissile[0] && D2Common_10234(pMissile->pDynamicPath))
    {
        D2GAME_CreateMissile_6FD115E0(pGame, pOwner, MISSILE_GetSkill(pMissile), MISSILE_GetLevel(pMissile), pMissilesTxtRecord->wSubMissile[0], CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile));
    }

    if (!pOwner || SUNIT_IsDead(pOwner))
    {
        MISSMODE_SrvDmgHitHandler(pGame, pMissile, nullptr, 1);
        return 2;
    }

    if (pOwner && pMissile->pDynamicPath)
    {
        D2Common_10229(pMissile->pDynamicPath, pMissile, UNITS_GetRoom(pOwner), CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner));
    }

    MISSILE_SetCurrentFrame(pMissile, 10);
    MISSMODE_HandleMissileCollision(pGame, pMissile);
    return 1;
}

//D2Game.0x6FC58B00
int32_t __fastcall MISSMODE_SrvDo22_LightningTrailingJavelin(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] == 0 || !pMissile->pDynamicPath)
    {
        return 2;
    }

    const int32_t nX = CLIENTS_GetUnitX(pMissile);
    const int32_t nY = CLIENTS_GetUnitY(pMissile);

    if (MISSILE_GetRemainingFrames(pMissile) < 2)
    {
        const int32_t nParam1 = D2COMMON_10175_PathGetFirstPointX(pMissile->pDynamicPath) - nX;
        const int32_t nParam2 = D2COMMON_10176_PathGetFirstPointY(pMissile->pDynamicPath) - nY;
        MISSILE_SetTargetX(pMissile, -nParam2);
        MISSILE_SetTargetY(pMissile, nParam1);
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 11;
        missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
        missileParams.nX = nX;
        missileParams.nY = nY;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        missileParams.nMissile = pMissilesTxtRecord->wSubMissile[0];
        missileParams.nLoops = pMissilesTxtRecord->dwParam[0];
        missileParams.nTargetX = MISSILE_GetTargetX(pMissile);
        missileParams.nTargetY = MISSILE_GetTargetY(pMissile);
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
        missileParams.nTargetX = -MISSILE_GetTargetX(pMissile);
        missileParams.nTargetY = -MISSILE_GetTargetY(pMissile);
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC58CC0
int32_t __fastcall MISSMODE_SrvDo23_24_SuccFireBall_FirestormMaker(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] == 0)
    {
        return 2;
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 1;
        if (pMissilesTxtRecord->dwParam[0] > 0)
        {
            missileParams.dwFlags = 9;
            missileParams.nLoops = pMissilesTxtRecord->dwParam[0];
        }
        missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
        missileParams.nX = CLIENTS_GetUnitX(pMissile);
        missileParams.nY = CLIENTS_GetUnitY(pMissile);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        missileParams.nMissile = pMissilesTxtRecord->wSubMissile[0];
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC58E10
int32_t __fastcall MISSMODE_SrvDo25_EruptionCenter(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] == 0)
    {
        return 2;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 2;
    }

    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    const int32_t nRange = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nLevel);
    const int32_t nFrames = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[1], nSkillId, nLevel);
    MISSMODE_CreateMissileWithCollisionCheck(pGame, pMissile, nRange, nFrames, pMissilesTxtRecord->wSubMissile[0], 0x45);

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC58F30
int32_t __fastcall MISSMODE_SrvDo26_Vines_PlagueVines(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] == 0)
    {
        return 2;
    }

    const int32_t nFrames = MISSILE_GetRemainingFrames(pMissile);
    const int32_t nParam = std::max(pMissilesTxtRecord->dwParam[0], 1);

    if (!(nFrames % nParam))
    {
        D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
        D2GAME_CreateMissile_6FD115E0(pGame, pOwner, MISSILE_GetSkill(pMissile), MISSILE_GetLevel(pMissile), pMissilesTxtRecord->wSubMissile[0], CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile));
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC59040
int32_t __fastcall MISSMODE_SrvDo27_Tornado(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 2;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);

    int32_t nParam0 = pMissilesTxtRecord->dwParam[0];
    if (nParam0 <= 0)
    {
        nParam0 = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[3], nSkillId, nLevel), 1);
    }

    if (!(nRemainingFrames % nParam0))
    {
        D2DamageStrc damage = {};
        MISSMODE_FillDamageParams(pMissile, 0, &damage);

        int32_t nParam1 = pMissilesTxtRecord->dwParam[1];
        if (nParam1 <= 0)
        {
            nParam1 = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nLevel), 1);
        }

        damage.dwHitFlags |= pMissilesTxtRecord->dwHitFlags;
        damage.wResultFlags |= pMissilesTxtRecord->wResultFlags;

        sub_6FD10200(pGame, pMissile, 0, 0, nParam1, &damage, pSkillsTxtRecord->dwAuraFilter);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC591C0
int32_t __fastcall MISSMODE_SrvDo28_Volcano(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] < 0)
    {
        return 2;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 2;
    }

    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (pOwner)
    {
        int32_t nAuraRange = pMissilesTxtRecord->dwParam[1];
        if (nAuraRange <= 0)
        {
            nAuraRange = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nLevel), 1);
        }

        int32_t nFrames = pMissilesTxtRecord->dwParam[0];
        if (nFrames <= 0)
        {
            nFrames = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[3], nSkillId, nLevel), 1);
        }

        const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
        if (nRemainingFrames > pMissilesTxtRecord->dwParam[2] && nRemainingFrames < pMissilesTxtRecord->dwParam[3] && !(nRemainingFrames % nFrames))
        {
            SEED_InitLowSeed(&pMissile->pSeed, MISSILE_GetTargetX(pMissile));
            const int32_t nOffsetX = ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, 2 * nAuraRange + 1) - nAuraRange;
            const int32_t nOffsetY = ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, 2 * nAuraRange + 1) - nAuraRange;
            MISSILE_SetTargetX(pMissile, SEED_GetLowSeed(&pMissile->pSeed));

            D2MissileStrc missileParams = {};

            missileParams.dwFlags = 1312;
            missileParams.pOwner = pOwner;
            missileParams.pOrigin = pMissile;
            missileParams.nTargetX = nOffsetX + CLIENTS_GetUnitX(pMissile);
            missileParams.nTargetY = nOffsetY + CLIENTS_GetUnitY(pMissile);
            missileParams.nSkill = nSkillId;
            missileParams.nGfxArg = pMissilesTxtRecord->dwParam[4];
            missileParams.nSkillLevel = nLevel;
            missileParams.nMissile = pMissilesTxtRecord->wSubMissile[0];
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
        }
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC594B0
int32_t __fastcall MISSMODE_SrvDo29_RecyclerDelay(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 2;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (pOwner && !SUNIT_IsDead(pOwner))
    {
        if (MISSILE_GetRemainingFrames(pMissile) == pMissilesTxtRecord->dwParam[0])
        {
            const int32_t nHitpoints = STATLIST_UnitGetStatValue(pOwner, STAT_HITPOINTS, 0) >> 8;
            const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pOwner) >> 8;
            if (nHitpoints < nMaxHp)
            {
                const int32_t nNewHp = std::min(MONSTERUNIQUE_CalculatePercentage(nMaxHp, SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, MISSILE_GetLevel(pMissile)), 100) + nHitpoints, nMaxHp);
                STATLIST_SetUnitStat(pOwner, STAT_HITPOINTS, nNewHp << 8, 0);

                if (pMissilesTxtRecord->wProgOverlay > 0 && pMissilesTxtRecord->wProgOverlay < sgptDataTables->nOverlayTxtRecordCount)
                {
                    UNITS_SetOverlay(pOwner, pMissilesTxtRecord->wProgOverlay, 0);
                }
            }
        }
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC596C0
int32_t __fastcall MISSMODE_SrvDo33_VineRecyclerDelay(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 2;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (pOwner && !SUNIT_IsDead(pOwner))
    {
        if (MISSILE_GetRemainingFrames(pMissile) == pMissilesTxtRecord->dwParam[0])
        {
            const int32_t nMana = STATLIST_UnitGetStatValue(pOwner, STAT_MANA, 0) >> 8;
            const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pOwner) >> 8;
            if (nMana < nMaxMana)
            {
                const int32_t nNewMana = std::min(MONSTERUNIQUE_CalculatePercentage(nMaxMana, SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, MISSILE_GetLevel(pMissile)), 100) + nMana, nMaxMana);
                STATLIST_SetUnitStat(pOwner, STAT_MANA, nNewMana << 8, 0);

                if (pMissilesTxtRecord->wProgOverlay > 0 && pMissilesTxtRecord->wProgOverlay < sgptDataTables->nOverlayTxtRecordCount)
                {
                    UNITS_SetOverlay(pOwner, pMissilesTxtRecord->wProgOverlay, 0);
                }
            }
        }
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC598D0
int32_t __fastcall MISSMODE_SrvDo30_RabiesPlague(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);

    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] < 0)
    {
        return 2;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 2;
    }

    const int32_t nUnitGUID = MISSILE_GetTargetY(pMissile);
    const int32_t nUnitType = MISSILE_GetTargetX(pMissile);
    D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);

    if (!pUnit || !pOwner || SUNIT_IsDead(pOwner) || MISSILE_GetCurrentFrame(pMissile) < 0)
    {
        MISSMODE_SrvDmgHitHandler(pGame, pMissile, nullptr, 1);
        return 2;
    }

    if (pOwner && pMissile->pDynamicPath)
    {
        D2Common_10229(pMissile->pDynamicPath, pMissile, UNITS_GetRoom(pOwner), CLIENTS_GetUnitX(pOwner), CLIENTS_GetUnitY(pOwner));
    }

    const int32_t nParam = std::max(pMissilesTxtRecord->dwParam[0], 1);

    if (!(MISSILE_GetRemainingFrames(pMissile) % nParam))
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 2;
        missileParams.nMissile = pMissilesTxtRecord->wSubMissile[0];
        missileParams.pOwner = pUnit;
        missileParams.pOrigin = pMissile;
        missileParams.nSkill = nSkillId;
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        const int32_t nRange = 2 * pMissilesTxtRecord->dwParam[1] + 1;
        missileParams.nTargetX = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRange) - pMissilesTxtRecord->dwParam[1];
        missileParams.nTargetY = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nRange) - pMissilesTxtRecord->dwParam[1];

        D2UnitStrc* pSubMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
        if (pSkillsTxtRecord->wAuraTargetState > 0)
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pOwner, pSkillsTxtRecord->wAuraTargetState);
            if (pSubMissile && pStatList)
            {
                MISSILE_SetTargetX(pSubMissile, D2COMMON_10473(pStatList));
            }
        }
    }

    MISSMODE_HandleMissileCollision(pGame, pMissile);

    return 1;
}

//D2Game.0x6FC59B90
int32_t __fastcall MISSMODE_SrvDo31_WakeOfDestructionMaker_BaalColdMaker(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] < 0)
    {
        return 2;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        MISSMODE_SrvDmgHitHandler(pGame, pMissile, nullptr, 0);
        return 2;
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        const int32_t nX = MISSILE_GetTargetX(pMissile);
        const int32_t nY = MISSILE_GetTargetY(pMissile);

        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 2;
        missileParams.pOwner = pOwner;
        missileParams.pOrigin = pMissile;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nTargetX = nX;
        missileParams.nTargetY = nY;
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        missileParams.nMissile = pMissilesTxtRecord->wSubMissile[0];
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
        missileParams.nTargetX = -nX;
        missileParams.nTargetY = -nY;
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC59CB0
int32_t __fastcall MISSMODE_SrvDo32_TigerFury(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSubMissile[0] < 0)
    {
        return 2;
    }

    if (D2Common_10234(pMissile->pDynamicPath))
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 1;
        missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
        missileParams.pOrigin = pMissile;
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nMissile = pMissilesTxtRecord->wSubMissile[0];
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return MISSMODE_SrvDo07_GuidedArrow_BoneSpirit(pGame, pMissile);
}

//D2Game.0x6FC59D80
int32_t __fastcall MISSMODE_SrvDo34_BaalTauntControl(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }
    
    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    int32_t nCounter = 0;
    do
    {
        if (pMissilesTxtRecord->wSubMissile[nCounter] < 0 || pMissilesTxtRecord->dwParam[nCounter + 1] <= 0)
        {
            break;
        }

        ++nCounter;
    }
    while (nCounter < 3);

    if (nCounter <= 0)
    {
        return 2;
    }

    const int32_t nRemainingFrames = MISSILE_GetRemainingFrames(pMissile);
    if (nRemainingFrames >= pMissilesTxtRecord->dwParam[0])
    {
        SEED_InitLowSeed(&pMissile->pSeed, CLIENTS_GetUnitX(pMissile));

        const int32_t nIndex = ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, nCounter);
        if (!(nRemainingFrames % pMissilesTxtRecord->dwParam[nIndex + 1]))
        {
            D2MissileStrc missileParams = {};
            missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
            missileParams.pOrigin = pMissile;
            missileParams.nSkill = MISSILE_GetSkill(pMissile);
            missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
            missileParams.nMissile = pMissilesTxtRecord->wSubMissile[nIndex];

            D2UnitStrc* pSubMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
            if (pSubMissile)
            {
                MISSMODE_SrvDmgHitHandler(pGame, pSubMissile, nullptr, 1);
            }
        }
    }

    if (!pMissile->pDynamicPath || !PATH_GetVelocity(pMissile->pDynamicPath))
    {
        COLLISION_SetMask(UNITS_GetRoom(pMissile), CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), 0x40u);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC59F
int32_t __fastcall MISSMODE_SrvDo35_RoyalStrikeChaosIce(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }
    
    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    const int32_t nFrames = std::max(pMissilesTxtRecord->dwParam[0], 1);

    if (MISSILE_GetRemainingFrames(pMissile) % nFrames)
    {
        return MISSMODE_HandleMissileCollision(pGame, pMissile);
    }

    SEED_InitLowSeed(&pMissile->pSeed, MISSILE_GetTargetX(pMissile));

    const int32_t nParam = MISSILE_GetTargetY(pMissile);
    const int16_t nParam1 = (int16_t)nParam;
    const int16_t nParam2 = (int16_t)(nParam >> 16);

    int32_t nXParam = nParam1;
    int32_t nYParam = nParam2;
    if (ITEMS_RollRandomNumber(&pMissile->pSeed) & 1)
    {
        nXParam = -nXParam;
    }
    else
    {
        nYParam = -nYParam;
    }

    int32_t nX = (nYParam + 4 * nParam1) / 4;
    int32_t nY = (nXParam + 4 * nParam2) / 4;
    if (!nX)
    {
        nX = 1;
    }

    if (!nY)
    {
        nY = 1;
    }

    D2COMMON_10170_PathSetTargetPos(pMissile->pDynamicPath, nX + CLIENTS_GetUnitX(pMissile), nY + CLIENTS_GetUnitY(pMissile));
    D2Common_10142(pMissile->pDynamicPath, pMissile, 0);

    MISSILE_SetTargetX(pMissile, SEED_GetLowSeed(&pMissile->pSeed));
    MISSILE_SetTargetY(pMissile, (int16_t)nX + (nY << 16));

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC5A180
int32_t __fastcall MISSMODE_SrvHit01_Fireball_ExplodingArrow_FreezingArrowExplosion(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    int32_t nParam = pMissilesTxtRecord->dwHitPar[0];
    if (nParam <= 0)
    {
        const int32_t nSkillId = MISSILE_GetSkill(pMissile);

        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 1;
        }

        nParam = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, MISSILE_GetLevel(pMissile)), 1);
    }

    D2DamageStrc damage = {};

    MISSMODE_GetDamageValue(pGame, pMissile, pUnit, &damage);
    damage.dwHitFlags |= pMissilesTxtRecord->dwHitFlags;
    damage.wResultFlags |= pMissilesTxtRecord->wResultFlags;
    sub_6FD10200(pGame, pOwner, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), nParam, &damage, 0);
    return 1;
}

//D2Game.0x6FC5A330
int32_t __fastcall MISSMODE_SrvHit24_PantherPotOrange(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    int32_t nParam = pMissilesTxtRecord->dwHitPar[0];
    if (nParam <= 0)
    {
        const int32_t nSkillId = MISSILE_GetSkill(pMissile);

        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 1;
        }

        nParam = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, MISSILE_GetLevel(pMissile)), 1);
    }

    D2DamageStrc damage = {};

    MISSMODE_FillDamageParams(pMissile, pUnit, &damage);
    damage.dwHitFlags |= pMissilesTxtRecord->dwHitFlags;
    damage.wResultFlags |= pMissilesTxtRecord->wResultFlags;
    sub_6FD10200(pGame, pOwner, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), nParam, &damage, 0);
    return 1;
}

//D2Game.0x6FC5A4F0
int32_t __fastcall MISSMODE_SrvHit02_PlagueJavelin_PoisonPotion(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 3;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 3;
    }

    D2UnitStrc* pOrigin = pUnit;
    if (!pUnit)
    {
        pOrigin = pMissile;
    }

    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    MISSMODE_CreatePoisonCloudHitSubmissiles(pGame, pOwner, pOrigin, pMissilesTxtRecord->wHitSubMissile[0], nSkillId, nLevel, pMissilesTxtRecord->dwHitPar[0], pMissilesTxtRecord->dwHitPar[1], pMissilesTxtRecord->dwHitPar[2]);
    return 3;
}

//D2Game.0x6FC5A580
int32_t __fastcall MISSMODE_SrvHit44_ExplodingJavelin(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    int32_t nRange = pMissilesTxtRecord->dwHitPar[0];
    if (!nRange)
    {
        const int32_t nSkillId = MISSILE_GetSkill(pMissile);
        const int32_t nLevel = MISSILE_GetLevel(pMissile);
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 1;
        }

        nRange = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nLevel), 1);
    }

    D2DamageStrc damage = {};
    MISSMODE_FillDamageParams(pMissile, pUnit, &damage);
    damage.wResultFlags |= pMissilesTxtRecord->wResultFlags;
    damage.dwHitFlags |= pMissilesTxtRecord->dwHitFlags;

    if (sub_6FD10200(pGame, pOwner, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), nRange, &damage, 0))
    {
        return 1;
    }

    return 3;
}

//D2Game.0x6FC5A740
int32_t __fastcall MISSMODE_SrvHit03_ExplosivePotion_BombOnGround(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return MISSMODE_SrvHit44_ExplodingJavelin(pGame, pMissile, 0);
    }

    return 0;
}

//D2Game.0x6FC5A760
int32_t __fastcall MISSMODE_SrvHit07_HolyBolt_FistOfTheHeavenBolt(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || !pUnit)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pMissilesTxtRecord->dwHitPar[0] || !pOwner || !SUNIT_CanPetBeTargetedBySkill(pGame, pOwner, pUnit, nSkillId) && !SUNIT_CanAllyBeTargetedBySkill(pGame, pOwner, pUnit, nSkillId))
    {
        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            if (!pMissilesTxtRecord->dwHitPar[1])
            {
                return 3;
            }
        }
        else if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            if (!pMissilesTxtRecord->dwHitPar[1])
            {
                return 3;
            }

            if (pMissilesTxtRecord->dwHitPar[1] == 2)
            {
                if (MONSTERS_IsDemon(pUnit))
                {
                    return 3;
                }
            }
            else
            {
                if (MONSTERS_IsUndead(pUnit))
                {
                    return 3;
                }
            }
        }

        return 4;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    const int32_t nMin = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nLevel) << 8;
    const int32_t nMax = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[1], nSkillId, nLevel) << 8;
    int32_t nRand = ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, nMax - nMin) + nMin;
    if (nRand <= 0)
    {
        nRand = MISSMODE_RollDamageValue(pMissile, STAT_MINDAMAGE, STAT_MAXDAMAGE, -1);
    }

    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
    const int32_t nNewHp = std::min(nRand + (int32_t)STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0), nMaxHp);
    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);

    if (pMissilesTxtRecord->wProgOverlay > 0)
    {
        UNITS_SetOverlay(pUnit, pMissilesTxtRecord->wProgOverlay, 0);
    }

    return 1;
}

//D2Game.0x6FC5A9B0
int32_t __fastcall MISSMODE_SrvHit08_Blaze(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return 2;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner || pUnit != pOwner || !STATES_CheckState(pOwner, STATE_BLAZE))
    {
        return 2;
    }

    return 0;
}

//D2Game.0x6FC5AA00
int32_t __fastcall MISSMODE_SrvHit06_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    const int32_t nMonsterId = pMissilesTxtRecord->dwHitPar[0];
    if (nMonsterId >= 0 && nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        int32_t nSpawnMode = pMissilesTxtRecord->dwHitPar[1];
        if (nSpawnMode < 0 || nSpawnMode >= 16)
        {
            nSpawnMode = MONMODE_NEUTRAL;
        }

        D2GAME_SpawnPresetMonster_6FC66560(pGame, UNITS_GetRoom(pMissile), nMonsterId, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), nSpawnMode);
    }

    return 1;
}

//D2Game.0x6FC5AB00
int32_t __fastcall MISSMODE_SrvHit09_ImmolationArrow(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (pMissilesTxtRecord->wHitSubMissile[0] >= 0)
    {
        STATLIST_SetUnitStat(pMissile, STAT_COLDLENGTH, 0, 0);
        STATLIST_SetUnitStat(pMissile, STAT_ITEM_FREEZE, 0, 0);

        int32_t nCalc0 = pMissilesTxtRecord->dwHitPar[0];
        if (nCalc0 <= 0)
        {
            nCalc0 = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nLevel), 1);
        }

        const int32_t nHitCalc = MISSILE_EvaluateMissileFormula(pMissile, pOwner, pMissilesTxtRecord->dwHitCalc, pMissile->dwClassId, nLevel);
        MISSMODE_CreateImmolationArrowHitSubmissiles(pGame, pMissile, nCalc0, pMissilesTxtRecord->wHitSubMissile[0], nHitCalc);
    }

    int32_t nCalc1 = pMissilesTxtRecord->dwHitPar[1];
    if (nCalc1 <= 0)
    {
        nCalc1 = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[1], nSkillId, nLevel), 1);
    }

    D2DamageStrc damage = {};
    MISSMODE_GetDamageValue(pGame, pMissile, pUnit, &damage);
    damage.dwHitFlags |= pMissilesTxtRecord->dwHitFlags;
    damage.wResultFlags |= pMissilesTxtRecord->wResultFlags;

    sub_6FD10200(pGame, pOwner, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), nCalc1, &damage, 0);

    return 3;
}

//D2Game.0x6FC5AD40
int32_t __fastcall MISSMODE_SrvHit10_GuidedArrow_BoneSpirit(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (DUNGEON_IsRoomInTown(UNITS_GetRoom(pMissile)))
    {
        return 1;
    }

    // TODO: v8, v9
    const int32_t v8 = MISSILE_GetTargetX(pMissile);
    if (!pUnit && v8 & 4)
    {
        return 1;
    }

    const int32_t v9 = D2Common_10201(pMissile->pDynamicPath);
    if (!pUnit && v9 & 4)
    {
        return 1;
    }

    if (v8 & 1)
    {
        if (pUnit && pUnit != SUNIT_GetTargetUnit(pGame, pMissile))
        {
            return 4;
        }

        return 3;
    }

    if (!(v8 & 2))
    {
        return 3;
    }

    if (MISSILE_GetCurrentFrame(pMissile) > 0)
    {
        return 4 - ((v8 & 4) != 0);
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || MISSILE_GetTargetX(pMissile) & 4)
    {
        return 1;
    }
    
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2UnitStrc* pTarget = sub_6FD107F0(pGame, pOwner, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), pMissilesTxtRecord->dwParam[1], 3, -1, 0);
    if (pTarget && SUNIT_IsDead(pTarget))
    {
        pTarget = nullptr;
    }

    const int32_t nFrames = pMissilesTxtRecord->wRange + pMissilesTxtRecord->wLevRange * (MISSILE_GetLevel(pMissile) - 1);
    MISSILE_SetTotalFrames(pMissile, nFrames);
    MISSILE_SetCurrentFrame(pMissile, nFrames);

    if (pTarget)
    {
        PATH_SetTargetUnit(pMissile->pDynamicPath, pTarget);
        MISSILE_SetTargetX(pMissile, 5);

        if (UNITS_GetDistanceToOtherUnit(pMissile, pTarget) >= 25)
        {
            return 4;
        }
    }
    else
    {
        const int32_t nParam = MISSILE_GetTargetY(pMissile);
        const int32_t nX = CLIENTS_GetUnitX(pMissile) + (int16_t)nParam;
        const int32_t nY = CLIENTS_GetUnitY(pMissile) + (int16_t)(nParam >> 16);
        D2COMMON_10170_PathSetTargetPos(pMissile->pDynamicPath, nX, nY);
        MISSILE_SetTargetX(pMissile, 6);

        if (UNITS_GetDistanceToCoordinates(pMissile, nX, nY) >= 25)
        {
            return 4;
        }
    }

    D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
    return 4;
}

//D2Game.0x6FC5B020
int32_t __fastcall MISSMODE_SrvHit12_ChainLightning_LightningStrike(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner || !pUnit)
    {
        return 3;
    }

    const int32_t nParam = MISSILE_GetTargetX(pMissile);
    if (nParam <= 1)
    {
        return 3;
    }

    const int32_t nX = CLIENTS_GetUnitX(pMissile);
    const int32_t nY = CLIENTS_GetUnitY(pMissile);

    int32_t nRange = pMissilesTxtRecord->dwHitPar[0];
    if (nRange <= 0)
    {
        const int32_t nSkillId = MISSILE_GetSkill(pMissile);
        const int32_t nLevel = MISSILE_GetLevel(pMissile);
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 3;
        }

        nRange = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nLevel), 1);
    }

    D2UnitStrc* pTarget = sub_6FD107F0(pGame, pOwner, nX, nY, nRange, 0x88583u, pUnit->dwUnitId, 0);
    if (pTarget && pTarget != pUnit)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 0x21;
        missileParams.nMissile = pMissile->dwClassId;
        missileParams.pOwner = pOwner;
        missileParams.pTarget = pTarget;
        missileParams.nX = nX;
        missileParams.nY = nY;
        missileParams.nTargetX = CLIENTS_GetUnitX(pTarget);
        missileParams.nTargetY = CLIENTS_GetUnitY(pTarget);
        missileParams.nSkill = MISSILE_GetSkill(pMissile);
        missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

        D2UnitStrc* pSubMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
        if (pSubMissile)
        {
            MISSILE_SetTargetX(pSubMissile, nParam - 1);
        }
    }

    return 3;
}

//D2Game.0x6FC5B280
int32_t __fastcall MISSMODE_SrvHit13_GlacialSpike_HellMeteorDown(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);

    int32_t nRange = pMissilesTxtRecord->dwHitPar[0];
    if (pMissilesTxtRecord->dwHitPar[0] <= 0)
    {
        nRange = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nLevel), 1);
    }

    int32_t nLength = pMissilesTxtRecord->dwHitPar[1];
    if (nLength <= 0)
    {
        nLength = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nLevel);
    }

    const int32_t nX = CLIENTS_GetUnitX(pMissile);
    const int32_t nY = CLIENTS_GetUnitY(pMissile);

    D2DamageStrc damage = {};

    MISSMODE_GetDamageValue(pGame, pMissile, pUnit, &damage);

    if (nLength > 0)
    {
        switch (MISSILE_GetElemTypeFromMissileId(pMissile->dwClassId))
        {
        case ELEMTYPE_COLD:
            damage.dwColdLen = nLength;
            break;

        case ELEMTYPE_POIS:
            damage.dwPoisLen = nLength;
            break;

        case ELEMTYPE_STUN:
            damage.dwStunLen = nLength;
            break;

        case ELEMTYPE_BURN:
            damage.dwBurnLen = nLength;
            break;

        case ELEMTYPE_FREEZE:
            damage.dwFrzLen = nLength;
            break;

        default:
            break;
        }
    }

    damage.dwHitFlags |= pMissilesTxtRecord->dwHitFlags;
    damage.wResultFlags |= pMissilesTxtRecord->wResultFlags;

    if (sub_6FD10200(pGame, pOwner, nX, nY, nRange, &damage, 0))
    {
        return 1;
    }

    return 3;
}

//D2Game.0x6FC5B4E0
int32_t __fastcall MISSMODE_SrvHit04_ExplodingArrow_FreezingArrow_RoyalStrikeMeteorCenter(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
    missileParams.pOrigin = pMissile;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    for (int32_t i = 0; i < 4; ++i)
    {
        if (pMissilesTxtRecord->wHitSubMissile[i] > 0)
        {
            missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[i];

            D2UnitStrc* pHitSubMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
            if (pHitSubMissile && pMissilesTxtRecord->dwHitPar[0] > 0)
            {
                MISSMODE_SrvDmgHitHandler(pGame, pHitSubMissile, pUnit, 1);
            }
        }
    }

    return 3;
}

//D2Game.0x6FC5B5C0
int32_t __fastcall MISSMODE_SrvHit11_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
    missileParams.pOrigin = pMissile;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    for (int32_t i = 0; i < 4; ++i)
    {
        if (pMissilesTxtRecord->wHitSubMissile[i] > 0)
        {
            missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[i];
            D2UnitStrc* pHitSubMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
            if (pHitSubMissile && pMissilesTxtRecord->dwHitPar[0] > 0)
            {
                MISSMODE_SrvDmgHitHandler(pGame, pHitSubMissile, pUnit, 1);
            }
        }
    }

    return 1;
}

//D2Game.0x6FC5B6A0
int32_t __fastcall MISSMODE_SrvHit14_MeteorCenter_CatapultMeteor_RoyalStrikeMeteor(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    const int32_t nX = CLIENTS_GetUnitX(pMissile);
    const int32_t nY = CLIENTS_GetUnitY(pMissile);

    int32_t nRange = pMissilesTxtRecord->dwHitPar[0];
    if (nRange <= 0)
    {
        nRange = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nLevel), 1);
    }

    D2DamageStrc damage = {};
    MISSMODE_FillDamageParams(pMissile, pUnit, &damage);

    damage.dwHitFlags |= pMissilesTxtRecord->dwHitFlags;
    damage.wResultFlags |= pMissilesTxtRecord->wResultFlags;

    int32_t nResult = 1;
    if (!sub_6FD10200(pGame, pOwner, nX, nY, nRange, &damage, 0))
    {
        nResult = 3;
    }

    const int32_t nHitSubMissile = pMissilesTxtRecord->wHitSubMissile[0];
    if (nHitSubMissile >= 0)
    {
        int32_t nMissileRange = 0;
        if (nLevel > 0)
        {
            nMissileRange = pSkillsTxtRecord->dwParam[2] + (nLevel - 1) * pSkillsTxtRecord->dwParam[3];
        }

        const int32_t nParam = std::max(pMissilesTxtRecord->dwHitPar[1], 1u);
        MISSMODE_CreateMeteor_MoltenBoulderSubmissiles(pGame, pMissile, nHitSubMissile, nMissileRange, nParam);
    }

    return nResult;
}

//D2Game.0x6FC5B910
void __fastcall MISSMODE_CreateMeteor_MoltenBoulderSubmissiles(D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nMissileId, int32_t nRange, int32_t nStep)
{
    if (nMissileId < 0)
    {
        return;
    }

    const int32_t nX = CLIENTS_GetUnitX(pMissile);
    const int32_t nY = CLIENTS_GetUnitY(pMissile);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 1;
    if (nRange > 0)
    {
        missileParams.dwFlags = 0x8001;
        missileParams.nRange = nRange;
    }

    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nMissile = nMissileId;

    nStep = std::max(nStep, 1);

    constexpr int32_t xOffsets[18] =
    {
        2, -2, 0, 0, -3, 0, 3, -1, 1, -1, 2, -4, -3, -1, 0, 1, 3, 4
    };

    constexpr int32_t yOffsets[18] =
    {
        -2, -2, 2, 5, 3, 3, 3, 2, 1, -1, -1, -2, -2, -3, -4, -3, -3, -2
    };

    for (int32_t i = 0; i < 18; i += nStep)
    {
        missileParams.nX = nX + xOffsets[i];
        missileParams.nY = nY + yOffsets[i];
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }
}

//D2Game.0x6FC5BA50
int32_t __fastcall MISSMODE_SrvHit15_SpiderGooLay(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 1;
    missileParams.pOwner = pOwner;
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[0];
    MISSILES_CreateMissileFromParams(pGame, &missileParams);

    return 0;
}

//D2Game.0x6FC5BB80
int32_t __fastcall MISSMODE_SrvHit16_SpiderGoo_VinesTrail_VinesWither(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    if (!pUnit)
    {
        return 0;
    }

    if (pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 1;
    }

    const int32_t nLength = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[3], nSkillId, nLevel), 5);

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->wAuraTargetState);
    if (!pStatList)
    {
        D2StatListStrc* pNewStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nLength + pGame->dwGameFrame, pOwner->dwUnitType, pOwner->dwUnitId);
        if (!pNewStatList)
        {
            return 1;
        }

        STATLIST_SetState(pNewStatList, pSkillsTxtRecord->wAuraTargetState);
        STATLIST_SetStatRemoveCallback(pNewStatList, sub_6FD10E50);
        D2COMMON_10475_PostStatToStatList(pUnit, pNewStatList, 1);
        STATES_ToggleState(pUnit, pSkillsTxtRecord->wAuraTargetState, 1);

        // TODO: pStatList = pNewStatList; ????
    }

    sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nLevel);
    STATES_ToggleGfxStateFlag(pUnit, pSkillsTxtRecord->wAuraTargetState, 1);
    D2COMMON_10476(pStatList, nLength + pGame->dwGameFrame);
    EVENT_SetEvent(pGame, pUnit, 12, nLength + pGame->dwGameFrame, 0, 0);
    return 0;
}

//D2Game.0x6FC5BD30
int32_t __fastcall MISSMODE_SrvHit17_Howl(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    // TODO: v13, v15

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    if (pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 1;
    }

    if (STATES_CheckState(pUnit, pSkillsTxtRecord->wAuraTargetState))
    {
        return 0;
    }

    if (nLevel + pSkillsTxtRecord->dwParam[1] + STATLIST_UnitGetStatValue(pOwner, STAT_LEVEL, 0) <= STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0))
    {
        return 0;
    }

    int32_t v13 = 0;
    if (nLevel > 0)
    {
        v13 = pSkillsTxtRecord->dwParam[2] + (nLevel - 1) * pSkillsTxtRecord->dwParam[3];
    }

    int32_t v15 = 0;
    if (nLevel > 0)
    {
        v15 = pSkillsTxtRecord->dwParam[4] + (nLevel - 1) * pSkillsTxtRecord->dwParam[5];
    }

    AIUTIL_ApplyTerrorCurseState(pGame, pOwner, pUnit, nSkillId, v13, v15);
    return 0;
}

//D2Game.0x6FC5BED0
int32_t __fastcall MISSMODE_SrvHit18_Shout_BattleCommand_BattleOrders(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    if (pUnit && SUNIT_AreUnitsAligned(pGame, pOwner, pUnit))
    {
        SKILLS_ApplyWarcryStats(pGame, pUnit, pOwner, MISSILE_GetSkill(pMissile), MISSILE_GetLevel(pMissile));
    }

    return  0;
}

//D2Game.0x6FC5BF30
int32_t __fastcall MISSMODE_SrvHit19_FingerMageSpider(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner || !pUnit || pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 1;
    }

    const int32_t nLength = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nLevel), 5);
    const int32_t nTimeout = nLength + pGame->dwGameFrame;
    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->wAuraTargetState);
    if (!pStatList)
    {
        D2StatListStrc* pNewStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nTimeout, pOwner->dwUnitType, pOwner->dwUnitId);
        if (!pNewStatList)
        {
            return 1;
        }

        STATLIST_SetState(pNewStatList, pSkillsTxtRecord->wAuraTargetState);
        STATLIST_SetStatRemoveCallback(pNewStatList, sub_6FD10E50);
        D2COMMON_10475_PostStatToStatList(pUnit, pNewStatList, 1);
        STATES_ToggleState(pUnit, pSkillsTxtRecord->wAuraTargetState, 1);

        // TODO: pStatList = pNewStatList; ????
    }

    sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nLevel);
    STATES_ToggleGfxStateFlag(pUnit, pSkillsTxtRecord->wAuraTargetState, 1);
    D2COMMON_10476(pStatList, nTimeout);
    EVENT_SetEvent(pGame, pUnit, 12, nTimeout, 0, 0);
    return 3;
}

//D2Game.0x6FC5C0D0
int32_t __fastcall MISSMODE_SrvHit20_LightningFury(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    int32_t nAuraRange = pMissilesTxtRecord->dwHitPar[0];
    if (nAuraRange <= 0)
    {
        nAuraRange = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nLevel), 1);
    }

    int32_t nParam = pMissilesTxtRecord->dwHitPar[1];
    if (nParam <= 0)
    {
        nParam = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nLevel), 1);
    }

    int32_t nAuraFilter = pSkillsTxtRecord->dwAuraFilter;
    if (!nAuraFilter)
    {
        nAuraFilter = 0xA783u;
    }

    D2HitFunc22ParamStrc params = {};
    params.pOwner = pOwner;
    params.pOrigin = pMissile;
    params.nSkillId = nSkillId;
    params.nLevel = nLevel;
    params.nParam = nParam;
    params.nHitSubMissile = pMissilesTxtRecord->wHitSubMissile[0];

    sub_6FD0FE80(pGame, pOwner, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), nAuraRange, nAuraFilter, MISSMODE_LightningFury_AuraCallback, &params, 1, __FILE__, __LINE__);
    return 3;
}

//D2Game.0x6FC5C2D0
int32_t __fastcall MISSMODE_LightningFury_AuraCallback(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pTarget)
{
    D2HitFunc22ParamStrc* pParams = (D2HitFunc22ParamStrc*)pAuraCallback->pArgs;
    if (pAuraCallback->nCounter >= pParams->nParam)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x20;
    missileParams.pOwner = pParams->pOwner;
    missileParams.pOrigin = pParams->pOrigin;
    missileParams.nSkill = pParams->nSkillId;
    missileParams.nSkillLevel = pParams->nLevel;
    missileParams.nMissile = pParams->nHitSubMissile;
    missileParams.nTargetX = CLIENTS_GetUnitX(pTarget);
    missileParams.nTargetY = CLIENTS_GetUnitY(pTarget);
    MISSILES_CreateMissileFromParams(pAuraCallback->pGame, &missileParams);

    return 1;
}

//D2Game.0x6FC5C3A0
int32_t __fastcall MISSMODE_SrvHit21_BattleCry(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner || !pUnit)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);

    if (!pSkillsTxtRecord || pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 1;
    }

    int32_t nAuraFilter = pSkillsTxtRecord->dwAuraFilter;
    if (nAuraFilter)
    {
        nAuraFilter = 42883;
    }

    if (!sub_6FD0FA00(pOwner, pUnit, nAuraFilter))
    {
        return 1;
    }

    D2CurseStrc curse = {};

    curse.pUnit = pOwner;
    curse.pTarget = pUnit;
    curse.nSkill = nSkillId;
    curse.nSkillLevel = nLevel;
    curse.nDuration = SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nLevel);
    curse.nState = pSkillsTxtRecord->wAuraTargetState;
    curse.pStateRemoveCallback = nullptr;

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    if (pStatList)
    {
        sub_6FCFE0E0(pUnit, pStatList, pSkillsTxtRecord, nSkillId, nLevel);
    }

    return 0;
}

//D2Game.0x6FC5C4D0
int32_t __fastcall MISSMODE_SrvHit22_FistOfTheHeavensDelay(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, MISSILE_GetTargetX(pMissile), MISSILE_GetTargetY(pMissile));
    if (!pTarget)
    {
        return 0;
    }

    int32_t nAuraRange = pMissilesTxtRecord->dwHitPar[0];
    if (nAuraRange <= 0)
    {
        nAuraRange = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nLevel), 1);
    }

    int32_t nParam = pMissilesTxtRecord->dwHitPar[1];
    if (nParam <= 0)
    {
        nParam = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[3], nSkillId, nLevel), 1);
    }

    D2DamageStrc damage = {};
    MISSMODE_FillDamageParams(pMissile, pTarget, &damage);
    MISSMODE_SetDamageFlags(pGame, pMissile, pTarget, &damage);

    if (pTarget->dwUnitType == UNIT_MONSTER && !MONSTERS_GetHirelingTypeId(pTarget))
    {
        const int32_t nDamageTargetAc = STATLIST_UnitGetStatValue(pMissile, STAT_ITEM_DAMAGETARGETAC, 0);
        if (nDamageTargetAc)
        {
            const int32_t nArmorClass = std::max(nDamageTargetAc + STATLIST_UnitGetStatValue(pTarget, STAT_ARMORCLASS, 0), 0);
            STATLIST_SetUnitStat(pTarget, STAT_ARMORCLASS, nArmorClass, 0);
        }
    }

    int32_t nAuraFilter = pSkillsTxtRecord->dwAuraFilter;
    if (!nAuraFilter)
    {
        nAuraFilter = 0xA683u;
    }

    D2HitFunc22ParamStrc params = {};
    params.pOwner = pOwner;
    params.pOrigin = pMissile;
    params.nSkillId = nSkillId;
    params.nLevel = nLevel;
    params.nParam = nParam;
    params.nHitSubMissile = pMissilesTxtRecord->wHitSubMissile[0];
    sub_6FD0FE80(pGame, pOwner, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), nAuraRange, nAuraFilter, MISSMODE_FistOfTheHeavensDelay_AuraCallback, &params, 0, __FILE__, __LINE__);
    return 1;
}

//D2Game.0x6FC5C790
int32_t __fastcall MISSMODE_FistOfTheHeavensDelay_AuraCallback(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pTarget)
{
    D2HitFunc22ParamStrc* pParams = (D2HitFunc22ParamStrc*)pAuraCallback->pArgs;
    if (pParams->nParam > 0 && pAuraCallback->nCounter >= pParams->nParam)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x20;
    missileParams.pOwner = pParams->pOwner;
    missileParams.pOrigin = pParams->pOrigin;
    missileParams.nSkill = pParams->nSkillId;
    missileParams.nSkillLevel = pParams->nLevel;
    missileParams.nMissile = pParams->nHitSubMissile;
    missileParams.nTargetX = CLIENTS_GetUnitX(pTarget);
    missileParams.nTargetY = CLIENTS_GetUnitY(pTarget);
    MISSILES_CreateMissileFromParams(pAuraCallback->pGame, &missileParams);

    return 1;
}

//D2Game.0x6FC5C860
int32_t __fastcall MISSMODE_CreatePantherPotGreenSubmissiles(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep)
{
    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(nMissileId);
    if (!pMissilesTxtRecord)
    {
        return 0;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x1F;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pOrigin;
    missileParams.nMissile = nMissileId;
    missileParams.nX = CLIENTS_GetUnitX(pOrigin);
    missileParams.nY = CLIENTS_GetUnitY(pOrigin);
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nSkillLevel;
    missileParams.nLoops = nSkillLevel - 1;
    missileParams.nVelocity = pMissilesTxtRecord->dwParam[0] << 7;

    constexpr int32_t xOffsets[] =
    {
        0, 2, 2, 2, 0, -2, -2, -2
    };

    constexpr int32_t yOffsets[] =
    {
        2, 2, 0, -2, -2, -2, 0, 2
    };

    for (int32_t i = 0; i < 8; i += nStep)
    {
        missileParams.nTargetX = xOffsets[i];
        missileParams.nTargetY = yOffsets[i];
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return 1;
}

//D2Game.0x6FC5C990
int32_t __fastcall MISSMODE_SrvHit25_PantherPotGreen(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2UnitStrc* pOrigin = pUnit;
    if (!pUnit)
    {
        pOrigin = pMissile;
    }

    const int32_t nHitPar = std::max(pMissilesTxtRecord->dwHitPar[0], 1u);
    MISSMODE_CreatePantherPotGreenSubmissiles(pGame, pOwner, pOrigin, pMissilesTxtRecord->wHitSubMissile[0], MISSILE_GetSkill(pMissile), MISSILE_GetLevel(pMissile), nHitPar);
    return 3;
}

//D2Game.0x6FC5CA50
int32_t __fastcall MISSMODE_SrvHit26_GrimWardStart(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 1;
    }

    int32_t nRange = pMissilesTxtRecord->dwHitPar[0];
    if (nRange <= 0)
    {
        nRange = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nLevel), 5);
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 32769;
    missileParams.pOwner = pOwner;
    missileParams.nRange = nRange;
    missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[0];
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nLevel;
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    MISSILES_CreateMissileFromParams(pGame, &missileParams);
    return 1;
}

//D2Game.0x6FC5CBE0
int32_t __fastcall MISSMODE_SrvHit27_GrimWard(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    return 1;
}

//D2Game.0x6FC5CBF0
int32_t __fastcall MISSMODE_SrvHit28_GrimWardScare(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !SUNIT_GetOwner(pGame, pMissile) || !pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 1;
    }

    D2UnitStrc* pSubMissile = SUNIT_GetServerUnit(pGame, UNIT_MISSILE, MISSILE_GetTargetX(pMissile));
    if (!pSubMissile)
    {
        return 1;
    }

    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    const int32_t nDistance = nLevel > 0 ? pSkillsTxtRecord->dwParam[0] + (nLevel - 1) * pSkillsTxtRecord->dwParam[1] : 0;

    if (PATH_ComputeSquaredDistance(CLIENTS_GetUnitX(pSubMissile), CLIENTS_GetUnitY(pSubMissile), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit)) < nDistance * nDistance)
    {
        AIUTIL_ApplyTerrorCurseState(pGame, pSubMissile, pUnit, nSkillId, pSkillsTxtRecord->dwParam[4], pSkillsTxtRecord->dwParam[5]);
    }
    return 1;
}

//D2Game.0x6FC5CE10
int32_t __fastcall MISSMODE_SrvHit29_FrozenOrb(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    if (MISSILE_GetCurrentFrame(pMissile))
    {
        return 2;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 2;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[0];

    const int32_t nParam = std::max(pMissilesTxtRecord->dwHitPar[0], 1u);

    const int32_t xOffsets[64] =
    {
        30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2, 0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29,
    };

    const int32_t yOffsets[64] =
    {
        0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29, 30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2,
    };

    for (int32_t i = 0; i < 64; i += nParam)
    {
        missileParams.nTargetX = xOffsets[i];
        missileParams.nTargetY = yOffsets[i];

        D2UnitStrc* pHitSubMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
        if (pHitSubMissile)
        {
            MISSILE_SetTargetX(pHitSubMissile, xOffsets[i]);
            MISSILE_SetTargetY(pHitSubMissile, yOffsets[i]);
        }
    }

    return 3;
}

//D2Game.0x6FC5CF50
int32_t __fastcall MISSMODE_SrvHit05_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 2;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[0];

    const int32_t nParam = std::max(pMissilesTxtRecord->dwHitPar[0], 1u);

    const int32_t xOffsets[64] =
    {
        30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2, 0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29
    };

    const int32_t yOffsets[64] =
    {
        0, 2, 5, 8, 11, 14, 16, 19, 21, 23, 24, 26, 27, 28, 29, 29, 30, 29, 29, 28, 27, 26, 24, 23, 21, 19, 16, 14, 11, 8, 5, 2, 0, -2, -5, -8, -11, -14, -16, -19, -21, -23, -24, -26, -27, -28, -29, -29, -30, -29, -29, -28, -27, -26, -24, -23, -21, -19, -16, -14, -11, -8, -5, -2
    };

    for (int32_t i = 0; i < 64; i += nParam)
    {
        missileParams.nTargetX = xOffsets[i];
        missileParams.nTargetY = yOffsets[i];

        D2UnitStrc* pHitSubMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
        if (pHitSubMissile)
        {
            MISSILE_SetTargetX(pHitSubMissile, xOffsets[i]);
            MISSILE_SetTargetY(pHitSubMissile, yOffsets[i]);
        }
    }

    return 3;
}

//D2Game.0x6FC5D070
int32_t __fastcall MISSMODE_SrvHit31_FireHead(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner || !pUnit)
    {
        return 1;
    }

    D2DamageStrc damage = {};
    const int32_t nHeal = std::max(MISSMODE_GetDamageValue(pGame, pMissile, pUnit, &damage), 0);
    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pOwner);
    const int32_t nNewHp = std::min(nHeal + (int32_t)STATLIST_UnitGetStatValue(pOwner, STAT_HITPOINTS, 0), nMaxHp);
    STATLIST_SetUnitStat(pOwner, STAT_HITPOINTS, nNewHp, 0);

    if (pMissilesTxtRecord->nCollideKill)
    {
        return 3;
    }

    return 2;
}

//D2Game.0x6FC5D160
int32_t __fastcall MISSMODE_SrvHit32_CairnStones(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 0;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 0;
    }

    if (!pUnit && !MISSILE_GetTargetX(pMissile))
    {
        MISSILE_SetTargetX(pMissile, 1);
        D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
        D2GAME_CreatePortalObject_6FD13DF0(pGame, pOwner, pRoom, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), pMissilesTxtRecord->dwParam[3], 0, 60u, 1);
        DUNGEON_ToggleHasPortalFlag(pRoom, 1);
    }

    return 0;
}

//D2Game.0x6FC5D270
int32_t __fastcall MISSMODE_SrvHit33_TowerChestSpawner(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
        if (pRoom)
        {
            DUNGEON_ToggleHasPortalFlag(pRoom, 1);
        }
    }

    return 0;
}

//D2Game.0x6FC5D290
int32_t __fastcall MISSMODE_SrvHit35_OrbMist(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        return 0;
    }

    D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, MISSILE_GetTargetX(pMissile));
    if (!pObject)
    {
        return 0;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pObject);
    if (pRoom)
    {
        DUNGEON_ToggleHasPortalFlag(pRoom, 1);
    }

    if (pObject->dwAnimMode == 0)
    {
        UNITS_ChangeAnimMode(pObject, 1);
        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pObject->dwClassId);
        if (pObjectsTxtRecord)
        {
            EVENT_SetEvent(pGame, pObject, 1, pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);
        }
    }

    return 1;
}

//D2Game.0x6FC5D320
int32_t __fastcall MISSMODE_SrvHit36_MissileInAir(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    if (pUnit)
    {
        return 0;
    }

    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    D2UnitStrc* pHitSubMissile = D2GAME_CreateMissile_6FD115E0(pGame, pOwner, nSkillId, nLevel, pMissilesTxtRecord->wHitSubMissile[0], CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile));
    if (pHitSubMissile)
    {
        MISSILE_SetTargetX(pHitSubMissile, MISSILE_GetTargetX(pMissile));
    }
    return 1;
}

//D2Game.0x6FC5D430
int32_t __fastcall MISSMODE_SrvHit37_BladeCreeper(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        return 2;
    }

    return 0;
}

//D2Game.0x6FC5D440
void __fastcall MISSMODE_CatapultChargedBall_LightningTrailingJavelin_SubmissileInitFunc(D2UnitStrc* pMissile, int32_t nUnused)
{
    if (!pMissile)
    {
        return;
    }

    const int32_t nRange = std::min(MISSILE_GetTotalFrames(pMissile), 255);

    SEED_InitLowSeed(&pMissile->pSeed, D2COMMON_10175_PathGetFirstPointX(pMissile->pDynamicPath));
    PATH_SetType(pMissile->pDynamicPath, PATHTYPE_CHARGEDBOLT);
    PATH_SetNewDistance(pMissile->pDynamicPath, nRange);
    D2Common_10142(pMissile->pDynamicPath, pMissile, 0);
}

//D2Game.0x6FC5D490
int32_t __fastcall MISSMODE_SrvHit38_CatapultChargedBall(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 3;
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    missileParams.pOwner = pOwner;
    missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[0];
    missileParams.nSkill = nSkillId;
    missileParams.nSkillLevel = nLevel;
    missileParams.pInitFunc = MISSMODE_CatapultChargedBall_LightningTrailingJavelin_SubmissileInitFunc;

    int32_t nCalc = pMissilesTxtRecord->dwHitPar[0] + pMissilesTxtRecord->dwHitPar[1] * (MISSILE_GetLevel(pMissile) - 1);
    if (nCalc <= 0)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 1;
        }

        nCalc = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[3], nSkillId, nLevel), 1);
    }

    sub_6FD14260(pGame, nCalc, &missileParams);
    return 1;
}

//D2Game.0x6FC5D620
int32_t __fastcall MISSMODE_SrvHit39_ImpSpawnMonsters(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    MONSTERSPAWN_SpawnRandomMonsterForLevel(pGame, UNITS_GetRoom(pMissile), CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile));
    return 1;
}

//D2Game.0x6FC5D6A0
int32_t __fastcall MISSMODE_SrvHit40_CatapultSpikeBall(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    const int32_t nSkillId = MISSILE_GetSkill(pMissile);

    int32_t nCalc = pMissilesTxtRecord->dwHitPar[0] + (nLevel - 1) * pMissilesTxtRecord->dwHitPar[1];
    if (nCalc <= 0)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 1;
        }

        nCalc = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[3], nSkillId, nLevel), 1);
    }

    sub_6FCF8330(pGame, pMissile, pOwner, pMissilesTxtRecord->wHitSubMissile[0], nCalc, nCalc / 4, nSkillId, nLevel);
    return 1;
}

//D2Game.0x6FC5D7C0
int32_t __fastcall MISSMODE_SrvHit43_HealingVortex(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || !pUnit)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);

    if (!SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    const int32_t nMinDamage = SKILLS_GetMinPhysDamage(pOwner, nSkillId, nLevel, 1);
    const int32_t nMaxDamage = SKILLS_GetMaxPhysDamage(pOwner, nSkillId, nLevel, 1);
    const int32_t nDamage = ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, nMaxDamage - nMinDamage) + nMinDamage;

    const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);

    if (nHitpoints != nMaxHp && pMissilesTxtRecord->wProgOverlay > 0)
    {
        UNITS_SetOverlay(pUnit, pMissilesTxtRecord->wProgOverlay, 0);
    }

    const int32_t nNewHp = std::min(nDamage + nHitpoints, nMaxHp);
    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);

    return pMissilesTxtRecord->nCollideKill != 0;
}

//D2Game.0x6FC5D950
int32_t __fastcall MISSMODE_SrvHit45_LightningTrailingJavelin(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0 || pMissilesTxtRecord->dwHitPar[0] <= 0)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 3;
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
    missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[0];
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    if (pMissilesTxtRecord->dwHitPar[1])
    {
        missileParams.pInitFunc = MISSMODE_CatapultChargedBall_LightningTrailingJavelin_SubmissileInitFunc;
    }

    sub_6FD14260(pGame, pMissilesTxtRecord->dwHitPar[0], &missileParams);
    return 1;
}

//D2Game.0x6FC5DA80
int32_t __fastcall MISSMODE_SrvHit47_MoltenBoulder(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 2;
    }

    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
    if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_LARGE]))
    {
        return 2;
    }

    const int32_t nX = CLIENTS_GetUnitX(pMissile);
    const int32_t nY = CLIENTS_GetUnitY(pMissile);

    int32_t nResult = 1;

    D2DamageStrc damage = {};
    MISSMODE_FillDamageParams(pMissile, pUnit, &damage);

    int32_t nRange = pMissilesTxtRecord->dwHitPar[0];
    if (nRange <= 0)
    {
        const int32_t nSkillId = MISSILE_GetSkill(pMissile);
        const int32_t nLevel = MISSILE_GetLevel(pMissile);
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 1;
        }

        nRange = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nLevel), 1);
    }

    damage.dwHitFlags |= pMissilesTxtRecord->dwHitFlags;
    damage.wResultFlags |= pMissilesTxtRecord->wResultFlags;

    if (!sub_6FD10200(pGame, pOwner, nX, nY, nRange, &damage, 0))
    {
        nResult = 3;
    }

    const int32_t nParam = std::max(pMissilesTxtRecord->dwHitPar[1], 1u);
    MISSMODE_CreateMeteor_MoltenBoulderSubmissiles(pGame, pMissile, pMissilesTxtRecord->wHitSubMissile[0], nRange, nParam);
    return nResult;
}

//D2Game.0x6FC5DD10
int32_t __fastcall MISSMODE_SrvHit48_MoltenBoulderEmerge(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 33;
    missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
    missileParams.nX = CLIENTS_GetUnitX(pMissile);
    missileParams.nY = CLIENTS_GetUnitY(pMissile);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[0];
    missileParams.nTargetX = D2COMMON_10175_PathGetFirstPointX(pMissile->pDynamicPath);
    missileParams.nTargetY = D2COMMON_10176_PathGetFirstPointY(pMissile->pDynamicPath);
    MISSILES_CreateMissileFromParams(pGame, &missileParams);
    return 1;
}

//D2Game.0x6FC5DE50
int32_t __fastcall MISSMODE_SrvHit50_PlagueVinesTrail(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    if (pUnit && (MISSILE_GetRemainingFrames(pMissile) < (int32_t)(MISSILE_GetTotalFrames(pMissile) - pMissilesTxtRecord->dwHitPar[0])))
    {
        return 2;
    }

    return 0;
}

//D2Game.0x6FC5DED0
int32_t __fastcall MISSMODE_SrvHit51_VolcanoDebris(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    if (pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.pOwner = SUNIT_GetOwner(pGame, pMissile);
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.pOrigin = pMissile;
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[0];
    MISSILES_CreateMissileFromParams(pGame, &missileParams);

    if (pMissilesTxtRecord->wHitSubMissile[1] >= 0)
    {
        missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[1];
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    if (pMissilesTxtRecord->wHitSubMissile[2] >= 0)
    {
        missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[2];
        MISSILES_CreateMissileFromParams(pGame, &missileParams);
    }

    return 1;
}

//D2Game.0x6FC5DFC0
int32_t __fastcall MISSMODE_SrvHit52_BladeFury(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 2;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);

    const int32_t nParam = std::max(pMissilesTxtRecord->dwHitPar[0], 1u);

    const int32_t xOffsets[] =
    {
        16, 16, 0, -16, -16, -16, 0, 16
    };

    const int32_t yOffsets[] =
    {
        0, 16, 16, 16, 0, -16, -16, -16
    };

    for (int32_t i = 0; i < 8; i += nParam)
    {
        missileParams.nTargetX = xOffsets[i];
        missileParams.nTargetY = yOffsets[i];

        D2UnitStrc* pHitSubMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
        if (pHitSubMissile)
        {
            MISSILE_SetTargetX(pHitSubMissile, xOffsets[i]);
            MISSILE_SetTargetY(pHitSubMissile, yOffsets[i]);
        }
    }

    return 1;
}

//D2Game.0x6FC5E0D0
int32_t __fastcall MISSMODE_SrvHit53_RabiesContagion(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (pOwner && pUnit)
    {
        const int32_t nSkillId = MISSILE_GetSkill(pMissile);
        const int32_t nLevel = MISSILE_GetLevel(pMissile);
        const int32_t nFrame = MISSILE_GetTargetX(pMissile) - pGame->dwGameFrame;

        if (nFrame >= 10 && nFrame <= SKILLS_GetElementalLength(pOwner, nSkillId, nLevel, 1))
        {
            sub_6FCFEDD0(pGame, pOwner, pUnit, nFrame, nSkillId, nLevel);
            return 2;
        }
    }

    return 1;
}

//D2Game.0x6FC5E160
int32_t __fastcall MISSMODE_SrvHit54_BaalSpawnMonsters(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
        if (pOwner)
        {
            D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pOwner, MISSILE_GetSkill(pMissile));
            if (pSkill)
            {
                D2GAME_SpawnPresetMonster_6FC66560(pGame, UNITS_GetRoom(pMissile), SKILLS_GetParam1(pSkill), CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), MONMODE_NEUTRAL);
            }
        }
    }

    return 1;
}

//D2Game.0x6FC5E220
int32_t __fastcall MISSMODE_SrvHit55_Baalnferno(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
    {
        return 2;
    }

    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->dwHitPar[0] <= 0)
    {
        return 1;
    }

    const int32_t nCurrentMana = STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0);
    if (nCurrentMana <= 0)
    {
        return 1;
    }

    const int32_t nPercentage = D2Clamp(pMissilesTxtRecord->dwHitPar[0], 1u, 100u);
    const int32_t nLostMana = D2Clamp(MONSTERUNIQUE_CalculatePercentage(nCurrentMana, nPercentage, 100), 1, nCurrentMana);
    STATLIST_SetUnitStat(pUnit, STAT_MANA, nCurrentMana - nLostMana, 0);
    return 2;
}

//D2Game.0x6FC5E370
int32_t __fastcall MISSMODE_SrvHit56_ArmageddonControl(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);

    const int32_t nX = CLIENTS_GetUnitX(pMissile);
    const int32_t nY = CLIENTS_GetUnitY(pMissile);

    D2DamageStrc damage = {};
    MISSMODE_FillDamageParams(pMissile, 0, &damage);

    int32_t nRange = pMissilesTxtRecord->dwHitPar[0];
    if (nRange <= 0)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxtRecord)
        {
            return 1;
        }

        nRange = std::max(SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nLevel), 1);
    }

    damage.dwHitFlags |= pMissilesTxtRecord->dwHitFlags;
    damage.wResultFlags |= pMissilesTxtRecord->wResultFlags;
    sub_6FD10200(pGame, pOwner, nX, nY, nRange, &damage, 0);

    if (pMissilesTxtRecord->wHitSubMissile[0] >= 0)
    {
        D2GAME_CreateMissile_6FD115E0(pGame, pOwner, nSkillId, nLevel, pMissilesTxtRecord->wHitSubMissile[0], nX, nY);
    }

    return 1;
}

//D2Game.0x6FC5E530
int32_t __fastcall MISSMODE_SrvHit58_BaalTauntLightningControl(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    SEED_InitLowSeed(&pMissile->pSeed, CLIENTS_GetUnitX(pMissile));
    const int32_t nParam = std::max(pMissilesTxtRecord->dwHitPar[0], 1u);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 32;
    missileParams.pOwner = pOwner;
    missileParams.pOrigin = pMissile;
    missileParams.nSkill = MISSILE_GetSkill(pMissile);
    missileParams.nSkillLevel = MISSILE_GetLevel(pMissile);
    missileParams.nMissile = pMissilesTxtRecord->wHitSubMissile[0];

    const int32_t nRange = 2 * nParam + 1;
    missileParams.nTargetX = CLIENTS_GetUnitX(pMissile) + ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, nRange) - nParam;
    missileParams.nTargetY = CLIENTS_GetUnitY(pMissile) + ITEMS_RollLimitedRandomNumber(&pMissile->pSeed, nRange) - nParam;

    MISSILES_CreateMissileFromParams(pGame, &missileParams);
    return 1;
}

//D2Game.0x6FC5E760
int32_t __fastcall MISSMODE_SrvHit59_BaalTauntPoisonControl(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wHitSubMissile[0] < 0)
    {
        return 1;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (!pOwner)
    {
        return 1;
    }

    const int32_t nHitPar = std::max(pMissilesTxtRecord->dwHitPar[0], 1u);
    MISSMODE_CreatePoisonCloudHitSubmissiles(pGame, pOwner, pMissile, pMissilesTxtRecord->wHitSubMissile[0], MISSILE_GetSkill(pMissile), MISSILE_GetLevel(pMissile), nHitPar, 2, 0);
    return 1;
}

//D2Game.0x6FC5E7F0
int32_t __fastcall MISSMODE_SrvHit23_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pMissile)
    {
        return 1;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 1;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    if (nSkillId > 0 && nLevel > 0 && SUNIT_GetOwner(pGame, pMissile))
    {
        if (pUnit)
        {
            UNITS_SetTargetUnitForDynamicUnit(pMissile, pUnit);
        }
        sub_6FD14400(pGame, pMissile, pMissilesTxtRecord->dwHitPar[0], nSkillId, nLevel);
    }
    return 1;
}

//D2Game.0x6FC5E890
void __fastcall MISSMODE_SrvDmg01_FireArrow_MagicArrow_ColdArrow(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pMissile)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return;
    }

    int32_t nPercentage = MISSILE_EvaluateMissileFormula(pMissile, SUNIT_GetOwner(pGame, pMissile), pMissilesTxtRecord->dwDmgCalc, -1, -1);
    if (nPercentage <= 0)
    {
        return;
    }

    nPercentage = std::min(nPercentage, 100);

    const int32_t nDamageReduction = std::min(MONSTERUNIQUE_CalculatePercentage(pDamage->dwPhysDamage, nPercentage, 100), pDamage->dwPhysDamage);

    pDamage->dwPhysDamage -= nDamageReduction;
    MISSMODE_AddDamageValue(pGame, pMissile, pUnit, pDamage, nDamageReduction);
}

//D2Game.0x6FC5E9D0
void __fastcall MISSMODE_SrvDmg12_LightningJavelin(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pMissile)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return;
    }

    int32_t nPercentage = MISSILE_EvaluateMissileFormula(pMissile, SUNIT_GetOwner(pGame, pMissile), pMissilesTxtRecord->dwDmgCalc, -1, -1);
    if (nPercentage <= 0)
    {
        return;
    }

    nPercentage = std::min(nPercentage, 100);

    const int32_t nDamageBonus = std::min(MONSTERUNIQUE_CalculatePercentage(pDamage->dwPhysDamage, nPercentage, 100), pDamage->dwPhysDamage);
    const int32_t nFinalDamage = MISSMODE_GetDamageValue(pGame, pMissile, pUnit, pDamage) + nDamageBonus;
    MISSMODE_ResetDamageParams(pGame, pMissile, pDamage);
    MISSMODE_AddDamageValue(pGame, pMissile, pUnit, pDamage, nFinalDamage);
}

//D2Game.0x6FC5EB20
void __fastcall MISSMODE_SrvDmg02_IceArrow_RoyalStrikeChaos(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pMissile)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (pMissilesTxtRecord)
    {
        const int32_t nPercentage = std::max(pMissilesTxtRecord->dwDmgParam[0], 0);
        pDamage->dwFrzLen = MONSTERUNIQUE_CalculatePercentage(STATLIST_UnitGetStatValue(pMissile, STAT_COLDLENGTH, 0), nPercentage, 100);
        pDamage->dwColdLen = 0;
    }
}

//D2Game.0x6FC5EC70
void __fastcall MISSMODE_SrvDmg03_Blaze_FireWall_ImmolationFire_MeteorFire(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (pMissile)
    {
        D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
        if (pMissilesTxtRecord && (ITEMS_RollRandomNumber(&pMissile->pSeed) & 127) < pMissilesTxtRecord->dwDmgParam[0])
        {
            pDamage->wResultFlags |= 0x4000u;
        }
    }
}

//D2Game.0x6FC5ECE0
void __fastcall MISSMODE_SrvDmg04_IceBlast(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    pDamage->dwFrzLen = pDamage->dwColdLen;
    pDamage->dwColdLen = 0;
}

//D2Game.0x6FC5ED00
void __fastcall MISSMODE_SrvDmg10_BladesOfIceCubes(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (pMissile && SKILLS_GetMissilesTxtRecord(pMissile->dwClassId))
    {
        pDamage->dwFrzLen = pDamage->dwColdLen;
    }
}

//D2Game.0x6FC5ED50
void __fastcall MISSMODE_SrvDmg05_BlessedHammer(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pMissile)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return;
    }

    const int32_t nBaseDamage = MISSMODE_GetDamageValue(pGame, pMissile, pUnit, pDamage);
    if (pMissilesTxtRecord->dwDmgParam[0] > 0 && MONSTERS_IsUndead(pUnit))
    {
        MISSMODE_AddDamageValue(pGame, pMissile, pUnit, pDamage, MONSTERUNIQUE_CalculatePercentage(nBaseDamage, pMissilesTxtRecord->dwDmgParam[0], 100));
    }

    if (pMissilesTxtRecord->dwDmgParam[1] > 0 && MONSTERS_IsDemon(pUnit))
    {
        MISSMODE_AddDamageValue(pGame, pMissile, pUnit, pDamage, MONSTERUNIQUE_CalculatePercentage(nBaseDamage, pMissilesTxtRecord->dwDmgParam[1], 100));
    }
}

//D2Game.0x6FC5EF40
void __fastcall MISSMODE_SrvDmg06_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (pOwner && !(pOwner->dwFlags & UNITFLAG_ISMERC) && pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_NOTC;
    }
}

//D2Game.0x6FC5EF80
void __fastcall MISSMODE_SrvDmg07_Warcry_ShockWave(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pMissile)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    const int32_t nLevel = MISSILE_GetLevel(pMissile);
    if (pMissilesTxtRecord->dwDmgParam[0] > 0)
    {
        pDamage->dwStunLen = pMissilesTxtRecord->dwDmgParam[0];
    }
    else
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (pSkillsTxtRecord && nLevel > 0)
        {
            pDamage->dwStunLen = pSkillsTxtRecord->dwParam[0] + (nLevel - 1) * pSkillsTxtRecord->dwParam[1];
        }
        else
        {
            pDamage->dwStunLen = 0;
        }
    }
    pDamage->dwHitClass = 0x60u;
}

//D2Game.0x6FC5F060
void __fastcall MISSMODE_SrvDmg08_EruptionCrack(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (pMissile)
    {
        D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
        if (pMissilesTxtRecord && pUnit && pMissilesTxtRecord->wProgOverlay > 0)
        {
            UNITS_SetOverlay(pUnit, pMissilesTxtRecord->wProgOverlay, 0);
        }
    }
}

//D2Game.0x6FC5F0C0
void __fastcall MISSMODE_SrvDmg09_Twister(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pMissile)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return;
    }

    const int32_t nSkillId = MISSILE_GetSkill(pMissile);
    if (pMissilesTxtRecord->dwDmgParam[0] > 0)
    {
        pDamage->dwStunLen = pMissilesTxtRecord->dwDmgParam[0];
    }
    else
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (pSkillsTxtRecord)
        {
            pDamage->dwStunLen = pSkillsTxtRecord->dwParam[1];
        }
        else
        {
            pDamage->dwStunLen = 0;
        }
    }
    pDamage->dwHitClass = 0x60u;
}

//D2Game.0x6FC5F170
void __fastcall MISSMODE_SrvDmg11_RabiesContagion(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
    if (pOwner && pUnit)
    {
        const int32_t nSkillId = MISSILE_GetSkill(pMissile);
        const int32_t nLevel = MISSILE_GetLevel(pMissile);
        const int32_t nPoisonLength = MISSILE_GetTargetX(pMissile) - pGame->dwGameFrame;
        if (nPoisonLength >= 10 && nPoisonLength <= SKILLS_GetElementalLength(pOwner, nSkillId, nLevel, 1))
        {
            pDamage->dwPoisLen = nPoisonLength;
            return;
        }
    }

    pDamage->dwPoisDamage = 0;
    pDamage->dwPoisLen = 0;
}

//D2Game.0x6FC5F1F0
void __fastcall MISSMODE_SrvDmg13_BlessedHammerEx(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
        if (pOwner)
        {
            pDamage->dwPhysDamage += (int32_t)(pDamage->dwPhysDamage * STATLIST_UnitGetStatValue(pOwner, STAT_DAMAGEPERCENT, 0)) / 100;
        }

        MISSMODE_SrvDmg05_BlessedHammer(pGame, pMissile, pUnit, pDamage);
    }
}

//D2Game.0x6FC5F250
void __fastcall MISSMODE_SrvDmg14_MoltenBoulder(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pMissile || pMissile->dwUnitType != UNIT_MISSILE || !pUnit)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return;
    }

    uint32_t nChance = 0;
    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    {
        if (pMissilesTxtRecord->dwDmgParam[0] < 1)
        {
            return;
        }

        nChance = pMissilesTxtRecord->dwDmgParam[1];
        break;
    }
    case UNIT_MONSTER:
    {
        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
        if (!pMonStats2TxtRecord)
        {
            return;
        }

        const int32_t nParam = pMissilesTxtRecord->dwDmgParam[0];
        if (nParam < 1)
        {
            if (!(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_SMALL]))
            {
                return;
            }

            nChance = pMissilesTxtRecord->dwDmgParam[1];
        }
        else if (nParam == 1)
        {
            if (pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_SMALL])
            {
                nChance = 2 * pMissilesTxtRecord->dwDmgParam[1];
            }
            else if (!(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_LARGE]))
            {
                nChance = pMissilesTxtRecord->dwDmgParam[1];
            }
            else
            {
                return;
            }
        }
        else
        {
            if (pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_SMALL])
            {
                nChance = 3 * pMissilesTxtRecord->dwDmgParam[1];
            }
            else if (!(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_LARGE]))
            {
                nChance = 2 * pMissilesTxtRecord->dwDmgParam[1];
            }
            else
            {
                nChance = pMissilesTxtRecord->dwDmgParam[1];
            }
        }
    }
    default:
    {
        return;
    }
    }

    if (nChance > 0 && (ITEMS_RollRandomNumber(&pMissile->pSeed) % 100) < nChance)
    {
        pDamage->wResultFlags |= 8u;
    }
}

//D2Game.0x6FC5F4B0
D2MonStats2Txt* __fastcall D2GAME_GetMonStats2TxtRecord_6FC5F4B0(int32_t nRecordId)
{
    if (nRecordId >= 0 && nRecordId < sgptDataTables->nMonStats2TxtRecordCount)
    {
        return &sgptDataTables->pMonStats2Txt[nRecordId];
    }

    return nullptr;
}

//D2Game.0x6FC5F4E0
void __fastcall MISSMODE_SetDamageFlags(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pDefender, D2DamageStrc* pDamage)
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
    if (!pMissilesTxtRecord)
    {
        return;
    }

    pDamage->wResultFlags |= DAMAGERESULTFLAG_SUCCESSFULHIT;

    if (!STATES_CheckState(pDefender, STATE_UNINTERRUPTABLE))
    {
        if (pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_GETHIT])
        {
            pDamage->wResultFlags |= DAMAGERESULTFLAG_GETHIT;
        }
        else if (pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_SOFTHIT])
        {
            pDamage->wResultFlags |= DAMAGERESULTFLAG_SOFTHIT;
        }

        if (pMissilesTxtRecord->nKnockBack && (ITEMS_RollRandomNumber(&pMissile->pSeed) % 100) < pMissilesTxtRecord->nKnockBack)
        {
            pDamage->wResultFlags |= DAMAGERESULTFLAG_KNOCKBACK;
        }
    }

    uint32_t nBlockFlags = SUNITDMG_ApplyBlockOrDodge(pGame, pOwner, pDefender, 1, pDamage->dwPhysDamage != 0);
    if (nBlockFlags & 0x1F && !(pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_GETHIT]) && !pMissilesTxtRecord->nKnockBack)
    {
        pDamage->wResultFlags = pDamage->wResultFlags & (uint16_t)~DAMAGERESULTFLAG_SUCCESSFULHIT | DAMAGERESULTFLAG_SOFTHIT;
        nBlockFlags &= 0xFFFFFFE0;
    }

    if (nBlockFlags & 8)
    {
        pDamage->wResultFlags |= DAMAGERESULTFLAG_EVADE;
    }

    if (nBlockFlags & 2)
    {
        pDamage->wResultFlags |= DAMAGERESULTFLAG_AVOID;
    }

    if (nBlockFlags & 0x11)
    {
        pDamage->wResultFlags |= DAMAGERESULTFLAG_BLOCK;
    }

    if (nBlockFlags)
    {
        pDamage->wResultFlags &= (uint16_t)~DAMAGERESULTFLAG_SUCCESSFULHIT;
    }

    pDamage->dwHitClass = pDamage->dwHitClass & 0xF0 | MISSILE_GetHitClass(pMissile);

    if (pDamage->dwHitClass & 0xF0)
    {
        pDamage->nHitClassActiveSet = 1;
    }

    D2GAME_MONSTER_ApplyCriticalDamage_6FC62E70(pOwner, pDefender, pDamage);

    if (MISSILE_GetFlags(pMissile) & 1)
    {
        pDamage->dwHitFlags |= DAMAGEHITFLAG_32;
    }

    if (MISSILE_GetFlags(pMissile) & 2)
    {
        pDamage->dwHitFlags |= DAMAGEHITFLAG_128;
    }

    pDamage->dwPiercePct = STATLIST_UnitGetStatValue(pMissile, STAT_DAMAGE_FRAMERATE, 0);

    if (pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
    {
        SUNITDMG_ExecuteEvents(pGame, pOwner, pDefender, 1, pDamage);
    }

    SUNITDMG_ExecuteMissileDamage(pGame, pOwner, pDefender, pDamage);
}

//D2Game.0x6FC5F6C0
int32_t __fastcall MISSMODE_SrvDo19_RadamentDeath(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    const int32_t nCurrentFrame = MISSILE_GetCurrentFrame(pMissile);
    if (nCurrentFrame > 1 && nCurrentFrame < 25)
    {
        sub_6FD14BD0(pGame, 0x3002, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), pMissile, 124, 1, MISSMODE_RadamentDeathAreaEffectCallback);
    }
    else if (nCurrentFrame == 1)
    {
        sub_6FD14BD0(pGame, 0x3002, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), pMissile, 124, 1, MISSMODE_RadamentDeathAreaEffectCallbackFirstFrame);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC5F7C0
int32_t __fastcall MISSMODE_RadamentDeathAreaEffectCallback(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pCorpse, int32_t nSkillLevel, int32_t nUnused)
{
    return SKILLS_ApplyRedemptionEffect(pGame, pUnit, pCorpse, SKILL_REDEMPTION, nSkillLevel, 0);
}

//D2Game.0x6FC5F7E0
int32_t __fastcall MISSMODE_RadamentDeathAreaEffectCallbackFirstFrame(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pCorpse, int32_t nSkillLevel, int32_t nUnused)
{
    return SKILLS_ApplyRedemptionEffect(pGame, pUnit, pCorpse, SKILL_REDEMPTION, nSkillLevel, 1);
}

//D2Game.0x6FC5F800
int32_t __fastcall MISSMODE_SrvDo36_BaalFxControl(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    const int32_t nFrame = MISSILE_GetCurrentFrame(pMissile);
    if (!MISSILE_GetTargetX(pMissile) && nFrame <= 100)
    {
        MISSILE_SetTargetX(pMissile, 1);
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
        if (QUESTS_CheckNotIntroQuest(pGame, 36))
        {
            ACT5Q6_SpawnTyrael(pGame, pRoom, pMissile);
        }

        DUNGEON_ToggleHasPortalFlag(pRoom, 1);
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC5F860
int32_t __fastcall MISSMODE_SrvHit57_BaalFxControl(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit)
{
    if (!pUnit && !MISSILE_GetTargetX(pMissile))
    {
        MISSILE_SetTargetX(pMissile, 1);
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
        if (QUESTS_CheckNotIntroQuest(pGame, 36))
        {
            ACT5Q6_SpawnTyrael(pGame, pRoom, pMissile);
        }

        DUNGEON_ToggleHasPortalFlag(pRoom, 1);
    }

    return 0;
}

//D2Game.0x6FC5F8B0
int32_t __fastcall MISSMODE_SrvDo37_Unused(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    if (!pMissile)
    {
        return 2;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord)
    {
        return 2;
    }

    if (pMissilesTxtRecord->wSubMissile[0] > 0 && MISSILE_GetRemainingFrames(pMissile) == pMissilesTxtRecord->dwParam[0])
    {
        D2GAME_CreateMissile_6FD115E0(pGame, SUNIT_GetOwner(pGame, pMissile), MISSILE_GetSkill(pMissile), MISSILE_GetLevel(pMissile),
                                      pMissilesTxtRecord->wSubMissile[0], CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile));
    }

    if (pMissilesTxtRecord->wSubMissile[1] > 0 && MISSILE_GetRemainingFrames(pMissile) == pMissilesTxtRecord->dwParam[1])
    {
        D2GAME_CreateMissile_6FD115E0(pGame, SUNIT_GetOwner(pGame, pMissile), MISSILE_GetSkill(pMissile), MISSILE_GetLevel(pMissile),
                                      pMissilesTxtRecord->wSubMissile[1], CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile));
    }

    if (pMissilesTxtRecord->wSubMissile[2] > 0 && MISSILE_GetRemainingFrames(pMissile) == pMissilesTxtRecord->dwParam[2])
    {
        D2GAME_CreateMissile_6FD115E0(pGame, SUNIT_GetOwner(pGame, pMissile), MISSILE_GetSkill(pMissile), MISSILE_GetLevel(pMissile),
                                      pMissilesTxtRecord->wSubMissile[2], CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile));
    }

    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC5F8C0
int32_t __fastcall MISSMODE_SrvDo01_BasicMissile(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    return MISSMODE_HandleMissileCollision(pGame, pMissile);
}

//D2Game.0x6FC5FAD0
int32_t __fastcall MISSMODE_SrvDmgHitHandler(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, int32_t a4)
{
    using MissileDmgFunc = void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, D2DamageStrc*);
    constexpr MissileDmgFunc gpMissileSrvDmgFnTable_6FD2E838[] =
    {
        nullptr,
        MISSMODE_SrvDmg01_FireArrow_MagicArrow_ColdArrow,
        MISSMODE_SrvDmg02_IceArrow_RoyalStrikeChaos,
        MISSMODE_SrvDmg03_Blaze_FireWall_ImmolationFire_MeteorFire,
        MISSMODE_SrvDmg04_IceBlast,
        MISSMODE_SrvDmg05_BlessedHammer,
        MISSMODE_SrvDmg06_Unused,
        MISSMODE_SrvDmg07_Warcry_ShockWave,
        MISSMODE_SrvDmg08_EruptionCrack,
        MISSMODE_SrvDmg09_Twister,
        MISSMODE_SrvDmg10_BladesOfIceCubes,
        MISSMODE_SrvDmg11_RabiesContagion,
        MISSMODE_SrvDmg12_LightningJavelin,
        MISSMODE_SrvDmg13_BlessedHammerEx,
        MISSMODE_SrvDmg14_MoltenBoulder,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };

    using MissileHitFunc = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*);
    constexpr MissileHitFunc gpMissileSrvHitFnTable_6FD2E718[] =
    {
        nullptr,
        MISSMODE_SrvHit01_Fireball_ExplodingArrow_FreezingArrowExplosion,
        MISSMODE_SrvHit02_PlagueJavelin_PoisonPotion,
        MISSMODE_SrvHit03_ExplosivePotion_BombOnGround,
        MISSMODE_SrvHit04_ExplodingArrow_FreezingArrow_RoyalStrikeMeteorCenter,
        MISSMODE_SrvHit05_Unused,
        MISSMODE_SrvHit06_Unused,
        MISSMODE_SrvHit07_HolyBolt_FistOfTheHeavenBolt,
        MISSMODE_SrvHit08_Blaze,
        MISSMODE_SrvHit09_ImmolationArrow,
        MISSMODE_SrvHit10_GuidedArrow_BoneSpirit,
        MISSMODE_SrvHit11_Unused,
        MISSMODE_SrvHit12_ChainLightning_LightningStrike,
        MISSMODE_SrvHit13_GlacialSpike_HellMeteorDown,
        MISSMODE_SrvHit14_MeteorCenter_CatapultMeteor_RoyalStrikeMeteor,
        MISSMODE_SrvHit15_SpiderGooLay,
        MISSMODE_SrvHit16_SpiderGoo_VinesTrail_VinesWither,
        MISSMODE_SrvHit17_Howl,
        MISSMODE_SrvHit18_Shout_BattleCommand_BattleOrders,
        MISSMODE_SrvHit19_FingerMageSpider,
        MISSMODE_SrvHit20_LightningFury,
        MISSMODE_SrvHit21_BattleCry,
        MISSMODE_SrvHit22_FistOfTheHeavensDelay,
        MISSMODE_SrvHit23_Unused,
        MISSMODE_SrvHit24_PantherPotOrange,
        MISSMODE_SrvHit25_PantherPotGreen,
        MISSMODE_SrvHit26_GrimWardStart,
        MISSMODE_SrvHit27_GrimWard,
        MISSMODE_SrvHit28_GrimWardScare,
        MISSMODE_SrvHit29_FrozenOrb,
        nullptr,
        MISSMODE_SrvHit31_FireHead,
        MISSMODE_SrvHit32_CairnStones,
        MISSMODE_SrvHit33_TowerChestSpawner,
        nullptr,
        MISSMODE_SrvHit35_OrbMist,
        MISSMODE_SrvHit36_MissileInAir,
        MISSMODE_SrvHit37_BladeCreeper,
        MISSMODE_SrvHit38_CatapultChargedBall,
        MISSMODE_SrvHit39_ImpSpawnMonsters,
        MISSMODE_SrvHit40_CatapultSpikeBall,
        nullptr,
        nullptr,
        MISSMODE_SrvHit43_HealingVortex,
        MISSMODE_SrvHit44_ExplodingJavelin,
        MISSMODE_SrvHit45_LightningTrailingJavelin,
        nullptr,
        MISSMODE_SrvHit47_MoltenBoulder,
        MISSMODE_SrvHit48_MoltenBoulderEmerge,
        nullptr,
        MISSMODE_SrvHit50_PlagueVinesTrail,
        MISSMODE_SrvHit51_VolcanoDebris,
        MISSMODE_SrvHit52_BladeFury,
        MISSMODE_SrvHit53_RabiesContagion,
        MISSMODE_SrvHit54_BaalSpawnMonsters,
        MISSMODE_SrvHit55_Baalnferno,
        MISSMODE_SrvHit56_ArmageddonControl,
        MISSMODE_SrvHit57_BaalFxControl,
        MISSMODE_SrvHit58_BaalTauntLightningControl,
        MISSMODE_SrvHit59_BaalTauntPoisonControl,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };

    D2MissilesTxt* pMissilesTxtRecord = nullptr;
    if (pMissile)
    {
        pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);

    int32_t nHitResult = 2;
    int32_t nPierceFlags = 3;

    if (pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_EXPLOSION])
    {
        nHitResult = 0;
    }

    if (pUnit)
    {
        if ((pMissilesTxtRecord->nNextHit && STATES_CheckState(pUnit, STATE_JUSTHIT)) || MISSILE_CheckUnitIfOwner(pMissile, pUnit))
        {
            return 1;
        }

        MISSILE_SetOwner(pMissile, pUnit);

        int32_t nAnimMode = 0;
        if (pMissile)
        {
            nAnimMode = pMissile->dwAnimMode;
        }

        switch (nAnimMode)
        {
        case 0:
            return 1;
        case 1:
            if (pUnit->dwUnitType != UNIT_PLAYER)
            {
                return 1;
            }
            break;
        case 2:
            if (pUnit->dwUnitType != UNIT_MONSTER)
            {
                return 1;
            }
            break;
        default:
            break;
        }

        if (pOwner)
        {
            if (!sub_6FCBD900(pGame, pOwner, pUnit) && !pMissilesTxtRecord->nCollideFriend)
            {
                return 1;
            }

            if (pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_PIERCE] && (STATLIST_UnitGetStatValue(pOwner, STAT_SKILL_PIERCE, 0) || STATLIST_UnitGetItemStatOrSkillStatValue(pOwner, STAT_ITEM_PIERCE, 0)))
            {
                nPierceFlags = 3;

                const int32_t nPierceIndex = STATLIST_UnitGetStatValue(pMissile, STAT_PIERCE_IDX, 0);
                if (nPierceIndex)
                {
                    nPierceFlags = 2;
                    STATLIST_SetUnitStat(pMissile, STAT_PIERCE_IDX, nPierceIndex - 1, 0);
                }
            }
        }
    }

    if (pMissilesTxtRecord->nCollideKill)
    {
        nHitResult |= 1u;
    }

    if (pUnit)
    {
        if (pMissilesTxtRecord->nToHit)
        {
            int32_t nToHit = 0;
            if (pMissile)
            {
                nToHit = STATLIST_UnitGetStatValue(pMissile, STAT_TOHIT, 0);
            }

            if (!SUNITDMG_IsHitSuccessful(pOwner, pUnit, nToHit, 1))
            {
                if (pMissilesTxtRecord->nNextHit)
                {
                    const int32_t nDelay = pMissile->pGame->dwGameFrame + pMissilesTxtRecord->nNextDelay;
                    D2StatListStrc* pStatList = STATLIST_AllocStatList(pMissile->pGame->pMemoryPool, 2u, nDelay, pUnit->dwUnitType, pUnit->dwUnitId);
                    if (pStatList)
                    {
                        EVENT_SetEvent(pMissile->pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, nDelay, 0, 0);
                        STATLIST_SetState(pStatList, STATE_JUSTHIT);
                        STATLIST_SetStatRemoveCallback(pStatList, MISSMODE_ToggleStateOff);
                        D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
                        STATES_ToggleState(pUnit, STATE_JUSTHIT, 1);
                    }
                }

                SUNITEVENT_EventFunc_Handler(pGame, EVENT_HITBYMISSILE, pUnit, pMissile, nullptr);

                if (pMissilesTxtRecord->nAlwaysExplode)
                {
                    const uint16_t nHitFunc = pMissilesTxtRecord->wSrvHitFunc;
                    if (nHitFunc > 0 && nHitFunc < std::size(gpMissileSrvHitFnTable_6FD2E718))
                    {
                        if (gpMissileSrvHitFnTable_6FD2E718[nHitFunc](pGame, pMissile, pUnit) & 4)
                        {
                            return 1;
                        }
                    }
                }

                return 2;
            }
        }
    }
    else
    {
        if (!a4 && !pMissilesTxtRecord->nAlwaysExplode)
        {
            if (!(nHitResult & 1) && !a4 || !(nPierceFlags & 1))
            {
                return 1;
            }

            D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
            if (pRoom)
            {
                COLLISION_ResetMaskWithSize(pRoom, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), UNITS_GetUnitSizeX(pMissile), 0x40u);
            }

            return 2;
        }
    }

    if (nPierceFlags & 2)
    {
        if (pMissilesTxtRecord->nNextHit && pUnit)
        {
            const int32_t nDelay = pMissile->pGame->dwGameFrame + pMissilesTxtRecord->nNextDelay;
            D2StatListStrc* pStatList = STATLIST_AllocStatList(pMissile->pGame->pMemoryPool, 2u, nDelay, pUnit->dwUnitType, pUnit->dwUnitId);
            if (pStatList)
            {
                EVENT_SetEvent(pMissile->pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, nDelay, 0, 0);
                STATLIST_SetState(pStatList, STATE_JUSTHIT);
                STATLIST_SetStatRemoveCallback(pStatList, MISSMODE_ToggleStateOff);
                D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
                STATES_ToggleState(pUnit, STATE_JUSTHIT, 1);
            }
        }

        SUNITEVENT_EventFunc_Handler(pGame, EVENT_HITBYMISSILE, pUnit, pMissile, 0);

        const uint16_t nHitFunc = pMissilesTxtRecord->wSrvHitFunc;
        if (nHitFunc > 0 && nHitFunc < std::size(gpMissileSrvHitFnTable_6FD2E718))
        {
            nHitResult = gpMissileSrvHitFnTable_6FD2E718[nHitFunc](pGame, pMissile, pUnit);
            if (nHitResult & 4)
            {
                return 1;
            }
        }
    }

    if (pUnit && nHitResult & 2)
    {
        D2DamageStrc damage = {};
        MISSMODE_FillDamageParams(pMissile, pUnit, &damage);
        
        const uint16_t nDmgFunc = pMissilesTxtRecord->wSrvDmgFunc;
        if (nDmgFunc > 0 && nDmgFunc < std::size(gpMissileSrvDmgFnTable_6FD2E838))
        {
            gpMissileSrvDmgFnTable_6FD2E838[nDmgFunc](pGame, pMissile, pUnit, &damage);
        }

        MISSMODE_SetDamageFlags(pGame, pMissile, pUnit, &damage);

        if (pUnit->dwUnitType == UNIT_MONSTER && !MONSTERS_GetHirelingTypeId(pUnit))
        {
            const int32_t nDamageTargetAc = STATLIST_UnitGetStatValue(pMissile, STAT_ITEM_DAMAGETARGETAC, 0);
            if (nDamageTargetAc)
            {
                const int32_t nArmor = std::max(nDamageTargetAc + STATLIST_UnitGetStatValue(pUnit, STAT_ARMORCLASS, 0), 0);
                STATLIST_SetUnitStat(pUnit, STAT_ARMORCLASS, nArmor, 0);
            }
        }
    }

    if (!(nHitResult & 1) && !a4 || !(nPierceFlags & 1))
    {
        return 1;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);
    if (pRoom)
    {
        COLLISION_ResetMaskWithSize(pRoom, CLIENTS_GetUnitX(pMissile), CLIENTS_GetUnitY(pMissile), UNITS_GetUnitSizeX(pMissile), 0x40u);
    }

    return 2;
}

//D2Game.0x6FC60080
void __fastcall MISSMODE_ToggleStateOff(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    D2_MAYBE_UNUSED(pStatList);
    STATES_ToggleState(pUnit, nState, 0);
}

//D2Game.0x6FC60090
void __fastcall MISSMODE_SrvDoHandler(D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nUnused)
{
    using MissileSrvDoFunc = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*);
    constexpr MissileSrvDoFunc gMissileSrvDoFuncTable[] =
    {
        nullptr,
        MISSMODE_SrvDo01_BasicMissile,
        MISSMODE_SrvDo02_PlagueJavelin_PoisonJavelin_PoisonTrap,
        MISSMODE_SrvDo03_PoisonCloud_Blizzard_ThunderStorm_HandOfGod,
        nullptr,
        MISSMODE_SrvDo05_FireWall_ImmolationFire_MeteorFire_MoltenBoulderFirePath,
        MISSMODE_SrvDo06_MoltenBoulder_FireWallMaker,
        MISSMODE_SrvDo07_GuidedArrow_BoneSpirit,
        MISSMODE_SrvDo08_MonBlizzCenter,
        MISSMODE_SrvDo09_BatLightningBolt,
        MISSMODE_SrvDo10_BlizzardCenter,
        MISSMODE_SrvDo11_FingerMageSpider,
        MISSMODE_SrvDo12_DiabWallMaker,
        MISSMODE_SrvDo13_BoneWallMaker,
        MISSMODE_SrvDo14_GrimWard,
        MISSMODE_SrvDo15_FrozenOrb,
        MISSMODE_SrvDo16_FrozenOrbNova,
        MISSMODE_SrvDo17_CairnStones,
        MISSMODE_SrvDo18_TowerChestSpawner,
        MISSMODE_SrvDo19_RadamentDeath,
        MISSMODE_SrvDo20_BladeCreeper,
        MISSMODE_SrvDo21_Distraction,
        MISSMODE_SrvDo22_LightningTrailingJavelin,
        MISSMODE_SrvDo23_24_SuccFireBall_FirestormMaker,
        MISSMODE_SrvDo23_24_SuccFireBall_FirestormMaker,
        MISSMODE_SrvDo25_EruptionCenter,
        MISSMODE_SrvDo26_Vines_PlagueVines,
        MISSMODE_SrvDo27_Tornado,
        MISSMODE_SrvDo28_Volcano,
        MISSMODE_SrvDo29_RecyclerDelay,
        MISSMODE_SrvDo30_RabiesPlague,
        MISSMODE_SrvDo31_WakeOfDestructionMaker_BaalColdMaker,
        MISSMODE_SrvDo32_TigerFury,
        MISSMODE_SrvDo33_VineRecyclerDelay,
        MISSMODE_SrvDo34_BaalTauntControl,
        MISSMODE_SrvDo35_RoyalStrikeChaosIce,
        MISSMODE_SrvDo36_BaalFxControl,
        MISSMODE_SrvDo37_Unused,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };

    if (!pMissile)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pMissile->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->wSrvDoFunc <= 0 || pMissilesTxtRecord->wSrvDoFunc >= std::size(gMissileSrvDoFuncTable))
    {
        return;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pMissile);

    if (pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_SRCTOWN])
    {
        D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pMissile);
        if (pOwner && DUNGEON_IsRoomInTown(UNITS_GetRoom(pOwner)))
        {
            SUNIT_RemoveUnit(pGame, pMissile);
            return;
        }
    }

    if (!(pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_TOWN]) && DUNGEON_IsRoomInTown(pRoom))
    {
        SUNIT_RemoveUnit(pGame, pMissile);
        return;
    }

    if (gMissileSrvDoFuncTable[pMissilesTxtRecord->wSrvDoFunc](pGame, pMissile) == 2)
    {
        SUNIT_RemoveUnit(pGame, pMissile);
    }
}
