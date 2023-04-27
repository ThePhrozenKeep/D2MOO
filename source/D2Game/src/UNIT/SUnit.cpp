#include "UNIT/SUnit.h"

#include <D2BitManip.h>

#include <D2Dungeon.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgPreset.h>
#include <Units/UnitRoom.h>
#include <D2StatList.h>
#include <D2Collision.h>
#include <D2States.h>
#include <D2Monsters.h>
#include <D2Items.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/LevelsIds.h>
#include <Path/PathMisc.h>


#include "AI/AiGeneral.h"
#include "GAME/Arena.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "GAME/Targets.h"
#include "INVENTORY/InvMode.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerList.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlrModes.h"
#include "QUESTS/Quests.h"
#include "SKILLS/SkillAss.h"
#include "UNIT/Party.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitEvent.h"
#include "UNIT/SUnitMsg.h"
#include "UNIT/SUnitProxy.h"


#pragma warning(disable: 28159)


int32_t dword_6FD4DC40;



//D2Game.0x6FCBAEE0
void __fastcall SUNIT_RemoveUnit(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);
    D2_ASSERT(pUnit->dwUnitType < 6);

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    {
        if (pUnit->nInteract)
        {
            D2UnitStrc* pInteractUnit = SUNIT_GetServerUnit(pGame, pUnit->dwInteractType, pUnit->dwInteractGUID);
            if (pInteractUnit)
            {
                if (pInteractUnit->dwUnitType == UNIT_MONSTER)
                {
                    MONSTERAI_RemoveInteractInfoFor(pGame, pUnit, pInteractUnit->pMonsterData ? pInteractUnit->pMonsterData->pMonInteract : nullptr);
                    SUNITPROXY_FreeVendorChain(pGame, pUnit, pInteractUnit);
                }
                else if (pInteractUnit->dwUnitType == UNIT_OBJECT)
                {
                    sub_6FC79C00(pGame, pInteractUnit);
                }

                pUnit->dwInteractGUID = -1;
                pUnit->dwInteractType = 6;
                pUnit->nInteract = 0;
            }
        }

        D2Common_10228(pUnit);
        break;
    }
    case UNIT_MONSTER:
    case UNIT_OBJECT:
    case UNIT_MISSILE:
    {
        D2Common_10228(pUnit);
        break;
    }
    default:
        break;
    }

    UNITROOM_RemoveUnitFromRoom(pUnit);

    D2UnitStrc** ppUnitList = nullptr;
    if (pUnit->dwUnitType == UNIT_TILE)
    {
        ppUnitList = &pGame->pTileList;
    }
    else
    {
        ppUnitList = SUNIT_GetUnitList(pUnit->dwUnitType, pGame, pUnit->dwUnitId);
    }

    D2UnitStrc* pPrevious = nullptr;
    for (D2UnitStrc* i = *ppUnitList; i; i = i->pListNext)
    {
        if (i == pUnit)
        {
            if (pPrevious)
            {
                pPrevious->pListNext = i->pListNext;
            }
            else
            {
                *ppUnitList = i->pListNext;
            }
            break;
        }
        pPrevious = i;
    }

    if (pUnit->pQuestEventList)
    {
        QUESTS_FreeChainRecord(pGame, pUnit->pQuestEventList);
        pUnit->pQuestEventList = nullptr;
    }

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    {
        D2GAME_FreeUnitNode_6FC40310(pGame, pUnit);
        sub_6FC7C170(pGame, pUnit);
        PARTY_LeaveParty(pGame, pUnit);
        PLAYER_Destroy(pGame, pUnit);

        if (pUnit->pDynamicPath)
        {
            PATH_FreeDynamicPath(pGame->pMemoryPool, pUnit->pDynamicPath);
            pUnit->pDynamicPath = nullptr;
        }
        break;
    }
    case UNIT_MONSTER:
    {
        D2GAME_TARGETS_Last_6FC40380(pGame, pUnit);

        if (pUnit->pDynamicPath)
        {
            PATH_FreeDynamicPath(pGame->pMemoryPool, pUnit->pDynamicPath);
            pUnit->pDynamicPath = nullptr;
        }

        MONSTER_Free(pGame, pUnit);
        break;
    }
    case UNIT_OBJECT:
    {
        OBJECTS_FreeHoverMessage(pGame, pUnit);
        UNITS_FreeStaticPath(pUnit);
        D2_FREE_POOL(pGame->pMemoryPool, pUnit->pPlayerData);
        break;
    }
    case UNIT_MISSILE:
    {
        if (pUnit->pDynamicPath)
        {
            PATH_FreeDynamicPath(pGame->pMemoryPool, pUnit->pDynamicPath);
            pUnit->pDynamicPath = nullptr;
        }

        MISSILES_Free(pGame, pUnit);
        break;
    }
    case UNIT_ITEM:
    {
        sub_6FC502B0(pGame, pUnit);
        ITEMS_FreeItemData(pGame->pMemoryPool, pUnit);
        UNITS_FreeStaticPath(pUnit);
        break;
    }
    case UNIT_TILE:
    {
        UNITS_FreeStaticPath(pUnit);
        break;
    }
    default:
        break;
    }

    SUNITEVENT_FreeTimerList(pGame, pUnit);
    D2GAME_SUNITMSG_FreeUnitMessages_6FCC6790(pUnit);
    UNITS_FreeUnit(pUnit);
}

//D2Game.0x6FCBB160
D2UnitStrc* __fastcall SUNIT_GetNextUnitFromList(D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit);

    return pUnit->pListNext;
}

//D2Game.0x6FCBB190
void __fastcall sub_6FCBB190(D2GameStrc* pGame, D2UnitStrc* pUnit, D2RoomStrc* pRoom)
{
    if (!D2Common_10355(pUnit))
    {
        return;
    }

    D2RoomStrc* pCurrentRoom = UNITS_GetRoom(pUnit);
    D2RoomStrc* pPreviousRoom = pRoom;
    if (pRoom)
    {
        if (pCurrentRoom == pPreviousRoom)
        {
            return;
        }
    }
    else
    {
        D2RoomStrc* pNextRoom = PATH_GetNextRoom(pUnit->pDynamicPath);

        const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pCurrentRoom);
        if (nLevelId)
        {
            D2RoomStrc* pTemp = DUNGEON_GetRoomFromAct(pGame->pAct[DRLG_GetActNoFromLevelId(nLevelId)]);
            while (pTemp)
            {
                if (pTemp == pNextRoom)
                {
                    pPreviousRoom = pNextRoom;
                    if (pNextRoom && pCurrentRoom == pPreviousRoom)
                    {
                        return;
                    }

                    break;
                }

                pTemp = pTemp->pRoomNext;
            }

            if (!pTemp)
            {
                PATH_ClearNextRoom(pUnit->pDynamicPath);
                pPreviousRoom = nullptr;
            }
        }
        else
        {
            PATH_ClearNextRoom(pUnit->pDynamicPath);
            pPreviousRoom = nullptr;
        }
    }

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        pUnit->pMonsterData->pVision = 0;
    }

    D2ClientStrc** ppClientsCurr = nullptr;
    int32_t nCurrClients = 0;
    if (pCurrentRoom)
    {
        ppClientsCurr = pCurrentRoom->ppClients;
        nCurrClients = pCurrentRoom->nNumClients;
    }

    D2ClientStrc** ppClientsPrev = nullptr;
    int32_t nPrevClients = 0;
    if (pPreviousRoom)
    {
        ppClientsPrev = pPreviousRoom->ppClients;
        nPrevClients = pPreviousRoom->nNumClients;
    }

    int32_t nCurrentCounter = 0;
    int32_t nPreviousCounter = 0;
    if (nCurrClients)
    {
        while (nPreviousCounter < nPrevClients && nCurrentCounter < nCurrClients)
        {
            D2_ASSERT(ppClientsCurr);
            D2_ASSERT(ppClientsPrev);

            D2ClientStrc* pCurrentClient = ppClientsCurr[nCurrentCounter];
            D2ClientStrc* pPreviousClient = ppClientsPrev[nPreviousCounter];
            if (pCurrentClient >= pPreviousClient)
            {
                if (pCurrentClient <= pPreviousClient)
                {
                    ++nCurrentCounter;
                }
                else
                {
                    if (pUnit != CLIENTS_GetPlayerFromClient(pPreviousClient, 0))
                    {
                        D2GAME_PACKETS_SendPacket0x0A_RemoveObject_6FCC5F80(pUnit, ppClientsPrev[nPreviousCounter]);
                    }
                }

                ++nPreviousCounter;
            }
            else
            {
                if (pUnit != CLIENTS_GetPlayerFromClient(pCurrentClient, 0))
                {
                    D2GAME_SUNITMSG_FirstFn_6FCC5520(pGame, pUnit, ppClientsCurr[nCurrentCounter]);
                }

                ++nCurrentCounter;
            }
        }

        while (nCurrentCounter < nCurrClients)
        {
            D2_ASSERT(ppClientsCurr);

            if (pUnit != CLIENTS_GetPlayerFromClient(ppClientsCurr[nCurrentCounter], 0))
            {
                D2GAME_SUNITMSG_FirstFn_6FCC5520(pGame, pUnit, ppClientsCurr[nCurrentCounter]);
            }

            ++nCurrentCounter;
        }
    }

    while (nPreviousCounter < nPrevClients)
    {
        D2_ASSERT(ppClientsPrev);

        if (pUnit != CLIENTS_GetPlayerFromClient(ppClientsPrev[nPreviousCounter], 0))
        {
            D2GAME_PACKETS_SendPacket0x0A_RemoveObject_6FCC5F80(pUnit, ppClientsPrev[nPreviousCounter]);
        }

        ++nPreviousCounter;
    }

    D2Common_10173(pUnit->pDynamicPath, 0);
}

