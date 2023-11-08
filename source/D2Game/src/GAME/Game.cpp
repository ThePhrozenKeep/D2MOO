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

static_assert(offsetof(D2GameStrc, lpCriticalSection) == 0x18, "Make sure TSHashObject<D2GameStrc,HASHKEY_NONE> has the correct size.");
static_assert(sizeof(D2GameDataTableStrc) == 0x68, "Make sure D2GameDataTableStrc has the correct size.");

D2GameStrc* gpGame_6FD457FC;
int32_t gnAct_6FD45824;
D2ServerCallbackFunctions* gpD2EventCallbackTable_6FD45830;
CRITICAL_SECTION gCriticalSection_6FD45800;
HGAMEDATA hGameArray_6FD447F8[1024]; // TODO: Right size?
int32_t gwGameId_6FD2CA04 = 1;
D2GameDataTableStrc* gpGameDataTbl_6FD45818;
int32_t gbD2ServerCallbackFunctionsInitialized_6FD45834;
int32_t nInitSeed_6FDC2CA08 = -1;
int32_t dword_6FD45820;
int32_t dword_6FD4581C;
int32_t gnTargetFrameRate_6FD2CA60 = DEFAULT_FRAMES_PER_SECOND;
int32_t gnTargetMsPerFrame_6FD457F8;
int32_t gnFrameRate_6FD2CA14;
int32_t gnPeakMemoryUsageInLast10s_6FD45828;
DWORD dword_6FD4582C;
uint32_t gnPreviousMemUsageUpdateTickCount_6FD45840;
uint32_t dword_6FD45844;
uint32_t dword_6FD45848;
int32_t dword_6FD2CA10 = 1;
char gszEmptyString_6FD447EC[8];

int32_t gnGamesGUIDs_6FD447F8[1024];

BOOL gbAllowTimeoutDisconnection_6FD2CA00 = TRUE;

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
    vsprintf(szMessage, szFormat, va); // NOLINT(clang-diagnostic-deprecated-declarations)

    if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfServerLogMessage)
    {
        gpD2EventCallbackTable_6FD45830->pfServerLogMessage(a1, "%s", szMessage);
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
    
    const HGAMEDATA hGame = hGameArray_6FD447F8[nGameId-1];
    const D2GameGUID nGUID = GetHashValueFromGameHandle(hGame);
    if (nGUID && nGUID != -1)
    {
        nResult = nGUID;
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    return nResult;
}

//D2Game.0x6FC35880
void __stdcall GAME_InitGameDataTable(D2GameDataTableStrc* pGameDataTbl, void* pGameList)
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
        gpD2EventCallbackTable_6FD45830 = pD2ServerCallbackFunctions;
        gbD2ServerCallbackFunctionsInitialized_6FD45834 = 1;
        //D2COMMON_11277_Return((int32_t)pD2ServerCallbackFunctions->pfLoadComplete);
    }
    else
    {
        gpD2EventCallbackTable_6FD45830 = nullptr;
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

    gpGame_6FD457FC = GAME_LockGame(nGUID);

    if (!gpGame_6FD457FC)
    {
        return 0;
    }

    const uint32_t nInitSeed = gpGame_6FD457FC->dwInitSeed;
    D2_UNLOCK(gpGame_6FD457FC->lpCriticalSection);
    return nInitSeed;
}

// Helper function
D2GameStrc* GAME_FindGameWithName(const char* szGameName)
{
    for (int nCurrentGameGUID : gnGamesGUIDs_6FD447F8)
    {
        if (nCurrentGameGUID && nCurrentGameGUID != D2GameInvalidGUID)
        {
            if (D2GameStrc* pCurrentGame = GAME_LockGame(nCurrentGameGUID))
            {
                bool bGameNameMatches = SStrCmpI(szGameName, pCurrentGame->szGameName, ARRAY_SIZE(D2GameStrc::szGameName)) == 0;
                D2_UNLOCK(pCurrentGame->lpCriticalSection);

                if (bGameNameMatches)
                {
                    return pCurrentGame;
                }
            }
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
    if (gpD2EventCallbackTable_6FD45830 && pPacket->nGameType)
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

    if (hGameArray_6FD447F8[nGameId - 1])
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

            if (!hGameArray_6FD447F8[nGameId - 1])
            {
                break;
            }
        }
    }

    D2GameGUID* v21 = (D2GameGUID*)&hGameArray_6FD447F8[nGameId - 1];
    *v21 = -1;
    LeaveCriticalSection(&gCriticalSection_6FD45800);

    if (!nGameId)
    {
        GAME_LogMessage(6, "[SERVER]  SrvCreateNewEmptyGame: *** Out of game slots! ***");
        return 0;
    }


    GAMEDATALOCKEDHANDLE hLock;
    HGAMEDATA hGame;
    D2GameStrc* pGame = gpGameDataTbl_6FD45818->tHashTable.NewLock(&hGame, &hLock);
    gpGameDataTbl_6FD45818->tHashTable.Unlock(hLock);

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

    if (nFlags & GAMEFLAG_ARENA_EXPANSION)
    {
        pGame->bExpansion = 1;
    }
    else
    {
        pGame->bExpansion = 0;
    }

    pGame->wItemFormat = pGame->bExpansion != 0 ? 101 : 2;

    if (nFlags & GAMEFLAG_ARENA_LADDER)
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

    pGame->nDifficulty = (nFlags & GAMEFLAG_DIFFICULTY_MASK) >> GAMEFLAG_DIFFICULTY_BIT;

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
    *v21 = GetHashValueFromGameHandle(hGame);
    LeaveCriticalSection(&gCriticalSection_6FD45800);

    pGame->nSyncTimer = FOG_10055_GetSyncTime();
    if (pGame->nSyncTimer <= 1)
    {
        pGame->nSyncTimer = 2;
    }

    D2Game_10042((D2TaskStrc*) &pGame[1], 0, (D2LinkStrc*)hGame);
    pGame->nCreationTimeMs_Or_CPUTargetRatioFP10 = GetTickCount();
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
        if (D2GameStrc* pGame = GAME_LockGame(SERVER_GetClientGameGUID(nClientId)))
        {
            if (CLIENTS_IsInGame(pGame, nClientId))
            {
                CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
            }

            D2_UNLOCK(pGame->lpCriticalSection);
        }

        D2NET_10015(nClientId, __FILE__, __LINE__);
        return 0;
    }

    if (!nTotalSize)
    {
        FOG_Trace("Character size was zero");
        
        if (D2GameStrc* pGame = GAME_LockGame(SERVER_GetClientGameGUID(nClientId)))
        {
            if (CLIENTS_IsInGame(pGame, nClientId))
            {
                CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
            }

            D2_UNLOCK(pGame->lpCriticalSection);
        }

        D2NET_10015(nClientId, __FILE__, __LINE__);
        return 0;
    }

    D2GameStrc* pGame = GAME_LockGame(SERVER_GetClientGameGUID(nClientId));
    if (!pGame)
    {
        GAME_LogMessage(6, "[SERVER]  SrvRecvDatabaseCharacter: *** Failed SrvLockGame for client %d ***", nClientId);
        return 0;
    }

    if (!CLIENTS_IsInGame(pGame, nClientId))
    {
        D2_UNLOCK(pGame->lpCriticalSection);
        GAME_LogMessage(6, "[SERVER]  SrvRecvDatabaseCharacter: *** Failed SrvLockGame for client %d ***", nClientId);
        return 0;
    }

    D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientId);
    if (!pClient)
    {
        GAME_LogMessage(6, "[SERVER]  SrvRecvDatabaseCharacter: *** Failed ClientGetFromId for client %d in game %d '%s' ***", nClientId, pGame->nServerToken, pGame->szGameName);
        D2_UNLOCK(pGame->lpCriticalSection);
        return 0;
    }

    if (pClient->unk0x60 != a8)
    {
        D2_UNLOCK(pGame->lpCriticalSection);

        if (D2GameStrc* pGame = GAME_LockGame(SERVER_GetClientGameGUID(nClientId)))
        {
            if (CLIENTS_IsInGame(pGame, nClientId))
            {
                CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
            }

            D2_UNLOCK(pGame->lpCriticalSection);
        }

        D2NET_10015(nClientId, __FILE__, __LINE__);
        return 0;
    }

    pClient->unk0x190 = *(int32_t*)a7;
    pClient->unk0x194[0] = *(int32_t*)(a7 + 4);

    D2_UNLOCK(pGame->lpCriticalSection);

    D2GAME_PACKETS_SendHeaderOnlyPacket(pClient, 2u);
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

    GAMEDATALOCKEDHANDLE hLock;
    HGAMEDATA hGame;
    D2GameStrc* pGame = gpGameDataTbl_6FD45818->tHashTable.NewLock(&hGame, &hLock);
    gpGameDataTbl_6FD45818->tHashTable.Unlock(hLock);

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

    if (hGameArray_6FD447F8[nGameId - 1])
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

            if (!hGameArray_6FD447F8[nGameId - 1])
            {
                hGameArray_6FD447F8[nGameId - 1] = D2GameReservedSlotHandle;
                LeaveCriticalSection(&gCriticalSection_6FD45800);
                break;
            }
        }
    }
    else
    {
        hGameArray_6FD447F8[nGameId - 1] = D2GameReservedSlotHandle;
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

    D2ClientStrc* pClient = CLIENTS_AddToGame(pGame, nClientId, nCharTemplate, szClientName, gszEmptyString_6FD447EC, 0, nExpLost, a13, a14);
    SERVER_SetClientGameGUID(nClientId, GetHashValueFromGameHandle(hGame));

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
    if (hGameArray_6FD447F8[pGame->nServerToken-1] != D2GameReservedSlotHandle)
    {
        FOG_DisplayAssert("hGameArray[wGameId-1] == RESERVED_SLOT", __FILE__, __LINE__);
        exit(-1);
    }
    hGameArray_6FD447F8[pGame->nServerToken - 1] = hGame;
    LeaveCriticalSection(&gCriticalSection_6FD45800);

    D2GAME_PACKETS_SendPacket0x01_6FC3C7C0(pClient, 1, pGame);
    D2GAME_PACKETS_SendHeaderOnlyPacket(pClient, 0);
    CLIENTS_SetClientState(pClient, CLIENTSTATE_GAME_INIT_SENT);
    GAME_LogMessage(6, "[SERVER]  sSrvSendGameInit:      Sent game init to client %d '%s' for game %d '%s'", pClient->dwClientId, pClient->szName, pGame->nServerToken, pGame->szGameName);
    CLIENTS_SetActNo(pClient, gnAct_6FD45824);

    pGame->nSyncTimer = FOG_10055_GetSyncTime();
    if (pGame->nSyncTimer <= 1)
    {
        pGame->nSyncTimer = 2;
    }

    // TODO: 1st argument
    D2Game_10042((D2TaskStrc*)&pGame[1], 0, (D2LinkStrc*)hGame);

    if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfGetDatabaseCharacter)
    {
        FOG_DisplayAssert("!\"This is not supposed to happen!\"", __FILE__, __LINE__);
        exit(-1);
    }

    D2GAME_PACKETS_SendHeaderOnlyPacket(pClient, 2u);
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
            if (pClient->dwClientState == CLIENTSTATE_INGAME)
            {
                D2GAME_PACKETS_SendPacket0x5C_6FC3E090(pClient, pPlayer->dwUnitId);
            }
        }
    }
}

