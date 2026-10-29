#include "PLAYER/PlayerPets.h"

#include <algorithm>

#include <Fog.h>
#include <D2BitManip.h>

#include <D2Collision.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Monsters.h>
#include <D2Skills.h>
#include <Path/PathMisc.h>
#include <Units/UnitRoom.h>

#include "GAME/Clients.h"
#include "GAME/SCmd.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterMode.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"


//D2Game.0x6FC7CAF0
//1.14d: 0x00575AF0
void __fastcall PLAYERPETS_AllocPetList(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    if (pPlayerData->pPlayerPets)
    {
        PLAYERPETS_FreePetsFromPlayerData(pGame, pPlayer);
    }

    pPlayerData->pPlayerPets = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2PlayerPetStrc);
    pPlayerData->pPlayerPets->pPetList = (D2PetListStrc*)D2_CALLOC_POOL(pGame->pMemoryPool, sizeof(D2PetListStrc) * sgptDataTables->nPetTypeTxtRecordCount);
    
    for (int32_t nPetType = 0; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetTypeTxt* pPetTypeTxtRecord = &sgptDataTables->pPetTypeTxt[nPetType];
        if (pPetTypeTxtRecord)
        {
            PLAYERPETS_SetPetLimit(pGame, pPlayer, nPetType, pPetTypeTxtRecord->wBaseMax);
        }
    }
}

//D2Game.0x6FC7CD10
//1.14d: 0x00575BC0
void __fastcall D2GAME_KillPlayerPets_6FC7CD10(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    D2PlayerPetStrc* pPlayerPets = pPlayerData->pPlayerPets;
    if (!pPlayerPets)
    {
        return;
    }

    for (int32_t nPetType = 1; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerPets, nPetType);
        PLAYERPETS_ClearPetList(pGame, pPlayer, pPetList, nPetType == PETTYPE_HIREABLE, nPetType == PETTYPE_HIREABLE);
    }

    PLAYERPETS_UpdatePetLimits(pGame, pPlayer);
}

//D2Game.0x6FC7CDC0
//1.14d: 0x00574570
void __fastcall PLAYERPETS_ClearPetList(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2PetListStrc* pPetList, int32_t bIsHirelingList, int32_t bKeepHireling)
{
    D2PetDataStrc* pPetData = pPetList->pPetData;
    while (pPetData)
    {
        D2PetDataStrc* pNext = pPetData->pNext;
        if (!bIsHirelingList)
        {
            PLAYERPETS_KillPet(pGame, pPetData->nUnitGUID);
            --pPetList->nCurrent;
            --pPetList->nMax;
            D2_FREE_POOL(pGame->pMemoryPool, pPetData);
        }
        else
        {
            D2UnitStrc* pPet = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pPetData->nUnitGUID);
            if (pPet)
            {
                D2PetArgStrc petArg = {};
                petArg.nPetGUID = pPetData->nUnitGUID;
                SUNIT_IterateLivingPlayers(pGame, PLAYERPETS_PlayerIterate_SynchronizeWithClient, &petArg);
                DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pPet), UNIT_MONSTER, pPet->dwUnitId);

                if (bKeepHireling)
                {
                    if (!(pPetData->nFlags & PETFLAG_DEAD))
                    {
                        pPetData->nFlags |= PETFLAG_DEAD;

                        pPetList->pPetData = pPetData;
                        if (UNITS_GetRoom(pPet))
                        {
                            D2ModeChangeStrc pModeChange = {};
                            MONSTERMODE_GetModeChangeInfo(pPet, MONMODE_DEATH, &pModeChange);
                            pModeChange.pTargetUnit = SUNIT_GetOwner(pGame, pPet);
                            D2GAME_ModeChange_6FC65220(pGame, &pModeChange, 1);
                        }

                        D2GAME_PACKETS_SendPacket0x9B_6FC3FB30(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), pPetData->petInfo.wName, MONSTERS_GetHirelingResurrectionCost(pPet));
                    }
                }
                else
                {
                    SUNIT_RemoveUnit(pGame, pPet);
                }
            }

            if (!bKeepHireling)
            {
                --pPetList->nCurrent;
                --pPetList->nMax;
                D2_FREE_POOL(pGame->pMemoryPool, pPetData);
            }
        }

        pPetData = pNext;
    }

    if (!pPetList->nCurrent)
    {
        pPetList->pPetData = nullptr;
    }
}

