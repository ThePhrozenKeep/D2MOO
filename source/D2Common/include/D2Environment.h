#pragma once

#include "CommonDefinitions.h"

#pragma pack(1)

enum D2EnvironmentDayCycles : int32_t {
	EnvCycleSunrise,
	EnvCycleMorning,
	EnvCycleNoon,
	EnvCycleAfteroon,
	EnvCycleSunset,
	EnvCycleNight,
	NUM_ENVIRONMENT_CYCLES
};

struct D2EnvironmentCycleStrc
{
	int32_t nTicksBegin;			//0x00
	int32_t nPeriodOfDay;				//0x04
	uint8_t nRed;					//0x08
	uint8_t nGreen;				//0x09
	uint8_t nBlue;					//0x0A
	uint8_t nIntensity;			//0x0B Unused
};

struct D2DrlgEnvironmentStrc
{
	int32_t nCycleIndex;					//0x00
	int32_t nPeriodOfDay;					//0x04
	int32_t nTicks;							//0x08
	int32_t nIntensity;						//0x0C
	uint32_t dwInitTick;					//0x10
	int32_t unk0x14;						//0x14
	uint8_t nRed;							//0x18
	uint8_t nGreen;							//0x19
	uint8_t nBlue;							//0x1A
	uint8_t pad0x1B;						//0x1B
	float fCos;								//0x1C
	float fLast;							//0x20
	float fSin;								//0x24
	int32_t nTimeRate;						//0x28
	int32_t nTimeRateIndex;					//0x2C
	BOOL bEclipse;							//0x30
	int32_t nPrev;							//0x34
};

#pragma pack()

//D2Common.0x6FD8D8E0
D2DrlgEnvironmentStrc* __fastcall ENVIRONMENT_AllocDrlgEnvironment(void* pMemPool);
//D2Common.0x6FD8D970
void __fastcall ENVIRONMENT_UpdateLightIntensity(D2DrlgEnvironmentStrc* pEnvironment, int nLevelId, int nAct);
//D2Common.0x6FD8DAC0
void __fastcall ENVIRONMENT_UpdateLightColor(D2DrlgEnvironmentStrc* pEnvironment, int nAct);
//D2Common.6FD8DBE0
void __fastcall ENVIRONMENT_FreeDrlgEnvironment(void* pMemPool, D2DrlgEnvironmentStrc* pEnvironment);
//D2Common.0x6FD8DC00 (#10923)
D2COMMON_DLL_DECL BOOL __stdcall ENVIRONMENT_UpdatePeriodOfDay(D2DrlgActStrc* pAct, D2RoomStrc* pRoom);
//D2Common.0x6FD8DC70
void __fastcall ENVIRONMENT_UpdateTicks(D2DrlgEnvironmentStrc* pEnvironment, int nActNo);
//D2Common.0x6FD8DD60 (#10924)
D2COMMON_DLL_DECL BOOL __stdcall ENVIRONMENT_UpdateCycleIndex(D2DrlgActStrc* pAct, int nActNo);
//D2Common.0x6FD8DDD0 (#10927)
D2COMMON_DLL_DECL void __stdcall ENVIRONMENT_GetLightColorFromAct(D2DrlgActStrc* pAct, uint8_t* pRed, uint8_t* pGreen, uint8_t* pBlue);
//D2Common.0x6FD8DE00 (#10926)
D2COMMON_DLL_DECL int __stdcall ENVIRONMENT_GetIntensityFromAct(D2DrlgActStrc* pAct);
//D2Common.0x6FD8DE20 (#10933)
D2COMMON_DLL_DECL int __stdcall ENVIRONMENT_GetPeriodOfDayFromAct(D2DrlgActStrc* pAct, int* pBaseTime);
//D2Common.0x6FD8DE70 (#10928)
D2COMMON_DLL_DECL int __stdcall ENVIRONMENT_GetUnusedMember(D2DrlgActStrc* pAct);
//D2Common.0x6FD8DE90 (#10929)
D2COMMON_DLL_DECL void __stdcall ENVIRONMENT_NextEnvCycle(D2DrlgActStrc* pAct, D2RoomStrc* pRoom);
//D2Common.0x6FD8DF30 (#10930)
D2COMMON_DLL_DECL int __stdcall ENVIRONMENT_GetCycleIndexFromAct(D2DrlgActStrc* pAct);
//D2Common.0x6FD8DF40 (#10932)
D2COMMON_DLL_DECL void __stdcall ENVIRONMENT_InitializeEnvironment(D2DrlgActStrc* pAct, D2RoomStrc* pRoom, int nIndex, int nAngle, BOOL bEclipse);
//D2Common.0x6FD8E080 (#10931)
D2COMMON_DLL_DECL void __stdcall ENVIRONMENT_GetCycleIndex_Ticks_EclipseFromAct(D2DrlgActStrc* pAct, int* pTimeIndex, int* pTicks, BOOL* pEclipse);
//D2Common.0x6FD8E0B0 (#10925)
D2COMMON_DLL_DECL void __stdcall ENVIRONMENT_GetStatistics(D2DrlgActStrc* pAct, float* pCos, float* pLast, float* pSin, int* a5);
//D2Common.0x6FD8E0F0 (#10934)
D2COMMON_DLL_DECL int __stdcall ENVIRONMENT_GetTimeRateFromAct(D2DrlgActStrc* pAct);
//D2Common.0x6FD8E100 (#10935)
D2COMMON_DLL_DECL void __stdcall ENVIRONMENT_SetNextTimeRate(D2DrlgActStrc* pAct, D2RoomStrc* pRoom);
//D2Common.0x6FD8E1B0 (#10936)
D2COMMON_DLL_DECL void __stdcall ENVIRONMENT_TaintedSunBegin(D2DrlgActStrc* pAct);
//D2Common.0x6FD8E1E0 (#10937)
D2COMMON_DLL_DECL void __stdcall ENVIRONMENT_TaintedSunEnd(D2DrlgActStrc* pAct);
