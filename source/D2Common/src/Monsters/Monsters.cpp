#include "D2Monsters.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgMaze.h"
#include "D2Dungeon.h"
#include "Path/Path.h"
#include "D2Seed.h"
#include "D2States.h"
#include "D2StatList.h"
#include "Units/Units.h"
#include <D2Lang.h>
#include <D2BitManip.h>
#include <DataTbls/MonsterIds.h>


//TODO: Check names

struct D2UnkHireStrc
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
};

struct D2UnkHireStrc2
{
	int field_0;
	int field_4;
};

struct D2UnkHireStrc3
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
};

struct D2UnkHireStrc4
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
};

struct D2UnkHireStrc5
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
};


//D2Common.0x6FDA4C10 (#11082)
int __fastcall MONSTERS_HirelingInit(BOOL bExpansion, D2UnitStrc* pMonster, int nLowSeed, int nAct, int nDifficulty, D2HirelingInitStrc* pHirelingInit)
{
	D2HirelingTxt* pHirelingTxtRecord = NULL;
	D2HirelingTxt* pRecord = NULL;
	int nLevelUps = 0;
	int nEntries = 0;
	int nRecord = 0;
	int nLevel = 0;
	int nRand = 0;
	D2HirelingTxt* pRecords[16] = {};
	D2SeedStrc pSeed = {};

	SEED_InitSeed(&pSeed);
	SEED_InitLowSeed(&pSeed, nLowSeed);

	while (1)
	{
		pRecord = DATATBLS_GetNextHirelingTxtRecordFromActAndDifficulty(bExpansion, nAct, nDifficulty, pRecord);
		if (!pRecord)
		{
			break;
		}
		pRecords[nEntries] = pRecord;
		++nEntries;
	}

	if (nEntries)
	{
		nRecord = SEED_RollLimitedRandomNumber(&pSeed, nEntries);

		pHirelingTxtRecord = pRecords[nRecord];

		pHirelingInit->nId = pHirelingTxtRecord->nId;

		nLevel = STATLIST_GetUnitStatUnsigned(pMonster, STAT_LEVEL, 0);
		nRand = (int)SEED_RollRandomNumber(&pSeed) % 5;
		if (nRand >= nLevel - pHirelingTxtRecord->nHirelingLevel)
		{
			nLevelUps = nLevel - pHirelingTxtRecord->nHirelingLevel;
		}
		else
		{
			nLevelUps = nRand;
		}
		pHirelingInit->nLevel = pHirelingTxtRecord->nHirelingLevel + nLevelUps;

		pHirelingInit->nHitpoints = nLevelUps * pHirelingTxtRecord->dwHitpointsPerLvl + pHirelingTxtRecord->dwHitpoints;
		if (pHirelingInit->nHitpoints <= 40)
		{
			pHirelingInit->nHitpoints = 40;
		}

		pHirelingInit->nStrength = (nLevelUps * pHirelingTxtRecord->nStrPerLvl >> 3) + pHirelingTxtRecord->nStr;
		if (pHirelingInit->nStrength <= 10)
		{
			pHirelingInit->nStrength = 10;
		}

		pHirelingInit->nDexterity = (nLevelUps * pHirelingTxtRecord->nDexPerLvl >> 3) + pHirelingTxtRecord->nDex;
		if (pHirelingInit->nDexterity <= 10)
		{
			pHirelingInit->nDexterity = 10;
		}

		pHirelingInit->nGold = pHirelingTxtRecord->nGold * (15 * nLevelUps + 100) / 100;
		if (pHirelingInit->nGold < pHirelingTxtRecord->nGold)
		{
			pHirelingInit->nGold = pHirelingTxtRecord->nGold;
		}

		pHirelingInit->nExperience = pHirelingInit->nLevel * pHirelingInit->nLevel * pHirelingTxtRecord->dwExpPerLvl * (pHirelingInit->nLevel + 1);
		if (pHirelingInit->nExperience <= 0)
		{
			pHirelingInit->nExperience = 0;
		}

		pHirelingInit->nDefense = pHirelingTxtRecord->dwDefense + nLevelUps * pHirelingTxtRecord->dwDefensePerLvl;
		if (pHirelingInit->nDefense <= 0)
		{
			pHirelingInit->nDefense = 0;
		}

		pHirelingInit->nMinDamage = pHirelingTxtRecord->nDmgMin + (nLevelUps * pHirelingTxtRecord->nDmgPerLvl >> 3);
		if (pHirelingInit->nMinDamage <= 0)
		{
			pHirelingInit->nMinDamage = 0;
		}

		pHirelingInit->nMaxDamage = pHirelingTxtRecord->nDmgMax + (nLevelUps * pHirelingTxtRecord->nDmgPerLvl >> 3);
		if (pHirelingInit->nMaxDamage <= 1)
		{
			pHirelingInit->nMaxDamage = 1;
		}

		pHirelingInit->nShare = pHirelingTxtRecord->dwShare;
		if (pHirelingInit->nShare <= 0)
		{
			pHirelingInit->nShare = 0;
		}

		pHirelingInit->nResist = pHirelingTxtRecord->dwResist;
		if (pHirelingInit->nResist <= 0)
		{
			pHirelingInit->nResist = 0;
		}

		pHirelingInit->nHireDesc = pHirelingTxtRecord->nHireDesc;
		pHirelingInit->field_34 = 1;
		pHirelingInit->nFlags = 0;
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDA4E20
//TODO: Unknown structs, a2 probably not D2HirelingInitStrc
BOOL __fastcall D2Common_11081(int nLowSeed, D2HirelingInitStrc* pHirelingInit, uint8_t a3)
{
	static const int dword_6FDE4A84[] =
	{
		0, 2, 4, 0
	};

	static const int dword_6FDE4A94[] =
	{
		2, 2, 2, 0, 6
	};

	static const int dword_6FDE4AA8[] =
	{
		0, 2, 4, 0
	};

	static const int dword_6FDE4AB8[] =
	{
		2, 2, 2, 0, 6
	};

	static const int dword_6FDE4ACC[] =
	{
		0, 2, 4, 0
	};

	static const int dword_6FDE4ADC[] =
	{
		2, 2, 2, 0, 12
	};

	static const int dword_6FDE4AF0[] =
	{
		0, 3, 6, 0
	};

	static const int dword_6FDE4B00[] =
	{
		3, 3, 6, 0
	};

	static const D2UnkHireStrc2 stru_6FDE4B10[] =
	{
		{ 150, 3 },
		{ 300, 9 },
		{ 600, 15 },
		{ 0, 0 }
	};

	static const D2UnkHireStrc3 stru_6FDE4B30[] =
	{
		{ 0, 30, 0, 0 },
		{ 0, 60, 100, 2 },
		{ 0, 120, 0, 0 },
		{ 1, 180, 110, 3 },
		{ 0, 240, 0, 0 },
		{ 1, 400, 170, 3 },
	};

	static const D2UnkHireStrc3 stru_6FDE4B90[] =
	{
		{ 0, 15, 0, 0 },
		{ 1, 45, 80, 1 },
		{ 0, 25, 0, 0 },
		{ 1, 60, 90, 2 },
		{ 0, 80, 0, 0 },
		{ 1, 150, 125, 2 },
	};

	static const D2UnkHireStrc stru_6FDE4D70[] =
	{
		{ 0, 0, 0, 1, 1, 0, 1 },
		{ 80, 40, 25, 2, 2, 3, 2 },
		{ 120, 60, 40, 3, 3, 5, 3 }
	};

	static const D2UnkHireStrc4 stru_6FDE4C80[] =
	{
		{ 0, 0, 0, 0, 0 },
		{ 1, 55, 2, 0, 3371 },
		{ 2, 85, 2, 0, 3372 },
		{ 3, 0, 0, 0, 0 },
		{ 4, 65, 1, 3, 3373 },
		{ 5, 75, 1, 0, 3374 },
		{ 6, 0, 0, 2, 3375 },
		{ 7, 50, 1, 2, 3376 },
		{ 8, 0, 0, 2, 3377 },
		{ 9, 145, 1, 3, 3378 },
		{ 10, 190, 2, 3, 3379 },
		{ 11, 130, 1, 3, 3380 },
	};

	static const D2UnkHireStrc5 stru_6FDE4BF0[] =
	{
		{ 0, 1, 1, 3, 0, 0 },
		{ 1, 1, 2, 4, 75, 2 },
		{ 0, 1, 2, 6, 0, 0 },
		{ 1, 1, 3, 8, 125, 2 },
		{ 1, 1, 4, 10, 200, 4 },
		{ 0, 1, 3, 8, 0, 0 },
	};

	D2SeedStrc pSeed = {};
	int nIndex = 0;

	if (nLowSeed && pHirelingInit && a3 <= 2 && pHirelingInit->nId >= 0 && pHirelingInit->nId <= 3)
	{
		pHirelingInit->nFlags = 0;

		SEED_InitSeed(&pSeed);
		SEED_InitLowSeed(&pSeed, nLowSeed);

		pHirelingInit->field_34 = stru_6FDE4B10[pHirelingInit->nId].field_0;
		pHirelingInit->nMinDamage = stru_6FDE4B10[pHirelingInit->nId].field_4;

		nIndex = SEED_RollLimitedRandomNumber(&pSeed, dword_6FDE4A94[pHirelingInit->nId]) + dword_6FDE4A84[pHirelingInit->nId];
		pHirelingInit->nLevel = stru_6FDE4B30[nIndex].field_0;
		pHirelingInit->nExperience = stru_6FDE4B30[nIndex].field_4;
		pHirelingInit->nMinDamage += stru_6FDE4B30[nIndex].field_C;
		pHirelingInit->field_34 += stru_6FDE4B30[nIndex].field_8;

		nIndex = SEED_RollLimitedRandomNumber(&pSeed, dword_6FDE4AB8[pHirelingInit->nId]) + dword_6FDE4AA8[pHirelingInit->nId];
		pHirelingInit->nHitpoints = stru_6FDE4B90[nIndex].field_0;
		pHirelingInit->nDefense = stru_6FDE4B90[nIndex].field_4;
		pHirelingInit->nMinDamage += stru_6FDE4B90[nIndex].field_C;
		pHirelingInit->field_34 += stru_6FDE4B90[nIndex].field_8;

		nIndex = SEED_RollLimitedRandomNumber(&pSeed, dword_6FDE4ADC[pHirelingInit->nId]) + dword_6FDE4ACC[pHirelingInit->nId];
		if (pHirelingInit->nId >= 3)
		{
			pHirelingInit->nStrength = 0;
			pHirelingInit->nGold = stru_6FDE4BF0[nIndex].field_0;
		}
		else
		{
			pHirelingInit->nGold = 0;
			pHirelingInit->nStrength = stru_6FDE4BF0[nIndex].field_0;
		}
		pHirelingInit->nMaxDamage = stru_6FDE4BF0[nIndex].field_8;
		pHirelingInit->nShare = stru_6FDE4BF0[nIndex].field_C;
		if (!stru_6FDE4BF0[nIndex].field_4)
		{
			pHirelingInit->nFlags |= 1;
		}
		pHirelingInit->nMinDamage += stru_6FDE4BF0[nIndex].field_14;
		pHirelingInit->field_34 += stru_6FDE4BF0[nIndex].field_10;

		nIndex = SEED_RollLimitedRandomNumber(&pSeed, dword_6FDE4B00[pHirelingInit->nId]) + dword_6FDE4AF0[pHirelingInit->nId];
		pHirelingInit->nResist = stru_6FDE4C80[nIndex].field_0;
		pHirelingInit->nHireDesc = stru_6FDE4C80[nIndex].field_C;
		pHirelingInit->nDexterity = stru_6FDE4C80[nIndex].field_0 >= 9 && stru_6FDE4C80[nIndex].field_0 <= 11;
		pHirelingInit->nMinDamage += stru_6FDE4C80[nIndex].field_8;
		pHirelingInit->field_34 += stru_6FDE4C80[nIndex].field_4;

		if (a3)
		{
			pHirelingInit->nExperience += stru_6FDE4D70[a3].field_0;
			pHirelingInit->nDefense += stru_6FDE4D70[a3].field_4;
			pHirelingInit->nMinDamage += stru_6FDE4D70[a3].field_8;
			pHirelingInit->nMaxDamage *= stru_6FDE4D70[a3].field_C;
			pHirelingInit->nShare *= stru_6FDE4D70[a3].field_10;
			pHirelingInit->nHireDesc += stru_6FDE4D70[a3].field_14;
			pHirelingInit->field_34 *= stru_6FDE4D70[a3].field_18;
		}

		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDA51A0 (#11085)
wchar_t* __fastcall MONSTERS_GetHirelingDescString(int nId)
{
	//D2Common.0x6FDD16E0
	static const short gnHirelingDescStringIds[9] =
	{
		0, 3371, 3372, 11090, 11088, 11089, 3377, 3376, 3375
	};

	if (nId >= 0 && nId < ARRAY_SIZE(gnHirelingDescStringIds) && gnHirelingDescStringIds[nId])
	{
		return D2LANG_GetStringFromTblIndex(gnHirelingDescStringIds[nId]);
	}

	return NULL;
}

//D2Common.0x6FDA51C0 (#11086)
int __fastcall MONSTERS_GetActFromHirelingTxt(BOOL bExpansion, int nClassId, uint16_t nNameId)
{
	D2HirelingTxt* pHirelingTxtRecord = NULL;

	pHirelingTxtRecord = DATATBLS_GetNextHirelingTxtRecordFromClassId(bExpansion, nClassId, NULL);
	if (pHirelingTxtRecord)
	{
		return pHirelingTxtRecord->dwAct - 1;
	}
	else
	{
		pHirelingTxtRecord = DATATBLS_GetNextHirelingTxtRecordFromNameId(bExpansion, nNameId, NULL);
		if (pHirelingTxtRecord)
		{
			return pHirelingTxtRecord->dwAct - 1;
		}
	}

	return 0;
}

//D2Common.0x6FDA5200 (#11084)
int __stdcall MONSTERS_GetHirelingExpForNextLevel(int nLevel, int nExpPerLevel)
{
	return nExpPerLevel * nLevel * nLevel * (nLevel + 1);
}

//D2Common.0x6FDA5220 (#11083)
int __stdcall MONSTERS_GetHirelingResurrectionCost(D2UnitStrc* pHireling)
{
	int nLevel = STATLIST_GetUnitStatUnsigned(pHireling, STAT_LEVEL, 0);
	int nCost = 15 * nLevel * nLevel / 2;

	if (nCost > 50000)
	{
		nCost = 50000;
	}

	return nCost;
}

//D2Common.0x6FDA5270 (#11068)
int __stdcall D2COMMON_11068_GetCompInfo(D2UnitStrc* pMonster, int nComponent)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && nComponent < ARRAY_SIZE(pMonStats2TxtRecord->unk0x15))
	{
		pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pMonster->dwClassId);
		if (pMonStats2TxtRecord)
		{
			return pMonStats2TxtRecord->unk0x15[nComponent];
		}
	}

	return 0;
}

//D2Common.0x6FDA52F0 (#11069)
uint32_t __stdcall D2Common_11069(D2UnitStrc* pMonster, unsigned int nIndex, unsigned int nComponent)
{
	static uint32_t dword_6FDE4DC8[7] =
	{
		' til', ' til', ' sed', ' sed', ' yvh', ' yvh', ' yvh'
	};

	static uint32_t dword_6FDE4DE4[3] =
	{
		' til', ' sed', ' yvh'
	};

	static uint32_t dword_6FDE4DF0[10] =
	{
		' exa', ' exa', ' alf', ' alf', ' xah', ' xah', ' cam', ' cam', ' mcs', 'mcs'
	};

	static uint32_t dword_6FDE4E18[5] =
	{
		0, ' cub', ' grl', ' tik', ' lms'
	};

	static uint32_t dword_6FDE4E2C[12] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, ' til', ' sed', ' yvh'
	};

	static uint32_t dword_6FDE4E5C[3] =
	{
		' til', ' dem', ' yvh'
	};

	static const uint32_t* off_6FDE4E68[16] =
	{
		dword_6FDE4DC8,
		dword_6FDE4E5C,
		dword_6FDE4DE4,
		dword_6FDE4DE4,
		dword_6FDE4DE4,
		dword_6FDE4DF0,
		NULL,
		dword_6FDE4E18,
		dword_6FDE4E2C,
		dword_6FDE4E2C,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	};

	static uint32_t dword_6FDE4EA8[3] =
	{
		' til', ' sed', ' yvh'
	};

	static uint32_t dword_6FDE4EB4[11] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, ' til', ' sed', ' yvh'
	};

	static uint32_t dword_6FDE4EE0[2] =
	{
		' wbs', 0
	};

	static const uint32_t* off_6FDE4EE8[16] =
	{
		dword_6FDE4EA8,
		dword_6FDE4E5C,
		dword_6FDE4EA8,
		dword_6FDE4EA8,
		dword_6FDE4EA8,
		NULL,
		dword_6FDE4EE0,
		NULL,
		dword_6FDE4EB4,
		dword_6FDE4EB4,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	};

	D2MonStats2Txt* pMonStats2TxtRecord = NULL;
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	D2RoomStrc* pRoom = NULL;
	const uint32_t* pCode = NULL;
	int nMonsterId = 0;
	int nLevelId = 0;
	uint32_t dwCode = 0;

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && nIndex < ARRAY_SIZE(pMonStats2TxtRecord->unk0x15))
	{
		pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(pMonster->dwClassId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->unk0x15[nIndex] > nComponent)
		{
			dwCode = DATATBLS_GetCodeFromCompCodeTxt(pMonStats2TxtRecord->unk0x26[nIndex].nComposit[nComponent]);

			pRoom = UNITS_GetRoom(pMonster);
			if (pRoom)
			{
				nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
				if (DRLG_GetActNoFromLevelId(nLevelId) == ACT_II)
				{
					nMonsterId = pMonster->dwClassId;
					if (nMonsterId >= 0)
					{
						pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);
						if (pMonStatsTxtRecord)
						{
							nMonsterId = pMonStatsTxtRecord->nBaseId;
						}

						if (nMonsterId == MONSTER_SKELETON1)
						{
							pCode = off_6FDE4E68[nIndex];
							if (pCode)
							{
								return pCode[nComponent];
							}
						}
						else if (nMonsterId == MONSTER_SK_ARCHER1)
						{
							pCode = off_6FDE4EE8[nIndex];
							if (pCode)
							{
								return pCode[nComponent];
							}
						}
					}
				}
			}

			return dwCode;
		}
	}

	return 0;
}

