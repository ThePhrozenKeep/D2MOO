#include "OBJECTS/ObjRgn.h"

#include <algorithm>

#include <D2DataTbls.h>
#include <DataTbls/LevelsTbls.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <D2Dungeon.h>


#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterRegion.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"


//D2Game.0x6FC7A3A0
int32_t __fastcall OBJRGN_AllocObjectControl(D2GameStrc* pGame)
{
    D2_ASSERT(pGame);

    pGame->pObjectControl = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2ObjectControlStrc);
    SEED_InitSeed(&pGame->pObjectControl->pSeed);

    const int32_t nObjectSeed = ITEMS_RollRandomNumber(&pGame->pGameSeed);

    SEED_InitLowSeed(&pGame->pObjectControl->pSeed, nObjectSeed);

    pGame->pObjectControl->pObjectRegion[0] = nullptr;
    pGame->pObjectControl->pObjectRoomCoord = nullptr;

    for (int32_t i = 1; i < sgptDataTables->nLevelsTxtRecordCount; ++i)
    {
        D2ObjectRegionStrc* pObjectRegion = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2ObjectRegionStrc);
        D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(i);
        memset(pObjectRegion, 0x00, sizeof(D2ObjectRegionStrc));
        pObjectRegion->nAct = pLevelsTxtRecord->nAct;
        pObjectRegion->nPopulatedRooms = INT_MAX;
        pObjectRegion->nTrapMonsterId = -1;
        pGame->pObjectControl->pObjectRegion[0] = pObjectRegion;
    }

    int32_t nShrineSubTypes[8] = {};
    for (int32_t i = 0; i < DATATBLS_GetShrinesTxtRecordCount(); ++i)
    {
        const uint8_t nEffectClass = DATATBLS_GetShrinesTxtRecord(i)->nEffectClass;
        if (nEffectClass < std::size(nShrineSubTypes))
        {
            ++nShrineSubTypes[nEffectClass];
        }
    }

    memset(pGame->pObjectControl->field_1048, 0, sizeof(pGame->pObjectControl->field_1048));
    memset(pGame->pObjectControl->shrineData.pShrineSubTypeIds, 0, sizeof(pGame->pObjectControl->shrineData.pShrineSubTypeIds));

    D2ShrineDataStrc* pShrineData = &pGame->pObjectControl->shrineData;
    for (int32_t i = 0; i < std::size(nShrineSubTypes); ++i)
    {
        if (nShrineSubTypes[i] > 0)
        {
            pShrineData->pShrineSubTypeIds[i] = (int32_t*)D2_CALLOC_POOL(pGame->pMemoryPool, sizeof(int32_t) * nShrineSubTypes[i]);
        }

        pShrineData->nShrineSubTypes[i] = 0;
    }

    for (int32_t i = 0; i < DATATBLS_GetShrinesTxtRecordCount(); ++i)
    {
        const uint8_t nEffectClass = DATATBLS_GetShrinesTxtRecord(i)->nEffectClass;
        if (nEffectClass < std::size(pGame->pObjectControl->shrineData.pShrineSubTypeIds))
        {
            pGame->pObjectControl->shrineData.pShrineSubTypeIds[nEffectClass][pGame->pObjectControl->shrineData.nShrineSubTypes[nEffectClass]] = i;
            ++pGame->pObjectControl->shrineData.nShrineSubTypes[nEffectClass];
        }
    }

    return nObjectSeed;
}

//D2Game.0x6FC7A5C0
void __fastcall OBJRGN_FreeObjectControl(D2GameStrc* pGame)
{
    D2_ASSERT(pGame);

    D2ObjectControlStrc* pObjectControl = pGame->pObjectControl;
    if (!pObjectControl)
    {
        return;
    }

    D2ObjectRoomCoordStrc* pObjectRoomCoord = pObjectControl->pObjectRoomCoord;
    while (pObjectRoomCoord)
    {
        D2ObjectRoomCoordStrc* pNext = pObjectRoomCoord->pNext;
        D2_FREE_POOL(pGame->pMemoryPool, pObjectRoomCoord);
        pObjectRoomCoord = pNext;
    }
    pObjectControl->pObjectRoomCoord = nullptr;

    for (int32_t i = 1; i < sgptDataTables->nLevelsTxtRecordCount; ++i)
    {
        D2_FREE_POOL(pGame->pMemoryPool, pObjectControl->pObjectRegion[i]);
    }

    for (int32_t i = 0; i < std::size(pObjectControl->shrineData.pShrineSubTypeIds); ++i)
    {
        if (pObjectControl->shrineData.pShrineSubTypeIds[i])
        {
            D2_FREE_POOL(pGame->pMemoryPool, pObjectControl->shrineData.pShrineSubTypeIds[i]);
        }
    }

    D2_FREE_POOL(pGame->pMemoryPool, pGame->pObjectControl);
}

