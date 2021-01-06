#include "D2StatList.h"

#include <D2BitManip.h>
#include "D2DataTbls.h"
#include "D2Environment.h"
#include "D2ItemMods.h"
#include "D2Items.h"
#include "D2States.h"
#include "Units/Units.h"
//TODO: Find names


//D2Common.0x6FDB57C0 (#10563)
BOOL __stdcall STATLIST_AreUnitsAligned(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2)
{
	int nAlignment1 = 0;
	int nAlignment2 = 0;

	if (pUnit1 && pUnit2)
	{
		if (pUnit1 == pUnit2)
		{
			return TRUE;
		}

		nAlignment1 = STATLIST_GetUnitAlignment(pUnit1);
		nAlignment2 = STATLIST_GetUnitAlignment(pUnit2);

		if (nAlignment1 == 0 && nAlignment2 == 0)
		{
			return TRUE;
		}
		else if (nAlignment1 == 2 && nAlignment2 == 2)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FDB5830
int __fastcall sub_6FDB5830(D2StatListExStrc* pStatListEx, int nLayer_StatId)
{
	D2ItemStatCostTxt* pOpBaseItemStatCostTxtRecord = NULL;
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2CharStatsTxt* pCharStatsTxtRecord = NULL;
	D2StatExStrc* pStatEx = NULL;
	int nRecursiveValue = 0;
	int nBaseValue = 0;
	int nOldValue = 0;
	int nBaseTime = 0;
	int nPeriodOfDay = 0;
	int nStatId = 0;
	int nOpBase = 0;
	int nIndex = 0;
	int nValue = 0;
	int nTemp = 0;
	int nTmp = 0;

	nStatId = (nLayer_StatId >> 16) & 0xFFFF;

	pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}

	nValue = sub_6FDB6340(pStatListEx, nLayer_StatId, 0);

	if (pStatListEx->dwFlags & 0x80000000)
	{
		for (D2StatListStrc* i = pStatListEx->pMyLastList; i; i = i->pPrevLink)
		{
			if (!(pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED]) || !(i->dwFlags & 0x40000000))
			{
				nValue += sub_6FDB63E0((D2StatListExStrc*)i, nLayer_StatId, 0);
			}
		}
	}

	if (!pItemStatCostTxtRecord->unk0x51[1])
	{
		return nValue;
	}

	nOldValue = nValue;

	for (int i = 0; i < ARRAY_SIZE(pItemStatCostTxtRecord->pOpStatData); ++i)
	{
		switch (pItemStatCostTxtRecord->pOpStatData[i].nOp)
		{
		case 0:
			return nValue;

		case 1:
			if (pStatListEx->pOwner)
			{
				if (pStatListEx->pOwner->dwUnitType == UNIT_ITEM)
				{
					if (pStatListEx->pOwner->pStatListEx)
					{
						nOldValue = sub_6FDB6340(pStatListEx->pOwner->pStatListEx, nStatId << 16, pItemStatCostTxtRecord);
					}
					else
					{
						nOldValue = 0;
					}
				}

				if (nOldValue)
				{
					nTemp = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);
					if (nTemp)
					{
						nValue += nOldValue * nTemp / 100;
					}
				}
			}
			break;

		case 2:
			if (pStatListEx->dwOwnerType == UNIT_PLAYER || pStatListEx->dwOwnerType == UNIT_MONSTER)
			{
				nOpBase = pItemStatCostTxtRecord->pOpStatData[i].nOpBase;

				if (nOpBase != -1)
				{
					pOpBaseItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpBase);
					if (pOpBaseItemStatCostTxtRecord)
					{
						if ((pStatListEx->dwFlags & 0x80000000) != 0)
						{
							pStatEx = &pStatListEx->FullStats;
						}
						else
						{
							pStatEx = &pStatListEx->Stats;
						}

						nIndex = sub_6FDB6300(pStatEx, nOpBase << 16);

						if (nIndex >= 0 && &pStatEx->pStat[nIndex])
						{
							nTmp = pStatEx->pStat[nIndex].nValue >> pOpBaseItemStatCostTxtRecord->nValShift;
							if (nTmp > 0)
							{
								nRecursiveValue = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);
								if (nRecursiveValue)
								{
									nValue += (nTmp * nRecursiveValue) >> pItemStatCostTxtRecord->pOpStatData[i].nOpParam;
								}
							}
						}
					}
				}
			}
			break;

		case 3:
			if (pStatListEx->dwOwnerType == UNIT_PLAYER || pStatListEx->dwOwnerType == UNIT_MONSTER)
			{
				nOpBase = pItemStatCostTxtRecord->pOpStatData[i].nOpBase;

				if (nOpBase != -1)
				{
					pOpBaseItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpBase);
					if (pOpBaseItemStatCostTxtRecord)
					{
						if (pStatListEx->dwFlags & 0x80000000)
						{
							pStatEx = &pStatListEx->FullStats;
						}
						else
						{
							pStatEx = &pStatListEx->Stats;
						}

						nIndex = sub_6FDB6300(pStatEx, nOpBase << 16);

						if (nIndex >= 0 && &pStatEx->pStat[nIndex])
						{
							nTmp = pStatEx->pStat[nIndex].nValue >> pOpBaseItemStatCostTxtRecord->nValShift;
							if (nTmp > 0)
							{
								nRecursiveValue = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);
								if (nRecursiveValue)
								{
									nTemp = (nTmp * nRecursiveValue) >> pItemStatCostTxtRecord->pOpStatData[i].nOpParam;
									nValue += nTemp * nOldValue / 100;
								}
							}
						}
					}
				}
			}
			break;

		case 4:
			if (pStatListEx->pUnit && (pStatListEx->pUnit->dwUnitType == UNIT_PLAYER || pStatListEx->pUnit->dwUnitType == UNIT_MONSTER))
			{
				nOpBase = pItemStatCostTxtRecord->pOpStatData[i].nOpBase;

				if (nOpBase != -1)
				{
					pOpBaseItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpBase);
					if (pOpBaseItemStatCostTxtRecord && pStatListEx->pUnit->pStatListEx)
					{
						nTmp = sub_6FDB63E0(pStatListEx->pUnit->pStatListEx, nOpBase << 16, pOpBaseItemStatCostTxtRecord) >> pOpBaseItemStatCostTxtRecord->nValShift;
						if (nTmp > 0)
						{
							nRecursiveValue = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);
							if (nRecursiveValue)
							{
								nValue += (nTmp * nRecursiveValue) >> pItemStatCostTxtRecord->pOpStatData[i].nOpParam;
							}
						}
					}
				}
			}
			break;

		case 5:
			if (pStatListEx->pUnit && (pStatListEx->pUnit->dwUnitType == UNIT_PLAYER || pStatListEx->pUnit->dwUnitType == UNIT_MONSTER))
			{
				nOpBase = pItemStatCostTxtRecord->pOpStatData[i].nOpBase;

				if (nOpBase != -1)
				{
					pOpBaseItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpBase);
					if (pOpBaseItemStatCostTxtRecord && pStatListEx->pUnit->pStatListEx)
					{
						nTmp = sub_6FDB63E0(pStatListEx->pUnit->pStatListEx, nOpBase << 16, pOpBaseItemStatCostTxtRecord) >> pOpBaseItemStatCostTxtRecord->nValShift;
						if (nTmp > 0)
						{
							nRecursiveValue = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);
							if (nRecursiveValue)
							{
								nTemp = (nTmp * nRecursiveValue) >> pItemStatCostTxtRecord->pOpStatData[i].nOpParam;
								nValue += nOldValue * nTemp / 100;
							}
						}
					}
				}
			}
			break;

		case 6:
			if (pStatListEx->pOwner && pStatListEx->pOwner->pDrlgAct)
			{
				nTmp = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);
				if (nTmp)
				{
					nBaseTime = 0;
					nPeriodOfDay = ENVIRONMENT_GetPeriodOfDayFromAct(pStatListEx->pOwner->pDrlgAct, &nBaseTime);
					nValue += D2COMMON_10843_GetByTimeAdjustment(nTmp, nPeriodOfDay, nBaseTime, NULL, NULL, NULL);
				}
			}
			break;

		case 7:
			if (pStatListEx->pOwner && pStatListEx->pOwner->pDrlgAct)
			{
				nTmp = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);
				if (nTmp)
				{
					nBaseTime = 0;
					nPeriodOfDay = ENVIRONMENT_GetPeriodOfDayFromAct(pStatListEx->pOwner->pDrlgAct, &nBaseTime);
					nValue += nValue * D2COMMON_10843_GetByTimeAdjustment(nTmp, nPeriodOfDay, nBaseTime, NULL, NULL, NULL) / 100;
				}
			}
			break;

		case 8:
			if (pStatListEx->pOwner && pStatListEx->pOwner->dwUnitType == UNIT_PLAYER)
			{
				pCharStatsTxtRecord = UNITS_GetCharStatsTxtRecord(pStatListEx->pOwner->dwClassId);
				if (pCharStatsTxtRecord)
				{
					nIndex = sub_6FDB6300(&pStatListEx->Stats, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);

					if (nIndex >= 0 && &pStatListEx->Stats.pStat[nIndex])
					{
						nTemp = pStatListEx->Stats.pStat[nIndex].nValue;
					}
					else
					{
						nTemp = 0;
					}

					nBaseValue = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16) - nTemp;
					if (nBaseValue)
					{
						nValue += nBaseValue * pCharStatsTxtRecord->nManaPerMagic << 6;
					}
				}
			}
			break;

		case 9:
			if (pStatListEx->pOwner && pStatListEx->pOwner->dwUnitType == UNIT_PLAYER)
			{
				pCharStatsTxtRecord = UNITS_GetCharStatsTxtRecord(pStatListEx->pOwner->dwClassId);
				if (pCharStatsTxtRecord)
				{
					nIndex = sub_6FDB6300(&pStatListEx->Stats, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);

					if (nIndex >= 0 && &pStatListEx->Stats.pStat[nIndex])
					{
						nTemp = pStatListEx->Stats.pStat[nIndex].nValue;
					}
					else
					{
						nTemp = 0;
					}

					nBaseValue = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16) - nTemp;
					if (nBaseValue)
					{
						if (nStatId == STAT_MAXSTAMINA)
						{
							nValue += nBaseValue * pCharStatsTxtRecord->nStaminaPerVitality << 6;
						}
						else
						{
							nValue += nBaseValue * pCharStatsTxtRecord->nLifePerVitality << 6;
						}
					}
				}
			}
			break;

		case 11:
			if (nOldValue)
			{
				if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
				{
					nTemp = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);
					if (nTemp)
					{
						nValue += nTemp * nOldValue / 100;
					}
				}
			}
			break;

		case 13:
			if (pStatListEx->pOwner && pStatListEx->pOwner->dwUnitType == UNIT_ITEM)
			{
				if (pStatListEx->pOwner->pStatListEx)
				{
					nIndex = sub_6FDB6300(&pStatListEx->pOwner->pStatListEx->Stats, nStatId << 16);
					if (nIndex >= 0 && &pStatListEx->pOwner->pStatListEx->Stats.pStat[nIndex])
					{
						nOldValue = pStatListEx->pOwner->pStatListEx->Stats.pStat[nIndex].nValue;
						if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN] && pStatListEx->pOwner->pStatListEx->dwFlags & 0x80000000)
						{
							if (pStatListEx->pOwner->pStatListEx->pOwner && (pStatListEx->pOwner->pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
							{
								if (nOldValue < pItemStatCostTxtRecord->dwMinAccr)
								{
									nOldValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
								}
							}
						}
					}
					else
					{
						nOldValue = 0;
					}

					if (nOldValue)
					{
						nTemp = sub_6FDB5830(pStatListEx, pItemStatCostTxtRecord->pOpStatData[i].nStat << 16);
						if (nTemp)
						{
							nValue += nTemp * nOldValue / 100;
						}
					}
				}
				else
				{
					nOldValue = 0;
				}
			}
			break;

		default:
			break;
		}
	}

	return nValue;
}

//D2Common.0x6FDB6300
int __fastcall sub_6FDB6300(D2StatExStrc* pStatEx, int nLayer_StatId)
{
	int nIndex = 0;
	int nMin = 0;
	int nMax = 0;

	nMax = pStatEx->nStatCount;

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId < pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if (nLayer_StatId > pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			return nIndex;
		}
	}

	return -1;
}

