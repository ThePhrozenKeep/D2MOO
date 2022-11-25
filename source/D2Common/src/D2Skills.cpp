#include "D2Skills.h"

#include "D2Collision.h"
#include "D2Composit.h"
#include "D2DataTbls.h"
#include "D2Dungeon.h"
#include "D2Inventory.h"
#include "D2ItemMods.h"
#include "D2Items.h"
#include "Units/Missile.h"
#include "Path/Path.h"
#include "D2Seed.h"
#include "D2States.h"
#include "D2StatList.h"
#include "Units/Units.h"
#include <D2BitManip.h>
#include <DataTbls/SkillsIds.h>
#include <D2Combat.h>
#include <D2Monsters.h>

//TODO: Find names

D2UnkFogStrc off_6FDE5804[] =
{
	{ MISSILE_GetMinimum, 2 },
	{ MISSILE_GetMaximum, 2 },
	{ sub_6FD98220, 2 },
	{ sub_6FDAF6C0, 2 },
	{ sub_6FDAF780, 2 },
	{ sub_6FD982A0, 2 },
	{ sub_6FDAF7A0, 3 },
};
int dword_6FDE583C = ARRAY_SIZE(off_6FDE5804);

const int dword_6FDD2B98[] =
{
	0, 1, 2, 4, 7, 8, 9, 10, 13, 14, 15, 16, 17, 19, 18, 3
};

const int dword_6FDD2BD8[] =
{
	0, 1, 2, 15, 3, 1, 2, 4, 5, 6, 7, 4, 11, 8, 9, 10, 11, 12, 14, 13
};




//D2Common.0x6FDAEB10 (#10938)
short __stdcall SKILLS_GetPassiveState(int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord && pSkillsTxtRecord->nPassiveState > 0)
	{
		return pSkillsTxtRecord->nPassiveState;
	}

	return 0;
}

//D2Common.0x6FDAEB60 (#11271)
//TODO: v8
int __stdcall SKILLS_GetSpecialParamValue(D2UnitStrc* pUnit, uint8_t nParamId, int nSkillId, int nSkillLevel)
{
	D2SkillDescTxt* pSkillDescTxtRecord = NULL;
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2SkillStrc* pSkill = NULL;
	__int64 v8 = 0;
	int nMissileId = 0;
	D2SkillCalcStrc pSkillCalc = {};

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
	if (pSkillsTxtRecord)
	{
		switch (nParamId)
		{
		case 0:
			if (nSkillLevel > 0)
			{
				return pSkillsTxtRecord->dwParam[0] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[1];
			}
			break;

		case 1:
			if (nSkillLevel > 0)
			{
				return D2Common_11033(nSkillLevel, pSkillsTxtRecord->dwParam[0], pSkillsTxtRecord->dwParam[1]);
			}
			break;

		case 2:
			if (nSkillLevel > 0)
			{
				return pSkillsTxtRecord->dwParam[2] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[3];
			}
			break;

		case 3:
			if (nSkillLevel > 0)
			{
				return D2Common_11033(nSkillLevel, pSkillsTxtRecord->dwParam[2], pSkillsTxtRecord->dwParam[3]);
			}
			break;

		case 4:
			if (nSkillLevel > 0)
			{
				return pSkillsTxtRecord->dwParam[4] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[5];
			}
			break;

		case 5:
			if (nSkillLevel > 0)
			{
				return D2Common_11033(nSkillLevel, pSkillsTxtRecord->dwParam[4], pSkillsTxtRecord->dwParam[5]);
			}
			break;

		case 6:
			if (nSkillLevel > 0)
			{
				return pSkillsTxtRecord->dwParam[6] + (nSkillLevel - 1) * pSkillsTxtRecord->dwParam[7];
			}
			break;

		case 7:
			return D2Common_11033(nSkillLevel, pSkillsTxtRecord->dwParam[6], pSkillsTxtRecord->dwParam[7]);

		case 8:
			return pSkillsTxtRecord->dwParam[0];

		case 9:
			return pSkillsTxtRecord->dwParam[1];

		case 10:
			return pSkillsTxtRecord->dwParam[2];

		case 11:
			return pSkillsTxtRecord->dwParam[3];

		case 12:
			return pSkillsTxtRecord->dwParam[4];

		case 13:
			return pSkillsTxtRecord->dwParam[5];

		case 14:
			return pSkillsTxtRecord->dwParam[6];

		case 15:
			return pSkillsTxtRecord->dwParam[7];

		case 16:
			return nSkillLevel;

		case 17:
			return SKILLS_GetMinElemDamage(pUnit, nSkillId, nSkillLevel, 0) >> 8;

		case 18:
			return SKILLS_GetMaxElemDamage(pUnit, nSkillId, nSkillLevel, 0) >> 8;

		case 19:
			return SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 0);

		case 20:
			if (nSkillLevel > 0)
			{
				if (pSkillsTxtRecord->dwToHitCalc == -1)
				{
					return pSkillsTxtRecord->dwToHit + (nSkillLevel - 1) * pSkillsTxtRecord->dwLevToHit;
				}
				else
				{
					return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwToHitCalc, nSkillId, nSkillLevel);
				}
			}
			break;

		case 21:
			if (nSkillLevel > 0)
			{
				return SKILLS_GetManaCosts(nSkillId, nSkillLevel) >> 8;
			}
			break;

		case 22:
			if (nSkillLevel > 0)
			{
				v8 = 25 * ((signed int)pSkillsTxtRecord->wMana + (nSkillLevel - 1) * (signed int)pSkillsTxtRecord->wLevelMana);
				return ((signed int)v8 - HIDWORD(v8)) >> 1 << pSkillsTxtRecord->nManaShift >> 8;
			}
			break;

		case 23:
			return D2Common_11032(pUnit, nSkillId, nSkillLevel, 0);

		case 24:
			return D2Common_11032(pUnit, nSkillId, nSkillLevel, 1);

		case 25:
			return D2Common_11032(pUnit, nSkillId, nSkillLevel, 2);

		case 26:
		case 29:
		case 32:
			if (nSkillLevel > 0)
			{
				pSkillDescTxtRecord = DATATBLS_GetSkillDescTxtRecord(pSkillsTxtRecord->wSkillDesc);
				if (pSkillDescTxtRecord)
				{
					nMissileId = pSkillDescTxtRecord->wDescMissile[(nParamId - 26) / 3];
					if (DATATBLS_GetMissilesTxtRecord(nMissileId))
					{
						return MISSILE_GetMinElemDamage(0, pUnit, nMissileId, nSkillLevel) >> 8;
					}
				}
			}
			break;

		case 27:
		case 30:
		case 33:
			if (nSkillLevel > 0)
			{
				pSkillDescTxtRecord = DATATBLS_GetSkillDescTxtRecord(pSkillsTxtRecord->wSkillDesc);
				if (pSkillDescTxtRecord)
				{
					nMissileId = pSkillDescTxtRecord->wDescMissile[(nParamId - 27) / 3];
					if (DATATBLS_GetMissilesTxtRecord(nMissileId))
					{
						return MISSILE_GetMaxElemDamage(0, pUnit, nMissileId, nSkillLevel) >> 8;
					}
				}
			}
			break;

		case 28:
		case 31:
		case 34:
			if (nSkillLevel > 0)
			{
				pSkillDescTxtRecord = DATATBLS_GetSkillDescTxtRecord(pSkillsTxtRecord->wSkillDesc);
				if (pSkillDescTxtRecord)
				{
					nMissileId = pSkillDescTxtRecord->wDescMissile[(nParamId - 28) / 3];
					if (DATATBLS_GetMissilesTxtRecord(nMissileId))
					{
						return MISSILE_GetElementalLength(0, pUnit, nMissileId, nSkillLevel);
					}
				}
			}
			break;

		case 35:
		case 36:
		case 37:
			if (nSkillLevel > 0)
			{
				pSkillDescTxtRecord = DATATBLS_GetSkillDescTxtRecord(pSkillsTxtRecord->wSkillDesc);
				if (pSkillDescTxtRecord)
				{
					nMissileId = pSkillDescTxtRecord->wDescMissile[nParamId - 35];
					if (DATATBLS_GetMissilesTxtRecord(nMissileId))
					{
						return sgptDataTables->pMissilesTxt[nMissileId].wRange + nSkillLevel * sgptDataTables->pMissilesTxt[nMissileId].wLevRange;
					}
				}
			}
			break;

		case 38:
			return SKILLS_GetMinElemDamage(pUnit, nSkillId, nSkillLevel, 0);

		case 39:
			return SKILLS_GetMaxElemDamage(pUnit, nSkillId, nSkillLevel, 0);

		case 40:
			if (pUnit)
			{
				return STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0);
			}
			break;

		case 41:
			if (pUnit)
			{
				pSkill = SKILLS_GetHighestLevelSkillFromSkillId(pUnit, nSkillId);
				if (pSkill)
				{
					return SKILLS_GetSkillLevel(pUnit, pSkill, FALSE);
				}
			}
			break;

		case 42:
			if (nSkillLevel > 0)
			{
				return SKILLS_GetManaCosts(nSkillId, nSkillLevel);
			}
			break;

		case 43:
		case 45:
		case 47:
			if (nSkillLevel > 0)
			{
				pSkillDescTxtRecord = DATATBLS_GetSkillDescTxtRecord(pSkillsTxtRecord->wSkillDesc);
				if (pSkillDescTxtRecord)
				{
					nMissileId = pSkillDescTxtRecord->wDescMissile[(nParamId - 43) / 3];//TODO: Probably Copy-Paste-Error, Should be (a2 - 43) / 2
					if (DATATBLS_GetMissilesTxtRecord(nMissileId))
					{
						return MISSILE_GetMinElemDamage(0, pUnit, nMissileId, nSkillLevel);
					}
				}
			}
			break;

		case 44:
		case 46:
		case 48:
			if (nSkillLevel > 0)
			{
				pSkillDescTxtRecord = DATATBLS_GetSkillDescTxtRecord(pSkillsTxtRecord->wSkillDesc);
				if (pSkillDescTxtRecord)
				{
					nMissileId = pSkillDescTxtRecord->wDescMissile[(nParamId - 44) / 3];//TODO: Probably Copy-Paste-Error, Should be (a2 - 44) / 2
					if (DATATBLS_GetMissilesTxtRecord(nMissileId))
					{
						return MISSILE_GetMaxElemDamage(0, pUnit, nMissileId, nSkillLevel);
					}
				}
			}
			break;

		case 49:
			return SKILLS_GetMinElemDamage(pUnit, nSkillId, nSkillLevel, 1) >> 8;

		case 50:
			return SKILLS_GetMaxElemDamage(pUnit, nSkillId, nSkillLevel, 1) >> 8;

		case 51:
			return SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1);

		case 52:
			return SKILLS_GetMinElemDamage(pUnit, nSkillId, nSkillLevel, 1);

		case 53:
			return SKILLS_GetMaxElemDamage(pUnit, nSkillId, nSkillLevel, 1);

		case 54:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);

		case 55:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);

		case 56:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);

		case 57:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);

		case 58:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);

		case 59:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);

		case 60:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[0], nSkillId, nSkillLevel);

		case 61:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[1], nSkillId, nSkillLevel);

		case 62:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[2], nSkillId, nSkillLevel);

		case 63:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[3], nSkillId, nSkillLevel);

		case 64:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[4], nSkillId, nSkillLevel);

		case 65:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[5], nSkillId, nSkillLevel);

		case 66:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[0], nSkillId, nSkillLevel);

		case 67:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[1], nSkillId, nSkillLevel);

		case 68:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[2], nSkillId, nSkillLevel);

		case 69:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[3], nSkillId, nSkillLevel);

		case 70:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[4], nSkillId, nSkillLevel);

		case 71:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

		case 72:
			return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwSkPoints, nSkillId, nSkillLevel);

		default:
			return 0;
		}
	}
	
	return 0;
}

//D2Common.0x6FDAF6A0
int __fastcall sub_6FDAF6A0(uint8_t nParamId, D2SkillCalcStrc* pSkillCalc)
{
	if (pSkillCalc)
	{
		return SKILLS_GetSpecialParamValue(pSkillCalc->pUnit, nParamId, pSkillCalc->nSkillId, pSkillCalc->nSkillLevel);
	}

	return 0;
}

//D2Common.0x6FDAF6C0
int __fastcall sub_6FDAF6C0(int nSkillId, uint8_t nParamId, int nUnused, D2SkillCalcStrc* pSkillCalc)
{
	D2SkillStrc* pSkill = NULL;
	int nSkillLevel = 0;

	if (pSkillCalc)
	{
		if (pSkillCalc->pUnit)
		{
			pSkill = SKILLS_GetHighestLevelSkillFromSkillId(pSkillCalc->pUnit, nSkillId);

			if (pSkill)
			{
				nSkillLevel = SKILLS_GetSkillLevel(pSkillCalc->pUnit, pSkill, TRUE);
			}
		}

		return SKILLS_GetSpecialParamValue(pSkillCalc->pUnit, nParamId, nSkillId, nSkillLevel);
	}
	
	return 0;
}

//D2Common.0x6FDAF780
int __fastcall sub_6FDAF780(int nMissileId, uint8_t nParamId, int nUnused, D2SkillCalcStrc* pSkillCalc)
{
	if (pSkillCalc)
	{
		return MISSILE_GetSpecialParamValue(NULL, pSkillCalc->pUnit, nParamId, nMissileId, pSkillCalc->nSkillLevel);
	}

	return 0;
}

