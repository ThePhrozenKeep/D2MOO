#pragma once

#include "D2Structs.h"
#include <Units/Units.h>

struct D2DrlgCoordsStrc;

#pragma pack(1)

struct D2ObjInitFnStrc
{
	D2GameStrc* pGame;						//0x00
	D2UnitStrc* pObject;					//0x04
	D2RoomStrc* pRoom;						//0x08
	D2ObjectControlStrc* pObjectregion;		//0x0C
	D2ObjectsTxt* pObjectTxt;				//0x10
	int32_t nX;								//0x14
	int32_t nY;								//0x18
};
using ObjInitFunction = void(__fastcall*)(D2ObjInitFnStrc*);

#pragma pack()

//D2Game.0x6FC70180
D2UnitStrc* __fastcall OBJECTS_SpawnShrine(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC70270
D2UnitStrc* __fastcall OBJECTS_SpawnPresetChest(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC70470
D2UnitStrc* __fastcall OBJECTS_SpawnSpecialChest(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC70520
D2UnitStrc* __fastcall OBJECTS_SpawnArcaneSymbol(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC70570
D2UnitStrc* __fastcall OBJECTS_SpawnPresetObject(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC70600
void __fastcall OBJECTS_InitHandler(D2GameStrc* pGame, D2UnitStrc* pObject, int32_t nUnitId, D2RoomStrc* pRoom, int32_t nX, int32_t nY);
//D2Game.0x6FC70850
void __fastcall OBJECTS_InitFunction10_Unused(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC708B0
void __fastcall OBJECTS_InitFunction28_GoldPlaceHolder(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70A50
void __fastcall OBJECTS_InitFunction01_Shrine(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70BA0
int32_t __fastcall OBJECTS_GetShrineId(D2GameStrc* pGame, uint8_t nShrineClass, int32_t nLevelId, D2RoomStrc* pRoom);
//D2Game.0x6FC70CA0
void __fastcall OBJECTS_InitFunction22_Fire(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70CF0
void __fastcall OBJECTS_InitFunction16_Well(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70D10
void __fastcall OBJECTS_InitFunction24_SpikeFloorTrap(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70D30
void __fastcall OBJECTS_InitFunction02_Urn(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70DC0
void __fastcall OBJECTS_InitFunction27_GooPile(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70E10
void __fastcall OBJECTS_InitFunction03_Chest(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70F30
void __fastcall OBJECTS_InitFunction57_SparklyChest(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71060
void __fastcall OBJECTS_InitFunction58_Fissure(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC710B0
void __fastcall OBJECTS_InitFunction59_VileDogAfterglow(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71110
void __fastcall OBJECTS_InitFunction12_PermanentPortal(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71360
void __fastcall OBJECTS_InitFunction08_Torch(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71370
void __fastcall OBJECTS_InitFunction14_Brazier(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71380
void __fastcall OBJECTS_InitFunction34_HellBrazierFire(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC713D0
void __fastcall OBJECTS_InitFunction11_Portal(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71470
void __fastcall OBJECTS_InitFunction51_TrappedSoul(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC714A0
void __fastcall OBJECTS_InitFunction46_TrappedSoulPlaceHolder(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC718C0
D2UnitStrc* __fastcall OBJECTS_PopulateFn1_CasketJarSarcophagusUrn(D2GameStrc* pGame, D2RoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC71F60
int32_t __cdecl sub_6FC71F60();
//D2Game.0x6FC71FB0
D2UnitStrc* __fastcall OBJECTS_PopulateFn7_RogueOnStick(D2GameStrc* pGame, D2RoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC72340
void __fastcall OBJECTS_SpawnFliesOnCorpse(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC723F0
D2UnitStrc* __fastcall OBJECTS_PopulateFn3_CommonObjects(D2GameStrc* pGame, D2RoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nChance);
//D2Game.0x6FC72510
D2UnitStrc* __fastcall OBJECTS_CreateObject(D2GameStrc* pGame, int32_t nClassId, int32_t nSizeX, int32_t nSizeY, D2RoomStrc* pRoom);
//D2Game.0x6FC726D0
D2UnitStrc* __fastcall OBJECTS_PopulateFn9_TrappedSoul(D2GameStrc* pGame, D2RoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC727F0
D2UnitStrc* __fastcall OBJECTS_PopulateFn6_RogueGuardCorpse(D2GameStrc* pGame, D2RoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC728C0
D2UnitStrc* __fastcall OBJECTS_PopulateFn8_Well(D2GameStrc* pGame, D2RoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC72C30
D2UnitStrc* __fastcall OBJECTS_PopulateFn2_WaypointShrine(D2GameStrc* pGame, D2RoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC73050
int32_t __fastcall OBJECTS_RollRandomNumber(D2SeedStrc* pSeed, int32_t nMin, int32_t nMax);
//D2Game.0x6FC730C0
D2UnitStrc* __fastcall OBJECTS_PopulateFn4_Barrel(D2GameStrc* pGame, D2RoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC73550
D2UnitStrc* __fastcall OBJECTS_PopulateFn5_Crate(D2GameStrc* pGame, D2RoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC73A70
int32_t __fastcall OBJECTS_SpawnNothing(D2GameStrc* pGame, D2RoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords);
//D2Game.0x6FC73A80
int32_t __fastcall OBJECTS_SpawnBarrel(D2GameStrc* pGame, D2RoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords);
//D2Game.0x6FC73C50
int32_t __fastcall OBJECTS_SpawnNothing2(D2GameStrc* pGame, D2RoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords);
//D2Game.0x6FC73D80
int32_t __fastcall OBJECTS_SpawnArmorStand(D2GameStrc* pGame, D2RoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords);
//D2Game.0x6FC73DA0
void __fastcall OBJECTS_SpawnArmorStandOrWeaponRack(D2GameStrc* pGame, D2RoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords, int32_t bWeaponRack);
//D2Game.0x6FC740C0
int32_t __fastcall OBJECTS_SpawnWeaponRack(D2GameStrc* pGame, D2RoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords);
//D2Game.0x6FC740E0
void __fastcall OBJECTS_PopulationHandler(D2GameStrc* pGame, D2RoomStrc* pRoom);
//D2Game.0x6FC744B0
void __fastcall OBJECTS_FreeHoverMessage(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74520
void __fastcall OBJECTS_RemoveAll(D2GameStrc* pGame);
//D2Game.0x6FC74590
void __fastcall OBJECTS_SetUnitIdInTimerArg(D2UnitStrc* pPortal, int32_t nUnitId);
//D2Game.0x6FC745B0
int32_t __fastcall OBJECTS_GetUnitIdFromTimerArg(D2UnitStrc* pUnit);
