#pragma once

#include "CommonDefinitions.h"
#include <DataTbls/ItemsTbls.h>
#include <Units/Units.h>

struct D2GameStrc; // From D2Game
struct D2StatListStrc;

#pragma pack(1)

enum D2C_ItemQualities
{
	ITEMQUAL_INFERIOR = 0x01, 	//0x01 Inferior
	ITEMQUAL_NORMAL = 0x02, 	//0x02 Normal
	ITEMQUAL_SUPERIOR = 0x03, 	//0x03 Superior
	ITEMQUAL_MAGIC = 0x04, 		//0x04 Magic
	ITEMQUAL_SET = 0x05, 		//0x05 Set
	ITEMQUAL_RARE = 0x06, 		//0x06 Rare
	ITEMQUAL_UNIQUE = 0x07, 	//0x07 Unique
	ITEMQUAL_CRAFT = 0x08, 		//0x08 Crafted
	ITEMQUAL_TEMPERED = 0x09	//0x09 Tempered
};

enum D2C_ItemTypes
{
	ITEMTYPE_NONE_1 = 0,
	ITEMTYPE_NONE_2,
	ITEMTYPE_SHIELD,
	ITEMTYPE_ARMOR,
	ITEMTYPE_GOLD,
	ITEMTYPE_BOW_QUIVER,
	ITEMTYPE_CROSSBOW_QUIVER,
	ITEMTYPE_PLAYER_BODY_PART,
	ITEMTYPE_HERB,
	ITEMTYPE_POTION,
	ITEMTYPE_RING,
	ITEMTYPE_ELIXIR,
	ITEMTYPE_AMULET,
	ITEMTYPE_CHARM,
	ITEMTYPE_NONE_3,
	ITEMTYPE_BOOTS,
	ITEMTYPE_GLOVES,
	ITEMTYPE_NONE_4,
	ITEMTYPE_BOOK,
	ITEMTYPE_BELT,
	ITEMTYPE_GEM,
	ITEMTYPE_TORCH,
	ITEMTYPE_SCROLL,
	ITEMTYPE_NONE_5,
	ITEMTYPE_SCEPTER,
	ITEMTYPE_WAND,
	ITEMTYPE_STAFF,
	ITEMTYPE_BOW,
	ITEMTYPE_AXE,
	ITEMTYPE_CLUB,
	ITEMTYPE_SWORD,
	ITEMTYPE_HAMMER,
	ITEMTYPE_KNIFE,
	ITEMTYPE_SPEAR,
	ITEMTYPE_POLEARM,
	ITEMTYPE_CROSSBOW,
	ITEMTYPE_MACE,
	ITEMTYPE_HELM,
	ITEMTYPE_MISSILE_POTION,
	ITEMTYPE_QUEST,
	ITEMTYPE_BODY_PART,
	ITEMTYPE_KEY,
	ITEMTYPE_THROWING_KNIFE,
	ITEMTYPE_THROWING_AXE,
	ITEMTYPE_JAVELIN,
	ITEMTYPE_WEAPON,
	ITEMTYPE_MELEE_WEAPON,
	ITEMTYPE_MISSILE_WEAPON,
	ITEMTYPE_THROWN_WEAPON,
	ITEMTYPE_COMBO_WEAPON,
	ITEMTYPE_ANY_ARMOR,
	ITEMTYPE_ANY_SHIELD,
	ITEMTYPE_MISCELLANEOUS,
	ITEMTYPE_SOCKET_FILLER,
	ITEMTYPE_SECOND_HAND,
	ITEMTYPE_STAVES_AND_RODS,
	ITEMTYPE_MISSILE,
	ITEMTYPE_BLUNT,
	ITEMTYPE_JEWEL,
	ITEMTYPE_CLASS_SPECIFIC,
	ITEMTYPE_AMAZON_ITEM,
	ITEMTYPE_BARBARIAN_ITEM,
	ITEMTYPE_NECROMANCER_ITEM,
	ITEMTYPE_PALADIN_ITEM,
	ITEMTYPE_SORCERESS_ITEM,
	ITEMTYPE_ASSASSIN_ITEM,
	ITEMTYPE_DRUID_ITEM,
	ITEMTYPE_HAND_TO_HAND,
	ITEMTYPE_ORB,
	ITEMTYPE_VOODOO_HEADS,
	ITEMTYPE_AURIC_SHIELDS,
	ITEMTYPE_PRIMAL_HELM,
	ITEMTYPE_PELT,
	ITEMTYPE_CLOAK,
	ITEMTYPE_RUNE,
	ITEMTYPE_CIRCLET,
	ITEMTYPE_HEALING_POTION,
	ITEMTYPE_MANA_POTION,
	ITEMTYPE_REJUV_POTION,
	ITEMTYPE_STAMINA_POTION,
	ITEMTYPE_ANTIDOTE_POTION,
	ITEMTYPE_THAWING_POTION,
	ITEMTYPE_SMALL_CHARM,
	ITEMTYPE_MEDIUM_CHARM,
	ITEMTYPE_LARGE_CHARM,
	ITEMTYPE_AMAZON_BOW,
	ITEMTYPE_AMAZON_SPEAR,
	ITEMTYPE_AMAZON_JAVELIN,
	ITEMTYPE_HAND_TO_HAND_2,
	ITEMTYPE_MAGIC_BOW_QUIV,
	ITEMTYPE_UNK,
	ITEMTYPE_CHIPPED_GEM,
	ITEMTYPE_FLAWED_GEM,
	ITEMTYPE_STANDARD_GEM,
	ITEMTYPE_FLAWLESS_GEM,
	ITEMTYPE_PERFECT_GEM,
	ITEMTYPE_AMETHYST,
	ITEMTYPE_DIAMOND,
	ITEMTYPE_EMERALD,
	ITEMTYPE_RUBY,
	ITEMTYPE_SAPPHIRE,
	ITEMTYPE_TOPAZ,
	ITEMTYPE_SKULL,
	// Own ItemTypes
	ITEMTYPE_TOME,
	ITEMTYPE_CLUESCROLL
};