//D2Common.0x6FDB6340
int __fastcall sub_6FDB6340(D2StatListExStrc* pStatListEx, int nLayer_StatId, D2ItemStatCostTxt* pItemStatCostTxtRecord)
{
	int nIndex = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	nMax = pStatListEx->Stats.nStatCount;

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId < pStatListEx->Stats.pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if (nLayer_StatId > pStatListEx->Stats.pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			if (nIndex >= 0 && &pStatListEx->Stats.pStat[nIndex])
			{
				nValue = pStatListEx->Stats.pStat[nIndex].nValue;

				if (pItemStatCostTxtRecord)
				{
					if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN])
					{
						if ((pStatListEx->dwFlags & 0x80000000) != 0)
						{
							if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
							{
								if (nValue < pItemStatCostTxtRecord->dwMinAccr)
								{
									nValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
								}
							}
						}
					}
				}

				return nValue;
			}

			return 0;
		}
	}

	return 0;
}

//D2Common.0x6FDB63E0
int __fastcall sub_6FDB63E0(D2StatListExStrc* pStatListEx, int nLayer_StatId, D2ItemStatCostTxt* pItemStatCostTxtRecord)
{
	D2StatExStrc* pStatEx = NULL;
	int nIndex = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	if ((pStatListEx->dwFlags & 0x80000000) != 0)
	{
		pStatEx = &pStatListEx->FullStats;
	}
	else
	{
		pStatEx = &pStatListEx->Stats;
	}

	nMax = pStatEx->nStatCount;

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId < pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if (nLayer_StatId > pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			if (nIndex >= 0 && &pStatEx->pStat[nIndex])
			{
				nValue = pStatEx->pStat[nIndex].nValue;

				if (pItemStatCostTxtRecord)
				{
					if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN])
					{
						if ((pStatListEx->dwFlags & 0x80000000) != 0)
						{
							if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
							{
								if (nValue < pItemStatCostTxtRecord->dwMinAccr)
								{
									nValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
								}
							}
						}
					}
				}

				return nValue;
			}

			return 0;
		}
	}

	return 0;
}

//D2Common.0x6FDB64A0
int __fastcall sub_6FDB64A0(D2StatListExStrc* pStatListEx, int nLayer_StatId, D2ItemStatCostTxt* pItemStatCostTxtRecord, D2UnitStrc* pUnit)
{
	D2ItemStatCostTxt* pOpItemStatCostTxtRecord = NULL;
	D2StatExStrc* pStatEx = NULL;
	D2StatStrc* pStat = NULL;
	int nNewValue = 0;
	int nOldValue = 0;
	int nCounter = 0;
	int nOpStat = 0;
	int nOpBase = 0;
	int nStatId = 0;
	int nIndex = 0;
	int nValue = 0;
	BOOL bUpdate = FALSE;

	nNewValue = sub_6FDB5830(pStatListEx, nLayer_StatId);

	if (pItemStatCostTxtRecord->unk0x51[0])
	{
		if (!nNewValue)
		{
			nIndex = sub_6FDB6300(&pStatListEx->FullStats, nLayer_StatId);
			if (nIndex >= 0)
			{
				pStat = &pStatListEx->FullStats.pStat[nIndex];
				if (pStat)
				{
					nOldValue = pStat->nValue;

					if (pItemStatCostTxtRecord->nKeepZero)
					{
						pStat->nValue = 0;
						if (pItemStatCostTxtRecord->unk0x51[2])
						{
							pStatListEx->dwFlags |= 0x20000000;
						}
					}
					else
					{
						sub_6FDB6A30(pStatListEx->pMemPool, &pStatListEx->FullStats, pStat);
					}

					if (nOldValue)
					{
						if (pStatListEx->fpCallBack && pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FCALLBACK])
						{
							((void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, int, int, int))pStatListEx->fpCallBack)(pStatListEx->pGame, pStatListEx->pOwner, pUnit, nLayer_StatId, nOldValue, 0);
						}
					}
				}
			}
		}

		bUpdate = TRUE;
		nCounter = 0;
		while (nCounter < 3)
		{
			if (pItemStatCostTxtRecord->wOpStat[nCounter] == -1)
			{
				break;
			}

			nOpStat = pItemStatCostTxtRecord->wOpStat[nCounter];
			pOpItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpStat);
			pStatEx = &pStatListEx->FullStats;
			nValue = sub_6FDB64A0(pStatListEx, nOpStat << 16, pOpItemStatCostTxtRecord, pUnit);
			nIndex = sub_6FDB6300(&pStatListEx->FullStats, nOpStat << 16);

			if (nIndex >= 0 && &pStatEx->pStat[nIndex])
			{
				pStat = &pStatEx->pStat[nIndex];
			}
			else if (nValue)
			{
				pStat = sub_6FDB6970(pStatListEx->pMemPool, &pStatListEx->FullStats, nOpStat << 16);
			}

			nOldValue = pStat->nValue;
			if (nValue || pOpItemStatCostTxtRecord->nKeepZero)
			{
				pStat->nValue = nValue;
				if (pOpItemStatCostTxtRecord->unk0x51[2])
				{
					pStatListEx->dwFlags |= 0x20000000;
				}
			}
			else
			{
				sub_6FDB6A30(pStatListEx->pMemPool, &pStatListEx->FullStats, pStat);
			}

			if (nOldValue != nValue && pStatListEx->fpCallBack && pOpItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FCALLBACK])
			{
				((void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, int, int, int))pStatListEx->fpCallBack)(pStatListEx->pGame, pStatListEx->pOwner, pUnit, nOpStat << 16, nOldValue, nValue);
			}

			if (nValue)
			{
				switch (pItemStatCostTxtRecord->nOp)
				{
				case 2:
				case 3:
					if ((pStatListEx->dwOwnerType == UNIT_PLAYER || pStatListEx->dwOwnerType == UNIT_MONSTER) && pItemStatCostTxtRecord->pOpStatData[nCounter].nOpBase != -1)
					{
						if (!(pStatListEx->dwFlags & 0x80000000))
						{
							pStatEx = &pStatListEx->Stats;
						}

						pStat = sub_6FDB6920(pStatEx, pItemStatCostTxtRecord->pOpStatData[nCounter].nOpBase << 16);
						if (pStat && pStat->nValue > 0)
						{
							bUpdate = FALSE;
						}
					}
					break;

				case 4:
				case 5:
					if (pStatListEx->pUnit && (pStatListEx->pUnit->dwUnitType == UNIT_PLAYER || pStatListEx->pUnit->dwUnitType == UNIT_MONSTER))
					{
						nOpBase = pItemStatCostTxtRecord->pOpStatData[nCounter].nOpBase;
						if (nOpBase != -1)
						{
							if (pStatListEx->pUnit->pStatListEx && nOpBase < sgptDataTables->nItemStatCostTxtRecordCount)
							{
								if (&sgptDataTables->pItemStatCostTxt[nOpBase] && sub_6FDB63E0(pStatListEx->pUnit->pStatListEx, nOpBase << 16, &sgptDataTables->pItemStatCostTxt[nOpBase]) > 0)
								{
									bUpdate = FALSE;
								}
							}
						}
					}
					break;

				case 6:
				case 7:
					if (pStatListEx->dwOwnerType == UNIT_PLAYER || pStatListEx->dwOwnerType == UNIT_MONSTER)
					{
						bUpdate = FALSE;
					}
					break;

				case 13:
					if (pStatListEx->dwOwnerType == UNIT_ITEM)
					{
						bUpdate = FALSE;
					}
					break;

				default:
					bUpdate = FALSE;
					break;

				case 0:
				case 8:
				case 9:
				case 10:
				case 11:
					break;
				}
			}

			++nCounter;
		}

		if (nCounter >= 3 || pItemStatCostTxtRecord->wOpStat[nCounter] == -1)
		{
			if (bUpdate && nNewValue)
			{
				sub_6FDB6AB0(pStatListEx, nLayer_StatId, nNewValue, pItemStatCostTxtRecord, pUnit);

				for (int i = 0; i < ARRAY_SIZE(pItemStatCostTxtRecord->unk0x5E); ++i)
				{
					nStatId = pItemStatCostTxtRecord->unk0x5E[i];

					if (nStatId == -1)
					{
						break;
					}

					sub_6FDB6AB0(pStatListEx, nStatId << 16, sub_6FDB64A0(pStatListEx, nStatId << 16, ITEMS_GetItemStatCostTxtRecord(nStatId), pUnit), ITEMS_GetItemStatCostTxtRecord(nStatId), pUnit);
				}
			}

			return nNewValue;
		}
	}

	nIndex = sub_6FDB6300(&pStatListEx->FullStats, nLayer_StatId);

	if (nIndex >= 0 && &pStatListEx->FullStats.pStat[nIndex])
	{
		pStat = &pStatListEx->FullStats.pStat[nIndex];
	}
	else if (nNewValue)
	{
		pStat = sub_6FDB6970(pStatListEx->pMemPool, &pStatListEx->FullStats, nLayer_StatId);
	}
	else
	{
		return 0;
	}

	nOldValue = pStat->nValue;
	if (nNewValue || pItemStatCostTxtRecord->nKeepZero)
	{
		pStat->nValue = nNewValue;
		if (pItemStatCostTxtRecord->unk0x51[2])
		{
			pStatListEx->dwFlags |= 0x20000000;
		}
	}
	else
	{
		sub_6FDB6A30(pStatListEx->pMemPool, &pStatListEx->FullStats, pStat);
	}

	if (nOldValue != nNewValue && pStatListEx->fpCallBack && pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FCALLBACK])
	{
		((void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, int, int, int))pStatListEx->fpCallBack)(pStatListEx->pGame, pStatListEx->pOwner, pUnit, nLayer_StatId, nOldValue, nNewValue);
	}

	return nNewValue;
}

//D2Common.0x6FDB6920
D2StatStrc* __fastcall sub_6FDB6920(D2StatExStrc* pStatEx, int nLayer_StatId)
{
	int nIndex = sub_6FDB6300(pStatEx, nLayer_StatId);

	if (nIndex < 0)
	{
		return NULL;
	}

	return &pStatEx->pStat[nIndex];
}

//D2Common.0x6FDB6970
D2StatStrc* __fastcall sub_6FDB6970(void* pMemPool, D2StatExStrc* pStatEx, int nLayer_StatId)
{
	int nIndex = 0;
	int nMin = 0;
	int nMax = 0;

	nMax = pStatEx->nStatCount;

	if (nMax > 0)
	{
		do
		{
			nIndex = nMin + (nMax - nMin) / 2;

			if (nLayer_StatId < pStatEx->pStat[nIndex].nLayer_StatId)
			{
				nMax = nMin + (nMax - nMin) / 2;
			}
			else if (nLayer_StatId > pStatEx->pStat[nIndex].nLayer_StatId)
			{
				nMin = nIndex + 1;
			}
			else
			{
				return NULL;
			}
		}
		while (nMin < nMax);

		if (nMin < 0)
		{
			return NULL;
		}
	}

	if (pStatEx->nStatCount >= pStatEx->nBitSize)
	{
		pStatEx->nBitSize += 4;
		pStatEx->pStat = (D2StatStrc*)FOG_ReallocServerMemory(pMemPool, pStatEx->pStat, sizeof(D2StatStrc) * pStatEx->nBitSize, __FILE__, __LINE__, 0);
	}

	if (nMin < pStatEx->nStatCount)
	{
		memcpy(&pStatEx->pStat[nMin + 1], &pStatEx->pStat[nMin], sizeof(D2StatStrc) * (pStatEx->nStatCount - nMin));
	}

	pStatEx->pStat[nMin].nLayer_StatId = nLayer_StatId;
	pStatEx->pStat[nMin].nValue = 0;
	++pStatEx->nStatCount;

	return &pStatEx->pStat[nMin];
}

//D2Common.0x6FDB6A30
void __fastcall sub_6FDB6A30(void* pMemPool, D2StatExStrc* pStatEx, D2StatStrc* pStat)
{
	int nIndex = pStat - pStatEx->pStat;

	if (nIndex < pStatEx->nStatCount)
	{
		if (nIndex < pStatEx->nStatCount - 1)
		{
			memcpy(pStat, &pStat[1], sizeof(D2StatStrc) * (pStatEx->nStatCount - (nIndex + 1)));
		}

		--pStatEx->nStatCount;

		if ((int)pStatEx->nBitSize - (int)pStatEx->nStatCount > 8)
		{
			pStatEx->nBitSize -= 4;
			pStatEx->pStat = (D2StatStrc*)FOG_ReallocServerMemory(pMemPool, pStatEx->pStat, sizeof(D2StatStrc) * (short)pStatEx->nBitSize, __FILE__, __LINE__, 0);
		}
	}
}

