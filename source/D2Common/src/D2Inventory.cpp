#include "D2Inventory.h"

#include "D2Composit.h"
#include "D2DataTbls.h"
#include <DataTbls/MonsterIds.h>
#include "D2Dungeon.h"
#include "D2Items.h"
#include "D2StatList.h"
#include "Units/UnitRoom.h"
#include "Units/Units.h"
#include <Path/Path.h>

struct D2UnkInventoryComponentStrc
{
	int dwCode;
	int nItemType;
};


//D2Common.0x6FDE2310
D2InventoryGridInfoStrc gBodyLocInventoryGridInfo = { 13, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
//D2Common.0x6FDE2328
D2InventoryGridInfoStrc gBeltInventoryGridInfo = { 16, 1, 0, 0, 0, 0, 0, 0, 0, 0 };

//D2Common.0x6FDE2820
D2UnkInventoryComponentStrc stru_6FDE2820[]
{
	{ 0, ITEMTYPE_NONE_2 },
	{ ' til', ITEMTYPE_NONE_2 },
	{ ' dem', ITEMTYPE_NONE_2 },
	{ ' yvh', ITEMTYPE_NONE_2 },
	{ ' pac', ITEMTYPE_HELM },
	{ ' pks', ITEMTYPE_HELM },
	{ ' mlh', ITEMTYPE_HELM },
	{ ' lhf', ITEMTYPE_HELM },
	{ ' mhg', ITEMTYPE_HELM },
	{ ' nrc', ITEMTYPE_HELM },
	{ ' ksm', ITEMTYPE_HELM },
	{ ' epc', ITEMTYPE_ARMOR },
	{ ' klc', ITEMTYPE_ARMOR },
	{ ' bor', ITEMTYPE_ARMOR },
	{ ' iuq', ITEMTYPE_ARMOR },
	{ ' ael', ITEMTYPE_ARMOR },
	{ ' alh', ITEMTYPE_ARMOR },
	{ ' uts', ITEMTYPE_ARMOR },
	{ ' gnr', ITEMTYPE_ARMOR },
	{ ' lcs', ITEMTYPE_ARMOR },
	{ ' nhc', ITEMTYPE_ARMOR },
	{ ' srb', ITEMTYPE_ARMOR },
	{ ' lps', ITEMTYPE_ARMOR },
	{ ' tlp', ITEMTYPE_ARMOR },
	{ ' dlf', ITEMTYPE_ARMOR },
	{ ' htg', ITEMTYPE_ARMOR },
	{ ' luf', ITEMTYPE_ARMOR },
	{ ' cub', ITEMTYPE_SHIELD },
	{ ' grl', ITEMTYPE_SHIELD },
	{ ' tik', ITEMTYPE_SHIELD },
	{ ' wot', ITEMTYPE_SHIELD },
	{ ' lgl', ITEMTYPE_GLOVES },
	{ ' lgm', ITEMTYPE_GLOVES },
	{ ' lgh', ITEMTYPE_GLOVES },
	{ ' tbl', ITEMTYPE_BOOTS },
	{ ' tbm', ITEMTYPE_BOOTS },
	{ ' tbh', ITEMTYPE_BOOTS },
	{ ' lbl', ITEMTYPE_BELT },
	{ ' lbm', ITEMTYPE_BELT },
	{ ' lbh', ITEMTYPE_BELT },
	{ ' mhb', ITEMTYPE_HELM },
	{ ' hsb', ITEMTYPE_SHIELD },
	{ ' kps', ITEMTYPE_SHIELD },
	{ ' dnw', ITEMTYPE_WAND },
	{ ' nwy', ITEMTYPE_WAND },
	{ ' nwb', ITEMTYPE_WAND },
	{ ' blc', ITEMTYPE_CLUB },
	{ ' xah', ITEMTYPE_THROWING_AXE },
	{ ' dss', ITEMTYPE_SWORD },
	{ ' mcs', ITEMTYPE_SWORD },
	{ ' cam', ITEMTYPE_MACE },
	{ ' exa', ITEMTYPE_AXE },
	{ ' clf', ITEMTYPE_SWORD },
	{ ' dsl', ITEMTYPE_SWORD },
	{ ' alf', ITEMTYPE_MACE },
	{ ' mdh', ITEMTYPE_HAMMER },
	{ ' mhw', ITEMTYPE_HAMMER },
	{ ' src', ITEMTYPE_SWORD },
	{ ' slf', ITEMTYPE_SWORD },
	{ ' lpg', ITEMTYPE_MISSILE_POTION },
	{ ' spg', ITEMTYPE_MISSILE_POTION },
	{ ' lpo', ITEMTYPE_MISSILE_POTION },
	{ ' spo', ITEMTYPE_MISSILE_POTION },
	{ ' rgd', ITEMTYPE_KNIFE },
	{ ' 33d', ITEMTYPE_KNIFE },
	{ ' 33g', ITEMTYPE_KNIFE },
	{ ' rid', ITEMTYPE_KNIFE },
	{ ' vaj', ITEMTYPE_SPEAR },
	{ ' vlg', ITEMTYPE_SPEAR },
	{ ' lip', ITEMTYPE_SPEAR },
	{ ' mlc', ITEMTYPE_SWORD },
	{ ' dsg', ITEMTYPE_SWORD },
	{ ' rps', ITEMTYPE_SPEAR },
	{ ' irt', ITEMTYPE_SPEAR },
	{ ' kip', ITEMTYPE_SPEAR },
	{ ' rps', ITEMTYPE_SPEAR },
	{ ' wbs', ITEMTYPE_BOW },
	{ ' wbl', ITEMTYPE_BOW },
	{ ' bbs', ITEMTYPE_BOW },
	{ ' bbl', ITEMTYPE_BOW },
	{ ' tss', ITEMTYPE_STAFF },
	{ ' tsl', ITEMTYPE_STAFF },
	{ ' tsc', ITEMTYPE_STAFF },
	{ ' tsb', ITEMTYPE_STAFF },
	{ ' xal', ITEMTYPE_AXE },
	{ ' ycs', ITEMTYPE_POLEARM },
	{ ' xig', ITEMTYPE_AXE },
	{ ' xtb', ITEMTYPE_AXE },
	{ ' lah', ITEMTYPE_POLEARM },
	{ ' uam', ITEMTYPE_HAMMER },
	{ ' xap', ITEMTYPE_POLEARM },
	{ ' lah', ITEMTYPE_POLEARM },
	{ ' bxh', ITEMTYPE_CROSSBOW },
	{ ' bxl', ITEMTYPE_CROSSBOW },
	{ ' xat', ITEMTYPE_THROWING_AXE },
	{ ' cps', ITEMTYPE_CLUB },
	{ ' rbs', ITEMTYPE_SWORD },
	{ ' tsm', ITEMTYPE_MACE },
	{ ' pcs', ITEMTYPE_SCEPTER },
	{ ' mpg', ITEMTYPE_MISSILE_POTION },
	{ ' mpo', ITEMTYPE_MISSILE_POTION },
	{ ' fkt', ITEMTYPE_THROWING_KNIFE },
	{ ' dlb', ITEMTYPE_KNIFE },
	{ ' dsb', ITEMTYPE_SWORD },
	{ ' wsb', ITEMTYPE_SWORD },
	{ ' sh2', ITEMTYPE_SWORD },
	{ ' nrb', ITEMTYPE_SPEAR },
	{ ' wbh', ITEMTYPE_BOW },
	{ ' wbc', ITEMTYPE_BOW },
	{ ' bws', ITEMTYPE_BOW },
	{ ' bwl', ITEMTYPE_BOW },
	{ ' tsw', ITEMTYPE_STAFF },
	{ ' xab', ITEMTYPE_AXE },
	{ ' rab', ITEMTYPE_POLEARM },
	{ ' xag', ITEMTYPE_AXE },
	{ ' bxr', ITEMTYPE_CROSSBOW },
	{ ' bxm', ITEMTYPE_CROSSBOW },
	{ ' tbz', ITEMTYPE_NONE_2 },
	{ ' raa', ITEMTYPE_ARMOR },
	{ ' ptl', ITEMTYPE_ARMOR },
	{ ' lms', ITEMTYPE_SHIELD },
	{ ' stg', ITEMTYPE_ARMOR },
	{ ' vqa', ITEMTYPE_BODY_PART },
	{ ' vqc', ITEMTYPE_BODY_PART },
	{ ' lbv', ITEMTYPE_BELT },
	{ ' lbt', ITEMTYPE_BELT },
	{ ' lgv', ITEMTYPE_GLOVES },
	{ ' lgt', ITEMTYPE_GLOVES },
	{ ' tbv', ITEMTYPE_BOOTS },
	{ ' tbt', ITEMTYPE_BOOTS },
	{ ' blf', ITEMTYPE_SWORD },
	{ ' ipm', ITEMTYPE_AXE },
	{ ' lab', ITEMTYPE_THROWING_AXE },
	{ ' gel', ITEMTYPE_CLUB },
	{ ' tlq', ITEMTYPE_ARMOR },
	{ ' xa2', ITEMTYPE_AXE },
	{ ' xaw', ITEMTYPE_AXE },
	{ ' nwg', ITEMTYPE_MACE },
	{ ' amg', ITEMTYPE_MACE },
	{ ' dsw', ITEMTYPE_SWORD },
	{ ' sig', ITEMTYPE_SWORD },
	{ ' dlb', ITEMTYPE_KNIFE },
	{ ' fkb', ITEMTYPE_KNIFE },
	{ ' pss', ITEMTYPE_SPEAR },
	{ ' pst', ITEMTYPE_SPEAR },
	{ ' eps', ITEMTYPE_SPEAR },
	{ ' uov', ITEMTYPE_POLEARM },
	{ ' csw', ITEMTYPE_POLEARM },
	{ ' irk', ITEMTYPE_KNIFE },
	{ ' tps', ITEMTYPE_SPEAR },
	{ ' csg', ITEMTYPE_SCEPTER },
	{ ' psw', ITEMTYPE_SCEPTER },
	{ ' tsh', ITEMTYPE_STAFF },
	{ ' fsm', ITEMTYPE_STAFF },
	{ ' hfh', ITEMTYPE_MACE },
	{ ' ah9', ITEMTYPE_AXE },
	{ ' xa9', ITEMTYPE_AXE },
	{ ' a29', ITEMTYPE_AXE },
	{ ' pm9', ITEMTYPE_AXE },
	{ ' aw9', ITEMTYPE_AXE },
	{ ' al9', ITEMTYPE_AXE },
	{ ' ab9', ITEMTYPE_AXE },
	{ ' tb9', ITEMTYPE_AXE },
	{ ' ag9', ITEMTYPE_AXE },
	{ ' ig9', ITEMTYPE_AXE },
	{ ' nw9', ITEMTYPE_WAND },
	{ ' wy9', ITEMTYPE_WAND },
	{ ' wb9', ITEMTYPE_WAND },
	{ ' wg9', ITEMTYPE_WAND },
	{ ' lc9', ITEMTYPE_MACE },
	{ ' cs9', ITEMTYPE_SCEPTER },
	{ ' sq9', ITEMTYPE_SCEPTER },
	{ ' sw9', ITEMTYPE_SCEPTER },
	{ ' ps9', ITEMTYPE_MACE },
	{ ' am9', ITEMTYPE_MACE },
	{ ' tm9', ITEMTYPE_MACE },
	{ ' lf9', ITEMTYPE_MACE },
	{ ' hw9', ITEMTYPE_MACE },
	{ ' 9m9', ITEMTYPE_MACE },
	{ ' mg9', ITEMTYPE_MACE },
	{ ' ss9', ITEMTYPE_SWORD },
	{ ' ms9', ITEMTYPE_SWORD },
	{ ' bs9', ITEMTYPE_SWORD },
	{ ' cf9', ITEMTYPE_SWORD },
	{ ' rc9', ITEMTYPE_SWORD },
	{ ' sb9', ITEMTYPE_SWORD },
	{ ' sl9', ITEMTYPE_SWORD },
	{ ' dw9', ITEMTYPE_SWORD },
	{ ' h29', ITEMTYPE_SWORD },
	{ ' mc9', ITEMTYPE_SWORD },
	{ ' sg9', ITEMTYPE_SWORD },
	{ ' 9b9', ITEMTYPE_SWORD },
	{ ' bf9', ITEMTYPE_SWORD },
	{ ' dg9', ITEMTYPE_SWORD },
	{ ' gd9', ITEMTYPE_KNIFE },
	{ ' id9', ITEMTYPE_KNIFE },
	{ ' rk9', ITEMTYPE_KNIFE },
	{ ' lb9', ITEMTYPE_KNIFE },
	{ ' kt9', ITEMTYPE_THROWING_KNIFE },
	{ ' at9', ITEMTYPE_THROWING_AXE },
	{ ' kb9', ITEMTYPE_THROWING_KNIFE },
	{ ' 8b9', ITEMTYPE_THROWING_AXE },
	{ ' aj9', ITEMTYPE_SPEAR },
	{ ' ip9', ITEMTYPE_SPEAR },
	{ ' 9s9', ITEMTYPE_SPEAR },
	{ ' lg9', ITEMTYPE_SPEAR },
	{ ' st9', ITEMTYPE_SPEAR },
	{ ' rs9', ITEMTYPE_SPEAR },
	{ ' rt9', ITEMTYPE_SPEAR },
	{ ' rb9', ITEMTYPE_SPEAR },
	{ ' ts9', ITEMTYPE_SPEAR },
	{ ' 9p9', ITEMTYPE_SPEAR },
	{ ' 7b9', ITEMTYPE_POLEARM },
	{ ' ov9', ITEMTYPE_POLEARM },
	{ ' 8s9', ITEMTYPE_POLEARM },
	{ ' ap9', ITEMTYPE_POLEARM },
	{ ' 9h9', ITEMTYPE_POLEARM },
	{ ' cw9', ITEMTYPE_POLEARM },
	{ ' ss8', ITEMTYPE_STAFF },
	{ ' sl8', ITEMTYPE_STAFF },
	{ ' sc8', ITEMTYPE_STAFF },
	{ ' sb8', ITEMTYPE_STAFF },
	{ ' sw8', ITEMTYPE_STAFF },
	{ ' bs8', ITEMTYPE_BOW },
	{ ' bh8', ITEMTYPE_BOW },
	{ ' bl8', ITEMTYPE_BOW },
	{ ' bc8', ITEMTYPE_BOW },
	{ ' 8s8', ITEMTYPE_BOW },
	{ ' 8l8', ITEMTYPE_BOW },
	{ ' ws8', ITEMTYPE_BOW },
	{ ' wl8', ITEMTYPE_BOW },
	{ ' xl8', ITEMTYPE_CROSSBOW },
	{ ' xm8', ITEMTYPE_CROSSBOW },
	{ ' xh8', ITEMTYPE_CROSSBOW },
	{ ' xr8', ITEMTYPE_CROSSBOW },
	{ ' pax', ITEMTYPE_HELM },
	{ ' pkx', ITEMTYPE_HELM },
	{ ' mlx', ITEMTYPE_HELM },
	{ ' lhx', ITEMTYPE_HELM },
	{ ' mhx', ITEMTYPE_HELM },
	{ ' nrx', ITEMTYPE_HELM },
	{ ' ksx', ITEMTYPE_HELM },
	{ ' iux', ITEMTYPE_ARMOR },
	{ ' aex', ITEMTYPE_ARMOR },
	{ ' alx', ITEMTYPE_ARMOR },
	{ ' utx', ITEMTYPE_ARMOR },
	{ ' gnx', ITEMTYPE_ARMOR },
	{ ' lcx', ITEMTYPE_ARMOR },
	{ ' nhx', ITEMTYPE_ARMOR },
	{ ' srx', ITEMTYPE_ARMOR },
	{ ' lpx', ITEMTYPE_ARMOR },
	{ ' tlx', ITEMTYPE_ARMOR },
	{ ' dlx', ITEMTYPE_ARMOR },
	{ ' htx', ITEMTYPE_ARMOR },
	{ ' lux', ITEMTYPE_ARMOR },
	{ ' rax', ITEMTYPE_ARMOR },
	{ ' ptx', ITEMTYPE_ARMOR },
	{ ' cux', ITEMTYPE_SHIELD },
	{ ' lmx', ITEMTYPE_SHIELD },
	{ ' grx', ITEMTYPE_SHIELD },
	{ ' tix', ITEMTYPE_SHIELD },
	{ ' wox', ITEMTYPE_SHIELD },
	{ ' stx', ITEMTYPE_SHIELD },
	{ ' glx', ITEMTYPE_GLOVES },
	{ ' gvx', ITEMTYPE_GLOVES },
	{ ' gmx', ITEMTYPE_GLOVES },
	{ ' gtx', ITEMTYPE_GLOVES },
	{ ' ghx', ITEMTYPE_GLOVES },
	{ ' blx', ITEMTYPE_BOOTS },
	{ ' bvx', ITEMTYPE_BOOTS },
	{ ' bmx', ITEMTYPE_BOOTS },
	{ ' btx', ITEMTYPE_BOOTS },
	{ ' bhx', ITEMTYPE_BOOTS },
	{ ' blz', ITEMTYPE_BELT },
	{ ' bvz', ITEMTYPE_BELT },
	{ ' bmz', ITEMTYPE_BELT },
	{ ' btz', ITEMTYPE_BELT },
	{ ' bhz', ITEMTYPE_BELT },
	{ ' 9hx', ITEMTYPE_HELM },
	{ ' hsx', ITEMTYPE_SHIELD },
	{ ' kpx', ITEMTYPE_SHIELD },
	{ ' 1fq', ITEMTYPE_MACE },
	{ ' 2fq', ITEMTYPE_MACE },
};

//D2Common.0x6FDEAF00
int dword_6FDEAF00;
//D2Common.0x6FDEAF04
BOOL dword_6FDEAF04;

//D2Common.0x6FDEA708
D2UnkInventoryComponentStrc stru_6FDEA708[255] = {};


//D2Common.0x6FD8E210 -----
BOOL __fastcall INVENTORY_RemoveItem(D2UnitStrc* pItem)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2ItemExtraDataStrc* pItemExtraData = NULL;
	D2ItemExtraDataStrc* pExtraData = NULL;
	D2UnitStrc** ppItem = NULL;
	int nNodePos = 0;
	uint8_t nHeight = 0;
	uint8_t nWidth = 0;
	D2CoordStrc pCoords = {};

	pItemExtraData = INVENTORY_GetItemExtraDataFromItem(pItem);

	if (pItemExtraData && pItemExtraData->pParentInv && pItemExtraData->pParentInv->dwSignature == 0x1020304)
	{
		if (pItemExtraData->nNodePos > 0)
		{
			nNodePos = pItemExtraData->nNodePos - 1;
			if (pItemExtraData->pParentInv->nGridCount > nNodePos)
			{
				pInventoryGrid = &pItemExtraData->pParentInv->pGrids[nNodePos];

				if (pItemExtraData->unk0x10)
				{
					pExtraData = INVENTORY_GetItemExtraDataFromItem(pItemExtraData->unk0x10);
					if (pExtraData)
					{
						pExtraData->unk0x14 = pItemExtraData->unk0x14;
					}
				}
				else
				{
					if (pInventoryGrid->pItem == pItem)
					{
						pInventoryGrid->pItem = pItemExtraData->unk0x14;
					}
				}

				if (pItemExtraData->unk0x14)
				{
					pExtraData = INVENTORY_GetItemExtraDataFromItem(pItemExtraData->unk0x14);
					if (pExtraData)
					{
						pExtraData->unk0x10 = pItemExtraData->unk0x10;
					}
				}
				else
				{
					if (pInventoryGrid->pLastItem == pItem)
					{
						pInventoryGrid->pLastItem = pItemExtraData->unk0x10;
					}
				}

				if (pInventoryGrid->ppItems)
				{
					UNITS_GetCoords(pItem, &pCoords);

					if (nNodePos < 1)
					{
						pInventoryGrid->ppItems[pCoords.nX + pCoords.nY * pInventoryGrid->nGridWidth] = NULL;
					}
					else
					{
						ITEMS_GetDimensions(pItem, &nWidth, &nHeight, __FILE__, __LINE__);

						for (int j = pCoords.nY; j < pCoords.nY + nHeight; ++j)
						{
							ppItem = &pInventoryGrid->ppItems[j * pInventoryGrid->nGridWidth];

							for (int i = pCoords.nX; i < pCoords.nX + nWidth; ++i)
							{
								ppItem[i] = NULL;
							}
						}
					}
				}

				pItemExtraData->unk0x14 = NULL;
				pItemExtraData->unk0x10 = NULL;
				pItemExtraData->nNodePos = 0;
			}
		}

		if (pItemExtraData->pParentInv->pCursorItem == pItem)
		{
			pItemExtraData->pParentInv->pCursorItem = NULL;
		}
		else
		{
			if (pItemExtraData->pPreviousItem)
			{
				pExtraData = INVENTORY_GetItemExtraDataFromItem(pItemExtraData->pPreviousItem);
				if (pExtraData)
				{
					pExtraData->pNextItem = pItemExtraData->pNextItem;
				}
			}
			else
			{
				if (pItemExtraData->pParentInv->pFirstItem == pItem)
				{
					pItemExtraData->pParentInv->pFirstItem = pItemExtraData->pNextItem;
				}
			}

			if (pItemExtraData->pNextItem)
			{
				pExtraData = INVENTORY_GetItemExtraDataFromItem(pItemExtraData->pNextItem);
				if (pExtraData)
				{
					pExtraData->pPreviousItem = pItemExtraData->pPreviousItem;
				}
			}
			else
			{
				if (pItemExtraData->pParentInv->pLastItem == pItem)
				{
					pItemExtraData->pParentInv->pLastItem = pItemExtraData->pPreviousItem;
				}
			}

			pItemExtraData->pNextItem = NULL;
			pItemExtraData->pPreviousItem = NULL;

			--pItemExtraData->pParentInv->dwItemCount;
		}

		pItemExtraData->pParentInv = NULL;
		pItemExtraData->nNodePos = 0;
		pItemExtraData->nNodePosOther = 0;

		ITEMS_SetOwnerId(pItem, -1);

		if (pItemExtraData->pParentInv->pOwner)
		{
			STATLIST_ExpireUnitStatlist(pItemExtraData->pParentInv->pOwner, pItem);
		}

		if (pItemExtraData->pParentInv->dwLeftItemGUID == pItem->dwUnitId)
		{
			pItemExtraData->pParentInv->dwLeftItemGUID = -1;
		}

		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD8E4A0
D2ItemExtraDataStrc* __fastcall INVENTORY_GetItemExtraDataFromItem(D2UnitStrc* pItem)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM && pItem->pItemData)
	{
		return &pItem->pItemData->pExtraData;
	}
	
	return NULL;
}