//D2Common.0x6FDAF7A0
int __fastcall sub_6FDAF7A0(int nSkillId, uint8_t a2, uint8_t a3, D2SkillCalcStrc* pSkillCalc)
{
	if (pSkillCalc)
	{
		return SKILLS_GetSpecialParamValue(pSkillCalc->pUnit, a3, nSkillId, SKILLS_GetSpecialParamValue(pSkillCalc->pUnit, a2, pSkillCalc->nSkillId, pSkillCalc->nSkillLevel));
	}

	return 0;
}

//D2Common.0x6FDAF7E0 (#11276)
int __stdcall SKILLS_EvaluateSkillFormula(D2UnitStrc* pUnit, unsigned int nCalc, int nSkillId, int nSkillLevel)
{
	D2SkillCalcStrc pSkillCalc = {};

	if (sgptDataTables->pSkillsCode && nCalc < sgptDataTables->nSkillsCodeSize)
	{
		pSkillCalc.pUnit = pUnit;
		pSkillCalc.nSkillId = nSkillId;
		pSkillCalc.nSkillLevel = nSkillLevel;

		return FOG_10253(&sgptDataTables->pSkillsCode[nCalc], sgptDataTables->nSkillsCodeSize - nCalc, sub_6FDAF6A0, off_6FDE5804, dword_6FDE583C, &pSkillCalc);
	}

	return 0;
}

//D2Common.0x6FDAF850 (#11302)
int __stdcall SKILLS_EvaluateSkillDescFormula(D2UnitStrc* pUnit, unsigned int nCalc, int nSkillId, int nSkillLevel)
{
	D2SkillCalcStrc pSkillCalc = {};

	if (sgptDataTables->pSkillDescCode && nCalc < sgptDataTables->nSkillDescCodeSize)
	{
		pSkillCalc.pUnit = pUnit;
		pSkillCalc.nSkillId = nSkillId;
		pSkillCalc.nSkillLevel = nSkillLevel;

		return FOG_10253(&sgptDataTables->pSkillDescCode[nCalc], sgptDataTables->nSkillDescCodeSize - nCalc, sub_6FDAF6A0, off_6FDE5804, dword_6FDE583C, &pSkillCalc);
	}

	return 0;
}

//D2Common.0x6FDAF8C0 (#10940)
void __stdcall SKILLS_RefreshSkill(D2UnitStrc* pUnit, int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2StatListStrc* pStatList = NULL;
	D2SkillStrc* pSkill = NULL;
	int nSkillLevel = 0;
	int nValue = 0;
	short nPassiveState = 0;

	if (pUnit)
	{
		pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
		if (pSkillsTxtRecord)
		{
			nPassiveState = pSkillsTxtRecord->nPassiveState;
			if (nPassiveState > 0 && nPassiveState < sgptDataTables->nStatesTxtRecordCount)
			{
				pSkill = SKILLS_GetHighestLevelSkillFromSkillId(pUnit, nSkillId);
				if (!pSkill || pSkillsTxtRecord->nAuraState > 0 && STATES_CheckState(pUnit, pSkillsTxtRecord->nAuraState))
				{
					pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nPassiveState);
					if (pStatList)
					{
						D2Common_10474(pUnit, (D2StatListExStrc*)pStatList);
						STATLIST_FreeStatList(pStatList);
					}
					return;
				}

				nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, TRUE);

				pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, pSkillsTxtRecord->nPassiveState);

				if (!nSkillLevel)
				{
					if (pStatList)
					{
						D2Common_10474(pUnit, (D2StatListExStrc*)pStatList);
						STATLIST_FreeStatList(pStatList);
					}
					return;
				}

				if (!pStatList)
				{
					pStatList = STATLIST_AllocStatList(pUnit->pMemoryPool, 0, 0, pUnit->dwUnitType, pUnit->dwUnitId);
					STATLIST_SetState(pStatList, pSkillsTxtRecord->nPassiveState);
					D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);
				}

				if (pStatList)
				{
					if (STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0) != nSkillLevel)
					{
						for (int i = 0; i < ARRAY_SIZE(pSkillsTxtRecord->nPassiveStat); ++i)
						{
							if (pSkillsTxtRecord->nPassiveStat[i] < 0 || pSkillsTxtRecord->nPassiveStat[i] >= sgptDataTables->nItemStatCostTxtRecordCount)
							{
								break;
							}

							nValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[i], nSkillId, nSkillLevel);

							if (pStatList)
							{
								if (pSkillsTxtRecord->nPassiveIType > 0)
								{
									STATLIST_SetStat(pStatList, pSkillsTxtRecord->nPassiveStat[i], nValue, pSkillsTxtRecord->nPassiveIType);
								}
								else
								{
									STATLIST_SetStat(pStatList, pSkillsTxtRecord->nPassiveStat[i], nValue, 0);
								}
							}
						}

						if (pStatList)
						{
							STATLIST_SetStat(pStatList, STAT_MODIFIERLIST_SKILL, nSkillId, 0);
							STATLIST_SetStat(pStatList, STAT_MODIFIERLIST_LEVEL, nSkillLevel, 0);
						}

						STATES_ToggleGfxStateFlag(pUnit, pSkillsTxtRecord->nPassiveState, TRUE);
					}
				}
			}
		}
	}
}

//D2Common.0x6FDAFB40 (#10941)
void __stdcall SKILLS_RefreshPassiveSkills(D2UnitStrc* pUnit)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2SkillStrc* pSkill = NULL;
	int nSkillId = 0;
	short nPassiveState = 0;

	for (int i = 0; i < sgptDataTables->nPassiveSkills; ++i)
	{
		nSkillId = sgptDataTables->pPassiveSkills[i];

		pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
		if (pSkillsTxtRecord)
		{
			nPassiveState = pSkillsTxtRecord->nPassiveState;
			if (nPassiveState > 0)
			{
				pSkill = SKILLS_GetHighestLevelSkillFromSkillId(pUnit, nSkillId);

				if (pSkill && STATES_CheckState(pUnit, nPassiveState))
				{
					SKILLS_RefreshSkill(pUnit, nSkillId);
				}
			}
		}
	}
}

//D2Common.0x6FDAFC30
D2SkillStrc* __fastcall SKILLS_GetHighestLevelSkillFromSkillId(D2UnitStrc* pUnit, int nSkillId)
{
	D2SkillStrc* pHighestLevelSkill = NULL;

	if (pUnit->pSkills)
	{
		for (D2SkillStrc* pSkill = pUnit->pSkills->pFirstSkill; pSkill; pSkill = pSkill->pNextSkill)
		{
			if (pSkill->pSkillsTxt->nSkillId == nSkillId && (!pHighestLevelSkill || pSkill->nOwnerGUID == D2UnitInvalidGUID || pHighestLevelSkill->nOwnerGUID != D2UnitInvalidGUID && pSkill->nSkillLevel > pHighestLevelSkill->nSkillLevel))
			{
				pHighestLevelSkill = pSkill;
			}
		}
	}

	return pHighestLevelSkill;
}

//D2Common.0x6FDAFC80 (#10942)
int __stdcall SKILLS_GetSkillModeFromUnit(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	if (pUnit && pSkill)
	{
		return pSkill->dwSkillMode;
	}

	return 0;
}

//D2Common.0x6FDAFCA0 (#11049)
int __stdcall SKILLS_Return1(int a1)
{
	REMOVE_LATER_WriteToLogFile("SKILLS_Return1: Useless");
	return 1;
}

//D2Common.0x6FDAFCB0 (#10944)
int __stdcall SKILLS_GetRange(D2SkillStrc* pSkill)
{
	if (pSkill && pSkill->pSkillsTxt)
	{
		return pSkill->pSkillsTxt->nRange;
	}

	return 0;
}

//D2Common.0x6FDAFCD0 (#10945)
D2SkillListStrc* __stdcall SKILLS_AllocSkillList(void* pMemPool)
{
	D2SkillListStrc* pSkillList = (D2SkillListStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2SkillListStrc), __FILE__, __LINE__, 0);
	memset(pSkillList, 0x00, sizeof(D2SkillListStrc));

	pSkillList->pMemPool = pMemPool;

	return pSkillList;
}

//D2Common.0x6FDAFD10 (#10946)
void __stdcall SKILLS_InitSkillList(D2UnitStrc* pUnit)
{
	D2CharStatsTxt* pCharStatsTxtRecord = NULL;
	int nCounter = 0;
	int nClass = 0;

	if (!SKILLS_GetSkill(pUnit, 0, -1))
	{
		nClass = pUnit->dwClassId;
		if (nClass < 0 || nClass >= sgptDataTables->nCharStatsTxtRecordCount)
		{
			return;
		}

		pCharStatsTxtRecord = &sgptDataTables->pCharStatsTxt[nClass];
		if (!pCharStatsTxtRecord)
		{
			return;
		}

		SKILLS_AddSkill(pUnit, 0);

		do
		{
			if (DATATBLS_GetSkillsTxtRecord(pCharStatsTxtRecord->nBaseSkill[nCounter]))
			{
				SKILLS_AddSkill(pUnit, pCharStatsTxtRecord->nBaseSkill[nCounter]);
			}

			++nCounter;
		}
		while (nCounter < 10);
	}

	if (!UNITS_GetLeftSkill(pUnit))
	{
		SKILLS_SetLeftActiveSkill(pUnit, 0, -1);
	}

	if (!UNITS_GetRightSkill(pUnit))
	{
		SKILLS_SetRightActiveSkill(pUnit, 0, -1);
	}
}

//D2Common.0x6FDAFEA0 (#10947)
D2SkillStrc* __stdcall SKILLS_GetNextSkill(D2SkillStrc* pSkill)
{
	if (pSkill)
	{
		return pSkill->pNextSkill;
	}

	REMOVE_LATER_WriteToLogFile("SKILLS_GetNextSkill: NULL pointer");
	return NULL;
}

//D2Common.0x6FDAFEA0 (#10948)
D2SkillStrc* __stdcall SKILLS_GetFirstSkillFromSkillList(D2SkillListStrc* pSkillList)
{
	if (pSkillList)
	{
		return pSkillList->pFirstSkill;
	}

	REMOVE_LATER_WriteToLogFile("SKILLS_GetFirstSkillFromSkillList: NULL pointer");
	return NULL;
}

//D2Common.0x6FDAFEC0
D2SkillStrc* __fastcall SKILLS_GetLeftSkillFromSkillList(D2SkillListStrc* pSkillList)
{
	D2_ASSERT(pSkillList);

	return pSkillList->pLeftSkill;
}

//D2Common.0x6FDAFEF0
D2SkillStrc* __fastcall SKILLS_GetRightSkillFromSkillList(D2SkillListStrc* pSkillList)
{
	D2_ASSERT(pSkillList);

	return pSkillList->pRightSkill;
}

//D2Common.0x6FDAFF20
void __fastcall SKILLS_SetUsedSkillInSkillList(D2SkillListStrc* pSkillList, D2SkillStrc* pUsedSkill)
{
	if (pSkillList)
	{
		pSkillList->pUsedSkill = pUsedSkill;
	}
}

//D2Common.0x6FDAFF30
D2SkillStrc* __fastcall SKILLS_GetUsedSkillFromSkillList(D2SkillListStrc* pSkillList)
{
	if (pSkillList)
	{
		return pSkillList->pUsedSkill;
	}

	return NULL;
}

//D2Common.0x6FDAFF40 (#10949)
D2SkillStrc* __fastcall SKILLS_GetSkillById(D2UnitStrc* pUnit, int nSkillId, D2UnitGUID nOwnerGUID)
{
	return SKILLS_GetSkill(pUnit, nSkillId, nOwnerGUID);
}

//D2Common.0x6FDAFF80 (#10950)
D2SkillStrc* __fastcall SKILLS_GetHighestLevelSkillFromUnitAndId(D2UnitStrc* pUnit, int nSkillId)
{
	return SKILLS_GetHighestLevelSkillFromSkillId(pUnit, nSkillId);
}

//D2Common.0x6FDAFFD0 (#10951)
void __stdcall SKILLS_RemoveSkill(D2UnitStrc* pUnit, int nSkillId, char* szFile, int nLine)
{
	D2COMMON_SKILLS_RemoveSkill_6FDAFFF0(pUnit, nSkillId, 1, szFile, nLine);
}

