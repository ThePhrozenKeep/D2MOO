#include "D2Items.h"
#include <Units/Item.h>
#include "D2BitManip.h"
#include "D2Composit.h"
#include "D2DataTbls.h"
#include "D2Inventory.h"
#include "D2ItemMods.h"
#include "D2QuestRecord.h"
#include "D2Seed.h"
#include "D2States.h"
#include "D2StatList.h"
#include "Units/Units.h"
#include <D2CMP.h>
#include <DataTbls/MonsterIds.h>


//D2Common.0x6FD98380 (#10687)
void __stdcall ITEMS_AllocItemData(void* pMemPool, D2UnitStrc* pItem)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItem->pItemData = D2_ALLOC_STRC_POOL(pMemPool, D2ItemDataStrc);
		if (!pItem->pItemData)
		{
			//FOG_DisplayHalt("Out of Memory in ITEMSDataInit()", __FILE__, __LINE__);
			exit(-1);
		}
		memset(pItem->pItemData, 0x00, sizeof(D2ItemDataStrc));

		pItem->pItemData->dwOwnerGUID = D2UnitInvalidGUID;
	}
}

//D2Common.0x6FD983F0 (#10688)
void __stdcall ITEMS_FreeItemData(void* pMemPool, D2UnitStrc* pItem)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		INVENTORY_RemoveItem(pItem);
		if (pItem->pItemData)
		{
			D2_FREE_POOL(pMemPool, pItem->pItemData);
		}
	}
}

//D2Common.0x6FD98430 (#10689)
uint8_t __stdcall ITEMS_GetBodyLocation(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->nBodyLoc;
	}

	return BODYLOC_NONE;
}

//D2Common.0x6FD98450 (#10690)
void __stdcall ITEMS_SetBodyLocation(D2UnitStrc* pItem, uint8_t nBodyLoc)
{
	if (D2ItemDataStrc* pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->nBodyLoc = nBodyLoc;
	}
}

//D2Common.0x6FD98470 (#10691)
D2SeedStrc* __stdcall ITEMS_GetItemSeed(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc* pItemData = ITEMS_GetItemData(pItem))
	{
		return &pItemData->pSeed;
	}
	
	return NULL;
}

//D2Common.0x6FD98490 (#10692)
void __stdcall ITEMS_InitItemSeed(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc* pItemData = ITEMS_GetItemData(pItem))
	{
		SEED_InitSeed(&pItemData->pSeed);
	}
}

//D2Common.0x6FD984B0 (#10693)
int __stdcall ITEMS_GetItemStartSeed(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->dwInitSeed;
	}
	
	return 0;
}

//D2Common.0x6FD984D0 (#10694)
void __stdcall ITEMS_SetItemStartSeed(D2UnitStrc* pItem, int nSeed)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);
	D2_ASSERT(pItem->pItemData);
	pItem->pItemData->dwInitSeed = nSeed;
}

//D2Common.0x6FD98550 (#10695)
int __stdcall ITEMS_GetItemQuality(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->dwQualityNo;
	}

	return ITEMQUAL_NORMAL;
}

//D2Common.0x6FD98580 (#10696)
void __stdcall ITEMS_SetItemQuality(D2UnitStrc* pItem, int nQuality)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->dwQualityNo = nQuality;
	}
}

//D2Common.0x6FD985A0 (#10699)
uint16_t __stdcall ITEMS_GetPrefixId(D2UnitStrc* pItem, int nPrefixNo)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->wMagicPrefix[nPrefixNo];
	}

	return 0;
}

//D2Common.0x6FD985D0 (#10700)
void __stdcall ITEMS_AssignPrefix(D2UnitStrc* pItem, uint16_t nPrefix, int nPrefixNo)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->wMagicPrefix[nPrefixNo] = nPrefix;
	}
}

//D2Common.0x6FD98600 (#10697)
uint16_t __stdcall ITEMS_GetAutoAffix(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->wAutoAffix;
	}

	return 0;
}

//D2Common.0x6FD98630 (#10698)
void __stdcall ITEMS_SetAutoAffix(D2UnitStrc* pItem, uint16_t nAffix)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->wAutoAffix = nAffix;
	}
}

//D2Common.0x6FD98650 (#10701)
uint16_t __stdcall ITEMS_GetSuffixId(D2UnitStrc* pItem, int nSuffixNo)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->wMagicSuffix[nSuffixNo];
	}

	return 0;
}

//D2Common.0x6FD98680 (#10702)
void __stdcall ITEMS_AssignSuffix(D2UnitStrc* pItem, uint16_t nSuffix, int nSuffixNo)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->wMagicSuffix[nSuffixNo] = nSuffix;
	}
}

//D2Common.0x6FD986B0 (#10703)
uint16_t __stdcall ITEMS_GetRarePrefixId(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->wRarePrefix;
	}

	return 0;
}

//D2Common.0x6FD986E0 (#10704)
void __stdcall ITEMS_AssignRarePrefix(D2UnitStrc* pItem, uint16_t nPrefix)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->wRarePrefix = nPrefix;
	}
}

//D2Common.0x6FD98700 (#10705)
uint16_t __stdcall ITEMS_GetRareSuffixId(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->wRareSuffix;
	}

	return 0;
}

//D2Common.0x6FD98730 (#10706)
void __stdcall ITEMS_AssignRareSuffix(D2UnitStrc* pItem, uint16_t nSuffix)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->wRareSuffix = nSuffix;
	}
}

//D2Common.0x6FD98750 (#10707)
BOOL __stdcall ITEMS_CheckItemFlag(D2UnitStrc* pItem, uint32_t dwFlag, int nLine, const char* szFile)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->dwItemFlags & dwFlag;
	}

	return 0;
}

//D2Common.0x6FD98780 (#10708)
void __stdcall ITEMS_SetItemFlag(D2UnitStrc* pItem, uint32_t dwFlag, BOOL bSet)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		if (bSet)
		{
			pItemData->dwItemFlags |= dwFlag;
		}
		else
		{
			pItemData->dwItemFlags &= ~dwFlag;
		}
	}
}

//D2Common.0x6FD987C0 (#10709)
uint32_t __stdcall ITEMS_GetItemFlags(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->dwItemFlags;
	}

	return 0;
}

//D2Common.0x6FD987E0 (#10710)
BOOL __stdcall ITEMS_CheckItemCMDFlag(D2UnitStrc* pItem, int nFlag)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->dwCommandFlags & nFlag;
	}

	return 0;
}

//D2Common.0x6FD98810 (#10711)
void __stdcall ITEMS_SetItemCMDFlag(D2UnitStrc* pItem, int nFlag, BOOL bSet)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		if (bSet)
		{
			pItemData->dwCommandFlags |= nFlag;
		}
		else
		{
			pItemData->dwCommandFlags &= ~nFlag;
		}
	}
}

//D2Common.0x6FD98850 (#10712)
uint32_t __stdcall ITEMS_GetItemCMDFlags(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->dwCommandFlags;
	}

	return 0;
}

//D2Common.0x6FD98870 (#10717)
int __stdcall ITEMS_GetItemLevel(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		if (pItemData->dwItemLevel < 1)
		{
			pItemData->dwItemLevel = 1;
		}
		return pItemData->dwItemLevel;
	}

	return 1;
}

//D2Common.0x6FD988B0 (#10718)
void __stdcall ITEMS_SetItemLevel(D2UnitStrc* pItem, int nItemLevel)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		if (nItemLevel < 1)
		{
			nItemLevel = 1;
		}
		pItemData->dwItemLevel = nItemLevel;
	}
}

//D2Common.0x6FD988E0 (#10719)
uint8_t __stdcall ITEMS_GetInvPage(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->nInvPage;
	}

	return -1;
}

//D2Common.0x6FD98900 (#10720)
void __stdcall ITEMS_SetInvPage(D2UnitStrc* pItem, uint8_t nPage)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->nInvPage = nPage;
	}
}

//D2Common.0x6FD98920 (#10721)
uint8_t __stdcall ITEMS_GetCellOverlap(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->nCellOverlap;
	}

	return -1;
}

//D2Common.0x6FD98940 (#10722)
void __stdcall ITEMS_SetCellOverlap(D2UnitStrc* pItem, int nCellOverlap)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->nCellOverlap = nCellOverlap;
	}
}

//D2Common.0x6FD98960 (#10853)
uint8_t __stdcall ITEMS_GetItemCell(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->nItemCell;
	}

	return -1;
}

//D2Common.0x6FD98980 (#10854)
void __stdcall ITEMS_SetItemCell(D2UnitStrc* pItem, int nItemCell)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->nItemCell = nItemCell;
	}
}

//D2Common.0x6FD989A0 (#10723)
char* __stdcall ITEMS_GetEarName(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->szPlayerName;
	}

	return NULL;
}

//D2Common.0x6FD989C0 (#10724)
void __stdcall ITEMS_SetEarName(D2UnitStrc* pItem, char* szName)
{
	int nCounter = 0;

	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		do
		{
			pItemData->szPlayerName[nCounter] = szName[nCounter];
			++nCounter;
		}
		while (szName[nCounter -1]);
	}
}

//D2Common.0x6FD989F0 (#10725)
uint8_t __stdcall ITEMS_GetEarLevel(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->nEarLvl;
	}

	return 1;
}

//D2Common.0x6FD98A10 (#10726)
void __stdcall ITEMS_SetEarLevel(D2UnitStrc* pItem, uint8_t nLevel)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->nEarLvl = nLevel;
	}
}

//D2Common.0x6FD98A30 (#10727)
uint8_t __stdcall ITEMS_GetVarGfxIndex(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->nInvGfxIdx;
	}

	return 0;
}

//D2Common.0x6FD98A50 (#10728)
void __stdcall ITEMS_SetVarGfxIndex(D2UnitStrc* pItem, uint8_t nIndex)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->nInvGfxIdx = nIndex;
	}
}

//D2Common.0x6FD98A70 (#10777)
BOOL __stdcall ITEMS_IsRepairable(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nItemType = 0;
	int nCounter = 0;
	int nStats = 0;
	D2StatStrc pStat[64] = {};

	if (pItem && pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwItemFlags & IFLAG_IDENTIFIED && !(pItem->pItemData->dwItemFlags & IFLAG_ETHEREAL))
	{
		nStats = STATLIST_CopyStats(pItem, STAT_ITEM_CHARGED_SKILL, pStat, ARRAY_SIZE(pStat));

		if (nStats > 0)
		{
			while ((uint8_t)pStat[nCounter].nValue >= (pStat[nCounter].nValue >> 8))
			{
				++nCounter;
				if (nCounter >= nStats)
				{
					break;
				}
			}

			if (nCounter < nStats)
			{
				return TRUE;
			}
		}

		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] < 0)
		{
			return FALSE;
		}

		if (pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord && pItemTypesTxtRecord->nRepair)
			{
				nItemType = ITEMS_GetItemTypeFromItemId(pItem->dwClassId);
				if (nItemType < 0)
				{
					if (!pItemsTxtRecord->nNoDurability && pItemsTxtRecord->nDurability && STATLIST_GetMaxDurabilityFromUnit(pItem) && STATLIST_UnitGetStatValue(pItem, STAT_ITEM_INDESCTRUCTIBLE, 0) <= 0)
					{
						return TRUE;
					}
					return FALSE;
				}

				if (nItemType < sgptDataTables->nItemTypesTxtRecordCount)
				{
					pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
					if (pItemTypesTxtRecord && pItemTypesTxtRecord->nRepair)
					{
						if (ITEMS_CheckItemTypeIfThrowable(ITEMS_GetItemType(pItem)) && ITEMS_CheckIfStackable(pItem) && !ITEMS_CheckItemFlag(pItem, IFLAG_ETHEREAL, __LINE__, __FILE__))
						{
							return TRUE;
						}
					}
				}

				if (!pItemsTxtRecord->nNoDurability && pItemsTxtRecord->nDurability && STATLIST_GetMaxDurabilityFromUnit(pItem) && STATLIST_UnitGetStatValue(pItem, STAT_ITEM_INDESCTRUCTIBLE, 0) <= 0)
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD98C60 (#10780)
int32_t __stdcall ITEMS_GetAmmoTypeFromItemType(int nItemType)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;

	if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
	{
		pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
		if (pItemTypesTxtRecord)
		{
			return pItemTypesTxtRecord->wShoots;
		}
	}

	return 0;
}

//D2Common.0x6FD98CA0 (#10781)
int32_t __stdcall ITEMS_GetAmmoType(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		return ITEMS_GetAmmoTypeFromItemType(pItemsTxtRecord->wType[0]);
	}

	return 0;
}

//D2Common.0x6FD98D20 (#10782)
int32_t __stdcall ITEMS_GetQuiverTypeFromItemType(int nItemType)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;

	if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
	{
		pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
		if (pItemTypesTxtRecord)
		{
			return pItemTypesTxtRecord->wQuiver;
		}
	}

	return 0;
}

//D2Common.0x6FD98D60 (#10783)
int32_t __stdcall ITEMS_GetQuiverType(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		return ITEMS_GetQuiverTypeFromItemType(pItemsTxtRecord->wType[0]);
	}

	return 0;
}

//D2Common.0x6FD98DE0 (#10784)
uint32_t __stdcall ITEMS_GetAutoStackFromItemType(int nItemType)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;

	if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
	{
		pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
		{
			return pItemTypesTxtRecord->nAutoStack;
		}
	}

	return 0;
}

//D2Common.0x6FD98E20 (#10785)
uint32_t __stdcall ITEMS_GetAutoStack(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		return ITEMS_GetAutoStackFromItemType(pItemsTxtRecord->wType[0]);
	}

	return 0;
}

//D2Common.0x6FD98EA0 (#10786)
uint32_t __stdcall ITEMS_GetReload(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				return pItemTypesTxtRecord->nReload;
			}
		}
	}

	return 0;
}

//D2Common.0x6FD98F20 (#10787)
uint32_t __stdcall ITEMS_GetReEquip(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				return pItemTypesTxtRecord->nReEquip;
			}
		}
	}

	return 0;
}

//D2Common.0x6FD98FA0 (#10788)
uint8_t __stdcall ITEMS_GetStorePage(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				return pItemTypesTxtRecord->nStorePage;
			}
		}
	}

	return -1;
}

//D2Common.0x6FD99020 (#10789)
uint8_t __stdcall ITEMS_GetVarInvGfxCount(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				return pItemTypesTxtRecord->nVarInvGfx;
			}
		}
	}

	return 0;
}

//D2Common.0x6FD990A0 (#10790)
char* __stdcall ITEMS_GetVarInvGfxString(D2UnitStrc* pItem, int nId)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				if (nId < 0)
				{
					nId = 0;
				}

				if (nId >= pItemTypesTxtRecord->nVarInvGfx - 1)
				{
					nId = pItemTypesTxtRecord->nVarInvGfx - 1;
				}

				return pItemTypesTxtRecord->szInvGfx[nId];
			}
		}
	}

	return &sgptDataTables->szDefaultString;
}

//D2Common.0x6FD99140 (#10792)
BOOL __stdcall ITEMS_CanBeRare(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				return pItemTypesTxtRecord->nRare;
			}
		}
	}

	return 0;
}

//D2Common.0x6FD991C0 (#10791)
BOOL __stdcall ITEMS_CanBeMagic(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				return pItemTypesTxtRecord->nMagic;
			}
		}
	}

	return 0;
}

//D2Common.0x6FD99240 (#10793)
BOOL __stdcall ITEMS_CanBeNormal(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				return pItemTypesTxtRecord->nNormal;
			}
		}
	}

	return 0;
}

//D2Common.0x6FD992C0 (#10744)
uint32_t __stdcall ITEMS_GetWeaponClassCode(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		if (pItemsTxtRecord)
		{
			return pItemsTxtRecord->dwWeapClass;
		}
	}

	return 0;
}

//D2Common.0x6FD992F0 (#10745)
uint32_t __stdcall ITEMS_Get2HandWeaponClassCode(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);
	return pItemsTxtRecord->dwWeapClass2Hand;
}

//D2Common.0x6FD99370 (#10746)
uint32_t __stdcall ITEMS_GetBaseCode(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);
	return pItemsTxtRecord->dwCode;
}

//D2Common.0x6FD993F0 (#10747)
uint32_t __stdcall ITEMS_GetAltGfx(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);
	if (pItemsTxtRecord->dwAlternateGfx)
	{
		return pItemsTxtRecord->dwAlternateGfx;
	}
	else
	{
		return pItemsTxtRecord->dwCode;
	}
}

//D2Common.0x6FD99480 (#10748)
uint8_t __stdcall ITEMS_GetComponent(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->nComponent;
}

//D2Common.0x6FD99500 (#10749)
void __stdcall ITEMS_GetDimensions(D2UnitStrc* pItem, uint8_t* pWidth, uint8_t* pHeight, const char* szFile, int nLine)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		if (D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId))
		{
			*pWidth = pItemsTxtRecord->nInvWidth;
			*pHeight = pItemsTxtRecord->nInvHeight;
		}
	}
}

//D2Common.0x6FD99540 (#10750)
void __stdcall ITEMS_GetAllowedBodyLocations(D2UnitStrc* pItem, uint8_t* pBodyLoc1, uint8_t* pBodyLoc2)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				*pBodyLoc1 = pItemTypesTxtRecord->nBodyLoc1;
				*pBodyLoc2 = pItemTypesTxtRecord->nBodyLoc2;
				return;
			}
		}
	}

	*pBodyLoc1 = 0;
	*pBodyLoc2 = 0;
}

//D2Common.0x6FD995D0 (#10751)
uint32_t __stdcall ITEMS_GetItemType(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	D2_ASSERT(pItem);

	if (pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);
		return pItemsTxtRecord->wType[0];
	}

	return 0;
}

//D2Common.0x6FD99640 (#10752)
uint32_t __stdcall ITEMS_GetItemTypeFromItemId(uint32_t dwItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(dwItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->wType[0];
}

//D2Common.0x6FD99680 (#10753)
uint8_t __stdcall ITEMS_GetItemQlvl(const D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);
	
	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);
	return pItemsTxtRecord->nLevel;
}

//D2Common.0x6FD99700 (#10754)
int __stdcall ITEMS_CheckIfFlagIsSet(int nFlags, int nFlag)
{
	return nFlags & nFlag;
}

//D2Common.0x6FD99710 (#10755)
void __stdcall ITEMS_SetOrRemoveFlag(int* pFlags, int nFlag, BOOL bSet)
{
	if (bSet)
	{
		*pFlags |= nFlag;
	}
	else
	{
		*pFlags &= ~nFlag;
	}
}

static int ITEMS_GetBonusStatFromSockets(D2UnitStrc* pItem, D2C_ItemStats stat)
{
	int nStatBonusFromSockets = 0;
	if (pItem->dwUnitType == UNIT_ITEM)
	{
		if (ITEMS_CheckIfSocketableByItemId(pItem->dwClassId))
		{
			for (D2UnitStrc* i = INVENTORY_GetFirstItem(pItem->pInventory); i != NULL; i = INVENTORY_GetNextItem(i))
			{
				i = INVENTORY_UnitIsItem(i);
				if (pItem->dwAnimMode == IMODE_EQUIP)
				{
					nStatBonusFromSockets += STATLIST_UnitGetStatValue(i, stat, 0);
				}
			}
		}
	}
	return nStatBonusFromSockets;
}

static bool ITEMS_CheckStatRequirement(D2UnitStrc* pItem, D2UnitStrc* pUnit, D2C_ItemStats stat, BOOL bEquipping, int nStatWithPctBonus)
{

	BOOL bStatReqMet = FALSE;
	const int nUnitStat = STATLIST_UnitGetStatValue(pUnit, stat, 0);
	if (nUnitStat > 0 && nUnitStat >= nStatWithPctBonus)
	{
		bStatReqMet = TRUE;
		// Note: Owner was not checked in the original game for STAT_DEXTERITY
		if (bEquipping && STATLIST_GetOwner(pItem, 0))
		{
			const int nStatBonusFromSockets = ITEMS_GetBonusStatFromSockets(pItem, stat);
			if ((nUnitStat - nStatBonusFromSockets) <= 0 || (nUnitStat - nStatBonusFromSockets) < nStatWithPctBonus)
			{
				bStatReqMet = FALSE;
			}
		}
	}
	return bStatReqMet;
}


//D2Common.0x6FD99740 (#10756)
BOOL __stdcall ITEMS_CheckRequirements(D2UnitStrc* pItem, D2UnitStrc* pUnit, BOOL bEquipping, BOOL* bStrength, BOOL* bDexterity, BOOL* bLevel)
{
	if (bStrength)
	{
		*bStrength = FALSE;
	}

	if (bDexterity)
	{
		*bDexterity = FALSE;
	}

	if (bLevel)
	{
		*bLevel = FALSE;
	}

	if (pItem == NULL || pItem->dwUnitType != UNIT_ITEM)
	{
		return FALSE;
	}


	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	if (!pItemsTxtRecord)
	{
		return FALSE;
	}


	BOOL bIdentified = FALSE;
	const D2ItemDataStrc* pItemData = pItem->pItemData;
	if (pItemData)
	{
		bIdentified = pItemData->dwItemFlags & IFLAG_IDENTIFIED;
	}
	else
	{
		bIdentified = FALSE;
	}

	const int nBaseReqStr = pItemsTxtRecord->wReqStr;
	const int nBaseReqDex = pItemsTxtRecord->wReqDex;


	int nReqStrBonus = 0;
	int nReqDexBonus = 0;
	if (int nReqPctBonus = STATLIST_UnitGetItemStatOrSkillStatValue(pItem, STAT_ITEM_REQ_PERCENT, 0))
	{
		// It seems the original game has some additional logic to handle overflow here
		nReqStrBonus = nBaseReqStr * int64_t(nReqPctBonus) / 100;
		nReqDexBonus = nBaseReqDex * int64_t(nReqPctBonus) / 100;
	}

	if (pItemData && pItemData->dwItemFlags & IFLAG_ETHEREAL)
	{
		nReqStrBonus -= 10;
		nReqDexBonus -= 10;
	}

	const BOOL bStrReqMet = ITEMS_CheckStatRequirement(pItem, pUnit, STAT_STRENGTH, bEquipping, nBaseReqStr + nReqStrBonus);
	const BOOL bDexReqMet = ITEMS_CheckStatRequirement(pItem, pUnit, STAT_DEXTERITY, bEquipping, nBaseReqDex + nReqDexBonus);

	BOOL bLevelReqMet = FALSE;
	const int nLevelReq = ITEMS_GetLevelRequirement(pItem, pUnit);
	bLevelReqMet = nLevelReq == -1 || STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0) >= nLevelReq;

	if (bStrength)
	{
		*bStrength = bStrReqMet;
	}

	if (bDexterity)
	{
		*bDexterity = bDexReqMet;
	}

	if (bLevel)
	{
		*bLevel = bLevelReqMet;
	}

	if (bStrReqMet && bDexReqMet && bLevelReqMet && bIdentified)
	{
		if (pItemsTxtRecord->wType[0] == ITEMTYPE_BOOK && STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0) <= 0)
		{
			return FALSE;
		}

		const int nItemType = ITEMS_GetItemTypeFromItemId(pItem->dwClassId);
		const D2ItemTypesTxt* pItemTypesTxtRecord = DATATBLS_GetItemTypesTxtRecord(nItemType);

		if (!pItemTypesTxtRecord || pItemTypesTxtRecord->nClass >= NUMBER_OF_PLAYERCLASSES)
		{
			return TRUE;
		}

		const int nClassReq = pItemTypesTxtRecord->nClass;

		if (!pUnit)
		{
			return FALSE;
		}

		if (pUnit->dwUnitType == UNIT_MONSTER && nClassReq == PCLASS_BARBARIAN)
		{
			const int nMonsterId = pUnit->dwClassId;
			if (nMonsterId >= MONSTER_ACT5HIRE1 && nMonsterId <= MONSTER_ACT5HIRE2)
			{
				return TRUE;
			}
		}

		if (pUnit->dwUnitType == UNIT_PLAYER && nClassReq == pUnit->dwClassId)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD99BC0 (#10741)
BOOL __stdcall ITEMS_GetQuestFromItemId(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	if (ITEMS_CheckItemTypeIdByItemId(nItemId, ITEMTYPE_ANY_ARMOR) || ITEMS_CheckItemTypeIdByItemId(nItemId, ITEMTYPE_WEAPON))
	{
		if (pItemsTxtRecord->dwUberCode == pItemsTxtRecord->dwCode || pItemsTxtRecord->dwUltraCode == pItemsTxtRecord->dwCode)
		{
			if (pItemsTxtRecord->wType[0] != ITEMTYPE_MISSILE_POTION)
			{
				return !pItemsTxtRecord->nQuest;
			}
		}
	}

	return 0;
}

//D2Common.0x6FD99C60 (#10742)
BOOL __stdcall ITEMS_GetQuest(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	return ITEMS_GetQuestFromItemId(pItem->dwClassId);
}

//D2Common.0x6FD99D40 (#10743)
uint32_t __stdcall ITEMS_GetNormalCode(D2UnitStrc* pItem)
{

	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	if (pItemsTxtRecord->dwNormCode)
	{
		return pItemsTxtRecord->dwNormCode;
	}
	else
	{
		return pItemsTxtRecord->dwCode;
	}
}

//D2Common.0x6FD99DB0
int __fastcall ITEMS_GetRequiredLevel(D2UnitStrc* pItem, D2UnitStrc* pPlayer)
{
	D2UniqueItemsTxt* pUniqueItemsTxtRecord = NULL;
	D2MagicAffixTxt* pAutoAffixTxtRecord = NULL;
	D2MagicAffixTxt* pPrefixTxtRecord = NULL;
	D2MagicAffixTxt* pSuffixTxtRecord = NULL;
	D2SetItemsTxt* pSetItemsTxtRecord = NULL;
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nRequiredLevelForSocketables = 0;
	int nRequiredLevelForSkill = 0;
	int nLevelRequirement = 0;
	int nRequiredLevel = 0;
	int nCraftBonus = 0;
	D2StatStrc pStat[64] = {};
	
	D2ItemDataStrc* pItemData = ITEMS_GetItemData(pItem);
	if (!pItemData)
	{
		return 0;
	}

	nRequiredLevel = 0;
	switch (pItemData->dwQualityNo)
	{
	case ITEMQUAL_MAGIC:
		pPrefixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wMagicPrefix[0]);
		pSuffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wMagicSuffix[0]);
		pAutoAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wAutoAffix);

		if (pPrefixTxtRecord)
		{
			nLevelRequirement = pPrefixTxtRecord->nClass != -1 && pPlayer && pPrefixTxtRecord->nClass == pPlayer->dwClassId ? pPrefixTxtRecord->nClassLevelReq : pPrefixTxtRecord->nLevelReq;
			if (nLevelRequirement >= 0)
			{
				nRequiredLevel = nLevelRequirement;
			}
		}

		if (pSuffixTxtRecord)
		{
			nLevelRequirement = pSuffixTxtRecord->nClass != -1 && pPlayer && pSuffixTxtRecord->nClass == pPlayer->dwClassId ? pSuffixTxtRecord->nClassLevelReq : pSuffixTxtRecord->nLevelReq;
			if (nRequiredLevel <= nLevelRequirement)
			{
				nRequiredLevel = nLevelRequirement;
			}
		}

		if (pAutoAffixTxtRecord)
		{
			nLevelRequirement = pAutoAffixTxtRecord->nClass != -1 && pPlayer && pAutoAffixTxtRecord->nClass == pPlayer->dwClassId ? pAutoAffixTxtRecord->nClassLevelReq : pAutoAffixTxtRecord->nLevelReq;
			if (nRequiredLevel <= nLevelRequirement)
			{
				nRequiredLevel = nLevelRequirement;
			}
		}
		break;
	case ITEMQUAL_RARE:
		for(int i = 0; i < ITEMS_MAX_MODS; ++i)
		{
			pPrefixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wMagicPrefix[i]);
			pSuffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wMagicSuffix[i]);

			if (pPrefixTxtRecord)
			{
				nLevelRequirement = pPrefixTxtRecord->nClass != -1 && pPlayer && pPrefixTxtRecord->nClass == pPlayer->dwClassId ? pPrefixTxtRecord->nClassLevelReq : pPrefixTxtRecord->nLevelReq;
				if (nRequiredLevel <= nLevelRequirement)
				{
					nRequiredLevel = nLevelRequirement;
				}
			}

			if (pSuffixTxtRecord)
			{
				nLevelRequirement = pSuffixTxtRecord->nClass != -1 && pPlayer && pSuffixTxtRecord->nClass == pPlayer->dwClassId ? pSuffixTxtRecord->nClassLevelReq : pSuffixTxtRecord->nLevelReq;
				if (nRequiredLevel <= nLevelRequirement)
				{
					nRequiredLevel = nLevelRequirement;
				}
			}
		}

		pAutoAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wAutoAffix);
		if (pAutoAffixTxtRecord)
		{
			nLevelRequirement = pAutoAffixTxtRecord->nClass != -1 && pPlayer && pAutoAffixTxtRecord->nClass == pPlayer->dwClassId ? pAutoAffixTxtRecord->nClassLevelReq : pAutoAffixTxtRecord->nLevelReq;
			if (nRequiredLevel <= nLevelRequirement)
			{
				nRequiredLevel = nLevelRequirement;
			}
		}
		break;
	case ITEMQUAL_CRAFT:
		nCraftBonus = 10;
		for (int i = 0; i < ITEMS_MAX_MODS; ++i)
		{
			pPrefixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wMagicPrefix[i]);
			pSuffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wMagicSuffix[i]);

			if (pPrefixTxtRecord)
			{
				nCraftBonus += 3;
				nLevelRequirement = pPrefixTxtRecord->nClass != -1 && pPlayer && pPrefixTxtRecord->nClass == pPlayer->dwClassId ? pPrefixTxtRecord->nClassLevelReq : pPrefixTxtRecord->nLevelReq;
				if (nRequiredLevel <= nLevelRequirement)
				{
					nRequiredLevel = nLevelRequirement;
				}
			}

			if (pSuffixTxtRecord)
			{
				nCraftBonus += 3;
				nLevelRequirement = pSuffixTxtRecord->nClass != -1 && pPlayer && pSuffixTxtRecord->nClass == pPlayer->dwClassId ? pSuffixTxtRecord->nClassLevelReq : pSuffixTxtRecord->nLevelReq;
				if (nRequiredLevel <= nLevelRequirement)
				{
					nRequiredLevel = nLevelRequirement;
				}
			}
		}

		nRequiredLevel += nCraftBonus;

		if (nRequiredLevel >= DATATBLS_GetMaxLevel(0))
		{
			nRequiredLevel = DATATBLS_GetMaxLevel(0) - 1;
		}
		break;
	case ITEMQUAL_UNIQUE:
		if (pItemData->dwFileIndex >= 0 && pItemData->dwFileIndex < sgptDataTables->nUniqueItemsTxtRecordCount)
		{
			pUniqueItemsTxtRecord = &sgptDataTables->pUniqueItemsTxt[pItemData->dwFileIndex];
			if (pUniqueItemsTxtRecord)
			{
				if (pPlayer)
				{
					if ((ITEMS_CheckUnitFlagEx(pPlayer, UNITFLAGEX_ISEXPANSION) || ITEMS_GetItemFormat(pItem) >= 1) && (int)pUniqueItemsTxtRecord->wLvlReq >= 0)
					{
						nRequiredLevel = pUniqueItemsTxtRecord->wLvlReq;
					}
				}
				else
				{
					if ((int)pUniqueItemsTxtRecord->wLvlReq >= 0)
					{
						nRequiredLevel = pUniqueItemsTxtRecord->wLvlReq;
					}
				}
			}
		}
		break;
	case ITEMQUAL_SET:
		pSetItemsTxtRecord = ITEMS_GetSetItemsTxtRecord(ITEMS_GetFileIndex(pItem));
		if (pSetItemsTxtRecord && pSetItemsTxtRecord->wLvlReq >= 0)
		{
			nRequiredLevel = pSetItemsTxtRecord->wLvlReq;
		}
		break;
	default:
		break;
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	if (pItemsTxtRecord)
	{
		if (pItemsTxtRecord->nLevelReq > nRequiredLevel)
		{
			nRequiredLevel = pItemsTxtRecord->nLevelReq;
		}
	}

	if (pItem->pInventory)
	{
		for (D2UnitStrc* i = INVENTORY_GetFirstItem(pItem->pInventory); i; i = INVENTORY_GetNextItem(i))
		{
			if (!INVENTORY_UnitIsItem(i))
			{
				break;
			}

			nRequiredLevelForSocketables = ITEMS_GetRequiredLevel(i, pPlayer);
			if (nRequiredLevel <= nRequiredLevelForSocketables)
			{
				nRequiredLevel = nRequiredLevelForSocketables;
			}
		}
	}

	for (int i = 0; i < STATLIST_CopyStats(pItem, STAT_ITEM_SINGLESKILL, pStat, ARRAY_SIZE(pStat)); ++i)
	{
		pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(pStat[i].nLayer);
		if (pSkillsTxtRecord)
		{
			if (pSkillsTxtRecord->wReqLevel > nRequiredLevel)
			{
				nRequiredLevel = pSkillsTxtRecord->wReqLevel;
			}
		}
	}

	for (int i = 0; i < STATLIST_CopyStats(pItem, STAT_ITEM_NONCLASSSKILL, pStat, ARRAY_SIZE(pStat)); ++i)
	{
		pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(pStat[i].nLayer);
		if (pSkillsTxtRecord)
		{
			if (!pPlayer || pPlayer->dwUnitType || pSkillsTxtRecord->nCharClass < 0 || pSkillsTxtRecord->nCharClass >= 7 || (pPlayer->dwClassId & 0xFF) != pSkillsTxtRecord->nCharClass)
			{
				nRequiredLevelForSkill = pSkillsTxtRecord->wReqLevel + 6;
			}
			else
			{
				nRequiredLevelForSkill = pSkillsTxtRecord->wReqLevel;
			}

			if (nRequiredLevelForSkill > nRequiredLevel)
			{
				nRequiredLevel = nRequiredLevelForSkill;
			}
		}
	}

	nRequiredLevel += STATLIST_UnitGetStatValue(pItem, STAT_ITEM_LEVELREQ, 0);
	if (nRequiredLevel <= 0)
	{
		nRequiredLevel = 0;
	}

	return nRequiredLevel;
}

