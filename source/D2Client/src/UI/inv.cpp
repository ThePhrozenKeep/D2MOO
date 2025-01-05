#include <CGAME/Game.h>
#include <D2Client.h>
#include <D2Gfx.h>
#include <D2Items.h>
#include <D2StatList.h>
#include <D2WinFont.h>
#include <UI/inv.h>
#include <UI/text.h>
#include <UI/UI.h>
#include <Units/Units.h>
#include <DataTbls/InvTbls.h>
#include <Unit/CUnit.h>
#include <Engine/Cursor.h>
#include <Storm.h>
#include <Texture.h>
#include <DataTbls/StringIds.h>
#include <UI/npcmenu.h>
#include <Core/ARCHIVE.h>
#include <D2WinPalette.h>
#include <CGAME/scroll.h>
#include <Engine/Gfx.h>
#include <D2Combat.h>
#include <D2Environment.h>
#include <D2ItemMods.h>
#include <D2Math.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StrTable.h>
#include <DataTbls/SkillsIds.h>
#include <D2DataTbls.h>

static const int kBuySellButtonSize = 32;
#ifdef NO_BUG_FIX
static const int kBuySellButtonSizeForTrade = 45; // Note this is incorrect... real size is 32
#else
static const int kBuySellButtonSizeForTrade = kBuySellButtonSize;
#endif
static const int kButtonTextOffsetY = 9;

static const int kGoldAmountTextOffsetX = 165;
static const int kGoldAmountTextOffsetYClassic = -244;
static const int kGoldAmountTextOffsetYExpansion = -440;

static const int kCoinButtonBorderOffsetX = 73;
static const int kCoinButtonBorderOffsetYClassic = -242;
static const int kCoinButtonBorderOffsetYExpansion = -438;


static const D2CoordStrc kCoinButtonOffsetClassic{ 73, -242 };
static const D2CoordStrc kCoinButtonOffsetExpansion{ 73, -438 };
static const int kCoinButtonWidth = 20;
static const int kCoinButtonHeight = 18;
static const D2CoordStrc kStashHoverTextOffset = { -20, 20 };

static const D2CoordStrc kInvStashCloseButtonOffsetClassic = { 275, 65 };
static const D2CoordStrc kInvStashCloseButtonOffsetExpansion = { 272, 64 };
static const D2CoordStrc kInvEquipCloseButtonOffset = { 270, 64 };


#ifdef D2_VERSION_110F

D2CLIENTSTUB(GetExpansionFlag, 6FAAC080, BOOL, __fastcall, ());
D2CLIENTSTUB(D2CLIENT_NPC_GetActivatedNpcGUID, 6FAFBB50, BOOL, __fastcall, (D2UnitGUID* pGUID));
D2CLIENTSTUB(sub, 6FAF9E60, void, __fastcall, ());

D2CLIENTSTUB(D2CLIENT_GetDifficulty, 6FAAC090, D2C_Difficulties , __fastcall, ());
D2CLIENTSTUB(D2CLIENT_GetGlobalQuestHistory, 6FAFBD20, D2BitBufferStrc*, __fastcall, ());
//1.13c: D2Client.0x6FB05B20
D2CLIENTSTUB(ITEMDESC_Mods, 6FAF19C0, void, __fastcall, (D2UnitStrc* pItem, Unicode* pBuffer, int nBufferSize, BOOL bUndeadDamage, D2C_States nState1, D2C_States nState2, D2C_StatlistFlags fFilter, int bAppendNewline));

D2CLIENTSTUB(D2CLIENT_FormatStats, 6FADCFE0, void, __fastcall, (const Unicode* wszFormat, ...));


//1.13c: D2Client.0x6FB3C760
D2CLIENTSTUB(UI_INV_HirelingCanEquip, 6FAED470, BOOL, __fastcall, (D2UnitStrc* pOwner, D2UnitStrc* pItem, BOOL* pbItemTypeCompatible));

D2VAR(D2CLIENT, pgbIsHigherResolution, BOOL, 0x6FB9A708 - D2ClientImageBase);
D2VAR(D2CLIENT, pgnInventoryMode, BOOL, 0x6FBB58EC - D2ClientImageBase);

using BodyLocsInvCompGrids = D2InvCompGridStrc[NUM_BODYLOC_NO_SWITCH];

D2VAR(D2CLIENT, pgtCharInvRect,		D2InvRectStrc,				0x6FBB38C8 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCharInvGridInfo,	D2InventoryGridInfoStrc,	0x6FBB3710 - D2ClientImageBase);
D2VAR(D2CLIENT, pgaCharInvCompGrids,BodyLocsInvCompGrids,		0x6FBB5958 - D2ClientImageBase); //1.13c: D2Client.0x6FBCBCA0

D2VAR(D2CLIENT, pgtHirelingInvRect,		D2InvRectStrc,			0x6FBB3780 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpHirelingInvGridInfo,	D2InventoryGridInfoStrc,0x6FBB36F8 - D2ClientImageBase);
D2VAR(D2CLIENT, pgaHirelingInvCompGrids,BodyLocsInvCompGrids,	0x6FBB5A88 - D2ClientImageBase); //1.13c: D2Client.0x6FBCBD80



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
D2VAR(D2CLIENT, pgbInvMouseOverEquippedItem, BOOL, 0x6FBB58E4 - D2ClientImageBase);
D2CLIENTDWORDSTUB(6FBB58F4);
D2CLIENTDWORDSTUB(6FB79290);
D2CLIENTDWORDSTUB(6FB79294);
D2CLIENTDWORDSTUB(6FB79298);
D2CLIENTDWORDSTUB(6FB7929C);

D2VAR(D2CLIENT, pgpSelectedItem, D2UnitStrc*, 0x6FBB58F0 - D2ClientImageBase); // aka sghSelItem 1.13c: 0x6FBCBC38
D2VAR(D2CLIENT, pgbSelectedItemIsOwned, BOOL, 0x6FB7928C - D2ClientImageBase); // 1.13c: 0x6FB90EA4
D2VAR(D2CLIENT, pgnSelectedItemDescTopPosY, int32_t, 0x6FB79294 - D2ClientImageBase); // 1.13c: 0x6FB90EAC
D2VAR(D2CLIENT, pgnSelectedItemDescBottomPosY, int32_t, 0x6FB7929C - D2ClientImageBase); // 1.13c: 0x6FB90EB4
D2VAR(D2CLIENT, pgnSelectedItemDescTopPosX, int32_t, 0x6FB79290 - D2ClientImageBase); // 1.13c: 0x6FB90EA8
D2VAR(D2CLIENT, pgnSelectedItemDescBottomPosX, int32_t, 0x6FB79298 - D2ClientImageBase); // 1.13c: 0x6FB90EB0


D2VAR(D2CLIENT, pqword_6FB792A0, int64_t, 0x6FB792A0 - D2ClientImageBase);

D2VAR(D2CLIENT, pgnScreenWidth, uint32_t, 0x6FB740EC - D2ClientImageBase);
D2VAR(D2CLIENT, pgnScreenHeight, uint32_t, 0x6FB740F0 - D2ClientImageBase);

//1.10f: D2Client.0x6FBB388C
//1.13c: D2Client.0x6FBBB2A4
D2VAR(D2CLIENT, pgnInvMousePosX, int32_t, 0x6FBB388C - D2ClientImageBase);
//1.10f: D2Client.0x6FBB3894
//1.13c: D2Client.0x6FBBB430
D2VAR(D2CLIENT, pgnInvMousePosY, int32_t, 0x6FBB3894 - D2ClientImageBase);
//1.10f: D2Client.0x6FBBA748
//1.13c: D2Client.0x6FBBA748
D2VAR(D2CLIENT, pgnInvPanelOffsetX, int32_t, 0x6FBBA748 - D2ClientImageBase);
//1.10f: D2Client.0x6FBBA74C
//1.13c: D2Client.0x6FBCB9A4
D2VAR(D2CLIENT, pgnInvPanelOffsetY, int32_t, 0x6FBBA74C - D2ClientImageBase);
//1.10f: D2Client.0x6FB795B0
//1.13c: D2Client.0x6FB911C8
D2VAR(D2CLIENT, pgnNumberOfButtons, int32_t, 0x6FB795B0 - D2ClientImageBase);

D2VAR(D2CLIENT, pgpCellfile_GOLDCOINBTN, D2CellFileStrc*, 0x6FBB5934 - D2ClientImageBase);
D2VAR(D2CLIENT, pgbInvEquipCoinButtonPressed, BOOL, 0x6FBB5B38 - D2ClientImageBase);
D2VAR(D2CLIENT, pgbInvInventoryCoinButtonPressed, BOOL, 0x6FBB5B3C - D2ClientImageBase);

D2VAR(D2CLIENT, pgbInvCloseButtonPressed, BOOL, 0x6FBB5B98 - D2ClientImageBase);
D2VAR(D2CLIENT, pgpCellfile_BUYSELLBUTTONS, D2CellFileStrc*, 0x6FBBA76C - D2ClientImageBase);

D2VAR(D2CLIENT, pgnInvHoveredBodyLoc, D2C_PlayerBodyLocs, 0x6FBCBC68 - D2ClientImageBase);

D2VAR(D2CLIENT, pgnDrawMaxWidth, int32_t, 0x6FB9A704 - D2ClientImageBase); // 1.13c: D2Client.0x6FBA9E14
D2VAR(D2CLIENT, pgnDrawMaxHeight, int32_t, 0x6FB9A700 - D2ClientImageBase);// 1.13c: D2Client.0x6FBA9E18

//1.10f: D2Client.0x6FBB3778
//1.13c: D2Client.0x6FBBB330
uint8_t gnBodyLocBgColors[UI_ItemBgColor_COUNT];

#pragma pack(push, 1)
struct InvButtonInfoStrc
{
	DWORD bEnabled;
	DWORD bPressed;
	D2C_StringIndices nHoverTextId;
	D2C_VendorInventoryModes nInventoryMode;
	UI_BuySellButtonFrames nBaseFrame;
	D2C_CursorBuySellFrame nCursorFrame;
};
#pragma pack(pop)

D2_ASSERT_SIZE_110F(InvButtonInfoStrc, 0x14);

constexpr int cMaxButtonsCount = 4;
constexpr int cButtonsConfigurations = 4;

//1.10f: D2Client.0x6FBB37A8
//1.13c: D2Client.0x6FBBB238
using InvButtonArray = InvButtonInfoStrc[cMaxButtonsCount];
D2VAR(D2CLIENT, pgatInvButtonInfos, InvButtonArray, 0x6FBB37A8 - D2ClientImageBase);

// It seems the following was meant to change the position of the button based on the number 
// of buttons per page. However, it is always used with a value of 3. 
// They probably intended to draw only the buttons needed and align them to the right, 
// but instead decided to have 4 fixed slots.
//1.10f: D2Client.0x6FB795AC
//1.13c: D2Client.0x6FB911C4
int32_t gnInvButtonsConfiguration = 3;

//1.10f: D2Client.0x6FB795B8
//1.13c: D2Client.0x6FB911D0
int32_t gatButtonsOffsetX[cButtonsConfigurations][cMaxButtonsCount] =
{
	{273,   0,   0,   0},
	{169, 273,   0,   0},
	{169, 221, 273,   0},
	{116, 169, 221, 273},
};

//1.10f: D2Client.0x6FBB5B14
//1.13c: D2Client.0x6FBD3424
BOOL gbInvPositionsAreHigherPrecision = FALSE;

//1.10f: D2Client.0x6FBB58D8
//1.13c: D2Client.0x6FBCBC20
BOOL gbHasRetrievedInvPositions = FALSE;

//1.10f: D2Client.0x6FBB58DC
//1.13c: D2Client.0x6FBCBC24
BOOL gbHasRetrievedNpcInvPositions = FALSE;

// Helper functions
bool IsInRectangle(D2CoordStrc tCoords, D2InvRectStrc tRect)
{
	return tCoords.nX >= tRect.nLeft && tCoords.nX <= tRect.nRight
		&& tCoords.nY >= tRect.nTop && tCoords.nY <= tRect.nBottom;
}

D2InvRectStrc D2InvRectFromPosAndSize(D2CoordStrc tCoords, int width, int height)
{
	return { tCoords.nX, tCoords.nY - height, tCoords.nX + width, tCoords.nY };
}

// TODO: move those to headers, which means seperate structure definitions from functions declaration since it needs sgptDataTables
// Inlined
D2SetsTxt* __stdcall CLIENT_DATATBLS_GetSetsTxtRecord(int nSetId)
{
	if (nSetId >= 0 && nSetId < sgptDataTables->nSetsTxtRecordCount)
	{
		return &sgptDataTables->pSetsTxt[nSetId];
	}
	return nullptr;
}
// Inlined
D2SetItemsTxt* __stdcall CLIENT_DATATBLS_GetSetItemsTxtRecord(int nSetItemId)
{
	if (nSetItemId >= 0 && nSetItemId < sgptDataTables->nSetItemsTxtRecordCount)
	{
		return &sgptDataTables->pSetItemsTxt[nSetItemId];
	}
	return nullptr;
}
// Inlined
D2SetsTxt* __stdcall CLIENT_DATATBLS_GetSetsTxtRecordFromSetItemId(int nSetItemId)
{
	if (D2SetItemsTxt* pSetItemTxt = CLIENT_DATATBLS_GetSetItemsTxtRecord(nSetItemId))
	{
		return CLIENT_DATATBLS_GetSetsTxtRecord(pSetItemTxt->nSetId);
	}
	return nullptr;
}
// Inlined
D2CharStatsTxt* __fastcall CLIENT_DATATBLS_GetCharStatsTxtRecord(int nRecordId)
{
	if (nRecordId >= 0 && nRecordId < sgptDataTables->nCharStatsTxtRecordCount)
	{
		return &sgptDataTables->pCharStatsTxt[nRecordId];
	}

	return nullptr;
}
// Inlined
D2ItemStatCostTxt* __fastcall CLIENT_DATATBLS_GetItemStatCostTxtRecord(int nStatId)
{
	if (nStatId >= 0 && nStatId < sgptDataTables->nItemStatCostTxtRecordCount)
	{
		return &sgptDataTables->pItemStatCostTxt[nStatId];
	}

	return NULL;
}
// Inlined
D2UniqueItemsTxt* __fastcall CLIENT_DATATBLS_GetUniqueItemsTxtRecord(int nUniqueItemId)
{
	if (nUniqueItemId >= 0 && nUniqueItemId < sgptDataTables->nUniqueItemsTxtRecordCount)
	{
		return &sgptDataTables->pUniqueItemsTxt[nUniqueItemId];
	}

	return nullptr;
}

