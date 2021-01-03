#pragma once

#include "CommonDefinitions.h"

#pragma pack(1)


#pragma pack()

//D2Common.0x6FD733D0
void __fastcall sub_6FD733D0(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73450
void __fastcall sub_6FD73450(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73550
void __fastcall sub_6FD73550(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD736F0
void __fastcall sub_6FD736F0(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73790
void __fastcall sub_6FD73790(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73880
void __fastcall sub_6FD73880(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD739A0
void __fastcall sub_6FD739A0(D2DrlgStrc* pDrlg, int nLevelId, int nX, int nY, D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73A30
void __fastcall sub_6FD73A30(void* pMemPool, D2RoomExStrc* pRoomEx, uint8_t a3);
//D2Common.0x6FD73B40
void __fastcall sub_6FD73B40(D2DrlgStrc* pDrlg, int nLevelId, int nX, int nY, D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73BE0
void __fastcall sub_6FD73BE0(D2RoomExStrc* pRoomEx, uint8_t a2);
//D2Common.0x6FD73C40
void __fastcall sub_6FD73C40(D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2);
//D2Common.0x6FD73CF0
void __fastcall DRLGACTIVATE_InitializeRoomEx(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73D80
D2RoomStrc* __fastcall sub_6FD73D80(D2DrlgStrc* pDrlg, int nX, int nY);
//D2Common.0x6FD73E30
void __fastcall sub_6FD73E30(D2DrlgStrc* pDrlg);
//D2Common.0x6FD73E60
D2RoomStrc* __fastcall sub_6FD73E60(D2DrlgStrc* pDrlg);
//D2Common.0x6FD73E90
D2RoomStrc* __fastcall sub_6FD73E90(D2DrlgStrc* pDrlg, D2RoomExStrc* pRoomEx);
//D2Common.0x6FD73EF0
void __fastcall D2Common_10015(int* a1, int* a2, int* a3, int* a4);
//D2Common.0x6FD73F20
void __stdcall D2Common_10003(D2DrlgStrc* pDrlg);
//D2Common.0x6FD74060
BOOL __fastcall DRLGACTIVATE_TestRoomCanUnTile(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD740F0
void __fastcall DRLGACTIVATE_ToggleHasPortalFlag(D2RoomExStrc* pRoomEx, BOOL bReset);
//D2Common.0x6FD74110
uint8_t __fastcall DRLGACTIVATE_GetRoomStatusFlags(D2RoomExStrc* pRoomEx);
