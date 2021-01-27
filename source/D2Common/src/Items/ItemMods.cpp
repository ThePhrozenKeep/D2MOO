#include "D2ItemMods.h"

#include "D2Collision.h"
#include "D2DataTbls.h"
#include "Drlg/D2DrlgMaze.h"
#include "D2Inventory.h"
#include "D2Items.h"
#include "Units/Missile.h"
#include "D2Seed.h"
#include "D2Skills.h"
#include "D2StatList.h"
#include "Units/Units.h"
#include <D2States.h>

/*
Date: Sun Apr 08 09:12:50 2012
Author: Necrolis
Function: STATS_ApplyProperty
Address: D2Common.0x55810 & D2Common.#11292
Comments: nApplyType was totally unused
*/
//void __stdcall STATS_ApplyProperty(int nType, int nArg, D2UnitStrc* pItem, void* pMods, int nIndex, int nPropSet, Property* pProperty, int nState, uint32_t fStatlist, int nSubType)
//{
//	if (pItem == NULL || pProperty == NULL)
//		return;
//
//	FilePropertiesTable* pRecord = INLINE_GetPropertyRecord(pProperty->nProperty);
//	if (pRecord == NULL)
//		return;
//
//	int nFirstValue = 0;
//	for (int i = 0; i < ARRAY(pRecord->nFunc); i++)
//	{
//		int nFunc = pRecord->nFunc[i];
//		if (nFunc >= ARRAY(gpfPropertyFunctions))
//			return;
//
//		ITEMPROPFUNC pfFunc = gpfPropertyFunctions[nFunc];
//		if (pfFunc == NULL)
//			return;
//
//		int nReturn = pfFunc(nType, nArg, pItem, pProperty, pRecord->nSetFunc[i], pRecord->nStat[i], pRecord->nValue[i], nFirstValue, nState, fStatlist, nSubType);
//		if (i == 0)
//			nFirstValue = nReturn;
//	}
//}


//enum eD2PropertySets
//{
//	PROPSET_WEAPON = 0,
//	PROPSET_HELM = 1,
//	PROPSET_SHEILD = 2,
//};

#define STATLIST_MAGIC 0x40
#define STATLIST_SET 0x2000

/*
Date: Sun Apr 08 08:55:02 2012
Author: Necrolis
Function: STATS_ApplyProperties
Address: D2Common.#10855
Comments: the old one was horrible...
cases 6 & 7 where unreachable...
nApplyType was unused
*/
//void __stdcall STATS_ApplyProperties(int nType, int nSubType, D2UnitStrc* pItem, void* pMods, int nPropSet, int nApplyType)
//{
//	if (pItem == NULL)
//		return;
//
//	switch (nType)
//	{
//	case PROP_AFFIXES:
//	{
//		D2MagicAffixTxt* pRecord = (D2MagicAffixTxt*)pMods;
//		if (pRecord == NULL)
//			return;
//
//		D2PropertyStrc* pProperties = pRecord->pProperties;
//		for (int i = 0; i < ARRAY_SIZE(pRecord->pProperties); i++)
//		{
//			if (!VALID_PROPERTY(pProperties->nProperty))
//				return;
//
//			STATS_ApplyProperty(PROP_AFFIXES, 0, pItem, pMods, i, nPropSet, pProperties, 0, STATLIST_MAGIC, 0);
//			pProperties++;
//		}
//
//		return;
//	}
//
//	case PROP_GEMS:
//	case PROP_RUNES:
//	{
//		D2GemsTxt* pRecord = (FileGemsTable*)pMods;
//		if (pRecord == NULL)
//			return;
//
//		D2PropertyStrc* pProperties = pRecord->pProperties[nPropSet];
//		for (int i = 0; i < ARRAY_SIZE(pRecord->pProperties[0]); i++)
//		{
//			if (!VALID_PROPERTY(pProperties->nProperty))
//				return;
//
//			STATS_ApplyProperty(nType, 0, pItem, pMods, i, nPropSet, pProperties, 0, STATLIST_MAGIC, 0);
//			pProperties++;
//		}
//
//		return;
//	}
//
//	case PROP_GRADES:
//	{
//		D2QualityItemsTxt* pRecord = (FileQualityitemsTable*)pMods;
//		if (pRecord == NULL)
//			return;
//
//		D2PropertyStrc* pProperties = pRecord->pProperties;
//		for (int i = 0; i < ARRAY(pRecord->pProperties); i++)
//		{
//			if (!VALID_PROPERTY(pProperties->nProperty))
//				return;
//
//			STATS_ApplyProperty(PROP_GRADES, 0, pItem, pMods, i, nPropSet, pProperties, 0, STATLIST_MAGIC, 0);
//			pProperties++;
//		}
//
//		return;
//	}
//
//	case PROP_UNIQUES:
//	{
//		int nSpecialIndex = ITEMS_GetFileIndex(pItem);
//		if (VALID_UNIQUEITEM(nSpecialIndex))
//		{
//			D2UniqueItemsTxt* pRecord = INLINE_GetUniqueItemRecord(nSpecialIndex);
//			if (pRecord == NULL)
//				break;
//
//			D2PropertyStrc* pProperties = pRecord->pProperties;
//			for (int i = 0; i < ARRAY(pRecord->pProperties); i++)
//			{
//				STATS_ApplyProperty(PROP_UNIQUES, 0, pItem, NULL, 0, PROPSET_WEAPON, pProperties, 0, STATLIST_MAGIC, 0);
//				pProperties++;
//			}
//		}
//
//		return;
//	}
//
//	case PROP_SETS:
//	{
//		int nSpecialIndex = ITEMS_GetFileIndex(pItem);
//		if (VALID_UNIQUEITEM(nSpecialIndex))
//		{
//			D2SetItemsTxt* pRecord = INLINE_GetSetItemRecord(nSpecialIndex);
//			if (pRecord == NULL)
//				return;
//
//			D2PropertyStrc* pProperties = pRecord->pProperties;
//			for (int i = 0; i < ARRAY_SIZE(pRecord->pProperties); i++)
//			{
//				STATS_ApplyProperty(PROP_SETS, 0, pItem, NULL, 0, PROPSET_WEAPON, pProperties, 0, STATLIST_MAGIC, 0);
//				pProperties++;
//			}
//
//			static D2PropertySetFuncStrc pSetItemFuncs[7] =
//			{
//				{ 0,					STATLIST_MAGIC },
//				{ 0,					STATLIST_MAGIC },
//				{ STATE_ITEMSET1,	STATLIST_MAGIC | STATLIST_SET },
//				{ STATE_ITEMSET2,	STATLIST_MAGIC | STATLIST_SET },
//				{ STATE_ITEMSET3,	STATLIST_MAGIC | STATLIST_SET },
//				{ STATE_ITEMSET4,	STATLIST_MAGIC | STATLIST_SET },
//				{ STATE_ITEMSET5,	STATLIST_MAGIC | STATLIST_SET }
//			};
//
//			pProperties = pRecord->pPartialBoni;
//			for (int j = 0; j < ARRAY_SIZE(pRecord->pPartialBoni); j++)
//			{
//				D2PropertySetFuncStrc* pSet = &pSetItemFuncs[pRecord->nAddFunc ? (j / 2) + 2 : 0];
//				STATS_ApplyProperty(PROP_SETS, 0, pItem, NULL, 0, PROPSET_WEAPON, pProperties, pSet->nState, pSet->fStatList, nSubType);
//				pProperties++;
//			}
//		}
//
//		return;
//	}
//
//	case PROP_UNUSED:
//	{
//		FOG_WriteToLogFile("STATS_ApplyProperties(0x%p): Unused Property Type: %d", _ReturnAddress(), nType);
//		return;
//	}
//	}
//}


struct D2ItemTypeCheckStrc
{
	uint8_t bItemType;				//0x00
	uint8_t pad0x01[3];			//0x01
	int nItemType;				//0x04
};

//TODO: Replace by the respective header includes

D2UnkFogStrc off_6FDE3BA0[] =
{
	{ MISSILE_GetMinimum, 2},
	{ MISSILE_GetMaximum, 2},
	{ sub_6FD98220, 2},
	{ sub_6FD982A0, 2},
};
int dword_6FDE3BC0 = ARRAY_SIZE(off_6FDE3BA0);

//TODO: Find names