//D2Common.0x6FDAFFF0
void __fastcall D2COMMON_SKILLS_RemoveSkill_6FDAFFF0(D2UnitStrc* pUnit, int nSkillId, BOOL bDecrementAndCheckSkillLevel, char* szFile, int nLine)
{
	if (!pUnit)
	{
		FOG_WriteToLogFile("sSkillsRemoveSkill(): NULL unit  FILE:%s  LINE:%d", szFile, nLine);
		return;
	}
	if (!pUnit->pSkills)
	{
		FOG_WriteToLogFile("sSkillsRemoveSkill(): NULL skillinfo  (TYPE:%d  CLASS:%d)  FILE:%s  LINE:%d", pUnit->dwUnitType, pUnit->dwClassId, szFile, nLine);
		return;
	}
	if (!pUnit->pSkills->pFirstSkill)
	{
		FOG_WriteToLogFile("sSkillsRemoveSkill(): NULL skilllist  (TYPE:%d  CLASS:%d)  FILE:%s  LINE:%d", pUnit->dwUnitType, pUnit->dwClassId, szFile, nLine);
		return;
	}

	if (D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId))
	{
		if (pSkillsTxtRecord->nPassiveState > 0)
		{
			STATES_ToggleState(pUnit, pSkillsTxtRecord->nPassiveState, 0);
		}
	}

	if (const D2SkillStrc* pLeftSkill = pUnit->pSkills->pLeftSkill)
	{
		if (nSkillId == pLeftSkill->pSkillsTxt->nSkillId && pLeftSkill->nOwnerGUID == D2UnitInvalidGUID)
		{
			SKILLS_SetLeftActiveSkill(pUnit, SKILL_ATTACK, -1);
		}
	}

	if (const D2SkillStrc* pRightSkill = pUnit->pSkills->pRightSkill)
	{
		if (nSkillId == pRightSkill->pSkillsTxt->nSkillId && pRightSkill->nOwnerGUID == D2UnitInvalidGUID)
		{
			SKILLS_SetRightActiveSkill(pUnit, SKILL_ATTACK, -1);
		}
	}

	if (const D2SkillStrc* pUsedSkill = pUnit->pSkills->pUsedSkill)
	{
		if (nSkillId == pUsedSkill->pSkillsTxt->nSkillId && pUsedSkill->nOwnerGUID == D2UnitInvalidGUID)
		{
			SKILLS_SetUsedSkillInSkillList(pUnit->pSkills, nullptr);
		}
	}

	// Try to find the skill in the unit skills list
	D2SkillStrc* pPreviousSkill = nullptr;
	D2SkillStrc* pSkill = pUnit->pSkills->pFirstSkill;
	while (pSkill && !(pSkill->pSkillsTxt->nSkillId == nSkillId && pSkill->nOwnerGUID == D2UnitInvalidGUID))
	{
		pPreviousSkill = pSkill;
		pSkill = pSkill->pNextSkill;
	}

	if (pSkill)
	{
		if (!bDecrementAndCheckSkillLevel || (--pSkill->nSkillLevel) <= 0)
		{
			if (pPreviousSkill)
			{
				pPreviousSkill->pNextSkill = pSkill->pNextSkill;
			}
			else
			{
				pUnit->pSkills->pFirstSkill = pSkill->pNextSkill;
			}

			FOG_FreeServerMemory(pUnit->pMemoryPool, pSkill, __FILE__, __LINE__, 0);
		}
	}

	SKILLS_RefreshSkill(pUnit, nSkillId);
}

//D2Common.0x6FDB0270 (#10958)
void* __stdcall D2Common_10958(D2UnitStrc* pUnit, void* a2)
{
	void* pResult = NULL;

	if (pUnit && pUnit->pSkills)
	{
		pResult = (void*)pUnit->pSkills->__014;
		pUnit->pSkills->__014 = (int)a2;
	}

	return pResult;
}

//D2Common.0x6FDB02A0 (#10959)
void* __stdcall D2Common_10959(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pSkills)
	{
		return (void*)pUnit->pSkills->__014;
	}

	return NULL;
}

//D2Common.0x6FDB02C0 (#10960)
void __stdcall SKILLS_FreeSkillList(D2UnitStrc* pUnit)
{
	D2SkillListStrc* pSkillList = NULL;
	D2SkillStrc* pNextSkill = NULL;
	D2SkillStrc* pSkill = NULL;

	if (pUnit)
	{
		pSkillList = pUnit->pSkills;
		if (pSkillList)
		{
			pSkill = pSkillList->pFirstSkill;
			while (pSkill)
			{
				pNextSkill = pSkill->pNextSkill;
				FOG_FreeServerMemory(pUnit->pMemoryPool, pSkill, __FILE__, __LINE__, 0);
				pSkill = pNextSkill;
			}

			FOG_FreeServerMemory(pUnit->pMemoryPool, pSkillList, __FILE__, __LINE__, 0);
			pUnit->pSkills = NULL;
		}
	}
}

//D2Common.0x6FDB0320 (#10952)
D2SkillStrc* __stdcall SKILLS_AddSkill(D2UnitStrc* pUnit, int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2SkillStrc* pPreviousSkill = NULL;
	D2SkillStrc* pSkill = NULL;
	int nMaxSkillLevel = 0;

	if (pUnit)
	{
		pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
		if (pSkillsTxtRecord)
		{
			if (pSkillsTxtRecord->nPassiveState > 0)
			{
				STATES_ToggleState(pUnit, pSkillsTxtRecord->nPassiveState, TRUE);
			}

			pSkill = UNITS_GetStartSkill(pUnit);
			if (pSkill)
			{
				while (pSkill->pSkillsTxt != pSkillsTxtRecord || pSkill->nOwnerGUID != D2UnitInvalidGUID)
				{
					pSkill = pSkill->pNextSkill;
					if (!pSkill)
					{
						break;
					}
				}

				if (pSkill)
				{
					nMaxSkillLevel = pSkillsTxtRecord->wMaxLvl;
					if (nMaxSkillLevel <= 0)
					{
						nMaxSkillLevel = 20;
					}

					if (pSkill->nSkillLevel < nMaxSkillLevel || pUnit->dwUnitType == UNIT_MONSTER)
					{
						++pSkill->nSkillLevel;
					}

					SKILLS_RefreshSkill(pUnit, nSkillId);

					return pSkill;
				}
			}

			if (pUnit->pSkills)
			{
				pSkill = (D2SkillStrc*)FOG_AllocServerMemory(pUnit->pSkills->pMemPool, sizeof(D2SkillStrc), __FILE__, __LINE__, 0);
				memset(pSkill, 0x00, sizeof(D2SkillStrc));
				pSkill->pSkillsTxt = pSkillsTxtRecord;

				if (pUnit->dwUnitType == UNIT_MONSTER)
				{
					pSkill->dwSkillMode = pSkillsTxtRecord->nMonAnim;
				}
				else
				{
					if (pUnit->dwUnitType != UNIT_PLAYER || pUnit->dwClassId != PCLASS_ASSASSIN || nSkillId != SKILL_LEFTHANDSWING)
					{
						pSkill->dwSkillMode = pSkillsTxtRecord->nAnim;
					}
					else
					{
						pSkill->dwSkillMode = 16;
					}
				}
				pSkill->nSkillLevel = 1;
				pSkill->nOwnerGUID = -1;
				pSkill->pNextSkill = NULL;

				pPreviousSkill = pUnit->pSkills->pFirstSkill;
				if (pPreviousSkill)
				{
					for (D2SkillStrc* i = pPreviousSkill->pNextSkill; i; i = i->pNextSkill)
					{
						pPreviousSkill = i;
					}
					pPreviousSkill->pNextSkill = pSkill;
				}
				else
				{
					pUnit->pSkills->pFirstSkill = pSkill;
				}

				SKILLS_RefreshSkill(pUnit, nSkillId);

				return pSkill;
			}
		}
	}

	return NULL;
}

//D2Common.0x6FDB04D0 (#10953)
void __stdcall SKILLS_AssignSkill(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL bRemove, char* szFile, int nLine)
{
	D2SkillStrc* pSkill = NULL;

	if (nSkillLevel)
	{
		pSkill = SKILLS_GetSkill(pUnit, nSkillId, -1);

		if (!pSkill)
		{
			pSkill = SKILLS_AddSkill(pUnit, nSkillId);
		}

		if (pSkill)
		{
			pSkill->nSkillLevel = nSkillLevel;
		}

		SKILLS_RefreshSkill(pUnit, nSkillId);
	}
	else
	{
		if (bRemove)
		{
			D2COMMON_SKILLS_RemoveSkill_6FDAFFF0(pUnit, nSkillId, 0, szFile, nLine);
			SKILLS_RefreshSkill(pUnit, nSkillId);
		}
		else
		{
			pSkill = SKILLS_GetSkill(pUnit, nSkillId, -1);

			if (!pSkill)
			{
				SKILLS_AddSkill(pUnit, nSkillId);
				pSkill = SKILLS_GetSkill(pUnit, nSkillId, -1);
			}

			if (pSkill)
			{
				pSkill->nSkillLevel = 0;
				SKILLS_RefreshSkill(pUnit, nSkillId);
			}
		}
	}
}

//D2Common.0x6FDB05E0 (#10954)
void __stdcall D2Common_10954(D2UnitStrc* pUnit, D2UnitGUID nOwnerGUID, int nSkillId, int nSkillLevel, int nCharges, BOOL bFreeMemory)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2SkillListStrc* pSkillList = NULL;
	D2SkillStrc* pPreviousSkill = NULL;
	D2SkillStrc* pSkill = NULL;

	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && nSkillLevel > 0)
	{
		pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
		if (pSkillsTxtRecord)
		{
			pSkillList = pUnit->pSkills;
			if (pSkillList)
			{
				if (bFreeMemory)
				{
					if (nSkillId == pSkillList->pLeftSkill->pSkillsTxt->nSkillId && pSkillList->pLeftSkill->nOwnerGUID == nOwnerGUID)
					{
						SKILLS_SetLeftActiveSkill(pUnit, 0, -1);
					}

					if (nSkillId == pSkillList->pRightSkill->pSkillsTxt->nSkillId && pSkillList->pRightSkill->nOwnerGUID == nOwnerGUID)
					{
						SKILLS_SetRightActiveSkill(pUnit, 0, -1);
					}

					if (pSkillList->pUsedSkill && nSkillId == SKILLS_GetSkillIdFromSkill(pSkillList->pUsedSkill, __FILE__, __LINE__))
					{
						pSkillList->pUsedSkill = NULL;
					}

					pSkill = pSkillList->pFirstSkill;
					pPreviousSkill = NULL;

					if (pSkill)
					{
						while (pSkill->nOwnerGUID != nOwnerGUID || pSkill->pSkillsTxt != pSkillsTxtRecord)
						{
							pPreviousSkill = pSkill;
							pSkill = pSkill->pNextSkill;
							if (!pSkill)
							{
								return;
							}
						}

						if (pPreviousSkill)
						{
							pPreviousSkill->pNextSkill = pSkill->pNextSkill;
						}
						else
						{
							pSkillList->pFirstSkill = pSkill->pNextSkill;
						}

						FOG_FreeServerMemory(pUnit->pMemoryPool, pSkill, __FILE__, __LINE__, 0);
					}
				}
				else
				{
					if (pSkillList->pFirstSkill)
					{
						pSkill = pSkillList->pFirstSkill;
						pPreviousSkill = NULL;

						while (pSkill && (pSkill->nOwnerGUID != nOwnerGUID || pSkill->pSkillsTxt != pSkillsTxtRecord))
						{
							pPreviousSkill = pSkill;
							pSkill = pSkill->pNextSkill;
						}
					}

					if (pSkill)
					{
						pSkill->nSkillLevel = nSkillLevel;
						pSkill->nCharges = nCharges;
					}
					else
					{
						pSkill = (D2SkillStrc*)FOG_AllocServerMemory(pSkillList->pMemPool, sizeof(D2SkillStrc), __FILE__, __LINE__, 0);
						memset(pSkill, 0x00, sizeof(D2SkillStrc));
						pSkill->pSkillsTxt = pSkillsTxtRecord;

						switch (pSkillsTxtRecord->nAnim)
						{
						case PLRMODE_ATTACK1:
						case PLRMODE_ATTACK2:
						case PLRMODE_CAST:
						case PLRMODE_THROW:
						case PLRMODE_SEQUENCE:
							pSkill->dwSkillMode = pSkillsTxtRecord->nAnim;
							break;
						default:
							pSkill->dwSkillMode = PLRMODE_CAST;
							break;
						}

						pSkill->nSkillLevel = nSkillLevel;
						pSkill->nCharges = nCharges;
						pSkill->nOwnerGUID = nOwnerGUID;
						pSkill->pNextSkill = NULL;

						if (pPreviousSkill)
						{
							pPreviousSkill->pNextSkill = pSkill;
						}
						else
						{
							pSkillList->pFirstSkill = pSkill;
						}
					}
				}
			}
		}
	}
}

//D2Common.0x6FDB08C0 (#10957)
D2UnitGUID __stdcall SKILLS_GetOwnerGUIDFromSkill(D2SkillStrc* pSkill)
{
	D2_ASSERT(pSkill);

	return pSkill->nOwnerGUID;
}

//D2Common.0x6FDB08F0 (#10955)
BOOL __stdcall SKILLS_GetSkillInfo(D2SkillStrc* pSkill, D2UnitGUID* pOwnerGUID, int* pSkillId, int* pSkillLevel, int* pCharges)
{
	D2_ASSERT(pSkill);

	if (pSkill->nOwnerGUID == D2UnitInvalidGUID)
	{
		return FALSE;
	}
	else
	{
		if (pOwnerGUID)
		{
			*pOwnerGUID = pSkill->nOwnerGUID;
		}
		if (pSkillId)
		{
			*pSkillId = pSkill->pSkillsTxt->nSkillId;
		}
		if (pSkillLevel)
		{
			*pSkillLevel = pSkill->nSkillLevel;
		}
		if (pCharges)
		{
			*pCharges = pSkill->nCharges;
		}

		return TRUE;
	}
}

//D2Common.0x6FDB0960 (#10956)
BOOL __stdcall SKILLS_SetCharges(D2SkillStrc* pSkill, int nCharges)
{
	D2_ASSERT(pSkill);

	if (pSkill->nOwnerGUID == D2UnitInvalidGUID)
	{
		return FALSE;
	}
	else
	{
		pSkill->nCharges = nCharges;
		return TRUE;
	}
}

//D2Common.0x6FDB09A0 (#10961)
void __stdcall SKILLS_SetLeftActiveSkill(D2UnitStrc* pUnit, int nSkillId, D2UnitGUID nOwnerGUID)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
	D2_ASSERT(pSkillsTxtRecord);

	if (D2SkillStrc* pSkill = UNITS_GetStartSkill(pUnit))
	{
		bool bSkillFound = false;
		while (!bSkillFound)
		{
			if (pSkill->pSkillsTxt != pSkillsTxtRecord || pSkill->nOwnerGUID != nOwnerGUID)
			{
				pSkill = pSkill->pNextSkill;
			}
			else
			{
				bSkillFound = true;
			}

			if (!pSkill)
			{
				if (!bSkillFound)
				{
					return;
				}
				break;
			}
		}

		pUnit->pSkills->pLeftSkill = pSkill;
	}
}

