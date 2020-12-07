#pragma once

#include "CommonDefinitions.h"

#include "D2Math.h"


extern D2ArenaTxt* gpArenaTxtTable;
extern D2CharTemplateTxt* gpCharTemplateTxtTable;
extern int gnCharTemplateTxtTableRecordCount;
extern DWORD gnCharTemplateStartIds[64];
extern D2BeltsTxt* gpBeltsTxtTable;
extern D2DataTablesStrc gpDataTables;
extern D2DataTablesStrc* sgptDataTables;


extern D2SeedStrc* gpAutomapSeed;
extern LPCRITICAL_SECTION gpLvlSubTypeFilesCriticalSection;

//TODO: Reimport defs from .cpps


//D2Common.0x6FDC412C
void __fastcall DATATBLS_CloseFileInMPQ(void* pMemPool, void* pFileHandle);
//D2Common.0x6FDC40F0
BOOL __fastcall DATATBLS_CheckIfFileExists(void* pMemPool, char* szFileName, void** pFileHandle, int bDontLogError);
//D2Common.0x6FDC45EE
size_t __cdecl DATATBLS_LockAndWriteToFile(const void* Str, size_t Size, size_t Count, FILE* File);
//D2Common.0x6FDC41C1
BOOL __fastcall DATATBLS_ReadFromFile(void* pMemPool, void* pFileHandle, void* pBuffer, size_t nBytesToRead);
//D2Common.0x6FDC4152
size_t __fastcall DATATBLS_GetFileSize(void* pMemPool, void* pFileHandle, DWORD* lpFileSizeHigh);
//D2Common.0x6FDC4268
void* __fastcall DATATBLS_GetBinaryData(void* pMemPool, char* szFileName, int* pSize, char* szFile, int nLine);



//D2Common.0x6FD473C0
D2AnimDataStrc* __fastcall DATATBLS_LoadAnimDataD2(void* pMemPool);
//D2Common.0x6FD47460
void __fastcall DATATBLS_UnloadAnimDataD2(D2AnimDataStrc* pAnimData);
//D2Common.0x6FD474A0
D2AnimDataRecordStrc* __fastcall DATATBLS_GetAnimDataRecord(D2UnitStrc* pUnit, int nClassId, int nMode, int nUnitType, D2InventoryStrc* pInventory);
//D2Common.0x6FD475D0 (#10640)
void __stdcall D2Common_10640(D2UnitStrc* pUnit, int nAnimSpeed);
//D2Common.0x6FD47700 (#10641)
BOOL __stdcall D2Common_10641(char* szPath, int* pLength, int* pOutAnimSpeed, int* pOutFirstFrameTagged);



//D2Common.0x6FD47840
void __fastcall DATATBLS_LoadArenaTxt(void* pMemPool);
//D2Common.0x6FD47970 (#10596)
D2ArenaTxt* __fastcall DATATBLS_GetArenaTxtRecord(int nArenaType);
//D2Common.0x6FD479B0
void __fastcall DATATBLS_UnloadArenaTxt();
//D2Common.0x6FD479D0
void __fastcall DATATBLS_LoadCharTemplateTxt(void* pMemPool);
//D2Common.0x6FD48770
void __fastcall DATATBLS_UnloadCharTemplateTxt();
//D2Common.0x6FD48790 (#10664)
int __fastcall DATATBLS_GetCharTemplateTxtRecordCount();
//D2Common.0x6FD487A0 (#10665)
D2CharTemplateTxt* __fastcall DATATBLS_GetCharTemplateTxtRecord(int nTemplate, int nLevel);
//D2Common.0x6FD48810 (#10666)
int __fastcall DATATBLS_GetClassFromCharTemplateTxtRecord(int nTemplate, int nLevel);



//D2Common.0x6FD48880
void __fastcall DATATBLS_LoadBeltsTxt(void* pMemPool);
//D2Common.0x6FD493A0
void __fastcall DATATBLS_UnloadBeltsTxt();
//D2Common.0x6FD493B0 (#10638)
void __stdcall DATATBLS_GetBeltsTxtRecord(int nIndex, int bHigherRes, D2BeltsTxt* pRecord);
//D2Common.0x6FD49420 (#10639)
void __stdcall DATATBLS_GetInvRectFromBeltsTxt(int nIndex, int bHigherRes, D2InvRectStrc* pInvRect, int nBoxId);



//D2Common.0x6FD494D0
WORD __fastcall DATATBLS_GetStringIdFromReferenceString(char* szReference);
//D2Common.0x6FD49500 - Changed this function a lot (had 6 hardcoded (i.e. pre-defined) Args)
void __fastcall DATATBLS_InitUnicodeClassNamesInCharStatsTxt();
//D2Common.0x6FD49660 (#11255)
DWORD __stdcall DATATBLS_GetCodeFromCompCodeTxt(int nCompCode);
//D2Common.0x6FD49680 (#11249)
DWORD __stdcall DATATBLS_GetExpRatio(int nLevel);
//D2Common.0x6FD496B0 (#10628)
DWORD __stdcall DATATBLS_GetLevelThreshold(int nClass, DWORD dwLevel);
//D2Common.0x6FD496E0 (#10629)
int __stdcall DATATBLS_GetMaxLevel(int nClass);
//D2Common.0x6FD49710 (#10630)
DWORD __stdcall DATATBLS_GetCurrentLevelFromExp(int nClass, DWORD dwExperience);
//D2Common.0x6FD49760
void __fastcall DATATBLS_GetBinFileHandle(void* pMemPool, char* szFile, void** ppFileHandle, int* pSize, int* pSizeEx);
//D2Common.0x6FD49850
int __fastcall DATATBLS_AppendMemoryBuffer(char** ppCodes, int* pSize, int* pSizeEx, char* pBuffer, int nBufferSize);