static const D2C_PlayerBodyLocs gaInvComponentToBodyLoc[NUM_INV_COMPS]
{
	/*[INV_COMP_RARM]*/BODYLOC_RARM,
	/*[INV_COMP_TORSO]*/BODYLOC_TORSO,
	/*[INV_COMP_LARM]*/BODYLOC_LARM,
	/*[INV_COMP_HEAD]*/BODYLOC_HEAD,
	/*[INV_COMP_NECK]*/BODYLOC_NECK,
	/*[INV_COMP_RHAND]*/BODYLOC_RHAND,
	/*[INV_COMP_LHAND]*/BODYLOC_LHAND,
	/*[INV_COMP_BELT]*/BODYLOC_BELT,
	/*[INV_COMP_FEET]*/BODYLOC_FEET,
	/*[INV_COMP_GLOVES]*/BODYLOC_GLOVES,
};
//1.10f: D2Client.0x6FAE0A10
//1.13c: D2Client.0x6FB408C0
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
		for (int i = 0; i < NUM_INV_COMPS; i++)
		{
			DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, &(*D2CLIENT_pgaCharInvCompGrids)[gaInvComponentToBodyLoc[i]], D2InventoryComponent(i));
		}

		DATATBLS_GetInventoryRect(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgtHirelingInvRect);
		DATATBLS_GetInventoryGridInfo(INVENTORYRECORD_HIRELING, *D2CLIENT_pgbIsHigherResolution, D2CLIENT_pgpHirelingInvGridInfo);
		for (int i = 0; i < NUM_INV_COMPS; i++)
		{
			DATATBLS_GetInventoryComponentGrid(nInvId, *D2CLIENT_pgbIsHigherResolution, &(*D2CLIENT_pgaHirelingInvCompGrids)[gaInvComponentToBodyLoc[i]], D2InventoryComponent(i));
		}

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

//1.10f: D2Client.0x6FADD2F0
//1.13c: D2Client.0x6FB3C6B0
void __fastcall UI_INV_RefreshUsageColors()
{
	gnBodyLocBgColors[UI_ItemBgColor_Red] = D2Win_10034_MixRGB(128u, 0, 0);
	gnBodyLocBgColors[UI_ItemBgColor_Green] = D2Win_10034_MixRGB(0, 128u, 0);
	gnBodyLocBgColors[UI_ItemBgColor_Navy] = D2Win_10034_MixRGB(0, 0, 128u);
	gnBodyLocBgColors[UI_ItemBgColor_Olive] = D2Win_10034_MixRGB(128u, 128u, 0);
	gnBodyLocBgColors[UI_ItemBgColor_Brown] = D2Win_10034_MixRGB(128u, 64u, 64u);
}

//1.10f: D2Client.0x6FAE17D0
//1.13c: D2Client.0x6FB3C2B0
BOOL __fastcall UI_INV_CheckIfCursorItemCanBePutIntoSocket(D2UnitStrc* pCursorItem, D2UnitStrc* pEquippedItem)
{
	if (!pCursorItem || !pEquippedItem)
		return FALSE;
	if (*D2CLIENT_pgnInventoryMode == VENDORMODE_PLRTRADE_OLD || *D2CLIENT_pgnInventoryMode == VENDORMODE_PLRTRADE)
		return FALSE;
	if (!ITEMS_IsSocketFiller(pCursorItem))
		return FALSE;
	if (!ITEMS_CHECK_FLAG(pEquippedItem, IFLAG_SOCKETED))
		return FALSE;
	if (!ITEMS_CheckIfSocketable(pEquippedItem))
		return FALSE;
	if (!ITEMS_GetMaxSockets(pEquippedItem))
		return FALSE;
	if (!ITEMS_CHECK_FLAG(pEquippedItem,IFLAG_IDENTIFIED) || ITEMS_CHECK_FLAG(pEquippedItem, IFLAG_BROKEN))
		return FALSE;
	if (!pEquippedItem->pInventory)
		return FALSE;
	return INVENTORY_GetItemCount(pEquippedItem->pInventory) < ITEMS_GetSockets(pEquippedItem);
}

// Helper function
void __fastcall UI_INV_DrawItemBackgroundRect(D2CoordStrc tScreenPos, int32_t nWidth, int32_t nHeight, UI_ItemBackgroundColor nBgColor)
{
	// Note: This looks like a dumb test, we only the position of the rect, not its extents...
	if (UI_IsInScreen(tScreenPos))
	{
		GFX_DrawFilledRect(
			tScreenPos.nX, tScreenPos.nY,
			nWidth, nHeight,
			gnBodyLocBgColors[nBgColor],
			DRAWMODE_TRANS25);
	}
}


//1.10f: Inlined
//1.13c: D2Client.0x6FB3C5B0
void __fastcall UI_INV_DrawCompColor(D2InvCompGridStrc *pInvCompGrid, UI_ItemBackgroundColor nBgColor)
{
	if (nBgColor != UI_ItemBgColor_Navy)
	{
		UI_INV_DrawItemBackgroundRect({ pInvCompGrid->rect.nLeft, pInvCompGrid->rect.nTop }, pInvCompGrid->nWidth, pInvCompGrid->nHeight, nBgColor);
	}
}

