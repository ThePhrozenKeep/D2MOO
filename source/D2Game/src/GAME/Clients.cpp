#include "GAME/Clients.h"

#include <algorithm>

#include <Fog.h>
#include <Storm.h>

#include <D2Dungeon.h>
#include <D2Skills.h>
#include <D2StatList.h>
#include <Units/UnitRoom.h>
#include <DataTbls/SkillsIds.h>

#include "GAME/Arena.h"
#include "GAME/Game.h"
#include "GAME/Level.h"
#include "GAME/SCmd.h"
#include "INVENTORY/InvMode.h"
#include "ITEMS/ItemMode.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlrModes.h"
#include "PLAYER/PlrMsg.h"
#include "PLAYER/PlrSave.h"
#include "PLAYER/PlrTrade.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT5/A5Q3.h"
#include "SKILLS/Skills.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitNpc.h"
#include "UNIT/SUnitProxy.h"


#pragma warning(disable: 28159)


//1.10f: D2Game.0x6FD447D0
//1.14d: 0x008846A8
CRITICAL_SECTION gClientListLock_6FD447D0;
//1.10f: D2Game.0x6FD443B8
//1.14d: 0x008846C0
CRITICAL_SECTION gSrvClientListByNameLock_6FD443B8;
//1.10f: 0x6FD447E8
//1.13c: 0x6FD31C18
//1.14d: 0x008846D8
BOOL gbClientListInitialized_6FD447E8;
//1.10f: D2Game.0x6FD43FB8
//1.13c: D2Game.0x6FD307B8
//1.14d: 0x008842A8
D2ClientStrc* gpClientList_6FD43FB8[256];
//1.10f: D2Game.0x6FD443D0
//1.14d: 0x00883EA8
D2ClientStrc* gpClientListByName_6FD443D0[256];


//D2Game.0x6FC31CD0
//1.14d: 0x005376C0
void __fastcall CLIENTS_Initialize()
{
    InitializeCriticalSection(&gClientListLock_6FD447D0);

    EnterCriticalSection(&gClientListLock_6FD447D0);
    memset(gpClientList_6FD43FB8, 0, sizeof(gpClientList_6FD43FB8));

    LeaveCriticalSection(&gClientListLock_6FD447D0);

    InitializeCriticalSection(&gSrvClientListByNameLock_6FD443B8);

    EnterCriticalSection(&gSrvClientListByNameLock_6FD443B8);
    memset(gpClientListByName_6FD443D0, 0, sizeof(gpClientListByName_6FD443D0));

    LeaveCriticalSection(&gSrvClientListByNameLock_6FD443B8);

    gbClientListInitialized_6FD447E8 = TRUE;
}

//D2Game.0x6FC31D50
//1.14d: 0x00537730
void __fastcall CLIENTS_Release()
{
    DeleteCriticalSection(&gClientListLock_6FD447D0);
    DeleteCriticalSection(&gSrvClientListByNameLock_6FD443B8);

    gbClientListInitialized_6FD447E8 = FALSE;
}

//D2Game.0x6FC31D80 (#10058)
//1.14d: 0x005377B0
int32_t __stdcall CLIENTS_GetExpansionClientCount()
{
    int32_t result = 0;
    if (!gbClientListInitialized_6FD447E8)
    {
        return 0;
    }

    EnterCriticalSection(&gClientListLock_6FD447D0);

    for (int32_t i = 0; i < 256; ++i)
    {
        for (D2ClientStrc* pClient = gpClientList_6FD43FB8[i]; pClient; pClient = pClient->pServerNext)
        {
            if (pClient->tSaveFlags.bExpansion)
            {
                ++result;
            }
        }
    }

    LeaveCriticalSection(&gClientListLock_6FD447D0);

    return result;
}

// Helper Function
// Returns the removed client
typedef D2ClientStrc* D2ClientStrc::* D2ClientListPtr;
static D2ClientStrc* __fastcall CLIENTS_RemoveClientFromListWithId(D2ClientStrc** ppClientListHead, int32_t nClientIdToRemove, D2ClientListPtr pListMemberPtr)
{
    if ((*ppClientListHead)->dwClientId == nClientIdToRemove)
    {
        D2ClientStrc* pClientToRemove = *ppClientListHead;
		*ppClientListHead = pClientToRemove->*pListMemberPtr;
        return pClientToRemove;
    }
    else
    {
        D2ClientStrc* pPreviousClient = (*ppClientListHead);
        for (D2ClientStrc* pCurrentClient = pPreviousClient->*pListMemberPtr; ; pCurrentClient = pCurrentClient->pServerNext)
        {
            if (pCurrentClient->dwClientId == nClientIdToRemove)
            {
                // Unlink client
                pPreviousClient->*pListMemberPtr = pCurrentClient->*pListMemberPtr;
                return pCurrentClient;
            }
            pPreviousClient = pCurrentClient;
            D2_ASSERT(pCurrentClient->*pListMemberPtr != nullptr);
        }
        D2_UNREACHABLE;
    }
}

// Returns the removed client
static D2ClientStrc* __fastcall CLIENTS_RemoveClientFromListByName(D2ClientStrc** ppClientListHead, const char* szClientToRemoveName)
{    
    if (0 == SStrCmpI((*ppClientListHead)->szName, szClientToRemoveName, 16))
    {
        D2ClientStrc* pClientToRemove = *ppClientListHead;
        *ppClientListHead = pClientToRemove->pServerNextByName;
        return pClientToRemove;
    }
    else
    {
        D2ClientStrc* pPreviousClient = (*ppClientListHead);
        for (D2ClientStrc* pCurrentClient = pPreviousClient->pServerNextByName; pCurrentClient != nullptr; pCurrentClient = pCurrentClient->pServerNextByName)
        {
            if (0 == SStrCmpI(pCurrentClient->szName, szClientToRemoveName, 16))
            {
                // Unlink client
                pPreviousClient->pServerNextByName = pCurrentClient->pServerNextByName;
                return pCurrentClient;
            }
            pPreviousClient = pCurrentClient;
        }
        return nullptr;
    }
}

static D2ClientStrc* __fastcall CLIENTS_GetClientFromClientListWithId(D2ClientStrc* pClientList, int32_t nClientId)
{
    for (D2ClientStrc* pClient = pClientList; pClient != nullptr; pClient = pClient->pNext)
    {
        if (pClient->dwClientId == nClientId)
        {
            return pClient;
        }
    }

    return nullptr;
}

//D2Game.0x6FC31DE0
//1.14d: 0x00537810
D2ClientStrc* __fastcall CLIENTS_GetClientFromClientId(D2GameStrc* pGame, int32_t nClientId)
{
    D2_ASSERT(pGame);
    return CLIENTS_GetClientFromClientListWithId(pGame->pClientList, nClientId);
}

//D2Game.0x6FC31E20
//1.14d: 0x00537860
D2UnitStrc* __stdcall CLIENTS_GetPlayerFromClient(D2ClientStrc* pClient, BOOL bIgnoreDeath)
{
    D2_ASSERT(pClient);

    if ((pClient->dwFlags & CLIENTFLAGEX_PLAYER_UNIT_ALIVE) || bIgnoreDeath)
    {
        return pClient->pPlayer;
    }

    if (!pClient->pPlayer)
    {
        return nullptr;
    }

    D2UnitStrc* pPlayer = SUNIT_GetServerUnit(pClient->pGame, pClient->dwUnitType, pClient->dwUnitGUID);
    if (!pPlayer)
    {
        pClient->pPlayer = nullptr;
    }

    return pPlayer;
}

//D2Game.0x6FC31EA0
//1.14d: 0x005378E0
void __fastcall CLIENTS_SetPlayerInClient(D2ClientStrc* pClient, D2UnitStrc* pUnit)
{
    D2_ASSERT(pClient);

    if (pUnit)
    {
        pClient->dwUnitType = pUnit->dwUnitType;
        pClient->dwUnitGUID = pUnit->dwUnitId;
        pClient->pPlayer = pUnit;
    }
    else
    {
        pClient->pPlayer = nullptr;
    }
}