//D2Common.0x6FDB0A30 (#10962)
void __stdcall SKILLS_SetRightActiveSkill(D2UnitStrc* pUnit, int nSkillId, D2UnitGUID nOwnerGUID)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2SkillStrc* pSkill = NULL;
	BOOL bSkillFound = FALSE;

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
	D2_ASSERT(pSkillsTxtRecord);

	pSkill = UNITS_GetStartSkill(pUnit);

	if (pSkill)
	{
		while (!bSkillFound)
		{
			if (pSkill->pSkillsTxt != pSkillsTxtRecord || pSkill->nOwnerGUID != nOwnerGUID)
			{
				pSkill = pSkill->pNextSkill;
			}
			else
			{
				bSkillFound = TRUE;
			}

			if (!pSkill)
			{
				if (!bSkillFound)
				{
					return;
				}
				break;
			}
		}

		pUnit->pSkills->pRightSkill = pSkill;
	}
}

//D2Common.0x6FDB0AC0 (#10963)
int __stdcall SKILLS_GetSkillIdFromSkill(D2SkillStrc* pSkill, char* szFile, int nLine)
{
	if (pSkill)
	{
		return pSkill->pSkillsTxt->nSkillId;
	}
	else
	{
		FOG_WriteToLogFile("Null skill passed to SkillsGetType() from FILE:%s  LINE:%d", szFile, nLine);
		return 0;
	}
}

//D2Common.0x6FDB0AF0 (#10965)
int __fastcall SKILLS_GetSeqNumFromSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nCounter = 0;

	if (pUnit && pSkill)
	{
		if (!pUnit->dwUnitType)
		{
			return pSkill->pSkillsTxt->nSeqNum;
		}

		if (pSkill->pSkillsTxt->nSkillId != SKILL_DIABPRISON)
		{
			pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
			if (pMonStatsTxtRecord)
			{
				while (pMonStatsTxtRecord->nSkill[nCounter] != pSkill->pSkillsTxt->nSkillId)
				{
					++nCounter;

					if (nCounter >= ARRAY_SIZE(pMonStatsTxtRecord->nSkill))
					{
						return 0;
					}
				}

				return pMonStatsTxtRecord->nSequence[nCounter];
			}
		}
	}

	return 0;
}

//D2Common.0x6FDB0B70 (#10964)
int __stdcall SKILLS_GetUseState(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	D2UnitStrc* pRightHandItem = NULL;
	D2UnitStrc* pLeftHandItem = NULL;
	D2UnitStrc* pItem = NULL;
	BOOL bIsLeftHandItem = FALSE;
	BOOL bContinue = FALSE;
	int nSkillId = 0;
	int nBodyLoc = 0;

	D2_ASSERT(pSkill);

	if (!pSkill->pSkillsTxt || !(pSkill->pSkillsTxt->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_INGAME]))
	{
		return SKILLUSTATE_DISABLED;
	}

	if (!SKILLS_GetSkillLevel(pUnit, pSkill, TRUE))
	{
		return SKILLUSTATE_NOLEVEL;
	}

	if (pSkill->pSkillsTxt->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_AURA])
	{
		return SKILLUSTATE_AURA;
	}

	if (pSkill->pSkillsTxt->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_PASSIVE])
	{
		return SKILLUSTATE_PASSIVE;
	}

	if (pSkill->pSkillsTxt->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_SCROLL])
	{
		if (!pUnit->pInventory || pSkill->nQuantity <= 0)
		{
			return SKILLUSTATE_NOQUANTITY;
		}
	}
	else
	{
		nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
		if (nSkillId == SKILL_THROW)
		{
			bContinue = 0;
			if (!pUnit->pInventory)
			{
				return SKILLUSTATE_NOQUANTITY;
			}

			if (UNITS_CanDualWield(pUnit))
			{
				if (INVENTORY_GetEquippedWeapon(pUnit->pInventory, &pItem, &nBodyLoc, &bIsLeftHandItem))
				{
					if (!bIsLeftHandItem)
					{
						pItem = sub_6FDB1050(pUnit->pInventory, nBodyLoc);
					}

					if (pItem)
					{
						if (ITEMS_CheckIfThrowable(pItem))
						{
							bContinue = 1;
						}
						else
						{
							bContinue = STATLIST_GetUnitStatSigned(pItem, STAT_ITEM_THROWABLE, 0);
						}
					}
				}
			}
			else
			{
				if (INVENTORY_GetEquippedWeapon(pUnit->pInventory, &pItem, &nBodyLoc, &bIsLeftHandItem) && bIsLeftHandItem)
				{
					if (ITEMS_CheckIfThrowable(pItem))
					{
						bContinue = 1;
					}
					else
					{
						bContinue = STATLIST_GetUnitStatSigned(pItem, STAT_ITEM_THROWABLE, 0);
					}
				}
			}

			if (!bContinue)
			{
				return SKILLUSTATE_NOQUANTITY;
			}
		}
		else if (nSkillId == SKILL_LEFTHANDTHROW)
		{
			bContinue = 0;
			if (!pUnit->pInventory)
			{
				return SKILLUSTATE_NOQUANTITY;
			}

			if (INVENTORY_GetEquippedWeapon(pUnit->pInventory, &pItem, &nBodyLoc, &bIsLeftHandItem))
			{
				if (bIsLeftHandItem)
				{
					pItem = sub_6FDB1050(pUnit->pInventory, nBodyLoc);
					if (pItem)
					{
						if (ITEMS_CheckIfThrowable(pItem))
						{
							bContinue = 1;
						}
						else
						{
							bContinue = STATLIST_GetUnitStatSigned(pItem, STAT_ITEM_THROWABLE, 0);
						}
					}
				}
				else if (!ITEMS_CheckIfThrowable(pItem))
				{
					bContinue = STATLIST_GetUnitStatSigned(pItem, STAT_ITEM_THROWABLE, 0);
				}
			}

			if (!bContinue)
			{
				return SKILLUSTATE_NOQUANTITY;
			}
		}
		else if (nSkillId == SKILL_LEFTHANDSWING)
		{
			if (!pUnit->pInventory || !UNITS_CanDualWield(pUnit))
			{
				return SKILLUSTATE_NOQUANTITY;
			}

			bContinue = 0;
			if (INVENTORY_GetEquippedWeapon(pUnit->pInventory, &pItem, &nBodyLoc, &bIsLeftHandItem))
			{
				if (bIsLeftHandItem)
				{
					pItem = sub_6FDB1050(pUnit->pInventory, nBodyLoc);
					if (pItem)
					{
						bContinue = ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON);
					}
				}
				else
				{
					bContinue = ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON);
				}
			}

			if (!bContinue)
			{
				return SKILLUSTATE_NOQUANTITY;
			}
		}
	}

	if (pSkill->pSkillsTxt->nETypeA[0] > 0 || pSkill->pSkillsTxt->nITypeA[0] > 0)
	{
		if (!pUnit->pInventory)
		{
			return SKILLUSTATE_NOQUANTITY;
		}

		pRightHandItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
		pLeftHandItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);

		if (pSkill->pSkillsTxt->nSkillId == SKILL_LEFTHANDTHROW || pSkill->pSkillsTxt->nSkillId == SKILL_LEFTHANDSWING)
		{
			pItem = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
			if (pItem == pRightHandItem)
			{
				pRightHandItem = NULL;
			}
			else if (pItem == pLeftHandItem)
			{
				pLeftHandItem = NULL;
			}
		}

		if (sub_6FDB1130(pRightHandItem, pLeftHandItem, pSkill->pSkillsTxt, 0))
		{
			if (!sub_6FDB1130(pLeftHandItem, pRightHandItem, pSkill->pSkillsTxt, 1))
			{
				return SKILLUSTATE_NOQUANTITY;
			}
		}
		else if (sub_6FDB1130(pRightHandItem, pLeftHandItem, pSkill->pSkillsTxt, 1))
		{
			if (!sub_6FDB1130(pLeftHandItem, pRightHandItem, pSkill->pSkillsTxt, 0))
			{
				return SKILLUSTATE_NOQUANTITY;
			}
		}
		else
		{
			return SKILLUSTATE_NOQUANTITY;
		}
	}

	if (!D2Common_SKILLMANA_CheckStat_6FDB0F50(pUnit, pSkill))
	{
		return SKILLUSTATE_NOMANA;
	}

	if (!D2Common_SKILLS_CheckShapeRestriction_6FDB1380(pUnit, pSkill))
	{
		return SKILLUSTATE_SHAPERESTRICT;
	}

	if (!D2Common_SKILLMANA_CheckStartStat_6FDB1400(pUnit, pSkill))
	{
		return SKILLUSTATE_NOMANA;
	}

	if (!sub_6FDB1070(pUnit, pSkill))
	{
		return SKILLUSTATE_NOQUANTITY;
	}

	if (!SKILLS_CheckSkillDelay(pUnit, pSkill))
	{
		return SKILLUSTATE_BLOCKED;
	}
	
	return SKILLUSTATE_USABLE;
}

//D2Common.0x6FDB0F50
//TODO: Check name
BOOL __fastcall D2Common_SKILLMANA_CheckStat_6FDB0F50(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	int nRequiredMana = 0;
	int nSkillLevel = 0;

	D2_ASSERT(pSkill);

	if (pSkill->nOwnerGUID == D2UnitInvalidGUID)
	{
		nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, TRUE) - 1;
		D2_ASSERT(pSkill->pSkillsTxt);

		if (nSkillLevel < 0)
		{
			nSkillLevel = 0;
		}

		nRequiredMana = (pSkill->pSkillsTxt->wMana + nSkillLevel * pSkill->pSkillsTxt->wLevelMana) << pSkill->pSkillsTxt->nManaShift;
		if (STATES_CheckState(pUnit, STATE_BLOOD_MANA))
		{
			return STATLIST_GetUnitStatUnsigned(pUnit, STAT_HITPOINTS, 0) >= nRequiredMana;
		}
		else
		{
			if (pSkill->pSkillsTxt->wSrvDoFunc == 116 && STATES_IsUnitShapeShifted(pUnit))
			{
				return TRUE;
			}
			else
			{
				return STATLIST_GetUnitStatUnsigned(pUnit, STAT_MANA, 0) >= nRequiredMana;
			}
		}
	}
	else
	{
		return pSkill->nCharges > 0;
	}
}

//D2Common.0x6FDB1050
D2UnitStrc* __fastcall sub_6FDB1050(D2InventoryStrc* pInventory, int nBodyLoc)
{
	if (nBodyLoc == BODYLOC_LARM)
	{
		return INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_RARM);
	}
	else
	{
		return INVENTORY_GetItemFromBodyLoc(pInventory, BODYLOC_LARM);
	}
}

//D2Common.0x6FDB1070
BOOL __fastcall sub_6FDB1070(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	D2UnitStrc* pItem = NULL;
	int nSkillLevel = 0;

	if (pUnit && pSkill && pUnit->pInventory)
	{
		if (pSkill->nOwnerGUID != D2UnitInvalidGUID)
		{
			pItem = INVENTORY_GetFirstItem(pUnit->pInventory);

			while (pItem)
			{
				if (INVENTORY_UnitIsItem(pItem) && pItem->dwUnitId == pSkill->nOwnerGUID)
				{
					break;
				}

				pItem = INVENTORY_GetNextItem(pItem);
			}

			if (pItem)
			{
				nSkillLevel = pSkill->nSkillLevel;

				if (nSkillLevel < 0)
				{
					nSkillLevel = 0;
				}

				if (nSkillLevel > DATATBLS_GetMaxLevel(0))
				{
					nSkillLevel = DATATBLS_GetMaxLevel(0);
				}

				return STATLIST_GetUnitStatUnsigned(pItem, STAT_ITEM_CHARGED_SKILL, (pSkill->pSkillsTxt->nSkillId << 6) + (nSkillLevel & 0x3F)) > 0;
			}
		}

		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDB1130
BOOL __fastcall sub_6FDB1130(D2UnitStrc* pItem, D2UnitStrc* pUnit, D2SkillsTxt* pSkillsTxtRecord, int nType)
{
	short* pEType = NULL;
	short* pIType = NULL;
	int i = 0;

	if (!pItem)
	{
		if (pSkillsTxtRecord->nETypeA[2 * nType] <= 0 && pSkillsTxtRecord->nITypeA[3 * nType] <= 0)
		{
			return TRUE;
		}

		if ((pSkillsTxtRecord->nITypeA[0] == ITEMTYPE_WEAPON || pSkillsTxtRecord->nITypeA[0] == ITEMTYPE_MELEE_WEAPON || pSkillsTxtRecord->nITypeA[0] == ITEMTYPE_HAND_TO_HAND) && pSkillsTxtRecord->nITypeB[0] <= 0 && !ITEMS_CheckItemTypeId(NULL, ITEMTYPE_WEAPON) && !ITEMS_CheckItemTypeId(pUnit, ITEMTYPE_WEAPON))
		{
			return TRUE;
		}

		if (pSkillsTxtRecord->nITypeA[0] == ITEMTYPE_HAND_TO_HAND && pSkillsTxtRecord->nITypeB[0] <= 0 && !ITEMS_CheckItemTypeId(NULL, ITEMTYPE_WEAPON) && !ITEMS_CheckItemTypeId(pUnit, ITEMTYPE_WEAPON))
		{
			return TRUE;
		}

		return FALSE;
	}

	pEType = &pSkillsTxtRecord->nETypeA[2 * nType];
	for (int i = 0; i < 2; ++i)
	{
		if (*pEType <= 0)
		{
			break;
		}

		if (ITEMS_CheckItemTypeId(pItem, *pEType))
		{
			return FALSE;
		}

		++pEType;
	}

	i = 0;
	pIType = &pSkillsTxtRecord->nITypeA[3 * nType];
	while (1)
	{
		if (*pIType <= 0)
		{
			return i == 0;
		}

		if (ITEMS_CheckItemTypeId(pItem, *pIType))
		{
			break;
		}

		++i;
		++pIType;
		if (i >= 3)
		{
			return FALSE;
		}
	}

	if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__) || ITEMS_CheckItemFlag(pItem, IFLAG_BROKEN, __LINE__, __FILE__))
	{
		return pSkillsTxtRecord->nSkillId == 0;
	}
	else
	{
		if (ITEMS_GetAmmoType(pItem))
		{
			if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_NOAMMO])
			{
				return TRUE;
			}

			if (!pSkillsTxtRecord->nSkillId && STATLIST_GetUnitStatSigned(pItem, STAT_ITEM_MAGICARROW, 0))
			{
				return TRUE;
			}

			if (!pUnit || !ITEMS_CheckItemTypeId(pUnit, ITEMS_GetAmmoType(pItem)))
			{
				return FALSE;
			}

			if (!pSkillsTxtRecord->nSkillId && STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_MAGICARROW, 0))
			{
				return TRUE;
			}

			if (!ITEMS_GetTotalMaxStack(pUnit))
			{
				return TRUE;
			}

			if (STATLIST_GetUnitStatUnsigned(pUnit, STAT_QUANTITY, 0) <= 0)
			{
				return FALSE;
			}
		}

		return TRUE;
	}
}