//D2Common.0x6FDB6AB0
void __fastcall sub_6FDB6AB0(D2StatListExStrc* pStatListEx, int nLayer_StatId, int nValue, D2ItemStatCostTxt* pItemStatCostTxtRecord, D2UnitStrc* pUnit)
{
	D2StatStrc* pStat = NULL;
	int nOldValue = 0;
	int nIndex = 0;
	int nStat = 0;
	int nMin = 0;
	int nMax = 0;

	nMax = pStatListEx->FullStats.nStatCount;

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId == pStatListEx->FullStats.pStat[nIndex].nLayer_StatId)
		{
			if (nIndex < 0 || !&pStatListEx->FullStats.pStat[nIndex])
			{
				if (nValue)
				{
					pStat = sub_6FDB6970(pStatListEx->pMemPool, &pStatListEx->FullStats, nLayer_StatId);
				}
				else
				{
					return;
				}
			}
			else
			{
				pStat = &pStatListEx->FullStats.pStat[nIndex];
			}

			break;
		}
		else if (nLayer_StatId < pStatListEx->FullStats.pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else
		{
			nMin = nIndex + 1;
		}
	}

	if (nMin >= nMax)
	{
		pStat = sub_6FDB6970(pStatListEx->pMemPool, &pStatListEx->FullStats, nLayer_StatId);
	}

	nOldValue = pStat->nValue;
	if (nValue || pItemStatCostTxtRecord->nKeepZero)
	{
		pStat->nValue = nValue;
		if (pItemStatCostTxtRecord->unk0x51[2])
		{
			pStatListEx->dwFlags |= 0x20000000;
		}
	}
	else
	{
		nStat = pStat - pStatListEx->FullStats.pStat;
		if (nStat < pStatListEx->FullStats.nStatCount)
		{
			if (nStat < pStatListEx->FullStats.nStatCount - 1)
			{
				memcpy(pStat, &pStat[1], sizeof(D2StatStrc) * (pStatListEx->FullStats.nStatCount - (nStat + 1)));
			}

			--pStatListEx->FullStats.nStatCount;

			if (pStatListEx->FullStats.nBitSize - (int)pStatListEx->FullStats.nStatCount > 8)
			{
				pStatListEx->FullStats.nBitSize -= 4;
				pStatListEx->FullStats.pStat = (D2StatStrc*)FOG_ReallocServerMemory(pStatListEx->pMemPool, pStatListEx->FullStats.pStat, sizeof(D2StatStrc) * pStatListEx->FullStats.nBitSize, __FILE__, __LINE__, 0);
			}
		}
	}

	if (nOldValue != nValue && pStatListEx->fpCallBack && pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FCALLBACK])
	{
		((void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, int, int, int))pStatListEx->fpCallBack)(pStatListEx->pGame, pStatListEx->pOwner, pUnit, nLayer_StatId, nOldValue, nValue);
	}
}

//D2Common.0x6FDB6C10
void __fastcall sub_6FDB6C10(D2StatListExStrc* pStatListEx, int nLayer_StatId, int nValue, D2UnitStrc* pUnit)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatListExStrc* pStatList = NULL;
	D2StatStrc* pStat = NULL;
	int nOldValue = 0;
	int nStatId = 0;
	int nIndex = 0;
	int nStat = 0;
	int nMin = 0;
	int nMax = 0;

	nStatId = (nLayer_StatId >> 16) & 0xFFFF;

	if (!nValue)
	{
		return;
	}

	pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);

	if (!pItemStatCostTxtRecord || pStatListEx->dwFlags & 0x2000)
	{
		return;
	}

	if (!(pStatListEx->dwFlags & 0x80000000))
	{
		pStatList = (D2StatListExStrc*)pStatListEx->pParent;
	}
	else
	{
		pStatList = pStatListEx;
	}

	while (pStatList)
	{
		if (pItemStatCostTxtRecord->unk0x51[0] || pItemStatCostTxtRecord->unk0x51[1])
		{
			sub_6FDB64A0(pStatList, nLayer_StatId, pItemStatCostTxtRecord, pUnit);
		}
		else
		{
			nMin = 0;
			nMax = pStatList->FullStats.nStatCount;

			while (nMin < nMax)
			{
				nIndex = nMin + (nMax - nMin) / 2;

				if (nLayer_StatId == pStatList->FullStats.pStat[nIndex].nLayer_StatId)
				{
					if (nIndex < 0 || !&pStatList->FullStats.pStat[nIndex])
					{
						pStat = sub_6FDB6970(pStatList->pMemPool, &pStatList->FullStats, nLayer_StatId);
					}
					else
					{
						pStat = &pStatList->FullStats.pStat[nIndex];
					}

					break;
				}
				else if (nLayer_StatId < pStatList->FullStats.pStat[nIndex].nLayer_StatId)
				{
					nMax = nMin + (nMax - nMin) / 2;
				}
				else
				{
					nMin = nIndex + 1;
				}
			}

			if (nMin >= nMax)
			{
				pStat = sub_6FDB6970(pStatList->pMemPool, &pStatList->FullStats, nLayer_StatId);
			}

			nOldValue = pStat->nValue;
			pStat->nValue += nValue;

			if (pStat->nValue > 0 && pItemStatCostTxtRecord->unk0x51[2])
			{
				pStatList->dwFlags |= 0x20000000;
			}

			if (!pStat->nValue && !pItemStatCostTxtRecord->nKeepZero)
			{
				nStat = pStat - pStatList->FullStats.pStat;
				if (nStat < pStatList->FullStats.nStatCount)
				{
					if (nStat < pStatList->FullStats.nStatCount - 1)
					{
						memcpy(pStat, &pStat[1], sizeof(D2StatStrc) * (pStatList->FullStats.nStatCount - (nStat + 1)));
					}

					--pStatList->FullStats.nStatCount;
					if (pStatList->FullStats.nBitSize - (int)pStatList->FullStats.nStatCount > 8)
					{
						pStatList->FullStats.nBitSize -= 4;
						pStatList->FullStats.pStat = (D2StatStrc*)FOG_ReallocServerMemory(pStatList->pMemPool, pStatList->FullStats.pStat, sizeof(D2StatStrc) * pStatList->FullStats.nBitSize, __FILE__, __LINE__, 0);
					}
				}
			}

			if (pStatList->fpCallBack && pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FCALLBACK])
			{
				((void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, int, int, int))pStatList->fpCallBack)(pStatList->pGame, pStatList->pOwner, pUnit, nLayer_StatId, nOldValue, pStat->nValue);
			}
		}

		if ((pStatList->dwFlags & 0x2000) || (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED] && (pStatList->dwFlags & 0x40000000)))
		{
			break;
		}

		pStatList = (D2StatListExStrc*)pStatList->pParent;
	}
}

//D2Common.0x6FDB6E30
void __stdcall D2Common_ExpireStatListEx_6FDB6E30(D2StatListExStrc* pStatListEx)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatListExStrc* pPrevStatList = NULL;
	D2StatExStrc* pStatEx = NULL;
	D2UnitStrc* pOwner = NULL;
	D2UnitStrc* pUnit = NULL;
	int nCounter = 0;
	int nStatId = 0;
	int nLayer_StatIds[16] = {};

	if (pStatListEx)
	{
		pPrevStatList = (D2StatListExStrc*)pStatListEx->pParent;

		if (pPrevStatList)
		{
			if ((pPrevStatList->dwFlags & 0x80000000) != 0)
			{
				if ((D2StatListExStrc*)pPrevStatList->pMyLastList == pStatListEx)
				{
					pPrevStatList->pMyLastList = pStatListEx->pPrevLink;
				}

				if ((D2StatListExStrc*)pPrevStatList->pMyStats == pStatListEx)
				{
					pPrevStatList->pMyStats = pStatListEx->pPrevLink;
				}
			}

			pStatListEx->pParent = NULL;
		}

		if (pStatListEx->pNextLink)
		{
			pStatListEx->pNextLink->pPrevLink = pStatListEx->pPrevLink;
		}

		if (pStatListEx->pPrevLink)
		{
			pStatListEx->pPrevLink->pNextLink = pStatListEx->pNextLink;
		}

		pStatListEx->pPrevLink = NULL;
		pStatListEx->pNextLink = NULL;

		pUnit = pStatListEx->pUnit;
		pStatListEx->pUnit = NULL;

		if (!(pStatListEx->dwFlags & 0x2000))
		{
			if (pStatListEx->dwFlags & 0x80000000)
			{
				pOwner = pStatListEx->pOwner;
			}
			else
			{
				pOwner = NULL;
			}

			if (pStatListEx->dwFlags & 0x80000000 && pStatListEx->dwFlags & 0x20000000)
			{
				for (D2StatStrc* i = pStatListEx->FullStats.pStat; i < &pStatListEx->FullStats.pStat[pStatListEx->FullStats.nStatCount]; ++i)
				{
					pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
					if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->unk0x51[2])
					{
						nLayer_StatIds[nCounter] = i->nLayer_StatId;
						++nCounter;
					}
				}

				for (int i = 0; i < nCounter; ++i)
				{
					nStatId = nLayer_StatIds[i] >> 16;
					pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
					if (pItemStatCostTxtRecord)
					{
						sub_6FDB64A0(pStatListEx, nLayer_StatIds[i], pItemStatCostTxtRecord, 0);
					}
				}
			}

			if (pPrevStatList)
			{
				if (!(pStatListEx->dwFlags & 0x80000000))
				{
					pStatEx = &pStatListEx->Stats;
				}
				else
				{
					pStatEx = &pStatListEx->FullStats;
				}

				if (pStatListEx->dwFlags & 0x40000000)
				{
					for (D2StatStrc* i = pStatEx->pStat; i < &pStatEx->pStat[pStatEx->nStatCount]; ++i)
					{
						pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
						if (pItemStatCostTxtRecord && !(pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED]))
						{
							sub_6FDB6C10(pPrevStatList, i->nLayer_StatId, -i->nValue, pOwner);
						}
					}
				}
				else
				{
					for (D2StatStrc* i = pStatEx->pStat; i < &pStatEx->pStat[pStatEx->nStatCount]; ++i)
					{
						sub_6FDB6C10(pPrevStatList, i->nLayer_StatId, -i->nValue, pOwner);
					}
				}
			}

			if (pUnit)
			{
				if (pStatListEx->fpStatRemove)
				{
					((void(__fastcall*)(D2UnitStrc*, int, D2StatListExStrc*))pStatListEx->fpStatRemove)(pUnit, pStatListEx->dwStateNo, pStatListEx);
				}
			}
		}
	}
}

//D2Common.0x6FDB7030 (#10485)
void __stdcall STATLIST_FreeStatList(D2StatListStrc* pStatList)
{
	if (pStatList && !(pStatList->dwFlags & 0x80000000))
	{
		D2Common_STATLIST_FreeStatListEx_6FDB7050((D2StatListExStrc*)pStatList);
	}
}

//D2Common.0x6FDB7050
void __fastcall D2Common_STATLIST_FreeStatListEx_6FDB7050(D2StatListExStrc* pStatListEx)
{
	D2StatListStrc* pCurrent = NULL;
	D2StatListStrc* pPrevious = NULL;

	D2Common_ExpireStatListEx_6FDB6E30(pStatListEx);

	if (pStatListEx->Stats.pStat)
	{
		FOG_FreeServerMemory(pStatListEx->pMemPool, pStatListEx->Stats.pStat, __FILE__, __LINE__, 0);
	}

	if ((pStatListEx->dwFlags & 0x80000000) != 0)
	{
		pCurrent = pStatListEx->pMyLastList;
		if (pCurrent)
		{
			do
			{
				pPrevious = pCurrent->pPrevLink;
				pCurrent->pParent = NULL;
				pCurrent->pUnit = NULL;
				if (!(pCurrent->dwFlags & 0x80000000))
				{
					if (pStatListEx->pMyLastList == pCurrent)
					{
						pStatListEx->pMyLastList = pPrevious;
					}

					D2Common_STATLIST_FreeStatListEx_6FDB7050((D2StatListExStrc*)pCurrent);

					pPrevious = pStatListEx->pMyLastList;
				}
				pCurrent = pPrevious;
			}
			while (pPrevious);
		}

		if (pStatListEx->FullStats.pStat)
		{
			FOG_FreeServerMemory(pStatListEx->pMemPool, pStatListEx->FullStats.pStat, __FILE__, __LINE__, 0);
		}

		if (pStatListEx->ModStats.pStat)
		{
			FOG_FreeServerMemory(pStatListEx->pMemPool, pStatListEx->ModStats.pStat, __FILE__, __LINE__, 0);
		}

		FOG_FreeServerMemory(pStatListEx->pMemPool, pStatListEx->StatFlags, __FILE__, __LINE__, 0);
	}

	FOG_FreeServerMemory(pStatListEx->pMemPool, pStatListEx, __FILE__, __LINE__, 0);
}

