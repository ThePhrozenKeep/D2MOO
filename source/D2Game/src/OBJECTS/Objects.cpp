#include "OBJECTS/Objects.h"

#include <algorithm>

#include <Fog.h>

#include <D2Collision.h>
#include <DataTbls/ObjectsTbls.h>
#include <D2Dungeon.h>
#include <Drlg/D2DrlgDrlg.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/LevelsTbls.h>
#include <DataTbls/MonsterIds.h>
#include <D2Chat.h>
#include <D2DataTbls.h>


#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterSpawn.h"
#include "OBJECTS/ObjRgn.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT1/A1Q4.h"
#include "QUESTS/ACT1/A1Q5.h"
#include "QUESTS/ACT2/A2Q4.h"
#include "QUESTS/ACT2/A2Q6.h"
#include "QUESTS/ACT2/A2Q7.h"
#include "QUESTS/ACT3/A3Q0.h"
#include "QUESTS/ACT3/A3Q2.h"
#include "QUESTS/ACT3/A3Q3.h"
#include "QUESTS/ACT3/A3Q5.h"
#include "QUESTS/ACT3/A3Q6.h"
#include "QUESTS/ACT3/A3Q7.h"
#include "QUESTS/ACT4/A4Q2.h"
#include "QUESTS/ACT4/A4Q3.h"
#include "QUESTS/ACT5/A5Q1.h"
#include "QUESTS/ACT5/A5Q2.h"
#include "QUESTS/ACT5/A5Q3.h"
#include "QUESTS/ACT5/A5Q5.h"
#include "QUESTS/ACT5/A5Q6.h"
#include "UNIT/SUnit.h"


#pragma pack(push, 1)
struct D2ObjectTableStrc
{
    int32_t nObjectId;
    int32_t nMin;
    int32_t nMax;
};

struct D2ObjectPopulateStrc
{
    int32_t bActive;
    int32_t(__fastcall* pfPopulate)(D2GameStrc*, D2ActiveRoomStrc*, D2DrlgCoordsStrc*);
};

struct D2ObjectCoordInitStrc
{
    const D2CoordStrc* pCoords;
    int32_t nCount;
};
#pragma pack(pop)


//D2Game.0x6FC70180
D2UnitStrc* __fastcall OBJECTS_SpawnShrine(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode)
{
    constexpr D2ObjectTableStrc stru_6FD28C78[8] =
    {
        { OBJECT_DESERT_SHRINE1, 2, 6 },
        { OBJECT_DESERT_SHRINE1, 7, 7 },
        { OBJECT_DESERT_SHRINE1, 8, 11 },
        { OBJECT_DESERT_SHRINE1, 12, 12 },
        { OBJECT_DESERT_SHRINE1, 1, 5 },
        { OBJECT_DESERT_SHRINE1, 14, 14 },
    };

    if (nClassId < 574 || nClassId > 579)
    {
        return nullptr;
    }

    D2_ASSERT(pRoom);

    const D2ObjectTableStrc* pObjectTableRecord = &stru_6FD28C78[nClassId - 574];
    D2UnitStrc* pShrine = SUNIT_AllocUnitData(UNIT_OBJECT, pObjectTableRecord->nObjectId, nX, nY, pGame, pRoom, 1, nMode, 0);

    int32_t nShrineId = ITEMS_RollLimitedRandomNumber(&pShrine->pSeed, pObjectTableRecord->nMax - pObjectTableRecord->nMin) + pObjectTableRecord->nMin;
    if (nShrineId == 5 || nShrineId == 4)
    {
        nShrineId = 2;
    }

    pShrine->pObjectData->InteractType = nShrineId;
    UNITS_SetShrineTxtRecordInObjectData(pShrine, DATATBLS_GetShrinesTxtRecord(nShrineId));
    return pShrine;
}

//D2Game.0x6FC70270
D2UnitStrc* __fastcall OBJECTS_SpawnPresetChest(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode)
{
    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);

    int32_t nObjectId = 0;
    switch (DRLG_GetActNoFromLevelId(nLevelId))
    {
    case ACT_II:
    {
        if (nLevelId == LEVEL_ARCANESANCTUARY)
        {
            constexpr int32_t nObjectIds[] = {
                OBJECT_ARCANE_SANCTUARY_BIG_CHEST_LEFT, OBJECT_ARCANE_SANCTUARY_BIG_CHEST_RIGHT,
                OBJECT_ARCANE_SANCTUARY_SMALL_CHEST_LEFT, OBJECT_ARCANE_SANCTUARY_SMALL_CHEST_RIGHT
            };
            nObjectId = nObjectIds[ITEMS_RollRandomNumber(&pObjectControl->pSeed) & (std::size(nObjectIds) - 1)];
        }
        else
        {
            constexpr int32_t nObjectIds[] = { OBJECT_TOMB_CHEST_LEFT, OBJECT_TOMB_CHEST_RIGHT };
            nObjectId = nObjectIds[ITEMS_RollRandomNumber(&pObjectControl->pSeed) & (std::size(nObjectIds) - 1)];
        }
        break;
    }
    case 2:
    {
        if (nLevelId == LEVEL_TRAVINCAL)
        {
            constexpr int32_t nObjectIds[] = {
                OBJECT_MEPHISTO_CHEST_LARGE_LEFT, OBJECT_MEPHISTO_CHEST_LARGE_RIGHT,
                OBJECT_MEPHISTO_CHEST_MEDIUM_LEFT, OBJECT_MEPHISTO_CHEST_MEDIUM_RIGHT
            };
            nObjectId = nObjectIds[ITEMS_RollRandomNumber(&pObjectControl->pSeed) & (std::size(nObjectIds) - 1)];
        }
        else
        {
            constexpr int32_t nObjectIds[] = { OBJECT_JUNGLE_CHEST_ACT3, OBJECT_CHEST_LEFT_MEDIUM_JUNGLE };
            nObjectId = nObjectIds[ITEMS_RollRandomNumber(&pObjectControl->pSeed) & (std::size(nObjectIds) - 1)];
        }
        break;
    }
    default:
    {
        constexpr int32_t nObjectIds[] = {
            OBJECT_LARGE_CHEST_RIGHT, OBJECT_LARGE_CHEST_LEFT,
            OBJECT_CHEST_RIGHT_LARGE_ACT1, OBJECT_CHEST_RIGHT_TALLSKINNY_ACT1,
            OBJECT_CHEST_RIGHT_MEDIUM_ACT1, OBJECT_LEFT_CHEST_ACT1,
            OBJECT_CHEST_LEFT_MEDIUM, OBJECT_CHEST_LEFT_LARGE,
            OBJECT_CHEST_LEFT_TALLSKINNY,
            OBJECT_GENERAL_CHEST_LEFT1, OBJECT_GENERAL_CHEST_RIGHT2,
            OBJECT_GENERAL_CHEST_RIGHT3, OBJECT_GENERAL_CHEST_LEFT3
        };
        nObjectId = nObjectIds[ITEMS_RollRandomNumber(&pObjectControl->pSeed) % std::size(nObjectIds)];
        break;
    }
    }

    if (nClassId == OBJECT_CHEST)
    {
        nObjectId = OBJECT_CHEST;
    }

    D2_ASSERT(pRoom);

    return SUNIT_AllocUnitData(UNIT_OBJECT, nObjectId, nX, nY, pGame, pRoom, 1, 0, 0);
}

//D2Game.0x6FC70470
D2UnitStrc* __fastcall OBJECTS_SpawnSpecialChest(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode)
{
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);

    int32_t nObjectId = OBJECT_CHEST;
    if (nLevelId != LEVEL_TOWERCELLARLEV5)
    {
        nObjectId = nClassId;
    }

    D2UnitStrc* pObject = OBJECTS_SpawnPresetChest(pGame, pRoom, nObjectId, nX, nY, nMode);
    if (!pObject)
    {
        return nullptr;
    }

    if (nLevelId < LEVEL_MAGGOTLAIRLEV1 || nLevelId > LEVEL_MAGGOTLAIRLEV3 || (ITEMS_RollRandomNumber(&OBJRGN_GetObjectControlFromGame(pGame)->pSeed) % 100) <= 50)
    {
        D2GAME_SetSparkChest_6FCBD7C0(pObject, 1);

        if (nLevelId == LEVEL_TOWERCELLARLEV5)
        {
            pObject->pObjectData->InteractType = 3;
        }
    }

    pObject->dwFlags |= UNITFLAG_OBJPREOPERATE;
    UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);

    return pObject;
}

//D2Game.0x6FC70520
D2UnitStrc* __fastcall OBJECTS_SpawnArcaneSymbol(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode)
{
    const int32_t nObjectId = ACT2Q6_GetObjectIdForArcaneThing(pGame);

    D2_ASSERT(pRoom);

    return SUNIT_AllocUnitData(UNIT_OBJECT, nObjectId, nX, nY, pGame, pRoom, 1, 0, 0);
}

//D2Game.0x6FC70570
D2UnitStrc* __fastcall OBJECTS_SpawnPresetObject(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode)
{
    using PresetFunc = D2UnitStrc * (__fastcall*)(D2GameStrc*, D2ActiveRoomStrc*, int32_t, int32_t, int32_t, int32_t);
    constexpr PresetFunc sgPresetStuff_6FD2F6E4[] =
    {
        OBJECTS_SpawnShrine,
        OBJECTS_SpawnShrine,
        OBJECTS_SpawnShrine,
        OBJECTS_SpawnShrine,
        OBJECTS_SpawnShrine,
        OBJECTS_SpawnShrine,
        OBJECTS_SpawnSpecialChest,
        OBJECTS_SpawnPresetChest,
        OBJECTS_SpawnArcaneSymbol
    };

    if (nClassId <= 573)
    {
        return 0;
    }

    const int32_t nIndex = nClassId - 574;
    if (nIndex >= 9)
    {
        return 0;
    }

    if (IsBadCodePtr((FARPROC)sgPresetStuff_6FD2F6E4[nIndex]))
    {
        FOG_DisplayAssert("sgPresetStuff[idx]", __FILE__, __LINE__);
        exit(-1);
    }

    return sgPresetStuff_6FD2F6E4[nIndex](pGame, pRoom, nClassId, nX, nY, nMode);
}