//1.10f: Inlined
//1.13c: D2Client.0x6FB3E2F0
void __fastcall UI_INV_DrawComponentBackground(D2InventoryGridInfoStrc* pGridInfo, D2InventoryStrc* pInventory)
{
	const bool bMouseInRightHalfOfScreen = CLIENT_GetMouseX() >= (*D2CLIENT_pgnScreenHeight / 2);
	if ((*D2CLIENT_pgnInventoryMode != VENDORMODE_TRADE || bMouseInRightHalfOfScreen)
		&& CLIENT_GetMouseY() < (*D2CLIENT_pgnScreenHeight - 39))
	{
		if (bMouseInRightHalfOfScreen)
		{
			D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(pInventory);
			if (pCursorItem && *D2CLIENT_pgbInvMouseOverEquippedItem)
			{

				// Note: ITEMS_GetDimensions was called to do some bound test but the test result was unused or optimized away
				//uint8_t nWidth, nHeight;
				//ITEMS_GetDimensions(pCursorItem, &nWidth, &nHeight, "..\\Source\\D2Common/CmnItems.h", 1754);

				UI_ItemBackgroundColor nBgColor = UI_ItemBgColor_Red;
				if ((ITEMS_CheckBodyLocation(pCursorItem, *D2CLIENT_pgnInvHoveredBodyLoc) && ITEMS_CheckRequirements(pCursorItem, D2CLIENT_GetControlUnit(), 0, 0, 0, 0))
					|| UI_INV_CheckIfCursorItemCanBePutIntoSocket(INVENTORY_GetItemFromBodyLoc(pInventory, *D2CLIENT_pgnInvHoveredBodyLoc), pCursorItem))
				{
					nBgColor = UI_ItemBgColor_Green;
				}

				UI_INV_DrawCompColor(&(*D2CLIENT_pgaCharInvCompGrids)[*D2CLIENT_pgnInvHoveredBodyLoc], nBgColor);
			}
		}
		else
		{
			*D2CLIENT_pgnInvHoveredBodyLoc = BODYLOC_NONE;
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
		|| *D2CLIENT_pgnInventoryMode == VENDORMODE_CLOSE
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
		*D2CLIENT_pgbInvMouseOverEquippedItem = 0;
		*D2CLIENT_pgpSelectedItem = nullptr;
		*D2CLIENT_pdword_6FBB58F4 = 0;
		*D2CLIENT_pdword_6FB79290 = -1;
		*D2CLIENT_pdword_6FB79294 = -1;
		*D2CLIENT_pdword_6FB79298 = -1;
		*D2CLIENT_pdword_6FB7929C = -1;
		*D2CLIENT_pqword_6FB792A0 = -1;
	}
}

//1.10f: Inlined
//1.13c: D2Client.0x6FB3F260
void __fastcall UI_INV_DrawButtonHoverText()
{
	if (*D2CLIENT_pgnInvMousePosX < D2CLIENT_pgtNpcInvRect->nLeft || *D2CLIENT_pgnInvMousePosX > D2CLIENT_pgtNpcInvRect->nRight)
		return;
	const int32_t nButtonY = *D2CLIENT_pgnInvPanelOffsetY + *D2CLIENT_pgnScreenHeight - (kInvStashCloseButtonOffsetExpansion.nY + kBuySellButtonSizeForTrade);
	if (*D2CLIENT_pgnInvMousePosY < nButtonY || *D2CLIENT_pgnInvMousePosY > D2CLIENT_pgtNpcInvRect->nBottom)
		return;

	if (*D2CLIENT_pgnNumberOfButtons <= 0)
		return;
	for (int nButtonIdx = 0; nButtonIdx < *D2CLIENT_pgnNumberOfButtons; nButtonIdx++)
	{
		const InvButtonInfoStrc& rButtonInfo = (*D2CLIENT_pgatInvButtonInfos)[nButtonIdx];
		if (rButtonInfo.bEnabled != 1)
			continue;
		const int32_t nButtonX = *D2CLIENT_pgnInvPanelOffsetX + gatButtonsOffsetX[gnInvButtonsConfiguration][nButtonIdx];
		if (*D2CLIENT_pgnInvMousePosX > nButtonX && *D2CLIENT_pgnInvMousePosX < nButtonX + kBuySellButtonSizeForTrade 
			&& *D2CLIENT_pgnInvMousePosY > nButtonY && *D2CLIENT_pgnInvMousePosY < nButtonY + kBuySellButtonSizeForTrade - 1)
		{
			if (rButtonInfo.nHoverTextId == STR_IDX_10095_x_REPAIR_ALL_EQUIPMENT_FMT)
			{
				Unicode wszRepairAllTextbuffer[256];
				D2UnitStrc* hControlUnit = D2CLIENT_GetControlUnit();
				D2UnitStrc* hActiveNpcUnit = UI_NPC_GetActiveNpcUnit();
				if (hControlUnit && hActiveNpcUnit)
				{
					int nRepairCost = ITEMS_GetAllRepairCosts(nullptr, hControlUnit, hActiveNpcUnit->dwClassId,
						D2CLIENT_GetDifficulty_6FAAC090(),
						D2CLIENT_GetGlobalQuestHistory_6FAFBD20(),
						0);
					Unicode::sprintf(256, wszRepairAllTextbuffer, D2LANG_GetStringFromTblIndex(STR_IDX_10095_x_REPAIR_ALL_EQUIPMENT_FMT), nRepairCost);
					D2Win_10129_DrawFramedText(wszRepairAllTextbuffer, nButtonX, nButtonY + kButtonTextOffsetY, 0, 0);
				}
			}
			else
			{
				D2Win_10129_DrawFramedText(D2LANG_GetStringFromTblIndex(rButtonInfo.nHoverTextId), nButtonX, nButtonY + kButtonTextOffsetY, 0, 0);
			}
			return;
		}
	}
	
}

//1.10f: D2Client.0x6FAE97C0
//1.13c: D2Client.0x6FB3CFB0
void __fastcall UI_INV_DrawGoldValue(D2C_ItemInvPage nPage)
{
	const Font nPreviousFont = D2Win_10127_SetFont(D2FONT_FONT16);

	int32_t nGoldAmount;
	D2CoordStrc tGoldAmountTextPos;
	D2CoordStrc tCoinButtonBorderPos;
	int32_t nCoinButtonFrame = 0;

	switch (nPage)
	{
	case INVPAGE_INVENTORY:
	{
		nGoldAmount = STATLIST_UnitGetStatValue(D2CLIENT_GetControlUnit(), STAT_GOLDBANK, 0);
		tGoldAmountTextPos.nX = *D2CLIENT_pgnInvPanelOffsetX + kGoldAmountTextOffsetX;
		tCoinButtonBorderPos.nX = *D2CLIENT_pgnInvPanelOffsetX + kCoinButtonBorderOffsetX;
		if (D2CLIENT_GetExpansionFlag_6FAAC080())
		{
			tGoldAmountTextPos.nY = *D2CLIENT_pgnInvPanelOffsetY + *D2CLIENT_pgnScreenHeight + kGoldAmountTextOffsetYExpansion;
			tCoinButtonBorderPos.nY = *D2CLIENT_pgnInvPanelOffsetY + *D2CLIENT_pgnScreenHeight + kCoinButtonBorderOffsetYExpansion;
		}
		else
		{
			tGoldAmountTextPos.nY = *D2CLIENT_pgnInvPanelOffsetY + *D2CLIENT_pgnScreenHeight + kGoldAmountTextOffsetYClassic;
			tCoinButtonBorderPos.nY = *D2CLIENT_pgnInvPanelOffsetY + *D2CLIENT_pgnScreenHeight + kCoinButtonBorderOffsetYClassic;
		}
		nCoinButtonFrame = *D2CLIENT_pgbInvEquipCoinButtonPressed != 0;

		const D2CoordStrc tCoinButtonOffset = D2CLIENT_GetExpansionFlag_6FAAC080() ? kCoinButtonOffsetExpansion : kCoinButtonOffsetClassic;
		const int32_t nX = *D2CLIENT_pgnInvPanelOffsetX + tCoinButtonOffset.nX;
		const int32_t nY = *D2CLIENT_pgnInvPanelOffsetY + *D2CLIENT_pgnScreenHeight + tCoinButtonOffset.nY;
		D2CoordStrc tMouseCoord = { CLIENT_GetMouseX(), CLIENT_GetMouseY() };
		if (IsInRectangle(tMouseCoord, D2InvRectFromPosAndSize(tMouseCoord, kCoinButtonWidth, kCoinButtonHeight)))
		{
			D2Win_10129_DrawFramedText(D2LANG_GetStringFromTblIndex(STR_IDX_4124_strGoldWithdraw), nX + kStashHoverTextOffset.nX, nY - kStashHoverTextOffset.nY, 0, 0);
		}
		break;
	}
	case INVPAGE_EQUIP:
	{
		nGoldAmount = STATLIST_UnitGetStatValue(D2CLIENT_GetControlUnit(), STAT_GOLD, 0);
		tGoldAmountTextPos.nX = *D2CLIENT_pgnScreenWidth - *D2CLIENT_pgnInvPanelOffsetX - 212;
		tCoinButtonBorderPos.nX = *D2CLIENT_pgnScreenWidth - *D2CLIENT_pgnInvPanelOffsetX - 237;
		tGoldAmountTextPos.nY = *D2CLIENT_pgnInvPanelOffsetY + *D2CLIENT_pgnScreenHeight - 72;
		tCoinButtonBorderPos.nY = *D2CLIENT_pgnInvPanelOffsetY + *D2CLIENT_pgnScreenHeight - 69;
		nCoinButtonFrame = *D2CLIENT_pgbInvEquipCoinButtonPressed != 0;
	}
	default:
	{
		nGoldAmount = STATLIST_UnitGetStatValue(D2CLIENT_GetControlUnit(), STAT_GOLDBANK, 0);
		tCoinButtonBorderPos.nY = 0;
		tGoldAmountTextPos.nX = D2Win_10121_GetTextWidth(D2LANG_GetStringFromTblIndex(STR_IDX_3315_stash)) + *D2CLIENT_pgnInvPanelOffsetX + 21;
		nCoinButtonFrame = 0;
		tGoldAmountTextPos.nY = *D2CLIENT_pgnInvPanelOffsetY + *D2CLIENT_pgnScreenHeight - 106;
		tCoinButtonBorderPos.nX = 0;
		tCoinButtonBorderPos.nX = 0;
		nCoinButtonFrame = 0;
	}
	}
	if (nPage != INVPAGE_CUBE)
	{
		char szGoldAmount[20];
		SStrPrintf(szGoldAmount, 20u, "%d", nGoldAmount);
		Unicode wszGoldAmount[20];
		Unicode::win2Unicode(wszGoldAmount, szGoldAmount, 19);
		if (nPage == INVPAGE_TRADE)
			tGoldAmountTextPos.nX = *D2CLIENT_pgnInvPanelOffsetX - D2Win_10121_GetTextWidth(wszGoldAmount) + 198;
		D2Win_10117_DrawText(wszGoldAmount, tGoldAmountTextPos.nX, tGoldAmountTextPos.nY, 0, 0);
	}
	D2CoordStrc tCoinButtonPos;
	switch (nPage)
	{
	case INVPAGE_INVENTORY:
		if (*D2CLIENT_pgbInvEquipCoinButtonPressed)
			++tCoinButtonBorderPos.nY;
		tCoinButtonPos.nY = tCoinButtonBorderPos.nY - 2;
		tCoinButtonPos.nX = tCoinButtonBorderPos.nX + 2;
		break;
	case INVPAGE_EQUIP:
		if (*D2CLIENT_pgbInvInventoryCoinButtonPressed)
			++tCoinButtonBorderPos.nY;
		tCoinButtonPos.nY = tCoinButtonBorderPos.nY - 2;
		tCoinButtonPos.nX = tCoinButtonBorderPos.nX + 1;
		break;
	default:
		D2Win_10117_DrawText(D2LANG_GetStringFromTblIndex(STR_IDX_3315_stash), *D2CLIENT_pgnInvPanelOffsetX + 21, tGoldAmountTextPos.nY, 0, 0);
		D2Win_10127_SetFont(nPreviousFont);
		return;
	}
	D2GfxDataStrc tGfxData{};
	tGfxData.nDirection = 0;
	tGfxData.nFrame = nCoinButtonFrame;
	tGfxData.pCellFile = *D2CLIENT_pgpCellfile_GOLDCOINBTN;
	TEXTURE_CelDraw(&tGfxData, tCoinButtonPos.nX, tCoinButtonPos.nY, -1, DRAWMODE_NORMAL, 0);
	D2Win_10127_SetFont(nPreviousFont);
}

//1.10f: D2Client.0x6FB24110
//1.13c: D2Client.0x6FB6EAF0
D2CellFileStrc* __fastcall D2CLIENT_CELLFILE_GetBuySellButtons()
{
	if (!*D2CLIENT_pgpCellfile_BUYSELLBUTTONS)
	{
		CHAR szFilename[260];
		wsprintfA(szFilename, "%s\\ui\\panel\\buysellbtn", "DATA\\GLOBAL");
		*D2CLIENT_pgpCellfile_BUYSELLBUTTONS = D2CLIENT_CELLFILE_GetCellFile(szFilename, 0);
	}
	return *D2CLIENT_pgpCellfile_BUYSELLBUTTONS;
}

//1.00 : Inlined
//1.10f: Inlined
//1.13c: Inlined
void __fastcall UI_INV_DrawCloseButton()
{
	D2GfxDataStrc tGfxData{};
	tGfxData.pCellFile = D2CLIENT_CELLFILE_GetBuySellButtons();
	tGfxData.nFrame = (*D2CLIENT_pgbInvCloseButtonPressed != 0) + BUYSELLBTN_CLOSE;
	TEXTURE_CelDraw(&tGfxData,
		*D2CLIENT_pgnScreenWidth - (*D2CLIENT_pgnInvPanelOffsetX + kInvEquipCloseButtonOffset.nX + kBuySellButtonSize),
		*D2CLIENT_pgnInvPanelOffsetY + *D2CLIENT_pgnScreenHeight - kInvEquipCloseButtonOffset.nY,
		-1, DRAWMODE_NORMAL, 0);

}

//1.10f: D2CLient.0x6FAE1710
//1.13c: D2CLient.0x6FB3C600
void __fastcall UI_INV_DrawItemBackgroundOnGrid(
	D2InventoryGridInfoStrc* pGridInfo,
	int nItemGridPosX, int nItemGridPosY, 
	uint8_t nItemGridWidth, uint8_t nItemGridHeight,
	UI_ItemBackgroundColor nBgColor)
{
	const int32_t nEndBoxX = nItemGridPosX + nItemGridWidth;
	const int32_t nEndBoxY = nItemGridPosY + nItemGridHeight;
	for (int32_t nCurBoxY = nItemGridPosY; nCurBoxY < nEndBoxY; nCurBoxY++)
	{
		for (int32_t nCurBoxX = nItemGridPosX; nCurBoxX < nEndBoxX; ++nCurBoxX)
		{
			const int32_t nPosX = pGridInfo->nGridLeft + nCurBoxX * pGridInfo->nGridBoxWidth;
			const int32_t nPosY = pGridInfo->nGridTop + nCurBoxY * pGridInfo->nGridBoxHeight;
			UI_INV_DrawItemBackgroundRect({ nPosX, nPosY }, pGridInfo->nGridBoxWidth, pGridInfo->nGridBoxHeight, nBgColor);
		}
	}
}

//1.10f: D2Client.0x6FAED310
//1.13c: D2Client.0x6FB3E8E0
void __fastcall UI_INV_DrawHirelingItemBackground(D2UnitStrc* pHireling)
{
	if (D2UnitStrc* pCursorItem = INVENTORY_GetCursorItem(D2CLIENT_GetControlUnit()->pInventory))
	{	
		for (int nBodyLoc = 0; nBodyLoc < NUM_BODYLOC_NO_SWITCH; nBodyLoc++)
		{
			const D2InvCompGridStrc& rInvCompGrid = (*D2CLIENT_pgaHirelingInvCompGrids)[nBodyLoc];
			D2CoordStrc tMouseCoord = { CLIENT_GetMouseX(), CLIENT_GetMouseY() };
			if (IsInRectangle(tMouseCoord, rInvCompGrid.rect))
			{
				auto nBgColor = UI_ItemBgColor_Red;
				//Results optimized away
				//int nItemWidth, nItemHeight;
				//D2COMMON_10749_GetItemDimensions(pCursorItem, &nItemWidth, &nItemHeight, "..\\D2Common/CmnItems.h", 1754);
				
				BOOL bItemTypeCompatible;
				if (ITEMS_CheckBodyLocation(pCursorItem, D2C_PlayerBodyLocs(nBodyLoc))
					&& D2CLIENT_UI_INV_HirelingCanEquip_6FAED470(pHireling, pCursorItem, &bItemTypeCompatible)
					&& ITEMS_CheckRequirements(pCursorItem, pHireling, 0, 0, 0, 0))
				{
					nBgColor = UI_ItemBgColor_Green;
				}
				UI_INV_DrawItemBackgroundRect({ rInvCompGrid.rect.nLeft, rInvCompGrid.rect.nTop }, rInvCompGrid.nWidth, rInvCompGrid.nHeight, nBgColor);
			}
		}
	}
}


//1.10f: D2Client.0x6FAE4060
//1.13c: D2Client.0x6FB40770
void __fastcall ITEMDESC_Durability(D2UnitStrc* pUnit, Unicode* pOutBuffer, const D2ItemsTxt* pItemTxt)
{
	D2_MAYBE_UNUSED(pItemTxt);

	const Unicode* wszSpace = D2LANG_GetStringFromTblIndex(STR_IDX_3995_space);
	const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);
	if (ITEMS_HasDurability(pUnit) && STATLIST_GetMaxDurabilityFromUnit(pUnit) > 0 && !ITEMS_CheckIfThrowable(pUnit))
	{
		char szBuffer[10];
		Unicode wszBuffer[10];
		Unicode::strcpy(pOutBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3457_ItemStats1d));
		SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", STATLIST_UnitGetStatValue(pUnit, STAT_DURABILITY, 0));
		Unicode::strcat(pOutBuffer, wszSpace);
		Unicode::win2Unicode(wszBuffer, szBuffer, ARRAY_SIZE(wszBuffer) - 1);
		Unicode::strcat(pOutBuffer, wszBuffer);
		Unicode::strcat(pOutBuffer, wszSpace);
		Unicode::strcat(pOutBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3463_ItemStats1j));
		Unicode::strcat(pOutBuffer, wszSpace);
		SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", STATLIST_GetMaxDurabilityFromUnit(pUnit));
		Unicode::win2Unicode(wszBuffer, szBuffer, ARRAY_SIZE(szBuffer)-1);
		if (STATLIST_GetUnitStatBonus(pUnit, STAT_ITEM_MAXDURABILITY_PERCENT, 0) != 0)
			TEXT_ApplyColorCode(wszBuffer, 3);
		Unicode::strcat(pOutBuffer, wszBuffer);
		Unicode::strcat(pOutBuffer, wszNewline);
	}
}

//1.10f: D2Client.0x6FAE41B0
//1.13c: D2Client.0x6FB3DE40
void __fastcall ITEMDESC_RequiredLevel(Unicode* pOutBuffer, int nReqLvl)
{
	const Unicode* wszSpace = D2LANG_GetStringFromTblIndex(STR_IDX_3995_space);
	const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);

	char szBuffer[10];
	Unicode wszBuffer[10];
	Unicode::strcpy(pOutBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3469_ItemStats1p));
	SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nReqLvl);
	Unicode::win2Unicode(wszBuffer, szBuffer, ARRAY_SIZE(wszBuffer) - 1);
	Unicode::strcat(pOutBuffer, wszSpace);
	Unicode::strcat(pOutBuffer, wszBuffer);
	Unicode::strcat(pOutBuffer, wszNewline);
}

//1.10f: D2Client.0x6FAE4250
//1.13c: D2Client.0x6FB3DD80
void __fastcall ITEMDESC_RequiredStrength(Unicode* pOutBuffer, const D2ItemsTxt* pItemTxt, int nRequiredStr)
{
	const Unicode* wszSpace = D2LANG_GetStringFromTblIndex(STR_IDX_3995_space);
	const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);

	nRequiredStr += pItemTxt->wReqStr;

	if (nRequiredStr > 0)
	{
		char szBuffer[10];
		Unicode wszBuffer[10];
		Unicode::strcpy(pOutBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3458_ItemStats1e));
		SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nRequiredStr);
		Unicode::win2Unicode(wszBuffer, szBuffer, ARRAY_SIZE(wszBuffer) - 1);
		Unicode::strcat(pOutBuffer, wszSpace);
		Unicode::strcat(pOutBuffer, wszBuffer);
		Unicode::strcat(pOutBuffer, wszNewline);
	}
	else
	{
		*pOutBuffer = L'\0';
	}
}

//1.10f: D2Client.0x6FAE4310
//1.13c: D2Client.0x6FB3DCC0
void __fastcall ITEMDESC_RequiredDexterity(Unicode* pOutBuffer, const D2ItemsTxt* pItemTxt, int nRequiredDex)
{
	const Unicode* wszSpace = D2LANG_GetStringFromTblIndex(STR_IDX_3995_space);
	const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);

	nRequiredDex += pItemTxt->wReqDex;

	if (nRequiredDex > 0)
	{
		char szBuffer[10];
		Unicode wszBuffer[10];
		Unicode::strcpy(pOutBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3459_ItemStats1f));
		SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nRequiredDex);
		Unicode::win2Unicode(wszBuffer, szBuffer, ARRAY_SIZE(wszBuffer) - 1);
		Unicode::strcat(pOutBuffer, wszSpace);
		Unicode::strcat(pOutBuffer, wszBuffer);
		Unicode::strcat(pOutBuffer, wszNewline);
	}
	else
	{
		*pOutBuffer = L'\0';
	}
}


#pragma pack(push, 1)
struct WeaponClassDesc
{
	D2C_ItemTypes nItemType;
	D2C_StringIndices nStringId;
};
#pragma pack(pop)

