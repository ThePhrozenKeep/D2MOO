#include <cstdio>

#include <Archive.h>
#include <File.h>
#include <D2BitManip.h>
#include <D2Lang.h>

#include "D2DataTbls.h"
#include "D2States.h"
#include "Units/Units.h"

// D2Common.0x6FDE9600
D2ArenaTxt* gpArenaTxtTable;
// D2Common.0x6FDE95F8
D2CharTemplateTxt* gpCharTemplateTxtTable;
int gnCharTemplateTxtTableRecordCount;
uint32_t gnCharTemplateStartIds[64];
// D2Common.0x6FDE9604
D2BeltsTxt* gpBeltsTxtTable;
// D2Common.0x6FDE9608
D2DataTablesStrc gpDataTables;
// D2Common.0x6FDD6A20 (#10042)
extern "C" D2DataTablesStrc * sgptDataTables = &gpDataTables;
BOOL DATATBLS_LoadFromBin = TRUE;

// ARENA & CHARTEMPLATE

// BELTS

//D2Common.0x6FD494D0
uint16_t __fastcall DATATBLS_GetStringIdFromReferenceString(char* szReference)
{
	const Unicode* pUnicode = NULL;
	uint16_t nIndex = 0;

	nIndex = D2LANG_GetTblIndex(szReference, &pUnicode);
	if (!nIndex)
	{
		if (*szReference)
		{
			FOG_Trace("Couldn't find string hash: %s", szReference);
		}
		nIndex = 5382;
	}

	return nIndex;
}

//D2Common.0x6FD49500 - Changed this function a lot (had 6 hardcoded (i.e. pre-defined) Args)
void __fastcall DATATBLS_InitUnicodeClassNamesInCharStatsTxt()
{
	Unicode wszClass[512] = {};
	Unicode wszName[512] = {};

	for (int i = 0; i < sgptDataTables->nCharStatsTxtRecordCount; ++i)
	{
		memset(sgptDataTables->pCharStatsTxt[i].wszClassName, 0x00, ARRAY_SIZE(sgptDataTables->pCharStatsTxt[i].wszClassName));

		const Unicode* wszClassName = D2LANG_GetStringByReferenceString(sgptDataTables->pCharStatsTxt[i].szClassName);

		if (wszClassName)
		{
			Unicode::strncpy(sgptDataTables->pCharStatsTxt[i].wszClassName, wszClassName, ARRAY_SIZE(sgptDataTables->pCharStatsTxt[i].wszClassName));
		}
		else
		{
			Unicode::toUnicode(wszName, sgptDataTables->pCharStatsTxt[i].szClassName, ARRAY_SIZE(sgptDataTables->pCharStatsTxt[i].szClassName) - 1);
			Unicode::strcpy(wszClass, (const Unicode*)L"<");
			Unicode::strcat(wszClass, wszName);
			Unicode::strcat(wszClass, (const Unicode*)L">");

			Unicode::strncpy(sgptDataTables->pCharStatsTxt[i].wszClassName, wszClass, ARRAY_SIZE(sgptDataTables->pCharStatsTxt[i].wszClassName));
		}
	}
}

//D2Common.0x6FD49660 (#11255)
uint32_t __stdcall DATATBLS_GetCodeFromCompCodeTxt(int nCompCode)
{
	if (nCompCode < sgptDataTables->nCompCodeTxtRecordCount)
	{
		return sgptDataTables->pCompCodeTxt[nCompCode].dwCode;
	}

	return 0;
}

//D2Common.0x6FD49680 (#11249)
uint32_t __stdcall DATATBLS_GetExpRatio(int nLevel)
{
	if (sgptDataTables->pExperienceTxt)
	{
		if (nLevel > 0)
		{
			if (nLevel <= (int)sgptDataTables->pExperienceTxt->tMax.dwClass[0])
			{
				return sgptDataTables->pExperienceTxt->aLevels[nLevel].dwExpRatio;
			}
		}
		else
		{
			return sgptDataTables->pExperienceTxt->tMax.dwExpRatio;
		}
	}

	return 0;
}

//D2Common.0x6FD496B0 (#10628)
uint32_t __stdcall DATATBLS_GetLevelThreshold(int nClass, uint32_t dwLevel)
{
	if (nClass < 0 || nClass >= NUMBER_OF_PLAYERCLASSES)
	{
		nClass = 0;
	}

	return sgptDataTables->pExperienceTxt->aLevels[dwLevel].dwClass[nClass];
}

//D2Common.0x6FD496E0 (#10629)
int __stdcall DATATBLS_GetMaxLevel(int nClass)
{
	if (nClass >= 0 && nClass < NUMBER_OF_PLAYERCLASSES)
	{
		return sgptDataTables->pExperienceTxt->tMax.dwClass[nClass];
	}

	return sgptDataTables->pExperienceTxt->tMax.dwClass[0];
}

//D2Common.0x6FD49710 (#10630)
uint32_t __stdcall DATATBLS_GetCurrentLevelFromExp(int nClass, uint32_t dwExperience)
{
	if (nClass < 0 || nClass >= NUMBER_OF_PLAYERCLASSES)
	{
		nClass = 0;
	}

	int nLevel = 0;
	while ( nLevel < sgptDataTables->pExperienceTxt->tMax.dwClass[nClass]
		&& dwExperience >= sgptDataTables->pExperienceTxt->aLevels[nLevel].dwClass[nClass])
	{
		++nLevel;
	}

	return nLevel;
}

//D2Common.0x6FD49760
void __fastcall DATATBLS_GetBinFileHandle(HD2ARCHIVE hArchive, const char* szFile, void** ppFileHandle, int* pSize, int* pSizeEx)
{
	FILE* pFile = NULL;
	size_t dwSize = 0;
	char szFilePath[MAX_PATH] = {};

	wsprintfA(szFilePath, "%s\\%s.bin", "DATA\\GLOBAL\\EXCEL", szFile);

	if (sgptDataTables->bCompileTxt && *ppFileHandle)
	{
		fopen_s(&pFile, szFilePath, "wb");
		if (pFile)
		{
			FileLockAndWrite(*ppFileHandle, *pSize, 1, pFile);

			fclose(pFile);
		}
		D2_FREE_POOL(nullptr, *ppFileHandle);
	}

	*ppFileHandle = ARCHIVE_READ_FILE_TO_ALLOC_BUFFER(hArchive, szFilePath, &dwSize);
	*pSizeEx = dwSize;
	*pSize = dwSize;
}