//D2Game.0x6FC70600
void __fastcall OBJECTS_InitHandler(D2GameStrc* pGame, D2UnitStrc* pObject, int32_t nUnitId, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY)
{
    constexpr ObjInitFunction gpObjectInitFnTable[] =
    {
        nullptr,
        OBJECTS_InitFunction01_Shrine,
        OBJECTS_InitFunction02_Urn,
        OBJECTS_InitFunction03_Chest,
        OBJECTS_InitFunction04_TowerTome,
        OBJECTS_InitFunction05_Door,
        OBJECTS_InitFunction06_CairnStone,
        OBJECTS_InitFunction07_CainGibbet,
        OBJECTS_InitFunction08_Torch,
        OBJECTS_InitFunction09_InifussTree,
        OBJECTS_InitFunction10_Unused,
        OBJECTS_InitFunction11_Portal,
        OBJECTS_InitFunction12_PermanentPortal,
        OBJECTS_InitFunction13_InvisibleObject,
        OBJECTS_InitFunction14_Brazier,
        OBJECTS_InitFunction15_MalusStand,
        OBJECTS_InitFunction16_Well,
        OBJECTS_InitFunction17_Waypoint,
        OBJECTS_InitFunction18_JerhynPosition,
        OBJECTS_InitFunction19_JerhynPositionEx,
        OBJECTS_InitFunction20_TaintedAltar,
        OBJECTS_InitFunction21_HoradricOrifice,
        OBJECTS_InitFunction22_Fire,
        OBJECTS_InitFunction23_LamEsenTomeStand,
        OBJECTS_InitFunction24_SpikeFloorTrap,
        OBJECTS_InitFunction25_Gidbinn,
        OBJECTS_InitFunction14_Brazier,
        OBJECTS_InitFunction27_GooPile,
        OBJECTS_InitFunction28_GoldPlaceHolder,
        OBJECTS_InitFunction29_ArcaneSanctuaryPortal,
        OBJECTS_InitFunction30_HaremBlocker,
        nullptr,
        nullptr,
        nullptr,
        OBJECTS_InitFunction34_HellBrazierFire,
        nullptr,
        nullptr,
        OBJECTS_InitFunction37_Unused,
        OBJECTS_InitFunction38_TyraelsDoor,
        OBJECTS_InitFunction39_GidbinnAltar,
        nullptr,
        OBJECTS_InitFunction41_SewerStairs,
        OBJECTS_InitFunction42_SewerLever,
        OBJECTS_InitFunction43_DarkWanderer,
        OBJECTS_InitFunction44_HellGatePortal,
        OBJECTS_InitFunction45_MephistoBridge,
        OBJECTS_InitFunction46_TrappedSoulPlaceHolder,
        OBJECTS_InitFunction47_CountessChest,
        OBJECTS_InitFunction48_HellForge,
        OBJECTS_InitFunction49_HratliStart,
        OBJECTS_InitFunction50_HratliEnd,
        OBJECTS_InitFunction51_TrappedSoul,
        OBJECTS_InitFunction52_NatalyaStart,
        OBJECTS_InitFunction53_StairsR,
        OBJECTS_InitFunction54_CainStartPosition,
        OBJECTS_InitFunction55_DiabloStartPoint,
        nullptr,
        OBJECTS_InitFunction57_SparklyChest,
        OBJECTS_InitFunction58_Fissure,
        OBJECTS_InitFunction59_VileDogAfterglow,
        OBJECTS_InitFunction60_CompellingOrb,
        OBJECTS_InitFunction61_CainPortal,
        OBJECTS_InitFunction62_CagedWussie,
        OBJECTS_InitFunction63_AncientStatue3,
        OBJECTS_InitFunction64_AncientStatue1,
        OBJECTS_InitFunction65_AncientStatue2,
        OBJECTS_InitFunction66_DrehyaStartInTown,
        OBJECTS_InitFunction67_DrehyaStartOutsideTown,
        OBJECTS_InitFunction68_NihlathakStartInTown,
        OBJECTS_InitFunction69_NihlathakStartOutsideTown,
        nullptr,
        OBJECTS_InitFunction71_LarzukStandard,
        OBJECTS_InitFunction72_AncientsAltar,
        OBJECTS_InitFunction73_AncientsDoor,
        OBJECTS_InitFunction74_FrozenAnya,
        OBJECTS_InitFunction75_BaalsPortal,
        OBJECTS_InitFunction76_SummitDoor,
        OBJECTS_InitFunction77_LastPortal,
        OBJECTS_InitFunction78_LastLastPortal,
        OBJECTS_InitFunction79_Zoo,
    };


    if (!pGame || !pObject)
    {
        return;
    }

    D2ObjectsTxt* pObjectsTxtRecord = UNITS_GetObjectTxtRecordFromObject(pObject);

    pObject->dwUnitId = nUnitId;
    pObject->bSparkChest = 0;
    pObject->pHoverText = nullptr;
    pObject->dwFlags &= ~UNITFLAG_ISVALIDTARGET;

    if (pObject->dwAnimMode >= 8)
    {
        FOG_DisplayAssert("nMode < NUM_OBJECTMODES", __FILE__, __LINE__);
        exit(-1);
    }

    if (pObjectsTxtRecord->nIsAttackable0)
    {
        pObject->dwFlags |= UNITFLAG_CANBEATTACKED;
    }
    else
    {
        pObject->dwFlags &= ~UNITFLAG_CANBEATTACKED;
    }

    if (pObject->pObjectData)
    {
        pObject->pObjectData->InteractType = 0;
    }

    D2GAME_DeleteTimersOnUnit_6FC34A30(pGame, pObject);

    if (pObject->dwClassId >= 573)
    {
        FOG_DisplayAssert("wClass < NUM_OBJECTS", __FILE__, __LINE__);
        exit(-1);
    }

    if (pObjectsTxtRecord->nInitFn >= std::size(gpObjectInitFnTable))
    {
        FOG_DisplayAssert("ptTypeData->bInitFunction < sizeof(sgInitTable)/sizeof(PTOBJECTINIT)", __FILE__, __LINE__);
        exit(-1);
    }

    if (pObjectsTxtRecord->nInitFn < std::size(gpObjectInitFnTable))
    {
        ObjInitFunction pInitFn = gpObjectInitFnTable[pObjectsTxtRecord->nInitFn];
        if (pInitFn)
        {
            if (IsBadCodePtr((FARPROC)pInitFn))
            {
                FOG_DisplayAssert("sgInitTable[ptTypeData->bInitFunction]", __FILE__, __LINE__);
                exit(-1);
            }

            D2ObjInitFnStrc pObjInit = {};
            pObjInit.pGame = pGame;
            pObjInit.pObject = pObject;
            pObjInit.pRoom = pRoom;
            pObjInit.pObjectregion = pGame->pObjectControl;
            pObjInit.pObjectTxt = pObjectsTxtRecord;
            pObjInit.nX = nX;
            pObjInit.nY = nY;

            pInitFn(&pObjInit);
        }
    }

    if (pObjectsTxtRecord->nSelectable[pObject->dwAnimMode])
    {
        pObject->dwFlags |= UNITFLAG_TARGETABLE;
    }
    else
    {
        pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    if (pObjectsTxtRecord->nPreOperate && !(pObject->dwFlags & UNITFLAG_OBJPREOPERATE))
    {
        if (!(ITEMS_RollRandomNumber(&pGame->pObjectControl->pSeed) % 14))
        {
            UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
        }
    }

    D2TimerArgStrc* pTimerArg = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2TimerArgStrc);
    pTimerArg->unk0x00 = nullptr;
    pTimerArg->dwUnitId = -1;

    D2TimerArg2Strc* pArg = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2TimerArg2Strc);
    pArg->nUnitGUID = -1;
    pArg->pNext = nullptr;
    pTimerArg->unk0x00 = pArg;

    UNITS_SetTimerArg(pObject, pTimerArg);
}

//D2Game.0x6FC70850
void __fastcall OBJECTS_InitFunction10_Unused(D2ObjInitFnStrc* pOp)
{
    if (DUNGEON_GetLevelIdFromRoom(pOp->pRoom) == LEVEL_ROGUEENCAMPMENT)
    {
        EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_PERIODICSKILLS, pOp->pGame->dwGameFrame + 60, 0, 0);
    }
    else
    {
        UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
        EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_MODECHANGE, pOp->pGame->dwGameFrame + 25, 0, 0);
    }
}

//D2Game.0x6FC708B0
void __fastcall OBJECTS_InitFunction28_GoldPlaceHolder(D2ObjInitFnStrc* pOp)
{
    if (pOp->pObject && pOp->pObject->dwAnimMode != OBJMODE_NEUTRAL)
    {
        return;
    }

    UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
    D2CoordStrc pCoord = {};
    pCoord.nX = pOp->nX;
    pCoord.nY = pOp->nY;

    const int32_t nCounter = ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 9 + 1;

    for (int32_t i = 0; i < nCounter; ++i)
    {
        const int32_t nX = pOp->nX + ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) & 3;
        const int32_t nY = pOp->nY + ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) & 3;

        D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomAtPosition(pOp->pRoom, nX, nY);

        if (pRoom == pOp->pRoom)
        {
            pCoord.nX = nX;
            pCoord.nY = nY;
            if (!COLLISION_CheckMaskWithSizeXY(pRoom, pCoord.nX, pCoord.nY, 1, 1, COLLIDE_BLOCK_PLAYER | COLLIDE_ALTERNATE_FLOOR | COLLIDE_MONSTER | COLLIDE_ITEM | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_UNIT_RELATED | COLLIDE_PET))
            {
                sub_6FC4F640(pOp->pGame, pOp->pRoom, &pCoord);
            }
        }
    }
}

//D2Game.0x6FC70A50
void __fastcall OBJECTS_InitFunction01_Shrine(D2ObjInitFnStrc* pOp)
{
    int32_t nShrineId = 0;

    if (pOp->pObjectTxt->dwParm[0])
    {
        BYTE nClass = 0;
        switch (pOp->pObjectTxt->dwParm[0] - 1)
        {
        case 0:
            nClass = 2;
            break;

        case 1:
            nClass = 3;
            break;

        default:
            nClass = 4;

            if (!(ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 10))
            {
                nClass = 1;
            }
            break;
        }

        nShrineId = OBJECTS_GetShrineId(pOp->pGame, nClass, DUNGEON_GetLevelIdFromRoom(pOp->pRoom), 0);
    }
    else
    {
        for (int32_t i = 0; i < 8; ++i)
        {
            nShrineId = ITEMS_RollLimitedRandomNumber(&pOp->pObjectregion->pSeed, DATATBLS_GetShrinesTxtRecordCount() - 1) + 1;

            if (DUNGEON_GetLevelIdFromRoom(pOp->pRoom) >= (int32_t)DATATBLS_GetShrinesTxtRecord(nShrineId)->dwLevelMin)
            {
                break;
            }
        }
    }

    switch (nShrineId)
    {
    case 4:
        nShrineId = 2;
        break;
    case 5:
        nShrineId = 3;
        break;
    case 16:
        nShrineId = 18;
        break;
    }

    pOp->pObject->pObjectData->InteractType = nShrineId;
    pOp->pObject->pObjectData->pShrineTxt = DATATBLS_GetShrinesTxtRecord(nShrineId);
}

