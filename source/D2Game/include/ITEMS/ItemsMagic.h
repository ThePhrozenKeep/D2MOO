#pragma once

#include <Units/Units.h>

#include <DataTbls/ItemsTbls.h>
#include "Items.h"

//D2Game.0x6FC52920
//1.14d: 0x005C18E0
int16_t __fastcall ITEMS_RollMagicAffixes(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefixes, int32_t nPreferredAffixIndex);
//D2Game.0x6FC52980
//1.14d: 0x005C12F0
int16_t __fastcall ITEMS_RollMagicAffixesOld(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefixes, int32_t nPreferredAffixIndex);
//D2Game.0x6FC52C00
//1.14d: 0x005C1560
int16_t __fastcall ITEMS_RollMagicAffixesNew(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefixes, int32_t nPreferredAffixIndex, int32_t nAutoMagicGroup);
//D2Game.0x6FC53080
//1.14d: 0x005C1940
int16_t __fastcall ITEMS_RollMagicAffixes(D2UnitStrc* pItem, int32_t bRequireSpawnableAffix, int32_t bForceAffixRoll, int32_t bAssignProperties, int32_t bPrefixes, int32_t nPreferredAffixIndex, int32_t nAutoMagicGroup);
//D2Game.0x6FC530E0
//1.14d: 0x005C1BC0
int16_t __fastcall ITEMS_RollTemperedItemAffix(D2UnitStrc* pItem, int32_t bPrefix);
//D2Game.0x6FC53360
int32_t __fastcall D2GAME_RollRareItem_6FC53360(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC53610
//1.14d: 0x005C1AB0
int16_t __fastcall D2GAME_RollRareAffix_6FC53610(D2UnitStrc* pItem, int32_t bPrefix);
//D2Game.0x6FC53760
//1.14d: 0x005C1E80
int32_t __fastcall ITEMS_RollLegacyRareItem(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC53CD0
//1.14d: 0x005C21D0
int32_t __fastcall ITEMS_RollCraftedItem(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC54240
//1.14d: 0x005C2540
int32_t __fastcall ITEMS_FindBookRecordIndex(D2UnitStrc* pItem, int32_t bScroll);
//D2Game.0x6FC542C0
int32_t __fastcall ITEMS_RollSetItem(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC544A0
//1.14d: 0x005C2740
int32_t __fastcall ITEMS_RollLegacySetItem(D2UnitStrc* pItem);
//D2Game.0x6FC54690
int32_t __fastcall ITEMS_RollSuperiorItem(D2UnitStrc* pUnit, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC549F0
//1.14d: 0x005C2AF0
int32_t __fastcall ITEMS_ApplyLegacyLowQuality(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC54D00
//1.14d: 0x005C2D40
int32_t __fastcall ITEMS_ApplyLowQuality(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC55060
void __fastcall ITEMS_AssignLegacyStaffMods(D2UnitStrc* pUnit, int32_t nItemLevel, int32_t nClassFirstSkillId, int32_t nUnusedSkillCount);
//D2Game.0x6FC55270
//1.14d: 0x005C2FB0
int32_t __fastcall ITEMS_ApplyLowQualityForFormat(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
