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


// Helper function
template<class T>
static int __fastcall StatArray_FindInsertionIndex(const T* pStatsArray, int nLayer_StatId, bool* pAlreadyInArray)
{
	*pAlreadyInArray = false;
	// Find by dichotomy, stats are sorted by nLayer_StatId
	int nMin = 0;
	int nMax = pStatsArray->nStatCount;

	while (nMin < nMax)
	{
		const int nMidPoint = nMin + (nMax - nMin) / 2;

		if (nLayer_StatId < pStatsArray->pStat[nMidPoint].nLayer_StatId)
		{
			nMax = nMin + (nMax - nMin) / 2;
		}
		else if (nLayer_StatId > pStatsArray->pStat[nMidPoint].nLayer_StatId)
		{
			nMin = nMidPoint + 1;
		}
		else // found it
		{
			*pAlreadyInArray = true;
			return nMidPoint;
		}
	}

	return nMin;
}

template<class T>
int StatArray_DichotomySearch(const T* pStatArray, int nLayer_StatId)
{
	bool alreadyInArray = false;
	int insertionIdx = StatArray_FindInsertionIndex(pStatArray, nLayer_StatId, &alreadyInArray);
	if (alreadyInArray)
		return insertionIdx;
	return -1;
}

// Helper function
template<class T>
decltype(T::pStat) __fastcall StatArray_InsertStat(void* pMemPool, T* pStatsArray, int nLayer_StatId, int insertionIdx)
{
	typedef decltype(T::pStat) StatType;

	if (pStatsArray->nStatCount >= pStatsArray->nCapacity)
	{
		pStatsArray->nCapacity += T::nGrowthAmount;
		pStatsArray->pStat = (StatType)D2_REALLOC_SERVER(pMemPool, pStatsArray->pStat, pStatsArray->nCapacity * sizeof(*pStatsArray->pStat));
	}

	if (insertionIdx < pStatsArray->nStatCount)
	{
		// Move memory by 1 slot so that we can insert our stat
		memmove(&pStatsArray->pStat[insertionIdx + 1], &pStatsArray->pStat[insertionIdx], (pStatsArray->nStatCount - insertionIdx) * sizeof(D2StatStrc));
	}

	pStatsArray->pStat[insertionIdx] = {}; // Default init to 0
	pStatsArray->pStat[insertionIdx].nLayer_StatId = nLayer_StatId;
	++pStatsArray->nStatCount;

	return &pStatsArray->pStat[insertionIdx];
}

// Helper function
static D2StatStrc* __fastcall STATLIST_GetOrInsertStat(void* pMemPool, D2StatsArrayStrc* pStatsArray, int nLayer_StatId)
{
	bool bFoundStatInArray = false;
	const int insertionIdx = StatArray_FindInsertionIndex(pStatsArray, nLayer_StatId, &bFoundStatInArray);

	if (bFoundStatInArray)
	{
		return &pStatsArray->pStat[insertionIdx];
	}
	else
	{
		return StatArray_InsertStat(pMemPool, pStatsArray, nLayer_StatId, insertionIdx);
	}
}

// Helper function
static void STATLIST_NotifyUnitOfStatValueChange(D2ItemStatCostTxt* pItemStatCostTxtRecord, D2StatListExStrc* pStatListEx, D2UnitStrc* pUnit, int nLayer_StatId, int nPreviousValue, int nNewValue)
{
	if (pStatListEx->fpCallBack && (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FCALLBACK]))
	{
		((void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, int, int, int))pStatListEx->fpCallBack)(pStatListEx->pGame, pStatListEx->pOwner, pUnit, nLayer_StatId, nPreviousValue, nNewValue);
	}
}

//Helper function
static void STATLIST_SetUnitStatNewValue(D2StatListExStrc* pStatListEx, D2StatsArrayStrc* pStatsArray, D2StatStrc* pStat, int nLayer_StatId, int nNewValue, D2ItemStatCostTxt* pItemStatCostTxtRecord, D2UnitStrc* pUnit)
{
	int nPreviousValue = pStat->nValue;
	if (nNewValue != 0 || pItemStatCostTxtRecord->nKeepZero)
	{
		pStat->nValue = nNewValue;
		if (pItemStatCostTxtRecord->bHasOpApplyingToItem)
		{
			pStatListEx->dwFlags |= STATLIST_PERMANENT;
		}
	}
	else // We do not need to keep a value of 0, it's the default if not found
	{
		STATLIST_RemoveStat_6FDB6A30(pStatListEx->pMemPool, pStatsArray, pStat);
	}

	if (nPreviousValue != nNewValue)
	{
		STATLIST_NotifyUnitOfStatValueChange(pItemStatCostTxtRecord, pStatListEx, pUnit, nLayer_StatId, nPreviousValue, nNewValue);
	}
}

// Helper function
static int ComputeStatPercentage(int nValue, D2StatListExStrc* pStatListEx, int opStatId)
{
	int nOpStatValue = sub_6FDB5830(pStatListEx, opStatId << 16);
	if (nOpStatValue)
	{
		return nValue * (nOpStatValue / 100.0);
	}
	return 0;
}

