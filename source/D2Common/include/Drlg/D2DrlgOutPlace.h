#pragma once

#include "CommonDefinitions.h"

//D2Common.0x6FD80480
void __fastcall DRLGOUTPLACE_BuildKurast(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD806A0
void __fastcall DRLGOUTPLACE_InitAct3OutdoorLevel(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD80750
BOOL __fastcall sub_6FD80750(D2DrlgLevelStrc* pLevel, int a2);
//D2Common.0x6FD80BE0
int __fastcall sub_6FD80BE0(int a1, int a2, int a3);
//D2Common.0x6FD80C10
int __fastcall sub_6FD80C10(int a1, int a2, int a3, int a4, int a5);
//D2Common.0x6FD80C80
void __fastcall DRLGOUTPLACE_SetBlankBorderGridCells(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD80DA0
void __fastcall DRLGOUTPLACE_SetOutGridLinkFlags(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD80E10
void __fastcall DRLGOUTPLACE_PlaceAct1245OutdoorBorders(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD81330
BOOL __fastcall sub_6FD81330(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD81380
BOOL __fastcall sub_6FD81380(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD81430
void __fastcall sub_6FD81430(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int a3, int a4);
//D2Common.0x6FD81530
BOOL __fastcall sub_6FD81530(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD815E0
void __fastcall sub_6FD815E0(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int a3, int a4);
//D2Common.0x6FD81720
BOOL __fastcall sub_6FD81720(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD81850
void __fastcall sub_6FD81850(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int a3, int a4);
//D2Common.0x6FD81950
BOOL __fastcall sub_6FD81950(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD81AD0
BOOL __fastcall sub_6FD81AD0(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD81B30
BOOL __fastcall sub_6FD81B30(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD81BF0
BOOL __fastcall sub_6FD81BF0(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD81CA0
BOOL __fastcall sub_6FD81CA0(D2DrlgLevelLinkDataStrc* pLevelLinkData);
//D2Common.0x6FD81D60
void __fastcall DRLGOUTPLACE_CreateLevelConnections(D2DrlgStrc* pDrlg, uint8_t nActNo);
//D2Common.0x6FD82050
BOOL __fastcall sub_6FD82050(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration);
//D2Common.0x6FD82130
BOOL __fastcall sub_6FD82130(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration);
//D2Common.0x6FD821E0
BOOL __fastcall DRLGOUTPLACE_LinkAct2Outdoors(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration);
//D2Common.0x6FD82240
BOOL __fastcall DRLGOUTPLACE_LinkAct2Canyon(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration);
//D2Comon.0x6FD822A0
BOOL __fastcall DRLGOUTPLACE_LinkAct4Outdoors(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration);
//D2Common.0x6FD82300
BOOL __fastcall DRLGOUTPLACE_LinkAct4ChaosSanctum(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration);
//D2Common.0x6FD82360
void __fastcall sub_6FD82360(D2DrlgLevelStrc* pLevel, int nIteration, int* pRand);
//D2Common.0x6FD823C0
void __fastcall sub_6FD823C0(D2DrlgStrc* pDrlg, D2DrlgLinkStrc* pDrlgLink, int(__fastcall* a3)(D2DrlgLevelLinkDataStrc*, int), void(__fastcall* a4)(D2DrlgLevelStrc*, int, int*));
//D2Common.0x6FD826D0
void __fastcall sub_6FD826D0(D2DrlgStrc* pDrlg, int nStartId, int nEndId);
//D2Common.0x6FD82750
void __fastcall sub_6FD82750(D2DrlgStrc* pDrlg, int nStartId, int nEndId);
//D2Common.0x6FD82820
D2DrlgLevelStrc* __fastcall sub_6FD82820(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD83970
void __fastcall sub_6FD83970(D2DrlgCoordStrc* pDrlgCoord, D2UnkOutPlaceStrc* a2, int nRand, int nSizeX, int nSizeY);
//D2Common.0x6FD83A20
void __fastcall DRLGOUTPLACE_InitOutdoorRoomGrids(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD83C90
void __fastcall DRLGOUTPLACE_CreateOutdoorRoomEx(D2DrlgLevelStrc* pLevel, int nX, int nY, int nWidth, int nHeight, int a6, int dwFlags, int dwFlagsEx, int dwDT1Mask);