WeaponClassDesc gWeaponClassDescs_6FB792D8[] = {
	{ ITEMTYPE_STAFF, STR_IDX_4085_WeaponDescStaff },
	{ ITEMTYPE_AXE, STR_IDX_4078_WeaponDescAxe },
	{ ITEMTYPE_SWORD, STR_IDX_4079_WeaponDescSword },
	{ ITEMTYPE_KNIFE, STR_IDX_4080_WeaponDescDagger },
	{ ITEMTYPE_MISSILE_POTION, STR_IDX_4081_WeaponDescThrownPotion },
	{ ITEMTYPE_JAVELIN, STR_IDX_4082_WeaponDescJavelin },
	{ ITEMTYPE_SPEAR, STR_IDX_4083_WeaponDescSpear },
	{ ITEMTYPE_BOW, STR_IDX_4084_WeaponDescBow },
	{ ITEMTYPE_POLEARM, STR_IDX_4086_WeaponDescPoleArm },
	{ ITEMTYPE_CROSSBOW, STR_IDX_4087_WeaponDescCrossBow },
	{ ITEMTYPE_HAND_TO_HAND, STR_IDX_21258_WeaponDescH2H },
	{ ITEMTYPE_HAND_TO_HAND_2, STR_IDX_21258_WeaponDescH2H },
	{ ITEMTYPE_ORB, STR_IDX_4085_WeaponDescStaff },
	{ ITEMTYPE_WAND, STR_IDX_4085_WeaponDescStaff },
	{ ITEMTYPE_BLUNT, STR_IDX_4077_WeaponDescMace },
};

#pragma pack(push, 1)
struct WeaponSpeedInfo
{
	int32_t nSpeedId;
	D2C_StringIndices nStringId;
};
#pragma pack(pop)

WeaponSpeedInfo gnWeaponSpeedsDesc_6FB79334[]{
	{ 0, STR_IDX_4088_WeaponAttackFastest },
	{ 1, STR_IDX_4089_WeaponAttackVeryFast },
	{ 2, STR_IDX_4090_WeaponAttackFast },
	{ 3, STR_IDX_4091_WeaponAttackNormal },
	{ 4, STR_IDX_4092_WeaponAttackSlow },
	{ 5, STR_IDX_4093_WeaponAttackVerySlow },
	{ 6, STR_IDX_4094_WeaponAttackSlowest },
};
int ganWeaponSpeedClassToSpeedDescIdx_6FB79360[18][5] = {
	{ 1, 1, 1, 1, 1},
	{ 1, 1, 1, 1, 1},
	{ 1, 1, 1, 1, 1},
	{ 1, 1, 2, 1, 1},
	{ 2, 1, 2, 2, 1},
	{ 2, 1, 2, 2, 2},
	{ 2, 2, 3, 2, 2},
	{ 3, 2, 3, 3, 2},
	{ 3, 2, 3, 3, 3},
	{ 3, 2, 4, 3, 3},
	{ 4, 3, 4, 4, 3},
	{ 4, 3, 4, 4, 4},
	{ 4, 3, 5, 4, 4},
	{ 5, 4, 5, 5, 4},
	{ 5, 4, 5, 5, 5},
	{ 5, 4, 5, 5, 5},
	{ 5, 5, 5, 5, 5},
	{ 5, 5, 5, 5, 5},
};
int gaClassAndDistanceToSpeedClass_6FB794C8[NUMBER_OF_PLAYERCLASSES][2] = {
	{ 0, 2 },
	{ 1, 4 },
	{ 1, 4 },
	{ 0, 3 },
	{ 0, 3 },
	{ 1, 4 },
	{ 0, 3 },
};

//1.10f: D2Client.0x6FAE5570
//1.13c: D2Client.0x6FB3F9E0
void __fastcall ITEMDESC_AttackSpeed_WeaponClass(D2UnitStrc* pItem, Unicode* pOutBuffer, const D2ItemsTxt* pItemTxt)
{

	const Unicode* wszSpace = D2LANG_GetStringFromTblIndex(STR_IDX_3995_space);
	const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);
	const Unicode* wszDash = D2LANG_GetStringFromTblIndex(STR_IDX_3996_dash);
	const int32_t nWeaponAttackSpeed = ITEMS_GetWeaponAttackSpeed(D2CLIENT_GetControlUnit(), pItem);
	int nSpeedTextIdx = 0;
	if (nWeaponAttackSpeed >= 28)
	{
		nSpeedTextIdx = 5;
	}
	else if (nWeaponAttackSpeed >= 10)
	{
		const bool bIsDistanceWeapon = ITEMS_CheckItemTypeId(pItem, ITEMTYPE_CROSSBOW) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOW);
		const int dwClassId = D2CLIENT_GetControlUnit() ? D2CLIENT_GetControlUnit()->dwClassId : -1;
		nSpeedTextIdx = ganWeaponSpeedClassToSpeedDescIdx_6FB79360[nWeaponAttackSpeed - 10][gaClassAndDistanceToSpeedClass_6FB794C8[dwClassId][bIsDistanceWeapon]];
	}
	else
	{
		nSpeedTextIdx = 1;
	}

	for (auto& rWeaponClassDesc : gWeaponClassDescs_6FB792D8)
	{
		if (ITEMS_CheckItemTypeId(pItem, rWeaponClassDesc.nItemType))
		{
			Unicode::strcpy(pOutBuffer, D2LANG_GetStringFromTblIndex(rWeaponClassDesc.nStringId));
			Unicode::strcat(pOutBuffer, wszSpace);
			Unicode::strcat(pOutBuffer, wszDash);
			Unicode::strcat(pOutBuffer, wszSpace);
			break;
		}
	}
	Unicode wszSpeedDesc[256];
	Unicode::strcpy(wszSpeedDesc, D2LANG_GetStringFromTblIndex(gnWeaponSpeedsDesc_6FB79334[nSpeedTextIdx].nStringId));
	if (STATLIST_GetUnitStatBonus(pItem, STAT_ITEM_FASTERATTACKRATE, 0) != 0)
		TEXT_ApplyColorCode(wszSpeedDesc, 3);
	Unicode::strcat(pOutBuffer, wszSpeedDesc);
	Unicode::strcat(pOutBuffer, wszNewline);
}

//1.10f: D2Client.0x6FAE4C60
//1.13c: D2Client.0x6FB3DAD0
void __fastcall ITEMDESC_GetMinMaxStats(
	D2UnitStrc* pPlayer, D2UnitStrc* pItem, 
	D2C_ItemStats nMinStatId, D2C_ItemStats nMaxStatId, 
	int32_t* pMinValue, int32_t* pMaxValue,	BOOL* bHasBonus)
{
	*pMinValue = 1;
	*pMaxValue = 2;
	if (pPlayer && pItem)
	{
		BOOL bDynamic = 0;
		D2UnitStrc* pItemOwner = STATLIST_GetOwner(*D2CLIENT_pgpSelectedItem, &bDynamic);
		if (pItemOwner && pItemOwner != pPlayer)
		{
			pPlayer = pItemOwner;
		}
		STATLIST_MergeStatLists(pPlayer, pItem, TRUE);
		int32_t nMinValue = STATLIST_UnitGetStatValue(pItem, nMinStatId, 0);
		int32_t nMaxValue = STATLIST_UnitGetStatValue(pItem, nMaxStatId, 0);
		if (pItemOwner)
			STATLIST_MergeStatLists(pPlayer, pItem, bDynamic);
		else
			STATLIST_ExpireUnitStatlist(pPlayer, pItem);
		const int32_t nBaseStatMinValue = STATLIST_GetUnitBaseStat(pItem, nMinStatId, 0);
		const int32_t nBaseStatMaxValue = STATLIST_GetUnitBaseStat(pItem, nMaxStatId, 0);
		if (nBaseStatMinValue < nMinValue || nBaseStatMaxValue < nMaxValue)
			*bHasBonus = TRUE;
		if (nMaxValue <= nMinValue)
			nMaxValue = nMinValue;
		const int32_t nMaxDmgByTime = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_MAXDAMAGE_BYTIME, 0);
		if (nMaxDmgByTime && CLIENT_DATATBLS_GetItemStatCostTxtRecord(STAT_ITEM_MAXDAMAGE_BYTIME) && pPlayer->pDrlgAct)
		{
			int32_t nBaseTime = 0;
			const int nDayTimeFromAct = ENVIRONMENT_GetPeriodOfDayFromAct(pPlayer->pDrlgAct, &nBaseTime);
			const int32_t nByTimeAdjustment = ITEMMODS_GetByTimeAdjustment(nMaxDmgByTime, nDayTimeFromAct, nBaseTime, nullptr, nullptr, nullptr);
			nMaxValue += nByTimeAdjustment;
			if (nByTimeAdjustment)
				*bHasBonus = TRUE;
		}
		const int32_t nMaxDmgPercByTime = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_MAXDAMAGE_PERCENT_BYTIME, 0);
		if (nMaxDmgPercByTime && CLIENT_DATATBLS_GetItemStatCostTxtRecord(STAT_ITEM_MAXDAMAGE_PERCENT_BYTIME) && pPlayer->pDrlgAct)
		{
			int nBaseTime = 0;
			int nDayTimeFromAct = ENVIRONMENT_GetPeriodOfDayFromAct(pPlayer->pDrlgAct, &nBaseTime);
			int nByTimeAdjustment = ITEMMODS_GetByTimeAdjustment(nMaxDmgPercByTime, nDayTimeFromAct, nBaseTime, nullptr, nullptr, nullptr);
			nMaxValue += D2_ComputePercentage(nMaxValue, nByTimeAdjustment);
			if (nByTimeAdjustment)
				*bHasBonus = TRUE;
		}
		*pMinValue = nMinValue;
		*pMaxValue = nMaxValue;
	}
}

