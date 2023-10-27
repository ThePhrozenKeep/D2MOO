#pragma once

#include "D2WinControlHeader.h"

#include "Font.h"


#pragma pack(push, 1)
struct D2WinListDataStrc
{
	wchar_t wszText[256];							//0x00
	int32_t field_200;								//0x200
	int32_t(__stdcall* field_204)(D2WinMsgStrc*);	//0x204
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
D2WinListStrc* __fastcall LIST_Create(int nX, int nY, int nWidth, int nHeight, void* a5, Font* pFont);
//D2Win.0x6F8AC490
int __fastcall LIST_Destroy(D2WinListStrc* pList);
//D2Win.0x6F8AC4F0
void __fastcall D2Win_10138(D2WinListStrc* pList, const char* szText, int(__stdcall* a3)(D2WinMsgStrc*), char a4, int a5, int a6, int a7);
//D2Win.0x6F8AC570
void __fastcall D2Win_10137(D2WinListStrc* pList, wchar_t* wszText, int(__stdcall* a3)(D2WinMsgStrc*), char a4, int a5, int a6, int a7);
//D2Win.0x6F8AC6D0
int __fastcall LIST_GetSelectedDataIndex(D2WinListStrc* pList);
//D2Win.0x6F8AC720
D2WinListDataStrc* __fastcall LIST_GetDataFromIndex(D2WinListStrc* pList, int a2);
//D2Win.0x6F8AC770
int __fastcall D2Win_10141(D2WinListStrc* pList);
//D2Win.0x6F8AC7A0
void __stdcall LIST_LoadPentspinCellfile();
//D2Win.0x6F8AC800
void __stdcall LIST_UnloadPentspinCellfile();
//D2Win.0x6F8AC820
int32_t __fastcall LIST_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8AC9B0
int __stdcall LIST_HandleMouseDown(D2WinMsgStrc* pMsg);
//D2Win.0x6F8ACA70
int __stdcall LIST_HandleCharInput(D2WinMsgStrc* pMsg);
//D2Win.0x6F8ACAE0
int __stdcall LIST_HandleVirtualKeyInput(D2WinMsgStrc* pMsg);
