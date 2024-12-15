#pragma once
#include <D2BasicTypes.h>

void __cdecl LOG_11100(int a1, int a2, int a3, int a4, const char* szFile, int nLine, const char* szFormat, ...);
void __cdecl LOG_11101(int nGame, int nFrame, int nClient, int a4, int nSize, const char* szFormat, ...);
void __fastcall LOG_SetEnabled(BOOL bEnabled);
void __fastcall LOG_11103(void* a1);
void __fastcall LOG_11104(void* a1);
void __cdecl LOG_11105();
void __cdecl LOG_11106();
