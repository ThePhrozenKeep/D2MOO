#include "GAME/Game.h"

#include <algorithm>

#include <timeapi.h>

#include <D2Lang.h>
#include <cstdio>
#include <Fog.h>
#include <Storm.h>
#include <D2Unicode.h>
#include <D2Math.h>

// D2Net
#include <Server.h>

#include <D2Dungeon.h>
#include <DataTbls/ItemsTbls.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/ObjectsTbls.h>
#include <D2Environment.h>
#include <UselessOrdinals.h>
#include <D2StatList.h>

#include "GAME/Arena.h"
#include "GAME/CCmd.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Level.h"
#include "GAME/SCmd.h"
#include "GAME/Task.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "OBJECTS/Objects.h"
#include "OBJECTS/ObjRgn.h"
#include "PLAYER/PlayerList.h"
#include "PLAYER/PlrMsg.h"
#include "PLAYER/PlrSave.h"
#include "QUESTS/Quests.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitInactive.h"
#include "UNIT/SUnitProxy.h"


#pragma warning(disable: 28159)



D2GameStrc* gpGame_6FD457FC;
int32_t gnAct_6FD45824;
D2ServerCallbackFunctions* gpD2ServerCallbackFunctions_6FD45830;
CRITICAL_SECTION gCriticalSection_6FD45800;
int32_t gpGameArray_6FD447F4[1024]; // TODO: Right size?
int32_t gwGameId_6FD2CA04;
D2GameManagerStrc* gpGameDataTbl_6FD45818;
int32_t gbD2ServerCallbackFunctionsInitialized_6FD45834;
int32_t nInitSeed_6FDC2CA08 = -1;
int32_t dword_6FD45820;
int32_t dword_6FD4581C;
int32_t dword_6FD2CA60;
int32_t dword_6FD457F8;
int32_t dword_6FD2CA14;
int32_t gnPeakMemoryUsageInLast10s_6FD45828;
DWORD dword_6FD4582C;
uint32_t dword_6FD45840;
uint32_t dword_6FD45844;
uint32_t dword_6FD45848;
int32_t dword_6FD2CA10;
char byte_6FD447EC[8];

int32_t gnGamesGUIDs_6FD447F8[1024];

int32_t dword_6FD2CA00 = 1;

// TODO: Looks like it's a bigger struct, but unused
int32_t dword_6FD27D88;

volatile LONG gnNumAdminConnections_6FD45838;

constexpr void(__fastcall* gpfGetDescription_6FD2CA64[5])(char*, int32_t) =
{
    GAME_GetPlayerDescription,
    GAME_GetMonsterDescription,
    GAME_GetObjectDescription,
    GAME_GetItemDescription,
    GAME_GetMissileDescription
};



//D2Game.0x6FC356D0
int32_t __stdcall sub_6FC356D0(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5)
{
    return 0;
}

//D2Game.0x6FC35700
int32_t __fastcall sub_6FC35700()
{
    return 0;
}

//D2Game.0x6FC35720
void* __fastcall sub_6FC35720()
{
    return &dword_6FD27D88;
}

//D2Game.0x6FC35740
void GAME_LogMessage(int32_t a1, const char* szFormat, ...)
{
    char szMessage[4096] = {};
    va_list va = nullptr;

    va_start(va, szFormat);
    vsprintf(szMessage, szFormat, va);

    if (gpD2ServerCallbackFunctions_6FD45830 && gpD2ServerCallbackFunctions_6FD45830->pfServerLogMessage)
    {
        gpD2ServerCallbackFunctions_6FD45830->pfServerLogMessage(a1, "%s", szMessage);
    }
    else if (a1 == 1)
    {
        FOG_Trace("%s", szMessage);
    }
}

//D2Game.0x6FC357C0
int32_t __stdcall D2Game_10046()
{
    SRegLoadValue("Diablo II", "PlayerPos", 0, &dword_6FD4582C);
    memset(gnGamesGUIDs_6FD447F8, 0, sizeof(gnGamesGUIDs_6FD447F8));
    InitializeCriticalSection(&gCriticalSection_6FD45800);
    CLIENTS_Initialize();
    D2NET_10019(sub_6FC36B20);
    SUNITPROXY_FillGlobalItemCache();
    return 1;
}

//D2Game.0x6FC35810
int32_t __stdcall D2Game_10050()
{
    CLIENTS_Release();
    DeleteCriticalSection(&gCriticalSection_6FD45800);
    SUNITPROXY_ClearGlobalItemCache();
    gwGameId_6FD2CA04 = 1;
    gpGameDataTbl_6FD45818 = nullptr;
    gnAct_6FD45824 = 0;
    return 1;
}

//D2Game.0x6FC35840
int32_t __fastcall sub_6FC35840(uint16_t nGameId)
{
    int32_t nResult = 0;
    
    EnterCriticalSection(&gCriticalSection_6FD45800);
    
    const int32_t nGUID = gpGameArray_6FD447F4[nGameId];
    if (nGUID && nGUID != -1)
    {
        nResult = nGUID;
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    return nResult;
}

//D2Game.0x6FC35880
void __stdcall D2Game_10002(D2GameManagerStrc* pGameDataTbl, void* pGameList)
{
    D2_ASSERT(pGameDataTbl);

    gpGameDataTbl_6FD45818 = pGameDataTbl;

    D2_ASSERT(pGameList);
}

//D2Game.0x6FC358E0
void __stdcall GAME_SetServerCallbackFunctions(D2ServerCallbackFunctions* pD2ServerCallbackFunctions)
{
    if (pD2ServerCallbackFunctions)
    {
        gpD2ServerCallbackFunctions_6FD45830 = pD2ServerCallbackFunctions;
        gbD2ServerCallbackFunctionsInitialized_6FD45834 = 1;
        //D2COMMON_11277_Return((int32_t)pD2ServerCallbackFunctions->pfLoadComplete);
    }
    else
    {
        gpD2ServerCallbackFunctions_6FD45830 = nullptr;
        gbD2ServerCallbackFunctionsInitialized_6FD45834 = 0;
    }
}

//D2Game.0x6FC35920 (#10010)
void __stdcall GAME_SetInitSeed(int32_t nInitSeed)
{
    nInitSeed_6FDC2CA08 = nInitSeed;
}

//D2Game.0x6FC35930
uint32_t __fastcall D2Game_10011()
{
    int32_t nGUID = 0;
    EnterCriticalSection(&gCriticalSection_6FD45800);

    if (*gnGamesGUIDs_6FD447F8 && *gnGamesGUIDs_6FD447F8 != -1)
    {
        nGUID = *gnGamesGUIDs_6FD447F8;
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    if (gpGameDataTbl_6FD45818)
    {
        EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);

        D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
        if (pGame)
        {
            D2_ASSERT(pGame->lpCriticalSection);
            EnterCriticalSection(pGame->lpCriticalSection);
        }

        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
        gpGame_6FD457FC = pGame;
    }
    else
    {
        gpGame_6FD457FC = nullptr;
    }

    if (!gpGame_6FD457FC)
    {
        return 0;
    }

    const uint32_t nInitSeed = gpGame_6FD457FC->dwInitSeed;
    D2_ASSERT(gpGame_6FD457FC->lpCriticalSection);
    LeaveCriticalSection(gpGame_6FD457FC->lpCriticalSection);
    return nInitSeed;
}

// Helper function
D2GameStrc* GAME_FindGameWithName(const char* szGameName)
{
    for (int nCurrentGameGUID : gnGamesGUIDs_6FD447F8)
    {
        if (nCurrentGameGUID && nCurrentGameGUID != D2GameInvalidGUID)
        {
            BOOL bHasLock = FALSE;
            D2GameManagerStrc_Lock_6FC3B510(gpGameDataTbl_6FD45818, 0, &bHasLock, FALSE);
            if (D2GameStrc* pCurrentGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nCurrentGameGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C))
            {
                D2_LOCK(pCurrentGame->lpCriticalSection);
                bool bGameNameMatches = SStrCmpI(szGameName, pCurrentGame->szGameName, ARRAY_SIZE(D2GameStrc::szGameName)) == 0;
                D2_UNLOCK(pCurrentGame->lpCriticalSection);

                if (bGameNameMatches)
                {
                    D2GameManagerStrc_Unlock_6FC3B540(gpGameDataTbl_6FD45818, 0, bHasLock);
                    return pCurrentGame;
                }
            }
            D2GameManagerStrc_Unlock_6FC3B540(gpGameDataTbl_6FD45818, 0, bHasLock);
        }
    }
    return nullptr;
}

//D2Game.0x6FC35A10
void __fastcall GAME_ResolveGameNameConflict(D2GameStrc* pGameToSanitize, char* szGameName, int32_t a3)
{
    if (!GAME_FindGameWithName(szGameName))
    {
        SStrCopy(pGameToSanitize->szGameName, szGameName, 0x7FFFFFFFu);
    }
    else // Name conflicts
    {
        const int nCharactersForSuffix = 3;
        if (strlen(szGameName) + nCharactersForSuffix >= ARRAY_SIZE(D2GameStrc::szGameName))
        {
            // Truncate so that we have enough space for the game ID
            // Somehow the original game truncated at 10 characters instead of 11 ?
            szGameName[ARRAY_SIZE(D2GameStrc::szGameName) - 1 - nCharactersForSuffix] = '\0';
        }

        char szGameNameWithID[ARRAY_SIZE(D2GameStrc::szGameName)];
        int nGameNumber = 0;
        do
        {
            sprintf(szGameNameWithID, "%s~%d", szGameName, nGameNumber);
            D2_ASSERT(nGameNumber < 100);
            nGameNumber++;
        }
        while (GAME_FindGameWithName(szGameNameWithID) != nullptr); //Note: Original game had a slightly different version of the find loop here, but it did the same thing.
        // Use the new name
        SStrCopy(pGameToSanitize->szGameName, szGameNameWithID, 0x7FFFFFFFu);
    }
}

//D2Game.0x6FC35CB0
int32_t __fastcall GAME_VerifyCreateNewGame(int32_t nClientId, D2GSPacketClt66* pPacket)
{
    if (gpD2ServerCallbackFunctions_6FD45830 && pPacket->bSkipVerification)
    {
        return 0;
    }

    if (!CCMD_IsStringZeroTerminated(pPacket->szClientName, 16))
    {
        GAME_LogMessage(3, "[HACKLIST]  <D2CLTSYS_CREATEGAME>  CLIENTID:%d  ERROR: Invalid player name", nClientId);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyCreateNewGame: *** Invalid player name for client %d '%s' ***", nClientId, pPacket->szClientName);
        return 0;
    }

    if (pPacket->nLocale > 13u)
    {
        GAME_LogMessage(3, "[HACKLIST]  <D2CLTSYS_CREATEGAME>  CLIENTID:%d  ERROR: Invalid locale code", nClientId);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyCreateNewGame: *** Invalid locale code for client %d '%s' ***", nClientId, pPacket->szClientName);
        return 0;
    }

    if (!CCMD_IsStringZeroTerminated(pPacket->szGameName, 16))
    {
        GAME_LogMessage(3, "[HACKLIST]  <D2CLTSYS_CREATEGAME>  PLAYER:%s  ERROR: Invalid game name", pPacket->szGameName);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyCreateNewGame: *** Invalid game name '%s' for client %d '%s' ***", pPacket->szGameName, nClientId, pPacket->szClientName);
        return 0;
    }

    if (!(pPacket->nGameFlags & 6))
    {
        GAME_LogMessage(3, "[HACKLIST]  <D2CLTSYS_CREATEGAME>  PLAYER:%s  GAME:%s  FLAGS:%x  ERROR: Invalid Game Flags", pPacket->szClientName, pPacket->szGameName, pPacket->nGameFlags);
        return 0;
    }

    char szGameName[16] = {};
    if (!sub_6FC33EA0(nClientId, szGameName))
    {
        GAME_LogMessage(3, "[HACKLIST]  <D2CLTSYS_CREATEGAME>  PLAYER:%s  GAME:%s  ERROR: Client already in a game", pPacket->szClientName, pPacket->szGameName);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyCreateNewGame: *** Client %d '%s' already in game '%s' ***", nClientId, pPacket->szClientName, szGameName);
        return 0;
    }

    if (!sub_6FC33F90(pPacket->szClientName, szGameName))
    {
        GAME_LogMessage(3, "[HACKLIST]  <D2CLTSYS_CREATEGAME>  PLAYER:%s  GAME:%s  ERROR: Client already in a game", pPacket->szClientName, pPacket->szGameName);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyCreateNewGame: *** Client %d '%s' already in game '%s' ***", nClientId, pPacket->szClientName, szGameName);
        return 0;
    }

    if (pPacket->nPlayerClass >= 7u)
    {
        GAME_LogMessage(3, "[HACKLIST]  <D2CLTSYS_CREATEGAME>  PLAYER:%s  GAME:%s  CLASS:%d  ERROR: Invalid player class", pPacket->szClientName, pPacket->szGameName, pPacket->nPlayerClass);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyCreateNewGame: *** Invalid player class %d for client %d '%s' ***", pPacket->nPlayerClass, nClientId, pPacket->szClientName);
        return 0;
    }

    return 1;
}

//D2Game.0x6FC35E50
void __stdcall D2Game_10056(int32_t a1)
{
    dword_6FD45820 = 0;
    dword_6FD4581C = a1;
}

//D2Game.0x6FC35E70 (#10047)
int32_t __stdcall GAME_CreateNewEmptyGame(char* szGameName, const char* szPassword, const char* szGameDescription, uint32_t nFlags, uint8_t a5, uint8_t a6, uint8_t a7, uint16_t* pGameId)
{
    // TODO: v21, v22
    if (!gpGameDataTbl_6FD45818)
    {
        return 0;
    }

    EnterCriticalSection(&gCriticalSection_6FD45800);

    int32_t nGameId = gwGameId_6FD2CA04;
    const int32_t nBaseId = gwGameId_6FD2CA04;

    ++gwGameId_6FD2CA04;
    if (gwGameId_6FD2CA04 > 1024u)
    {
        gwGameId_6FD2CA04 = 1;
    }

    if (gpGameArray_6FD447F4[nGameId])
    {
        while (1)
        {
            nGameId = gwGameId_6FD2CA04;

            ++gwGameId_6FD2CA04;
            if (gwGameId_6FD2CA04 > 1024u)
            {
                gwGameId_6FD2CA04 = 1;
            }

            if (nGameId == nBaseId)
            {
                LeaveCriticalSection(&gCriticalSection_6FD45800);
                GAME_LogMessage(6, "[SERVER]  SrvCreateNewEmptyGame: *** Out of game slots! ***");
                return 0;
            }

            if (!gpGameArray_6FD447F4[nGameId])
            {
                break;
            }
        }
    }

    int32_t* v21 = &gpGameArray_6FD447F4[nGameId];
    *v21 = -1;
    LeaveCriticalSection(&gCriticalSection_6FD45800);

    if (!nGameId)
    {
        GAME_LogMessage(6, "[SERVER]  SrvCreateNewEmptyGame: *** Out of game slots! ***");
        return 0;
    }

    EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    do
    {
        while (1)
        {
            ++gpGameDataTbl_6FD45818->pGamesEx.unk0x20;
            if (gpGameDataTbl_6FD45818->pGamesEx.unk0x20)
            {
                break;
            }

            gpGameDataTbl_6FD45818->pGamesEx.unk0x24 = 1;
        }
    }
    while (gpGameDataTbl_6FD45818->pGamesEx.unk0x24 && D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, gpGameDataTbl_6FD45818->pGamesEx.unk0x20, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C));

    const int32_t v22 = gpGameDataTbl_6FD45818->pGamesEx.unk0x20;

    D2GameStrc* pGame = sub_6FC3B590(gpGameDataTbl_6FD45818, 0, gpGameDataTbl_6FD45818->pGamesEx.unk0x20, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C, 0, 0);
    LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

    D2_ASSERT(pGame);
    pGame->lpCriticalSection = D2_ALLOC_STRC_POOL(nullptr, CRITICAL_SECTION);
    InitializeCriticalSection(pGame->lpCriticalSection);

    FOG_CreateNewPoolSystem(&pGame->pMemoryPool, szGameName, 0x8000u, 0x1000u);

    memset(pGame->pUnitList[0], 0, sizeof(pGame->pUnitList[0]));
    memset(pGame->pUnitList[1], 0, sizeof(pGame->pUnitList[1]));
    memset(pGame->pUnitList[2], 0, sizeof(pGame->pUnitList[2]));
    memset(pGame->pUnitList[3], 0, sizeof(pGame->pUnitList[3]));
    memset(pGame->pUnitList[4], 0, sizeof(pGame->pUnitList[4]));

    memset(pGame->dwUniqueFlags, 0, sizeof(pGame->dwUniqueFlags));

    pGame->nServerToken = nGameId;
    pGame->unk0x24 = 1;

    GAME_ResolveGameNameConflict(pGame, szGameName, 0);
    SStrCopy(pGame->szGamePassword, szPassword, 16);
    SStrCopy(pGame->szGameDesc, szGameDescription, 32);

    pGame->nArenaTemplate = a5;
    pGame->unk0x6C = a6;
    pGame->unk0x6E = a7;

    if (nFlags & 0x100000)
    {
        pGame->bExpansion = 1;
    }
    else
    {
        pGame->bExpansion = 0;
    }

    pGame->wItemFormat = pGame->bExpansion != 0 ? 101 : 2;

    if (nFlags & 0x200000)
    {
        pGame->dwGameType = 1;
    }
    else
    {
        pGame->dwGameType = 0;
    }

    pGame->dwLastUsedUnitGUID[0] = 0;
    pGame->dwLastUsedUnitGUID[1] = 0;
    pGame->dwLastUsedUnitGUID[2] = 0;
    pGame->dwLastUsedUnitGUID[3] = 0;
    pGame->dwLastUsedUnitGUID[4] = 0;
    pGame->dwLastUsedUnitGUID[5] = 0;

    pGame->nDifficulty = (nFlags >> 12) & 7;

    if (nInitSeed_6FDC2CA08 == -1)
    {
        pGame->InitSeed = 0;

        LARGE_INTEGER seed = {};
        QueryPerformanceCounter(&seed);
        SEED_InitSeed(&pGame->pGameSeed);
        SEED_InitLowSeed(&pGame->pGameSeed, SEED_GetRandomValue(seed.LowPart));
        pGame->dwInitSeed = SEED_GetRandomValue(ITEMS_RollRandomNumber(&pGame->pGameSeed));
    }
    else
    {
        pGame->InitSeed = 1;
        pGame->dwInitSeed = nInitSeed_6FDC2CA08;
        SEED_InitSeed(&pGame->pGameSeed);
        SEED_InitLowSeed(&pGame->pGameSeed, nInitSeed_6FDC2CA08);
    }

    EVENT_AllocEventQueue(pGame);
    ARENA_AllocArena(pGame, 0, nFlags, 0);
    CLIENTS_SetGameData(pGame);
    PARTY_AllocPartyControl(pGame);
    sub_6FC68110(pGame);
    pGame->dwObjSeed = OBJRGN_AllocObjectControl(pGame);
    SUNITPROXY_InitializeNpcControl(pGame);
    QUESTS_QuestInit(pGame);

    EnterCriticalSection(&gCriticalSection_6FD45800);
    if (*v21 != -1)
    {
        FOG_DisplayAssert("hGameArray[wGameId-1] == RESERVED_SLOT", __FILE__, __LINE__);
        exit(-1);
    }
    *v21 = v22;
    LeaveCriticalSection(&gCriticalSection_6FD45800);

    pGame->nSyncTimer = FOG_10055_GetSyncTime();
    if (pGame->nSyncTimer <= 1)
    {
        pGame->nSyncTimer = 2;
    }

    D2Game_10042((D2TaskStrc*) &pGame[1], 0, (D2LinkStrc*) v22);
    pGame->unk0x1DB8[1] = GetTickCount();
    *pGameId = pGame->nServerToken;

    return 1;
}

