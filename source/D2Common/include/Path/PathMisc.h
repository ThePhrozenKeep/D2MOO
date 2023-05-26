#pragma once

#include "CommonDefinitions.h"
#include <D2Seed.h>
#include "Path.h"
#pragma pack(1)

#pragma pack()

//D2Common.0x6FDAA880
BOOL __fastcall sub_6FDAA880(D2PathInfoStrc* pPathInfo, int* pTestDir, D2PathPointStrc pPoint, int* pDirection);

//D2Common.0x6FDABA50
int __stdcall  sub_6FDABA50(D2PathPointStrc pPoint1, D2PathPointStrc pPoint2);
//D2Common.0x6FDAB750
int __fastcall sub_6FDAB750(int nX1, int nY1, int nX2, int nY2);

//D2Common.0x6FDAB0B0
int __fastcall sub_6FDAB0B0(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDAA9F0
int __fastcall sub_6FDAA9F0(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDAB130
int __fastcall sub_6FDAB130(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDAB270
int __fastcall sub_6FDAB270(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDAB1E0
int __fastcall sub_6FDAB1E0(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDAB240
int __fastcall sub_6FDAB240(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDAB0C0
int __fastcall sub_6FDAB0C0(D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDAB790
void __fastcall PATH_GetDirections_6FDAB790(int* pTestDir, D2PathPointStrc pPoint1, D2PathPointStrc pPoint2);

//D2Common.0x6FDAB3C0
signed int __fastcall PATH_ComputePathBlessedHammer_6FDAB3C0(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAAD10
BOOL __fastcall sub_6FDAAD10(D2DynamicPathStrc* a1, D2PathPointStrc* a2, D2PathPointStrc a3);

//D2Common.0x6FDAB4A0
int __fastcall PATH_ComputePathChargedBolt_6FDAB4A0(D2DynamicPathStrc* pDynamicPath, D2SeedStrc* pSeed);

//D2Common.0x6FDAB610
int __fastcall sub_6FDAB610(int nX1, int nY1, int nX2, int nY2);

//D2Common.0x6FDAC5E0
void __fastcall PATH_GetDirectionVector_6FDAC5E0(D2CoordStrc* pDirectionVector, int* pOutDirection, DWORD dwStartPrecisionX, DWORD dwStartPrecisionY, DWORD dwTargetPrecisionX, DWORD dwTargetPrecisionY);

//D2Common.0x6FDAC700 (#10215)
int __stdcall PATH_ComputeDirection(int nX1, int nY1, int nX2, int nY2);

//D2Common.0x6FDAC760
int __stdcall PATH_ComputeDirectionFromPreciseCoords_6FDAC760(DWORD dwStartPrecisionX, DWORD dwStartPrecisionY, DWORD dwTargetPrecisionX, DWORD dwTargetPrecisionY);

//D2Common.0x6FDAC790
void __fastcall sub_6FDAC790(D2DynamicPathStrc* pPath, int a2, int a3);

//D2Common.0x6FDAC8F0 (#10236)
int __stdcall D2Common_10236(D2UnitStrc* pUnit, int a2);

//D2Common.0x6FDACC40
void __fastcall sub_6FDACC40(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* a2, unsigned int a3, unsigned int a4);
//D2Common.0x6FDACEC0
BOOL __fastcall sub_6FDACEC0(D2DynamicPathStrc* pDynamicPath, D2FP32_16* a2, D2UnitStrc** pUnit);

//D2Common.0x6FDAC9A0 (#10226)
BOOL __stdcall D2Common_10226(D2UnitStrc* pUnit, signed int a2);

//D2Common.0x6FDAD330
int __fastcall sub_6FDAD330(D2PathInfoStrc* ptPathInfo);

//D2Common.0x6FDAD530 (#10227)
BOOL __stdcall D2Common_10227(D2UnitStrc* pUnit);

//D2Common.0x6FDAD590 (#10229)
BOOL __stdcall D2Common_10229(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit, D2RoomStrc* pDestRoom, uint32_t nDestX, uint32_t nDestY);

//D2Common.0x6FDAD5E0
BOOL __fastcall sub_6FDAD5E0(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pDestRoom, D2PathPointStrc tDest);

//D2Common.0x6FDADA20
void __fastcall PATH_RecacheRoom(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pHintRoom);

//D2Common.0x6FDAE250
void __fastcall PATHMISC_SetRoom(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pRoom);

//D2Common.0x6FDAE290
void __stdcall D2Common_10233(D2DynamicPathStrc* pDynamicPath);




//D2Common.0x6FDADF00 (#10230)
BOOL __stdcall D2Common_10230(D2DynamicPathStrc* pDynamicPath, int a2, D2RoomStrc* pRoom, unsigned int a4, __int16 a5);
//D2Common.0x6FDADC20 (#10231)
signed int __stdcall D2Common_10231(D2DynamicPathStrc * a1, D2UnitStrc * a2, D2RoomStrc * pRooms, int nX, int nY);

BOOL __stdcall D2Common_10234(D2DynamicPathStrc* pDynamicPath);
//D2Common.0x6FDAE520 (#10235)
void __stdcall D2Common_10235_PATH_UpdateRiderPath(D2UnitStrc* pRiderUnit, D2UnitStrc* pMountUnit);

