#include "MONSTER/MonsterMode.h"

#include <algorithm>

#include <D2BitManip.h>

#include <D2Chat.h>
#include <D2Collision.h>
#include <D2Dungeon.h>
#include <D2Monsters.h>
#include <D2States.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/MonsterTbls.h>
#include <D2StatList.h>
#include <D2QuestRecord.h>
#include <Units/UnitRoom.h>
#include <D2DataTbls.h>
#include <D2Skills.h>
#include <D2Combat.h>
#include <DataTbls/MissilesIds.h>


#include "AI/AiGeneral.h"
#include "AI/AiTactics.h"
#include "AI/AiThink.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Targets.h"
#include "ITEMS/Items.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterUnique.h"
#include "SKILLS/SkillMonst.h"
#include "SKILLS/Skills.h"
#include "SKILLS/SkillSor.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitEvent.h"
#include "UNIT/SUnitMsg.h"


#pragma pack(push, 1)
struct D2MonModeInfoTableStrc
{
    int32_t bUseRecurringEvent;
    int32_t bUseAnimationEvents;
    int32_t bCheckMovingMode;
};
#pragma pack(pop)

constexpr D2MonModeInfoTableStrc gtMonsterModeEventPolicies[16] =
{
    { 0, 1, 0 },
    { 0, 1, 0 },
    { 1, 0, 0 },
    { 0, 1, 0 },
    { 0, 0, 1 },
    { 0, 0, 1 },
    { 0, 1, 0 },
    { 0, 0, 1 },
    { 0, 0, 1 },
    { 0, 0, 1 },
    { 0, 0, 1 },
    { 0, 0, 1 },
    { 0, 1, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
};

using MonEventFunc = void(__fastcall*)(D2GameStrc*, D2UnitStrc*, int32_t, int32_t);
constexpr MonEventFunc gpMonsterEventFunctions[] =
{
    MONSTERMODE_EventModeChange,
    MONSTERMODE_EventEndAnimation,
    AITHINK_ExecuteMonsterAiTick,
    D2GAME_MONSTER_ApplyStatRegen_6FC63440,
    nullptr,
    SKILLS_EventCallback_ActiveState,
    MONSTERMODE_EventFreeHoverText,
    MONSTERUNIQUE_EventExecuteUModCallbacks,
    SKILLS_EventCallback_PeriodicSkillAura,
    SKILLS_EventCallback_ItemAura,
    MONSTERMODE_EventClearTargetOverride,
    nullptr,
    MONSTERMODE_EventUpdateStatListExpirations,
    nullptr,
    nullptr,
};
static_assert(EVENTTYPE_COUNT == std::size(gpMonsterEventFunctions), "missing callbacks");

constexpr D2MonModeCallbackTableStrc gMonModeCallbacks[16] =
{
    { MONSTERMODE_StartDeath, MONSTERMODE_UpdateDeath,                                   MONSTERMODE_EndDeath, 1 },
    { MONSTERMODE_StartNeutral, D2GAME_RemoveModeChangeEventCallback_6FC63B20,  nullptr, 0 },
    { MONSTERMODE_StartWalk, MONSTERMODE_UpdateWalk,                                   nullptr, 1 },
    { MONSTERMODE_StartGetHit, D2GAME_RemoveModeChangeEventCallback_6FC63B20,  MONSTERMODE_HandleEndAnimation, 1 },
    { MONSTERMODE_StartAttackOrSkill, MONSTERMODE_UpdateAttackOrSkill,                                   MONSTERMODE_HandleEndAnimation, 1 },
    { MONSTERMODE_StartAttackOrSkill, MONSTERMODE_UpdateAttackOrSkill,                                   MONSTERMODE_HandleEndAnimation, 1 },
    { MONSTERMODE_StartBlock, D2GAME_RemoveModeChangeEventCallback_6FC63B20,  MONSTERMODE_HandleEndAnimation, 1 },
    { MONSTERMODE_StartAttackOrSkill, MONSTERMODE_UpdateAttackOrSkill,                                   MONSTERMODE_HandleEndAnimation, 1 },
    { MONSTERMODE_StartAttackOrSkill, MONSTERMODE_UpdateAttackOrSkill,                                   MONSTERMODE_HandleEndAnimation, 1 },
    { MONSTERMODE_StartAttackOrSkill, MONSTERMODE_UpdateAttackOrSkill,                                   MONSTERMODE_HandleEndAnimation, 1 },
    { MONSTERMODE_StartSkill3, MONSTERMODE_UpdateSkill3,                                   nullptr/*D2GAME_OBJECTS_InitFunction05_Door_6FCA6660*/, 0},
    { MONSTERMODE_StartSkill4, D2GAME_RemoveModeChangeEventCallback_6FC63B20,  nullptr, 0 },
    { MONSTERMODE_StartDead, D2GAME_RemoveModeChangeEventCallback_6FC63B20,  nullptr, 0 },
    { MONSTERMODE_StartKnockback, MONSTERMODE_UpdateKnockback,                                   MONSTERMODE_EndKnockback, 1 },
    { MONSTERMODE_StartSequence, MONSTERMODE_UpdateSequence,                                   MONSTERMODE_HandleEndAnimation, 1 },
    { MONSTERMODE_StartRun, MONSTERMODE_UpdateRun,                                   nullptr, 1 },
};

constexpr D2MonModeCallbackTableStrc gDiablo_Skill4_Callbacks =
{
    MONSTERMODE_StartAttackOrSkill, MONSTERMODE_UpdateAttackOrSkill,                                     MONSTERMODE_HandleEndAnimation, 1
};

constexpr D2MonModeCallbackTableStrc gDiablo_Skill3_Callbacks =
{
    MONSTERMODE_StartAttackOrSkill, MONSTERMODE_UpdateAttackOrSkill,                                     MONSTERMODE_HandleEndAnimation, 1
};

constexpr D2MonModeCallbackTableStrc gTrappedSoul_Attack_Callbacks =
{
    MONSTERMODE_StartAttackOrSkill, MONSTERMODE_UpdateAttackOrSkill,                                     MONSTERMODE_EndTrappedSoulAttack, 1
};

constexpr D2MonModeCallbackTableStrc gTrappedSoul_Skill_Callbacks =
{
    MONSTERMODE_StartAttackOrSkill, MONSTERMODE_UpdateAttackOrSkill,                                     MONSTERMODE_EndTrappedSoulSkill, 1
};

constexpr D2MonModeCallbackTableStrc gShadowMasterWarrior_Skill4_Callbacks =
{
    MONSTERMODE_StartAttackOrSkill, MONSTERMODE_UpdateAttackOrSkill,                                     MONSTERMODE_HandleEndAnimation, 1
};


//D2Game.0x6FC62770
//1.14d: 0x005A4EE0
int32_t __fastcall D2GAME_IsMonster_6FC62770(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        return 1;
    }

    return 0;
}

//D2Game.0x6FC62780
//1.14d: 0x005A4EF0
void __fastcall MONSTERMODE_DistributeExperienceIfEligible(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2GameStrc* pGame)
{
    if (pAttacker && pDefender && !(pDefender->dwFlags & 0x04000000))
    {
        SUNITDMG_DistributeExperience(pGame, pAttacker, pDefender);
    }
}