//D2Game.0x6FC7CF20
//1.14d: 0x00574410
void __fastcall PLAYERPETS_PlayerIterate_SynchronizeWithClient(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg)
{
    D2PetArgStrc* pPetArg = (D2PetArgStrc*)pArg;
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket0x7A_6FC3E100(pClient, 0, pPetArg->nPetType, pPetArg->nPetGUID, pPetArg->nUnitGUID, pPetArg->nPetClass);
}

//D2Game.0x6FC7CF50
//1.14d: 0x00574450
void __fastcall PLAYERPETS_KillPet(D2GameStrc* pGame, int32_t nPetGUID)
{
    D2UnitStrc* pPet = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nPetGUID);
    if (!pPet)
    {
        return;
    }

    if (pPet->dwFlagEx & UNITFLAGEX_HASBEENDELETED)
    {
        FOG_DisplayWarning("!hPet", __FILE__, __LINE__);
        SUNIT_RemoveUnit(pGame, pPet);
    }
    else
    {
        pPet->dwFlags |= UNITFLAG_NOXP;

        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pPet->dwClassId);
        if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_KILLABLE])
        {
            SUNITDMG_KillMonster(pGame, pPet, nullptr, 0);
        }
        else
        {
            D2ModeChangeStrc pModeChange = {};
            MONSTERMODE_GetModeChangeInfo(pPet, MONMODE_DEATH, &pModeChange);
            pModeChange.pTargetUnit = SUNIT_GetOwner(pGame, pPet);
            D2GAME_ModeChange_6FC65220(pGame, &pModeChange, 1);
        }
    }

    D2PetArgStrc arg = {};
    arg.nPetGUID = nPetGUID;
    SUNIT_IterateLivingPlayers(pGame, PLAYERPETS_PlayerIterate_SynchronizeWithClient, &arg);
}

//D2Game.0x6FC7D060
//1.14d: 0x005746D0
void __fastcall PLAYERPETS_FreePetsFromPlayerData(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    D2_ASSERT(pPlayerData);

    D2PlayerPetStrc* pPlayerPets = pPlayerData->pPlayerPets;
    if (!pPlayerPets)
    {
        return;
    }

    for (int32_t nPetType = 1; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerPets, nPetType);
        PLAYERPETS_ClearPetList(pGame, pUnit, pPetList, nPetType == PETTYPE_HIREABLE, 0);
    }

    D2_FREE_POOL(pGame->pMemoryPool, pPlayerPets->pPetList);
    D2_FREE_POOL(pGame->pMemoryPool, pPlayerPets);
    pPlayerData->pPlayerPets = nullptr;
}

//D2Game.0x6FC7D150
//1.14d: 0x00575850
void __fastcall PLAYERPETS_SetPetLimit(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPetType, int32_t nBaseMax)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    if (nPetType == PETTYPE_SINGLE && nBaseMax != 1 || !pPlayerData->pPlayerPets)
    {
        return;
    }

    D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerData->pPlayerPets, nPetType);
    if (!pPetList)
    {
        return;
    }

    pPetList->nMax = nBaseMax;

    for (int32_t nCurrent = pPetList->nCurrent; nCurrent > nBaseMax && nCurrent > 0 && nPetType != PETTYPE_HIREABLE; nCurrent = pPetList->nCurrent)
    {
        PLAYERPETS_RemovePet(pGame, pPlayer, pPetList->pPetData ? pPetList->pPetData->nUnitGUID : -1, 1);
    }
}

//D2Game.0x6FC7D260
//1.14d: 0x005750E0
void __fastcall PLAYERPETS_RemovePet(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPetGUID, int32_t bKillPet)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    D2PlayerPetStrc* pPlayerPets = pPlayerData->pPlayerPets;
    if (!pPlayerPets)
    {
        return;
    }

    const int32_t nPetType = PLAYERPETS_GetPetTypeFromPetGUID(pPlayer, nPetGUID);

    if (nPetType == PETTYPE_NONE && bKillPet)
    {
        PLAYERPETS_KillPet(pGame, nPetGUID);
    }

    D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerPets, nPetType);
    if (pPetList)
    {
        PLAYERPETS_RemovePetFromList(pGame, pPetList, nPetGUID, bKillPet);

        D2PetArgStrc arg = {};
        arg.nPetGUID = nPetGUID;
        SUNIT_IterateLivingPlayers(pGame, PLAYERPETS_PlayerIterate_SynchronizeWithClient, &arg);
    }
}

