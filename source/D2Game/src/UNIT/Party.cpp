#include "UNIT/Party.h"

#include <algorithm>

#include <Fog.h>

#include <D2Dungeon.h>
#include <D2StatList.h>

#include "GAME/SCmd.h"
#include "PLAYER/PartyScreen.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlrTrade.h"
#include "UNIT/Friendly.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FCB9B00
void __fastcall PARTY_AllocPartyControl(D2GameStrc* pGame)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pGame->pPartyControl);

    D2PartyControlStrc* pPartyControl = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2PartyControlStrc);
    D2_ASSERT(pPartyControl);

    pPartyControl->field_0 = 3;
    pPartyControl->field_2 = 0;
    pPartyControl->pParties = nullptr;

    pGame->pPartyControl = pPartyControl;
}

//D2Game.0x6FCB9BA0
void __fastcall PARTY_FreePartyControl(D2GameStrc* pGame)
{
    D2_ASSERT(pGame);

    if (!pGame->pPartyControl)
    {
        return;
    }

    D2PartyStrc* pParty = pGame->pPartyControl->pParties;
    while (pParty)
    {
        D2PartyStrc* pNextParty = pParty->pNext;

        D2PartyNodeStrc* pPartyNode = pParty->pPartyNodes;
        while (pPartyNode)
        {
            D2PartyNodeStrc* pNextPartyNode = pPartyNode->pNext;
            D2_FREE_POOL(pGame->pMemoryPool, pPartyNode);
            pPartyNode = pNextPartyNode;
        }

        D2_FREE_POOL(pGame->pMemoryPool, pParty);
        pParty = pNextParty;
    }

    D2_FREE_POOL(pGame->pMemoryPool, pGame->pPartyControl);
    pGame->pPartyControl = nullptr;
}

//D2Game.0x6FCB9C40
int16_t __fastcall sub_6FCB9C40(D2GameStrc* pGame)
{
    D2_ASSERT(pGame->pPartyControl);

    D2PartyStrc* pParty = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2PartyStrc);
    D2_ASSERT(pParty);

    D2PartyControlStrc* pPartyControl = pGame->pPartyControl;

    int16_t nPartyId = std::max(pPartyControl->field_0, 3i16);

    while (1)
    {
        D2PartyStrc* pCurrent = pPartyControl->pParties;
        while (pCurrent && pCurrent->nPartyId != nPartyId)
        {
            pCurrent = pCurrent->pNext;
        }

        if (!pCurrent)
        {
            break;
        }

        ++nPartyId;

        nPartyId = std::max(nPartyId, 3i16);
    }

    pPartyControl->field_0 = nPartyId + 1;
    pParty->nPartyId = nPartyId;
    pParty->pNext = pPartyControl->pParties;
    pPartyControl->pParties = pParty;
    return pParty->nPartyId;
}

//D2Game.0x6FCB9D10
int32_t __fastcall sub_6FCB9D10(D2GameStrc* pGame, int16_t nPartyId, D2UnitStrc* pPlayer)
{
    D2_ASSERT(pGame->pPartyControl);
   
    D2PartyStrc* pParty = nullptr;
    for (D2PartyStrc* i = pGame->pPartyControl->pParties; i; i = i->pNext)
    {
        if (i->nPartyId == nPartyId)
        {
            pParty = i;
            break;
        }
    }

    D2_ASSERT(pParty);
    D2_ASSERT(pPlayer && pPlayer->dwUnitType == UNIT_PLAYER);

    int32_t nUnitGUID = -1;
    if (pPlayer)
    {
        nUnitGUID = pPlayer->dwUnitId;
    }

    for (D2PartyNodeStrc* i = pParty->pPartyNodes; i; i = i->pNext)
    {
        if (i->nUnitGUID == nUnitGUID)
        {
            return 1;
        }
    }

    D2PartyNodeStrc* pPartyNode = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2PartyNodeStrc);
    D2_ASSERT(pPartyNode);

    pPartyNode->nUnitGUID = nUnitGUID;
    pPartyNode->pNext = pParty->pPartyNodes;

    pParty->pPartyNodes = pPartyNode;

    sub_6FC3F8F0(pGame, pPlayer);

    for (D2PartyNodeStrc* i = pPartyNode->pNext; i; i = i->pNext)
    {
        D2UnitStrc* pPartyMember = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, i->nUnitGUID);
        if (pPartyMember)
        {
            FRIENDLY_RemoveHostility(pGame, pPlayer, pPartyMember);
            FRIENDLY_RemoveHostility(pGame, pPartyMember, pPlayer);
        }
    }

    return 1;
}