//D2Common.0x6FD498D0
int __fastcall DATATBLS_MapSkillsTxtKeywordToNumber(char* szKey);
//D2Common.0x6FD49980
int __fastcall sub_6FD49980(int nValue);
//D2Common.0x6FD49990
int __fastcall sub_6FD49990(char* szText, int* a2, int a3, int nKeywordNumber);
//D2Common.0x6FD49D20
void __fastcall DATATBLS_SkillCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD49E40
void __fastcall DATATBLS_SkillDescCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD49F60
void __fastcall DATATBLS_LoadSkills_SkillDescTxt(void* pMemPool);
//D2Common.0x6FD4E350
void __fastcall DATATBLS_UnloadSkills_SkillDescTxt();
//Inlined at various places
D2SkillsTxt* __fastcall DATATBLS_GetSkillsTxtRecord(int nSkillId);
//Inlined at various places
D2SkillDescTxt* __fastcall DATATBLS_GetSkillDescTxtRecord(int nSkillDesc);



//D2Common.0x6FD4E4B0 (#10593)
D2CharStatsTxt* __fastcall DATATBLS_GetCharstatsTxtTable();
//D2Common.0x6FD4E4C0
D2AnimDataStrc* __fastcall DATATBLS_GetAnimData();
//D2Common.0x6FD4E4D0 (#10655)
D2DifficultyLevelsTxt* __stdcall DATATBLS_GetDifficultyLevelsTxtRecord(int nDifficulty);
//D2Common.0x6FD4E500
void __fastcall DATATBLS_LoadStatesTxt(void* pMemPool);
//D2Common.0x6FD4F4A0
void __fastcall DATATBLS_UnloadStatesTxt();
//D2Common.0x6FD4F5A0
void __fastcall DATATBLS_LoadPetTypeTxt(void* pMemPool);
//D2Common.0x6FD4F990 (#11298)
char* __stdcall DATATBLS_GetUnitNameFromUnit(D2UnitStrc* pUnit, char* szName);
//D2Common.0x6FD4FB50 (#11299)
char* __stdcall DATATBLS_GetUnitNameFromUnitTypeAndClassId(int nUnitType, int nClassId, char* szName);
//D2Common.0x6FD4FCF0 (#10580)
void __stdcall DATATBLS_WriteBinFile(char* szFileName, void* pWriteBuffer, size_t nBufferSize, int nRecordCount);
//D2Common.0x6FD4FD70 (#10578)
void* __stdcall DATATBLS_CompileTxt(void* pMemPool, char* szName, D2BinFieldStrc* pTbl, int* pRecordCount, int nSize);
//D2Common.0x6FD500F0 (#11242)
void __stdcall DATATBLS_ToggleCompileTxtFlag(BOOL bSilent);
//D2Common.0x6FD50110 (#10579)
void __stdcall DATATBLS_UnloadBin(void* pBinFile);
//D2Common.0x6FD50150 (#10575)
void __stdcall DATATBLS_UnloadAllBins();
//D2Common.0x6FD504B0 (#10576)
void __stdcall DATATBLS_LoadAllTxts(void* pMemPool, int a2, int a3);
//D2Common.0x6FD507B0
void __fastcall DATATBLS_LoadSomeTxts(void* pMemPool);
//D2Common.0x6FD50FB0
void __fastcall DATATBLS_LoadCharStatsTxt(void* pMemPool);
//D2Common.0x6FD51BF0
void __fastcall DATATBLS_LoadDifficultyLevelsTxt(void* pMemPool);



//D2Common.0x6FD51FC0
BOOL __fastcall DATATBLS_LoadExpFieldD2(void* pMemPool);
//D2Common.0x6FD52010 (#11089)
BOOL __stdcall DATATBLS_InitializeCollisionFieldTable(char* pExpField, int nSize);
//D2Common.0x6FD520F0 (#11090)
BOOL __stdcall DATATBLS_FreeCollisionFieldTable();
//D2Common.0x6FD52140 (#11091)
void __stdcall DATATBLS_GetCollisionFieldCenter(int* a1, int* a2);
//D2Common.0x6FD52160 (#11092)
int __stdcall DATATBLS_GetCollisionFieldWidth();
//D2Common.0x6FD52170 (#11093)
int __stdcall DATATBLS_GetCollisionFieldHeight();
//D2Common.0x6FD52180 (#11094)
D2FieldStrc* __stdcall DATATBLS_AllocField();
//D2Common.0x6FD521D0 (#11095)
void __stdcall DATATBLS_FreeField(D2FieldStrc* pField);
//D2Common.0x6FD52210 (#11096)
void __stdcall DATATBLS_SetFieldCoordinates(D2FieldStrc* pField, int nX, int nY);
//D2Common.0x6FD52250 (#11097)
int __stdcall D2Common_11097(D2FieldStrc* pField, int a2, int a3);
//D2Common.0x6FD522A0 (#11098)
int __stdcall D2Common_11098(D2FieldStrc* pField, int* pX, int* pY);
//D2Common.0x6FD52360 (#11099)
BOOL __stdcall D2Common_11099(D2FieldStrc* pField, D2RoomStrc* pRoom, int nX, int nY, WORD fMask);



