#pragma once

#include "CommonDefinitions.h"


//D2Common.0x6FDBC230 (#11213)
void __stdcall MISSTREAM_AllocMissileStream(D2UnitStrc* pMissile);
//D2Common.0x6FDBC280 (#11214)
void __stdcall MISSTREAM_FreeMissileStream(D2UnitStrc* pMissile);
//D2Common.0x6FDBC2E0 (#11215)
void __stdcall MISSTREAM_ExecuteHit(D2UnitStrc* pUnit, int nCollisionType, int nCollisionPattern, void (__fastcall* pfnHit)(D2UnitStrc*, D2UnitStrc*));
//D2Common.0x6FDBC3B0
int __fastcall MISSTREAM_Return1(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2);
//D2Common.0x6FDBC3C0 (#11216)
void __stdcall MISSTREAM_Update(D2UnitStrc* a1, D2UnitStrc* pMissile, int a3, int a4, int (__fastcall* pfCreate)(D2UnitStrc*, int, int, int, int, int), void (__fastcall* a6)(int));
