#include "PLAYER/PartyScreen.h"

#include <D2Dungeon.h>
#include <D2StatList.h>

#include "GAME/Clients.h"
#include "GAME/SCmd.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerList.h"
#include "PLAYER/PlrTrade.h"
#include "UNIT/Friendly.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


#pragma warning(disable: 28159)

enum D2PartyScreenType
{
    PARTYSCREENTYPE_ALLOW_MALE_TO_LOOT = 1,
    PARTYSCREENTYPE_ALLOW_FEMALE_TO_LOOT = 2,
    PARTYSCREENTYPE_HOSTILE = 3,
    PARTYSCREENTYPE_NO_LONGER_HOSTILE = 4,
    PARTYSCREENTYPE_RECEIVED_INVITE = 5,
    PARTYSCREENTYPE_INVITATION_CANCELLED = 6,
    PARTYSCREENTYPE_HAS_JOINED_PARTY = 7,
    PARTYSCREENTYPE_NOW_ALLIED_WITH = 8,
    PARTYSCREENTYPE_HAS_LEFT_PARTY = 9,
    PARTYSCREENTYPE_DISALLOW_MALE_TO_LOOT = 10,
    PARTYSCREENTYPE_DISALLOW_FEMALE_TO_LOOT = 11,
};


#pragma pack(push, 1)
struct D2PartyScreenCallbackStrc
{
    D2UnitStrc* pPlayer1;
    D2UnitStrc* pPlayer2;
    int16_t nPartyId;
};
#pragma pack(pop)


//D2Game.0x6FC7AB50
void __fastcall PARTYSCREEN_ToggleLootability(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure)
{
    D2_MAYBE_UNUSED(pFailure);
    if (!CLIENTS_CheckFlag(SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__), CLIENTSAVEFLAG_HARDCORE))
    {
        return;
    }

    PLAYERLIST_AllocPlayerList(pGame, pPlayer1, pPlayer2);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer2, pPlayer1);

    if (PLAYERLIST_CheckFlag(pPlayer1, pPlayer2, 1) != nParam)
    {
        PLAYERLIST_ToggleFlag(pGame, pPlayer1, pPlayer2, 1, nParam);
    }

    SCMD_Send0x75_PartyRosterUpdate(pPlayer1, pPlayer2);
    SCMD_Send0x75_PartyRosterUpdate(pPlayer2, pPlayer1);

    uint8_t nType = 0;
    if (pPlayer1 && (pPlayer1->dwClassId == PCLASS_AMAZON || pPlayer1->dwClassId == PCLASS_SORCERESS || pPlayer1->dwClassId == PCLASS_ASSASSIN))
    {
        nType = nParam != 0 ? PARTYSCREENTYPE_ALLOW_FEMALE_TO_LOOT : PARTYSCREENTYPE_DISALLOW_FEMALE_TO_LOOT;
    }
    else
    {
        nType = nParam != 0 ? PARTYSCREENTYPE_ALLOW_MALE_TO_LOOT : PARTYSCREENTYPE_DISALLOW_MALE_TO_LOOT;
    }

    PARTYSCREEN_SendEventUpdateToClient(SUNIT_GetClientFromPlayer(pPlayer2, __FILE__, __LINE__), pPlayer1 ? pPlayer1->dwUnitId : -1, nType);
}

//D2Game.0x6FC7AC20
void __fastcall PARTYSCREEN_SendEventUpdateToClient(D2ClientStrc* pClient, int32_t nUnitGUID, uint8_t nType)
{
    D2GSPacketSrv5A packet5A = {};
    packet5A.nHeader = 0x5A;
    packet5A.nType = EVENTTYPE_PVPSTATUS;
    packet5A.dwParam = nUnitGUID;
    packet5A.nParam = nType;

    switch (nType)
    {
    case PARTYSCREENTYPE_ALLOW_MALE_TO_LOOT:
    case PARTYSCREENTYPE_ALLOW_FEMALE_TO_LOOT:
        packet5A.nColor = STRCOLOR_DARK_GOLD;
        break;

    case PARTYSCREENTYPE_HOSTILE:
        packet5A.nColor = STRCOLOR_ORANGE;
        break;

    case PARTYSCREENTYPE_NO_LONGER_HOSTILE:
        packet5A.nColor = STRCOLOR_WHITE;
        break;

    case PARTYSCREENTYPE_RECEIVED_INVITE:
    case PARTYSCREENTYPE_HAS_JOINED_PARTY:
    case PARTYSCREENTYPE_NOW_ALLIED_WITH:
        packet5A.nColor = STRCOLOR_LIGHTGREEN;
        break;

    case PARTYSCREENTYPE_INVITATION_CANCELLED:
    case PARTYSCREENTYPE_HAS_LEFT_PARTY:
    case PARTYSCREENTYPE_DISALLOW_MALE_TO_LOOT:
    case PARTYSCREENTYPE_DISALLOW_FEMALE_TO_LOOT:
        packet5A.nColor = STRCOLOR_YELLOW;
        break;

    default:
        packet5A.nColor = STRCOLOR_BLUE;
        break;
    }

    D2GAME_PACKETS_SendPacket0x5A_6FC3DEC0(pClient, &packet5A);
}

