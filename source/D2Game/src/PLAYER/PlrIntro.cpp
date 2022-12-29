#include "PLAYER/PlrIntro.h"

#include <algorithm>

#include <Fog.h>
#include <D2BitManip.h>

#include <DataTbls/MonsterIds.h>


#pragma pack(push, 1)
struct D2NpcIndexMapStrc
{
    int32_t nNpcClassId;
    int32_t nIndex;
};
#pragma pack(pop)


constexpr D2NpcIndexMapStrc npcIndexMap[] =
{
    { -1, 0 },
    { MONSTER_GHEED, 1 },
    { MONSTER_AKARA, 2 },
    { MONSTER_KASHYA, 3 },
    { MONSTER_WARRIV1, 4 },
    { MONSTER_CHARSI, 5 },
    { MONSTER_CAIN5, 6 },
    { MONSTER_WARRIV2, 7 },
    { MONSTER_ATMA, 8 },
    { MONSTER_DROGNAN, 9 },
    { MONSTER_FARA, 10 },
    { MONSTER_LYSANDER, 11 },
    { MONSTER_GEGLASH, 12 },
    { MONSTER_MESHIF1, 13 },
    { MONSTER_JERHYN, 14 },
    { MONSTER_GREIZ, 15 },
    { MONSTER_ELZIX, 16 },
    { MONSTER_CAIN2, 17 },
    { MONSTER_CAIN3, 18 },
    { MONSTER_CAIN4, 19 },
    { MONSTER_TYRAEL1, 20 },
    { MONSTER_ASHEARA, 21 },
    { MONSTER_HRATLI, 22 },
    { MONSTER_ALKOR, 23 },
    { MONSTER_ORMUS, 24 },
    { MONSTER_IZUAL, 25 },
    { MONSTER_HALBU, 26 },
    { MONSTER_MESHIF2, 27 },
    { MONSTER_NATALYA, 28 },
    { MONSTER_LARZUK, 29 },
    { MONSTER_DREHYA, 30 },
    { MONSTER_MALAH, 31 },
    { MONSTER_NIHLATHAK, 32 },
    { MONSTER_QUAL_KEHK, 33 },
    { MONSTER_CAIN6, 34 },
};

constexpr uint32_t bufferSize = 8;

//D2Game.0x6FC7F000
void __fastcall PLRINTRO_SetQuestIntroFlag(D2UnitStrc* pUnit, D2GameStrc* pGame, int32_t nNpcId)
{
    D2PlrIntroStrc* pPlayerIntro = UNITS_GetPlayerData(pUnit)->pPlayerIntro[pGame->nDifficulty];

    for (int32_t i = 0; i < std::size(npcIndexMap); ++i)
    {
        if (npcIndexMap[i].nNpcClassId == nNpcId)
        {
            BITMANIP_SetBitState(pPlayerIntro->pQuestIntroFlags->pBuffer, npcIndexMap[i].nIndex);
        }
    }

    BITMANIP_SetBitState(pPlayerIntro->pQuestIntroFlags->pBuffer, 0);
}

//D2Game.0x6FC7F060
int32_t __fastcall PLRINTRO_GetQuestIntroFlag(D2UnitStrc* pUnit, D2GameStrc* pGame, int32_t nNpcId)
{
    D2PlrIntroStrc* pPlayerIntro = UNITS_GetPlayerData(pUnit)->pPlayerIntro[pGame->nDifficulty];

    for (int32_t i = 0; i < std::size(npcIndexMap); ++i)
    {
        if (npcIndexMap[i].nNpcClassId == nNpcId)
        {
            return BITMANIP_GetBitState(pPlayerIntro->pQuestIntroFlags->pBuffer, npcIndexMap[i].nIndex);
        }
    }

    return BITMANIP_GetBitState(pPlayerIntro->pQuestIntroFlags->pBuffer, 0);
}

//D2Game.0x6FC7F0C0
void __fastcall PLRINTRO_SetNpcIntroFlag(D2PlrIntroStrc* pPlayerIntro, int32_t nNpcId)
{
    for (int32_t i = 0; i < std::size(npcIndexMap); ++i)
    {
        if (npcIndexMap[i].nNpcClassId == nNpcId)
        {
            BITMANIP_SetBitState(pPlayerIntro->pNpcIntroFlags->pBuffer, npcIndexMap[i].nIndex);
        }
    }

    BITMANIP_SetBitState(pPlayerIntro->pNpcIntroFlags->pBuffer, 0);
}

