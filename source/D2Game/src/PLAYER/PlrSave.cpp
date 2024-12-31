#include "PLAYER/PlrSave.h"

#include <Fog.h>
#include <File.h>
#include <CRC.h>
#include <D2BitManip.h>

#include <D2Items.h>
#include <D2Monsters.h>
#include <D2QuestRecord.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <D2Waypoints.h>

#include "GAME/Arena.h"
#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/Monster.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlayerStats.h"
#include "PLAYER/PlrIntro.h"
#include "PLAYER/PlrSave2.h"
#include "PLAYER/PlrTrade.h"
#include "QUESTS/Quests.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitMsg.h"
#include "UNIT/SUnitNpc.h"


int32_t gbWriteSaveFile_6FD30E08;
int32_t dword_6FD4DC28;


//D2Game.0x6FC895D0 (#10036)
void __fastcall D2GAME_10036_PLRSAVE_EnableSaveFileWriting(int32_t bWriteSaveFile)
{
    gbWriteSaveFile_6FD30E08 = bWriteSaveFile;
}

//D2Game.0x6FC895E0
int32_t __fastcall sub_6FC895E0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2SavedItemStrc* pSavedItem, D2UnitStrc** ppItem, int32_t* a5)
{
    if (a5)
    {
        *a5 = 0;
    }

    D2UnitStrc* pItem = sub_6FC897F0(pGame, pSavedItem);
    if (!pItem)
    {
        return 12;
    }

    switch (pItem->dwAnimMode)
    {
    case IMODE_STORED:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
        UNITS_SetXForStaticUnit(pItem, pSavedItem->nX);
        UNITS_SetYForStaticUnit(pItem, pSavedItem->nY);

        if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pPlayer, pItem->dwUnitId, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 0, 1, 0))
        {
            return 12;
        }

        break;
    }
    case IMODE_EQUIP:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
        int32_t nUnused = 0;
        if (!sub_6FC45060(pGame, pPlayer, pItem->dwUnitId, pSavedItem->nBodyLoc, 1, &nUnused))
        {
            return 12;
        }

        break;
    }
    case IMODE_INBELT:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
        UNITS_SetXForStaticUnit(pItem, pSavedItem->nX);
        UNITS_SetYForStaticUnit(pItem, pSavedItem->nY);
        int32_t nUnused = 0;
        if (!sub_6FC48940(pGame, pPlayer, pItem->dwUnitId, pSavedItem->nX, 0, &nUnused))
        {
            return 12;
        }

        break;
    }
    case IMODE_ONCURSOR:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
        INVENTORY_SetCursorItem(pPlayer->pInventory, pItem);
        if (!sub_6FC4B9D0(pGame, pPlayer, pItem))
        {
            return 12;
        }

        break;
    }
    case IMODE_SOCKETED:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
        int32_t nUnused = 0;
        if (!*ppItem || !D2GAME_ITEMSOCKET_PlaceItem_6FC497E0(pGame, pPlayer, pItem->dwUnitId, (*ppItem)->dwUnitId, &nUnused, 0, 1u, 0))
        {
            return 12;
        }
        break;
    }
    default:
        return 12;
    }

    if (pSavedItem->unk0x01)
    {
        *ppItem = pItem;
    }

    if (a5)
    {
        *a5 = pSavedItem->unk0x01;
    }

    return 0;
}

//D2Game.0x6FC897F0
D2UnitStrc* __fastcall sub_6FC897F0(D2GameStrc* pGame, D2SavedItemStrc* pSavedItem)
{
    D2ItemDropStrc itemDrop = {};

    itemDrop.nId = pSavedItem->nItemId;
    itemDrop.nItemLvl = pSavedItem->nItemLevel;
    itemDrop.nX = pSavedItem->nX;
    itemDrop.nY = pSavedItem->nY;
    itemDrop.nQuantity = pSavedItem->nQuantity;
    itemDrop.wItemFormat = pSavedItem->nItemFormat;
    itemDrop.nQuality = pSavedItem->nItemQuality;
    itemDrop.nMinDur = pSavedItem->nMinDurability;
    itemDrop.nMaxDur = pSavedItem->nMaxDurability;
    itemDrop.nSpawnType = pSavedItem->nSpawnType;
    itemDrop.dwItemSeed = pSavedItem->nItemSeed;
    itemDrop.wUnitInitFlags = 1;
    itemDrop.dwSeed = pSavedItem->nSeed;
    itemDrop.nItemIndex = pSavedItem->nItemIndex;
    itemDrop.bForce = 1;
    itemDrop.pGame = pGame;
    itemDrop.pRoom = 0;
    itemDrop.dwFlags1 = pSavedItem->nItemFlags;
    
    strcpy_s(itemDrop.szName, pSavedItem->szName);

    itemDrop.eEarLvl = pSavedItem->nEarLevel;

    D2UnitStrc* pItem = D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
    if (pItem)
    {
        ITEMS_SetFileIndex(pItem, pSavedItem->nItemIndex);
        ITEMS_SetPage(pItem, pSavedItem->nInvPage);
    }

    return pItem;
}

//D2Game.0x6FC898F0
int32_t __fastcall sub_6FC898F0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2UnitStrc* a4)
{
    if (!pItem)
    {
        return 12;
    }

    switch (pItem->dwAnimMode)
    {
    case IMODE_STORED:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
        if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pPlayer, pItem->dwUnitId, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 0, 1, 0))
        {
            return 12;
        }
        return 0;
    }
    case IMODE_EQUIP:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
        int32_t nItemGUID = -1;
        if (pItem)
        {
            nItemGUID = pItem->dwUnitId;
        }

        int32_t nUnused = 0;
        if (!sub_6FC45060(pGame, pPlayer, nItemGUID, ITEMS_GetBodyLocation(pItem), 1, &nUnused))
        {
            return 12;
        }
        return 0;
    }
    case IMODE_INBELT:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
        int32_t nUnused = 0;
        if (!sub_6FC48940(pGame, pPlayer, pItem->dwUnitId, CLIENTS_GetUnitX(pItem), 0, &nUnused))
        {
            return 12;
        }
        return 0;
    }
    case IMODE_ONCURSOR:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
        INVENTORY_SetCursorItem(pPlayer->pInventory, pItem);
        if (!sub_6FC4B9D0(pGame, pPlayer, pItem))
        {
            return 12;
        }
        return 0;
    }
    case IMODE_SOCKETED:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);

        int32_t nUnused = 0;
        if (!a4 || !D2GAME_ITEMSOCKET_PlaceItem_6FC497E0(pGame, pPlayer, pItem ? pItem->dwUnitId : -1, a4->dwUnitId, &nUnused, 0, 0, 0))
        {
            return 12;
        }

        return 0;
    }
    default:
        return 12;
    }
}

//D2Game.0x6FC89AB0 (#10029)
int32_t __stdcall D2GAME_10029_SAVE_WriteFileInterface(D2GameStrc* pGame, D2UnitStrc* pPlayer, const char* szName)
{
    return D2GAME_SAVE_WriteFile_6FC8A500(pGame, pPlayer, szName, 0);
}

//D2Game.0x6FC89AD0
int32_t __stdcall D2GAME_SAVE_SerializeItem_6FC89AD0(D2UnitStrc* pItem, uint8_t* pBitstream, uint32_t nBitstreamSize, int32_t n0, int32_t a5)
{
    int32_t nBitsLeft = 0;

    if (pItem && !(pItem->dwFlagEx & 0x8000)/* && (n0 || !a5 || D2GAME_Return1_6FC41900())*/)
    {
        nBitsLeft = ITEMS_SerializeItemToBitstream(pItem, pBitstream, nBitstreamSize, 1, 1, 0);

        if (!nBitsLeft)
        {
            FOG_DisplayAssert("!\"Character has too many items\"", __FILE__, __LINE__);
            exit(-1);
        }
    }

    return nBitsLeft;
}

//D2Game.0x6FC89B50
int32_t __stdcall PLRSAVE_WriteItems_6FC89B50(D2InventoryStrc* pInventory, uint8_t* pBitstream, uint32_t nBitstreamSize, int32_t a4, int32_t a5)
{
    if (nBitstreamSize < 2)
    {
        return 0;
    }

    *(WORD*)pBitstream = 'MJ';

    int32_t nItemCount = 0;
    if (pInventory)
    {
        if (INVENTORY_GetCursorItem(pInventory))
        {
            nItemCount = 1;
        }

        for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
        {
            if (INVENTORY_UnitIsItem(pItem))
            {
                ++nItemCount;
            }
        }
    }

    if (nBitstreamSize < 4)
    {
        return 2;
    }

    uint8_t* pItemCount = pBitstream + 2;
    *(uint16_t*)pItemCount = nItemCount;

    uint8_t* pData = pItemCount + 2;
    if (!pInventory)
    {
        return pData - pBitstream;
    }

    int32_t nSize = 0;
    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
    {
        D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(pItem);

        const D2C_PlayerBodyLocs nBodyLoc = ITEMS_GetBodyLocation(pCheckedItem);
        if (INVENTORY_GetItemNodePage(pItem) != 3 || nBodyLoc != BODYLOC_RARM && nBodyLoc != BODYLOC_LARM)
        {
            nSize = D2GAME_SAVE_SerializeItem_6FC89AD0(pCheckedItem, pData, nBitstreamSize + pBitstream - pData, a4, a5);
            if (nSize < 0)
            {
                return -1;
            }

            pData += nSize;

            if (!nSize && (uint16_t)nItemCount > 0u)
            {
                nItemCount += UINT16_MAX;
            }
        }
    }

    D2UnitStrc* pWeapon = INVENTORY_GetLeftHandWeapon(pInventory);
    D2UnitStrc* pRightHandItem = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
    D2UnitStrc* pLeftHandItem = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);
    if (pWeapon)
    {
        if (pRightHandItem == pWeapon)
        {
            nSize = D2GAME_SAVE_SerializeItem_6FC89AD0(pRightHandItem, pData, nBitstreamSize + pBitstream - pData, a4, a5);
            if (nSize < 0)
            {
                return -1;
            }

            pData += nSize;

            if (!nSize && (uint16_t)nItemCount > 0u)
            {
                nItemCount += UINT16_MAX;
            }

            if (pLeftHandItem)
            {
                nSize = D2GAME_SAVE_SerializeItem_6FC89AD0(pLeftHandItem, pData, nBitstreamSize + pBitstream - pData, a4, a5);
                if (nSize < 0)
                {
                    return -1;
                }

                pData += nSize;

                if (!nSize && (uint16_t)nItemCount > 0u)
                {
                    nItemCount += UINT16_MAX;
                }
            }
        }
        else
        {
            nSize = D2GAME_SAVE_SerializeItem_6FC89AD0(pLeftHandItem, pData, nBitstreamSize + pBitstream - pData, a4, a5);
            if (nSize < 0)
            {
                return -1;
            }

            pData += nSize;

            if (!nSize && (uint16_t)nItemCount > 0u)
            {
                nItemCount += UINT16_MAX;
            }

            if (pRightHandItem)
            {
                nSize = D2GAME_SAVE_SerializeItem_6FC89AD0(pRightHandItem, pData, nBitstreamSize + pBitstream - pData, a4, a5);
                if (nSize < 0)
                {
                    return -1;
                }

                pData += nSize;

                if (!nSize && (uint16_t)nItemCount > 0u)
                {
                    nItemCount += UINT16_MAX;
                }
            }
        }
    }
    else
    {
        if (pRightHandItem)
        {
            nSize = D2GAME_SAVE_SerializeItem_6FC89AD0(pRightHandItem, pData, nBitstreamSize + pBitstream - pData, a4, a5);
            if (nSize < 0)
            {
                return -1;
            }

            pData += nSize;

            if (!nSize && (uint16_t)nItemCount > 0u)
            {
                nItemCount += UINT16_MAX;
            }
        }

        if (pLeftHandItem)
        {
            nSize = D2GAME_SAVE_SerializeItem_6FC89AD0(pLeftHandItem, pData, nBitstreamSize + pBitstream - pData, a4, a5);
            if (nSize < 0)
            {
                return -1;
            }

            pData += nSize;

            if (!nSize && (uint16_t)nItemCount > 0u)
            {
                nItemCount += UINT16_MAX;
            }
        }
    }

    D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pInventory);
    if (pCursorItem && pCursorItem->dwUnitType == UNIT_ITEM)
    {
        nSize = D2GAME_SAVE_SerializeItem_6FC89AD0(pCursorItem, pData, nBitstreamSize + pBitstream - pData, a4, a5);
        if (nSize < 0)
        {
            return -1;
        }

        pData += nSize;

        if (!nSize && (uint16_t)nItemCount > 0u)
        {
            nItemCount += UINT16_MAX;
        }
    }

    *(WORD*)pItemCount = nItemCount;

    return pData - pBitstream;
}