//D2Common.0x6FDB1380
//TODO: Check name
BOOL __fastcall D2Common_SKILLS_CheckShapeRestriction_6FDB1380(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	int nCounter = 0;

	if (pUnit && pSkill)
	{
		if (pSkill->pSkillsTxt)
		{
			if (!pSkill->pSkillsTxt->nRestrict)
			{
				return STATES_CheckStateMaskRestrictOnUnit(pUnit, pSkill) == 0;
			}

			if (pSkill->pSkillsTxt->nRestrict == 1 || pSkill->pSkillsTxt->nRestrict != 2)
			{
				return TRUE;
			}

			if (STATES_CheckStateMaskRestrictOnUnit(pUnit, pSkill))
			{
				nCounter = 0;
				do
				{
					if (pSkill->pSkillsTxt->nState[nCounter] < 0)
					{
						break;
					}

					if (STATES_CheckState(pUnit, pSkill->pSkillsTxt->nState[nCounter]))
					{
						return TRUE;
					}

					++nCounter;
				}
				while (nCounter < 3);
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FDB1400
//TODO: Check name
BOOL __fastcall D2Common_SKILLMANA_CheckStartStat_6FDB1400(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER || pSkill->pSkillsTxt->wStartMana <= 0
		|| STATLIST_GetUnitStatUnsigned(pUnit, STAT_MANA, 0) >= (int)pSkill->pSkillsTxt->wStartMana << 8 || STATES_CheckState(pUnit, STATE_INFERNO))
	{
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDB1450
BOOL __fastcall SKILLS_CheckSkillDelay(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && STATES_CheckState(pUnit, STATE_SKILLDELAY))
	{
		if (pSkill && pSkill->pSkillsTxt)
		{
			return SKILLS_EvaluateSkillFormula(pUnit, pSkill->pSkillsTxt->dwDelay, pSkill->pSkillsTxt->nSkillId, SKILLS_GetSkillLevel(pUnit, pSkill, TRUE)) <= 0;
		}
	}

	return TRUE;
}

//D2Common.0x6FDB1540 (#10966)
D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecordFromSkill(D2SkillStrc* pSkill)
{
	if (pSkill)
	{
		return pSkill->pSkillsTxt;
	}

	return NULL;
}

//D2Common.0x6FDB1550 (#10967)
int __stdcall SKILLS_GetShrineSkillLevelBonus(D2UnitStrc* pUnit)
{
	if (STATES_CheckState(pUnit, STATE_SHRINE_SKILL))
	{
		return 2;
	}

	return 0;
}

//D2Common.0x6FDB1580
int __fastcall SKILLS_GetBonusSkillLevel(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	D2SkillDescTxt* pSkillDescTxtRecord = NULL;
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	int nSkillLevel = 0;
	int nBonus = 0;

	if (!pSkill || !pSkill->pSkillsTxt || !DATATBLS_GetSkillsTxtRecord(pSkill->pSkillsTxt->nSkillId))
	{
		return 0;
	}

	nSkillLevel = pSkill->nLevelBonus;

	if (STATES_CheckState(pUnit, STATE_SHRINE_SKILL))
	{
		nSkillLevel += 2;
	}

	nSkillLevel += STATLIST_GetUnitStatUnsigned(pUnit, STAT_ITEM_ALLSKILLS, 0);

	if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
	{
		if (pSkill->pSkillsTxt->nCharClass == pUnit->dwClassId)
		{
			nSkillLevel += STATLIST_GetUnitStatUnsigned(pUnit, STAT_ITEM_ADDCLASSSKILLS, pUnit->dwClassId);

			pSkillsTxtRecord = &sgptDataTables->pSkillsTxt[pSkill->pSkillsTxt->nSkillId];
			if (pSkillsTxtRecord)
			{
				pSkillDescTxtRecord = DATATBLS_GetSkillDescTxtRecord(pSkillsTxtRecord->wSkillDesc);

				if (pSkillDescTxtRecord->nSkillPage)
				{
					nSkillLevel += STATLIST_GetUnitStatUnsigned(pUnit, STAT_ITEM_ADDSKILL_TAB, (char)pSkillDescTxtRecord->nSkillPage + 8 * pUnit->dwClassId - 1);
				}
			}

			nBonus = STATLIST_GetUnitStatUnsigned(pUnit, STAT_ITEM_NONCLASSSKILL, pSkill->pSkillsTxt->nSkillId);
			if (nBonus > 3)
			{
				nBonus = 3;
			}
			nSkillLevel += nBonus;
		}
		else
		{
			nSkillLevel += STATLIST_GetUnitStatUnsigned(pUnit, STAT_ITEM_NONCLASSSKILL, pSkill->pSkillsTxt->nSkillId);
		}
	}
	else
	{
		if (pSkill->nSkillLevel <= 0)
		{
			nSkillLevel += STATLIST_GetUnitStatUnsigned(pUnit, STAT_ITEM_NONCLASSSKILL, pSkill->pSkillsTxt->nSkillId);
		}
		else
		{
			nBonus = STATLIST_GetUnitStatUnsigned(pUnit, STAT_ITEM_NONCLASSSKILL, pSkill->pSkillsTxt->nSkillId);
			if (nBonus)
			{
				if (nBonus > 3)
				{
					nBonus = 3;
				}
				nSkillLevel += nBonus;
			}
		}
	}

	if (pSkill->pSkillsTxt->nEType)
	{
		nSkillLevel += STATLIST_GetUnitStatUnsigned(pUnit, STAT_ITEM_ELEMSKILL, pSkill->pSkillsTxt->nEType);
	}

	return nSkillLevel + STATLIST_GetUnitStatUnsigned(pUnit, STAT_ITEM_SINGLESKILL, pSkill->pSkillsTxt->nSkillId);
}

//D2Common.0x6FDB1700 (#10968)
int __stdcall SKILLS_GetSkillLevel(D2UnitStrc* pUnit, D2SkillStrc* pSkill, BOOL bBonus)
{
	int nSkillLevel = 0;
	int nMaxLevel = 0;

	if (pUnit && pSkill)
	{
		nSkillLevel = pSkill->nSkillLevel;
		if (bBonus && pSkill->nOwnerGUID == D2UnitInvalidGUID)
		{
			nSkillLevel += SKILLS_GetBonusSkillLevel(pUnit, pSkill);
		}

		nMaxLevel = DATATBLS_GetMaxLevel(0);

		if (nSkillLevel < 0)
		{
			nSkillLevel = 0;
		}

		if (nSkillLevel > nMaxLevel)
		{
			return nMaxLevel;
		}

		return nSkillLevel;
	}

	return 0;
}

//D2Common.0x6FDB1750 (#11029)
int __stdcall SKILLS_GetBonusSkillLevelFromSkillId(D2UnitStrc* pUnit, int nSkillId)
{
	D2SkillStrc* pSkill = NULL;
	
	if (pUnit && pUnit->pSkills)
	{
		pSkill = pUnit->pSkills->pFirstSkill;

		while (pSkill && (pSkill->pSkillsTxt->nSkillId != nSkillId || pSkill->nOwnerGUID != D2UnitInvalidGUID))
		{
			pSkill = pSkill->pNextSkill;
		}

		if (pSkill && pSkill->nOwnerGUID == D2UnitInvalidGUID)
		{
			return SKILLS_GetBonusSkillLevel(pUnit, pSkill);
		}
	}

	return 0;
}

//D2Common.0x6FDB17A0 (#11030)
void __stdcall D2Common_11030(D2UnitStrc* pUnit, int nSkillId, int a3)
{
	D2SkillStrc* pSkill = NULL;

	if (pUnit)
	{
		pSkill = SKILLS_GetSkill(pUnit, nSkillId, -1);
		if (!pSkill)
		{
			if (a3 <= 0)
			{
				return;
			}

			pSkill = SKILLS_AddSkill(pUnit, nSkillId);
			if (!pSkill)
			{
				return;
			}

			SKILLS_AssignSkill(pUnit, nSkillId, 0, 0, __FILE__, __LINE__);
		}

		if (pSkill->nOwnerGUID == D2UnitInvalidGUID)
		{
			pSkill->nLevelBonus = a3;
			SKILLS_RefreshSkill(pUnit, nSkillId);
		}
	}
}

//D2Common.0x6FDB1820 (#11031)
void __stdcall D2Common_11031(D2UnitStrc* pUnit, int nSkillId, int a3)
{
	D2SkillStrc* pSkill = SKILLS_GetSkill(pUnit, nSkillId, -1);

	if (!pSkill)
	{
		if (a3 <= 0)
		{
			return;
		}
		
		pSkill = SKILLS_AddSkill(pUnit, nSkillId);
		if(!pSkill)
		{
			return;
		}

		SKILLS_AssignSkill(pUnit, nSkillId, 0, 0, __FILE__, __LINE__);
	}

	if (pSkill->nOwnerGUID == D2UnitInvalidGUID)
	{
		pSkill->nLevelBonus += a3;
		if (pSkill->nLevelBonus < 0)
		{
			pSkill->nLevelBonus = 0;
		}
	}
}

//D2Common.0x6FDB18B0 (#10974)
void __stdcall SKILLS_SetSkillMode(D2SkillStrc* pSkill, int nSkillMode)
{
	if (pSkill && nSkillMode >= 0 && nSkillMode <= 32)
	{
		pSkill->dwSkillMode = nSkillMode;
	}
}

//D2Common.0x6FDB18D0 (#10975)
int __stdcall SKILLS_GetSkillMode(D2SkillStrc* pSkill)
{
	if (pSkill)
	{
		return pSkill->dwSkillMode;
	}

	REMOVE_LATER_WriteToLogFile("SKILLS_GetSkillMode: NULL pointer");
	return 0;
}

//D2Common.0x6FDB18F0 (#10969)
int __stdcall D2Common_10969(D2SkillStrc* pSkill)
{
	D2_ASSERT(pSkill);

	return pSkill->unk0x10[0];
}

//D2Common.0x6FDB1920 (#10970)
void __stdcall D2Common_10970(D2SkillStrc* pSkill, int nUnknown)
{
	D2_ASSERT(pSkill);

	pSkill->unk0x10[0] = nUnknown;
}

//D2Common.0x6FDB1950 (#10971)
int __stdcall D2Common_10971(D2SkillStrc* pSkill)
{
	D2_ASSERT(pSkill);

	return pSkill->unk0x10[1];
}

//D2Common.0x6FDB1980 (#10972)
void __stdcall D2Common_10972(D2SkillStrc* pSkill, int nUnknown)
{
	D2_ASSERT(pSkill);

	pSkill->unk0x10[1] = nUnknown;
}

//D2Common.0x6FDB19B0 (#10973)
void __stdcall D2Common_10973(D2SkillStrc* pSkill)
{
	D2_ASSERT(pSkill);

	pSkill->unk0x10[0] = 0;
	pSkill->unk0x10[1] = 0;
}

//D2Common.0x6FDB19F0 (#10976)
int __stdcall SKILLS_GetParam1(D2SkillStrc* pSkill)
{
	D2_ASSERT(pSkill);

	return pSkill->nPar1;
}

//D2Common.0x6FDB1A20 (#10977)
int __stdcall SKILLS_GetParam2(D2SkillStrc* pSkill)
{
	D2_ASSERT(pSkill);

	return pSkill->nPar2;
}

//D2Common.0x6FDB1A50 (#10978)
int __stdcall SKILLS_GetParam3(D2SkillStrc* pSkill)
{
	D2_ASSERT(pSkill);

	return pSkill->nPar3;
}

//D2Common.0x6FDB1A80 (#10979)
int __stdcall SKILLS_GetParam4(D2SkillStrc* pSkill)
{
	D2_ASSERT(pSkill);

	return pSkill->nPar4;
}

//D2Common.0x6FDB1AB0 (#10980)
void __stdcall SKILLS_SetParam1(D2SkillStrc* pSkill, int nPar1)
{
	D2_ASSERT(pSkill);

	pSkill->nPar1 = nPar1;
}

//D2Common.0x6FDB1AE0 (#10981)
void __stdcall SKILLS_SetParam2(D2SkillStrc* pSkill, int nPar2)
{
	D2_ASSERT(pSkill);

	pSkill->nPar2 = nPar2;
}

//D2Common.0x6FDB1B10 (#10982)
void __stdcall SKILLS_SetParam3(D2SkillStrc* pSkill, int nPar3)
{
	D2_ASSERT(pSkill);

	pSkill->nPar3 = nPar3;
}

//D2Common.0x6FDB1B40 (#10983)
void __stdcall SKILLS_SetParam4(D2SkillStrc* pSkill, int nPar4)
{
	D2_ASSERT(pSkill);

	pSkill->nPar4 = nPar4;
}

//D2Common.0x6FDB1B70 (#10984)
void __stdcall SKILLS_SetFlags(D2SkillStrc* pSkill, int nFlags)
{
	D2_ASSERT(pSkill);

	pSkill->dwFlags = nFlags;
}

//D2Common.0x6FDB1BA0 (#10985)
int __stdcall SKILLS_GetFlags(D2SkillStrc* pSkill)
{
	if (pSkill)
	{
		return pSkill->dwFlags;
	}

	REMOVE_LATER_WriteToLogFile("SKILLS_GetFlags: NULL pointer");
	return 0;
}

//D2Common.0x6FDB1BC0 (#10986)
int __stdcall SKILLS_GetRequiredLevel(int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord)
	{
		return pSkillsTxtRecord->wReqLevel;
	}

	return 0x7FFFFFFF;
}

//D2Common.0x6FDB1C00 (#10987)
int __stdcall SKILLS_GetRequiredLevelBasedOnCurrent(D2UnitStrc* pUnit, int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2SkillStrc* pSkill = NULL;
	int nRequiredLevel = 0;

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
	if (pSkillsTxtRecord)
	{
		nRequiredLevel = pSkillsTxtRecord->wReqLevel;
		if (pUnit->pSkills)
		{
			pSkill = pUnit->pSkills->pFirstSkill;
			while (pSkill && (pSkill->pSkillsTxt->nSkillId != nSkillId || pSkill->nOwnerGUID != D2UnitInvalidGUID))
			{
				pSkill = pSkill->pNextSkill;
			}

			if (pSkill)
			{
				nRequiredLevel += pSkill->nSkillLevel;
			}
		}

		return nRequiredLevel;
	}

	return 0x7FFFFFFF;
}

//D2Common.0x6FDB1C80 (#10988)
BOOL __stdcall SKILLS_CheckRequiredSkills(D2UnitStrc* pUnit, int nSkillId)
{
	D2SkillsTxt* pReqSkillSkillsTxtRecord = NULL;
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2SkillStrc* pSkill = NULL;
	int nRequiredLevel = 0;
	int nLevel = 0;

	if (!pUnit)
	{
		return FALSE;
	}

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
	if (!pSkillsTxtRecord)
	{
		return FALSE;
	}

	nLevel = STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0);

	nRequiredLevel = pSkillsTxtRecord->wReqLevel;

	pSkill = SKILLS_GetSkill(pUnit, nSkillId, -1);
	if (pSkill)
	{
		nRequiredLevel += pSkill->nSkillLevel;
	}

	if (nRequiredLevel > nLevel)
	{
		return FALSE;
	}

	for (int i = 0; i < ARRAY_SIZE(pSkillsTxtRecord->nReqSkill); ++i)
	{
		if (pSkillsTxtRecord->nReqSkill[i] >= 0)
		{
			pReqSkillSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(pSkillsTxtRecord->nReqSkill[i]);
			if (!pReqSkillSkillsTxtRecord)
			{
				return FALSE;
			}

			pSkill = UNITS_GetStartSkill(pUnit);
			D2_ASSERT(pSkill);

			while (pSkill->pSkillsTxt != pReqSkillSkillsTxtRecord)
			{
				pSkill = pSkill->pNextSkill;
				if (!pSkill)
				{
					return FALSE;
				}
			}

			pSkill = SKILLS_GetSkill(pUnit, pSkillsTxtRecord->nReqSkill[i], -1);
			if (!pSkill || pSkill->nSkillLevel <= 0)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//D2Common.0x6FDB1F80
D2SkillStrc* __fastcall SKILLS_GetSkill(D2UnitStrc* pUnit, int nSkillId, D2UnitGUID nOwnerGUID)
{
	D2SkillStrc* pSkill = NULL;

	if (pUnit->pSkills)
	{
		pSkill = pUnit->pSkills->pFirstSkill;

		while (pSkill && (pSkill->pSkillsTxt->nSkillId != nSkillId || pSkill->nOwnerGUID != nOwnerGUID))
		{
			pSkill = pSkill->pNextSkill;
		}

		return pSkill;
	}
	
	return NULL;
}

//D2Common.0x6FDB1FC0 (#10989)
BOOL __stdcall SKILLS_CheckRequiredAttributes(D2UnitStrc* pUnit, int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord && pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_INGAME])
	{
		if (SKILLS_GetRequiredLevelBasedOnCurrent(pUnit, nSkillId) <= STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0))
		{
			if (pSkillsTxtRecord->wReqStr <= STATLIST_GetUnitStatUnsigned(pUnit, STAT_STRENGTH, 0))
			{
				if (pSkillsTxtRecord->wReqDex <= STATLIST_GetUnitStatUnsigned(pUnit, STAT_DEXTERITY, 0))
				{
					if (pSkillsTxtRecord->wReqInt <= STATLIST_GetUnitStatUnsigned(pUnit, STAT_ENERGY, 0))
					{
						return pSkillsTxtRecord->wReqVit <= STATLIST_GetUnitStatUnsigned(pUnit, STAT_VITALITY, 0);
					}
				}
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FDB2110 (#10999)
int __stdcall SKILLS_GetToHitFactor(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	unsigned int nCalc = 0;

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
	if (pSkillsTxtRecord && nSkillLevel > 0)
	{
		nCalc = pSkillsTxtRecord->dwToHitCalc;
		if (nCalc == -1)
		{
			return pSkillsTxtRecord->dwToHit + (nSkillLevel - 1) * pSkillsTxtRecord->dwLevToHit;
		}
		else
		{
			return SKILLS_EvaluateSkillFormula(pUnit, nCalc, nSkillId, nSkillLevel);
		}
	}

	return 0;
}

//D2Common.0x6FDB21E0 (#11000)
int __stdcall SKILLS_GetHighestSkillLevelById(D2UnitStrc* pUnit, int nSkillId)
{
	D2SkillStrc* pSkill = NULL;

	if (pUnit)
	{
		pSkill = SKILLS_GetHighestLevelSkillFromSkillId(pUnit, nSkillId);
		if (pSkill)
		{
			return SKILLS_GetSkillLevel(pUnit, pSkill, TRUE);
		}
	}

	return 0;
}

//D2Common.0x6FDB2280 (#11001)
int __stdcall SKILLS_GetManaCosts(int nSkillId, int nSkillLevel)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord)
	{
		return (pSkillsTxtRecord->wMana + (nSkillLevel - 1) * pSkillsTxtRecord->wLevelMana) << pSkillsTxtRecord->nManaShift;
	}

	return 0;
}

//D2Common.0x6FDB22E0
int __fastcall SKILLS_CalculateDamageBonusByLevel(int nLevel, int* pLevelDamage)
{
	if (nLevel > 1 && pLevelDamage)
	{
		if (nLevel > 28)
		{
			return 7 * *pLevelDamage + pLevelDamage[4] * (nLevel - 28) + 6 * (pLevelDamage[2] + pLevelDamage[3]) + 8 * pLevelDamage[1];
		}
		else if (nLevel > 22)
		{
			return 7 * *pLevelDamage + pLevelDamage[3] * (nLevel - 22) + 6 * pLevelDamage[2] + 8 * pLevelDamage[1];
		}
		else if (nLevel > 16)
		{
			return 7 * *pLevelDamage + pLevelDamage[2] * (nLevel - 16) + 8 * pLevelDamage[1];
		}
		else if (nLevel > 8)
		{
			return 7 * *pLevelDamage + pLevelDamage[1] * (nLevel - 8);
		}
		else
		{
			return *pLevelDamage * (nLevel - 1);
		}

	}
	
	return 0;
}

//D2Common.0x6FDB2390 (#11002)
int __stdcall SKILLS_GetMinPhysDamage(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL a4)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2UnitStrc* pItem = NULL;
	long long int nMinDamage = 0;
	int nDamage = 0;
	int nBonus = 0;

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord)
	{
		if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_KICK])
		{
			if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
			{
				nDamage = 3 * STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0);
			}
			else
			{
				nDamage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_DEXTERITY, 0) + STATLIST_GetUnitStatUnsigned(pUnit, STAT_STRENGTH, 0) - 20;
			}

			if (nDamage < 1)
			{
				nDamage = 1;
			}

			return nDamage / 4 << 8;
		}
		else
		{
			if (pSkillsTxtRecord->nSrcDam && a4)
			{
				pItem = D2Common_10434(pUnit, 0);
				if (pItem && INVENTORY_GetWieldType(pUnit, pUnit->pInventory) == 2)
				{
					nMinDamage = STATLIST_GetMinDamageFromUnit(pItem, 1) * pSkillsTxtRecord->nSrcDam;
				}
				else
				{
					nMinDamage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_MINDAMAGE, 0) * pSkillsTxtRecord->nSrcDam;
				}

				nDamage = ((int)nMinDamage) >> 7;
			}

			nDamage += pSkillsTxtRecord->dwMinDam + SKILLS_CalculateDamageBonusByLevel(nSkillLevel, (int*)pSkillsTxtRecord->dwMinLvlDam);

			if (pSkillsTxtRecord->dwDmgSymPerCalc != -1)
			{
				nBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwDmgSymPerCalc, nSkillId, nSkillLevel);

				if (nBonus)
				{
					nDamage += nDamage * nBonus / 100;
				}
			}

			return nDamage << pSkillsTxtRecord->nToHitShift;
		}
	}

	return 1;
}

