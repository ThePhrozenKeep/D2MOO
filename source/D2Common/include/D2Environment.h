#pragma once

#include "CommonDefinitions.h"


//D2Common.0x6FD8D8E0
D2DrlgEnvironmentStrc* __fastcall ENVIRONMENT_AllocDrlgEnvironment(void* pMemPool);
//D2Common.0x6FD8D970
void __fastcall ENVIRONMENT_UpdateLightIntensity(D2DrlgEnvironmentStrc* pEnvironment, int nLevelId, int nAct);
//D2Common.0x6FD8DAC0
void __fastcall ENVIRONMENT_UpdateLightColor(D2DrlgEnvironmentStrc* pEnvironment, int nAct);
//D2Common.6FD8DBE0
void __fastcall ENVIRONMENT_FreeDrlgEnvironment(void* pMemPool, D2DrlgEnvironmentStrc* pEnvironment);
//D2Common.0x6FD8DC00 (#10923)
BOOL __stdcall ENVIRONMENT_UpdatePeriodOfDay(D2DrlgActStrc* pAct, D2RoomStrc* pRoom);
//D2Common.0x6FD8DC70
void __fastcall ENVIRONMENT_UpdateTicks(D2DrlgEnvironmentStrc* pEnvironment, int nActNo);
//D2Common.0x6FD8DD60 (#10924)
BOOL __stdcall ENVIRONMENT_UpdateCycleIndex(D2DrlgActStrc* pAct, int nActNo);
//D2Common.0x6FD8DDD0 (#10927)
void __stdcall ENVIRONMENT_GetLightColorFromAct(D2DrlgActStrc* pAct, BYTE* pRed, BYTE* pGreen, BYTE* pBlue);
//D2Common.0x6FD8DE00 (#10926)
int __stdcall ENVIRONMENT_GetIntensityFromAct(D2DrlgActStrc* pAct);
//D2Common.0x6FD8DE20 (#10933)
int __stdcall ENVIRONMENT_GetPeriodOfDayFromAct(D2DrlgActStrc* pAct, int* pBaseTime);
//D2Common.0x6FD8DE70 (#10928)
int __stdcall ENVIRONMENT_GetUnusedMember(D2DrlgActStrc* pAct);
//D2Common.0x6FD8DE90 (#10929)
void __stdcall ENVIRONMENT_NextEnvCycle(D2DrlgActStrc* pAct, D2RoomStrc* pRoom);
//D2Common.0x6FD8DF30 (#10930)
int __stdcall ENVIRONMENT_GetCycleIndexFromAct(D2DrlgActStrc* pAct);
//D2Common.0x6FD8DF40 (#10932)
void __stdcall ENVIRONMENT_InitializeEnvironment(D2DrlgActStrc* pAct, D2RoomStrc* pRoom, int nIndex, int nAngle, BOOL bEclipse);
//D2Common.0x6FD8E080 (#10931)
void __stdcall ENVIRONMENT_GetCycleIndex_Ticks_EclipseFromAct(D2DrlgActStrc* pAct, int* pTimeIndex, int* pTicks, BOOL* pEclipse);
//D2Common.0x6FD8E0B0 (#10925)
void __stdcall ENVIRONMENT_GetStatistics(D2DrlgActStrc* pAct, float* pCos, float* pLast, float* pSin, int* a5);
//D2Common.0x6FD8E0F0 (#10934)
int __stdcall ENVIRONMENT_GetTimeRateFromAct(D2DrlgActStrc* pAct);
//D2Common.0x6FD8E100 (#10935)
void __stdcall ENVIRONMENT_SetNextTimeRate(D2DrlgActStrc* pAct, D2RoomStrc* pRoom);
//D2Common.0x6FD8E1B0 (#10936)
void __stdcall ENVIRONMENT_TaintedSunBegin(D2DrlgActStrc* pAct);
//D2Common.0x6FD8E1E0 (#10937)
void __stdcall ENVIRONMENT_TaintedSunEnd(D2DrlgActStrc* pAct);