//D2Common.0x6FD523E0
void __fastcall DATATBLS_CubeMainInputLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD52410
BOOL __fastcall DATATBLS_CubeMainInputParser(D2CubeInputItem* pCubeInput, char* szInput, int a3, int a4);
//D2Common.0x6FD528D0
void __fastcall DATATBLS_CubeMainOutputLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD52910
BOOL __fastcall DATATBLS_CubeMainOutputParser(D2CubeOutputItem* pCubeOutputParam, char* szOutput, int nTxtRow, int nItemId);
//D2Common.0x6FD52FC0
void __fastcall DATATBLS_CubeMainParamLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD53030
void __fastcall DATATBLS_LoadCubeMainTxt(void* pMemPool);
//D2Common.0x6FD54250
void __fastcall DATATBLS_UnloadCubeMainTxt();
//D2Common.0x6FD54260 (#11232)
D2CubeMainTxt* __stdcall DATATBLS_GetCubemainTxtRecord(int nIndex);
//D2Common.0x6FD542C0 (#11233)
int __fastcall DATATBLS_GetCubemainTxtRecordCount();



//D2Common.0x6FD542D0
void __fastcall DATATBLS_LoadInventoryTxt(void* pMemPool);
//D2Common.0x6FD54F10
void __fastcall DATATBLS_UnloadInventoryTxt();
//D2Common.0x6FD54F20 (#10635)
void __stdcall DATATBLS_GetInventoryRect(int nInventoryTxtId, int bHigherRes, D2InvRectStrc* pInvRect);
//D2Common.0x6FD54FB0 (#10636)
void __stdcall DATATBLS_GetInventoryGridInfo(int nInventoryTxtId, int bHigherRes, D2InventoryGridInfoStrc* pInventoryGridInfo);
//D2Common.0x6FD55030 (#10637)
void __stdcall DATATBLS_GetInventoryComponentGrid(int nInventoryTxtId, int bHigherRes, D2InvCompGridStrc* pInvCompGrid, int nComponent);
//D2Common.0x6FD550E0
int __fastcall DATATBLS_MapItemsTxtKeywordToNumber(char* szKey);
//D2Common.0x6FD55140
int __fastcall DATATBLS_Return2();
//D2Common.0x6FD55150
int __fastcall sub_6FD55150(char* szText, int* a2, int a3, int nKeywordNumber);
//D2Common.0x6FD551E0
void __fastcall DATATBLS_ItemCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD55280
void __fastcall DATATBLS_LoadItemsTxt(void* pMemPool);
//D2Common.0x6FD575D0
void __fastcall DATATBLS_UnloadItemsTxt();
//D2Common.0x6FD57620 (#10599)
D2ItemDataTbl* __stdcall DATATBLS_GetItemDataTables();
//D2Common.0x6FD57630 (#10597)
int __stdcall DATATBLS_MapOldItemIndexToCurrent(int nItemId);
//D2Common.0x6FD57680 (#10600)
D2ItemsTxt* __stdcall DATATBLS_GetItemsTxtRecord(int nItemId);
//D2Common.0x6FD576D0 (#10601)
D2ItemsTxt* __stdcall DATATBLS_GetItemRecordFromItemCode(DWORD dwCode, int* pItemId);
//D2Common.0x6FD57720 (#10602)
int __stdcall DATATBLS_GetItemIdFromItemCode(DWORD dwCode);
//D2Common.0x6FD57740
void __fastcall DATATBLS_ItemParamLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD57820
void __fastcall DATATBLS_LoadMagicSuffix_Prefix_AutomagicTxt(void* pMemPool);
//D2Common.0x6FD58080
void __fastcall DATATBLS_UnloadMagicSuffix_Prefix_AutomagicTxt();
//D2Common.0x6FD580B0 (#10603)
D2MagicAffixDataTbl* __stdcall DATATBLS_GetMagicAffixDataTables();
//D2Common.0x6FD580C0 (#10604)
D2MagicAffixTxt* __stdcall DATATBLS_GetMagicAffixTxtRecord(int nIndex);
//D2Common.0x6FD58110
void __fastcall DATATBLS_LoadRareSuffix_PrefixTxt(void* pMemPool);
//D2Common.0x6FD58450
void __fastcall DATATBLS_UnloadRareSuffix_PrefixTxt();
//D2Common.0x6FD58480 (#10605)
D2RareAffixDataTbl* __fastcall DATATBLS_GetRareAffixDataTables();
//D2Common.0x6FD58490 (#10606)
D2RareAffixTxt* __stdcall DATATBLS_GetRareAffixTxtRecord(int nId);
//D2Common.0x6FD584E0
void __fastcall DATATBLS_LoadUniqueItemsTxt(void* pMemPool);
//D2Common.0x6FD59110
void __fastcall DATATBLS_UnloadUniqueItemsTxt();
//D2Common.0x6FD59140
void __fastcall DATATBLS_LoadSets_SetItemsTxt(void* pMemPool);
//D2Common.0x6FD5AE00
void __fastcall DATATBLS_UnloadSets_SetItemsTxt();
//D2Common.0x6FD5AE40
void __fastcall DATATBLS_LoadQualityItemsTxt(void* pMemPool);
//D2Common.0x6FD5B250
void __fastcall DATATBLS_UnloadQualityItemsTxt();
//D2Common.0x6FD5B260 (#10611)
D2QualityItemDataTbl* __fastcall DATATBLS_GetQualityItemDataTables();
//D2Common.0x6FD5B270 (#10612)
D2QualityItemsTxt* __stdcall DATATBLS_GetQualityItemsTxtRecord(int nIndex);
//D2Common.0x6FD5B2F0
void __fastcall DATATBLS_LoadGemsTxt(void* pMemPool);
//D2Common.0x6FD5BAE0
void __fastcall DATATBLS_UnloadGemsTxt();
//D2Common.0x6FD5BAF0 (#10615)
D2GemDataTbl* __fastcall DATATBLS_GetGemDataTables();
//D2Common.0x6FD5BB00 (#10616)
D2GemsTxt* __stdcall DATATBLS_GetGemsTxtRecord(int nGemId);
//D2Common.0x6FD5BB70
void __fastcall DATATBLS_LoadBooksTxt(void* pMemPool);
//D2Common.0x6FD5BD10
void __fastcall DATATBLS_UnloadBooksTxt();
//D2Common.0x6FD5BD20 (#10617)
D2BookDataTbl* __fastcall DATATBLS_GetBookDataTables();
//D2Common.0x6FD5BD30 (#10618)
D2BooksTxt* __stdcall DATATBLS_GetBooksTxtRecord(int nBookId);
//D2Common.0x6FD5BDA0
void __fastcall DATATBLS_LoadLowQualityItemsTxt(void* pMemPool);
//D2Common.0x6FD5BE40
void __fastcall DATATBLS_UnloadLowQualityItemsTxt();
//D2Common.0x6FD5BE50 (#10613)
D2LowQualityItemDataTbl* __fastcall DATATBLS_GetLowQualityItemDataTables();
//D2Common.0x6FD5BE60 (#10614)
D2LowQualityItemsTxt* __stdcall DATATBLS_GetLowQualityItemsTxtRecord(int nId);
//D2Common.0x6FD5BED0
void __fastcall DATATBLS_LoadItemRatioTxt(void* pMemPool);
//D2Common.0x6FD5C200
void __fastcall DATATBLS_UnloadItemRatioTxt();
//D2Common.0x6FD5C210 (#10622)
D2ItemRatioDataTbl* __fastcall DATATBLS_GetItemRatioDataTables();
//D2Common.0x6FD5C220 (#10623)
D2ItemRatioTxt* __stdcall DATATBLS_GetItemRatioTxtRecord(int nItemId, BYTE nDifficulty, WORD wVersion);
//D2Common.0x6FD5C2F0
int __cdecl DATATBLS_CompareItemStatCostDescs(const void* pRecord1, const void* pRecord2);
//D2Common.0x6FD5C320
void __fastcall DATATBLS_LoadItemStatCostTxt(void* pMemPool);
//D2Common.0x6FD5D070
void __fastcall DATATBLS_UnloadItemStatCostTxt();
//D2Common.0x6FD5D0D0
void __fastcall DATATBLS_LoadPropertiesTxt(void* pMemPool);
//D2Common.0x6FD5D5E0
void __fastcall DATATBLS_UnloadPropertiesTxt();
//D2Common.0x6FD5D600
void __fastcall DATATBLS_LoadGambleTxt(void* pMemPool);
//D2Common.0x6FD5D790
int __cdecl DATATBLS_CompareGambleTxtRecords(const void* pRecord1, const void* pRecord2);
//D2Common.0x6FD5D7B0
void __fastcall DATATBLS_UnloadGambleTxt();
//D2Common.0x6FD5D7F0 (#10671)
D2GambleDataTbl* __fastcall DATATBLS_GetGambleDataTables();
//D2Common.0x6FD5D800
BOOL __fastcall DATATBLS_CheckNestedItemTypes(int nItemType1, int nItemType2);
//D2Common.0x6FD5D8C0
void __fastcall DATATBLS_LoadItemTypesTxt(void* pMemPool);
//D2Common.0x6FD5DFE0
void __fastcall DATATBLS_UnloadItemTypesTxt();
//D2Common.0x6FD5E020
void __fastcall DATATBLS_LoadRunesTxt(void* pMemPool);
//D2Common.0x6FD5E9C0
void __fastcall DATATBLS_UnloadRunesTxt();
//D2Common.0x6FD5E9E0 (#10619)
D2RuneDataTbl* __fastcall DATATBLS_GetRuneDataTables();
//D2Common.0x6FD5E9F0 (#10621)
void __stdcall DATATBLS_AddOrChangeRunesTxtRecord(int nRecordId, D2RunesTxt* pRecord);
//D2Common.0x6FD5EAA0 (#10620)
D2RunesTxt* __stdcall DATATBLS_GetRunesTxtRecord(int nRunewordId);