//D2Game.0x6FC627B0
//1.14d: 0x005A4F50
void __fastcall MONSTERMODE_UpdateModeAttackStats(D2UnitStrc* pUnit, int32_t nMode)
{
    constexpr int32_t nPlayerCountDamageBonus[] =
    {
        0, 0, 8, 16, 24, 32, 40, 48, 56
    };

    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
    {
        return;
    }
    
    D2MonStatsTxt* pMonStatsTxtRecord = pUnit->pMonsterData->pMonstatsTxt;
    if (!pMonStatsTxtRecord)
    {
        return;
    }
    
    D2MonSkillInfoStrc monSkillInfo = {};
    SKILLS_FillMinionSkillDamage(pUnit, nMode, &monSkillInfo);

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndFlag(pUnit, 1);
    if (!pStatList)
    {
        return;
    }

    STATLIST_RemoveAllStats(pStatList);

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    int32_t nGameType = 0;
    if (pGame->nGameType || pGame->dwGameType)
    {
        nGameType = 1;
    }

    int32_t nMultiplier = 0;
    if (STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_EVIL)
    {
        const int32_t nPlayerCount = std::max(STATLIST_UnitGetStatValue(pUnit, STAT_MONSTER_PLAYERCOUNT, 0), 1);

        if (pGame->nDifficulty != DIFFMODE_NORMAL && nPlayerCount >= 2)
        {
            if (nPlayerCount >= std::size(nPlayerCountDamageBonus))
            {
                nMultiplier = 8 * nPlayerCount - 16;
            }
            else
            {
                nMultiplier = nPlayerCountDamageBonus[nPlayerCount];
            }
        }
    }

    D2MonStatsInitStrc pMonStatsInit = {};
    int32_t nMinDamage = 0;
    int32_t nMaxDamage = 0;
    int32_t nToHit = 0;
    if (nMode == MONMODE_ATTACK2)
    {
        DATATBLS_CalculateMonsterStatsByLevel(pUnit->dwClassId, nGameType, pGame->nDifficulty, STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0), 16, &pMonStatsInit);
        nMinDamage = pMonStatsInit.nA2MinD + monSkillInfo.nMinDamage;
        nMaxDamage = pMonStatsInit.nA2MaxD + monSkillInfo.nMaxDamage;
        nToHit = pMonStatsInit.nTH + monSkillInfo.nToHit;
    }
    else if (nMode == MONMODE_BLOCK || nMode == MONMODE_CAST || nMode == MONMODE_SKILL1)
    {
        DATATBLS_CalculateMonsterStatsByLevel(pUnit->dwClassId, nGameType, pGame->nDifficulty, STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0), 32, &pMonStatsInit);
        nMinDamage = pMonStatsInit.nS1MinD + monSkillInfo.nMinDamage;
        nMaxDamage = pMonStatsInit.nS1MaxD + monSkillInfo.nMaxDamage;
        nToHit = pMonStatsInit.nTH + monSkillInfo.nToHit;
    }
    else
    {
        DATATBLS_CalculateMonsterStatsByLevel(pUnit->dwClassId, nGameType, pGame->nDifficulty, STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0), 8, &pMonStatsInit);
        nMinDamage = pMonStatsInit.nA1MinD + monSkillInfo.nMinDamage;
        nMaxDamage = pMonStatsInit.nA1MaxD + monSkillInfo.nMaxDamage;
        nToHit = pMonStatsInit.nTH + monSkillInfo.nToHit;
    }

    if (pGame->bExpansion)
    {
        if (nMultiplier)
        {
            nMinDamage += nMultiplier * nMinDamage / 128;
            nMaxDamage += nMultiplier * nMaxDamage / 128;
            nToHit += nMultiplier * nToHit / 128;
        }
    }
    else
    {
        if (pGame->nDifficulty != DIFFMODE_NORMAL && pMonStatsTxtRecord->nAlign != MONALIGN_GOOD)
        {
            nMinDamage = 10 * nMinDamage / 12;
            nMaxDamage = 10 * nMaxDamage / 12;
            nToHit = 10 * nToHit / 15;
        }
    }

    STATLIST_SetStatIfListIsValid(pStatList, STAT_MINDAMAGE, nMinDamage, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_MAXDAMAGE, nMaxDamage, 0);
    STATLIST_SetStatIfListIsValid(pStatList, STAT_TOHIT, nToHit, 0);

    for (int32_t i = 0; i < 3; ++i)
    {
        if (pMonStatsTxtRecord->nElMode[i] && nMode == pMonStatsTxtRecord->nElMode[i])
        {
            const uint8_t nChance = pMonStatsTxtRecord->nElPct[i][pGame->nDifficulty];
            if (nChance && (nChance >= 100 || (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < nChance))
            {
                memset(&pMonStatsInit, 0, sizeof(pMonStatsInit));
                DATATBLS_CalculateMonsterStatsByLevel(pUnit->dwClassId, nGameType, pGame->nDifficulty, STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0), 0x40 + i, &pMonStatsInit);

                int32_t nElemDuration = pMonStatsInit.nElDur;
                int32_t nElemMinDamage = pMonStatsInit.nElMaxD;
                int32_t nElemMaxDamage = pMonStatsInit.nElMinD;
                if (pGame->bExpansion && nMultiplier)
                {
                    nElemMaxDamage = pMonStatsInit.nElMinD + nMultiplier * pMonStatsInit.nElMinD / 128;
                    nElemMinDamage = pMonStatsInit.nElMaxD + nMultiplier * pMonStatsInit.nElMaxD / 128;
                    nElemDuration = pMonStatsInit.nElDur + nMultiplier * pMonStatsInit.nElDur / 128;
                }

                uint8_t nElemType = pMonStatsTxtRecord->nElType[i];
                if (nElemType == ELEMTYPE_RAND)
                {
                    nElemType = ITEMS_RollRandomNumber(&pUnit->pSeed) % 5 + 1;

                    if (!nElemDuration)
                    {
                        nElemDuration = 25;
                    }
                }

                switch (nElemType)
                {
                case ELEMTYPE_FIRE:
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_FIREMINDAM, nElemMaxDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_FIREMAXDAM, nElemMinDamage, 0);
                    break;

                case ELEMTYPE_LTNG:
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_LIGHTMINDAM, nElemMaxDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_LIGHTMAXDAM, nElemMinDamage, 0);
                    break;

                case ELEMTYPE_MAGIC:
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_MAGICMINDAM, nElemMaxDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_MAGICMAXDAM, nElemMinDamage, 0);
                    break;

                case ELEMTYPE_COLD:
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_COLDMINDAM, nElemMaxDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_COLDMAXDAM, nElemMinDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_COLDLENGTH, nElemDuration, 0);
                    break;

                case ELEMTYPE_POIS:
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONMINDAM, 10 * nElemMaxDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONMAXDAM, 10 * nElemMinDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONLENGTH, 2 * nElemDuration, 0);
                    break;

                case ELEMTYPE_LIFE:
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_LIFEDRAINMINDAM, nElemMaxDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_LIFEDRAINMAXDAM, nElemMinDamage, 0);
                    break;

                case ELEMTYPE_MANA:
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_MANADRAINMINDAM, nElemMaxDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_MANADRAINMAXDAM, nElemMinDamage, 0);
                    break;

                case ELEMTYPE_STAMINA:
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_STAMDRAINMINDAM, nElemMaxDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_STAMDRAINMAXDAM, nElemMinDamage, 0);
                    break;

                case ELEMTYPE_STUN:
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_STUNLENGTH, nElemDuration, 0);
                    break;

                case ELEMTYPE_BURN:
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_BURNINGMIN, nElemMaxDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_BURNINGMAX, nElemMinDamage, 0);
                    STATLIST_SetStatIfListIsValid(pStatList, STAT_FIRELENGTH, nElemDuration, 0);
                    break;

                default:
                    break;
                }
            }
        }
    }
}

//D2Game.0x6FC62D90
//1.14d: 0x005A5490
void __fastcall MONSTERMODE_AllocateTargetCombat(D2UnitStrc* pUnit, D2GameStrc* pGame)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        D2DamageStrc damage = {};
        damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, 0, 0);
        SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, 128);
    }
}

//D2Game.0x6FC62DF0
//1.14d: 0x005A54F0
void __stdcall MONSTERMODE_AddSandLeaperKnockback(D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (pDamage->wResultFlags & 4 && D2GAME_GetMonsterBaseId_6FC64B10(pUnit) == MONSTER_SANDLEAPER1 && !STATES_CheckState(pUnit, STATE_FREEZE))
    {
        pDamage->wResultFlags |= 8u;
    }
}

//D2Game.0x6FC62E70
//1.14d: 0x005A5560
void __fastcall D2GAME_MONSTER_ApplyCriticalDamage_6FC62E70(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage)
{
    if (pAttacker && pAttacker->dwUnitType == UNIT_MONSTER)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pAttacker->dwClassId);
        if (pMonStatsTxtRecord && pMonStatsTxtRecord->nCrit && (ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100) < pMonStatsTxtRecord->nCrit)
        {
            pDamage->dwPhysDamage *= 2;
            pDamage->dwFireDamage *= 2;
            pDamage->dwLtngDamage *= 2;
            pDamage->dwMagDamage *= 2;
            pDamage->dwColdDamage *= 2;
            pDamage->dwPoisDamage *= 2;

            if (pDefender && !SUNITDMG_SetHitClass(pDamage, 0x10u))
            {
                UNITS_SetOverlay(pDefender, 54, 0);
            }
        }
    }
}

//D2Game.0x6FC62F50
//1.14d: 0x005A5650
uint8_t __fastcall MONSTERMODE_GetHpPercent128(D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) >> 8;
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit) >> 8;
        if (nMaxHp > 0 && nHitpoints < nMaxHp)
        {
            return (nHitpoints << 7) / nMaxHp;
        }
    }

    return 0x80;
}

//D2Game.0x6FC62F90
//1.14d: 0x005A61F0
void __fastcall D2GAME_MONSTERMODE_ResetVelocityParams_6FC62F90(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        D2AiParamStrc* pAiParam = pUnit->pMonsterData->pAiParam;
        pAiParam->unk0x18 = 0;
        pAiParam->nVelocity = 0;
        pAiParam->unk0x20 = 0;
    }
}

//D2Game.0x6FC62FC0
//1.14d: 0x005A6260
void __fastcall D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(D2AiParamStrc* pAiParam, int32_t nPathType, int32_t nVel, uint8_t nNewDistance)
{
    if (nPathType)
    {
        pAiParam->unk0x18 = nPathType;
    }

    if (nVel)
    {
        pAiParam->nVelocity = nVel;
    }

    if (nNewDistance)
    {
        pAiParam->unk0x20 = std::min(nNewDistance, 77ui8);
    }
}

//D2Game.0x6FC62FF0
//1.14d: 0x005A6490
D2AiParamStrc* __fastcall D2GAME_MONSTERMODE_AllocParams_6FC62FF0(D2GameStrc* pGame)
{
    return D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2AiParamStrc);
}

//D2Game.0x6FC63020
//1.14d: 0x005A64D0
void __fastcall D2GAME_MONSTERMODE_FreeParams_6FC63020(D2GameStrc* pGame, D2AiParamStrc* pAiParam)
{
    D2_FREE_POOL(pGame->pMemoryPool, pAiParam);
}

//D2Game.0x6FC63040
//1.14d: 0x005A6830
void __fastcall MONSTERMODE_TryDropTreasure(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange, int32_t nUnused)
{
    D2UnitStrc* pUnit = pModeChange->pUnit;
    D2_ASSERT(pUnit);

    if (pUnit->dwFlags & 0x20000)
    {
        return;
    }

    if (pUnit->dwClassId == MONSTER_BONEWALL)
    {
        FOG_DisplayAssert("A bone wall died and didn't have the UNIT_MASK_SPAWNNOITEM bit set!", __FILE__, __LINE__);
        exit(-1);
    }

    if (!COLLISION_CheckMask(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 0x801u))
    {
        D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);

        D2_ASSERT(pRoom);

        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);

        D2_ASSERT(pMonStatsTxtRecord);

        MONSTERMODE_DropTreasure(pGame, pUnit, 0, pModeChange);
    }
}

