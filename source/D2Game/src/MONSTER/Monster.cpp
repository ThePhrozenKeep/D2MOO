#include "MONSTER/Monster.h"

#include <algorithm>

#include <D2BitManip.h>
#include <D2Math.h>

#include <DataTbls/MonsterIds.h>
#include <DataTbls/MonsterTbls.h>
#include <D2Chat.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2ItemMods.h>
#include <D2Monsters.h>
#include <D2Skills.h>
#include <D2StatList.h>

#include "AI/AiGeneral.h"
#include "AI/AiThink.h"
#include "GAME/Event.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterUnique.h"
#include "PLAYER/Player.h"
#include "QUESTS/Quests.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitProxy.h"


//D2Game.0x6FC601C0
void __fastcall MONSTER_SetAiState(D2UnitStrc* pMonster, int32_t nAiState)
{
    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        pMonster->pMonsterData->dwAiState = nAiState;
    }
}

//D2Game.0x6FC601E0
int32_t __fastcall MONSTER_GetAiState(D2UnitStrc* pMonster)
{
    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        return pMonster->pMonsterData->dwAiState;
    }

    return 1;
}

//D2Game.0x6FC60200
void __fastcall MONSTER_SetLevelId(D2UnitStrc* pMonster, int32_t nLevelId)
{
    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        pMonster->pMonsterData->dwTxtLevelNo = nLevelId;
    }
}

//D2Game.0x6FC60220
int32_t __fastcall MONSTER_GetLevelId(D2UnitStrc* pMonster)
{
    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        return pMonster->pMonsterData->dwTxtLevelNo;
    }

    return 0;
}

//D2Game.0x6FC60240
int32_t __fastcall MONSTER_CheckSummonerFlag(D2UnitStrc* pMonster, uint16_t nFlag)
{
    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        return (pMonster->pMonsterData->wSummonerFlag & nFlag) != 0;
    }

    return 0;
}

//D2Game.0x6FC60270
void __fastcall MONSTER_ToggleSummonerFlag(D2UnitStrc* pMonster, uint16_t nFlag, int32_t bSet)
{
    if (!pMonster || pMonster->dwUnitType != UNIT_MONSTER || !pMonster->pMonsterData)
    {
        return;
    }

    if (bSet)
    {
        pMonster->pMonsterData->wSummonerFlag |= nFlag;
    }
    else
    {
        pMonster->pMonsterData->wSummonerFlag &= ~nFlag;
    }
}

//D2Game.0x6FC602A0
void __fastcall MONSTER_Initialize(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2UnitStrc* pMonster, int32_t nUnitGUID)
{
    if (pMonster)
    {
        pMonster->dwUnitId = nUnitGUID;
        pMonster->dwFlags |= UNITFLAG_ISVALIDTARGET | UNITFLAG_TARGETABLE;
    }

    D2GAME_DeleteTimersOnUnit_6FC34A30(pGame, pMonster);

    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        pMonster->pMonsterData->pAiControl = AIGENERAL_AllocAiControl(pGame);
        pMonster->pMonsterData->pAiParam = D2GAME_MONSTERMODE_AllocParams_6FC62FF0(pGame);
        pMonster->pMonsterData->pMonInteract = MONSTERAI_AllocMonsterInteract(pGame);
        pMonster->pMonsterData->pMonstatsTxt = MONSTERMODE_GetMonStatsTxtRecord(pMonster->dwClassId);

        MONSTER_InitializeStatsAndSkills(pGame, pRoom, pMonster, sub_6FC67FA0(pGame->pMonReg, pRoom, pMonster));
        AITHINK_ExecuteAiFn(pGame, pMonster, pMonster->pMonsterData->pAiControl, AISPECIALSTATE_NONE);
        pMonster->pMonsterData->dwTxtLevelNo = DUNGEON_GetLevelIdFromRoom(pRoom);

        const int32_t nAnimMode = pMonster->dwAnimMode;
        if (nAnimMode != MONMODE_DEATH && nAnimMode != MONMODE_DEAD)
        {
            QUESTS_AttachLevelChainRecord(pGame, pMonster, pRoom, 1);
            sub_6FCBC4D0(pMonster);
            return;
        }

        SUNIT_SetCombatMode(pGame, pMonster, nAnimMode);
    }

    sub_6FCBC4D0(pMonster);
}