enum D2C_ItemModes
{
	IMODE_STORED, 		//Item is in Storage (inventory, cube, Stash?)
	IMODE_EQUIP, 		//Item is Equippped
	IMODE_INBELT, 		//Item is in Belt Rows
	IMODE_ONGROUND, 	//Item is on Ground
	IMODE_ONCURSOR, 	//Item is on Cursor
	IMODE_DROPPING, 	//Item is Being Dropped
	IMODE_SOCKETED		//Item is Socketed in another Item
};

enum D2C_ItemFlags
{
	IFLAG_NEWITEM = 0x00000001,
	IFLAG_TARGET = 0x00000002,
	IFLAG_TARGETING = 0x00000004,
	IFLAG_DELETED = 0x00000008,
	IFLAG_IDENTIFIED = 0x00000010,
	IFLAG_QUANTITY = 0x00000020,
	IFLAG_SWITCHIN = 0x00000040,
	IFLAG_SWITCHOUT = 0x00000080,
	IFLAG_BROKEN = 0x00000100,
	IFLAG_REPAIRED = 0x00000200,
	IFLAG_UNK1 = 0x00000400,
	IFLAG_SOCKETED = 0x00000800,
	IFLAG_NOSELL = 0x00001000,
	IFLAG_INSTORE = 0x00002000,
	IFLAG_NOEQUIP = 0x00004000,
	IFLAG_NAMED = 0x00008000,
	IFLAG_ISEAR = 0x00010000,
	IFLAG_STARTITEM = 0x00020000,
	IFLAG_UNK2 = 0x00040000,
	IFLAG_INIT = 0x00080000,
	IFLAG_UNK3 = 0x00100000,
	IFLAG_COMPACTSAVE = 0x00200000,
	IFLAG_ETHEREAL = 0x00400000,
	IFLAG_JUSTSAVED = 0x00800000,
	IFLAG_PERSONALIZED = 0x01000000,
	IFLAG_LOWQUALITY = 0x02000000,
	IFLAG_RUNEWORD = 0x04000000,
	IFLAG_ITEM = 0x08000000
};

enum D2C_UniqueItemFlags
{
	UNIQUEITEM_ENABLED = 0x00000001,
	UNIQUEITEM_NOLIMIT = 0x00000002,
	UNIQUEITEM_CARRY1 = 0x00000004,
	UNIQUEITEM_LADDER = 0x00000008,
};

enum D2C_ItemDropFlags
{
	ITEMDROPFLAG_ALWAYSETH = 0x00000004,
};

enum D2C_ItemCmdFlags
{
	ITEMCMDFLAG_DELETE = 0x00000020,
};