//D2Game.0x6FCB9E80
void __fastcall PARTY_LeaveParty(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit && pUnit->dwUnitType == UNIT_PLAYER);

    const int16_t nPartyId = PARTY_GetPartyIdForUnitOwner(pGame, pUnit);
    if (nPartyId == -1)
    {
        return;
    }

    D2_ASSERT(pGame->pPartyControl);

    D2PartyStrc* pParty = pGame->pPartyControl->pParties;
    while (pParty)
    {
        if (pParty->nPartyId == nPartyId)
        {
            break;
        }
        pParty = pParty->pNext;
    }

    D2_ASSERT(pParty);

    D2PartyNodeStrc* pPreviousPartyNode = nullptr;
    for (D2PartyNodeStrc* pPartyNode = pParty->pPartyNodes; pPartyNode; pPartyNode = pPartyNode->pNext)
    {
        if (pPartyNode->nUnitGUID == pUnit->dwUnitId)
        {
            if (pPreviousPartyNode)
            {
                pPreviousPartyNode->pNext = pPartyNode->pNext;
            }
            else
            {
                pParty->pPartyNodes = pPartyNode->pNext;
            }

            sub_6FC7B450(pGame, pUnit);
            D2GAME_PACKETS_SendPacket0x8D_6FC3F960(pUnit, pUnit);
            D2_FREE_POOL(pGame->pMemoryPool, pPartyNode);
            break;
        }
        pPreviousPartyNode = pPartyNode;
    }

    D2_ASSERT(pGame->pPartyControl);

    int32_t nPartyNodes = 0;
    if (pParty)
    {
        for (D2PartyNodeStrc* i = pParty->pPartyNodes; i; i = i->pNext)
        {
            ++nPartyNodes;
        }
    }

    if (nPartyNodes > 1)
    {
        return;
    }

    D2PartyStrc* pPreviousParty = nullptr;
    for (D2PartyStrc* pCurrentParty = pGame->pPartyControl->pParties; pCurrentParty; pCurrentParty = pCurrentParty->pNext)
    {
        if (pCurrentParty->nPartyId == nPartyId)
        {
            if (pPreviousParty)
            {
                pPreviousParty->pNext = pCurrentParty->pNext;
            }
            else
            {
                pGame->pPartyControl->pParties = pCurrentParty->pNext;
            }

            D2PartyNodeStrc* pNode = pCurrentParty->pPartyNodes;
            while (pNode)
            {
                D2UnitStrc* pPartyMember = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pNode->nUnitGUID);
                if (pPartyMember)
                {
                    sub_6FC7B450(pGame, pPartyMember);
                    D2GAME_PACKETS_SendPacket0x8D_6FC3F960(pPartyMember, pPartyMember);
                }
                else
                {
                    FOG_DisplayWarning("hUnit", __FILE__, __LINE__);
                }

                D2PartyNodeStrc* pNext = pNode->pNext;
                D2_FREE_POOL(pGame->pMemoryPool, pNode);
                pNode = pNext;
            }

            D2_FREE_POOL(pGame->pMemoryPool, pCurrentParty);
            break;
        }

        pPreviousParty = pCurrentParty;
    }
}

//D2Game.0x6FCBA0C0
int32_t __fastcall PARTY_GetLivingPartyMemberCountInSameLevel(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    int32_t nLivingPartyMembers = 0;
    PARTY_IteratePartyMembersInSameLevel(pGame, pUnit, PARTY_CountLivingUnits, &nLivingPartyMembers);
    return nLivingPartyMembers;
}