//1.10f: D2Client.0x6FAE43D0
//1.13c: D2Client.0x6FB3FF20
void __fastcall ITEMDESC_Damage(D2UnitStrc* pItem, Unicode* pOutBuffer, const D2ItemsTxt* pItemTxt)
{
	char szBuffer[256];
	Unicode wszMinMaxDmg[256];

	const Unicode* wszSpace = D2LANG_GetStringFromTblIndex(STR_IDX_3995_space);
	const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);
	D2UnitStrc* hControlUnit = D2CLIENT_GetControlUnit();
	if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_MISSILE_POTION))
	{
		Unicode wszDescBuffer[256];
		const int nItemMissileType = ITEMS_GetMissileType(pItem);
		int32_t nMinElemDmgFP = MISSILE_GetMinElemDamage(0, hControlUnit, nItemMissileType, 1);
		int32_t nMaxElemDmgFP = MISSILE_GetMaxElemDamage(0, hControlUnit, nItemMissileType, 1);
		Unicode::strcpy(wszDescBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3467_ItemStats1n));
		TEXT_ApplyColorCode(wszDescBuffer, 0);
		int nMinMaxDmgColorIdx = 0;
		switch ((D2C_ElemTypes)MISSILE_GetElemTypeFromMissileId(nItemMissileType))
		{
		case ELEMTYPE_FIRE:
			nMinMaxDmgColorIdx = 1;
			break;
		case ELEMTYPE_LTNG:
			nMinMaxDmgColorIdx = 4;
			break;
		case ELEMTYPE_COLD:
			nMinMaxDmgColorIdx = 3;
			break;
		case ELEMTYPE_POIS:
		{
			nMinMaxDmgColorIdx = 2;
			int32_t nPoisonLength = MISSILE_GetElementalLength(0, hControlUnit, nItemMissileType, 1) / 25;
			if (nPoisonLength <= 0)
				nPoisonLength = 1;
			nMinElemDmgFP /= nPoisonLength;
			nMaxElemDmgFP /= nPoisonLength;
			break;
		}
		default:
			break;
		}
		const int32_t nMinDmgFP = nMinElemDmgFP + MISSILE_GetMinDamage(0, hControlUnit, nItemMissileType, 1);
		const int32_t nMaxDmgFP = nMaxElemDmgFP + MISSILE_GetMaxDamage(0, hControlUnit, nItemMissileType, 1);
		const int32_t nMinDmg = nMinDmgFP >> 8;
		int32_t nMaxDmg = nMaxDmgFP >> 8;
		if (nMaxDmg <= nMinDmg)
			nMaxDmg = nMinDmg;
		SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMinDmg);
		Unicode::win2Unicode(wszMinMaxDmg, szBuffer, 9);
		TEXT_ApplyColorCode(wszMinMaxDmg, nMinMaxDmgColorIdx);
		Unicode::strcat(wszDescBuffer, wszSpace);
		Unicode::strcat(wszDescBuffer, wszMinMaxDmg);
		if (nMinDmgFP != nMaxDmg)
		{
			Unicode::strcat(wszDescBuffer, wszSpace);
			Unicode::strcat(wszDescBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3464_ItemStast1k));
			SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMaxDmg);
			Unicode::win2Unicode(wszMinMaxDmg, szBuffer, 9);
			TEXT_ApplyColorCode(wszMinMaxDmg, nMinMaxDmgColorIdx);
			Unicode::strcat(wszDescBuffer, wszSpace);
			Unicode::strcat(wszDescBuffer, wszMinMaxDmg);
		}
		Unicode::strcat(wszDescBuffer, wszNewline);
		Unicode::strcpy(pOutBuffer, wszDescBuffer);
	}
	else
	{
		int nMinDmg, nMaxDmg;
		BOOL bHasBonus = FALSE;
		if (ITEMS_Is1Or2HandedForBarbarian(hControlUnit, pItem))
		{
			ITEMDESC_GetMinMaxStats(
				hControlUnit, pItem,
				STAT_SECONDARY_MINDAMAGE, STAT_SECONDARY_MAXDAMAGE,
				&nMinDmg,&nMaxDmg, &bHasBonus);
			Unicode wszBufferSecondary[128];
			Unicode::strcpy(wszBufferSecondary, D2LANG_GetStringFromTblIndex(STR_IDX_3466_ItemStats1m));
			Unicode::strcat(wszBufferSecondary, wszSpace);
			SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMinDmg);
			Unicode::win2Unicode(wszMinMaxDmg, szBuffer, 5);
			if (bHasBonus) TEXT_ApplyColorCode(wszMinMaxDmg, 3);
			Unicode::strcat(wszBufferSecondary, wszMinMaxDmg);
			Unicode::strcat(wszBufferSecondary, wszSpace);
			Unicode::strcat(wszBufferSecondary, D2LANG_GetStringFromTblIndex(STR_IDX_3464_ItemStast1k));
			SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMaxDmg);
			Unicode::win2Unicode(wszMinMaxDmg, szBuffer, 5);
			Unicode::strcat(wszBufferSecondary, wszSpace);
			Unicode::strcat(wszBufferSecondary, wszMinMaxDmg);
			Unicode::strcat(wszBufferSecondary, wszNewline);

			ITEMDESC_GetMinMaxStats(
				hControlUnit, pItem,
				STAT_MINDAMAGE, STAT_MAXDAMAGE,
				&nMinDmg, &nMaxDmg, &bHasBonus);
			Unicode wszBufferPrimary[128];
			Unicode::strcpy(wszBufferPrimary, D2LANG_GetStringFromTblIndex(STR_IDX_3465_ItemStats1l));
			Unicode::strcat(wszBufferPrimary, wszSpace);
			SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMinDmg);
			Unicode::win2Unicode(wszMinMaxDmg, szBuffer, 5);
			if (bHasBonus) TEXT_ApplyColorCode(wszMinMaxDmg, 3);
			Unicode::strcat(wszBufferPrimary, wszMinMaxDmg);
			Unicode::strcat(wszBufferPrimary, wszSpace);
			Unicode::strcat(wszBufferPrimary, D2LANG_GetStringFromTblIndex(STR_IDX_3464_ItemStast1k));
			SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMaxDmg);
			Unicode::win2Unicode(wszMinMaxDmg, szBuffer, 5);
			Unicode::strcat(wszBufferPrimary, wszSpace);
			Unicode::strcat(wszBufferPrimary, wszMinMaxDmg);
			Unicode::strcat(wszBufferPrimary, wszNewline);

			TEXT_ApplyColorCode(wszBufferSecondary, 0);
			TEXT_ApplyColorCode(wszBufferPrimary, 0);
			Unicode::strcpy(pOutBuffer, wszBufferSecondary);
			Unicode::strcat(pOutBuffer, wszBufferPrimary);
		}
		else
		{
			D2C_StringIndices nStringIdx;
			if (ITEMS_CheckWeaponIfTwoHanded(pItem))
			{
				ITEMDESC_GetMinMaxStats(
					hControlUnit, pItem,
					STAT_SECONDARY_MINDAMAGE, STAT_SECONDARY_MAXDAMAGE,
					&nMinDmg, &nMaxDmg, &bHasBonus);
				nStringIdx = STR_IDX_3466_ItemStats1m;
			}
			else
			{
				ITEMDESC_GetMinMaxStats(
					hControlUnit, pItem,
					STAT_MINDAMAGE, STAT_MAXDAMAGE,
					&nMinDmg, &nMaxDmg, &bHasBonus);
				nStringIdx = STR_IDX_3465_ItemStats1l;
			}
			if (nMaxDmg <= nMinDmg + 1)
				nMaxDmg = nMinDmg + 1;
			Unicode::strcpy(pOutBuffer, D2LANG_GetStringFromTblIndex(nStringIdx));
			SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMinDmg);
			Unicode::win2Unicode(wszMinMaxDmg, szBuffer, 9);
			Unicode::strcat(pOutBuffer, wszSpace);
			if (bHasBonus) TEXT_ApplyColorCode(wszMinMaxDmg, 3);
			Unicode::strcat(pOutBuffer, wszMinMaxDmg);
			Unicode::strcat(pOutBuffer, wszSpace);
			Unicode::strcat(pOutBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3464_ItemStast1k));
			SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMaxDmg);
			Unicode::win2Unicode(wszMinMaxDmg, szBuffer, 9);
			Unicode::strcat(pOutBuffer, wszSpace);
			Unicode::strcat(pOutBuffer, wszMinMaxDmg);
			Unicode::strcat(pOutBuffer, wszNewline);
		}
		bHasBonus = STATLIST_GetUnitStatBonus(pItem, STAT_ITEM_MINDAMAGE_PERCENT, 0)
			|| STATLIST_GetUnitStatBonus(pItem, STAT_ITEM_MAXDAMAGE_PERCENT, 0)
			|| STATLIST_GetUnitStatBonus(pItem, STAT_ITEM_THROW_MINDAMAGE, 0)
			|| STATLIST_GetUnitStatBonus(pItem, STAT_ITEM_THROW_MAXDAMAGE, 0)
			;
		if (ITEMS_CheckIfThrowable(pItem))
		{
			Unicode wszBuffer[128];
			Unicode::strcpy(wszBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3467_ItemStats1n));
			TEXT_ApplyColorCode(wszBuffer, 0);
			ITEMDESC_GetMinMaxStats(
				hControlUnit, pItem,
				STAT_ITEM_THROW_MINDAMAGE, STAT_ITEM_THROW_MAXDAMAGE,
				&nMinDmg, &nMaxDmg, &bHasBonus);
			SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMinDmg);
			Unicode::win2Unicode(wszMinMaxDmg, szBuffer, 9);
			TEXT_ApplyColorCode(wszMinMaxDmg, bHasBonus ? 3 : 0);
			Unicode::strcat(wszBuffer, wszSpace);
			Unicode::strcat(wszBuffer, wszMinMaxDmg);
			Unicode::strcat(wszBuffer, wszSpace);
			Unicode::strcat(wszBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3464_ItemStast1k));
			SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMaxDmg);
			Unicode::win2Unicode(wszMinMaxDmg, szBuffer, 9);
			TEXT_ApplyColorCode(wszMinMaxDmg, bHasBonus ? 3 : 0);
			Unicode::strcat(wszBuffer, wszSpace);
			Unicode::strcat(wszBuffer, wszMinMaxDmg);
			Unicode::strcat(wszBuffer, wszNewline);
			Unicode::strcat(pOutBuffer, wszBuffer);
		}
	}
}

//1.10f: D2Client.0x6FAE5040
//1.13c: D2Client.0x6FB3D940
void __fastcall ITEMDESC_SmiteAndKickDamage(D2UnitStrc* pItem, Unicode* pOutBuffer, const D2ItemsTxt* pItemTxt)
{
	char szBuffer[10];
	Unicode wszBuffer[10];
	const Unicode* wszSpace = D2LANG_GetStringFromTblIndex(STR_IDX_3995_space);
	const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);
	int32_t nMinDam = pItemTxt->nMinDam;
	int32_t nMaxDam = pItemTxt->nMaxDam;

	D2C_StringIndices nDescId = STR_IDX_21782_ModStre10k;
	if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_SHIELD))
	{
		nDescId = STR_IDX_3468_ItemStats1o;
		D2UnitStrc* hControlUnit = D2CLIENT_GetControlUnit();
		if (D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(hControlUnit, SKILL_HOLYSHIELD))
		{
			if (STATES_CheckState(hControlUnit, STATE_HOLYSHIELD))
			{
				const int32_t nSkillLevel = SKILLS_GetSkillLevel(hControlUnit, pSkill, 1);
				nMinDam += SKILLS_GetMinPhysDamage(hControlUnit, SKILL_HOLYSHIELD, nSkillLevel, 1) >> 8;
				nMaxDam += SKILLS_GetMaxPhysDamage(hControlUnit, SKILL_HOLYSHIELD, nSkillLevel, 1) >> 8;
			}
		}
	}
	else if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOTS))
	{
			return;
	}
	Unicode::strcpy(pOutBuffer, D2LANG_GetStringFromTblIndex(nDescId));
	SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMinDam);
	Unicode::win2Unicode(wszBuffer, szBuffer, ARRAY_SIZE(wszBuffer) - 1);
	Unicode::strcat(pOutBuffer, wszSpace);
	Unicode::strcat(pOutBuffer, wszBuffer);
	Unicode::strcat(pOutBuffer, wszSpace);
	Unicode::strcat(pOutBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3464_ItemStast1k));
	SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nMaxDam);
	Unicode::win2Unicode(wszBuffer, szBuffer, ARRAY_SIZE(wszBuffer) - 1);
	Unicode::strcat(pOutBuffer, wszSpace);
	Unicode::strcat(pOutBuffer, wszBuffer);
	Unicode::strcat(pOutBuffer, wszNewline);
}

//1.10f: D2Client.0x6FAE4EE0
//1.13c: D2Client.0x6FB3FDC0
void __fastcall ITEMDESC_Blockchance(D2UnitStrc* pUnit, Unicode* pBuffer, const D2ItemsTxt* pItemTxt)
{
	char szBuffer[32];
	Unicode wszBlockBuffer[256];
	Unicode wszBuffer[32];

	int32_t nBlock = STATLIST_UnitGetStatValue(pUnit, STAT_TOBLOCK, 0);

	if (D2UnitStrc* hControlUnit = D2CLIENT_GetControlUnit())
	{
		nBlock += CLIENT_DATATBLS_GetCharStatsTxtRecord(hControlUnit->dwClassId)->nBlockFactor;
		D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(hControlUnit, SKILL_HOLYSHIELD);
		if (pSkill && STATES_CheckState(hControlUnit, STATE_HOLYSHIELD))
		{
			int32_t nSkillLevel = SKILLS_GetSkillLevel(hControlUnit, pSkill, 1);
			nBlock += D2COMMON_11036_GetMonCurseResistanceSubtraction(nSkillLevel, SKILL_HOLYSHIELD);
		}
	}

	if (nBlock > 75)
	{
		nBlock = 75;
	}
	else if (nBlock == 0)
	{
		return;
	}
	SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%d%%\n", nBlock);
	Unicode::win2Unicode(wszBuffer, szBuffer, ARRAY_SIZE(wszBuffer));
	if (nBlock > pItemTxt->nBlock)
		TEXT_ApplyColorCode(wszBuffer, 3);
	Unicode::strcpy(wszBlockBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_11018_ItemStats1r));
	TEXT_ApplyColorCode(wszBlockBuffer, 0);
	Unicode::strcpy(pBuffer, wszBlockBuffer);
	Unicode::strcat(pBuffer, wszBuffer);
}

//1.10f: D2Client.0x6FAE51D0
//1.13c: D2Client.0x6FB3FB80
void __fastcall ITEMDESC_Defense(D2UnitStrc* pUnit, D2UnitStrc* pItem, Unicode* pBuffer, const D2ItemsTxt* pItemTxt)
{
	if (!pUnit) return;
		
	const Unicode* wszSpace = D2LANG_GetStringFromTblIndex(STR_IDX_3995_space);
	const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);
	BOOL bDynamic = 0;
	D2UnitStrc* pOwner = STATLIST_GetOwner(*D2CLIENT_pgpSelectedItem, &bDynamic);
	;
	if (pOwner && pUnit != pOwner)
		pUnit = pOwner;
	STATLIST_MergeStatLists(pUnit, *D2CLIENT_pgpSelectedItem, TRUE);
	int32_t nItemDefense = STATLIST_GetDefenseFromUnit(*D2CLIENT_pgpSelectedItem);
	if (pOwner)
		STATLIST_MergeStatLists(pOwner, *D2CLIENT_pgpSelectedItem, bDynamic);
	else
		STATLIST_ExpireUnitStatlist(pUnit, *D2CLIENT_pgpSelectedItem);
	bool bHasBonus = STATLIST_GetUnitBaseStat(pItem, STAT_ARMORCLASS, 0) != nItemDefense;

	int32_t nArmorByTime = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_ARMOR_BYTIME, 0);
	if (nArmorByTime && CLIENT_DATATBLS_GetItemStatCostTxtRecord(STAT_ITEM_ARMOR_BYTIME) && pUnit->pDrlgAct)
	{
		int nBaseTime = 0;
		const int nDayTimeFromAct = ENVIRONMENT_GetPeriodOfDayFromAct(pUnit->pDrlgAct, &nBaseTime);
		const int nByTimeAdjustment = ITEMMODS_GetByTimeAdjustment(nArmorByTime, nDayTimeFromAct, nBaseTime, 0, 0, 0);
		nItemDefense += nByTimeAdjustment;
		if (nByTimeAdjustment)
			bHasBonus = true;
	}
	int32_t nArmorPctByTime = STATLIST_UnitGetStatValue(pItem, STAT_ITEM_ARMORPERCENT_BYTIME, 0);
	if (nArmorPctByTime && CLIENT_DATATBLS_GetItemStatCostTxtRecord(STAT_ITEM_ARMORPERCENT_BYTIME) && pUnit->pDrlgAct)
	{
		int nBaseTime = 0;
		const int nDayTimeFromAct = ENVIRONMENT_GetPeriodOfDayFromAct(pUnit->pDrlgAct, &nBaseTime);
		const int nByTimeAdjustment = ITEMMODS_GetByTimeAdjustment(nArmorPctByTime, nDayTimeFromAct, nBaseTime, 0, 0, 0);
		nItemDefense += D2_ComputePercentage(nItemDefense, nByTimeAdjustment);
		if (nByTimeAdjustment)
			bHasBonus = true;
	}
	Unicode::strcpy(pBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3461_ItemStats1h));
	Unicode::strcat(pBuffer, wszSpace);
	char szBuffer[10];
	SStrPrintf(szBuffer, ARRAY_SIZE(szBuffer), "%i", nItemDefense);
	Unicode wszBuffer[10];
	Unicode::win2Unicode(wszBuffer, szBuffer, ARRAY_SIZE(wszBuffer) - 1);
	if (bHasBonus)
		TEXT_ApplyColorCode(wszBuffer, 3);
	Unicode::strcat(pBuffer, wszBuffer);
	Unicode::strcat(pBuffer, wszNewline);
}

//1.10f: D2Client.0x6FAE3EE0
//1.13c: D2Client.0x6FB3E180
void __fastcall ITEMDESC_SocketsAndEthereality(D2UnitStrc* pItem, Unicode* pOutBuffer)
{
	Unicode wszBuffer[16];
	const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);
	
	Unicode::win2Unicode(wszBuffer, "", ARRAY_SIZE(wszBuffer) - 1);
	Unicode::strcpy(pOutBuffer, wszBuffer);
	const bool bIsEthereal = ITEMS_CHECK_FLAG(pItem, IFLAG_ETHEREAL);
	if (bIsEthereal)
	{
		Unicode::win2Unicode(wszBuffer, "Ethereal", ARRAY_SIZE(wszBuffer) - 1);
		Unicode::strcat(pOutBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_22745_X_Ethereal_CanNotBeRepaired));
	}
	if (ITEMS_CHECK_FLAG(pItem, IFLAG_SOCKETED))
	{
		if (bIsEthereal)
		{
			Unicode::win2Unicode(wszBuffer, ", ", ARRAY_SIZE(wszBuffer) - 1);
			Unicode::strcat(pOutBuffer, wszBuffer);
		}
		Unicode::strcat(pOutBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3453_Socketable));
		char szSocketDesc[10];
		Unicode wszSocketsDesc[10];
		const Unicode* wszSpace = D2LANG_GetStringFromTblIndex(STR_IDX_3995_space);
		int32_t nItemSockets = ITEMS_GetSockets(pItem);
		SStrPrintf(szSocketDesc, ARRAY_SIZE(szSocketDesc), "(%i)", nItemSockets);
		Unicode::win2Unicode(wszSocketsDesc, szSocketDesc, ARRAY_SIZE(wszSocketsDesc) - 1);
		Unicode::strcat(pOutBuffer, wszSpace);
		Unicode::strcat(pOutBuffer, wszSocketsDesc);
		Unicode::strcat(pOutBuffer, wszNewline);
	}
	else if (bIsEthereal)
	{
		Unicode::strcat(pOutBuffer, wszNewline);
	}
}