static const D2PropertyAssignStrc stru_6FDE3160[] =
{
	{ sub_6FD92E80, STAT_ARMORCLASS },
	{ sub_6FD92E80, STAT_ARMORCLASS_VS_MISSILE },
	{ sub_6FD92E80, STAT_ARMORCLASS_VS_HTH },
	{ sub_6FD92E80, STAT_NORMAL_DAMAGE_REDUCTION },
	{ sub_6FD92E80, STAT_DAMAGERESIST },
	{ sub_6FD93200, STAT_ITEM_ARMOR_PERCENT },
	{ sub_6FD92E80, STAT_MAGIC_DAMAGE_REDUCTION },
	{ sub_6FD92E80, STAT_STRENGTH },
	{ sub_6FD92E80, STAT_DEXTERITY },
	{ sub_6FD92E80, STAT_VITALITY },
	{ sub_6FD92E80, STAT_ENERGY },
	{ sub_6FD92E80, STAT_MAXMANA },
	{ sub_6FD93200, STAT_ITEM_MAXMANA_PERCENT },
	{ sub_6FD92E80, STAT_MAXHP },
	{ sub_6FD93200, STAT_ITEM_MAXHP_PERCENT },
	{ sub_6FD92E80, STAT_TOHIT },
	{ sub_6FD92E80, STAT_TOBLOCK },
	{ sub_6FD92E80, STAT_COLDMINDAM },
	{ sub_6FD92E80, STAT_COLDMAXDAM },
	{ sub_6FD92E80, STAT_COLDLENGTH },
	{ sub_6FD92E80, STAT_FIREMINDAM },
	{ sub_6FD92E80, STAT_FIREMAXDAM },
	{ sub_6FD92E80, STAT_LIGHTMINDAM },
	{ sub_6FD92E80, STAT_LIGHTMAXDAM },
	{ sub_6FD92E80, STAT_POISONMINDAM },
	{ sub_6FD92E80, STAT_POISONMAXDAM },
	{ sub_6FD92E80, STAT_POISONLENGTH },
	{ sub_6FD93790, STAT_MINDAMAGE },
	{ sub_6FD93A20, STAT_MAXDAMAGE },
	{ sub_6FD93CB0, -1 },
	{ sub_6FD93200, STAT_ITEM_DAMAGETOMANA },
	{ sub_6FD92E80, STAT_FIRERESIST },
	{ sub_6FD92E80, STAT_MAXFIRERESIST },
	{ sub_6FD92E80, STAT_LIGHTRESIST },
	{ sub_6FD92E80, STAT_MAXLIGHTRESIST },
	{ sub_6FD92E80, STAT_COLDRESIST },
	{ sub_6FD92E80, STAT_MAXCOLDRESIST },
	{ sub_6FD92E80, STAT_MAGICRESIST },
	{ sub_6FD92E80, STAT_MAXMAGICRESIST },
	{ sub_6FD92E80, STAT_POISONRESIST },
	{ sub_6FD92E80, STAT_MAXPOISONRESIST },
	{ sub_6FD93230, -1 },
	{ sub_6FD935B0, -1 },
	{ sub_6FD92E80, STAT_ITEM_ABSORBFIRE_PERCENT },
	{ sub_6FD92E80, STAT_ITEM_ABSORBFIRE },
	{ sub_6FD92E80, STAT_ITEM_ABSORBLIGHT_PERCENT },
	{ sub_6FD92E80, STAT_ITEM_ABSORBLIGHT },
	{ sub_6FD92E80, STAT_ITEM_ABSORBMAGIC_PERCENT },
	{ sub_6FD92E80, STAT_ITEM_ABSORBMAGIC },
	{ sub_6FD92E80, STAT_ITEM_ABSORBCOLD_PERCENT },
	{ sub_6FD92E80, STAT_ITEM_ABSORBCOLD },
	{ sub_6FD92E80, STAT_MAXDURABILITY },
	{ sub_6FD93200, STAT_ITEM_MAXDURABILITY_PERCENT },
	{ sub_6FD92E80, STAT_HPREGEN },
	{ sub_6FD92E80, STAT_ITEM_ATTACKERTAKESDAMAGE },
	{ sub_6FD95210, STAT_ITEM_FASTERATTACKRATE },
	{ sub_6FD95210, STAT_ITEM_FASTERATTACKRATE },
	{ sub_6FD95210, STAT_ITEM_FASTERATTACKRATE },
	{ sub_6FD92E80, STAT_ITEM_GOLDBONUS },
	{ sub_6FD92E80, STAT_ITEM_MAGICBONUS },
	{ sub_6FD92E80, STAT_ITEM_KNOCKBACK },
	{ sub_6FD93200, STAT_STAMINARECOVERYBONUS },
	{ sub_6FD93200, STAT_MANARECOVERYBONUS },
	{ sub_6FD92E80, STAT_MAXSTAMINA },
	{ sub_6FD92E80, STAT_ITEM_TIMEDURATION },
	{ sub_6FD92E80, STAT_MANADRAINMINDAM },
	{ sub_6FD92E80, STAT_LIFEDRAINMINDAM },
	{ sub_6FD95050, STAT_ITEM_ADDCLASSSKILLS },
	{ sub_6FD95050, STAT_UNSENTPARAM1 },
	{ sub_6FD95050, STAT_ITEM_ADDEXPERIENCE },
	{ sub_6FD95050, STAT_ITEM_HEALAFTERKILL },
	{ sub_6FD95050, STAT_ITEM_REDUCEDPRICES },
	{ sub_6FD92E80, STAT_ITEM_DOUBLEHERBDURATION },
	{ sub_6FD92E80, STAT_ITEM_LIGHTRADIUS },
	{ sub_6FD92E80, STAT_ITEM_LIGHTCOLOR },
	{ sub_6FD92E80, STAT_ITEM_REQ_PERCENT },
	{ sub_6FD95210, STAT_ITEM_FASTERMOVEVELOCITY },
	{ sub_6FD95210, STAT_ITEM_FASTERMOVEVELOCITY },
	{ sub_6FD95210, STAT_ITEM_FASTERMOVEVELOCITY },
	{ sub_6FD95210, STAT_ITEM_FASTERGETHITRATE },
	{ sub_6FD95210, STAT_ITEM_FASTERGETHITRATE },
	{ sub_6FD95210, STAT_ITEM_FASTERGETHITRATE },
	{ sub_6FD95210, STAT_ITEM_FASTERBLOCKRATE },
	{ sub_6FD95210, STAT_ITEM_FASTERBLOCKRATE },
	{ sub_6FD95210, STAT_ITEM_FASTERBLOCKRATE },
	{ sub_6FD95210, STAT_ITEM_FASTERCASTRATE },
	{ sub_6FD95210, STAT_ITEM_FASTERCASTRATE },
	{ sub_6FD95210, STAT_ITEM_FASTERCASTRATE },
	{ sub_6FD92E80, STAT_ITEM_POISONLENGTHRESIST },
	{ sub_6FD92E80, STAT_ITEM_NORMALDAMAGE },
	{ sub_6FD92E80, STAT_ITEM_HOWL },
	{ sub_6FD92E80, STAT_ITEM_STUPIDITY },
	{ sub_6FD92E80, STAT_ITEM_IGNORETARGETAC },
	{ sub_6FD92E80, STAT_ITEM_FRACTIONALTARGETAC },
	{ sub_6FD92E80, STAT_ITEM_PREVENTHEAL },
	{ sub_6FD92E80, STAT_ITEM_HALFFREEZEDURATION },
	{ sub_6FD93200, STAT_ITEM_TOHIT_PERCENT },
	{ sub_6FD92E80, STAT_ITEM_DAMAGETARGETAC },
	{ sub_6FD93200, STAT_ITEM_DEMONDAMAGE_PERCENT },
	{ sub_6FD93200, STAT_ITEM_UNDEADDAMAGE_PERCENT },
	{ sub_6FD92E80, STAT_ITEM_DEMON_TOHIT },
	{ sub_6FD92E80, STAT_ITEM_UNDEAD_TOHIT },
	{ sub_6FD92E80, STAT_ITEM_THROWABLE },
	{ sub_6FD94F70, STAT_ITEM_ELEMSKILL },
	{ sub_6FD94160, STAT_ITEM_ALLSKILLS },
	{ sub_6FD92E80, STAT_ITEM_ATTACKERTAKESLIGHTDAMAGE },
	{ sub_6FD92E80, STAT_ITEM_FREEZE },
	{ sub_6FD92E80, STAT_ITEM_OPENWOUNDS },
	{ sub_6FD92E80, STAT_ITEM_CRUSHINGBLOW },
	{ sub_6FD92E80, STAT_ITEM_KICKDAMAGE },
	{ sub_6FD92E80, STAT_ITEM_MANAAFTERKILL },
	{ sub_6FD92E80, STAT_ITEM_HEALAFTERDEMONKILL },
	{ sub_6FD92E80, STAT_ITEM_EXTRABLOOD },
	{ sub_6FD92E80, STAT_ITEM_DEADLYSTRIKE },
	{ sub_6FD92E80, STAT_ITEM_SLOW },
	{ sub_6FD92E80, STAT_ITEM_CANNOTBEFROZEN },
	{ sub_6FD92E80, STAT_ITEM_STAMINADRAINPCT },
	{ sub_6FD92E80, STAT_ITEM_REANIMATE },
	{ sub_6FD92E80, STAT_ITEM_PIERCE },
	{ sub_6FD92E80, STAT_ITEM_MAGICARROW },
	{ sub_6FD92E80, STAT_ITEM_EXPLOSIVEARROW },
	{ sub_6FD95050, STAT_ATTACK_VS_MONTYPE },
	{ sub_6FD95050, STAT_DAMAGE_VS_MONTYPE },
	{ sub_6FD94E80, STAT_ITEM_SINGLESKILL },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD93170, STAT_ITEM_NUMSOCKETS },
	{ sub_6FD944E0, STAT_FIREMINDAM },
	{ sub_6FD944E0, STAT_LIGHTMINDAM },
	{ sub_6FD944E0, STAT_MAGICMINDAM },
	{ sub_6FD94AB0, STAT_COLDMINDAM },
	{ sub_6FD94AB0, STAT_POISONMINDAM },
	{ sub_6FD944E0, STAT_ITEM_THROW_MINDAMAGE },
	{ sub_6FD944E0, STAT_MINDAMAGE },
	{ sub_6FD92A80, STAT_ITEM_ARMOR_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_ARMORPERCENT_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_HP_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_MANA_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_MAXDAMAGE_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_MAXDAMAGE_PERCENT_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_STRENGTH_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_DEXTERITY_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_ENERGY_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_VITALITY_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_TOHIT_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_TOHITPERCENT_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_COLD_DAMAGEMAX_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_FIRE_DAMAGEMAX_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_LTNG_DAMAGEMAX_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_POIS_DAMAGEMAX_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_RESIST_COLD_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_RESIST_FIRE_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_RESIST_LTNG_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_RESIST_POIS_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_ABSORB_COLD_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_ABSORB_FIRE_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_ABSORB_LTNG_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_ABSORB_POIS_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_THORNS_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_FIND_GOLD_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_FIND_MAGIC_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_REGENSTAMINA_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_STAMINA_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_DAMAGE_DEMON_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_DAMAGE_UNDEAD_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_TOHIT_DEMON_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_TOHIT_UNDEAD_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_CRUSHINGBLOW_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_OPENWOUNDS_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_KICK_DAMAGE_PERLEVEL },
	{ sub_6FD92A80, STAT_ITEM_DEADLYSTRIKE_PERLEVEL },
	{ sub_6FD95200, -1 },
	{ sub_6FD92A80, STAT_ITEM_REPLENISH_DURABILITY },
	{ sub_6FD92A80, STAT_ITEM_REPLENISH_QUANTITY },
	{ sub_6FD92E80, STAT_ITEM_EXTRA_STACK },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD943C0, STAT_ITEM_ARMOR_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_ARMORPERCENT_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_HP_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_MANA_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_MAXDAMAGE_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_MAXDAMAGE_PERCENT_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_STRENGTH_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_DEXTERITY_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_ENERGY_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_VITALITY_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_TOHIT_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_TOHITPERCENT_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_COLD_DAMAGEMAX_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_FIRE_DAMAGEMAX_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_LTNG_DAMAGEMAX_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_POIS_DAMAGEMAX_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_RESIST_COLD_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_RESIST_FIRE_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_RESIST_LTNG_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_RESIST_POIS_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_ABSORB_COLD_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_ABSORB_FIRE_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_ABSORB_LTNG_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_ABSORB_POIS_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_FIND_GOLD_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_FIND_MAGIC_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_REGENSTAMINA_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_STAMINA_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_DAMAGE_DEMON_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_DAMAGE_UNDEAD_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_TOHIT_DEMON_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_TOHIT_UNDEAD_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_CRUSHINGBLOW_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_OPENWOUNDS_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_KICK_DAMAGE_BYTIME },
	{ sub_6FD943C0, STAT_ITEM_DEADLYSTRIKE_BYTIME },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD95200, -1 },
	{ sub_6FD931C0, -1 },
	{ sub_6FD94190, STAT_ITEM_CHARGED_SKILL },
};


static const PROPERTYASSIGNFN off_6FDE3920[36] =
{
	NULL,
	ITEMMODS_PropertyFunc01,
	ITEMMODS_PropertyFunc02,
	ITEMMODS_PropertyFunc03,
	ITEMMODS_PropertyFunc04,
	ITEMMODS_PropertyFunc05,
	ITEMMODS_PropertyFunc06,
	ITEMMODS_PropertyFunc07,
	ITEMMODS_PropertyFunc08,
	ITEMMODS_PropertyFunc09,
	ITEMMODS_PropertyFunc10,
	ITEMMODS_PropertyFunc11,
	ITEMMODS_PropertyFunc12,
	ITEMMODS_PropertyFunc13,
	ITEMMODS_PropertyFunc14,
	ITEMMODS_PropertyFunc15,
	ITEMMODS_PropertyFunc16,
	ITEMMODS_PropertyFunc17,
	ITEMMODS_PropertyFunc18,
	ITEMMODS_PropertyFunc19,
	ITEMMODS_PropertyFunc20,
	ITEMMODS_PropertyFunc21,
	ITEMMODS_PropertyFunc22,
	ITEMMODS_PropertyFunc23,
	ITEMMODS_PropertyFunc24
};
static const int dword_6FDE39B0 = ARRAY_SIZE(off_6FDE3920);


//D2Common.0x6FD92640 (#10844)
void __stdcall D2Common_10844_ITEMMODS_First(int nDataBits, int* pLayer, int* pValue)
{
	*pLayer = nDataBits & 511;
	*pValue = (nDataBits >> 9) & 31;
}

//D2Common.0x6FD92670 (#10846)
void __stdcall D2Common_10846(int nDataBits, int* a2, int* a3, int* a4, int* a5)
{
	*a2 = nDataBits & 511;
	*a3 = (nDataBits >> 9) & 31;
	*a4 = (nDataBits >> 14) & 255;
	*a5 = (nDataBits >> 22) & 255;
}

