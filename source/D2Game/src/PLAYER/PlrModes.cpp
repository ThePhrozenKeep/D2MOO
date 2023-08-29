#include "PLAYER/PlrModes.h"

#include <algorithm>

#include <D2BitManip.h>

#include <D2Chat.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <Path/PathMisc.h>
#include <Units/UnitRoom.h>

#include "AI/AiGeneral.h"
#include "GAME/Arena.h"
#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "INVENTORY/InvMode.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterUnique.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerList.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlrMsg.h"
#include "PLAYER/PlrTrade.h"
#include "PLAYER/PlrSave.h"
#include "PLAYER/PlrSave2.h"
#include "QUESTS/Quests.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitMsg.h"
#include "UNIT/Party.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/Skills.h"
#include "SKILLS/SkillSor.h"


#pragma warning(disable: 28159)


#pragma pack(push, 1)
struct D2PlrModeCallbackStrc
{
    void(__fastcall* pfnStartXY)(D2GameStrc*, D2UnitStrc*, int32_t, int32_t, int32_t);
    void(__fastcall* pfnStartID)(D2GameStrc*, D2UnitStrc*, int32_t, D2UnitStrc*);
};
#pragma pack(pop)

constexpr D2PlrModeCallbackStrc stru_6FD28EA0[] =
{
    { nullptr,                                              PLRMODE_StartID_Death },
    { PLRMODE_StartXY_Neutral,                              nullptr },
    { PLRMODE_StartXY_WalkRunKnockback,                     PLRMODE_StartID_WalkRunKnockback },
    { PLRMODE_StartXY_WalkRunKnockback,                     PLRMODE_StartID_WalkRunKnockback },
    { PLRMODE_StartXY_GetHit,                               nullptr },
    { PLRMODE_StartXY_Neutral,                              nullptr },
    { PLRMODE_StartXY_WalkRunKnockback,                     PLRMODE_StartID_WalkRunKnockback },
    { PLRMODE_StartXY_AttackCastThrowKickSpecialSequence,   PLRMODE_StartID_AttackCastThrowKickSpecialSequence },
    { PLRMODE_StartXY_AttackCastThrowKickSpecialSequence,   PLRMODE_StartID_AttackCastThrowKickSpecialSequence },
    { PLRMODE_StartXY_Block,                                nullptr },
    { PLRMODE_StartXY_AttackCastThrowKickSpecialSequence,   PLRMODE_StartID_AttackCastThrowKickSpecialSequence },
    { PLRMODE_StartXY_AttackCastThrowKickSpecialSequence,   PLRMODE_StartID_AttackCastThrowKickSpecialSequence },
    { PLRMODE_StartXY_AttackCastThrowKickSpecialSequence,   PLRMODE_StartID_AttackCastThrowKickSpecialSequence },
    { PLRMODE_StartXY_AttackCastThrowKickSpecialSequence,   PLRMODE_StartID_AttackCastThrowKickSpecialSequence },
    { PLRMODE_StartXY_AttackCastThrowKickSpecialSequence,   PLRMODE_StartID_AttackCastThrowKickSpecialSequence },
    { PLRMODE_StartXY_AttackCastThrowKickSpecialSequence,   PLRMODE_StartID_AttackCastThrowKickSpecialSequence },
    { PLRMODE_StartXY_AttackCastThrowKickSpecialSequence,   PLRMODE_StartID_AttackCastThrowKickSpecialSequence },
    { PLRMODE_StartXY_Dead,                                 nullptr },
    { PLRMODE_StartXY_AttackCastThrowKickSpecialSequence,   PLRMODE_StartID_AttackCastThrowKickSpecialSequence },
    { PLRMODE_StartXY_WalkRunKnockback,                     PLRMODE_StartID_WalkRunKnockback },
};


//D2Game.0x6FC7F340
int32_t __fastcall D2GAME_PLRMODES_First_6FC7F340(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMode, int32_t a4, D2SkillStrc* pSkill)
{
    if (STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE) || !pUnit || pUnit->dwAnimMode == PLRMODE_DEATH || pUnit->dwAnimMode == PLRMODE_DEAD)
    {
        return 0;
    }

    if (nMode == PLRMODE_DEATH || !a4)
    {
        return 1;
    }

    D2SkillStrc* pUsedSkill = UNITS_GetUsedSkill(pUnit);
    if (!pUsedSkill)
    {
        return 1;
    }

    const int32_t nUsedSkillId = SKILLS_GetSkillIdFromSkill(pUsedSkill, __FILE__, __LINE__);

    D2SkillsTxt* pUsedSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nUsedSkillId);
    if (!pUsedSkillsTxtRecord)
    {
        return 1;
    }

    if (pSkill == pUsedSkill && (pUsedSkillsTxtRecord->wSrvDoFunc == 67 || pUsedSkillsTxtRecord->wSrvDoFunc == 76))
    {
        return 1;
    }

    if (!(pUsedSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_INTERRUPT]))
    {
        if (pUnit->dwAnimMode == PLRMODE_NEUTRAL)
        {
            sub_6FC817D0(pGame, pUnit, nullptr, PLRMODE_NEUTRAL, 0, 0, 1);
            return 1;
        }

        switch (nMode)
        {
        case PLRMODE_ATTACK1:
        case PLRMODE_ATTACK2:
        case PLRMODE_CAST:
        case PLRMODE_THROW:
        case PLRMODE_SPECIAL1:
        case PLRMODE_SEQUENCE:
            if (pGame->dwGameFrame <= EVENT_GetEventFrame(pGame, pUnit, UNITEVENTCALLBACK_ENDANIM) + 5)
            {
                return 1;
            }
            break;

        default:
            return 0;
        }

        return 0;
    }

    if ((STATES_CheckState(pUnit, STATE_CONCENTRATION)
        && (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < STATLIST_GetStatValue(STATLIST_GetStatListFromUnitAndState(pUnit, STATE_CONCENTRATION), STAT_SKILL_CONCENTRATION, 0))
        || STATES_CheckState(pUnit, STATE_CONCENTRATE))
    {
        if (pUnit->dwAnimMode == PLRMODE_NEUTRAL)
        {
            sub_6FC817D0(pGame, pUnit, nullptr, PLRMODE_NEUTRAL, 0, 0, 1);
            return 1;
        }

        return 0;
    }

    return 1;
}

//D2Game.0x6FC7F550
void __fastcall PLRMODE_StartXY_Neutral(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, int32_t nX, int32_t nY)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pPlayer);
    int32_t nCombatMode = nMode;

    if (pRoom)
    {
        nCombatMode = DUNGEON_IsRoomInTown(pRoom) != 0 ? PLRMODE_TNEUTRAL : PLRMODE_NEUTRAL;
    }

    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pPlayer);
    SUNIT_SetCombatMode(pGame, pPlayer, nCombatMode);
    UNITS_SetUsedSkill(pPlayer, 0);
}

//D2Game.0x6FC7F5A0
void __fastcall PLRMODE_StartID_WalkRunKnockback(D2GameStrc* pGame, D2UnitStrc* a2, int32_t a3, D2UnitStrc* pTargetUnit)
{
    D2_ASSERT(pGame);

    if (a3 != 19 || a2->dwAnimMode != 19)
    {
        PATH_SetTargetUnit(a2->pDynamicPath, pTargetUnit);
        sub_6FC7F600(pGame, a2, a3);
    }
}

//D2Game.0x6FC7F600
void __fastcall sub_6FC7F600(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode)
{
    D2_ASSERT(pGame);

    PATH_ResetToPreviousType(pPlayer->pDynamicPath);

    int32_t nCombatMode = nMode;
    switch (nMode)
    {
    case PLRMODE_RUN:
        if (!STATLIST_UnitGetStatValue(pPlayer, STAT_STAMINA, 0))
        {
            nCombatMode = DUNGEON_IsRoomInTown(UNITS_GetRoom(pPlayer)) != 0 ? PLRMODE_TWALK : PLRMODE_WALK;
        }
        break;

    case PLRMODE_WALK:
    case PLRMODE_TWALK:
    {
        nCombatMode = DUNGEON_IsRoomInTown(UNITS_GetRoom(pPlayer)) != 0 ? PLRMODE_TWALK : PLRMODE_WALK;
        break;
    }

    case PLRMODE_KNOCKBACK:
    {
        PATH_SetType(pPlayer->pDynamicPath, 8);
        _10190_PATH_SetDistance(pPlayer->pDynamicPath, 5u);
        break;
    }
    default:
        break;
    }

    D2Common_10142(pPlayer->pDynamicPath, pPlayer, 0);
    UNITS_SetUsedSkill(pPlayer, nullptr);

    if (PATH_GetNumberOfPathPoints(pPlayer->pDynamicPath))
    {
        SUNIT_SetCombatMode(pGame, pPlayer, nCombatMode);
        D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pPlayer);
        sub_6FCBD3A0(pGame, pPlayer);
    }
    else
    {
        PLRMODE_StartXY_Neutral(pGame, pPlayer, PLRMODE_NEUTRAL, 0, 0);
    }
}