//D2Common.0x6FD8E4C0 (#10240)
D2InventoryStrc* __stdcall INVENTORY_AllocInventory(void* pMemPool, D2UnitStrc* pOwner)
{
	D2InventoryStrc* pInventory = NULL;

	if (!pMemPool && pOwner)
	{
		pMemPool = pOwner->pMemoryPool;
	}

	pInventory = (D2InventoryStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2InventoryStrc), __FILE__, __LINE__, 0);
	if (pInventory)
	{
		memset(pInventory, 0x00, sizeof(D2InventoryStrc));

		pInventory->dwSignature = 0x1020304;
		pInventory->pMemPool = pMemPool;
		pInventory->pOwner = pOwner;

		if (pOwner)
		{
			pOwner->pInventory = pInventory;
			pInventory->dwOwnerId = pOwner->dwUnitId;
		}
	}

	return pInventory;
}

//D2Common.0x6FD8E520 (#10241)
void __stdcall INVENTORY_FreeInventory(D2InventoryStrc* pInventory)
{
	D2InventoryNodeStrc* pNextNode = NULL;
	D2CorpseStrc* pNextCorpse = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		if (pInventory->pCursorItem)
		{
			INVENTORY_RemoveItem(pInventory->pCursorItem);
		}

		for (D2UnitStrc* pItem = pInventory->pFirstItem; pItem; pItem = pInventory->pFirstItem)
		{
			INVENTORY_RemoveItem(pItem);
		}

		for (int i = 0; i < pInventory->nGridCount; ++i)
		{
			if (pInventory->pGrids[i].ppItems)
			{
				FOG_FreeServerMemory(pInventory->pMemPool, pInventory->pGrids[i].ppItems, __FILE__, __LINE__, 0);
			}
		}

		if (pInventory->pGrids)
		{
			FOG_FreeServerMemory(pInventory->pMemPool, pInventory->pGrids, __FILE__, __LINE__, 0);
		}

		if (pInventory->pOwner)
		{
			pInventory->pOwner->pInventory = NULL;
		}
		
		for (D2InventoryNodeStrc* pNode = pInventory->pFirstNode; pNode; pNode = pNextNode)
		{
			pNextNode = pNode->pNext;
			FOG_FreeServerMemory(pInventory->pMemPool, pNode, __FILE__, __LINE__, 0);
			
		}

		for (D2CorpseStrc* pCorpse = pInventory->pFirstCorpse; pCorpse; pCorpse = pNextCorpse)
		{
			pNextCorpse = pCorpse->pNextCorpse;
			FOG_FreeServerMemory(pInventory->pMemPool, pCorpse, __FILE__, __LINE__, 0);	
		}

		FOG_FreeServerMemory(pInventory->pMemPool, pInventory, __FILE__, __LINE__, 0);
	}
}

//D2Common.0x6FD8E620 (#10244)
BOOL __stdcall INVENTORY_CompareWithItemsParentInventory(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	D2ItemExtraDataStrc* pItemExtraData = NULL;
	D2InventoryStrc** ppParentInv = NULL;

	if (pInventory)
	{
		pItemExtraData = INVENTORY_GetItemExtraDataFromItem(pItem);
		ppParentInv = &pItemExtraData->pParentInv;

		if (ppParentInv) // TODO: *ppParentInv?
		{
			return *ppParentInv == pInventory;
		}
	}

	return FALSE;
}

//D2Common.0x6FD8E660 (#10243)
D2UnitStrc* __stdcall INVENTORY_RemoveItemFromInventory(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	// TODO: Check return value
	if (INVENTORY_CompareWithItemsParentInventory(pInventory, pItem))
	{
		INVENTORY_RemoveItem(pItem);
		return pItem;
	}
	
	return NULL;
}

//D2Common.0x6FD8E6A0 (#10242)
BOOL __stdcall INVENTORY_PlaceItemInSocket(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nUnused)
{
	D2ItemExtraDataStrc* pLastItemExtraData = NULL;
	D2ItemExtraDataStrc* pItemExtraData = NULL;
	D2UnitStrc* pLastItem = NULL;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return FALSE;
	}

	if (!pInventory->pOwner || pInventory->pOwner->dwUnitType != UNIT_ITEM)
	{
		return TRUE;
	}

	pItemExtraData = INVENTORY_GetItemExtraDataFromItem(pItem);
	if (!pItemExtraData)
	{
		return FALSE;
	}

	INVENTORY_RemoveItem(pItem);

	pLastItem = pInventory->pLastItem;
	if (pLastItem)
	{
		pLastItemExtraData = INVENTORY_GetItemExtraDataFromItem(pLastItem);
		if (pLastItemExtraData)
		{
			pLastItemExtraData->pNextItem = pItem;
		}
	}
	else
	{
		pInventory->pFirstItem = pItem;
	}

	pItemExtraData->pPreviousItem = pInventory->pLastItem;
	pInventory->pLastItem = pItem;
	pItemExtraData->pParentInv = pInventory;

	UNITS_SetXForStaticUnit(pItem, pInventory->dwItemCount);
	UNITS_SetYForStaticUnit(pItem, 0);

	pItemExtraData->nNodePos = 0;
	pItemExtraData->nNodePosOther = 1;

	++pInventory->dwItemCount;

	ITEMS_SetOwnerId(pItem, -1);

	return TRUE;
}

//D2Common.0x6FD8E7A0 (#10277)
D2UnitStrc* __stdcall INVENTORY_GetFirstItem(D2InventoryStrc* pInventory)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		return pInventory->pFirstItem;
	}

	return NULL;
}

//D2Common.0x6FD8E7C0 (#10278)
D2UnitStrc* __stdcall INVENTORY_GetLastItem(D2InventoryStrc* pInventory)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		return pInventory->pLastItem;
	}

	return NULL;
}

