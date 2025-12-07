#pragma once

#include "D2CommonDefinitions.h"

struct D2ActiveRoomStrc;
struct D2DrlgRoomStrc;
struct D2DrlgStrc;

#pragma pack(1)


#pragma pack()

//D2Common.0x6FD733D0
void __fastcall DRLGACTIVATE_RoomExSetStatus_ClientInRoom(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD73450
void __fastcall DRLGACTIVATE_RoomExSetStatus_ClientInSight(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD73550
void __fastcall DRLGACTIVATE_RoomExSetStatus_ClientOutOfSight(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD736F0
void __fastcall DRLGACTIVATE_RoomExSetStatus_Untile(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD73790
void __fastcall DRLGACTIVATE_RoomExIdentifyRealStatus(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD73880
void __fastcall DRLGACTIVATE_RoomExStatusUnset_Untile(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD739A0
void __fastcall DRLGACTIVATE_SetClientIsInSight(D2DrlgStrc* pDrlg, int nLevelId, int nX, int nY, D2DrlgRoomStrc* pDrlgRoomHint);
//D2Common.0x6FD73A30
void __fastcall DRLGACTIVATE_RoomExPropagateSetStatus(void* pMemPool, D2DrlgRoomStrc* pDrlgRoom, uint8_t nStatus);
//D2Common.0x6FD73B40
void __fastcall DRLGACTIVATE_UnsetClientIsInSight(D2DrlgStrc* pDrlg, int nLevelId, int nX, int nY, D2DrlgRoomStrc* pDrlgRoomHint);
//D2Common.0x6FD73BE0
void __fastcall DRLGACTIVATE_RoomExPropagateUnsetStatus(D2DrlgRoomStrc* pDrlgRoom, uint8_t nStatus);
//D2Common.0x6FD73C40
void __fastcall DRLGACTIVATE_ChangeClientRoom(D2DrlgRoomStrc* pPreviousRoom, D2DrlgRoomStrc* pNewRoom);
//D2Common.0x6FD73CF0
void __fastcall DRLGACTIVATE_InitializeRoomEx(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD73D80
D2ActiveRoomStrc* __fastcall DRLGACTIVATE_StreamRoomAtCoords(D2DrlgStrc* pDrlg, int nX, int nY);
//D2Common.0x6FD73E30
void __fastcall DRLGACTIVATE_InitializeRoomExStatusLists(D2DrlgStrc* pDrlg);
//D2Common.0x6FD73E60
D2ActiveRoomStrc* __fastcall DRLGACTIVATE_GetARoomInClientSight(D2DrlgStrc* pDrlg);
//D2Common.0x6FD73E90
D2ActiveRoomStrc* __fastcall DRLGACTIVATE_GetARoomInSightButWithoutClient(D2DrlgStrc* pDrlg, D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD73EF0 (#10015)
void __fastcall DRLGACTIVATE_GetRoomsAllocationStats(int* pOutStatsClientAllocatedRooms, int* pOutStatsClientFreedRooms, int* pOutStatsAllocatedRooms, int* pOutStatsFreedRooms);
//D2Common.0x6FD73F20 (#10003)
void __stdcall DRLGACTIVATE_Update(D2DrlgStrc* pDrlg);
//D2Common.0x6FD74060
BOOL __fastcall DRLGACTIVATE_TestRoomCanUnTile(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD740F0
void __fastcall DRLGACTIVATE_ToggleHasPortalFlag(D2DrlgRoomStrc* pDrlgRoom, BOOL bReset);
//D2Common.0x6FD74110
uint8_t __fastcall DRLGACTIVATE_GetRoomStatusFlags(D2DrlgRoomStrc* pDrlgRoom);