//D2Game.0x6FCBB440
void __fastcall SUNIT_WarpPlayer(D2GameStrc* pGame, D2UnitStrc* pTarget, D2UnitStrc* pRoomTile)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pTarget);

    D2RoomStrc* pTilesRoom = UNITS_GetRoom(pRoomTile);

    D2LvlWarpTxt* pLvlWarpTxtRecord = nullptr;
    D2UnitStrc* pWarpTile = DUNGEON_GetWarpTileFromRoomAndSourceLevelId(pTilesRoom, pRoomTile ? pRoomTile->dwClassId : -1, &pLvlWarpTxtRecord);
    if (!pWarpTile)
    {
        return;
    }

    D2CoordStrc pCoord = {};
    D2RoomStrc* pWarpTileRoom = UNITS_GetRoom(pWarpTile);
    pCoord.nX = CLIENTS_GetUnitX(pWarpTile);
    pCoord.nY = CLIENTS_GetUnitY(pWarpTile);

    D2_ASSERT(pTarget->dwUnitType == UNIT_PLAYER);

    D2RoomStrc* pDestinationRoom = COLLISION_GetFreeCoordinates(pWarpTileRoom, &pCoord, UNITS_GetUnitSizeX(pTarget), 0x1C09u, 1);
    if (!pDestinationRoom)
    {
        return;
    }

    const int32_t nSourceLevel = DUNGEON_GetLevelIdFromRoom(pTilesRoom);
    const int32_t nDestinationLevel = DUNGEON_GetLevelIdFromRoom(pDestinationRoom);
    if ((nDestinationLevel == LEVEL_DURIELSLAIR || nDestinationLevel == LEVEL_THEWORLDSTONEKEEPLEV1 || nDestinationLevel == LEVEL_THEWORLDSTONECHAMBER || nDestinationLevel == LEVEL_GLACIALCAVESLEV1 || nDestinationLevel == LEVEL_DURANCEOFHATELEV1)
        && QUESTS_LevelWarpCheck(pGame, pTarget, nSourceLevel, nDestinationLevel))
    {
        return;
    }

    int32_t nX = pCoord.nX;
    int32_t nY = pCoord.nY;
    if (!sub_6FCBDFE0(pGame, pTarget, pWarpTileRoom, nX, nY, 0, 0))
    {
        return;
    }

    nX += pLvlWarpTxtRecord->dwExitWalkX;
    nY += pLvlWarpTxtRecord->dwExitWalkY;
    sub_6FC817D0(pGame, pTarget, 0, 2, nX, nY, 0);
    D2GAME_PACKETS_SendPacket0x0D_6FC3C920(UNITS_GetPlayerData(pTarget)->pClient, 0xDu, pTarget->dwUnitType, pTarget->dwUnitId, 1, nX, nY, 0, UNITS_GetCurrentLifePercentage(pTarget));
}

//D2Game.0x6FCBB630
void __fastcall SUNIT_InitSeed(D2UnitStrc* pUnit, D2SeedStrc* pSeed)
{
    D2_ASSERT(pUnit);

    SEED_InitSeed(&pUnit->pSeed);

    if (pSeed)
    {
        pUnit->dwInitSeed = ITEMS_RollRandomNumber(pSeed);
    }
    else
    {
        pUnit->dwInitSeed = SEED_GetRandomValue(2 * dword_6FD4DC40);
        ++dword_6FD4DC40;
    }

    SEED_InitLowSeed(&pUnit->pSeed, pUnit->dwInitSeed);
}

//D2Game.0x6FCBB6C0
D2UnitStrc* __fastcall SUNIT_AllocUnitData(int32_t nUnitType, int32_t nClassId, int32_t nX, int32_t nY, D2GameStrc* pGame, D2RoomStrc* pRoom, char a7, int32_t nMode, DWORD a3)
{
    if (nUnitType == UNIT_PLAYER)
    {
        if ((uint32_t)nClassId >= 7)
        {
            return nullptr;
        }
    }
    else if (nUnitType == UNIT_MONSTER)
    {
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
        if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_ENABLED]))
        {
            return nullptr;
        }
    }

    D2UnitStrc* pUnit = UNITS_AllocUnit(pGame->pMemoryPool, nUnitType);
    if (!pUnit)
    {
        return nullptr;
    }

    pUnit->dwUnitType = nUnitType;
    pUnit->dwClassId = nClassId;
    pUnit->pGame = pGame;
    pUnit->pMemoryPool = pGame->pMemoryPool;

    if (pGame->bExpansion)
    {
        pUnit->dwFlagEx |= UNITFLAGEX_ISEXPANSION;
    }
    else
    {
        pUnit->dwFlagEx &= ~UNITFLAGEX_ISEXPANSION;
    }
    pUnit->dwFlagEx |= UNITFLAGEX_SERVERUNIT;

    pUnit->nAct = DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom));
    pUnit->pDrlgAct = pGame->pAct[pUnit->nAct];
    pUnit->pQuestEventList = nullptr;

    if (nUnitType)
    {
        SUNIT_InitSeed(pUnit, pGame ? &pGame->pGameSeed : nullptr);

        if (nUnitType == UNIT_ITEM)
        {
            ITEMS_AllocItemData(pGame->pMemoryPool, pUnit);
            ITEMS_InitItemSeed(pUnit);
            ITEMS_SetItemStartSeed(pUnit, ITEMS_RollRandomNumber(&pGame->pGameSeed));
            SEED_InitLowSeed(ITEMS_GetItemSeed(pUnit), ITEMS_GetItemStartSeed(pUnit));
        }
    }

    pUnit->dwNodeIndex = 11;
    pUnit->dwFlags |= UNITFLAG_INITSEEDSET;

    uint32_t nUnitGUID = 0;
    if (nUnitType != UNIT_MONSTER)
    {
        D2_ASSERT(nUnitType < 6);

        nUnitGUID = pGame->dwSpawnedUnits[nUnitType] + 1;
        if (pGame->dwSpawnedUnits[nUnitType] == -2)
        {
            nUnitGUID = 1;
        }

        pGame->dwSpawnedUnits[nUnitType] = nUnitGUID;
    }
    else
    {
        if (!(a7 & 2))
        {
            nUnitGUID = pGame->dwSpawnedUnits[nUnitType] + 1;
            if (pGame->dwSpawnedUnits[nUnitType] == -2)
            {
                nUnitGUID = 1;
            }

            pGame->dwSpawnedUnits[nUnitType] = nUnitGUID;
        }
        else
        {
            nUnitGUID = a3;
        }
    }

    switch (nUnitGUID)
    {
    case UNIT_PLAYER:
    {
        PLAYER_Create(pGame, pUnit, nUnitGUID);

        if (nMode != PLRMODE_DEAD)
        {
            D2GAME_TARGETS_First_6FC40170(pGame, pUnit, 0);
        }
        break;
    }
    case UNIT_MONSTER:
    {
        pUnit->dwAnimMode = nMode;
        MONSTER_Initialize(pGame, pRoom, pUnit, nUnitGUID);
        break;
    }
    case UNIT_OBJECT:
    {
        pUnit->pObjectData = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2ObjectDataStrc);
        pUnit->pObjectData->pObjectTxt = DATATBLS_GetObjectsTxtRecord(nClassId);
        pUnit->dwAnimMode = nMode;
        UNITS_AllocStaticPath(pUnit);
        OBJECTS_InitHandler(pGame, pUnit, nUnitGUID, pRoom, nX, nY);
        break;
    }
    case UNIT_ITEM:
    {
        pUnit->dwAnimMode = nMode;
        UNITS_AllocStaticPath(pUnit);
        sub_6FC4BE80(pUnit, pGame, nUnitGUID, pRoom);
        break;
    }
    case UNIT_MISSILE:
    {
        pUnit->dwAnimMode = nMode;
        MISSILES_Initialize(pGame, pUnit, nUnitGUID);
        break;
    }
    case UNIT_TILE:
    {
        UNITS_AllocStaticPath(pUnit);
        pUnit->dwUnitId = nUnitGUID;
        pUnit->dwFlags |= UNITFLAG_TARGETABLE;
        break;
    }
    default:
        break;
    }

    if (a7 & 1)
    {
        SUNIT_Add(pUnit, nX, nY, pGame, pRoom, 1);

        if (nUnitType == UNIT_PLAYER)
        {
            if (pUnit->dwAnimMode == PLRMODE_DEAD || pUnit->dwAnimMode == PLRMODE_DEATH)
            {
                PATH_RemoveCollisionFootprintForUnit(pUnit, 1);
                PATH_SetUnitCollisionPattern(pUnit, COLLISION_PATTERN_SMALL_NO_PRESENCE);
                PATH_SetCollisionType(pUnit->pDynamicPath, 0x8000u);
            }
        }
        else if (nUnitType == UNIT_MONSTER)
        {
            if (MONSTERS_IsDead(pUnit))
            {
                D2MonStats2Txt* pMonStatsTxt2Record = MONSTERREGION_GetMonStats2TxtRecord(nClassId);
                if (!pMonStatsTxt2Record || !(pMonStatsTxt2Record->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_DEADCOL]))
                {
                    PATH_RemoveCollisionFootprintForUnit(pUnit, 1);
                    PATH_SetUnitCollisionPattern(pUnit, COLLISION_PATTERN_SMALL_NO_PRESENCE);
                    PATH_SetCollisionType(pUnit->pDynamicPath, 0x8000u);
                    return pUnit;
                }
            }
        }
    }

    return pUnit;
}

