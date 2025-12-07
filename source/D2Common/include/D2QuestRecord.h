#pragma once

#include "D2CommonDefinitions.h"

#include <D2BitManip.h>

#define NUM_QUEST_WORDS 48

//D2Common.0x6FDAE800 (#11107)
D2COMMON_DLL_DECL BOOL __stdcall QUESTRECORD_GetQuestState(D2BitBufferStrc* pQuestRecord, int nQuest, int nState);
//D2Common.0x6FDAE850 (#11108)
D2COMMON_DLL_DECL void __stdcall QUESTRECORD_SetQuestState(D2BitBufferStrc* pQuestRecord, int nQuest, int nState);
//D2Common.0x6FDAE890 (#11109)
D2COMMON_DLL_DECL void __stdcall QUESTRECORD_ClearQuestState(D2BitBufferStrc* pQuestRecord, int nQuest, int nState);
//D2Common.0x6FDAE8D0 (#11110)
D2COMMON_DLL_DECL void __stdcall QUESTRECORD_ResetIntermediateStateFlags(D2BitBufferStrc* pQuestRecord, int nQuest);
//D2Common.0x6FDAE920 (#11111)
D2COMMON_DLL_DECL D2BitBufferStrc* __stdcall QUESTRECORD_AllocRecord(void* pMemPool);
//D2Common.0x6FDAE970 (#11112)
D2COMMON_DLL_DECL void __stdcall QUESTRECORD_FreeRecord(void* pMemPool, D2BitBufferStrc* pQuestRecord);
//D2Common.0x6FDAE9B0 (#11113)
D2COMMON_DLL_DECL void __stdcall QUESTRECORD_CopyBufferToRecord(D2BitBufferStrc* pQuestRecord, uint8_t* pBuffer, uint16_t nBufferSize, BOOL bResetStates);
//D2Common.0x6FDAEA40 (#11114)
D2COMMON_DLL_DECL void __stdcall QUESTRECORD_CopyRecordToBuffer(D2BitBufferStrc* pQuestRecord, uint8_t* pBuffer, uint16_t nBufferSize, int nUnused);
