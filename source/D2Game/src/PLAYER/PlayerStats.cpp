#include "PLAYER/PlayerStats.h"

#include <algorithm>

#include <D2DataTbls.h>
#include <D2StatList.h>
#include <Units/UnitRoom.h>

#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "PLAYER/PlrSave2.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitProxy.h"


//D2Game.0x6FC7EA50
void __fastcall PLAYERSTATS_SetStatsForStartingAct(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nAct)
{
    constexpr int32_t nTargetLevels[] =
    {
        1, 15, 20, 26, 32
    };

    if (nAct >= NUM_ACTS)
    {
        nAct = 4;
    }

    if (!pUnit)
    {
        return;
    }

    D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pUnit->dwClassId);
    if (!pCharStatsTxtRecord)
    {
        return;
    }

    STATLIST_SetUnitStat(pUnit, STAT_STRENGTH, pCharStatsTxtRecord->nStr, 0);
    STATLIST_SetUnitStat(pUnit, STAT_ENERGY, pCharStatsTxtRecord->nInt, 0);
    STATLIST_SetUnitStat(pUnit, STAT_DEXTERITY, pCharStatsTxtRecord->nDex, 0);
    STATLIST_SetUnitStat(pUnit, STAT_VITALITY, pCharStatsTxtRecord->nVit, 0);
    STATLIST_SetUnitStat(pUnit, STAT_TOHIT, 0, 0);
    STATLIST_SetUnitStat(pUnit, STAT_TOBLOCK, 0, 0);
    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, (pCharStatsTxtRecord->nVit + pCharStatsTxtRecord->nLifeAdd) << 8, 0);
    STATLIST_SetUnitStat(pUnit, STAT_MAXHP, (pCharStatsTxtRecord->nVit + pCharStatsTxtRecord->nLifeAdd) << 8, 0);
    STATLIST_SetUnitStat(pUnit, STAT_MANA, pCharStatsTxtRecord->nInt << 8, 0);
    STATLIST_SetUnitStat(pUnit, STAT_MAXMANA, pCharStatsTxtRecord->nInt << 8, 0);
    STATLIST_SetUnitStat(pUnit, STAT_STAMINA, pCharStatsTxtRecord->nStamina << 8, 0);
    STATLIST_SetUnitStat(pUnit, STAT_MAXSTAMINA, pCharStatsTxtRecord->nStamina << 8, 0);
    STATLIST_SetUnitStat(pUnit, STAT_LEVEL, 1, 0);
    STATLIST_SetUnitStat(pUnit, STAT_NEXTEXP, DATATBLS_GetLevelThreshold(pUnit->dwClassId, 1u), 0);
    STATLIST_SetUnitStat(pUnit, STAT_ATTACKRATE, 100, 0);
    STATLIST_SetUnitStat(pUnit, STAT_VELOCITYPERCENT, 100, 0);
    STATLIST_SetUnitStat(pUnit, STAT_OTHER_ANIMRATE, 100, 0);

    if (nAct == ACT_I)
    {
        return;
    }

    int32_t nIndex = nAct;
    if (nIndex >= std::size(nTargetLevels))
    {
        nIndex = 0;
    }

    SUNITDMG_SetExperienceForTargetLevel(pGame, pUnit, nTargetLevels[nIndex]);
}

//D2Game.0x6FC7EC00
void __fastcall PLAYERSTATS_LevelUp(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    int32_t nClassId = -1;
    if (pUnit)
    {
        nClassId = pUnit->dwClassId;
    }

    const int32_t nOldLevel = STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0);
    const uint32_t nExperience = STATLIST_UnitGetStatValue(pUnit, STAT_EXPERIENCE, 0);
    const int32_t nNewLevel = DATATBLS_GetCurrentLevelFromExp(nClassId, nExperience);
    STATLIST_SetUnitStat(pUnit, STAT_LEVEL, nNewLevel, 0);
    STATLIST_SetUnitStat(pUnit, STAT_NEXTEXP, DATATBLS_GetLevelThreshold(nClassId, nNewLevel), 0);
    const int32_t nLevelDiff = nNewLevel - nOldLevel;
    if (nLevelDiff <= 0)
    {
        return;
    }

    D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(nClassId);

    STATLIST_SetUnitStat(pUnit, STAT_MAXHP, (nLevelDiff * pCharStatsTxtRecord->nLifePerLevel << 6) + STATLIST_GetUnitBaseStat(pUnit, STAT_MAXHP, 0), 0);
    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
    if (STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) > 0)
    {
        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nMaxHp, 0);
    }

    STATLIST_SetUnitStat(pUnit, STAT_MAXMANA, (nLevelDiff * pCharStatsTxtRecord->nManaPerLevel << 6) + STATLIST_GetUnitBaseStat(pUnit, STAT_MAXMANA, 0), 0);
    const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pUnit);
    STATLIST_SetUnitStat(pUnit, STAT_MANA, nMaxMana, 0);

    STATLIST_SetUnitStat(pUnit, STAT_MAXSTAMINA, (nLevelDiff * pCharStatsTxtRecord->nStaminaPerLevel << 6) + STATLIST_GetUnitBaseStat(pUnit, STAT_MAXSTAMINA, 0), 0);
    const int32_t nMaxStamina = STATLIST_GetMaxStaminaFromUnit(pUnit);
    STATLIST_SetUnitStat(pUnit, STAT_STAMINA, nMaxStamina, 0);

    STATLIST_AddUnitStat(pUnit, STAT_STATPTS, nLevelDiff * pCharStatsTxtRecord->nStatsPerLevel, 0);

    STATLIST_AddUnitStat(pUnit, STAT_SKILLPTS, nLevelDiff, 0);

    SUNITPROXY_InitializeNpcEventChain(pGame, pUnit);
    SUNIT_AttachSound(pUnit, 2u, pUnit);
    SUNIT_IterateLivingPlayers(pGame, PLAYERSTATS_OnPlayerLeveledUp, pUnit);
    D2GAME_UpdatePlayerVitals_6FC4B240(pGame, pUnit);
    D2GAME_UpdatePlayerItems_6FC4BB90(pGame, pUnit, 1);

    if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfUpdateGameInformation)
    {
        gpD2EventCallbackTable_6FD45830->pfUpdateGameInformation(pGame->nGameId, UNITS_GetPlayerName(pUnit), nClassId, nNewLevel);
    }
}

