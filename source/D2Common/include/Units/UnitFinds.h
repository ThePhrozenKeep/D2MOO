#pragma once

#include "CommonDefinitions.h"


#define UNIT_FIND_ARRAY_SIZE 15

//D2Common.0x6FDBC680 (#10408)
BOOL __stdcall UNITFINDS_AreUnitsInNeighboredRooms(D2UnitStrc* pDestUnit, D2UnitStrc* pSrcUnit);
//D2Common.0x6FDBC720 (#11087)
D2UnitStrc* __stdcall UNITFINDS_FindUnitInNeighboredRooms(D2RoomStrc* pRoom, int nUnitType, int nClassId);
//D2Common.0x6FDBC7B0 (#10405)
int __stdcall UNITFINDS_GetTestedUnitsFromRoom(D2RoomStrc* pRoom, D2UnitStrc** ppUnits, UNITFINDTEST pfnUnitTest, D2UnitFindArgStrc* pUnitFindArg);
//D2Common.0x6FDBC840 (#11088)
D2UnitStrc* __stdcall UNITFINDS_GetNearestTestedUnit(D2UnitStrc* pUnit, int nX, int nY, int nSize, int(__fastcall* pfnUnitTest)(D2UnitStrc*, D2UnitStrc*));
//D2Common.0x6FDBC990 (#10401)
void __stdcall UNITFINDS_InitializeUnitFindData(void* pMemPool, D2UnitFindDataStrc* pUnitFindData, D2RoomStrc* pRoom, int nX, int nY, int nSize, UNITFINDTEST pfnUnitTest, D2UnitFindArgStrc* pUnitFindArg);
//D2Common.0x6FDBCA50 (#10402)
void __stdcall UNITFINDS_FreeUnitFindData(D2UnitFindDataStrc* pUnitFindData);
//D2Common.0x6FDBCA80 (#10403)
void __stdcall UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(D2UnitFindDataStrc* pUnitFindData);
//D2Common.0x6FDBCCA0 (#10404)
int __stdcall UNITFINDS_TestUnit(D2UnitStrc* pUnit, D2UnitFindArgStrc* pUnitFindArg);
