#include <Unit/CUnit.h>
#include <D2Client.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2Dungeon.h>
#include <CGAME/Game.h>
#include <Storm.h>

#ifdef D2_VERSION_110F
D2CLIENTSTUB(CLIENT_UNITS_InvalidateSelectedItem, 6FAE8F50, void, __fastcall, (D2UnitStrc* pItem));
D2CLIENTSTUB(CLIENT_UNITS_Destroy, 6FB27ED0, void, __fastcall, (D2UnitStrc* pItem));
D2VAR(D2CLIENT, pgpCurrentAct_6FBA7984, D2DrlgActStrc*, 0x6FBA7984 - D2ClientImageBase); //1.13c:0x6FBCC3B8


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
void __fastcall CUNIT_Delete_Unk1(int nUnitGUID, D2C_UnitTypes nUnitType)
{
	D2CLIENT_pgpUnitsTable_Unk_6FBBAA00->aTables[nUnitType].DestroyUnit(nUnitGUID, nUnitType);
}

//1.10f: D2Client.0x6FB26C70
void __fastcall CUNIT_Delete_Unk2(int nUnitGUID, D2C_UnitTypes nUnitType)
{
	D2CLIENT_pgpUnitsTable_Unk_6FBBB600->aTables[nUnitType].DestroyUnit(nUnitGUID, nUnitType);
}

//1.10f: D2Client.0x6FB26C90
D2ActiveRoomStrc* __fastcall CUNIT_GetRoomAtPosition(int nX, int nY)
{
	char DstBuf[200];

	if (!nX && !nY)
		return 0;

	D2UnitStrc* hControlUnit = D2CLIENT_GetControlUnit();
	D2ActiveRoomStrc* hUnitRoom = nullptr;
	if (hControlUnit)
	{
		if (hUnitRoom = UNITS_GetRoom(hControlUnit))
		{
			if (nX >= hUnitRoom->tCoords.nSubtileX && nX < hUnitRoom->tCoords.nSubtileX + hUnitRoom->tCoords.nSubtileWidth
				&& nY >= hUnitRoom->tCoords.nSubtileY && nY < hUnitRoom->tCoords.nSubtileY + hUnitRoom->tCoords.nSubtileHeight)
			{
				return hUnitRoom;
			}


			D2ActiveRoomStrc** phRoom = 0;
			int nNumRooms = 0;
			DUNGEON_GetAdjacentRoomsListFromRoom(hUnitRoom, &phRoom, &nNumRooms);
			for (int i = 0; i < nNumRooms; ++i)
			{
				D2ActiveRoomStrc* hAdjacentRoom = phRoom[i];
				if (nX >= hAdjacentRoom->tCoords.nSubtileX && nX < hAdjacentRoom->tCoords.nSubtileX + hAdjacentRoom->tCoords.nSubtileWidth
					&& nY >= hAdjacentRoom->tCoords.nSubtileY && nY < hAdjacentRoom->tCoords.nSubtileY + hAdjacentRoom->tCoords.nSubtileHeight)
				{
					return hAdjacentRoom;
				}
			}
		}
	}

	D2ActiveRoomStrc* hFindRoom = DUNGEON_FindRoomBySubtileCoordinates(*D2CLIENT_pgpCurrentAct_6FBA7984, nX, nY);
	if (!hFindRoom)
	{
		SStrPrintf(
			DstBuf,
			0x200u,
			"Unable to find room for unit...  hControlUnit:%8x  hUnitRoom:%8x  ghAct:%8x  nX:%d  nY:%d",
			hControlUnit,
			hUnitRoom,
			*D2CLIENT_pgpCurrentAct_6FBA7984,
			nX,
			nY);
		FOG_DisplayWarning(DstBuf, "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Client\\UNIT\\CUnit.cpp", 314);
		D2_ASSERT(hFindRoom);
	}
	return hFindRoom;
}


#endif
