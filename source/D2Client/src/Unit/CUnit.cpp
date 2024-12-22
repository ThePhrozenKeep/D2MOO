#include <Unit/CUnit.h>
#include <D2Client.h>

struct UnitsHashTable {
	static constexpr int BucketsCount = 128;
	using Bucket = D2UnitStrc* [BucketsCount];
	D2UnitStrc* aBuckets[BucketsCount];
};

struct UnitsHashTables {
	UnitsHashTable aTables[UNIT_TYPES_COUNT];
};

D2VAR(D2CLIENT, pgpUnitsTable_Unk_6FBBAA00, UnitsHashTables, 0x6FBBAA00 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpUnitsTable_Unk_6FBBB600, UnitsHashTables, 0x6FBBB600 - D2ClientImageBase);

//1.10f: D2Client.0x6FB269F0
D2UnitStrc* __fastcall D2CLIENT_GetClientUnit(int nGUID, D2C_UnitTypes nUnitType)
{
	for (D2UnitStrc* pUnit = D2CLIENT_pgpUnitsTable_Unk_6FBBAA00->aTables[nUnitType].aBuckets[nGUID & 127];
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

