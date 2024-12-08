#include <Engine/Gfx.h> 
#include <D2Client.h>
#include <D2CMP.h>
#include <D2Gfx.h>
#include <Texture.h>
#include <D2Items.h>
#include <D2StatList.h>

#ifdef D2_VERSION_110F
D2FUNC(D2CLIENT, GetItemGfxData_6FB5EC80, int, __fastcall, (D2GfxDataStrc* pData, D2UnitStrc* pItem, D2CellFileStrc* pCellFile, int nDirection, int nFrame, DWORD* a6, int nComponent, int a8), 0x6FB5EC80 - D2ClientImageBase);
D2FUNC(D2CLIENT, UNITDRAW_DrawOverlays, void, __fastcall, (D2UnitStrc* pUnit, DWORD dwColorTint, BOOL bBack, int nXpos, int nYpos, BOOL bPerspectiveDisabled), 0x6FB5A490 - D2ClientImageBase);

D2VAR(D2CLIENT, pgnScreenWidth, uint32_t, 0x6FB740EC - D2ClientImageBase);
D2VAR(D2CLIENT, pgnScreenHeight, uint32_t, 0x6FB740F0 - D2ClientImageBase);

//1.10f:D2Client.0x6FB5EC80
BOOL __fastcall CLIENT_GetItemGfxData(D2GfxDataStrc* pData, D2UnitStrc* pItem, D2CellFileStrc* pCellFile, int nDirection, int nFrame, DWORD* a6, int nComponent, int a8)
{
	return D2CLIENT_GetItemGfxData_6FB5EC80(pData, pItem, pCellFile, nDirection, nFrame, a6, nComponent, a8);
}

//1.10f: D2Client.0x6FB5E170
BOOL __fastcall CelCullTestInScreen(D2GfxCellStrc* pCel, int nLeftX, int nTopY, int bMaybeRotating)
{
	const int nTestPosY = nTopY + D2CMP_CelGetOffsetY(pCel);
	const int nHeight = D2CMP_CelGetHeight(pCel);
	
	int nTestPosX = nLeftX + D2CMP_CelGetOffsetX(pCel);
	int nWidth = D2CMP_CelGetWidth(pCel);
	if (bMaybeRotating) // Assumes height > width ?
	{
		nTestPosX -= nHeight;
		nWidth += nHeight;
	}
	if (nTestPosX > *D2CLIENT_pgnScreenWidth)
		return FALSE;
	if (nTestPosX + nWidth < 0)
		return FALSE;
	if (nTestPosY - nHeight <= *D2CLIENT_pgnScreenHeight)
		return nHeight + nTestPosY >= 0;
	return FALSE;
}

//1.10f:D2Client.0x6FB5A320
BOOL __fastcall CLIENT_DrawItem(D2UnitStrc* pItem, int nXpos, int nYpos)
{
	if (!pItem || pItem->dwUnitType != UNIT_ITEM)
		return FALSE;
	
	int nDirection = 0;
	if (ITEMS_GetItemType(pItem) == ITEMTYPE_GOLD)
	{
		const int32_t nGoldAmount = STATLIST_UnitGetStatValue(pItem, STAT_GOLD, 0);
		if (nGoldAmount >= 5000)
			nDirection = 3;
		else if (nGoldAmount >= 500)
			nDirection = 2;
		else if (nGoldAmount >= 100)
			nDirection = 1;
	}

	D2GfxDataStrc tCelContext;
	memset(&tCelContext, 0, sizeof(tCelContext));
	DWORD a6;
	if (D2CLIENT_GetItemGfxData_6FB5EC80(&tCelContext, pItem, 0, nDirection, 0, &a6, 1, -1)
		&& D2CMP_SpriteValidate(&tCelContext, FALSE, TRUE)
		&& CelCullTestInScreen(tCelContext.pCurrentCell, nXpos, nYpos + D2CMP_CelGetHeight(tCelContext.pCurrentCell), 0)
		)
	{
		const uint32_t celWidth = D2CMP_CelGetWidth(tCelContext.pCurrentCell);
		const uint32_t celHeight = D2CMP_CelGetHeight(tCelContext.pCurrentCell);
		uint8_t colorIdx;
		uint8_t* pPalette = ITEMS_GetColor(0, pItem, &colorIdx, 1);
		DrawMode drawMode = ITEMS_CheckItemFlag(pItem, IFLAG_ETHEREAL, __LINE__, __FILE__) ? DRAWMODE_TRANS50 : DRAWMODE_NORMAL;
		TEXTURE_CelDraw(&tCelContext, nXpos, nYpos + celHeight, -1, drawMode, pPalette);
		D2CLIENT_UNITDRAW_DrawOverlays(pItem, 0xFFu, 0, nXpos + (celWidth / 2), nYpos + (celHeight / 2), 0);
		return TRUE;
	}
	return FALSE;
}

#endif //D2_VERSION_110F