//D2Game.0x6FC8A0F0
int32_t __fastcall sub_6FC8A0F0(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t* pSection, int32_t nSize, int32_t a5, int32_t a6)
{
    if (a5 && pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        const int32_t result = PLRTRADE_CopyTradeSaveDataToBuffer(pUnit, pSection, nSize);
        if (result != 0)
        {
            return result;
        }
    }

    return PLRSAVE_WriteItems_6FC89B50(pUnit->pInventory, pSection, nSize, a5, a6);
}

//D2Game.0x6FC8A140
int32_t __fastcall D2GAME_SAVE_CalculateChecksum_6FC8A140(D2SaveHeaderStrc* pSaveHeader, int32_t nSize)
{
    int32_t nCheckSum = 0;
    int32_t nLastTime = 0;
    int32_t nPlayTime = 0;
    int32_t nMapSeed = 0;
    if (pSaveHeader->dwVersion > 91)
    {
        nCheckSum = pSaveHeader->dwChecksum;
        nLastTime = pSaveHeader->dwLasTime;
        nPlayTime = pSaveHeader->dwPlayTime;
        nMapSeed = pSaveHeader->dwMapSeed;

        pSaveHeader->dwChecksum = 0;
        pSaveHeader->dwLasTime = 0;
        pSaveHeader->dwPlayTime = 0;
        pSaveHeader->dwMapSeed = 0;
    }

    const int32_t result = CRC32_Compute(pSaveHeader, nSize);

    if (pSaveHeader->dwVersion > 91)
    {
        pSaveHeader->dwChecksum = nCheckSum;
        pSaveHeader->dwLasTime = nLastTime;
        pSaveHeader->dwPlayTime = nPlayTime;
        pSaveHeader->dwMapSeed = nMapSeed;
    }

    return result;
}

//D2Game.0x6FC8A1B0
int32_t __fastcall D2GAME_SAVE_WriteFileOnRealm_6FC8A1B0(D2GameStrc* pGame, D2UnitStrc* pPlayer, const char* szCharName, char* szAccountName, int32_t bInteractsWithPlayer, int32_t nCharSaveTransactionToken, int32_t a7, D2ClientInfoStrc* pClientInfo)
{
    if (!gbWriteSaveFile_6FD30E08)
    {
        return 0;
    }

    uint8_t pSaveData[8192] = {};
    uint32_t nFileSize = 0;
    PLRSAVE2_CreateSaveFile(pGame, pPlayer, &pSaveData[2], &nFileSize, 8192, bInteractsWithPlayer, a7);

    if (nFileSize <= 0)
    {
        FOG_DisplayHalt("Unable to save player to stream.\n", __FILE__, __LINE__);
        exit(-1);
    }

    *(uint16_t*)pSaveData = nFileSize + 2;

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    
    const uint32_t nCalculatedChecksum = D2GAME_SAVE_CalculateChecksum_6FC8A140((D2SaveHeaderStrc*)&pSaveData[2], nFileSize);
    if (nCalculatedChecksum == D2GAME_GetSaveChecksumFromClient_6FC33940(pClient))
    {
        D2SaveHeaderStrc* pOldSaveData = CLIENTS_GetSaveHeader(pClient);
        const uint32_t nOldFileSize = CLIENTS_GetSaveHeaderSize(pClient);

        D2_ASSERT(pOldSaveData);

        if (nOldFileSize == nFileSize)
        {
            const uint32_t nOldVersion = pOldSaveData->dwVersion;
            uint32_t nOldChecksum = 0;
            uint32_t nOldLastTime = 0;
            uint32_t nOldPlayTime = 0;
            uint32_t nOldMapSeed = 0;
            if (nOldVersion > 91)
            {
                nOldChecksum = pOldSaveData->dwChecksum;
                nOldLastTime = pOldSaveData->dwLasTime;
                nOldPlayTime = pOldSaveData->dwPlayTime;
                nOldMapSeed = pOldSaveData->dwMapSeed;
                pOldSaveData->dwChecksum = 0;
                pOldSaveData->dwLasTime = 0;
                pOldSaveData->dwPlayTime = 0;
                pOldSaveData->dwMapSeed = 0;
            }

            const uint32_t nNewVersion = *(uint32_t*)&pSaveData[6];
            uint32_t nNewChecksum = 0;
            uint32_t nNewLastTime = 0;
            uint32_t nNewPlayTime = 0;
            uint32_t nNewMapSeed = 0;
            if (nNewVersion > 91u)
            {
                nNewChecksum = *(uint32_t*)&pSaveData[14];
                nNewLastTime = *(uint32_t*)&pSaveData[50];
                nNewPlayTime = *(uint32_t*)&pSaveData[54];
                nNewMapSeed = *(uint32_t*)&pSaveData[173];
                *(uint32_t*)&pSaveData[14] = 0;
                *(uint32_t*)&pSaveData[50] = 0;
                *(uint32_t*)&pSaveData[54] = 0;
                *(uint32_t*)&pSaveData[173] = 0;
            }

            bool bDataSameAsPreviousSave = true;
            for (int32_t i = 0; i < nOldFileSize; ++i)
            {
                if (((uint8_t*)pOldSaveData)[i] != pSaveData[i + 2])
                {
					bDataSameAsPreviousSave = false;
                    break;
                }
            }

            if (nOldVersion > 91)
            {
                pOldSaveData->dwChecksum = nOldChecksum;
                pOldSaveData->dwLasTime = nOldLastTime;
                pOldSaveData->dwPlayTime = nOldPlayTime;
                pOldSaveData->dwMapSeed = nOldMapSeed;
            }

            if (nNewVersion > 91)
            {
                *(uint32_t*)&pSaveData[14] = nNewChecksum;
                *(uint32_t*)&pSaveData[50] = nNewLastTime;
                *(uint32_t*)&pSaveData[54] = nNewPlayTime;
                *(uint32_t*)&pSaveData[173] = nNewMapSeed;
            }

            if (bDataSameAsPreviousSave)
            {
				// Since we have the same save file, relock instead of saving
                ++dword_6FD4DC28;

                if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfRelockDatabaseCharacter)
                {
                    gpD2EventCallbackTable_6FD45830->pfRelockDatabaseCharacter(&pClientInfo, szCharName, szAccountName);
                }

                D2GAME_SetClientsRealmId_6FC346C0(pClient, pClientInfo);
                return FALSE;
            }
        }

        GAME_LogMessage(7, "[SAVE] *** %s len:%d checksums matched but data didn't ***", szCharName, nFileSize);
    }

    D2GAME_SetClientsRealmId_6FC346C0(pClient, pClientInfo);
    D2GAME_SetSaveFileChecksum_6FC33970(pClient, nCalculatedChecksum);

    if (gpD2EventCallbackTable_6FD45830 && gpD2EventCallbackTable_6FD45830->pfSaveDatabaseCharacter)
    {
        if (IsBadCodePtr((FARPROC)gpD2EventCallbackTable_6FD45830->pfSaveDatabaseCharacter))
        {
            FOG_DisplayAssert("ptEventCallbackTable->fpSaveDatabaseCharacter", __FILE__, __LINE__);
            exit(-1);
        }

        gpD2EventCallbackTable_6FD45830->pfSaveDatabaseCharacter(&pClientInfo, szCharName, szAccountName, pSaveData, nFileSize + 2, nCharSaveTransactionToken);
        CLIENTS_CopySaveDataToClient(pClient, &pSaveData[2], nFileSize);
    }

    return TRUE;
}