//D2Game.0x6FC7D390
//1.14d: 0x00574850
void __fastcall PLAYERPETS_RemovePetFromList(D2GameStrc* pGame, D2PetListStrc* pPetList, int32_t nUnitGUID, int32_t bKillPet)
{
    D2PetDataStrc* pPrevious = nullptr;
    for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
    {
        if (pPetData->nUnitGUID == nUnitGUID)
        {
            if (pPrevious)
            {
                pPrevious->pNext = pPetData->pNext;
            }
            else
            {
                pPetList->pPetData = pPetData->pNext;
            }

            D2PetArgStrc arg = {};
            arg.nPetGUID = nUnitGUID;
            SUNIT_IterateLivingPlayers(pGame, PLAYERPETS_PlayerIterate_SynchronizeWithClient, &arg);

            D2UnitStrc* pPet = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pPetData->nUnitGUID);
            if (pPet)
            {
                if (bKillPet)
                {
                    PLAYERPETS_KillPet(pGame, pPetData->nUnitGUID);
                }
                else
                {
                    pPet->dwFlags &= 0x7FFFFFFFu;
                }
            }

            D2_FREE_POOL(pGame->pMemoryPool, pPetData);

            --pPetList->nCurrent;
            if (pPetList->nCurrent < 0)
            {
                FOG_DisplayAssert("Too many pets removed from list!", __FILE__, __LINE__);
                exit(-1);
            }

            break;
        }

        pPrevious = pPetData;
    }
}

//D2Game.0x6FC7D470
//1.14d: 0x005751A0
void __fastcall PLAYERPETS_HandlePetDeath(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    int32_t nPetGUID = -1;
    if (pPet)
    {
        nPetGUID = pPet->dwUnitId;
    }

    D2PlayerPetStrc* pPlayerPets = pPlayerData->pPlayerPets;
    if (!pPlayerPets)
    {
        return;
    }

    const int32_t nPetType = PLAYERPETS_GetPetTypeFromPetGUID(pPlayer, nPetGUID);

    if (nPetType != PETTYPE_HIREABLE)
    {
        PLAYERPETS_RemovePet(pGame, pPlayer, nPetGUID, 0);
    }

    D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerPets, nPetType);
    if (!pPetList)
    {
        return;
    }

    for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
    {
        if (pPetData->nUnitGUID == nPetGUID)
        {
            pPetData->nFlags |= PETFLAG_DEAD;
            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
            D2GAME_PACKETS_SendPacket0x9B_6FC3FB30(pClient, pPetData->petInfo.wName, MONSTERS_GetHirelingResurrectionCost(pPet));
            break;
        }
    }

    D2PetArgStrc arg = {};
    arg.nPetGUID = nPetGUID;
    SUNIT_IterateLivingPlayers(pGame, PLAYERPETS_PlayerIterate_SynchronizeWithClient, &arg);
}

//D2Game.0x6FC7D5F0
//1.14d: 0x005752B0
int32_t __fastcall PLAYERPETS_HandlePetDeactivation(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    int32_t nPetGUID = -1;
    if (pPet)
    {
        nPetGUID = pPet->dwUnitId;
    }

    D2PlayerPetStrc* pPlayerPets = pPlayerData->pPlayerPets;
    if (!pPlayerPets)
    {
        return 0;
    }

    const int32_t nPetType = PLAYERPETS_GetPetTypeFromPetGUID(pPlayer, nPetGUID);

    if (nPetType != PETTYPE_HIREABLE)
    {
        PLAYERPETS_RemovePet(pGame, pPlayer, nPetGUID, 0);
    }

    D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerPets, nPetType);
    if (!pPetList)
    {
        return 0;
    }

    for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
    {
        if (pPetData->nUnitGUID == nPetGUID)
        {
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC7D720
//1.14d: 0x00574930
void __fastcall PLAYERPETS_PlayerIterate_UpdateClient(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg)
{
    D2PetArgStrc* pPetArg = (D2PetArgStrc*)pArg;

    if (pPetArg->nSeed || pPetArg->wName)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        D2GAME_PACKETS_SendPacket0x81_6FC3E160(pClient, 1, pPetArg->nPetType, pPetArg->nPetGUID, pPetArg->nUnitGUID, pPetArg->nPetClass, pPetArg->nSeed, pPetArg->wName);
    }
    else
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        D2GAME_PACKETS_SendPacket0x7A_6FC3E100(pClient, 1, pPetArg->nPetType, pPetArg->nPetGUID, pPetArg->nUnitGUID, pPetArg->nPetClass);
    }
}

