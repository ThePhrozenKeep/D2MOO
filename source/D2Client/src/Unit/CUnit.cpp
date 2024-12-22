#include <Unit/CUnit.h>
#include <D2Client.h>

#ifdef D2_VERSION_110F
D2CLIENTSTUB(CLIENT_UNITS_InvalidateSelectedItem, 6FAE8F50, void, __fastcall, (D2UnitStrc* pItem));
D2CLIENTSTUB(CLIENT_UNITS_Destroy, 6FB27ED0, void, __fastcall, (D2UnitStrc* pItem));


struct UnitsHashTable {
	static constexpr int BucketsCount = 128;
	using Bucket = D2UnitStrc* [BucketsCount];
	D2UnitStrc* aBuckets[BucketsCount];

	//1.10f: D2Client.0x6FB26C00
	void DestroyUnit(D2UnitGUID nGUID, D2C_UnitTypes nUnitType)
	{
		D2UnitStrc* pPrevUnit = nullptr;
		uint32_t nBucketId = nGUID % BucketsCount;
		for (D2UnitStrc* pUnit = aBuckets[nBucketId];
			pUnit != nullptr;
			pUnit = pUnit->pListNext)
		{
			if (pUnit->dwUnitId == nGUID && pUnit->dwUnitType == nUnitType)
			{
				if (pPrevUnit)
					pPrevUnit->pListNext = pUnit->pListNext;
				else
					aBuckets[nBucketId] = pUnit->pListNext;
				
				if(nUnitType == UNIT_ITEM)
					CLIENT_UNITS_InvalidateSelectedItem_6FAE8F50(pUnit);
				CLIENT_UNITS_Destroy_6FB27ED0(pUnit);
				return;
			}
			pPrevUnit = pUnit;
		}
	}
};

struct UnitsHashTables {
	UnitsHashTable aTables[UNIT_TYPES_COUNT];

	D2UnitStrc* GetClientUnit(D2UnitGUID nGUID, D2C_UnitTypes nUnitType) const
	{
		for (D2UnitStrc* pUnit = aTables[nUnitType].aBuckets[nGUID & 127];
			pUnit != nullptr;
			pUnit = pUnit->pListNext)
		{
			if (pUnit->dwUnitId == nGUID)
			{
				if (pUnit->dwUnitType != nUnitType) {
					FOG_Trace("current unit is %d, table is supposed to be %d\n", pUnit->dwUnitType, nUnitType);
					D2_ASSERT(pUnit->dwUnitType == nUnitType);
					continue;
				}
				return pUnit;
			}
		}
		return nullptr;
	}

};

D2VAR(D2CLIENT, pgpUnitsTable_Unk_6FBBAA00, UnitsHashTables, 0x6FBBAA00 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpUnitsTable_Unk_6FBBB600, UnitsHashTables, 0x6FBBB600 - D2ClientImageBase);

//1.10f: D2Client.0x6FB269F0
D2UnitStrc* __fastcall CUNIT_GetClientUnit(int nGUID, D2C_UnitTypes nUnitType)
{
	return D2CLIENT_pgpUnitsTable_Unk_6FBBAA00->GetClientUnit(nGUID, nUnitType);
}

//1.10f: D2Client.0x6FB26A60
D2UnitStrc* __fastcall CUNIT_GetClientUnitUnk(int nGUID, D2C_UnitTypes nUnitType)
{
	return D2CLIENT_pgpUnitsTable_Unk_6FBBB600->GetClientUnit(nGUID, nUnitType);
}

//1.10f: D2Client.0x6FB26AD0
D2UnitStrc* __fastcall CUNIT_GetOwner(D2UnitStrc* ptUnit)
{
	D2_ASSERT(ptUnit);

	if (!(ptUnit->dwFlagEx & UNITFLAGEX_STOREOWNERINFO))
		return nullptr;

	if (ptUnit->dwOwnerType == UNIT_MISSILE)
	{
		return D2CLIENT_pgpUnitsTable_Unk_6FBBB600->GetClientUnit(ptUnit->dwOwnerGUID, UNIT_MISSILE);
	}
	else
	{
		
		return D2CLIENT_pgpUnitsTable_Unk_6FBBAA00->GetClientUnit(ptUnit->dwOwnerGUID, ptUnit->dwOwnerType);
	}
}

//1.10f: D2Client.0x6FB26BE0
void __fastcall CLIENTS_UNITS_Delete_Unk1(int nUnitGUID, D2C_UnitTypes nUnitType)
{
	D2CLIENT_pgpUnitsTable_Unk_6FBBAA00->aTables[nUnitType].DestroyUnit(nUnitGUID, nUnitType);
}

//1.10f: D2Client.0x6FB26C70
void __fastcall CLIENTS_UNITS_Delete_Unk2(int nUnitGUID, D2C_UnitTypes nUnitType)
{
	D2CLIENT_pgpUnitsTable_Unk_6FBBB600->aTables[nUnitType].DestroyUnit(nUnitGUID, nUnitType);
}



#endif
