#pragma once  
#include <Units/Units.h>
#include <D2Unicode.h>

//1.10f: D2Client.0x6FAF3930
//1.13c: D2Client.0x6FAF6150
D2UnitStrc* __fastcall UI_NPC_GetActiveNpcUnit();

//1.10f: D2Client.0x6FAFB200
//1.13c: D2Client.0x6FAF6230
BOOL __fastcall NPCMENU_TransactionCost(D2UnitStrc* pItem, BOOL bIsOwned, int* pValue, Unicode* pBuffer, int nSize);