//D2Common.0x6FD926C0 (#11293)
BOOL __stdcall ITEMMODS_GetItemCharges(D2UnitStrc* pItem, int nSkillId, int nSkillLevel, int* pValue, D2StatListStrc** ppStatList)
{
	D2StatListStrc* pStatList = NULL;
	int nValue = 0;

	if (pValue)
	{
		*pValue = 0;
	}

	if (ppStatList)
	{
		*ppStatList = NULL;
	}

	if (!pItem || pItem->dwUnitType != UNIT_ITEM || !DATATBLS_GetSkillsTxtRecord(nSkillId) || nSkillLevel <= 0 || nSkillLevel > 63)
	{
		return FALSE;
	}

	pStatList = STATLIST_GetStatListFromUnitAndFlag(pItem, 0x40);
	if (pStatList)
	{
		do
		{
			nValue = STATLIST_GetStatValue(pStatList, STAT_ITEM_CHARGED_SKILL, (nSkillLevel & sgptDataTables->nShiftedStuff) + (nSkillId << sgptDataTables->nStuff));
			if (nValue)
			{
				break;
			}

			pStatList = STATLIST_GetStatListFromFlag(pStatList, 0x40);
		}
		while (pStatList);

		if (pStatList)
		{
			if (pValue)
			{
				*pValue = (uint8_t)nValue;
			}

			if (ppStatList)
			{
				*ppStatList = pStatList;
			}

			return TRUE;
		}
	}

	if (pItem->pInventory)
	{
		for (D2UnitStrc* i = INVENTORY_GetFirstItem(pItem->pInventory); i; i = INVENTORY_GetNextItem(i))
		{
			if (ITEMMODS_GetItemCharges(INVENTORY_UnitIsItem(i), nSkillId, nSkillLevel, pValue, ppStatList))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD927D0 (#10847)
BOOL __stdcall ITEMMODS_UpdateItemWithSkillCharges(D2UnitStrc* pItem, int nSkillId, int nSkillLevel, int a4)
{
	D2StatListStrc* pStatList = NULL;
	D2UnitStrc* pSocketable = NULL;
	int nShiftedValue = 0;
	int nValue = 0;
	int nLayer = 0;
	uint8_t v11 = 0;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		nLayer = (nSkillLevel & sgptDataTables->nShiftedStuff) + (nSkillId << sgptDataTables->nStuff);

		pStatList = STATLIST_GetStatListFromUnitAndFlag(pItem, 0x40);
		while (pStatList)
		{
			nValue = STATLIST_GetStatValue(pStatList, STAT_ITEM_CHARGED_SKILL, nLayer);
			if (nValue)
			{
				break;
			}
			pStatList = STATLIST_GetStatListFromFlag(pStatList, 0x40);
		}

		if (pStatList)
		{
			nShiftedValue = nValue >> 8;
			if (nShiftedValue > 0 && nShiftedValue <= 255)
			{
				v11 = a4;

				if (v11 < 0)
				{
					v11 = 0;
				}

				if (v11 >= nShiftedValue)
				{
					v11 = nShiftedValue;
				}

				STATLIST_SetBaseStat2(pStatList, STAT_ITEM_CHARGED_SKILL, v11 + (nShiftedValue << 8), nLayer, pItem);
				return TRUE;
			}

			return FALSE;
		}

		if (pItem->pInventory)
		{
			pSocketable = INVENTORY_GetFirstItem(pItem->pInventory);
			while (pSocketable)
			{
				if (ITEMMODS_UpdateItemWithSkillCharges(INVENTORY_UnitIsItem(pSocketable), nSkillId, nSkillLevel, a4))
				{
					break;
				}

				pSocketable = INVENTORY_GetNextItem(pSocketable);
			}

			if (pSocketable)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD928D0 (#10843)
//TODO: Find names for variables
int __stdcall D2COMMON_10843_GetByTimeAdjustment(int nAmount, int nTimeOfDay, int nBaseTime, int* a4, int* a5, int* a6)
{
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;

	v6 = ((nAmount >> 12) & 1023) - 256;
	v7 = nAmount & 3;
	v8 = ((nAmount >> 2) & 1023) - 256;

	if (a4)
	{
		*a4 = v7;
	}

	if (a5)
	{
		*a5 = v8;
	}

	if (a6)
	{
		*a6 = v6;
	}

	v9 = nBaseTime - 90 * v7;
	if (v9 < 0)
	{
		v9 = -v9;
	}

	v10 = 15 * (v9 + 7) / 15;
	if (v10 > 0)
	{
		if (v10 >= 359)
		{
			return v6 - (v6 - v8) / 180;
		}
	}
	else
	{
		v10 = 0;
	}

	if (v10 > 180)
	{
		v10 = 360 - v10;
	}

	return v6 - v10 * (v6 - v8) / 180;
}

//D2Common.0x6FD929A0 (#10849)
int __stdcall D2Common_10849(int a1, int a2)
{
	return a1 + (a2 << 9);
}

//D2Common.0x6FD929B0 (#10845)
void __stdcall D2Common_10845(int nDataBits, int* a2, int* a3, int* a4)
{
	*a2 = nDataBits & 511;
	*a3 = (nDataBits >> 9) & 31;
	*a4 = (nDataBits >> 14) & 127;
}

//D2Common.0x6FD929E0 (#10850)
int __stdcall D2Common_10850(int a1, int a2, int a3)
{
	return a1 + ((a2 + 32 * a3) << 9);
}

//D2Common.0x6FD92A00 (#10848)
void __stdcall D2Common_10848(int nDataBits, int* pClass, int* pTab, int* pLevel)
{
	D2_ASSERT(pClass && pTab && pLevel);

	*pClass = (nDataBits & 31) / 3;
	*pTab = (nDataBits & 31) % 3;
	*pLevel = (nDataBits >> 5) & 31;
}

//D2Common.0x6FD92A60 (#10851)
int __stdcall D2Common_10851(int a1, int a2, int a3)
{
	return a2 + 3 * a1 + 32 * a3;
}

//D2Common.0x6FD92A80
BOOL __fastcall sub_6FD92A80(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2StatListStrc* pStatList = NULL;
	int nValue = 0;

	D2_ASSERT(pProperty);

	nValue = pProperty->nLayer;
	if (nValue)
	{
		if (nStatId >= STAT_ITEM_ARMOR_PERCENT && nStatId <= STAT_ITEM_MINDAMAGE_PERCENT || nType == 1)
		{
			sub_6FD92CF0(pItem, nStatId);
		}

		switch (nStatId)
		{
		case STAT_HITPOINTS:
		case STAT_MAXHP:
		case STAT_MANA:
		case STAT_MAXMANA:
		case STAT_STAMINA:
		case STAT_MAXSTAMINA:
		case STAT_ITEM_HP_PERLEVEL:
		case STAT_ITEM_MANA_PERLEVEL:
			nValue <<= 8;
			break;

		default:
			break;
		}

		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		D2_ASSERT(pStatList);

		if (nApplyType)
		{
			nValue = -STATLIST_GetStatValue(pStatList, nStatId, 0);
		}

		STATLIST_AddStat(pStatList, nStatId, nValue, 0);
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD92C40
D2StatListStrc* __fastcall ITEMMODS_GetOrCreateStatList(D2UnitStrc* pUnit, D2UnitStrc* pItem, int nState, int fFilter)
{
	D2StatListStrc* pStatList = NULL;
	void* pMemPool = NULL;
	int nItemGUID = 0;

	if (pUnit)
	{
		pStatList = STATLIST_GetStatListFromUnitStateOrFlag(pUnit, nState, fFilter);
		if (pStatList)
		{
			return pStatList;
		}

		pStatList = STATLIST_AllocStatList(pUnit->pMemoryPool, fFilter, 0, 4, pUnit->dwUnitId);
		D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
	}
	else
	{
		pStatList = STATLIST_GetStatListFromUnitStateOrFlag(pItem, nState, fFilter);
		if (pStatList)
		{
			return pStatList;
		}

		if (pItem)
		{
			nItemGUID = pItem->dwUnitId;
			pMemPool = pItem->pMemoryPool;
		}
		else
		{
			nItemGUID = -1;
			pMemPool = NULL;
		}

		pStatList = STATLIST_AllocStatList(pMemPool, fFilter, 0, 4, nItemGUID);

		D2COMMON_10475_PostStatToStatList(pItem, pStatList, 1);
	}

	STATLIST_SetState(pStatList, nState);

	D2_ASSERT(pStatList);

	return pStatList;
}

//D2Common.0x6FD92CF0
void __fastcall sub_6FD92CF0(D2UnitStrc* pItem, int nStatId)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nItemId = 0;
	int nValue = 0;

	pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(ITEMS_GetBaseCode(pItem), &nItemId);
	if (pItemsTxtRecord)
	{
		switch (nStatId)
		{
		case STAT_ITEM_ARMOR_PERCENT:
		case STAT_ARMORCLASS:
			if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_ARMOR) && pItemsTxtRecord->dwMaxAc)
			{
				nValue = STATLIST_GetUnitBaseStat(pItem, 31, 0) + 1;
				if (nValue <= pItemsTxtRecord->dwMaxAc)
				{
					nValue = pItemsTxtRecord->dwMaxAc + 1;
				}

				STATLIST_SetUnitStat(pItem, 31, nValue, 0);
			}
			break;

		case STAT_ITEM_MAXDAMAGE_PERCENT:
		case STAT_MAXDAMAGE:
			if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
			{
				if (pItemsTxtRecord->nMaxDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_MAXDAMAGE, pItemsTxtRecord->nMaxDam, 0);
				}

				if (pItemsTxtRecord->n2HandMaxDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MAXDAMAGE, pItemsTxtRecord->n2HandMaxDam, 0);
				}

				if (ITEMS_CheckIfThrowable(pItem) && pItemsTxtRecord->nMaxMisDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MAXDAMAGE, pItemsTxtRecord->nMaxMisDam, 0);
				}
			}
			break;

		case STAT_ITEM_MINDAMAGE_PERCENT:
		case STAT_MINDAMAGE:
			if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
			{
				if (pItemsTxtRecord->nMinDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_MINDAMAGE, pItemsTxtRecord->nMinDam, 0);
				}

				if (pItemsTxtRecord->n2HandMinDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MINDAMAGE, pItemsTxtRecord->n2HandMinDam, 0);
				}

				if (ITEMS_CheckIfThrowable(pItem) && pItemsTxtRecord->nMinMisDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MINDAMAGE, pItemsTxtRecord->nMinMisDam, 0);
				}
			}
			break;

		default:
			return;
		}
	}
}

//D2Common.0x6FD92E80
BOOL __fastcall sub_6FD92E80(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	return sub_6FD92EB0(nType, pUnit, pItem, pProperty, nStatId, nApplyType, 0, nState, fStatList, a9);
}

//D2Common.0x6FD92EB0
BOOL __fastcall sub_6FD92EB0(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int a7, int nState, int fStatList, D2UnitStrc* a9)
{
	D2StatListStrc* pStatList = NULL;
	int nPossibleRolls = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (a7)
	{
		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			nPossibleRolls = nMax - nMin;
			if (ITEMS_GetItemFormat(pItem) >= 1)
			{
				++nPossibleRolls;
			}

			nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
		}

		nValue = nMin;
		sub_6FD92CF0(pItem, nStatId);
	}
	else
	{
		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			nPossibleRolls = nMax - nMin;
			if (ITEMS_GetItemFormat(pItem) >= 1)
			{
				++nPossibleRolls;
			}

			nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
		}

		nValue = nMin;
		if (nType == 1)
		{
			sub_6FD92CF0(pItem, nStatId);
		}
	}

	if (nValue)
	{
		switch (nStatId)
		{
		case STAT_HITPOINTS:
		case STAT_MAXHP:
		case STAT_MANA:
		case STAT_MAXMANA:
		case STAT_STAMINA:
		case STAT_MAXSTAMINA:
		case STAT_ITEM_HP_PERLEVEL:
		case STAT_ITEM_MANA_PERLEVEL:
			nValue <<= 8;
			break;

		default:
			break;
		}

		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		D2_ASSERT(pStatList);

		if (nApplyType)
		{
			nValue = -STATLIST_GetStatValue(pStatList, nStatId, 0);
		}

		STATLIST_AddStat(pStatList, nStatId, nValue, 0);
		if (nStatId == STAT_POISONMAXDAM)
		{
			STATLIST_AddStat(pStatList, STAT_POISON_COUNT, 1, 0);
		}

		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD93170
BOOL __fastcall sub_6FD93170(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2_ASSERT(pProperty);

	ITEMS_SetItemFlag(pItem, IFLAG_SOCKETED, TRUE);
	ITEMS_SetSockets(pItem, pProperty->nLayer);

	return 1;
}

//D2Common.0x6FD931C0
BOOL __fastcall sub_6FD931C0(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	if (!a9 || a9->dwUnitType != UNIT_ITEM)
	{
		STATLIST_SetUnitStat(pItem, STAT_MAXDURABILITY, 0, 0);
		STATLIST_SetUnitStat(pItem, STAT_DURABILITY, 0, 0);
	}
	else
	{
		STATLIST_SetUnitStat(a9, STAT_MAXDURABILITY, 0, 0);
		STATLIST_SetUnitStat(a9, STAT_DURABILITY, 0, 0);
	}

	return 1;
}

//D2Common.0x6FD93200
BOOL __fastcall sub_6FD93200(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	return sub_6FD92EB0(nType, pUnit, pItem, pProperty, nStatId, nApplyType, 1, nState, fStatList, a9);
}

//D2Common.0x6FD93230
BOOL __fastcall sub_6FD93230(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	int nPossibleRolls = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	if (ITEMS_GetItemFormat(pItem) < 1)
	{
		sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_FIRERESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_LIGHTRESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_COLDRESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_POISONRESIST, nApplyType, 0, nState, fStatList, a9);
	}
	else
	{
		D2_ASSERT(pProperty);

		if (pProperty->nMax == pProperty->nMin)
		{
			nValue = pProperty->nMin;
		}
		else
		{
			nMin = pProperty->nMin;
			nMax = pProperty->nMax;

			if (nMax < nMin)
			{
				nMin = pProperty->nMax;
				nMax = pProperty->nMin;
			}

			if (ITEMS_GetItemFormat(pItem) < 1)
			{
				nPossibleRolls = nMax - nMin;
			}
			else
			{
				nPossibleRolls = nMax - nMin + 1;
			}

			nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);

			nValue = nMin;
		}

		sub_6FD93410(nType, pUnit, pItem, nValue, STAT_FIRERESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD93410(nType, pUnit, pItem, nValue, STAT_LIGHTRESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD93410(nType, pUnit, pItem, nValue, STAT_COLDRESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD93410(nType, pUnit, pItem, nValue, STAT_POISONRESIST, nApplyType, 0, nState, fStatList, a9);
	}

	return nStatId;
}

//D2Common.0x6FD93410
BOOL __fastcall sub_6FD93410(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, int a4, int nStatId, int nApplyType, int a7, int nState, int fStatList, D2UnitStrc* a10)
{
	D2StatListStrc* pStatList = NULL;
	int nValue = 0;

	if (a7 || nType == 1)
	{
		sub_6FD92CF0(pItem, nStatId);
	}

	if (a4)
	{
		nValue = a4;

		switch (nStatId)
		{
		case STAT_HITPOINTS:
		case STAT_MAXHP:
		case STAT_MANA:
		case STAT_MAXMANA:
		case STAT_STAMINA:
		case STAT_MAXSTAMINA:
		case STAT_ITEM_HP_PERLEVEL:
		case STAT_ITEM_MANA_PERLEVEL:
			nValue <<= 8;
			break;

		default:
			break;
		}

		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		D2_ASSERT(pStatList);

		if (nApplyType)
		{
			nValue = -STATLIST_GetStatValue(pStatList, nStatId, 0);
		}

		STATLIST_AddStat(pStatList, nStatId, nValue, 0);
		return TRUE;
	}
	
	return FALSE;
}

//D2Common.0x6FD935B0
BOOL __fastcall sub_6FD935B0(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	int nPossibleRolls = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	if (ITEMS_GetItemFormat(pItem) < 1)
	{
		sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_MAXFIRERESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_MAXLIGHTRESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_MAXCOLDRESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_MAXPOISONRESIST, nApplyType, 0, nState, fStatList, a9);
	}
	else
	{
		D2_ASSERT(pProperty);

		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMax == nMin)
		{
			nValue = nMin;
		}
		else
		{
			if (nMax < nMin)
			{
				nMin = pProperty->nMax;
				nMax = pProperty->nMin;
			}
			if (ITEMS_GetItemFormat(pItem) < 1)
			{
				nPossibleRolls = nMax - nMin;
			}
			else
			{
				nPossibleRolls = nMax - nMin + 1;
			}

			nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);

			nValue = nMin;
		}

		sub_6FD93410(nType, pUnit, pItem, nValue, STAT_MAXFIRERESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD93410(nType, pUnit, pItem, nValue, STAT_MAXLIGHTRESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD93410(nType, pUnit, pItem, nValue, STAT_MAXCOLDRESIST, nApplyType, 0, nState, fStatList, a9);
		sub_6FD93410(nType, pUnit, pItem, nValue, STAT_MAXPOISONRESIST, nApplyType, 0, nState, fStatList, a9);
	}

	return nStatId;
}

//D2Common.0x6FD93790
BOOL __fastcall sub_6FD93790(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2StatListStrc* pStatList = NULL;
	int nPossibleRolls = 0;
	int nClassId = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	D2_ASSERT(pProperty);

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMin != nMax)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		nPossibleRolls = nMax - nMin;
		if (ITEMS_GetItemFormat(pItem) >= 1)
		{
			++nPossibleRolls;
		}

		nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
	}

	if (pUnit && pUnit->dwUnitType == UNIT_ITEM)
	{
		nClassId = pUnit->dwClassId;
	}
	else
	{
		if (pItem)
		{
			nClassId = pItem->dwClassId;
		}
		else
		{
			nClassId = -1;
		}
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nClassId);

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || pItemsTxtRecord->nMinDam || !pItemsTxtRecord->n2HandMinDam)
	{
		nValue = nMin;
		if (nType == 1)
		{
			sub_6FD92CF0(pItem, STAT_MINDAMAGE);
		}

		if (nMin)
		{
			pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
			D2_ASSERT(pStatList);

			if (nApplyType)
			{
				nValue = -STATLIST_GetStatValue(pStatList, STAT_MINDAMAGE, 0);
			}

			STATLIST_AddStat(pStatList, STAT_MINDAMAGE, nValue, 0);
		}
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || pItemsTxtRecord->n2HandMinDam || !pItemsTxtRecord->nMinDam)
	{
		nValue = nMin;
		if (nType == 1)
		{
			sub_6FD92CF0(pItem, STAT_SECONDARY_MINDAMAGE);
		}

		if (nMin)
		{
			pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
			D2_ASSERT(pStatList);

			if (nApplyType)
			{
				nValue = -STATLIST_GetStatValue(pStatList, STAT_SECONDARY_MINDAMAGE, 0);
			}

			STATLIST_AddStat(pStatList, STAT_SECONDARY_MINDAMAGE, nValue, 0);
		}
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || ITEMS_CheckIfThrowable(pItem))
	{
		nValue = nMin;
		if (nType == 1)
		{
			sub_6FD92CF0(pItem, STAT_ITEM_THROW_MINDAMAGE);
		}

		if (nMin)
		{
			pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
			D2_ASSERT(pStatList);

			if (nApplyType)
			{
				nValue = -STATLIST_GetStatValue(pStatList, STAT_ITEM_THROW_MINDAMAGE, 0);
			}

			STATLIST_AddStat(pStatList, STAT_ITEM_THROW_MINDAMAGE, nValue, 0);
		}
	}

	return TRUE;
}

