#pragma once

#include <Units/Units.h>

#include <GAME/Game.h>

//D2Game.0x6FC40170
//1.14d: 0x005B1880
void __fastcall TARGETS_AllocPlayerNode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nNodeData);
//D2Game.0x6FC401F0
//1.14d: 0x005B1900
void __fastcall D2GAME_UpdateSummonAI_6FC401F0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nNodeData, int32_t nNodeIndex);
//D2Game.0x6FC40280
//1.14d: 0x005B1990
void __fastcall TARGETS_InsertUnitNode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nNodeData, int32_t nNodeIndex);
//D2Game.0x6FC40310
//1.14d: 0x005B1A20
void __fastcall D2GAME_FreeUnitNode_6FC40310(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC40380
//1.14d: 0x005B1A90
void __fastcall TARGETS_RemoveUnitNode(D2GameStrc* pGame, D2UnitStrc* pUnit);