//D2Game.0x6FC36AE0
int32_t __fastcall GAME_VerifyJoinAct(int32_t nClientId)
{
    if (CLIENTS_CheckState(nClientId, CLIENTSTATE_GAME_INIT_SENT))
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
        if (D2GameStrc* pGame = GAME_LockGame(nGameGUID))
        {
            if (CLIENTS_IsInGame(pGame, nClientId)/* && CLIENTS_IsInGame(pGame, nClientId)*/)
            {
                CLIENTS_RemoveClientFromGame(pGame, nClientId, 1);
            }

            D2_UNLOCK(pGame->lpCriticalSection);
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
    const int32_t nGameGUID = SERVER_GetClientGameGUID(nClientId);
    
    D2GameStrc* pGame = GAME_LockGame(nGameGUID);
    if (!pGame)
    {
        return;
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
            CLIENTS_SetClientState(pClient, CLIENTSTATE_ACT_INIT_SENT);
            GAME_LogMessage(6, "[SERVER]  SrvSendActInit:        Sent act init to client %d '%s' for game %d '%s'", pClient->dwClientId, pClient->szName, pGame->nServerToken, pGame->szGameName);
            sub_6FC31EF0(pClient, CLIENTS_GetPlayerFromClient(pClient, 1), pGame, 0, 0, 0);
            CLIENTS_SetClientState(pClient, CLIENTSTATE_PLAYER_SPAWNED);
        }

        D2_UNLOCK(pGame->lpCriticalSection);
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
                if (!gpD2EventCallbackTable_6FD45830)
                {
                    return 1;
                }

                if (IsBadCodePtr((FARPROC)gpD2EventCallbackTable_6FD45830->pfFindPlayerToken))
                {
                    FOG_DisplayAssert("ptEventCallbackTable->fpFindPlayerToken", __FILE__, __LINE__);
                    exit(-1);
                }

                if (!gpD2EventCallbackTable_6FD45830->pfFindPlayerToken(szClientName, nToken, nGameId, szAccountName, a7, a9, a10))
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
    if (hGameArray_6FD447F8[nGameId - 1] && hGameArray_6FD447F8[nGameId - 1] != D2GameReservedSlotHandle)
    {
        nGUID = GetHashValueFromGameHandle(hGameArray_6FD447F8[nGameId - 1]);
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

    D2GameStrc* pGame = GAME_LockGame(nGUID);
    if (!pGame)
    {
        GAME_LogMessage(6, "[SERVER]  SrvJoinGame:           *** Failed to lock game %d (client %d '%s') ***", nGameId, dwClientId, szClientName);
        return;
    }

    SERVER_SetClientGameGUID(dwClientId, nGUID);

    D2ClientStrc* pClient = CLIENTS_AddToGame(pGame, dwClientId, a3, szClientName, szAccountName, a6, a7, a8, a9);
    if (pClient)
    {
        D2GAME_PACKETS_SendPacket0x01_6FC3C7C0(pClient, 1, pGame);
        D2GAME_PACKETS_SendHeaderOnlyPacket(pClient, 0);
        CLIENTS_SetClientState(pClient, CLIENTSTATE_GAME_INIT_SENT);
        GAME_LogMessage(6, "[SERVER]  sSrvSendGameInit:      Sent game init to client %d '%s' for game %d '%s'", pClient->dwClientId, pClient->szName, pGame->nServerToken, pGame->szGameName);
        pClient->nAct = gnAct_6FD45824;
        GAME_LogMessage(6, "[SERVER]  SrvJoinGame:           client %d '%s' joined game %d '%s'", dwClientId, szClientName, nGameId, pGame->szGameName);
        
        D2_UNLOCK(pGame->lpCriticalSection);

        if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfGetDatabaseCharacter)
        {
            if (IsBadCodePtr((FARPROC)gpD2EventCallbackTable_6FD45830->pfGetDatabaseCharacter))
            {
                FOG_DisplayAssert("ptEventCallbackTable->fpGetDatabaseCharacter", __FILE__, __LINE__);
                exit(-1);
            }

            gpD2EventCallbackTable_6FD45830->pfGetDatabaseCharacter(&pClient->pClientInfo, szClientName, dwClientId, szAccountName);
            sub_6FC33910(pClient);
        }
        else
        {
            D2GAME_PACKETS_SendHeaderOnlyPacket(pClient, 2u);
        }
    }
    else
    {
        if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfUnlockDatabaseCharacter)
        {
            GAME_LogMessage(6, "[SERVER]  SrvJoinGame:           *** Unable to add client %d '%s' to game %d (unlocking character)", dwClientId, szClientName, nGameId);

            if (IsBadCodePtr((FARPROC)gpD2EventCallbackTable_6FD45830->pfUnlockDatabaseCharacter))
            {
                FOG_DisplayAssert("ptEventCallbackTable->fpUnlockDatabaseCharacter", __FILE__, __LINE__);
                exit(-1);
            }

            gpD2EventCallbackTable_6FD45830->pfUnlockDatabaseCharacter(&pGame->nGameData, szClientName, szAccountName);
        }

        D2_UNLOCK(pGame->lpCriticalSection);

        sub_6FC3C690(dwClientId);
        D2NET_10016(dwClientId);
    }
}

