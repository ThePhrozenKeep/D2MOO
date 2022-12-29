#include <ctime>

#include "PLAYER/PlrSave2.h"

#include <Fog.h>
#include <Storm.h>
#include <D2BitManip.h>

#include <D2Items.h>
#include <D2Waypoints.h>
#include <D2StatList.h>
#include <D2States.h>
#include <D2Skills.h>
#include <D2QuestRecord.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/SkillsIds.h>
#include <D2Dungeon.h>
#include <D2Monsters.h>

#include "GAME/Arena.h"
#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterAI.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlayerStats.h"
#include "PLAYER/PlrIntro.h"
#include "PLAYER/PlrSave.h"
#include "QUESTS/Quests.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitMsg.h"
#include "UNIT/SUnitNpc.h"


#pragma pack(push, 1)
struct D2PlrIntroSaveStrc
{
    uint16_t nHeader;
    uint16_t nLength;
    uint8_t unk0x04[3][8];
    uint8_t unk0x1C[3][8];
};

struct D2WaypointSaveFlagStrc
{
    D2WaypointDataStrc flags;
    uint32_t unk0x18;
    uint32_t unk0x1C;
};

struct D2WaypointSaveDataStrc
{
    uint16_t nHeader;
    int32_t unk0x02;
    uint16_t nRecordSize;
    D2WaypointSaveFlagStrc waypoints[3];
};
#pragma pack(pop)


//D2Game.0x6FC8CCD0
void __fastcall PLRSAVE2_WriteMercData(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2SaveHeaderStrc* pMercData)
{
    D2UnitStrc* pHireling = sub_6FC7E8B0(pGame, pPlayer, PETTYPE_HIREABLE, 1);
    if (!pHireling)
    {
        return;
    }

    D2PetInfoStrc* pPetInfo = PLAYERPETS_GetPetInfoFromPetGUID(pPlayer, pHireling->dwUnitId);
    const int32_t nLevel = STATLIST_GetUnitStatUnsigned(pHireling, STAT_LEVEL, 0);

    D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, pPetInfo->nHirelingId, nLevel);
    if (!pHirelingTxtRecord)
    {
        return;
    }

    pMercData->MercSaveData.nSeed = pPetInfo->nSeed;
    pMercData->MercSaveData.wName = pPetInfo->wName;
    pMercData->MercSaveData.nHirelingId = pPetInfo->nHirelingId;
    pMercData->MercSaveData.nExperience = STATLIST_GetUnitStatUnsigned(pHireling, STAT_EXPERIENCE, 0);
    pMercData->MercSaveData.nFlags = 0;
    pMercData->MercSaveData.wName -= pHirelingTxtRecord->wNameFirst;

    if (SUNIT_IsDead(pHireling))
    {
        pMercData->MercSaveData.nFlags |= UNITFLAG_ISDEAD;
    }
}

// Inlined in D2Game.0x6FC8CD80
int16_t __fastcall PLRSAVE2_GetItemIndex(D2UnitStrc* pPlayer, int32_t nGUID)
{
    if (nGUID == -1 || !pPlayer->pInventory)
    {
        return 0;
    }

    int16_t nIdx = 1;
    D2UnitStrc* pItem = INVENTORY_GetFirstItem(pPlayer->pInventory);
    while (pItem)
    {
        D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(pItem);
        D2UnitStrc* pNextItem = INVENTORY_GetNextItem(pItem);
        if ((pCheckedItem ? pCheckedItem->dwUnitId : -1) == nGUID)
        {
            D2_ASSERT(nIdx < SHRT_MAX);
            return nIdx;
        }

        ++nIdx;
        pItem = pNextItem;
    }

    return 0;
}

//D2Game.0x6FC8CD80
int32_t __fastcall PLRSAVE2_WriteSaveHeader(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, const uint8_t* pEnd)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    if (!pClient)
    {
        return 2;
    }

    const char* szName = CLIENTS_GetName(pClient);
    if (!szName || szName[0] == '\0')
    {
        return 2;
    }

    int32_t i = 0;
    while (i < 16)
    {
        if (szName[i] == '\0')
        {
            break;
        }
        ++i;
    }

    if (i == 16)
    {
        return 2;
    }

    if (*ppSection + sizeof(D2SaveHeaderStrc) > pEnd)
    {
        return 1;
    }

    D2SaveHeaderStrc saveHeader = {};
    memset(&saveHeader, 0, sizeof(D2SaveHeaderStrc));

    saveHeader.dwHeaderMagic = D2SMAGIC_HEADER;
    saveHeader.dwVersion = 96;
    SStrCopy(saveHeader.szName, szName, INT_MAX);
    saveHeader.dwSaveFlags = CLIENTS_GetFlags(pClient);
    if (pGame->bExpansion)
    {
        saveHeader.dwSaveFlags |= 0x20;
    }

    if (pGame->dwGameType)
    {
        saveHeader.dwSaveFlags |= 0x40;
    }

    saveHeader.nClass = CLIENTS_GetClassId(pClient);
    saveHeader.nLevel = STATLIST_GetUnitStatUnsigned(pPlayer, STAT_LEVEL, 0);
    saveHeader.dwCreateTime = CLIENTS_GetCreateTime(pClient);
    saveHeader.dwLasTime = time(0);
    saveHeader.dwPlayTime = -1;
    saveHeader.dwMapSeed = pGame->dwInitSeed;
    saveHeader.nStats = 16;
    saveHeader.nSkills = (uint8_t)sgptDataTables->nHighestClassSkillCount;

    if (CLIENTS_GetWeaponSwitch(pClient))
    {
        saveHeader.dwWeaponSet |= 1;
    }

    for (i = 0; i < 16; ++i)
    {
        int32_t nSkill = 0;
        int32_t nParam = 0;
        int32_t nGUID = 0;
        CLIENTS_GetSkillHotKey(pClient, i, &nSkill, &nParam, &nGUID);

        if (nSkill >= 32768)
        {
            FOG_DisplayAssert("nSkill < HOTKEY_LEFT_FLAG", __FILE__, __LINE__);
            exit(-1);
        }

        saveHeader.SkillKeys[i].nSkill = nSkill | (nParam != 0 ? 0x8000 : 0);
        *((int16_t*)&saveHeader.SkillKeys[0].nKey + 2 * i) = PLRSAVE2_GetItemIndex(pPlayer, nGUID);
    }

    D2SkillStrc* pSkill = UNITS_GetLeftSkill(pPlayer);
    if (pSkill)
    {
        saveHeader.nLeftSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
        saveHeader.nLeftSkillItemIndex = PLRSAVE2_GetItemIndex(pPlayer, SKILLS_GetOwnerGUIDFromSkill(pSkill));
    }

    pSkill = UNITS_GetRightSkill(pPlayer);
    saveHeader.nRightSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
    saveHeader.nRightSkillItemIndex = PLRSAVE2_GetItemIndex(pPlayer, SKILLS_GetOwnerGUIDFromSkill(pSkill));

    int32_t nSkillId = 0;
    int32_t nGUID = 0;
    UNITS_GetSwitchLeftSkillData(pPlayer, &nSkillId, &nGUID);
    saveHeader.nSwitchLeftSkillId = nSkillId;
    saveHeader.nSwitchLeftSkillItemIndex = PLRSAVE2_GetItemIndex(pPlayer, nGUID);

    UNITS_GetSwitchRightSkillData(pPlayer, &nSkillId, &nGUID);
    saveHeader.nSwitchRightSkillId = nSkillId;
    saveHeader.nSwitchRightSkillItemIndex = PLRSAVE2_GetItemIndex(pPlayer, nGUID);

    saveHeader.nTown[pGame->nDifficulty] = CLIENTS_GetActNo(pClient) | 0x80;

    memset(&saveHeader.nComponent, 0xFF, 16);
    memset(&saveHeader.nCompColor, 0xFF, 16);

    INVENTORY_GetItemSaveGfxInfo(pPlayer, saveHeader.nComponent, saveHeader.nCompColor);
    PLRSAVE2_WriteMercData(pGame, pPlayer, &saveHeader);

    uint8_t nSaveField = 0;
    uint8_t nUnused1 = 0;
    uint8_t nUnused2 = 0;
    CLIENTS_GetGuildEmblem(pClient, &nSaveField, &nUnused1, &nUnused2);
    saveHeader.nSaveField = nSaveField;

    memcpy(*ppSection, &saveHeader, sizeof(D2SaveHeaderStrc));
    *ppSection += sizeof(D2SaveHeaderStrc);
    return 0;
}

