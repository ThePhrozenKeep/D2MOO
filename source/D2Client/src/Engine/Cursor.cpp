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
#include <CGAME/Game.h>
#include <Units/Units.h>
#include <D2CMP.h>

#ifdef D2_VERSION_110F
HMODULE delayedD2CLIENTDllBaseGet();

D2FUNC(D2CLIENT, GetItemGfxData_6FB5EC80, int, __fastcall, (D2GfxDataStrc* pData, D2UnitStrc* pItem, D2CellFileStrc* pCellFile, int nDirection, int nFrame, DWORD* a6, int nComponent, int a8), 0x6FB5EC80 - D2ClientImageBase);

//1.10f: D2Client.0x6FBC1AA4
//1.13c: D2Client.0x6FBCB834
D2UnitStrc* gpCursorItem = nullptr;

//1.10f: D2Client.0x6FBC1AC4
//1.13c: D2Client.0x6FBCB854
D2C_CursorTypes gnCursorType = CURSOR_Gauntlet;

//1.10f: D2Client.0x6FBC1AC8 
//1.13c: D2Client.0x6FBCB858
int32_t gnCursorFrame = 0;

//1.10f: D2Client.0x6FBC1ACC
//1.13c: D2Client.0x6FBCB85C
uint16_t gnUnusedWord0x6FBC1ACC;

//1.10f: D2Client.0x6FBC1AD0
//1.13c: D2Client.0x6FBCB860
DWORD gnCursorLastPositionUpdateTickCount;

//1.10f: D2Client.0x6FBC1AD4
//1.13c: D2Client.0x6FBCB864
D2C_CursorState gnCursorState;

//1.10f: D2Client.0x6FBC1AE0
//1.13c: D2Client.0x6FBCB824
int32_t gnCursorClickOffsetX;
//1.10f: D2Client.0x6FBC1AE4 
//1.13c: D2Client.0x6FBCB828
int32_t gnMouseX;
//1.10f: D2Client.0x6FBC1AE8
//1.13c: D2Client.0x6FBCB824
int32_t gnMouseY;

D2VAR(D2CLIENT, pgnScreenWidth, int32_t, 0x6FB740EC - D2ClientImageBase);
D2VAR(D2CLIENT, pgnScreenHeight, int32_t, 0x6FB740F0 - D2ClientImageBase);

//1.10f:0x6FBC1B04
uint32_t dword_6FBC1B04;

 


//1.10f: D2Client.0x6FB96B30
//1.13c: D2Client.0x6FBA8530
D2WindowProcCallbackStrc atCursorCallbacks[] = {
	{D2_WINPROC_MESSAGE, WM_MOUSEMOVE,	CLIENT_OnMouseMove},
	{D2_WINPROC_MESSAGE, WM_NCMOUSEMOVE,CLIENT_OnNonClientMouseMove},
	{D2_WINPROC_MESSAGE, WM_LBUTTONUP,	CLIENT_UpdateCursorOnLeftButtonUp},
};

//1.10f: D2Client.0x6FB96B58
//1.13c: D2Client.0x6FBA8558
const CursorDescStrc atCursorDescs_6FB96B58[NUM_CURSORS]{
	{0,      0,      1,      0,      1, CLIENT_DrawCursorMain, "Gaunt"},
	{1,      0,      8,      0,      1, CLIENT_DrawCursorMain, "Grasp"},
	{1,      0,      8,   0x40,      1, CLIENT_DrawCursorMain, "ohand"},
	{1,      1,      8,   0x20,      1, CLIENT_DrawCursorMain, "orotate"},
	{1,      1,      8,   0x40,      1, CLIENT_DrawCursorMain, "ppress"},
	{1,      1,      8,   0x40,      1, CLIENT_DrawCursorMain, "protate"},
	{0,      0,      1,      0,      0, CLIENT_DrawCursorBuySell, "buysell"},
};
//1.10f: D2Client.0x6FB96C1C
const uint32_t gnCursorTableSize = NUM_CURSORS;

//1.10f: D2Client.0x6FBC1AA8
//1.13c: D2Client.0x6FBCB838
D2CellFileStrc* apCursorCellFiles[NUM_CURSORS];