//D2Game.0x6FCBBB00
D2UnitStrc* __fastcall SUNIT_GetServerUnit(D2GameStrc* pGame, int32_t nUnitType, int32_t nUnitGUID)
{
    D2_ASSERT(pGame);

    D2UnitStrc** ppUnitList = nullptr;

    if (nUnitType == UNIT_TILE)
    {
        ppUnitList = &pGame->pTileList;
    }
    else
    {
        ppUnitList = SUNIT_GetUnitList(nUnitType, pGame, nUnitGUID);
    }

    for (D2UnitStrc* pUnit = *ppUnitList; pUnit; pUnit = pUnit->pListNext)
    {
        if (pUnit->dwUnitId == nUnitGUID)
        {
            return pUnit;
        }
    }

    return nullptr;
}

//D2Game.0x6FCBBB70
D2UnitStrc* __fastcall SUNIT_GetOwner(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit || !(pUnit->dwFlagEx & UNITFLAGEX_STOREOWNERINFO))
    {
        return nullptr;
    }
    
    return SUNIT_GetServerUnit(pGame, pUnit->dwOwnerType, pUnit->dwOwnerGUID);
}

//D2Game.0x6FCBBC00
D2UnitStrc* __fastcall SUNIT_GetLastAttacker(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit);

    if (!(pUnit->dwFlagEx & UNITFLAGEX_STORELASTATTACKER))
    {
        return nullptr;
    }

    return SUNIT_GetServerUnit(pGame, pUnit->dwKillerType, pUnit->dwKillerGUID);
}

//D2Game.0x6FCBBCB0
void __fastcall SUNIT_Add(D2UnitStrc* pUnit, int32_t nX, int32_t nY, D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t a6)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    pUnit->nAct = DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom));
    pUnit->pDrlgAct = pGame->pAct[pUnit->nAct];

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    {
        PATH_AllocDynamicPath(pGame->pMemoryPool, pRoom, nX, nY, pUnit, 0);
        sub_6FC7B7A0(pGame, pUnit, nX, nY);
        break;
    }
    case UNIT_MONSTER:
    {
        PATH_AllocDynamicPath(pGame->pMemoryPool, pRoom, nX, nY, pUnit, 0);
        MONSTER_SetVelocityAndPosition(pGame, pUnit, nX, nY, a6);
        D2RoomStrc* pCurrentRoom = UNITS_GetRoom(pUnit);
        if (pCurrentRoom && pCurrentRoom->nNumClients)
        {
            MONSTER_UpdateAiCallbackEvent(pGame, pUnit);
        }
        else
        {
            sub_6FCBC300(pGame, pUnit);
        }
        break;
    }
    case UNIT_OBJECT:
    {
        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);
        UNITS_InitializeStaticPath(pUnit, pRoom, nX, nY);
        if (pObjectsTxtRecord->nHasCollision[pUnit->dwAnimMode])
        {
            UNITS_BlockCollisionPath(pUnit, pRoom, nX, nY);
        }

        D2_ASSERT(pRoom);
        UNITROOM_AddUnitToRoom(pUnit, pRoom);
        break;
    }
    case UNIT_MISSILE:
    {
        PATH_AllocDynamicPath(pGame->pMemoryPool, pRoom, nX, nY, pUnit, 0);
        break;
    }
    case UNIT_ITEM:
    {
        if (pUnit->dwAnimMode == IMODE_ONGROUND)
        {
            UNITS_InitializeStaticPath(pUnit, pRoom, nX, nY);
            UNITS_BlockCollisionPath(pUnit, pRoom, nX, nY);

            if (pRoom)
            {
                UNITROOM_AddUnitToRoomEx(pUnit, pRoom, 0);
            }
        }
        break;
    }
    case UNIT_TILE:
    {
        UNITS_InitializeStaticPath(pUnit, pRoom, nX, nY);
        UNITS_BlockCollisionPath(pUnit, pRoom, nX, nY);
        
        D2_ASSERT(pRoom);
        UNITROOM_AddUnitToRoom(pUnit, pRoom);
        break;
    }
    default:
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
        return;
    }

    D2UnitStrc** ppUnitList = nullptr;
    if (pUnit->dwUnitType == UNIT_TILE)
    {
        ppUnitList = &pGame->pTileList;
    }
    else
    {
        ppUnitList = SUNIT_GetUnitList(pUnit->dwUnitType, pGame, pUnit->dwUnitId);
    }

    D2UnitStrc* i = *ppUnitList;
    D2UnitStrc* pPrevious = nullptr;
    while (i)
    {
        if (i->dwUnitId <= pUnit->dwUnitId)
        {
            if (i->dwUnitId == pUnit->dwUnitId)
            {
                char szMessage[256] = {};
                FOG_DisplayAssert(FOG_csprintf(szMessage, "Tried to add a duplicate unit - Type:%d Class:%d Id:%u\n", pUnit->dwUnitType, pUnit->dwClassId, pUnit->dwUnitId), __FILE__, __LINE__);
                exit(-1);
            }
            break;
        }

        pPrevious = i;
        i = i->pListNext;
    }

    pUnit->pListNext = i;

    if (pPrevious)
    {
        pPrevious->pListNext = pUnit;
    }
    else
    {
        *ppUnitList = pUnit;
    }

    if (UNITS_GetRoom(pUnit))
    {
        UNITROOM_RefreshUnit(pUnit);
    }

    if (!a6)
    {
        D2Common_10356(pUnit, 1);
    }
}

//D2Game.0x6FCBBFE0
void __fastcall SUNIT_Restore(D2GameStrc* pGame, D2UnitStrc* pUnit, D2RoomStrc* pRoom, int32_t nX, int32_t nY)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    pUnit->nAct = DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom));
    pUnit->pDrlgAct = pGame->pAct[pUnit->nAct];
    pUnit->dwFlagEx &= ~UNITFLAGEX_HASBEENDELETED;

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    {
        PATH_FreeDynamicPath(pGame->pMemoryPool, pUnit->pDynamicPath);
        pUnit->pDynamicPath = nullptr;

        PATH_AllocDynamicPath(pGame->pMemoryPool, pRoom, nX, nY, pUnit, 0);
        sub_6FC7B7A0(pGame, pUnit, nX, nY);

        if (pUnit->dwAnimMode == PLRMODE_DEAD || pUnit->dwAnimMode == PLRMODE_DEATH)
        {
            PATH_RemoveCollisionFootprintForUnit(pUnit, 1);
            PATH_SetUnitCollisionPattern(pUnit, COLLISION_PATTERN_SMALL_NO_PRESENCE);
            PATH_SetCollisionType(pUnit->pDynamicPath, 0x8000);
        }
        break;
    }
    case UNIT_MONSTER:
    {
        PATH_FreeDynamicPath(pGame->pMemoryPool, pUnit->pDynamicPath);
        pUnit->pDynamicPath = nullptr;

        PATH_AllocDynamicPath(pGame->pMemoryPool, pRoom, nX, nY, pUnit, 0);
        MONSTER_SetVelocityAndPosition(pGame, pUnit, nX, nY, 0);

        D2RoomStrc* pUnitRoom = UNITS_GetRoom(pUnit);
        if (pUnitRoom && pUnitRoom->nNumClients)
        {
            MONSTER_UpdateAiCallbackEvent(pGame, pUnit);
        }
        else
        {
            sub_6FCBC300(pGame, pUnit);
        }

        if (MONSTERS_IsDead(pUnit))
        {
            PATH_RemoveCollisionFootprintForUnit(pUnit, 1);
            PATH_SetUnitCollisionPattern(pUnit, COLLISION_PATTERN_SMALL_NO_PRESENCE);
            PATH_SetCollisionType(pUnit->pDynamicPath, 0x8000);
        }
        break;
    }
    case UNIT_OBJECT:
    {
        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId);
        UNITS_InitializeStaticPath(pUnit, pRoom, nX, nY);

        if (pObjectsTxtRecord->nHasCollision[pUnit->dwAnimMode])
        {
            UNITS_BlockCollisionPath(pUnit, pRoom, nX, nY);
        }

        UNITROOM_AddUnitToRoom(pUnit, pRoom);
        break;
    }
    case UNIT_ITEM:
    {
        if (pUnit->dwAnimMode == IMODE_ONGROUND)
        {
            UNITS_InitializeStaticPath(pUnit, pRoom, nX, nY);
            UNITS_BlockCollisionPath(pUnit, pRoom, nX, nY);
            UNITROOM_AddUnitToRoom(pUnit, pRoom);
            pUnit->pItemData->dwActionStamp = ITEMS_GetGroundRemovalTime(pGame, pUnit);
        }
        break;
    }
    case UNIT_MISSILE:
    {
        PATH_FreeDynamicPath(pGame->pMemoryPool, pUnit->pDynamicPath);
        pUnit->pDynamicPath = nullptr;
        PATH_AllocDynamicPath(pGame->pMemoryPool, pRoom, nX, nY, pUnit, 0);
        break;
    }
    case UNIT_TILE:
    {
        UNITS_InitializeStaticPath(pUnit, pRoom, nX, nY);
        UNITS_BlockCollisionPath(pUnit, pRoom, nX, nY);
        UNITROOM_AddUnitToRoom(pUnit, pRoom);
        break;
    }
    default:
        FOG_DisplayHalt("Illegal unit type", __FILE__, __LINE__);
        exit(-1);
    }

    if (UNITS_GetRoom(pUnit))
    {
        UNITROOM_RefreshUnit(pUnit);
    }

    D2Common_10356(pUnit, 1);
}