//D2Game.0x6FC31EF0
//1.14d: 0x005394A0
void __fastcall CLIENTS_SpawnPlayer(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2GameStrc* pGame, D2ActiveRoomStrc* pRoomArg, int32_t nXArg, int32_t nYArg)
{
    if (!pGame || !pPlayer || !pClient)
    {
        return;
    }

    D2ActiveRoomStrc* pRoom = pRoomArg;
    int32_t nX = 0;
    int32_t nY = 0;

    if (pRoom)
    {
        nX = nXArg;
        nY = nYArg;
    }
    else
    {
        pRoom = DUNGEON_FindActSpawnLocationEx(pGame->pAct[pClient->nAct], DUNGEON_GetTownLevelIdFromAct(pGame->pAct[pClient->nAct]), 0, &nX, &nY, UNITS_GetUnitSizeX(pPlayer));

        D2_ASSERT(pRoom);

        D2DrlgCoordsStrc drlgCoords = {};
        DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);
        D2GAME_PACKETS_SendPacket0x07_6FC3D120(pClient, DUNGEON_GetLevelIdFromRoom(pRoom), (uint16_t)drlgCoords.nTileXPos, (uint16_t)drlgCoords.nTileYPos);
    }

    if (pRoom != pClient->pRoom)
    {
        CLIENTS_ChangeRoom(pClient, pRoom);
    }

    pClient->pRoom = pRoom;
    SUNIT_Add(pPlayer, nX, nY, pGame, pRoom, 0);

    pClient->dwFlags &= (~CLIENTFLAGEX_PLAYER_UNIT_ALIVE);

    D2GAME_PACKETS_SendPacket0x15_6FC3D0D0(pClient, 0x15u, pPlayer->dwUnitType, pPlayer->dwUnitId, nX, nY, 1);
    D2GAME_PACKETS_SendPacket0x7E_6FC3F770(pClient);

    D2CorpseStrc* pCorpse = INVENTORY_GetFirstCorpse(pPlayer->pInventory);
    while (pCorpse)
    {
        const int32_t nCorpseUnitGUID = INVENTORY_GetUnitGUIDFromCorpse(pCorpse);
        if (D2Common_10316(pCorpse))
        {
            D2UnitStrc* pCorpsePlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nCorpseUnitGUID);
            if (pCorpsePlayer)
            {
                D2CoordStrc coords = {};
                D2ActiveRoomStrc* pSpawnLocation = DUNGEON_FindActSpawnLocationEx(pGame->pAct[pClient->nAct], DUNGEON_GetTownLevelIdFromActNo(pClient->nAct), 0, &coords.nX, &coords.nY, UNITS_GetUnitSizeX(pCorpsePlayer));
                PLAYER_PlaceCorpse(pGame, pSpawnLocation, nCorpseUnitGUID, &coords);
            }

            pCorpse = INVENTORY_GetNextCorpse(pCorpse);
        }
        else
        {
            const uint16_t wName = D2Common_10315(pCorpse);
            pCorpse = INVENTORY_GetNextCorpse(pCorpse);

            INVENTORY_FreeCorpse(pPlayer->pInventory, nCorpseUnitGUID, 0);

            if (nCorpseUnitGUID && wName)
            {
                D2GAME_MERCS_Create_6FCC8630(pGame, pPlayer, wName, nCorpseUnitGUID, 0xFFFFu, 0, 0);
            }
        }
    }

    SUNITNPC_UpdateHirelingPosition(pGame, pPlayer);

    const int32_t nIronGolemItemGUID = pClient->nIronGolemItemGUID;
    if (nIronGolemItemGUID && nIronGolemItemGUID != -1)
    {
        D2UnitStrc* pIronGolemItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, pClient->nIronGolemItemGUID);
        if (pIronGolemItem)
        {
            D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pPlayer, SKILL_IRONGOLEM);
            if (pSkill)
            {
                UNITS_SetXForStaticUnit(pIronGolemItem, CLIENTS_GetUnitX(pPlayer));
                UNITS_SetYForStaticUnit(pIronGolemItem, CLIENTS_GetUnitY(pPlayer));
                UNITS_SetTargetUnitForDynamicUnit(pPlayer, pIronGolemItem);

                SKILLS_ExecuteSkill(pGame, pPlayer, SKILL_IRONGOLEM, SKILLS_GetSkillLevel(pPlayer, pSkill, 1), 1, 0, 0);
            }
        }

        pClient->nIronGolemItemGUID = -1;
    }
}

//D2Game.0x6FC32220
//1.14d: 0x00537970
void __fastcall CLIENTS_SetPlayerUnitAlive(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    pClient->dwFlags |= CLIENTFLAGEX_PLAYER_UNIT_ALIVE;
}

//D2Game.0x6FC32260
//1.14d: 0x00539760
int32_t __fastcall CLIENTS_AddPlayerToGame(D2ClientStrc* pClient, D2GameStrc* pGame, int32_t bLoadSave, D2ActiveRoomStrc* pRoomArg, int32_t nXArg, int32_t nYArg)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pClient);

    pClient->dwFlags |= CLIENTFLAGEX_PLAYER_UNIT_ALIVE;

    D2UnitStrc* ppUnit = nullptr;

    const int32_t nError = D2GAME_SAVE_GetUnitDataFromFile_6FC8CB40(pGame, pClient, pClient->szName, bLoadSave, &ppUnit, pRoomArg, nXArg, nYArg);
    if (nError)
    {
        FOG_TraceF(gszEmptyString_6FD447EC, "[PLAYER LOAD]  ClientAddPlayerToGame()  Error Loading:%s  Error:%d=%s", pClient->szName, nError, "nError");
        pClient->pPlayer = nullptr;
        return nError;
    }

    if (pClient->tSaveFlags.bExpansion)
    {
        if (!pGame->bExpansion)
        {
            FOG_Trace("[PLAYER LOAD]  ClientAddPlayerToGame()  Error Loading:%s  Error:SYSERROR_NOTEXPANSIONGAME\n", pClient->szName);
            pClient->pPlayer = nullptr;
            return SYSERROR_NOTEXPANSIONGAME;
        }
    }
    else
    {
        if (pGame->bExpansion)
        {
            FOG_Trace("[PLAYER LOAD]  ClientAddPlayerToGame()  Error Loading:%s  Error:SYSERROR_EXPANSIONGAME\n", pClient->szName);
            pClient->pPlayer = nullptr;
            return SYSERROR_EXPANSIONGAME;
        }
    }

    if (pClient->tSaveFlags.bLadder)
    {
        if (!pGame->dwGameType)
        {
            FOG_Trace("[PLAYER LOAD]  ClientAddPlayerToGame()  Error Loading:%s  Error:SYSERROR_NOTLADDERGAME\n", pClient->szName);
            pClient->pPlayer = nullptr;
            return SYSERROR_NOTLADDERGAME;
        }
    }
    else
    {
        if (pGame->dwGameType)
        {
            FOG_Trace("[PLAYER LOAD]  ClientAddPlayerToGame()  Error Loading:%s  Error:SYSERROR_LADDERGAME\n", pClient->szName);
            pClient->pPlayer = 0;
            return SYSERROR_LADDERGAME;
        }
    }
    if (pClient->tSaveFlags.bHardcore)
    {
        if (pClient->tSaveFlags.bDead)
        {
            FOG_TraceF(gszEmptyString_6FD447EC, "[PLAYER LOAD]  ClientAddPlayerToGame()  Error Loading:%s  Error:SYSERROR_DEADHARDCORE", pClient->szName);
            pClient->pPlayer = nullptr;
            return SYSERROR_DEADHARDCORE;
        }

        if (!(ARENA_GetFlags(pGame) & GAMEFLAG_ARENA_HARDCORE))
        {
            FOG_TraceF(gszEmptyString_6FD447EC, "[PLAYER LOAD]  ClientAddPlayerToGame()  Error Loading:%s  Error:SYSERROR_HARDCOREJOINSOFTCORE", pClient->szName);
            pClient->pPlayer = nullptr;
            return SYSERROR_HARDCOREJOINSOFTCORE;
        }
    }
    else
    {
        if (ARENA_GetFlags(pGame) & GAMEFLAG_ARENA_HARDCORE)
        {
            FOG_TraceF(gszEmptyString_6FD447EC, "[PLAYER LOAD]  ClientAddPlayerToGame()  Error Loading:%s  Error:SYSERROR_SOFTCOREJOINHARDCORE", pClient->szName);
            pClient->pPlayer = nullptr;
            return SYSERROR_SOFTCOREJOINHARDCORE;
        }
    }

    int32_t nUnitType = 6;
    int32_t nUnitGUID = -1;

    if (ppUnit)
    {
        nUnitType = ppUnit->dwUnitType;
        nUnitGUID = ppUnit->dwUnitId;
    }

    D2GAME_PACKETS_SendPacketSize06_6FC3C850(pClient, 0x0B, nUnitType, nUnitGUID);

    D2GAME_SendPacketSize05_6FC3C880(pClient, 0x5Fu, UNITS_GetPlayerPortalFlags(ppUnit));
    D2Common_10513(ppUnit, ppUnit, (void(__fastcall*)(D2UnitStrc*, int32_t, int32_t, D2UnitStrc*))D2GAME_UpdateAttribute_6FC822D0);

    if (ppUnit && ppUnit->dwFlagEx & UNITFLAGEX_HASINV)
    {
        D2GAME_INVMODE_First_6FC40FB0(pGame, ppUnit, pClient, 0);
        if (ppUnit)
        {
            ppUnit->dwFlagEx |= 0x200000u;
        }
    }

    for (int32_t i = 0; i < 16; ++i)
    {
        if (pClient->HotkeySkills[i].nSkill >= 0 && pClient->HotkeySkills[i].nSkill < sgptDataTables->nSkillsTxtRecordCount)
        {
            D2GAME_PACKETS_SendPacket0x7B_6FC3F720(pClient, i, pClient->HotkeySkills[i].nSkill, (uint8_t)pClient->HotkeySkills[i].nHand, pClient->HotkeySkills[i].nItemGUID);
        }
    }

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(ppUnit);
    if (pPlayerData)
    {
        D2GAME_PACKETS_SendPacket0x23_6FC3DC60(pClient, ppUnit->dwUnitType, ppUnit->dwUnitId, 1u, (uint16_t)pPlayerData->nLeftSkillId, pPlayerData->nLeftSkillFlags);
        D2GAME_PACKETS_SendPacket0x23_6FC3DC60(pClient, ppUnit->dwUnitType, ppUnit->dwUnitId, 0, (uint16_t)pPlayerData->nRightSkillId, pPlayerData->nRightSkillFlags);
    }

    D2Common_10513(ppUnit, ppUnit, (void(__fastcall*)(D2UnitStrc*, int32_t, int32_t, D2UnitStrc*))D2GAME_UpdateAttribute_6FC822D0);
    PLRMSG_SendPlayerStatUpdates(ppUnit, ppUnit, 1);
    D2GAME_INVMODE_Last_6FC416D0(pGame, ppUnit);

    if (pGame->bExpansion)
    {
        ACT5Q3_ApplyResistanceReward(pGame, ppUnit);
    }

    return 0;
}

