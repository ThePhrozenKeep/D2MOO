#include "GAME/Arena.h"

#include "D2DataTbls.h"
#include <Units/UnitRoom.h>

#include "GAME/Clients.h"
#include "GAME/SCmd.h"


#pragma warning(disable: 26812)
#pragma warning(disable: 33010)


int32_t dword_6FD43FB0;


//D2Game.0x6FC31010
int32_t __fastcall D2Game_10001_Return0()
{
    return dword_6FD43FB0;
}

//D2Game.0x6FC31040
void __fastcall ARENA_AllocArena(D2GameStrc* pGame, int32_t nUnused, uint32_t nFlags, int32_t nTemplate)
{
    D2ArenaStrc* pArena = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2ArenaStrc);

    pGame->pArenaCtrl = pArena;
    pArena->fFlags = nFlags & (0x200000 | ARENAFLAG_ALLOWPARTY | ARENAFLAG_ACTIVE | 0x4000 | 0x2000 | 0x1000 | ARENAFLAG_HARDCORE | 0x100 | 0x80 | 0x40 | ARENAFLAG_UPDATECLIENTS | ARENAFLAG_ARENAMODE | 0x01);
    pArena->nTemplate = nTemplate;
}

//D2Game.0x6FC31090
void __fastcall ARENA_FreeArena(D2GameStrc* pGame)
{
    if (!pGame->pArenaCtrl)
    {
        return;
    }

    D2_FREE_POOL(pGame->pMemoryPool, pGame->pArenaCtrl);
    pGame->pArenaCtrl = nullptr;
}

//D2Game.0x6FC310C0
void __fastcall ARENA_AllocArenaUnit(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2ArenaUnitStrc* pArenaUnit = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2ArenaUnitStrc);

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    D2_ASSERT(pPlayerData);

    pPlayerData->pArenaUnit = pArenaUnit;
}

//D2Game.0x6FC31110
void __fastcall ARENA_FreeArenaUnit(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    D2_ASSERT(pPlayerData);

    if (!pPlayerData->pArenaUnit)
    {
        return;
    }

    D2_FREE_POOL(pGame->pMemoryPool, pPlayerData->pArenaUnit);
    pPlayerData->pArenaUnit = nullptr;
}

//D2Game.0x6FC31160
int32_t __fastcall ARENA_GetAlternateStartTown(D2GameStrc* pGame)
{
    D2ArenaStrc* pArena = pGame->pArenaCtrl;
    D2_ASSERT(pArena);

    return pArena->nAlternateStartTown;
}

//D2Game.0x6FC31190
void __fastcall ARENA_ProcessKillEvent(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender)
{
    if (!pGame->pArenaCtrl || !pAttacker)
    {
        return;
    }

    if (pAttacker->dwUnitType == UNIT_PLAYER)
    {
        D2ArenaUnitStrc* pArenaUnit = UNITS_GetPlayerData(pAttacker)->pArenaUnit;
        D2_ASSERT(pArenaUnit);

        if (pAttacker == pDefender)
        {
            ARENA_UpdateScore(pGame, pAttacker, pAttacker, ARENASCORE_SUICIDE);
        }
        else if (pDefender)
        {
            if (pDefender->dwUnitType == UNIT_PLAYER)
            {
                ARENA_UpdateScore(pGame, pAttacker, pDefender, ARENASCORE_PLAYERKILL);
                ARENA_UpdateScore(pGame, pAttacker, pDefender, ARENASCORE_PLAYERKILLPERCENT);
                ARENA_UpdateScore(pGame, pDefender, pAttacker, ARENASCORE_PLAYERDEATH);
                ARENA_UpdateScore(pGame, pDefender, pAttacker, ARENASCORE_PLAYERDEATHPERCENT);
            }
            else if (pDefender->dwUnitType == UNIT_MONSTER)
            {
                ARENA_UpdateScore(pGame, pAttacker, pDefender, ARENASCORE_MONSTERKILL);
            }
        }
    }
    else if (pAttacker->dwUnitType == UNIT_MONSTER)
    {
        if (pDefender && pDefender->dwUnitType == UNIT_PLAYER)
        {
            ARENA_UpdateScore(pGame, pDefender, pAttacker, ARENASCORE_MONSTERDEATH);
        }
    }
}

//D2Game.0x6FC31280
void __fastcall ARENA_UpdateScore(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2ArenaScoreTypes eScore)
{
    D2_ASSERT(pAttacker && pAttacker->dwUnitType == UNIT_PLAYER);

    D2ArenaUnitStrc* pArenaUnit = UNITS_GetPlayerData(pAttacker)->pArenaUnit;
    D2_ASSERT(pArenaUnit);

    D2ArenaStrc* pArena = pGame->pArenaCtrl;
    D2_ASSERT(pArena);

    D2ArenaTxt* pArenaTxtRecord = DATATBLS_GetArenaTxtRecord(pArena->nType);
    D2_ASSERT(eScore < NUM_ARENA_SCORES);

    switch (eScore)
    {
    case ARENASCORE_SUICIDE:
    case ARENASCORE_PLAYERKILL:
    case ARENASCORE_MONSTERKILL:
    case ARENASCORE_PLAYERDEATH:
    case ARENASCORE_MONSTERDEATH:
    {
        pArenaUnit->nScore += pArenaTxtRecord->dwScores[eScore];
        break;
    }
    case ARENASCORE_PLAYERKILLPERCENT:
    case ARENASCORE_PLAYERDEATHPERCENT:
    {
        D2ArenaUnitStrc* pSrcArenaUnit = UNITS_GetPlayerData(pDefender)->pArenaUnit;
        D2_ASSERT(pSrcArenaUnit);

        pArenaUnit->nScore += pSrcArenaUnit->nScore * pArenaTxtRecord->dwScores[eScore] / 100;
        break;
    }
    default:
        break;
    }

    pArenaUnit->bUpdateScore = 1;

    pArena->fFlags |= ARENAFLAG_UPDATE;
    UNITROOM_RefreshUnit(pDefender);
}

