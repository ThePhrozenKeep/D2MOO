#include "OBJECTS/ObjMode.h"

#include <algorithm>
#include <D2BitManip.h>

#include <D2PacketDef.h>

#include <D2Environment.h>
#include <D2Dungeon.h>
#include <DataTbls/ObjectsTbls.h>
#include <DataTbls/ObjectsIds.h>
#include <Units/UnitRoom.h>
#include <Units/UnitFinds.h>
#include <D2Chat.h>
#include <D2Collision.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/LevelsIds.h>
#include <D2Monsters.h>
#include <D2DataTbls.h>
#include <D2StatList.h>
#include <D2Skills.h>
#include <D2Items.h>
#include <D2QuestRecord.h>
#include <D2Waypoints.h>
#include <D2States.h>


#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Level.h"
#include "GAME/SCmd.h"
#include "INVENTORY/InvMode.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjEval.h"
#include "OBJECTS/ObjRgn.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerList.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlrModes.h"
#include "PLAYER/PlrMsg.h"
#include "PLAYER/PlrSave2.h"
#include "PLAYER/PlrTrade.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT1/A1Q3.h"
#include "QUESTS/ACT1/A1Q4.h"
#include "QUESTS/ACT1/A1Q5.h"
#include "QUESTS/ACT2/A2Q2.h"
#include "QUESTS/ACT2/A2Q3.h"
#include "QUESTS/ACT2/A2Q4.h"
#include "QUESTS/ACT2/A2Q6.h"
#include "QUESTS/ACT3/A3Q1.h"
#include "QUESTS/ACT3/A3Q2.h"
#include "QUESTS/ACT3/A3Q3.h"
#include "QUESTS/ACT3/A3Q5.h"
#include "QUESTS/ACT4/A4Q2.h"
#include "QUESTS/ACT4/A4Q3.h"
#include "QUESTS/ACT5/A5Q3.h"
#include "QUESTS/ACT5/A5Q5.h"
#include "QUESTS/ACT5/A5Q6.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitMsg.h"
#include "UNIT/SUnitNpc.h"



#pragma warning(disable: 28159)



//D2Game.0x6FD2FB78
constexpr ObjOperateFunction gpObjOperateFnTable[] =
{
    nullptr,
    OBJECTS_OperateFunction01_Casket,
    OBJECTS_OperateFunction02_Shrine,
    OBJECTS_OperateFunction03_Urn_Basket_Jar,
    OBJECTS_OperateFunction04_Chest,
    OBJECTS_OperateFunction05_Barrel,
    OBJECTS_OperateFunction06_TowerTome,
    OBJECTS_OperateFunction07_ExplodingBarrel,
    OBJECTS_OperateFunction08_Door,
    OBJECTS_OperateFunction09_Monolith,
    OBJECTS_OperateFunction10_CainGibbet,
    OBJECTS_OperateFunction11_Torch,
    OBJECTS_OperateFunction12_InifussTree,
    OBJECTS_OperateFunction13_TorchTiki,
    OBJECTS_OperateFunction14_Corpse,
    OBJECTS_OperateFunction15_Portal,
    OBJECTS_OperateFunction16_TrapDoor,
    OBJECTS_OperateFunction17_Obelisk,
    OBJECTS_OperateFunction18_SecretDoor,
    OBJECTS_OperateFunction19_ArmorStand,
    OBJECTS_OperateFunction20_WeaponRack,
    OBJECTS_OperateFunction21_HoradrimMalus,
    OBJECTS_OperateFunction22_Well,
    OBJECTS_OperateFunction23_Waypoint,
    OBJECTS_OperateFunction24_TaintedSunAltar,
    OBJECTS_OperateFunction25_StaffOrifice,
    OBJECTS_OperateFunction26_BookShelf,
    OBJECTS_OperateFunction27_TeleportPad,
    OBJECTS_OperateFunction28_LamEsenTome,
    OBJECTS_OperateFunction29_SlimeDoor,
    OBJECTS_OperateFunction30_ExplodingChest,
    OBJECTS_OperateFunction31_GidbinnDecoy,
    OBJECTS_OperateFunction32_Bank,
    OBJECTS_OperateFunction33_WirtsBody,
    OBJECTS_OperateFunction34_ArcaneSanctuaryPortal,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    OBJECTS_OperateFunction39_HoradricCubeChest,
    OBJECTS_OperateFunction40_HoradricScrollChest,
    OBJECTS_OperateFunction41_StaffOfKingsChest,
    OBJECTS_OperateFunction42_SanctuaryTome,
    OBJECTS_OperateFunction43_DurielPortal,
    OBJECTS_OperateFunction44_SewerStairs,
    OBJECTS_OperateFunction45_SewerLever,
    OBJECTS_OperateFunction46_HellGatePortal,
    OBJECTS_OperateFunction47_Stair,
    OBJECTS_OperateFunction48_TrappedSoul,
    OBJECTS_OperateFunction49_HellForge,
    OBJECTS_OperateFunction50_Stair,
    OBJECTS_OperateFunction51_JungleStash,
    OBJECTS_OperateFunction52_DiabloSeal,
    OBJECTS_OperateFunction53_CompellingOrb,
    OBJECTS_OperateFunction54_DiabloSeal,
    OBJECTS_OperateFunction55_DiabloSeal,
    OBJECTS_OperateFunction56_DiabloSeal,
    OBJECTS_OperateFunction57_KhalimChest,
    OBJECTS_OperateFunction58_KhalimChest,
    OBJECTS_OperateFunction59_KhalimChest,
    nullptr,
    OBJECTS_OperateFunction61_HarrogathMainGate,
    OBJECTS_OperateFunction62_63_64_AncientStatue,
    OBJECTS_OperateFunction62_63_64_AncientStatue,
    OBJECTS_OperateFunction62_63_64_AncientStatue,
    OBJECTS_OperateFunction65_AncientsAltar,
    OBJECTS_OperateFunction66_AncientsDoor,
    OBJECTS_OperateFunction67_FrozenAnya,
    OBJECTS_OperateFunction68_EvilUrn,
    OBJECTS_OperateFunction69_InvisibleAncient,
    OBJECTS_OperateFunction70_BaalPortal,
    OBJECTS_OperateFunction71_SummitDoor,
    OBJECTS_OperateFunction72_LastPortal,
    OBJECTS_OperateFunction73_LastLastPortal,
    nullptr,
    nullptr,
};


//D2Game.0x6FC748A0
void __fastcall sub_6FC748A0(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    int32_t nAnimMode = 0;
    if (pObject)
    {
        nAnimMode = pObject->dwAnimMode;
        if (pObject->dwClassId == 399)
        {
            if (!nAnimMode || nAnimMode == 2)
            {
                //UNITS_GetRoom(pObject);
                EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_PERIODICSKILLS, pGame->dwGameFrame + ITEMS_RollRandomNumber(&OBJRGN_GetObjectControlFromGame(pGame)->pSeed) % 250 + 25, 0, 0);
                UNITS_ChangeAnimMode(pObject, 1);
                EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (DATATBLS_GetObjectsTxtRecord(pObject->dwClassId)->dwFrameCnt[1] >> 8) + pGame->dwGameFrame + 1, 0, 0);
            }
            return;
        }
    }

    const int32_t nPeriodOfDay = ENVIRONMENT_GetPeriodOfDayFromAct(pGame->pAct[0], 0);
    if (nPeriodOfDay)
    {
        if (nPeriodOfDay <= 0 || nPeriodOfDay > 3)
        {
            EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_PERIODICSKILLS, pGame->dwGameFrame + 600, 0, 0);
        }
        else
        {
            if (!nAnimMode)
            {
                UNITS_ChangeAnimMode(pObject, 1);
                EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (DATATBLS_GetObjectsTxtRecord(pObject ? pObject->dwClassId : -1)->dwFrameCnt[1] >> 8) + pGame->dwGameFrame + 1, 0, 0);
            }

            EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_PERIODICSKILLS, pGame->dwGameFrame + 1000, 0, 0);
        }
    }
    else
    {
        if (nAnimMode == 2 || nAnimMode == 1)
        {
            UNITS_ChangeAnimMode(pObject, 0);
        }

        EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_PERIODICSKILLS, pGame->dwGameFrame + 1000, 0, 0);
    }
}

//D2Game.0x6FC74A40
void __fastcall sub_6FC74A40(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    const int32_t nSourceLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pObject));

    int32_t nDestinationLevelId = LEVEL_HELL3;
    if (nSourceLevelId == LEVEL_ID_ACT5_BARRICADE_1)
    {
        nDestinationLevelId = LEVEL_HELL1;
    }
    else if (nSourceLevelId == LEVEL_ARREATPLATEAU)
    {
        nDestinationLevelId = LEVEL_HELL2;
    }

    D2UnitStrc* pOtherPortal = D2GAME_CreateLinkPortal_6FD13B20(pGame, pObject, nDestinationLevelId, nSourceLevelId);
    if (pOtherPortal)
    {
        UNITS_SetObjectPortalFlags(pOtherPortal, UNITS_GetObjectPortalFlags(pOtherPortal) | 3);
    }

    DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pObject), 0);
    DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pOtherPortal), 0);
}

//D2Game.0x6FC74AC0
void __fastcall sub_6FC74AC0(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    if (pObject && pObject->dwAnimMode == 1)
    {
        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pObject->dwClassId);
        if (pObjectsTxtRecord->nMode[2])
        {
            pObject->dwAnimMode = 2;
            if (!pObjectsTxtRecord->nHasCollision[2])
            {
                UNITS_FreeCollisionPath(pObject);
            }
        }
    }
}

//D2Game.0x6FC74B00
void __fastcall sub_6FC74B00(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    if (UNITS_IsShrine(pObject))
    {
        UNITROOM_RefreshUnit(pObject);
        if (pObject)
        {
            pObject->dwFlags |= 1;
        }

        UNITS_ChangeAnimMode(pObject, 0);
        pObject->pObjectData->dwOperateGUID = 0;
    }
}

//D2Game.0x6FC74B40
void __fastcall sub_6FC74B40(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    constexpr uint8_t nObjectRegenerateModes = 2;
    const uint8_t nLeft = pObject->pObjectData->InteractType;

    D2ObjectsTxt* pObjectsTxtRecord = UNITS_GetObjectTxtRecordFromObject(pObject);
    if (nLeft / pObjectsTxtRecord->dwParm[2] < nObjectRegenerateModes)
    {
        pObject->pObjectData->InteractType = nLeft + 1;

        const uint8_t nObjectRegenerateMax = 2 * pObjectsTxtRecord->dwParm[2];
        if (nObjectRegenerateMax - nLeft >= 0)
        {
            if ((nLeft + 1) <= nObjectRegenerateMax && !((uint8_t)(nLeft + 1) % (nObjectRegenerateMax / nObjectRegenerateModes)))
            {
                UNITS_ChangeAnimMode(pObject, nObjectRegenerateModes - (uint8_t)(nLeft + 1) / pObjectsTxtRecord->dwParm[2]);
            }

            UNITROOM_RefreshUnit(pObject);
            pObject->dwFlags |= UNITFLAG_DOUPDATE;
        }
        else
        {
            pObject->pObjectData->InteractType = nObjectRegenerateMax;

            if (nObjectRegenerateMax - nLeft < 0)
            {
                FOG_DisplayAssert("ObjectRegenerateMax( ptTypeData ) - bLeft >= 0", __FILE__, __LINE__);
                exit(-1);
            }
        }
    }
    else
    {
        if (nLeft / pObjectsTxtRecord->dwParm[2] >= nObjectRegenerateModes)
        {
            FOG_DisplayAssert("bLeft/ptTypeData->Param[OBJECT_PARAM_WELL_REGEN_PER_MODE] < ObjectRegenerateModes()", __FILE__, __LINE__);
            exit(-1);
        }
    }
}

//D2Game.0x6FC74CA0
void __fastcall sub_6FC74CA0(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    if (pObject && pObject->pHoverText)
    {
        const int32_t nTimeout = CHAT_GetTimeoutFromHoverMsg(pObject->pHoverText);
        if (nTimeout > pGame->dwGameFrame)
        {
            EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_FREEHOVER, nTimeout, 0, 0);
        }
        else
        {
            CHAT_FreeHoverMsg(pGame->pMemoryPool, pObject->pHoverText);
            pObject->pHoverText = nullptr;
            UNITROOM_RefreshUnit(pObject);
            pObject->dwFlags |= 0x100;
        }
    }
}

//D2Game.0x6FC74D10
void __fastcall sub_6FC74D10(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    if (pObject && pObject->dwAnimMode == 1)
    {
        pObject->dwAnimMode = 2;
    }

    D2ObjectControlStrc* pObjectControl = OBJRGN_GetObjectControlFromGame(pGame);

    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pObject ? pObject->dwClassId : -1);

    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pObject, &pCoord);

    for (D2UnitStrc* i = UNITS_GetRoom(pObject)->pUnitFirst; i; i = i->pRoomNext)
    {
        if (i->dwUnitType == UNIT_PLAYER && i->dwAnimMode != PLRMODE_DEAD && UNITS_GetDistanceToOtherUnit(pObject, i) <= (int32_t)(pObjectsTxtRecord->dwParm[0] + 1))
        {
            OBJEVAL_ApplyTrapObjectDamage(pGame, pObject, i, 1);
        }
    }

    EVENT_SetEvent(pGame, pObject, 0, pGame->dwGameFrame + ITEMS_RollRandomNumber(&pObjectControl->pSeed) % 35 + 15, 0, 0);
}

//D2Game.0x6FC74DF0
void __fastcall sub_6FC74DF0(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    void(__fastcall* gpObjectTrapHandlerTable_6FD2FB48[])(D2GameStrc*, D2UnitStrc*) =
    {
        nullptr,
        D2GAME_OBJECTS_TrapHandler1_6FC75C70,
        D2GAME_OBJECTS_TrapHandler2_6_6FC75D00,
        D2GAME_OBJECTS_TrapHandler3_6FC75D90,
        D2GAME_OBJECTS_TrapHandler4_6FC75E20,
        D2GAME_OBJECTS_TrapHandler5_7_6FC75BC0,
        D2GAME_OBJECTS_TrapHandler2_6_6FC75D00,
        D2GAME_OBJECTS_TrapHandler5_7_6FC75BC0,
        D2GAME_OBJECTS_TrapHandler8_9_6FC75AC0,
        D2GAME_OBJECTS_TrapHandler8_9_6FC75AC0
    };

    D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);

    const uint8_t nTrapType = pObject->pObjectData->InteractType & 127;
    if (nTrapType >= std::size(gpObjectTrapHandlerTable_6FD2FB48))
    {
        return;
    }

    if (nTrapType == 8)
    {
        if (nLevelId >= 75)
        {
            if (IsBadCodePtr((FARPROC)gpObjectTrapHandlerTable_6FD2FB48[2]))
            {
                FOG_DisplayAssert("sgObjectTrapHandlers[OBJECT_TRAPPEDFIREBOLT]", __FILE__, __LINE__);
                exit(-1);
            }
            gpObjectTrapHandlerTable_6FD2FB48[2](pGame, pObject);
            return;
        }

        if (IsBadCodePtr((FARPROC)gpObjectTrapHandlerTable_6FD2FB48[nTrapType]))
        {
            FOG_DisplayAssert("sgObjectTrapHandlers[bAttributes]", __FILE__, __LINE__);
            exit(-1);
        }
        gpObjectTrapHandlerTable_6FD2FB48[nTrapType](pGame, pObject);
        return;
    }

    if (nTrapType == 3)
    {
        if (nLevelId < 40 && nLevelId != 25)
        {
            if (IsBadCodePtr((FARPROC)gpObjectTrapHandlerTable_6FD2FB48[2]))
            {
                FOG_DisplayAssert("sgObjectTrapHandlers[OBJECT_TRAPPEDFIREBOLT]", __FILE__, __LINE__);
                exit(-1);
            }
            gpObjectTrapHandlerTable_6FD2FB48[2](pGame, pObject);
            return;
        }

        if (IsBadCodePtr((FARPROC)gpObjectTrapHandlerTable_6FD2FB48[nTrapType]))
        {
            FOG_DisplayAssert("sgObjectTrapHandlers[bAttributes]", __FILE__, __LINE__);
            exit(-1);
        }
        gpObjectTrapHandlerTable_6FD2FB48[nTrapType](pGame, pObject);
        return;
    }

    if (nTrapType != 1 && nTrapType != 4 || nLevelId >= 40)
    {
        if (IsBadCodePtr((FARPROC)gpObjectTrapHandlerTable_6FD2FB48[nTrapType]))
        {
            FOG_DisplayAssert("sgObjectTrapHandlers[bAttributes]", __FILE__, __LINE__);
            exit(-1);
        }
        gpObjectTrapHandlerTable_6FD2FB48[nTrapType](pGame, pObject);
        return;
    }

    if (IsBadCodePtr((FARPROC)gpObjectTrapHandlerTable_6FD2FB48[2]))
    {
        FOG_DisplayAssert("sgObjectTrapHandlers[OBJECT_TRAPPEDFIREBOLT]", __FILE__, __LINE__);
        exit(-1);
    }
    gpObjectTrapHandlerTable_6FD2FB48[2](pGame, pObject);
}