//D2Common.0x6FDB5830
int __fastcall sub_6FDB5830(D2StatListExStrc* pStatListEx, int nLayer_StatId)
{
	int nAccumulatedValue = 0;

	int nStatId = (nLayer_StatId >> 16) & 0xFFFF;

	D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}

	nAccumulatedValue = STATLIST_GetBaseStat_6FDB6340(pStatListEx, nLayer_StatId, 0);

	if (STATLIST_IsExtended(pStatListEx))
	{
		for (D2StatListStrc* i = pStatListEx->pMyLastList; i; i = i->pPrevLink)
		{
			if (!(pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED]) || !(i->dwFlags & 0x40000000))
			{
				nAccumulatedValue += STATLIST_GetTotalStat_6FDB63E0((D2StatListExStrc*)i, nLayer_StatId, 0);
			}
		}
	}

	if (!pItemStatCostTxtRecord->bHasOpStatData)
	{
		return nAccumulatedValue;
	}

	int nPreviousOpStatValue = nAccumulatedValue;

	for (int i = 0; i < ARRAY_SIZE(pItemStatCostTxtRecord->pOpStatData); ++i)
	{
		D2OpStatDataStrc& opStatData = pItemStatCostTxtRecord->pOpStatData[i];
		switch (opStatData.nOp)
		{
		case 0:
			return nAccumulatedValue;

		case 1:

			if (pStatListEx->pOwner)
			{
				if (pStatListEx->pOwner->dwUnitType == UNIT_ITEM)
				{
					nPreviousOpStatValue = 0;
					if (pItemStatCostTxtRecord && pStatListEx->pOwner->pStatListEx)
					{
						nPreviousOpStatValue = STATLIST_GetBaseStat_6FDB6340(pStatListEx->pOwner->pStatListEx, nStatId << 16, pItemStatCostTxtRecord);
					}
				}

				if (nPreviousOpStatValue)
				{
					nAccumulatedValue += ComputeStatPercentage(nPreviousOpStatValue, pStatListEx, opStatData.nStat);
				}
			}
			break;

		case 2:
			if (pStatListEx->dwOwnerType == UNIT_PLAYER || pStatListEx->dwOwnerType == UNIT_MONSTER)
			{
				uint16_t nOpBase = opStatData.nOpBase;
				if (nOpBase != 0xFFFF)
				{
					D2ItemStatCostTxt* pOpBaseItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpBase);
					if (pOpBaseItemStatCostTxtRecord)
					{
						D2StatsArrayStrc* pStatsArray = STATLIST_IsExtended(pStatListEx) ? &pStatListEx->FullStats : &pStatListEx->Stats;
						
						if (D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(pStatsArray, nOpBase << 16))
						{
							int nShiftedVal = pStat->nValue >> pOpBaseItemStatCostTxtRecord->nValShift;
							if (nShiftedVal > 0)
							{
								int nRecursiveValue = sub_6FDB5830(pStatListEx, opStatData.nStat << 16);
								if (nRecursiveValue)
								{
									nAccumulatedValue += (nShiftedVal * nRecursiveValue) >> opStatData.nOpParam;
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
				uint16_t nOpBase = opStatData.nOpBase;
				if (nOpBase != 0xFFFF)
				{
					D2ItemStatCostTxt* pOpBaseItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpBase);
					if (pOpBaseItemStatCostTxtRecord)
					{
						D2StatsArrayStrc* pStatsArray = STATLIST_IsExtended(pStatListEx) ? &pStatListEx->FullStats : &pStatListEx->Stats;

						if (D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(pStatsArray, nOpBase << 16))
						{
							int nShiftedVal = pStat->nValue >> pOpBaseItemStatCostTxtRecord->nValShift;
							if (nShiftedVal > 0)
							{
								int nRecursiveValue = sub_6FDB5830(pStatListEx, opStatData.nStat << 16);
								if (nRecursiveValue)
								{
									int nTemp = (nShiftedVal * nRecursiveValue) >> opStatData.nOpParam;
									nAccumulatedValue += nTemp * nPreviousOpStatValue / 100.0;
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
				uint16_t nOpBase = opStatData.nOpBase;
				if (nOpBase != 0xFFFF)
				{
					D2ItemStatCostTxt* pOpBaseItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpBase);
					if (pOpBaseItemStatCostTxtRecord && pStatListEx->pUnit->pStatListEx)
					{
						int nTmp = STATLIST_GetTotalStat_6FDB63E0(pStatListEx->pUnit->pStatListEx, nOpBase << 16, pOpBaseItemStatCostTxtRecord) >> pOpBaseItemStatCostTxtRecord->nValShift;
						if (nTmp > 0)
						{
							int nRecursiveValue = sub_6FDB5830(pStatListEx, opStatData.nStat << 16);
							if (nRecursiveValue)
							{
								nAccumulatedValue += (nTmp * nRecursiveValue) >> opStatData.nOpParam;
							}
						}
					}
				}
			}
			break;

		case 5:
			if (pStatListEx->pUnit && (pStatListEx->pUnit->dwUnitType == UNIT_PLAYER || pStatListEx->pUnit->dwUnitType == UNIT_MONSTER))
			{
				uint16_t nOpBase = opStatData.nOpBase;
				if (nOpBase != 0xFFFF)
				{
					D2ItemStatCostTxt* pOpBaseItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpBase);
					if (pOpBaseItemStatCostTxtRecord && pStatListEx->pUnit->pStatListEx)
					{
						int nTmp = STATLIST_GetTotalStat_6FDB63E0(pStatListEx->pUnit->pStatListEx, nOpBase << 16, pOpBaseItemStatCostTxtRecord) >> pOpBaseItemStatCostTxtRecord->nValShift;
						if (nTmp > 0)
						{
							int nRecursiveValue = sub_6FDB5830(pStatListEx, opStatData.nStat << 16);
							if (nRecursiveValue)
							{
								int nTemp = (nTmp * nRecursiveValue) >> opStatData.nOpParam;
								nAccumulatedValue += nPreviousOpStatValue * nTemp / 100.0;
							}
						}
					}
				}
			}
			break;

		case 6:
			if (pStatListEx->pOwner && pStatListEx->pOwner->pDrlgAct)
			{
				if (int nTmp = sub_6FDB5830(pStatListEx, opStatData.nStat << 16))
				{
					int nBaseTime = 0;
					int nPeriodOfDay = ENVIRONMENT_GetPeriodOfDayFromAct(pStatListEx->pOwner->pDrlgAct, &nBaseTime);
					nAccumulatedValue += D2COMMON_10843_GetByTimeAdjustment(nTmp, nPeriodOfDay, nBaseTime, NULL, NULL, NULL);
				}
			}
			break;

		case 7:
			if (pStatListEx->pOwner && pStatListEx->pOwner->pDrlgAct)
			{
				if (int nTmp = sub_6FDB5830(pStatListEx, opStatData.nStat << 16))
				{
					int nBaseTime = 0;
					int nPeriodOfDay = ENVIRONMENT_GetPeriodOfDayFromAct(pStatListEx->pOwner->pDrlgAct, &nBaseTime);
					nAccumulatedValue += nAccumulatedValue * D2COMMON_10843_GetByTimeAdjustment(nTmp, nPeriodOfDay, nBaseTime, NULL, NULL, NULL) / 100.0;
				}
			}
			break;

		case 8:
			if (pStatListEx->pOwner && pStatListEx->pOwner->dwUnitType == UNIT_PLAYER)
			{
				if (D2CharStatsTxt* pCharStatsTxtRecord = UNITS_GetCharStatsTxtRecord(pStatListEx->pOwner->dwClassId))
				{
					int nTemp = 0;
					if (D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(&pStatListEx->Stats, opStatData.nStat << 16))
					{
						nTemp = pStat->nValue;
					}

					int nBaseValue = sub_6FDB5830(pStatListEx, opStatData.nStat << 16) - nTemp;
					if (nBaseValue)
					{
						nAccumulatedValue += nBaseValue * pCharStatsTxtRecord->nManaPerMagic << 6;
					}
				}
			}
			break;

		case 9:
			if (pStatListEx->pOwner && pStatListEx->pOwner->dwUnitType == UNIT_PLAYER)
			{
				if (D2CharStatsTxt* pCharStatsTxtRecord = UNITS_GetCharStatsTxtRecord(pStatListEx->pOwner->dwClassId))
				{
					int nBaseStatValue = 0;
					if (D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(&pStatListEx->Stats, opStatData.nStat << 16))
					{
						nBaseStatValue = pStat->nValue;
					}
					
					int nBonusValue = sub_6FDB5830(pStatListEx, opStatData.nStat << 16) - nBaseStatValue;
					if (nBonusValue)
					{
						if (nStatId == STAT_MAXSTAMINA)
						{
							nAccumulatedValue += nBonusValue * pCharStatsTxtRecord->nStaminaPerVitality << 6;
						}
						else
						{
							nAccumulatedValue += nBonusValue * pCharStatsTxtRecord->nLifePerVitality << 6;
						}
					}
				}
			}
			break;

		case 11:
			if (nPreviousOpStatValue)
			{
				if (pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
				{
					nAccumulatedValue += ComputeStatPercentage(nPreviousOpStatValue, pStatListEx, opStatData.nStat);
				}
			}
			break;

		case 13:
			if (pStatListEx->pOwner && pStatListEx->pOwner->dwUnitType == UNIT_ITEM)
			{
				nPreviousOpStatValue = 0;
				if (pStatListEx->pOwner->pStatListEx)
				{
					nPreviousOpStatValue = STATLIST_GetBaseStat_6FDB6340(pStatListEx->pOwner->pStatListEx, nStatId << 16, pItemStatCostTxtRecord);
					
					if (nPreviousOpStatValue)
					{
						nAccumulatedValue += ComputeStatPercentage(nPreviousOpStatValue, pStatListEx, opStatData.nStat);
					}
				}
			}
			break;

		default:
			break;
		}
	}

	return nAccumulatedValue;
}


//D2Common.0x6FDB6300
int __fastcall STATLIST_FindStatIndex_6FDB6300(D2StatsArrayStrc* pStatArray, int nLayer_StatId)
{
	return StatArray_DichotomySearch(pStatArray, nLayer_StatId);
}

// Helper function
static int STATLIST_ApplyMinValue(int nValue, D2ItemStatCostTxt* pItemStatCostTxtRecord, D2StatListExStrc* pStatListEx)
{
	if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_FMIN])
	{
		if (pStatListEx && pStatListEx->pOwner && (pStatListEx->pOwner->dwUnitType == UNIT_PLAYER || pStatListEx->pOwner->dwUnitType == UNIT_MONSTER))
		{
			// Note: looks wrong but this is what the game acutally does.
			if (nValue < pItemStatCostTxtRecord->dwMinAccr)
			{
				nValue = pItemStatCostTxtRecord->dwMinAccr << pItemStatCostTxtRecord->nValShift;
			}
		}
	}
	return nValue;
}

//D2Common.0x6FDB6340
int __fastcall STATLIST_GetBaseStat_6FDB6340(D2StatListStrc* pStatList, int nLayer_StatId, D2ItemStatCostTxt* pItemStatCostTxtRecord)
{
	if (D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(&pStatList->Stats, nLayer_StatId))
	{
		return STATLIST_ApplyMinValue(pStat->nValue, pItemStatCostTxtRecord, STATLIST_StatListExCast(pStatList));
	}
	return 0;
}

//D2Common.0x6FDB63E0
int __fastcall STATLIST_GetTotalStat_6FDB63E0(D2StatListStrc* pStatList, int nLayer_StatId, D2ItemStatCostTxt* pItemStatCostTxtRecord)
{
	D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList);
	D2StatsArrayStrc* pStatArray = pStatListEx ? &pStatListEx->FullStats : &pStatList->Stats;

	if (D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(pStatArray, nLayer_StatId))
	{
		return STATLIST_ApplyMinValue(pStat->nValue, pItemStatCostTxtRecord, pStatListEx);
	}
	return 0;
}

//D2Common.0x6FDB64A0
int __fastcall sub_6FDB64A0(D2StatListExStrc* pStatListEx, int nLayer_StatId, D2ItemStatCostTxt* pItemStatCostTxtRecord, D2UnitStrc* pUnit)
{
	int nNewValue = sub_6FDB5830(pStatListEx, nLayer_StatId);

	if (pItemStatCostTxtRecord->bIsBaseOfOtherStatOp)
	{
		if (nNewValue == 0)
		{
			if (D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(&pStatListEx->FullStats, nLayer_StatId))
			{
				STATLIST_SetUnitStatNewValue(pStatListEx, &pStatListEx->FullStats, pStat, nLayer_StatId, nNewValue, pItemStatCostTxtRecord, pUnit);
			}
		}

		bool bUpdate = TRUE;
		
		for (int nCounter = 0; nCounter < 3; ++nCounter)
		{
			if (pItemStatCostTxtRecord->wOpStat[nCounter] == uint16_t(-1))
			{
				break;
			}

			int nOpStat = pItemStatCostTxtRecord->wOpStat[nCounter];
			int nOpStatLayer_StatId = nOpStat << 16;
			D2ItemStatCostTxt* pOpItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nOpStat);
			D2StatsArrayStrc* pStatsArray = &pStatListEx->FullStats;
			int nOpStatNewValue = sub_6FDB64A0(pStatListEx, nOpStatLayer_StatId, pOpItemStatCostTxtRecord, pUnit);
			
			D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(pStatsArray, nOpStatLayer_StatId);
			if (pStat == nullptr && nOpStatNewValue != 0)
			{
				pStat = STATLIST_InsertStatOrFail_6FDB6970(pStatListEx->pMemPool, pStatsArray, nOpStatLayer_StatId);
			}

			if (pStat == nullptr)
				continue;

			STATLIST_SetUnitStatNewValue(pStatListEx, pStatsArray, pStat, nOpStatLayer_StatId, nOpStatNewValue, pOpItemStatCostTxtRecord, pUnit);

			if (nOpStatNewValue)
			{
				switch (pItemStatCostTxtRecord->nOp)
				{
				case 2: // FALLTHROUGH
				case 3:
					if ((pStatListEx->dwOwnerType == UNIT_PLAYER || pStatListEx->dwOwnerType == UNIT_MONSTER) && pItemStatCostTxtRecord->pOpStatData[nCounter].nOpBase != uint16_t(-1))
					{
						if (!STATLIST_IsExtended(pStatListEx))
						{
							pStatsArray = &pStatListEx->Stats;
						}

						pStat = STATLIST_FindStat_6FDB6920(pStatsArray, pItemStatCostTxtRecord->pOpStatData[nCounter].nOpBase << 16);
						if (pStat && pStat->nValue > 0)
						{
							bUpdate = FALSE;
						}
					}
					break;

				case STAT_OP_APPLY_TO_ITEM: // FALLTHROUGH
				case STAT_OP_APPLY_TO_ITEM_PCT:
					if (pStatListEx->pUnit && (pStatListEx->pUnit->dwUnitType == UNIT_PLAYER || pStatListEx->pUnit->dwUnitType == UNIT_MONSTER))
					{
						int nOpBase = pItemStatCostTxtRecord->pOpStatData[nCounter].nOpBase;
						if (nOpBase != uint16_t(-1))
						{
							if (D2ItemStatCostTxt* pOpItemStatCost = ITEMS_GetItemStatCostTxtRecord(nOpBase))
							{
								int nOpStatValue = STATLIST_GetTotalStat_6FDB63E0(pStatListEx->pUnit->pStatListEx, nOpBase << 16, pOpItemStatCost);
								bUpdate = nOpStatValue <= 0;
							}
						}
					}
					break;

				case STAT_OP_BY_TIME: // FALLTHROUGH
				case STAT_OP_BY_TIME_PCT:
					if (pStatListEx->dwOwnerType == UNIT_PLAYER || pStatListEx->dwOwnerType == UNIT_MONSTER)
					{
						bUpdate = FALSE;
					}
					break;

				case STAT_OP_ADD_ITEM_STAT_PCT:
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
		}

		if (bUpdate && nNewValue)
		{
			STATLIST_UpdateUnitStat_6FDB6AB0(pStatListEx, nLayer_StatId, nNewValue, pItemStatCostTxtRecord, pUnit);

			for (int i = 0; i < ARRAY_SIZE(pItemStatCostTxtRecord->unk0x5E); ++i)
			{
				const int nStatId = pItemStatCostTxtRecord->unk0x5E[i];

				if (nStatId == uint16_t(-1))
				{
					break;
				}

				D2ItemStatCostTxt* pLayerStatCostRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
				const int layerStatId = nStatId << 16;
				const int nLayerStatNewValue = sub_6FDB64A0(pStatListEx, layerStatId, pLayerStatCostRecord, pUnit);
				STATLIST_UpdateUnitStat_6FDB6AB0(pStatListEx, layerStatId, nLayerStatNewValue, pLayerStatCostRecord, pUnit);
			}
		}
		return nNewValue;
	}

	D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(&pStatListEx->FullStats, nLayer_StatId);
	if (pStat == nullptr)
	{
		if (nNewValue != 0)
		{
			pStat = STATLIST_InsertStatOrFail_6FDB6970(pStatListEx->pMemPool, &pStatListEx->FullStats, nLayer_StatId);
		}
		else
		{
			return 0;
		}
	}

	STATLIST_SetUnitStatNewValue(pStatListEx, &pStatListEx->FullStats, pStat, nLayer_StatId, nNewValue, pItemStatCostTxtRecord, pUnit);

	return nNewValue;
}

//D2Common.0x6FDB6920
D2StatStrc* __fastcall STATLIST_FindStat_6FDB6920(D2StatsArrayStrc* pStatArray, int nLayer_StatId)
{
	const int nStatIndex = STATLIST_FindStatIndex_6FDB6300(pStatArray, nLayer_StatId);
	return (nStatIndex >= 0) ? &pStatArray->pStat[nStatIndex] : nullptr;
}

//D2Common.0x6FDB6970
D2StatStrc* __fastcall STATLIST_InsertStatOrFail_6FDB6970(void* pMemPool, D2StatsArrayStrc* pStatsArray, int nLayer_StatId)
{
	bool bFoundStatInArray = false;
	int insertionIdx = StatArray_FindInsertionIndex(pStatsArray, nLayer_StatId, &bFoundStatInArray);
	if (bFoundStatInArray)
	{
		return nullptr;
	}
	return StatArray_InsertStat(pMemPool, pStatsArray, nLayer_StatId, insertionIdx);
}

//D2Common.0x6FDB6A30
void __fastcall STATLIST_RemoveStat_6FDB6A30(void* pMemPool, D2StatsArrayStrc* pStatEx, D2StatStrc* pStat)
{
	int nIndex = pStat - pStatEx->pStat;

	if (nIndex < pStatEx->nStatCount)
	{
		if (nIndex < pStatEx->nStatCount - 1)
		{
			memmove(pStat, pStat + 1, (pStatEx->nStatCount - (nIndex + 1)) * sizeof(D2StatStrc));
		}

		--pStatEx->nStatCount;
		
		D2_ASSERT(pStatEx->nCapacity >= pStatEx->nStatCount);
		if ((pStatEx->nCapacity - pStatEx->nStatCount) > D2StatsArrayStrc::nShrinkThreshold)
		{
			pStatEx->nCapacity -= D2StatsArrayStrc::nGrowthAmount;
			pStatEx->pStat = (D2StatStrc*)D2_REALLOC_SERVER(pMemPool, pStatEx->pStat, pStatEx->nCapacity * sizeof(D2StatStrc));
		}
	}
}

//D2Common.0x6FDB6AB0
void __fastcall STATLIST_UpdateUnitStat_6FDB6AB0(D2StatListExStrc* pStatListEx, int nLayer_StatId, int nNewValue, D2ItemStatCostTxt* pItemStatCostTxtRecord, D2UnitStrc* pUnit)
{
	D2StatsArrayStrc* pStatsArray = &pStatListEx->FullStats;
	bool bFoundStatInArray = false;
	const int insertionIdx = StatArray_FindInsertionIndex(pStatsArray, nLayer_StatId, &bFoundStatInArray);

	D2StatStrc* pStat = nullptr;
	if (bFoundStatInArray)
	{
		pStat = &pStatsArray->pStat[insertionIdx];
	}
	else if (nNewValue != 0)
	{
		pStat = StatArray_InsertStat(pStatListEx->pMemPool, pStatsArray, nLayer_StatId, insertionIdx);
	}

	if (pStat == nullptr)
	{
		return;
	}
	STATLIST_SetUnitStatNewValue(pStatListEx, pStatsArray, pStat, nLayer_StatId, nNewValue, pItemStatCostTxtRecord, pUnit);
}

//D2Common.0x6FDB6C10
void __fastcall sub_6FDB6C10(D2StatListExStrc* pStatListEx, int nLayer_StatId, int nValue, D2UnitStrc* pUnit)
{
	if (!nValue)
	{
		return;
	}

	const int nStatId = (nLayer_StatId >> 16) & 0xFFFF;
	D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord || (pStatListEx->dwFlags & STATLIST_SET))
	{
		return;
	}
	const bool bStatIsDamageRelated = (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED]);

	D2StatListExStrc* pParentStatList = nullptr;
	if (!STATLIST_IsExtended(pStatListEx))
	{
		pParentStatList = static_cast<D2StatListExStrc*>(pStatListEx->pParent);
	}
	else
	{
		pParentStatList = pStatListEx;
	}

	while (pParentStatList)
	{
		if (pItemStatCostTxtRecord->bIsBaseOfOtherStatOp || pItemStatCostTxtRecord->bHasOpStatData)
		{
			sub_6FDB64A0(pParentStatList, nLayer_StatId, pItemStatCostTxtRecord, pUnit);
		}
		else
		{
			D2StatStrc* pStat = STATLIST_GetOrInsertStat(pParentStatList->pMemPool, &pParentStatList->FullStats, nLayer_StatId);

			int nNewValue = pStat->nValue + nValue;
			STATLIST_SetUnitStatNewValue(pParentStatList, &pParentStatList->FullStats, pStat, nLayer_StatId, nNewValue, pItemStatCostTxtRecord, pUnit);
		}
		if ((pParentStatList->dwFlags & STATLIST_SET) || (bStatIsDamageRelated && (pParentStatList->dwFlags & STATLIST_DYNAMIC)))
		{
			break;
		}

		pParentStatList = (D2StatListExStrc*)pParentStatList->pParent;
	}
}

//D2Common.0x6FDB6E30
void __stdcall D2Common_ExpireStatListEx_6FDB6E30(D2StatListStrc* pStatList)
{
	if (!pStatList)
	{
		return;
	}
	D2StatListStrc* pParentStatList = pStatList->pParent;
	if (pParentStatList)
	{
		if (D2StatListExStrc* pParentStatListEx = STATLIST_StatListExCast(pParentStatList))
		{
			// Unlink the stat from the parent list
			if (pParentStatListEx->pMyLastList == pStatList)
			{
				pParentStatListEx->pMyLastList = pStatList->pPrevLink;
			}

			if (pParentStatListEx->pMyStats == pStatList)
			{
				pParentStatListEx->pMyStats = pStatList->pPrevLink;
			}
		}

		pStatList->pParent = NULL;
	}

	// Unlink the stat from the list
	if (pStatList->pNextLink)
	{
		pStatList->pNextLink->pPrevLink = pStatList->pPrevLink;
	}

	if (pStatList->pPrevLink)
	{
		pStatList->pPrevLink->pNextLink = pStatList->pNextLink;
	}

	pStatList->pPrevLink = NULL;
	pStatList->pNextLink = NULL;

	D2UnitStrc* pUnit = pStatList->pUnit;
	pStatList->pUnit = NULL;

	if (!(pStatList->dwFlags & STATLIST_SET))
	{
		D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList);
		D2UnitStrc* pOwner = pStatListEx ? pStatListEx->pOwner : nullptr;
			
		if (pStatListEx && pStatList->dwFlags & STATLIST_PERMANENT)
		{
			int nLayer_StatIds[16] = {};
			int nCounter = 0;
			for (D2StatStrc* i = pStatListEx->FullStats.pStat; i < &pStatListEx->FullStats.pStat[pStatListEx->FullStats.nStatCount]; ++i)
			{
				D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
				if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->bHasOpApplyingToItem)
				{
					nLayer_StatIds[nCounter] = i->nLayer_StatId;
					++nCounter;
				}
			}

			for (int i = 0; i < nCounter; ++i)
			{
				int nStatId = nLayer_StatIds[i] >> 16;
				if (D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId))
				{
					sub_6FDB64A0(pStatListEx, nLayer_StatIds[i], pItemStatCostTxtRecord, 0);
				}
			}
		}

		if (pParentStatList)
		{
			D2StatsArrayStrc* pStatsArray = nullptr;
			if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList))
			{
				pStatsArray = &pStatListEx->FullStats;
			}
			else
			{
				pStatsArray = &pStatList->Stats;
			}

			D2StatListExStrc* pParentStatListEx = STATLIST_StatListExCast(pParentStatList);
			// Something looks wrong, seems like some checks were eluded in D2Common.0x6FDB6C10
			// Or inversely, the check above is useless as pParentStatListEx is in fact always an extended statlist
			// if it exists or when STATLIST_SET is set on the current statlist ?
			// In any case, we add an assert here
			D2_ASSERT(pParentStatListEx);

			if (pStatList->dwFlags & STATLIST_DYNAMIC)
			{
				for (D2StatStrc* i = pStatsArray->pStat; i < &pStatsArray->pStat[pStatsArray->nStatCount]; ++i)
				{
					D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
					if (pItemStatCostTxtRecord && !(pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED]))
					{
						sub_6FDB6C10(pParentStatListEx, i->nLayer_StatId, -i->nValue, pOwner);
					}
				}
			}
			else
			{
				for (D2StatStrc* i = pStatsArray->pStat; i < &pStatsArray->pStat[pStatsArray->nStatCount]; ++i)
				{
					sub_6FDB6C10(pParentStatListEx, i->nLayer_StatId, -i->nValue, pOwner);
				}
			}
		}

		if (pUnit && pStatList->fpStatRemove)
		{
			((void(__fastcall*)(D2UnitStrc*, int, D2StatListStrc*))pStatList->fpStatRemove)(pUnit, pStatList->dwStateNo, pStatList);
		}
	}
}