//D2Game.0x6FC8D390
int32_t __fastcall PLRSAVE2_WriteWaypointData(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t** ppSection, const uint8_t* pEnd)
{
    if (*ppSection + sizeof(D2WaypointSaveDataStrc) > pEnd)
    {
        return 1;
    }

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    if (!pPlayerData)
    {
        return 2;
    }

    D2WaypointSaveDataStrc waypointSaveData = {};
    waypointSaveData.nHeader = 'SW';
    waypointSaveData.unk0x02 = 1;
    waypointSaveData.nRecordSize = sizeof(D2WaypointSaveDataStrc);

    for (int32_t i = 0; i < 3; ++i)
    {
        WAYPOINTS_ValidateAndCopyWaypointData(pPlayerData->pWaypointData[i], &waypointSaveData.waypoints[i].flags);
    }

    memcpy(*ppSection, &waypointSaveData, sizeof(D2WaypointSaveDataStrc));
    *ppSection += sizeof(D2WaypointSaveDataStrc);

    return 0;
}

//D2Game.0x6FC8D440
int32_t __fastcall PLRSAVE2_WritePlrIntroSection(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t** ppSection, const uint8_t* pEnd)
{
    if (*ppSection + sizeof(D2PlrIntroSaveStrc) > pEnd)
    {
        return 1;
    }

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    if (!pPlayerData)
    {
        return 2;
    }

    D2PlrIntroSaveStrc plrIntroSave = {};
    plrIntroSave.nHeader = 0x7701u;
    plrIntroSave.nLength = sizeof(D2PlrIntroSaveStrc);

    for (int32_t i = 0; i < 3; ++i)
    {
        PLRINTRO_CopyQuestIntroFlagsToBuffer(pPlayerData->pPlayerIntro[i], plrIntroSave.unk0x04[i]);
        PLRINTRO_CopyNpcIntroFlagsToBuffer(pPlayerData->pPlayerIntro[i], plrIntroSave.unk0x1C[i]);
    }

    memcpy(*ppSection, &plrIntroSave, sizeof(D2PlrIntroSaveStrc));
    *ppSection += sizeof(D2PlrIntroSaveStrc);

    return 0;
}

//D2Game.0x6FC8D4F0
int32_t __fastcall PLRSAVE2_WritePlayerStats(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, const uint8_t* pEnd)
{
    uint8_t* pData = *ppSection + 2;
    if (pData > pEnd)
    {
        return 1;
    }

    *(uint16_t*)*ppSection = 0x6667u;

    D2BitBufferStrc bitBuffer = {};
    BITMANIP_Initialize(&bitBuffer, pData, pEnd - pData);

    if (pPlayer->pStatListEx)
    {
        D2StatStrc statBuffer[512] = {};
        const int32_t nStats = STATLIST_GetBaseStatsData((D2StatListStrc*)pPlayer->pStatListEx, statBuffer, 512);
        
        for (int32_t i = 0; i < nStats; ++i)
        {
            int32_t nValue = statBuffer[i].nValue;
            const int32_t nLayer = statBuffer[i].nLayer;
            const int32_t nStatId = statBuffer[i].nStat;

            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
            if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->nCsvBits)
            {
                if (pItemStatCostTxtRecord->nCsvBits < 32)
                {
                    if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_CSVSIGNED])
                    {
                        const int32_t nMinimum = -(1 << (pItemStatCostTxtRecord->nCsvBits - 1));
                        int32_t nMaximum = 1 << (pItemStatCostTxtRecord->nCsvBits - 1);
                        if (nValue <= nMinimum || nValue >= nMaximum)
                        {
                            FOG_Trace("Save error: stat:%d  value:%d", nStatId, nValue);
                        }

                        nMaximum -= 1;

                        if (nValue < nMinimum)
                        {
                            nValue = nMinimum;
                        }

                        if (nValue > nMaximum)
                        {
                            nValue = nMaximum;
                        }
                    }
                    else
                    {
                        int32_t nMaximum = 1 << pItemStatCostTxtRecord->nCsvBits;
                        if (nValue < 0 || nValue >= nMaximum)
                        {
                            FOG_Trace("Save error: stat:%d  value:%d", nStatId, nValue);
                        }

                        nMaximum -= 1;

                        if (nValue < 0)
                        {
                            nValue = 0;
                        }

                        if (nValue >= nMaximum)
                        {
                            nValue = nMaximum;
                        }
                    }
                }

                BITMANIP_Write(&bitBuffer, nStatId, 9);
                if (pItemStatCostTxtRecord->nCsvParam)
                {
                    if (nLayer >= 1 << pItemStatCostTxtRecord->nCsvParam)
                    {
                        FOG_Trace("Save error: stat:%d  param:%d", nStatId, nLayer);
                    }

                    BITMANIP_Write(&bitBuffer, nLayer, pItemStatCostTxtRecord->nCsvParam);
                }

                BITMANIP_Write(&bitBuffer, nValue, pItemStatCostTxtRecord->nCsvBits);
            }
        }
    }

    BITMANIP_Write(&bitBuffer, 511, 9);

    *ppSection = &pData[BITMANIP_GetSize(&bitBuffer)];

    return 0;
}

