#include <CGAME/Game.h>
#include <D2Client.h>
#include <UI/inv.h>
#include <UI/UI.h>
#include <Units/Units.h>
#include <DataTbls/InvTbls.h>
#include <Unit/CUnit.h>
#include <Engine/Cursor.h>
#include <Storm.h>

#ifdef D2_VERSION_110F

D2CLIENTSTUB(GetExpansionFlag, 6FAAC080, BOOL, __fastcall, ());
D2CLIENTSTUB(D2CLIENT_NPC_GetActivatedNpcGUID, 6FAFBB50, BOOL, __fastcall, (D2UnitGUID* pGUID));
D2CLIENTSTUB(sub, 6FAF9E60, void, __fastcall, ());

D2VAR(D2CLIENT, pgbIsHigherResolution, BOOL, 0x6FB9A708 - D2ClientImageBase);
D2VAR(D2CLIENT, pgnInventoryMode, BOOL, 0x6FBB58EC - D2ClientImageBase);


D2VAR(D2CLIENT, pgtCharInvRect,				D2InvRectStrc,				0x6FBB38C8 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCharInvGridInfo,			D2InventoryGridInfoStrc,	0x6FBB3710 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtCharInvCompGrid_RARM,	D2InvCompGridStrc,			0x6FBB59A8 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtCharInvCompGrid_TORSO,	D2InvCompGridStrc,			0x6FBB5994 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtCharInvCompGrid_LARM,	D2InvCompGridStrc,			0x6FBB59BC - D2ClientImageBase);
D2VAR(D2CLIENT, pgtCharInvCompGrid_HEAD,	D2InvCompGridStrc,			0x6FBB596C - D2ClientImageBase);
D2VAR(D2CLIENT, pgtCharInvCompGrid_NECK,	D2InvCompGridStrc,			0x6FBB5980 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtCharInvCompGrid_RHAND,	D2InvCompGridStrc,			0x6FBB59D0 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtCharInvCompGrid_LHAND,	D2InvCompGridStrc,			0x6FBB59E4 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtCharInvCompGrid_BELT,	D2InvCompGridStrc,			0x6FBB59F8 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtCharInvCompGrid_FEET,	D2InvCompGridStrc,			0x6FBB5A0C - D2ClientImageBase);
D2VAR(D2CLIENT, pgtCharInvCompGrid_GLOVES,	D2InvCompGridStrc,			0x6FBB5A20 - D2ClientImageBase);

D2VAR(D2CLIENT, pgtHirelingInvRect, D2InvRectStrc,					0x6FBB3780 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpHirelingInvGridInfo, D2InventoryGridInfoStrc,	0x6FBB36F8 - D2ClientImageBase);

D2VAR(D2CLIENT, pgtHirelingInvCompGrid_RARM,	D2InvCompGridStrc,	0x6FBB5A88 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtHirelingInvCompGrid_TORSO,	D2InvCompGridStrc,	0x6FBB5A74 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtHirelingInvCompGrid_LARM,	D2InvCompGridStrc,	0x6FBB5A9C - D2ClientImageBase);
D2VAR(D2CLIENT, pgtHirelingInvCompGrid_HEAD,	D2InvCompGridStrc,	0x6FBB5A4C - D2ClientImageBase);
D2VAR(D2CLIENT, pgtHirelingInvCompGrid_NECK,	D2InvCompGridStrc,	0x6FBB5A60 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtHirelingInvCompGrid_RHAND,	D2InvCompGridStrc,	0x6FBB5AB0 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtHirelingInvCompGrid_LHAND,	D2InvCompGridStrc,	0x6FBB5AC4 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtHirelingInvCompGrid_BELT,	D2InvCompGridStrc,	0x6FBB5AD8 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtHirelingInvCompGrid_FEET,	D2InvCompGridStrc,	0x6FBB5AEC - D2ClientImageBase);
D2VAR(D2CLIENT, pgtHirelingInvCompGrid_GLOVES,	D2InvCompGridStrc,	0x6FBB5B00 - D2ClientImageBase);


D2VAR(D2CLIENT, pgtTradePage1Rect,				D2InvRectStrc,				0x6FBB3810 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtTradePage1GridInfo,			D2InventoryGridInfoStrc,	0x6FBB37F8 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtTradePage2Rect,				D2InvRectStrc,				0x6FBB3860 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtTradePage2GridInfo,			D2InventoryGridInfoStrc,	0x6FBB3838 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtBankPageRect,				D2InvRectStrc,				0x6FBB3794 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtBankPageGridInfo,			D2InventoryGridInfoStrc,	0x6FBB3728 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtBigBankPageRect,				D2InvRectStrc,				0x6FBB3750 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtBigBankPageGridInfo,			D2InventoryGridInfoStrc,	0x6FBB3760 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtTransmogrifyBoxPageRect,		D2InvRectStrc,				0x6FBB3850 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtTransmogrifyBoxPageGridInfo, D2InventoryGridInfoStrc,	0x6FBB38B0 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtGuildVaultPageRect,			D2InvRectStrc,				0x6FBB3740 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtGuildVaultPageGridInfo,		D2InventoryGridInfoStrc,	0x6FBB3874 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtTrophyCasePageRect,			D2InvRectStrc,				0x6FBB36D8 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtTrophyCasePageGridInfo,		D2InventoryGridInfoStrc,	0x6FBB3820 - D2ClientImageBase);