//D2Common.0x6FD9A3F0 (#10757)
int __stdcall ITEMS_GetLevelRequirement(D2UnitStrc* pItem, D2UnitStrc* pUnit)
{
	return ITEMS_GetRequiredLevel(pItem, pUnit);
}

//D2Common.0x6FD9A400 (#10758)
BOOL __stdcall ITEMS_CheckBodyLocation(D2UnitStrc* pItem, uint8_t nBodyLoc)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	uint8_t nBodyLoc1 = 0;
	uint8_t nBodyLoc2 = 0;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				nBodyLoc1 = pItemTypesTxtRecord->nBodyLoc1;
				nBodyLoc2 = pItemTypesTxtRecord->nBodyLoc2;
			}
		}

		if (nBodyLoc1 == nBodyLoc || nBodyLoc2 == nBodyLoc
			|| (nBodyLoc == BODYLOC_SWRARM || nBodyLoc == BODYLOC_SWLARM) && (nBodyLoc1 == BODYLOC_RARM || nBodyLoc2 == BODYLOC_RARM || nBodyLoc1 == BODYLOC_LARM || nBodyLoc2 == BODYLOC_LARM))
		{
			return TRUE;
		}
	}
	else
	{
		FOG_Trace(" ----- JONM NOTE: Would have crashed, see code at ITEMSCheckBodyLocation. From FILE: %s  LINE: %d", __FILE__, __LINE__);
	}

	return FALSE;
}

//D2Common.0x6FD9A4F0 (#10762)
int __stdcall ITEMS_CheckItemTypeIfThrowable(int nItemType)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;

	if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
	{
		pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
		if (pItemTypesTxtRecord)
		{
			return pItemTypesTxtRecord->nThrowable;
		}
	}

	return 0;
}

//D2Common.0x6FD9A530 (#10759)
int __stdcall ITEMS_CheckIfThrowable(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		return ITEMS_CheckItemTypeIfThrowable(pItemsTxtRecord->wType[0]);
	}

	return 0;
}

//D2Common.0x6FD9A5B0 (#10760)
int __stdcall ITEMS_GetMissileType(D2UnitStrc* pItem)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		return DATATBLS_GetItemsTxtRecord(pItem->dwClassId)->wMissileType;
	}

	return -1;
}

//D2Common.0x6FD9A5E0 (#10761)
uint8_t __stdcall ITEMS_GetMeleeRange(D2UnitStrc* pItem)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		return DATATBLS_GetItemsTxtRecord(pItem->dwClassId)->nRangeAdder;
	}

	return 0;
}

//D2Common.0x6FD9A610 (#10763)
BOOL __stdcall ITEMS_CheckWeaponClassByItemId(int nItemId, int nWeapClass)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->dwWeapClass == nWeapClass;
}

//D2Common.0x6FD9A660 (#10764)
BOOL __stdcall ITEMS_CheckWeaponClass(D2UnitStrc* pItem, int nWeapClass)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		return ITEMS_CheckWeaponClassByItemId(pItem->dwClassId, nWeapClass);
	}

	return 0;
}

//D2Common.0x6FD9A6C0 (#10766)
uint32_t __stdcall ITEMS_CheckWeaponIfTwoHandedByItemId(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->n2Handed;
}

//D2Common.0x6FD9A700 (#10765)
uint32_t __stdcall ITEMS_CheckWeaponIfTwoHanded(D2UnitStrc* pItem)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		return ITEMS_CheckWeaponIfTwoHandedByItemId(pItem->dwClassId);
	}

	return 0;
}

//D2Common.0x6FD9A750 (#10767)
uint32_t __stdcall ITEMS_CheckIfStackable(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		return pItemsTxtRecord->nStackable;
	}

	return 0;
}

//D2Common.0x6FD9A7A0 (#10768)
uint32_t __stdcall ITEMS_CheckIfBeltable(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				return pItemTypesTxtRecord->nBeltable;
			}
		}
	}

	return 0;
}

//D2Common.0x6FD9A820 (#10769)
BOOL __stdcall ITEMS_ComparePotionTypes(D2UnitStrc* pItem1, D2UnitStrc* pItem2)
{
	static const int szHealthPotCodes[] =
	{
		' 1ph', ' 2ph', ' 3ph', ' 4ph', ' 5ph'
	};

	static const int szManaPotCodes[] =
	{
		' 1pm', ' 2pm', ' 3pm', ' 4pm', ' 5pm'
	};

	static const int szRejuvPotCodes[] =
	{
		' lvr', ' svr'
	};

	D2ItemsTxt* pItemsTxtRecord1 = NULL;
	D2ItemsTxt* pItemsTxtRecord2 = NULL;
	int nCounter = 0;
	bool bPot1 = false;
	bool bPot2 = false;

	if (pItem1 && pItem2)
	{
		if (pItem1->dwClassId == pItem2->dwClassId)
		{
			return TRUE;
		}

		pItemsTxtRecord1 = DATATBLS_GetItemsTxtRecord(pItem1->dwClassId);
		pItemsTxtRecord2 = DATATBLS_GetItemsTxtRecord(pItem2->dwClassId);

		bPot1 = false;
		bPot2 = false;
		nCounter = 0;
		do
		{
			if (!bPot1 && pItemsTxtRecord1->dwCode == szHealthPotCodes[nCounter])
			{
				bPot1 = true;
			}

			if (!bPot2 && pItemsTxtRecord2->dwCode == szHealthPotCodes[nCounter])
			{
				bPot2 = true;
			}

			if (bPot1 && bPot2)
			{
				return TRUE;
			}

			++nCounter;
		}
		while (nCounter < ARRAY_SIZE(szHealthPotCodes));


		bPot1 = false;
		bPot2 = false;
		nCounter = 0;
		do
		{
			if (!bPot1 && pItemsTxtRecord1->dwCode == szManaPotCodes[nCounter])
			{
				bPot1 = true;
			}

			if (!bPot2 && pItemsTxtRecord2->dwCode == szManaPotCodes[nCounter])
			{
				bPot2 = true;
			}

			if (bPot1 && bPot2)
			{
				return TRUE;
			}

			++nCounter;
		}
		while (nCounter < ARRAY_SIZE(szManaPotCodes));


		bPot1 = false;
		bPot2 = false;
		nCounter = 0;
		do
		{
			if (!bPot1 && pItemsTxtRecord1->dwCode == szRejuvPotCodes[nCounter])
			{
				bPot1 = true;
			}

			if (!bPot2 && pItemsTxtRecord2->dwCode == szRejuvPotCodes[nCounter])
			{
				bPot2 = true;
			}

			if (bPot1 && bPot2)
			{
				return TRUE;
			}

			++nCounter;
		}
		while (nCounter < ARRAY_SIZE(szRejuvPotCodes));

	}

	return FALSE;
}

//D2Common.0x6FD9A960 (#10770)
BOOL __stdcall ITEMS_CheckIfAutoBeltable(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);
	return pItemsTxtRecord->nAutoBelt || INVENTORY_HasSimilarPotionInBelt(pInventory, pItem) != -1;
}

//D2Common.0x6FD9AA00 (#10771)
BOOL __stdcall ITEMS_CheckIfUseable(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		return pItemsTxtRecord->nUseable;
	}
	else
	{
		FOG_Trace(" ----- JONM NOTE: Would have crashed, see code at ITEMSIsUseable. From FILE: %s  LINE: %d", __FILE__, __LINE__);
		return 0;
	}
}

//D2Common.0x6FD9AA70 (#10772)
int __stdcall ITEMS_GetUniqueColumnFromItemsTxt(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);
	return pItemsTxtRecord->nUnique;
}

//D2Common.0x6FD9AB00 (#10773)
BOOL __stdcall ITEMS_IsQuestItem(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);
	return pItemsTxtRecord->nQuest;
}

//D2Common.0x6FD9AB90
int __fastcall ITEMS_CalculateAdditionalCostsForChargedSkills(D2UnitStrc* pUnit, int nBaseCost)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	int nCounter = 0;
	int nSkillId = 0;
	int nValue = 0;
	int nStats = 0;
	int nCost = 0;
	int nBase = 0;
	int nAdd = 0;
	int nMax = 0;
	D2StatStrc pStat[64] = {};

	nStats = STATLIST_CopyStats(pUnit, STAT_ITEM_CHARGED_SKILL, pStat, ARRAY_SIZE(pStat));
	if (nStats > 0)
	{
		do
		{
			nSkillId = pStat[nCounter].nLayer >> sgptDataTables->nStuff;

			nMax = pStat[nCounter].nValue >> 8;
			nValue = pStat[nCounter].nValue & 0xFF;

			if (nValue < nMax)
			{
				pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
				if (pSkillsTxtRecord)
				{
					nBase = nBaseCost * ((pStat[nCounter].nLayer & sgptDataTables->nShiftedStuff) + 2 * pSkillsTxtRecord->wReqLevel / 6 + 2);
					if (nBase > 65535 && pSkillsTxtRecord->nCostMult)
					{
						nAdd = pSkillsTxtRecord->nCostMult * nBase / 1024;
					}
					else
					{
						nAdd = nBase * pSkillsTxtRecord->nCostMult / 1024;
					}

					nCost += (pSkillsTxtRecord->nCostAdd + nAdd) * (nMax - nValue) / nMax;
				}
			}
			++nCounter;
		}
		while (nCounter < nStats);

		return nCost;
	}

	return 0;
}

//D2Common.0x6FD9ACE0
void __fastcall ITEMS_CalculateAdditionalCostsForBonusStats(D2UnitStrc* pItem, int* pSellCost, int* pBuyCost, int* pRepCost, unsigned int nDivisor)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	unsigned int nSell = 0;
	unsigned int nBuy = 0;
	unsigned int nRep = 0;
	int nBonusValue = 0;
	int nSkillId = 0;
	int nCounter = 0;
	int nStatId = 0;
	int nStats = 0;
	int nValue = 0;
	int nItemPeriodOfDay = 0;
	int nMinValue = 0;
	int nMaxValue = 0;
	D2StatStrc pStat[511] = {};

	nStats = STATLIST_GetFullStatsDataFromUnit(pItem, pStat, ARRAY_SIZE(pStat));

	while (nCounter < nStats)
	{
		nStatId = pStat[nCounter].nStat;
		nBonusValue = STATLIST_GetUnitStatBonus(pItem, nStatId, pStat[nCounter].nLayer);

		if (nBonusValue)
		{
			pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
			if (pItemStatCostTxtRecord)
			{
				if (pItemStatCostTxtRecord->nValShift)
				{
					nBonusValue >>= pItemStatCostTxtRecord->nValShift;
				}

				switch (pItemStatCostTxtRecord->nEncode)
				{
				case 1:
					pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(pStat[nCounter].nLayer);
					if (pSkillsTxtRecord)
					{
						if (nBonusValue * *pSellCost > 65535 && pSkillsTxtRecord->nCostMult)
						{
							nSell += pSkillsTxtRecord->nCostAdd + pSkillsTxtRecord->nCostMult * nBonusValue * *pSellCost / 1024;
							nBuy += pSkillsTxtRecord->nCostAdd + pSkillsTxtRecord->nCostMult * nBonusValue * *pBuyCost / 4096;
							nRep += pSkillsTxtRecord->nCostAdd + pSkillsTxtRecord->nCostMult * nBonusValue * *pRepCost / 1024;
						}
						else
						{
							nSell += pSkillsTxtRecord->nCostAdd + nBonusValue * *pSellCost * pSkillsTxtRecord->nCostMult / 1024;
							nBuy += pSkillsTxtRecord->nCostAdd + nBonusValue * pSkillsTxtRecord->nCostMult * *pBuyCost / 4096;
							nRep += pSkillsTxtRecord->nCostAdd + nBonusValue * pSkillsTxtRecord->nCostMult * *pRepCost / 1024;
						}
					}
					break;
				case 2:
				case 3:
					nSkillId = pStat[nCounter].nLayer >> sgptDataTables->nStuff;
					nValue = pStat[nCounter].nLayer & sgptDataTables->nShiftedStuff;

					pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

					if (nValue * *pSellCost > 65535 && pSkillsTxtRecord->nCostMult)
					{
						nSell += pSkillsTxtRecord->nCostAdd + pSkillsTxtRecord->nCostMult * nValue * *pSellCost / 1024;
						nBuy += pSkillsTxtRecord->nCostAdd + pSkillsTxtRecord->nCostMult * nValue * *pBuyCost / 4096;
						nRep += pSkillsTxtRecord->nCostAdd + pSkillsTxtRecord->nCostMult * nValue * *pRepCost / 1024;
					}
					else
					{
						nSell += pSkillsTxtRecord->nCostAdd + nValue * *pSellCost * pSkillsTxtRecord->nCostMult / 1024;
						nBuy += pSkillsTxtRecord->nCostAdd + nValue * pSkillsTxtRecord->nCostMult * *pBuyCost / 4096;
						nRep += pSkillsTxtRecord->nCostAdd + nValue * pSkillsTxtRecord->nCostMult * *pRepCost / 1024;
					}
					break;
				case 4:
					ITEMMODS_GetByTimeAdjustment(nBonusValue, 0, 0, &nItemPeriodOfDay, &nMinValue, &nMaxValue);

					nValue = (nMinValue + nMaxValue) / 2;

					if (nValue * *pSellCost > 65535 && pItemStatCostTxtRecord->dwMultiply)
					{
						nSell += pItemStatCostTxtRecord->dwAdd + pItemStatCostTxtRecord->dwMultiply * nValue * *pSellCost / 1024;
						nBuy += pItemStatCostTxtRecord->dwAdd + pItemStatCostTxtRecord->dwMultiply * nValue * *pBuyCost / 1024;
						nRep += pItemStatCostTxtRecord->dwAdd + pItemStatCostTxtRecord->dwMultiply * nValue * *pRepCost / 1024;
					}
					else
					{
						nSell += pItemStatCostTxtRecord->dwAdd + nValue * *pSellCost * pItemStatCostTxtRecord->dwMultiply / 1024;
						nBuy += pItemStatCostTxtRecord->dwAdd + nValue * pItemStatCostTxtRecord->dwMultiply * *pBuyCost / 1024;
						nRep += pItemStatCostTxtRecord->dwAdd + nValue * pItemStatCostTxtRecord->dwMultiply * *pRepCost / 1024;
					}
					break;
				default:
					if (nBonusValue * *pSellCost > 65535 && pItemStatCostTxtRecord->dwMultiply)
					{
						nSell += pItemStatCostTxtRecord->dwAdd + pItemStatCostTxtRecord->dwMultiply * nBonusValue * *pSellCost / 1024;
						nBuy += pItemStatCostTxtRecord->dwAdd + pItemStatCostTxtRecord->dwMultiply * nBonusValue * *pBuyCost / 1024;
						nRep += pItemStatCostTxtRecord->dwAdd + pItemStatCostTxtRecord->dwMultiply * nBonusValue * *pRepCost / 1024;
					}
					else
					{
						nSell += pItemStatCostTxtRecord->dwAdd + nBonusValue * *pSellCost * pItemStatCostTxtRecord->dwMultiply / 1024;
						nBuy += pItemStatCostTxtRecord->dwAdd + nBonusValue * pItemStatCostTxtRecord->dwMultiply * *pBuyCost / 1024;
						nRep += pItemStatCostTxtRecord->dwAdd + nBonusValue * pItemStatCostTxtRecord->dwMultiply * *pRepCost / 1024;
					}
					break;
				}
			}
		}

		++nCounter;
	}

	*pSellCost += nSell / nDivisor;
	*pBuyCost += nBuy / nDivisor;
	*pRepCost += nRep / nDivisor;
}