//D2Game.0x6FC8D710
int32_t __fastcall PLRSAVE2_WriteCorpsesSection(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, const uint8_t* pEnd, int32_t a5)
{
    if (!pPlayer->pInventory)
    {
        return 2;
    }

    uint16_t bHasCorpseWithItems = 0;
    D2UnitStrc* pUnit = nullptr;
    for (D2CorpseStrc* pCorpse = INVENTORY_GetFirstCorpse(pPlayer->pInventory); pCorpse; pCorpse = INVENTORY_GetNextCorpse(pCorpse))
    {
        pUnit = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, INVENTORY_GetUnitGUIDFromCorpse(pCorpse));
        if (pUnit && pUnit->pInventory && INVENTORY_GetItemCount(pUnit->pInventory))
        {
            bHasCorpseWithItems = 1;
            break;
        }
    }

    uint8_t* pSection = *ppSection;
    if (pSection + 4 > pEnd)
    {
        return 1;
    }

    *(uint16_t*)pSection = 'MJ';
    pSection += 2;

    *(uint16_t*)pSection = bHasCorpseWithItems;
    pSection += 2;

    if (!pUnit || !bHasCorpseWithItems)
    {
        *ppSection = pSection;
        return 0;
    }

    if (pSection + 12 > pEnd)
    {
        return 1;
    }

    //*(uint32_t*)pSection = v23; // TODO: Maybe Corpse Id??? In IDA it's undefined
    *((uint32_t*)pSection + 1) = CLIENTS_GetUnitX(pUnit);
    *((uint32_t*)pSection + 2) = CLIENTS_GetUnitY(pUnit);

    pSection += 12;

    const int32_t nSectionSize = sub_6FC8A0F0(pGame, pUnit, pSection, pEnd - pSection, 0, a5);
    if (nSectionSize < 0)
    {
        return 4;
    }

    *ppSection = &pSection[nSectionSize];
    return 0;
}

//D2Game.0x6FC8D880
int32_t __fastcall PLRSAVE2_WriteIronGolemSection(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, const uint8_t* pEnd, int32_t a5)
{
    if (!pGame->bExpansion)
    {
        return 0;
    }

    *(uint16_t*)*ppSection = 'fk';
    
    D2UnitStrc* pIronGolem = sub_6FC7E8B0(pGame, pPlayer, 3, 1);

    D2UnitStrc* pGolemItem = nullptr;
    uint8_t bHasIronGolem = 0;
    if (pIronGolem && pIronGolem->dwClassId == MONSTER_IRONGOLEM && SKILLS_GetSkillById(pPlayer, SKILL_IRONGOLEM, -1) && pIronGolem->pInventory)
    {
        pGolemItem = INVENTORY_UnitIsItem(INVENTORY_GetFirstItem(pIronGolem->pInventory));
        if (pGolemItem)
        {
            bHasIronGolem = 1;
        }
    }

    uint8_t* pData = *ppSection + 2;
    *pData = bHasIronGolem;

    uint8_t* pItemData = pData + 1;
    if (bHasIronGolem)
    {
        const int32_t nLength = D2GAME_SAVE_SerializeItem_6FC89AD0(pGolemItem, pItemData, pEnd - pItemData, 0, a5);
        if (nLength <= 0)
        {
            *pData = 0;
        }

        pItemData += nLength;
    }

    *ppSection = pItemData;
    return 0;
}

//D2Game.0x6FC8D940
int32_t __fastcall PLRSAVE2_CreateSaveFile(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t* pData, uint32_t* pSize, uint32_t nMaxSize, int32_t a6, int32_t a7)
{
    *pSize = 0;

    if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER)
    {
        return 2;
    }

    int32_t nResult = PLRSAVE2_WriteSaveHeader(pGame, pPlayer, &pData, &pData[nMaxSize]);
    if (nResult)
    {
        return nResult;
    }

    const uint8_t* pEnd = &pData[nMaxSize];
    
    uint8_t* ppSection = pData;
    if (ppSection + 298 > pEnd)
    {
        return 1;
    }

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (!pPlayerData)
    {
        return 2;
    }

    uint8_t pQuestDataBuffer[298] = {};
    *(uint32_t*)pQuestDataBuffer = '!ooW';
    *(uint32_t*)&pQuestDataBuffer[4] = 6;
    *(uint16_t*)&pQuestDataBuffer[8] = 298;
    
    for (int32_t i = 0; i < 3; ++i)
    {
        QUESTRECORD_CopyRecordToBuffer(pPlayerData->pQuestData[i], &pQuestDataBuffer[10 + 96 * i], 96, 1);
    }

    memcpy(ppSection, pQuestDataBuffer, 298);
    ppSection += 298;
    
    nResult = PLRSAVE2_WriteWaypointData(pGame, pPlayer, &ppSection, pEnd);
    if (nResult)
    {
        return nResult;
    }

    nResult = PLRSAVE2_WritePlrIntroSection(pGame, pPlayer, &ppSection, pEnd);
    if (nResult)
    {
        return nResult;
    }

    nResult = PLRSAVE2_WritePlayerStats(pGame, pPlayer, &ppSection, pEnd);
    if (nResult)
    {
        return nResult;
    }

    if (&ppSection[sgptDataTables->nHighestClassSkillCount + 2] > pEnd)
    {
        return 1;
    }

    *(uint16_t*)ppSection = 'fi';
    const int32_t nClassId = CLIENTS_GetClassId(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__));
    if (nClassId >= 7)
    {
        return 2;
    }

    ppSection += 2;
    for (int32_t i = 0; i < SKILLS_GetPlayerSkillCount(nClassId); ++i)
    {
        *ppSection = SKILLS_GetSkillLevel(pPlayer, SKILLS_GetSkillById(pPlayer, SKILLS_GetClassSkillId(nClassId, i), -1u), 0);
        ++ppSection;
    }

    const int32_t nSectionSize = sub_6FC8A0F0(pGame, pPlayer, ppSection, pEnd - ppSection, a6, a7);
    if (nSectionSize < 0)
    {
        return 3;
    }

    ppSection += nSectionSize;

    nResult = PLRSAVE2_WriteCorpsesSection(pGame, pPlayer, &ppSection, pEnd, a7);
    if (nResult)
    {
        return nResult;
    }

    if (pGame->bExpansion)
    {
        *(uint16_t*)ppSection = 'fj';
        ppSection += 2;

        D2UnitStrc* pPet = sub_6FC7E8B0(pGame, pPlayer, 7, 1);
        if (pPet)
        {
            const int32_t nPetSectionSize = sub_6FC8A0F0(pGame, pPet, ppSection, pEnd - ppSection, 0, a7);
            if (nPetSectionSize < 0)
            {
                return 5;
            }

            ppSection += nPetSectionSize;
        }

        nResult = PLRSAVE2_WriteIronGolemSection(pGame, pPlayer, &ppSection, pEnd, a7);
        if (nResult)
        {
            return nResult;
        }
    }

    *pSize = ppSection - pData;

    *((uint32_t*)pData + 2) = *pSize;
    *((uint32_t*)pData + 3) = FOG_ComputeChecksum(pData, *pSize);

    return 0;
}

//D2Game.0x6FC8DC20
int32_t __fastcall PLRSAVE2_ReadPlayerFlags(D2GameStrc* pGame, uint32_t dwFlags)
{
    if (dwFlags & 0x20)
    {
        if (!pGame->bExpansion)
        {
            return 8;
        }
    }
    else
    {
        if (pGame->bExpansion)
        {
            return 9;
        }
    }

    if (dwFlags & 0x40)
    {
        if (!pGame->dwGameType)
        {
            return 25;
        }
    }
    else
    {
        if (pGame->dwGameType)
        {
            return 26;
        }
    }

    if (dwFlags & 4)
    {
        if (dwFlags & 8)
        {
            return 10;
        }

        if (!(ARENA_GetFlags(pGame) & 0x800))
        {
            return 11;
        }
    }
    else
    {
        if (ARENA_GetFlags(pGame) & 0x800)
        {
            return 12;
        }
    }

    const uint8_t nFlagHeader = (dwFlags >> 8) & 0x1F;
    if (pGame->nDifficulty <= 1u)
    {
        if (pGame->nDifficulty && (dwFlags & 0x20 && nFlagHeader < 5 || nFlagHeader < 4))
        {
            return 13;
        }
    }
    else
    {
        if (dwFlags & 0x20 && nFlagHeader < 10 || nFlagHeader < 8)
        {
            return 14;
        }
    }

    return 0;
}

