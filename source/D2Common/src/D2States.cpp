#include "D2States.h"

#include "D2DataTbls.h"
#include "D2StatList.h"
#include "Units/UnitRoom.h"
#include "Units/Units.h"
#include <D2BitManip.h>

//Used in some of the following functions
__forceinline BOOL __fastcall STATES_CheckStateMaskByStateId(int nState, int nStateMask)
{
	if (nState >= 0 && nState < sgptDataTables->nStatesTxtRecordCount)
	{
		return sgptDataTables->fStateMasks[nStateMask][nState >> 5] & gdwBitMasks[nState & 31];
	}

	return FALSE;
}


//D2Common.0x6FDB4560 (#10486)
//1.14d: 0x00639DB0
void __stdcall STATES_ToggleState(D2UnitStrc* pUnit, int nState, BOOL bSet)
{
	if (nState >= 0 && nState < sgptDataTables->nStatesTxtRecordCount)
	{
		D2Common_STATES_ToggleState_6FDB8900(pUnit, nState, bSet);
		UNITROOM_RefreshUnit(pUnit);
	}
}

//D2Common.0x6FDB45A0 (#10487)
//1.14d: 0x00639DF0
BOOL __stdcall STATES_CheckState(D2UnitStrc* pUnit, int nState)
{
	if (pUnit && (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER || pUnit->dwUnitType == UNIT_MISSILE) && nState >= 0 && nState < sgptDataTables->nStatesTxtRecordCount)
	{
		if (pUnit->pStatListEx && STATLIST_IsExtended(pUnit->pStatListEx))
		{
			return pUnit->pStatListEx->StatFlags[nState >> 5] & gdwBitMasks[nState & 31];
		}
	}

	return FALSE;
}

//D2Common.0x6FDB45F0 (#10488)
//1.14d: 0x00639E30
void __stdcall STATES_ToggleGfxStateFlag(D2UnitStrc* pUnit, int nState, BOOL bSet)
{
	uint32_t* pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);

	if (pGfxFlags && nState >= 0 && nState < sgptDataTables->nStatesTxtRecordCount)
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
//1.14d: 0x00639E90
BOOL __stdcall STATES_CheckGfxStateFlag(D2UnitStrc* pUnit, int nState)
{
	uint32_t* pGfxFlags = NULL;

	if (nState >= 0 && nState < sgptDataTables->nStatesTxtRecordCount)
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
//1.14d: 0x00639EE0
void __stdcall STATES_ClearGfxStateFlags(D2UnitStrc* pUnit)
{
	uint32_t* pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);

	if (pGfxFlags)
	{
		memset(pGfxFlags, 0x00, sizeof(uint32_t) * (sgptDataTables->nStatesTxtRecordCount + 31) / 32);
	}
}

//D2Common.0x6FDB4710 (#10490)
//1.14d: 0x00639F20
BOOL __stdcall STATES_IsAnyGfxStateFlagSet(D2UnitStrc* pUnit)
{
	uint32_t* pGfxFlags = NULL;
	int nCounter = 0;

	pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);
	if (pGfxFlags && (sgptDataTables->nStatesTxtRecordCount + 31) / 32 > 0)
	{
		while (!pGfxFlags[nCounter])
		{
			++nCounter;
			if (nCounter >= (sgptDataTables->nStatesTxtRecordCount + 31) / 32)
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDB4760 (#10492)
//1.14d: 0x00639F70
uint32_t* __stdcall STATES_GetGfxStateFlags(D2UnitStrc* pUnit)
{
	D2_ASSERT(pUnit);

	return D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);
}

//D2Common.0x6FDB4790 (#10493)
//1.14d: 0x00639FB0
void __stdcall STATES_UpdateStayDeathFlags(D2UnitStrc* pUnit, BOOL bIsBoss)
{
	uint32_t* pStatFlags = NULL;
	uint32_t* pGfxFlags = NULL;

	pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);
	pStatFlags = D2COMMON_STATES_GetStatFlags_6FDB8A90(pUnit);

	if (pGfxFlags && pStatFlags)
	{
		if (bIsBoss)
		{
			for (int i = 0; i < (sgptDataTables->nStatesTxtRecordCount + 31) / 32; ++i)
			{
				pGfxFlags[i] |= pStatFlags[i] & ~sgptDataTables->fStateMasks[STATEMASK_BOSSSTAYDEATH][i];
				pStatFlags[i] &= sgptDataTables->fStateMasks[STATEMASK_BOSSSTAYDEATH][i];
			}
		}
		else if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
		{
			for (int i = 0; i < (sgptDataTables->nStatesTxtRecordCount + 31) / 32; ++i)
			{
				pGfxFlags[i] |= pStatFlags[i] & ~sgptDataTables->fStateMasks[STATEMASK_MONSTAYDEATH][i];
				pStatFlags[i] &= sgptDataTables->fStateMasks[STATEMASK_MONSTAYDEATH][i];
			}
		}
		else
		{
			for (int i = 0; i < (sgptDataTables->nStatesTxtRecordCount + 31) / 32; ++i)
			{
				pGfxFlags[i] |= pStatFlags[i] & ~sgptDataTables->fStateMasks[STATEMASK_PLRSTAYDEATH][i];
				pStatFlags[i] &= sgptDataTables->fStateMasks[STATEMASK_PLRSTAYDEATH][i];
			}
		}

		UNITROOM_RefreshUnit(pUnit);
	}
}

//D2Common.0x6FDB48F0 (#10494)
//1.14d: 0x0063A100
uint32_t* __stdcall D2COMMON_10494_STATES_GetStatFlags(D2UnitStrc* pUnit)
{
	return D2COMMON_STATES_GetStatFlags_6FDB8A90(pUnit);
}

//D2Common.0x6FDB4900 (#10495)
//1.14d: 0x0063A110
int __fastcall STATES_GetNumberOfStateFlags()
{
	return (sgptDataTables->nStatesTxtRecordCount + 31) / 32;
}

//D2Common.0x6FDB4920 (#10496)
//1.14d: 0x0063A210
BOOL __stdcall STATES_CheckStateMaskAuraByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_AURA);
}

