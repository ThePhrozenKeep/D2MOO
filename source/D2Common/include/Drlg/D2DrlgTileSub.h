#pragma once

#include "D2CommonDefinitions.h"
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"
#include <DataTbls/LevelsTbls.h>


#pragma pack(1)


struct D2UnkOutdoorStrc
{
	D2DrlgLevelStrc* pLevel;			//0x00
	int32_t* field_4;					//0x04
	D2DrlgGridStrc* pGrid1;				//0x08
	D2DrlgGridStrc* pGrid2;				//0x0C
	int32_t nLevelPrestId;				//0x10
	int32_t field_14;					//0x14
	int32_t nLvlSubId;					//0x18
	unsigned int(__fastcall* field_1C)(D2DrlgLevelStrc* pLevel, int nX, int nY);						//0x1C
	BOOL(__fastcall* field_20)(D2DrlgLevelStrc* pLevel, int nX, int nY, int nId, int nOffset, char nFlags);						//0x20
	BOOL(__fastcall* field_24)(D2DrlgLevelStrc* pLevel, int nX, int nY, int a4, int a5, unsigned int a6);						//0x24
	int(__fastcall* field_28)(D2DrlgLevelStrc* pLevel, int nStyle, int a3);						//0x28
	void(__fastcall* field_2C)(D2DrlgLevelStrc* pLevel, int nX, int nY);						//0x2C
	void(__fastcall* field_30)(D2DrlgLevelStrc* pLevel, int nX, int nY);						//0x30
	void(__fastcall* field_34)(D2DrlgLevelStrc* pLevel, int nX, int nY, int nLevelPrestId, int nRand, BOOL a6);						//0x34
};

struct D2UnkOutdoorStrc2
{
	D2DrlgRoomStrc* pDrlgRoom;									//0x00
	D2DrlgOutdoorRoomStrc* pOutdoorRooms[DRLG_MAX_WALL_LAYERS];	//0x04
	D2DrlgGridStrc* pWallsGrids[DRLG_MAX_WALL_LAYERS];			//0x14
	D2DrlgGridStrc* pFloorGrid;									//0x24
	int32_t field_28;											//0x28
	int32_t field_2C;											//0x28
	int32_t nSubWaypoint_Shrine;								//0x2C
	int32_t nSubTheme;											//0x30
	int32_t nSubThemePicked;									//0x34
};

struct D2UnkOutdoorStrc3
{
	int32_t nLevelId;						//0x00
	int32_t nExcludedLevel1;				//0x04
	int32_t nExcludedLevel2;				//0x08
	int32_t nRand;							//0x0C
	int32_t nNextRand;						//0x10
	int32_t nFlags;							//0x14
};

struct D2DrlgSubstGroupStrc
{
	D2DrlgCoordStrc tBox;
	int32_t field_10;
	int32_t field_14;
};


#pragma pack()


//D2Common.0x6FD8A460
void __fastcall DRLGTILESUB_AddSecondaryBorder(D2UnkOutdoorStrc* a1);
//D2Common.0x6FD8A750
BOOL __fastcall DRLGTILESUB_TestReplaceSubPreset(int a1, int a2, D2UnkOutdoorStrc* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8A8E0
void __fastcall DRLGTILESUB_ReplaceSubPreset(int a1, int a2, D2UnkOutdoorStrc* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord, int a6);
//D2Common.0x6FD8AA80
void __fastcall sub_6FD8AA80(D2UnkOutdoorStrc2* a1);
//D2Common.0x6FD8ACE0
void __fastcall sub_6FD8ACE0(void* pMemPool, int nX, int nY, D2UnkOutdoorStrc2* a4, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord, int a7);
//D2Common.0x6FD8B010
BOOL __fastcall sub_6FD8B010(int a1, int a2, D2UnkOutdoorStrc2* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8B130
BOOL __fastcall sub_6FD8B130(int a1, int a2, D2UnkOutdoorStrc2* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8B290
void __fastcall DRLGTILESUB_DoSubstitutions(D2UnkOutdoorStrc2* pOutdoorLevel, D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8B640
void __fastcall DRLGTILESUB_InitializeDrlgFile(HD2ARCHIVE hArchive, D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8B770
void __fastcall DRLGTILESUB_FreeDrlgFile(D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8B7E0
int __fastcall DRLGTILESUB_PickSubThemes(D2DrlgRoomStrc* pDrlgRoom, int nSubType, int nSubTheme);
