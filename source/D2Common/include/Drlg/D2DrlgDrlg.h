#pragma once

#include "CommonDefinitions.h"

enum D2DrlgFlags
{
	DRLGFLAG_ONCLIENT = 0x01,
};

enum D2Directions
{
	DIRECTION_INVALID = -1,
	DIRECTION_SOUTHWEST = 0,
	DIRECTION_NORTHWEST = 1,
	DIRECTION_SOUTHEAST = 2,
	DIRECTION_NORTHEAST = 3,
	DIRECTION_COUNT = 4,
};

enum D2AltDirections
{
	ALTDIR_WEST,
	ALTDIR_NORTH,
	ALTDIR_EAST,
	ALTDIR_SOUTH,
	ALTDIR_NORTHWEST,
	ALTDIR_NORTHEAST,
	ALTDIR_SOUTHEAST,
	ALTDIR_SOUTHWEST,
};

enum D2C_DrlgTypes
{
	DRLGTYPE_MAZE = 0x01,
	DRLGTYPE_PRESET = 0x02,
	DRLGTYPE_OUTDOOR = 0x03,
	NUM_DRLGTYPES
};

enum D2RoomExFlags
{
	ROOMEXFLAG_INACTIVE = 0x00000002,
	ROOMEXFLAG_HAS_WARP_0 = 0x00000010,
	ROOMEXFLAG_HAS_WARP_1 = 0x00000020,
	ROOMEXFLAG_HAS_WARP_2 = 0x00000040,
	ROOMEXFLAG_HAS_WARP_3 = 0x00000080,
	ROOMEXFLAG_HAS_WARP_4 = 0x00000100,
	ROOMEXFLAG_HAS_WARP_5 = 0x00000200,
	ROOMEXFLAG_HAS_WARP_6 = 0x00000400,
	ROOMEXFLAG_HAS_WARP_7 = 0x00000800,
	ROOMEXFLAG_SUBSHRINE_ROW1 = 0x00001000,
	ROOMEXFLAG_SUBSHRINE_ROW2 = 0x00002000,
	ROOMEXFLAG_SUBSHRINE_ROW3 = 0x00004000,
	ROOMEXFLAG_SUBSHRINE_ROW4 = 0x00008000,
	ROOMEXFLAG_HAS_WAYPOINT = 0x00010000,			//outdoors with subtheme and subwaypoint
	ROOMEXFLAG_HAS_WAYPOINT_SMALL = 0x00020000,		//waypoint small
	ROOMEXFLAG_AUTOMAP_REVEAL = 0x00040000,
	ROOMEXFLAG_NO_LOS_DRAW = 0x00080000,
	ROOMEXFLAG_HAS_ROOM = 0x00100000,				//an active pRoom structure is attached to this RoomEx
	ROOMEXFLAG_ROOM_FREED_SRV = 0x00200000,			//set after freeing the pRoom
	ROOMEXFLAG_HASPORTAL = 0x00400000,				//prevents room deletion
	ROOMEXFLAG_POPULATION_ZERO = 0x00800000,		//set for towns, also set if a ds1 has populate=0 // nospawn region
	ROOMEXFLAG_TILELIB_LOADED = 0x01000000,
	ROOMEXFLAG_PRESET_UNITS_ADDED = 0x02000000,		//refers to DRLGMap or DRLGFile; adds the hardcoded preset units
	ROOMEXFLAG_PRESET_UNITS_SPAWNED = 0x04000000,	//set after RoomEx preset units have been spawned / prevents respawning them on room reactivation
	ROOMEXFLAG_ANIMATED_FLOOR = 0x08000000,			//animated floors (river of flame, hell act5)

	ROOMEXFLAG_HAS_WARP = (ROOMEXFLAG_HAS_WARP_0 | ROOMEXFLAG_HAS_WARP_1 | ROOMEXFLAG_HAS_WARP_2 | ROOMEXFLAG_HAS_WARP_3 | ROOMEXFLAG_HAS_WARP_4 | ROOMEXFLAG_HAS_WARP_5 | ROOMEXFLAG_HAS_WARP_6 | ROOMEXFLAG_HAS_WARP_7),
};

enum D2DrlgLevelFlags
{
	DRLGLEVELFLAG_AUTOMAP_REVEAL = 0x10,
};

