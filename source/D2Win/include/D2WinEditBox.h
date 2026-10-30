#pragma once

#include "D2WinControlHeader.h"

#include <D2Unicode.h>
#include "Font.h"


enum D2EditBoxFlags
{
	EDITBOX_PASSWORD = 0x00000001,				// every character is drawn as '*'
	EDITBOX_RETURN_KEEPS_EDITING = 0x00000002,	// after pfOnReturn, Enter goes on to the caret update and pfOnChange
	EDITBOX_TAB_TO_KEY_FILTER = 0x00000004,		// Tab (char 9) is only handed to pfKeyFilter, never typed
	EDITBOX_MULTILINE = 0x00000008,				// Enter types CR LF, Up / Down move between lines, other draw / mouse handlers
	EDITBOX_ALLOW_CHAR_14 = 0x00000010,			// char 14 may be typed although the font has no glyph for it
	EDITBOX_ALLOW_COLOR_CODE = 0x00000020,		// char 0xFF (the color code prefix) may be typed
};


struct D2WinEditBoxStrc;

// Called on Enter with the box's text, and on Escape with nullptr
using EditBoxReturnCallback = int32_t(__stdcall*)(const Unicode* wszText);
// Called with the box, the caret position and the typed character (which it may replace); FALSE refuses the character
using EditBoxKeyFilterCallback = BOOL(__stdcall*)(D2WinEditBoxStrc* pEditBox, int32_t nPosition, char* pChar);
// Called with 0 after every change of the text
using EditBoxChangeCallback = void(__stdcall*)(int32_t nUnused);

#pragma pack(push, 1)
struct D2WinEditBoxStrc
{
	D2WinControlStrc controlHeader;					//0x00
	int32_t nTextInsetX;							//0x40
	int32_t nTextInsetY;							//0x44	
	int32_t nMaxLength;								//0x48
	int32_t nFirstVisibleChar;						//0x4C
	int32_t nLastVisibleChar;						//0x50
	int32_t nSelectionStart;						//0x54
	int32_t nSelectionEnd;							//0x58
	Unicode wszText[256];							//0x5C
	int32_t nCaretPosition;							//0x25C
	uint32_t dwEditBoxFlags;						//0x260
	EditBoxReturnCallback pfOnReturn;				//0x264
	EditBoxKeyFilterCallback pfKeyFilter;			//0x268
	EditBoxChangeCallback pfOnChange;				//0x26C
	Font eFont;										//0x270
	int32_t nTextColor;								//0x274
	D2WinEditBoxStrc* pNext;						//0x278
	D2WinEditBoxStrc* pPrevious;					//0x27C
	int32_t bMouseSelecting;						//0x280
};
#pragma pack(pop)


//D2Win.0x6F8A6C80
//1.14d: 0x005001D0
D2WinEditBoxStrc* __fastcall D2Win_10070_EDITBOX_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t a5, int32_t a6, D2CellFileStrc* pCellFile, int32_t pfOnReturn, int32_t(__stdcall* pfCallback)(SMSGHANDLER_PARAMS*), int32_t nEditBoxFlags, int32_t a11);
//D2Win.0x6F8A6DC0
//1.14d: 0x004FDAA0
int __fastcall D2Win_10071_EDITBOX_Destroy(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8A6E10
//1.14d: 0x004FDAD0
void __fastcall EDITBOX_SetKeyFilter(D2WinEditBoxStrc* pEditBox, EditBoxKeyFilterCallback pfKeyFilter);
//D2Win.0x6F8A6E60
//1.14d: 0x004FDB00
void __fastcall EDITBOX_SetChangeCallback(D2WinEditBoxStrc* pEditBox, EditBoxChangeCallback pfOnChange);
//D2Win.0x6F8A6EB0
//1.14d: 0x004FDCC0
void __fastcall EDITBOX_ClearSelection(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8A6F10
//1.14d: 0x004FDD00
int __fastcall D2Win_10082(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8A6F70
//1.14d: 0x004FEAE0
int32_t __fastcall D2Win_10074(D2WinEditBoxStrc* pEditBox, uint8_t a2);
//D2Win.0x6F8A7970
//1.14d: 0x004FE9F0
void __fastcall sub_6F8A7970(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8A7AB0
//1.14d: 0x004FF050
int __fastcall EDITBOX_HandleKeyDown(D2WinEditBoxStrc* pEditBox, char nVirtualKey);
//D2Win.0x6F8A80A0
//1.14d: 0x004FDD50
void __fastcall EDITBOX_SetMaxLength(D2WinEditBoxStrc* pEditBox, int32_t nMaxLength);
//D2Win.0x6F8A80F0
//1.14d: 0x004FFE10
int __fastcall D2Win_10076(D2WinEditBoxStrc* pEditBox, const char* szText);
//D2Win.0x6F8A8140
//1.14d: 0x004FF5A0
int __fastcall EDITBOX_SetText(D2WinEditBoxStrc* pEditBox, const Unicode* pText);
//D2Win.0x6F8A81E0
//1.14d: 0x004FDD90
const Unicode* __fastcall D2Win_10077_EDITBOX_GetText(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8A8230
//1.14d: 0x004FDDD0
void __fastcall D2Win_10080_EDITBOX_SetNextEditBox(D2WinEditBoxStrc* pEditBox, D2WinEditBoxStrc* pNext);
//D2Win.0x6F8A82D0
//1.14d: 0x004FFE50
int32_t __fastcall EDITBOX_DrawSingleLine(D2WinControlStrc* pControl);
//D2Win.0x6F8A86C0
//1.14d: 0x004FF620
int32_t __fastcall EDITBOX_DrawMultiLine(D2WinControlStrc* pControl);
//D2Win.0x6F8A8D60
//1.14d: 0x004FDFF0
void __fastcall EDITBOX_GetCaretPosition(D2WinEditBoxStrc* pEditBox, int32_t* pX, int32_t* pY);
//D2Win.0x6F8A9120
//1.14d: 0x004FFBF0
int __stdcall EDITBOX_HandleSingleLineMouseDown(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A93A0
//1.14d: 0x004FE350
int __stdcall EDITBOX_HandleMouseUp(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A9410
//1.14d: 0x004FE3B0
int32_t __fastcall EDITBOX_PlaceMultiLineCaret(SMSGHANDLER_PARAMS* pMsg, int32_t nX, int32_t nY);
//D2Win.0x6F8A97D0
//1.14d: 0x004FE710
int __stdcall sub_6F8A97D0(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A9850
//1.14d: 0x004FE770
bool __stdcall D2Win_10078_IsNumber(int32_t nUnused1, int32_t nUnused2, char* szChar);
//D2Win.0x6F8A9870
//1.14d: 0x004FE790
bool __stdcall D2Win_10079_IsLetter(int nUnused1, int nUnused2, char* szChar);
//D2Win.0x6F8A98A0
//1.14d: 0x004FE7C0
void __fastcall sub_6F8A98A0(D2WinEditBoxStrc* pEditBox);
