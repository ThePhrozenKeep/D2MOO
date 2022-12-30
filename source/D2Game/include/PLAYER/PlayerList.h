#pragma once

#include <Units/Units.h>


//D2Game.0x6FCBA690
void __fastcall PLAYERLIST_FreePlayerLists(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FCBA6E0
void __fastcall PLAYERLIST_IterateCallback_FreePlayerList(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg);
//Inlined in some functions
D2PlayerListStrc* __fastcall PLAYERLIST_GetPlayerListRecordFromUnitGUID(D2UnitStrc* pPlayer, int32_t nUnitGUID);
//D2Game.0x6FCBA750
void __fastcall PLAYERLIST_AllocPlayerList(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBA840
void __fastcall PLAYERLIST_ToggleFlag(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, uint32_t nFlags, int32_t bSet);
//D2Game.0x6FCBA8E0
int32_t __fastcall PLAYERLIST_CheckFlag(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, uint32_t nFlags);
//D2Game.0x6FCBA930
uint16_t __fastcall PLAYERLIST_GetFlags(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBA980
void __fastcall sub_6FCBA980(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t a3);
//D2Game.0x6FCBA9D0
int16_t __fastcall sub_6FCBA9D0(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBAA20
void __fastcall sub_6FCBAA20(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBAAA0
void __fastcall sub_6FCBAAA0(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBAB20
void __fastcall sub_6FCBAB20(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FCBAB50
void __fastcall sub_6FCBAB50(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg);
//D2Game.0x6FCBAD60
void __fastcall PLAYERLIST_sub_6FCBAD60(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FCBAE20
int32_t __fastcall PLAYERLIST_GetHostileDelay(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBAE70
void __fastcall PLAYERLIST_SetHostileDelay(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t a3);
//D2Game.0x6FCBAED0
void __fastcall sub_6FCBAED0();