//D2Common.0x6FDA5450 (#11070)
int __stdcall D2Common_11070(int nMonsterId, unsigned int nComponent, unsigned int a3)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;

	if (nComponent < ARRAY_SIZE(pMonStats2TxtRecord->unk0x15))
	{
		pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(nMonsterId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->unk0x15[nComponent] > a3)
		{
			return DATATBLS_GetCodeFromCompCodeTxt(pMonStats2TxtRecord->unk0x26[nComponent].nComposit[a3]);
		}
	}

	return 0;
}

//D2Common.0x6FDA54E0 (#11050)
int __stdcall D2Common_11050(D2UnitStrc* pUnit, int a2)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nFrameCount = 0;
	int nCounter = 0;
	int nFrame = 0;
	int nStart = 0;

	if (a2)
	{
		if (!pUnit->nActionFrame)
		{
			return -1;
		}

		UNITS_GetFrameMetrics(pUnit, &nFrame, &nFrameCount);

		nStart = (nFrame - pUnit->wAnimSpeed) >> 8;
		nFrame >>= 8;

		nCounter = nStart;
		while (nCounter <= nFrame)
		{
			if (UNITS_GetEventFrameInfo(pUnit, nCounter) == MONSEQ_EVENT_MISSILE_ATTACK || UNITS_GetEventFrameInfo(pUnit, nCounter) == MONSEQ_EVENT_MELEE_ATTACK)
			{
				break;
			}

			++nCounter;
		}

		if (nCounter > nFrame)
		{
			return -1;
		}

		if (nCounter <= nStart)
		{
			return -1;
		}
	}

	if (!pUnit)
	{
		return -1;
	}
	
	pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
	if (pMonStatsTxtRecord)
	{
		switch (pUnit->dwAnimMode)
		{
		case MONMODE_ATTACK1:
			return pMonStatsTxtRecord->wMissA1;

		case MONMODE_ATTACK2:
			return pMonStatsTxtRecord->wMissA2;

		case MONMODE_CAST:
			return pMonStatsTxtRecord->wMissC;

		case MONMODE_SKILL1:
			return pMonStatsTxtRecord->wMissS1;

		case MONMODE_SKILL2:
			return pMonStatsTxtRecord->wMissS2;

		case MONMODE_SKILL3:
			return pMonStatsTxtRecord->wMissS3;

		case MONMODE_SKILL4:
			return pMonStatsTxtRecord->wMissS4;

		case MONMODE_SEQUENCE:
			return pMonStatsTxtRecord->wMissSQ;

		default:
			return -1;
		}
	}

	return -1;
}

