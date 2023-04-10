#include "MONSTER/MonsterUnique.h"

#include <algorithm>

#include <D2BitManip.h>
#include <D2Math.h>

#include <D2StatList.h>
#include <D2DataTbls.h>
#include <D2Skills.h>
#include <DataTbls/SkillsIds.h>
#include <D2Monsters.h>
#include <D2States.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/MissilesIds.h>
#include <D2Collision.h>
#include <D2Items.h>
#include <D2Dungeon.h>


#include "AI/AiGeneral.h"
#include "AI/AiTactics.h"
#include "AI/AiThink.h"
#include "AI/AiUtil.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"
#include "PLAYER/PlayerPets.h"
#include "QUESTS/Quests.h"
#include "QUESTS/QuestsFX.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/Skills.h"
#include "SKILLS/SkillSor.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"


#pragma pack(push, 1)
struct D2AuraModStrc
{
    int32_t nMinLevel;
    int32_t nLevelOffset;
    int32_t nMultiplier;
    int32_t nDivisor;
    int32_t nSkillId;
};

using UModInitFunc = void(__fastcall*)(D2UnitStrc*, int32_t, int32_t);

using MonUModFunc = void(__fastcall*)(D2GameStrc*, D2UnitStrc*, int32_t, int32_t);
struct D2UnkUModStrc
{
    MonUModFunc unk0x00[6];
};
#pragma pack(pop)


//D2Game.0x6FC6AC00
void __fastcall MONSTERUNIQUE_ToggleUnitFlag(D2UnitStrc* pUnit, int32_t nFlag, int32_t bSet)
{
    if (!pUnit)
    {
        return;
    }

    if (bSet)
    {
        pUnit->dwFlags |= nFlag;
    }
    else
    {
        pUnit->dwFlags &= ~nFlag;
    }
}

//D2Game.0x6FC6AC30
uint8_t* __fastcall MONSTERUNIQUE_GetUMods(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        return pUnit->pMonsterData->nMonUmod;
    }

    return nullptr;
}

//D2Game.0x6FC6AC50
uint16_t __fastcall MONSTERUNIQUE_GetNameSeed(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        return pUnit->pMonsterData->wNameSeed;
    }

    return 5382;
}

//D2Game.0x6FC6AC70
int32_t __fastcall MONSTERUNIQUE_CheckMonTypeFlag(D2UnitStrc* pUnit, uint16_t nFlag)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        return (pUnit->pMonsterData->nTypeFlag & nFlag) != 0;
    }

    return 0;
}

//D2Game.0x6FC6ACA0
void __fastcall MONSTERUNIQUE_ToggleMonTypeFlag(D2UnitStrc* pUnit, uint16_t nFlag, int32_t bSet)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
    {
        return;
    }

    if (bSet)
    {
        pUnit->pMonsterData->nTypeFlag |= nFlag;
    }
    else
    {
        pUnit->pMonsterData->nTypeFlag &= ~nFlag;
    }
}

//D2Game.0x6FC6ACD0
int16_t __fastcall MONSTERUNIQUE_GetBossHcIdx(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        return pUnit->pMonsterData->wBossHcIdx;
    }

    return -1;
}

//D2Game.0x6FC6ACF0
int32_t __fastcall MONSTERUNIQUE_HasUMods(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        return pUnit->pMonsterData->nMonUmod[0] != 0;
    }

    return 0;
}

//D2Game.0x6FC6AD10
int32_t __fastcall MONSTERUNIQUE_GetSuperUniqueBossHcIdx(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData && pUnit->pMonsterData->nTypeFlag & MONTYPEFLAG_SUPERUNIQUE)
    {
        return pUnit->pMonsterData->wBossHcIdx;
    }

    return -1;
}

//D2Game.0x6FC6AD50
void __fastcall MONSTERUNIQUE_UMod1_RandomName(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (pUnit && bUnique && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        pUnit->pMonsterData->wNameSeed = ITEMS_RollRandomNumber(&pUnit->pSeed);
    }
}

//D2Game.0x6FC6AD90
void __fastcall MONSTERUNIQUE_UMod2_HealthBonus(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    if (!pGame)
    {
        return;
    }

    const uint8_t nDifficulty = pGame->nDifficulty;
    if (nDifficulty >= 3)
    {
        return;
    }

    if (bUnique)
    {
        int32_t nHitpoints = 0;
        if (pUnit->pMonsterData && pUnit->pMonsterData->nTypeFlag & MONTYPEFLAG_CHAMPION)
        {
            D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 4);
            const int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

            nHitpoints = STATLIST_GetMaxLifeFromUnit(pUnit);
            nHitpoints += MONSTERUNIQUE_CalculatePercentage(nHitpoints, nPercentage, 100);
        }
        else
        {
            D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 7);
            const int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

            nHitpoints = STATLIST_GetMaxLifeFromUnit(pUnit);
            nHitpoints += MONSTERUNIQUE_CalculatePercentage(nHitpoints, nPercentage, 100);
        }

        STATLIST_SetUnitStat(pUnit, STAT_MAXHP, nHitpoints, 0);
        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nHitpoints, 0);
        STATLIST_SetUnitStat(pUnit, STAT_HPREGEN, 0, 0);
    }
    else
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 1);
        const int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        int32_t nHitpoints = STATLIST_GetMaxLifeFromUnit(pUnit);
        nHitpoints += MONSTERUNIQUE_CalculatePercentage(nHitpoints, nPercentage, 100);

        STATLIST_SetUnitStat(pUnit, STAT_MAXHP, nHitpoints, 0);
        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nHitpoints, 0);
    }
}

//D2Game.0x6FC6AF70
int32_t __fastcall MONSTERUNIQUE_CalculatePercentage(int32_t a1, int32_t a2, int32_t a3)
{
    if (!a3)
    {
        return 0;
    }

    if (a1 <= 1048576)
    {
        if (a2 <= 65536)
        {
            return a2 * a1 / a3;
        }

        if (a3 <= a2 >> 4)
        {
            return a1 * a2 / a3;
        }
    }
    else
    {
        if (a3 <= a1 >> 4)
        {
            return a2 * a1 / a3;
        }
    }

    return a2 * (int64_t)a1 / a3;
}

//D2Game.0x6FC6AFF0
void __fastcall MONSTERUNIQUE_UMod4_LevelBonus(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        STATLIST_SetUnitStat(pUnit, STAT_LEVEL, STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0) + 3, 0);
        STATLIST_SetUnitStat(pUnit, STAT_EXPERIENCE, 5 * STATLIST_GetUnitBaseStat(pUnit, STAT_EXPERIENCE, 0), 0);
    }
}

//D2Game.0x6FC6B030
void __fastcall MONSTERUNIQUE_UMod16_Champion(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    if (!pGame)
    {
        return;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    if (!pMonStatsTxtRecord || !bUnique)
    {
        return;
    }

    STATLIST_SetUnitStat(pUnit, STAT_LEVEL, STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0) - 1, 0);
    const int32_t nExperience = STATLIST_GetUnitBaseStat(pUnit, STAT_EXPERIENCE, 0);
    STATLIST_SetUnitStat(pUnit, STAT_EXPERIENCE, nExperience - 2 * nExperience / 5u, 0);

    D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(11);
    int32_t nDamagePercent = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;
    pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(10);
    int32_t nToHitPercent = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

    D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
    if (pDifficultyLevelsTxtRecord)
    {
        nDamagePercent = nDamagePercent * pDifficultyLevelsTxtRecord->dwChampionDmgBonus / 100;
        nToHitPercent = nToHitPercent * pDifficultyLevelsTxtRecord->dwChampionDmgBonus / 100;
    }

    STATLIST_AddUnitStat(pUnit, STAT_DAMAGEPERCENT, nDamagePercent, 0);
    STATLIST_AddUnitStat(pUnit, STAT_ITEM_TOHIT_PERCENT, nToHitPercent, 0);

    if (pMonStatsTxtRecord->nVelocity <= 0 || nUMod == 36)
    {
        return;
    }

    // TODO: First case can never be reached
    if (nUMod == 36)
    {
        STATLIST_AddUnitStat(pUnit, STAT_VELOCITYPERCENT, -33, 0);
    }
    else if (nUMod == 37)
    {
        const int32_t nVelocityPercent = D2Clamp(2048 / pMonStatsTxtRecord->nVelocity - 128, 10, 100);
        STATLIST_AddUnitStat(pUnit, STAT_VELOCITYPERCENT, nVelocityPercent, 0);
    }
    else
    {
        STATLIST_AddUnitStat(pUnit, STAT_VELOCITYPERCENT, 20, 0);
    }
}

//D2Game.0x6FC6B210
void __fastcall MONSTERUNIQUE_UMod36_Ghostly(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    if (!pGame || !bUnique)
    {
        return;
    }

    if (pUnit->pMonsterData)
    {
        pUnit->pMonsterData->nTypeFlag |= 0x40u;
    }

    STATLIST_SetUnitStat(pUnit, STAT_DAMAGERESIST, 80, 0);
    MONSTERUNIQUE_UMod16_Champion(pUnit, nUMod, bUnique);

    const uint8_t nDifficulty = D2Clamp(pGame->nDifficulty, 0ui8, 2ui8);
    const int32_t nGameType = pGame->nGameType || pGame->dwGameType;
    const int32_t nLevel = D2Clamp(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 1u, (uint32_t)sgptDataTables->nMonLvlTxtRecordCount - 1);

    D2MonLvlTxt* pMonLvlTxtRecord = &sgptDataTables->pMonLvlTxt[nLevel];
    if (!pMonLvlTxtRecord)
    {
        return;
    }

    const int32_t nDamage = nGameType ? pMonLvlTxtRecord->dwLDM[nDifficulty] : pMonLvlTxtRecord->dwDM[nDifficulty];

    D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 22);
    int32_t nDamagePercent = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

    STATLIST_AddUnitStat(pUnit, STAT_COLDMINDAM, nDamage * nDamagePercent / 100, 0);

    pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 25);
    nDamagePercent = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

    STATLIST_AddUnitStat(pUnit, STAT_COLDMAXDAM, nDamage * nDamagePercent / 100, 0);
    STATLIST_AddUnitStat(pUnit, STAT_COLDLENGTH, 150, 0);
}

//D2Game.0x6FC6B3A0
void __fastcall MONSTERUNIQUE_UMod37_Fanatic(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && bUnique)
    {
        STATLIST_SetUnitStat(pUnit, STAT_ITEM_ARMOR_PERCENT, -70, 0);
        MONSTERUNIQUE_UMod16_Champion(pUnit, nUMod, bUnique);
    }
}

//D2Game.0x6FC6B3E0
void __fastcall MONSTERUNIQUE_UMod38_Possessed(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !bUnique)
    {
        return;
    }

    if (pUnit->pMonsterData)
    {
        pUnit->pMonsterData->nTypeFlag |= MONTYPEFLAG_POSSESSED;
    }

    const int32_t nMaxHp = 2 * STATLIST_GetMaxLifeFromUnit(pUnit);
    STATLIST_SetUnitStat(pUnit, STAT_MAXHP, nMaxHp, 0);
    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nMaxHp, 0);
    MONSTERUNIQUE_UMod16_Champion(pUnit, nUMod, bUnique);
}

//D2Game.0x6FC6B4B0
void __fastcall MONSTERUNIQUE_UMod39_Berserk(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pGame || !bUnique)
    {
        return;
    }

    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
    const int32_t nNewHp = MONSTERUNIQUE_CalculatePercentage(nMaxHp, -75, 100) + nMaxHp;
    STATLIST_SetUnitStat(pUnit, STAT_MAXHP, nNewHp, 0);
    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);

    int32_t nDamage = 300;
    D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pUnit->pGame->nDifficulty);
    if (pDifficultyLevelsTxtRecord)
    {
        nDamage = 3 * pDifficultyLevelsTxtRecord->dwChampionDmgBonus;
    }
    STATLIST_AddUnitStat(pUnit, STAT_DAMAGEPERCENT, nDamage, 0);
    STATLIST_AddUnitStat(pUnit, STAT_ITEM_TOHIT_PERCENT, nDamage, 0);
}

//D2Game.0x6FC6B5D0
void __fastcall MONSTERUNIQUE_UMod41_AlwaysRun(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pGame)
    {
        EVENT_SetEvent(pUnit->pGame, pUnit, UNITEVENTCALLBACK_MONUMOD, pUnit->pGame->dwGameFrame + 75, 0, 0);
    }
}

