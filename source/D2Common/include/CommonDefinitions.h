#pragma once

#include <cstdint>
#include <Imports/DllBases.h>
#include <D2Structs.h>
#include <D2Constants.h>


#define D2FUNC(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	typedef RETURN(##CONV##* DLL##_##NAME##_t) ARGS; \
	static DLL##_##NAME##_t DLL##_##NAME = (DLL##_##NAME##_t)(uintptr_t(delayed##DLL##DllBaseGet()) + OFFSET);

#define D2VAR(DLL, NAME, TYPE, OFFSET) \
	typedef TYPE DLL##_##NAME##_vt; \
	static DLL##_##NAME##_vt * DLL##_##NAME = (DLL##_##NAME##_vt *)(uintptr_t(delayed##DLL##DllBaseGet()) + OFFSET);

#define D2PTR(DLL, NAME, OFFSET) \
	static PVOID NAME = (uintptr_t(delayed##DLL##DllBaseGet()) + OFFSET);

// Use for empty/duplicated functions
#define REMOVE_LATER_WriteToLogFile FOG_WriteToLogFile

#define dword_6FDD6A24 1

#include <Fog.h>
#include <D2CMP.h>
#include <D2LANG.h>

#define D2_MAX_PATH 260

struct D2MazeLevelIdStrc
{
	int nLevelPrestId1;						//0x00
	int nLevelPrestId2;						//0x04
	int nPickedFile;						//0x08
	int nDirection;							//0x0C
};

struct D2UnkFileStrc
{
	char szPath[D2_MAX_PATH];					//0x00
	LONG field_104;							//0x104
	D2DrlgFileStrc* pFile;					//0x108
	D2UnkFileStrc* pNext;					//0x10C
};

struct D2UnkOutdoorStrc
{
	D2DrlgLevelStrc* pLevel;			//0x00
	int* field_4;						//0x04
	D2DrlgGridStrc* pGrid1;				//0x08
	D2DrlgGridStrc* pGrid2;				//0x0C
	int nLevelPrestId;					//0x10
	int field_14;						//0x14
	int nLvlSubId;						//0x18
	unsigned int (__fastcall* field_1C)(D2DrlgLevelStrc* pLevel, int nX, int nY);						//0x1C
	BOOL (__fastcall* field_20)(D2DrlgLevelStrc* pLevel, int nX, int nY, int nId, int nOffset, char nFlags);						//0x20
	BOOL (__fastcall* field_24)(D2DrlgLevelStrc* pLevel, int nX, int nY, int a4, int a5, unsigned int a6);						//0x24
	int (__fastcall* field_28)(D2DrlgLevelStrc* pLevel, int nStyle, int a3);						//0x28
	void (__fastcall* field_2C)(D2DrlgLevelStrc* pLevel, int nX, int nY);						//0x2C
	void (__fastcall* field_30)(D2DrlgLevelStrc* pLevel, int nX, int nY);						//0x30
	void (__fastcall* field_34)(D2DrlgLevelStrc* pLevel, int nX, int nY, int nLevelPrestId, int nRand, BOOL a6);						//0x34
};

struct D2UnkOutdoorStrc2
{
	D2RoomExStrc* pRoomEx;					//0x00
	D2DrlgOutdoorRoomStrc* pOutdoorRoom;	//0x04
	int field_8;							//0x08
	int field_C;							//0x0C
	int field_10;							//0x10
	D2DrlgGridStrc* pWallGrid;				//0x14
	int field_18;							//0x18
	int field_1C;							//0x1C
	int field_20;							//0x20
	D2DrlgGridStrc* pFloorGrid;				//0x24
	int field_28;							//0x28
	int field_2C;							//0x28
	int nSubWaypoint_Shrine;				//0x2C
	int nSubTheme;							//0x30
	int nSubThemePicked;					//0x34
};

struct D2UnkOutdoorStrc3
{
	int nLevelId;							//0x00
	int nExcludedLevel1;					//0x04
	int nExcludedLevel2;					//0x08
	int nRand;								//0x0C
	int nNextRand;							//0x10
	int nFlags;								//0x14
};

struct D2DrlgSubstGroupStrc
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
};

struct D2UnkDrlgLogicStrc
{
	D2RoomExStrc* pRoomEx;					//0x00
	D2DrlgGridStrc* field_4;				//0x04
	D2DrlgGridStrc* pOrientationGrid;		//0x08
	D2DrlgGridStrc* pWallGrid;				//0x0C
	D2DrlgGridStrc* pFloorGrid;				//0x10
	D2DrlgGridStrc* field_14;				//0x14
	int field_18;							//0x18
	int nFlags;								//0x1C
};

struct D2UnkOutPlaceStrc
{
	D2DrlgCoordStrc pDrlgCoord;				//0x00
	int field_10;							//0x10
	int nBranch;							//0x14
	D2UnkOutPlaceStrc* field_18;			//0x18
	D2UnkOutPlaceStrc* field_1C[3];			//0x1C
	int field_28;							//0x18
	int field_2C;							//0x2C
	int* pJungleDefs;						//0x30
	int nJungleDefs;						//0x34
};

struct D2UnkMissileCalcStrc
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	D2SeedStrc pSeed;
};

struct D2BoundingBoxStrc
{
	int nLeft;								//0x00
	int nBottom;							//0x04
	int nRight;								//0x08
	int nTop;								//0x0C
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

enum D2DrlgFlags
{
	DRLGFLAG_ONCLIENT = 0x01,
};