//D2Common.0x6FDA55E0 (#11052)
char __stdcall D2Common_11052(uint8_t a1)
{
	//D2Common.0x6FDE4F28
	static const char byte_6FDE4F28[] =
	{
		0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7,
		8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 0, 0
	};

	return byte_6FDE4F28[a1];
}

//D2Common.0x6FDA5600 (#11053)
char __stdcall D2Common_11053(uint8_t a1)
{
	//D2Common.0x6FDE4F68
	static const char byte_6FDE4F68[] =
	{
		0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4,
		4, 4, 4, 4,	5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0
	};

	return byte_6FDE4F68[a1];
}

//D2Common.0x6FDA5620 (#11054)
char __stdcall D2Common_11054(uint8_t a1)
{
	//D2Common.0x6FDE4FA8
	static const char byte_6FDE4FA8[] =
	{
		4, 12, 20, 28, 36, 44, 52, 60
	};

	return byte_6FDE4FA8[a1];
}

//D2Common.0x6FDA5640 (#11055)
void __stdcall D2Common_11055(uint8_t a1, int* a2, int* a3)
{
	//D2Common.0x6FDD1738
	static const char byte_6FDD1738[] =
	{
		0, -1, -1, -1, 0, 1, 1, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -3, -3, -3, 0, 3, 3, 3
	};

	//D2Common.0x6FDD1758
	static const char byte_6FDD1758[] =
	{
		-1, -1, 0, 1, 1, 1, 0, -1, -2, -2, -2, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -3, -3, 0, 3, 3, 3, 0, -3
	};

	*a2 = byte_6FDD1738[a1];
	*a3 = byte_6FDD1758[a1];
}