//D2Game.0x6FC6B610
void __fastcall MONSTERUNIQUE_UMod8_Resistant(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !bUnique)
    {
        return;
    }

    if (nUMod == 28)
    {
        STATLIST_SetUnitStat(pUnit, STAT_ARMORCLASS, 2 * STATLIST_GetUnitStatUnsigned(pUnit, STAT_ARMORCLASS, 0), 0);
    }

    int32_t nImmunities = 0;
    const int32_t nFireResist = STATLIST_GetUnitStatUnsigned(pUnit, STAT_FIRERESIST, 0);
    const int32_t nLightResist = STATLIST_GetUnitStatUnsigned(pUnit, STAT_LIGHTRESIST, 0);
    const int32_t nColdResist = STATLIST_GetUnitStatUnsigned(pUnit, STAT_COLDRESIST, 0);
    const int32_t nPoisonResist = STATLIST_GetUnitStatUnsigned(pUnit, STAT_POISONRESIST, 0);
    const int32_t nDamageResist = STATLIST_GetUnitStatUnsigned(pUnit, STAT_DAMAGERESIST, 0);
    const int32_t nMagicResist = STATLIST_GetUnitStatUnsigned(pUnit, STAT_MAGICRESIST, 0);

    if (nFireResist >= 100)
    {
        ++nImmunities;
    }

    if (nLightResist >= 100)
    {
        ++nImmunities;
    }

    if (nColdResist >= 100)
    {
        ++nImmunities;
    }

    if (nPoisonResist >= 100)
    {
        ++nImmunities;
    }

    if (nDamageResist >= 100)
    {
        ++nImmunities;
    }

    if (nMagicResist >= 100)
    {
        ++nImmunities;
    }

    if (nImmunities < 2)
    {
        switch (nUMod)
        {
        case 8:
            if (nColdResist < 100)
            {
                const int32_t nNewColdResist = nColdResist + 40;
                STATLIST_SetUnitStat(pUnit, STAT_COLDRESIST, nNewColdResist, 0);
                if (nNewColdResist >= 100)
                {
                    ++nImmunities;
                }
            }

            if (nImmunities < 2)
            {
                if (nFireResist < 100)
                {
                    const int32_t nNewFireResist = nFireResist + 40;
                    STATLIST_SetUnitStat(pUnit, STAT_FIRERESIST, nNewFireResist, 0);
                    if (nNewFireResist >= 100)
                    {
                        ++nImmunities;
                    }
                }

                if (nImmunities < 2)
                {
                    if (nLightResist < 100)
                    {
                        STATLIST_SetUnitStat(pUnit, STAT_LIGHTRESIST, nLightResist + 40, 0);
                    }
                }
            }
            break;

        case 9:
            STATLIST_SetUnitStat(pUnit, STAT_FIRERESIST, nFireResist + 75, 0);
            break;

        case 18:
            STATLIST_SetUnitStat(pUnit, STAT_COLDRESIST, nColdResist + 75, 0);
            break;

        case 17:
            STATLIST_SetUnitStat(pUnit, STAT_LIGHTRESIST, nLightResist + 75, 0);
            break;

        case 23:
            STATLIST_SetUnitStat(pUnit, STAT_POISONRESIST, nPoisonResist + 75, 0);
            break;

        case 25:
            STATLIST_SetUnitStat(pUnit, STAT_MAGICRESIST, nMagicResist + 20, 0);
            break;

        case 27:
            if (nColdResist < 75)
            {
                const int32_t nNewColdResist = nColdResist + 20;
                STATLIST_SetUnitStat(pUnit, STAT_COLDRESIST, nNewColdResist, 0);
                if (nNewColdResist >= 100)
                {
                    ++nImmunities;
                }
            }

            if (nImmunities < 2)
            {
                if (nFireResist < 75)
                {
                    const int32_t nNewFireResist = nFireResist + 20;
                    STATLIST_SetUnitStat(pUnit, STAT_FIRERESIST, nNewFireResist, 0);
                    if (nNewFireResist >= 100)
                    {
                        ++nImmunities;
                    }
                }

                if (nImmunities < 2 && nLightResist < 75)
                {
                    STATLIST_SetUnitStat(pUnit, STAT_LIGHTRESIST, nLightResist + 20, 0);
                }
            }
            break;

        case 28:
            STATLIST_SetUnitStat(pUnit, STAT_DAMAGERESIST, nDamageResist + 50, 0);
            break;
        }
    }
}

//D2Game.0x6FC6B8C0
void __fastcall MONSTERUNIQUE_UMod26_Teleport(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && bUnique)
    {
        D2GAME_SetSkills_6FD14C60(pUnit, SKILL_MONTELEPORT, 1, 1);
        SKILLS_SetSkillMode(SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, SKILL_MONTELEPORT), 4);
        AIUTIL_ToggleAiControlFlag0x20(pUnit, 1);
    }
}

//D2Game.0x6FC6B910
void __fastcall MONSTERUNIQUE_UMod30_AuraEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    constexpr D2AuraModStrc gAuraMods[8] =
    {
        {   0, 0, 1, 6, SKILL_MIGHT },
        {   0, 0, 1, 6, SKILL_HOLYFIRE },
        {   0, 0, 1, 5, SKILL_BLESSEDAIM },
        {   0, 0, 1, 7, SKILL_HOLYFREEZE },
        {   0, 0, 1, 8, SKILL_CONVICTION },
        {   0, 0, 1, 8, SKILL_FANATICISM },
        {  20, 0, 1, 8, SKILL_HOLYSHOCK },
        { 999, 0, 0, 1, SKILL_THORNS },
    };

    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !bUnique)
    {
        return;
    }

    const int32_t nLevel = std::max(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 1u);

    int32_t nMax = 0;
    for (int32_t i = 0; i < 8; ++i)
    {
        if (gAuraMods[i].nMinLevel <= nLevel)
        {
            ++nMax;
        }
    }

    if (!nMax)
    {
        nMax = 1;
    }

    D2SeedStrc seed = {};
    SEED_InitSeed(&seed);
    SEED_InitLowSeed(&seed, MONSTERUNIQUE_GetNameSeed(pUnit));

    const int32_t nIndex = MONSTERUNIQUE_GetBossHcIdx(pUnit) != SUPERUNIQUE_LORD_DE_SEIS ? ITEMS_RollLimitedRandomNumber(&seed, nMax) : 5;
    const int32_t nSkillLevel = D2Clamp(gAuraMods[nIndex].nMultiplier * (nLevel + gAuraMods[nIndex].nLevelOffset) / gAuraMods[nIndex].nDivisor, 1, 99);
    D2GAME_SetSkills_6FD14C60(pUnit, gAuraMods[nIndex].nSkillId, nSkillLevel, 1);
    D2GAME_AssignSkill_6FD13800(pUnit, 0, gAuraMods[nIndex].nSkillId, -1);
}

//D2Game.0x6FC6BA70
void __fastcall MONSTERUNIQUE_UMod5_Strong(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    if (!pGame)
    {
        return;
    }

    int32_t nDamagePercent = 0;
    int32_t nToHitPercent = 0;
    if (bUnique)
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(15);
        nDamagePercent = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(13);
        nToHitPercent = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;
    }
    else
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(14);
        nDamagePercent = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(12);
        nToHitPercent = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;
    }

    D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
    if (pDifficultyLevelsTxtRecord)
    {
        nDamagePercent = nDamagePercent * pDifficultyLevelsTxtRecord->dwChampionDmgBonus / 100;
        nToHitPercent = nToHitPercent * pDifficultyLevelsTxtRecord->dwChampionDmgBonus / 100;
    }

    STATLIST_AddUnitStat(pUnit, STAT_DAMAGEPERCENT, nDamagePercent, 0);
    STATLIST_AddUnitStat(pUnit, STAT_ITEM_TOHIT_PERCENT, nToHitPercent, 0);
}

//D2Game.0x6FC6BB80
void __fastcall MONSTERUNIQUE_UMod6_Fast(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    if (!pMonStatsTxtRecord || pMonStatsTxtRecord->nVelocity <= 0)
    {
        return;
    }

    const int32_t nVelocity = D2Clamp(2048 / pMonStatsTxtRecord->nVelocity - 128, 10, 100);
    STATLIST_AddUnitStat(pUnit, STAT_VELOCITYPERCENT, nVelocity, 0);
}

//D2Game.0x6FC6BC10
void __fastcall MONSTERUNIQUE_UMod9_FireEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    if (!pGame)
    {
        return;
    }

    const uint8_t nDifficulty = D2Clamp(pGame->nDifficulty, 0ui8, 2ui8);
    const int32_t nGameType = pGame->nGameType || pGame->dwGameType;
    const int32_t nLevel = D2Clamp(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 1u, (uint32_t)sgptDataTables->nMonLvlTxtRecordCount - 1);
    D2MonLvlTxt* pMonLvlTxtRecord = &sgptDataTables->pMonLvlTxt[nLevel];
    if (!pMonLvlTxtRecord)
    {
        return;
    }

    const int32_t nDamage = nGameType ? pMonLvlTxtRecord->dwLDM[nDifficulty] : pMonLvlTxtRecord->dwDM[nDifficulty];
    if (bUnique)
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 28);
        int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_FIREMINDAM, nDamage * nPercentage / 100, 0);

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 31);
        nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_FIREMAXDAM, nDamage * nPercentage / 100, 0);
    }
    else
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 16);
        int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_FIREMINDAM, nDamage * nPercentage / 100, 0);

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 19);
        nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_FIREMAXDAM, nDamage * nPercentage / 100, 0);
    }

    MONSTERUNIQUE_UMod8_Resistant(pUnit, nUMod, bUnique);
}

//D2Game.0x6FC6BDD0
void __fastcall MONSTERUNIQUE_UMod17_LightningEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    if (!pGame)
    {
        return;
    }

    const uint8_t nDifficulty = D2Clamp(pGame->nDifficulty, 0ui8, 2ui8);
    const int32_t nGameType = pGame->nGameType || pGame->dwGameType;
    const int32_t nLevel = D2Clamp(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 1u, (uint32_t)sgptDataTables->nMonLvlTxtRecordCount - 1);
    D2MonLvlTxt* pMonLvlTxtRecord = &sgptDataTables->pMonLvlTxt[nLevel];
    if (!pMonLvlTxtRecord)
    {
        return;
    }

    const int32_t nDamage = nGameType ? pMonLvlTxtRecord->dwLDM[nDifficulty] : pMonLvlTxtRecord->dwDM[nDifficulty];
    if (bUnique)
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 28);
        int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_LIGHTMINDAM, nDamage * nPercentage / 100, 0);

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 31);
        nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_LIGHTMAXDAM, nDamage * nPercentage / 100, 0);
    }
    else
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 16);
        int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_LIGHTMINDAM, nDamage * nPercentage / 100, 0);

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 19);
        nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_LIGHTMAXDAM, nDamage * nPercentage / 100, 0);
    }

    MONSTERUNIQUE_UMod8_Resistant(pUnit, nUMod, bUnique);
}

//D2Game.0x6FC6BF90
void __fastcall MONSTERUNIQUE_UMod18_ColdEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    if (!pGame)
    {
        return;
    }

    const uint8_t nDifficulty = D2Clamp(pGame->nDifficulty, 0ui8, 2ui8);
    const int32_t nGameType = pGame->nGameType || pGame->dwGameType;
    const int32_t nLevel = D2Clamp(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 1u, (uint32_t)sgptDataTables->nMonLvlTxtRecordCount - 1);
    D2MonLvlTxt* pMonLvlTxtRecord = &sgptDataTables->pMonLvlTxt[nLevel];
    if (!pMonLvlTxtRecord)
    {
        return;
    }

    const int32_t nDamage = nGameType ? pMonLvlTxtRecord->dwLDM[nDifficulty] : pMonLvlTxtRecord->dwDM[nDifficulty];
    if (bUnique)
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 28);
        int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_COLDMINDAM, nDamage * nPercentage / 100, 0);

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 31);
        nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_COLDMAXDAM, nDamage * nPercentage / 100, 0);
    }
    else
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 16);
        int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_COLDMINDAM, nDamage * nPercentage / 100, 0);

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 19);
        nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_COLDMAXDAM, nDamage * nPercentage / 100, 0);
    }

    STATLIST_AddUnitStat(pUnit, STAT_COLDLENGTH, 5 * nLevel + 100, 0);

    MONSTERUNIQUE_UMod8_Resistant(pUnit, nUMod, bUnique);
}

