#include <UI/npcmenu.h> 
#include <Unit/CUnit.h>
#include <D2Client.h>
#include <D2Items.h>
#include <D2Items.h>
#include <D2QuestRecord.h>
#include <D2StatList.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/StringIds.h>
#include <CGAME/Game.h>
#include <UI/text.h>
#include <QUESTS/Quests.h>

#ifdef D2_VERSION_110F

//1.10f: D2Client.0x6FBB5CF9
//1.13c: D2Client.0x6FBC9721
D2VAR(D2CLIENT, pgbNpcActive, BOOL, 0x6FBB5CF9 - D2ClientImageBase);
//1.10f: D2Client.0x6FBB5CF5
//1.13c: D2Client.0x6FBC971D
D2VAR(D2CLIENT, pgnActiveNpcGUID, D2UnitGUID, 0x6FBB5CF5 - D2ClientImageBase);

D2VAR(D2CLIENT, pgbVendorGamble, BOOL, 0x6FBB5D7C - D2ClientImageBase);

D2VAR(D2CLIENT, pgpQuestHistory, D2BitBufferStrc*, 0x6FBB5D13 - D2ClientImageBase);
D2CLIENTSTUB(D2CLIENT_GetDifficulty, 6FAAC090, D2C_Difficulties, __fastcall, ());
D2CLIENTSTUB(D2CLIENT_CheckInventoryModeIfRepair, 6FAEB930, BOOL, __fastcall, ());

//1.10f: D2Client.0x6FAF3930
//1.13c: D2Client.0x6FAF6150
D2UnitStrc* __fastcall UI_NPC_GetActiveNpcUnit()
{
	if (*D2CLIENT_pgbNpcActive)
		return CUNIT_GetClientUnit(*D2CLIENT_pgnActiveNpcGUID, UNIT_MONSTER);
	else
		return nullptr;
}



bool ITEM_NeedsRepair(D2UnitStrc* pItem)
{
	if (ITEMS_GetItemType(pItem) == ITEMTYPE_GOLD || !ITEMS_IsRepairable(pItem))
		return false;

	if (ITEMS_HasDurability(pItem))
	{
		int32_t nMaxDurability = STATLIST_GetMaxDurabilityFromUnit(pItem);
		if (nMaxDurability && STATLIST_UnitGetStatValue(pItem, STAT_DURABILITY, 0) < nMaxDurability)
		{
			return true;
		}
	}
	if (ITEMS_CheckIfStackable(pItem))
	{
		if (STATLIST_UnitGetStatValue(pItem, STAT_QUANTITY, 0) < ITEMS_GetTotalMaxStack(pItem))
		{
			return true;
		}
	}

	BOOL bHasChargedSkills;
	if (ITEMS_HasUsedCharges(pItem, &bHasChargedSkills) && !bHasChargedSkills)
	{
		return true;
	}

	return false;
}

int NPCMENU_GetActiveNpcClassId()
{
	int32_t nNpcClassId = -1;
	if (*D2CLIENT_pgbNpcActive)
	{
		if (D2UnitStrc* pNpc = CUNIT_GetClientUnit(*D2CLIENT_pgnActiveNpcGUID, UNIT_MONSTER))
		{
			nNpcClassId = pNpc->dwClassId;
		}
	}
	return nNpcClassId;
}