//D2Game.0x6FC8DD00
int32_t __fastcall PLRSAVE2_ReadSaveHeader(D2GameStrc* pGame, D2ClientStrc* pClient, uint8_t** ppSection, uint8_t* pEnd, D2UnitStrc** ppPlayer)
{
    *ppPlayer = nullptr;

    CLIENTS_SetFlags(pClient, 0);
    
    if (*ppSection + 335 > pEnd)
    {
        return 4;
    }

    uint8_t* pSection = *ppSection;

    const uint32_t nSize = pEnd - pSection;
    const uint32_t nCheckSum = *((uint32_t*)pSection + 3);
    *((uint32_t*)pSection + 3) = 0;

    if (FOG_ComputeChecksum(pSection, nSize) != nCheckSum)
    {
        return 6;
    }

    if (nSize != *((uint32_t*)pSection + 2))
    {
        return 5;
    }

    const uint32_t nVersion = *((uint32_t*)pSection + 1);
    if (nVersion <= 91 || nVersion > 96)
    {
        return 7;
    }

    pSection[35] = 0;

    const char* szClientName = CLIENTS_GetName(pClient);
    if (!szClientName)
    {
        return 3;
    }

    if (_strcmpi(szClientName, (const char*)pSection + 20))
    {
        return 7;
    }

    const int32_t nResult = PLRSAVE2_ReadPlayerFlags(pGame, *((uint16_t*)pSection + 18));
    if (nResult)
    {
        return nResult;
    }

    const uint8_t nClass = pSection[40];
    if (nClass > 7u)
    {
        return 4;
    }

    CLIENTS_SetClassId(pClient, nClass);

    uint32_t nFlags = *((uint32_t*)pSection + 9);
    if (nFlags & 1)
    {
        nFlags &= 0xFFFFFFFE;
        *((uint32_t*)pSection + 9) = nFlags;
        CLIENTS_SetFlags(pClient, (uint16_t)nFlags);

        *ppSection += 335;
        return 2;
    }

    CLIENTS_SetWeaponSwitch(pClient, pSection[16] & 1);
    CLIENTS_SetFlags(pClient, *((uint16_t*)pSection + 18));
    CLIENTS_SetCreateTime(pClient, *((uint32_t*)pSection + 11));

    const uint8_t v18 = pSection[168 + pGame->nDifficulty];
    uint8_t nAct = v18 & 127;
    if (nAct >= 5u)
    {
        nAct = 0;
    }

    CLIENTS_SetActNo(pClient, nAct);

    if (pGame->nGameType == 3 && !pGame->InitSeed && v18 & 0x80)
    {
        pGame->dwInitSeed = *(uint32_t*)(pSection + 171);
    }

    D2UnitStrc* pPlayer = SUNIT_AllocUnitData(UNIT_PLAYER, nClass, 0, 0, pGame, 0, 0, PLRMODE_DEATH, 0);
    *ppPlayer = pPlayer;

    CLIENTS_SetPlayerInClient(pClient, pPlayer);
    SUNIT_InitClientInPlayerData(pPlayer, pClient);
    UNITS_SetNameInPlayerData(pPlayer, CLIENTS_GetName(pClient));
    SUNIT_InitSeed(pPlayer, &pGame->pGameSeed);
    UNITS_ChangeAnimMode(pPlayer, PLRMODE_NEUTRAL);

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    pPlayerData->pTrade = nullptr;
    pPlayerData->dwTradeState = 0;
    pPlayerData->dwAcceptTradeTick = 0;
    pPlayerData->unk0x54 = 0;

    D2GAME_SUNITMSG_FirstFn_6FCC5520(CLIENTS_GetGame(pClient), *ppPlayer, pClient);

    const uint8_t* pSkillKeyData = pSection + 56;
    for (int32_t i = 0; i < 16; ++i)
    {
        if (*(int16_t*)pSkillKeyData == -1)
        {
            CLIENTS_SetSkillHotKey(pClient, i, -1, 0, -1);
        }
        else
        {
            const int16_t nSkillData = *(int16_t*)pSkillKeyData;
            const int16_t nSkillId = nSkillData & 0xFFF;
            const uint8_t v28 = nSkillData >> 15;

            int16_t nFlags = *((int16_t*)pSkillKeyData + 1);
            if (!nFlags)
            {
                nFlags = -1;
            }

            CLIENTS_SetSkillHotKey(pClient, i, nSkillId, v28, nFlags);
        }

        pSkillKeyData += 4;
    }

    if (!pPlayerData)
    {
        return 4;
    }

    const int16_t nLeftSkill = *((int16_t*)pSection + 60);
    if (nLeftSkill == -1)
    {
        pPlayerData->nLeftSkillId = -1;
        pPlayerData->nLeftSkillFlags = -1;
    }
    else
    {
        pPlayerData->nLeftSkillId = nLeftSkill & 0xFFF;
        pPlayerData->nLeftSkillFlags = *((uint16_t*)pSection + 61);

        if (!pPlayerData->nLeftSkillFlags)
        {
            pPlayerData->nLeftSkillFlags = -1;
        }
    }

    const int16_t nRightSkill = *((int16_t*)pSection + 62);
    if (nRightSkill == -1)
    {
        pPlayerData->nRightSkillId = -1;
        pPlayerData->nRightSkillFlags = -1;
    }
    else
    {
        pPlayerData->nRightSkillId = nRightSkill & 0xFFF;
        pPlayerData->nRightSkillFlags = *((int16_t*)pSection + 63);

        if (!pPlayerData->nRightSkillFlags)
        {
            pPlayerData->nRightSkillFlags = -1;
        }
    }

    const int16_t nSwitchLeftSkill = *((int16_t*)pSection + 64);
    if (nSwitchLeftSkill == -1)
    {
        pPlayerData->nSwitchLeftSkillId = -1;
        pPlayerData->nSwitchLeftSkillFlags = -1;
    }
    else
    {
        pPlayerData->nSwitchLeftSkillId = nSwitchLeftSkill & 0xFFF;
        pPlayerData->nSwitchLeftSkillFlags = *((int16_t*)pSection + 65);

        if (!pPlayerData->nSwitchLeftSkillFlags)
        {
            pPlayerData->nSwitchLeftSkillFlags = -1;
        }
    }

    const int16_t nSwitchRightSkill = *((int16_t*)pSection + 66);
    if (nSwitchRightSkill == -1)
    {
        pPlayerData->nSwitchRightSkillId = -1;
        pPlayerData->nSwitchRightSkillFlags = -1;
    }
    else
    {
        pPlayerData->nSwitchRightSkillId = nSwitchRightSkill & 0xFFF;
        pPlayerData->nSwitchRightSkillFlags = *((int16_t*)pSection + 67);

        if (!pPlayerData->nSwitchRightSkillFlags)
        {
            pPlayerData->nSwitchRightSkillFlags = -1;
        }
    }

    CLIENTS_SetGuildEmblem(pClient, pSection[207], 0, 0);

    *ppSection += 335;

    return 0;
}