//D2Game.0x6FC74F60
void __fastcall D2GAME_SpikeTraps_6FC74F60(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    // TODO: v8, v10, v11
    int32_t nAnimMode = 0;
    if (pObject)
    {
        nAnimMode = pObject->dwAnimMode;
    }

    D2RoomStrc* pRoom = UNITS_GetRoom(pObject);

    int32_t v10 = 0;
    if (pObject->pObjectData->InteractType != 0)
    {
        if (pObject->pObjectData->InteractType != 1)
        {
            if (pObject->pObjectData->InteractType == 2 && nAnimMode)
            {
                UNITS_ChangeAnimMode(pObject, 0);
                pObject->pObjectData->InteractType = 0;
            }

            EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_STATREGEN, pGame->dwGameFrame + 15, 0, 0);
            return;
        }

        v10 = 1;
    }

    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pObject, &pCoord);

    int32_t v8 = 0;
    int32_t v11 = 0;
    D2UnitStrc* pUnit = pRoom->pUnitFirst;
    if (pUnit)
    {
        do
        {
            if (pUnit->dwUnitType == UNIT_PLAYER && pUnit->dwAnimMode != PLRMODE_DEAD && UNITS_GetDistanceToOtherUnit(pObject, pUnit) <= 0)
            {
                if (v10)
                {
                    OBJEVAL_ApplyTrapObjectDamage(pGame, pObject, pUnit, 0);
                    v8 = 1;
                }
                else
                {
                    SUNIT_AttachSound(pObject, 13, 0);
                    v11 = 1;
                }
            }

            pUnit = pUnit->pRoomNext;
        }
        while (pUnit);

        if (v11)
        {
            pObject->pObjectData->InteractType = 1;
            EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_STATREGEN, pGame->dwGameFrame + 25, 0, 0);
            return;
        }
    }

    if (!v10)
    {
        EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_STATREGEN, pGame->dwGameFrame + 15, 0, 0);
        return;
    }

    if (v8 == 1)
    {
        pObject->pObjectData->InteractType = 2;
        UNITS_ChangeAnimMode(pObject, 1);
    }
    else
    {
        pObject->pObjectData->InteractType = 0;
    }

    EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_STATREGEN, (DATATBLS_GetObjectsTxtRecord(pObject->dwClassId)->dwFrameCnt[1] >> 8) + pGame->dwGameFrame + 1, 0, 0);
}

//D2Game.0x6FC750D0
void __fastcall sub_6FC750D0(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    if (!pObject)
    {
        return;
    }

    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pObject->dwClassId);
    switch (pObject->dwAnimMode)
    {
    case 1u:
        EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_PERIODICSTATS, pGame->dwGameFrame + 10, 0, 0);
        break;

    case 2u:
        ++pObject->pObjectData->InteractType;

        if (pObject->pObjectData->InteractType <= 1u)
        {
            //UNITS_GetRoom(pObject);
            //OBJRGN_GetObjectControlFromGame(pGame);

            D2RoomStrc* pRoom = UNITS_GetRoom(pObject);

            D2CoordStrc pCoord = {};
            UNITS_GetCoords(pObject, &pCoord);

            for (D2UnitStrc* i = pRoom->pUnitFirst; i; i = i->pRoomNext)
            {
                if (i->dwUnitType == UNIT_PLAYER && i->dwAnimMode != PLRMODE_DEAD && UNITS_GetDistanceToOtherUnit(pObject, i) <= pObjectsTxtRecord->dwParm[0] + 1)
                {
                    OBJEVAL_ApplyTrapObjectDamage(pGame, pObject, i, 1);
                }
            }

            EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_PERIODICSTATS, pGame->dwGameFrame + 25, 0, 0);
        }
        else
        {
            D2ObjOperateFnStrc pOp = {};
            pOp.pGame = pGame;
            pOp.pObject = pObject;
            pOp.pPlayer = nullptr;
            //UNITS_GetRoom(pObject);
            pOp.pObjectregion = OBJRGN_GetObjectControlFromGame(pGame);
            pOp.nObjectIdx = pObject->dwClassId;

            if (OBJMODE_DropFromChestTCWithQuality(&pOp, 0))
            {
                UNITS_ChangeAnimMode(pObject, 3);
                EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_PERIODICSTATS, (pObjectsTxtRecord->dwFrameCnt[3] >> 8) + pGame->dwGameFrame + 1, 0, 0);
            }
        }
        break;

    case 3u:
        UNITS_ChangeAnimMode(pObject, 4);
        break;
    }
}

//D2Game.0x6FC75250
void __fastcall D2GAME_OBJMODE_InvokeEventFunction_6FC75250(D2GameStrc* pGame, D2UnitStrc* pObject, int32_t nEventType)
{
    void(__fastcall* gpEventFunctions_6FD28CD8[])(D2GameStrc*, D2UnitStrc*) =
    {
        sub_6FC74D10,
        sub_6FC74AC0,
        sub_6FC74B40,
        D2GAME_SpikeTraps_6FC74F60,
        sub_6FC74DF0,
        sub_6FC74B00,
        sub_6FC74CA0,
        QUESTS_ObjectEvents,
        sub_6FC748A0,
        sub_6FC750D0,
        sub_6FC78340,
        sub_6FC74A40,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };

    if (IsBadCodePtr((FARPROC)gpEventFunctions_6FD28CD8[nEventType]))
    {
        FOG_DisplayAssert("scpfnEventFunctions[eEventType]", __FILE__, __LINE__);
        exit(-1);
    }

    gpEventFunctions_6FD28CD8[nEventType](pGame, pObject);
}

//D2Game.0x6FC752A0
void __fastcall sub_6FC752A0(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    int32_t bTargetable = 0;
    int32_t nObjectGUID = -1;

    if (pUnit)
    {
        nObjectGUID = pUnit->dwUnitId;
        bTargetable = pUnit->dwFlags & UNITFLAG_TARGETABLE;
    }

    D2GAME_PACKETS_SendPacket0x0E_6FC3C8E0(pClient, 0xEu, 2, nObjectGUID, 3, bTargetable, (uint16_t)pUnit->dwAnimMode);

    const uint32_t nSubClass = DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId)->nSubClass;
    if (nSubClass)
    {
        if (nSubClass & 4)
        {
            sub_6FC3F550(pClient, pUnit);
        }
        else if (pUnit->dwAnimMode == 1 && nSubClass & 1)
        {
            D2ShrinesTxt* pShrinesTxtRecord = UNITS_GetShrineTxtRecordFromObject(pUnit);

            int32_t nCode = 0;
            if (pShrinesTxtRecord)
            {
                nCode = pShrinesTxtRecord->nCode;
            }

            int32_t nOperateGUID = pUnit->pObjectData->dwOperateGUID;
            if (nOperateGUID)
            {
                sub_6FC3F060(pClient, 2u, pUnit->dwUnitId, 0, 0, nOperateGUID - 1, 0, nCode, 0);
            }
        }
    }
}

//D2Game.0x6FC75350
void __fastcall sub_6FC75350(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    if (pUnit)
    {
        if (pUnit->dwFlags & 1)
        {
            sub_6FC752A0(pUnit, pClient);
        }

        if ((pUnit->dwFlags >> 10) & 1)
        {
            D2GAME_UpdateUnit_6FCC6080(pUnit, pClient);
        }

        if ((pUnit->dwFlags >> 8) & 1)
        {
            sub_6FCC5FA0(pUnit, pClient);
        }

        if (pUnit->dwFlagEx & 1)
        {
            D2GAME_INVMODE_First_6FC40FB0(pGame, pUnit, pClient, 0);
        }
    }

    sub_6FCC6540(pUnit, pClient);
}

//D2Game.0x6FC753E0
int32_t __fastcall OBJECTS_OperateFunction27_TeleportPad(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    // TODO: Refactor RoomList part
    if (!pOp || !pOp->pObject || !pOp->pPlayer)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;
    D2UnitStrc* pPlayer = pOp->pPlayer;

    D2RoomStrc* pRoom = UNITS_GetRoom(pObject);

    if (pRoom)
    {
        const int32_t nUnitId = pObject->dwClassId;

        D2UnitStrc* pUnit = pRoom->pUnitFirst;
        while (pUnit && (pUnit->dwUnitType != UNIT_OBJECT || pUnit == pObject || pUnit->dwClassId != nUnitId))
        {
            pUnit = pUnit->pRoomNext;
        }

        if (!pUnit)
        {
            D2RoomStrc** ppRoomList = nullptr;
            int32_t nNumRooms = 0;
            DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);
            if (nNumRooms > 0)
            {
                int32_t nCounter = 0;
                while (1)
                {
                    D2RoomStrc* pTemp = ppRoomList[nCounter];
                    if (pTemp != pRoom)
                    {
                        pUnit = pTemp->pUnitFirst;
                        while (pUnit && (pUnit->dwUnitType != UNIT_OBJECT || pUnit == pObject || pUnit->dwClassId != nUnitId))
                        {
                            pUnit = pUnit->pRoomNext;
                        }

                        if (pUnit)
                        {
                            pRoom = ppRoomList[nCounter];
                            break;
                        }
                    }
                    ++nCounter;
                    if (nCounter >= nNumRooms)
                    {
                        return 0;
                    }
                }
            }
        }

        D2CoordStrc pCoord = {};
        UNITS_GetCoords(pUnit, &pCoord);
        if (COLLISION_GetFreeCoordinates(pRoom, &pCoord, 3, 0x1C09, 0) && pPlayer && sub_6FCBDFE0(pPlayer->pGame, pPlayer, pRoom, pCoord.nX, pCoord.nY, 0, 0))
        {
            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);

            D2GSPacketSrv07 packet07 = {};
            packet07.nHeader = 0x07;
            packet07.nX = (WORD)pRoom->nTileXPos;
            packet07.nY = (WORD)pRoom->nTileYPos;
            packet07.nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet07, sizeof(D2GSPacketSrv07));

            UNITROOM_RefreshUnit(pPlayer);
            pPlayer->dwFlagEx |= UNITFLAGEX_TELEPORTED;
        }
    }

    return 0;
}

//D2Game.0x6FC75530
int32_t __fastcall OBJECTS_OperateFunction30_ExplodingChest(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        OBJEVAL_ApplyTrapObjectDamage(pOp->pGame, pObject, pOp->pPlayer, 0);
        OBJEVAL_ApplyTrapObjectDamage(pOp->pGame, pObject, pOp->pPlayer, 1);

        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
    }

    return 1;
}

//D2Game.0x6FC755A0
int32_t __fastcall OBJECTS_OperateFunction08_Door(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pObject, &pCoord);
    
    const int32_t nSizeX = UNITS_GetUnitSizeX(pObject);
    const int32_t nSizeY = UNITS_GetUnitSizeY(pObject);
    const uint32_t nTickCount = GetTickCount();

    const int32_t nAnimMode = pObject->dwAnimMode;

    if (nTickCount < pObject->dwTickCount + 500)
    {
        return 1;
    }
    else
    {
        switch (nAnimMode)
        {
        case OBJMODE_SPECIAL4:
            if (D2GAME_DoKeyCheck_6FC4A4B0(pOp->pGame, pOp->pPlayer))
            {
                UNITS_FreeCollisionPath(pObject);
                UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
                pObject->dwTickCount = nTickCount;
                return 1;
            }

            SUNIT_AttachSound(pOp->pPlayer, 0x16, pOp->pPlayer);
            return 1;

        case OBJMODE_NEUTRAL:
            UNITS_FreeCollisionPath(pObject);
            UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
            pObject->dwTickCount = nTickCount;
            return 1;

        case OBJMODE_OPENED:
        case OBJMODE_SPECIAL3:
            if (!COLLISION_CheckMaskWithSizeXY(pRoom, pCoord.nX, pCoord.nY, nSizeX, nSizeY, COLLIDE_PLAYER | COLLIDE_MONSTER | COLLIDE_CORPSE))
            {
                UNITS_BlockCollisionPath(pObject, pRoom, pCoord.nX, pCoord.nY);
                UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
                pObject->dwTickCount = nTickCount;
                return 1;
            }

            if (COLLISION_CheckMaskWithSizeXY(pRoom, pCoord.nX, pCoord.nY, nSizeX, nSizeY, COLLIDE_CORPSE))
            {
                UNITS_ChangeAnimMode(pObject, OBJMODE_SPECIAL2);
                pObject->dwTickCount = nTickCount;
                return 1;
            }

            if (nAnimMode == OBJMODE_SPECIAL3)
            {
                return 1;
            }

            UNITS_ChangeAnimMode(pObject, OBJMODE_SPECIAL3);
            pObject->dwTickCount = nTickCount;
            return 1;

        default:
            return 1;
        }
    }

    return 1;
}

//D2Game.0x6FC75730
int32_t __fastcall OBJECTS_OperateFunction16_TrapDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
        return 1;
    }

    if (pObject->dwAnimMode != OBJMODE_OPENED)
    {
        return 1;
    }

    D2UnitStrc* pTile = UNITS_GetRoom(pObject)->pUnitFirst;
    D2_ASSERT(pTile);

    while (pTile->dwUnitType != UNIT_TILE)
    {
        pTile = pTile->pRoomNext;
        D2_ASSERT(pTile);
    }

    SUNIT_WarpPlayer(pOp->pGame, pOp->pPlayer, pTile);

    return 1;
}

//D2Game.0x6FC757E0
int32_t __fastcall OBJECTS_OperateFunction47_Stair(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;
    D2RoomStrc* pRoom = UNITS_GetRoom(pObject);

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
    }
    else if (pObject->dwAnimMode == OBJMODE_OPENED)
    {
        for (D2UnitStrc* pTile = pRoom->pUnitFirst; pTile; pTile = pTile->pRoomNext)
        {
            if (pTile->dwUnitType == UNIT_TILE)
            {
                SUNIT_WarpPlayer(pOp->pGame, pOp->pPlayer, pTile);
                return 1;
            }
        }

        D2RoomStrc** pRoomList = nullptr;
        int32_t nNumRooms = 0;
        DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &pRoomList, &nNumRooms);

        for (int32_t i = 0; i < nNumRooms; ++i)
        {
            if (pRoomList[i] != pRoom)
            {
                for (D2UnitStrc* pTile = pRoomList[i]->pUnitFirst; pTile; pTile = pTile->pRoomNext)
                {
                    if (pTile->dwUnitType == UNIT_TILE)
                    {
                        SUNIT_WarpPlayer(pOp->pGame, pOp->pPlayer, pTile);
                        return 1;
                    }
                }
            }
        }
    }

    return 1;
}

//D2Game.0x6FC758F0
int32_t __fastcall OBJECTS_OperateFunction61_HarrogathMainGate(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    D2RoomStrc* pRoom = UNITS_GetRoom(pObject);

    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pObject, &pCoord);

    const uint32_t nTickCount = GetTickCount();

    if (nTickCount >= pObject->dwTickCount + 500)
    {
        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
        {
            UNITS_FreeCollisionPath(pObject);

            UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
            pObject->dwTickCount = nTickCount;
            EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
        }
        else if (pObject->dwAnimMode == OBJMODE_OPENED)
        {
            UNITS_BlockCollisionPath(pObject, pRoom, pCoord.nX, pCoord.nY);
            UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
            pObject->dwTickCount = nTickCount;
        }
    }

    return 1;
}

//D2Game.0x6FC759F0
int32_t __fastcall OBJECTS_OperateFunction50_Stair(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (pOp && pOp->pObject && pOp->pObject->dwAnimMode == OBJMODE_OPENED)
    {
        return OBJECTS_OperateFunction47_Stair(pOp, nOperate);
    }

    return 0;
}

//D2Game.0x6FC75A10
int32_t __fastcall OBJECTS_OperateFunction29_SlimeDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
        UNITS_FreeCollisionPath(pObject);

        if (!DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx)->nSelectable[pObject->dwAnimMode])
        {
            pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);
        }
        else
        {
            pObject->dwFlags |= UNITFLAG_TARGETABLE;
        }

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);

        if (pObjectsTxtRecord->nMode[2])
        {
            EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
        }
    }

    return 1;
}