//1.10f:Inlined
const CursorDescStrc& __fastcall CLIENT_GetCursorDescChecked(int Type)
{
	D2_ASSERT(NUM_CURSORS == gnCursorTableSize);
	D2_ASSERT(Type < NUM_CURSORS);
	return atCursorDescs_6FB96B58[Type];
}

//1.10f: D2Client.0x6FB579A0
//1.13c: D2Client.0x6FAC6130
uint32_t __fastcall CLIENT_GetCursorUnk0x08(D2C_CursorTypes Type)
{
	return CLIENT_GetCursorDescChecked(Type).unk0x08;
}

//1.10f: D2Client.0x6FB57A00
//1.13f:D2Client.0x6FAC60E0
uint32_t __fastcall CLIENT_GetCursorFrameDuration(D2C_CursorTypes Type)
{
	return CLIENT_GetCursorDescChecked(Type).unk0x0C << 8;
}

//1.10f: D2Client.0x6FB57330
//1.13c: D2Client.0x6FAC63D0
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
	if (gnCursorState == CURSORSTATE_HandOpened || gnCursorState == CURSORSTATE_HandOpening)
	{
		gnCursorState = CURSORSTATE_HandClosing;
		gnCursorType = CURSOR_OHand;
		gnCursorFrame = CLIENT_GetCursorFrameDuration(CURSOR_OHand) - 256;
	}
	pMsg->bUseResult = FALSE;
	pMsg->lResult = 0;
}

//1.10f: D2Client.0x6FB57450
//1.13c: D2Client.0x6FAC60A0
void __stdcall CLIENT_OnNonClientMouseMove(SMSGHANDLER_PARAMS* pMsg)
{
	if (pMsg->wParam == HTCAPTION)
		WINDOW_ShowCursor(FALSE);
	pMsg->bUseResult = FALSE;
	pMsg->lResult = 0;
}

//1.10f: D2Client.0x6FB57480
//1.13c: D2Client.0x6FAC6360
void __stdcall CLIENT_UpdateCursorPosInGame(SMSGHANDLER_PARAMS* pMsg)
{
	gnMouseX = (int)(int16_t)LOWORD(pMsg->lParam); // GET_X_LPARAM
	gnMouseY = (int)(int16_t)HIWORD(pMsg->lParam); // GET_Y_LPARAM
	gnCursorLastPositionUpdateTickCount = GetTickCount();
	if (gnCursorState != CURSORSTATE_ButtonPressed)
	{
		if (atCursorDescs_6FB96B58[gnCursorType].unk0x14)
		{
			// Trigger example: clicking on skill/stats +1 icon or leveling something up
			gnCursorState = CURSORSTATE_ButtonPressed;
			gnCursorType = CURSOR_PPress;
			gnCursorFrame = 0;
		}
	}
	pMsg->bUseResult = FALSE;
}

//1.10f: D2Client.0x6FB57500
//1.13c: D2Client.0x6FAC6300
void __stdcall CLIENT_UpdateCursorOnLeftButtonUp(SMSGHANDLER_PARAMS* pMsg)
{
	gnMouseX = (int)(int16_t)LOWORD(pMsg->lParam); // GET_X_LPARAM
	gnMouseY = (int)(int16_t)HIWORD(pMsg->lParam); // GET_Y_LPARAM
	gnCursorLastPositionUpdateTickCount = GetTickCount();
	if (gnCursorState == CURSORSTATE_ButtonPressed)
	{
		gnCursorState = CURSORSTATE_Default;
		gnCursorType = CURSOR_PRotate;
		gnCursorFrame = 0;
	}
	pMsg->bUseResult = FALSE;
}

//1.10f: D2Client.0x6FB57580
//1.13c: D2Client.0x6FAC6030
void __fastcall CLIENT_SetCursorBuySell(int nFrame, BOOL bUnknown)
{
	gpCursorItem = nullptr;
	gnCursorType = CURSOR_Buysell;
	gnCursorFrame = nFrame; // Buy = 3 Sell = 4
	gnCursorState = bUnknown ? CURSORSTATE_ItemRelated8 : CURSORSTATE_ItemRelated7; // Always set to FALSE => 7
}