//1.10f: D2Game.0x6FC325E0
//1.13c: D2Game.0x6FC6A9B0
//1.14d: 0x00539A30
D2ClientStrc* __fastcall CLIENTS_AddToGame(D2GameStrc* pGame, int32_t nClientId, uint8_t nClassIdOrCharTemplate, const char* szClientName, const char* szAccount, int32_t nCharSaveTransactionToken, uint32_t nLocale, int32_t nUnused1, int32_t nUnused2)
{
    if (!gbClientListInitialized_6FD447E8 || !pGame)
    {
        return 0;
    }

    if (pGame->nClients >= 8)
    {
        GAME_LogMessage(6, "[CLIENT]  ClientAddToGame:       Couldn't add client %d '%s' to full game %d '%s'", nClientId, szClientName, pGame->nGameId, pGame->szGameName);
        return 0;
    }

    D2ClientStrc* pClient = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2ClientStrc);

    pClient->dwClientId = nClientId;
    pClient->dwLastPacketTick = GetTickCount() + 180000;
    pClient->pGame = pGame;

    if (ARENA_ShouldTreatClassIdAsTemplateId(pGame))
    {
        pClient->nCharTemplate = nClassIdOrCharTemplate;
        pClient->nClassId = DATATBLS_GetClassFromCharTemplateTxtRecord(nClassIdOrCharTemplate, ARENA_GetTemplateType(pGame));
    }
    else
    {
        pClient->nClassId = nClassIdOrCharTemplate;
    }

    SStrCopy(pClient->szName, szClientName, INT_MAX);
    SStrCopy(pClient->szAccount, szAccount, INT_MAX);

    pClient->nCharSaveTransactionToken = nCharSaveTransactionToken;
    pClient->pClientInfo = 0;
    pClient->dwLocale = nLocale;
    pClient->dwClientState = CLIENTSTATE_JUST_CREATED;
    pClient->tPacketDataList.pHead = nullptr;
    pClient->tPacketDataList.pTail = nullptr;
    pClient->tPacketDataList.pPacketDataPool = nullptr;

    for (int32_t i = 0; i < 16; ++i)
    {
        pClient->HotkeySkills[i].nSkill = -1;
        pClient->HotkeySkills[i].nHand = 0;
        pClient->HotkeySkills[i].nItemGUID = -1;
    }

    pClient->pNext = pGame->pClientList;
    pGame->pClientList = pClient;

    EnterCriticalSection(&gClientListLock_6FD447D0);
    pClient->pServerNext = gpClientList_6FD43FB8[(uint8_t)nClientId];
    gpClientList_6FD43FB8[(uint8_t)nClientId] = pClient;
    LeaveCriticalSection(&gClientListLock_6FD447D0);

    const uint8_t nIndex = (uint8_t)FOG_ComputeStringCRC16(szClientName);

    EnterCriticalSection(&gSrvClientListByNameLock_6FD443B8);
    pClient->pServerNextByName = gpClientListByName_6FD443D0[nIndex];
    gpClientListByName_6FD443D0[nIndex] = pClient;
    LeaveCriticalSection(&gSrvClientListByNameLock_6FD443B8);

    ++pGame->nClients;

    GAME_LogMessage(6, "[CLIENT]  ClientAddToGame:       Added client %d '%s' to game %d '%s'", nClientId, szClientName, pGame->nGameId, pGame->szGameName);

    return pClient;
}

//D2Game.0x6FC327E0
//1.14d: 0x005379A0
void __fastcall CLIENTS_SetGameData(D2GameStrc* pGame)
{
    pGame->nClients = 0;
    pGame->pClientList = nullptr;

    if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfCreateClientInfo)
    {
        pGame->pClientInfo = gpD2EventCallbackTable_6FD45830->pfCreateClientInfo();
    }
}

//D2Game.0x6FC32810
//1.14d: 0x00539BE0
void __fastcall CLIENTS_FillCharacterPreviewInfo(D2ClientStrc* pClient, D2CharacterPreviewInfoStrc* pCharacterPreviewInfo)
{
    FOG_Encode14BitsToString(&pCharacterPreviewInfo->nVersion, 10);
    D2_ASSERT(pClient);

    if (D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, FALSE))
    {
        uint8_t aComponents[NUM_COMPONENTS];
        uint8_t aComponentsColors[NUM_COMPONENTS];
        memset(aComponents, 0xFF, sizeof(aComponents));
        INVENTORY_GetItemSaveGfxInfo(pPlayer, aComponents, aComponentsColors);
        
        for (int32_t i = 0; i < 16; ++i)
        {
            if (!aComponentsColors[i])
            {
                aComponentsColors[i] = 0xFF;
            }
        }
        // Note: the arrays are of size 11, not 16 (NUM_COMPONENTS).
        memcpy(pCharacterPreviewInfo->pComponents, aComponents, sizeof(pCharacterPreviewInfo->pComponents));
        memcpy(pCharacterPreviewInfo->pComponentColors, aComponentsColors, sizeof(pCharacterPreviewInfo->pComponentColors));

        const int32_t nPlayerClassId = pPlayer->dwClassId;
        if (nPlayerClassId < NUMBER_OF_PLAYERCLASSES)
        {
            pCharacterPreviewInfo->nClass = nPlayerClassId + 1;
        }
        else
        {
            pCharacterPreviewInfo->nClass = 1;
        }
        
        const uint32_t nPlayerLevel = STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0);

        if (nPlayerLevel != 0 && nPlayerLevel <= 99)
        {
            pCharacterPreviewInfo->nLevel = nPlayerLevel;
        }
        else
        {
            pCharacterPreviewInfo->nLevel = 1;
        }

        const uint32_t nPlayerMode = pPlayer->dwAnimMode;
        
		D2PackedClientSaveFlags tClientFlags = pClient->tSaveFlags;
        if ((nPlayerMode == PLRMODE_DEAD || nPlayerMode == PLRMODE_DEATH) && pClient->tSaveFlags.bHardcore)
        {
			tClientFlags.bDead = true;
        }
        else
        {
			tClientFlags.bDead = false;
        }

        FOG_Encode14BitsToString(&pCharacterPreviewInfo->nClientFlags, tClientFlags.nPackedValue);
        FOG_Encode14BitsToString(&pCharacterPreviewInfo->nGuildFlags, pClient->tGuildInfo.nGuildFlags);

        pCharacterPreviewInfo->nGuildEmblemBgColor = pClient->tGuildInfo.nBackgroundColor ? pClient->tGuildInfo.nBackgroundColor : 0xFFu;
        pCharacterPreviewInfo->nGuildEmblemFgColor = pClient->tGuildInfo.nForegroundColor ? pClient->tGuildInfo.nForegroundColor : 0xFFu;
        pCharacterPreviewInfo->nGuildEmblemType    = pClient->tGuildInfo.nEmblemType      ? pClient->tGuildInfo.nEmblemType      : 0xFFu;
        
        pCharacterPreviewInfo->szGuildTag = pClient->tGuildInfo.szGuildTag;

        if (!nPlayerLevel || nPlayerLevel > 99u
            || nPlayerClassId >= NUMBER_OF_PLAYERCLASSES
            || (tClientFlags.bInit || tClientFlags.bUnkFlag0x02 || tClientFlags.bUnkFlag0x10)
            || SStrLen((const char*)pCharacterPreviewInfo) != 33) // If any member of D2SaveLaunchStrc is 0. Looks like a debug check that is not required since we only fill with non-zero values.
        {
            // If any issue, we return an empty string
            *((char*)&pCharacterPreviewInfo->nVersion) = '\0';
        }
    }
}