//D2Common.0x6FDB25D0 (#11003)
int __stdcall SKILLS_GetMaxPhysDamage(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL a4)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2UnitStrc* pItem = NULL;
	long long int nMaxDamage = 0;
	int nDamage = 0;
	int nBonus = 0;

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord)
	{
		if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_KICK])
		{
			if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
			{
				nDamage = 3 * STATLIST_GetUnitStatUnsigned(pUnit, STAT_LEVEL, 0);
			}
			else
			{
				nDamage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_DEXTERITY, 0) + STATLIST_GetUnitStatUnsigned(pUnit, STAT_STRENGTH, 0) - 20;
			}

			if (nDamage < 1)
			{
				nDamage = 1;
			}

			return nDamage / 3 << 8;
		}
		else
		{
			if (pSkillsTxtRecord->nSrcDam && a4)
			{
				pItem = D2Common_10434(pUnit, 0);
				if (pItem && INVENTORY_GetWieldType(pUnit, pUnit->pInventory) == 2)
				{
					nMaxDamage = STATLIST_GetMaxDamageFromUnit(pItem, 24) * pSkillsTxtRecord->nSrcDam;
				}
				else
				{
					nMaxDamage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_MAXDAMAGE, 0) * pSkillsTxtRecord->nSrcDam;
				}

				nDamage = ((int)nMaxDamage) >> 7;
			}

			nDamage += pSkillsTxtRecord->dwMaxDam + SKILLS_CalculateDamageBonusByLevel(nSkillLevel, (int*)pSkillsTxtRecord->dwMaxLvlDam);

			if (pSkillsTxtRecord->dwDmgSymPerCalc != -1)
			{
				nBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwDmgSymPerCalc, nSkillId, nSkillLevel);

				if (nBonus)
				{
					nDamage += nDamage * nBonus / 100;
				}
			}

			return nDamage << pSkillsTxtRecord->nToHitShift;
		}
	}

	return 2;
}

//D2Common.0x6FDB2810 (#11004)
int __stdcall SKILLS_GetMinElemDamage(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL a4)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	int nDamage = 0;
	int nBonus = 0;

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord && nSkillLevel > 0)
	{
		nDamage = (pSkillsTxtRecord->dwEMin + SKILLS_CalculateDamageBonusByLevel(nSkillLevel, (int*)pSkillsTxtRecord->dwEMinLev)) << pSkillsTxtRecord->nToHitShift;

		if (pSkillsTxtRecord->dwEDmgSymPerCalc != -1 && (nDamage > 256 || pSkillsTxtRecord->dwEMinLev[0]))
		{
			nBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwEDmgSymPerCalc, nSkillId, nSkillLevel);

			if (nBonus)
			{
				nDamage += nDamage * nBonus / 100;
			}
		}

		if (a4 && pUnit)
		{
			nDamage += SKILLS_CalculateMasteryBonus(pUnit, pSkillsTxtRecord->nEType, nDamage);
		}

		return nDamage;
	}

	return 0;
}

//D2Common.0x6FDB29D0
int __fastcall SKILLS_CalculateMasteryBonus(D2UnitStrc* pUnit, int nElemType, int nSrcDamage)
{
	int nPercentage = 0;

	switch (nElemType)
	{
	case ELEMTYPE_FIRE:
		nPercentage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_PASSIVE_FIRE_MASTERY, 0);
		if (!nPercentage)
		{
			return 0;
		}

		return nSrcDamage * nPercentage / 100;
	case ELEMTYPE_LTNG:
		nPercentage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_PASSIVE_LTNG_MASTERY, 0);
		if (!nPercentage)
		{
			return 0;
		}

		return nSrcDamage * nPercentage / 100;
	case ELEMTYPE_COLD:
	case ELEMTYPE_FREEZE:
		nPercentage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_PASSIVE_COLD_MASTERY, 0);
		if (!nPercentage)
		{
			return 0;
		}

		return nSrcDamage * nPercentage / 100;
	case ELEMTYPE_POIS:
		nPercentage = STATLIST_GetUnitStatUnsigned(pUnit, STAT_PASSIVE_POIS_MASTERY, 0);
		if (!nPercentage)
		{
			return 0;
		}
		
		return nSrcDamage * nPercentage / 100;
	default:
		return 0;
	}
}