//D2Game.0x6FC631B0
//1.14d: 0x005A6600
void __fastcall MONSTERMODE_DropTreasure(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t bIgnoreNoDrop, D2ModeChangeStrc* pModeChange)
{    
    D2_ASSERT(pUnit);

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    D2_ASSERT(pRoom);

    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    //DRLG_GetActNoFromLevelId(nLevelId);

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    D2_ASSERT(pMonStatsTxtRecord);

    short nTCId = 0;

    const int32_t nItemLevel = ITEMS_GetItemLevelForNewItem(pUnit, nLevelId);
    const int32_t nSuperUniqueId = MONSTERUNIQUE_GetSuperUniqueBossHcIdx(pGame, pUnit);
    if (nSuperUniqueId != -1)
    {
        D2SuperUniquesTxt* pSuperUniquesTxtRecord = DATATBLS_GetSuperUniquesTxtRecord(nSuperUniqueId);
        if (pSuperUniquesTxtRecord)
        {
            nTCId = pSuperUniquesTxtRecord->dwTC[pGame->nDifficulty];
        }
        else
        {
            nTCId = pMonStatsTxtRecord->wTreasureClass[pGame->nDifficulty][2];
        }
    }
    else
    {
        if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_CHAMPION))
        {
            nTCId = pMonStatsTxtRecord->wTreasureClass[pGame->nDifficulty][1];
        }
        else if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_UNIQUE))
        {
            nTCId = pMonStatsTxtRecord->wTreasureClass[pGame->nDifficulty][2];
        }
        else
        {
            nTCId = pMonStatsTxtRecord->wTreasureClass[pGame->nDifficulty][0];
        }
    }

    if (pMonStatsTxtRecord->nTCQuestId && pMonStatsTxtRecord->wTreasureClass[pGame->nDifficulty][3] && pModeChange)
    {
        D2UnitStrc* pTarget = pModeChange->pTargetUnit;
        if (pTarget)
        {
            if (pTarget->dwUnitType != UNIT_PLAYER)
            {
                D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pModeChange->pTargetUnit);
                if (STATES_CheckStateMaskExpOnUnit(pTarget))
                {
                    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndFlag(pTarget, 0x800u);
                    if (pStatList)
                    {
                        pOwner = SUNIT_GetServerUnit(pGame, STATLIST_GetOwnerType(pStatList), STATLIST_GetOwnerGUID(pStatList));
                    }
                }

                if (pOwner)
                {
                    pTarget = pOwner;
                }
            }

            if (pTarget && pTarget->dwUnitType == UNIT_PLAYER)
            {
                D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pTarget)->pQuestData[pGame->nDifficulty];
                if (pQuestFlags && QUESTRECORD_GetQuestState(pQuestFlags, pMonStatsTxtRecord->nTCQuestId, QFLAG_COMPLETEDBEFORE) != 1
                    && QUESTRECORD_GetQuestState(pQuestFlags, pMonStatsTxtRecord->nTCQuestId, QFLAG_REWARDPENDING) != 1
                    && QUESTRECORD_GetQuestState(pQuestFlags, pMonStatsTxtRecord->nTCQuestId, pMonStatsTxtRecord->nTCQuestCP) != 1)
                {
                    nTCId = pMonStatsTxtRecord->wTreasureClass[pGame->nDifficulty][3];
                }
            }
        }
    }

    ITEMS_DropTreasureClassById(pGame, pUnit, pModeChange->pTargetUnit, nTCId, 0, nItemLevel, bIgnoreNoDrop, nullptr, nullptr, 0);
}

//D2Game.0x6FC63440
//1.14d: 0x005A6920
void __fastcall D2GAME_MONSTER_ApplyStatRegen_6FC63440(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUnused1, int32_t nUnused2)
{
    pUnit = pUnit;

    int32_t nHpRegen = STATLIST_UnitGetStatValue(pUnit, STAT_HPREGEN, 0);

    if (STATES_CheckStateMaskLifeOnUnit(pUnit))
    {
        nHpRegen -= STATLIST_GetUnitBaseStat(pUnit, STAT_HPREGEN, 0);
    }

    if (!STATES_CheckState(pUnit, STATE_PREVENTHEAL) || nHpRegen < 0)
    {
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_STATREGEN, pGame->dwGameFrame + 1, 0, 0);

        if (nHpRegen)
        {
            const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
            if (nHpRegen < 0 && nHitpoints < 256)
            {
                D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
                if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
                {
                    return;
                }
            }

            int32_t nNewHp = nHpRegen + nHitpoints;

            const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
            if (nNewHp > nMaxHp)
            {
                D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_STATREGEN, 0);
                nNewHp = nMaxHp;
            }

            if (nNewHp < 1)
            {
                nNewHp = 0;
            }

            STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);

            const uint8_t nLastSentHpPct = STATLIST_UnitGetStatValue(pUnit, STAT_LAST_SENT_HP_PCT, 0);
            const uint8_t nNewSentHpPct = MONSTERMODE_GetHpPercent128(pUnit);
            int32_t nHpPctDiff = nLastSentHpPct - nNewSentHpPct;
            nHpPctDiff = std::abs(nHpPctDiff);

            if (nHpPctDiff > 4)
            {
                SUNITMSG_QueueLifeUpdate(pUnit, nNewSentHpPct);
                STATLIST_SetUnitStat(pUnit, STAT_LAST_SENT_HP_PCT, nNewSentHpPct, 0);
            }

            if (!nNewHp && pUnit && pUnit->dwAnimMode != MONMODE_DEATH && pUnit->dwAnimMode != MONMODE_DEAD)
            {
                D2StatListStrc* pStatList = nullptr;

                constexpr int32_t states[] = { STATE_POISON, STATE_OPENWOUNDS };
                for (size_t i = 0; i < std::size(states); ++i)
                {
                    if (pStatList)
                    {
                        break;
                    }

                    pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, states[i]);
                }

                pUnit->dwLastHitClass = 0;

                D2UnitStrc* pStatListOwner = nullptr;
                if (pStatList)
                {
                    pStatListOwner = SUNIT_GetServerUnit(pGame, STATLIST_GetOwnerType(pStatList), STATLIST_GetOwnerGUID(pStatList));
                }

                if (STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
                {
                    STATES_ToggleState(pUnit, STATE_DEATH_DELAY, 1);
                }
                else
                {
                    SUNITDMG_KillMonster(pGame, pUnit, pStatListOwner, 1);
                    SUNITEVENT_Trigger(pGame, UNITEVENT_KILLED, pUnit, pStatListOwner, 0);

                    if (pStatListOwner)
                    {
                        SUNITEVENT_Trigger(pGame, UNITEVENT_KILL, pStatListOwner, pUnit, 0);
                    }
                }
            }
        }
        else
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, EVENTTYPE_STATREGEN, 0);
        }
    }
}

//D2Game.0x6FC63650
//1.14d: 0x00451F80
D2MonStatsTxt* __fastcall MONSTERMODE_GetMonStatsTxtRecord(int32_t nMonsterId)
{
    if (nMonsterId >= 0 && nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        return &sgptDataTables->pMonStatsTxt[nMonsterId];
    }

    return nullptr;
}

//D2Game.0x6FC63680
//1.14d: 0x005A8030
void __fastcall MONSTERMODE_HandleEndAnimation(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }
    
    int32_t nBaseId = pUnit->dwClassId;
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    if (pMonStatsTxtRecord)
    {
        nBaseId = pMonStatsTxtRecord->nBaseId;
    }

    pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nBaseId);
    if (!pMonStatsTxtRecord)
    {
        return;
    }

    if (pMonStatsTxtRecord->nSplEndGeneric)
    {
        bool returnEarly = false;
        switch (nBaseId)
        {
        case MONSTER_VULTURE1:
        {
            if (pUnit->dwAnimMode == MONMODE_SKILL1)
            {
                returnEarly = true;
            }
            break;
        }
        case MONSTER_WILLOWISP1:
        {
            if (pUnit->dwAnimMode == MONMODE_WALK)
            {
                returnEarly = true;
            }
            break;
        }
        case MONSTER_BATDEMON1:
        {
            if (pUnit->dwAnimMode == MONMODE_SKILL3 || pUnit->dwAnimMode == MONMODE_SKILL4)
            {
                returnEarly = true;
            }
            break;
        }
        case MONSTER_FROGDEMON1:
        {
            if (pUnit->dwAnimMode == MONMODE_SEQUENCE)
            {
                returnEarly = true;
            }
            break;
        }
        case MONSTER_FIREBEAST:
        case MONSTER_ICEGLOBE:
        case MONSTER_TRAPPEDSOUL1:
        {
            returnEarly = true;
            break;
        }
        default:
            break;
        }

        if (returnEarly)
        {
            if ((!STATES_CheckState(pUnit, STATE_FREEZE) || SUNIT_IsDead(pUnit)) && gpMonsterEventFunctions[EVENTTYPE_AITHINK])
            {
                gpMonsterEventFunctions[EVENTTYPE_AITHINK](pGame, pUnit, 0, 0);
            }
            return;
        }
    }

    if (pUnit->dwAnimMode == MONMODE_WALK || pUnit->dwAnimMode == MONMODE_RUN)
    {
        UNITS_ChangeAnimMode(pUnit, MONMODE_NEUTRAL);
        MONSTERMODE_EventHandler(pGame, pUnit, EVENTTYPE_AITHINK, 0, 0);
    }
    else
    {
        if (STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
        {
            SUNIT_SetUninterruptable(pUnit, 0);

            if (SUNIT_IsDead(pUnit))
            {
                return;
            }
        }

        D2ModeChangeStrc modeChange = {};
        MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_NEUTRAL, &modeChange);
        modeChange.pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
        D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
    }
}