//1.00 : D2Game.0x10002150
//1.10f: D2Game.0x6FC32A30
//1.14d: 0x00539DA0
void __fastcall CLIENTS_RemoveClientFromGame(D2GameStrc* pGame, int32_t nClientIdToRemove, BOOL bTriggerSave)
{
    D2_ASSERT(pGame);

    if (D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientIdToRemove))
    {
        CLIENTS_FillCharacterPreviewInfo(pClient, &pClient->tCharacterInfo);
        if (bTriggerSave)
        {
            GAME_TriggerClientSave(pClient, pGame);
        }
    }

    char szName[16] = { '\0' };
    if (gbClientListInitialized_6FD447E8)
    {
        D2_LOCK(&gClientListLock_6FD447D0);
        D2ClientStrc* pClientToRemove = CLIENTS_RemoveClientFromListWithId(&gpClientList_6FD43FB8[nClientIdToRemove & 0xFF], nClientIdToRemove, &D2ClientStrc::pServerNext);
        D2_ASSERT(pClientToRemove);
        strcpy(szName, pClientToRemove->szName); // NOLINT(clang-diagnostic-deprecated-declarations)
        D2_UNLOCK(&gClientListLock_6FD447D0);
    }

    D2ClientStrc* pClientToRemove = nullptr;
    if (pGame->pClientList)
    {
        pClientToRemove = CLIENTS_RemoveClientFromListWithId(&pGame->pClientList, nClientIdToRemove, &D2ClientStrc::pNext);
    }

    if (gbClientListInitialized_6FD447E8)
    {
        const uint8_t nClientNameCrc16LowByte = (uint8_t)FOG_ComputeStringCRC16(szName);
        D2_LOCK(&gSrvClientListByNameLock_6FD443B8);
        CLIENTS_RemoveClientFromListByName(&gpClientListByName_6FD443D0[nClientNameCrc16LowByte], szName);
        D2_UNLOCK(&gSrvClientListByNameLock_6FD443B8);
    }

    if (!pClientToRemove)
    {
        GAME_LogMessage(6, "[CLIENT]  ClientRemoveFromGame:  *** Can't find client %d to remove from game %d '%s' ***",
			nClientIdToRemove, pGame->nGameId, pGame->szGameName);
        return;
    }

    GAME_SendPacket0x5CToAllConnectedClients(pGame, pClientToRemove);

    if (D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClientToRemove, FALSE))
    {
        const uint64_t nPlayerExperience = STATLIST_GetUnitBaseStat(pPlayer, STAT_EXPERIENCE, 0);
        const int32_t nPlayerLevel = STATLIST_GetUnitBaseStat(pPlayer, STAT_LEVEL, 0);
        PARTY_LeaveParty(pGame, pPlayer);
        PLAYERPETS_FreePetsFromPlayerData(pGame, pPlayer);
        SUNITPROXY_OnClientRemovedFromGame(pGame, pPlayer);
        QUESTS_PlayerDroppedWithQuestItem(pGame, pPlayer);
        PLRTRADE_StopAllPlayerInteractions(pGame, pPlayer);

        D2ActiveRoomStrc* pPlayerRoom = UNITS_GetRoom(pPlayer);
        if (pPlayerRoom)
        {
            DUNGEON_AllocDrlgDelete(pPlayerRoom, pPlayer->dwUnitType, pPlayer->dwUnitId);
        }

        SUNIT_RemoveUnit(pClientToRemove->pGame, pPlayer);
        DUNGEON_ChangeClientRoom(pPlayerRoom, 0);
        D2Common_10077(pPlayerRoom, 0);

        if (gpD2EventCallbackTable_6FD45830)
        {
            if (D2_VERIFY(gpD2EventCallbackTable_6FD45830->pfLeaveGame))
            {
                gpD2EventCallbackTable_6FD45830->pfLeaveGame(
						&pClientToRemove->pClientInfo,
						pGame->nGameId,
						pPlayer->dwClassId, nPlayerLevel, nPlayerExperience,
						pClientToRemove->tSaveFlags.nPackedValue, pClientToRemove->szName,
						reinterpret_cast<const char*>(&pClientToRemove->tCharacterInfo), pClientToRemove->bUnlockCharacter,
						0,
						0,
						pClientToRemove->szAccount, pClientToRemove->nCharSaveTransactionToken D2_SERVER_CALLBACK_ARGS_114(0, 0, 0, 0, 0), &pClientToRemove->nSaveCreationTimestamp);
            }

            GAME_LogMessage(6, "[SERVER]  ClientRemoveFromGame:  save and remove client %d '%s' from game %d '%s'%s",
				pClientToRemove->dwClientId, pClientToRemove->szName,
				pGame->nGameId, pGame->szGameName,
                pClientToRemove->bUnlockCharacter ? " (but unlock character)" : "(don't unlock character)");
        }
    }
    else
    {
        if (gpD2EventCallbackTable_6FD45830)
        {
            *(char*)&pClientToRemove->tCharacterInfo.nVersion = '\0';

            if (D2_VERIFY(gpD2EventCallbackTable_6FD45830->pfLeaveGame))
            {
                gpD2EventCallbackTable_6FD45830->pfLeaveGame(
						&pClientToRemove->pClientInfo,
						pGame->nGameId,
						0, 0, 0,0, pClientToRemove->szName,
						reinterpret_cast<const char*>(&pClientToRemove->tCharacterInfo), pClientToRemove->bUnlockCharacter,
						0,
						0,
						pClientToRemove->szAccount, pClientToRemove->nCharSaveTransactionToken D2_SERVER_CALLBACK_ARGS_114(0, 0, 0, 0, 0), &pClientToRemove->nSaveCreationTimestamp);
            }
        }

        GAME_LogMessage(6, "[SERVER]  ClientRemoveFromGame:  no HUNIT to save for client %d '%s' from game %d '%s'%s",
			pClientToRemove->dwClientId, pClientToRemove->szName,
			pGame->nGameId, pGame->szGameName,
            pClientToRemove->bUnlockCharacter ? " (but unlock character)" : "(don't unlock character)");
    }

    if (D2ActiveRoomStrc* pClientRoom = pClientToRemove->pRoom)
    {
        LEVEL_RemoveClientFromAdjacentRooms(pClientRoom, pClientToRemove);
    }
    
    if (D2SaveHeaderStrc* pSaveHeader = pClientToRemove->pSaveHeader)
    {
        D2_FREE_POOL(pClientToRemove->pGame->pMemoryPool, pSaveHeader);
        pClientToRemove->pSaveHeader = 0;
    }

    GAME_LogMessage(6, "[SERVER]  ClientRemoveFromGame: Disconnect %d '%s'", pClientToRemove->dwClientId, pClientToRemove->szName);

    for (D2PacketDataStrc* pPacketData = pClientToRemove->tPacketDataList.pPacketDataPool; pPacketData != nullptr; pPacketData = pClientToRemove->tPacketDataList.pPacketDataPool)
    {
        pClientToRemove->tPacketDataList.pPacketDataPool = pPacketData->pNext;
        D2_FREE_POOL(nullptr, pPacketData);
    }

    D2_FREE_POOL(pClientToRemove->pGame->pMemoryPool, pClientToRemove);

    --pGame->nClients;
}

//D2Game.0x6FC32FE0
//1.14d: 0x00537AC0
void __fastcall CLIENTS_FreeClientsFromGame(D2GameStrc* pGame)
{
    D2ClientStrc* pClient = pGame->pClientList;
    while (pClient)
    {
        D2ClientStrc* pNext = pClient->pNext;
        D2_FREE_POOL(pGame->pMemoryPool, pClient);
        pClient = pNext;
    }
}

//D2Game.0x6FC33020
//1.14d: 0x00537B50
void __fastcall CLIENTS_ChangeRoom(D2ClientStrc* pClient, D2ActiveRoomStrc* pRoom)
{
    if (!pClient || !pClient->pGame || pClient->pRoom == pRoom)
    {
        return;
    }

    DUNGEON_ChangeClientRoom(pClient->pRoom, pRoom);
    D2Common_10077(pClient->pRoom, pRoom);

    D2ActiveRoomStrc** ppAdjacentRooms = 0;
    int32_t nAdjacentRooms = 0;
    if (pRoom)
    {
        DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppAdjacentRooms, &nAdjacentRooms);
    }

    D2ActiveRoomStrc** ppClientAdjacentRooms = 0;
    int32_t nClientAdjacentRooms = 0;
    if (pClient->pRoom)
    {
        DUNGEON_GetAdjacentRoomsListFromRoom(pClient->pRoom, &ppClientAdjacentRooms, &nClientAdjacentRooms);
    }

    for (int32_t i = 0; i < nAdjacentRooms; ++i)
    {
        int32_t j = 0;
        while (j < nClientAdjacentRooms)
        {
            if (ppAdjacentRooms[i] == ppClientAdjacentRooms[j])
            {
                break;
            }

            ++j;
        }

        if (j >= nClientAdjacentRooms)
        {
            LEVEL_AddClient(pClient->pGame, ppAdjacentRooms[i], pClient);
        }
    }

    for (int32_t i = 0; i < nClientAdjacentRooms; ++i)
    {
        int32_t j = 0;
        while (j < nAdjacentRooms)
        {
            if (ppClientAdjacentRooms[i] == ppAdjacentRooms[j])
            {
                break;
            }

            ++j;
        }

        if (j >= nAdjacentRooms)
        {
            LEVEL_RemoveClient(pClient->pGame, ppClientAdjacentRooms[i], pClient);

            if (pClient->pRoom == ppClientAdjacentRooms[i])
            {
                D2UnitStrc* pUnit = nullptr;
                if (pClient->dwFlags & CLIENTFLAGEX_PLAYER_UNIT_ALIVE)
                {
                    pUnit = pClient->pPlayer;
                }
                else if (pClient->pPlayer)
                {
                    pUnit = SUNIT_GetServerUnit(pClient->pGame, pClient->dwUnitType, pClient->dwUnitGUID);
                    if (!pUnit)
                    {
                        pClient->pPlayer = nullptr;
                    }
                }

                PLRMODE_SendPlayerUpdates(pClient->pGame, pUnit, pClient, 0);
            }
        }
    }

    pClient->pRoom = pRoom;

    if (pRoom && pClient->pPlayer)
    {
        UNITS_SetPlayerPortalFlags(pClient->pPlayer, UNITS_GetPlayerPortalFlags(pClient->pPlayer) | DUNGEON_GetPortalFlagFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom)));
    }
}