//TODO: Check Calculations for unnamed variables, contains some small (rounding?) errors
//D2Common.0x6FD9B1C0
int __fastcall ITEMS_CalculateTransactionCost(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int nDifficulty, D2BitBufferStrc* pQuestFlags, int nVendorId, int nTransactionType)
{
	D2UniqueItemsTxt* pUniqueItemsTxtRecord = NULL;
	D2MagicAffixTxt* pMagicAffixTxtRecord = NULL;
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2SetItemsTxt* pSetItemsTxtRecord = NULL;
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	D2ItemsTxt* pUltraItemsTxtRecord = NULL;
	D2ItemsTxt* pUberItemsTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2BooksTxt* pBooksTxtRecord = NULL;
	D2NpcTxt* pNpcTxtRecord = NULL;
	D2UnitStrc* pSocketable = NULL;
	int nSocketableCost = 0;
	int nReducePricePct = 0;
	int nStackQuantity = 0;
	int nArmorClass = 0;
	int nDurability = 0;
	int nFileIndex = 0;
	int nUltraCost = 0;
	int nUberCost = 0;
	int nItemType = 0;
	int nQuantity = 0;
	int nMaxStack = 0;
	int nSellCost = 0;
	int nBuyCost = 0;
	int nRepCost = 0;
	int nDivisor = 0;
	int nMaxDura = 0;
	int nQuality = 0;
	int nItemId = 0;
	int nLevel = 0;
	int nStack = 0;
	int nCost = 0;
	int nBase = 0;
	int nSell = 0;
	int nBuy = 0;
	int nRep = 0;

	int v30 = 0, v27 = 0, v22 = 0, v23 = 0; //TODO: Rename
	D2ItemDataStrc* pItemData = ITEMS_GetItemData(pItem);
	if (!pPlayer || !pItemData)
	{
		return 0x7FFFFFFF;
	}

	nLevel = STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0);

	if (nTransactionType == 3 && !ITEMS_IsRepairable(pItem))
	{
		return 0;
	}

	if (pItemData->dwItemFlags & IFLAG_STARTITEM)
	{
		return 1;
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	nQuantity = STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0);
	if (nQuantity <= 0)
	{
		nQuantity = 1;
	}
	
	nReducePricePct = STATLIST_UnitGetStatValue(pPlayer, STAT_ITEM_REDUCEDPRICES, 0);
	if (nReducePricePct >= 99)
	{
		nReducePricePct = 99;
	}

	if (nTransactionType != 2)
	{
		nSellCost = 0;
		nBuyCost = 0;
		nRepCost = 0;
		nDivisor = 1;

		if (pItemData->dwItemFlags & IFLAG_ISEAR)
		{
			nSellCost = pItemsTxtRecord->dwCost * pItemData->nEarLvl;
			nBuyCost = nSellCost;
		}
		else
		{
			if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BODY_PART))
			{
				nFileIndex = pItemData->dwFileIndex;
				pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nFileIndex);
				if (pMonStatsTxtRecord)
				{
					nSellCost = 8 * pMonStatsTxtRecord->nLevel[nDifficulty];
				}

				nSellCost += pItemsTxtRecord->dwCost;
				nBuyCost = nSellCost;
			}
			else
			{
				if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
				{
					pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(pItemData->wMagicSuffix[0]);
					D2_ASSERT(pBooksTxtRecord);

					nSellCost = pItemsTxtRecord->dwCost + nQuantity * pBooksTxtRecord->dwCostPerCharge;
					nBuyCost = nSellCost;
				}
				else
				{
					pItemTypesTxtRecord = DATATBLS_GetItemTypesTxtRecord(ITEMS_GetItemTypeFromItemId(pItem->dwClassId));

					if (pItemTypesTxtRecord && pItemTypesTxtRecord->wQuiver)
					{
						nSellCost = nQuantity * pItemsTxtRecord->dwCost / 1024;
						nBuyCost = nSellCost;

						nStackQuantity = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_EXTRA_STACK, 0) + pItemsTxtRecord->dwMaxStack;
						if (nStackQuantity >= 511)
						{
							nStackQuantity = 511;
						}

						nRepCost = nStackQuantity * pItemsTxtRecord->dwCost / 1024;
					}
					else
					{
						if (!pItemsTxtRecord->nStackable)
						{
							nSellCost = pItemsTxtRecord->dwCost;
							nBuyCost = nSellCost;
							nRepCost = nSellCost;
						}
						else
						{
							nSellCost = pItemsTxtRecord->dwCost;
							nBuyCost = nSellCost;
							nRepCost = nSellCost;

							nDivisor = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_EXTRA_STACK, 0) + pItemsTxtRecord->dwMaxStack;

							if (nDivisor < 511)
							{
								if (nDivisor <= 1)
								{
									nDivisor = 1;
								}
							}
							else
							{
								nDivisor = 511;
							}
						}
					}
				}
			}
		}

		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_ARMOR))
		{
			nArmorClass = STATLIST_GetUnitBaseStat(pItem, STAT_ARMORCLASS, 0);
			if (pItemsTxtRecord->dwMaxAc - pItemsTxtRecord->dwMinAc != -1)
			{
				if (pItemsTxtRecord->dwMaxAc)
				{
					nSellCost = nArmorClass * pItemsTxtRecord->dwCost / pItemsTxtRecord->dwMaxAc;
					nBuyCost = nArmorClass * pItemsTxtRecord->dwCost / pItemsTxtRecord->dwMaxAc;
					nRepCost = nArmorClass * pItemsTxtRecord->dwCost / pItemsTxtRecord->dwMaxAc;
				}
			}
		}

		nQuality = pItemData->dwQualityNo;

		if (nQuality < ITEMQUAL_MAGIC || nQuality > ITEMQUAL_TEMPERED)
		{
			ITEMS_CalculateAdditionalCostsForItemSkill(pItem, &nSellCost, &nBuyCost, &nRepCost, nDivisor);
		}

		if (pItemData->dwItemFlags & IFLAG_IDENTIFIED)
		{
			pMagicAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wAutoAffix);
			if (pMagicAffixTxtRecord)
			{
				if (nSellCost > 65535 && pMagicAffixTxtRecord->dwMultiply)
				{
					nSell = pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nSellCost / 1024;
					nBuy = pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nBuyCost / 1024;
					nRep = pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nRepCost / 1024;
				}
				else
				{
					nSell = pMagicAffixTxtRecord->dwAdd + nSellCost * pMagicAffixTxtRecord->dwMultiply / 1024;
					nBuy = pMagicAffixTxtRecord->dwAdd + nBuyCost * pMagicAffixTxtRecord->dwMultiply / 1024;
					nRep = pMagicAffixTxtRecord->dwAdd + nRepCost * pMagicAffixTxtRecord->dwMultiply / 1024;
				}
			}

			switch (nQuality)
			{
			case ITEMQUAL_RARE:
			case ITEMQUAL_CRAFT:
				for (int i = 0; i < ITEMS_MAX_MODS; ++i)
				{
					pMagicAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wMagicPrefix[i]);
					if (pMagicAffixTxtRecord)
					{
						if (nSellCost > 65535 && pMagicAffixTxtRecord->dwMultiply)
						{
							nSell += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nSellCost / 1024;
							nBuy += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nBuyCost / 1024;
							nRep += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nRepCost / 1024;
						}
						else
						{
							nSell += pMagicAffixTxtRecord->dwAdd + nSellCost * pMagicAffixTxtRecord->dwMultiply / 1024;
							nBuy += pMagicAffixTxtRecord->dwAdd + nBuyCost * pMagicAffixTxtRecord->dwMultiply / 1024;
							nRep += pMagicAffixTxtRecord->dwAdd + nRepCost * pMagicAffixTxtRecord->dwMultiply / 1024;
						}
					}

					pMagicAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wMagicSuffix[i]);
					if (pMagicAffixTxtRecord)
					{
						if (nSellCost > 65535 && pMagicAffixTxtRecord->dwMultiply)
						{
							nSell += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nSellCost / 1024;
							nBuy += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nBuyCost / 1024;
							nRep += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nRepCost / 1024;
						}
						else
						{
							nSell += pMagicAffixTxtRecord->dwAdd + nSellCost * pMagicAffixTxtRecord->dwMultiply / 1024;
							nBuy += pMagicAffixTxtRecord->dwAdd + nBuyCost * pMagicAffixTxtRecord->dwMultiply / 1024;
							nRep += pMagicAffixTxtRecord->dwAdd + nRepCost * pMagicAffixTxtRecord->dwMultiply / 1024;
						}
					}
				}

				ITEMS_CalculateAdditionalCostsForBonusStats(pItem, &nSellCost, &nBuyCost, &nRepCost, nDivisor);
				break;

			case ITEMQUAL_UNIQUE:
				if (pItemData->dwFileIndex >= 0 && pItemData->dwFileIndex < sgptDataTables->nUniqueItemsTxtRecordCount)
				{
					pUniqueItemsTxtRecord = &sgptDataTables->pUniqueItemsTxt[pItemData->dwFileIndex];
					if (pUniqueItemsTxtRecord)
					{
						if (nSellCost > 65535 && pUniqueItemsTxtRecord->dwCostMult)
						{
							nSell += pUniqueItemsTxtRecord->dwCostAdd + pUniqueItemsTxtRecord->dwCostMult * nSellCost / 1024;
							nBuy += pUniqueItemsTxtRecord->dwCostAdd + pUniqueItemsTxtRecord->dwCostMult * nBuyCost / 1024;
							nRep += pUniqueItemsTxtRecord->dwCostAdd + pUniqueItemsTxtRecord->dwCostMult * nRepCost / 1024;
						}
						else
						{
							nSell += pUniqueItemsTxtRecord->dwCostAdd + nSellCost * pUniqueItemsTxtRecord->dwCostMult / 1024;
							nBuy += pUniqueItemsTxtRecord->dwCostAdd + nBuyCost * pUniqueItemsTxtRecord->dwCostMult / 1024;
							nRep += pUniqueItemsTxtRecord->dwCostAdd + nRepCost * pUniqueItemsTxtRecord->dwCostMult / 1024;
							break;
						}
					}
				}
				//TODO: Not sure if it was intended to have no break here

			case ITEMQUAL_MAGIC:
				pMagicAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wMagicPrefix[0]);
				if (pMagicAffixTxtRecord)
				{
					if (nSellCost > 65535 && pMagicAffixTxtRecord->dwMultiply)
					{
						nSell += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nSellCost / 1024;
						nBuy += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nBuyCost / 1024;
						nRep += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nRepCost / 1024;
					}
					else
					{
						nSell += pMagicAffixTxtRecord->dwAdd + nSellCost * pMagicAffixTxtRecord->dwMultiply / 1024;
						nBuy += pMagicAffixTxtRecord->dwAdd + nBuyCost * pMagicAffixTxtRecord->dwMultiply / 1024;
						nRep += pMagicAffixTxtRecord->dwAdd + nRepCost * pMagicAffixTxtRecord->dwMultiply / 1024;
					}
				}

				pMagicAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItemData->wMagicSuffix[0]);
				if (pMagicAffixTxtRecord)
				{
					if (nSellCost > 65535 && pMagicAffixTxtRecord->dwMultiply)
					{
						nSell += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nSellCost / 1024;
						nBuy += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nBuyCost / 1024;
						nRep += pMagicAffixTxtRecord->dwAdd + pMagicAffixTxtRecord->dwMultiply * nRepCost / 1024;
					}
					else
					{
						nSell += pMagicAffixTxtRecord->dwAdd + nSellCost * pMagicAffixTxtRecord->dwMultiply / 1024;
						nBuy += pMagicAffixTxtRecord->dwAdd + nBuyCost * pMagicAffixTxtRecord->dwMultiply / 1024;
						nRep += pMagicAffixTxtRecord->dwAdd + nRepCost * pMagicAffixTxtRecord->dwMultiply / 1024;
					}
				}

				ITEMS_CalculateAdditionalCostsForBonusStats(pItem, &nSellCost, &nBuyCost, &nRepCost, nDivisor);
				break;

			case ITEMQUAL_SUPERIOR:
			case ITEMQUAL_TEMPERED:
				ITEMS_CalculateAdditionalCostsForBonusStats(pItem, &nSellCost, &nBuyCost, &nRepCost, nDivisor);
				break;

			case ITEMQUAL_SET:
				if (pItemData->dwFileIndex >= 0 && pItemData->dwFileIndex < sgptDataTables->nSetItemsTxtRecordCount)
				{
					pSetItemsTxtRecord = &sgptDataTables->pSetItemsTxt[pItemData->dwFileIndex];
					if (pSetItemsTxtRecord)
					{
						if (nSellCost > 65535 && pSetItemsTxtRecord->dwCostMult)
						{
							nSell += pSetItemsTxtRecord->dwCostAdd + pSetItemsTxtRecord->dwCostMult * nSellCost / 1024;
							nBuy += pSetItemsTxtRecord->dwCostAdd + pSetItemsTxtRecord->dwCostMult * nBuyCost / 1024;
							nRep += pSetItemsTxtRecord->dwCostAdd + pSetItemsTxtRecord->dwCostMult * nRepCost / 1024;
						}
						else
						{
							nSell += pSetItemsTxtRecord->dwCostAdd + nSellCost * pSetItemsTxtRecord->dwCostMult / 1024;
							nBuy += pSetItemsTxtRecord->dwCostAdd + nBuyCost * pSetItemsTxtRecord->dwCostMult / 1024;
							nRep += pSetItemsTxtRecord->dwCostAdd + nRepCost * pSetItemsTxtRecord->dwCostMult / 1024;
						}
					}
				}
				break;

			case ITEMQUAL_INFERIOR:
				nSell = nSellCost / -2;
				nBuy = nBuyCost / -2;
				nRep = nRepCost / -2;
				break;

			default:
				break;
			}

			nSellCost += nSell / nDivisor;
			nBuyCost += nBuy / nDivisor;
			nRepCost += nRep / nDivisor;

			if (nQuality >= ITEMQUAL_MAGIC && nQuality <= ITEMQUAL_TEMPERED)
			{
				ITEMS_CalculateAdditionalCostsForItemSkill(pItem, &nSellCost, &nBuyCost, &nRepCost, nDivisor);
			}
		}

		if (pItem->pInventory)
		{
			pSocketable = INVENTORY_GetFirstItem(pItem->pInventory);
			while (pSocketable)
			{
				if (INVENTORY_UnitIsItem(pSocketable))
				{
					nSocketableCost = DATATBLS_GetItemsTxtRecord(pSocketable->dwClassId)->dwCost / 2;
					nSellCost += nSocketableCost;
					nBuyCost += nSocketableCost;
					nRepCost += nSocketableCost;
				}
				pSocketable = INVENTORY_GetNextItem(pSocketable);
			}
		}

		if (pItemData->dwItemFlags & IFLAG_ETHEREAL)
		{
			nBuyCost /= 4;
		}

		nItemType = ITEMS_GetItemTypeFromItemId(pItem->dwClassId);
		if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
			if (pItemTypesTxtRecord && pItemTypesTxtRecord->nClass < 7)
			{
				nBuyCost /= 4;
			}
		}

		if (nTransactionType == 1)
		{
			if (pItemData->dwItemFlags & IFLAG_ETHEREAL && !pItemsTxtRecord->nNoDurability && pItemsTxtRecord->nDurability && STATLIST_GetMaxDurabilityFromUnit(pItem) && STATLIST_UnitGetStatValue(pItem, STAT_ITEM_INDESCTRUCTIBLE, 0) <= 0)
			{
				if (STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0) <= 0)
				{
					nBuyCost = 0;
				}
			}
		}
		else if (nTransactionType == 3)
		{
			pItemTypesTxtRecord = DATATBLS_GetItemTypesTxtRecord(pItemsTxtRecord->wType[0]);
			if (!pItemTypesTxtRecord || !pItemTypesTxtRecord->wQuiver || !pItemTypesTxtRecord->nThrowable)
			{
				if (!pItemsTxtRecord->nNoDurability && pItemsTxtRecord->nDurability && STATLIST_GetMaxDurabilityFromUnit(pItem) && STATLIST_UnitGetStatValue(pItem, STAT_ITEM_INDESCTRUCTIBLE, 0) <= 0)
				{
					nDurability = STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0);
					nMaxDura = STATLIST_GetMaxDurabilityFromUnit(pItem);

					if (nMaxDura && nDurability < nMaxDura)
					{
						if (STATLIST_UnitGetStatValue(pItem, STAT_ITEM_REPLENISH_DURABILITY, 0))
						{
							if (nDurability < nMaxDura - 1)
							{
								nRepCost *= (nMaxDura - 1) / nMaxDura;
							}
							else
							{
								nRepCost = 0;
							}
						}
						else
						{
							nRepCost *= (nMaxDura - nDurability) / nMaxDura;
						}
					}
					else
					{
						nRepCost = 0;
					}
				}
			}
		}

		pNpcTxtRecord = DATATBLS_GetNpcTxtRecord(nVendorId);
		D2_ASSERT(pNpcTxtRecord);

		if (nSellCost <= 65535 || !pNpcTxtRecord->dwSellMult)
		{
			nSellCost = nSellCost * pNpcTxtRecord->dwSellMult / 1024;
		}
		else
		{
			nSellCost = pNpcTxtRecord->dwSellMult * nSellCost / 1024;
		}

		if (nBuyCost <= 65535 || !pNpcTxtRecord->dwSellMult)//TODO: Probably Copy-Paste Error by Blizzard, should be dwBuyMult
		{
			nBuyCost = nBuyCost * pNpcTxtRecord->dwBuyMult / 1024;
		}
		else
		{
			nBuyCost = nBuyCost / 1024 * pNpcTxtRecord->dwBuyMult;
		}

		if (nRepCost <= 65535 || !pNpcTxtRecord->dwRepMult)
		{
			nRepCost = nRepCost * pNpcTxtRecord->dwRepMult / 1024;
		}
		else
		{
			nRepCost = pNpcTxtRecord->dwRepMult * nRepCost / 1024;
		}

		for (int i = 0; i < 3; ++i)
		{
			if (pNpcTxtRecord->dwQuestFlag[i] && (QUESTRECORD_GetQuestState(pQuestFlags, pNpcTxtRecord->dwQuestFlag[i], QFLAG_REWARDGRANTED) == 1
												  || QUESTRECORD_GetQuestState(pQuestFlags, pNpcTxtRecord->dwQuestFlag[i], QFLAG_REWARDPENDING) == 1))
			{
				if (nSellCost > 65535 && pNpcTxtRecord->dwQuestSellMult[i])
				{
					nSellCost = pNpcTxtRecord->dwQuestSellMult[i] * nSellCost / 1024;
				}
				else
				{
					nSellCost = nSellCost * pNpcTxtRecord->dwQuestSellMult[i] / 1024;
				}

				if (nBuyCost > 65535 && pNpcTxtRecord->dwQuestBuyMult[i])
				{
					nBuyCost = pNpcTxtRecord->dwQuestBuyMult[i] * nBuyCost / 1024;
				}
				else
				{
					nBuyCost = nBuyCost * pNpcTxtRecord->dwQuestBuyMult[i] / 1024;
				}

				if (nRepCost > 65535 && pNpcTxtRecord->dwQuestRepMult[i])
				{
					nRepCost = pNpcTxtRecord->dwQuestRepMult[i] * nRepCost / 1024;
				}
				else
				{
					nRepCost = nRepCost * pNpcTxtRecord->dwQuestRepMult[i] / 1024;
				}
			}
		}

		if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
		{
			pItemTypesTxtRecord = DATATBLS_GetItemTypesTxtRecord(pItemsTxtRecord->wType[0]);
			if (!pItemTypesTxtRecord || !pItemTypesTxtRecord->wQuiver)
			{
				nSellCost *= nQuantity;

				if (pItemsTxtRecord->nStackable && ITEMS_IsRepairable(pItem))
				{
					nMaxStack = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_EXTRA_STACK, 0) + pItemsTxtRecord->dwMaxStack;
					if (nMaxStack >= 511)
					{
						nMaxStack = 511;
					}

					if (nQuantity < nMaxStack && !STATLIST_UnitGetStatValue(pItem, STAT_ITEM_REPLENISH_QUANTITY, 0))
					{
						nRepCost *= (nMaxStack - nQuantity);
					}
					else
					{
						nRepCost = 0;
					}

					nBuyCost *= (nMaxStack - nRepCost);
				}
				else
				{
					nBuyCost *= nQuantity;
				}
			}
		}

		nCost = nBuyCost;

		if (nTransactionType == 3)
		{
			if (!(pItemData->dwItemFlags & IFLAG_ETHEREAL))
			{
				nRepCost += ITEMS_CalculateAdditionalCostsForChargedSkills(pItem, 10000);
			}
		}

		if (nCost > pNpcTxtRecord->nMaxBuy[nDifficulty])
		{
			nCost = pNpcTxtRecord->nMaxBuy[nDifficulty];
		}

		if (nTransactionType != 1)
		{
			if (nTransactionType == 3)
			{
				nCost = nRepCost - DATATBLS_CalculatePercentage(nRepCost, nReducePricePct, 100);
			}
			else
			{
				nCost = nSellCost - DATATBLS_CalculatePercentage(nSellCost, nReducePricePct, 100);
			}
		}

		if (nCost < 1)
		{
			nCost = 1;
		}

		return nCost;
	}
	else
	{
		if (pItemData->wItemFormat >= 1)
		{
			pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(DATATBLS_GetItemIdFromItemCode(pItemsTxtRecord->dwNormCode ? pItemsTxtRecord->dwNormCode : pItemsTxtRecord->dwCode));
			D2_ASSERT(pItemsTxtRecord);

			if (pItemsTxtRecord->dwCode == ' nir' || pItemsTxtRecord->dwCode == ' uma')
			{
				nCost = pItemsTxtRecord->dwGambleCost;
			}
			else
			{
				nStack = pItemsTxtRecord->dwMinStack + pItemsTxtRecord->dwMaxStack;

				v27 = ((signed int)nStack - HIDWORD(nStack)) >> 1;
				if (v27 < 1)
				{
					v27 = 1;
				}

				if (pItemsTxtRecord->dwUberCode && pItemsTxtRecord->dwUberCode != '   0')
				{
					pUberItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwUberCode, &nItemId);
					if (pUberItemsTxtRecord)
					{
						v30 = 100 * (nLevel - pUberItemsTxtRecord->nLevel);
						v23 = (((signed int)v30 - HIDWORD(v30)) >> 1) + 1;
						if (v23 < 0)
						{
							v23 = 0;
						}
						nUberCost = pUberItemsTxtRecord->dwCost;
					}
				}

				if (pItemsTxtRecord->dwUltraCode && pItemsTxtRecord->dwUltraCode != '   0')
				{
					pUltraItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwUltraCode, &nItemId);
					if (pUltraItemsTxtRecord)
					{
						v30 = 100 * (nLevel - pUltraItemsTxtRecord->nLevel);
						v22 = (((BYTE4(v30) & 3) + (signed int)v30) >> 2) + 1;
						if (v22 < 0)
						{
							v22 = 0;
						}
						nUltraCost = pUltraItemsTxtRecord->dwCost;
					}
				}

				nBase = v23 * nUberCost + v22 * nUltraCost + (v27 * pItemsTxtRecord->dwCost) * (10000 - v22 - v23);
				if (nLevel < 5)
				{
					nLevel = 5;
				}

				nCost = ((2 * nLevel + 1) / 3 + 20) * (nBase / 10000 + (250 * (nLevel + ((pItemsTxtRecord->nLevel - 45) > 0 ? (pItemsTxtRecord->nLevel - 45) : 0) - (int)pItemsTxtRecord->nLevel / 2)) / 3) / 15;
			}

			if (nReducePricePct)
			{
				return nCost - DATATBLS_CalculatePercentage(nCost, nReducePricePct, 100);
			}
			else
			{
				return nCost;
			}
		}
		else
		{
			return DATATBLS_GetItemsTxtRecord(DATATBLS_GetItemIdFromItemCode(pItemsTxtRecord->dwNormCode ? pItemsTxtRecord->dwNormCode : pItemsTxtRecord->dwCode))->dwGambleCost;
		}
	}
}

//D2Common.0x6FD9CB50
void __fastcall ITEMS_CalculateAdditionalCostsForItemSkill(D2UnitStrc* pItem, int* pSellCost, int* pBuyCost, int* pRepCost, unsigned int nDivisor)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	unsigned int nSell = 0;
	unsigned int nBuy = 0;
	unsigned int nRep = 0;
	int nStats = 0;
	D2StatStrc pStat[64] = {};

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord && pItemTypesTxtRecord->nStaffMods != 7)
			{
				nStats = STATLIST_CopyStats(pItem, STAT_ITEM_SINGLESKILL, pStat, ARRAY_SIZE(pStat));

				for (int nStatId = 0; nStatId < nStats; ++nStatId)
				{
					pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(pStat[nStatId].nLayer);
					if (pSkillsTxtRecord)
					{
						if (*pSellCost > 65535 && pSkillsTxtRecord->nCostMult)
						{
							nSell += (2 * pStat[nStatId].nValue - 1) * (pSkillsTxtRecord->nCostAdd + *pSellCost * pSkillsTxtRecord->nCostMult / 1024);
							nBuy += (2 * pStat[nStatId].nValue - 1) * (pSkillsTxtRecord->nCostAdd + *pBuyCost * pSkillsTxtRecord->nCostMult / 4096);
							nRep += (2 * pStat[nStatId].nValue - 1) * (pSkillsTxtRecord->nCostAdd + *pRepCost * pSkillsTxtRecord->nCostMult / 1024);
						}
						else
						{
							nSell += (2 * pStat[nStatId].nValue - 1) * (pSkillsTxtRecord->nCostAdd + pSkillsTxtRecord->nCostMult * *pSellCost / 1024);
							nBuy += (2 * pStat[nStatId].nValue - 1) * (pSkillsTxtRecord->nCostAdd + pSkillsTxtRecord->nCostMult * *pBuyCost / 4096);
							nRep += (2 * pStat[nStatId].nValue - 1) * (pSkillsTxtRecord->nCostAdd + pSkillsTxtRecord->nCostMult * *pRepCost / 1024);
						}
					}
				}

				*pSellCost += nSell / nDivisor;
				*pBuyCost += nBuy / nDivisor;
				*pRepCost += nRep / nDivisor;
			}
		}
	}
}

//D2Common.0x6FD9CDC0
int __fastcall ITEMS_CheckUnitFlagEx(D2UnitStrc* pUnit, int nFlag)
{
	if (pUnit)
	{
		return (nFlag & pUnit->dwFlagEx) != 0;
	}

	return 0;
}

//D2Common.0x6FD9CDE0 (#10775)
int __stdcall ITEMS_GetTransactionCost(D2UnitStrc* pPlayer, D2UnitStrc* pItem, int nDifficulty, D2BitBufferStrc* pQuestFlags, int nVendorId, int nTransactionType)
{
	return ITEMS_CalculateTransactionCost(pPlayer, pItem, nDifficulty, pQuestFlags, nVendorId, nTransactionType);
}

//D2Common.0x6FD9CE10 (#10794)
int __stdcall ITEMS_GetMaxStack(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->dwMaxStack;
}

//D2Common.0x6FD9CE50 (#10795)
int __stdcall ITEMS_GetTotalMaxStack(D2UnitStrc* pItem)
{

	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);

	int nMaxStack = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_EXTRA_STACK, 0) + pItemsTxtRecord->dwMaxStack;
	if (nMaxStack >= 511)
	{
		nMaxStack = 511;
	}

	return nMaxStack;
}

//D2Common.0x6FD9CEF0 (#10798)
int __stdcall ITEMS_GetSpawnStackFromItemId(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->dwSpawnStack;
}

//D2Common.0x6FD9CF30 (#10799)
int __stdcall ITEMS_GetSpawnStack(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	return ITEMS_GetSpawnStackFromItemId(pItem->dwClassId);
}

//D2Common.0x6FD9CFB0 (#10796)
int __stdcall ITEMS_GetMinStackFromItemId(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->dwMinStack;
}

//D2Common.0x6FD9CFF0 (#10797)
int __stdcall ITEMS_GetMinStack(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	return ITEMS_GetMinStackFromItemId(pItem->dwClassId);
}

//D2Common.0x6FD9D070 (#10800) - Unused
int __stdcall ITEMS_CheckBitField1Flag8(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);
	return pItemsTxtRecord->dwBitField1 & 8;
}

//D2Common.0x6FD9D0F0 (#10804)
int __stdcall ITEMS_GetSpellIcon(D2UnitStrc* pItem)
{
	D2BooksTxt* pBooksTxtRecord = NULL;
	uint16_t wBookId = 0;

	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
	{
		if (D2ItemDataStrc* pItemData = ITEMS_GetItemData(pItem))
		{
			wBookId = pItemData->wMagicSuffix[0];
		}
		else
		{
			wBookId = 0;
		}

		pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(wBookId);
		if (pBooksTxtRecord)
		{
			return pBooksTxtRecord->nSpellIcon;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);
		return pItemsTxtRecord->nSpellIcon;
	}
}

//D2Common.0x6FD9D1E0 (#10805)
uint8_t __stdcall ITEMS_GetDurWarnCount(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->nDurWarning;
}

//D2Common.0x6FD9D260 (#10806)
uint8_t __stdcall ITEMS_GetQtyWarnCount(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);
	return pItemsTxtRecord->nQuantityWarning;
}

//D2Common.0x6FD9D2E0 (#10807)
short __stdcall ITEMS_GetStrengthBonus(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		if (pItemsTxtRecord && pItemsTxtRecord->nStrBonus != 1)
		{
			return pItemsTxtRecord->nStrBonus;
		}
	}

	return 100;
}

//D2Common.0x6FD9D310 (#10808)
short __stdcall ITEMS_GetDexBonus(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		if (pItemsTxtRecord && pItemsTxtRecord->nDexBonus != 1)
		{
			return pItemsTxtRecord->nDexBonus;
		}
	}

	return 100;
}

//D2Common.0x6FD9D340 (#10809)
int __stdcall ITEMS_CheckIfSocketableByItemId(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);
	if (pItemsTxtRecord)
	{
		return pItemsTxtRecord->nHasInv;
	}

	return 0;
}

//D2Common.0x6FD9D360 (#10810)
int __stdcall ITEMS_CheckIfSocketable(D2UnitStrc* pItem)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		return ITEMS_CheckIfSocketableByItemId(pItem->dwClassId);
	}

	return 0;
}

//D2Common.0x6FD9D390 (#10811)
BOOL __stdcall ITEMS_HasDurability(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		if (pItemsTxtRecord && !pItemsTxtRecord->nNoDurability && pItemsTxtRecord->nDurability && STATLIST_GetMaxDurabilityFromUnit(pItem))
		{
			return STATLIST_UnitGetStatValue(pItem, STAT_ITEM_INDESCTRUCTIBLE, 0) <= 0;
		}
	}

	return FALSE;
}

//D2Common.0x6FD9D3F0 (#10813)
int __stdcall ITEMS_GetStaffMods(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				return pItemTypesTxtRecord->nStaffMods;
			}
		}
	}

	return 7;
}

//D2Common.0x6FD9D470 (#10814)
uint8_t __stdcall ITEMS_GetAllowedGemSocketsFromItemId(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	if (pItemsTxtRecord)
	{
		return pItemsTxtRecord->nGemSockets;
	}

	return 0;
}

//D2Common.0x6FD9D490 (#10815)
uint8_t __stdcall ITEMS_GetMaxSockets(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	uint8_t nMaxSockets = 0;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				if (pItem->pItemData)
				{
					if (pItem->pItemData->dwItemLevel < 1)
					{
						pItem->pItemData->dwItemLevel = 1;
					}

					if (pItem->pItemData->dwItemLevel > 25)
					{
						if (pItem->pItemData->dwItemLevel > 40)
						{
							nMaxSockets = pItemTypesTxtRecord->nMaxSock40;
						}
						else
						{
							nMaxSockets = pItemTypesTxtRecord->nMaxSock25;
						}
					}
					else
					{
						nMaxSockets = pItemTypesTxtRecord->nMaxSock1;
					}
				}
				else
				{
					nMaxSockets = pItemTypesTxtRecord->nMaxSock1;
				}

				if (pItemsTxtRecord->nGemSockets < nMaxSockets)
				{
					return pItemsTxtRecord->nGemSockets;
				}
				else
				{
					return nMaxSockets;
				}
			}
		}
	}

	return 0;
}

//D2Common.0x6FD9D580 (#10816)
int __stdcall ITEMS_GetSockets(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	return STATLIST_UnitGetStatValue(pItem, STAT_ITEM_NUMSOCKETS, 0);
}

//D2Common.0x6FD9D5E0 (#10817)
void __stdcall ITEMS_AddSockets(D2UnitStrc* pItem, int nSockets)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	uint32_t nQuality = 0;
	int nCurrentSockets = 0;
	int nMaxSockets = 0;
	int nItemLevel = 0;
	int nItemType = 0;
	int nMaxSocks = 0;
	uint8_t nGemSockets = 0;
	uint8_t nHeight = 0;
	uint8_t nWidth = 0;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		nMaxSockets = 6;
		nWidth = 0;
		nHeight = 0;

		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		if (pItemsTxtRecord)
		{
			nWidth = pItemsTxtRecord->nInvWidth;
			nHeight = pItemsTxtRecord->nInvHeight;
		}

		if (nWidth * nHeight <= 6)
		{
			nMaxSockets = nWidth * nHeight;
		}

		if (nWidth * nHeight)
		{
			if (pItem->pItemData)
			{
				nQuality = pItem->pItemData->dwQualityNo;
			}
			else
			{
				nQuality = ITEMQUAL_NORMAL;
			}

			switch (nQuality)
			{
			case ITEMQUAL_SET:
			case ITEMQUAL_UNIQUE:
				nCurrentSockets = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_NUMSOCKETS, 0);
				if (nCurrentSockets <= 0)
				{
					if (nMaxSockets >= 1)
					{
						nMaxSockets = 1;
					}
				}
				else
				{
					nSockets = nCurrentSockets;
				}
				break;
			case ITEMQUAL_RARE:
				if (nMaxSockets >= 2)
				{
					nMaxSockets = 2;
				}
				break;
			case ITEMQUAL_MAGIC:
				if (nMaxSockets >= 4)
				{
					nMaxSockets = 4;
				}
				break;
			case ITEMQUAL_CRAFT:
			case ITEMQUAL_TEMPERED:
				if (nMaxSockets >= 3)
				{
					nMaxSockets = 3;
				}
				break;
			default:
				break;
			}

			nItemType = ITEMS_GetItemTypeFromItemId(pItem->dwClassId);
			if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
			{
				pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
				if (pItemTypesTxtRecord)
				{
					if (pItemsTxtRecord)
					{
						nGemSockets = pItemsTxtRecord->nGemSockets;
					}
					else
					{
						nGemSockets = 0;
					}

					nItemLevel = ITEMS_GetItemLevel(pItem);
					if (nItemLevel > 25)
					{
						if (nItemLevel > 40)
						{
							nMaxSocks = pItemTypesTxtRecord->nMaxSock40;
						}
						else
						{
							nMaxSocks = pItemTypesTxtRecord->nMaxSock25;
						}
					}
					else
					{
						nMaxSocks = pItemTypesTxtRecord->nMaxSock1;
					}

					if (nGemSockets >= nMaxSocks)
					{
						nGemSockets = nMaxSocks;
					}
				}
			}

			if (nMaxSockets >= nGemSockets)
			{
				nMaxSockets = nGemSockets;
			}

			if (nSockets <= 1)
			{
				nSockets = 1;
			}

			if (nSockets >= nMaxSockets)
			{
				nSockets = nMaxSockets;
			}

			if (nSockets > 0)
			{
				if (pItem->pItemData)
				{
					pItem->pItemData->dwItemFlags |= IFLAG_SOCKETED;
				}

				STATLIST_SetUnitStat(pItem, STAT_ITEM_NUMSOCKETS, nSockets, 0);
			}
		}
	}
}

