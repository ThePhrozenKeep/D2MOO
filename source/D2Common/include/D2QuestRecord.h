#pragma once

#include "CommonDefinitions.h"

#define NUM_QUEST_WORDS 48

//D2Common.0x6FDAE800 (#11107)
BOOL __stdcall QUESTRECORD_GetQuestState(D2BitBufferStrc* pQuestRecord, int nQuest, int nState);
//D2Common.0x6FDAE850 (#11108)
void __stdcall QUESTRECORD_SetQuestState(D2BitBufferStrc* pQuestRecord, int nQuest, int nState);
//D2Common.0x6FDAE890 (#11109)
void __stdcall QUESTRECORD_ClearQuestState(D2BitBufferStrc* pQuestRecord, int nQuest, int nState);
//D2Common.0x6FDAE8D0 (#11110)
void __stdcall QUESTRECORD_ResetIntermediateStateFlags(D2BitBufferStrc* pQuestRecord, int nQuest);
//D2Common.0x6FDAE920 (#11111)
D2BitBufferStrc* __stdcall QUESTRECORD_AllocRecord(void* pMemPool);
//D2Common.0x6FDAE970 (#11112)
void __stdcall QUESTRECORD_FreeRecord(void* pMemPool, D2BitBufferStrc* pQuestRecord);
//D2Common.0x6FDAE9B0 (#11113)
void __stdcall QUESTRECORD_CopyBufferToRecord(D2BitBufferStrc* pQuestRecord, BYTE* pBuffer, WORD nBufferSize, BOOL bResetStates);
//D2Common.0x6FDAEA40 (#11114)
void __stdcall QUESTRECORD_CopyRecordToBuffer(D2BitBufferStrc* pQuestRecord, BYTE* pBuffer, WORD nBufferSize, int nUnused);
