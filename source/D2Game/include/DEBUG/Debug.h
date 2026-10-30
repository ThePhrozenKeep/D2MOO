#pragma once

#include <Units/Units.h>


//D2Game.0x6FCCD2E0 (#10025)
//1.14d: 0x00534650
D2UnitStrc* __stdcall DEBUG_GetUnitFromTypeAndGUID(int32_t nUnitType, int32_t nUnitGUID);
//D2Game.0x6FCCD310 (#10026)
//1.14d: 0x00534680
D2DrlgActStrc* __stdcall DEBUG_GetAct(uint8_t nAct);
//D2Game.0x6FCCD350 (#10027)
//1.14d: 0x005346C0
int32_t __stdcall DEBUG_GetMonsterCount();
//D2Game.0x6FCCD3B0 (#10028)
//1.14d: 0x00534730
D2ActiveRoomStrc* __stdcall DEBUG_GetRoomBySubtileCoordinates(uint8_t nAct, int32_t nX, int32_t nY);