//D2Game.0x6FCBC280
void __fastcall SUNIT_InitClientInPlayerData(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    D2_ASSERT(pUnit);
    D2_ASSERT(pUnit->dwUnitType == UNIT_PLAYER);

    UNITS_GetPlayerData(pUnit)->pClient = pClient;
}

//D2Game.0x6FCBC2E0
D2ClientStrc* __fastcall SUNIT_GetClientFromPlayer(D2UnitStrc* pPlayer, const char* szFile, int32_t nLine)
{
    if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER)
    {
        return nullptr;
    }

    return UNITS_GetPlayerData(pPlayer)->pClient;
}

//D2Game.0x6FCBC300
void __fastcall sub_6FCBC300(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit);

    STATLIST_FreeModStats(pUnit);
    D2GAME_SUNITMSG_FreeUnitMessages_6FCC6790(pUnit);
    pUnit->dwFlags &= ~(UNITFLAG_DOUPDATE | UNITFLAG_INITSEEDSET);
    D2Common_10356(pUnit, 0);
    pUnit->dwFlags &= ~(UNITFLAG_HASEVENTSOUND | UNITFLAG_UPGRLIFENHITCLASS);
    pUnit->dwFlagEx &= 0xFFDEE7FF;
    STATES_ClearGfxStateFlags(pUnit);
    D2GAME_INVMODE_Last_6FC416D0(pGame, pUnit);

    if (D2COMMON_10530_D2CheckStatlistFlagDMGRed(pUnit))
    {
        STATLIST_RemoveAllStatsFromOverlay(pUnit);
    }

    if (D2COMMON_10530_D2CheckStatlistFlagDMGRed(pUnit))
    {
        STATLIST_RemoveAllStatsFromOverlay(pUnit);
    }

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    {
        STATLIST_SetUnitStat(pUnit, STAT_LASTEXP, -1, 0);
        pUnit->dwFlags &= ~UNITFLAG_HASTXTMSG;

        D2_ASSERT(pUnit && pUnit->dwUnitType == UNIT_PLAYER);

        D2ArenaUnitStrc* pArenaUnit = UNITS_GetPlayerData(pUnit)->pArenaUnit;

        D2_ASSERT(pArenaUnit);

        pArenaUnit->bUpdateScore = 0;
        break;
    }
    case UNIT_MONSTER:
    {
        pUnit->dwFlags &= ~UNITFLAG_HASTXTMSG;
        if (pUnit->dwFlags & UNITFLAG_SUMMONER)
        {
            pUnit->dwFlags &= ~UNITFLAG_SUMMONER;
            MONSTER_ToggleSummonerFlag(pUnit, 1, 0);
        }
        pUnit->dwFlagEx &= ~UNITFLAGEX_TELEPORTED;
        break;
    }
    case UNIT_OBJECT:
    {
        pUnit->dwFlags &= ~UNITFLAG_HASTXTMSG;
        break;
    }
    case UNIT_ITEM:
    {
        pUnit->dwFlags &= ~UNITFLAG_SENDREFRESHMSG;
        ITEMS_SetItemFlag(pUnit, IFLAG_QUANTITY, 0);
        ITEMS_SetItemFlag(pUnit, IFLAG_INSTORE, 0);
        break;
    }
    default:
        return;
    }
}

//D2Game.0x6FCBC480
void __fastcall SUNIT_AttachSound(D2UnitStrc* pUnit, int32_t nUpdateType, D2UnitStrc* pUpdateUnit)
{
    D2_ASSERT(pUnit);

    pUnit->nUpdateType = nUpdateType;
    pUnit->pUpdateUnit = pUpdateUnit;
    UNITROOM_RefreshUnit(pUnit);
    pUnit->dwFlags |= UNITFLAG_HASEVENTSOUND;
}

//D2Game.0x6FCBC4D0
void __fastcall sub_6FCBC4D0(D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit);

    pUnit->nActionFrame = 0;

    const bool bSequence = (pUnit->dwUnitType == UNIT_MONSTER && pUnit->dwAnimMode == MONMODE_SEQUENCE || pUnit->dwUnitType == UNIT_PLAYER && pUnit->dwAnimMode == PLRMODE_SEQUENCE);

    if (bSequence && DATATBLS_GetMonSeqTxtRecordFromUnit(pUnit))
    {
        UNITS_InitializeSequence(pUnit);
        D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
    }
    else
    {
        pUnit->pAnimSeq = 0;
        pUnit->dwSeqCurrentFrame = 0;
        if (pUnit->pDynamicPath)
        {
            D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
            pUnit->dwFrameCount = pUnit->pAnimData->dwFrames << 8;
        }
        else
        {
            UNITS_SetAnimData(pUnit, pUnit->dwUnitType, pUnit->dwClassId, pUnit->dwAnimMode);
            pUnit->dwFrameCount = pUnit->pAnimData->dwFrames << 8;
        }
    }
}

//D2Game.0x6FCBC590
D2UnitStrc* __fastcall SUNIT_CreatePresetUnit(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nUnitType, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode, int32_t nUnitFlags)
{
    D2UnitStrc* pUnit = nullptr;
    if (nUnitType == UNIT_MONSTER)
    {
        pUnit = D2GAME_SpawnPresetMonster_6FC66560(pGame, pRoom, nClassId, nX, nY, nMode);
    }
    else if (nUnitType == UNIT_OBJECT)
    {
        if (nClassId == 573)
        {
            FOG_Trace(" could not place object due to invalid preset data\n");
            return nullptr;
        }

        if (nClassId > 573)
        {
            pUnit = OBJECTS_SpawnPresetObject(pGame, pRoom, nClassId, nX, nY, nMode);
        }
        else
        {
            pUnit = SUNIT_AllocUnitData(nUnitType, nClassId, nX, nY, pGame, pRoom, 1, nMode, 0);
        }
    }
    else
    {
        pUnit = SUNIT_AllocUnitData(nUnitType, nClassId, nX, nY, pGame, pRoom, 1, nMode, 0);

        if (nUnitType == UNIT_ITEM)
        {
            if (!pUnit)
            {
                return nullptr;
            }

            D2ItemDropStrc itemDrop = {};
            itemDrop.nQuality = ITEMQUAL_NORMAL;
            itemDrop.nItemLvl = DUNGEON_GetLevelIdFromRoom(pRoom);
            if (!D2GAME_InitItemStats_6FC4E520(pGame, &pUnit, &itemDrop, 1))
            {
                SUNIT_RemoveUnit(pGame, pUnit);
                return nullptr;
            }
        }
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_ISRESURRECT | UNITFLAG_ISINIT;
    }

    return pUnit;
}

//D2Game.0x6FCBC6F0
void __fastcall SUNIT_SpawnPresetUnit(D2GameStrc* pGame, D2RoomStrc* pRoom, D2PresetUnitStrc* pPresetUnit)
{
    D2DrlgCoordsStrc pCoord = {};

    DUNGEON_GetRoomCoordinates(pRoom, &pCoord);

    D2UnitStrc* pUnit = SUNIT_CreatePresetUnit(pGame, pRoom, pPresetUnit->nUnitType, pPresetUnit->nIndex, pCoord.dwSubtilesLeft + pPresetUnit->nXpos, pCoord.dwSubtilesTop + pPresetUnit->nYpos, pPresetUnit->nMode, 0);
    if (!pUnit)
    {
        return;
    }

    if (pPresetUnit->nUnitType == UNIT_MONSTER)
    {
        D2MapAIStrc** ppMapAI = AIGENERAL_GetMapAiFromUnit(pUnit);
        if (pPresetUnit->pMapAI)
        {
            DRLGPRESET_ChangeMapAI(&pPresetUnit->pMapAI, ppMapAI);
        }
    }
    else if (pPresetUnit->nUnitType == UNIT_OBJECT)
    {
        QUESTS_MapPresetSpawn(pGame, pUnit, pPresetUnit->pMapAI);
    }
}