//D2Game.0x6FC33210
//1.14d: 0x00537FD0
void __fastcall CLIENTS_RefreshUnitsUpdateList(D2ClientStrc* pClient, uint32_t nUpdateSize)
{
    D2_ASSERT(pClient->pRoom);
    D2ActiveRoomStrc** pAdjacentRoomsList = nullptr;
    int32_t nNumRooms = 0;
    DUNGEON_GetAdjacentRoomsListFromRoom(pClient->pRoom, &pAdjacentRoomsList, &nNumRooms);
    D2_ASSERT(pAdjacentRoomsList);

    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, false);
    if (!pPlayer)
    {
        return;
    }

    D2CoordStrc tPlayerCoords;
    UNITS_GetCoords(pPlayer, &tPlayerCoords);

    uint32_t nCurrentMaxManhattanDistance = 0;
    D2ClientUnitUpdateSortStrc tUnitUpdatesSort[55] = {};

    pClient->nUnitUpdateIndex = 0;
    tUnitUpdatesSort[0].nNextIndex = 0;
    tUnitUpdatesSort[0].nDistance = 0;
    int32_t nInsertIndex = 0;

    for (int32_t nCurrentRoom = 0; nCurrentRoom < nNumRooms; nCurrentRoom++)
    {
        for (D2UnitStrc* pRoomUnit = pAdjacentRoomsList[nCurrentRoom]->pUnitFirst; pRoomUnit != nullptr; pRoomUnit = pRoomUnit->pRoomNext)
        {
            if (pRoomUnit->dwUnitType != UNIT_PLAYER)
            {
                continue;
            }

            const int32_t nDeltaPosX = std::abs(tPlayerCoords.nX - CLIENTS_GetUnitX(pRoomUnit));
            const int32_t nDeltaPosY = std::abs(tPlayerCoords.nY - CLIENTS_GetUnitY(pRoomUnit));

            uint32_t nUnitUpdateIndex = pClient->nUnitUpdateIndex;
            uint32_t nManhattanDistance = nDeltaPosY + nDeltaPosX;
            if (nUnitUpdateIndex >= nUpdateSize && nManhattanDistance >= nCurrentMaxManhattanDistance)
            {
                continue;
            }

            int32_t nNextIndex = tUnitUpdatesSort[0].nNextIndex;
            int32_t nPreviousIndex = 0;
            if (tUnitUpdatesSort[0].nNextIndex)
            {
                nNextIndex = tUnitUpdatesSort[0].nNextIndex;
                do
                {
                    if (nManhattanDistance <= tUnitUpdatesSort[nNextIndex].nDistance)
                        break;
                    nPreviousIndex = nNextIndex;
                    nNextIndex = tUnitUpdatesSort[nNextIndex].nNextIndex;
                }
                while (nNextIndex);
            }

            if (nUnitUpdateIndex == nUpdateSize)
            {
                int32_t nLastIndex = 0;
                if (tUnitUpdatesSort[tUnitUpdatesSort[0].nNextIndex].nNextIndex)
                {
                    do
                    {
                        nLastIndex = nNextIndex;
                        nNextIndex = tUnitUpdatesSort[nNextIndex].nNextIndex;
                    }
                    while (tUnitUpdatesSort[nNextIndex].nNextIndex);
                }
                int32_t nPreviousTailIndex = nLastIndex;
                nInsertIndex = tUnitUpdatesSort[nLastIndex].nNextIndex;

                if (nUnitUpdateIndex)
                {
                    if (nManhattanDistance >= tUnitUpdatesSort[nInsertIndex].nDistance)
                    {
                        continue;
                    }
                }

                if (nUnitUpdateIndex == 1)
                {
                    tUnitUpdatesSort[nPreviousTailIndex].nNextIndex = 0;
                    nNextIndex = tUnitUpdatesSort[0].nNextIndex;
                    nCurrentMaxManhattanDistance = nManhattanDistance;
                }
                else
                {
                    nCurrentMaxManhattanDistance = tUnitUpdatesSort[nPreviousTailIndex].nDistance;
                    tUnitUpdatesSort[nPreviousTailIndex].nNextIndex = 0;
                    nNextIndex = tUnitUpdatesSort[0].nNextIndex;
                }
            }
            else
            {
                nInsertIndex = nUnitUpdateIndex;
                pClient->nUnitUpdateIndex = nUnitUpdateIndex + 1;
            }

            if (nPreviousIndex || nManhattanDistance >= tUnitUpdatesSort[0].nDistance)
            {
                tUnitUpdatesSort[nInsertIndex].nNextIndex = tUnitUpdatesSort[nPreviousIndex].nNextIndex;
                tUnitUpdatesSort[nPreviousIndex].nNextIndex = nInsertIndex;
            }
            else
            {
                if (pClient->nUnitUpdateIndex != 1)
                {
                    tUnitUpdatesSort[nInsertIndex].nNextIndex = nNextIndex;
                    tUnitUpdatesSort[nInsertIndex].pUnit = tUnitUpdatesSort[0].pUnit;
                    tUnitUpdatesSort[nInsertIndex].nDistance = tUnitUpdatesSort[0].nDistance;
                }
                tUnitUpdatesSort[0].nNextIndex = nInsertIndex;
                nInsertIndex = 0;
            }

            tUnitUpdatesSort[nInsertIndex].nDistance = nManhattanDistance;
            tUnitUpdatesSort[nInsertIndex].pUnit = pRoomUnit;
            if (nManhattanDistance > nCurrentMaxManhattanDistance)
            {
                nCurrentMaxManhattanDistance = nManhattanDistance;
            }
        }
    }

    CLIENTS_SetUnitsUpdateList(pClient, tUnitUpdatesSort);
}

//D2Game.0x6FC33510
//1.14d: 0x0045ADF0
int32_t __fastcall CLIENTS_GetUnitX(D2UnitStrc* pUnit)
{
    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    case UNIT_MONSTER:
    case UNIT_MISSILE:
        return pUnit->pDynamicPath ? pUnit->pDynamicPath->tGameCoords.wPosX : 0;

    case UNIT_OBJECT:
    case UNIT_ITEM:
    case UNIT_TILE:
        return pUnit->pStaticPath ? pUnit->pStaticPath->tGameCoords.nX : 0;
	default:
		break;
    }

    return 0;
}

//D2Game.0x6FC33540
//1.14d: 0x0045AE20
int32_t __fastcall CLIENTS_GetUnitY(D2UnitStrc* pUnit)
{
    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    case UNIT_MONSTER:
    case UNIT_MISSILE:
        return pUnit->pDynamicPath ? pUnit->pDynamicPath->tGameCoords.wPosY : 0;

    case UNIT_OBJECT:
    case UNIT_ITEM:
    case UNIT_TILE:
        return pUnit->pStaticPath ? pUnit->pStaticPath->tGameCoords.nY : 0;
	default:
		break;
    }

    return 0;
}

//D2Game.0x6FC33570
//1.14d: 0x00537EE0
void __fastcall CLIENTS_SetUnitsUpdateList(D2ClientStrc* pClient, D2ClientUnitUpdateSortStrc* pSort)
{
    D2ClientUnitUpdateSortStrc* pCurrent = pSort;

    pClient->nUnitUpdateIndex = 0;

    do
    {
        pClient->unitUpdate[pClient->nUnitUpdateIndex].nUnitType = pCurrent->pUnit ? pCurrent->pUnit->dwUnitType : 6;
        pClient->unitUpdate[pClient->nUnitUpdateIndex].nUnitGUID = pCurrent->pUnit ? pCurrent->pUnit->dwUnitId : -1;
        pClient->unitUpdate[pClient->nUnitUpdateIndex].nX = CLIENTS_GetUnitX(pCurrent->pUnit);
        pClient->unitUpdate[pClient->nUnitUpdateIndex].nY = CLIENTS_GetUnitY(pCurrent->pUnit);

        ++pClient->nUnitUpdateIndex;
        
        pCurrent = &pSort[pCurrent->nNextIndex];
    }
    while (pCurrent->nNextIndex);
}

//D2Game.0x6FC33670
//1.14d: 0x005380D0
void __fastcall CLIENTS_SynchronizePlayer(D2GameStrc* pGame, D2ClientStrc* pClient)
{
    if (!pGame || !pClient)
    {
        return;
    }

    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (!pPlayer)
    {
        return;
    }

    D2ActiveRoomStrc* pClientRoom = pClient->pRoom;
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pPlayer);
    LEVEL_RemoveUnitsExceptClientPlayer(pClientRoom, pClient);
    LEVEL_UpdateUnitsInAdjacentRooms(pGame, pClientRoom, pClient);
    D2Common_10513(pPlayer, pPlayer, (void(__fastcall*)(D2UnitStrc*, int32_t, int32_t, D2UnitStrc*))D2GAME_UpdateAttribute_6FC822D0);

    if ((pPlayer->dwFlagEx >> 21) & 1)
    {
        D2GAME_ITEMS_UpdateInventoryItems_6FC45050(pGame, pPlayer, 1, 1);
        //D2Game_10034_Return(1);
    }

    ++pClient->nPlayerSyncCount;

    if (pClientRoom != pRoom)
    {
        const int32_t nSourceLevelId = DUNGEON_GetLevelIdFromRoom(pClientRoom);
        const int32_t nDestinationLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
        if (nSourceLevelId != nDestinationLevelId)
        {
            QUESTS_ChangeLevel(pGame, nSourceLevelId, nDestinationLevelId, pPlayer);
            SUNITPROXY_UpdateNpcsOnActChange(pGame, pPlayer, nSourceLevelId, nDestinationLevelId);
        }
        CLIENTS_ChangeRoom(pClient, pRoom);
    }

    ARENA_SynchronizeWithClients(pGame, pClient);

    if (UNITS_GetRoom(pPlayer))
    {
        UNITROOM_RefreshUnit(pPlayer);
    }
}
//1.00 : D2Game.0x10002D70
//1.10f: D2Game.0x6FC337B0
//1.14d: 0x005381C0
int32_t __fastcall CLIENTS_IsInGame(D2GameStrc* pGame, int32_t nClientId)
{
    D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientId);

    if (pClient)
    {
        return 1;
    }

    return 0;
}

