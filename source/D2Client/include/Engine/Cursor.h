#pragma once  

#include <D2BasicTypes.h>
#include <Storm.h>
#include <Units/Units.h>

#pragma pack(push, 1)

enum D2C_CursorTypes {
	CURSOR_Gauntlet = 0,
	CURSOR_Grasp = 1,
	CURSOR_OHand = 2,
	CURSOR_ORotate = 3,
	CURSOR_PPress = 4,
	CURSOR_PRotate = 5,
	CURSOR_Buysell = 6,
	NUM_CURSORS = 7,
};

enum D2C_CursorState {
	CURSORSTATE_Default = 1,
	CURSORSTATE_HandOpening = 2,
	CURSORSTATE_HandClosing = 3,
	CURSORSTATE_HandOpened = 4,
	CURSORSTATE_ButtonPressed = 5,
	CURSORSTATE_ItemSelected = 6,
	CURSORSTATE_ItemRelated7 = 7,
	CURSORSTATE_ItemRelated8 = 8,
};

struct CursorDescStrc
{
	uint32_t unk0x04;
	uint32_t unk0x08;
	uint32_t unk0x0C;
	uint32_t unk0x10;
	uint32_t unk0x14;
	void(__fastcall* pDrawFunction)();
	const char* szName;
};
#pragma pack(pop)

//1.10f: D2Client.0x6FB57330
//1.13c: D2Client.0x6FAC63D0
void __stdcall CLIENT_OnMouseMove(SMSGHANDLER_PARAMS* pMsg);

//1.10f: D2Client.0x6FB57450
//1.13c: D2Client.0x6FAC60A0
void __stdcall CLIENT_OnNonClientMouseMove(SMSGHANDLER_PARAMS* pMsg);

//1.10f: D2Client.0x6FB57480
//1.13c: D2Client.0x6FAC6360
void __stdcall CLIENT_UpdateCursorPosInGame(SMSGHANDLER_PARAMS* pMsg);

//1.10f: D2Client.0x6FB57500
//1.13c: D2Client.0x6FAC6300
void __stdcall CLIENT_UpdateCursorOnLeftButtonUp(SMSGHANDLER_PARAMS* pMsg);


//1.10f: D2Client.0x6FB57580
//1.13c: D2Client.0x6FAC6030
void __fastcall CLIENT_SetCursorBuySell(int nFrame, BOOL bUnknown);

//1.10f: Inlined in D2CLient.0x6FB2E880
//1.13c: D2Client.0x6FAC6070
void __fastcall CLIENT_SetCursorUsingItem(int nFrame, D2UnitStrc* pItem);

//1.10f: D2Client.0x6FB575B0
//1.13c: D2Client.0x6FAC62D0
void __fastcall CLIENT_SetCursorHeldItem(D2UnitStrc* pItem);

//1.10f:0x6FB575E0
D2UnitStrc* __cdecl CLIENT_GetCursorItem();

//1.10f: D2Client.0x6FB575F0
//1.13c: D2Client.0x6FAC66C0
BOOL __fastcall CLIENT_LoadCursors(uint32_t nCursorClickOffsetX);

//1.10f: D2Client.0x6FB576B0
void __fastcall CLIENT_UnloadCursors();

//1.10f: D2Client.0x6FB579A0
//1.13c: D2Client.0x6FAC6130
uint32_t __fastcall CLIENT_GetCursorUnk0x08(D2C_CursorTypes Type);

//1.10f: D2Client.0x6FB57A00
//1.13f:D2Client.0x6FAC60E0
uint32_t __fastcall CLIENT_GetCursorFrameDuration(D2C_CursorTypes Type);

//1.10f: D2Client.0x6FB57BC0
//1.13c: D2Client.0x6FAC5FB0
int __fastcall CLIENT_GetMouseX();

//1.10f: D2Client.0x6FB57BD0
//1.13c: D2Client.0x6FAC5FA0
int __fastcall CLIENT_GetMouseY();

//1.10f: D2Client.0x6FB57BE0
//1.13c: D2Client.0x6FAC61B0
void __fastcall CLIENT_SetCursorPos(int nX, int nY);

//1.10f: D2CLient.0x6FB57700
//1.13c: D2CLient.0x6FAC67D0
void __fastcall CLIENT_DrawCursorMain();

//1.10f: D2Client.0x6FB57AC0
void __fastcall CLIENT_DrawCursorDefault();

//1.10f: D2Client.0x6FB57A60
//1.13c: D2CLient.0x6FAC5FC0
void __fastcall CLIENT_DrawCursorBuySell();

//1.10f:0x6FB57CC0
D2C_CursorState __fastcall CLIENT_GetCursorState();
