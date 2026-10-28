#pragma once

#include <Units/Units.h>


//D2Game.0x6FC7EA50
//1.14d: 0x005706D0
void __fastcall PLAYERSTATS_SetStatsForStartingAct(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nAct);
//D2Game.0x6FC7EC00
//1.14d: 0x00570880
void __fastcall PLAYERSTATS_LevelUp(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC7EDE0
//1.14d: 0x00570850
void __fastcall PLAYERSTATS_OnPlayerLeveledUp(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pLeveledUpPlayer);
//D2Game.0x6FC7EDF0
//1.14d: 0x00570D60
int32_t __fastcall PLAYERSTATS_SpendStatPoint(D2UnitStrc* pUnit, int32_t nStatId);