//D2Game.0x6FC37450
void __fastcall GAME_FreeGame(D2GameGUID nGameGUID, D2GameStrc* pGame)
{
    if (pGame)
    {
        GAME_LogMessage(6, "[SERVER]  SrvFreeGame:           freeing game %d '%s'", pGame->nServerToken, pGame->szGameName);
    }

    EnterCriticalSection(&gCriticalSection_6FD45800);
    
    for (int32_t i = 0; i < 1024; ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] == nGameGUID)
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
    if (CLIENTS_CheckState(nClientId, CLIENTSTATE_INGAME))
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

            if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfUpdateCharacterLadder)
            {
                const int32_t nExperience = STATLIST_GetUnitBaseStat(pPlayer, STAT_EXPERIENCE, 0);
                const int32_t nLevel = STATLIST_GetUnitBaseStat(pPlayer, STAT_LEVEL, 0);
                const char* szClientName = CLIENTS_GetName(i);
                gpD2EventCallbackTable_6FD45830->pfUpdateCharacterLadder(szClientName, pPlayer->dwClassId, nLevel, nExperience, 0, CLIENTS_GetFlags(i), &i->unk0x190);
            }
        }
    }
}

//D2Game.0x6FC37690
void __fastcall GAME_DisconnectClient(D2GameStrc* pGame, D2ClientStrc* pClient, D2C_SRV2CLT5A_TYPES nEventType)
{
    if (!pClient)
    {
        return;
    }

    const int32_t nClientId = CLIENTS_GetClientId(pClient);

    D2GSPacketSrv5A packet5A = {};
    packet5A.nHeader = 0x5Au;
    packet5A.nType = nEventType;
    packet5A.nColor = STRCOLOR_DARK_GOLD;
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
void __fastcall GAME_DisconnectClientById(int32_t nClientId, D2C_SRV2CLT5A_TYPES nEventType)
{
    if (!nClientId)
    {
        return;
    }

    const int32_t nGUID = SERVER_GetClientGameGUID(nClientId);
    D2GameStrc* pGame = GAME_LockGame(nGUID);
    if (!pGame)
    {
        GAME_LogMessage(6, "[SERVER]  SrvDisconnectClientById: *** failed to disconnect client %d ***", nClientId);
        return;
    }

    if (!CLIENTS_IsInGame(pGame, nClientId))
    {
        D2_UNLOCK(pGame->lpCriticalSection);

        GAME_LogMessage(6, "[SERVER]  SrvDisconnectClientById: *** failed to disconnect client %d ***", nClientId);
        return;
    }

    GAME_DisconnectClient(pGame, CLIENTS_GetClientFromClientId(pGame, nClientId), nEventType);
    
    D2_UNLOCK(pGame->lpCriticalSection);
}

//D2Game.0x6FC37880
int32_t __stdcall GAME_DisconnectClientByName(const char* szClientName, D2C_SRV2CLT5A_TYPES nEventType)
{
    const int32_t nClientId = sub_6FC33F20(szClientName);
    if (!nClientId)
    {
        return 0;
    }

    GAME_DisconnectClientById(nClientId, nEventType);
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
    
    D2GameStrc* pGame = GAME_LockGame(nGUID);
    if (!pGame)
    {
        return;
    }

    if (CLIENTS_IsInGame(pGame, nClientId))
    {
        D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientId);
        D2GSPacketSrv5A packet5A = {};
        packet5A.nHeader = 0x5Au;
        packet5A.nType = EVENTTYPE_DISCONNECT;
        packet5A.nColor = STRCOLOR_DARK_GOLD;
        packet5A.dwParam = 0;
        packet5A.szText[16] = 0;
        SStrCopy(packet5A.szText, CLIENTS_GetName(pClient), 16u);
        sub_6FC84D40(pGame, &packet5A);
        CLIENTS_RemoveClientFromGame(pGame, CLIENTS_GetClientId(pClient), 1);
    }

    D2_UNLOCK(pGame->lpCriticalSection);
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
        GAME_DisconnectClient(pGame, pClient, EVENTTYPE_DISCONNECT);
    }
}