//D2Game.0x6FC337E0
//1.14d: 0x005381F0
void __fastcall CLIENTS_SetRoomInClient(D2ClientStrc* pClient, D2ActiveRoomStrc* pRoom)
{
    D2_ASSERT(pClient);

    if (pRoom != pClient->pRoom)
    {
        CLIENTS_ChangeRoom(pClient, pRoom);
    }

    pClient->pRoom = pRoom;
}

//D2Game.0x6FC33830
//1.14d: 0x00538260
void __fastcall D2GAME_SetClientDead_6FC33830(D2ClientStrc* pClient, void* pAlwaysNull)
{
    D2_ASSERT(pClient);

    if (pAlwaysNull)
    {
        pClient->unk0x198[0] = *(int32_t*)pAlwaysNull;
        pClient->unk0x198[1] = *((int32_t*)(pAlwaysNull) + 3);
        pClient->dwFlags |= 2;
    }
    else
    {
        pClient->dwFlags &= ~2;
    }
}

//D2Game.0x6FC33890
//1.14d: 0x005382B0
uint8_t __fastcall CLIENTS_GetActNo(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return pClient->nAct;
}

//D2Game.0x6FC338C0
//1.14d: 0x005382E0
void __fastcall CLIENTS_SetActNo(D2ClientStrc* pClient, uint8_t nAct)
{
    D2_ASSERT(pClient);
    D2_ASSERT(nAct < NUM_ACTS);
    
    pClient->nAct = nAct;
}

//D2Game.0x6FC33910
//1.14d: 0x00538320
void __fastcall CLIENTS_EnableCharacterUnlock(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    pClient->bUnlockCharacter = 1;
}

//D2Game.0x6FC33940
//1.14d: 0x00538350
uint32_t __fastcall D2GAME_GetSaveChecksumFromClient_6FC33940(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return pClient->nSaveChecksum;
}

//D2Game.0x6FC33970
//1.14d: 0x00538380
void __fastcall D2GAME_SetSaveFileChecksum_6FC33970(D2ClientStrc* pClient, uint32_t nSaveChecksum)
{
    D2_ASSERT(pClient);

    pClient->nSaveChecksum = nSaveChecksum;
}

//D2Game.0x6FC339A0
//1.14d: 0x00538590
int32_t __fastcall CLIENTS_GetClientId(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return pClient->dwClientId;
}

//D2Game.0x6FC339E0
//1.14d: 0x005385F0
int32_t __fastcall CLIENTS_GetClassId(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return pClient->nClassId;
}

//D2Game.0x6FC33A10
//1.14d: 0x00538620
void __fastcall CLIENTS_SetClassId(D2ClientStrc* pClient, int32_t nClass)
{
    pClient->nClassId = nClass;
}

//D2Game.0x6FC33A20
//1.14d: 0x00538630
void __fastcall CLIENTS_SetFlags(D2ClientStrc* pClient, int32_t nFlags)
{
    pClient->tSaveFlags.nPackedValue = nFlags;
}

//D2Game.0x6FC33A30
//1.14d: 0x00538640
int32_t __fastcall CLIENTS_GetFlags(D2ClientStrc* pClient)
{
    return pClient->tSaveFlags.nPackedValue;
}

//D2Game.0x6FC33A40
//1.14d: 0x00538650
void __fastcall CLIENTS_ToggleFlag(D2ClientStrc* pClient, uint16_t nFlag, int32_t bSet)
{
    if (bSet)
    {
        pClient->tSaveFlags.nPackedValue |= nFlag;
    }
    else
    {
        pClient->tSaveFlags.nPackedValue &= ~nFlag;
    }
}

//D2Game.0x6FC33A60
//1.14d: 0x00538670
int32_t __fastcall CLIENTS_CheckFlag(D2ClientStrc* pClient, uint16_t nFlag)
{
    return pClient->tSaveFlags.nPackedValue & nFlag;
}

//D2Game.0x6FC33A70
//1.14d: 0x00538680
void __fastcall CLIENTS_UpdateCharacterProgression(D2ClientStrc* pClient, uint16_t nAct, uint16_t nDifficulty)
{
	const int32_t nActsPerDifficulty = pClient->tSaveFlags.bExpansion ? NUM_ACTS : (NUM_ACTS - 1);
    int32_t nNewActDifficulty = nAct + nDifficulty * nActsPerDifficulty;

    if (nNewActDifficulty >= pClient->tSaveFlags.nProgression)
    {
		pClient->tSaveFlags.nProgression = nNewActDifficulty;
    }
}

//D2Game.0x6FC33AC0
//1.14d: 0x005386D0
void __fastcall CLIENTS_SetClientState(D2ClientStrc* pClient, D2ClientState nClientState)
{
    D2_ASSERT(pClient);

    pClient->dwClientState = nClientState;
}

//D2Game.0x6FC33AF0
//1.14d: 0x00538700
void __fastcall CLIENTS_SetIronGolemItemGUID(D2ClientStrc* pClient, int32_t nIronGolemItemGUID)
{
    D2_ASSERT(pClient);

    pClient->nIronGolemItemGUID = nIronGolemItemGUID;
}

//D2Game.0x6FC33B20
//1.14d: 0x00538760
void __fastcall CLIENTS_SetCreateTime(D2ClientStrc* pClient, DWORD dwCreateTime)
{
    D2_ASSERT(pClient);

    pClient->nCreateTime = dwCreateTime;
}

//D2Game.0x6FC33B50
//1.14d: 0x00538790
int32_t __fastcall CLIENTS_GetCreateTime(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return pClient->nCreateTime;
}

//D2Game.0x6FC33B80
//1.14d: 0x005387C0
D2GameStrc* __fastcall CLIENTS_GetGame(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return pClient->pGame;
}