//D2Common.0x6FDB7030 (#10485)
void __stdcall STATLIST_FreeStatList(D2StatListStrc* pStatList)
{
	if (pStatList && !(pStatList->dwFlags & STATLIST_EXTENDED))
	{
		D2Common_STATLIST_FreeStatListImpl_6FDB7050(pStatList);
	}
}

//D2Common.0x6FDB7050
void __fastcall D2Common_STATLIST_FreeStatListImpl_6FDB7050(D2StatListStrc* pStatList)
{

	D2Common_ExpireStatListEx_6FDB6E30(pStatList);

	if (pStatList->Stats.pStat)
	{
		D2_FREE_SERVER(pStatList->pMemPool, pStatList->Stats.pStat);
	}

	if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList))
	{
		if (D2StatListStrc* pCurrent = pStatListEx->pMyLastList)
		{
			D2StatListStrc* pPrevious = nullptr;
			do
			{
				pPrevious = pCurrent->pPrevLink;
				pCurrent->pParent = nullptr;
				pCurrent->pUnit = nullptr;
				if (!(pCurrent->dwFlags & STATLIST_EXTENDED))
				{
					if (pStatListEx->pMyLastList == pCurrent)
					{
						pStatListEx->pMyLastList = pPrevious;
					}

					D2Common_STATLIST_FreeStatListImpl_6FDB7050((D2StatListExStrc*)pCurrent);

					pPrevious = pStatListEx->pMyLastList;
				}
				pCurrent = pPrevious;
			}
			while (pPrevious);
		}

		if (pStatListEx->FullStats.pStat)
		{
			D2_FREE_SERVER(pStatListEx->pMemPool, pStatListEx->FullStats.pStat);
		}

		if (pStatListEx->ModStats.pStat)
		{
			D2_FREE_SERVER(pStatListEx->pMemPool, pStatListEx->ModStats.pStat);
		}

		D2_FREE_SERVER(pStatListEx->pMemPool, pStatListEx->StatFlags);
	}

	D2_FREE_SERVER(pStatList->pMemPool, pStatList);
}