//D2Game.0x6FC7F100
int32_t __fastcall PLRINTRO_GetNpcIntroFlag(D2PlrIntroStrc* pPlayerIntro, int32_t nNpcId)
{
    for (int32_t i = 0; i < std::size(npcIndexMap); ++i)
    {
        if (npcIndexMap[i].nNpcClassId == nNpcId)
        {
            return BITMANIP_GetBitState(pPlayerIntro->pNpcIntroFlags->pBuffer, npcIndexMap[i].nIndex);
        }
    }

    return BITMANIP_GetBitState(pPlayerIntro->pNpcIntroFlags->pBuffer, 0);
}

//D2Game.0x6FC7F140
void __fastcall PLRINTRO_MaskNpcIntroFlag(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcId)
{
    D2PlrIntroStrc* pPlayerIntro = UNITS_GetPlayerData(pPlayer)->pPlayerIntro[pGame->nDifficulty];

    for (int32_t i = 0; i < std::size(npcIndexMap); ++i)
    {
        if (npcIndexMap[i].nNpcClassId == nNpcId)
        {
            BITMANIP_MaskBitstate(pPlayerIntro->pNpcIntroFlags->pBuffer, npcIndexMap[i].nIndex);
        }
    }

    BITMANIP_MaskBitstate(pPlayerIntro->pNpcIntroFlags->pBuffer, 0);
}

//D2Game.0x6FC7F1A0
void __fastcall PLRINTRO_CopyQuestIntroFlagsToBuffer(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer)
{
    memcpy(pBuffer, pPlayerIntro->pQuestIntroFlags->pBuffer, bufferSize);
}

//D2Game.0x6FC7F1B0
void __fastcall PLRINTRO_CopyNpcIntroFlagsToBuffer(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer)
{
    memcpy(pBuffer, pPlayerIntro->pNpcIntroFlags->pBuffer, bufferSize);
}

//D2Game.0x6FC7F1C0
void __fastcall PLRINTRO_CopyBufferToQuestIntroFlags(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer)
{
    memcpy(pPlayerIntro->pQuestIntroFlags->pBuffer, pBuffer, bufferSize);
}

//D2Game.0x6FC7F1D0
void __fastcall PLRINTRO_CopyBufferToNpcIntroFlags(D2PlrIntroStrc* pPlayerIntro, uint8_t* pBuffer)
{
    memcpy(pPlayerIntro->pNpcIntroFlags->pBuffer, pBuffer, bufferSize);
}

//D2Game.0x6FC7F1E0
D2PlrIntroStrc* __fastcall PLRINTRO_Create(D2GameStrc* pGame)
{
    D2PlrIntroStrc* pPlayerIntro = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2PlrIntroStrc);
    D2BitBufferStrc* pBitBuffer1 = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2BitBufferStrc);
    uint8_t* pBuffer1 = (uint8_t*)FOG_AllocPool(pGame->pMemoryPool, bufferSize, __FILE__, __LINE__, 0);
    memset(pBuffer1, 0x00, bufferSize);

    BITMANIP_Initialize(pBitBuffer1, pBuffer1, bufferSize);
    pPlayerIntro->pQuestIntroFlags = pBitBuffer1;

    D2BitBufferStrc* pBitBuffer2 = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2BitBufferStrc);
    uint8_t* pBuffer2 = (uint8_t*)FOG_AllocPool(pGame->pMemoryPool, bufferSize, __FILE__, __LINE__, 0);
    memset(pBuffer2, 0x00, bufferSize);

    BITMANIP_Initialize(pBitBuffer2, pBuffer2, bufferSize);
    pPlayerIntro->pNpcIntroFlags = pBitBuffer2;

    return pPlayerIntro;
}

//D2Game.0x6FC7F2A0
void __fastcall PLRINTRO_Destroy(D2GameStrc* pGame, D2PlrIntroStrc* pPlayerIntro)
{
    if (pPlayerIntro->pQuestIntroFlags)
    {
        D2_FREE_POOL(pGame->pMemoryPool, pPlayerIntro->pQuestIntroFlags->pBuffer);
        D2_FREE_POOL(pGame->pMemoryPool, pPlayerIntro->pQuestIntroFlags);
        pPlayerIntro->pQuestIntroFlags = nullptr;
    }

    if (pPlayerIntro->pNpcIntroFlags)
    {
        D2_FREE_POOL(pGame->pMemoryPool, pPlayerIntro->pNpcIntroFlags->pBuffer);
        D2_FREE_POOL(pGame->pMemoryPool, pPlayerIntro->pNpcIntroFlags);
        pPlayerIntro->pNpcIntroFlags = nullptr;
    }

    D2_FREE_POOL(pGame->pMemoryPool, pPlayerIntro);
}