//D2Game.0x6FC75AC0
void __fastcall D2GAME_OBJECTS_TrapHandler8_9_6FC75AC0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2ObjOperateFnStrc op = {};
    
    op.pObject = pUnit;
    op.pObjectregion = OBJRGN_GetObjectControlFromGame(pGame);
    op.pGame = pGame;

    const int32_t nMonsters = (ITEMS_RollRandomNumber(&op.pObjectregion->pSeed) & 1) + 1;

    const int32_t nMonsterId = OBJRGN_GetTrapMonsterId(&op);
    if (nMonsterId >= 0 && nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        for (int32_t i = 0; i < nMonsters; ++i)
        {
            D2GAME_SpawnTrapMonster_6FC75B40(&op, nMonsterId, 8);
        }
    }
}

//D2Game.0x6FC75B40
void __fastcall D2GAME_SpawnTrapMonster_6FC75B40(D2ObjOperateFnStrc* pOp, int32_t nMonsterId, int32_t a3)
{
    if (nMonsterId >= 0 && nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(pOp->pObject);
        if (nMonsterId != 234 || !pRoom || DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom)) != ACT_I)
        {
            if (!sub_6FC6A150(pOp->pGame, pOp->pObject, nMonsterId, a3, -1, 0) && !sub_6FC6A150(pOp->pGame, pOp->pObject, nMonsterId, a3, 1, 0))
            {
                sub_6FC6A150(pOp->pGame, pOp->pObject, nMonsterId, a3, 3, 0);
            }
        }
    }
}

//D2Game.0x6FC75BC0
void __fastcall D2GAME_OBJECTS_TrapHandler5_7_6FC75BC0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pUnit, &pCoord);
    
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    D2UnitStrc* pFireLarge = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_FIRE_LARGE, pCoord.nX, pCoord.nY, pGame, pRoom, 1, 1, 0);
    if (pFireLarge)
    {
        D2GAME_SetSparkChest_6FCBD7C0(pFireLarge, 2);
    }

    D2DrlgCoordsStrc drlgCoords = {};
    DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);

    const int32_t nX = pCoord.nX + 1;
    if (nX >= drlgCoords.dwSubtilesLeft && nX < drlgCoords.dwSubtilesLeft + drlgCoords.dwSubtilesWidth)
    {
        D2UnitStrc* pFireSmall = SUNIT_AllocUnitData(UNIT_OBJECT, OBJECT_FIRE_SMALL, nX, pCoord.nY, pGame, pRoom, 1, 1, 0);
        D2GAME_SetSparkChest_6FCBD7C0(pFireSmall, 2);
    }
}

//D2Game.0x6FC75C70
void __fastcall D2GAME_OBJECTS_TrapHandler1_6FC75C70(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pUnit, &pCoord);

    if (!sub_6FC6A090(pGame, UNITS_GetRoom(pUnit), pCoord.nX, pCoord.nY, MONSTER_TRAP_LIGHTNING, 1, 136))
    {
        D2RoomStrc* ppRoom = nullptr;
        D2Common_10136(UNITS_GetRoom(pUnit), &pCoord, 0, 0x3F11u, &ppRoom);

        if (ppRoom)
        {
            sub_6FC69F10(pGame, ppRoom, pCoord.nX, pCoord.nY, MONSTER_TRAP_LIGHTNING, 1, 3, 8);
        }
    }
}

//D2Game.0x6FC75D00
void __fastcall D2GAME_OBJECTS_TrapHandler2_6_6FC75D00(D2GameStrc* pGame, D2UnitStrc* pUnit)
{    
    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pUnit, &pCoord);

    if (!sub_6FC6A090(pGame, UNITS_GetRoom(pUnit), pCoord.nX, pCoord.nY, MONSTER_TRAP_FIREBOLT, 1, 136))
    {
        D2RoomStrc* ppRoom = nullptr;
        D2Common_10136(UNITS_GetRoom(pUnit), &pCoord, 0, 0x3F11u, &ppRoom);

        if (ppRoom)
        {
            sub_6FC69F10(pGame, ppRoom, pCoord.nX, pCoord.nY, MONSTER_TRAP_FIREBOLT, 1, 3, 8);
        }
    }
}

//D2Game.0x6FC75D90
void __fastcall D2GAME_OBJECTS_TrapHandler3_6FC75D90(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pUnit, &pCoord);
   
    if (!sub_6FC6A090(pGame, UNITS_GetRoom(pUnit), pCoord.nX, pCoord.nY, MONSTER_TRAP_POISONCLOUD, 1, 136))
    {
        D2RoomStrc* ppRoom = nullptr;
        D2Common_10136(UNITS_GetRoom(pUnit), &pCoord, 0, 0x3F11u, &ppRoom);

        if (ppRoom)
        {
            sub_6FC69F10(pGame, ppRoom, pCoord.nX, pCoord.nY, MONSTER_TRAP_POISONCLOUD, 1, 3, 8);
        }
    }
}

//D2Game.0x6FC75E20
void __fastcall D2GAME_OBJECTS_TrapHandler4_6FC75E20(D2GameStrc* pGame, D2UnitStrc* pUnit)
{    
    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pUnit, &pCoord);

    if (!sub_6FC6A090(pGame, UNITS_GetRoom(pUnit), pCoord.nX, pCoord.nY, MONSTER_TRAP_NOVA, 1, 136))
    {
        D2RoomStrc* ppRoom = nullptr;
        D2Common_10136(UNITS_GetRoom(pUnit), &pCoord, 0, 0x3F11u, &ppRoom);
        
        if (ppRoom)
        {
            sub_6FC69F10(pGame, ppRoom, pCoord.nX, pCoord.nY, MONSTER_TRAP_NOVA, 1, 3, 8);
        }
    }
}

//D2Game.0x6FC75EB0
void __fastcall sub_6FC75EB0(D2ObjOperateFnStrc* pOp)
{
    if (!pOp->pObject || pOp->pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);

        D2CoordStrc pCoord = {};
        UNITS_GetCoords(pOp->pObject, &pCoord);

        OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
        OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
        OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);

        uint32_t dwCode = ITEMS_GetHealthPotionDropCode(pOp->pObject);
        D2_ASSERT(dwCode);

        pOp->pObject->dwDropItemCode = dwCode;

        int32_t nItemLevel = 0;
        D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pOp->pObject, 2, &nItemLevel, 0, -1, 1);
        D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pOp->pObject, 2, &nItemLevel, 0, -1, 1);

        dwCode = ITEMS_GetManaPotionDropCode(pOp->pObject);
        D2_ASSERT(dwCode);

        pOp->pObject->dwDropItemCode = dwCode;

        D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pOp->pObject, 2, &nItemLevel, 0, -1, 1);
        D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pOp->pObject, 2, &nItemLevel, 0, -1, 1);
        
        if (pObjectsTxtRecord->nMode[1])
        {
            UNITS_ChangeAnimMode(pOp->pObject, 1);
            EVENT_SetEvent(pOp->pGame, pOp->pObject, 1, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
        }
        else
        {
            UNITS_ChangeAnimMode(pOp->pObject, 2);
        }

        if (pOp->pObject)
        {
            pOp->pObject->dwFlags &= 0xFFFFFFFD;
        }
    }
}

//Inlined in OBJECTS_OperateFunction04_Chest
__forceinline void __fastcall OBJECTS_ChestEnd(D2ObjOperateFnStrc* pOp, int32_t nType)
{
    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);

    if (pObjectsTxtRecord->nMode[1])
    {
        UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPERATING);
        EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
    }
    else
    {
        UNITS_ChangeAnimMode(pOp->pObject, OBJMODE_OPENED);
    }

    pOp->pObject->dwFlags &= ~UNITFLAG_TARGETABLE;

    if (pOp->pObject->dwDropItemCode)
    {
        int32_t nItemLevel = 0;
        D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pOp->pObject, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 1);
    }

     D2GAME_SetTrapCallback_6FC764B0(pOp, nType & 0x7F);
}

//D2Game.0x6FC76030
int32_t __fastcall OBJECTS_OperateFunction04_Chest(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    // TODO: Refactor int32_to smaller functions

    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;
    const uint8_t nType = pObject->pObjectData->InteractType;

    int32_t nDrops = 1;
    if (nType & 0x80)
    {
        D2UnitStrc* pPlayer = pOp->pPlayer;
        if ((!pPlayer || pPlayer->dwClassId != PCLASS_ASSASSIN) && !D2GAME_DoKeyCheck_6FC4A4B0(pOp->pGame, pPlayer))
        {
            SUNIT_AttachSound(pPlayer, 22, pPlayer);
            return 1;
        }

        SUNIT_AttachSound(pPlayer, 11, pPlayer);

        D2GAME_UpdateUnit_6FCC6080(pPlayer, SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__));

        nDrops = 2;
    }

    bool bSparkChest = false;
    int32_t nItemQuality = 0;
    if (pObject->bSparkChest & 1)
    {
        bSparkChest = true;
        nItemQuality = (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 100) < 5 ? ITEMQUAL_RARE : ITEMQUAL_MAGIC;
    }

    D2UnitStrc* pItem = nullptr;
    int32_t nMagicItemsDropped = 0;
    int32_t nItemsDropped = 0;
    int32_t nItemLevel = 0;
    int32_t nCounter = 0;
    int32_t nRand = 0;

    if (pObject->dwClassId != 397)
    {
        if ((ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 100) >= 25 || bSparkChest || nType & 0x80)
        {
            nCounter = 0;
            while (nDrops)
            {
                pItem = OBJMODE_DropFromChestTCWithQuality(pOp, nItemQuality);
                if (pItem && pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwQualityNo >= ITEMQUAL_MAGIC && pItem->pItemData->dwQualityNo <= ITEMQUAL_TEMPERED)
                {
                    ++nCounter;
                }

                --nDrops;
            }

            if (bSparkChest && !nCounter)
            {
                do
                {
                    pItem = OBJMODE_DropFromChestTCWithQuality(pOp, nItemQuality);
                    if (pItem && pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwQualityNo >= ITEMQUAL_MAGIC && pItem->pItemData->dwQualityNo <= ITEMQUAL_TEMPERED)
                    {
                        break;
                    }
                    ++nCounter;
                }
                while (nCounter < 10);
            }
        }

        OBJECTS_ChestEnd(pOp, nType);
    }
    else
    {
        nRand = ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 10000;

        if (nRand < 200)
        {
            nDrops = 2;

            do
            {
                pItem = OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_UNIQUE);
                if (!pItem)
                {
                    break;
                }

                if (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwQualityNo >= ITEMQUAL_MAGIC && pItem->pItemData->dwQualityNo <= ITEMQUAL_TEMPERED)
                {
                    OBJECTS_ChestEnd(pOp, nType);
                    return 1;
                }

                --nDrops;
            }
            while (nDrops);
        }
        else if (nRand < 600)
        {
            nDrops = 2;

            do
            {
                pItem = OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_SET);
                if (!pItem)
                {
                    break;
                }

                if (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwQualityNo >= ITEMQUAL_MAGIC && pItem->pItemData->dwQualityNo <= ITEMQUAL_TEMPERED)
                {
                    OBJECTS_ChestEnd(pOp, nType);
                    return 1;
                }

                --nDrops;
            }
            while (nDrops);
        }
        else if (nRand < 1200)
        {
            nDrops = 2;

            do
            {
                pItem = OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_RARE);
                if (!pItem)
                {
                    break;
                }

                if (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwQualityNo >= ITEMQUAL_MAGIC && pItem->pItemData->dwQualityNo <= ITEMQUAL_TEMPERED)
                {
                    OBJECTS_ChestEnd(pOp, nType);
                    return 1;
                }

                --nDrops;
            }
            while (nDrops);
        }
        else if (nRand < 3200)
        {
            nCounter = 0;
            do
            {
                pItem = OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
                if (pItem)
                {
                    if (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwQualityNo >= ITEMQUAL_MAGIC && pItem->pItemData->dwQualityNo <= ITEMQUAL_TEMPERED)
                    {
                        ++nMagicItemsDropped;
                    }

                    ++nItemsDropped;
                }

                if (nMagicItemsDropped >= 3)
                {
                    break;
                }

                ++nCounter;
            }
            while (nCounter < 10);

            if (nItemsDropped)
            {
                OBJECTS_ChestEnd(pOp, nType);
                return 1;
            }
        }
        else if (nRand < 6200)
        {
            nItemsDropped = 0;
            nCounter = 0;
            do
            {
                pItem = OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
                if (pItem)
                {
                    if (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwQualityNo >= ITEMQUAL_MAGIC && pItem->pItemData->dwQualityNo <= ITEMQUAL_TEMPERED)
                    {
                        ++nMagicItemsDropped;
                    }
                    else
                    {
                        ++nItemsDropped;
                    }
                }

                if (nMagicItemsDropped >= 2)
                {
                    break;
                }

                ++nCounter;
            }
            while (nCounter < 10);

            if (nItemsDropped)
            {
                if (nItemsDropped >= 7)
                {
                    OBJECTS_ChestEnd(pOp, nType);
                    return 1;
                }
            }
            else if (OBJMODE_DropFromChestTCWithQuality(pOp, 0))
            {
                nItemsDropped = 1;
            }

            if (7 - nItemsDropped > 0)
            {
                nDrops = 7 - nItemsDropped;
                do
                {
                    OBJMODE_DropItemWithCodeAndQuality(pOp->pGame, pObject, ' dlg', 0);
                    --nDrops;
                }
                while (nDrops);
            }

            OBJECTS_ChestEnd(pOp, nType);

            return 1;
        }

        nDrops = nItemsDropped;

        nCounter = 0;
        do
        {
            pItem = OBJMODE_DropFromChestTCWithQuality(pOp, ITEMQUAL_MAGIC);
            if (pItem)
            {
                if (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwQualityNo >= ITEMQUAL_MAGIC && pItem->pItemData->dwQualityNo <= ITEMQUAL_TEMPERED)
                {
                    break;
                }

                ++nDrops;
            }

            ++nCounter;
        }
        while (nCounter < 10);

        if (nDrops < 4 && 4 - nDrops > 0)
        {
            nDrops = 4 - nDrops;
            do
            {
                OBJMODE_DropFromChestTCWithQuality(pOp, 0);
                --nDrops;
            }
            while (nDrops);
        }

        nDrops = 5;
        do
        {
            OBJMODE_DropItemWithCodeAndQuality(pOp->pGame, pObject, ' dlg', 0);
            --nDrops;
        }
        while (nDrops);

        nDrops = 2;
        do
        {
            OBJMODE_DropItemWithCodeAndQuality(pOp->pGame, pObject, ' 3ph', 0);
            --nDrops;
        }
        while (nDrops);

        nDrops = 2;
        do
        {
            OBJMODE_DropItemWithCodeAndQuality(pOp->pGame, pObject, ' 3pm', 0);
            --nDrops;
        }
        while (nDrops);

        OBJECTS_ChestEnd(pOp, nType);
    }

    return 1;
}

//D2Game.0x6FC764B0
void __fastcall  D2GAME_SetTrapCallback_6FC764B0(D2ObjOperateFnStrc* pOp, uint8_t nTrapType)
{
    using TrapFunc = void(__fastcall*)(D2GameStrc*, D2UnitStrc*);
    constexpr TrapFunc gpObjectTrapHandlerTable_6FD2FB48[] =
    {
        nullptr,
        D2GAME_OBJECTS_TrapHandler1_6FC75C70,
        D2GAME_OBJECTS_TrapHandler2_6_6FC75D00,
        D2GAME_OBJECTS_TrapHandler3_6FC75D90,
        D2GAME_OBJECTS_TrapHandler4_6FC75E20,
        D2GAME_OBJECTS_TrapHandler5_7_6FC75BC0,
        D2GAME_OBJECTS_TrapHandler2_6_6FC75D00,
        D2GAME_OBJECTS_TrapHandler5_7_6FC75BC0,
        D2GAME_OBJECTS_TrapHandler8_9_6FC75AC0,
        D2GAME_OBJECTS_TrapHandler8_9_6FC75AC0
    };

    if (nTrapType < std::size(gpObjectTrapHandlerTable_6FD2FB48) && gpObjectTrapHandlerTable_6FD2FB48[nTrapType])
    {
        if (IsBadCodePtr((FARPROC)gpObjectTrapHandlerTable_6FD2FB48[nTrapType]))
        {
            FOG_DisplayAssert("sgObjectTrapHandlers[bTrapType]", __FILE__, __LINE__);
            exit(-1);
        }

        if (gpObjectTrapHandlerTable_6FD2FB48[nTrapType] != D2GAME_OBJECTS_TrapHandler8_9_6FC75AC0)
        {
            EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_TRAP, pOp->pGame->dwGameFrame + 35, 0, 0);
            SUNIT_AttachSound(pOp->pObject, 13, 0);
            return;
        }

        const int32_t nMonsterId = OBJRGN_GetTrapMonsterId(pOp);
        D2RoomStrc* pRoom = UNITS_GetRoom(pOp->pObject);
        if (nMonsterId != MONSTER_FLYINGSCIMITAR || !pRoom || DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom)) != ACT_I)
        {
            EVENT_SetEvent(pOp->pGame, pOp->pObject, UNITEVENTCALLBACK_TRAP, pOp->pGame->dwGameFrame + 35, 0, 0);
            SUNIT_AttachSound(pOp->pObject, 13, 0);
        }
    }
}

