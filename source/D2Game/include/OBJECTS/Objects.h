#pragma once

#include <Units/Units.h>

struct D2DrlgCoordsStrc;

#pragma pack(1)

struct D2ObjInitFnStrc
{
	D2GameStrc* pGame;						//0x00
	D2UnitStrc* pObject;					//0x04
	D2ActiveRoomStrc* pRoom;						//0x08
	D2ObjectControlStrc* pObjectregion;		//0x0C
	D2ObjectsTxt* pObjectTxt;				//0x10
	int32_t nX;								//0x14
	int32_t nY;								//0x18
};
using ObjInitFunction = void(__fastcall*)(D2ObjInitFnStrc*);

#pragma pack()

//D2Game.0x6FC70180
//1.14d: 0x0054F0D0
D2UnitStrc* __fastcall OBJECTS_SpawnShrine(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC70270
//1.14d: 0x0054F180
D2UnitStrc* __fastcall OBJECTS_SpawnPresetChest(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC70470
//1.14d: 0x0054F370
D2UnitStrc* __fastcall OBJECTS_SpawnSpecialChest(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC70520
//1.14d: 0x0054F430
D2UnitStrc* __fastcall OBJECTS_SpawnArcaneSymbol(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC70570
//1.14d: 0x0054F490
D2UnitStrc* __fastcall OBJECTS_SpawnPresetObject(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode);
//D2Game.0x6FC70600
//1.14d: 0x0054F5D0
void __fastcall OBJECTS_InitHandler(D2GameStrc* pGame, D2UnitStrc* pObject, int32_t nUnitId, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY);
//D2Game.0x6FC70850
//1.14d: 0x0054F860
void __fastcall OBJECTS_InitFunction10_Unused(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC708B0
void __fastcall OBJECTS_InitFunction28_GoldPlaceHolder(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70A50
//1.14d: 0x0054F9D0
void __fastcall OBJECTS_InitFunction01_Shrine(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70BA0
//1.14d: 0x0054F770
int32_t __fastcall OBJECTS_GetShrineId(D2GameStrc* pGame, uint8_t nShrineClass, int32_t nLevelId, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC70CA0
//1.14d: 0x0054FB40
void __fastcall OBJECTS_InitFunction22_Fire(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70CF0
//1.14d: 0x00552B30
void __fastcall OBJECTS_InitFunction16_Well(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70D10
//1.14d: 0x0054FB90
void __fastcall OBJECTS_InitFunction24_SpikeFloorTrap(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70D30
//1.14d: 0x0054FBB0
void __fastcall OBJECTS_InitFunction02_Urn(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70DC0
//1.14d: 0x0054FC50
void __fastcall OBJECTS_InitFunction27_GooPile(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70E10
//1.14d: 0x0054FCB0
void __fastcall OBJECTS_InitFunction03_Chest(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC70F30
//1.14d: 0x0054FD90
void __fastcall OBJECTS_InitFunction57_SparklyChest(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71060
//1.14d: 0x0054FDB0
void __fastcall OBJECTS_InitFunction58_Fissure(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC710B0
//1.14d: 0x0054FE10
void __fastcall OBJECTS_InitFunction59_VileDogAfterglow(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71110
//1.14d: 0x0054FE70
void __fastcall OBJECTS_InitFunction12_PermanentPortal(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71360
//1.14d: 0x005500C0
void __fastcall OBJECTS_InitFunction08_Torch(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71370
//1.14d: 0x005500D0
void __fastcall OBJECTS_InitFunction14_Brazier(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71380
//1.14d: 0x005500F0
void __fastcall OBJECTS_InitFunction34_HellBrazierFire(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC713D0
//1.14d: 0x00550140
void __fastcall OBJECTS_InitFunction11_Portal(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC71470
//1.14d: 0x005501F0
void __fastcall OBJECTS_InitFunction51_TrappedSoul(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC714A0
//1.14d: 0x005506D0
void __fastcall OBJECTS_InitFunction46_TrappedSoulPlaceHolder(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC718C0
//1.14d: 0x00550C20
D2UnitStrc* __fastcall OBJECTS_PopulateFn1_CasketJarSarcophagusUrn(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC71F60
int32_t __cdecl sub_6FC71F60();
//D2Game.0x6FC71FB0
//1.14d: 0x00551200
D2UnitStrc* __fastcall OBJECTS_PopulateFn7_RogueOnStick(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC72340
//1.14d: 0x00551150
void __fastcall OBJECTS_SpawnFliesOnCorpse(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC723F0
//1.14d: 0x00551470
D2UnitStrc* __fastcall OBJECTS_PopulateFn3_CommonObjects(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nChance);
//D2Game.0x6FC72510
//1.14d: 0x00550380
D2UnitStrc* __fastcall OBJECTS_CreateObject(D2GameStrc* pGame, int32_t nClassId, int32_t nSizeX, int32_t nSizeY, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC726D0
//1.14d: 0x00551580
D2UnitStrc* __fastcall OBJECTS_PopulateFn9_TrappedSoul(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC727F0
//1.14d: 0x00551690
D2UnitStrc* __fastcall OBJECTS_PopulateFn6_RogueGuardCorpse(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC728C0
//1.14d: 0x005516C0
D2UnitStrc* __fastcall OBJECTS_PopulateFn8_Well(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC72C30
//1.14d: 0x00552B50
D2UnitStrc* __fastcall OBJECTS_PopulateFn2_WaypointShrine(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC73050
//1.14d: 0x004BC500
int32_t __fastcall OBJECTS_RollRandomNumber(D2SeedStrc* pSeed, int32_t nMin, int32_t nMax);
//D2Game.0x6FC730C0
//1.14d: 0x00551850
D2UnitStrc* __fastcall OBJECTS_PopulateFn4_Barrel(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC73550
//1.14d: 0x00551C00
D2UnitStrc* __fastcall OBJECTS_PopulateFn5_Crate(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, uint8_t nDensity, uint32_t nObjectId, uint16_t nProbability);
//D2Game.0x6FC73A70
//1.14d: 0x00551FF0
int32_t __fastcall OBJECTS_SpawnNothing(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords);
//D2Game.0x6FC73A80
//1.14d: 0x00552000
int32_t __fastcall OBJECTS_SpawnBarrel(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords);
//D2Game.0x6FC73C50
//1.14d: 0x00552140
int32_t __fastcall OBJECTS_SpawnRandomItemGrid(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords);
//D2Game.0x6FC73D80
//1.14d: 0x005523C0
int32_t __fastcall OBJECTS_SpawnArmorStand(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords);
//D2Game.0x6FC73DA0
//1.14d: 0x00552200
void __fastcall OBJECTS_SpawnArmorStandOrWeaponRack(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords, int32_t bWeaponRack);
//D2Game.0x6FC740C0
//1.14d: 0x005523E0
int32_t __fastcall OBJECTS_SpawnWeaponRack(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2DrlgCoordsStrc* pDrlgCoords);
//D2Game.0x6FC740E0
//1.14d: 0x00552610
void __fastcall OBJECTS_PopulationHandler(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC744B0
//1.14d: 0x00552A00
void __fastcall OBJECTS_FreeHoverMessage(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74520
//1.14d: 0x00552A40
void __fastcall OBJECTS_RemoveAll(D2GameStrc* pGame);
//D2Game.0x6FC74590
//1.14d: 0x00552AF0
void __fastcall OBJECTS_SetUnitIdInTimerArg(D2UnitStrc* pPortal, int32_t nUnitId);
//D2Game.0x6FC745B0
//1.14d: 0x00552B10
int32_t __fastcall OBJECTS_GetUnitIdFromTimerArg(D2UnitStrc* pUnit);