//D2Game.0x6FC7F730
void __fastcall PLRMODE_StartXY_WalkRunKnockback(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, int32_t nX, int32_t nY)
{
    D2_ASSERT(pGame);

    D2COMMON_10170_PathSetTargetPos(pPlayer->pDynamicPath, nX, nY);
    sub_6FC7F600(pGame, pPlayer, nMode);
}

//D2Game.0x6FC7F780
int32_t __fastcall sub_6FC7F780(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t a3, int32_t a4)
{
    // TODO: Names

    sub_6FCBC7E0(pGame, pPlayer);

    if (STATES_CheckState(pPlayer, STATE_BLAZE))
    {
        SKILLS_CreateBlazeMissile(pGame, pPlayer);
    }

    if (pPlayer && pPlayer->dwAnimMode == PLRMODE_RUN && !DUNGEON_IsRoomInTown(UNITS_GetRoom(pPlayer)))
    {
        D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pPlayer->dwClassId);
        if (!pCharStatsTxtRecord || !pPlayer->pInventory)
        {
            sub_6FC7F600(pGame, pPlayer, PLRMODE_WALK);
        }
        else
        {
            int32_t nStaminaLost = 2 * pCharStatsTxtRecord->nRunDrain;

            D2UnitStrc* pTorsoArmor = INVENTORY_GetItemFromBodyLoc(pPlayer->pInventory, BODYLOC_TORSO);
            if (pTorsoArmor)
            {
                D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pTorsoArmor->dwClassId);
                if (pItemsTxtRecord)
                {
                    nStaminaLost *= pItemsTxtRecord->dwSpeed / 10 + 1;
                }
            }

            const int32_t nStaminaDrainPct = STATLIST_UnitGetItemStatOrSkillStatValue(pPlayer, STAT_ITEM_STAMINADRAINPCT, 0);
            if (nStaminaDrainPct)
            {
                nStaminaLost += nStaminaLost * nStaminaDrainPct / -100;
            }

            if (nStaminaLost < 1)
            {
                nStaminaLost = 1;
            }

            STATLIST_AddUnitStat(pPlayer, STAT_STAMINA, -nStaminaLost, 0);

            if (STATLIST_UnitGetStatValue(pPlayer, STAT_STAMINA, 0) <= 0)
            {
                STATLIST_SetUnitStat(pPlayer, STAT_STAMINA, 0, 0);
                sub_6FC7F600(pGame, pPlayer, PLRMODE_WALK);
            }
        }
    }

    const int32_t v44 = sub_6FCBC930(pGame, pPlayer);
    const uint32_t nTickCount = GetTickCount();
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    if (nTickCount > pPlayerData->unk0xA4 + 25)
    {
        const uint8_t v21 = pPlayerData->unk0xA0;
        const uint8_t v47 = pPlayerData->unk0xA0;
        uint8_t v46 = 19;
        if (v21)
        {
            v46 = v21 - 1;
        }

        const int32_t v23 = CLIENTS_GetUnitX(pPlayer);
        const int32_t v25 = CLIENTS_GetUnitY(pPlayer);
        if (PATH_ComputeSquaredDistance(v23, v25, pPlayerData->unk0xA8[2 * v46], pPlayerData->unk0xA8[2 * v46 + 1]) > 45)
        {
            pPlayerData->unk0xA8[2 * v47] = v23;
            pPlayerData->unk0xA8[2 * v47 + 1] = v25;
            pPlayerData->unk0xA4 = nTickCount;

            ++pPlayerData->unk0xA0;
            if (pPlayerData->unk0xA0 >= 20)
            {
                pPlayerData->unk0xA0 = 0;
            }
        }
    }

    if (v44 != 2)
    {
        return v44;
    }

    if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER || !pPlayer->pPlayerData || !pPlayer->pPlayerData->unk0xA8[42])
    {
        PLRMODE_StartXY_Neutral(pGame, pPlayer, 1, 0, 0);
        return v44;
    }

    const int32_t v28 = pPlayer->pPlayerData->unk0xA8[43];
    if (v28 < 0)
    {
        int32_t nMode = PLRMODE_NEUTRAL;
        D2RoomStrc* pRoom = UNITS_GetRoom(pPlayer);
        if (pRoom && DUNGEON_IsRoomInTown(pRoom))
        {
            nMode = PLRMODE_TNEUTRAL;
        }

        D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pPlayer);
        SUNIT_SetCombatMode(pGame, pPlayer, nMode);
        UNITS_SetUsedSkill(pPlayer, 0);
        sub_6FC828D0(pPlayer, sub_6FC7FB90(pPlayer), sub_6FC7FBB0(pPlayer), v28 == -2, pGame);
        sub_6FC7FB70(pPlayer, 0);
        return v44;
    }

    int32_t nResult = 0;
    D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pPlayer, v28);
    if (pSkill)
    {
        const int32_t nSkillMode = SKILLS_GetSkillModeFromUnit(pPlayer, pSkill);
        if (pPlayer->dwUnitType == UNIT_PLAYER && pPlayer->pPlayerData)
        {
            D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pPlayer, pSkill, nSkillMode, pPlayer->pPlayerData->unk0xA8[44], pPlayer->pPlayerData->unk0xA8[45], 0);
        }
        else
        {
            D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pPlayer, pSkill, nSkillMode, 0, 0, 0);
        }

        nResult = 1;
    }
    else
    {
        int32_t nMode = PLRMODE_NEUTRAL;
        D2RoomStrc* pRoom = UNITS_GetRoom(pPlayer);
        if (pRoom && DUNGEON_IsRoomInTown(pRoom))
        {
            nMode = PLRMODE_TNEUTRAL;
        }

        D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pPlayer);
        SUNIT_SetCombatMode(pGame, pPlayer, nMode);
        UNITS_SetUsedSkill(pPlayer, 0);
        nResult = v44;
    }

    if (pPlayer->dwUnitType == UNIT_PLAYER && pPlayer->pPlayerData)
    {
        pPlayer->pPlayerData->unk0xA8[42] = 0;
    }

    return nResult;
}

//D2Game.0x6FC7FB70
void __fastcall sub_6FC7FB70(D2UnitStrc* pPlayer, int32_t a2)
{
    if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER && pPlayer->pPlayerData)
    {
        pPlayer->pPlayerData->unk0xA8[42] = a2;
    }
}

//D2Game.0x6FC7FB90
int32_t __fastcall sub_6FC7FB90(D2UnitStrc* pPlayer)
{
    if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER && pPlayer->pPlayerData)
    {
        return pPlayer->pPlayerData->unk0xA8[44];
    }

    return 0;
}

//D2Game.0x6FC7FBB0
int32_t __fastcall sub_6FC7FBB0(D2UnitStrc* pPlayer)
{
    if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER && pPlayer->pPlayerData)
    {
        return pPlayer->pPlayerData->unk0xA8[45];
    }

    return 0;
}

