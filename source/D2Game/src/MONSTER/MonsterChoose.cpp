#include "MONSTER/MonsterChoose.h"

#include <D2BitManip.h>

#include <Drlg/D2DrlgDrlg.h>
#include <DataTbls/LevelsTbls.h>

#include "ITEMS/Items.h"
#include "GAME/Game.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"


D2UnkMonCreateStrc2 dword_6FD2E9A0 = {
    4, 0, 0, 0, 0, 0,
    {
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 2, 1, 0 },
        { 1, 2, 0}
    }
};
D2UnkMonCreateStrc2* off_6FD2E9DC = &dword_6FD2E9A0;


//D2Game.0x6FC62020
void __stdcall sub_6FC62020(D2SeedStrc* pSeed, D2MonRegDataStrc* pMonRegData, int32_t nCount)
{
    D2_ASSERT(pSeed);
    D2_ASSERT(pMonRegData);

    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pMonRegData->nMonHcIdx);
    if (!pMonStats2TxtRecord)
    {
        return;
    }

    int32_t nMax = (1 << pMonStats2TxtRecord->unk0x25) - pMonRegData->unk0x03;
    
    if (nCount > nMax)
    {
        nCount = nMax;
    }

    if (nCount <= 0)
    {
        return;
    }

    nMax = 3 - pMonRegData->unk0x03;
    if (nCount > nMax)
    {
        nCount = nMax;
    }

    if (nCount <= 0)
    {
        return;
    }

    if (!pMonRegData->unk0x03)
    {
        for (int32_t i = 0; i < 16; ++i)
        {
            if (pMonStats2TxtRecord->unk0x15[i] > 1u)
            {
                pMonRegData->unk0x04[0][i] = ITEMS_RollLimitedRandomNumber(pSeed, pMonStats2TxtRecord->unk0x15[i]);
            }
            else
            {
                pMonRegData->unk0x04[0][i] = 0;
            }
        }

        --nCount;
        ++pMonRegData->unk0x03;
    }

    if (!nCount)
    {
        return;
    }

    uint8_t indices[16] = {};
    int32_t nMaxIndex = 0;
    for (int32_t i = 0; i < 16; ++i)
    {
        if (pMonStats2TxtRecord->unk0x15[i] > 1u)
        {
            indices[nMaxIndex] = i;
            ++nMaxIndex;
        }
    }

    if (nMaxIndex < 1)
    {
        return;
    }

    uint8_t nIndex1 = indices[0];
    uint8_t nIndex2 = indices[0];
    if (nMaxIndex != 1)
    {
        const uint32_t nRand = ITEMS_RollLimitedRandomNumber(pSeed, nMaxIndex);
        nIndex1 = indices[nRand];
        indices[nRand] = indices[nMaxIndex - 1];
        nIndex2 = indices[ITEMS_RollLimitedRandomNumber(pSeed, nMaxIndex - 1)];
    }

    uint8_t* pData = pMonRegData->unk0x04[pMonRegData->unk0x03];
    for (int32_t i = 0; i < nCount; ++i)
    {
        int32_t nCounter = 3;
        for (int32_t j = 0; j < 16; ++j)
        {
            pData[j] = pMonRegData->unk0x04[0][j];
        }
        
        int32_t bContinue = 0;
        do
        {
            pData[nIndex1] = ITEMS_RollLimitedRandomNumber(pSeed, pMonStats2TxtRecord->unk0x15[nIndex1]);
            if (nIndex1 != nIndex2)
            {
                pData[nIndex2] = ITEMS_RollLimitedRandomNumber(pSeed, pMonStats2TxtRecord->unk0x15[nIndex2]);
            }

            bContinue = 0;
            for (int32_t j = 0; j < pMonRegData->unk0x03; ++j)
            {
                if (!memcmp(pData, pMonRegData->unk0x04[j], 16))
                {
                    bContinue = 1;
                    --nCounter;
                }
            }
        }
        while (bContinue && nCounter);

        pData += 16;

        ++pMonRegData->unk0x03;
    }
}

//D2Game.0x6FC62420
int32_t __fastcall MONSTERCHOOSE_GetPresetMonsterId(D2GameStrc* pGame, D2MonsterRegionStrc* pMonsterRegion, D2ActiveRoomStrc* pRoom, D2MonStatsTxt** ppMonStatsTxtRecord, uint8_t nChance, int32_t bSpawnUMon)
{
    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    if (bSpawnUMon && pGame->nDifficulty == DIFFMODE_NORMAL)
    {
        D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(pMonsterRegion->dwlevel);
        if (!pLevelsTxtRecord->nNumUniqueMonsters)
        {
            *ppMonStatsTxtRecord = nullptr;
            return 0;
        }

        const int32_t nMonsterId = pLevelsTxtRecord->wUniqueMonsters[ITEMS_RollLimitedRandomNumber(pSeed, pLevelsTxtRecord->nNumUniqueMonsters)];
        *ppMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
        return nMonsterId;
    }

    if (!pMonsterRegion->nMonCount)
    {
        *ppMonStatsTxtRecord = nullptr;
        return 0;
    }

    int32_t nRarityRoll = ITEMS_RollLimitedRandomNumber(pSeed, pMonsterRegion->nTotalRarity) + 1;
    D2MonRegDataStrc* pMonRegData = pMonsterRegion->pMonData;
    for (int32_t i = 0; i < pMonsterRegion->nMonCount; ++i)
    {
        nRarityRoll -= pMonRegData->nRarity;
        if (nRarityRoll <= 0)
        {
            break;
        }

        ++pMonRegData;
    }

    int32_t nMonsterId = pMonRegData->nMonHcIdx;
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
    if (pMonStatsTxtRecord && pMonStatsTxtRecord->wSpawn >= 0 && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_PLACESPAWN] && (ITEMS_RollRandomNumber(pSeed) % 100) > nChance)
    {
        nMonsterId = pMonStatsTxtRecord->wSpawn;
    }

    *ppMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
    return nMonsterId;
}

//D2Game.0x6FC62640
D2UnkMonCreateStrc2* __fastcall sub_6FC62640(D2ActiveRoomStrc* pRoom)
{
    if (pRoom)
    {
        ITEMS_RollRandomNumber(&pRoom->pSeed);
    }

    return off_6FD2E9DC;
}

//D2Game.0x6FC62670
int32_t __fastcall MONSTERCHOOSE_GetBossSpawnType(D2MonsterRegionStrc* pMonsterRegion, D2ActiveRoomStrc* pRoom)
{
    if (pMonsterRegion->dwUniqueCount < pMonsterRegion->nBossMin && pMonsterRegion->unk0x0C && (ITEMS_RollRandomNumber(&pRoom->pSeed) % 100) < 100 * pMonsterRegion->unk0x04 / pMonsterRegion->unk0x0C)
    {
        return 0;
    }

    if (pMonsterRegion->dwUniqueCount < pMonsterRegion->nBossMax && (ITEMS_RollRandomNumber(&pRoom->pSeed) % 100) <= 5)
    {
        return 0;
    }

    if ((ITEMS_RollRandomNumber(&pRoom->pSeed) % 100) > 35)
    {
        return 2;
    }

    return 1;
}