//D2Game.0x6FC70BA0
int32_t __fastcall OBJECTS_GetShrineId(D2GameStrc* pGame, uint8_t nShrineClass, int32_t nLevelId, D2ActiveRoomStrc* pRoom)
{
    uint8_t nClass = nShrineClass;
    if (!nShrineClass || nShrineClass > 4u)
    {
        nClass = 2;
    }

    D2ShrineDataStrc* pShrineData = OBJRGN_GetShrineDataFromGame(pGame);
    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);

    D2_ASSERT(pShrineData->nShrineSubTypes[nClass]);

    int32_t nShrineId = 0;
    for (int32_t i = 0; i < std::size(pShrineData->nShrineSubTypes); ++i)
    {
        const int32_t nShrineSubTypes = pShrineData->nShrineSubTypes[nClass];
        nShrineId = pShrineData->pShrineSubTypeIds[nClass][ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, nShrineSubTypes)];
        if (!nShrineId)
        {
            nShrineId = 1;
        }

        if (nLevelId >= DATATBLS_GetShrinesTxtRecord(nShrineId)->dwLevelMin)
        {
            break;
        }
    }

    return nShrineId;
}

//D2Game.0x6FC70CA0
void __fastcall OBJECTS_InitFunction22_Fire(D2ObjInitFnStrc* pOp)
{
    if (pOp->pObjectTxt->nMode[2])
    {
        if (!pOp->pObject || !pOp->pObject->dwAnimMode)
        {
            if (!pOp->pObjectTxt->nMode[0])
            {
                UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
            }
        }
    }

    EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_MODECHANGE, pOp->pGame->dwGameFrame + 25, 0, 0);
}

//D2Game.0x6FC70CF0
void __fastcall OBJECTS_InitFunction16_Well(D2ObjInitFnStrc* pOp)
{
    pOp->pObject->pObjectData->InteractType = 2 * LOBYTE(pOp->pObjectTxt->dwParm[2]);
}

//D2Game.0x6FC70D10
void __fastcall OBJECTS_InitFunction24_SpikeFloorTrap(D2ObjInitFnStrc* pOp)
{
    EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_STATREGEN, pOp->pGame->dwGameFrame + 25, 0, 0);
}

//D2Game.0x6FC70D30
void __fastcall OBJECTS_InitFunction02_Urn(D2ObjInitFnStrc* pOp)
{
    pOp->pObject->pObjectData->InteractType = 0;
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pOp->pRoom);
    D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(nLevelId);

    if ((ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 100) < (WORD)(pLevelsTxtRecord->wMonLvl[0] / 8 + 5))
    {
        pOp->pObject->pObjectData->InteractType = (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) & 7) + 1;
    }
}

//D2Game.0x6FC70DC0
void __fastcall OBJECTS_InitFunction27_GooPile(D2ObjInitFnStrc* pOp)
{
    pOp->pObject->pObjectData->InteractType = 0;

    if ((ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 1000) < 333)
    {
        pOp->pObject->pObjectData->InteractType = 3;
    }
}

//D2Game.0x6FC70E10
void __fastcall OBJECTS_InitFunction03_Chest(D2ObjInitFnStrc* pOp)
{
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pOp->pRoom);
    D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(nLevelId);

    if ((ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 100) >= (WORD)(pLevelsTxtRecord->wMonLvl[0] / 8 + 5))
    {
        pOp->pObject->pObjectData->InteractType = 0;
    }
    else
    {
        pOp->pObject->pObjectData->InteractType = (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) & 7) + 1;
    }

    if (pOp->pObjectTxt->nLockable)
    {
        if ((ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 100) < (WORD)(pLevelsTxtRecord->wMonLvl[0] / 2 + 8))
        {
            pOp->pObject->pObjectData->InteractType |= 0x80;
        }
        else
        {
            pOp->pObject->pObjectData->InteractType &= 0x7F;
        }
    }
    else
    {
        pOp->pObject->pObjectData->InteractType &= 0x7F;
    }

    SEED_InitLowSeed(&pOp->pObject->pSeed, ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 65534 + 1);
}

//D2Game.0x6FC70F30
void __fastcall OBJECTS_InitFunction57_SparklyChest(D2ObjInitFnStrc* pOp)
{
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pOp->pRoom);
    D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(nLevelId);

    if ((ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 100) >= (WORD)(pLevelsTxtRecord->wMonLvl[0] / 8 + 5))
    {
        pOp->pObject->pObjectData->InteractType = 0;
    }
    else
    {
        pOp->pObject->pObjectData->InteractType = (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) & 7) + 1;
    }

    if (pOp->pObjectTxt->nLockable)
    {
        if ((ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 100) < (WORD)(pLevelsTxtRecord->wMonLvl[0] / 2 + 8))
        {
            pOp->pObject->pObjectData->InteractType |= 0x80;
        }
        else
        {
            pOp->pObject->pObjectData->InteractType &= 0x7F;
        }
    }
    else
    {
        pOp->pObject->pObjectData->InteractType &= 0x7F;
    }

    SEED_InitLowSeed(&pOp->pObject->pSeed, ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 65534 + 1);

    pOp->pObject->bSparkChest = 1;
}

//D2Game.0x6FC71060
void __fastcall OBJECTS_InitFunction58_Fissure(D2ObjInitFnStrc* pOp)
{
    EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_PERIODICSKILLS, pOp->pGame->dwGameFrame + ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 250 + 25, 0, 0);
}

//D2Game.0x6FC710B0
void __fastcall OBJECTS_InitFunction59_VileDogAfterglow(D2ObjInitFnStrc* pOp)
{
    if (pOp->pObject && pOp->pObject->dwAnimMode != OBJMODE_NEUTRAL)
    {
        return;
    }

    UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPERATING);
    EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_MONUMOD, pOp->pGame->dwGameFrame + 27, 0, 0);
    EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_ENDANIM, (pOp->pObjectTxt->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
}

//D2Game.0x6FC71110
void __fastcall OBJECTS_InitFunction12_PermanentPortal(D2ObjInitFnStrc* pOp)
{
    if (!pOp->pObject || pOp->pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPERATING);
        UNITS_BlockCollisionPath(pOp->pObject, pOp->pRoom, pOp->nX, pOp->nY);
        EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_ENDANIM, (pOp->pObjectTxt->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
    }

    switch (DUNGEON_GetLevelIdFromRoom(pOp->pRoom))
    {
    case LEVEL_ROGUEENCAMPMENT:
        pOp->pObject->pObjectData->InteractType = LEVEL_MOOMOOFARM;
        return;

    case LEVEL_MOOMOOFARM:
        pOp->pObject->pObjectData->InteractType = LEVEL_ROGUEENCAMPMENT;
        return;

    case LEVEL_TRISTRAM:
        pOp->pObject->pObjectData->InteractType = LEVEL_STONYFIELD;
        return;

    case LEVEL_STONYFIELD:
        pOp->pObject->pObjectData->InteractType = LEVEL_TRISTRAM;
        return;

    case LEVEL_ARCANESANCTUARY:
        pOp->pObject->pObjectData->InteractType = LEVEL_CANYONOFTHEMAGI;
        return;

    case LEVEL_CANYONOFTHEMAGI:
        pOp->pObject->pObjectData->InteractType = LEVEL_ARCANESANCTUARY;
        return;

    case LEVEL_DURIELSLAIR:
        pOp->pObject->pObjectData->InteractType = DUNGEON_GetHoradricStaffTombLevelId(pOp->pGame->pAct[ACT_II]);
        return;

    case LEVEL_NIHLATHAKSTEMPLE:
        pOp->pObject->pObjectData->InteractType = LEVEL_HARROGATH;
        return;

    case LEVEL_HARROGATH:
        pOp->pObject->pObjectData->InteractType = LEVEL_NIHLATHAKSTEMPLE;
        return;

    case LEVEL_HELL1:
        pOp->pObject->pObjectData->InteractType = LEVEL_ID_ACT5_BARRICADE_1;
        return;

    case LEVEL_HELL2:
        pOp->pObject->pObjectData->InteractType = LEVEL_ARREATPLATEAU;
        return;

    case LEVEL_HELL3:
        pOp->pObject->pObjectData->InteractType = LEVEL_TUNDRAWASTELANDS;
        return;

    case LEVEL_ID_ACT5_BARRICADE_1:
        if (pOp->pObject->pObjectData->InteractType != LEVEL_HELL1)
        {
            pOp->pObject->pObjectData->InteractType = LEVEL_HELL1;
            EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_DELAYEDPORTAL, pOp->pGame->dwGameFrame + 1, 0, 0);
        }
        return;

    case LEVEL_ARREATPLATEAU:
        if (pOp->pObject->pObjectData->InteractType != LEVEL_HELL2)
        {
            pOp->pObject->pObjectData->InteractType = LEVEL_HELL2;
            EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_DELAYEDPORTAL, pOp->pGame->dwGameFrame + 1, 0, 0);
        }
        return;

    case LEVEL_TUNDRAWASTELANDS:
        if (pOp->pObject->pObjectData->InteractType != LEVEL_HELL3)
        {
            pOp->pObject->pObjectData->InteractType = LEVEL_HELL3;
            EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_DELAYEDPORTAL, pOp->pGame->dwGameFrame + 1, 0, 0);
        }
        return;

    default:
        if (DUNGEON_GetLevelIdFromRoom(pOp->pRoom) == DUNGEON_GetHoradricStaffTombLevelId(pOp->pGame->pAct[ACT_II]))
        {
            pOp->pObject->pObjectData->InteractType = LEVEL_DURIELSLAIR;
        }
        return;
    }
}

//D2Game.0x6FC71360
void __fastcall OBJECTS_InitFunction08_Torch(D2ObjInitFnStrc* pOp)
{
    UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
}

//D2Game.0x6FC71370
void __fastcall OBJECTS_InitFunction14_Brazier(D2ObjInitFnStrc* pOp)
{
    UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPERATING);
}

//D2Game.0x6FC71380
void __fastcall OBJECTS_InitFunction34_HellBrazierFire(D2ObjInitFnStrc* pOp)
{
    if (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) & 1)
    {
        UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPERATING);
    }
}

//D2Game.0x6FC713D0
void __fastcall OBJECTS_InitFunction11_Portal(D2ObjInitFnStrc* pOp)
{
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pOp->pRoom);
    const int32_t nTownLevelId = DUNGEON_GetTownLevelIdFromActNo(DRLG_GetActNoFromLevelId(nLevelId));
    
    D2_ASSERT(nTownLevelId <= 255);

    pOp->pObject->pObjectData->InteractType = nTownLevelId;

    if (pOp->pObject && pOp->pObject->dwAnimMode == OBJMODE_OPERATING)
    {
        UNITS_BlockCollisionPath(pOp->pObject, pOp->pRoom, pOp->nX, pOp->nY);
        EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_ENDANIM, (pOp->pObjectTxt->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
    }
}

//D2Game.0x6FC71470
void __fastcall OBJECTS_InitFunction51_TrappedSoul(D2ObjInitFnStrc* pOp)
{
    if (!pOp->pObject)
    {
        return;
    }

    const int32_t nAnimMode = pOp->pObject->dwAnimMode;
    if (nAnimMode == OBJMODE_OPENED || nAnimMode == OBJMODE_OPERATING)
    {
        EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_PERIODICSTATS, pOp->pGame->dwGameFrame + 35, 0, 0);
    }
}