//D2Common.0x6FD8E7E0 (#10245)
BOOL __stdcall INVENTORY_GetFreePosition(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nInventoryRecordId, int* pFreeX, int* pFreeY, uint8_t nPage)
{
	D2InventoryGridInfoStrc pInventoryGridInfo = {};
	D2InventoryGridStrc* pInventoryGrid = NULL;
	int nX = 0;
	int nY = 0;
	uint8_t nWidth = 0;
	uint8_t nHeight = 0;

	if (!pInventory || pInventory->dwSignature != 0x1020304 || !pItem || pItem->dwUnitType != UNIT_ITEM || !pItem->pItemData)
	{
		return FALSE;
	}
	
	ITEMS_GetDimensions(pItem, &nWidth, &nHeight, __FILE__, __LINE__);
	if (!nWidth || !nHeight)
	{
		return FALSE;
	}

	DATATBLS_GetInventoryGridInfo(nInventoryRecordId, 0, &pInventoryGridInfo);

	pInventoryGrid = INVENTORY_GetGrid(pInventory, nPage + 2, &pInventoryGridInfo);
	if (!pInventoryGrid)
	{
		return FALSE;
	}

	if (nHeight == 1)
	{
		if (pInventory->pOwner && pInventory->pOwner->dwUnitType == UNIT_PLAYER)
		{
			return sub_6FD8ECF0(pInventoryGrid, pFreeX, pFreeY, nWidth, 1);
		}

		for (int nX = pInventoryGrid->nGridWidth - 1; nX >= 0; --nX)
		{
			for (int nY = 0; nY < pInventoryGrid->nGridHeight; ++nY)
			{
				if (INVENTORY_CanItemBePlacedAtPos(pInventoryGrid, nX, nY, nWidth, 1))
				{
					*pFreeX = nX;
					*pFreeY = nY;

					return TRUE;
				}
			}
		}

		return FALSE;
	}

	if (pInventory->pOwner && pInventory->pOwner->dwUnitType == UNIT_PLAYER)
	{
		return sub_6FD8EFB0(pInventoryGrid, pFreeX, pFreeY, nWidth, nHeight);
	}

	return sub_6FD8F0E0(pInventoryGrid, pFreeX, pFreeY, nWidth, nHeight);
}

//D2Common.0x6FD8EAF0
D2InventoryGridStrc* __fastcall INVENTORY_GetGrid(D2InventoryStrc* pInventory, int nInventoryGrid, D2InventoryGridInfoStrc* pInventoryGridInfo)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;

	if (pInventory->nGridCount <= nInventoryGrid)
	{
		if (!pInventoryGridInfo)
		{
			return NULL;
		}

		pInventory->pGrids = (D2InventoryGridStrc*)FOG_ReallocServerMemory(pInventory->pMemPool, pInventory->pGrids, sizeof(D2InventoryGridStrc) * (nInventoryGrid + 1), __FILE__, __LINE__, 0);
		memset(&pInventory->pGrids[pInventory->nGridCount], 0x00, sizeof(D2InventoryGridStrc) * (nInventoryGrid - pInventory->nGridCount + 1));
		pInventory->nGridCount = nInventoryGrid + 1;

		pInventoryGrid = &pInventory->pGrids[nInventoryGrid];
		pInventoryGrid->nGridWidth = pInventoryGridInfo->nGridX;
		pInventoryGrid->nGridHeight = pInventoryGridInfo->nGridY;
		pInventoryGrid->ppItems = (D2UnitStrc**)FOG_AllocServerMemory(pInventory->pMemPool, sizeof(D2UnitStrc*) * pInventoryGrid->nGridHeight * pInventoryGrid->nGridWidth, __FILE__, __LINE__, 0);
		memset(pInventoryGrid->ppItems, 0x00, sizeof(D2UnitStrc*) * pInventoryGrid->nGridHeight * pInventoryGrid->nGridWidth);
	}
	else
	{
		pInventoryGrid = &pInventory->pGrids[nInventoryGrid];
		if (pInventoryGridInfo)
		{
			if (!pInventoryGrid->ppItems)
			{
				pInventoryGrid->nGridWidth = pInventoryGridInfo->nGridX;
				pInventoryGrid->nGridHeight = pInventoryGridInfo->nGridY;
				pInventoryGrid->ppItems = (D2UnitStrc**)FOG_AllocServerMemory(pInventory->pMemPool, sizeof(D2UnitStrc*) * pInventoryGrid->nGridHeight * pInventoryGrid->nGridWidth, __FILE__, __LINE__, 0);
				memset(pInventoryGrid->ppItems, 0x00, sizeof(D2UnitStrc*) * pInventoryGrid->nGridHeight * pInventoryGrid->nGridWidth);
			}

			if (pInventoryGrid->nGridWidth != pInventoryGridInfo->nGridX || pInventoryGrid->nGridHeight != pInventoryGridInfo->nGridY)
			{
				return NULL;
			}
		}
		else if (!pInventoryGrid->ppItems)
		{
			return NULL;
		}
	}

	return pInventoryGrid;
}

//D2Common.0x6FD8EC70
BOOL __fastcall INVENTORY_CanItemBePlacedAtPos(D2InventoryGridStrc* pInventoryGrid, int nX, int nY, uint8_t nItemWidth, uint8_t nItemHeight)
{
	if (nItemWidth + nX > pInventoryGrid->nGridWidth && nItemHeight + nY > pInventoryGrid->nGridHeight)
	{
		return FALSE;
	}

	for (int nTestY = 0; nTestY < nItemHeight; ++nTestY)
	{
		for (int nTestX = 0; nTestX < nItemWidth; ++nTestX)
		{
			if (pInventoryGrid->ppItems[(nX + nTestX) + (nY + nTestY) * pInventoryGrid->nGridWidth])
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//D2Common.0x6FD8ECF0
//TODO: Find a name, v15, v17
BOOL __fastcall sub_6FD8ECF0(D2InventoryGridStrc* pInventoryGrid, int* pFreeX, int* pFreeY, uint8_t nItemWidth, uint8_t nItemHeight)
{
	uint8_t v15 = 0;
	uint8_t v17 = 0;

	if (pInventoryGrid->nGridWidth < 1 || pInventoryGrid->nGridHeight < 1)
	{
		return FALSE;
	}

	for (int nX = pInventoryGrid->nGridWidth - 1; nX >= 0; --nX)
	{
		for (int nY = pInventoryGrid->nGridHeight - 1; nY >= 0; --nY)
		{
			if (INVENTORY_CanItemBePlacedAtPos(pInventoryGrid, nX, nY, nItemWidth, nItemHeight))
			{
				v15 = sub_6FD8EE20(pInventoryGrid, nX, nY, nItemWidth, nItemHeight);

				if (v15 > v17)
				{
					v17 = v15;

					*pFreeX = nX;
					*pFreeY = nY;

					if (v15 == 255)
					{
						return TRUE;
					}
				}
			}
		}
	}

	return v17 > 0;
}

//D2Common.0x6FD8EE20
//TODO: Find a name
int __fastcall sub_6FD8EE20(D2InventoryGridStrc* pInventoryGrid, int nXPos, int nYPos, uint8_t nItemWidth, uint8_t nItemHeight)
{
	int nResult = 0;

	if (nXPos > 0)
	{
		for (int i = 0; i < nItemHeight; ++i)
		{
			if (pInventoryGrid->ppItems[nXPos - 1 + (nYPos + i) * pInventoryGrid->nGridWidth])
			{
				++nResult;
			}
		}
	}
	else
	{
		nResult = nItemHeight;
	}

	if (nXPos + nItemWidth < pInventoryGrid->nGridWidth)
	{
		for (int i = 0; i < nItemHeight; ++i)
		{
			if (pInventoryGrid->ppItems[nXPos + nItemWidth + (nYPos + i) * pInventoryGrid->nGridWidth])
			{
				++nResult;
			}
		}
	}
	else
	{
		nResult += nItemHeight;
	}

	if (nYPos > 0)
	{
		for (int i = 0; i < nItemWidth; ++i)
		{
			if (pInventoryGrid->ppItems[(nXPos + i) + (nYPos - 1) * pInventoryGrid->nGridWidth])
			{
				++nResult;
			}
		}
	}
	else
	{
		nResult += nItemWidth;
	}

	if (nYPos + nItemHeight < pInventoryGrid->nGridHeight)
	{
		for (int i = 0; i < nItemWidth; ++i)
		{
			if (pInventoryGrid->ppItems[(nXPos + i) + (nYPos + nItemHeight) * pInventoryGrid->nGridWidth])
			{
				++nResult;
			}
		}
	}
	else
	{
		nResult += nItemWidth;
	}

	if (nResult >= 2 * (nItemHeight + nItemWidth))
	{
		nResult = 255;
	}

	return nResult;
}

//D2Common.0x6FD8EFB0
//TODO: Find a name, v14, v16
BOOL __fastcall sub_6FD8EFB0(D2InventoryGridStrc* pInventoryGrid, int* pFreeX, int* pFreeY, uint8_t nItemWidth, uint8_t nItemHeight)
{
	uint8_t v14 = 0;
	uint8_t v16 = 0;

	if (pInventoryGrid->nGridWidth <= 0 || pInventoryGrid->nGridHeight <= 0)
	{
		return FALSE;
	}

	for (int nY = 0; nY < pInventoryGrid->nGridHeight; ++nY)
	{
		for (int nX = 0; nX < pInventoryGrid->nGridWidth; ++nX)
		{
			if (INVENTORY_CanItemBePlacedAtPos(pInventoryGrid, nX, nY, nItemWidth, nItemHeight))
			{
				v14 = sub_6FD8EE20(pInventoryGrid, nX, nY, nItemWidth, nItemHeight);

				if (v14 > v16)
				{
					v16 = v14;

					*pFreeX = nX;
					*pFreeY = nY;

					if (v14 == 255)
					{
						return TRUE;
					}
				}
			}
		}
	}

	return v16 > 0;
}

//D2Common.0x6FD8F0E0
//TODO: Find a name
BOOL __fastcall sub_6FD8F0E0(D2InventoryGridStrc* pInventoryGrid, int* pFreeX, int* pFreeY, uint8_t nItemWidth, uint8_t nItemHeight)
{
	for (int nX = 0; nX < pInventoryGrid->nGridWidth; ++nX)
	{
		for (int nY = 0; nY < pInventoryGrid->nGridHeight; ++nY)
		{
			if (INVENTORY_CanItemBePlacedAtPos(pInventoryGrid, nX, nY, nItemWidth, nItemHeight))
			{
				*pFreeX = nX;
				*pFreeY = nY;
				return TRUE;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD8F1E0 (#10246)
BOOL __stdcall INVENTORY_PlaceItemAtFreePosition(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nInventoryRecordId, BOOL bUnused, uint8_t nPage, char* szFile, int nLine)
{
	int nX = 0;
	int nY = 0;

	if (pInventory && pItem && pItem->dwUnitType == UNIT_ITEM && INVENTORY_GetFreePosition(pInventory, pItem, nInventoryRecordId, &nX, &nY, nPage))
	{
		return INVENTORY_PlaceItemInGrid(pInventory, pItem, nX, nY, nPage + 2, nInventoryRecordId, bUnused);
	}

	return FALSE;
}

//D2Common.0x6FD8F250
BOOL __fastcall INVENTORY_PlaceItemInGrid(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nXPos, int nYPos, int nInventoryGrid, int nInventoryRecordId, BOOL bUnused)
{
	D2InventoryGridInfoStrc pInventoryGridInfo = {};
	D2ItemExtraDataStrc* pLastItemExtraData = NULL;
	D2ItemExtraDataStrc* pItemExtraData = NULL;
	D2InventoryGridStrc* pInventoryGrid = NULL;
	int nOwnerId = 0;
	int nXMax = 0;
	int nYMax = 0;
	int nX = 0;
	int nY = 0;
	uint8_t nHeight = 0;
	uint8_t nWidth = 0;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return FALSE;
	}

	pItemExtraData = INVENTORY_GetItemExtraDataFromItem(pItem);
	if (!pItemExtraData)
	{
		return FALSE;
	}

	ITEMS_GetDimensions(pItem, &nWidth, &nHeight, __FILE__, __LINE__);
	if (!nWidth || !nHeight)
	{
		return FALSE;
	}

	if (nInventoryGrid != INVGRID_INVENTORY)
	{
		nWidth = 1;
		nHeight = 1;
		if (nInventoryGrid == INVGRID_BODYLOC)
		{
			pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
		}
		else if (nInventoryGrid == INVGRID_BELT)
		{
			pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BELT, &gBeltInventoryGridInfo);
		}
		else
		{
			return FALSE;
		}
	}
	else // INVGRID_INVENTORY
	{
		DATATBLS_GetInventoryGridInfo(nInventoryRecordId, 0, &pInventoryGridInfo);
		if (nXPos < 0 || nXPos + nWidth > pInventoryGridInfo.nGridX || nYPos < 0 || nYPos + nHeight > pInventoryGridInfo.nGridY)
		{
			return FALSE;
		}

		pInventoryGrid = INVENTORY_GetGrid(pInventory, nInventoryGrid, &pInventoryGridInfo);
	}

	if (!pInventoryGrid)
	{
		return FALSE;
	}

	nXMax = nXPos + nWidth;
	nYMax = nYPos + nHeight;

	nY = nYPos;
	while (nY < nYMax)
	{
		nX = nXPos;
		while (nX < nXMax && !pInventoryGrid->ppItems[nX + nY * pInventoryGrid->nGridWidth])
		{
			++nX;
		}

		if (nX < nXMax)
		{
			return FALSE;
		}

		++nY;
	}

	if (pItem->dwAnimMode == IMODE_ONGROUND && UNITS_GetRoom(pItem))
	{
		if (pItem->dwFlagEx & UNITFLAGEX_SERVERUNIT)
		{
			DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pItem), pItem->dwUnitType, pItem->dwUnitId);
		}

		UNITS_FreeCollisionPath(pItem);
		UNITROOM_RemoveUnitFromRoom(pItem);
	}

	INVENTORY_RemoveItem(pItem);

	if (pInventory->pLastItem)
	{
		pLastItemExtraData = INVENTORY_GetItemExtraDataFromItem(pInventory->pLastItem);
		if (pLastItemExtraData)
		{
			pLastItemExtraData->pNextItem = pItem;
		}
	}
	else
	{
		pInventory->pFirstItem = pItem;
	}

	pItemExtraData->pPreviousItem = pInventory->pLastItem;
	pInventory->pLastItem = pItem;
	pItemExtraData->pParentInv = pInventory;

	if (pInventoryGrid->pLastItem)
	{
		pLastItemExtraData = INVENTORY_GetItemExtraDataFromItem(pInventoryGrid->pLastItem);
		if (pLastItemExtraData)
		{
			pLastItemExtraData->unk0x14 = pItem;
		}
	}
	else
	{
		pInventoryGrid->pItem = pItem;
	}

	pItemExtraData->unk0x10 = pInventoryGrid->pLastItem;
	pInventoryGrid->pLastItem = pItem;

	for (int i = nYPos; i < nHeight + nYPos; ++i)
	{
		if (nXPos < nWidth + nXPos)
		{
			//memset32(&pInventoryGrid->ppItems[i * pInventoryGrid->nGridWidth] + nXPos, (int)pItem, nWidth);
			memset(&pInventoryGrid->ppItems[i * pInventoryGrid->nGridWidth] + nXPos, (int)pItem, nWidth);
		}
	}

	pItemExtraData->nNodePos = nInventoryGrid + 1;
	++pInventory->dwItemCount;

	UNITS_SetXForStaticUnit(pItem, nXPos);
	UNITS_SetYForStaticUnit(pItem, nYPos);

	if (pInventory->pOwner && pInventory->pOwner->dwUnitType == UNIT_PLAYER)
	{
		nOwnerId = pInventory->pOwner->dwUnitId;
	}
	else
	{
		nOwnerId = -1;
	}

	ITEMS_SetOwnerId(pItem, nOwnerId);
	if (nInventoryGrid >= 2)
	{
		ITEMS_SetInvPage(pItem, nInventoryGrid - 2);
	}

	if (nInventoryGrid != INVGRID_BODYLOC)
	{
		if (nInventoryGrid == INVGRID_BELT)
		{
			pItemExtraData->nNodePosOther = 2;
		}
		else
		{
			pItemExtraData->nNodePosOther = 1;
		}
	}
	else
	{
		if (nXPos >= 11)
		{
			pItemExtraData->nNodePosOther = 4;
		}
		else
		{
			pItemExtraData->nNodePosOther = 3;
		}
	}

	return TRUE;
}

//D2Common.0x6FD8F600 (#10247)
BOOL __stdcall INVENTORY_CanItemBePlaced(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nXPos, int nYPos, int nInventoryRecordId, D2UnitStrc** ppExchangeItem, unsigned int* pHoveredItems, uint8_t nPage)
{
	D2InventoryGridInfoStrc pInventoryGridInfo = {};
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pCurrentItem = NULL;
	BOOL bFree = FALSE;
	uint8_t nHeight = 0;
	uint8_t nWidth = 0;

	*pHoveredItems = 0;

	if (!pInventory || pInventory->dwSignature != 0x1020304 || !pItem || pItem->dwUnitType != UNIT_ITEM || nPage == -1 || nXPos < 0 || nYPos < 0)
	{
		return FALSE;
	}

	ITEMS_GetDimensions(pItem, &nWidth, &nHeight, __FILE__, __LINE__);
	if (!nWidth || !nHeight)
	{
		return FALSE;
	}

	DATATBLS_GetInventoryGridInfo(nInventoryRecordId, FALSE, &pInventoryGridInfo);

	pInventoryGrid = INVENTORY_GetGrid(pInventory, nPage + 2, &pInventoryGridInfo);
	if (!pInventoryGrid)
	{
		return FALSE;
	}

	bFree = TRUE;

	for (int nY = nYPos; nY < nYPos + nHeight; ++nY)
	{
		if (nY >= pInventoryGrid->nGridHeight)
		{
			return FALSE;
		}

		for (int nX = nXPos; nX < nXPos + nWidth; ++nX)
		{
			if (nX >= pInventoryGrid->nGridWidth)
			{
				return FALSE;
			}

			pCurrentItem = pInventoryGrid->ppItems[nX + nY * pInventoryGrid->nGridWidth];
			if (pCurrentItem)
			{
				bFree = FALSE;

				if (*ppExchangeItem != pCurrentItem)
				{
					*ppExchangeItem = pCurrentItem;

					++*pHoveredItems;
				}
				else
				{
					break;
				}
			}
		}
	}

	return bFree;
}

//D2Common.0x6FD8F780 (#10248)
BOOL __stdcall INVENTORY_CanItemsBeExchanged(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nXPos, int nYPos, int nInventoryRecordId, D2UnitStrc** ppExchangeItem, uint8_t nPage, BOOL bCheckIfCube)
{
	D2InventoryGridInfoStrc pInventoryGridInfo = {};
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pCurrentItem = NULL;
	D2UnitStrc* pLastItem = NULL;
	int nCounter = 0;
	int nXMax = 0;
	int nYMax = 0;
	uint8_t nHeight = 0;
	uint8_t nWidth = 0;

	if (!pInventory || pInventory->dwSignature != 0x1020304 || !pItem || pItem->dwUnitType != UNIT_ITEM || nPage == -1 || nXPos < 0 || nYPos < 0)
	{
		return FALSE;
	}

	ITEMS_GetDimensions(pItem, &nWidth, &nHeight, __FILE__, __LINE__);
	if (!nWidth || !nHeight)
	{
		return FALSE;
	}

	DATATBLS_GetInventoryGridInfo(nInventoryRecordId, FALSE, &pInventoryGridInfo);
	pInventoryGrid = INVENTORY_GetGrid(pInventory, nPage + 2, &pInventoryGridInfo);
	if (!pInventoryGrid)
	{
		return FALSE;
	}

	nXMax = nXPos + nWidth;
	nYMax = nYPos + nHeight;

	if (nXMax > pInventoryGrid->nGridWidth || nYMax > pInventoryGrid->nGridHeight)
	{
		return FALSE;
	}

	for (int nY = nYPos; nY < nYMax; ++nY)
	{
		for (int nX = nXPos; nX < nXMax; ++nX)
		{
			pCurrentItem = pInventoryGrid->ppItems[nX + nY * pInventoryGrid->nGridWidth];
			if (pCurrentItem)
			{
				*ppExchangeItem = pCurrentItem;

				if (bCheckIfCube && DATATBLS_GetItemsTxtRecord(pCurrentItem->dwClassId)->dwCode == ' xob')
				{
					return FALSE;
				}

				if (pCurrentItem != pLastItem)
				{
					pLastItem = pCurrentItem;

					++nCounter;
				}
			}
		}
	}

	if (nCounter > 1)
	{
		*ppExchangeItem = NULL;
		return FALSE;
	}

	return TRUE;
}

//D2Common.0x6FD8F930 (#10249)
BOOL __stdcall INVENTORY_PlaceItemAtInventoryPage(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nXPos, int nYPos, int nInventoryRecordId, BOOL bUnused, uint8_t nPage)
{
	if (pInventory)
	{
		return INVENTORY_PlaceItemInGrid(pInventory, pItem, nXPos, nYPos, nPage + 2, nInventoryRecordId, bUnused);
	}

	return FALSE;
}

//D2Common.0x6FD8F970 (#10250)
void __stdcall INVENTORY_Return(char* szFile, int nLine, D2InventoryStrc* pInventory, int nX, int nY, int nInventoryRecordId, BOOL bClient, uint8_t nPage)
{
	return;
}

//D2Common.0x6FD8F980 (#10252)
D2UnitStrc* __stdcall INVENTORY_GetItemFromInventoryPage(D2InventoryStrc* pInventory, int nGridX, int nGridY, int* pX, int* pY, int nInventoryRecordId, uint8_t nPage)
{
	D2InventoryGridInfoStrc pInventoryGridInfo = {};
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return NULL;
	}

	DATATBLS_GetInventoryGridInfo(nInventoryRecordId, 0, &pInventoryGridInfo);

	pInventoryGrid = INVENTORY_GetGrid(pInventory, nPage + 2, &pInventoryGridInfo);
	if (pInventoryGrid && nGridX >= 0 && nGridX <= pInventoryGrid->nGridWidth && nGridY >= 0 && nGridY <= pInventoryGrid->nGridHeight)
	{
		pItem = pInventoryGrid->ppItems[nGridX + nGridY * pInventoryGrid->nGridWidth];
		if (pItem)
		{
			switch (pItem->dwUnitType)
			{
			case UNIT_PLAYER:
			case UNIT_MONSTER:
			case UNIT_MISSILE:
				*pX = pItem->pDynamicPath ? pItem->pDynamicPath->wPosX : 0;
				*pY = pItem->pDynamicPath ? pItem->pDynamicPath->wPosY : 0;
				break;

			case UNIT_OBJECT:
			case UNIT_ITEM:
			case UNIT_TILE:
				*pX = pItem->pStaticPath ? pItem->pStaticPath->nXPos : 0;
				*pY = pItem->pStaticPath ? pItem->pStaticPath->nYPos : 0;
				break;

			default:
				*pX = 0;
				*pY = 0;
				break;
			}

			return pItem;
		}
	}

	return NULL;
}

//D2Common.0x6FD8FAB0 (#10253)
BOOL __stdcall INVENTORY_PlaceItemInBodyLoc(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nBodyLoc)
{
	if (pInventory && nBodyLoc >= 0 && nBodyLoc < 13)
	{
		return INVENTORY_PlaceItemInGrid(pInventory, pItem, nBodyLoc, 0, INVGRID_BODYLOC, 0, 0);
	}

	return FALSE;
}

//D2Common.0x6FD8FAE0 (#10257)
D2UnitStrc* __stdcall INVENTORY_GetItemFromBodyLoc(D2InventoryStrc* pInventory, int nBodyLoc)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304 && nBodyLoc >= 0 && nBodyLoc < 13)
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
		if (pInventoryGrid)
		{
			return pInventoryGrid->ppItems[nBodyLoc];
		}
	}

	return NULL;
}