//D2Common.0x6FD5EAE0
void __fastcall DATATBLS_LoadLevelsTxt(void* pMemPool);
//D2Common.0x6FD603C0 (#10631)
D2LevelsTxt* __stdcall DATATBLS_GetLevelsTxtRecord(int nLevelId);
//D2Common.0x6FD603F0 (#10632)
BYTE __stdcall DATATBLS_GetRainFromLevelsTxt(int nLevelId);
//D2Common.0x6FD60430 (#10634)
BYTE __stdcall DATATBLS_GetNoPerFromLevelsTxt(int nLevelId);
//D2Common.0x6FD60470 (#10633)
BYTE __stdcall DATATBLS_GetMudFromLevelsTxt(int nLevelId);
//D2Common.0x6FD604B0
void __fastcall DATATBLS_UnloadLevelsTxt();
//D2Common.0x6FD604F0 (#11247)
int __stdcall DATATBLS_GetMonsterLevelInArea(int nLevelId, BYTE nDifficulty, BOOL bExpansion);
//D2Common.0x6FD60560
int* __fastcall DATATBLS_GetPortalLevels(int* pnPortalLevels);
//D2Common.0x6FD60570
void __fastcall DATATBLS_LoadLevelDefsBin(void* pMemPool);
//D2Common.0x6FD60D60
void __fastcall DATATBLS_UnloadLevelDefsBin();
//D2Common.0x6FD60D90 (#10010)
D2LevelDefBin* __fastcall DATATBLS_GetLevelDefRecord(int nLevelId);
//D2Common.0x6FD60DC0
void __fastcall DATATBLS_LoadLevelTypesTxt(void* pMemPool);
//D2Common.0x6FD61450
void __fastcall DATATBLS_UnloadLevelTypesTxt();
//D2Common.0x6FD61460 (#10023)
D2LvlTypesTxt* __stdcall DATATBLS_GetLevelTypesTxtRecord(int nLevelType);
//D2Common.0x6FD614A0 (#11226)
bool __stdcall DATATBLS_CheckActInLevelTypesTxt(int nLevelType, BYTE nAct);
//D2Common.0x6FD61500 (#11227)
void __stdcall DATATBLS_GetFileNameFromLevelTypeAndFileId(int nLevelType, int nFile, char* szFile);
//D2Common.0x6FD61570
void __fastcall DATATBLS_LoadLvlPrestTxt(void* pMemPool, int a2);
//D2Common.0x6FD61AD0
void __fastcall DATATBLS_UnloadLvlPrestTxt();
//D2Common.0x6FD61B50 (#10024)
D2LvlPrestTxt* __stdcall DATATBLS_GetLvlPrestTxtRecord(int nId);
//D2Common.0x6FD61B80
D2LvlPrestTxt* __fastcall DATATBLS_GetLvlPrestTxtRecordFromLevelId(int nLevelId);
//D2Common.0x6FD61BC0
void __fastcall DATATBLS_LoadLvlWarpTxt(void* pMemPool);
//D2Common.0x6FD61DD0
void __fastcall DATATBLS_UnloadLvlWarpTxt();
//D2Common.0x6FD61DE0
D2LvlWarpTxt* __fastcall DATATBLS_GetLvlWarpTxtRecordFromLevelIdAndDirection(int nLevelId, char szDirection);
//D2Common.0x6FD61E60
void __fastcall DATATBLS_LoadLvlMazeTxt(void* pMemPool);
//D2Common.0x6FD61F90
void __fastcall DATATBLS_UnloadLvlMazeTxt();
//D2Common.0x6FD61FA0
D2LvlMazeTxt* __fastcall DATATBLS_GetLvlMazeTxtRecordFromLevelId(int nLevelId);
//D2Common.0x6FD62020
void __fastcall DATATBLS_LoadLvlSubTxt(void* pMemPool, int a2, int a3);
//D2Common.0x6FD62600
void __fastcall DATATBLS_UnloadLvlSubTxt();
//D2Common.0x6FD626F0
D2LvlSubTxt* __fastcall DATATBLS_GetLvlSubTxtRecord(int nSubType);
//D2Common.0x6FD62780
void __fastcall DATATBLS_AllocGlobalTileLibraryHash();
//D2Common.0x6FD628A0
void __fastcall DATATBLS_FreeGlobalTileLibraryHash();
//D2Common.0x6FD628C0
void __fastcall DATATBLS_LoadAutomapTxt(void* pMemPool);
//D2Common.0x6FD62D30 (#10011)
int __fastcall DATATBLS_GetAutomapCellId(DWORD dwAutomapLevelType, DWORD dwAutomapTileType, int nStyle, int nSequence);
//D2Common.0x6FD62E70
void __fastcall DATATBLS_FreeAutomap();