//D2Game.0x6FC36280 (#10007)
int32_t __stdcall GAME_ReceiveDatabaseCharacter(int32_t nClientId, const uint8_t* pSaveData, uint16_t nSaveSize, uint16_t nTotalSize, int32_t a5, int32_t a6, int32_t a7, int32_t a8)
{
    if (nTotalSize && !CLIENTS_AttachSaveFile(nClientId, pSaveData, nSaveSize, nTotalSize, a5 == 0, 1, a6))
    {
        return 1;
    }

    if (a5)
    {
        D2GameStrc* pGame = TASK_GetGame_6FC397A0(SERVER_GetClientGameGUID(nClientId));
        if (pGame)
        {
            if (CLIENTS_IsInGame(pGame, nClientId))
            {
                CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
            }

            D2_ASSERT(pGame->lpCriticalSection);
            LeaveCriticalSection(pGame->lpCriticalSection);
        }

        D2NET_10015(nClientId, __FILE__, __LINE__);
        return 0;
    }

    if (!nTotalSize)
    {
        FOG_Trace("Character size was zero");
        
        D2GameStrc* pGame = TASK_GetGame_6FC397A0(SERVER_GetClientGameGUID(nClientId));
        if (pGame)
        {
            if (CLIENTS_IsInGame(pGame, nClientId))
            {
                CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
            }

            D2_ASSERT(pGame->lpCriticalSection);
            LeaveCriticalSection(pGame->lpCriticalSection);
        }

        D2NET_10015(nClientId, __FILE__, __LINE__);
        return 0;
    }

    D2GameStrc* pGame = TASK_GetGame_6FC397A0(SERVER_GetClientGameGUID(nClientId));
    if (!pGame)
    {
        GAME_LogMessage(6, "[SERVER]  SrvRecvDatabaseCharacter: *** Failed SrvLockGame for client %d ***", nClientId);
        return 0;
    }

    if (!CLIENTS_IsInGame(pGame, nClientId))
    {
        D2_ASSERT(pGame->lpCriticalSection);
        LeaveCriticalSection(pGame->lpCriticalSection);

        GAME_LogMessage(6, "[SERVER]  SrvRecvDatabaseCharacter: *** Failed SrvLockGame for client %d ***", nClientId);
        return 0;
    }

    D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientId);
    if (!pClient)
    {
        GAME_LogMessage(6, "[SERVER]  SrvRecvDatabaseCharacter: *** Failed ClientGetFromId for client %d in game %d '%s' ***", nClientId, pGame->nServerToken, pGame->szGameName);

        D2_ASSERT(pGame->lpCriticalSection);
        LeaveCriticalSection(pGame->lpCriticalSection);
        return 0;
    }

    if (pClient->unk0x60 != a8)
    {
        D2_ASSERT(pGame->lpCriticalSection);
        LeaveCriticalSection(pGame->lpCriticalSection);

        D2GameStrc* pGame = TASK_GetGame_6FC397A0(SERVER_GetClientGameGUID(nClientId));
        if (pGame)
        {
            if (CLIENTS_IsInGame(pGame, nClientId))
            {
                CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
            }

            D2_ASSERT(pGame->lpCriticalSection);
            LeaveCriticalSection(pGame->lpCriticalSection);
        }

        D2NET_10015(nClientId, __FILE__, __LINE__);
        return 0;
    }

    pClient->unk0x184[3] = *(int32_t*)a7;
    pClient->unk0x184[4] = *(int32_t*)(a7 + 4);

    D2_ASSERT(pGame->lpCriticalSection);
    LeaveCriticalSection(pGame->lpCriticalSection);

    D2GAME_PACKETS_SendPacket0x4F_StartMercList_6FC3C6F0(pClient, 2u);
    GAME_LogMessage(6, "[SERVER]  SrvRecvDatabaseCharacter: Sent ACTINITDONE for client %d '%s'", nClientId, pClient->szName);
    return nTotalSize;
}

//D2Game.0x6FC36570
void __fastcall GAME_SendGameInit(int32_t nClientId, char* szGameName, uint8_t nGameType, uint8_t nCharTemplate, const char* szClientName, int32_t a2, uint32_t nFlags, int32_t nArenaTemplate, int32_t a9, int32_t a10, uint8_t nDifficulty, uint8_t nExpLost, int32_t a13, int32_t a14)
{
    // TODO: v22
    if (!gpGameDataTbl_6FD45818)
    {
        return;
    }

    EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    do
    {
        while (1)
        {
            ++gpGameDataTbl_6FD45818->pGamesEx.unk0x20;

            if (gpGameDataTbl_6FD45818->pGamesEx.unk0x20)
            {
                break;
            }

            gpGameDataTbl_6FD45818->pGamesEx.unk0x24 = 1;
        }
    }
    while (gpGameDataTbl_6FD45818->pGamesEx.unk0x24 && D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, gpGameDataTbl_6FD45818->pGamesEx.unk0x20, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C));

    const int32_t v22 = gpGameDataTbl_6FD45818->pGamesEx.unk0x20;

    D2GameStrc* pGame = sub_6FC3B590(gpGameDataTbl_6FD45818, 0, v22, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C, 0, 0);
    LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

    if (!pGame)
    {
        return;
    }

    pGame->lpCriticalSection = D2_ALLOC_STRC_POOL(nullptr, CRITICAL_SECTION);
    InitializeCriticalSection(pGame->lpCriticalSection);

    pGame->pMemoryPool = nullptr;

    memset(pGame->pUnitList[0], 0, sizeof(pGame->pUnitList[0]));
    memset(pGame->pUnitList[1], 0, sizeof(pGame->pUnitList[1]));
    memset(pGame->pUnitList[2], 0, sizeof(pGame->pUnitList[2]));
    memset(pGame->pUnitList[3], 0, sizeof(pGame->pUnitList[3]));
    memset(pGame->pUnitList[4], 0, sizeof(pGame->pUnitList[4]));

    memset(pGame->dwUniqueFlags, 0, sizeof(pGame->dwUniqueFlags));

    pGame->unk0x24 = 1;

    EnterCriticalSection(&gCriticalSection_6FD45800);

    int32_t nGameId = gwGameId_6FD2CA04;
    const int32_t nBaseId = nGameId;
    
    ++gwGameId_6FD2CA04;
    if (gwGameId_6FD2CA04 > 1024u)
    {
        gwGameId_6FD2CA04 = 1;
    }

    if (gpGameArray_6FD447F4[nGameId])
    {
        while (1)
        {
            nGameId = gwGameId_6FD2CA04;
            
            ++gwGameId_6FD2CA04;
            if (gwGameId_6FD2CA04 > 1024u)
            {
                gwGameId_6FD2CA04 = 1;
            }

            if (nGameId == nBaseId)
            {
                LeaveCriticalSection(&gCriticalSection_6FD45800);
                nGameId = 0;
                break;
            }

            if (!gpGameArray_6FD447F4[nGameId])
            {
                gpGameArray_6FD447F4[nGameId] = -1;
                LeaveCriticalSection(&gCriticalSection_6FD45800);
                break;
            }
        }
    }
    else
    {
        gpGameArray_6FD447F4[nGameId] = -1;
        LeaveCriticalSection(&gCriticalSection_6FD45800);
    }

    pGame->nServerToken = nGameId;
    GAME_ResolveGameNameConflict(pGame, szGameName, nClientId);
    pGame->nDifficulty = nDifficulty;
    pGame->nGameType = nGameType;
    pGame->dwLastUsedUnitGUID[0] = 0;
    pGame->dwLastUsedUnitGUID[1] = 0;
    pGame->dwLastUsedUnitGUID[2] = 0;
    pGame->dwLastUsedUnitGUID[3] = 0;
    pGame->dwLastUsedUnitGUID[4] = 0;
    pGame->dwLastUsedUnitGUID[5] = 0;
    pGame->nArenaTemplate = nArenaTemplate;

    if (nFlags & 0x100000)
    {
        pGame->bExpansion = 1;
    }
    else
    {
        pGame->bExpansion = 0;
    }

    pGame->wItemFormat = pGame->bExpansion != 0 ? 101 : 2;

    if (nFlags & 0x200000)
    {
        pGame->dwGameType = 1;
    }
    else
    {
        pGame->dwGameType = 0;
    }

    if (nInitSeed_6FDC2CA08 == -1)
    {
        pGame->InitSeed = 0;

        LARGE_INTEGER seed = {};
        QueryPerformanceCounter(&seed);
        SEED_InitSeed(&pGame->pGameSeed);
        SEED_InitLowSeed(&pGame->pGameSeed, SEED_GetRandomValue(seed.LowPart));
        pGame->dwInitSeed = SEED_GetRandomValue(ITEMS_RollRandomNumber(&pGame->pGameSeed));
    }
    else
    {
        pGame->InitSeed = 1;
        pGame->dwInitSeed = nInitSeed_6FDC2CA08;
        SEED_InitSeed(&pGame->pGameSeed);
        SEED_InitLowSeed(&pGame->pGameSeed, nInitSeed_6FDC2CA08);
    }

    EVENT_AllocEventQueue(pGame);
    ARENA_AllocArena(pGame, a2, nFlags, nArenaTemplate);
    CLIENTS_SetGameData(pGame);

    D2ClientStrc* pClient = CLIENTS_AddToGame(pGame, nClientId, nCharTemplate, szClientName, byte_6FD447EC, 0, nExpLost, a13, a14);
    SERVER_SetClientGameGUID(nClientId, v22);

    if (pGame->nGameType == 1)
    {
        CLIENT_SetSaveLoaded_6FC34390(pClient);
    }

    PARTY_AllocPartyControl(pGame);
    sub_6FC68110(pGame);
    pGame->dwObjSeed = OBJRGN_AllocObjectControl(pGame);
    SUNITPROXY_InitializeNpcControl(pGame);
    QUESTS_QuestInit(pGame);

    EnterCriticalSection(&gCriticalSection_6FD45800);
    if (gpGameArray_6FD447F4[pGame->nServerToken] != -1)
    {
        FOG_DisplayAssert("hGameArray[wGameId-1] == RESERVED_SLOT", __FILE__, __LINE__);
        exit(-1);
    }
    gpGameArray_6FD447F4[pGame->nServerToken] = v22;
    LeaveCriticalSection(&gCriticalSection_6FD45800);

    D2GAME_PACKETS_SendPacket0x01_6FC3C7C0(pClient, 1, pGame);
    D2GAME_PACKETS_SendPacket0x4F_StartMercList_6FC3C6F0(pClient, 0);
    CLIENTS_SetClientState(pClient, 1u);
    GAME_LogMessage(6, "[SERVER]  sSrvSendGameInit:      Sent game init to client %d '%s' for game %d '%s'", pClient->dwClientId, pClient->szName, pGame->nServerToken, pGame->szGameName);
    CLIENTS_SetActNo(pClient, gnAct_6FD45824);

    pGame->nSyncTimer = FOG_10055_GetSyncTime();
    if (pGame->nSyncTimer <= 1)
    {
        pGame->nSyncTimer = 2;
    }

    // TODO: 1st argument
    D2Game_10042((D2TaskStrc*)&pGame[1], 0, (D2LinkStrc*)v22);

    if (gpD2ServerCallbackFunctions_6FD45830 && gpD2ServerCallbackFunctions_6FD45830->pfGetDatabaseCharacter)
    {
        FOG_DisplayAssert("!\"This is not supposed to happen!\"", __FILE__, __LINE__);
        exit(-1);
    }

    D2GAME_PACKETS_SendPacket0x4F_StartMercList_6FC3C6F0(pClient, 2u);
}

//D2Game.0x6FC369C0
void __fastcall sub_6FC369C0(D2GameStrc* pGame, D2ClientStrc* pClient)
{
    D2UnitStrc* pNewUnit = CLIENTS_GetPlayerFromClient(pClient, 0);
    D2_ASSERT(pNewUnit);

    for (D2ClientStrc* i = pGame->pClientList; i; i = i->pNext)
    {
        if (i != pClient)
        {
            D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(i, 0);
            if (pPlayer)
            {
                sub_6FC3DF80(pClient, pPlayer, i);
                sub_6FC3DF80(i, pNewUnit, pClient);

                if (pPlayer->pInventory)
                {
                    for (D2CorpseStrc* pCorpse = INVENTORY_GetFirstCorpse(pPlayer->pInventory); pCorpse; pCorpse = INVENTORY_GetNextCorpse(pCorpse))
                    {
                        const int32_t nCorpseGUID = INVENTORY_GetUnitGUIDFromCorpse(pCorpse);
                        if (nCorpseGUID != -1)
                        {
                            D2GAME_PACKETS_SendPacket0x8E_6FC3FA20(pClient, pPlayer->dwUnitId, nCorpseGUID, 1u);
                        }
                    }
                }
            }
        }
    }

    sub_6FC3DF80(pClient, pNewUnit, pClient);
    ARENA_SendScoresToClient(pGame, pClient);
}

//D2Game.0x6FC36AA0
void __fastcall GAME_SendPacket0x5CToAllConnectedClients(D2GameStrc* pGame, D2ClientStrc* pClient)
{
    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (pPlayer)
    {
        for (D2ClientStrc* pClient = pGame->pClientList; pClient; pClient = pClient->pNext)
        {
            if (pClient->dwClientState == 4)
            {
                D2GAME_PACKETS_SendPacket0x5C_6FC3E090(pClient, pPlayer->dwUnitId);
            }
        }
    }
}

//D2Game.0x6FC36AE0
int32_t __fastcall GAME_VerifyJoinAct(int32_t nClientId)
{
    if (CLIENTS_Verify(nClientId, 1))
    {
        return 1;
    }

    GAME_LogMessage(3, "[HACKLIST] <D2CLTSYS_JOINACT>  CLIENT:%d  ERROR: Invalid state to join act.", nClientId);
    GAME_LogMessage(6, "[SERVER]  SrvVerifyJoinAct:      *** Client %d in invalid state ***", nClientId);

    return 0;
}

//D2Game.0x6FC36B20
void __stdcall sub_6FC36B20(int32_t nClientId, const char* szFile, int32_t nLine)
{
    const int32_t nGameGUID = SERVER_GetClientGameGUID(nClientId);
    if (gpGameDataTbl_6FD45818)
    {
        int32_t bLeaveCriticalSection = 0;
        D2GameManagerStrc_Lock_6FC3B510(gpGameDataTbl_6FD45818, 0, &bLeaveCriticalSection, 0);

        D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGameGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
        if (pGame)
        {
            D2_ASSERT(pGame->lpCriticalSection);
            EnterCriticalSection(pGame->lpCriticalSection);

            if (bLeaveCriticalSection)
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            }

            if (CLIENTS_IsInGame(pGame, nClientId)/* && CLIENTS_IsInGame(pGame, nClientId)*/)
            {
                CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
            }

            D2_ASSERT(pGame->lpCriticalSection);
            LeaveCriticalSection(pGame->lpCriticalSection);
        }
        else
        {
            if (bLeaveCriticalSection)
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            }
        }
    }

    D2NET_10015(nClientId, szFile, nLine);
}

//D2Game.0x6FC36C20
void __fastcall sub_6FC36C20(D2GameStrc* pGame, int32_t nClientId, const char* szFile, int32_t nLine)
{
    if (pGame && CLIENTS_IsInGame(pGame, nClientId))
    {
        CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
    }

    D2NET_10015(nClientId, szFile, nLine);
}

//D2Game.0x6FC36C60
void __fastcall GAME_SendActInit(int32_t nClientId)
{
    int32_t bLeaveCriticalSection = 0;
    const int32_t nGameGUID = SERVER_GetClientGameGUID(nClientId);
    if (!gpGameDataTbl_6FD45818)
    {
        return;
    }

    D2GameStrc* pGame = D2GAME_FindAndLockGameByGUID__6FC3B480(gpGameDataTbl_6FD45818, 0, nGameGUID, &bLeaveCriticalSection, 0);
    if (!pGame)
    {
        return;
    }

    D2_ASSERT(pGame->lpCriticalSection);

    EnterCriticalSection(pGame->lpCriticalSection);

    if (bLeaveCriticalSection)
    {
        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    }

    if (CLIENTS_IsInGame(pGame, nClientId))
    {
        D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientId);
        const int32_t nErrorCode = CLIENTS_AddPlayerToGame(pClient, pGame, 1, 0, 0, 0);
        if (nErrorCode)
        {
            sub_6FC3C6D0(nClientId, nErrorCode);
            CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
            D2NET_10015(nClientId, __FILE__, __LINE__);
        }
        else
        {
            D2_ASSERT(pClient);

            LEVEL_LoadAct(pGame, pClient->nAct);
            LEVEL_SynchronizeDayNightCycleWithClient(pGame, pClient);
            CLIENTS_SetClientState(pClient, 2u);
            GAME_LogMessage(6, "[SERVER]  SrvSendActInit:        Sent act init to client %d '%s' for game %d '%s'", pClient->dwClientId, pClient->szName, pGame->nServerToken, pGame->szGameName);
            sub_6FC31EF0(pClient, CLIENTS_GetPlayerFromClient(pClient, 1), pGame, 0, 0, 0);
            CLIENTS_SetClientState(pClient, 3u);
        }

        D2_ASSERT(pGame->lpCriticalSection);
        LeaveCriticalSection(pGame->lpCriticalSection);
    }
    else
    {
        GAME_LeaveGamesCriticalSection(pGame);
    }
}