static int UnicodeFindLastChar(const Unicode* wszStr, int nLastCharIdx, Unicode wCharacter)
{
	int nIdx = nLastCharIdx;
	while (nIdx >= 0)
	{
		if (wszStr[nIdx].ch != wCharacter)
		{
			return nIdx;
		}
		nIdx--;
	}
	return nIdx;
}

//1.10f: D2Client.0x6FAF3160
//1.13c: D2Client.0x6FB060B0
void __fastcall ITEMDESC_RunewordMods(
	D2UnitStrc* pItem, Unicode* pBuffer, int nBufferSize, BOOL bAppendNewLine, Unicode* pDescString)
{
	Unicode wszModDesc[1024];
	wszModDesc[0] = 0;
	ITEMDESC_Mods_6FAF19C0(pItem, wszModDesc, ARRAY_SIZE(wszModDesc), 1, STATE_NONE, STATE_RUNEWORD, STATLIST_MAGIC, bAppendNewLine);
	if (!pDescString || !wszModDesc[0].ch)
	{
		Unicode::strcat(pBuffer, wszModDesc);
	}
	else
	{
		// The following code outputs the mod desc but inserting the desc before the last line.
		const int nStrLen = Unicode::strlen(wszModDesc);
		int nLineBeginIdx = UnicodeFindLastChar(wszModDesc, nStrLen - 2, L'\n');
		if (nLineBeginIdx > 0)
		{
			const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);
			const Unicode* wszComma = D2LANG_GetStringFromTblIndex(STR_IDX_3852_KeyComma);
			wszModDesc[nLineBeginIdx] = 0;
			Unicode::strcat(pBuffer, wszModDesc);
			Unicode::strcat(pBuffer, wszNewline);
			Unicode::strcat(pBuffer, pDescString);
			if (wszModDesc[nStrLen - 1].ch == L'\n')
				wszModDesc[nStrLen - 1] = L'\0';
			Unicode::strcat(pBuffer, &wszModDesc[nLineBeginIdx + 1]);
			Unicode::strcat(pBuffer, wszComma);
			Unicode::strcat(pBuffer, wszNewline);
		}
		else
		{
			Unicode::strcat(pBuffer, pDescString);
			Unicode::strcat(pBuffer, wszModDesc);
		}
	}
}


//1.10f: D2Client.0x6FB6FA38
D2C_States gaSetItemsStates[6] =
{
	STATE_ITEMSET1,
	STATE_ITEMSET2,
	STATE_ITEMSET3,
	STATE_ITEMSET4,
	STATE_ITEMSET5,
	STATE_ITEMSET6,
};
//1.10f: D2Client.0x6FB6FA50
int gaBitCount[64]
{
/*0b000000*/ 0,
/*0b000001*/ 1,
/*0b00001X*/ 1, 2,
/*0b0001XX*/ 1, 2, 2, 3,
/*0b001XXX*/ 1, 2, 2, 3, 2, 3, 3, 4,
/*0b01XXXX*/ 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
/*0b1XXXXX*/ 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
};

//1.10f: D2Client.0x6FAF32B0
//1.13c: D2Client.0x6FB05FB0
void __fastcall ITEMDESC_SingleSetBonus(D2UnitStrc* pUnit, D2UnitStrc* pItem, Unicode* pBuffer, int nBufferSize)
{
	if (pItem && pItem->dwUnitType == UNIT_ITEM && ITEMS_GetItemQuality(pItem) == ITEMQUAL_SET)
	{
		if (D2SetItemsTxt* pSetItemsTxt = ITEMS_GetSetItemsTxtRecordFromItem(pItem))
		{
			if (pSetItemsTxt->nAddFunc == 1) // Props are added depending on which set item is equipped	(See Civerb's Ward)
			{
				uint32_t nSetItemMask = ITEMS_GetSetItemsMask(pUnit, pItem, 0);
				int nIndexOfCurrentItemInSet = ITEMS_GetNoOfSetItemsFromItem(pItem);
				for (int nSetItemIdx = 0; nSetItemIdx < 6; ++nSetItemIdx)
				{
					if (nSetItemIdx != nIndexOfCurrentItemInSet)
					{
						int nSetItemStateIdx = nSetItemIdx;
						if (nSetItemIdx > nIndexOfCurrentItemInSet)
							nSetItemStateIdx = nSetItemIdx - 1; // Don't count the current item

						if (((1 << nSetItemIdx) & nSetItemMask) != 0)
						{
							ITEMDESC_Mods_6FAF19C0(pItem, pBuffer, nBufferSize,
								0, gaSetItemsStates[nSetItemStateIdx], 
								STATE_NONE, STATLIST_BASE, 1);
						}
					}
				}
			}
			else if (pSetItemsTxt->nAddFunc == 2) // Props are added depending on the number of set items equipped
			{
				uint32_t nSetItemsMask = ITEMS_GetSetItemsMask(pUnit, pItem, 1);
				int nItems = (nSetItemsMask < 64 ? gaBitCount[nSetItemsMask] : 0);
				int nItemsExcludingCurrentOne = nItems - 1;
				for (int nSetItemStateIdx = 0; nSetItemStateIdx < nItemsExcludingCurrentOne; ++nSetItemStateIdx)
				{
					ITEMDESC_Mods_6FAF19C0(pItem, pBuffer, nBufferSize,
						0, gaSetItemsStates[nSetItemStateIdx],
						STATE_NONE, STATLIST_BASE, 1);
				}
			}
		}
	}
}

//1.10f: D2Client.0x6FAF33C0
//1.13c: D2Client.0x6FB05F10
void __fastcall ITEMDESC_FullSetBonus(D2UnitStrc* pUnit, D2UnitStrc* pItem, Unicode* pBuffer, int nBufferSize)
{
	if (ITEMS_GetItemQuality(pItem) == ITEMQUAL_SET)
	{
		int nFileIdx = ITEMS_GetFileIndex(pItem);
		if (const D2SetItemsTxt* pSetItemsTxt = CLIENT_DATATBLS_GetSetItemsTxtRecord(nFileIdx))
		{
			for (int nSetItemIdx = 0; nSetItemIdx < 6; ++nSetItemIdx)
			{
				D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, gaSetItemsStates[nSetItemIdx]);
				if (pStatList && STATLIST_GetStatValue(pStatList, STAT_VALUE, 0) == pSetItemsTxt->nSetId)
				{
					ITEMDESC_Mods_6FAF19C0(pUnit, pBuffer, nBufferSize,
						0, gaSetItemsStates[nSetItemIdx],
						STATE_NONE, STATLIST_BASE, 1);
				}
			}
		}
	}
}

//1.10f: D2Client.0x6FAE5990
//1.13c: D2Client.0x6FB3D430
D2UnitStrc* __fastcall INV_FindSetItem(D2InventoryStrc* pInventory, int nSetItemId)
{
	for(D2UnitStrc* pCurrentInvItem = INVENTORY_GetFirstItem(pInventory); 
		pCurrentInvItem != nullptr; 
		pCurrentInvItem = INVENTORY_GetNextItem(pCurrentInvItem))
	{
		if (INVENTORY_UnitIsItem(pCurrentInvItem) && ITEMS_GetItemQuality(pCurrentInvItem) == ITEMQUAL_SET)
		{
			D2C_ItemInvPage nInvPage = (D2C_ItemInvPage)ITEMS_GetPage(pCurrentInvItem);
			if (nInvPage == INVPAGE_INVENTORY || nInvPage == INVPAGE_CUBE || nInvPage == INVPAGE_STASH || nInvPage == INVPAGE_NULL)
			{
				int nNodePage = INVENTORY_GetItemNodePage(pCurrentInvItem);
				if (nNodePage == 1 || nNodePage == 3 || nNodePage == 4)
				{
					int nFileIdx = ITEMS_GetFileIndex(pCurrentInvItem);
					if (const D2SetItemsTxt* pSetItemsTxt = CLIENT_DATATBLS_GetSetItemsTxtRecord(nFileIdx))
					{
						if (pSetItemsTxt->wSetItemId == nSetItemId)
						{
							return pCurrentInvItem;
						}
					}
				}
			}
		}
	}
	return nullptr;
}

//1.10f: Inline
//1.13c: D2Client.0x6FB3C720
const Unicode* ITEMDESC_StripBracketPrefix(const Unicode* wszString)
{
	int nStrLen = Unicode::strlen(wszString);
	int nClosingBracketIdx = UnicodeFindLastChar(wszString, nStrLen, L']');
	if (nClosingBracketIdx >= 3 && wszString[nClosingBracketIdx - 3] == L'[')
		return &wszString[nClosingBracketIdx + 1];
	else
		return wszString;
}

//1.10f: Inline
//1.13c: D2Client.0x6FB3E080
BOOL ITEMDESC_GetPersonalizedName(D2UnitStrc* pItem, Unicode* pOutBuffer)
{
	Unicode wszName[512];
	Unicode wszBuffer[512];

	if (!ITEMS_CHECK_FLAG(pItem, IFLAG_PERSONALIZED)) // line 1770
		return FALSE;
	int ItemQuality = ITEMS_GetItemQuality(pItem);
	if (ItemQuality >= ITEMQUAL_SET && ItemQuality <= ITEMQUAL_TEMPERED)
		return FALSE;
	
	Unicode v12[32];
	D2_MAYBE_UNUSED(v12);

	Unicode::win2Unicode(wszName, ITEMS_GetEarName(pItem), 32);
	Unicode::Personalize(wszBuffer, wszName, pOutBuffer, 512, STRTABLE_GetLanguage());
	Unicode::strcpy(pOutBuffer, wszBuffer);
	return TRUE;
}

//1.10f: Inline
//1.13c: D2Client.0x6FB3DFB0
BOOL ITEMDESC_GetPersonalizedName2(D2UnitStrc* pItem, Unicode* pOutBuffer, const Unicode* a3)
{
	Unicode wszName[512];
	Unicode v14[512];
	D2_MAYBE_UNUSED(v14);

	if (!ITEMS_CHECK_FLAG(pItem, IFLAG_PERSONALIZED))// line 1803
		return FALSE;
	
	Unicode v12[32];
	D2_MAYBE_UNUSED(v12);

	Unicode::win2Unicode(wszName, ITEMS_GetEarName(pItem), 32);
	Unicode::Personalize(pOutBuffer, wszName, a3, 512, STRTABLE_GetLanguage());
	return TRUE;
}

const Unicode* GetPlayerClassName(int nClass)
{
	D2_ASSERT(nClass >= PCLASS_AMAZON && nClass < NUMBER_OF_PLAYERCLASSES);
	switch (nClass)
	{
	case PCLASS_AMAZON:
		return D2LANG_GetStringFromTblIndex(STR_IDX_4011_Amazon);
	case PCLASS_SORCERESS:
		return D2LANG_GetStringFromTblIndex(STR_IDX_4010_Sorceress);
	case PCLASS_NECROMANCER:
		return D2LANG_GetStringFromTblIndex(STR_IDX_4009_Necromancer);
	case PCLASS_PALADIN:
		return D2LANG_GetStringFromTblIndex(STR_IDX_4008_Paladin);
	case PCLASS_BARBARIAN:
		return D2LANG_GetStringFromTblIndex(STR_IDX_4007_Barbarian);
	case PCLASS_DRUID:
		return D2LANG_GetStringFromTblIndex(STR_IDX_10097_Druid);
	case PCLASS_ASSASSIN:
		return D2LANG_GetStringFromTblIndex(STR_IDX_10098_Assassin);
	default:
		return nullptr;
	}
}