//D2Game.0x6FC6C160
void __fastcall MONSTERUNIQUE_UMod23_PoisonEnchanted(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    if (!pGame)
    {
        return;
    }

    const uint8_t nDifficulty = D2Clamp(pGame->nDifficulty, 0ui8, 2ui8);
    const int32_t nGameType = pGame->nGameType || pGame->dwGameType;
    const int32_t nLevel = D2Clamp(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 1u, (uint32_t)sgptDataTables->nMonLvlTxtRecordCount - 1);
    D2MonLvlTxt* pMonLvlTxtRecord = &sgptDataTables->pMonLvlTxt[nLevel];
    if (!pMonLvlTxtRecord)
    {
        return;
    }

    const int32_t nDamage = nGameType ? pMonLvlTxtRecord->dwLDM[nDifficulty] : pMonLvlTxtRecord->dwDM[nDifficulty];
    if (bUnique)
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 28);
        int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_POISONMINDAM, nDamage * nPercentage / 100, 0);

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 31);
        nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_POISONMAXDAM, nDamage * nPercentage / 100, 0);
    }
    else
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 16);
        int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_POISONMINDAM, nDamage * nPercentage / 100, 0);

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 19);
        nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_POISONMAXDAM, nDamage * nPercentage / 100, 0);
    }

    STATLIST_AddUnitStat(pUnit, STAT_POISONLENGTH, 2 * (5 * nLevel + 150), 0);

    MONSTERUNIQUE_UMod8_Resistant(pUnit, nUMod, bUnique);
}

//D2Game.0x6FC6C340
void __fastcall MONSTERUNIQUE_UMod25_ManaSteal(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    if (!pGame)
    {
        return;
    }

    const uint8_t nDifficulty = D2Clamp(pGame->nDifficulty, 0ui8, 2ui8);
    const int32_t nGameType = pGame->nGameType || pGame->dwGameType;
    const int32_t nLevel = D2Clamp(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 1u, (uint32_t)sgptDataTables->nMonLvlTxtRecordCount - 1);
    D2MonLvlTxt* pMonLvlTxtRecord = &sgptDataTables->pMonLvlTxt[nLevel];
    if (!pMonLvlTxtRecord)
    {
        return;
    }

    const int32_t nDamage = nGameType ? pMonLvlTxtRecord->dwLDM[nDifficulty] : pMonLvlTxtRecord->dwDM[nDifficulty];
    if (bUnique)
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 28);
        int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_MANADRAINMINDAM, (nDamage * nPercentage / 100) << 8, 0);

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 31);
        nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_MANADRAINMAXDAM, (nDamage * nPercentage / 100) << 8, 0);
    }
    else
    {
        D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 16);
        int32_t nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_MANADRAINMINDAM, (nDamage * nPercentage / 100) << 8, 0);

        pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nDifficulty + 19);
        nPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

        STATLIST_AddUnitStat(pUnit, STAT_MANADRAINMAXDAM, (nDamage * nPercentage / 100) << 8, 0);
    }

    MONSTERUNIQUE_UMod8_Resistant(pUnit, nUMod, bUnique);
}

//D2Game.0x6FC6C4F0
void __fastcall MONSTERUNIQUE_CastAmplifyDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4)
{
    if (!a4 || ((uint8_t)ITEMS_RollRandomNumber(&pUnit->pSeed) & 3u) < 1)
    {
        return;
    }

    const int32_t nSkillLevel = std::max(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0) / 5 + 1, 1u);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(SKILL_AMPLIFYDAMAGE);
    if (!pSkillsTxtRecord)
    {
        return;
    }

    const int32_t nRange = D2Clamp(SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, SKILL_AMPLIFYDAMAGE, nSkillLevel), 1, 40);
    sub_6FD14770(pGame, 3, pUnit, nSkillLevel, nRange, MONSTERUNIQUE_CurseCallback_ApplyAmplifyDamage);
}

//D2Game.0x6FC6C5B0
void __fastcall MONSTERUNIQUE_CurseCallback_ApplyAmplifyDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillLevel)
{
    if (!sub_6FCBD900(pGame, pUnit, pTarget))
    {
        return;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(SKILL_AMPLIFYDAMAGE);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wAuraStat[0] < -1 || pSkillsTxtRecord->wAuraStat[0] >= sgptDataTables->nItemStatCostTxtRecordCount)
    {
        return;
    }

    if (pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return;
    }

    D2CurseStrc curse = {};

    curse.pTarget = pTarget;
    curse.pUnit = pUnit;
    curse.nSkill = SKILL_AMPLIFYDAMAGE;
    curse.nSkillLevel = nSkillLevel;
    curse.nDuration = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, SKILL_AMPLIFYDAMAGE, nSkillLevel);
    curse.nStat = pSkillsTxtRecord->wAuraStat[0];
    curse.nStatValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[0], SKILL_AMPLIFYDAMAGE, nSkillLevel);
    curse.nState = pSkillsTxtRecord->wAuraTargetState;
    curse.pStateFunc = nullptr;

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    if (!pStatList)
    {
        return;
    }

    for (int32_t i = 1; i < 6; ++i)
    {
        if (pSkillsTxtRecord->wAuraStat[i] >= 0 && pSkillsTxtRecord->wAuraStat[i] < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            const int32_t nValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], SKILL_AMPLIFYDAMAGE, nSkillLevel);
            if (nValue)
            {
                STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxtRecord->wAuraStat[i], nValue, 0);
            }
        }
    }
}

//D2Game.0x6FC6C710
void __fastcall MONSTERUNIQUE_FireEnchantedModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique)
{
    if (bUnique && (!pBoss || pBoss->dwAnimMode == MONMODE_DEATH))
    {
        EVENT_SetEvent(pGame, pBoss, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + 4, 0, 0);
    }
}

//D2Game.0x6FC6C740
void __fastcall MONSTERUNIQUE_CastCorpseExplode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    const int32_t nX = CLIENTS_GetUnitX(pUnit);
    const int32_t nY = CLIENTS_GetUnitY(pUnit);

    D2UnitStrc* pMissile = sub_6FD11420(pGame, MISSILE_MONSTERCORPSEEXPLODE, pUnit, 0, 1, 0, 0, nX, nY, 1);
    if (!pMissile)
    {
        return;
    }

    D2MonStatsInitStrc monStatsInit = {};
    DATATBLS_CalculateMonsterStatsByLevel(pUnit->dwClassId, pGame->nGameType || pGame->dwGameType, pGame->nDifficulty, STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 1, &monStatsInit);

    D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
    if (!pDifficultyLevelsTxtRecord)
    {
        return;
    }

    const int32_t nMaxDamage = MONSTERUNIQUE_CalculatePercentage(monStatsInit.nMaxHP, pDifficultyLevelsTxtRecord->dwMonsterCEDmgPercent, 100);
    const int32_t nMinDamage = MONSTERUNIQUE_CalculatePercentage(nMaxDamage, 60, 100);
    const int32_t nDamage = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDamage - nMinDamage);

    D2DamageStrc damage = {};
    damage.dwPhysDamage = (nMinDamage + nDamage) << 6;
    damage.dwFireDamage = damage.dwPhysDamage;
    SUNITDMG_SetMissileDamageFlagsForNearbyUnits(pGame, pMissile, nX, nY, pGame->nDifficulty + 4, &damage, 0, 0, nullptr, 0x581u);
}

//D2Game.0x6FC6C9E0
void __fastcall MONSTERUNIQUE_CastCorpseExplode2(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    const int32_t nX = CLIENTS_GetUnitX(pUnit);
    const int32_t nY = CLIENTS_GetUnitY(pUnit);

    D2UnitStrc* pMissile = sub_6FD11420(pGame, MISSILE_MONSTERCORPSEEXPLODE, pUnit, 0, 1, 0, 0, nX, nY, 1);
    if (!pMissile)
    {
        return;
    }

    D2DamageStrc damage = {};
    damage.dwPhysDamage = 0x6400u;
    damage.dwFireDamage = 0x6400u;
    SUNITDMG_SetMissileDamageFlagsForNearbyUnits(pGame, pMissile, nX, nY, 6, &damage, 0, 0, nullptr, 0x583);
}

//D2Game.0x6FC6CAB0
void __fastcall MONSTERUNIQUE_CastNova(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!bUnique)
    {
        return;
    }

    int32_t nSkillLevel = 1;
    D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
    if (pOwner)
    {
        D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pOwner, SKILL_SHADOWWARRIOR);
        if (pSkill)
        {
            nSkillLevel = std::min(SKILLS_GetSkillLevel(pOwner, pSkill, 1) / 2 + 1, 15);
        }
    }

    if (UNITS_GetCurrentLifePercentage(pUnit) > 10)
    {
        nSkillLevel = 1;
    }

    sub_6FD14170(pGame, pUnit, pUnit, MISSILE_NOVA, 0, nSkillLevel, DATATBLS_GetMissileVelocityFromMissilesTxt(MISSILE_NOVA, 0));
}

//D2Game.0x6FC6CB40
void __fastcall MONSTERUNIQUE_CastLightUniqueMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!bUnique)
    {
        return;
    }

    int32_t nLastFrame = 0;
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        nLastFrame = pUnit->pMonsterData->dwDurielFlag;
    }

    const int32_t nFrameDiff = std::abs(pGame->dwGameFrame - nLastFrame);
    if (nFrameDiff < 10)
    {
        if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
        {
            pUnit->pMonsterData->nLastAnimMode &= 0xFEu;
        }
        return;
    }

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        pUnit->pMonsterData->dwDurielFlag = pGame->dwGameFrame;
        pUnit->pMonsterData->nLastAnimMode |= 1u;
    }

    D2MissileStrc missileParams = {};

    missileParams.nSkillLevel = std::max(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0) / 2, 1u);
    missileParams.dwFlags = 0x21;
    missileParams.pOwner = pUnit;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nMissile = MISSILE_LIGHTUNIQUE;
    missileParams.pInitFunc = SKILLS_MissileInit_ChargedBolt;

    constexpr int32_t nXOffsets[] = { 0, 1, 0, -1 };
    constexpr int32_t nYOffsets[] = { -1, 0, 1, 0 };

    for (int32_t i = 0; i < 4; ++i)
    {
        const int32_t nX = missileParams.nX + nXOffsets[i];
        const int32_t nY = missileParams.nY + nYOffsets[i];

        for (int32_t j = 0; j < 2; ++j)
        {
            missileParams.pInitArgs = j;
            missileParams.nTargetX = nX;
            missileParams.nTargetY = nY;
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
        }
    }
}

//D2Game.0x6FC6CD30
void __fastcall sub_6FC6CD30(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (bUnique && (!pUnit || pUnit->dwAnimMode != MONMODE_GETHIT))
    {
        MONSTERUNIQUE_CastLightUniqueMissile(pGame, pUnit, nUMod, bUnique);
    }
}

//D2Game.0x6FC6CD60
void __fastcall MONSTERUNIQUE_CastColdUniqueMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!bUnique)
    {
        return;
    }

    const int32_t nLevel = std::max(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0) / 2, 1u);
    sub_6FD14170(pGame, pUnit, pUnit, MISSILE_COLDUNIQUE, 0, nLevel, DATATBLS_GetMissileVelocityFromMissilesTxt(MISSILE_FROSTNOVA, 0));
}

//D2Game.0x6FC6CDB0
void __fastcall MONSTERUNIQUE_CastCorpsePoisonCloud(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    const int32_t nLevel = std::max(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 1u);
    sub_6FD11420(pGame, MISSILE_CORPSEPOISONCLOUD, pUnit, 0, nLevel, 0, 0, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 1);
}

