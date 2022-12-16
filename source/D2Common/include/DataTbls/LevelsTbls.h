#pragma once 

#include <D2BasicTypes.h>
#include <Drlg/D2DrlgDrlgGrid.h>

#pragma pack(1)

struct D2AutomapTxt
{
	char szLevelName[16];				//0x00
	char szTileName[8];					//0x10 Matches D2TileType
	char nStyle;						//0x18
	char nStartSequence;				//0x19
	char nEndSequence;					//0x1A
	uint8_t pad0x1B;					//0x1B
	int32_t nCel[4];					//0x1C
};

struct D2AutomapRandStrc
{
	int32_t nStart;						//0x00
	int32_t nEnd;						//0x04
};

struct D2AutomapShortStrc
{
	uint32_t dwAutomapLevelType;		//0x00
	uint32_t dwAutomapTileType;			//0x04 See D2TileType
	char nStyle;						//0x08
	char nStartSequence;				//0x09
	char nEndSequence;					//0x0A
	uint8_t pad0x0B;					//0x0B
	int32_t nCel[4];					//0x0C
	int32_t nCels;						//0x1C
};

struct D2LevelDefBin
{
	uint32_t dwQuestFlag;				//0x00
	uint32_t dwQuestFlagEx;				//0x04
	uint32_t dwLayer;					//0x08
	uint32_t dwSizeX[3];				//0x0C
	uint32_t dwSizeY[3];				//0x18
	uint32_t dwOffsetX;					//0x24
	uint32_t dwOffsetY;					//0x28
	uint32_t dwDepend;					//0x2C
	uint32_t dwDrlgType;				//0x30
	uint32_t dwLevelType;				//0x34
	uint32_t dwSubType;					//0x38
	uint32_t dwSubTheme;				//0x3C
	uint32_t dwSubWaypoint;				//0x40
	uint32_t dwSubShrine;				//0x44
	int32_t dwVis[8];					//0x48
	uint32_t dwWarp[8];					//0x68
	uint8_t nIntensity;					//0x88
	uint8_t nRed;						//0x89
	uint8_t nGreen;						//0x8A
	uint8_t nBlue;						//0x8B
	uint32_t dwPortal;					//0x8C
	uint32_t dwPosition;				//0x90
	uint32_t dwSaveMonsters;			//0x94
	uint32_t dwLOSDraw;					//0x98
};

struct D2LevelsTxt
{
	uint16_t wLevelNo;					//0x00
	uint8_t nPal;						//0x02
	uint8_t nAct;						//0x03
	uint8_t nTeleport;					//0x04
	uint8_t nRain;						//0x05
	uint8_t nMud;						//0x06
	uint8_t nNoPer;						//0x07
	uint8_t nIsInside;					//0x08
	uint8_t nDrawEdges;					//0x09
	uint16_t unk0x0A;					//0x0A
	uint32_t dwWarpDist;				//0x0C
	uint16_t wMonLvl[3];				//0x10
	uint16_t wMonLvlEx[3];				//0x16
	uint32_t dwMonDen[3];				//0x1C
	uint8_t nMonUMin[3];				//0x28
	uint8_t nMonUMax[3];				//0x2B
	uint8_t nMonWndr;					//0x2E
	uint8_t nMonSpcWalk;				//0x2F
	uint8_t nQuest;						//0x30
	uint8_t nRangedSpawn;				//0x31
	uint8_t nNumMon;					//0x32
	uint8_t nNumNormMon;				//0x33
	uint8_t nNumNMon;					//0x34
	uint8_t nNumUMon;					//0x35
	short wMon[25];						//0x36
	short wNMon[25];					//0x68
	short wUMon[25];					//0x9A
	uint16_t wCMon[4];					//0xCC
	uint16_t wCPct[4];					//0xD4
	uint16_t wCAmt[4];					//0xDC
	uint8_t nWaypoint;					//0xE4
	uint8_t nObjGroup[8];				//0xE5
	uint8_t nObjPrb[8];					//0xED
	char szLevelName[40];				//0xF5
	char szLevelWarp[40];				//0x11D
	char szEntryFile[40];				//0x145
	uint8_t pad0x16D;					//0x16D
	wchar_t wszLevelName[40];			//0x16E
	wchar_t wszLevelWarp[40];			//0x1BE
	uint16_t pad0x20E;					//0x20E
	uint32_t dwThemes;					//0x210
	uint32_t dwFloorFilter;				//0x214
	uint32_t dwBlankScreen;				//0x218
	uint32_t dwSoundEnv;				//0x21C
};

struct D2LvlMazeTxt
{
	uint32_t dwLevelId;						//0x00
	uint32_t dwRooms[3];					//0x04
	uint32_t dwSizeX;						//0x10
	uint32_t dwSizeY;						//0x14
	uint32_t dwMerge;						//0x18
};

struct D2LvlPrestTxt
{
	uint32_t dwDef;							//0x00
	uint32_t dwLevelId;						//0x04
	uint32_t dwPopulate;					//0x08
	uint32_t dwLogicals;					//0x0C
	uint32_t dwOutdoors;					//0x10
	uint32_t dwAnimate;						//0x14
	uint32_t dwKillEdge;					//0x18
	uint32_t dwFillBlanks;					//0x1C
	uint32_t dwExpansion;					//0x20
	uint32_t nAnimSpeed;					//0x24
	int32_t nSizeX;							//0x28
	int32_t nSizeY;							//0x2C
	uint32_t dwAutoMap;						//0x30
	uint32_t dwScan;						//0x34
	uint32_t dwPops;						//0x38
	uint32_t dwPopPad;						//0x3C
	uint32_t dwFiles;						//0x40
	char szFile[6][60];						//0x44
	//char szFile1[60];						//0x44
	//char szFile2[60];						//0x80
	//char szFile3[60];						//0xBC
	//char szFile4[60];						//0xF8
	//char szFile5[60];						//0x134
	//char szFile6[60];						//0x170
	uint32_t dwDt1Mask;						//0x1AC
};