//D2Common.0x6FD93A20
BOOL __fastcall sub_6FD93A20(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2StatListStrc* pStatList = NULL;
	int nPossibleRolls = 0;
	int nClassId = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	D2_ASSERT(pProperty);

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMin != nMax)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		nPossibleRolls = nMax - nMin;
		if (ITEMS_GetItemFormat(pItem) >= 1)
		{
			++nPossibleRolls;
		}

		nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
	}

	if (pUnit && pUnit->dwUnitType == UNIT_ITEM)
	{
		nClassId = pUnit->dwClassId;
	}
	else
	{
		if (pItem)
		{
			nClassId = pItem->dwClassId;
		}
		else
		{
			nClassId = -1;
		}
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nClassId);

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || pItemsTxtRecord->nMaxDam || !pItemsTxtRecord->n2HandMaxDam)
	{
		nValue = nMin;
		if (nType == 1)
		{
			sub_6FD92CF0(pItem, STAT_MAXDAMAGE);
		}

		if (nMin)
		{
			pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
			D2_ASSERT(pStatList);

			if (nApplyType)
			{
				nValue = -STATLIST_GetStatValue(pStatList, STAT_MAXDAMAGE, 0);
			}

			STATLIST_AddStat(pStatList, STAT_MAXDAMAGE, nValue, 0);
		}
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || pItemsTxtRecord->n2HandMaxDam || !pItemsTxtRecord->nMinDam)
	{
		nValue = nMin;
		if (nType == 1)
		{
			sub_6FD92CF0(pItem, STAT_SECONDARY_MAXDAMAGE);
		}

		if (nMin)
		{
			pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
			D2_ASSERT(pStatList);

			if (nApplyType)
			{
				nValue = -STATLIST_GetStatValue(pStatList, STAT_SECONDARY_MAXDAMAGE, 0);
			}

			STATLIST_AddStat(pStatList, STAT_SECONDARY_MAXDAMAGE, nValue, 0);
		}
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || ITEMS_CheckIfThrowable(pItem))
	{
		nValue = nMin;
		if (nType == 1)
		{
			sub_6FD92CF0(pItem, STAT_ITEM_THROW_MAXDAMAGE);
		}

		if (nMin)
		{
			pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
			D2_ASSERT(pStatList);

			if (nApplyType)
			{
				nValue = -STATLIST_GetStatValue(pStatList, STAT_ITEM_THROW_MAXDAMAGE, 0);
			}

			STATLIST_AddStat(pStatList, STAT_ITEM_THROW_MAXDAMAGE, nValue, 0);
		}
	}

	return TRUE;
}

//D2Common.0x6FD93CB0
BOOL __fastcall sub_6FD93CB0(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2StatListStrc* pStatList = NULL;
	int nPossibleRolls = 0;
	int nMaxDam = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	if (ITEMS_GetItemFormat(pItem) < 1)
	{
		sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_ITEM_MAXDAMAGE_PERCENT, nApplyType, 1, nState, fStatList, a9);
		sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_ITEM_MINDAMAGE_PERCENT, nApplyType, 1, nState, fStatList, a9);
		return nStatId;
	}

	D2_ASSERT(pProperty);

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMax == nMin)
	{
		nValue = pProperty->nMin;
	}
	else
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		nPossibleRolls = nMax - nMin;
		if (ITEMS_GetItemFormat(pItem) >= 1)
		{
			++nPossibleRolls;
		}

		nValue = nMin + SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
	}

	if (pUnit && pUnit->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pUnit->dwClassId);
	}
	else
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
	}

	D2_ASSERT(pItemsTxtRecord);

	nMaxDam = pItemsTxtRecord->n2HandMaxDam;
	if (nMaxDam <= pItemsTxtRecord->nMaxDam)
	{
		nMaxDam = pItemsTxtRecord->nMaxDam;
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || nMaxDam + (nValue * nMaxDam) / 100 > nMaxDam)
	{
		sub_6FD93410(nType, pUnit, pItem, nValue, STAT_ITEM_MAXDAMAGE_PERCENT, nApplyType, 1, nState, fStatList, a9);
		sub_6FD93410(nType, pUnit, pItem, nValue, STAT_ITEM_MINDAMAGE_PERCENT, nApplyType, 1, nState, fStatList, a9);
		return nStatId;
	}

	if (pItemsTxtRecord->nMinDam)
	{
		if (nType == 1)
		{
			sub_6FD92CF0(pItem, STAT_MAXDAMAGE);
		}

		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		D2_ASSERT(pStatList);

		if (nApplyType)
		{
			STATLIST_AddStat(pStatList, STAT_MAXDAMAGE, -STATLIST_GetStatValue(pStatList, STAT_MAXDAMAGE, 0), 0);
		}
		else
		{
			STATLIST_AddStat(pStatList, STAT_MAXDAMAGE, 1, 0);
		}
	}

	if (pItemsTxtRecord->n2HandMinDam)
	{
		if (nType == 1)
		{
			sub_6FD92CF0(pItem, STAT_SECONDARY_MAXDAMAGE);
		}

		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		D2_ASSERT(pStatList);

		if (nApplyType)
		{
			STATLIST_AddStat(pStatList, STAT_SECONDARY_MAXDAMAGE, -STATLIST_GetStatValue(pStatList, STAT_SECONDARY_MAXDAMAGE, 0), 0);
		}
		else
		{
			STATLIST_AddStat(pStatList, STAT_SECONDARY_MAXDAMAGE, 1, 0);
		}
	}

	if (ITEMS_CheckIfThrowable(pItem))
	{
		if (nType == 1)
		{
			sub_6FD92CF0(pItem, STAT_ITEM_THROW_MAXDAMAGE);
		}

		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		D2_ASSERT(pStatList);

		if (nApplyType)
		{
			STATLIST_AddStat(pStatList, STAT_ITEM_THROW_MAXDAMAGE, -STATLIST_GetStatValue(pStatList, STAT_ITEM_THROW_MAXDAMAGE, 0), 0);
		}
		else
		{
			STATLIST_AddStat(pStatList, STAT_ITEM_THROW_MAXDAMAGE, 1, 0);
		}
	}

	return nStatId;
}

//D2Common.0x6FD94060
void __fastcall sub_6FD94060(int nStatId, int* pValue)
{
	switch (nStatId)
	{
	case STAT_HITPOINTS:
	case STAT_MAXHP:
	case STAT_MANA:
	case STAT_MAXMANA:
	case STAT_STAMINA:
	case STAT_MAXSTAMINA:
	case STAT_ITEM_HP_PERLEVEL:
	case STAT_ITEM_MANA_PERLEVEL:
		*pValue <<= 8;
		break;

	default:
		return;
	}
}

//D2Common.0x6FD94160
BOOL __fastcall sub_6FD94160(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	sub_6FD92EB0(nType, pUnit, pItem, pProperty, STAT_ITEM_ALLSKILLS, nApplyType, 0, nState, fStatList, a9);

	return nStatId;
}

//D2Common.0x6FD94190
BOOL __fastcall sub_6FD94190(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2StatListStrc* pStatList = NULL;
	unsigned int nRand = 0;
	int nRequiredSkillLevel = 0;
	int nMaxLevel = 0;
	int nSkillId = 0;
	int nLevel = 0;
	int nTemp = 0;
	int nMin = 0;

	if (!pProperty)
	{
		return 0;
	}
	
	nSkillId = pProperty->nLayer;
	if (!DATATBLS_GetSkillsTxtRecord(nSkillId))
	{
		return 0;
	}

	if (pProperty->nMax == 0)
	{
		nTemp = (ITEMS_GetItemLevel(pItem) - SKILLS_GetRequiredLevel(nSkillId)) / 4 + 1;

		nMaxLevel = sgptDataTables->pSkillsTxt[nSkillId].wMaxLvl;
		if (nMaxLevel <= 0)
		{
			nMaxLevel = 20;
		}

		if (nTemp <= 1)
		{
			nTemp = 1;
		}

		if (nTemp >= nMaxLevel)
		{
			nLevel = nMaxLevel;
		}
		else
		{
			nLevel = nTemp;
		}
	}
	else if (pProperty->nMax < 0)
	{
		nRequiredSkillLevel = SKILLS_GetRequiredLevel(nSkillId);

		nTemp = 99 - nRequiredSkillLevel;
		if (nTemp < 1)
		{
			nTemp = 1;
		}

		nTemp = -(nTemp / pProperty->nMax);
		if (nTemp < 1)
		{
			nTemp = 1;
		}

		nLevel = (ITEMS_GetItemLevel(pItem) - nRequiredSkillLevel) / nTemp;
		if (nLevel <= 0)
		{
			nLevel = 1;
		}
	}
	else
	{
		nLevel = pProperty->nMax;
	}

	nMin = pProperty->nMin;
	if (!nMin)
	{
		nTemp = 5;
	}
	else
	{
		if (nMin < 0)
		{
			nMin = nLevel * -nMin / 8 - nMin;
		}

		if (nMin > 1)
		{
			if (nMin >= 255)
			{
				nTemp = 255;
			}
			else
			{
				nTemp = nMin;
			}
		}
		else
		{
			nTemp = 1;
		}
	}

	nRand = SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nTemp - nTemp / 8);

	pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
	STATLIST_SetStatIfListIsValid(pStatList, nStatId, (nTemp << 8) + ((nRand + nTemp / 8 + 1) & 0xFF), (nLevel & ((uint16_t)sgptDataTables->nShiftedStuff)) + (nSkillId << sgptDataTables->nStuff));

	return 1;
}

//D2Common.0x6FD943C0
BOOL __fastcall sub_6FD943C0(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2StatListStrc* pStatList = NULL;
	int nPeriod = 0;
	int nMin = 0;
	int nMax = 0;

	D2_ASSERT(pProperty->nMin <= pProperty->nMax);

	nPeriod = pProperty->nLayer;
	nMin = pProperty->nMin + 256;
	nMax = pProperty->nMax + 256;

	pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
	D2_ASSERT(pStatList);

	D2_ASSERT(nPeriod >= 0 && nPeriod <= 3);

#define ITEM_BYTIME_VAL_BITS 10
	D2_ASSERT(nMin >= 0 && nMin < (1 << ITEM_BYTIME_VAL_BITS));
	D2_ASSERT(nMax >= 0 && nMax < (1 << ITEM_BYTIME_VAL_BITS));

	STATLIST_SetStatIfListIsValid(pStatList, nStatId, nPeriod + 4 * (nMin + (nMax << 10)), 0);

	return TRUE;
}