//D2Game.0x6FC6CE50
void __fastcall MONSTERUNIQUE_KillMinions(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (pUnit && pUnit->dwAnimMode != MONMODE_DEATH)
    {
        return;
    }

    D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
    if (!pOwner)
    {
        return;
    }

    AIGENERAL_ExecuteCallbackOnMinions(pUnit, pGame, nullptr, MONSTERUNIQUE_MinionCallback_KillMinion);
    AIGENERAL_SetOwnerData(pGame, pOwner, -1, 1, 0, 0);
    if (pOwner == pUnit || pOwner->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2ModeChangeStrc modeChange = {};
    MONSTERMODE_GetModeChangeInfo(pOwner, MONMODE_DEATH, &modeChange);
    D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
}

//D2Game.0x6FC6CEC0
void __fastcall MONSTERUNIQUE_MinionCallback_KillMinion(D2UnitStrc* pUnit, void* pGameArg, void* pUnused)
{
    D2GameStrc* pGame = (D2GameStrc*)pGameArg;
    AIGENERAL_FreeMinionList(pUnit);
    AIGENERAL_SetOwnerData(pGame, pUnit, -1, 1, 0, 0);

    if (pUnit && pUnit->dwAnimMode != MONMODE_DEATH)
    {
        D2ModeChangeStrc modeChange = {};

        MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_DEATH, &modeChange);
        D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
    }
}

//D2Game.0x6FC6CF10
void __fastcall sub_6FC6CF10(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    int32_t nMinDamage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_MINDAMAGE, 0);
    int32_t nMaxDamage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_MAXDAMAGE, 0);
    if (nMaxDamage > 0 && (!pUnit || pUnit->dwClassId != MONSTER_ROGUEHIRE))
    {
        nMinDamage += STATLIST_GetUnitStatUnsigned(pUnit, STAT_SECONDARY_MINDAMAGE, 0) - 1;
        nMaxDamage += STATLIST_GetUnitStatUnsigned(pUnit, STAT_SECONDARY_MAXDAMAGE, 0) - 1;

        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndFlag(pUnit, 1);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_MINDAMAGE, std::max(nMinDamage, 0), 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_MAXDAMAGE, std::max(nMaxDamage, 0), 0);
    }
}

//D2Game.0x6FC6CF90
void __fastcall sub_6FC6CF90(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MISSILE)
    {
        return;
    }

    int32_t nMinDamage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_MINDAMAGE, 0);
    int32_t nMaxDamage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_MAXDAMAGE, 0);
    if (nMaxDamage <= 0)
    {
        return;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pUnit);
    if (!pOwner || pOwner->dwClassId != MONSTER_ROGUEHIRE)
    {
        return;
    }

    nMinDamage += (STATLIST_GetUnitStatUnsigned(pOwner, STAT_SECONDARY_MINDAMAGE, 0) << 8) - 256;
    nMaxDamage += (STATLIST_GetUnitStatUnsigned(pOwner, STAT_SECONDARY_MAXDAMAGE, 0) << 8) - 256;

    STATLIST_SetUnitStat(pUnit, STAT_MINDAMAGE, std::max(nMinDamage, 0), 0);
    STATLIST_SetUnitStat(pUnit, STAT_MAXDAMAGE, std::max(nMaxDamage, 0), 0);
}

//D2Game.0x6FC6D030
void __fastcall MONSTERUNIQUE_ScarabModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique)
{
    if (!pBoss || pBoss->dwAnimMode == MONMODE_GETHIT || pBoss->dwAnimMode == MONMODE_DEATH)
    {
        EVENT_SetEvent(pGame, pBoss, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + 2, 0, 0);
    }
}

//D2Game.0x6FC6D060
void __fastcall MONSTERUNIQUE_CastBugLightningMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    D2MissileStrc missileParams = {};

    missileParams.dwFlags = 0x21;
    missileParams.pOwner = pUnit;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nMissile = MISSILE_BUGLIGHTNING;
    missileParams.nSkillLevel = STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0);
    missileParams.pInitFunc = SKILLS_MissileInit_ChargedBolt;

    constexpr int32_t nXOffsets[] = { 0, 1, 0, -1 };
    constexpr int32_t nYOffsets[] = { -1, 0, 1, 0 };

    for (int32_t i = 0; i < 4; ++i)
    {
        const int32_t nX = missileParams.nX + nXOffsets[i];
        const int32_t nY = missileParams.nY + nYOffsets[i];

        for (int32_t j = 0; j < 2; ++j)
        {
            missileParams.pInitArgs = j;
            missileParams.nTargetX = nX;
            missileParams.nTargetY = nY;
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
        }
    }
}

//D2Game.0x6FC6D1C0
void __fastcall MONSTERUNIQUE_ApplyElementalDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    constexpr int32_t dword_6FD28C38[5][3] =
    {
        { STAT_FIREMINDAM, STAT_FIREMAXDAM, -1 },
        { STAT_LIGHTMINDAM, STAT_LIGHTMAXDAM, -1 },
        { STAT_MAGICMINDAM, STAT_MAGICMAXDAM, -1 },
        { STAT_COLDMINDAM, STAT_COLDMAXDAM, STAT_COLDLENGTH },
        { STAT_POISONMINDAM, STAT_POISONMAXDAM, STAT_POISONLENGTH },
    };

    if (!bUnique)
    {
        return;
    }

    const uint32_t nRand = ITEMS_RollRandomNumber(&pUnit->pSeed) % 5;
    const int32_t nMinDamageStatId = dword_6FD28C38[nRand][0];
    const int32_t nMaxDamageStatId = dword_6FD28C38[nRand][1];
    const int32_t nLengthStatId = dword_6FD28C38[nRand][2];

    if (pUnit && pUnit->dwUnitType == UNIT_MISSILE)
    {
        D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pUnit->dwClassId);
        if (!pMissilesTxtRecord || pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_NOUNIQUEMOD])
        {
            return;
        }
    }

    const uint8_t nDifficulty = D2Clamp(pGame->nDifficulty, 0ui8, 2ui8);
    const int32_t nGameType = pGame->nGameType || pGame->dwGameType;
    const int32_t nLevel = D2Clamp(STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 1u, (uint32_t)sgptDataTables->nMonLvlTxtRecordCount - 1);
    D2MonLvlTxt* pMonLvlTxtRecord = &sgptDataTables->pMonLvlTxt[nLevel];
    if (!pMonLvlTxtRecord)
    {
        return;
    }

    const int32_t nDamage = nGameType ? pMonLvlTxtRecord->dwLDM[nDifficulty] : pMonLvlTxtRecord->dwDM[nDifficulty];

    D2MonUModTxt* pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(28);
    const int32_t nMinPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

    pMonUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(31);
    const int32_t nMaxPercentage = pMonUModTxtRecord ? pMonUModTxtRecord->dwConstants : 0;

    D2StatListStrc* pStatList = nullptr;

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        pStatList = STATLIST_GetStatListFromUnitAndFlag(pUnit, 1);
        STATLIST_SetStatIfListIsValid(pStatList, nMinDamageStatId, nDamage * nMinPercentage / 100, 0);
        STATLIST_SetStatIfListIsValid(pStatList, nMaxDamageStatId, nDamage * nMaxPercentage / 100, 0);
    }
    else
    {
        STATLIST_SetUnitStat(pUnit, nMinDamageStatId, nDamage * nMinPercentage / 100, 0);
        STATLIST_SetUnitStat(pUnit, nMaxDamageStatId, nDamage * nMaxPercentage / 100, 0);
    }

    if (nLengthStatId == -1)
    {
        return;
    }

    const int32_t nLength = STATLIST_GetUnitStatUnsigned(pUnit, nLengthStatId, 0) + 40;
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        STATLIST_SetStatIfListIsValid(pStatList, nLengthStatId, nLength, 0);
    }
    else
    {
        STATLIST_SetUnitStat(pUnit, nLengthStatId, nLength, 0);
    }
}

//D2Game.0x6FC6D410
D2MonUModTxt* __fastcall MONSTERUNIQUE_GetMonUModTxtRecord(uint32_t nUMod)
{
    if (nUMod < sgptDataTables->nMonUModTxtRecordCount)
    {
        return &sgptDataTables->pMonUModTxt[nUMod];
    }

    return nullptr;
}

//D2Game.0x6FC6D440
void __fastcall sub_6FC6D440(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MISSILE)
    {
        MONSTERUNIQUE_ApplyElementalDamage(pGame, pUnit, nUMod, bUnique);
    }
}

//D2Game.0x6FC6D690
void __fastcall MONSTERUNIQUE_StealBeltItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!bUnique)
    {
        return;
    }

    if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < 30)
    {
        return;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || pTarget->dwUnitType != UNIT_PLAYER)
    {
        return;
    }

    for (int32_t i = 0; i < 16; ++i)
    {
        D2UnitStrc* pBeltItem = INVENTORY_GetItemFromBeltSlot(pTarget->pInventory, i);
        if (pBeltItem)
        {
            if (pBeltItem->dwUnitType == UNIT_ITEM && pBeltItem->dwAnimMode == IMODE_INBELT && !INVENTORY_GetCursorItem(pTarget->pInventory))
            {
                D2UnitStrc* pDupeItem = ITEMS_Duplicate(pGame, pBeltItem, pTarget, 1);
                D2CoordStrc pCoord = {};
                UNITS_GetCoords(pTarget, &pCoord);

                D2CoordStrc pReturnCoords = {};
                D2RoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pTarget), &pCoord, &pReturnCoords, 1);
                D2_ASSERT(pRoom);

                ITEMS_SetItemFlag(pDupeItem, IFLAG_INSTORE, 1);
                D2GAME_DropItem_6FC52260(pGame, pTarget, pDupeItem, pRoom, pReturnCoords.nX, pReturnCoords.nY);
                sub_6FC47380(pGame, pTarget, pBeltItem);
                sub_6FC49140(pGame, pTarget, CLIENTS_GetUnitX(pBeltItem));
            }
            return;
        }
    }
}

//D2Game.0x6FC6D800
void __fastcall MONSTERUNIQUE_QuestCompleteModeChange(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit)
    {
        return;
    }

    switch (pUnit->dwClassId)
    {
    case MONSTER_ANDARIEL:
        if (pUnit->dwAnimMode == MONMODE_DEATH)
        {
            QUESTSFX_Andariel(pGame, pUnit);
        }
        break;
    case MONSTER_RADAMENT:
        if (pUnit->dwAnimMode == MONMODE_DEATH)
        {
            QUESTSFX_SpawnRadamentDeathMissile(pGame, pUnit);
        }
        break;
    case MONSTER_MEPHISTO:
        if (pUnit->dwAnimMode == MONMODE_DEATH)
        {
            QUESTSFX_SpawnMephistoDeathControlMissile(pGame, pUnit);
        }
        break;
    case MONSTER_IZUAL:
        if (pUnit->dwAnimMode == MONMODE_DEATH)
        {
            QUESTSFX_Izual(pGame, pUnit);
        }
        break;
    case MONSTER_BLOODRAVEN:
        if (pUnit->dwAnimMode == MONMODE_DEATH)
        {
            QUESTSFX_Bloodraven(pGame, pUnit);
        }
        break;
    case MONSTER_OVERSEER1:
        if (pUnit->dwAnimMode == MONMODE_DEATH)
        {
            QUESTSFX_ShenkTheOverseer(pGame, pUnit);
        }
        break;
    //case MONSTER_ANCIENTBARB1:
    //case MONSTER_ANCIENTBARB2:
    //case MONSTER_ANCIENTBARB3:
    //    D2Game_10061_Return();
    //    break;
    }
}

//D2Game.0x6FC6D8B0
void __fastcall MONSTERUNIQUE_CastQueenPoisonCloudMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x21;
    missileParams.pOwner = pUnit;
    missileParams.nX = CLIENTS_GetUnitX(pUnit);
    missileParams.nY = CLIENTS_GetUnitY(pUnit);
    missileParams.nMissile = MISSILE_QUEENPOISONCLOUD;
    missileParams.nSkillLevel = STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0);
    missileParams.pInitFunc = SKILLS_MissileInit_ChargedBolt;

    constexpr int32_t nXOffsets[] = { 0, 1, 0, -1 };
    constexpr int32_t nYOffsets[] = { -1, 0, 1, 0 };

    for (int32_t j = 0; j < 4; ++j)
    {
        const int32_t nTargetX = CLIENTS_GetUnitX(pUnit) + nXOffsets[j];
        const int32_t nTargetY = CLIENTS_GetUnitY(pUnit) + nYOffsets[j];

        for (int32_t i = 0; i < 2; ++i)
        {
            missileParams.pInitArgs = i;
            missileParams.nTargetX = nTargetX;
            missileParams.nTargetY = nTargetY;
            MISSILES_CreateMissileFromParams(pGame, &missileParams);
        }
    }
}