//D2Common.0x6FDB7110 (#10527)
void __stdcall STATLIST_FreeStatListEx(D2UnitStrc* pUnit)
{
	if (pUnit->pStatListEx)
	{
		if (STATLIST_IsExtended(pUnit->pStatListEx))
		{
			D2Common_STATLIST_FreeStatListImpl_6FDB7050(pUnit->pStatListEx);
		}

		pUnit->pStatListEx = NULL;
	}
}

//D2Common.0x6FDB7140 (#10470)
D2StatListStrc* __stdcall STATLIST_AllocStatList(void* pMemPool, uint32_t fFilter, uint32_t dwTimeout, int nUnitType, int nUnitGUID)
{
	D2StatListStrc* pStatList = D2_CALLOC_STRC_SERVER(pMemPool, D2StatListStrc);

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
	STATLIST_FreeStatListEx(pUnit);

	D2StatListExStrc* pStatListEx = D2_CALLOC_STRC_SERVER(pUnit->pMemoryPool, D2StatListExStrc);

	pStatListEx->pMemPool = pUnit->pMemoryPool;
	pStatListEx->dwOwnerType = pUnit->dwUnitType;
	pStatListEx->dwOwnerId = pUnit->dwUnitId;
	pStatListEx->pOwner = pUnit;
	pStatListEx->fpCallBack = pCallbackFunc;
	pStatListEx->pGame = pGame;
	pStatListEx->dwFlags = (nFlags & STATLIST_BASIC) | STATLIST_EXTENDED;
	pStatListEx->StatFlags = (uint32_t*)D2_CALLOC_SERVER(pUnit->pMemoryPool, 2 * sizeof(uint32_t) * (sgptDataTables->nStatesTxtRecordCount + 31) / 32);

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
			pStatList->dwFlags |= STATLIST_NEWLENGTH;
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
	if (D2StatListExStrc* pUnitStatListEx = STATLIST_StatListExCast(pUnit->pStatListEx))
	{
		D2Common_ExpireStatListEx_6FDB6E30((D2StatListExStrc*)pStatList);
		D2StatListExStrc* pCurrentStatList = pUnitStatListEx;

		while (pCurrentStatList && (pStatList != pCurrentStatList))
		{
			pCurrentStatList = (D2StatListExStrc*)pCurrentStatList->pParent;
		}
		if (pCurrentStatList)
		{
			// pCurStatList already present, skip
			return;
		}


		D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList);
		if (pStatList->dwFlags & STATLIST_TEMPONLY)
		{
			pUnit->pStatListEx->dwFlags |= STATLIST_NEWLENGTH;
		}

		if (pStatList->dwFlags & STATLIST_SET)
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

			D2UnitStrc* pOwner = nullptr;

			if (pStatListEx)
			{
				pOwner = pStatListEx->pOwner;

				int nCounter = 0;
				int nLayer_StatIds[16] = {};
				for (D2StatStrc* i = pStatListEx->FullStats.pStat; i < &pStatListEx->FullStats.pStat[pStatListEx->FullStats.nStatCount]; ++i)
				{
					D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
					if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->bHasOpApplyingToItem)
					{
						nLayer_StatIds[nCounter] = i->nLayer_StatId;
						++nCounter;
					}
				}

				for (int i = 0; i < nCounter; ++i)
				{
					int nStatId = nLayer_StatIds[i] >> 16;
					D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
					if (pItemStatCostTxtRecord)
					{
						sub_6FDB64A0(pStatListEx, nLayer_StatIds[i], pItemStatCostTxtRecord, 0);
					}
				}
			}

			D2StatsArrayStrc* pStatsArray = pStatListEx ? &pStatListEx->FullStats : &pStatList->Stats;
			if (bResetFlag)
			{
				pStatList->dwFlags &= ~STATLIST_DYNAMIC;

				for (D2StatStrc* i = pStatsArray->pStat; i < &pStatsArray->pStat[pStatsArray->nStatCount]; ++i)
				{
					sub_6FDB6C10(pUnitStatListEx, i->nLayer_StatId, i->nValue, pOwner);
				}
			}
			else
			{
				pStatList->dwFlags |= STATLIST_DYNAMIC;

				for (D2StatStrc* i = pStatsArray->pStat; i < &pStatsArray->pStat[pStatsArray->nStatCount]; ++i)
				{
					D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
					if (pItemStatCostTxtRecord && !(pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED]))
					{
						sub_6FDB6C10(pUnitStatListEx, i->nLayer_StatId, i->nValue, pOwner);
					}
				}
			}
		}
	}
}