//D2Game.0x6FC7AD10
void __fastcall PARTYSCREEN_ToggleIgnore(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure)
{
    D2_MAYBE_UNUSED(pFailure);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer1, pPlayer2);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer2, pPlayer1);

    if (PLAYERLIST_CheckFlag(pPlayer1, pPlayer2, 2) != nParam)
    {
        PLAYERLIST_ToggleFlag(pGame, pPlayer1, pPlayer2, 2, nParam);
    }

    SCMD_Send0x75_PartyRosterUpdate(pPlayer1, pPlayer2);
    SCMD_Send0x75_PartyRosterUpdate(pPlayer2, pPlayer1);
}

//D2Game.0x6FC7AD70
void __fastcall PARTYSCREEN_ToggleSquelch(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure)
{
    D2_MAYBE_UNUSED(pFailure);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer1, pPlayer2);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer2, pPlayer1);

    if (PLAYERLIST_CheckFlag(pPlayer1, pPlayer2, 4) != nParam)
    {
        PLAYERLIST_ToggleFlag(pGame, pPlayer1, pPlayer2, 4, nParam);
    }

    SCMD_Send0x75_PartyRosterUpdate(pPlayer1, pPlayer2);
    SCMD_Send0x75_PartyRosterUpdate(pPlayer2, pPlayer1);
}

//D2Game.0x6FC7ADD0
void __fastcall D2GAME_PARTYSCREEN_Command8_6FC7ADD0(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure)
{
    D2_MAYBE_UNUSED(nParam);
    D2_MAYBE_UNUSED(pFailure);
    int16_t nPartyId = SUNIT_GetPartyId(pPlayer2);

    PLAYERLIST_AllocPlayerList(pGame, pPlayer1, pPlayer2);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer2, pPlayer1);

    if (sub_6FCBA9D0(pPlayer1, pPlayer2) != 2)
    {
        sub_6FCBAA20(pPlayer2, pPlayer1);
        sub_6FCBAA20(pPlayer1, pPlayer2);
        return;
    }

    if (nPartyId == -1)
    {
        nPartyId = sub_6FCB9C40(pGame);
        sub_6FCB9D10(pGame, nPartyId, pPlayer2);
        sub_6FCB9D10(pGame, nPartyId, pPlayer1);
        PLAYERLIST_sub_6FCBAD60(pGame, pPlayer2);
    }
    else
    {
        sub_6FCB9D10(pGame, nPartyId, pPlayer1);
    }

    PLAYERLIST_sub_6FCBAD60(pGame, pPlayer1);

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__);
    PARTYSCREEN_SendEventUpdateToClient(pClient, pPlayer2 ? pPlayer2->dwUnitId : -1, PARTYSCREENTYPE_NOW_ALLIED_WITH);

    D2PartyScreenCallbackStrc arg = {};
    arg.pPlayer1 = pPlayer2;
    arg.pPlayer2 = pPlayer1;
    arg.nPartyId = nPartyId;
    SUNIT_IterateLivingPlayers(pGame, sub_6FC7AED0, &arg);
}

