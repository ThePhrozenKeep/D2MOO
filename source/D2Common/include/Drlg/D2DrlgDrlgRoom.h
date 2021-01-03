#pragma once

#include "CommonDefinitions.h"

#pragma pack(1)


#pragma pack()

//D2Common.0x6FD771C0
D2RoomExStrc* __fastcall DRLGROOM_AllocRoomEx(D2DrlgLevelStrc* pLevel, int nType);
//D2Common.0x6FD77280
void __fastcall sub_6FD77280(D2RoomExStrc* pRoomEx, BOOL bClient, uint32_t nFlags);
//D2Common.0x6FD772B0
void __fastcall DRLGROOM_FreeRoomTiles(void* pMemPool, D2RoomExStrc* pRoomEx);
//D2Common.0x6FD772F0
void __fastcall DRLGROOM_FreeRoomEx(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD774F0
void __fastcall DRLGROOM_FreeRoomData(void* pMemPool, D2DrlgOrthStrc* pDrlgRoomData);
//D2Common.0x6FD77520
void __fastcall DRLGROOM_AllocDrlgOrthsForRooms(D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2, int nDirection);
//D2Common.0x6FD77600
void __fastcall DRLGROOM_AddOrth(D2DrlgOrthStrc** ppDrlgOrth, D2DrlgLevelStrc* pLevel, int nDirection, BOOL bIsPreset);
//D2Common.0x6FD776B0
BOOL __fastcall sub_6FD776B0(D2DrlgOrthStrc* pDrlgOrth1, D2DrlgOrthStrc* pDrlgOrth2);
//D2Common.0x6FD77740
BOOL __fastcall sub_6FD77740(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int nMaxDistance, int* pX, int* pY);
//D2Common.0x6FD777B0
BOOL __fastcall sub_6FD777B0(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int nMaxDistance);
//D2Common.0x6FD77800
BOOL __fastcall sub_6FD77800(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int nMaxDistance);
//D2Common.0x6FD77890
BOOL __fastcall sub_6FD77890(D2DrlgLevelStrc* pLevel, D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2, int nMaxDistance);
//D2Common.0x6FD77910
void __fastcall DRLGROOM_AddRoomExToLevel(D2DrlgLevelStrc* pLevel, D2RoomExStrc* pRoomEx);
//D2Common.0x6FD77930
BOOL __fastcall DRLGROOM_AreXYInsideCoordinates(D2DrlgCoordStrc* pDrlgCoord, int nX, int nY);
//D2Common.0x6FD77980
BOOL __fastcall DRLGROOM_AreXYInsideCoordinatesOrOnBorder(D2DrlgCoordStrc* pDrlgCoord, int nX, int nY);
//D2Common.0x6FD779D0
BOOL __fastcall DRLGROOM_CheckLOSDraw(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD779F0
int __fastcall sub_6FD779F0(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD77A00
int __fastcall DRLGROOM_CheckWaypointFlags(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD77A10
int __fastcall DRLGROOM_GetLevelId(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD77A20
int __fastcall DRLGROOM_GetWarpDestinationLevel(D2RoomExStrc* pRoomEx, int nSourceLevel);
//D2Common.0x6FD77AB0
int __fastcall DRLGROOM_GetLevelIdFromPopulatedRoom(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD77AF0
BOOL __fastcall DRLGROOM_HasWaypoint(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD77B20
char* __fastcall DRLGROOM_GetPickedLevelPrestFilePathFromRoomEx(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD77B50
int __fastcall DRLGROOM_ReorderNearRoomList(D2RoomExStrc* pRoomEx, D2RoomStrc** ppRoomList);
//D2Common.0x6FD77BB0
void __fastcall sub_6FD77BB0(void* pMemPool, D2RoomExStrc* pRoomEx);
//D2Common.0x6FD77EB0
void __fastcall DRLGROOM_SortRoomListByPosition(D2RoomExStrc** ppRoomList, int nListSize);
//D2Common.0x6FD77F00
BOOL __fastcall sub_6FD77F00(void* pMemPool, D2RoomExStrc* pRoomEx1, uint8_t nWarpId, D2RoomExStrc* pRoomEx2, char a5, int a6);
//D2Common.0x6FD780E0
D2PresetUnitStrc* __fastcall DRLGROOM_AllocPresetUnit(D2RoomExStrc* pRoomEx, void* pMemPool, int nUnitType, int nIndex, int nMode, int nX, int nY);
//D2Common.0x6FD78160
D2PresetUnitStrc* __fastcall DRLGROOM_GetPresetUnits(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD78190
void __fastcall DRLGROOM_SetRoom(D2RoomExStrc* pRoomEx, D2RoomStrc* pRoom);
//D2Common.0x6FD781A0
void __fastcall DRLGROOM_GetRGB_IntensityFromRoomEx(D2RoomExStrc* pRoomEx, uint8_t* pIntensity, uint8_t* pRed, uint8_t* pGreen, uint8_t* pBlue);
//D2Common.0x6FD781E0
int* __fastcall DRLGROOM_GetVisArrayFromLevelId(D2DrlgStrc* pDrlg, int nLevelId);
//D2Common.0x6FD78230
D2DrlgStrc* __fastcall DRLGROOM_GetDrlgFromRoomEx(D2RoomExStrc* pRoom);