//D2Game.0x6FC714A0
void __fastcall OBJECTS_InitFunction46_TrappedSoulPlaceHolder(D2ObjInitFnStrc* pOp)
{
    constexpr int32_t nXOffsets[] =
    {
        -1, 0, 1, -1, 1, -1, 0, 1
    };

    constexpr int32_t nYOffsets[] =
    {
        -1, -1, -1, 0, 0, 1, 1, 1
    };

    int32_t bPositionFound = 0;

    if (pOp->pObject && pOp->pObject->dwAnimMode != OBJMODE_NEUTRAL)
    {
        return;
    }

    D2DrlgCoordsStrc pRoomCoord = {};
    DUNGEON_GetRoomCoordinates(pOp->pRoom, &pRoomCoord);

    D2ObjectControlStrc* pObjectControl = pOp->pGame->pObjectControl;

    int32_t ii = 30 * (pRoomCoord.nSubtileWidth * pRoomCoord.nSubtileHeight >> 7) >> 8;

    for (int32_t nLoops = 12; ii > 0 && nLoops > 0; --nLoops)
    {
        int32_t nX = ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, pRoomCoord.nSubtileWidth - 4) + pRoomCoord.nSubtileX;
        int32_t nY = ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, pRoomCoord.nSubtileHeight - 4) + pRoomCoord.nSubtileY;

        if (pRoomCoord.nSubtileWidth >= 5 && pRoomCoord.nSubtileHeight >= 5 && nX > pRoomCoord.nSubtileX + 1 && nY > pRoomCoord.nSubtileY + 1
            && nX < pRoomCoord.nSubtileWidth + pRoomCoord.nSubtileX - 5 && nY < pRoomCoord.nSubtileY + pRoomCoord.nSubtileHeight - 5
            && !COLLISION_CheckMaskWithSizeXY(pOp->pRoom, nX, nY, 10, 10, COLLIDE_BLOCK_PLAYER | COLLIDE_OBJECT | COLLIDE_DOOR)
            && !COLLISION_CheckMaskWithSizeXY(pOp->pRoom, nX, nY, 3, 3, COLLIDE_BLOCK_PLAYER | COLLIDE_ALTERNATE_FLOOR | COLLIDE_MONSTER | COLLIDE_ITEM | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_UNIT_RELATED | COLLIDE_PET))
        {
            bPositionFound = 1;

            if (D2GAME_SpawnMonster_6FC69F10(pOp->pGame, pOp->pRoom, pOp->nX, pOp->nY, (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) & 1) ? MONSTER_TRAPPEDSOUL2 : MONSTER_TRAPPEDSOUL1, 1, -1, 0))
            {
                UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
                --ii;
            }

            int32_t nBase = 1;

            while (1)
            {
                int32_t nMax = nBase >> 1;

                if (nMax > 0 && (nMax - 1) & nMax ? ITEMS_RollRandomNumber(&pObjectControl->pSeed) % nMax : (nMax - 1) & ITEMS_RollRandomNumber(&pObjectControl->pSeed))
                {
                    break;
                }

                if (!bPositionFound)
                {
                    break;
                }

                bPositionFound = 0;

                if (ii < 4)
                {
                    ii = 4;
                }

                int32_t nCounter = 0;
                while (nCounter < 3 * ii)
                {
                    nX += 2 * ((ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 5 + 5) * nXOffsets[(ITEMS_RollRandomNumber(&pObjectControl->pSeed) & 7)]);
                    nY += 2 * ((ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 5 + 5) * nYOffsets[(ITEMS_RollRandomNumber(&pObjectControl->pSeed) & 7)]);

                    ++nCounter;

                    if (pRoomCoord.nSubtileWidth >= 5 && pRoomCoord.nSubtileHeight >= 5 && nX > pRoomCoord.nSubtileX + 1 && nY > pRoomCoord.nSubtileY + 1
                        && nX < pRoomCoord.nSubtileX + pRoomCoord.nSubtileWidth - 5 && nY < pRoomCoord.nSubtileY + pRoomCoord.nSubtileHeight - 5
                        && !COLLISION_CheckMaskWithSizeXY(pOp->pRoom, nX, nY, 10, 10, COLLIDE_BLOCK_PLAYER | COLLIDE_OBJECT | COLLIDE_DOOR)
                        && !COLLISION_CheckMaskWithSizeXY(pOp->pRoom, nX, nY, 3, 3, COLLIDE_BLOCK_PLAYER | COLLIDE_ALTERNATE_FLOOR | COLLIDE_MONSTER | COLLIDE_ITEM | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_UNIT_RELATED | COLLIDE_PET))
                    {
                        bPositionFound = 1;
                        break;
                    }
                }

                if (bPositionFound)
                {
                    ++nBase;

                    if (D2GAME_SpawnMonster_6FC69F10(pOp->pGame, pOp->pRoom, nX, nY, (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) & 1) ? MONSTER_TRAPPEDSOUL2 : MONSTER_TRAPPEDSOUL1, 1, -1, 0))
                    {
                        UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
                        --ii;
                    }
                }
            }
        }
    }
}

//D2Game.0x6FC718C0
D2UnitStrc* __fastcall OBJECTS_PopulateFn1_CasketJarSarcophagusUrn(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability)
{
    // TODO: Improve

    constexpr int32_t dword_6FD2F718[] =
    {
        OBJECT_CASKET6, 28
    };

    constexpr int32_t dword_6FD2F70C[] =
    {
        OBJECT_CASKET3, 53, OBJECT_CASKET5
    };

    constexpr int32_t gnUrnIds[] =
    {
        OBJECT_URN1, OBJECT_URN2, OBJECT_URN3, OBJECT_URN4, OBJECT_URN5
    };

    constexpr int32_t dword_6FD2F734[] =
    {
        OBJECT_MUMMY_COFFIN_LEFT, 284
    };

    constexpr int32_t dword_6FD2F73C[] =
    {
        OBJECT_BASKET1, OBJECT_BASKET2
    };

    constexpr int signX[] =
    {
        -1, 0, 1, -1, 1, -1, 0, 1
    };

    constexpr int signY[] =
    {
        -1, -1, -1, 0, 0, 1, 1, 1
    };

    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nObjectId);
   
    D2DrlgCoordsStrc drlgCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);
    if (ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 100 > nProbability)
    {
        return 0;
    }

    int32_t v51 = 12;
    int32_t v68 = 0;
    int32_t nBaseStep = 1;
    int32_t nMaxObjects = 0;
    int32_t nMaxStep = 0;

    const int32_t* objectIds = nullptr;
    if (nObjectId == OBJECT_CASKET6)
    {
        objectIds = dword_6FD2F718;
        nMaxObjects = std::size(dword_6FD2F718);
        v51 = 18;
        nBaseStep = 5;
        nMaxStep = 5;
    }
    else if (nObjectId == OBJECT_CASKET3 || nObjectId == OBJECT_CASKET5)
    {
        objectIds = dword_6FD2F70C;
        nMaxObjects = std::size(dword_6FD2F70C);
        nBaseStep = 5;
        nMaxStep = 5;
    }
    else if (nObjectId == OBJECT_URN1)
    {
        objectIds = gnUrnIds;
        nMaxObjects = std::size(gnUrnIds);
        nMaxStep = 0;
        v68 = 1;
    }
    else if (nObjectId == OBJECT_MUMMY_COFFIN_LEFT)
    {
        objectIds = dword_6FD2F734;
        nMaxObjects = std::size(dword_6FD2F734);
        nBaseStep = 5;
        nMaxStep = 5;
    }
    else if (nObjectId == OBJECT_BASKET1 || nObjectId == OBJECT_BASKET2)
    {
        objectIds = dword_6FD2F73C;
        nMaxObjects = std::size(dword_6FD2F73C);
        nMaxStep = 0;
        v68 = 1;
    }
    else
    {
        return 0;
    }

    D2UnitStrc* pObject = nullptr;
    int32_t nDensitya = nDensity * (drlgCoords.nSubtileWidth * drlgCoords.nSubtileHeight >> 7) >> 8;
    while (nDensitya > 0 && v51 > 0)
    {
        const int32_t nClassId = objectIds[ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, nMaxObjects)];

        uint16_t nX = drlgCoords.nSubtileX + ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, drlgCoords.nSubtileWidth - pObjectsTxtRecord->dwSizeX - 1);
        uint16_t nY = drlgCoords.nSubtileY + ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, drlgCoords.nSubtileHeight - pObjectsTxtRecord->dwSizeY - 1);
        if (drlgCoords.nSubtileWidth >= pObjectsTxtRecord->dwSizeX + 2
            && drlgCoords.nSubtileHeight >= pObjectsTxtRecord->dwSizeY + 2
            && nX > drlgCoords.nSubtileX + 1
            && nY > drlgCoords.nSubtileY + 1
            && nX < drlgCoords.nSubtileX - pObjectsTxtRecord->dwSizeX + drlgCoords.nSubtileWidth - 2
            && nY < drlgCoords.nSubtileY - pObjectsTxtRecord->dwSizeY + drlgCoords.nSubtileHeight - 2
            && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 7, pObjectsTxtRecord->dwSizeY + 7, 0xC01u)
            && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX, pObjectsTxtRecord->dwSizeY, 0x3F11u))
        {
            D2_ASSERT(pRoom);

            int32_t v28 = 1;
            SUNIT_AllocUnitData(UNIT_OBJECT, nClassId, nX, nY, pGame, pRoom, 1, 0, 0);
            int32_t nSpawnedObjects = 1;
            pObject = nullptr;

        LABEL_45:
            if (!ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, nSpawnedObjects >> 1) && v28)
            {
                v28 = 0;
                int32_t nCounter = 0;
                const int32_t v60 = 3 * std::max(nDensitya, 4);
                while (1)
                {
                    if (nCounter >= v60)
                    {
                        if (v28)
                        {
                            pObject = SUNIT_AllocUnitData(UNIT_OBJECT, objectIds[ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, nMaxObjects)], nX, nY, pGame, pRoom, 1, 0, 0);
                            ++nSpawnedObjects;
                        }

                        goto LABEL_45;
                    }

                    const uint32_t v33 = ITEMS_RollRandomNumber(&pObjectControl->pSeed) & 7;
                    nX += 2 * (nBaseStep + ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, nMaxStep)) * signX[v33];
                    nY += 2 * (nBaseStep + ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, nMaxStep)) * signY[v33];

                    v28 = 0;
                    if (v68)
                    {
                        if (drlgCoords.nSubtileWidth >= 2
                            && drlgCoords.nSubtileHeight >= 2
                            && nX > drlgCoords.nSubtileX + 2
                            && nY > drlgCoords.nSubtileY + 2
                            && nX < drlgCoords.nSubtileWidth + drlgCoords.nSubtileX - pObjectsTxtRecord->dwSizeX
                            && nY < drlgCoords.nSubtileHeight + drlgCoords.nSubtileY - pObjectsTxtRecord->dwSizeY
                            && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 2, pObjectsTxtRecord->dwSizeY + 2, 0x3F11u))
                        {
                            v28 = 1;
                        }
                    }
                    else
                    {
                        if (drlgCoords.nSubtileWidth >= pObjectsTxtRecord->dwSizeX + 2
                            && drlgCoords.nSubtileHeight >= pObjectsTxtRecord->dwSizeY + 2
                            && nX > drlgCoords.nSubtileX + 1
                            && nY > drlgCoords.nSubtileY + 1
                            && nX < drlgCoords.nSubtileX - pObjectsTxtRecord->dwSizeX + drlgCoords.nSubtileWidth - 2
                            && nY < drlgCoords.nSubtileY - pObjectsTxtRecord->dwSizeY + drlgCoords.nSubtileHeight - 2
                            && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 7, pObjectsTxtRecord->dwSizeY + 7, 0xC01u)
                            && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX, pObjectsTxtRecord->dwSizeY, 0x3F11u))
                        {
                            v28 = 1;
                        }
                    }

                    ++nCounter;

                    if (v28)
                    {
                        pObject = SUNIT_AllocUnitData(UNIT_OBJECT, objectIds[ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, nMaxObjects)], nX, nY, pGame, pRoom, 1, 0, 0);
                        ++nSpawnedObjects;
                        goto LABEL_45;
                    }
                }
            }
            else
            {
                --nDensitya;
            }
        }

        --v51;
    }

    return pObject;
}

