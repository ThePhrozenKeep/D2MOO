#pragma once

#include "CommonDefinitions.h"
#include <D2Seed.h>
#include "Path.h"
#pragma pack(1)

#pragma pack()


//1.10:  D2Common.0x1005E6D0
//1.10f: D2Common.0x6FDAC5E0
void __fastcall PATH_GetDirectionVector(D2CoordStrc* pDirectionVector, int* pOutDirection, DWORD dwStartPrecisionX, DWORD dwStartPrecisionY, DWORD dwTargetPrecisionX, DWORD dwTargetPrecisionY);
//1.00:  D2Common.0x1005F220
//1.10f: D2Common.0x6FDACC40
void __fastcall sub_6FDACC40(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* a2, unsigned int a3, unsigned int a4);
//1.00:  D2Common.0x1005F420
//1.10f: D2Common.0x6FDACEC0
BOOL __fastcall sub_6FDACEC0(D2DynamicPathStrc* pDynamicPath, D2FP32_16* a2, D2UnitStrc** pUnit);
//1.00:  D2Common.0x1005FAB0
//1.10f: D2Common.0x6FDAD5E0
BOOL __fastcall sub_6FDAD5E0(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pDestRoom, D2PathPointStrc tDest);
//1.00:  D2Common.0x100604E0 Note: Changed since then. Used to look for the room using coordinates
//1.10f: D2Common.0x6FDAE250
void __fastcall PATHMISC_SetRoom(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pRoom);


//1.00:  D2Common.0x10060640 (#10227)
//1.10f: D2Common.0x6FDADF00 (#10230)
BOOL __stdcall D2Common_10230(D2DynamicPathStrc* pDynamicPath, int a2, D2RoomStrc* pRoom, unsigned int a4, __int16 a5);
//1.00:  D2Common.0x10060120 (#10228)
//1.10f: D2Common.0x6FDADC20 (#10231)
signed int __stdcall D2Common_10231(D2DynamicPathStrc* a1, D2UnitStrc* a2, D2RoomStrc* pRooms, uint16_t nX, uint16_t nY);
//1.00:  D2Common.0x100606B0 (#10229)
//1.10f: D2Common.0x6FDADF50 (#10232)
signed int __stdcall D2Common_10232(D2DynamicPathStrc* a1, D2UnitStrc* a2, unsigned int pNumRooms, int a4, int pppRoom);
//1.00:  D2Common.0x10060A60 (#10230)
//1.10f: D2Common.0x6FDAE290 (#10233)
void __stdcall D2Common_10233(D2DynamicPathStrc* pDynamicPath);
//1.00:  D2Common.0x10060C40 (#10231)
//1.10f: D2Common.0x6FDAE500 (#10234)
BOOL __stdcall D2Common_10234(D2DynamicPathStrc* pDynamicPath);
//1.00:  D2Common.0x10060C60 (#10231)
//1.10f: D2Common.0x6FDAE520 (#10235)
void __stdcall D2Common_10235_PATH_UpdateRiderPath(D2UnitStrc* pRiderUnit, D2UnitStrc* pMountUnit);



