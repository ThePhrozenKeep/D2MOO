#pragma once

#include "CommonDefinitions.h"

#include <DataTbls/FieldTbls.h>

struct D2ActiveRoomStrc;
struct D2UnitStrc;

#pragma pack(1)

#define UNIT_FIND_ARRAY_SIZE 15

using UNITFINDTEST = int32_t(__fastcall*)(D2UnitStrc* pUnit, struct D2UnitFindArgStrc* pUnitFindArg);

struct D2UnitFindArgStrc
{
	int32_t nFlags;								//0x00
	int32_t unk0x04;							//0x04
	D2UnitStrc* pUnit;							//0x08
	int32_t nX;									//0x0C
	int32_t nY;									//0x10
	int32_t nSize;								//0x14
	int32_t nMaxArrayEntries;					//0x18
	int32_t nIndex;								//0x1C
	D2FieldStrc* pField;						//0x20
	UNITFINDTEST pfnUnitTest;					//0x24
	int32_t unk0x28;							//0x28
	int32_t unk0x2C;							//0x2C
	int32_t unk0x30;							//0x30
	int32_t unk0x34;							//0x34
};

struct D2UnitFindDataStrc
{
	void* pMemPool;								//0x00
	D2UnitStrc** pUnitsArray;					//0x04
	D2ActiveRoomStrc* pRoom;					//0x08
	UNITFINDTEST pfnUnitTest;					//0x0C
	D2UnitFindArgStrc* pUnitFindArg;			//0x10
	int32_t nFlags;								//0x14
	int32_t nIndex;								//0x18
	int32_t nMaxArrayEntries;					//0x1C
	int32_t nX;									//0x20
	int32_t nY;									//0x24
	int32_t nSize;								//0x28
};

#pragma pack()

//D2Common.0x6FDBC680 (#10408)
D2COMMON_DLL_DECL BOOL __stdcall UNITFINDS_AreUnitsInNeighboredRooms(D2UnitStrc* pDestUnit, D2UnitStrc* pSrcUnit);
//D2Common.0x6FDBC720 (#11087)
D2COMMON_DLL_DECL D2UnitStrc* __stdcall UNITFINDS_FindUnitInNeighboredRooms(D2ActiveRoomStrc* pRoom, int nUnitType, int nClassId);
//D2Common.0x6FDBC7B0 (#10405)
D2COMMON_DLL_DECL int __stdcall UNITFINDS_GetTestedUnitsFromRoom(D2ActiveRoomStrc* pRoom, D2UnitStrc** ppUnits, UNITFINDTEST pfnUnitTest, D2UnitFindArgStrc* pUnitFindArg);
//D2Common.0x6FDBC840 (#11088)
D2COMMON_DLL_DECL D2UnitStrc* __stdcall UNITFINDS_GetNearestTestedUnit(D2UnitStrc* pUnit, int nX, int nY, int nSize, int(__fastcall* pfnUnitTest)(D2UnitStrc*, D2UnitStrc*));
//D2Common.0x6FDBC990 (#10401)
D2COMMON_DLL_DECL void __stdcall UNITFINDS_InitializeUnitFindData(void* pMemPool, D2UnitFindDataStrc* pUnitFindData, D2ActiveRoomStrc* pRoom, int nX, int nY, int nSize, UNITFINDTEST pfnUnitTest, D2UnitFindArgStrc* pUnitFindArg);
//D2Common.0x6FDBCA50 (#10402)
D2COMMON_DLL_DECL void __stdcall UNITFINDS_FreeUnitFindData(D2UnitFindDataStrc* pUnitFindData);
//D2Common.0x6FDBCA80 (#10403)
D2COMMON_DLL_DECL void __stdcall UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(D2UnitFindDataStrc* pUnitFindData);
//D2Common.0x6FDBCCA0 (#10404)
D2COMMON_DLL_DECL int __stdcall UNITFINDS_TestUnit(D2UnitStrc* pUnit, D2UnitFindArgStrc* pUnitFindArg);