D2VAR(D2CLIENT, pgtNpcInvRect,		D2InvRectStrc,				0x6FBB36E8 - D2ClientImageBase);
D2VAR(D2CLIENT, pgtNpcInvGridInfo,	D2InventoryGridInfoStrc,	0x6FBB3898 - D2ClientImageBase);

D2CLIENTDWORDSTUB(6FBB5BA8);
D2CLIENTDWORDSTUB(6FBB388C);
D2CLIENTDWORDSTUB(6FBB3894);
D2CLIENTDWORDSTUB(6FBB58E0);
D2CLIENTDWORDSTUB(6FBB58E4);
D2CLIENTDWORDSTUB(6FBB58F4);
D2CLIENTDWORDSTUB(6FB79290);
D2CLIENTDWORDSTUB(6FB79294);
D2CLIENTDWORDSTUB(6FB79298);
D2CLIENTDWORDSTUB(6FB7929C);

D2VAR(D2CLIENT, pgpSelectedItem_6FBB58F0, D2UnitStrc*, 0x6FBB58F0 - D2ClientImageBase);
D2VAR(D2CLIENT, pqword_6FB792A0, int64_t, 0x6FB792A0 - D2ClientImageBase);


// 1.10f: D2Client.0x6FBB5B14
// 1.13c: D2Client.0x6FBD3424
BOOL gbInvPositionsAreHigherPrecision = FALSE;

// 1.10f: D2Client.0x6FBB58D8
// 1.13c: D2Client.0x6FBCBC20
BOOL gbHasRetrievedInvPositions = FALSE;

// 1.10f: D2Client.0x6FBB58DC
// 1.13c: D2Client.0x6FBCBC24
BOOL gbHasRetrievedNpcInvPositions = FALSE;