//D2Common.0x6FDB7110 (#10527)
void __stdcall STATLIST_FreeStatListEx(D2UnitStrc* pUnit)
{
	if (pUnit->pStatListEx)
	{
		if ((pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
		{
			D2Common_STATLIST_FreeStatListEx_6FDB7050(pUnit->pStatListEx);
		}

		pUnit->pStatListEx = NULL;
	}
}

//D2Common.0x6FDB7140 (#10470)
D2StatListStrc* __stdcall STATLIST_AllocStatList(void* pMemPool, uint32_t fFilter, uint32_t dwTimeout, int nUnitType, int nUnitGUID)
{
	D2StatListStrc* pStatList = (D2StatListStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2StatListStrc), __FILE__, __LINE__, 0);
	memset(pStatList, 0x00, sizeof(D2StatListStrc));

	pStatList->dwOwnerType = nUnitType;
	pStatList->dwOwnerId = nUnitGUID;
	pStatList->pMemPool = pMemPool;
	pStatList->dwFlags = fFilter;
	pStatList->dwExpireFrame = dwTimeout;

	return pStatList;
}

//D2Common.0x6FDB7190 (#10526)
void __stdcall STATLIST_AllocStatListEx(D2UnitStrc* pUnit, char nFlags, void* pCallbackFunc, D2GameStrc* pGame)
{
	D2StatListExStrc* pStatListEx = NULL;

	STATLIST_FreeStatListEx(pUnit);

	pStatListEx = (D2StatListExStrc*)FOG_AllocServerMemory(pUnit->pMemoryPool, sizeof(D2StatListExStrc), __FILE__, __LINE__, 0);
	memset(pStatListEx, 0x00, sizeof(D2StatListExStrc));

	pStatListEx->pMemPool = pUnit->pMemoryPool;
	pStatListEx->dwOwnerType = pUnit->dwUnitType;
	pStatListEx->dwOwnerId = pUnit->dwUnitId;
	pStatListEx->pOwner = pUnit;
	pStatListEx->fpCallBack = pCallbackFunc;
	pStatListEx->pGame = pGame;
	pStatListEx->dwFlags = nFlags & 1 | 0x80000000;

	pStatListEx->StatFlags = (uint32_t*)FOG_AllocServerMemory(pUnit->pMemoryPool, 2 * sizeof(uint32_t) * (sgptDataTables->nStatesTxtRecordCount + 31) / 32, __FILE__, __LINE__, 0);
	memset(pStatListEx->StatFlags, 0x00, 2 * sizeof(uint32_t) * (sgptDataTables->nStatesTxtRecordCount + 31) / 32);

	pUnit->pStatListEx = pStatListEx;
}

//D2Common.0x6FDB7260 (#10471)
int __stdcall STATLIST_GetOwnerType(D2StatListStrc* pStatList)
{
	if (pStatList)
	{
		return pStatList->dwOwnerType;
	}

	return 6;
}

//D2Common.0x6FD912D0 (#10472)
int __stdcall STATLIST_GetOwnerGUID(D2StatListStrc* pStatList)
{
	if (pStatList)
	{
		return pStatList->dwOwnerId;
	}

	REMOVE_LATER_WriteToLogFile("STATLIST_GetOwnerGUID: NULL pointer");
	return 0;
}

//D2Common.0x6FDB7280 (#11304)
int __stdcall STATLIST_GetBaseStatsCount(D2StatListStrc* pStatList)
{
	if (pStatList)
	{
		return pStatList->Stats.nStatCount;
	}

	return 0;
}

//D2Common.0x6FDB72A0 (#11305)
int __stdcall STATLIST_GetFullStatsCountFromUnit(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx)
	{
		return pUnit->pStatListEx->FullStats.nStatCount;
	}

	return 0;
}

//D2Common.0x6FDB72C0 (#10478)
void __stdcall STATLIST_SetState(D2StatListStrc* pStatList, int nState)
{
	if (pStatList)
	{
		pStatList->dwStateNo = nState;
	}
}

//D2Common.0x6FDB72E0 (#10479)
int __stdcall STATLIST_GetState(D2StatListStrc* pStatList)
{
	if (pStatList)
	{
		return pStatList->dwStateNo;
	}

	return 0;
}

//D2Common.0x6FDB7300 (#10528)
void __stdcall STATLIST_SetExpireFrame(D2StatListStrc* pStatList, int nExpireFrame)
{
	if (pStatList)
	{
		if (nExpireFrame > 0)
		{
			pStatList->dwFlags |= 2;
		}

		pStatList->dwExpireFrame = nExpireFrame;
	}
}

//D2Common.0x6FDB7320 (#10529)
int __stdcall STATLIST_GetExpireFrame(D2StatListStrc* pStatList)
{
	if (pStatList)
	{
		return pStatList->dwExpireFrame;
	}

	REMOVE_LATER_WriteToLogFile("STATLIST_GetExpireFrame: NULL pointer");
	return 0;
}

//D2Common.0x6FDB7340 (#10475)
void __stdcall D2COMMON_10475_PostStatToStatList(D2UnitStrc* pUnit, D2StatListStrc* pStatList, BOOL bResetFlag)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatListExStrc* pCurrentStatList = NULL;
	D2StatExStrc* pStatEx = NULL;
	D2UnitStrc* pOwner = NULL;
	int nCounter = 0;
	int nStatId = 0;
	int nLayer_StatIds[16] = {};

	if (pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		D2Common_ExpireStatListEx_6FDB6E30((D2StatListExStrc*)pStatList);
		pCurrentStatList = pUnit->pStatListEx;

		while ((D2StatListExStrc*)pStatList != pCurrentStatList)
		{
			pCurrentStatList = (D2StatListExStrc*)pCurrentStatList->pParent;
			if (!pCurrentStatList)
			{
				if (pStatList->dwFlags & 4)
				{
					pUnit->pStatListEx->dwFlags |= 2;
				}

				if (pStatList->dwFlags & 0x2000)
				{
					pStatList->pPrevLink = pUnit->pStatListEx->pMyStats;

					if (pUnit->pStatListEx->pMyStats)
					{
						pUnit->pStatListEx->pMyStats->pNextLink = pStatList;
					}

					pStatList->pNextLink = NULL;
					pUnit->pStatListEx->pMyStats = pStatList;
					pStatList->pParent = (D2StatListStrc*)pUnit->pStatListEx;
					pStatList->pUnit = pUnit;
				}
				else
				{
					pStatList->pPrevLink = pUnit->pStatListEx->pMyLastList;
					if (pUnit->pStatListEx->pMyLastList)
					{
						pUnit->pStatListEx->pMyLastList->pNextLink = pStatList;
					}

					pStatList->pParent = (D2StatListStrc*)pUnit->pStatListEx;
					pUnit->pStatListEx->pMyLastList = pStatList;
					pStatList->pUnit = pUnit;

					if (pStatList->dwFlags & 0x80000000)
					{
						pOwner = ((D2StatListExStrc*)pStatList)->pOwner;
					}
					else
					{
						pOwner = NULL;
					}

					if (pStatList->dwFlags & 0x80000000 && pStatList->dwFlags & 0x20000000)
					{
						nCounter = 0;
						for (D2StatStrc* i = ((D2StatListExStrc*)pStatList)->FullStats.pStat; i < &((D2StatListExStrc*)pStatList)->FullStats.pStat[((D2StatListExStrc*)pStatList)->FullStats.nStatCount]; ++i)
						{
							pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
							if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->unk0x51[2])
							{
								nLayer_StatIds[nCounter] = i->nLayer_StatId;
								++nCounter;
							}
						}

						for (int i = 0; i < nCounter; ++i)
						{
							nStatId = nLayer_StatIds[i] >> 16;
							pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
							if (pItemStatCostTxtRecord)
							{
								sub_6FDB64A0((D2StatListExStrc*)pStatList, nLayer_StatIds[i], pItemStatCostTxtRecord, 0);
							}
						}
					}

					if (!(pStatList->dwFlags & 0x80000000))
					{
						pStatEx = &pStatList->Stats;
					}
					else
					{
						pStatEx = &((D2StatListExStrc*)pStatList)->FullStats;
					}


					if (bResetFlag)
					{
						pStatList->dwFlags &= 0xBFFFFFFF;

						for (D2StatStrc* i = pStatEx->pStat; i < &pStatEx->pStat[pStatEx->nStatCount]; ++i)
						{
							sub_6FDB6C10(pUnit->pStatListEx, i->nLayer_StatId, i->nValue, pOwner);
						}
					}
					else
					{
						pStatList->dwFlags |= 0x40000000;

						for (D2StatStrc* i = pStatEx->pStat; i < &pStatEx->pStat[pStatEx->nStatCount]; ++i)
						{
							pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
							if (pItemStatCostTxtRecord && !(pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED]))
							{
								sub_6FDB6C10(pUnit->pStatListEx, i->nLayer_StatId, i->nValue, pOwner);
							}
						}
					}
				}
				return;
			}
		}
	}
}

//D2Common.0x6FDB7560 (#10464)
void __stdcall STATLIST_AddStat(D2StatListStrc* pStatList, int nStatId, int nValue, uint16_t nLayer)
{
	D2StatExStrc* pStatEx = NULL;
	D2StatStrc* pStat = NULL;
	int nStats = 0;

	if (!pStatList || !nValue)
	{
		return;
	}

	pStatEx = &pStatList->Stats;

	pStat = sub_6FDB6920(pStatEx, nLayer + (nStatId << 16));

	if (!pStat)
	{
		pStat = sub_6FDB6970(pStatList->pMemPool, pStatEx, nLayer + (nStatId << 16));
	}

	pStat->nValue += nValue;

	if (!(pStat->nValue))
	{
		nStats = pStat - pStatEx->pStat;

		if (nStats < pStatEx->nStatCount)
		{
			if (nStats < pStatEx->nStatCount - 1)
			{
				memcpy(pStat, &pStat[1], sizeof(D2StatStrc) * (pStatEx->nStatCount - (nStats + 1)));
			}

			--pStatEx->nStatCount;

			if (pStatEx->nBitSize - (signed int)pStatEx->nStatCount > 8)
			{
				pStatEx->nBitSize -= 4;
				pStatEx->pStat = (D2StatStrc*)FOG_ReallocServerMemory(pStatList->pMemPool, pStatEx->pStat, sizeof(D2StatStrc) * (short)pStatEx->nBitSize, __FILE__, __LINE__, 0);
			}
		}
	}

	sub_6FDB6C10((D2StatListExStrc*)pStatList, nLayer + (nStatId << 16), nValue, 0);
	if ((pStatList->dwFlags & 0x80000000) != 0)
	{
		if (pStatList->dwOwnerType == UNIT_PLAYER)
		{
			sub_6FDB7690((D2StatListExStrc*)pStatList, nLayer + (nStatId << 16));
		}
	}
}

//D2Common.0x6FDB7690
void __fastcall sub_6FDB7690(D2StatListExStrc* pStatListEx, int nLayer_StatId)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	int nStatId = 0;
	int nIndex = 0;
	int nMin = 0;
	int nMax = 0;

	if ((pStatListEx->dwFlags & 0x80000000) != 0)
	{
		nStatId = (nLayer_StatId >> 16) & 0xFFFF;

		switch (nStatId)
		{
		case STAT_HITPOINTS:
		case STAT_MANA:
		case STAT_STAMINA:
		case STAT_EXPERIENCE:
		case STAT_GOLD:
			return;
		default:
			pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
			if (pItemStatCostTxtRecord)
			{
				if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_SAVED])
				{
					nMax = pStatListEx->ModStats.nStatCount;
					nMin = 0;

					while (nMin < nMax)
					{
						nIndex = nMin + (nMax - nMin) / 2;

						if (nLayer_StatId < pStatListEx->ModStats.pStat[nIndex].nLayer_StatId)
						{
							nMax = nMin + (nMax - nMin) / 2;
						}
						else if (nLayer_StatId > pStatListEx->ModStats.pStat[nIndex].nLayer_StatId)
						{
							nMin = nIndex + 1;
						}
						else
						{
							return;
						}
					}

					if (nMin >= 0)
					{
						if ((short)pStatListEx->ModStats.nStatCount >= (short)pStatListEx->ModStats.nBitSize)
						{
							pStatListEx->ModStats.nBitSize += 4;
							pStatListEx->ModStats.pStat = (D2StatStrc*)FOG_ReallocServerMemory(pStatListEx->pMemPool, pStatListEx->ModStats.pStat, 4 * (short)pStatListEx->ModStats.nBitSize, __FILE__, __LINE__, 0);
						}

						if (nMin < pStatListEx->ModStats.nStatCount)
						{
							//memcpy(&pStatListEx->ModStats.pStat->nValue + nMin, (char*)pStatListEx->ModStats.pStat + 4 * nMin, 4 * (pStatListEx->ModStats.nStatCount - nMin));
							memcpy(&pStatListEx->ModStats.pStat[nMin + 1], &pStatListEx->ModStats.pStat[nMin], 4 * (pStatListEx->ModStats.nStatCount - nMin));
						}

						pStatListEx->ModStats.pStat[nMin].nLayer_StatId = nLayer_StatId;
						++pStatListEx->ModStats.nStatCount;
					}
				}
			}
			break;
		}
	}
}

