#pragma once

#include "D2WinControlHeader.h"

#include <D2Unicode.h>
#include "Font.h"


struct D2WinScrollBarStrc;

#pragma pack(push, 1)
struct D2WinTextBoxLineStrc
{
	Unicode* pColumns[5];							//0x00
	int32_t field_14;								//0x14
	char field_18;									//0x18
	char field_19;									//0x19
	char field_1A;									//0x1A
	char field_1B;									//0x1B
	D2WinTextBoxLineStrc* pNext;					//0x1C
};

struct D2WinTextBoxStrc
{
	D2WinControlStrc controlHeader;					//0x00
	int32_t field_40;								//0x40
	int32_t field_44;								//0x44
	D2WinTextBoxLineStrc* pFirstLine;				//0x48
	D2WinTextBoxLineStrc* pLastLine;				//0x4C
	D2WinTextBoxLineStrc* pCurrentLine;				//0x50
	int32_t nNumLines;								//0x54
	int32_t nLine;									//0x58
	int32_t dwTextBoxFlags;							//0x5C
	int32_t nCurrentLine;							//0x60
	int32_t nNumColumns;							//0x64
	int32_t unk0x68[5];								//0x68
	int32_t unk0x7C[5];								//0x7C
	D2WinScrollBarStrc* pScrollBar;					//0x90
	Font eFont;										//0x94
	int32_t field_98;								//0x98
	int32_t field_9C;								//0x9C
	int32_t field_A0;								//0xA0
	int32_t field_A4;								//0xA4
	int32_t field_A8;								//0xA8
};
#pragma pack(pop)


//D2Win.0x6F8AFAA0
D2WinTextBoxStrc* __fastcall D2Win_10042_TEXTBOX_Create(int nX, int nY, int nWidth, int nHeight, int a5, int a6, D2CellFileStrc* pCellFile, int(__stdcall* a8)(SMSGHANDLER_PARAMS*), unsigned int dwTextBoxFlags, int a10, int a11);
//D2Win.0x6F8AFCC0
void __fastcall sub_6F8AFCC0(Font nFontSize);
//D2Win.0x6F8AFDC0
int __fastcall D2Win_10043_TEXTBOX_Destroy(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8AFE30
int __fastcall D2Win_10047(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8AFED0
int __fastcall D2Win_10062(D2WinTextBoxStrc* pTextBox, int a2, int a3);
//D2Win.0x6F8AFF30
void __fastcall D2Win_10045(D2WinTextBoxStrc* pTextBox, const char* szText);
//D2Win.0x6F8AFFB0
BOOL __fastcall D2Win_10046(D2WinTextBoxStrc* pTextBox, const char* szText, int a3);
//D2Win.0x6F8B0030
BOOL __fastcall D2Win_10044(D2WinTextBoxStrc* pTextBox, const Unicode* wszText);
//D2Win.0x6F8B0040
BOOL __fastcall D2Win_10051(D2WinTextBoxStrc* pTextBox, const Unicode* a2, int a3, char a4, signed int a5);
//D2Win.0x6F8B05E0
int __fastcall D2Win_10198(D2WinTextBoxStrc* pTextBox, const Unicode* pText, int a3, int a4, char a5);
//D2Win.0x6F8B0750
int __fastcall D2Win_10057(D2WinTextBoxStrc* pTextBox, const char* a2, int a3, int a4);
//D2Win.0x6F8B07C0
int __fastcall D2Win_10058_0(D2WinTextBoxStrc* pTextBox, const Unicode* pText, int a3, int nColumn);
//D2Win.0x6F8B08F0
void __fastcall D2Win_10197(D2WinTextBoxStrc* pTextBox, int nLineNum);
//D2Win.0x6F8B09E0
int __fastcall D2Win_10052_TEXTBOX_GetNumLines(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0A30
int __fastcall D2Win_10054(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0A80
int __fastcall D2Win_10049(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0AD0
void __fastcall D2Win_10050_TEXTBOX_SetLine(D2WinTextBoxStrc* pTextBox, int nLine);
//D2Win.0x6F8B0B30
int __fastcall D2Win_10053(D2WinTextBoxStrc* pTextBox, int a2);
//D2Win.0x6F8B0BC0
int __fastcall D2Win_10055(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0C50
Unicode* __fastcall D2Win_10048(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0CC0
Unicode* __fastcall D2Win_10196(D2WinTextBoxStrc* pTextBox, int nLineNum, int nColumnNum);
//D2Win.0x6F8B0DC0
void __fastcall D2Win_10056(D2WinTextBoxStrc* pTextBox, int nNumColumns);
//D2Win.0x6F8B0E60
void __fastcall D2Win_10059(D2WinTextBoxStrc* pTextBox, int nColumn, Unicode* pColumn);
//D2Win.0x6F8B0EE0
void __fastcall D2Win_10060(D2WinTextBoxStrc* pTextBox, int nColumn, int a3);
//D2Win.0x6F8B0F60
int __fastcall D2Win_10061(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B0FC0
void __fastcall D2Win_10187(D2WinTextBoxStrc* pTextBox, int bSet);
//D2Win.0x6F8B1030
int __fastcall D2Win_10188(D2WinTextBoxStrc* pTextBox, int nLineNum, int a3);
//D2Win.0x6F8B1130
int32_t __fastcall TEXTBOX_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8B15A0
int __stdcall TEXTBOX_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8B16B0
int __stdcall TEXTBOX_HandleVirtualKeyInput(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8B18F0
int __fastcall D2Win_10190(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B1940
int __fastcall D2Win_10191(D2WinTextBoxStrc* pTextBox, int a2);
//D2Win.0x6F8B1990
int __fastcall D2Win_10192(D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8B19E0
int __fastcall D2Win_10193(D2WinTextBoxStrc* pTextBox, int a2);
//D2Win.0x6F8B1A30
int __fastcall D2Win_10194(D2WinTextBoxStrc* pTextBox, D2WinTextBoxStrc* a2);
//D2Win.0x6F8B1A80
int __fastcall D2Win_10195(D2WinTextBoxStrc* pTextBox, void(__stdcall* a2)(SMSGHANDLER_PARAMS*));
//D2Win.0x6F8B1AD0
void __fastcall D2Win_10204(D2WinTextBoxStrc* pTextBox, int a2);
//D2Win.0x6F8B1B20
int __fastcall D2Win_10203_TEXTBOX(D2WinTextBoxStrc* pTextBox);
