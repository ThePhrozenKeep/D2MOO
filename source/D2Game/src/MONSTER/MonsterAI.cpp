#include "MONSTER/MonsterAI.h"

#include <algorithm>

#include <Fog.h>
#include <D2BitManip.h>
#include <D2Math.h>

#include "D2PacketDef.h"

#include <D2StatList.h>
#include <D2Monsters.h>
#include <D2Items.h>
#include <D2Dungeon.h>
#include <D2Inventory.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/MonsterIds.h>
#include <D2Text.h>


#include "AI/AiGeneral.h"
#include "AI/AiTactics.h"
#include "GAME/Clients.h"
#include "GAME/SCmd.h"
#include "GAME/Targets.h"
#include "ITEMS/Items.h"
#include "ITEMS/ItemMode.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterUnique.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerPets.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT1/A1Q4.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitMsg.h"
#include "UNIT/SUnitNpc.h"
#include "UNIT/SUnitProxy.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/Skills.h"


//D2Game.0x6FC61190
void __fastcall sub_6FC61190(D2GameStrc* pGame, D2UnitStrc* pMonster, Unk* pItemCode, DWORD dwILvl, DWORD dwQuality)
{
    if (!pMonster || pMonster->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    if (!pMonster->pInventory)
    {
        INVENTORY_AllocInventory(nullptr, pMonster);
    }

    int32_t nItemId = 0;
    if (!DATATBLS_GetItemRecordFromItemCode(pItemCode->nItemCode, &nItemId))
    {
        return;
    }

    D2UnitStrc* pItem = D2GAME_CreateItemUnit_6FC501A0(pMonster, nItemId, pGame, 4u, dwQuality, 1u, 1u, dwILvl, 0, 0, 0);
    if (!pItem)
    {
        return;
    }

    ITEMS_SetItemFlag(pItem, IFLAG_NOSELL, 1);
    ITEMS_SetBodyLocation(pItem, pItemCode->nBodyLoc);
    if (pItemCode->nBodyLoc == BODYLOC_NONE)
    {
        return;
    }

    int32_t nUnused = 0;
    if (sub_6FC45060(pGame, pMonster, pItem->dwUnitId, ITEMS_GetBodyLocation(pItem), 1, &nUnused))
    {
        STATLIST_SetUnitStat(pItem, STAT_DURABILITY, STATLIST_GetMaxDurabilityFromUnit(pItem), 0);
    }
    else
    {
        SUNIT_RemoveUnit(pGame, pItem);
    }
}

//D2Game.0x6FC61270
void __fastcall sub_6FC61270(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pUnit, int16_t nId, D2MercDataStrc* pMercData, int32_t bDead)
{
    if (!pPlayer || !pUnit || !pMercData || pPlayer->dwUnitType != UNIT_PLAYER)
    {
        return;
    }

    pUnit->dwFlags |= UNITFLAG_NOXP | UNITFLAG_NOTC | UNITFLAG_ISMERC;

    sub_6FCBDD30(pUnit, 2u, 0);

    if (!bDead)
    {
        D2GAME_UpdateSummonAI_6FC401F0(pGame, pUnit, 0, pPlayer->dwNodeIndex);
    }

    D2UnitStrc* pPet = sub_6FC7E8B0(pGame, pPlayer, PETTYPE_HIREABLE, 1);
    if (pPet)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        D2GAME_PACKETS_SendPacket0x9B_6FC3FB30(pClient, -1, 0);

        AIGENERAL_SetOwnerData(pGame, pPet, -1, 1, 0, 0);
        sub_6FC7D260(pGame, pPlayer, pPet->dwUnitId, 0);
        DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pPet), UNIT_MONSTER, pPet->dwUnitId);
        SUNIT_RemoveUnit(pGame, pPet);
    }

    D2HirelingInitStrc hirelingInit = {};
    if (!MONSTERS_HirelingInit(pGame->bExpansion, pPlayer, pMercData->dwSeed, MONSTERS_GetActFromHirelingTxt(pGame->bExpansion, 0, pMercData->nMercName), pGame->nDifficulty, &hirelingInit))
    {
        return;
    }

    D2PetInfoStrc petInfo = {};
    petInfo.nSeed = pMercData->dwSeed;
    petInfo.wName = pMercData->nMercName;
    petInfo.nHirelingId = hirelingInit.nId;
    sub_6FC7DBF0(pGame, pPlayer, pUnit, PETTYPE_HIREABLE, &petInfo);

    pUnit->dwFlags |= UNITFLAG_ISREVIVE;
    AIGENERAL_SetOwnerData(pGame, pUnit, pPlayer->dwUnitId, pPlayer->dwUnitType, 0, 0);

    if (nId == -1)
    {
        if (!DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, hirelingInit.nId, hirelingInit.nLevel))
        {
            return;
        }

        STATLIST_SetUnitStat(pUnit, STAT_EXPERIENCE, hirelingInit.nExperience, 0);
        MONSTERAI_UpdateMercStatsAndSkills(pGame, pPlayer, pUnit, hirelingInit.nLevel);
        MONSTERAI_SendMercStats(pGame, pPlayer, 1u);
    }

    D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_UNUSEDMERCMOD, 0);

    if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2MonsterDataStrc* pMonsterData = pUnit->pMonsterData;
        if (pMonsterData)
        {
            pMonsterData->nComponent[0] = 0;
            pMonsterData->nComponent[1] = 0;
            pMonsterData->nComponent[5] = 0;
            pMonsterData->nComponent[6] = 0;
            pMonsterData->nComponent[7] = 0;
        }
    }
}