//D2Common.0x6FDB4960 (#10503)
//1.14d: 0x0063A250
BOOL __stdcall STATES_CheckStateMaskAuraOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_AURA);
}

//D2Common.0x6FDB49D0 (#10500)
//1.14d: 0x0063A270
BOOL __stdcall STATES_CheckStateMaskPgsvByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_PGSV);
}

//D2Common.0x6FDB4A10 (#11234)
//1.14d: 0x0063A2B0
BOOL __stdcall STATES_CheckStateMaskPgsvOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_PGSV);
}

//D2Common.0x6FDB4A80 (#10501)
//1.14d: 0x0063A2D0
void __stdcall STATES_UpdatePgsvFlags(D2UnitStrc* pUnit)
{
	uint32_t* pStatFlags = NULL;
	uint32_t* pGfxFlags = NULL;

	pGfxFlags = D2COMMON_STATES_GetListGfxFlags_6FDB8AC0(pUnit);
	pStatFlags = D2COMMON_STATES_GetStatFlags_6FDB8A90(pUnit);

	if (pGfxFlags && pStatFlags)
	{
		for (int i = 0; i < (sgptDataTables->nStatesTxtRecordCount + 31) / 32; ++i)
		{
			pGfxFlags[i] |= pStatFlags[i] & sgptDataTables->fStateMasks[STATEMASK_PGSV][i];
			pStatFlags[i] &= ~sgptDataTables->fStateMasks[STATEMASK_PGSV][i];
		}

		UNITROOM_RefreshUnit(pUnit);
	}
}

//D2Common.0x6FDB4B20 (#10558)
//1.14d: 0x0063A2F0
BOOL __stdcall STATES_CheckStateMaskHideByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_HIDE);
}

//D2Common.0x6FDB4B50 (#10557)
//1.14d: 0x0063A320
BOOL __stdcall STATES_CheckStateMaskHideOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_HIDE);
}

//D2Common.0x6FDB4BC0 (#10502)
//1.14d: 0x0063A340
BOOL __stdcall STATES_CheckStateMaskActiveOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_ACTIVE);
}

//D2Common.0x6FDB4C30 (#11272)
//1.14d: 0x0063A360
BOOL __stdcall STATES_CheckStateMaskRemHitOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_REMHIT);
}

//D2Common.0x6FDB4CA0 (#10538)
//1.14d: 0x0063A380
BOOL __stdcall STATES_CheckStateMaskDamBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_DAMBLUE);
}

//D2Common.0x6FDB4D10 (#10545)
//1.14d: 0x0063A3A0
BOOL __stdcall STATES_CheckStateMaskDamRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_DAMRED);
}

//D2Common.0x6FDB4D80 (#10537)
//1.14d: 0x0063A3C0
BOOL __stdcall STATES_CheckStateMaskAttBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_ATTBLUE);
}

//D2Common.0x6FDB4DF0 (#10544)
//1.14d: 0x0063A3E0
BOOL __stdcall STATES_CheckStateMaskAttRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_ATTRED);
}

//D2Common.0x6FDB4E60 (#10551)
//1.14d: 0x0063A400
BOOL __stdcall STATES_IsUnitShapeShifted(D2UnitStrc* pUnit)
{
	if (pUnit)
	{
		return pUnit->dwFlagEx & UNITFLAGEX_ISSHAPESHIFTED;
	}

	return FALSE;
}

//D2Common.0x6FDB4E80 (#10497)
//1.14d: 0x0063A420
BOOL __stdcall STATES_CheckStateMaskCurseByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_CURSE);
}

//D2Common.0x6FDB4EC0 (#10498)
//1.14d: 0x0063A460
BOOL __stdcall STATES_CheckStateMaskCurableByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_CURABLE);
}

