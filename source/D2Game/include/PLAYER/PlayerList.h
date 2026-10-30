#pragma once

#include <Units/Units.h>


//D2Game.0x6FCBA690
//1.14d: 0x0055B790
void __fastcall PLAYERLIST_FreePlayerLists(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FCBA6E0
//1.14d: 0x0055B1E0
void __fastcall PLAYERLIST_IterateCallback_FreePlayerList(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg);
//Inlined in some functions
D2PlayerListStrc* __fastcall PLAYERLIST_GetPlayerListRecordFromUnitGUID(D2UnitStrc* pPlayer, int32_t nUnitGUID);
//D2Game.0x6FCBA750
//1.14d: 0x0055B140
void __fastcall PLAYERLIST_AllocPlayerList(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBA840
//1.14d: 0x0055B260
void __fastcall PLAYERLIST_ToggleFlag(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, uint32_t nFlags, int32_t bSet);
//D2Game.0x6FCBA8E0
//1.14d: 0x0055B300
int32_t __fastcall PLAYERLIST_CheckFlag(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, uint32_t nFlags);
//D2Game.0x6FCBA930
//1.14d: 0x0055B350
uint16_t __fastcall PLAYERLIST_GetFlags(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBA980
//1.14d: 0x0055B3A0
void __fastcall PLAYERLIST_SetPartyState(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nPartyState);
//D2Game.0x6FCBA9D0
//1.14d: 0x0055B3F0
int16_t __fastcall PLAYERLIST_GetPartyState(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBAA20
//1.14d: 0x0055B440
void __fastcall PLAYERLIST_SendPartyState(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBAAA0
//1.14d: 0x0055B4C0
void __fastcall PLAYERLIST_BroadcastFlags(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBAB20
//1.14d: 0x0055B620
void __fastcall PLAYERLIST_InitializePlayerRelations(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FCBAB50
//1.14d: 0x0055B540
void __fastcall PLAYERLIST_IterateCallback_InitializePlayerRelation(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg);
//D2Game.0x6FCBAD60
//1.14d: 0x0055B650
void __fastcall PLAYERLIST_ResetPendingPartyInvitations(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FCBAE20
//1.14d: 0x0055B6C0
int32_t __fastcall PLAYERLIST_GetHostileDelay(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2);
//D2Game.0x6FCBAE70
//1.14d: 0x0055B720
void __fastcall PLAYERLIST_SetHostileDelay(D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, int32_t nHostileDelayEndTick);
//D2Game.0x6FCBAED0
//1.14d: 0x006CAF60
void __fastcall PLAYERLIST_InitializeUnitSeed();
