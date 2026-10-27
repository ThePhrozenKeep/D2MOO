#pragma once

#include "D2WinControlHeader.h"

#include <D2Unicode.h>
#include "Font.h"


#pragma pack(push, 1)
struct D2WinListDataStrc
{
	Unicode wszText[256];							//0x00
	int32_t field_200;								//0x200
	int32_t(__stdcall* field_204)(SMSGHANDLER_PARAMS*);	//0x204
	int32_t field_208;								//0x208
	int32_t field_20C;								//0x20C
	int32_t field_210;								//0x210
	int32_t field_214;								//0x214
	char field_218;									//0x218
	char field_219;									//0x219
	char field_21A;									//0x21A
	char field_21B;									//0x21B
	int32_t field_21C;								//0x21C
	D2WinListDataStrc* pNext;						//0x220
};

struct D2WinListStrc
{
	D2WinControlStrc controlHeader;					//0x00
	Font eFont;										//0x40
	int32_t field_44;								//0x44
	int32_t field_48;								//0x48
	int32_t field_4C;								//0x4C
	int32_t field_50;								//0x50
	int32_t field_54;								//0x54
	int32_t field_58;								//0x58
	int32_t field_5C;								//0x5C
	int32_t field_60;								//0x60
	D2WinListDataStrc* pDataList;					//0x64
	D2WinListDataStrc* pSelectedDataEntry;			//0x68
};
#pragma pack(pop)


//D2Win.0x6F8AC270
//1.14d: 0x005075E0
D2WinListStrc* __fastcall LIST_Create(int nX, int nY, int nWidth, int nHeight, void* a5, Font* pFont);
//D2Win.0x6F8AC490
//1.14d: 0x00506E60
int __fastcall LIST_Destroy(D2WinListStrc* pList);
//D2Win.0x6F8AC4F0
//1.14d: 0x005077F0
void __fastcall D2Win_10138(D2WinListStrc* pList, const char* szText, int(__stdcall* a3)(SMSGHANDLER_PARAMS*), char a4, int a5, int a6, int a7);
//D2Win.0x6F8AC570
//1.14d: 0x00506EC0
void __fastcall D2Win_10137(D2WinListStrc* pList, Unicode* wszText, int(__stdcall* a3)(SMSGHANDLER_PARAMS*), char a4, int a5, int a6, int a7);
//D2Win.0x6F8AC6D0
//1.14d: 0x00507000
int __fastcall LIST_GetSelectedDataIndex(D2WinListStrc* pList);
//D2Win.0x6F8AC720
//1.14d: 0x00507050
D2WinListDataStrc* __fastcall LIST_GetDataFromIndex(D2WinListStrc* pList, int32_t nIndex);
//D2Win.0x6F8AC770
//1.14d: 0x005070A0
int __fastcall D2Win_10141(D2WinListStrc* pList);
//D2Win.0x6F8AC7A0
//1.14d: 0x005070D0
void __stdcall LIST_LoadPentspinCellfile();
//D2Win.0x6F8AC800
//1.14d: 0x00507140
void __stdcall LIST_UnloadPentspinCellfile();
//D2Win.0x6F8AC820
//1.14d: 0x00507160
int32_t __fastcall LIST_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8AC9B0
//1.14d: 0x005072F0
int __stdcall LIST_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ACA70
//1.14d: 0x005073B0
int __stdcall LIST_HandleCharInput(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8ACAE0
//1.14d: 0x00507420
int __stdcall LIST_HandleVirtualKeyInput(SMSGHANDLER_PARAMS* pMsg);
