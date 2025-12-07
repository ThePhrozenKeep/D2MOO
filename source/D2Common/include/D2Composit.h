#pragma once

#include "D2CommonDefinitions.h"
#include <D2Inventory.h>

#pragma pack(1)

enum D2Composits
{
	COMPOSIT_HEAD,
	COMPOSIT_TORSO,
	COMPOSIT_LEGS,
	COMPOSIT_RIGHTARM,
	COMPOSIT_LEFTARM,
	COMPOSIT_RIGHTHAND,
	COMPOSIT_LEFTHAND,
	COMPOSIT_SHIELD,
	COMPOSIT_SPECIAL1,
	COMPOSIT_SPECIAL2,
	COMPOSIT_SPECIAL3,
	COMPOSIT_SPECIAL4,
	COMPOSIT_SPECIAL5,
	COMPOSIT_SPECIAL6,
	COMPOSIT_SPECIAL7,
	COMPOSIT_SPECIAL8,
	NUM_COMPONENTS,
};

enum D2WeaponClasses
{
	WEAPONCLASS_HTH, // Hand To Hand (Bare fists)
	WEAPONCLASS_BOW, // Bow (All Bow class weapons)
	WEAPONCLASS_1HS, // One-Handed: Swing (axes, swords, flails, maces, hammers, clubs and wands)
	WEAPONCLASS_1HT, // One-Handed: Thrust (Daggers, javelins, potions)
	WEAPONCLASS_STF, // Staff (Staves, polearms, axes, mauls)
	WEAPONCLASS_2HS, // Two-Handed: Swing (swords)
	WEAPONCLASS_2HT, // Two-Handed: Thrust (All Spear class weapons)
	WEAPONCLASS_XBW, // Crossbow (All Crossbow class weapons)
	WEAPONCLASS_1JS, // 2 x One-handed: left Jab  , right Swing
	WEAPONCLASS_1JT, // 2 x One-handed: left Jab  , right Thrust
	WEAPONCLASS_1SS, // 2 x One-handed: left Swing, right Swing
	WEAPONCLASS_1ST, // 2 x One-handed: left Swing, right Thrust
	WEAPONCLASS_HT1, // One Hand To hand (Katar, Claw, Scissors type assassin class weapons)
	WEAPONCLASS_HT2, // Two Hand To hand (Katar, claw, scissors type assassin class weapons)
	NUM_WEAPON_CLASSES,

};

struct D2CompositStrc
{
	int32_t nWeaponClassCode;					//0x00
	int32_t nWeaponClassId;						//0x04
};

#pragma pack()

//D2Common.0x6FD466C0 (#10884)
D2COMMON_DLL_DECL void __stdcall D2Common_10884_COMPOSIT_unk(D2UnitStrc* pUnit, int nClass, int nMode, int nUnitType, D2InventoryStrc* pInventory, char* szPath, int* pWeaponClassCode, BOOL bAddPathPrefix, int a9);
//D2Common.0x6FD46BC0 (#10885)
D2COMMON_DLL_DECL void __stdcall D2Common_10885_COMPOSIT_unk(D2UnitStrc* pUnit, char* szPath, int* pWeaponClassCode, BOOL bAddPathPrefix, int a5, D2InventoryStrc* pInventory, int nAnimMode);
//D2Common.0x6FD46C60 (#10886)
D2COMMON_DLL_DECL int __stdcall COMPOSIT_GetWeaponClassIdFromCode(int nWeaponClassCode);
//D2Common.0x6FD46C90 (#10887)
D2COMMON_DLL_DECL int __stdcall COMPOSIT_GetWeaponClassCode(D2UnitStrc* pUnit, int nUnitType, int nClass, int nMode, D2InventoryStrc* pInventory, int* pWeaponClassId);
//D2Common.0x6FD47150 (#10888)
D2COMMON_DLL_DECL int __stdcall COMPOSIT_GetWeaponClassId(D2UnitStrc* pUnit, D2InventoryStrc* pInventory, int* pWeaponClassId, int nAnimMode, BOOL a5);
//D2Common.0x6FD47200 (#10889)
D2COMMON_DLL_DECL BOOL __stdcall COMPOSIT_IsArmorComponent(int nComponent);
//D2Common.0x6FD47230 (#10890)
D2COMMON_DLL_DECL BOOL __stdcall COMPOSIT_IsWeaponBowOrXBow(D2UnitStrc* pUnit);
//D2Common.0x6FD472E0 (#10891)
D2COMMON_DLL_DECL unsigned __stdcall COMPOSIT_GetArmorTypeFromComponent(int nComponent, uint8_t* pArmorComponents);
