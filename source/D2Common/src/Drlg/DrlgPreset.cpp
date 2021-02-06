#include "Drlg/D2DrlgPreset.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgActivate.h"
#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgAnim.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgDrlgLogic.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "Drlg/D2DrlgDrlgWarp.h"
#include "Drlg/D2DrlgDrlgVer.h"
#include "Drlg/D2DrlgMaze.h"
#include "Drlg/D2DrlgRoomTile.h"
#include "Drlg/D2DrlgTileSub.h"
#include "D2Dungeon.h"
#include "D2Seed.h"
#include <D2CMP.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <D2Items.h>
#include <D2Monsters.h>
#include <Units/Units.h>


D2UnkFileStrc* dword_6FDEA700;


//D2Common.0x6FD859A0 (#11222)
int __stdcall DRLGPRESET_CountPresetObjectsByAct(uint8_t nAct)
{
	for (int i = 0; i < 150; ++i)
	{
		if (!DRLGPRESET_GetObjectIndexFromObjPreset(nAct, i))
		{
			return i;
		}
	}

	return 0;
}

//D2Common.0x6FD859E0 (#11223)
int __stdcall DRLGPRESET_GetObjectIndexFromObjPreset(uint8_t nAct, int nUnitId)
{
	static const int dword_6FDE1180[5][150] =
	{
		{
			12, 37, 39, 35, 36, 5, 17, 18, 19, 20, 21, 22, 30, 70, 70, 69, 69, 29, 31, 33, 34, 37, 61, 65, 66,
			8, 26, 28, 82, 2, 81, 84, 83, 78, 61, 103, 108, 119, 580, 130, 159, 163, 169, 160, 161, 162, 104, 105, 106, 107,
			179, 180, 119, 157, 247, 248, 155, 174, 175, 139, 140, 141, 144, 6, 240, 241, 242, 54, 55, 56, 57, 58, 171, 178, 239,
			245, 250, 111, 138, 132, 164, 165, 77, 85, 86, 262, 263, 264, 265, 50, 51, 79, 53, 1, 3, 7, 46, 38, 256, 257,
			258, 129, 267, 268, 269, 581, 351, 352, 353, 374, 385, 397, 321, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			74, 37, 192, 304, 305, 306, 101, 102, 78, 103, 156, 580, 132, 129, 357, 153, 121, 122, 229, 230, 196, 267, 261, 149, 269,
			4, 9, 52, 94, 95, 142, 143, 5, 6, 87, 88, 146, 146, 147, 148, 240, 241, 242, 243, 176, 177, 198, 246, 29, 160,
			161, 162, 273, 283, 85, 86, 109, 116, 134, 135, 136, 150, 151, 172, 173, 279, 280, 281, 282, 166, 167, 113, 137, 89, 104,
			105, 106, 107, 154, 171, 178, 270, 271, 272, 266, 274, 244, 284, 288, 298, 289, 296, 297, 287, 286, 285, 290, 291, 292, 293,
			294, 295, 133, 303, 299, 300, 301, 302, 581, 354, 582, 314, 315, 316, 317, 323, 322, 110, 112, 114, 355, 356, 357, 351, 352,
			353, 152, 374, 387, 389, 390, 391, 388, 397, 402, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			117, 237, 580, 130, 102, 37, 160, 161, 162, 104, 105, 106, 107, 194, 195, 193, 207, 211, 210, 234, 214, 215, 213, 228, 216,
			227, 217, 235, 218, 219, 220, 221, 223, 224, 267, 269, 581, 170, 325, 184, 190, 191, 197, 199, 200, 201, 202, 206, 278, 120,
			130, 326, 158, 271, 272, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 5, 6, 176, 240, 241, 181, 183, 246, 185, 186,
			187, 188, 203, 204, 205, 208, 209, 169, 323, 324, 196, 212, 225, 244, 351, 352, 353, 360, 361, 362, 365, 251, 252, 208, 283,
			367, 366, 368, 341, 342, 343, 344, 374, 370, 378, 379, 386, 397, 405, 407, 406, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			238, 580, 267, 269, 581, 573, 573, 573, 345, 346, 347, 348, 349, 350, 351, 352, 353, 358, 359, 363, 259, 373, 372, 374, 236,
			249, 226, 231, 232, 93, 97, 123, 124, 96, 225, 233, 222, 125, 126, 127, 128, 375, 376, 254, 253, 342, 255, 392, 393, 394,
			395, 396, 398, 397, 399, 401, 400, 380, 383, 384, 296, 297, 403, 102, 408, 409, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			452, 453, 338, 337, 267, 374, 482, 39, 35, 36, 33, 34, 38, 102, 411, 438, 412, 435, 436, 440, 441, 441, 442, 429, 420,
			431, 430, 413, 432, 433, 418, 419, 424, 425, 416, 414, 415, 427, 428, 421, 422, 423, 426, 451, 267, 443, 444, 445, 446, 447,
			448, 450, 451, 459, 460, 461, 462, 482, 473, 455, 456, 457, 458, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 477, 479,
			480, 481, 483, 484, 485, 486, 487, 454, 437, 508, 488, 493, 493, 495, 497, 499, 503, 509, 512, 489, 490, 514, 515, 493, 498,
			513, 494, 496, 511, 500, 501, 502, 504, 505, 506, 507, 510, 160, 161, 162, 269, 523, 434, 496, 496, 496, 527, 528, 538, 539,
			542, 543, 546, 547, 548, 549, 550, 551, 552, 555, 553, 554, 557, 541, 544, 559, 560, 564, 567, 568, 536, 537, 563, 570, 397,
		},
	};

	return dword_6FDE1180[nAct][nUnitId];
}


struct D2DS1FileStrc
{
	int field_0;
	int nWidth;
	int nHeight;
	int pData[1];
};