//D2Game.0x6FC6DA40
void __fastcall sub_6FC6DA40(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!bUnique || !pUnit || pUnit->dwUnitType != UNIT_MISSILE)
    {
        return;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pUnit);
    if (!pOwner)
    {
        return;
    }

    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(pUnit->dwClassId);
    if (!pMissilesTxtRecord || pMissilesTxtRecord->dwMissileFlags & gdwBitMasks[MISSILESFLAGINDEX_NOMULTISHOT])
    {
        return;
    }

    if (pOwner->dwUnitType == UNIT_MONSTER && pOwner->pMonsterData && pOwner->pMonsterData->nTypeFlag & 0x80)
    {
        return;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pOwner);

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

    D2SeedStrc seed = {};
    SEED_InitSeed(&seed);
    SEED_InitLowSeed(&seed, 'SEIS');
    seed.nHighSeed = pOwner->dwUnitId;

    int32_t nXDiff = D2signum(CLIENTS_GetUnitX(pOwner) - nX);
    int32_t nYDiff = D2signum(CLIENTS_GetUnitY(pOwner) - nY);
    if (pUnit->dwClassId >= MISSILE_MUMMY1 && pUnit->dwClassId <= MISSILE_MUMMY4)
    {
        nYDiff = 0;
        nXDiff = 0;
    }

    if (pOwner->dwUnitType == UNIT_MONSTER && pOwner->pMonsterData)
    {
        pOwner->pMonsterData->nTypeFlag |= 0x80u;
    }

    const int32_t nMissileLevel = MISSILE_GetLevel(pUnit);
    const int32_t nSkillId = MISSILE_GetSkill(pUnit);
    sub_6FD11420(pGame, pUnit->dwClassId, pOwner, nSkillId, nMissileLevel, 0, 0, nX - nYDiff, nY + nXDiff, 0);
    sub_6FD11420(pGame, pUnit->dwClassId, pOwner, nSkillId, nMissileLevel, 0, 0, nX + nYDiff, nY - nXDiff, 0);

    if (pOwner->dwUnitType == UNIT_MONSTER && pOwner->pMonsterData)
    {
        pOwner->pMonsterData->nTypeFlag &= 0x7Fu;
    }
}

//D2Game.0x6FC6DCB0
void __fastcall MONSTERUNIQUE_LightningEnchantedModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique)
{
    if (bUnique && pBoss && pBoss->dwAnimMode == MONMODE_GETHIT)
    {
        EVENT_SetEvent(pGame, pBoss, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + 2, 0, 0);
    }
}

//D2Game.0x6FC6DCE0
void __fastcall MONSTERUNIQUE_ColdEnchantedModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique)
{
    if ((bUnique || nUMod != 18) && (!pBoss || pBoss->dwAnimMode == MONMODE_DEATH))
    {
        EVENT_SetEvent(pGame, pBoss, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + 4, 0, 0);
    }
}

//D2Game.0x6FC6DD20
void __fastcall sub_6FC6DD20(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit)
    {
        return;
    }

    if (pUnit->dwAnimMode != MONMODE_DEATH || STATLIST_GetUnitAlignment(pUnit))
    {
        return;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_MONUMOD, 0);

    if ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < pMonStatsTxtRecord->wAiParam[7][pGame->nDifficulty])
    {
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + 10 * pMonStatsTxtRecord->wAiParam[0][pGame->nDifficulty] + 1, 0, 0);
    }
}

//D2Game.0x6FC6DDE0
void __fastcall sub_6FC6DDE0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit)
    {
        return;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, 0);

    if (!SUNIT_IsDead(pUnit) || STATES_CheckStateMaskUdeadOnUnit(pUnit))
    {
        return;
    }

    if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData && pUnit->pMonsterData->nTypeFlag & 8)
    {
        return;
    }

    if (STATLIST_GetUnitAlignment(pUnit) == 2)
    {
        return;
    }

    if (COLLISION_CheckMaskWithPattern(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 1, 0x3C01u))
    {
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + 10 * pMonStatsTxtRecord->wAiParam[0][pGame->nDifficulty] + 1, 0, 0);
    }
    else if (AIGENERAL_GetAiControlParam(pUnit, 1) < 2 && D2COMMON_11017_CheckUnitIfConsumeable(pUnit, 0))
    {
        if (!STATLIST_GetUnitAlignment(pUnit))
        {
            AITACTICS_UseSkill(pGame, pUnit, MONMODE_SKILL1, SKILL_SELF_RESURRECT, nullptr, 0, 0);
            pUnit->dwFlags &= 0xFBFDFFFFu;
            const int32_t nParam = AIGENERAL_GetAiControlParam(pUnit, 1);
            AIGENERAL_SetAiControlParam(pUnit, 1, nParam + 1);
            D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, 0);
            EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, pGame->dwGameFrame + 51, 0, 0);
        }
    }
}

//D2Game.0x6FC6DFA0
void __fastcall MONSTERUNIQUE_ApplyShatterState(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwAnimMode == MONMODE_DEATH)
    {
        STATES_ToggleState(pUnit, STATE_SHATTER, 1);
    }
}

//D2Game.0x6FC6DFC0
void __fastcall sub_6FC6DFC0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (SUNIT_IsDead(pUnit))
    {
        return;
    }

    if (STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
    {
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + 3, 0, 0);
    }
    else
    {
        D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
        if (!pOwner || pOwner->dwUnitType != UNIT_PLAYER)
        {
            D2ModeChangeStrc modeChange = {};
            MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_DEATH, &modeChange);
            D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
        }
        else
        {
            sub_6FC7D260(pGame, pOwner, pUnit ? pUnit->dwUnitId : -1, 1);
        }
    }
}

//D2Game.0x6FC6E070
void __fastcall MONSTERUNIQUE_SetTrapDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    int32_t nLevel = 2;
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit));
    if (nLevelId)
    {
        nLevel = std::max(DATATBLS_GetMonsterLevelInArea(nLevelId, pGame->nDifficulty, pGame->bExpansion), 1);
    }

    const int32_t nToHit = std::min(nLevel + 50, 90);
    STATLIST_SetUnitStat(pUnit, STAT_LEVEL, nLevel, 0);
    STATLIST_SetUnitStat(pUnit, STAT_TOHIT, nToHit, 0);

    D2_ASSERT(pUnit && pUnit->dwUnitType == UNIT_MONSTER);

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndFlag(pUnit, 1);
    switch (pUnit->dwClassId)
    {
    case MONSTER_TRAP_FIREBOLT:
    {
        STATLIST_SetStatIfListIsValid(pStatList, STAT_MINDAMAGE, 0, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_MAXDAMAGE, 0, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_FIREMINDAM, nLevel >> 1, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_FIREMAXDAM, 3 * nLevel >> 1, 0);
        break;
    }
    case MONSTER_TRAP_HORZMISSILE:
    case MONSTER_TRAP_VERTMISSILE:
    {
        const int32_t nMinDamage = std::min((nLevel >> 1) + 1, nLevel);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_MINDAMAGE, nMinDamage, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_MAXDAMAGE, nLevel, 0);
        break;
    }
    case MONSTER_TRAP_POISONCLOUD:
    {
        STATLIST_SetStatIfListIsValid(pStatList, STAT_MINDAMAGE, 0, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_MAXDAMAGE, 0, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONMINDAM, nLevel, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONMAXDAM, 2 * nLevel, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONLENGTH, 2 * nLevel, 0);
        break;
    }
    case MONSTER_TRAP_LIGHTNING:
    case MONSTER_TRAP_NOVA:
    {
        STATLIST_SetStatIfListIsValid(pStatList, STAT_MINDAMAGE, 0, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_MAXDAMAGE, 0, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_LIGHTMINDAM, nLevel >> 1, 0);
        STATLIST_SetStatIfListIsValid(pStatList, STAT_LIGHTMAXDAM, 3 * nLevel >> 1, 0);
        break;
    }
    default:
        return;
    }
}

//D2Game.0x6FC6E240
void __fastcall sub_6FC6E240(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit)
    {
        return;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    if (!pMonStatsTxtRecord)
    {
        return;
    }

    D2DamageStrc damage = {};
    damage.dwFireDamage = (ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, pMonStatsTxtRecord->nElMaxD[0][0] - pMonStatsTxtRecord->nElMinD[0][0]) + pMonStatsTxtRecord->nElMinD[0][0]) << 8;

    SUNITDMG_SetMissileDamageFlagsForNearbyUnits(pGame, pUnit, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), pMonStatsTxtRecord->wAiParam[2][pGame->nDifficulty] * STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), &damage, 0, 0, nullptr, 0);
}

//D2Game.0x6FC6E390
void __fastcall MONSTERUNIQUE_SuicideModeChange(D2GameStrc* pGame, D2UnitStrc* pBoss, int32_t nUMod, int32_t bUnique)
{
    if (!pBoss || pBoss->dwAnimMode == MONMODE_DEATH || pBoss->dwAnimMode == MONMODE_DEAD)
    {
        EVENT_SetEvent(pGame, pBoss, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + 4, 0, 0);
        return;
    }

    if (pBoss->dwAnimMode == MONMODE_GETHIT)
    {
        D2ModeChangeStrc modeChange = {};
        MONSTERMODE_GetModeChangeInfo(pBoss, MONMODE_DEATH, &modeChange);
        D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
        EVENT_SetEvent(pGame, pBoss, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + 4, 0, 0);
    }
}

//D2Game.0x6FC6E410
void __fastcall MONSTERUNIQUE_CastSuicideExplodeMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit)
    {
        return;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);

    int32_t nGameType = 0;
    if (pGame->nGameType || pGame->dwGameType)
    {
        nGameType = 1;
    }

    D2MonStatsInitStrc monStatsInit = {};
    DATATBLS_CalculateMonsterStatsByLevel(pUnit->dwClassId, nGameType, pGame->nDifficulty, STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0), 8, &monStatsInit);

    D2DamageStrc damage = {};

    damage.wResultFlags |= 8u;

    const int32_t nDamage = (ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, monStatsInit.nA1MaxD - monStatsInit.nA1MinD) + monStatsInit.nA1MinD) << 8;
    damage.dwPhysDamage = nDamage;

    const int32_t nX = CLIENTS_GetUnitX(pUnit);
    const int32_t nY = CLIENTS_GetUnitY(pUnit);

    const int32_t nColdLength = pMonStatsTxtRecord->wAiParam[0][pGame->nDifficulty];
    int32_t nMissileId = 0;
    if (nColdLength == 1)
    {
        damage.dwFireDamage = nDamage;

        nMissileId = MISSILE_SUICIDEFIREEXPLODE;
    }
    else if (nColdLength < 2)
    {
        nMissileId = MISSILE_SUICIDECORPSEEXPLODE;
    }
    else
    {
        damage.dwColdLen = nColdLength;
        damage.dwColdDamage = nDamage;
        nMissileId = MISSILE_SUICIDEICEEXPLODE;
    }

    D2UnitStrc* pMissile = sub_6FD11420(pGame, nMissileId, pUnit, 0, 1, 0, 0, nX, nY, 1);
    if (pMissile)
    {
        SUNITDMG_SetMissileDamageFlagsForNearbyUnits(pGame, pMissile, nX, nY, pMonStatsTxtRecord->wAiParam[3][pGame->nDifficulty], &damage, 0, 0, nullptr, 0x581);
    }
}

//D2Game.0x6FC6E700
void __fastcall MONSTERUNIQUE_CreatePainWorm(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwAnimMode == MONMODE_DEATH)
    {
        sub_6FC6A150(pGame, pUnit, MONSTER_PAINWORM1, MONMODE_NEUTRAL, 1, 0);
        STATES_ToggleState(pUnit, STATE_CORPSE_NODRAW, 1);
    }
}

//D2Game.0x6FC6E730
void __fastcall sub_6FC6E730(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (SUNIT_IsDead(pUnit))
    {
        return;
    }

    MONSTER_UpdateAiCallbackEvent(pGame, pUnit);
    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + 75, 0, 0);
}

//D2Game.0x6FC6E770
void __fastcall sub_6FC6E770(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    sub_6FC6E780(pGame, pUnit, 0, 0);
}