//D2Game.0x6FC63940
//1.14d: 0x005A8330
void __fastcall MONSTERMODE_EndTrappedSoulAttack(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    UNITS_SetUsedSkill(pUnit, nullptr);

    D2ModeChangeStrc modeChange = {};

    modeChange.unk0x14[1] = 100;
    modeChange.nMode = MONMODE_SKILL1;
    modeChange.pUnit = pUnit;

    if (pUnit)
    {
        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
        if (pMonStats2TxtRecord && pMonStats2TxtRecord->nModeEnabledWhenMovingFlags[1] & gdwBitMasks[0])
        {
            modeChange.unk0x14[1] = 101;
        }
    }

    modeChange.pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
    
    D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);

    if (!STATES_CheckState(pUnit, STATE_FREEZE) || SUNIT_IsDead(pUnit))
    {
        if (gpMonsterEventFunctions[EVENTTYPE_AITHINK])
        {
            gpMonsterEventFunctions[EVENTTYPE_AITHINK](pGame, pUnit, 0, 0);
        }
    }
}

//D2Game.0x6FC63A30
//1.14d: 0x005A83E0
void __fastcall MONSTERMODE_EndTrappedSoulSkill(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    UNITS_SetUsedSkill(pUnit, nullptr);

    D2ModeChangeStrc modeChange = {};
    
    modeChange.unk0x14[1] = 100;
    modeChange.nMode = MONMODE_SKILL1;
    modeChange.pUnit = pUnit;

    if (pUnit)
    {
        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
        if (pMonStats2TxtRecord && pMonStats2TxtRecord->nModeEnabledWhenMovingFlags[1] & gdwBitMasks[0])
        {
            modeChange.unk0x14[1] = 101;
        }
    }

    modeChange.pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);

    D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);

    if (!STATES_CheckState(pUnit, STATE_FREEZE) || SUNIT_IsDead(pUnit))
    {
        if (gpMonsterEventFunctions[EVENTTYPE_AITHINK])
        {
            gpMonsterEventFunctions[EVENTTYPE_AITHINK](pGame, pUnit, 0, 0);
        }
    }
}

//D2Game.0x6FC63B20
//1.14d: 0x005A6DE0
int32_t __fastcall D2GAME_RemoveModeChangeEventCallback_6FC63B20(D2GameStrc* pGame, D2UnitStrc* pMonster)
{
    D2GAME_EVENTS_Delete_6FC34840(pGame, pMonster, EVENTTYPE_MODECHANGE, 0);
    return 1;
}

//D2Game.0x6FC63B30
//1.14d: 0x005A6FF0
int32_t __fastcall MONSTERMODE_StartDeath(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    SUNIT_SetCombatMode(pGame, pModeChange->pUnit, MONMODE_DEATH);
    MONSTERMODE_CleanupDeath(pGame, pModeChange->pUnit, pModeChange->unk0x14[0]);
    MONSTERMODE_TryDropTreasure(pGame, pModeChange, 0);
    MONSTERREGION_RecordMonsterKill(pGame->pMonReg, pModeChange->pUnit);
    DUNGEON_SaveKilledUnitGUID(UNITS_GetRoom(pModeChange->pUnit), pModeChange->pUnit ? pModeChange->pUnit->dwUnitId : -1);

    if (!pModeChange->pUnit)
    {
        return 1;
    }
    
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pModeChange->pUnit->dwClassId);
    if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_DEATHDMG]))
    {
        return 1;
    }

    if (pModeChange->pUnit->dwUnitType == UNIT_MONSTER)
    {
        if (pMonStatsTxtRecord->nBaseId == MONSTER_BONEFETISH1)
        {
            const int32_t nX = CLIENTS_GetUnitX(pModeChange->pUnit);
            const int32_t nY = CLIENTS_GetUnitY(pModeChange->pUnit);
            D2UnitStrc* pMissile = SKILLS_CreateMissileAtOffset(pGame, MISSILE_MONSTERCORPSEEXPLODE, pModeChange->pUnit, 0, 1, 0, 0, nX, nY, 1);
            if (pMissile)
            {
                D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
                if (pDifficultyLevelsTxtRecord)
                {
                    D2MonStatsInitStrc monStatsInit = {};
                    DATATBLS_CalculateMonsterStatsByLevel(pModeChange->pUnit->dwClassId, pGame->dwGameType, pGame->nDifficulty, STATLIST_UnitGetStatValue(pModeChange->pUnit, STAT_LEVEL, 0), 1, &monStatsInit);

                    const int32_t nMaxDamage = MONSTERUNIQUE_CalculatePercentage(monStatsInit.nMaxHP, pDifficultyLevelsTxtRecord->dwMonsterCEDmgPercent, 100);
                    const int32_t nMinDamage = MONSTERUNIQUE_CalculatePercentage(nMaxDamage, 60, 100);
                    
                    D2DamageStrc damage = {};
                    damage.dwPhysDamage = (nMinDamage + ITEMS_RollLimitedRandomNumber(&pModeChange->pUnit->pSeed, nMaxDamage - nMinDamage)) << 7;
                    SUNITDMG_SetMissileDamageFlagsForNearbyUnits(pGame, pMissile, nX, nY, 5, &damage, 0, 0, nullptr, 0x581);
                    return 1;

                }
            }
            return 1;
        }
        else if (pMonStatsTxtRecord->nBaseId == MONSTER_SIEGEBEAST1)
        {
            MONSTERMODE_HandleSiegeBeastDeath(pGame, pModeChange->pUnit);
            return 1;
        }
    }

    MONSTERMODE_ApplyDeathAreaDamage(pGame, pModeChange->pUnit);
    return 1;
}

//D2Game.0x6FC63E80
//1.14d: 0x005A6520
void __fastcall MONSTERMODE_CleanupDeath(D2GameStrc* pGame, D2UnitStrc* pUnit, DWORD dwDir)
{
    if (!pUnit)
    {
        return;
    }

    if (pUnit->pHoverText)
    {
        CHAT_FreeHoverMsg(pGame->pMemoryPool, pUnit->pHoverText);
        pUnit->pHoverText = nullptr;
        UNITROOM_RefreshUnit(pUnit);
        pUnit->dwFlags |= UNITFLAG_HASTXTMSG;
    }

    AIGENERAL_UpdateMinionList(pUnit);

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    TARGETS_RemoveUnitNode(pGame, pUnit);

    pUnit->dwFlags &= 0xFFFF7FF3u;

    D2Common_10469(pUnit);
    STATES_UpdateStayDeathFlags(pUnit, MONSTERS_IsBoss(pMonStatsTxtRecord, pUnit));

    if (STATES_CheckStateMaskHideOnUnit(pUnit))
    {
        pUnit->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
    if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_DEADCOL]))
    {
        PATH_SetUnitDeadCollision(pUnit, 1);
    }

    PATH_SetDirection(pUnit->pDynamicPath, dwDir);
    MONSTER_DeleteEvents(pGame, pUnit);
}

//D2Game.0x6FC63FD0
//1.14d: 0x005A6DF0
void __fastcall MONSTERMODE_ApplyDeathAreaDamage(D2GameStrc* pGame, D2UnitStrc* pAttacker)
{
    for (D2UnitStrc* i = UNITS_GetRoom(pAttacker)->pUnitFirst; i; i = i->pRoomNext)
    {
        if (i->dwUnitType == UNIT_PLAYER && i->dwAnimMode != PLRMODE_DEAD && UNITS_GetDistanceToOtherUnit(pAttacker, i) <= 2 && !UNITS_TestCollisionBetweenInteractingUnits(i, pAttacker, 15361))
        {
            D2DamageStrc pDamage = {};
            pDamage.dwPhysDamage = (int32_t )STATLIST_UnitGetStatValue(i, STAT_HITPOINTS, 0) >> 5;
            pDamage.wResultFlags = DAMAGERESULTFLAG_SUCCESSFULHIT;
            if (!STATES_CheckState(i, STATE_UNINTERRUPTABLE))
            {
                pDamage.wResultFlags |= DAMAGERESULTFLAG_GETHIT;
            }

            SUNITDMG_CalculateTotalDamage(pGame, pAttacker, i, &pDamage);
            SUNITDMG_ExecuteEvents(pGame, pAttacker, i, 1, &pDamage);
            SUNITDMG_ExecuteMissileDamage(pGame, pAttacker, i, &pDamage);
        }
    }
}

//D2Game.0x6FC64090
//1.14d: 0x005A6EB0
void __fastcall MONSTERMODE_HandleSiegeBeastDeath(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2UnitStrc* pMonster = SUNIT_GetOwner(pGame, pUnit);
    if (pMonster)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pMonster->dwClassId);
        if (pMonStatsTxtRecord)
        {
            D2CoordStrc pCoord = {};
            pCoord.nX = CLIENTS_GetUnitX(pMonster);
            pCoord.nY = CLIENTS_GetUnitY(pMonster);
            D2ActiveRoomStrc* pRoom = COLLISION_GetFreeCoordinates(UNITS_GetRoom(pMonster), &pCoord, UNITS_GetUnitSizeX(pMonster), COLLIDE_MASK_MONSTER_PATH, 0);
            if (STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_EVIL && pRoom && AITACTICS_UseSkill(pGame, pMonster, pMonStatsTxtRecord->nSkillMode[0], pMonStatsTxtRecord->nSkill[0], 0, pCoord.nX, pCoord.nY))
            {
                D2GAME_EVENTS_Delete_6FC34840(pGame, pMonster, EVENTTYPE_AITHINK, 0);
            }
            else
            {
                SUNITDMG_KillMonster(pGame, pMonster, 0, 1);
            }
        }
    }
}