//D2Common.0x6FDB77B0 (#10463)
BOOL __stdcall STATLIST_SetStat(D2StatListStrc* pStatList, int nStatId, int nValue, uint16_t nLayer)
{
	return D2Common_11294(pStatList, nStatId, nValue, nLayer, NULL);
}

//D2Common.0x6FDB7910 (#10465)
void __stdcall STATLIST_SetStatIfListIsValid(D2StatListStrc* pStatList, int nStatId, int nValue, uint16_t nLayer)
{
	if (pStatList)
	{
		STATLIST_SetStat(pStatList, nStatId, nValue, nLayer);
	}
}

//D2Common.0x6FDB7930 (#11294)
BOOL __stdcall D2Common_11294(D2StatListStrc* pStatList, int nStatId, int nValue, uint16_t nLayer, D2UnitStrc* pUnit)
{
	D2StatExStrc* pStatEx = NULL;
	D2StatStrc* pStat = NULL;
	int nStats = 0;

	if (!pStatList)
	{
		return FALSE;
	}

	pStatEx = &pStatList->Stats;
	pStat = sub_6FDB6920(pStatEx, nLayer + (nStatId << 16));

	if(!pStat)
	{
		if (!nValue)
		{
			return FALSE;
		}

		pStat = sub_6FDB6970(pStatList->pMemPool, pStatEx, nLayer + (nStatId << 16));
	}

	if (nValue == pStat->nValue)
	{
		return FALSE;
	}
	else
	{
		if (nValue)
		{
			pStat->nValue = nValue;
		}
		else
		{
			nStats = pStat - pStatEx->pStat;

			if (nStats < pStatEx->nStatCount)
			{
				if (nStats < pStatEx->nStatCount - 1)
				{
					memcpy(pStat, &pStat[1], sizeof(D2StatStrc) * (pStatEx->nStatCount - (nStats + 1)));
				}

				--pStatEx->nStatCount;

				if (pStatEx->nBitSize - (signed int)pStatEx->nStatCount > 8)
				{
					pStatEx->nBitSize -= 4;
					pStatEx->pStat = (D2StatStrc*)FOG_ReallocServerMemory(pStatList->pMemPool, pStatEx->pStat, sizeof(D2StatStrc) * (short)pStatEx->nBitSize, __FILE__, __LINE__, 0);
				}
			}
		}

		sub_6FDB6C10((D2StatListExStrc*)pStatList, nLayer + (nStatId << 16), nValue - pStat->nValue, pUnit);
		if ((pStatList->dwFlags & 0x80000000) != 0)
		{
			if (pStatList->dwOwnerType == UNIT_PLAYER)
			{
				sub_6FDB7690((D2StatListExStrc*)pStatList, nLayer + (nStatId << 16));
			}
		}

		return TRUE;
	}
}

//D2Common.0x6FDB7A90 (#11295)
void __stdcall D2Common_11295(D2StatListStrc* pStatList, int nStatId, int nValue, uint16_t nLayer, D2UnitStrc* pUnit)
{
	if (pStatList)
	{
		D2Common_11294(pStatList, nStatId, nValue, nLayer, pUnit);
	}
}

//D2Common.0x6FDB7AB0 (#10517)
void __stdcall STATLIST_SetUnitStat(D2UnitStrc* pUnit, int nStatId, int nValue, uint16_t nLayer)
{
	if (STATLIST_SetStat(pUnit->pStatListEx, nStatId, nValue, nLayer))
	{
		if (pUnit->dwUnitType == UNIT_PLAYER)
		{
			sub_6FDB7690(pUnit->pStatListEx, nLayer + (nStatId << 16));
		}
	}
}

//D2Common.0x6FDB7B00 (#10518)
void __stdcall STATLIST_AddUnitStat(D2UnitStrc* pUnit, int nStatId, int nValue, uint16_t nLayer)
{
	if (nValue)
	{
		STATLIST_AddStat(pUnit->pStatListEx, nStatId, nValue, nLayer);
	}
}

//D2Common.0x6FDB7B30 (#10521)
int __stdcall STATLIST_GetUnitBaseStat(D2UnitStrc* pUnit, int nStatId, uint16_t nLayer)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatListExStrc* pStatListEx = NULL;
	int nLayer_StatId = 0;
	int nIndex = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;
	
	if (!pUnit || !pUnit->pStatListEx)
	{
		return 0;
	}

	pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}

	pStatListEx = pUnit->pStatListEx;

	nMax = pStatListEx->Stats.nStatCount;
	nLayer_StatId = nLayer + (nStatId << 16);

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId < pStatListEx->Stats.pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if (nLayer_StatId > pStatListEx->Stats.pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			if (nIndex >= 0 && &pStatListEx->Stats.pStat[nIndex])
			{
				nValue = pStatListEx->Stats.pStat[nIndex].nValue;

				if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN])
				{
					if ((pStatListEx->dwFlags & 0x80000000) != 0)
					{
						if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
						{
							if (nValue < pItemStatCostTxtRecord->dwMinAccr)
							{
								nValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
							}
						}
					}
				}

				return nValue;
			}
			
			return 0;
		}
	}

	return 0;
}

//D2Common.0x6FDB7C30 (#10519)
int __stdcall STATLIST_GetUnitStat(const D2UnitStrc* pUnit, int nStatId, uint16_t nLayer)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatListExStrc* pStatListEx = NULL;
	D2StatExStrc* pStatEx = NULL;
	int nLayer_StatId = 0;
	int nIndex = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pUnit || !pUnit->pStatListEx)
	{
		return 0;
	}
	
	pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}

	pStatListEx = pUnit->pStatListEx;

	if (!(pStatListEx->dwFlags & 0x80000000))
	{
		pStatEx = &pStatListEx->Stats;
	}
	else
	{
		pStatEx = &pStatListEx->FullStats;
	}

	nMax = pStatEx->nStatCount;
	nLayer_StatId = nLayer + (nStatId << 16);

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId < pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;			
		}
		else if (nLayer_StatId > pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			if (nIndex >= 0 && &pStatEx->pStat[nIndex])
			{
				nValue = pStatEx->pStat[nIndex].nValue;

				if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN])
				{
					if ((pStatListEx->dwFlags & 0x80000000) != 0)
					{
						if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
						{
							if (nValue < pItemStatCostTxtRecord->dwMinAccr)
							{
								nValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
							}
						}
					}
				}

				return nValue;
			}

			return 0;
		}
	}

	return 0;
}

//D2Common.0x(6FDB7D40 (#10466)
int __stdcall STATLIST_GetStatValue(D2StatListStrc* pStatList, int nStatId, uint16_t nLayer)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatListExStrc* pStatListEx = NULL;
	D2StatExStrc* pStatEx = NULL;
	int nLayer_StatId = 0;
	int nIndex = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pStatList)
	{
		return 0;
	}
		
	pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}

	pStatListEx = (D2StatListExStrc*)pStatList;

	nMax = pStatListEx->Stats.nStatCount;
	nLayer_StatId = nLayer + (nStatId << 16);

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId < pStatListEx->Stats.pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if (nLayer_StatId > pStatListEx->Stats.pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			if (nIndex >= 0 && &pStatListEx->Stats.pStat[nIndex])
			{
				nValue = pStatListEx->Stats.pStat[nIndex].nValue;
				if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN])
				{
					if ((pStatListEx->dwFlags & 0x80000000) != 0)
					{
						if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
						{
							if (nValue < pItemStatCostTxtRecord->dwMinAccr)
							{
								nValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
							}
						}
					}
				}

				return nValue;
			}

			return 0;
		}
	}

	return 0;
}

//D2Common.0x6FDB7E30 (#10520)
int __stdcall STATLIST_GetUnitStatSigned(D2UnitStrc* pUnit, int nStatId, uint16_t nLayer)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatListExStrc* pStatListEx = NULL;
	D2StatExStrc* pStatEx = NULL;
	int nLayer_StatId = 0;
	int nIndex = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pUnit->pStatListEx)
	{
		return 0;
	}

	pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}

	pStatListEx = pUnit->pStatListEx;

	if (!(pStatListEx->dwFlags & 0x80000000))
	{
		pStatEx = &pStatListEx->Stats;
	}
	else
	{
		pStatEx = &pStatListEx->FullStats;
	}

	nMax = pStatEx->nStatCount;
	nLayer_StatId = nLayer + (nStatId << 16);

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId < pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if (nLayer_StatId > pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			if (nIndex >= 0 && &pStatEx->pStat[nIndex])
			{
				nValue = pStatEx->pStat[nIndex].nValue;
				if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN])
				{
					if ((pStatListEx->dwFlags & 0x80000000) != 0)
					{
						if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
						{
							if (nValue < pItemStatCostTxtRecord->dwMinAccr)
							{
								nValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
							}
						}
					}
				}

				return nValue;
			}

			return 0;
		}
	}

	return 0;
}

//D2Common.0x6FDB7F40 (#10522)
int __stdcall STATLIST_GetUnitStatBonus(D2UnitStrc* pUnit, int nStatId, uint16_t nLayer)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatListExStrc* pStatListEx = NULL;
	D2StatExStrc* pStatEx = NULL;
	int nBaseValue = 0;
	int nValue = 0;
	int nIndex = 0;

	pStatListEx = pUnit->pStatListEx;

	if (pStatListEx)
	{
		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);

		if (pItemStatCostTxtRecord)
		{
			if ((pStatListEx->dwFlags & 0x80000000) != 0)
			{
				pStatEx = &pStatListEx->FullStats;
			}
			else
			{
				pStatEx = &pStatListEx->Stats;
			}

			nIndex = sub_6FDB6300(pStatEx, nLayer + (nStatId << 16));
			if (nIndex >= 0 && &pStatEx->pStat[nIndex])
			{
				nValue = pStatEx->pStat[nIndex].nValue;

				if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN])
				{
					if ((pStatListEx->dwFlags & 0x80000000) != 0)
					{
						if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
						{
							if (nValue < pItemStatCostTxtRecord->dwMinAccr)
							{
								nValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
							}
						}
					}
				}
			}
			else
			{
				nValue = 0;
			}

			nIndex = sub_6FDB6300(&pStatListEx->Stats, nLayer + (nStatId << 16));
			if (nIndex >= 0 && &pStatListEx->Stats.pStat[nIndex])
			{
				nBaseValue = pStatListEx->Stats.pStat[nIndex].nValue;

				if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN])
				{
					if ((pStatListEx->dwFlags & 0x80000000) != 0)
					{
						if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
						{
							if (nBaseValue < pItemStatCostTxtRecord->dwMinAccr)
							{
								nBaseValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
							}
						}
					}
				}

				return nValue - nBaseValue;
			}
			else
			{
				return nValue;
			}
		}
	}

	return 0;
}

//D2Common.0x6FDB80C0 (#10515)
void __stdcall D2Common_10515(D2UnitStrc* pUnit)
{
	D2StatListStrc* pStatListEx = NULL;
	D2StatListStrc* pPrevious = NULL;

	if (pUnit->pStatListEx && pUnit->pStatListEx->dwFlags & 2)
	{
		pStatListEx = pUnit->pStatListEx->pMyLastList;
		if (pStatListEx)
		{
			do
			{
				pPrevious = pStatListEx->pPrevLink;
				if (pStatListEx->dwFlags & 4)
				{
					if (pStatListEx->dwStateNo)
					{
						STATES_ToggleState(pUnit, pStatListEx->dwStateNo, FALSE);
					}

					if (!(pStatListEx->dwFlags & 0x80000000))
					{
						D2Common_STATLIST_FreeStatListEx_6FDB7050((D2StatListExStrc*)pStatListEx);
					}

					pPrevious = pUnit->pStatListEx->pMyLastList;
				}
				pStatListEx = pPrevious;
			}
			while (pPrevious);
		}

		pUnit->pStatListEx->dwFlags &= ~2;
	}
}

//D2Common.0x6FDB8120 (#10467)
int __stdcall D2Common_10467(D2StatListStrc* pStatList, int nStat)
{
	if (pStatList && nStat < pStatList->Stats.nStatCount)
	{
		return pStatList->Stats.pStat[nStat].nValue;
	}

	return 0;
}

//D2Common.0x6FDB8150 (#10468)
void __stdcall STATLIST_RemoveAllStats(D2StatListStrc* pStatList)
{
	if (pStatList && pStatList->Stats.nStatCount > 0)
	{
		do
		{
			STATLIST_SetStat(pStatList, pStatList->Stats.pStat->nStat, 0, pStatList->Stats.pStat->nLayer);
		}
		while (pStatList->Stats.nStatCount > 0);
	}
}