//D2Game.0x6FC6E780
void __fastcall sub_6FC6E780(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pMissile, int32_t nIndex)
{
    constexpr D2UnkUModStrc stru_6FD2F0C8[43] =
    {
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    MONSTERUNIQUE_CastAmplifyDamage,    nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_FireEnchantedModeChange,      MONSTERUNIQUE_CastCorpseExplode,            nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_ColdEnchantedModeChange,      MONSTERUNIQUE_CastCorpsePoisonCloud,        nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { MONSTERUNIQUE_SetTrapDamage,                  nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_KillMinions,                  nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_LightningEnchantedModeChange, MONSTERUNIQUE_CastLightUniqueMissile,       nullptr,                            sub_6FC6CD30,   nullptr },
        { nullptr,                                      MONSTERUNIQUE_ColdEnchantedModeChange,      MONSTERUNIQUE_CastColdUniqueMissile,        nullptr,                            nullptr,        nullptr },
        { sub_6FC6CF10,                                 nullptr,                                    nullptr,                                    nullptr,                            nullptr,        sub_6FC6CF90 },
        { nullptr,                                      MONSTERUNIQUE_ScarabModeChange,             MONSTERUNIQUE_CastBugLightningMissile,      nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    sub_6FC6DFC0,                               nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_QuestCompleteModeChange,      nullptr,                                    nullptr,                            nullptr,        nullptr },
        { MONSTERUNIQUE_CastQueenPoisonCloudMissile,    nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    MONSTERUNIQUE_StealBeltItem,        nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { MONSTERUNIQUE_ApplyElementalDamage,           nullptr,                                    nullptr,                                    nullptr,                            nullptr,        sub_6FC6D440 },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        sub_6FC6DA40 },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_ColdEnchantedModeChange,      MONSTERUNIQUE_CastCorpseExplode2,           nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_ColdEnchantedModeChange,      sub_6FC6E240,                               nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_SuicideModeChange,            MONSTERUNIQUE_CastSuicideExplodeMissile,    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      sub_6FC6DD20,                               sub_6FC6DDE0,                               nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_ApplyShatterState,            nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_CreatePainWorm,               nullptr,                                    nullptr,                            nullptr,        nullptr },
        { nullptr,                                      nullptr,                                    sub_6FC6E730,                               nullptr,                            nullptr,        nullptr },
        { nullptr,                                      MONSTERUNIQUE_ColdEnchantedModeChange,      MONSTERUNIQUE_CastNova,                     nullptr,                            nullptr,        nullptr },
    };

    int32_t bUnique = 0;
    if (pUnit && pUnit->dwUnitType == 1 && pUnit->pMonsterData)
    {
        bUnique = (pUnit->pMonsterData->nTypeFlag & MONTYPEFLAG_UNIQUE) != 0;
    }

    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
    {
        return;
    }

    const uint8_t* pUMods = pUnit->pMonsterData->nMonUmod;
    if (!pUMods || !*pUMods)
    {
        return;
    }

    if (nIndex == 5)
    {
        for (int32_t i = 0; i < 9; ++i)
        {
            MonUModFunc pUModFunc = stru_6FD2F0C8[pUMods[i]].unk0x00[nIndex];
            if (pUModFunc)
            {
                pUModFunc(pGame, pMissile, pUMods[i], bUnique);
            }
        }
    }
    else
    {
        for (int32_t i = 0; i < 9; ++i)
        {
            MonUModFunc pUModFunc = stru_6FD2F0C8[pUMods[i]].unk0x00[nIndex];
            if (pUModFunc)
            {
                pUModFunc(pGame, pUnit, pUMods[i], bUnique);
            }
        }
    }
}

//D2Game.0x6FC6E860
void __fastcall sub_6FC6E860(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    sub_6FC6E780(pGame, pUnit, 0, 1);
}

//D2Game.0x6FC6E870
void __fastcall D2GAME_MONSTERS_AiFunction08_6FC6E870(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    sub_6FC6E780(pGame, pUnit, 0, 2);
}

//D2Game.0x6FC6E890
void __fastcall sub_6FC6E890(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    sub_6FC6E780(pGame, pUnit, 0, 3);
}

//D2Game.0x6FC6E8A0
void __fastcall sub_6FC6E8A0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    sub_6FC6E780(pGame, pUnit, 0, 4);
}

//D2Game.0x6FC6E8B0
void __fastcall sub_6FC6E8B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pMissile)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pMissile)
    {
        sub_6FC6E780(pGame, pUnit, pMissile, 5);
    }
}

//D2Game.0x6FC6E8D0
D2UnitStrc* __fastcall sub_6FC6E8D0(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nSuperUniqueId, int32_t a5, uint16_t nX, uint16_t nY, int32_t a8)
{
    D2UnitStrc* pMonster = D2GAME_SpawnMonster_6FC6F220(pGame, pRoom, pRoomCoordList, nX, nY, -1, nSuperUniqueId, a8);
    if (!pMonster)
    {
        return nullptr;
    }

    sub_6FC6E940(pGame, pMonster, a5);
    D2GAME_SpawnMinions_6FC6F440(pGame, pRoom, pRoomCoordList, pMonster, 1, 3, 6);
    return pMonster;
}

//D2Game.0x6FC6E940
void __fastcall sub_6FC6E940(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3)
{
    if (!pGame || pGame->nDifficulty >= 3u || !pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData || !pUnit->pMonsterData->nMonUmod)
    {
        return;
    }

    int32_t nIndex = 0;
    while (pUnit->pMonsterData->nMonUmod[nIndex])
    {
        ++nIndex;
        if (nIndex >= 9)
        {
            return;
        }
    }

    if (nIndex >= 8)
    {
        return;
    }

    if (a3)
    {
        uint32_t nChance = 0;
        if (sgptDataTables->nMonUModTxtRecordCount > 0 && sgptDataTables->pMonUModTxt)
        {
            nChance = sgptDataTables->pMonUModTxt->dwConstants;
        }

        if (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100 < nChance)
        {
            pUnit->pMonsterData->nTypeFlag |= 4u;

            int32_t nAccumulatedPicks = 0;
            int32_t nPickCount = 0;
            int32_t picks[512] = {};

            for (int32_t i = 0; i < sgptDataTables->nMonUModTxtRecordCount; ++i)
            {
                D2MonUModTxt* pMonUModTxtRecord = &sgptDataTables->pMonUModTxt[i];
                if (pMonUModTxtRecord->wCPick[pGame->nDifficulty] > 0 && pMonUModTxtRecord->nChampion && sub_6FC6EC10(pUnit, pMonUModTxtRecord, pGame->bExpansion))
                {
                    const uint16_t nCPick = pMonUModTxtRecord->wCPick[pGame->nDifficulty];

                    picks[2 * nPickCount] = i;
                    picks[2 * nPickCount + 1] = nCPick;

                    nAccumulatedPicks += nCPick;

                    ++nPickCount;
                    if (nPickCount >= 256)
                    {
                        break;
                    }
                }
            }

            int32_t nRand = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nAccumulatedPicks);
            nAccumulatedPicks = 0;
            for (int32_t i = 0; i < nPickCount; ++i)
            {
                const int32_t nPick = picks[2 * i + 1];
                if (nRand < nPick)
                {
                    nAccumulatedPicks = picks[2 * i];
                    break;
                }

                nRand -= nPick;
            }

            const int32_t nUModIndex = MONSTERUNIQUE_GetUModCount(pUnit->pMonsterData->nMonUmod);
            if (nUModIndex < 9)
            {
                pUnit->pMonsterData->nMonUmod[nUModIndex] = nAccumulatedPicks;
            }
            return;
        }
    }

    int32_t nRandCount = pGame->nDifficulty + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, 1) + 1;
    if (nRandCount + nIndex >= 9)
    {
        nRandCount = 9 - nIndex;
    }

    D2_ASSERT(sgptDataTables->nMonUModTxtRecordCount < 256);
    uint8_t bUModUsageTable[256] = {};

    for (int32_t i = 0; i < nIndex; ++i)
    {
        bUModUsageTable[pUnit->pMonsterData->nMonUmod[i]] = 1;
    }

    for (int32_t i = 0; i < nRandCount; ++i)
    {
        uint8_t* pUMod = &pUnit->pMonsterData->nMonUmod[nIndex];

        const int32_t nUMod = sub_6FC6EE90(pGame, pUnit, bUModUsageTable);
        if (!nUMod)
        {
            break;
        }

        *pUMod = nUMod;
        ++pUMod;

        bUModUsageTable[nUMod] = 1;
    }
}

//D2Game.0x6FC6EBE0
D2MonsterDataStrc* __fastcall MONSTERUNIQUE_GetMonsterData(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
    {
        return pUnit->pMonsterData;
    }
    
    return nullptr;
}

//D2Game.0x6FC6EBF0
int32_t __fastcall MONSTERUNIQUE_GetUModCount(uint8_t* pUMods)
{
    int32_t nIndex = 0;
    while (nIndex < 9 && pUMods[nIndex])
    {
        ++nIndex;
    }

    return nIndex;
}

//D2Game.0x6FC6EC10
int32_t __fastcall sub_6FC6EC10(D2UnitStrc* pUnit, D2MonUModTxt* pMonUModTxtRecord, int32_t bExpansion)
{
    if (!pMonUModTxtRecord->nEnabled)
    {
        return 0;
    }

    if (!bExpansion && pMonUModTxtRecord->wVersion >= 100u)
    {
        return 0;
    }

    for (int32_t i = 0; i < 2; ++i)
    {
        const int16_t nExcludeMonType = pMonUModTxtRecord->wExclude[i];
        if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
            if (pMonStatsTxtRecord && pMonStatsTxtRecord->wMonType >= 0 && pMonStatsTxtRecord->wMonType < sgptDataTables->nMonTypeTxtRecordCount
                && nExcludeMonType >= 0 && nExcludeMonType < sgptDataTables->nMonTypeTxtRecordCount
                && sgptDataTables->pMonTypeNest[(pMonStatsTxtRecord->wMonType >> 5) + nExcludeMonType * sgptDataTables->nMonTypeIndex] & gdwBitMasks[pMonStatsTxtRecord->wMonType & 0x1F])
            {
                return 0;
            }
        }
    }

    switch (pMonUModTxtRecord->nFPick)
    {
    case 1u:
    {
        if (!pUnit)
        {
            return 0;
        }

        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
        if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[4]))
        {
            return 0;
        }

        break;
    }
    case 2u:
    {
        if (pUnit)
        {
            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
            if (pMonStatsTxtRecord && (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_ISMELEE] || pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NOMULTISHOT]))
            {
                return 0;
            }
        }

        break;
    }
    case 3u:
    {
        if (!pUnit)
        {
            return 0;
        }

        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
        if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[2]))
        {
            return 0;
        }

        break;
    }
    }

    return 1;
}

//D2Game.0x6FC6EE90
int32_t __fastcall sub_6FC6EE90(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t* a3)
{
    if (!pGame || pGame->nDifficulty >= 3u)
    {
        return 0;
    }

    int32_t nPickCount = 0;
    int32_t nAccumulatedPicks = 0;
    int32_t picks[512] = {};

    for (int32_t i = 0; i < sgptDataTables->nMonUModTxtRecordCount; ++i)
    {
        D2MonUModTxt* pMonUModTxtRecord = &sgptDataTables->pMonUModTxt[i];
        if (pMonUModTxtRecord->wUPick[pGame->nDifficulty] > 0 && !pMonUModTxtRecord->nChampion && sub_6FC6EC10(pUnit, pMonUModTxtRecord, pGame->bExpansion))
        {
            if (!a3[i])
            {
                const uint16_t nUPick = pMonUModTxtRecord->wUPick[pGame->nDifficulty];
                
                picks[2 * nPickCount] = i;
                picks[2 * nPickCount + 1] = nUPick;

                nAccumulatedPicks += nUPick;

                ++nPickCount;
                if (nPickCount >= 256)
                {
                    break;
                }
            }
        }
    }

    uint32_t nRand = (uint32_t)ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nAccumulatedPicks);
    for (int32_t i = 0; i < nPickCount; ++i)
    {
        const uint32_t nPick = picks[2 * i + 1];
        if (nRand < nPick)
        {
            return picks[2 * i];
        }

        nRand -= nPick;
    }

    return 0;
}

//D2Game.0x6FC6F160
uint32_t __fastcall MONSTERUNIQUE_CheckMonModeFlag(int32_t nMonsterId, int32_t nFlag)
{
    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(nMonsterId);
    if (pMonStats2TxtRecord)
    {
        
        return BITMANIP_GetBitState(pMonStats2TxtRecord->nModeFlags, nFlag);
    }

    return 0;
}

