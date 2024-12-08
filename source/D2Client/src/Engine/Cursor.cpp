#include <Engine/Cursor.h> 
#include <Engine/Gfx.h> 
#include <D2Client.h>
#include <Window.h>
#include <D2Gfx.h>
#include <Core/WNDPROC.h>
#include <Core/ARCHIVE.h>
#include <D2Math.h>
#include <Texture.h>
#include <Fog.h>
#include <Game/Game.h>
#include <Units/Units.h>
#include <D2CMP.h>

#ifdef D2_VERSION_110F
HMODULE delayedD2CLIENTDllBaseGet();

D2FUNC(D2CLIENT, GetItemGfxData_6FB5EC80, int, __fastcall, (D2GfxDataStrc* pData, D2UnitStrc* pItem, D2CellFileStrc* pCellFile, int nDirection, int nFrame, DWORD* a6, int nComponent, int a8), 0x6FB5EC80 - D2ClientImageBase);

D2VAR(D2CLIENT, pgnCursorType, uint32_t, 0x6FBC1AC4 - D2ClientImageBase);//1.10f: 1ref left in D2Client.0x6FB2E880
D2VAR(D2CLIENT, pgnCursorFrame, int32_t, 0x6FBC1AC8 - D2ClientImageBase); //1.13c:0x6FBCB858 //1.10f: 1ref left in D2Client.0x6FB2E880

//1.10f:0x6FBC1ACC
//1.13c:0x6FBCB85C
uint16_t gnUnusedWord0x6FBC1ACC;
//1.10f:0x6FBC1AD0
//1.13c:0x6FBCB860
DWORD gnCursorLastPositionUpdateTickCount;

D2VAR(D2CLIENT, pgnCursorState, uint32_t, 0x6FBC1AD4 - D2ClientImageBase); //1.10f: 1ref left in D2Client.0x6FB2E880

//1.10f:0x6FBC1AE0
//1.13c:0x6FBCB824
int32_t gnCursorClickOffsetX;
//1.10f:0x6FBC1AE4 
//1.13c:0x6FBCB828
int32_t gnMouseX;
//1.10f:0x6FBC1AE8
//1.13c:0x6FBCB824
int32_t gnMouseY;

D2VAR(D2CLIENT, pgnScreenWidth, int32_t, 0x6FB740EC - D2ClientImageBase);
D2VAR(D2CLIENT, pgnScreenHeight, int32_t, 0x6FB740F0 - D2ClientImageBase);

//1.10f:0x6FBC1B04
uint32_t dword_6FBC1B04;

D2VAR(D2CLIENT, pgpHeldItem, D2UnitStrc*, 0x6FBC1AA4 - D2ClientImageBase); //1.10f: 1ref left in D2Client.0x6FB2E880
 


//1.10f:D2Client.0x6FB96B30
//1.13c:D2Client.0x6FBA8530
D2WindowProcCallbackStrc atCursorCallbacks[] = {
	{D2_WINPROC_MESSAGE, WM_MOUSEMOVE,	CLIENT_OnMouseMove},
	{D2_WINPROC_MESSAGE, WM_NCMOUSEMOVE,CLIENT_OnNonClientMouseMove},
	{D2_WINPROC_MESSAGE, WM_LBUTTONUP,	CLIENT_UpdateCursorOnLeftButtonUp},
};

//1.10f:D2Client.0x6FB96B58
//1.13c:D2Client.0x6FBA8558
const CursorDescStrc atCursorDescs_6FB96B58[NUM_CURSORS]{
	{0,      0,      1,      0,      1, CLIENT_DrawCursorMain, "Gaunt"},
	{1,      0,      8,      0,      1, CLIENT_DrawCursorMain, "Grasp"},
	{1,      0,      8,   0x40,      1, CLIENT_DrawCursorMain, "ohand"},
	{1,      1,      8,   0x20,      1, CLIENT_DrawCursorMain, "orotate"},
	{1,      1,      8,   0x40,      1, CLIENT_DrawCursorMain, "ppress"},
	{1,      1,      8,   0x40,      1, CLIENT_DrawCursorMain, "protate"},
	{0,      0,      1,      0,      0, CLIENT_DrawCursorBuySell, "buysell"},
};
//1.10f:D2Client.0x6FB96C1C
const uint32_t gnCursorTableSize = NUM_CURSORS;