//D2Game.0x6FC76570
int32_t __fastcall OBJECTS_OperateFunction17_Obelisk(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    D2_MAYBE_UNUSED(nOperate);
    if (!pOp || !pOp->pObject || !pOp->pPlayer)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;
    D2UnitStrc* pPlayer = pOp->pPlayer;

    D2UnitStrc* pInteractUnit = SUNIT_GetInteractUnit(pOp->pGame, pPlayer);

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        if (PLAYER_IsBusy(pPlayer) != 1)
        {
            D2_ASSERT(pPlayer->pInventory);

            D2UnitStrc* pItem = INVENTORY_GetFirstItem(pPlayer->pInventory);
            if (!pItem)
            {
                SUNIT_AttachSound(pPlayer, 19, pPlayer);
                return 1;
            }

            while (INVENTORY_UnitIsItem(pItem) && !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_GEM))
            {
                pItem = INVENTORY_GetNextItem(pItem);
                if (!pItem)
                {
                    SUNIT_AttachSound(pPlayer, 19, pPlayer);
                    return 1;
                }
            }

            const int32_t nObjectGUID = pObject->dwUnitId;

            SUNIT_SetInteractInfo(pPlayer, UNIT_OBJECT, nObjectGUID);
            UNITS_ChangeAnimMode(pObject, OBJMODE_SPECIAL1);

            D2GSPacketSrv58 packet58 = {};
            packet58.nHeader = 0x58;
            packet58.nUnitId = nObjectGUID;
            //packet58.unk0x05 = 0;
            D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), &packet58, sizeof(packet58));
        }
    }
    else if (pObject->dwAnimMode == OBJMODE_SPECIAL1)
    {
        if (pInteractUnit == pObject)
        {
            SUNIT_ResetInteractInfo(pOp->pPlayer);
            UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
        }
    }

    return 1;
}

//D2Game.0x6FC766B0
void __fastcall D2GAME_SHRINES_Health_6FC766B0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2_MAYBE_UNUSED(pShrinesTxtRecord);
    const int32_t nDiff = STATLIST_GetMaxLifeFromUnit(pOp->pPlayer) - STATLIST_GetUnitStatUnsigned(pOp->pPlayer, STAT_HITPOINTS, 0);
    if (nDiff)
    {
        STATLIST_AddUnitStat(pOp->pPlayer, STAT_HITPOINTS, nDiff, 0);
    }
}

//D2Game.0x6FC766F0
void __fastcall D2GAME_SHRINES_FillMana_6FC766F0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2_MAYBE_UNUSED(pShrinesTxtRecord);
    const int32_t nDiff = STATLIST_GetMaxManaFromUnit(pOp->pPlayer) - STATLIST_GetUnitStatUnsigned(pOp->pPlayer, STAT_MANA, 0);
    if (nDiff)
    {
        STATLIST_AddUnitStat(pOp->pPlayer, STAT_MANA, nDiff, 0);
    }
}

//D2Game.0x6FC76730
void __fastcall D2GAME_SHRINES_Refill_6FC76730(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2_MAYBE_UNUSED(pShrinesTxtRecord);
    const int32_t nHpDiff = STATLIST_GetMaxLifeFromUnit(pOp->pPlayer) - STATLIST_GetUnitStatUnsigned(pOp->pPlayer, STAT_HITPOINTS, 0);
    if (nHpDiff)
    {
        STATLIST_AddUnitStat(pOp->pPlayer, STAT_HITPOINTS, nHpDiff, 0);
    }

    const int32_t nManaDiff = STATLIST_GetMaxManaFromUnit(pOp->pPlayer) - STATLIST_GetUnitStatUnsigned(pOp->pPlayer, STAT_MANA, 0);
    if (nManaDiff)
    {
        STATLIST_AddUnitStat(pOp->pPlayer, STAT_MANA, nManaDiff, 0);
    }
}

//D2Game.0x6FC76790
void __fastcall D2GAME_SHRINES_ExchangeHealth_6FC76790(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    const int32_t nHpPct = ((int32_t)STATLIST_GetUnitStatUnsigned(pOp->pPlayer, STAT_HITPOINTS, 0) >> 8) * pShrinesTxtRecord->dwArg0;
    const int32_t nValue = nHpPct / 100 << 8;
    STATLIST_AddUnitStat(pOp->pPlayer, STAT_HITPOINTS, -nValue, 0);
    STATLIST_AddUnitStat(pOp->pPlayer, STAT_MANA, nValue * pShrinesTxtRecord->dwArg1 / 100, 0);
}

//D2Game.0x6FC767F0
void __fastcall D2GAME_SHRINES_ExchangeMana_6FC767F0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    const int32_t nValue = STATLIST_GetUnitStatUnsigned(pOp->pPlayer, STAT_MANA, 0) * pShrinesTxtRecord->dwArg0 / 100;
    STATLIST_AddUnitStat(pOp->pPlayer, STAT_MANA, -nValue, 0);
    STATLIST_AddUnitStat(pOp->pPlayer, STAT_HITPOINTS, nValue * pShrinesTxtRecord->dwArg1 / 100, 0);
}

//D2Game.0x6FC76850
void __fastcall D2GAME_SHRINES_Enirhs_6FC76850(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2_MAYBE_UNUSED(pShrinesTxtRecord);
    char* szName = UNITS_GetPlayerData(pOp->pPlayer)->szName;
    if (szName)
    {
        strncpy(szName, _strrev(szName), 16);
    }
}

//D2Game.0x6FC76880
void __fastcall D2GAME_SHRINES_Portal_6FC76880(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2_MAYBE_UNUSED(pShrinesTxtRecord);
    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pOp->pPlayer, &pCoord);

    D2RoomStrc* pRoom = UNITS_GetRoom(pOp->pPlayer);
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    const int32_t nTownId = DUNGEON_GetTownLevelIdFromActNo(DRLG_GetActNoFromLevelId(nLevelId));

    if (COLLISION_GetFreeCoordinates(pRoom, &pCoord, 3, 0x1C09u, 0))
    {
        D2GAME_CreatePortalObject_6FD13DF0(pOp->pGame, pOp->pPlayer, pRoom, pCoord.nX + 5, pCoord.nY + 5, nTownId, 0, 59u, 0);
    }
}

//D2Game.0x6FC76910
void __fastcall D2GAME_SHRINES_Gem_6FC76910(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2_MAYBE_UNUSED(pShrinesTxtRecord);
    D2InventoryStrc* pInventory = pOp->pPlayer->pInventory;
    D2UnitStrc* pGem = INVENTORY_GetBackPackItemByType(pInventory, ITEMTYPE_GEM, 0);
    
    int32_t bBetterGemDropped = 0;
    while (pGem)
    {
        if (bBetterGemDropped)
        {
            return;
        }

        const uint32_t nBetterGemCode = DATATBLS_GetItemsTxtRecord(pGem->dwClassId)->dwBetterGem;
        if (nBetterGemCode != ' non')
        {
            const int32_t nBetterGemId = DATATBLS_GetItemIdFromItemCode(nBetterGemCode);
            D2_ASSERT(nBetterGemId != -1);

            if (sub_6FC76A60(pOp->pGame, pOp->pPlayer, nBetterGemId, pGem))
            {
                bBetterGemDropped = 1;
                pGem = nullptr;
            }
        }

        pGem = INVENTORY_GetBackPackItemByType(pInventory, ITEMTYPE_GEM, pGem);
    }

    if (bBetterGemDropped)
    {
        return;
    }

    uint32_t nNewGemCode = 0;
    switch (ITEMS_RollRandomNumber(&pOp->pPlayer->pSeed) % 6)
    {
    case 0u:
        nNewGemCode = ' wcg';
        break;

    case 1u:
        nNewGemCode = ' rcg';
        break;

    case 2u:
        nNewGemCode = ' gcg';
        break;

    case 3u:
        nNewGemCode = ' bcg';
        break;

    case 4u:
        nNewGemCode = ' ycg';
        break;

    default:
        nNewGemCode = ' vcg';
        break;
    }

    const int32_t nNewGemId = DATATBLS_GetItemIdFromItemCode(nNewGemCode);
    D2_ASSERT(nNewGemId != -1);

    sub_6FC76A60(pOp->pGame, pOp->pPlayer, nNewGemId, 0);
}

//D2Game.0x6FC76A60
int32_t __fastcall sub_6FC76A60(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nItemId, D2UnitStrc* pItem)
{
    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pPlayer, &pCoord);
   
    D2CoordStrc pReturnCoords = {};
    D2RoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pPlayer), &pCoord, &pReturnCoords, 1);
    if (pRoom)
    {
        D2ItemDropStrc itemDrop = {};
        itemDrop.pUnit = 0;
        itemDrop.pSeed = 0;
        itemDrop.nId = nItemId;
        itemDrop.wItemFormat = pGame->wItemFormat;
        itemDrop.nX = pReturnCoords.nX;
        itemDrop.nY = pReturnCoords.nY;
        itemDrop.pRoom = pRoom;
        itemDrop.pGame = pGame;
        itemDrop.nSpawnType = 3;
        itemDrop.wUnitInitFlags = 1;
        itemDrop.nQuality = ITEMQUAL_NORMAL;
        itemDrop.nItemLvl = ITEMS_GetItemLevelForNewItem(pPlayer, 0);

        D2UnitStrc* pDroppedItem = D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
        STATLIST_SetUnitStat(pDroppedItem, STAT_QUANTITY, 1, 0);
        UNITROOM_RefreshUnit(pDroppedItem);

        if (pDroppedItem)
        {
            pDroppedItem->dwFlags |= UNITFLAG_DOUPDATE;
        }

        if (pItem)
        {
            int32_t nFlags = 0;
            ITEMS_SetOrRemoveFlag(&nFlags, 32, 1);
            ITEMS_SetItemCell(pItem, ITEMS_GetInvPage(pItem));
            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
            D2GAME_UpdateClientItem_6FC3E9D0(pClient, pPlayer, pItem, nFlags);
            D2GAME_RemoveItem_6FC471F0(pGame, pPlayer, pItem, 0);
        }

        return 1;
    }

    return 0;
}

//D2Game.0x6FC76BC0
void __fastcall D2GAME_SHRINES_Storm_6FC76BC0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    const int32_t nX = CLIENTS_GetUnitX(pOp->pObject);
    const int32_t nY = CLIENTS_GetUnitY(pOp->pObject);

    D2UnitFindArgStrc unitFindArg = {};
    unitFindArg.nX = nX;
    unitFindArg.nY = nY;
    unitFindArg.nSize = pShrinesTxtRecord->dwArg1;
    unitFindArg.nFlags = 0x583u;

    D2UnitFindDataStrc unitFindData = {};
    UNITFINDS_InitializeUnitFindData(pOp->pGame->pMemoryPool, &unitFindData, UNITS_GetRoom(pOp->pObject), nX, nY, pShrinesTxtRecord->dwArg1, sub_6FC76E90, &unitFindArg);
    UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(&unitFindData);

    for (int32_t i = 0; i < unitFindData.nIndex; ++i)
    {
        D2UnitStrc* pFoundUnit = unitFindData.pUnitsArray[i];
        if (pFoundUnit && (pFoundUnit->dwUnitType == UNIT_MONSTER || pFoundUnit->dwUnitType == UNIT_PLAYER) && !SUNIT_IsDead(unitFindData.pUnitsArray[i]))
        {
            const int32_t nHitpoints = STATLIST_GetUnitStatUnsigned(pFoundUnit, STAT_HITPOINTS, 0);
            STATLIST_AddUnitStat(pFoundUnit, STAT_HITPOINTS, ((nHitpoints >> 8) * pShrinesTxtRecord->dwArg0) / -100 << 8, 0);
        }
    }

    UNITFINDS_FreeUnitFindData(&unitFindData);

    for (int32_t x = 1; x < 5; ++x)
    {
        for (int32_t y = 1; y < 5; ++y)
        {
            D2MissileStrc missileParams = {};
            missileParams.dwFlags = 3;
            missileParams.nX = nX;
            missileParams.nY = nY;

            if (x & 1)
            {
                missileParams.nTargetX = 5 * x;
            }
            else
            {
                missileParams.nTargetX = -5 * x;
            }

            if (y & 1)
            {
                missileParams.nTargetY = 5 * y;
            }
            else
            {
                missileParams.nTargetY = -5 * y;
            }

            missileParams.pOwner = pOp->pPlayer;
            missileParams.nMissile = 62;

            missileParams.nSkillLevel = STATLIST_GetUnitStatUnsigned(missileParams.pOwner, STAT_LEVEL, 0) / 5;

            if (missileParams.nSkillLevel < 1)
            {
                missileParams.nSkillLevel = 1;
            }
            else if (missileParams.nSkillLevel > 8)
            {
                missileParams.nSkillLevel = 8;
            }

            MISSILES_CreateMissileFromParams(pOp->pGame, &missileParams);
        }
    }
}

//D2Game.0x6FC76E90
int32_t __fastcall sub_6FC76E90(D2UnitStrc* pUnit, D2UnitFindArgStrc* pArg)
{
    if (pUnit)
    {
        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            if (pUnit->dwAnimMode != PLRMODE_DEATH && pUnit->dwAnimMode != PLRMODE_DEAD)
            {
                return 1;
            }
        }
        else if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            if (!MONSTERS_IsDead(pUnit))
            {
                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FC76ED0
void __fastcall D2GAME_SHRINES_Monster_6FC76ED0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2_MAYBE_UNUSED(pShrinesTxtRecord);
    D2UnitStrc* pMonster = UNITFINDS_GetNearestTestedUnit(pOp->pPlayer, CLIENTS_GetUnitX(pOp->pPlayer), CLIENTS_GetUnitY(pOp->pPlayer), 0, sub_6FC76F60);
    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
    {
        pMonster->pMonsterData->nTypeFlag |= 1u;

        D2MonsterRegionStrc* pMonsterRegion = MONSTERREGION_GetMonsterRegionFromLevelId(pOp->pGame->pMonReg, MONSTER_GetLevelId(pMonster));
        if (pMonsterRegion && !(pMonster->pMonsterData->nTypeFlag & 8))
        {
            ++pMonsterRegion->dwUniqueCount;
        }
        pMonster->pMonsterData->nTypeFlag |= 8u;

        sub_6FC6E940(pOp->pGame, pMonster, 1);
        D2GAME_SpawnMinions_6FC6F440(pOp->pGame, UNITS_GetRoom(pMonster), 0, pMonster, 0, 0, 0);
        pMonster->dwFlags |= 0x800;
        MONSTER_ToggleSummonerFlag(pMonster, 1, 1);
    }
}

//D2Game.0x6FC76F60
int32_t __fastcall sub_6FC76F60(D2UnitStrc* pMonster, D2UnitStrc* pUnit1)
{
    if (pMonster != pUnit1 && pMonster && pMonster->dwUnitType == UNIT_MONSTER && STATLIST_AreUnitsAligned(pUnit1, pMonster) != 1 && !STATLIST_GetUnitAlignment(pMonster) && !MONSTERS_IsDead(pMonster))
    {
        const int32_t nAnimMode = pMonster->dwAnimMode;
        if ((nAnimMode == MONMODE_NEUTRAL || nAnimMode == MONMODE_WALK))
        {
            D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pMonster->dwClassId);
            if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[2] && pMonStats2TxtRecord->nHeight)
            {
                if (pMonster->pMonsterData->pMonstatsTxt && !MONSTERS_IsBoss(pMonster->pMonsterData->pMonstatsTxt, pMonster) && !MONSTERS_IsPrimeEvil(pMonster))
                {
                    return MONSTERUNIQUE_CheckMonTypeFlag(pMonster, 31) == 0;
                }
            }
        }
    }

    return 0;
}

