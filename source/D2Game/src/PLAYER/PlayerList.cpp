#include "PLAYER/PlayerList.h"

#include <D2States.h>

#include "GAME/SCmd.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


#pragma warning(disable: 28159)


uint32_t dword_6FD4DC40;


//D2Game.0x6FCBA690
void __fastcall PLAYERLIST_FreePlayerLists(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    SUNIT_IterateLivingPlayers(pGame, PLAYERLIST_IterateCallback_FreePlayerList, pPlayer);

    D2PlayerListStrc* pPlayerList = pPlayerData->pPlayerList;
    while (pPlayerList)
    {
        D2PlayerListStrc* pNext = pPlayerList->pNext;
        D2_FREE_POOL(pGame->pMemoryPool, pPlayerList);
        pPlayerList = pNext;
    }

    pPlayerData->pPlayerList = nullptr;
}

//D2Game.0x6FCBA6E0
void __fastcall PLAYERLIST_IterateCallback_FreePlayerList(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg)
{
    D2UnitStrc* pPlayerArg = (D2UnitStrc*)pArg;

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);

    if (pPlayer == pPlayerArg)
    {
        return;
    }

    int32_t nUnitGUID = -1;
    if (pPlayerArg)
    {
        nUnitGUID = pPlayerArg->dwUnitId;
    }

    D2PlayerListStrc* pPrevious = nullptr;
    for (D2PlayerListStrc* pPlayerList = pPlayerData->pPlayerList; pPlayerList; pPlayerList = pPlayerList->pNext)
    {
        if (pPlayerList->nUnitGUID == nUnitGUID)
        {
            if (pPrevious)
            {
                pPrevious->pNext = pPlayerList->pNext;
            }
            else
            {
                pPlayerData->pPlayerList = pPlayerList->pNext;
            }

            D2_FREE_POOL(pGame->pMemoryPool, pPlayerList);
            return;
        }

        pPrevious = pPlayerList;
    }
}

//Inlined in some functions
D2PlayerListStrc* __fastcall PLAYERLIST_GetPlayerListRecordFromUnitGUID(D2UnitStrc* pPlayer, int32_t nUnitGUID)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);

    for (D2PlayerListStrc* pPlayerList = pPlayerData->pPlayerList; pPlayerList; pPlayerList = pPlayerList->pNext)
    {
        if (pPlayerList->nUnitGUID == nUnitGUID)
        {
            return pPlayerList;
        }
    }

    return nullptr;
}

//D2Game.0x6FCBA750
void __fastcall PLAYERLIST_AllocPlayerList(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer1);
    if (pPlayer1 == pPlayer2 || STATES_CheckState(pPlayer2, STATE_PLAYERBODY) || STATES_CheckState(pPlayer1, STATE_PLAYERBODY))
    {
        return;
    }

    D2PlayerListStrc* pPlayerList = PLAYERLIST_GetPlayerListRecordFromUnitGUID(pPlayer1, pPlayer2 ? pPlayer2->dwUnitId : -1);
    if (pPlayerList)
    {
        return;
    }

    pPlayerList = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2PlayerListStrc);

    pPlayerList->nUnitGUID = pPlayer2 ? pPlayer2->dwUnitId : -1;
    pPlayerList->field_8 = 0;

    if (SUNIT_GetPartyId(pPlayer2) == -1)
    {
        pPlayerList->field_C = 0;
        pPlayerList->pNext = pPlayerData->pPlayerList;
        pPlayerData->pPlayerList = pPlayerList;
    }
    else
    {
        pPlayerList->field_C = 1;
        pPlayerList->pNext = pPlayerData->pPlayerList;
        pPlayerData->pPlayerList = pPlayerList;
    }
}

//D2Game.0x6FCBA840
void __fastcall PLAYERLIST_ToggleFlag(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, uint32_t nFlags, int32_t bSet)
{
    if (pPlayer1 == pPlayer2)
    {
        return;
    }

    D2PlayerListStrc* pPlayerList = PLAYERLIST_GetPlayerListRecordFromUnitGUID(pPlayer1, pPlayer2 ? pPlayer2->dwUnitId : -1);
    if (!pPlayerList)
    {
        return;
    }

    if (bSet)
    {
        pPlayerList->dwFlags |= nFlags;
    }
    else
    {
        pPlayerList->dwFlags &= ~nFlags;
    }

    sub_6FC3F840(pGame, pPlayer1 ? pPlayer1->dwUnitId : -1, pPlayer2 ? pPlayer2->dwUnitId : -1, pPlayerList->dwFlags);
}

//D2Game.0x6FCBA8E0
int32_t __fastcall PLAYERLIST_CheckFlag(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, uint32_t nFlags)
{
    if (pPlayer1 == pPlayer2)
    {
        return 0;
    }

    D2PlayerListStrc* pPlayerList = PLAYERLIST_GetPlayerListRecordFromUnitGUID(pPlayer1, pPlayer2 ? pPlayer2->dwUnitId : -1);
    if (!pPlayerList)
    {
        return 0;
    }

    return pPlayerList->dwFlags & nFlags;
}

//D2Game.0x6FCBA930
uint16_t __fastcall PLAYERLIST_GetFlags(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2)
{
    if (pPlayer1 == pPlayer2)
    {
        return 0;
    }

    D2PlayerListStrc* pPlayerList = PLAYERLIST_GetPlayerListRecordFromUnitGUID(pPlayer1, pPlayer2 ? pPlayer2->dwUnitId : -1);
    if (!pPlayerList)
    {
        return 0;
    }

    return pPlayerList->dwFlags;
}