//D2Common.0x6FD85A10
void __fastcall DRLGPRESET_ParseDS1File(D2DrlgFileStrc* pDrlgFile, void* pMemPool, char* szFileName)
{
	D2MonPresetTxt* pMonPresetTxtSection = NULL;
	D2PresetUnitStrc* pNewPresetUnit = NULL;
	D2PresetUnitStrc* pPresetUnit = NULL;
	D2DS1FileStrc* pDS1File = NULL;
	int* pOrientationLayer = NULL;
	int* pData = NULL;
	int nMonPresetRecords = 0;
	int nSpawnFlag = 0;
	int nUnitType = 0;
	int nStrings = 0;
	int nUnitId = 0;
	int nUnits = 0;
	int nNodes = 0;
	int nMode = 0;
	int nArea = 0;
	int nAct = 0;
	int nX = 0;
	int nY = 0;
	int v72 = 0;
	int v59 = 0;
	char szItemCode[4] = {};

	pDS1File = (D2DS1FileStrc*)DATATBLS_GetBinaryData(pMemPool, szFileName, NULL, __FILE__, __LINE__);
	pDrlgFile->pDS1File = pDS1File;
	pDrlgFile->nWidth = pDS1File->nWidth;
	pDrlgFile->nHeight = pDS1File->nHeight;

	v72 = pDS1File->field_0;
	pData = pDS1File->pData;
	
	nAct = 0;
	if (v72 >= 8)
	{
		nAct = *pData;
		++pData;

		if (nAct > ACT_V)
		{
			nAct = ACT_V;
		}
	}

	pDrlgFile->unk0x00 = 0;
	if (v72 >= 10)
	{
		pDrlgFile->unk0x00 = *pData;
		++pData;
	}

	if (v72 >= 3)
	{
		nStrings = *pData;
		++pData;

		for (int i = 0; i < nStrings; ++i)
		{
			pData = (int*)((char*)pData + strlen((char*)pData) + 1);
		}
	}

	nArea = (pDrlgFile->nWidth + 1) * (pDrlgFile->nHeight + 1);

	if (v72 >= 9 && v72 < 14)
	{
		pData += 2;
	}

	if (v72 < 4)
	{
		pDrlgFile->pWallLayer[0] = pData;
		pData = &pData[nArea];
		pDrlgFile->nWalls = 1;
		pDrlgFile->pFloorLayer[0] = pData;
		pData = &pData[nArea];
		pDrlgFile->pOrientationLayer[0] = pData;
		pData = &pData[nArea];
		pDrlgFile->pSubstGroupTags = pData;
		pData = &pData[nArea];
	}
	else
	{
		pDrlgFile->nWalls = *pData;
		++pData;

		if (v72 < 16)
		{
			pDrlgFile->nFloors = 1;
		}
		else
		{
			pDrlgFile->nFloors = *pData;
			++pData;
		}

		for (int i = 0; i < pDrlgFile->nWalls; ++i)
		{
			pDrlgFile->pWallLayer[i] = pData;
			pData += nArea;
			pDrlgFile->pOrientationLayer[i] = pData;
			pData += nArea;
		}

		for (int i = 0; i < pDrlgFile->nFloors; ++i)
		{
			pDrlgFile->pFloorLayer[i] = pData;
			pData += nArea;
		}
	}

	if (v72 < 7)
	{
		for (int j = 0; j < pDrlgFile->nWalls; ++j)
		{
			pOrientationLayer = (int*)pDrlgFile->pOrientationLayer[j];
			for (int i = 0; i < nArea; ++i)
			{
				pOrientationLayer[i] = DRLGPRESET_MapOrientationLayer(pOrientationLayer[i]);
			}
		}
	}

	pDrlgFile->pShadowLayer = pData;
	pData = &pData[nArea];
	if (pDrlgFile->unk0x00 > 0 && pDrlgFile->unk0x00 <= 2)
	{
		pDrlgFile->pSubstGroupTags = pData;
		pData += nArea;
	}

	if (v72 > 1)
	{
		nUnits = *pData;
		++pData;

		for (int i = 0; i < nUnits; ++i)
		{
			nUnitType = *pData;
			++pData;
			nUnitId = *pData;
			++pData;

			nMode = 0;

			switch (nUnitType)
			{
			case UNIT_MONSTER:
				nMode = MONMODE_NEUTRAL;
				if (v72 > 4)
				{
					nMonPresetRecords = 0;
					pMonPresetTxtSection = DATATBLS_GetMonPresetTxtActSection(nAct, &nMonPresetRecords);

					if (pMonPresetTxtSection && nUnitId < nMonPresetRecords)
					{
						nUnitId = pMonPresetTxtSection[nUnitId].wPlace;

						switch (pMonPresetTxtSection[nUnitId].nType)
						{
						case 0:
							nUnitId += DATATBLS_GetSuperUniquesTxtRecordCount();
							nUnitId += sgptDataTables->nMonStatsTxtRecordCount;
							break;

						case 1:
							break;

						case 2:
							nUnitId += sgptDataTables->nMonStatsTxtRecordCount;
							break;

						default:
							nUnitId = -1;
							break;
						}
					}

					switch (nAct)
					{
					case ACT_III:
						switch (nUnitId)
						{
						case MONSTER_NATALYA:
							nUnitId = OBJECT_NATALYA_START;
							nUnitType = UNIT_OBJECT;
							nMode = OBJMODE_NEUTRAL;
							break;
						
						case MONSTER_COMPELLINGORB:
							nUnitId = OBJECT_COMPELLING_ORB;
							nUnitType = UNIT_OBJECT;
							nMode = OBJMODE_NEUTRAL;
							break;

						default:
							break;
						}
						break;

					case ACT_V:
						switch (nUnitId)
						{
						case MONSTER_NIHLATHAK:
							nUnitId = OBJECT_NIHLATHAK_START_IN_TOWN;
							nUnitType = UNIT_OBJECT;
							nMode = OBJMODE_NEUTRAL;
							break;

						case MONSTER_ANCIENTSTATUE1:
							nUnitId = OBJECT_ANCIENTSTATUE2;
							nUnitType = UNIT_OBJECT;
							nMode = OBJMODE_NEUTRAL;
							break;

						case MONSTER_ANCIENTSTATUE2:
							nUnitId = OBJECT_ANCIENTSTATUE1;
							nUnitType = UNIT_OBJECT;
							nMode = OBJMODE_NEUTRAL;
							break;

						case MONSTER_ANCIENTSTATUE3:
							nUnitId = OBJECT_ANCIENTSTATUE3;
							nUnitType = UNIT_OBJECT;
							nMode = OBJMODE_NEUTRAL;
							break;

						default:
							break;
						}
						break;

					default:
						break;
					}
				}
				break;

			case UNIT_OBJECT:
				if (v72 > 5)
				{
					if (nUnitId >= 150)
					{
						nUnitId -= 150;
					}
					else
					{
						nUnitId = DRLGPRESET_GetObjectIndexFromObjPreset(nAct, nUnitId);
					}
				}
				else
				{
					if (nUnitId == 573)
					{
						nUnitId = -1;
					}
				}
				break;

			case UNIT_ITEM:
				if (v72 > 4)
				{
					strncpy_s(szItemCode, /*(&Source)[4 * nUnitId]*/"HDM", ARRAY_SIZE(szItemCode)); //TODO: Check comment
					szItemCode[3] = 0;

					for (int j = 0; j < 3; ++j)
					{
						szItemCode[j] = tolower(szItemCode[j]);
						++j;
					}

					DATATBLS_GetItemRecordFromItemCode(DATATBLS_StringToCode(szItemCode), &nUnitId);
				}
				nMode = IMODE_ONGROUND;
				break;
			}

			nX = *pData;
			++pData;
			nY = *pData;
			++pData;

			if (v72 > 5)
			{
				nSpawnFlag = *pData;
				++pData;
			}
			else
			{
				nSpawnFlag = 0;
			}

			if (nUnitId >= 0 && (nUnitType != UNIT_MONSTER || v72 > 4))
			{
				pNewPresetUnit = DRLGROOM_AllocPresetUnit(NULL, NULL, nUnitType, nUnitId, nMode, nX, nY);
				pNewPresetUnit->pNext = pDrlgFile->pPresetUnit;
				pDrlgFile->pPresetUnit = pNewPresetUnit;
				pNewPresetUnit->bSpawned |= nSpawnFlag;
			}
		}
	}

	if (v72 >= 12 && pDrlgFile->unk0x00 > 0 && pDrlgFile->unk0x00 <= 2)
	{
		if (v72 >= 18)
		{
			++pData;
		}

		pDrlgFile->nSubstGroups = *pData;
		++pData;
		pDrlgFile->pSubstGroups = (D2DrlgSubstGroupStrc*)FOG_AllocServerMemory(NULL, sizeof(D2DrlgSubstGroupStrc) * pDrlgFile->nSubstGroups, __FILE__, __LINE__, 0);

		for (int i = 0; i < pDrlgFile->nSubstGroups; ++i)
		{
			((D2DrlgSubstGroupStrc*)pDrlgFile->pSubstGroups)[i].field_0 = *pData;
			++pData;
			((D2DrlgSubstGroupStrc*)pDrlgFile->pSubstGroups)[i].field_4 = *pData;
			++pData;
			((D2DrlgSubstGroupStrc*)pDrlgFile->pSubstGroups)[i].field_8 = *pData;
			++pData;
			((D2DrlgSubstGroupStrc*)pDrlgFile->pSubstGroups)[i].field_C = *pData;
			++pData;

			if (v72 >= 13)
			{
				((D2DrlgSubstGroupStrc*)pDrlgFile->pSubstGroups)[i].field_14 = *pData;
				++pData;
			}
		}
	}

	if (v72 >= 14)
	{
		v59 = *pData;
		++pData;

		for (int i = 0; i < v59; ++i)
		{
			nNodes = *pData;
			++pData;
			nX = *pData;
			++pData;
			nY = *pData;
			++pData;

			if (nNodes)
			{
				while (pPresetUnit)
				{
					if (pPresetUnit->nXpos == nX && pPresetUnit->nYpos == nY)
					{
						pPresetUnit->pMapAI = (D2MapAIStrc*)FOG_AllocServerMemory(NULL, sizeof(D2MapAIStrc), __FILE__, __LINE__, 0);
						pPresetUnit->pMapAI->pPosition = (D2MapAIPathPositionStrc*)FOG_AllocServerMemory(NULL, sizeof(D2MapAIPathPositionStrc) * nNodes, __FILE__, __LINE__, 0);
						pPresetUnit->pMapAI->nPathNodes = nNodes;

						for (int j = 0; j < nNodes; ++j)
						{
							pPresetUnit->pMapAI->pPosition[j].nX = *pData;
							++pData;
							pPresetUnit->pMapAI->pPosition[j].nY = *pData;
							++pData;

							if (v72 < 15)
							{
								pPresetUnit->pMapAI->pPosition[j].nMapAIAction = 1;
							}
							else
							{
								pPresetUnit->pMapAI->pPosition[j].nMapAIAction = *pData;
								++pData;
							}
						}

						break;
					}

					pPresetUnit = pPresetUnit->pNext;
				}

				if (!pPresetUnit)
				{
					pData += 2 * nNodes;
					if (v72 >= 15)
					{
						pData += nNodes;
					}
				}
			}
		}
	}
}

//D2Common.0x6FD86050
//TODO: Rename v4, v5
void __fastcall DRLGPRESET_AllocDrlgFile(D2DrlgFileStrc** ppDrlgFile, void* pMemPool, char* szFile)
{
	D2UnkFileStrc* v4 = NULL;
	D2UnkFileStrc* v5 = NULL;

	if (gpLvlSubTypeFilesCriticalSection)
	{
		FOG_10050_EnterCriticalSection(gpLvlSubTypeFilesCriticalSection, 754);
	}

	v4 = dword_6FDEA700;
	while (v4 && strcmp(v4->szPath, szFile))
	{
		v4 = v4->pNext;
	}

	if (v4)
	{
		InterlockedIncrement(&v4->field_104);
		*ppDrlgFile = v4->pFile;
		if (gpLvlSubTypeFilesCriticalSection)
		{
			LeaveCriticalSection(gpLvlSubTypeFilesCriticalSection);
		}
	}
	else
	{
		v5 = (D2UnkFileStrc*)FOG_AllocServerMemory(NULL, sizeof(D2UnkFileStrc), __FILE__, __LINE__, 0);

		*ppDrlgFile = (D2DrlgFileStrc*)FOG_AllocServerMemory(NULL, sizeof(D2DrlgFileStrc), __FILE__, __LINE__, 0);
		memset(*ppDrlgFile, 0x00, sizeof(D2DrlgFileStrc));

		strcpy_s(v5->szPath, szFile);
		v5->field_104 = 1;
		v5->pFile = *ppDrlgFile;
		v5->pNext = dword_6FDEA700;
		dword_6FDEA700 = v5;

		DRLGPRESET_ParseDS1File(*ppDrlgFile, pMemPool, szFile);

		if (gpLvlSubTypeFilesCriticalSection)
		{
			LeaveCriticalSection(gpLvlSubTypeFilesCriticalSection);
		}
	}
}

//D2Common.0x6FD86190
void __fastcall DRLGPRESET_FreeDrlgFile(D2DrlgFileStrc** ppDrlgFile)
{
	D2PresetUnitStrc* pNextPresetUnit = NULL;
	D2PresetUnitStrc* pPresetUnit = NULL;
	D2UnkFileStrc* pPrevious = NULL;
	D2UnkFileStrc* pCurrent = NULL;

	if (*ppDrlgFile)
	{
		if (gpLvlSubTypeFilesCriticalSection)
		{
			FOG_10050_EnterCriticalSection(gpLvlSubTypeFilesCriticalSection, 754);
		}

		pPrevious = NULL;
		for (pCurrent = dword_6FDEA700; pCurrent; pCurrent = pCurrent->pNext)
		{
			if (pCurrent->pFile == *ppDrlgFile)
			{
				break;
			}
			pPrevious = pCurrent;
		}

		InterlockedDecrement(&pCurrent->field_104);

		if (pCurrent && pCurrent->field_104 <= 0)
		{
			if (pPrevious)
			{
				pPrevious->pNext = pCurrent->pNext;
			}
			else
			{
				dword_6FDEA700 = pCurrent->pNext;
			}

			if (gpLvlSubTypeFilesCriticalSection)
			{
				LeaveCriticalSection(gpLvlSubTypeFilesCriticalSection);
			}

			if ((*ppDrlgFile)->pDS1File)
			{
				FOG_FreeClientMemory((*ppDrlgFile)->pDS1File, __FILE__, __LINE__, 0);
			}

			if ((*ppDrlgFile)->pSubstGroups)
			{
				FOG_FreeServerMemory(NULL, (*ppDrlgFile)->pSubstGroups, __FILE__, __LINE__, 0);
			}

			pPresetUnit = (*ppDrlgFile)->pPresetUnit;
			while (pPresetUnit)
			{
				pNextPresetUnit = pPresetUnit->pNext;

				DRLGPRESET_FreePresetUnit(NULL, pPresetUnit);

				pPresetUnit = pNextPresetUnit;
			}

			FOG_FreeServerMemory(NULL, *ppDrlgFile, __FILE__, __LINE__, 0);
			FOG_FreeServerMemory(NULL, pCurrent, __FILE__, __LINE__, 0);
			*ppDrlgFile = NULL;
		}
		else
		{
			if (gpLvlSubTypeFilesCriticalSection)
			{
				LeaveCriticalSection(gpLvlSubTypeFilesCriticalSection);
			}
		}
	}
}

