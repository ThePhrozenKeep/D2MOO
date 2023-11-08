#pragma once 

#include <D2BasicTypes.h>

struct D2ActiveRoomStrc;

#pragma pack(1)
struct D2CollisionFieldStrc
{
	int32_t nWidth;							//0x00
	int32_t nHeight;						//0x04
	int32_t nArea;							//0x08
	int32_t nCenterX;						//0x0C
	int32_t nCenterY;						//0x10
};

struct D2FieldStrc
{
	int32_t nX;								//0x00
	int32_t nY;								//0x04
};
#pragma pack()

//D2Common.0x6FD51FC0
BOOL __fastcall DATATBLS_LoadExpFieldD2(void* pMemPool);
//D2Common.0x6FD52010 (#11089)
D2COMMON_DLL_DECL BOOL __stdcall DATATBLS_InitializeCollisionFieldTable(char* pExpField, int nSize);
//D2Common.0x6FD520F0 (#11090)
D2COMMON_DLL_DECL BOOL __stdcall DATATBLS_FreeCollisionFieldTable();
//D2Common.0x6FD52140 (#11091)
D2COMMON_DLL_DECL void __stdcall DATATBLS_GetCollisionFieldCenter(int* pCenterX, int* pCenterY);
//D2Common.0x6FD52160 (#11092)
D2COMMON_DLL_DECL int __stdcall DATATBLS_GetCollisionFieldWidth();
//D2Common.0x6FD52170 (#11093)
D2COMMON_DLL_DECL int __stdcall DATATBLS_GetCollisionFieldHeight();
//D2Common.0x6FD52180 (#11094)
D2COMMON_DLL_DECL D2FieldStrc* __stdcall DATATBLS_AllocField();
//D2Common.0x6FD521D0 (#11095)
D2COMMON_DLL_DECL void __stdcall DATATBLS_FreeField(D2FieldStrc* pField);
//D2Common.0x6FD52210 (#11096)
D2COMMON_DLL_DECL void __stdcall DATATBLS_SetFieldCoordinates(D2FieldStrc* pField, int nX, int nY);
//D2Common.0x6FD52250 (#11097)
D2COMMON_DLL_DECL int __stdcall D2Common_11097(D2FieldStrc* pField, int nX, int nY);
//D2Common.0x6FD522A0 (#11098)
D2COMMON_DLL_DECL int __stdcall D2Common_11098(D2FieldStrc* pField, int* pX, int* pY);
//D2Common.0x6FD52360 (#11099)
D2COMMON_DLL_DECL BOOL __stdcall D2Common_11099(D2FieldStrc* pField, D2ActiveRoomStrc* pRoom, int nX, int nY, uint16_t fMask);

