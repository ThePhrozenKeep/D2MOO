#include "Drlg/D2DrlgOutJung.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgOutdoors.h"
#include "D2Seed.h"
#include <DataTbls/LevelsIds.h>

//TODO: Find names, rename variables + globals

//D2Common.0x6FD7FC20
//TODO: v19
void __fastcall DRLGOUTJUNG_BuildJungle(D2DrlgLevelStrc* pLevel)
{
	static const int dword_6FDCFB18[] =
	{
		0, 10, 20, 0
	};

	static const int dword_6FDCFB28[] =
	{
		0, 1, 2, 1, 0, 2, 0, 2, 1, 1, 2, 0, 2, 0, 1, 2, 1, 0
	};

	D2LevelDefBin* pLevelDefBin = NULL;
	int nFileIndex = 0;
	int nJungleDef = 0;
	int nDefId = 0;
	int nSizeX = 0;
	int nSizeY = 0;
	int nRand = 0;
	int v19 = 0;

	if (pLevel->nLevelId >= LEVEL_SPIDERFOREST && pLevel->nLevelId <= LEVEL_FLAYERJUNGLE)
	{
		pLevelDefBin = DATATBLS_GetLevelDefRecord(LEVEL_SPIDERFOREST);
		nSizeX = pLevelDefBin->dwSizeX[pLevel->pDrlg->nDifficulty] >> 5;
		nSizeY = pLevelDefBin->dwSizeY[pLevel->pDrlg->nDifficulty] >> 5;

		nRand = SEED_RollLimitedRandomNumber(&pLevel->pSeed, 4 * (pLevel->nJungleDefs == 3) + 2);

		if (!pLevel->pJungleDefs)
		{
			FOG_10025("ptDrlgLevel->pnJungleDefs", __FILE__, __LINE__);
		}

		for (int i = 0; i < nSizeY; ++i)
		{
			if (pLevel->nLevelId == LEVEL_SPIDERFOREST && i == nSizeY - 1)
			{
				DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, 4 * i, LVLPREST_ACT3_JUNGLE_HEAD, pLevel->pJungleDefs[nSizeX * nSizeY - 1] == 0, 0);
				nDefId += 2;
			}
			else if (pLevel->nLevelId == LEVEL_FLAYERJUNGLE && i == 0)
			{
				DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, 0, LVLPREST_ACT3_JUNGLE_TAIL, pLevel->pJungleDefs[1] == 0, 0);
				nDefId += 2;
			}
			else
			{
				for (int j = 0; j < nSizeX; ++j)
				{
					++nDefId;
					nJungleDef = pLevel->pJungleDefs[nDefId - 1];
					if (nJungleDef > LVLPREST_ACT3_JUNGLE_TAIL)
					{
						if (nFileIndex >= 3)
						{
							FOG_10025("nFileIndex < 3", __FILE__, __LINE__);
						}

						nJungleDef += dword_6FDCFB18[pLevel->nLevelId - 76];
						v19 = dword_6FDCFB28[nFileIndex + 3 * nRand];
						++nFileIndex;
					}
					else
					{
						v19 = -1;
					}

					if (nJungleDef)
					{
						DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 4 * j, 4 * i, nJungleDef, v19, 0);
					}
				}
			}
		}
	}
}

//D2Common.0x6FD7FE50
//TODO: v5, v10
void __fastcall DRLGOUTJUNG_BuildLowerKurast(D2DrlgLevelStrc* pLevel)
{
	int nWidth = pLevel->nWidth / 8 - 1;
	int nHeight = pLevel->nHeight / 8 - 1;
	int v5 = (pLevel->nWidth / 8 - 1) / 2;
	int v10 = 0;
	
	if (pLevel->pDrlg->bJungleInterlink)
	{
		v10 = 1;
	}
	else
	{
		v10 = nWidth - 1;
	}

	for (int i = 1; i < nWidth; ++i)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, i, 0, 8 * (i == v10) + LVLPREST_ACT3_SLUMS_BORDER_N, -1, 0);
	}

	for (int i = 1; i < nWidth; i += (i == v5) + 1)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, i, nHeight, 8 * (i == v5) + LVLPREST_ACT3_SLUMS_BORDER_S, -1, 0);
	}

	for (int i = 1; i < nHeight; ++i)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nWidth, i, LVLPREST_ACT3_SLUMS_BORDER_E, -1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, i, LVLPREST_ACT3_SLUMS_BORDER_W, -1, 0);
	}

	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, 0, LVLPREST_ACT3_SLUMS_BORDER_NW, -1, 0);
	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nWidth, 0, LVLPREST_ACT3_SLUMS_BORDER_NE, -1, 0);
	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, nHeight, LVLPREST_ACT3_SLUMS_BORDER_SW, -1, 0);
	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nWidth, nHeight, LVLPREST_ACT3_SLUMS_BORDER_SE, -1, 0);
}

