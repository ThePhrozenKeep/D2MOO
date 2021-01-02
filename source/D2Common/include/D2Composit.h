#pragma once

#include "CommonDefinitions.h"

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
	WEAPONCLASS_NONE,
	WEAPONCLASS_BOW,
	WEAPONCLASS_1HS,
	WEAPONCLASS_1HT,
	WEAPONCLASS_STF,
	WEAPONCLASS_2HS,
	WEAPONCLASS_2HT,
	WEAPONCLASS_XBW,
	WEAPONCLASS_1JS,
	WEAPONCLASS_1JT,
	WEAPONCLASS_1SS,
	WEAPONCLASS_1ST,
	WEAPONCLASS_HT1,
	WEAPONCLASS_HT2
};

struct D2CompositStrc
{
	int32_t nWeaponClassCode;					//0x00
	int32_t nWeaponClassId;						//0x04
};

//D2Common.0x6FD466C0 (#10884)
void __stdcall D2Common_10884_COMPOSIT_unk(D2UnitStrc* pUnit, int nClass, int nMode, int nUnitType, D2InventoryStrc* pInventory, char* szPath, int* pWeaponClassCode, BOOL bAddPathPrefix, int a9);
//D2Common.0x6FD46BC0 (#10885)
void __stdcall D2Common_10885_COMPOSIT_unk(D2UnitStrc* pUnit, char* szPath, int* pWeaponClassCode, BOOL bAddPathPrefix, int a5, D2InventoryStrc* pInventory, int nAnimMode);
//D2Common.0x6FD46C60 (#10886)
int __stdcall COMPOSIT_GetWeaponClassIdFromCode(int nWeaponClassCode);
//D2Common.0x6FD46C90 (#10887)
int __stdcall COMPOSIT_GetWeaponClassCode(D2UnitStrc* pUnit, int nUnitType, int nClass, int nMode, D2InventoryStrc* pInventory, int* pWeaponClassId);
//D2Common.0x6FD47150 (#10888)
int __stdcall COMPOSIT_GetWeaponClassId(D2UnitStrc* pUnit, D2InventoryStrc* pInventory, int* pWeaponClassId, int nAnimMode, BOOL a5);
//D2Common.0x6FD47200 (#10889)
BOOL __stdcall COMPOSIT_IsArmorComponent(int nComponent);
//D2Common.0x6FD47230 (#10890)
BOOL __stdcall COMPOSIT_IsWeaponBowOrXBow(D2UnitStrc* pUnit);
//D2Common.0x6FD472E0 (#10891)
uint8_t __stdcall COMPOSIT_GetArmorTypeFromComponent(int nComponent, uint8_t* pArmorComponents);
