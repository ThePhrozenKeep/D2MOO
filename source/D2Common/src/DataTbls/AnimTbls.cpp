#include "D2DataTbls.h"

#include "D2Composit.h"
#include <Units/Units.h>

//TODO: Find names


//D2Common.0x6FD473C0
D2AnimDataStrc* __fastcall DATATBLS_LoadAnimDataD2(void* pMemPool)
{
	D2AnimDataRecordStrc** pHashTable = NULL;
	D2AnimDataStrc* pAnimData = NULL;
	char* pRecords = NULL;
	char szPath[MAX_PATH] = {};

	pAnimData = (D2AnimDataStrc*)FOG_AllocPool(NULL, sizeof(D2AnimDataStrc), __FILE__, __LINE__, 0);
	memset(pAnimData, 0x00, sizeof(D2AnimDataStrc));

	wsprintfA(szPath, "%s\\AnimData.d2", "DATA\\GLOBAL");
	pRecords = (char*)DATATBLS_GetBinaryData(pMemPool, szPath, NULL, __FILE__, __LINE__);
	pAnimData->pRecords = (D2AnimDataRecordStrc*)pRecords;

	pHashTable = pAnimData->pHashTable;
	for (int i = 0; i < 256; ++i)
	{
		*pHashTable = (D2AnimDataRecordStrc*)pRecords;
		++pHashTable;

		pRecords += 160 * *(int*)pRecords + 4;
	}

	pAnimData->pDefaultRecord.dwFrames = 2048;
	pAnimData->pDefaultRecord.dwAnimSpeed = 256;

	return pAnimData;
}

//D2Common.0x6FD47460
void __fastcall DATATBLS_UnloadAnimDataD2(D2AnimDataStrc* pAnimData)
{
	if (pAnimData)
	{
		FOG_Free(pAnimData->pRecords, __FILE__, __LINE__, 0);
		FOG_FreePool(NULL, pAnimData, __FILE__, __LINE__, 0);
	}
}

//D2Common.0x6FD474A0
D2AnimDataRecordStrc* __fastcall DATATBLS_GetAnimDataRecord(D2UnitStrc* pUnit, int nClassId, int nMode, int nUnitType, D2InventoryStrc* pInventory)
{
	D2AnimDataRecordStrc* pRecord = NULL;
	D2AnimDataRecordStrc* pHash = NULL;
	D2AnimDataStrc* pAnimData = NULL;
	char* szTmp = NULL;
	int nWeaponClassCode = 0;
	int nCounter = 0;
	int nHash = 0;
	char szPath[8] = {};

	D2Common_10884_COMPOSIT_unk(pUnit, nClassId, nMode, nUnitType, pInventory, szPath, &nWeaponClassCode, FALSE, 1);

	szTmp = szPath;
	while (*szTmp)
	{
		if (*szTmp >= 'a' && *szTmp <= 'z')
		{
			*szTmp -= ' ';
		}
		++szTmp;
	}

	pAnimData = DATATBLS_GetAnimData();

	nHash = 0;
	szTmp = szPath;
	while(*szTmp)
	{
		nHash += *szTmp;
		++szTmp;
	}

	pHash = pAnimData->pHashTable[(uint8_t)nHash];
	nCounter = *(int*)&pHash->szAnimDataName[0];
	pRecord = (D2AnimDataRecordStrc*)((char*)pHash + 4);

	while (nCounter > 0)
	{
		D2_ASSERT(strlen(szPath) <= 8);
		D2_ASSERT(strlen(pRecord->szAnimDataName) <= 8);

		if (*(int*)szPath == *(int*)&pRecord->szAnimDataName[0] && *(int*)&szPath[4] == *(int*)&pRecord->szAnimDataName[4])
		{
			return pRecord;
		}

		++pRecord;
		--nCounter;
	}

	return &DATATBLS_GetAnimData()->pDefaultRecord;
}