//D2Game.0x6FC71FB0
D2UnitStrc* __fastcall OBJECTS_PopulateFn7_RogueOnStick(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability)
{
    constexpr D2CoordStrc stru_6FD2F758[6] =
    {
        { -4, 0 },
        { 0, 0 },
        { 4, 0 },
        { 8, 0 },
        { 0, 4 },
        { 0, -4 },
    };
    
    constexpr D2CoordStrc stru_6FD2F78C[5] =
    {
        { -8, 0 },
        { -4, 0 },
        { 0, 0 },
        { 4, 0 },
        { 8, 0 },
    };
    
    constexpr D2CoordStrc stru_6FD2F7B8[5] =
    {
        { 0, -8 },
        { 0, -4 },
        { 0, 0 },
        { 0, 4 },
        { 0, 8 },
    };

    constexpr D2CoordStrc stru_6FD2F7E4[7] =
    {
        { -7, -5 },
        { -5, -3 },
        { -3, -1 },
        { 0, 0 },
        { 3, -1 },
        { 5, -3 },
        { 7, -5 },
    };

    const D2ObjectCoordInitStrc stru_6FD2F820[4] =
    {
        { stru_6FD2F758, std::size(stru_6FD2F758) },
        { stru_6FD2F78C, std::size(stru_6FD2F78C) },
        { stru_6FD2F7B8, std::size(stru_6FD2F7B8) },
        { stru_6FD2F7E4, std::size(stru_6FD2F7E4) },
    };

    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);

    if (ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 100 > nProbability)
    {
        return nullptr;
    }

    const int32_t nIndex = ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, std::size(stru_6FD2F820));
    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nObjectId);

    D2DrlgCoordsStrc drlgCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

    D2UnitStrc* pObject = nullptr;
    for (int32_t i = 0; i < 8; ++i)
    {
        const int32_t nBaseX = drlgCoords.nSubtileX + ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, drlgCoords.nSubtileWidth - pObjectsTxtRecord->dwSizeX - 1);
        const int32_t nBaseY = drlgCoords.nSubtileY + ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, drlgCoords.nSubtileHeight - pObjectsTxtRecord->dwSizeY - 1);
        if (drlgCoords.nSubtileWidth >= 7
            && drlgCoords.nSubtileHeight >= 7
            && (uint16_t)nBaseX > drlgCoords.nSubtileX + 1
            && (uint16_t)nBaseY > drlgCoords.nSubtileY + 1
            && (uint16_t)nBaseX < drlgCoords.nSubtileWidth + drlgCoords.nSubtileX - 7
            && (uint16_t)nBaseY < drlgCoords.nSubtileHeight + drlgCoords.nSubtileY - 7
            && !COLLISION_CheckMaskWithSizeXY(pRoom, nBaseX, nBaseY, 12u, 12u, 0xC01u)
            && !COLLISION_CheckMaskWithSizeXY(pRoom, nBaseX, nBaseY, 5u, 5u, 0x3F11u))
        {
            for (int32_t j = 0; j < stru_6FD2F820[nIndex].nCount; ++j)
            {
                const int32_t nX = nBaseX + stru_6FD2F820[nIndex].pCoords[j].nX;
                const int32_t nY = nBaseY + stru_6FD2F820[nIndex].pCoords[j].nY;

                if (drlgCoords.nSubtileWidth >= (int32_t)(pObjectsTxtRecord->dwSizeX + 2)
                    && drlgCoords.nSubtileHeight >= (int32_t)(pObjectsTxtRecord->dwSizeY + 2)
                    && (uint16_t)nX > drlgCoords.nSubtileX + 1
                    && (uint16_t)nY > drlgCoords.nSubtileY + 1
                    && (uint16_t)nX < (int32_t)(drlgCoords.nSubtileX + drlgCoords.nSubtileWidth - pObjectsTxtRecord->dwSizeX - 2)
                    && (uint16_t)nY < (int32_t)(drlgCoords.nSubtileY + drlgCoords.nSubtileHeight - pObjectsTxtRecord->dwSizeY - 2)
                    && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 7, pObjectsTxtRecord->dwSizeY + 7, 0xC01u)
                    && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX, pObjectsTxtRecord->dwSizeY, 0x3F11u))
                {
                    const uint32_t nClassId = (ITEMS_RollRandomNumber(&pObjectControl->pSeed) & 1) + OBJECT_ROGUE_ON_STICK1;
                    D2_ASSERT(nClassId <= OBJECT_ROGUE_ON_STICK2);
                    D2_ASSERT(pRoom);

                    pObject = SUNIT_AllocUnitData(UNIT_OBJECT, nClassId, nX, nY, pGame, pRoom, 1, 0, 0);
                    if (pObject)
                    {
                        OBJECTS_SpawnFliesOnCorpse(pGame, pObject);
                    }
                }
            }

            return pObject;
        }
    }

    return nullptr;
}

//D2Game.0x6FC72340
void __fastcall OBJECTS_SpawnFliesOnCorpse(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);
    if (ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 100 <= 70)
    {
        return;
    }

    D2CoordStrc coords = {};
    UNITS_GetCoords(pUnit, &coords);

    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    D2_ASSERT(pRoom);

    SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_FLIES, coords.nX, coords.nY, pGame, pRoom, 1, 0, 0);
}

//D2Game.0x6FC723F0
D2UnitStrc* __fastcall OBJECTS_PopulateFn3_CommonObjects(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nChance)
{
    D2_ASSERT(nDensity <= 128);

    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nObjectId);

    D2DrlgCoordsStrc drlgCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);
    if (ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 100 > nChance)
    {
        return nullptr;
    }

    D2UnitStrc* pObject = nullptr;

    const int32_t nMax = nDensity * (drlgCoords.nSubtileWidth * drlgCoords.nSubtileHeight >> 7) >> 8;
    for (int32_t i = 0; i < nMax; ++i)
    {
        pObject = OBJECTS_CreateObject(pGame, nObjectId, pObjectsTxtRecord->dwSizeX, pObjectsTxtRecord->dwSizeY, pRoom);
        if (pObject)
        {
            if (pObjectsTxtRecord->nSelectable[pObject->dwAnimMode])
            {
                pObject->dwFlags |= UNITFLAG_TARGETABLE;
            }
            else
            {
                pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
            }
        }
    }

    return pObject;
}

//D2Game.0x6FC72510
D2UnitStrc* __fastcall OBJECTS_CreateObject(D2GameStrc* pGame, int32_t nClassId, int32_t nSizeX, int32_t nSizeY, D2ActiveRoomStrc* pRoom)
{
    D2DrlgCoordsStrc drlgCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

    if (drlgCoords.nSubtileWidth < 2 || drlgCoords.nSubtileHeight < 2)
    {
        return nullptr;
    }

    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);
    for (int32_t i = 0; i < 5; ++i)
    {
        const uint16_t nX = ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, drlgCoords.nSubtileWidth - nSizeX - 1) + drlgCoords.nSubtileX;
        const uint16_t nY = ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, drlgCoords.nSubtileHeight - nSizeY - 1) + drlgCoords.nSubtileY;
        if (nX && nY && nX >= drlgCoords.nSubtileX + 1 && nY >= drlgCoords.nSubtileY + 1 && nX < drlgCoords.nSubtileX + drlgCoords.nSubtileWidth - 1
            && nY < drlgCoords.nSubtileY + drlgCoords.nSubtileHeight - 1 && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, nSizeX + 6, nSizeY + 6, 0x3F11u))
        {
            D2_ASSERT(pRoom);

            return SUNIT_AllocUnitData(UNIT_OBJECT, nClassId, nX, nY, pGame, pRoom, 1, 0, 0);
        }
    }

    return nullptr;
}

//D2Game.0x6FC726D0
D2UnitStrc* __fastcall OBJECTS_PopulateFn9_TrappedSoul(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability)
{
    D2_ASSERT(nDensity <= 128);

    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nObjectId);

    D2DrlgCoordsStrc drlgCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    if (ITEMS_RollRandomNumber(pSeed) % 100 > nProbability)
    {
        return nullptr;
    }

    const int32_t nMax = nDensity * (drlgCoords.nSubtileWidth * drlgCoords.nSubtileHeight >> 7) >> 8;

    for (int32_t i = 0; i < nMax; ++i)
    {
        D2UnitStrc* pObject = OBJECTS_CreateObject(pGame, nObjectId, pObjectsTxtRecord->dwSizeX, pObjectsTxtRecord->dwSizeY, pRoom);
        if (pObject)
        {
            if (pObjectsTxtRecord->nSelectable[pObject->dwAnimMode])
            {
                pObject->dwFlags |= UNITFLAG_TARGETABLE;
            }
            else
            {
                pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
            }

            return pObject;
        }
    }

    return nullptr;
}

//D2Game.0x6FC727F0
D2UnitStrc* __fastcall OBJECTS_PopulateFn6_RogueGuardCorpse(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability)
{
    D2UnitStrc* pObject = OBJECTS_PopulateFn3_CommonObjects(pGame, pRoom, nDensity, nObjectId, nProbability);
    if (!pObject)
    {
        return nullptr;
    }

    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);

    if (ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 100 <= 70)
    {
        return pObject;
    }

    D2CoordStrc coords = {};
    UNITS_GetCoords(pObject, &coords);

    D2ActiveRoomStrc* pObjectRoom = UNITS_GetRoom(pObject);
    D2_ASSERT(pObjectRoom);

    SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_FLIES, coords.nX, coords.nY, pGame, pObjectRoom, 1, 0, 0);
    return pObject;
}