//D2Game.0x6FC61490
void __fastcall MONSTERAI_SendMercStats(D2GameStrc* pGame, D2UnitStrc* pPlayer, DWORD dwZero)
{
    D2UnitStrc* pMerc = sub_6FC7E8B0(pGame, pPlayer, PETTYPE_HIREABLE, 0);
    if (!pMerc)
    {
        return;
    }

    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_LEVEL, STATLIST_UnitGetStatValue(pMerc, STAT_LEVEL, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_STRENGTH, STATLIST_GetUnitBaseStat(pMerc, STAT_STRENGTH, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_DEXTERITY, STATLIST_GetUnitBaseStat(pMerc, STAT_DEXTERITY, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_MAXHP, STATLIST_GetUnitBaseStat(pMerc, STAT_MAXHP, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_HITPOINTS, STATLIST_UnitGetStatValue(pMerc, STAT_HITPOINTS, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_ARMORCLASS, STATLIST_GetUnitBaseStat(pMerc, STAT_ARMORCLASS, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_EXPERIENCE, STATLIST_UnitGetStatValue(pMerc, STAT_EXPERIENCE, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_NEXTEXP, STATLIST_UnitGetStatValue(pMerc, STAT_NEXTEXP, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_MINDAMAGE, STATLIST_GetUnitBaseStat(pMerc, STAT_MINDAMAGE, 0) + STATLIST_GetUnitBaseStat(pMerc, STAT_SECONDARY_MINDAMAGE, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_MAXDAMAGE, STATLIST_GetUnitBaseStat(pMerc, STAT_MAXDAMAGE, 0) + STATLIST_GetUnitBaseStat(pMerc, STAT_SECONDARY_MAXDAMAGE, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_FIRERESIST, STATLIST_GetUnitBaseStat(pMerc, STAT_FIRERESIST, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_LIGHTRESIST, STATLIST_GetUnitBaseStat(pMerc, STAT_LIGHTRESIST, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_COLDRESIST, STATLIST_GetUnitBaseStat(pMerc, STAT_COLDRESIST, 0));
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_POISONRESIST, STATLIST_GetUnitBaseStat(pMerc, STAT_POISONRESIST, 0));
}

//D2Game.0x6FC61610
void __fastcall MONSTERAI_UpdateMercStatsAndSkills(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pMerc, int32_t nLevel)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pPlayer);
    D2_ASSERT(pPlayer->dwUnitType == UNIT_PLAYER);

    D2UnitStrc* pHireling = pMerc;
    if (!pHireling)
    {
        pHireling = sub_6FC7E8B0(pGame, pPlayer, PETTYPE_HIREABLE, 0);
    }

    if (!pHireling)
    {
        return;
    }

    //STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0);
    if (!nLevel)
    {
        nLevel = STATLIST_UnitGetStatValue(pHireling, STAT_LEVEL, 0) + 1;
    }

    AITACTICS_AddMessage(pGame, pHireling, pPlayer, 3452u, 1);

    STATLIST_SetUnitStat(pHireling, STAT_LEVEL, nLevel, 0);

    D2PetInfoStrc* pPetInfo = PLAYERPETS_GetPetInfoFromPetGUID(pPlayer, pHireling->dwUnitId);
    D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, pPetInfo->nHirelingId, nLevel);
    if (!pHirelingTxtRecord)
    {
        return;
    }

    const int32_t nLevelUps = nLevel - pHirelingTxtRecord->nHirelingLevel;

    int32_t nNextExp = 0;
    if (nLevel < DATATBLS_GetMaxLevel(0) - 1)
    {
        nNextExp = MONSTERS_GetHirelingExpForNextLevel(nLevel + 1, pHirelingTxtRecord->dwExpPerLvl);
    }
    STATLIST_SetUnitStat(pHireling, STAT_NEXTEXP, nNextExp, 0);

    const int32_t nExperience = MONSTERS_GetHirelingExpForNextLevel(nLevel, pHirelingTxtRecord->dwExpPerLvl);
    if (STATLIST_UnitGetStatValue(pHireling, STAT_EXPERIENCE, 0) < nExperience)
    {
        STATLIST_SetUnitStat(pHireling, STAT_EXPERIENCE, nExperience, 0);
    }

    const int32_t nStrength = std::max(pHirelingTxtRecord->nStr + nLevelUps * pHirelingTxtRecord->nStrPerLvl / 8, 10);
    STATLIST_SetUnitStat(pHireling, STAT_STRENGTH, nStrength, 0);

    const int32_t nDexterity = std::max(pHirelingTxtRecord->nDex + nLevelUps * pHirelingTxtRecord->nDexPerLvl / 8, 10);
    STATLIST_SetUnitStat(pHireling, STAT_DEXTERITY, nDexterity, 0);

    const int32_t nHitpoints = std::max((pHirelingTxtRecord->dwHitpoints << 8) + nLevelUps * (pHirelingTxtRecord->dwHitpointsPerLvl << 8), (40u << 8));
    STATLIST_SetUnitStat(pHireling, STAT_MAXHP, nHitpoints, 0);
    STATLIST_SetUnitStat(pHireling, STAT_HITPOINTS, nHitpoints, 0);

    const int32_t nDefense = std::max(pHirelingTxtRecord->dwDefense + nLevelUps * pHirelingTxtRecord->dwDefensePerLvl, 0);
    STATLIST_SetUnitStat(pHireling, STAT_ARMORCLASS, nDefense, 0);

    const int32_t nMinDamage = std::max(pHirelingTxtRecord->nDmgMin + nLevelUps * pHirelingTxtRecord->nDmgPerLvl / 8, 0);
    STATLIST_SetUnitStat(pHireling, STAT_SECONDARY_MINDAMAGE, nMinDamage, 0);

    const int32_t nMaxDamage = std::max(pHirelingTxtRecord->nDmgMax + nLevelUps * pHirelingTxtRecord->nDmgPerLvl / 8, 1);
    STATLIST_SetUnitStat(pHireling, STAT_SECONDARY_MAXDAMAGE, nMaxDamage, 0);

    const int32_t nAttackRate = std::max(pHirelingTxtRecord->dwAttackRate + nLevelUps * pHirelingTxtRecord->dwAttackRatePerLvl, 0);
    STATLIST_SetUnitStat(pHireling, STAT_TOHIT, nAttackRate, 0);

    const int32_t nResist = std::max(pHirelingTxtRecord->dwResist + (nLevelUps * pHirelingTxtRecord->dwResistPerLvl) / 4, 0);
    STATLIST_SetUnitStat(pHireling, STAT_FIRERESIST, nResist, 0);
    STATLIST_SetUnitStat(pHireling, STAT_LIGHTRESIST, nResist, 0);
    STATLIST_SetUnitStat(pHireling, STAT_COLDRESIST, nResist, 0);
    STATLIST_SetUnitStat(pHireling, STAT_POISONRESIST, nResist, 0);

    const int32_t nHpRegen = std::max(STATLIST_GetUnitBaseStat(pHireling, STAT_MAXHP, 0) / 2000, 0);
    STATLIST_SetUnitStat(pHireling, STAT_HPREGEN, nHpRegen, 0);

    for (int32_t i = 0; i < 6; ++i)
    {
        const int32_t nSkillId = pHirelingTxtRecord->dwSkill[i];
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (!pSkillsTxtRecord || pHirelingTxtRecord->nMode[i] >= 16)
        {
            return;
        }

        if (nLevel >= pSkillsTxtRecord->wReqLevel)
        {
            const int32_t nSkillLevel = D2Clamp(pHirelingTxtRecord->nLevel[i] + (nLevelUps * pHirelingTxtRecord->nSLvlPerLvl[i] >> 5), 0, 32);
            if (nSkillLevel > 0)
            {
                D2GAME_SetSkills_6FD14C60(pHireling, nSkillId, nSkillLevel, 0);
            }
        }
    }
}

//D2Game.0x6FC61980
D2MonsterInteractStrc* __fastcall MONSTERAI_AllocMonsterInteract(D2GameStrc* pGame)
{
    D2MonsterInteractStrc* pMonInteract = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2MonsterInteractStrc);
    pMonInteract->pInteractInfo = nullptr;
    return pMonInteract;
}

//D2Game.0x6FC619A0
void __fastcall MONSTERAI_FreeMonsterInteract(D2GameStrc* pGame, D2MonsterInteractStrc* pMonInteract)
{
    D2InteractInfoStrc* pInteractInfo = pMonInteract->pInteractInfo;
    while (pInteractInfo)
    {
        D2InteractInfoStrc* pNext = pInteractInfo->pNext;
        D2_FREE_POOL(pGame->pMemoryPool, pInteractInfo);
        pInteractInfo = pNext;
    }
    D2_FREE_POOL(pGame->pMemoryPool, pMonInteract);
}

//D2Game.0x6FC619F0
int32_t __fastcall MONSTERAI_GetInteractUnitCount(D2MonsterInteractStrc* pMonInteract)
{
    int32_t nResult = 0;

    for (D2InteractInfoStrc* i = pMonInteract->pInteractInfo; i; i = i->pNext)
    {
        if (i->pUnit)
        {
            ++nResult;
        }
    }

    return nResult;
}

//D2Game.0x6FC61A10
int32_t __fastcall MONSTERAI_HasInteractUnit(D2MonsterInteractStrc* pMonInteract)
{   
    for (D2InteractInfoStrc* i = pMonInteract->pInteractInfo; i; i = i->pNext)
    {
        if (i->pUnit)
        {
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC61A30
int32_t __fastcall MONSTERAI_IsInteractingWith(D2MonsterInteractStrc* pMonInteract, D2UnitStrc* pUnit)
{
    for (D2InteractInfoStrc* i = pMonInteract->pInteractInfo; i; i = i->pNext)
    {
        if (i->pUnit == pUnit)
        {
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC61A50
void __fastcall MONSTERAI_RemoveInteractInfoFor(D2GameStrc* pGame, D2UnitStrc* pMonster, D2MonsterInteractStrc* pMonInteract)
{
    if (!pMonInteract)
    {
        return;
    }

    D2InteractInfoStrc* pPrevious = nullptr;
    for (D2InteractInfoStrc* pInteractInfo = pMonInteract->pInteractInfo; pInteractInfo; pInteractInfo = pInteractInfo->pNext)
    {
        if (pInteractInfo->pUnit == pMonster)
        {
            if (pInteractInfo == pMonInteract->pInteractInfo)
            {
                pMonInteract->pInteractInfo = pInteractInfo->pNext;
            }
            else
            {
                pPrevious->pNext = pInteractInfo->pNext;
            }
            D2_FREE_POOL(pGame->pMemoryPool, pInteractInfo);
            return;
        }

        pPrevious = pInteractInfo;
    }
}

//D2Game.0x6FC61AB0
void __fastcall sub_6FC61AB0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pNpc, D2MonsterInteractStrc* pMonInteract)
{
    for (D2InteractInfoStrc* pInteractInfo = pMonInteract->pInteractInfo; pInteractInfo; pInteractInfo = pInteractInfo->pNext)
    {
        if (pInteractInfo->pUnit == pUnit)
        {
            if (!pInteractInfo->nInteract)
            {
                pInteractInfo->nInteract = 1;
                D2GAME_NPC_Heal_6FCCB220(pGame, pUnit, pNpc);
            }
            return;
        }
    }

}

//D2Game.0x6FC61AF0
void __fastcall D2GAME_NPCS_SetInteractTrading_6FC61AF0(D2UnitStrc* pNPC, D2UnitStrc* pPlayer)
{
    if (!pNPC || pNPC->dwUnitType != UNIT_MONSTER || !pNPC->pMonsterData || !pNPC->pMonsterData->pMonInteract)
    {
        return;
    }

    for (D2InteractInfoStrc* pInteractInfo = pNPC->pMonsterData->pMonInteract->pInteractInfo; pInteractInfo; pInteractInfo = pInteractInfo->pNext)
    {
        if (pInteractInfo->pUnit == pPlayer)
        {
            if (pInteractInfo->nInteract == 1)
            {
                pInteractInfo->nInteract = 2;
            }
            return;
        }
    }
}

//D2Game.0x6FC61B30
int32_t __fastcall sub_6FC61B30(D2UnitStrc* pMonster, D2UnitStrc* pPlayer)
{
    if (!pMonster || pMonster->dwUnitType != UNIT_MONSTER || !pMonster->pMonsterData || !pMonster->pMonsterData->pMonInteract)
    {
        return 0;
    }

    for (D2InteractInfoStrc* pInteractInfo = pMonster->pMonsterData->pMonInteract->pInteractInfo; pInteractInfo; pInteractInfo = pInteractInfo->pNext)
    {
        if (pInteractInfo->pUnit == pPlayer)
        {
            return pInteractInfo->nInteract == 2;
        }
    }

    return 0;
}

//D2Game.0x6FC61B70
void __fastcall sub_6FC61B70(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, D2MonsterInteractStrc* pMonInteract)
{
    QUESTS_NPCDeactivate(pGame, pPlayer, pNpc);

    D2InteractInfoStrc* pPrevious = nullptr;
    for (D2InteractInfoStrc* pInteractInfo = pMonInteract->pInteractInfo; pInteractInfo; pInteractInfo = pInteractInfo->pNext)
    {
        if (pInteractInfo->pUnit == pPlayer && pInteractInfo->nInteract >= 0)
        {
            if (pPrevious)
            {
                pPrevious->pNext = pInteractInfo->pNext;
            }
            else
            {
                pMonInteract->pInteractInfo = pInteractInfo->pNext;
            }

            int32_t nUnitType = 0;
            int32_t nUnitGUID = 0;
            if (SUNIT_GetInteractInfo(pPlayer, &nUnitType, &nUnitGUID) && nUnitType == UNIT_MONSTER)
            {
                SUNIT_ResetInteractInfo(pPlayer);
            }

            D2_FREE_POOL(pGame->pMemoryPool, pInteractInfo);

            if (pInteractInfo->nInteract >= 1 && !pMonInteract->pInteractInfo)
            {
                SUNITPROXY_FreeNpcGamble(pGame, pNpc, pPlayer);
            }
            return;
        }

        pPrevious = pInteractInfo;
    }
}

//D2Game.0x6FC61C70
void __fastcall sub_6FC61C70(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2MonsterInteractStrc* pMonInteract, char a5)
{
    if (SUNIT_GetInteractUnit(pGame, pUnit))
    {
        return;
    }

    if (!pTarget || pTarget->dwAnimMode == MONMODE_DEATH || pTarget->dwAnimMode == MONMODE_DEAD || PLAYER_IsBusy(pUnit) == 1)
    {
        return;
    }

    int32_t nClassId = pTarget->dwClassId;
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
    if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT]) || (nClassId == MONSTER_CAIN1 && ACT1Q4_IsCainInTristramDeactivated(pGame)))
    {
        return;
    }

    for (D2InteractInfoStrc* i = pMonInteract->pInteractInfo; i; i = i->pNext)
    {
        if (i->pUnit == pUnit)
        {
            return;
        }
    }

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    D2InteractInfoStrc* pInteractInfo = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2InteractInfoStrc);
    pInteractInfo->pUnit = pUnit;

    int32_t bNoInteractInfo = 0;
    if (!pMonInteract->pInteractInfo)
    {
        bNoInteractInfo = 1;
    }

    pInteractInfo->pNext = pMonInteract->pInteractInfo;
    pMonInteract->pInteractInfo = pInteractInfo;

    D2GAME_NPC_BuildHirelingList_6FCC6FF0(CLIENTS_GetGame(pClient), pClient, pTarget, bNoInteractInfo);
    SUNIT_SetInteractInfo(pUnit, UNIT_MONSTER, pTarget->dwUnitId);
    pInteractInfo->nInteract = 0;

    D2TextHeaderStrc* pTextHeader = TEXT_AllocTextHeader(pGame->pMemoryPool);
    QUESTS_NPCActivate(pClient, pUnit, pTarget, pTextHeader);

    D2GSPacketSrv27 packet27 = {};
    packet27.nHeader = 0x27u;
    packet27.nUnitType = pTarget->dwUnitType;
    packet27.dwUnitGUID = pTarget->dwUnitId;
    TEXT_CreateMessageListFromTextHeader(pTextHeader, &packet27.pMessageList);

    D2GAME_PACKETS_SendPacket0x27_6FC3DF20(pClient, &packet27);

    TEXT_FreeTextHeader(pTextHeader);

    QUESTS_SendCurrentFlags(pGame, pClient);

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    D2GAME_SendPacket0x28_6FC3F2F0(pClient, 0x28u, UNIT_MONSTER, pTarget->dwUnitId, pPlayerData->pQuestData[pGame->nDifficulty], a5);
}

//D2Game.0x6FC61E30
int32_t __fastcall sub_6FC61E30(D2UnitStrc* pUnit, int32_t a2, int32_t a3)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || a2 >= 5)
    {
        return 0;
    }

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
    if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_SWITCHAI] && pUnit->pMonsterData)
    {
        pUnit->pMonsterData->unk0x38 = a2;
        pUnit->pMonsterData->unk0x34 = a3;
        return 1;
    }

    return 0;
}

//D2Game.0x6FC61EC0
int32_t __fastcall sub_6FC61EC0(D2UnitStrc* pMonster)
{
    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        return pMonster->pMonsterData->unk0x38;
    }

    return 0;
}

//D2Game.0x6FC61EE0
int32_t __fastcall sub_6FC61EE0(D2UnitStrc* pMonster)
{
    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        return pMonster->pMonsterData->unk0x34;
    }

    return 0;
}

//D2Game.0x6FC61F00
void __fastcall sub_6FC61F00(D2UnitStrc* pMonster)
{
    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        pMonster->pMonsterData->unk0x38 = 0;
        pMonster->pMonsterData->unk0x34 = 0;
    }
}