//D2Game.0x6FC7FBD0
D2UnitStrc* __fastcall D2GAME_CORPSE_Handler_6FC7FBD0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, D2RoomStrc* pRoom)
{
    if (pUnit->pInventory)
    {
        D2CorpseStrc* pCorpse = INVENTORY_GetFirstCorpse(pUnit->pInventory);
        while (pCorpse)
        {
            D2CorpseStrc* pNextCorpse = INVENTORY_GetNextCorpse(pCorpse);
            const int32_t nCorpseGUID = INVENTORY_GetUnitGUIDFromCorpse(pCorpse);
            if (!SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nCorpseGUID))
            {
                INVENTORY_FreeCorpse(pUnit->pInventory, nCorpseGUID, 1);
            }
            pCorpse = pNextCorpse;
        }
    }

    if (INVENTORY_GetCorpseCount(pUnit->pInventory) > 15)
    {
        for (int32_t nBodyLoc = -1; nBodyLoc < 13; ++nBodyLoc)
        {
            D2UnitStrc* pItem = nullptr;
            if (nBodyLoc == -1)
            {
                pItem = INVENTORY_GetCursorItem(pUnit->pInventory);
            }
            else
            {
                pItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
            }

            if (pItem)
            {
                if (nBodyLoc == -1)
                {
                    sub_6FC803F0(pGame, pItem, pUnit);
                }
                else
                {
                    if (pItem->dwAnimMode != IMODE_EQUIP)
                    {
                        FOG_DisplayAssert("UnitGetMode(hItem) == ITEM_MODE_INVBODY", __FILE__, __LINE__);
                        exit(-1);
                    }
                    sub_6FC802F0(pGame, nBodyLoc, pItem, pUnit);
                }

                D2UnitStrc* pDupedItem = ITEMS_Duplicate(pGame, pItem, pUnit, 1);
                D2CoordStrc pCoord = {};
                UNITS_GetCoords(pUnit, &pCoord);
                D2CoordStrc pReturnCoords = {};
                D2GAME_DropItem_6FC52260(pGame, pUnit, pDupedItem, D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pUnit), &pCoord, &pReturnCoords, 1), pReturnCoords.nX, pReturnCoords.nY);
            }
        }

        const int32_t nGold = STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0);
        D2GAME_SetStatOrResetGold_6FC7CA70(pUnit, STAT_GOLD, 0);
        sub_6FC7C260(pGame, pUnit, pUnit->dwUnitId, nGold);
        return nullptr;
    }

    D2RoomStrc* pTargetRoom = D2GAME_GetRoom_6FC52070(pRoom, nX, nY);
    if (!pTargetRoom)
    {
        char szMessage[256] = {};
        FOG_DisplayAssert(FOG_csprintf(szMessage, "Server tried to place a player in an invalid room %d %d", nX, nY), __FILE__, __LINE__);
        exit(-1);
    }

    D2UnitStrc* pDeadBody = SUNIT_AllocUnitData(UNIT_PLAYER, pUnit->dwClassId, nX, nY, pGame, pTargetRoom, 1, PLRMODE_DEAD, 0);
    D2_ASSERT(pDeadBody);

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);

    pDeadBody->dwFlags &= ~UNITFLAG_CANBEATTACKED;
    SUNIT_InitClientInPlayerData(pDeadBody, pClient);

    UNITS_SetNameInPlayerData(pDeadBody, CLIENTS_GetName(pClient));

    SUNIT_InitSeed(pDeadBody, &pGame->pGameSeed);
    UNITS_ChangeAnimMode(pDeadBody, PLRMODE_DEAD);
    STATES_ToggleState(pDeadBody, STATE_PLAYERBODY, 1);

    pDeadBody->dwFlags |= UNITFLAG_TARGETABLE;
    pDeadBody->dwFlagEx |= UNITFLAGEX_ISCORPSE;

    const int32_t nExpLoss = CLIENTS_GetExperienceLoss(pClient);
    CLIENTS_SetExperienceLoss(pClient, 0);
    STATLIST_SetUnitStat(pDeadBody, STAT_EXPERIENCE, 75 * nExpLoss / 100, 0);

    INVENTORY_CreateCorpseForPlayer(pUnit->pInventory, pDeadBody->dwUnitId, 0, 1);

    D2_ASSERT(pDeadBody->dwUnitId != pUnit->dwUnitId);

    INVENTORY_SetOwnerId(pDeadBody->pInventory, pUnit->dwUnitId);

    for (int32_t nBodyLoc = -1; nBodyLoc < 13; ++nBodyLoc)
    {
        D2UnitStrc* pItem = nullptr;
        if (nBodyLoc == -1)
        {
            pItem = INVENTORY_GetCursorItem(pUnit->pInventory);
        }
        else
        {
            pItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
        }

        if (pItem)
        {
            if (nBodyLoc == -1)
            {
                D2GAME_PACKETS_SendPacket0x42_6FC3EE40(pClient, pUnit);

                INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);
                ITEMS_SetItemCMDFlag(pItem, 1, 1);
                INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
                UNITS_RefreshInventory(pUnit, 1);
            }
            else
            {
                if (pItem->dwAnimMode != IMODE_EQUIP)
                {
                    FOG_DisplayAssert("UnitGetMode(hItem) == ITEM_MODE_INVBODY", __FILE__, __LINE__);
                    exit(-1);
                }

                sub_6FC802F0(pGame, nBodyLoc, pItem, pUnit);
            }

            ITEMS_SetInvPage(pItem, INVPAGE_INVENTORY);

            D2UnitStrc* pDupedItem = ITEMS_Duplicate(pGame, pItem, pUnit, 1);
            if (nBodyLoc == -1)
            {
                if (INVENTORY_PlaceItemAtFreePosition(pDeadBody->pInventory, pDupedItem, UNITS_GetInventoryRecordId(pDeadBody, 0, pGame->bExpansion), 0, 0, __FILE__, __LINE__) 
                    && INVENTORY_PlaceItemInSocket(pDeadBody->pInventory, pDupedItem, 1))
                {
                    if (pDupedItem)
                    {
                        pDupedItem->dwFlags &= ~UNITFLAG_TARGETABLE;
                    }

                    UNITS_ChangeAnimMode(pDupedItem, IMODE_STORED);

                    if (pDupedItem)
                    {
                        pDupedItem->dwFlags &= ~UNITFLAG_ISRESURRECT;
                    }

                    ITEMS_SetItemCMDFlag(pDupedItem, 2, 1);
                    INVENTORY_AddItemToTradeInventory(pDeadBody->pInventory, pDupedItem);
                    UNITS_RefreshInventory(pDeadBody, 1);
                    QUESTS_ItemPickedUp(pGame, pDeadBody, pDupedItem);
                }
                else
                {
                    D2CoordStrc pCoord = {};
                    UNITS_GetCoords(pUnit, &pCoord);
                    D2CoordStrc pReturnCoords = {};
                    D2GAME_DropItem_6FC52260(pGame, pUnit, pDupedItem, D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pUnit), &pReturnCoords, &pCoord, 1), pReturnCoords.nX, pReturnCoords.nY);
                }
            }
            else
            {
                if (INVENTORY_PlaceItemInBodyLoc(pDeadBody->pInventory, pDupedItem, nBodyLoc))
                {
                    if (INVENTORY_PlaceItemInSocket(pDeadBody->pInventory, pDupedItem, nBodyLoc == BODYLOC_SWRARM || nBodyLoc == BODYLOC_SWLARM ? 4 : 3))
                    {
                        ITEMS_SetBodyLocation(pDupedItem, nBodyLoc);

                        if (pDupedItem)
                        {
                            pDupedItem->dwFlags &= ~UNITFLAG_TARGETABLE;
                        }

                        UNITS_ChangeAnimMode(pDupedItem, IMODE_EQUIP);
                        ITEMS_SetInvPage(pDupedItem, INVPAGE_NULL);
                        ITEMS_SetItemCMDFlag(pDupedItem, 8, 1);

                        if (pDupedItem)
                        {
                            pDupedItem->dwFlags &= ~UNITFLAG_ISRESURRECT;
                        }

                        INVENTORY_AddItemToTradeInventory(pDeadBody->pInventory, pDupedItem);
                        UNITS_RefreshInventory(pDeadBody, 1);
                        QUESTS_ItemPickedUp(pGame, pDeadBody, pDupedItem);
                    }
                    else
                    {
                        D2CoordStrc pCoord = {};
                        UNITS_GetCoords(pUnit, &pCoord);
                        D2CoordStrc pReturnCoords = {};
                        D2GAME_DropItem_6FC52260(pGame, pUnit, pDupedItem, D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pUnit), &pCoord, &pReturnCoords, 1), pReturnCoords.nX, pReturnCoords.nY);
                    }
                }
            }
        }
    }

    const int32_t nGold = STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0);

    STATLIST_SetUnitStat(pUnit, STAT_GOLD, 0, 0);

    if (nGold > 0)
    {
        D2CoordStrc pCoord = {};
        UNITS_GetCoords(pDeadBody, &pCoord);
        D2UnitStrc* pGold = sub_6FC4FCA0(pGame, UNITS_GetRoom(pDeadBody), &pCoord, 4, 1, pUnit);
        if (pGold)
        {
            D2_ASSERT(ITEMS_GetItemType(pGold) == ITEMTYPE_GOLD);

            D2GAME_SetStatOrResetGold_6FC7CA70(pGold, STAT_GOLD, nGold);

            ITEMS_SetOwnerId(pGold, pUnit->dwUnitId);
        }
    }

    D2GAME_SendPacket0x8E_CorpseAssign_ToAllPlayers_6FC3F9B0(pGame, pUnit->dwUnitId, pDeadBody->dwUnitId, 1);

    return pDeadBody;
}

