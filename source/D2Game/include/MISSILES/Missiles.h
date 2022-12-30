#pragma once

#include <Units/Units.h>


//D2Game.0x6FC552A0
void __fastcall MISSILES_RemoveAll(D2GameStrc* pGame);
//D2Game.0x6FC552F0
void __fastcall MISSILES_Initialize(D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nUnitGUID);
//D2Game.0x6FC55340
void __fastcall MISSILES_Free(D2GameStrc* pGame, D2UnitStrc* pMissile);
//D2Game.0x6FC55360
D2UnitStrc* __fastcall MISSILES_CreateMissileFromParams(D2GameStrc* pGame, D2MissileStrc* missileParams);
//D2Game.0x6FC55B70
void __fastcall MISSILES_SyncToClient(D2ClientStrc* pClient, D2GameStrc* pGame, D2UnitStrc* pMissile, int32_t nVelocity);