//D2Common.0x6FD49850
int __fastcall DATATBLS_AppendMemoryBuffer(char** ppCodes, int* pSize, int* pSizeEx, void* pBuffer, int nBufferSize)
{
	int nNewSize = 0;
	int nResult = 0;

	if (nBufferSize + *pSize < *pSizeEx)
	{
		nResult = *pSize;
		memcpy(&(*ppCodes)[*pSize], pBuffer, nBufferSize);
		*pSize += nBufferSize;
	}
	else
	{
		while (1)
		{
			nNewSize = *pSizeEx + 1024;
			*pSizeEx = nNewSize;

			if (nNewSize >= 0x7FFFFFFF)
			{
				break;
			}

			*ppCodes = (char*)D2_REALLOC_POOL(NULL, *ppCodes, nNewSize);

			if (nBufferSize + *pSize < *pSizeEx)
			{
				nResult = *pSize;
				memcpy(&(*ppCodes)[*pSize], pBuffer, nBufferSize);
				*pSize += nBufferSize;
				return nResult;
			}
		}

		nResult = 0;
	}
	return nResult;
}

// SKILLS

//D2Common.0x6FD4E4B0 (#10593)
D2CharStatsTxt* __fastcall DATATBLS_GetCharstatsTxtTable()
{
	return sgptDataTables->pCharStatsTxt;
}

//D2Common.0x6FD4E4C0
D2AnimDataTableStrc* __fastcall DATATBLS_GetAnimData()
{
	return sgptDataTables->pAnimData;
}

//D2Common.0x6FD4E4D0 (#10655)
D2DifficultyLevelsTxt* __stdcall DATATBLS_GetDifficultyLevelsTxtRecord(int nDifficulty)
{
	if (nDifficulty < 0)
	{
		nDifficulty = 0;
	}

	if (nDifficulty > sgptDataTables->nDifficultyLevelsTxtRecordCount - 1)
	{
		nDifficulty = sgptDataTables->nDifficultyLevelsTxtRecordCount - 1;
	}

	return &sgptDataTables->pDifficultyLevelsTxt[nDifficulty];
}