//D2Game.0x6FC8A500
int32_t __fastcall D2GAME_SAVE_WriteFile_6FC8A500(D2GameStrc* pGame, D2UnitStrc* pPlayer, const char* szName, DWORD dwArg)
{
    if (!pPlayer)
    {
        return 0;
    }

    pPlayer->dwFlagEx &= ~UNITFLAGEX_UPDATEINV;

    if (STATES_CheckState(pPlayer, STATE_PLAYERBODY))
    {
        FOG_Trace("[SAVE ERROR] Tried to save a dead body!\n");
    }

    int32_t bInteractsWithPlayer = 0;
    int32_t nUnitType = 6;
    int32_t nUnitGUID = 0;
    if (SUNIT_GetInteractInfo(pPlayer, &nUnitType, &nUnitGUID) && nUnitType == UNIT_PLAYER)
    {
        bInteractsWithPlayer = 1;
    }

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    char szAccountName[50] = {};
    CLIENTS_CopyAccountNameToBuffer(pClient, szAccountName);
    int32_t nCharSaveTransactionToken = 0;
    D2GAME_GetCharSaveTransactionToken_6FC346A0(pClient, &nCharSaveTransactionToken);
    D2ClientInfoStrc* pClientInfo = nullptr;
    D2GAME_GetRealmIdFromClient_6FC346B0(pClient, &pClientInfo);

    uint8_t pSaveData[8192] = {};
    uint32_t nFileSize = 0;

    if (pGame->nGameType == 1 || pGame->nGameType == 2)
    {
        if (CLIENTS_GetSaveHeader(pClient))
        {
            CLIENTS_FreeSaveHeader(pClient);
        }

        PLRSAVE2_CreateSaveFile(pGame, pPlayer, pSaveData, &nFileSize, 0x2000u, bInteractsWithPlayer, 0);

        D2_ASSERT(nFileSize > 0);

        CLIENTS_AttachSaveFile(CLIENTS_GetClientId(pClient), pSaveData, nFileSize, nFileSize, 0, 0, 0);
        D2GAME_SetSaveLoadComplete_6FC34300(pClient);
        return 1;
    }

    if (gpD2EventCallbackTable_6FD45830)
    {
        return D2GAME_SAVE_WriteFileOnRealm_6FC8A1B0(pGame, pPlayer, szName, szAccountName, bInteractsWithPlayer, nCharSaveTransactionToken, dwArg, pClientInfo);
    }

    if (!gbWriteSaveFile_6FD30E08)
    {
        return 1;
    }

    char szDirectory[1024] = {};
    FOG_GetSavePath(szDirectory, 1024);
    char szFileName[500] = {};
    sprintf(szFileName, "%s%s.d2s", szDirectory, szName);

    PLRSAVE2_CreateSaveFile(pGame, pPlayer, pSaveData, &nFileSize, 0x2000u, bInteractsWithPlayer, dwArg);
    
    if (nFileSize <= 0)
    {
        return 1;
    }

    FILE* pFile = fopen(szFileName, "wb"); // NOLINT(clang-diagnostic-deprecated-declarations)
    if (pFile)
    {
        FileLockAndWrite(pSaveData, nFileSize, 1u, pFile);
        fclose(pFile);
    }
    else
    {
        FOG_Trace("Unable to open player save file %s", szFileName);
    }

    return 1;
}

//D2Game.0x6FC8A780
int32_t __fastcall sub_6FC8A780(D2GameStrc* pGame, D2ClientStrc* pClient, uint8_t* pSavefile, D2UnitStrc** ppPlayer, int32_t* pValid, int32_t* pVersion, int32_t* a7, int32_t* a8, int32_t* a9, int32_t* a10, int32_t* a11)
{
    // TODO: Names
    *a11 = 0;

    uint8_t saveHeader[130] = {};
    memcpy(saveHeader, pSavefile, sizeof(saveHeader));

    CLIENTS_SetFlags(pClient, 0);

    if (*(uint32_t*)saveHeader != D2SMAGIC_HEADER)
    {
        if (gpD2EventCallbackTable_6FD45830)
        {
            *pValid = 1;
            return 0;
        }

        return SYSERROR_BAD_INPUT;
    }

    if (*(uint16_t*)&saveHeader[32] != 0x82)
    {
        if (gpD2EventCallbackTable_6FD45830)
        {
            *pValid = 1;
            return 0;
        }

        return SYSERROR_UNK_14;
    }

    const uint32_t nVersion = *(uint32_t*)&saveHeader[4];
    *pVersion = nVersion;

    if (nVersion < 71 || nVersion > 96)
    {
        if (gpD2EventCallbackTable_6FD45830)
        {
            *pValid = 1;
            return 9;
        }

        return SYSERROR_BAD_INPUT;
    }

    saveHeader[23] = 0;

    char* szClientName = CLIENTS_GetName(pClient);
    szClientName[15] = 0;

    if (_strcmpi(szClientName, (const char*)&saveHeader[8]))
    {
        *pValid = 0;
        return SYSERROR_BAD_INPUT;
    }

    const uint32_t nFlags = *(uint32_t*)&saveHeader[24];

    if (nFlags & CLIENTSAVEFLAG_EXPANSION)
    {
        if (!pGame->bExpansion)
        {
            FOG_Trace("[PLAYER LOAD]  Error Loading:%s  Error:SYSERROR_NOTEXPANSIONGAME\n", CLIENTS_GetName(pClient));
            return SYSERROR_NOTEXPANSIONGAME;
        }
    }
    else
    {
        if (pGame->bExpansion)
        {
            FOG_Trace("[PLAYER LOAD]  Error Loading:%s  Error:SYSERROR_EXPANSIONGAME\n", CLIENTS_GetName(pClient));
            return SYSERROR_EXPANSIONGAME;
        }
    }

    if (nFlags & CLIENTSAVEFLAG_LADDER)
    {
        if (!pGame->dwGameType)
        {
            FOG_Trace("[PLAYER LOAD]  ClientAddPlayerToGame()  Error Loading:%s  Error:SYSERROR_NOTLADDERGAME\n", CLIENTS_GetName(pClient));
            CLIENTS_SetPlayerInClient(pClient, 0);
            return SYSERROR_NOTLADDERGAME;
        }
    }
    else
    {
        if (pGame->dwGameType)
        {
            FOG_Trace("[PLAYER LOAD]  ClientAddPlayerToGame()  Error Loading:%s  Error:SYSERROR_LADDERGAME\n", CLIENTS_GetName(pClient));
            CLIENTS_SetPlayerInClient(pClient, 0);
            return SYSERROR_LADDERGAME;
        }
    }

    if (nFlags & CLIENTSAVEFLAG_HARDCORE)
    {
        if (nFlags & CLIENTSAVEFLAG_DEAD)
        {
            return SYSERROR_DEADHARDCORE;
        }

        if (!(ARENA_GetFlags(pGame) & GAMEFLAG_ARENA_HARDCORE))
        {
            return SYSERROR_HARDCOREJOINSOFTCORE;
        }
    }
    else
    {
        if (ARENA_GetFlags(pGame) & GAMEFLAG_ARENA_HARDCORE)
        {
            return SYSERROR_SOFTCOREJOINHARDCORE;
        }
    }

    const uint8_t nCharacterProgression = (nFlags & CLIENTSAVEFLAG_CHARACTER_PROGRESSION_MASK) >> CLIENTSAVEFLAG_CHARACTER_PROGRESSION_BIT;
    if (nCharacterProgression < 8 && pGame->nDifficulty >= DIFFMODE_HELL)
    {
        return SYSERROR_HELL_NOT_UNLOCKED;
    }

    if ((nFlags & CLIENTSAVEFLAG_EXPANSION) && nCharacterProgression < NUM_ACTS && pGame->nDifficulty != DIFFMODE_NORMAL)
    {
        return SYSERROR_NIGHTMARE_NOT_UNLOCKED;
    }

    if (nCharacterProgression < 4 && pGame->nDifficulty != DIFFMODE_NORMAL)
    {
        return SYSERROR_NIGHTMARE_NOT_UNLOCKED;
    }

    CLIENTS_SetGuildFlags(pClient, *(int32_t*)&saveHeader[89]);

    uint8_t v44[4] = {};
    *(uint32_t*)v44 = *(uint32_t*)&saveHeader[91];
    char v47[28] = {};
    memcpy(v47, &saveHeader[95], sizeof(v47));

    v44[3] = 0;
    v47[27] = 0;

    CLIENTS_SetGuildTag(pClient, (int32_t*)v44);
    CLIENTS_SetGuildName(pClient, v47);
    CLIENTS_SetGuildEmblem(pClient, saveHeader[123], saveHeader[124], saveHeader[125]);

    if (nFlags & CLIENTSAVEFLAG_INIT)
    {
        const uint32_t nFlagsCopy = nFlags & (~CLIENTSAVEFLAG_INIT);
        *pValid = 1;
        CLIENTS_SetFlags(pClient, nFlagsCopy);
        return 0;
    }

    CLIENTS_SetWeaponSwitch(pClient, nFlags & CLIENTSAVEFLAG_WEAPON_SWITCH);

    const int32_t nClassId = *(uint16_t*)&saveHeader[34];
    *pValid = 0;
    CLIENTS_SetClassId(pClient, nClassId);
    CLIENTS_SetFlags(pClient, nFlags);

    uint8_t nAct = saveHeader[88] & 0xF;
    const uint8_t nDifficulty = saveHeader[88] >> 4;
    if (nAct >= 5 || nDifficulty >= 3u)
    {
        return SYSERROR_UNK_9;
    }

    if (pGame->nGameType == 3 && !pGame->InitSeed)
    {
        if (pGame->nDifficulty != nDifficulty)
        {
            nAct = 0;
        }
        else
        {
            pGame->dwInitSeed = *(uint32_t*)&saveHeader[126];
        }
    }
    else
    {
        if (pGame->nDifficulty != nDifficulty)
        {
            nAct = 0;
        }
    }

    CLIENTS_SetActNo(pClient, nAct);

    D2UnitStrc* pPlayer = SUNIT_AllocUnitData(UNIT_PLAYER, nClassId, 0, 0, pGame, nullptr, 0, 0, 0);
    *ppPlayer = pPlayer;
    CLIENTS_SetPlayerInClient(pClient, pPlayer);
    SUNIT_InitClientInPlayerData(pPlayer, pClient);
    UNITS_SetNameInPlayerData(pPlayer, CLIENTS_GetName(pClient));
    SUNIT_InitSeed(pPlayer, &pGame->pGameSeed);
    UNITS_ChangeAnimMode(pPlayer, 1);

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    pPlayerData->pTrade = nullptr;
    pPlayerData->dwTradeState = 0;
    pPlayerData->dwAcceptTradeTick = 0;
    pPlayerData->unk0x54 = 0;

    D2GAME_SUNITMSG_FirstFn_6FCC5520(CLIENTS_GetGame(pClient), *ppPlayer, pClient);

    *a7 = 16;
    *a8 = sgptDataTables->nSkillsTxtRecordCount;

    const uint16_t v37 = *(uint16_t*)&saveHeader[30];
    const uint16_t v39 = *(uint16_t*)&saveHeader[28];
    if (v37 > 16 || v39 > sgptDataTables->nSkillsTxtRecordCount)
    {
        if (gpD2EventCallbackTable_6FD45830)
        {
            *pValid = 1;
            return 0;
        }

        return SYSERROR_BAD_INPUT;
    }

    if (v37 != 16)
    {
        *a7 = v37;
    }

    if (v39 != sgptDataTables->nSkillsTxtRecordCount)
    {
        *a8 = v39;
    }

    uint8_t* v40 = &saveHeader[71];
    for (int32_t i = 0; i < 8; ++i)
    {
        int32_t v41 = *(v40 - 1);
        if (v41 < 156 || v41 > 216)
        {
            if (v41 == 255)
            {
                v41 = -1;
            }
        }
        else
        {
            v41 = v41 + 65;
        }
        CLIENTS_SetSkillHotKey(pClient, i, v41, *v40, -1);
        v40 += 2;
    }

    int32_t v42 = saveHeader[86];
    if (v42 < 156 || v42 > 216)
    {
        if (v42 == 255)
        {
            v42 = -1;
        }
    }
    else
    {
        v42 = saveHeader[86] + 65;
    }
    *a9 = v42;

    int32_t v43 = saveHeader[87];
    if (v43 < 156 || v43 > 216)
    {
        if (v43 == 255)
        {
            v43 = -1;
        }
    }
    else
    {
        v43 = saveHeader[87] + 65;
    }
    *a10 = v43;

    *a11 = *(uint16_t*)&saveHeader[32];
    return 0;
}