//D2Game.0x6FC770D0
void __fastcall D2GAME_SHRINES_Exploding_6FC770D0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    const int32_t nItemsToDrop = ITEMS_RollLimitedRandomNumber(&pOp->pObject->pSeed, pShrinesTxtRecord->dwArg1 - pShrinesTxtRecord->dwArg0) + pShrinesTxtRecord->dwArg0;
    for (int32_t i = 0; i < nItemsToDrop; ++i)
    {
        const int32_t nItemIndex = DATATBLS_GetItemIdFromItemCode(' mpo');

        D2_ASSERT(nItemIndex != -1);

        D2CoordStrc pCoord = {};
        UNITS_GetCoords(pOp->pPlayer, &pCoord);
        D2CoordStrc pReturnCoords = {};
        D2RoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pOp->pPlayer), &pCoord, &pReturnCoords, 1);
        if (pRoom)
        {
            D2ItemDropStrc itemDrop = {};
            itemDrop.pUnit = nullptr;
            itemDrop.pSeed = nullptr;
            itemDrop.pGame = pOp->pGame;
            itemDrop.wItemFormat = pOp->pGame->wItemFormat;
            itemDrop.nId = nItemIndex;
            itemDrop.nX = pReturnCoords.nX;
            itemDrop.nY = pReturnCoords.nY;
            itemDrop.pRoom = pRoom;
            itemDrop.nSpawnType = 3;
            itemDrop.wUnitInitFlags = 1;
            itemDrop.nQuality = 0;
            itemDrop.nItemLvl = ITEMS_GetItemLevelForNewItem(pOp->pPlayer, 0);
            itemDrop.nQtyOverride = 1;

            D2UnitStrc* pItem = D2GAME_CreateItemEx_6FC4ED80(pOp->pGame, &itemDrop, 0);

            STATLIST_SetUnitStat(pItem, STAT_QUANTITY, 1, 0);
            UNITROOM_RefreshUnit(pItem);

            if (pItem)
            {
                pItem->dwFlags |= 1;
            }
        }
    }

    constexpr int32_t nXOffsets[] = { -6, -6, 0, 0, 6, 6 };
    constexpr int32_t nYOffsets[] = { 6, -6, 6, -6, 6, -6 };

    for (int32_t i = 0; i < 6; ++i)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 1312;
        missileParams.nX = CLIENTS_GetUnitX(pOp->pObject);
        missileParams.nY = CLIENTS_GetUnitY(pOp->pObject);
        missileParams.nTargetX = missileParams.nX + nXOffsets[i];
        missileParams.nTargetY = missileParams.nY + nYOffsets[i];
        missileParams.pOwner = pOp->pPlayer;
        missileParams.pOrigin = pOp->pObject;
        missileParams.nGfxArg = 1;
        missileParams.nMissile = 45;

        missileParams.nSkillLevel = STATLIST_GetUnitStatUnsigned(pOp->pPlayer, STAT_LEVEL, 0) / 5;
        if (missileParams.nSkillLevel < 1)
        {
            missileParams.nSkillLevel = 1;
        }
        else if (missileParams.nSkillLevel > 8)
        {
            missileParams.nSkillLevel = 8;
        }

        MISSILES_CreateMissileFromParams(pOp->pGame, &missileParams);
    }
}

//D2Game.0x6FC773B0
void __fastcall D2GAME_SHRINES_Poison_6FC773B0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    const int32_t nItemsToDrop = ITEMS_RollLimitedRandomNumber(&pOp->pObject->pSeed, pShrinesTxtRecord->dwArg1 - pShrinesTxtRecord->dwArg0) + pShrinesTxtRecord->dwArg0;
    for (int32_t i = 0; i < nItemsToDrop; ++i)
    {
        const int32_t nItemIndex = DATATBLS_GetItemIdFromItemCode(' mpg');

        D2_ASSERT(nItemIndex != -1);

        D2CoordStrc pCoord = {};
        UNITS_GetCoords(pOp->pPlayer, &pCoord);
        D2CoordStrc pReturnCoords = {};
        D2RoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pOp->pPlayer), &pCoord, &pReturnCoords, 1);
        if (pRoom)
        {
            D2ItemDropStrc itemDrop = {};
            itemDrop.pUnit = nullptr;
            itemDrop.pSeed = nullptr;
            itemDrop.pGame = pOp->pGame;
            itemDrop.wItemFormat = pOp->pGame->wItemFormat;
            itemDrop.nId = nItemIndex;
            itemDrop.nX = pReturnCoords.nX;
            itemDrop.nY = pReturnCoords.nY;
            itemDrop.pRoom = pRoom;
            itemDrop.nSpawnType = 3;
            itemDrop.wUnitInitFlags = 1;
            itemDrop.nQuality = 0;
            itemDrop.nItemLvl = ITEMS_GetItemLevelForNewItem(pOp->pPlayer, 0);
            itemDrop.nQtyOverride = 1;

            D2UnitStrc* pItem = D2GAME_CreateItemEx_6FC4ED80(pOp->pGame, &itemDrop, 0);

            STATLIST_SetUnitStat(pItem, STAT_QUANTITY, 1, 0);
            UNITROOM_RefreshUnit(pItem);

            if (pItem)
            {
                pItem->dwFlags |= 1;
            }
        }
    }

    constexpr int32_t nXOffsets[] = { -6, -6, 0, 0, 6, 6 };
    constexpr int32_t nYOffsets[] = { 6, -6, 6, -6, 6, -6 };

    for (int32_t i = 0; i < 6; ++i)
    {
        D2MissileStrc missileParams = {};
        missileParams.dwFlags = 1312;
        missileParams.nX = CLIENTS_GetUnitX(pOp->pObject);
        missileParams.nY = CLIENTS_GetUnitY(pOp->pObject);
        missileParams.nTargetX = missileParams.nX + nXOffsets[i];
        missileParams.nTargetY = missileParams.nY + nYOffsets[i];
        missileParams.pOwner = pOp->pPlayer;
        missileParams.pOrigin = pOp->pObject;
        missileParams.nGfxArg = 1;
        missileParams.nMissile = 48;

        missileParams.nSkillLevel = STATLIST_GetUnitStatUnsigned(pOp->pPlayer, STAT_LEVEL, 0) / 5;
        if (missileParams.nSkillLevel < 1)
        {
            missileParams.nSkillLevel = 1;
        }
        else if (missileParams.nSkillLevel > 8)
        {
            missileParams.nSkillLevel = 8;
        }

        MISSILES_CreateMissileFromParams(pOp->pGame, &missileParams);
    }
}

constexpr D2ShrineTableStrc gpShrineTable_6FD28D18[] =
{
    { nullptr, -1, 0 },
    { D2GAME_SHRINES_Refill_6FC76730, -1, 0 },
    { D2GAME_SHRINES_Health_6FC766B0, -1, 0 },
    { D2GAME_SHRINES_FillMana_6FC766F0, -1, 0 },
    { D2GAME_SHRINES_ExchangeHealth_6FC76790, -1, 0 },
    { D2GAME_SHRINES_ExchangeMana_6FC767F0, -1, 0 },
    { D2GAME_SHRINES_DefensiveBoost_6FC77AE0, 0xAB, 0x80 },
    { D2GAME_SHRINES_CombatBoost_6FC77690, -1, 0x81 },
    { D2GAME_SHRINES_DefensiveBoost_6FC77AE0, 0x27, 0x83 },
    { D2GAME_SHRINES_DefensiveBoost_6FC77AE0, 0x2B, 0x84 },
    { D2GAME_SHRINES_DefensiveBoost_6FC77AE0, 0x29, 0x82 },
    { D2GAME_SHRINES_DefensiveBoost_6FC77AE0, 0x2D, 0x85 },
    { D2GAME_SHRINES_SkillBoost_6FC77BA0, 0x0B, 0x86 },
    { D2GAME_SHRINES_DefensiveBoost_6FC77AE0, 0x1B, 0x87 },
    { D2GAME_SHRINES_Stamina_6FC779C0, 0xA2, 0x88 },
    { D2GAME_SHRINES_DefensiveBoost_6FC77AE0, 0x55, 0x89 },
    { D2GAME_SHRINES_Enirhs_6FC76850, -1, 0 },
    { D2GAME_SHRINES_Portal_6FC76880, -1, 0 },
    { D2GAME_SHRINES_Gem_6FC76910, -1, 0 },
    { D2GAME_SHRINES_Storm_6FC76BC0, -1, 0 },
    { D2GAME_SHRINES_Monster_6FC76ED0, -1, 0 },
    { D2GAME_SHRINES_Exploding_6FC770D0, -1, 0 },
    { D2GAME_SHRINES_Poison_6FC773B0, -1, 0 },
    { nullptr, -1, 0 },
};

//D2Game.0x6FC77690
void __fastcall D2GAME_SHRINES_CombatBoost_6FC77690(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2_ASSERT(pShrinesTxtRecord->nCode < std::size(gpShrineTable_6FD28D18));

    const D2ShrineTableStrc* pShrineRecord = &gpShrineTable_6FD28D18[pShrinesTxtRecord->nCode];

    const int32_t nValue = sub_6FC77750(pShrinesTxtRecord, 19, pShrinesTxtRecord->dwArg0, pOp->pPlayer);

    D2CurseStrc curse = {};
    curse.pTarget = pOp->pPlayer;
    curse.pUnit = pOp->pObject;
    curse.nState = pShrineRecord->unk0x08;
    curse.nSkill = 0;
    curse.nSkillLevel = 0;
    curse.nDuration = pShrinesTxtRecord->dwDurationInFrames;
    curse.nStat = 25;
    curse.nStatValue = pShrinesTxtRecord->dwArg1;
    curse.pStateFunc = 0;

    STATLIST_SetStat(sub_6FD10EC0(&curse), STAT_TOHIT, nValue, 0);
}

//D2Game.0x6FC77750
int32_t __fastcall sub_6FC77750(D2ShrinesTxt* pShrinesTxtRecord, int32_t nStatId, int32_t nValue, D2UnitStrc* pUnit)
{
    D2UnitStrc* pWeapon = nullptr;
    if (pUnit && pUnit->pInventory)
    {
        pWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
    }

    switch (nStatId)
    {
    case STAT_MAXSTAMINA:
    case STAT_MANARECOVERYBONUS:
    case STAT_FIRERESIST:
    case STAT_LIGHTRESIST:
    case STAT_COLDRESIST:
    case STAT_POISONRESIST:
    case STAT_ITEM_ADDEXPERIENCE:
    case STAT_SKILL_ARMOR_PERCENT:
    {
        return nValue;
    }
    case STAT_TOHIT:
    {
        D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, 0, -1);
        int32_t nAttackRate = UNITS_GetAttackRate(pUnit);
        if (!pSkill)
        {
            FOG_DisplayWarning("hSkill", __FILE__, __LINE__);
        }

        if (!pWeapon)
        {
            pWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
        }

        if (pWeapon && ITEMS_GetItemType(pWeapon) == ITEMTYPE_MISSILE_POTION)
        {
            nAttackRate = 0;
        }

        int32_t nBonus = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_TOHIT_PERCENT, 0) + SKILLS_GetWeaponMasteryBonus(pUnit, pWeapon, pSkill, 0);
        int32_t nPercentage = nAttackRate * ((nBonus + SKILLS_GetSkillsTxtRecordFromSkill(pSkill)->dwToHit) / 100 + 1);

        if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
        {
            D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pUnit->dwClassId);
            if (pCharStatsTxtRecord)
            {
                nPercentage += pCharStatsTxtRecord->dwToHitFactor;
            }
        }

        return nValue * nPercentage / 100;
    }
    case STAT_MINDAMAGE:
    {
        if ((D2Common_10434(pUnit, 1) || pWeapon) && INVENTORY_GetWieldType(pUnit, pUnit->pInventory) == 2)
        {
            return nValue * STATLIST_GetUnitStatUnsigned(pUnit, STAT_SECONDARY_MINDAMAGE, 0) / 100;
        }
        else
        {
            return nValue * STATLIST_GetUnitStatUnsigned(pUnit, STAT_MINDAMAGE, 0) / 100;
        }
    }
    default:
        break;
    }

    return nValue * STATLIST_GetUnitStatUnsigned(pUnit, nStatId, 0) / 100;
}

//D2Game.0x6FC779C0
void __fastcall D2GAME_SHRINES_Stamina_6FC779C0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2_ASSERT(pShrinesTxtRecord->nCode < std::size(gpShrineTable_6FD28D18));

    const D2ShrineTableStrc* pShrineRecord = &gpShrineTable_6FD28D18[pShrinesTxtRecord->nCode];

    STATLIST_SetUnitStat(pOp->pPlayer, 10, STATLIST_GetMaxStaminaFromUnit(pOp->pPlayer), 0);

    D2CurseStrc curse = {};
    curse.pTarget = pOp->pPlayer;
    curse.pUnit = pOp->pObject;
    curse.nStatValue = sub_6FC77750(pShrinesTxtRecord, pShrineRecord->unk0x04, pShrinesTxtRecord->dwArg0, pOp->pPlayer);
    curse.nStat = pShrineRecord->unk0x04;
    curse.nSkill = 0;
    curse.nSkillLevel = 0;
    curse.nDuration = pShrinesTxtRecord->dwDurationInFrames;
    curse.nState = pShrineRecord->unk0x08;
    curse.pStateFunc = sub_6FC77AB0;

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    STATLIST_SetStat(pStatList, STAT_STAMINA, 2 * curse.nStatValue, 0);
    STATLIST_SetStat(pStatList, STAT_STAMINARECOVERYBONUS, 1000, 0);
}

//D2Game.0x6FC77AB0
void __fastcall sub_6FC77AB0(D2UnitStrc* pUnit, int32_t a2, int32_t a3)
{
    sub_6FD10E50(pUnit, a2, a3);
    STATLIST_SetUnitStat(pUnit, STAT_STAMINA, STATLIST_GetMaxStaminaFromUnit(pUnit), 0);
}

//D2Game.0x6FC77AE0
void __fastcall D2GAME_SHRINES_DefensiveBoost_6FC77AE0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2_ASSERT(pShrinesTxtRecord->nCode < std::size(gpShrineTable_6FD28D18));

    const D2ShrineTableStrc* pShrineRecord = &gpShrineTable_6FD28D18[pShrinesTxtRecord->nCode];

    int32_t nStatValue = 0;
    if (pShrineRecord->unk0x04 != -1)
    {
        nStatValue = sub_6FC77750(pShrinesTxtRecord, pShrineRecord->unk0x04, pShrinesTxtRecord->dwArg0, pOp->pPlayer);
    }

    D2CurseStrc curse = {};
    curse.pUnit = pOp->pObject;
    curse.nDuration = pShrinesTxtRecord->dwDurationInFrames;
    curse.pTarget = pOp->pPlayer;
    curse.nStatValue = nStatValue;
    curse.nStat = pShrineRecord->unk0x04;
    curse.nSkill = 0;
    curse.nSkillLevel = 0;
    curse.nState = pShrineRecord->unk0x08;
    curse.pStateFunc = 0;

    sub_6FD10EC0(&curse);
}

//D2Game.0x6FC77BA0
void __fastcall D2GAME_SHRINES_SkillBoost_6FC77BA0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord)
{
    D2CurseStrc curse = {};

    curse.pUnit = pOp->pObject;
    curse.nSkill = 0;
    curse.nSkillLevel = 0;
    curse.nStatValue = 0;
    curse.pTarget = pOp->pPlayer;
    curse.nDuration = pShrinesTxtRecord->dwDurationInFrames;
    curse.nStat = STAT_MAXSTAMINA;
    curse.pStateFunc = sub_6FC77C10;
    curse.nState = gpShrineTable_6FD28D18[pShrinesTxtRecord->nCode].unk0x08;
    sub_6FD10EC0(&curse);
    sub_6FD14C30(pOp->pPlayer);
}

//D2Game.0x6FC77C10
void __fastcall sub_6FC77C10(D2UnitStrc* pUnit, int32_t a2, int32_t a3)
{
    sub_6FD10E50(pUnit, a2, a3);
    sub_6FD14C30(pUnit);
}

