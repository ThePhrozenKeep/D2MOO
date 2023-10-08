#include <cmath>
#include "D2Environment.h"

#include "Drlg/D2DrlgDrlg.h"
#include "D2Dungeon.h"
#include "D2Math.h"
#include <DataTbls/LevelsIds.h>

#define M_PI 3.14159265358979323846
#define NUM_TIME_RATES 3

//D2Common.0x6FDE21B8
const D2EnvironmentCycleStrc gNormalEnvironmentCycle[NUM_ENVIRONMENT_CYCLES] =
{
	{ 320, ENVPERIOD_DAWN,  125, 144, 243, 0 }, // ENVCYCLE_SUNRISE
	{ 340, ENVPERIOD_DAWN,  208, 184, 131, 0 }, // ENVCYCLE_MORNING
	{   0, ENVPERIOD_DAY,   255, 255, 255, 0 }, // ENVCYCLE_NOON
	{ 160, ENVPERIOD_DUSK,  255, 255, 255, 0 }, // ENVCYCLE_AFTERNOON
	{ 180, ENVPERIOD_DUSK,  194, 152, 193, 0 }, // ENVCYCLE_SUNSET
	{ 200, ENVPERIOD_NIGHT, 125, 144, 243, 0 }  // ENVCYCLE_NIGHT
};

//D2Common.0x6FDE2200
const D2EnvironmentCycleStrc gAct4EnvironmentCycle[NUM_ENVIRONMENT_CYCLES] =
{
	{ 340, ENVPERIOD_DAWN,  243,  70, 243, 0 }, // ENVCYCLE_SUNRISE
	{ 350, ENVPERIOD_DAWN,  208, 184, 131, 0 }, // ENVCYCLE_MORNING
	{   0, ENVPERIOD_DAY,   255,  20,  20, 0 }, // ENVCYCLE_NOON
	{ 180, ENVPERIOD_DUSK,  255, 255,  30, 0 }, // ENVCYCLE_AFTERNOON
	{ 190, ENVPERIOD_DUSK,   20, 152, 193, 0 }, // ENVCYCLE_SUNSET
	{ 200, ENVPERIOD_NIGHT, 125, 144, 243, 0 }  // ENVCYCLE_NIGHT
};

//D2Common.0x6FDE2248
//The tainted sun quest eclipse
const D2EnvironmentCycleStrc gEclipseEnvironmentCycle[NUM_ENVIRONMENT_CYCLES] =
{
	{ 300, ENVPERIOD_DAWN,  0, 30, 243, 0 }, // ENVCYCLE_SUNRISE
	{   0, ENVPERIOD_DAY,   0, 30, 244, 0 }, // ENVCYCLE_MORNING
	{  60, ENVPERIOD_DUSK,  0, 30, 243, 0 }, // ENVCYCLE_NOON
	{ 120, ENVPERIOD_NIGHT, 0, 30, 243, 0 }, // ENVCYCLE_AFTERNOON
	{ 180, ENVPERIOD_NIGHT, 0, 30, 244, 0 }, // ENVCYCLE_SUNSET
	{ 240, ENVPERIOD_NIGHT, 0, 30, 243, 0 }	 // ENVCYCLE_NIGHT
};

//D2Common.0x6FDE2290
const int gnTimeRates[4] =
{
	128, 4, 8, 0
};

static const D2EnvironmentCycleStrc* ENVIRONMENT_GetCycle(int nCycle, int nAct, bool bEclipse)
{
	// Lectem's note: Checks are inconsitents between functions
	// In most, the act check has priority, but sometimes the eclipse one has it.
	// Eclipse seems to be only be triggered by the tainted sun quest, make sure it's the case.
	D2_ASSERT(!(bEclipse && nAct == ACT_IV));
	if (nAct == ACT_IV)
	{
		return &gAct4EnvironmentCycle[nCycle];
	}
	else if (bEclipse)
	{
		return &gEclipseEnvironmentCycle[nCycle];
	}
	else
	{
		return &gNormalEnvironmentCycle[nCycle];
	}
}


