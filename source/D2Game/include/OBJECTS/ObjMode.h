#pragma once

#include "D2Structs.h"
#include <Units/Units.h>

#pragma pack(1)

using ObeliskPowerUpFunction = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*, int32_t);

struct D2ObeliskPowerUpStrc
{
	ObeliskPowerUpFunction pPowerUpCallback;//0x00
	uint32_t nChance;						//0x04
	int32_t nValue;							//0x08
};

using ObjOperateFunction = int32_t(__fastcall*)(D2ObjOperateFnStrc*, int32_t);

struct D2ObjOperateFnStrc
{
	D2GameStrc* pGame;						//0x00
	D2UnitStrc* pObject;					//0x04
	D2UnitStrc* pPlayer;					//0x08
	D2ObjectControlStrc* pObjectregion;		//0x0C
	int32_t nObjectIdx;						//0x10
};

typedef void(__fastcall* SHRINECALLBACK)(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);

struct D2ShrineTableStrc
{
	SHRINECALLBACK pfShrineCallback;		//0x00
	int32_t unk0x04;							//0x04
	int32_t unk0x08;							//0x08
};

#pragma pack()

//D2Game.0x6FC748A0
void __fastcall sub_6FC748A0(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74A40
void __fastcall sub_6FC74A40(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74AC0
void __fastcall sub_6FC74AC0(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74B00
void __fastcall sub_6FC74B00(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74B40
void __fastcall sub_6FC74B40(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74CA0
void __fastcall sub_6FC74CA0(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74D10
void __fastcall sub_6FC74D10(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74DF0
void __fastcall sub_6FC74DF0(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74F60
void __fastcall D2GAME_SpikeTraps_6FC74F60(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC750D0
void __fastcall sub_6FC750D0(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC75250
void __fastcall D2GAME_OBJMODE_InvokeEventFunction_6FC75250(D2GameStrc* pGame, D2UnitStrc* pObject, int32_t nEventType);
//D2Game.0x6FC752A0
void __fastcall sub_6FC752A0(D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FC75350
void __fastcall sub_6FC75350(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FC753E0
int32_t __fastcall OBJECTS_OperateFunction27_TeleportPad(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC75530
int32_t __fastcall OBJECTS_OperateFunction30_ExplodingChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC755A0
int32_t __fastcall OBJECTS_OperateFunction08_Door(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC75730
int32_t __fastcall OBJECTS_OperateFunction16_TrapDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC757E0
int32_t __fastcall OBJECTS_OperateFunction47_Stair(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC758F0
int32_t __fastcall OBJECTS_OperateFunction61_HarrogathMainGate(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC759F0
int32_t __fastcall OBJECTS_OperateFunction50_Stair(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC75A10
int32_t __fastcall OBJECTS_OperateFunction29_SlimeDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC75AC0
void __fastcall D2GAME_OBJECTS_TrapHandler8_9_6FC75AC0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75B40
void __fastcall D2GAME_SpawnTrapMonster_6FC75B40(D2ObjOperateFnStrc* pOp, int32_t nMonsterId, int32_t a3);
//D2Game.0x6FC75BC0
void __fastcall D2GAME_OBJECTS_TrapHandler5_7_6FC75BC0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75C70
void __fastcall D2GAME_OBJECTS_TrapHandler1_6FC75C70(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75D00
void __fastcall D2GAME_OBJECTS_TrapHandler2_6_6FC75D00(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75D90
void __fastcall D2GAME_OBJECTS_TrapHandler3_6FC75D90(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75E20
void __fastcall D2GAME_OBJECTS_TrapHandler4_6FC75E20(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75EB0
void __fastcall sub_6FC75EB0(D2ObjOperateFnStrc* pOp);
//Inlined in OBJECTS_OperateFunction04_Chest
__forceinline void __fastcall OBJECTS_ChestEnd(D2ObjOperateFnStrc* pOp, int32_t nType);
//D2Game.0x6FC76030
int32_t __fastcall OBJECTS_OperateFunction04_Chest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC764B0
void __fastcall  D2GAME_SetTrapCallback_6FC764B0(D2ObjOperateFnStrc* pOp, uint8_t nTrapType);
//D2Game.0x6FC76570
int32_t __fastcall OBJECTS_OperateFunction17_Obelisk(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC766B0
void __fastcall D2GAME_SHRINES_Health_6FC766B0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC766F0
void __fastcall D2GAME_SHRINES_FillMana_6FC766F0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76730
void __fastcall D2GAME_SHRINES_Refill_6FC76730(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76790
void __fastcall D2GAME_SHRINES_ExchangeHealth_6FC76790(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC767F0
void __fastcall D2GAME_SHRINES_ExchangeMana_6FC767F0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76850
void __fastcall D2GAME_SHRINES_Enirhs_6FC76850(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76880
void __fastcall D2GAME_SHRINES_Portal_6FC76880(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76910
void __fastcall D2GAME_SHRINES_Gem_6FC76910(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76A60
int32_t __fastcall sub_6FC76A60(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nItemId, D2UnitStrc* pItem);
//D2Game.0x6FC76BC0
void __fastcall D2GAME_SHRINES_Storm_6FC76BC0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76E90
int32_t __fastcall sub_6FC76E90(D2UnitStrc* pUnit, D2UnitFindArgStrc* pArg);
//D2Game.0x6FC76ED0
void __fastcall D2GAME_SHRINES_Monster_6FC76ED0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76F60
int32_t __fastcall sub_6FC76F60(D2UnitStrc* pMonster, D2UnitStrc* pUnit1);
//D2Game.0x6FC770D0
void __fastcall D2GAME_SHRINES_Exploding_6FC770D0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC773B0
void __fastcall D2GAME_SHRINES_Poison_6FC773B0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC77690
void __fastcall D2GAME_SHRINES_CombatBoost_6FC77690(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC77750
int32_t __fastcall sub_6FC77750(D2ShrinesTxt* pShrinesTxtRecord, int32_t nStatId, int32_t nValue, D2UnitStrc* pUnit);
//D2Game.0x6FC779C0
void __fastcall D2GAME_SHRINES_Stamina_6FC779C0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC77AB0
void __fastcall sub_6FC77AB0(D2UnitStrc* pUnit, int32_t a2, int32_t a3);
//D2Game.0x6FC77AE0
void __fastcall D2GAME_SHRINES_DefensiveBoost_6FC77AE0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC77BA0
void __fastcall D2GAME_SHRINES_SkillBoost_6FC77BA0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC77C10
void __fastcall sub_6FC77C10(D2UnitStrc* pUnit, int32_t a2, int32_t a3);
//D2Game.0x6FC77C30
int32_t __fastcall OBJECTS_OperateFunction02_Shrine(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC77E80
int32_t __fastcall OBJECTS_OperateFunction01_Casket(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC77F70
int32_t __fastcall OBJECTS_OperateFunction68_EvilUrn(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78120
int32_t __fastcall OBJECTS_OperateFunction33_WirtsBody(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC781C0
int32_t __fastcall OBJECTS_OperateFunction03_Urn_Basket_Jar(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78290
int32_t __fastcall OBJECTS_OperateFunction14_Corpse(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78340
void __fastcall sub_6FC78340(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC78390
int32_t __fastcall OBJECTS_OperateFunction51_JungleStash(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78470
int32_t __fastcall OBJECTS_OperateFunction18_SecretDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC784E0
int32_t __fastcall OBJECTS_OperateFunction26_BookShelf(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC785D0
int32_t __fastcall OBJECTS_OperateFunction19_ArmorStand(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78640
int32_t __fastcall OBJECTS_OperateFunction20_WeaponRack(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC786B0
int32_t __fastcall OBJECTS_OperateFunction05_Barrel(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC787F0
int32_t __fastcall OBJECTS_OperateFunction07_ExplodingBarrel(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78940
int32_t __fastcall OBJECTS_OperateFunction13_TorchTiki(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78970
int32_t __fastcall OBJECTS_OperateFunction11_Torch(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC789C0
int32_t __fastcall OBJECTS_OperateHandler(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nObjectType, int32_t nObjectGUID, int32_t* pResult);
//D2Game.0x6FC78B20
int32_t __fastcall sub_6FC78B20(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nObjectType, int32_t nObjectGUID);
//D2Game.0x6FC78BB0
void __fastcall sub_6FC78BB0(D2GameStrc* pGame, int32_t nUnused, int32_t nObjectGUID);
//D2Game.0x6FC78C90
int32_t __fastcall OBJECTS_OperateFunction34_ArcaneSanctuaryPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78D30
int32_t __fastcall OBJECTS_OperateFunction46_HellGatePortal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78E00
int32_t __fastcall OBJECTS_OperateFunction15_Portal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC79310
int32_t __fastcall OBJECTS_OperateFunction43_DurielPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC79490
int32_t __fastcall OBJECTS_OperateFunction23_Waypoint(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC79600
void __fastcall D2GAME_WAYPOINT_Unk_6FC79600(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nWaypointGUID, int32_t nLevelId);
//D2Game.0x6FC797A0
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseMana(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC797E0
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseEnergy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC79800
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseDexterity(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC79820
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseVitality(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC79840
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseStrength(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC79860
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseHitpoints(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC798A0
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseSkillpoints(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC798C0
int32_t __fastcall OBJMODE_MainObeliskHandler(D2GameStrc* pGame, int32_t nPlayerGUID, int32_t nObjectGUID, int32_t nItemGUID, int16_t nState);
//D2Game.0x6FC79B50
void __fastcall sub_6FC79B50(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pObject);
//D2Game.0x6FC79C00
void __fastcall sub_6FC79C00(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC79C30
void __fastcall sub_6FC79C30(D2GameStrc* pGame, int32_t nUnused, int32_t nPortalGUID, int32_t nDestLevel);
//D2Game.0x6FC79D90
int32_t __fastcall OBJECTS_OperateFunction22_Well(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC7A000
void __fastcall OBJMODE_PetIterate_Heal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pUnit2, void* pUnitHealed);
//D2Game.0x6FC7A070
int32_t __fastcall OBJECTS_OperateFunction48_TrappedSoul(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC7A140
D2UnitStrc* __fastcall OBJMODE_DropItemWithCodeAndQuality(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t dwCode, int32_t nItemQuality);
//D2Game.0x6FC7A220
D2UnitStrc* __fastcall OBJMODE_DropFromChestTCWithQuality(D2ObjOperateFnStrc* pOp, int32_t nItemQuality);