//D2Game.0x6FC77C30
int32_t __fastcall OBJECTS_OperateFunction02_Shrine(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;
    D2ObjectDataStrc* pObjectData = pObject->pObjectData;
    D2UnitStrc* pPlayer = pOp->pPlayer;

    if (!pObject || !pObjectData || !pPlayer)
    {
        return 0;
    }

    if (pObjectData->dwOperateGUID || pObject->dwAnimMode != OBJMODE_NEUTRAL)
    {
        return 0;
    }

    pObjectData->dwOperateGUID = pPlayer->dwUnitId + 1;
    UNITROOM_RefreshUnit(pObject);

    pObject->dwFlags |= UNITFLAG_DOUPDATE;
    UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

    if (pObject->pHoverText)
    {
        CHAT_FreeHoverMsg(pOp->pGame->pMemoryPool, pObject->pHoverText);
    }

    char szText[256] = {};
    wsprintfA(szText, "%d", (WORD)(pObjectData->InteractType + 3683));

    D2HoverTextStrc* pHoverText = CHAT_AllocHoverMsg(pOp->pGame->pMemoryPool, szText, pOp->pGame->dwGameFrame);
    if (pHoverText)
    {
        pObject->pHoverText = pHoverText;
        UNITROOM_RefreshUnit(pObject);

        pObject->dwFlags |= UNITFLAG_HASTXTMSG;
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_FREEHOVER, pOp->pGame->dwGameFrame + 300, 0, 0);
    }

    D2ShrinesTxt* pShrinesTxtRecord = UNITS_GetShrineTxtRecordFromObject(pObject);

    const int32_t nId = pShrinesTxtRecord->nCode;
    if (nId >= 0 && nId < 24)
    {
        ObjShrineFunction pfCallback = gpShrineTable_6FD28D18[nId].pfShrineCallback;
        if (!pfCallback || IsBadCodePtr((FARPROC)pfCallback))
        {
            FOG_DisplayAssert("sgcptShrineChart[idx].pfnHandler", __FILE__, __LINE__);
            exit(-1);
        }

        pfCallback(pOp, pShrinesTxtRecord);
    }
    else
    {
        D2GAME_SHRINES_Refill_6FC76730(pOp, pShrinesTxtRecord);
    }

    if (pShrinesTxtRecord->nResetTimeInMins)
    {
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ACTIVESTATE, 1200 * pShrinesTxtRecord->nResetTimeInMins + pOp->pGame->dwGameFrame + 1, 0, 0);
    }

    if (pObject->dwAnimMode == OBJMODE_OPERATING)
    {
        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        if (!pObjectsTxtRecord->nCycleAnim[1] && pObjectsTxtRecord->nMode[2])
        {
            EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
        }
    }

    return 1;
}

//D2Game.0x6FC77E80
int32_t __fastcall OBJECTS_OperateFunction01_Casket(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL && OBJMODE_DropFromChestTCWithQuality(pOp, 0))
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);

        if (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 10000 & 0xFFFFE000)
        {
            D2GAME_SpawnTrapMonster_6FC75B40(pOp, OBJRGN_GetTrapMonsterId(pOp), 8);
        }

        if (!pObjectsTxtRecord->nHasCollision[1])
        {
            UNITS_FreeCollisionPath(pObject);
        }

         D2GAME_SetTrapCallback_6FC764B0(pOp, pObject->pObjectData->InteractType & 0x7F);
    }

    return 1;
}

//D2Game.0x6FC77F70
int32_t __fastcall OBJECTS_OperateFunction68_EvilUrn(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL && OBJMODE_DropFromChestTCWithQuality(pOp, 0))
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
        const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);

        if (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 255 <= pObjectsTxtRecord->dwParm[7])
        {
            D2MonsterRegionStrc* pMonsterRegion = pOp->pGame->pMonReg[nLevelId];
            
            for (int32_t i = 0; i < pMonsterRegion->nMonCount; ++i)
            {
                D2MonRegDataStrc* pMonRegData = &pMonsterRegion->pMonData[i];
                D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pMonRegData->nMonHcIdx);
                if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_WALK])
                {
                    sub_6FC6E8D0(pOp->pGame, pRoom, 0, pMonRegData->nMonHcIdx, 1, 0, 0, 1);
                    break;
                }
            }
        }

        if (!pObjectsTxtRecord->nHasCollision[1])
        {
            UNITS_FreeCollisionPath(pObject);
        }

         D2GAME_SetTrapCallback_6FC764B0(pOp, pObject->pObjectData->InteractType & 0x7F);
    }

    return 1;
}

//D2Game.0x6FC78120
int32_t __fastcall OBJECTS_OperateFunction33_WirtsBody(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        pObject->dwDropItemCode = ' gel';

        int32_t nItemLevel = 0;
        if (D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pObject, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0))
        {
            UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

            D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
            EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
            EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_QUESTFN, pOp->pGame->dwGameFrame + 10, 0, 0);
        }
    }

    return 1;
}

//D2Game.0x6FC781C0
int32_t __fastcall OBJECTS_OperateFunction03_Urn_Basket_Jar(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);

        if (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 100 <= 20)
        {
            OBJMODE_DropFromChestTCWithQuality(pOp, 0);
        }

        if (!pObjectsTxtRecord->nHasCollision[1])
        {
            UNITS_FreeCollisionPath(pObject);
        }

         D2GAME_SetTrapCallback_6FC764B0(pOp, pObject->pObjectData->InteractType & 0x7F);
    }

    return 1;
}

//D2Game.0x6FC78290
int32_t __fastcall OBJECTS_OperateFunction14_Corpse(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        OBJMODE_DropFromChestTCWithQuality(pOp, 0);
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);

        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);

        if (!pObjectsTxtRecord->nHasCollision[1])
        {
            UNITS_FreeCollisionPath(pObject);
        }

         D2GAME_SetTrapCallback_6FC764B0(pOp, pObject->pObjectData->InteractType & 0x7F);
    }

    return 1;
}

//D2Game.0x6FC78340
void __fastcall sub_6FC78340(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2ObjOperateFnStrc op = {};

    op.pGame = pGame;
    op.pObject = pUnit;
    //UNITS_GetRoom(pUnit);
    op.pObjectregion = OBJRGN_GetObjectControlFromGame(pGame);
    op.pPlayer = SUNIT_GetServerUnit(pGame, 0, OBJECTS_GetUnitIdFromTimerArg(pUnit));
    OBJMODE_DropFromChestTCWithQuality(&op, 0);
}

//D2Game.0x6FC78390
int32_t __fastcall OBJECTS_OperateFunction51_JungleStash(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

        pObject->dwFlags &= ~UNITFLAG_TARGETABLE;

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);

        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_AIRESET, pOp->pGame->dwGameFrame + pObjectsTxtRecord->dwParm[1] + 1, 0, 0);

        if (!pObjectsTxtRecord->nHasCollision[1])
        {
            UNITS_FreeCollisionPath(pObject);
        }

         D2GAME_SetTrapCallback_6FC764B0(pOp, pObject->pObjectData->InteractType & 0x7F);
         OBJECTS_SetUnitIdInTimerArg(pObject, pOp->pPlayer ? pOp->pPlayer->dwUnitId : -1);
    }

    return 1;
}

//D2Game.0x6FC78470
int32_t __fastcall OBJECTS_OperateFunction18_SecretDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);

        UNITS_FreeCollisionPath(pObject);
    }

    return 1;
}

//D2Game.0x6FC784E0
int32_t __fastcall OBJECTS_OperateFunction26_BookShelf(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);

        if (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 20 <= 12)
        {
            if (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) & 1)
            {
                pObject->dwDropItemCode = ' csi';
            }
            else
            {
                pObject->dwDropItemCode = ' cst';
            }
        }
        else
        {
            if (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) & 1)
            {
                pObject->dwDropItemCode = ' kbi';
            }
            else
            {
                pObject->dwDropItemCode = ' kbt';
            }
        }

        int32_t nItemLevel = 0;
        D2GAME_DropItemAtUnit_6FC4FEC0(pOp->pGame, pObject, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0);
    }

    return 1;
}

//D2Game.0x6FC785D0
int32_t __fastcall OBJECTS_OperateFunction19_ArmorStand(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        D2CoordStrc pCoord = {};
        UNITS_GetCoords(pObject, &pCoord);

        D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
        D2GAME_DropArmor_6FC4F830(pOp->pGame, pRoom, &pCoord, -1, 0, 0);

        UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);
    }

    return 1;
}

//D2Game.0x6FC78640
int32_t __fastcall OBJECTS_OperateFunction20_WeaponRack(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        D2CoordStrc pCoord = {};
        UNITS_GetCoords(pObject, &pCoord);

        D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
        D2GAME_DropWeapon_6FC4FA50(pOp->pGame, pRoom, &pCoord, -1, 0, 0);

        UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);
    }

    return 1;
}

//D2Game.0x6FC786B0
int32_t __fastcall OBJECTS_OperateFunction05_Barrel(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        D2UnitStrc* pPlayer = pOp->pPlayer;
        if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER)
        {
            D2SkillStrc* pSkill = SKILLS_GetSkillById(pPlayer, 1, -1);
            if (pSkill)
            {
                UNITS_SetTargetUnitForDynamicUnit(pPlayer, pObject);

                D2GAME_PLAYERMODE_Change_6FC81A00(pOp->pGame, pPlayer, pSkill, SKILLS_GetSkillMode(pSkill), UNIT_OBJECT, pObject->dwUnitId, 0);
            }
        }

        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);

        UNITS_FreeCollisionPath(pObject);

        int32_t nMonsterId = 0;
        if (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 10000 & 0xFFFFE000)
        {
            nMonsterId = OBJRGN_GetTrapMonsterId(pOp);
            D2GAME_SpawnTrapMonster_6FC75B40(pOp, nMonsterId, 8);
        }

        if (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 100 <= 20)
        {
            OBJMODE_DropFromChestTCWithQuality(pOp, 0);
        }

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
    }

    return 1;
}

//D2Game.0x6FC787F0
int32_t __fastcall OBJECTS_OperateFunction07_ExplodingBarrel(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);

        D2ObjOperateFnStrc pNewOp = {};
        memcpy(&pNewOp, pOp, sizeof(pNewOp));
        
        D2CoordStrc pCoord = {};
        UNITS_GetCoords(pObject, &pCoord);

        for (D2UnitStrc* i = UNITS_GetRoom(pObject)->pUnitFirst; i; i = i->pRoomNext)
        {
            const int32_t nUnitType = i->dwUnitType;
            const int32_t nAnimMode = i->dwAnimMode;

            if (i != pObject && nUnitType != UNIT_TILE)
            {
                if (nUnitType == UNIT_PLAYER)
                {
                    if (nAnimMode != PLRMODE_DEAD && UNITS_IsInRange(i, &pCoord, 3))
                    {
                        OBJEVAL_ApplyTrapObjectDamage(pOp->pGame, pObject, i, 0);
                    }
                }
                else if (nUnitType == UNIT_MONSTER)
                {
                    if (nAnimMode != MONMODE_DEAD && UNITS_IsInRange(i, &pCoord, 3))
                    {
                        OBJEVAL_ApplyTrapObjectDamage(pOp->pGame, pObject, i, 0);
                    }
                }
                else if (nUnitType == UNIT_OBJECT)
                {
                    if (i && i->dwClassId == 11 && i->dwAnimMode == OBJMODE_NEUTRAL && UNITS_GetDistanceToOtherUnit(pObject, i) <= 2)
                    {
                        pNewOp.pObject = i;
                        OBJECTS_OperateFunction07_ExplodingBarrel(&pNewOp, nOperate);
                    }
                }
            }
        }

        UNITS_FreeCollisionPath(pObject);

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
    }

    return 1;
}

//D2Game.0x6FC78940
int32_t __fastcall OBJECTS_OperateFunction13_TorchTiki(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
    {
        if (pObject->dwAnimMode == OBJMODE_OPERATING)
        {
            UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
        }
    }
    else
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
    }

    return 1;
}

//D2Game.0x6FC78970
int32_t __fastcall OBJECTS_OperateFunction11_Torch(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
    {
        if (pObject->dwAnimMode <= OBJMODE_OPENED)
        {
            UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
        }
    }
    else
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);
        pObject->dwFlags &= ~(UNITFLAG_TARGETABLE);
    }

    return 1;
}

//D2Game.0x6FC789C0
int32_t __fastcall OBJECTS_OperateHandler(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nObjectType, int32_t nObjectGUID, int32_t* pResult)
{
    *pResult = 1;

    D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, nObjectType, nObjectGUID);
    if (!pObject)
    {
        return 0;
    }

    D2ObjectsTxt* pObjectTxtRecord = UNITS_GetObjectTxtRecordFromObject(pObject);

    if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER || !SUNIT_GetInteractInfo(pPlayer, &nObjectType, &nObjectGUID) && pPlayer->pPlayerData && !pPlayer->pPlayerData->bBusy && (!INVENTORY_GetCursorItem(pPlayer->pInventory) || pObject->dwClassId == OBJECT_STASH))
    {
        D2_ASSERT(pObjectTxtRecord->nOperateFn < std::size(gpObjOperateFnTable));

        switch (pObject->dwClassId)
        {
        case OBJECT_STONETHETA:
        case OBJECT_JERHYN_PLACEHOLDER1:
        case OBJECT_JERHYN_PLACEHOLDER2:
            return 0;
        }

        const ObjOperateFunction pOperateFn = gpObjOperateFnTable[pObjectTxtRecord->nOperateFn];

        D2_ASSERT(pOperateFn);

        D2ObjOperateFnStrc pObjOperate = {};

        pObjOperate.pGame = pGame;
        pObjOperate.pObject = pObject;
        pObjOperate.pPlayer = pPlayer;
        pObjOperate.pObjectregion = pGame->pObjectControl;
        pObjOperate.nObjectIdx = pObject->dwClassId;

        return pOperateFn(&pObjOperate, pObjectTxtRecord->nOperateFn);
    }

    return 1;
}

//D2Game.0x6FC78B20
int32_t __fastcall sub_6FC78B20(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nObjectType, int32_t nObjectGUID)
{
    D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, nObjectType, nObjectGUID);
    if (pObject)
    {
        if (pUnit)
        {
            if (pUnit->dwUnitType == UNIT_MONSTER && !DATATBLS_GetObjectsTxtRecord(pObject->dwClassId)->nMonsterOK)
            {
                return 1;
            }

            if (!UNITS_IsObjectInInteractRange(pUnit, pObject))
            {
                return 1;
            }
        }

        int32_t nResult = 1;
        OBJECTS_OperateHandler(pGame, pUnit, nObjectType, nObjectGUID, &nResult);
        return nResult;
    }

    return 0;
}

//D2Game.0x6FC78BB0
void __fastcall sub_6FC78BB0(D2GameStrc* pGame, int32_t nUnused, int32_t nObjectGUID)
{
    D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, nObjectGUID);
    D2_ASSERT(pObject);

    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pObject, &pCoord);

    const int32_t nUnitSizeX = UNITS_GetUnitSizeX(pObject);
    const int32_t nUnitSizeY = UNITS_GetUnitSizeY(pObject);
    const int32_t nAnimMode = pObject->dwAnimMode;
    if (nAnimMode)
    {
        if (nAnimMode == OBJMODE_OPENED || nAnimMode == OBJMODE_SPECIAL3)
        {
            if (COLLISION_CheckMaskWithSizeXY(UNITS_GetRoom(pObject), pCoord.nX, pCoord.nY, nUnitSizeX, nUnitSizeY, 0x8180u))
            {
                if (nAnimMode != OBJMODE_SPECIAL3)
                {
                    UNITS_ChangeAnimMode(pObject, OBJMODE_SPECIAL3);
                }
            }
            else
            {
                if (nAnimMode != OBJMODE_OPENED)
                {
                    UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
                }
            }
        }
    }
    //else
    //{
    //    COLLISION_CheckMaskWithSizeXY(UNITS_GetRoom(pObject), pCoord.nX, pCoord.nY, nUnitSizeX, nUnitSizeY, 0x8180u);
    //}
}

//D2Game.0x6FC78C90
int32_t __fastcall OBJECTS_OperateFunction34_ArcaneSanctuaryPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);

    if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
    {
        if (nLevelId == LEVEL_ARCANESANCTUARY)
        {
            LEVEL_WarpUnit(pOp->pGame, pOp->pPlayer, LEVEL_PALACECELLARLEV3, 0);
            ACT2Q4_SetPortalMode(pOp->pGame, LEVEL_ARCANESANCTUARY);
        }
        else if (nLevelId == LEVEL_PALACECELLARLEV3)
        {
            LEVEL_WarpUnit(pOp->pGame, pOp->pPlayer, LEVEL_ARCANESANCTUARY, 11);
            ACT2Q4_SetPortalMode(pOp->pGame, LEVEL_PALACECELLARLEV3);
        }
    }
    else
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
    }

    return 1;
}