//D2Game.0x6FC6F1D0
uint32_t __fastcall MONSTERUNIQUE_CheckMonStatsFlag(int32_t nMonsterId, int32_t nFlag)
{
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
    if (pMonStatsTxtRecord)
    {
        return pMonStatsTxtRecord->nMonStatsFlags[nFlag >> 3] & gdwBitMasks[nFlag & 7];
    }

    return 0;
}

//D2Game.0x6FC6F220
D2UnitStrc* __fastcall D2GAME_SpawnMonster_6FC6F220(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nX, int32_t nY, int32_t nUnitGUID, int32_t nClassId, int32_t a8)
{
    if (!nX && !nY)
    {
        if (!sub_6FC66260(pGame, pRoom, pRoomCoordList, nClassId, &nX, &nY, a8))
        {
            return nullptr;
        }
    }

    int32_t nTargetX = nX;
    int32_t nTargetY = nY;

    D2UnitStrc* pMonster = nullptr;
    if (pRoomCoordList)
    {
        pMonster = sub_6FC6A030(pGame, pRoom, pRoomCoordList, nTargetX, nTargetY, nClassId, 1, -1, 64);
    }
    else
    {
        if (nUnitGUID == -1)
        {
            pMonster = sub_6FC69F10(pGame, pRoom, nTargetX, nTargetY, nClassId, 1, nUnitGUID, 64);
            if (!pMonster)
            {
                pMonster = sub_6FC69F10(pGame, pRoom, nX, nY, nClassId, 1, 5, 64);
            }
        }
        else
        {
            pMonster = sub_6FC6A0F0(pGame, pRoom, nTargetX, nTargetY, nClassId, 1, nUnitGUID, -1, 98);
            if (!pMonster)
            {
                pMonster = sub_6FC6A0F0(pGame, pRoom, nX, nY, nClassId, 1, nUnitGUID, 5, 98);
                if (!pMonster)
                {
                    if (sub_6FC66260(pGame, pRoom, 0, nClassId, &nX, &nY, 0))
                    {
                        pMonster = sub_6FC6A0F0(pGame, pRoom, nX, nY, nClassId, 1, nUnitGUID, -1, 98);
                    }

                    if (!pMonster)
                    {
                        D2CoordStrc pCoord = {};
                        pCoord.nX = nX;
                        pCoord.nY = nY;

                        D2RoomStrc* pTargetRoom = nullptr;
                        D2Common_10136(pRoom, &pCoord, 1, 0x3C01u, &pTargetRoom);

                        if (!pTargetRoom)
                        {
                            return nullptr;
                        }

                        pMonster = sub_6FC6A0F0(pGame, pTargetRoom, pCoord.nX, pCoord.nY, nClassId, 1, nUnitGUID, -1, 98);
                    }
                }
            }
        }
    }

    if (!pMonster)
    {
        return nullptr;
    }

    if (pMonster->dwUnitType == UNIT_MONSTER)
    {
        D2MonsterRegionStrc* pMonsterRegion = MONSTERREGION_GetMonsterRegionFromLevelId(pGame->pMonReg, MONSTER_GetLevelId(pMonster));
        if (pMonsterRegion && (!pMonster->pMonsterData || !(pMonster->pMonsterData->nTypeFlag & 8)))
        {
            ++pMonsterRegion->dwUniqueCount;
        }

        if (pMonster->pMonsterData)
        {
            pMonster->pMonsterData->nTypeFlag |= 8u;
            pMonster->pMonsterData->nTypeFlag |= 1u;
        }
    }

    QUESTS_SetJadeFigurineBoss(pGame, pMonster);
    AIGENERAL_SetOwnerData(pGame, pMonster, pMonster->dwUnitId, 1, 1, 0);
    return pMonster;
}

//D2Game.0x6FC6F440
void __fastcall D2GAME_SpawnMinions_6FC6F440(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, D2UnitStrc* pUnit, int32_t bSpawnMinions, int32_t nMinGroup, int32_t nMaxGroup)
{
    if (bSpawnMinions)
    {
        D2MonsterDataStrc* pMonsterData = MONSTERUNIQUE_GetMonsterData(pUnit);
        if (!pMonsterData || !(pMonsterData->nTypeFlag & 4))
        {
            int32_t nMinionId = pUnit->dwClassId;

            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
            if (pMonStatsTxtRecord && pMonStatsTxtRecord->wMinion[0] >= 0 && pMonStatsTxtRecord->wMinion[0] < sgptDataTables->nMonStatsTxtRecordCount)
            {
                nMinionId = pMonStatsTxtRecord->wMinion[0];
            }

            if (nMinionId >= 0 && nMinionId < sgptDataTables->nMonStatsTxtRecordCount)
            {
                const int32_t nMinionCount = nMinGroup + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxGroup - nMinGroup + 1);
                for (int32_t i = 0; i < nMinionCount; ++i)
                {
                    D2UnitStrc* pMinion = nullptr;
                    if (pRoomCoordList)
                    {
                        pMinion = sub_6FC69F70(pGame, pRoomCoordList, pUnit, nMinionId, 1, 3, 64);
                    }
                    else
                    {
                        pMinion = sub_6FC68D70(pGame, pUnit, nMinionId, 1, 3, 64);
                    }

                    if (pMinion)
                    {
                        uint8_t* pOwnerUMods = MONSTERUNIQUE_GetUMods(pUnit);
                        uint8_t* pMinionUMods = MONSTERUNIQUE_GetUMods(pMinion);
                        if (pOwnerUMods && pMinionUMods)
                        {
                            int32_t nUModIndex = MONSTERUNIQUE_GetUModCount(pMinionUMods);
                            uint8_t* pMinionUMod = &pMinionUMods[nUModIndex];
                            while (nUModIndex < 9)
                            {
                                uint8_t nOwnerUMod = *pOwnerUMods;
                                if (!nOwnerUMod)
                                {
                                    break;
                                }

                                D2MonUModTxt* pUModTxtRecord = MONSTERUNIQUE_GetMonUModTxtRecord(nOwnerUMod);
                                if (!pUModTxtRecord)
                                {
                                    break;
                                }

                                if (pUModTxtRecord->nXfer)
                                {
                                    *pMinionUMod = nOwnerUMod;
                                    ++pMinionUMod;
                                }

                                ++pOwnerUMods;
                                ++nUModIndex;
                            }
                        }

                        AIGENERAL_SetOwnerData(pGame, pMinion, pUnit->dwUnitId, 1, 0, 0);
                        AIGENERAL_AllocMinionList(pGame, pUnit, pMinion);
                        AIUTIL_SetOwnerGUIDAndType(pMinion, pUnit);
                        MONSTERUNIQUE_ToggleMonTypeFlag(pMinion, 16, 1);
                    }
                }
            }
        }
    }

    constexpr uint8_t mods[4] =
    {
        1, 2, 3, 4
    };

    for (int32_t i = 0; i < std::size(mods); ++i)
    {
        sub_6FC6F670(pUnit, mods[i], 1);

        AIGENERAL_ExecuteCallbackOnMinions(pUnit, (void*)mods[i], 0, (void(__fastcall*)(D2UnitStrc*, void*, void*))sub_6FC6F670);
    }

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData && pUnit->pMonsterData->nMonUmod)
    {
        for (int32_t i = 0; i < 9; ++i)
        {
            const uint8_t nUMod = pUnit->pMonsterData->nMonUmod[i];
            if (!nUMod)
            {
                break;
            }

            sub_6FC6F670(pUnit, nUMod, 1);

            AIGENERAL_ExecuteCallbackOnMinions(pUnit, (void*)nUMod, 0, (void(__fastcall*)(D2UnitStrc*, void*, void*))sub_6FC6F670);
        }
    }
}

//D2Game.0x6FC6F670
void __fastcall sub_6FC6F670(D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    constexpr UModInitFunc dword_6FD2F018[] =
    {
        nullptr,
        MONSTERUNIQUE_UMod1_RandomName,
        MONSTERUNIQUE_UMod2_HealthBonus,
        nullptr,
        MONSTERUNIQUE_UMod4_LevelBonus,
        MONSTERUNIQUE_UMod5_Strong,
        MONSTERUNIQUE_UMod6_Fast,
        nullptr,
        MONSTERUNIQUE_UMod8_Resistant,
        MONSTERUNIQUE_UMod9_FireEnchanted,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        MONSTERUNIQUE_UMod16_Champion,
        MONSTERUNIQUE_UMod17_LightningEnchanted,
        MONSTERUNIQUE_UMod18_ColdEnchanted,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        MONSTERUNIQUE_UMod23_PoisonEnchanted,
        nullptr,
        MONSTERUNIQUE_UMod25_ManaSteal,
        MONSTERUNIQUE_UMod26_Teleport,
        MONSTERUNIQUE_UMod8_Resistant,
        MONSTERUNIQUE_UMod8_Resistant,
        nullptr,
        MONSTERUNIQUE_UMod30_AuraEnchanted,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        MONSTERUNIQUE_UMod36_Ghostly,
        MONSTERUNIQUE_UMod37_Fanatic,
        MONSTERUNIQUE_UMod38_Possessed,
        MONSTERUNIQUE_UMod39_Berserk,
        nullptr,
        MONSTERUNIQUE_UMod41_AlwaysRun,
        nullptr,
        nullptr,
    };

    UModInitFunc pCallback = dword_6FD2F018[nUMod];
    if (pCallback)
    {
        pCallback(pUnit, nUMod, bUnique);
    }
}