//D2Game.0x6FC7D7A0
//1.14d: 0x00575D90
void __fastcall PLAYERPETS_AddSummonedPet(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet, int32_t nPetType, int32_t nPetMax)
{
    if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER)
    {
        return;
    }

    D2PetTypeTxt* pPetTypeTxtRecord = DATATBLS_GetPetTypeTxtRecord(nPetType);
    if (pPetTypeTxtRecord && pPetTypeTxtRecord->wGroup > 0)
    {
        for (int32_t nOtherPetType = 0; nOtherPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nOtherPetType)
        {
            if (nOtherPetType != nPetType)
            {
                D2PetTypeTxt* pOtherPetTypeTxtRecord = DATATBLS_GetPetTypeTxtRecord(nOtherPetType);
                if (pOtherPetTypeTxtRecord && pOtherPetTypeTxtRecord->wGroup == pPetTypeTxtRecord->wGroup)
                {
                    for (D2UnitStrc* pOtherPet = PLAYERPETS_GetFirstPetByType(pGame, pPlayer, nOtherPetType, 1); pOtherPet; pOtherPet = PLAYERPETS_GetFirstPetByType(pGame, pPlayer, nOtherPetType, 1))
                    {
                        PLAYERPETS_RemovePet(pGame, pPlayer, pOtherPet->dwUnitId, 1);
                    }
                }
            }
        }
    }

    PLAYERPETS_SetPetLimit(pGame, pPlayer, nPetType, nPetMax);

    D2PetInfoStrc petInfo = {};
    PLAYERPETS_AddPetWithInfo(pGame, pPlayer, pPet, nPetType, &petInfo);
}

//D2Game.0x6FC7D9D0
//1.14d: 0x005747B0
int32_t __fastcall PLAYERPETS_GetFirstPetGUID(D2UnitStrc* pPlayer, int32_t nPetType)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    if (!pPlayerData->pPlayerPets)
    {
        return -1;
    }

    D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerData->pPlayerPets, nPetType);
    if (pPetList && pPetList->pPetData)
    {
        return pPetList->pPetData->nUnitGUID;
    }

    return -1;
}

//D2Game.0x6FC7DA40
//1.14d: 0x00575C70
int32_t __fastcall PLAYERPETS_AppendPet(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2PetListStrc* pPetList, D2UnitStrc* pPet, D2PetInfoStrc* pPetInfo)
{
    if (!pPetList->nMax)
    {
        PLAYERPETS_UpdatePetLimits(pGame, pPlayer);
    }

    if (!pPetList->nMax)
    {
        PLAYERPETS_KillPet(pGame, pPet ? pPet->dwUnitId : -1);
        return 0;
    }

    if (pPetList->nCurrent == pPetList->nMax)
    {
        D2_ASSERT(pPetList->pPetData);

        PLAYERPETS_RemovePetFromList(pGame, pPetList, pPetList->pPetData->nUnitGUID, 1);
        D2_ASSERT(pPetList->nCurrent != pPetList->nMax);
    }

    D2PetDataStrc* pPrevious = pPetList->pPetData;
    if (pPetList->pPetData)
    {
        for (D2PetDataStrc* i = pPrevious->pNext; i; i = i->pNext)
        {
            pPrevious = i;
        }
    }

    D2PetDataStrc* pPetData = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2PetDataStrc);

    if (pPet)
    {
        pPetData->nUnitGUID = pPet->dwUnitId;
    }
    else
    {
        pPetData->nUnitGUID = -1;
    }

    pPetData->petInfo.nSeed = pPetInfo->nSeed;
    pPetData->petInfo.wName = pPetInfo->wName;
    pPetData->petInfo.unk0x06 = pPetInfo->unk0x06;
    pPetData->petInfo.nHirelingId = pPetInfo->nHirelingId;

    if (pPrevious)
    {
        pPrevious->pNext = pPetData;
    }
    else
    {
        pPetList->pPetData = pPetData;
    }

    ++pPetList->nCurrent;
    D2_ASSERT(pPetList->nCurrent <= pPetList->nMax);
    return 1;
}