//D2Common.0x6FDA5670 (#11297)
void __fastcall MONSTERS_SetMonsterNameInMonsterData(D2UnitStrc* pMonster, wchar_t* wszName)
{
	unsigned int nSize = 0;

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pPlayerData)
	{
		if (wszName)
		{
			nSize = sizeof(wchar_t) * (wcslen(wszName) + 1);
			pMonster->pMonsterData->wszMonName = (wchar_t*)FOG_AllocClientMemory(nSize, __FILE__, __LINE__, 0);
			wcscpy_s(pMonster->pMonsterData->wszMonName, nSize, wszName);
		}
	}
}

//D2Common.0x6FDA56C0
BOOL __fastcall MONSTERS_CanBeInTown(D2UnitStrc* pMonster)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER)
	{
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pMonster->dwClassId);
		if (pMonStatsTxtRecord)
		{
			if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NPC] || pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTOWN] || pMonster->dwFlags & UNITFLAG_ISREVIVE)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

//D2Common.0x6FDA5750 (#11057)
BOOL __stdcall MONSTESR_IsSandLeaper(D2UnitStrc* pMonster, BOOL bAlwaysReturnFalse)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nClassId = 0;

	if (bAlwaysReturnFalse)
	{
		return FALSE;
	}

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER)
	{
		nClassId = pMonster->dwClassId;
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nClassId);
		if (pMonStatsTxtRecord)
		{
			nClassId = pMonStatsTxtRecord->nBaseId;
		}
	}
	else
	{
		nClassId = -1;
	}

	if (sgptDataTables->nMonStatsTxtRecordCount > MONSTER_SANDLEAPER1)
	{
		if (nClassId == MONSTER_SANDLEAPER1)
		{
			return TRUE;
		}
	}
	else
	{
		if (nClassId == -1)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//D2Common.0x6FDA57D0 (#11058)
BOOL __stdcall MONSTERS_IsDemon(D2UnitStrc* pMonster)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER)
	{
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pMonster->dwClassId);
		if (pMonStatsTxtRecord)
		{
			return (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_DEMON]) != 0;
		}
	}

	return FALSE;
}