//D2Game.0x6FC36DF0
int32_t __fastcall GAME_VerifyJoinGme(int32_t nClientId, uint16_t nGameId, uint8_t nPlayerClass, const char* szClientName, int32_t nToken, const char* szAccountName, int32_t* a7, uint8_t nLocale, int32_t* a9, int32_t* a10)
{
    if (!CCMD_IsStringZeroTerminated(szClientName, 16))
    {
        GAME_LogMessage(3, "[HACKLIST] <D2CLTSYS_JOINGAME>  CLIENT:%d  GAMEID:%d  TOKEN:%x  ERROR: Invalid Player Name", nClientId, nGameId, nToken);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyJoinGame:     *** Invalid player name for client %d '%s' ***", nClientId, szClientName);
        return 0;
    }

    if (nLocale >= 13)
    {
        GAME_LogMessage(3, "[HACKLIST] <D2CLTSYS_JOINGAME>  CLIENT:%d  GAMEID:%d  TOKEN:%x  ERROR: Invalid Locale Enum", nClientId, nGameId, nToken);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyJoinGame:     *** Invalid locale enum for client %d '%s' ***", nClientId, szClientName);
        return 0;
    }

    char szGameName[16] = {};
    if (!sub_6FC33EA0(nClientId, szGameName) || !sub_6FC33F90(szClientName, szGameName))
    {
        GAME_LogMessage(3, "[HACKLIST] <D2CLTSYS_JOINGAME>  ACCT:%s  CLIENT:%s  GAMEID:%d  TOKEN:%x  ERROR: Already in a Game", szAccountName, szClientName, nGameId, nToken);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyJoinGame:     *** Client %d '%s' already in game '%s' ***", nClientId, szClientName, szGameName);
        return 0;
    }

    if (!sub_6FC35840(nGameId))
    {
        GAME_LogMessage(3, "[HACKLIST] <D2CLTSYS_JOINGAME>  ACCT:%s  CLIENT:%s  GAMEID:%d  TOKEN:%x  ERROR: Invalid Game ID", szAccountName, szClientName, nGameId, nToken);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyJoinGame:     *** Invalid gameId %d given to client %d '%s' ***", nGameId, nClientId, szClientName);
        return 0;
    }

    if (nPlayerClass >= 7u)
    {
        GAME_LogMessage(3, "[HACKLIST] <D2CLTSYS_JOINGAME>  ACCT:%s  CLIENT:%s  GAMEID:%d  TOKEN:%x  ERROR: Invalid Player Class", szAccountName, szClientName, nGameId, nToken);
        GAME_LogMessage(6, "[SERVER]  SrvVerifyJoinGame:     *** Invalid player class %d for client %d '%s' ***", nPlayerClass, nClientId, szClientName);
        return 0;
    }

    bool bContainsSpecialCharacter = false;

    for (int32_t i = 0; i < 16; ++i)
    {
        bool bContinue = false;

        switch (szClientName[i])
        {
        case '\'':
        case '-':
        case '_':
            if (!bContainsSpecialCharacter && i != 0 && szClientName[i + 1] != '\0')
            {
                bContainsSpecialCharacter = true;
                bContinue = true;
            }
            break;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            bContinue = true;
            break;

        case '\0':
            if (i != 0)
            {
                if (!gpD2ServerCallbackFunctions_6FD45830)
                {
                    return 1;
                }

                if (IsBadCodePtr((FARPROC)gpD2ServerCallbackFunctions_6FD45830->pfFindPlayerToken))
                {
                    FOG_DisplayAssert("ptEventCallbackTable->fpFindPlayerToken", __FILE__, __LINE__);
                    exit(-1);
                }

                if (!gpD2ServerCallbackFunctions_6FD45830->pfFindPlayerToken(szClientName, nToken, nGameId, szAccountName, a7, a9, a10))
                {
                    GAME_LogMessage(3, "[HACKLIST] <D2CLTSYS_JOINGAME>  ACCT:%s  CLIENT:%s  GAMEID:%d  TOKEN:%x  ERROR: Invalid Token", szAccountName, szClientName, nGameId, nToken);
                    GAME_LogMessage(6, "[SERVER]  SrvVerifyJoinGame:     *** Invalid player token %d for client %d '%s' ***", nToken, nClientId, szClientName);
                    return 0;
                }

                return 1;
            }
            break;

        default:
            break;
        }

        if (!bContinue)
        {
            break;
        }
    }

    GAME_LogMessage(3, "[HACKLIST] <D2CLTSYS_JOINGAME>  ACCT:%s  CLIENT:%s  GAMEID:%d  TOKEN:%x  ERROR: Invalid Player Name", szAccountName, szClientName, nGameId, nToken);
    GAME_LogMessage(6, "[SERVER]  SrvVerifyJoinGame:     *** Invalid player name for client %d '%s' ***", nClientId, szClientName);
    return 0;
}

//D2Game.0x6FC37150
void __fastcall GAME_JoinGame(int32_t dwClientId, uint16_t nGameId, int32_t a3, char* szClientName, char* szAccountName, int32_t a6, int32_t a7, int32_t a8, int32_t a9)
{
    if (!gpGameDataTbl_6FD45818)
    {
        return;
    }

    int32_t nGUID = 0;
    EnterCriticalSection(&gCriticalSection_6FD45800);
    if (gpGameArray_6FD447F4[nGameId] && gpGameArray_6FD447F4[nGameId] != -1)
    {
        nGUID = gpGameArray_6FD447F4[nGameId];
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    if (!nGUID)
    {
        GAME_LogMessage(6, "[SERVER]  SrvJoinGame:           *** Game %d does not exist (client %d '%s') ***", nGameId, dwClientId, szClientName);
        return;
    }

    if (!gpGameDataTbl_6FD45818)
    {
        GAME_LogMessage(6, "[SERVER]  SrvJoinGame:           *** Failed to lock game %d (client %d '%s') ***", nGameId, dwClientId, szClientName);
        return;
    }

    EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);

    D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
    if (!pGame)
    {
        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
        GAME_LogMessage(6, "[SERVER]  SrvJoinGame:           *** Failed to lock game %d (client %d '%s') ***", nGameId, dwClientId, szClientName);
        return;
    }

    D2_ASSERT(pGame->lpCriticalSection);

    EnterCriticalSection(pGame->lpCriticalSection);
    LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    SERVER_SetClientGameGUID(dwClientId, nGUID);

    D2ClientStrc* pClient = CLIENTS_AddToGame(pGame, dwClientId, a3, szClientName, szAccountName, a6, a7, a8, a9);
    if (pClient)
    {
        D2GAME_PACKETS_SendPacket0x01_6FC3C7C0(pClient, 1, pGame);
        D2GAME_PACKETS_SendPacket0x4F_StartMercList_6FC3C6F0(pClient, 0);
        CLIENTS_SetClientState(pClient, 1u);
        GAME_LogMessage(6, "[SERVER]  sSrvSendGameInit:      Sent game init to client %d '%s' for game %d '%s'", pClient->dwClientId, pClient->szName, pGame->nServerToken, pGame->szGameName);
        pClient->nAct = gnAct_6FD45824;
        GAME_LogMessage(6, "[SERVER]  SrvJoinGame:           client %d '%s' joined game %d '%s'", dwClientId, szClientName, nGameId, pGame->szGameName);
        
        D2_ASSERT(pGame->lpCriticalSection);

        LeaveCriticalSection(pGame->lpCriticalSection);

        if (gpD2ServerCallbackFunctions_6FD45830 && gpD2ServerCallbackFunctions_6FD45830->pfGetDatabaseCharacter)
        {
            if (IsBadCodePtr((FARPROC)gpD2ServerCallbackFunctions_6FD45830->pfGetDatabaseCharacter))
            {
                FOG_DisplayAssert("ptEventCallbackTable->fpGetDatabaseCharacter", __FILE__, __LINE__);
                exit(-1);
            }

            gpD2ServerCallbackFunctions_6FD45830->pfGetDatabaseCharacter(&pClient->pClientInfo, szClientName, dwClientId, szAccountName);
            sub_6FC33910(pClient);
        }
        else
        {
            D2GAME_PACKETS_SendPacket0x4F_StartMercList_6FC3C6F0(pClient, 2u);
        }
    }
    else
    {
        if (gpD2ServerCallbackFunctions_6FD45830 && gpD2ServerCallbackFunctions_6FD45830->pfUnlockDatabaseCharacter)
        {
            GAME_LogMessage(6, "[SERVER]  SrvJoinGame:           *** Unable to add client %d '%s' to game %d (unlocking character)", dwClientId, szClientName, nGameId);

            if (IsBadCodePtr((FARPROC)gpD2ServerCallbackFunctions_6FD45830->pfUnlockDatabaseCharacter))
            {
                FOG_DisplayAssert("ptEventCallbackTable->fpUnlockDatabaseCharacter", __FILE__, __LINE__);
                exit(-1);
            }

            gpD2ServerCallbackFunctions_6FD45830->pfUnlockDatabaseCharacter(&pGame->nGameData, szClientName, szAccountName);
        }

        D2_ASSERT(pGame->lpCriticalSection);

        LeaveCriticalSection(pGame->lpCriticalSection);

        sub_6FC3C690(dwClientId);
        D2NET_10016(dwClientId);
    }
}

//D2Game.0x6FC37450
void __fastcall GAME_FreeGame(int32_t a1, D2GameStrc* pGame)
{
    if (pGame)
    {
        GAME_LogMessage(6, "[SERVER]  SrvFreeGame:           freeing game %d '%s'", pGame->nServerToken, pGame->szGameName);
    }

    EnterCriticalSection(&gCriticalSection_6FD45800);
    
    for (int32_t i = 0; i < 1024; ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] == a1)
        {
            gnGamesGUIDs_6FD447F8[i] = 0;
        }
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    MONSTERREGION_FreeAll(pGame->pMemoryPool, pGame->pMonReg);
    OBJRGN_FreeObjectControl(pGame);
    QUESTS_QuestFree(pGame);
    SUNITPROXY_FreeNpcControl(pGame);
    CLIENTS_FreeClientsFromGame(pGame);
    LEVEL_RemoveAllUnits(pGame);
    SUNITINACTIVE_FreeInactiveUnitLists(pGame);
    EVENT_FreeEventQueue(pGame);
    PARTY_FreePartyControl(pGame);
    ARENA_FreeArena(pGame);

    for (int32_t i = 0; i < 5; ++i)
    {
        if (pGame->pAct[i])
        {
            DUNGEON_FreeAct(pGame->pAct[i]);
            pGame->pAct[i] = nullptr;
        }
    }

    if (pGame->pMemoryPool)
    {
        FOG_DestroyMemoryPoolSystem(pGame->pMemoryPool);
    }

    LeaveCriticalSection(pGame->lpCriticalSection);
    DeleteCriticalSection(pGame->lpCriticalSection);
    D2_FREE_POOL(nullptr, pGame->lpCriticalSection);
    pGame->lpCriticalSection = nullptr;
}

//D2Game.0x6FC37560
int32_t __fastcall GAME_VerifyEndGame(int32_t nClientId)
{
    if (CLIENTS_Verify(nClientId, 4))
    {
        return 1;
    }

    GAME_LogMessage(3, "[HACKLIST] <D2CLTSYS_ENDGAME>  CLTID:%d  REASON:Tried to end game they weren't in.", nClientId);
    GAME_LogMessage(6, "[SERVER]  SrvVerifyEndGame:      *** Client %d not in a game? ***", nClientId);

    return 0;
}

//D2Game.0x6FC375A0
void __fastcall GAME_TriggerClientSave(D2ClientStrc* pClient, D2GameStrc* pGame)
{
    if (!pClient)
    {
        return;
    }

    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (!pPlayer || !D2GAME_SAVE_WriteFile_6FC8A500(pGame, pPlayer, CLIENTS_GetName(pClient), 1u))
    {
        return;
    }

    for (D2ClientStrc* i = pGame->pClientList; i; i = i->pNext)
    {
        pPlayer = CLIENTS_GetPlayerFromClient(i, 0);
        if (pPlayer)
        {
            if (i != pClient)
            {
                D2GAME_SAVE_WriteFile_6FC8A500(pGame, pPlayer, CLIENTS_GetName(i), 0);
            }

            if (gpD2ServerCallbackFunctions_6FD45830 && gpD2ServerCallbackFunctions_6FD45830->pfUpdateCharacterLadder)
            {
                const int32_t nExperience = STATLIST_GetUnitBaseStat(pPlayer, STAT_EXPERIENCE, 0);
                const int32_t nLevel = STATLIST_GetUnitBaseStat(pPlayer, STAT_LEVEL, 0);
                const char* szClientName = CLIENTS_GetName(i);
                gpD2ServerCallbackFunctions_6FD45830->pfUpdateCharacterLadder(szClientName, pPlayer->dwClassId, nLevel, nExperience, 0, CLIENTS_GetFlags(i), &i->unk0x184[3]);
            }
        }
    }
}

//D2Game.0x6FC37690
void __fastcall GAME_DisconnectClient(D2GameStrc* pGame, D2ClientStrc* pClient, uint8_t a3)
{
    if (!pClient)
    {
        return;
    }

    const int32_t nClientId = CLIENTS_GetClientId(pClient);

    D2GSPacketSrv5A packet5A = {};
    packet5A.nHeader = 0x5Au;
    packet5A.nType = a3;
    packet5A.nColor = 4;
    packet5A.dwParam = 0;
    CLIENTS_CopyAccountNameToBuffer(pClient, &packet5A.szText[16]);
    packet5A.szText[31] = '\0';
    SStrCopy(packet5A.szText, CLIENTS_GetName(pClient), 16u);
    sub_6FC84D40(pGame, &packet5A);

    GAME_LogMessage(6, "[SERVER]  SrvDisconnectClient:   disconnect client %d '%s' from game %d '%s'", nClientId, packet5A.szText, pGame->nServerToken, pGame->szGameName);
    CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
    sub_6FC3C690(nClientId);
    D2NET_10016(nClientId);
}

//D2Game.0x6FC37750
void __fastcall GAME_DisconnectClientById(int32_t nClientId, int32_t a2)
{
    if (!nClientId)
    {
        return;
    }

    const int32_t nGUID = SERVER_GetClientGameGUID(nClientId);
    if (!gpGameDataTbl_6FD45818)
    {
        GAME_LogMessage(6, "[SERVER]  SrvDisconnectClientById: *** failed to disconnect client %d ***", nClientId);
        return;
    }

    // TODO: v14
    int32_t v14 = 0;
    D2GameManagerStrc_Lock_6FC3B510(gpGameDataTbl_6FD45818, 0, &v14, 0);

    D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
    if (!pGame)
    {
        if (v14)
        {
            LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
        }

        GAME_LogMessage(6, "[SERVER]  SrvDisconnectClientById: *** failed to disconnect client %d ***", nClientId);
        return;
    }

    D2_ASSERT(pGame->lpCriticalSection);
    EnterCriticalSection(pGame->lpCriticalSection);

    if (v14)
    {
        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    }

    if (!CLIENTS_IsInGame(pGame, nClientId))
    {
        D2_ASSERT(pGame->lpCriticalSection);
        LeaveCriticalSection(pGame->lpCriticalSection);

        GAME_LogMessage(6, "[SERVER]  SrvDisconnectClientById: *** failed to disconnect client %d ***", nClientId);
        return;
    }

    GAME_DisconnectClient(pGame, CLIENTS_GetClientFromClientId(pGame, nClientId), a2);
    
    D2_ASSERT(pGame->lpCriticalSection);
    LeaveCriticalSection(pGame->lpCriticalSection);
}

//D2Game.0x6FC37880
int32_t __stdcall GAME_DisconnectClientByName(const char* szClientName, char a3)
{
    const int32_t nClientId = sub_6FC33F20(szClientName);
    if (!nClientId)
    {
        return 0;
    }

    GAME_DisconnectClientById(nClientId, a3);
    return 1;
}

//D2Game.0x6FC379C0
void __stdcall D2Game_10024_RemoveClientFromGame(int32_t nClientId)
{
    if (!nClientId)
    {
        return;
    }

    const int32_t nGUID = SERVER_GetClientGameGUID(nClientId);
    if (!gpGameDataTbl_6FD45818)
    {
        return;
    }

    // TODO: v14
    int32_t v14 = 0;
    D2GameManagerStrc_Lock_6FC3B510(gpGameDataTbl_6FD45818, 0, &v14, 0);

    D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
    if (!pGame)
    {
        if (v14)
        {
            LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
        }
        return;
    }

    D2_ASSERT(pGame->lpCriticalSection);
    EnterCriticalSection(pGame->lpCriticalSection);

    if (v14)
    {
        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    }

    if (CLIENTS_IsInGame(pGame, nClientId))
    {
        D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientId);
        D2GSPacketSrv5A packet5A = {};
        packet5A.nHeader = 0x5Au;
        packet5A.nType = 0;
        packet5A.nColor = 4;
        packet5A.dwParam = 0;
        packet5A.szText[16] = 0;
        SStrCopy(packet5A.szText, CLIENTS_GetName(pClient), 16u);
        sub_6FC84D40(pGame, &packet5A);
        CLIENTS_RemoveClientFromGame(pGame, CLIENTS_GetClientId(pClient), 1);
    }

    D2_ASSERT(pGame->lpCriticalSection);
    LeaveCriticalSection(pGame->lpCriticalSection);
}

//D2Game.0x6FC37B10
void __fastcall sub_6FC37B10(D2GameStrc* pGame)
{
    const int32_t nGameType = pGame->nGameType;
    if (nGameType == 1 || nGameType == 2)
    {
        D2ClientStrc* pClient = pGame->pClientList;
        while (pClient)
        {
            D2ClientStrc* pNextClient = pClient->pNext;

            D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
            if (pPlayer)
            {
                D2GAME_SAVE_WriteFile_6FC8A500(pGame, pPlayer, CLIENTS_GetName(pClient), 0);

                while (CLIENT_GetSaveHeader_6FC34350(pClient))
                {
                    sub_6FC37B90(pGame, pClient);
                }
            }

            pClient = pNextClient;
        }
    }
}

//D2Game.0x6FC37B90
void __fastcall sub_6FC37B90(D2GameStrc* pGame, D2ClientStrc* pClient)
{
    if (!CLIENT_GetSaveHeader_6FC34350(pClient))
    {
        return;
    }

    if (pClient->unk0x184[0])
    {
        --pClient->unk0x184[0];
        return;
    }

    const uint8_t* pSaveData = (uint8_t*)pClient->pSaveHeader;
    pClient->unk0x184[0] = 5;
    if (!pSaveData)
    {
        return;
    }

    int32_t nDataSize = pClient->nSaveHeaderSize - pClient->unk0x184[1];
    const uint8_t* pData = &pSaveData[pClient->unk0x184[1]];
    if (nDataSize >= 255)
    {
        nDataSize = 255;
    }

    uint8_t* pPacket = (uint8_t*)D2_ALLOC_POOL(pClient->pGame->pMemoryPool, 0x107u);
    *pPacket = 0xB2u;
    pPacket[1] = nDataSize;
    if (pClient->unk0x184[1])
    {
        pPacket[2] = 0;
    }
    else
    {
        pPacket[2] = 1;
    }

    uint8_t* pPacketData = pPacket + 7;
    *(int32_t*)(pPacket + 3) = pClient->nSaveHeaderSize;

    for (int32_t i = 0; i < nDataSize; ++i)
    {
        *pPacketData++ = *pData++;
    }

    if (D2NET_10006(0, pClient->dwClientId, pPacket, nDataSize + 7))
    {
        pClient->unk0x184[1] += nDataSize;

        if (pClient->unk0x184[1] == pClient->nSaveHeaderSize)
        {
            CLIENTS_FreeSaveHeader(pClient);
        }

        pClient->unk0x1C4 = 0;
    }
    else
    {
        ++pClient->unk0x1C4;
    }

    D2_FREE_POOL(pClient->pGame->pMemoryPool, pPacket);

    if (pClient->unk0x1C4 >= 3)
    {
        CLIENTS_FreeSaveHeader(pClient);
        GAME_DisconnectClient(pGame, pClient, 0);
    }
}