//1.10f: D2Client.0x6FADD360
//1.13c: D2Client.0x6FB414F0
void __fastcall INV_GetItemName(D2UnitStrc* pItem, Unicode* pBuffer, int nBufferSize)
{
	Unicode wszDest[512];
	if (!pItem)
		return;
	Unicode wszSuffix[512];
	Unicode wszPrefixBuffer[512];
	int nNameLength = 0;
	const Unicode* wszSpace = D2LANG_GetStringFromTblIndex(STR_IDX_3995_space);
	const Unicode* wszNewline = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);
	D2ItemsTxt* pItemStats = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
	D2_ASSERT(pItemStats);
	int32_t nItemTypeFromItem = ITEMS_GetItemType(pItem);
	if (ITEMS_CHECK_FLAG(pItem, IFLAG_RUNEWORD))
	{
		Unicode wszBuffer[512];
		Unicode::strcpy(wszBuffer, D2LANG_GetStringFromTblIndex((D2C_StringIndices)pItemStats->wNameStr));
		Unicode::strcpy(wszDest, ITEMDESC_StripBracketPrefix(wszBuffer));
		Unicode::strcat(wszDest, wszNewline);
		const Unicode* wszPrefix = D2LANG_GetStringFromTblIndex(ITEMS_GetPrefixId(pItem, 0));
		Unicode::strcpy(wszSuffix, wszPrefix ? wszPrefix : wszSpace);
		Unicode::strcat(wszPrefixBuffer, wszSuffix);
		TEXT_ApplyColorCode(wszPrefixBuffer, 4);
		Unicode::strcat(pBuffer, wszDest);
		Unicode::strcat(pBuffer, wszPrefixBuffer);
	}
	else if (!ITEMS_CHECK_FLAG(pItem, IFLAG_IDENTIFIED))
	{
		Unicode wszBuffer[512];
		Unicode::strcpy(wszBuffer, D2LANG_GetStringFromTblIndex(pItemStats->wNameStr));
		Unicode::strcpy(pBuffer, ITEMDESC_StripBracketPrefix(wszBuffer));
	}
	else 
	{
		switch (ITEMS_GetItemQuality(pItem))
		{
		case ITEMQUAL_INFERIOR:
			if (D2LowQualityItemsTxt* pLowQualityItemTxt = DATATBLS_GetLowQualityItemsTxtRecord(ITEMS_GetFileIndex(pItem)))
			{
				D2CLIENT_FormatStats_6FADCFE0(pBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_1712_LowqualityFormat), D2LANG_GetStringFromTblIndex(pLowQualityItemTxt->wTblId), D2LANG_GetStringFromTblIndex(pItemStats->wNameStr), 0);
			}
			break;
		case ITEMQUAL_NORMAL:
			if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
			{
				int nSuffixId = ITEMS_GetSuffixId(pItem, 0);
				if (nSuffixId == 0 || nSuffixId == 1)
				{
					int nStrId;
					if (nSuffixId == 0)
					{
						nStrId = nItemTypeFromItem != ITEMTYPE_BOOK ? STR_IDX_2200_tsc : STR_IDX_2199_tbk;
					}
					else
					{
						nStrId = nItemTypeFromItem != ITEMTYPE_BOOK ? STR_IDX_2202_isc : STR_IDX_2201_ibk;
					}
					Unicode::strcpy(pBuffer, D2LANG_GetStringFromTblIndex(nStrId));
				}
			}
			else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_PLAYER_BODY_PART))
			{
				Unicode wszEarName[256];
				Unicode wszEarLevel[3];
				char szEarLevel[3] = { 0 };
				SStrPrintf(szEarLevel, ARRAY_SIZE(szEarLevel), "%d", ITEMS_GetEarLevel(pItem));
				Unicode::win2Unicode(wszEarLevel, szEarLevel, ARRAY_SIZE(wszEarLevel));
				Unicode wszBuffer[256];
				char szEarName[256];
				strcpy(szEarName, ITEMS_GetEarName(pItem));
				Unicode::win2Unicode(wszEarName, szEarName, ARRAY_SIZE(wszEarName));
				if (ITEMS_CHECK_FLAG(pItem, IFLAG_NAMED))
				{
					Unicode::strcpy(pBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_5126_Hardcore));
					Unicode::strcat(pBuffer, wszNewline);
					Unicode::strcat(pBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_4141_level));
				}
				else
				{
					Unicode::strcpy(pBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_4141_level));
				}
				Unicode::strcat(pBuffer, wszSpace);
				Unicode::strcat(pBuffer, wszEarLevel);
				Unicode::strcat(pBuffer, wszNewline);
				Unicode::strcat(pBuffer, GetPlayerClassName(ITEMS_GetFileIndex(pItem)));
				Unicode::strcat(pBuffer, wszNewline);
				Unicode v200[100];
				Unicode v202[512];
				Unicode::strcpy(v202, D2LANG_GetStringFromTblIndex(pItemStats->wNameStr));
				Unicode::Personalize(v200, wszEarName, v202, ARRAY_SIZE(v200), STRTABLE_GetLanguage());
				Unicode::strcat(pBuffer, v200);
			}
			else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BODY_PART))
			{
				D2MonStatsTxt* pMonStatsTxt = DATATBLS_GetMonStatsTxtRecord(ITEMS_GetFileIndex(pItem));
				D2CLIENT_FormatStats_6FADCFE0(pBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_1716_BodyPartsFormat), D2LANG_GetStringFromTblIndex(pMonStatsTxt->wNameStr), D2LANG_GetStringFromTblIndex(pItemStats->wNameStr), 0);
			}
			else if (ITEMS_CHECK_FLAG(pItem, IFLAG_SOCKETED)
				&& pItem->pInventory
				&& INVENTORY_GetItemCount(pItem->pInventory))
			{
				D2CLIENT_FormatStats_6FADCFE0(pBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_1715_GemmedNormalName), D2LANG_GetStringFromTblIndex(STR_IDX_1728_Gemmed), D2LANG_GetStringFromTblIndex(pItemStats->wNameStr), 0);
			}
			else
			{
				Unicode wszBuffer[512];
				Unicode::strcpy(wszBuffer, D2LANG_GetStringFromTblIndex(pItemStats->wNameStr));
				Unicode::strcpy(pBuffer, ITEMDESC_StripBracketPrefix(wszBuffer));
			}
			break;
		case ITEMQUAL_SUPERIOR:
			D2CLIENT_FormatStats_6FADCFE0(pBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_1711_HiqualityFormat), D2LANG_GetStringFromTblIndex(STR_IDX_1727_Hiquality), D2LANG_GetStringFromTblIndex(pItemStats->wNameStr), 0);
			break;
		case ITEMQUAL_MAGIC:
		{
			if (D2MagicAffixTxt* pPrefixTxt = DATATBLS_GetMagicAffixTxtRecord(ITEMS_GetPrefixId(pItem, 0)))
			{
				if (pPrefixTxt->wTblIndex)
				{
					nNameLength = Unicode::strlen(D2LANG_GetStringFromTblIndex(pPrefixTxt->wTblIndex));
					Unicode::strcpy(wszDest, D2LANG_GetStringFromTblIndex(pPrefixTxt->wTblIndex));
				}
				else
				{
					char szSource[6] = { 0 };
					strcpy(szSource, "(");
					strcat(szSource, pPrefixTxt->szName);
					strcat(szSource, ")");
					nNameLength = strlen(szSource);
					Unicode::toUnicode(wszDest, szSource, ARRAY_SIZE(wszDest));
				}
			}
			else
			{
				wszDest[0] = 0;
			}
			if (D2MagicAffixTxt* pSuffixTxt = DATATBLS_GetMagicAffixTxtRecord(ITEMS_GetSuffixId(pItem, 0)))
			{
				if (pSuffixTxt->wTblIndex)
				{
					nNameLength += Unicode::strlen(D2LANG_GetStringFromTblIndex(pSuffixTxt->wTblIndex));
					Unicode::strcat(wszSuffix, D2LANG_GetStringFromTblIndex(pSuffixTxt->wTblIndex));
				}
				else
				{
					char szSource[6] = { 0 };
					strcpy(szSource, "(");
					strcat(szSource, pSuffixTxt->szName);
					strcat(szSource, ")");
					nNameLength += strlen(szSource);
					Unicode wszSuffixBuffer[512];
					Unicode::toUnicode(wszSuffixBuffer, szSource, ARRAY_SIZE(wszSuffixBuffer));
					Unicode::strcat(wszSuffix, wszSuffixBuffer);
				}
			}
			else
			{
				wszSuffix[0] = 0;
			}
			int32_t Len = nNameLength + Unicode::strlen(D2LANG_GetStringFromTblIndex(pItemStats->wNameStr)) + 3;
			D2_ASSERT(Len < nBufferSize);
			D2CLIENT_FormatStats_6FADCFE0(pBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_1714_MagicFormat), wszDest, D2LANG_GetStringFromTblIndex(pItemStats->wNameStr), wszSuffix, 0);
			break;
		}
		case ITEMQUAL_SET:
			if (D2SetItemsTxt* pSetItemRecord = CLIENT_DATATBLS_GetSetItemsTxtRecord(ITEMS_GetFileIndex(pItem)))
			{
				Unicode wszBuffer[512];
				Unicode::strcpy(wszBuffer, D2LANG_GetStringFromTblIndex(pItemStats->wNameStr));
				Unicode::strcpy(pBuffer, ITEMDESC_StripBracketPrefix(wszBuffer));
				Unicode::strcat(pBuffer, wszNewline);
				if (pSetItemRecord->wStringId)
				{
					Unicode szSource[512];
					Unicode v202[512];
					Unicode::strcpy(v202, D2LANG_GetStringFromTblIndex(pSetItemRecord->wStringId));
					if (ITEMDESC_GetPersonalizedName2(pItem, szSource, v202))
					{
						Unicode::strcat(pBuffer, szSource);
					}
					else
					{
						Unicode v200[100] = {};
						D2CLIENT_FormatStats_6FADCFE0(v200, D2LANG_GetStringFromTblIndex(STR_IDX_10089_SetItemFormatX), v202, 0);
						Unicode::strcat(pBuffer, v200);
					}
				}
			}
			break;
		case ITEMQUAL_RARE:
		case ITEMQUAL_CRAFT:
		case ITEMQUAL_TEMPERED:
		{
			D2RareAffixTxt* pRarePrefix = DATATBLS_GetRareAffixTxtRecord(ITEMS_GetRarePrefixId(pItem));
			if (pRarePrefix)
			{
				if (pRarePrefix->wStringId)
				{
					nNameLength = Unicode::strlen(D2LANG_GetStringFromTblIndex(pRarePrefix->wStringId));
					Unicode::strcpy(wszDest, D2LANG_GetStringFromTblIndex(pRarePrefix->wStringId));
				}
				else
				{
					char szSource[6] = { 0 };
					strcpy(szSource, "(");
					strcat(szSource, pRarePrefix->szName);
					strcat(szSource, ")");
					nNameLength = strlen(szSource);
					Unicode::toUnicode(wszDest, szSource, ARRAY_SIZE(wszDest));
				}
			}
			else
			{
				wszDest[0] = 0;
			}
			if (D2RareAffixTxt* pRareSuffix = DATATBLS_GetRareAffixTxtRecord(ITEMS_GetRareSuffixId(pItem)))
			{
				const Unicode* wszRareSuffix;
				if (pRareSuffix->wStringId)
				{
					nNameLength += Unicode::strlen(D2LANG_GetStringFromTblIndex(pRareSuffix->wStringId));
					wszRareSuffix = D2LANG_GetStringFromTblIndex(pRareSuffix->wStringId);
				}
				else
				{
					Unicode wszRareSuffixBuffer[512];
					char szRareSuffixBuffer[6] = { 0 };
					strcpy(szRareSuffixBuffer, "(");
					strcat(szRareSuffixBuffer, pRareSuffix->szName);
					strcat(szRareSuffixBuffer, ")");
					Unicode::toUnicode(wszRareSuffixBuffer, szRareSuffixBuffer, ARRAY_SIZE(wszRareSuffixBuffer));
					nNameLength += Unicode::strlen(wszRareSuffixBuffer);
					wszRareSuffix = wszRareSuffixBuffer;
				}
				if (pRarePrefix)
					Unicode::strcat(wszSuffix, wszRareSuffix);
				else
					Unicode::strcpy(wszSuffix, wszRareSuffix);
			}
			else
			{
				wszSuffix[0] = 0;
			}
			int Len = nNameLength + Unicode::strlen(D2LANG_GetStringFromTblIndex(pItemStats->wNameStr)) + 3;
			D2_ASSERT(Len < nBufferSize);
			Unicode wszBuffer[512];
			Unicode::strcpy(wszBuffer, D2LANG_GetStringFromTblIndex(pItemStats->wNameStr));
			Unicode::strcpy(wszPrefixBuffer, ITEMDESC_StripBracketPrefix(wszBuffer));
			Unicode::strcat(wszPrefixBuffer, wszNewline);
			Unicode v200[100];
			D2CLIENT_FormatStats_6FADCFE0(v200, D2LANG_GetStringFromTblIndex(STR_IDX_1718_RareFormat), wszDest, wszSuffix, 0);
			Unicode v203[512];
			if (ITEMDESC_GetPersonalizedName2(pItem, v203, v200))
			{
				Unicode::strcat(wszPrefixBuffer, v203);
			}
			else
			{
				Unicode::strcat(wszPrefixBuffer, v200);
			}
			Unicode::strcat(pBuffer, wszPrefixBuffer);
			break;
		}
		case ITEMQUAL_UNIQUE:
		{
			Unicode wszBuffer[512];
			Unicode::strcpy(wszBuffer, D2LANG_GetStringFromTblIndex(pItemStats->wNameStr));
			const Unicode* wszUniqueName = wszBuffer;
			if (ITEMS_GetFileIndex(pItem) >= 0)
			{
				if (D2UniqueItemsTxt* pUniqueItemRecord = CLIENT_DATATBLS_GetUniqueItemsTxtRecord(ITEMS_GetFileIndex(pItem)))
				{
					if (!pItemStats->nSkipName)
					{
						Unicode::strcpy(pBuffer, ITEMDESC_StripBracketPrefix(wszBuffer));
						Unicode::strcat(pBuffer, wszNewline);
					}
					Unicode v202[512];
					if (ITEMDESC_GetPersonalizedName2(pItem, v202, ITEMDESC_StripBracketPrefix(D2LANG_GetStringFromTblIndex(pUniqueItemRecord->wTblIndex))))
					{
						Unicode::strcat(pBuffer, v202);
						break;
					}
					else
					{
						wszUniqueName = D2LANG_GetStringFromTblIndex(pUniqueItemRecord->wTblIndex);
					}
				}
			}
			Unicode::strcpy(pBuffer, ITEMDESC_StripBracketPrefix(wszUniqueName));
			break;
		}
		default:
			D2_UNREACHABLE;
			return;
		}
	}
	ITEMDESC_GetPersonalizedName(pItem, pBuffer);
	if (pItemStats->nQuest)
	{
		if (pItemStats->nQuestDiffCheck
			&& (STATLIST_UnitGetStatValue(pItem, STAT_QUESTITEMDIFFICULTY, 0) < D2CLIENT_GetDifficulty_6FAAC090()))
		{
			TEXT_ApplyColorCode(pBuffer, 1);
		}
		else if (pItemStats->dwCode != ' gel')
		{
			TEXT_ApplyColorCode(pBuffer, 4);
		}
	}
}

