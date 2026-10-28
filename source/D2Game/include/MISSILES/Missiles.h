#pragma once

#include <Units/Units.h>


//D2Game.0x6FC552A0
//1.14d: 0x0059F840
void __fastcall MISSILES_RemoveAll(D2GameStrc* pGame);
//D2Game.0x6FC552F0
//1.14d: 0x0059F8A0
void __fastcall MISSILES_Initialize(D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nUnitGUID);
//D2Game.0x6FC55340
//1.14d: 0x0059F8E0
void __fastcall MISSILES_Free(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC55360
//1.14d: 0x0059FA30
D2UnitStrc* __fastcall MISSILES_CreateMissileFromParams(D2GameStrc* pGame, D2MissileStrc* missileParams);
//D2Game.0x6FC55B70
//1.14d: 0x0059FEE0
void __fastcall MISSILES_SyncToClient(D2ClientStrc* pClient, D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nVelocity);