//Inlined in D2Game.0x6FC7AED0
void __fastcall inlinedfunction(D2UnitStrc* pLocalPlayer, D2UnitStrc* pOtherPlayer)
{
    int32_t nLocalGUID = -1;
    if (pLocalPlayer)
    {
        nLocalGUID = pLocalPlayer->dwUnitId;
    }

    int32_t nOtherGUID = -1;
    if (pOtherPlayer)
    {
        nOtherGUID = pOtherPlayer->dwUnitId;
    }

    if (nLocalGUID == nOtherGUID)
    {
        return;
    }

    const int16_t nPartyId = SUNIT_GetPartyId(pLocalPlayer);
    if (nPartyId != -1)
    {
        sub_6FCBA980(pOtherPlayer, pLocalPlayer, 1);
        sub_6FCBA980(pLocalPlayer, pOtherPlayer, 1);
    }
    else if (sub_6FCBA9D0(pOtherPlayer, pLocalPlayer) == 4)
    {
        sub_6FCBA980(pLocalPlayer, pOtherPlayer, 2);
    }
    else
    {
        sub_6FCBA980(pOtherPlayer, pLocalPlayer, 0);
        sub_6FCBA980(pLocalPlayer, pOtherPlayer, 1);
    }
}

//D2Game.0x6FC7AED0
void __fastcall sub_6FC7AED0(D2GameStrc* pGame, D2UnitStrc* pLocalPlayer, void* a3)
{
    D2PartyScreenCallbackStrc* pArg = (D2PartyScreenCallbackStrc*)a3;
    D2UnitStrc* pPlayer1 = pArg->pPlayer1;
    D2UnitStrc* pPlayer2 = pArg->pPlayer2;

    const int16_t nPartyId = SUNIT_GetPartyId(pLocalPlayer);
    PLAYERLIST_AllocPlayerList(pGame, pLocalPlayer, pPlayer1);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer1, pLocalPlayer);
    PLAYERLIST_AllocPlayerList(pGame, pLocalPlayer, pPlayer2);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer2, pLocalPlayer);

    inlinedfunction(pLocalPlayer, pPlayer1);
    inlinedfunction(pLocalPlayer, pPlayer2);

    if (nPartyId == pArg->nPartyId && nPartyId != -1 && pLocalPlayer != pPlayer2)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pLocalPlayer, __FILE__, __LINE__);
        PARTYSCREEN_SendEventUpdateToClient(pClient, pPlayer2 ? pPlayer2->dwUnitId : -1, PARTYSCREENTYPE_HAS_JOINED_PARTY);
    }

    sub_6FCBAA20(pLocalPlayer, pPlayer1);
    sub_6FCBAA20(pLocalPlayer, pPlayer2);
    sub_6FC3F8F0(pGame, pLocalPlayer);
}

//D2Game.0x6FC7B030
void __fastcall PARTYSCREEN_InvitationCancelled(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure)
{
    D2_MAYBE_UNUSED(nParam);
    D2_MAYBE_UNUSED(pFailure);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer1, pPlayer2);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer2, pPlayer1);

    if (sub_6FCBA9D0(pPlayer1, pPlayer2) == 4)
    {
        sub_6FCBA980(pPlayer1, pPlayer2, SUNIT_GetPartyId(pPlayer2) != -1);
        sub_6FCBA980(pPlayer2, pPlayer1, SUNIT_GetPartyId(pPlayer1) != -1);

        PARTYSCREEN_SendEventUpdateToClient(SUNIT_GetClientFromPlayer(pPlayer2, __FILE__, __LINE__), pPlayer1 ? pPlayer1->dwUnitId : -1, PARTYSCREENTYPE_INVITATION_CANCELLED);
    }

    sub_6FCBAA20(pPlayer1, pPlayer2);
    sub_6FCBAA20(pPlayer2, pPlayer1);
}

//D2Game.0x6FC7B0E0
void __fastcall PARTYSCREEN_ReceivedInvitation(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure)
{
    D2_MAYBE_UNUSED(nParam);
    D2_MAYBE_UNUSED(pFailure);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer1, pPlayer2);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer2, pPlayer1);

    if (!sub_6FCBA9D0(pPlayer1, pPlayer2))
    {
        sub_6FCBA980(pPlayer1, pPlayer2, 4);
        sub_6FCBA980(pPlayer2, pPlayer1, 2);

        PARTYSCREEN_SendEventUpdateToClient(SUNIT_GetClientFromPlayer(pPlayer2, __FILE__, __LINE__), pPlayer1 ? pPlayer1->dwUnitId : -1, PARTYSCREENTYPE_RECEIVED_INVITE);
    }

    sub_6FCBAA20(pPlayer1, pPlayer2);
    sub_6FCBAA20(pPlayer2, pPlayer1);
}