//D2Common.0x6FDB8190
D2StatListStrc* __stdcall D2Common_GetStateFromStatListEx_6FDB8190(D2StatListExStrc* pStatListEx, int nStateId)
{
	D2StatListStrc* pStatList = NULL;

	if (pStatListEx && nStateId)
	{
		if ((pStatListEx->dwFlags & 0x80000000) != 0)
		{
			if (pStatListEx->pMyLastList)
			{
				pStatList = pStatListEx->pMyLastList;
				while (pStatList->dwStateNo != nStateId)
				{
					pStatList = pStatList->pPrevLink;
					if (!pStatList)
					{
						break;
					}
				}

				if (pStatList)
				{
					return pStatList;
				}
			}

			pStatList = pStatListEx->pMyStats;
			if (pStatList)
			{
				while (pStatList->dwStateNo != nStateId)
				{
					pStatList = pStatList->pPrevLink;
					if (!pStatList)
					{
						return NULL;
					}
				}

				return pStatList;
			}
		}
	}

	return NULL;
}

//D2Common.0x6FDB81E0 (#10480)
D2StatListStrc* __stdcall STATLIST_GetStatListFromUnitAndState(D2UnitStrc* pUnit, int nState)
{
	if (pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		return D2Common_GetStateFromStatListEx_6FDB8190(pUnit->pStatListEx, nState);
	}

	return 0;
}

//D2Common.0x6FDB8200 (#10482)
D2StatListStrc* __stdcall STATLIST_GetStatListFromFlag(D2StatListStrc* pStatList, int nFlag)
{
	if (pStatList && pStatList->pPrevLink)
	{
		pStatList = pStatList->pPrevLink;
		while (!(nFlag & pStatList->dwFlags))
		{
			pStatList = pStatList->pPrevLink;
			if (!pStatList)
			{
				return NULL;
			}
		}

		return pStatList;
	}
	
	return NULL;
}

//D2Common.0x6FDB8230 (#10481)
D2StatListStrc* __stdcall STATLIST_GetStatListFromUnitAndFlag(D2UnitStrc* pUnit, int nFlag)
{
	D2StatListStrc* pStatList = NULL;

	if (pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		pStatList = nFlag & 0x2000 ? pUnit->pStatListEx->pMyStats : pUnit->pStatListEx->pMyLastList;
		nFlag &= ~0x2000;

		if(pStatList)
		{
			while (!(nFlag & pStatList->dwFlags))
			{
				pStatList = pStatList->pPrevLink;
				if (!pStatList)
				{
					return NULL;
				}
			}

			return pStatList;
		}
	}

	return NULL;
}

//D2Common.0x6FDB8270 (#10483)
D2StatListStrc* __stdcall STATLIST_GetStatListFromUnitStateOrFlag(D2UnitStrc* pUnit, int nState, int nFlag)
{
	if (pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		if (nState)
		{
			return D2Common_GetStateFromStatListEx_6FDB8190(pUnit->pStatListEx, nState);
		}

		return STATLIST_GetStatListFromUnitAndFlag(pUnit, nFlag);
	}

	return NULL;
}

//D2Common.0x6FDB82C0 (#10484)
D2StatListStrc* __stdcall STATLIST_GetStatListFromUnitStateAndFlag(D2UnitStrc* pUnit, int nState, int nFlag)
{
	D2StatListStrc* pStatList = NULL;

	if (pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		pStatList = nFlag & 0x2000 ? pUnit->pStatListEx->pMyStats : pUnit->pStatListEx->pMyLastList;
		nFlag &= ~0x2000;

		if (pStatList)
		{
			while (pStatList->dwStateNo != nState || nFlag && !(nFlag & pStatList->dwFlags))
			{
				pStatList = pStatList->pPrevLink;
				if (!pStatList)
				{
					return NULL;
				}
			}

			return pStatList;
		}
	}

	return NULL;
}

//D2Common.0x6FDB8310 (#10523)
void __stdcall STATLIST_MergeStatLists(D2UnitStrc* pTarget, D2UnitStrc* pUnit, BOOL bType)
{
	if (pTarget && pUnit && pUnit->pStatListEx)
	{
		if (pUnit->dwUnitType != UNIT_ITEM || ITEMS_GetBodyLocation(pUnit) != BODYLOC_SWRARM && ITEMS_GetBodyLocation(pUnit) != BODYLOC_SWLARM)
		{
			if (pUnit->pStatListEx->pUnit == pTarget)
			{
				if (bType)
				{
					if (pUnit->pStatListEx->dwFlags & 0x40000000)
					{
						D2Common_11274(pTarget, pUnit);
					}
				}
				else
				{
					if (!(pUnit->pStatListEx->dwFlags & 0x40000000))
					{
						D2Common_11275(pTarget, pUnit);
					}
				}
			}
			else
			{
				D2COMMON_10475_PostStatToStatList(pTarget, pUnit->pStatListEx, bType);
				pUnit->pStatListEx->pUnit = pTarget;
			}
		}
		else
		{
			D2Common_ExpireStatListEx_6FDB6E30(pUnit->pStatListEx);
		}
	}
}

//D2Common.0x6FDB83A0 (#10535)
D2UnitStrc* __stdcall STATLIST_GetOwner(D2UnitStrc* pUnit, BOOL* pDynamic)
{
	if (!pUnit || !pUnit->pStatListEx || !pUnit->pStatListEx->pParent || !(pUnit->pStatListEx->pParent->dwFlags & 0x80000000))
	{
		if (pDynamic)
		{
			*pDynamic = FALSE;
		}

		return NULL;
	}

	if (pDynamic)
	{
		*pDynamic = (pUnit->pStatListEx->dwFlags >> 30) & 1;
	}

	return ((D2StatListExStrc*)pUnit->pStatListEx->pParent)->pOwner;
}

//D2Common.0x6FDB8420 (#10512)
void __stdcall D2Common_10512(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int nStatId, void (__fastcall* pfCallback)(D2UnitStrc*, int, int, D2UnitStrc*))
{
	int nIndex = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pUnit1 || !pUnit1->pStatListEx || !(pUnit1->pStatListEx->dwFlags & 0x80000000))
	{
		return;
	}

	nMin = 0;
	nMax = pUnit1->pStatListEx->ModStats.nStatCount;

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if ((nStatId << 16) < pUnit1->pStatListEx->ModStats.pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if ((nStatId << 16) > pUnit1->pStatListEx->ModStats.pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			nMin = -1;
			break;
		}
	}

	if (nMin != -1)
	{
		return;
	}

	nMin = 0;
	nMax = pUnit1->pStatListEx->Stats.nStatCount;

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if ((nStatId << 16) < pUnit1->pStatListEx->Stats.pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if((nStatId << 16) > pUnit1->pStatListEx->Stats.pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			if (nIndex >= 0 && &pUnit1->pStatListEx->Stats.pStat[nIndex])
			{
				pfCallback(pUnit1, nStatId, pUnit1->pStatListEx->Stats.pStat[nIndex].nValue, pUnit2);
			}

			break;
		}
	}
}

//D2Common.0x6FDB84E0 (#10513)
void __stdcall D2Common_10513(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, void (__fastcall* pfCallback)(D2UnitStrc*, int, int, D2UnitStrc*))
{
	int nIndex = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pUnit1 || !pUnit2 && pfCallback || !pUnit1->pStatListEx || !(pUnit1->pStatListEx->dwFlags & 0x80000000))
	{
		return;
	}

	for (int i = 0; i < pUnit1->pStatListEx->ModStats.nStatCount; ++i)
	{
		nMin = 0;
		nMax = pUnit1->pStatListEx->Stats.nStatCount;

		while (nMin < nMax)
		{
			nIndex = nMin + (nMax - nMin) / 2;

			if (pUnit1->pStatListEx->ModStats.pStat[i].nLayer_StatId < pUnit1->pStatListEx->Stats.pStat[nIndex].nLayer_StatId)
			{
				nMax = nMin + (nMax - nMin) / 2;
			}
			else if (pUnit1->pStatListEx->ModStats.pStat[i].nLayer_StatId > pUnit1->pStatListEx->Stats.pStat[nIndex].nLayer_StatId)
			{
				nMin = nIndex + 1;
			}
			else
			{
				if (nIndex >= 0 && &pUnit1->pStatListEx->Stats.pStat[nIndex])
				{
					pfCallback(pUnit1, pUnit1->pStatListEx->Stats.pStat[nIndex].nStat, pUnit1->pStatListEx->Stats.pStat[nIndex].nValue, pUnit2);
				}
				else
				{
					pfCallback(pUnit1, pUnit1->pStatListEx->ModStats.pStat[i].nStat, 0, pUnit2);
				}

				break;
			}
		}

		if (nMin >= nMax)
		{
			pfCallback(pUnit1, pUnit1->pStatListEx->ModStats.pStat[i].nStat, 0, pUnit2);
		}
	}
}

//D2Common.0x6FDB85D0 (#10511)
void __stdcall STATLIST_FreeModStats(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		if (pUnit->pStatListEx->ModStats.pStat)
		{
			FOG_FreeServerMemory(pUnit->pStatListEx->pMemPool, pUnit->pStatListEx->ModStats.pStat, __FILE__, __LINE__, 0);
			pUnit->pStatListEx->ModStats.pStat = NULL;
		}

		pUnit->pStatListEx->ModStats.nBitSize = 0;
		pUnit->pStatListEx->ModStats.nStatCount = 0;
	}
}

//D2Common.0x6FDB8620 (#10562)
int __stdcall STATLIST_GetUnitAlignment(D2UnitStrc* pUnit)
{
	D2StatListStrc* pStatList = NULL;

	if (!pUnit || !pUnit->pStatListEx)
	{
		return 0;
	}

	if (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER)
	{
		return 2;
	}

	if (!(pUnit->pStatListEx->dwFlags & 0x80000000))
	{
		return 0;
	}

	pStatList = D2Common_GetStateFromStatListEx_6FDB8190(pUnit->pStatListEx, STATE_ALIGNMENT);
	if (!pStatList)
	{
		return 0;
	}

	return D2Common_10532((D2StatListExStrc*)pStatList, STAT_ALIGNMENT);
}

//D2Common.0x6FDB8750 (#10534)
void __stdcall D2Common_10534(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx)
	{
		pUnit->pStatListEx->dwFlags |= 0x100;
	}
}

//D2Common.0x6FDB8770 (#10530)
BOOL __stdcall D2COMMON_10530_D2CheckStatlistFlagDMGRed(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx)
	{
		return pUnit->pStatListEx->dwFlags & 0x100;
	}

	return FALSE;
}

//D2Common.0x6FDB87A0 (#10532)
int __stdcall D2Common_10532(D2StatListExStrc* pStatListEx, int nStatId)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatExStrc* pStatEx = NULL;
	int nLayer_StatId = 0;
	int nIndex = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pStatListEx)
	{
		return 0;
	}

	pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}
	
	nLayer_StatId = nStatId << 16;

	if (!(pStatListEx->dwFlags & 0x80000000))
	{
		pStatEx = &pStatListEx->Stats;
	}
	else
	{
		pStatEx = &pStatListEx->FullStats;
	}

	nMax = pStatEx->nStatCount;

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId < pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if (nLayer_StatId > pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			if (nIndex >= 0 && &pStatEx->pStat[nIndex])
			{
				nValue = pStatEx->pStat[nIndex].nValue;

				if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN])
				{
					if ((pStatListEx->dwFlags & 0x80000000) != 0)
					{
						if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
						{
							if (nValue < pItemStatCostTxtRecord->dwMinAccr)
							{
								nValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
							}
						}
					}
				}

				return nValue;
			}

			return 0;
		}
	}

	return 0;
}