//1.10f: Inlined in D2CLient.0x6FB2E880
//1.13c: D2Client.0x6FAC6070
void __fastcall CLIENT_SetCursorUsingItem(int nFrame, D2UnitStrc* pItem)
{
	gpCursorItem = pItem;
	gnCursorType = CURSOR_Buysell;
	gnCursorFrame = nFrame;
	gnCursorState = CURSORSTATE_ItemSelected;
}


//1.10f: D2Client.0x6FB575B0
//1.13c: D2Client.0x6FAC62D0
void __fastcall CLIENT_SetCursorHeldItem(D2UnitStrc* pItem)
{
	gpCursorItem = pItem;
	gnCursorFrame = 0;
	gnCursorType = CURSOR_PRotate;
	gnCursorState = CURSORSTATE_Default;
	if (pItem)
		gnCursorState = CURSORSTATE_HandOpened;
}


//1.10f:0x6FB575E0
D2UnitStrc* __cdecl CLIENT_GetCursorItem()
{
	return gpCursorItem;
}

//1.10f: D2Client.0x6FB575F0
//1.13c: D2Client.0x6FAC66C0
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
	gnCursorState = CURSORSTATE_Default;
	gnCursorType = CURSOR_PRotate;
	gnCursorFrame = 0;
	gnUnusedWord0x6FBC1ACC = 0;
	gnCursorLastPositionUpdateTickCount = GetTickCount();
	gnMouseX = *D2CLIENT_pgnScreenWidth / 2;
	gnMouseY = *D2CLIENT_pgnScreenHeight / 2;
	return 1;
}

//1.10f: D2Client.0x6FB576B0
//1.13c: D2Client.0x6FAC6620
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

//1.10f: D2Client.0x6FB57BC0
//1.13c: D2Client.0x6FAC5FB0
int __fastcall CLIENT_GetMouseX()
{
	return gnMouseX;
}

//1.10f: D2Client.0x6FB57BD0
//1.13c: D2Client.0x6FAC5FA0
int __fastcall CLIENT_GetMouseY()
{
	return gnMouseY;
}

//1.10f: D2Client.0x6FB57BE0
//1.13c: D2Client.0x6FAC61B0
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

//1.10f: D2Client.Inlined
//1.13c: D2Client.0x6FAC6250
void sub_6FAC6250()
{
	gnCursorFrame -= 64;
	if (gnCursorFrame < 0)
	{
		if (CLIENT_GetCursorUnk0x08((D2C_CursorTypes)gnCursorType))
		{
			gnCursorFrame -= CLIENT_GetCursorFrameDuration((D2C_CursorTypes)gnCursorType);
		}
		else
		{
			gnCursorState = CURSORSTATE_Default;
			gnCursorType = CURSOR_PRotate;
			gnCursorFrame = 0;
		}
	}
}

//1.10f: D2Client.Inlined
//1.13c: D2Client.0x6FAC64F0
void __fastcall sub_6FAC64F0()
{
	const D2C_CursorTypes nCursorType = (D2C_CursorTypes)(gnCursorType);
	const auto& cursorDesc = CLIENT_GetCursorDescChecked(nCursorType);
	if (D2UnitStrc* pPlayerUnit = D2CLIENT_GetControlUnit())
	{
		if (gnCursorState == CURSORSTATE_Default)
		{
			uint8_t v4 = SEED_RollLimitedRandomNumber(&pPlayerUnit->pSeed, 64);
			if (v4 < 16)
				gnCursorFrame += 32;
		}
		else
		{
			gnCursorFrame += cursorDesc.unk0x10;
		}

		if (gnCursorFrame >= CLIENT_GetCursorFrameDuration(nCursorType))
		{
			if (CLIENT_GetCursorUnk0x08(nCursorType))
			{
				gnCursorFrame -= CLIENT_GetCursorFrameDuration(nCursorType);
			}
			else if (gnCursorState == CURSORSTATE_HandOpening)
			{
				gnCursorType = CURSOR_ORotate;
				gnCursorFrame = 0;
				gnCursorState = CURSORSTATE_HandOpened;
			}
			else
			{
				if (gnCursorState != CURSORSTATE_ButtonPressed)
				{
					D2_ASSERT(FALSE);
				}
				gnCursorState = CURSORSTATE_Default;
				gnCursorType = CURSOR_PRotate;
				gnCursorFrame = 0;
			}
		}
	}
}