//D2Game.0xFC61F20
int32_t __fastcall sub_6FC61F20(D2UnitStrc* pMonster, D2UnitStrc* pUnit)
{
    if (!pMonster || pMonster->dwUnitType != UNIT_MONSTER || !pMonster->pMonsterData || !pMonster->pMonsterData->pMonInteract)
    {
        return 0;
    }

    for (D2InteractInfoStrc* pInteractInfo = pMonster->pMonsterData->pMonInteract->pInteractInfo; pInteractInfo; pInteractInfo = pInteractInfo->pNext)
    {
        if (pInteractInfo->pUnit == pUnit)
        {
            return pInteractInfo->nInteract >= 1;
        }
    }

    return 1;
}

//D2Game.0x6FC61F70
void __fastcall D2GAME_MONSTERAI_Last_6FC61F70(D2GameStrc* pGame, D2UnitStrc* pMonster, void(__fastcall* pCallback)(D2UnitStrc*))
{
    if (!pMonster || pMonster->dwUnitType != UNIT_MONSTER || !pMonster->pMonsterData)
    {
        return;
    }

    D2MonsterInteractStrc* pMonInteract = pMonster->pMonsterData->pMonInteract;
    if (!pMonInteract)
    {
        return;
    }

    D2InteractInfoStrc* pInteractInfo = pMonInteract->pInteractInfo;
    while (pInteractInfo)
    {
        D2InteractInfoStrc* pNext = pInteractInfo->pNext;
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pInteractInfo->pUnit, __FILE__, __LINE__);
        sub_6FC3F340(pClient, 0x62u, UNIT_MONSTER, pMonster->dwUnitId, 0);
        SUNIT_ResetInteractInfo(pInteractInfo->pUnit);

        if (pCallback)
        {
            pCallback(pInteractInfo->pUnit);
        }

        D2_FREE_POOL(pGame->pMemoryPool, pInteractInfo);
        pInteractInfo = pNext;
    }
    pMonInteract->pInteractInfo = nullptr;
}