//D2Common.0x6FDA5830 (#11059)
BOOL __stdcall MONSTERS_IsUndead(D2UnitStrc* pMonster)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER)
	{
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pMonster->dwClassId);
		if (pMonStatsTxtRecord)
		{
			return pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_LUNDEAD] || pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_HUNDEAD];
		}
	}

	return FALSE;
}

//D2Common.0x6FDA58A0 (#11060)
BOOL __stdcall MONSTERS_IsBoss(D2MonStatsTxt* pMonStatsTxtRecord, D2UnitStrc* pMonster)
{
	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER)
	{
		if (!pMonStatsTxtRecord)
		{
			pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pMonster->dwClassId);
		}

		if (pMonStatsTxtRecord)
		{
			return pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_BOSS];
		}
	}

	return FALSE;
}

//D2Common.0x6FDA5900 (#11064)
BOOL __stdcall MONSTERS_IsDead(D2UnitStrc* pMonster)
{
	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER)
	{
		return pMonster->dwAnimMode == MONMODE_DEATH || pMonster->dwAnimMode == MONMODE_DEAD;
	}

	return FALSE;
}

//D2Common.0x6FDA5930 (#11280)
int __stdcall MONSTERS_GetSpawnMode_XY(D2UnitStrc* pMonster, BOOL bFromMonster, int nSkillId, int nUnused, int* pSpawnMode, int* pX, int* pY)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	D2SkillsTxt* pSkillsTxtRecord = NULL;
	D2CoordStrc pCoords = {};

	if (bFromMonster && pMonster && pMonster->dwUnitType == UNIT_MONSTER)
	{
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pMonster->dwClassId);
		if (pMonStatsTxtRecord)
		{
			UNITS_GetCoords(pMonster, &pCoords);

			if (pX)
			{
				*pX = pCoords.nX + pMonStatsTxtRecord->nSpawnX;
			}

			if (pY)
			{
				*pY = pCoords.nY + pMonStatsTxtRecord->nSpawnY;
			}

			if (pSpawnMode)
			{
				*pSpawnMode = pMonStatsTxtRecord->nSpawnMode;
				if (pMonStatsTxtRecord->nSpawnMode < 0 || pMonStatsTxtRecord->nSpawnMode >= 16)
				{
					*pSpawnMode = MONMODE_NEUTRAL;
				}
			}

			return pMonStatsTxtRecord->wSpawn;
		}
	}
	else
	{
		pSkillsTxtRecord = DATATBLS_GetSkillsTxtRecord(nSkillId);
		if (pSkillsTxtRecord && pSkillsTxtRecord->wSummon >= 0 && pSkillsTxtRecord->wSummon < sgptDataTables->nMonStatsTxtRecordCount)
		{
			UNITS_GetCoords(pMonster, &pCoords);

			if (pX)
			{
				*pX = pCoords.nX;
			}

			if (pY)
			{
				*pY = pCoords.nY;
			}

			if (pSpawnMode)
			{
				*pSpawnMode = pSkillsTxtRecord->nSumMode;
				if (pSkillsTxtRecord->nSumMode < 0 || pSkillsTxtRecord->nSumMode >= 16)
				{
					*pSpawnMode = MONMODE_NEUTRAL;
				}
			}

			return pSkillsTxtRecord->wSummon;
		}
	}

	return -1;
}