//D2Game.0x6FCBA0E0
void __fastcall PARTY_CountLivingUnits(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pLivingUnits)
{
    if (pUnit && !SUNIT_IsDead(pUnit))
    {
        ++*((int32_t*)pLivingUnits);
    }
}

//D2Game.0x6FCBA100
void __fastcall PARTY_IteratePartyMembers(D2GameStrc* pGame, int16_t nPartyId, PartyCallbackFunction pCallback, void* pArgs)
{
    D2_ASSERT(pGame->pPartyControl);

    for (D2PartyStrc* pParty = pGame->pPartyControl->pParties; pParty; pParty = pParty->pNext)
    {
        if (pParty->nPartyId == nPartyId)
        {
            if (nPartyId != -1)
            {
                for (D2PartyNodeStrc* pPartyNode = pParty->pPartyNodes; pPartyNode; pPartyNode = pPartyNode->pNext)
                {
                    D2UnitStrc* pPartyMember = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pPartyNode->nUnitGUID);
                    if (pPartyMember)
                    {
                        pCallback(pGame, pPartyMember, pArgs);
                    }
                }
            }
            return;
        }
    }
}

//D2Game.0x6FCBA190
void __fastcall PARTY_IteratePartyMembersInSameLevel(D2GameStrc* pGame, D2UnitStrc* pUnit, PartyCallbackFunction pCallback, void* pArgs)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (!pRoom)
    {
        return pCallback(pGame, pUnit, pArgs);
    }

    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    const int32_t nPartyId = SUNIT_GetPartyId(pUnit);
    if (nPartyId == -1)
    {
        return pCallback(pGame, pUnit, pArgs);
    }

    D2_ASSERT(pGame->pPartyControl);

    for (D2PartyStrc* pParty = pGame->pPartyControl->pParties; pParty; pParty = pParty->pNext)
    {
        if (pParty->nPartyId == nPartyId)
        {
            for (D2PartyNodeStrc* pPartyNode = pParty->pPartyNodes; pPartyNode; pPartyNode = pPartyNode->pNext)
            {
                D2UnitStrc* pPartyMember = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pPartyNode->nUnitGUID);
                if (pPartyMember)
                {
                    D2RoomStrc* pPartyMemberRoom = UNITS_GetRoom(pPartyMember);
                    if (pPartyMemberRoom && DUNGEON_GetLevelIdFromRoom(pPartyMemberRoom) == nLevelId)
                    {
                        pCallback(pGame, pPartyMember, pArgs);
                    }
                }
            }
            break;
        }
    }
}

