#pragma once


#include <cstdarg>
#include <cstring>
#include <memory>
#include <utility>

#include <Windows.h>

#include <D2BitManip.h>
#include <D2DataTbls.h>
#include <D2StatList.h>

#include "ItemStatCostTxtFixture.h"


// ItemStatCost.bin only contains the compiled txt columns. The op stat links (bIsBaseOfOtherStatOp, bHasOpStatData, bHasOpApplyingToItem,
// unk0x5E and pOpStatData) are computed after compiling the table (see DATATBLS_LoadItemStatCostTxt), so do the same here.
// Both D2MOO and the original share the same table, so this only has to be done once.
template<class Fixture>
struct ItemStatCostTxtOpStatsFixture : ItemStatCostTxtFixture<Fixture>
{
	ItemStatCostTxtOpStatsFixture()
	{
		auto* const records = this->itemstatcost_txt.get();
		const auto record_count = this->itemstatcost_record_count;

		for (int i = 0; i < record_count; ++i)
		{
			std::memset(records[i].unk0x5E, -1, sizeof(records[i].unk0x5E));
		}

		for (int nStatId = 0; nStatId < record_count; ++nStatId)
		{
			D2ItemStatCostTxt& rCurrentStatRecord = records[nStatId];
			const uint8_t nCurrentStatOp = rCurrentStatRecord.nOp;
			if (nCurrentStatOp <= 0 || nCurrentStatOp >= 14)
			{
				rCurrentStatRecord.nOp = STAT_OP_NONE;
				continue;
			}

			const uint16_t nOpBase = rCurrentStatRecord.wOpBase;
			if (nOpBase < record_count)
			{
				records[nOpBase].bIsBaseOfOtherStatOp = TRUE;

				for (auto& nLinkedStatId : records[nOpBase].unk0x5E)
				{
					if (nLinkedStatId >= record_count)
					{
						nLinkedStatId = nStatId;
						break;
					}
				}

				if (nCurrentStatOp == STAT_OP_APPLY_TO_ITEM || nCurrentStatOp == STAT_OP_APPLY_TO_ITEM_PCT)
				{
					rCurrentStatRecord.bHasOpApplyingToItem = TRUE;
				}
			}

			for (const uint16_t wOpStat : rCurrentStatRecord.wOpStat)
			{
				if (wOpStat >= record_count)
				{
					break;
				}

				rCurrentStatRecord.bIsBaseOfOtherStatOp = TRUE;

				D2ItemStatCostTxt& rCurrentOpStatRecord = records[wOpStat];
				for (auto& rOpStatData : rCurrentOpStatRecord.pOpStatData)
				{
					if (rOpStatData.nOp != STAT_OP_NONE)
					{
						continue;
					}

					rOpStatData.nStat = nStatId;
					rOpStatData.nOp = nCurrentStatOp;
					rOpStatData.nOpBase = nOpBase;
					rOpStatData.nOpParam = rCurrentStatRecord.nOpParam;

					rCurrentOpStatRecord.bHasOpStatData = TRUE;

					if (nStatId == STAT_MAXHP || wOpStat == STAT_MAXHP)
					{
						rCurrentStatRecord.dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_HP];
						rCurrentStatRecord.dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_HP_MANA_STAMINA];
					}
					else if (nStatId == STAT_MAXMANA || wOpStat == STAT_MAXMANA)
					{
						rCurrentStatRecord.dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_MANA];
						rCurrentStatRecord.dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_HP_MANA_STAMINA];
					}
					else if (nStatId == STAT_MAXSTAMINA || wOpStat == STAT_MAXSTAMINA)
					{
						rCurrentStatRecord.dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_STAMINA];
						rCurrentStatRecord.dwItemStatFlags |= gdwBitMasks[ITEMSTATCOSTFLAGINDEX_HP_MANA_STAMINA];
					}
					break;
				}
			}
		}
	}
};