//D2Common.0x6FD8D8E0
D2DrlgEnvironmentStrc* __fastcall ENVIRONMENT_AllocDrlgEnvironment(void* pMemPool)
{
	D2DrlgEnvironmentStrc* pEnvironment = D2_CALLOC_STRC_POOL(pMemPool, D2DrlgEnvironmentStrc);
	
	pEnvironment->nCycleIndex = ENVCYCLE_NOON;
	pEnvironment->nTimeRate = gnTimeRates[pEnvironment->nTimeRateIndex];

	const D2EnvironmentCycleStrc* pEnvironmentCycle = &gNormalEnvironmentCycle[pEnvironment->nCycleIndex];

	pEnvironment->nPeriodOfDay = pEnvironmentCycle->nPeriodOfDay;
	pEnvironment->nTicks = pEnvironmentCycle->nTicksBegin * pEnvironment->nTimeRate;

	ENVIRONMENT_UpdateLightIntensity(pEnvironment, 0, 0);
	ENVIRONMENT_UpdateLightColor(pEnvironment, 0);

	pEnvironment->unk0x14 = 0;
	pEnvironment->dwInitTick = GetTickCount();
	pEnvironment->bEclipse = FALSE;

	return pEnvironment;
}

//D2Common.0x6FD8D970
void __fastcall ENVIRONMENT_UpdateLightIntensity(D2DrlgEnvironmentStrc* pEnvironment, int nLevelId, int nAct)
{
	int nTargetIntensity = 0;

	if (nAct == ACT_IV)
	{
		switch (nLevelId)
		{
		case LEVEL_THEPANDEMONIUMFORTRESS:
			nTargetIntensity = 128;
			break;

		case LEVEL_OUTERSTEPPES:
			nTargetIntensity = 64;
			break;

		case LEVEL_PLAINSOFDESPAIR:
			nTargetIntensity = 56;
			break;

		case LEVEL_CITYOFTHEDAMNED:
			nTargetIntensity = 48;
			break;

		default:
			nTargetIntensity = 16;
			break;
		}
		// "lerp" towards nTargetIntensity
		if (pEnvironment->nIntensity < nTargetIntensity)
		{
			++pEnvironment->nIntensity;
		}

		if (pEnvironment->nIntensity > nTargetIntensity)
		{
			--pEnvironment->nIntensity;
		}

		if (!pEnvironment->nIntensity)
		{
			pEnvironment->nIntensity = nTargetIntensity;
		}
	}
	else if (pEnvironment->bEclipse)
	{
		// Quickly "lerp" towards eclipse light intensity
		const int nEclipseLightIntensity = 32;
		if (pEnvironment->nIntensity > nEclipseLightIntensity)
		{
			pEnvironment->nIntensity -= 8;
		}

		if (pEnvironment->nIntensity < nEclipseLightIntensity)
		{
			pEnvironment->nIntensity = nEclipseLightIntensity;
		}
	}
	else if (nLevelId == LEVEL_ROCKYSUMMIT)
	{
		pEnvironment->nIntensity = 200;
	}
	else
	{
		double dAngle = (double)pEnvironment->nTicks / (double)pEnvironment->nTimeRate * M_PI / ENV_HALF_CIRCLE_DEGREES;

		pEnvironment->fCos = float(-cos(dAngle));
		pEnvironment->fSin = float(sin(dAngle));
		if (pEnvironment->nTicks >= ENV_HALF_CIRCLE_DEGREES * pEnvironment->nTimeRate)
		{
			pEnvironment->fSin *= 0.5;
		}

		pEnvironment->fLast = 0.0f;
		pEnvironment->nIntensity = (int32_t)(int64_t)((pEnvironment->fSin * 128.0) + 128.0 + 0.5); // NOLINT(bugprone-incorrect-roundings)

		if (nAct == ACT_V)
		{
			nTargetIntensity = 170;
		}
		else
		{
			nTargetIntensity = 255;
		}

		pEnvironment->nIntensity = D2Clamp(pEnvironment->nIntensity, 0, nTargetIntensity);
	}
}




