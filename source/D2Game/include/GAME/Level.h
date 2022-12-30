#pragma once

#include <Units/Units.h>


//D2Game.0x6FC3BBA0
void __fastcall LEVEL_UpdateUnitsInAdjacentRooms(D2GameStrc* pGame, D2RoomStrc* pRoom, D2ClientStrc* pClient);
//D2Game.0x6FC3BD10
void __fastcall LEVEL_RemoveUnitsExceptClientPlayer(D2RoomStrc* pRoom, D2ClientStrc* pClient);
//D2Game.0x6FC3BDE0
void __fastcall LEVEL_FreeDrlgDeletes(D2GameStrc* pGame);
//D2Game.0x6FC3BE40
void __fastcall LEVEL_AddClient(D2GameStrc* pGame, D2RoomStrc* pRoom, D2ClientStrc* pClient);
//D2Game.0x6FC3BF00
void __fastcall LEVEL_RemoveClient(D2GameStrc* pGame, D2RoomStrc* pRoom, D2ClientStrc* pClient);
//D2Game.0x6FC3BFB0
void __fastcall LEVEL_RemoveClientFromAdjacentRooms(D2RoomStrc* pRoom, D2ClientStrc* pClient);
//D2Game.0x6FC3C010
void __fastcall LEVEL_SynchronizeDayNightCycleWithClient(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC3C0B0
void __fastcall LEVEL_ChangeAct(D2GameStrc* pGame, D2ClientStrc* pClient, int32_t nDestinationLevelId, int32_t nTileCalc);
//D2Game.0x6FC3C410
void __fastcall LEVEL_WarpUnit(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nDestinationLevelId, int32_t nTileCalc);
//D2Game.0x6FC3C510
void __fastcall LEVEL_LoadAct(D2GameStrc* pGame, uint8_t nAct);
//D2Game.0x6FC3C580
void __fastcall LEVEL_RemoveAllUnits(D2GameStrc* pGame);
//D2Game.0x6FC3C5B0
void __fastcall LEVEL_UpdateQueuedUnitsInAllActs(D2GameStrc* pGame);