//D2Common.0x6FD944E0
BOOL __fastcall sub_6FD944E0(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2StatListStrc* pStatList = NULL;
	int nMaxStatId = 0;
	int nClassId = 0;
	int nTemp = 0;
	int nVal = 0;
	int nMin = 0;
	int nMax = 0;

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	nMaxStatId = nStatId + 1;

	switch (nStatId)
	{
	case STAT_HITPOINTS:
	case STAT_MAXHP:
	case STAT_MANA:
	case STAT_MAXMANA:
	case STAT_STAMINA:
	case STAT_MAXSTAMINA:
	case STAT_ITEM_HP_PERLEVEL:
	case STAT_ITEM_MANA_PERLEVEL:
		nMin <<= 8;
		break;

	default:
		break;
	}

	switch (nMaxStatId)
	{
	case STAT_HITPOINTS:
	case STAT_MAXHP:
	case STAT_MANA:
	case STAT_MAXMANA:
	case STAT_STAMINA:
	case STAT_MAXSTAMINA:
	case STAT_ITEM_HP_PERLEVEL:
	case STAT_ITEM_MANA_PERLEVEL:
		nMax <<= 8;
		break;

	default:
		break;
	}

	if (nStatId == STAT_MINDAMAGE)
	{
		if (pUnit && pUnit->dwUnitType == UNIT_ITEM)
		{
			pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pUnit->dwClassId);
		}
		else
		{
			if (pItem)
			{
				nClassId = pItem->dwClassId;
			}
			else
			{
				nClassId = -1;
			}

			pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nClassId);
		}

		if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || pItemsTxtRecord->nMaxDam || !pItemsTxtRecord->n2HandMaxDam)
		{
			nVal = nMin;
			if (nType == 1)
			{
				sub_6FD92CF0(pItem, STAT_MINDAMAGE);
			}

			if (nMin)
			{
				pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
				D2_ASSERT(pStatList);

				if (nApplyType)
				{
					nVal = -STATLIST_GetStatValue(pStatList, STAT_MINDAMAGE, 0);
				}

				STATLIST_AddStat(pStatList, STAT_MINDAMAGE, nVal, 0);
			}

			nVal = nMax;
			if (nType == 1)
			{
				sub_6FD92CF0(pItem, STAT_MAXDAMAGE);
			}

			if (nMax)
			{
				pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
				D2_ASSERT(pStatList);

				if (nApplyType)
				{
					nVal = -STATLIST_GetStatValue(pStatList, STAT_MAXDAMAGE, 0);
				}

				STATLIST_AddStat(pStatList, STAT_MAXDAMAGE, nVal, 0);
			}
		}

		if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || pItemsTxtRecord->n2HandMaxDam || !pItemsTxtRecord->nMinDam)
		{
			if (nType == 1)
			{
				sub_6FD92CF0(pItem, STAT_SECONDARY_MINDAMAGE);
			}

			nTemp = nMin;
			if (nMin)
			{
				sub_6FD94060(STAT_SECONDARY_MINDAMAGE, &nTemp);
				pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
				D2_ASSERT(pStatList);

				if (nApplyType)
				{
					nVal = -STATLIST_GetStatValue(pStatList, STAT_SECONDARY_MINDAMAGE, 0);
				}
				else
				{
					nVal = nTemp;
				}

				STATLIST_AddStat(pStatList, STAT_SECONDARY_MINDAMAGE, nVal, 0);
			}

			if (nType == 1)
			{
				sub_6FD92CF0(pItem, STAT_SECONDARY_MAXDAMAGE);
			}

			nTemp = nMax;
			if (nMax)
			{
				sub_6FD94060(STAT_SECONDARY_MAXDAMAGE, &nTemp);
				pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
				D2_ASSERT(pStatList);

				if (nApplyType)
				{
					nVal = -STATLIST_GetStatValue(pStatList, STAT_SECONDARY_MAXDAMAGE, 0);
				}
				else
				{
					nVal = nTemp;
				}

				STATLIST_AddStat(pStatList, STAT_SECONDARY_MAXDAMAGE, nVal, 0);
			}
		}

		if (ITEMS_CheckIfThrowable(pItem))
		{
			sub_6FD93410(nType, pUnit, pItem, nMin, STAT_ITEM_THROW_MINDAMAGE, nApplyType, 0, nState, fStatList, a9);
			sub_6FD93410(nType, pUnit, pItem, nMax, STAT_ITEM_THROW_MAXDAMAGE, nApplyType, 0, nState, fStatList, a9);
			return 1;
		}
	}
	else
	{
		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		D2_ASSERT(pStatList);

		if (nApplyType)
		{
			nMin = -STATLIST_GetStatValue(pStatList, nStatId, 0);
			nMax = -STATLIST_GetStatValue(pStatList, nMaxStatId, 0);
		}

		STATLIST_AddStat(pStatList, nStatId, nMin, 0);
		STATLIST_AddStat(pStatList, nMaxStatId, nMax, 0);

		if (nStatId == STAT_POISONMINDAM)
		{
			STATLIST_AddStat(pStatList, STAT_POISON_COUNT, 1, 0);
		}
	}

	return 1;
}

//D2Common.0x6FD94AB0
BOOL __fastcall sub_6FD94AB0(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2StatListStrc* pStatList = NULL;
	int nLayer = 0;
	int nMin = 0;
	int nMax = 0;

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;
	nLayer = pProperty->nLayer;

	switch (nStatId)
	{
	case STAT_HITPOINTS:
	case STAT_MAXHP:
	case STAT_MANA:
	case STAT_MAXMANA:
	case STAT_STAMINA:
	case STAT_MAXSTAMINA:
	case STAT_ITEM_HP_PERLEVEL:
	case STAT_ITEM_MANA_PERLEVEL:
		nMin <<= 8;
		break;

	default:
		break;
	}

	switch (nStatId)
	{
	case STAT_SKILLPTS:
	case STAT_HITPOINTS:
	case STAT_MAXHP:
	case STAT_MANA:
	case STAT_MAXMANA:
	case STAT_STAMINA:
	case STAT_ITEM_ARMORPERCENT_PERLEVEL:
	case STAT_ITEM_HP_PERLEVEL:
		nMax <<= 8;
		break;

	default:
		break;
	}

	switch (nStatId)
	{
	case STAT_STATPTS:
	case STAT_SKILLPTS:
	case STAT_HITPOINTS:
	case STAT_MAXHP:
	case STAT_MANA:
	case STAT_MAXMANA:
	case STAT_ITEM_ARMOR_PERLEVEL:
	case STAT_ITEM_ARMORPERCENT_PERLEVEL:
		nLayer <<= 8;
		break;

	default:
		break;
	}

	pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
	D2_ASSERT(pStatList);

	if (nApplyType)
	{
		nMin = -STATLIST_GetStatValue(pStatList, nStatId, 0);
		nMax = -STATLIST_GetStatValue(pStatList, nStatId + 1, 0);
		nLayer = -STATLIST_GetStatValue(pStatList, nStatId + 2, 0);
	}

	STATLIST_AddStat(pStatList, nStatId, nMin, 0);
	STATLIST_AddStat(pStatList, nStatId + 1, nMax, 0);
	STATLIST_AddStat(pStatList, nStatId + 2, nLayer, 0);

	if (nStatId == STAT_POISONMINDAM)
	{
		STATLIST_AddStat(pStatList, STAT_POISON_COUNT, 1, 0);
	}

	return 1;
}

//D2Common.0x6FD94E80
BOOL __fastcall sub_6FD94E80(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2StatListStrc* pStatList = NULL;
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return FALSE;
	}

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMin != nMax)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		nPossibleRolls = nMax - nMin;
		if (ITEMS_GetItemFormat(pItem) >= 1)
		{
			++nPossibleRolls;
		}

		nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
	}

	if (DATATBLS_GetSkillsTxtRecord(pProperty->nLayer))
	{
		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		if (pStatList)
		{
			if (nApplyType)
			{
				nMin = -STATLIST_GetStatValue(pStatList, STAT_ITEM_SINGLESKILL, pProperty->nLayer);
			}

			STATLIST_AddStat(pStatList, STAT_ITEM_SINGLESKILL, nMin, pProperty->nLayer);
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD94F70
BOOL __fastcall sub_6FD94F70(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2StatListStrc* pStatList = NULL;
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMin != nMax)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		nPossibleRolls = nMax - nMin;
		if (ITEMS_GetItemFormat(pItem) >= 1)
		{
			++nPossibleRolls;
		}

		nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
	}

	if (nMin)
	{
		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		if (pStatList)
		{
			if (nApplyType)
			{
				nMin = -STATLIST_GetStatValue(pStatList, STAT_ITEM_ELEMSKILL, 1);
			}

			STATLIST_AddStat(pStatList, STAT_ITEM_ELEMSKILL, nMin, 1);
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD95050
BOOL __fastcall sub_6FD95050(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2StatListStrc* pStatList = NULL;
	int nPossibleRolls = 0;
	int nLayer = 0;
	int nMin = 0;
	int nMax = 0;

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMin != nMax)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		nPossibleRolls = nMax - nMin;
		if (ITEMS_GetItemFormat(pItem) >= 1)
		{
			++nPossibleRolls;
		}

		nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
	}

	if (nMin)
	{
		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		if (pStatList)
		{
			switch (nStatId)
			{
			case STAT_ITEM_ADDCLASSSKILLS:
				nLayer = 0;
				break;

			case STAT_UNSENTPARAM1:
				nLayer = 1;
				break;

			case STAT_ITEM_ADDEXPERIENCE:
				nLayer = 2;
				break;

			case STAT_ITEM_HEALAFTERKILL:
				nLayer = 3;
				break;

			case STAT_ITEM_REDUCEDPRICES:
				nLayer = 4;
				break;

			case STAT_ATTACK_VS_MONTYPE:
				nLayer = 5;
				break;

			case STAT_DAMAGE_VS_MONTYPE:
				nLayer = 6;
				break;

			default:
				break;
			}

			if (nApplyType)
			{
				nMin = -STATLIST_GetStatValue(pStatList, STAT_ITEM_ADDCLASSSKILLS, nLayer);
			}

			STATLIST_AddStat(pStatList, STAT_ITEM_ADDCLASSSKILLS, nMin, nLayer);
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD95200
BOOL __fastcall sub_6FD95200(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	return 1;
}

//D2Common.0x6FD95210
BOOL __fastcall sub_6FD95210(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nStatId, int nApplyType, int nState, int fStatList, D2UnitStrc* a9)
{
	D2StatListStrc* pStatList = NULL;
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMin != nMax)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		nPossibleRolls = nMax - nMin;
		if (ITEMS_GetItemFormat(pItem) >= 1)
		{
			++nPossibleRolls;
		}

		nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
	}

	if (nType == 1)
	{
		sub_6FD92CF0(pItem, nStatId);
	}

	if (nMin)
	{
		switch (nStatId)
		{
		case STAT_HITPOINTS:
		case STAT_MAXHP:
		case STAT_MANA:
		case STAT_MAXMANA:
		case STAT_STAMINA:
		case STAT_MAXSTAMINA:
		case STAT_ITEM_HP_PERLEVEL:
		case STAT_ITEM_MANA_PERLEVEL:
			nMin <<= 8;
			break;

		default:
			break;
		}

		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		D2_ASSERT(pStatList);

		if (nApplyType)
		{
			nMin = -STATLIST_GetStatValue(pStatList, nStatId, 0);
		}

		STATLIST_AddStat(pStatList, nStatId, nMin, 0);
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD95430 (#10855)
void __stdcall ITEMMODS_AssignProperty(int nType, D2UnitStrc* a2, D2UnitStrc* pItem, void* pMods, int nPropSet, int nApplyType)
{
	static const int dword_6FDD146C[] =
	{
		STATLIST_MAGIC,
		STATLIST_MAGIC,
		STATLIST_MAGIC | STATLIST_SET,
		STATLIST_MAGIC | STATLIST_SET,
		STATLIST_MAGIC | STATLIST_SET,
		STATLIST_MAGIC | STATLIST_SET,
		STATLIST_MAGIC | STATLIST_SET,
	};

	static const int dword_6FDD1488[] =
	{
		0,
		0,
		STATE_ITEMSET1,
		STATE_ITEMSET2,
		STATE_ITEMSET3,
		STATE_ITEMSET4,
		STATE_ITEMSET5, 
	};

	D2UniqueItemsTxt* pUniqueItemsTxtRecord = NULL;
	D2SetItemsTxt* pSetItemsTxtRecord = NULL;
	D2PropertyStrc* pProperties[3] = {};
	D2PropertyStrc* pProperty = NULL;
	D2PropertyStrc* pModsa = NULL;
	int nProperties = 0;
	int nFileIndex = 0;
	int nState = 0;
	int nFlag = 0;

	switch (nType)
	{
	case PROPMODE_AFFIX:
	case PROPMODE_GEM:
	case PROPMODE_RUNE:
	{
		nProperties = 3;
		break;
	}

	case PROPMODE_GRADE:
	{
		nProperties = 2;
		break;
	}

	case PROPMODE_UNIQUE:
	{
		nFileIndex = ITEMS_GetFileIndex(pItem);

		if (nFileIndex >= 0 && nFileIndex < sgptDataTables->nUniqueItemsTxtRecordCount)
		{
			pUniqueItemsTxtRecord = &sgptDataTables->pUniqueItemsTxt[nFileIndex];
			if (pUniqueItemsTxtRecord)
			{
				for (int i = 0; i < ARRAY_SIZE(pUniqueItemsTxtRecord->pProperties); ++i)
				{
					sub_6FD95810(3, NULL, pItem, NULL, 0, 0, nApplyType, &pUniqueItemsTxtRecord->pProperties[i], 0, 0x40, 0);
				}
			}
		}

		return;
	}

	case PROPMODE_SET:
	{
		nFileIndex = ITEMS_GetFileIndex(pItem);
		if (nFileIndex >= 0 && nFileIndex < sgptDataTables->nSetItemsTxtRecordCount)
		{
			pSetItemsTxtRecord = &sgptDataTables->pSetItemsTxt[nFileIndex];
			if (pSetItemsTxtRecord)
			{
				if (ITEMS_GetItemFormat(pItem))
				{
					for (int i = 0; i < ARRAY_SIZE(pSetItemsTxtRecord->pProperties); ++i)
					{
						sub_6FD95810(4, NULL, pItem, NULL, 0, 0, nApplyType, &pSetItemsTxtRecord->pProperties[i], 0, 0x40, 0);
					}

					for (int i = 0; i < ARRAY_SIZE(pSetItemsTxtRecord->pPartialBoni); ++i)
					{
						if (pSetItemsTxtRecord->nAddFunc)
						{
							nFlag = dword_6FDD146C[i / 2 + 2];
							nState = dword_6FDD1488[i / 2 + 2];
						}
						else
						{
							nState = 0;
							nFlag = 0x40;
						}

						sub_6FD95810(4, NULL, pItem, NULL, 0, 0, nApplyType, &pSetItemsTxtRecord->pPartialBoni[i], nState, nFlag, 0);
					}
				}
				else
				{
					for (int i = 0; i < 2; ++i)
					{
						sub_6FD95810(4, NULL, pItem, NULL, 0, 0, nApplyType, &pSetItemsTxtRecord->pProperties[i], 0, 0x40, 0);
					}
				}
			}
		}

		return;
	}

	default:
		break;
	}

	for (int nMod = 0; nMod < nProperties; ++nMod)
	{
		switch (nType)
		{
		case PROPMODE_UNIQUE:
		case PROPMODE_SET:
		{
			return;
		}

		case PROPMODE_AFFIX:
		{
			D2_ASSERT(pMods);
#define MAX_MAGICITEM_PROPERTIES 3
			D2_ASSERT(nMod < MAX_MAGICITEM_PROPERTIES && nMod >= 0);

			pProperty = &((D2MagicAffixTxt*)pMods)->pProperties[nMod];
			break;
		}

		case PROPMODE_GRADE:
		{
#define MAX_UNIQUE_ITEM_PROPERTIES 12
			D2_ASSERT(nMod < MAX_UNIQUE_ITEM_PROPERTIES && nMod >= 0);
			return;
		}

		case PROPMODE_GEM:
		case PROPMODE_RUNE:
		{
			pProperties[0] = ((D2GemsTxt*)pMods)->pProperties[0];
			pProperties[1] = ((D2GemsTxt*)pMods)->pProperties[1];
			pProperties[2] = ((D2GemsTxt*)pMods)->pProperties[2];

			const int nNumGemProperties = 3;
			D2_ASSERT((unsigned int)nPropSet < nNumGemProperties);
#define MAX_GEMITEM_PROPERTIES 3
			D2_ASSERT(nMod < MAX_GEMITEM_PROPERTIES && nMod >= 0);

			pProperty = &pProperties[nPropSet][nMod];
			break;
		}

		case PROPMODE_RUNEWORD:
		case PROPMODE_UNUSED:
		{
			pProperty = (D2PropertyStrc*)pMods;
			break;
		}

		default:
			D2_UNREACHABLE;
			return;
		}

		if (!pProperty || pProperty->nProperty < 0)
		{
			break;
		}

		sub_6FD95810(nType, NULL, pItem, pMods, nMod, nPropSet, nApplyType, pProperty, 0, 0x40, a2);
	}
}

//D2Common.0x6FD95810
void __fastcall sub_6FD95810(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, void* pMods, int nIndex, int nPropSet, int nApplyType, D2PropertyStrc* pProperty, int nState, int fStatlist, D2UnitStrc* a11)
{
	if (ITEMS_GetItemFormat(pItem) < 1)
	{
		D2_ASSERT(pProperty);

		if (pProperty->nProperty >= 0 && pProperty->nProperty < sgptDataTables->nPropertiesTxtRecordCount)
		{
			if (stru_6FDE3160[pProperty->nProperty].pfAssign)
			{
				stru_6FDE3160[pProperty->nProperty].pfAssign(nType, pUnit, pItem, pProperty, stru_6FDE3160[pProperty->nProperty].nStatId, nApplyType, nState, fStatlist, a11);
			}
		}
	}
	else
	{
		D2COMMON_11292_ItemAssignProperty(nType, pUnit, pItem, pMods, nIndex, nPropSet, pProperty, nState, fStatlist, a11);
	}
}

//D2Common.0x6FD958D0 (#10865)
void __stdcall ITEMMODS_ApplyEthereality(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);

	ITEMS_SetItemFlag(pItem, IFLAG_ETHEREAL, TRUE);

	if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
	{
		STATLIST_SetUnitStat(pItem, STAT_MINDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_MINDAMAGE, 0) / 2, 0);
		STATLIST_SetUnitStat(pItem, STAT_MAXDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_MAXDAMAGE, 0) / 2, 0);
		STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MINDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_SECONDARY_MINDAMAGE, 0) / 2, 0);
		STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MAXDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_SECONDARY_MAXDAMAGE, 0) / 2, 0);
		STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MINDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_ITEM_THROW_MINDAMAGE, 0) / 2, 0);
		STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MAXDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_ITEM_THROW_MAXDAMAGE, 0) / 2, 0);
	}
	else
	{
		STATLIST_SetUnitStat(pItem, STAT_ARMORCLASS, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_ARMORCLASS, 0) / 2, 0);
	}
}