//D2Common.0x6FDA5B30 (#11061)
void __stdcall MONSTERS_GetMinionSpawnInfo(D2UnitStrc* pMonster, int* pId, int* pX, int* pY, int* pSpawnMode, int nDifficulty, int(__fastcall* pfSpawnClassCallback)(D2UnitStrc*))
{
	int nChainId = 0;
	int nBaseId = 0;
	D2CoordStrc pCoords = {};

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER)
	{
		nBaseId = MONSTERS_GetBaseIdFromMonsterId(pMonster->dwClassId);

		if (nBaseId < 0 || nBaseId >= sgptDataTables->nMonStatsTxtRecordCount)
		{
			nBaseId = -1;
		}
	}
	else
	{
		nBaseId = -1;
	}

	switch (nBaseId)
	{
	case MONSTER_CROWNEST1:
		DATATBLS_GetMonsterChainInfo(pMonster ? pMonster->dwClassId : -1, 0, &nChainId);
		*pId = MONSTERS_GetClassIdFromMonsterChain(MONSTERS_ValidateMonsterId(MONSTER_FOULCROW1), nChainId);

		UNITS_GetCoords(pMonster, &pCoords);

		*pX = pCoords.nX;
		*pY = pCoords.nY + 3;
		*pSpawnMode = MONMODE_NEUTRAL;
		break;

	case MONSTER_SARCOPHAGUS:
		*pId = D2Common_11063(UNITS_GetRoom(pMonster), MONSTERS_ValidateMonsterId(MONSTER_MUMMY1));

		UNITS_GetCoords(pMonster, &pCoords);

		*pX = pCoords.nX;
		*pY = pCoords.nY + 2;
		*pSpawnMode = MONMODE_NEUTRAL;
		break;

	case MONSTER_BLOODRAVEN:
		*pId = MONSTERS_ValidateMonsterId(MONSTER_ZOMBIE2);
		*pX = D2COMMON_10175_PathGetFirstPointX(pMonster->pDynamicPath);
		*pY = D2COMMON_10176_PathGetFirstPointY(pMonster->pDynamicPath);
		*pSpawnMode = MONMODE_SKILL1;
		break;

	case MONSTER_MAGGOTQUEEN1:
		DATATBLS_GetMonsterChainInfo(pMonster ? pMonster->dwClassId : -1, 0, &nChainId);
		*pId = MONSTERS_GetClassIdFromMonsterChain(MONSTERS_ValidateMonsterId(MONSTER_SANDMAGGOT1), nChainId);

		UNITS_GetCoords(pMonster, &pCoords);

		*pX = pCoords.nX + 8;
		*pY = pCoords.nY;
		*pSpawnMode = MONMODE_SKILL1;
		break;

	case MONSTER_VILEMOTHER1:
		DATATBLS_GetMonsterChainInfo(pMonster ? pMonster->dwClassId : -1, 0, &nChainId);
		*pId = MONSTERS_GetClassIdFromMonsterChain(MONSTERS_ValidateMonsterId(MONSTER_VILECHILD1), nChainId);

		*pX = D2COMMON_10175_PathGetFirstPointX(pMonster->pDynamicPath);
		*pY = D2COMMON_10176_PathGetFirstPointY(pMonster->pDynamicPath);
		*pSpawnMode = MONMODE_NEUTRAL;
		break;

	case MONSTER_EVILHOLE1:
		*pId = MONSTERS_ValidateMonsterId(MONSTER_FALLEN1);

		UNITS_GetCoords(pMonster, &pCoords);

		*pX = pCoords.nX;
		*pY = pCoords.nY;
		*pSpawnMode = MONMODE_NEUTRAL;
		break;

	case MONSTER_SUCKERNEST1:
		DATATBLS_GetMonsterChainInfo(pMonster->dwClassId, 0, &nChainId);
		*pId = MONSTERS_GetClassIdFromMonsterChain(MONSTERS_ValidateMonsterId(MONSTER_MOSQUITO1), nChainId);

		UNITS_GetCoords(pMonster, &pCoords);

		*pX = pCoords.nX - 2;
		*pY = pCoords.nY - 2;
		*pSpawnMode = MONMODE_NEUTRAL;
		break;

	case MONSTER_MINIONSPAWNER1:
		DATATBLS_GetMonsterChainInfo(pMonster->dwClassId, 0, &nChainId);
		*pId = MONSTERS_GetClassIdFromMonsterChain(MONSTERS_ValidateMonsterId(MONSTER_MINION1), nChainId);

		UNITS_GetCoords(pMonster, &pCoords);

		*pX = pCoords.nX;
		*pY = pCoords.nY + 3;
		*pSpawnMode = MONMODE_NEUTRAL;
		break;

	case MONSTER_NIHLATHAKBOSS:
	case MONSTER_EVILHUT:
		D2_ASSERT(pfSpawnClassCallback);

		*pId = pfSpawnClassCallback(pMonster);

		UNITS_GetCoords(pMonster, &pCoords);

		*pX = pCoords.nX + 2;
		*pY = pCoords.nY + 2;
		*pSpawnMode = MONMODE_NEUTRAL;
		break;

	case MONSTER_ANCIENTSTATUE1:
		DATATBLS_GetMonsterChainInfo(pMonster->dwClassId, 0, &nChainId);
		*pId = MONSTERS_GetClassIdFromMonsterChain(MONSTERS_ValidateMonsterId(MONSTER_ANCIENTBARB1), nChainId);

		UNITS_GetCoords(pMonster, &pCoords);

		*pX = pCoords.nX;
		*pY = pCoords.nY + 2;
		*pSpawnMode = MONMODE_NEUTRAL;

		STATES_ToggleState(pMonster, STATE_INVIS, 1);
		break;

	case MONSTER_BAALCRAB:
		UNITS_GetCoords(pMonster, &pCoords);

		if (*pId == MONSTERS_ValidateMonsterId(MONSTER_BAALCLONE))
		{
			*pX = pCoords.nX + SEED_RollLimitedRandomNumber(&pMonster->pSeed, 24) - 12;
			*pY = pCoords.nY + SEED_RollLimitedRandomNumber(&pMonster->pSeed, 24) - 12;
			*pSpawnMode = MONMODE_NEUTRAL;
		}
		else
		{
			*pId = MONSTERS_GetClassIdFromMonsterChain(MONSTERS_ValidateMonsterId(MONSTER_BAALTENTACLE1), nDifficulty + SEED_RollLimitedRandomNumber(&pMonster->pSeed, 2));
			*pX = pCoords.nX + SEED_RollLimitedRandomNumber(&pMonster->pSeed, 24) - 12;
			*pY = pCoords.nY + SEED_RollLimitedRandomNumber(&pMonster->pSeed, 24) - 12;
			*pSpawnMode = MONMODE_ATTACK1;
		}
		break;

	default:
		*pId = MONSTERS_ValidateMonsterId(0);
		*pX = 0;
		*pY = 0;
		*pSpawnMode = MONMODE_NEUTRAL;
		break;
	}
}