//D2Game.0x6FC8E070
int32_t __fastcall PLRSAVE2_LoadWaypointData(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t** ppSection, uint8_t* pEnd, int32_t nUnused)
{
    if (*ppSection + sizeof(D2WaypointSaveDataStrc) > pEnd)
    {
        return 16;
    }

    D2WaypointSaveDataStrc* pWaypointSaveData = (D2WaypointSaveDataStrc*)*ppSection;
    if (pWaypointSaveData->nHeader != 'SW')
    {
        return 16;
    }

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    for (int32_t i = 0; i < 3; ++i)
    {
        D2WaypointDataStrc* pSavedData = &pWaypointSaveData->waypoints[i].flags;

        if (pSavedData->nFlags[0] != 0x102 && pSavedData->nFlags[0] != 0x101 && pSavedData->nFlags[0])
        {
            return 16;
        }

        WAYPOINTS_CopyAndValidateWaypointData(pPlayerData->pWaypointData[i], pSavedData);
    }

    *ppSection += sizeof(D2WaypointSaveDataStrc);
    return 0;
}

//D2Game.0x6FC8E0F0
int32_t __fastcall PLRSAVE2_LoadStatsEx(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t** ppSection, uint8_t* pEnd, uint32_t nVersion)
{
    uint8_t* pData = *ppSection + 2;
    if (pData > pEnd)
    {
        return 18;
    }

    if (*(uint16_t*)*ppSection != 0x6667)
    {
        return 18;
    }

    D2BitBufferStrc bitBuffer = {};
    BITMANIP_Initialize(&bitBuffer, pData, pEnd - pData);
    
    for (int32_t nStatId = BITMANIP_Read(&bitBuffer, 9); nStatId < 511; nStatId = BITMANIP_Read(&bitBuffer, 9))
    {
        D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
        if (!pItemStatCostTxtRecord || !pItemStatCostTxtRecord->nCsvBits)
        {
            FOG_Trace("WARNING: sPLAYERSAVEReadStatsEx, Attempt to set an invalid stat!\n");
            return 18;
        }

        int32_t nLayer = 0;
        if (pItemStatCostTxtRecord->nCsvParam)
        {
            nLayer = BITMANIP_ReadSigned(&bitBuffer, pItemStatCostTxtRecord->nCsvParam);
        }

        int32_t nValue = 0;
        if (pItemStatCostTxtRecord->nCsvBits < 32 && pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_CSVSIGNED])
        {
            nValue = BITMANIP_ReadSigned(&bitBuffer, pItemStatCostTxtRecord->nCsvBits);
        }
        else
        {
            nValue = BITMANIP_Read(&bitBuffer, pItemStatCostTxtRecord->nCsvBits);
        }

        STATLIST_SetUnitStat(pUnit, nStatId, nValue, nLayer);
    }

    *ppSection = &pData[BITMANIP_GetSize(&bitBuffer)];
    return 0;
}

//D2Game.0x6FC8E250
int32_t __fastcall PLRSAVE2_LoadStats(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t** ppSection, uint8_t* pEnd, uint32_t nVersion, int32_t nStats)
{
    if (nVersion > 94)
    {
        return PLRSAVE2_LoadStatsEx(pGame, pUnit, ppSection, pEnd, nVersion);
    }

    const int32_t nHeaderSectionLength = (nStats + 7) / 8;
    if (&(*ppSection)[nHeaderSectionLength + 2] > pEnd)
    {
        return 18;
    }

    if (*(uint16_t*)*ppSection != 0x6667)
    {
        return 18;
    }

    uint8_t* pHeaderData = *ppSection + 2;
    uint8_t* pStatData = &pHeaderData[nHeaderSectionLength];

    if (&pStatData[4 * FOG_PopCount(pHeaderData, nHeaderSectionLength)] > pEnd)
    {
        return 18;
    }

    int32_t nCounter = 0;
    for (int32_t i = 0; i < nStats; ++i)
    {
        int32_t nValue = 0;
        if (pHeaderData[i >> 3] & gdwBitMasks[i & 7])
        {
            nValue = *(int32_t*)&pStatData[4 * nCounter];
            ++nCounter;
        }

        STATLIST_SetUnitStat(pUnit, i, nValue, 0);
    }

    *ppSection = &pStatData[4 * nCounter];
    return 0;
}

//D2Game.0x6FC8E330
int32_t __fastcall PLRSAVE2_LoadSkills(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, uint8_t* pEnd, uint32_t nVersion, int32_t nSkills)
{
    uint8_t* pData = *ppSection + 2;
    if (pData > pEnd)
    {
        return 19;
    }

    if (*(uint16_t*)*ppSection != 0x6669)
    {
        return 19;
    }

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    if (!pClient)
    {
        return 19;
    }

    const uint8_t nClassId = CLIENTS_GetClassId(pClient);
    for (int32_t i = 0; i < nSkills; ++i)
    {
        const int32_t nSkillId = SKILLS_GetClassSkillId(nClassId, i);
        if (i > SKILLS_GetPlayerSkillCount(nClassId))
        {
            break;
        }

        if (pData[i])
        {
            D2GAME_SetSkills_6FD14C60(pPlayer, nSkillId, pData[i], 1);
        }
    }

    sub_6FC3DCA0(pClient, pPlayer);
    sub_6FD14C30(pPlayer);

    *ppSection = &pData[nSkills];
    return 0;
}

//D2Game.0x6FC8E420
int32_t __fastcall PLRSAVE2_LoadCorpses(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, uint8_t* pEnd, uint32_t nVersion, int32_t* a6)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);

    if (!pClient)
    {
        if (a6)
        {
            *a6 = 101;
        }

        return 21;
    }

    if (*ppSection + 4 > pEnd)
    {
        if (a6)
        {
            *a6 = 102;
        }

        return 21;
    }

    if (*(uint16_t*)*ppSection != 'MJ')
    {
        if (a6)
        {
            *a6 = 103;
        }

        return 21;
    }

    const uint16_t nCount = *((uint16_t*)*ppSection + 1);
    if (nCount > 1)
    {
        if (a6)
        {
            *a6 = 104;
        }

        return 21;
    }

    uint8_t* pDataStart = *ppSection + 4;
    for (int32_t i = 0; i < (int32_t)nCount; ++i)
    {
        uint8_t* pData = pDataStart + 12;
        if (pData > pEnd)
        {
            if (a6)
            {
                *a6 = 105;
            }

            return 21;
        }

        D2UnitStrc* pPlayerBody = SUNIT_AllocUnitData(UNIT_PLAYER, pPlayer ? pPlayer->dwClassId : -1, 0, 0, pGame, nullptr, 1, PLRMODE_DEAD, 0);
        if (!pPlayerBody)
        {
            if (a6)
            {
                *a6 = 106;
            }

            return 21;
        }

        pPlayerBody->dwFlags &= 0xFFFFFFFBu;

        SUNIT_InitClientInPlayerData(pPlayerBody, pClient);
        UNITS_SetNameInPlayerData(pPlayerBody, CLIENTS_GetName(pClient));
        SUNIT_InitSeed(pPlayerBody, &pGame->pGameSeed);
        STATES_ToggleState(pPlayerBody, STATE_PLAYERBODY, 1);

        pPlayerBody->dwFlags &= 0xFFFFFFEFu;

        INVENTORY_SetOwnerId(pPlayerBody->pInventory, pPlayer ? pPlayer->dwUnitId : -1);

        int32_t nLength = 0;
        const int32_t nErrorCode = sub_6FC8BC70(pGame, pPlayerBody, pData, nVersion, pEnd - pData, 0, &nLength);
        if (nErrorCode)
        {
            if (a6)
            {
                *a6 = nErrorCode;
            }

            return 21;
        }

        pDataStart = &pData[nLength];

        INVENTORY_CreateCorpseForPlayer(pPlayer->pInventory, pPlayerBody->dwUnitId, 0, 1);
        D2GAME_SendPacket0x8E_CorpseAssign_ToAllPlayers_6FC3F9B0(pGame, pPlayer->dwUnitId, pPlayerBody->dwUnitId, 1u);
    }

    *ppSection = pDataStart;
    return 0;
}

