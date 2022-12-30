#pragma once

#include "CommonDefinitions.h"

struct D2RoomStrc;
struct D2RoomExStrc;
struct D2DrlgStrc;

#pragma pack(1)


#pragma pack()

//D2Common.0x6FD733D0
void __fastcall DRLGACTIVATE_RoomExSetStatus_ClientInRoom(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73450
void __fastcall DRLGACTIVATE_RoomExSetStatus_ClientInSight(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73550
void __fastcall DRLGACTIVATE_RoomExSetStatus_ClientOutOfSight(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD736F0
void __fastcall DRLGACTIVATE_RoomExSetStatus_Untile(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73790
void __fastcall DRLGACTIVATE_RoomExIdentifyRealStatus(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73880
void __fastcall DRLGACTIVATE_RoomExStatusUnset_Untile(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD739A0
void __fastcall DRLGACTIVATE_SetClientIsInSight(D2DrlgStrc* pDrlg, int nLevelId, int nX, int nY, D2RoomExStrc* pRoomExHint);
//D2Common.0x6FD73A30
void __fastcall DRLGACTIVATE_RoomExPropagateSetStatus(void* pMemPool, D2RoomExStrc* pRoomEx, uint8_t nStatus);
//D2Common.0x6FD73B40
void __fastcall DRLGACTIVATE_UnsetClientIsInSight(D2DrlgStrc* pDrlg, int nLevelId, int nX, int nY, D2RoomExStrc* pRoomExHint);
//D2Common.0x6FD73BE0
void __fastcall DRLGACTIVATE_RoomExPropagateUnsetStatus(D2RoomExStrc* pRoomEx, uint8_t nStatus);
//D2Common.0x6FD73C40
void __fastcall DRLGACTIVATE_ChangeClientRoom(D2RoomExStrc* pPreviousRoom, D2RoomExStrc* pNewRoom);
//D2Common.0x6FD73CF0
void __fastcall DRLGACTIVATE_InitializeRoomEx(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73D80
D2RoomStrc* __fastcall DRLGACTIVATE_StreamRoomAtCoords(D2DrlgStrc* pDrlg, int nX, int nY);
//D2Common.0x6FD73E30
void __fastcall DRLGACTIVATE_InitializeRoomExStatusLists(D2DrlgStrc* pDrlg);
//D2Common.0x6FD73E60
D2RoomStrc* __fastcall DRLGACTIVATE_GetARoomInClientSight(D2DrlgStrc* pDrlg);
//D2Common.0x6FD73E90
D2RoomStrc* __fastcall DRLGACTIVATE_GetARoomInSightButWithoutClient(D2DrlgStrc* pDrlg, D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73EF0 (#10015)
void __fastcall DRLGACTIVATE_GetRoomsAllocationStats(int* pOutStatsClientAllocatedRooms, int* pOutStatsClientFreedRooms, int* pOutStatsAllocatedRooms, int* pOutStatsFreedRooms);
//D2Common.0x6FD73F20 (#10003)
void __stdcall DRLGACTIVATE_Update(D2DrlgStrc* pDrlg);
//D2Common.0x6FD74060
BOOL __fastcall DRLGACTIVATE_TestRoomCanUnTile(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD740F0
void __fastcall DRLGACTIVATE_ToggleHasPortalFlag(D2RoomExStrc* pRoomEx, BOOL bReset);
//D2Common.0x6FD74110
uint8_t __fastcall DRLGACTIVATE_GetRoomStatusFlags(D2RoomExStrc* pRoomEx);
