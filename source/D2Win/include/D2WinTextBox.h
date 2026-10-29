#pragma once

#include "D2WinControlHeader.h"

#include <D2Unicode.h>
#include "Font.h"


struct D2WinScrollBarStrc;

enum TextBoxFlags
{
	TEXTBOXFLAG_SELECTABLE = 1,				// Lines can be selected (mouse / arrow keys, nCurrentLine); implies TEXTBOXFLAG_NO_WORD_WRAP
	TEXTBOXFLAG_CENTERED = 2,				// Column 0 (without a column offset) is centred in the box
	TEXTBOXFLAG_SCROLLBAR = 4,				// Create attaches a scroll bar (and shrinks the box by 26 unless a scroll bar rect is given)
	TEXTBOXFLAG_CLAMP_SELECTION = 8,		// A click below the last line selects the last line instead of nothing
	TEXTBOXFLAG_RIGHT_ALIGNED = 0x10,		// Column 0 (without a column offset) ends at nImageX
	TEXTBOXFLAG_NO_WORD_WRAP = 0x20,		// D2Win_10051 never splits a line (set by more than one column too)
	TEXTBOXFLAG_HORIZONTAL_TICKER = 0x40,	// TEXTBOX_Draw scrolls the top line from right to left
	TEXTBOXFLAG_VERTICAL_SCROLL = 0x80,		// TEXTBOX_Draw scrolls the lines up
};

#pragma pack(push, 1)
struct D2WinTextBoxLineStrc
{
	Unicode* pColumns[5];							//0x00
	int32_t nColor;								//0x14
	char bWrappedSegment;									//0x18
	char field_19;									//0x19
	char field_1A;									//0x1A
	char field_1B;									//0x1B
	D2WinTextBoxLineStrc* pNext;					//0x1C
};

struct D2WinTextBoxStrc
{
	D2WinControlStrc controlHeader;					//0x00
	int32_t nTextInsetX;								//0x40
	int32_t nTextInsetY;								//0x44
	D2WinTextBoxLineStrc* pFirstLine;				//0x48
	D2WinTextBoxLineStrc* pLastLine;				//0x4C
	D2WinTextBoxLineStrc* pCurrentLine;				//0x50
	int32_t nNumLines;								//0x54
	int32_t nLine;									//0x58
	int32_t dwTextBoxFlags;							//0x5C
	int32_t nCurrentLine;							//0x60
	int32_t nNumColumns;							//0x64
	int32_t nColumnOffsets[5];								//0x68
	int32_t nColumnAlignFlags[5];								//0x7C
	D2WinScrollBarStrc* pScrollBar;					//0x90
	Font eFont;										//0x94
	int32_t field_98;								//0x98
	int32_t nScrollOffset;								//0x9C
	int32_t nWrappedLineGap;								//0xA0
	int32_t nLineGap;								//0xA4
	int32_t nLineSpacing;								//0xA8
};
#pragma pack(pop)


