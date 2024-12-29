#include <CGAME/Game.h>
#include <D2Client.h>
#include <D2Gfx.h>
#include <D2Items.h>
#include <D2StatList.h>
#include <D2WinFont.h>
#include <UI/inv.h>
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

static const int kBuySellButtonSize = 32;
static const int kBuySellButtonSizeForTrade = 45; // Note this is incorrect... real size is 32
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

D2VAR(D2CLIENT, pgpSelectedItem_6FBB58F0, D2UnitStrc*, 0x6FBB58F0 - D2ClientImageBase); // aka sghSelItem
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


#endif // D2_VERSION_110F