//D2Game.0x6FC7A6B0
D2ObjectRegionStrc* __fastcall OBJRGN_GetObjectRegionForLevel(D2GameStrc* pGame, int32_t nLevelId)
{
    return pGame->pObjectControl->pObjectRegion[nLevelId];
}

//D2Game.0x6FC7A6C0
D2ObjectControlStrc* __fastcall OBJRGN_GetObjectControlFromGame(D2GameStrc* pGame)
{
    return pGame->pObjectControl;
}

//D2Game.0x6FC7A6D0
D2ShrineDataStrc* __fastcall OBJRGN_GetShrineDataFromGame(D2GameStrc* pGame)
{
    return &pGame->pObjectControl->shrineData;
}

//D2Game.0x6FC7A6E0
void __fastcall OBJRGN_AllocObjectRoomCoords(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom)
{
    D2ObjectRoomCoordStrc* pObjectRoomCoord = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2ObjectRoomCoordStrc);

    pObjectRoomCoord->nX = CLIENTS_GetUnitX(pUnit);
    pObjectRoomCoord->nY = CLIENTS_GetUnitY(pUnit);
    pObjectRoomCoord->pRoom = pRoom;
    pObjectRoomCoord->pNext = pGame->pObjectControl->pObjectRoomCoord;
    pGame->pObjectControl->pObjectRoomCoord = pObjectRoomCoord;
}

//D2Game.0x6FC7A780
void __fastcall OBJECTS_InitFunction17_Waypoint(D2ObjInitFnStrc* pOp)
{
    D2ObjectRoomCoordStrc* pObjectRoomCoord = pOp->pGame->pObjectControl->pObjectRoomCoord;
    while (pObjectRoomCoord)
    {
        if (pObjectRoomCoord->pRoom == pOp->pRoom
            || (pOp->pRoom && DungeonTestRoomGame(pOp->pRoom, pObjectRoomCoord->nX, pObjectRoomCoord->nY)))
        {
            if (!pOp->pObject || pOp->pObject->dwAnimMode == OBJMODE_NEUTRAL)
            {
                UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPERATING);

                D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->pObject ? pOp->pObject->dwClassId : -1);
                EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_ENDANIM, pOp->pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);
            }

            D2_FREE_POOL(pOp->pGame->pMemoryPool, pObjectRoomCoord);
            pOp->pGame->pObjectControl->pObjectRoomCoord = nullptr;
            return;
        }

        pObjectRoomCoord = pObjectRoomCoord->pNext;
    }

    if (DUNGEON_IsRoomInTown(pOp->pRoom))
    {
        UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
    }
}

//D2Game.0x6FC7A860
bool __fastcall OBJRGN_CanNotSpawnMoreWells(D2GameStrc* pGame, int32_t nLevelId)
{
    D2ObjectRegionStrc* pObjectRegion = pGame->pObjectControl->pObjectRegion[nLevelId];
    return pObjectRegion->nWells == 4 || pObjectRegion->nWells > pObjectRegion->nPopulatedRooms / 8;
}

//D2Game.0x6FC7A890
bool __fastcall OBJRGN_ShouldSpawnHealingShrineOrWell(D2GameStrc* pGame, int32_t nLevelId)
{
    D2ObjectRegionStrc* pObjectRegion = pGame->pObjectControl->pObjectRegion[nLevelId];
    return pObjectRegion->nPopulatedRooms > 0 && (pObjectRegion->field_4 << 7) / pObjectRegion->nPopulatedRooms > 96 && !pObjectRegion->nHealingShrines;
}

//D2Game.0x6FC7A8C0
bool __fastcall OBJRGN_CanNotSpawnMoreShrines(D2GameStrc* pGame, int32_t nLevelId)
{
    D2ObjectRegionStrc* pObjectRegion = pGame->pObjectControl->pObjectRegion[nLevelId];
    return pObjectRegion->nShrines == 10 || pObjectRegion->nShrines > pObjectRegion->nPopulatedRooms / 8;
}

//D2Game.0x6FC7A8F0
void __fastcall OBJRGN_IncreaseHealingShrineCount(D2GameStrc* pGame, int32_t nLevelId)
{
    D2ObjectRegionStrc* pObjectRegion = pGame->pObjectControl->pObjectRegion[nLevelId];
    ++pObjectRegion->nHealingShrines;
}

