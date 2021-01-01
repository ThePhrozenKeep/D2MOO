#pragma once 

#include <D2BasicTypes.h>

struct D2CollisionFieldStrc
{
	int32_t nWidth;							//0x00
	int32_t nHeight;						//0x04
	int32_t nArea;							//0x08
	int32_t nCenterX;						//0x0C
	int32_t nCenterY;						//0x10
};


//D2Common.0x6FD51FC0
BOOL __fastcall DATATBLS_LoadExpFieldD2(void* pMemPool);
//D2Common.0x6FD52010 (#11089)
BOOL __stdcall DATATBLS_InitializeCollisionFieldTable(char* pExpField, int nSize);
//D2Common.0x6FD520F0 (#11090)
BOOL __stdcall DATATBLS_FreeCollisionFieldTable();
//D2Common.0x6FD52140 (#11091)
void __stdcall DATATBLS_GetCollisionFieldCenter(int* a1, int* a2);
//D2Common.0x6FD52160 (#11092)
int __stdcall DATATBLS_GetCollisionFieldWidth();
//D2Common.0x6FD52170 (#11093)
int __stdcall DATATBLS_GetCollisionFieldHeight();
//D2Common.0x6FD52180 (#11094)
D2FieldStrc* __stdcall DATATBLS_AllocField();
//D2Common.0x6FD521D0 (#11095)
void __stdcall DATATBLS_FreeField(D2FieldStrc* pField);
//D2Common.0x6FD52210 (#11096)
void __stdcall DATATBLS_SetFieldCoordinates(D2FieldStrc* pField, int nX, int nY);
//D2Common.0x6FD52250 (#11097)
int __stdcall D2Common_11097(D2FieldStrc* pField, int a2, int a3);
//D2Common.0x6FD522A0 (#11098)
int __stdcall D2Common_11098(D2FieldStrc* pField, int* pX, int* pY);
//D2Common.0x6FD52360 (#11099)
BOOL __stdcall D2Common_11099(D2FieldStrc* pField, D2RoomStrc* pRoom, int nX, int nY, uint16_t fMask);