//D2Common.0x6FDB2B00 (#11005)
int __stdcall SKILLS_GetMaxElemDamage(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL a4)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	int nDamage = 0;
	int nBonus = 0;

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord && nSkillLevel > 0)
	{
		nDamage = (pSkillsTxtRecord->dwEMax + SKILLS_CalculateDamageBonusByLevel(nSkillLevel, (int*)pSkillsTxtRecord->dwEMaxLev)) << pSkillsTxtRecord->nToHitShift;

		if (pSkillsTxtRecord->dwEDmgSymPerCalc != -1)
		{
			nBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwEDmgSymPerCalc, nSkillId, nSkillLevel);

			if (nBonus)
			{
				nDamage += nDamage * nBonus / 100;
			}
		}

		if (a4 && pUnit)
		{
			nDamage += SKILLS_CalculateMasteryBonus(pUnit, pSkillsTxtRecord->nEType, nDamage);
		}

		return nDamage;
	}

	return 0;
}

//D2Common.0x6FDB2CA0 (#11006)
int __stdcall SKILLS_GetElementalLength(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL bUnused)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	int nLength = 0;
	int nBonus = 0;

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (!pSkillsTxtRecord || nSkillLevel <= 0)
	{
		return 0;
	}

	if (nSkillLevel <= 16)
	{
		if (nSkillLevel <= 8)
		{
			nLength = pSkillsTxtRecord->dwELevLen[0] * (nSkillLevel - 1);
		}
		else
		{
			nLength = 7 * pSkillsTxtRecord->dwELevLen[0] + (nSkillLevel - 8) * pSkillsTxtRecord->dwELevLen[1];
		}
	}
	else
	{
		nLength = 7 * pSkillsTxtRecord->dwELevLen[0] + (nSkillLevel - 16) * pSkillsTxtRecord->dwELevLen[2] + 8 * pSkillsTxtRecord->dwELevLen[1];
	}

	nLength += pSkillsTxtRecord->dwELen;

	if (pSkillsTxtRecord->dwELenSymPerCalc != -1)
	{
		nBonus = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwELenSymPerCalc, nSkillId, nSkillLevel);

		if (nBonus)
		{
			nLength += nLength * nBonus / 100;
		}
	}

	return nLength;
}

//D2Common.0x6FDB2E70 (#11239)
int __stdcall SKILLS_Return0(int arg)
{
	REMOVE_LATER_WriteToLogFile("SKILLS_Return0: Useless");
	return 0;
}

//D2Common.0x6FDB2E80 (#11008)
int __stdcall SKILLS_GetClassIdFromSkillId(int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord && pSkillsTxtRecord->nCharClass >= 0)
	{
		return pSkillsTxtRecord->nCharClass;
	}

	return 7;
}

//D2Common.0x6FDB2EC0 (#11010)
BOOL __stdcall SKILLS_IsPlayerClassSkill(int nSkillId, int* pPlayerClass)
{
	D2_ASSERT(pPlayerClass);

	*pPlayerClass = SKILLS_GetClassIdFromSkillId(nSkillId);
	return *pPlayerClass != 7;
}

//D2Common.0x6FDB2F40 (#11011)
int __stdcall SKILLS_GetQuantity(D2SkillStrc* pSkill)
{
	D2_ASSERT(pSkill);

	return pSkill->nQuantity;
}

//D2Common.0x6FDB2F70 (#11012)
void __stdcall SKILLS_SetQuantity(D2SkillStrc* pSkill, int nQuantity)
{
	D2_ASSERT(pSkill);

	pSkill->nQuantity = nQuantity;
}

//D2Common.0x6FDB2FA0 (#11014)
int __stdcall D2Common_11014_ConvertShapeShiftedMode(int nArrayIndex, int nMonsterId)
{
	int nMode = dword_6FDD2BD8[nArrayIndex];
	
	while (1)
	{
		D2MonStats2Txt* pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(nMonsterId);
		if (pMonStats2TxtRecord && (pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[nMode]))
		{
			return nMode;
		}

		switch (nMode)
		{
		case MONMODE_NEUTRAL:
			return nMode;
		case MONMODE_ATTACK2:
		case MONMODE_CAST:
			nMode = MONMODE_ATTACK1;
			continue;
		case MONMODE_BLOCK:
			nMode = MONMODE_GETHIT;
			continue;
		case MONMODE_SKILL2:
		case MONMODE_SKILL3:
		case MONMODE_SKILL4:
			nMode = MONMODE_SKILL1;
			continue;
		case MONMODE_RUN:
			nMode = MONMODE_WALK;
			continue;
		default:
			nMode = MONMODE_NEUTRAL;
			continue;
		}
	}
}

//D2Common.0x6FDB30A0 (#11013)
void __stdcall D2COMMON_11013_ConvertMode(D2UnitStrc* pUnit, int* pType, int* pClass, int* pMode, char* szFile, int nLine)
{
	if (!(pUnit && (pUnit->dwFlagEx & UNITFLAGEX_ISSHAPESHIFTED) && sgptDataTables->nTransformStates > 0))
	{
		return;
	}

	D2StatesTxt* pStatesTxtRecord = NULL;
	int nCounter = 0;
	while (1)
	{
		int nState = sgptDataTables->pTransformStates[nCounter];
		if (STATES_CheckState(pUnit, nState) && (0 <= nState && nState < sgptDataTables->nStatesTxtRecordCount))
		{
			pStatesTxtRecord = &sgptDataTables->pStatesTxt[nState];
			if (pStatesTxtRecord->nGfxType == 1)
			{
				break;
			}
			
			if (pStatesTxtRecord->nGfxType == 2)
			{
				*pType = UNIT_PLAYER;
				*pClass = pStatesTxtRecord->wGfxClass;

				if (pUnit->dwUnitType == UNIT_MONSTER)
				{
					if (pStatesTxtRecord->wGfxClass == 6 && *pMode == 4)
					{
						*pMode = 12;
					}
					else
					{
						*pMode = dword_6FDD2B98[*pMode];
					}
				}
				return;
			}
		}

		++nCounter;
		if (nCounter >= sgptDataTables->nTransformStates)
		{
			return;
		}
	}

	*pType = UNIT_MONSTER;
	int nMonsterId = pStatesTxtRecord->wGfxClass;
	*pClass = nMonsterId;

	if (pUnit->dwUnitType == UNIT_PLAYER)
	{
		*pMode = D2Common_11014_ConvertShapeShiftedMode(*pMode, nMonsterId);
	}
}

//D2Common.0x6FDB3290 (#11015)
void __stdcall D2Common_11015(D2UnitStrc* pUnit, int a2, int nSkillId)
{
	D2SkillStrc* pSkill = NULL;
	int nPosition = 0;

	pSkill = SKILLS_GetSkill(pUnit, nSkillId, -1);
	if (a2)
	{
		if (pUnit)
		{
			nPosition = pUnit->dwUnitId + (pUnit->dwUnitType << 16);
		}
		else
		{
			nPosition = 0x5FFFF;
		}
	}
	else
	{
		nPosition = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath) + 3 * D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
	}

	D2_ASSERT(pSkill);

	if (pSkill->nXpos == nPosition)
	{
		pSkill->nXpos = nPosition;
		pSkill->nYpos += 7;
		SEED_InitLowSeed(&pUnit->pSeed, pSkill->nYpos);
	}
	else
	{
		pSkill->nXpos = nPosition;
		pSkill->nYpos = nPosition;
		SEED_InitLowSeed(&pUnit->pSeed, nPosition);
	}
}

//D2Common.0x6FDB3340 (#11016)
int __stdcall D2Common_11016(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
	int nRange = 0;

	if (pSkill && pSkill->pSkillsTxt)
	{
		nRange = pSkill->pSkillsTxt->nRange;
		if (nRange == 3)
		{
			nRange = (COMPOSIT_IsWeaponBowOrXBow(pUnit) != 0) + 1;
			if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && nRange != 1 && STATES_CheckStateMaskOnUnit(pUnit, STATEMASK_MELEEONLY))
			{
				return 1;
			}
		}

		return nRange;
	}

	return 0;
}

//D2Common.0x6FDB33A0 (#11017)
//TODO: Check name
BOOL __stdcall D2COMMON_11017_CheckUnitIfConsumeable(D2UnitStrc* pUnit, int a2)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;

	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && (pUnit->dwAnimMode == MONMODE_DEAD || a2 && pUnit->dwAnimMode == MONMODE_DEATH) && !STATES_CheckStateMaskUdeadOnUnit(pUnit))
	{
		pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_CORPSESEL])
		{
			return sgptDataTables->pMonStatsTxt[pUnit->dwClassId].nVelocity != 0;
		}
	}

	return FALSE;
}

//D2Common.0x6FDB3480 (#11020)
BOOL __stdcall D2Common_11020(D2UnitStrc* pUnit)
{
	if (SKILLS_CanUnitCorpseBeSelected(pUnit) && UNITS_IsSoftMonster(pUnit))
	{
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDB3520 (#11022)
BOOL __stdcall SKILLS_CanUnitCorpseBeSelected(D2UnitStrc* pUnit)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;

	if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->dwAnimMode == MONMODE_DEAD && !STATES_CheckStateMaskUdeadOnUnit(pUnit))
	{
		pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pUnit->dwClassId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_CORPSESEL])
		{
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FDB35B0 (#11024)
int __stdcall D2Common_11024(D2UnitStrc* pUnit, D2UnitStrc* pItem, D2SkillStrc* pSkill, int nType, BOOL* pHasThrowBonus)
{
	int nStatId = 0;
	int nValue = 0;
	D2StatStrc pStat[32] = {};

	if (pHasThrowBonus)
	{
		*pHasThrowBonus = FALSE;
	}

	if (!pSkill)
	{
		pSkill = UNITS_GetUsedSkill(pUnit);
		if (!pSkill)
		{
			return 0;
		}
	}
	
	if(!ITEMS_CheckIfThrowable(pItem) || !pSkill->pSkillsTxt || pSkill->pSkillsTxt->nITypeA[0] <= 0
	   || !ITEMS_CheckType(pSkill->pSkillsTxt->nITypeA[0], ITEMTYPE_THROWN_WEAPON) || pSkill->pSkillsTxt->nRange != 2)
	{
		return 0;
	}

	if (nType == 0)
	{
		nStatId = STAT_PASSIVE_MASTERY_THROW_TH;
	}
	else if (nType == 1)
	{
		nStatId = STAT_PASSIVE_MASTERY_THROW_DMG;
	}
	else if (nType == 2)
	{
		nStatId = STAT_PASSIVE_MASTERY_THROW_CRIT;
	}
	else
	{
		return 0;
	}

	for (int i = 0; i < D2Common_11270(pUnit, nStatId, pStat, ARRAY_SIZE(pStat)); ++i)
	{
		if (ITEMS_CheckItemTypeId(pItem, pStat[i].nLayer) && pStat[i].nValue > nValue)
		{
			nValue = pStat[i].nValue;
		}
	}

	if (pHasThrowBonus)
	{
		*pHasThrowBonus = TRUE;
	}

	return nValue;
}

//D2Common.0x6FDB36D0 (#11023)
int __stdcall SKILLS_GetWeaponMasteryBonus(D2UnitStrc* pUnit, D2UnitStrc* pItem, D2SkillStrc* pSkill, int nType)
{
	int nStatId = 0;
	int nValue = 0;
	BOOL bHasThrowBonus = FALSE;
	D2StatStrc pStat[32] = {};

	if (pUnit && pItem)
	{
		if (!pSkill)
		{
			pSkill = UNITS_GetUsedSkill(pUnit);
		}

		nValue = D2Common_11024(pUnit, pItem, pSkill, nType, &bHasThrowBonus);
		if (!bHasThrowBonus)
		{
			if (nType == 0)
			{
				nStatId = STAT_PASSIVE_MASTERY_MELEE_TH;
			}
			else if (nType == 1)
			{
				nStatId = STAT_PASSIVE_MASTERY_MELEE_DMG;
			}
			else if (nType == 2)
			{
				nStatId = STAT_PASSIVE_MASTERY_MELEE_CRIT;
			}
			else
			{
				return 0;
			}

			nValue = 0;
			for (int i = 0; i < D2Common_11270(pUnit, nStatId, pStat, ARRAY_SIZE(pStat)); ++i)
			{
				if (ITEMS_CheckItemTypeId(pItem, pStat[i].nLayer))
				{
					if (pStat[i].nValue > nValue)
					{
						nValue = pStat[i].nValue;
					}
				}
			}
		}

		return nValue;
	}

	return 0;
}

//D2Common.0x6FDB37B0 (#11032)
int __stdcall D2Common_11032(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, int nType)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	int nThrowStatId = 0;
	int nMeleeStatId = 0;
	D2SkillCalcStrc pSkillCalc = {};

	pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
	if (pSkillsTxtRecord && nSkillLevel > 0)
	{
		switch (nType)
		{
		case 0:
			nMeleeStatId = STAT_PASSIVE_MASTERY_MELEE_TH;
			nThrowStatId = STAT_PASSIVE_MASTERY_THROW_TH;
			break;
		case 1:
			nMeleeStatId = STAT_PASSIVE_MASTERY_MELEE_DMG;
			nThrowStatId = STAT_PASSIVE_MASTERY_THROW_DMG;
			break;
		case 2:
			nMeleeStatId = STAT_PASSIVE_MASTERY_MELEE_CRIT;
			nThrowStatId = STAT_PASSIVE_MASTERY_THROW_CRIT;
			break;
		default:
			return 0;
		}

		for (int i = 0; i < 5; ++i)
		{
			if (pSkillsTxtRecord->nPassiveStat[i] == nMeleeStatId || pSkillsTxtRecord->nPassiveStat[i] == nThrowStatId)
			{
				return SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[i], nSkillId, nSkillLevel);
			}
		}
	}

	return 0;
}

//D2Common.0x6FDB3910 (#11025)
BOOL __stdcall D2Common_11025(int nX1, int nY1, int nX2, int nY2, D2RoomStrc* pRoom, int a6)
{
	D2CoordStrc pCoords1 = {};
	D2CoordStrc pCoords2 = {};

	pCoords1.nX = nX1;
	pCoords1.nY = nY1;
	pCoords2.nX = nX2;
	pCoords2.nY = nY2;

	return D2Common_11263(pRoom, &pCoords1, &pCoords2, a6) == 0;
}

//D2Common.0x6FDB3960 (#11026)
BOOL __stdcall D2Common_11026(int nX, int nY, D2UnitStrc* pUnit, int a4)
{
	D2CoordStrc pCoords1 = {};
	D2CoordStrc pCoords2 = {};

	pCoords1.nX = nX;
	pCoords1.nY = nY;
	
	UNITS_GetCoords(pUnit, &pCoords2);

	return D2Common_11263(UNITS_GetRoom(pUnit), &pCoords1, &pCoords2, a4) == 0;
}

//D2Common.0x6FDB3A10 (#11027)
int __stdcall SKILLS_GetShiftedManaCosts(int nSkillId, int nLevel)
{
	int nMana = SKILLS_GetManaCosts(nSkillId, nLevel) >> 8;

	if (nMana < 0)
	{
		nMana = 0;
	}

	return nMana;

}

//D2Common.0x6FDB3A90 (#11028)
int __stdcall D2Common_11028(int a1)
{
	return 20 * (5 * a1 + 5) / (a1 + 8);
}

//D2Common.0x6FDB3AB0 (#11033)
int __stdcall D2Common_11033(int nLevel, int nParam, int nMax)
{
	int nValue = nParam + (nMax - nParam) * 110 * nLevel / (nLevel + 6) / 100;

	if (nValue > nMax)
	{
		nValue = nMax;
	}

	return nValue;
}

//D2Common.0x6FDB3B00 (#11034)
int __stdcall D2Common_11034(int nLevel, int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord && nLevel > 0)
	{
		return D2Common_11033(nLevel, pSkillsTxtRecord->dwParam[0], pSkillsTxtRecord->dwParam[1]);
	}

	return 0;
}

