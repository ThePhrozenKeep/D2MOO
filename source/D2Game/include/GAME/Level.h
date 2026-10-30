#pragma once

#include <Units/Units.h>


//D2Game.0x6FC3BBA0
//1.14d: 0x0053A620
void __fastcall LEVEL_UpdateUnitsInAdjacentRooms(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2ClientStrc* pClient);
//D2Game.0x6FC3BD10
//1.14d: 0x0053A770
void __fastcall LEVEL_RemoveUnitsExceptClientPlayer(D2ActiveRoomStrc* pRoom, D2ClientStrc* pClient);
//D2Game.0x6FC3BDE0
//1.14d: 0x0053A820
void __fastcall LEVEL_FreeDrlgDeletes(D2GameStrc* pGame);
//D2Game.0x6FC3BE40
//1.14d: 0x0053A8E0
void __fastcall LEVEL_AddClient(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2ClientStrc* pClient);
//D2Game.0x6FC3BF00
//1.14d: 0x0053A9B0
void __fastcall LEVEL_RemoveClient(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2ClientStrc* pClient);
//D2Game.0x6FC3BFB0
//1.14d: 0x0053AB80
void __fastcall LEVEL_RemoveClientFromAdjacentRooms(D2ActiveRoomStrc* pRoom, D2ClientStrc* pClient);
//D2Game.0x6FC3C010
//1.14d: 0x0053ABE0
void __fastcall LEVEL_SynchronizeDayNightCycleWithClient(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC3C0B0
//1.14d: 0x0053ACC0
void __fastcall LEVEL_ChangeAct(D2GameStrc* pGame, D2ClientStrc* pClient, int32_t nDestinationLevelId, int32_t nTileCalc);
//D2Game.0x6FC3C410
//1.14d: 0x0053AEC0
void __fastcall LEVEL_WarpUnit(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nDestinationLevelId, int32_t nTileCalc);
//D2Game.0x6FC3C510
//1.14d: 0x0053AFB0
void __fastcall LEVEL_LoadAct(D2GameStrc* pGame, uint8_t nAct);
//D2Game.0x6FC3C580
//1.14d: 0x0053AFD0
void __fastcall LEVEL_RemoveAllUnits(D2GameStrc* pGame);
//D2Game.0x6FC3C5B0
//1.14d: 0x0053B000
void __fastcall LEVEL_UpdateQueuedUnitsInAllActs(D2GameStrc* pGame);
