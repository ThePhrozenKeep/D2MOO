#pragma once

#include "CommonDefinitions.h"
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"

#pragma pack(1)

struct D2UnkFileStrc
{
	char szPath[D2_MAX_PATH];			//0x00
	long field_104;						//0x104
	D2DrlgFileStrc* pFile;				//0x108
	D2UnkFileStrc* pNext;				//0x10C
};

struct D2PresetUnitStrc
{
	int32_t nUnitType;						//0x00
	int32_t nIndex;							//0x04 see D2Common.#11278
	int32_t nMode;							//0x08
	int32_t nXpos;							//0x0C
	int32_t nYpos;							//0x10
	BOOL bSpawned;							//0x14 
	D2MapAIStrc* pMapAI;					//0x18
	D2PresetUnitStrc* pNext;				//0x1C
};

struct D2DrlgMapStrc
{
	int32_t nLevelPrest;						//0x00
	int32_t nPickedFile;						//0x04
	D2LvlPrestTxt* pLvlPrestTxtRecord;		//0x08
	D2DrlgFileStrc* pFile;					//0x0C
	D2DrlgCoordStrc pDrlgCoord;				//0x10
	BOOL bHasInfo;							//0x20
	D2DrlgGridStrc pMapGrid;				//0x24
	D2PresetUnitStrc* pPresetUnit;			//0x38
	BOOL bInited;							//0x3C
	int32_t nPops;								//0x40
	int32_t* pPopsIndex;						//0x44
	int32_t* pPopsSubIndex;						//0x48
	int32_t* pPopsOrientation;					//0x4C
	D2DrlgCoordStrc* pPopsLocation;			//0x50
	D2DrlgMapStrc* pNext;					//0x54
};

struct D2DrlgPresetRoomStrc
{
	int32_t nLevelPrest;						//0x00
	int32_t nPickedFile;						//0x04
	D2DrlgMapStrc* pMap;					//0x08
	union
	{
		struct
		{
			uint8_t nFlags;					//0x0C
			uint8_t unk0x0D[3];				//0x0D
		};
		uint32_t dwFlags;						//0x0C
	};
	D2DrlgGridStrc pWallGrid[4];			//0x10
	D2DrlgGridStrc pOrientationGrid[4];		//0x60
	D2DrlgGridStrc pFloorGrid[2];			//0xB0
	D2DrlgGridStrc pCellGrid;				//0xD8
	D2DrlgGridStrc* pMazeGrid;				//0xEC
	D2CoordStrc* pTombStoneTiles;			//0xF0
	int32_t nTombStoneTiles;					//0xF4
};

#pragma pack()