//D2Game.0x6FC37CE0
void __fastcall GAME_EndGame(int32_t nClientId, int32_t a2)
{
    // TODO: v10
    if (!gpGameDataTbl_6FD45818)
    {
        GAME_LogMessage(6, "[SERVER]  SrvEndGame: Client %d was not in any game", nClientId);
        return;
    }
    
    int32_t bLeaveCriticalSection = 0;
    D2GameStrc* pGame = D2GAME_FindAndLockGameByGUID__6FC3B480(gpGameDataTbl_6FD45818, 0, SERVER_GetClientGameGUID(nClientId), &bLeaveCriticalSection, 0);
    if (!pGame)
    {
        GAME_LogMessage(6, "[SERVER]  SrvEndGame: Client %d was not in any game", nClientId);
        return;
    }

    D2_ASSERT(pGame->lpCriticalSection);
    EnterCriticalSection(pGame->lpCriticalSection);

    D2GameManagerStrc_Unlock_6FC3B540(gpGameDataTbl_6FD45818, 0, bLeaveCriticalSection);

    if (!CLIENTS_IsInGame(pGame, nClientId))
    {
        D2_ASSERT(pGame->lpCriticalSection);
        LeaveCriticalSection(pGame->lpCriticalSection);

        GAME_LogMessage(6, "[SERVER]  SrvEndGame: Client %d was not in any game", nClientId);
        return;
    }

    D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientId);
    const uint32_t v10 = CLIENT_IsSaveLoaded(pClient);
    if (v10 || pGame->nGameType)
    {
        D2ClientStrc* i = pGame->pClientList;
        while (i)
        {
            D2ClientStrc* pNext = i->pNext;

            D2UnitStrc* pUnit = CLIENTS_GetPlayerFromClient(i, 0);
            if (pUnit)
            {
                D2GAME_SAVE_WriteFile_6FC8A500(pGame, pUnit, CLIENTS_GetName(i), 0);
                if (gpD2ServerCallbackFunctions_6FD45830 && gpD2ServerCallbackFunctions_6FD45830->pfUpdateCharacterLadder)
                {
                    gpD2ServerCallbackFunctions_6FD45830->pfUpdateCharacterLadder(
                        CLIENTS_GetName(i),
                        pUnit->dwClassId,
                        STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0),
                        STATLIST_GetUnitBaseStat(pUnit, STAT_EXPERIENCE, 0),
                        0,
                        CLIENTS_GetFlags(i),
                        &i->unk0x184[3]
                    );
                }
            }
            i = pNext;
        }
    }

    if (pClient)
    {
        while (1)
        {
            D2GAME_PACKETS_SendPacket0x4F_StartMercList_6FC3C6F0(pClient, 5u);

            if ((pGame->nGameType == 1 || pGame->nGameType == 2) && CLIENT_GetSaveHeader_6FC34350(pClient))
            {
                do
                {
                    sub_6FC37B90(pGame, pClient);
                }
                while (CLIENT_GetSaveHeader_6FC34350(pClient));
            }

            D2GAME_PACKETS_SendPacket0x4F_StartMercList_6FC3C6F0(pClient, 6u);

            sub_6FC3C690(nClientId);
            sub_6FC39030(pGame, pClient, 0, 0);

            D2GSPacketSrv5A packet5A = {};
            packet5A.nHeader = 0x5Au;
            packet5A.nType = 3;
            packet5A.nColor = 4;
            packet5A.dwParam = 0;
            CLIENTS_CopyAccountNameToBuffer(pClient, &packet5A.szText[16]);
            packet5A.szText[31] = '\0';
            SStrCopy(packet5A.szText, CLIENTS_GetName(pClient), 16u);
            CLIENTS_RemoveClientFromGame(pGame, nClientId, !v10);

            if (a2)
            {
                sub_6FC84D40(pGame, &packet5A);
            }

            GAME_LogMessage(6, "[SERVER]  SrvEndGame: Disconnect %d '%s' from game %d '%s'", nClientId, packet5A.szText, pGame->nServerToken, pGame->szGameName);

            if (!v10)
            {
                break;
            }

            pClient = pGame->pClientList;
            if (!pClient)
            {
                break;
            }

            nClientId = CLIENTS_GetClientId(pClient);
        }
    }

    D2_ASSERT(pGame->lpCriticalSection);
    LeaveCriticalSection(pGame->lpCriticalSection);

    if (pGame->nGameType == 1 || pGame->nGameType == 2)
    {
        Sleep(100u);
    }

    D2NET_10016(nClientId);
}

//D2Game.0x6FC37FB0
void __fastcall sub_6FC37FB0(D2GameStrc* pGame, D2ClientStrc* pClient)
{
    D2ClientStrc* pCheckedClient = nullptr;
    GAME_SendPacketToAllConnectedClients(pGame, sub_6FC380D0, &pCheckedClient);

    if (pCheckedClient)
    {
        GAME_EndGame(CLIENTS_GetClientId(pCheckedClient), 0);
        return;
    }

    if (pClient)
    {
        GAME_EndGame(CLIENTS_GetClientId(pClient), 0);
        return;
    }

    pCheckedClient = nullptr;
    GAME_SendPacketToAllConnectedClients(pGame, sub_6FC380D0, &pCheckedClient);

    if (pCheckedClient)
    {
        GAME_EndGame(CLIENTS_GetClientId(pCheckedClient), 0);
    }
}

//D2Game.0x6FC380D0
void __fastcall sub_6FC380D0(D2ClientStrc* pClient, void* pArg)
{
    D2ClientStrc** ppClient = (D2ClientStrc**)pArg;
    if (CLIENT_IsSaveLoaded(pClient))
    {
        *ppClient = pClient;
    }
}

//D2Game.0x6FC380F0
void __fastcall sub_6FC380F0(D2ClientStrc* pClient, void* pArg)
{
    D2ClientStrc** ppClient = (D2ClientStrc**)pArg;
    if (!*ppClient)
    {
        *ppClient = pClient;
    }
}

//D2Game.0x6FC38100
int32_t __fastcall sub_6FC38100(int32_t nClientId)
{
    // TODO: size of v3
    char v3; // [sp+4h] [bp-10h]@1

    if (sub_6FC33EA0(nClientId, &v3))
    {
        return 1;
    }

    GAME_LogMessage(3, "[HACKLIST] <D2CLTSYS_DISCONNECT>  CLTID:%d  REASON:Not in game", nClientId);
    return 0;
}

//D2Game.0x6FC38140
//Note: You better disable those packets as they were used to DDos servers in the past...
void __fastcall sub_6FC38140(void *a1, int32_t a2)
{
    char a3[16];
    char szIPAddress[16];

    if (gpD2ServerCallbackFunctions_6FD45830)
    {
        const int nClientIdx = *(DWORD*)a1;
        switch (*((BYTE*)a1 + 5))
        {
        case PACKET_ADMIN_GETINFO:
        case PACKET_ADMIN_GETGAMEINFO:
            if (gpD2ServerCallbackFunctions_6FD45830->pfHandlePacket && gpD2ServerCallbackFunctions_6FD45830->pfServerLogMessage)
            {
                D2GSPacketSrvFF01 tPacket;
                D2GameServerInfoExStrc tGameServerInfoEx;
                gpD2ServerCallbackFunctions_6FD45830->pfHandlePacket(&tGameServerInfoEx, a2);
                tPacket.nPacketSubType = *((BYTE*)a1 + 5);
                tPacket.tServerInfo.field_0x4 = tGameServerInfoEx.field_0x4;
                tPacket.tServerInfo.field_0x8 = tGameServerInfoEx.field_0x8;
                tPacket.tServerInfo.field_0x0 = tGameServerInfoEx.field_0x0;
                tPacket.tServerInfo.field_0x10 = tGameServerInfoEx.field_0x10;
                tPacket.tServerInfo.field_0x14 = tGameServerInfoEx.field_0x14;
                tPacket.tServerInfo.field_0xc = tGameServerInfoEx.field_0xc;
                tPacket.tServerInfo.field_0x1c = tGameServerInfoEx.field_0x1c;
                tPacket.tServerInfo.field_0x20 = tGameServerInfoEx.field_0x20;
                tPacket.tServerInfo.field_0x18 = tGameServerInfoEx.field_0x18;
                tPacket.tServerInfo.field_0x24 = tGameServerInfoEx.field_0x24;
                SStrCopy(tPacket.tServerInfo.szCompilationData, tGameServerInfoEx.szCompilationData, sizeof(D2GameServerInfoStrc::szCompilationData));
                tPacket.tServerInfo.field_0xa8 = tGameServerInfoEx.field_0xa8;
                tPacket.field_0x10f = tGameServerInfoEx.field_0xb0;
                tPacket.field_0x10b = tGameServerInfoEx.field_0xac;
                tPacket.field_0x117 = tGameServerInfoEx.dwords0xBC[0];
                tPacket.field_0x113 = tGameServerInfoEx.field_0xb8 - gnNumAdminConnections_6FD45838;
                tPacket.tServerInfo.field_0xac = tGameServerInfoEx.field_0xac + tGameServerInfoEx.field_0xb0;
                tPacket.tServerInfo.field_0xb0 = tGameServerInfoEx.field_0xb4;
                tPacket.tServerInfo.field_0xb4 = tGameServerInfoEx.field_0xb8
                    - gnNumAdminConnections_6FD45838
                    + tGameServerInfoEx.dwords0xBC[0];
                tPacket.tServerInfo.field_0xb8 = tGameServerInfoEx.dwords0xBC[1];
                tPacket.field_0xbd = tGameServerInfoEx.dwords0xD8;
                memcpy(tPacket.aSomeInts, tGameServerInfoEx.dwords0xF0, sizeof(tPacket.aSomeInts));
                tPacket.field_0xe5 = tGameServerInfoEx.dwords0xBC[2];
                tPacket.field_0xe1 = tGameServerInfoEx.dwords0x110;
                tPacket.field_0xe9 = tGameServerInfoEx.dwords0xBC[3];
                tPacket.field_0xed = tGameServerInfoEx.dwords0xBC[4];
                tPacket.field_0xf1 = tGameServerInfoEx.dwords0xBC[5];
                tPacket.field_0xf5 = tGameServerInfoEx.word0xD4;
                memcpy(tPacket.field_0xf6, tGameServerInfoEx.szUnk0xD6, sizeof(tPacket.field_0xf6));
                memset(tPacket.nZeroed, 0, sizeof(tPacket.nZeroed));
                D2NET_10006(2, nClientIdx, &tPacket, tPacket.nPacketSubType == 1 ? sizeof(D2GSPacketSrvFF01) : sizeof(D2GSPacketSrvFFFD));
            }
            break;
        case PACKET_ADMIN_CONNECT:
            SERVER_GetIpAddressStringFromClientId(nClientIdx, szIPAddress, 16);
            InterlockedIncrement(&gnNumAdminConnections_6FD45838);
            a3[0] = PACKET_ADMIN_CONNECT;
            D2NET_10006(2, nClientIdx, a3, 16);
            break;
        case PACKET_ADMIN_DISCONNECT:
            SERVER_GetIpAddressStringFromClientId(nClientIdx, szIPAddress, 16);
            a3[0] = PACKET_ADMIN_DISCONNECT;
            D2NET_10006(2, nClientIdx, a3, 16);
            D2NET_10016(nClientIdx);
            InterlockedDecrement(&gnNumAdminConnections_6FD45838);
            break;
        case PACKET_ADMIN_GETIP:
            SERVER_GetIpAddressStringFromClientId(nClientIdx, szIPAddress, 16);
            a3[0] = PACKET_ADMIN_GETIP;
            D2NET_10006(2, nClientIdx, a3, 16);
            break;
        default:
            return;
        }
    }
}

//D2Game.0x6FC38530 (#10003)
void __fastcall GAME_ProcessNetworkMessages()
{
    uint8_t buffer[520] = {};

    while (1)
    {
        int32_t nSize = SERVER_ReadFromMessageList0(buffer, 512);
        if (nSize == -1)
        {
            break;
        }
        CCMD_ProcessClientSystemMessage(buffer, nSize);
    }

    while (1)
    {
        int32_t nSize = SERVER_ReadFromMessageList1(buffer, 512);
        if (nSize == -1)
        {
            break;
        }
        CCMD_ProcessClientMessage(buffer, nSize);
    }

    while (1)
    {
        int32_t nSize = SERVER_ReadFromMessageList2(buffer, 512);
        if (nSize == -1)
        {
            break;
        }
        sub_6FC38140(buffer, nSize);
    }
}

//D2Game.0x6FC385A0
void __fastcall sub_6FC385A0(D2GameStrc* pGame, D2RoomStrc* pRoom)
{
    sub_6FC679F0(pGame, pRoom);

    if (pRoom->dwFlags & 1)
    {
        if (!D2Common_10074(pRoom))
        {
            SUNITINACTIVE_RestoreInactiveUnits(pGame, pRoom);
            D2Common_10075(pRoom, 1);
        }
    }
    else
    {
        SUNIT_SpawnPresetUnitsInRoom(pGame, pRoom);
        SUNITINACTIVE_RestoreInactiveUnits(pGame, pRoom);
        OBJECTS_PopulationHandler(pGame, pRoom);
        D2GAME_PopulateRoom_6FC67190(pGame, pRoom);
        pRoom->dwFlags |= 1u;
        D2Common_10075(pRoom, 1);
    }
}

//D2Game.0x6FC38610
void __fastcall GAME_UpdateEnvironment(D2GameStrc* pGame)
{
    for (int32_t i = 0; i < 5; ++i)
    {
        if (pGame->pAct[i] && ENVIRONMENT_UpdateCycleIndex(pGame->pAct[i], i))
        {
            int32_t nTimeIndex = 0;
            int32_t nTicks = 0;
            int32_t nEclipse = 0;
            ENVIRONMENT_GetCycleIndex_Ticks_EclipseFromAct(pGame->pAct[i], &nTimeIndex, &nTicks, &nEclipse);

            D2GSPacketSrv53 packet53 = {};
            packet53.nHeader = 0x53u;
            packet53.unk0x01 = nTimeIndex;
            packet53.unk0x05 = nTicks;
            packet53.unk0x09 = nEclipse;

            for (D2ClientStrc* pClient = pGame->pClientList; pClient; pClient = pClient->pNext)
            {
                D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
                if (pPlayer)
                {
                    D2GAME_UpdatePlayerItems_6FC4BB90(pGame, pPlayer, 1);

                    if (pClient->dwClientState == 4 && pClient->nAct == i)
                    {
                        D2GAME_SendPacket0x53_6FC3DF50(pClient, &packet53);
                    }
                }
            }
        }
    }
}

//D2Game.0x6FC386D0
void __fastcall GAME_UpdateProgress(D2GameStrc* pGame)
{
    // TODO: v2, v6

    ++pGame->dwGameFrame;

    // Debug ?
    const uint32_t nDebugBreakTrigger = pGame[1].__0014;
    if (nDebugBreakTrigger == 1)
    {
        *((BYTE*)0) = 0;
    }
    else if (nDebugBreakTrigger == 2)
    {
        *((BYTE*)0xC0000001) = 0;
    }
    else if (nDebugBreakTrigger == 3)
    {
        D2_ASSERT(0);
    }
    else if (nDebugBreakTrigger == 4)
    {
        D2_ALLOC(0x70000000u);
    }

    GAME_UpdateEnvironment(pGame);

    const uint32_t nTickCount = GetTickCount();
    const uint32_t nTickDiff = nTickCount - pGame->unk0xAC[2];
    if (nTickDiff >= 1000)
    {
        const uint32_t nOldTick = pGame->unk0xAC[1];
        pGame->unk0xAC[1] = 0;
        pGame->unk0xAC[2] = nTickCount;
        pGame->unk0xAC[0] = 1000 * nOldTick / nTickDiff;
        dword_6FD2CA14 = 1000 * nOldTick / nTickDiff;

        const uint32_t v6 = FOG_GetMemoryUsage(pGame->pMemoryPool);
        if (nTickCount - dword_6FD45840 > 10000)
        {
            dword_6FD45840 = nTickCount;
            gnPeakMemoryUsageInLast10s_6FD45828 = v6;
        }
        else if (v6 > gnPeakMemoryUsageInLast10s_6FD45828)
        {
            gnPeakMemoryUsageInLast10s_6FD45828 = v6;
        }
    }

    ++pGame->unk0xAC[1];

    for (int32_t i = 0; i < 5; ++i)
    {
        if (pGame->pAct[i] && pGame->pAct[i]->unk0x20)
        {
            for (D2RoomStrc* pRoom = DUNGEON_GetRoomFromAct(pGame->pAct[i]); pRoom; pRoom = pRoom->pRoomNext)
            {
                sub_6FC679F0(pGame, pRoom);

                if (!(pRoom->dwFlags & 1))
                {
                    GAME_PopulateRoom(pGame, pRoom);
                    pRoom->dwFlags |= 1u;
                    D2Common_10075(pRoom, 1);
                }
                else if (!D2Common_10074(pRoom))
                {
                    SUNITINACTIVE_RestoreInactiveUnits(pGame, pRoom);
                    D2Common_10075(pRoom, 1);
                }
            }

            pGame->pAct[i]->unk0x20 = 0;
        }
    }

    EVENT_IterateEvents(pGame);
    D2GAME_UpdateAllClients_6FC389C0(pGame);
    LEVEL_UpdateQueuedUnitsInAllActs(pGame);
    LEVEL_FreeDrlgDeletes(pGame);

    if (!(pGame->dwGameFrame % 20))
    {
        QUESTS_QuestUpdater(pGame);
    }

    if (!(pGame->dwGameFrame % 12))
    {
        for (int32_t i = 0; i < 5; ++i)
        {
            if (pGame->pAct[i])
            {
                D2RoomStrc* pNextRoom = nullptr;
                for (D2RoomStrc* pRoom = DUNGEON_GetRoomFromAct(pGame->pAct[i]); pRoom; pRoom = pNextRoom)
                {
                    pNextRoom = pRoom->pRoomNext;
                    if (D2Common_10081_GetTileCountFromRoom(pRoom) > 10 && DUNGEON_TestRoomCanUnTile(pGame->pAct[i], pRoom))
                    {
                        D2UnitStrc* pNextUnit = nullptr;
                        for (D2UnitStrc* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pNextUnit)
                        {
                            pNextUnit = pUnit->pRoomNext;
                            SUNITINACTIVE_CompressUnitIfNeeded(pGame, pUnit);
                        }

                        DUNGEON_RemoveRoomFromAct(pGame->pAct[i], pRoom);
                    }
                }
            }
        }
    }

    if (!(pGame->dwGameFrame % 11))
    {
        for (int32_t i = 0; i < 5; ++i)
        {
            if (pGame->pAct[i])
            {
                DUNGEON_UpdateAndFreeInactiveRooms(pGame->pAct[i]);
            }
        }
    }

    if (!(pGame->dwGameFrame % 1500))
    {
        for (int32_t i = 0; i < 5; ++i)
        {
            ITEMS_DeleteInactiveItems(pGame, i);
            SUNITINACTIVE_DeleteExpiredItemNodes(pGame, i);
        }
    }
}

