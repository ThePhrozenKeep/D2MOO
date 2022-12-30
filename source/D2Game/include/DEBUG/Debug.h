#pragma once

#include <Units/Units.h>


//D2Game.0x6FCCD2E0 (#10025)
D2UnitStrc* __stdcall DEBUG_GetUnitFromTypeAndGUID(int32_t nUnitType, int32_t nUnitGUID);
//D2Game.0x6FCCD310 (#10026)
D2DrlgActStrc* __stdcall DEBUG_GetAct(uint8_t nAct);
//D2Game.0x6FCCD350 (#10027)
int32_t __stdcall DEBUG_GetFreeMonsterIndex();
//D2Game.0x6FCCD3B0 (#10028)
D2RoomStrc* __stdcall DEBUG_GetRoomBySubtileCoordinates(uint8_t nAct, int32_t nX, int32_t nY);