//D2Game.0x6FC8AD50
int32_t __fastcall D2GAME_SAVE_ReadWaypointData_6FC8AD50(D2UnitStrc* pUnit, uint8_t* pSection, int32_t nUnused, int32_t* pSize)
{
    constexpr uint32_t WAYPOINT_DATA_SIZE = 80;

    *pSize = 0;

    uint8_t pSource[WAYPOINT_DATA_SIZE] = {};
    memcpy(pSource, pSection, sizeof(pSource));

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
    if (*(uint16_t*)pSource != 'SW')
    {
        return SYSERROR_BAD_WAYPOINT;
    }

    uint8_t* pData = &pSource[8];
    for (int32_t i = 0; i < 3; ++i)
    {
        D2WaypointDataStrc* pWaypointData = (D2WaypointDataStrc*)pData;
        if (pWaypointData->nFlags[0] != 0x102 && pWaypointData->nFlags[0] != 0x101 && pWaypointData->nFlags[0] != 0)
        {
            return SYSERROR_BAD_WAYPOINT;
        }

        WAYPOINTS_CopyAndValidateWaypointData(pPlayerData->pWaypointData[i], pWaypointData);
        pData = &pData[24];
    }

    *pSize = WAYPOINT_DATA_SIZE;
    return 0;
}

//D2Game.0x6FC8ADE0
int32_t __fastcall sub_6FC8ADE0(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t* pSection, int32_t a4, int32_t nUnused, int32_t* pSize)
{
    *pSize = 0;

    if (*(uint16_t*)pSection == 'fg')
    {
        const uint8_t* pData = pSection + 2;
        int32_t nSize = 0;
        for (int32_t i = 0; i < a4; ++i)
        {
            if ((uint8_t)(1 << (i & 7)) & pData[i / 8])
            {
                STATLIST_SetUnitStat(pUnit, i, *(uint32_t*)&pData[4 * nSize + 3], 0);
                ++nSize;
            }
            else
            {
                STATLIST_SetUnitStat(pUnit, i, 0, 0);
            }
        }

        if (ARENA_IsInArenaMode(pGame))
        {
            STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, STATLIST_GetMaxLifeFromUnit(pUnit), 0);
            STATLIST_SetUnitStat(pUnit, STAT_MANA, STATLIST_GetMaxManaFromUnit(pUnit), 0);
        }

        STATLIST_SetUnitStat(pUnit, STAT_STAMINA, STATLIST_GetMaxStaminaFromUnit(pUnit), 0);
        *pSize = 4 * nSize + 5;

        return 0;
    }

    return SYSERROR_BAD_STATS;
}

//D2Game.0x6FC8AEC0
int32_t __fastcall sub_6FC8AEC0(D2GameStrc* pGame, D2ClientStrc* pClient, D2UnitStrc* pUnit, uint8_t* pSection, int32_t a5, int32_t a6, int32_t* pSkillCount)
{
    *pSkillCount = 0;

    if (*(uint16_t*)pSection == 'fi')
    {
        const int32_t nClassId = CLIENTS_GetClassId(pClient);
        for (int32_t i = 0; i < SKILLS_GetPlayerSkillCount(nClassId); ++i)
        {
            const int32_t nSkillId = SKILLS_GetClassSkillId(nClassId, i);
            if (pSection[i + 2])
            {
                D2GAME_SetSkills_6FD14C60(pUnit, nSkillId, pSection[i + 2], 1);
            }
        }

        sub_6FC3DCA0(pClient, pUnit);

        *pSkillCount = SKILLS_GetPlayerSkillCount(nClassId) + 2;
        
        sub_6FD14C30(pUnit);
        
        return 0;
    }

    return SYSERROR_BAD_SKILLS;
}

//D2Game.0x6FC8AF70
int32_t __fastcall sub_6FC8AF70(D2SavedItemStrc* pSavedItem, uint8_t* pData)
{
    memset(pSavedItem, 0, sizeof(D2SavedItemStrc));

    D2BitBufferStrc bitBuffer = {};
    BITMANIP_Initialize(&bitBuffer, pData, 0x40u);

    pSavedItem->nItemFlags = BITMANIP_Read(&bitBuffer, 32);

    if (ITEMS_CheckIfFlagIsSet(pSavedItem->nItemFlags, IFLAG_UNK3))
    {
        if (ITEMS_CheckIfFlagIsSet(pSavedItem->nItemFlags, IFLAG_COMPACTSAVE))
        {
            pSavedItem->nItemFormat = (uint8_t)BITMANIP_Read(&bitBuffer, 10);
            pSavedItem->nBodyLoc = (D2C_PlayerBodyLocs)BITMANIP_Read(&bitBuffer, 5);
            pSavedItem->nItemLevel = 1;
            pSavedItem->nSpawnType = BITMANIP_Read(&bitBuffer, 3);
            pSavedItem->nX = (uint8_t)BITMANIP_Read(&bitBuffer, 5);
            pSavedItem->nY = (uint8_t)BITMANIP_Read(&bitBuffer, 3);
            pSavedItem->nInvPage = (D2C_ItemInvPage)BITMANIP_Read(&bitBuffer, 8);
            
            if (ITEMS_CheckIfFlagIsSet(pSavedItem->nItemFlags, IFLAG_ISEAR))
            {
                pSavedItem->nItemCode = ' rae';
                pSavedItem->nItemIndex = BITMANIP_Read(&bitBuffer, 3);
                pSavedItem->nEarLevel = BITMANIP_Read(&bitBuffer, 8);

                for (int32_t i = 0; i < 16; ++i)
                {
                    pSavedItem->szName[i] = BITMANIP_Read(&bitBuffer, 7);
                }
            }
            else
            {
                pSavedItem->nItemCode = BITMANIP_Read(&bitBuffer, 32);
            }
        }
        else
        {
            pSavedItem->nItemFormat = (uint8_t)BITMANIP_Read(&bitBuffer, 10);
            pSavedItem->nItemCode = BITMANIP_Read(&bitBuffer, 32);
            pSavedItem->nBodyLoc = (D2C_PlayerBodyLocs)BITMANIP_Read(&bitBuffer, 5);
            pSavedItem->unk0x01 = BITMANIP_Read(&bitBuffer, 3);
            pSavedItem->nItemLevel = (uint8_t)BITMANIP_Read(&bitBuffer, 12);
            pSavedItem->nSpawnType = BITMANIP_Read(&bitBuffer, 3);
            pSavedItem->nItemQuality = BITMANIP_Read(&bitBuffer, 4);
            pSavedItem->nQuantity = BITMANIP_ReadSigned(&bitBuffer, 20);
            pSavedItem->nMinDurability = (uint8_t)BITMANIP_Read(&bitBuffer, 8);
            pSavedItem->nMaxDurability = (uint8_t)BITMANIP_Read(&bitBuffer, 8);
            pSavedItem->nX = (uint8_t)BITMANIP_Read(&bitBuffer, 5);
            pSavedItem->nY = (uint8_t)BITMANIP_Read(&bitBuffer, 3);
            pSavedItem->nItemIndex = BITMANIP_ReadSigned(&bitBuffer, 10);
            pSavedItem->nSeed = BITMANIP_Read(&bitBuffer, 32);
            pSavedItem->nItemSeed = BITMANIP_Read(&bitBuffer, 32);
            pSavedItem->nInvPage = (D2C_ItemInvPage)BITMANIP_Read(&bitBuffer, 8);
        }

        pSavedItem->nItemId = DATATBLS_GetItemIdFromItemCode(pSavedItem->nItemCode);
        return BITMANIP_GetSize(&bitBuffer);
    }
    else
    {
        pSavedItem->nItemFormat = 0;
        
        if (ITEMS_CheckIfFlagIsSet(pSavedItem->nItemFlags, IFLAG_ISEAR))
        {
            pSavedItem->nBodyLoc = (D2C_PlayerBodyLocs)BITMANIP_Read(&bitBuffer, 5);
            pSavedItem->nItemLevel = 1;
            BITMANIP_ReadSigned(&bitBuffer, 10);
            pSavedItem->nItemId = DATATBLS_GetItemIdFromItemCode(' rae');
            pSavedItem->nSpawnType = BITMANIP_Read(&bitBuffer, 3);
            pSavedItem->nX = (uint8_t)BITMANIP_Read(&bitBuffer, 5);
            pSavedItem->nY = (uint8_t)BITMANIP_Read(&bitBuffer, 3);
            pSavedItem->nItemIndex = BITMANIP_ReadSigned(&bitBuffer, 10);
            pSavedItem->nInvPage = (D2C_ItemInvPage)BITMANIP_Read(&bitBuffer, 8);
            pSavedItem->nEarLevel = BITMANIP_Read(&bitBuffer, 8);

            for (int32_t i = 0; i < 16; ++i)
            {
                pSavedItem->szName[i] = BITMANIP_Read(&bitBuffer, 7);
                if (!pSavedItem->szName[i])
                {
                    break;
                }
            }

            pSavedItem->nItemCode = ' rae';
        }
        else
        {
            pSavedItem->nBodyLoc = (D2C_PlayerBodyLocs)BITMANIP_Read(&bitBuffer, 5);
            pSavedItem->unk0x01 = BITMANIP_Read(&bitBuffer, 3);
            pSavedItem->nItemLevel = (uint8_t)BITMANIP_Read(&bitBuffer, 12);
            const int32_t nItemId = BITMANIP_ReadSigned(&bitBuffer, 10);
            pSavedItem->nSpawnType = BITMANIP_Read(&bitBuffer, 3);
            pSavedItem->nItemQuality = BITMANIP_Read(&bitBuffer, 4);
            pSavedItem->nQuantity = BITMANIP_ReadSigned(&bitBuffer, 20);
            pSavedItem->nMinDurability = (uint8_t)BITMANIP_Read(&bitBuffer, 8);
            pSavedItem->nMaxDurability = (uint8_t)BITMANIP_Read(&bitBuffer, 8);
            pSavedItem->nX = (uint8_t)BITMANIP_Read(&bitBuffer, 5);
            pSavedItem->nY = (uint8_t)BITMANIP_Read(&bitBuffer, 3);
            pSavedItem->nItemIndex = BITMANIP_ReadSigned(&bitBuffer, 10);
            pSavedItem->nSeed = BITMANIP_Read(&bitBuffer, 32);
            pSavedItem->nItemSeed = BITMANIP_Read(&bitBuffer, 32);
            pSavedItem->nInvPage = (D2C_ItemInvPage)BITMANIP_Read(&bitBuffer, 8);

            if (nItemId >= 0)
            {
                pSavedItem->nItemId = DATATBLS_MapOldItemIndexToCurrent(nItemId);

                D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pSavedItem->nItemId);
                if (pItemsTxtRecord)
                {
                    pSavedItem->nItemCode = pItemsTxtRecord->dwCode;
                }
                else
                {
                    pSavedItem->nItemId = -1;
                    pSavedItem->nItemCode = 0;
                }
            }
            else
            {
                pSavedItem->nItemId = -1;
                pSavedItem->nItemCode = 0;
            }
        }

        return 25;
    }
}