//D2Game.0x6FC37CE0
void __fastcall GAME_EndGame(int32_t nClientId, int32_t a2)
{
    D2GameStrc* pGame = GAME_LockGame(SERVER_GetClientGameGUID(nClientId));

    // TODO: v10
    if (!pGame)
    {
        GAME_LogMessage(6, "[SERVER]  SrvEndGame: Client %d was not in any game", nClientId);
        return;
    }
    
    if (!CLIENTS_IsInGame(pGame, nClientId))
    {
        D2_UNLOCK(pGame->lpCriticalSection);

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
                if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfUpdateCharacterLadder)
                {
                    gpD2EventCallbackTable_6FD45830->pfUpdateCharacterLadder(
                        CLIENTS_GetName(i),
                        pUnit->dwClassId,
                        STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0),
                        STATLIST_GetUnitBaseStat(pUnit, STAT_EXPERIENCE, 0),
                        0,
                        CLIENTS_GetFlags(i),
                        &i->unk0x190
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
            D2GAME_PACKETS_SendHeaderOnlyPacket(pClient, 5u);

            if ((pGame->nGameType == 1 || pGame->nGameType == 2) && CLIENT_GetSaveHeader_6FC34350(pClient))
            {
                do
                {
                    sub_6FC37B90(pGame, pClient);
                }
                while (CLIENT_GetSaveHeader_6FC34350(pClient));
            }

            D2GAME_PACKETS_SendHeaderOnlyPacket(pClient, 6u);

            sub_6FC3C690(nClientId);
            sub_6FC39030(pGame, pClient, 0, 0);

            D2GSPacketSrv5A packet5A = {};
            packet5A.nHeader = 0x5Au;
            packet5A.nType = EVENTTYPE_PLAYERLEFT;
            packet5A.nColor = STRCOLOR_DARK_GOLD;
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

    D2_UNLOCK(pGame->lpCriticalSection);

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
    GAME_ForEachIngameClient(pGame, sub_6FC380D0, &pCheckedClient);

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
    GAME_ForEachIngameClient(pGame, sub_6FC380D0, &pCheckedClient);

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
    uint8_t a3[16];
    char szIPAddress[16];

    if (gpD2EventCallbackTable_6FD45830)
    {
        const int nClientIdx = *(DWORD*)a1;
        switch (*((BYTE*)a1 + 5))
        {
        case PACKET_ADMIN_GETINFO:
        case PACKET_ADMIN_GETGAMEINFO:
            if (gpD2EventCallbackTable_6FD45830->pfHandlePacket && gpD2EventCallbackTable_6FD45830->pfServerLogMessage)
            {
                D2GSPacketSrvFF01 tPacket;
                D2GameServerInfoExStrc tGameServerInfoEx;
                gpD2EventCallbackTable_6FD45830->pfHandlePacket(&tGameServerInfoEx, a2);
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
void __fastcall sub_6FC385A0(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom)
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

                    if (pClient->dwClientState == CLIENTSTATE_INGAME && pClient->nAct == i)
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
    const uint32_t nDebugBreakTrigger = *(uint32_t*)&pGame[1].m_key;
    if (nDebugBreakTrigger == 1)
    {
        *((volatile BYTE*)0) = 0;
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
    const uint32_t nTickDiff = nTickCount - pGame->nPreviousUpdateTickCount;
    const uint32_t nTicksPerSec = 1000;
    if (nTickDiff >= nTicksPerSec)
    {
        const uint32_t nFrames = pGame->nFramesSinceLastFrameRateUpdate;
        pGame->nFramesSinceLastFrameRateUpdate = 0;
        pGame->nPreviousUpdateTickCount = nTickCount;
        pGame->nFrameRate = nTicksPerSec * nFrames / nTickDiff;
        gnFrameRate_6FD2CA14 = pGame->nFrameRate;

        const uint32_t nMemoryUsage = FOG_GetMemoryUsage(pGame->pMemoryPool);
        if ((nTickCount - gnPreviousMemUsageUpdateTickCount_6FD45840) > (10 * nTicksPerSec))
        {
            // "Forget" previous results every 10s
            gnPreviousMemUsageUpdateTickCount_6FD45840 = nTickCount;
            gnPeakMemoryUsageInLast10s_6FD45828 = nMemoryUsage;
        }
        else if (nMemoryUsage > gnPeakMemoryUsageInLast10s_6FD45828)
        {
            gnPeakMemoryUsageInLast10s_6FD45828 = nMemoryUsage;
        }
    }

    ++pGame->nFramesSinceLastFrameRateUpdate;

    for (int32_t i = 0; i < 5; ++i)
    {
        if (pGame->pAct[i] && pGame->pAct[i]->unk0x20)
        {
            for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(pGame->pAct[i]); pRoom; pRoom = pRoom->pRoomNext)
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
                D2ActiveRoomStrc* pNextRoom = nullptr;
                for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(pGame->pAct[i]); pRoom; pRoom = pNextRoom)
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
void __fastcall GAME_PopulateRoom(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom)
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

    if (gbAllowTimeoutDisconnection_6FD2CA00)
    {
        const uint32_t nTickCount = GetTickCount();
        
        D2ClientStrc* pClient = pGame->pClientList;
        while (pClient)
        {
            D2ClientStrc* pNext = pClient->pNext;
            if (nTickCount > pClient->dwLastPacketTick && (CLIENTS_CheckFlag(pClient, CLIENTSAVEFLAG_HARDCORE) && nTickCount - pClient->dwLastPacketTick > 10000 && pClient->dwPingsCount > 10 || nTickCount - pClient->dwLastPacketTick > 45000) && gpD2EventCallbackTable_6FD45830)
            {
                D2GAME_SAVE_WriteFile_6FC8A500(pGame, CLIENTS_GetPlayerFromClient(pClient, 0), CLIENTS_GetName(pClient), 0);
                bPlayerDisconnected = 1;
                GAME_LogMessage(6, "[DISCONNECT]  PLAYER:%s  REASON:Heartbeat Timeout", CLIENTS_GetName(pClient));
                GAME_DisconnectClient(pGame, pClient, EVENTTYPE_DISCONNECT);
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

                    if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfUpdateCharacterLadder)
                    {
                        gpD2EventCallbackTable_6FD45830->pfUpdateCharacterLadder(
                            CLIENTS_GetName(pClient),
                            pPlayer->dwClassId,
                            STATLIST_GetUnitBaseStat(pPlayer, STAT_LEVEL, 0),
                            STATLIST_GetUnitBaseStat(pPlayer, STAT_EXPERIENCE, 0),
                            0,
                            CLIENTS_GetFlags(pClient),
                            &pClient->unk0x190
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
                if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfUpdateCharacterLadder)
                {
                    gpD2EventCallbackTable_6FD45830->pfUpdateCharacterLadder(
                        CLIENTS_GetName(pClient),
                        pPlayer->dwClassId, STATLIST_GetUnitBaseStat(pPlayer, STAT_LEVEL, 0),
                        STATLIST_GetUnitBaseStat(pPlayer, STAT_EXPERIENCE, 0),
                        0,
                        CLIENTS_GetFlags(pClient),
                        &pClient->unk0x190
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
        case CLIENTSTATE_PLAYER_SPAWNED:
        {
            sub_6FC33670(pGame, pClient);

            if (D2Common_10073(pClient->pRoom) && CLIENTS_GetPlayerFromClient(pClient, 0))
            {
                D2GAME_PACKETS_SendHeaderOnlyPacket(pClient, 4u);
                CLIENTS_SetClientState(pClient, CLIENTSTATE_INGAME);

                D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
                if (pPlayer)
                {
                    D2GAME_ITEMS_UpdateInventoryItems_6FC45050(pGame, pPlayer, 0, 1);
                }

                sub_6FC369C0(pGame, pClient);
                sub_6FCBAB20(pGame, CLIENTS_GetPlayerFromClient(pClient, 1));

                if (gpD2EventCallbackTable_6FD45830)
                {
                    pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
                    if (pPlayer && gpD2EventCallbackTable_6FD45830->pfEnterGame)
                    {
                        gpD2EventCallbackTable_6FD45830->pfEnterGame(pGame->nServerToken, pClient->szName, pPlayer->dwClassId, STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0), pClient->nSaveFlags);
                    }
                }

                
                D2GSPacketSrv5A packet5A = {};
                packet5A.nHeader = 0x5A;
                packet5A.nType = EVENTTYPE_PLAYERJOIN;
                packet5A.nColor = STRCOLOR_DARK_GOLD;
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
        case CLIENTSTATE_INGAME:
        {
            sub_6FC33670(pGame, pClient);
            break;
        }

        case CLIENTSTATE_CHANGING_ACT:
        {
            sub_6FC33670(pGame, pClient);

            if (D2Common_10073(pClient->pRoom))
            {
                D2GAME_PACKETS_SendHeaderOnlyPacket(pClient, 4u);
                CLIENTS_SetClientState(pClient, CLIENTSTATE_INGAME);

                D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
                if (pPlayer)
                {
                    D2GAME_ITEMS_UpdateInventoryItems_6FC45050(pGame, pPlayer, 0, 1);
                }
            }
            break;
        }
        default:
            break;
        }

        pClient = pNext;
    }
}

//D2Game.0x6FC38E00
void __fastcall sub_6FC38E00()
{
    gnTargetMsPerFrame_6FD457F8 = 1000 / gnTargetFrameRate_6FD2CA60;
}

//D2Game.0x6FC38E20 (#10004)
int32_t __stdcall GAME_UpdateGamesProgress(int32_t a1)
{
    // TODO: Names
    LARGE_INTEGER start = {};
    QueryPerformanceCounter(&start);

    const uint32_t nSysTimeMs = timeGetTime() & 0x7FFFFFFF;
    if (!dword_6FD45844)
    {
        dword_6FD45844 = nSysTimeMs;
    }

    if (nSysTimeMs - dword_6FD45844 < gnTargetMsPerFrame_6FD457F8)
    {
        return 0;
    }

    int32_t v5 = nSysTimeMs - dword_6FD45844 - gnTargetMsPerFrame_6FD457F8;
    if (a1 && v5 >= gnTargetMsPerFrame_6FD457F8)
    {
        v5 = gnTargetMsPerFrame_6FD457F8;
    }

    dword_6FD45844 = nSysTimeMs - v5;
    
    int32_t bQueryPerformance = 0;
    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        const int32_t nGUID = gnGamesGUIDs_6FD447F8[i];
        if (nGUID && nGUID != -1)
        {
            if (D2GameStrc* pGame = GAME_LockGame(nGUID))
            {
                bQueryPerformance = 1;

                if (dword_6FD2CA10)
                {
                    pGame->nCreationTimeMs_Or_CPUTargetRatioFP10 = 1024;
                }
                else
                {
                    if (pGame->nLastUpdateSystemTimeMs)
                    {
                        const uint32_t nTimeSinceLastUpdateMs = nSysTimeMs - pGame->nLastUpdateSystemTimeMs;
                        pGame->nCreationTimeMs_Or_CPUTargetRatioFP10 = D2Clamp((nTimeSinceLastUpdateMs << 10) / gnTargetMsPerFrame_6FD457F8, 10u, 2048u);
                    }

                    pGame->nLastUpdateSystemTimeMs = nSysTimeMs;
                }

                GAME_UpdateProgress(pGame);

                D2_UNLOCK(pGame->lpCriticalSection);
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

    if (a3 && pClient->dwClientState == CLIENTSTATE_INGAME)
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
                GAME_DisconnectClient(pGame, pClient, EVENTTYPE_DISCONNECT);
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
        if (nGUID && nGUID != -1)
        {
            GAMEDATALOCKEDHANDLE hLock;
            if(D2GameStrc* pGame = GAME_LockGame(nGUID))
            {
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
                    pGame->nTickCountSinceNoClients = 0;
                    D2_UNLOCK(pGame->lpCriticalSection);
                }
                else
                {
                    if (!pGame->nTickCountSinceNoClients)
                    {
                        pGame->nTickCountSinceNoClients = nTickCount;
                    }

                    const uint32_t nTicksToWaitBeforePurgingEmptyGame = 5 * 60 * 1000; // 5 minutes
                    if (
                        // No clients for more than 5 minutes.
                        (nTickCount - pGame->nTickCountSinceNoClients) > nTicksToWaitBeforePurgingEmptyGame
                        // No clients during the first 1-5 minutes, close game immediately (creator failed to join)
                        || pGame->dwGameFrame > (60 * DEFAULT_FRAMES_PER_SECOND) && pGame->dwGameFrame < (5 * 60 * DEFAULT_FRAMES_PER_SECOND) )
                    {
                        GAME_LogMessage(6, "[SERVER]  Deleting game from SrvSendAllMsgs()");

                        D2_UNLOCK(pGame->lpCriticalSection);
                        GAME_CloseGame(nGUID);
                    }
                    else
                    {
                        D2_UNLOCK(pGame->lpCriticalSection);
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

    D2GameStrc* pGame = GAME_LockGame(nGameGUID);
    if (!pGame)
    {
        return 0;
    }

    if (CLIENTS_IsInGame(pGame, nClientId))
    {
        return pGame;
    }

    D2_UNLOCK(pGame->lpCriticalSection);

    return nullptr;
}

//D2Game.0x6FC395B0
void __fastcall GAME_LeaveGamesCriticalSection(D2GameStrc* pGame)
{
    D2_ASSERT(pGame);
    D2_UNLOCK(pGame->lpCriticalSection);
}

//D2Game.0x6FC39600
void __fastcall GAME_CloseGame(D2GameGUID nGameGUID)
{
    if (gpGameDataTbl_6FD45818)
    {
        GAMEDATALOCKEDHANDLE hLock;
        if (D2GameStrc* pGame = gpGameDataTbl_6FD45818->tHashTable.Lock(GetGameHandleFromHashValue(nGameGUID), &hLock, TRUE))
        {
            if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfCloseGame)
            {
                D2_ASSERT(!IsBadCodePtr((FARPROC)gpD2EventCallbackTable_6FD45830->pfCloseGame));
                gpD2EventCallbackTable_6FD45830->pfCloseGame(
                    pGame->nServerToken,
                    pGame->bExpansion ? 'D2XP' : 'D2DV',
                    pGame->dwLastUsedUnitGUID[UNIT_PLAYER],
                    (signed int)pGame->dwGameFrame / 25);
            }
            D2_LOCK(pGame->lpCriticalSection);

            GAME_FreeGame(nGameGUID, pGame);

            gpGameDataTbl_6FD45818->tHashTable.DeleteUnlock(pGame, hLock);
        }
    }

}

//D2Game.0x6FC397A0
D2GameStrc* __fastcall GAME_LockGame(D2GameGUID nGameGUID)
{
    if (!gpGameDataTbl_6FD45818)
    {
        return nullptr;
    }

    GAMEDATALOCKEDHANDLE hLock;
    if (D2GameStrc* pGame = gpGameDataTbl_6FD45818->tHashTable.Lock(GetGameHandleFromHashValue(nGameGUID), &hLock, TRUE))
    {
        D2_LOCK(pGame->lpCriticalSection);
        gpGameDataTbl_6FD45818->tHashTable.Unlock(hLock);
        return pGame;
    }
    return nullptr;
}

//D2Game.0x6FC39870
void __fastcall sub_6FC39870(int32_t nClientId)
{
    for (int32_t i = 0; i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1)
        {
            if (D2GameStrc* pGame = GAME_LockGame(gnGamesGUIDs_6FD447F8[i]))
            {
                const uint32_t nClientCount = pGame->nClients;
                const uint16_t nServerToken = pGame->nServerToken;
                char szGameName[16] = {};
                SStrCopy(szGameName, pGame->szGameName, 0x7FFFFFFFu);
                
                D2_UNLOCK(pGame->lpCriticalSection);

                sub_6FC3C640(nClientId, nServerToken, nClientCount, szGameName);
            }
        }
    }

    sub_6FC3C640(nClientId, -1, 0, gszEmptyString_6FD447EC);
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

            D2GameStrc* pGame = GAME_LockGame(gnGamesGUIDs_6FD447F8[i]);
            D2_ASSERT(pGame);

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

            D2_UNLOCK(pGame->lpCriticalSection);

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
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1)
        {
            if (D2GameStrc* pGame = GAME_LockGame(gnGamesGUIDs_6FD447F8[i]))
            {
                if (pGame->bExpansion)
                {
                    ++nExpansionGames;
                }

                D2_UNLOCK(pGame->lpCriticalSection);
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
        const int32_t nGameGuid = gnGamesGUIDs_6FD447F8[i];
        if (nGameGuid && nGameGuid != -1)
        {
            if (D2GameStrc* pGame = GAME_LockGame(nGameGuid))
            {
                int32_t nIndex = pGame->nClients;
                
                D2_UNLOCK(pGame->lpCriticalSection);

                if (nIndex > nArraySize)
                {
                    nIndex = nArraySize;
                }

                ++pCount[nIndex];
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
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1)
        {
            if (D2GameStrc* pGame = GAME_LockGame(gnGamesGUIDs_6FD447F8[i]))
            {
                const int32_t nSecondsPassed = pGame->dwGameFrame / DEFAULT_FRAMES_PER_SECOND;

                D2_UNLOCK(pGame->lpCriticalSection);

                int32_t nIndex = nSecondsPassed / 300;
                if (nIndex >= nMaxCount)
                {
                    nIndex = nMaxCount - 1;
                }

                ++a1[nIndex];
            }
        }
    }
}

//D2Game.0x6FC39EC0 (#10051)
int32_t __fastcall D2Game_10051()
{
    return gnFrameRate_6FD2CA14;
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
    if (hGameArray_6FD447F8[nGameId - 1] && hGameArray_6FD447F8[nGameId - 1] != D2GameReservedSlotHandle)
    {
        nGUID = GetHashValueFromGameHandle(hGameArray_6FD447F8[nGameId - 1]);
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    D2_ASSERT(gpGameDataTbl_6FD45818);

    D2GameStrc* pGame = GAME_LockGame(nGUID);
    D2_ASSERT(pGame);
    const int32_t nClients = pGame->nClients;
    
    D2_UNLOCK(pGame->lpCriticalSection);

    return nClients;
}

//D2Game.0x6FC39FF0
int32_t __stdcall D2Game_10014(uint16_t nGameId, D2GameInfoStrc* pGameInfo)
{
    int32_t nGUID = 0;
    EnterCriticalSection(&gCriticalSection_6FD45800);

    if (hGameArray_6FD447F8[nGameId - 1] && hGameArray_6FD447F8[nGameId - 1] != D2GameReservedSlotHandle)
    {
        nGUID = GetHashValueFromGameHandle(hGameArray_6FD447F8[nGameId - 1]);
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    D2GameStrc* pGame = GAME_LockGame(nGUID);
    if (!pGame)
    {
        return 0;
    }

    pGameInfo->nServerToken = pGame->nServerToken;
    pGameInfo->nInitSeed = pGame->dwInitSeed;
    pGameInfo->nClients = pGame->nClients;
    for (int i = 0; i < UNIT_TYPES_COUNT; i++)
    {
        pGameInfo->dwLastUsedUnitGUID[i] = pGame->dwLastUsedUnitGUID[i];
    }

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

    if (pGame->nTotalPathTypesCount)
    {
        pGameInfo->nPathTowardPct = 100 * pGame->dwPathTypesCount[PATHTYPE_TOWARD] / pGame->nTotalPathTypesCount;
        pGameInfo->nPathClockPct = 100 * pGame->dwPathTypesCount[PATHTYPE_MON_CIRCLE_CW] / pGame->nTotalPathTypesCount;
        pGameInfo->nPathCounterPct = 100 * pGame->dwPathTypesCount[PATHTYPE_MON_CIRCLE_CCW] / pGame->nTotalPathTypesCount;
        pGameInfo->nPathFoWallPct = 100 * pGame->dwPathTypesCount[PATHTYPE_FOLLOW_WALL] / pGame->nTotalPathTypesCount;
        pGameInfo->nPathAStarPct = 100 * pGame->dwPathTypesCount[PATHTYPE_WF] / pGame->nTotalPathTypesCount;
        pGameInfo->nPathTotalCalls = pGame->nTotalPathTypesCount;
        if (pGame->nTotalPathTypesCount > 200 && pGameInfo->nPathTotalCalls)
        {
            memset(pGame->dwPathTypesCount, 0, sizeof(pGame->dwPathTypesCount));
            pGame->nTotalPathTypesCount = 0;
        }
    }

    SStrCopy(pGameInfo->szGameName, pGame->szGameName, 16u);

    // Should have been the uptime of the game, but this actually now always returns the uptime of the machine. (nCreationTimeMs_Or_CPUTargetRatioFP10 always 1024)
    pGameInfo->nTime = (GetTickCount() - pGame->nCreationTimeMs_Or_CPUTargetRatioFP10) / 1000 / 60;
    pGameInfo->nFrames = pGame->dwGameFrame / 100;
    pGameInfo->nFrameRate = pGame->nFrameRate;
    pGameInfo->nArenaTemplate = pGame->nArenaTemplate;
    pGameInfo->unk0xA5 = pGame->unk0x6C;
    pGameInfo->unk0xA6 = pGame->unk0x6E;

    SStrCopy(pGameInfo->szGamePassword, pGame->szGamePassword, 16u);
    SStrCopy(pGameInfo->szGameDescription, pGame->szGameDesc, 32u);

    pGameInfo->nArenaFlags = ARENA_GetFlags(pGame);
    pGameInfo->pMemoryPool = pGame->pMemoryPool;

    D2_UNLOCK(pGame->lpCriticalSection);
    return 1;
}

//D2Game.0x6FC3A390 (#10015)
int32_t __stdcall GAME_GetGameServerTokens(uint16_t* pServerToken, int32_t nMaxCount)
{
    int32_t nServerTokenCount = 0;
    for (int32_t i = 0; nServerTokenCount < nMaxCount && i < std::size(gnGamesGUIDs_6FD447F8); ++i)
    {
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1)
        {
            if (D2GameStrc* pGame = GAME_LockGame(gnGamesGUIDs_6FD447F8[i]))
            {
                pServerToken[nServerTokenCount] = pGame->nServerToken;
                ++nServerTokenCount;

                D2_UNLOCK(pGame->lpCriticalSection);
            }
        }
    }

    return nServerTokenCount;
}

//D2Game.0x6FC3A490 (#10016)
int32_t __stdcall D2Game_10016(uint16_t nGameId)
{
    EnterCriticalSection(&gCriticalSection_6FD45800);

    int32_t nGUID = 0;
    if (hGameArray_6FD447F8[nGameId - 1] && hGameArray_6FD447F8[nGameId - 1] != D2GameReservedSlotHandle)
    {
        nGUID = GetHashValueFromGameHandle(hGameArray_6FD447F8[nGameId - 1]);
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    int32_t nUnits = 0;
    if (D2GameStrc* pGame = GAME_LockGame(nGUID))
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
        
        D2_UNLOCK(pGame->lpCriticalSection);
    }

    return nUnits;
}

//D2Game.0x6FC3A5A0 (#10017)
int32_t __stdcall D2Game_10017(uint16_t nGameId, D2UnitInfoStrc* pUnitInfo, int32_t nMaxCount)
{
    const int32_t nGUID = sub_6FC35840(nGameId);

    int32_t nUnitInfoCount = 0;
    if (D2GameStrc* pGame = GAME_LockGame(nGUID))
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

        D2_UNLOCK(pGame->lpCriticalSection);
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
    // NOLINTBEGIN(clang-diagnostic-deprecated-declarations)
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
    // NOLINTEND(clang-diagnostic-deprecated-declarations)
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
        strcpy(szDescription, pObjectsTxtRecord->szName); // NOLINT(clang-diagnostic-deprecated-declarations)
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

//D2Game.0x6FC3A8B0 (#10019)
// Note: used by server only to retrieve unit lists
void __stdcall GAME_GetUnitsDescriptions(uint16_t nGameId, D2UnitDescriptionListStrc* pUnitDescriptionsList, uint32_t eType)
{
    const int32_t nGUID = sub_6FC35840(nGameId);
    if (D2GameStrc* pGame = GAME_LockGame(nGUID))
    {
        // Note: eType is not D2C_UnitTypes
        D2UnitStrc** pUnitList = eType <= 4 ? pGame->pUnitList[eType] : nullptr;
        for (int nUnitIndex = 0; nUnitIndex < ARRAYSIZE(pGame->pUnitList[eType]); nUnitIndex++)
        {
            for(D2UnitStrc* pUnit = pUnitList[nUnitIndex];
                pUnit != nullptr;
                pUnit = SUNIT_GetNextUnitFromList(pUnit)
                )
            {
                D2_ASSERT(pUnitDescriptionsList);
                int32_t nUnitClassId = pUnit->dwClassId;
                D2UnitDescriptionListStrc* pCurrentNode = pUnitDescriptionsList;
                do
                {
                    if (pCurrentNode->nClassId == nUnitClassId)
                    {
                        pCurrentNode->nCount++;
                        break;
                    }
                    pCurrentNode = pCurrentNode->pNext;
                } while (pCurrentNode);

                if (pCurrentNode == nullptr) // Unit class id not in list yet
                {
                    if (D2UnitDescriptionListStrc* pNewNode = D2_CALLOC_STRC_POOL(nullptr, D2UnitDescriptionListStrc))
                    {
                        pNewNode->nClassId = nUnitClassId;
                        pNewNode->nCount = 1;
                        pNewNode->dwUnk0x08 = 0;
                        pNewNode->pNext = pUnitDescriptionsList->pNext;
                        pUnitDescriptionsList->pNext = pNewNode;
                    }
                }
            }
        }
        
        D2_UNLOCK(pGame->lpCriticalSection);

        D2_ASSERT(gpfGetDescription_6FD2CA64[eType]);

        for (D2UnitDescriptionListStrc* pCurrentUnitInfo = pUnitDescriptionsList->pNext; 
            pCurrentUnitInfo != nullptr; 
            pCurrentUnitInfo = pCurrentUnitInfo->pNext)
        {
            gpfGetDescription_6FD2CA64[eType](pCurrentUnitInfo->szDescription, pCurrentUnitInfo->nClassId);
        }
    }
}

//D2Game.0x6FC3AB20 (#10018)
int32_t __stdcall D2Game_10018(uint16_t nGameId, int32_t nMaxCount, D2UnitInfoStrc* pUnitInfo, int32_t* pUnitType, int32_t* pUnitGUID)
{
    const int32_t nGUID = sub_6FC35840(nGameId);

    int32_t nUnitInfoCount = 0;
    if (D2GameStrc* pGame = GAME_LockGame(nGUID))
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

                    gpfGetDescription_6FD2CA64[pUnitType[i]](pUnitInfo[nUnitInfoCount].szDescription, pUnit->dwClassId);
                }

                ++nUnitInfoCount;
            }
        }

        D2_UNLOCK(pGame->lpCriticalSection);
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
        if (gnGamesGUIDs_6FD447F8[i] && gnGamesGUIDs_6FD447F8[i] != -1)
        {
            if (D2GameStrc* pGame = GAME_LockGame(gnGamesGUIDs_6FD447F8[i]))
            {
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
                
                D2_UNLOCK(pGame->lpCriticalSection);

                ++pStats->nGamesCount;
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
            D2GameStrc* pGame = GAME_LockGame(gnGamesGUIDs_6FD447F8[i]);
            D2_ASSERT(pGame);
            if (a1 == 1)
            {
                D2GSPacketSrv5A packet5A = {};
                packet5A.nHeader = 0x5Au;
                packet5A.nType = EVENTTYPE_REALMGOINGDOWN;
                packet5A.nColor = STRCOLOR_DARK_GOLD;
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
                    strncpy(szCopy, szMessage, 255); // NOLINT(clang-diagnostic-deprecated-declarations)
                    szCopy[255] = '\0';
                    sub_6FC84CD0(pGame, szCopy, 1u);
                }
            }

            D2_UNLOCK(pGame->lpCriticalSection);
        }
    }
}

//D2Game.0x6FC3AFB0 (#10022)
void __stdcall D2Game_10022(uint16_t nGameId, char* a2)
{
    EnterCriticalSection(&gCriticalSection_6FD45800);

    int32_t nGUID = 0;
    if (hGameArray_6FD447F8[nGameId - 1] && hGameArray_6FD447F8[nGameId - 1] != D2GameReservedSlotHandle)
    {
        nGUID = GetHashValueFromGameHandle(hGameArray_6FD447F8[nGameId - 1]);
    }

    LeaveCriticalSection(&gCriticalSection_6FD45800);

    D2GameStrc* pGame = GAME_LockGame(nGUID);
    D2_ASSERT(pGame);

    a2[255] = 0;
    if (!strncmp("<ABEND> ", a2, 8u))
    {
        pGame->unk0x1DDC = a2[8] & 0xF;
    }

    sub_6FC84CD0(pGame, a2, 1u);

    D2_UNLOCK(pGame->lpCriticalSection);
}

//D2Game.0x6FC3B0E0
void __fastcall GAME_ForEachIngameClient(D2GameStrc* pGame, void(__fastcall* pFn)(D2ClientStrc*, void*), void* pContext)
{
    D2_ASSERT(pGame);

    if (IsBadCodePtr((FARPROC)pFn))
    {
        FOG_DisplayAssert("pfn", __FILE__, __LINE__);
        exit(-1);
    }

    for (D2ClientStrc* pClient = pGame->pClientList; pClient; pClient = pClient->pNext)
    {
        if (pClient->dwClientState == CLIENTSTATE_INGAME)
        {
            pFn(pClient, pContext);
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

    gpGame_6FD457FC = GAME_LockGame(nGUID);
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
    gnTargetFrameRate_6FD2CA60 = a1;
    gnTargetMsPerFrame_6FD457F8 = 1000 / a1;
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
        if (pClient->dwClientState == CLIENTSTATE_INGAME)
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

    D2ActiveRoomStrc* pUnitRoom = UNITS_GetRoom(pUnit);
    D2ActiveRoomStrc* pClientRoom = pClient->pRoom;
    D2_ASSERT(pClientRoom);

    D2ActiveRoomStrc** ppRoomList = nullptr;
    int32_t nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pClientRoom, &ppRoomList, &nNumRooms);

    for (int32_t i = 0; i < nNumRooms; ++i)
    {
        D2ActiveRoomStrc* pRoom = ppRoomList[i];
        D2_ASSERT(pRoom);

        if (pUnitRoom == pRoom)
        {
            D2GAME_PACKETS_SendPacket0x0A_RemoveObject_6FC3D3A0(pClient, 0xAu, pUnit->dwUnitType, pUnit->dwUnitId);
            return;
        }
    }
}

//D2Game.0x6FC3B480
// Should be __thiscall, nUnused is present due to using __fastcall 
D2GameStrc* __fastcall D2GameDataTable_Lock(D2GameDataTableStrc* pGameDataTable, int32_t nUnused, HGAMEDATA hGame, GAMEDATALOCKEDHANDLE* pLockedHandle, int32_t forWriting)
{
    return pGameDataTable->tHashTable.Lock(hGame, pLockedHandle, forWriting);
}

//D2Game.0x6FC3B510
// See TSExportTableSyncReuse::SyncEnterLock
// Should be __thiscall, nUnused is present due to using __fastcall 
void __fastcall D2GameDataTable_SyncEnterLock(D2GameDataTableStrc* pGameDataTable, int32_t nUnused, int32_t* pLockHandle, int32_t bForWriting)
{
    D2_MAYBE_UNUSED(nUnused);
    pGameDataTable->tHashTable.GetSync().Enter(bForWriting);
    *pLockHandle = 2 * (bForWriting != 0) - 1;
}

//D2Game.0x6FC3B540
// Should be __thiscall, nUnused is present due to using __fastcall 
// See TSExportTableSyncReuse::SyncLeaveLock
void __fastcall D2GameDataTable_SyncLeaveLock(D2GameDataTableStrc* pGameDataTable, int32_t nUnused, int32_t tLockHandle)
{
    D2_MAYBE_UNUSED(nUnused);
    if (tLockHandle != 0)
    {
        pGameDataTable->tHashTable.GetSync().Leave(tLockHandle == 1);
    }
}

//D2Game.0x6FC3B560
// Should be __thiscall, nUnused is present due to using __fastcall 
TSLink<D2GameStrc>* __fastcall D2GameDataTable_TLink_NextLink(TSLink<D2GameStrc>* pLink, int32_t nUnused, int nLinkOffset)
{
    return pLink->NextLink(nLinkOffset);
}

//D2Game.0x6FC3B590
// Should be __thiscall, nUnused is present due to using __fastcall 
D2GameStrc* __fastcall D2GameDataTable_New(D2GameDataTableStrc* pGameDataTable, int32_t nUnused, D2GameGUID nGameGUID, HASHKEY_NONE* pKey, int32_t extrabytes, int32_t flags)
{
    return static_cast<TSHashTable<D2GameStrc, HASHKEY_NONE>&>(pGameDataTable->tHashTable).New((uint32_t)nGameGUID, *pKey, extrabytes, flags);
}

//D2Game.0x6FC3B6A0
// Should be __thiscall, nUnused is present due to using __fastcall 
D2GameStrc* __fastcall D2GameDataTable_Ptr(D2GameDataTableStrc* pGameDataTable, int32_t nUnused, D2GameGUID nGameGUID, const HASHKEY_NONE* pHashKey)
{
    return static_cast<TSHashTable<D2GameStrc,HASHKEY_NONE>&>(pGameDataTable->tHashTable).Ptr((uint32_t)nGameGUID, *pHashKey);
}

//D2Game.0x6FC3B6F0
// Should be __thiscall, nUnused is present due to using __fastcall 
TSLink<D2GameStrc>* __fastcall D2GameDataTable_TSExplicitList_Link(STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)* pNode, int32_t nUnused, D2GameStrc* ptr)
{
    return pNode->Link(ptr);
}

//D2Game.0x6FC3B710
//void __thiscall TSHashTable_SGAMEDATA::Grow_6FC3B710(D2GameDataTableStrc pGameDataTable)

//D2Game.0x6FC3B8E0
// Should be __thiscall
void __fastcall D2GameDataTable_TSExplicitList_UnlinkAll_AndUninitTerminator(STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)* pList)
{
    pList->UnlinkAll();
    pList->m_terminator.Unlink();
}

//D2Game.0x6FC3B960
// Should be __thiscall
void __fastcall D2GameDataTable_TSLink_Unlink(TSLink<D2GameStrc>* pLink)
{
    pLink->Unlink();
}

//D2Game.0x6FC3B9A0
// Should be __thiscall, nUnused is present due to using __fastcall 
void __fastcall D2GameDataTable_GrowableArray_TSExplicitList_SetCount(TSGrowableArray<STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)>* pArray, int32_t nUnused, uint32_t nCount)
{
    D2_MAYBE_UNUSED(nUnused);
    //TODO: Fix squall impl
    D2_ASSERTM(nCount >= pArray->Count(), "Squall's implementation is buggy, and won't work if new size is smaller than old one. (should destroy objects after nCount)");
    pArray->SetCount(nCount);
}
 
//D2Game.0x6FC3BB10
// Should be __thiscall, nUnused is present due to using __fastcall 
void __fastcall D2GameDataTable_TSExplicitList_Destroy(STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)* pNode, int32_t nUnused, char bShouldFree)
{
    D2GameDataTable_TSExplicitList_UnlinkAll_AndUninitTerminator(pNode);
    if ((bShouldFree & 1) != 0 && pNode)
    {
        SMemFree(pNode, "delete", -1, 0);
    }
}

//D2Game.0x6FC3BB40
// Should be __thiscall
void __fastcall D2GameDataTable_TSExplicitList_InitializeTerminator(STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)* pList)
{
    pList->InitializeTerminator();
}

////D2Game.0x6FC3BB50
// Should be __thiscall
void __fastcall D2GameDataTable_TSExplicitList_InplaceNew(void* pMemory)
{
    if (TSExplicitList<D2GameStrc, 0xDDDDDDDD>* pNode = (TSExplicitList<D2GameStrc, 0xDDDDDDDD>*)pMemory)
    {
        // TODO: squall should be using #define LISTEXDYN(structname) TSExplicitList< structname ,(int)0xDDDDDDDD> but is not.
        // Investigate why and fix it.
        pNode->m_terminator.m_prevlink = 0;
        pNode->m_terminator.m_next = 0;
        pNode->m_terminator.m_prevlink = &pNode->m_terminator;
        pNode->m_linkoffset = 0xDDDDDDDD;
        pNode->m_terminator.m_next = (struct D2GameStrc*)~(unsigned int)&pNode->m_terminator;
    }
}

//D2Game.0x6FC3BB80
// Should be __thiscall, nUnused is present due to using __fastcall 
void __fastcall D2GameDataTable_TSExplicitList_InplaceNew_WithList(void* pMemory, int32_t nUnused, TSExplicitList<D2GameStrc, 0xDDDDDDDD>* pList)
{
    if (TSExplicitList<D2GameStrc, 0xDDDDDDDD>*pNode = (TSExplicitList<D2GameStrc, 0xDDDDDDDD>*)pMemory)
    {
        // TODO: squall should be using #define LISTEXDYN(structname) TSExplicitList< structname ,(int)0xDDDDDDDD> but is not.
        // Investigate why and fix it.
        pNode->m_terminator.m_prevlink = 0;
        pNode->m_terminator.m_next = 0;
        pNode->m_terminator.m_prevlink = &pNode->m_terminator;
        pNode->m_linkoffset = pList->m_linkoffset;
        pNode->m_terminator.m_next = (struct D2GameStrc*)~(unsigned int)&pNode->m_terminator;
    }
}