//D2Common.0x6FD62EA0
int __fastcall DATATBLS_MapMissilesTxtKeywordToNumber(char* szKey);
//D2Common.0x6FD62F20
int __fastcall sub_6FD62F20(char* szText, int* a2, int a3, int nKeywordNumber);
//D2Common.0x6FD630F0
void __fastcall DATATBLS_MissileCalcLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD63180
void __fastcall DATATBLS_LoadMissilesTxt(void* pMemPool);
//D2Common.0x6FD64B80
void __fastcall DATATBLS_UnloadMissilesTxt();
//D2Common.0x6FD64BE0 (#10590)
int __stdcall DATATBLS_GetMissileVelocityFromMissilesTxt(int nMissileId, int nLevel);
//Inlined at various places
D2MissilesTxt* __fastcall DATATBLS_GetMissilesTxtRecord(int nMissileId);


//Inlined in some functions
DWORD __fastcall DATATBLS_StringToCode(char* szText);
//D2Common.0x6FD64C40 (#10592)
int __stdcall DATATBLS_GetMonsterChainInfo(int nMonsterId, int* pMaxChainId, int* pChainId);
//D2Common.0x6FD64CD0
void __fastcall DATATBLS_MonStatsSkillModeLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD64DF0
void __fastcall DATATBLS_LoadMonStatsTxt(void* pMemPool);
//D2Common.0x6FD67BD0 (#11261)
BOOL __stdcall DATATBLS_CalculateMonsterStatsByLevel(int nMonsterId, int nGameType, int nDifficulty, int nLevel, short nFlags, D2MonStatsInitStrc* pMonStatsInit);
//D2Common.0x6FD689B0 (#10642)
void __stdcall DATATBLS_SetVelocityInMonStatsTxtRecord(int nMonsterId, short nVelocity);
//D2Common.0x6FD68A00
void __stdcall DATATBLS_ResetGlobalDefaultUnicodeString();
//D2Common.0x6FD68A10 (#10651)
wchar_t* __fastcall DATATBLS_RollRandomUniqueTitleString(D2UnitStrc* pUnit);
//D2Common.0x6FD68A80 (#10652)
wchar_t* __fastcall DATATBLS_RollRandomUniquePrefixString(D2UnitStrc* pUnit);
//D2Common.0x6FD68AF0 (#10653)
wchar_t* __fastcall DATATBLS_RollRandomUniqueSuffixString(D2UnitStrc* pUnit);
//D2Common.0x6FD68B60 (#10654)
wchar_t* __fastcall DATATBLS_RollRandomUniqueAppellationString(D2UnitStrc* pUnit);
//D2Common.0x6FD68BD0
void __fastcall DATATBLS_ReallocTCExInfo(D2TCExShortStrc* pTCExShort, int nNewRecordCount);
//D2Common.0x6FD68C40
void __fastcall DATATBLS_UpdateTreasureClassProbabilities(D2TCExShortStrc* pTCExShort, D2TCExInfoStrc* pTCExInfo, int nProbability, BOOL bExpansion);
//D2Common.0x6FD68D40
int __fastcall DATATBLS_CalculatePercentage(signed int nValue1, signed int nValue2, signed int nDivisor);
//D2Common.0x6FD68DC0 (#10658)
D2TCExShortStrc* __stdcall DATATBLS_GetTreasureClassExRecordFromName(char* szText);
//D2Common.0x6FD68DF0 (#10659)
D2TCExShortStrc* __stdcall DATATBLS_GetTreasureClassExRecordFromIdAndLevel(WORD wTCId, int nLvl);
//D2Common.0x6FD68E50 (#10660)
D2TCExShortStrc* __stdcall DATATBLS_GetTreasureClassExRecordFromActAndDifficulty(int nDifficulty, int nAct, int nIndex);
//D2Common.0x6FD68EC0
void __fastcall DATATBLS_LoadTreasureClassExTxt(void* pMemPool);
//D2Common.0x6FD69B70 (#10656)
int __stdcall DATATBLS_ShouldNeverCallInExpansion();
//D2Common.0x6FD69B90
void __fastcall DATATBLS_LoadMonItemPercentTxt(void* pMemPool);
//D2Common.0x6FD69C40
void __fastcall DATATBLS_UnloadMonItemPercentTxt();
//D2Common.0x6FD69C50 (#10662)
D2MonItemPercentDataTbl* __fastcall DATATBLS_GetMonItemPercentDataTables();
//D2Common.0x6FD69C60 (#10663)
D2MonItemPercentTxt* __stdcall DATATBLS_GetMonItemPercentTxtRecord(int nId);
//D2Common.0x6FD69CD0
void __fastcall DATATBLS_LoadMonUModTxt(void* pMemPool);
//D2Common.0x6FD69FA0
void __fastcall DATATBLS_LoadSuperUniquesTxt(void* pMemPool);
//D2Common.0x6FD6A440 (#10668)
D2SuperUniquesTxt* __stdcall DATATBLS_GetSuperUniquesTxtRecord(int nSuperUniqueId);
//D2Common.0x6FD6A470 (#11257)
int __fastcall DATATBLS_GetSuperUniquesTxtRecordCount();
//D2Common.0x6FD6A480
void __fastcall DATATBLS_LoadHirelingTxt(void* pMemPool);
//D2Common.0x6FD6B1A0
D2ItemTypesTxt* __fastcall DATATBLS_GetItemTypesTxtRecord(int nItemType);
//D2Common.0x6FD6B1D0 (#10583)
D2HirelingTxt* __stdcall DATATBLS_GetHirelingTxtRecordFromIdAndLevel(BOOL bExpansion, int nId, int nLevel);
//D2Common.0x6FD6B270 (#10585)
D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromNameId(BOOL bExpansion, WORD nNameId, D2HirelingTxt* pOldRecord);
//D2Common.0x6FD6B310 (#10586)
D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromClassId(BOOL bExpansion, int nClass, D2HirelingTxt* pOldRecord);
//D2Common.0x6FD6B3A0 (#10587)
D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromVendorIdAndDifficulty(BOOL bExpansion, int nVendorId, int nDifficulty, D2HirelingTxt* pOldRecord);
//D2Common.0x6FD6B440 (#10584)
D2HirelingTxt* __stdcall DATATBLS_GetNextHirelingTxtRecordFromActAndDifficulty(BOOL bExpansion, int nAct, int nDifficulty, D2HirelingTxt* pOldRecord);
//D2Common.0x6FD6B4F0
void __fastcall DATATBLS_LoadNpcTxt(void* pMemPool);
//D2Common.0x6FD6B820 (#10588)
D2NpcTxt* __stdcall DATATBLS_GetNpcTxtRecord(DWORD dwNpcId);
//D2Common.0x6FD6B850
void __fastcall DATATBLS_LoadMonSoundsTxt(void* pMemPool);
//D2Common.0x6FD6BF50 (#11252)
D2MonSoundsTxt* __stdcall DATATBLS_GetMonSoundsTxtRecordFromSoundId(int nSoundId);
//D2Common.0x6FD6BF80 (#11250)
D2MonSoundsTxt* __stdcall DATATBLS_GetMonSoundsTxtRecordFromMonsterId(int nMonsterId);
//D2Common.0x6FD6BFE0
void __fastcall DATATBLS_MonStats2CompositLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD6C150
void __fastcall DATATBLS_LoadMonStats2Txt(void* pMemPool);
//D2Common.0x6FD6D660
int __fastcall DATATBLS_CheckNestedMonsterTypes(int nMonType1, int nMonType2);
//D2Common.0x6FD6D730
void __fastcall DATATBLS_LoadMonTypeTxt(void* pMemPool);
//D2Common.0x6FD6D910
void __fastcall DATATBLS_UnloadMonTypeTxt();
//D2Common.0x6FD6D960
void __fastcall DATATBLS_LoadMonPropTxt(void* pMemPool);
//D2Common.0x6FD6E8E0
void __fastcall DATATBLS_LoadMonLvlTxt(void* pMemPool);
//D2Common.0x6FD6EDE0
void __fastcall DATATBLS_MonPresetPlaceLinker(char* pSrc, void* pRecord, int nOffset, int nPosition, int nTxtRow, int nTxtColumn);
//D2Common.0x6FD6EE60
void __fastcall DATATBLS_LoadMonPresetTxt(void* pMemPool);
//D2Common.0x6FD6EF30 (#11256)
D2MonPresetTxt* __stdcall DATATBLS_GetMonPresetTxtActSection(int nAct, int* pRecordCount);
//D2Common.0x6FD6EF70 (#11258)
int __stdcall DATATBLS_MapSuperUniqueId(int nType, int nSuperUnique);
//D2Common.0x6FD6EFA0
void __fastcall DATATBLS_LoadSoundsTxt(void* pMemPool);
//D2Common.0x6FD6F020
void __fastcall DATATBLS_UnloadSoundsTxt();
//D2Common.0x6FD6F050
void __fastcall DATATBLS_LoadMonSeqTxt(void* pMemPool);
//D2Common.0x6FD6F200 (#11262)
D2SeqRecordStrc* __stdcall DATATBLS_GetMonSeqTableRecord(int nSequence);
//D2Common.0x6FD6F230
void __fastcall DATATBLS_LoadMonEquipTxt(void* pMemPool);
//D2Common.0x6FD6F580
void __fastcall DATATBLS_LoadSomeMonsterTxts(void* pMemPool);
//D2Common.0x6FD6F810
void __fastcall DATATBLS_CreateItemTypeTreasureClasses();
//D2Common.0x6FD6FBB0
void __fastcall DATATBLS_UnloadSomeMonsterTxts();
//Inlined at various places
D2MonStatsTxt* __fastcall DATATBLS_GetMonStatsTxtRecord(int nMonsterId);



