#pragma once

#include "D2Structs.h"
#include <Units/Units.h>


//D2Game.0x6FC601C0
void __fastcall MONSTER_SetAiState(D2UnitStrc* pMonster, int32_t nAiState);
//D2Game.0x6FC601E0
int32_t __fastcall MONSTER_GetAiState(D2UnitStrc* pMonster);
//D2Game.0x6FC60200
void __fastcall MONSTER_SetLevelId(D2UnitStrc* pMonster, int32_t nLevelId);
//D2Game.0x6FC60220
int32_t __fastcall MONSTER_GetLevelId(D2UnitStrc* pMonster);
//D2Game.0x6FC60240
int32_t __fastcall MONSTER_CheckSummonerFlag(D2UnitStrc* pMonster, uint16_t nFlag);
//D2Game.0x6FC60270
void __fastcall MONSTER_ToggleSummonerFlag(D2UnitStrc* pMonster, uint16_t nFlag, int32_t bSet);
//D2Game.0x6FC602A0
void __fastcall MONSTER_Initialize(D2GameStrc* pGame, D2RoomStrc* pRoom, D2UnitStrc* pMonster, int32_t nUnitGUID);
//D2Game.0x6FC603D0
void __fastcall MONSTER_InitializeStatsAndSkills(D2GameStrc* pGame, D2RoomStrc* pRoom, D2UnitStrc* pUnit, D2MonRegDataStrc* pMonRegData);
//D2Game.0x6FC60B10
int32_t __fastcall MONSTER_SetVelocityAndPosition(D2GameStrc* pGame, D2UnitStrc* pMonster, int32_t nX, int32_t nY, int32_t a5);
//D2Game.0x6FC60BC0
void __fastcall MONSTER_RemoveAll(D2GameStrc* pGame);
//D2Game.0x6FC60C10
void __fastcall MONSTER_Free(D2GameStrc* pGame, D2UnitStrc* pMonster);
//D2Game.0x6FC60CD0
void __fastcall MONSTER_UpdateAiCallbackEvent(D2GameStrc* pGame, D2UnitStrc* pMonster);
//D2Game.0x6FC60E50
void __fastcall MONSTER_DeleteEvents(D2GameStrc* pGame, D2UnitStrc* pMonster);
//D2Game.0x6FC60E70
int32_t __fastcall MONSTER_GetHpBonus(int32_t nPlayerCount);
//Inlined in D2Game.0x6FC60E90
int32_t __fastcall MONSTER_GetExperienceBonus(int32_t nPlayerCount);
//D2Game.0x6FC60E90
void __fastcall MONSTER_GetPlayerCountBonus(D2GameStrc* pGame, D2PlayerCountBonusStrc* pPlayerCountBonus, D2RoomStrc* pRoom, D2UnitStrc* pMonster);
//D2Game.0x6FC60F70
void __fastcall MONSTER_SetComponents(D2MonRegDataStrc* pMonRegData, D2UnitStrc* pUnit);
//D2Game.0x6FC610C0
int32_t __fastcall MONSTER_HasComponents(D2UnitStrc* pMonster);
//D2Game.0x6FC610F0
int32_t __fastcall MONSTER_Reinitialize(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nClassId, int32_t nMode);