//D2Game.0x6FC7DB90
//1.14d: 0x005749B0
void __fastcall PLAYERPETS_UpdatePetInfo(D2UnitStrc* pUnit, int32_t nPetType, int32_t nUnitGUID, D2PetInfoStrc* pPetInfo)
{
    D2PlayerPetStrc* pPlayerPets = UNITS_GetPlayerData(pUnit)->pPlayerPets;
    D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerPets, nPetType);
    if (!pPetList)
    {
        return;
    }

    for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
    {
        if (pPetData->nUnitGUID == nUnitGUID)
        {
            pPetData->petInfo.nSeed = pPetInfo->nSeed;
            pPetData->petInfo.wName = pPetInfo->wName;
            pPetData->petInfo.unk0x06 = pPetInfo->unk0x06;
            pPetData->petInfo.nHirelingId = pPetInfo->nHirelingId;
        }
    }
}

//D2Game.0x6FC7DBF0
//1.14d: 0x00575E90
void __fastcall PLAYERPETS_AddPetWithInfo(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet, int32_t nPetType, D2PetInfoStrc* pPetInfo)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerData->pPlayerPets, nPetType);
    if (!pPetList)
    {
        return;
    }

    D2PetInfoStrc petInfo = {};
    petInfo.nSeed = pPetInfo->nSeed;
    petInfo.wName = pPetInfo->wName;
    petInfo.unk0x06 = pPetInfo->unk0x06;
    petInfo.nHirelingId = pPetInfo->nHirelingId;

    if (PLAYERPETS_AppendPet(pGame, pPlayer, pPetList, pPet, &petInfo))
    {
        D2PetArgStrc pArg = {};
        pArg.nPetType = nPetType;
        if (pPet)
        {
            pArg.nPetGUID = pPet->dwUnitId;
            pArg.nPetClass = pPet->dwClassId;
        }
        else
        {
            pArg.nPetGUID = -1;
            pArg.nPetClass = -1;
        }

        if (pPlayer)
        {
            pArg.nUnitGUID = pPlayer->dwUnitId;
        }
        else
        {
            pArg.nUnitGUID = -1;
        }

        pArg.nSeed = pPetInfo->nSeed;
        pArg.wName = pPetInfo->wName;
        pArg.unk0x16 = pPetInfo->unk0x06;

        SUNIT_IterateLivingPlayers(pGame, PLAYERPETS_PlayerIterate_UpdateClient, &pArg);
    }
}

//D2Game.0x6FC7DD10
//1.14d: 0x00574A20
int32_t __fastcall PLAYERPETS_GetPetTypeFromPetGUID(D2UnitStrc* pPlayer, int32_t nPetGUID)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    if (!pPlayerData->pPlayerPets)
    {
        return 0;
    }

    for (int32_t nPetType = 1; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerData->pPlayerPets, nPetType);
        if (pPetList)
        {
            for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
            {
                if (pPetData->nUnitGUID == nPetGUID)
                {
                    return nPetType;
                }
            }
        }
    }

    return 0;
}

//D2Game.0x6FC7DD90
//1.14d: 0x00574AB0
int32_t __fastcall PLAYERPETS_MarkPetAlive(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    if (!pPlayerData->pPlayerPets)
    {
        return 0;
    }

    for (int32_t nPetType = 1; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerData->pPlayerPets, nPetType);
        if (pPetList)
        {
            for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
            {
                if (pPetData->nUnitGUID == (pPet ? pPet->dwUnitId : -1))
                {
                    pPetData->nFlags &= ~PETFLAG_DEAD;

                    D2PetArgStrc arg = {};

                    arg.nPetType = nPetType;
                    arg.nPetGUID = pPetData->nUnitGUID;

                    if (pPet)
                    {
                        arg.nPetClass = pPet->dwClassId;
                    }
                    else
                    {
                        arg.nPetClass = -1;
                    }

                    if (pPlayer)
                    {
                        arg.nUnitGUID = pPlayer->dwUnitId;
                    }
                    else
                    {
                        arg.nUnitGUID = -1;
                    }

                    arg.nSeed = pPetData->petInfo.nSeed;
                    arg.wName = pPetData->petInfo.wName;
                    arg.unk0x16 = pPetData->petInfo.unk0x06;
                    SUNIT_IterateLivingPlayers(pGame, PLAYERPETS_PlayerIterate_UpdateClient, &arg);

                    return 1;
                }
            }
        }
    }

    return 0;
}