//D2Common.0x6FD475D0 (#10640)
void __stdcall D2Common_10640(D2UnitStrc* pUnit, int nAnimSpeed)
{
	D2AnimDataRecordStrc* pRecord = NULL;
	D2AnimDataRecordStrc* pHash = NULL;
	D2AnimDataStrc* pAnimData = NULL;
	char* szTmp = NULL;
	int nWeaponClassCode = 0;
	int nCounter = 0;
	int nHash = 0;
	char szPath[8] = {};

	D2Common_10885_COMPOSIT_unk(pUnit, szPath, &nWeaponClassCode, 0, 1, pUnit->pInventory, -1);

	szTmp = szPath;
	while (*szTmp)
	{
		if (*szTmp >= 'a' && *szTmp <= 'z')
		{
			*szTmp -= ' ';
		}
		++szTmp;
	}

	pAnimData = DATATBLS_GetAnimData();

	nHash = 0;
	szTmp = szPath;
	while (*szTmp)
	{
		nHash += *szTmp;
		++szTmp;
	}

	pHash = pAnimData->pHashTable[(uint8_t)nHash];
	nCounter = *(int*)&pHash->szAnimDataName[0];
	pRecord = (D2AnimDataRecordStrc*)((char*)pHash + 4);

	while (nCounter > 0)
	{
		D2_ASSERT(strlen(szPath) <= 8);
		D2_ASSERT(strlen(pRecord->szAnimDataName) <= 8);

		if (*(int*)szPath == *(int*)&pRecord->szAnimDataName[0] && *(int*)&szPath[4] == *(int*)&pRecord->szAnimDataName[4])
		{
			pRecord->dwAnimSpeed = nAnimSpeed;
			return;
		}

		++pRecord;
		--nCounter;
	}

	FOG_10024_PacketAssertion("DataTablesUnitAnimInfoDebugSet", __FILE__, __LINE__);
	exit(-1);
}

//D2Common.0x6FD47700 (#10641)
BOOL __stdcall D2Common_10641(char* szPath, int* pLength, int* pOutAnimSpeed, int* pOutFirstFrameTagged)
{
	D2AnimDataRecordStrc* pRecord = NULL;
	D2AnimDataRecordStrc* pHash = NULL;
	D2AnimDataStrc* pAnimData = NULL;
	char* szTmp = NULL;
	int nCounter = 0;
	int nHash = 0;

	szTmp = szPath;
	while (*szTmp)
	{
		if (*szTmp >= 'a' && *szTmp <= 'z')
		{
			*szTmp -= ' ';
		}
		++szTmp;
	}

	pAnimData = DATATBLS_GetAnimData();

	nHash = 0;
	szTmp = szPath;
	while (*szTmp)
	{
		nHash += *szTmp;
		++szTmp;
	}

	pHash = pAnimData->pHashTable[(uint8_t)nHash];
	nCounter = *(int*)&pHash->szAnimDataName[0];
	pRecord = (D2AnimDataRecordStrc*)((char*)pHash + 4);

	while (nCounter > 0)
	{
		D2_ASSERT(strlen(szPath) <= 8);
		D2_ASSERT(strlen(pRecord->szAnimDataName) <= 8);

		if (*(int*)szPath == *(int*)&pRecord->szAnimDataName[0] && *((int*)szPath + 1) == *(int*)&pRecord->szAnimDataName[4])
		{
			*pLength = pRecord->dwFrames;
			*pOutAnimSpeed = pRecord->dwAnimSpeed;

			for (uint32_t i = 0; i < pRecord->dwFrames; ++i)
			{
				if (i >= 144 || pRecord->pFrameFlags[i])
				{
					*pOutFirstFrameTagged = i;
					return TRUE;
				}
			}

			*pOutFirstFrameTagged = 0;
			return TRUE;
		}

		++pRecord;
		--nCounter;
	}

	*pLength = pAnimData->pDefaultRecord.dwFrames;
	*pOutAnimSpeed = pAnimData->pDefaultRecord.dwAnimSpeed;
	*pOutFirstFrameTagged = *pLength;

	return FALSE;
}
