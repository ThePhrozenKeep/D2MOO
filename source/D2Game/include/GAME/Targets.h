#pragma once

#include "D2Structs.h"
#include <Units/Units.h>

#include <GAME/Game.h>

//D2Game.0x6FC40170
void __fastcall D2GAME_TARGETS_First_6FC40170(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3);
//D2Game.0x6FC401F0
void __fastcall D2GAME_UpdateSummonAI_6FC401F0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t nNodeIndex);
//D2Game.0x6FC40280
void __fastcall sub_6FC40280(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t nNodeIndex);
//D2Game.0x6FC40310
void __fastcall D2GAME_FreeUnitNode_6FC40310(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC40380
void __fastcall D2GAME_TARGETS_Last_6FC40380(D2GameStrc* pGame, D2UnitStrc* pUnit);