struct D2LvlSubTxt
{
	uint32_t dwType;						//0x00
	char szFile[60];						//0x04
	uint32_t dwCheckAll;					//0x40
	uint32_t dwBordType;					//0x44
	uint32_t dwDt1Mask;						//0x48
	uint32_t dwGridSize;					//0x4C
	D2DrlgFileStrc* pDrlgFile;				//0x50
	D2DrlgGridStrc pTileTypeGrid[4];		//0x54
	D2DrlgGridStrc pWallGrid[4];			//0xA4
	D2DrlgGridStrc pFloorGrid;				//0xF4
	D2DrlgGridStrc pShadowGrid;				//0x108
	uint32_t nProb[5];						//0x11C
	int32_t nTrials[5];						//0x130
	int32_t nMax[5];						//0x144
	uint32_t dwExpansion;					//0x158
};

struct D2LvlTypesTxt
{
	char szFile[32][60];					//0x00
	uint8_t nAct;							//0x780
	uint8_t pad0x781[3];					//0x781
	uint32_t dwExpansion;					//0x784
};

struct D2LvlWarpTxt
{
	uint32_t dwLevelId;						//0x00
	uint32_t dwSelectX;						//0x04
	uint32_t dwSelectY;						//0x08
	uint32_t dwSelectDX;					//0x0C
	uint32_t dwSelectDY;					//0x10
	uint32_t dwExitWalkX;					//0x14
	uint32_t dwExitWalkY;					//0x18
	uint32_t dwOffsetX;						//0x1C
	uint32_t dwOffsetY;						//0x20
	uint32_t dwLitVersion;					//0x24
	uint32_t dwTiles;						//0x28
	char szDirection[4];					//0x30
};

#pragma pack()


//D2Common.0x6FD5EAE0
void __fastcall DATATBLS_LoadLevelsTxt(void* pMemPool);
//D2Common.0x6FD603C0 (#10631)
D2COMMON_DLL_DECL D2LevelsTxt* __stdcall DATATBLS_GetLevelsTxtRecord(int nLevelId);
//D2Common.0x6FD603F0 (#10632)
D2COMMON_DLL_DECL uint8_t __stdcall DATATBLS_GetRainFromLevelsTxt(int nLevelId);
//D2Common.0x6FD60430 (#10634)
D2COMMON_DLL_DECL uint8_t __stdcall DATATBLS_GetNoPerFromLevelsTxt(int nLevelId);
//D2Common.0x6FD60470 (#10633)
D2COMMON_DLL_DECL uint8_t __stdcall DATATBLS_GetMudFromLevelsTxt(int nLevelId);
//D2Common.0x6FD604B0
void __fastcall DATATBLS_UnloadLevelsTxt();
//D2Common.0x6FD604F0 (#11247)
D2COMMON_DLL_DECL int __stdcall DATATBLS_GetMonsterLevelInArea(int nLevelId, uint8_t nDifficulty, BOOL bExpansion);
//D2Common.0x6FD60560
int* __fastcall DATATBLS_GetPortalLevels(int* pnPortalLevels);
//D2Common.0x6FD60570
void __fastcall DATATBLS_LoadLevelDefsBin(void* pMemPool);
//D2Common.0x6FD60D60
void __fastcall DATATBLS_UnloadLevelDefsBin();
//D2Common.0x6FD60D90 (#10010)
D2COMMON_DLL_DECL D2LevelDefBin* __fastcall DATATBLS_GetLevelDefRecord(int nLevelId);
//D2Common.0x6FD60DC0
void __fastcall DATATBLS_LoadLevelTypesTxt(void* pMemPool);
//D2Common.0x6FD61450
void __fastcall DATATBLS_UnloadLevelTypesTxt();
//D2Common.0x6FD61460 (#10023)
D2COMMON_DLL_DECL D2LvlTypesTxt* __stdcall DATATBLS_GetLevelTypesTxtRecord(int nLevelType);
//D2Common.0x6FD614A0 (#11226)
D2COMMON_DLL_DECL bool __stdcall DATATBLS_CheckActInLevelTypesTxt(int nLevelType, uint8_t nAct);
//D2Common.0x6FD61500 (#11227)
D2COMMON_DLL_DECL void __stdcall DATATBLS_GetFileNameFromLevelTypeAndFileId(int nLevelType, int nFile, char* szFile);
//D2Common.0x6FD61570
void __fastcall DATATBLS_LoadLvlPrestTxt(void* pMemPool, int a2);
//D2Common.0x6FD61AD0
void __fastcall DATATBLS_UnloadLvlPrestTxt();
//D2Common.0x6FD61B50 (#10024)
D2COMMON_DLL_DECL D2LvlPrestTxt* __stdcall DATATBLS_GetLvlPrestTxtRecord(int nId);
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
D2COMMON_DLL_DECL int __fastcall DATATBLS_GetAutomapCellId(uint32_t dwAutomapLevelType, uint32_t dwAutomapTileType, int nStyle, int nSequence);
//D2Common.0x6FD62E70
void __fastcall DATATBLS_FreeAutomap();