//D2Common.0x6FD8FB20 (#10255)
void __stdcall INVENTORY_GetSecondWieldingWeapon(D2UnitStrc* pPlayer, D2InventoryStrc* pInventory, D2UnitStrc** ppItem, int nBodyLoc)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304 && !*ppItem && nBodyLoc >= 0 && nBodyLoc < 13)
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
		if (pInventoryGrid)
		{
			if (nBodyLoc == BODYLOC_RARM)
			{
				if (INVENTORY_GetWieldType(pPlayer, pInventory) == 2)
				{
					*ppItem = pInventoryGrid->ppItems[BODYLOC_LARM];
				}
			}
			else if (nBodyLoc == BODYLOC_LARM)
			{
				if (INVENTORY_GetWieldType(pPlayer, pInventory) == 2)
				{
					*ppItem = pInventoryGrid->ppItems[BODYLOC_RARM];
				}
			}
		}
	}
}

//D2Common.0x6FD8FBB0 (#10256)
BOOL __stdcall INVENTORY_CheckEquipmentForWeaponByClass(D2InventoryStrc* pInventory, int nWeaponClass)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);

		pItem = pInventoryGrid ? pInventoryGrid->ppItems[BODYLOC_LARM] : NULL;

		if (ITEMS_CanBeEquipped(pItem) && ITEMS_CheckWeaponClass(pItem, nWeaponClass))
		{
			return TRUE;
		}

		pItem = pInventoryGrid ? pInventoryGrid->ppItems[BODYLOC_RARM] : NULL;

		if (ITEMS_CanBeEquipped(pItem) && ITEMS_CheckWeaponClass(pItem, nWeaponClass))
		{
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD8FC60 (#10258)
D2UnitStrc* __stdcall INVENTORY_GetLeftHandWeapon(D2InventoryStrc* pInventory)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304 && pInventory->dwLeftItemGUID != -1)
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
		if (pInventoryGrid)
		{
			pItem = pInventoryGrid->ppItems[BODYLOC_LARM];
			if (pItem && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) && pItem->dwUnitId == pInventory->dwLeftItemGUID)
			{
				return pItem;
			}

			pItem = pInventoryGrid->ppItems[BODYLOC_RARM];
			if (pItem && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) && pItem->dwUnitId == pInventory->dwLeftItemGUID)
			{
				return pItem;
			}
		}
	}

	return NULL;
}

//D2Common.0x6FD8FD10 (#11301)
D2UnitStrc* __stdcall INVENTORY_GetSecondaryWeapon(D2InventoryStrc* pInventory)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pRightHandItem = NULL;
	D2UnitStrc* pLeftHandItem = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304 && pInventory->dwLeftItemGUID != -1)
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
		if (pInventoryGrid)
		{
			pLeftHandItem = pInventoryGrid->ppItems[BODYLOC_LARM];
			if (!pLeftHandItem || !ITEMS_CheckItemTypeId(pLeftHandItem, ITEMTYPE_WEAPON))
			{
				pLeftHandItem = NULL;
			}

			pRightHandItem = pInventoryGrid->ppItems[BODYLOC_RARM];
			if (!pRightHandItem || !ITEMS_CheckItemTypeId(pRightHandItem, ITEMTYPE_WEAPON))
			{
				pRightHandItem = NULL;
			}

			if (pLeftHandItem && pLeftHandItem->dwUnitId == pInventory->dwLeftItemGUID)
			{
				return pRightHandItem;
			}

			if (pRightHandItem && pRightHandItem->dwUnitId == pInventory->dwLeftItemGUID)
			{
				return pLeftHandItem;
			}
		}
	}

	return NULL;
}

//D2Common.0x6FD8FDD0 (#10259)
D2UnitStrc* __stdcall INVENTORY_GetCompositItem(D2InventoryStrc* pInventory, int nComponent)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pLeftHandItem = NULL;
	D2UnitStrc* pItem = NULL;
	int nWeaponClass = 0;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return NULL;
	}

	pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
	if (pInventoryGrid)
	{
		pLeftHandItem = INVENTORY_GetLeftHandWeapon(pInventory);

		for (int i = 0; i < 11; ++i)
		{
			pItem = pInventoryGrid->ppItems[i];
			if (pItem)
			{
				nWeaponClass = ITEMS_GetWeaponClassId(pInventoryGrid->ppItems[i]);
				if (nWeaponClass == WEAPONCLASS_1HS || nWeaponClass == WEAPONCLASS_1HT || nWeaponClass == WEAPONCLASS_HT1)
				{
					if (nComponent == COMPOSIT_LEFTHAND)
					{
						if (pItem != pLeftHandItem)
						{
							return pItem;
						}
					}
					else if (nComponent == COMPOSIT_RIGHTHAND && pItem == pLeftHandItem)
					{
						return pItem;
					}
				}
				else
				{
					if (ITEMS_GetComponent(pItem) == nComponent)
					{
						return pItem;
					}
				}
			}
		}
	}

	return NULL;
}

//D2Common.0x6FD8FE80 (#10260)
int __stdcall INVENTORY_GetBodyLocFromEquippedItem(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	if (pInventory && pInventory->dwSignature == 0x1020304 && pItem && pItem->dwUnitType == UNIT_ITEM && pItem->pStaticPath->nXPos < 11)
	{
		return pItem->pStaticPath->nXPos;
	}

	return 0;
}

//D2Common.0x6FD8FED0 (#11278)
int __stdcall INVENTORY_GetItemsXPosition(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	if (pInventory && pInventory->dwSignature == 0x1020304 && pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		return pItem->pStaticPath->nXPos;
	}

	return 0;
}

//D2Common.0x6FD8FF20 (#10261)
void __stdcall INVENTORY_SetCursorItem(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	D2ItemExtraDataStrc* pItemExtraData = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		if (pItem)
		{
			pItemExtraData = INVENTORY_GetItemExtraDataFromItem(pItem);
			if (pItemExtraData)
			{
				pInventory->pCursorItem = pItem;
				pItemExtraData->pParentInv = pInventory;
			}
		}
		else
		{
			INVENTORY_RemoveItem(pInventory->pCursorItem);
		}
	}
}