//Inlined in D2Game.0x6FC603D0
D2MonPropTxt* __fastcall MONSTER_GetMonPropTxtRecord(int32_t nId)
{
    if (nId >= 0 && nId < sgptDataTables->nMonPropTxtRecordCount)
    {
        return &sgptDataTables->pMonPropTxt[nId];
    }

    return nullptr;
}

//D2Game.0x6FC603D0
void __fastcall MONSTER_InitializeStatsAndSkills(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2UnitStrc* pUnit, D2MonRegDataStrc* pMonRegData)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData || !pUnit->pMonsterData->pMonstatsTxt)
    {
        return;
    }

    int32_t nClassId = pUnit->dwClassId;
    D2MonStatsTxt* pMonStatsTxtRecord = pUnit->pMonsterData->pMonstatsTxt;

    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(nClassId);
    if (!pMonStats2TxtRecord)
    {
        return;
    }

    MONSTER_SetComponents(pMonRegData, pUnit);

    if (nClassId >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        nClassId = -1;
    }

    if (nClassId == MONSTER_DOOMKNIGHT2)
    {
        pUnit->pMonsterData->nComponent[10] = ITEMS_RollRandomNumber(&pUnit->pSeed) & 3;
    }
    else if (nClassId == MONSTER_DOOMKNIGHT3)
    {
        const uint8_t nRand = ITEMS_RollRandomNumber(&pUnit->pSeed) & 3;
        pUnit->pMonsterData->nComponent[10] = nRand;
        pUnit->pMonsterData->nComponent[11] = nRand;
    }

    D2PlayerCountBonusStrc playerCountBonus = {};
    MONSTER_GetPlayerCountBonus(pGame, &playerCountBonus, pRoom, pUnit);

    uint8_t nDifficulty = D2Clamp(pGame->nDifficulty, 0ui8, 2ui8);
    if (MONSTERS_GetHirelingTypeId(pUnit))
    {
        nDifficulty = 0;
    }

    const int32_t nPlayerCount = std::max(playerCountBonus.nPlayerCount, 1);
    int32_t nLevel = pMonStatsTxtRecord->nLevel[nDifficulty];
    if (pGame->bExpansion && nDifficulty > 0 && !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NORATIO]) && !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_BOSS]))
    {
        nLevel = DATATBLS_GetMonsterLevelInArea(DUNGEON_GetLevelIdFromRoom(pRoom), nDifficulty, pGame->bExpansion);
    }

    STATLIST_AllocStatListEx(pUnit, 0, D2GAME_ITEMMODE_ServerStatlistCallback_6FC41910, pGame);
    STATLIST_SetUnitStat(pUnit, STAT_LEVEL, nLevel, 0);
    STATLIST_SetUnitStat(pUnit, STAT_MONSTER_PLAYERCOUNT, nPlayerCount, 0);
    STATLIST_SetUnitStat(pUnit, STAT_DAMAGERESIST, pMonStatsTxtRecord->nResistances[0][nDifficulty], 0);
    STATLIST_SetUnitStat(pUnit, STAT_MAGICRESIST, pMonStatsTxtRecord->nResistances[1][nDifficulty], 0);
    STATLIST_SetUnitStat(pUnit, STAT_FIRERESIST, pMonStatsTxtRecord->nResistances[2][nDifficulty], 0);
    STATLIST_SetUnitStat(pUnit, STAT_LIGHTRESIST, pMonStatsTxtRecord->nResistances[3][nDifficulty], 0);
    STATLIST_SetUnitStat(pUnit, STAT_COLDRESIST, pMonStatsTxtRecord->nResistances[4][nDifficulty], 0);
    STATLIST_SetUnitStat(pUnit, STAT_POISONRESIST, pMonStatsTxtRecord->nResistances[5][nDifficulty], 0);
    STATLIST_SetUnitStat(pUnit, STAT_TOBLOCK, pMonStatsTxtRecord->nToBlock[nDifficulty], 0);
    STATLIST_SetUnitStat(pUnit, STAT_ATTACKRATE, 100, 0);
    STATLIST_SetUnitStat(pUnit, STAT_VELOCITYPERCENT, 75, 0);
    STATLIST_SetUnitStat(pUnit, STAT_OTHER_ANIMRATE, 100, 0);
    STATLIST_SetUnitStat(pUnit, STAT_LAST_SENT_HP_PCT, 128, 0);

    D2MonStatsInitStrc monStatsInit = {};
    DATATBLS_CalculateMonsterStatsByLevel(pUnit->dwClassId, pGame->nGameType || pGame->dwGameType, nDifficulty, nLevel, 7, &monStatsInit);

    const int32_t nBaseHp = monStatsInit.nMinHP + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, monStatsInit.nMaxHP - monStatsInit.nMinHP + 1);
    const int32_t nHp = std::min(nBaseHp + MONSTERUNIQUE_CalculatePercentage(nBaseHp, playerCountBonus.nHp, 100), (1 << 23) - 1);
    const int32_t nShiftedHp = nHp << 8;
    STATLIST_SetUnitStat(pUnit, STAT_MAXHP, nShiftedHp, 0);
    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nShiftedHp, 0);
    STATLIST_SetUnitStat(pUnit, STAT_ARMORCLASS, monStatsInit.nAC, 0);
    STATLIST_SetUnitStat(pUnit, STAT_EXPERIENCE, monStatsInit.nExp + MONSTERUNIQUE_CalculatePercentage(monStatsInit.nExp, playerCountBonus.nExperience, 100), 0);
    STATLIST_SetUnitStat(pUnit, STAT_HPREGEN, (nShiftedHp * pMonStatsTxtRecord->dwDamageRegen) >> 12, 0);

    MONSTERS_ApplyClassicScaling(pUnit, pGame->bExpansion, pGame->nDifficulty);
    
    D2COMMON_10475_PostStatToStatList(pUnit, STATLIST_AllocStatList(pGame->pMemoryPool, 1u, 0, UNIT_MONSTER, pUnit->dwUnitId), 1);

    if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INVENTORY])
    {
        if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT])
        {
            pUnit->pInventory = SUNITPROXY_GetNpcInventory(pGame, nClassId);
        }
        else
        {
            INVENTORY_AllocInventory(nullptr, pUnit);
        }
    }

    pUnit->pSkills = SKILLS_AllocSkillList(pGame->pMemoryPool);
    for (int32_t i = 0; i < 8; ++i)
    {
        if (pMonStatsTxtRecord->nSkill[i] >= 0)
        {
            const uint8_t nSkillLevel = pMonStatsTxtRecord->nSkLvl[i];
            if (nSkillLevel > 0)
            {
                D2GAME_SetSkills_6FD14C60(pUnit, pMonStatsTxtRecord->nSkill[i], playerCountBonus.nMonsterSkillBonus + nSkillLevel, 0);

                const int8_t nSkillMode = pMonStatsTxtRecord->nSkillMode[i];
                if (nSkillMode >= 0)
                {
                    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, pMonStatsTxtRecord->nSkill[i]);
                    SKILLS_SetSkillMode(pSkill, nSkillMode);
                }
            }
        }
    }
    
    //D2COMMON_11062_Return(pUnit, playerCountBonus.nMonsterSkillBonus);
    
    if (pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_ISATT])
    {
        pUnit->dwFlags |= UNITFLAG_CANBEATTACKED;
    }

    if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_PETIGNORE])
    {
        pUnit->dwFlags |= UNITFLAG_PETIGNORE;
    }

    pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    if (pMonStatsTxtRecord)
    {
        D2MonPropTxt* pMonPropTxtRecord = MONSTER_GetMonPropTxtRecord(pMonStatsTxtRecord->wMonProp);
        if (pMonPropTxtRecord)
        {
            for (int32_t i = 0; i < 6; ++i)
            {
                D2PropertyStrc* pProperty = &pMonPropTxtRecord->props[nDifficulty][i];
                if (pProperty->nProperty < 0)
                {
                    break;
                }

                const uint8_t nChance = pMonPropTxtRecord->nChance[nDifficulty][i];
                if (!nChance || ITEMS_RollRandomNumber(&pUnit->pSeed) % 100 < nChance)
                {
                    D2COMMON_11292_ItemAssignProperty(0, nullptr, pUnit, nullptr, 0, 0, pProperty, 0, 0x40, nullptr);
                }
            }
        }
    }

    sub_6FCF9580(pGame, nullptr, pUnit, -1, nLevel, nLevel, 1);
}

