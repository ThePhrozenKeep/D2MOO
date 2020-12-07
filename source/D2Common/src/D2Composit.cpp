#include "D2Composit.h"

#include "D2DataTbls.h"
#include "D2Inventory.h"
#include "D2Items.h"
#include "D2Math.h"
#include "D2Monsters.h"
#include "D2Skills.h"
#include "Units/Units.h"


//TODO: Find names


//D2Common.0x6FDCE260
static const D2CompositStrc stru_6FDCE260[] =
{
	{ ' sh1', WEAPONCLASS_1HS },
	{ ' th1', WEAPONCLASS_1HT },
	{ ' wob', WEAPONCLASS_BOW },
	{ ' sh2', WEAPONCLASS_2HS },
	{ ' th2', WEAPONCLASS_2HT },
	{ ' sj1', WEAPONCLASS_1JS },
	{ ' tj1', WEAPONCLASS_1JT },
	{ ' ss1', WEAPONCLASS_1SS },
	{ ' ts1', WEAPONCLASS_1ST },
	{ ' fts', WEAPONCLASS_STF },
	{ ' wbx', WEAPONCLASS_XBW },
	{ ' 1th', WEAPONCLASS_HT1 },
	{ ' 2th', WEAPONCLASS_HT2 },
};



//D2Common.0x6FD466C0 (#10884)
void __stdcall D2Common_10884_COMPOSIT_unk(D2UnitStrc* pUnit, int nClass, int nMode, int nUnitType, D2InventoryStrc* pInventory, char* szPath, int* pWeaponClassCode, BOOL bAddPathPrefix, int a9)
{
	static const D2CompositStrc stru_6FDD6088[] =
	{
		{ '  hg', PLRMODE_SEQUENCE },
		{ '  hg', PLRMODE_KNOCKBACK },
	};
	static const int dword_6FDD6098 = ARRAY_SIZE(stru_6FDD6088);

	static const D2CompositStrc stru_6FDD609C[] =
	{
		{'  hg', MONMODE_KNOCKBACK },
	};
	static const int dword_6FDD60A4 = ARRAY_SIZE(stru_6FDD609C);

	D2MonStats2Txt* pMonStats2TxtRecord = NULL;
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	char* szPathPrefix = NULL;
	DWORD dwClassToken = 0;
	DWORD dwModeToken = 0;
	int nWeaponClassId = 0;
	char szWeaponClass[4] = {};
	char szClass[4] = {};
	char szMode[4] = {};

	if (nUnitType == UNIT_PLAYER)
	{
		dwClassToken = *(DWORD*)&DATATBLS_GetPlrModeTypeTxtRecord(nClass, 0)->szToken[0];
		dwModeToken = *(DWORD*)&DATATBLS_GetPlrModeTypeTxtRecord(nMode, 1)->szToken[0];

		if (a9)
		{
			if (nMode && nMode != PLRMODE_DEAD)
			{
				*pWeaponClassCode = COMPOSIT_GetWeaponClassCode(pUnit, UNIT_PLAYER, nClass, nMode, pInventory, &nWeaponClassId);
			}
			else
			{
				*pWeaponClassCode = ' hth';
			}
		}

		for (int i = 0; i < dword_6FDD6098; ++i)
		{
			if (nMode == stru_6FDD6088[i].nWeaponClassId)
			{
				dwModeToken = stru_6FDD6088[i].nWeaponClassCode;
			}
		}

		if (nMode == PLRMODE_THROW)
		{
			if (*pWeaponClassCode != ' sh1' && *pWeaponClassCode != ' th1' && *pWeaponClassCode != ' sj1' && *pWeaponClassCode != ' tj1' && *pWeaponClassCode != ' ss1' && *pWeaponClassCode != ' ts1')
			{
				*pWeaponClassCode = ' hth';
			}
		}

		szPathPrefix = "DATA\\GLOBAL\\CHARS";
	}
	else if (nUnitType == UNIT_MONSTER)
	{
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nClass);
		if (pMonStatsTxtRecord)
		{
			dwClassToken = pMonStatsTxtRecord->dwCode;
			dwModeToken = DATATBLS_GetMonModeTxtRecord(nMode, 1)->dwToken;

			if (nMode && nMode != MONMODE_DEAD)
			{
				*pWeaponClassCode = COMPOSIT_GetWeaponClassCode(pUnit, UNIT_MONSTER, nClass, nMode, pInventory, &nWeaponClassId);
			}
			else
			{
				pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecord(pMonStatsTxtRecord->wMonStatsEx);
				if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_COMPOSITEDEATH])
				{
					*pWeaponClassCode = COMPOSIT_GetWeaponClassCode(pUnit, UNIT_MONSTER, nClass, nMode, pInventory, &nWeaponClassId);
				}
				else
				{
					*pWeaponClassCode = ' hth';
				}
			}

			for (int i = 0; i < dword_6FDD60A4; ++i)
			{
				if (nMode == stru_6FDD609C[i].nWeaponClassId)
				{
					dwModeToken = stru_6FDD609C[i].nWeaponClassCode;
				}
			}

			szPathPrefix = "DATA\\GLOBAL\\MONSTERS";
		}
	}
	else if (nUnitType == UNIT_OBJECT)
	{
		dwClassToken = *(DWORD*)&DATATBLS_GetObjModeTypeTxtRecord(nClass, 0)->szToken[0];
		dwModeToken = *(DWORD*)&DATATBLS_GetObjModeTypeTxtRecord(nMode, 1)->szToken[0];

		*pWeaponClassCode = COMPOSIT_GetWeaponClassCode(pUnit, UNIT_OBJECT, nClass, nMode, pInventory, &nWeaponClassId);

		szPathPrefix = "DATA\\GLOBAL\\OBJECTS";
	}
	else
	{
		return;
	}

	szWeaponClass[0] = ((char*)pWeaponClassCode)[0];
	szWeaponClass[0] &= ((szWeaponClass[0] == ' ') - 1);
	szWeaponClass[1] = ((char*)pWeaponClassCode)[1];
	szWeaponClass[1] &= ((szWeaponClass[1] == ' ') - 1);
	szWeaponClass[2] = ((char*)pWeaponClassCode)[2];
	szWeaponClass[2] &= ((szWeaponClass[2] == ' ') - 1);
	szWeaponClass[3] = 0;

	szClass[0] = ((char*)&dwClassToken)[0];
	szClass[0] &= ((szClass[0] == ' ') - 1);
	szClass[1] = ((char*)&dwClassToken)[1];
	szClass[1] &= ((szClass[1] == ' ') - 1);
	szClass[2] = ((char*)&dwClassToken)[2];
	szClass[2] &= ((szClass[2] == ' ') - 1);
	szClass[3] = 0;

	szMode[0] = ((char*)&dwModeToken)[0];
	szMode[0] &= ((szMode[0] == ' ') - 1);
	szMode[1] = ((char*)&dwModeToken)[1];
	szMode[1] &= ((szMode[1] == ' ') - 1);
	szMode[2] = ((char*)&dwModeToken)[2];
	szMode[2] &= ((szMode[2] == ' ') - 1);
	szMode[3] = 0;

	if (bAddPathPrefix)
	{
		wsprintfA(szPath, "%s\\%s\\COF\\%s%s%s.COF", szPathPrefix, szClass, szClass, szMode, szWeaponClass);
	}
	else
	{
		wsprintfA(szPath, "%s%s%s", szClass, szMode, szWeaponClass);
	}
}