//D2Game.0x6FCBA980
void __fastcall sub_6FCBA980(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t a3)
{
    if (pPlayer1 == pPlayer2)
    {
        return;
    }

    D2PlayerListStrc* pPlayerList = PLAYERLIST_GetPlayerListRecordFromUnitGUID(pPlayer1, pPlayer2 ? pPlayer2->dwUnitId : -1);
    if (!pPlayerList)
    {
        return;
    }

    pPlayerList->field_C = a3;
}

//D2Game.0x6FCBA9D0
int16_t __fastcall sub_6FCBA9D0(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2)
{
    if (pPlayer1 == pPlayer2)
    {
        return 1;
    }

    D2PlayerListStrc* pPlayerList = PLAYERLIST_GetPlayerListRecordFromUnitGUID(pPlayer1, pPlayer2 ? pPlayer2->dwUnitId : -1);
    if (!pPlayerList)
    {
        return 0;
    }

    return pPlayerList->field_C;
}

//D2Game.0x6FCBAA20
void __fastcall sub_6FCBAA20(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2)
{
    if (pPlayer1 == pPlayer2)
    {
        return;
    }

    D2PlayerListStrc* pPlayerList = PLAYERLIST_GetPlayerListRecordFromUnitGUID(pPlayer1, pPlayer2 ? pPlayer2->dwUnitId : -1);

    if ((pPlayer1 ? pPlayer1->dwUnitId : -1) != (pPlayer2 ? pPlayer2->dwUnitId : -1) && pPlayerList)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__);
        D2GAME_PACKETS_SendPacket0x8B_6FC3F810(pClient, pPlayer2 ? pPlayer2->dwUnitId : -1, pPlayerList->field_C);
    }
}

//D2Game.0x6FCBAAA0
void __fastcall sub_6FCBAAA0(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2)
{
    if (pPlayer1 == pPlayer2)
    {
        return;
    }

    D2PlayerListStrc* pPlayerList = PLAYERLIST_GetPlayerListRecordFromUnitGUID(pPlayer1, pPlayer2 ? pPlayer2->dwUnitId : -1);
    if (!pPlayerList)
    {
        return;
    }

    sub_6FC3F840(pGame, pPlayer1 ? pPlayer1->dwUnitId : -1, pPlayer2 ? pPlayer2->dwUnitId : -1, pPlayerList->dwFlags);
}

//D2Game.0x6FCBAB20
void __fastcall sub_6FCBAB20(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2_ASSERT(pPlayer);

    SUNIT_IterateLivingPlayers(pGame, sub_6FCBAB50, pPlayer);
}

//D2Game.0x6FCBAB50
void __fastcall sub_6FCBAB50(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg)
{
    D2UnitStrc* pUnit = (D2UnitStrc*)pArg;
    PLAYERLIST_AllocPlayerList(pGame, pPlayer, pUnit);
    PLAYERLIST_AllocPlayerList(pGame, pUnit, pPlayer);

    D2GAME_PACKETS_SendPacket0x8D_6FC3F960(pUnit, pPlayer);

    sub_6FCBA980(pPlayer, pUnit, 0);

    if (SUNIT_GetPartyId(pPlayer) == -1)
    {
        sub_6FCBA980(pUnit, pPlayer, 0);
    }
    else
    {
        sub_6FCBA980(pUnit, pPlayer, 1);
    }

    sub_6FCBAA20(pUnit, pPlayer);
    sub_6FCBAA20(pPlayer, pUnit);

    for (D2PlayerListStrc* i = UNITS_GetPlayerData(pPlayer)->pPlayerList; i; i = i->pNext)
    {
        D2GAME_PACKETS_SendPacket0x8C_6FC3F8B0(pUnit, pPlayer ? pPlayer->dwUnitId : -1, i->nUnitGUID, i->dwFlags);
    }
}

//D2Game.0x6FCBAD60
void __fastcall PLAYERLIST_sub_6FCBAD60(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    for (D2PlayerListStrc* pPlayerList = pPlayerData->pPlayerList; pPlayerList; pPlayerList = pPlayerList->pNext)
    {
        if (pPlayerList->field_C == 2)
        {
            D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pPlayerList->nUnitGUID);
            if (pOtherPlayer)
            {
                pPlayerList->field_C = PARTY_GetPartyIdForUnitOwner(pGame, pOtherPlayer) != -1;
                sub_6FCBAA20(pPlayer, pOtherPlayer);
            }
        }
    }
}

//D2Game.0x6FCBAE20
int32_t __fastcall PLAYERLIST_GetHostileDelay(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2)
{
    if (!pPlayer2)
    {
        D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer1);
        if (pPlayerData)
        {
            return pPlayerData->dwHostileDelay;
        }
    }

    if (pPlayer1 == pPlayer2)
    {
        return 0;
    }

    D2PlayerListStrc* pPlayerList = PLAYERLIST_GetPlayerListRecordFromUnitGUID(pPlayer1, pPlayer2 ? pPlayer2->dwUnitId : -1);
    if (!pPlayerList)
    {
        return 0;
    }
 
    return pPlayerList->field_8;
}

//D2Game.0x6FCBAE70
void __fastcall PLAYERLIST_SetHostileDelay(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t a3)
{
    if (pPlayer1 != pPlayer2)
    {
        D2PlayerListStrc* pPlayerList = PLAYERLIST_GetPlayerListRecordFromUnitGUID(pPlayer1, pPlayer2 ? pPlayer2->dwUnitId : -1);
        if (pPlayerList)
        {
            pPlayerList->field_8 = a3;
        }
    }

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer1);
    if (pPlayerData)
    {
        pPlayerData->dwHostileDelay = GetTickCount();
    }
}

//D2Game.0x6FCBAED0
void __fastcall sub_6FCBAED0()
{
    dword_6FD4DC40 = GetTickCount();
}