//D2Common.0x6FD959F0 (#10867)
BOOL __stdcall ITEMMODS_UpdateRuneword(D2UnitStrc* pUnit, D2UnitStrc* pItem, int nUnused)
{
	const D2PropertyStrc* pProperty = NULL;

	const D2RunesTxt* pRunesTxtRecord = ITEMS_GetRunesTxtRecordFromItem(pUnit);

	if (pRunesTxtRecord && !STATLIST_GetStatListFromUnitStateOrFlag(pUnit, STATE_RUNEWORD, STATLIST_MAGIC))
	{
		ITEMS_SetItemFlag(pUnit, IFLAG_RUNEWORD, TRUE);

		for (int i = 0; i < ARRAY_SIZE(pRunesTxtRecord->pProperties); ++i)
		{
			pProperty = &pRunesTxtRecord->pProperties[i];

			if (pProperty->nProperty < 0)
			{
				break;
			}

			D2COMMON_11292_ItemAssignProperty(PROPMODE_RUNEWORD, pUnit, pItem, NULL, i, 0, pProperty, STATE_RUNEWORD, STATLIST_MAGIC, NULL);
		}

		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD95A70
void __fastcall ITEMMODS_UpdateFullSetBoni(D2UnitStrc* pUnit, D2UnitStrc* pItem, int nState)
{
	static const int dword_6FDD14A8[] =
	{
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	};

	D2SetItemsTxt* pSetItemsTxtRecord = NULL;
	D2SetsTxt* pSetsTxtRecord = NULL;
	D2PropertyStrc* pProperty = NULL;
	int nSetItemsMask = 0;
	int nFileIndex = 0;
	int nItems = 0;

	if (nState && pItem && pItem->dwUnitType == UNIT_ITEM && ITEMS_GetItemQuality(pItem) == ITEMQUAL_SET)
	{
		nFileIndex = ITEMS_GetFileIndex(pItem);
		if (nFileIndex >= 0 && nFileIndex < sgptDataTables->nSetItemsTxtRecordCount)
		{
			pSetItemsTxtRecord = &sgptDataTables->pSetItemsTxt[nFileIndex];
			if (pSetItemsTxtRecord && pSetItemsTxtRecord->nSetId >= 0 && pSetItemsTxtRecord->nSetId < sgptDataTables->nSetsTxtRecordCount)
			{
				pSetsTxtRecord = &sgptDataTables->pSetsTxt[pSetItemsTxtRecord->nSetId];
				if (pSetsTxtRecord)
				{
					nSetItemsMask = ITEMS_GetSetItemsMask(pUnit, pItem, 1);
					if (nSetItemsMask < 0x40)
					{
						nItems = dword_6FDD14A8[nSetItemsMask];
					}
					else
					{
						nItems = 0;
					}

					if (nItems >= pSetsTxtRecord->nSetItems - 1)
					{
						nItems = pSetsTxtRecord->nSetItems - 1;
					}

					for (int i = 0; i < 2 * (nItems - 1); ++i)
					{
						pProperty = &pSetsTxtRecord->pBoni2[i];

						if (pProperty->nProperty >= 0)
						{
							sub_6FD95810(PROPMODE_SET, pUnit, pItem, 0, i, 0, 0, pProperty, nState, 0, 0);
						}
					}

					if (nItems >= pSetsTxtRecord->nSetItems)
					{
						for (int i = 0; i < ARRAY_SIZE(pSetsTxtRecord->pFBoni); ++i)
						{
							pProperty = &pSetsTxtRecord->pFBoni[i];

							if (pProperty->nProperty < 0)
							{
								break;
							}

							sub_6FD95810(PROPMODE_SET, pUnit, pItem, 0, i, 0, 0, pProperty, nState, 0, 0);
						}
					}
				}
			}
		}
	}
}

//D2Common.0x6FD95BE0 (#10859)
BOOL __stdcall ITEMMODS_CanItemHaveMagicAffix(D2UnitStrc* pItem, D2MagicAffixTxt* pMagicAffixTxtRecord)
{
	D2PropertiesTxt* pPropertiesTxtRecord = NULL;

	if (ITEMS_GetItemFormat(pItem) >= 100 || !ITEMS_CheckIfStackable(pItem) && !ITEMS_CheckIfThrowable(pItem))
	{
		if (ITEMS_CheckIfSocketable(pItem) && ITEMS_GetMaxSockets(pItem) || pMagicAffixTxtRecord->pProperties[0].nProperty < 0 || pMagicAffixTxtRecord->pProperties[0].nProperty < sgptDataTables->nPropertiesTxtRecordCount)
		{
			pPropertiesTxtRecord = &sgptDataTables->pPropertiesTxt[pMagicAffixTxtRecord->pProperties[0].nProperty];
			if (pPropertiesTxtRecord && pPropertiesTxtRecord->wStat[0] != STAT_ITEM_NUMSOCKETS)
			{
				for (int i = 0; i < ARRAY_SIZE(pMagicAffixTxtRecord->wEType); ++i)
				{
					if (pMagicAffixTxtRecord->wEType[i] <= 0)
					{
						break;
					}

					if (ITEMS_CheckItemTypeId(pItem, pMagicAffixTxtRecord->wEType[i]))
					{
						return FALSE;
					}
				}

				for (int i = 0; i < ARRAY_SIZE(pMagicAffixTxtRecord->wIType); ++i)
				{
					if (pMagicAffixTxtRecord->wIType[i] <= 0)
					{
						break;
					}

					if (ITEMS_CheckItemTypeId(pItem, pMagicAffixTxtRecord->wIType[i]))
					{
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD95CC0 (#10860)
BOOL __stdcall ITEMMODS_CanItemHaveRareAffix(D2UnitStrc* pItem, D2RareAffixTxt* pRareAffixTxtRecord)
{
	if ((ITEMS_GetItemFormat(pItem) >= 100 || !ITEMS_CheckIfStackable(pItem) && !ITEMS_CheckIfThrowable(pItem)) && (pRareAffixTxtRecord->wVersion < 100 || ITEMS_GetItemFormat(pItem) >= 100))
	{
		for (int i = 0; i < ARRAY_SIZE(pRareAffixTxtRecord->wEType); ++i)
		{
			if (pRareAffixTxtRecord->wEType[i] <= 0)
			{
				break;
			}

			if (ITEMS_CheckItemTypeId(pItem, pRareAffixTxtRecord->wEType[i]))
			{
				return FALSE;
			}
		}

		for (int i = 0; i < ARRAY_SIZE(pRareAffixTxtRecord->wIType); ++i)
		{
			if (pRareAffixTxtRecord->wIType[i] <= 0)
			{
				break;
			}

			if (ITEMS_CheckItemTypeId(pItem, pRareAffixTxtRecord->wIType[i]))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD95D60 (#10861)
BOOL __stdcall ITEMMODS_CanItemBeHighQuality(D2UnitStrc* pItem, D2QualityItemsTxt* pQualityItemsTxtRecord)
{
	D2ItemTypeCheckStrc pItemTypeCheck[9] =
	{
		{ pQualityItemsTxtRecord->nShield, ITEMTYPE_SHIELD },
		{ pQualityItemsTxtRecord->nScepter, ITEMTYPE_SCEPTER },
		{ pQualityItemsTxtRecord->nWand, ITEMTYPE_WAND },
		{ pQualityItemsTxtRecord->nStaff, ITEMTYPE_STAFF },
		{ pQualityItemsTxtRecord->nBow, ITEMTYPE_BOW },
		{ pQualityItemsTxtRecord->nBow, ITEMTYPE_CROSSBOW },
		{ pQualityItemsTxtRecord->nBoots, ITEMTYPE_BOOTS },
		{ pQualityItemsTxtRecord->nGloves, ITEMTYPE_GLOVES },
		{ pQualityItemsTxtRecord->nBelt, ITEMTYPE_BELT },
	};
	
	int nItemType = ITEMS_GetItemType(pItem);

	if (!pQualityItemsTxtRecord->nWeapon || !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || nItemType == ITEMTYPE_STAFF || nItemType == ITEMTYPE_BOW || nItemType == ITEMTYPE_CROSSBOW || nItemType == ITEMTYPE_SCEPTER || nItemType == ITEMTYPE_WAND)
	{
		if (!pQualityItemsTxtRecord->nArmor || !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_ARMOR) || nItemType == ITEMTYPE_SHIELD || nItemType == ITEMTYPE_BOOTS || nItemType == ITEMTYPE_GLOVES || nItemType == ITEMTYPE_BELT)
		{
			for (int i = 0; i < ARRAY_SIZE(pItemTypeCheck); ++i)
			{
				if (pItemTypeCheck[i].bItemType && nItemType == pItemTypeCheck[i].nItemType)
				{
					return TRUE;
				}
			}

			return FALSE;
		}
	}

	return TRUE;
}

//D2Common.0x6FD95E90 (#10862)
void __stdcall ITEMMODS_SetRandomElixirFileIndex(D2UnitStrc* pItem)
{
	static const uint32_t dwFileIndex[] =
	{
		0, 1, 2, 3, 9, 7
	};

	unsigned int nRand = SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), ARRAY_SIZE(dwFileIndex));

	ITEMS_SetFileIndex(pItem, dwFileIndex[nRand]);

	if (dwFileIndex[nRand] == 9)
	{
		STATLIST_SetUnitStat(pItem, STAT_VALUE, ((SEED_RollRandomNumber(ITEMS_GetItemSeed(pItem)) & 3) + 1) << 8, 0);
	}
	else if (dwFileIndex[nRand] == 7)
	{
		STATLIST_SetUnitStat(pItem, STAT_VALUE, ((SEED_RollRandomNumber(ITEMS_GetItemSeed(pItem)) & 3) + 1) << 8, 0);
	}
	else
	{
		STATLIST_SetUnitStat(pItem, STAT_VALUE, 1, 0);
	}
}

//D2Common.0x6FD95F90 (#10868)
void __stdcall ITEMMODS_AddCraftPropertyList(D2UnitStrc* pItem, D2PropertyStrc* pProperty, int nUnused)
{
	if (pItem)
	{
		sub_6FD95810(PROPMODE_UNUSED, NULL, pItem, pProperty, 0, 0, 0, pProperty, 0, STATLIST_MAGIC, NULL);
	}
}

//D2Common.0x6FD95FC0
int __fastcall ITEMMODS_PropertyFunc01(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	if (pItem && pItem->dwUnitType == UNIT_ITEM && nType == 1)
	{
		sub_6FD92CF0(pItem, nStatId);
	}

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMax != nMin)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		if (pItem && pItem->dwUnitType == UNIT_ITEM)
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
			}
		}
		else
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
			}
		}
	}

	return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, 0, nMin, nState, fStatList, a11);
}

//D2Common.0x6FD96110
int __fastcall ITEMMODS_AddPropertyToItemStatList(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* pUnused)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatListStrc* pStatList = NULL;

	if (!pProperty || !nValue)
	{
		return 0;
	}

	pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}

	pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
	if (pStatList)
	{
		if (nSet)
		{
			STATLIST_SetStatIfListIsValid(pStatList, nStatId, (nValue << pItemStatCostTxtRecord->nValShift), nLayer);

			if (nStatId == STAT_POISONMAXDAM && !STATLIST_GetStatValue(pStatList, STAT_POISON_COUNT, 0))
			{
				STATLIST_SetStatIfListIsValid(pStatList, STAT_POISON_COUNT, 1, 0);
			}
		}
		else
		{
			STATLIST_AddStat(pStatList, nStatId, (nValue << pItemStatCostTxtRecord->nValShift), nLayer);

			if (nStatId == STAT_POISONMAXDAM)
			{
				STATLIST_AddStat(pStatList, STAT_POISON_COUNT, 1, 0);
			}
		}

		return nValue;
	}

	return 0;
}

//D2Common.0x6FD96210
int __fastcall ITEMMODS_PropertyFunc02(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		sub_6FD92CF0(pItem, nStatId);
	}

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMax != nMin)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		if (pItem && pItem->dwUnitType == UNIT_ITEM)
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
			}
		}
		else
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
			}
		}
	}

	return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, 0, nMin, nState, fStatList, a11);
}