//1.10f:D2Client.0x6FBC1AA8
//1.13c:D2Client.0x6FBCB838
D2CellFileStrc* apCursorCellFiles[NUM_CURSORS];

//1.10f:Inlined
const CursorDescStrc& __fastcall CLIENT_GetCursorDescChecked(int Type)
{
	D2_ASSERT(NUM_CURSORS == gnCursorTableSize);
	D2_ASSERT(Type < NUM_CURSORS);
	return atCursorDescs_6FB96B58[Type];
}

//1.10f:D2Client.0x6FB579A0
//1.13c:D2Client.0x6FAC6130
uint32_t __fastcall CLIENT_GetCursorUnk0x08(D2C_CursorTypes Type)
{
	return CLIENT_GetCursorDescChecked(Type).unk0x08;
}

//1.10f:D2Client.0x6FB57A00
//1.13f:D2Client.0x6FAC60E0
uint32_t __fastcall CLIENT_GetCursorFrameDuration(D2C_CursorTypes Type)
{
	return CLIENT_GetCursorDescChecked(Type).unk0x0C << 8;
}

//1.10f:D2Client.0x6FB57330
//1.13c:D2Client.0x6FAC63D0
void __stdcall CLIENT_OnMouseMove(SMSGHANDLER_PARAMS* pMsg)
{
	gnMouseX = (int)(int16_t)LOWORD(pMsg->lParam); // GET_X_LPARAM
	gnMouseY = (int)(int16_t)HIWORD(pMsg->lParam); // GET_Y_LPARAM
	if (D2GFX_HardwareAcceleratedRenderMode() && !FOG_IsHandlingError())
	{
		bool clampedPos = false;
		if (gnMouseX > *D2CLIENT_pgnScreenWidth - 1)
		{
			gnMouseX = *D2CLIENT_pgnScreenWidth - 1;
			clampedPos = true;
		}
		if (gnMouseY > *D2CLIENT_pgnScreenHeight - 1)
		{
			gnMouseY = *D2CLIENT_pgnScreenHeight - 1;
			clampedPos = true;
		}
		if (clampedPos)
		{
			SetCursorPos(gnMouseX, gnMouseY);
			return;
		}
	}
	WINDOW_ShowCursor(TRUE);
	gnCursorLastPositionUpdateTickCount = GetTickCount();
	if (*D2CLIENT_pgnCursorState == CURSORSTATE_HandOpened || *D2CLIENT_pgnCursorState == CURSORSTATE_HandOpening)
	{
		*D2CLIENT_pgnCursorState = CURSORSTATE_HandClosing;
		*D2CLIENT_pgnCursorType = CURSOR_OHand;
		*D2CLIENT_pgnCursorFrame = CLIENT_GetCursorFrameDuration(CURSOR_OHand) - 256;
	}
	pMsg->bUseResult = FALSE;
	pMsg->lResult = 0;
}

//1.10f:D2Client.0x6FB57450
//1.13c:D2Client.0x6FAC60A0
void __stdcall CLIENT_OnNonClientMouseMove(SMSGHANDLER_PARAMS* pMsg)
{
	if (pMsg->wParam == HTCAPTION)
		WINDOW_ShowCursor(FALSE);
	pMsg->bUseResult = FALSE;
	pMsg->lResult = 0;
}

//1.10f:D2Client.0x6FB57480
//1.13c:D2Client.0x6FAC6360
void __stdcall CLIENT_UpdateCursorPosInGame(SMSGHANDLER_PARAMS* pMsg)
{
	gnMouseX = (int)(int16_t)LOWORD(pMsg->lParam); // GET_X_LPARAM
	gnMouseY = (int)(int16_t)HIWORD(pMsg->lParam); // GET_Y_LPARAM
	gnCursorLastPositionUpdateTickCount = GetTickCount();
	if (*D2CLIENT_pgnCursorState != CURSORSTATE_ButtonPressed)
	{
		if (atCursorDescs_6FB96B58[*D2CLIENT_pgnCursorType].unk0x14)
		{
			// Trigger example: clicking on skill/stats +1 icon or leveling something up
			*D2CLIENT_pgnCursorState = CURSORSTATE_ButtonPressed;
			*D2CLIENT_pgnCursorType = CURSOR_PPress;
			*D2CLIENT_pgnCursorFrame = 0;
		}
	}
	pMsg->bUseResult = FALSE;
}