//D2Game.0x6FC38990
void __fastcall GAME_PopulateRoom(D2GameStrc* pGame, D2RoomStrc* pRoom)
{
    SUNIT_SpawnPresetUnitsInRoom(pGame, pRoom);
    SUNITINACTIVE_RestoreInactiveUnits(pGame, pRoom);
    OBJECTS_PopulationHandler(pGame, pRoom);
    D2GAME_PopulateRoom_6FC67190(pGame, pRoom);
}

//D2Game.0x6FC389C0
void __fastcall D2GAME_UpdateAllClients_6FC389C0(D2GameStrc* pGame)
{
    if (!ARENA_NeedsClientUpdate(pGame))
    {
        FOG_DisplayHalt("This should never happen! [sUpdateClients]", __FILE__, __LINE__);
        exit(-1);
    }

    int32_t bUpdateLadder = 0;
    int32_t bPlayerDisconnected = 0;

    if (!(pGame->dwGameFrame % 8192))
    {
        bUpdateLadder = 1;
    }

    if (dword_6FD2CA00)
    {
        const uint32_t nTickCount = GetTickCount();
        
        D2ClientStrc* pClient = pGame->pClientList;
        while (pClient)
        {
            D2ClientStrc* pNext = pClient->pNext;
            if (nTickCount > pClient->dwLastPacketTick && (CLIENTS_CheckFlag(pClient, CLIENTSAVEFLAG_HARDCORE) && nTickCount - pClient->dwLastPacketTick > 10000 && pClient->dwPingsCount > 10 || nTickCount - pClient->dwLastPacketTick > 45000) && gpD2ServerCallbackFunctions_6FD45830)
            {
                D2GAME_SAVE_WriteFile_6FC8A500(pGame, CLIENTS_GetPlayerFromClient(pClient, 0), CLIENTS_GetName(pClient), 0);
                bPlayerDisconnected = 1;
                GAME_LogMessage(6, "[DISCONNECT]  PLAYER:%s  REASON:Heartbeat Timeout", CLIENTS_GetName(pClient));
                GAME_DisconnectClient(pGame, pClient, 0);
            }
            pClient = pNext;
        }

        if (bPlayerDisconnected)
        {
            D2ClientStrc* pClient = pGame->pClientList;
            while (pClient)
            {
                D2ClientStrc* pNext = pClient->pNext;
                
                D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
                if (pPlayer)
                {
                    D2GAME_SAVE_WriteFile_6FC8A500(pGame, pPlayer, CLIENTS_GetName(pClient), 0);

                    if (gpD2ServerCallbackFunctions_6FD45830 && gpD2ServerCallbackFunctions_6FD45830->pfUpdateCharacterLadder)
                    {
                        gpD2ServerCallbackFunctions_6FD45830->pfUpdateCharacterLadder(
                            CLIENTS_GetName(pClient),
                            pPlayer->dwClassId,
                            STATLIST_GetUnitBaseStat(pPlayer, STAT_LEVEL, 0),
                            STATLIST_GetUnitBaseStat(pPlayer, STAT_EXPERIENCE, 0),
                            0,
                            CLIENTS_GetFlags(pClient),
                            &pClient->unk0x184[3]
                        );
                    }
                }
                pClient = pNext;
            }
        }
    }

    if (bUpdateLadder && !bPlayerDisconnected)
    {
        D2ClientStrc* pClient = pGame->pClientList;
        while (pClient)
        {
            D2ClientStrc* pNext = pClient->pNext;
            
            D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
            if (pPlayer)
            {
                D2GAME_SAVE_WriteFile_6FC8A500(pGame, pPlayer, CLIENTS_GetName(pClient), 0);
                if (gpD2ServerCallbackFunctions_6FD45830 && gpD2ServerCallbackFunctions_6FD45830->pfUpdateCharacterLadder)
                {
                    gpD2ServerCallbackFunctions_6FD45830->pfUpdateCharacterLadder(
                        CLIENTS_GetName(pClient),
                        pPlayer->dwClassId, STATLIST_GetUnitBaseStat(pPlayer, STAT_LEVEL, 0),
                        STATLIST_GetUnitBaseStat(pPlayer, STAT_EXPERIENCE, 0),
                        0,
                        CLIENTS_GetFlags(pClient),
                        &pClient->unk0x184[3]
                    );
                }
            }
            pClient = pNext;
        }
    }

    D2ClientStrc* pClient = pGame->pClientList;
    while (pClient)
    {
        D2ClientStrc* pNext = pClient->pNext;

        switch (pClient->dwClientState)
        {
        case 3u:
        {
            sub_6FC33670(pGame, pClient);

            if (D2Common_10073(pClient->pRoom) && CLIENTS_GetPlayerFromClient(pClient, 0))
            {
                D2GAME_PACKETS_SendPacket0x4F_StartMercList_6FC3C6F0(pClient, 4u);
                CLIENTS_SetClientState(pClient, 4u);

                D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
                if (pPlayer)
                {
                    D2GAME_ITEMS_UpdateInventoryItems_6FC45050(pGame, pPlayer, 0, 1);
                }

                sub_6FC369C0(pGame, pClient);
                sub_6FCBAB20(pGame, CLIENTS_GetPlayerFromClient(pClient, 1));

                if (gpD2ServerCallbackFunctions_6FD45830)
                {
                    pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
                    if (pPlayer && gpD2ServerCallbackFunctions_6FD45830->pfEnterGame)
                    {
                        gpD2ServerCallbackFunctions_6FD45830->pfEnterGame(pGame->nServerToken, pClient->szName, pPlayer->dwClassId, STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0), pClient->nSaveFlags);
                    }
                }

                
                D2GSPacketSrv5A packet5A = {};
                packet5A.nHeader = 0x5A;
                packet5A.nType = 2;
                packet5A.nColor = 4;
                packet5A.dwParam = 0;
                packet5A.szText[16] = '\0';
                
                char szAccountName[256] = {};
                CLIENTS_CopyAccountNameToBuffer(pClient, szAccountName);
                if (strlen(szAccountName) != 0)
                {
                    SStrCopy(&packet5A.szText[16], szAccountName, 16u);
                    packet5A.szText[31] = '\0';
                }

                SStrCopy(packet5A.szText, pClient->szName, 16u);
                sub_6FC84D40(pGame, &packet5A);
            }
            break;
        }
        case 4u:
        {
            sub_6FC33670(pGame, pClient);
            break;
        }

        case 5u:
        {
            sub_6FC33670(pGame, pClient);

            if (D2Common_10073(pClient->pRoom))
            {
                D2GAME_PACKETS_SendPacket0x4F_StartMercList_6FC3C6F0(pClient, 4u);
                CLIENTS_SetClientState(pClient, 4u);

                D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
                if (pPlayer)
                {
                    D2GAME_ITEMS_UpdateInventoryItems_6FC45050(pGame, pPlayer, 0, 1);
                }
            }
            break;
        }
        }

        pClient = pNext;
    }
}

//D2Game.0x6FC38E00
void __fastcall sub_6FC38E00()
{
    dword_6FD457F8 = 1000 / dword_6FD2CA60;
}

//D2Game.0x6FC38E20 (#10004)
int32_t __stdcall GAME_UpdateGamesProgress(int32_t a1)
{
    // TODO: Names
    LARGE_INTEGER start = {};
    QueryPerformanceCounter(&start);

    const uint32_t time = timeGetTime() & 0x7FFFFFFF;
    if (!dword_6FD45844)
    {
        dword_6FD45844 = time;
    }

    if (time - dword_6FD45844 < dword_6FD457F8)
    {
        return 0;
    }

    int32_t v5 = time - dword_6FD45844 - dword_6FD457F8;
    if (a1 && v5 >= dword_6FD457F8)
    {
        v5 = dword_6FD457F8;
    }

    dword_6FD45844 = time - v5;
    
    int32_t bQueryPerformance = 0;
    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        const int32_t nGUID = gnGamesGUIDs_6FD447F8[i];
        if (nGUID && nGUID != -1 && gpGameDataTbl_6FD45818)
        {
            EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);

            D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
            if (!pGame)
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            }
            else
            {
                D2_ASSERT(pGame->lpCriticalSection);
                EnterCriticalSection(pGame->lpCriticalSection);

                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

                bQueryPerformance = 1;

                if (dword_6FD2CA10)
                {
                    pGame->unk0x1DB8[1] = 1024;
                }
                else
                {
                    if (pGame->unk0x1DB8[0])
                    {
                        pGame->unk0x1DB8[1] = D2Clamp(((time - pGame->unk0x1DB8[0]) << 10) / dword_6FD457F8, 10u, 2048u);
                    }

                    pGame->unk0x1DB8[0] = time;
                }

                GAME_UpdateProgress(pGame);

                D2_ASSERT(pGame->lpCriticalSection);
                LeaveCriticalSection(pGame->lpCriticalSection);
            }
        }
    }

    if (!bQueryPerformance)
    {
        return 0;
    }

    LARGE_INTEGER end = {};
    QueryPerformanceCounter(&end);
    start.HighPart = (uint64_t)(end.QuadPart - start.QuadPart) >> 32;
    return end.QuadPart - start.QuadPart;
}

//D2Game.0x6FC39030
void __fastcall sub_6FC39030(D2GameStrc* pGame, D2ClientStrc* pClient, int32_t a3, int32_t nUnused)
{
    D2_MAYBE_UNUSED(nUnused);
    D2_ASSERT(pGame);
    D2_ASSERT(pClient);

    if (a3 && pClient->dwClientState == 4)
    {
        if (dword_6FD4582C)
        {
            D2PacketDataStrc* pPacketData = CLIENTS_PacketDataList_GetHead(pClient);
            if (pPacketData)
            {
                D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
                if (pPlayer)
                {
                    if (pClient->unk0x1B0 >= 10)
                    {
                        D2_ASSERT(pPlayer->dwUnitType == UNIT_PLAYER);

                        const int32_t nUpdateSize = (499 - pPacketData->nPacketSize) / 9;
                        if (nUpdateSize > 0)
                        {
                            D2_ASSERT(nUpdateSize < CLIENTS_MAX_UPDATES);
                            CLIENTS_RefreshUnitsUpdateList(pClient, nUpdateSize);
                            D2GAME_PACKETS_SendPacket0x16_UnitsUpdate(pClient);
                        }
                    }
                }
            }
        }

        D2PacketDataStrc* pPacketData = CLIENTS_PacketDataList_GetHead(pClient);
        
        int32_t bSkipHpCheck = 0;
        D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
        if (pPlayer)
        {
            if (pPacketData && pClient->unk0x1B0 >= 10)
            {
                bSkipHpCheck = 1;
            }

            if (pClient->unk0x1B0 >= 20)
            {
                bSkipHpCheck = 1;
            }

            D2_ASSERT(pPlayer->dwUnitType == UNIT_PLAYER);

            if (sub_6FC82360(pPlayer, pPlayer, bSkipHpCheck))
            {
                pClient->unk0x1B0 = 0;
            }
        }
    }

    if (pGame->nGameType == 1 || pGame->nGameType == 2)
    {
        sub_6FC37B90(pGame, pClient);
    }

    int32_t nCounter = 0;
    int32_t bError = 0;
    do
    {
        D2PacketDataStrc* pPacketData = CLIENTS_PacketDataList_PopHead(pClient);
        if (!pPacketData || !pPacketData->nPacketSize)
        {
            break;
        }

        if (D2NET_10006(1, pClient->dwClientId, pPacketData->packetData, pPacketData->nPacketSize))
        {
            pPacketData->nPacketSize = 0;
            pPacketData->pNext = pClient->tPacketDataList.pPacketDataPool;
            pClient->tPacketDataList.pPacketDataPool = pPacketData;
            pClient->unk0x1C4 = 0;
            ++nCounter;
        }
        else
        {
            SERVER_WSAGetLastError();
            bError = 1;
            ++pClient->unk0x1C4;
            CLIENTS_PacketDataList_Reset(pClient, pPacketData);
            if (pClient->unk0x1C4 >= 3)
            {
                GAME_DisconnectClient(pGame, pClient, 0);
                return;
            }
        }
    }
    while ((pGame->nGameType != 1 && pGame->nGameType != 2 || nCounter < 3) && !bError);
}

//D2Game.0x6FC39270
void __fastcall sub_6FC39270(D2GameStrc* pGame, int32_t a2)
{
    D2ClientStrc* pClient = pGame->pClientList;
    while (pClient)
    {
        D2ClientStrc* pNextClient = pClient->pNext;
        sub_6FC39030(pGame, pClient, 1, a2);
        pClient = pNextClient;
    }
}

//D2Game.0x6FC392A0 (#10005)
void __stdcall GAME_UpdateClients(int32_t a1, int32_t a2)
{
    LARGE_INTEGER start = {};
    QueryPerformanceCounter(&start);
    const uint32_t nTickCount = GetTickCount();
    
    if (!a1 && !a2)
    {
        if (nTickCount - dword_6FD45848 < 40)
        {
            return;
        }

        dword_6FD45848 = nTickCount;
    }

    int32_t bQueryPerformance = 0;
    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        const int32_t nGUID = gnGamesGUIDs_6FD447F8[i];
        if (nGUID && nGUID != -1 && gpGameDataTbl_6FD45818)
        {
            EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);

            D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
            if (!pGame)
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            }
            else
            {
                D2_ASSERT(pGame->lpCriticalSection);
                EnterCriticalSection(pGame->lpCriticalSection);

                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

                D2ClientStrc* pClient = pGame->pClientList;

                bQueryPerformance = 1;
                while (pClient)
                {
                    D2ClientStrc* pNext = pClient->pNext;
                    sub_6FC39030(pGame, pClient, 1, a1);
                    pClient = pNext;
                }

                if (pGame->nClients)
                {
                    pGame->unk0x1DB8[2] = 0;
                    D2_ASSERT(pGame->lpCriticalSection);
                    LeaveCriticalSection(pGame->lpCriticalSection);
                }
                else
                {
                    if (!pGame->unk0x1DB8[2])
                    {
                        pGame->unk0x1DB8[2] = nTickCount;
                    }

                    if (nTickCount - pGame->unk0x1DB8[2] > 300000 || pGame->dwGameFrame > 1500 && pGame->dwGameFrame < 7500)
                    {
                        GAME_LogMessage(6, "[SERVER]  Deleting game from SrvSendAllMsgs()");

                        D2_ASSERT(pGame->lpCriticalSection);
                        LeaveCriticalSection(pGame->lpCriticalSection);
                        GAME_CloseGame(nGUID);
                    }
                    else
                    {
                        D2_ASSERT(pGame->lpCriticalSection);
                        LeaveCriticalSection(pGame->lpCriticalSection);
                    }
                }
            }
        }
    }

    if (bQueryPerformance)
    {
        LARGE_INTEGER end = {};
        QueryPerformanceCounter(&end);
        start.HighPart = (uint64_t)(end.QuadPart - start.QuadPart) >> 32;
    }
}

//D2Game.0x6FC394E0
D2GameStrc* __fastcall GAME_GetGameByClientId(int32_t nClientId)
{
    const int32_t nGameGUID = SERVER_GetClientGameGUID(nClientId);
    if (!gpGameDataTbl_6FD45818)
    {
        return nullptr;
    }

    EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);

    D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGameGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
    if (!pGame)
    {
        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
        return 0;
    }

    D2_ASSERT(pGame->lpCriticalSection);
    EnterCriticalSection(pGame->lpCriticalSection);

    LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

    if (CLIENTS_IsInGame(pGame, nClientId))
    {
        return pGame;
    }

    D2_ASSERT(pGame->lpCriticalSection);
    LeaveCriticalSection(pGame->lpCriticalSection);

    return nullptr;
}

//D2Game.0x6FC395B0
void __fastcall GAME_LeaveGamesCriticalSection(D2GameStrc* pGame)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pGame->lpCriticalSection);

    LeaveCriticalSection(pGame->lpCriticalSection);
}

//D2Game.0x6FC39600) --------------------------------------------------------
void __fastcall GAME_CloseGame(int32_t a1)
{
    UNIMPLEMENTED();
//    D2GameManagerStrc* v1; // edi@1
//    int32_t v2; // ebx@1
//    struct _RTL_CRITICAL_SECTION* v3; // ebp@2
//    int32_t v4; // eax@2
//    D2GameStrc* v5; // esi@2
//    LPCRITICAL_SECTION v6; // ecx@13
//    D2GameManagerStrc* v7; // ebp@16
//    char* v8; // edi@17
//    char* v9; // edi@19
//
//    v1 = gpGameDataTbl_6FD45818;
//    v2 = a1;
//    if (gpGameDataTbl_6FD45818)
//    {
//        v3 = &gpGameDataTbl_6FD45818->pLock;
//        EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
//        v4 = *(_DWORD*)(v1->field_0.field_1C + 12 * (v2 & v1->field_0.field_24) + 8);
//        v5 = (D2GameStrc*)(v4 & ((v4 <= 0) - 1));
//        if ((int32_t)v5 > 0)
//        {
//            while (v5->__0000[0] != v2)
//            {
//                v5 = *(D2GameStrc**)(sub_6FC3B6F0((void*)(v1->field_0.field_1C + 12 * (v2 & v1->field_0.field_24)), (int32_t)v5)
//                                     + 4);
//                if ((int32_t)v5 <= 0)
//                {
//                    LeaveCriticalSection(&v1->pLock);
//                    return;
//                }
//            }
//            if (v5)
//            {
//                if (gpD2ServerCallbackFunctions_6FD45830 && gpD2ServerCallbackFunctions_6FD45830->pfCloseGame)
//                {
//                    if (IsBadCodePtr((FARPROC)gpD2ServerCallbackFunctions_6FD45830->pfCloseGame))
//                    {
//                        FOG_DisplayAssert("ptEventCallbackTable->fpCloseGame", __FILE__, __LINE__);
//                        exit(-1);
//                    }
//                    gpD2ServerCallbackFunctions_6FD45830->pfCloseGame(v5->nServerToken, v5->bExpansion != 0 ? 0x44325850u : 0x44324456u, v5->dwSpawnedPlayers, (signed __int32_t32)v5->dwGameFrame / 25);
//                }
//                v6 = v5->lpCriticalSection;
//                if (!v6)
//                {
//                    FOG_DisplayAssert("ptGame->_ptLock != nullptr", __FILE__, __LINE__);
//                    exit(-1);
//                }
//                EnterCriticalSection(v6);
//                GAME_FreeGame(v2, v5);
//                v7 = gpGameDataTbl_6FD45818;
//                if (v5->__0000[2])
//                {
//                    v8 = (char*)&v5->__0000[1];
//                    if (v5->__0000[1])
//                    {
//                        *(_DWORD*)sub_6FC3B560(&v5->__0000[1], -1) = *(_DWORD*)v8;
//                        *(_DWORD*)(*(_DWORD*)v8 + 4) = v5->__0000[2];
//                        *(_DWORD*)v8 = 0;
//                        v5->__0000[2] = 0;
//                    }
//                    v9 = (char*)&v5->__0000[3];
//                    if (v5->__0000[3])
//                    {
//                        *(_DWORD*)sub_6FC3B560(&v5->__0000[3], -1) = *(_DWORD*)v9;
//                        *(_DWORD*)(*(_DWORD*)v9 + 4) = v5->pNext;
//                        *(_DWORD*)v9 = 0;
//                        v5->pNext = 0;
//                    }
//                }
//                (*(void(__thiscall**)(D2GameManagerStrc*, D2GameStrc*))v7->field_0.field_0)(v7, v5);
//                v3 = &v7->pLock;
//            }
//        }
//        LeaveCriticalSection(v3);
//    }
}