//D2Game.0x6FC728C0
D2UnitStrc* __fastcall OBJECTS_PopulateFn8_Well(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability)
{
    const int32_t nLevelId = DUNGEON_GetLevelIdFromPopulatedRoom(pRoom);
    if (OBJRGN_CanNotSpawnMoreWells(pGame, nLevelId))
    {
        return nullptr;
    }

    D2_ASSERT(nDensity <= 128);

    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nObjectId);

    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    if (ITEMS_RollRandomNumber(pSeed) % 100 > nProbability)
    {
        return nullptr;
    }

    D2UnitStrc* pObject = nullptr;
    const int32_t nSizeX = 2 * pObjectsTxtRecord->dwSizeX + 1;
    const int32_t nSizeY = 2 * pObjectsTxtRecord->dwSizeY + 1;
    for (int32_t i = 0; i < 1; ++i)
    {
        D2DrlgCoordsStrc drlgCoords = {};
        DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

        pObject = nullptr;
        if (drlgCoords.nSubtileWidth >= 2 && drlgCoords.nSubtileHeight >= 2 && drlgCoords.nSubtileWidth > nSizeX && drlgCoords.nSubtileHeight > nSizeY)
        {
            for (int32_t j = 0; j < 5; ++j)
            {
                const int32_t nX = drlgCoords.nSubtileX + ITEMS_RollLimitedRandomNumber(pSeed, drlgCoords.nSubtileWidth - nSizeX - 1);
                const int32_t nY = drlgCoords.nSubtileY + ITEMS_RollLimitedRandomNumber(pSeed, drlgCoords.nSubtileHeight - nSizeY - 1);
                if (nX && nY 
                    && (uint16_t)nX >= drlgCoords.nSubtileX + 1
                    && (uint16_t)nY >= drlgCoords.nSubtileY + 1
                    && (uint16_t)nX < drlgCoords.nSubtileX + drlgCoords.nSubtileWidth - 1
                    && (uint16_t)nY < drlgCoords.nSubtileHeight + drlgCoords.nSubtileY - 1)
                {
                    if (IsBadCodePtr((FARPROC)OBJRGN_CanSpawnWell))
                    {
                        FOG_DisplayAssert("ptFilter", __FILE__, __LINE__);
                        exit(-1);
                    }

                    if (!COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, nSizeX + 6, nSizeY + 6, 0x3F11u) && OBJRGN_CanSpawnWell(pGame, nLevelId, nX, nY))
                    {
                        D2_ASSERT(pRoom);

                        pObject = SUNIT_AllocUnitData(UNIT_OBJECT, nObjectId, nX, nY, pGame, pRoom, 1, 0, 0);
                        if (pObject)
                        {
                            if (pObjectsTxtRecord->nSelectable[pObject->dwAnimMode])
                            {
                                pObject->dwFlags |= UNITFLAG_TARGETABLE;
                            }
                            else
                            {
                                pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
                            }

                            OBJRGN_SetWellCoordinates(pGame, nLevelId, CLIENTS_GetUnitX(pObject), CLIENTS_GetUnitY(pObject));
                        }
                        break;
                    }
                }
            }
        }
    }

    return pObject;
}

//D2Game.0x6FC72C30
D2UnitStrc* __fastcall OBJECTS_PopulateFn2_WaypointShrine(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability)
{
    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nObjectId);
    const int32_t nLevelId = DUNGEON_GetLevelIdFromPopulatedRoom(pRoom);

    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    bool bSpawnHealingWell = false;
    int32_t nCounter = 3;
    if (OBJRGN_ShouldSpawnHealingShrineOrWell(pGame, nLevelId))
    {
        bSpawnHealingWell = true;
        nCounter = 30;
    }
    else if (ITEMS_RollRandomNumber(pSeed) % 100 > nProbability)
    {
        return nullptr;
    }

    if (OBJRGN_CanNotSpawnMoreShrines(pGame, nLevelId))
    {
        return nullptr;
    }

    while (nCounter > 0)
    {
        D2DrlgCoordsStrc drlgCoords = {};
        DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

        if (drlgCoords.nSubtileWidth >= (int32_t)(pObjectsTxtRecord->dwSizeX + 2) && drlgCoords.nSubtileHeight >= (int32_t)(pObjectsTxtRecord->dwSizeY + 2))
        {
            for (int32_t nProbabilitya = 0; nProbabilitya < 5; ++nProbabilitya)
            {
                int32_t nX = 0;
                int32_t nY = 0;
                if (pObjectsTxtRecord->nOrientation == 1)
                {
                    nX = drlgCoords.nSubtileX + drlgCoords.nSubtileWidth / 4 + ITEMS_RollLimitedRandomNumber(pSeed, drlgCoords.nSubtileWidth / 2);
                    nY = drlgCoords.nSubtileY + OBJECTS_RollRandomNumber(pSeed, 1, 2);
                }
                else if (pObjectsTxtRecord->nOrientation == 2)
                {
                    nX = drlgCoords.nSubtileX + OBJECTS_RollRandomNumber(pSeed, 1, 2);
                    nY = ITEMS_RollLimitedRandomNumber(pSeed, drlgCoords.nSubtileHeight / 2) + drlgCoords.nSubtileY + drlgCoords.nSubtileHeight / 4;
                }
                else
                {
                    nX = drlgCoords.nSubtileX + ITEMS_RollLimitedRandomNumber(pSeed, drlgCoords.nSubtileWidth - pObjectsTxtRecord->dwSizeX - 1);
                    nY = drlgCoords.nSubtileY + ITEMS_RollLimitedRandomNumber(pSeed, drlgCoords.nSubtileHeight - pObjectsTxtRecord->dwSizeY - 1);
                }

                if (nX && nY && (uint16_t)nX >= drlgCoords.nSubtileX + 1 && (uint16_t)nY >= drlgCoords.nSubtileY + 1 && (uint16_t)nX < drlgCoords.nSubtileX + drlgCoords.nSubtileWidth - 1 && (uint16_t)nY < drlgCoords.nSubtileY + drlgCoords.nSubtileHeight - 1)
                {
                    if (IsBadCodePtr((FARPROC)OBJRGN_CanSpawnShrine))
                    {
                        FOG_DisplayAssert("ptFilter", __FILE__, __LINE__);
                        exit(-1);
                    }

                    if (!COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 6, pObjectsTxtRecord->dwSizeY + 6, 0x3F11u) && OBJRGN_CanSpawnShrine(pGame, nLevelId, nX, nY))
                    {
                        D2_ASSERT(pRoom);

                        D2UnitStrc* pObject = SUNIT_AllocUnitData(UNIT_OBJECT, nObjectId, nX, nY, pGame, pRoom, 1, 0, 0);
                        if (pObject)
                        {
                            if (pObjectsTxtRecord->nSelectable[pObject->dwAnimMode])
                            {
                                pObject->dwFlags |= UNITFLAG_TARGETABLE;
                            }
                            else
                            {
                                pObject->dwFlags &= ~UNITFLAG_TARGETABLE;
                            }

                            if (pObject->pObjectData->InteractType == 2)
                            {
                                OBJRGN_IncreaseHealingShrineCount(pGame, nLevelId);
                            }
                            else if (bSpawnHealingWell)
                            {
                                const int32_t nClassId = DATATBLS_GetObjectsTxtRecord(pObject->dwClassId)->dwParm[1];

                                pObject->dwClassId = OBJECT_HEALINGWELL;
                                if (nClassId && nClassId < 573)
                                {
                                    pObject->dwClassId = nClassId;
                                }

                                OBJECTS_GetShrineId(pGame, 2u, 1, pRoom);
                                pObject->pObjectData->InteractType = 2;
                                UNITS_SetShrineTxtRecordInObjectData(pObject, DATATBLS_GetShrinesTxtRecord(2));
                                OBJRGN_IncreaseHealingShrineCount(pGame, nLevelId);
                            }

                            OBJRGN_SetShrineCoordinates(pGame, nLevelId, CLIENTS_GetUnitX(pObject), CLIENTS_GetUnitY(pObject));

                            return pObject;
                        }

                        break;
                    }
                }
            }
        }

        --nCounter;
    }

    return nullptr;
}

//D2Game.0x6FC73050
int32_t __fastcall OBJECTS_RollRandomNumber(D2SeedStrc* pSeed, int32_t nMin, int32_t nMax)
{
    return ITEMS_RollLimitedRandomNumber(pSeed, nMax - nMin) + nMin;
}

//D2Game.0x6FC730C0
D2UnitStrc* __fastcall OBJECTS_PopulateFn4_Barrel(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability)
{
    constexpr int signX[] =
    {
        -1, 0, 1, -1, 1, -1, 0, 1
    };

    constexpr int signY[] =
    {
        -1, -1, -1, 0, 0, 1, 1, 1
    };

    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(OBJECT_BARREL);

    D2_ASSERT(nDensity <= 128);

    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);
    if (ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 100 > nProbability)
    {
        return 0;
    }

    D2DrlgCoordsStrc drlgCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

    D2UnitStrc* pObject = nullptr;
    int32_t j = nDensity * (drlgCoords.nSubtileWidth * drlgCoords.nSubtileHeight >> 7) >> 8;
    int32_t k = 2 * j;
    int32_t nCounter = 0;
    while (j > 0 && k > 0)
    {
        const int32_t nClassId = (ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 3 == 0) ? OBJECT_EXPLODING_BARREL : OBJECT_BARREL;
        int32_t nX = drlgCoords.nSubtileX + ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, drlgCoords.nSubtileWidth);
        int32_t nY = drlgCoords.nSubtileY + ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, drlgCoords.nSubtileHeight);

        if (drlgCoords.nSubtileWidth >= 2 && drlgCoords.nSubtileHeight >= 2
            && (uint16_t)nX > drlgCoords.nSubtileX + 2 && (uint16_t)nY > drlgCoords.nSubtileY + 2
            && (uint16_t)nX < (int32_t)(drlgCoords.nSubtileWidth + drlgCoords.nSubtileX - pObjectsTxtRecord->dwSizeX)
            && (uint16_t)nY < (int32_t)(drlgCoords.nSubtileHeight + drlgCoords.nSubtileY - pObjectsTxtRecord->dwSizeY)
            && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 2, pObjectsTxtRecord->dwSizeY + 2, 0x3F11u))
        {
            D2_ASSERT(pRoom);

            pObject = SUNIT_AllocUnitData(UNIT_OBJECT, nClassId, nX, nY, pGame, pRoom, 1, 0, 0);
            bool bSpawnObject = true;
            int32_t nMax = 1;

            ++nCounter;
            if (nCounter >= 8)
            {
                return pObject;
            }

            pObject = nullptr;
            while (!ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, nMax / 2) && bSpawnObject)
            {
                bSpawnObject = false;
                for (int32_t i = 0; !bSpawnObject && i < 15; ++i)
                {
                    const int32_t nIndex = ITEMS_RollRandomNumber(&pObjectControl->pSeed) & 7;
                    nX += pObjectsTxtRecord->nXSpace * signX[nIndex];
                    nY += pObjectsTxtRecord->nYSpace * signY[nIndex];

                    bSpawnObject = drlgCoords.nSubtileWidth >= 2 && drlgCoords.nSubtileHeight >= 2
                        && (uint16_t)nX > drlgCoords.nSubtileX + 2
                        && (uint16_t)nY > drlgCoords.nSubtileY + 2
                        && (uint16_t)nX < (int32_t)(drlgCoords.nSubtileWidth + drlgCoords.nSubtileX - pObjectsTxtRecord->dwSizeX)
                        && (uint16_t)nY < (int32_t)(drlgCoords.nSubtileHeight + drlgCoords.nSubtileY - pObjectsTxtRecord->dwSizeY)
                        && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 2, pObjectsTxtRecord->dwSizeY + 2, 0x3F11u);
                }

                if (bSpawnObject)
                {
                    pObject = SUNIT_AllocUnitData(UNIT_OBJECT, ((ITEMS_RollRandomNumber(&pObjectControl->pSeed) & 3) == 0) ? OBJECT_EXPLODING_BARREL : OBJECT_BARREL, nX, nY, pGame, pRoom, 1, 0, 0);
                    ++nCounter;
                    ++nMax;

                    if (nCounter >= 8)
                    {
                        return pObject;
                    }
                }
            }

            --j;
        }

        --k;
    }

    return pObject;
}