//D2Common.0x6FD4E500
void __fastcall DATATBLS_LoadStatesTxt(HD2ARCHIVE hArchive)
{
	uint32_t* pStateMasks = NULL;

	D2BinFieldStrc pTbl[] =
	{
		{ "state", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pStatesLinker },
		{ "group", TXTFIELD_WORD, 0, 30, NULL },
		{ "nosend", TXTFIELD_BIT, 0, 16, NULL },
		{ "hide", TXTFIELD_BIT, 2, 16, NULL },
		{ "transform", TXTFIELD_BIT, 3, 16, NULL },
		{ "aura", TXTFIELD_BIT, 1, 16, NULL },
		{ "pgsv", TXTFIELD_BIT, 4, 16, NULL },
		{ "active", TXTFIELD_BIT, 5, 16, NULL },
		{ "remhit", TXTFIELD_BIT, 6, 16, NULL },
		{ "damblue", TXTFIELD_BIT, 7, 16, NULL },
		{ "damred", TXTFIELD_BIT, 8, 16, NULL },
		{ "attblue", TXTFIELD_BIT, 9, 16, NULL },
		{ "attred", TXTFIELD_BIT, 10, 16, NULL },
		{ "curse", TXTFIELD_BIT, 11, 16, NULL },
		{ "curable", TXTFIELD_BIT, 12, 16, NULL },
		{ "plrstaydeath", TXTFIELD_BIT, 13, 16, NULL },
		{ "monstaydeath", TXTFIELD_BIT, 14, 16, NULL },
		{ "bossstaydeath", TXTFIELD_BIT, 15, 16, NULL },
		{ "disguise", TXTFIELD_BIT, 16, 16, NULL },
		{ "bossinv", TXTFIELD_BIT, 37, 16, NULL },
		{ "meleeonly", TXTFIELD_BIT, 38, 16, NULL },
		{ "restrict", TXTFIELD_BIT, 17, 16, NULL },
		{ "blue", TXTFIELD_BIT, 18, 16, NULL },
		{ "armblue", TXTFIELD_BIT, 19, 16, NULL },
		{ "rfblue", TXTFIELD_BIT, 20, 16, NULL },
		{ "rcblue", TXTFIELD_BIT, 21, 16, NULL },
		{ "rlblue", TXTFIELD_BIT, 22, 16, NULL },
		{ "rpblue", TXTFIELD_BIT, 23, 16, NULL },
		{ "stambarblue", TXTFIELD_BIT, 24, 16, NULL },
		{ "armred", TXTFIELD_BIT, 25, 16, NULL },
		{ "rfred", TXTFIELD_BIT, 26, 16, NULL },
		{ "rcred", TXTFIELD_BIT, 27, 16, NULL },
		{ "rlred", TXTFIELD_BIT, 28, 16, NULL },
		{ "rpred", TXTFIELD_BIT, 29, 16, NULL },
		{ "exp", TXTFIELD_BIT, 30, 16, NULL },
		{ "shatter", TXTFIELD_BIT, 31, 16, NULL },
		{ "life", TXTFIELD_BIT, 32, 16, NULL },
		{ "udead", TXTFIELD_BIT, 33, 16, NULL },
		{ "green", TXTFIELD_BIT, 34, 16, NULL },
		{ "nooverlays", TXTFIELD_BIT, 35, 16, NULL },
		{ "notondead", TXTFIELD_BIT, 39, 16, NULL },
		{ "noclear", TXTFIELD_BIT, 36, 16, NULL },
		{ "overlay1", TXTFIELD_NAMETOWORD, 0, 2, &sgptDataTables->pOverlayLinker },
		{ "overlay2", TXTFIELD_NAMETOWORD, 0, 4, &sgptDataTables->pOverlayLinker },
		{ "overlay3", TXTFIELD_NAMETOWORD, 0, 6, &sgptDataTables->pOverlayLinker },
		{ "overlay4", TXTFIELD_NAMETOWORD, 0, 8, &sgptDataTables->pOverlayLinker },
		{ "pgsvoverlay", TXTFIELD_NAMETOWORD, 0, 14, &sgptDataTables->pOverlayLinker },
		{ "castoverlay", TXTFIELD_NAMETOWORD, 0, 10, &sgptDataTables->pOverlayLinker },
		{ "removerlay", TXTFIELD_NAMETOWORD, 0, 12, &sgptDataTables->pOverlayLinker },
		{ "stat", TXTFIELD_NAMETOWORD, 0, 24, &sgptDataTables->pItemStatCostLinker },
		{ "setfunc", TXTFIELD_WORD, 0, 26, NULL },
		{ "remfunc", TXTFIELD_WORD, 0, 28, NULL },
		{ "missile", TXTFIELD_NAMETOWORD, 0, 58, &sgptDataTables->pMissilesLinker },
		{ "skill", TXTFIELD_NAMETOWORD, 0, 56, &sgptDataTables->iSkillCode },
		{ "colorpri", TXTFIELD_BYTE, 0, 32, NULL },
		{ "colorshift", TXTFIELD_BYTE, 0, 33, NULL },
		{ "light-r", TXTFIELD_BYTE, 0, 34, NULL },
		{ "light-g", TXTFIELD_BYTE, 0, 35, NULL },
		{ "light-b", TXTFIELD_BYTE, 0, 36, NULL },
		{ "onsound", TXTFIELD_NAMETOWORD, 0, 38, &sgptDataTables->pSoundsLinker },
		{ "offsound", TXTFIELD_NAMETOWORD, 0, 40, &sgptDataTables->pSoundsLinker },
		{ "itemtype", TXTFIELD_CODETOWORD, 0, 42, &sgptDataTables->pItemTypesLinker },
		{ "itemtrans", TXTFIELD_CODETOBYTE, 0, 44, &sgptDataTables->pColorsLinker },
		{ "gfxtype", TXTFIELD_BYTE, 0, 45, NULL },
		{ "gfxclass", TXTFIELD_WORD, 0, 46, NULL },
		{ "cltevent", TXTFIELD_NAMETOWORD, 0, 48, &sgptDataTables->pEventsLinker },
		{ "clteventfunc", TXTFIELD_WORD, 0, 50, NULL },
		{ "cltactivefunc", TXTFIELD_WORD, 0, 52, NULL },
		{ "srvactivefunc", TXTFIELD_WORD, 0, 54, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pStatesLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pStatesTxt = (D2StatesTxt*)DATATBLS_CompileTxt(hArchive, "states", pTbl, &sgptDataTables->nStatesTxtRecordCount, sizeof(D2StatesTxt));

	if (sgptDataTables->nStatesTxtRecordCount >= 256)
	{
		FOG_DisplayWarning("Exceeded maximum allowable number of states", __FILE__, __LINE__);
	}

	sgptDataTables->pStateMasks = (uint32_t*)D2_CALLOC_POOL(nullptr, ARRAY_SIZE(sgptDataTables->fStateMasks) * sizeof(uint32_t) * (sgptDataTables->nStatesTxtRecordCount + 31) / 32);
	
	for (int i = 0; i < ARRAY_SIZE(sgptDataTables->fStateMasks); ++i)
	{
		pStateMasks = &sgptDataTables->pStateMasks[(sgptDataTables->nStatesTxtRecordCount + 31) / 32 * i];
		sgptDataTables->fStateMasks[i] = pStateMasks;

		for (int j = 0; j < sgptDataTables->nStatesTxtRecordCount; ++j)
		{
			if (sgptDataTables->pStatesTxt[j].nStateFlags[i >> 3] & gdwBitMasks[i & 7])
			{
				pStateMasks[j >> 5] |= gdwBitMasks[j & 31];
			}
		}
	}

	sgptDataTables->pProgressiveStates = (short*)D2_CALLOC_POOL(nullptr, sizeof(short) * sgptDataTables->nStatesTxtRecordCount);
	sgptDataTables->nProgressiveStates = 0;

	sgptDataTables->pCurseStates = (short*)D2_CALLOC_POOL(nullptr, sizeof(short) * sgptDataTables->nStatesTxtRecordCount);
	sgptDataTables->nCurseStates = 0;

	sgptDataTables->pTransformStates = (short*)D2_CALLOC_POOL(nullptr, sizeof(short) * sgptDataTables->nStatesTxtRecordCount);
	sgptDataTables->nTransformStates = 0;

	sgptDataTables->pActionStates = (short*)D2_CALLOC_POOL(nullptr, sizeof(short) * sgptDataTables->nStatesTxtRecordCount);
	sgptDataTables->nActionStates = 0;

	sgptDataTables->pColourStates = (short*)D2_CALLOC_POOL(nullptr, sizeof(short) * sgptDataTables->nStatesTxtRecordCount);
	sgptDataTables->nColourStates = 0;
	
	for (int i = 0; i < sgptDataTables->nStatesTxtRecordCount; ++i)
	{
		if (sgptDataTables->pStatesTxt[i].dwStateFlags & gdwBitMasks[STATEMASK_PGSV])
		{
			sgptDataTables->pProgressiveStates[sgptDataTables->nProgressiveStates] = i;
			++sgptDataTables->nProgressiveStates;
		}

		if (sgptDataTables->pStatesTxt[i].dwStateFlags & gdwBitMasks[STATEMASK_CURSE])
		{
			sgptDataTables->pCurseStates[sgptDataTables->nCurseStates] = i;
			++sgptDataTables->nCurseStates;
		}

		if (sgptDataTables->pStatesTxt[i].dwStateFlags & gdwBitMasks[STATEMASK_DISGUISE])
		{
			sgptDataTables->pTransformStates[sgptDataTables->nTransformStates] = i;
			++sgptDataTables->nTransformStates;
		}

		if (sgptDataTables->pStatesTxt[i].dwStateFlags & gdwBitMasks[STATEMASK_ACTIVE])
		{
			sgptDataTables->pActionStates[sgptDataTables->nActionStates] = i;
			++sgptDataTables->nActionStates;
		}

		if (sgptDataTables->pStatesTxt[i].wItemType > 0)
		{
			sgptDataTables->pColourStates[sgptDataTables->nColourStates] = i;
			++sgptDataTables->nColourStates;
		}
	}
}

//D2Common.0x6FD4F4A0
void __fastcall DATATBLS_UnloadStatesTxt()
{
	if (sgptDataTables->pStateMasks)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pStateMasks);
		sgptDataTables->pStateMasks = NULL;
	}

	if (sgptDataTables->pProgressiveStates)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pProgressiveStates);
		sgptDataTables->pProgressiveStates = NULL;
	}

	if (sgptDataTables->pCurseStates)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pCurseStates);
		sgptDataTables->pCurseStates = NULL;
	}

	if (sgptDataTables->pTransformStates)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pTransformStates);
		sgptDataTables->pTransformStates = NULL;
	}

	if (sgptDataTables->pActionStates)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pActionStates);
		sgptDataTables->pActionStates = NULL;
	}

	memset(sgptDataTables->fStateMasks, 0x00, sizeof(sgptDataTables->fStateMasks));

	DATATBLS_UnloadBin(sgptDataTables->pStatesTxt);
	sgptDataTables->pStatesTxt = NULL;
	FOG_FreeLinker(sgptDataTables->pStatesLinker);
	sgptDataTables->pStatesLinker = NULL;
}

D2StatesTxt* DATATBLS_GetStatesTxtRecord(int nStateId)
{
	if (nStateId >= 0 && nStateId < sgptDataTables->nStatesTxtRecordCount)
	{
		return &sgptDataTables->pStatesTxt[nStateId];
	}

	return NULL;
}

