#pragma once

#include <Units/Units.h>
#include <Units/UnitFinds.h>

#pragma pack(1)

using ObeliskPowerUpFunction = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*, int32_t);

struct D2ObeliskPowerUpStrc
{
	ObeliskPowerUpFunction pPowerUpCallback;//0x00
	uint32_t nChance;						//0x04
	int32_t nValue;							//0x08
};


struct D2ObjOperateFnStrc
{
	D2GameStrc* pGame;						//0x00
	D2UnitStrc* pObject;					//0x04
	D2UnitStrc* pPlayer;					//0x08
	D2ObjectControlStrc* pObjectregion;		//0x0C
	int32_t nObjectIdx;						//0x10
};
using ObjOperateFunction = int32_t(__fastcall*)(D2ObjOperateFnStrc*, int32_t);

using ObjShrineFunction = void(__fastcall* )(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
struct D2ShrineTableStrc
{
	ObjShrineFunction pfShrineCallback;		//0x00
	int32_t unk0x04;						//0x04
	int32_t unk0x08;						//0x08
};

#pragma pack()

//D2Game.0x6FC748A0
//1.14d: 0x00581250
void __fastcall OBJMODE_UpdatePeriodicAnimation(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74A40
//1.14d: 0x00581410
void __fastcall OBJMODE_CreateOutdoorHellPortal(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74AC0
//1.14d: 0x00581490
void __fastcall OBJMODE_CompleteObjectAnimation(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74B00
//1.14d: 0x005814D0
void __fastcall OBJMODE_ResetShrine(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74B40
//1.14d: 0x00581510
void __fastcall OBJMODE_RegenerateWell(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74CA0
//1.14d: 0x00581620
void __fastcall OBJMODE_ExpireHoverText(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74D10
//1.14d: 0x00581700
void __fastcall OBJMODE_UpdateTrapProximity(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74DF0
//1.14d: 0x005817A0
void __fastcall OBJMODE_DispatchTrap(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC74F60
//1.14d: 0x005818B0
void __fastcall D2GAME_SpikeTraps_6FC74F60(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC750D0
//1.14d: 0x00585CE0
void __fastcall OBJMODE_UpdateTrappedSoul(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC75250
//1.14d: 0x00586AD0
void __fastcall D2GAME_OBJMODE_InvokeEventFunction_6FC75250(D2GameStrc* pGame, D2UnitStrc* pObject, int32_t nEventType);
//D2Game.0x6FC752A0
//1.14d: 0x00581A20
void __fastcall OBJMODE_SendObjectModeAndShrineUpdate(D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FC75350
//1.14d: 0x00581AD0
void __fastcall OBJMODE_SendObjectUpdates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FC753E0
//1.14d: 0x00581BF0
int32_t __fastcall OBJECTS_OperateFunction27_TeleportPad(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC75530
//1.14d: 0x00581CD0
int32_t __fastcall OBJECTS_OperateFunction30_ExplodingChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC755A0
//1.14d: 0x00581D40
int32_t __fastcall OBJECTS_OperateFunction08_Door(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC75730
//1.14d: 0x00581EB0
int32_t __fastcall OBJECTS_OperateFunction16_TrapDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC757E0
//1.14d: 0x00581F60
int32_t __fastcall OBJECTS_OperateFunction47_Stair(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC758F0
//1.14d: 0x00582080
int32_t __fastcall OBJECTS_OperateFunction61_HarrogathMainGate(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC759F0
//1.14d: 0x00582180
int32_t __fastcall OBJECTS_OperateFunction50_Stair(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC75A10
//1.14d: 0x005821A0
int32_t __fastcall OBJECTS_OperateFunction29_SlimeDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC75AC0
//1.14d: 0x005822F0
void __fastcall D2GAME_OBJECTS_TrapHandler8_9_6FC75AC0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75B40
//1.14d: 0x00582280
void __fastcall D2GAME_SpawnTrapMonster_6FC75B40(D2ObjOperateFnStrc* pOp, int32_t nMonsterId, int32_t nAnimMode);
//D2Game.0x6FC75BC0
//1.14d: 0x00582380
void __fastcall D2GAME_OBJECTS_TrapHandler5_7_6FC75BC0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75C70
//1.14d: 0x00582490
void __fastcall D2GAME_OBJECTS_TrapHandler1_6FC75C70(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75D00
//1.14d: 0x005824B0
void __fastcall D2GAME_OBJECTS_TrapHandler2_6_6FC75D00(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75D90
//1.14d: 0x005824D0
void __fastcall D2GAME_OBJECTS_TrapHandler3_6FC75D90(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75E20
//1.14d: 0x005824F0
void __fastcall D2GAME_OBJECTS_TrapHandler4_6FC75E20(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC75EB0
//1.14d: 0x00585E00
void __fastcall OBJMODE_OpenTowerChest(D2ObjOperateFnStrc* pOp);
//Inlined in OBJECTS_OperateFunction04_Chest
__forceinline void __fastcall OBJECTS_ChestEnd(D2ObjOperateFnStrc* pOp, int32_t nType);
//D2Game.0x6FC76030
//1.14d: 0x00585F60
int32_t __fastcall OBJECTS_OperateFunction04_Chest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC764B0
//1.14d: 0x00582510
void __fastcall  D2GAME_SetTrapCallback_6FC764B0(D2ObjOperateFnStrc* pOp, uint8_t nTrapType);
//D2Game.0x6FC76570
//1.14d: 0x00582610
int32_t __fastcall OBJECTS_OperateFunction17_Obelisk(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC766B0
//1.14d: 0x00582860
void __fastcall D2GAME_SHRINES_Health_6FC766B0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC766F0
//1.14d: 0x005828A0
void __fastcall D2GAME_SHRINES_FillMana_6FC766F0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76730
//1.14d: 0x005828E0
void __fastcall D2GAME_SHRINES_Refill_6FC76730(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76790
//1.14d: 0x00582940
void __fastcall D2GAME_SHRINES_ExchangeHealth_6FC76790(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC767F0
//1.14d: 0x005829A0
void __fastcall D2GAME_SHRINES_ExchangeMana_6FC767F0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76850
//1.14d: 0x00582A00
void __fastcall D2GAME_SHRINES_Enirhs_6FC76850(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76880
//1.14d: 0x00582A30
void __fastcall D2GAME_SHRINES_Portal_6FC76880(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76910
//1.14d: 0x00582C40
void __fastcall D2GAME_SHRINES_Gem_6FC76910(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76A60
//1.14d: 0x00582AC0
int32_t __fastcall OBJMODE_DropShrineGem(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nItemId, D2UnitStrc* pItem);
//D2Game.0x6FC76BC0
//1.14d: 0x00582DA0
void __fastcall D2GAME_SHRINES_Storm_6FC76BC0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76E90
//1.14d: 0x00582710
int32_t __fastcall OBJMODE_IsLivingPlayerOrMonster(D2UnitStrc* pUnit, D2UnitFindArgStrc* pArg);
//D2Game.0x6FC76ED0
//1.14d: 0x00583050
void __fastcall D2GAME_SHRINES_Monster_6FC76ED0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC76F60
//1.14d: 0x00582750
int32_t __fastcall OBJMODE_IsMonsterShrineCandidate(D2UnitStrc* pMonster, D2UnitStrc* pUnit1);
//D2Game.0x6FC770D0
//1.14d: 0x005830E0
void __fastcall D2GAME_SHRINES_Exploding_6FC770D0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC773B0
//1.14d: 0x00583410
void __fastcall D2GAME_SHRINES_Poison_6FC773B0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC77690
//1.14d: 0x005839B0
void __fastcall D2GAME_SHRINES_CombatBoost_6FC77690(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC77750
//1.14d: 0x00583840
int32_t __fastcall OBJMODE_CalculateShrineStatBonus(D2ShrinesTxt* pShrinesTxtRecord, int32_t nStatId, int32_t nValue, D2UnitStrc* pUnit);
//D2Game.0x6FC779C0
//1.14d: 0x00583A70
void __fastcall D2GAME_SHRINES_Stamina_6FC779C0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC77AB0
//1.14d: 0x00583A40
void __fastcall OBJMODE_RemoveStaminaShrine(D2UnitStrc* pUnit, int32_t nState, struct D2StatListStrc* pStatList);
//D2Game.0x6FC77AE0
//1.14d: 0x00583B30
void __fastcall D2GAME_SHRINES_DefensiveBoost_6FC77AE0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC77BA0
//1.14d: 0x00583BF0
void __fastcall D2GAME_SHRINES_SkillBoost_6FC77BA0(D2ObjOperateFnStrc* pOp, D2ShrinesTxt* pShrinesTxtRecord);
//D2Game.0x6FC77C10
//1.14d: 0x00583BD0
void __fastcall OBJMODE_RemoveSkillShrine(D2UnitStrc* pUnit, int32_t nState, struct D2StatListStrc* pStatList);
//D2Game.0x6FC77C30
//1.14d: 0x00583C70
int32_t __fastcall OBJECTS_OperateFunction02_Shrine(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC77E80
//1.14d: 0x00586410
int32_t __fastcall OBJECTS_OperateFunction01_Casket(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC77F70
//1.14d: 0x00586520
int32_t __fastcall OBJECTS_OperateFunction68_EvilUrn(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78120
//1.14d: 0x00583E70
int32_t __fastcall OBJECTS_OperateFunction33_WirtsBody(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC781C0
//1.14d: 0x005866C0
int32_t __fastcall OBJECTS_OperateFunction03_Urn_Basket_Jar(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78290
//1.14d: 0x005867A0
int32_t __fastcall OBJECTS_OperateFunction14_Corpse(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78340
//1.14d: 0x00586850
void __fastcall OBJMODE_DropJungleStashItems(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC78390
//1.14d: 0x00583F10
int32_t __fastcall OBJECTS_OperateFunction51_JungleStash(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78470
//1.14d: 0x00583FF0
int32_t __fastcall OBJECTS_OperateFunction18_SecretDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC784E0
//1.14d: 0x00584060
int32_t __fastcall OBJECTS_OperateFunction26_BookShelf(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC785D0
//1.14d: 0x00584160
int32_t __fastcall OBJECTS_OperateFunction19_ArmorStand(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78640
//1.14d: 0x005841D0
int32_t __fastcall OBJECTS_OperateFunction20_WeaponRack(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC786B0
//1.14d: 0x005868A0
int32_t __fastcall OBJECTS_OperateFunction05_Barrel(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC787F0
//1.14d: 0x00584330
int32_t __fastcall OBJECTS_OperateFunction07_ExplodingBarrel(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78940
//1.14d: 0x005843A0
int32_t __fastcall OBJECTS_OperateFunction13_TorchTiki(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78970
//1.14d: 0x005843D0
int32_t __fastcall OBJECTS_OperateFunction11_Torch(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC789C0
//1.14d: 0x00584420
int32_t __fastcall OBJECTS_OperateHandler(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nObjectType, int32_t nObjectGUID, int32_t* pResult);
//D2Game.0x6FC78B20
//1.14d: 0x00584540
int32_t __fastcall OBJMODE_OperateObjectInRange(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nObjectType, int32_t nObjectGUID);
//D2Game.0x6FC78BB0
//1.14d: 0x005845D0
void __fastcall OBJMODE_UpdateDoorModeFromCollision(D2GameStrc* pGame, int32_t nUnused, int32_t nObjectGUID);
//D2Game.0x6FC78C90
//1.14d: 0x005846B0
int32_t __fastcall OBJECTS_OperateFunction34_ArcaneSanctuaryPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78D30
//1.14d: 0x00584750
int32_t __fastcall OBJECTS_OperateFunction46_HellGatePortal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC78E00
//1.14d: 0x00584870
int32_t __fastcall OBJECTS_OperateFunction15_Portal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC79310
//1.14d: 0x00584D00
int32_t __fastcall OBJECTS_OperateFunction43_DurielPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC79490
//1.14d: 0x00584E30
int32_t __fastcall OBJECTS_OperateFunction23_Waypoint(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC79600
//1.14d: 0x00584F60
void __fastcall OBJMODE_TravelViaWaypoint(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nWaypointGUID, int32_t nLevelId);
//D2Game.0x6FC797A0
//1.14d: 0x00585120
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseMana(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC797E0
//1.14d: 0x00585160
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseEnergy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC79800
//1.14d: 0x00585180
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseDexterity(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC79820
//1.14d: 0x005851A0
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseVitality(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC79840
//1.14d: 0x005851C0
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseStrength(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC79860
//1.14d: 0x005851E0
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseHitpoints(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC798A0
//1.14d: 0x00585220
int32_t __fastcall OBJMODE_ObeliskPowerUp_IncreaseSkillpoints(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FC798C0
//1.14d: 0x005852E0
int32_t __fastcall OBJMODE_MainObeliskHandler(D2GameStrc* pGame, int32_t nPlayerGUID, int32_t nObjectGUID, int32_t nItemGUID, int16_t nState);
//D2Game.0x6FC79B50
//1.14d: 0x005854D0
void __fastcall OBJMODE_CloseObjectInteraction(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pObject);
//D2Game.0x6FC79C00
//1.14d: 0x00585550
void __fastcall OBJMODE_ResetObjectInteraction(D2GameStrc* pGame, D2UnitStrc* pObject);
//D2Game.0x6FC79C30
//1.14d: 0x00585580
void __fastcall OBJMODE_RelocateTownPortal(D2GameStrc* pGame, int32_t nUnused, int32_t nPortalGUID, int32_t nDestLevel);
//D2Game.0x6FC79D90
//1.14d: 0x005858A0
int32_t __fastcall OBJECTS_OperateFunction22_Well(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC7A000
//1.14d: 0x005856A0
void __fastcall OBJMODE_PetIterate_Heal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pUnit2, void* pUnitHealed);
//D2Game.0x6FC7A070
//1.14d: 0x005869F0
int32_t __fastcall OBJECTS_OperateFunction48_TrappedSoul(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC7A140
//1.14d: 0x00585970
D2UnitStrc* __fastcall OBJMODE_DropItemWithCodeAndQuality(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t dwCode, int32_t nItemQuality);
//D2Game.0x6FC7A220
//1.14d: 0x00585B90
D2UnitStrc* __fastcall OBJMODE_DropFromChestTCWithQuality(D2ObjOperateFnStrc* pOp, int32_t nItemQuality);