// 1.10f: D2Client.0x6FAE0A10
// 1.13c: D2Client.0x6FB408C0
void __fastcall UI_INV_RefreshPositions() {
	if (*D2CLIENT_pgbIsHigherResolution != gbInvPositionsAreHigherPrecision) 
	{
		gbInvPositionsAreHigherPrecision = *D2CLIENT_pgbIsHigherResolution;
		gbHasRetrievedInvPositions = FALSE;
		gbHasRetrievedNpcInvPositions = FALSE;
	}
	if (gbHasRetrievedInvPositions) 
	{
		const D2C_InventoryRecords nInvId = UNITS_GetInventoryRecordId(D2CLIENT_GetControlUnit(), INVPAGE_INVENTORY, D2CLIENT_GetExpansionFlag_6FAAC080());
		DATATBLS_GetInventoryRect(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvRect);
		DATATBLS_GetInventoryGridInfo(nInvId, *D2CLIENT_pgbIsHigherResolution,		D2CLIENT_pgpCharInvGridInfo);
		DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_RARM, INV_COMP_RARM);
		DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_TORSO, INV_COMP_TORSO);
		DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_LARM, INV_COMP_LARM);
		DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_HEAD, INV_COMP_HEAD);
		DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_NECK, INV_COMP_NECK);
		DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_RHAND, INV_COMP_RHAND);
		DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_LHAND, INV_COMP_LHAND);
		DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_BELT, INV_COMP_BELT);
		DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_FEET, INV_COMP_FEET);
		DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_GLOVES, INV_COMP_GLOVES);

		DATATBLS_GetInventoryRect(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtHirelingInvRect);
		DATATBLS_GetInventoryGridInfo(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgpHirelingInvGridInfo);
		DATATBLS_GetInventoryComponentGrid(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_RARM, INV_COMP_RARM);
		DATATBLS_GetInventoryComponentGrid(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_TORSO, INV_COMP_TORSO);
		DATATBLS_GetInventoryComponentGrid(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_LARM, INV_COMP_LARM);
		DATATBLS_GetInventoryComponentGrid(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_HEAD, INV_COMP_HEAD);
		DATATBLS_GetInventoryComponentGrid(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_NECK, INV_COMP_NECK);
		DATATBLS_GetInventoryComponentGrid(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_RHAND, INV_COMP_RHAND);
		DATATBLS_GetInventoryComponentGrid(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_LHAND, INV_COMP_LHAND);
		DATATBLS_GetInventoryComponentGrid(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_BELT, INV_COMP_BELT);
		DATATBLS_GetInventoryComponentGrid(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_FEET, INV_COMP_FEET);
		DATATBLS_GetInventoryComponentGrid(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtCharInvCompGrid_GLOVES, INV_COMP_GLOVES);

		DATATBLS_GetInventoryRect(		INVENTORYRECORD_TRADE_PAGE_1,			*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtTradePage1Rect);
		DATATBLS_GetInventoryGridInfo(	INVENTORYRECORD_TRADE_PAGE_1,			*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtTradePage1GridInfo);
		DATATBLS_GetInventoryRect(		INVENTORYRECORD_TRADE_PAGE_2,			*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtTradePage2Rect);
		DATATBLS_GetInventoryGridInfo(	INVENTORYRECORD_TRADE_PAGE_2,			*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtTradePage2GridInfo);
		DATATBLS_GetInventoryRect(		INVENTORYRECORD_BANK_PAGE_1,			*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtBankPageRect);
		DATATBLS_GetInventoryGridInfo(	INVENTORYRECORD_BANK_PAGE_1,			*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtBankPageGridInfo);
		DATATBLS_GetInventoryRect(		INVENTORYRECORD_BIG_BANK_PAGE_1,		*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtBigBankPageRect);
		DATATBLS_GetInventoryGridInfo(	INVENTORYRECORD_BIG_BANK_PAGE_1,		*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtBigBankPageGridInfo);
		DATATBLS_GetInventoryRect(		INVENTORYRECORD_TRANSMOGRIFY_BOX_PAGE_1,*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtTransmogrifyBoxPageRect);
		DATATBLS_GetInventoryGridInfo(	INVENTORYRECORD_TRANSMOGRIFY_BOX_PAGE_1,*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtTransmogrifyBoxPageGridInfo);
		DATATBLS_GetInventoryRect(		INVENTORYRECORD_GUILD_VAULT_PAGE_1,		*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtGuildVaultPageRect);
		DATATBLS_GetInventoryGridInfo(	INVENTORYRECORD_GUILD_VAULT_PAGE_1,		*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtGuildVaultPageGridInfo);
		DATATBLS_GetInventoryRect(		INVENTORYRECORD_TROPHY_CASE_PAGE_1,		*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtTrophyCasePageRect);
		DATATBLS_GetInventoryGridInfo(	INVENTORYRECORD_TROPHY_CASE_PAGE_1,		*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtTrophyCasePageGridInfo);
		gbHasRetrievedInvPositions = TRUE;
	}

	D2UnitGUID nNPCGuid;
	if (!gbHasRetrievedNpcInvPositions && *D2CLIENT_pgnInventoryMode && D2CLIENT_NPC_GetActivatedNpcGUID_6FAFBB50(&nNPCGuid))
	{
		D2UnitStrc* ptNpc = CUNIT_GetClientUnit(nNPCGuid, UNIT_MONSTER);
		if (ptNpc) 
		{
			const D2C_InventoryRecords nInvId = UNITS_GetInventoryRecordId(ptNpc, 0, D2CLIENT_GetExpansionFlag_6FAAC080());
			DATATBLS_GetInventoryRect(nInvId,		*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtNpcInvRect);
			DATATBLS_GetInventoryGridInfo(nInvId,	*D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtNpcInvGridInfo);
			gbHasRetrievedNpcInvPositions = TRUE;
		}
	}
}

//1.10f: D2Client.0x6FAE9FC0
void __stdcall UI_INPUT_NpcShop_VKEscSpace(SMSGHANDLER_PARAMS* pMsg) 
{
	if ((*D2CLIENT_pgnInventoryMode == VENDORMODE_TRADE
		|| *D2CLIENT_pgnInventoryMode == VENDORMODE_BUY
		|| *D2CLIENT_pgnInventoryMode == VENDORMODE_SELL
		|| *D2CLIENT_pgnInventoryMode == VENDORMODE_REPAIR
		|| *D2CLIENT_pgnInventoryMode == VENDORMODE_UNKNOWN
		) && !D2CLIENT_CheckUIState_6FB23860(UI_CHATBOX))
	{
		gbHasRetrievedNpcInvPositions = FALSE;
		D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(D2CLIENT_GetControlUnit()->pInventory);
		CLIENT_SetCursorHeldItem(pCursorItem);
		pMsg->bUseResult = 1;
		pMsg->lResult = 0;
		SMsgBreakHandlerChain(pMsg);
		*D2CLIENT_pdword_6FBB5BA8 = 0;
		sub_6FAF9E60();
		*D2CLIENT_pdword_6FBB388C = 0;
		*D2CLIENT_pdword_6FBB3894 = 0;
		*D2CLIENT_pdword_6FBB58E0 = 0;
		*D2CLIENT_pdword_6FBB58E4 = 0;
		*D2CLIENT_pgpSelectedItem_6FBB58F0 = nullptr;
		*D2CLIENT_pdword_6FBB58F4 = 0;
		*D2CLIENT_pdword_6FB79290 = -1;
		*D2CLIENT_pdword_6FB79294 = -1;
		*D2CLIENT_pdword_6FB79298 = -1;
		*D2CLIENT_pdword_6FB7929C = -1;
		*D2CLIENT_pqword_6FB792A0 = -1;
	}
}

#endif // D2_VERSION_110F