//D2Common.0x6FD9D7C0 (#10818)
void __stdcall ITEMS_SetSockets(D2UnitStrc* pItem, int nSockets)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nMaxSockets = 0;
	int nMaxSocks = 0;
	int nItemLevel = 0;
	int nItemType = 0;
	uint8_t nAllowedSockets = 0;
	uint8_t nWidth = 0;
	uint8_t nHeight = 0;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		nMaxSockets = 6;
		nWidth = 0;
		nHeight = 0;

		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		if (pItemsTxtRecord)
		{
			nWidth = pItemsTxtRecord->nInvWidth;
			nHeight = pItemsTxtRecord->nInvHeight;
		}

		if (nWidth * nHeight > 6)
		{
			if (nWidth * nHeight <= 6)
			{
				nMaxSockets = nWidth * nHeight;
			}

			nItemType = ITEMS_GetItemTypeFromItemId(pItem->dwClassId);
			if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
			{
				pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
				if (pItemTypesTxtRecord)
				{
					nAllowedSockets = ITEMS_GetAllowedGemSocketsFromItemId(pItem->dwClassId);
					nItemLevel = ITEMS_GetItemLevel(pItem);

					if (nItemLevel > 25)
					{
						if (nItemLevel > 40)
						{
							nMaxSocks = pItemTypesTxtRecord->nMaxSock40;
						}
						else
						{
							nMaxSocks = pItemTypesTxtRecord->nMaxSock25;
						}
					}
					else
					{
						nMaxSocks = pItemTypesTxtRecord->nMaxSock1;
					}

					if (nAllowedSockets >= nMaxSocks)
					{
						nAllowedSockets = nMaxSocks;
					}
				}
			}

			if (nMaxSockets >= nAllowedSockets)
			{
				nMaxSockets = nAllowedSockets;
			}

			if (nSockets <= 1)
			{
				nSockets = 1;
			}
			if (nSockets >= nMaxSockets)
			{
				nSockets = nMaxSockets;
			}

			if (pItem->pItemData)
			{
				pItem->pItemData->dwItemFlags |= IFLAG_SOCKETED;
			}

			STATLIST_SetUnitStat(pItem, STAT_ITEM_NUMSOCKETS, nSockets, 0);
		}
	}
}

//D2Common.0x6FD9D900 (#10819)
int __stdcall ITEMS_GetGemApplyTypeFromItemId(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->nGemApplyType;
}

//D2Common.0x6FD9D940 (#10820)
int __stdcall ITEMS_GetGemApplyType(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	return ITEMS_GetGemApplyTypeFromItemId(pItem->dwClassId);
}

//D2Common.0x6FD9D9D0 (#10821)
BOOL __stdcall ITEMS_IsSocketFiller(D2UnitStrc* pItem)
{
	return ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SOCKET_FILLER);
}

//D2Common.0x6FD9D9E0 (#10822)
const D2RunesTxt* __stdcall ITEMS_GetRunesTxtRecordFromItem(const D2UnitStrc* pItem)
{
	if (!pItem)
	{
		return nullptr;
	}

	const bool isItemOfSpecialQuality = (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwQualityNo >= ITEMQUAL_MAGIC && pItem->pItemData->dwQualityNo <= ITEMQUAL_TEMPERED);
	// Note: The game specifically checks all the things above, which means we will enter the condition if dwUnitType == UNIT_ITEM
	// While this is not really correct, this is what the game does, and it works because it expects the unit to be an item.
	// and is using another function that checked if an item was special, which happened to check that the unit was indeed an item
	if (isItemOfSpecialQuality)
	{
		return nullptr;
	}

	if (!DATATBLS_GetItemsTxtRecord(pItem->dwClassId)->nQuest && pItem->pInventory)
	{
		int nSocketableIds[6] = {};
		int nSocketedItems = 0;

		for (D2UnitStrc* pInventoryItem = INVENTORY_GetFirstItem(pItem->pInventory); pInventoryItem && INVENTORY_UnitIsItem(pInventoryItem); pInventoryItem = INVENTORY_GetNextItem(pInventoryItem))
		{
			nSocketableIds[nSocketedItems] = pInventoryItem->dwClassId;
			++nSocketedItems;
		}

		D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

		const int nSockets = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_NUMSOCKETS, 0);

		if (nSockets != nSocketedItems)
		{
			return nullptr;
		}

		for (int i = 0; i < sgptDataTables->pRuneDataTables.nRunesTxtRecordCount; ++i)
		{
			const D2RunesTxt* pRunesTxtRecord = &sgptDataTables->pRuneDataTables.pRunesTxt[i];

			if (pRunesTxtRecord->nComplete)
			{
				int nRuneCounter = 0;
				bool bMatch = true;

				while (nRuneCounter < 6 && pRunesTxtRecord->nRune[nRuneCounter] > 0)
				{
					if (nSocketedItems < nRuneCounter || nSocketableIds[nRuneCounter] != pRunesTxtRecord->nRune[nRuneCounter])
					{
						bMatch = false;
						break;
					}

					++nRuneCounter;
				}

				if (bMatch)
				{
					// All runes are a match, now check if the item types correspond
					bool isItemTypeExcluded = false;
					for (int nETypeCounter = 0; nETypeCounter < 3 && pRunesTxtRecord->wEType[nETypeCounter]; ++nETypeCounter)
					{
						if (ITEMS_CheckItemTypeId(pItem, pRunesTxtRecord->wEType[nETypeCounter]))
						{
							isItemTypeExcluded = true;
							break;
						}
					}

					// Not an excluded type, check valid types
					if (!isItemTypeExcluded)
					{
						for (int nITypeCounter = 0; nITypeCounter < 6 && pRunesTxtRecord->wIType[nITypeCounter]; ++nITypeCounter)
						{
							if (ITEMS_CheckItemTypeId(pItem, pRunesTxtRecord->wIType[nITypeCounter]))
							{
								return pRunesTxtRecord;
							}
						}
					}
				}
			}
		}
	}

	return nullptr;
}


static BOOL ITEMS_CheckTypeEquivalenceFromLUT(uint32_t* pEquivalenceLUT, uint32_t nItemType)
{
	return pEquivalenceLUT[nItemType / 32] & gdwBitMasks[nItemType % 32];
}

static uint32_t* ITEMS_TypeEquivalenceLUT(int nItemTypeLUT)
{
	return &sgptDataTables->pItemTypesEquivalenceLUTs[nItemTypeLUT * sgptDataTables->nItemTypesIndex];
}

//D2Common.0x6FD9DBA0 (#10729)
BOOL __stdcall ITEMS_CheckItemTypeIdByItemId(int nItemId, int nItemType)
{
	if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
	{
		const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);
		D2_ASSERT(pItemsTxtRecord);
		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			if (ITEMS_CheckTypeEquivalenceFromLUT(ITEMS_TypeEquivalenceLUT(pItemsTxtRecord->wType[0]), nItemType))
			{
				return TRUE;
			}

			if (pItemsTxtRecord->wType[1] > 0 && pItemsTxtRecord->wType[1] < sgptDataTables->nItemTypesTxtRecordCount)
			{
				return ITEMS_CheckTypeEquivalenceFromLUT(ITEMS_TypeEquivalenceLUT(pItemsTxtRecord->wType[1]), nItemType);
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD9DC80 (#10730)
BOOL __stdcall ITEMS_CheckType(int nItemType1, int nItemType2)
{
	if (nItemType1 >= 0 && nItemType1 < sgptDataTables->nItemTypesTxtRecordCount && nItemType2 >= 0 && nItemType2 < sgptDataTables->nItemTypesTxtRecordCount)
	{
		return ITEMS_CheckTypeEquivalenceFromLUT(ITEMS_TypeEquivalenceLUT(nItemType1), nItemType2);
	}

	return FALSE;
}

//D2Common.0x6FD9DCE0 (#10731)
BOOL __stdcall ITEMS_CheckItemTypeId(const D2UnitStrc* pItem, int nItemType)
{
	if (pItem)
	{
		return ITEMS_CheckItemTypeIdByItemId(pItem->dwClassId, nItemType);
	}

	return FALSE;
}

//D2Common.0x6FD9DDD0 (#10803)
//TODO: Find a better name
int __stdcall ITEMS_CheckBitField1Flag1(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->dwBitField1 & 1;
}

//D2Common.0x6FD9DE10 (#10802)
int __stdcall ITEMS_IsMetalItem(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->dwBitField1 & 2;
}

//D2Common.0x6FD9DE50 (#10801) - Unused
int __stdcall ITEMS_CheckBitField1Flag4(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->dwBitField1 & 4;
}

//D2Common.0x6FD9DE90 (#10774)
BOOL __stdcall ITEMS_IsNotQuestItemByItemId(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->nQuest < 1 && pItemsTxtRecord->wType[0] != ITEMTYPE_QUEST;
}

//D2Common.0x6FD9DEE0 (#10732)
int __stdcall ITEMS_GetFileIndex(D2UnitStrc* pItem)
{
	D2_ASSERT(ITEMS_GetItemData(pItem));
	return pItem->pItemData->dwFileIndex;
}

//D2Common.0x6FD9DF60 (#10733)
void __stdcall ITEMS_SetFileIndex(D2UnitStrc* pItem, uint32_t dwFileIndex)
{
	D2_ASSERT(ITEMS_GetItemData(pItem));
	pItem->pItemData->dwFileIndex = dwFileIndex;
}

//D2Common.0x6FD9DFE0 (#11244)
void __stdcall ITEMS_GetRealmData(D2UnitStrc* pItem, int* pRealmData0, int* pRealmData1)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		*pRealmData0 = pItemData->dwRealmData[0];
		*pRealmData1 = pItemData->dwRealmData[1];
	}
	else
	{
		*pRealmData0 = 0;
		*pRealmData1 = 0;
	}
}

//D2Common.0x6FD9E070 (#11245)
void __stdcall ITEMS_SetRealmData(D2UnitStrc* pItem, int a2, int a3)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->dwRealmData[0] = a2;
		pItemData->dwRealmData[1] = a3;
	}
}

//D2Common.0x6FD9E0A0 (#10734)
void __stdcall ITEMS_SetOwnerId(D2UnitStrc* pItem, D2UnitGUID nOwnerGUID)
{
	D2_ASSERT(ITEMS_GetItemData(pItem));
	pItem->pItemData->dwOwnerGUID = nOwnerGUID;
}

//D2Common.0x6FD9E120 (#10735)
int __stdcall ITEMS_GetOwnerId(D2UnitStrc* pItem)
{
	D2_ASSERT(ITEMS_GetItemData(pItem));
	return pItem->pItemData->dwOwnerGUID;
}

//D2Common.0x6FD9E1A0 (#10736)
BOOL __stdcall ITEMS_IsBodyItem(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord)
			{
				if (pItemTypesTxtRecord->nBodyLoc1 || pItemTypesTxtRecord->nBodyLoc2)
				{
					return pItemTypesTxtRecord->nBody;
				}
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD9E2A0 (#10738)
BOOL __stdcall ITEMS_IsClassValidByItemId(int nItemId)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);
	D2_ASSERT(pItemsTxtRecord);

	if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
	{
		pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
		if (pItemTypesTxtRecord)
		{
			return pItemTypesTxtRecord->nClass < 7;
		}
	}

	return FALSE;
}

//D2Common.0x6FD9E310 (#10737)
BOOL __stdcall ITEMS_IsClassValid(D2UnitStrc* pItem)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		return ITEMS_IsClassValidByItemId(pItem->dwClassId);
	}

	return FALSE;
}

//D2Common.0x6FD9E390 (#10739)
int __stdcall ITEMS_GetClassOfClassSpecificItem(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] >= 0 && pItemsTxtRecord->wType[0] < sgptDataTables->nItemTypesTxtRecordCount)
		{
			pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[pItemsTxtRecord->wType[0]];
			if (pItemTypesTxtRecord && pItemTypesTxtRecord->nClass < NUMBER_OF_PLAYERCLASSES)
			{
				return pItemTypesTxtRecord->nClass;
			}
		}
	}

	return PCLASS_INVALID;
}

//D2Common.0x6FD9E410 (#10823)
uint16_t __stdcall ITEMS_GetWeaponClassId(D2UnitStrc* pItem)
{
	static const D2CompositStrc stru_6FDE3C08[8] =
	{
		{ ' wob', WEAPONCLASS_BOW },
		{ ' sh1', WEAPONCLASS_1HS },
		{ ' th1', WEAPONCLASS_1HT },
		{ ' fts', WEAPONCLASS_STF },
		{ ' sh2', WEAPONCLASS_2HS },
		{ ' th2', WEAPONCLASS_2HT },
		{ ' wbx', WEAPONCLASS_XBW },
		{ ' 1th', WEAPONCLASS_HT1 },
	};

	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nCounter = 0;

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
	D2_ASSERT(pItemsTxtRecord);

	while (pItemsTxtRecord->dwWeapClass != stru_6FDE3C08[nCounter].nWeaponClassCode)
	{
		++nCounter;

		if (nCounter >= ARRAY_SIZE(stru_6FDE3C08))
		{
			return WEAPONCLASS_HTH;
		}
	}

	return (uint16_t)stru_6FDE3C08[nCounter].nWeaponClassId;
}

//D2Common.0x6FD9E480 (#10824)
uint32_t __stdcall ITEMS_GetTransmogrifyFromItemId(int nItemId)
{
	D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nItemId);

	D2_ASSERT(pItemsTxtRecord);

	return pItemsTxtRecord->nTransmogrify != 0;
}

//D2Common.0x6FD9E4C0 (#10825)
uint32_t __stdcall ITEMS_GetTransmogrify(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	return ITEMS_GetTransmogrifyFromItemId(pItem->dwClassId);
}

//D2Common.0x6FD9E550 (#10826)
int __stdcall ITEMS_IsMagSetRarUniCrfOrTmp(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc* pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->dwQualityNo >= ITEMQUAL_MAGIC && pItemData->dwQualityNo <= ITEMQUAL_TEMPERED;
	}
	return false;
}

//D2Common.0x6FD9E580 (#10740)
BOOL __stdcall ITEMS_IsNotQuestItem(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nItemId = 0;

	if (pItem)
	{
		if (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwItemFlags & IFLAG_NOSELL)
		{
			return FALSE;
		}

		nItemId = pItem->dwClassId;
	}
	else
	{
		nItemId = -1;
	}

	return ITEMS_IsNotQuestItemByItemId(nItemId);
}

//D2Common.0x6FD9E5F0 (#10827)
uint8_t __stdcall ITEMS_GetHitClassFromItem(D2UnitStrc* pItem)
{
	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);
	return pItemsTxtRecord->nHitClass;
}

//D2Common.0x6FD9E670 (#10828)
int __stdcall ITEMS_Is1Or2Handed(D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	D2_ASSERT(pItem);
	D2_ASSERT(pPlayer);

	if (pPlayer->dwUnitType || pPlayer->dwClassId != PCLASS_BARBARIAN)
	{
		return FALSE;
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);

	if (!pItemsTxtRecord->n1or2Handed)
	{
		return FALSE;
	}

	return TRUE;
}

//D2Common.0x6FD9E710 (#10829)
uint8_t* __stdcall ITEMS_GetColor(D2UnitStrc* pPlayer, D2UnitStrc* pItem, uint8_t* pColor, int nTransType)
{
	D2UniqueItemsTxt* pUniqueItemsTxtRecord = NULL;
	D2MagicAffixTxt* pMagicAffixTxtRecord = NULL;
	D2MagicAffixTxt* pAutoAffixTxtRecord = NULL;
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2SetItemsTxt* pSetItemsTxtRecord = NULL;
	D2ItemsTxt* pGemItemsTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2StatesTxt* pStatesTxtRecord = NULL;
	D2GemsTxt* pGemsTxtRecord = NULL;
	D2UnitStrc* pInventoryItem = NULL;
	int nItemQuality = 0;
	int nGemSockets = 0;
	int nMaxSockets = 0;
	int nItemLevel = 0;
	int nItemType = 0;
	int nCounter = 0;
	uint16_t nAutoAffix = 0;

	if (!pItem)
	{
		return NULL;
	}

	if (pPlayer)
	{
		for (int i = 0; i < sgptDataTables->nColourStates; ++i)
		{
			if (STATES_CheckState(pPlayer, sgptDataTables->pColourStates[i]) && sgptDataTables->pColourStates[i] >= 0 && sgptDataTables->pColourStates[i] < sgptDataTables->nStatesTxtRecordCount)
			{
				pStatesTxtRecord = &sgptDataTables->pStatesTxt[sgptDataTables->pColourStates[i]];
				if (pStatesTxtRecord && pStatesTxtRecord->nItemTrans < 21 && ITEMS_CheckItemTypeId(pItem, pStatesTxtRecord->wItemType))
				{
					pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
					if (pItemsTxtRecord)
					{
						return D2CMP_MixPalette(pItemsTxtRecord->nTransform, pStatesTxtRecord->nItemTrans);
					}
				}
			}
		}
	}

	if (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData)
	{
		nItemQuality = pItem->pItemData->dwQualityNo;
	}
	else
	{
		nItemQuality = ITEMQUAL_NORMAL;
	}

	switch (nItemQuality)
	{
	case ITEMQUAL_MAGIC:
	case ITEMQUAL_RARE:
		nCounter = 0;
		do
		{
			pMagicAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItem->dwUnitType == UNIT_ITEM && pItem->pItemData ? pItem->pItemData->wMagicSuffix[nCounter] : 0);
			if (pMagicAffixTxtRecord && pMagicAffixTxtRecord->nTransformColor != -1)
			{
				pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
				if (pItemsTxtRecord->nTransform <= 0 || pItemsTxtRecord->nTransform >= 9 || pMagicAffixTxtRecord->nTransformColor >= 21)
				{
					*pColor = 0;
				}
				else
				{
					*pColor = (pMagicAffixTxtRecord->nTransformColor & 31) + 32 * pItemsTxtRecord->nTransform;
				}

				if (nTransType)
				{
					return D2CMP_MixPalette(pItemsTxtRecord->nInvTrans, pMagicAffixTxtRecord->nTransformColor);
				}
				else
				{
					return D2CMP_MixPalette(pItemsTxtRecord->nTransform, pMagicAffixTxtRecord->nTransformColor);
				}
			}

			++nCounter;
		}
		while (nCounter < ITEMS_MAX_MODS);

		nCounter = 0;
		do
		{
			pMagicAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(pItem->dwUnitType == UNIT_ITEM && pItem->pItemData ? pItem->pItemData->wMagicPrefix[nCounter] : 0);
			if (pMagicAffixTxtRecord && pMagicAffixTxtRecord->nTransformColor != -1)
			{
				pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
				if (pItemsTxtRecord->nTransform <= 0 || pItemsTxtRecord->nTransform >= 9 || pMagicAffixTxtRecord->nTransformColor >= 21)
				{
					*pColor = 0;
				}
				else
				{
					*pColor = (pMagicAffixTxtRecord->nTransformColor & 31) + 32 * pItemsTxtRecord->nTransform;
				}

				if (nTransType)
				{
					return D2CMP_MixPalette(pItemsTxtRecord->nInvTrans, pMagicAffixTxtRecord->nTransformColor);
				}
				else
				{
					return D2CMP_MixPalette(pItemsTxtRecord->nTransform, pMagicAffixTxtRecord->nTransformColor);
				}
			}

			++nCounter;
		}
		while (nCounter < ITEMS_MAX_MODS);
		
		break;

	case ITEMQUAL_UNIQUE:
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		if (!pItemsTxtRecord)
		{
			return NULL;
		}

		D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);
		D2_ASSERT(pItem->pItemData);

		if (pItem->pItemData->dwFileIndex >= 0 && pItem->pItemData->dwFileIndex < sgptDataTables->nUniqueItemsTxtRecordCount)
		{
			pUniqueItemsTxtRecord = &sgptDataTables->pUniqueItemsTxt[pItem->pItemData->dwFileIndex];
			if (pUniqueItemsTxtRecord)
			{
				if (nTransType)
				{
					if (pUniqueItemsTxtRecord->nInvTransform >= 0)
					{
						if (pItemsTxtRecord->nTransform <= 0 || pItemsTxtRecord->nTransform >= 9 || pUniqueItemsTxtRecord->nChrTransform < 0 || pUniqueItemsTxtRecord->nChrTransform >= 21)
						{
							*pColor = 0;
						}
						else
						{
							*pColor = (pUniqueItemsTxtRecord->nChrTransform & 31) + 32 * pItemsTxtRecord->nTransform;
						}

						return D2CMP_MixPalette(pItemsTxtRecord->nInvTrans, pUniqueItemsTxtRecord->nInvTransform);
					}
				}
				else
				{
					if (pUniqueItemsTxtRecord->nChrTransform >= 0)
					{
						if (pItemsTxtRecord->nTransform <= 0 || pItemsTxtRecord->nTransform >= 9 || pUniqueItemsTxtRecord->nChrTransform < 0 || pUniqueItemsTxtRecord->nChrTransform >= 21)
						{
							*pColor = 0;
						}
						else
						{
							*pColor = (pUniqueItemsTxtRecord->nChrTransform & 31) + 32 * pItemsTxtRecord->nTransform;
						}

						return D2CMP_MixPalette(pItemsTxtRecord->nTransform, pUniqueItemsTxtRecord->nChrTransform);
					}
				}
			}
		}

		return NULL;

	case ITEMQUAL_SET:
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		if (!pItemsTxtRecord)
		{
			return NULL;
		}

		D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);
		D2_ASSERT(pItem->pItemData);

		if (pItem->pItemData->dwFileIndex >= 0 && pItem->pItemData->dwFileIndex < sgptDataTables->nSetItemsTxtRecordCount)
		{
			pSetItemsTxtRecord = &sgptDataTables->pSetItemsTxt[pItem->pItemData->dwFileIndex];
			if (pSetItemsTxtRecord)
			{
				if (nTransType)
				{
					if (pSetItemsTxtRecord->nInvTransform >= 0)
					{
						if (pItemsTxtRecord->nTransform <= 0 || pItemsTxtRecord->nTransform >= 9 || pSetItemsTxtRecord->nChrTransform < 0 || pSetItemsTxtRecord->nChrTransform >= 21)
						{
							*pColor = 0;
						}
						else
						{
							*pColor = (pSetItemsTxtRecord->nChrTransform & 31) + 32 * pItemsTxtRecord->nTransform;
						}

						return D2CMP_MixPalette(pItemsTxtRecord->nInvTrans, pSetItemsTxtRecord->nInvTransform);
					}
				}
				else
				{
					if (pSetItemsTxtRecord->nChrTransform >= 0)
					{
						if (pItemsTxtRecord->nTransform <= 0 || pItemsTxtRecord->nTransform >= 9 || pSetItemsTxtRecord->nChrTransform < 0 || pSetItemsTxtRecord->nChrTransform >= 21)
						{
							*pColor = 0;
						}
						else
						{
							*pColor = (pSetItemsTxtRecord->nChrTransform & 31) + 32 * pItemsTxtRecord->nTransform;
						}

						return D2CMP_MixPalette(pItemsTxtRecord->nTransform, pSetItemsTxtRecord->nChrTransform);
					}
				}
			}
		}

		return NULL;

	default:
		if (pItem->dwUnitType == UNIT_ITEM)
		{
			pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
			if (pItemsTxtRecord && pItemsTxtRecord->nHasInv)
			{
				nItemType = ITEMS_GetItemTypeFromItemId(pItem->dwClassId);
				if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
				{
					pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
					if (pItemTypesTxtRecord)
					{
						nGemSockets = ITEMS_GetAllowedGemSocketsFromItemId(pItem->dwClassId);
						nItemLevel = ITEMS_GetItemLevel(pItem);
						if (nItemLevel > 25)
						{
							if (nItemLevel > 40)
							{
								nMaxSockets = pItemTypesTxtRecord->nMaxSock40;
							}
							else
							{
								nMaxSockets = pItemTypesTxtRecord->nMaxSock25;
							}
						}
						else
						{
							nMaxSockets = pItemTypesTxtRecord->nMaxSock1;
						}

						if (nGemSockets >= nMaxSockets)
						{
							nGemSockets = nMaxSockets;
						}

						if (nGemSockets && pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwItemFlags & IFLAG_SOCKETED)
						{
							pInventoryItem = INVENTORY_GetFirstItem(pItem->pInventory);
							if (pInventoryItem && INVENTORY_UnitIsItem(pInventoryItem) && ITEMS_CheckItemTypeId(pInventoryItem, ITEMTYPE_GEM))
							{
								pGemItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pInventoryItem->dwClassId);
								D2_ASSERT(pGemItemsTxtRecord);

								pGemsTxtRecord = DATATBLS_GetGemsTxtRecord(pGemItemsTxtRecord->dwGemOffset);
								if (pItemsTxtRecord->nTransform <= 0 || pItemsTxtRecord->nTransform >= 9 || pGemsTxtRecord->nTransForm >= 21)
								{
									*pColor = 0;
								}
								else
								{
									*pColor = (pGemsTxtRecord->nTransForm & 31) + 32 * pItemsTxtRecord->nTransform;
								}

								if (nTransType)
								{
									return D2CMP_MixPalette(pItemsTxtRecord->nInvTrans, pGemsTxtRecord->nTransForm);
								}
								else
								{
									return D2CMP_MixPalette(pItemsTxtRecord->nTransform, pGemsTxtRecord->nTransForm);
								}
							}
						}
					}
				}
			}
		}

		break;
	}

	if (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData)
	{
		nAutoAffix = pItem->pItemData->wAutoAffix;
	}

	pAutoAffixTxtRecord = DATATBLS_GetMagicAffixTxtRecord(nAutoAffix);
	if (pAutoAffixTxtRecord && pAutoAffixTxtRecord->nTransformColor != -1)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		if (pItemsTxtRecord->nTransform <= 0 || pItemsTxtRecord->nTransform >= 9 || pAutoAffixTxtRecord->nTransformColor >= 21)
		{
			*pColor = 0;
		}
		else
		{
			*pColor = (pAutoAffixTxtRecord->nTransformColor & 31) + 32 * pItemsTxtRecord->nTransform;
		}

		if (nTransType)
		{
			return D2CMP_MixPalette(pItemsTxtRecord->nInvTrans, pAutoAffixTxtRecord->nTransformColor);
		}
		else
		{
			return D2CMP_MixPalette(pItemsTxtRecord->nTransform, pAutoAffixTxtRecord->nTransformColor);
		}
	}

	return NULL;
}

//D2Common.0x6FD9EE70
D2SetItemsTxt* __fastcall ITEMS_GetSetItemsTxtRecord(int nRecordId)
{
	if (nRecordId >= 0 && nRecordId < sgptDataTables->nSetItemsTxtRecordCount)
	{
		return &sgptDataTables->pSetItemsTxt[nRecordId];
	}

	return NULL;
}