//D2Game.0x6FC7A900
bool __fastcall OBJRGN_CanSpawnWell(D2GameStrc* pGame, int32_t nLevelId, int32_t nX, int32_t nY)
{
    D2ObjectRegionStrc* pObjectRegion = pGame->pObjectControl->pObjectRegion[nLevelId];
    for (int32_t i = 0; i < pObjectRegion->nWells; ++i)
    {
        if (std::abs(pObjectRegion->wellCoordinates[i].nX - nX) < 100 || std::abs(pObjectRegion->wellCoordinates[i].nY - nY) < 100)
        {
            return false;
        }
    }

    return true;
}

//D2Game.0x6FC7A960
void __fastcall OBJRGN_SetWellCoordinates(D2GameStrc* pGame, int32_t nLevelId, int32_t nX, int32_t nY)
{
    D2ObjectRegionStrc* pObjectRegion = pGame->pObjectControl->pObjectRegion[nLevelId];
    if (pObjectRegion->nWells >= std::size(pObjectRegion->wellCoordinates))
    {
        return;
    }

    pObjectRegion->wellCoordinates[pObjectRegion->nWells].nX = nX;
    pObjectRegion->wellCoordinates[pObjectRegion->nWells].nY = nY;
    ++pObjectRegion->nWells;
}

//D2Game.0x6FC7A990
bool __fastcall OBJRGN_CanSpawnShrine(D2GameStrc* pGame, int32_t nLevelId, int32_t nX, int32_t nY)
{
    D2ObjectRegionStrc* pObjectRegion = pGame->pObjectControl->pObjectRegion[nLevelId];
    for (int32_t i = 0; i < pObjectRegion->nShrines; ++i)
    {
        if (std::abs(pObjectRegion->shrineCoordinates[i].nX - nX) < 50 || std::abs(pObjectRegion->shrineCoordinates[i].nY - nY) < 50)
        {
            return false;
        }
    }

    return true;
}

//D2Game.0x6FC7A9F0
void __fastcall OBJRGN_SetShrineCoordinates(D2GameStrc* pGame, int32_t nLevelId, int32_t nX, int32_t nY)
{
    D2ObjectRegionStrc* pObjectRegion = pGame->pObjectControl->pObjectRegion[nLevelId];
    if (pObjectRegion->nShrines >= std::size(pObjectRegion->shrineCoordinates))
    {
        return;
    }

    pObjectRegion->shrineCoordinates[pObjectRegion->nShrines].nX = nX;
    pObjectRegion->shrineCoordinates[pObjectRegion->nShrines].nY = nY;
    ++pObjectRegion->nShrines;
}

//D2Game.0x6FC7AA20
int32_t __fastcall OBJRGN_GetTrapMonsterId(D2ObjOperateFnStrc* pOp)
{
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pOp->pObject));
    D2ObjectRegionStrc* pObjectRegion = pOp->pGame->pObjectControl->pObjectRegion[nLevelId];

    if (pObjectRegion->nTrapMonsterId >= 0 && pObjectRegion->nTrapMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        return pObjectRegion->nTrapMonsterId;
    }

    constexpr int32_t nTrapMonsterIds1[] = { MONSTER_ZOMBIE1, MONSTER_MUMMY1 };
    constexpr int32_t nTrapMonsterIds2[] = { MONSTER_SKELETON1, MONSTER_SK_ARCHER1, MONSTER_SKMAGE_POIS1, MONSTER_SKMAGE_COLD1, MONSTER_SKMAGE_FIRE1, MONSTER_SKMAGE_LTNG1 };
    
    const int32_t* pTrapMonsterId = nTrapMonsterIds1;
    if (DRLG_GetActNoFromLevelId(nLevelId) == ACT_II)
    {
        pTrapMonsterId = &nTrapMonsterIds1[1];
    }

    D2MonsterRegionStrc* pMonsterRegion = MONSTERREGION_GetMonsterRegionFromLevelId(pOp->pGame->pMonReg, nLevelId);

    pObjectRegion->nTrapMonsterId = MONSTER_FLYINGSCIMITAR;
    
    for (int32_t j = 0; j < pMonsterRegion->nMonCount; ++j)
    {
        const int32_t nMonsterId = pMonsterRegion->pMonData[j].nMonHcIdx;

        const int32_t nTrapMonsterId = *pTrapMonsterId;
        if (nMonsterId >= nTrapMonsterId && nMonsterId < nTrapMonsterId + 5)
        {
            pObjectRegion->nTrapMonsterId = nTrapMonsterId;
            return nTrapMonsterId;
        }

        for (int32_t i = 0; i < std::size(nTrapMonsterIds2); ++i)
        {
            const int32_t nTrapMonsterId = nTrapMonsterIds2[i];
            if (nMonsterId >= nTrapMonsterId && nMonsterId < nTrapMonsterId + 4)
            {
                pObjectRegion->nTrapMonsterId = nTrapMonsterId;
                return nTrapMonsterId;
            }
        }
    }

    return pObjectRegion->nTrapMonsterId;
}
