#pragma once

#include "D2WinControlHeader.h"

#include <D2Unicode.h>
#include "Font.h"


enum ButtonFlags
{
	BUTTONFLAG_IS_TOGGLE = 2,
	BUTTONFLAG_SKIP_SOUND = 4,
};


#pragma pack(push, 1)
struct D2WinButtonStrc
{
	D2WinControlStrc controlHeader;							//0x00
	uint32_t dwButtonFlags;									//0x40
	int32_t bIsPressed;										//0x44
	int32_t bIsCallbackPending;								//0x48
	int32_t nActivationCounter;								//0x4C
	int32_t nActivationVirtualKey;							//0x50
	int32_t nIsPressedFrameOffset;							//0x54
	int32_t field_58;										//0x58
	int32_t nBaseFrame;										//0x5C
	Font eFont;												//0x60
	Unicode wszText[256];									//0x64
	int32_t nTextColor;										//0x264
	int32_t(__stdcall* pfUpdateHoverState)(SMSGHANDLER_PARAMS*);	//0x268
	int32_t isButtonHovered;								//0x26C
	uint16_t nStringId;										//0x270
	int16_t pad0x272;										//0x272
};
#pragma pack(pop)


//D2Win.0x6F8A61A0
D2WinButtonStrc* __fastcall BUTTON_Create(int nX, int nY, int nWidth, int nHeight, D2CellFileStrc* pCellFile, int(__stdcall* pCallback)(SMSGHANDLER_PARAMS*), int a7, int a8, uint32_t a9, uint16_t nStringId, int(__stdcall* a11)(SMSGHANDLER_PARAMS*));
//D2Win.0x6F8A62C0
int __fastcall BUTTON_Destroy(D2WinButtonStrc* pButton);
//D2Win.0x6F8A62F0
BOOL __fastcall BUTTON_IsPressed(D2WinButtonStrc* pButton);
//D2Win.0x6F8A6330
void __fastcall BUTTON_SetIsPressed(D2WinButtonStrc* pButton, int a2);
//D2Win.0x6F8A6360
void __fastcall BUTTON_SetText(D2WinButtonStrc* pButton, Font nFont, const Unicode* wszText, int nTextColor);
//D2Win.0x6F8A63A0
void __fastcall BUTTON_SetTextWithStringId(D2WinButtonStrc* pButton, Font nFont, WORD nStringIndex, int nTextColor);
//D2Win.0x6F8A63F0
void __fastcall BUTTON_SetCoordinates(D2WinButtonStrc* pButton, int nX, int nY, int nWidth, int nHeight);
//D2Win.0x6F8A6430
void __fastcall BUTTON_SimulateClick(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A64A0
void __fastcall BUTTON_SetBaseFrame(D2WinButtonStrc* pButton, int a2);
//D2Win.0x6F8A64D0
int __fastcall BUTTON_OnMouseWheelScrolled(D2WinButtonStrc* pButton, int a2);
//D2Win.0x6F8A6570
void __fastcall BUTTON_SetStringId(D2WinButtonStrc* pButton, WORD nStringIndex);
//D2Win.0x6F8A65A0
int32_t __fastcall BUTTON_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8A6A10
int __stdcall BUTTON_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A6AF0
int __stdcall BUTTON_HandleMouseUp(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A6BB0
int __stdcall BUTTON_HandleVirtualKeyInput(SMSGHANDLER_PARAMS* pMsg);