//1.10f:D2Client.0x6FB57500
//1.13c:D2Client.0x6FAC6300
void __stdcall CLIENT_UpdateCursorOnLeftButtonUp(SMSGHANDLER_PARAMS* pMsg)
{
	gnMouseX = (int)(int16_t)LOWORD(pMsg->lParam); // GET_X_LPARAM
	gnMouseY = (int)(int16_t)HIWORD(pMsg->lParam); // GET_Y_LPARAM
	gnCursorLastPositionUpdateTickCount = GetTickCount();
	if (*D2CLIENT_pgnCursorState == CURSORSTATE_ButtonPressed)
	{
		*D2CLIENT_pgnCursorState = CURSORSTATE_Default;
		*D2CLIENT_pgnCursorType = CURSOR_PRotate;
		*D2CLIENT_pgnCursorFrame = 0;
	}
	pMsg->bUseResult = FALSE;
}

//1.10f:D2Client.0x6FB57580
//1.13c:D2Client.0x
void __fastcall CLIENT_SetCursorBuySell(int nFrame, BOOL bUnknown)
{
	*D2CLIENT_pgpHeldItem = nullptr;
	*D2CLIENT_pgnCursorType = CURSOR_Buysell;
	*D2CLIENT_pgnCursorFrame = nFrame; // Buy = 3 Sell = 4
	*D2CLIENT_pgnCursorState = bUnknown ? CURSORSTATE_ItemRelated8 : CURSORSTATE_ItemRelated7; // Always set to FALSE => 7
}

//1.10f:D2Client.0x6FB575B0
//1.13c:D2Client.0x
void __fastcall CLIENT_SetCursorItem(D2UnitStrc* pItem)
{
	*D2CLIENT_pgpHeldItem = pItem;
	*D2CLIENT_pgnCursorFrame = 0;
	*D2CLIENT_pgnCursorType = CURSOR_PRotate;
	*D2CLIENT_pgnCursorState = CURSORSTATE_Default;
	if (pItem)
		*D2CLIENT_pgnCursorState = CURSORSTATE_HandOpened;
}


//1.10f:0x6FB575E0
D2UnitStrc* __cdecl CLIENT_GetCursorItem()
{
	return *D2CLIENT_pgpHeldItem;
}

//1.10f:D2Client.0x6FB575F0
//1.13c:D2Client.0x6FAC66C0
BOOL __fastcall CLIENT_LoadCursors(uint32_t nCursorClickOffsetX)
{
	gnCursorClickOffsetX = nCursorClickOffsetX;

	D2CLIENT_INPUT_RegisterCallbacks(WINDOW_GetWindow(), atCursorCallbacks, ARRAY_SIZE(atCursorCallbacks));

	for(int i = 0; i < NUM_CURSORS; i++)
	{
		CHAR szFilename[80];
		wsprintfA(szFilename, "%s\\UI\\CURSOR\\%s", "DATA\\GLOBAL", atCursorDescs_6FB96B58[i].szName);
		apCursorCellFiles[i] = D2CLIENT_CELLFILE_GetCellFile(szFilename, FALSE);
	}
	dword_6FBC1B04 = 1;
	*D2CLIENT_pgnCursorState = CURSORSTATE_Default;
	*D2CLIENT_pgnCursorType = CURSOR_PRotate;
	*D2CLIENT_pgnCursorFrame = 0;
	gnUnusedWord0x6FBC1ACC = 0;
	gnCursorLastPositionUpdateTickCount = GetTickCount();
	gnMouseX = *D2CLIENT_pgnScreenWidth / 2;
	gnMouseY = *D2CLIENT_pgnScreenHeight / 2;
	return 1;
}

//1.10f:D2Client.0x6FB576B0
void __fastcall CLIENT_UnloadCursors()
{
	D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), atCursorCallbacks, ARRAY_SIZE(atCursorCallbacks));
	dword_6FBC1B04 = 0;

	for (int i = 0; i < NUM_CURSORS; i++)
	{
		D2CLIENT_CELLFILE_FreeCellFile(apCursorCellFiles[i]);
		apCursorCellFiles[i] = nullptr;
	}
}