//D2Game.0x6FC802F0
void __fastcall sub_6FC802F0(D2GameStrc* pGame, int32_t nBodyloc, D2UnitStrc* pItem, D2UnitStrc* pUnit)
{
    INVENTORY_UpdateWeaponGUIDOnRemoval(pUnit->pInventory, pItem);
    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pItem);

    D2_ASSERT(pRemove);
    D2_ASSERT(pItem == pRemove);

    if (ITEMS_GetItemType(pItem) == ITEMTYPE_BELT)
    {
        sub_6FC45930(pGame, pUnit, nullptr);
    }

    if (pItem)
    {
        pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pItem, pUnit, 0, 1);
    ITEMS_SetItemFlag(pItem, IFLAG_QUANTITY, 1);
    ITEMS_SetItemCMDFlag(pItem, 1, 1);
    ITEMS_SetItemCMDFlag(pItem, 16, 1);
    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
    UNITS_RefreshInventory(pUnit, 1);
}

//D2Game.0x6FC803F0
void __fastcall sub_6FC803F0(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pPlayer)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);

    D2GAME_PACKETS_SendPacket0x42_6FC3EE40(pClient, pPlayer);
    INVENTORY_SetCursorItem(pPlayer->pInventory, 0);
    ITEMS_SetItemCMDFlag(pItem, 1, 1);
    INVENTORY_AddItemToTradeInventory(pPlayer->pInventory, pItem);
    UNITS_RefreshInventory(pPlayer, 1);
}

//D2Game.0x6FC80440
void __fastcall sub_6FC80440(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pDeadBody)
{
    if (!STATES_CheckState(pDeadBody, STATE_PLAYERBODY))
    {
        return;
    }

    D2_ASSERT(pPlayer);
    D2_ASSERT(pDeadBody);
    D2_ASSERT(pPlayer->dwUnitType == UNIT_PLAYER);

    const int32_t nOwnerGUID = INVENTORY_GetOwnerId(pDeadBody->pInventory);
    if (pPlayer->dwUnitId != nOwnerGUID)
    {
        D2UnitStrc* pOwner = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nOwnerGUID);
        if (!pOwner || !PLAYERLIST_CheckFlag(pOwner, pPlayer, 1))
        {
            return;
        }
    }

    if (pPlayer->dwUnitId == INVENTORY_GetOwnerId(pDeadBody->pInventory))
    {
        const int32_t nExperience = STATLIST_UnitGetStatValue(pDeadBody, STAT_EXPERIENCE, 0);
        if (nExperience)
        {
            STATLIST_SetUnitStat(pDeadBody, STAT_EXPERIENCE, 0, 0);
            SUNITDMG_AddExperience(pGame, pPlayer, nExperience);
        }
    }

    if (sub_6FC4AD80(pGame, pPlayer, pDeadBody))
    {
        INVENTORY_FreeCorpse(pPlayer->pInventory, pDeadBody->dwUnitId, 1);
        DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pDeadBody), pDeadBody->dwUnitType, pDeadBody->dwUnitId);
        D2GAME_SendPacket0x8E_CorpseAssign_ToAllPlayers_6FC3F9B0(pGame, pPlayer->dwUnitId, pDeadBody->dwUnitId, 0);
        UNITS_FreeCollisionPath(pDeadBody);
        SUNIT_RemoveUnit(pGame, pDeadBody);
        SUNIT_AttachSound(pPlayer, 0x5Du, pPlayer);
    }
    else
    {
        SUNIT_AttachSound(pPlayer, 0x17u, pPlayer);
    }
}

//D2Game.0x6FC805B0
void __fastcall PLRMODE_StartID_Death(D2GameStrc* pGame, D2UnitStrc* pDefender, int32_t a3, D2UnitStrc* pAttacker)
{
    if (SKILLS_RemoveTransformStatesFromShapeshiftedUnit(pDefender))
    {
        sub_6FC817D0(pGame, pDefender, nullptr, 1, 0, 0, 1);
        return;
    }

    PLAYER_ApplyDeathPenalty(pGame, pDefender, pAttacker);
    SUNIT_SetCombatMode(pGame, pDefender, 0);
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pDefender, __FILE__, __LINE__);
    CLIENTS_ToggleFlag(pClient, CLIENTSAVEFLAG_DEAD, TRUE);
    sub_6FCBC4D0(pDefender);
    STATLIST_SetUnitStat(pDefender, STAT_HITPOINTS, 0, 0);
    D2Common_10469(pDefender);
    STATES_UpdateStayDeathFlags(pDefender, 0);
    D2GAME_SetClientDead_6FC33830(pClient, 0);
    PATH_SetUnitDeadCollision(pDefender, 1);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pDefender, 8, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pDefender, 9, 0);
    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pDefender);
    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pDefender);
    sub_6FCBCE70(pGame, pDefender);

    if (!STATES_CheckState(pDefender, STATE_PLAYERBODY) && pDefender)
    {
        pDefender->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    if (pDefender && pAttacker && pDefender->dwUnitType == UNIT_PLAYER && pDefender != pAttacker)
    {
        if (pAttacker->dwUnitType == UNIT_PLAYER || pAttacker->dwUnitType == UNIT_MONSTER)
        {
            D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pAttacker);
            if (pOwner && pOwner->dwUnitType == UNIT_PLAYER && pOwner != pDefender)
            {
                ITEMS_DropPlayerEar(pGame, pDefender);
            }
        }
    }

    if (pAttacker)
    {
        D2GAME_PLRMSG_Last_6FC89450(pGame, pAttacker, pDefender);
        ARENA_ProcessKillEvent(pGame, pAttacker, pDefender);
    }

    PLAYER_StopInteractions(pGame, pDefender);
}

//D2Game.0x6FC80710
void __fastcall PLRMODE_StartXY_Dead(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4, int32_t a5)
{
    D2GAME_CORPSE_Handler_6FC7FBD0(pGame, pUnit, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), UNITS_GetRoom(pUnit));
    D2GAME_KillPlayerPets_6FC7CD10(pGame, pUnit);
    
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    CLIENTS_ToggleFlag(pClient, CLIENTSAVEFLAG_DEAD, TRUE);

    if (!gpD2EventCallbackTable_6FD45830 || CLIENTS_CheckFlag(pClient, CLIENTSAVEFLAG_HARDCORE))
    {
        D2GAME_SAVE_WriteFile_6FC8A500(pGame, pUnit, CLIENTS_GetName(pClient), 0);
    }

    SUNIT_SetCombatMode(pGame, pUnit, PLRMODE_DEAD);
    sub_6FCBC4D0(pUnit);
    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, 0, 0);
    D2Common_10469(pUnit);
    STATES_UpdateStayDeathFlags(pUnit, 0);

    D2GAME_SetClientDead_6FC33830(pClient, 0);

    PATH_SetUnitDeadCollision(pUnit, 1);

    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 8, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 9, 0);

    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pUnit);
    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pUnit);
}

//D2Game.0x6FC80870
void __fastcall PLRMODE_StartXY_Block(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, int32_t nX, int32_t nY)
{
    D2_ASSERT(pPlayer);

    SUNIT_SetCombatMode(pGame, pPlayer, nMode);
    sub_6FCBC4D0(pPlayer);
    UNITS_SetUsedSkill(pPlayer, nullptr);
    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pPlayer);
    sub_6FCBCE70(pGame, pPlayer);
}

//D2Game.0x6FC808D0
int32_t __fastcall sub_6FC808D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nSkillId, int32_t nSkillLevel)
{
    return 1;
}

//D2Game.0x6FC808E0
void __fastcall PLRMODE_StartXY_GetHit(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, int32_t nX, int32_t nY)
{
    D2_ASSERT(pPlayer);

    SUNIT_SetCombatMode(pGame, pPlayer, nMode);
    sub_6FCBC4D0(pPlayer);
    UNITS_SetUsedSkill(pPlayer, 0);
    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pPlayer);
    sub_6FCBCE70(pGame, pPlayer);
}