//D2Game.0x6FC8B3D0
int32_t __fastcall sub_6FC8B3D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t* pSection, uint32_t dwVersion, int32_t nSize, int32_t a6, int32_t* pSize)
{
    // Original game computes the remaining size but does nothing with it.
    D2_MAYBE_UNUSED(nSize);

    if (*(uint16_t*)pSection != 'MJ')
    {
        return 14;
    }

    int32_t nTotalSize = 4;
    uint8_t* pData = pSection + 4;
    const int32_t nTotal = *((uint16_t*)pSection + 1);
    for (int32_t j = 0; j < nTotal; ++j)
    {
        D2SavedItemStrc savedItem = {};

        if (*(uint16_t*)pData != 'MJ')
        {
            return SYSERROR_UNK_14;
        }

        pData += 2;
        int32_t nDataSize = sub_6FC8AF70(&savedItem, pData);
        pData += nDataSize;
        nTotalSize += nDataSize + 2;

        int32_t nCount = 0;
        D2UnitStrc* pItem = nullptr;
        if (a6)
        {
            const int32_t nResult = sub_6FC8B680(pGame, pPlayer, &savedItem, &pItem, &nCount);
            if (nResult)
            {
                return nResult;
            }
        }
        else
        {
            if (sub_6FC895E0(pGame, pPlayer, &savedItem, &pItem, &nCount))
            {
                continue;
            }
        }

        for (int32_t i = 0; i < nCount; ++i)
        {
            memset(&savedItem, 0, sizeof(savedItem));

            if (*(uint16_t*)pData != 'MJ')
            {
                return SYSERROR_UNK_14;
            }

            pData += 2;
            nDataSize = sub_6FC8AF70(&savedItem, pData);
            pData += nDataSize;
            nTotalSize += nDataSize + 2;

            int32_t nResult = 0;
            if (a6)
            {
                int32_t nUnused = 0;
                nResult = sub_6FC8B680(pGame, pPlayer, &savedItem, &pItem, &nUnused);
            }
            else
            {
                int32_t nUnused = 0;
                nResult = sub_6FC895E0(pGame, pPlayer, &savedItem, &pItem, &nUnused);
            }

            if (nResult)
            {
                return nResult;
            }
        }
    }

    *pSize = nTotalSize;
    return 0;
}

//D2Game.0x6FC8B680
int32_t __fastcall sub_6FC8B680(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SavedItemStrc* pSavedItem, D2UnitStrc** ppItem, int32_t* a5)
{
    D2UnitStrc* pItem = sub_6FC897F0(pGame, pSavedItem);
    if (!pItem)
    {
        return SYSERROR_INVENTORY;
    }

    switch (pItem->dwAnimMode)
    {
    case IMODE_STORED:
    {
        UNITS_ChangeAnimMode(pItem, 4);
        UNITS_SetXForStaticUnit(pItem, pSavedItem->nX);
        UNITS_SetYForStaticUnit(pItem, pSavedItem->nY);

        if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pUnit, pItem->dwUnitId, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 0, 0, 0))
        {
            return SYSERROR_INVENTORY;
        }
        break;
    }
    case IMODE_EQUIP:
    {
        const D2C_PlayerBodyLocs nBodyLoc = ITEMS_GetBodyLocation(pItem);
        if (!nBodyLoc || !INVENTORY_PlaceItemInBodyLoc(pUnit->pInventory, pItem, nBodyLoc))
        {
            return SYSERROR_INVENTORY;
        }

        int32_t nUnused = 3;
        if (nBodyLoc == 11 || nBodyLoc == 12)
        {
            nUnused = 4;
        }

        if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pItem, nUnused))
        {
            return SYSERROR_INVENTORY;
        }

        ITEMS_SetBodyLocation(pItem, nBodyLoc);
        pItem->dwFlags &= 0xFFFFFFFDu;
        UNITS_ChangeAnimMode(pItem, 1);
        ITEMS_SetPage(pItem, INVPAGE_NULL);
        ITEMS_SetItemCMDFlag(pItem, 8, 1);
        pItem->dwFlags &= 0xFDFFFFFFu;
        INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
        UNITS_RefreshInventory(pUnit, 1);
        QUESTS_ItemPickedUp(pGame, pUnit, pItem);
        break;
    }
    case IMODE_SOCKETED:
    {
        if (!*ppItem)
        {
            return 13;
        }

        UNITS_ChangeAnimMode(pItem, 4);

        int32_t nUnused = 0;
        if (!D2GAME_ITEMSOCKET_PlaceItem_6FC497E0(pGame, pUnit, pItem->dwUnitId, (*ppItem) ? (*ppItem)->dwUnitId : -1, &nUnused, 0, 1u, 0))
        {
            return SYSERROR_INVENTORY;
        }

        break;
    }
    default:
        return SYSERROR_INVENTORY;
    }

    if (pSavedItem->unk0x01)
    {
        *ppItem = pItem;
    }

    *a5 = pSavedItem->unk0x01;
    return 0;
}

//D2Game.0x6FC8B8A0
int32_t __fastcall PLRSAVE_ReadItems_6FC8B8A0(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t* pSection, uint32_t dwVersion, int32_t nSize, int32_t a6, int32_t* pSize)
{
    if (*(uint16_t*)pSection != 'MJ')
    {
        return 14;
    }

    const uint16_t nItemCount = *((uint16_t*)pSection + 1);

    uint8_t* pItemBitstream = pSection + 4;
    for (int32_t i = 0; i < nItemCount; ++i)
    {
        const uint32_t nBitstreamSize = pSection - pItemBitstream + nSize;
        D2ItemSaveStrc itemSave = {};
        ITEMS_GetCompactItemDataFromBitstream(pItemBitstream, nBitstreamSize, 1, &itemSave);
        if (itemSave.nClassId == -1)
        {
            return SYSERROR_UNK_14;
        }

        uint32_t nRemainingSize = 0;
        D2UnitStrc* pItem = sub_6FC4EC10(pGame, 0, pItemBitstream, nBitstreamSize, 1, &itemSave, &nRemainingSize, dwVersion);
        if (!pItem && nRemainingSize <= 0)
        {
            return SYSERROR_UNK_14;
        }

        pItemBitstream += nRemainingSize;
        
        int32_t bRemoveItem = 0;
        if (pItem)
        {
            if (a6)
            {
                bRemoveItem = sub_6FC8BAA0(pGame, pPlayer, pItem, 0);
            }
            else
            {
                bRemoveItem = sub_6FC898F0(pGame, pPlayer, pItem, 0);
            }
        }

        D2UnitStrc* pParentItem = pItem;
        if (bRemoveItem)
        {
            if (pItem)
            {
                SUNIT_RemoveUnit(pGame, pItem);
            }

            pParentItem = nullptr;
        }

        for (int32_t j = 0; j < itemSave.nItemFileIndex; ++j)
        {
            const uint32_t nSocketableBitstreamSize = pSection - pItemBitstream + nSize;
            ITEMS_GetCompactItemDataFromBitstream(pItemBitstream, nSocketableBitstreamSize, 1, &itemSave);
            
            D2UnitStrc* pSocketable = sub_6FC4EC10(pGame, 0, pItemBitstream, nSocketableBitstreamSize, 1, &itemSave, &nRemainingSize, dwVersion);
            if (!pSocketable && nRemainingSize <= 0)
            {
                return SYSERROR_UNK_14;
            }

            pItemBitstream += nRemainingSize;
            
            if (pParentItem)
            {
                if (pSocketable)
                {
                    if (a6)
                    {
                        sub_6FC8BAA0(pGame, pPlayer, pSocketable, pParentItem);
                    }
                    else
                    {
                        sub_6FC898F0(pGame, pPlayer, pSocketable, pParentItem);
                    }
                }
            }
            else
            {
                if (pSocketable)
                {
                    SUNIT_RemoveUnit(pGame, pSocketable);
                }
            }
        }

        if (pParentItem && ITEMS_GetItemFlags(pParentItem) & IFLAG_RUNEWORD && !ITEMS_GetRunesTxtRecordFromItem(pParentItem))
        {
            D2GAME_ITEMMODE_Unk_6FC4BC10(pGame, pPlayer, pParentItem);
        }
    }

    if (pSize)
    {
        *pSize = pItemBitstream - pSection;
    }

    return 0;
}