//D2Win.0x6F8AFAA0
//1.14d: 0x004FC7A0
D2WinTextBoxStrc* __fastcall D2Win_10042_TEXTBOX_Create(int nX, int nY, int nWidth, int nHeight, int nInsetX, int nInsetY, D2CellFileStrc* pCellFile, int(__stdcall* pfCallback)(SMSGHANDLER_PARAMS*), unsigned int dwTextBoxFlags, int pFontInfo, int pScrollBarRect);
//D2Win.0x6F8AFCC0
//1.14d: 0x004FB5A0
void __fastcall sub_6F8AFCC0(Font nFontSize);
//D2Win.0x6F8AFDC0
//1.14d: 0x004FC960
int32_t __fastcall D2Win_10043_TEXTBOX_Destroy(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8AFE30
//1.14d: 0x004FB6A0
int32_t __fastcall D2Win_10047(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8AFED0
//1.14d: 0x004FB740
int32_t __fastcall D2Win_10062(D2WinTextBoxStrc* pTextBox, int32_t nWrappedLineGap, int32_t nLineGap);
//D2Win.0x6F8AFF30
//1.14d: 0x004FCF80
void __fastcall D2Win_10045(D2WinTextBoxStrc* pTextBox, const char* szText);
//D2Win.0x6F8AFFB0
//1.14d: 0x004FCFF0
BOOL __fastcall D2Win_10046(D2WinTextBoxStrc* pTextBox, const char* szText, int nColor);
//D2Win.0x6F8B0030
//1.14d: 0x004FD060
BOOL __fastcall D2Win_10044(D2WinTextBoxStrc* pTextBox, const Unicode* wszText);
//D2Win.0x6F8B0040
//1.14d: 0x004FC9B0
BOOL __fastcall D2Win_10051(D2WinTextBoxStrc* pTextBox, const Unicode* wszText, int nColor, char bIsWrapped, signed int nIndent);
//D2Win.0x6F8B05E0
//1.14d: 0x004FCC40
int32_t __fastcall D2Win_10198(D2WinTextBoxStrc* pTextBox, const Unicode* pText, int32_t nCount, int32_t nColor, char bIsWrapped);
//D2Win.0x6F8B0750
//1.14d: 0x004FCD40
int32_t __fastcall D2Win_10057(D2WinTextBoxStrc* pTextBox, const char* szText, int32_t nLine, int32_t nColumn);
//D2Win.0x6F8B07C0
//1.14d: 0x004FB790
int __fastcall D2Win_10058_0(D2WinTextBoxStrc* pTextBox, const Unicode* pText, int nLine, int nColumn);
//D2Win.0x6F8B08F0
//1.14d: 0x004FB870
void __fastcall D2Win_10197(D2WinTextBoxStrc* pTextBox, int32_t nLineNum);
//D2Win.0x6F8B09E0
//1.14d: 0x004FB910
int32_t __fastcall D2Win_10052_TEXTBOX_GetNumLines(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0A30
//1.14d: 0x004FB950
int32_t __fastcall D2Win_10054(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0A80
//1.14d: 0x004FB990
int32_t __fastcall D2Win_10049(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0AD0
//1.14d: 0x004FB9D0
void __fastcall D2Win_10050_TEXTBOX_SetLine(D2WinTextBoxStrc* pTextBox, int32_t nLine);
//D2Win.0x6F8B0B30
//1.14d: 0x004FBA30
int32_t __fastcall D2Win_10053(D2WinTextBoxStrc* pTextBox, int32_t nLine);
//D2Win.0x6F8B0BC0
//1.14d: 0x004FBAB0
int __fastcall D2Win_10055(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0C50
//1.14d: 0x004FBB30
Unicode* __fastcall D2Win_10048(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0CC0
//1.14d: 0x004FBB90
Unicode* __fastcall D2Win_10196(D2WinTextBoxStrc* pTextBox, int32_t nLineNum, int32_t nColumnNum);
//D2Win.0x6F8B0DC0
//1.14d: 0x004FBC20
void __fastcall D2Win_10056(D2WinTextBoxStrc* pTextBox, int32_t nNumColumns);
//D2Win.0x6F8B0E60
//1.14d: 0x004FBC80
void __fastcall D2Win_10059(D2WinTextBoxStrc* pTextBox, int nColumn, Unicode* pColumn);
//D2Win.0x6F8B0EE0
//1.14d: 0x004FBCD0
void __fastcall D2Win_10060(D2WinTextBoxStrc* pTextBox, int32_t nColumn, int32_t nAlignFlags);
//D2Win.0x6F8B0F60
//1.14d: 0x004FBD20
int32_t __fastcall D2Win_10061(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0FC0
//1.14d: 0x004FBD70
void __fastcall D2Win_10187(D2WinTextBoxStrc* pTextBox, int32_t bSet);
//D2Win.0x6F8B1030
//1.14d: 0x004FBDE0
int __fastcall D2Win_10188(D2WinTextBoxStrc* pTextBox, int nLineNum, int nColor);
//Inlined in D2Win.0x6F8B1130
//1.14d: 0x004FBEE0
int32_t __fastcall TEXTBOX_TruncateTextToWidth(Unicode* wszText, int32_t nWidth);
//D2Win.0x6F8B1130
//1.14d: 0x004FBF30
int32_t __fastcall TEXTBOX_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8B15A0
//1.14d: 0x004FC2F0
int32_t __stdcall TEXTBOX_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8B16B0
//1.14d: 0x004FC3F0
int32_t __stdcall TEXTBOX_HandleVirtualKeyInput(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8B18F0
//1.14d: 0x004FC5A0
int32_t __fastcall D2Win_10190(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B1940
//1.14d: 0x004FC5E0
int32_t __fastcall D2Win_10191(D2WinTextBoxStrc* pTextBox, int32_t nMaxSteps);
//D2Win.0x6F8B1990
//1.14d: 0x004FC620
int32_t __fastcall D2Win_10192(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B19E0
//1.14d: 0x004FC660
int32_t __fastcall D2Win_10193(D2WinTextBoxStrc* pTextBox, int32_t nCurrentStep);
//D2Win.0x6F8B1A30
//1.14d: 0x004FC6A0
int32_t __fastcall D2Win_10194(D2WinTextBoxStrc* pTextBox, D2WinTextBoxStrc* pLinkedTextBox);
//D2Win.0x6F8B1A80
//1.14d: 0x004FC6E0
int __fastcall D2Win_10195(D2WinTextBoxStrc* pTextBox, void(__stdcall* pfScrollCallback)(SMSGHANDLER_PARAMS*));
//D2Win.0x6F8B1AD0
//1.14d: 0x004FC720
void __fastcall D2Win_10204(D2WinTextBoxStrc* pTextBox, int32_t nPosition);
//D2Win.0x6F8B1B20
//1.14d: 0x004FC760
int32_t __fastcall D2Win_10203_TEXTBOX(D2WinTextBoxStrc* pTextBox);
