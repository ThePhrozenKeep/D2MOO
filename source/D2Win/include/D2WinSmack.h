#pragma once

#include "D2WinControlHeader.h"


#pragma pack(push, 1)
struct D2WinSmackStrc
{
	D2WinControlStrc controlHeader;					//0x00
	char szFileName[256];							//0x40
	int32_t field_140;								//0x140
	int32_t field_144;								//0x144
	int32_t field_148;								//0x148
	int32_t field_14C;								//0x14C
};
#pragma pack(pop)


//D2Win.0x6F8AF710
D2WinSmackStrc* __fastcall SMACK_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t(__stdcall* a5)(D2WinMsgStrc*));
//D2Win.0x6F8AF7E0
int __stdcall SMACK_Decode(D2WinSmackStrc* pSmack, const char* szFileName, int nVersion, int nUnused, int nUnused2);
//D2Win.0x6F8AF860
int __stdcall SMACK_DeleteFile(D2WinSmackStrc* pSmack);
//D2Win.0x6F8AF8B0
int __stdcall SMACK_SetCoordinates(D2WinSmackStrc* pSmack, int nX, int nY);
//D2Win.0x6F8AF900
int __fastcall SMACK_Destroy(D2WinSmackStrc* pSmack);
//D2Win.0x6F8AF950
void __stdcall D2Win_10184();
//D2Win.0x6F8AF960
int32_t __fastcall SMACK_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8AFA40
int __stdcall SMACK_HandleMouseDown(D2WinMsgStrc* pMsg);
//D2Win.0x6F8AFA90
void __fastcall D2Win_10208(int a1);