//D2Common.0x6FD96350
int __fastcall ITEMMODS_PropertyFunc03(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	if (pItem && pItem->dwUnitType == UNIT_ITEM && nType == 1)
	{
		sub_6FD92CF0(pItem, nStatId);
	}

	nMin = nValue;
	if (!nValue)
	{
		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			if (pItem && pItem->dwUnitType == UNIT_ITEM)
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
				}
			}
			else
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
				}
			}
		}
	}

	return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, 0, nMin, nState, fStatList, a11);
}

//D2Common.0x6FD964A0
int __fastcall ITEMMODS_PropertyFunc04(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		sub_6FD92CF0(pItem, nStatId);
	}

	nMin = nValue;
	if (!nValue)
	{
		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			if (pItem && pItem->dwUnitType == UNIT_ITEM)
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
				}
			}
			else
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
				}
			}
		}
	}

	return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, 0, nMin, nState, fStatList, a11);
}

//D2Common.0x6FD965F0
int __fastcall ITEMMODS_PropertyFunc05(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nPossibleRolls = 0;
	int nStatValue = 0;
	int nClassId = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	if (!pItem || pItem->dwUnitType != UNIT_ITEM)
	{
		return 0;
	}

	if (pUnit && pUnit->dwUnitType == UNIT_ITEM)
	{
		nClassId = pUnit->dwClassId;
	}
	else
	{
		nClassId = pItem->dwClassId;
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nClassId);
	if (!pItemsTxtRecord)
	{
		return 0;
	}

	nMin = nValue;
	if (!nValue)
	{
		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			if (pItem->dwUnitType == UNIT_ITEM)
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
				}
			}
			else
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
				}
			}
		}
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || pItemsTxtRecord->nMinDam || !pItemsTxtRecord->n2HandMinDam)
	{
		nStatValue = nMin;
		if (pItemsTxtRecord->nMinDam)
		{
			if (pItemsTxtRecord->nMinDam + nMin <= 0)
			{
				nStatValue = 1 - pItemsTxtRecord->nMinDam;
			}
		}

		if (nStatValue)
		{
			ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, STAT_MINDAMAGE, 0, nStatValue, nState, fStatList, a11);
		}
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || pItemsTxtRecord->n2HandMinDam || !pItemsTxtRecord->nMinDam)
	{
		nStatValue = nMin;
		if (pItemsTxtRecord->n2HandMinDam)
		{
			if (pItemsTxtRecord->n2HandMinDam + nMin <= 0)
			{
				nStatValue = 1 - pItemsTxtRecord->n2HandMinDam;
			}
		}

		if (nStatValue)
		{
			ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, STAT_SECONDARY_MINDAMAGE, 0, nStatValue, nState, fStatList, a11);
		}
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || ITEMS_CheckIfThrowable(pItem))
	{
		nStatValue = nMin;
		if (pItemsTxtRecord->nMinMisDam)
		{
			if (pItemsTxtRecord->nMinMisDam + nMin <= 0)
			{
				nStatValue = 1 - pItemsTxtRecord->nMinMisDam;
			}
		}

		if (nStatValue)
		{
			ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, STAT_ITEM_THROW_MINDAMAGE, 0, nStatValue, nState, fStatList, a11);
		}
	}

	return nMin;
}

//D2Common.0x6FD96880
int __fastcall ITEMMODS_PropertyFunc06(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nTwoHandMaxDamage = 0;
	int nPossibleRolls = 0;
	int nMisMaxDamage = 0;
	int nMaxDamage = 0;
	int nTemp = 0;
	int nMin = 0;
	int nMax = 0;
	uint32_t dwClassId = 0;

	if (!pProperty)
	{
		return 0;
	}

	if (!pItem || pItem->dwUnitType != UNIT_ITEM)
	{
		return 0;
	}

	if (pUnit && pUnit->dwUnitType == UNIT_ITEM)
	{
		dwClassId = pUnit->dwClassId;
	}
	else
	{
		dwClassId = pItem->dwClassId;
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(dwClassId);
	if (!pItemsTxtRecord)
	{
		return 0;
	}

	nMin = nValue;
	if (!nValue)
	{
		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			if (pItem && pItem->dwUnitType == UNIT_ITEM)
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
				}
			}
			else
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
				}
			}
		}
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || pItemsTxtRecord->nMaxDam || !pItemsTxtRecord->n2HandMaxDam)
	{
		nTemp = nMin;
		if (pItemsTxtRecord->nMaxDam)
		{
			nMaxDamage = pItemsTxtRecord->nMaxDam;
			if (nMaxDamage + nMin <= 0)
			{
				nTemp = -nMaxDamage;
			}
		}

		if (nTemp)
		{
			ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, STAT_MAXDAMAGE, 0, nTemp, nState, fStatList, a11);
		}
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || pItemsTxtRecord->n2HandMaxDam || !pItemsTxtRecord->nMaxDam)
	{
		nTemp = nMin;
		if (pItemsTxtRecord->n2HandMaxDam)
		{
			nTwoHandMaxDamage = pItemsTxtRecord->n2HandMaxDam;
			if (nTwoHandMaxDamage + nMin <= 0)
			{
				nTemp = -nTwoHandMaxDamage;
			}
		}

		if (nTemp)
		{
			ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, STAT_SECONDARY_MAXDAMAGE, 0, nTemp, nState, fStatList, a11);
		}
	}

	if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) || ITEMS_CheckIfThrowable(pItem))
	{
		nTemp = nMin;
		if (pItemsTxtRecord->nMaxMisDam)
		{
			nMisMaxDamage = pItemsTxtRecord->nMaxMisDam;
			if (nMisMaxDamage + nMin <= 0)
			{
				nTemp = -nMisMaxDamage;
			}
		}

		if (nTemp)
		{
			ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, STAT_ITEM_THROW_MAXDAMAGE, 0, nTemp, nState, fStatList, a11);
		}
	}

	return nMin;
}

//D2Common.0x6FD96B00
int __fastcall ITEMMODS_PropertyFunc07(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	long long int nBonus = 0;
	int nPossibleRolls = 0;
	int nMaxDamage = 0;
	int nMin = 0;
	int nMax = 0;
	uint32_t dwClassId = 0;

	if (!pProperty)
	{
		return 0;
	}

	nMin = nValue;
	if (!nValue)
	{
		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			if (pItem && pItem->dwUnitType == UNIT_ITEM)
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
				}
			}
			else
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
				}
			}
		}
	}

	if (!pItem || pItem->dwUnitType != UNIT_ITEM)
	{
		ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, STAT_ITEM_MINDAMAGE_PERCENT, 0, nMin, nState, fStatList, a11);
		ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, STAT_ITEM_MAXDAMAGE_PERCENT, 0, nMin, nState, fStatList, a11);
		return 1;
	}

	if (pUnit && pUnit->dwUnitType == UNIT_ITEM)
	{
		dwClassId = pUnit->dwClassId;
	}
	else
	{
		dwClassId = pItem->dwClassId;
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(dwClassId);
	if (pItemsTxtRecord)
	{
		sub_6FD92CF0(pItem, STAT_MINDAMAGE);
		sub_6FD92CF0(pItem, STAT_MAXDAMAGE);
		sub_6FD92CF0(pItem, STAT_SECONDARY_MINDAMAGE);
		sub_6FD92CF0(pItem, STAT_SECONDARY_MAXDAMAGE);
		sub_6FD92CF0(pItem, STAT_ITEM_THROW_MINDAMAGE);
		sub_6FD92CF0(pItem, STAT_ITEM_THROW_MAXDAMAGE);

		nMaxDamage = pItemsTxtRecord->n2HandMaxDam;
		if (nMaxDamage <= pItemsTxtRecord->nMaxDam)
		{
			nMaxDamage = pItemsTxtRecord->nMaxDam;
		}

		nBonus = nMin * (long long int)nMaxDamage / 100;

		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) && nMaxDamage + nBonus <= nMaxDamage)
		{
			return ITEMMODS_PropertyFunc06(nType, pUnit, pItem, pProperty, nSet, nStatId, 0, 1, nState, fStatList, a11);
		}
		else
		{
			ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, STAT_ITEM_MINDAMAGE_PERCENT, 0, nMin, nState, fStatList, a11);
			ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, STAT_ITEM_MAXDAMAGE_PERCENT, 0, nMin, nState, fStatList, a11);
			return nMin;
		}
	}

	return 0;
}

//D2Common.0x6FD96DA0
int __fastcall ITEMMODS_PropertyFunc08(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	nMin = nValue;
	if (!nValue)
	{
		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			if (pItem && pItem->dwUnitType == UNIT_ITEM)
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
				}
			}
			else
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
				}
			}
		}
	}

	return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, 0, nMin, nState, fStatList, a11);
}

//D2Common.0x6FD96EE0
int __fastcall ITEMMODS_PropertyFunc09(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	nMin = nValue;
	if (!nValue)
	{
		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			if (pItem && pItem->dwUnitType == UNIT_ITEM)
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
				}
			}
			else
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
				}
			}
		}

		if (!nMin)
		{
			return 0;
		}
	}

	if (DATATBLS_GetSkillsTxtRecord(pProperty->nLayer))
	{
		return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, pProperty->nLayer, nMin, nState, fStatList, a11);
	}

	return 0;
}

//D2Common.0x6FD97040
int __fastcall ITEMMODS_PropertyFunc24(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	nMin = nValue;
	if (!nValue)
	{
		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			if (pItem && pItem->dwUnitType == UNIT_ITEM)
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
				}
			}
			else
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
				}
			}
		}
	}

	return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, pProperty->nLayer, nMin, nState, fStatList, a11);
}

//D2Common.0x6FD97180
int __fastcall ITEMMODS_PropertyFunc10(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	if (nValue)
	{
		return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, pProperty->nLayer % 3 + 8 * pProperty->nLayer / 3, nValue, nState, fStatList, a11);
	}

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMax != nMin)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		if (pItem && pItem->dwUnitType == UNIT_ITEM)
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
			}
		}
		else
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
			}
		}
	}

	if (!nMin)
	{
		return 0;
	}

	return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, pProperty->nLayer % 3 + 8 * pProperty->nLayer / 3, nMin, nState, fStatList, a11);
}

//D2Common.0x6FD972E0
int __fastcall ITEMMODS_PropertyFunc11(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nStatValue = 0;
	int nDivisor = 0;
	int nMaxLevel = 0;
	int nLevel = 0;
	int nTmp = 0;

	if (pProperty && DATATBLS_GetSkillsTxtRecord(pProperty->nLayer))
	{
		nStatValue = pProperty->nMin;
		if (nStatValue <= 0)
		{
			nStatValue = 5;
		}

		nLevel = pProperty->nMax;
		if (nLevel)
		{
			if (nLevel < 0)
			{
				nTmp = 99 - SKILLS_GetRequiredLevel(pProperty->nLayer);
				if (nTmp < 1)
				{
					nTmp = 1;
				}

				nDivisor = -(nTmp / nLevel);
				if (nDivisor < 1)
				{
					nDivisor = 1;
				}

				nLevel = (ITEMS_GetItemLevel(pItem) - SKILLS_GetRequiredLevel(pProperty->nLayer)) / nDivisor;
				if (nLevel <= 0)
				{
					nLevel = 1;
				}
			}
		}
		else
		{
			nLevel = (ITEMS_GetItemLevel(pItem) - SKILLS_GetRequiredLevel(pProperty->nLayer)) / 4 + 1;

			nMaxLevel = sgptDataTables->pSkillsTxt[pProperty->nLayer].wMaxLvl;
			if (nMaxLevel <= 0)
			{
				nMaxLevel = 20;
			}

			if (nLevel <= 1)
			{
				nLevel = 1;
			}

			if (nLevel >= nMaxLevel)
			{
				nLevel = nMaxLevel;
			}
		}

		return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, (nLevel & 63) + (pProperty->nLayer << 6), nStatValue, nState, fStatList, a11);
	}

	return 0;
}