//D2Common.0x6FD86310
D2PresetUnitStrc* __fastcall DRLGPRESET_CopyPresetUnit(void* pMemPool, D2PresetUnitStrc* pPresetUnit, int nX, int nY)
{
	D2PresetUnitStrc* pNewPresetUnit = (D2PresetUnitStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2PresetUnitStrc), __FILE__, __LINE__, 0);
	memset(pNewPresetUnit, 0x00, sizeof(D2PresetUnitStrc));

	pNewPresetUnit->nUnitType = pPresetUnit->nUnitType;
	pNewPresetUnit->nIndex = pPresetUnit->nIndex;
	pNewPresetUnit->nMode = pPresetUnit->nMode;
	pNewPresetUnit->nXpos = nX + pPresetUnit->nXpos;
	pNewPresetUnit->nYpos = nY + pPresetUnit->nYpos;
	pNewPresetUnit->bSpawned = pPresetUnit->bSpawned;

	if (pPresetUnit->pMapAI)
	{
		pNewPresetUnit->pMapAI = DRLGPRESET_CreateCopyOfMapAI(pMemPool, pPresetUnit->pMapAI);

		for (int i = 0; i < pNewPresetUnit->pMapAI->nPathNodes; ++i)
		{
			pNewPresetUnit->pMapAI->pPosition[i].nX += nX;
			pNewPresetUnit->pMapAI->pPosition[i].nY += nY;
		}
	}

	return pNewPresetUnit;
}

//D2Common.0x6FD86430
void __fastcall DRLGPRESET_FreePresetUnit(void* pMemPool, D2PresetUnitStrc* pPresetUnit)
{
	if (pPresetUnit->pMapAI)
	{
		DRLGPRESET_FreeMapAI(pMemPool, pPresetUnit->pMapAI);
	}

	FOG_FreeServerMemory(pMemPool, pPresetUnit, __FILE__, __LINE__, 0);
}

//D2Common.0x6FD86480 (#10020)
D2MapAIStrc* __fastcall DRLGPRESET_CreateCopyOfMapAI(void* pMemPool, D2MapAIStrc* pMapAI)
{
	D2MapAIStrc* pNewMapAI = (D2MapAIStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2MapAIStrc), __FILE__, __LINE__, 0);

	pNewMapAI->nPathNodes = pMapAI->nPathNodes;
	pNewMapAI->pPosition = (D2MapAIPathPositionStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2MapAIPathPositionStrc) * pMapAI->nPathNodes, __FILE__, __LINE__, 0);
	memcpy(pNewMapAI->pPosition, pMapAI->pPosition, sizeof(D2MapAIPathPositionStrc) * pMapAI->nPathNodes);

	return pNewMapAI;
}

//D2Common.0x6FD864F0 (#10021)
D2MapAIStrc* __fastcall DRLGPRESET_ChangeMapAI(D2MapAIStrc** ppMapAI1, D2MapAIStrc** ppMapAI2)
{
	*ppMapAI2 = *ppMapAI1;
	*ppMapAI1 = NULL;

	return *ppMapAI2;
}

//D2Common.0x6FD86500 (#10022)
void __fastcall DRLGPRESET_FreeMapAI(void* pMemPool, D2MapAIStrc* pMapAI)
{
	FOG_FreeServerMemory(pMemPool, pMapAI->pPosition, __FILE__, __LINE__, 0);
	FOG_FreeServerMemory(pMemPool, pMapAI, __FILE__, __LINE__, 0);
}

//D2Common.0x6FD86540
void __fastcall DRLGPRESET_AddPresetUnitToDrlgMap(void* pMemPool, D2DrlgMapStrc* pDrlgMap, D2SeedStrc* pSeed)
{
	D2PresetUnitStrc* pNewPresetUnit = NULL;
	int nIndex = 0;
	int nX = 0;
	int nY = 0;

	nX = pDrlgMap->pDrlgCoord.nPosX;
	nY = pDrlgMap->pDrlgCoord.nPosY;

	DUNGEON_ExpandCoords(&nX, &nY);

	for (D2PresetUnitStrc* pPresetUnit = pDrlgMap->pFile->pPresetUnit; pPresetUnit; pPresetUnit = pPresetUnit->pNext)
	{
		if (pPresetUnit->nUnitType == UNIT_MONSTER)
		{
			nIndex = pPresetUnit->nIndex;
			if (nIndex < sgptDataTables->nMonStatsTxtRecordCount)
			{
				if (nIndex < 0 || nIndex >= sgptDataTables->nMonStatsTxtRecordCount)
				{
					nIndex = -1;
				}

				switch (nIndex)
				{
				case MONSTER_ACT2VENDOR1:
				case MONSTER_ACT2VENDOR2:
				case MONSTER_LIGHTNINGSPIRE:
				case MONSTER_FIRETOWER:
					if (((unsigned int)SEED_RollRandomNumber(pSeed) % 3))
					{
						continue;
					}
					break;

				default:
					break;
				}
			}
			else
			{
				if (pPresetUnit->nIndex - sgptDataTables->nMonStatsTxtRecordCount >= DATATBLS_GetSuperUniquesTxtRecordCount())
				{
					if (pPresetUnit->nIndex - sgptDataTables->nMonStatsTxtRecordCount - DATATBLS_GetSuperUniquesTxtRecordCount() == SUPERUNIQUE_THE_TORMENTOR)
					{
						if (!(SEED_RollRandomNumber(pSeed) & 3))
						{
							continue;
						}
					}
					else if (pPresetUnit->nIndex - sgptDataTables->nMonStatsTxtRecordCount - DATATBLS_GetSuperUniquesTxtRecordCount() == SUPERUNIQUE_TAINTBREEDER)
					{
						if (!(SEED_RollRandomNumber(pSeed) & 1))
						{
							continue;
						}
					}
					else if (pPresetUnit->nIndex - sgptDataTables->nMonStatsTxtRecordCount - DATATBLS_GetSuperUniquesTxtRecordCount() == SUPERUNIQUE_RIFTWRAITH_THE_CANNIBAL)
					{
						if (SEED_RollRandomNumber(pSeed) & 3)
						{
							continue;
						}
					}
				}
			}
		}
		else if (pPresetUnit->nUnitType == UNIT_OBJECT)
		{
			if (pPresetUnit->nIndex == OBJECT_FLOORTRAP || pPresetUnit->nIndex == OBJECT_TOMBFLOORTRAP)
			{
				if (SEED_RollRandomNumber(pSeed) & 1)
				{
					continue;
				}
			}
			else if (pPresetUnit->nIndex == 581)
			{
				if (!(SEED_RollRandomNumber(pSeed) & 3))
				{
					continue;
				}
			}
		}

		pNewPresetUnit = DRLGPRESET_CopyPresetUnit(pMemPool, pPresetUnit, nX, nY);
		pNewPresetUnit->pNext = pDrlgMap->pPresetUnit;
		pDrlgMap->pPresetUnit = pNewPresetUnit;
	}
}

//D2Common.0x6FD867A0
//TODO: Rename v22, nCellFlags
void __fastcall DRLGPRESET_SpawnHardcodedPresetUnits(D2RoomExStrc* pRoomEx)
{
	D2DrlgCoordStrc pDrlgCoord = {};
	D2DrlgGridStrc pDrlgGrid = {};
	D2PresetUnitStrc* pPresetUnit = NULL;
	D2DrlgFileStrc** ppFile = NULL;
	unsigned int v22 = 0;
	int nLevelPrestId = 0;
	int nX = 0;
	int nY = 0;
	int nHeight = 0;
	int nWidth = 0;
	int nCellFlags[256] = {};

	ppFile = &pRoomEx->pMaze->pMap->pFile;

	if (!pRoomEx->pMaze->pMap->pFile)
	{
		DRLGPRESET_AllocDrlgFile(&pRoomEx->pMaze->pMap->pFile, pRoomEx->pLevel->pDrlg->unk0x08, pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->szFile[pRoomEx->pMaze->pMap->nPickedFile]);
		DRLGPRESET_AddPresetUnitToDrlgMap(pRoomEx->pLevel->pDrlg->pMempool, pRoomEx->pMaze->pMap, &pRoomEx->pSeed);
	}

	if (pRoomEx->pMaze->pMap->bInited == 1)
	{
		pRoomEx->pMaze->pMap->bInited = 0;

		nLevelPrestId = pRoomEx->pMaze->pMap->nLevelPrest;

		if (nLevelPrestId == LVLPREST_ACT1_DOE_ENTRANCE)
		{
			if (pRoomEx->pLevel->nLevelId == LEVEL_BLOODMOOR)
			{
				nX = pRoomEx->pMaze->pMap->pDrlgCoord.nPosX + pRoomEx->pMaze->pMap->pDrlgCoord.nWidth / 2;
				nY = pRoomEx->pMaze->pMap->pDrlgCoord.nPosY + pRoomEx->pMaze->pMap->pDrlgCoord.nHeight / 2;

				DUNGEON_ExpandCoords(&nX, &nY);

				pPresetUnit = DRLGROOM_AllocPresetUnit(NULL, pRoomEx->pLevel->pDrlg->pMempool, UNIT_MONSTER, MONSTER_NAVI, MONMODE_NEUTRAL, nX, nY);
				pPresetUnit->pNext = pRoomEx->pMaze->pMap->pPresetUnit;
				pRoomEx->pMaze->pMap->pPresetUnit = pPresetUnit;
			}
		}
		else if (nLevelPrestId == LVLPREST_ACT1_TOWN_1 || nLevelPrestId == LVLPREST_ACT1_TOWN_1_TRANSITION_S || (nLevelPrestId >= LVLPREST_ACT1_RIVER_UPPER && nLevelPrestId <= LVLPREST_ACT1_BRIDGE) || nLevelPrestId == LVLPREST_ACT1_TRISTRAM)
		{
			if (pRoomEx->dwFlags & ROOMEXFLAG_AUTOMAP_REVEAL)
			{
				pDrlgCoord.nPosX = 0;
				pDrlgCoord.nPosY = 0;
				pDrlgCoord.nWidth = pRoomEx->pMaze->pMap->pDrlgCoord.nWidth;
				pDrlgCoord.nHeight = pRoomEx->pMaze->pMap->pDrlgCoord.nHeight;

				DRLGGRID_FillExternalCellFlags(&pDrlgGrid, (int*)(*ppFile)->pFloorLayer[0], &pDrlgCoord, pRoomEx->pMaze->pMap->pDrlgCoord.nWidth + 1, nCellFlags);

				pDrlgCoord.nPosX = pRoomEx->pMaze->pMap->pDrlgCoord.nPosX;
				pDrlgCoord.nPosY = pRoomEx->pMaze->pMap->pDrlgCoord.nPosY;

				if (pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->dwDef == LVLPREST_ACT1_RIVER_LOWER)
				{
					nX = pRoomEx->pMaze->pMap->pDrlgCoord.nPosX;
					nY = pRoomEx->pMaze->pMap->pDrlgCoord.nPosY;
					nHeight = pRoomEx->pMaze->pMap->pDrlgCoord.nPosY + (*ppFile)->nHeight;
					nWidth = 0;

					DUNGEON_ExpandCoords(&nX, &nY);
					DUNGEON_ExpandCoords(&nWidth, &nHeight);

					while (nY < nHeight)
					{
						pPresetUnit = DRLGROOM_AllocPresetUnit(NULL, pRoomEx->pLevel->pDrlg->pMempool, UNIT_OBJECT, OBJECT_INVISIBLE_RIVER_SOUND1, OBJMODE_NEUTRAL, nX, nY);
						pPresetUnit->bSpawned |= 1;
						pPresetUnit->pNext = pRoomEx->pMaze->pMap->pPresetUnit;
						pRoomEx->pMaze->pMap->pPresetUnit = pPresetUnit;

						nY += 40;
					}

					DRLGPRESET_AddPresetRiverObjects(pRoomEx->pMaze->pMap, pRoomEx->pLevel->pDrlg->pMempool, -1, &pDrlgGrid);
				}
				else
				{
					for (int i = 0; i < pDrlgCoord.nWidth; ++i)
					{
						v22 = DRLGGRID_GetGridFlags(&pDrlgGrid, i, 0);
						if (((v22 >> 20) & 63) == 2 && BYTE1(v22) == 24)
						{
							nX = i + pDrlgCoord.nPosX + 1;
							nY = pDrlgCoord.nPosY;
							nHeight = pDrlgCoord.nPosY + (*ppFile)->nHeight;
							nWidth = 0;

							DUNGEON_ExpandCoords(&nX, &nY);
							DUNGEON_ExpandCoords(&nWidth, &nHeight);

							while (nY < nHeight)
							{
								pPresetUnit = DRLGROOM_AllocPresetUnit(NULL, pRoomEx->pLevel->pDrlg->pMempool, UNIT_OBJECT, OBJECT_INVISIBLE_RIVER_SOUND1, OBJMODE_NEUTRAL, nX, nY);
								pPresetUnit->bSpawned |= 1;
								pPresetUnit->pNext = pRoomEx->pMaze->pMap->pPresetUnit;
								pRoomEx->pMaze->pMap->pPresetUnit = pPresetUnit;

								nY += 40;
							}

							DRLGPRESET_AddPresetRiverObjects(pRoomEx->pMaze->pMap, pRoomEx->pLevel->pDrlg->pMempool, i, &pDrlgGrid);

							i += 4;
						}
					}
				}

				DRLGGRID_ResetGrid(&pDrlgGrid);
			}
		}
	}

	pRoomEx->dwFlags |= ROOMEXFLAG_PRESET_UNITS_ADDED;
}