//D2Game.0x6FC60B10
int32_t __fastcall MONSTER_SetVelocityAndPosition(D2GameStrc* pGame, D2UnitStrc* pMonster, int32_t nX, int32_t nY, int32_t a5)
{
    int32_t nAnimMode = 0;
    if (pMonster)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pMonster->dwClassId);
        nAnimMode = pMonster->dwAnimMode;
        PATH_SetVelocity(pMonster->pDynamicPath, pMonStatsTxtRecord->nVelocity << 8, __FILE__, __LINE__);
    }

    UNITS_UpdateAnimRateAndVelocity(pMonster, __FILE__, __LINE__);

    D2ModeChangeStrc modeChange = {};
    MONSTERMODE_GetModeChangeInfo(pMonster, nAnimMode, &modeChange);
    modeChange.nX = nX;
    modeChange.nY = nY;
    return D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
}

//D2Game.0x6FC60BC0
void __fastcall MONSTER_RemoveAll(D2GameStrc* pGame)
{
    for (int32_t i = 0; i < 128; ++i)
    {
        D2UnitStrc* pMonster = pGame->pUnitList[UNIT_MONSTER][i];
        while (pMonster)
        {
            D2UnitStrc* pNext = SUNIT_GetNextUnitFromList(pMonster);
            pGame->pUnitList[UNIT_MONSTER][i] = pNext;
            SUNIT_RemoveUnit(pGame, pMonster);
            pMonster = pNext;
        }
    }
}