//D2Game.0x6FC6F690
D2UnitStrc* __fastcall D2GAME_SpawnSuperUnique_6FC6F690(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nSuperUnique)
{
    if (pGame->nDifficulty >= 3u)
    {
        return nullptr;
    }
    
    D2SuperUniquesTxt* pSuperUniquesTxtRecord = DATATBLS_GetSuperUniquesTxtRecord(nSuperUnique);
    if (!pSuperUniquesTxtRecord)
    {
        return nullptr;
    }

    if (pSuperUniquesTxtRecord->dwClass < 0 || !pSuperUniquesTxtRecord->nStacks && pGame->nBossFlagList[nSuperUnique >> 3] & gdwBitMasks[nSuperUnique & 7])
    {
        return nullptr;
    }
    
    D2UnitStrc* pMonster = nullptr;
    if (pSuperUniquesTxtRecord->nAutopos)
    {
        pMonster = D2GAME_SpawnMonster_6FC6F220(pGame, pRoom, nullptr, 0, 0, -1, pSuperUniquesTxtRecord->dwClass, 0);
    }
    else
    {
        pMonster = D2GAME_SpawnMonster_6FC6F220(pGame, pRoom, nullptr, nX, nY, -1, pSuperUniquesTxtRecord->dwClass, 0);
    }

    if (!pMonster)
    {
        return nullptr;
    }

    if (pMonster->dwUnitType != UNIT_MONSTER || !pMonster->pMonsterData || !pMonster->pMonsterData->nMonUmod)
    {
        return pMonster;
    }

    pGame->nBossFlagList[nSuperUnique >> 3] |= (uint8_t)gdwBitMasks[nSuperUnique & 7];

    pMonster->pMonsterData->nTypeFlag |= 2u;
    pMonster->pMonsterData->wBossHcIdx = nSuperUnique;


    const int32_t nCounter = MONSTERUNIQUE_GetUModCount(pMonster->pMonsterData->nMonUmod);
    if (nCounter >= 5)
    {
        return pMonster;
    }

    D2_ASSERT(sgptDataTables->nMonUModTxtRecordCount < 256);
    uint8_t bUModUsageTable[256] = {};

    int32_t bAuraEnchanted = 0;

    uint8_t* pMonUMods = &pMonster->pMonsterData->nMonUmod[nCounter];
    for (int32_t i = 0; i < 3; ++i)
    {
        const uint8_t nUMod = pSuperUniquesTxtRecord->dwMod[i];
        if (!nUMod)
        {
            break;
        }

        if (nUMod == MONUMOD_AURACHANT)
        {
            bAuraEnchanted = 1;
            *pMonUMods = nUMod;
            bUModUsageTable[nUMod] = 1;
            ++pMonUMods;
        }
        else if (nUMod != MONUMOD_THIEF)
        {
            *pMonUMods = nUMod;
            bUModUsageTable[nUMod] = 1;
            ++pMonUMods;
        }
    }

    for (int32_t i = 0; i < pGame->nDifficulty; ++i)
    {
        const int32_t nUMod = sub_6FC6EE90(pGame, pMonster, bUModUsageTable);
        if (!nUMod)
        {
            break;
        }

        *pMonUMods = nUMod;
        ++pMonUMods;

        bUModUsageTable[nUMod] = 1;
    }

    int32_t nMinGroup = pSuperUniquesTxtRecord->dwMinGrp;
    int32_t nMaxGroup = pSuperUniquesTxtRecord->dwMaxGrp;
    if (nMinGroup && nMaxGroup)
    {
        nMinGroup += pGame->nDifficulty;
        nMaxGroup += pGame->nDifficulty;
    }

    D2GAME_SpawnMinions_6FC6F440(pGame, pRoom, 0, pMonster, 1, nMinGroup, nMaxGroup);

    if (bAuraEnchanted)
    {
        MONSTERUNIQUE_UMod30_AuraEnchanted(pMonster, MONUMOD_AURACHANT, 1);
    }

    switch (pSuperUniquesTxtRecord->dwHcIdx)
    {
    case SUPERUNIQUE_THE_COUNTESS:
    {
        STATES_ToggleState(pMonster, STATE_CORPSE_NOSELECT, 1);
        QUESTS_CreateChainRecord(pGame, pMonster, QUEST_A1Q5_COUNTESS);
        AITHINK_ExecuteAiFn(pGame, pMonster, pMonster->pMonsterData->pAiControl, 13);
        break;
    }
    case SUPERUNIQUE_ISMAIL_VILEHAND:
    case SUPERUNIQUE_GELEB_FLAMEFINGER:
    case SUPERUNIQUE_TOORC_ICEFIST:
    {
        QUESTS_CreateChainRecord(pGame, pMonster, QUEST_A3Q5_TRAVINCAL);
        QUESTS_MonsterSpawn(pGame, pMonster);
        break;
    }
    case SUPERUNIQUE_INFECTOR_OF_SOULS:
    case SUPERUNIQUE_LORD_DE_SEIS:
    case SUPERUNIQUE_GRAND_VIZIER_OF_CHAOS:
    {
        QUESTS_CreateChainRecord(pGame, pMonster, QUEST_A4Q2_DIABLO);
        break;
    }
    case SUPERUNIQUE_THE_COW_KING:
    {
        QUESTS_CreateChainRecord(pGame, pMonster, QUEST_A1Q4_CAIN);
        break;
    }
    case SUPERUNIQUE_RADAMENT:
    {
        const uint32_t nMinionCount = ITEMS_RollRandomNumber(&pMonster->pSeed) % 5 + 2;
        for (uint32_t i = 0; i < nMinionCount; ++i)
        {
            sub_6FC68D70(pGame, pMonster, MONSTER_SKELETON5, 1, 4, 64);
        }

        sub_6FC68D70(pGame, pMonster, MONSTER_SKMAGE_POIS3, 1, 4, 64);
        sub_6FC68D70(pGame, pMonster, MONSTER_SKMAGE_COLD4, 1, 4, 64);
        sub_6FC68D70(pGame, pMonster, MONSTER_SKMAGE_FIRE3, 1, 4, 64);
        sub_6FC68D70(pGame, pMonster, MONSTER_SKMAGE_LTNG3, 1, 4, 64);
        break;
    }
    case SUPERUNIQUE_SIEGE_BOSS:
    {
        sub_6FC6A230(pGame, pMonster, MONSTER_MINION1, 1, 20, 20, 0);
        QUESTS_CreateChainRecord(pGame, pMonster, QUEST_A5Q1_SHENK);
        QUESTS_MonsterSpawn(pGame, pMonster);
        STATES_ToggleState(pMonster, STATE_CORPSE_NOSELECT, 1);
        break;
    }
    case SUPERUNIQUE_ANCIENT_BARBARIAN_1:
    case SUPERUNIQUE_ANCIENT_BARBARIAN_2:
    case SUPERUNIQUE_ANCIENT_BARBARIAN_3:
    {
        QUESTS_CreateChainRecord(pGame, pMonster, QUEST_A5Q5_ANCIENTS);
        QUESTS_MonsterSpawn(pGame, pMonster);
        break;
    }
    case SUPERUNIQUE_NIHLATHAK_BOSS:
    {
        AIGENERAL_SetOwnerData(pGame, pMonster, pMonster->dwUnitId, 1, 1, 0);
        sub_6FC6A230(pGame, pMonster, D2Common_11063(UNITS_GetRoom(pMonster), MONSTER_MINION1), 1, 10, 20, 64);
        QUESTS_CreateChainRecord(pGame, pMonster, QUEST_A5Q4_NIHLATHAK);
        break;
    }
    case SUPERUNIQUE_BAAL_SUBJECT_2:
    {
        sub_6FC6A230(pGame, pMonster, MONSTER_SKMAGE_COLD3, 1, 20, 10, 64);
        break;
    }
    default:
        break;
    }

    D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pMonster, MONUMOD_QUESTMOD, 1);

    return pMonster;
}

//D2Game.0x6FC6FBA0
D2UnitStrc* __fastcall sub_6FC6FBA0(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nClassId, int32_t nUnitGUID, uint16_t nNameSeed, int32_t bChampion, int32_t bSuperUnique, int16_t nBossHcIdx, uint8_t* pUMods)
{
    D2UnitStrc* pMonster = D2GAME_SpawnMonster_6FC6F220(pGame, pRoom, 0, nX, nY, nUnitGUID, nClassId, 1);
    if (!pMonster || !pMonster->dwUnitType == UNIT_MONSTER)
    {
        return pMonster;
    }

    D2MonsterDataStrc* pMonsterData = pMonster->pMonsterData;
    if (!pMonsterData)
    {
        return pMonster;
    }

    if (bChampion)
    {
        pMonsterData->nTypeFlag |= MONTYPEFLAG_CHAMPION;
    }
    else
    {
        pMonsterData->nTypeFlag &= (uint8_t)~MONTYPEFLAG_CHAMPION;
    }

    int32_t bIsAuraEnchanted = 0;
    for (int32_t i = 0; i < 9; ++i)
    {
        pMonsterData->nMonUmod[i] = pUMods[i];
        if (pUMods[i] == MONUMOD_AURACHANT)
        {
            bIsAuraEnchanted = 1;
        }
    }

    D2GAME_SpawnMinions_6FC6F440(pGame, pRoom, nullptr, pMonster, 0, 0, 0);

    pMonsterData->wNameSeed = nNameSeed;

    if (bIsAuraEnchanted)
    {
        MONSTERUNIQUE_UMod30_AuraEnchanted(pMonster, MONUMOD_AURACHANT, 1);
    }

    if (bSuperUnique)
    {
        pMonsterData->nTypeFlag |= MONTYPEFLAG_SUPERUNIQUE;
        pMonsterData->wBossHcIdx = nBossHcIdx;

        switch (nBossHcIdx)
        {
        case SUPERUNIQUE_THE_COUNTESS:
            STATES_ToggleState(pMonster, STATE_CORPSE_NOSELECT, 1);
            QUESTS_CreateChainRecord(pGame, pMonster, 5);
            break;
        case SUPERUNIQUE_NIHLATHAK_BOSS:
            QUESTS_CreateChainRecord(pGame, pMonster, 34);
            break;
        case SUPERUNIQUE_SIEGE_BOSS:
            STATES_ToggleState(pMonster, STATE_CORPSE_NOSELECT, 1);
            QUESTS_CreateChainRecord(pGame, pMonster, 31);
            break;
        case SUPERUNIQUE_ANCIENT_BARBARIAN_1:
        case SUPERUNIQUE_ANCIENT_BARBARIAN_2:
        case SUPERUNIQUE_ANCIENT_BARBARIAN_3:
            QUESTS_CreateChainRecord(pGame, pMonster, 35);
            break;
        case SUPERUNIQUE_ISMAIL_VILEHAND:
        case SUPERUNIQUE_GELEB_FLAMEFINGER:
        case SUPERUNIQUE_TOORC_ICEFIST:
            QUESTS_CreateChainRecord(pGame, pMonster, 19);
            break;
        case SUPERUNIQUE_THE_COW_KING:
            QUESTS_CreateChainRecord(pGame, pMonster, 4);
            break;
        case SUPERUNIQUE_INFECTOR_OF_SOULS:
        case SUPERUNIQUE_LORD_DE_SEIS:
        case SUPERUNIQUE_GRAND_VIZIER_OF_CHAOS:
            QUESTS_CreateChainRecord(pGame, pMonster, 23);
            break;
        default:
            break;
        }
    }

    return pMonster;
}

//D2Game.0x6FC6FDC0
D2UnitStrc* __fastcall sub_6FC6FDC0(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nClassId, int32_t nUnitGUID, uint8_t* pUMods)
{
    D2UnitStrc* pMonster = sub_6FC6A0F0(pGame, pRoom, nX, nY, nClassId, 1, nUnitGUID, -1, 98);
    if (!pMonster)
    {
        pMonster = sub_6FC6A0F0(pGame, pRoom, nX, nY, nClassId, 1, nUnitGUID, 4, 98);
        if (!pMonster && sub_6FC66260(pGame, pRoom, 0, nClassId, &nX, &nY, 1))
        {
            pMonster = sub_6FC6A0F0(pGame, pRoom, nX, nY, nClassId, 1, nUnitGUID, -1, 98);
        }
    }

    if (!pMonster || pMonster->dwUnitType != UNIT_MONSTER || !pMonster->pMonsterData)
    {
        return nullptr;
    }

    for (int32_t i = 0; i < 9; ++i)
    {
        pMonster->pMonsterData->nMonUmod[i] = pUMods[i];
    }

    if (pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        pMonster->pMonsterData->nTypeFlag |= MONTYPEFLAG_MINION;
    }

    constexpr int32_t mods[] = { 1, 2, 3, 4 };
    for (int32_t i = 0; i < 4; ++i)
    {
        sub_6FC6F670(pMonster, mods[i], 0);
    }

    for (int32_t i = 0; i < 9; ++i)
    {
        sub_6FC6F670(pMonster, pMonster->pMonsterData->nMonUmod[i], 0);
    }
    
    return pMonster;
}

//D2Game.0x6FC6FF10
void __fastcall D2GAME_BOSSES_AssignUMod_6FC6FF10(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUMod, int32_t bUnique)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
    {
        return;
    }

    uint8_t* pUMods = pUnit->pMonsterData->nMonUmod;
    if (!pUMods)
    {
        return;
    }

    if (bUnique)
    {
        D2MonsterRegionStrc* pMonRegion = MONSTERREGION_GetMonsterRegionFromLevelId(pGame->pMonReg, MONSTER_GetLevelId(pUnit));
        if (pMonRegion && !(pUnit->pMonsterData->nTypeFlag & MONTYPEFLAG_UNIQUE))
        {
            ++pMonRegion->dwUniqueCount;
        }

        pUnit->pMonsterData->nTypeFlag |= MONTYPEFLAG_UNIQUE;
    }

    for (int32_t i = 0; i < 9; ++i)
    {
        if (!pUMods[i])
        {
            pUMods[i] = nUMod;
            sub_6FC6F670(pUnit, nUMod, bUnique);
        }
    }
}

//D2Game.0x6FC6FFD0
void __fastcall D2GAME_MONSTERS_Unk_6FC6FFD0(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nUMod)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2MonsterDataStrc* pMonsterData = pUnit->pMonsterData;
    if (!pMonsterData || pMonsterData->nTypeFlag & MONTYPEFLAG_CHAMPION)
    {
        return;
    }

    D2MonsterRegionStrc* pMonsterRegion = MONSTERREGION_GetMonsterRegionFromLevelId(pGame->pMonReg, MONSTER_GetLevelId(pUnit));
    if (pMonsterRegion && !(pMonsterData->nTypeFlag & MONTYPEFLAG_UNIQUE))
    {
        ++pMonsterRegion->dwUniqueCount;
    }

    pMonsterData->nTypeFlag |= MONTYPEFLAG_UNIQUE;
    pMonsterData->nTypeFlag |= MONTYPEFLAG_CHAMPION | MONTYPEFLAG_OTHER;

    for (int32_t i = 0; i < 9; ++i)
    {
        if (!pMonsterData->nMonUmod[i])
        {
            pMonsterData->nMonUmod[i] = nUMod;
            D2GAME_SpawnMinions_6FC6F440(pGame, UNITS_GetRoom(pUnit), nullptr, pUnit, 1, 0, 0);
            return;
        }
    }
}