//D2Common.0x6FD8FF80 (#10262)
D2UnitStrc* __stdcall INVENTORY_GetCursorItem(D2InventoryStrc* pInventory)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		return pInventory->pCursorItem;
	}

	return NULL;
}

//D2Common.0x6FD8FFA0 (#10263)
D2UnitStrc* __stdcall INVENTORY_FindBackPackItemForStack(D2InventoryStrc* pInventory, D2UnitStrc* a2, D2UnitStrc* a3)
{
	D2InventoryGridInfoStrc pInventoryGridInfo = {};
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return NULL;
	}

	if (pInventory->pOwner && pInventory->pOwner->dwUnitType == UNIT_PLAYER)
	{
		DATATBLS_GetInventoryGridInfo(UNITS_GetInventoryRecordId(pInventory->pOwner, 0, TRUE), FALSE, &pInventoryGridInfo);
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_INVENTORY, &pInventoryGridInfo);
	}
	else
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_INVENTORY, NULL);
	}

	if (!pInventoryGrid)
	{
		return NULL;
	}

	pItem = pInventoryGrid->pItem;
	if (a3)
	{
		while (pItem)
		{
			if (pItem == a3)
			{
				break;
			}

			pItem = INVENTORY_GetUnknownItem(pItem);
		}
	}

	while (pItem)
	{
		if (ITEMS_AreStackablesEqual(pItem, a2) && STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0) < ITEMS_GetTotalMaxStack(pItem))
		{
			return pItem;
		}

		pItem = INVENTORY_GetUnknownItem(pItem);
	}

	return NULL;
}

//D2Common.0x6FD90080 (#10264)
D2UnitStrc* __stdcall INVENTORY_FindEquippedItemForStack(D2InventoryStrc* pInventory, D2UnitStrc* a2, D2UnitStrc* a3)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return NULL;
	}

	pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
	if (!pInventoryGrid)
	{
		return NULL;
	}

	pItem = pInventoryGrid->pItem;
	if (a3)
	{
		while (pItem)
		{
			if (pItem == a3)
			{
				break;
			}

			pItem = INVENTORY_GetUnknownItem(pItem);
		}
	}

	while (pItem)
	{
		if (ITEMS_AreStackablesEqual(pItem, a2) && STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0) < ITEMS_GetTotalMaxStack(pItem))
		{
			return pItem;
		}

		pItem = INVENTORY_GetUnknownItem(pItem);
	}

	return NULL;
}

//D2Common.0x6FD90130 (#10265)
D2UnitStrc* __stdcall INVENTORY_FindFillableBook(D2InventoryStrc* pInventory, D2UnitStrc* pScrolls, D2UnitStrc* a3)
{
	D2InventoryGridInfoStrc pInventoryGridInfo = {};
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return NULL;
	}

	if (pInventory->pOwner && pInventory->pOwner->dwUnitType == UNIT_PLAYER)
	{
		DATATBLS_GetInventoryGridInfo(UNITS_GetInventoryRecordId(pInventory->pOwner, INVPAGE_INVENTORY, TRUE), FALSE, &pInventoryGridInfo);
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_INVENTORY, &pInventoryGridInfo);
	}
	else
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_INVENTORY, NULL);
	}

	if (!pInventoryGrid)
	{
		return NULL;
	}

	pItem = pInventoryGrid->pItem;
	if (a3)
	{
		while (pItem)
		{
			if (pItem == a3)
			{
				break;
			}

			pItem = INVENTORY_GetUnknownItem(pItem);
		}
	}

	while (pItem)
	{
		if (ITEMS_GetItemType(pItem) == ITEMTYPE_BOOK && ITEMS_GetSuffixId(pItem, 0) == ITEMS_GetSuffixId(pScrolls, 0) && STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0) < ITEMS_GetTotalMaxStack(pItem))
		{
			return pItem;
		}

		pItem = INVENTORY_GetUnknownItem(pItem);
	}

	return NULL;
}

//D2Common.0x6FD90230 (#10266)
BOOL __stdcall INVENTORY_PlaceItemInBeltSlot(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nSlot)
{
	uint8_t nHeight = 0;
	uint8_t nWidth = 0;

	if (pItem && pItem->dwUnitType == UNIT_ITEM && ITEMS_CheckIfBeltable(pItem))
	{
		ITEMS_GetDimensions(pItem, &nWidth, &nHeight, __FILE__, __LINE__);

		if (nWidth == 1 && nHeight == 1 && pInventory && nSlot >= 0 && nSlot < 16)
		{
			return INVENTORY_PlaceItemInGrid(pInventory, pItem, nSlot, 0, INVGRID_BELT, 0, 0);
		}
	}

	return FALSE;
}

//D2Common.0x6FD902B0 (#10268)
BOOL __stdcall INVENTORY_HasSimilarPotionInBelt(D2InventoryStrc* pInventory, D2UnitStrc* pPotion)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304 && pPotion && pPotion->dwUnitType == UNIT_ITEM)
	{
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pPotion->dwClassId);
		if (pItemsTxtRecord && pItemsTxtRecord->dwCode != ' csi' && pItemsTxtRecord->dwCode != ' cst')
		{
			pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BELT, &gBeltInventoryGridInfo);
			if (pInventoryGrid)
			{
				for (int i = 0; i < 4; ++i)
				{
					if (pInventoryGrid->ppItems[i] && ITEMS_ComparePotionTypes(pPotion, pInventoryGrid->ppItems[i]))
					{
						return TRUE;
					}
				}
			}
		}
	}
	
	return FALSE;
}

//D2Common.0x6FD90340 (#10269)
BOOL __stdcall INVENTORY_GetFreeBeltSlot(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int* pFreeSlotId)
{
	D2BeltsTxt pBeltsTxtRecord = {};
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2UnitStrc* pBelt = NULL;
	int nBeltType = 0;
	int nId = 0;
	uint8_t nHeight = 0;
	uint8_t nWidth = 0;

	if (!pInventory || pInventory->dwSignature != 0x1020304 || !pItem || pItem->dwUnitType != UNIT_ITEM || !ITEMS_CheckIfBeltable(pItem))
	{
		return FALSE;
	}

	ITEMS_GetDimensions(pItem, &nWidth, &nHeight, __FILE__, __LINE__);
	if (nWidth != 1 || nHeight != 1)
	{
		return FALSE;
	}

	pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
	if (pInventoryGrid)
	{
		pBelt = pInventoryGrid->ppItems[BODYLOC_BELT];
	}

	if (pBelt)
	{
		nBeltType = UNITS_GetBeltType(pBelt);
	}
	else
	{
		nBeltType = 2;
	}

	DATATBLS_GetBeltsTxtRecord(nBeltType, 0, &pBeltsTxtRecord);

	pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BELT, &gBeltInventoryGridInfo);
	if (pInventoryGrid)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (pInventoryGrid->ppItems[i] && ITEMS_ComparePotionTypes(pItem, pInventoryGrid->ppItems[i]))
			{
				for (int nId = i; nId < pBeltsTxtRecord.nBoxes; nId += 4)
				{
					if (!pInventoryGrid->ppItems[nId])
					{
						*pFreeSlotId = nId;
						return TRUE;
					}
				}
			}
		}

		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
		if (!pItemsTxtRecord || !pItemsTxtRecord->nAutoBelt)
		{
			return FALSE;
		}

		for (int i = 0; i < 4; ++i)
		{
			if (!pInventoryGrid->ppItems[i])
			{
				*pFreeSlotId = i;
				return TRUE;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD904F0 (#10270)
BOOL __stdcall INVENTORY_PlaceItemInFreeBeltSlot(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	int nFreeSlot = 0;

	if (INVENTORY_GetFreeBeltSlot(pInventory, pItem, &nFreeSlot) && pInventory && pItem && pItem->dwUnitType == UNIT_ITEM && nFreeSlot >= 0 && nFreeSlot < 16)
	{
		return INVENTORY_PlaceItemInGrid(pInventory, pItem, nFreeSlot, 0, INVGRID_BELT, 0, 0);
	}

	return FALSE;
}

//D2Common.0x6FD90550 (#10271)
D2UnitStrc* __stdcall INVENTORY_GetItemFromBeltSlot(D2InventoryStrc* pInventory, int nSlotId)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BELT, &gBeltInventoryGridInfo);
		if (pInventoryGrid && nSlotId >= 0 && nSlotId < 16)
		{
			return pInventoryGrid->ppItems[nSlotId];
		}
	}

	return NULL;
}

//D2Common.0x6FD90590 (#10272)
BOOL __stdcall INVENTORY_GetUseableItemFromBeltSlot(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nSlotId, D2UnitStrc** ppItem)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	uint8_t nHeight = 0;
	uint8_t nWidth = 0;

	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BELT, &gBeltInventoryGridInfo);
		if (pInventoryGrid && pItem && pItem->dwUnitType == UNIT_ITEM && ITEMS_CheckIfBeltable(pItem))
		{
			ITEMS_GetDimensions(pItem, &nWidth, &nHeight, __FILE__, __LINE__);

			if (nWidth == 1 && nHeight == 1)
			{
				pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
				if (pItemsTxtRecord && pItemsTxtRecord->nUseable == 1 && pItemsTxtRecord->nBelt == 1)
				{
					if (nSlotId >= 0 && nSlotId < 16)
					{
						*ppItem = pInventoryGrid->ppItems[nSlotId];
					}
					else
					{
						*ppItem = NULL;
					}

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD90690 (#10273)
BOOL __stdcall INVENTORY_GetEquippedShield(D2InventoryStrc* pInventory, D2UnitStrc** ppItem)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return FALSE;
	}

	if (ppItem)
	{
		*ppItem = NULL;
	}

	pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
	if (pInventoryGrid)
	{
		pItem = pInventoryGrid->ppItems[BODYLOC_RARM];
	}
	else
	{
		pItem = NULL;
	}

	if (ITEMS_CanBeEquipped(pItem) && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_SHIELD))
	{
		if (ppItem)
		{
			*ppItem = pItem;
		}
		return TRUE;
	}

	if (pInventoryGrid)
	{
		pItem = pInventoryGrid->ppItems[BODYLOC_LARM];
	}
	else
	{
		pItem = NULL;
	}

	if (ITEMS_CanBeEquipped(pItem) && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_SHIELD))
	{
		if (ppItem)
		{
			*ppItem = pItem;
		}
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FD90760 (#10274)
BOOL __stdcall INVENTORY_GetEquippedWeapon(D2InventoryStrc* pInventory, D2UnitStrc** ppItem, int* pBodyLoc, BOOL* pIsLeftHandItem)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pLeftHandItem = NULL;
	D2UnitStrc* pItem = NULL;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		*ppItem = NULL;
		return FALSE;
	}

	*pIsLeftHandItem = FALSE;

	pLeftHandItem = INVENTORY_GetLeftHandWeapon(pInventory);

	pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);

	pItem = pInventoryGrid ? pInventoryGrid->ppItems[BODYLOC_RARM] : NULL;

	if (ITEMS_CanBeEquipped(pItem) && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
	{
		*ppItem = pItem;
		*pBodyLoc = BODYLOC_RARM;

		if (pItem == pLeftHandItem)
		{
			*pIsLeftHandItem = TRUE;
		}

		return TRUE;
	}
	else
	{
		pItem = pInventoryGrid ? pInventoryGrid->ppItems[BODYLOC_LARM] : NULL;

		if (ITEMS_CanBeEquipped(pItem) && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
		{
			*ppItem = pItem;
			*pBodyLoc = BODYLOC_LARM;

			if (pItem == pLeftHandItem)
			{
				*pIsLeftHandItem = TRUE;
			}

			return TRUE;
		}
	}

	*ppItem = NULL;
	return FALSE;
}

//D2Common.0x6FD90850 (#10275)
BOOL __stdcall INVENTORY_HasBodyArmorEquipped(D2InventoryStrc* pInventory)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
		if (pInventoryGrid && pInventoryGrid->ppItems[BODYLOC_TORSO])
		{
			return ITEMS_CheckItemTypeId(pInventoryGrid->ppItems[BODYLOC_TORSO], ITEMTYPE_ARMOR) != 0;
		}
	}

	return FALSE;
}

//D2Common.0x6FD908A0 (#10276)
BOOL __stdcall INVENTORY_IsItemBodyLocFree(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nBodyLoc, int nInventoryRecordId)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	int nFreeX = 0;
	int nFreeY = 0;

	if (pInventory && pInventory->dwSignature == 0x1020304 && nBodyLoc >= 0 && nBodyLoc < 13)
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);

		if (pInventoryGrid && pInventoryGrid->ppItems[nBodyLoc] && pItem)
		{
			return INVENTORY_GetFreePosition(pInventory, pInventoryGrid->ppItems[nBodyLoc], nInventoryRecordId, &nFreeX, &nFreeY, 0) != 0;
		}
	}

	return FALSE;
}

//D2Common.0x6FD90910 (#10279)
void __stdcall INVENTORY_RemoveInventoryItems(D2InventoryStrc* pInventory)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		for (D2UnitStrc* i = pInventory->pFirstItem; i; i = pInventory->pFirstItem)
		{
			INVENTORY_RemoveItem(i);
		}
	}
}

//D2Common.0x6FD90940 (#10280)
D2InventoryNodeStrc* __stdcall INVENTORY_GetTradeInventory(D2InventoryStrc* pInventory)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		return pInventory->pFirstNode;
	}

	return NULL;
}

//D2Common.0x6FD90960 (#10281)
void __stdcall INVENTORY_FreeTradeInventory(D2InventoryStrc* pInventory)
{
	D2InventoryNodeStrc* pNextNode = NULL;
	D2InventoryNodeStrc* pNode = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		pNode = pInventory->pFirstNode;
		while (pNode)
		{
			pNextNode = pNode->pNext;
			FOG_FreeServerMemory(pInventory->pMemPool, pNode, __FILE__, __LINE__, 0);
			pNode = pNextNode;
		}

		pInventory->pFirstNode = NULL;
		pInventory->pLastNode = NULL;
	}
}