//D2Game.0x6FC7DEB0
//1.14d: 0x00574BD0
D2PetInfoStrc* __fastcall PLAYERPETS_GetPetInfoFromPetGUID(D2UnitStrc* pUnit, int32_t nUnitGUID)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    D2_ASSERT(pPlayerData);

    if (!pPlayerData->pPlayerPets)
    {
        return nullptr;
    }

    for (int32_t nPetType = 1; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerData->pPlayerPets, nPetType);
        if (pPetList)
        {
            for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
            {
                if (pPetData->nUnitGUID == nUnitGUID)
                {
                    return &pPetData->petInfo;
                }
            }
        }
    }

    return nullptr;
}

//D2Game.0x6FC7DF40
//1.14d: 0x00575900
void __fastcall PLAYERPETS_UpdatePetLimits(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER)
    {
        return;
    }

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (!pPlayerData || !pPlayerData->pPlayerPets || !pPlayer->pSkills)
    {
        return;
    }

    int32_t nPetCounters[256] = {};

    D2SkillStrc* pSkill = SKILLS_GetFirstSkillFromSkillList(pPlayer->pSkills);
    while (pSkill)
    {
        D2SkillStrc* pNextSkill = SKILLS_GetNextSkill(pSkill);

        const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
        const int32_t nLevel = SKILLS_GetSkillLevel(pPlayer, pSkill, 1);

        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (pSkillsTxtRecord)
        {
            const int32_t nPetType = pSkillsTxtRecord->nPetType;
            if (nPetType > 0 && nPetType < sgptDataTables->nPetTypeTxtRecordCount)
            {
                const int32_t nPetMax = std::max(SKILLS_EvaluateSkillFormula(pPlayer, pSkillsTxtRecord->dwPetMax, nSkillId, nLevel), 1);

                if (nPetMax > nPetCounters[nPetType])
                {
                    nPetCounters[nPetType] = nPetMax;

                    PLAYERPETS_SetPetLimit(pGame, pPlayer, nPetType, nPetMax);
                }
            }
        }

        pSkill = pNextSkill;
    }

    for (int32_t nPetType = 0; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        if (nPetCounters[nPetType] <= 0)
        {
            D2PetTypeTxt* pPetTypeTxtRecord = &sgptDataTables->pPetTypeTxt[nPetType];
            if (pPetTypeTxtRecord)
            {
                PLAYERPETS_SetPetLimit(pGame, pPlayer, nPetType, pPetTypeTxtRecord->wBaseMax);
            }
        }
    }
}

//D2Game.0x6FC7E2E0
//1.14d: 0x00574540
D2PetListStrc* __fastcall PLAYERPETS_GetPetListFromPetType(D2PlayerPetStrc* pPlayerPets, int32_t nPetType)
{
    if (nPetType >= 0 && nPetType < sgptDataTables->nPetTypeTxtRecordCount)
    {
        return &pPlayerPets->pPetList[nPetType];
    }

    return nullptr;
}

