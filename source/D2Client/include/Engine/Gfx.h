#pragma once  
#include <D2BasicTypes.h>
#include <Units/Units.h>
#include <D2CMP.h>
#include <D2Gfx.h>

//1.10f:D2Client.0x6FB5A320
BOOL __fastcall CLIENT_DrawItem(D2UnitStrc* pItem, int nXpos, int nYpos);
//1.10f:D2Client.0x6FB5EC80
BOOL __fastcall CLIENT_GetItemGfxData(D2GfxDataStrc* pData, D2UnitStrc* pItem, D2CellFileStrc* pCellFile, int nDirection, int nFrame, DWORD* a6, int nComponent, int a8);
