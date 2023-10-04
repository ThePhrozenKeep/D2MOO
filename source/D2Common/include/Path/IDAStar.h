#pragma once

#include "CommonDefinitions.h"
#include "Path.h"

#pragma pack(1)

enum PATH_IDASTAR_CONSTANTS
{
	IDASTAR_MAXPROOM = 50000, // Max size of a room for IDAStar (ptRoomCoords->nSizeGameX * ptRoomCoords->nSizeGameY <= MAXPROOM)
};

#pragma pack()

//1.10f: D2Common.0x6FDA7970
//1.13c: D2Common.0x6FDC0E40
int __fastcall PATH_IdaStar_6FDA7970(D2PathInfoStrc* pPathInfo);

////D2Common.0x6FDA7D40
//int __fastcall sub_6FDA7D40(int a1, signed int a2, int a3);
////D2Common.0x6FDA81C0
//int __fastcall sub_6FDA81C0(int a1, int a2);