//1.10f: D2Client.Inlined
//1.13f:D2CLient.0x6FAC6870
void __fastcall CLIENT_UpdateCursors()
{
	if (CLIENT_GetCursorDescChecked(gnCursorType).unk0x04)
	{
		if (gnCursorState == CURSORSTATE_HandClosing)
		{
			sub_6FAC6250();
		}
		else
		{
			sub_6FAC64F0();
		}
	}

	if (gnCursorState == CURSORSTATE_Default)
	{
		// If idling for 5 seconds, change cursor to OHand
		if (GetTickCount() > gnCursorLastPositionUpdateTickCount + 5000)
		{
			gnCursorFrame = 0;
			gnCursorState = CURSORSTATE_HandOpening;
			gnCursorType = CURSOR_OHand;
		}
	}
}

//1.10f: D2CLient.0x6FB57700
//1.13c: D2CLient.0x6FAC67D0
void __fastcall CLIENT_DrawCursorMain()
{
	int32_t cursorOffsetX = gnCursorClickOffsetX;
	int32_t nPosX = D2Clamp<int32_t>(cursorOffsetX + gnMouseX, cursorOffsetX, *D2CLIENT_pgnScreenWidth - cursorOffsetX - 1);
	int32_t nPosY = D2Clamp<int32_t>(gnMouseY, 0, *D2CLIENT_pgnScreenHeight - 1);
	D2GfxDataStrc tCelContext;
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.pCellFile = apCursorCellFiles[gnCursorType];
	tCelContext.nDirection = 0;
	tCelContext.nFrame = (gnCursorFrame) >> 8;
	TEXTURE_CelDraw(&tCelContext, nPosX, nPosY, -1, DRAWMODE_NORMAL, 0);
	CLIENT_UpdateCursors();
}

//1.10f: D2Client.0x6FB57A60
//1.13c: D2CLient.0x6FAC5FC0
void __fastcall CLIENT_DrawCursorBuySell()
{
	D2GfxDataStrc tCelContext;
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.pCellFile = apCursorCellFiles[CURSOR_Buysell];
	tCelContext.nFrame = gnCursorFrame;
	tCelContext.nDirection = 0;
	TEXTURE_CelDraw(&tCelContext, gnMouseX + gnCursorClickOffsetX, gnMouseY+ 33, -1, DRAWMODE_NORMAL, 0);
}

//1.10f: D2Client.0x6FB57AC0
void __fastcall CLIENT_DrawCursorDefault()
{
	if (dword_6FBC1B04)
	{
		if (gpCursorItem != nullptr 
			&& (gpCursorItem)->dwUnitType == UNIT_ITEM 
			&& gnCursorState <= CURSORSTATE_ButtonPressed)
		{
			D2GfxDataStrc tCelContext;
			memset(&tCelContext, 0, sizeof(tCelContext));
			DWORD a6;
			if (CLIENT_GetItemGfxData(&tCelContext, gpCursorItem, 0, 0, 0, &a6, 1, -1)
				&& D2CMP_SpriteValidate(&tCelContext, 0, 1))
			{
				D2_ASSERT(tCelContext.pCurrentCell);
				const unsigned CellWidth = D2CMP_CelGetWidth(tCelContext.pCurrentCell);
				const int drawPosX = gnMouseX + gnCursorClickOffsetX - (CellWidth / 2);
				const unsigned CellHeight = D2CMP_CelGetHeight(tCelContext.pCurrentCell);
				const int drawPosY = gnMouseY - (CellHeight / 2);
				CLIENT_DrawItem(gpCursorItem, drawPosX, drawPosY);
			}
		}
		else
		{
			atCursorDescs_6FB96B58[gnCursorType].pDrawFunction();
		}
	}
}

//1.10f:0x6FB57CC0
uint32_t __fastcall CLIENT_CursorGetDword0x6FBC1AD4()
{
	return gnCursorState;
}


#endif //D2_VERSION_110F