//D2Game.0x6FC7EDE0
void __fastcall PLAYERSTATS_OnPlayerLeveledUp(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pLeveledUpPlayer)
{
    SCMD_Send0x75_PartyRosterUpdate(pUnit, (D2UnitStrc*)pLeveledUpPlayer);
}

//D2Game.0x6FC7EDF0
int32_t __fastcall PLAYERSTATS_SpendStatPoint(D2UnitStrc* pUnit, int32_t nStatId)
{
    if (!STATLIST_UnitGetStatValue(pUnit, STAT_STATPTS, 0))
    {
        return 0;
    }

    switch (nStatId)
    {
    case STAT_STRENGTH:
    {
        STATLIST_AddUnitStat(pUnit, STAT_STATPTS, -1, 0);
        STATLIST_AddUnitStat(pUnit, STAT_STRENGTH, 1, 0);
        UNITROOM_RefreshUnit(pUnit);
        return 1;
    }
    case STAT_ENERGY:
    {
        D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pUnit->dwClassId);
        if (pCharStatsTxtRecord)
        {
            STATLIST_AddUnitStat(pUnit, STAT_STATPTS, -1, 0);
            STATLIST_AddUnitStat(pUnit, STAT_ENERGY, 1, 0);
            STATLIST_AddUnitStat(pUnit, STAT_MANA, pCharStatsTxtRecord->nManaPerMagic << 6, 0);
            STATLIST_AddUnitStat(pUnit, STAT_MAXMANA, pCharStatsTxtRecord->nManaPerMagic << 6, 0);
        }
        return 1;
    }
    case STAT_DEXTERITY:
    {
        STATLIST_AddUnitStat(pUnit, STAT_STATPTS, -1, 0);
        STATLIST_AddUnitStat(pUnit, STAT_DEXTERITY, 1, 0);
        UNITROOM_RefreshUnit(pUnit);
        return 1;
    }
    case STAT_VITALITY:
    {
        if (pUnit)
        {
            D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pUnit->dwClassId);
            if (pCharStatsTxtRecord)
            {
                STATLIST_AddUnitStat(pUnit, STAT_STATPTS, -1, 0);
                STATLIST_AddUnitStat(pUnit, STAT_VITALITY, 1, 0);
                STATLIST_AddUnitStat(pUnit, STAT_MAXHP, pCharStatsTxtRecord->nLifePerVitality << 6, 0);
                STATLIST_AddUnitStat(pUnit, STAT_HITPOINTS, pCharStatsTxtRecord->nLifePerVitality << 6, 0);
                const int32_t nMaxHp = STATLIST_UnitGetStatValue(pUnit, STAT_MAXHP, 0);
                if (STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) > nMaxHp)
                {
                    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nMaxHp, 0);
                }
                STATLIST_AddUnitStat(pUnit, STAT_MAXSTAMINA, pCharStatsTxtRecord->nStaminaPerVitality << 6, 0);
                STATLIST_AddUnitStat(pUnit, STAT_STAMINA, pCharStatsTxtRecord->nStaminaPerVitality << 6, 0);
                const int32_t nMaxStamina = STATLIST_UnitGetStatValue(pUnit, STAT_MAXSTAMINA, 0);
                if (STATLIST_UnitGetStatValue(pUnit, STAT_STAMINA, 0) > nMaxStamina)
                {
                    STATLIST_SetUnitStat(pUnit, STAT_STAMINA, nMaxStamina, 0);
                }
            }
        }
        return 1;
    }
	default:
		return 0;
    }
}