//D2Game.0x6FC8BAA0
int32_t __fastcall sub_6FC8BAA0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2UnitStrc* a4)
{
    if (!pItem)
    {
        return SYSERROR_INVENTORY;
    }

    switch (pItem->dwAnimMode)
    {
    case IMODE_STORED:
    {
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);

        if (D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pPlayer, pItem->dwUnitId, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 0, 0, 0))
        {
            return 0;
        }

        return SYSERROR_INVENTORY;
    }
    case IMODE_EQUIP:
    {
        const D2C_PlayerBodyLocs nBodyLoc = ITEMS_GetBodyLocation(pItem);
        if (nBodyLoc == BODYLOC_NONE || !INVENTORY_PlaceItemInBodyLoc(pPlayer->pInventory, pItem, nBodyLoc))
        {
            return 13;
        }

        int32_t nUnused = 3;
        if (nBodyLoc == BODYLOC_SWRARM || nBodyLoc == BODYLOC_SWLARM)
        {
            nUnused = 4;
        }

        if (!INVENTORY_PlaceItemInSocket(pPlayer->pInventory, pItem, nUnused))
        {
            return SYSERROR_INVENTORY;
        }

        ITEMS_SetBodyLocation(pItem, nBodyLoc);

        if (pItem)
        {
            pItem->dwFlags &= 0xFFFFFFFD;
        }

        UNITS_ChangeAnimMode(pItem, 1);
        ITEMS_SetPage(pItem, INVPAGE_NULL);
        ITEMS_SetItemCMDFlag(pItem, 8, 1);

        if (pItem)
        {
            pItem->dwFlags &= 0xFDFFFFFFu;
        }

        INVENTORY_AddItemToTradeInventory(pPlayer->pInventory, pItem);
        UNITS_RefreshInventory(pPlayer, 1);
        QUESTS_ItemPickedUp(pGame, pPlayer, pItem);
        return 0;
    }
    case IMODE_SOCKETED:
    {
        if (!a4)
        {
            return SYSERROR_INVENTORY;
        }

        UNITS_ChangeAnimMode(pItem, 4);

        int32_t bPlaced = 0;
        if (!D2GAME_ITEMSOCKET_PlaceItem_6FC497E0(pGame, pPlayer, pItem ? pItem->dwUnitId : -1, a4->dwUnitId, &bPlaced, 0, 1u, 0))
        {
            return SYSERROR_INVENTORY;
        }

        return 0;
    }
    default:
        return SYSERROR_INVENTORY;
    }
}

//D2Game.0x6FC8BC70
int32_t __fastcall sub_6FC8BC70(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t* pSection, uint32_t dwVersion, int32_t nSize, int32_t a6, int32_t* pSize)
{
    if (dwVersion == 71)
    {
        return sub_6FC8B3D0(pGame, pUnit, pSection, 71u, nSize, a6, pSize);
    }
    else if (dwVersion >= 72 && dwVersion <= 96)
    {
        return PLRSAVE_ReadItems_6FC8B8A0(pGame, pUnit, pSection, dwVersion, nSize, a6, pSize);
    }
    else
    {
        return 1;
    }
}

//D2Game.0x6FC8BCC0
int32_t __fastcall sub_6FC8BCC0(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t* pSection, uint32_t dwVersion, int32_t nRemainingSize, int32_t* pSize)
{
    *pSize = 0;

    if (dwVersion < 71 || dwVersion > 96)
    {
        return 1;
    }

    if (*(uint16_t*)pSection != 'MJ')
    {
        return SYSERROR_UNK_14;
    }

    uint8_t* pData = pSection + 4;
    nRemainingSize -= 4;
    int32_t nSize = 4;

    for (int32_t i = 0; i < *(uint16_t*)(pSection + 2); ++i)
    {
        D2UnitStrc* pPlayerBody = SUNIT_AllocUnitData(UNIT_PLAYER, pPlayer ? pPlayer->dwClassId : -1, 0, 0, pGame, nullptr, 1, PLRMODE_DEAD, 0);
        if (!pPlayerBody)
        {
            return SYSERROR_CORPSES;
        }

        pPlayerBody->dwFlags &= 0xFFFFFFFB;

        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
        SUNIT_InitClientInPlayerData(pPlayerBody, pClient);
        UNITS_SetNameInPlayerData(pPlayerBody, CLIENTS_GetName(pClient));
        SUNIT_InitSeed(pPlayerBody, &pGame->pGameSeed);
        STATES_ToggleState(pPlayerBody, STATE_PLAYERBODY, 1);
        pPlayerBody->dwFlags &= 0xFFFFFFEFu;
        INVENTORY_SetOwnerId(pPlayerBody->pInventory, pPlayer ? pPlayer->dwUnitId : -1);

        int32_t nReadSize = 0;
        const int32_t nErrorCode = sub_6FC8BC70(pGame, pPlayer, pData + 12, dwVersion, nRemainingSize - 12, 1, &nReadSize);
        if (nErrorCode)
        {
            return nErrorCode;
        }

        INVENTORY_CreateCorpseForPlayer(pPlayer->pInventory, pPlayerBody->dwUnitId, 0, 1);
        D2GAME_SendPacket0x8E_CorpseAssign_ToAllPlayers_6FC3F9B0(pGame, pPlayer->dwUnitId, pPlayerBody->dwUnitId, 1u);

        const int32_t nSectionSize = nReadSize + 12;
        nRemainingSize -= nSectionSize;
        pData += nSectionSize;
        nSize += nSectionSize;
    }

    *pSize = nSize;
    return 0;
}

//D2Game.0x6FC8BEE0
int32_t __fastcall sub_6FC8BEE0(int16_t nHirelingId, int32_t nLevel)
{
    switch (nHirelingId)
    {
    case 0:
    case 1:
        if (nLevel < 49)
        {
            return (((nLevel < 25) - 1) & 100) + 100;
        }

        return 400;
    case 2:
    case 3:
        return (((nLevel < 49) - 1) & 220) + 220;
    case 4:
    case 5:
    case 12:
    case 13:
    case 14:
    case 21:
    case 23:
        return 480;
    case 6:
    case 7:
    case 8:
        if (nLevel < 55)
        {
            return (((nLevel < 31) - 1) & 105) + 105;
        }

        return 420;
    case 9:
    case 10:
    case 11:
        return (((nLevel < 55) - 1) & 230) + 230;
    case 15:
    case 17:
        if (nLevel < 61)
        {
            return (((nLevel < 37) - 1) & 110) + 110;
        }

        return 440;
    case 16:
        if (nLevel < 61)
        {
            return (((nLevel < 37) - 1) & 115) + 115;
        }

        return 260;
    case 18:
    case 20:
        return (((nLevel < 61) - 1) & 230) + 230;
    case 19:
        return (((nLevel < 61) - 1) & 240) + 240;
    case 22:
        return 500;
    case 24:
    case 25:
        if (nLevel < 75)
        {
            return (((nLevel < 42) - 1) & 220) + 220;
        }

        return 880;
    case 26:
    case 27:
        return (((nLevel < 75) - 1) & 460) + 460;
    default:
        return 960;
    }
}

//D2Game.0x6FC8C050
uint32_t __fastcall sub_6FC8C050(D2GameStrc* pGame, int16_t nHirelingId, uint32_t a3)
{
    int32_t nLevel = 1;
    while (nLevel < 99)
    {
        if (3 * nLevel * (nLevel + 1) * sub_6FC8BEE0(nHirelingId, nLevel + 1) > a3)
        {
            break;
        }
        ++nLevel;
    }

    D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, nHirelingId, nLevel);
    if (!pHirelingTxtRecord)
    {
        return a3;
    }
    
    const uint32_t nExperience = MONSTERS_GetHirelingExpForNextLevel(nLevel, pHirelingTxtRecord->dwExpPerLvl);
    if (nExperience <= a3)
    {
        return a3;
    }

    return nExperience;
}