//D2Common.0x6FD86AC0
void __fastcall DRLGPRESET_AddPresetRiverObjects(D2DrlgMapStrc* pDrlgMap, void* pMemPool, int nX, D2DrlgGridStrc* pDrlgGrid)
{
	D2PresetUnitStrc* pPresetUnit = NULL;
	unsigned int nFlags = 0;
	int nObjectX = 0;
	int nObjectY = 0;
	int j = 0;

	if (nX < 0)
	{
		j = 0;
	}
	else
	{
		j = nX;
	}

	for (int i = 0; i < pDrlgMap->pDrlgCoord.nHeight; ++i)
	{
		nObjectX = nX + pDrlgMap->pDrlgCoord.nPosX;
		nObjectY = i + pDrlgMap->pDrlgCoord.nPosY;
		DUNGEON_ExpandCoords(&nObjectX, &nObjectY);
		nObjectX -= 5;

		pPresetUnit = DRLGROOM_AllocPresetUnit(NULL, pMemPool, UNIT_OBJECT, OBJECT_RIVER1, OBJMODE_NEUTRAL, nObjectX, nObjectY);
		pPresetUnit->bSpawned |= 1;
		pPresetUnit->pNext = pDrlgMap->pPresetUnit;
		pDrlgMap->pPresetUnit = pPresetUnit;

		pPresetUnit = DRLGROOM_AllocPresetUnit(NULL, pMemPool, UNIT_OBJECT, OBJECT_RIVER2, OBJMODE_NEUTRAL, nObjectX + 5, nObjectY);
		pPresetUnit->bSpawned |= 1;
		pPresetUnit->pNext = pDrlgMap->pPresetUnit;
		pDrlgMap->pPresetUnit = pPresetUnit;

		pPresetUnit = DRLGROOM_AllocPresetUnit(NULL, pMemPool, UNIT_OBJECT, OBJECT_RIVER2, OBJMODE_NEUTRAL, nObjectX + 10, nObjectY);
		pPresetUnit->bSpawned |= 1;
		pPresetUnit->pNext = pDrlgMap->pPresetUnit;
		pDrlgMap->pPresetUnit = pPresetUnit;

		pPresetUnit = DRLGROOM_AllocPresetUnit(NULL, pMemPool, UNIT_OBJECT, OBJECT_RIVER2, OBJMODE_NEUTRAL, nObjectX + 15, nObjectY);
		pPresetUnit->bSpawned |= 1;
		pPresetUnit->pNext = pDrlgMap->pPresetUnit;
		pDrlgMap->pPresetUnit = pPresetUnit;

		pPresetUnit = DRLGROOM_AllocPresetUnit(NULL, pMemPool, UNIT_OBJECT, OBJECT_RIVER3, OBJMODE_NEUTRAL, nObjectX + 20, nObjectY);
		pPresetUnit->bSpawned |= 1;
		pPresetUnit->pNext = pDrlgMap->pPresetUnit;
		pDrlgMap->pPresetUnit = pPresetUnit;

		nFlags = DRLGGRID_GetGridFlags(pDrlgGrid, j, i);
		if (((nFlags >> 20) & 63) == 4)
		{
			switch ((uint16_t)nFlags >> 8)
			{
			case 0:
			case 4:
			case 8:
			case 16:
			case 29:
			case 39:
				i += 3;
				break;

			default:
				break;
			}
		}
	}
}

//D2Common.0x6FD86C80
void __fastcall DRLGPRESET_FreePresetRoomData(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->pMaze)
	{
		DRLGPRESET_FreeDrlgGrids(pRoomEx->pLevel->pDrlg->pMempool, pRoomEx);

		if (pRoomEx->pMaze->pTombStoneTiles)
		{
			FOG_FreeServerMemory(pRoomEx->pLevel->pDrlg->pMempool, pRoomEx->pMaze->pTombStoneTiles, __FILE__, __LINE__, 0);
		}

		FOG_FreeServerMemory(pRoomEx->pLevel->pDrlg->pMempool, pRoomEx->pMaze, __FILE__, __LINE__, 0);
		pRoomEx->pMaze = NULL;
	}
}

//D2Common.0x6FD86CE0
void __fastcall DRLGPRESET_FreeDrlgGrids(void* pMemPool, D2RoomExStrc* pRoomEx)
{
	D2DrlgFileStrc* pDrlgFile = NULL;

	if (pRoomEx->pMaze && pRoomEx->pMaze->pMap)
	{
		pDrlgFile = pRoomEx->pMaze->pMap->pFile;
		if (pDrlgFile)
		{
			for (int i = 0; i < pDrlgFile->nWalls; ++i)
			{
				DRLGGRID_FreeGrid(pMemPool, &pRoomEx->pMaze->pOrientationGrid[i]);
				DRLGGRID_FreeGrid(pMemPool, &pRoomEx->pMaze->pWallGrid[i]);
			}

			for (int i = 0; i < pDrlgFile->nFloors; ++i)
			{
				DRLGGRID_FreeGrid(pMemPool, &pRoomEx->pMaze->pFloorGrid[i]);
			}

			DRLGGRID_FreeGrid(pMemPool, &pRoomEx->pMaze->pCellGrid);
		}
	}
}

//D2Common.0x6FD86D60
void __fastcall DRLGPRESET_FreeDrlgGridsFromPresetRoom(D2RoomExStrc* pRoomEx)
{
	if (pRoomEx->pMaze)
	{
		DRLGPRESET_FreeDrlgGrids(pRoomEx->pLevel->pDrlg->pMempool, pRoomEx);
	}
}

//D2Common.0x6FD86D80
void __fastcall DRLGPRESET_AllocPresetRoomData(D2RoomExStrc* pRoomEx)
{
	pRoomEx->pMaze = (D2DrlgPresetRoomStrc*)FOG_AllocServerMemory(pRoomEx->pLevel->pDrlg->pMempool, sizeof(D2DrlgPresetRoomStrc), __FILE__, __LINE__, 0);
	memset(pRoomEx->pMaze, 0x00, sizeof(D2DrlgPresetRoomStrc));
}

//D2Common.0x6FD86DC0
D2RoomExStrc* __fastcall DRLGPRESET_InitPresetRoomData(D2DrlgLevelStrc* pLevel, D2DrlgMapStrc* pDrlgMap, D2DrlgCoordStrc* pDrlgCoord, uint32_t dwDT1Mask, int dwRoomFlags, int dwPresetFlags, D2DrlgGridStrc* a7)
{
	D2DrlgPresetRoomStrc* pDrlgPresetRoom = NULL;
	D2RoomExStrc* pRoomEx = NULL;

	pRoomEx = DRLGROOM_AllocRoomEx(pLevel, DRLGTYPE_PRESET);

	pRoomEx->dwDT1Mask = dwDT1Mask;
	pRoomEx->dwFlags |= dwRoomFlags;

	pRoomEx->nTileXPos = pDrlgCoord->nPosX;
	pRoomEx->nTileYPos = pDrlgCoord->nPosY;
	pRoomEx->nTileWidth = pDrlgCoord->nWidth;
	pRoomEx->nTileHeight = pDrlgCoord->nHeight;

	pDrlgPresetRoom = pRoomEx->pMaze;
	pDrlgPresetRoom->pMap = pDrlgMap;
	pDrlgPresetRoom->dwFlags = dwPresetFlags;
	pDrlgPresetRoom->nLevelPrest = pDrlgMap->pLvlPrestTxtRecord->dwDef;
	pDrlgPresetRoom->pMazeGrid = a7;

	if (a7)
	{
		a7->nHeight |= 1;
	}

	if (!pDrlgPresetRoom->pMap->pLvlPrestTxtRecord->dwPopulate)
	{
		pRoomEx->dwFlags |= ROOMEXFLAG_POPULATION_ZERO;
	}

	DRLGROOM_AddRoomExToLevel(pLevel, pRoomEx);

	return pRoomEx;
}