//D2Common.0x6FD46BC0 (#10885)
void __stdcall D2Common_10885_COMPOSIT_unk(D2UnitStrc* pUnit, char* szPath, int* pWeaponClassCode, BOOL bAddPathPrefix, int a5, D2InventoryStrc* pInventory, int nAnimMode)
{
	int nClass = 0;
	int nType = 0;
	int nMode = 0;

	D2_ASSERT(pUnit);

	nClass = pUnit->dwClassId;
	nType = pUnit->dwUnitType;
	nMode = UNITS_GetAnimOrSeqMode(pUnit);

	D2COMMON_11013_ConvertMode(pUnit, &nType, &nClass, &nMode, __FILE__, __LINE__);

	if (nAnimMode != -1)
	{
		nMode = nAnimMode;
	}

	D2Common_10884_COMPOSIT_unk(pUnit, nClass, nMode, nType, pInventory, szPath, pWeaponClassCode, bAddPathPrefix, a5);
}

//D2Common.0x6FD46C60 (#10886)
int __stdcall COMPOSIT_GetWeaponClassIdFromCode(int nWeaponClassCode)
{
	for (int i = 0; i < ARRAY_SIZE(stru_6FDCE260); ++i)
	{
		if (stru_6FDCE260[i].nWeaponClassCode == nWeaponClassCode)
		{
			return stru_6FDCE260[i].nWeaponClassId;
		}
	}

	return 0;
}

