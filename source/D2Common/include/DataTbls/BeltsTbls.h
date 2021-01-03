#pragma once 

#include <D2BasicTypes.h>
#include <D2Inventory.h>

#pragma pack(1)

struct D2BeltsTxt
{
	uint8_t nIndex;						//0x00
	uint8_t pad0x01[3];					//0x01
	int32_t nBoxes;						//0x04
	D2InvRectStrc beltBoxes[16];		//0x08
};

#pragma pack()

//D2Common.0x6FD48880
void __fastcall DATATBLS_LoadBeltsTxt(void* pMemPool);
//D2Common.0x6FD493A0
void __fastcall DATATBLS_UnloadBeltsTxt();
//D2Common.0x6FD493B0 (#10638)
void __stdcall DATATBLS_GetBeltsTxtRecord(int nIndex, int bHigherRes, D2BeltsTxt* pRecord);
//D2Common.0x6FD49420 (#10639)
void __stdcall DATATBLS_GetInvRectFromBeltsTxt(int nIndex, int bHigherRes, D2InvRectStrc* pInvRect, int nBoxId);