BOOL __fastcall NPCMENU_TransactionCost(D2UnitStrc* pItem, BOOL bIsOwned, int* pValue, Unicode* pBuffer, int nSize)
{
	*pValue = 0;
	if (!pItem || ITEMS_GetItemType(pItem) == ITEMTYPE_GOLD)
		return FALSE;

	int32_t nTransactionCost = 0;
	D2C_StringIndices wszTransactionId;
	if (!bIsOwned)
	{
		D2C_TransactionTypes nTransactionType = TRANSACTIONTYPE_BUY;
		if (*D2CLIENT_pgbVendorGamble) nTransactionType = TRANSACTIONTYPE_GAMBLE;

		nTransactionCost = ITEMS_GetTransactionCost(
			D2CLIENT_GetControlUnit(),
			pItem,
			D2CLIENT_GetDifficulty_6FAAC090(),
			*D2CLIENT_pgpQuestHistory,
			NPCMENU_GetActiveNpcClassId(),
			nTransactionType);
		wszTransactionId = STR_IDX_3329_cost;
	}
	else
	{
		if (*D2CLIENT_pgbNpcActive)
		{
			return FALSE;
		}
		D2UnitStrc* pNpc = CUNIT_GetClientUnit(*D2CLIENT_pgnActiveNpcGUID, UNIT_MONSTER);
		if (!pNpc)
		{
			return FALSE;
		}
		int32_t nNpcClassId = pNpc->dwClassId;
		DATATBLS_GetItemsTxtRecord(pItem->dwClassId);

		switch (nNpcClassId)
		{
		case MONSTER_CHARSI:
		case MONSTER_FARA:
		case MONSTER_HRATLI:
		case MONSTER_HALBU:
		case MONSTER_LARZUK:
			if (D2CLIENT_CheckInventoryModeIfRepair_6FAEB930())
			{
				if (!ITEMS_CHECK_FLAG(pItem, IFLAG_IDENTIFIED))
				{
					Unicode::strcat(pBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_4022_strCannotDoThisToUnknown));
					TEXT_ApplyColorCode(pBuffer, 1);
					return 1;
				}
				if (!ITEM_NeedsRepair(pItem))
				{
					return FALSE;
				}
				nTransactionCost = ITEMS_GetTransactionCost(
					D2CLIENT_GetControlUnit(),
					pItem,
					D2CLIENT_GetDifficulty_6FAAC090(),
					*D2CLIENT_pgpQuestHistory,
					NPCMENU_GetActiveNpcClassId(),
					TRANSACTIONTYPE_REPAIR);
				wszTransactionId = STR_IDX_3330_Repair;
				break;
			}
			else // Sell
			{
				// FALLTHROUGH
			}
		case MONSTER_JAMELLA:
		case MONSTER_DREHYA:
		case MONSTER_MALAH:
		case MONSTER_NIHLATHAK:
		case MONSTER_ALKOR:
		case MONSTER_ORMUS:
		case MONSTER_GHEED:
		case MONSTER_AKARA:
		case MONSTER_DROGNAN:
		case MONSTER_ELZIX:
		case MONSTER_LYSANDER:
			if (!ITEMS_IsNotQuestItem(pItem))
			{
				return FALSE;
			}
			nTransactionCost = ITEMS_GetTransactionCost(
				D2CLIENT_GetControlUnit(),
				pItem,
				D2CLIENT_GetDifficulty_6FAAC090(),
				*D2CLIENT_pgpQuestHistory,
				NPCMENU_GetActiveNpcClassId(),
				TRANSACTIONTYPE_SELL);
			wszTransactionId = STR_IDX_3331_Sell;
			break;
		case MONSTER_CAIN2:
		case MONSTER_CAIN3:
		case MONSTER_CAIN4:
		case MONSTER_CAIN5:
			if (ITEMS_CHECK_FLAG(pItem, IFLAG_IDENTIFIED))
			{
				*pBuffer = L'0';
				return TRUE;
			}
			nTransactionCost = QUESTRECORD_GetQuestState(*D2CLIENT_pgpQuestHistory, QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) ? 0 : 100;
			wszTransactionId = STR_IDX_3332_Identify;
			break;
		default:
			return FALSE;
		}
	}

	*pValue = nTransactionCost;
	const Unicode* wszTransactionName = D2LANG_GetStringFromTblIndex(wszTransactionId);

	if (Unicode::strlen(wszTransactionName) + (_MAX_ULTOSTR_BASE10_COUNT - 1) <= nSize)
	{
		Unicode::strcpy(pBuffer, wszTransactionName);
		char szBuffer[20];
		Unicode wszBuffer[20];
		wsprintfA(szBuffer, "%d", *pValue);
		Unicode::win2Unicode(wszBuffer, szBuffer, ARRAY_SIZE(wszBuffer) - 1);
		Unicode::strcat(pBuffer, wszBuffer);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}





#endif // D2_VERSION_110F