//D2Common.0x6FDB8890 (#10533)
void __stdcall D2Common_10533(D2UnitStrc* pUnit)
{
	D2StatListStrc* pStatList = NULL;

	if (pUnit && pUnit->pStatListEx)
	{
		pUnit->pStatListEx->dwFlags &= ~0x100;

		if ((pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
		{
			pStatList = pUnit->pStatListEx->pMyLastList;
			if (pStatList)
			{
				while (!(pStatList->dwFlags & 0x80))
				{
					pStatList = pStatList->pPrevLink;
					if (!pStatList)
					{
						return;
					}
				}

				STATLIST_RemoveAllStats(pStatList);
			}
		}
	}
}

//D2Common.0x6FDB8900
void __stdcall D2Common_STATES_ToggleState_6FDB8900(D2UnitStrc* pUnit, int nState, BOOL bSet)
{
	D2StatesTxt* pStatesTxtRecord = NULL;
	BOOL bIsSetAlready = FALSE;

	if (pUnit && pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		bIsSetAlready = pUnit->pStatListEx->StatFlags[nState >> 5] & gdwBitMasks[nState & 31];

		if (bSet)
		{
			pUnit->pStatListEx->StatFlags[nState >> 5] |= gdwBitMasks[nState & 31];
		}
		else
		{
			pUnit->pStatListEx->StatFlags[nState >> 5] &= gdwInvBitMasks[nState & 31];
		}

		if (bIsSetAlready)
		{
			if (bSet)
			{
				return;
			}
		}
		else
		{
			if (!bSet)
			{
				return;
			}
		}

		pUnit->pStatListEx->StatFlags[(nState >> 5) + (sgptDataTables->nStatesTxtRecordCount + 31) / 32] |= gdwBitMasks[nState & 31];

		if (nState >= 0 && nState < sgptDataTables->nStatesTxtRecordCount)
		{
			pStatesTxtRecord = &sgptDataTables->pStatesTxt[nState];
			if (pStatesTxtRecord)
			{
				if (bSet)
				{
					if (pStatesTxtRecord->dwStateFlags & gdwBitMasks[STATEMASK_DISGUISE])
					{
						pUnit->dwFlagEx |= UNITFLAGEX_ISSHAPESHIFTED;
					}
				}
				else
				{
					if (pStatesTxtRecord->dwStateFlags & gdwBitMasks[STATEMASK_DISGUISE] && !STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_DISGUISE))
					{
						pUnit->dwFlagEx &= ~UNITFLAGEX_ISSHAPESHIFTED;
					}
				}
			}
		}
	}
}

//D2Common.0x6FDB8A90
uint32_t* __stdcall D2COMMON_STATES_GetStatFlags_6FDB8A90(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		return pUnit->pStatListEx->StatFlags;
	}

	return NULL;
}

//D2Common.0x6FDB8AC0
uint32_t* __stdcall D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		return &pUnit->pStatListEx->StatFlags[(sgptDataTables->nStatesTxtRecordCount + 31) / 32];
	}

	return NULL;
}

//D2Common.0x6FDB8B10 (#10516)
void __stdcall D2Common_10516(D2UnitStrc* pUnit, int nFrame)
{
	D2StatListStrc* pStatListEx = NULL;
	D2StatListStrc* pPrevious = NULL;

	if (pUnit && pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		pStatListEx = pUnit->pStatListEx->pMyLastList;
		if (nFrame)
		{
			if (pStatListEx)
			{
				do
				{
					pPrevious = pStatListEx->pPrevLink;
					if (pStatListEx->dwFlags & 2 && (int)pStatListEx->dwExpireFrame <= nFrame)
					{
						if (!(pStatListEx->dwFlags & 0x80000000))
						{
							D2Common_STATLIST_FreeStatListEx_6FDB7050((D2StatListExStrc*)pStatListEx);
						}

						pPrevious = pUnit->pStatListEx->pMyLastList;
					}
					pStatListEx = pPrevious;
				}
				while (pPrevious);
			}
		}
		else
		{
			if (pStatListEx)
			{
				do
				{
					pPrevious = pStatListEx->pPrevLink;
					if (pStatListEx->dwFlags & 2)
					{
						--pStatListEx->dwExpireFrame;
						if ((int)pStatListEx->dwExpireFrame <= 0)
						{
							if (!(pStatListEx->dwFlags & 0x80000000))
							{
								D2Common_STATLIST_FreeStatListEx_6FDB7050((D2StatListExStrc*)pStatListEx);
							}
							pPrevious = pUnit->pStatListEx->pMyLastList;
						}
					}
					pStatListEx = pPrevious;
				}
				while (pPrevious);
			}
		}
	}
}

//D2Common.0x6FDB8BA0 (#11268)
int __stdcall STATLIST_GetFullStatsDataFromUnit(D2UnitStrc* pUnit, D2StatStrc* pStatBuffer, int nBufferSize)
{
	int nStatCount = 0;
	int nCounter = 0;

	if (pUnit && pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		nStatCount = pUnit->pStatListEx->FullStats.nStatCount;
		if (nStatCount >= nBufferSize)
		{
			nStatCount = nBufferSize;
		}

		while (nCounter < nStatCount)
		{
			pStatBuffer[nCounter].nLayer = pUnit->pStatListEx->FullStats.pStat[nCounter].nLayer;
			pStatBuffer[nCounter].nStat = pUnit->pStatListEx->FullStats.pStat[nCounter].nStat;
			pStatBuffer[nCounter].nValue = pUnit->pStatListEx->FullStats.pStat[nCounter].nValue;

			++nCounter;
		}

		return nStatCount;
	}

	return 0;
}

//D2Common.0x6FDB8C00 (#11243)
int __stdcall STATLIST_GetBaseStatsData(D2StatListExStrc* pStatListEx, D2StatStrc* pStatBuffer, int nBufferSize)
{
	int nStatCount = 0;
	int nCounter = 0;

	if (pStatListEx)
	{
		nStatCount = pStatListEx->Stats.nStatCount;
		if (nStatCount >= nBufferSize)
		{
			nStatCount = nBufferSize;
		}

		while (nCounter < nStatCount)
		{
			pStatBuffer[nCounter].nLayer = pStatListEx->Stats.pStat[nCounter].nLayer;
			pStatBuffer[nCounter].nStat = pStatListEx->Stats.pStat[nCounter].nStat;
			pStatBuffer[nCounter].nValue = pStatListEx->Stats.pStat[nCounter].nValue;

			++nCounter;
		}

		return nStatCount;
	}

	return 0;
}

//D2Common.0x6FDB8C50 (#10573)
void __stdcall STATLIST_MergeBaseStats(D2StatListStrc* pTargetStatList, D2StatListStrc* pSourceStatlist)
{
	if (pTargetStatList && pSourceStatlist)
	{
		for (D2StatStrc* i = pSourceStatlist->Stats.pStat; i < &pSourceStatlist->Stats.pStat[pSourceStatlist->Stats.nStatCount]; ++i)
		{
			STATLIST_AddStat(pTargetStatList, i->nStat, i->nValue, i->nLayer);
		}
	}
}

//D2Common.0x6FDB8CA0 (#10477)
void __stdcall STATLIST_SetStatRemoveCallback(D2StatListStrc* pStatList, void* pfStatRemove)
{
	if (pStatList)
	{
		pStatList->fpStatRemove = pfStatRemove;
	}
}

//D2Common.0x6FDB8CC0 (#10469)
void __stdcall D2Common_10469(D2UnitStrc* pUnit)
{
	D2StatListStrc* pStatListEx = NULL;
	D2StatListStrc* pPrevious = NULL;

	if (pUnit && pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		pStatListEx = pUnit->pStatListEx->pMyLastList;
		if (pStatListEx)
		{
			do
			{
				pPrevious = pStatListEx->pPrevLink;
				if (pStatListEx->dwOwnerType != UNIT_ITEM && !(pStatListEx->dwFlags & 0x181) && !STATES_CheckStateMaskStayDeathOnUnitByStateId(pUnit, pStatListEx->dwStateNo))
				{
					if (!(pStatListEx->dwFlags & 0x80000000))
					{
						D2Common_STATLIST_FreeStatListEx_6FDB7050((D2StatListExStrc*)pStatListEx);
					}

					pPrevious = pUnit->pStatListEx->pMyLastList;
				}
				pStatListEx = pPrevious;
			}
			while (pPrevious);
		}
	}
}

//D2Common.0x6FDB8D30 (#10514)
void __stdcall D2Common_10514(D2UnitStrc* pUnit)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	int nCurrent = 0;
	int nMax = 0;

	if (pUnit && pUnit->pStatListEx)
	{
		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_STAMINA);
		if (pItemStatCostTxtRecord)
		{
			nCurrent = sub_6FDB63E0(pUnit->pStatListEx, 0xA0000, pItemStatCostTxtRecord);
		}
		else
		{
			nCurrent = 0;
		}

		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAXSTAMINA);
		if (pItemStatCostTxtRecord)
		{
			nMax = sub_6FDB63E0(pUnit->pStatListEx, 0xB0000, pItemStatCostTxtRecord);
		}
		else
		{
			nMax = 0;
		}

		if (nCurrent > nMax)
		{
			STATLIST_AddStat(pUnit->pStatListEx, STAT_STAMINA, nMax - nCurrent, 0);
		}

		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MANA);
		if (pItemStatCostTxtRecord)
		{
			nCurrent = sub_6FDB63E0(pUnit->pStatListEx, 0x80000, pItemStatCostTxtRecord);
		}
		else
		{
			nCurrent = 0;
		}

		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAXMANA);
		if (pItemStatCostTxtRecord)
		{
			nMax = sub_6FDB63E0(pUnit->pStatListEx, 0x90000, pItemStatCostTxtRecord);
		}
		else
		{
			nMax = 0;
		}

		if (nCurrent > nMax)
		{
			STATLIST_AddStat(pUnit->pStatListEx, STAT_MANA, nMax - nCurrent, 0);
		}

		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_HITPOINTS);
		if (pItemStatCostTxtRecord)
		{
			nCurrent = sub_6FDB63E0(pUnit->pStatListEx, 0x60000, pItemStatCostTxtRecord);
		}
		else
		{
			nCurrent = 0;
		}

		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(STAT_MAXHP);
		if (pItemStatCostTxtRecord)
		{
			nMax = sub_6FDB63E0(pUnit->pStatListEx, 0x70000, pItemStatCostTxtRecord);
		}
		else
		{
			nMax = 0;
		}

		if (nCurrent > nMax)
		{
			STATLIST_AddStat(pUnit->pStatListEx, STAT_HITPOINTS, nMax - nCurrent, 0);
		}
	}
}

