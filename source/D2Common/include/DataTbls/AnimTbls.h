#pragma once 

#include <D2Common.h>
#include <D2BasicTypes.h>

#pragma pack(1)

struct D2AnimDataRecordStrc
{
	char szAnimDataName[8];					//0x00
	uint32_t dwFrames;							//0x08
	uint32_t dwAnimSpeed;						//0x0C
	uint8_t pFrameFlags[144];					//0x10
};

struct D2AnimDataStrc
{
	D2AnimDataRecordStrc* pRecords;			//0x00
	D2AnimDataRecordStrc* pHashTable[256];	//0x04
	D2AnimDataRecordStrc pDefaultRecord;	//0x404
};

struct D2UnitStrc;
struct D2InventoryStrc;

#pragma pack()

//D2Common.0x6FD473C0
D2AnimDataStrc* __fastcall DATATBLS_LoadAnimDataD2(void* pMemPool);
//D2Common.0x6FD47460
void __fastcall DATATBLS_UnloadAnimDataD2(D2AnimDataStrc* pAnimData);
//D2Common.0x6FD474A0
D2AnimDataRecordStrc* __fastcall DATATBLS_GetAnimDataRecord(D2UnitStrc* pUnit, int nClassId, int nMode, int nUnitType, D2InventoryStrc* pInventory);
//D2Common.0x6FD475D0 (#10640)
D2COMMON_DLL_DECL void __stdcall D2Common_10640(D2UnitStrc* pUnit, int nAnimSpeed);
//D2Common.0x6FD47700 (#10641)
D2COMMON_DLL_DECL BOOL __stdcall D2Common_10641(char* szPath, int* pLength, int* pOutAnimSpeed, int* pOutFirstFrameTagged);