//D2Game.0x6FC78D30
int32_t __fastcall OBJECTS_OperateFunction46_HellGatePortal(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);

    if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
    {
        D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pOp->pPlayer)->pQuestData[pOp->pGame->nDifficulty];
        if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q6, QFLAG_PRIMARYGOALDONE) || QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q6, QFLAG_REWARDGRANTED))
        {
            QUESTS_HellPortalActChange(pOp->pGame, pOp->pPlayer, nLevelId, pObject);
            if (nLevelId == LEVEL_DURANCEOFHATELEV3)
            {
                D2WaypointDataStrc* pWaypointFlags = UNITS_GetPlayerData(pOp->pPlayer)->pWaypointData[pOp->pGame->nDifficulty];
                short nWpNo = 0;
                if (WAYPOINTS_GetWaypointNoFromLevelId(LEVEL_THEPANDEMONIUMFORTRESS, &nWpNo))
                {
                    WAYPOINTS_ActivateWaypoint(pWaypointFlags, nWpNo);
                }

                LEVEL_WarpUnit(pOp->pGame, pOp->pPlayer, LEVEL_THEPANDEMONIUMFORTRESS, 0);
            }
        }
    }

    return 1;
}

//D2Game.0x6FC78E00
int32_t __fastcall OBJECTS_OperateFunction15_Portal(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject || !pOp->pPlayer || !pOp->pGame)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;
    D2UnitStrc* pPlayer = pOp->pPlayer;
    D2GameStrc* pGame = pOp->pGame;

    if (!PLAYER_IsBusy(pPlayer))
    {
        if (!D2GAME_IteratePlayers_6FC7C750(pGame, pPlayer) || pPlayer->dwUnitId == OBJECTS_GetUnitIdFromTimerArg(pObject))
        {
            if (PLAYERLIST_GetHostileDelay(pPlayer, 0) + 5000 > GetTickCount())
            {
                SUNIT_AttachSound(pPlayer, 19, pPlayer);
                return 0;
            }

            const int32_t nOtherPlayerId = OBJECTS_GetUnitIdFromTimerArg(pObject);
            if (nOtherPlayerId != -1 && nOtherPlayerId != pPlayer->dwUnitId)
            {
                if (SUNIT_GetPartyId(pPlayer) == -1)
                {
                    SUNIT_AttachSound(pPlayer, 19, pPlayer);
                    return 0;
                }

                D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nOtherPlayerId);
                if (pOtherPlayer)
                {
                    if (SUNIT_GetPartyId(pPlayer) != SUNIT_GetPartyId(pOtherPlayer))
                    {
                        SUNIT_AttachSound(pPlayer, 19, pPlayer);
                        return 0;
                    }
                }
            }

            D2_ASSERT(pPlayer && pPlayer->dwUnitType == UNIT_PLAYER);

            D2UnitStrc* pPortalOwner = SUNIT_GetPortalOwner(pGame, pObject);
            if (pPortalOwner && pPortalOwner->dwUnitType != UNIT_OBJECT)
            {
                pPortalOwner = nullptr;
            }

            if (UNITS_GetRoom(pObject))
            {
                D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pPlayer)->pQuestData[pGame->nDifficulty];
                if (!pQuestFlags)
                {
                    SUNIT_AttachSound(pPlayer, 19, pPlayer);
                    return 0;
                }

                D2LevelDefBin* pLevelDef = DATATBLS_GetLevelDefRecord(pObject->pObjectData->InteractType);
                if (!pLevelDef)
                {
                    SUNIT_AttachSound(pPlayer, 19, pPlayer);
                    return 0;
                }

                if (pOp->nObjectIdx == OBJECT_TOWN_PORTAL && PLAYER_GetUniqueIdFromPlayerData(pPlayer) != pObject->dwUnitId)
                {
                    if (pPortalOwner && PLAYER_GetUniqueIdFromPlayerData(pPlayer) != pPortalOwner->dwUnitId)
                    {
                        const int32_t nQuestFlag = pGame->bExpansion ? pLevelDef->dwQuestFlagEx : pLevelDef->dwQuestFlag;
                        if (nQuestFlag > 0 && !QUESTRECORD_GetQuestState(pQuestFlags, nQuestFlag, QFLAG_REWARDGRANTED))
                        {
                            SUNIT_AttachSound(pPlayer, 19, pPlayer);
                            return 0;
                        }
                    }
                }
            }

            D2CoordStrc pCoord = {}; 
            D2RoomStrc* pRoom = nullptr;
            if (pPortalOwner)
            {
                pCoord.nX = CLIENTS_GetUnitX(pPortalOwner);
                pCoord.nY = CLIENTS_GetUnitY(pPortalOwner);
                pRoom = UNITS_GetRoom(pPortalOwner);
            }
            else
            {
                const uint8_t nActNo = DRLG_GetActNoFromLevelId(pObject->pObjectData->InteractType);
                pRoom = DUNGEON_FindActSpawnLocationEx(pGame->pAct[nActNo], pObject->pObjectData->InteractType, 0, &(pCoord.nX), &(pCoord.nY), 3);
            }

            UNITS_SetObjectPortalFlags(pObject, UNITS_GetObjectPortalFlags(pObject) | 5);

            D2RoomStrc* pPlayerRoom = UNITS_GetRoom(pPlayer);
            if (DUNGEON_IsRoomInTown(pPlayerRoom))
            {
                QUESTS_ChangeLevel(pGame, DUNGEON_GetLevelIdFromRoom(pPlayerRoom), DUNGEON_GetLevelIdFromRoom(pRoom), pPlayer);
            }

            D2RoomStrc* pFreeCoordsRoom = COLLISION_GetFreeCoordinates(pRoom, &pCoord, UNITS_GetUnitSizeX(pPlayer), 0x1C09, 0);
            if (pFreeCoordsRoom)
            {
                if (!sub_6FCBDFE0(pGame, pPlayer, pFreeCoordsRoom, pCoord.nX, pCoord.nY, 0, 0))
                {
                    FOG_DisplayAssert("fRet", __FILE__, __LINE__);
                    exit(-1);
                }

                D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
                SUNIT_AttachSound(pPlayer, 8, 0);
                sub_6FC817D0(pGame, pPlayer, 0, 2, pCoord.nX + 5, pCoord.nY + 5, 0);
                D2GAME_PACKETS_SendPacket0x0D_6FC3C920(pClient, 0xD, pPlayer->dwUnitType, pPlayer->dwUnitId, 1, pCoord.nX + 5, pCoord.nY + 5, 0, 0);

                if (pPortalOwner && pObject->dwClassId != OBJECT_PERMANENT_TOWN_PORTAL)
                {
                    if (PLAYER_GetUniqueIdFromPlayerData(pPlayer) == pPortalOwner->dwUnitId)
                    {
                        D2RoomStrc* pObjectRoom = UNITS_GetRoom(pObject);
                        DUNGEON_AllocDrlgDelete(pObjectRoom, UNIT_OBJECT, pObject->dwUnitId);
                        SUNIT_RemoveUnit(pGame, pObject);
                        DUNGEON_ToggleHasPortalFlag(pObjectRoom, 1);
                        ACT5Q5_OnPortalClosed(pGame, pPortalOwner);

                        D2RoomStrc* pOwnerRoom = UNITS_GetRoom(pPortalOwner);
                        DUNGEON_AllocDrlgDelete(pOwnerRoom, UNIT_OBJECT, pPortalOwner->dwUnitId);
                        SUNIT_RemoveUnit(pGame, pPortalOwner);
                        DUNGEON_ToggleHasPortalFlag(pOwnerRoom, 1);
                    }
                }
                else
                {
                    if (pObject->dwAnimMode == OBJMODE_OPERATING)
                    {
                        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pObject->dwClassId);
                        EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pGame->dwGameFrame + 1, 0, 0);
                    }
                }

                D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2, pGame->dwGameFrame + 75, 0, pPlayer->dwUnitType);
                if (pStatList)
                {
                    EVENT_SetEvent(pGame, pPlayer, UNITEVENTCALLBACK_REMOVESTATE, pGame->dwGameFrame + 75, 0, 0);
                    STATES_ToggleState(pPlayer, STATE_JUST_PORTALED, 1);
                    STATLIST_SetState(pStatList, STATE_JUST_PORTALED);
                    STATLIST_SetStatRemoveCallback(pStatList, sub_6FD10E50);
                    D2COMMON_10475_PostStatToStatList(pPlayer, pStatList, 1);
                }
            }
        }
    }

    return 0;
}

//D2Game.0x6FC79310
int32_t __fastcall OBJECTS_OperateFunction43_DurielPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject || !pOp->pPlayer)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;
    D2UnitStrc* pPlayer = pOp->pPlayer;

    D2_ASSERT(pPlayer->dwUnitType == UNIT_PLAYER);

    UNITS_SetObjectPortalFlags(pObject, UNITS_GetObjectPortalFlags(pObject) | 5);

    int32_t nLevel = 0;
    if (pObject->dwClassId == 100)
    {
        nLevel = LEVEL_DURIELSLAIR;
    }
    else if (pObject->dwClassId == 377)
    {
        nLevel = pObject->pObjectData->InteractType;
    }

    const uint8_t nAct = DRLG_GetActNoFromLevelId(nLevel);
    D2_ASSERT(nAct < 5);

    const int32_t nSize = UNITS_GetUnitSizeX(pPlayer);
    int32_t nX = 0;
    int32_t nY = 0;
    D2RoomStrc* pRoom = DUNGEON_FindActSpawnLocationEx(pOp->pGame->pAct[nAct], nLevel, 0, &nX, &nY, nSize);

    D2_ASSERT(pRoom);

    D2CoordStrc pCoord = {};
    pCoord.nX = nX;
    pCoord.nY = nY;

    pRoom = COLLISION_GetFreeCoordinates(pRoom, &pCoord, nSize, 0x1C09, 0);
    if (pRoom && !sub_6FCBDFE0(pOp->pGame, pPlayer, pRoom, pCoord.nX, pCoord.nY, 0, 0))
    {
        FOG_DisplayAssert("fRet", __FILE__, __LINE__);
        exit(-1);
    }

    return 1;
}

//D2Game.0x6FC79490
int32_t __fastcall OBJECTS_OperateFunction23_Waypoint(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    D2RoomStrc* pRoom = UNITS_GetRoom(pObject);
    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    D2WaypointDataStrc* pWaypointFlags = UNITS_GetPlayerData(pOp->pPlayer)->pWaypointData[pOp->pGame->nDifficulty];
    short nWpNo = 0;
    if (WAYPOINTS_GetWaypointNoFromLevelId(nLevelId, &nWpNo))
    {
        WAYPOINTS_ActivateWaypoint(pWaypointFlags, nWpNo);
    }

    if (pObject->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pOp->nObjectIdx);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
    }
    else if (pObject->dwAnimMode == OBJMODE_OPENED || pObject->dwAnimMode == OBJMODE_OPERATING)
    {
        if (!PLAYER_IsBusy(pOp->pPlayer))
        {
            const uint32_t nFrame = PLAYERLIST_GetHostileDelay(pOp->pPlayer, 0) + 10000;
            if (nFrame > GetTickCount())
            {
                SUNIT_AttachSound(pOp->pPlayer, 0x13, pOp->pPlayer);
                return 0;
            }

            D2GSPacketSrv63 packet63 = {};
            packet63.nHeader = 0x63;
            packet63.dwUnitId = pObject->dwUnitId;
            WAYPOINTS_ValidateAndCopyWaypointData(pWaypointFlags, (D2WaypointDataStrc*)packet63.pData);

            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pOp->pPlayer, __FILE__, __LINE__);
            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet63, sizeof(D2GSPacketSrv63));

            SUNIT_SetInteractInfo(pOp->pPlayer, UNIT_OBJECT, pObject->dwUnitId);
        }
    }

    return 1;
}

//D2Game.0x6FC79600
void __fastcall D2GAME_WAYPOINT_Unk_6FC79600(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nWaypointGUID, int32_t nLevelId)
{
    D2UnitStrc* pWaypoint = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, nWaypointGUID);
    if (pWaypoint)
    {
        D2ObjectsTxt* pObjectsTxtRecord = UNITS_GetObjectTxtRecordFromObject(pWaypoint);
        if (pObjectsTxtRecord)
        {
            const int32_t nOperateFn = pObjectsTxtRecord->nOperateFn;
            if (nOperateFn < std::size(gpObjOperateFnTable) && gpObjOperateFnTable[nOperateFn] == OBJECTS_OperateFunction23_Waypoint)
            {
                const int32_t nWaypointLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pWaypoint));

                SUNIT_ResetInteractInfo(pPlayer);

                if (nWaypointLevelId != nLevelId && nLevelId)
                {
                    D2WaypointDataStrc* v13 = UNITS_GetPlayerData(pPlayer)->pWaypointData[pGame->nDifficulty];
                    int16_t nWaypointNo = 0;
                    if (WAYPOINTS_GetWaypointNoFromLevelId(nLevelId, &nWaypointNo) && WAYPOINTS_IsActivated(v13, nWaypointNo))
                    {
                        int32_t nParam = 0;
                        if (DUNGEON_IsTownLevelId(nLevelId) || nLevelId == LEVEL_CANYONOFTHEMAGI || nLevelId == LEVEL_ARCANESANCTUARY)
                        {
                            nParam = 13;
                        }
                        LEVEL_WarpUnit(pGame, pPlayer, nLevelId, nParam);

                        D2RoomStrc* pRoom = UNITS_GetRoom(pPlayer);
                        int32_t nX = 0;
                        int32_t nY = 0;
                        if (pRoom && pRoom == DUNGEON_FindActSpawnLocation(pGame->pAct[DRLG_GetActNoFromLevelId(nLevelId)], nLevelId, nParam, &nX, &nY))
                        {
                            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
                            D2CoordStrc pCoord = {};
                            UNITS_GetCoords(pPlayer, &pCoord);
                            sub_6FC817D0(pGame, pPlayer, 0, 2, pCoord.nX, pCoord.nY, 0);
                            D2GAME_PACKETS_SendPacket0x0D_6FC3C920(pClient, 0xDu, 0, pPlayer ? pPlayer->dwUnitId : -1, 1, pCoord.nX + 3, pCoord.nY + 3, 0, 0);
                        }

                        OBJRGN_AllocObjectRoomCoords(pGame, pPlayer, pRoom);
                    }
                }
            }
        }
    }
}

//D2Game.0x6FC797A0
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseMana(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue)
{
    const int32_t nNewValue = (nValue << 8) + STATLIST_GetMaxManaFromUnit(pUnit);
    STATLIST_SetUnitStat(pUnit, STAT_MANA, nNewValue, 0);
    STATLIST_SetUnitStat(pUnit, STAT_MAXMANA, nNewValue, 0);
    return 1;
}

//D2Game.0x6FC797E0
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseEnergy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue)
{
    STATLIST_AddUnitStat(pUnit, STAT_ENERGY, nValue, 0);
    return 1;
}

//D2Game.0x6FC79800
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseDexterity(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue)
{
    STATLIST_AddUnitStat(pUnit, STAT_DEXTERITY, nValue, 0);
    return 1;
}

//D2Game.0x6FC79820
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseVitality(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue)
{
    STATLIST_AddUnitStat(pUnit, STAT_VITALITY, nValue, 0);
    return 1;
}

//D2Game.0x6FC79840
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseStrength(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue)
{
    STATLIST_AddUnitStat(pUnit, STAT_STRENGTH, nValue, 0);
    return 1;
}

//D2Game.0x6FC79860
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseHitpoints(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue)
{
    const int32_t nNewValue = (nValue << 8) + STATLIST_GetMaxLifeFromUnit(pUnit);
    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewValue, 0);
    STATLIST_SetUnitStat(pUnit, STAT_MAXHP, nNewValue, 0);
    return 1;
}

//D2Game.0x6FC798A0
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseSkillpoints(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue)
{
    STATLIST_AddUnitStat(pUnit, STAT_SKILLPTS, nValue, 0);
    return 1;
}

