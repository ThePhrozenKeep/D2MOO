#include "D2States.h"

#include "D2DataTbls.h"
#include "D2StatList.h"
#include "Units/UnitRoom.h"


//Used in some of the following functions
__forceinline BOOL __fastcall STATES_CheckStateMaskByStateId(int nState, int nStateMask)
{
	if (nState >= 0 && nState < gpDataTables.nStatesTxtRecordCount)
	{
		return gpDataTables.fStateMasks[nStateMask][nState >> 5] & gdwBitMasks[nState & 31];
	}

	return FALSE;
}


//D2Common.0x6FDB4560 (#10486)
void __stdcall STATES_ToggleState(D2UnitStrc* pUnit, int nState, BOOL bSet)
{
	if (nState >= 0 && nState < gpDataTables.nStatesTxtRecordCount)
	{
		D2Common_STATES_ToggleState_6FDB8900(pUnit, nState, bSet);
		UNITROOM_RefreshUnit(pUnit);
	}
}

//D2Common.0x6FDB45A0 (#10487)
BOOL __stdcall STATES_CheckState(D2UnitStrc* pUnit, int nState)
{
	if (pUnit && (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER || pUnit->dwUnitType == UNIT_MISSILE) && nState >= 0 && nState < gpDataTables.nStatesTxtRecordCount)
	{
		if (pUnit->pStatListEx && (pUnit->pStatListEx->dwFlags & 0x80000000) != 0)
		{
			return pUnit->pStatListEx->StatFlags[nState >> 5] & gdwBitMasks[nState & 31];
		}
	}

	return FALSE;
}

//D2Common.0x6FDB45F0 (#10488)
void __stdcall STATES_ToggleGfxStateFlag(D2UnitStrc* pUnit, int nState, BOOL bSet)
{
	DWORD* pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);

	if (pGfxFlags && nState >= 0 && nState < gpDataTables.nStatesTxtRecordCount)
	{
		if (bSet)
		{
			pGfxFlags[nState >> 5] |= gdwBitMasks[nState & 31];
		}
		else
		{
			pGfxFlags[nState >> 5] &= gdwInvBitMasks[nState & 31];
		}

		UNITROOM_RefreshUnit(pUnit);
	}
}

//D2Common.0x6FDB4670 (#10489)
BOOL __stdcall STATES_CheckGfxStateFlag(D2UnitStrc* pUnit, int nState)
{
	DWORD* pGfxFlags = NULL;

	if (nState >= 0 && nState < gpDataTables.nStatesTxtRecordCount)
	{
		pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);
		if (pGfxFlags)
		{
			return pGfxFlags[nState >> 5] & gdwBitMasks[nState & 31];
		}
	}

	return FALSE;
}

//D2Common.0x6FDB46C0 (#10491)
void __stdcall STATES_ClearGfxStateFlags(D2UnitStrc* pUnit)
{
	DWORD* pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);

	if (pGfxFlags)
	{
		memset(pGfxFlags, 0x00, sizeof(DWORD) * (gpDataTables.nStatesTxtRecordCount + 31) / 32);
	}
}