//D2Common.0x6FD7FFA0
//TODO: v7, v8
void __fastcall DRLGOUTJUNG_BuildKurastBazaar(D2DrlgLevelStrc* pLevel)
{
	int nWidth = pLevel->nWidth / 8 - 1;
	int nHeight = pLevel->nHeight / 8 - 1;
	int v7 = 0;
	int v8 = 0;

	if (pLevel->pDrlg->bJungleInterlink)
	{
		v7 = pLevel->nWidth / 8 - 2;
		v8 = 1;
	}
	else
	{
		v7 = 1;
		v8 = pLevel->nWidth / 8 - 2;
	}

	for (int i = 1; i < nWidth; ++i)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, i, 0, 8 * (i == v7) + LVLPREST_ACT3_BURBS_BORDER_N, -1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, i, nHeight, 8 * (i == v8) + LVLPREST_ACT3_BURBS_BORDER_S, -1, 0);
	}

	for (int i = 1; i < nHeight; ++i)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nWidth, i, LVLPREST_ACT3_BURBS_BORDER_E, -1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, i, LVLPREST_ACT3_BURBS_BORDER_W, -1, 0);
	}

	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, 0, LVLPREST_ACT3_BURBS_BORDER_NW, -1, 0);
	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nWidth, 0, LVLPREST_ACT3_BURBS_BORDER_NE, -1, 0);
	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, nHeight, LVLPREST_ACT3_BURBS_BORDER_SW, -1, 0);
	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nWidth, nHeight, LVLPREST_ACT3_BURBS_BORDER_SE, -1, 0);
}

//D2Common.0x6FD800E0
//TODO: v8
void __fastcall DRLGOUTJUNG_BuildUpperKurast(D2DrlgLevelStrc* pLevel)
{
	int nWidth = pLevel->nWidth / 8 - 1;
	int nHeight = pLevel->nHeight / 8 - 1;
	int v8 = 0;

	if (pLevel->pDrlg->bJungleInterlink)
	{
		v8 = nWidth - 1;
	}
	else
	{
		v8 = 1;
	}

	for (int i = 1; i < nWidth; i += (i == nWidth / 2) + 1)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, i, 0, 8 * (i == nWidth / 2) + LVLPREST_ACT3_METRO_BORDER_N, -1, 0);
	}

	for (int i = 1; i < nWidth; ++i)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, i, nHeight, 8 * (i == v8) + LVLPREST_ACT3_METRO_BORDER_S, -1, 0);
	}

	for (int i = 1; i < nHeight; ++i)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nWidth, i, LVLPREST_ACT3_METRO_BORDER_E, -1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, i, LVLPREST_ACT3_METRO_BORDER_W, -1, 0);
	}

	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, 0, LVLPREST_ACT3_METRO_BORDER_NW, -1, 0);
	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nWidth, 0, LVLPREST_ACT3_METRO_BORDER_NE, -1, 0);
	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, nHeight, LVLPREST_ACT3_METRO_BORDER_SW, -1, 0);
	DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nWidth, nHeight, LVLPREST_ACT3_METRO_BORDER_SE, -1, 0);
}

//D2Common.0x6FD80230
//TODO: Rename variables
void __fastcall DRLGOUTJUNG_SpawnRandomPreset(D2DrlgLevelStrc* pLevel, int nLevelPrestId1, int nLevelPrestId2, int a4)
{
	int v8; // edi@1
	int *v11; // ebx@2
	int v12; // eax@3
	int v13; // edx@3
	unsigned int v14; // ecx@7
	unsigned int v19; // edx@12
	int v23; // eax@16
	int v24; // ST44_4@16
	unsigned int v26; // edx@24
	int v34; // [sp+10h] [bp-28h]@1
	int a3a; // [sp+18h] [bp-20h]@23
	int a2b; // [sp+20h] [bp-18h]@23
	unsigned int nVariants; // [sp+3Ch] [bp+4h]@1

	nVariants = nLevelPrestId2 - nLevelPrestId1 + 1;
	v34 = 0;

	v8 = pLevel->pOutdoors->nGridWidth * pLevel->pOutdoors->nGridHeight;

	if (v8)
	{
		v11 = (int*)D2_ALLOC_POOL(pLevel->pDrlg->pMempool, 8 * v8);
	
		for (int i = 0; i < v8; ++i)
		{
			v12 = i / pLevel->pOutdoors->nGridWidth;
			v13 = i % pLevel->pOutdoors->nGridWidth;

			v11[2 * i] = v13;
			v11[2 * i + 1] = v12;
		}

		for (int i = 0; i < v8; ++i)
		{
			v14 = SEED_RollLimitedRandomNumber(&pLevel->pSeed, v8);
			v19 = SEED_RollLimitedRandomNumber(&pLevel->pSeed, v8);

			v23 = v11[2 * v14];
			v24 = v11[2 * v14 + 1];

			v11[2 * v14] = v11[2 * v19];
			v11[2 * v14 + 1] = v11[2 * v19 + 1];

			v11[2 * v19] = v23;
			v11[2 * v19 + 1] = v24;
		}

		for (int i = 0; i < v8; ++i)
		{
			a2b = v11[2 * i];
			a3a = v11[2 * i + 1];

			v26 = SEED_RollLimitedRandomNumber(&pLevel->pSeed, nVariants);

			if (DRLGOUTDOORS_TestOutdoorLevelPreset(pLevel, a2b, a3a, v26 + nLevelPrestId1, 0, 15))
			{
				DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, a2b, a3a, v26 + nLevelPrestId1, -1, 0);

				++v34;

				if (a4 > 0 && v34 >= a4)
				{
					break;
				}
			}
		}

		D2_FREE_POOL(pLevel->pDrlg->pMempool, v11);
	}
}