static uint8_t LerpLightColor(uint8_t nThis, uint8_t nNext, double lerpRatio)
{
	return uint8_t(D2Lerp<double>(nThis,nNext,lerpRatio) + 0.5); // Round half up // NOLINT(bugprone-incorrect-roundings)
}

//D2Common.0x6FD8DAC0
void __fastcall ENVIRONMENT_UpdateLightColor(D2DrlgEnvironmentStrc* pEnvironment, int nAct)
{
	const int nCycleIndex = pEnvironment->nCycleIndex;
	const int nNextCycleIndex = (nCycleIndex + 1) % NUM_ENVIRONMENT_CYCLES;
	const D2EnvironmentCycleStrc* pCurrEnvironmentCycle = ENVIRONMENT_GetCycle(nCycleIndex, nAct, pEnvironment->bEclipse);
	const D2EnvironmentCycleStrc* pNextEnvironmentCycle = ENVIRONMENT_GetCycle(nNextCycleIndex, nAct, pEnvironment->bEclipse);
	
	const double dLerpRatio = (double)(pEnvironment->nTicks - pEnvironment->nTimeRate * pCurrEnvironmentCycle->nTicksBegin) / (double)(pEnvironment->nTimeRate * (pNextEnvironmentCycle->nTicksBegin - pCurrEnvironmentCycle->nTicksBegin));

	pEnvironment->nRed   = LerpLightColor(pCurrEnvironmentCycle->nRed,   pNextEnvironmentCycle->nRed,   dLerpRatio);
	pEnvironment->nGreen = LerpLightColor(pCurrEnvironmentCycle->nGreen, pNextEnvironmentCycle->nGreen, dLerpRatio);
	pEnvironment->nBlue  = LerpLightColor(pCurrEnvironmentCycle->nBlue,  pNextEnvironmentCycle->nBlue,  dLerpRatio);
}

//D2Common.6FD8DBE0
void __fastcall ENVIRONMENT_FreeDrlgEnvironment(void* pMemPool, D2DrlgEnvironmentStrc* pEnvironment)
{
	D2_FREE_POOL(pMemPool, pEnvironment);
}

//D2Common.0x6FD8DC00 (#10923)
BOOL __stdcall ENVIRONMENT_UpdatePeriodOfDay(D2DrlgActStrc* pAct, D2RoomStrc* pRoom)
{
	D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct);

	const int nPreviousPeriodOfDay = pEnvironment->nPeriodOfDay;
	const int nLevelId = pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom): 0;
	const int nActNo = DRLG_GetActNoFromLevelId(nLevelId);

	ENVIRONMENT_UpdateTicks(pEnvironment, nActNo);

	ENVIRONMENT_UpdateLightIntensity(pEnvironment, nLevelId, nActNo);
	ENVIRONMENT_UpdateLightColor(pEnvironment, nActNo);

	if (nLevelId == LEVEL_ROCKYSUMMIT)
	{
		pEnvironment->nRed = 245;
		pEnvironment->nGreen = 240;
		pEnvironment->nBlue = 255;
	}

	return nPreviousPeriodOfDay != pEnvironment->nPeriodOfDay;
}