//D2Common.0x6FD74120 (#10014)
D2DrlgStrc* __fastcall DRLG_AllocDrlg(D2DrlgActStrc* pAct, uint8_t nActNo, void* a3, uint32_t nInitSeed, int nLevelId, uint32_t nFlags, D2GameStrc* pGame, uint8_t nDifficulty, AUTOMAPFN pfAutoMap, TOWNAUTOMAPFN pfTownAutoMap);
//D2Common.0x6FD743B0 (#10012)
void __fastcall DRLG_FreeDrlg(D2DrlgStrc* pDrlg);
//D2Common.0x6FD74440
void __fastcall DRLG_FreeLevel(void* pMemPool, D2DrlgLevelStrc* pLevel, BOOL a3);
//D2Common.0x6FD745C0
void __fastcall sub_6FD745C0(D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2);
//D2Common.0x6FD74700
void __fastcall sub_6FD74700(D2DrlgStrc* pDrlg);
//D2Common.0x6FD748D0 (#10013)
D2DrlgLevelStrc* __fastcall DRLG_AllocLevel(D2DrlgStrc* pDrlg, int nLevelId);
//D2Common.0x6FD749A0 (#10005)
D2DrlgLevelStrc* __stdcall DRLG_GetLevel(D2DrlgStrc* pDrlg, int nLevelId);
//D2Common.0x6FD749D0
int __fastcall DRLG_GetHoradricStaffTombLevelId(D2DrlgStrc* pDrlg);
//D2Common.0x6FD749E0
int __fastcall DRLG_GetDirectionFromCoordinates(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2);
//D2Common.0x6FD74A40
void __fastcall DRLG_CreateRoomForRoomEx(D2DrlgStrc* pDrlg, D2RoomExStrc* pRoomEx);
//D2Common.0x6FD74B30
int* __fastcall DRLG_GetRoomCenterX_RoomWarpXFromRoom(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD74B40
void __fastcall DRLG_ComputeLevelWarpInfo(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD74C10 (#10006)
void __stdcall DRLG_InitLevel(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD74D50
int __fastcall DRLG_GetNumberOfPopulatedRoomsInLevel(D2DrlgStrc* pDrlg, int nLevelId);
//D2Common.0x6FD74D90
void __fastcall DRLG_GetMinAndMaxCoordinatesFromLevel(D2DrlgLevelStrc* pLevel, int* pTileMinX, int* pTileMinY, int* pTileMaxX, int* pTileMaxY);
//D2Common.0x6FD74E10
void __fastcall DRLG_UpdateRoomExCoordinates(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD74EF0
D2RoomExStrc* __fastcall DRLG_GetRoomExFromLevelAndCoordinates(D2DrlgLevelStrc* pLevel, int nX, int nY);
//D2Common.0x6FD74F70
D2RoomExStrc* __fastcall DRLG_GetRoomExFromCoordinates(int nX, int nY, D2DrlgStrc* pDrlg, D2RoomExStrc* pRoomEx, D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD751C0
BOOL __fastcall DRLG_IsTownLevel(int nLevelId);
//D2Common.0x6FD75260 (#10000)
int __stdcall DRLG_GetLevelTypeFromLevelId(int nLevelId);
//D2Common.0x6FD75270
void __fastcall DRLG_SetLevelPositionAndSize(D2DrlgStrc* pDrlg, D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD75300 (#10001)
uint8_t __stdcall DRLG_GetActNoFromLevelId(int nLevelId);
//D2Common.0x6FD75330 (#10004)
int __stdcall DRLG_GetSaveMonstersFromLevelId(int nLevelId);
//D2Common.0x6FD75350 (#10002)
int __stdcall DRLG_GetLOSDrawFromLevelId(int nLevelId);
//D2Common.0x6FD75370
D2DrlgWarpStrc* __fastcall DRLG_GetDrlgWarpFromLevelId(D2DrlgStrc* pDrlg, int nLevelId);
//D2Common.0x6FD753F0
void __fastcall DRLG_SetWarpId(D2DrlgWarpStrc* pDrlgWarp, int nVis, int nWarp, int nId);
//D2Common.0x6FD75450
int __fastcall DRLG_IsOnClient(D2DrlgStrc* pDrlg);