//D2Common.0x6FD4F5A0
void __fastcall DATATBLS_LoadPetTypeTxt(HD2ARCHIVE hArchive)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "pet type", TXTFIELD_NAMETOINDEX2, 0, 0, &sgptDataTables->pPetTypeLinker },
		{ "group", TXTFIELD_WORD, 0, 8, NULL },
		{ "basemax", TXTFIELD_WORD, 0, 10, NULL },
		{ "warp", TXTFIELD_BIT, 0, 4, NULL },
		{ "range", TXTFIELD_BIT, 1, 4, NULL },
		{ "partysend", TXTFIELD_BIT, 2, 4, NULL },
		{ "unsummon", TXTFIELD_BIT, 3, 4, NULL },
		{ "automap", TXTFIELD_BIT, 4, 4, NULL },
		{ "drawhp", TXTFIELD_BIT, 5, 4, NULL },
		{ "icontype", TXTFIELD_BYTE, 0, 14, NULL },
		{ "baseicon", TXTFIELD_ASCII, 32, 15, NULL },
		{ "mclass1", TXTFIELD_WORD, 0, 178, NULL },
		{ "micon1", TXTFIELD_ASCII, 32, 47, NULL },
		{ "mclass2", TXTFIELD_WORD, 0, 180, NULL },
		{ "micon2", TXTFIELD_ASCII, 32, 79, NULL },
		{ "mclass3", TXTFIELD_WORD, 0, 182, NULL },
		{ "micon3", TXTFIELD_ASCII, 32, 111, NULL },
		{ "mclass4", TXTFIELD_WORD, 0, 184, NULL },
		{ "micon4", TXTFIELD_ASCII, 32, 143, NULL },
		{ "name", TXTFIELD_KEYTOWORD, 0, 12, DATATBLS_GetStringIdFromReferenceString },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pPetTypeLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pPetTypeTxt = (D2PetTypeTxt*)DATATBLS_CompileTxt(hArchive, "pettype", pTbl, &sgptDataTables->nPetTypeTxtRecordCount, sizeof(D2PetTypeTxt));

	if (sgptDataTables->nPetTypeTxtRecordCount > 0)
	{
		if (sgptDataTables->nPetTypeTxtRecordCount >= 256)
		{
			FOG_DisplayWarning("Pet types table exceeded maximum number of entries.", __FILE__, __LINE__);
			sgptDataTables->nPetTypeTxtRecordCount = 256;
		}
	}
	else
	{
		sgptDataTables->nPetTypeTxtRecordCount = 0;
	}
}

//D2Common.0x6FD4F990 (#11298)
char* __stdcall DATATBLS_GetUnitNameFromUnit(D2UnitStrc* pUnit, char* szName)
{
	*szName = 0;

	if (pUnit)
	{
		return DATATBLS_GetUnitNameFromUnitTypeAndClassId(pUnit->dwUnitType, pUnit->dwClassId, szName);
	}
	else
	{
		printf(szName, "NULL Unit");
	}

	return szName;
}

//D2Common.0x6FD4FB50 (#11299)
char* __stdcall DATATBLS_GetUnitNameFromUnitTypeAndClassId(int nUnitType, int nClassId, char* szName)
{
	D2CharStatsTxt* pCharStatsTxtRecord = NULL;
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	D2ObjectsTxt* pObjectsTxtRecord = NULL;
	D2ItemsTxt* pItemsTxtRecord = NULL;

	*szName = 0;

	switch (nUnitType)
	{
	case UNIT_PLAYER:
		if (nClassId >= 0 && nClassId < sgptDataTables->nCharStatsTxtRecordCount)
		{
			pCharStatsTxtRecord = &sgptDataTables->pCharStatsTxt[nClassId];
			if (pCharStatsTxtRecord && pCharStatsTxtRecord->szClassName[0])
			{
				strcpy_s(szName, 64, pCharStatsTxtRecord->szClassName);
			}
			else
			{
				sprintf_s(szName, 64, "player %d", nClassId);
			}
		}
		else
		{
			sprintf_s(szName, 64, "player %d", nClassId);
		}
		break;

	case UNIT_MONSTER:
		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nClassId);
		if (pMonStatsTxtRecord && pMonStatsTxtRecord->wNameStr != 5382)
		{
			Unicode::unicode2Win(szName, (const Unicode*)D2LANG_GetStringFromTblIndex(pMonStatsTxtRecord->wNameStr), 64);
		}
		else
		{
			sprintf_s(szName, 64, "monster %d", nClassId);
		}
		break;

	case UNIT_OBJECT:
		pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(nClassId);
		if (pObjectsTxtRecord && pObjectsTxtRecord->szName[0])
		{
			strcpy_s(szName, 64, pObjectsTxtRecord->szName);
		}
		else
		{
			sprintf_s(szName, 64, "object %d", nClassId);
		}
		break;

	case UNIT_MISSILE:
		sprintf_s(szName, 64, "missile %d", nClassId);
		break;

	case UNIT_ITEM:
		pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(nClassId);
		if (pItemsTxtRecord && pItemsTxtRecord->wNameStr != 5382)
		{
			Unicode::unicode2Win(szName, (const Unicode*)D2LANG_GetStringFromTblIndex(pItemsTxtRecord->wNameStr), 64);
		}
		else
		{
			sprintf_s(szName, 64, "item %d", nClassId);
		}
		break;

	default:
		sprintf_s(szName, 64, "Invalid Unit");
		break;
	}

	return szName;
}

//D2Common.0x6FD4FCF0 (#10580)
void __stdcall DATATBLS_WriteBinFile(char* szFileName, void* pWriteBuffer, size_t nBufferSize, int nRecordCount)
{
	FILE* pFile = NULL;
	char szFilePath[MAX_PATH] = {};

	wsprintfA(szFilePath, "%s\\%s", "DATA\\GLOBAL\\EXCEL", szFileName);

	fopen_s(&pFile, szFilePath, "wb");
	if (pFile)
	{
		FileLockAndWrite(&nRecordCount, sizeof(nRecordCount), 1, pFile);
		FileLockAndWrite(pWriteBuffer, nBufferSize, 1, pFile);
		fclose(pFile);
	}
}