//D2Common.0x6FDB7560 (#10464)
void __stdcall STATLIST_AddStat(D2StatListStrc* pStatList, int nStatId, int nValue, uint16_t nLayer)
{
	if (!pStatList || !nValue)
	{
		return;
	}

	D2StatsArrayStrc* pStatsArray = &pStatList->Stats;

	D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(pStatsArray, nLayer + (nStatId << 16));

	if (!pStat)
	{
		pStat = STATLIST_InsertStatOrFail_6FDB6970(pStatList->pMemPool, pStatsArray, nLayer + (nStatId << 16));
	}

	pStat->nValue += nValue;

	if (pStat->nValue == 0)
	{
		STATLIST_RemoveStat_6FDB6A30(pStatList->pMemPool, pStatsArray, pStat);
	}

	sub_6FDB6C10((D2StatListExStrc*)pStatList, nLayer + (nStatId << 16), nValue, 0);
	if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList))
	{
		if (pStatList->dwOwnerType == UNIT_PLAYER)
		{
			STATLIST_InsertStatModOrFail_6FDB7690(pStatListEx, nLayer + (nStatId << 16));
		}
	}
}

//D2Common.0x6FDB7690
void __fastcall STATLIST_InsertStatModOrFail_6FDB7690(D2StatListStrc* pStatList, int nLayer_StatId)
{
	if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList))
	{
		int nStatId = (nLayer_StatId >> 16) & 0xFFFF;

		switch (nStatId)
		{
		case STAT_HITPOINTS:
		case STAT_MANA:
		case STAT_STAMINA:
		case STAT_EXPERIENCE:
		case STAT_GOLD:
			return;
		default:
			break;
		}

		D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
		if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_SAVED])
		{
			bool alreadyInArray = false;
			int insertIdx = StatArray_FindInsertionIndex(&pStatListEx->ModStats, nLayer_StatId, &alreadyInArray);
			if (!alreadyInArray)
				StatArray_InsertStat(pStatListEx->pMemPool, &pStatListEx->ModStats, nLayer_StatId, insertIdx);
		}
	}
}

//D2Common.0x6FDB77B0 (#10463)
BOOL __stdcall STATLIST_SetStat(D2StatListStrc* pStatList, int nStatId, int nValue, uint16_t nLayer)
{
	return STATLIST_SetBaseStat(pStatList, nStatId, nValue, nLayer, NULL);
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
BOOL __stdcall STATLIST_SetBaseStat(D2StatListStrc* pStatList, int nStatId, int nValue, uint16_t nLayer, D2UnitStrc* pUnit)
{
	if (!pStatList)
	{
		return FALSE;
	}

	D2StatsArrayStrc* pStatsArray = &pStatList->Stats;
	D2StatStrc* pStat = STATLIST_GetOrInsertStat(pStatList->pMemPool, pStatsArray, nLayer + (nStatId << 16));
	
	const int diffValue = nValue - pStat->nValue;
	if (diffValue == 0)
	{
		return FALSE;
	}

	if (nValue != 0)
	{
		pStat->nValue = nValue;
	}
	else
	{
		STATLIST_RemoveStat_6FDB6A30(pStatList->pMemPool, pStatsArray, pStat);
	}

	sub_6FDB6C10((D2StatListExStrc*)pStatList, nLayer + (nStatId << 16), diffValue, pUnit);
	if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList))
	{
		if (pStatList->dwOwnerType == UNIT_PLAYER)
		{
			STATLIST_InsertStatModOrFail_6FDB7690(pStatListEx, nLayer + (nStatId << 16));
		}
	}

	return TRUE;

}

//D2Common.0x6FDB7A90 (#11295)
void __stdcall STATLIST_SetBaseStat2(D2StatListStrc* pStatList, int nStatId, int nValue, uint16_t nLayer, D2UnitStrc* pUnit)
{
	if (pStatList)
	{
		STATLIST_SetBaseStat(pStatList, nStatId, nValue, nLayer, pUnit);
	}
}

//D2Common.0x6FDB7AB0 (#10517)
void __stdcall STATLIST_SetUnitStat(D2UnitStrc* pUnit, int nStatId, int nValue, uint16_t nLayer)
{
	if (STATLIST_SetStat(pUnit->pStatListEx, nStatId, nValue, nLayer))
	{
		if (pUnit->dwUnitType == UNIT_PLAYER)
		{
			STATLIST_InsertStatModOrFail_6FDB7690(pUnit->pStatListEx, nLayer + (nStatId << 16));
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
	if (!pUnit || !pUnit->pStatListEx)
	{
		return 0;
	}
	D2StatListStrc* pStatList = pUnit->pStatListEx;

	D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}

	return STATLIST_GetBaseStat_6FDB6340(pStatList, nLayer + (nStatId << 16), pItemStatCostTxtRecord);
}

// Helper function
static uint32_t __stdcall STATLIST_GetStatUnsigned(D2StatListStrc* pStatList, int nStatId, uint16_t nLayer)
{
	if (!pStatList)
	{
		return 0;
	}

	D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}
	return STATLIST_GetTotalStat_6FDB63E0(pStatList, nLayer + (nStatId << 16), pItemStatCostTxtRecord);
}