//D2Game.0x6FC33BB0
//1.14d: 0x005387F0
int32_t __fastcall CLIENTS_IsInUnitsRoom(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (pRoom)
    {
        for (int32_t i = 0; i < pRoom->nNumClients; ++i)
        {
            if (pRoom->ppClients[i] == pClient)
            {
                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FC33BE0
//1.14d: 0x00538830
char* __fastcall CLIENTS_GetName(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return pClient->szName;
}

//D2Game.0x6FC33C10
//note: Mentor uses the name D2CLIENTS_TestOtherClients
//1.14d: 0x005388C0
uint32_t __fastcall CLIENTS_HasArenaFlag(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return pClient->dwFlags & CLIENTFLAGEX_ARENA_RELATED;
}

//D2Game.0x6FC33C50
//1.14d: 0x00538930
BOOL __fastcall CLIENTS_CheckState(int32_t nClientId, D2ClientState nExpectedClientState)
{
    if (gbClientListInitialized_6FD447E8)
    {
        EnterCriticalSection(&gClientListLock_6FD447D0);

        for (D2ClientStrc* pClient = gpClientList_6FD43FB8[(uint8_t)nClientId]; pClient; pClient = pClient->pServerNext)
        {
            if (pClient->dwClientId == nClientId)
            {
                const BOOL result = pClient->dwClientState == nExpectedClientState;
                LeaveCriticalSection(&gClientListLock_6FD447D0);
                return result;
            }
        }

        LeaveCriticalSection(&gClientListLock_6FD447D0);
    }

    return FALSE;
}

//D2Game.0x6FC33CD0
//1.14d: 0x005389A0
void __fastcall CLIENTS_UpdatePing(int32_t nClientId, int32_t nClientTick, int32_t nTickAdjustment)
{
    if (D2GameStrc* pGame = GAME_GetGameByClientId(nClientId))
    {
        if (D2ClientStrc* pClient = CLIENTS_GetClientFromClientId(pGame, nClientId))
        {
            DWORD nTickCount = GetTickCount();
            uint32_t nPing = nTickCount - nClientTick - nTickAdjustment;
            pClient->aPingHistory[pClient->dwPingsCount % std::size(pClient->aPingHistory)] = nPing;
            
            uint64_t nPingsSum = 0;
            const uint32_t nPingsCount = std::min(++pClient->dwPingsCount, std::size(pClient->aPingHistory));
            for (int32_t i = 0; i < nPingsCount; ++i)
            {
                nPingsSum += pClient->aPingHistory[i];
            }

            pClient->nPingMovingAverage = nPingsSum / nPingsCount;
            D2GAME_PACKETS_SendPacket0x8F_6FC3FA90(pClient);
            pClient->dwLastPacketTick = nTickCount;
        }
        GAME_LeaveGamesCriticalSection(pGame);
    }
    else if (gbClientListInitialized_6FD447E8)
    {
        D2_LOCK(&gClientListLock_6FD447D0);
        
        if (D2ClientStrc* pClient = CLIENTS_GetClientFromClientListWithId(gpClientList_6FD43FB8[nClientId], nClientId))
        {
            DWORD nTickCount = GetTickCount();
            uint32_t dwPingsCount = pClient->dwPingsCount;
            // This means no more updates of the ping after 10 values. 
            // But this is actually even worse, because the ping count is incremented twice for those, so we only see the 5 first pings.
            if (dwPingsCount < 10)
            {
                uint32_t nPingsSum = dwPingsCount * pClient->nPingMovingAverage;
                ++pClient->dwPingsCount;
                pClient->nPingMovingAverage = (nPingsSum + nTickCount - nClientTick - nTickAdjustment) / pClient->dwPingsCount;
            }
            D2GAME_PACKETS_SendPacket0x8F_6FC3FA90(pClient);
            pClient->dwLastPacketTick = nTickCount;
        }
        LeaveCriticalSection(&gClientListLock_6FD447D0);
    }
}

//D2Game.0x6FC33EA0
//1.14d: 0x00538B70
int32_t __fastcall CLIENTS_GetNameById(int32_t nClientId, char* szName)
{
    if (!gbClientListInitialized_6FD447E8)
    {
        return 1;
    }

    int32_t nResult = 1;
    EnterCriticalSection(&gClientListLock_6FD447D0);

    for (D2ClientStrc* pClient = gpClientList_6FD43FB8[(uint8_t)nClientId]; pClient; pClient = pClient->pServerNext)
    {
        if (!nResult)
        {
            break;
        }

        if (pClient->dwClientId == nClientId)
        {
            if (szName)
            {
                SStrCopy(szName, pClient->szName, sizeof(pClient->szName));
            }

            nResult = 0;
        }
    }

    LeaveCriticalSection(&gClientListLock_6FD447D0);
    return nResult;
}

//D2Game.0x6FC33F20
//1.14d: 0x00538BF0
int32_t __fastcall CLIENTS_GetClientIdByName(const char* szName)
{
    int32_t nClientId = 0;
    const uint8_t nIndex = (uint8_t)FOG_ComputeStringCRC16(szName);

    EnterCriticalSection(&gSrvClientListByNameLock_6FD443B8);

    for (D2ClientStrc* pClient = gpClientListByName_6FD443D0[nIndex]; pClient; pClient = pClient->pServerNextByName)
    {
        if (nClientId)
        {
            break;
        }

        if (!SStrCmpI(pClient->szName, szName, sizeof(pClient->szName)))
        {
            nClientId = pClient->dwClientId;
        }
    }

    LeaveCriticalSection(&gSrvClientListByNameLock_6FD443B8);
    return nClientId;
}

//D2Game.0x6FC33F90
//1.14d: 0x00538C60
int32_t __fastcall CLIENTS_GetCanonicalName(const char* szName, char* szCanonicalName)
{
    int32_t result = 1;
    const uint8_t nIndex = (uint8_t)FOG_ComputeStringCRC16(szName);
    EnterCriticalSection(&gSrvClientListByNameLock_6FD443B8);

    D2ClientStrc* pClient = gpClientListByName_6FD443D0[nIndex];
    if (pClient)
    {
        while (SStrCmpI(pClient->szName, szName, sizeof(pClient->szName)))
        {
            pClient = pClient->pServerNextByName;
            if (!pClient)
            {
                LeaveCriticalSection(&gSrvClientListByNameLock_6FD443B8);
                return 1;
            }
        }

        if (szCanonicalName)
        {
            SStrCopy(szCanonicalName, pClient->szName, sizeof(pClient->szName));
        }

        result = 0;
    }

    LeaveCriticalSection(&gSrvClientListByNameLock_6FD443B8);
    return result;
}

//D2Game.0x6FC34020
//1.14d: 0x00538CE0
int32_t __fastcall CLIENTS_AttachSaveFile(int32_t nClientId, const void* pSaveData, int32_t nSize, int32_t nTotalSize, BOOL bUnlockCharacter, int32_t nUnused1, int32_t nUnused2)
{
    if (!gbClientListInitialized_6FD447E8)
    {
        return 0;
    }

    EnterCriticalSection(&gClientListLock_6FD447D0);

    D2ClientStrc* pClient = gpClientList_6FD43FB8[(uint8_t)nClientId];
    while (pClient)
    {
        if (pClient->dwClientId == nClientId)
        {
            break;
        }

        pClient = pClient->pNext;
    }

    int32_t nResult = 0;

    if (pClient)
    {
        void* pDestination = nullptr;
        if (pClient->nSaveHeaderSize)
        {
            pDestination = (char*)pClient->pSaveHeader + pClient->nSaveHeaderSize;
        }
        else
        {
            pClient->pSaveHeader = (D2SaveHeaderStrc*)D2_ALLOC_POOL(pClient->pGame->pMemoryPool, nTotalSize);
            pDestination = pClient->pSaveHeader;
        }

        D2_ASSERT(pClient->nSaveHeaderSize + nSize <= nTotalSize);

        memcpy(pDestination, pSaveData, nSize);

        pClient->nSaveHeaderSize += nSize;
        pClient->bUnlockCharacter = bUnlockCharacter;

        if (pClient->nSaveHeaderSize == nTotalSize)
        {
            pClient->dwFlags |= CLIENTFLAGEX_HAS_SAVE_CHECKSUM;
            pClient->nSaveChecksum = D2GAME_SAVE_CalculateChecksum_6FC8A140(pClient->pSaveHeader, pClient->nSaveHeaderSize);
            GAME_LogMessage(7, "[LOAD]   CKSUM:%08X len:%-4d %s", pClient->nSaveChecksum, pClient->nSaveHeaderSize, pClient->szName);
            nResult = 1;
        }
    }

    LeaveCriticalSection(&gClientListLock_6FD447D0);
    return nResult;
}

//D2Game.0x6FC34170
//1.14d: 0x00538E00
D2SaveHeaderStrc* __fastcall CLIENTS_GetSaveHeader(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return pClient->pSaveHeader;
}

//D2Game.0x6FC341A0
//1.14d: 0x00538E30
uint32_t __fastcall CLIENTS_GetSaveHeaderSize(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return pClient->nSaveHeaderSize;
}

//D2Game.0x6FC341D0
//1.14d: 0x00538E60
void __fastcall CLIENTS_CopySaveDataToClient(D2ClientStrc* pClient, const void* pData, uint32_t nSize)
{
    D2_ASSERT(pClient);

    if (pClient->nSaveHeaderSize != nSize || pClient->pSaveHeader)
    {
        if (pClient->pSaveHeader)
        {
            D2_FREE_POOL(pClient->pGame->pMemoryPool, pClient->pSaveHeader);
        }

        pClient->pSaveHeader = (D2SaveHeaderStrc*)D2_ALLOC_POOL(pClient->pGame->pMemoryPool, nSize);
        pClient->nSaveHeaderSize = nSize;
    }

    if (pClient->pSaveHeader)
    {
        memcpy(pClient->pSaveHeader, pData, nSize);
    }
}

//D2Game.0x6FC34280
//1.14d: 0x00538F10
void __fastcall CLIENTS_FreeSaveHeader(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    if (pClient->pSaveHeader)
    {
        D2_FREE_POOL(pClient->pGame->pMemoryPool, pClient->pSaveHeader);
        pClient->pSaveHeader = nullptr;
    }

    pClient->nSaveHeaderSize = 0;
    pClient->nSaveHeaderDataSentBytes = 0;
    pClient->nSaveHeaderSendDelay = 0;
    pClient->dwFlags &= ~CLIENTFLAGEX_SAVE_LOADED;
}

//D2Game.0x6FC34300
//1.14d: 0x00538F80
void __fastcall D2GAME_SetSaveLoadComplete_6FC34300(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    pClient->nSaveHeaderSendDelay = 5;
    pClient->nSaveHeaderDataSentBytes = 0;
    pClient->dwFlags |= CLIENTFLAGEX_SAVE_LOADED;
}

//D2Game.0x6FC34350
//1.14d: 0x00538FC0
int32_t __fastcall CLIENTS_HasLoadedSave(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return (pClient->dwFlags & CLIENTFLAGEX_SAVE_LOADED) && pClient->pSaveHeader;
}

//D2Game.0x6FC34390
//1.14d: 0x00539000
void __fastcall CLIENT_SetGameOwner(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    pClient->dwFlags |= CLIENTFLAGEX_GAME_OWNER;
}

//D2Game.0x6FC343D0
//1.14d: 0x00539030
uint32_t __fastcall CLIENT_IsGameOwner(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return (pClient->dwFlags & CLIENTFLAGEX_GAME_OWNER) != 0;
}

//D2Game.0x6FC34420
//1.14d: 0x00539080
D2ClientStrc* __fastcall CLIENTS_GetNext(D2ClientStrc* pClient)
{
    return pClient->pNext;
}

//D2Game.0x6FC34430
//1.14d: 0x005390A0
void __fastcall CLIENTS_SetSkillHotKey(D2ClientStrc* pClient, int32_t nHotkeyId, int16_t nSkillId, uint8_t nHand, int32_t nItemGUID)
{
    if (pClient)
    {
        pClient->HotkeySkills[nHotkeyId].nSkill = nSkillId;
        pClient->HotkeySkills[nHotkeyId].nHand = nHand;
        pClient->HotkeySkills[nHotkeyId].nItemGUID = nItemGUID;
    }
}

//D2Game.0x6FC34460
//1.14d: 0x005390D0
void __fastcall CLIENTS_GetSkillHotKey(D2ClientStrc* pClient, int32_t nId, int32_t* pSkillId, int32_t* nHand, int32_t* nItemGUID)
{
    if (pClient)
    {
        *pSkillId = pClient->HotkeySkills[nId].nSkill;
        *nHand = pClient->HotkeySkills[nId].nHand;
        *nItemGUID = pClient->HotkeySkills[nId].nItemGUID;
    }
}

//D2Game.0x6FC344A0
//1.14d: 0x00539110
int16_t __fastcall CLIENTS_GetGuildFlags(D2ClientStrc* pClient)
{
    return pClient->tGuildInfo.nGuildFlags;
}

//D2Game.0x6FC344B0
//1.14d: 0x00539120
void __fastcall CLIENTS_SetGuildFlags(D2ClientStrc* pClient, int16_t nFlags)
{
    pClient->tGuildInfo.nGuildFlags = nFlags;
}

//D2Game.0x6FC344C0
//1.14d: 0x00539130
void __fastcall CLIENTS_GetGuildTag(D2ClientStrc* pClient, int32_t* pTag)
{
    *pTag = pClient->tGuildInfo.szGuildTag;
}

//D2Game.0x6FC344D0
//1.14d: 0x00539140
void __fastcall CLIENTS_SetGuildTag(D2ClientStrc* pClient, int32_t* pTag)
{
    pClient->tGuildInfo.szGuildTag = *pTag;
}

//D2Game.0x6FC344E0
//1.14d: 0x00539150
void __fastcall CLIENTS_GetGuildName(D2ClientStrc* pClient, char* szName)
{
    int32_t i = 0;
    do
    {
        szName[i] = pClient->tGuildInfo.szGuildName[i];
        ++i;
    }
    while (pClient->tGuildInfo.szGuildName[i - 1]);
}

//D2Game.0x6FC34500
//1.14d: 0x00539170
void __fastcall CLIENTS_SetGuildName(D2ClientStrc* pClient, char* szName)
{
    int32_t i = 0;
    do
    {
        pClient->tGuildInfo.szGuildName[i] = szName[i];
        ++i;
    }
    while (szName[i - 1]);
}

//D2Game.0x6FC34520
//1.14d: 0x00539190
void __fastcall CLIENTS_GetGuildEmblem(D2ClientStrc* pClient, uint8_t* pBackgroundColor, uint8_t* pForegroundColor, uint8_t* pEmblemType)
{
    *pBackgroundColor = pClient->tGuildInfo.nBackgroundColor;
    *pForegroundColor = pClient->tGuildInfo.nForegroundColor;
    *pEmblemType = pClient->tGuildInfo.nEmblemType;
}

//D2Game.0x6FC34550
//1.14d: 0x005391C0
void __fastcall CLIENTS_SetGuildEmblem(D2ClientStrc* pClient, uint8_t nBackgroundColor, uint8_t nForegroundColor, uint8_t nEmblemType)
{
    pClient->tGuildInfo.nBackgroundColor = nBackgroundColor;
    pClient->tGuildInfo.nForegroundColor = nForegroundColor;
    pClient->tGuildInfo.nEmblemType      = nEmblemType;
}

//D2Game.0x6FC34570
//1.14d: 0x005391E0
void __fastcall CLIENTS_SetExperienceLoss(D2ClientStrc* pClient, int32_t nExpLoss)
{
    D2_ASSERT(nExpLoss >= 0);

    pClient->nExpLoss = nExpLoss;
}

//D2Game.0x6FC345A0
//1.14d: 0x00539210
int32_t __fastcall CLIENTS_GetExperienceLoss(D2ClientStrc* pClient)
{
    return pClient->nExpLoss;
}

//D2Game.0x6FC345B0
//1.14d: 0x00539220
uint8_t __fastcall CLIENTS_GetWeaponSwitch(D2ClientStrc* pClient)
{
    return pClient->bSwitchWeapon;
}

//D2Game.0x6FC345C0
//1.14d: 0x00539230
void __fastcall CLIENTS_SetWeaponSwitch(D2ClientStrc* pClient, uint8_t bSwitchWeapon)
{
    pClient->bSwitchWeapon = bSwitchWeapon;
}

//D2Game.0x6FC345D0
//1.14d: 0x00539240
void __fastcall CLIENTS_PacketDataList_Append(D2ClientStrc* pClient, D2PacketDataStrc* pPacketData)
{
    pPacketData->pNext = nullptr;

    if (pClient->tPacketDataList.pTail)
    {
        pClient->tPacketDataList.pTail->pNext = pPacketData;
        pClient->tPacketDataList.pTail = pPacketData;
    }
    else
    {
        pClient->tPacketDataList.pHead = pPacketData;
        pClient->tPacketDataList.pTail = pPacketData;
    }
}

//D2Game.0x6FC34600
//1.14d: 0x00539270
void __fastcall CLIENTS_PacketDataList_Reset(D2ClientStrc* pClient, D2PacketDataStrc* pSinglePacketData)
{
    pSinglePacketData->pNext = nullptr;

    if (pClient->tPacketDataList.pHead)
    {
        pClient->tPacketDataList.pTail = pSinglePacketData;
        pClient->tPacketDataList.pHead = pSinglePacketData;
    }
    else
    {
        pSinglePacketData->pNext = nullptr;
        pClient->tPacketDataList.pHead = pSinglePacketData;
    }
}

//D2Game.0x6FC34630
//1.14d: 0x005392A0
D2PacketDataStrc* __fastcall CLIENTS_PacketDataList_PopHead(D2ClientStrc* pClient)
{
    D2PacketDataStrc* pPacketData = pClient->tPacketDataList.pHead;

    if (pClient->tPacketDataList.pTail == pPacketData)
    {
        pClient->tPacketDataList.pTail = nullptr;
        pClient->tPacketDataList.pHead = nullptr;
    }

    if (pPacketData)
    {
        pClient->tPacketDataList.pHead = pPacketData->pNext;
    }

    return pPacketData;
}

//D2Game.0x6FC34670
//1.14d: 0x005392D0
D2PacketDataStrc* __fastcall CLIENTS_PacketDataList_GetTail(D2ClientStrc* pClient)
{
    return pClient->tPacketDataList.pTail;
}

//D2Game.0x6FC34680
//1.14d: 0x005392E0
D2PacketDataStrc* __fastcall CLIENTS_PacketDataList_GetHead(D2ClientStrc* pClient)
{
    return pClient->tPacketDataList.pHead;
}

//D2Game.0x6FC34690
//1.14d: 0x005392F0
void __fastcall CLIENTS_CopyAccountNameToBuffer(D2ClientStrc* pClient, char* szAccount)
{
    SStrCopy(szAccount, pClient->szAccount, 50u);
}

//D2Game.0x6FC346A0
//1.14d: 0x00539300
void __fastcall D2GAME_GetCharSaveTransactionToken_6FC346A0(D2ClientStrc* pClient, int32_t* pCharSaveTransactionToken)
{
    *pCharSaveTransactionToken = pClient->nCharSaveTransactionToken;
}

//D2Game.0x6FC346B0
//1.14d: 0x00539310
void __fastcall CLIENTS_GetClientInfo(D2ClientStrc* pClient, D2ClientInfoStrc** ppClientInfo)
{
    *ppClientInfo = pClient->pClientInfo;
}

//D2Game.0x6FC346C0
//1.14d: 0x00539320
void __fastcall CLIENTS_SetClientInfo(D2ClientStrc* pClient, D2ClientInfoStrc* pClientInfo)
{
    pClient->pClientInfo = pClientInfo;
}

//D2Game.0x6FC346D0
//1.14d: 0x00539330
D2ClientPlayerDataStrc* __fastcall CLIENTS_GetClientPlayerData(D2ClientStrc* pClient)
{
    D2_ASSERT(pClient);

    return &pClient->clientPlayerData;
}

static const uint32_t kWarpAttemptExpirationFrameCount = 90 * DEFAULT_FRAMES_PER_SECOND;

static bool IsWarpAttemptToBeConsideredForDelay(D2GameStrc* pGame, int32_t nLastWarpFrame)
{
	return nLastWarpFrame != 0
		&& (uint32_t)(pGame->dwGameFrame - nLastWarpFrame) <= kWarpAttemptExpirationFrameCount;
}

//D2Game.0x6FC34700
//1.14d: 0x00539360
void __fastcall CLIENTS_NotifyWarpAttempt(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit && pUnit->dwUnitType == UNIT_PLAYER);

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    D2_ASSERT(pClient);

	for (int32_t& rLastWarpFrameEntry : pClient->aLastWarpAttemptsFrame)
	{
		if (!IsWarpAttemptToBeConsideredForDelay(pGame, rLastWarpFrameEntry))
		{
			rLastWarpFrameEntry = pGame->dwGameFrame;
			break;
		}
	}
}

//D2Game.0x6FC347A0
//1.14d: 0x005393F0
BOOL __fastcall CLIENTS_ShouldDelayWarpAttempt(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit && pUnit->dwUnitType == UNIT_PLAYER);

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    D2_ASSERT(pClient);

    for (const int32_t nLastWarpFrameEntry : pClient->aLastWarpAttemptsFrame)
    {
		if (!IsWarpAttemptToBeConsideredForDelay(pGame, nLastWarpFrameEntry))
        {
            return FALSE;
        }
    }
	// If we had 5 recent warps, delay.
    return TRUE;
}