//D2Common.0x6FDA6410 (#11051)
uint8_t __stdcall MONSTERS_GetMaximalLightRadius(D2UnitStrc* pMonster)
{
	D2ItemsTxt* pItemsTxtRecord = NULL;
	uint8_t nMaxLightRadius = 0;
	int nCode = 0;
	int nItemId = 0;

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData)
	{
		nMaxLightRadius = 0;

		for (int i = 0; i < 16; ++i)
		{
			nCode = D2Common_11069(pMonster, i, pMonster->pMonsterData->nComponent[i]);

			if (nCode && nCode != ' til' && nCode != ' dem' && nCode != ' yvh')
			{
				pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(nCode, &nItemId);
				if (pItemsTxtRecord && pItemsTxtRecord->nLightRadius > nMaxLightRadius)
				{
					nMaxLightRadius = pItemsTxtRecord->nLightRadius;
				}
			}
		}

		return nMaxLightRadius;
	}

	return 0;
}

//D2Common.0x6FDA64B0 (#11063)
int __stdcall D2Common_11063(D2RoomStrc* pRoom, int nMonsterId)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	D2LevelsTxt* pLevelsTxtRecord = NULL;
	int nMonstersInChain = 0;
	int nNextInClass = 0;
	int nClassId = 0;
	int nResult = 0;

	pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(DUNGEON_GetLevelIdFromRoom(pRoom));

	if (pLevelsTxtRecord && pLevelsTxtRecord->nNumNormMon)
	{
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);
		if (pMonStatsTxtRecord)
		{
			nClassId = pMonStatsTxtRecord->nBaseId;

			pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nClassId);
			if (pMonStatsTxtRecord)
			{
				for (int i = 0; i < pLevelsTxtRecord->nNumNormMon; ++i)
				{
					if (MONSTERS_GetBaseIdFromMonsterId(pLevelsTxtRecord->wMon[i]) == nClassId)
					{
						return pLevelsTxtRecord->wMon[i];
					}
				}

				DATATBLS_GetMonsterChainInfo(nMonsterId, &nMonstersInChain, NULL);

				nResult = nMonsterId;
				nNextInClass = pMonStatsTxtRecord->nNextInClass;

				for (int i = 0; i < nMonstersInChain; ++i)
				{
					pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nNextInClass);
					if (!pMonStatsTxtRecord || pMonStatsTxtRecord->nLevel[0] > pLevelsTxtRecord->wMonLvlEx[0] + 1)
					{
						return nResult;
					}

					nResult = nNextInClass;

					nNextInClass = pMonStatsTxtRecord->nNextInClass;
				}

				return nResult;
			}
		}
	}

	return nMonsterId;
}