//D2Game.0x6FC73550
D2UnitStrc* __fastcall OBJECTS_PopulateFn5_Crate(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability)
{
    constexpr int32_t gnUrnIds[] =
    {
        OBJECT_URN1, OBJECT_URN2, OBJECT_URN3, OBJECT_URN4, OBJECT_URN5,
    };

    constexpr int32_t signX[] =
    {
        -1, 0, 1, -1, 1, -1, 0, 1
    };

    constexpr int32_t signY[] =
    {
        -1, -1, -1, 0, 0, 1, 1, 1
    };
    
    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nObjectId);

    D2_ASSERT(nDensity <= 128);

    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);
    if (ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 100 > nProbability)
    {
        return nullptr;
    }
    
    D2DrlgCoordsStrc drlgCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

    D2UnitStrc* pObject = nullptr;
    int32_t j = nDensity * (drlgCoords.nSubtileWidth * drlgCoords.nSubtileHeight >> 7) >> 8;
    int32_t k = 2 * j;
    int32_t l = 4 * j;
    int32_t nCounter = 0;
    while (j > 0 && k > 0)
    {
        int32_t nClassId = nObjectId;
        if (nObjectId != OBJECT_CRATE)
        {
            nClassId = gnUrnIds[ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, std::size(gnUrnIds))];
        }

        int32_t nX = drlgCoords.nSubtileX + ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, drlgCoords.nSubtileWidth);
        int32_t nY = drlgCoords.nSubtileY + ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, drlgCoords.nSubtileHeight);

        if (drlgCoords.nSubtileWidth >= 2
            && drlgCoords.nSubtileHeight >= 2
            && (uint16_t)nX > drlgCoords.nSubtileX + 2
            && (uint16_t)nY > drlgCoords.nSubtileY + 2
            && (uint16_t)nX < (int32_t)(drlgCoords.nSubtileWidth + drlgCoords.nSubtileX - pObjectsTxtRecord->dwSizeX)
            && (uint16_t)nY < (int32_t)(drlgCoords.nSubtileHeight + drlgCoords.nSubtileY - pObjectsTxtRecord->dwSizeY))
        {
            if (!COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 2, pObjectsTxtRecord->dwSizeY + 2, 0x3F11u))
            {
                if (nClassId != OBJECT_CRATE)
                {
                    nClassId = gnUrnIds[ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, std::size(gnUrnIds))];
                }

                D2_ASSERT(pRoom);

                SUNIT_AllocUnitData(UNIT_OBJECT, nClassId, nX, nY, pGame, pRoom, 1, 0, 0);

                bool bSpawnObject = true;
                int32_t nMax = 1;
                pObject = nullptr;
                while (!ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, nMax / 2) || bSpawnObject)
                {
                    bSpawnObject = false;
                    int32_t nCount = 4;
                    if (l >= 4)
                    {
                        nCount = l;
                    }

                   
                    for (int32_t i = 0; !bSpawnObject && i < nCount; ++i)
                    {
                        const int32_t v32 = ITEMS_RollRandomNumber(&pObjectControl->pSeed) & 7;
                        nX += pObjectsTxtRecord->nXSpace * signX[v32];
                        nY += pObjectsTxtRecord->nYSpace * signY[v32];

                        bSpawnObject = drlgCoords.nSubtileWidth >= 2
                            && drlgCoords.nSubtileHeight >= 2
                            && (uint16_t)nX > drlgCoords.nSubtileX + 2
                            && (uint16_t)nY > drlgCoords.nSubtileY + 2
                            && (uint16_t)nX < (int32_t)(drlgCoords.nSubtileWidth + drlgCoords.nSubtileX - pObjectsTxtRecord->dwSizeX)
                            && (uint16_t)nY < (int32_t)(drlgCoords.nSubtileHeight + drlgCoords.nSubtileY - pObjectsTxtRecord->dwSizeY)
                            && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 2, pObjectsTxtRecord->dwSizeY + 2, 0x3F11u);
                    }

                    if (bSpawnObject)
                    {
                        pObject = SUNIT_AllocUnitData(UNIT_OBJECT, nClassId, nX, nY, pGame, pRoom, 1, 0, 0);
                        if (pObject)
                        {
                            ++nCounter;
                            if (nCounter >= 8)
                            {
                                return pObject;
                            }
                        }

                        ++nMax;
                    }
                }

                --j;
                l -= 4;
            }
        }

        --k;
    }

    return pObject;
}

//D2Game.0x6FC73A70
int32_t __fastcall OBJECTS_SpawnNothing(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords)
{
    return 1;
}

//D2Game.0x6FC73A80
int32_t __fastcall OBJECTS_SpawnBarrel(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords)
{
    const uint16_t nXEnd = pDrlgCoords->nSubtileX + pDrlgCoords->nSubtileWidth + 1;
    const uint16_t nYEnd = pDrlgCoords->nSubtileY + pDrlgCoords->nSubtileHeight + 1;
    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(OBJECT_BARREL);
    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);

    int32_t nCounter = 0;

    for (uint16_t nX = pDrlgCoords->nSubtileX; nX < nXEnd; nX += 2)
    {
        for (uint16_t nY = pDrlgCoords->nSubtileY; nY < nYEnd; nY += 2)
        {
            const int32_t nClassId = ((ITEMS_RollRandomNumber(&pObjectControl->pSeed) & 3) == 0) ? OBJECT_EXPLODING_BARREL : OBJECT_BARREL;

            if (pDrlgCoords->nSubtileWidth >= 2 && pDrlgCoords->nSubtileHeight >= 2 && nX > pDrlgCoords->nSubtileX + 2 && nY > pDrlgCoords->nSubtileY + 2)
            {
                if (nX < (int32_t)(pDrlgCoords->nSubtileWidth + pDrlgCoords->nSubtileX - pObjectsTxtRecord->dwSizeX)
                    && nY < (int32_t)(pDrlgCoords->nSubtileHeight + pDrlgCoords->nSubtileY - pObjectsTxtRecord->dwSizeY))
                {
                    if (!COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 2, pObjectsTxtRecord->dwSizeY + 2, 0x3F11u))
                    {
                        D2_ASSERT(pRoom);

                        SUNIT_AllocUnitData(UNIT_OBJECT, nClassId, nX, nY, pGame, pRoom, 1, 0, 0);

                        ++nCounter;
                        if (nCounter >= 32)
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }

    return 1;
}

//D2Game.0x6FC73C50
int32_t __fastcall OBJECTS_SpawnNothing2(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords)
{
    const uint16_t nXEnd = pDrlgCoords->nSubtileX + std::min(pDrlgCoords->nSubtileWidth, 12);
    const uint16_t nYEnd = pDrlgCoords->nSubtileY + std::min(pDrlgCoords->nSubtileHeight, 12);
    
    int32_t nCounter = 0;
    
    for (uint16_t nX = pDrlgCoords->nSubtileX + 1; nX < nXEnd; nX += 2)
    {
        for (uint16_t nY = pDrlgCoords->nSubtileY + 1; nY < nYEnd; nY += 2)
        {
            D2CoordStrc coords = {};
            coords.nX = nX;
            coords.nY = nY;

            if (pDrlgCoords->nSubtileWidth >= 2
                && pDrlgCoords->nSubtileHeight >= 2
                && nX > pDrlgCoords->nSubtileX + 2
                && nY > pDrlgCoords->nSubtileY + 2
                && nX < pDrlgCoords->nSubtileX + pDrlgCoords->nSubtileWidth - 1
                && nY < pDrlgCoords->nSubtileY + pDrlgCoords->nSubtileHeight - 1
                && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, 3u, 3u, 0x3F11u))
            {
                sub_6FC4F290(pGame, pRoom, &coords, nullptr);

                ++nCounter;
                if (nCounter >= 32)
                {
                    return 1;
                }
            }
        }
    }

    return 1;
}

//D2Game.0x6FC73D80
int32_t __fastcall OBJECTS_SpawnArmorStand(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords)
{
    OBJECTS_SpawnArmorStandOrWeaponRack(pGame, pRoom, pDrlgCoords, 0);
    return 1;
}

//D2Game.0x6FC73DA0
void __fastcall OBJECTS_SpawnArmorStandOrWeaponRack(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords, int32_t bWeaponRack)
{
    constexpr int32_t nObjectIds[4] =
    {
        OBJECT_ARMOR_STAND_RIGHT, OBJECT_ARMOR_STAND_LEFT, OBJECT_WEAPON_RACK_RIGHT, OBJECT_WEAPON_RACK_LEFT
    };

    constexpr int32_t dword_6FD2F88C = 2;
    if (bWeaponRack >= dword_6FD2F88C)
    {
        return;
    }

    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);
    const int32_t nRand = ITEMS_RollRandomNumber(&pObjectControl->pSeed) & 1;
    const int32_t nObjectId = nObjectIds[nRand + 2 * bWeaponRack];
    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nObjectId);

    if (nRand)
    {
        const uint16_t nX = pDrlgCoords->nSubtileX + 1;
        
        const uint32_t nYEnd = pDrlgCoords->nSubtileHeight + pDrlgCoords->nSubtileY - pObjectsTxtRecord->dwSizeY;
        for (uint16_t nY = pDrlgCoords->nSubtileY + pObjectsTxtRecord->dwSizeY; nY < nYEnd; nY += 2 * pObjectsTxtRecord->dwSizeY)
        {
            if (pDrlgCoords->nSubtileWidth >= (int32_t)(pObjectsTxtRecord->dwSizeX + 2) && pDrlgCoords->nSubtileHeight >= (int32_t)(pObjectsTxtRecord->dwSizeY + 2)
                && nX > pDrlgCoords->nSubtileX + 1 && nY > pDrlgCoords->nSubtileY + 1 
                && nX < (int32_t)(pDrlgCoords->nSubtileX - pObjectsTxtRecord->dwSizeX + pDrlgCoords->nSubtileWidth - 2)
                && nY < (int32_t)(pDrlgCoords->nSubtileY - pObjectsTxtRecord->dwSizeY + pDrlgCoords->nSubtileHeight - 2))
            {
                if (!COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 7, pObjectsTxtRecord->dwSizeY + 7, 0xC01u) && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX, pObjectsTxtRecord->dwSizeY, 0x3F11u))
                {
                    D2_ASSERT(pRoom);

                    SUNIT_AllocUnitData(UNIT_OBJECT, nObjectId, nX, nY, pGame, pRoom, 1, 0, 0);
                }
            }
        }
    }
    else
    {
        const uint16_t nY = pDrlgCoords->nSubtileY + 1;
        
        const uint32_t nXEnd = pDrlgCoords->nSubtileX + pDrlgCoords->nSubtileWidth - pObjectsTxtRecord->dwSizeX;
        for (uint16_t nX = pDrlgCoords->nSubtileX + pObjectsTxtRecord->dwSizeX; nX < nXEnd; nX += 2 * pObjectsTxtRecord->dwSizeX)
        {
            if (pDrlgCoords->nSubtileWidth >= (int32_t)(pObjectsTxtRecord->dwSizeX + 2) && pDrlgCoords->nSubtileHeight >= (int32_t)(pObjectsTxtRecord->dwSizeY + 2) 
                && nX > pDrlgCoords->nSubtileX + 1 && nY > pDrlgCoords->nSubtileY + 1
                && nX < (int32_t)(pDrlgCoords->nSubtileX - pObjectsTxtRecord->dwSizeX + pDrlgCoords->nSubtileWidth - 2)
                && nY < (int32_t)(pDrlgCoords->nSubtileY - pObjectsTxtRecord->dwSizeY + pDrlgCoords->nSubtileHeight - 2))
            {
                if (!COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX + 7, pObjectsTxtRecord->dwSizeY + 7, 0xC01u) && !COLLISION_CheckMaskWithSizeXY(pRoom, nX, nY, pObjectsTxtRecord->dwSizeX, pObjectsTxtRecord->dwSizeY, 0x3F11u))
                {
                    D2_ASSERT(pRoom);

                    SUNIT_AllocUnitData(UNIT_OBJECT, nObjectId, nX, nY, pGame, pRoom, 1, 0, 0);
                }
            }
        }
    }
}