//D2Game.0x6FC641D0
//1.14d: 0x005A72B0
void __fastcall MONSTERMODE_EndDeath(D2GameStrc* pGame, D2UnitStrc* pAttacker)
{
    if (pAttacker)
    {
        SUNIT_SetCombatMode(pGame, pAttacker, 12);
        SUNITEVENT_Trigger(pGame, UNITEVENT_DEATH, pAttacker, 0, 0);

        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pAttacker->dwClassId);
        if (pMonStatsTxtRecord)
        {
            if (pMonStatsTxtRecord->nSplEndDeath == 1)
            {
                if (pMonStatsTxtRecord->wMinion[0] >= 0 && pMonStatsTxtRecord->wMinion[0] <= sgptDataTables->nMonStatsTxtRecordCount)
                {
                    MONSTER_Reinitialize(pGame, pAttacker, pMonStatsTxtRecord->wMinion[0], 1);
                    MONSTER_UpdateAiCallbackEvent(pGame, pAttacker);
                }
            }
            else if (pMonStatsTxtRecord->nSplEndDeath == 2)
            {
                D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pAttacker);
                if (pOwner)
                {
                    SUNITDMG_KillMonster(pGame, pOwner, 0, 1);
                }
            }
        }
    }
}

//D2Game.0x6FC64280
//1.14d: 0x005A7350
int32_t __fastcall MONSTERMODE_UpdateDeath(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!MONSTERS_IsSandLeaper(pUnit, 0))
    {
        SUNIT_SetCombatMode(pGame, pUnit, 12);
        return 1;
    }

    SUNIT_UpdateUnitPath(pGame, pUnit);
    UNITS_StopSequence(pUnit);

    if (UNITS_IsAtEndOfFrameCycle(pUnit))
    {
        SUNIT_SetCombatMode(pGame, pUnit, 12);
    }

    return 1;
}

//D2Game.0x6FC642C0
//1.14d: 0x005A7390
int32_t __fastcall MONSTERMODE_StartDead(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    if (pModeChange->pUnit)
    {
        if (pModeChange->pUnit->dwAnimMode)
        {
            MONSTERMODE_CleanupDeath(pGame, pModeChange->pUnit, pModeChange->unk0x14[0]);
        }
    }

    SUNIT_SetCombatMode(pGame, pModeChange->pUnit, 12);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pModeChange->pUnit, EVENTTYPE_PERIODICSKILLS, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pModeChange->pUnit, EVENTTYPE_PERIODICSTATS, 0);

    return 1;
}

//D2Game.0x6FC64310
//1.14d: 0x005A73E0
int32_t __fastcall MONSTERMODE_StartNeutral(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    SUNIT_SetCombatMode(pGame, pModeChange->pUnit, 1);

    if (EVENT_GetEventFrame(pGame, pModeChange->pUnit, EVENTTYPE_AITHINK) <= (int32_t)pGame->dwGameFrame)
    {
        if (STATES_CheckState(pModeChange->pUnit, STATE_STUNNED))
        {
            EVENT_SetEvent(pGame, pModeChange->pUnit, EVENTTYPE_AITHINK, pGame->dwGameFrame + 45, 0, 0);
            return 1;
        }

        if (pModeChange->pUnit && pModeChange->pUnit->dwUnitType == UNIT_MONSTER && pModeChange->pUnit->pMonsterData)
        {
            D2MonStatsTxt* pMonStatsTxtRecord = pModeChange->pUnit->pMonsterData->pMonstatsTxt;
            if (pMonStatsTxtRecord)
            {
                int32_t nAiDelay = 0;
                if (pGame->nGameType || pGame->dwGameType)
                {
                    nAiDelay = pMonStatsTxtRecord->nAIdel[pGame->nDifficulty];
                }
                else
                {
                    nAiDelay = pMonStatsTxtRecord->nAIdel[0];
                }

                if (nAiDelay <= 0)
                {
                    nAiDelay = 15;
                }

                EVENT_SetEvent(pGame, pModeChange->pUnit, EVENTTYPE_AITHINK, nAiDelay + pGame->dwGameFrame, 0, 0);
            }
        }
    }

    return 1;
}

//D2Game.0x6FC643D0
//1.14d: 0x005A7490
int32_t __fastcall MONSTERMODE_StartSkill3(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    SUNIT_SetCombatMode(pGame, pModeChange->pUnit, 10);
    return 1;
}

//D2Game.0x6FC643E0
//1.14d: 0x005A74A0
int32_t __fastcall MONSTERMODE_UpdateSkill3(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    SUNIT_UpdateUnitPath(pGame, pUnit);
    UNITS_StopSequence(pUnit);
    if (UNITS_IsAtEndOfFrameCycle(pUnit))
    {
        SUNIT_SetCombatMode(pGame, pUnit, 11);
    }

    return 1;
}

//D2Game.0x6FC64410
//1.14d: 0x005A74E0
int32_t __fastcall MONSTERMODE_StartSkill4(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    SUNIT_SetCombatMode(pGame, pModeChange->pUnit, 11);
    EVENT_SetEvent(pGame, pModeChange->pUnit, EVENTTYPE_AITHINK, pGame->dwGameFrame + 15, 0, 0);
    return 1;
}

//D2Game.0x6FC64450
//1.14d: 0x005A7520
int32_t __fastcall MONSTERMODE_StartWalk(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    if (PATH_GetNumberOfPathPoints(pModeChange->pUnit->pDynamicPath))
    {
        SUNIT_SetCombatMode(pGame, pModeChange->pUnit, 2);
        return 1;
    }

    return 0;
}

//D2Game.0x6FC64480
//1.14d: 0x005A8490
int32_t __fastcall MONSTERMODE_UpdateWalk(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (STATES_CheckState(pUnit, STATE_BLAZE))
    {
        SKILLS_CreateBlazeMissile(pGame, pUnit);
    }

    if (STATES_CheckState(pUnit, STATE_SPIDERLAY))
    {
        SKILLS_CreateSpiderLayMissile(pGame, pUnit);
    }

    if (SUNIT_UpdateUnitPath(pGame, pUnit) == 2)
    {
        MONSTERMODE_HandleEndAnimation(pGame, pUnit);
        return 2;
    }

    return 1;
}

//D2Game.0x6FC644E0
//1.14d: 0x005A7550
int32_t __fastcall MONSTERMODE_StartRun(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    if (PATH_GetNumberOfPathPoints(pModeChange->pUnit->pDynamicPath))
    {
        SUNIT_SetCombatMode(pGame, pModeChange->pUnit, 15);
        return 1;
    }

    return 0;
}

//D2Game.0x6FC64510
//1.14d: 0x005A84F0
int32_t __fastcall MONSTERMODE_UpdateRun(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (SUNIT_UpdateUnitPath(pGame, pUnit) == 2)
    {
        MONSTERMODE_HandleEndAnimation(pGame, pUnit);
        return 2;
    }

    return 1;
}

//D2Game.0x6FC64540
//1.14d: 0x005A7580
int32_t __fastcall MONSTERMODE_StartGetHit(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    if (pModeChange->pUnit)
    {
        if (pModeChange->pUnit->dwAnimMode != MONMODE_DEATH && pModeChange->pUnit->dwAnimMode != MONMODE_DEAD)
        {
            D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pModeChange->pUnit->dwClassId);
            if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[3]))
            {
                return 0;
            }

            SUNIT_SetCombatMode(pGame, pModeChange->pUnit, 3);
        }
    }

    return 1;
}

//D2Game.0x6FC645E0
//1.14d: 0x005A75C0
int32_t __fastcall MONSTERMODE_StartAttackOrSkill(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    if (pModeChange->pUnit && pModeChange->nMode < 16)
    {
        bool bContinue = false;
        const D2MonModeInfoTableStrc* pMonModeInfoTableRecord = &gtMonsterModeEventPolicies[pModeChange->nMode];
        if (pMonModeInfoTableRecord->bUseRecurringEvent)
        {
            bContinue = true;
        }
        else if (!pMonModeInfoTableRecord->bUseAnimationEvents && pMonModeInfoTableRecord->bCheckMovingMode)
        {
            D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pModeChange->pUnit->dwClassId);
            if (pMonStats2TxtRecord && pMonStats2TxtRecord->nModeEnabledWhenMovingFlags[pModeChange->nMode >> 3] & gdwBitMasks[pModeChange->nMode & 7])
            {
                bContinue = true;
            }
        }

        if (bContinue)
        {
            if (!PATH_GetNumberOfPathPoints(pModeChange->pUnit->pDynamicPath))
            {
                if (pModeChange->pUnit->dwUnitType == UNIT_MONSTER)
                {
                    int32_t nBaseId = pModeChange->pUnit->dwClassId;
                    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nBaseId);
                    if (pMonStatsTxtRecord)
                    {
                        nBaseId = pMonStatsTxtRecord->nBaseId;
                    }

                    if (nBaseId == MONSTER_VULTURE1)
                    {
                        D2Common_10142(pModeChange->pUnit->pDynamicPath, pModeChange->pUnit, 0);
                        SUNIT_SetCombatMode(pGame, pModeChange->pUnit, pModeChange->nMode);
                        return 1;
                    }
                }

                return 0;
            }

            D2Common_10142(pModeChange->pUnit->pDynamicPath, pModeChange->pUnit, 0);
        }
    }

    SUNIT_SetCombatMode(pGame, pModeChange->pUnit, pModeChange->nMode);

    if (UNITS_GetUsedSkill(pModeChange->pUnit))
    {
        SKILLS_StartUsedSkill(pGame, pModeChange->pUnit);
    }

    return 1;
}