//D2Common.0x6FDB4710 (#10490)
BOOL __stdcall STATES_IsAnyGfxStateFlagSet(D2UnitStrc* pUnit)
{
	DWORD* pGfxFlags = NULL;
	int nCounter = 0;

	pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);
	if (pGfxFlags && (gpDataTables.nStatesTxtRecordCount + 31) / 32 > 0)
	{
		while (!pGfxFlags[nCounter])
		{
			++nCounter;
			if (nCounter >= (gpDataTables.nStatesTxtRecordCount + 31) / 32)
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDB4760 (#10492)
DWORD* __stdcall STATES_GetGfxStateFlags(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	return D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);
}

//D2Common.0x6FDB4790 (#10493)
void __stdcall STATES_UpdateStayDeathFlags(D2UnitStrc* pUnit, BOOL bIsBoss)
{
	DWORD* pStatFlags = NULL;
	DWORD* pGfxFlags = NULL;

	pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);
	pStatFlags = D2COMMON_STATES_GetStatFlags_6FDB8A90(pUnit);

	if (pGfxFlags && pStatFlags)
	{
		if (bIsBoss)
		{
			for (int i = 0; i < (gpDataTables.nStatesTxtRecordCount + 31) / 32; ++i)
			{
				pGfxFlags[i] |= pStatFlags[i] & ~gpDataTables.fStateMasks[STATEMASK_BOSSSTAYDEATH][i];
				pStatFlags[i] &= gpDataTables.fStateMasks[STATEMASK_BOSSSTAYDEATH][i];
			}
		}
		else if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
		{
			for (int i = 0; i < (gpDataTables.nStatesTxtRecordCount + 31) / 32; ++i)
			{
				pGfxFlags[i] |= pStatFlags[i] & ~gpDataTables.fStateMasks[STATEMASK_MONSTAYDEATH][i];
				pStatFlags[i] &= gpDataTables.fStateMasks[STATEMASK_MONSTAYDEATH][i];
			}
		}
		else
		{
			for (int i = 0; i < (gpDataTables.nStatesTxtRecordCount + 31) / 32; ++i)
			{
				pGfxFlags[i] |= pStatFlags[i] & ~gpDataTables.fStateMasks[STATEMASK_PLRSTAYDEATH][i];
				pStatFlags[i] &= gpDataTables.fStateMasks[STATEMASK_PLRSTAYDEATH][i];
			}
		}

		UNITROOM_RefreshUnit(pUnit);
	}
}

//D2Common.0x6FDB48F0 (#10494)
DWORD* __stdcall D2COMMON_10494_STATES_GetStatFlags(D2UnitStrc* pUnit)
{
	return D2COMMON_STATES_GetStatFlags_6FDB8A90(pUnit);
}

//D2Common.0x6FDB4900 (#10495)
int __fastcall STATES_GetNumberOfStateFlags()
{
	return (gpDataTables.nStatesTxtRecordCount + 31) / 32;
}

//D2Common.0x6FDB4920 (#10496)
BOOL __stdcall STATES_CheckStateMaskAuraByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_AURA);
}

//D2Common.0x6FDB4960 (#10503)
BOOL __stdcall STATES_CheckStateMaskAuraOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_AURA);
}

//D2Common.0x6FDB49D0 (#10500)
BOOL __stdcall STATES_CheckStateMaskPgsvByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_PGSV);
}

//D2Common.0x6FDB4A10 (#11234)
BOOL __stdcall STATES_CheckStateMaskPgsvOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_PGSV);
}

//D2Common.0x6FDB4A80 (#10501)
void __stdcall STATES_UpdatePgsvFlags(D2UnitStrc* pUnit)
{
	DWORD* pStatFlags = NULL;
	DWORD* pGfxFlags = NULL;

	pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);
	pStatFlags = D2COMMON_STATES_GetStatFlags_6FDB8A90(pUnit);

	if (pGfxFlags && pStatFlags)
	{
		for (int i = 0; i < (gpDataTables.nStatesTxtRecordCount + 31) / 32; ++i)
		{
			pGfxFlags[i] |= pStatFlags[i] & gpDataTables.fStateMasks[STATEMASK_PGSV][i];
			pStatFlags[i] &= ~gpDataTables.fStateMasks[STATEMASK_PGSV][i];
		}

		UNITROOM_RefreshUnit(pUnit);
	}
}

//D2Common.0x6FDB4B20 (#10558)
BOOL __stdcall STATES_CheckStateMaskHideByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_HIDE);
}

//D2Common.0x6FDB4B50 (#10557)
BOOL __stdcall STATES_CheckStateMaskHideOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_HIDE);
}

//D2Common.0x6FDB4BC0 (#10502)
BOOL __stdcall STATES_CheckStateMaskActiveOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_ACTIVE);
}

//D2Common.0x6FDB4C30 (#11272)
BOOL __stdcall STATES_CheckStateMaskRemHitOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_REMHIT);
}

//D2Common.0x6FDB4CA0 (#10538)
BOOL __stdcall STATES_CheckStateMaskDamBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_DAMBLUE);
}

//D2Common.0x6FDB4D10 (#10545)
BOOL __stdcall STATES_CheckStateMaskDamRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_DAMRED);
}

//D2Common.0x6FDB4D80 (#10537)
BOOL __stdcall STATES_CheckStateMaskAttBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_ATTBLUE);
}

//D2Common.0x6FDB4DF0 (#10544)
BOOL __stdcall STATES_CheckStateMaskAttRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_ATTRED);
}

//D2Common.0x6FDB4E60 (#10551)
BOOL __stdcall STATES_IsUnitShapeShifted(D2UnitStrc* pUnit)
{
	if (pUnit)
	{
		return pUnit->dwFlagEx & UNITFLAGEX_ISSHAPESHIFTED;
	}

	return FALSE;
}

