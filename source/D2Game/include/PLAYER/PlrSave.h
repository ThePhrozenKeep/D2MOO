#pragma once

#include <Units/Units.h>
#include "PlrSave2.h"

#pragma pack(push, 1)
struct D2SavedItemStrc
{
    uint8_t nBodyLoc;
    uint8_t nSocketedItems;
    int32_t nItemLevel;
    int32_t nItemId;
    uint8_t nSpawnType;
    uint8_t nItemQuality;
    int32_t nQuantity;
    int32_t nMinDurability;
    int32_t nMaxDurability;
    int32_t nX;
    int32_t nY;
    int32_t nItemIndex;
    int32_t nItemFlags;
    int32_t nSeed;
    int32_t nItemSeed;
    uint8_t nInvPage;
    uint8_t nEarLevel;
    char szName[16];
    uint16_t nItemFormat;
    uint32_t nItemCode;
};
#pragma pack(pop)


//D2Game.0x6FC895D0
void __fastcall D2GAME_10036_PLRSAVE_EnableSaveFileWriting(int32_t bWriteSaveFile);
//D2Game.0x6FC895E0
int32_t __fastcall PLRSAVE_CreateAndPlaceLegacyPlayerItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2SavedItemStrc* pSavedItem, D2UnitStrc** ppItem, int32_t* pSocketedItemCount);
//D2Game.0x6FC897F0
D2UnitStrc* __fastcall PLRSAVE_CreateItemFromSavedData(D2GameStrc* pGame, D2SavedItemStrc* pSavedItem);
//D2Game.0x6FC898F0
int32_t __fastcall PLRSAVE_PlacePlayerItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2UnitStrc* pSocketParent);
//D2Game.0x6FC89AB0
int32_t __stdcall D2GAME_10029_SAVE_WriteFileInterface(D2GameStrc* pGame, D2UnitStrc* pPlayer, const char* szName);
//D2Game.0x6FC89AD0
int32_t __stdcall D2GAME_SAVE_SerializeItem_6FC89AD0(D2UnitStrc* pItem, uint8_t* pBitstream, uint32_t nBitstreamSize, int32_t nUnused1, int32_t nUnused2);
//D2Game.0x6FC89B50
int32_t __stdcall PLRSAVE_WriteItems_6FC89B50(D2InventoryStrc* pInventory, uint8_t* pBitstream, uint32_t nBitstreamSize, int32_t nUnused1, int32_t nUnused2);
//D2Game.0x6FC8A0F0
int32_t __fastcall PLRSAVE_WriteUnitItems(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t* pSection, int32_t nSize, int32_t bUseTradeInventory, int32_t nUnused);
//D2Game.0x6FC8A140
int32_t __fastcall D2GAME_SAVE_CalculateChecksum_6FC8A140(D2SaveHeaderStrc* pSaveHeader, int32_t nSize);
//D2Game.0x6FC8A1B0
int32_t __fastcall D2GAME_SAVE_WriteFileOnRealm_6FC8A1B0(D2GameStrc* pGame, D2UnitStrc* pPlayer, const char* szCharName, char* szAccountName, int32_t bInteractsWithPlayer, int32_t a6, int32_t nSaveParam, int32_t nRealmId);
//D2Game.0x6FC8A500
int32_t __fastcall D2GAME_SAVE_WriteFile_6FC8A500(D2GameStrc* pGame, D2UnitStrc* pPlayer, const char* szName, DWORD dwArg);
//D2Game.0x6FC8A780
int32_t __fastcall PLRSAVE_ReadLegacyHeader(D2GameStrc* pGame, D2ClientStrc* pClient, uint8_t* pSavefile, D2UnitStrc** ppPlayer, int32_t* pValid, int32_t* pVersion, int32_t* pStatCount, int32_t* pSkillCount, int32_t* pLeftSkillId, int32_t* pRightSkillId, int32_t* pHeaderSize);
//D2Game.0x6FC8AD50
int32_t __fastcall D2GAME_SAVE_ReadWaypointData_6FC8AD50(D2UnitStrc* pUnit, uint8_t* pSection, int32_t nUnused, int32_t* pSize);
//D2Game.0x6FC8ADE0
int32_t __fastcall PLRSAVE_ReadLegacyStats(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t* pSection, int32_t nStatCount, int32_t nUnused, int32_t* pSize);
//D2Game.0x6FC8AEC0
int32_t __fastcall PLRSAVE_ReadLegacySkills(D2GameStrc* pGame, D2ClientStrc* pClient, D2UnitStrc* pUnit, BYTE* pSection, int32_t nUnused1, int32_t nUnused2, int32_t* pSkillCount);
//D2Game.0x6FC8AF70
size_t __fastcall PLRSAVE_ReadLegacyItemData(void* a1, BYTE* a2);
//D2Game.0x6FC8B3D0
int32_t __fastcall PLRSAVE_ReadLegacyItems(D2GameStrc* pGame, D2UnitStrc* pPlayer, BYTE* pSection, uint32_t dwVersion, int32_t nSize, int32_t bCorpseItems, int32_t* pSize);
//D2Game.0x6FC8B680
int32_t __fastcall PLRSAVE_CreateAndPlaceLegacyCorpseItem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SavedItemStrc* pSavedItem, D2UnitStrc** ppItem, int32_t* pSocketedItemCount);
//D2Game.0x6FC8B8A0
int32_t __fastcall PLRSAVE_ReadItems_6FC8B8A0(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t* pSection, uint32_t dwVersion, int32_t nSize, int32_t bCorpseItems, int32_t* pSize);
//D2Game.0x6FC8BAA0
int32_t __fastcall PLRSAVE_PlaceCorpseItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2UnitStrc* pSocketParent);
//D2Game.0x6FC8BC70
int32_t __fastcall PLRSAVE_ReadItemSection(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t* pSection, uint32_t dwVersion, int32_t nSize, int32_t bCorpseItems, int32_t* pSize);
//D2Game.0x6FC8BCC0
int32_t __fastcall PLRSAVE_ReadCorpseSection(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t* pSection, uint32_t dwVersion, int32_t nRemainingSize, int32_t* pSize);
//D2Game.0x6FC8BEE0
int32_t __fastcall PLRSAVE_GetLegacyHirelingExpFactor(int16_t nHirelingId, int32_t nLevel);
//D2Game.0x6FC8C050
uint32_t __fastcall PLRSAVE_ConvertLegacyHirelingExperience(D2GameStrc* pGame, int16_t nHirelingId, uint32_t nSavedExperience);
//D2Game.0x6FC8C0C0
int32_t __fastcall D2GAME_SAVE_ProcessSaveFile_6FC8C0C0(D2GameStrc* pGame, D2ClientStrc* pClient, uint8_t* pSaveFile, int32_t nSize, D2UnitStrc** ppPlayer, int32_t nUnused1, int32_t nUnused2, int32_t nUnused3);
//D2Game.0x6FC8C890
int32_t __fastcall PLRSAVE_CreateNewPlayer(D2GameStrc* pGame, D2ClientStrc* pClient, D2UnitStrc** ppPlayer, int32_t a4, int32_t a5, int32_t a6);
//D2Game.0x6FC8C9D0
int32_t __fastcall D2GAME_SAVE_ReadFile_6FC8C9D0(D2GameStrc* pGame, D2ClientStrc* pClient, const char* szName, D2UnitStrc** ppPlayer, DWORD dw1, DWORD dw2, DWORD dw3);
//D2Game.0x6FC8CB40
int32_t __fastcall D2GAME_SAVE_GetUnitDataFromFile_6FC8CB40(D2GameStrc* pGame, D2ClientStrc* pClient, const char* szName, int32_t bLoadSave, D2UnitStrc** ppPlayer, D2ActiveRoomStrc* pRoomArg, int32_t nXArg, int32_t nYArg);