//D2Game.0x6FC64790
//1.14d: 0x005A7670
int32_t __fastcall MONSTERMODE_UpdateAttackOrSkill(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (!pSkill)
    {

        int32_t bUseRecurringEvent = 0;
        if (pUnit)
        {
            if (pUnit->dwAnimMode < 16)
            {
                const D2MonModeInfoTableStrc* pMonModeInfoTableRecord = &gtMonsterModeEventPolicies[pUnit->dwAnimMode];
                if (pMonModeInfoTableRecord->bUseRecurringEvent)
                {
                    bUseRecurringEvent = 1;
                }
                else if (!pMonModeInfoTableRecord->bUseAnimationEvents && pMonModeInfoTableRecord->bCheckMovingMode)
                {
                    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
                    if (pMonStats2TxtRecord && pMonStats2TxtRecord->nModeEnabledWhenMovingFlags[pUnit->dwAnimMode >> 3] & gdwBitMasks[pUnit->dwAnimMode & 7])
                    {
                        bUseRecurringEvent = 1;
                    }
                }
            }
        }

        if (bUseRecurringEvent)
        {
            SUNIT_UpdateUnitPath(pGame, pUnit);
            UNITS_StopSequence(pUnit);

            if (UNITS_IsAtEndOfFrameCycle(pUnit))
            {
                return 2;
            }

            if (pUnit->dwAnimMode == MONMODE_SEQUENCE)
            {
                if (!(pUnit->dwFlags & UNITFLAG_SKSRVDOFUNC))
                {
                    return 1;
                }
            }
            else
            {
                if (pUnit->nActionFrame != 1)
                {
                    return 1;
                }
            }
        }

        int32_t nMissileId = D2Common_11050(pUnit, bUseRecurringEvent);
        if (nMissileId >= 0)
        {
            int32_t nSkillLevel = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty)->dwMonsterSkillBonus + 1;
            if (pUnit->dwFlags & UNITFLAG_ISMERC)
            {
                nSkillLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
            }

            if (SKILLS_CreateMissileAtOffset(pGame, nMissileId, pUnit, 0, nSkillLevel, 0, 0, 0, 0, 1))
            {
                const int32_t nBaseId = D2GAME_GetMonsterBaseId_6FC64B10(pUnit);
                if (nBaseId == MONSTER_QUILLRAT1)
                {
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

                    int32_t nYOffset = 5;
                    int32_t nXOffset = 5;
                    
                    PATH_SetTargetUnit(pUnit->pDynamicPath, nullptr);

                    D2SeedStrc seed = {};
                    SEED_InitSeed(&seed);
                    SEED_InitLowSeed(&seed, 'SEIS');
                    seed.nHighSeed = pUnit->dwUnitId;

                    const int32_t nParam = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId)->wAiParam[2][pGame->nDifficulty];
                    for (int32_t i = 0; i < nParam; ++i)
                    {
                        if (ITEMS_RollRandomNumber(&seed) & 1)
                        {
                            nXOffset = -nXOffset;
                        }

                        if (ITEMS_RollRandomNumber(&seed) & 1)
                        {
                            nYOffset = -nYOffset;
                        }

                        D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nX + nXOffset, nY + nYOffset);
                        SKILLS_CreateMissileAtOffset(pGame, nMissileId, pUnit, 0, 1, 0, 0, 0, 0, 0);
                    }

                    PATH_SetTargetUnit(pUnit->pDynamicPath, pTarget);
                    return 1;
                }
            }
        }
        else
        {
            MONSTERMODE_AllocateTargetCombat(pUnit, pGame);

            D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
            if (pTarget)
            {
                SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
            }
        }

        return 1;
    }

    const int32_t nSkillFlags = SKILLS_GetFlags(pSkill);
    if (!(nSkillFlags & 1))
    {
        SKILLS_ExecuteUsedSkill(pGame, pUnit);
        UNITS_StopSequence(pUnit);
        return 1;
    }

    SUNIT_ValidateTargetUnit(pGame, pUnit);

    if (SUNIT_UpdateUnitPath(pGame, pUnit) != 2)
    {
        SKILLS_ExecuteUsedSkill(pGame, pUnit);
        UNITS_StopSequence(pUnit);
        return 1;
    }

    SKILLS_SetFlags(pSkill, nSkillFlags | 2);

    SKILLS_ExecuteUsedSkill(pGame, pUnit);

    if (pUnit && pUnit->dwAnimMode == MONMODE_SEQUENCE)
    {
        if (pUnit->dwFlags & UNITFLAG_SKSRVDOFUNC)
        {
            SKILLS_ExecuteUsedSkill(pGame, pUnit);
        }
    }
    else
    {
        if (pUnit->nActionFrame == 1)
        {
            SKILLS_ExecuteUsedSkill(pGame, pUnit);
        }
    }

    UNITS_StopSequence(pUnit);
    return 1;
}

//D2Game.0x6FC64B10
//1.14d: 0x00463860
int32_t __fastcall D2GAME_GetMonsterBaseId_6FC64B10(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
        if (pMonStatsTxtRecord)
        {
            return pMonStatsTxtRecord->nBaseId;
        }
        else
        {
            return pUnit->dwClassId;
        }
    }

    return -1;
}

//D2Game.0x6FC64B50
//1.14d: 0x005A77C0
int32_t __fastcall MONSTERMODE_StartBlock(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    SUNIT_SetCombatMode(pGame, pModeChange->pUnit, 6);
    return 1;
}

//D2Game.0x6FC64B60
//1.14d: 0x005A77D0
int32_t __fastcall MONSTERMODE_StartKnockback(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    if (!pModeChange->pUnit)
    {
        return 0;
    }

    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pModeChange->pUnit->dwClassId);
    if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[3] && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[13])
    {
        if (pModeChange->pUnit->dwAnimMode)
        {
            PATH_SetType(pModeChange->pUnit->pDynamicPath, PATHTYPE_KNOCKBACK_SERVER);

            int32_t nDistance = 5;
            if (pModeChange->pUnit->dwUnitType == UNIT_MONSTER)
            {
                int32_t nBaseId = pModeChange->pUnit->dwClassId;
                D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nBaseId);
                if (pMonStatsTxtRecord)
                {
                    nBaseId = pMonStatsTxtRecord->nBaseId;
                }

                if (nBaseId == MONSTER_SANDLEAPER1)
                {
                    nDistance = 10;
                }
            }

            D2COMMON_10190_PATH_SetDistance(pModeChange->pUnit->pDynamicPath, nDistance);
            D2Common_10142(pModeChange->pUnit->pDynamicPath, pModeChange->pUnit, 0);
            SUNIT_SetCombatMode(pGame, pModeChange->pUnit, 13);
        }

        return 1;
    }

    return 0;
}

//D2Game.0x6FC64CD0
//1.14d: 0x005A8520
void __fastcall MONSTERMODE_EndKnockback(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    PATH_ResetToPreviousType(pUnit->pDynamicPath);

    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
    if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[3])
    {
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            const int32_t nClassId = sgptDataTables->pMonStatsTxt[pUnit->dwClassId].nBaseId;
            if (nClassId == MONSTER_SANDLEAPER1)
            {
                if (!STATES_CheckState(pUnit, STATE_STUNNED))
                {
                    EVENT_SetEvent(pGame, pUnit, EVENTTYPE_AITHINK, pGame->dwGameFrame + 15, 0, 0);
                }
                else
                {
                    EVENT_SetEvent(pGame, pUnit, EVENTTYPE_AITHINK, pGame->dwGameFrame + 45, 0, 0);
                }
                return;
            }
        }

        UNITS_SetUsedSkill(pUnit, 0);

        D2ModeChangeStrc pModeChange = {};
        pModeChange.nMode = MONMODE_GETHIT;
        pModeChange.pUnit = pUnit;
        pModeChange.unk0x14[1] = 100;
        pUnit->dwLastHitClass = 160;
        D2GAME_ModeChange_6FC65220(pGame, &pModeChange, 1);
    }
    else
    {
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_AITHINK, pGame->dwGameFrame + 1, 0, 0);
    }
}

//D2Game.0x6FC64E20
//1.14d: 0x005A8630
int32_t __fastcall MONSTERMODE_UpdateKnockback(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    SUNIT_UpdateUnitPath(pGame, pUnit);
    UNITS_StopSequence(pUnit);
    if (UNITS_IsAtEndOfFrameCycle(pUnit))
    {
        MONSTERMODE_EndKnockback(pGame, pUnit);
        return 2;
    }

    return 1;
}

//D2Game.0x6FC64E60
//1.14d: 0x005A7870
int32_t __fastcall MONSTERMODE_StartSequence(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange)
{
    SUNIT_SetCombatMode(pGame, pModeChange->pUnit, 14);

    if (pModeChange->pUnit)
    {
        pModeChange->pUnit->dwFlags &= 0xFFFFFFBF;
    }

    return SKILLS_StartUsedSkill(pGame, pModeChange->pUnit);
}