//D2Common.0x6FD8DC70
void __fastcall ENVIRONMENT_UpdateTicks(D2DrlgEnvironmentStrc* pEnvironment, int nActNo)
{
	
	++pEnvironment->nTicks;

	if (!pEnvironment->bEclipse)
	{
		if (nActNo == ACT_IV)
		{
			pEnvironment->nTicks += 15;
		}
		else if (gNormalEnvironmentCycle[pEnvironment->nCycleIndex].nPeriodOfDay == ENVPERIOD_NIGHT)
		{
			++pEnvironment->nTicks;

			if (nActNo == ACT_III)
			{
				pEnvironment->nTicks += 9;
			}
		}
	}

	// Should probably be pEnvironment->nTicks %= 360 * pEnvironment->nTimeRate;
	if (pEnvironment->nTicks >= ENV_FULL_CIRCLE_DEGREES * pEnvironment->nTimeRate)
	{
		pEnvironment->nTicks = 0;
	}

	const int nNextEnvCycle = (pEnvironment->nCycleIndex + 1) % NUM_ENVIRONMENT_CYCLES;
	const D2EnvironmentCycleStrc* pNextEnvCycle = ENVIRONMENT_GetCycle(nNextEnvCycle, nActNo, pEnvironment->bEclipse);

	if (pEnvironment->nTicks > pEnvironment->nTimeRate * pNextEnvCycle->nTicksBegin)
	{
		pEnvironment->nCycleIndex = nNextEnvCycle;

		// Lectem's note: Missing act 4...
		// Should be using pNextEnvCycle
		if (pEnvironment->bEclipse)
		{
			pEnvironment->nPeriodOfDay = gEclipseEnvironmentCycle[pEnvironment->nCycleIndex].nPeriodOfDay;
			pEnvironment->nTicks = pEnvironment->nTimeRate * gEclipseEnvironmentCycle[pEnvironment->nCycleIndex].nTicksBegin;
		}
		else
		{
			pEnvironment->nPeriodOfDay = gNormalEnvironmentCycle[pEnvironment->nCycleIndex].nPeriodOfDay;
			pEnvironment->nTicks = pEnvironment->nTimeRate * gNormalEnvironmentCycle[pEnvironment->nCycleIndex].nTicksBegin;
		}
	}
}

//D2Common.0x6FD8DD60 (#10924)
BOOL __stdcall ENVIRONMENT_UpdateCycleIndex(D2DrlgActStrc* pAct, int nActNo)
{
	if (D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct))
	{
		const int nOldPeriodOfDay = pEnvironment->nPeriodOfDay;
		const int nOldCycleIndex = pEnvironment->nCycleIndex;

		ENVIRONMENT_UpdateTicks(pEnvironment, nActNo);

		if (pEnvironment->nTimeRate)
		{
			const int nNext = pEnvironment->nTicks / pEnvironment->nTimeRate;
			const int nDiff = std::abs(nNext - pEnvironment->nPrev);
			
			// This was probably intended to be >15 or >=16 (ENV_FULL_CIRCLE_DEGREES/NUM_ENVIRONMENT_PERIODS_OF_DAY/NUM_ENVIRONMENT_CYCLES = 15)
			if (nDiff > 16)
			{
				pEnvironment->nPrev = nNext;
				return TRUE;
			}
		}

		if (nOldPeriodOfDay != pEnvironment->nPeriodOfDay || nOldCycleIndex != pEnvironment->nCycleIndex)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FD8DDD0 (#10927)
void __stdcall ENVIRONMENT_GetLightColorFromAct(D2DrlgActStrc* pAct, uint8_t* pRed, uint8_t* pGreen, uint8_t* pBlue)
{
	if (D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct))
	{
		*pRed = pEnvironment->nRed;
		*pGreen = pEnvironment->nGreen;
		*pBlue = pEnvironment->nBlue;
	}
}

//D2Common.0x6FD8DE00 (#10926)
int __stdcall ENVIRONMENT_GetIntensityFromAct(D2DrlgActStrc* pAct)
{
	if (D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct))
	{
		return pEnvironment->nIntensity;
	}
	return 0;
}

//D2Common.0x6FD8DE20 (#10933)
int __stdcall ENVIRONMENT_GetPeriodOfDayFromAct(D2DrlgActStrc* pAct, int* pBaseTime)
{
	if (D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct))
	{
		if (pBaseTime)
		{
			if (pEnvironment->nTimeRate)
			{
				*pBaseTime = pEnvironment->nTicks / pEnvironment->nTimeRate;
			}
			else
			{
				*pBaseTime = 0;
			}
		}

		return pEnvironment->nPeriodOfDay;
	}
	else
	{
		if (pBaseTime)
		{
			*pBaseTime = 0;
		}

		return 0;
	}
}

//D2Common.0x6FD8DE70 (#10928) - UNUSED
int __stdcall ENVIRONMENT_GetUnusedMember(D2DrlgActStrc* pAct)
{
	if (D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct))
	{
		//TODO: Check type of unk0x14
		return pEnvironment->unk0x14;
	}

	return 0;
}

