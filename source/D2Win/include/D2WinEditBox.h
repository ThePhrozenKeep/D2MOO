#pragma once

#include "D2WinControlHeader.h"

#include <D2Unicode.h>
#include "Font.h"


enum D2EditBoxFlags
{
	EDITBOX_PASSWORD = 0x00000001,
};


#pragma pack(push, 1)
struct D2WinEditBoxStrc
{
	D2WinControlStrc controlHeader;					//0x00
	int32_t field_40;								//0x40
	int32_t field_44;								//0x44	
	int32_t field_48;								//0x48
	int32_t field_4C;								//0x4C
	int32_t field_50;								//0x50
	int32_t field_54;								//0x54
	int32_t nTextLength;							//0x58
	Unicode wszText[256];							//0x258
	int32_t field_25C;								//0x25C
	uint32_t dwEditBoxFlags;						//0x260
	int32_t field_264;								//0x264
	int32_t field_268;								//0x268
	void(__stdcall* field_26C)(int32_t);			//0x26C
	Font eFont;										//0x270
	int32_t nTextColor;								//0x274
	D2WinEditBoxStrc* pNext;						//0x278
	D2WinEditBoxStrc* pPrevious;					//0x27C
	int32_t field_280;								//0x280
};
#pragma pack(pop)


//D2Win.0x6F8A6C80
D2WinEditBoxStrc* __fastcall D2Win_10070_EDITBOX_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t a5, int32_t a6, D2CellFileStrc* pCellFile, int32_t a8, int32_t(__stdcall* a9)(SMSGHANDLER_PARAMS*), int32_t nEditBoxFlags, int32_t a11);
//D2Win.0x6F8A6DC0
int __fastcall D2Win_10071_EDITBOX_Destroy(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8A6E10
void __fastcall D2Win_10072(D2WinEditBoxStrc* pEditBox, int32_t a2);
//D2Win.0x6F8A6E60
void __fastcall D2Win_10073(D2WinEditBoxStrc* pEditBox, void(__stdcall* a2)(int32_t));
//D2Win.0x6F8A6EB0
void __fastcall D2Win_10083(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8A6F10
int __fastcall D2Win_10082(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8A6F70
int32_t __fastcall D2Win_10074(D2WinEditBoxStrc* pEditBox, uint8_t a2);
//D2Win.0x6F8A7970
void __fastcall sub_6F8A7970(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8A7AB0
int __fastcall sub_6F8A7AB0(D2WinEditBoxStrc* pEditBox, char a2);
//D2Win.0x6F8A80A0
void __fastcall D2Win_10081(D2WinEditBoxStrc* pEditBox, int32_t a2);
//D2Win.0x6F8A80F0
int __fastcall D2Win_10076(D2WinEditBoxStrc* pEditBox, const char* szText);
//D2Win.0x6F8A8140
int __fastcall D2Win_10075(D2WinEditBoxStrc* pEditBox, const Unicode* pText);
//D2Win.0x6F8A81E0
const Unicode* __fastcall D2Win_10077_EDITBOX_GetText(D2WinEditBoxStrc* pEditBox);
//D2Win.0x6F8A8230
void __fastcall D2Win_10080_EDITBOX_SetNextEditBox(D2WinEditBoxStrc* pEditBox, D2WinEditBoxStrc* pNext);
//D2Win.0x6F8A82D0
int32_t __fastcall sub_6F8A82D0(D2WinControlStrc* pControl);
//D2Win.0x6F8A86C0
int32_t __fastcall sub_6F8A86C0(D2WinControlStrc* pControl);
//D2Win.0x6F8A8D60
void __fastcall sub_6F8A8D60(D2WinEditBoxStrc* pEditBox, int32_t* a2, int32_t* a3);
//D2Win.0x6F8A9120
int __stdcall sub_6F8A9120(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A93A0
int __stdcall EDITBOX_HandleMouseUp(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A9410
int32_t __fastcall sub_6F8A9410(SMSGHANDLER_PARAMS* pMsg, int32_t nX, int32_t nY);
//D2Win.0x6F8A97D0
int __stdcall sub_6F8A97D0(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8A9850
bool __stdcall D2Win_10078_IsNumber(int32_t nUnused1, int32_t nUnused2, char* szChar);
//D2Win.0x6F8A9870
bool __stdcall D2Win_10079_IsLetter(int nUnused1, int nUnused2, char* szChar);
//D2Win.0x6F8A98A0
void __fastcall sub_6F8A98A0(D2WinEditBoxStrc* pEditBox);