//D2Common.0x6FD46C90 (#10887)
int __stdcall COMPOSIT_GetWeaponClassCode(D2UnitStrc* pUnit, int nUnitType, int nClass, int nMode, D2InventoryStrc* pInventory, int* pWeaponClassId)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;
	D2CharStatsTxt* pCharStatsTxtRecord = NULL;
	D2UnitStrc* pSecondaryWeapon = NULL;
	D2UnitStrc* pPrimaryWeapon = NULL;
	D2UnitStrc* pRightHandItem = NULL;
	D2UnitStrc* pLeftHandItem = NULL;
	D2UnitStrc* pItem = NULL;
	int nSecondaryWeaponClassId = 0;
	int nPrimaryWeaponClassId = 0;
	int nWeaponClassCode = 0;
	BYTE nComposit = 0;

	if (nUnitType == UNIT_PLAYER)
	{
		pItem = 0;
		if (pInventory)
		{
			pItem = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
			if (pItem && ITEMS_CanBeEquipped(pItem))
			{
				if (pItem->dwUnitType != UNIT_ITEM)
				{
					return 0;
				}

				nComposit = ITEMS_GetComponent(pItem);
			}

			if (nComposit != COMPOSIT_RIGHTHAND && nComposit != COMPOSIT_LEFTHAND)
			{
				pItem = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);
				if (pItem)
				{
					if (ITEMS_CanBeEquipped(pItem))
					{
						nComposit = ITEMS_GetComponent(pItem);
					}
					else
					{
						pItem = NULL;
					}
				}
			}
		}

		pCharStatsTxtRecord = UNITS_GetCharStatsTxtRecord(nClass);

		if (nMode && nMode != PLRMODE_DEAD && pItem && (nComposit == COMPOSIT_RIGHTHAND || nComposit == COMPOSIT_LEFTHAND))
		{
			pRightHandItem = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
			if (!ITEMS_CanBeEquipped(pRightHandItem))
			{
				pRightHandItem = NULL;
			}

			pLeftHandItem = INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);
			if (!ITEMS_CanBeEquipped(pLeftHandItem))
			{
				pLeftHandItem = NULL;
			}

			if (nClass == PCLASS_BARBARIAN)
			{
				if (ITEMS_CheckItemTypeId(pRightHandItem, ITEMTYPE_WEAPON) && ITEMS_CheckItemTypeId(pLeftHandItem, ITEMTYPE_WEAPON))
				{
					pPrimaryWeapon = INVENTORY_GetLeftHandWeapon(pInventory);
					if (!pPrimaryWeapon)
					{
						INVENTORY_UpdateWeaponGUIDOnInsert(pInventory, pRightHandItem);
						pPrimaryWeapon = pRightHandItem;
					}

					nPrimaryWeaponClassId = ITEMS_GetWeaponClassId(pPrimaryWeapon);
					if (pRightHandItem == pPrimaryWeapon)
					{
						pSecondaryWeapon = pLeftHandItem;
					}
					else
					{
						pSecondaryWeapon = pRightHandItem;
					}

					nSecondaryWeaponClassId = ITEMS_GetWeaponClassId(pSecondaryWeapon);
					if (nPrimaryWeaponClassId == WEAPONCLASS_1HS)
					{
						if (nSecondaryWeaponClassId == WEAPONCLASS_1HS)
						{
							*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(' ss1');
							return ' ss1';
						}
						else if (nSecondaryWeaponClassId == WEAPONCLASS_1HT)
						{
							*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(' sj1');
							return ' sj1';
						}
					}
					else if (nPrimaryWeaponClassId == WEAPONCLASS_1HT)
					{
						if (nSecondaryWeaponClassId == WEAPONCLASS_1HT)
						{
							*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(' tj1');
							return ' tj1';
						}
						else if (nSecondaryWeaponClassId == WEAPONCLASS_1HS)
						{
							*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(' ts1');
							return ' ts1';
						}
					}

					*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(' ss1');
					return ' ss1';
				}
			}
			else if (nClass == PCLASS_ASSASSIN)
			{
				if (ITEMS_CheckItemTypeId(pRightHandItem, ITEMTYPE_WEAPON) && ITEMS_CheckItemTypeId(pLeftHandItem, ITEMTYPE_WEAPON))
				{
					*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(' 2th');
					return ' 2th';
				}
			}

			if (INVENTORY_GetWieldType(pUnit, pInventory) == 2)
			{
				nWeaponClassCode = ITEMS_Get2HandWeaponClassCode(pItem);
				*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(nWeaponClassCode);
				return nWeaponClassCode;
			}
			else
			{
				nWeaponClassCode = ITEMS_GetWeaponClassCode(pItem);
				*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(nWeaponClassCode);
				return nWeaponClassCode;
			}
		}

		*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(pCharStatsTxtRecord->dwBaseWClass);
		return pCharStatsTxtRecord->dwBaseWClass;
	}
	else if (nUnitType == UNIT_MONSTER)
	{
		pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(nClass);

		if (!pMonStats2TxtRecord)
		{
			*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(' hth');
			return ' hth';
		}

		if ((nMode == MONMODE_DEATH || nMode == MONMODE_DEAD) && !(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_COMPOSITEDEATH]))
		{
			*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(' hth');
			return ' hth';
		}

		*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(pMonStats2TxtRecord->nBaseW);
		return pMonStats2TxtRecord->nBaseW;
	}
	else if (nUnitType == UNIT_OBJECT)
	{
		*pWeaponClassId = COMPOSIT_GetWeaponClassIdFromCode(' hth');
		return ' hth';
	}
	
	*pWeaponClassId = 0;
	return 0;
}