//D2Game.0x6FC397A0) --------------------------------------------------------
D2GameStrc* __fastcall TASK_GetGame_6FC397A0(int32_t nGameHashKey)
{
    UNIMPLEMENTED();
    return nullptr;

//    D2GameManagerStrc* v1; // esi@1
//    int32_t v2; // edi@1
//    D2GameStrc* result; // eax@2
//    struct _RTL_CRITICAL_SECTION* v4; // ebx@3
//    int32_t v5; // eax@3
//    D2GameStrc* v6; // eax@3
//    D2GameStrc* v7; // esi@7
//    LPCRITICAL_SECTION v8; // eax@9
//
//    v1 = gpGameDataTbl_6FD45818;
//    v2 = a1;
//    if (gpGameDataTbl_6FD45818)
//    {
//        v4 = &gpGameDataTbl_6FD45818->pLock;
//        EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
//        v5 = *(_DWORD*)(v1->field_0.field_1C + 12 * (v2 & v1->field_0.field_24) + 8);
//        v6 = (D2GameStrc*)(((v5 <= 0) - 1) & v5);
//        if ((int32_t)v6 <= 0)
//        {
//LABEL_6:
//            LeaveCriticalSection(&v1->pLock);
//            result = 0;
//        }
//        else
//        {
//            while (v6->__0000[0] != v2)
//            {
//                v6 = *(D2GameStrc**)(sub_6FC3B6F0((void*)(v1->field_0.field_1C + 12 * (v2 & v1->field_0.field_24)), (int32_t)v6)
//                                     + 4);
//                if ((int32_t)v6 <= 0)
//                    goto LABEL_6;
//            }
//            v7 = v6;
//            if (v6)
//            {
//                v8 = v6->lpCriticalSection;
//                if (!v8)
//                {
//                    FOG_DisplayAssert("ptGame->_ptLock != nullptr", __FILE__, __LINE__);
//                    exit(-1);
//                }
//                EnterCriticalSection(v8);
//                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
//                result = v7;
//            }
//            else
//            {
//                LeaveCriticalSection(v4);
//                result = v7;
//            }
//        }
//    }
//    else
//    {
//        result = 0;
//    }
//    return result;
}

//D2Game.0x6FC39870
void __fastcall sub_6FC39870(int32_t nClientId)
{
    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1 && gpGameDataTbl_6FD45818)
        {
            EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            
            D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, gnGamesGUIDs_6FD447F8[i], &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
            if (pGame)
            {
                D2_ASSERT(pGame->lpCriticalSection);
                EnterCriticalSection(pGame->lpCriticalSection);

                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

                const uint32_t nClientCount = pGame->nClients;
                const uint16_t nServerToken = pGame->nServerToken;
                char szGameName[16] = {};
                SStrCopy(szGameName, pGame->szGameName, 0x7FFFFFFFu);
                
                D2_ASSERT(pGame->lpCriticalSection);
                LeaveCriticalSection(pGame->lpCriticalSection);

                sub_6FC3C640(nClientId, nServerToken, nClientCount, szGameName);
            }
            else
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            }
        }
    }

    sub_6FC3C640(nClientId, -1, 0, byte_6FD447EC);
}

//D2Game.0x6FC399A0
int32_t __fastcall sub_6FC399A0(int32_t nClientId, D2GSPacketClt6B* pPacket)
{
    return CLIENTS_AttachSaveFile(nClientId, pPacket->saveData, pPacket->nSize, pPacket->nTotalSize, 0, 0, 0);
}

//D2Game.0x6FC399C0 (#10006)
void __stdcall D2Game_10006()
{
    TASK_FreeAllQueueSlots();

    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1)
        {
            D2_ASSERT(gpGameDataTbl_6FD45818);
            EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);

            D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, gnGamesGUIDs_6FD447F8[i], &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
            D2_ASSERT(pGame);

            D2_ASSERT(pGame->lpCriticalSection);
            EnterCriticalSection(pGame->lpCriticalSection);

            LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

            D2ClientStrc* pClient = pGame->pClientList;
            while (pClient)
            {
                D2ClientStrc* pNext = pClient->pNext;

                sub_6FC3C690(pClient->dwClientId);
                D2GAME_SAVE_WriteFile_6FC8A500(pGame, CLIENTS_GetPlayerFromClient(pClient, 0), CLIENTS_GetName(pClient), 1u);

                if ((pGame->nGameType == 1 || pGame->nGameType == 2) && CLIENT_GetSaveHeader_6FC34350(pClient))
                {
                    do
                    {
                        sub_6FC37B90(pGame, pClient);
                    }
                    while (CLIENT_GetSaveHeader_6FC34350(pClient));
                }

                sub_6FC3C6B0(pClient->dwClientId);
                CLIENTS_RemoveClientFromGame(pGame, pClient->dwClientId, 0);

                pClient = pNext;
            }

            D2_ASSERT(pGame->lpCriticalSection);
            LeaveCriticalSection(pGame->lpCriticalSection);

            GAME_CloseGame(gnGamesGUIDs_6FD447F8[i]);
        }
    }
}

//D2Game.0x6FC39B50 (#10012)
int32_t __fastcall GAME_GetGamesCount()
{
    int32_t nResult = 0;

    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] != 0 && gnGamesGUIDs_6FD447F8[i] != -1)
        {
            ++nResult;
        }
    }

    return nResult;
}

//D2Game.0x6FC39B70 (#10057)
int32_t __stdcall D2Game_10057()
{
    int32_t nExpansionGames = 0;
    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1 && gpGameDataTbl_6FD45818)
        {
            EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            
            D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, gnGamesGUIDs_6FD447F8[i], &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
            if (pGame)
            {
                D2_ASSERT(pGame->lpCriticalSection);
                EnterCriticalSection(pGame->lpCriticalSection);

                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

                if (pGame->bExpansion)
                {
                    ++nExpansionGames;
                }

                D2_ASSERT(pGame->lpCriticalSection);
                LeaveCriticalSection(pGame->lpCriticalSection);
            }
            else
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            }
        }
    }

    return nExpansionGames;
}

//D2Game.0x6FC39C70 (#10053)
void __stdcall D2Game_10053(int16_t* pCount, int32_t nArraySize)
{
    memset(pCount, 0, nArraySize);

    for (int32_t i = 0; i < 1024; ++i)
    {
        const int32_t v7 = gnGamesGUIDs_6FD447F8[i];
        if (v7 && v7 != -1 && gpGameDataTbl_6FD45818)
        {
            EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, v7, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
            if (pGame)
            {
                D2_ASSERT(pGame->lpCriticalSection);
                EnterCriticalSection(pGame->lpCriticalSection);
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
                int32_t nIndex = pGame->nClients;
                
                D2_ASSERT(pGame->lpCriticalSection);
                LeaveCriticalSection(pGame->lpCriticalSection);

                if (nIndex > nArraySize)
                {
                    nIndex = nArraySize;
                }

                ++pCount[nIndex];
            }
            else
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            }
        }
    }
}

//D2Game.0x6FC39D80 (#10054)
void __stdcall D2Game_10054(uint16_t* a1, int32_t nMaxCount)
{
    memset(a1, 0, sizeof(*a1) * nMaxCount);

    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1 && gpGameDataTbl_6FD45818)
        {
            EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            
            D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, gnGamesGUIDs_6FD447F8[i], &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
            if (pGame)
            {
                D2_ASSERT(pGame->lpCriticalSection);
                EnterCriticalSection(pGame->lpCriticalSection);

                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

                const int32_t nSecondsPassed = pGame->dwGameFrame / 25;

                D2_ASSERT(pGame->lpCriticalSection);
                LeaveCriticalSection(pGame->lpCriticalSection);

                int32_t nIndex = nSecondsPassed / 300;
                if (nIndex >= nMaxCount)
                {
                    nIndex = nMaxCount - 1;
                }

                ++a1[nIndex];
            }
            else
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            }
        }
    }
}

//D2Game.0x6FC39EC0 (#10051)
int32_t __fastcall D2Game_10051()
{
    return dword_6FD2CA14;
}

//D2Game.0x6FC39ED0 (#10052)
void __stdcall GAME_GetMemoryUsage(int* pCurrentMemoryUsage, int* pPeakMemoryUsageInLast10s)
{
    *pCurrentMemoryUsage = FOG_GetMemoryUsage(nullptr);
    *pPeakMemoryUsageInLast10s = gnPeakMemoryUsageInLast10s_6FD45828;
}

//D2Game.0x6FC39EF0 (#10013)
int32_t __stdcall D2Game_10013(uint16_t nGameId)
{
    int32_t nGUID = 0;
    EnterCriticalSection(&gCriticalSection_6FD45800);
    if (gpGameArray_6FD447F4[nGameId] && gpGameArray_6FD447F4[nGameId] != -1)
    {
        nGUID = gpGameArray_6FD447F4[nGameId];
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    D2_ASSERT(gpGameDataTbl_6FD45818);

    EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);

    D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
    if (!pGame)
    {
        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
        FOG_DisplayAssert("ptGame", __FILE__, __LINE__);
        exit(-1);
    }

    D2_ASSERT(pGame->lpCriticalSection);
    EnterCriticalSection(pGame->lpCriticalSection);
    LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

    const int32_t nClients = pGame->nClients;
    
    D2_ASSERT(pGame->lpCriticalSection);
    LeaveCriticalSection(pGame->lpCriticalSection);

    return nClients;
}

//D2Game.0x6FC39FF0
int32_t __stdcall D2Game_10014(uint16_t nGameId, D2GameInfoStrc* pGameInfo)
{
    int32_t nGUID = 0;
    EnterCriticalSection(&gCriticalSection_6FD45800);

    if (gpGameArray_6FD447F4[nGameId] && gpGameArray_6FD447F4[nGameId] != -1)
    {
        nGUID = gpGameArray_6FD447F4[nGameId];
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    if (!gpGameDataTbl_6FD45818)
    {
        return 0;
    }

    EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
    if (!pGame)
    {
        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
        return 0;
    }

    D2_ASSERT(pGame->lpCriticalSection);
    EnterCriticalSection(pGame->lpCriticalSection);
    LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

    pGameInfo->nServerToken = pGame->nServerToken;
    pGameInfo->nInitSeed = pGame->dwInitSeed;
    pGameInfo->nClients = pGame->nClients;
    pGameInfo->nSpawnedPlayers = pGame->dwLastUsedUnitGUID[0];
    pGameInfo->nSpawnedMonsters = pGame->dwLastUsedUnitGUID[1];
    pGameInfo->nSpawnedObjects = pGame->dwLastUsedUnitGUID[2];
    pGameInfo->nSpawnedMissiles = pGame->dwLastUsedUnitGUID[3];
    pGameInfo->nSpawnedItems = pGame->dwLastUsedUnitGUID[4];
    pGameInfo->nSpawnedTiles = pGame->dwLastUsedUnitGUID[5];

    pGameInfo->nPlayers = 0;
    for (int32_t i = 0; i < 128; ++i)
    {
        for (D2UnitStrc* pUnit = pGame->pUnitList[0][i]; pUnit; pUnit = SUNIT_GetNextUnitFromList(pUnit))
        {
            ++pGameInfo->nPlayers;
        }
    }

    pGameInfo->nMonsters = 0;
    for (int32_t i = 0; i < 128; ++i)
    {
        for (D2UnitStrc* pUnit = pGame->pUnitList[1][i]; pUnit; pUnit = SUNIT_GetNextUnitFromList(pUnit))
        {
            ++pGameInfo->nMonsters;
        }
    }

    pGameInfo->nObjects = 0;
    for (int32_t i = 0; i < 128; ++i)
    {
        for (D2UnitStrc* pUnit = pGame->pUnitList[2][i]; pUnit; pUnit = SUNIT_GetNextUnitFromList(pUnit))
        {
            ++pGameInfo->nObjects;
        }
    }

    pGameInfo->nItems = 0;
    for (int32_t i = 0; i < 128; ++i)
    {
        for (D2UnitStrc* pUnit = pGame->pUnitList[3][i]; pUnit; pUnit = SUNIT_GetNextUnitFromList(pUnit))
        {
            ++pGameInfo->nItems;
        }
    }

    pGameInfo->nMissiles = 0;
    for (int32_t i = 0; i < 128; ++i)
    {
        for (D2UnitStrc* pUnit = pGame->pUnitList[4][i]; pUnit; pUnit = SUNIT_GetNextUnitFromList(pUnit))
        {
            ++pGameInfo->nMissiles;
        }
    }

    if (pGame->nMonModeData)
    {
        pGameInfo->unk0x40 = 100 * pGame->dwMonModeData[2] / pGame->nMonModeData;
        pGameInfo->unk0x44 = 100 * pGame->dwMonModeData[5] / pGame->nMonModeData;
        pGameInfo->unk0x48 = 100 * pGame->dwMonModeData[6] / pGame->nMonModeData;
        pGameInfo->unk0x4C = 100 * pGame->dwMonModeData[1] / pGame->nMonModeData;
        pGameInfo->unk0x50 = 100 * pGame->dwMonModeData[15] / pGame->nMonModeData;
        pGameInfo->unk0x54 = pGame->nMonModeData;
        if (pGame->nMonModeData > 200 && pGameInfo->unk0x54)
        {
            memset(pGame->dwMonModeData, 0, 0x48u);
        }
    }

    SStrCopy(pGameInfo->szGameName, pGame->szGameName, 16u);

    pGameInfo->unk0x5C = (GetTickCount() - pGame->unk0x1DB8[1]) / 1000 / 60;
    pGameInfo->unk0x58 = pGame->dwGameFrame / 100;
    pGameInfo->unk0x60 = pGame->unk0xAC[0];
    pGameInfo->nArenaTemplate = pGame->nArenaTemplate;
    pGameInfo->unk0xA5 = pGame->unk0x6C;
    pGameInfo->unk0xA6 = pGame->unk0x6E;

    SStrCopy(pGameInfo->szGamePassword, pGame->szGamePassword, 16u);
    SStrCopy(pGameInfo->szGameDescription, pGame->szGameDesc, 32u);

    pGameInfo->nArenaFlags = ARENA_GetFlags(pGame);
    pGameInfo->pMemoryPool = pGame->pMemoryPool;

    D2_ASSERT(pGame->lpCriticalSection);
    LeaveCriticalSection(pGame->lpCriticalSection);

    return 1;
}

//D2Game.0x6FC3A390 (#10015)
int32_t __stdcall GAME_GetGameServerTokens(uint16_t* pServerToken, int32_t nMaxCount)
{
    int32_t nServerTokenCount = 0;
    for (int32_t i = 0; nServerTokenCount < nMaxCount && i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1 && gpGameDataTbl_6FD45818)
        {
            EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            
            D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, gnGamesGUIDs_6FD447F8[i], &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
            if (pGame)
            {
                D2_ASSERT(pGame->lpCriticalSection);
                EnterCriticalSection(pGame->lpCriticalSection);

                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

                pServerToken[nServerTokenCount] = pGame->nServerToken;
                ++nServerTokenCount;

                D2_ASSERT(pGame->lpCriticalSection);
                LeaveCriticalSection(pGame->lpCriticalSection);
            }
            else
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            }
        }
    }

    return nServerTokenCount;
}

//D2Game.0x6FC3A490 (#10016)
int32_t __stdcall D2Game_10016(uint16_t a1)
{
    EnterCriticalSection(&gCriticalSection_6FD45800);

    int32_t nGUID = 0;
    if (gpGameArray_6FD447F4[a1] && gpGameArray_6FD447F4[a1] != -1)
    {
        nGUID = gpGameArray_6FD447F4[a1];
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    D2GameStrc* pGame = nullptr;
    if (gpGameDataTbl_6FD45818)
    {
        EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
       
        pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
        if (pGame)
        {
            D2_ASSERT(pGame->lpCriticalSection);
            EnterCriticalSection(pGame->lpCriticalSection);
        }

        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    }

    int32_t nUnits = 0;
    if (pGame)
    {
        for (int32_t i = 0; i < 128; ++i)
        {
            D2UnitStrc* pFirstUnit = pGame->pUnitList[0][i];
            if (pFirstUnit)
            {
                ++nUnits;
                for (D2UnitStrc* pUnit = SUNIT_GetNextUnitFromList(pFirstUnit); pUnit; pUnit = SUNIT_GetNextUnitFromList(pUnit))
                {
                    ++nUnits;
                }
            }
        }
        
        D2_ASSERT(pGame->lpCriticalSection);
        LeaveCriticalSection(pGame->lpCriticalSection);
    }

    return nUnits;
}

//D2Game.0x6FC3A5A0 (#10017)
int32_t __stdcall D2Game_10017(uint16_t nGameId, D2UnitInfoStrc* pUnitInfo, int32_t nMaxCount)
{
    const int32_t nGUID = sub_6FC35840(nGameId);

    D2GameStrc* pGame = nullptr;
    if (gpGameDataTbl_6FD45818)
    {
        EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
        
        pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
        if (pGame)
        {
            D2_ASSERT(pGame->lpCriticalSection);
            EnterCriticalSection(pGame->lpCriticalSection);
        }

        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    }

    int32_t nUnitInfoCount = 0;
    if (pGame)
    {
        for (int32_t i = 0; nUnitInfoCount < nMaxCount && i < 128; ++i)
        {
            for (D2UnitStrc* j = pGame->pUnitList[0][i]; j; j = SUNIT_GetNextUnitFromList(j))
            {
                if (nUnitInfoCount >= nMaxCount)
                {
                    break;
                }

                SUNIT_FillUnitInfo(j, &pUnitInfo[nUnitInfoCount]);
                ++nUnitInfoCount;
            }
        }

        D2_ASSERT(pGame->lpCriticalSection);
        LeaveCriticalSection(pGame->lpCriticalSection);
    }

    return nUnitInfoCount;
}

//D2Game.0x6FC3A6F0
int32_t __stdcall GAME_ReturnArgument(int32_t a1)
{
    return a1;
}

//D2Game.0x6FC3A700
void __fastcall GAME_GetPlayerDescription(char* szDescription, int32_t nClassId)
{
    switch (nClassId)
    {
    case PCLASS_AMAZON:
        strcpy(szDescription, "Amazon");
        break;
    case PCLASS_SORCERESS:
        strcpy(szDescription, "Sorceress");
        break;
    case PCLASS_NECROMANCER:
        strcpy(szDescription, "Necromancer");
        break;
    case PCLASS_PALADIN:
        strcpy(szDescription, "Paladin");
        break;
    case PCLASS_BARBARIAN:
        strcpy(szDescription, "Barbarian");
        break;
    case PCLASS_DRUID:
        strcpy(szDescription, "Druid");
        break;
    case PCLASS_ASSASSIN:
        strcpy(szDescription, "Assassin");
        break;
    default:
        strcpy(szDescription, "Invalid Class");
        break;
    }
}

//D2Game.0x6FC3A7C0
void __fastcall GAME_GetMonsterDescription(char* szDescription, int32_t nClassId)
{
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
    if (pMonStatsTxtRecord)
    {
        Unicode::unicode2Win(szDescription, (Unicode*)D2LANG_GetStringFromTblIndex(pMonStatsTxtRecord->wNameStr), 64);
    }
    else
    {
        sprintf(szDescription, "Monster %d", nClassId);
    }
}

//D2Game.0x6FC3A820
void __fastcall GAME_GetObjectDescription(char* szDescription, int32_t nClassId)
{
    D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nClassId);
    if (pObjectsTxtRecord)
    {
        strcpy(szDescription, pObjectsTxtRecord->szName);
    }
    else
    {
        sprintf(szDescription, "Object %d", nClassId);
    }
}