//D2Common.0x6FDA6620 (#11065)
BOOL __stdcall MONSTERS_IsPrimeEvil(D2UnitStrc* pMonster)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;

	if (pMonster && pMonster->dwUnitType == UNIT_MONSTER)
	{
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pMonster->dwClassId);
		if (pMonStatsTxtRecord)
		{
			return pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_PRIMEEVIL];
		}
	}

	return FALSE;
}

//D2Common.0x6FDA6680 (#11066)
void __stdcall D2Common_11066(D2UnitStrc* pMonster, int* pDirectionX, int* pDirectionY)
{
	//D2Common.0x6FDD1778
	static const D2CoordStrc dword_6FDD1778[] =
	{
		{  0, 1 },
		{  1, 0 },
		{  1, 0 },
		{ -1, 0 },
	};

	int nIndex = 0;

	D2_ASSERT(pMonster && pMonster->dwUnitType == UNIT_MONSTER);

	DATATBLS_GetMonsterChainInfo(pMonster->dwClassId, NULL, &nIndex);

	D2_ASSERT(nIndex < ARRAY_SIZE(dword_6FDD1778));

	D2_ASSERT(nIndex >= 0);

	*pDirectionX = dword_6FDD1778[nIndex].nX;
	*pDirectionY = dword_6FDD1778[nIndex].nY;
}

//D2Common.0x6FDA6730 (#11067)
int __stdcall MONSTERS_GetHirelingTypeId(D2UnitStrc* pHireling)
{
	if (pHireling && pHireling->dwUnitType == UNIT_MONSTER)
	{
		switch (pHireling->dwClassId)
		{
		case MONSTER_ROGUEHIRE:
			return 1;

		case MONSTER_ACT2HIRE:
			return 2;

		case MONSTER_ACT3HIRE:
			return 3;

		case MONSTER_ACT5HIRE1:
		case MONSTER_ACT5HIRE2:
			return 4;
		}
	}

	return 0;
}

//D2Common.0x6FDA6790 (#11246)
void __stdcall D2Common_11246(D2UnitStrc* pMonster, int a2, uint8_t a3)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;

	if (!a2 && a3 && pMonster && pMonster->dwUnitType == UNIT_MONSTER)
	{
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pMonster->dwClassId);
		if (pMonStatsTxtRecord && pMonStatsTxtRecord->nAlign != 1)
		{
			STATLIST_SetUnitStat(pMonster, STAT_MAXHP, STATLIST_GetUnitStatUnsigned(pMonster, STAT_MAXHP, 0) / 2, 0);
			STATLIST_SetUnitStat(pMonster, STAT_ARMORCLASS, 10 * STATLIST_GetUnitStatUnsigned(pMonster, STAT_ARMORCLASS, 0) / 12, 0);
			STATLIST_SetUnitStat(pMonster, STAT_EXPERIENCE, 10 * STATLIST_GetUnitStatUnsigned(pMonster, STAT_EXPERIENCE, 0) / (a3 == 1 ? 17 : 26), 0);
			STATLIST_SetUnitStat(pMonster, STAT_LEVEL, 25 * a3 + pMonStatsTxtRecord->nLevel[0], 0);
		}
	}
}

//D2Common.0x6FDA6920
int __fastcall MONSTERS_GetBaseIdFromMonsterId(int nMonsterId)
{
	D2MonStatsTxt* pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);

	if (pMonStatsTxtRecord)
	{
		return pMonStatsTxtRecord->nBaseId;
	}

	return nMonsterId;
}

//D2Common.0x6FDA6950
int __fastcall MONSTERS_GetClassIdFromMonsterChain(int nMonsterId, int nChainId)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nClassId = 0;

	pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nMonsterId);
	if (pMonStatsTxtRecord)
	{
		nClassId = pMonStatsTxtRecord->nBaseId;
		for (int i = 0; i < nChainId; ++i)
		{
			nClassId = pMonStatsTxtRecord->nNextInClass;

			pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nClassId);
			if (!pMonStatsTxtRecord)
			{
				break;
			}
		}

		return nClassId;
	}

	return -1;
}

//D2Common.0x6FDA69C0
int __fastcall MONSTERS_ValidateMonsterId(int nMonsterId)
{
	if (nMonsterId >= 0 && nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
	{
		return nMonsterId;
	}

	return -1;
}