//D2Game.0x6FC798C0
int32_t __fastcall OBJMODE_MainObeliskHandler(D2GameStrc* pGame, int32_t nPlayerGUID, int32_t nObjectGUID, int32_t nItemGUID, int16_t nState)
{
    constexpr D2ObeliskPowerUpStrc gpObeliskPowerUpTable[21] =
    {
        { OBJMODE_ObeliskPowerUp_IncreaseMana, 100, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseMana, 100, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseMana, 100, 2 },
        { OBJMODE_ObeliskPowerUp_IncreaseEnergy, 5, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseEnergy, 10, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseEnergy, 15, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseDexterity, 5, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseDexterity, 10, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseDexterity, 15, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseVitality, 5, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseVitality, 10, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseVitality, 15, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseStrength, 5, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseStrength, 10, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseStrength, 15, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseHitpoints, 100, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseHitpoints, 100, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseHitpoints, 100, 2 },
        { OBJMODE_ObeliskPowerUp_IncreaseSkillpoints, 3, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseSkillpoints, 6, 1 },
        { OBJMODE_ObeliskPowerUp_IncreaseSkillpoints, 10, 1 },
    };

    D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, nObjectGUID);
    D2UnitStrc* pPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nPlayerGUID);
    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);

    if (!pPlayer)
    {
        if (!pObject)
        {
            FOG_DisplayAssert("hPlayer != nullptr || hObject != nullptr", __FILE__, __LINE__);
            exit(-1);
        }

        return 1;
    }

    if (!pObject)
    {
        return 1;
    }

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);

    D2GSPacketSrv58 packet58 = {};
    packet58.nHeader = 0x58;
    packet58.nUnitId = nObjectGUID;

    if (nState == 2)
    {
        packet58.unk0x05 = 1;
        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet58, sizeof(packet58));

        UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
        SUNIT_ResetInteractInfo(pPlayer);
    }
    else if (nState == 3)
    {
        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);

        bool bFail = false;
        switch (pObject->dwClassId)
        {
        case OBJECT_ORIFICE:
            if (pItemsTxtRecord->dwCode != ' tsh')
            {
                bFail = true;
            }
            break;

        default:
            if (!D2GAME_RemoveItemIfOnCursor_6FC49760(pGame, pPlayer, pItem))
            {
                bFail = true;
            }
            break;
        }

        if (bFail)
        {
            packet58.unk0x05 = 4;
            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet58, sizeof(packet58));
            return 1;
        }

        packet58.unk0x05 = 5;
        SUNIT_ResetInteractInfo(pPlayer);

        if (pObject->dwClassId == OBJECT_ORIFICE)
        {
            packet58.unk0x06 = 1;
        }
        else
        {
            const uint8_t nSubType = pItemsTxtRecord->nSubType;

            if (nSubType < std::size(gpObeliskPowerUpTable) && ITEMS_RollRandomNumber(&pPlayer->pSeed) % 100 < gpObeliskPowerUpTable[nSubType].nChance)
            {
                if (IsBadCodePtr((FARPROC)gpObeliskPowerUpTable[nSubType].pPowerUpCallback))
                {
                    FOG_DisplayAssert("sgObeliskPowerup[bSubType].pPowerup", __FILE__, __LINE__);
                    exit(-1);
                }

                packet58.unk0x06 = gpObeliskPowerUpTable[nSubType].pPowerUpCallback(pGame, pPlayer, gpObeliskPowerUpTable[nSubType].nValue);
            }
        }

        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet58, sizeof(packet58));

        if (packet58.unk0x06 == 1)
        {
            UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

            D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pObject->dwClassId);
            if (pObject->dwClassId == OBJECT_ORIFICE)
            {
                UNITS_ChangeAnimMode(pObject, OBJMODE_OPENED);
                ACT2Q6_DeleteAllHoradricItemsAndOpenTomb(pGame, pPlayer, pObject);
            }
            else
            {
                EVENT_SetEvent(pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pGame->dwGameFrame + 1, 0, 0);
            }
        }
        else
        {
            UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
        }
    }

    return 1;
}

//D2Game.0x6FC79B50
void __fastcall sub_6FC79B50(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pObject)
{
    if (pObject)
    {
        switch (pObject->dwClassId)
        {
        case OBJECT_OBELISK:
        case OBJECT_DESERT_OBELISK:
        {
            OBJMODE_MainObeliskHandler(pGame, pUnit ? pUnit->dwUnitId : -1, pObject->dwUnitId, 0, 2);
            break;
        }
        case OBJECT_STEEG_STONE:   
        {
            D2TimerArg2Strc* pArg = UNITS_GetTimerArg(pObject)->unk0x00;
            if (pArg && (!pUnit || SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pArg->nUnitGUID) == pUnit))
            {
                pArg->nUnitGUID = -1;
            }
            break;
        }
        case OBJECT_ORIFICE:
        {
            UNITS_ChangeAnimMode(pObject, OBJMODE_NEUTRAL);
            break;
        }
        }
    }

    SUNIT_ResetInteractInfo(pUnit);
}

//D2Game.0x6FC79C00
void __fastcall sub_6FC79C00(D2GameStrc* pGame, D2UnitStrc* pObject)
{
    if (pObject && pObject->dwClassId == OBJECT_STEEG_STONE)
    {
        D2TimerArg2Strc* pArg = UNITS_GetTimerArg(pObject)->unk0x00;
        if (pArg)
        {
            pArg->nUnitGUID = -1;
        }
    }
    else
    {
        UNITS_ChangeAnimMode(pObject, 0);
    }
}

//D2Game.0x6FC79C30
void __fastcall sub_6FC79C30(D2GameStrc* pGame, int32_t nUnused, int32_t nPortalGUID, int32_t nDestLevel)
{
    D2_ASSERT(pGame);

    D2UnitStrc* pPortal = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, nPortalGUID);
    if (!pPortal || pPortal->dwClassId != OBJECT_TOWN_PORTAL || UNITS_CheckObjectPortalFlag(pPortal, 1u))
    {
        return;
    }

    D2UnitStrc* pOwner = SUNIT_GetPortalOwner(pGame, pPortal);
    if (!pOwner)
    {
        return;
    }

    D2_ASSERT(pOwner->dwClassId == OBJECT_TOWN_PORTAL);
    D2_ASSERT((uint8_t)nDestLevel <= 255);

    pPortal->pObjectData->InteractType = nDestLevel;

    UNITS_SetObjectPortalFlags(pPortal, UNITS_GetObjectPortalFlags(pPortal) | 1);
    UNITS_ChangeAnimMode(pPortal, OBJMODE_SPECIAL1);
    DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pOwner), UNIT_OBJECT, pOwner->dwUnitId);
    SUNIT_RemoveUnit(pGame, pOwner);

    D2UnitStrc* pOtherPortal = D2GAME_CreateLinkPortal_6FD13B20(pGame, pPortal, nDestLevel, DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pPortal)));
    D2_ASSERT(pOtherPortal);
    UNITS_SetObjectPortalFlags(pOtherPortal, UNITS_GetObjectPortalFlags(pOtherPortal) | 3);
}

//D2Game.0x6FC79D90
int32_t __fastcall OBJECTS_OperateFunction22_Well(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject || !pOp->pPlayer)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;
    D2UnitStrc* pPlayer = pOp->pPlayer;

    D2ObjectsTxt* pObjectsTxtRecord = UNITS_GetObjectTxtRecordFromObject(pObject);

    uint8_t nType = pObject->pObjectData->InteractType;

    if (nType)
    {
        int32_t bUsed = 0;

        const int32_t nMaxLife = STATLIST_GetMaxLifeFromUnit(pPlayer);
        const int32_t nLife = STATLIST_GetUnitStatUnsigned(pPlayer, STAT_HITPOINTS, 0);
        if (nLife < nMaxLife && pObjectsTxtRecord->dwParm[3] & 2)
        {
            int32_t nNewLife = nLife + (nMaxLife * pObjectsTxtRecord->dwParm[1] >> 8);
            if (nNewLife > nMaxLife)
            {
                nNewLife = nMaxLife;
            }
            STATLIST_SetUnitStat(pPlayer, STAT_HITPOINTS, nNewLife, 0);
            D2GAME_UpdateAttribute_6FC822D0(pPlayer, STAT_HITPOINTS, nNewLife, pPlayer);
            bUsed = 1;
        }

        const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pPlayer);
        const int32_t nMana = STATLIST_GetUnitStatUnsigned(pPlayer, STAT_MANA, 0);
        if (nMana < nMaxMana && pObjectsTxtRecord->dwParm[3] & 1)
        {
            int32_t nNewMana = nMana + (nMaxMana * pObjectsTxtRecord->dwParm[1] >> 8);
            if (nNewMana > nMaxMana)
            {
                nNewMana = nMaxMana;
            }
            STATLIST_SetUnitStat(pPlayer, STAT_MANA, nNewMana, 0);
            D2GAME_UpdateAttribute_6FC822D0(pPlayer, STAT_MANA, nNewMana, pPlayer);
            bUsed = 1;
        }

        const int32_t nMaxStamina = STATLIST_GetMaxStaminaFromUnit(pPlayer);
        const int32_t nStamina = STATLIST_GetUnitStatUnsigned(pPlayer, STAT_STAMINA, 0);
        if (nStamina < nMaxStamina)
        {
            int32_t nNewStamina = nStamina + (nMaxStamina * pObjectsTxtRecord->dwParm[1] >> 8);
            if (nNewStamina > nMaxStamina)
            {
                nNewStamina = nMaxStamina;
            }
            STATLIST_SetUnitStat(pPlayer, STAT_STAMINA, nNewStamina, 0);
            D2GAME_UpdateAttribute_6FC822D0(pPlayer, STAT_STAMINA, nNewStamina, pPlayer);
            bUsed = 1;
        }

        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pPlayer, STATE_POISON);
        if (pStatList)
        {
            D2Common_10474(pPlayer, pStatList);
            STATLIST_FreeStatList(pStatList);
            bUsed = 1;
        }

        pStatList = STATLIST_GetStatListFromUnitAndState(pPlayer, STATE_FREEZE);
        if (pStatList)
        {
            D2Common_10474(pPlayer, pStatList);
            STATLIST_FreeStatList(pStatList);
            bUsed = 1;
        }

        if (D2GAME_NPC_RemoveStates_6FCCAF30(pPlayer))
        {
            bUsed = 1;
        }
        PLAYERPETS_IteratePets(pOp->pGame, pPlayer, OBJMODE_PetIterate_Heal, &bUsed);

        if (bUsed)
        {
            --nType;
            if (nType <= 2 * pObjectsTxtRecord->dwParm[2] && !(nType % pObjectsTxtRecord->dwParm[2]))
            {
                UNITS_ChangeAnimMode(pObject, 2 - nType / pObjectsTxtRecord->dwParm[2]);
            }

            pObject->pObjectData->InteractType = nType;
            EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_AITHINK, pOp->pGame->dwGameFrame + pObjectsTxtRecord->dwParm[0] + 1, 0, 0);
        }
    }

    return 1;
}

//D2Game.0x6FC7A000
void __fastcall OBJMODE_PetIterate_Heal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pUnit2, void* pUnitHealed)
{
    const int32_t nHitpoints = STATLIST_GetUnitStatUnsigned(pUnit2, STAT_HITPOINTS, 0);
    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit2);
    if (nHitpoints < nMaxHp)
    {
        STATLIST_SetUnitStat(pUnit2, STAT_HITPOINTS, nMaxHp, 0);
        *(uint8_t*)pUnitHealed = 1;
    }

    D2StatListStrc* pPoisonStatList = STATLIST_GetStatListFromUnitAndState(pUnit2, STATE_POISON);
    if (pPoisonStatList)
    {
        D2Common_10474(pUnit2, pPoisonStatList);
        STATLIST_FreeStatList(pPoisonStatList);
    }

    D2StatListStrc* pFreezeStatList = STATLIST_GetStatListFromUnitAndState(pUnit2, STATE_FREEZE);
    if (pFreezeStatList)
    {
        D2Common_10474(pUnit2, pFreezeStatList);
        STATLIST_FreeStatList(pFreezeStatList);
    }

    D2GAME_NPC_RemoveStates_6FCCAF30(pUnit2);
}

//D2Game.0x6FC7A070
int32_t __fastcall OBJECTS_OperateFunction48_TrappedSoul(D2ObjOperateFnStrc* pOp, int32_t nOperate)
{
    if (!pOp || !pOp->pObject)
    {
        return 0;
    }

    D2UnitStrc* pObject = pOp->pObject;

    if (pObject->dwAnimMode != OBJMODE_NEUTRAL)
    {
        return 0;
    }

    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(pObject->dwClassId);

    if (ITEMS_RollRandomNumber(&pOp->pObjectregion->pSeed) % 100 >= 90)
    {
        UNITS_ChangeAnimMode(pObject, OBJMODE_OPERATING);

        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_ENDANIM, (pObjectsTxtRecord->dwFrameCnt[1] >> 8) + pOp->pGame->dwGameFrame + 1, 0, 0);
        EVENT_SetEvent(pOp->pGame, pObject, UNITEVENTCALLBACK_PERIODICSTATS, pOp->pGame->dwGameFrame + 20, 0, 0);
    }
    else
    {
        if (OBJMODE_DropFromChestTCWithQuality(pOp, 0))
        {
            UNITS_ChangeAnimMode(pObject, OBJMODE_SPECIAL3);
        }
    }

    return 1;
}

//D2Game.0x6FC7A140
D2UnitStrc* __fastcall OBJMODE_DropItemWithCodeAndQuality(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t dwCode, int32_t nItemQuality)
{
    const int32_t nItemId = DATATBLS_GetItemIdFromItemCode(dwCode);
    if (nItemId == -1)
    {
        return 0;
    }

    D2CoordStrc pCoord = {};
    UNITS_GetCoords(pUnit, &pCoord);

    D2CoordStrc pReturnCoords = {};
    D2RoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pUnit), &pCoord, &pReturnCoords, 1);
    if (!pRoom)
    {
        return 0;
    }

    D2ItemDropStrc itemDrop = {};
    itemDrop.pSeed = nullptr;
    itemDrop.nX = pReturnCoords.nX;
    itemDrop.nY = pReturnCoords.nY;
    itemDrop.wItemFormat = pGame->wItemFormat;
    itemDrop.pRoom = pRoom;
    itemDrop.nQuality = nItemQuality;
    itemDrop.pUnit = pUnit;
    itemDrop.pGame = pGame;
    itemDrop.nId = nItemId;
    itemDrop.nSpawnType = 3;
    itemDrop.wUnitInitFlags = 1;
    itemDrop.nItemLvl = ITEMS_GetItemLevelForNewItem(pUnit, 0);

    return D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
}

//D2Game.0x6FC7A220
D2UnitStrc* __fastcall OBJMODE_DropFromChestTCWithQuality(D2ObjOperateFnStrc* pOp, int32_t nItemQuality)
{
    constexpr int32_t nLevelIds[] =
    {
        2, 37,
        41, 73,
        76, 102,
        104, 108,
        109, 132
    };

    D2_ASSERT(pOp->pObject);

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pOp->pObject);

    D2_ASSERT(pGame);
    D2_ASSERT(UNITS_GetObjectTxtRecordFromObject(pOp->pObject));

    D2RoomStrc* pRoom = UNITS_GetRoom(pOp->pObject);
    if (!pRoom)
    {
        return nullptr;
    }

    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    const uint8_t nAct = DRLG_GetActNoFromLevelId(nLevelId);

    const int32_t nCurrentMonsterLevel = DATATBLS_GetMonsterLevelInArea(nLevelId, pGame->nDifficulty, pGame->bExpansion);
    const int32_t nMinMonsterLevel = DATATBLS_GetMonsterLevelInArea(nLevelIds[2 * nAct], pGame->nDifficulty, pGame->bExpansion);
    const int32_t nMaxMonsterLevel = DATATBLS_GetMonsterLevelInArea(nLevelIds[2 * nAct + 1], pGame->nDifficulty, pGame->bExpansion);
    
    int32_t nMonsterLevelDiff = nMaxMonsterLevel - nMinMonsterLevel;
    nMonsterLevelDiff = std::abs(nMonsterLevelDiff);

    const int32_t nOffset = (nMonsterLevelDiff + 1) / 3;
    int32_t nTCIndex = 0;
    if (nCurrentMonsterLevel >= nOffset + nMinMonsterLevel)
    {
        nTCIndex = (nCurrentMonsterLevel >= nMinMonsterLevel + 2 * nOffset) + 1;
    }

    D2TCExShortStrc* pTCExRecord = DATATBLS_GetTreasureClassExRecordFromActAndDifficulty(pGame->nDifficulty, nAct, nTCIndex);
    if (!pTCExRecord)
    {
        return nullptr;
    }

    int32_t nItemDropCount = 0;
    D2UnitStrc* ppDroppableItems[6] = {};
    D2GAME_DropTC_6FC51360(pGame, pOp->pObject, pOp->pPlayer, pTCExRecord, nItemQuality, nTCIndex, 0, ppDroppableItems, &nItemDropCount, 6);
    return ppDroppableItems[0];
}