//D2Game.0x6FC7B160
void __fastcall PARTYSCREEN_PlayerLeftParty(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure)
{
    D2_MAYBE_UNUSED(pPlayer2);
    D2_MAYBE_UNUSED(nParam);
    D2_MAYBE_UNUSED(pFailure);
    PARTY_IteratePartyMembers(pGame, SUNIT_GetPartyId(pPlayer1), PARTYSCREEN_PartyIterate_PlayerLeftParty, pPlayer1);
    PARTY_LeaveParty(pGame, pPlayer1);
}

//D2Game.0x6FC7B190
void __fastcall PARTYSCREEN_PartyIterate_PlayerLeftParty(D2GameStrc* pGame, D2UnitStrc* pPlayer1, void* pArg)
{
    D2_MAYBE_UNUSED(pGame);
    D2UnitStrc* pPlayer2 = (D2UnitStrc*)pArg;
    if (pPlayer1 != pPlayer2)
    {
        PARTYSCREEN_SendEventUpdateToClient(SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__), pPlayer2 ? pPlayer2->dwUnitId : -1, PARTYSCREENTYPE_HAS_LEFT_PARTY);
    }
}

//D2Game.0x6FC7B1D0
void __fastcall PARTYSCREEN_ToggleHostile(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nParam, int32_t* pFailure)
{
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayer1);
    if (!pRoom || !DUNGEON_IsRoomInTown(pRoom) || STATLIST_GetUnitBaseStat(pPlayer1, STAT_LEVEL, 0) < 9 || STATLIST_GetUnitBaseStat(pPlayer2, STAT_LEVEL, 0) < 9)
    {
        *pFailure = 1;
        return;
    }

    PLAYERLIST_AllocPlayerList(pGame, pPlayer1, pPlayer2);
    PLAYERLIST_AllocPlayerList(pGame, pPlayer2, pPlayer1);

    if (PLAYERLIST_CheckFlag(pPlayer1, pPlayer2, 8) == nParam)
    {
        sub_6FCBAAA0(pGame, pPlayer1, pPlayer2);
        sub_6FCBAAA0(pGame, pPlayer2, pPlayer1);
        return;
    }

    if (!PLAYERLIST_CheckFlag(pPlayer1, pPlayer2, 8) && STATLIST_UnitGetStatValue(pPlayer2, STAT_LEVEL, 0) < 9)
    {
        return;
    }

    if (!nParam)
    {
        FRIENDLY_RemoveHostility(pGame, pPlayer1, pPlayer2);
        PARTYSCREEN_SendEventUpdateToClient(SUNIT_GetClientFromPlayer(pPlayer2, __FILE__, __LINE__), pPlayer1 ? pPlayer1->dwUnitId : -1, PARTYSCREENTYPE_NO_LONGER_HOSTILE);
        return;
    }

    if (PLAYERLIST_GetHostileDelay(pPlayer1, pPlayer2) > GetTickCount())
    {
        SUNIT_AttachSound(pPlayer1, 0x13u, pPlayer1);
        PLRTRADE_SendEventPacketToPlayer(pPlayer1, EVENTTYPE_PLEASEWAITHOSTILE, nullptr);
        sub_6FCBAAA0(pGame, pPlayer1, pPlayer2);
        sub_6FCBAAA0(pGame, pPlayer2, pPlayer1);
        return;
    }

    PLAYERLIST_SetHostileDelay(pPlayer1, pPlayer2, GetTickCount() + 60000);

    const int32_t nPartyId2 = SUNIT_GetPartyId(pPlayer2);
    if (nPartyId2 == -1)
    {
        FRIENDLY_OpenHostility(pGame, pPlayer1, pPlayer2);
        FRIENDLY_OpenHostility(pGame, pPlayer2, pPlayer1);

        PARTYSCREEN_SendEventUpdateToClient(SUNIT_GetClientFromPlayer(pPlayer2, __FILE__, __LINE__), pPlayer1 ? pPlayer1->dwUnitId : -1, PARTYSCREENTYPE_HOSTILE);
    }
    else if (STATLIST_GetUnitBaseStat(pPlayer1, STAT_LEVEL, 0) >= 9)
    {
        PARTY_IteratePartyMembers(pGame, nPartyId2, sub_6FC7B3F0, pPlayer1);
    }

    sub_6FC7C170(pGame, pPlayer1);

    const int32_t nPartyId1 = SUNIT_GetPartyId(pPlayer1);
    if (nPartyId1 != -1 && nPartyId1 == SUNIT_GetPartyId(pPlayer2))
    {
        PARTY_IteratePartyMembers(pGame, SUNIT_GetPartyId(pPlayer1), PARTYSCREEN_PartyIterate_PlayerLeftParty, pPlayer1);
        PARTY_LeaveParty(pGame, pPlayer1);
    }
}