//D2Game.0x6FC8E670
D2UnitStrc* __fastcall PLRSAVE2_LoadMercData(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2MercSaveDataStrc* pData, uint32_t dwVersion)
{
    if (!pData->nExperience && !pData->nSeed && !pData->wName)
    {
        return nullptr;
    }

    D2MercSaveDataStrc hirelingData = {};
    memcpy(&hirelingData, pData, sizeof(hirelingData));

    D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, hirelingData.nHirelingId, 1);
    if (!pHirelingTxtRecord)
    {
        return nullptr;
    }

    hirelingData.wName += pHirelingTxtRecord->wNameFirst;
    if (hirelingData.wName > pHirelingTxtRecord->wNameLast)
    {
        hirelingData.wName = pHirelingTxtRecord->wNameFirst;
    }

    D2UnitStrc* pMerc = D2GAME_MERCS_Create_6FCC8630(pGame, pPlayer, hirelingData.wName, hirelingData.nSeed, hirelingData.nHirelingId, pHirelingTxtRecord->dwClass, hirelingData.nFlags & UNITFLAG_ISDEAD);
    if (!pMerc)
    {
        return nullptr;
    }

    D2PetInfoStrc petInfo = {};
    petInfo.nSeed = hirelingData.nSeed;
    petInfo.wName = hirelingData.wName;
    petInfo.nHirelingId = hirelingData.nHirelingId;
    PLAYERPETS_UpdatePetInfo(pPlayer, PETTYPE_HIREABLE, pMerc->dwUnitId, &petInfo);

    int32_t nLevel = 1;
    const int32_t nMaxLevel = DATATBLS_GetMaxLevel(0);
    
    if (STATLIST_GetUnitStatUnsigned(pMerc, STAT_EXPERIENCE, 0) < hirelingData.nExperience)
    {
        STATLIST_SetUnitStat(pMerc, STAT_EXPERIENCE, hirelingData.nExperience, 0);
    }
    
    D2GAME_MERCS_SendStat_6FCC61D0(pMerc, STAT_EXPERIENCE, hirelingData.nExperience);

    do
    {
        pHirelingTxtRecord = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, hirelingData.nHirelingId, nLevel);
        if (!pHirelingTxtRecord)
        {
            SUNIT_RemoveUnit(pGame, pMerc);
            return 0;
        }

        ++nLevel;
    }
    while (nLevel <= nMaxLevel && hirelingData.nExperience >= MONSTERS_GetHirelingExpForNextLevel(nLevel, pHirelingTxtRecord->dwExpPerLvl));

    MONSTERAI_UpdateMercStatsAndSkills(pGame, pPlayer, pMerc, nLevel);
    
    if (hirelingData.nFlags & UNITFLAG_ISDEAD)
    {
        sub_6FC7D470(pGame, pPlayer, pMerc);
        pMerc->dwFlags |= UNITFLAG_ISDEAD;
        SUNIT_SetCombatMode(pGame, pMerc, MONMODE_DEAD);
        MONSTER_DeleteEvents(pGame, pMerc);
    }
    
    if (!pMerc->pInventory)
    {
        INVENTORY_AllocInventory(nullptr, pMerc);
    }

    return pMerc;
}

//D2Game.0x6FC8E850
int32_t __fastcall PLRSAVE2_ReadPetSection(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pMerc, uint8_t** ppSection, uint8_t* pEnd, uint32_t dwVersion, int32_t* a6)
{
    if (!pGame->bExpansion)
    {
        return 0;
    }
    
    uint8_t* pData = *ppSection + 2;
    if (pData > pEnd)
    {
        return 0;
    }

    if (*(uint16_t*)*ppSection != 'fj')
    {
        if (a6)
        {
            *a6 = 101;
        }

        return 22;
    }

    if (!pMerc)
    {
        *ppSection = pData;
        return 0;
    }

    int32_t nLength = 0;
    // TODO: v13
    const int32_t v13 = sub_6FC8BC70(pGame, pMerc, pData, dwVersion, pEnd - pData, 0, &nLength);
    if (v13)
    {
        if (a6)
        {
            *a6 = v13;
        }

        return 22;
    }

    D2GAME_ITEMS_UpdateInventoryItems_6FC45050(pGame, pMerc, 0, 0);
    //D2Game_10034_Return(0);
    if (!SUNIT_IsDead(pMerc))
    {
        STATLIST_SetUnitStat(pMerc, STAT_HITPOINTS, STATLIST_GetMaxLifeFromUnit(pMerc), 0);
    }

    *ppSection = &pData[nLength];
    return 0;
}

//D2Game.0x6FC8E920
int32_t __fastcall PLRSAVE2_CreateItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc** ppItem, uint8_t** ppSection, uint8_t* pEnd, uint32_t dwVersion)
{
    *ppItem = nullptr;

    uint8_t* pSection = *ppSection;
    uint32_t nSize = pEnd - pSection;

    D2ItemSaveStrc pItemSave = {};
    ITEMS_GetCompactItemDataFromBitstream(pSection, nSize, 1, &pItemSave);

    if (pItemSave.nClassId == -1)
    {
        return 0;
    }

    D2UnitStrc* pItem = sub_6FC4EC10(pGame, 0, pSection, nSize, 1, &pItemSave, &nSize, dwVersion);
    if (!pItem)
    {
        return 0;
    }

    uint8_t* pNext = &pSection[nSize];

    for (int32_t i = 0; i < pItemSave.nItemFileIndex; ++i)
    {
        nSize = pEnd - pNext;
        ITEMS_GetCompactItemDataFromBitstream(pNext, nSize, 1, &pItemSave);

        D2UnitStrc* pSocketable = sub_6FC4EC10(pGame, 0, pNext, nSize, 1, &pItemSave, &nSize, dwVersion);
        if (!pSocketable)
        {
            SUNIT_RemoveUnit(pGame, pItem);
            return 0;
        }

        pNext += nSize;

        UNITS_ChangeAnimMode(pSocketable, IMODE_ONCURSOR);

        int32_t bPlaced = 0;
        if (!D2GAME_ITEMSOCKET_PlaceItem_6FC497E0(pGame, pPlayer, pSocketable->dwUnitId, pItem ? pItem->dwUnitId : -1, &bPlaced, 0, 1u, 0))
        {
            SUNIT_RemoveUnit(pGame, pItem);
            return 0;
        }
    }

    if (ITEMS_GetItemFlags(pItem) & IFLAG_RUNEWORD && !ITEMS_GetRunesTxtRecordFromItem(pItem))
    {
        D2GAME_ITEMMODE_Unk_6FC4BC10(pGame, pPlayer, pItem);
        pItem = nullptr;
    }

    *ppSection = pNext;
    *ppItem = pItem;
    return 1;
}