//D2Common.0x6FD4FD70 (#10578)
void* __stdcall DATATBLS_CompileTxt(HD2ARCHIVE hArchive, const char* szName, D2BinFieldStrc* pTbl, int* pRecordCount, size_t dwSize)
{
	D2BinFileStrc* pBinFile = NULL;
	FILE* pFile = NULL;
	void* pData = NULL;
	void* pTxt = NULL;
	int nRecordCount = 0;
	size_t dwDataSize = 0;
	char szFilePath[MAX_PATH] = {};

	dwDataSize = 0;
	if (sgptDataTables->bCompileTxt)
	{
		if (_strcmpi(szName, "leveldefs"))
		{
			FOG_Trace("Translating data from: %s", szName);
			wsprintfA(szFilePath, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", szName, ".txt");
		}
		else
		{
			FOG_Trace("Translating data from: %s", "levels");
			wsprintfA(szFilePath, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", "levels", ".txt");
		}

		pData = ARCHIVE_READ_FILE_TO_ALLOC_BUFFER(hArchive, szFilePath, &dwDataSize);
		D2_ASSERT(pData);

		pBinFile = FOG_CreateBinFile(pData, dwDataSize);
		nRecordCount = FOG_GetRecordCountFromBinFile(pBinFile);
		pTxt = D2_CALLOC_POOL(nullptr, dwSize * nRecordCount);
		FOG_10207(pBinFile, pTbl, pTxt, nRecordCount, dwSize);
		FOG_FreeBinFile(pBinFile);

		wsprintfA(szFilePath, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", szName, ".bin");
		fopen_s(&pFile, szFilePath, "wb");
		if (pFile)
		{
			FileLockAndWrite(&nRecordCount, 4, 1, pFile);
			FileLockAndWrite(pTxt, dwSize * nRecordCount, 1, pFile);
			fclose(pFile);
		}
		D2_FREE_POOL(nullptr, pTxt);
	}

	if (DATATBLS_LoadFromBin)
	{
		wsprintfA(szFilePath, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", szName, ".bin");
	}
	else
	{
		if (_strcmpi(szName, "leveldefs"))
		{
			wsprintfA(szFilePath, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", szName, ".txt");
		}
		else
		{
			wsprintfA(szFilePath, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", "levels", ".txt");
		}
	}

	pData = ARCHIVE_READ_FILE_TO_ALLOC_BUFFER(hArchive, szFilePath, &dwDataSize);
	D2_ASSERT(pData);

	if (DATATBLS_LoadFromBin)
	{
		nRecordCount = *(int*)pData;
		pTxt = (char*)pData + 4;
	}
	else
	{
		pBinFile = FOG_CreateBinFile(pData, dwDataSize);
		nRecordCount = FOG_GetRecordCountFromBinFile(pBinFile);
		pTxt = D2_CALLOC_POOL(nullptr, dwSize * nRecordCount);
		FOG_10207(pBinFile, pTbl, pTxt, nRecordCount, dwSize);
		FOG_FreeBinFile(pBinFile);
	}

	if (pRecordCount)
	{
		*pRecordCount = nRecordCount;
	}

	return pTxt;
}

//D2Common.0x6FD500F0 (#11242)
void __stdcall DATATBLS_ToggleCompileTxtFlag(BOOL bSilent)
{
	sgptDataTables->bCompileTxt = !bSilent;
}

//D2Common.0x6FD50110 (#10579)
void __stdcall DATATBLS_UnloadBin(void* pBinFile)
{
	if (pBinFile)
	{
		if (DATATBLS_LoadFromBin)
		{
			D2_FREE_POOL(nullptr, (char*)pBinFile - 4);
		}
		else
		{
			D2_FREE_POOL(nullptr, pBinFile);
		}
	}
}

//D2Common.0x6FD50150 (#10575)
void __stdcall DATATBLS_UnloadAllBins()
{
	DATATBLS_UnloadBin(sgptDataTables->pCompCodeTxt);
	FOG_FreeLinker(sgptDataTables->pCompCodeLinker);

	if (sgptDataTables->bCompileTxt)
	{
		DATATBLS_UnloadBin(sgptDataTables->pPlayerClassTxt);
		FOG_FreeLinker(sgptDataTables->pPlayerClassLinker);
		DATATBLS_UnloadBin(sgptDataTables->pBodyLocsTxt);
		FOG_FreeLinker(sgptDataTables->pBodyLocsLinker);
		DATATBLS_UnloadBin(sgptDataTables->pStorePageTxt);
		FOG_FreeLinker(sgptDataTables->pStorePageLinker);
		DATATBLS_UnloadBin(sgptDataTables->pElemTypesTxt);
		FOG_FreeLinker(sgptDataTables->pElemTypesLinker);
		DATATBLS_UnloadBin(sgptDataTables->pHitClassTxt);
		FOG_FreeLinker(sgptDataTables->pHitClassLinker);
		DATATBLS_UnloadBin(sgptDataTables->pColorsTxt);
		FOG_FreeLinker(sgptDataTables->pColorsLinker);
		DATATBLS_UnloadBin(sgptDataTables->pHireDescTxt);
		FOG_FreeLinker(sgptDataTables->pHireDescLinker);
		DATATBLS_UnloadBin(sgptDataTables->pMonModeTxtStub);
		FOG_FreeLinker(sgptDataTables->pMonModeLinker);
		DATATBLS_UnloadBin(sgptDataTables->pPlrModeTxtStub);
		FOG_FreeLinker(sgptDataTables->pPlrModeLinker);
		DATATBLS_UnloadBin(sgptDataTables->pMonAiTxt);
		FOG_FreeLinker(sgptDataTables->pMonAiLinker);
		DATATBLS_UnloadBin(sgptDataTables->pMonPlaceTxt);
		FOG_FreeLinker(sgptDataTables->pMonPlaceLinker);
		DATATBLS_UnloadBin(sgptDataTables->pSkillCalcTxt);
		FOG_FreeLinker(sgptDataTables->pSkillCalcLinker);
		DATATBLS_UnloadBin(sgptDataTables->pMissileCalcTxt);
		FOG_FreeLinker(sgptDataTables->pMissileCalcLinker);
		DATATBLS_UnloadBin((void*)sgptDataTables->pSkillCode);
		FOG_FreeLinker(sgptDataTables->iSkillCode);
		DATATBLS_UnloadBin(sgptDataTables->pEventsTxt);
		FOG_FreeLinker(sgptDataTables->pEventsLinker);
	}

	DATATBLS_UnloadBin(sgptDataTables->pCharStatsTxt);
	DATATBLS_UnloadMissilesTxt();
	DATATBLS_UnloadOverlayTxt();
	DATATBLS_UnloadSkills_SkillDescTxt();
	DATATBLS_UnloadItemStatCostTxt();
	DATATBLS_UnloadStatesTxt();

	DATATBLS_UnloadBin(sgptDataTables->pPetTypeTxt);
	FOG_FreeLinker(sgptDataTables->pPetTypeLinker);

	DATATBLS_UnloadItemsTxt();
	DATATBLS_UnloadPropertiesTxt();
	DATATBLS_UnloadMagicSuffix_Prefix_AutomagicTxt();
	DATATBLS_UnloadRareSuffix_PrefixTxt();
	DATATBLS_UnloadUniqueItemsTxt();
	DATATBLS_UnloadSets_SetItemsTxt();
	DATATBLS_UnloadQualityItemsTxt();
	DATATBLS_UnloadLowQualityItemsTxt();
	DATATBLS_UnloadGemsTxt();
	DATATBLS_UnloadBooksTxt();
	DATATBLS_UnloadItemRatioTxt();
	DATATBLS_UnloadGambleTxt();
	DATATBLS_UnloadItemTypesTxt();
	DATATBLS_UnloadMonTypeTxt();
	DATATBLS_UnloadPlrMode_Type_MonMode_ObjMode_Type_Composit_ArmtypeTxt();
	DATATBLS_UnloadBin(sgptDataTables->pExperienceTxt);
	DATATBLS_UnloadAnimDataD2(sgptDataTables->pAnimData);
	DATATBLS_UnloadSomeMonsterTxts();
	DATATBLS_UnloadLevelsTxt();
	DATATBLS_UnloadLevelDefsBin();
	DATATBLS_UnloadLevelTypesTxt();
	DATATBLS_UnloadLvlPrestTxt();
	DATATBLS_UnloadLvlWarpTxt();
	DATATBLS_UnloadLvlMazeTxt();
	DATATBLS_FreeGlobalTileLibraryHash();
	DATATBLS_UnloadLvlSubTxt();
	DATATBLS_FreeAutomap();
	DATATBLS_UnloadObjectsTxt();
	DATATBLS_UnloadObjGroupTxt();
	DATATBLS_UnloadShrinesTxt();
	DATATBLS_UnloadInventoryTxt();
	DATATBLS_UnloadRunesTxt();
	DATATBLS_FreeCollisionFieldTable();
	DATATBLS_UnloadBeltsTxt();
	DATATBLS_UnloadMonItemPercentTxt();
	DATATBLS_UnloadArenaTxt();
	DATATBLS_UnloadCubeMainTxt();
	DATATBLS_UnloadCharTemplateTxt();
	//D2COMMON_10916_Return();
	DATATBLS_UnloadBin(sgptDataTables->pDifficultyLevelsTxt);
}

//D2Common.0x6FD504B0 (#10576)
void __stdcall DATATBLS_LoadAllTxts(HD2ARCHIVE hArchive, int a2, int a3)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Amazon", TXTFIELD_DWORD, 0, 0, NULL },
		{ "Sorceress", TXTFIELD_DWORD, 0, 4, NULL },
		{ "Necromancer", TXTFIELD_DWORD, 0, 8, NULL },
		{ "Paladin", TXTFIELD_DWORD, 0, 12, NULL },
		{ "Barbarian", TXTFIELD_DWORD, 0, 16, NULL },
		{ "Druid", TXTFIELD_DWORD, 0, 20, NULL },
		{ "Assassin", TXTFIELD_DWORD, 0, 24, NULL },
		{ "ExpRatio", TXTFIELD_DWORD, 0, 28, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	DATATBLS_LoadSomeTxts(hArchive);
	DATATBLS_LoadItemTypesTxt(hArchive);
	DATATBLS_LoadMonTypeTxt(hArchive);
	DATATBLS_LoadSoundsTxt(hArchive);
	DATATBLS_LoadPetTypeTxt(hArchive);
	DATATBLS_LoadOverlayTxt(hArchive);
	DATATBLS_LoadItemStatCostTxt(hArchive);
	DATATBLS_LoadPropertiesTxt(hArchive);
	DATATBLS_LoadMissilesTxt(hArchive);
	DATATBLS_LoadStatesTxt(hArchive);
	DATATBLS_LoadSkills_SkillDescTxt(hArchive);
	DATATBLS_LoadCharStatsTxt(hArchive);
	DATATBLS_LoadArenaTxt(hArchive);
	DATATBLS_LoadCharTemplateTxt(hArchive);
	DATATBLS_LoadItemsTxt(hArchive);
	DATATBLS_LoadMagicSuffix_Prefix_AutomagicTxt(hArchive);
	DATATBLS_LoadRareSuffix_PrefixTxt(hArchive);
	DATATBLS_LoadUniqueItemsTxt(hArchive);
	DATATBLS_LoadSets_SetItemsTxt(hArchive);
	DATATBLS_LoadGemsTxt(hArchive);
	DATATBLS_LoadBooksTxt(hArchive);
	DATATBLS_LoadQualityItemsTxt(hArchive);
	DATATBLS_LoadLowQualityItemsTxt(hArchive);
	DATATBLS_LoadRunesTxt(hArchive);
	DATATBLS_LoadItemRatioTxt(hArchive);
	DATATBLS_LoadGambleTxt(hArchive);
	DATATBLS_LoadPlrType_ModeTxt(hArchive);
	DATATBLS_LoadMonModeTxt(hArchive);
	DATATBLS_LoadObjType_ModeTxt(hArchive);
	DATATBLS_LoadCompositTxt(hArchive);
	DATATBLS_LoadArmTypeTxt(hArchive);

	sgptDataTables->pExperienceTxt = (D2ExperienceDataTbl*)DATATBLS_CompileTxt(hArchive, "experience", pTbl, NULL, sizeof(D2ExperienceTxt));

	sgptDataTables->pAnimData = DATATBLS_LoadAnimDataD2(hArchive);
	DATATBLS_LoadSomeMonsterTxts(hArchive);
	DATATBLS_LoadLevelsTxt(hArchive);
	DATATBLS_LoadLevelDefsBin(hArchive);
	DATATBLS_LoadLevelTypesTxt(hArchive);
	if (a2)
	{
		DATATBLS_AllocGlobalTileLibraryHash();
	}
	DATATBLS_LoadLvlPrestTxt(hArchive, a2);
	DATATBLS_LoadLvlWarpTxt(hArchive);
	DATATBLS_LoadLvlMazeTxt(hArchive);
	DATATBLS_LoadLvlSubTxt(hArchive, a2, a3);
	DATATBLS_LoadAutomapTxt(hArchive);
	DATATBLS_LoadObjectsTxt(hArchive);
	DATATBLS_LoadObjGroupTxt(hArchive);
	DATATBLS_LoadShrinesTxt(hArchive);
	DATATBLS_LoadInventoryTxt(hArchive);
	DATATBLS_LoadExpFieldD2(hArchive);
	DATATBLS_LoadBeltsTxt(hArchive);
	DATATBLS_LoadMonItemPercentTxt(hArchive);
	//DATATBLS_10916_Return();
	DATATBLS_LoadCubeMainTxt(hArchive);
	DATATBLS_LoadDifficultyLevelsTxt(hArchive);
	DATATBLS_UnloadSoundsTxt();
}

//D2Common.0x6FD507B0
void __fastcall DATATBLS_LoadSomeTxts(HD2ARCHIVE hArchive)
{
	int nRecordCount = 0;

	D2BinFieldStrc pHireDescTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pHireDescLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pMonModeTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pMonModeLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pPlayerClassTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pPlayerClassLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pPlrModeTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pPlrModeLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pStorePageTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pStorePageLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pMonAiTbl[] =
	{
		{ "AI", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pMonAiLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pHitClassTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pHitClassLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pMonPlaceTbl[] =
	{
		{ "code", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pMonPlaceLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pCompCodeTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pCompCodeLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pSkillCalcTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pSkillCalcLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pElemTypesTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pElemTypesLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pMissCalcTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pMissileCalcLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pBodyLocsTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pBodyLocsLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pSkillCodeTbl[] =
	{
		{ "skill", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->iSkillCode },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pColorsTbl[] =
	{
		{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &sgptDataTables->pColorsLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	D2BinFieldStrc pEventsTbl[] =
	{
		{ "event", TXTFIELD_NAMETOINDEX, 0, 0, &sgptDataTables->pEventsLinker },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pCompCodeLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
	sgptDataTables->pCompCodeTxt = (D2CompCodeTxt*)DATATBLS_CompileTxt(hArchive, "compcode", pCompCodeTbl, &sgptDataTables->nCompCodeTxtRecordCount, sizeof(D2CompCodeTxt));

	if (sgptDataTables->bCompileTxt)
	{
		sgptDataTables->pPlayerClassLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pPlayerClassTxt = (D2PlayerClassTxt*)DATATBLS_CompileTxt(hArchive, "playerclass", pPlayerClassTbl, &nRecordCount, sizeof(D2PlayerClassTxt));

		sgptDataTables->pBodyLocsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pBodyLocsTxt = (D2BodyLocsTxt*)DATATBLS_CompileTxt(hArchive, "bodylocs", pBodyLocsTbl, &nRecordCount, sizeof(D2BodyLocsTxt));

		sgptDataTables->pStorePageLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pStorePageTxt = (D2StorePageTxt*)DATATBLS_CompileTxt(hArchive, "storepage", pStorePageTbl, &nRecordCount, sizeof(D2StorePageTxt));

		sgptDataTables->pElemTypesLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pElemTypesTxt = (D2ElemTypesTxt*)DATATBLS_CompileTxt(hArchive, "elemtypes", pElemTypesTbl, &nRecordCount, sizeof(D2ElemTypesTxt));

		sgptDataTables->pHitClassLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pHitClassTxt = (D2HitClassTxt*)DATATBLS_CompileTxt(hArchive, "hitclass", pHitClassTbl, &nRecordCount, sizeof(D2HitClassTxt));

		sgptDataTables->pColorsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pColorsTxt = (D2ColorsTxt*)DATATBLS_CompileTxt(hArchive, "colors", pColorsTbl, &nRecordCount, sizeof(D2ColorsTxt));

		sgptDataTables->pHireDescLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pHireDescTxt = (D2HireDescTxt*)DATATBLS_CompileTxt(hArchive, "hiredesc", pHireDescTbl, &nRecordCount, sizeof(D2HireDescTxt));

		sgptDataTables->pMonModeLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pMonModeTxtStub = (D2MonModeTxtStub*)DATATBLS_CompileTxt(hArchive, "monmode", pMonModeTbl, &nRecordCount, sizeof(D2MonModeTxtStub));

		sgptDataTables->pPlrModeLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pPlrModeTxtStub = (D2PlrModeTxtStub*)DATATBLS_CompileTxt(hArchive, "plrmode", pPlrModeTbl, &nRecordCount, sizeof(D2PlrModeTxtStub));

		sgptDataTables->pMonAiLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pMonAiTxt = (D2MonAiTxt*)DATATBLS_CompileTxt(hArchive, "monai", pMonAiTbl, &sgptDataTables->nMonAiTxtRecordCount, sizeof(D2MonAiTxt));

		sgptDataTables->pMonPlaceLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pMonPlaceTxt = (D2MonPlaceTxt*)DATATBLS_CompileTxt(hArchive, "monplace", pMonPlaceTbl, &sgptDataTables->nMonPlaceTxtRecordCount, sizeof(D2MonPlaceTxt));

		sgptDataTables->pSkillCalcLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pSkillCalcTxt = (D2SkillCalcTxt*)DATATBLS_CompileTxt(hArchive, "skillcalc", pSkillCalcTbl, &nRecordCount, sizeof(D2SkillCalcTxt));

		sgptDataTables->pMissileCalcLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pMissileCalcTxt = (D2MissCalcTxt*)DATATBLS_CompileTxt(hArchive, "misscalc", pMissCalcTbl, &nRecordCount, sizeof(D2MissCalcTxt));

		sgptDataTables->iSkillCode = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pSkillCode = (const char*)DATATBLS_CompileTxt(hArchive, "skills", pSkillCodeTbl, &nRecordCount, 2);

		sgptDataTables->pEventsLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		sgptDataTables->pEventsTxt = (D2EventsTxt*)DATATBLS_CompileTxt(hArchive, "events", pEventsTbl, &nRecordCount, sizeof(D2EventsTxt));
	}
}

//D2Common.0x6FD50FB0
void __fastcall DATATBLS_LoadCharStatsTxt(HD2ARCHIVE hArchive)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "class", TXTFIELD_ASCII, 15, 32, NULL },
		{ "str", TXTFIELD_BYTE, 0, 48, NULL },
		{ "dex", TXTFIELD_BYTE, 0, 49, NULL },
		{ "vit", TXTFIELD_BYTE, 0, 51, NULL },
		{ "int", TXTFIELD_BYTE, 0, 50, NULL },
		{ "stamina", TXTFIELD_BYTE, 0, 52, NULL },
		{ "hpadd", TXTFIELD_BYTE, 0, 53, NULL },
		{ "PercentStr", TXTFIELD_BYTE, 0, 54, NULL },
		{ "PercentDex", TXTFIELD_BYTE, 0, 56, NULL },
		{ "PercentVit", TXTFIELD_BYTE, 0, 57, NULL },
		{ "PercentInt", TXTFIELD_BYTE, 0, 55, NULL },
		{ "ManaRegen", TXTFIELD_BYTE, 0, 58, NULL },
		{ "ToHitFactor", TXTFIELD_DWORD, 0, 60, NULL },
		{ "WalkVelocity", TXTFIELD_BYTE, 0, 64, NULL },
		{ "RunVelocity", TXTFIELD_BYTE, 0, 65, NULL },
		{ "RunDrain", TXTFIELD_BYTE, 0, 66, NULL },
		{ "LifePerLevel", TXTFIELD_BYTE, 0, 67, NULL },
		{ "StaminaPerLevel", TXTFIELD_BYTE, 0, 68, NULL },
		{ "ManaPerLevel", TXTFIELD_BYTE, 0, 69, NULL },
		{ "LifePerVitality", TXTFIELD_BYTE, 0, 70, NULL },
		{ "StaminaPerVitality", TXTFIELD_BYTE, 0, 71, NULL },
		{ "ManaPerMagic", TXTFIELD_BYTE, 0, 72, NULL },
		{ "BlockFactor", TXTFIELD_BYTE, 0, 73, NULL },
		{ "basewclass", TXTFIELD_RAW, 0, 76, NULL },
		{ "StrAllSkills", TXTFIELD_KEYTOWORD, 0, 82, DATATBLS_GetStringIdFromReferenceString },
		{ "StrSkillTab1", TXTFIELD_KEYTOWORD, 0, 84, DATATBLS_GetStringIdFromReferenceString },
		{ "StrSkillTab2", TXTFIELD_KEYTOWORD, 0, 86, DATATBLS_GetStringIdFromReferenceString },
		{ "StrSkillTab3", TXTFIELD_KEYTOWORD, 0, 88, DATATBLS_GetStringIdFromReferenceString },
		{ "StrClassOnly", TXTFIELD_KEYTOWORD, 0, 90, DATATBLS_GetStringIdFromReferenceString },
		{ "StartSkill", TXTFIELD_NAMETOWORD, 0, 172, &sgptDataTables->pSkillsLinker },
		{ "Skill 1", TXTFIELD_NAMETOWORD, 0, 174, &sgptDataTables->pSkillsLinker },
		{ "Skill 2", TXTFIELD_NAMETOWORD, 0, 176, &sgptDataTables->pSkillsLinker },
		{ "Skill 3", TXTFIELD_NAMETOWORD, 0, 178, &sgptDataTables->pSkillsLinker },
		{ "Skill 4", TXTFIELD_NAMETOWORD, 0, 180, &sgptDataTables->pSkillsLinker },
		{ "Skill 5", TXTFIELD_NAMETOWORD, 0, 182, &sgptDataTables->pSkillsLinker },
		{ "Skill 6", TXTFIELD_NAMETOWORD, 0, 184, &sgptDataTables->pSkillsLinker },
		{ "Skill 7", TXTFIELD_NAMETOWORD, 0, 186, &sgptDataTables->pSkillsLinker },
		{ "Skill 8", TXTFIELD_NAMETOWORD, 0, 188, &sgptDataTables->pSkillsLinker },
		{ "Skill 9", TXTFIELD_NAMETOWORD, 0, 190, &sgptDataTables->pSkillsLinker },
		{ "Skill 10", TXTFIELD_NAMETOWORD, 0, 192, &sgptDataTables->pSkillsLinker },
		{ "StatPerLevel", TXTFIELD_BYTE, 0, 80, NULL },
		{ "item1loc", TXTFIELD_CODETOBYTE, 0, 96, &sgptDataTables->pBodyLocsLinker },
		{ "item2loc", TXTFIELD_CODETOBYTE, 0, 104, &sgptDataTables->pBodyLocsLinker },
		{ "item3loc", TXTFIELD_CODETOBYTE, 0, 112, &sgptDataTables->pBodyLocsLinker },
		{ "item4loc", TXTFIELD_CODETOBYTE, 0, 120, &sgptDataTables->pBodyLocsLinker },
		{ "item5loc", TXTFIELD_CODETOBYTE, 0, 128, &sgptDataTables->pBodyLocsLinker },
		{ "item6loc", TXTFIELD_CODETOBYTE, 0, 136, &sgptDataTables->pBodyLocsLinker },
		{ "item7loc", TXTFIELD_CODETOBYTE, 0, 144, &sgptDataTables->pBodyLocsLinker },
		{ "item8loc", TXTFIELD_CODETOBYTE, 0, 152, &sgptDataTables->pBodyLocsLinker },
		{ "item9loc", TXTFIELD_CODETOBYTE, 0, 160, &sgptDataTables->pBodyLocsLinker },
		{ "item10loc", TXTFIELD_CODETOBYTE, 0, 168, &sgptDataTables->pBodyLocsLinker },
		{ "item1", TXTFIELD_RAW, 0, 92, NULL },
		{ "item1count", TXTFIELD_BYTE, 0, 97, NULL },
		{ "item2", TXTFIELD_RAW, 0, 100, NULL },
		{ "item2count", TXTFIELD_BYTE, 0, 105, NULL },
		{ "item3", TXTFIELD_RAW, 0, 108, NULL },
		{ "item3count", TXTFIELD_BYTE, 0, 113, NULL },
		{ "item4", TXTFIELD_RAW, 0, 116, NULL },
		{ "item4count", TXTFIELD_BYTE, 0, 121, NULL },
		{ "item5", TXTFIELD_RAW, 0, 124, NULL },
		{ "item5count", TXTFIELD_BYTE, 0, 129, NULL },
		{ "item6", TXTFIELD_RAW, 0, 132, NULL },
		{ "item6count", TXTFIELD_BYTE, 0, 137, NULL },
		{ "item7", TXTFIELD_RAW, 0, 140, NULL },
		{ "item7count", TXTFIELD_BYTE, 0, 145, NULL },
		{ "item8", TXTFIELD_RAW, 0, 148, NULL },
		{ "item8count", TXTFIELD_BYTE, 0, 153, NULL },
		{ "item9", TXTFIELD_RAW, 0, 156, NULL },
		{ "item9count", TXTFIELD_BYTE, 0, 161, NULL },
		{ "item10", TXTFIELD_RAW, 0, 164, NULL },
		{ "item10count", TXTFIELD_BYTE, 0, 169, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pCharStatsTxt = (D2CharStatsTxt*)DATATBLS_CompileTxt(hArchive, "charstats", pTbl, &sgptDataTables->nCharStatsTxtRecordCount, sizeof(D2CharStatsTxt));

	DATATBLS_InitUnicodeClassNamesInCharStatsTxt();
}

//D2Common.0x6FD51BF0
void __fastcall DATATBLS_LoadDifficultyLevelsTxt(HD2ARCHIVE hArchive)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "ResistPenalty", TXTFIELD_DWORD, 0, 0, NULL },
		{ "DeathExpPenalty", TXTFIELD_DWORD, 0, 4, NULL },
		{ "UberCodeOddsNormal", TXTFIELD_DWORD, 0, 8, NULL },
		{ "UberCodeOddsGood", TXTFIELD_DWORD, 0, 12, NULL },
		{ "UltraCodeOddsNormal", TXTFIELD_DWORD, 0, 32, NULL },
		{ "UltraCodeOddsGood", TXTFIELD_DWORD, 0, 36, NULL },
		{ "LifeStealDivisor", TXTFIELD_DWORD, 0, 40, NULL },
		{ "ManaStealDivisor", TXTFIELD_DWORD, 0, 44, NULL },
		{ "MonsterSkillBonus", TXTFIELD_DWORD, 0, 16, NULL },
		{ "MonsterFreezeDivisor", TXTFIELD_DWORD, 0, 20, NULL },
		{ "MonsterColdDivisor", TXTFIELD_DWORD, 0, 24, NULL },
		{ "AiCurseDivisor", TXTFIELD_DWORD, 0, 28, NULL },
		{ "UniqueDamageBonus", TXTFIELD_DWORD, 0, 48, NULL },
		{ "ChampionDamageBonus", TXTFIELD_DWORD, 0, 52, NULL },
		{ "HireableBossDamagePercent", TXTFIELD_DWORD, 0, 56, NULL },
		{ "MonsterCEDamagePercent", TXTFIELD_DWORD, 0, 60, NULL },
		{ "StaticFieldMin", TXTFIELD_DWORD, 0, 64, NULL },
		{ "GambleRare", TXTFIELD_DWORD, 0, 68, NULL },
		{ "GambleSet", TXTFIELD_DWORD, 0, 72, NULL },
		{ "GambleUnique", TXTFIELD_DWORD, 0, 76, NULL },
		{ "GambleUber", TXTFIELD_DWORD, 0, 80, NULL },
		{ "GambleUltra", TXTFIELD_DWORD, 0, 84, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pDifficultyLevelsTxt = (D2DifficultyLevelsTxt*)DATATBLS_CompileTxt(hArchive, "difficultylevels", pTbl, &sgptDataTables->nDifficultyLevelsTxtRecordCount, sizeof(D2DifficultyLevelsTxt));
#define NUM_DIFFICULTY_LEVELS 3
	D2_ASSERT(sgptDataTables->nDifficultyLevelsTxtRecordCount == NUM_DIFFICULTY_LEVELS);
}

// FIELD

// CUBE

// INVENTORY & ITEMS

// LEVELS & AUTOMAP

// MISSILES

// MONSTER & TC

// OBJECTS

// OVERLAY

// SEQUENCE & TOKEN
