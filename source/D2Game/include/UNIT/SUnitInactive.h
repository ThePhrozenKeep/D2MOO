#pragma once

#include "D2Structs.h"
#include <Units/Units.h>


//D2Game.0x6FCC3850
void __fastcall SUNITINACTIVE_RestoreInactiveUnits(D2GameStrc* pGame, D2RoomStrc* pRoom);
//D2Game.0x6FCC40D0
D2AiControlStrc* __fastcall AIGENERAL_GetAiControlFromUnit(D2UnitStrc* pUnit);
//D2Game.0x6FCC40F0
void __fastcall SUNITINACTIVE_FreeInactiveMonsterNode(D2GameStrc* pGame, D2InactiveMonsterNodeStrc* pInactiveMonsterNode);
//D2Game.0x6FCC4120
D2UnitStrc* __fastcall SUNITINACTIVE_RestoreInactiveItem(D2GameStrc* pGame, D2RoomStrc* pRoom, D2InactiveItemNodeStrc* pInactiveItemNode);
//D2Game.0x6FCC4270
void __fastcall SUNITINACTIVE_FreeInactiveUnitLists(D2GameStrc* pGame);
//D2Game.0x6FCC4370
void __fastcall SUNITINACTIVE_CompressUnitIfNeeded(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCC4650
void __fastcall SUNITINACTIVE_CompressInactiveUnit(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCC4C90
void __fastcall SUNITINACTIVE_DeleteSingleListNode(D2GameStrc* pGame, uint16_t nUnitType, uint16_t nClassId, uint8_t nAct);
//D2Game.0x6FCC4DC0
D2InactiveUnitListStrc* __fastcall SUNITINACTIVE_GetListNodeFromActAndCoordinates(D2GameStrc* pGame, int32_t nAct, int32_t nX, int32_t nY, int32_t bAllocNewNode);
//D2Game.0x6FCC4E80
D2InactiveUnitListStrc* __fastcall SUNITINACTIVE_GetListNodeFromRoom(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t bAllocNewNode);
//D2Game.0x6FCC4ED0
void __fastcall SUNITINACTIVE_RestoreSpecialMonsterParameters(D2GameStrc* pGame, D2UnitStrc* pUnit, D2InactiveMonsterNodeStrc* pInactiveMonsterNode);
//D2Game.0x6FCC50B0
void __fastcall SUNITINACTIVE_SaveSpecialMonsterParameters(D2GameStrc* pGame, D2UnitStrc* pUnit, D2InactiveMonsterNodeStrc* pInactiveMonsterNode);
//D2Game.0x6FCC52C0
int32_t __fastcall SUNITINACTIVE_IsUnitInsideRoom(D2GameStrc* pGame, D2RoomStrc* pRoomNear, int32_t nGameX, int32_t nGameY, int32_t nClassId);
//D2Game.0x6FCC5490
void __fastcall SUNITINACTIVE_DeleteExpiredItemNodes(D2GameStrc* pGame, int32_t nAct);
//D2Game.0x6FCC54F0
void __fastcall SUNITINACTIVE_SetUnitFlagEx(D2UnitStrc* pUnit, uint32_t nFlag, int32_t bSet);
