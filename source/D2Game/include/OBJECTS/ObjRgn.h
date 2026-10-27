#pragma once

#include <Units/Units.h>
#include "Objects.h"
#include "ObjMode.h"

//D2Game.0x6FC7A3A0
//1.14d: 0x00546C60
int32_t __fastcall OBJRGN_AllocObjectControl(D2GameStrc* pGame);
//D2Game.0x6FC7A5C0
//1.14d: 0x00546E90
void __fastcall OBJRGN_FreeObjectControl(D2GameStrc* pGame);
//D2Game.0x6FC7A6B0
//1.14d: 0x00546F90
D2ObjectRegionStrc* __fastcall OBJRGN_GetObjectRegionForLevel(D2GameStrc* pGame, int32_t nLevelId);
//D2Game.0x6FC7A6C0
//1.14d: 0x00546FA0
D2ObjectControlStrc* __fastcall OBJRGN_GetObjectControlFromGame(D2GameStrc* pGame);
//D2Game.0x6FC7A6D0
//1.14d: 0x00546FB0
D2ShrineDataStrc* __fastcall OBJRGN_GetShrineDataFromGame(D2GameStrc* pGame);
//D2Game.0x6FC7A6E0
//1.14d: 0x00547170
void __fastcall OBJRGN_AllocObjectRoomCoords(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC7A780
//1.14d: 0x00547210
void __fastcall OBJECTS_InitFunction17_Waypoint(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC7A860
//1.14d: 0x00547300
bool __fastcall OBJRGN_CanNotSpawnMoreWells(D2GameStrc* pGame, int32_t nLevelId);
//D2Game.0x6FC7A890
//1.14d: 0x00547330
bool __fastcall OBJRGN_ShouldSpawnHealingShrineOrWell(D2GameStrc* pGame, int32_t nLevelId);
//D2Game.0x6FC7A8C0
//1.14d: 0x00547360
bool __fastcall OBJRGN_CanNotSpawnMoreShrines(D2GameStrc* pGame, int32_t nLevelId);
//D2Game.0x6FC7A8F0
//1.14d: 0x00547390
void __fastcall OBJRGN_IncreaseHealingShrineCount(D2GameStrc* pGame, int32_t nLevelId);
//D2Game.0x6FC7A900
//1.14d: 0x005473A0
bool __fastcall OBJRGN_CanSpawnWell(D2GameStrc* pGame, int32_t nLevelId, int32_t nX, int32_t nY);
//D2Game.0x6FC7A960
//1.14d: 0x00547400
void __fastcall OBJRGN_SetWellCoordinates(D2GameStrc* pGame, int32_t nLevelId, int32_t nX, int32_t nY);
//D2Game.0x6FC7A990
//1.14d: 0x00547430
bool __fastcall OBJRGN_CanSpawnShrine(D2GameStrc* pGame, int32_t nLevelId, int32_t nX, int32_t nY);
//D2Game.0x6FC7A9F0
//1.14d: 0x00547490
void __fastcall OBJRGN_SetShrineCoordinates(D2GameStrc* pGame, int32_t nLevelId, int32_t nX, int32_t nY);
//D2Game.0x6FC7AA20
//1.14d: 0x005474C0
int32_t __fastcall OBJRGN_GetTrapMonsterId(D2ObjOperateFnStrc* pOp);