//D2Common.0x6FDB8EB0 (#10574)
BOOL __stdcall D2Common_10574(D2UnitStrc* pUnit, int nStateId, BOOL bSet)
{
	D2StatListStrc* pStatList = NULL;

	if (nStateId && pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		pStatList = D2Common_GetStateFromStatListEx_6FDB8190(pUnit->pStatListEx, nStateId);
		if (pStatList)
		{
			if (bSet)
			{
				if (!(pStatList->dwFlags & 0x2000))
				{
					D2Common_ExpireStatListEx_6FDB6E30((D2StatListExStrc*)pStatList);
					pStatList->dwFlags |= 0x2000;
					D2COMMON_10475_PostStatToStatList(pUnit, pStatList, TRUE);
				}
			}
			else
			{
				if (pStatList->dwFlags & 0x2000)
				{
					D2Common_ExpireStatListEx_6FDB6E30((D2StatListExStrc*)pStatList);
					pStatList->dwFlags &= ~0x2000;
					D2COMMON_10475_PostStatToStatList(pUnit, pStatList, TRUE);
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FDB8F30 (#10525)
void __stdcall D2Common_10525(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2)
{
	D2ItemStatCostTxt* pOpItemStatCostTxtRecord = NULL;
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatListExStrc* pStatListEx1 = NULL;
	D2StatListExStrc* pStatListEx2 = NULL;
	D2StatStrc* pStat = NULL;
	int nLayer_StatId = 0;
	int nOldValue = 0;
	int nNewValue = 0;
	int nOpStat = 0;
	int nStats = 0;
	int nValue = 0;
	int nIndex = 0;
	int nMin = 0;
	int nMax = 0;

	if (pUnit1 && pUnit2 && (pUnit1->dwUnitType == UNIT_PLAYER || pUnit1->dwUnitType == UNIT_MONSTER))
	{
		pStatListEx2 = pUnit2->pStatListEx;
		if (pStatListEx2 && (pStatListEx2->dwFlags & 0x80000000) != 0)
		{
			for (D2StatStrc* j = pStatListEx2->FullStats.pStat; j < &pStatListEx2->FullStats.pStat[pStatListEx2->FullStats.nStatCount]; ++j)
			{
				pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(j->nStat);

				if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->nOp >= 6 && pItemStatCostTxtRecord->nOp <= 7)
				{
					for (int i = 0; i < 3; ++i)
					{
						if (pItemStatCostTxtRecord->wOpStat[i] == -1)
						{
							break;
						}

						nOpStat = pItemStatCostTxtRecord->wOpStat[i];
						
						pOpItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpStat);

						nLayer_StatId = nOpStat << 16;
						nValue = sub_6FDB5830(pUnit1->pStatListEx, nLayer_StatId);

						pStatListEx1 = pUnit1->pStatListEx;

						nMin = 0;
						nMax = pStatListEx1->FullStats.nStatCount;

						while (nMin < nMax)
						{
							nIndex = nMin + (nMax - nMin) / 2;

							if (nLayer_StatId == pStatListEx1->FullStats.pStat[nIndex].nLayer_StatId)
							{
								if (nIndex < 0 || !&pStatListEx1->FullStats.pStat[nIndex])
								{
									if (nValue)
									{
										pStat = sub_6FDB6970(pStatListEx1->pMemPool, &pStatListEx1->FullStats, nLayer_StatId);
									}
								}
								else
								{
									pStat = &pStatListEx1->FullStats.pStat[nIndex];
								}

								nNewValue = nValue;
								nOldValue = pStat->nValue;

								if (nValue || pOpItemStatCostTxtRecord->nKeepZero)
								{
									pStat->nValue = nValue;
									if (pOpItemStatCostTxtRecord->unk0x51[2])
									{
										pStatListEx1->dwFlags |= 0x20000000;
									}
								}
								else
								{
									nStats = pStat - pStatListEx1->FullStats.pStat;
									if (nStats < pStatListEx1->FullStats.nStatCount)
									{
										if (nStats < pStatListEx1->FullStats.nStatCount - 1)
										{
											memcpy(pStat, &pStat[1], sizeof(D2StatStrc) * (pStatListEx1->FullStats.nStatCount - (nStats + 1)));
										}

										--pStatListEx1->FullStats.nStatCount;

										if (pStatListEx1->FullStats.nBitSize - (int)pStatListEx1->FullStats.nStatCount > 8)
										{
											pStatListEx1->FullStats.nBitSize -= 4;
											pStatListEx1->FullStats.pStat = (D2StatStrc*)FOG_ReallocServerMemory(pStatListEx1->pMemPool, pStatListEx1->FullStats.pStat, sizeof(D2StatStrc) * (short)pStatListEx1->FullStats.nBitSize, __FILE__, __LINE__, 0);
										}

										nNewValue = 0;
									}
								}

								if (nOldValue != nNewValue)
								{
									if (pStatListEx1->fpCallBack)
									{
										if (pOpItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FCALLBACK])
										{
											((void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, int, int, int))pStatListEx1->fpCallBack)(pStatListEx1->pGame, pStatListEx1->pOwner, pUnit1, nLayer_StatId, nOldValue, nNewValue);
										}
									}
								}

								break;
							}
							else if (nLayer_StatId < pStatListEx1->FullStats.pStat[nIndex].nLayer_StatId)
							{
								nMax = nMin + (nMax - nMin) / 2;
							}
							else
							{
								nMin = nIndex + 1;
							}
						}
					}
				}
			}
		}
	}
}

//D2Common.0x6FDB91C0 (#10474)
void __stdcall D2Common_10474(D2UnitStrc* pUnused, D2StatListStrc* pStatList)
{
	D2Common_ExpireStatListEx_6FDB6E30((D2StatListExStrc*)pStatList);
}

//D2Common.0x6FDB91D0 (#10564)
int __stdcall STATLIST_GetMaxLifeFromUnit(D2UnitStrc* pUnit)
{
	return D2Common_11248(NULL, pUnit, STAT_MAXHP);
}

//D2Common.0x6FDB92C0 (#10565)
int __stdcall STATLIST_GetMaxManaFromUnit(D2UnitStrc* pUnit)
{
	return D2Common_11248(NULL, pUnit, STAT_MAXMANA);
}

//D2Common.0x6FDB93B0 (#10566)
int __stdcall STATLIST_GetMaxStaminaFromUnit(D2UnitStrc* pUnit)
{
	return D2Common_11248(NULL, pUnit, STAT_MAXSTAMINA);
}

//D2Common.0x6FDB94A0 (#10567)
int __stdcall STATLIST_GetMaxDurabilityFromUnit(D2UnitStrc* pUnit)
{
	return D2Common_11248(NULL, pUnit, STAT_MAXDURABILITY);
}

//D2Common.0x6FDB95D0 (#10568)
int __stdcall STATLIST_GetMaxDamageFromUnit(D2UnitStrc* pUnit, BOOL b2Handed)
{
	if (b2Handed)
	{
		return D2Common_11248(NULL, pUnit, STAT_SECONDARY_MAXDAMAGE);
	}
	else
	{
		return D2Common_11248(NULL, pUnit, STAT_MAXDAMAGE);
	}
}

//D2Common.0x6FDB96F0 (#10569)
int __stdcall STATLIST_GetMinDamageFromUnit(D2UnitStrc* pUnit, BOOL b2Handed)
{
	if (b2Handed)
	{
		return D2Common_11248(NULL, pUnit, STAT_SECONDARY_MINDAMAGE);
	}
	else
	{
		return D2Common_11248(NULL, pUnit, STAT_MINDAMAGE);
	}
}

//D2Common.0x6FDB9810 (#10570)
int __stdcall STATLIST_GetMaxThrowDamageFromUnit(D2UnitStrc* pUnit)
{
	return D2Common_11248(NULL, pUnit, STAT_ITEM_THROW_MAXDAMAGE);
}

//D2Common.0x6FDB9900 (#10571)
int __stdcall STATLIST_GetMinThrowDamageFromUnit(D2UnitStrc* pUnit)
{
	return D2Common_11248(NULL, pUnit, STAT_ITEM_THROW_MINDAMAGE);
}

//D2Common.0x6FDB99F0 (#10572)
int __stdcall STATLIST_GetDefenseFromUnit(D2UnitStrc* pUnit)
{
	return D2Common_11248(NULL, pUnit, STAT_ARMORCLASS);
}

//D2Common.0x6FDB9AE0 (#10524)
void __stdcall STATLIST_ExpireUnitStatlist(D2UnitStrc* pUnused, D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx)
	{
		D2Common_ExpireStatListEx_6FDB6E30(pUnit->pStatListEx);
	}
}

//D2Common.0x6FDB9B00 (#10531)
int __stdcall D2Common_10531_SetStatInStatListLayer0(D2StatListStrc* pStatList, int nStatId, int nValue, int nUnused)
{
	return STATLIST_SetStat(pStatList, nStatId, nValue, 0);
}

//D2Common.0x6FDB9B10 (#11248)
int __stdcall D2Common_11248(D2UnitStrc* pUnused, D2UnitStrc* pUnit, int nStatId)
{
	if (pUnit)
	{
		return D2Common_10532(pUnit->pStatListEx, nStatId);
	}

	return 0;
}

//D2Common.0x6FDA9E60 (#11264)
void __stdcall STATLIST_SetSkillId(D2StatListStrc* pStatList, int nSkillId)
{
	pStatList->dwSkillNo = nSkillId;
}

//D2Common.0x6FDB9C10 (#11265)
int __stdcall STATLIST_GetSkillId(D2StatListStrc* pStatList)
{
	if (pStatList)
	{
		return pStatList->dwSkillNo;
	}

	REMOVE_LATER_WriteToLogFile("STATLIST_GetSkillId: NULL pointer");
	return 0;
}

//D2Common.0x6FDB9C20 (#11266)
void __stdcall STATLIST_SetSkillLevel(D2StatListStrc* pStatList, int nSkillLevel)
{
	if (pStatList)
	{
		pStatList->dwSLvl = nSkillLevel;
		return;
	}

	REMOVE_LATER_WriteToLogFile("STATLIST_SetSkillLevel: NULL pointer");
}

//D2Common.0x6FDA9E70 (#11267)
int __stdcall STATLIST_GetSkillLevel(D2StatListStrc* pStatList)
{
	if (pStatList)
	{
		return pStatList->dwSLvl;
	}

	REMOVE_LATER_WriteToLogFile("STATLIST_GetSkillLevel: NULL pointer");
	return 0;
}

//D2Common.0x6FDB9C30 (#11269)
int __stdcall D2COMMON_11269_CopyStats(D2StatListExStrc* pStatListEx, int nStatId, D2StatStrc* pBuffer, int nBufferSize)
{
	return D2Common_CopyStats_6FDB9C50(pStatListEx, nStatId, pBuffer, nBufferSize);
}

//D2Common.0x6FDB9C50
int __fastcall D2Common_CopyStats_6FDB9C50(D2StatListExStrc* pStatListEx, int nStatId, D2StatStrc* pBuffer, int nBufferSize)
{
	D2StatExStrc* pStatEx = NULL;
	int nLayer_StatId = 0;
	int nCopiedStats = 0;
	int nCounter = 0;
	int nIndex = 0;
	int nValue = 0;
	int nMin = 0;
	int nMax = 0;

	if (!pStatListEx)
	{
		return 0;
	}

	if (!(pStatListEx->dwFlags & 0x80000000))
	{
		pStatEx = &pStatListEx->Stats;
	}
	else
	{
		pStatEx = &pStatListEx->FullStats;
	}

	nMax = pStatEx->nStatCount;
	nLayer_StatId = nStatId << 16;

	while (nMin < nMax)
	{
		nIndex = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId < pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if (nLayer_StatId > pStatEx->pStat[nIndex].nLayer_StatId)
		{
			nMin = nIndex + 1;
		}
		else
		{
			break;
		}
	}

	if (nMin >= pStatEx->nStatCount && nMin >= nMax)
	{
		return 0;
	}
	
	if (nMin >= nMax)
	{
		nCounter = nMin;
	}
	else
	{
		nCounter = nMin + (nMax - nMin) / 2;
	}

	if (nCounter < 0)
	{
		return 0;
	}

	nCopiedStats = 0;

	while (nCounter < pStatEx->nStatCount)
	{
		if (pStatEx->pStat[nCounter].nStat != nStatId)
		{
			break;
		}

		if (nCopiedStats >= nBufferSize)
		{
			break;
		}

		pBuffer[nCopiedStats].nLayer_StatId = pStatEx->pStat[nCounter].nLayer_StatId;
		pBuffer[nCopiedStats].nValue = pStatEx->pStat[nCounter].nValue;

		++nCopiedStats;

		++nCounter;
	}

	return nCopiedStats;
}

//D2Common.0x6FDB9D20 (#11270)
int __stdcall D2Common_11270(D2UnitStrc* pUnit, int nStatId, D2StatStrc* pBuffer, int nBufferSize)
{
	if (pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		return D2Common_CopyStats_6FDB9C50(pUnit->pStatListEx, nStatId, pBuffer, nBufferSize);
	}

	return 0;
}

//D2Common.0x6FDB9D60 (#11273)
int __stdcall D2Common_11273(D2UnitStrc* pUnit, int a2)
{
	if (pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
	{
		return sub_6FDB5830(pUnit->pStatListEx, a2 << 16);
	}

	return 0;
}

//D2Common.0x6FDB9D90 (#11274)
BOOL __stdcall D2Common_11274(D2UnitStrc* pTarget, D2UnitStrc* pUnit)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatExStrc* pStatEx = NULL;

	if (!pTarget || !pUnit || !pUnit->pStatListEx)
	{
		return FALSE;
	}

	if (pUnit->pStatListEx->pUnit != pTarget)
	{
		STATLIST_MergeStatLists(pTarget, pUnit, 1);
		return FALSE;
	}

	if (!pTarget->pStatListEx || !(pTarget->pStatListEx->dwFlags & 0x80000000) || !(pUnit->pStatListEx->dwFlags & 0x40000000))
	{
		return FALSE;
	}

	pUnit->pStatListEx->dwFlags &= ~0x40000000;

	if (!(pUnit->pStatListEx->dwFlags & 0x80000000))
	{
		pStatEx = &pUnit->pStatListEx->Stats;
	}
	else
	{
		pStatEx = &pUnit->pStatListEx->FullStats;
	}

	for (D2StatStrc* i = pStatEx->pStat; i < &pStatEx->pStat[pStatEx->nStatCount]; ++i)
	{
		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
		if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED])
		{
			sub_6FDB6C10(pTarget->pStatListEx, i->nLayer_StatId, i->nValue, pUnit);
		}
	}

	return TRUE;
}

//D2Common.0x6FDB9E60 (#11275)
BOOL __stdcall D2Common_11275(D2UnitStrc* pTarget, D2UnitStrc* pUnit)
{
	D2ItemStatCostTxt* pItemStatCostTxtRecord = NULL;
	D2StatExStrc* pStatEx = NULL;

	if (!pTarget || !pUnit || !pUnit->pStatListEx)
	{
		return FALSE;
	}

	if (pUnit->pStatListEx->pUnit != pTarget)
	{
		STATLIST_MergeStatLists(pTarget, pUnit, 0);
		return FALSE;
	}

	if (!pTarget->pStatListEx || !(pTarget->pStatListEx->dwFlags & 0x80000000) || pUnit->pStatListEx->dwFlags & 0x40000000)
	{
		return FALSE;
	}

	pUnit->pStatListEx->dwFlags |= 0x40000000;

	if (!(pUnit->pStatListEx->dwFlags & 0x80000000))
	{
		pStatEx = &pUnit->pStatListEx->Stats;
	}
	else
	{
		pStatEx = &pUnit->pStatListEx->FullStats;
	}

	for (D2StatStrc* i = pStatEx->pStat; i < &pStatEx->pStat[pStatEx->nStatCount]; ++i)
	{
		pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
		if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED])
		{
			sub_6FDB6C10(pTarget->pStatListEx, i->nLayer_StatId, -i->nValue, pUnit);
		}
	}

	return TRUE;
}
