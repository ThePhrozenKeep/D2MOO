#pragma once

#include "CommonDefinitions.h"
#include "Path.h"

#pragma pack(1)

enum class PATH_IDASTAR_CONSTANTS
{
	MAXPROOM = 50000, // Max size of a room for IDAStar (ptRoomCoords->nSizeGameX * ptRoomCoords->nSizeGameY <= MAXPROOM)
};

struct D2PathPointsListStrc
{
    D2PathPointStrc point;
    DWORD unk0x4;
    DWORD unk0x8;
    D2PathPointsListStrc* pNextEntry;
};

#pragma pack()

//D2Common.0x6FDA69E0
int __fastcall sub_6FDA69E0(D2PathInfoStrc* pPathInfo);
////D2Common.0x6FDA6D10
//int __fastcall sub_6FDA6D10(int a1, int a2);
////D2Common.0x6FDA6D50
//signed int __fastcall sub_6FDA6D50(int a1, int a2, int a3, int a4);
////D2Common.0x6FDA7230
//int __stdcall sub_6FDA7230(int a1, int a2);
////D2Common.0x6FDA7280
//int __fastcall sub_6FDA7280(void* a1, int a2);
////D2Common.0x6FDA72D0
//int __fastcall sub_6FDA72D0(void* a1, int a2);
////D2Common.0x6FDA7320
//signed int __fastcall sub_6FDA7320(int a1, int a2);
////D2Common.0x6FDA7390
//int __fastcall sub_6FDA7390(int a1, int a2);
////D2Common.0x6FDA7450
//int __fastcall sub_6FDA7450(int a1);
////D2Common.0x6FDA7490
//signed int __userpurge sub_6FDA7490<eax>(int a1<edx>, int a2<edi>, int a3, int a4, int a5);
//D2Common.0x6FDA78A0
signed int __fastcall PATH_SimplifyToLines_6FDA78A0(D2PathPointsListStrc* pCurPoint, D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDA7970
int __fastcall sub_6FDA7970(D2PathInfoStrc* pPathInfo);
////D2Common.0x6FDA7D40
//int __fastcall sub_6FDA7D40(int a1, signed int a2, int a3);
////D2Common.0x6FDA81C0
//int __fastcall sub_6FDA81C0(int a1, int a2);