//D2Game.0x6FC7E310
//1.14d: 0x005754B0
void __fastcall PLAYERPETS_UpdatePetsForOwnerMove(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nX, int32_t nY)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (!pPlayerData->pPlayerPets)
    {
        return;
    }

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayer);

    for (int32_t nPetType = 1; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerData->pPlayerPets, nPetType);
        if (pPetList)
        {
            D2PetTypeTxt* pPetTypeTxtRecord = &sgptDataTables->pPetTypeTxt[nPetType];
            if (pPetTypeTxtRecord)
            {
                if (pPetTypeTxtRecord->dwPetFlags & gdwBitMasks[0])
                {
                    for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
                    {
                        if (!(pPetData->nFlags & PETFLAG_DEAD))
                        {
                            D2UnitStrc* pPet = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pPetData->nUnitGUID);
                            if (pPet)
                            {
                                PLAYERPETS_RelocatePet(pGame, pPet, pPetData, pRoom, nX, nY);
                            }
                        }
                    }
                }
                else if (pPetTypeTxtRecord->dwPetFlags & gdwBitMasks[1])
                {
                    D2PetDataStrc* pPetData = pPetList->pPetData;
                    while (pPetData)
                    {
                        D2PetDataStrc* pNext = pPetData->pNext;
                        if (!(pPetData->nFlags & PETFLAG_DEAD))
                        {
                            D2UnitStrc* pPet = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pPetData->nUnitGUID);
                            if (pPet && PATH_ComputeSquaredDistance(CLIENTS_GetUnitX(pPlayer), CLIENTS_GetUnitY(pPlayer), CLIENTS_GetUnitX(pPet), CLIENTS_GetUnitY(pPet)) > 1600)
                            {
                                PLAYERPETS_RemovePet(pGame, pPlayer, pPetData->nUnitGUID, 1);
                            }
                        }
                        pPetData = pNext;
                    }
                }
                else
                {
                    D2PetDataStrc* pPetData = pPetList->pPetData;
                    while (pPetData)
                    {
                        D2PetDataStrc* pNext = pPetData->pNext;
                        PLAYERPETS_KillPet(pGame, pPetData->nUnitGUID);
                        D2_FREE_POOL(pGame->pMemoryPool, pPetData);
                        pPetData = pNext;
                    }

                    pPetList->pPetData = nullptr;
                    pPetList->nCurrent = 0;
                }
            }
        }
    }
}

//D2Game.0x6FC7E550
//1.14d: 0x00574CC0
void __fastcall PLAYERPETS_RelocatePet(D2GameStrc* pGame, D2UnitStrc* pPet, D2PetDataStrc* pPetData, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY)
{
    D2ActiveRoomStrc* pPreviousRoom = UNITS_GetRoom(pPet);
    if (pRoom)
    {
        if (pPet->nAct != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom)))
        {
            D2Common_10229(pPet->pDynamicPath, pPet, 0, 0, 0);
        }
    }

    D2Common_10229(pPet->pDynamicPath, pPet, pRoom, nX, nY);

    if (pRoom != pPreviousRoom)
    {
        UNITS_SetCurrentRoomInvalid(pPet, 1);
        SUNIT_SynchronizeRoomClients(pGame, pPet, pPreviousRoom);
    }

    pPet->nAct = DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pPet)));
    pPet->pDrlgAct = pGame->pAct[pPet->nAct];
    UNITROOM_RefreshUnit(pPet);

    pPet->dwFlagEx |= 0x10000u;
    UNITROOM_RefreshUnit(pPet);

    MONSTER_UpdateAiCallbackEvent(pGame, pPet);
    if (!MONSTERS_IsDead(pPet) && pPet->pDynamicPath)
    {
        PATH_SetFootprintCollisionMask(pPet->pDynamicPath, COLLIDE_MONSTER);
    }
}

//D2Game.0x6FC7E640
//1.14d: 0x00575630
void __fastcall PLAYERPETS_UpdatePetStates(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2PlayerPetStrc* pPlayerPets = UNITS_GetPlayerData(pPlayer)->pPlayerPets;
    if (!pPlayerPets)
    {
        return;
    }

    for (int32_t nPetType = 1; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerPets, nPetType);
        if (pPetList && nPetType != PETTYPE_HIREABLE)
        {
            D2PetDataStrc* pPetData = pPetList->pPetData;
            while (pPetData)
            {
                D2PetDataStrc* pNext = pPetData->pNext;
                D2UnitStrc* pPet = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pPetData->nUnitGUID);
                if (!pPet || SUNIT_IsDead(pPet))
                {
                    PLAYERPETS_RemovePet(pGame, pPlayer, pPetData->nUnitGUID, 0);
                }
                pPetData = pNext;
            }
        }
    }

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    for (int32_t nPetType = 1; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerPets, nPetType);
        if (pPetList)
        {
            D2PetTypeTxt* pPetTypeTxtRecord = &sgptDataTables->pPetTypeTxt[nPetType];
            if (pPetTypeTxtRecord && pPetTypeTxtRecord->dwPetFlags & gdwBitMasks[2])
            {
                for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
                {
                    if (!(pPetData->nFlags & PETFLAG_DEAD))
                    {
                        D2UnitStrc* pPet = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pPetData->nUnitGUID);
                        if (pPet)
                        {
                            SCMD_SendUnitLifeAndPositionUpdate(pClient, pPet);
                        }
                    }
                }
            }
        }
    }
}