//D2Game.0x6FC60C10
void __fastcall MONSTER_Free(D2GameStrc* pGame, D2UnitStrc* pMonster)
{
    SUNITDMG_FreeAttackerCombatList(pGame, pMonster);

    if (!pMonster)
    {
        SUNIT_RemoveAllItemsFromInventory(pGame, pMonster);
        INVENTORY_FreeInventory(pMonster->pInventory);
    }
    else
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pMonster->dwClassId);
        if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT]))
        {
            SUNIT_RemoveAllItemsFromInventory(pGame, pMonster);
            INVENTORY_FreeInventory(pMonster->pInventory);
        }
    }

    if (pMonster->pHoverText)
    {
        CHAT_FreeHoverMsg(pGame->pMemoryPool, pMonster->pHoverText);
    }

    if (pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        AIGENERAL_FreeAiControl(pGame, pMonster->pMonsterData->pAiControl);
        D2GAME_MONSTERMODE_FreeParams_6FC63020(pGame, pMonster->pMonsterData->pAiParam);
        MONSTERAI_FreeMonsterInteract(pGame, pMonster->pMonsterData->pMonInteract);
    }

    sub_6FC349F0(pGame, pMonster);
}

//D2Game.0x6FC60CD0
void __fastcall MONSTER_UpdateAiCallbackEvent(D2GameStrc* pGame, D2UnitStrc* pMonster)
{
    D2GAME_EVENTS_Delete_6FC34840(pGame, pMonster, UNITEVENTCALLBACK_AITHINK, 0);

    if (pMonster)
    {
        const int32_t nAnimMode = pMonster->dwAnimMode;
        if (nAnimMode == MONMODE_NEUTRAL)
        {
            D2GAME_EVENTS_Delete_6FC34840(pGame, pMonster, UNITEVENTCALLBACK_AITHINK, 0);
            EVENT_SetEvent(pGame, pMonster, UNITEVENTCALLBACK_AITHINK, pGame->dwGameFrame + 2, 0, 0);
        }
        else if (nAnimMode == MONMODE_DEAD || nAnimMode == MONMODE_DEATH)
        {
            return;
        }

        switch (D2GAME_GetMonsterBaseId_6FC64B10(pMonster))
        {
        case MONSTER_VULTURE1:
        case MONSTER_WILLOWISP1:
        case MONSTER_BATDEMON1:
        case MONSTER_FROGDEMON1:
            D2GAME_EVENTS_Delete_6FC34840(pGame, pMonster, UNITEVENTCALLBACK_AITHINK, 0);
            EVENT_SetEvent(pGame, pMonster, UNITEVENTCALLBACK_AITHINK, pGame->dwGameFrame + 2, 0, 0);
            break;

        default:
            return;
        }
    }
}

//D2Game.0x6FC60E50
void __fastcall MONSTER_DeleteEvents(D2GameStrc* pGame, D2UnitStrc* pMonster)
{
    D2GAME_EVENTS_Delete_6FC34840(pGame, pMonster, UNITEVENTCALLBACK_AITHINK, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pMonster, UNITEVENTCALLBACK_STATREGEN, 0);
}