//D2Game.0x6FCBC780
void __fastcall SUNIT_SpawnPresetUnitsInRoom(D2GameStrc* pGame, D2RoomStrc* pRoom)
{
    if (!pGame)
    {
        return;
    }

    D2PresetUnitStrc* pPresetUnit = DUNGEON_GetPresetUnitsFromRoom(pRoom);
    for (D2PresetUnitStrc* i = pPresetUnit; i; i = i->pNext)
    {
        if (i->nUnitType != UNIT_MONSTER && !(i->bSpawned & 1))
        {
            SUNIT_SpawnPresetUnit(pGame, pRoom, i);
        }
    }

    for (D2PresetUnitStrc* i = pPresetUnit; i; i = i->pNext)
    {
        if (i->nUnitType == UNIT_MONSTER && !(i->bSpawned & 1))
        {
            SUNIT_SpawnPresetUnit(pGame, pRoom, i);
        }
    }
}

//D2Game.0x6FCBC7E0
void __fastcall sub_6FCBC7E0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit);
    D2_ASSERT(pGame);

    D2DynamicPathStrc* pPath = pUnit->pDynamicPath;
    D2_ASSERT(pPath);

    if (!PATH_GetTargetUnit(pUnit->pDynamicPath))
    {
        return;
    }

    int32_t nTargetType = 0;
    D2UnitGUID nTargetGUID = 0;
    PATH_GetTargetTypeAndGUID(pPath, &nTargetType, &nTargetGUID);

    D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, nTargetType, nTargetGUID);
    if (pTarget != PATH_GetTargetUnit(pPath))
    {
        PATH_SetTargetUnit(pPath, nullptr);
    }

    if (!pTarget || pTarget->dwUnitType != UNIT_ITEM)
    {
        return;
    }

    const int32_t nAnimMode = pTarget->dwAnimMode;
    if (nAnimMode == IMODE_INBELT || nAnimMode == IMODE_EQUIP)
    {
        PATH_SetTargetUnit(pPath, nullptr);
    }
}

//D2Game.0x6FCBC900
D2UnitStrc* __stdcall SUNIT_GetTargetUnit(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    sub_6FCBC7E0(pGame, pUnit);
    D2UnitStrc* pTargetUnit = PATH_GetTargetUnit(pUnit->pDynamicPath);
    if (pTargetUnit != pUnit)
    {
        return pTargetUnit;
    }

    return nullptr;
}

//D2Game.0x6FCBC930
int32_t __fastcall sub_6FCBC930(D2GameStrc* pGame, D2UnitStrc* pMissile)
{
    D2_ASSERT(pGame);

    sub_6FCBC7E0(pGame, pMissile);
    const int32_t v4 = 2 - (D2Common_10226(pMissile, 1024) != 0);
    if (D2Common_10355(pMissile))
    {
        sub_6FCBB190(pGame, pMissile, 0);
    }

    return v4;
}

//D2Game.0x6FCBC9A0
void __fastcall SUNIT_SetCombatMode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMode)
{
    SUNITDMG_FreeAttackerCombatList(pGame, pUnit);
    UNITS_ChangeAnimMode(pUnit, nMode);
}

//D2Game.0x6FCBC9C0
void __fastcall sub_6FCBC9C0(D2UnitStrc* pFirst, D2UnitStrc* pSecond)
{
    D2_ASSERT(pFirst);
    D2_ASSERT(pSecond);

    pFirst->pObjectData->DestPortalCoords.nX = CLIENTS_GetUnitX(pSecond);
    pFirst->pObjectData->DestPortalCoords.nY = CLIENTS_GetUnitY(pSecond);

    pSecond->pObjectData->DestPortalCoords.nX = CLIENTS_GetUnitX(pFirst);
    pSecond->pObjectData->DestPortalCoords.nY = CLIENTS_GetUnitY(pFirst);

    D2RoomStrc* pSecondRoom = UNITS_GetRoom(pSecond);
    if (pSecondRoom)
    {
        D2DrlgCoordsStrc drlgCoords = {};
        DUNGEON_GetRoomCoordinates(pSecondRoom, &drlgCoords);
        pFirst->pObjectData->DestRoomCooords.nX = drlgCoords.dwTilesLeft;
        pFirst->pObjectData->DestRoomCooords.nY = drlgCoords.dwTilesTop;
    }

    D2RoomStrc* pFirstRoom = UNITS_GetRoom(pFirst);
    if (pFirstRoom)
    {
        D2DrlgCoordsStrc drlgCoords = {};
        DUNGEON_GetRoomCoordinates(pFirstRoom, &drlgCoords);
        pSecond->pObjectData->DestRoomCooords.nX = drlgCoords.dwTilesLeft;
        pSecond->pObjectData->DestRoomCooords.nY = drlgCoords.dwTilesTop;
    }
}

//D2Game.0x6FCBCB30
D2UnitStrc* __fastcall SUNIT_GetPortalOwner(D2GameStrc* pGame, D2UnitStrc* pPortal)
{
    D2_ASSERT(pPortal);

    const uint8_t nActNo = DRLG_GetActNoFromLevelId(pPortal->pObjectData->InteractType);
    const int32_t nX = pPortal->pObjectData->DestRoomCooords.nX;
    const int32_t nY = pPortal->pObjectData->DestRoomCooords.nY;

    if (!DUNGEON_FindRoomBySubtileCoordinates(pGame->pAct[nActNo], nX, nY))
    {
        D2RoomStrc* pRoom = DUNGEON_StreamRoomAtCoords(pGame->pAct[nActNo], nX, nY);
        if (pRoom)
        {
            sub_6FC385A0(pGame, pRoom);
        }
    }

    return SUNIT_GetServerUnit(pGame, pPortal->dwOwnerType, pPortal->dwOwnerGUID);
}

//D2Game.0x6FCBCC40
void __fastcall SUNIT_IterateUnitsOfType(D2GameStrc* pGame, int32_t nType, void* pArg, int32_t(__fastcall* pfIterate)(D2GameStrc*, D2UnitStrc*, void*))
{
    D2_ASSERT(pGame);
    D2_ASSERT(pfIterate);

    D2UnitStrc** ppUnitList = nullptr;
    if (nType == UNIT_PLAYER)
    {
        ppUnitList = pGame->pUnitList[0];
    }
    else if (nType == UNIT_MONSTER)
    {
        ppUnitList = pGame->pUnitList[1];
    }
    else
    {
        return;
    }

    if (IsBadCodePtr((FARPROC)pfIterate))
    {
        FOG_DisplayAssert("pfnIterate", __FILE__, __LINE__);
        exit(-1);
    }

    for (int32_t i = 0; i < 128; ++i)
    {
        for (D2UnitStrc* pUnit = ppUnitList[i]; pUnit; pUnit = pUnit->pListNext)
        {
            if (!STATES_CheckState(pUnit, STATE_PLAYERBODY) && pfIterate(pGame, pUnit, pArg) == 1)
            {
                return;
            }

            D2_ASSERT(pUnit);
        }
    }
}

//D2Game.0x6FCBCD70
void __fastcall SUNIT_IterateLivingPlayers(D2GameStrc* pGame, void(__fastcall* pfIterate)(D2GameStrc*, D2UnitStrc*, void*), void* pArg)
{
    D2_ASSERT(pGame && pfIterate);

    for (int32_t i = 0; i < 128; ++i)
    {
        for (D2UnitStrc* pUnit = pGame->pUnitList[0][i]; pUnit; pUnit = pUnit->pListNext)
        {
            if (!STATES_CheckState(pUnit, STATE_PLAYERBODY))
            {
                pfIterate(pGame, pUnit, pArg);
            }

            D2_ASSERT(pUnit);
        }
    }
}

//D2Game.0x6FCBCE30
void __fastcall SUNIT_SetTimerOnUnit(D2UnitStrc* pUnit, D2EventTimerStrc* pTimer)
{
    if (pUnit)
    {
        pUnit->pTimer = pTimer;
    }
}

//D2Game.0x6FCBCE40
D2EventTimerStrc* __fastcall SUNIT_GetTimerFromUnit(D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        return pUnit->pTimer;
    }

    return nullptr;
}

//D2Game.0x6FCBCE50
void __fastcall D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_MODECHANGE, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, 0);
}