//D2Common.0x6FD97430
int __fastcall ITEMMODS_PropertyFunc14(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nAllowedMaxSockets = 0;
	int nMaxSockets = 0;
	int nSockets = 0;
	int nMin = 0;
	int nMax = 0;
	uint8_t nHeight = 0;
	uint8_t nWidth = 0;

	if (!pProperty || !pItem || pItem->dwUnitType != UNIT_ITEM)
	{
		return 0;
	}
	
	ITEMS_GetDimensions(pItem, &nWidth, &nHeight, __FILE__, __LINE__);

	nMaxSockets = nWidth * nHeight;
	if (nMaxSockets <= 0)
	{
		return 0;
	}

	nAllowedMaxSockets = ITEMS_GetMaxSockets(pItem);
	if (nMaxSockets >= nAllowedMaxSockets)
	{
		nMaxSockets = nAllowedMaxSockets;
	}

	nMin = nValue;
	if (nValue <= 0)
	{
		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMax != nMin)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			if (pItem->dwUnitType == UNIT_ITEM)
			{
				nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nMax - nMin + 1);
			}
			else
			{
				if (nMin < nMax)
				{
					nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nMax - nMin + 1);
				}
			}
		}

		if (nMin <= 0)
		{
			nMin = pProperty->nLayer;
		}
	}

	nSockets = nMin;
	if (nMin <= 1)
	{
		nSockets = 1;
	}

	if (nSockets >= nMaxSockets)
	{
		nSockets = nMaxSockets;
	}

	if (nSockets > 0)
	{
		ITEMS_SetItemFlag(pItem, IFLAG_SOCKETED, TRUE);
		STATLIST_SetUnitStat(pItem, STAT_ITEM_NUMSOCKETS, nSockets, 0);
		return nSockets;
	}

	return 0;
}

//D2Common.0x6FD975F0
int __fastcall ITEMMODS_PropertyFunc19(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	D2StatListStrc* pStatList = NULL;
	unsigned int nRand = 0;
	int nRequiredSkillLevel = 0;
	int nMaxLevel = 0;
	int nSkillId = 0;
	int nLevel = 0;
	int nTemp = 0;
	int nMin = 0;

	if (!pProperty || !pItem || pItem->dwUnitType != UNIT_ITEM)
	{
		return 0;
	}

	nSkillId = pProperty->nLayer;
	if (!DATATBLS_GetSkillsTxtRecord(nSkillId))
	{
		return 0;
	}

	if (pProperty->nMax == 0)
	{
		nTemp = (ITEMS_GetItemLevel(pItem) - SKILLS_GetRequiredLevel(nSkillId)) / 4 + 1;

		nMaxLevel = sgptDataTables->pSkillsTxt[nSkillId].wMaxLvl;
		if (nMaxLevel <= 0)
		{
			nMaxLevel = 20;
		}

		if (nTemp <= 1)
		{
			nTemp = 1;
		}

		if (nTemp >= nMaxLevel)
		{
			nLevel = nMaxLevel;
		}
		else
		{
			nLevel = nTemp;
		}
	}
	else if (pProperty->nMax < 0)
	{
		nRequiredSkillLevel = SKILLS_GetRequiredLevel(nSkillId);

		nTemp = 99 - nRequiredSkillLevel;
		if (nTemp < 1)
		{
			nTemp = 1;
		}

		nTemp = -(nTemp / pProperty->nMax);
		if (nTemp < 1)
		{
			nTemp = 1;
		}

		nLevel = (ITEMS_GetItemLevel(pItem) - nRequiredSkillLevel) / nTemp;
		if (nLevel <= 0)
		{
			nLevel = 1;
		}
	}
	else
	{
		nLevel = pProperty->nMax;
	}

	nMin = pProperty->nMin;
	if (!nMin)
	{
		nTemp = 5;
	}
	else
	{
		if (nMin < 0)
		{
			nMin = nLevel * -nMin / 8 - nMin;
		}

		if (nMin > 1)
		{
			if (nMin >= 255)
			{
				nTemp = 255;
			}
			else
			{
				nTemp = nMin;
			}
		}
		else
		{
			nTemp = 1;
		}
	}

	nRand = SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nTemp - nTemp / 8);

	pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
	STATLIST_SetStatIfListIsValid(pStatList, nStatId, (nTemp << 8) + ((nRand + nTemp / 8 + 1) & 0xFF), (nLevel & ((uint16_t)sgptDataTables->nShiftedStuff)) + (nSkillId << sgptDataTables->nStuff));

	return nTemp;
}

//D2Common.0x6FD97830
int __fastcall ITEMMODS_PropertyFunc18(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	D2StatListStrc* pStatList = NULL;
	int nPropLayer = 0;
	int nMin = 0;
	int nMax = 0;

	if (pProperty)
	{
		if (ITEMS_GetItemStatCostTxtRecord(nStatId))
		{
			nPropLayer = pProperty->nLayer;
			nMin = pProperty->nMin + 256;
			nMax = pProperty->nMax + 256;

			if (nPropLayer > 0)
			{
				if (nPropLayer >= 3)
				{
					nPropLayer = 3;
				}
			}
			else
			{
				nPropLayer = 0;
			}

			if (nMin > 0)
			{
				if (nMin >= 1023)
				{
					nMin = 1023;
				}
			}
			else
			{
				nMin = 0;
			}

			if (nMax > 0)
			{
				if (nMax >= 1023)
				{
					nMax = 1023;
				}
			}
			else
			{
				nMax = 0;
			}

			pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
			if (pStatList)
			{
				STATLIST_SetStatIfListIsValid(pStatList, nStatId, nPropLayer + 4 * (nMin + (nMax << 10)), 0);
				return nMax;
			}
		}
	}

	return 0;
}

//D2Common.0x6FD97920
int __fastcall ITEMMODS_PropertyFunc15(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	if (pProperty)
	{
		if (nStatId == STAT_MINDAMAGE)
		{
			ITEMMODS_PropertyFunc05(nType, pUnit, pItem, pProperty, nSet, STAT_MINDAMAGE, 0, pProperty->nMin, nState, fStatList, a11);
		}
		else
		{
			ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, 0, pProperty->nMin, nState, fStatList, a11);
		}

		return pProperty->nMin;
	}

	return 0;
}

//D2Common.0x6FD979A0
int __fastcall ITEMMODS_PropertyFunc16(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	if (pProperty)
	{
		if (nStatId == STAT_MAXDAMAGE)
		{
			ITEMMODS_PropertyFunc06(nType, pUnit, pItem, pProperty, nSet, STAT_MAXDAMAGE, 0, pProperty->nMax, nState, fStatList, a11);
		}
		else
		{
			ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, 0, pProperty->nMax, nState, fStatList, a11);
		}

		return pProperty->nMax;
	}

	return 0;
}

//D2Common.0x6FD97A20
int __fastcall ITEMMODS_PropertyFunc17(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nTemp = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	nTemp = pProperty->nLayer;
	if (!nTemp)
	{
		nMin = pProperty->nMin;
		nMax = pProperty->nMax;

		if (nMin != nMax)
		{
			if (nMax < nMin)
			{
				nMax = pProperty->nMin;
				nMin = pProperty->nMax;
			}

			if (pItem && pItem->dwUnitType == UNIT_ITEM)
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
				}
			}
			else
			{
				if (nMin < nMax)
				{
					nPossibleRolls = nMax - nMin + 1;
					nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
				}
			}
		}

		nTemp = nMin;
		if (!nTemp)
		{
			return 0;
		}
	}

	if (nStatId == STAT_MAXDAMAGE)
	{
		ITEMMODS_PropertyFunc06(nType, pUnit, pItem, pProperty, nSet, STAT_MAXDAMAGE, 0, nTemp, nState, fStatList, a11);
	}
	else
	{
		ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, 0, nTemp, nState, fStatList, a11);
	}

	return nTemp;
}

//D2Common.0x6FD97BA0
int __fastcall ITEMMODS_PropertyFunc20(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	D2StatListStrc* pStatList = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM && pProperty)
	{
		pStatList = ITEMMODS_GetOrCreateStatList(pUnit, pItem, nState, fStatList);
		if (pStatList)
		{
			STATLIST_AddStat(pStatList, STAT_ITEM_INDESCTRUCTIBLE, 1, 0);
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD97C20
int __fastcall ITEMMODS_PropertyFunc21(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMin != nMax)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		if (pItem && pItem->dwUnitType == UNIT_ITEM)
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
			}
		}
		else
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
			}
		}
	}

	return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, nLayer, nMin, nState, fStatList, a11);
}

//D2Common.0x6FD97D50
//TODO: nLayer always (in all functions and structs) 16 Bit, i.e. uint16_t??
int __fastcall ITEMMODS_PropertyFunc22(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMin != nMax)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		if (pItem && pItem->dwUnitType == UNIT_ITEM)
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
			}
		}
		else
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
			}
		}
	}

	return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, (uint16_t)pProperty->nLayer, nMin, nState, fStatList, a11);
}

//D2Common.0x6FD97E80
int __fastcall ITEMMODS_PropertyFunc12(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nPossibleRolls = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty)
	{
		return 0;
	}

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMin != nMax)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		if (pItem && pItem->dwUnitType == UNIT_ITEM)
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
			}
		}
		else
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
			}
		}
	}

	return ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, nMin, pProperty->nLayer, nState, fStatList, a11);
}

//D2Common.0x6FD97FB0
int __fastcall ITEMMODS_PropertyFunc13(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	int nMaxDurability = 0;
	int nPossibleRolls = 0;
	int nStatValue = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pProperty || !pItem)
	{
		return 0;
	}

	if (pItem->dwUnitType == UNIT_ITEM && nType == 1)
	{
		sub_6FD92CF0(pItem, nStatId);
	}

	nMin = pProperty->nMin;
	nMax = pProperty->nMax;

	if (nMax != nMin)
	{
		if (nMax < nMin)
		{
			nMax = pProperty->nMin;
			nMin = pProperty->nMax;
		}

		if (pItem->dwUnitType == UNIT_ITEM)
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nPossibleRolls);
			}
		}
		else
		{
			if (nMin < nMax)
			{
				nPossibleRolls = nMax - nMin + 1;
				nMin += SEED_RollLimitedRandomNumber(&pItem->pSeed, nPossibleRolls);
			}
		}
	}

	nStatValue = ITEMMODS_AddPropertyToItemStatList(nType, pUnit, pItem, pProperty, nSet, nStatId, 0, nMin, nState, fStatList, a11);
	if (!nStatValue)
	{
		return 0;
	}

	nMaxDurability = STATLIST_GetMaxDurabilityFromUnit(pItem);
	if (nMaxDurability > 0)
	{
		STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nMaxDurability, 0);
	}

	return nStatValue;
}

//D2Common.0x6FD98120
int __fastcall ITEMMODS_PropertyFunc23(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, const D2PropertyStrc* pProperty, int nSet, short nStatId, int nLayer, int nValue, int nState, int fStatList, D2UnitStrc* a11)
{
	if (pProperty && pItem && pItem->dwUnitType == UNIT_ITEM && !ITEMS_IsEthereal(pItem) && ITEMS_HasDurability(pItem))
	{
		ITEMMODS_ApplyEthereality(pItem);
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD98160 (#11292)
void __stdcall D2COMMON_11292_ItemAssignProperty(int nType, D2UnitStrc* pUnit, D2UnitStrc* pItem, void* pMods, int nIndex, int nPropSet, const D2PropertyStrc* pProperty, int nState, int fStatlist, D2UnitStrc* a10)
{
	D2PropertiesTxt* pPropertiesTxtRecord = NULL;
	PROPERTYASSIGNFN pfAssign = NULL;
	int nFirstValue = 0;
	int nResult = 0;

	if (pProperty && pProperty->nProperty >= 0 && pProperty->nProperty < sgptDataTables->nPropertiesTxtRecordCount)
	{
		pPropertiesTxtRecord = &sgptDataTables->pPropertiesTxt[pProperty->nProperty];
		if (pPropertiesTxtRecord)
		{
			for (int i = 0; i < ARRAY_SIZE(pPropertiesTxtRecord->nFunc); ++i)
			{
				if (pPropertiesTxtRecord->nFunc[i] >= dword_6FDE39B0)
				{
					break;
				}

				pfAssign = off_6FDE3920[pPropertiesTxtRecord->nFunc[i]];
				if (!pfAssign)
				{
					break;
				}

				nResult = pfAssign(nType, pUnit, pItem, pProperty, pPropertiesTxtRecord->nSet[i], pPropertiesTxtRecord->wStat[i], pPropertiesTxtRecord->wVal[i], nFirstValue, nState, fStatlist, a10);
				if (!i)
				{
					nFirstValue = nResult;
				}
			}
		}
	}
}

//D2Common.0x6FD98220
//TODO: a4, v5
int __fastcall sub_6FD98220(int nMin, int nMax, int nUnused, D2UnkMissileCalcStrc* a4)
{
	D2SeedStrc* v5 = NULL;

	if (a4)
	{
		v5 = (D2SeedStrc*)(a4->field_0 + 32);
		if (nMin < nMax)
		{
			return nMin + SEED_RollLimitedRandomNumber(v5, nMax - nMin + 1);
		}
		
		return nMin;
	}

	return 0;
}

//D2Common.0x6FD982A0
int __fastcall sub_6FD982A0(int nStatId, int a2, int nUnused, D2ItemCalcStrc* pItemCalc)
{
	if (pItemCalc && pItemCalc->pUnit && ITEMS_GetItemStatCostTxtRecord(nStatId))
	{
		if (nStatId == STAT_TOHIT)
		{
			return UNITS_GetAttackRate(pItemCalc->pUnit);
		}
		else
		{
			if (a2 == 1)
			{
				return STATLIST_GetUnitBaseStat(pItemCalc->pUnit, nStatId, 0);
			}
			else if (a2 == 2)
			{
				return STATLIST_GetUnitStatSigned(pItemCalc->pUnit, nStatId, 0);
			}
			else
			{
				return STATLIST_GetUnitStatUnsigned(pItemCalc->pUnit, nStatId, 0);
			}
		}
	}

	return 0;
}

//D2Common.0x6FD98300 (#11300)
int __stdcall ITEMMODS_EvaluateItemFormula(D2UnitStrc* pUnit, D2UnitStrc* pItem, unsigned int nCalc)
{
	D2ItemCalcStrc pItemCalc = {};

	if (sgptDataTables->pItemsCode && nCalc < sgptDataTables->nItemsCodeSize)
	{
		pItemCalc.pUnit = pUnit;
		pItemCalc.pItem = pItem;

		return FOG_10253(&sgptDataTables->pItemsCode[nCalc], sgptDataTables->nItemsCodeSize - nCalc, D2COMMON_10018_Return0, off_6FDE3BA0, dword_6FDE3BC0, &pItemCalc);
	}

	return 0;
}