//D2Common.0x6FD909B0 (#10282)
BOOL __stdcall INVENTORY_CheckForItemInTradeInventory(D2InventoryStrc* pInventory, int nItemId)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		for (D2InventoryNodeStrc* pNode = pInventory->pFirstNode; pNode; pNode = pNode->pNext)
		{
			if (pNode->nItemId == nItemId)
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

//D2Common.0x6FD909F0 (#10283)
void __stdcall INVENTORY_AddItemToTradeInventory(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	D2InventoryNodeStrc* pNode = NULL;

	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		if (INVENTORY_CheckForItemInTradeInventory(pInventory, pItem->dwUnitId))
		{
			return;
		}

		pNode = (D2InventoryNodeStrc*)FOG_AllocServerMemory(pInventory->pMemPool, sizeof(D2InventoryNodeStrc), __FILE__, __LINE__, 0);
		D2_ASSERT(pNode);

		pNode->pNext = NULL;
		pNode->nItemId = pItem->dwUnitId;

		if (pInventory->pFirstNode)
		{
			pInventory->pLastNode->pNext = pNode;
		}
		else
		{
			pInventory->pFirstNode = pNode;
		}

		pInventory->pLastNode = pNode;
	}
}

//D2Common.0x6FD90AB0 (#10316)
//TODO: Find a name
int __stdcall D2Common_10316(D2CorpseStrc* pCorpse)
{
	if (pCorpse)
	{
		return pCorpse->unk0x00;
	}

	REMOVE_LATER_WriteToLogFile("D2Common_10316: NULL pointer");
	return 0;
}

//D2Common.0x6FD90AC0 (#10284)
int __stdcall INVENTORY_GetItemCount(D2InventoryStrc* pInventory)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		return pInventory->dwItemCount;
	}

	return 0;
}

//D2Common.0x6FD90AE0 (#10285)
D2UnitStrc* __stdcall INVENTORY_GetBackPackItemByType(D2InventoryStrc* pInventory, int nItemType, D2UnitStrc* a3)
{
	D2InventoryGridInfoStrc pInventoryGridInfo = {};
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return NULL;
	}

	if (pInventory->pOwner && pInventory->pOwner->dwUnitType == UNIT_PLAYER)
	{
		DATATBLS_GetInventoryGridInfo(UNITS_GetInventoryRecordId(pInventory->pOwner, 0, 1), 0, &pInventoryGridInfo);
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_INVENTORY, &pInventoryGridInfo);
	}
	else
	{
		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_INVENTORY, NULL);
	}
	
	if (!pInventoryGrid)
	{
		return NULL;
	}

	pItem = pInventoryGrid->pItem;
	if (a3)
	{
		while (pItem)
		{
			if (pItem == a3)
			{
				break;
			}

			pItem = INVENTORY_GetUnknownItem(pItem);
		}

		pItem = INVENTORY_GetUnknownItem(pItem);
	}

	while (pItem)
	{
		if (ITEMS_CheckItemTypeId(pItem, nItemType))
		{
			return pItem;
		}

		pItem = INVENTORY_GetUnknownItem(pItem);
	}

	return NULL;
}

//D2Common.0x6FD90BC0 (#10286)
D2UnitStrc* __stdcall INVENTORY_GetEquippedItemByType(D2InventoryStrc* pInventory, int nItemType, D2UnitStrc* a3)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return NULL;
	}

	pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
	if (!pInventoryGrid)
	{
		return NULL;
	}

	pItem = pInventoryGrid->pItem;
	if (a3)
	{
		while (pItem)
		{
			if (pItem == a3)
			{
				break;
			}

			pItem = INVENTORY_GetUnknownItem(pItem);
		}
	}

	while (pItem)
	{
		if (ITEMS_CheckItemTypeId(pItem, nItemType) && UNITS_GetXPosition(pItem) < 11)
		{
			return pItem;
		}

		pItem = INVENTORY_GetUnknownItem(pItem);
	}

	return NULL;
}

//D2Common.0x6FD90C80 (#10287)
D2UnitStrc* __stdcall INVENTORY_GetEquippedItemByCode(D2InventoryStrc* pInventory, int nItemCode, D2UnitStrc* a3)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;
	int nClassId = 0;

	if (!pInventory || pInventory->dwSignature != 0x1020304 || !DATATBLS_GetItemRecordFromItemCode(nItemCode, &nClassId))
	{
		return NULL;
	}

	pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
	if (!pInventoryGrid)
	{
		return NULL;
	}

	pItem = pInventoryGrid->pItem;
	if (a3)
	{
		while (pItem)
		{
			if (pItem == a3)
			{
				break;
			}

			pItem = INVENTORY_GetUnknownItem(pItem);
		}
	}

	while (pItem)
	{
		if (pItem->dwClassId == nClassId && UNITS_GetXPosition(pItem) < 11)
		{
			return pItem;
		}

		pItem = INVENTORY_GetUnknownItem(pItem);
	}

	return NULL;
}

//D2Common.0x6FD90D50 (#11306)
D2UnitStrc* __stdcall INVENTORY_GetBackPackItemByCode(D2InventoryStrc* pInventory, int nItemCode, D2UnitStrc* a3)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;
	int nClassId = 0;

	if (!pInventory || pInventory->dwSignature != 0x1020304 || !DATATBLS_GetItemRecordFromItemCode(nItemCode, &nClassId))
	{
		return NULL;
	}
	
	pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_INVENTORY, &gBodyLocInventoryGridInfo);
	if (!pInventoryGrid)
	{
		return NULL;
	}

	pItem = pInventoryGrid->pItem;
	if (a3)
	{
		while (pItem)
		{
			if (pItem == a3)
			{
				break;
			}

			pItem = INVENTORY_GetUnknownItem(pItem);
		}
	}

	while (pItem)
	{
		if (pItem->dwClassId == nClassId && UNITS_GetXPosition(pItem) < 11)
		{
			return pItem;
		}

		pItem = INVENTORY_GetUnknownItem(pItem);
	}

	return NULL;
}

//D2Common.0x6FD90E20 (#10288)
int __stdcall INVENTORY_GetSetItemEquipCountByFileIndex(D2InventoryStrc* pInventory, int nItemFileIndex)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pItem = NULL;
	int nCounter = 0;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return 0;
	}

	pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
	
	if (pInventoryGrid)
	{
		pItem = pInventoryGrid->pItem;

		while (pItem && pItem->dwUnitType == UNIT_ITEM && pItem->pItemData)
		{
			if (ITEMS_GetItemQuality(pItem) == ITEMQUAL_SET && ITEMS_GetFileIndex(pItem) == nItemFileIndex)
			{
				if (pItem->pStaticPath->nXPos < 11)
				{
					++nCounter;
				}
			}

			pItem = pItem->pItemData->pExtraData.unk0x14;
		}

		return nCounter;
	}

	return 0;
}

//D2Common.0x6FD90ED0 (#10289)
void __stdcall INVENTORY_UpdateWeaponGUIDOnInsert(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	D2UnitStrc* pWeapon = NULL;

	if (pInventory && pItem && pItem->dwUnitType == UNIT_ITEM && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
	{
		if (ITEMS_GetBodyLocation(pItem) == BODYLOC_RARM || ITEMS_GetBodyLocation(pItem) == BODYLOC_LARM)
		{
			if (ITEMS_CanBeEquipped(pItem))
			{
				pWeapon = INVENTORY_GetLeftHandWeapon(pInventory);
				if (!pWeapon || !ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_WEAPON) || ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_MISSILE_POTION))
				{
					if (pInventory->dwSignature == 0x1020304)
					{
						pInventory->dwLeftItemGUID = pItem->dwUnitId;
					}
					return;
				}
			}
			
			if (pInventory->dwSignature == 0x1020304 && pInventory->dwLeftItemGUID == pItem->dwUnitId)
			{
				pInventory->dwLeftItemGUID = -1;
			}
		}
	}
}

//D2Common.0x6FD90F80 (#10290)
void __stdcall INVENTORY_UpdateWeaponGUIDOnRemoval(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pWeapon = NULL;
	int nBodyLoc = 0;

	if (pInventory && pInventory->dwSignature == 0x1020304 && pItem && pItem->dwUnitType == UNIT_ITEM && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
	{
		if (pItem->dwUnitType == UNIT_ITEM && pInventory->dwLeftItemGUID == pItem->dwUnitId)
		{
			pInventory->dwLeftItemGUID = -1;
		}

		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
		if (pInventoryGrid)
		{
			nBodyLoc = ITEMS_GetBodyLocation(pItem);
			if (nBodyLoc == BODYLOC_RARM)
			{
				pWeapon = pInventoryGrid->ppItems[BODYLOC_LARM];
			}
			else if (nBodyLoc == BODYLOC_LARM)
			{
				pWeapon = pInventoryGrid->ppItems[BODYLOC_RARM];
			}
			else
			{
				return;
			}

			if (pWeapon && ITEMS_CanBeEquipped(pWeapon) && pWeapon->dwUnitType == UNIT_ITEM)
			{
				pInventory->dwLeftItemGUID = pWeapon->dwUnitId;
			}
		}
	}
}

//D2Common.0x6FD91050 (#10291)
int __stdcall INVENTORY_GetWieldType(D2UnitStrc* pPlayer, D2InventoryStrc* pInventory)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pRightHandItem = NULL;
	D2UnitStrc* pLeftHandItem = NULL;

	if (pInventory)
	{
		if (pInventory->dwSignature == 0x1020304)
		{
			pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
			if (pInventoryGrid)
			{
				pLeftHandItem = pInventoryGrid->ppItems[BODYLOC_LARM];
				pRightHandItem = pInventoryGrid->ppItems[BODYLOC_RARM];
			}
		}

		if (!pLeftHandItem)
		{
			if (!pRightHandItem)
			{
				return -1;
			}

			if (ITEMS_CheckWeaponIfTwoHanded(pRightHandItem))
			{
				return 2;
			}

			if (ITEMS_Is1Or2Handed(pPlayer, pRightHandItem))
			{
				return 2;
			}
			else
			{
				return -1;
			}
		}

		if (!pRightHandItem)
		{
			if (ITEMS_CheckWeaponIfTwoHanded(pLeftHandItem))
			{
				return 2;
			}

			if (ITEMS_Is1Or2Handed(pPlayer, pLeftHandItem))
			{
				return 2;
			}
			else
			{
				return -1;
			}
		}

		if (ITEMS_Is1Or2Handed(pPlayer, pLeftHandItem) || ITEMS_Is1Or2Handed(pPlayer, pRightHandItem))
		{
			return 1;
		}

		if (ITEMS_CheckWeaponIfTwoHanded(pLeftHandItem))
		{
			return 2;
		}

		if (ITEMS_CheckWeaponIfTwoHanded(pRightHandItem))
		{
			return 2;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

//D2Common.0x6FD91140 (#10292)
void __stdcall INVENTORY_SetOwnerId(D2InventoryStrc* pInventory, int nOwnerId)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		pInventory->dwOwnerId = nOwnerId;
	}
}

//D2Common.0x6FD91160 (#10293)
int __stdcall INVENTORY_GetOwnerId(D2InventoryStrc* pInventory)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		return pInventory->dwOwnerId;
	}

	return -1;
}

//D2Common.0x6FD91190 (#10294)
void __stdcall INVENTORY_CreateCorpseForPlayer(D2InventoryStrc* pInventory, int nUnitId, int a3, int a4)
{
	D2CorpseStrc* pCorpse = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		pCorpse = (D2CorpseStrc*)FOG_AllocServerMemory(pInventory->pMemPool, sizeof(D2CorpseStrc), __FILE__, __LINE__, 0);
		pCorpse->unk0x00 = a4;
		pCorpse->dwUnitId = nUnitId;
		pCorpse->unk0x08 = a3;
		pCorpse->pNextCorpse = NULL;

		if (pInventory->pFirstCorpse)
		{
			pInventory->pLastCorpse->pNextCorpse = pCorpse;
		}
		else
		{
			pInventory->pFirstCorpse = pCorpse;
		}

		pInventory->pLastCorpse = pCorpse;

		if (pCorpse->unk0x00)
		{
			++pInventory->nCorpseCount;
		}
	}
}

//D2Common.0x6FD91210 (#10295)
BOOL __stdcall INVENTORY_FreeCorpse(D2InventoryStrc* pInventory, int nUnitId, int a3)
{
	D2CorpseStrc* pPreviousCorpse = NULL;
	D2CorpseStrc* pCorpse = NULL;

	if (pInventory && pInventory->dwSignature == 0x1020304 && pInventory->pFirstCorpse)
	{
		pCorpse = pInventory->pFirstCorpse;
		while (pCorpse->dwUnitId != nUnitId || pCorpse->unk0x00 != a3)
		{
			pPreviousCorpse = pCorpse;

			pCorpse = pCorpse->pNextCorpse;
			if (!pCorpse)
			{
				return FALSE;
			}
		}

		if (pPreviousCorpse)
		{
			pPreviousCorpse->pNextCorpse = pCorpse->pNextCorpse;
			if (!pCorpse->pNextCorpse)
			{
				pInventory->pLastCorpse = pPreviousCorpse;
			}
		}
		else
		{
			pInventory->pFirstCorpse = pCorpse->pNextCorpse;
			if (!pCorpse->pNextCorpse)
			{
				pInventory->pLastCorpse = NULL;
			}
		}

		if (pCorpse->unk0x00)
		{
			--pInventory->nCorpseCount;
		}

		FOG_FreeServerMemory(pInventory->pMemPool, pCorpse, __FILE__, __LINE__, 0);
		return TRUE;
	}
	
	return FALSE;
}

//D2Common.0x6FD91290 (#10296)
D2CorpseStrc* __stdcall INVENTORY_GetFirstCorpse(D2InventoryStrc* pInventory)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		return pInventory->pFirstCorpse;
	}

	return NULL;
}

//D2Common.0x6FD912B0 (#10297)
int __stdcall INVENTORY_GetCorpseCount(D2InventoryStrc* pInventory)
{
	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		return pInventory->nCorpseCount;
	}

	return 0;
}

//D2Common.0x6FD912D0 (#10313)
D2CorpseStrc* __stdcall INVENTORY_GetNextCorpse(D2CorpseStrc* pCorpse)
{
	if (pCorpse)
	{
		return pCorpse->pNextCorpse;
	}

	REMOVE_LATER_WriteToLogFile("INVENTORY_GetNextCorpse: NULL pointer");
	return NULL;
}

//D2Common.0x6FDAFEA0 (#10314)
int __stdcall INVENTORY_GetUnitGUIDFromCorpse(D2CorpseStrc* pCorpse)
{
	if (pCorpse)
	{
		return pCorpse->dwUnitId;
	}

	REMOVE_LATER_WriteToLogFile("INVENTORY_GetUnitGUIDFromCorpse: NULL pointer");
	return 0;
}

//D2Common.0x6FDB18D0 (#10315)
//TODO: Find a name
int __stdcall D2Common_10315(D2CorpseStrc* pCorpse)
{
	if (pCorpse)
	{
		return pCorpse->unk0x08; // Used as merc name id?
	}

	return 0;
}

