#pragma once

#include "CommonDefinitions.h"

#pragma pack(1)



#pragma pack()

//D2Common.0x6FDBCF10 (#11279)
D2COMMON_DLL_DECL int __stdcall UNITROOM_AddUnitToRoomEx(D2UnitStrc* pUnit, D2RoomStrc* pRoom, int nUnused);
//D2Common.0x6FDBD100 (#10384)
D2COMMON_DLL_DECL int __stdcall UNITROOM_AddUnitToRoom(D2UnitStrc* pUnit, D2RoomStrc* pRoom);
//D2Common.0x6FDBD120 (#10385)
D2COMMON_DLL_DECL void __stdcall UNITROOM_RefreshUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDBD1B0 (#10388)
D2COMMON_DLL_DECL void __stdcall UNITROOM_SortUnitListByTargetY(D2RoomStrc* pRoom);
//D2Common.0x6FDBD250 (#10390)
D2COMMON_DLL_DECL void __stdcall UNITROOM_UpdatePath(D2UnitStrc* pUnit);
//D2Common.0x6FDBD2B0 (#10391)
D2COMMON_DLL_DECL void __stdcall UNITROOM_ClearUpdateQueue(D2RoomStrc* pRoom);
//D2Common.0x6FDBD300 (#10386)
D2COMMON_DLL_DECL void __stdcall UNITROOM_RemoveUnitFromRoom(D2UnitStrc* pUnit);
//D2Common.0x6FDBD400 (#10387)
D2COMMON_DLL_DECL void __stdcall UNITROOM_RemoveUnitFromUpdateQueue(D2UnitStrc* pUnit);
//D2Common.0x6FDBD4C0 (#10389)
D2COMMON_DLL_DECL BOOL __stdcall UNITROOM_IsUnitInRoom(D2RoomStrc* pRoom, D2UnitStrc* pUnit);