//D2Common.0x6FD86E50
void __fastcall DRLGPRESET_InitPresetRoomGrids(D2RoomExStrc* pRoomEx)
{
	D2PresetUnitStrc* pPresetUnit = NULL;
	D2PresetUnitStrc* pPrevious = NULL;
	D2DrlgCoordStrc pDrlgCoord = {};
	int nWidth = 0;

	pDrlgCoord.nPosX = pRoomEx->nTileXPos - pRoomEx->pMaze->pMap->pDrlgCoord.nPosX;
	pDrlgCoord.nPosY = pRoomEx->nTileYPos - pRoomEx->pMaze->pMap->pDrlgCoord.nPosY;
	pDrlgCoord.nWidth = pRoomEx->nTileWidth + 1;
	pDrlgCoord.nHeight = pRoomEx->nTileHeight + 1;

	nWidth = pRoomEx->pMaze->pMap->pDrlgCoord.nWidth + 1;

	for (int i = 0; i < pRoomEx->pMaze->pMap->pFile->nWalls; ++i)
	{
		DRLGGRID_FillNewCellFlags(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pMaze->pWallGrid[i], (int*)pRoomEx->pMaze->pMap->pFile->pWallLayer[i], &pDrlgCoord, nWidth);
		DRLGGRID_FillNewCellFlags(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pMaze->pOrientationGrid[i], (int*)pRoomEx->pMaze->pMap->pFile->pOrientationLayer[i], &pDrlgCoord, nWidth);
	}

	if (pRoomEx->pMaze->pMap->pFile->nWalls)
	{
		DRLGGRID_AlterEdgeGridFlags(pRoomEx->pMaze->pWallGrid, 132, FLAG_OPERATION_OR);
	}

	for (int i = 1; i < pRoomEx->pMaze->pMap->pFile->nWalls; ++i)
	{
		DRLGGRID_AlterAllGridFlags(&pRoomEx->pMaze->pWallGrid[i], i << 18, FLAG_OPERATION_OR);
	}

	for (int i = 0; i < pRoomEx->pMaze->pMap->pFile->nFloors; ++i)
	{
		DRLGGRID_FillNewCellFlags(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pMaze->pFloorGrid[i], (int*)pRoomEx->pMaze->pMap->pFile->pFloorLayer[i], &pDrlgCoord, nWidth);
		DRLGGRID_AlterAllGridFlags(&pRoomEx->pMaze->pFloorGrid[i], i << 18, FLAG_OPERATION_OR);
	}

	DRLGGRID_FillNewCellFlags(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pMaze->pCellGrid, (int*)pRoomEx->pMaze->pMap->pFile->pShadowLayer, &pDrlgCoord, nWidth);

	for (int i = 0; i < pRoomEx->pMaze->pMap->pFile->nFloors; ++i)
	{
		DRLGGRID_AlterEdgeGridFlags(&pRoomEx->pMaze->pFloorGrid[i], 132, FLAG_OPERATION_OR);
	}

	DRLGGRID_AlterEdgeGridFlags(&pRoomEx->pMaze->pCellGrid, 132, FLAG_OPERATION_OR);

	pDrlgCoord.nPosX = pRoomEx->nTileXPos;
	pDrlgCoord.nPosY = pRoomEx->nTileYPos;
	pDrlgCoord.nWidth = pRoomEx->nTileWidth;
	pDrlgCoord.nHeight = pRoomEx->nTileHeight;

	DUNGEON_ExpandCoords(&pDrlgCoord.nPosX, &pDrlgCoord.nPosY);
	DUNGEON_ExpandCoords(&pDrlgCoord.nWidth, &pDrlgCoord.nHeight);

	pPresetUnit = pRoomEx->pMaze->pMap->pPresetUnit;
	pPrevious = NULL;

	while (pPresetUnit)
	{
		if (DRLGROOM_AreXYInsideCoordinates(&pDrlgCoord, pPresetUnit->nXpos, pPresetUnit->nYpos))
		{
			pPresetUnit->nXpos -= 5 * pRoomEx->nTileXPos;
			pPresetUnit->nYpos -= 5 * pRoomEx->nTileYPos;

			if (pPrevious)
			{
				pPrevious->pNext = pPresetUnit->pNext;
			}
			else
			{
				pRoomEx->pMaze->pMap->pPresetUnit = pPresetUnit->pNext;
			}

			pPresetUnit->pNext = pRoomEx->pPresetUnits;
			pRoomEx->pPresetUnits = pPresetUnit;

			if (pPrevious)
			{
				pPresetUnit = pPrevious->pNext;
			}
			else
			{
				pPresetUnit = pRoomEx->pMaze->pMap->pPresetUnit;
			}
		}
		else
		{
			pPrevious = pPresetUnit;
			pPresetUnit = pPresetUnit->pNext;
		}
	}
}

//D2Common.0x6FD870F0
void __fastcall DRLGPRESET_GetTombStoneTileCoords(D2RoomExStrc* pRoomEx, D2CoordStrc** ppTombStoneTiles, int* pnTombStoneTiles)
{
	if (pRoomEx->nType == DRLGTYPE_PRESET)
	{
		*ppTombStoneTiles = pRoomEx->pMaze->pTombStoneTiles;
		*pnTombStoneTiles = pRoomEx->pMaze->nTombStoneTiles;
	}
	else
	{
		*ppTombStoneTiles = NULL;
		*pnTombStoneTiles = 0;
	}
}

//D2Common.0x6FD87130
void __fastcall DRLGPRESET_AddPresetRoomMapTiles(D2RoomExStrc* pRoomEx)
{
	BOOL bTownOrGraveyard = FALSE;
	BOOL bKillEdgeX = FALSE;
	BOOL bKillEdgeY = FALSE;
	int nTombStoneTiles = 0;
	unsigned int nFlags = 0;
	D2DrlgGridStrc pDrlgGrid = {};
	D2DrlgGridStrc a3 = {};
	int v43[256] = {};
	int a4[1024] = {};
	int nCellFlags[256] = {};
	int v46[1024] = {};

	int nX = 0;
	int nY = 0;

	DRLGROOMTILE_AllocTileGrid(pRoomEx);

	if (pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->dwKillEdge)
	{
		bKillEdgeX = pRoomEx->nTileXPos + pRoomEx->nTileWidth == pRoomEx->pMaze->pMap->pDrlgCoord.nPosX + pRoomEx->pMaze->pMap->pDrlgCoord.nWidth;
		bKillEdgeY = pRoomEx->nTileYPos + pRoomEx->nTileHeight == pRoomEx->pMaze->pMap->pDrlgCoord.nPosY + pRoomEx->pMaze->pMap->pDrlgCoord.nHeight;
	}

	if (pRoomEx->pMaze->nLevelPrest == LVLPREST_ACT1_TOWN_1 || pRoomEx->pMaze->nLevelPrest == LVLPREST_ACT1_GRAVEYARD)
	{
		bTownOrGraveyard = TRUE;
		DRLGGRID_FillGrid(&a3, pRoomEx->nTileWidth + 1, pRoomEx->nTileHeight + 1, a4, v43);
		DRLGGRID_FillGrid(&pDrlgGrid, pRoomEx->nTileWidth + 1, pRoomEx->nTileHeight + 1, v46, nCellFlags);
	}

	for (int i = 0; i < pRoomEx->pMaze->pMap->pFile->nFloors; ++i)
	{
		DRLGROOMTILE_CountAllTileTypes(pRoomEx, &pRoomEx->pMaze->pFloorGrid[i], (!i && pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->dwFillBlanks), bKillEdgeX, bKillEdgeY);

		if (pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->dwAnimate)
		{
			DRLGANIM_TestLoadAnimatedRoomTiles(pRoomEx, &pRoomEx->pMaze->pFloorGrid[i], 0, 0, bKillEdgeX, bKillEdgeY);
		}
	}

	for (int i = 0; i < pRoomEx->pMaze->pMap->pFile->nWalls; ++i)
	{
		DRLGROOMTILE_CountWallWarpTiles(pRoomEx, pRoomEx->pMaze->pWallGrid, pRoomEx->pMaze->pOrientationGrid, bKillEdgeX, bKillEdgeY);
		DRLGROOMTILE_CountAllTileTypes(pRoomEx, pRoomEx->pMaze->pWallGrid, NULL, bKillEdgeX, bKillEdgeY);

		if (pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->dwAnimate)
		{
			DRLGANIM_TestLoadAnimatedRoomTiles(pRoomEx, pRoomEx->pMaze->pWallGrid, pRoomEx->pMaze->pOrientationGrid, 0, bKillEdgeX, bKillEdgeY);
		}
	}

	DRLGROOMTILE_CountAllTileTypes(pRoomEx, &pRoomEx->pMaze->pCellGrid, 0, bKillEdgeX, bKillEdgeY);

	if (pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->dwAnimate)
	{
		DRLGANIM_TestLoadAnimatedRoomTiles(pRoomEx, &pRoomEx->pMaze->pCellGrid, NULL, 13, bKillEdgeX, bKillEdgeY);
	}

	if (bTownOrGraveyard)
	{
		DRLGROOMTILE_CountAllTileTypes(pRoomEx, &pDrlgGrid, 0, bKillEdgeX, bKillEdgeY);
	}

	DRLGROOMTILE_AllocTileData(pRoomEx);

	for (int i = 0; i < pRoomEx->pMaze->pMap->pFile->nFloors; ++i)
	{
		DRLGROOMTILE_LoadInitRoomTiles(pRoomEx, &pRoomEx->pMaze->pFloorGrid[i], 0, (!i && pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->dwFillBlanks), bKillEdgeX, bKillEdgeY);
	}

	for (int i = 0; i < pRoomEx->pMaze->pMap->pFile->nWalls; ++i)
	{
		DRLGROOMTILE_LoadInitRoomTiles(pRoomEx, pRoomEx->pMaze->pWallGrid, pRoomEx->pMaze->pOrientationGrid, 0, bKillEdgeX, bKillEdgeY);
	}

	DRLGROOMTILE_LoadInitRoomTiles(pRoomEx, &pRoomEx->pMaze->pCellGrid, 0, 0, bKillEdgeX, bKillEdgeY);

	if (bTownOrGraveyard)
	{
		DRLGROOMTILE_LoadInitRoomTiles(pRoomEx, &pDrlgGrid, &a3, 0, bKillEdgeX, bKillEdgeY);
	}

	if (pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->dwAnimate)
	{
		DRLGANIM_AllocAnimationTileGrids(pRoomEx, pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->nAnimSpeed, pRoomEx->pMaze->pWallGrid, pRoomEx->pMaze->pMap->pFile->nWalls, pRoomEx->pMaze->pFloorGrid, pRoomEx->pMaze->pMap->pFile->nFloors, &pRoomEx->pMaze->pCellGrid);
	}

	pRoomEx->pTileGrid->pTiles.nWalls = pRoomEx->pTileGrid->nWalls;
	pRoomEx->pTileGrid->pTiles.nFloors = pRoomEx->pTileGrid->nFloors;
	pRoomEx->pTileGrid->pTiles.nRoofs = pRoomEx->pTileGrid->nShadows;

	if (pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->dwLogicals)
	{
		DRLGLOGIC_InitializeDrlgCoordList(pRoomEx, pRoomEx->pMaze->pOrientationGrid, pRoomEx->pMaze->pFloorGrid, pRoomEx->pMaze->pWallGrid);
	}
	else
	{
		DRLGLOGIC_AllocCoordLists(pRoomEx);
	}

	if (bTownOrGraveyard)
	{
		DRLGGRID_ResetGrid(&a3);
		DRLGGRID_ResetGrid(&pDrlgGrid);
	}

	if (pRoomEx->pLevel->nLevelId == LEVEL_BURIALGROUNDS && !pRoomEx->pMaze->pTombStoneTiles)
	{
		nTombStoneTiles = 0;

		pRoomEx->pMaze->pTombStoneTiles = (D2CoordStrc*)FOG_AllocServerMemory(pRoomEx->pLevel->pDrlg->pMempool, 6 * sizeof(D2CoordStrc), __FILE__, __LINE__, 0);

		for (int i = 0; i < pRoomEx->nTileHeight; ++i)
		{
			for (int j = 0; j < pRoomEx->nTileWidth; ++j)
			{
				nFlags = DRLGGRID_GetGridFlags(pRoomEx->pMaze->pWallGrid, j, i);
				if (((nFlags >> 20) & 0x3F) == 10 && BYTE1(nFlags) >= 23u && BYTE1(nFlags) <= 27u)
				{
					if (nTombStoneTiles < 6)
					{
						nX = j + pRoomEx->nTileXPos;
						nY = i + pRoomEx->nTileYPos;

						DUNGEON_ExpandCoords(&nX, &nY);

						pRoomEx->pMaze->pTombStoneTiles[nTombStoneTiles].nX = nX + 2;
						pRoomEx->pMaze->pTombStoneTiles[nTombStoneTiles].nY = nY + 2;

						++nTombStoneTiles;
					}
				}
			}
		}

		pRoomEx->pMaze->nTombStoneTiles = nTombStoneTiles;
	}
}