//1.10f:D2Client.0x6FB57BC0
//1.13c:D2Client.0x6FAC5FB0
int __fastcall CLIENT_GetMouseX()
{
	return gnMouseX;
}

//1.10f:D2Client.0x6FB57BD0
//1.13c:D2Client.0x6FAC5FA0
int __fastcall CLIENT_GetMouseY()
{
	return gnMouseY;
}

//1.10f:D2Client.0x6FB57BE0
//1.13c:D2Client.0x6FAC61B0
void __fastcall CLIENT_SetCursorPos(int nX, int nY)
{
	D2_ASSERT(nX >= 0);
	D2_ASSERT(nY >= 0);	
	if (WINDOW_IsFullScreen())
	{
		SetCursorPos(nX, nY);
	}
	else
	{
		RECT windowRect;
		GetWindowRect(WINDOW_GetWindow(), &windowRect);
		const int nDrawOriginX = windowRect.left + GetSystemMetrics(SM_CXFIXEDFRAME);
		const int nDrawOriginY = windowRect.top + GetSystemMetrics(SM_CXFIXEDFRAME);
		SetCursorPos(nDrawOriginX + nX, nDrawOriginY + nY);
	}
	gnMouseX = nY;
	gnMouseY = nX;
}

//1.10f:D2Client.Inlined
//1.13c:D2Client.0x6FAC6250
void sub_6FAC6250()
{
	*D2CLIENT_pgnCursorFrame -= 64;
	if (*D2CLIENT_pgnCursorFrame < 0)
	{
		if (CLIENT_GetCursorUnk0x08((D2C_CursorTypes)*D2CLIENT_pgnCursorType))
		{
			*D2CLIENT_pgnCursorFrame -= CLIENT_GetCursorFrameDuration((D2C_CursorTypes)*D2CLIENT_pgnCursorType);
		}
		else
		{
			*D2CLIENT_pgnCursorState = CURSORSTATE_Default;
			*D2CLIENT_pgnCursorType = CURSOR_PRotate;
			*D2CLIENT_pgnCursorFrame = 0;
		}
	}
}

//1.10f:D2Client.Inlined
//1.13c:D2Client.0x6FAC64F0
void __fastcall sub_6FAC64F0()
{
	const D2C_CursorTypes nCursorType = (D2C_CursorTypes)(*D2CLIENT_pgnCursorType);
	const auto& cursorDesc = CLIENT_GetCursorDescChecked(nCursorType);
	if (D2UnitStrc* pPlayerUnit = D2CLIENT_GetPlayerUnit_6FB283D0())
	{
		if (*D2CLIENT_pgnCursorState == CURSORSTATE_Default)
		{
			uint8_t v4 = SEED_RollLimitedRandomNumber(&pPlayerUnit->pSeed, 64);
			if (v4 < 16)
				*D2CLIENT_pgnCursorFrame += 32;
		}
		else
		{
			*D2CLIENT_pgnCursorFrame += cursorDesc.unk0x10;
		}

		if (*D2CLIENT_pgnCursorFrame >= CLIENT_GetCursorFrameDuration(nCursorType))
		{
			if (CLIENT_GetCursorUnk0x08(nCursorType))
			{
				*D2CLIENT_pgnCursorFrame -= CLIENT_GetCursorFrameDuration(nCursorType);
			}
			else if (*D2CLIENT_pgnCursorState == CURSORSTATE_HandOpening)
			{
				*D2CLIENT_pgnCursorType = CURSOR_ORotate;
				*D2CLIENT_pgnCursorFrame = 0;
				*D2CLIENT_pgnCursorState = CURSORSTATE_HandOpened;
			}
			else
			{
				if (*D2CLIENT_pgnCursorState != CURSORSTATE_ButtonPressed)
				{
					D2_ASSERT(FALSE);
				}
				*D2CLIENT_pgnCursorState = CURSORSTATE_Default;
				*D2CLIENT_pgnCursorType = CURSOR_PRotate;
				*D2CLIENT_pgnCursorFrame = 0;
			}
		}
	}
}