//D2Game.0x6FC3A860
void __fastcall GAME_GetItemDescription(char* szDescription, int32_t nClassId)
{
    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nClassId);
    if (pItemsTxtRecord)
    {
        Unicode::unicode2Win(szDescription, (Unicode*)D2LANG_GetStringFromTblIndex(pItemsTxtRecord->wNameStr), 64);
    }
    else
    {
        sprintf(szDescription, "Item %d", nClassId);
    }
}

//D2Game.0x6FC3A8A0
void __fastcall GAME_GetMissileDescription(char* szDescription, int32_t nClassId)
{
    sprintf(szDescription, "Missile %d", nClassId);
}

//D2Game.0x6FC3A8B0) --------------------------------------------------------
void __stdcall D2Game_10019(uint16_t a1, int32_t a2, int32_t nUnitType)
{
    UNIMPLEMENTED();
//    int32_t v3; // edi@1
//    int32_t v4; // eax@1
//    int32_t v5; // esi@4
//    D2GameStrc* v6; // edi@5
//    D2GameStrc* v7; // eax@6
//    LPCRITICAL_SECTION v8; // ecx@8
//    D2UnitStrc** v9; // edx@16
//    int32_t v10; // eax@21
//    D2UnitStrc* v11; // ebp@22
//    int32_t v12; // esi@23
//    int32_t v13; // eax@23
//    void* v14; // edx@26
//    int32_t i; // esi@41
//    D2GameStrc* v16; // [sp+10h] [bp-8h]@5
//    int32_t v17; // [sp+14h] [bp-4h]@21
//    D2UnitStrc** v18; // [sp+1Ch] [bp+4h]@13
//
//    v3 = 0;
//    EnterCriticalSection(&gCriticalSection_6FD45800);
//    v4 = gpGameArray_6FD447F4[a1];
//    if (v4 && v4 != -1)
//        v3 = gpGameArray_6FD447F4[a1];
//    LeaveCriticalSection(&gCriticalSection_6FD45800);
//    v5 = (int32_t)gpGameDataTbl_6FD45818;
//    if (gpGameDataTbl_6FD45818)
//    {
//        EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
//        v7 = D2GAME_FindGameByGUID_6FC3B6A0((D2GameManagerStrc*)v5, v5 + 68, v3, (void*)(v5 + 68));
//        v6 = v7;
//        if (v7)
//        {
//            v8 = v7->lpCriticalSection;
//            if (!v8)
//            {
//                FOG_DisplayAssert("ptGame->_ptLock != nullptr", __FILE__, __LINE__);
//                exit(-1);
//            }
//            EnterCriticalSection(v8);
//            LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
//        }
//        else
//        {
//            D2GameManagerStrc_Unlock_6FC3B540(v5, (LPCRITICAL_SECTION)0xFFFFFFFF);
//        }
//        v16 = v6;
//    }
//    else
//    {
//        v6 = 0;
//        v16 = 0;
//    }
//    v18 = 0;
//    if (v6)
//    {
//        switch (nUnitType)
//        {
//        case UNIT_PLAYER:
//            v18 = (D2UnitStrc**)v6->pUnitList;
//            break;
//        case UNIT_MONSTER:
//            v9 = v6->pUnitList[1];
//            goto LABEL_20;
//        case UNIT_OBJECT:
//            v18 = v6->pUnitList[2];
//            break;
//        case UNIT_MISSILE:
//            v18 = v6->pUnitList[3];
//            break;
//        case UNIT_ITEM:
//            v9 = v6->pUnitList[4];
//LABEL_20:
//            v18 = v9;
//            break;
//        default:
//            break;
//        }
//        v10 = 0;
//        v17 = 0;
//        do
//        {
//            v11 = v18[v10];
//            if (v11)
//            {
//                do
//                {
//                    v12 = v11->dwClassId;
//                    v13 = a2;
//                    if (!a2)
//                    {
//                        FOG_DisplayAssert("ptList", __FILE__, __LINE__);
//                        exit(-1);
//                    }
//                    do
//                    {
//                        if (*(_DWORD*)v13 == v12)
//                        {
//                            ++* (_DWORD*)(v13 + 4);
//                            goto LABEL_28;
//                        }
//                        v13 = *(_DWORD*)(v13 + 140);
//                    }
//                    while (v13);
//                    v14 = FOG_AllocPool(0, 144, __FILE__, __LINE__, 0);
//                    if (v14)
//                    {
//                        memset(v14, 0, 0x90u);
//                        v6 = v16;
//                        *(_DWORD*)v14 = v12;
//                        *((_DWORD*)v14 + 2) = 0;
//                        *((_DWORD*)v14 + 1) = 1;
//                        *((_DWORD*)v14 + 35) = *(_DWORD*)(a2 + 140);
//                        *(_DWORD*)(a2 + 140) = v14;
//                    }
//LABEL_28:
//                    v11 = SUNIT_GetNextUnitFromList(v11);
//                }
//                while (v11);
//                v10 = v17;
//            }
//            ++v10;
//            v17 = v10;
//        }
//        while (v10 < 128);
//        if (!v6->lpCriticalSection)
//        {
//            FOG_DisplayAssert("ptGame->_ptLock != nullptr", __FILE__, __LINE__);
//            exit(-1);
//        }
//        LeaveCriticalSection(v6->lpCriticalSection);
//        if (IsBadCodePtr((FARPROC)gpfGetDescription_6FD2CA64[nUnitType]))
//        {
//            FOG_DisplayAssert("sgpfnGetDescription[eType]", __FILE__, __LINE__);
//            exit(-1);
//        }
//        for (i = *(_DWORD*)(a2 + 140); i; i = *(_DWORD*)(i + 140))
//            gpfGetDescription_6FD2CA64[nUnitType]((char*)(i + 12), *(_DWORD*)i);
//    }
}

//D2Game.0x6FC3AB20 (#10018)
int32_t __stdcall D2Game_10018(uint16_t nGameId, int32_t nMaxCount, D2UnitInfoStrc* pUnitInfo, int32_t* pUnitType, int32_t* pUnitGUID)
{
    const int32_t nGUID = sub_6FC35840(nGameId);

    D2GameStrc* pGame = nullptr;
    if (gpGameDataTbl_6FD45818)
    {
        EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);

        pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
        if (pGame)
        {
            D2_ASSERT(pGame->lpCriticalSection);
            EnterCriticalSection(pGame->lpCriticalSection);
        }

        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    }

    int32_t nUnitInfoCount = 0;
    if (pGame)
    {
        for (int32_t i = 0; i < nMaxCount; ++i)
        {
            D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, pUnitType[i], pUnitGUID[i]);
            if (pUnit)
            {
                SUNIT_FillUnitInfo(pUnit, &pUnitInfo[nUnitInfoCount]);

                if (pUnitType[i])
                {
                    if (IsBadCodePtr((FARPROC)gpfGetDescription_6FD2CA64[pUnitType[i]]))
                    {
                        FOG_DisplayAssert("sgpfnGetDescription[peTOUs[i]]", __FILE__, __LINE__);
                        exit(-1);
                    }

                    gpfGetDescription_6FD2CA64[pUnitType[i]](pUnitInfo[nUnitInfoCount].szName, pUnit->dwClassId);
                }

                ++nUnitInfoCount;
            }
        }

        D2_ASSERT(pGame->lpCriticalSection);
        LeaveCriticalSection(pGame->lpCriticalSection);
    }

    return nUnitInfoCount;
}

//D2Game.0x6FC3ACD0 (#10020)
void __stdcall GAME_GetStatistics(D2GameStatisticsStrc* pStats)
{
    pStats->nGamesCount = 0;
    pStats->nActiveMonsters = 0;
    pStats->nInactiveMonsters = 0;
    pStats->nPlayersCount = 0;

    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1 && gpGameDataTbl_6FD45818)
        {
            EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);

            D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, gnGamesGUIDs_6FD447F8[i], &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
            if (pGame)
            {
                D2_ASSERT(pGame->lpCriticalSection);
                EnterCriticalSection(pGame->lpCriticalSection);

                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
                
                for (int32_t j = 0; j < 128; ++j)
                {
                    for (D2UnitStrc* k = pGame->pUnitList[UNIT_MONSTER][j]; k; k = SUNIT_GetNextUnitFromList(k))
                    {
                        ++pStats->nActiveMonsters;
                    }

                    for (D2UnitStrc* k = pGame->pUnitList[UNIT_PLAYER][j]; k; k = SUNIT_GetNextUnitFromList(k))
                    {
                        ++pStats->nPlayersCount;
                    }
                }
                
                D2_ASSERT(pGame->lpCriticalSection);
                LeaveCriticalSection(pGame->lpCriticalSection);

                ++pStats->nGamesCount;
            }
            else
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            }
        }
    }
}

//D2Game.0x6FC3AE10 (#10021)
void __stdcall D2Game_10021(int32_t a1, int32_t nPacketParam, const char* szMessage)
{
    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1)
        {
            D2_ASSERT(gpGameDataTbl_6FD45818);

            EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
            
            D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, gnGamesGUIDs_6FD447F8[i], &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
            if (!pGame)
            {
                LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
                D2_ASSERT(pGame);
            }

            D2_ASSERT(pGame->lpCriticalSection);
            EnterCriticalSection(pGame->lpCriticalSection);

            LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

            if (a1 == 1)
            {
                D2GSPacketSrv5A packet5A = {};
                packet5A.nHeader = 0x5Au;
                packet5A.nType = 15;
                packet5A.nColor = 4;
                packet5A.dwParam = nPacketParam;
                packet5A.szText[16] = 0;
                sub_6FC84D40(pGame, &packet5A);
            }
            else
            {
                if (strlen(szMessage) <= 255)
                {
                    sub_6FC84CD0(pGame, szMessage, 1u);
                }
                else
                {
                    char szCopy[256] = {};
                    strncpy(szCopy, szMessage, 255);
                    szCopy[255] = '\0';
                    sub_6FC84CD0(pGame, szCopy, 1u);
                }
            }

            D2_ASSERT(pGame->lpCriticalSection);
            LeaveCriticalSection(pGame->lpCriticalSection);
        }
    }
}

//D2Game.0x6FC3AFB0 (#10022)
void __stdcall D2Game_10022(uint16_t nGameId, char* a2)
{
    int32_t nGUID = 0;
    EnterCriticalSection(&gCriticalSection_6FD45800);

    if (gpGameArray_6FD447F4[nGameId] && gpGameArray_6FD447F4[nGameId] != -1)
    {
        nGUID = gpGameArray_6FD447F4[nGameId];
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    D2_ASSERT(gpGameDataTbl_6FD45818);
    EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    
    D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
    if (!pGame)
    {
        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
        D2_ASSERT(pGame);
    }

    D2_ASSERT(pGame->lpCriticalSection);
    EnterCriticalSection(pGame->lpCriticalSection);

    LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);

    a2[255] = 0;
    if (!strncmp("<ABEND> ", a2, 8u))
    {
        pGame->unk0x1DDC = a2[8] & 0xF;
    }

    sub_6FC84CD0(pGame, a2, 1u);

    D2_ASSERT(pGame->lpCriticalSection);
    LeaveCriticalSection(pGame->lpCriticalSection);
}

//D2Game.0x6FC3B0E0
void __fastcall GAME_SendPacketToAllConnectedClients(D2GameStrc* pGame, void(__fastcall* pFn)(D2ClientStrc*, void*), void* pPacket)
{
    D2_ASSERT(pGame);

    if (IsBadCodePtr((FARPROC)pFn))
    {
        FOG_DisplayAssert("pfn", __FILE__, __LINE__);
        exit(-1);
    }

    for (D2ClientStrc* pClient = pGame->pClientList; pClient; pClient = pClient->pNext)
    {
        if (pClient->dwClientState == 4)
        {
            pFn(pClient, pPacket);
        }
    }
}

//D2Game.0x6FC3B160
D2GameStrc* __fastcall sub_6FC3B160()
{
    int32_t nGUID = 0;
    EnterCriticalSection(&gCriticalSection_6FD45800);

    if (*gnGamesGUIDs_6FD447F8 && *gnGamesGUIDs_6FD447F8 != -1)
    {
        nGUID = *gnGamesGUIDs_6FD447F8;
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    if (!gpGameDataTbl_6FD45818)
    {
        gpGame_6FD457FC = nullptr;
        return gpGame_6FD457FC;
    }

    EnterCriticalSection(&gpGameDataTbl_6FD45818->pLock);

    D2GameStrc* pGame = D2GAME_FindGameByGUID_6FC3B6A0(gpGameDataTbl_6FD45818, 0, nGUID, &gpGameDataTbl_6FD45818->pGamesEx.unk0x1C);
    if (!pGame)
    {
        LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
        gpGame_6FD457FC = nullptr;
        return gpGame_6FD457FC;
    }

    D2_ASSERT(pGame->lpCriticalSection);

    EnterCriticalSection(pGame->lpCriticalSection);
    LeaveCriticalSection(&gpGameDataTbl_6FD45818->pLock);
    gpGame_6FD457FC = pGame;

    return gpGame_6FD457FC;
}

//D2Game.0x6FC3B220
void __fastcall GAME_LeaveGlobalGamesCriticalSection()
{
    return GAME_LeaveGamesCriticalSection(gpGame_6FD457FC);
}

//D2Game.0x6FC3B280
void __stdcall D2Game_10008(int32_t a1)
{
    dword_6FD2CA60 = a1;
    dword_6FD457F8 = 1000 / a1;
}

//D2Game.0x6FC3B2A0 (#10009)
void __stdcall GAME_SetGlobalAct(int32_t nAct)
{
    gnAct_6FD45824 = nAct;
}

//D2Game.0x6FC3B2B0
void __fastcall sub_6FC3B2B0(D2UnitStrc* pUnit, D2GameStrc* pGame)
{
    D2_ASSERT(pGame);

    if (IsBadCodePtr((FARPROC)sub_6FC3B3D0))
    {
        FOG_DisplayAssert("pfn", __FILE__, __LINE__);
        exit(-1);
    }

    for (D2ClientStrc* pClient = pGame->pClientList; pClient; pClient = pClient->pNext)
    {
        if (pClient->dwClientState == 4)
        {
            sub_6FC3B3D0(pClient, pUnit);
        }
    }

    SUNIT_RemoveUnit(pGame, pUnit);
}

//D2Game.0x6FC3B3D0
void __fastcall sub_6FC3B3D0(D2ClientStrc* pClient, D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return;
    }

    D2RoomStrc* pUnitRoom = UNITS_GetRoom(pUnit);
    D2RoomStrc* pClientRoom = pClient->pRoom;
    D2_ASSERT(pClientRoom);

    D2RoomStrc** ppRoomList = nullptr;
    int32_t nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pClientRoom, &ppRoomList, &nNumRooms);

    for (int32_t i = 0; i < nNumRooms; ++i)
    {
        D2RoomStrc* pRoom = ppRoomList[i];
        D2_ASSERT(pRoom);

        if (pUnitRoom == pRoom)
        {
            D2GAME_PACKETS_SendPacket0x0A_RemoveObject_6FC3D3A0(pClient, 0xAu, pUnit->dwUnitType, pUnit->dwUnitId);
            return;
        }
    }
}

//D2Game.0x6FC3B480) --------------------------------------------------------
D2GameStrc* __fastcall D2GAME_FindAndLockGameByGUID__6FC3B480(D2GameManagerStrc* pGameManager, int32_t nUnused, int32_t a2, void* a3, int32_t a4)
{
    UNIMPLEMENTED();
    D2_MAYBE_UNUSED(nUnused);
    return nullptr;

//    int32_t v4; // esi@1
//    struct _RTL_CRITICAL_SECTION* v5; // edi@1
//    int32_t v6; // eax@1
//    int32_t v7; // ecx@1
//    int32_t v8; // ecx@1
//    int32_t v9; // esi@4
//
//    v4 = this;
//    v5 = (struct _RTL_CRITICAL_SECTION*)(this + 80);
//    EnterCriticalSection((LPCRITICAL_SECTION)(this + 80));
//    *(_DWORD*)a3 = 2 * (a4 != 0) - 1;
//    v6 = *(_DWORD*)(*(_DWORD*)(v4 + 28) + 12 * (a2 & *(_DWORD*)(v4 + 36)) + 8);
//    v7 = v6 < 0;
//    LOBYTE(v7) = v6 <= 0;
//    v8 = v6 & (v7 - 1);
//    if (v8 <= 0)
//    {
//LABEL_4:
//        v9 = 0;
//LABEL_6:
//        if (*(_DWORD*)a3)
//            LeaveCriticalSection(v5);
//        *(_DWORD*)a3 = 0;
//        return v9;
//    }
//    while (*(_DWORD*)v8 != a2)
//    {
//        v8 = *(_DWORD*)(v8 + *(_DWORD*)(*(_DWORD*)(v4 + 28) + 12 * (a2 & *(_DWORD*)(v4 + 36))) + 4);
//        if (v8 <= 0)
//            goto LABEL_4;
//    }
//    v9 = v8;
//    if (!v8)
//        goto LABEL_6;
//    return v9;
}

//D2Game.0x6FC3B510
// Should be __thiscall, nUnused is present due to using __fastcall 
void __fastcall D2GameManagerStrc_Lock_6FC3B510(D2GameManagerStrc* pGameManager, int32_t nUnused, int32_t* a2, int32_t a3)
{
    UNIMPLEMENTED();
    D2_MAYBE_UNUSED(nUnused);
    EnterCriticalSection(&pGameManager->pLock);
    *a2 = 2 * (a3 != 0) - 1;
}

//D2Game.0x6FC3B540
// Should be __thiscall, nUnused is present due to using __fastcall 
void __fastcall D2GameManagerStrc_Unlock_6FC3B540(D2GameManagerStrc* pGameManager, int32_t nUnused, int32_t bLeaveCriticalSection)
{
    D2_MAYBE_UNUSED(nUnused);
    if (bLeaveCriticalSection)
    {
        LeaveCriticalSection(&pGameManager->pLock);
    }
}

////D2Game.0x6FC3B560) --------------------------------------------------------
//int32_t __thiscall sub_6FC3B560(void* this, int32_t a2)
//{
//    int32_t v2; // eax@1
//    int32_t result; // eax@2
//    int32_t v4; // edx@3
//
//    v2 = *((_DWORD*)this + 1);
//    if (v2 >= 0)
//    {
//        v4 = a2;
//        if (a2 < 0)
//            v4 = (int32_t)((char*)this - *(_DWORD*)(*(_DWORD*)this + 4));
//        result = v4 + v2;
//    }
//    else
//    {
//        result = ~v2;
//    }
//    return result;
//}