//D2Common.0x6FD87560
//TODO: v17
D2RoomExStrc* __fastcall DRLGPRESET_BuildArea(D2DrlgLevelStrc* pLevel, D2DrlgMapStrc* pDrlgMap, int nFlags, BOOL bSingleRoom)
{
	D2DrlgCoordStrc pDrlgCoord = {};
	D2DrlgGridStrc pDrlgGrid = {};
	int nCellPos[1024] = {};
	int nCellFlags[256] = {};
	D2DrlgGridStrc* v17 = NULL;
	D2RoomExStrc* pRoomEx = NULL;
	int nHeight = 0;
	int nWidth = 0;
	int nXEnd = 0;
	int nYEnd = 0;
	int nX = 0;
	int nY = 0;
	int i = 0;
	int j = 0;

	if (pDrlgMap->pLvlPrestTxtRecord->dwOutdoors)
	{
		nFlags |= 0x80000;
	}

	DRLGGRID_FillGrid(&pDrlgGrid, pDrlgMap->pDrlgCoord.nWidth / 8 + 1, pDrlgMap->pDrlgCoord.nHeight / 8 + 1, nCellPos, nCellFlags);
	DRLGPRESET_BuildPresetArea(pLevel, &pDrlgGrid, nFlags, pDrlgMap, bSingleRoom);

	pRoomEx = 0;

	if (bSingleRoom)
	{
		pRoomEx = DRLGPRESET_InitPresetRoomData(pLevel, pDrlgMap, &pDrlgMap->pDrlgCoord, pDrlgMap->pLvlPrestTxtRecord->dwDt1Mask, DRLGGRID_GetGridFlags(&pDrlgGrid, 0, 0), 1, 0);
	}
	else
	{
		nX = pDrlgMap->pDrlgCoord.nPosX;
		nY = pDrlgMap->pDrlgCoord.nPosY;
		nXEnd = pDrlgMap->pDrlgCoord.nPosX + pDrlgMap->pDrlgCoord.nWidth;
		nYEnd = pDrlgMap->pDrlgCoord.nPosY + pDrlgMap->pDrlgCoord.nHeight;

		pDrlgCoord.nPosX = 0;
		pDrlgCoord.nPosY = 0;
		pDrlgCoord.nWidth = 0;
		pDrlgCoord.nHeight = 0;

		i = 0;
		if (nY < nYEnd)
		{
			nHeight = nYEnd - nY;
			while (nY < nYEnd)
			{
				j = 0;
				if (nX < nXEnd)
				{
					pDrlgCoord.nPosY = nY;
					nWidth = nXEnd - nX;
					while (nX < nXEnd)
					{
						pDrlgCoord.nPosX = nX;

						pDrlgCoord.nWidth = nWidth;
						if (nWidth >= 8)
						{
							pDrlgCoord.nWidth = 8;
						}

						pDrlgCoord.nHeight = nHeight;
						if (nHeight >= 8)
						{
							pDrlgCoord.nHeight = 8;
						}

						if (pDrlgCoord.nWidth && pDrlgCoord.nHeight)
						{
							v17 = (D2DrlgGridStrc*)(pDrlgMap->bHasInfo ? DRLGGRID_GetGridFlags(&pDrlgMap->pMapGrid, j, i) : NULL);
							pRoomEx = DRLGPRESET_InitPresetRoomData(pLevel, pDrlgMap, &pDrlgCoord, pDrlgMap->pLvlPrestTxtRecord->dwDt1Mask, DRLGGRID_GetGridFlags(&pDrlgGrid, j, i), 0, v17);
						}

						nX += 8;
						nWidth -= 8;

						++j;
					}
				}

				nY += 8;
				nHeight -= 8;

				++i;
			}
		}
	}
	
	DRLGGRID_ResetGrid(&pDrlgGrid);

	return pRoomEx;
}

//D2Common.0x6FD87760
//TODO: variable names
void __fastcall DRLGPRESET_BuildPresetArea(D2DrlgLevelStrc* pLevel, D2DrlgGridStrc* pDrlgGrid, int nFlags, D2DrlgMapStrc* pDrlgMap, BOOL bSingleRoom)
{
	D2DrlgCoordStrc* pPopsLocation = NULL;
	int* pVisArray = NULL;
	uint32_t v10 = 0;
	__int64 v48 = 0;
	int nCounter = 0;
	int nHeight = 0;
	int nWidth = 0;
	int nPops = 0;
	int nFlag = 0;
	int nX = 0;
	int nY = 0;
	int v28 = 0;
	int v30 = 0;
	int v31 = 0;
	int v32 = 0;
	int v53 = 0;
	D2DrlgCoordStrc pDrlgCoord = {};
	D2DrlgGridStrc pDrlgGrid1 = {};
	D2DrlgGridStrc pDrlgGrid2 = {};
	int pCellFlags1[256] = {};
	int pCellFlags2[256] = {};

	pVisArray = DRLGROOM_GetVisArrayFromLevelId(pLevel->pDrlg, pLevel->nLevelId);
	nFlag = ROOMEXFLAG_HAS_WARP_0;
	for (int i = 0; i < 8; ++i)
	{
		if (pVisArray[i] && DRLGWARP_GetWarpDestinationFromArray(pLevel, i) == -1)
		{
			nFlags |= nFlag;
		}

		nFlag *= 2;
	}

	DRLGGRID_AlterAllGridFlags(pDrlgGrid, nFlags, FLAG_OPERATION_OR);

	nPops = 0;
	v10 = pDrlgMap->pLvlPrestTxtRecord->dwPops;

	if (pDrlgMap->pLvlPrestTxtRecord->dwScan || v10)
	{
		DRLGPRESET_AllocDrlgFile(&pDrlgMap->pFile, pLevel->pDrlg->unk0x08, pDrlgMap->pLvlPrestTxtRecord->szFile[pDrlgMap->nPickedFile]);

		DRLGPRESET_AddPresetUnitToDrlgMap(pLevel->pDrlg->pMempool, pDrlgMap, &pLevel->pSeed);

		if (pDrlgMap->pFile->nWidth != pDrlgMap->pDrlgCoord.nWidth)
		{
			FOG_10025("ptRegion->ptFile->nSizeX == ptRegion->tCoords.nSizeTileX", __FILE__, __LINE__);
		}

		if (pDrlgMap->pFile->nHeight != pDrlgMap->pDrlgCoord.nHeight)
		{
			FOG_10025("ptRegion->ptFile->nSizeY == ptRegion->tCoords.nSizeTileY", __FILE__, __LINE__);
		}

		if (v10)
		{
			pDrlgMap->pPopsIndex = (int*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, sizeof(int) * v10, __FILE__, __LINE__, 0);
			pDrlgMap->pPopsSubIndex = (int*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, sizeof(int) * v10, __FILE__, __LINE__, 0);
			pDrlgMap->pPopsOrientation = (int*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, sizeof(int) * v10, __FILE__, __LINE__, 0);
			pDrlgMap->pPopsLocation = (D2DrlgCoordStrc*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, sizeof(D2DrlgCoordStrc) * v10, __FILE__, __LINE__, 0);
		}

		memset(pDrlgMap->pPopsIndex, 0x00, sizeof(int) * v10);
		memset(pDrlgMap->pPopsSubIndex, 0x00, sizeof(int) * v10);
		memset(pDrlgMap->pPopsOrientation, 0x00, sizeof(int) * v10);
		memset(pDrlgMap->pPopsLocation, 0x00, sizeof(D2DrlgCoordStrc) * v10);

		memset(&pDrlgGrid1, 0x00, sizeof(D2DrlgGridStrc));
		memset(&pDrlgGrid2, 0x00, sizeof(D2DrlgGridStrc));
		memset(&pDrlgCoord, 0x00, sizeof(D2DrlgCoordStrc));

		pDrlgCoord.nWidth = pDrlgMap->pFile->nWidth + 1;
		pDrlgCoord.nHeight = pDrlgMap->pFile->nHeight + 1;

		for (int i = 0; i < pDrlgMap->pFile->nWalls; ++i)
		{
			DRLGGRID_FillExternalCellFlags(&pDrlgGrid1, (int*)pDrlgMap->pFile->pOrientationLayer[i], &pDrlgCoord, pDrlgMap->pFile->nWidth + 1, pCellFlags1);
			DRLGGRID_FillExternalCellFlags(&pDrlgGrid2, (int*)pDrlgMap->pFile->pWallLayer[i], &pDrlgCoord, pDrlgMap->pFile->nWidth + 1, pCellFlags2);

			for (int j = 0; j < pDrlgMap->pFile->nHeight; ++j)
			{
				for (int k = 0; k < pDrlgMap->pFile->nWidth; ++k)
				{
					v53 = DRLGGRID_GetGridFlags(&pDrlgGrid1, k, j);
					v28 = DRLGGRID_GetGridFlags(&pDrlgGrid2, k, j);
					v30 = BYTE1(v28);
					v31 = v28 & 0x80000000;
					v32 = ((unsigned int)v28 >> 20) & 0x3F;

					if (v53 == 11 || v53 == 10)
					{
						if (pDrlgMap->pLvlPrestTxtRecord->dwScan && v32 >= 0 && v32 <= 7 && (v30 == 0 || v30 == 4 || v31))
						{
							if (bSingleRoom)
							{
								DRLGGRID_AlterGridFlag(pDrlgGrid, 0, 0, 1 << (v32 + 4), FLAG_OPERATION_OR);
							}
							else
							{
								DRLGGRID_AlterGridFlag(pDrlgGrid, k / 8, j / 8, 1 << (v32 + 4), FLAG_OPERATION_OR);
							}
						}

						if (v10 && v32 >= 8 && v32 <= 29)
						{
							nCounter = 0;
							while (nCounter < nPops)
							{
								if (pDrlgMap->pPopsIndex[nCounter] == v32)
								{
									pDrlgMap->pPopsLocation[nCounter].nWidth = k;
									pDrlgMap->pPopsLocation[nCounter].nHeight = j;
									break;
								}

								++nCounter;
							}

							if (nCounter == nPops)
							{
								pDrlgMap->pPopsIndex[nPops] = v32;
								pDrlgMap->pPopsSubIndex[nPops] = v30;
								pDrlgMap->pPopsLocation[nPops].nPosX = k;
								pDrlgMap->pPopsLocation[nPops].nPosY = j;
								++nPops;
							}
						}

						if (pDrlgMap->pLvlPrestTxtRecord->dwScan && v32 >= 30 && v32 <= 33)
						{
							pLevel->pTileInfo[pLevel->nTileInfo].nPosX = k + pDrlgMap->pDrlgCoord.nPosX;
							pLevel->pTileInfo[pLevel->nTileInfo].nPosY = j + pDrlgMap->pDrlgCoord.nPosY;

							switch (v32)
							{
							case 30:
								pLevel->pTileInfo[pLevel->nTileInfo].nTileIndex = v30;
								break;

							case 31:
								pLevel->pTileInfo[pLevel->nTileInfo].nTileIndex = v30 + 5;
								break;

							case 32:
								pLevel->pTileInfo[pLevel->nTileInfo].nTileIndex = 10;
								break;

							case 33:
								pLevel->pTileInfo[pLevel->nTileInfo].nTileIndex = 11;
								break;

							case 34:
								pLevel->pTileInfo[pLevel->nTileInfo].nTileIndex = 12;
								break;

							default:
								break;
							}

							++pLevel->nTileInfo;
						}
					}
				}
			}

			DRLGGRID_ResetGrid(&pDrlgGrid2);
			DRLGGRID_ResetGrid(&pDrlgGrid1);
		}

		if (v10)
		{
			pDrlgMap->nPops = nPops;
			for (int i = 0; i < nPops; ++i)
			{
				pPopsLocation = &pDrlgMap->pPopsLocation[i];

				nWidth = pPopsLocation->nPosX;
				nX = pPopsLocation->nPosX;
				if (pPopsLocation->nPosX >= pPopsLocation->nWidth)
				{
					nX = pPopsLocation->nWidth;
				}

				nHeight = pPopsLocation->nPosY;
				nY = pPopsLocation->nPosY;
				if (nHeight >= pPopsLocation->nHeight)
				{
					nY = pPopsLocation->nHeight;
				}

				if (nWidth <= pPopsLocation->nWidth)
				{
					nWidth = pPopsLocation->nWidth;
				}

				if (nHeight <= pPopsLocation->nHeight)
				{
					nHeight = pPopsLocation->nHeight;
				}

				pPopsLocation->nPosX = nX + pDrlgMap->pDrlgCoord.nPosX;
				pPopsLocation->nPosY = nY + pDrlgMap->pDrlgCoord.nPosY;
				pPopsLocation->nWidth = nWidth - nX + 1;
				pPopsLocation->nHeight = nHeight - nY + 1;

				v48 = pDrlgMap->pPopsIndex[i];
				pDrlgMap->pPopsIndex[i] = (((BYTE4(v48) & 3) + (signed int)v48) >> 2) - 1;
			}
		}

		if (pDrlgMap->pLvlPrestTxtRecord->dwScan)
		{
			for (D2PresetUnitStrc* i = pDrlgMap->pFile->pPresetUnit; i; i = i->pNext)
			{
				if (i->nUnitType == UNIT_OBJECT && i->nIndex < 573 && DATATBLS_GetObjectsTxtRecord(i->nIndex)->nSubClass & OBJSUBCLASS_WAYPOINT)
				{
					if (bSingleRoom)
					{
						DRLGGRID_AlterGridFlag(pDrlgGrid, 0, 0, 0x30000, FLAG_OPERATION_OR);
					}
					else
					{
						DRLGGRID_AlterGridFlag(pDrlgGrid, i->nXpos / 5 / 8, i->nYpos / 5 / 8, 0x30000, FLAG_OPERATION_OR);
					}
				}
			}
		}
	}
}