//D2Common.0x6FDB7C30 (#10519)
uint32_t __stdcall STATLIST_GetUnitStatUnsigned(const D2UnitStrc* pUnit, int nStatId, uint16_t nLayer)
{
	return STATLIST_GetStatUnsigned(pUnit->pStatListEx, nStatId, nLayer);
}


//D2Common.0x6FDB7E30 (#10520)
int32_t __stdcall STATLIST_GetUnitStatSigned(D2UnitStrc* pUnit, int nStatId, uint16_t nLayer)
{
	return static_cast<int32_t>(STATLIST_GetUnitStatUnsigned(pUnit, nStatId, nLayer));
}


//D2Common.0x(6FDB7D40 (#10466)
int __stdcall STATLIST_GetStatValue(D2StatListStrc* pStatList, int nStatId, uint16_t nLayer)
{
	if (!pStatList)
	{
		return 0;
	}

	D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);
	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}

	if (D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(&pStatList->Stats,nLayer + (nStatId << 16)))
	{
		return STATLIST_ApplyMinValue(pStat->nValue, pItemStatCostTxtRecord, STATLIST_StatListExCast(pStatList));
	}

	return 0;
}


//D2Common.0x6FDB7F40 (#10522)
int __stdcall STATLIST_GetUnitStatBonus(D2UnitStrc* pUnit, int nStatId, uint16_t nLayer)
{


	if (!pUnit->pStatListEx)
	{
		return 0;
	}
	D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId);

	if (!pItemStatCostTxtRecord)
	{
		return 0;
	}

	D2StatListStrc* pStatList = pUnit->pStatListEx;
	D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList);

	int nValue = 0;
	D2StatsArrayStrc* pStatsArray = pStatListEx ? &pStatListEx->FullStats : &pStatList->Stats;
	if (const D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(pStatsArray, nLayer + (nStatId << 16)))
	{
		nValue = STATLIST_ApplyMinValue(pStat->nValue, pItemStatCostTxtRecord, pStatListEx);
	}

	D2StatsArrayStrc* pBaseStatsArray = &pStatList->Stats;
	if (const D2StatStrc* pStat = STATLIST_FindStat_6FDB6920(pBaseStatsArray, nLayer + (nStatId << 16)))
	{
		int nBaseValue = STATLIST_ApplyMinValue(pStat->nValue, pItemStatCostTxtRecord, pStatListEx);
		return nValue - nBaseValue;
	}

	return nValue;
}

//D2Common.0x6FDB80C0 (#10515)
void __stdcall D2Common_10515(D2UnitStrc* pUnit)
{
	D2StatListStrc* pStatListEx = NULL;
	D2StatListStrc* pPrevious = NULL;

	if (pUnit->pStatListEx && pUnit->pStatListEx->dwFlags & STATLIST_NEWLENGTH)
	{
		if (D2StatListStrc* pCurStatListEx = pUnit->pStatListEx->pMyLastList)
		{
			do
			{
				pPrevious = pCurStatListEx->pPrevLink;
				if (pCurStatListEx->dwFlags & STATLIST_TEMPONLY)
				{
					if (pCurStatListEx->dwStateNo)
					{
						STATES_ToggleState(pUnit, pCurStatListEx->dwStateNo, FALSE);
					}

					if (!(pCurStatListEx->dwFlags & STATLIST_EXTENDED))
					{
						D2Common_STATLIST_FreeStatListImpl_6FDB7050((D2StatListExStrc*)pCurStatListEx);
					}

					pPrevious = pUnit->pStatListEx->pMyLastList;
				}
				pCurStatListEx = pPrevious;
			}
			while (pPrevious);
		}

		pUnit->pStatListEx->dwFlags &= ~STATLIST_NEWLENGTH;
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
	if (pStatListEx && nStateId)
	{
		D2StatListStrc* pStatList = STATLIST_IsExtended(pStatListEx) ? pStatListEx->pMyLastList : nullptr;
		while (pStatList && pStatList->dwStateNo != nStateId)
		{
			pStatList = pStatList->pPrevLink;
		}

		if (pStatList)
		{
			return pStatList;
		}

		pStatList = pStatListEx->pMyStats;
		while (pStatList && pStatList->dwStateNo != nStateId)
		{
			pStatList = pStatList->pPrevLink;
		}
		return pStatList;
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
	if (!pStatList)
	{
		return nullptr;
	}

	pStatList = pStatList->pPrevLink;
	while (pStatList && !(nFlag & pStatList->dwFlags))
	{
		pStatList = pStatList->pPrevLink;
	}
	return pStatList;
}

//D2Common.0x6FDB8230 (#10481)
D2StatListStrc* __stdcall STATLIST_GetStatListFromUnitAndFlag(D2UnitStrc* pUnit, int nFlag)
{
	if (pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
	{
		D2StatListStrc* pStatList = pStatList = nFlag & STATLIST_SET ? pUnit->pStatListEx->pMyStats : pUnit->pStatListEx->pMyLastList;
		nFlag &= ~STATLIST_SET;

		while (pStatList && !(nFlag & pStatList->dwFlags))
		{
			pStatList = pStatList->pPrevLink;
		}

		return pStatList;
	}

	return NULL;
}

//D2Common.0x6FDB8270 (#10483)
D2StatListStrc* __stdcall STATLIST_GetStatListFromUnitStateOrFlag(D2UnitStrc* pUnit, int nState, int nFlag)
{
	if (pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
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
	if (pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
	{
		D2StatListStrc* pStatList = nFlag & STATLIST_SET ? pUnit->pStatListEx->pMyStats : pUnit->pStatListEx->pMyLastList;
		nFlag &= ~STATLIST_SET;
		while (pStatList && (pStatList->dwStateNo != nState || nFlag && !(nFlag & pStatList->dwFlags)))
		{
			pStatList = pStatList->pPrevLink;
		}

		return pStatList;
	}

	return NULL;
}

//D2Common.0x6FDB8310 (#10523)
void __stdcall STATLIST_MergeStatLists(D2UnitStrc* pTarget, D2UnitStrc* pUnit, BOOL bType)
{
	if (pTarget && pUnit && pUnit->pStatListEx)
	{
		if (pUnit->dwUnitType == UNIT_ITEM && (ITEMS_GetBodyLocation(pUnit) == BODYLOC_SWRARM || ITEMS_GetBodyLocation(pUnit) == BODYLOC_SWLARM))
		{
			D2Common_ExpireStatListEx_6FDB6E30(pUnit->pStatListEx);
		}
		else if (pUnit->pStatListEx->pUnit == pTarget)
		{
			if (bType && (pUnit->pStatListEx->dwFlags & STATLIST_DYNAMIC))
			{
				D2Common_11274(pTarget, pUnit);
			}
			else if (!bType && !(pUnit->pStatListEx->dwFlags & STATLIST_DYNAMIC))
			{
				D2Common_11275(pTarget, pUnit);
			}
		}
		else
		{
			D2COMMON_10475_PostStatToStatList(pTarget, pUnit->pStatListEx, bType);
			pUnit->pStatListEx->pUnit = pTarget;
		}
	}
}


//D2Common.0x6FDB83A0 (#10535)
D2UnitStrc* __stdcall STATLIST_GetOwner(D2UnitStrc* pUnit, BOOL* pStatNotDynamic)
{
	if (!pUnit || !pUnit->pStatListEx || !pUnit->pStatListEx->pParent || !STATLIST_IsExtended(pUnit->pStatListEx->pParent))
	{
		if (pStatNotDynamic)
		{
			*pStatNotDynamic = FALSE;
		}
		return NULL;
	}

	if (pStatNotDynamic)
	{
		*pStatNotDynamic = !(pUnit->pStatListEx->dwFlags & STATLIST_DYNAMIC);
	}
	return ((D2StatListExStrc*)pUnit->pStatListEx->pParent)->pOwner;
}

//D2Common.0x6FDB8420 (#10512)
void __stdcall D2Common_10512(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int nStatId, void (__fastcall* pfCallback)(D2UnitStrc*, int, int, D2UnitStrc*))
{
	if (!pUnit1 || !pUnit1->pStatListEx || !STATLIST_IsExtended(pUnit1->pStatListEx))
	{
		return;
	}

	const int nLayerId = nStatId << 16;

	if (StatArray_DichotomySearch(&pUnit1->pStatListEx->ModStats, nLayerId) < 0)
	{
		return;
	}

	if (D2StatStrc* pBaseStat = STATLIST_FindStat_6FDB6920(&pUnit1->pStatListEx->Stats, nLayerId))
	{
		pfCallback(pUnit1, nStatId, pBaseStat->nValue, pUnit2);
	}
}

//D2Common.0x6FDB84E0 (#10513)
void __stdcall D2Common_10513(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, void (__fastcall* pfCallback)(D2UnitStrc*, int, int, D2UnitStrc*))
{
	if (!pUnit1 || (!pUnit2 && pfCallback) || !pUnit1->pStatListEx || !STATLIST_IsExtended(pUnit1->pStatListEx))
	{
		return;
	}
	D2StatListExStrc* pUnit1StatList = pUnit1->pStatListEx;
	for (int i = 0; i < pUnit1StatList->ModStats.nStatCount; ++i)
	{
		const D2SLayerStatIdStrc* pModStat = &pUnit1StatList->ModStats.pStat[i];
		int nBaseStatValue = 0;
		if (D2StatStrc* pBaseStat = STATLIST_FindStat_6FDB6920(&pUnit1StatList->Stats, pModStat->nLayer_StatId))
		{
			nBaseStatValue = pBaseStat->nValue;
		}

		pfCallback(pUnit1, pModStat->nStat, nBaseStatValue, pUnit2);
	}
}

//D2Common.0x6FDB85D0 (#10511)
void __stdcall STATLIST_FreeModStats(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
	{
		if (pUnit->pStatListEx->ModStats.pStat)
		{
			D2_FREE_SERVER(pUnit->pStatListEx->pMemPool, pUnit->pStatListEx->ModStats.pStat);
			pUnit->pStatListEx->ModStats.pStat = NULL;
		}

		pUnit->pStatListEx->ModStats.nCapacity = 0;
		pUnit->pStatListEx->ModStats.nStatCount = 0;
	}
}

//D2Common.0x6FDB8620 (#10562)
int __stdcall STATLIST_GetUnitAlignment(D2UnitStrc* pUnit)
{
	if (!pUnit || !pUnit->pStatListEx)
	{
		return 0;
	}

	if (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER)
	{
		return 2;
	}

	if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pUnit->pStatListEx))
	{
		if (D2StatListStrc* pAlignmentStateList = D2Common_GetStateFromStatListEx_6FDB8190(pUnit->pStatListEx, STATE_ALIGNMENT))
		{
			return STATLIST_GetStatUnsigned_Layer0((D2StatListExStrc*)pAlignmentStateList, STAT_ALIGNMENT);
		}
	}

	return 0;
}

//D2Common.0x6FDB8750 (#10534)
void __stdcall D2Common_10534(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx)
	{
		pUnit->pStatListEx->dwFlags |= STATLIST_UNK_0x100;
	}
}

//D2Common.0x6FDB8770 (#10530)
BOOL __stdcall D2COMMON_10530_D2CheckStatlistFlagDMGRed(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx)
	{
		return pUnit->pStatListEx->dwFlags & STATLIST_UNK_0x100;
	}

	return FALSE;
}

//D2Common.0x6FDB87A0 (#10532)
int __stdcall STATLIST_GetStatUnsigned_Layer0(D2StatListExStrc* pStatListEx, int nStatId)
{
	return STATLIST_GetStatUnsigned(pStatListEx, nStatId, 0);
}

//D2Common.0x6FDB8890 (#10533)
void __stdcall STATLIST_RemoveAllStatsFromOverlay(D2UnitStrc* pUnit)
{

	if (pUnit && pUnit->pStatListEx)
	{
		pUnit->pStatListEx->dwFlags &= ~STATLIST_UNK_0x100;

		if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pUnit->pStatListEx))
		{
			D2StatListStrc* pCurStatList = pStatListEx->pMyLastList;
			while (pCurStatList && !(pCurStatList->dwFlags & STATLIST_OVERLAY))
			{
				pCurStatList = pCurStatList->pPrevLink;
			}
			if(pCurStatList)
			{
				STATLIST_RemoveAllStats(pCurStatList);
			}
		}
	}
}