//D2Common.0x6FD912F0 (#10298)
void __stdcall INVENTORY_GetItemSaveGfxInfo(D2UnitStrc* pPlayer, uint8_t* a2, uint8_t* pColor)
{
	D2ArmTypeTxt* pArmTypeTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2UnitStrc* pRightHandItem = NULL;
	D2UnitStrc* pLeftHandItem = NULL;
	D2UnitStrc* pItem = NULL;
	uint8_t* pComponentColor = NULL;
	int nCounter = 0;
	uint8_t nComponent = 0;
	uint8_t nBodyLoc = 0;
	uint8_t pArmorComponents[6] = {};

	pItem = INVENTORY_GetFirstItem(pPlayer->pInventory);
	while (pItem)
	{
		if (pItem->dwUnitType == UNIT_ITEM && pItem->dwAnimMode == IMODE_EQUIP)
		{
			pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
			D2_ASSERT(pItemsTxtRecord);

			nBodyLoc = ITEMS_GetBodyLocation(pItem);
			if (nBodyLoc == BODYLOC_HEAD || nBodyLoc == BODYLOC_RARM || nBodyLoc == BODYLOC_LARM)
			{
				if (!ITEMS_CheckType(pItemsTxtRecord->wType[0], ITEMTYPE_CIRCLET) && pPlayer->pInventory)
				{
					pRightHandItem = INVENTORY_GetCompositItem(pPlayer->pInventory, COMPOSIT_RIGHTHAND);
					pLeftHandItem = INVENTORY_GetCompositItem(pPlayer->pInventory, COMPOSIT_LEFTHAND);

					nComponent = ITEMS_GetComponent(pItem);

					sub_6FD915C0();

					nCounter = 1;

					while (nCounter < 255)
					{
						if (stru_6FDEA708[nCounter].dwCode == pItemsTxtRecord->dwAlternateGfx)
						{
							break;
						}

						if (stru_6FDEA708[nCounter].dwCode == pItemsTxtRecord->dwCode)
						{
							break;
						}

						++nCounter;
					}

					if (nCounter <= 0 || nCounter >= 255)
					{
						if (nComponent < 16)
						{
							a2[nComponent] = -1;
							pColor[nComponent] = -1;
						}
					}
					else
					{
						if (pItem == pRightHandItem)
						{
							nComponent = COMPOSIT_RIGHTHAND;
						}
						else if (pItem == pLeftHandItem)
						{
							nComponent = COMPOSIT_LEFTHAND;
						}
						else if (nComponent < 16)
						{
							a2[nComponent] = nCounter;

							sub_6FD917B0(pPlayer, a2, pColor, pItem);

							pComponentColor = &pColor[nComponent];
							if (ITEMS_GetColor(pPlayer, pItem, pComponentColor, 0))
							{
								++*pComponentColor;
							}
							else
							{
								*pComponentColor = -1;
							}
						}
					}
				}
			}
			else if (nBodyLoc == BODYLOC_TORSO)
			{
				for(int i = 0; i < 6; ++i)
				{
					pArmorComponents[i] = pItemsTxtRecord->nArmorComp[i];
				}

				for (int i = 0; i < 16; ++i)
				{
					if (COMPOSIT_IsArmorComponent(i))
					{
						pArmTypeTxtRecord = DATATBLS_GetArmTypeTxtRecord(COMPOSIT_GetArmorTypeFromComponent(i, pArmorComponents));
						if (pItem->dwClassId > 0)
						{
							sub_6FD915C0();

							nCounter = 1;

							while (nCounter < 255)
							{
								if (stru_6FDEA708[nCounter].dwCode == *(uint32_t*)&pArmTypeTxtRecord->szToken[0])
								{
									break;
								}

								++nCounter;
							}

							if (nCounter && nCounter < 255)
							{
								if (i < 16)
								{
									a2[i] = nCounter;

									pComponentColor = &pColor[i];
									if (ITEMS_GetColor(pPlayer, pItem, pComponentColor, 0))
									{
										++*pComponentColor;
									}
									else
									{
										*pComponentColor = -1;
									}
								}
							}
							else
							{
								if (i < 16)
								{
									a2[i] = -1;
									pColor[i] = -1;
								}
							}
						}
						else
						{
							if (i < 16)
							{
								a2[i] = -1;
								pColor[i] = -1;
							}
						}
					}
				}
			}
		}

		pItem = INVENTORY_GetNextItem(pItem);
	}
}

//D2Common.0x6FD915C0
//TODO: Find a name
void __fastcall sub_6FD915C0()
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2ItemDataTbl* pItemDataTbl = NULL;
	BOOL bNeedsToBeAdded = FALSE;
	int nCurrentTableEntries = 0;
	int nCounter = 0;
	int dwCode = 0;
	int nIndex = 0;

	if (!dword_6FDEAF04)
	{
		dword_6FDEAF04 = TRUE;

		memset(stru_6FDEA708, 0x00, sizeof(stru_6FDEA708));

		stru_6FDEA708[1].dwCode = ' til';
		stru_6FDEA708[1].nItemType = ITEMTYPE_ARMOR;
		stru_6FDEA708[2].dwCode = ' dem';
		stru_6FDEA708[2].nItemType = ITEMTYPE_ARMOR;
		stru_6FDEA708[3].dwCode = ' yvh';
		stru_6FDEA708[3].nItemType = ITEMTYPE_ARMOR;

		pItemDataTbl = DATATBLS_GetItemDataTables();

		nCurrentTableEntries = 4;

		for (int i = 0; i < pItemDataTbl->nItemsTxtRecordCount; ++i)
		{
			pItemsTxtRecord = &pItemDataTbl->pItemsTxt[i];

			if (pItemsTxtRecord->dwAlternateGfx)
			{
				dwCode = pItemsTxtRecord->dwAlternateGfx;
			}
			else
			{
				dwCode = pItemsTxtRecord->dwCode;
			}

			if (ITEMS_CheckType(pItemsTxtRecord->wType[0], ITEMTYPE_WEAPON)
				|| ITEMS_CheckType(pItemsTxtRecord->wType[0], ITEMTYPE_ARMOR)
				|| ITEMS_CheckType(pItemsTxtRecord->wType[0], ITEMTYPE_ANY_SHIELD)
				|| ITEMS_CheckType(pItemsTxtRecord->wType[0], ITEMTYPE_HELM) && !ITEMS_CheckType(pItemsTxtRecord->wType[0], ITEMTYPE_CIRCLET))
			{
				bNeedsToBeAdded = TRUE;
			}
			else
			{
				bNeedsToBeAdded = FALSE;
			}

			nCounter = 0;
			while (nCounter < nCurrentTableEntries)
			{
				if (stru_6FDEA708[nCounter].dwCode == dwCode)
				{
					break;
				}

				++nCounter;
			}

			if (nCounter >= nCurrentTableEntries && bNeedsToBeAdded == 1)
			{
				nIndex = nCurrentTableEntries;

				while (ITEMS_CheckType(stru_6FDE2820[nIndex].nItemType, ITEMTYPE_WEAPON) && ITEMS_CheckType(pItemsTxtRecord->wType[0], ITEMTYPE_WEAPON)
					   || ITEMS_CheckType(stru_6FDE2820[nIndex].nItemType, ITEMTYPE_ANY_ARMOR) && ITEMS_CheckType(pItemsTxtRecord->wType[0], ITEMTYPE_ANY_ARMOR)
					   || stru_6FDEA708[nIndex].dwCode)
				{
					++nIndex;
				}

				if (nIndex >= ARRAY_SIZE(stru_6FDEA708))
				{
					nIndex = nCurrentTableEntries;
				}

				stru_6FDEA708[nIndex].dwCode = dwCode;
				stru_6FDEA708[nIndex].nItemType = pItemsTxtRecord->wType[0];

				if (nCurrentTableEntries == nIndex)
				{
					++nCurrentTableEntries;
				}
			}
		}

		dword_6FDEAF00 = ARRAY_SIZE(stru_6FDEA708);
	}
}

//D2Common.0x6FD917B0
//TODO: Find a name
void __fastcall sub_6FD917B0(D2UnitStrc* pUnit, uint8_t* a2, uint8_t* pColor, D2UnitStrc* pItem)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2UnitStrc* pCompositItem = NULL;
	uint32_t dwWeaponClassCode = 0;
	int nWeaponClass = 0;
	int nCounter = 0;
	int nItemId = 0;
	uint8_t nBodyLoc = 0;
	uint8_t nComponent = 0;

	D2_ASSERT(pUnit);
	D2_ASSERT(pItem);

	nBodyLoc = ITEMS_GetBodyLocation(pItem);
	if (nBodyLoc == BODYLOC_RARM || nBodyLoc == BODYLOC_LARM)
	{
		dwWeaponClassCode = COMPOSIT_GetWeaponClassId(pUnit, pUnit->pInventory, &nWeaponClass, -1, 1);

		nComponent = ITEMS_GetComponent(pItem);
		if (dwWeaponClassCode == ' wbx')
		{
			if (nComponent == COMPOSIT_RIGHTHAND || nComponent == COMPOSIT_LEFTHAND)
			{
				if (pUnit->pInventory)
				{
					pCompositItem = INVENTORY_GetCompositItem(pUnit->pInventory, COMPOSIT_RIGHTHAND);
					if (pCompositItem)
					{
						pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pCompositItem->dwClassId);
						D2_ASSERT(pItemsTxtRecord);

						sub_6FD915C0();

						nCounter = 1;

						while (nCounter < ARRAY_SIZE(stru_6FDEA708))
						{
							if (stru_6FDEA708[nCounter].dwCode == pItemsTxtRecord->dwAlternateGfx)
							{
								break;
							}

							if (stru_6FDEA708[nCounter].dwCode == pItemsTxtRecord->dwCode)
							{
								break;
							}

							++nCounter;
						}

						if (nCounter <= 0 || nCounter >= ARRAY_SIZE(stru_6FDEA708))
						{
							a2[6] = -1;
							pColor[6] = -1;
						}
						else
						{
							a2[6] = nCounter;
						}
					}
				}
			}
		}
		else if(dwWeaponClassCode == ' wob')
		{
			if (nComponent == COMPOSIT_LEFTHAND)
			{
				pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode('til\0', &nItemId);
				if (pItemsTxtRecord)
				{
					sub_6FD915C0();

					nCounter = 1;

					while (nCounter < ARRAY_SIZE(stru_6FDEA708))
					{
						if (stru_6FDEA708[nCounter].dwCode == pItemsTxtRecord->dwAlternateGfx)
						{
							break;
						}

						if (stru_6FDEA708[nCounter].dwCode == pItemsTxtRecord->dwCode)
						{
							break;
						}

						++nCounter;
					}

					if (nCounter <= 0 || nCounter >= ARRAY_SIZE(stru_6FDEA708))
					{
						a2[5] = -1;
						pColor[5] = -1;
					}
					else
					{
						a2[5] = nCounter;
					}
				}
			}
		}
	}
}

//D2Common.0x6FD91B60 (#10299)
//TODO: Find a name
int __stdcall D2Common_10299(D2UnitStrc* pUnit, int nBodyLoc, D2UnitStrc* pItem, BOOL bDontCheckReqs)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	D2UnitStrc* pInventoryItem = NULL;
	D2UnitStrc* pItem1 = NULL;
	D2UnitStrc* pItem2 = NULL;
	int nOtherBodyLoc = 0;

	if (pUnit && pUnit->pInventory)
	{
		if (!pItem || ITEMS_CheckBodyLocation(pItem, nBodyLoc) && (bDontCheckReqs || ITEMS_CheckRequirements(pItem, pUnit, TRUE, NULL, NULL, NULL)))
		{
			pInventoryGrid = INVENTORY_GetGrid(pUnit->pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);

			switch (nBodyLoc)
			{
			case BODYLOC_NONE:
				return 0;

			case BODYLOC_RARM:
			case BODYLOC_LARM:
				if (nBodyLoc != BODYLOC_LARM)
				{
					nOtherBodyLoc = BODYLOC_LARM;
				}
				else
				{
					nOtherBodyLoc = BODYLOC_RARM;
				}

				if (pUnit->pInventory->dwSignature == 0x1020304 && nBodyLoc >= 0 && nBodyLoc < 13 && pInventoryGrid)
				{
					pItem1 = pInventoryGrid->ppItems[nBodyLoc];
				}
				else
				{
					pItem1 = NULL;
				}

				if (pUnit->pInventory->dwSignature == 0x1020304 && nOtherBodyLoc >= 0 && nOtherBodyLoc < 13 && pInventoryGrid)
				{
					pItem2 = pInventoryGrid->ppItems[nOtherBodyLoc];
				}
				else
				{
					pItem2 = NULL;
				}

				return sub_6FD91D50(pUnit, nBodyLoc, nOtherBodyLoc, pItem, pItem1, pItem2, bDontCheckReqs);

			case BODYLOC_SWRARM:
			case BODYLOC_SWLARM:
				if (nBodyLoc != BODYLOC_SWLARM)
				{
					nOtherBodyLoc = BODYLOC_SWLARM;
				}
				else
				{
					nOtherBodyLoc = BODYLOC_SWRARM;
				}

				if (pUnit->pInventory->dwSignature == 0x1020304 && nBodyLoc >= 0 && nBodyLoc < 13 && pInventoryGrid)
				{
					pItem1 = pInventoryGrid->ppItems[nBodyLoc];
				}
				else
				{
					pItem1 = NULL;
				}

				if (pUnit->pInventory->dwSignature == 0x1020304 && nOtherBodyLoc >= 0 && nOtherBodyLoc < 13 && pInventoryGrid)
				{
					pItem2 = pInventoryGrid->ppItems[nOtherBodyLoc];
				}
				else
				{
					pItem2 = NULL;
				}

				return sub_6FD91D50(pUnit, nBodyLoc, nOtherBodyLoc, pItem, pItem1, pItem2, bDontCheckReqs);

			default:
				if (pUnit->pInventory->dwSignature == 0x1020304 && nBodyLoc >= 0 && nBodyLoc < 13 && pInventoryGrid)
				{
					pInventoryItem = pInventoryGrid->ppItems[nBodyLoc];
				}
				else
				{
					pInventoryItem = NULL;
				}

				if (pItem)
				{
					return pInventoryItem != NULL ? 5 : 1;
				}
				else
				{
					return pInventoryItem != NULL ? 3 : 0;
				}
			}
		}
	}

	return 0;
}