//D2Game.0x6FC740C0
int32_t __fastcall OBJECTS_SpawnWeaponRack(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords)
{
    OBJECTS_SpawnArmorStandOrWeaponRack(pGame, pRoom, pDrlgCoords, 1);
    return 1;
}

//D2Game.0x6FC740E0
void __fastcall OBJECTS_PopulationHandler(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom)
{
    constexpr D2ObjectPopulateStrc stru_6FD2F844[7] =
    {
        { 0, nullptr },
        { 1, OBJECTS_SpawnNothing },
        { 1, OBJECTS_SpawnBarrel },
        { 0, nullptr },
        { 1, OBJECTS_SpawnNothing2 },
        { 1, OBJECTS_SpawnArmorStand },
        { 1, OBJECTS_SpawnWeaponRack }
    };

    using ObjectPopulateFunction = D2UnitStrc* (__fastcall*)(D2GameStrc*, D2ActiveRoomStrc*, uint8_t, uint32_t, uint16_t);
    constexpr ObjectPopulateFunction gpObjectPopulateTable[10] =
    {
        nullptr,
        OBJECTS_PopulateFn1_CasketJarSarcophagusUrn,
        OBJECTS_PopulateFn2_WaypointShrine,
        OBJECTS_PopulateFn3_CommonObjects,
        OBJECTS_PopulateFn4_Barrel,
        OBJECTS_PopulateFn5_Crate,
        OBJECTS_PopulateFn6_RogueGuardCorpse,
        OBJECTS_PopulateFn7_RogueOnStick,
        OBJECTS_PopulateFn8_Well,
        OBJECTS_PopulateFn9_TrappedSoul
    };

    constexpr uint8_t byte_6FD2F6E0 = 2;

    D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(DUNGEON_GetLevelIdFromRoom(pRoom));
    D2_ASSERT(pLevelsTxtRecord);

    if (DUNGEON_HasWaypoint(pRoom))
    {
        return;
    }

    const int32_t nLevelId = DUNGEON_GetLevelIdFromPopulatedRoom(pRoom);
    D2ObjectRegionStrc* pObjectRegion = OBJRGN_GetObjectRegionForLevel(pGame, nLevelId);

    if (!nLevelId || D2Common_10087(pRoom) || nLevelId >= sgptDataTables->nLevelsTxtRecordCount || DUNGEON_IsRoomInTown(pRoom))
    {
        return;
    }

    if (pObjectRegion->nPopulatedRooms == INT_MAX)
    {
        pObjectRegion->nPopulatedRooms = DUNGEON_GetNumberOfPopulatedRoomsInLevel(pGame->pAct[pObjectRegion->nAct], nLevelId);
    }

    ++pObjectRegion->field_4;

    if (pLevelsTxtRecord->dwThemes && pObjectRegion->nPopulatedRooms >= 10 * pObjectRegion->unk0x0C)
    {
        D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);

        int32_t nProbability = 0;
        if (pObjectRegion->field_4 > pObjectRegion->nPopulatedRooms / 2)
        {
            nProbability = 5;
        }

        if (pObjectRegion->field_4 > pObjectRegion->nPopulatedRooms - pObjectRegion->nPopulatedRooms / 4)
        {
            nProbability += 5;
        }

        if (ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, 100) < nProbability + 12)
        {
            D2DrlgCoordsStrc drlgCoords = {};
            DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

            int32_t nMax = 0;
            int32_t nIndices[7] = {};

            for (int32_t i = 0; i < 7; ++i)
            {
                if ((1 << i) & pLevelsTxtRecord->dwThemes)
                {
                    nIndices[nMax] = i + 1;
                    ++nMax;
                }
            }

            const int32_t nIndex = ITEMS_RollLimitedRandomNumber(&pObjectControl->pSeed, nMax);
            if (nIndex < nMax && nIndices[nIndex] < nMax && stru_6FD2F844[nIndices[nIndex]].bActive && stru_6FD2F844[nIndices[nIndex]].pfPopulate && stru_6FD2F844[nIndices[nIndex]].pfPopulate(pGame, pRoom, &drlgCoords) == 1)
            {
                return;
            }
        }
    }

    for (int32_t i = 0; i < 8; ++i)
    {
        const int32_t nObjGroup = pLevelsTxtRecord->nObjGroup[i];
        uint32_t nProbability = ITEMS_RollRandomNumber(&pRoom->pSeed) % 100;
        if (OBJRGN_ShouldSpawnHealingShrineOrWell(pGame, DUNGEON_GetLevelIdFromPopulatedRoom(pRoom)) && DATATBLS_GetObjectsTxtRecord(nObjGroup)->nSubClass)
        {
            nProbability = 100;
        }

        if (nObjGroup && nProbability <= pLevelsTxtRecord->nObjPrb[i])
        {
            D2ObjGroupTxt* pObjGroupTxtRecord = DATATBLS_GetObjGroupTxtRecord(nObjGroup);
            if (!pObjGroupTxtRecord)
            {
                return;
            }

            int32_t nAccumulatedProbability = 0;
            nProbability = ITEMS_RollRandomNumber(&pRoom->pSeed) % 100;
            for (int32_t nObjGroupId = 0; nObjGroupId < 8 && pObjGroupTxtRecord->dwId[nObjGroupId]; ++nObjGroupId)
            {
                nAccumulatedProbability += pObjGroupTxtRecord->nProbability[nObjGroupId];

                D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pObjGroupTxtRecord->dwId[nObjGroupId]);
                if (nProbability < nAccumulatedProbability && pObjectsTxtRecord->nGore <= byte_6FD2F6E0)
                {
                    D2_ASSERT(pObjectsTxtRecord->nPopulateFn < std::size(gpObjectPopulateTable));

                    const ObjectPopulateFunction pfPopulate = gpObjectPopulateTable[pObjectsTxtRecord->nPopulateFn];
                    if (pfPopulate)
                    {
                        if (IsBadCodePtr((FARPROC)pfPopulate))
                        {
                            FOG_DisplayAssert("sgPopulateTable[ptObjectType->bPopulateFunction]", __FILE__, __LINE__);
                            exit(-1);
                        }

                        D2_ASSERT(pObjGroupTxtRecord->nDensity[nObjGroupId] < 128);
                        pfPopulate(pGame, pRoom, pObjGroupTxtRecord->nDensity[nObjGroupId], pObjGroupTxtRecord->dwId[nObjGroupId], 100);
                    }

                    break;
                }
            }
        }
    }
}

//D2Game.0x6FC744B0
void __fastcall OBJECTS_FreeHoverMessage(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    if (pObject && pObject->pHoverText)
    {
        CHAT_FreeHoverMsg(pGame->pMemoryPool, pObject->pHoverText);
    }

    D2TimerArgStrc* pTimerArg = UNITS_GetTimerArg(pObject);
    D2TimerArg2Strc* pArg = pTimerArg->unk0x00;
    while (pArg)
    {
        D2TimerArg2Strc* pNext = pArg->pNext;
        D2_FREE_POOL(pGame->pMemoryPool, pArg);
        pArg = pNext;
    }

    D2_FREE_POOL(pGame->pMemoryPool, pTimerArg);
    sub_6FC349F0(pGame, pObject);
}

//D2Game.0x6FC74520
void __fastcall OBJECTS_RemoveAll(D2GameStrc* pGame)
{
    for (int32_t i = 0; i < 128; ++i)
    {
        D2UnitStrc* pObject = pGame->pUnitList[2][i];
        while (pObject)
        {
            D2UnitStrc* pNext = SUNIT_GetNextUnitFromList(pObject);
            SUNIT_RemoveUnit(pGame, pObject);
            pObject = pNext;
        }
    }
}

//D2Game.0x6FC74590
void __fastcall OBJECTS_SetUnitIdInTimerArg(D2UnitStrc* pPortal, int32_t nUnitId)
{
    D2TimerArgStrc* pTimerArg = UNITS_GetTimerArg(pPortal);
    if (pTimerArg)
    {
        pTimerArg->dwUnitId = nUnitId;
    }
}

//D2Game.0x6FC745B0
int32_t __fastcall OBJECTS_GetUnitIdFromTimerArg(D2UnitStrc* pUnit)
{
    D2TimerArgStrc* pTimerArg = UNITS_GetTimerArg(pUnit);
    if (pTimerArg)
    {
        return pTimerArg->dwUnitId;
    }

    return -1;
}