//D2Common.0x6FDB4F00 (#10554)
//1.14d: 0x0063A4A0
BOOL __stdcall STATES_CheckStateMaskStayDeathOnUnitByStateId(D2UnitStrc* pUnit, int nState)
{
	if (nState >= 0 && nState < sgptDataTables->nStatesTxtRecordCount)
	{
		if (pUnit && pUnit->dwUnitType == UNIT_MONSTER)
		{
			return sgptDataTables->fStateMasks[STATEMASK_MONSTAYDEATH][nState >> 5] & gdwBitMasks[nState & 31];
		}
		else
		{
			return sgptDataTables->fStateMasks[STATEMASK_PLRSTAYDEATH][nState >> 5] & gdwBitMasks[nState & 31];
		}
	}

	return FALSE;
}

//D2Common.0x6FDB4F60
//1.14d: 0x0063A510
BOOL __fastcall STATES_CheckStateMaskRestrictOnUnit(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RESTRICT);
}

//D2Common.0x6FDB4FC0 (#10536)
//1.14d: 0x0063A530
BOOL __stdcall STATES_CheckStateMaskBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_BLUE);
}

//D2Common.0x6FDB5030 (#10539)
//1.14d: 0x0063A550
BOOL __stdcall STATES_CheckStateMaskArmBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_ARMBLUE);
}

//D2Common.0x6FDB50A0 (#10540)
//1.14d: 0x0063A570
BOOL __stdcall STATES_CheckStateMaskRFBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RFBLUE);
}

//D2Common.0x6FDB5110 (#10541)
//1.14d: 0x0063A590
BOOL __stdcall STATES_CheckStateMaskRCBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RCBLUE);
}

//D2Common.0x6FDB5180 (#10542)
//1.14d: 0x0063A5B0
BOOL __stdcall STATES_CheckStateMaskRLBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RLBLUE);
}

//D2Common.0x6FDB51F0 (#10543)
//1.14d: 0x0063A5D0
BOOL __stdcall STATES_CheckStateMaskRPBlueOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RPBLUE);
}

//D2Common.0x6FDB5260 (#10546)
//1.14d: 0x0063A5F0
BOOL __stdcall STATES_CheckStateMaskArmRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_ARMRED);
}

//D2Common.0x6FDB52D0 (#10547)
//1.14d: 0x0063A610
BOOL __stdcall STATES_CheckStateMaskRFRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RFRED);
}

//D2Common.0x6FDB5340 (#10548)
//1.14d: 0x0063A630
BOOL __stdcall STATES_CheckStateMaskRCRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RCRED);
}

//D2Common.0x6FDB53B0 (#10549)
//1.14d: 0x0063A650
BOOL __stdcall STATES_CheckStateMaskRLRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RLRED);
}

//D2Common.0x6FDB5420 (#10550)
//1.14d: 0x0063A670
BOOL __stdcall STATES_CheckStateMaskRPRedOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_RPRED);
}

//D2Common.0x6FDB5490 (#10552)
//1.14d: 0x0063A690
BOOL __stdcall STATES_CheckStateMaskExpOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_EXP);
}

//D2Common.0x6FDB5500 (#10553)
//1.14d: 0x0063A6B0
BOOL __stdcall STATES_CheckStateMaskExpByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_EXP);
}

//D2Common.0x6FDB5540 (#10555)
//1.14d: 0x0063A6F0
BOOL __stdcall STATES_CheckStateMaskShatterOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_SHATTER);
}

//D2Common.0x6FDB55B0 (#10556)
//1.14d: 0x0063A710
BOOL __stdcall STATES_CheckStateMaskShatterByStateId(int nState)
{
	return STATES_CheckStateMaskByStateId(nState, STATEMASK_SHATTER);
}

//D2Common.0x6FDB55F0 (#10559)
//1.14d: 0x0063A750
BOOL __stdcall STATES_CheckStateMaskLifeOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_LIFE);
}

//D2Common.0x6FDB5660 (#10560)
//1.14d: 0x0063A770
BOOL __stdcall STATES_CheckStateMaskUdeadOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_UDEAD);
}

//D2Common.0x6FDB56D0 (#10561)
//1.14d: 0x0063A790
BOOL __stdcall STATES_CheckStateMaskGreenOnUnit(D2UnitStrc* pUnit)
{
	return STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_GREEN);
}

//D2Common.0x6FDB5740 (#11303)
//1.14d: 0x0063A7B0
BOOL __stdcall STATES_CheckStateMaskOnUnit(D2UnitStrc* pUnit, int nStateMask)
{

	if (pUnit && nStateMask >= 0 && nStateMask < 40)
	{
		uint32_t* pStatFlags = D2COMMON_STATES_GetStatFlags_6FDB8A90(pUnit);

		if (pStatFlags)
		{
			for (int i = 0; i < (sgptDataTables->nStatesTxtRecordCount + 31) / 32; ++i)
			{
				if (pStatFlags[i] & sgptDataTables->fStateMasks[nStateMask][i])
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}