//D2Common.0x6FDB8900
void __stdcall D2Common_STATES_ToggleState_6FDB8900(D2UnitStrc* pUnit, int nState, BOOL bSet)
{
	
	if (pUnit && pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
	{
		const uint32_t dwStateBitMask = gdwBitMasks[nState % 32];
		const bool bStateBitChanged = BITMANIP_SetBitsValueForMask(pUnit->pStatListEx->StatFlags[nState / 32], dwStateBitMask, bSet);
		if (!bStateBitChanged)
		{
			return;
		}
		const size_t states2Offset = (sgptDataTables->nStatesTxtRecordCount + 31) / 32;
		pUnit->pStatListEx->StatFlags[nState / 32 + states2Offset] |= dwStateBitMask;

		if (D2StatesTxt* pStatesTxtRecord = DATATBLS_GetStatesTxtRecord(nState))
		{
			bool bStateMaskDisguise = pStatesTxtRecord->dwStateFlags & gdwBitMasks[STATEMASK_DISGUISE];
			if (bSet && bStateMaskDisguise)
			{
				pUnit->dwFlagEx |= UNITFLAGEX_ISSHAPESHIFTED;
			}
			else if (bStateMaskDisguise  && !STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_DISGUISE))
			{
				pUnit->dwFlagEx &= ~UNITFLAGEX_ISSHAPESHIFTED;
			}
		}
	}
}

//D2Common.0x6FDB8A90
uint32_t* __stdcall D2COMMON_STATES_GetStatFlags_6FDB8A90(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
	{
		return pUnit->pStatListEx->StatFlags;
	}

	return NULL;
}

//D2Common.0x6FDB8AC0
uint32_t* __stdcall D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
	{
		return &pUnit->pStatListEx->StatFlags[(sgptDataTables->nStatesTxtRecordCount + 31) / 32];
	}

	return NULL;
}