//D2Game.0x6FC80940
void __fastcall PLRMODE_StartXY_AttackCastThrowKickSpecialSequence(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, int32_t nX, int32_t nY)
{
    D2COMMON_10170_PathSetTargetPos(pPlayer->pDynamicPath, nX, nY);
    SUNIT_SetCombatMode(pGame, pPlayer, nMode);
    UNITS_SetTargetUnitForDynamicUnit(pPlayer, nullptr);
    sub_6FCBC4D0(pPlayer);
    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pPlayer);
    sub_6FCBCE70(pGame, pPlayer);
    pPlayer->dwFlags &= ~UNITFLAG_SKSRVDOFUNC;
    sub_6FD12FD0(pGame, pPlayer);
}

//D2Game.0x6FC809B0
void __fastcall PLRMODE_StartID_AttackCastThrowKickSpecialSequence(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMode, D2UnitStrc* pTarget)
{
    SUNIT_SetCombatMode(pGame, pPlayer, nMode);
    
    D2_ASSERT(pTarget);

    UNITS_SetTargetUnitForDynamicUnit(pPlayer, pTarget);
    sub_6FCBC4D0(pPlayer);
    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pPlayer);
    sub_6FCBCE70(pGame, pPlayer);

    if (pPlayer)
    {
        pPlayer->dwFlags &= 0xFFFFFFBF;
    }

    sub_6FD12FD0(pGame, pPlayer);
}

//D2Game.0x6FC80A30
void __fastcall sub_6FC80A30(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && pUnit->pInventory)
    {
        D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pUnit);
        int32_t nLeftSkillOwnerGUID = -1;
        int32_t nLeftSkillId = 0;
        if (pLeftSkill)
        {
            nLeftSkillOwnerGUID = SKILLS_GetOwnerGUIDFromSkill(pLeftSkill);
            nLeftSkillId = SKILLS_GetSkillIdFromSkill(pLeftSkill, __FILE__, __LINE__);
        }

        D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pUnit);
        int32_t nRightSkillOwnerGUID = -1;
        int32_t nRightSkillId = 0; 
        if (pRightSkill)
        {
            nRightSkillOwnerGUID = SKILLS_GetOwnerGUIDFromSkill(pRightSkill);
            nRightSkillId = SKILLS_GetSkillIdFromSkill(pRightSkill, __FILE__, __LINE__);
        }

        D2UnitStrc* pRightWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
        if (pRightWeapon)
        {
            sub_6FC80B90(pGame, pUnit, pRightWeapon);
        }

        D2UnitStrc* pLeftWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
        if (pLeftWeapon)
        {
            sub_6FC80B90(pGame, pUnit, pLeftWeapon);
        }

        if (nLeftSkillId)
        {
            D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nLeftSkillId, nLeftSkillOwnerGUID);
            if (pSkill && pSkill != UNITS_GetLeftSkill(pUnit))
            {
                const int32_t nLeftSkillUseState = SKILLS_GetUseState(pUnit, pSkill);
                if (nLeftSkillUseState != SKILLUSTATE_NOQUANTITY && nLeftSkillUseState != SKILLUSTATE_NOLEVEL)
                {
                    D2GAME_AssignSkill_6FD13800(pUnit, 1, nLeftSkillId, nLeftSkillOwnerGUID);
                }
            }
        }

        if (nRightSkillId)
        {
            D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nRightSkillId, nRightSkillOwnerGUID);
            if (pSkill && pSkill != UNITS_GetRightSkill(pUnit))
            {
                const int32_t nRightSkillUseState = SKILLS_GetUseState(pUnit, pSkill);
                if (nRightSkillUseState != SKILLUSTATE_NOQUANTITY && nRightSkillUseState != SKILLUSTATE_NOLEVEL)
                {
                    D2GAME_AssignSkill_6FD13800(pUnit, 0, nRightSkillId, nRightSkillOwnerGUID);
                }
            }
        }
    }
}

//D2Game.0x6FC80B90
void __fastcall sub_6FC80B90(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pWeapon)
{
    if ((ITEMS_CheckIfStackable(pWeapon) || ITEMS_CheckIfThrowable(pWeapon) || STATLIST_UnitGetItemStatOrSkillStatValue(pWeapon, STAT_ITEM_THROWABLE, 0)) && (int32_t)STATLIST_UnitGetStatValue(pWeapon, STAT_QUANTITY, 0) <= 0)
    {
        if (STATLIST_UnitGetItemStatOrSkillStatValue(pWeapon, STAT_ITEM_THROWABLE, 0))
        {
            STATLIST_SetUnitStat(pWeapon, STAT_QUANTITY, 0, 0);
        }
        else
        {
            if (ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_WEAPON) && ITEMS_IsMagSetRarUniCrfOrTmp(pWeapon))
            {
                if (!ITEMS_CheckItemFlag(pWeapon, IFLAG_BROKEN, __LINE__, __FILE__))
                {
                    sub_6FC4B580(pGame, pPlayer, pWeapon);
                }
            }
            else
            {
                const int32_t nItemType = ITEMS_GetItemType(pWeapon);
                const int32_t nBodyLoc = ITEMS_GetBodyLocation(pWeapon);
                UNITS_ChangeAnimMode(pPlayer, 1);

                int32_t a5 = 0;
                sub_6FC45B30(pGame, pPlayer, nBodyLoc, 1, &a5);
                ITEMS_SetItemCMDFlag(pWeapon, 1, 1);

                if (ITEMS_GetReload(pWeapon))
                {
                    D2UnitStrc* pPreviousItem = nullptr;
                    D2UnitStrc* pItem = INVENTORY_GetBackPackItemByType(pPlayer->pInventory, nItemType, pPreviousItem);
                    while (pItem)
                    {
                        int32_t nWeaponClassId = -1;
                        if (pWeapon)
                        {
                            nWeaponClassId = pWeapon->dwClassId;
                        }

                        if (nWeaponClassId == pItem->dwClassId && !ITEMS_CheckItemFlag(pItem, 0x100u, __LINE__, __FILE__))
                        {
                            if (!INVENTORY_GetItemFromBodyLoc(pPlayer->pInventory, nBodyLoc) && sub_6FC49F80(pGame, pPlayer, pItem->dwUnitId, nBodyLoc))
                            {
                                return;
                            }

                            break;
                        }

                        pPreviousItem = pItem;
                        pItem = INVENTORY_GetBackPackItemByType(pPlayer->pInventory, nItemType, pPreviousItem);
                    }
                }

                if (ITEMS_GetReEquip(pWeapon))
                {
                    int32_t nWeaponGUID = UNITS_GetWeaponGUID(pPlayer);
                    if (nWeaponGUID)
                    {
                        D2UnitStrc* pWeaponItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nWeaponGUID);
                        if (pWeaponItem && pPlayer->pInventory && !ITEMS_GetInvPage(pWeaponItem) && INVENTORY_CompareWithItemsParentInventory(pPlayer->pInventory, pWeaponItem) && sub_6FC42F20(pPlayer, pWeaponItem, (int32_t*)&nBodyLoc, 0))
                        {
                            if (INVENTORY_GetItemFromBodyLoc(pPlayer->pInventory, nBodyLoc))
                            {
                                FOG_Trace("sRestoreWeapon: trying to restore weapon to already occupied spot!  location: %d", nBodyLoc);
                                FOG_DisplayAssert("0", __FILE__, __LINE__);
                                exit(-1);
                            }

                            sub_6FC49F80(pGame, pPlayer, nWeaponGUID, nBodyLoc);
                        }

                        UNITS_SetWeaponGUID(pPlayer, 0);
                    }
                }
            }
        }
    }
}

//D2Game.0x6FC80E10
void __fastcall sub_6FC80E10(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER)
    {
        D2UnitStrc* pItem = INVENTORY_GetFirstItem(pPlayer->pInventory);
        int32_t bChangedToNeutral = 0;
        while (pItem)
        {
            if (INVENTORY_GetItemNodePage(pItem) == 3 && INVENTORY_UnitIsItem(pItem) && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON)
                && ITEMS_HasDurability(pItem) && (int32_t)STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0) <= 0 
                && !ITEMS_CheckItemFlag(pItem, 0x100u, __LINE__, __FILE__))
            {
                if (!bChangedToNeutral)
                {
                    UNITS_ChangeAnimMode(pPlayer, PLRMODE_NEUTRAL);
                    bChangedToNeutral = 1;
                }

                sub_6FC4B580(pGame, pPlayer, pItem);
                pItem = INVENTORY_GetFirstItem(pPlayer->pInventory);
            }
            else
            {
                pItem = INVENTORY_GetNextItem(pItem);
            }
        }
    }
}