//D2Game.0x6FC8EAB0
int32_t __fastcall PLRSAVE2_ReadIronGolemSection(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, uint8_t* pEnd, uint32_t dwVersion)
{
    if (!pGame->bExpansion)
    {
        return 0;
    }

    uint8_t* pData = *ppSection + 2;
    if (pData > pEnd)
    {
        return 0;
    }

    if (*(uint16_t*)*ppSection != 'fk')
    {
        return 23;
    }

    uint8_t* pSectionEnd = pData + 1;
    if (*pData)
    {
        D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pPlayer, SKILL_IRONGOLEM);
        if (!pSkill)
        {
            return 23;
        }

        UNITS_SetUsedSkill(pPlayer, pSkill);

        D2UnitStrc* pItem = nullptr;
        if (!PLRSAVE2_CreateItem(pGame, pPlayer, &pItem, &pSectionEnd, pEnd, dwVersion))
        {
            return 23;
        }

        UNITS_ChangeAnimMode(pItem, IMODE_ONGROUND);
        CLIENTS_SetIronGolemItemGUID(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), pItem ? pItem->dwUnitId : -1);
    }

    *ppSection = pSectionEnd;
    return 0;
}

//Inlined in D2Game.0x6FC8EBA0
void __fastcall PLRSAVE2_AlterSkillFlags(D2UnitStrc* pUnit, int32_t* pFlags)
{
    if (*pFlags)
    {
        int32_t nItemGUID = -1;
        if (pUnit->pInventory)
        {
            int32_t nCounter = 1;
            for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
            {
                if (nCounter == *pFlags)
                {
                    if (INVENTORY_UnitIsItem(pItem))
                    {
                        nItemGUID = pItem->dwUnitId;
                    }
                    break;
                }

                ++nCounter;
            }
        }

        *pFlags = nItemGUID;
    }
}

//D2Game.0x6FC8EBA0
void __fastcall PLRSAVE2_InitializeStats(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nHitpoints, int32_t nMana)
{
    if (!pUnit)
    {
        return;
    }

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);

    const int32_t nGold = STATLIST_GetUnitStatUnsigned(pUnit, STAT_GOLD, 0);
    if (nGold < 0 || nGold > UNITS_GetInventoryGoldLimit(pUnit))
    {
        STATLIST_SetUnitStat(pUnit, STAT_GOLD, 0, 0);
    }

    const int32_t nGoldBank = STATLIST_GetUnitStatUnsigned(pUnit, STAT_GOLDBANK, 0);
    if (nGoldBank < 0 || nGoldBank > UNITS_GetStashGoldLimit(pUnit))
    {
        STATLIST_SetUnitStat(pUnit, STAT_GOLDBANK, 0, 0);
    }

    STATLIST_SetUnitStat(pUnit, STAT_STAMINA, STATLIST_GetMaxStaminaFromUnit(pUnit), 0);

    for (int32_t i = 0; i < 16; ++i)
    {
        // TODO: Names
        int32_t nSkillId = 0;
        int32_t a4 = 0;
        int32_t v46 = 0;
        CLIENTS_GetSkillHotKey(pClient, i, &nSkillId, &a4, &v46);

        if (v46)
        {
            int32_t nItemGUID = -1;
            if (pUnit->pInventory)
            {
                int32_t nCounter = 1;
                
                for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
                {
                    if (nCounter == v46)
                    {
                        if (INVENTORY_UnitIsItem(pItem))
                        {
                            nItemGUID = pItem->dwUnitId;
                        }
                        break;
                    }

                    ++nCounter;
                }
            }

            CLIENTS_SetSkillHotKey(pClient, i, nSkillId, a4, nItemGUID);
        }
    }

    PLRSAVE2_AlterSkillFlags(pUnit, &pPlayerData->nLeftSkillFlags);
    PLRSAVE2_AlterSkillFlags(pUnit, &pPlayerData->nRightSkillFlags);
    PLRSAVE2_AlterSkillFlags(pUnit, &pPlayerData->nSwitchLeftSkillFlags);
    PLRSAVE2_AlterSkillFlags(pUnit, &pPlayerData->nSwitchRightSkillFlags);

    if (pPlayerData->nLeftSkillId)
    {
        D2GAME_AssignSkill_6FD13800(pUnit, 1, pPlayerData->nLeftSkillId, pPlayerData->nLeftSkillFlags);
    }

    if (pPlayerData->nRightSkillId)
    {
        D2GAME_AssignSkill_6FD13800(pUnit, 0, pPlayerData->nRightSkillId, pPlayerData->nRightSkillFlags);
    }

    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nHitpoints, 0);
    STATLIST_SetUnitStat(pUnit, STAT_MANA, nMana, 0);
    STATLIST_SetUnitStat(pUnit, STAT_ATTACKRATE, 100, 0);
    STATLIST_SetUnitStat(pUnit, STAT_VELOCITYPERCENT, 100, 0);
    STATLIST_SetUnitStat(pUnit, STAT_OTHER_ANIMRATE, 100, 0);
    STATLIST_SetUnitStat(pUnit, STAT_NEXTEXP, DATATBLS_GetLevelThreshold(pUnit ? pUnit->dwClassId : -1, STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0)), 0);

    if (CLIENTS_GetWeaponSwitch(pClient))
    {
        D2GAME_PACKETS_SendPacket0x97_6FC3FB60(pClient);
    }
}