//D2Game.0x6FC31470
void __fastcall ARENA_SynchronizeWithClients(D2GameStrc* pGame, D2ClientStrc* pClient)
{
    D2ArenaStrc* pArena = pGame->pArenaCtrl;
    if (!pArena || !(pArena->fFlags & ARENAFLAG_UPDATE))
    {
        return;
    }

    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (pPlayer)
    {
        D2ArenaUnitStrc* pArenaUnit = UNITS_GetPlayerData(pPlayer)->pArenaUnit;
        D2_ASSERT(pArenaUnit);

        if (pArenaUnit->bUpdateScore)
        {
            D2GAME_PACKETS_SendPacket0x65_6FC3F5E0(pClient, pPlayer->dwUnitId, pArenaUnit->nScore);
        }
    }

    if (!sub_6FC33C10(pClient))
    {
        return;
    }

    D2UnitStrc* pLocalPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    for (D2ClientStrc* i = pGame->pClientList; i; i = i->pNext)
    {
        D2UnitStrc* pOtherPlayer = CLIENTS_GetPlayerFromClient(i, 0);
        if (i->dwClientState == CLIENTSTATE_INGAME && pOtherPlayer && pOtherPlayer != pLocalPlayer)
        {
            D2ArenaUnitStrc* pArenaUnit = UNITS_GetPlayerData(pOtherPlayer)->pArenaUnit;
            D2_ASSERT(pArenaUnit);

            if (pArenaUnit->bUpdateScore)
            {
                D2GAME_PACKETS_SendPacket0x65_6FC3F5E0(pClient, pOtherPlayer->dwUnitId, pArenaUnit->nScore);
            }
        }
    }
}

//D2Game.0x6FC315C0
void __fastcall ARENA_SendScoresToClient(D2GameStrc* pGame, D2ClientStrc* pClient)
{
    for (D2ClientStrc* i = pGame->pClientList; i; i = i->pNext)
    {
        D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(i, 0);
        if (i->dwClientState == CLIENTSTATE_INGAME && pPlayer)
        {
            D2ArenaUnitStrc* pArenaUnit = UNITS_GetPlayerData(pPlayer)->pArenaUnit;
            D2_ASSERT(pArenaUnit);
            D2GAME_PACKETS_SendPacket0x65_6FC3F5E0(pClient, pPlayer->dwUnitId, pArenaUnit->nScore);
        }
    }
    
    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 1);
    if (pClient->dwClientState != CLIENTSTATE_INGAME && pPlayer)
    {
        D2ArenaUnitStrc* pArenaUnit = UNITS_GetPlayerData(pPlayer)->pArenaUnit;
        D2_ASSERT(pArenaUnit);
        D2GAME_PACKETS_SendPacket0x65_6FC3F5E0(pClient, pPlayer->dwUnitId, pArenaUnit->nScore);
    }
}

//D2Game.0x6FC31690
uint32_t __fastcall ARENA_NeedsClientUpdate(D2GameStrc* pGame)
{
    D2ArenaStrc* pArena = pGame->pArenaCtrl;
    D2_ASSERT(pArena);

    return pArena->fFlags & ARENAFLAG_UPDATECLIENTS;
}

//D2Game.0x6FC316D0
uint32_t __fastcall ARENA_IsInArenaMode(D2GameStrc* pGame)
{
    D2ArenaStrc* pArena = pGame->pArenaCtrl;
    D2_ASSERT(pArena);

    return pArena->fFlags & ARENAFLAG_ARENAMODE;
}

//D2Game.0x6FC31710
uint32_t __fastcall ARENA_IsActive(D2GameStrc* pGame)
{
    D2ArenaStrc* pArena = pGame->pArenaCtrl;
    D2_ASSERT(pArena);

    return pArena->fFlags & ARENAFLAG_ACTIVE;
}

//D2Game.0x6FC31750
uint32_t __fastcall ARENA_GetFlags(D2GameStrc* pGame)
{
    D2ArenaStrc* pArena = pGame->pArenaCtrl;
    D2_ASSERT(pArena);

    return pArena->fFlags;
}

//D2Game.0x6FC31780
int32_t __fastcall ARENA_Return0()
{
    return 0;
}

//D2Game.0x6FC31790
uint32_t __fastcall sub_6FC31790(D2GameStrc* pGame)
{
    D2ArenaStrc* pArena = pGame->pArenaCtrl;
    D2_ASSERT(pArena);

    return pArena->fFlags & 0x100;
}

//D2Game.0x6FC317C0
int32_t __fastcall ARENA_GetTemplateType(D2GameStrc* pGame)
{
    D2ArenaStrc* pArena = pGame->pArenaCtrl;
    D2_ASSERT(pArena);

    return pArena->nTemplate;
}