//D2Game.0x6FC80EE0
int32_t __fastcall sub_6FC80EE0(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pPlayer);
    if (!pSkill)
    {
        return 2;
    }

    UNITS_SetAnimationFrame(pPlayer, nSkillLevel);

    const uint32_t nSkillFlags = SKILLS_GetFlags(pSkill);
    if (nSkillFlags & 1 && (sub_6FCBC7E0(pGame, pPlayer), sub_6FCBC930(pGame, pPlayer) == 2))
    {
        SKILLS_SetFlags(pSkill, nSkillFlags | 2);
    }
    else
    {
        if (pPlayer && pPlayer->dwFlags & UNITFLAG_SKSRVDOFUNC || nSkillId != 1 && nSkillId != 2)
        {
            return (SUNIT_IsDead(pPlayer) != 0) + 1;
        }
    }

    sub_6FD13410(pGame, pPlayer);
    return (SUNIT_IsDead(pPlayer) != 0) + 1;
}

//Inlined in D2Game.0x6FC80F80
void __fastcall EVENTS_HpRegen(D2UnitStrc* pUnit)
{
    const int32_t nHpRegen = STATLIST_UnitGetStatValue(pUnit, STAT_HPREGEN, 0);
    if (nHpRegen)
    {
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
        int32_t nLife = nHpRegen + STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
        if (nLife > nMaxHp)
        {
            nLife = nMaxHp;
            D2StatListStrc* pHealthPotStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_HEALTHPOT);
            if (pHealthPotStatList)
            {
                D2Common_10474(pUnit, pHealthPotStatList);
                STATLIST_FreeStatList(pHealthPotStatList);
            }
        }

        if (nLife < 256)
        {
            nLife = 256;
        }

        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nLife, 0);

        const uint8_t nLastSentHpPct = STATLIST_UnitGetStatValue(pUnit, STAT_LAST_SENT_HP_PCT, 0);

        uint8_t nHpPct = 0x80;
        if (pUnit)
        {
            const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) >> 8;
            const int32_t nMaxLife = STATLIST_GetMaxLifeFromUnit(pUnit) >> 8;

            if (nMaxLife > 0 && nHitpoints < nMaxLife)
            {
                nHpPct = (nHitpoints << 7) / nMaxLife;
            }
        }

        uint8_t nHpPctDiff = nLastSentHpPct - nHpPct;
        nHpPctDiff = std::abs(nHpPctDiff);

        if (nHpPctDiff > 4)
        {
            sub_6FCC6270(pUnit, nHpPct);
            STATLIST_SetUnitStat(pUnit, STAT_LAST_SENT_HP_PCT, nHpPct, 0);
        }
    }
}

//Inlined in D2Game.0x6FC80F80
void __fastcall EVENTS_StaminaRegen(D2UnitStrc* pUnit)
{
    const int32_t nStamina = STATLIST_UnitGetStatValue(pUnit, STAT_STAMINA, 0);
    const int32_t nStaminaRecoveryBonus = STATLIST_UnitGetStatValue(pUnit, STAT_STAMINARECOVERYBONUS, 0);

    int32_t nShift = 8;
    switch (pUnit ? pUnit->dwAnimMode : 0)
    {
    case PLRMODE_NEUTRAL:
    case PLRMODE_TNEUTRAL:
        break;

    case PLRMODE_WALK:
        nShift = 9;
        if (nStamina < 256)
        {
            return;
        }
        break;

    case PLRMODE_TWALK:
        nShift = 9;
        break;

    default:
        if (nStaminaRecoveryBonus < 1000)
        {
            return;
        }
        break;
    }

    const int32_t nMaxStamina = STATLIST_GetMaxStaminaFromUnit(pUnit);
    if (nStamina < nMaxStamina)
    {
        int32_t nNewStamina = nMaxStamina >> nShift;
        if (nStaminaRecoveryBonus)
        {
            nNewStamina += (nStaminaRecoveryBonus * nNewStamina) / 100;
        }

        nNewStamina += nStamina;
        if (nNewStamina > nMaxStamina)
        {
            nNewStamina = nMaxStamina;
        }

        STATLIST_SetUnitStat(pUnit, STAT_STAMINA, nNewStamina, 0);
    }
}

//Inlined in D2Game.0x6FC80F80
void __fastcall EVENTS_ManaRegen(D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        const int32_t nMana = STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0);
        const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pUnit);

        int32_t nManaRecoveryBonus = 0;
        if (!STATES_CheckState(pUnit, STATE_NOMANAREGEN))
        {
            D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pUnit->dwClassId);
            if (!pCharStatsTxtRecord)
            {
                return;
            }

            int32_t nManaRegen = 25 * pCharStatsTxtRecord->nManaRegen;
            if (!nManaRegen)
            {
                nManaRegen = 7500;
            }

            int32_t nManaMultiplier = nMaxMana / nManaRegen;
            if (nManaMultiplier < 1)
            {
                nManaMultiplier = 1;
            }

            nManaRecoveryBonus = MONSTERUNIQUE_CalculatePercentage(nManaMultiplier, STATLIST_UnitGetStatValue(pUnit, STAT_MANARECOVERYBONUS, 0) + 100, 100);
        }

        int32_t nManaRecovery = STATLIST_UnitGetStatValue(pUnit, STAT_MANARECOVERY, 0) + nManaRecoveryBonus;
        if (nMaxMana <= nMana && nManaRecovery > 0)
        {
            D2StatListStrc* pManaPotStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_MANAPOT);
            if (pManaPotStatList)
            {
                D2Common_10474(pUnit, pManaPotStatList);
                STATLIST_FreeStatList(pManaPotStatList);
            }
        }

        if (nManaRecovery > nMaxMana - nMana)
        {
            nManaRecovery = nMaxMana - nMana;
        }

        if (nManaRecovery < -nMana)
        {
            nManaRecovery = -nMana;
        }

        if (nManaRecovery)
        {
            STATLIST_AddUnitStat(pUnit, STAT_MANA, nManaRecovery, 0);
        }
    }
}

//D2Game.0x6FC80F80
void __fastcall D2GAME_EVENTS_StatRegen_6FC80F80(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4)
{
    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_STATREGEN, pGame->dwGameFrame + 1, a3, a4);

    if (SUNIT_IsDead(pUnit))
    {
        return;
    }

    EVENTS_HpRegen(pUnit);

    EVENTS_StaminaRegen(pUnit);

    EVENTS_ManaRegen(pUnit);
}