//D2Common.0x6FD87E10
void __fastcall DRLGPRESET_SetPickedFileInDrlgMap(D2DrlgMapStrc* pDrlgMap, int nPickedFile)
{
	pDrlgMap->nPickedFile = nPickedFile;
}

//D2Common.0x6FD87E20
D2DrlgMapStrc* __fastcall DRLGPRESET_AllocDrlgMap(D2DrlgLevelStrc* pLevel, int nLvlPrestId, D2DrlgCoordStrc* pDrlgCoord, D2SeedStrc* pSeed)
{
	D2DrlgMapStrc* pDrlgMap = (D2DrlgMapStrc*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, sizeof(D2DrlgMapStrc), __FILE__, __LINE__, 0);
	memset(pDrlgMap, 0x00, sizeof(D2DrlgMapStrc));

	pDrlgMap->pLvlPrestTxtRecord = DATATBLS_GetLvlPrestTxtRecord(nLvlPrestId);
	pDrlgMap->nLevelPrest = nLvlPrestId;

	pDrlgMap->nPickedFile = SEED_RollLimitedRandomNumber(pSeed, pDrlgMap->pLvlPrestTxtRecord->dwFiles);
	pDrlgMap->pDrlgCoord.nPosX = pDrlgCoord->nPosX;
	pDrlgMap->pDrlgCoord.nPosY = pDrlgCoord->nPosY;

	if (pDrlgMap->pLvlPrestTxtRecord->nSizeX && pDrlgMap->pLvlPrestTxtRecord->nSizeY)
	{
		pDrlgMap->pDrlgCoord.nWidth = pDrlgMap->pLvlPrestTxtRecord->nSizeX;
		pDrlgMap->pDrlgCoord.nHeight = pDrlgMap->pLvlPrestTxtRecord->nSizeY;
	}
	else
	{
		pDrlgMap->pDrlgCoord.nWidth = pDrlgCoord->nWidth;
		pDrlgMap->pDrlgCoord.nHeight = pDrlgCoord->nHeight;
	}

	pDrlgMap->bInited = 1;
	pDrlgMap->pNext = pLevel->pCurrentMap;
	pLevel->pCurrentMap = pDrlgMap;

	return pDrlgMap;
}

//D2Common.0x6FD87F00
int __fastcall DRLGPRESET_GetSizeX(int nLvlPrestId)
{
	return DATATBLS_GetLvlPrestTxtRecord(nLvlPrestId)->nSizeX;
}

//D2Common.0x6FD87F10
int __fastcall DRLGPRESET_GetSizeY(int nLvlPrestId)
{
	return DATATBLS_GetLvlPrestTxtRecord(nLvlPrestId)->nSizeY;
}

//D2Common.0x6FD87F20
void __fastcall DRLGPRESET_FreeDrlgMap(void* pMemPool, D2DrlgMapStrc* pDrlgMap)
{
	D2PresetUnitStrc* pNextPresetUnit = NULL;
	D2PresetUnitStrc* pPresetUnit = NULL;
	D2UnkFileStrc* pPrevious = NULL;
	D2UnkFileStrc* pCurrent = NULL;
	D2DrlgMapStrc* pNextMap = NULL;
	D2DrlgMapStrc* pMap = NULL;

	pMap = pDrlgMap;

	while (pMap)
	{
		pNextMap = pMap->pNext;

		DRLGGRID_FreeGrid(pMemPool, &pMap->pMapGrid);
		DRLGVER_FreeVertices(pMemPool, (D2DrlgVertexStrc**)&pMap->bHasInfo);

		DRLGPRESET_FreeDrlgFile(&pMap->pFile);

		pPresetUnit = pMap->pPresetUnit;
		while (pPresetUnit)
		{
			pNextPresetUnit = pPresetUnit->pNext;

			DRLGPRESET_FreePresetUnit(NULL, pPresetUnit);

			pPresetUnit = pNextPresetUnit;
		}

		if (pMap->pPopsIndex)
		{
			FOG_FreeServerMemory(pMemPool, pMap->pPopsIndex, __FILE__, __LINE__, 0);
		}

		if (pMap->pPopsSubIndex)
		{
			FOG_FreeServerMemory(pMemPool, pMap->pPopsSubIndex, __FILE__, __LINE__, 0);
		}

		if (pMap->pPopsOrientation)
		{
			FOG_FreeServerMemory(pMemPool, pMap->pPopsOrientation, __FILE__, __LINE__, 0);
		}

		if (pMap->pPopsLocation)
		{
			FOG_FreeServerMemory(pMemPool, pMap->pPopsLocation, __FILE__, __LINE__, 0);
		}

		FOG_FreeServerMemory(pMemPool, pMap, __FILE__, __LINE__, 0);

		pMap = pNextMap;
	}
}

//D2Common.0x6FD881A0 (#10008)
int __fastcall DRLGPRESET_GetLevelPrestIdFromRoomEx(D2RoomExStrc* pRoomEx)
{
	return pRoomEx->pMaze->nLevelPrest;
}

//D2Common.0x6FD881B0 (#10009)
char* __fastcall DRLGPRESET_GetPickedLevelPrestFilePathFromRoomEx(D2RoomExStrc* pRoomEx)
{
	return pRoomEx->pMaze->pMap->pLvlPrestTxtRecord->szFile[pRoomEx->pMaze->pMap->nPickedFile];
}