//D2Game.0x6FCBA270
int32_t __fastcall PARTY_ShareGoldDrop(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nGoldValue)
{
    const int32_t nLivingPartyMembers = PARTY_GetLivingPartyMemberCountInSameLevel(pGame, pUnit);
   
    if (nLivingPartyMembers > 1)
    {
        const int16_t nPartyId = SUNIT_GetPartyId(pUnit);

        D2_ASSERT(pGame->pPartyControl);

        for (D2PartyStrc* pParty = pGame->pPartyControl->pParties; pParty; pParty = pParty->pNext)
        {
            if (pParty->nPartyId == nPartyId)
            {
                const int32_t nDividedGold = nGoldValue / nLivingPartyMembers;
                const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit));
                
                for (D2PartyNodeStrc* pPartyNode = pParty->pPartyNodes; pPartyNode; pPartyNode = pPartyNode->pNext)
                {
                    D2UnitStrc* pPartyMember = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pPartyNode->nUnitGUID);
                    if (pPartyMember && pPartyMember != pUnit)
                    {
                        D2RoomStrc* pPartyMemberRoom = UNITS_GetRoom(pPartyMember);
                        if (pPartyMemberRoom && DUNGEON_GetLevelIdFromRoom(pPartyMemberRoom) == nLevelId && pUnit && pUnit->dwAnimMode != PLRMODE_DEATH && pUnit->dwAnimMode != PLRMODE_DEAD)
                        {
                            int32_t nGoldToPick = 0;
                            int32_t nGoldToDrop = 0;
                            PARTY_CalculatePickAndDrop(pUnit, nDividedGold, &nGoldToPick, &nGoldToDrop);
                            D2GAME_SetStatOrResetGold_6FC7CA70(pPartyMember, STAT_GOLD, nGoldToPick + STATLIST_UnitGetStatValue(pPartyMember, STAT_GOLD, 0));

                            nGoldValue += nGoldToDrop - nDividedGold;
                        }
                    }
                }

                int32_t nGoldToPick = 0;
                int32_t nGoldToDrop = 0;
                PARTY_CalculatePickAndDrop(pUnit, nGoldValue, &nGoldToPick, &nGoldToDrop);
                PLRTRADE_AddGold(pUnit, STAT_GOLD, nGoldToPick);

                return nGoldToDrop;
            }
        }
    }

    int32_t nGoldToPick = 0;
    int32_t nGoldToDrop = 0;
    PARTY_CalculatePickAndDrop(pUnit, nGoldValue, &nGoldToPick, &nGoldToDrop);
    D2GAME_SetStatOrResetGold_6FC7CA70(pUnit, STAT_GOLD, nGoldToPick + STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0));

    return nGoldToDrop;
}

//D2Game.0x6FCBA510
void __fastcall PARTY_CalculatePickAndDrop(D2UnitStrc* pUnit, int32_t nValue, int32_t* pGoldToPick, int32_t* pGoldToDrop)
{
    const int32_t nGoldLimit = UNITS_GetInventoryGoldLimit(pUnit);
    const int32_t nGold = STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0);
    const int32_t nDiff = nGoldLimit - nGold;
    if (nValue > nDiff)
    {
        *pGoldToPick = nDiff;
        *pGoldToDrop = nValue - nDiff;
    }
    else
    {
        *pGoldToPick = nValue;
        *pGoldToDrop = 0;
    }
}

//D2Game.0x6FCBA550
void __fastcall PARTY_SynchronizeWithClient(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    const int16_t nPartyId = SUNIT_GetPartyId(pUnit);
    if (nPartyId == -1)
    {
        return;
    }

    D2_ASSERT(pGame->pPartyControl);

    for (D2PartyStrc* pParty = pGame->pPartyControl->pParties; pParty; pParty = pParty->pNext)
    {
        if (pParty->nPartyId == nPartyId)
        {
            for (D2PartyNodeStrc* pPartyNode = pParty->pPartyNodes; pPartyNode; pPartyNode = pPartyNode->pNext)
            {
                D2UnitStrc* pPartyMember = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pPartyNode->nUnitGUID);
                if (pPartyMember && pPartyMember != pUnit)
                {
                    sub_6FC3E200(pClient, pPartyMember);
                }
            }
            return;
        }
    }
}

//D2Game.0x6FCBA5F0
int16_t __fastcall PARTY_GetPartyIdForUnitOwner(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit && pUnit->dwUnitType == UNIT_PLAYER);
    D2_ASSERT(pGame);

    int32_t nUnitGUID = -1;
    if (pUnit->pInventory)
    {
        nUnitGUID = INVENTORY_GetOwnerId(pUnit->pInventory);
    }
    else
    {
        nUnitGUID = pUnit->dwUnitId;
    }

    if (!pGame->pPartyControl)
    {
        return -1;
    }

    for (D2PartyStrc* pParty = pGame->pPartyControl->pParties; pParty; pParty = pParty->pNext)
    {
        for (D2PartyNodeStrc* pPartyNode = pParty->pPartyNodes; pPartyNode; pPartyNode = pPartyNode->pNext)
        {
            if (pPartyNode->nUnitGUID == nUnitGUID)
            {
                return pParty->nPartyId;
            }
        }
    }

    return -1;
}