//D2Game.0x6FC64E90
//1.14d: 0x005A8670
int32_t __fastcall MONSTERMODE_UpdateSequence(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (UNITS_IsAtEndOfFrameCycle(pUnit))
    {
        MONSTERMODE_HandleEndAnimation(pGame, pUnit);
        return 2;
    }

    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    const int32_t nSkillFlags = SKILLS_GetFlags(pSkill);
    int32_t bCheck = 1;
    if (nSkillFlags & 1)
    {
        SUNIT_ValidateTargetUnit(pGame, pUnit);

        if (SUNIT_UpdateUnitPath(pGame, pUnit) == 2)
        {
            SKILLS_SetFlags(pSkill, nSkillFlags | 2);
            SKILLS_ExecuteUsedSkill(pGame, pUnit);
            bCheck = 0;
        }
    }

    const int32_t nActionFrame = pUnit->nActionFrame;
    if (nActionFrame == 4 || bCheck && !((pUnit->dwFlags >> 6) & 1) && (nActionFrame == 1 || nActionFrame == 2))
    {
        SKILLS_ExecuteUsedSkill(pGame, pUnit);
    }

    UNITS_StopSequence(pUnit);

    return 1;
}

//1.10: D2Game.0x6FC64F50
//1.14d: 0x005A78A0
const D2MonModeCallbackTableStrc* __fastcall MONSTERMODE_GetCallbackTableRecord(D2UnitStrc* pUnit, int32_t nMode)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    if (!pMonStatsTxtRecord)
    {
        return 0;
    }

    if (nMode < 0 || nMode >= 16)
    {
        FOG_Trace("Invalid monster mode.  Class:%d  Mode:%d", pUnit->dwClassId, nMode);
        return 0;
    }

    if (pMonStatsTxtRecord->nSplGetModeChart)
    {
        switch (pUnit->dwClassId)
        {
        case MONSTER_DIABLO:
        case MONSTER_DIABLOCLONE:
#ifdef D2_VERSION_HAS_UBERS
        case MONSTER_UBERDIABLO:
#endif
            if (nMode == MONMODE_SKILL4)
            {
                return &gDiablo_Skill4_Callbacks;
            }

            if (nMode == MONMODE_SKILL3)
            {
                return &gDiablo_Skill3_Callbacks;
            }
            break;

        case MONSTER_TRAPPEDSOUL1:
            if (nMode == MONMODE_ATTACK1 || nMode == MONMODE_ATTACK2)
            {
                return &gTrappedSoul_Attack_Callbacks;
            }

            if (nMode == MONMODE_SKILL2 || nMode == MONMODE_SKILL1)
            {
                return &gTrappedSoul_Skill_Callbacks;
            }
            break;

        case MONSTER_SHADOWWARRIOR:
        case MONSTER_SHADOWMASTER:
            if (nMode == MONMODE_SKILL4)
            {
                return &gShadowMasterWarrior_Skill4_Callbacks;
            }
            break;

        case MONSTER_BAALTHRONE:
        case MONSTER_BAALCRAB:
        case MONSTER_BAALCLONE:
#ifdef D2_VERSION_HAS_UBERS
        case MONSTER_UBERBAAL:
#endif
            if (nMode == MONMODE_SKILL3)
            {
                return &gMonModeCallbacks[7];
            }
            break;
		default:
			break;
        }
    }

    return &gMonModeCallbacks[nMode];
}

//D2Game.0x6FC65080
//1.14d: 0x005A7BA0
void __fastcall MONSTERMODE_EventModeChange(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nEventCustomParam)
{
    int32_t nAnimMode = 0;
    if (pUnit)
    {
        nAnimMode = pUnit->dwAnimMode;
    }

    const D2MonModeCallbackTableStrc* pMonModeCallbackTableRecord = MONSTERMODE_GetCallbackTableRecord(pUnit, nAnimMode);
    if (pMonModeCallbackTableRecord && pMonModeCallbackTableRecord->pfUpdate)
    {
        pMonModeCallbackTableRecord->pfUpdate(pGame, pUnit);
        return;
    }

    UNITS_SetUsedSkill(pUnit, nullptr);

    if (nAnimMode == MONMODE_DEATH || nAnimMode == MONMODE_DEAD || !SUNIT_IsDead(pUnit))
    {
        D2ModeChangeStrc modeChange = {};

        modeChange.pUnit = pUnit;
        modeChange.nMode = MONMODE_NEUTRAL;

        MONSTERMODE_StartNeutral(pGame, &modeChange);
    }
}

//D2Game.0x6FC65150
//1.14d: 0x005A7BE0
void __fastcall MONSTERMODE_EventEndAnimation(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nEventCustomParam)
{
    int32_t nAnimMode = 0;
    if (pUnit)
    {
        nAnimMode = pUnit->dwAnimMode;
    }

    const D2MonModeCallbackTableStrc* pMonModeCallbackTableRecord = MONSTERMODE_GetCallbackTableRecord(pUnit, nAnimMode);
    if (pMonModeCallbackTableRecord && pMonModeCallbackTableRecord->pfEnd)
    {
        pMonModeCallbackTableRecord->pfEnd(pGame, pUnit);
        return;
    }

    UNITS_SetUsedSkill(pUnit, nullptr);

    if (nAnimMode == MONMODE_DEATH || nAnimMode == MONMODE_DEAD || !SUNIT_IsDead(pUnit))
    {
        D2ModeChangeStrc modeChange = {};

        modeChange.pUnit = pUnit;
        modeChange.nMode = MONMODE_NEUTRAL;

        MONSTERMODE_StartNeutral(pGame, &modeChange);
    }
}

//D2Game.0x6FC65220
//1.14d: 0x005A7C20
int32_t __stdcall D2GAME_ModeChange_6FC65220(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange, int32_t bClearUsedSkill)
{
    // TODO: v12, v13, dwNewDist

    D2UnitStrc* pUnit = pModeChange->pUnit;
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    const int32_t nMode = pModeChange->nMode;

    D2_ASSERT(!STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE));

    if (STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
    {
        SUNIT_SetUninterruptable(pUnit, 0);

        if (SUNIT_IsDead(pUnit))
        {
            return 1;
        }
    }

    if (!SUNIT_IsDead(pUnit))
    {
        PATH_AddCollisionFootprintForUnit(pUnit);
    }

    int32_t nNewAiState = pUnit->dwAnimMode;
    if (nNewAiState != MONMODE_NEUTRAL)
    {
        const int32_t nAiState = MONSTER_GetAiState(pUnit);
        if (nAiState < 16)
        {
            if (nAiState == MONMODE_KNOCKBACK && nNewAiState == MONMODE_GETHIT)
            {
                nNewAiState = MONMODE_KNOCKBACK;
            }
        }
        else
        {
            nNewAiState = nAiState - 16;
        }

        MONSTER_SetAiState(pUnit, nNewAiState);
    }

    D2AiParamStrc* pAiParam = nullptr;
    if (nMode != MONMODE_GETHIT)
    {
        if (pModeChange->pTargetUnit)
        {
            PATH_SetTargetUnit(pUnit->pDynamicPath, pModeChange->pTargetUnit);
        }
        else
        {
            D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, pModeChange->nX, pModeChange->nY);
        }

        PATH_SetDistance(pUnit->pDynamicPath, 20);

        if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
        {
            pAiParam = pUnit->pMonsterData->pAiParam;
        }

        int32_t nVelocityPercent = pModeChange->unk0x18;
        uint8_t nPathType = pModeChange->unk0x14[1];
        uint8_t dwNewDist = pModeChange->unk0x1C;

        if (pAiParam->unk0x18)
        {
            nPathType = pAiParam->unk0x18;
        }

        if (pAiParam->nVelocity)
        {
            nVelocityPercent = pAiParam->nVelocity;
        }

        if (pAiParam->unk0x20)
        {
            dwNewDist = pAiParam->unk0x20;
        }

        pAiParam->unk0x18 = 0;
        pAiParam->nVelocity = 0;
        pAiParam->unk0x20 = 0;

        int32_t v13 = -1;
        int32_t nNewPathType = 0;
        if (nPathType == 100)
        {
            nNewPathType = 0;
            nVelocityPercent = 0;
        }
        else
        {
            if (!dwNewDist)
            {
                dwNewDist = 5;
            }

            if (nPathType == 101)
            {
                nPathType = PATHTYPE_TOWARD_FINISH;
            }

            nNewPathType = MONSTERMODE_ComputePath(pUnit, nPathType, pAiParam, dwNewDist);
            if (nNewPathType != PATHTYPE_ASTAR && PATH_GetTargetUnit(pUnit->pDynamicPath))
            {
                v13 = 10;
            }
        }

        pAiParam->unk0x14 = v13;
        pAiParam->unk0x10 = nVelocityPercent;

        ++pGame->dwPathTypesCount[nNewPathType];

        if (nNewPathType)
        {
            ++pGame->nTotalPathTypesCount;
        }

        MONSTERMODE_UpdateModeAttackStats(pUnit, nMode);
        MONSTERUNIQUE_ExecuteUModModeStartCallbacks(pGame, pUnit);
    }

    const D2MonModeCallbackTableStrc* pTargetMonModeCallbackTableRecord = MONSTERMODE_GetCallbackTableRecord(pUnit, nMode);
    if (!pTargetMonModeCallbackTableRecord || !pTargetMonModeCallbackTableRecord->pfStart)
    {
        UNITS_SetUsedSkill(pUnit, nullptr);
        if (nMode == MONMODE_DEATH || nMode == MONMODE_DEAD || !SUNIT_IsDead(pUnit))
        {
            D2ModeChangeStrc modeChange = {};
            modeChange.pUnit = pUnit;
            modeChange.nMode = MONMODE_NEUTRAL;
            MONSTERMODE_StartNeutral(pGame, &modeChange);
        }

        return 0;
    }

    const int32_t nResult = pTargetMonModeCallbackTableRecord->pfStart(pGame, pModeChange);
    if (!nResult)
    {
        UNITS_SetUsedSkill(pUnit, nullptr);
        if (nMode && nMode != MONMODE_DEAD && SUNIT_IsDead(pUnit))
        {
            return 1;
        }

        MONSTERMODE_StartNeutral(pGame, pModeChange);
    }

    pUnit->dwFlags |= UNITFLAG_MONMODEISCHANGING;

    SUNIT_InitializeAnimation(pUnit);

    if (pAiParam && pAiParam->unk0x10)
    {
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndFlag(pUnit, 0x4000u);
        if (pStatList)
        {
            STATLIST_SetStat(pStatList, STAT_VELOCITYPERCENT, pAiParam->unk0x10, 0);
            UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
        }
        else
        {
            pStatList = STATLIST_AllocStatList(pUnit->pMemoryPool, 0x4004u, 0, 1, pUnit->dwUnitId);
            if (pStatList)
            {
                D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
                STATLIST_SetStat(pStatList, STAT_VELOCITYPERCENT, pAiParam->unk0x10, 0);
                UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
            }
        }
    }

    MONSTERUNIQUE_ExecuteUModModeChangeCallbacks(pGame, pUnit);

    const int32_t nCurrentAnimMode = pUnit->dwAnimMode;
    if (bClearUsedSkill && nMode == nCurrentAnimMode)
    {
        UNITS_SetUsedSkill(pUnit, nullptr);
    }

    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pUnit);

    const D2MonModeCallbackTableStrc* pCurrentMonModeCallbackTableRecord = MONSTERMODE_GetCallbackTableRecord(pUnit, nCurrentAnimMode);
    if (!pCurrentMonModeCallbackTableRecord)
    {
        UNITS_SetUsedSkill(pUnit, nullptr);
        if (nMode == MONMODE_DEATH || nMode == MONMODE_DEAD || !SUNIT_IsDead(pUnit))
        {
            D2ModeChangeStrc modeChange = {};
            modeChange.pUnit = pUnit;
            modeChange.nMode = MONMODE_NEUTRAL;
            MONSTERMODE_StartNeutral(pGame, &modeChange);
        }
        return 0;
    }

    if (pCurrentMonModeCallbackTableRecord->bScheduleEvents)
    {
        if (nCurrentAnimMode >= 16)
        {
            SUNIT_ScheduleAnimationEvents(pGame, pUnit);
        }
        else
        {
            const D2MonModeInfoTableStrc* pMonModeInfoTableRecord = &gtMonsterModeEventPolicies[nCurrentAnimMode];
            if (!pMonModeInfoTableRecord->bUseRecurringEvent)
            {
                if (pMonModeInfoTableRecord->bUseAnimationEvents || !pMonModeInfoTableRecord->bCheckMovingMode)
                {
                    SUNIT_ScheduleAnimationEvents(pGame, pUnit);
                }
                else
                {
                    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
                    if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->nModeEnabledWhenMovingFlags[nCurrentAnimMode >> 3] & gdwBitMasks[nCurrentAnimMode & 7]))
                    {
                        SUNIT_ScheduleAnimationEvents(pGame, pUnit);
                    }
                    else
                    {
                        SUNIT_ResetRecurringModeChangeEvent(pGame, pUnit);
                    }
                }
            }
            else
            {
                SUNIT_ResetRecurringModeChangeEvent(pGame, pUnit);
            }
        }
    }

    if (nCurrentAnimMode == MONMODE_GETHIT)
    {
        PATH_SetType(pUnit->pDynamicPath, PATHTYPE_IDASTAR);
        PATH_SetNewDistance(pUnit->pDynamicPath, 0);
    }

    return nResult;
}