//D2Common.0x6FD859A0 (#11222)
int __stdcall DRLGPRESET_CountPresetObjectsByAct(uint8_t a1);
//D2Common.0x6FD859E0 (#11223)
int __stdcall DRLGPRESET_GetObjectIndexFromObjPreset(uint8_t a1, int a2);
//D2Common.0x6FD85A10
void __fastcall DRLGPRESET_ParseDS1File(D2DrlgFileStrc* pDrlgFile, void* pMemPool, char* szFileName);
//D2Common.0x6FD86050
void __fastcall DRLGPRESET_LoadDrlgFile(D2DrlgFileStrc** ppDrlgFile, void* pMemPool, char* szFile);
//D2Common.0x6FD86190
void __fastcall DRLGPRESET_FreeDrlgFile(D2DrlgFileStrc** ppDrlgFile);
//D2Common.0x6FD86310
D2PresetUnitStrc* __fastcall DRLGPRESET_CopyPresetUnit(void* pMemPool, D2PresetUnitStrc* pPresetUnit, int nX, int nY);
//D2Common.0x6FD86430
void __fastcall DRLGPRESET_FreePresetUnit(void* pMemPool, D2PresetUnitStrc* pPresetUnit);
//D2Common.0x6FD86480 (#10020)
D2MapAIStrc* __fastcall DRLGPRESET_CreateCopyOfMapAI(void* pMemPool, D2MapAIStrc* pMapAI);
//D2Common.0x6FD864F0 (#10021)
D2MapAIStrc* __fastcall DRLGPRESET_ChangeMapAI(D2MapAIStrc** ppMapAI1, D2MapAIStrc** ppMapAI2);
//D2Common.0x6FD86500 (#10022)
void __fastcall DRLGPRESET_FreeMapAI(void* pMemPool, D2MapAIStrc* pMapAI);
//D2Common.0x6FD86540
void __fastcall DRLGPRESET_AddPresetUnitToDrlgMap(void* pMemPool, D2DrlgMapStrc* pDrlgMap, D2SeedStrc* pSeed);
//D2Common.0x6FD867A0
void __fastcall DRLGPRESET_SpawnHardcodedPresetUnits(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD86AC0
void __fastcall DRLGPRESET_AddPresetRiverObjects(D2DrlgMapStrc* pDrlgMap, void* pMemPool, int a3, D2DrlgGridStrc* pDrlgGrid);
//D2Common.0x6FD86C80
void __fastcall DRLGPRESET_FreePresetRoomData(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD86CE0
void __fastcall DRLGPRESET_FreeDrlgGrids(void* pMemPool, D2RoomExStrc* pRoomEx);
//D2Common.0x6FD86D60
void __fastcall DRLGPRESET_FreeDrlgGridsFromPresetRoom(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD86D80
void __fastcall DRLGPRESET_AllocPresetRoomData(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD86DC0
D2RoomExStrc* __fastcall DRLGPRESET_InitPresetRoomData(D2DrlgLevelStrc* pLevel, D2DrlgMapStrc* pDrlgMap, D2DrlgCoordStrc* pDrlgCoord, uint32_t dwDT1Mask, int dwRoomFlags, int dwPresetFlags, D2DrlgGridStrc* a7);
//D2Common.0x6FD86E50
void __fastcall DRLGPRESET_InitPresetRoomGrids(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD870F0
void __fastcall DRLGPRESET_GetTombStoneTileCoords(D2RoomExStrc* pRoomEx, D2CoordStrc** ppTombStoneTiles, int* pnTombStoneTiles);
//D2Common.0x6FD87130
void __fastcall DRLGPRESET_AddPresetRoomMapTiles(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD87560
D2RoomExStrc* __fastcall DRLGPRESET_BuildArea(D2DrlgLevelStrc* pLevel, D2DrlgMapStrc* pDrlgMap, int nFlags, BOOL bSingleRoom);
//D2Common.0x6FD87760
void __fastcall DRLGPRESET_BuildPresetArea(D2DrlgLevelStrc* pLevel, D2DrlgGridStrc* pDrlgGrid, int nFlags, D2DrlgMapStrc* pDrlgMap, BOOL bSingleRoom);
//D2Common.0x6FD87E10
void __fastcall DRLGPRESET_SetPickedFileInDrlgMap(D2DrlgMapStrc* pDrlgMap, int nPickedFile);
//D2Common.0x6FD87E20
D2DrlgMapStrc* __fastcall DRLGPRESET_AllocDrlgMap(D2DrlgLevelStrc* pLevel, int nLvlPrestId, D2DrlgCoordStrc* pDrlgCoord, D2SeedStrc* pSeed);
//D2Common.0x6FD87F00
int __fastcall DRLGPRESET_GetSizeX(int nLvlPrestId);
//D2Common.0x6FD87F10
int __fastcall DRLGPRESET_GetSizeY(int nLvlPrestId);
//D2Common.0x6FD87F20
void __fastcall DRLGPRESET_FreeDrlgMap(void* pMemPool, D2DrlgMapStrc* pDrlgMap);
//D2Common.0x6FD881A0 (#10008)
int __fastcall DRLGPRESET_GetLevelPrestIdFromRoomEx(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD881B0 (#10009)
char* __fastcall DRLGPRESET_GetPickedLevelPrestFilePathFromRoomEx(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD881D0
void __fastcall DRLGPRESET_UpdatePops(D2RoomExStrc* pRoomEx, int nX, int nY, BOOL bOtherRoom);
//D2Common.0x6FD88450
void __fastcall DRLGPRESET_TogglePopsVisibility(D2RoomExStrc* pRoomEx, int nPopSubIndex, D2DrlgCoordStrc* pDrlgCoord, int nTick, BOOL a5);
//D2Common.0x6FD88610
void __fastcall DRLGPRESET_InitLevelData(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD886F0
void __fastcall DRLGPRESET_GenerateLevel(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD88810
void __fastcall DRLGPRESET_ResetDrlgMap(D2DrlgLevelStrc* pLevel, BOOL bKeepPreset);
//D2Common.0x6FD88850
int __fastcall DRLGPRESET_MapOrientationLayer(int nId);
