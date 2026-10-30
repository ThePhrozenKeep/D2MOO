#pragma once

#include "D2WinControlHeader.h"

#include <D2Unicode.h>
#include "Font.h"


enum ButtonFlags
{
	BUTTONFLAG_CALLBACK_ON_PRESS = 1,		// The callback runs in the mouse down handler (with the press), not on the release
	BUTTONFLAG_IS_TOGGLE = 2,				// Each press flips bIsPressed / bIsCallbackPending; the callback runs on the press
	BUTTONFLAG_SKIP_SOUND = 4,
	BUTTONFLAG_WHEEL_UP = 8,				// A mouse wheel step up over the button runs the callback
	BUTTONFLAG_WHEEL_DOWN = 0x10,			// A mouse wheel step down over the button runs the callback
	BUTTONFLAG_HAS_DISABLED_FRAMES = 0x20,	// The cel file holds a third (disabled) set of frames, drawn without transparency
	BUTTONFLAG_SUBTITLE = 0x40,				// nStringId is drawn as a second text line under wszText
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
	int32_t nDisabledFrameOffset;										//0x58
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
//1.14d: 0x00501290
D2WinButtonStrc* __fastcall BUTTON_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, D2CellFileStrc* pCellFile, int32_t(__stdcall* pfCallback)(SMSGHANDLER_PARAMS*), int32_t nActivationVirtualKey, int32_t nBaseFrame, uint32_t nFlags, uint16_t nStringId, int32_t(__stdcall* pfUpdateHoverState)(SMSGHANDLER_PARAMS*));
//D2Win.0x6F8A62C0
//1.14d: 0x00500930
int __fastcall BUTTON_Destroy(D2WinButtonStrc* pButton);
//D2Win.0x6F8A62F0
//1.14d: 0x00500960
BOOL __fastcall BUTTON_IsPressed(D2WinButtonStrc* pButton);
//D2Win.0x6F8A6330
//1.14d: 0x005009A0
void __fastcall BUTTON_SetIsPressed(D2WinButtonStrc* pButton, int32_t bIsPressed);
//D2Win.0x6F8A6360
//1.14d: 0x005009D0
void __fastcall BUTTON_SetText(D2WinButtonStrc* pButton, Font eFont, const Unicode* wszText, int32_t nTextColor);
//D2Win.0x6F8A63A0
//1.14d: 0x00500A20
void __fastcall BUTTON_SetTextWithStringId(D2WinButtonStrc* pButton, Font eFont, uint16_t nStringId, int32_t nTextColor);
//D2Win.0x6F8A63F0
//1.14d: 0x00500A70
void __fastcall BUTTON_SetCoordinates(D2WinButtonStrc* pButton, int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight);
//D2Win.0x6F8A6430
//1.14d: 0x00500AB0
void __fastcall BUTTON_SimulateClick(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A64A0
//1.14d: 0x00500B20
void __fastcall BUTTON_SetBaseFrame(D2WinButtonStrc* pButton, int32_t nBaseFrame);
//D2Win.0x6F8A64D0
//1.14d: 0x00500B50
int32_t __fastcall BUTTON_OnMouseWheelScrolled(D2WinButtonStrc* pButton, int32_t nMouseWheelIncrement);
//D2Win.0x6F8A6570
//1.14d: 0x00500BF0
void __fastcall BUTTON_SetStringId(D2WinButtonStrc* pButton, uint16_t nStringId);
//D2Win.0x6F8A65A0
int32_t __fastcall BUTTON_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8A6A10
//1.14d: 0x00501090
int __stdcall BUTTON_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A6AF0
//1.14d: 0x00501150
int __stdcall BUTTON_HandleMouseUp(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A6BB0
//1.14d: 0x005011F0
int __stdcall BUTTON_HandleVirtualKeyInput(SMSGHANDLER_PARAMS* pMsg);
