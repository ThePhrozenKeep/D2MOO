#pragma once

#include <Units/Units.h>
#include "MonsterRegion.h"


//D2Game.0x6FC601C0
//1.14d: 0x005734C0
void __fastcall MONSTER_SetAiState(D2UnitStrc* pMonster, int32_t nAiState);
//D2Game.0x6FC601E0
//1.14d: 0x005734E0
int32_t __fastcall MONSTER_GetAiState(D2UnitStrc* pMonster);
//D2Game.0x6FC60200
//1.14d: 0x00573500
void __fastcall MONSTER_SetLevelId(D2UnitStrc* pMonster, int32_t nLevelId);
//D2Game.0x6FC60220
//1.14d: 0x00573520
int32_t __fastcall MONSTER_GetLevelId(D2UnitStrc* pMonster);
//D2Game.0x6FC60240
//1.14d: 0x00573540
int32_t __fastcall MONSTER_CheckSummonerFlag(D2UnitStrc* pMonster, uint16_t nFlag);
//D2Game.0x6FC60270
//1.14d: 0x00573570
void __fastcall MONSTER_ToggleSummonerFlag(D2UnitStrc* pMonster, uint16_t nFlag, int32_t bSet);
//D2Game.0x6FC602A0
//1.14d: 0x00574250
void __fastcall MONSTER_Initialize(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2UnitStrc* pMonster, int32_t nUnitGUID);
//D2Game.0x6FC603D0
//1.14d: 0x00573CB0
void __fastcall MONSTER_InitializeStatsAndSkills(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2UnitStrc* pUnit, D2MonRegDataStrc* pMonRegData);
//D2Game.0x6FC60B10
//1.14d: 0x005735A0
int32_t __fastcall MONSTER_SetVelocityAndPosition(D2GameStrc* pGame, D2UnitStrc* pMonster, int32_t nX, int32_t nY, int32_t bUnusedSkipRoomUpdate);
//D2Game.0x6FC60BC0
//1.14d: 0x00573640
void __fastcall MONSTER_RemoveAll(D2GameStrc* pGame);
//D2Game.0x6FC60C10
//1.14d: 0x005736A0
void __fastcall MONSTER_Free(D2GameStrc* pGame, D2UnitStrc* pMonster);
//D2Game.0x6FC60CD0
//1.14d: 0x00573780
void __fastcall MONSTER_UpdateAiCallbackEvent(D2GameStrc* pGame, D2UnitStrc* pMonster);
//D2Game.0x6FC60E50
//1.14d: 0x005738D0
void __fastcall MONSTER_DeleteEvents(D2GameStrc* pGame, D2UnitStrc* pMonster);
//D2Game.0x6FC60E70
//1.14d: 0x005738F0
int32_t __fastcall MONSTER_GetHpBonus(int32_t nPlayerCount);
//Inlined in D2Game.0x6FC60E90
//1.14d: 0x00573910
int32_t __fastcall MONSTER_GetExperienceBonus(int32_t nPlayerCount);
//D2Game.0x6FC60E90
//1.14d: 0x00573930
void __fastcall MONSTER_GetPlayerCountBonus(D2GameStrc* pGame, D2PlayerCountBonusStrc* pPlayerCountBonus, D2ActiveRoomStrc* pRoom, D2UnitStrc* pMonster);
//D2Game.0x6FC60F70
//1.14d: 0x005739D0
void __fastcall MONSTER_SetComponents(D2MonRegDataStrc* pMonRegData, D2UnitStrc* pUnit);
//D2Game.0x6FC610C0
//1.14d: 0x00573AE0
int32_t __fastcall MONSTER_HasComponents(D2UnitStrc* pMonster);
//D2Game.0x6FC610F0
//1.14d: 0x00574370
int32_t __fastcall MONSTER_Reinitialize(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nClassId, int32_t nMode);