//D2Common.0x6FD6FDF0
void __fastcall DATATBLS_LoadObjectsTxt(void* pMemPool);
//D2Common.0x6FD718F0 (#10626)
D2ObjectsTxt* __stdcall DATATBLS_GetObjectsTxtRecord(int nObjectId);
//D2Common.0x6FD71960
void __fastcall DATATBLS_UnloadObjectsTxt();
//D2Common.0x6FD71980
void __fastcall DATATBLS_UnloadObjGroupTxt();
//D2Common.0x6FD719A0
void __fastcall DATATBLS_LoadObjGroupTxt(void* pMemPool);
//D2Common.0x6FD71E00 (#10627)
D2ObjGroupTxt* __stdcall DATATBLS_GetObjGroupTxtRecord(int nId);
//D2Common.0x6FD71E30
void __fastcall DATATBLS_LoadShrinesTxt(void* pMemPool);
//D2Common.0x6FD72000 (#10624)
D2ShrinesTxt* __stdcall DATATBLS_GetShrinesTxtRecord(int nShrineId);
//D2Common.0x6FD72070 (#10625)
int __stdcall DATATBLS_GetShrinesTxtRecordCount();
//D2Common.0x6FD72080
void __fastcall DATATBLS_UnloadShrinesTxt();



//D2Common.0x6FD720A0
void __fastcall DATATBLS_LoadOverlayTxt(void* pMemPool);
//D2Common.0x6FD72500
void __fastcall DATATBLS_UnloadOverlayTxt();
//D2Common.0x6FD72530 (#10674)
DWORD __stdcall DATATBLS_GetFramesFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD72570 (#10675)
int __stdcall DATABLS_GetPreDrawFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD725B0 (#10676)
DWORD __stdcall DATATBLS_GetOffsetXFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD725F0 (#10677)
DWORD __stdcall DATATBLS_GetOffsetYFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD72630 (#10678)
int __stdcall DATATBLS_GetTransFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD72670 (#10679)
int __stdcall DATATBLS_GetRadiusAndColorFromOverlayTxt(int nOverlayId, int* pInitRadius, int* pRadius, BYTE* pRed, BYTE* pGreen, BYTE* pBlue);
//D2Common.0x6FD72720 (#10680)
DWORD __stdcall DATATBLS_Get1OfNFromOverlayTxt(int nOverlayId);
//D2Common.0x6FD72760 (#10681)
int __stdcall DATATBLS_GetDirFromOverlayTxt(int nOverlayId);
//Inlined at various places
D2OverlayTxt* __fastcall DATATBLS_GetOverlayTxtRecord(int nOverlay);



//D2Common.0x6FD727A0 (#10682)
D2MonSeqTxt* __stdcall DATATBLS_GetMonSeqTxtRecordFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FD727C0
D2SeqRecordStrc* __fastcall DATATBLS_GetSeqRecordFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FD728A0 (#10683)
int __stdcall D2Common_10683(D2UnitStrc* pUnit);
//D2Common.0x6FD728C0 (#10684)
int __stdcall D2Common_10684(D2UnitStrc* pUnit);
//D2Common.0x6FD728E0 (#10685)
void __stdcall D2Common_10685(D2MonSeqTxt* pMonSeqTxt, int a2, int a3, unsigned int* pMode, unsigned int* pFrame, int* pDirection, int* pEvent);
//D2Common.0x6FD72990 (#10686)
void __stdcall DATATBLS_GetSequenceEvent(D2MonSeqTxt* pMonSeqTxt, int a2, int* pEvent);
//D2Common.0x6FD729C0
void __fastcall DATATBLS_LoadPlrType_ModeTxt(void* pMemPool);
//D2Common.0x6FD72B30
void __fastcall DATATBLS_LoadMonModeTxt(void* pMemPool);
//D2Common.0x6FD72E50
void __fastcall DATATBLS_LoadObjType_ModeTxt(void* pMemPool);
//D2Common.0x6FD72FC0
void __fastcall DATATBLS_LoadCompositTxt(void* pMemPool);
//D2Common.0x6FD73040
void __fastcall DATATBLS_LoadArmTypeTxt(void* pMemPool);
//D2Common.0x6FD730C0
void __fastcall DATATBLS_UnloadPlrMode_Type_MonMode_ObjMode_Type_Composit_ArmtypeTxt();
//D2Common.0x6FD73150 (#10643)
D2PlrModeDataTbl* __fastcall DATATBLS_GetPlrMode_TypeDataTables();
//D2Common.0x6FD73160 (#10644)
D2MonModeDataTbl* __fastcall DATATBLS_GetMonModeDataTables();
//D2Common.0x6FD73170 (#10645)
D2ObjModeDataTbl* __fastcall DATATBLS_GetObjMode_TypeDataTables();
//D2Common.0x6FD73180 (#10646)
D2PlrModeTypeTxt* __stdcall DATATBLS_GetPlrModeTypeTxtRecord(int nIndex, int bGetMode);
//D2Common.0x6FD73230 (#10647)
D2MonModeTxt* __stdcall DATATBLS_GetMonModeTxtRecord(int nIndex, int bGetMode);
//D2Common.0x6FD732B0 (#10648)
D2ObjModeTypeTxt* __stdcall DATATBLS_GetObjModeTypeTxtRecord(int nIndex, int bGetMode);
//D2Common.0x6FD73330 (#10649)
D2CompositTxt* __stdcall DATATBLS_GetCompositTxtRecord(int nComposit);
//D2Common.0x6FD73370 (#10650)
D2ArmTypeTxt* __stdcall DATATBLS_GetArmTypeTxtRecord(int nId);
//D2Common.0x6FD733B0 (#10667)
int __stdcall DATATBLS_UNUSED_Return0(int a1, int a2);
