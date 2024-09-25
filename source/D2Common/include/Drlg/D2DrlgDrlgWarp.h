#pragma once

#include "CommonDefinitions.h"

struct D2DrlgStrc;
struct D2DrlgLevelStrc;
struct D2LvlWarpTxt;
struct D2ActiveRoomStrc;
struct D2DrlgRoomStrc;
struct D2UnitStrc;

#pragma pack(1)


#pragma pack()

//D2Common.0x6FD78780
D2ActiveRoomStrc* __fastcall DRLGWARP_GetDestinationRoom(D2DrlgRoomStrc* pDrlgRoom, int nSourceLevel, int* pDestinationLevel, D2LvlWarpTxt** ppLvlWarpTxtRecord);
//D2Common.0x6FD787F0
void __fastcall DRLGWARP_ToggleRoomTilesEnableFlag(D2DrlgRoomStrc* pDrlgRoom, BOOL bEnabled);
//D2Common.0x6FD78810
void __fastcall DRLGWARP_UpdateWarpRoomSelect(D2DrlgRoomStrc* pDrlgRoom, int nLevelId);
//D2Common.0x6FD78870
void __fastcall DRLGWARP_UpdateWarpRoomDeselect(D2DrlgRoomStrc* pDrlgRoom, int nLevelId);
//D2Common.0x6FD788D0
D2ActiveRoomStrc* __fastcall sub_6FD788D0(D2DrlgStrc* pDrlg, int nLevelId, int nTileIndex, int* pX, int* pY);
//D2Common.0x6FD78C10
D2DrlgRoomStrc* __fastcall DRLGWARP_GetWaypointRoomExFromLevel(D2DrlgLevelStrc* pLevel, int* pX, int* pY);
//D2Common.0x6FD78CC0
int* __fastcall DRLGWARP_GetWarpIdArrayFromLevelId(D2DrlgStrc* pDrlg, int nLevelId);
//D2Common.0x6FD78D10
int __fastcall DRLGWARP_GetWarpDestinationFromArray(D2DrlgLevelStrc* pLevel, uint8_t nArrayId);
//D2Common.0x6FD78D80
D2LvlWarpTxt* __fastcall DRLGWARP_GetLvlWarpTxtRecordFromWarpIdAndDirection(D2DrlgLevelStrc* pLevel, uint8_t nWarpId, char szDirection);
//D2Common.0x6FD78DF0
D2LvlWarpTxt* __fastcall DRLGWARP_GetLvlWarpTxtRecordFromUnit(D2DrlgRoomStrc* pDrlgRoom, D2UnitStrc* pUnit);