//D2Common.0x6FD881D0
void __fastcall DRLGPRESET_UpdatePops(D2RoomExStrc* pRoomEx, int nX, int nY, BOOL bOtherRoom)
{
	const int nTick = GetTickCount() + 500;

	D2DrlgMapStrc* pDrlgMap = NULL;
	int nPopIndex = 0;
	if (pRoomEx->nType == DRLGTYPE_PRESET)
	{
		pDrlgMap = pRoomEx->pMaze->pMap;
		for (int i = 0; i < pDrlgMap->nPops; ++i)
		{
			D2DrlgCoordStrc drlgCoord = {};
			drlgCoord.nPosX = 5 * pDrlgMap->pPopsLocation[i].nPosX;
			drlgCoord.nPosY = 5 * pDrlgMap->pPopsLocation[i].nPosY;

			drlgCoord.nWidth = pDrlgMap->pLvlPrestTxtRecord->dwPopPad + 5 * pDrlgMap->pPopsLocation[i].nWidth;
			drlgCoord.nHeight = pDrlgMap->pLvlPrestTxtRecord->dwPopPad + 5 * pDrlgMap->pPopsLocation[i].nHeight;

			if (DRLGROOM_AreXYInsideCoordinates(&drlgCoord, nX, nY))
			{
				nPopIndex = pDrlgMap->pPopsIndex[i];
				break;
			}
		}
	}

	BOOL bUpdate = FALSE;
	for (int i = 0; i < pRoomEx->nRoomsNear; ++i)
	{
		D2RoomExStrc* pAdjacentRoom = pRoomEx->ppRoomsNear[i];
		if (pAdjacentRoom->nType == DRLGTYPE_PRESET && pAdjacentRoom->pRoom)
		{
			const D2DrlgMapStrc* pAdjacentRoomMap = pAdjacentRoom->pMaze->pMap;
			for (int nCurrentPops = 0; nCurrentPops < pAdjacentRoomMap->nPops; ++nCurrentPops)
			{
				const int nOrientation = pAdjacentRoomMap->pPopsOrientation[nCurrentPops];

				if (pAdjacentRoomMap == pDrlgMap && pAdjacentRoomMap->pPopsIndex[nCurrentPops] == nPopIndex)
				{
					if (nOrientation == 0 || bOtherRoom)
					{
						const int nTickToUse = nOrientation == 0 ? nTick : nOrientation;

						DRLGPRESET_TogglePopsVisibility(pAdjacentRoom, pAdjacentRoomMap->pPopsSubIndex[nCurrentPops], &pAdjacentRoomMap->pPopsLocation[nCurrentPops], nTickToUse, 0);
						bUpdate = TRUE;
					}
				}
				else if (nOrientation != 0)
				{
					DRLGPRESET_TogglePopsVisibility(pAdjacentRoom, pAdjacentRoomMap->pPopsSubIndex[nCurrentPops], &pAdjacentRoomMap->pPopsLocation[nCurrentPops], nTick, 1);
					bUpdate = TRUE;
				}
			}
		}
	}

	if (bUpdate)
	{
		for (int nAdjacentRoomIdx = 0; nAdjacentRoomIdx < pRoomEx->nRoomsNear; ++nAdjacentRoomIdx)
		{
			const D2RoomExStrc* pAdjacentRoom = pRoomEx->ppRoomsNear[nAdjacentRoomIdx];
			if (pAdjacentRoom->nType == DRLGTYPE_PRESET)
			{
				const D2DrlgMapStrc* pAdjacentRoomMap = pAdjacentRoom->pMaze->pMap;
				for (int nAdjacentRoomIdx = 0; nAdjacentRoomIdx < pAdjacentRoomMap->nPops; ++nAdjacentRoomIdx)
				{
					pAdjacentRoomMap->pPopsOrientation[nAdjacentRoomIdx] = 0;
					if (pAdjacentRoomMap == pDrlgMap && pAdjacentRoomMap->pPopsIndex[nAdjacentRoomIdx] == nPopIndex)
					{
						pAdjacentRoomMap->pPopsOrientation[nAdjacentRoomIdx] = nTick;
					}
				}
			}
		}
	}
}

//D2Common.0x6FD88450
void __fastcall DRLGPRESET_TogglePopsVisibility(D2RoomExStrc* pRoomEx, int nPopSubIndex, D2DrlgCoordStrc* pDrlgCoord, int nTick, BOOL nCellFlags)
{
	D2DrlgTileDataStrc* pDrlgTileData = NULL;
	D2DrlgCoordStrc pDrlgCoords = {};

	D2_ASSERT(pRoomEx->pTileGrid);

	pDrlgCoords.nPosX = pDrlgCoord->nPosX - 1;
	pDrlgCoords.nPosY = pDrlgCoord->nPosY - 1;
	pDrlgCoords.nWidth = pDrlgCoord->nWidth + 2;
	pDrlgCoords.nHeight = pDrlgCoord->nHeight + 2;

	for (int i = 0; i < pRoomEx->pTileGrid->pTiles.nWalls; ++i)
	{
		pDrlgTileData = &pRoomEx->pTileGrid->pTiles.pWallTiles[i];

		if (pDrlgTileData->dwFlags & 0x200 && DRLGROOM_AreXYInsideCoordinates(&pDrlgCoords, pRoomEx->nTileXPos + pDrlgTileData->nPosX, pRoomEx->nTileYPos + pDrlgTileData->nPosY))
		{
			if (nCellFlags)
			{
				pDrlgTileData->unk0x24 |= 4;
				pDrlgTileData->unk0x2C = nTick;
			}
			else
			{
				pDrlgTileData->unk0x24 &= 0xFFFFFFFB;
				pDrlgTileData->dwFlags &= 0xFFFFFFF7;
			}
		}
		else
		{
			if (!(pDrlgTileData->dwFlags & 0x100) && D2CMP_10078_GetTileIndex(pDrlgTileData->pTile) == nPopSubIndex && DRLGROOM_AreXYInsideCoordinates(&pDrlgCoords, pRoomEx->nTileXPos + pDrlgTileData->nPosX, pRoomEx->nTileYPos + pDrlgTileData->nPosY))
			{
				pDrlgTileData->unk0x24 |= 2;
				if (nCellFlags)
				{
					pDrlgTileData->nGreen = 0;
					pDrlgTileData->nBlue = 255;

					if (pDrlgTileData->nRed)
					{
						pDrlgTileData->unk0x2C = nTick - 500 * pDrlgTileData->nRed / 255;
					}
					else
					{
						pDrlgTileData->unk0x2C = nTick;
					}
				}
				else
				{
					pDrlgTileData->nGreen = 255;
					pDrlgTileData->nBlue = 0;

					if (pDrlgTileData->nRed == 255)
					{
						pDrlgTileData->unk0x2C = nTick;
					}
					else
					{
						pDrlgTileData->unk0x2C = nTick + 500 * (pDrlgTileData->nRed - 255) / 255;
					}
				}
			}
		}
	}
}

//D2Common.0x6FD88610
void __fastcall DRLGPRESET_InitLevelData(D2DrlgLevelStrc* pLevel)
{
	D2LvlPrestTxt* pLvlPrestTxtRecord = NULL;

	pLvlPrestTxtRecord = DATATBLS_GetLvlPrestTxtRecordFromLevelId(pLevel->nLevelId);
	D2_ASSERTM(pLvlPrestTxtRecord, "Level labeled as Preset, but no preset claims the level");

	pLevel->pPreset = (D2DrlgPresetInfoStrc*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, sizeof(D2DrlgPresetInfoStrc), __FILE__, __LINE__, 0);

	pLevel->pPreset->pDrlgMap = NULL;
	pLevel->pPreset->nDirection = 0;

	if (pLvlPrestTxtRecord->dwFiles)
	{
		pLevel->pPreset->nDirection = SEED_RollLimitedRandomNumber(&pLevel->pSeed, pLvlPrestTxtRecord->dwFiles);
	}
	else
	{
		pLevel->pPreset->nDirection = -1;
	}

	DRLG_SetLevelPositionAndSize(pLevel->pDrlg, pLevel);
}

//D2Common.0x6FD886F0
void __fastcall DRLGPRESET_GenerateLevel(D2DrlgLevelStrc* pLevel)
{
	D2LvlPrestTxt* pLvlPrestTxtRecord = NULL;
	D2DrlgLevelStrc* pCurrentLevel = NULL;
	int* pVisLevelIds = NULL;

	pLvlPrestTxtRecord = DATATBLS_GetLvlPrestTxtRecordFromLevelId(pLevel->nLevelId);
	pLevel->pPreset->pDrlgMap = DRLGPRESET_AllocDrlgMap(pLevel, pLvlPrestTxtRecord->dwDef, &pLevel->pLevelCoords, &pLevel->pSeed);

	if (pLevel->pPreset->nDirection == -1)
	{
		pLevel->pPreset->nDirection = pLevel->pPreset->pDrlgMap->nPickedFile;
	}
	else
	{
		pLevel->pPreset->pDrlgMap->nPickedFile = pLevel->pPreset->nDirection;
	}

	DRLGPRESET_BuildArea(pLevel, pLevel->pPreset->pDrlgMap, 0, 0);

	if (pLevel->nLevelId == LEVEL_LUTGHOLEIN || pLevel->nLevelId == LEVEL_THEPANDEMONIUMFORTRESS || pLevel->nLevelId == LEVEL_HARROGATH)
	{
		if (pLevel->pDrlg->pfTownAutomap && pLevel->pPreset->pDrlgMap->pLvlPrestTxtRecord->dwAutoMap)
		{
			pLevel->pDrlg->pfTownAutomap(pLevel->nLevelId, pLevel->pPreset->pDrlgMap->nPickedFile, pLevel->nPosX + pLevel->nWidth / 2, pLevel->nPosY + pLevel->nHeight / 2);
		}
	}
	else
	{
		if (pLevel->pDrlg->pfAutomap && pLevel->pPreset->pDrlgMap->pLvlPrestTxtRecord->dwAutoMap)
		{
			pVisLevelIds = DRLGROOM_GetVisArrayFromLevelId(pLevel->pDrlg, pLevel->nLevelId);
			for (int i = 0; i < 8; ++i)
			{
				if (pVisLevelIds[i])
				{
					pCurrentLevel = DRLG_GetLevel(pLevel->pDrlg, pVisLevelIds[i]);
					if (!pCurrentLevel->pFirstRoomEx)
					{
						DRLG_InitLevel(pCurrentLevel);
					}
				}
			}

			for (D2RoomExStrc* i = pLevel->pFirstRoomEx; i; i = i->pRoomExNext)
			{
				DRLGACTIVATE_InitializeRoomEx(i);
				pLevel->pDrlg->pfAutomap(i->pRoom);
			}
		}
	}
}

//D2Common.0x6FD88810
void __fastcall DRLGPRESET_ResetDrlgMap(D2DrlgLevelStrc* pLevel, BOOL bKeepPreset)
{
	pLevel->pPreset->pDrlgMap = NULL;

	if (!bKeepPreset)
	{
		FOG_FreeServerMemory(pLevel->pDrlg->pMempool, pLevel->pPreset, __FILE__, __LINE__, 0);
		pLevel->pPreset = NULL;
	}
}

//D2Common.0x6FD88850
int __fastcall DRLGPRESET_MapOrientationLayer(int nId)
{
	static const int nOrientationLayerMappingTable[] =
	{
		0, 1, 2, 1, 2, 3, 3, 5, 5, 6, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
		17, 18, 20, 0, 0, 1, 2, 5, 5, 7, 9, 11, 13, 14, 15, 16, 17, 18, 19, 20,
	};

	return nOrientationLayerMappingTable[nId];
}