//1.10f: D2Client.0x6FAE20C8(Inlined)
//1.13c: D2Client.0x6FB428C0
void __fastcall UI_INV_DrawSetItemDesc(D2UnitStrc* pOwner)
{
	const Unicode* wszNewLine = D2LANG_GetStringFromTblIndex(STR_IDX_3998_newline);
	D2UnitStrc* pUnitForStatsComputation = pOwner;
	if (pOwner->dwUnitType == UNIT_MONSTER && !MONSTERS_GetHirelingTypeId(pOwner))
		pUnitForStatsComputation = D2CLIENT_GetControlUnit();

	if ((*D2CLIENT_pgbInvMouseOverEquippedItem || *D2CLIENT_pdword_6FBB58E0)
		&& *D2CLIENT_pgpSelectedItem
		&& !INVENTORY_GetCursorItem(pOwner->pInventory))
	{
		int32_t nClassId = -1;
		if (*D2CLIENT_pgpSelectedItem)
			nClassId = (*D2CLIENT_pgpSelectedItem)->dwClassId;

		const D2ItemsTxt* ptItemStats = DATATBLS_GetItemsTxtRecord(nClassId);
		D2_ASSERT(ptItemStats);
		const int32_t nFileIdx = ITEMS_GetFileIndex(*D2CLIENT_pgpSelectedItem);
		if (const D2SetsTxt* pSetsRecord = CLIENT_DATATBLS_GetSetsTxtRecordFromSetItemId(nFileIdx))
		{
			Unicode wszFullText[2048];
			Unicode wszSetName[256];
			Unicode wszSetBonusesDesc[1024];
			Unicode wszStatsDesc[1024];
			Unicode wszDescription[1024];

			Unicode wszDurability[256];

			Unicode wszLevelRequirement[256];
			uint8_t nLevelRequirementColorIndex = 0;
			Unicode wszStrRequirement[256];
			uint8_t nStrRequirementColorIndex = 0;
			Unicode wszDexRequirement[256];
			uint8_t nDexRequirementColorIndex = 0;

			Unicode::strcpy(wszSetName, D2LANG_GetStringFromTblIndex(pSetsRecord->wStringId));
			Unicode::strcat(wszSetName, wszNewLine);
			BOOL bStrength;
			BOOL bDexterity;
			BOOL bTrade;
			ITEMS_CheckRequirements(*D2CLIENT_pgpSelectedItem, pUnitForStatsComputation, FALSE, &bStrength, &bDexterity, &bTrade);
			ITEMDESC_Durability(*D2CLIENT_pgpSelectedItem, wszDurability, ptItemStats);
			int nRequiredLevel = ITEMS_GetLevelRequirement(*D2CLIENT_pgpSelectedItem, pUnitForStatsComputation);
			if (nRequiredLevel > 1)
			{
				if (!bTrade)
					nLevelRequirementColorIndex = 1;
				ITEMDESC_RequiredLevel(wszLevelRequirement, nRequiredLevel);
			}
			if (ITEMS_CheckItemTypeId(*D2CLIENT_pgpSelectedItem, ITEMTYPE_WEAPON)
				|| ITEMS_CheckItemTypeId(*D2CLIENT_pgpSelectedItem, ITEMTYPE_ANY_ARMOR))
			{
				int nRequiredStr = 0;
				int nRequiredDex = 0;
				if (const int32_t nReqPct = STATLIST_UnitGetItemStatOrSkillStatValue(*D2CLIENT_pgpSelectedItem, STAT_ITEM_REQ_PERCENT, 0))
				{
					nRequiredStr = D2_ComputePercentage(ptItemStats->wReqStr, nReqPct);
					nRequiredDex = D2_ComputePercentage(ptItemStats->wReqDex, nReqPct);
				}
				if (ITEMS_CHECK_FLAG(*D2CLIENT_pgpSelectedItem, IFLAG_ETHEREAL))
				{
					nRequiredStr -= 10;
					nRequiredDex -= 10;
				}
				if (ptItemStats->wReqStr)
				{
					if (!bStrength)
						nStrRequirementColorIndex = 1;
					ITEMDESC_RequiredStrength(wszStrRequirement, ptItemStats, nRequiredStr);
				}
				if (ptItemStats->wReqDex)
				{
					if (!bDexterity)
						nDexRequirementColorIndex = 1;
					ITEMDESC_RequiredDexterity(wszDexRequirement, ptItemStats, nRequiredDex);
				}
			}
			Unicode::strcpy(wszDescription, TEXT_ApplyColorCode(wszLevelRequirement, nLevelRequirementColorIndex));
			Unicode::strcat(wszDescription, TEXT_ApplyColorCode(wszStrRequirement, nStrRequirementColorIndex));
			Unicode::strcat(wszDescription, TEXT_ApplyColorCode(wszDexRequirement, nDexRequirementColorIndex));

			const D2C_PlayerClasses nClassSpecific = ITEMS_GetClassOfClassSpecificItem(*D2CLIENT_pgpSelectedItem);
			if (nClassSpecific != PCLASS_INVALID)
			{
				Unicode wszClassRequirement[256];
				Unicode::strcat(wszClassRequirement, D2LANG_GetStringFromTblIndex(nClassSpecific + STR_IDX_10917_AmaOnly));
				Unicode::strcat(wszClassRequirement, wszNewLine);
				uint8_t nClassRequirementColorIndex = 0;
				if (pUnitForStatsComputation
					&& pUnitForStatsComputation->dwUnitType == UNIT_PLAYER
					&& pUnitForStatsComputation->dwClassId != nClassSpecific)
				{
					nClassRequirementColorIndex = 1;
				}
				Unicode::strcat(wszDescription, TEXT_ApplyColorCode(wszClassRequirement, nClassRequirementColorIndex));
			}

			Unicode::strcat(wszDescription, TEXT_ApplyColorCode(wszDurability, 0));

			if (ITEMS_CheckItemTypeId(*D2CLIENT_pgpSelectedItem, ITEMTYPE_WEAPON))
			{
				Unicode wszAttackSpeed[256];
				ITEMDESC_AttackSpeed_WeaponClass(*D2CLIENT_pgpSelectedItem, wszAttackSpeed, ptItemStats);
				TEXT_ApplyColorCode(wszAttackSpeed, 0);
				Unicode::strcat(wszDescription, wszAttackSpeed);

				if (STATLIST_GetMinDamageFromUnit(*D2CLIENT_pgpSelectedItem, 0) >= 0
					&& STATLIST_GetMaxDamageFromUnit(*D2CLIENT_pgpSelectedItem, 0) >= 0)
				{
					Unicode wszMinMaxDamage[256];
					ITEMDESC_Damage(*D2CLIENT_pgpSelectedItem, wszMinMaxDamage, ptItemStats);
					TEXT_ApplyColorCode(wszMinMaxDamage, 0);
					Unicode::strcat(wszDescription, wszMinMaxDamage);
				}
			}
			if (ITEMS_CheckItemTypeId(*D2CLIENT_pgpSelectedItem, ITEMTYPE_ANY_SHIELD))
			{
				if (pUnitForStatsComputation
					&& pUnitForStatsComputation->dwUnitType == UNIT_PLAYER
					&& pUnitForStatsComputation->dwClassId == PCLASS_PALADIN
					&& (!ITEMS_IsClassValid(*D2CLIENT_pgpSelectedItem)
						|| ITEMS_GetClassOfClassSpecificItem(*D2CLIENT_pgpSelectedItem) == PCLASS_PALADIN))
				{
					Unicode wszSmiteDamage[256];
					ITEMDESC_SmiteAndKickDamage(*D2CLIENT_pgpSelectedItem, wszSmiteDamage, ptItemStats);
					TEXT_ApplyColorCode(wszSmiteDamage, 0);
					Unicode::strcat(wszDescription, wszSmiteDamage);
				}
				Unicode wszBlockChance[256];
				ITEMDESC_Blockchance(*D2CLIENT_pgpSelectedItem, wszBlockChance, ptItemStats);
				TEXT_ApplyColorCode(wszBlockChance, 0);
				Unicode::strcat(wszDescription, wszBlockChance);
			}
			if (ITEMS_CheckItemTypeId(*D2CLIENT_pgpSelectedItem, ITEMTYPE_ANY_ARMOR)
				&& STATLIST_GetDefenseFromUnit(*D2CLIENT_pgpSelectedItem) > 0)
			{
				Unicode wszDefense[256];
				ITEMDESC_Defense(pUnitForStatsComputation, *D2CLIENT_pgpSelectedItem, wszDefense, ptItemStats);
				TEXT_ApplyColorCode(wszDefense, 0);
				Unicode::strcat(wszDescription, wszDefense);
			}

			Unicode wszItemName[256];
			INV_GetItemName(*D2CLIENT_pgpSelectedItem, wszItemName, ARRAY_SIZE(wszItemName));
			uint8_t nItemNameColor = 1;
			if (!ITEMS_CHECK_FLAG(*D2CLIENT_pgpSelectedItem, IFLAG_BROKEN))
				nItemNameColor = 2;
			TEXT_ApplyColorCode(wszItemName, nItemNameColor);
			Unicode::strcat(wszDescription, wszItemName);
			wszStatsDesc[0] = 0;
			if (ITEMS_CHECK_FLAG(*D2CLIENT_pgpSelectedItem, IFLAG_SOCKETED))
			{
				Unicode wszSocketDesc[300];
				ITEMDESC_SocketsAndEthereality(*D2CLIENT_pgpSelectedItem, wszSocketDesc);
				Unicode::strcpy(wszStatsDesc, wszSocketDesc);
			}
			ITEMDESC_RunewordMods(*D2CLIENT_pgpSelectedItem, wszStatsDesc, ARRAY_SIZE(wszStatsDesc), 1, 0);
			Unicode wszSingleSetBonus[512];
			wszSingleSetBonus[0] = 0;
			ITEMDESC_SingleSetBonus(pUnitForStatsComputation, *D2CLIENT_pgpSelectedItem, wszSingleSetBonus, 512);
			Unicode wszFullSetBonus[512];
			wszFullSetBonus[0] = 0;
			if (*D2CLIENT_pgpSelectedItem && (*D2CLIENT_pgpSelectedItem)->dwItemMode == IMODE_EQUIP)
				ITEMDESC_FullSetBonus(pUnitForStatsComputation, *D2CLIENT_pgpSelectedItem, wszFullSetBonus, 512);

			Unicode wszSetItemsBonusesDesc[512];
			for (int nSetItemIdx = 0; nSetItemIdx < pSetsRecord->nSetItems; nSetItemIdx++)
			{
				D2SetItemsTxt* pCurrentSetItem = pSetsRecord->pSetItem[nSetItemIdx];
				if (!pCurrentSetItem)
					break;

				if (pCurrentSetItem->wStringId)
				{
					D2CLIENT_FormatStats_6FADCFE0(wszSetBonusesDesc, D2LANG_GetStringFromTblIndex(STR_IDX_10089_SetItemFormatX), D2LANG_GetStringFromTblIndex(pCurrentSetItem->wStringId), 0);
					Unicode::strcat(wszSetBonusesDesc, wszNewLine);
				}
				uint8_t nSetItemBonusColorIndex = 2;
				if (!INV_FindSetItem(pOwner->pInventory, pCurrentSetItem->wSetItemId))
					nSetItemBonusColorIndex = 1;
				TEXT_ApplyColorCode(wszSetBonusesDesc, nSetItemBonusColorIndex);
				Unicode::strcat(wszSetItemsBonusesDesc, wszSetBonusesDesc);
			}
			Unicode::strcpy(wszSetBonusesDesc, TEXT_ApplyColorCode(wszSetItemsBonusesDesc, 2));
			Unicode::strcat(wszSetBonusesDesc, TEXT_ApplyColorCode(wszSetName, 4));
			if (wszFullSetBonus[0])
			{
				Unicode::strcat(wszSetBonusesDesc, wszNewLine);
				Unicode::strcat(wszSetBonusesDesc, TEXT_ApplyColorCode(wszFullSetBonus, 4));
			}
			Unicode::strcat(wszSetBonusesDesc, wszNewLine);
			if (wszSingleSetBonus[0])
			{
				Unicode::strcat(wszSetBonusesDesc, TEXT_ApplyColorCode(wszSingleSetBonus, 2));
			}
			Unicode::strcat(wszSetBonusesDesc, TEXT_ApplyColorCode(wszStatsDesc, 3));
			Unicode::strcat(wszSetBonusesDesc, wszDescription);

			if (*D2CLIENT_pgnInventoryMode <= VENDORMODE_NONE || *D2CLIENT_pgnInventoryMode > VENDORMODE_CLOSE)
			{
				Unicode::strcpy(wszFullText, wszSetBonusesDesc);
			}
			else
			{
				Unicode wszTransactionCost[64];
				int nTransactionCost;
				if (NPCMENU_TransactionCost(*D2CLIENT_pgpSelectedItem, *D2CLIENT_pgbSelectedItemIsOwned, &nTransactionCost, wszTransactionCost, ARRAY_SIZE(wszTransactionCost)))
				{
					Unicode::strcpy(wszFullText, wszSetBonusesDesc);
					if (wszTransactionCost[0])
						Unicode::strcat(wszFullText, wszNewLine);
					Unicode::strcat(wszFullText, wszTransactionCost);
				}
				else
				{
					Unicode::strcpy(wszFullText, wszSetBonusesDesc);
					if (*D2CLIENT_pgnInventoryMode != VENDORMODE_REPAIR)
					{
						Unicode wszPriceless[64];
						Unicode::strcpy(wszPriceless, D2LANG_GetStringFromTblIndex(STR_IDX_3333_priceless));
						Unicode::strcat(wszFullText, wszNewLine);
						Unicode::strcat(wszFullText, TEXT_ApplyColorCode(wszPriceless, 1));
					}
				}
			}
			int nTextWidth, nTextHeight;
			D2Win_10131_GetTextDimensions(wszFullText, &nTextWidth, &nTextHeight);
			if ((*D2CLIENT_pgnSelectedItemDescTopPosY - nTextHeight) > 0)
			{
				D2Win_10129_DrawFramedText(wszFullText, *D2CLIENT_pgnSelectedItemDescTopPosX, *D2CLIENT_pgnSelectedItemDescTopPosY, 0, 1);
			}
			else
			{
				D2Win_10129_DrawFramedText(wszFullText, *D2CLIENT_pgnSelectedItemDescBottomPosX, *D2CLIENT_pgnSelectedItemDescBottomPosY + nTextHeight, 0, 1);
			}
		}
	}
}


#endif // D2_VERSION_110F