//D2Game.0x6FC60E70
int32_t __fastcall MONSTER_GetHpBonus(int32_t nPlayerCount)
{
    static const int32_t gnHpBonusTable[] =
    {
        0, 0, 50, 100, 150, 200, 250, 300, 350
    };

    if (nPlayerCount >= 9)
    {
        return 10 * (5 * nPlayerCount - 10);
    }

    return gnHpBonusTable[nPlayerCount];
}

//Inlined in D2Game.0x6FC60E90
int32_t __fastcall MONSTER_GetExperienceBonus(int32_t nPlayerCount)
{
    static const int32_t gnExperienceBonusTable[] =
    {
        0, 0, 50, 100, 150, 200, 250, 300, 350
    };

    if (nPlayerCount >= 9)
    {
        return 2 * (5 * nPlayerCount + 130);
    }

    return gnExperienceBonusTable[nPlayerCount];
}

//D2Game.0x6FC60E90
void __fastcall MONSTER_GetPlayerCountBonus(D2GameStrc* pGame, D2PlayerCountBonusStrc* pPlayerCountBonus, D2ActiveRoomStrc* pRoom, D2UnitStrc* pMonster)
{
    if (!pPlayerCountBonus)
    {
        return;
    }

    memset(pPlayerCountBonus, 0x00, sizeof(*pPlayerCountBonus));
    D2MonsterDataStrc* pMonsterData = MONSTERUNIQUE_GetMonsterData(pMonster);
    if (!pMonsterData || !pMonster->pMonsterData->pMonstatsTxt)
    {
        return;
    }

    if (pMonsterData->pMonstatsTxt->nAlign != MONALIGN_EVIL)
    {
        pPlayerCountBonus->nPlayerCount = 1;
    }
    else
    {
        const int32_t nPlayerCount = std::max(PLAYER_GetPlayerCount(pGame), 1);
        pPlayerCountBonus->nHp = MONSTER_GetHpBonus(nPlayerCount);
        pPlayerCountBonus->nExperience = MONSTER_GetExperienceBonus(nPlayerCount);
        pPlayerCountBonus->nPlayerCount = nPlayerCount;
    }

    D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
    if (pGame->nDifficulty >= 3)
    {
        pGame->nDifficulty = 2;
    }

    pPlayerCountBonus->nMonsterSkillBonus = pDifficultyLevelsTxtRecord->dwMonsterSkillBonus;
    pPlayerCountBonus->nDifficulty = pGame->nDifficulty;
}

//D2Game.0x6FC60F70
void __fastcall MONSTER_SetComponents(D2MonRegDataStrc* pMonRegData, D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2MonsterDataStrc* pMonsterData = pUnit->pMonsterData;
    if (!pMonsterData)
    {
        return;
    }

    if (pMonRegData && pMonRegData->unk0x03 > 0)
    {
        const uint8_t* pComponentData = pMonRegData->unk0x04[ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, pMonRegData->unk0x03)];
        memcpy(pMonsterData->nComponent, pComponentData, sizeof(pMonsterData->nComponent));
    }
    else
    {
        for (int32_t i = 0; i < 12; ++i)
        {
            pMonsterData->nComponent[i] = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, D2COMMON_11068_GetCompInfo(pUnit, i));
        }
    }
}

//D2Game.0x6FC610C0
int32_t __fastcall MONSTER_HasComponents(D2UnitStrc* pMonster)
{
    if (!pMonster || pMonster->dwUnitType != UNIT_MONSTER || !pMonster->pMonsterData)
    {
        return 1;
    }

    for (int32_t i = 0; i < 16; ++i)
    {
        if (pMonster->pMonsterData->nComponent[i])
        {
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC610F0
int32_t __fastcall MONSTER_Reinitialize(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nClassId, int32_t nMode)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
    if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_ENABLED]))
    {
        return 0;
    }

    const int32_t nUnitGUID = pUnit->dwUnitId;
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    MONSTER_Free(pGame, pUnit);

    pUnit->dwClassId = nClassId;
    MONSTER_Initialize(pGame, pRoom, pUnit, nUnitGUID);
    UNITS_ChangeAnimMode(pUnit, nMode);
    return 1;
}