//D2Common.0x6FD8DE90 (#10929) - UNUSED
void __stdcall ENVIRONMENT_NextEnvCycle(D2DrlgActStrc* pAct, D2RoomStrc* pRoom)
{
	D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct);
	
	pEnvironment->nCycleIndex = (pEnvironment->nCycleIndex + 1) % NUM_ENVIRONMENT_CYCLES;

	// Lectem's note: missing act4 again
	const D2EnvironmentCycleStrc* pEnvironmentCycle = NULL;
	if (pEnvironment->bEclipse)
	{
		pEnvironmentCycle = &gEclipseEnvironmentCycle[pEnvironment->nCycleIndex];
	}
	else
	{
		pEnvironmentCycle = &gNormalEnvironmentCycle[pEnvironment->nCycleIndex];
	}

	pEnvironment->nPeriodOfDay = pEnvironmentCycle->nPeriodOfDay;
	pEnvironment->nTicks = pEnvironmentCycle->nTicksBegin * pEnvironment->nTimeRate;

	const int nLevelId = pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom) : 0;
	const int nActNo = DRLG_GetActNoFromLevelId(nLevelId);

	ENVIRONMENT_UpdateLightIntensity(pEnvironment, nLevelId, nActNo);
	ENVIRONMENT_UpdateLightColor(pEnvironment, nActNo);

	if (nLevelId == LEVEL_ROCKYSUMMIT)
	{
		pEnvironment->nRed = 245;
		pEnvironment->nGreen = 240;
		pEnvironment->nBlue = 255;
	}
}

//D2Common.0x6FD8DF30 (#10930)
int __stdcall ENVIRONMENT_GetCycleIndexFromAct(D2DrlgActStrc* pAct)
{
	return DUNGEON_GetEnvironmentFromAct(pAct)->nCycleIndex;
}

//D2Common.0x6FD8DF40 (#10932)
void __stdcall ENVIRONMENT_InitializeEnvironment(D2DrlgActStrc* pAct, D2RoomStrc* pRoom, int nIndex, int nTicks, BOOL bEclipse)
{
	D2_ASSERT(nIndex >= 0);
	D2_ASSERT(nIndex < NUM_ENVIRONMENT_CYCLES);
	D2_ASSERT(nTicks >= 0);

	D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct);
	if (nTicks > ENV_FULL_CIRCLE_DEGREES * pEnvironment->nTimeRate)
	{
		nTicks = 0;
	}

	pEnvironment->nCycleIndex = nIndex;
	pEnvironment->nTicks = nTicks;

	// Lectem's note: missing act4 again
	const D2EnvironmentCycleStrc* pEnvironmentCycle = NULL;
	if (bEclipse)
	{
		pEnvironmentCycle = &gEclipseEnvironmentCycle[nIndex];
	}
	else
	{
		pEnvironmentCycle = &gNormalEnvironmentCycle[nIndex];
	}

	pEnvironment->nPeriodOfDay = pEnvironmentCycle->nPeriodOfDay;

	const int nLevelId = pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom) : 0;
	const int nAct = DRLG_GetActNoFromLevelId(nLevelId);

	ENVIRONMENT_UpdateLightIntensity(pEnvironment, nLevelId, nAct);

	// See ENVIRONMENT_GetCycle
	D2_ASSERT(!(bEclipse && nAct == ACT_IV));
	pEnvironment->bEclipse = bEclipse;

	if (bEclipse)
	{
		pEnvironment->nPeriodOfDay = gEclipseEnvironmentCycle[pEnvironment->nCycleIndex].nPeriodOfDay;
		pEnvironment->nTicks = gEclipseEnvironmentCycle[pEnvironment->nCycleIndex].nTicksBegin * pEnvironment->nTimeRate;

		ENVIRONMENT_UpdateLightIntensity(pEnvironment, nLevelId, nAct);
		ENVIRONMENT_UpdateLightColor(pEnvironment, nAct);
	}

	if (nLevelId == LEVEL_ROCKYSUMMIT)
	{
		pEnvironment->nRed = 245;
		pEnvironment->nGreen = 240;
		pEnvironment->nBlue = 255;
	}
}