//D2Common.0x6FDB3B90 (#11035)
int __stdcall D2Common_11035(int nLevel, int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord && nLevel > 0)
	{
		return D2Common_11033(nLevel, pSkillsTxtRecord->dwParam[2], pSkillsTxtRecord->dwParam[3]);
	}

	return 0;
}

//D2Common.0x6FDB3C20 (#11036)
//TODO: Check name
int __stdcall D2COMMON_11036_GetMonCurseResistanceSubtraction(int nLevel, int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord && nLevel > 0)
	{
		return D2Common_11033(nLevel, pSkillsTxtRecord->dwParam[4], pSkillsTxtRecord->dwParam[5]);
	}

	return 0;
}

//D2Common.0x6FDB3CB0 (#11037)
BOOL __stdcall D2Common_11037(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int* pX, int* pY)
{
	D2RoomStrc* pRoom = NULL;
	int nDivisor = 0;
	D2CoordStrc pCoords1 = {};
	D2CoordStrc pCoords2 = {};
	D2CoordStrc pCoord = {};

	if (pUnit2 && UNITS_IsInMeleeRange(pUnit1, pUnit2, 0))
	{
		return FALSE;
	}

	UNITS_GetCoords(pUnit1, &pCoords1);

	if (pUnit2)
	{
		UNITS_GetCoords(pUnit2, &pCoords2);
	}
	else
	{
		pCoords2.nX = D2COMMON_10175_PathGetFirstPointX(pUnit1->pDynamicPath);
		pCoords2.nY = D2COMMON_10176_PathGetFirstPointY(pUnit1->pDynamicPath);
	}

	nDivisor = UNITS_GetDistanceToCoordinates(pUnit1, pCoords2.nX, pCoords2.nY);
	
	if (nDivisor)
	{
		pCoord.nX = pCoords2.nX + 2 * (pCoords2.nX - pCoords1.nX) / nDivisor;
		pCoord.nY = pCoords2.nY + 2 * (pCoords2.nY - pCoords1.nY) / nDivisor;

		pRoom = UNITS_GetRoom(pUnit1);
		D2_ASSERT(pRoom);

		if (COLLISION_CheckMaskWithPattern2(pRoom, pCoord.nX, pCoord.nY, PATH_GetUnitCollisionPattern(pUnit1), 0x1C09))
		{
			pCoord.nX = pCoords2.nX;
			pCoord.nY = pCoords2.nY;

			pRoom = COLLISION_GetFreeCoordinates(pRoom, &pCoord, UNITS_GetUnitSizeX(pUnit1), 0x1C09, 0);
			if (!pRoom)
			{
				return FALSE;
			}
		}
		
		if (!D2Common_11263(pRoom, &pCoords1, &pCoord, 0x804))
		{
			*pX = pCoord.nX;
			*pY = pCoord.nY;

			pRoom = UNITS_GetRoom(pUnit1);
			if (pRoom)
			{
				pRoom = COLLISION_GetRoomBySubTileCoordinates(pRoom, *pX, *pY);
				if (pRoom)
				{
					return !DUNGEON_IsRoomInTown(pRoom);
				}
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FDB3F60 (#11039)
//TODO: Check name
int __stdcall D2COMMON_11039_CheckWeaponIsMissileBased(D2UnitStrc* pUnit, int* pValue)
{
	int nWeaponClass = 0;
	int nValue = 0;

	if (!pUnit)
	{
		return 0;
	}

	if (pUnit->dwUnitType != UNIT_PLAYER)
	{
		nValue = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_MAGICARROW, 0);
		if (nValue > 0)
		{
			if (pValue)
			{
				*pValue = nValue;
			}
			return 27;
		}

		nValue = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_EXPLOSIVEARROW, 0);
		if (nValue > 0)
		{
			if (pValue)
			{
				*pValue = nValue;
			}
			return 41;
		}

		return 0;
	}
	else
	{
		nWeaponClass = UNITS_GetWeaponClass(pUnit);
		if (nWeaponClass == WEAPONCLASS_BOW)
		{
			nValue = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_MAGICARROW, 0);
			if (nValue > 0)
			{
				if (pValue)
				{
					*pValue = nValue;
				}
				return 27;
			}

			nValue = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_EXPLOSIVEARROW, 0);
			if (nValue > 0)
			{
				if (pValue)
				{
					*pValue = nValue;
				}
				return 41;
			}

			return 0;
		}
		else if (nWeaponClass == WEAPONCLASS_XBW)
		{
			nValue = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_MAGICARROW, 0);
			if (nValue > 0)
			{
				if (pValue)
				{
					*pValue = nValue;
				}
				return 27;
			}

			nValue = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_EXPLOSIVEARROW, 0);
			if (nValue > 0)
			{
				if (pValue)
				{
					*pValue = nValue;
				}
				return 41;
			}

			return 31;
		}

		return -1;
	}
}

//D2Common.0x6FDB4020 (#11040)
BOOL __stdcall SKILLS_IsEnhanceable(int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);

	if (pSkillsTxtRecord)
	{
		return pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_ENHANCEABLE];
	}

	return FALSE;
}

//D2Common.0x6FDB4070 (#11230)
BOOL __stdcall SKILLS_RemoveTransformStatesFromShapeshiftedUnit(D2UnitStrc* pUnit)
{
	D2StatListStrc* pStatList = NULL;

	if (STATES_IsUnitShapeShifted(pUnit))
	{
		for (int i = 0; i < sgptDataTables->nTransformStates; ++i)
		{
			if (STATES_CheckState(pUnit, sgptDataTables->pTransformStates[i]))
			{
				pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, sgptDataTables->pTransformStates[i]);
				if (pStatList)
				{
					D2Common_10474(pUnit, (D2StatListExStrc*)pStatList);
					STATLIST_FreeStatList(pStatList);
				}

				STATES_ToggleState(pUnit, sgptDataTables->pTransformStates[i], FALSE);
			}
		}

		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDB4100 (#11041)
int __stdcall SKILLS_GetClassSkillId(int nClassId, int nPosition)
{
	if (nClassId >= 0 && nClassId < 7 && nPosition >= 0 && nPosition < sgptDataTables->nClassSkillCount[nClassId])
	{
		return sgptDataTables->nClassSkillList[nPosition + nClassId * sgptDataTables->nHighestClassSkillCount];
	}

	return -1;
}

//D2Common.0x6FDB4150 (#11042)
int __stdcall SKILLS_GetPlayerSkillCount(int nClassId)
{
	if (nClassId >= 0 && nClassId < 7)
	{
		return sgptDataTables->nClassSkillCount[nClassId];
	}

	return 0;
}

//D2Common.0x6FDB4180 (#11043)
int __stdcall D2Common_11043(D2UnitStrc* pUnit)
{
	D2UnitStrc* pItem = NULL;
	int nAttackSpeed = 19;

	if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER || !STATES_IsUnitShapeShifted(pUnit))
	{
		return 0;
	}
	
	pItem = D2Common_10434(pUnit, 0);
	if(pItem)
	{
		nAttackSpeed = ITEMS_GetWeaponAttackSpeed(pUnit, pItem);
		if (nAttackSpeed <= 0)
		{
			return 0;
		}
	}

	return (pUnit->dwFrameCount & 0xFFFFFF00) / nAttackSpeed;
}

//D2Common.0x6FDB41D0 (#11047)
int __stdcall SKILLS_GetConcentrationDamageBonus(D2UnitStrc* pUnit, int nSkillId)
{
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2StatListStrc* pStatList = NULL;

	if (STATES_CheckState(pUnit, STATE_CONCENTRATION))
	{
		pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_CONCENTRATION);
		if (pStatList)
		{
			pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
			if (pSkillsTxtRecord)
			{
				return STATLIST_GetStatValue(pStatList, STAT_DAMAGEPERCENT, 0) * pSkillsTxtRecord->dwParam[0] / 8;
			}
		}
	}

	return 0;
}

//D2Common.0x6FDB4260 (#11283)
void __stdcall SKILLS_CalculateKickDamage(D2UnitStrc* pUnit, int* pMinDamage, int* pMaxDamage, int* pDamagePercent)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	D2UnitStrc* pRightHandItem = NULL;
	D2UnitStrc* pLeftHandItem = NULL;
	D2UnitStrc* pBoots = NULL;
	int nDamagePercent = 0;
	int nKickDamage = 0;
	int nMinDamage = 0;
	int nMaxDamage = 0;
	short nStrBonus = 0;
	short nDexBonus = 0;

	if (pUnit)
	{
		nKickDamage = STATLIST_GetUnitStatSigned(pUnit, STAT_ITEM_KICKDAMAGE, 0);
		*pMinDamage += nKickDamage;
		*pMaxDamage += nKickDamage;

		if (pUnit->pInventory)
		{
			pLeftHandItem = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);

			if (pLeftHandItem)
			{
				pRightHandItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, (ITEMS_GetBodyLocation(pLeftHandItem) == BODYLOC_RARM) + 4);
			}
			else
			{
				pRightHandItem = NULL;
			}

			if (ITEMS_CheckItemTypeId(pLeftHandItem, ITEMTYPE_WEAPON) && ITEMS_CanBeEquipped(pLeftHandItem) && STATLIST_GetOwner(pLeftHandItem, 0))
			{
				STATLIST_MergeStatLists(pUnit, pLeftHandItem, 0);
			}
			else
			{
				pLeftHandItem = NULL;
			}

			if (pRightHandItem && pRightHandItem != pLeftHandItem && ITEMS_CheckItemTypeId(pRightHandItem, ITEMTYPE_WEAPON) && ITEMS_CanBeEquipped(pRightHandItem) && STATLIST_GetOwner(pRightHandItem, 0))
			{
				STATLIST_MergeStatLists(pUnit, pRightHandItem, 0);
			}

			pBoots = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_FEET);
			if (pBoots && pBoots->dwUnitType == UNIT_ITEM)
			{
				pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pBoots->dwClassId);
				if (pItemsTxtRecord)
				{
					nMinDamage = nKickDamage + pItemsTxtRecord->nMinDam;
					nMaxDamage = nKickDamage + pItemsTxtRecord->nMaxDam;

					nStrBonus = ITEMS_GetStrengthBonus(pBoots);
					if (nStrBonus)
					{
						nDamagePercent += nStrBonus * STATLIST_GetUnitStatUnsigned(pUnit, STAT_STRENGTH, 0) / 100;
					}

					nDexBonus = ITEMS_GetDexBonus(pBoots);
					if (nDexBonus)
					{
						nDamagePercent += nDexBonus * STATLIST_GetUnitStatUnsigned(pUnit, STAT_DEXTERITY, 0) / 100;
					}

					nDamagePercent += STATLIST_GetUnitStatUnsigned(pUnit, STAT_DAMAGEPERCENT, 0);
					if (nDamagePercent <= -90)
					{
						nDamagePercent = -90;
					}

					if (nMinDamage >= nMaxDamage)
					{
						nMaxDamage = nMinDamage;
					}

					*pDamagePercent += STATLIST_GetUnitStatUnsigned(pUnit, STAT_ITEM_MAXDAMAGE_PERCENT, 0) + nDamagePercent;
					*pMinDamage += nMinDamage;
					*pMaxDamage += nMaxDamage;
				}
			}

			if (pLeftHandItem)
			{
				STATLIST_MergeStatLists(pUnit, pLeftHandItem, 1);
			}
		}
	}
}