//D2Game.0x6FC8EF20
int32_t __fastcall PLRSAVE2_ProcessSaveFile(D2GameStrc* pGame, D2ClientStrc* pClient, uint8_t* pSaveFile, uint32_t nSize, D2UnitStrc** ppPlayer, int32_t nUnused1, int32_t nUnused2, int32_t nUnused3)
{
    constexpr int32_t dword_6FD292D0[] =
    {
        0, 14, 9, 14, 9, 14, 14, 1, 24, 23, 21, 20, 19, 17, 18, 2, 3, 11, 4, 5, 7, 8, 10, 10, 14, 26, 25
    };

    D2SaveHeaderStrc* pSaveHeader = (D2SaveHeaderStrc*)pSaveFile;

    uint8_t* pSection = pSaveFile;
    uint8_t* pSaveFileEnd = &pSaveFile[nSize];

    *ppPlayer = nullptr;

    if (!pClient)
    {
        CLIENTS_SetPlayerInClient(pClient, 0);
        return dword_6FD292D0[3];
    }

    int32_t nErrorCode = PLRSAVE2_ReadSaveHeader(pGame, pClient, &pSection, pSaveFileEnd, ppPlayer);
    if (nErrorCode == 0)
    {
        D2UnitStrc* pPlayer = *ppPlayer;
        D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);

        uint8_t* pSectionEnd = pSection + 298;
        const uint32_t nVersion = pSaveHeader->dwVersion;
        if (pSectionEnd > pSaveFileEnd || pSaveHeader->dwHeaderMagic != '!ooW' || nVersion != 6)
        {
            nErrorCode = 15;
        }
        else
        {
            uint8_t* pQuestRecord = pSection + 10;
            for (int32_t i = 0; i < 3; ++i)
            {
                QUESTRECORD_CopyBufferToRecord(pPlayerData->pQuestData[i], pQuestRecord, 96, 1);
                pQuestRecord += 96;
            }

            pSection = pSectionEnd;
            nErrorCode = PLRSAVE2_LoadWaypointData(pGame, pPlayer, &pSection, pSaveFileEnd, nVersion);
            if (!nErrorCode)
            {
                pSectionEnd = pSection + sizeof(D2PlrIntroSaveStrc);
                D2PlrIntroSaveStrc* pPlrIntro = (D2PlrIntroSaveStrc*)pSection;
                if (pSectionEnd > pSaveFileEnd || pPlrIntro->nHeader != 0x7701)
                {
                    nErrorCode = 17;
                }
                else
                {
                    for (int32_t i = 0; i < 3; ++i)
                    {
                        PLRINTRO_CopyBufferToQuestIntroFlags(pPlayerData->pPlayerIntro[i], pPlrIntro->unk0x04[i]);
                        PLRINTRO_CopyBufferToNpcIntroFlags(pPlayerData->pPlayerIntro[i], pPlrIntro->unk0x1C[i]);
                    }

                    pSection = pSectionEnd;
                    nErrorCode = PLRSAVE2_LoadStats(pGame, pPlayer, &pSection, pSaveFileEnd, nVersion, pSaveHeader->nStats);

                    if (!nErrorCode)
                    {
                        const int32_t nHitpoints = STATLIST_GetUnitStatUnsigned(pPlayer, STAT_HITPOINTS, 0);
                        const int32_t nMana = STATLIST_GetUnitStatUnsigned(pPlayer, STAT_MANA, 0);

                        nErrorCode = PLRSAVE2_LoadSkills(pGame, pPlayer, &pSection, pSaveFileEnd, nVersion, pSaveHeader->nSkills);
                        if (!nErrorCode)
                        {
                            int32_t nSize = 0;
                            if (sub_6FC8BC70(pGame, pPlayer, pSection, nVersion, pSaveFileEnd - pSection, 0, &nSize))
                            {
                                nErrorCode = 20;
                            }
                            else
                            {
                                pSection += nSize;
                                nErrorCode = PLRSAVE2_LoadCorpses(pGame, pPlayer, &pSection, pSaveFileEnd, nVersion, &nSize);
                                if (!nErrorCode)
                                {
                                    D2UnitStrc* pMerc = PLRSAVE2_LoadMercData(pGame, pPlayer, &pSaveHeader->MercSaveData, nVersion);
                                    nErrorCode = PLRSAVE2_ReadPetSection(pGame, pPlayer, pMerc, &pSection, pSaveFileEnd, nVersion, &nSize);
                                    if (!nErrorCode)
                                    {
                                        if (pMerc)
                                        {
                                            MONSTERAI_SendMercStats(pGame, pPlayer, 1u);
                                        }

                                        nErrorCode = PLRSAVE2_ReadIronGolemSection(pGame, pPlayer, &pSection, pSaveFileEnd, nVersion);
                                        if (!nErrorCode)
                                        {
                                            PLRSAVE2_InitializeStats(pGame, pPlayer, nHitpoints, nMana);
                                            return 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (nErrorCode < 0 || nErrorCode >= 27)
            {
                nErrorCode = 1;
            }
        }

        if (pPlayer)
        {
            if (UNITS_GetRoom(pPlayer))
            {
                DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pPlayer), pPlayer->dwUnitType, pPlayer->dwUnitId);
            }

            SUNIT_RemoveUnit(pGame, pPlayer);
        }

        CLIENTS_SetPlayerInClient(pClient, nullptr);
        return dword_6FD292D0[nErrorCode];
    }
    else if (nErrorCode == 2)
    {
        if (pSection != pSaveFileEnd)
        {
            CLIENTS_SetPlayerInClient(pClient, nullptr);
            return dword_6FD292D0[2];
        }

        GAME_LogMessage(6, "[CLIENT]  PlayerCreate: %s", CLIENTS_GetName(pClient));
        sub_6FC32220(pClient);

        D2UnitStrc* pPlayer = SUNIT_AllocUnitData(UNIT_PLAYER, CLIENTS_GetClassId(pClient), 0, 0, pGame, nullptr, 0, 0, 0);
        CLIENTS_SetPlayerInClient(pClient, pPlayer);
        SUNIT_InitClientInPlayerData(pPlayer, pClient);
        UNITS_SetNameInPlayerData(pPlayer, CLIENTS_GetName(pClient));
        SUNIT_InitSeed(pPlayer, &pGame->pGameSeed);
        UNITS_ChangeAnimMode(pPlayer, 1);

        D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
        pPlayerData->pTrade = nullptr;
        pPlayerData->dwTradeState = 0;
        pPlayerData->dwAcceptTradeTick = 0;
        pPlayerData->unk0x54 = 0;

        D2GAME_SUNITMSG_FirstFn_6FCC5520(pGame, pPlayer, pClient);

        const uint8_t nAct = CLIENTS_GetActNo(pClient);
        PLAYERSTATS_SetStatsForStartingAct(pGame, pPlayer, nAct);
        PLAYER_CreateStartItemsFromCharStatsTxt(pPlayer, pGame, (void*)nAct);

        pPlayerData->nLeftSkillId = 0;
        pPlayerData->nRightSkillId = 0;

        if (nAct == ACT_I)
        {
            D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pPlayer ? pPlayer->dwClassId : -1);
            if (pCharStatsTxtRecord && pCharStatsTxtRecord->wStartSkill && SKILLS_GetHighestLevelSkillFromUnitAndId(pPlayer, pCharStatsTxtRecord->wStartSkill))
            {
                D2GAME_AssignSkill_6FD13800(pPlayer, 0, pCharStatsTxtRecord->wStartSkill, -1);
                pPlayerData->nRightSkillId = pCharStatsTxtRecord->wStartSkill;
            }
        }

        QUESTS_SequenceCycler(pGame, pPlayer, 1);
        *ppPlayer = pPlayer;

        return 0;
    }

    if (nErrorCode < 0 || nErrorCode >= 27)
    {
        nErrorCode = 1;
    }

    CLIENTS_SetPlayerInClient(pClient, nullptr);
    return dword_6FD292D0[nErrorCode];
}

//D2Game.0x6FC8F3A0
D2CharStatsTxt* __fastcall PLRSAVE2_GetCharStatsTxtRecord(int32_t nClass)
{
    if (nClass >= 0 && nClass < sgptDataTables->nCharStatsTxtRecordCount)
    {
        return &sgptDataTables->pCharStatsTxt[nClass];
    }

    return nullptr;
}