//D2Common.0x6FD47150 (#10888)
int __stdcall COMPOSIT_GetWeaponClassId(D2UnitStrc* pUnit, D2InventoryStrc* pInventory, int* pWeaponClassId, int nAnimMode, BOOL a5)
{
	int nClass = 0;
	int nType = 0;
	int nMode = 0;

	if (pUnit)
	{
		nClass = pUnit->dwClassId;
		nType = pUnit->dwUnitType;
		nMode = nAnimMode;

		if (nAnimMode == -1)
		{
			nMode = UNITS_GetAnimOrSeqMode(pUnit);
			nAnimMode = nMode;
		}

		if (pUnit->dwFlagEx & UNITFLAGEX_ISSHAPESHIFTED)
		{
			D2COMMON_11013_ConvertMode(pUnit, &nType, &nClass, &nAnimMode, __FILE__, __LINE__);

			if (a5)
			{
				nMode = nAnimMode;
			}
		}

		return COMPOSIT_GetWeaponClassCode(pUnit, nType, nClass, nMode, pInventory, pWeaponClassId);
	}
	else
	{
		return ' hth';
	}
}

//D2Common.0x6FD47200 (#10889)
BOOL __stdcall COMPOSIT_IsArmorComponent(int nComponent)
{
	return nComponent == COMPOSIT_TORSO || nComponent == COMPOSIT_LEGS || nComponent == COMPOSIT_RIGHTARM || nComponent == COMPOSIT_LEFTARM || nComponent == COMPOSIT_SPECIAL1 || nComponent == COMPOSIT_SPECIAL2;
}

//D2Common.0x6FD47230 (#10890)
BOOL __stdcall COMPOSIT_IsWeaponBowOrXBow(D2UnitStrc* pUnit)
{
	int nWeaponClassId = 0;
	int nClass = 0;
	int nType = 0;
	int nMode = 0;

	nType = pUnit->dwUnitType;
	nClass = pUnit->dwClassId;
	nMode = UNITS_GetAnimOrSeqMode(pUnit);

	if (pUnit->dwFlagEx & UNITFLAGEX_ISSHAPESHIFTED)
	{
		D2COMMON_11013_ConvertMode(pUnit, &nType, &nClass, &nMode, __FILE__, __LINE__);
	}

	COMPOSIT_GetWeaponClassCode(pUnit, nType, nClass, nMode, pUnit->pInventory, &nWeaponClassId);

	return nWeaponClassId == WEAPONCLASS_BOW || nWeaponClassId == WEAPONCLASS_XBW;
}

//D2Common.0x6FD472E0 (#10891)
BYTE __stdcall COMPOSIT_GetArmorTypeFromComponent(int nComponent, BYTE* pArmorComponents)
{
	D2_ASSERT(nComponent < NUM_COMPONENTS);

	switch (nComponent)
	{
	case COMPOSIT_TORSO:
		return pArmorComponents[2];

	case COMPOSIT_LEGS:
		return pArmorComponents[3];

	case COMPOSIT_RIGHTARM:
		return pArmorComponents[0];

	case COMPOSIT_LEFTARM:
		return pArmorComponents[1];

	case COMPOSIT_SPECIAL1:
		return pArmorComponents[4];

	case COMPOSIT_SPECIAL2:
		return pArmorComponents[5];

	default:
		FOG_10024_PacketAssertion(0, __FILE__, __LINE__);
		exit(-1);
	}
}