//D2Common.0x6FDB8B10 (#10516)
void __stdcall STATLIST_UpdateStatListsExpiration(D2UnitStrc* pUnit, int nFrame)
{
	if (pUnit && pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
	{
		if (D2StatListStrc* pStatListEx = pUnit->pStatListEx->pMyLastList)
		{
			D2StatListStrc* pPrevious = nullptr;
			do
			{
				pPrevious = pStatListEx->pPrevLink;
				if (pStatListEx->dwFlags & STATLIST_NEWLENGTH)
				{
					if (nFrame == 0) // Client, the server only syncs
					{
						pStatListEx->dwExpireFrame--;
					}
					if (pStatListEx->dwExpireFrame <= nFrame)
					{
						if (!STATLIST_IsExtended(pStatListEx))
						{
							D2Common_STATLIST_FreeStatListImpl_6FDB7050(pStatListEx);
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


// Helper function
int __stdcall STATLIST_CopyStatsData(D2StatsArrayStrc* pStatsArray, D2StatStrc* pOutStatBuffer, int nBufferSize)
{
	int nStatCount = pStatsArray->nStatCount;
	if (nStatCount >= nBufferSize)
	{
		nStatCount = nBufferSize;
	}

	for(int nCounter = 0; nCounter < nStatCount; ++nCounter)
	{
		pOutStatBuffer[nCounter].nLayer = pStatsArray->pStat[nCounter].nLayer;
		pOutStatBuffer[nCounter].nStat  = pStatsArray->pStat[nCounter].nStat;
		pOutStatBuffer[nCounter].nValue = pStatsArray->pStat[nCounter].nValue;
	}

	return nStatCount;
}

//D2Common.0x6FDB8BA0 (#11268)
int __stdcall STATLIST_GetFullStatsDataFromUnit(D2UnitStrc* pUnit, D2StatStrc* pOutStatBuffer, int nBufferSize)
{
	if (pUnit && pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
	{
		return STATLIST_CopyStatsData(&pUnit->pStatListEx->FullStats, pOutStatBuffer, nBufferSize);
	}
	return 0;
}

//D2Common.0x6FDB8C00 (#11243)
int __stdcall STATLIST_GetBaseStatsData(D2StatListStrc* pStatListEx, D2StatStrc* pOutStatBuffer, int nBufferSize)
{
	if (pStatListEx)
	{
		return STATLIST_CopyStatsData(&pStatListEx->Stats, pOutStatBuffer, nBufferSize);
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
	if (pUnit && pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
	{
		if (D2StatListStrc* pStatListEx = pUnit->pStatListEx->pMyLastList)
		{
			D2StatListStrc* pPrevious = NULL;
			do
			{
				pPrevious = pStatListEx->pPrevLink;
				if (pStatListEx->dwOwnerType != UNIT_ITEM 
					&& !(pStatListEx->dwFlags & (STATLIST_BASIC|STATLIST_OVERLAY|STATLIST_UNK_0x100)) 
					&& !STATES_CheckStateMaskStayDeathOnUnitByStateId(pUnit, pStatListEx->dwStateNo))
				{
					if (!STATLIST_IsExtended(pStatListEx))
					{
						D2Common_STATLIST_FreeStatListImpl_6FDB7050(pStatListEx);
					}

					pPrevious = pUnit->pStatListEx->pMyLastList;
				}
				pStatListEx = pPrevious;
			}
			while (pPrevious);
		}
	}
}

// Helper function
static void STATLIST_ClampStat(D2StatListExStrc* pStatListEx, int nStatId)
{
	int nCurrentValue = 0;
	if (D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nStatId))
	{
		nCurrentValue = STATLIST_GetTotalStat_6FDB63E0(pStatListEx, nStatId << 16, pItemStatCostTxtRecord);
	}

	const int nMaxStatId = nStatId + 1;
	int nMaxValue = 0;
	if (D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(nMaxStatId))
	{
		nMaxValue = STATLIST_GetTotalStat_6FDB63E0(pStatListEx, nMaxStatId << 16, pItemStatCostTxtRecord);
	}

	if (nCurrentValue > nMaxValue)
	{
		STATLIST_AddStat(pStatListEx, nStatId, nMaxValue - nCurrentValue, 0);
	}
}

//D2Common.0x6FDB8D30 (#10514)
void __stdcall STATLIST_ClampStaminaManaHP(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pStatListEx)
	{
		STATLIST_ClampStat(pUnit->pStatListEx, STAT_STAMINA);
		STATLIST_ClampStat(pUnit->pStatListEx, STAT_MANA);
		STATLIST_ClampStat(pUnit->pStatListEx, STAT_HITPOINTS);
	}
}

//D2Common.0x6FDB8EB0 (#10574)
BOOL __stdcall D2Common_10574(D2UnitStrc* pUnit, int nStateId, BOOL bSet)
{

	if (nStateId && pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
	{
		if (D2StatListStrc* pStatList = D2Common_GetStateFromStatListEx_6FDB8190(pUnit->pStatListEx, nStateId))
		{
			if (bSet)
			{
				if (!(pStatList->dwFlags & STATLIST_SET))
				{
					D2Common_ExpireStatListEx_6FDB6E30((D2StatListExStrc*)pStatList);
					pStatList->dwFlags |= STATLIST_SET;
					D2COMMON_10475_PostStatToStatList(pUnit, pStatList, TRUE);
				}
			}
			else
			{
				if (pStatList->dwFlags & STATLIST_SET)
				{
					D2Common_ExpireStatListEx_6FDB6E30((D2StatListExStrc*)pStatList);
					pStatList->dwFlags &= ~STATLIST_SET;
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
	if (!pUnit1 || !pUnit2 || !(pUnit1->dwUnitType == UNIT_PLAYER || pUnit1->dwUnitType == UNIT_MONSTER))
	{
		return;
	}
	D2StatListExStrc* pStatListEx2 = pUnit2->pStatListEx ? STATLIST_StatListExCast(pUnit2->pStatListEx) : nullptr;
	if (!pStatListEx2)
	{
		return;
	}

	for (D2StatStrc* j = pStatListEx2->FullStats.pStat; j < &pStatListEx2->FullStats.pStat[pStatListEx2->FullStats.nStatCount]; ++j)
	{
		D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(j->nStat);
		if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->nOp >= 6 && pItemStatCostTxtRecord->nOp <= 7)
		{
			for (uint16_t wOpStat : pItemStatCostTxtRecord->wOpStat)
			{
				if (wOpStat == uint16_t(-1))
				{
					break;
				}

				int nOpLayer_StatId = wOpStat << 16;
				D2StatListExStrc* pUnit1StatListEx = pUnit1->pStatListEx;
				int nOpStatValue = sub_6FDB5830(pUnit1StatListEx, nOpLayer_StatId);

				D2StatStrc* pUnit1Stat = STATLIST_FindStat_6FDB6920(&pUnit1StatListEx->FullStats, nOpLayer_StatId);
				if (!pUnit1Stat && nOpStatValue != 0)
				{
					pUnit1Stat = STATLIST_InsertStatOrFail_6FDB6970(pUnit1StatListEx->pMemPool, &pUnit1StatListEx->FullStats, nOpLayer_StatId);
				}
				if (pUnit1Stat)
				{
					STATLIST_SetUnitStatNewValue(pUnit1StatListEx, &pUnit1StatListEx->FullStats, pUnit1Stat, nOpLayer_StatId, nOpStatValue, ITEMS_GetItemStatCostTxtRecord(wOpStat), pUnit1);
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
		return STATLIST_GetStatUnsigned_Layer0(pUnit->pStatListEx, nStatId);
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
int __fastcall D2Common_CopyStats_6FDB9C50(D2StatListStrc* pStatList, int nStatId, D2StatStrc* pBuffer, int nBufferSize)
{
	if (!pStatList)
	{
		return 0;
	}

	D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pStatList);
	D2StatsArrayStrc* pStatArray = pStatListEx ? &pStatListEx->FullStats : &pStatList->Stats;
	int nStatIndex = STATLIST_FindStatIndex_6FDB6300(pStatArray, nStatId << 16);
	if (nStatIndex < 0)
	{
		return 0;
	}

	int nCopiedStats = 0;
	while (nStatIndex < pStatArray->nStatCount
		&& pStatArray->pStat[nStatIndex].nStat == nStatId
		&& nCopiedStats < nBufferSize
		)
	{
		pBuffer[nCopiedStats].nLayer_StatId = pStatArray->pStat[nStatIndex].nLayer_StatId;
		pBuffer[nCopiedStats].nValue = pStatArray->pStat[nStatIndex].nValue;

		++nCopiedStats;
		++nStatIndex;
	}

	return nCopiedStats;
}

//D2Common.0x6FDB9D20 (#11270)
int __stdcall D2Common_11270(D2UnitStrc* pUnit, int nStatId, D2StatStrc* pBuffer, int nBufferSize)
{
	if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pUnit->pStatListEx))
	{
		return D2Common_CopyStats_6FDB9C50(pStatListEx, nStatId, pBuffer, nBufferSize);
	}
	return 0;
}

//D2Common.0x6FDB9D60 (#11273)
int __stdcall D2Common_11273(D2UnitStrc* pUnit, int nStatId)
{
	if (D2StatListExStrc* pStatListEx = STATLIST_StatListExCast(pUnit->pStatListEx))
	{
		return sub_6FDB5830(pStatListEx, nStatId << 16);
	}
	return 0;
}


// Helper function
BOOL __stdcall D2Common_11274_11275_Impl(D2UnitStrc* pTarget, D2UnitStrc* pUnit, bool addOrSubstract)
{

	if (!pTarget || !pUnit || !pUnit->pStatListEx)
	{
		return FALSE;
	}

	if (pUnit->pStatListEx->pUnit != pTarget)
	{
		STATLIST_MergeStatLists(pTarget, pUnit, addOrSubstract);
		return FALSE;
	}

	if (!(pTarget->pStatListEx && STATLIST_IsExtended(pTarget->pStatListEx)))
	{
		return FALSE;
	}

	bool bDynamicBitChanged = BITMANIP_SetBitsValueForMask(pUnit->pStatListEx->dwFlags, STATLIST_DYNAMIC, !addOrSubstract);
	if (!bDynamicBitChanged)
	{
		return FALSE;
	}

	D2StatListExStrc* pUnitStatListEx = STATLIST_StatListExCast(pUnit->pStatListEx);
	D2StatsArrayStrc* pStatsArray = pUnitStatListEx ? &pUnitStatListEx->FullStats : &pUnit->pStatListEx->Stats;

	for (D2StatStrc* i = pStatsArray->pStat; i < &pStatsArray->pStat[pStatsArray->nStatCount]; ++i)
	{
		D2ItemStatCostTxt* pItemStatCostTxtRecord = ITEMS_GetItemStatCostTxtRecord(i->nStat);
		if (pItemStatCostTxtRecord && (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_DAMAGERELATED]))
		{
			const int nValue = addOrSubstract ? i->nValue : -i->nValue;
			sub_6FDB6C10(pTarget->pStatListEx, i->nLayer_StatId, nValue, pUnit);
		}
	}

	return TRUE;
}

//D2Common.0x6FDB9D90 (#11274)
BOOL __stdcall D2Common_11274(D2UnitStrc* pTarget, D2UnitStrc* pUnit)
{
	return D2Common_11274_11275_Impl(pTarget, pUnit, true);
}

//D2Common.0x6FDB9E60 (#11275)
BOOL __stdcall D2Common_11275(D2UnitStrc* pTarget, D2UnitStrc* pUnit)
{
	return D2Common_11274_11275_Impl(pTarget, pUnit, false);
}
