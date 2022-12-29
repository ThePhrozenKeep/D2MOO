#pragma once

#include "D2Structs.h"
#include <Units/Units.h>

#include <DataTbls/ItemsTbls.h>


//D2Game.0x6FC52920
int16_t __fastcall ITEMS_RollMagicAffixes(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefixes, int32_t nPreferredAffixIndex);
//D2Game.0x6FC52980
int16_t __fastcall ITEMS_RollMagicAffixesOld(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefixes, int32_t nPreferredAffixIndex);
//D2Game.0x6FC52C00
int16_t __fastcall ITEMS_RollMagicAffixesNew(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefixes, int32_t nPreferredAffixIndex, int32_t nAutoMagicGroup);
//D2Game.0x6FC53080
int16_t __fastcall ITEMS_RollMagicAffixes(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefixes, int32_t nPreferredAffixIndex, int32_t nAutoMagicGroup);
//D2Game.0x6FC530E0
int16_t __fastcall ITEMS_RollTemperedItemAffix(D2UnitStrc* pItem, int32_t bPrefix);
//D2Game.0x6FC53360
int32_t __fastcall D2GAME_RollRareItem_6FC53360(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC53610
int16_t __fastcall D2GAME_RollRareAffix_6FC53610(D2UnitStrc* pItem, int32_t bPrefix);
//D2Game.0x6FC53760
int32_t __fastcall sub_6FC53760(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC53CD0
int32_t __fastcall sub_6FC53CD0(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC54240
int32_t __fastcall sub_6FC54240(D2UnitStrc* pItem, int32_t bScroll);
//D2Game.0x6FC542C0
int32_t __fastcall sub_6FC542C0(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC544A0
int32_t __fastcall sub_6FC544A0(D2UnitStrc* pItem);
//D2Game.0x6FC54690
int32_t __fastcall sub_6FC54690(D2UnitStrc* pUnit, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC549F0
int32_t __fastcall sub_6FC549F0(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC54D00
int32_t __fastcall sub_6FC54D00(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC55060
void __fastcall sub_6FC55060(D2UnitStrc* pUnit, int32_t nItemLevel, int32_t nClassFirstSkillId, int32_t a4);
//D2Game.0x6FC55270
int32_t __fastcall sub_6FC55270(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