//D2Common.0x6FD91D50
//TODO: Find names for function and arguments
int __fastcall sub_6FD91D50(D2UnitStrc* pPlayer, int a2, int nBodyLoc, D2UnitStrc* a3, D2UnitStrc* a4, D2UnitStrc* pItem, int nUnused)
{
	D2InventoryGridStrc* pInventoryGrid = NULL;
	int nInventoryRecord = 0;

	if (!a3)
	{
		if (a4)
		{
			return 3;
		}

		if (pItem && ITEMS_CheckWeaponIfTwoHanded(pItem))
		{
			return 4;
		}

		return 0;
	}

	if (a4)
	{
		if (ITEMS_AreStackablesEqual(a4, a3))
		{
			return 6;
		}

		if (sub_6FD91E80(pPlayer, a3, pItem))
		{
			return 5;
		}

		nInventoryRecord = UNITS_GetInventoryRecordId(pPlayer, 0, 1);
		if (pPlayer->pInventory && pPlayer->pInventory->dwSignature == 0x1020304 && nBodyLoc >= 0 && nBodyLoc < 13)
		{
			pInventoryGrid = INVENTORY_GetGrid(pPlayer->pInventory, INVGRID_BODYLOC, &gBodyLocInventoryGridInfo);
			if (pInventoryGrid && pInventoryGrid->ppItems[nBodyLoc] && INVENTORY_GetFreePosition(pPlayer->pInventory, pInventoryGrid->ppItems[nBodyLoc], nInventoryRecord, (int*)&a4, (int*)&pItem, 0))
			{
				return 7;
			}
		}

		return 0;
	}

	if (pItem)
	{
		return 2 - (sub_6FD91E80(pPlayer, a3, pItem) != 0);
	}
	else
	{
		return 1;
	}
}

//D2Common.0x6FD91E80
//TODO: Find a name
BOOL __fastcall sub_6FD91E80(D2UnitStrc* pUnit, D2UnitStrc* pItem1, D2UnitStrc* pItem2)
{
	int nUnitId = 0;

	if (!pItem1 || !pItem2
		|| ITEMS_GetAmmoType(pItem1) && ITEMS_CheckItemTypeId(pItem2, ITEMS_GetAmmoType(pItem1))
		|| ITEMS_GetAmmoType(pItem2) && ITEMS_CheckItemTypeId(pItem1, ITEMS_GetAmmoType(pItem2)))
	{
		return TRUE;
	}

	if (ITEMS_GetQuiverType(pItem1) || ITEMS_GetQuiverType(pItem2))
	{
		return FALSE;
	}

	if (!ITEMS_CheckWeaponIfTwoHanded(pItem1) || ITEMS_Is1Or2Handed(pUnit, pItem1))
	{
		if (!ITEMS_CheckWeaponIfTwoHanded(pItem2) || ITEMS_Is1Or2Handed(pUnit, pItem2))
		{
			if (ITEMS_CheckItemTypeId(pItem1, ITEMTYPE_WEAPON) && ITEMS_CheckItemTypeId(pItem2, ITEMTYPE_WEAPON))
			{
				if (!pUnit)
				{
					return FALSE;
				}

				if (pUnit->dwUnitType == UNIT_MONSTER)
				{
					nUnitId = pUnit->dwClassId;
					if (nUnitId < 0 || nUnitId >= sgptDataTables->nMonStatsTxtRecordCount)
					{
						nUnitId = -1;
					}

					switch (nUnitId)
					{
					case MONSTER_SHADOWWARRIOR:
					case MONSTER_SHADOWMASTER:
						if (ITEMS_CheckItemTypeId(pItem1, ITEMTYPE_HAND_TO_HAND) && ITEMS_CheckItemTypeId(pItem2, ITEMTYPE_HAND_TO_HAND))
						{
							return TRUE;
						}
						return FALSE;

					case MONSTER_ANCIENTBARB1:
					case MONSTER_ANCIENTBARB2:
					case MONSTER_ANCIENTBARB3:
						return TRUE;

					default:
						return FALSE;
					}

					return FALSE;
				}

				if (pUnit->dwUnitType)
				{
					return FALSE;
				}

				if (pUnit->dwClassId == PCLASS_BARBARIAN)
				{
					return TRUE;
				}

				if (pUnit->dwClassId != PCLASS_ASSASSIN || !ITEMS_CheckItemTypeId(pItem1, ITEMTYPE_HAND_TO_HAND))
				{
					return FALSE;
				}

				if (!ITEMS_CheckItemTypeId(pItem2, ITEMTYPE_HAND_TO_HAND))
				{
					return FALSE;
				}
			}
			else
			{
				if (ITEMS_CheckItemTypeId(pItem1, ITEMTYPE_WEAPON))
				{
					return TRUE;
				}

				if (!ITEMS_CheckItemTypeId(pItem2, ITEMTYPE_WEAPON))
				{
					return FALSE;
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD92080 (#10304)
D2UnitStrc* __stdcall INVENTORY_GetNextItem(D2UnitStrc* pItem)
{
	D2ItemExtraDataStrc* pItemExtraData = INVENTORY_GetItemExtraDataFromItem(pItem);

	if (pItemExtraData)
	{
		return pItemExtraData->pNextItem;
	}

	return NULL;
}

//Inlined at various places
D2UnitStrc* __stdcall INVENTORY_GetUnknownItem(D2UnitStrc* pItem)
{
	D2ItemExtraDataStrc* pItemExtraData = INVENTORY_GetItemExtraDataFromItem(pItem);

	if (pItemExtraData)
	{
		return pItemExtraData->unk0x14;
	}

	return NULL;
}

//D2Common.0x6FD920C0 (#10305)
D2UnitStrc* __stdcall INVENTORY_UnitIsItem(D2UnitStrc* pItem)
{
	return pItem && pItem->dwUnitType == UNIT_ITEM ? pItem : NULL;
}

//D2Common.0x6FD920E0 (#10306)
int __stdcall INVENTORY_GetItemGUID(D2UnitStrc* pItem)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM)
	{
		return pItem->dwUnitId;
	}

	return 0;
}

//D2Common.0x6FD92100 (#10307)
int __stdcall INVENTORY_GetItemNodePage(D2UnitStrc* pItem)
{
	D2ItemExtraDataStrc* pItemExtraData = INVENTORY_GetItemExtraDataFromItem(pItem);

	if (pItemExtraData)
	{
		return pItemExtraData->nNodePosOther;
	}

	return 0;
}

//D2Common.0x6FD92140 (#10310)
D2UnitStrc* __stdcall INVENTORY_IsItemInInventory(D2InventoryStrc* pInventory, D2UnitStrc* pItem)
{
	D2ItemExtraDataStrc* pItemExtraData = NULL;

	if (pInventory)
	{
		pItemExtraData = INVENTORY_GetItemExtraDataFromItem(pItem);
		if (pItemExtraData && pInventory == pItemExtraData->pParentInv)
		{
			return pItem;
		}
	}

	return NULL;
}

//D2Common.0x6FDAFEA0 (#10311)
D2InventoryNodeStrc* __stdcall INVENTORY_GetNextNode(D2InventoryNodeStrc* pNode)
{
	if (pNode)
	{
		return pNode->pNext;
	}

	return NULL;
}

//D2Common.0x6FD90AB0 (#10312)
int __stdcall INVENTORY_GetItemGUIDFromNode(D2InventoryNodeStrc* pNode)
{
	if (pNode)
	{
		return pNode->nItemId;
	}

	return 0;
}

//D2Common.0x6FD92180 (#10300)
BOOL __stdcall INVENTORY_RemoveAllItems(D2InventoryStrc* pInventory)
{
	D2UnitStrc* pItem = NULL;

	if (!pInventory || pInventory->dwSignature != 0x1020304)
	{
		return FALSE;
	}

	pInventory->pCursorItem = NULL;
	pInventory->dwLeftItemGUID = -1;

	pItem = pInventory->pFirstItem;
	if (pItem)
	{
		while (INVENTORY_RemoveItem(pItem))
		{
			pItem = pInventory->pFirstItem;
			if (!pItem)
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	return TRUE;
}

//D2Common.0x6FD921D0 (#10302)
BOOL __stdcall INVENTORY_CanItemsBeTraded(void* pMemPool, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, BOOL* a4)
{
	D2InventoryStrc* pTradeInventory1 = NULL;
	D2InventoryStrc* pTradeInventory2 = NULL;
	D2UnitStrc* pItem = NULL;
	BOOL bResult = FALSE;
	
	pTradeInventory1 = INVENTORY_AllocInventory(pMemPool, NULL);
	pTradeInventory2 = INVENTORY_AllocInventory(pMemPool, NULL);

	if (pTradeInventory1 && pTradeInventory2 && INVENTORY_CopyUnitItemsToTradeInventory(pTradeInventory1, pPlayer1) && INVENTORY_CopyUnitItemsToTradeInventory(pTradeInventory2, pPlayer2))
	{
		if (pPlayer1->pInventory && pPlayer1->pInventory->dwSignature == 0x1020304 && pPlayer1->pInventory->pFirstItem)
		{
			pItem = pPlayer1->pInventory->pFirstItem;
			while (pItem)
			{
				if (ITEMS_GetStorePage(INVENTORY_UnitIsItem(pItem)) == INVPAGE_TRADE && !INVENTORY_CanItemBePlacedInInventory(pPlayer2, INVENTORY_UnitIsItem(pItem), pTradeInventory2))
				{
					if (a4)
					{
						*a4 = FALSE;
					}

					break;
				}

				pItem = INVENTORY_GetNextItem(pItem);
			}
		}

		if (!pItem)
		{
			if (pPlayer2->pInventory && pPlayer2->pInventory->dwSignature == 0x1020304 && pPlayer2->pInventory->pFirstItem)
			{
				pItem = pPlayer2->pInventory->pFirstItem;
				while (pItem)
				{
					if (ITEMS_GetStorePage(INVENTORY_UnitIsItem(pItem)) == INVPAGE_TRADE && !INVENTORY_CanItemBePlacedInInventory(pPlayer1, INVENTORY_UnitIsItem(pItem), pTradeInventory1))
					{
						if (a4)
						{
							*a4 = TRUE;
						}

						break;
					}

					pItem = INVENTORY_GetNextItem(pItem);
				}
			}

			if (!pItem)
			{
				bResult = TRUE;
			}
		}
	}

	INVENTORY_FreeInventory(pTradeInventory1);
	INVENTORY_FreeInventory(pTradeInventory2);

	return bResult;
}

//D2Common.0x6FD923C0
BOOL __fastcall INVENTORY_CopyUnitItemsToTradeInventory(D2InventoryStrc* pTradeInventory, D2UnitStrc* pUnit)
{
	D2InventoryGridInfoStrc pInventoryGridInfo = {};
	D2InventoryGridStrc* pTradeInventoryGrid = NULL;
	D2InventoryGridStrc* pUnitInventoryGrid = NULL;
	int nInventoryRecordId = 0;

	if (pTradeInventory && pTradeInventory->dwSignature == 0x1020304)
	{
		nInventoryRecordId = UNITS_GetInventoryRecordId(pUnit, INVPAGE_INVENTORY, TRUE);
		DATATBLS_GetInventoryGridInfo(nInventoryRecordId, 0, &pInventoryGridInfo);

		pTradeInventoryGrid = INVENTORY_GetGrid(pTradeInventory, INVGRID_INVENTORY, &pInventoryGridInfo);

		if (pTradeInventoryGrid && pUnit->pInventory && pUnit->pInventory->dwSignature == 0x1020304)
		{
			pUnitInventoryGrid = INVENTORY_GetGrid(pUnit->pInventory, INVGRID_INVENTORY, &pInventoryGridInfo);

			if (pUnitInventoryGrid && pUnitInventoryGrid->nGridWidth && pUnitInventoryGrid->nGridHeight
				&& pUnitInventoryGrid->nGridWidth == pTradeInventoryGrid->nGridWidth && pUnitInventoryGrid->nGridHeight == pTradeInventoryGrid->nGridHeight
				&& pUnitInventoryGrid->ppItems && pTradeInventoryGrid->ppItems)
			{
				memcpy(pTradeInventoryGrid->ppItems, pUnitInventoryGrid->ppItems, sizeof(D2UnitStrc*) * pUnitInventoryGrid->nGridWidth * pUnitInventoryGrid->nGridHeight);

				return TRUE;
			}
		}
	}
	
	return FALSE;
}

//D2Common.0x6FD92490
BOOL __fastcall INVENTORY_CanItemBePlacedInInventory(D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2InventoryStrc* pInventory)
{
	D2InventoryGridInfoStrc pInventoryGridInfo = {};
	D2InventoryGridStrc* pInventoryGrid = NULL;
	int nInventoryRecordId = 0;
	int nX = 0;
	int nY = 0;
	uint8_t nHeight = 0;
	uint8_t nWidth = 0;

	if (pInventory && pInventory->dwSignature == 0x1020304)
	{
		nInventoryRecordId = UNITS_GetInventoryRecordId(pPlayer, INVPAGE_INVENTORY, TRUE);
		DATATBLS_GetInventoryGridInfo(nInventoryRecordId, 0, &pInventoryGridInfo);

		pInventoryGrid = INVENTORY_GetGrid(pInventory, INVGRID_INVENTORY, &pInventoryGridInfo);

		if (pInventoryGrid && INVENTORY_GetFreePosition(pInventory, pItem, nInventoryRecordId, &nX, &nY, 0))
		{
			ITEMS_GetDimensions(pItem, &nWidth, &nHeight, __FILE__, __LINE__);

			if (nWidth && nHeight && nX >= 0 && nX + nWidth <= pInventoryGrid->nGridWidth && nY >= 0 && nY + nHeight <= pInventoryGrid->nGridHeight)
			{
				for (int j = nY; j < nY + nHeight; ++j)
				{
					for (int i = nX; i < nX + nWidth; ++i)
					{
						pInventoryGrid->ppItems[i + j * pInventoryGrid->nGridWidth] = (D2UnitStrc*)-1;//TODO: -1?!?
					}
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FD925E0
int __fastcall UNITS_GetXPosition(D2UnitStrc* pUnit)
{
	switch (pUnit->dwUnitType)
	{
	case UNIT_PLAYER:
	case UNIT_MONSTER:
	case UNIT_MISSILE:
		return pUnit->pDynamicPath ? pUnit->pDynamicPath->wPosX : 0;

	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		return pUnit->pStaticPath ? pUnit->pStaticPath->nXPos : 0;
	}

	return 0;
}

//D2Common.0x6FD92610
int __fastcall UNITS_GetYPosition(D2UnitStrc* pUnit)
{
	switch (pUnit->dwUnitType)
	{
	case UNIT_PLAYER:
	case UNIT_MONSTER:
	case UNIT_MISSILE:
		return pUnit->pDynamicPath ? pUnit->pDynamicPath->wPosY : 0;

	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_TILE:
		return pUnit->pStaticPath ? pUnit->pStaticPath->nYPos : 0;
	}

	return 0;
}