//D2Game.0x6FC8C0C0) --------------------------------------------------------
int32_t __fastcall D2GAME_SAVE_ProcessSaveFile_6FC8C0C0(D2GameStrc* pGame, D2ClientStrc* pClient, uint8_t* pSaveFile, int32_t nSize, D2UnitStrc** ppPlayer, D2ActiveRoomStrc* pRoomArg, int32_t nXArg, int32_t nYArg)
{
    return 0;

//    D2ClientStrc* ptClient; // esi@1
//    D2GameStrc* ptGame; // edi@1
//    int result; // eax@2
//    int v11; // eax@6
//    D2UnitStrc* v12; // ebx@6
//    BYTE* v13; // ebp@6
//    D2BitBufferStrc** v14; // esi@8
//    BYTE* v15; // edi@8
//    BYTE* v16; // ebp@10
//    const void* v17; // ebp@11
//    BYTE* v18; // edi@12
//    signed int v19; // esi@12
//    D2PlayerDataStrc* v20; // eax@13
//    D2PlayerDataStrc* v21; // eax@13
//    BYTE* v22; // edi@13
//    D2GameStrc* v23; // edi@14
//    BYTE* v24; // ebp@14
//    int v25; // eax@19
//    int v26; // eax@25
//    char* v27; // esi@30
//    BYTE* v28; // ebp@30
//    D2PlayerDataStrc* v29; // eax@35
//    D2UnitStrc* v30; // edx@35
//    int v31; // esi@35
//    BYTE* v32; // ebp@35
//    int v33; // eax@36
//    int v34; // esi@41
//    int v35; // ebp@41
//    int v36; // ebp@42
//    int v37; // esi@42
//    DWORD v38; // ecx@44
//    WORD v39; // ax@45
//    char* v40; // eax@47
//    int v41; // eax@54
//    int v42; // ecx@54
//    int v43; // ST130_4@54
//    D2HirelingTxt* v44; // esi@54
//    D2UnitStrc* v45; // esi@60
//    int v46; // edx@62
//    int v47; // eax@62
//    int v48; // eax@62
//    int v49; // edx@62
//    int v50; // eax@62
//    D2HirelingTxt* v51; // eax@65
//    int v52; // eax@67
//    int v53; // eax@76
//    int v54; // ebp@77
//    D2ClientStrc* v55; // ecx@77
//    signed int v56; // esi@79
//    int v57; // edi@79
//    signed int v58; // eax@81
//    int v59; // esi@88
//    signed int v60; // esi@90
//    int v61; // eax@92
//    int v62; // eax@92
//    D2ClientStrc* v63; // esi@92
//    int v64; // [sp+148h] [bp-1A0h]@10
//    unsigned int v65; // [sp+14Ch] [bp-19Ch]@10
//    int a2; // [sp+150h] [bp-198h]@3
//    int nVersion; // [sp+154h] [bp-194h]@3
//    D2UnitStrc* pPlayer; // [sp+158h] [bp-190h]@1
//    int pClienta; // [sp+15Ch] [bp-18Ch]@3
//    int pGamea; // [sp+160h] [bp-188h]@3
//    int a7; // [sp+164h] [bp-184h]@3
//    D2GameStrc* ptGame2; // [sp+168h] [bp-180h]@1
//    BOOL bValid; // [sp+16Ch] [bp-17Ch]@3
//    int ptClient2; // [sp+170h] [bp-178h]@1
//    int v75[3]; // [sp+174h] [bp-174h]@3
//    BYTE v76[52]; // [sp+180h] [bp-168h]@11
//    BYTE v77[298]; // [sp+1BCh] [bp-12Ch]@6
//    char* v78; // [sp+2E8h] [bp+0h]@10
//
//    pPlayer = 0;
//    *ppPlayer = 0;
//
//    if (nSize < 130)
//    {
//        return 9;
//    }
//
//    result = sub_6FC8A780(pGame, pClient, pSaveFile, &pPlayer, &bValid, &nVersion, v75, &a7, &pClienta, &pGamea, &a2);
//    if (result)
//    {
//        return result;
//    }
//
//    if (bValid)
//    {
//        return sub_6FC8C890(pGame, pClient, ppPlayer, nUnused1, nUnused2, nUnused3);
//    }
//
//    v11 = a2;
//    v12 = pPlayer;
//    *ppPlayer = pPlayer;
//    v13 = &pSaveFile[v11];
//    memcpy(v77, &pSaveFile[v11], 298u);
//    if (*(uint32_t*)v77 != '!ooW' || *(uint32_t*)&v77[4] != 6)
//    {
//        return 2;
//    }
//
//    v14 = UNITS_GetPlayerData(v12)->pQuestData;
//    v15 = &v77[10];
//    pPlayer = (D2UnitStrc*)3;
//    do
//    {
//        QUESTRECORD_CopyBufferToRecord(*v14, v15, 0x60u, 1);
//        ++v14;
//        v15 += 96;
//        --a2;
//    }
//    while (a2);
//
//    v64 = 298;
//    v16 = v13 + 298;
//    v78 -= 298;
//
//    result = D2GAME_SAVE_ReadWaypointData_6FC8AD50(v12, v16, v65, &v64);
//    if (result)
//    {
//        return result;
//    }
//
//    v17 = &v16[v64];
//    memcpy(v76, v17, sizeof(v76));
//    v78 -= v64;
//    if (*(uint16_t*)v76 != 0x7701)
//    {
//        return 11;
//    }
//
//    v18 = &v76[28];
//    v19 = 96;
//    a2 = (int)&v76[28];
//    do
//    {
//        v20 = UNITS_GetPlayerData(v12);
//        PLRINTRO_CopyBufferToQuestIntroFlags(*(D2PlrIntroStrc**)&v20->szName[v19], v18 - 24);
//        v21 = UNITS_GetPlayerData(v12);
//        v22 = (BYTE*)a2;
//        PLRINTRO_CopyBufferToNpcIntroFlags(*(D2PlrIntroStrc**)&v21->szName[v19], (BYTE*)a2);
//        v19 += 4;
//        v18 = v22 + 8;
//        a2 = (int)v18;
//    }
//    while (v19 < 108);
//    v64 = 52;
//    v23 = (D2GameStrc*)pGamea;
//    v24 = (BYTE*)((char*)v17 + 52);
//    result = sub_6FC8ADE0((D2GameStrc*)pGamea, v12, v24, 16, v65, &v64);
//    if (result)
//    {
//        return result;
//    }
//
//    ptClient2 = STATLIST_UnitGetStatValue(v12, STAT_HITPOINTS, 0);
//    bValid = STATLIST_UnitGetStatValue(v12, STAT_MANA, 0);
//    pGamea = STATLIST_UnitGetStatValue(v12, STAT_GOLD, 0);
//    if (pGamea < 0 || (v25 = UNITS_GetInventoryGoldLimit(v12), pGamea > v25))
//    {
//        if (v12)
//        {
//            if (!v12->dwUnitType)
//                UNITS_GetInventoryGoldLimit(v12);
//            STATLIST_SetUnitStat(v12, STAT_GOLD, 0, 0);
//        }
//    }
//    pGamea = STATLIST_UnitGetStatValue(v12, STAT_GOLDBANK, 0);
//    if (pGamea < 0 || (v26 = UNITS_GetStashGoldLimit(v12), pGamea > v26))
//    {
//        if (v12)
//        {
//            if (!v12->dwUnitType)
//                UNITS_GetStashGoldLimit(v12);
//            STATLIST_SetUnitStat(v12, STAT_GOLDBANK, 0, 0);
//        }
//    }
//    v27 = &v78[-v64 - 52];
//    v28 = &v24[v64];
//    result = sub_6FC8AEC0(v23, pClient, v12, v28, pClienta, v65, &v64);
//    if (result)
//    {
//        return result;
//    }
//
//    ARENA_Return0();
//    if (nVersion)
//        D2GAME_AssignSkill_6FD13800(v12, 1, nVersion, -1);
//    if (pPlayer)
//        D2GAME_AssignSkill_6FD13800(v12, 0, (int)pPlayer, -1);
//    v29 = UNITS_GetPlayerData(v12);
//    v30 = pPlayer;
//    v29->nLeftSkillId = nVersion;
//    v29->nRightSkillId = (int)v30;
//    v31 = (int)&v27[-v64];
//    v32 = &v28[v64];
//    if (v65 == 71)
//    {
//        v33 = sub_6FC8B3D0(v23, v12, v32, 71u, v31, 0, &v64);
//    }
//    else
//    {
//        if (v65 < 72 || v65 > 96)
//            goto LABEL_89;
//        v33 = sub_6FC8B8A0(v23, v12, v32, v65, v31, 0, &v64);
//    }
//    if (!v33)
//    {
//        v34 = v31 - v64;
//        v35 = (int)&v32[v64];
//        if (!sub_6FC8BCC0(v23, v12, v35, v65, v34, &v64))
//        {
//            v36 = v64 + v35;
//            v37 = v34 - v64;
//            pGamea = v37;
//            if (v65 == 71)
//            {
//                if (*(uint16_t*)v36 == 'MJ')
//                {
//                    v38 = *(uint32_t*)(v36 + 2);
//                    if (v38)
//                    {
//                        v39 = *(uint16_t*)(v36 + 6);
//                        if (v39)
//                            D2GAME_MERCS_Create_6FCC8630(v23, v12, v39, v38, 0xFFFFu, 0, 0);
//                    }
//                    v40 = (char*)8;
//                    goto LABEL_77;
//                }
//            }
//            else
//            {
//                if ((unsigned int)v37 < 1)
//                {
//                    FOG_Trace("Premature end of save data.");
//                }
//                else
//                {
//                    if (!*(uint8_t*)v36)
//                    {
//                        v40 = (char*)1;
//LABEL_77:
//                        v54 = (int)&v40[v36];
//                        v55 = SUNIT_GetClientFromPlayer(
//                            v12,
//                            "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Game\\PLAYER\\PlrSave.cpp",
//                            5607);
//                        pClienta = (int)v55;
//                        if (v55 && v65 > 0x48)
//                        {
//                            v56 = 8;
//                            v57 = v54;
//                            while (1)
//                            {
//                                v58 = *(uint8_t*)v57;
//                                if (v58 < 156 || v58 > 216)
//                                {
//                                    if (v58 == 255)
//                                        LOWORD(v58) = -1;
//                                }
//                                else
//                                {
//                                    LOWORD(v58) = v58 + 65;
//                                }
//                                CLIENTS_SetSkillHotKey(v55, v56++, v58, *(uint8_t*)(v57 + 1), -1);
//                                v57 += 6;
//                                if (v56 >= 16)
//                                    break;
//                                v55 = (D2ClientStrc*)pClienta;
//                            }
//                            if (v65 > 0x51)
//                            {
//                                v59 = *(uint32_t*)(v54 + 52);
//                                UNITS_SetSwitchLeftSkill(v12, *(uint32_t*)(v54 + 48), -1);
//                                UNITS_SetSwitchRightSkill(v12, v59, -1);
//                            }
//                        }
//                        goto LABEL_89;
//                    }
//
//                    if ((unsigned int)v37 < 0x12)
//                    {
//                        FOG_Trace("Premature end of save data.");
//                    }
//                    else
//                    {
//                        v41 = *(uint32_t*)(v36 + 2);
//                        v42 = v23->bExpansion;
//                        a7 = v41 & 0x10000;
//                        v43 = *(uint16_t*)(v36 + 12);
//                        pPlayer = (D2UnitStrc*)(v36 + 18);
//                        v44 = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(v42, v43, 1);
//                        if (v44)
//                        {
//                            if (v65 <= 89)
//                                *(_DWORD*)(v36 + 14) = sub_6FC8C050(
//                                    v23,
//                                    *(_WORD*)(v36 + 12),
//                                    *(_DWORD*)(v36 + 14));
//                            *(_WORD*)(v36 + 10) += v44->wNameFirst;
//                            if (*(_WORD*)(v36 + 10) > v44->wNameLast)
//                                *(_WORD*)(v36 + 10) = v44->wNameFirst;
//                            v45 = D2GAME_MERCS_Create_6FCC8630(
//                                v23,
//                                v12,
//                                *(_WORD*)(v36 + 10),
//                                *(_DWORD*)(v36 + 6),
//                                *(_WORD*)(v36 + 12),
//                                v44->dwClass,
//                                a7);
//                            if (v45)
//                            {
//                                v46 = *(_WORD*)(v36 + 10);
//                                v47 = *(_WORD*)(v36 + 12);
//                                v75[0] = *(_DWORD*)(v36 + 6);
//                                v75[1] = v46;
//                                v75[2] = v47;
//                                PLAYERPETS_UpdatePetInfo(v12, 7, v45->dwUnitId, (int)v75);
//                                STATLIST_UnitGetStatValue(v12, 12, 0);
//                                v48 = DATATBLS_GetMaxLevel(0);
//                                v49 = *(_DWORD*)(v36 + 14);
//                                pClienta = v48;
//                                nVersion = 1;
//                                v64 = v49;
//                                v50 = STATLIST_UnitGetStatValue(v45, STAT_EXPERIENCE, 0);
//                                if (v50 < (unsigned int)v64)
//                                    STATLIST_SetUnitStat(v45, STAT_EXPERIENCE, v64, 0);
//                                D2GAME_MERCS_SendStat_6FCC61D0(v45, STAT_EXPERIENCE, v64);
//                                while (1)
//                                {
//                                    v51 = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(
//                                        v23->bExpansion,
//                                        *(_WORD*)(v36 + 12),
//                                        nVersion);
//                                    if (!v51)
//                                    {
//                                        FOG_DisplayAssert(
//                                            "ptMercStats",
//                                            "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Game\\PLAYER\\PlrSave.cpp",
//                                            5542);
//                                        exit(-1);
//                                    }
//                                    a2 = nVersion + 1;
//                                    if (nVersion + 1 > pClienta)
//                                        break;
//                                    v52 = MONSTERS_GetHirelingExpForNextLevel(
//                                        nVersion + 1,
//                                        v51->dwExpPerLvl);
//                                    if (v64 < (unsigned int)v52)
//                                        break;
//                                    nVersion = a2;
//                                }
//                                MONSTERAI_UpdateMercStatsAndSkills(v23, v12, v45, nVersion);
//                                if (!v45->pInventory)
//                                    INVENTORY_AllocInventory(0, v45);
//                                if (a7)
//                                {
//                                    sub_6FC7D470(v23, v12, v45);
//                                    v45->dwFlags |= UNITFLAG_ISDEAD;
//                                    SUNIT_SetCombatMode(v23, v45, MONMODE_DEAD);
//                                    MONSTER_DeleteEvents(v23, v45);
//                                }
//                                if (sub_6FC8BC70(
//                                    v23,
//                                    v45,
//                                    (BYTE*)pPlayer,
//                                    v65,
//                                    v36 + pGamea - (_DWORD)pPlayer,
//                                    0,
//                                    &pClienta))
//                                    goto LABEL_89;
//                                D2GAME_ITEMS_UpdateInventoryItems_6FC45050(v23, v45, 0, 0);
//                                //D2Game_10034_Return(0);
//                                v53 = STATLIST_GetMaxLifeFromUnit(v45);
//                                STATLIST_SetUnitStat(v45, STAT_HITPOINTS, v53, 0);
//                                MONSTERAI_SendMercStats(v23, v12, 1u);
//                                v40 = (char*)pPlayer + pClienta - v36;
//                            }
//                            else
//                            {
//                                v40 = (char*)pPlayer - v36;
//                            }
//                            goto LABEL_77;
//                        }
//
//                        FOG_Trace("Unable to load merc.");
//                    }
//                }
//            }
//        }
//    }
//
//LABEL_89:
//    STATLIST_SetUnitStat(v12, STAT_HITPOINTS, ptClient2, 0);
//    STATLIST_SetUnitStat(v12, STAT_MANA, bValid, 0);
//    STATLIST_SetUnitStat(v12, STAT_ATTACKRATE, 100, 0);
//    STATLIST_SetUnitStat(v12, STAT_VELOCITYPERCENT, 100, 0);
//    STATLIST_SetUnitStat(v12, STAT_OTHER_ANIMRATE, 100, 0);
//    if (v12)
//        v60 = v12->dwClassId;
//    else
//        v60 = -1;
//    v61 = STATLIST_GetUnitBaseStat(v12, STAT_LEVEL, 0);
//    v62 = DATATBLS_GetLevelThreshold(v60, v61);
//    STATLIST_SetUnitStat(v12, STAT_NEXTEXP, v62, 0);
//
//    if (CLIENTS_GetWeaponSwitch(pClient))
//    {
//        D2GAME_PACKETS_SendPacket0x97_6FC3FB60(pClient);
//    }
//
//    return 0;
}