struct D2ItemSaveStrc
{
	int32_t nClassId;							//0x00
	uint16_t nX;								//0x04
	uint16_t nY;								//0x06
	uint8_t nAnimMode;							//0x08
	uint8_t pad0x09[3];							//0x09
	uint32_t dwFlags;							//0x0C
	uint8_t nStorePage;							//0x10
	uint8_t nBodyloc;							//0x11
	uint8_t pad0x12[2];							//0x12
	int32_t nItemFileIndex;						//0x14
};
#pragma pack()

// Helper function
// Checks that the given unit is an item with data
inline D2ItemDataStrc* ITEMS_GetItemData(D2UnitStrc* pItem) { return (pItem && pItem->dwUnitType == UNIT_ITEM && pItem->pItemData) ? pItem->pItemData : nullptr; }

//D2Common.0x6FD98380 (#10687)
D2COMMON_DLL_DECL void __stdcall ITEMS_AllocItemData(void* pMemPool, D2UnitStrc* pItem);
//D2Common.0x6FD983F0 (#10688)
D2COMMON_DLL_DECL void __stdcall ITEMS_FreeItemData(void* pMemPool, D2UnitStrc* pItem);
//D2Common.0x6FD98430 (#10689)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetBodyLocation(D2UnitStrc* pItem);
//D2Common.0x6FD98450 (#10690)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetBodyLocation(D2UnitStrc* pItem, uint8_t nBodyLoc);
//D2Common.0x6FD98470 (#10691)
D2COMMON_DLL_DECL D2SeedStrc* __stdcall ITEMS_GetItemSeed(D2UnitStrc* pItem);
//D2Common.0x6FD98490 (#10692)
D2COMMON_DLL_DECL void __stdcall ITEMS_InitItemSeed(D2UnitStrc* pItem);
//D2Common.0x6FD984B0 (#10693)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetItemStartSeed(D2UnitStrc* pItem);
//D2Common.0x6FD984D0 (#10694)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetItemStartSeed(D2UnitStrc* pItem, int nSeed);
//D2Common.0x6FD98550 (#10695)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetItemQuality(D2UnitStrc* pItem);
//D2Common.0x6FD98580 (#10696)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetItemQuality(D2UnitStrc* pItem, int nQuality);
//D2Common.0x6FD985A0 (#10699)
D2COMMON_DLL_DECL uint16_t __stdcall ITEMS_GetPrefixId(D2UnitStrc* pItem, int nPrefixNo);
//D2Common.0x6FD985D0 (#10700)
D2COMMON_DLL_DECL void __stdcall ITEMS_AssignPrefix(D2UnitStrc* pItem, uint16_t nPrefix, int nPrefixNo);
//D2Common.0x6FD98600 (#10697)
D2COMMON_DLL_DECL uint16_t __stdcall ITEMS_GetAutoAffix(D2UnitStrc* pItem);
//D2Common.0x6FD98630 (#10698)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetAutoAffix(D2UnitStrc* pItem, uint16_t nAffix);
//D2Common.0x6FD98650 (#10701)
D2COMMON_DLL_DECL uint16_t __stdcall ITEMS_GetSuffixId(D2UnitStrc* pItem, int nSuffixNo);
//D2Common.0x6FD98680 (#10702)
D2COMMON_DLL_DECL void __stdcall ITEMS_AssignSuffix(D2UnitStrc* pItem, uint16_t nSuffix, int nSuffixNo);
//D2Common.0x6FD986B0 (#10703)
D2COMMON_DLL_DECL uint16_t __stdcall ITEMS_GetRarePrefixId(D2UnitStrc* pItem);
//D2Common.0x6FD986E0 (#10704)
D2COMMON_DLL_DECL void __stdcall ITEMS_AssignRarePrefix(D2UnitStrc* pItem, uint16_t nPrefix);
//D2Common.0x6FD98700 (#10705)
D2COMMON_DLL_DECL uint16_t __stdcall ITEMS_GetRareSuffixId(D2UnitStrc* pItem);
//D2Common.0x6FD98730 (#10706)
D2COMMON_DLL_DECL void __stdcall ITEMS_AssignRareSuffix(D2UnitStrc* pItem, uint16_t nSuffix);
//D2Common.0x6FD98750 (#10707)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CheckItemFlag(D2UnitStrc* pItem, uint32_t dwFlag, int nLine, const char* szFile);
//D2Common.0x6FD98780 (#10708)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetItemFlag(D2UnitStrc* pItem, uint32_t dwFlag, BOOL bSet);
//D2Common.0x6FD987C0 (#10709)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetItemFlags(D2UnitStrc* pItem);
//D2Common.0x6FD987E0 (#10710)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CheckItemCMDFlag(D2UnitStrc* pItem, int nFlag);
//D2Common.0x6FD98810 (#10711)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetItemCMDFlag(D2UnitStrc* pItem, int nFlag, BOOL bSet);
//D2Common.0x6FD98850 (#10712)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetItemCMDFlags(D2UnitStrc* pItem);
//D2Common.0x6FD98870 (#10717)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetItemLevel(D2UnitStrc* pItem);
//D2Common.0x6FD988B0 (#10718)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetItemLevel(D2UnitStrc* pItem, int nItemLevel);
//D2Common.0x6FD988E0 (#10719)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetInvPage(D2UnitStrc* pItem);
//D2Common.0x6FD98900 (#10720)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetInvPage(D2UnitStrc* pItem, uint8_t nPage);
//D2Common.0x6FD98920 (#10721)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetCellOverlap(D2UnitStrc* pItem);
//D2Common.0x6FD98940 (#10722)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetCellOverlap(D2UnitStrc* pItem, int nCellOverlap);
//D2Common.0x6FD98960 (#10853)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetItemCell(D2UnitStrc* pItem);
//D2Common.0x6FD98980 (#10854)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetItemCell(D2UnitStrc* pItem, int nItemCell);
//D2Common.0x6FD989A0 (#10723)
D2COMMON_DLL_DECL char* __stdcall ITEMS_GetEarName(D2UnitStrc* pItem);
//D2Common.0x6FD989C0 (#10724)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetEarName(D2UnitStrc* pItem, char* szName);
//D2Common.0x6FD989F0 (#10725)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetEarLevel(D2UnitStrc* pItem);
//D2Common.0x6FD98A10 (#10726)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetEarLevel(D2UnitStrc* pItem, uint8_t nLevel);
//D2Common.0x6FD98A30 (#10727)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetVarGfxIndex(D2UnitStrc* pItem);
//D2Common.0x6FD98A50 (#10728)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetVarGfxIndex(D2UnitStrc* pItem, uint8_t nIndex);
//D2Common.0x6FD98A70 (#10777)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsRepairable(D2UnitStrc* pItem);
//D2Common.0x6FD98C60 (#10780)
D2COMMON_DLL_DECL int32_t __stdcall ITEMS_GetAmmoTypeFromItemType(int nItemType);
//D2Common.0x6FD98CA0 (#10781)
D2COMMON_DLL_DECL int32_t __stdcall ITEMS_GetAmmoType(D2UnitStrc* pItem);
//D2Common.0x6FD98D20 (#10782)
D2COMMON_DLL_DECL int32_t __stdcall ITEMS_GetQuiverTypeFromItemType(int nItemType);
//D2Common.0x6FD98D60 (#10783)
D2COMMON_DLL_DECL int32_t __stdcall ITEMS_GetQuiverType(D2UnitStrc* pItem);
//D2Common.0x6FD98DE0 (#10784)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetAutoStackFromItemType(int nItemType);
//D2Common.0x6FD98E20 (#10785)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetAutoStack(D2UnitStrc* pItem);
//D2Common.0x6FD98EA0 (#10786)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetReload(D2UnitStrc* pItem);
//D2Common.0x6FD98F20 (#10787)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetReEquip(D2UnitStrc* pItem);
//D2Common.0x6FD98FA0 (#10788)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetStorePage(D2UnitStrc* pItem);
//D2Common.0x6FD99020 (#10789)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetVarInvGfxCount(D2UnitStrc* pItem);
//D2Common.0x6FD990A0 (#10790)
D2COMMON_DLL_DECL char* __stdcall ITEMS_GetVarInvGfxString(D2UnitStrc* pItem, int nId);
//D2Common.0x6FD99140 (#10792)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CanBeRare(D2UnitStrc* pItem);
//D2Common.0x6FD991C0 (#10791)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CanBeMagic(D2UnitStrc* pItem);
//D2Common.0x6FD99240 (#10793)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CanBeNormal(D2UnitStrc* pItem);
//D2Common.0x6FD992C0 (#10744)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetWeaponClassCode(D2UnitStrc* pItem);
//D2Common.0x6FD992F0 (#10745)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_Get2HandWeaponClassCode(D2UnitStrc* pItem);
//D2Common.0x6FD99370 (#10746)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetBaseCode(D2UnitStrc* pItem);
//D2Common.0x6FD993F0 (#10747)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetAltGfx(D2UnitStrc* pItem);
//D2Common.0x6FD99480 (#10748)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetComponent(D2UnitStrc* pItem);
//D2Common.0x6FD99500 (#10749)
D2COMMON_DLL_DECL void __stdcall ITEMS_GetDimensions(D2UnitStrc* pItem, uint8_t* pWidth, uint8_t* pHeight, const char* szFile, int nLine);
//D2Common.0x6FD99540 (#10750)
D2COMMON_DLL_DECL void __stdcall ITEMS_GetAllowedBodyLocations(D2UnitStrc* pItem, uint8_t* pBodyLoc1, uint8_t* pBodyLoc2);
//D2Common.0x6FD995D0 (#10751)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetItemType(D2UnitStrc* pItem);
//D2Common.0x6FD99640 (#10752)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetItemTypeFromItemId(uint32_t dwItemId);
//D2Common.0x6FD99680 (#10753)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetItemQlvl(const D2UnitStrc* pItem);
//D2Common.0x6FD99700 (#10754)
D2COMMON_DLL_DECL int __stdcall ITEMS_CheckIfFlagIsSet(int nFlags, int nFlag);
//D2Common.0x6FD99710 (#10755)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetOrRemoveFlag(int* pFlags, int nFlag, BOOL bSet);
//D2Common.0x6FD99740 (#10756)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CheckRequirements(D2UnitStrc* pItem, D2UnitStrc* pUnit, BOOL bEquipping, BOOL* bStrength, BOOL* bDexterity, BOOL* bLevel);
//D2Common.0x6FD99BC0 (#10741)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_GetQuestFromItemId(int nItemId);
//D2Common.0x6FD99C60 (#10742)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_GetQuest(D2UnitStrc* pItem);
//D2Common.0x6FD99D40 (#10743)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetNormalCode(D2UnitStrc* pItem);
//D2Common.0x6FD99DB0
int __fastcall ITEMS_GetRequiredLevel(D2UnitStrc* pItem, D2UnitStrc* pPlayer);
//D2Common.0x6FD9A3F0 (#10757)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetLevelRequirement(D2UnitStrc* pItem, D2UnitStrc* pUnit);
//D2Common.0x6FD9A400 (#10758)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CheckBodyLocation(D2UnitStrc* pItem, uint8_t nBodyLoc);
//D2Common.0x6FD9A4F0 (#10762)
D2COMMON_DLL_DECL int __stdcall ITEMS_CheckItemTypeIfThrowable(int nItemType);
//D2Common.0x6FD9A530 (#10759)
D2COMMON_DLL_DECL int __stdcall ITEMS_CheckIfThrowable(D2UnitStrc* pItem);
//D2Common.0x6FD9A5B0 (#10760)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetMissileType(D2UnitStrc* pItem);
//D2Common.0x6FD9A5E0 (#10761)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetMeleeRange(D2UnitStrc* pItem);
//D2Common.0x6FD9A610 (#10763)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CheckWeaponClassByItemId(int nItemId, int nWeapClass);
//D2Common.0x6FD9A660 (#10764)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CheckWeaponClass(D2UnitStrc* pItem, int nWeapClass);
//D2Common.0x6FD9A6C0 (#10766)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_CheckWeaponIfTwoHandedByItemId(int nItemId);
//D2Common.0x6FD9A700 (#10765)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_CheckWeaponIfTwoHanded(D2UnitStrc* pItem);
//D2Common.0x6FD9A750 (#10767)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_CheckIfStackable(D2UnitStrc* pItem);
//D2Common.0x6FD9A7A0 (#10768)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_CheckIfBeltable(D2UnitStrc* pItem);
//D2Common.0x6FD9A820 (#10769)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_ComparePotionTypes(D2UnitStrc* pItem1, D2UnitStrc* pItem2);
//D2Common.0x6FD9A960 (#10770)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CheckIfAutoBeltable(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD9AA00 (#10771)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CheckIfUseable(D2UnitStrc* pItem);
//D2Common.0x6FD9AA70 (#10772)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetUniqueColumnFromItemsTxt(D2UnitStrc* pItem);
//D2Common.0x6FD9AB00 (#10773)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsQuestItem(D2UnitStrc* pItem);
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
D2COMMON_DLL_DECL int __stdcall ITEMS_GetTransactionCost(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int nDifficulty, D2BitBufferStrc* pQuestFlags, int nVendorId, int nTransactionType);
//D2Common.0x6FD9CE10 (#10794)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetMaxStack(int nItemId);
//D2Common.0x6FD9CE50 (#10795)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetTotalMaxStack(D2UnitStrc* pItem);
//D2Common.0x6FD9CEF0 (#10798)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetSpawnStackFromItemId(int nItemId);
//D2Common.0x6FD9CF30 (#10799)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetSpawnStack(D2UnitStrc* pItem);
//D2Common.0x6FD9CFB0 (#10796)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetMinStackFromItemId(int nItemId);
//D2Common.0x6FD9CFF0 (#10797)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetMinStack(D2UnitStrc* pItem);
//D2Common.0x6FD9D070 (#10800) - Unused
int __stdcall ITEMS_CheckBitField1Flag8(D2UnitStrc* pItem);
//D2Common.0x6FD9D0F0 (#10804)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetSpellIcon(D2UnitStrc* pItem);
//D2Common.0x6FD9D1E0 (#10805)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetDurWarnCount(D2UnitStrc* pItem);
//D2Common.0x6FD9D260 (#10806)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetQtyWarnCount(D2UnitStrc* pItem);
//D2Common.0x6FD9D2E0 (#10807)
D2COMMON_DLL_DECL short __stdcall ITEMS_GetStrengthBonus(D2UnitStrc* pItem);
//D2Common.0x6FD9D310 (#10808)
D2COMMON_DLL_DECL short __stdcall ITEMS_GetDexBonus(D2UnitStrc* pItem);
//D2Common.0x6FD9D340 (#10809)
D2COMMON_DLL_DECL int __stdcall ITEMS_CheckIfSocketableByItemId(int nItemId);
//D2Common.0x6FD9D360 (#10810)
D2COMMON_DLL_DECL int __stdcall ITEMS_CheckIfSocketable(D2UnitStrc* pItem);
//D2Common.0x6FD9D390 (#10811)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_HasDurability(D2UnitStrc* pItem);
//D2Common.0x6FD9D3F0 (#10813)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetStaffMods(D2UnitStrc* pItem);
//D2Common.0x6FD9D470 (#10814)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetAllowedGemSocketsFromItemId(int nItemId);
//D2Common.0x6FD9D490 (#10815)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetMaxSockets(D2UnitStrc* pItem);
//D2Common.0x6FD9D580 (#10816)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetSockets(D2UnitStrc* pItem);
//D2Common.0x6FD9D5E0 (#10817)
D2COMMON_DLL_DECL void __stdcall ITEMS_AddSockets(D2UnitStrc* pItem, int nSockets);
//D2Common.0x6FD9D7C0 (#10818)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetSockets(D2UnitStrc* pItem, int nSockets);
//D2Common.0x6FD9D900 (#10819)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetGemApplyTypeFromItemId(int nItemId);
//D2Common.0x6FD9D940 (#10820)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetGemApplyType(D2UnitStrc* pItem);
//D2Common.0x6FD9D9D0 (#10821)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsSocketFiller(D2UnitStrc* pItem);
//D2Common.0x6FD9D9E0 (#10822) Retrieves the runes.txt record based on the socketed items
const D2RunesTxt* __stdcall ITEMS_GetRunesTxtRecordFromItem(const D2UnitStrc* pItem);
//D2Common.0x6FD9DBA0 (#10729)
D2COMMON_DLL_DECL //Checks if the given item has a type equivalent to a given item type
BOOL __stdcall ITEMS_CheckItemTypeIdByItemId(int nItemId, int nItemType);
//D2Common.0x6FD9DC80 (#10730)
D2COMMON_DLL_DECL //Checks if the given item types are equivalent
BOOL __stdcall ITEMS_CheckType(int nItemType1, int nItemType2);
//D2Common.0x6FD9DCE0 (#10731)
D2COMMON_DLL_DECL //Checks if the given item has a type equivalent to a given item type
BOOL __stdcall ITEMS_CheckItemTypeId(const D2UnitStrc* pItem, int nItemType);
//D2Common.0x6FD9DDD0 (#10803)
D2COMMON_DLL_DECL int __stdcall ITEMS_CheckBitField1Flag1(int nItemId);
//D2Common.0x6FD9DE10 (#10802)
D2COMMON_DLL_DECL int __stdcall ITEMS_IsMetalItem(int nItemId);
//D2Common.0x6FD9DE50 (#10801) - Unused
int __stdcall ITEMS_CheckBitField1Flag4(int nItemId);
//D2Common.0x6FD9DE90 (#10774)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsNotQuestItemByItemId(int nItemId);
//D2Common.0x6FD9DEE0 (#10732)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetFileIndex(D2UnitStrc* pItem);
//D2Common.0x6FD9DF60 (#10733)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetFileIndex(D2UnitStrc* pItem, uint32_t dwFileIndex);
//D2Common.0x6FD9DFE0 (#11244)
D2COMMON_DLL_DECL void __stdcall ITEMS_GetRealmData(D2UnitStrc* pItem, int* pRealmData0, int* pRealmData1);
//D2Common.0x6FD9E070 (#11245)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetRealmData(D2UnitStrc* pItem, int a2, int a3);
//D2Common.0x6FD9E0A0 (#10734)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetOwnerId(D2UnitStrc* pItem, D2UnitGUID nOwnerGUID);
//D2Common.0x6FD9E120 (#10735)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetOwnerId(D2UnitStrc* pItem);
//D2Common.0x6FD9E1A0 (#10736)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsBodyItem(D2UnitStrc* pItem);
//D2Common.0x6FD9E2A0 (#10738)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsClassValidByItemId(int nItemId);
//D2Common.0x6FD9E310 (#10737)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsClassValid(D2UnitStrc* pItem);
//D2Common.0x6FD9E390 (#10739)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetClassOfClassSpecificItem(D2UnitStrc* pItem);
//D2Common.0x6FD9E410 (#10823)
D2COMMON_DLL_DECL uint16_t __stdcall ITEMS_GetWeaponClassId(D2UnitStrc* pItem);
//D2Common.0x6FD9E480 (#10824)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetTransmogrifyFromItemId(int nItemId);
//D2Common.0x6FD9E4C0 (#10825)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetTransmogrify(D2UnitStrc* pItem);
//D2Common.0x6FD9E550 (#10826)
D2COMMON_DLL_DECL int __stdcall ITEMS_IsMagSetRarUniCrfOrTmp(D2UnitStrc* pItem);
//D2Common.0x6FD9E580 (#10740)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsNotQuestItem(D2UnitStrc* pItem);
//D2Common.0x6FD9E5F0 (#10827)
D2COMMON_DLL_DECL uint8_t __stdcall ITEMS_GetHitClassFromItem(D2UnitStrc* pItem);
//D2Common.0x6FD9E670 (#10828)
D2COMMON_DLL_DECL int __stdcall ITEMS_Is1Or2Handed(D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Common.0x6FD9E710 (#10829)
D2COMMON_DLL_DECL uint8_t* __stdcall ITEMS_GetColor(D2UnitStrc* pPlayer, D2UnitStrc* pItem, uint8_t* pColor, int nTransType);
//D2Common.0x6FD9EE70
D2SetItemsTxt* __fastcall ITEMS_GetSetItemsTxtRecord(int nRecordId);
//D2Common.0x6FD9EEA0 (#10830)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsImbueable(D2UnitStrc* pItem);
//D2Common.0x6FD9F080 (#10832)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsPersonalizable(D2UnitStrc* pItem);
//D2Common.0x6FD9F260 (#10831)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsSocketable(D2UnitStrc* pItem);
//D2Common.0x6FD9F490 (#10877)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetAllRepairCosts(D2GameStrc* pGame, D2UnitStrc* pUnit, int nNpcId, int nDifficulty, D2BitBufferStrc* pQuestFlags, void(__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*));
//D2Common.0x6FD9F720 (#10833)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_AreStackablesEqual(D2UnitStrc* pItem1, D2UnitStrc* pItem2);
//D2Common.0x6FD9FA70 (#10834)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CanItemBeUsedForThrowSkill(D2UnitStrc* pItem);
//D2Common.0x6FD9FB40 (#11079)
D2COMMON_DLL_DECL int __stdcall D2COMMON_11079_Return0(int a1, int a2);
//D2Common.0x6FD9FB50 (#10836)
D2COMMON_DLL_DECL uint32_t __stdcall ITEMS_GetSetItemsMask(D2UnitStrc* pPlayer, D2UnitStrc* pSetItem, BOOL bSkipItem);
//D2Common.0x6FD9FD80 (#10838)
D2COMMON_DLL_DECL D2SetItemsTxt* __stdcall ITEMS_GetSetItemsTxtRecordFromItem(D2UnitStrc* pItem);
//D2Common.0x6FD9FE20 (#10839)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_CanBeEquipped(D2UnitStrc* pItem);
//D2Common.0x6FD9FE70 (#10840)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsCharmUsable(D2UnitStrc* pItem, D2UnitStrc* pPlayer);
//D2Common.0x6FD9FF00 (#10776)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetNoOfUnidItems(D2UnitStrc* pUnit);
//D2Common.0x6FD9FF90 (#10841)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetBonusLifeBasedOnClass(D2UnitStrc* pPlayer, int nValue);
//D2Common.0x6FD9FFE0 (#10842)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetBonusManaBasedOnClass(D2UnitStrc* pPlayer, int nValue);
//D2Common.0x6FDA0030 (#10875)
D2COMMON_DLL_DECL uint16_t __stdcall ITEMS_GetItemFormat(D2UnitStrc* pItem);
//D2Common.0x6FDA00B0 (#10876)
D2COMMON_DLL_DECL void __stdcall ITEMS_SetItemFormat(D2UnitStrc* pItem, uint16_t nItemFormat);
//D2Common.0x6FDA0130 (#10878)
D2COMMON_DLL_DECL int __stdcall ITEMS_GetWeaponAttackSpeed(D2UnitStrc* pUnit, D2UnitStrc* pWeapon);
//D2Common.0x6FDA02B0 (#10879)
D2COMMON_DLL_DECL int __stdcall ITEMS_HasUsedCharges(D2UnitStrc* pItem, BOOL* pHasChargedSkills);
//D2Common.0x6FDA0340 (#10880)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_IsEthereal(D2UnitStrc* pItem);
//D2Common.0x6FDA0370 (#10883)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_GetCompactItemDataFromBitstream(uint8_t* pBitstream, size_t nSize, BOOL bCheckForHeader, D2ItemSaveStrc* pItemSave);
//D2Common.0x6FDA0490 (#10882)
D2COMMON_DLL_DECL size_t __stdcall ITEMS_DecodeItemFromBitstream(D2UnitStrc* pItem, uint8_t* pBitstream, size_t nSize, BOOL bCheckForHeader, int* pSocketedItemCount, uint32_t dwVersion, BOOL* pFail);
//D2Common.0x6FDA0620
int __fastcall ITEMS_DecodeItemBitstreamCompact(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, BOOL bCheckForHeader, uint32_t dwVersion);
//D2Common.0x6FDA0A20
int __fastcall ITEMS_DecodeItemBitstreamComplete(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, BOOL bCheckForHeader, BOOL bGamble, int* pSocketedItems, uint32_t dwVersion);
//D2Common.0x6FDA2690
void __fastcall ITEMS_SetDefenseOrDamage(D2UnitStrc* pItem, int nStat);
//D2Common.0x6FDA29D0
void __fastcall ITEMS_ReadStatFromItemBitstream(D2BitBufferStrc* pBuffer, D2StatListStrc* pStatList, D2ItemStatCostTxt* pItemStatCostTxtRecord, int nStatId, uint32_t dwVersion, int n109);
//D2Common.0x6FDA2BA0 (#10881)
D2COMMON_DLL_DECL size_t __stdcall ITEMS_SerializeItemToBitstream(D2UnitStrc* pItem, uint8_t* pBitstream, size_t nSize, BOOL bServer, BOOL bSaveItemInv, BOOL bGamble);
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
D2COMMON_DLL_DECL int __stdcall ITEMS_GetNoOfSetItemsFromItem(D2UnitStrc* pItem);
//D2Common.0x6FDA4380
BOOL __fastcall sub_6FDA4380(D2UnitStrc* pItem, unsigned int nSetItemMask);
//D2Common.0x6FDA4490
BOOL __fastcall sub_6FDA4490(D2UnitStrc* pUnit, D2UnitStrc* pItem, int a3);
//D2Common.0x6FDA4640 (#10866)
D2COMMON_DLL_DECL BOOL __stdcall ITEMS_UpdateSets(D2UnitStrc* pUnit, D2UnitStrc* pItem, int a3, int a4);