//D2Game.0x6FC81250
void __fastcall sub_6FC81250(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4)
{
    int32_t bSkip = 0;
    int32_t nAnimMode = 0;
    if (pUnit)
    {
        nAnimMode = pUnit->dwAnimMode;
        if (nAnimMode != PLRMODE_ATTACK1 && nAnimMode != PLRMODE_ATTACK2 && nAnimMode != PLRMODE_THROW)
        {
            D2SkillStrc* pUsedSkill = UNITS_GetUsedSkill(pUnit);
            if (pUsedSkill)
            {
                D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(SKILLS_GetSkillIdFromSkill(pUsedSkill, __FILE__, __LINE__));
                if (!pSkillsTxtRecord || !(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_DURABILITY]))
                {
                    bSkip = 1;
                }
                else if (!pUnit)
                {
                    sub_6FC80E10(pGame, pUnit);
                    bSkip = 1;
                }
            }
        }
    }

    if (!bSkip)
    {
        if (pUnit->dwUnitType == UNIT_PLAYER && pUnit->pInventory)
        {
            D2UnkPlrModeStrc2 a2 = {};

            D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pUnit);
            if (pLeftSkill)
            {
                a2.nLeftSkillOwnerGUID = SKILLS_GetOwnerGUIDFromSkill(pLeftSkill);
                a2.nLeftSkillId = SKILLS_GetSkillIdFromSkill(pLeftSkill, __FILE__, __LINE__);
            }
            else
            {
                a2.nLeftSkillOwnerGUID = -1;
                a2.nLeftSkillId = 0;
            }

            D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pUnit);
            if (pRightSkill)
            {
                a2.nRightSkillOwnerGUID = SKILLS_GetOwnerGUIDFromSkill(pRightSkill);
                a2.nRightSkillId = SKILLS_GetSkillIdFromSkill(pRightSkill, __FILE__, __LINE__);
            }
            else
            {
                a2.nRightSkillOwnerGUID = -1;
                a2.nRightSkillId = 0;
            }

            D2UnitStrc* pRightWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
            if (pRightWeapon)
            {
                sub_6FC80B90(pGame, pUnit, pRightWeapon);
            }

            D2UnitStrc* pLeftWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
            if (pLeftWeapon)
            {
                sub_6FC80B90(pGame, pUnit, pLeftWeapon);
            }

            sub_6FC81560(pUnit, &a2);
        }

        sub_6FC80E10(pGame, pUnit);
    }

    if (nAnimMode == PLRMODE_DEATH)
    {
        D2GAME_CORPSE_Handler_6FC7FBD0(pGame, pUnit, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), UNITS_GetRoom(pUnit));
        D2GAME_KillPlayerPets_6FC7CD10(pGame, pUnit);
        
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        CLIENTS_ToggleFlag(pClient, CLIENTSAVEFLAG_DEAD, TRUE);

        if (!gpD2EventCallbackTable_6FD45830 || CLIENTS_CheckFlag(pClient, CLIENTSAVEFLAG_HARDCORE))
        {
            D2GAME_SAVE_WriteFile_6FC8A500(pGame, pUnit, CLIENTS_GetName(pClient), 0);
        }

        SUNIT_SetCombatMode(pGame, pUnit, PLRMODE_DEAD);
        sub_6FCBC4D0(pUnit);
        sub_6FC814F0(pGame, pUnit);
        D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pUnit);
    }
    else if (nAnimMode == PLRMODE_KNOCKBACK)
    {
        sub_6FC817D0(pGame, pUnit, 0, 4, -1, 0, 0);
    }
    else
    {
        int32_t nCombatMode = PLRMODE_NEUTRAL;
        D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
        if (pRoom)
        {
            nCombatMode = DUNGEON_IsRoomInTown(pRoom) != 0 ? PLRMODE_TNEUTRAL : PLRMODE_NEUTRAL;
        }
        D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pUnit);
        SUNIT_SetCombatMode(pGame, pUnit, nCombatMode);
        UNITS_SetUsedSkill(pUnit, 0);
    }
}

//D2Game.0x6FC814F0
void __fastcall sub_6FC814F0(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    STATLIST_SetUnitStat(pPlayer, STAT_HITPOINTS, 0, 0);
    D2Common_10469(pPlayer);
    STATES_UpdateStayDeathFlags(pPlayer, 0);
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    D2GAME_SetClientDead_6FC33830(pClient, 0);
    PATH_SetUnitDeadCollision(pPlayer, 1);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pPlayer, 8, 0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pPlayer, 9, 0);
    D2GAME_DeletePlayerPerFrameEvents_6FCBCE50(pGame, pPlayer);
}

//D2Game.0x6FC81560
void __fastcall sub_6FC81560(D2UnitStrc* pUnit, D2UnkPlrModeStrc2* a2)
{
    if (a2->nLeftSkillId)
    {
        D2SkillStrc* pLeftSkill = SKILLS_GetSkillById(pUnit, a2->nLeftSkillId, a2->nLeftSkillOwnerGUID);
        if (pLeftSkill && pLeftSkill != UNITS_GetLeftSkill(pUnit))
        {
            const int32_t nLeftSkillUseState = SKILLS_GetUseState(pUnit, pLeftSkill);
            if (nLeftSkillUseState != SKILLUSTATE_NOQUANTITY && nLeftSkillUseState != SKILLUSTATE_NOLEVEL)
            {
                D2GAME_AssignSkill_6FD13800(pUnit, 1, a2->nLeftSkillId, a2->nLeftSkillOwnerGUID);
            }
        }
    }

    if (a2->nRightSkillId)
    {
        D2SkillStrc* pRightSkill = SKILLS_GetSkillById(pUnit, a2->nRightSkillId, a2->nRightSkillOwnerGUID);
        if (pRightSkill && pRightSkill != UNITS_GetRightSkill(pUnit))
        {
            const int32_t nRightSkillUseState = SKILLS_GetUseState(pUnit, pRightSkill);
            if (nRightSkillUseState != SKILLUSTATE_NOQUANTITY && nRightSkillUseState != SKILLUSTATE_NOLEVEL)
            {
                D2GAME_AssignSkill_6FD13800(pUnit, 0, a2->nRightSkillId, a2->nRightSkillOwnerGUID);
            }
        }
    }
}

//D2Game.0x6FC81600
void __fastcall sub_6FC81600(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t(__fastcall* off_6FD304B8[])(D2GameStrc*, D2UnitStrc*, int32_t, int32_t) =
    {
        sub_6FC808D0,
        sub_6FC808D0,
        sub_6FC7F780,
        sub_6FC7F780,
        sub_6FC808D0,
        sub_6FC808D0,
        sub_6FC7F780,
        sub_6FC80EE0,
        sub_6FC80EE0,
        sub_6FC808D0,
        sub_6FC80EE0,
        sub_6FC80EE0,
        sub_6FC80EE0,
        sub_6FC80EE0,
        sub_6FC80EE0,
        sub_6FC80EE0,
        sub_6FC80EE0,
        sub_6FC808D0,
        sub_6FC80EE0,
        sub_6FC7F780,
        //nullptr,
        //nullptr,
        //PLRMODE_StartID_WalkRunKnockback,
        //PLRMODE_StartID_WalkRunKnockback,
        //nullptr,
        //nullptr,
        //PLRMODE_StartID_WalkRunKnockback,
        //PLRMODE_StartID_AttackCastThrowKickSpecialSequence,
        //PLRMODE_StartID_AttackCastThrowKickSpecialSequence,
        //nullptr,
        //PLRMODE_StartID_AttackCastThrowKickSpecialSequence,
        //PLRMODE_StartID_AttackCastThrowKickSpecialSequence,
        //PLRMODE_StartID_AttackCastThrowKickSpecialSequence,
        //PLRMODE_StartID_AttackCastThrowKickSpecialSequence,
        //PLRMODE_StartID_AttackCastThrowKickSpecialSequence,
        //PLRMODE_StartID_AttackCastThrowKickSpecialSequence,
        //PLRMODE_StartID_AttackCastThrowKickSpecialSequence,
        //nullptr,
        //PLRMODE_StartID_AttackCastThrowKickSpecialSequence,
        //PLRMODE_StartID_WalkRunKnockback,
    };

    int32_t nAnimMode = 0;
    if (pUnit)
    {
        nAnimMode = pUnit->dwAnimMode;
    }

    if (off_6FD304B8[nAnimMode] && off_6FD304B8[nAnimMode](pGame, pUnit, nSkillId, nSkillLevel) == 2)
    {
        sub_6FC81250(pGame, pUnit, nSkillId, nSkillLevel);
    }
}