//D2Game.0x6FC3B590) --------------------------------------------------------
D2GameStrc* __fastcall sub_6FC3B590(D2GameManagerStrc* a1, int32_t nUnused, int32_t a2, int32_t* a3, int32_t a4, int32_t a5)
{
    UNIMPLEMENTED();
    D2_MAYBE_UNUSED(nUnused);
    return nullptr;

//    int32_t v5; // esi@1
//    int32_t v6; // edx@1
//    uint32_t v7; // eax@2
//    int32_t v8; // eax@5
//    int32_t v9; // ecx@5
//    int32_t v10; // ecx@5
//    int32_t v11; // eax@6
//    int32_t v12; // eax@7
//    int32_t v13; // eax@8
//    int32_t result; // eax@13
//    int32_t v15; // ecx@14
//    int32_t v16; // ebx@16
//    int32_t v17; // edi@17
//    int32_t v18; // edi@18
//    int32_t v19; // edx@21
//
//    v5 = this;
//    v6 = a2 & *(_DWORD*)(this + 36);
//    if (*(_DWORD*)(this + 36) < 0x3FFu)
//    {
//        v7 = *(_DWORD*)(this + 16);
//        *(_DWORD*)(this + 16) = v7 <= 3 ? 0 : v7 - 3;
//        v8 = *(_DWORD*)(*(_DWORD*)(this + 28) + 12 * v6 + 8);
//        v9 = v8 < 0;
//        LOBYTE(v9) = v8 <= 0;
//        v10 = v8 & (v9 - 1);
//        if (v10 > 0)
//        {
//            while (1)
//            {
//                v11 = *(_DWORD*)(v5 + 16) + 1;
//                *(_DWORD*)(v5 + 16) = v11;
//                if ((uint32_t)v11 > 0xD)
//                    break;
//                v12 = 12 * v6 + *(_DWORD*)(v5 + 28);
//                if (v10)
//                    v13 = v10 + *(_DWORD*)v12;
//                else
//                    v13 = v12 + 4;
//                v10 = *(_DWORD*)(v13 + 4);
//                if (v10 <= 0)
//                    goto LABEL_13;
//            }
//            *(_DWORD*)(v5 + 16) = 0;
//            sub_6FC3B710(v5);
//            v6 = *(_DWORD*)(v5 + 36) & a2;
//        }
//    }
//LABEL_13:
//    result = (*(int32_t(__thiscall**)(int32_t, int32_t, int32_t, int32_t))(*(_DWORD*)v5 + 4))(v5, *(_DWORD*)(v5 + 28) + 12 * v6, a4, a5);
//    if (result)
//        v15 = result + *(_DWORD*)(v5 + 4);
//    else
//        v15 = v5 + 8;
//    v16 = *(_DWORD*)v15;
//    if (*(_DWORD*)v15)
//    {
//        v17 = *(_DWORD*)(v15 + 4);
//        if (v17 >= 0)
//            v18 = v15 - *(_DWORD*)(v16 + 4) + v17;
//        else
//            v18 = ~v17;
//        *(_DWORD*)v18 = v16;
//        *(_DWORD*)(*(_DWORD*)v15 + 4) = *(_DWORD*)(v15 + 4);
//        *(_DWORD*)v15 = 0;
//        *(_DWORD*)(v15 + 4) = 0;
//    }
//    v19 = *(_DWORD*)(v5 + 8);
//    *(_DWORD*)v15 = v19;
//    *(_DWORD*)(v15 + 4) = *(_DWORD*)(v19 + 4);
//    *(_DWORD*)(v19 + 4) = result;
//    *(_DWORD*)(v5 + 8) = v15;
//    *(_DWORD*)result = a2;
//    *(_BYTE*)(result + 20) = *(_BYTE*)a3;
//    return result;
}

//D2Game.0x6FC3B6A0) --------------------------------------------------------
D2GameStrc* __fastcall D2GAME_FindGameByGUID_6FC3B6A0(D2GameManagerStrc* a1, int32_t nUnused, D2GameGUID nGameGUID, void* a4)
{
    UNIMPLEMENTED();
    D2_MAYBE_UNUSED(nUnused);
    return nullptr;

//    int32_t v4; // eax@1
//    D2GameStrc* result; // eax@1
//
//    v4 = *(_DWORD*)(a1->field_0.field_1C + 12 * (a1->field_0.field_24 & a3) + 8);
//    result = (D2GameStrc*)(((v4 <= 0) - 1) & v4);
//    if ((int32_t)result <= 0)
//    {
//LABEL_4:
//        result = 0;
//    }
//    else
//    {
//        while (result->__0000[0] != a3)
//        {
//            result = *(D2GameStrc**)((char*)&result->__0000[1]
//                                     + *(_DWORD*)(a1->field_0.field_1C + 12 * (a1->field_0.field_24 & a3)));
//            if ((int32_t)result <= 0)
//                goto LABEL_4;
//        }
//    }
//    return result;
}

////D2Game.0x6FC3B6F0) --------------------------------------------------------
//int32_t __thiscall sub_6FC3B6F0(void* this, int32_t a2)
//{
//    int32_t result; // eax@2
//
//    if (a2)
//        result = a2 + *(_DWORD*)this;
//    else
//        result = (int32_t)((char*)this + 4);
//    return result;
//}

////D2Game.0x6FC3B710) --------------------------------------------------------
//void __thiscall sub_6FC3B710(int32_t this)
//{
//    int32_t v1; // esi@1
//    int32_t v2; // ebp@1
//    int32_t v3; // ebx@1
//    uint32_t v4; // edi@1
//    int32_t v5; // eax@3
//    int32_t v6; // eax@6
//    int32_t v7; // edi@7
//    int32_t v8; // esi@8
//    int32_t v9; // ebx@14
//    int32_t v10; // esi@15
//    int32_t v11; // eax@17
//    int32_t v12; // ecx@22
//    int32_t v13; // ebx@23
//    int32_t v14; // eax@23
//    int32_t v15; // edi@23
//    int32_t v16; // esi@23
//    int32_t v17; // eax@25
//    int32_t v18; // eax@28
//    int32_t v19; // [sp+10h] [bp-24h]@1
//    uint32_t v20; // [sp+14h] [bp-20h]@5
//    int32_t v21; // [sp+18h] [bp-1Ch]@1
//    int32_t v22; // [sp+1Ch] [bp-18h]@1
//    int32_t v23; // [sp+20h] [bp-14h]@1
//    int32_t v24; // [sp+24h] [bp-10h]@1
//    int32_t v25; // [sp+30h] [bp-4h]@1
//
//    v1 = this;
//    v2 = 0;
//    v19 = this;
//    v3 = *(_DWORD*)(this + 36) + 1;
//    v23 = 0;
//    v24 = 0;
//    v4 = 2 * v3;
//    v21 = 2 * v3;
//    v25 = 0;
//    v22 = 0;
//    sub_6FC3BB40((int32_t)&v22);
//    v25 = 1;
//    v22 = -572662307;
//    sub_6FC3BB40((int32_t)&v22);
//    v25 = 2;
//    if (v22 != 4)
//    {
//        while (v24 > 0)
//        {
//            v5 = sub_6FC3B6F0(&v22, v24);
//            sub_6FC3B960((void*)v5);
//        }
//        v22 = 4;
//        sub_6FC3BB40((int32_t)&v22);
//    }
//    v20 = 0;
//    if ((uint32_t)v3 > 0)
//    {
//        v6 = v23;
//        do
//        {
//            while (1)
//            {
//                v7 = *(_DWORD*)(*(_DWORD*)(v1 + 28) + v2 + 8);
//                if (v7 <= 0)
//                    break;
//                v8 = v7 + v22;
//                if (*(_DWORD*)(v7 + v22))
//                {
//                    sub_6FC3B960((void*)(v7 + v22));
//                    v6 = v23;
//                }
//                *(_DWORD*)v8 = v6;
//                *(_DWORD*)(v8 + 4) = *(_DWORD*)(v6 + 4);
//                *(_DWORD*)(v6 + 4) = v7;
//                v6 = v8;
//                v1 = v19;
//                v23 = v6;
//            }
//            v2 += 12;
//            ++v20;
//        }
//        while (v20 < v3);
//        v4 = 2 * v3;
//    }
//    sub_6FC3B9A0(v1 + 20, v4);
//    if (v4)
//    {
//        v9 = 0;
//        do
//        {
//            v10 = v9 + *(_DWORD*)(v1 + 28);
//            if (*(_DWORD*)v10 != 4)
//            {
//                while (*(_DWORD*)(v10 + 8) > 0)
//                {
//                    v11 = sub_6FC3B6F0((void*)v10, *(_DWORD*)(v10 + 8));
//                    sub_6FC3B960((void*)v11);
//                }
//                *(_DWORD*)v10 = 4;
//                sub_6FC3BB40(v10);
//            }
//            v1 = v19;
//            v9 += 12;
//            --v4;
//        }
//        while (v4);
//        v4 = v21;
//    }
//    *(_DWORD*)(v1 + 36) = v4 - 1;
//    while (1)
//    {
//        v12 = v24;
//        if (v24 <= 0)
//            break;
//        v13 = v24;
//        v14 = *(_DWORD*)(v1 + 28);
//        v15 = v14 + 12 * (*(_DWORD*)(v1 + 36) & *(_DWORD*)v24);
//        v16 = v24 + *(_DWORD*)(v14 + 12 * (*(_DWORD*)(v1 + 36) & *(_DWORD*)v24));
//        if (*(_DWORD*)v16)
//            sub_6FC3B960((void*)v16);
//        v17 = *(_DWORD*)(v15 + 4);
//        *(_DWORD*)v16 = v17;
//        *(_DWORD*)(v16 + 4) = *(_DWORD*)(v17 + 4);
//        *(_DWORD*)(v17 + 4) = v13;
//        *(_DWORD*)(v15 + 4) = v16;
//        v1 = v19;
//    }
//    v25 = 3;
//    while (v12 > 0)
//    {
//        v18 = sub_6FC3B6F0(&v22, v12);
//        sub_6FC3B960((void*)v18);
//        v12 = v24;
//    }
//    v25 = -1;
//    sub_6FC3B960(&v23);
//}

////D2Game.0x6FC3B8E0) --------------------------------------------------------
//void __thiscall sub_6FC3B8E0(int32_t this)
//{
//    int32_t v1; // ebx@1
//    int32_t v2; // edx@2
//    int32_t v3; // eax@3
//    int32_t v4; // edi@3
//    int32_t v5; // esi@4
//    int32_t v6; // esi@5
//    int32_t v7; // edx@8
//    int32_t v8; // ecx@9
//    int32_t v9; // ecx@10
//
//    v1 = this + 4;
//    while (1)
//    {
//        v2 = *(_DWORD*)(this + 8);
//        if (v2 <= 0)
//            break;
//        v3 = v2 + *(_DWORD*)this;
//        v4 = *(_DWORD*)v3;
//        if (*(_DWORD*)v3)
//        {
//            v5 = *(_DWORD*)(v3 + 4);
//            if (v5 >= 0)
//                v6 = v3 - *(_DWORD*)(v4 + 4) + v5;
//            else
//                v6 = ~v5;
//            *(_DWORD*)v6 = v4;
//            *(_DWORD*)(*(_DWORD*)v3 + 4) = *(_DWORD*)(v3 + 4);
//            *(_DWORD*)v3 = 0;
//            *(_DWORD*)(v3 + 4) = 0;
//        }
//    }
//    v7 = *(_DWORD*)v1;
//    if (*(_DWORD*)v1)
//    {
//        v8 = *(_DWORD*)(this + 8);
//        if (v8 >= 0)
//            v9 = v1 - *(_DWORD*)(v7 + 4) + v8;
//        else
//            v9 = ~v8;
//        *(_DWORD*)v9 = v7;
//        *(_DWORD*)(*(_DWORD*)v1 + 4) = *(_DWORD*)(v1 + 4);
//        *(_DWORD*)v1 = 0;
//        *(_DWORD*)(v1 + 4) = 0;
//    }
//}

////D2Game.0x6FC3B960) --------------------------------------------------------
//void __thiscall sub_6FC3B960(void* this)
//{
//    int32_t v1; // esi@1
//    int32_t v2; // edx@2
//    char* v3; // edx@3
//
//    v1 = *(_DWORD*)this;
//    if (*(_DWORD*)this)
//    {
//        v2 = *((_DWORD*)this + 1);
//        if (v2 >= 0)
//            v3 = (char*)this + v2 - *(_DWORD*)(v1 + 4);
//        else
//            v3 = (char*)~v2;
//        *(_DWORD*)v3 = v1;
//        *(_DWORD*)(*(_DWORD*)this + 4) = *((_DWORD*)this + 1);
//        *(_DWORD*)this = 0;
//        *((_DWORD*)this + 1) = 0;
//    }
//}

////D2Game.0x6FC3B9A0) --------------------------------------------------------
//int32_t __thiscall sub_6FC3B9A0(int32_t this, uint32_t a2)
//{
//    uint32_t v2; // ebp@1
//    int32_t v3; // esi@1
//    int32_t result; // eax@1
//    uint32_t v5; // ecx@3
//    uint32_t i; // eax@5
//    uint32_t v7; // edi@10
//    int32_t v8; // eax@12
//    uint32_t v9; // ebx@12
//    int32_t v10; // ebp@13
//    int32_t v11; // eax@16
//    int32_t v12; // ebx@17
//    uint32_t v13; // eax@18
//    int32_t v14; // edi@21
//    uint32_t v15; // ebp@21
//    int32_t v16; // edi@26
//    int32_t v17; // ebx@26
//    uint32_t v18; // ebx@30
//    uint32_t v19; // edi@30
//    int32_t v20; // [sp+24h] [bp-4h]@12
//
//    v2 = a2;
//    v3 = this;
//    result = *(_DWORD*)(this + 4);
//    if (a2 <= result)
//    {
//        if (a2 < result)
//        {
//            v18 = a2;
//            v19 = 12 * a2;
//            do
//            {
//                sub_6FC3B8E0(v19 + *(_DWORD*)(v3 + 8));
//                result = *(_DWORD*)(v3 + 4);
//                ++v18;
//                v19 += 12;
//            }
//            while (v18 < result);
//        }
//        goto LABEL_32;
//    }
//    if (a2 > *(_DWORD*)this)
//    {
//        v5 = *(_DWORD*)(this + 12);
//        if (!v5)
//        {
//            v5 = a2;
//            if (a2 >= 0x15)
//            {
//                *(_DWORD*)(v3 + 12) = 21;
//                v5 = 21;
//            }
//            else
//            {
//                for (i = a2 & (a2 - 1); i; i &= i - 1)
//                    v5 = i;
//                if (v5 < 1)
//                    v5 = 1;
//            }
//        }
//        v7 = a2;
//        if (a2 % v5)
//            v7 = a2 + v5 - a2 % v5;
//        v8 = *(_DWORD*)(v3 + 8);
//        v20 = *(_DWORD*)(v3 + 8);
//        v9 = v7;
//        if (v7 < *(_DWORD*)(v3 + 4))
//        {
//            v10 = v8 + 12 * v7;
//            do
//            {
//                sub_6FC3BB10(v10, 0);
//                ++v9;
//                v10 += 12;
//            }
//            while (v9 < *(_DWORD*)(v3 + 4));
//            v8 = v20;
//            v2 = a2;
//        }
//        *(_DWORD*)v3 = v7;
//        v11 = Storm_405(v8, 12 * v7, ".?AV?$TSExplicitList@USGAMEDATA@@$0?CCCCCCCD@@@", -2, 16);
//        *(_DWORD*)(v3 + 8) = v11;
//        if (!v11)
//        {
//            v12 = v20;
//            *(_DWORD*)(v3 + 8) = Storm_401(12 * v7, ".?AV?$TSExplicitList@USGAMEDATA@@$0?CCCCCCCD@@@", -2, 0);
//            if (v20)
//            {
//                v13 = *(_DWORD*)(v3 + 4);
//                if (v7 < v13)
//                    v13 = v7;
//                if (v13)
//                {
//                    v14 = 0;
//                    v15 = v13;
//                    do
//                    {
//                        sub_6FC3BB80(v14 + *(_DWORD*)(v3 + 8), v12);
//                        sub_6FC3BB10(v12, 0);
//                        v14 += 12;
//                        v12 += 12;
//                        --v15;
//                    }
//                    while (v15);
//                    v2 = a2;
//                    v12 = v20;
//                }
//                Storm_403(v12, ".?AV?$TSExplicitList@USGAMEDATA@@$0?CCCCCCCD@@@", -2, 0);
//            }
//        }
//    }
//    result = *(_DWORD*)(v3 + 4);
//    if (result >= v2)
//    {
//LABEL_32:
//        *(_DWORD*)(v3 + 4) = v2;
//        return result;
//    }
//    v16 = 12 * result;
//    v17 = v2 - result;
//    do
//    {
//        result = sub_6FC3BB50(v16 + *(_DWORD*)(v3 + 8));
//        v16 += 12;
//        --v17;
//    }
//    while (v17);
//    *(_DWORD*)(v3 + 4) = v2;
//    return result;
//}

////D2Game.0x6FC3BB10) --------------------------------------------------------
//int32_t __thiscall sub_6FC3BB10(int32_t this, char a2)
//{
//    int32_t v2; // esi@1
//
//    v2 = this;
//    sub_6FC3B8E0(this);
//    if (a2 & 1 && v2)
//        Storm_403(v2, "delete", -1, 0);
//    return v2;
//}

////D2Game.0x6FC3BB40) --------------------------------------------------------
//int32_t __thiscall sub_6FC3BB40(int32_t this)
//{
//    int32_t result; // eax@1
//
//    *(_DWORD*)(this + 4) = this + 4;
//    result = ~(this + 4);
//    *(_DWORD*)(this + 8) = result;
//    return result;
//}

////D2Game.0x6FC3BB50) --------------------------------------------------------
//int32_t __thiscall sub_6FC3BB50(int32_t this)
//{
//    int32_t result; // eax@2
//
//    if (this)
//    {
//        *(_DWORD*)(this + 4) = 0;
//        *(_DWORD*)(this + 8) = 0;
//        *(_DWORD*)(this + 4) = this + 4;
//        *(_DWORD*)this = -572662307;
//        result = ~(this + 4);
//        *(_DWORD*)(this + 8) = result;
//    }
//    return result;
//}

////D2Game.0x6FC3BB80) --------------------------------------------------------
//void __fastcall sub_6FC3BB80(int32_t a1, int32_t a2)
//{
//    int32_t v2; // edx@2
//
//    if (a1)
//    {
//        *(_DWORD*)(a1 + 4) = 0;
//        *(_DWORD*)(a1 + 8) = 0;
//        v2 = *(_DWORD*)a2;
//        *(_DWORD*)(a1 + 4) = a1 + 4;
//        *(_DWORD*)a1 = v2;
//        *(_DWORD*)(a1 + 8) = ~(a1 + 4);
//    }
//}