//D2Game.0x6FC65680
//1.14d: 0x005A6290
int32_t __fastcall MONSTERMODE_ComputePath(D2UnitStrc* pUnit, int32_t nPathType, D2AiParamStrc* pAiParam, int32_t dwNewDist)
{
    PATH_SetNewDistance(pUnit->pDynamicPath, dwNewDist);

    D2UnitStrc* pTarget = PATH_GetTargetUnit(pUnit->pDynamicPath);
    const int32_t nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
    const int32_t nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);

    if (pTarget != pAiParam->pTarget || nX != pAiParam->unk0x08 || nY != pAiParam->unk0x0C)
    {
        pAiParam->pTarget = pTarget;
        pAiParam->unk0x08 = nX;
        pAiParam->unk0x0C = nY;
        pAiParam->unk0x00 = 0;
        pAiParam->unk0x01 = 0;
    }

    PATH_SetType(pUnit->pDynamicPath, nPathType);
    D2Common_10142(pUnit->pDynamicPath, pUnit, 0);

    if (PATH_GetNumberOfPathPoints(pUnit->pDynamicPath))
    {
        UNITROOM_RefreshUnit(pUnit);
        pUnit->dwFlags |= UNITFLAG_DOUPDATE;
        pAiParam->unk0x00 = 0;
        pAiParam->unk0x01 = 0;
    }
    else
    {
        switch (nPathType)
        {
        case PATHTYPE_TOWARD:
        case PATHTYPE_STRAIGHT:
        case PATHTYPE_LEAP:
        case PATHTYPE_TOWARD_FINISH:
            UNITROOM_RefreshUnit(pUnit);
            pUnit->dwFlags |= UNITFLAG_DOUPDATE;
            PATH_SetType(pUnit->pDynamicPath, PATHTYPE_WALL_FOLLOW);
            D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
            return PATHTYPE_WALL_FOLLOW;

        default:
            break;
        }
    }

    return nPathType;
}

//D2Game.0x6FC65780
//1.14d: 0x005A7E60
void __stdcall MONSTERMODE_GetModeChangeInfo(D2UnitStrc* pUnit, int32_t nMode, D2ModeChangeStrc* pModeChange)
{
    D2_ASSERT(nMode < 16);

    UNITS_SetUsedSkill(pUnit, nullptr);

    memset(pModeChange, 0x00, sizeof(D2ModeChangeStrc));

    pModeChange->unk0x14[1] = 100;
    pModeChange->nMode = nMode;
    pModeChange->pUnit = pUnit;

    if (!pUnit)
    {
        return;
    }

    const D2MonModeInfoTableStrc* pMonModeInfoTableRecord = &gtMonsterModeEventPolicies[nMode];
    if (pMonModeInfoTableRecord->bUseRecurringEvent)
    {
        pModeChange->unk0x14[1] = 101;
    }
    else if (!pMonModeInfoTableRecord->bUseAnimationEvents && pMonModeInfoTableRecord->bCheckMovingMode)
    {
        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
        if (pMonStats2TxtRecord && pMonStats2TxtRecord->nModeEnabledWhenMovingFlags[nMode >> 3] & gdwBitMasks[nMode & 7])
        {
            pModeChange->unk0x14[1] = 101;
        }
    }
}

//D2Game.0x6FC65890
//1.14d: 0x005A7EF0
void __fastcall MONSTERMODE_EventUpdateStatListExpirations(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nEventCustomParam)
{
    STATLIST_UpdateStatListsExpiration(pUnit, pGame->dwGameFrame);
}

//D2Game.0x6FC658B0
//1.14d: 0x005A7F00
void __fastcall MONSTERMODE_EventFreeHoverText(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nEventCustomParam)
{
    if (!pUnit || !pUnit->pHoverText)
    {
        return;
    }

    const int32_t nTimeout = CHAT_GetTimeoutFromHoverMsg(pUnit->pHoverText);
    if (nTimeout > pGame->dwGameFrame)
    {
        EVENT_SetEvent(pGame, pUnit, EVENTTYPE_FREEHOVER, nTimeout, 0, 0);
    }
    else
    {
        CHAT_FreeHoverMsg(pGame->pMemoryPool, pUnit->pHoverText);
        pUnit->pHoverText = nullptr;
        UNITROOM_RefreshUnit(pUnit);
        pUnit->dwFlags |= UNITFLAG_HASTXTMSG;
    }
}

//D2Game.0x6FC65920
//1.14d: 0x005A7F70
void __fastcall MONSTERMODE_EventClearTargetOverride(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nEventCustomParam)
{
    MONSTERAI_ClearTargetOverride(pUnit);
}

//D2Game.0x6FC65930
//1.14d: 0x005A7F80
void __fastcall MONSTERMODE_EventHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEvent, int32_t nSkillId, int32_t nSkillLevel)
{
    if (nEvent < 0 || nEvent >= EVENTTYPE_COUNT)
    {
        return;
    }

    switch (nEvent)
    {
    case EVENTTYPE_STATREGEN:
    case EVENTTYPE_TRAP:
    case EVENTTYPE_RESET:
    case EVENTTYPE_PERIODICSKILLS:
    case EVENTTYPE_REMOVESTATE:
        break;
    default:
        if (STATES_CheckState(pUnit, STATE_FREEZE) && !SUNIT_IsDead(pUnit))
        {
            return;
        }
        break;
    }

    const MonEventFunc pfMonEvent = gpMonsterEventFunctions[nEvent];
    if (pfMonEvent)
    {
        pfMonEvent(pGame, pUnit, nSkillId, nSkillLevel);
    }
}

//D2Game.0x6FC659B0
//1.14d: 0x005A8000
void __fastcall MONSTERMODE_DropTreasureForFindItem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t a4)
{
    D2ModeChangeStrc modeChange = {};

    modeChange.pUnit = pUnit;
    modeChange.pTargetUnit = pTarget;

    MONSTERMODE_DropTreasure(pGame, pUnit, 1, &modeChange);
}