//D2Game.0x6FC8C890
int32_t __fastcall sub_6FC8C890(D2GameStrc* pGame, D2ClientStrc* pClient, D2UnitStrc** ppPlayer, D2ActiveRoomStrc* pRoomArg, int32_t nXArg, int32_t nYArg)
{
    sub_6FC32220(pClient);
    D2UnitStrc* pPlayer = SUNIT_AllocUnitData(UNIT_PLAYER, CLIENTS_GetClassId(pClient), 0, 0, pGame, nullptr, 0, 0, 0);
    CLIENTS_SetPlayerInClient(pClient, pPlayer);
    SUNIT_InitClientInPlayerData(pPlayer, pClient);
    UNITS_SetNameInPlayerData(pPlayer, CLIENTS_GetName(pClient));
    SUNIT_InitSeed(pPlayer, &pGame->pGameSeed);
    UNITS_ChangeAnimMode(pPlayer, PLRMODE_NEUTRAL);

    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pPlayer);
    pPlayerData->pTrade = nullptr;
    pPlayerData->dwTradeState = 0;
    pPlayerData->dwAcceptTradeTick = 0;
    pPlayerData->unk0x54 = 0;

    D2GAME_SUNITMSG_FirstFn_6FCC5520(pGame, pPlayer, pClient);

    const uint8_t nAct = CLIENTS_GetActNo(pClient);
    PLAYERSTATS_SetStatsForStartingAct(pGame, pPlayer, nAct);
    PLAYER_CreateStartItemsFromCharStatsTxt(pPlayer, pGame, (void*)(uintptr_t)nAct);

    pPlayerData->nLeftSkillId = 0;
    pPlayerData->nRightSkillId = 0;

    if (!nAct && pPlayer)
    {
        D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pPlayer->dwClassId);
        if (pCharStatsTxtRecord && pCharStatsTxtRecord->wStartSkill && SKILLS_GetHighestLevelSkillFromUnitAndId(pPlayer, pCharStatsTxtRecord->wStartSkill))
        {
            D2GAME_AssignSkill_6FD13800(pPlayer, 0, pCharStatsTxtRecord->wStartSkill, -1);
            pPlayerData->nRightSkillId = pCharStatsTxtRecord->wStartSkill;
        }
    }

    QUESTS_SequenceCycler(pGame, pPlayer, 1);
    *ppPlayer = pPlayer;

    return 0;
}

//D2Game.0x6FC8C9D0
int32_t __fastcall D2GAME_SAVE_ReadFile_6FC8C9D0(D2GameStrc* pGame, D2ClientStrc* pClient, const char* szName, D2UnitStrc** ppPlayer, D2ActiveRoomStrc* pRoomArg, int32_t nXArg, int32_t nYArg)
{
    *ppPlayer = nullptr;

    char szPath[1024] = {};
    FOG_GetSavePath(szPath, sizeof(szPath));

    char szFileName[500] = {};
    sprintf_s(szFileName, "%s%s.d2s", szPath, szName);

    FILE* pSaveFile = fopen(szFileName, "rb"); // NOLINT(clang-diagnostic-deprecated-declarations)
    if (!pSaveFile)
    {
        return SYSERROR_UNK_14;
    }

    uint8_t saveFile[0x2000] = {};
    const uint32_t nFileSize = FileLockAndRead(saveFile, 1u, 0x2000u, pSaveFile);
    fclose(pSaveFile);
    
    if (!nFileSize)
    {
        return SYSERROR_UNK_14;
    }

    if (nFileSize < 8 || *(uint32_t*)saveFile != D2SMAGIC_HEADER)
    {
        return SYSERROR_UNK_9;
    }

    D2UnitStrc* pPlayer = nullptr;
    int32_t nResult = 0;
    if (*(uint32_t*)&saveFile[4] > 91u)
    {
        nResult = PLRSAVE2_ProcessSaveFile(pGame, pClient, saveFile, nFileSize, &pPlayer, pRoomArg, nXArg, nYArg);
    }
    else
    {
        nResult = D2GAME_SAVE_ProcessSaveFile_6FC8C0C0(pGame, pClient, saveFile, nFileSize, &pPlayer, pRoomArg, nXArg, nYArg);
    }

    if (!nResult)
    {
        *ppPlayer = pPlayer;
        if (!pPlayer)
        {
            nResult = SYSERROR_UNK_14;
        }
    }

    return nResult;
}

//D2Game.0x6FC8CB40
int32_t __fastcall D2GAME_SAVE_GetUnitDataFromFile_6FC8CB40(D2GameStrc* pGame, D2ClientStrc* pClient, const char* szName, int32_t a4, D2UnitStrc** ppPlayer, D2ActiveRoomStrc* pRoomArg, int32_t nXArg, int32_t nYArg)
{
    if (pGame->nGameType != 1 && pGame->nGameType != 2 && !gpD2EventCallbackTable_6FD45830)
    {
        if (a4)
        {
            D2UnitStrc* pPlayer = nullptr;
            const int32_t nErrorCode = D2GAME_SAVE_ReadFile_6FC8C9D0(pGame, pClient, szName, &pPlayer, pRoomArg, nXArg, nYArg);
            if (pPlayer)
            {
                QUESTS_SequenceCycler(pGame, pPlayer, 0);
            }

            *ppPlayer = pPlayer;
            if (nErrorCode)
            {
                return nErrorCode;
            }
        }
        else
        {
            const int32_t nErrorCode = sub_6FC8C890(pGame, pClient, ppPlayer, pRoomArg, nXArg, nYArg);
            if (nErrorCode)
            {
                return nErrorCode;
            }
        }
    }
    else
    {
        if (!a4)
        {
            const int32_t nErrorCode = sub_6FC8C890(pGame, pClient, ppPlayer, pRoomArg, nXArg, nYArg);
            if (nErrorCode)
            {
                return nErrorCode;
            }
        }
        else
        {
            *ppPlayer = nullptr;
            D2SaveHeaderStrc* pSaveHeader = CLIENTS_GetSaveHeader(pClient);
            const uint32_t nSaveHeaderSize = CLIENTS_GetSaveHeaderSize(pClient);
            if (!nSaveHeaderSize)
            {
                return SYSERROR_UNK_14;
            }

            if (nSaveHeaderSize < 8)
            {
                *ppPlayer = nullptr;
                return SYSERROR_UNK_9;
            }

            if (pSaveHeader->dwHeaderMagic != D2SMAGIC_HEADER)
            {
                *ppPlayer = nullptr;
                return SYSERROR_UNK_9;
            }

            int32_t nErrorCode = 0;
            if (pSaveHeader->dwVersion > 91)
            {
                nErrorCode = PLRSAVE2_ProcessSaveFile(pGame, pClient, (uint8_t*)pSaveHeader, nSaveHeaderSize, ppPlayer, pRoomArg, nXArg, nYArg);
            }
            else
            {
                nErrorCode = D2GAME_SAVE_ProcessSaveFile_6FC8C0C0(pGame, pClient, (uint8_t*)pSaveHeader, nSaveHeaderSize, ppPlayer, pRoomArg, nXArg, nYArg);
            }

            if (nErrorCode)
            {
                *ppPlayer = nullptr;
                return nErrorCode;
            }

            if (!*ppPlayer)
            {
                return SYSERROR_UNK_14;
            }

            if (pGame->nGameType == 1 || pGame->nGameType == 2)
            {
                CLIENTS_FreeSaveHeader(pClient);
            }

            QUESTS_SequenceCycler(pGame, *ppPlayer, 0);
        }
    }

    if (!*ppPlayer)
    {
        return SYSERROR_UNK_14;
    }

    sub_6FD14C30(*ppPlayer);
    return 0;
}
