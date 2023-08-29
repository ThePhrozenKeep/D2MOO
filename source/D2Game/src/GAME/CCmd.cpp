#include "GAME/CCmd.h"

#include <Storm.h>

// D2Net
#include <Server.h>

#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "PLAYER/PlrMsg.h"


#pragma warning(disable: 28159)

constexpr int32_t CHARACTER_SAVE_SIZE = 0x2000;


//D2Game.0x6FC317F0
int32_t __fastcall CCMD_IsStringZeroTerminated(const char* pData, int32_t nDataSize)
{
    for (int32_t i = 0; i < nDataSize; ++i)
    {
        if (pData[i] == '\0')
        {
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC31810
uint32_t __fastcall CCMD_CanClientJoinGame(int32_t a1, int32_t a2, char* szClientName)
{
    // TODO: Names
    if (a1 != 1)
    {
        return 6;
    }

    const int32_t v6 = sub_6FC35840(1u);
    if (!v6)
    {
        return 6;
    }

    D2GameStrc* pGame = GAME_GetGame(v6);
    if (!pGame)
    {
        return 6;
    }

    if (a2 != 10)
    {
        GAME_LeaveGamesCriticalSection(pGame);
        return 16;
    }

    if (pGame->nClients >= 8)
    {
        GAME_LeaveGamesCriticalSection(pGame);
        return 15;
    }

    if (!szClientName[0] || !szClientName[1] || !CCMD_IsStringZeroTerminated(szClientName, 16))
    {
        GAME_LeaveGamesCriticalSection(pGame);
        return 6;
    }
  
    for (D2ClientStrc* pClient = pGame->pClientList; pClient; pClient = CLIENTS_GetNext(pClient))
    {
        const char* szName = CLIENTS_GetName(pClient);
        if (szName && !SStrCmpI(szName, szClientName, 16))
        {
            GAME_LeaveGamesCriticalSection(pGame);
            return 6;
        }
    }

    GAME_LeaveGamesCriticalSection(pGame);
    return 0;
}

//D2Game.0x6FC31910
void __fastcall CCMD_ProcessClientSystemMessage(void* pData, int32_t nSize)
{
    // TODO: Names
    const int32_t nClientId = *(int32_t*)pData;
    int32_t pPacket = (int32_t)((char*)pData + 4);

    switch (*((uint8_t*)pPacket))
    {
    case D2CLTSYS_NEWGAME:
    {
        D2GSPacketClt66* pPacket66 = (D2GSPacketClt66*)pPacket;
        if (GAME_VerifyCreateNewGame(nClientId, pPacket66))
        {
            GAME_SendGameInit(nClientId, pPacket66->szGameName, pPacket66->bSkipVerification, pPacket66->nPlayerClass, pPacket66->szClientName, pPacket66->unk0x25, pPacket66->nGameFlags, pPacket66->unk0x13, pPacket66->unk0x2B, pPacket66->unk0x2C, pPacket66->unk0x14, pPacket66->nLocale, 0, 0);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_NEWGAME)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_JOINGAME:
    {
        D2GSPacketClt67* pPacket67 = (D2GSPacketClt67*)pPacket;
        if (!gpD2ServerCallbackFunctions_6FD45830)
        {
            const uint32_t nErrorCode = CCMD_CanClientJoinGame(pPacket67->nGameId, pPacket67->unk0x08, pPacket67->szClientName);
            if (nErrorCode)
            {
                sub_6FC3C6D0(nClientId, nErrorCode);
                D2NET_10015(nClientId, __FILE__, __LINE__);
            }
        }

        char szAccountName[16] = {};
        int32_t v17 = 0;
        int32_t v18 = 0;
        int32_t v19 = 0;
        if (GAME_VerifyJoinGme(nClientId, pPacket67->nGameId, pPacket67->nPlayerClass, pPacket67->szClientName, pPacket67->unk0x01, szAccountName, &v17, pPacket67->nLocale, &v19, &v18))
        {
            GAME_JoinGame(nClientId, pPacket67->nGameId, pPacket67->nPlayerClass, pPacket67->szClientName, szAccountName, v17, pPacket67->nLocale, v19, v18);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_JOINGAME)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_ENDGAME:
    {
        if (GAME_VerifyEndGame(nClientId))
        {
            GAME_EndGame(nClientId, 1);
        }
        break;
    }
    case D2CLTSYS_JOINLIST:
    {
        if (D2GAME_Return1_6FC41900())
        {
            sub_6FC39870(nClientId);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_JOINLIST)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_JOINACT:
    {
        if (GAME_VerifyJoinAct(nClientId))
        {
            GAME_SendActInit(nClientId);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_JOINACT)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_OPENCHAR:
    {
        D2GSPacketClt6B* pPacket6B = (D2GSPacketClt6B*)pPacket;
        D2_ASSERT(pPacket6B->nTotalSize < CHARACTER_SAVE_SIZE);

        if (D2GAME_Return1_6FC41900())
        {
            sub_6FC399A0(nClientId, pPacket6B);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_OPENCHAR)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_0x6C:
    {
        D2GSPacketClt6C* pPacket6C = (D2GSPacketClt6C*)pPacket;
        CLIENTS_UpdatePing(nClientId, pPacket6C->unk0x01, pPacket6C->unk0x05);
        break;
    }
    case D2CLTSYS_DISCONNECT:
    {
        if (sub_6FC38100(nClientId))
        {
            GAME_DisconnectClientById(nClientId, 0);
        }
        else
        {
            GAME_LogMessage(3, "[HACKLIST]  ProcessCltSysMessage(D2CLTSYS_FORCEDISCONNECT)");
            sub_6FC36B20(nClientId, __FILE__, __LINE__);
        }
        break;
    }
    case D2CLTSYS_REMOVEFROMGAME:
    {
        D2Game_10024_RemoveClientFromGame(nClientId);
        break;
    }
    default:
        return;
    }
}

//D2Game.0x6FC31C00
void __fastcall CCMD_ProcessClientMessage(void* pData, int32_t nPacketSize)
{
    const int32_t nClientId = *(int32_t*)pData;
    uint8_t* pPacket = (uint8_t*)pData + 4;
    D2GameStrc* pGame = GAME_GetGameByClientId(nClientId);
    if (!pGame)
    {
        return;
    }

    D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientId);
    D2_ASSERT(pClient);

    pClient->dwLastPacketTick = GetTickCount();

    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER && D2GAME_PACKET_Handler_6FC89320(pGame, pPlayer, pPacket, nPacketSize) >= 3 && pGame->nGameType != 3)
    {
        GAME_LogMessage(3, "[HACKLIST]  ProcessClientMessage: Client %d '%s' believed to be cheating. Command was %d", nClientId, pClient->szName, *pPacket);
        sub_6FC36C20(pGame, nClientId, __FILE__, __LINE__);
    }

    GAME_LeaveGamesCriticalSection(pGame);
}