//D2Game.0x6FC7B3F0
void __fastcall sub_6FC7B3F0(D2GameStrc* pGame, D2UnitStrc* pPlayer1, void* pArg)
{
    D2UnitStrc* pPlayer2 = (D2UnitStrc*)pArg;
    if (pPlayer2 == pPlayer1 || STATLIST_GetUnitBaseStat(pPlayer1, STAT_LEVEL, 0) < 9)
    {
        return;
    }

    FRIENDLY_OpenHostility(pGame, pPlayer2, pPlayer1);
    FRIENDLY_OpenHostility(pGame, pPlayer1, pPlayer2);

    PARTYSCREEN_SendEventUpdateToClient(SUNIT_GetClientFromPlayer(pPlayer1, __FILE__, __LINE__), pPlayer2 ? pPlayer2->dwUnitId : -1, PARTYSCREENTYPE_HOSTILE);
}

//D2Game.0x6FC7B450
void __fastcall sub_6FC7B450(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    SUNIT_IterateLivingPlayers(pGame, sub_6FC7B460, pPlayer);
}

//D2Game.0x6FC7B460
void __fastcall sub_6FC7B460(D2GameStrc* pGame, D2UnitStrc* pPlayer1, void* pArg)
{
    D2UnitStrc* pPlayer2 = (D2UnitStrc*)pArg;
    if (pPlayer1 == pPlayer2)
    {
        sub_6FCBA980(pPlayer1, pPlayer1, 1);
        SCMD_Send0x75_PartyRosterUpdate(pPlayer1, pPlayer1);
    }
    else
    {
        sub_6FCBA980(pPlayer1, pPlayer2, 0);
        sub_6FCBA980(pPlayer2, pPlayer1, SUNIT_GetPartyId(pPlayer1) != -1);

        SCMD_Send0x75_PartyRosterUpdate(pPlayer1, pPlayer2);
        SCMD_Send0x75_PartyRosterUpdate(pPlayer2, pPlayer1);
    }
}

//D2Game.0x6FC7B4C0
int32_t __fastcall PARTYSCREEN_CommandHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCallbackId, int32_t nOtherPlayerGUID, int32_t nParam)
{
    constexpr void(__fastcall* gpfPartyCommands_6FD28E60[10])(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, int32_t, int32_t*) =
    {
        nullptr,
        PARTYSCREEN_ToggleLootability,
        PARTYSCREEN_ToggleIgnore,
        PARTYSCREEN_ToggleSquelch,
        PARTYSCREEN_ToggleHostile,
        nullptr,
        PARTYSCREEN_ReceivedInvitation,
        PARTYSCREEN_InvitationCancelled,
        D2GAME_PARTYSCREEN_Command8_6FC7ADD0,
        PARTYSCREEN_PlayerLeftParty,
    };

    D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nOtherPlayerGUID);
    if (!pOtherPlayer || !gpfPartyCommands_6FD28E60[nCallbackId])
    {
        return 0;
    }

    if (IsBadCodePtr((FARPROC)gpfPartyCommands_6FD28E60[nCallbackId]))
    {
        FOG_DisplayAssert("sgcptPartyCommands[eCommand].pfnProcess", __FILE__, __LINE__);
        exit(-1);
    }

    int32_t bFailure = 0;
    gpfPartyCommands_6FD28E60[nCallbackId](pGame, pUnit, pOtherPlayer, nParam, &bFailure);
    if (bFailure)
    {
        return 3;
    }

    return 0;
}