//D2Game.0x6FCBCE70
void __fastcall sub_6FCBCE70(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    int32_t nFrame = pGame->dwGameFrame;
    const int32_t nFrameBonus = UNITS_GetFrameBonus(pUnit) << 8;

    int32_t nAnimSpeed = 0;
    int32_t nFrameCount = 0;
    if (pUnit->pAnimSeq)
    {
        nAnimSpeed = pUnit->dwSeqSpeed;
        nFrameCount = pUnit->dwSeqFrameCount;
    }
    else
    {
        nAnimSpeed = pUnit->wAnimSpeed;
        nFrameCount = pUnit->dwFrameCount;
    }

    if (nAnimSpeed)
    {
        int32_t nIndex = nFrameBonus >> 8;
        int32_t nParam = 0;
        int32_t nAnimFrame = nAnimSpeed + nFrameBonus;
        while (nAnimFrame < nFrameCount)
        {
            ++nFrame;
            for (int32_t i = nAnimFrame >> 8; nIndex <= i; ++nIndex)
            {
                if (nIndex >= 144)
                {
                    break;
                }

                D2MonSeqTxt* pSequence = (D2MonSeqTxt*)pUnit->pAnimSeq;
                int32_t nEvent = 0;
                if (pSequence)
                {
                    DATATBLS_GetSequenceEvent(pSequence, nIndex << 8, &nEvent);
                }
                else
                {
                    nEvent = pUnit->pAnimData->pFrameFlags[nIndex];
                }

                switch (nEvent)
                {
                case 1:
                case 2:
                case 4:
                {
                    EVENT_SetEvent(pGame, pUnit, 0, nFrame, nEvent, nParam);
                    ++nParam;
                    break;
                }
                case 3:
                {
                    EVENT_SetEvent(pGame, pUnit, 0, nFrame, nEvent, 0);
                    break;
                }
                default:
                    break;
                }
            }

            nAnimFrame += nAnimSpeed;
        }

        if (nFrame == pGame->dwGameFrame)
        {
            ++nFrame;
        }

        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, nFrame + 1, 0, 0);
        pUnit->dwSeqCurrentFrame = pGame->dwGameFrame << 8;
    }
    else
    {
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, pGame->dwGameFrame + 1, 0, 0);
    }
}

//D2Game.0x6FCBCFD0
void __fastcall sub_6FCBCFD0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3)
{
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 0, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 1, 0);

    const int32_t nCalc = (100 - a3) * (pGame->dwGameFrame - (pUnit->dwSeqCurrentFrame >> 8)) / 100;
    pUnit->dwSeqCurrentFrame = (pGame->dwGameFrame - nCalc) << 8;

    int32_t nAnimSpeed = 0;
    int32_t nFrameCount = 0;
    if (pUnit->pAnimSeq)
    {
        nAnimSpeed = pUnit->dwSeqSpeed;
        nFrameCount = pUnit->dwSeqFrameCount;
    }
    else
    {
        nAnimSpeed = pUnit->wAnimSpeed;
        nFrameCount = pUnit->dwFrameCount;
    }

    if (nAnimSpeed)
    {
        int32_t nFrame = pGame->dwGameFrame;
        int32_t nIndex = nCalc - 1;
        int32_t nAnimFrame = (nCalc << 8) + nAnimSpeed;
        while (nAnimFrame < nFrameCount)
        {
            ++nFrame;

            int32_t nEvent = 0;
            while (nIndex <= (nAnimFrame >> 8))
            {
                D2MonSeqTxt* pSequence = (D2MonSeqTxt*)pUnit->pAnimSeq;
                if (pSequence)
                {
                    DATATBLS_GetSequenceEvent(pSequence, nIndex << 8, &nEvent);
                }
                else
                {
                    nEvent = pUnit->pAnimData->pFrameFlags[nIndex];
                }

                if (nEvent > 0 && nEvent <= 4)
                {
                    EVENT_SetEvent(pGame, pUnit, 0, nFrame, nEvent, 0);
                }

                ++nIndex;
            }

            nAnimFrame += nAnimSpeed;
        }

        if (nFrame == pGame->dwGameFrame)
        {
            ++nFrame;
        }

        EVENT_SetEvent(pGame, pUnit, 1, nFrame + 1, 0, 0);
    }
    else
    {
        EVENT_SetEvent(pGame, pUnit, 1, pGame->dwGameFrame + 1, 0, 0);
    }
}

//D2Game.0x6FCBD120
void __fastcall sub_6FCBD120(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3)
{
    if (a3 <= 0)
    {
        return;
    }

    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 0, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 1, 0);

    const int32_t nCalc = pGame->dwGameFrame - (pUnit->dwSeqCurrentFrame >> 8) - a3;
    pUnit->dwSeqCurrentFrame = (pGame->dwGameFrame - nCalc) << 8;

    int32_t nAnimSpeed = 0;
    int32_t nFrameCount = 0;
    if (pUnit->pAnimSeq)
    {
        nAnimSpeed = pUnit->dwSeqSpeed;
        nFrameCount = pUnit->dwSeqFrameCount;
    }
    else
    {
        nAnimSpeed = pUnit->wAnimSpeed;
        nFrameCount = pUnit->dwFrameCount;
    }

    if (nAnimSpeed)
    {
        int32_t nFrame = pGame->dwGameFrame;
        int32_t nIndex = nCalc - 1;
        int32_t nAnimFrame = (nCalc << 8) + nAnimSpeed;
        while (nAnimFrame < nFrameCount)
        {
            ++nFrame;

            int32_t nEvent = 0;
            while (nIndex <= (nAnimFrame >> 8))
            {
                D2MonSeqTxt* pSequence = (D2MonSeqTxt*)pUnit->pAnimSeq;
                if (pSequence)
                {
                    DATATBLS_GetSequenceEvent(pSequence, nIndex << 8, &nEvent);
                }
                else
                {
                    nEvent = pUnit->pAnimData->pFrameFlags[nIndex];
                }

                if (nEvent > 0 && nEvent <= 4)
                {
                    EVENT_SetEvent(pGame, pUnit, 0, nFrame, nEvent, 0);
                }

                ++nIndex;
            }

            nAnimFrame += nAnimSpeed;
        }

        if (nFrame == pGame->dwGameFrame)
        {
            ++nFrame;
        }

        EVENT_SetEvent(pGame, pUnit, 1, nFrame + 1, 0, 0);
    }
    else
    {
        EVENT_SetEvent(pGame, pUnit, 1, pGame->dwGameFrame + 1, 0, 0);
    }
}

//D2Game.0x6FCBD260
void __fastcall D2GAME_SKILLS_RewindSkillEx_6FCBD260(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3)
{
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_MODECHANGE, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, 0);

    pUnit->dwSeqCurrentFrame = (pGame->dwGameFrame - a3) << 8;

    int32_t nAnimSpeed = 0;
    int32_t nFrameCount = 0;
    if (pUnit->pAnimSeq)
    {
        nAnimSpeed = pUnit->dwSeqSpeed;
        nFrameCount = pUnit->dwSeqFrameCount;
    }
    else
    {
        nAnimSpeed = pUnit->wAnimSpeed;
        nFrameCount = pUnit->dwFrameCount;
    }

    if (nAnimSpeed)
    {
        int32_t nFrame = pGame->dwGameFrame;
        int32_t nIndex = a3 - 1;
        int32_t nAnimFrame = (a3 << 8) + nAnimSpeed;
        while (nAnimFrame < nFrameCount)
        {
            while (nIndex <= (nAnimFrame >> 8))
            {
                D2MonSeqTxt* pSequence = (D2MonSeqTxt*)pUnit->pAnimSeq;
                int32_t nEvent = 0;
                if (pSequence)
                {
                    DATATBLS_GetSequenceEvent(pSequence, nIndex << 8, &nEvent);
                }
                else
                {
                    nEvent = pUnit->pAnimData->pFrameFlags[nIndex];
                }

                if (nEvent > 0 && nEvent <= 4)
                {
                    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_MODECHANGE, nFrame + 1, nEvent, 0);
                }

                ++nIndex;
            }

            nAnimFrame += nAnimSpeed;
            ++nFrame;
        }

        if (nFrame == pGame->dwGameFrame)
        {
            ++nFrame;
        }

        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, nFrame + 1, 0, 0);
    }
    else
    {
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM, pGame->dwGameFrame + 1, 0, 0);
    }
}

//D2Game.0x6FCBD3A0
void __fastcall sub_6FCBD3A0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_MODECHANGE, 0);
    sub_6FC35570(pGame, pUnit, 0, 0, 0);
}

//D2Game.0x6FCBD3D0
void __fastcall SUNIT_FillUnitInfo(D2UnitStrc* pUnit, D2UnitInfoStrc* pInfo)
{
    D2_ASSERT(pInfo);
    D2_ASSERT(pUnit);

    pInfo->szName[0] = 0;
    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
        if (pPlayerData)
        {
            strcpy_s(pInfo->szName, pPlayerData->szName);
        }
    }

    pInfo->nUnitType = pUnit->dwUnitType;
    pInfo->nClassId = pUnit->dwClassId;
    pInfo->nUnitGUID = pUnit->dwUnitId;
    pInfo->nAnimMode = pUnit->dwAnimMode;
    pInfo->nX = CLIENTS_GetUnitX(pUnit);
    pInfo->nY = CLIENTS_GetUnitY(pUnit);
    pInfo->nOwnerType = pUnit->dwOwnerType;
    pInfo->nOwnerGUID = pUnit->dwOwnerGUID;
}