//D2Common.0x6FD8E080 (#10931)
void __stdcall ENVIRONMENT_GetCycleIndex_Ticks_EclipseFromAct(D2DrlgActStrc* pAct, int* pCycleIndex, int* pTicks, BOOL* pEclipse)
{
	D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct);

	*pCycleIndex = pEnvironment->nCycleIndex;
	*pTicks = pEnvironment->nTicks;
	*pEclipse = pEnvironment->bEclipse;
}

//D2Common.0x6FD8E0B0 (#10925) - UNUSED
void __stdcall ENVIRONMENT_GetStatistics(D2DrlgActStrc* pAct, float* pCos, float* pLast, float* pSin, int* a5)
{
	D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct);

	*pCos = pEnvironment->fCos;
	*pLast = pEnvironment->fLast;
	*pSin = pEnvironment->fSin;

	*a5 = 0;
}

//D2Common.0x6FD8E0F0 (#10934) - UNUSED
int __stdcall ENVIRONMENT_GetTimeRateFromAct(D2DrlgActStrc* pAct)
{
	return DUNGEON_GetEnvironmentFromAct(pAct)->nTimeRate;
}

//D2Common.0x6FD8E100 (#10935) - UNUSED
void __stdcall ENVIRONMENT_SetNextTimeRate(D2DrlgActStrc* pAct, D2RoomStrc* pRoom)
{
	
	D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct);

	pEnvironment->nTimeRateIndex = (pEnvironment->nTimeRateIndex + 1) % NUM_TIME_RATES;

	pEnvironment->nTimeRate = gnTimeRates[pEnvironment->nTimeRateIndex];

	// Lectem's note: missing act4 again
	const D2EnvironmentCycleStrc* pEnvironmentCycle = NULL;
	if (pEnvironment->bEclipse)
	{
		pEnvironmentCycle = &gEclipseEnvironmentCycle[pEnvironment->nCycleIndex];
	}
	else
	{
		pEnvironmentCycle = &gNormalEnvironmentCycle[pEnvironment->nCycleIndex];
	}

	pEnvironment->nPeriodOfDay = pEnvironmentCycle->nPeriodOfDay;
	pEnvironment->nTicks = pEnvironment->nTimeRate * pEnvironmentCycle->nTicksBegin;

	const int nLevelId = pRoom ? DUNGEON_GetLevelIdFromRoom(pRoom) : 0;
	const int nActNo = DRLG_GetActNoFromLevelId(nLevelId);

	ENVIRONMENT_UpdateLightIntensity(pEnvironment, nLevelId, nActNo);
	ENVIRONMENT_UpdateLightColor(pEnvironment, nActNo);

	if (nLevelId == LEVEL_ROCKYSUMMIT)
	{
		pEnvironment->nRed = 245;
		pEnvironment->nGreen = 240;
		pEnvironment->nBlue = 255;
	}
}

//D2Common.0x6FD8E1B0 (#10936)
void __stdcall ENVIRONMENT_TaintedSunBegin(D2DrlgActStrc* pAct)
{
	D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct);

	pEnvironment->nTimeRateIndex = 1;
	pEnvironment->nTimeRate = gnTimeRates[1];

	ENVIRONMENT_InitializeEnvironment(pAct, NULL, ENVCYCLE_SUNRISE, 0, TRUE);
}

//D2Common.0x6FD8E1E0 (#10937)
void __stdcall ENVIRONMENT_TaintedSunEnd(D2DrlgActStrc* pAct)
{
	D2DrlgEnvironmentStrc* pEnvironment = DUNGEON_GetEnvironmentFromAct(pAct);

	pEnvironment->nTimeRateIndex = 0;
	pEnvironment->nTimeRate = gnTimeRates[0];

	ENVIRONMENT_InitializeEnvironment(pAct, NULL, ENVCYCLE_NOON, 0, FALSE);
}
