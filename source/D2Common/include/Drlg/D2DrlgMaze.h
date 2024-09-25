#pragma once

#include "CommonDefinitions.h"

struct D2DrlgStrc;
struct D2ActiveRoomStrc;
struct D2DrlgRoomStrc;
struct D2DrlgLevelStrc;

#pragma pack(1)

struct D2MazeLevelIdStrc
{
	int32_t nLevelPrestId1;					//0x00
	int32_t nLevelPrestId2;					//0x04
	int32_t nPickedFile;					//0x08
	int32_t nDirection;						//0x0C
};

#pragma pack()

//D2Common.0x6FD78E50
D2DrlgRoomStrc* __fastcall sub_6FD78E50(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD78F70
void __fastcall DRLGMAZE_PickRoomPreset(D2DrlgRoomStrc* pDrlgRoom, BOOL bResetFlag);
//D2Common.0x6FD79240
D2DrlgRoomStrc* __fastcall sub_6FD79240(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD79360
D2DrlgRoomStrc* __fastcall sub_6FD79360(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD79480
void __fastcall DRLGMAZE_InitLevelData(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD794A0
void __fastcall DRLGMAZE_GenerateLevel(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD79E10
void __fastcall DRLGMAZE_SetPickedFileAndPresetId(D2DrlgRoomStrc* pDrlgRoom, int nLevelPrest, int nPickedFile, BOOL bResetFlag);
//D2Commo.0x6FD79E40
D2DrlgRoomStrc* __fastcall DRLGMAZE_ReplaceRoomPreset(D2DrlgLevelStrc* pLevel, int nLevelPrestId1, int nLevelPrestId2, int nPickedFile, BOOL bResetFlag);
//D2Common.0x6FD79EA0
D2DrlgRoomStrc* __fastcall DRLGMAZE_AddAdjacentMazeRoom(D2DrlgRoomStrc* pDrlgRoom, int nDirection, int bMergeRooms);
//D2Common.0x6FD7A110
void __fastcall DRLGMAZE_InitBasicMazeLayout(D2DrlgLevelStrc* pLevel, int nRoomsPerDirection);
//D2Common.0x6FD7A340
BOOL __fastcall DRLGMAZE_LinkMazeRooms(D2DrlgRoomStrc* pDrlgRoom1, D2DrlgRoomStrc* pDrlgRoom2, int nDirection);
//D2Common.0x6FD7A450
void __fastcall DRLGMAZE_MergeMazeRooms(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD7A570
D2DrlgRoomStrc* __fastcall DRLGMAZE_GetRandomRoomExFromLevel(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7A5D0
void __fastcall DRLGMAZE_BuildBasicMaze(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7A830
void __fastcall DRLGMAZE_PlaceAct5LavaPresets(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7A9B0
void __fastcall DRLGMAZE_FillBlankMazeSpaces(D2DrlgLevelStrc* pLevel, int nLevelPrest, D2DrlgRoomStrc* pIgnoreRoomEx);
//D2Common.0x6FD7AAC0
void __fastcall DRLGMAZE_PlaceAct2TombPrev_Act5BaalPrev(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7ABC0
void __fastcall DRLGMAZE_PlaceArcaneSanctuary(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7AFD0
D2DrlgRoomStrc* __fastcall DRLGMAZE_PlaceAdjacentPresetRoom(D2DrlgRoomStrc* pParentRoomEx, int nDirection, int bMergeRooms);
//D2Common.0x6FD7B230
void __fastcall DRLGMAZE_ScanReplaceSpecialPreset(D2DrlgLevelStrc* pLevel, const D2MazeLevelIdStrc* pMazeInit, int* pRand);
//D2Common.0x6FD7B330
void __fastcall DRLGMAZE_ScanReplaceSpecialAct2SewersPresets(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7B660
void __fastcall DRLGMAZE_AddSpecialPreset(D2DrlgLevelStrc* pLevel, int nDirection, int nLvlPrestId, int nFile);
//D2Common.0x6FD7B710
BOOL __fastcall DRLGMAZE_CheckIfMayPlaceAdjacentPresetRoom(D2DrlgRoomStrc* pDrlgRoom, int nDirection);
//D2Common.0x6FD7B8B0
void __fastcall DRLGMAZE_PlaceAct2TombStuff(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7BC40
D2DrlgRoomStrc* __fastcall DRLGMAZE_InitRoomFixedPreset(D2DrlgRoomStrc* pDrlgRoom, int nDirection, int nLvlPrestId, int nFile, int bUseInitPreset);
//D2Common.0x6FD7BCD0
void __fastcall DRLGMAZE_PlaceAct2LairStuff(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7BE60
void __fastcall DRLGMAZE_PlaceAct3DungeonStuff(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7C000
void __fastcall DRLGMAZE_PlaceAct3SewerStuff(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7C1A0
void __fastcall DRLGMAZE_PlaceAct3MephistoStuff(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7C380
void __fastcall DRLGMAZE_PlaceAct5TempleStuff(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7C500
void __fastcall DRLGMAZE_PlaceAct5BaalStuff(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7C660
void __fastcall DRLGMAZE_PlaceAct1Barracks(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7CA20
void __fastcall DRLGMAZE_SetRoomSize(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD7CA40
void __fastcall DRLGMAZE_PlaceAct4Lava(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7CCB0
void __fastcall DRLGMAZE_PlaceAct5IceStuff(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7CEA0
void __fastcall DRLGMAZE_RollAct_1_2_3_BasicPresets(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7D130
void __fastcall DRLGMAZE_RollBasicPresets(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7D3D0
void __fastcall DRLGMAZE_ResetMazeRecord(D2DrlgLevelStrc* pLevel, BOOL bKeepMazeRecord);