//D2Common.0x6FD9EEA0 (#10830)
BOOL __stdcall ITEMS_IsImbueable(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2UnitStrc* pInventoryItem = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);

		if (pItemsTxtRecord && pItemsTxtRecord->wType[0] != ITEMTYPE_GOLD)
		{
			if (!pItem->pItemData || !(pItem->pItemData->dwItemFlags & IFLAG_NOSELL))
			{
				if (pItemsTxtRecord->dwBitField1 & 1)
				{
					pItemTypesTxtRecord = DATATBLS_GetItemTypesTxtRecord(ITEMS_GetItemTypeFromItemId(pItem->dwClassId));
					if (pItem->dwFlagEx & UNITFLAGEX_ISEXPANSION || !pItemTypesTxtRecord || !pItemTypesTxtRecord->nThrowable)
					{
						if (!pItemsTxtRecord->nQuest || pItemsTxtRecord->dwCode == ' gel')
						{
							pInventoryItem = INVENTORY_GetFirstItem(pItem->pInventory);
							if (pInventoryItem)
							{
								while (!INVENTORY_UnitIsItem(pInventoryItem))
								{
									pInventoryItem = INVENTORY_GetNextItem(pInventoryItem);
									if (!pInventoryItem)
									{
										if ((!pItem->pItemData || !(pItem->pItemData->dwItemFlags & IFLAG_SOCKETED)) && (!pItem->pItemData || (pItem->pItemData->dwQualityNo >= 4 && pItem->pItemData->dwQualityNo <= 9)))
										{
											return TRUE;
										}
										break;
									}
								}
							}
							else
							{
								if ((!pItem->pItemData || !(pItem->pItemData->dwItemFlags & IFLAG_SOCKETED)) && (!pItem->pItemData || (pItem->pItemData->dwQualityNo >= 4 && pItem->pItemData->dwQualityNo <= 9)))
								{
									return TRUE;
								}
							}
						}
					}
				}
			}
		}
	}
	
	return FALSE;
}

//D2Common.0x6FD9F080 (#10832)
BOOL __stdcall ITEMS_IsPersonalizable(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	D2_ASSERT(pItem);
	if (pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] == ITEMTYPE_GOLD || pItemsTxtRecord->wType[0] == ITEMTYPE_BOW_QUIVER
			|| pItemsTxtRecord->wType[0] == ITEMTYPE_CROSSBOW_QUIVER || pItemsTxtRecord->wType[0] == ITEMTYPE_PLAYER_BODY_PART)
		{
			return FALSE;
		}

		if (pItem->pItemData && (pItem->pItemData->dwItemFlags & IFLAG_NOSELL || pItem->pItemData->dwItemFlags & IFLAG_BROKEN))
		{
			return FALSE;
		}

		if (!pItem->pItemData || !(pItem->pItemData->dwItemFlags & IFLAG_PERSONALIZED))
		{
			if (!pItemsTxtRecord->nCompactSave)
			{
				return pItemsTxtRecord->nNameable != 0;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD9F260 (#10831)
BOOL __stdcall ITEMS_IsSocketable(D2UnitStrc* pItem)
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2UnitStrc* pSocket = NULL;
	uint8_t nAllowedSockets = 0;
	int nMaxSockets = 0;
	int nItemLevel = 0;
	int nItemType = 0;

	D2_ASSERT(pItem);

	if (pItem->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->wType[0] == ITEMTYPE_GOLD)
		{
			return FALSE;
		}

		if (pItem->pItemData && pItem->pItemData->dwItemFlags & IFLAG_NOSELL)
		{
			return FALSE;
		}
	}

	if (DATATBLS_GetItemsTxtRecord(pItem->dwClassId)->nQuest)
	{
		D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		if (pItemsTxtRecord->dwCode != ' gel')
		{
			return FALSE;
		}
	}

	if (pItem->dwUnitType == UNIT_ITEM && pItem->pItemData && pItem->pItemData->dwItemFlags & IFLAG_BROKEN)
	{
		return FALSE;
	}

	pSocket = INVENTORY_GetFirstItem(pItem->pInventory);
	if (pSocket)
	{
		while (!INVENTORY_UnitIsItem(pSocket))
		{
			pSocket = INVENTORY_GetNextItem(pSocket);
			if (!pSocket)
			{
				break;
			}
		}

		if (pSocket)
		{
			return FALSE;
		}
	}

	if (pItem->dwUnitType != UNIT_ITEM || pItem->pItemData && pItem->pItemData->dwItemFlags & IFLAG_SOCKETED)
	{
		return FALSE;
	}

	nItemType = ITEMS_GetItemTypeFromItemId(pItem->dwClassId);
	if (nItemType < 0 || nItemType >= sgptDataTables->nItemTypesTxtRecordCount)
	{
		return FALSE;
	}

	pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
	if (!pItemTypesTxtRecord)
	{
		return FALSE;
	}

	nAllowedSockets = ITEMS_GetAllowedGemSocketsFromItemId(pItem->dwClassId);
	nItemLevel = ITEMS_GetItemLevel(pItem);
	if (nItemLevel > 25)
	{
		if (nItemLevel > 40)
		{
			nMaxSockets = pItemTypesTxtRecord->nMaxSock40;
		}
		else
		{
			nMaxSockets = pItemTypesTxtRecord->nMaxSock25;
		}
	}
	else
	{
		nMaxSockets = pItemTypesTxtRecord->nMaxSock1;
	}

	if (nAllowedSockets >= nMaxSockets)
	{
		nAllowedSockets = nMaxSockets;
	}

	if (!nAllowedSockets)
	{
		return 0;
	}

	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

	return STATLIST_UnitGetStatValue(pItem, STAT_ITEM_NUMSOCKETS, 0) == 0;
}

//D2Common.0x6FD9F490 (#10877)
int __stdcall ITEMS_GetAllRepairCosts(D2GameStrc* pGame, D2UnitStrc* pUnit, int nNpcId, int nDifficulty, D2BitBufferStrc* pQuestFlags, void(__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*))
{
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2UnitStrc* pItem = NULL;
	BOOL bCanBeRepaired = FALSE;
	int nMaxDurability = 0;
	int nRepairCosts = 0;
	int nItemType = 0;
	int nMaxStack = 0;
	int nCounter = 0;
	int nBodyLoc = 0;
	int nStats = 0;
	D2StatStrc pStat[64] = {};

	if (!pUnit || !pUnit->pInventory)
	{
		return 0;
	}

	while (nBodyLoc < NUM_BODYLOC)
	{
		pItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
		if (pItem)
		{
			bCanBeRepaired = FALSE;

			if (ITEMS_IsRepairable(pItem) && pItem->dwUnitType == UNIT_ITEM)
			{
				pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
				if (pItemsTxtRecord && !pItemsTxtRecord->nNoDurability && pItemsTxtRecord->nDurability && STATLIST_GetMaxDurabilityFromUnit(pItem) && STATLIST_UnitGetStatValue(pItem, STAT_ITEM_INDESCTRUCTIBLE, 0) <= 0)
				{
					nMaxDurability = STATLIST_GetMaxDurabilityFromUnit(pItem);
					if (nMaxDurability && STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0) != nMaxDurability)
					{
						bCanBeRepaired = TRUE;
					}
				}
			}

			if (pItem->dwUnitType == UNIT_ITEM)
			{
				nItemType = ITEMS_GetItemTypeFromItemId(pItem->dwClassId);
			}
			else
			{
				nItemType = 0;
			}

			if (nItemType >= 0 && nItemType < sgptDataTables->nItemTypesTxtRecordCount)
			{
				pItemTypesTxtRecord = &sgptDataTables->pItemTypesTxt[nItemType];
				if (pItemTypesTxtRecord && pItemTypesTxtRecord->nRepair && pItem->dwUnitType == UNIT_ITEM && ITEMS_CheckItemTypeIfThrowable(ITEMS_GetItemType(pItem)))
				{
					if (ITEMS_CheckIfStackable(pItem) && !ITEMS_CheckItemFlag(pItem, IFLAG_ETHEREAL, __LINE__, __FILE__))
					{
						pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
						D2_ASSERT(pItemsTxtRecord);

						nMaxStack = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_EXTRA_STACK, 0) + pItemsTxtRecord->dwMaxStack;
						if (nMaxStack >= 511)
						{
							nMaxStack = 511;
						}

						if (STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0) < nMaxStack)
						{
							bCanBeRepaired = TRUE;
						}
					}
				}
			}

			nStats = STATLIST_CopyStats(pItem, STAT_ITEM_CHARGED_SKILL, pStat, ARRAY_SIZE(pStat));
			if (nStats <= 0)
			{
				if (bCanBeRepaired)
				{
					nRepairCosts += ITEMS_CalculateTransactionCost(pUnit, pItem, nDifficulty, pQuestFlags, nNpcId, 3);

					if (pfCallback)
					{
						pfCallback(pGame, pItem, pUnit);
					}
				}
			}
			else
			{
				nCounter = 0;
				while ((uint8_t)pStat[nCounter].nValue >= pStat[nCounter].nValue >> 8)
				{
					++nCounter;
					if (nCounter >= nStats)
					{
						if (bCanBeRepaired)
						{
							nRepairCosts += ITEMS_CalculateTransactionCost(pUnit, pItem, nDifficulty, pQuestFlags, nNpcId, 3);

							if (pfCallback)
							{
								pfCallback(pGame, pItem, pUnit);
							}
						}
						break;
					}
				}
			}
		}

		++nBodyLoc;
	}

	return nRepairCosts;
}

//D2Common.0x6FD9F720 (#10833)
BOOL __stdcall ITEMS_AreStackablesEqual(D2UnitStrc* pItem1, D2UnitStrc* pItem2)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (!pItem1 || pItem1->dwUnitType != UNIT_ITEM || !pItem1->pItemData || !pItem2 || pItem2->dwUnitType != UNIT_ITEM || !pItem2->pItemData)
	{
		return FALSE;
	}

	if (pItem1->dwClassId != pItem2->dwClassId || pItem1->pItemData->dwQualityNo != pItem2->pItemData->dwQualityNo || pItem1->pItemData->dwFileIndex != pItem2->pItemData->dwFileIndex)
	{
		return FALSE;
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem1->dwClassId);
	D2_ASSERT(pItemsTxtRecord);

	if (!pItemsTxtRecord->nStackable)
	{
		return FALSE;
	}
	
	if((ITEMS_CheckItemFlag(pItem1, IFLAG_ETHEREAL, __LINE__, __FILE__) != ITEMS_CheckItemFlag(pItem2, IFLAG_ETHEREAL, __LINE__, __FILE__))
		|| !pItem1->pItemData->dwQualityNo || pItem1->pItemData->dwQualityNo > 3 && pItem1->pItemData->dwQualityNo <= 9
		|| !pItem2->pItemData->dwQualityNo || pItem2->pItemData->dwQualityNo > 3 && pItem2->pItemData->dwQualityNo <= 9
		|| STATLIST_UnitGetStatValue(pItem1, STAT_MINDAMAGE, 0) != STATLIST_UnitGetStatValue(pItem2, STAT_MINDAMAGE, 0)
		|| STATLIST_UnitGetStatValue(pItem1, STAT_MAXDAMAGE, 0) != STATLIST_UnitGetStatValue(pItem2, STAT_MAXDAMAGE, 0)
		|| STATLIST_UnitGetStatValue(pItem1, STAT_SECONDARY_MINDAMAGE, 0) != STATLIST_UnitGetStatValue(pItem2, STAT_SECONDARY_MINDAMAGE, 0)
		|| STATLIST_UnitGetStatValue(pItem1, STAT_SECONDARY_MAXDAMAGE, 0) != STATLIST_UnitGetStatValue(pItem2, STAT_SECONDARY_MAXDAMAGE, 0)
		|| STATLIST_UnitGetStatValue(pItem1, STAT_ITEM_THROW_MINDAMAGE, 0) != STATLIST_UnitGetStatValue(pItem2, STAT_ITEM_THROW_MINDAMAGE, 0)
		|| STATLIST_UnitGetStatValue(pItem1, STAT_ITEM_THROW_MAXDAMAGE, 0) != STATLIST_UnitGetStatValue(pItem2, STAT_ITEM_THROW_MAXDAMAGE, 0))
	{
		return FALSE;
	}

	if (STATLIST_UnitGetStatValue(pItem1, STAT_ITEM_NUMSOCKETS, 0) || STATLIST_UnitGetStatValue(pItem2, STAT_ITEM_NUMSOCKETS, 0))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

//D2Common.0x6FD9FA70 (#10834)
BOOL __stdcall ITEMS_CanItemBeUsedForThrowSkill(D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nStack = 0;

	if (pItem)
	{
		if (STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0) > 0)
		{
			return TRUE;
		}

		if (STATLIST_UnitGetItemStatOrSkillStatValue(pItem, STAT_ITEM_THROWABLE, 0))
		{
			STATLIST_SetUnitStat(pItem, STAT_QUANTITY, 0, 0);
			return TRUE;
		}

		D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		nStack = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_EXTRA_STACK, 0) + pItemsTxtRecord->dwMaxStack;
		if (nStack < 511 && nStack <= 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD9FB40 (#11079)
int __stdcall D2COMMON_11079_Return0(int a1, int a2)
{
	REMOVE_LATER_Trace("D2COMMON_11079_Return0: Useless");
	return 0;
}

//D2Common.0x6FD9FB50 (#10836)
uint32_t __stdcall ITEMS_GetSetItemsMask(D2UnitStrc* pPlayer, D2UnitStrc* pSetItem, BOOL bSkipItem)
{
	D2SetItemsTxt* pSetItemsTxtRecord = NULL;
	D2ItemDataStrc* pItemData = NULL;
	uint32_t nSetItemMask = 0;
	int nFileIndex = 0;

	if (!pPlayer || !pSetItem || pSetItem->dwUnitType != UNIT_ITEM || !pSetItem->pItemData || pSetItem->pItemData->dwQualityNo != ITEMQUAL_SET || !pPlayer->pInventory)
	{
		return 0;
	}

	nFileIndex = pSetItem->pItemData->dwFileIndex;

	if (nFileIndex >= 0 && nFileIndex < sgptDataTables->nSetItemsTxtRecordCount)
	{
		pSetItemsTxtRecord = &sgptDataTables->pSetItemsTxt[nFileIndex];

		if (pSetItemsTxtRecord && pSetItemsTxtRecord->nSetId >= 0 && pSetItemsTxtRecord->nSetId < sgptDataTables->nSetsTxtRecordCount && &sgptDataTables->pSetsTxt[pSetItemsTxtRecord->nSetId])
		{
			for (D2UnitStrc* i = INVENTORY_GetFirstItem(pPlayer->pInventory); i; i = INVENTORY_GetNextItem(i))
			{
				if (INVENTORY_GetItemNodePage(i) == 3)
				{
					if (INVENTORY_UnitIsItem(i) && (bSkipItem || i != pSetItem))
					{
						pItemData = i->pItemData;
						if (pItemData && pItemData->dwQualityNo == ITEMQUAL_SET && !(pItemData->dwItemFlags & IFLAG_NOEQUIP) && !(pItemData->dwItemFlags & IFLAG_BROKEN))
						{
							nFileIndex = pItemData->dwFileIndex;
							if (nFileIndex >= 0 && nFileIndex < sgptDataTables->nSetItemsTxtRecordCount)
							{
								pSetItemsTxtRecord = &sgptDataTables->pSetItemsTxt[nFileIndex];
								if (pSetItemsTxtRecord && pSetItemsTxtRecord->nSetId == pSetItemsTxtRecord->nSetId)
								{
									nSetItemMask |= 1 << LOBYTE(pSetItemsTxtRecord->nSetItems);
								}
							}
						}
					}
				}
			}
			return nSetItemMask;
		}
	}

	return 0;
}

//D2Common.0x6FD9FD80 (#10838)
D2SetItemsTxt* __stdcall ITEMS_GetSetItemsTxtRecordFromItem(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		if (pItemData->dwQualityNo == ITEMQUAL_SET
			&& pItemData->dwFileIndex >= 0
			&& pItemData->dwFileIndex < sgptDataTables->nSetItemsTxtRecordCount)
		{
			return &sgptDataTables->pSetItemsTxt[pItemData->dwFileIndex];
		}
	}

	return nullptr;
}

//D2Common.0x6FD9FE20 (#10839)
BOOL __stdcall ITEMS_CanBeEquipped(D2UnitStrc* pItem)
{
	if (pItem)
	{
		if (pItem->dwUnitType != UNIT_ITEM || !pItem->pItemData)
		{
			return TRUE;
		}

		return (!(pItem->pItemData->dwItemFlags & IFLAG_BROKEN) && !(pItem->pItemData->dwItemFlags & IFLAG_NOEQUIP));
	}

	return FALSE;
}

//D2Common.0x6FD9FE70 (#10840)
BOOL __stdcall ITEMS_IsCharmUsable(D2UnitStrc* pItem, D2UnitStrc* pPlayer)
{
	D2ItemDataStrc* pItemData = ITEMS_GetItemData(pItem);
	if (pItemData->dwItemFlags & IFLAG_BROKEN || pItemData->dwItemFlags & IFLAG_NOEQUIP)
	{
		return FALSE;
	}
	
	if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_CHARM))
	{
		if (pItem->dwUnitType == UNIT_ITEM && pItemData->nInvPage == INVPAGE_INVENTORY)
		{
			return ITEMS_CheckRequirements(pItem, pPlayer, FALSE, NULL, NULL, NULL) != 0;
		}
	}

	return FALSE;
}

//D2Common.0x6FD9FF00 (#10776)
int __stdcall ITEMS_GetNoOfUnidItems(D2UnitStrc* pUnit)
{
	D2UnitStrc* pItem = NULL;
	int nUnidItems = 0;
	int nNodePage = 0;

	pItem = INVENTORY_GetFirstItem(pUnit->pInventory);
	if (pItem)
	{
		nUnidItems = 0;
		while (INVENTORY_UnitIsItem(pItem))
		{
			if (!pItem->pItemData || !(pItem->pItemData->dwItemFlags & IFLAG_IDENTIFIED))
			{
				nNodePage = INVENTORY_GetItemNodePage(pItem);
				if (nNodePage != 1)
				{
					if (nNodePage == 3)
					{
						++nUnidItems;
					}
				}
				else
				{
					if (pItem->pItemData && (pItem->pItemData->nInvPage == INVPAGE_CUBE || pItem->pItemData->nInvPage == INVPAGE_INVENTORY))
					{
						++nUnidItems;
					}
				}
			}

			pItem = INVENTORY_GetNextItem(pItem);
			if (!pItem)
			{
				return nUnidItems;
			}
		}
	}

	return 0;
}

//D2Common.0x6FD9FF90 (#10841)
int __stdcall ITEMS_GetBonusLifeBasedOnClass(D2UnitStrc* pPlayer, int nValue)
{
	if (!pPlayer || pPlayer->dwUnitType != UNIT_PLAYER)
	{
		return 2 * nValue;
	}
	else
	{
		switch (pPlayer->dwClassId)
		{
		case PCLASS_AMAZON:
		case PCLASS_PALADIN:
		case PCLASS_ASSASSIN:
			return nValue + (nValue >> 1);
		case PCLASS_BARBARIAN:
			return 2 * nValue;
		default:
			return nValue;
		}
	}
}

//D2Common.0x6FD9FFE0 (#10842)
int __stdcall ITEMS_GetBonusManaBasedOnClass(D2UnitStrc* pPlayer, int nValue)
{
	if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER)
	{
		switch (pPlayer->dwClassId)
		{
		case PCLASS_SORCERESS:
		case PCLASS_NECROMANCER:
		case PCLASS_DRUID:
			return 2 * nValue;
		case PCLASS_AMAZON:
		case PCLASS_PALADIN:
		case PCLASS_ASSASSIN:
			return nValue + (nValue >> 1);
		default:
			return nValue;
		}
	}

	return nValue;
}

//D2Common.0x6FDA0030 (#10875)
uint16_t __stdcall ITEMS_GetItemFormat(D2UnitStrc* pItem)
{
	D2_ASSERT(ITEMS_GetItemData(pItem));
	return pItem->pItemData->wItemFormat;
}

//D2Common.0x6FDA00B0 (#10876)
void __stdcall ITEMS_SetItemFormat(D2UnitStrc* pItem, uint16_t nItemFormat)
{
	D2_ASSERT(ITEMS_GetItemData(pItem));
	pItem->pItemData->wItemFormat = nItemFormat;
}

//D2Common.0x6FDA0130 (#10878)
int __stdcall ITEMS_GetWeaponAttackSpeed(D2UnitStrc* pUnit, D2UnitStrc* pWeapon)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	int nWeaponClass = 0;
	int v11 = 0;
	int v13 = 0;
	char szPath[8] = {};

	int nLength = 0;

	D2_ASSERT(pWeapon && pWeapon->dwUnitType == UNIT_ITEM);
	D2_ASSERT(ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_WEAPON));
	D2_ASSERT(pUnit);

	if (pWeapon->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pWeapon->dwClassId);
		if (pItemsTxtRecord)
		{
			nWeaponClass = pItemsTxtRecord->dwWeapClass;
		}
	}

	D2Common_10884_COMPOSIT_unk(pUnit, pUnit->dwClassId, PLRMODE_ATTACK1, pUnit->dwUnitType, pUnit->pInventory, szPath, &nWeaponClass, 0, 0);

	if (D2Common_10641(szPath, &nLength, &v11, &v13))
	{
		D2_ASSERT(nLength);

		return (nLength << 8) / (v11 * (STATLIST_UnitGetStatValue(pWeapon, STAT_ATTACKRATE, 0) + STATLIST_UnitGetItemStatOrSkillStatValue(pWeapon, STAT_ITEM_FASTERATTACKRATE, 0) + 100) / 100);
	}

	return 45;
}

//D2Common.0x6FDA02B0 (#10879)
int __stdcall ITEMS_HasUsedCharges(D2UnitStrc* pItem, BOOL* pHasChargedSkills)
{
	D2StatStrc pStat[64] = {};
	int nCounter = 0;
	int nStats = 0;

	if (pHasChargedSkills)
	{
		*pHasChargedSkills = TRUE;
	}

	nStats = STATLIST_CopyStats(pItem, STAT_ITEM_CHARGED_SKILL, pStat, ARRAY_SIZE(pStat));

	if (nStats <= 0)
	{
		return FALSE;
	}
	else
	{
		if (pHasChargedSkills)
		{
			nCounter = 0;
			if (nStats > 0)
			{
				while ((uint8_t)pStat[nCounter].nValue >= (pStat[nCounter].nValue >> 8))
				{
					++nCounter;

					if (nCounter >= nStats)
					{
						return TRUE;
					}
				}

				*pHasChargedSkills = FALSE;
			}
		}

		return TRUE;
	}
}

//D2Common.0x6FDA0340 (#10880)
BOOL __stdcall ITEMS_IsEthereal(D2UnitStrc* pItem)
{
	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		return pItemData->dwItemFlags & IFLAG_ETHEREAL;
	}

	return 0;
}

//D2Common.0x6FDA0370 (#10883)
BOOL __stdcall ITEMS_GetCompactItemDataFromBitstream(uint8_t* pBitstream, size_t nSize, BOOL bCheckForHeader, D2ItemSaveStrc* pItemSave)
{
	D2BitBufferStrc pBuffer = {};
	int dwCode = 0;

	BITMANIP_Initialize(&pBuffer, pBitstream, nSize);

	if (bCheckForHeader && BITMANIP_ReadSigned(&pBuffer, 16) != 'MJ')
	{
		return FALSE;
	}
	else
	{
		pItemSave->dwFlags = BITMANIP_Read(&pBuffer, 32);

		BITMANIP_Read(&pBuffer, 10);

		pItemSave->nAnimMode = (uint8_t)BITMANIP_Read(&pBuffer, 3);

		if (pItemSave->nAnimMode == IMODE_ONGROUND || pItemSave->nAnimMode == IMODE_DROPPING)
		{
			pItemSave->nX = (uint16_t)BITMANIP_Read(&pBuffer, 16);
			pItemSave->nY = (uint16_t)BITMANIP_Read(&pBuffer, 16);
		}
		else
		{
			pItemSave->nBodyloc = (uint8_t)BITMANIP_Read(&pBuffer, 4);
			pItemSave->nX = (uint16_t)BITMANIP_Read(&pBuffer, 4);
			pItemSave->nY = (uint16_t)BITMANIP_Read(&pBuffer, 4);
			pItemSave->nStorePage = (uint8_t)BITMANIP_Read(&pBuffer, 3) - 1;
		}

		if (pItemSave->dwFlags & IFLAG_ISEAR)
		{
			dwCode = ' rae';
		}
		else
		{
			dwCode = BITMANIP_Read(&pBuffer, 32);
		}

		pItemSave->nClassId = DATATBLS_GetItemIdFromItemCode(dwCode);

		if (!(pItemSave->dwFlags & (IFLAG_LOWQUALITY | IFLAG_COMPACTSAVE)))
		{
			pItemSave->nItemFileIndex = BITMANIP_Read(&pBuffer, 3);
		}
		else
		{
			pItemSave->nItemFileIndex = 0;
		}

		return TRUE;
	}
}

//D2Common.0x6FDA0490 (#10882)
size_t __stdcall ITEMS_DecodeItemFromBitstream(D2UnitStrc* pItem, uint8_t* pBitstream, size_t nSize, BOOL bCheckForHeader, int* pSocketedItemCount, uint32_t dwVersion, BOOL* pFail)
{
	D2BitBufferStrc pBuffer = {};
	uint32_t dwFlags = 0;
	BOOL bGamble = FALSE;

	BITMANIP_Initialize(&pBuffer, pBitstream, nSize);

	if (bCheckForHeader && BITMANIP_ReadSigned(&pBuffer, 16) != 'MJ')
	{
		*pFail = TRUE;
		return 0;
	}

	*pFail = FALSE;

	dwFlags = BITMANIP_Read(&pBuffer, 32);
	if (dwFlags & IFLAG_LOWQUALITY)
	{
		bGamble = TRUE;
		dwFlags &= ~IFLAG_LOWQUALITY;
	}

	if (D2ItemDataStrc * pItemData = ITEMS_GetItemData(pItem))
	{
		pItemData->dwItemFlags = dwFlags;

		pItemData->dwItemFlags &= ~IFLAG_INIT;

		pItemData->dwItemLevel = 1;

		for (int i = 0; i < ITEMS_MAX_MODS; ++i)
		{
			pItemData->wMagicPrefix[i] = 0;
			pItemData->wMagicSuffix[i] = 0;
		}

		pItemData->wRarePrefix = 0;
		pItemData->wRareSuffix = 0;
	}

	if (dwFlags & IFLAG_COMPACTSAVE)
	{
		if (pSocketedItemCount)
		{
			*pSocketedItemCount = 0;
		}

		if (ITEMS_DecodeItemBitstreamCompact(pItem, &pBuffer, bCheckForHeader, dwVersion) <= 0)
		{
			*pFail = TRUE;
		}
	}
	else
	{
		if (ITEMS_DecodeItemBitstreamComplete(pItem, &pBuffer, bCheckForHeader, bGamble, pSocketedItemCount, dwVersion) <= 0)
		{
			*pFail = TRUE;
		}
	}

	return BITMANIP_GetSize(&pBuffer);
}