//D2Common.0x6FDB4E80 (#10497)
BOOL __stdcall STATES_CheckStateMaskCurseByStateId(int nState)
{
	if (nState >= 0 && nState < gpDataTables.nStatesTxtRecordCount)
	{
		return gpDataTables.fStateMasks[11][nState >> 5] & gdwBitMasks[nState & 31];
	}

	return STATES_CheckStateMaskByStateId(nState, STATEMASK_CURSE);
}

//D2Common.0x6FDB4EC0 (#10498)
BOOL __stdcall STATES_CheckStateMaskCurableByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_CURABLE);
}

//D2Common.0x6FDB4F00 (#10554)
BOOL __stdcall STATES_CheckStateMaskStayDeathOnUnitByStateId(D2UnitStrc* pUnit, int nState)
{
	if (nState >= 0 && nState < gpDataTables.nStatesTxtRecordCount)
	{
		if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
		{
			return gpDataTables.fStateMasks[STATEMASK_MONSTAYDEATH][nState >> 5] & gdwBitMasks[nState & 31];
		}
		else
		{
			return gpDataTables.fStateMasks[STATEMASK_PLRSTAYDEATH][nState >> 5] & gdwBitMasks[nState & 31];
		}
	}

	return FALSE;
}

//D2Common.0x6FDB4F60
BOOL __fastcall STATES_CheckStateMaskRestrictOnUnit(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RESTRICT);
}

//D2Common.0x6FDB4FC0 (#10536)
BOOL __stdcall STATES_CheckStateMaskBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_BLUE);
}

//D2Common.0x6FDB5030 (#10539)
BOOL __stdcall STATES_CheckStateMaskArmBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_ARMBLUE);
}

//D2Common.0x6FDB50A0 (#10540)
BOOL __stdcall STATES_CheckStateMaskRFBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RFBLUE);
}

//D2Common.0x6FDB5110 (#10541)
BOOL __stdcall STATES_CheckStateMaskRCBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RCBLUE);
}

//D2Common.0x6FDB5180 (#10542)
BOOL __stdcall STATES_CheckStateMaskRLBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RLBLUE);
}

//D2Common.0x6FDB51F0 (#10543)
BOOL __stdcall STATES_CheckStateMaskRPBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RPBLUE);
}

//D2Common.0x6FDB5260 (#10546)
BOOL __stdcall STATES_CheckStateMaskArmRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_ARMRED);
}

//D2Common.0x6FDB52D0 (#10547)
BOOL __stdcall STATES_CheckStateMaskRFRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RFRED);
}

//D2Common.0x6FDB5340 (#10548)
BOOL __stdcall STATES_CheckStateMaskRCRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RCRED);
}

//D2Common.0x6FDB53B0 (#10549)
BOOL __stdcall STATES_CheckStateMaskRLRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RLRED);
}

//D2Common.0x6FDB5420 (#10550)
BOOL __stdcall STATES_CheckStateMaskRPRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RPRED);
}

//D2Common.0x6FDB5490 (#10552)
BOOL __stdcall STATES_CheckStateMaskExpOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_EXP);
}

//D2Common.0x6FDB5500 (#10553)
BOOL __stdcall STATES_CheckStateMaskExpByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_EXP);
}

//D2Common.0x6FDB5540 (#10555)
BOOL __stdcall STATES_CheckStateMaskShatterOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_SHATTER);
}

//D2Common.0x6FDB55B0 (#10556)
BOOL __stdcall STATES_CheckStateMaskShatterByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_SHATTER);
}

//D2Common.0x6FDB55F0 (#10559)
BOOL __stdcall STATES_CheckStateMaskLifeOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_LIFE);
}

//D2Common.0x6FDB5660 (#10560)
BOOL __stdcall STATES_CheckStateMaskUdeadOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_UDEAD);
}

//D2Common.0x6FDB56D0 (#10561)
BOOL __stdcall STATES_CheckStateMaskGreenOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_GREEN);
}

//D2Common.0x6FDB5740 (#11303)
BOOL __stdcall STATES_CheckStateMaskOnUnit(D2UnitStrc* pUnit, int nStateMask)
{
	DWORD* pStatFlags = NULL;
	int nCounter = 0;

	if (pUnit && nStateMask >= 0 && nStateMask < 40)
	{
		pStatFlags = D2COMMON_STATES_GetStatFlags_6FDB8A90(pUnit);

		if (pStatFlags)
		{
			for (int i = 0; i < (gpDataTables.nStatesTxtRecordCount + 31) / 32; ++i)
			{
				if (pStatFlags[i] & gpDataTables.fStateMasks[nStateMask][i])
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}