//D2Game.0x6FC7E7C0
//1.14d: 0x00574DE0
void __fastcall PLAYERPETS_IteratePets(D2GameStrc* pGame, D2UnitStrc* pPlayer, void(__fastcall* pCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*), void* pArg)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);

    if (IsBadCodePtr((FARPROC)pCallback))
    {
        FOG_DisplayAssert("pfnCallBack", __FILE__, __LINE__);
        exit(-1);
    }

    if (!pPlayerData->pPlayerPets)
    {
        return;
    }

    for (int32_t nPetType = 1; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerData->pPlayerPets, nPetType);
        if (pPetList)
        {
            for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
            {
                if (!(pPetData->nFlags & PETFLAG_DEAD))
                {
                    D2UnitStrc* pPet = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pPetData->nUnitGUID);
                    if (pPet)
                    {
                        pCallback(pGame, pPlayer, pPet, pArg);
                    }
                }
            }
        }
    }
}

//D2Game.0x6FC7E8B0
D2UnitStrc* __fastcall PLAYERPETS_GetFirstPetByType(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPetType, int32_t bIncludeDead)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (!pPlayerData)
    {
        return nullptr;
    }

    D2PetListStrc* pPetList = PLAYERPETS_GetPetListFromPetType(pPlayerData->pPlayerPets, nPetType);
    if (!pPetList)
    {
        return nullptr;
    }

    for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
    {
        if (bIncludeDead || !(pPetData->nFlags & PETFLAG_DEAD))
        {
            return SUNIT_GetServerUnit(pGame, UNIT_MONSTER, pPetData->nUnitGUID);
        }
    }

    return nullptr;
}

//D2Game.0x6FC7E930
//1.14d: 0x00574F40
int32_t __fastcall PLAYERPETS_GetTotalPetCount(D2UnitStrc* pPlayer)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (!pPlayerData || !pPlayerData->pPlayerPets)
    {
        return 0;
    }

    int32_t nPets = 0;
    for (int32_t i = 0; i < sgptDataTables->nPetTypeTxtRecordCount; ++i)
    {
        nPets += pPlayerData->pPlayerPets->pPetList[i].nCurrent;
    }

    return nPets;
}

//D2Game.0x6FC7E970
//1.14d: 0x00574F80
void __fastcall PLAYERPETS_SendPetsToClient(D2ClientStrc* pClient, D2UnitStrc* pPlayer)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);

    if (!pPlayerData || !pPlayer || !pPlayer->pGame || !pPlayerData->pPlayerPets)
    {
        return;
    }

    for (int32_t nPetType = 0; nPetType < sgptDataTables->nPetTypeTxtRecordCount; ++nPetType)
    {
        D2PetListStrc* pPetList = &pPlayerData->pPlayerPets->pPetList[nPetType];
        if (pPetList)
        {
            for (D2PetDataStrc* pPetData = pPetList->pPetData; pPetData; pPetData = pPetData->pNext)
            {
                if (!(pPetData->nFlags & PETFLAG_DEAD))
                {
                    D2UnitStrc* pPet = SUNIT_GetServerUnit(pPlayer->pGame, UNIT_MONSTER, pPetData->nUnitGUID);
                    if (pPet)
                    {
                        if (nPetType == PETTYPE_HIREABLE)
                        {
                            D2GAME_PACKETS_SendPacket0x81_6FC3E160(pClient, 1, PETTYPE_HIREABLE, pPetData->nUnitGUID, pPlayer->dwUnitId, pPet->dwClassId, pPetData->petInfo.nSeed, pPetData->petInfo.wName);
                        }
                        else
                        {
                            D2GAME_PACKETS_SendPacket0x7A_6FC3E100(pClient, 1, nPetType, pPetData->nUnitGUID, pPlayer->dwUnitId, pPet->dwClassId);
                        }
                    }
                }
            }
        }
    }
}