//D2Game.0x6FCBD4D0
D2UnitStrc* __fastcall SUNIT_GetInteractUnit(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit->nInteract)
    {
        return nullptr;
    }

    return SUNIT_GetServerUnit(pGame, pUnit->dwInteractType, pUnit->dwInteractGUID);
}

//D2Game.0x6FCBD550
void __fastcall SUNIT_RemoveAllItemsFromInventory(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit);

    D2InventoryStrc* pInventory = pUnit->pInventory;
    if (!pInventory)
    {
        return;
    }

    D2UnitStrc* i = INVENTORY_GetFirstItem(pInventory);
    while (i)
    {
        D2UnitStrc* pNext = INVENTORY_GetNextItem(i);

        D2UnitStrc* pItem = INVENTORY_UnitIsItem(i);
        D2_ASSERT(pItem);

        if (pItem->dwUnitType == UNIT_ITEM)
        {
            D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pItem, pUnit, 0, 1);

            //if (INVENTORY_GetItemNodePage(i) == 3)
            //{
            //    D2COMMON_10254_Return0(pInventory, INVENTORY_GetBodyLocFromEquippedItem(pInventory, pItem));
            //}
            //else if (ITEMS_IsCharmUsable(pItem, pUnit))
            //{
            //    const uint8_t nPage = ITEMS_GetInvPage(pItem);
            //    INVENTORY_Return(__FILE__, __LINE__, pInventory, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), UNITS_GetInventoryRecordId(pUnit, nPage, pGame->bExpansion), 0, nPage);
            //}
        }

        if (!INVENTORY_RemoveItemFromInventory(pInventory, pItem))
        {
            FOG_DisplayAssert("hRemovedItem", __FILE__, __LINE__);
            exit(-1);
        }

        if (pItem->dwUnitType < 6)
        {
            SUNIT_RemoveUnit(pGame, pItem);
        }

        i = pNext;
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pInventory);
    if (pCursorItem && pCursorItem->dwUnitType < 6)
    {
        SUNIT_RemoveUnit(pGame, pCursorItem);
    }
}

//D2Game.0x6FCBD730
D2GameStrc* __fastcall SUNIT_GetGameFromUnit(D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit);

    return pUnit->pGame;
}

//D2Game.0x6FCBD760
void __fastcall D2GAME_SetNecropetFlag_6FCBD760(D2UnitStrc* pUnit, int32_t nFlag)
{
    //TODO: pUnit == PLAYER or MONSTER?
    D2_ASSERT(pUnit);

    pUnit->pPlayerData->unk0x3C[1] = nFlag;
}

//D2Game.0x6FCBD790
uint32_t __fastcall D2GAME_GetNecropetFlag_6FCBD790(D2UnitStrc* pUnit)
{
    //TODO: pUnit == PLAYER or MONSTER?
    D2_ASSERT(pUnit);

    return pUnit->pPlayerData->unk0x3C[1];
}

//D2Game.0x6FCBD7C0
void __fastcall D2GAME_SetSparkChest_6FCBD7C0(D2UnitStrc* pUnit, uint8_t nSparkChest)
{
    D2_ASSERT(pUnit);

    pUnit->bSparkChest = nSparkChest;
}

//D2Game.0x6FCBD7F0
uint8_t __fastcall D2GAME_CheckIfSparklyChest_6FCBD7F0(D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit);

    return pUnit->bSparkChest;
}

//D2Game.0x6FCBD820
int32_t __stdcall SUNIT_GetInteractInfo(D2UnitStrc* pUnit, int32_t* pInteractUnitType, int32_t* pInteractUnitGUID)
{
    *pInteractUnitGUID = pUnit->dwInteractGUID;
    *pInteractUnitType = pUnit->dwInteractType;
    return pUnit->nInteract;
}

//D2Game.0x6FCBD840
void __stdcall SUNIT_SetInteractInfo(D2UnitStrc* pUnit, int32_t nInteractUnitType, int32_t nInteractUnitGUID)
{
    if (pUnit->nInteract)
    {
        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
            if (pPlayerData)
            {
                char szMessage[80] = {};
                sprintf(szMessage, "Player %s attempted to operate while busy\n", pPlayerData->szName);
            }
        }
    }
    else
    {
        pUnit->dwInteractGUID = nInteractUnitGUID;
        pUnit->dwInteractType = nInteractUnitType;
        pUnit->nInteract = 1;
    }
}

//D2Game.0x6FCBD890
void __stdcall SUNIT_ResetInteractInfo(D2UnitStrc* pUnit)
{
    pUnit->dwInteractGUID = -1;
    pUnit->dwInteractType = 6;
    pUnit->nInteract = 0;
}

//D2Game.0x6FCBD8B0
int32_t __fastcall SUNIT_IsDead(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwFlags & 0x10000)
    {
        return 1;
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        if (pUnit->dwAnimMode == PLRMODE_DEATH || pUnit->dwAnimMode == PLRMODE_DEAD)
        {
            return 1;
        }
        return 0;
    }
    else if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        if (pUnit->dwAnimMode == MONMODE_DEATH || pUnit->dwAnimMode == MONMODE_DEAD)
        {
            return 1;
        }
        return 0;
    }

    return 1;
}

//D2Game.0x6FCBD900
int32_t __fastcall sub_6FCBD900(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget)
{
    D2UnitStrc* pTestUnit = pUnit;
    if (pTestUnit)
    {
        D2UnitStrc* pOwner = nullptr;
        if (pTestUnit->dwUnitType == UNIT_MONSTER)
        {
            pOwner = AIGENERAL_GetMinionOwner(pTestUnit);
        }
        else if (pTestUnit->dwUnitType == UNIT_MISSILE)
        {
            pOwner = SUNIT_GetOwner(pGame, pTestUnit);
        }

        if (pOwner)
        {
            pTestUnit = pOwner;
        }
    }

    D2UnitStrc* i = pTarget;
    while (i && i->dwUnitType == UNIT_PLAYER)
    {
        D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(i);
        if (!pOwner || pOwner == i)
        {
            break;
        }

        i = pOwner;
    }

    if (pTestUnit == i)
    {
        return 0;
    }

    if (pTestUnit && pTestUnit->dwUnitType == UNIT_PLAYER && i && i->dwUnitType == UNIT_PLAYER)
    {
        return PLAYERLIST_CheckFlag(pTestUnit, i, 8);
    }

    return STATLIST_AreUnitsAligned(pTestUnit, i) == 0;
}

//D2Game.0x6FCBDA10
D2UnitStrc** __fastcall SUNIT_GetUnitList(int32_t nUnitType, D2GameStrc* pGame, int32_t nUnitGUID)
{
    switch (nUnitType)
    {
    case UNIT_PLAYER:
        return &pGame->pUnitList[0][nUnitGUID & 127];

    case UNIT_MONSTER:
        return &pGame->pUnitList[1][nUnitGUID & 127];

    case UNIT_OBJECT:
        return &pGame->pUnitList[2][nUnitGUID & 127];

    case UNIT_MISSILE:
        return &pGame->pUnitList[4][nUnitGUID & 127];

    case UNIT_ITEM:
        return &pGame->pUnitList[3][nUnitGUID & 127];
    }

    return nullptr;
}

//D2Game.0x6FCBDA40
int32_t __fastcall SUNIT_CanPetBeTargetedBySkill(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pPet, int32_t nSkillId)
{
    if (!pOwner || !pPet || (pOwner->dwUnitType != UNIT_PLAYER && pPet->dwUnitType != UNIT_MONSTER))
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_TARGETPET]))
    {
        return 0;
    }

    int32_t nOwnerGUID = 0;
    int32_t nOwnerType = 0;
    AIGENERAL_GetOwnerData(pPet, &nOwnerGUID, &nOwnerType);
    if (nOwnerType != UNIT_PLAYER || nOwnerGUID != pOwner->dwUnitId)
    {
        return 0;
    }

    return 1;
}

//D2Game.0x6FCBDAD0
int32_t __fastcall SUNIT_CanAllyBeTargetedBySkill(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pPet, int32_t nSkillId)
{
    if (!pOwner || !pPet || pOwner->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }
    
    int32_t nUnitType = pPet->dwUnitType;   
    if (nUnitType != UNIT_PLAYER && nUnitType != UNIT_MONSTER)
    {
        return 0;
    }
    
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || !(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_TARGETALLY]))
    {
        return 0;
    }
    
    int32_t nUnitGUID = pPet->dwUnitId;
    if (nUnitType == UNIT_MONSTER)
    {
        AIGENERAL_GetOwnerData(pPet, &nUnitGUID, &nUnitType);
        if (nUnitGUID == -1 || nUnitType != UNIT_PLAYER)
        {
            return 0;
        }
    }
    
    if (!pOwner->pGame)
    {
        return 0;
    }
    
    const int16_t nPartyId = PARTY_GetPartyIdForUnitOwner(pOwner->pGame, pOwner);
    if (nPartyId == -1)
    {
        return 0;
    }

    D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
    if (!pOtherPlayer->pGame || pOtherPlayer->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }
    
    return nPartyId == PARTY_GetPartyIdForUnitOwner(pOtherPlayer->pGame, pOtherPlayer);
}

