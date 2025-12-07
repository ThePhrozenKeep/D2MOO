#pragma once

#include "D2CommonDefinitions.h"
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"

#pragma pack(1)

struct D2UnkDrlgLogicStrc
{
	D2DrlgRoomStrc* pDrlgRoom;				//0x00
	D2DrlgGridStrc* field_4;				//0x04
	D2DrlgGridStrc* pTileTypeGrid;			//0x08
	D2DrlgGridStrc* pWallGrid;				//0x0C
	D2DrlgGridStrc* pFloorGrid;				//0x10
	D2DrlgGridStrc* field_14;				//0x14
	int32_t field_18;						//0x18
	int32_t nFlags;							//0x1C
};

enum D2DrlgLogicalRoomInfoFlags
{
	DRLGLOGIC_ROOMINFO_HAS_COORD_LIST = 0x1,
	DRLGLOGIC_ROOMINFO_HAS_GRID_CELLS = 0x2,
};

struct D2DrlgLogicalRoomInfoStrc // aka D2DrlgCoordListStrc
{
	int32_t dwFlags;						//0x00 D2DrlgLogicalRoomInfoFlags
	int32_t nLists;							//0x04
	D2DrlgGridStrc pIndexX;					//0x08
	D2DrlgGridStrc pIndexY;					//0x1C
	D2RoomCoordListStrc* pCoordList;		//0x30
};

#pragma pack()

//D2Common.0x6FD76420
void __fastcall DRLGLOGIC_FreeDrlgCoordList(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD764A0
//TODO: v28, a1
void __fastcall DRLGLOGIC_InitializeDrlgCoordList(D2DrlgRoomStrc* pDrlgRoom, D2DrlgGridStrc* pTileTypeGrid, D2DrlgGridStrc* pFloorGrid, D2DrlgGridStrc* pWallGrid);
//D2Common.0x6FD76830
void __fastcall DRLGLOGIC_SetTileGridFlags(D2UnkDrlgLogicStrc* a1, int nX, int nY, int a4);
//D2Common.0x6FD769B0
void __fastcall sub_6FD769B0(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD76A90
void __fastcall sub_6FD76A90(D2DrlgRoomStrc* pDrlgRoom1, D2DrlgRoomStrc* pDrlgRoom2, int nX, int nY);
//D2Common.0x6FD76B90
void __fastcall sub_6FD76B90(D2DrlgRoomStrc* pDrlgRoom, int nIndex1, int nIndex2, BOOL bNode);
//D2Common.0x6FD76C20
BOOL __fastcall DRLG_CheckLayer1ButNotWallObject(D2DrlgTileDataStrc* pTileData);
//D2Common.0x6FD76C50
void __fastcall DRLGLOGIC_SetCoordListForTiles(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD76CF0
void __fastcall DRLGLOGIC_AssignCoordListsForGrids(D2DrlgRoomStrc* pDrlgRoom, D2DrlgLogicalRoomInfoStrc* pDrlgCoordList, int nLists);
//D2Common.0x6FD76F90
void __fastcall DRLGLOGIC_AllocCoordLists(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD77080
int __fastcall DRLGLOGIC_GetRoomCoordListIndex(D2DrlgRoomStrc* pDrlgRoom, int nX, int nY);
//D2Common.0x6FD77110
D2RoomCoordListStrc* __fastcall sub_6FD77110(D2DrlgRoomStrc* pDrlgRoom, int nX, int nY);
//D2Common.0x6FD77190
D2RoomCoordListStrc* __fastcall DRLGLOGIC_GetRoomCoordList(D2DrlgRoomStrc* pDrlgRoom);