//1.10f:D2Client.Inlined
//1.13f:D2CLient.0x6FAC6870
void __fastcall CLIENT_UpdateCursors()
{
	if (CLIENT_GetCursorDescChecked(*D2CLIENT_pgnCursorType).unk0x04)
	{
		if (*D2CLIENT_pgnCursorState == CURSORSTATE_HandClosing)
		{
			sub_6FAC6250();
		}
		else
		{
			sub_6FAC64F0();
		}
	}

	if (*D2CLIENT_pgnCursorState == CURSORSTATE_Default)
	{
		// If idling for 5 seconds, change cursor to OHand
		if (GetTickCount() > gnCursorLastPositionUpdateTickCount + 5000)
		{
			*D2CLIENT_pgnCursorFrame = 0;
			*D2CLIENT_pgnCursorState = CURSORSTATE_HandOpening;
			*D2CLIENT_pgnCursorType = CURSOR_OHand;
		}
	}
}

//1.10f:D2CLient.0x6FB57700
//1.13c:D2CLient.0x6FAC67D0
void __fastcall CLIENT_DrawCursorMain()
{
	int32_t cursorOffsetX = gnCursorClickOffsetX;
	int32_t nPosX = D2Clamp<int32_t>(cursorOffsetX + gnMouseX, cursorOffsetX, *D2CLIENT_pgnScreenWidth - cursorOffsetX - 1);
	int32_t nPosY = D2Clamp<int32_t>(gnMouseY, 0, *D2CLIENT_pgnScreenHeight - 1);
	D2GfxDataStrc tCelContext;
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.pCellFile = apCursorCellFiles[*D2CLIENT_pgnCursorType];
	tCelContext.nDirection = 0;
	tCelContext.nFrame = (*D2CLIENT_pgnCursorFrame) >> 8;
	TEXTURE_CelDraw(&tCelContext, nPosX, nPosY, -1, DRAWMODE_NORMAL, 0);
	CLIENT_UpdateCursors();
}

//1.10f:D2Client.0x6FB57A60
//1.13c:D2CLient.0x6FAC5FC0
void __fastcall CLIENT_DrawCursorBuySell()
{
	D2GfxDataStrc tCelContext;
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.pCellFile = apCursorCellFiles[CURSOR_Buysell];
	tCelContext.nFrame = *D2CLIENT_pgnCursorFrame;
	tCelContext.nDirection = 0;
	TEXTURE_CelDraw(&tCelContext, gnMouseX + gnCursorClickOffsetX, gnMouseY+ 33, -1, DRAWMODE_NORMAL, 0);
}

//1.10f:D2Client.0x6FB57AC0
void __fastcall CLIENT_DrawCursorDefault()
{
	if (dword_6FBC1B04)
	{
		if (*D2CLIENT_pgpHeldItem != nullptr 
			&& (*D2CLIENT_pgpHeldItem)->dwUnitType == UNIT_ITEM 
			&& *D2CLIENT_pgnCursorState <= CURSORSTATE_ButtonPressed)
		{
			D2GfxDataStrc tCelContext;
			memset(&tCelContext, 0, sizeof(tCelContext));
			DWORD a6;
			if (CLIENT_GetItemGfxData(&tCelContext, *D2CLIENT_pgpHeldItem, 0, 0, 0, &a6, 1, -1)
				&& D2CMP_SpriteValidate(&tCelContext, 0, 1))
			{
				D2_ASSERT(tCelContext.pCurrentCell);
				const unsigned CellWidth = D2CMP_CelGetWidth(tCelContext.pCurrentCell);
				const int drawPosX = gnMouseX + gnCursorClickOffsetX - (CellWidth / 2);
				const unsigned CellHeight = D2CMP_CelGetHeight(tCelContext.pCurrentCell);
				const int drawPosY = gnMouseY - (CellHeight / 2);
				CLIENT_DrawItem(*D2CLIENT_pgpHeldItem, drawPosX, drawPosY);
			}
		}
		else
		{
			atCursorDescs_6FB96B58[*D2CLIENT_pgnCursorType].pDrawFunction();
		}
	}
}

//1.10f:0x6FB57CC0
uint32_t __fastcall CLIENT_CursorGetDword0x6FBC1AD4()
{
	return *D2CLIENT_pgnCursorState;
}


#endif //D2_VERSION_110F