//D2Game.0x6FCBDC60
int32_t __fastcall SUNIT_AreUnitsAligned(D2GameStrc* pGame, D2UnitStrc* pUnit1, D2UnitStrc* pUnit2)
{
    D2_MAYBE_UNUSED(pGame);
    D2UnitStrc* pOwner1 = pUnit1;
    if (pUnit1 && pUnit1->dwUnitType == UNIT_MONSTER)
    {
        D2UnitStrc* pTemp = AIGENERAL_GetMinionOwner(pUnit1);
        if (pTemp)
        {
            pOwner1 = pTemp;
        }
    }

    D2UnitStrc* pOwner2 = pUnit2;
    if (pUnit2 && pUnit2->dwUnitType == UNIT_MONSTER)
    {
        D2UnitStrc* pTemp = AIGENERAL_GetMinionOwner(pUnit2);
        if (pTemp)
        {
            pOwner2 = pTemp;
        }
    }

    if (pOwner1 == pOwner2)
    {
        return 1;
    }

    if (!pOwner1 || pOwner1->dwUnitType != UNIT_PLAYER || !pOwner2 || pOwner2->dwUnitType != UNIT_PLAYER)
    {
        return STATLIST_AreUnitsAligned(pOwner1, pOwner2);
    }

    int16_t nPartyId1 = -1;
    if (pOwner1->pGame)
    {
        nPartyId1 = PARTY_GetPartyIdForUnitOwner(pOwner1->pGame, pOwner1);
    }

    int16_t nPartyId2 = -1;
    if (pOwner2->pGame && pOwner2->dwUnitType == UNIT_PLAYER)
    {
        nPartyId2 = PARTY_GetPartyIdForUnitOwner(pOwner2->pGame, pOwner2);
    }

    return nPartyId1 == nPartyId2 && nPartyId2 != -1;
}

//D2Game.0x6FCBDD30
void __fastcall sub_6FCBDD30(D2UnitStrc* pUnit, uint8_t nAlignNew, int32_t a3)
{
    int32_t nAlignment = 4;

    if (!pUnit)
    {
        return;
    }

    if (nAlignNew >= 3)
    {
        FOG_DisplayAssert("eAlignNew < NUM_ALIGNMENT", __FILE__, __LINE__);
        exit(-1);
    }

    const int32_t nClassId = pUnit->dwClassId;
    if (nClassId != MONSTER_HYDRA1 && nClassId != MONSTER_HYDRA2 && nClassId != MONSTER_HYDRA3 && !nAlignNew && pUnit->dwFlags >> 31)
    {
        return;
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_ALIGNMENT);
    if (pStatList)
    {
        nAlignment = STATLIST_GetStatValue(pStatList, STAT_ALIGNMENT, 0);
    }
    else
    {
        pStatList = STATLIST_AllocStatList(pUnit->pMemoryPool, 0, 0, pUnit->dwUnitType, pUnit->dwUnitId);
        if (pStatList)
        {
            STATLIST_SetState(pStatList, STATE_ALIGNMENT);
            D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
            STATES_ToggleState(pUnit, STATE_ALIGNMENT, 1);
        }
        else
        {
            FOG_DisplayWarning("hList", __FILE__, __LINE__);
        }
    }

    STATLIST_SetStatIfListIsValid(pStatList, STAT_ALIGNMENT, nAlignNew, 0);
    STATES_ToggleGfxStateFlag(pUnit, STATE_ALIGNMENT, 1);

    if (pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pGame)
    {
        return;
    }

    if (a3)
    {
        sub_6FC681C0(pUnit->pGame->pMonReg, pUnit, nAlignment, nAlignNew);
    }

    if (nAlignment == nAlignNew)
    {
        return;
    }

    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return;
    }

    if (nAlignNew == 2)
    {
        DUNGEON_IncreaseAlliedCountOfRoom(pRoom);
    }
    else if (nAlignment == 2)
    {
        DUNGEON_DecreaseAlliedCountOfRoom(pRoom);
    }

    if (!MONSTERS_IsDead(pUnit))
    {
        D2Common_10214(pUnit);
    }
}

//D2Game.0x6FCBDE90
void __fastcall sub_6FCBDE90(D2UnitStrc* pUnit, int32_t bSetUninterruptable)
{
    STATES_ToggleState(pUnit, STATE_UNINTERRUPTABLE, bSetUninterruptable);

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    if (!pGame)
    {
        return;
    }

    if (!bSetUninterruptable && STATES_CheckState(pUnit, STATE_DEATH_DELAY))
    {
        STATES_ToggleState(pUnit, STATE_DEATH_DELAY, 0);

        D2UnitStrc* pAttacker = SUNIT_GetLastAttacker(pGame, pUnit);
        if (pAttacker)
        {
            D2DamageStrc damage = {};
            damage.wResultFlags |= DAMAGERESULTFLAG_WILLDIE;
            SUNITDMG_ExecuteMissileDamage(pGame, pAttacker, pUnit, &damage);
        }
        else
        {
            SUNITDMG_KillMonster(pGame, pUnit, nullptr, 1);
        }
    }

    if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, 0);
    }
}

//D2Game.0x6FCBDF90
int32_t __fastcall sub_6FCBDF90(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t a3, int32_t a4, int32_t a5)
{
    if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER)
    {
        return 0;
    }

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (!pPlayerData)
    {
        return 0;
    }

    pPlayerData->unk0xA8[40] = 0;
    pPlayerData->unk0xA8[41] = 0;
    sub_6FC349B0(pGame, pPlayer, UNITEVENTCALLBACK_REMOVESKILLCOOLDOWN, sub_6FCBDF90);
    return 1;
}

//D2Game.0x6FCBDFE0
int32_t __fastcall sub_6FCBDFE0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2RoomStrc* pInputRoom, int32_t nX, int32_t nY, int32_t a6, int32_t a7)
{
    D2_ASSERT(pUnit);
    D2_ASSERT(pUnit->pDynamicPath);

    D2RoomStrc* pRoom = pInputRoom;
    if (!pRoom)
    {
        pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
        if (!pRoom)
        {
            return 0;
        }
    }

    D2CoordStrc coords = {};
    coords.nX = nX;
    coords.nY = nY;

    if (!a6)
    {
        pRoom = COLLISION_GetFreeCoordinates(pRoom, &coords, UNITS_GetUnitSizeX(pUnit), 0x1C09u, 0);
    }

    if (!pRoom || !D2Common_10229(pUnit->pDynamicPath, pUnit, pRoom, coords.nX, coords.nY))
    {
        return 0;
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        D2ClientStrc* pClient = UNITS_GetPlayerData(pUnit)->pClient;

        D2DrlgCoordsStrc drlgCoords = {};

        DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);
        D2GAME_PACKETS_SendPacket0x07_6FC3D120(pClient, DUNGEON_GetLevelIdFromRoom(pRoom), (uint16_t)drlgCoords.dwTilesLeft, (uint16_t)drlgCoords.dwTilesTop);

        if (a7)
        {
            UNITROOM_RefreshUnit(pUnit);
            pUnit->dwFlagEx |= 0x800;
        }
        else
        {
            UNITROOM_RefreshUnit(pUnit);
            pUnit->dwFlagEx |= UNITFLAGEX_TELEPORTED;
        }

        sub_6FCBB190(pGame, pUnit, 0);

        D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);

        pPlayerData->unk0xA8[40] = coords.nX;
        pPlayerData->unk0xA8[41] = coords.nY;
        pPlayerData->unk0xA4 = GetTickCount();

        pPlayerData->unk0xA8[2 * pPlayerData->unk0xA0] = coords.nX;
        pPlayerData->unk0xA8[2 * pPlayerData->unk0xA0 + 1] = coords.nY;

        ++pPlayerData->unk0xA0;
        if (pPlayerData->unk0xA0 >= 20u)
        {
            pPlayerData->unk0xA0 = 0;
        }

        j_D2GAME_InitTimer_6FC351D0(pGame, pUnit, UNITEVENTCALLBACK_REMOVESKILLCOOLDOWN, pGame->dwGameFrame + 50, sub_6FCBDF90, 0, 0);
        sub_6FC7E310(pGame, pUnit, coords.nX, coords.nY);
    }
    else
    {
        if (a7)
        {
            UNITROOM_RefreshUnit(pUnit);
            pUnit->dwFlagEx |= 0x800;
        }
        else
        {
            UNITROOM_RefreshUnit(pUnit);
            pUnit->dwFlagEx |= UNITFLAGEX_TELEPORTED;
        }

        sub_6FCBB190(pGame, pUnit, 0);
    }

    return 1;
}

//D2Game.0x6FCBE2D0
int16_t __fastcall SUNIT_GetPartyId(D2UnitStrc* pPlayer)
{
    if (pPlayer && pPlayer->pGame && pPlayer->dwUnitType == UNIT_PLAYER)
    {
        return PARTY_GetPartyIdForUnitOwner(pPlayer->pGame, pPlayer);
    }

    return -1;
}