//D2Common.0x6FDA0620
int __fastcall ITEMS_DecodeItemBitstreamCompact(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, BOOL bCheckForHeader, uint32_t dwVersion)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2StatListStrc* pStatList = NULL;
	int nBits = 0;
	int nItemId = 0;
	int nValue = 0;
	uint32_t dwCode = 0;
	char szChar = 0;
	uint8_t nAnimMode = 0;

	D2ItemDataStrc* pItemData = ITEMS_GetItemData(pItem);
	D2_ASSERT(pItemData);

	pItemData->wItemFormat = (uint16_t)BITMANIP_Read(pBuffer, 10);

	nAnimMode = (uint8_t)BITMANIP_Read(pBuffer, 3);
	if (nAnimMode == IMODE_ONGROUND || nAnimMode == IMODE_DROPPING)
	{
		UNITS_SetXForStaticUnit(pItem, BITMANIP_Read(pBuffer, 16));
		UNITS_SetYForStaticUnit(pItem, BITMANIP_Read(pBuffer, 16));
	}
	else
	{
		pItemData->nBodyLoc = (uint8_t)BITMANIP_Read(pBuffer, 4);
		UNITS_SetXForStaticUnit(pItem, BITMANIP_Read(pBuffer, 4));
		UNITS_SetYForStaticUnit(pItem, BITMANIP_Read(pBuffer, 4));
		pItemData->nInvPage = (uint8_t)BITMANIP_Read(pBuffer, 3) - 1;
	}
	UNITS_ChangeAnimMode(pItem, nAnimMode);

	if (pItemData->dwItemFlags & IFLAG_ISEAR)
	{
		pItemData->dwFileIndex = BITMANIP_Read(pBuffer, 3);
		pItemData->nEarLvl = (uint8_t)BITMANIP_Read(pBuffer, 7);

		int i = 0;
		do
		{
			szChar = (char)BITMANIP_Read(pBuffer, 7);
			pItemData->szPlayerName[i] = szChar;
			++i;
		}
		while (szChar);
	}
	else
	{
		dwCode = BITMANIP_Read(pBuffer, 32);
		pItem->dwClassId = DATATBLS_GetItemIdFromItemCode(dwCode);

		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_GOLD))
		{
			if (BITMANIP_Read(pBuffer, 1))
			{
				nBits = 32;
			}
			else
			{
				nBits = 12;
			}
			STATLIST_SetUnitStat(pItem, STAT_GOLD, BITMANIP_Read(pBuffer, nBits), 0);
		}

		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
		{
			if (dwCode == ' cst')
			{
				pItemData->wMagicSuffix[0] = 0;
			}
			else if (dwCode == ' csi')
			{
				pItemData->wMagicSuffix[0] = 1;
			}
		}
	}

	if (dwVersion > 92)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		D2_ASSERT(pItemsTxtRecord);

		pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwCode, &nItemId);
		if (!pItemsTxtRecord)
		{
			return -1;
		}

		if (pItemsTxtRecord->nQuest && pItemsTxtRecord->nQuestDiffCheck)
		{
			pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_QUESTITEMDIFFICULTY);
			nValue = (BITMANIP_Read(pBuffer, pItemStatCostTxtRecord->nSaveBits) - pItemStatCostTxtRecord->dwSaveAdd) << pItemStatCostTxtRecord->nValShift;

			pStatList = STATLIST_GetStatListFromUnitStateOrFlag(pItem, STATE_NONE, 0x40);
			if (!pStatList)
			{
				pStatList = STATLIST_AllocStatList(pItem->pMemoryPool, 0x40, 0, UNIT_ITEM, pItem->dwUnitId);
				D2COMMON_10475_PostStatToStatList(pItem, pStatList, 1);
			}
			if (pStatList)
			{
				STATLIST_SetStat(pStatList, STAT_QUESTITEMDIFFICULTY, nValue, 0);
			}
		}
	}

	if (bCheckForHeader && dwVersion > 86 && BITMANIP_Read(pBuffer, 1))
	{
		pItemData->dwRealmData[0] = BITMANIP_Read(pBuffer, 32);
		pItemData->dwRealmData[1] = BITMANIP_Read(pBuffer, 32);

		if (dwVersion > 93)
		{
			BITMANIP_Read(pBuffer, 32);
		}
	}

	pItem->dwInitSeed = 0;
	SEED_InitLowSeed(&pItem->pSeed, 0);

	pItemData->dwItemLevel = 1;
	pItemData->dwQualityNo = ITEMQUAL_NORMAL;

	return 1;
}

//D2Common.0x6FDA0A20
int __fastcall ITEMS_DecodeItemBitstreamComplete(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, BOOL bCheckForHeader, BOOL bGamble, int* pSocketedItems, uint32_t dwVersion)
{
	static const int gnItemSetStates[] =
	{
		STATE_ITEMSET1, STATE_ITEMSET2, STATE_ITEMSET3, STATE_ITEMSET4, STATE_ITEMSET5
	};

	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2MagicAffixDataTbl* pMagicAffixDataTbl = NULL;
	D2SetItemsTxt* pSetItemsTxtRecord = NULL;
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2SetsTxt* pSetsTxtRecord = NULL;
	D2ItemDataStrc* pItemData = NULL;
	D2StatListStrc* pStatList = NULL;
	char* szName = NULL;
	int nMagicPrefixOffset = 0;
	int nAutoMagicOffset = 0;
	int nSocketedItems = 0;
	int nSetItemMask = 0;
	int nSetRecordId = 0;
	int nLastStatId = 0;
	int nMaxSockets = 0;
	int nStatLists = 0;
	int nAnimMode = 0;
	int nClassId = 0;
	int nSkillId = 0;
	int nStatId = 0;
	int nState = 0;
	int nValue = 0;
	int nBits = 0;
	int nFlag = 0;
	int nMax = 0;

	int v237, v238, v240, v241, v242, v243, v244, v245, v246, v247, v248, v249, v250, v251, v252; //TODO: Change names

	uint32_t dwCode = 0;
	char szChar = 0;
	BOOL bRuneword = FALSE;
	BOOL bError = FALSE;
	BOOL b109 = FALSE;

	pItemData = ITEMS_GetItemData(pItem);
	D2_ASSERT(pItemData);

	pItemData->wItemFormat = (uint16_t)BITMANIP_Read(pBuffer, 10);

	nAnimMode = BITMANIP_Read(pBuffer, 3);

	if (nAnimMode == IMODE_ONGROUND || nAnimMode == IMODE_DROPPING)
	{
		UNITS_SetXForStaticUnit(pItem, BITMANIP_Read(pBuffer, 16));
		UNITS_SetYForStaticUnit(pItem, BITMANIP_Read(pBuffer, 16));
	}
	else
	{
		pItemData->nBodyLoc = (uint8_t)BITMANIP_Read(pBuffer, 4);

		UNITS_SetXForStaticUnit(pItem, BITMANIP_Read(pBuffer, 4));
		UNITS_SetYForStaticUnit(pItem, BITMANIP_Read(pBuffer, 4));

		pItemData->nInvPage = (uint8_t)(BITMANIP_Read(pBuffer, 3) - 1);
	}

	UNITS_ChangeAnimMode(pItem, nAnimMode);

	dwCode = BITMANIP_Read(pBuffer, 32);

	nClassId = DATATBLS_GetItemIdFromItemCode(dwCode);
	if (nClassId >= 0)
	{
		pItem->dwClassId = nClassId;
	}

	if (bGamble)
	{
		pItemData->dwItemLevel = 1;
		pItemData->dwQualityNo = ITEMQUAL_INFERIOR;

		return 1;
	}

	nSocketedItems = BITMANIP_Read(pBuffer, 3);
	if (pSocketedItems)
	{
		*pSocketedItems = nSocketedItems;
	}

	if (bCheckForHeader)
	{
		pItem->dwInitSeed = BITMANIP_Read(pBuffer, 32);
		SEED_InitLowSeed(&pItem->pSeed, pItem->dwInitSeed);
	}

	pItemData->dwItemLevel = BITMANIP_Read(pBuffer, 7);
	if (pItemData->dwItemLevel < 1)
	{
		pItemData->dwItemLevel = 1;
	}

	pItemData->dwQualityNo = BITMANIP_Read(pBuffer, 4);

	if (BITMANIP_Read(pBuffer, 1))
	{
		pItemData->nInvGfxIdx = (uint8_t)BITMANIP_Read(pBuffer, 3);
	}

	pMagicAffixDataTbl = DATATBLS_GetMagicAffixDataTables();

	nMagicPrefixOffset = pMagicAffixDataTbl->pMagicPrefix - pMagicAffixDataTbl->pMagicAffixTxt;
	nAutoMagicOffset = pMagicAffixDataTbl->pAutoMagic - pMagicAffixDataTbl->pMagicAffixTxt;

	switch (dwVersion)
	{
	case 72:
	case 73:
	case 80:
	case 81:
	case 82:
		nMagicPrefixOffset -= 661;
		nAutoMagicOffset -= 1262;
		break;
	case 83:
	case 84:
		nMagicPrefixOffset -= 674;
		nAutoMagicOffset -= 1265;
		break;
	case 85:
	case 86:
	case 87:
		nMagicPrefixOffset -= 675;
		nAutoMagicOffset -= 1266;
		break;
	case 88:
		nMagicPrefixOffset -= 729;
		nAutoMagicOffset -= 1440;
		break;
	default:
		break;
	}

	if (BITMANIP_Read(pBuffer, 1))
	{
		pItemData->wAutoAffix = (uint16_t)BITMANIP_Read(pBuffer, 11);
		if (dwVersion > 88)
		{
			if (dwVersion <= 89)
			{
				++pItemData->wAutoAffix;
			}

			if (pItemData->wAutoAffix)
			{
				pItemData->wAutoAffix += nAutoMagicOffset;
			}
		}
		else
		{
			pItemData->wAutoAffix = 0;
		}
	}
	else
	{
		pItemData->wAutoAffix = 0;
	}

	switch (pItemData->dwQualityNo)
	{
	case ITEMQUAL_INFERIOR:
	case ITEMQUAL_SUPERIOR:
	{
		pItemData->dwFileIndex = BITMANIP_Read(pBuffer, 3);
		break;
	}
	case ITEMQUAL_NORMAL:
	{
		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_CHARM))
		{
			if (bCheckForHeader || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
			{
				if (BITMANIP_Read(pBuffer, 1))
				{
					pItemData->wMagicPrefix[0] = (uint16_t)BITMANIP_Read(pBuffer, 11);
					if (pItemData->wMagicPrefix[0])
					{
						pItemData->wMagicPrefix[0] += nMagicPrefixOffset;
					}
				}
				else
				{
					pItemData->wMagicSuffix[0] = (uint16_t)BITMANIP_Read(pBuffer, 11);
				}
			}
		}

		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BODY_PART) && !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_PLAYER_BODY_PART))
		{
			pItemData->dwFileIndex = BITMANIP_Read(pBuffer, 10);
		}

		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
		{
			pItemData->wMagicSuffix[0] = (uint16_t)BITMANIP_Read(pBuffer, 5);
		}

		break;
	}
	case ITEMQUAL_RARE:
	case ITEMQUAL_CRAFT:
	{
		if (bCheckForHeader || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
		{
			pItemData->wRarePrefix = (uint16_t)BITMANIP_Read(pBuffer, 8);
			pItemData->wRareSuffix = (uint16_t)BITMANIP_Read(pBuffer, 8);
		}

		for (int i = 0; i < ITEMS_MAX_MODS; ++i)
		{
			if (BITMANIP_Read(pBuffer, 1))
			{
				pItemData->wMagicPrefix[i] = (uint16_t)BITMANIP_Read(pBuffer, 11);
				if (dwVersion <= 89)
				{
					++pItemData->wMagicPrefix[i];
				}

				if (pItemData->wMagicPrefix[i])
				{
					pItemData->wMagicPrefix[i] += nMagicPrefixOffset;
				}
			}
			else
			{
				pItemData->wMagicPrefix[i] = 0;
			}

			if (BITMANIP_Read(pBuffer, 1))
			{
				pItemData->wMagicSuffix[i] = (uint16_t)BITMANIP_Read(pBuffer, 11);
				if (dwVersion <= 89)
				{
					++pItemData->wMagicSuffix[i];
				}
			}
			else
			{
				pItemData->wMagicSuffix[i] = 0;
			}
		}

		break;
	}
	case ITEMQUAL_TEMPERED:
	{
		if (bCheckForHeader || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
		{
			pItemData->wRarePrefix = (uint16_t)BITMANIP_Read(pBuffer, 8);
			pItemData->wRareSuffix = (uint16_t)BITMANIP_Read(pBuffer, 8);
		}

		break;
	}
	case ITEMQUAL_MAGIC:
	{
		if (bCheckForHeader || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
		{
			pItemData->wMagicPrefix[0] = (uint16_t)BITMANIP_Read(pBuffer, 11);
			if (pItemData->wMagicPrefix[0])
			{
				pItemData->wMagicPrefix[0] += nMagicPrefixOffset;
			}

			pItemData->wMagicSuffix[0] = (uint16_t)BITMANIP_Read(pBuffer, 11);
		}

		break;
	}
	case ITEMQUAL_UNIQUE:
	{
		if (bCheckForHeader || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
		{
			pItemData->dwFileIndex = BITMANIP_Read(pBuffer, 12);
			if (pItemData->dwFileIndex < 0 || pItemData->dwFileIndex >= sgptDataTables->nUniqueItemsTxtRecordCount)
			{
				pItemData->dwFileIndex = -1;
			}
		}

		break;
	}
	case ITEMQUAL_SET:
	{
		if (bCheckForHeader || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
		{
			bError = TRUE;

			nSetRecordId = BITMANIP_Read(pBuffer, 12);
			if (dwVersion > 92)
			{
				if (nSetRecordId >= 0 && nSetRecordId < sgptDataTables->nSetItemsTxtRecordCount)
				{
					pSetItemsTxtRecord = &sgptDataTables->pSetItemsTxt[nSetRecordId];
					if (pSetItemsTxtRecord)
					{
						pItemData->dwFileIndex = pSetItemsTxtRecord->wSetItemId;
						bError = FALSE;
					}
				}
			}
			else
			{
				if (nSetRecordId >= 0 && nSetRecordId < sgptDataTables->nSetsTxtRecordCount)
				{
					pSetsTxtRecord = &sgptDataTables->pSetsTxt[nSetRecordId];
					if (pSetsTxtRecord)
					{
						for (int i = 0; i < pSetsTxtRecord->nSetItems; ++i)
						{
							if (pSetsTxtRecord->pSetItem[i]->szItemCode == dwCode)
							{
								pItemData->dwFileIndex = pSetsTxtRecord->pSetItem[i]->wSetItemId;
								bError = FALSE;
								break;
							}
						}
					}
				}
			}
		}
		break;
	}
	default:
	{
		bError = TRUE;
		break;
	}
	}

	if (pItemData->dwItemFlags & IFLAG_RUNEWORD)
	{
		bRuneword = TRUE;
		pItemData->wMagicPrefix[0] = (uint16_t)BITMANIP_Read(pBuffer, 16);
	}

	if (pItemData->dwItemFlags & IFLAG_ISEAR)
	{
		pItemData->dwFileIndex = BITMANIP_Read(pBuffer, 3);
		pItemData->nEarLvl = (uint8_t)BITMANIP_Read(pBuffer, 7);

		szName = pItemData->szPlayerName;
		do
		{
			szChar = (char)BITMANIP_Read(pBuffer, 7);
			*szName++ = szChar;
		}
		while (szChar);
	}
	else if (pItemData->dwItemFlags & IFLAG_PERSONALIZED)
	{
		szName = pItemData->szPlayerName;
		do
		{
			szChar = (char)BITMANIP_Read(pBuffer, 7);
			*szName++ = szChar;
		}
		while (szChar);
	}

	if (bCheckForHeader && dwVersion > 86 && BITMANIP_Read(pBuffer, 1))
	{
		pItemData->dwRealmData[0] = BITMANIP_Read(pBuffer, 32);
		pItemData->dwRealmData[1] = BITMANIP_Read(pBuffer, 32);

		if (dwVersion > 93)
		{
			BITMANIP_Read(pBuffer, 32);
		}
	}

	if (dwVersion < 93)
	{
		b109 = TRUE;
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);

	if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_ARMOR))
	{
		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_ARMORCLASS);
		nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
		STATLIST_SetUnitStat(pItem, STAT_ARMORCLASS, nValue, 0);

		STATLIST_SetUnitStat(pItem, STAT_TOBLOCK, pItemsTxtRecord->nBlock, 0);
		STATLIST_SetUnitStat(pItem, STAT_VELOCITYPERCENT, -pItemsTxtRecord->dwSpeed, 0);

		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAXDURABILITY);
		nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
		STATLIST_SetUnitStat(pItem, STAT_MAXDURABILITY, nValue, 0);

		if (nValue)
		{
			pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_DURABILITY);

			nBits = (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits);

			if (dwVersion <= 95)
			{
				nBits = 8;
			}

			nValue = BITMANIP_Read(pBuffer, nBits) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
			STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nValue, 0);
		}
	}
	else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
	{
		STATLIST_SetUnitStat(pItem, STAT_ATTACKRATE, -pItemsTxtRecord->dwSpeed, 0);

		if (pItemData->dwQualityNo == ITEMQUAL_INFERIOR)
		{
			if (pItemsTxtRecord->nMaxDam)
			{
				nValue = 3 * pItemsTxtRecord->nMaxDam / 4;
				if (nValue < 2)
				{
					nValue = 2;
				}
				STATLIST_SetUnitStat(pItem, STAT_MAXDAMAGE, nValue, 0);
			}

			if (pItemsTxtRecord->nMinDam)
			{
				nValue = 3 * pItemsTxtRecord->nMinDam / 4;
				if (nValue < 1)
				{
					nValue = 1;
				}
				STATLIST_SetUnitStat(pItem, STAT_MINDAMAGE, nValue, 0);
			}

			if (pItemsTxtRecord->n2HandMaxDam)
			{
				nValue = 3 * pItemsTxtRecord->n2HandMaxDam / 4;
				if (nValue < 2)
				{
					nValue = 2;
				}
				STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MAXDAMAGE, nValue, 0);
			}

			if (pItemsTxtRecord->n2HandMinDam)
			{
				nValue = 3 * pItemsTxtRecord->n2HandMinDam / 4;
				if (nValue < 1)
				{
					nValue = 1;
				}
				STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MINDAMAGE, nValue, 0);
			}

			if (pItemsTxtRecord->nMaxMisDam)
			{
				nValue = 3 * pItemsTxtRecord->nMinMisDam / 4;
				if (nValue < 1)
				{
					nValue = 1;
				}
				STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MINDAMAGE, nValue, 0);

				nValue = 3 * pItemsTxtRecord->nMaxMisDam / 4;
				if (nValue < 2)
				{
					nValue = 2;
				}
				STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MAXDAMAGE, nValue, 0);
			}
		}
		else
		{
			STATLIST_SetUnitStat(pItem, STAT_MAXDAMAGE, pItemsTxtRecord->nMaxDam, 0);
			STATLIST_SetUnitStat(pItem, STAT_MINDAMAGE, pItemsTxtRecord->nMinDam, 0);

			STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MAXDAMAGE, pItemsTxtRecord->n2HandMaxDam, 0);
			STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MINDAMAGE, pItemsTxtRecord->n2HandMinDam, 0);

			if (pItemsTxtRecord->nMaxMisDam)
			{
				STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MINDAMAGE, pItemsTxtRecord->nMinMisDam, 0);
				STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MAXDAMAGE, pItemsTxtRecord->nMaxMisDam, 0);
			}
		}

		if (pItemData->dwItemFlags & IFLAG_ETHEREAL)
		{
			STATLIST_SetUnitStat(pItem, STAT_MINDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_MINDAMAGE, 0) / 2, 0);
			STATLIST_SetUnitStat(pItem, STAT_MAXDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_MAXDAMAGE, 0) / 2, 0);
			STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MINDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_SECONDARY_MINDAMAGE, 0) / 2, 0);
			STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MAXDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_SECONDARY_MAXDAMAGE, 0) / 2, 0);
			STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MINDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_ITEM_THROW_MINDAMAGE, 0) / 2, 0);
			STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MAXDAMAGE, 3 * STATLIST_GetUnitBaseStat(pItem, STAT_ITEM_THROW_MAXDAMAGE, 0) / 2, 0);
		}

		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAXDURABILITY);
		nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
		STATLIST_SetUnitStat(pItem, STAT_MAXDURABILITY, nValue, 0);

		if (nValue)
		{
			pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_DURABILITY);

			nBits = (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits);

			if (dwVersion <= 95)
			{
				nBits = 8;
			}

			nValue = BITMANIP_Read(pBuffer, nBits) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
			STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nValue, 0);
		}
	}
	else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_GOLD))
	{
		if (BITMANIP_Read(pBuffer, 1))
		{
			nBits = 32;
		}
		else
		{
			nBits = 12;
		}

		STATLIST_SetUnitStat(pItem, STAT_GOLD, BITMANIP_Read(pBuffer, nBits), 0);
	}

	if (pItemsTxtRecord->nStackable)
	{
		if (dwVersion > 80)
		{
			nBits = 9;
		}
		else
		{
			nBits = 8;
		}

		STATLIST_SetUnitStat(pItem, STAT_QUANTITY, BITMANIP_Read(pBuffer, nBits), 0);
	}
	
	if (pItemData->dwItemFlags & IFLAG_SOCKETED)
	{
		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_ITEM_NUMSOCKETS);

		nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits));

		nMaxSockets = pItemsTxtRecord->nInvWidth * pItemsTxtRecord->nInvHeight;

		if (nMaxSockets > 0)
		{
			if (nMaxSockets > 6)
			{
				nMaxSockets = 6;
			}

			if (nMaxSockets > ITEMS_GetMaxSockets(pItem))
			{
				nMaxSockets = ITEMS_GetMaxSockets(pItem);
			}

			if (nValue < 1)
			{
				nValue = 1;
			}

			if (nValue > nMaxSockets)
			{
				nValue = nMaxSockets;
			}

			ITEMS_SetItemFlag(pItem, IFLAG_SOCKETED, TRUE);
			STATLIST_SetUnitStat(pItem, STAT_ITEM_NUMSOCKETS, nValue, 0);
		}
	}

	if (!bCheckForHeader && !(pItemData->dwItemFlags & IFLAG_IDENTIFIED))
	{
		return 2 * (bError == 0) - 1;
	}

	if (dwVersion > 84 && pItemData->dwQualityNo == ITEMQUAL_SET)
	{
		nStatLists = 5;
		nSetItemMask = BITMANIP_Read(pBuffer, 5);
	}
	else
	{
		nStatLists = 0;
		nSetItemMask = 0;
	}

	if (bRuneword)
	{
		++nStatLists;
	}

	for (int nStatListCounter = -1; nStatListCounter < nStatLists; ++nStatListCounter)
	{
		if (nStatListCounter < 0)
		{
			nState = 0;
			nFlag = 0x40;
		}
		else if (bRuneword && nStatListCounter == nStatLists - 1)
		{
			nState = STATE_RUNEWORD;
			nFlag = 0x40;
		}
		else if (nSetItemMask & (1 << nStatListCounter))
		{
			nState = gnItemSetStates[nStatListCounter];
			nFlag = 0x2040;
		}
		else
		{
			continue;
		}
		
		pStatList = STATLIST_GetStatListFromUnitStateOrFlag(pItem, nState, nFlag);
		if (!pStatList)
		{
			pStatList = STATLIST_AllocStatList(pItem->pMemoryPool, nFlag, 0, UNIT_ITEM, pItem->dwUnitId);
			D2COMMON_10475_PostStatToStatList(pItem, pStatList, 1);
			STATLIST_SetState(pStatList, nState);
		}

		nLastStatId = -1;
		while (1)
		{
			nStatId = BITMANIP_Read(pBuffer, 9);
			if (nStatId == 511)
			{
				break;
			}

			if (!nStatId && !nLastStatId)
			{
				bError = TRUE;
				break;
			}

			nLastStatId = nStatId;

			pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
			if (!pItemStatCostTxtRecord)
			{
				break;
			}

			switch (nStatId)
			{
			case STAT_STRENGTH:
			case STAT_DEXTERITY:
			{
				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, nStatId, nValue, 0);
				break;
			}
			case STAT_VITALITY:
			{
				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_VITALITY, nValue, 0);

				if (dwVersion <= 85)
				{
					pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAXHP);

					nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
					STATLIST_SetStatIfListIsValid(pStatList, STAT_MAXHP, nValue << 8, 0);
				}
				break;
			}
			case STAT_ENERGY:
			{
				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_ENERGY, nValue, 0);

				if (dwVersion <= 85)
				{
					pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAXMANA);

					nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
					STATLIST_SetStatIfListIsValid(pStatList, STAT_MAXMANA, nValue, 0); //TODO: nValue << 8?
				}
				break;
			}
			case STAT_ITEM_MAXDAMAGE_PERCENT:
			{
				ITEMS_SetDefenseOrDamage(pItem, STAT_ITEM_MAXDAMAGE_PERCENT);

				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_ITEM_MAXDAMAGE_PERCENT, nValue, 0);

				ITEMS_SetDefenseOrDamage(pItem, STAT_ITEM_MINDAMAGE_PERCENT);

				pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_ITEM_MINDAMAGE_PERCENT);

				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_ITEM_MINDAMAGE_PERCENT, nValue, 0);
				break;
			}
			case STAT_FIREMINDAM:
			{
				nBits = (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits);
				if (dwVersion <= 81)
				{
					nBits = 6;
				}

				nValue = BITMANIP_Read(pBuffer, nBits) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_FIREMINDAM, nValue, 0);

				pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_FIREMAXDAM);

				nBits = (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits);
				if (dwVersion <= 81)
				{
					nBits = 7;
				}

				nValue = BITMANIP_Read(pBuffer, nBits) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_FIREMAXDAM, nValue, 0);
				break;
			}
			case STAT_LIGHTMINDAM:
			{
				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_LIGHTMINDAM, nValue, 0);

				pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_LIGHTMAXDAM);

				nBits = (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits);
				if (dwVersion <= 81)
				{
					nBits = 7;
				}

				nValue = BITMANIP_Read(pBuffer, nBits) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_LIGHTMAXDAM, nValue, 0);
				break;
			}
			case STAT_MAGICMINDAM:
			{
				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_MAGICMINDAM, nValue, 0);

				pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAGICMAXDAM);

				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_MAGICMAXDAM, nValue, 0);
				break;
			}
			case STAT_COLDMINDAM:
			{
				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_COLDMINDAM, nValue, 0);

				pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_COLDMAXDAM);

				nBits = (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits);
				if (dwVersion <= 81)
				{
					nBits = 7;
				}

				nValue = BITMANIP_Read(pBuffer, nBits) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_COLDMAXDAM, nValue, 0);

				pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_COLDLENGTH);

				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_COLDLENGTH, nValue, 0);
				break;
			}
			case STAT_POISONMINDAM:
			{
				nBits = (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits);
				if (dwVersion <= 81)
				{
					nBits = 7;
				}

				nValue = BITMANIP_Read(pBuffer, nBits) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONMINDAM, nValue, 0);

				pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_POISONMAXDAM);

				nBits = (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits);
				if (dwVersion <= 81)
				{
					nBits = 8;
				}

				nValue = BITMANIP_Read(pBuffer, nBits) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONMAXDAM, nValue, 0);

				pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_POISONLENGTH);

				nValue = BITMANIP_Read(pBuffer, (b109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits)) - (b109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONLENGTH, nValue, 0);
				STATLIST_SetStatIfListIsValid(pStatList, STAT_POISON_COUNT, 1, 0);
				break;
			}
			case STAT_ITEM_FREEZE:
			{
				if (dwVersion > 89)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, STAT_ITEM_FREEZE, dwVersion, b109);
				}
				else
				{
					BITMANIP_Read(pBuffer, 16);
					STATLIST_SetStatIfListIsValid(pStatList, STAT_ITEM_FREEZE, 1, 0);
				}
				break;
			}
			case STAT_ITEM_ADDCLASSSKILLS:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, STAT_ITEM_ADDCLASSSKILLS, dwVersion, b109);
				}
				else
				{
					STATLIST_SetStat(pStatList, STAT_ITEM_ADDCLASSSKILLS, BITMANIP_Read(pBuffer, 3), 0);
				}
				break;
			}
			case STAT_UNSENTPARAM1:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, STAT_UNSENTPARAM1, dwVersion, b109);
				}
				else
				{
					STATLIST_SetStat(pStatList, STAT_ITEM_ADDCLASSSKILLS, BITMANIP_Read(pBuffer, 3), 3);
				}
				break;
			}
			case STAT_ITEM_ADDEXPERIENCE:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, STAT_ITEM_ADDEXPERIENCE, dwVersion, b109);
				}
				else
				{
					STATLIST_SetStat(pStatList, STAT_ITEM_ADDCLASSSKILLS, BITMANIP_Read(pBuffer, 3), 2);
				}
				break;
			}
			case STAT_ITEM_HEALAFTERKILL:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, STAT_ITEM_HEALAFTERKILL, dwVersion, b109);
				}
				else
				{
					STATLIST_SetStat(pStatList, STAT_ITEM_ADDCLASSSKILLS, BITMANIP_Read(pBuffer, 3), 1);
				}
				break;
			}
			case STAT_ITEM_REDUCEDPRICES:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, STAT_ITEM_REDUCEDPRICES, dwVersion, b109);
				}
				else
				{
					STATLIST_SetStat(pStatList, STAT_ITEM_ADDCLASSSKILLS, BITMANIP_Read(pBuffer, 3), 4);
				}
				break;
			}
			case STAT_ATTACK_VS_MONTYPE:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, STAT_ATTACK_VS_MONTYPE, dwVersion, b109);
				}
				else
				{
					STATLIST_SetStat(pStatList, STAT_ITEM_ADDCLASSSKILLS, BITMANIP_Read(pBuffer, 3), 5);
				}
				break;
			}
			case STAT_DAMAGE_VS_MONTYPE:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, STAT_DAMAGE_VS_MONTYPE, dwVersion, b109);
				}
				else
				{
					STATLIST_SetStat(pStatList, STAT_ITEM_ADDCLASSSKILLS, BITMANIP_Read(pBuffer, 3), 6);
				}
				break;
			}
			case STAT_ITEM_ADDSKILL_TAB:
			case STAT_UNUSED189:
			case STAT_UNUSED190:
			case STAT_UNUSED191:
			case STAT_UNUSED192:
			case STAT_UNUSED193:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, nStatId, dwVersion, b109);
				}
				else
				{
					D2Common_10848(BITMANIP_Read(pBuffer, 10), &v238, &v237, &nValue);
					if (nValue > 7)
					{
						nValue = 7;
					}
					STATLIST_SetStat(pStatList, STAT_ITEM_ADDSKILL_TAB, nValue, v237 + 8 * v238);
				}
				break;
			}
			case STAT_ITEM_ELEMSKILL:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, STAT_ITEM_ELEMSKILL, dwVersion, b109);
				}
				else
				{
					STATLIST_SetStat(pStatList, STAT_ITEM_ELEMSKILL, BITMANIP_Read(pBuffer, 4), 1);
				}
				break;
			}
			case STAT_ITEM_SINGLESKILL:
			case STAT_ITEM_RESTINPEACE:
			case STAT_CURSE_RESISTANCE:
			case STAT_FADE:
			case STAT_ARMOR_OVERRIDE_PERCENT:
			case STAT_UNUSED183:
			case STAT_UNUSED184:
			case STAT_UNUSED185:
			case STAT_UNUSED186:
			case STAT_UNUSED187:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, nStatId, dwVersion, b109);
				}
				else
				{
					D2Common_10844_ITEMMODS_First(BITMANIP_Read(pBuffer, 14), &nSkillId, &nValue);

					pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_ITEM_SINGLESKILL);

					if (pItemStatCostTxtRecord)
					{
						nMax = (1 << pItemStatCostTxtRecord->nSaveBits) - 1;
					}
					else
					{
						nMax = 7;
					}

					if (nValue > 0 && nValue <= nMax)
					{
						pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
						if (pSkillsTxtRecord && DATATBLS_GetSkillDescTxtRecord(pSkillsTxtRecord->wSkillDesc))
						{
							STATLIST_SetStat(pStatList, STAT_ITEM_SINGLESKILL, nValue, nSkillId);
						}
					}
				}
				break;
			}
			case STAT_ITEM_SKILLONATTACK:
			case STAT_ITEM_SKILLONKILL:
			case STAT_ITEM_SKILLONDEATH:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, nStatId, dwVersion, b109);
				}
				else
				{
					D2Common_10845(BITMANIP_Read(pBuffer, 21), &v241, &v240, &v242);
					STATLIST_SetStat(pStatList, STAT_ITEM_SKILLONATTACK, v242, ((uint16_t)v241 << 6) + (v240 & 0x3F));
				}
				break;
			}
			case STAT_ITEM_SKILLONHIT:
			case STAT_ITEM_SKILLONLEVELUP:
			case STAT_UNUSED200:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, nStatId, dwVersion, b109);
				}
				else
				{
					D2Common_10845(BITMANIP_Read(pBuffer, 21), &v244, &v243, &v245);
					STATLIST_SetStat(pStatList, STAT_ITEM_SKILLONHIT, v245, ((uint16_t)v244 << 6) + (v243 & 0x3F));
				}
				break;
			}
			case STAT_ITEM_SKILLONGETHIT:
			case STAT_UNUSED202:
			case STAT_UNUSED203:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, nStatId, dwVersion, b109);
				}
				else
				{
					D2Common_10845(BITMANIP_Read(pBuffer, 21), &v247, &v246, &v248);
					STATLIST_SetStat(pStatList, STAT_ITEM_SKILLONGETHIT, v248, ((uint16_t)v247 << 6) + (v246 & 0x3F));
				}
				break;
			}
			case STAT_ITEM_CHARGED_SKILL:
			case STAT_UNUSED204:
			case STAT_UNUSED205:
			case STAT_UNUSED206:
			case STAT_UNUSED207:
			case STAT_UNUSED208:
			case STAT_UNUSED209:
			case STAT_UNUSED210:
			case STAT_UNUSED211:
			case STAT_UNUSED212:
			{
				if (dwVersion > 92)
				{
					ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, nStatId, dwVersion, b109);
				}
				else
				{
					if (dwVersion > 73)
					{
						nBits = 30;
					}
					else
					{
						nBits = 29;
					}

					D2Common_10846(BITMANIP_Read(pBuffer, nBits), &v249, &v250, &v251, &v252);

					STATLIST_SetStat(pStatList, STAT_ITEM_CHARGED_SKILL, (v252 << 8) + (uint8_t)v251, (v250 & LOWORD(sgptDataTables->nShiftedStuff)) + ((uint16_t)v249 << sgptDataTables->nStuff));
				}
				break;
			}
			default:
			{
				ITEMS_ReadStatFromItemBitstream(pBuffer, pStatList, pItemStatCostTxtRecord, nStatId, dwVersion, b109);
				break;
			}
			}
		}
			
		if (dwVersion <= 92)
		{
			nValue = STATLIST_GetStatValue(pStatList, STAT_ITEM_ADDCLASSSKILLS, 0);

			if (nValue > 0)
			{
				if (nValue == STATLIST_GetStatValue(pStatList, STAT_ITEM_ADDCLASSSKILLS, 1) && nValue == STATLIST_GetStatValue(pStatList, STAT_ITEM_ADDCLASSSKILLS, 2)
					&& nValue == STATLIST_GetStatValue(pStatList, STAT_ITEM_ADDCLASSSKILLS, 3) && nValue == STATLIST_GetStatValue(pStatList, STAT_ITEM_ADDCLASSSKILLS, 4))
				{
					for (int i = 0; i < 7; ++i)
					{
						STATLIST_SetStatIfListIsValid(pStatList, STAT_ITEM_ADDCLASSSKILLS, 0, i);
					}

					STATLIST_SetStatIfListIsValid(pStatList, STAT_ITEM_ALLSKILLS, nValue, 0);
				}
			}
		}
	}

	return 2 * (bError == FALSE) - 1;
}

