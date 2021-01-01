#pragma once 

#include <D2BasicTypes.h>

struct D2MonSeqTxt
{
	uint16_t wSequence;					//0x00
	uint8_t nMode;						//0x02
	uint8_t nFrame;						//0x03
	uint8_t nDir;						//0x04
	uint8_t nEvent;						//0x05
};

struct D2SeqRecordStrc
{
	D2MonSeqTxt* pMonSeqTxtRecord;		//0x00
	int32_t unk0x04;					//0x04
	int32_t unk0x08;					//0x08
};

//TODO: Find a better name
struct D2UnkSequenceStrc
{
	D2SeqRecordStrc unk0x00[14];		//0x00
};

//D2Common.0x6FD727A0 (#10682)
D2MonSeqTxt* __stdcall DATATBLS_GetMonSeqTxtRecordFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FD727C0
D2SeqRecordStrc* __fastcall DATATBLS_GetSeqRecordFromUnit(D2UnitStrc* pUnit);
//D2Common.0x6FD728A0 (#10683)
int __stdcall D2Common_10683(D2UnitStrc* pUnit);
//D2Common.0x6FD728C0 (#10684)
int __stdcall D2Common_10684(D2UnitStrc* pUnit);
//D2Common.0x6FD728E0 (#10685)
void __stdcall D2Common_10685(D2MonSeqTxt* pMonSeqTxt, int a2, int a3, unsigned int* pMode, unsigned int* pFrame, int* pDirection, int* pEvent);
//D2Common.0x6FD72990 (#10686)
void __stdcall DATATBLS_GetSequenceEvent(D2MonSeqTxt* pMonSeqTxt, int a2, int* pEvent);
//D2Common.0x6FD6F050
void __fastcall DATATBLS_LoadMonSeqTxt(void* pMemPool);
//D2Common.0x6FD6F200 (#11262)
D2SeqRecordStrc* __stdcall DATATBLS_GetMonSeqTableRecord(int nSequence);
