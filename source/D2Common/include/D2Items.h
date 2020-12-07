#pragma once

#include "CommonDefinitions.h"

//TODO: Revise



//D2Common.0x6FD98380 (#10687)
void __stdcall ITEMS_AllocItemData(void* pMemPool, D2UnitStrc* pItem);
//D2Common.0x6FD983F0 (#10688)
void __stdcall ITEMS_FreeItemData(void* pMemPool, D2UnitStrc* pItem);
//D2Common.0x6FD98430 (#10689)
BYTE __stdcall ITEMS_GetBodyLocation(D2UnitStrc* pItem);
//D2Common.0x6FD98450 (#10690)
void __stdcall ITEMS_SetBodyLocation(D2UnitStrc* pItem, BYTE nBodyLoc);
//D2Common.0x6FD98470 (#10691)
D2SeedStrc* __stdcall ITEMS_GetItemSeed(D2UnitStrc* pItem);
//D2Common.0x6FD98490 (#10692)
void __stdcall ITEMS_InitItemSeed(D2UnitStrc* pItem);
//D2Common.0x6FD984B0 (#10693)
int __stdcall ITEMS_GetItemStartSeed(D2UnitStrc* pItem);
//D2Common.0x6FD984D0 (#10694)
void __stdcall ITEMS_SetItemStartSeed(D2UnitStrc* pItem, int nSeed);
//D2Common.0x6FD98550 (#10695)
int __stdcall ITEMS_GetItemQuality(D2UnitStrc* pItem);
//D2Common.0x6FD98580 (#10696)
void __stdcall ITEMS_SetItemQuality(D2UnitStrc* pItem, int nQuality);
//D2Common.0x6FD985A0 (#10699)
WORD __stdcall ITEMS_GetPrefixId(D2UnitStrc* pItem, int nPrefixNo);
//D2Common.0x6FD985D0 (#10700)
void __stdcall ITEMS_AssignPrefix(D2UnitStrc* pItem, WORD nPrefix, int nPrefixNo);
//D2Common.0x6FD98600 (#10697)
WORD __stdcall ITEMS_GetAutoAffix(D2UnitStrc* pItem);
//D2Common.0x6FD98630 (#10698)
void __stdcall ITEMS_SetAutoAffix(D2UnitStrc* pItem, WORD nAffix);
//D2Common.0x6FD98650 (#10701)
WORD __stdcall ITEMS_GetSuffixId(D2UnitStrc* pItem, int nSuffixNo);
//D2Common.0x6FD98680 (#10702)
void __stdcall ITEMS_AssignSuffix(D2UnitStrc* pItem, WORD nSuffix, int nSuffixNo);
//D2Common.0x6FD986B0 (#10703)
WORD __stdcall ITEMS_GetRarePrefixId(D2UnitStrc* pItem);
//D2Common.0x6FD986E0 (#10704)
void __stdcall ITEMS_AssignRarePrefix(D2UnitStrc* pItem, WORD nPrefix);
//D2Common.0x6FD98700 (#10705)
WORD __stdcall ITEMS_GetRareSuffixId(D2UnitStrc* pItem);
//D2Common.0x6FD98730 (#10706)
void __stdcall ITEMS_AssignRareSuffix(D2UnitStrc* pItem, WORD nSuffix);
//D2Common.0x6FD98750 (#10707)
BOOL __stdcall ITEMS_CheckItemFlag(D2UnitStrc* pItem, DWORD dwFlag, int nLine, char* szFile);
//D2Common.0x6FD98780 (#10708)
void __stdcall ITEMS_SetItemFlag(D2UnitStrc* pItem, DWORD dwFlag, BOOL bSet);
//D2Common.0x6FD987C0 (#10709)
DWORD __stdcall ITEMS_GetItemFlags(D2UnitStrc* pItem);
//D2Common.0x6FD987E0 (#10710)
BOOL __stdcall ITEMS_CheckItemCMDFlag(D2UnitStrc* pItem, int nFlag);
//D2Common.0x6FD98810 (#10711)
void __stdcall ITEMS_SetItemCMDFlag(D2UnitStrc* pItem, int nFlag, BOOL bSet);
//D2Common.0x6FD98850 (#10712)
DWORD __stdcall ITEMS_GetItemCMDFlags(D2UnitStrc* pItem);
//D2Common.0x6FD98870 (#10717)
int __stdcall ITEMS_GetItemLevel(D2UnitStrc* pItem);
//D2Common.0x6FD988B0 (#10718)
void __stdcall ITEMS_SetItemLevel(D2UnitStrc* pItem, int nItemLevel);
//D2Common.0x6FD988E0 (#10719)
BYTE __stdcall ITEMS_GetInvPage(D2UnitStrc* pItem);
//D2Common.0x6FD98900 (#10720)
void __stdcall ITEMS_SetInvPage(D2UnitStrc* pItem, BYTE nPage);
//D2Common.0x6FD98920 (#10721)
BYTE __stdcall ITEMS_GetCellOverlap(D2UnitStrc* pItem);
//D2Common.0x6FD98940 (#10722)
void __stdcall ITEMS_SetCellOverlap(D2UnitStrc* pItem, int nCellOverlap);
//D2Common.0x6FD98960 (#10853)
BYTE __stdcall ITEMS_GetItemCell(D2UnitStrc* pItem);
//D2Common.0x6FD98980 (#10854)
void __stdcall ITEMS_SetItemCell(D2UnitStrc* pItem, int nItemCell);
//D2Common.0x6FD989A0 (#10723)
char* __stdcall ITEMS_GetEarName(D2UnitStrc* pItem);
//D2Common.0x6FD989C0 (#10724)
void __stdcall ITEMS_SetEarName(D2UnitStrc* pItem, char* szName);
//D2Common.0x6FD989F0 (#10725)
BYTE __stdcall ITEMS_GetEarLevel(D2UnitStrc* pItem);
//D2Common.0x6FD98A10 (#10726)
void __stdcall ITEMS_SetEarLevel(D2UnitStrc* pItem, BYTE nLevel);
//D2Common.0x6FD98A30 (#10727)
BYTE __stdcall ITEMS_GetVarGfxIndex(D2UnitStrc* pItem);
//D2Common.0x6FD98A50 (#10728)
void __stdcall ITEMS_SetVarGfxIndex(D2UnitStrc* pItem, BYTE nIndex);
//D2Common.0x6FD98A70 (#10777)
BOOL __stdcall ITEMS_IsRepairable(D2UnitStrc* pItem);
//D2Common.0x6FD98C60 (#10780)
DWORD __stdcall ITEMS_GetAmmoTypeFromItemType(int nItemType);
//D2Common.0x6FD98CA0 (#10781)
DWORD __stdcall ITEMS_GetAmmoType(D2UnitStrc* pItem);
//D2Common.0x6FD98D20 (#10782)
DWORD __stdcall ITEMS_GetQuiverTypeFromItemType(int nItemType);
//D2Common.0x6FD98D60 (#10783)
DWORD __stdcall ITEMS_GetQuiverType(D2UnitStrc* pItem);
//D2Common.0x6FD98DE0 (#10784)
DWORD __stdcall ITEMS_GetAutoStackFromItemType(int nItemType);
//D2Common.0x6FD98E20 (#10785)
DWORD __stdcall ITEMS_GetAutoStack(D2UnitStrc* pItem);
//D2Common.0x6FD98EA0 (#10786)
DWORD __stdcall ITEMS_GetReload(D2UnitStrc* pItem);
//D2Common.0x6FD98F20 (#10787)
DWORD __stdcall ITEMS_GetReEquip(D2UnitStrc* pItem);
//D2Common.0x6FD98FA0 (#10788)
BYTE __stdcall ITEMS_GetStorePage(D2UnitStrc* pItem);
//D2Common.0x6FD99020 (#10789)
BYTE __stdcall ITEMS_GetVarInvGfxCount(D2UnitStrc* pItem);
//D2Common.0x6FD990A0 (#10790)
char* __stdcall ITEMS_GetVarInvGfxString(D2UnitStrc* pItem, int nId);
//D2Common.0x6FD99140 (#10792)
BOOL __stdcall ITEMS_CanBeRare(D2UnitStrc* pItem);
//D2Common.0x6FD991C0 (#10791)
BOOL __stdcall ITEMS_CanBeMagic(D2UnitStrc* pItem);
//D2Common.0x6FD99240 (#10793)
BOOL __stdcall ITEMS_CanBeNormal(D2UnitStrc* pItem);
//D2Common.0x6FD992C0 (#10744)
DWORD __stdcall ITEMS_GetWeaponClassCode(D2UnitStrc* pItem);
//D2Common.0x6FD992F0 (#10745)
DWORD __stdcall ITEMS_Get2HandWeaponClassCode(D2UnitStrc* pItem);
//D2Common.0x6FD99370 (#10746)
DWORD __stdcall ITEMS_GetBaseCode(D2UnitStrc* pItem);
//D2Common.0x6FD993F0 (#10747)
DWORD __stdcall ITEMS_GetAltGfx(D2UnitStrc* pItem);
//D2Common.0x6FD99480 (#10748)
BYTE __stdcall ITEMS_GetComponent(D2UnitStrc* pItem);
//D2Common.0x6FD99500 (#10749)
void __stdcall ITEMS_GetDimensions(D2UnitStrc* pItem, BYTE* pWidth, BYTE* pHeight, char* szFile, int nLine);
//D2Common.0x6FD99540 (#10750)
void __stdcall ITEMS_GetAllowedBodyLocations(D2UnitStrc* pItem, BYTE* pBodyLoc1, BYTE* pBodyLoc2);
//D2Common.0x6FD995D0 (#10751)
DWORD __stdcall ITEMS_GetItemType(D2UnitStrc* pItem);
//D2Common.0x6FD99640 (#10752)
DWORD __stdcall ITEMS_GetItemTypeFromItemId(DWORD dwItemId);
//D2Common.0x6FD99680 (#10753)
BYTE __stdcall ITEMS_GetItemQlvl(D2UnitStrc* pItem);
//D2Common.0x6FD99700 (#10754)
int __stdcall ITEMS_CheckIfFlagIsSet(int nFlags, int nFlag);
//D2Common.0x6FD99710 (#10755)
void __stdcall ITEMS_SetOrRemoveFlag(int* pFlags, int nFlag, BOOL bSet);
//D2Common.0x6FD99740 (#10756)
BOOL __stdcall ITEMS_CheckRequirements(D2UnitStrc* pItem, D2UnitStrc* pUnit, BOOL bEquipping, BOOL* bStrength, BOOL* bDexterity, BOOL* bLevel);
//D2Common.0x6FD99BC0 (#10741)
BOOL __stdcall ITEMS_GetQuestFromItemId(int nItemId);
//D2Common.0x6FD99C60 (#10742)
BOOL __stdcall ITEMS_GetQuest(D2UnitStrc* pItem);
//D2Common.0x6FD99D40 (#10743)
DWORD __stdcall ITEMS_GetNormalCode(D2UnitStrc* pItem);
//D2Common.0x6FD99DB0
int __fastcall ITEMS_GetRequiredLevel(D2UnitStrc* pItem, D2UnitStrc* pPlayer);
//D2Common.0x6FD9A3F0 (#10757)
int __stdcall ITEMS_GetLevelRequirement(D2UnitStrc* pItem, D2UnitStrc* pUnit);
//D2Common.0x6FD9A400 (#10758)
BOOL __stdcall ITEMS_CheckBodyLocation(D2UnitStrc* pItem, BYTE nBodyLoc);
//D2Common.0x6FD9A4F0 (#10762)
int __stdcall ITEMS_CheckItemTypeIfThrowable(int nItemType);
//D2Common.0x6FD9A530 (#10759)
int __stdcall ITEMS_CheckIfThrowable(D2UnitStrc* pItem);
//D2Common.0x6FD9A5B0 (#10760)
int __stdcall ITEMS_GetMissileType(D2UnitStrc* pItem);
//D2Common.0x6FD9A5E0 (#10761)
BYTE __stdcall ITEMS_GetMeleeRange(D2UnitStrc* pItem);
//D2Common.0x6FD9A610 (#10763)
BOOL __stdcall ITEMS_CheckWeaponClassByItemId(int nItemId, int nWeapClass);
//D2Common.0x6FD9A660 (#10764)
BOOL __stdcall ITEMS_CheckWeaponClass(D2UnitStrc* pItem, int nWeapClass);
//D2Common.0x6FD9A6C0 (#10766)
DWORD __stdcall ITEMS_CheckWeaponIfTwoHandedByItemId(int nItemId);
//D2Common.0x6FD9A700 (#10765)
DWORD __stdcall ITEMS_CheckWeaponIfTwoHanded(D2UnitStrc* pItem);
//D2Common.0x6FD9A750 (#10767)
DWORD __stdcall ITEMS_CheckIfStackable(D2UnitStrc* pItem);
//D2Common.0x6FD9A7A0 (#10768)
DWORD __stdcall ITEMS_CheckIfBeltable(D2UnitStrc* pItem);
//D2Common.0x6FD9A820 (#10769)
BOOL __stdcall ITEMS_ComparePotionTypes(D2UnitStrc* pItem1, D2UnitStrc* pItem2);
//D2Common.0x6FD9A960 (#10770)
BOOL __stdcall ITEMS_CheckIfAutoBeltable(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD9AA00 (#10771)
BOOL __stdcall ITEMS_CheckIfUseable(D2UnitStrc* pItem);
//D2Common.0x6FD9AA70 (#10772)
int __stdcall ITEMS_GetUniqueColumnFromItemsTxt(D2UnitStrc* pItem);
//D2Common.0x6FD9AB00 (#10773)
BOOL __stdcall ITEMS_IsQuestItem(D2UnitStrc* pItem);
//D2Common.0x6FD9AB90
int __fastcall ITEMS_CalculateAdditionalCostsForChargedSkills(D2UnitStrc* pUnit, int nBaseCost);
//D2Common.0x6FD9ACE0
void __fastcall ITEMS_CalculateAdditionalCostsForBonusStats(D2UnitStrc* pItem, int* pSellCost, int* pBuyCost, int* pRepCost, unsigned int nDivisor);
//D2Common.0x6FD9B1C0
int __fastcall ITEMS_CalculateTransactionCost(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int nDifficulty, D2BitBufferStrc* pQuestFlags, int nVendorId, int nTransactionType);
//D2Common.0x6FD9CB50
void __fastcall ITEMS_CalculateAdditionalCostsForItemSkill(D2UnitStrc* pItem, int* pSellCost, int* pBuyCost, int* pRepCost, unsigned int nDivisor);
//D2Common.0x6FD9CDC0
int __fastcall ITEMS_CheckUnitFlagEx(D2UnitStrc* pUnit, int nFlag);
//D2Common.0x6FD9CDE0 (#10775)
int __stdcall ITEMS_GetTransactionCost(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int nDifficulty, D2BitBufferStrc* pQuestFlags, int nVendorId, int nTransactionType);
//D2Common.0x6FD9CE10 (#10794)
int __stdcall ITEMS_GetMaxStack(int nItemId);
//D2Common.0x6FD9CE50 (#10795)
int __stdcall ITEMS_GetTotalMaxStack(D2UnitStrc* pItem);
//D2Common.0x6FD9CEF0 (#10798)
int __stdcall ITEMS_GetSpawnStackFromItemId(int nItemId);
//D2Common.0x6FD9CF30 (#10799)
int __stdcall ITEMS_GetSpawnStack(D2UnitStrc* pItem);
//D2Common.0x6FD9CFB0 (#10796)
int __stdcall ITEMS_GetMinStackFromItemId(int nItemId);
//D2Common.0x6FD9CFF0 (#10797)
int __stdcall ITEMS_GetMinStack(D2UnitStrc* pItem);
//D2Common.0x6FD9D070 (#10800) - Unused
int __stdcall ITEMS_CheckBitField1Flag8(D2UnitStrc* pItem);
//D2Common.0x6FD9D0F0 (#10804)
int __stdcall ITEMS_GetSpellIcon(D2UnitStrc* pItem);
//D2Common.0x6FD9D1E0 (#10805)
BYTE __stdcall ITEMS_GetDurWarnCount(D2UnitStrc* pItem);
//D2Common.0x6FD9D260 (#10806)
BYTE __stdcall ITEMS_GetQtyWarnCount(D2UnitStrc* pItem);
//D2Common.0x6FD9D2E0 (#10807)
short __stdcall ITEMS_GetStrengthBonus(D2UnitStrc* pItem);
//D2Common.0x6FD9D310 (#10808)
short __stdcall ITEMS_GetDexBonus(D2UnitStrc* pItem);
//D2Common.0x6FD9D340 (#10809)
int __stdcall ITEMS_CheckIfSocketableByItemId(int nItemId);
//D2Common.0x6FD9D360 (#10810)
int __stdcall ITEMS_CheckIfSocketable(D2UnitStrc* pItem);
//D2Common.0x6FD9D390 (#10811)
BOOL __stdcall ITEMS_HasDurability(D2UnitStrc* pItem);
//D2Common.0x6FD9D3F0 (#10813)
int __stdcall ITEMS_GetStaffMods(D2UnitStrc* pItem);
//D2Common.0x6FD9D470 (#10814)
BYTE __stdcall ITEMS_GetAllowedGemSocketsFromItemId(int nItemId);
//D2Common.0x6FD9D490 (#10815)
BYTE __stdcall ITEMS_GetMaxSockets(D2UnitStrc* pItem);
//D2Common.0x6FD9D580 (#10816)
int __stdcall ITEMS_GetSockets(D2UnitStrc* pItem);
//D2Common.0x6FD9D5E0 (#10817)
void __stdcall ITEMS_AddSockets(D2UnitStrc* pItem, int nSockets);
//D2Common.0x6FD9D7C0 (#10818)
void __stdcall ITEMS_SetSockets(D2UnitStrc* pItem, int nSockets);
//D2Common.0x6FD9D900 (#10819)
int __stdcall ITEMS_GetGemApplyTypeFromItemId(int nItemId);
//D2Common.0x6FD9D940 (#10820)
int __stdcall ITEMS_GetGemApplyType(D2UnitStrc* pItem);
//D2Common.0x6FD9D9D0 (#10821)
BOOL __stdcall ITEMS_IsSocketFiller(D2UnitStrc* pItem);
//D2Common.0x6FD9D9E0 (#10822)
D2RunesTxt* __stdcall ITEMS_GetRunesTxtRecordFromItem(D2UnitStrc* pItem);
//D2Common.0x6FD9DBA0 (#10729)
BOOL __stdcall ITEMS_CheckItemTypeIdByItemId(int nItemId, int nItemType);
//D2Common.0x6FD9DC80 (#10730)
BOOL __stdcall ITEMS_CheckType(int nItemType1, int nItemType2);
//D2Common.0x6FD9DCE0 (#10731)
BOOL __stdcall ITEMS_CheckItemTypeId(D2UnitStrc* pItem, int nItemType);
//D2Common.0x6FD9DDD0 (#10803)
int __stdcall ITEMS_CheckBitField1Flag1(int nItemId);
//D2Common.0x6FD9DE10 (#10802)
int __stdcall ITEMS_IsMetalItem(int nItemId);
//D2Common.0x6FD9DE50 (#10801) - Unused
int __stdcall ITEMS_CheckBitField1Flag4(int nItemId);
//D2Common.0x6FD9DE90 (#10774)
BOOL __stdcall ITEMS_IsNotQuestItemByItemId(int nItemId);
//D2Common.0x6FD9DEE0 (#10732)
int __stdcall ITEMS_GetFileIndex(D2UnitStrc* pItem);
//D2Common.0x6FD9DF60 (#10733)
void __stdcall ITEMS_SetFileIndex(D2UnitStrc* pItem, DWORD dwFileIndex);
//D2Common.0x6FD9DFE0 (#11244)
void __stdcall ITEMS_GetRealmData(D2UnitStrc* pItem, int* pRealmData0, int* pRealmData1);
//D2Common.0x6FD9E070 (#11245)
void __stdcall ITEMS_SetRealmData(D2UnitStrc* pItem, int a2, int a3);
//D2Common.0x6FD9E0A0 (#10734)
void __stdcall ITEMS_SetOwnerId(D2UnitStrc* pItem, int nOwnerGUID);
//D2Common.0x6FD9E120 (#10735)
int __stdcall ITEMS_GetOwnerId(D2UnitStrc* pItem);
//D2Common.0x6FD9E1A0 (#10736)
BOOL __stdcall ITEMS_IsBodyItem(D2UnitStrc* pItem);
//D2Common.0x6FD9E2A0 (#10738)
BOOL __stdcall ITEMS_IsClassValidByItemId(int nItemId);
//D2Common.0x6FD9E310 (#10737)
BOOL __stdcall ITEMS_IsClassValid(D2UnitStrc* pItem);
//D2Common.0x6FD9E390 (#10739)
int __stdcall ITEMS_GetClassOfClassSpecificItem(D2UnitStrc* pItem);
//D2Common.0x6FD9E410 (#10823)
WORD __stdcall ITEMS_GetWeaponClassId(D2UnitStrc* pItem);
//D2Common.0x6FD9E480 (#10824)
DWORD __stdcall ITEMS_GetTransmogrifyFromItemId(int nItemId);
//D2Common.0x6FD9E4C0 (#10825)
DWORD __stdcall ITEMS_GetTransmogrify(D2UnitStrc* pItem);
//D2Common.0x6FD9E550 (#10826)
int __stdcall ITEMS_IsMagSetRarUniCrfOrTmp(D2UnitStrc* pItem);
//D2Common.0x6FD9E580 (#10740)
BOOL __stdcall ITEMS_IsNotQuestItem(D2UnitStrc* pItem);
//D2Common.0x6FD9E5F0 (#10827)
BYTE __stdcall ITEMS_GetHitClassFromItem(D2UnitStrc* pItem);
//D2Common.0x6FD9E670 (#10828)
int __stdcall ITEMS_Is1Or2Handed(D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Common.0x6FD9E710 (#10829)
BYTE* __stdcall ITEMS_GetColor(D2UnitStrc* pPlayer, D2UnitStrc* pItem, BYTE* pColor, int nTransType);
//D2Common.0x6FD9EE70
D2SetItemsTxt* __fastcall ITEMS_GetSetItemsTxtRecord(int nRecordId);
//D2Common.0x6FD9EEA0 (#10830)
BOOL __stdcall ITEMS_IsImbueable(D2UnitStrc* pItem);
//D2Common.0x6FD9F080 (#10832)
BOOL __stdcall ITEMS_IsPersonalizable(D2UnitStrc* pItem);
//D2Common.0x6FD9F260 (#10831)
BOOL __stdcall ITEMS_IsSocketable(D2UnitStrc* pItem);
//D2Common.0x6FD9F490 (#10877)
int __stdcall ITEMS_GetAllRepairCosts(D2GameStrc* pGame, D2UnitStrc* pUnit, int nNpcId, int nDifficulty, D2BitBufferStrc* pQuestFlags, void(__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*));
//D2Common.0x6FD9F720 (#10833)
BOOL __stdcall ITEMS_AreStackablesEqual(D2UnitStrc* pItem1, D2UnitStrc* pItem2);
//D2Common.0x6FD9FA70 (#10834)
BOOL __stdcall ITEMS_CanItemBeUsedForThrowSkill(D2UnitStrc* pItem);
//D2Common.0x6FD9FB40 (#11079)
int __stdcall D2COMMON_11079_Return0(int a1, int a2);
//D2Common.0x6FD9FB50 (#10836)
DWORD __stdcall ITEMS_GetSetItemsMask(D2UnitStrc* pPlayer, D2UnitStrc* pSetItem, BOOL bSkipItem);
//D2Common.0x6FD9FD80 (#10838)
D2SetItemsTxt* __stdcall ITEMS_GetSetItemsTxtRecordFromItem(D2UnitStrc* pItem);
//D2Common.0x6FD9FE20 (#10839)
BOOL __stdcall ITEMS_CanBeEquipped(D2UnitStrc* pItem);
//D2Common.0x6FD9FE70 (#10840)
BOOL __stdcall ITEMS_IsCharmUsable(D2UnitStrc* pItem, D2UnitStrc* pPlayer);
//D2Common.0x6FD9FF00 (#10776)
int __stdcall ITEMS_GetNoOfUnidItems(D2UnitStrc* pUnit);
//D2Common.0x6FD9FF90 (#10841)
int __stdcall ITEMS_GetBonusLifeBasedOnClass(D2UnitStrc* pPlayer, int nValue);
//D2Common.0x6FD9FFE0 (#10842)
int __stdcall ITEMS_GetBonusManaBasedOnClass(D2UnitStrc* pPlayer, int nValue);
//D2Common.0x6FDA0030 (#10875)
WORD __stdcall ITEMS_GetItemFormat(D2UnitStrc* pItem);
//D2Common.0x6FDA00B0 (#10876)
void __stdcall ITEMS_SetItemFormat(D2UnitStrc* pItem, WORD nItemFormat);
//D2Common.0x6FDA0130 (#10878)
int __stdcall ITEMS_GetWeaponAttackSpeed(D2UnitStrc* pUnit, D2UnitStrc* pWeapon);
//D2Common.0x6FDA02B0 (#10879)
int __stdcall ITEMS_HasUsedCharges(D2UnitStrc* pItem, BOOL* pHasChargedSkills);
//D2Common.0x6FDA0340 (#10880)
BOOL __stdcall ITEMS_IsEthereal(D2UnitStrc* pItem);
//D2Common.0x6FDA0370 (#10883)
BOOL __stdcall ITEMS_GetCompactItemDataFromBitstream(BYTE* pBitstream, size_t nSize, BOOL bCheckForHeader, D2ItemSaveStrc* pItemSave);
//D2Common.0x6FDA0490 (#10882)
size_t __stdcall ITEMS_DecodeItemFromBitstream(D2UnitStrc* pItem, BYTE* pBitstream, size_t nSize, BOOL bCheckForHeader, int* pSocketedItemCount, DWORD dwVersion, BOOL* pFail);
//D2Common.0x6FDA0620
int __fastcall ITEMS_DecodeItemBitstreamCompact(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, BOOL bCheckForHeader, DWORD dwVersion);
//D2Common.0x6FDA0A20
int __fastcall ITEMS_DecodeItemBitstreamComplete(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, BOOL bCheckForHeader, BOOL bGamble, int* pSocketedItems, DWORD dwVersion);
//D2Common.0x6FDA2690
void __fastcall ITEMS_SetDefenseOrDamage(D2UnitStrc* pUnit, int nStat);
//D2Common.0x6FDA29D0
void __fastcall ITEMS_ReadStatFromItemBitstream(D2BitBufferStrc* pBuffer, D2StatListStrc* pStatList, D2ItemStatCostTxt* pItemStatCostTxtRecord, int nStatId, DWORD dwVersion, int n109);
//D2Common.0x6FDA2BA0 (#10881)
size_t __stdcall ITEMS_SerializeItemToBitstream(D2UnitStrc* pItem, BYTE* pBitstream, size_t nSize, BOOL bServer, BOOL bSaveItemInv, BOOL bGamble);
//Inlined in D2Common.0x6FDA2C00
void __fastcall ITEMS_SerializeItemCompact(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, D2ItemsTxt* pItemsTxtRecord, BOOL bServer);
//D2Common.0x6FDA2C00
size_t __fastcall ITEMS_SerializeItem(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, BOOL bServer, BOOL bSaveItemInv, BOOL bGamble);
//D2Common.0x6FDA2FD0
void __fastcall ITEMS_WriteBitsToBitstream(D2BitBufferStrc* pBuffer, int nData, int nBits);
//D2Common.0x6FDA3010
void __fastcall ITEMS_SerializeItemComplete(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, BOOL bServer, BOOL bGamble);
//D2Common.0x6FDA42B0
D2ItemStatCostTxt* __fastcall ITEMS_GetItemStatCostTxtRecord(int nStatId);
//D2Common.0x6FDA42E0 (#10837)
int __stdcall ITEMS_GetNoOfSetItemsFromItem(D2UnitStrc* pItem);
//D2Common.0x6FDA4380
BOOL __fastcall sub_6FDA4380(D2UnitStrc* pItem, unsigned int nSetItemMask);
//D2Common.0x6FDA4490
BOOL __fastcall sub_6FDA4490(D2UnitStrc* pUnit, D2UnitStrc* pItem, int a3);
//D2Common.0x6FDA4640 (#10866)
BOOL __stdcall ITEMS_UpdateSets(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int a3, int a4);