//D2Common.0x6FDA2690
void __fastcall ITEMS_SetDefenseOrDamage(D2UnitStrc* pItem, int nStat)
{
	int nMaxAc = 0;
	int nItemId = 0;


	D2_ASSERT(pItem);
	D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);
	const D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);

	pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwCode, &nItemId);
	if (pItemsTxtRecord)
	{
		switch (nStat)
		{
		case STAT_ITEM_ARMOR_PERCENT:
		case STAT_ARMORCLASS:
			if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_ARMOR) && pItemsTxtRecord->dwMaxAc)
			{
				nMaxAc = STATLIST_GetUnitBaseStat(pItem, STAT_ARMORCLASS, 0) + 1;
				if (nMaxAc <= pItemsTxtRecord->dwMaxAc)
				{
					nMaxAc = pItemsTxtRecord->dwMaxAc + 1;
				}

				STATLIST_SetUnitStat(pItem, STAT_ARMORCLASS, nMaxAc, 0);
			}
			break;

		case STAT_ITEM_MAXDAMAGE_PERCENT:
		case STAT_MAXDAMAGE:
			if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
			{
				if (pItemsTxtRecord->nMaxDam && STATLIST_GetUnitBaseStat(pItem, STAT_MAXDAMAGE, 0) < pItemsTxtRecord->nMaxDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_MAXDAMAGE, pItemsTxtRecord->nMaxDam, 0);
				}

				if (pItemsTxtRecord->n2HandMaxDam && STATLIST_GetUnitBaseStat(pItem, STAT_SECONDARY_MAXDAMAGE, 0) < pItemsTxtRecord->n2HandMaxDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MAXDAMAGE, pItemsTxtRecord->n2HandMaxDam, 0);
				}

				if (ITEMS_CheckIfThrowable(pItem) && pItemsTxtRecord->nMaxMisDam && STATLIST_GetUnitBaseStat(pItem, STAT_ITEM_THROW_MAXDAMAGE, 0) < pItemsTxtRecord->nMaxMisDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_ITEM_THROW_MAXDAMAGE, pItemsTxtRecord->nMaxMisDam, 0);
				}
			}
			break;

		case STAT_ITEM_MINDAMAGE_PERCENT:
		case STAT_MINDAMAGE:
			if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
			{
				if (pItemsTxtRecord->nMinDam && STATLIST_GetUnitBaseStat(pItem, STAT_MINDAMAGE, 0) < pItemsTxtRecord->nMinDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_MINDAMAGE, pItemsTxtRecord->nMinDam, 0);
				}

				if (pItemsTxtRecord->n2HandMinDam && STATLIST_GetUnitBaseStat(pItem, STAT_SECONDARY_MINDAMAGE, 0) < pItemsTxtRecord->n2HandMinDam)
				{
					STATLIST_SetUnitStat(pItem, STAT_SECONDARY_MINDAMAGE, pItemsTxtRecord->n2HandMinDam, 0);
				}

				if (ITEMS_CheckIfThrowable(pItem) && pItemsTxtRecord->nMinMisDam && STATLIST_GetUnitBaseStat(pItem, STAT_ITEM_THROW_MINDAMAGE, 0) < pItemsTxtRecord->nMinMisDam)
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

//D2Common.0x6FDA29D0
void __fastcall ITEMS_ReadStatFromItemBitstream(D2BitBufferStrc* pBuffer, D2StatListStrc* pStatList, D2ItemStatCostTxt* pItemStatCostTxtRecord, int nStatId, uint32_t dwVersion, int n109)
{
	int nSaveAdd = n109 ? pItemStatCostTxtRecord->dw09SaveAdd : pItemStatCostTxtRecord->dwSaveAdd;
	int nParamBits = n109 ? pItemStatCostTxtRecord->dw09SaveParamBits : pItemStatCostTxtRecord->dwSaveParamBits;
	int nSaveBits = n109 ? pItemStatCostTxtRecord->n09SaveBits : pItemStatCostTxtRecord->nSaveBits;
	int nParam = 0;
	int nValue = 0;

	switch (nStatId)
	{
	case STAT_SECONDARY_MINDAMAGE:
		if (dwVersion <= 89)
		{
			nSaveBits = 5;
		}
		break;
	case STAT_SECONDARY_MAXDAMAGE:
		if (dwVersion <= 89)
		{
			nSaveBits = 6;
		}
		break;
	case STAT_HPREGEN:
		if (dwVersion <= 89)
		{
			nSaveBits = 5;
			nSaveAdd = 10;
		}
		break;
	case STAT_ITEM_GOLDBONUS:
		if (dwVersion <= 89)
		{
			nSaveBits = 8;
			nSaveAdd = 20;
		}
		break;
	case STAT_ITEM_MAGICBONUS:
		if (dwVersion <= 0x59)
		{
			nSaveBits = 7;
			nSaveAdd = 20;
		}
		break;
	case STAT_ITEM_FASTERATTACKRATE:
	case STAT_ITEM_FASTERMOVEVELOCITY:
	case STAT_ITEM_FASTERGETHITRATE:
	case STAT_ITEM_FASTERBLOCKRATE:
	case STAT_ITEM_FASTERCASTRATE:
		if (dwVersion <= 87)
		{
			nSaveBits = 6;
		}
		break;
	case STAT_ITEM_LEVELREQ:
	case STAT_ITEM_LEVELREQPCT:
		if (dwVersion < 93)
		{
			nStatId = 93;
		}
		break;
	case STAT_LASTBLOCKFRAME:
	case STAT_ITEM_NONCLASSSKILL:
		if (dwVersion < 93)
		{
			nStatId = 96;
		}
		break;
	case STAT_STATE:
	case STAT_MONSTER_PLAYERCOUNT:
		if (dwVersion < 93)
		{
			nStatId = 99;
		}
		break;
	case STAT_SKILL_POISON_OVERRIDE_LENGTH:
	case STAT_SKILL_BYPASS_UNDEAD:
		if (dwVersion < 93)
		{
			nStatId = 102;
		}
		break;
	case STAT_SKILL_BYPASS_DEMONS:
	case STAT_SKILL_BYPASS_BEASTS:
		if (dwVersion < 93)
		{
			nStatId = 105;
		}
		break;
	default:
		break;
	}
	if (nParamBits > 0)
	{
		nParam = BITMANIP_Read(pBuffer, nParamBits);
	}

	nValue = BITMANIP_Read(pBuffer, nSaveBits);
	if (pStatList)
	{
		STATLIST_SetStat(pStatList, nStatId, (nValue - nSaveAdd) << pItemStatCostTxtRecord->nValShift, nParam);
	}
}

//D2Common.0x6FDA2BA0 (#10881)
size_t __stdcall ITEMS_SerializeItemToBitstream(D2UnitStrc* pItem, uint8_t* pBitstream, size_t nSize, BOOL bServer, BOOL bSaveItemInv, BOOL bGamble)
{
	D2BitBufferStrc pBuffer = {};

	BITMANIP_Initialize(&pBuffer, pBitstream, nSize);

	ITEMS_SerializeItem(pItem, &pBuffer, bServer, bSaveItemInv, bGamble);

	if (pBuffer.bFull)
	{
		return 0;
	}
	else
	{
		return BITMANIP_GetSize(&pBuffer);
	}
}

//Inlined in D2Common.0x6FDA2C00
void __fastcall ITEMS_SerializeItemCompact(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, D2ItemsTxt* pItemsTxtRecord, BOOL bServer)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	char* szName = NULL;
	int nItemFormat = 0;
	int nValue = 0;
	int nGold = 0;
	int nX = 0;
	int nY = 0;
	uint8_t nBodyLoc = 0;

	int v39; // [sp+24h] [bp-8h]@63
	int v40; // [sp+28h] [bp-4h]@63


	nItemFormat = ITEMS_GetItemFormat(pItem);
	if (nItemFormat > 0)
	{
		if (nItemFormat >= 1023)
		{
			nItemFormat = 1023;
		}
	}
	else
	{
		nItemFormat = 0;
	}
	BITMANIP_Write(pBuffer, nItemFormat, 10);

	ITEMS_WriteBitsToBitstream(pBuffer, pItem->dwAnimMode, 3);

	if (pItem->dwAnimMode == IMODE_ONGROUND || pItem->dwAnimMode == IMODE_DROPPING)
	{
		ITEMS_WriteBitsToBitstream(pBuffer, UNITS_GetXPosition(pItem), 16);
		ITEMS_WriteBitsToBitstream(pBuffer, UNITS_GetYPosition(pItem), 16);
	}
	else
	{
		ITEMS_WriteBitsToBitstream(pBuffer, ITEMS_GetBodyLocation(pItem), 4);

		nX = UNITS_GetXPosition(pItem);
		if (nX > 0)
		{
			if (nX >= 15)
			{
				nX = 15;
			}
		}
		else
		{
			nX = 0;
		}
		ITEMS_WriteBitsToBitstream(pBuffer, nX, 4);

		nY = UNITS_GetYPosition(pItem);
		if (nY > 0)
		{
			if (nY >= 15)
			{
				nY = 15;
			}
		}
		else
		{
			nY = 0;
		}
		ITEMS_WriteBitsToBitstream(pBuffer, nY, 4);

		ITEMS_WriteBitsToBitstream(pBuffer, (ITEMS_GetInvPage(pItem) + 1), 3);
	}

	if (ITEMS_GetItemFlags(pItem) & IFLAG_ISEAR)
	{
		ITEMS_WriteBitsToBitstream(pBuffer, ITEMS_GetFileIndex(pItem), 3);

		ITEMS_WriteBitsToBitstream(pBuffer, ITEMS_GetEarLevel(pItem), 7);

		szName = ITEMS_GetEarName(pItem);
		do
		{
			ITEMS_WriteBitsToBitstream(pBuffer, *szName, 7);
		}
		while (*szName++);
	}
	else
	{
		ITEMS_WriteBitsToBitstream(pBuffer, ITEMS_GetBaseCode(pItem), 32);

		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_GOLD))
		{
			nGold = STATLIST_UnitGetStatValue(pItem, STAT_GOLD, 0);
			ITEMS_WriteBitsToBitstream(pBuffer, (nGold >= 4096), 1);
			ITEMS_WriteBitsToBitstream(pBuffer, nGold, (((nGold < 4096) - 1) & 20) + 12);
		}
	}

	if (pItemsTxtRecord->nQuest && pItemsTxtRecord->nQuestDiffCheck)
	{
		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_QUESTITEMDIFFICULTY);
		D2_ASSERT(pItemStatCostTxtRecord);

		nValue = STATLIST_UnitGetStatValue(pItem, STAT_QUESTITEMDIFFICULTY, 0);
		ITEMS_WriteBitsToBitstream(pBuffer, pItemStatCostTxtRecord->dwSaveAdd + (nValue >> pItemStatCostTxtRecord->nValShift), pItemStatCostTxtRecord->nSaveBits);
	}

	if (bServer)
	{
		v40 = 0;
		v39 = 0;
		ITEMS_GetRealmData(pItem, &v40, &v39);
		if (v39)
		{
			ITEMS_WriteBitsToBitstream(pBuffer, 1, 1);
			ITEMS_WriteBitsToBitstream(pBuffer, v40, 32);
			ITEMS_WriteBitsToBitstream(pBuffer, v39, 32);
			ITEMS_WriteBitsToBitstream(pBuffer, 0, 32);
		}
		else
		{
			ITEMS_WriteBitsToBitstream(pBuffer, 0, 1);
		}
	}
}

//D2Common.0x6FDA2C00
size_t __fastcall ITEMS_SerializeItem(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, BOOL bServer, BOOL bSaveItemInv, BOOL bGamble)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	uint32_t nItemFlags = 0;

	if (!pItem || pItem->dwUnitType != UNIT_ITEM)
	{
		return 0;
	}

	D2ItemDataStrc* pItemData = pItem->pItemData;
	if (pItemData)
	{
		nItemFlags = pItemData->dwItemFlags;
	}
	else
	{
		nItemFlags = 0;
	}

	nItemFlags = nItemFlags & ~IFLAG_INIT | IFLAG_JUSTSAVED;

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	if (!pItemsTxtRecord)
	{
		return 0;
	}

	if (pItemsTxtRecord->nCompactSave)
	{
		nItemFlags |= IFLAG_COMPACTSAVE;
	}

	if (bGamble)
	{
		nItemFlags = nItemFlags & ~IFLAG_ETHEREAL | IFLAG_LOWQUALITY;
	}

	if (bServer)
	{
		BITMANIP_Write(pBuffer, 'MJ', 16);
	}
	else
	{
		if (!(nItemFlags & IFLAG_IDENTIFIED))
		{
			nItemFlags &= ~IFLAG_SOCKETED;
		}
	}
	BITMANIP_Write(pBuffer, nItemFlags, 32);

	if (nItemFlags & IFLAG_COMPACTSAVE)
	{
		ITEMS_SerializeItemCompact(pItem, pBuffer, pItemsTxtRecord, bServer);
	}
	else
	{
		ITEMS_SerializeItemComplete(pItem, pBuffer, bServer, bGamble);
	}

	if (!bGamble && bSaveItemInv && pItem->pInventory)
	{
		for (D2UnitStrc* i = INVENTORY_GetFirstItem(pItem->pInventory); i; i = INVENTORY_GetNextItem(i))
		{
			BITMANIP_GoToNextByte(pBuffer);
			ITEMS_SerializeItem(INVENTORY_UnitIsItem(i), pBuffer, bServer, bSaveItemInv, 0);
		}
	}

	return BITMANIP_GetSize(pBuffer);
}

//D2Common.0x6FDA2FD0
void __fastcall ITEMS_WriteBitsToBitstream(D2BitBufferStrc* pBuffer, int nData, int nBits)
{
	if (nBits != 32)
	{
		nData &= (nData <= 0) - 1;
		if (nData >= (1 << nBits) - 1)
		{
			nData = (1 << nBits) - 1;
		}
	}

	return BITMANIP_Write(pBuffer, nData, nBits);
}