//D2Game.0x6FC81650
void __fastcall sub_6FC81650(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2ClientStrc* pClient, int32_t a4)
{
    D2UnitStrc* pOtherPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (pPlayer)
    {
        if (pPlayer->dwFlagEx & UNITFLAGEX_TELEPORTED)
        {
            sub_6FC81CA0(pPlayer, pClient, 1);
        }
        else if (pPlayer->dwFlagEx & 0x800 && pPlayer != pOtherPlayer)
        {
            sub_6FC81CA0(pPlayer, pClient, 0);
        }
    }

    if (!a4 && pPlayer && pPlayer->dwFlagEx & UNITFLAGEX_HASINV)
    {
        D2GAME_INVMODE_First_6FC40FB0(pGame, pPlayer, pClient, 0);
        D2GAME_SendPacket0x47_6FC3EEE0(pClient, pPlayer);
        D2GAME_SendPacket0x48_6FC3EF40(pClient, pPlayer, 0);
    }

    if (pPlayer && pPlayer->dwFlags & UNITFLAG_DOUPDATE)
    {
        sub_6FC82270(pGame, pPlayer, pClient);
    }

    sub_6FCC6540(pPlayer, pClient);

    if (pPlayer)
    {
        if (pPlayer->dwFlags & 0x400)
        {
            D2GAME_UpdateUnit_6FCC6080(pPlayer, pClient);
        }

        if (pPlayer->dwFlags & 0x100)
        {
            sub_6FCC5FA0(pPlayer, pClient);
        }

        if (pPlayer->dwFlags & 0x8000)
        {
            sub_6FC81C00(pPlayer, pClient);
        }
    }

    if (STATES_IsAnyGfxStateFlagSet(pPlayer))
    {
        D2GAME_STATES_SendStates_6FCC5F00(pPlayer, pClient, 0);
    }

    if (D2COMMON_10530_D2CheckStatlistFlagDMGRed(pPlayer))
    {
        sub_6FCC5F20(pPlayer, pClient);
    }

    D2Common_10512(pPlayer, pOtherPlayer, STAT_VELOCITYPERCENT, (void(__fastcall*)(D2UnitStrc*, int32_t, int32_t, D2UnitStrc*))D2GAME_UpdateAttribute_6FC822D0);
    D2Common_10512(pPlayer, pOtherPlayer, STAT_ATTACKRATE, (void(__fastcall*)(D2UnitStrc*, int32_t, int32_t, D2UnitStrc*))D2GAME_UpdateAttribute_6FC822D0);
    D2Common_10512(pPlayer, pOtherPlayer, STAT_LEVEL, (void(__fastcall*)(D2UnitStrc*, int32_t, int32_t, D2UnitStrc*))D2GAME_UpdateAttribute_6FC822D0);
    D2Common_10512(pPlayer, pOtherPlayer, STAT_STRENGTH, (void(__fastcall*)(D2UnitStrc*, int32_t, int32_t, D2UnitStrc*))D2GAME_UpdateAttribute_6FC822D0);
    D2Common_10512(pPlayer, pOtherPlayer, STAT_DEXTERITY, (void(__fastcall*)(D2UnitStrc*, int32_t, int32_t, D2UnitStrc*))D2GAME_UpdateAttribute_6FC822D0);
}

//D2Game.0x6FC817D0
void __fastcall sub_6FC817D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SkillStrc* pUsedSkill, int32_t nMode, int32_t nX, int32_t nY, int32_t a7)
{
    D2_ASSERT(pGame);

    if (sub_6FC81890(pGame, pUnit, nMode) && (a7 || D2GAME_PLRMODES_First_6FC7F340(pGame, pUnit, nMode, 1, pUsedSkill)))
    {
        if (IsBadCodePtr((FARPROC)stru_6FD28EA0[nMode].pfnStartXY))
        {
            FOG_DisplayAssert("cptStartChart->pfnStartXY", __FILE__, __LINE__);
            exit(-1);
        }

        if (pUsedSkill)
        {
            UNITS_SetUsedSkill(pUnit, pUsedSkill);
        }

        stru_6FD28EA0[nMode].pfnStartXY(pGame, pUnit, nMode, nX, nY);
    }
}

//D2Game.0x6FC81890
int32_t __fastcall sub_6FC81890(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMode)
{
    if (pUnit->pInventory && INVENTORY_GetCursorItem(pUnit->pInventory) && nMode != PLRMODE_DEATH && nMode != PLRMODE_DEAD)
    {
        return 0;
    }

    switch (nMode)
    {
    case PLRMODE_DEATH:
    case PLRMODE_NEUTRAL:
    case PLRMODE_TNEUTRAL:
    case PLRMODE_DEAD:
        return 1;

    default:
        break;
    }

    switch (pUnit->dwAnimMode)
    {
    case PLRMODE_ATTACK1:
    case PLRMODE_ATTACK2:
    case PLRMODE_CAST:
    case PLRMODE_THROW:
    {
        if (pGame->dwGameFrame <= EVENT_GetEventFrame(pGame, pUnit, 1) + 5 || nMode == PLRMODE_GETHIT || nMode == PLRMODE_BLOCK)
        {
            return 1;
        }
        return 0;
    }
    case PLRMODE_DEATH:
    case PLRMODE_GETHIT:
    case PLRMODE_BLOCK:
    case PLRMODE_DEAD:
    {
        return 0;
    }
    case PLRMODE_KICK:
    {
        return pGame->dwGameFrame <= EVENT_GetEventFrame(pGame, pUnit, 1) + 5;
    }
    case PLRMODE_SPECIAL1:
    {
        if (pUnit->dwClassId != PCLASS_AMAZON)
        {
            return 1;
        }
        return 0;
    }
    case PLRMODE_SPECIAL3:
    {
        if (pUnit->dwClassId != PCLASS_DRUID)
        {
            return 1;
        }
        return 0;
    }
    case PLRMODE_SEQUENCE:
    {
        D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
        if (pSkill)
        {
            D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
            if (pSkillsTxtRecord && pSkillsTxtRecord->nSeqInput)
            {
                return 1;
            }
        }

        return pGame->dwGameFrame <= EVENT_GetEventFrame(pGame, pUnit, 1) + 5;
    }
    default:
    {
        break;
    }
    }

    return 1;
}

//D2Game.0x6FC81A00
void __fastcall D2GAME_PLAYERMODE_Change_6FC81A00(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2SkillStrc* pSkill, BYTE nMode, int32_t nUnitType, int32_t nTargetGUID, int32_t bAllowReEnter)
{
    D2_ASSERT(pGame);

    D2UnitStrc* pTarget = SUNIT_GetServerUnit(pGame, nUnitType, nTargetGUID);
    if (pTarget)
    {
        if (sub_6FC81890(pGame, pPlayer, nMode) && (bAllowReEnter || D2GAME_PLRMODES_First_6FC7F340(pGame, pPlayer, nMode, 1, pSkill)))
        {
            if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER && pPlayer->pPlayerData)
            {
                pPlayer->pPlayerData->unk0xA8[43] = 0;
                pPlayer->pPlayerData->unk0xA8[42] = 0;
            }

            if (IsBadCodePtr((FARPROC)stru_6FD28EA0[nMode].pfnStartID))
            {
                FOG_DisplayAssert("cptStartChart->pfnStartID", __FILE__, __LINE__);
                exit(-1);
            }

            if (pSkill)
            {
                UNITS_SetUsedSkill(pPlayer, pSkill);
            }

            stru_6FD28EA0[nMode].pfnStartID(pGame, pPlayer, nMode, pTarget);
        }
    }
    //else
    //{
    //    LOG_11100(1, 2, 4, 4, __FILE__, __LINE__, "StartModeID: Bad Target");
    //}
}

//D2Game.0x6FC81B20
void __fastcall sub_6FC81B20(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4)
{
    if (!pUnit || !pUnit->pHoverText)
    {
        return;
    }

    const int32_t nTimeout = CHAT_GetTimeoutFromHoverMsg(pUnit->pHoverText);
    if (nTimeout > pGame->dwGameFrame)
    {
        EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_FREEHOVER, nTimeout, 0, 0);
    }
    else
    {
        CHAT_FreeHoverMsg(pGame->pMemoryPool, pUnit->pHoverText);
        pUnit->pHoverText = nullptr;
        UNITROOM_RefreshUnit(pUnit);
        pUnit->dwFlags |= UNITFLAG_HASTXTMSG;
    }
}

//D2Game.0x6FC81B90
void __fastcall sub_6FC81B90(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4)
{
    PARTY_SynchronizeWithClient(pGame, pUnit);
    sub_6FC7E640(pGame, pUnit);
    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_DELAYEDPORTAL, pGame->dwGameFrame + 30, 0, 0);
}

//D2Game.0x6FC81BD0
void __fastcall D2GAME_EVENTS_Callback_6FC81BD0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEvent, int32_t dwArg, int32_t dwArgEx)
{
    using EventCallbackFunction = void(__fastcall*)(D2GameStrc*, D2UnitStrc*, int32_t, int32_t);

    constexpr EventCallbackFunction off_6FD28F40[] =
    {
        sub_6FC81600,
        sub_6FC81250,
        nullptr,
        D2GAME_EVENTS_StatRegen_6FC80F80,
        nullptr,
        D2GAME_MONSTERS_AiFunction06_6FD14370,
        sub_6FC81B20,
        nullptr,
        D2GAME_MONSTERS_AiFunction09_6FD13470,
        D2GAME_MONSTERS_AiFunction10_6FD13610,
        nullptr,
        sub_6FC81B90,
        D2GAME_MONSTERS_AiFunction13_6FC65890,
        D2GAME_PLRTRADE_Last_6FC937F0
    };

    if (nEvent >= 0 && nEvent <= std::size(off_6FD28F40))
    {
        EventCallbackFunction pCallback = off_6FD28F40[nEvent];
        if (pCallback)
        {
            pCallback(pGame, pUnit, dwArg, dwArgEx);
        }
    }
}
