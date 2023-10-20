#pragma once 

#include <D2Common.h>
#include <D2BasicTypes.h>

#pragma pack(1)

struct D2AnimDataRecordStrc
{
	static const int MAX_FRAME_FLAGS = 144;
	char szAnimDataName[8];						//0x00
	uint32_t dwFrames;							//0x08
	int32_t dwAnimSpeed;						//0x0C
	uint8_t pFrameFlags[MAX_FRAME_FLAGS];		//0x10
};

struct D2AnimDataBucketStrc
{
	int32_t nbEntries;
	D2AnimDataRecordStrc aEntries[]; // Dynamically sized
};

struct D2AnimDataTableStrc
{
	void* pBinaryData;							//0x00
	D2AnimDataBucketStrc* pHashTableBucket[256];//0x04
	D2AnimDataRecordStrc tDefaultRecord;		//0x404
};

struct D2UnitStrc;
struct D2InventoryStrc;

#pragma pack()

//D2Common.0x6FD473C0
D2AnimDataTableStrc* __fastcall DATATBLS_LoadAnimDataD2(void* pMemPool);
//D2Common.0x6FD47460
void __fastcall DATATBLS_UnloadAnimDataD2(D2AnimDataTableStrc* pAnimData);
//D2Common.0x6FD474A0
const D2AnimDataRecordStrc* __fastcall DATATBLS_GetAnimDataRecord(D2UnitStrc* pUnit, int nClassId, int nMode, int nUnitType, D2InventoryStrc* pInventory);
//D2Common.0x6FD475D0 (#10640)
D2COMMON_DLL_DECL void __stdcall DATATBLS_UnitAnimInfoDebugSet(D2UnitStrc* pUnit, int nAnimSpeed);
//D2Common.0x6FD47700 (#10641)
D2COMMON_DLL_DECL BOOL __stdcall DATATBLS_GetAnimDataInfo(char* szPath, int* pOutLength, int* pOutAnimSpeed, int* pOutFirstFrameTagged);