//D2Common.0x6FDA3010
void __fastcall ITEMS_SerializeItemComplete(D2UnitStrc* pItem, D2BitBufferStrc* pBuffer, BOOL bServer, BOOL bGamble)
{
	static const int gnItemSetStates_6FDD15A8[] =
	{
		STATE_ITEMSET1, STATE_ITEMSET2, STATE_ITEMSET3, STATE_ITEMSET4, STATE_ITEMSET5
	};

	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2ItemStatCostTxt* pLocalRecord = NULL;
	D2MagicAffixDataTbl* pMagicAffixInfo = NULL;
	D2ItemTypesTxt* pItemTypesTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2StatListStrc* pStatListEx = NULL;
	D2UnitStrc* pInventoryItem = NULL;
	char* szPlayerName = NULL;
	int nMagicPrefixOffset = 0;
	int nAutoMagicOffset = 0;
	int nCurrentCharacter = 0;
	int nStatListCounter = 0;
	int nSocketedItems = 0;
	int nMaxDurability = 0;
	int nItemFormat = 0;
	int nItemLevel = 0;
	int nItemType = 0;
	int nQuality = 0;
	int nInvGfx = 0;
	int nVarInvGfx = 0;
	int nAnimMode = 0;
	int nBodyLoc = 0;
	int nStatCounter = 0;
	int nSetItemMask = 0;
	int nMagicPrefix = 0;
	int nMagicSuffix = 0;
	int nRarePrefix = 0;
	int nRareSuffix = 0;
	int nRunewordId = 0;
	int nScrollType = 0;
	int nAutoAffix = 0;
	int nStatLists = 0;
	int nFileIndex = 0;
	int nEarLevel = 0;
	int nCounter = 0;
	int nStatId = 0;
	int nStats = 0;
	int nValue = 0;
	int nGold = 0;
	uint8_t nStorePage = 0;
	BOOL bIsRuneword = FALSE;
	BOOL bContinue = FALSE;
	BOOL bInvalid = FALSE;
	D2CoordStrc pCoords = {};
	int nStatValues[511] = {};
	D2StatStrc pStat[511] = {};

	D2ItemDataStrc* pItemData = ITEMS_GetItemData(pItem);
	D2_ASSERT(pItemData);

	nItemFormat = pItemData->wItemFormat;
	if (nItemFormat > 0)
	{
		if (nItemFormat >= 1023)
		{
			nItemFormat = 1023;
		}
	}
	else
	{
		nItemFormat = 0;
	}
	BITMANIP_Write(pBuffer, nItemFormat, 10);

	nAnimMode = pItem->dwAnimMode;
	if (nAnimMode > 0)
	{
		if (nAnimMode >= 7)
		{
			nAnimMode = 7;
		}
	}
	else
	{
		nAnimMode = 0;
	}
	BITMANIP_Write(pBuffer, nAnimMode, 3);

	if (nAnimMode == IMODE_ONGROUND || nAnimMode == IMODE_DROPPING)
	{
		UNITS_GetCoords(pItem, &pCoords);

		if (pCoords.nX > 0)
		{
			if (pCoords.nX >= 65535)
			{
				pCoords.nX = 65535;
			}
		}
		else
		{
			pCoords.nX = 0;
		}
		BITMANIP_Write(pBuffer, pCoords.nX, 16);

		if (pCoords.nY > 0)
		{
			if (pCoords.nY >= 65535)
			{
				pCoords.nY = 65535;
			}
		}
		else
		{
			pCoords.nY = 0;
		}
		BITMANIP_Write(pBuffer, pCoords.nY, 16);
	}
	else
	{
		nBodyLoc = pItemData->nBodyLoc;
		if (nBodyLoc > 0)
		{
			if (nBodyLoc >= 15)
			{
				nBodyLoc = 15;
			}
		}
		else
		{
			nBodyLoc = 0;
		}
		BITMANIP_Write(pBuffer, nBodyLoc, 4);

		UNITS_GetCoords(pItem, &pCoords);

		if (pCoords.nX > 0)
		{
			if (pCoords.nX >= 15)
			{
				pCoords.nX = 15;
			}
		}
		else
		{
			pCoords.nX = 0;
		}
		BITMANIP_Write(pBuffer, pCoords.nX, 4);

		if (pCoords.nY > 0)
		{
			if (pCoords.nY >= 15)
			{
				pCoords.nY = 15;
			}
		}
		else
		{
			pCoords.nY = 0;
		}
		BITMANIP_Write(pBuffer, pCoords.nY, 4);

		nStorePage = pItemData->nInvPage + 1;
		if (nStorePage > 0)
		{
			if (nStorePage >= 7)
			{
				nStorePage = 7;
			}
		}
		else
		{
			nStorePage = 0;
		}
		BITMANIP_Write(pBuffer, nStorePage, 3);
	}

	pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemsTxtRecord);

	if (bGamble)
	{
		if (pItemsTxtRecord->dwNormCode)
		{
			BITMANIP_Write(pBuffer, pItemsTxtRecord->dwNormCode, 32);
		}
		else
		{
			BITMANIP_Write(pBuffer, pItemsTxtRecord->dwCode, 32);
		}
		return;
	}

	BITMANIP_Write(pBuffer, pItemsTxtRecord->dwCode, 32);

	nSocketedItems = 0;
	if (ITEMS_CheckIfSocketable(pItem) && pItem->pInventory)
	{
		pInventoryItem = INVENTORY_GetFirstItem(pItem->pInventory);
		if (pInventoryItem)
		{
			do
			{
				++nSocketedItems;
				pInventoryItem = INVENTORY_GetNextItem(pInventoryItem);
			}
			while (pInventoryItem);

			if (nSocketedItems > 0)
			{
				if (nSocketedItems >= 7)
				{
					nSocketedItems = 7;
				}
			}
			else
			{
				nSocketedItems = 0;
			}
		}
	}
	BITMANIP_Write(pBuffer, nSocketedItems, 3);

	if (bServer)
	{
		BITMANIP_Write(pBuffer, pItem->dwInitSeed, 32);
	}

	if ((int)pItemData->dwItemLevel < 1)
	{
		pItemData->dwItemLevel = 1;
	}
		
	nItemLevel = pItemData->dwItemLevel;

	if (nItemLevel > 0)
	{
		if (nItemLevel >= 127)
		{
			nItemLevel = 127;
		}
	}
	else
	{
		nItemLevel = 0;
	}
	BITMANIP_Write(pBuffer, nItemLevel, 7);

	nQuality = pItemData->dwQualityNo;
	if (nQuality > 0)
	{
		if (nQuality >= 15)
		{
			nQuality = 15;
		}
	}
	else
	{
		nQuality = 0;
	}
	BITMANIP_Write(pBuffer, nQuality, 4);
	
	nItemType = ITEMS_GetItemType(pItem);
	pItemTypesTxtRecord = DATATBLS_GetItemTypesTxtRecord(nItemType);
	if (pItemTypesTxtRecord)
	{
		nVarInvGfx = pItemTypesTxtRecord->nVarInvGfx;
	}
	else
	{
		nVarInvGfx = 0;
	}
	BITMANIP_Write(pBuffer, nVarInvGfx ? 1 : 0, 1);

	if (pItemTypesTxtRecord && pItemTypesTxtRecord->nVarInvGfx)
	{
		nInvGfx = pItemData->nInvGfxIdx;
		if (nInvGfx > 0)
		{
			if (nInvGfx >= 7)
			{
				nInvGfx = 7;
			}
		}
		else
		{
			nInvGfx = 0;
		}
		BITMANIP_Write(pBuffer, nInvGfx, 3);
	}

	pMagicAffixInfo = DATATBLS_GetMagicAffixDataTables();
	nMagicPrefixOffset = ((int)pMagicAffixInfo->pMagicPrefix - (int)pMagicAffixInfo->pMagicAffixTxt) / sizeof(D2MagicAffixTxt);
	nAutoMagicOffset = ((int)pMagicAffixInfo->pAutoMagic - (int)pMagicAffixInfo->pMagicAffixTxt) / sizeof(D2MagicAffixTxt);

	nAutoAffix = pItemData->wAutoAffix;
	if (nAutoAffix > nAutoMagicOffset)
	{
		nAutoAffix -= nAutoMagicOffset;
	}
	BITMANIP_Write(pBuffer, nAutoAffix ? 1 : 0, 1);

	if (nAutoAffix)
	{
		if (nAutoAffix > 0)
		{
			if (nAutoAffix >= 2047)
			{
				nAutoAffix = 2047;
			}
		}
		else
		{
			nAutoAffix = 0;
		}
		BITMANIP_Write(pBuffer, nAutoAffix, 11);
	}

	switch (pItemData->dwQualityNo)
	{
	case ITEMQUAL_INFERIOR:
	case ITEMQUAL_SUPERIOR:
	{
		nFileIndex = pItemData->dwFileIndex;
		if (nFileIndex > 0)
		{
			if (nFileIndex >= 7)
			{
				nFileIndex = 7;
			}
		}
		else
		{
			nFileIndex = 0;
		}
		BITMANIP_Write(pBuffer, nFileIndex, 3);
		break;
	}
	case ITEMQUAL_RARE:
	case ITEMQUAL_CRAFT:
	{
		if (bServer || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
		{
			nRarePrefix = pItemData->wRarePrefix;
			if (nRarePrefix > 0)
			{
				if (nRarePrefix >= 255)
				{
					nRarePrefix = 255;
				}
			}
			else
			{
				nRarePrefix = 0;
			}
			BITMANIP_Write(pBuffer, nRarePrefix, 8);

			nRareSuffix = pItemData->wRareSuffix;
			if (nRareSuffix > 0)
			{
				if (nRareSuffix >= 255)
				{
					nRareSuffix = 255;
				}
			}
			else
			{
				nRareSuffix = 0;
			}
			BITMANIP_Write(pBuffer, nRareSuffix, 8);
		}

		nCounter = 0;
		do
		{
			nMagicPrefix = pItemData->wMagicPrefix[nCounter];
			if (nMagicPrefix > nMagicPrefixOffset)
			{
				nMagicPrefix -= nMagicPrefixOffset;
			}

			if (nMagicPrefix)
			{
				BITMANIP_Write(pBuffer, 1, 1);

				if (nMagicPrefix > 0)
				{
					if (nMagicPrefix >= 2047)
					{
						nMagicPrefix = 2047;
					}
				}
				else
				{
					nMagicPrefix = 0;
				}
				BITMANIP_Write(pBuffer, nMagicPrefix, 11);
			}
			else
			{
				BITMANIP_Write(pBuffer, 0, 1);
			}

			if (pItemData->wMagicSuffix[nCounter] != 0)
			{
				BITMANIP_Write(pBuffer, 1, 1);

				nMagicSuffix = pItemData->wMagicSuffix[nCounter];
				if (nMagicSuffix > 0)
				{
					if (nMagicSuffix >= 2047)
					{
						nMagicSuffix = 2047;
					}
				}
				else
				{
					nMagicSuffix = 0;
				}
				BITMANIP_Write(pBuffer, nMagicSuffix, 11);
			}
			else
			{
				BITMANIP_Write(pBuffer, 0, 1);
			}

			++nCounter;
		}
		while (nCounter < ITEMS_MAX_MODS);

		break;
	}
	case ITEMQUAL_TEMPERED:
	{
		if (bServer || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
		{
			nRarePrefix = pItemData->wRarePrefix;
			if (nRarePrefix > 0)
			{
				if (nRarePrefix >= 255)
				{
					nRarePrefix = 255;
				}
			}
			else
			{
				nRarePrefix = 0;
			}
			BITMANIP_Write(pBuffer, nRarePrefix, 8);

			nRareSuffix = pItemData->wRareSuffix;
			if (nRareSuffix > 0)
			{
				if (nRareSuffix >= 255)
				{
					nRareSuffix = 255;
				}
			}
			else
			{
				nRareSuffix = 0;
			}
			BITMANIP_Write(pBuffer, nRareSuffix, 8);
		}
		break;
	}
	case ITEMQUAL_MAGIC:
	{
		if (bServer || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
		{
			nMagicPrefix = pItemData->wMagicPrefix[0];
			if (nMagicPrefix > nMagicPrefixOffset)
			{
				nMagicPrefix -= nMagicPrefixOffset;
			}

			if (nMagicPrefix > 0)
			{
				if (nMagicPrefix >= 2047)
				{
					nMagicPrefix = 2047;
				}
			}
			else
			{
				nMagicPrefix = 0;
			}
			BITMANIP_Write(pBuffer, nMagicPrefix, 11);

			nMagicSuffix = pItemData->wMagicSuffix[0];
			if (nMagicSuffix > 0)
			{
				if (nMagicSuffix >= 2047)
				{
					nMagicSuffix = 2047;
				}

			}
			else
			{
				nMagicSuffix = 0;
			}
			BITMANIP_Write(pBuffer, nMagicSuffix, 11);
		}
		break;
	}
	case ITEMQUAL_SET:
	case ITEMQUAL_UNIQUE:
	{
		if (bServer || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
		{
			nFileIndex = pItemData->dwFileIndex;
			if (nFileIndex < 0)
			{
				nFileIndex = 4095;
			}
			if (nFileIndex >= 4095)
			{
				nFileIndex = 4095;
			}
			BITMANIP_Write(pBuffer, nFileIndex, 12);
		}
		break;
	}
	default:
	{
		if (pItemData->dwQualityNo != ITEMQUAL_NORMAL)
		{
			pItemData->dwQualityNo = ITEMQUAL_NORMAL;
			bInvalid = TRUE;
		}

		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_CHARM) && (bServer || pItemData->dwItemFlags & IFLAG_IDENTIFIED))
		{
			nMagicPrefix = pItemData->wMagicPrefix[0];
			if (nMagicPrefix > nMagicPrefixOffset)
			{
				nMagicPrefix -= nMagicPrefixOffset;
			}
			BITMANIP_Write(pBuffer, nMagicPrefix ? 1 : 0, 1);

			if (nMagicPrefix)
			{
				if (nMagicPrefix > 0)
				{
					if (nMagicPrefix >= 2047)
					{
						nMagicPrefix = 2047;
					}
					BITMANIP_Write(pBuffer, nMagicPrefix, 11);
				}
				else
				{
					BITMANIP_Write(pBuffer, 0, 11);
				}
			}
			else
			{
				nMagicSuffix = pItemData->wMagicSuffix[0];
				if (nMagicSuffix > 0)
				{
					if (nMagicSuffix >= 2047)
					{
						nMagicSuffix = 2047;
					}
					BITMANIP_Write(pBuffer, nMagicSuffix, 11);
				}
				else
				{
					BITMANIP_Write(pBuffer, 0, 11);
				}
			}
		}

		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BODY_PART) && !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_PLAYER_BODY_PART))
		{
			nFileIndex = pItemData->dwFileIndex;
			if (nFileIndex > 0)
			{
				if (nFileIndex >= 1023)
				{
					nFileIndex = 1023;
				}
			}
			else
			{
				nFileIndex = 0;
			}
			BITMANIP_Write(pBuffer, nFileIndex, 10);
		}

		if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
		{
			nScrollType = pItemData->wMagicSuffix[0];
			if (nScrollType > 0)
			{
				if (nScrollType >= 31)
				{
					nScrollType = 31;
				}
			}
			else
			{
				nScrollType = 0;
			}
			BITMANIP_Write(pBuffer, nScrollType, 5);
		}
		break;
	}
	}

	if (pItemData->dwItemFlags & IFLAG_RUNEWORD)
	{
		if (const D2RunesTxt* pRunesTxtRecord = ITEMS_GetRunesTxtRecordFromItem(pItem))
		{
			nRunewordId = pRunesTxtRecord->wStringId;
		}
		else
		{
			nRunewordId = -1;
		}

		if (nRunewordId > 0)
		{
			if (nRunewordId >= 65535)
			{
				nRunewordId = 65535;
			}
		}
		else
		{
			nRunewordId = 0;
		}
		BITMANIP_Write(pBuffer, nRunewordId, 16);
	}

	if (pItemData->dwItemFlags & IFLAG_ISEAR)
	{
		nFileIndex = pItemData->dwFileIndex;
		if (nFileIndex > 0)
		{
			if (nFileIndex >= 7)
			{
				nFileIndex = 7;
			}
		}
		else
		{
			nFileIndex = 0;
		}
		BITMANIP_Write(pBuffer, nFileIndex, 3);

		nEarLevel = pItemData->nEarLvl;
		if (nEarLevel > 0)
		{
			if (nEarLevel >= 127)
			{
				nEarLevel = 127;
			}
		}
		else
		{
			nEarLevel = 0;
		}
		BITMANIP_Write(pBuffer, nEarLevel, 7);

		szPlayerName = pItemData->szPlayerName;
		do
		{
			nCurrentCharacter = *szPlayerName;
			if (nCurrentCharacter > 0)
			{
				if (nCurrentCharacter >= 127)
				{
					nCurrentCharacter = 127;
				}
			}
			else
			{
				nCurrentCharacter = 0;
			}
			BITMANIP_Write(pBuffer, nCurrentCharacter, 7);
			bContinue = *szPlayerName++;
		}
		while (bContinue);
	}
	else if (pItemData->dwItemFlags & IFLAG_PERSONALIZED)
	{
		szPlayerName = pItemData->szPlayerName;
		do
		{
			nCurrentCharacter = *szPlayerName;
			if (nCurrentCharacter > 0)
			{
				if (nCurrentCharacter >= 127)
				{
					nCurrentCharacter = 127;
				}
			}
			else
			{
				nCurrentCharacter = 0;
			}
			BITMANIP_Write(pBuffer, nCurrentCharacter, 7);
			bContinue = *szPlayerName++;
		}
		while (bContinue);
	}

	if (bServer)
	{
		if (pItemData->dwRealmData[1])
		{
			BITMANIP_Write(pBuffer, 1, 1);
			BITMANIP_Write(pBuffer, pItemData->dwRealmData[0], 32);
			ITEMS_WriteBitsToBitstream(pBuffer, pItemData->dwRealmData[1], 32);
			ITEMS_WriteBitsToBitstream(pBuffer, 0, 32);
		}
		else
		{
			ITEMS_WriteBitsToBitstream(pBuffer, 0, 1);
		}
	}

	if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_ARMOR))
	{
		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_ARMORCLASS);
		ITEMS_WriteBitsToBitstream(pBuffer, STATLIST_GetUnitBaseStat(pItem, STAT_ARMORCLASS, 0) + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);

		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAXDURABILITY);
		nMaxDurability = STATLIST_GetUnitBaseStat(pItem, STAT_MAXDURABILITY, 0);
		ITEMS_WriteBitsToBitstream(pBuffer, nMaxDurability + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);
		if (nMaxDurability)
		{
			pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_DURABILITY);
			ITEMS_WriteBitsToBitstream(pBuffer, STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0) + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);
		}
	}
	else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
	{
		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAXDURABILITY);
		nMaxDurability = STATLIST_GetUnitBaseStat(pItem, STAT_MAXDURABILITY, 0);
		ITEMS_WriteBitsToBitstream(pBuffer, nMaxDurability + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);

		if (nMaxDurability)
		{
			pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_DURABILITY);
			ITEMS_WriteBitsToBitstream(pBuffer, STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0) + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);
		}
	}
	else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_GOLD))
	{
		nGold = STATLIST_UnitGetStatValue(pItem, STAT_GOLD, 0);
		ITEMS_WriteBitsToBitstream(pBuffer, nGold >= 4096, 1);
		ITEMS_WriteBitsToBitstream(pBuffer, nGold, nGold >= 4096 ? 32 : 12);
	}

	if (ITEMS_CheckIfStackable(pItem))
	{
		ITEMS_WriteBitsToBitstream(pBuffer, STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0), 9);
	}

	if (pItemData->dwItemFlags & IFLAG_SOCKETED)
	{
		ITEMS_WriteBitsToBitstream(pBuffer, STATLIST_GetUnitBaseStat(pItem, STAT_ITEM_NUMSOCKETS, 0), ITEMS_GetItemStatCostTxtRecord(STAT_ITEM_NUMSOCKETS)->nSaveBits);
	}

	if (bServer || pItemData->dwItemFlags & IFLAG_IDENTIFIED)
	{
		nStatLists = 0;
		if (ITEMS_GetItemQuality(pItem) == ITEMQUAL_SET)
		{
			nSetItemMask = 0;
			nCounter = 0;
			do
			{
				if (STATLIST_GetStatListFromUnitStateAndFlag(pItem, gnItemSetStates_6FDD15A8[nCounter], 0x2040) || STATLIST_GetStatListFromUnitStateAndFlag(pItem, gnItemSetStates_6FDD15A8[nCounter], 0x40))
				{
					nStatLists = nCounter + 1;
					nSetItemMask += 1 << nCounter;
				}

				++nCounter;
			}
			while (nCounter < 5);

			ITEMS_WriteBitsToBitstream(pBuffer, nSetItemMask, 5);
		}

		bIsRuneword = 0;
		if (ITEMS_CheckItemFlag(pItem, IFLAG_RUNEWORD, __LINE__, __FILE__))
		{
			++nStatLists;
			bIsRuneword = TRUE;
		}

		nStatListCounter = -1;

		pStatListEx = STATLIST_GetStatListFromUnitStateAndFlag(pItem, 0, 0x40);

		while (1)
		{
			if (pStatListEx && !bInvalid)
			{
				memset(nStatValues, 0, sizeof(nStatValues));
				nStats = STATLIST_GetBaseStatsData((D2StatListExStrc*)pStatListEx, pStat, ARRAY_SIZE(pStat));

				nStatCounter = 0;
				while (nStatCounter < nStats)
				{
					nStatId = pStat[nStatCounter].nStat;

					pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
					nValue = pStat[nStatCounter].nValue >> pItemStatCostTxtRecord->nValShift;

					if (pItemStatCostTxtRecord->nSaveBits && nValue && nValue != nStatValues[nStatId])
					{
						ITEMS_WriteBitsToBitstream(pBuffer, nStatId, 9);

						if (nStatId <= STAT_MAGICMINDAM)
						{
							if (nStatId == STAT_MAGICMINDAM)
							{
								ITEMS_WriteBitsToBitstream(pBuffer, nValue + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);

								pLocalRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAGICMAXDAM);
								nStatValues[STAT_MAGICMAXDAM] = STATLIST_GetStatValue(pStatListEx, STAT_MAGICMAXDAM, 0);
								ITEMS_WriteBitsToBitstream(pBuffer, nStatValues[STAT_MAGICMAXDAM] + pLocalRecord->dwSaveAdd, pLocalRecord->nSaveBits);
							}
							else if (nStatId == STAT_ITEM_MAXDAMAGE_PERCENT)
							{
								ITEMS_WriteBitsToBitstream(pBuffer, nValue + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);

								pLocalRecord = ITEMS_GetItemStatCostTxtRecord(STAT_ITEM_MINDAMAGE_PERCENT);
								nStatValues[STAT_ITEM_MINDAMAGE_PERCENT] = STATLIST_GetStatValue(pStatListEx, STAT_ITEM_MINDAMAGE_PERCENT, 0);
								ITEMS_WriteBitsToBitstream(pBuffer, nStatValues[STAT_ITEM_MINDAMAGE_PERCENT] + pLocalRecord->dwSaveAdd, pLocalRecord->nSaveBits);
							}
							else if (nStatId == STAT_FIREMINDAM)
							{
								ITEMS_WriteBitsToBitstream(pBuffer, nValue + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);

								pLocalRecord = ITEMS_GetItemStatCostTxtRecord(STAT_FIREMAXDAM);
								nStatValues[STAT_FIREMAXDAM] = STATLIST_GetStatValue(pStatListEx, STAT_FIREMAXDAM, 0);
								ITEMS_WriteBitsToBitstream(pBuffer, nStatValues[STAT_FIREMAXDAM] + pLocalRecord->dwSaveAdd, pLocalRecord->nSaveBits);
							}
							else if (nStatId == STAT_LIGHTMINDAM)
							{
								ITEMS_WriteBitsToBitstream(pBuffer, nValue + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);

								pLocalRecord = ITEMS_GetItemStatCostTxtRecord(STAT_LIGHTMAXDAM);
								nStatValues[STAT_LIGHTMAXDAM] = STATLIST_GetStatValue(pStatListEx, STAT_LIGHTMAXDAM, 0);
								ITEMS_WriteBitsToBitstream(pBuffer, nStatValues[STAT_LIGHTMAXDAM] + pLocalRecord->dwSaveAdd, pLocalRecord->nSaveBits);
							}
							else
							{
								if ((int)pItemStatCostTxtRecord->dwSaveParamBits > 0)
								{
									ITEMS_WriteBitsToBitstream(pBuffer, pStat[nStatCounter].nLayer, pItemStatCostTxtRecord->dwSaveParamBits);
								}

								if (pItemStatCostTxtRecord->nSaveBits)
								{
									ITEMS_WriteBitsToBitstream(pBuffer, nValue + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);
								}
							}
						}
						else if (nStatId == STAT_COLDMINDAM)
						{
							ITEMS_WriteBitsToBitstream(pBuffer, nValue + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);

							pLocalRecord = ITEMS_GetItemStatCostTxtRecord(STAT_COLDMAXDAM);
							nStatValues[STAT_COLDMAXDAM] = STATLIST_GetStatValue(pStatListEx, STAT_COLDMAXDAM, 0);
							ITEMS_WriteBitsToBitstream(pBuffer, nStatValues[STAT_COLDMAXDAM] + pLocalRecord->dwSaveAdd, pLocalRecord->nSaveBits);

							pLocalRecord = ITEMS_GetItemStatCostTxtRecord(STAT_COLDLENGTH);
							nStatValues[STAT_COLDLENGTH] = STATLIST_GetStatValue(pStatListEx, STAT_COLDLENGTH, 0);
							ITEMS_WriteBitsToBitstream(pBuffer, nStatValues[STAT_COLDLENGTH] + pLocalRecord->dwSaveAdd, pLocalRecord->nSaveBits);
						}
						else if (nStatId == STAT_POISONMINDAM)
						{
							ITEMS_WriteBitsToBitstream(pBuffer, nValue + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);

							pLocalRecord = ITEMS_GetItemStatCostTxtRecord(STAT_POISONMAXDAM);
							nStatValues[STAT_POISONMAXDAM] = STATLIST_GetStatValue(pStatListEx, STAT_POISONMAXDAM, 0);
							ITEMS_WriteBitsToBitstream(pBuffer, nStatValues[STAT_POISONMAXDAM] + pLocalRecord->dwSaveAdd, pLocalRecord->nSaveBits);

							pLocalRecord = ITEMS_GetItemStatCostTxtRecord(STAT_POISONLENGTH);
							nStatValues[STAT_POISONLENGTH] = STATLIST_GetStatValue(pStatListEx, STAT_POISONLENGTH, 0);
							ITEMS_WriteBitsToBitstream(pBuffer, nStatValues[STAT_POISONLENGTH] + pLocalRecord->dwSaveAdd, pLocalRecord->nSaveBits);
						}
						else if (nStatId != STAT_POISON_COUNT)
						{
							if ((int)pItemStatCostTxtRecord->dwSaveParamBits > 0)
							{
								ITEMS_WriteBitsToBitstream(pBuffer, pStat[nStatCounter].nLayer, pItemStatCostTxtRecord->dwSaveParamBits);
							}

							if (pItemStatCostTxtRecord->nSaveBits)
							{
								ITEMS_WriteBitsToBitstream(pBuffer, nValue + pItemStatCostTxtRecord->dwSaveAdd, pItemStatCostTxtRecord->nSaveBits);
							}
						}
					}

					++nStatCounter;
				}
			}

			if (nStatListCounter < 0 || pStatListEx || bIsRuneword)
			{
				ITEMS_WriteBitsToBitstream(pBuffer, 511, 9);
			}

			++nStatListCounter;

			if (nStatListCounter >= nStatLists)
			{
				return;
			}

			if (bIsRuneword && nStatListCounter == nStatLists - 1)
			{
				pStatListEx = STATLIST_GetStatListFromUnitStateOrFlag(pItem, STATE_RUNEWORD, 0x40);
			}
			else
			{
				pStatListEx = STATLIST_GetStatListFromUnitStateAndFlag(pItem, gnItemSetStates_6FDD15A8[nStatListCounter], 0x2040);
				if (!pStatListEx)
				{
					pStatListEx = STATLIST_GetStatListFromUnitStateAndFlag(pItem, gnItemSetStates_6FDD15A8[nStatListCounter], 0x40);
				}
			}
		}
	}
}

//D2Common.0x6FDA42B0
D2ItemStatCostTxt* __fastcall ITEMS_GetItemStatCostTxtRecord(int nStatId)
{
	if (nStatId >= 0 && nStatId < sgptDataTables->nItemStatCostTxtRecordCount)
	{
		return &sgptDataTables->pItemStatCostTxt[nStatId];
	}

	return NULL;
}

//D2Common.0x6FDA42E0 (#10837)
int __stdcall ITEMS_GetNoOfSetItemsFromItem(D2UnitStrc* pItem)
{
	if (D2SetItemsTxt* pSetItemsTxtRecord = ITEMS_GetSetItemsTxtRecordFromItem(pItem))
	{
		return pSetItemsTxtRecord->nSetItems;
	}
	return -1;
}


//D2Common.0x6FDD15C8
int gnItemSetStates[] =
{
	STATE_ITEMSET1, STATE_ITEMSET2, STATE_ITEMSET3, STATE_ITEMSET4, STATE_ITEMSET5, STATE_ITEMSET6
};

//D2Common.0x6FDD15E0
int gnSetMaskToBonusMappingTable[] =
{
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6
};

//D2Common.0x6FDA4380
//TODO: Find a name
BOOL __fastcall sub_6FDA4380(D2UnitStrc* pItem, unsigned int nSetItemMask)
{
	D2SetItemsTxt* pSetItemsTxt = NULL;
	int nSetItems = 0;
	int nBonuses = 0;
	int nIndex = 0;
	BOOL bSet = FALSE;

	if (!pItem || pItem->dwUnitType != UNIT_ITEM || ITEMS_GetItemQuality(pItem) != ITEMQUAL_SET)
	{
		return FALSE;
	}
	
	pSetItemsTxt = ITEMS_GetSetItemsTxtRecordFromItem(pItem);
	if (!pSetItemsTxt || !pSetItemsTxt->nAddFunc)
	{
		return FALSE;
	}

	if (pSetItemsTxt->nAddFunc == 1)
	{
		nSetItems = ITEMS_GetNoOfSetItemsFromItem(pItem);
		for (int i = 0; i < 6; ++i)
		{
			nIndex = i;
			if (i != nSetItems)
			{
				if (i > nSetItems)
				{
					--nIndex;
				}

				if ((1 << i) & nSetItemMask)
				{
					bSet = FALSE;
				}
				else
				{
					bSet = TRUE;
				}

				D2Common_10574(pItem, gnItemSetStates[nIndex], bSet);
			}
		}
	}
	else if (pSetItemsTxt->nAddFunc == 2)
	{
		nBonuses = nSetItemMask < ARRAY_SIZE(gnSetMaskToBonusMappingTable) ? gnSetMaskToBonusMappingTable[nSetItemMask] : 0;

		for (int i = 0; i < nBonuses - 1; ++i)
		{
			D2Common_10574(pItem, gnItemSetStates[i], FALSE);
		}

		if (nBonuses - 1 >= 5)
		{
			return TRUE;
		}

		for(int i = nBonuses - 1; i < 6; ++i)
		{
			D2Common_10574(pItem, gnItemSetStates[i], TRUE);
		}
	}

	return TRUE;
}

//D2Common.0x6FDA4490
//TODO: Find a name
BOOL __fastcall sub_6FDA4490(D2UnitStrc* pUnit, D2UnitStrc* pItem, int a3)
{
	D2SetItemsTxt* pSetItemsTxtRecord = NULL;
	D2StatListStrc* pStatList = NULL;
	int nFileIndex = 0;
	int nIndex = 0;
	int nValue = 0;

	if (a3 <= 1 && pUnit && pItem && pItem->dwUnitType == UNIT_ITEM && ITEMS_GetItemQuality(pItem) == ITEMQUAL_SET && (pUnit->dwUnitType != UNIT_ITEM || !ITEMS_IsMagSetRarUniCrfOrTmp(pUnit)))
	{
		nFileIndex = ITEMS_GetFileIndex(pItem);
		if (nFileIndex >= 0 && nFileIndex < sgptDataTables->nSetItemsTxtRecordCount)
		{
			pSetItemsTxtRecord = &sgptDataTables->pSetItemsTxt[nFileIndex];
			if (pSetItemsTxtRecord && pSetItemsTxtRecord->nSetId >= 0 && pSetItemsTxtRecord->nSetId < sgptDataTables->nSetsTxtRecordCount)
			{
				nValue = pSetItemsTxtRecord->nSetId;
				nIndex = -1;

				for (int i = 0; i < 6; ++i)
				{
					pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, gnItemSetStates[i]);
					if (pStatList)
					{
						if (nValue == STATLIST_GetStatValue(pStatList, STAT_VALUE, 0))
						{
							if (a3 == 0)
							{
								STATLIST_RemoveAllStats(pStatList);
								STATLIST_AddStat(pStatList, STAT_VALUE, nValue, 0);
								ITEMMODS_UpdateFullSetBoni(pUnit, pItem, gnItemSetStates[i]);
							}
							else if (a3 == 1)
							{
								D2Common_10474(pUnit, pStatList);
								STATLIST_FreeStatList(pStatList);
							}

							return TRUE;
						}
					}
					else if (nIndex < 0)
					{
						nIndex = i;
					}
				}

				if (a3 == 1 || nIndex < 0)
				{
					return FALSE;
				}

				pStatList = STATLIST_AllocStatList(pUnit->pMemoryPool, 0, 0, pItem->dwUnitType, pItem->dwUnitId);
				D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
				STATLIST_SetState(pStatList, gnItemSetStates[nIndex]);

				if (a3 == 0)
				{
					STATLIST_AddStat(pStatList, STAT_VALUE, nValue, 0);
					ITEMMODS_UpdateFullSetBoni(pUnit, pItem, gnItemSetStates[nIndex]);
				}
				//else if (a3 == 1)
				//{
				//	D2Common_10474(pUnit, pStatList);
				//	STATLIST_FreeStatList(pStatList);
				//}

				return TRUE;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FDA4640 (#10866)
BOOL __stdcall ITEMS_UpdateSets(D2UnitStrc* pUnit, D2UnitStrc* pItem, int a3, int a4)
{
	unsigned int nSetItemMask = 0;

	if (pUnit && pItem && pItem->dwUnitType == UNIT_ITEM && pUnit->pInventory)
	{
		if (INVENTORY_IsItemInInventory(pUnit->pInventory, pItem) && INVENTORY_GetItemNodePage(pItem) == 3)
		{
			nSetItemMask = ITEMS_GetSetItemsMask(pUnit, pItem, TRUE);
		}

		if (a4 || !a3)
		{
			sub_6FDA4380(pItem, nSetItemMask);
		}

		sub_6FDA4490(pUnit, pItem, a3);
		
		return TRUE;
	}

	return FALSE;
}
