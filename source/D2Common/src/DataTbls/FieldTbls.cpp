#include "D2DataTbls.h"

#include <Archive.h>

#include "D2Collision.h"


//D2Common.0x6FDD8480
static const int gnFieldXOffsets[] = { 0, 1, 1, 1, 0, -1, -1, -1, 0 };
//D2Common.0xFDD84A4
static const int gnFieldYOffsets[] = { -1, -1, 0, 1, 1, 1, 0, -1, 0 };


//D2Common.0x6FD51FC0
BOOL __fastcall DATATBLS_LoadExpFieldD2(void* pMemPool)
{
	char szPath[80] = {};
	char* pExpField = NULL;
	size_t nSize = 0;

	wsprintfA(szPath, "%s\\expfield.d2", "DATA\\GLOBAL");
	pExpField = (char*)ARCHIVE_READ_FILE_TO_ALLOC_BUFFER(pMemPool, szPath, &nSize);

	return DATATBLS_InitializeCollisionFieldTable(pExpField, nSize);
}

//D2Common.0x6FD52010 (#11089)
BOOL __stdcall DATATBLS_InitializeCollisionFieldTable(char* pExpField, int nSize)
{
	uint32_t v2 = *(uint32_t*)(pExpField + 2);
	uint32_t v3 = *(uint32_t*)(pExpField + 6);

	sgptDataTables->pFieldData = (uint8_t*)D2_ALLOC_POOL(nullptr, v2 * v3);
	D2_ASSERT(sgptDataTables->pFieldData);
	memcpy(sgptDataTables->pFieldData, pExpField + 10, v2 * v3);

	sgptDataTables->pCollisionField.nWidth = v3;
	sgptDataTables->pCollisionField.nHeight = v2;
	sgptDataTables->pCollisionField.nArea = v2 * v3;
	sgptDataTables->pCollisionField.nCenterX = v3 >> 1;
	sgptDataTables->pCollisionField.nCenterY = v2 >> 1;

	for (int i = 0; i < ARRAY_SIZE(gnFieldXOffsets); ++i)
	{
		sgptDataTables->ExpFieldI[i] = gnFieldXOffsets[i] + v3 * gnFieldYOffsets[i];
	}

	FOG_Free(pExpField, __FILE__, __LINE__, 0);

	return TRUE;
}

//D2Common.0x6FD520F0 (#11090)
BOOL __stdcall DATATBLS_FreeCollisionFieldTable()
{
	if (sgptDataTables->pFieldData)
	{
		D2_FREE_POOL(nullptr, sgptDataTables->pFieldData);
	}

	sgptDataTables->pFieldData = NULL;
	sgptDataTables->pCollisionField.nWidth = 0;
	sgptDataTables->pCollisionField.nHeight = 0;
	sgptDataTables->pCollisionField.nArea = 0;
	sgptDataTables->pCollisionField.nCenterX = 0;
	sgptDataTables->pCollisionField.nCenterY = 0;

	return TRUE;
}

//D2Common.0x6FD52140 (#11091)
void __stdcall DATATBLS_GetCollisionFieldCenter(int* pCenterX, int* pCenterY)
{
	*pCenterX = sgptDataTables->pCollisionField.nCenterX;
	*pCenterY = sgptDataTables->pCollisionField.nCenterY;
}

//D2Common.0x6FD52160 (#11092)
int __stdcall DATATBLS_GetCollisionFieldWidth()
{
	return sgptDataTables->pCollisionField.nWidth;
}

//D2Common.0x6FD52170 (#11093)
int __stdcall DATATBLS_GetCollisionFieldHeight()
{
	return sgptDataTables->pCollisionField.nHeight;
}

//D2Common.0x6FD52180 (#11094)
D2FieldStrc* __stdcall DATATBLS_AllocField()
{
	D2FieldStrc* pField = D2_ALLOC_STRC_POOL(nullptr, D2FieldStrc);
	D2_ASSERT(pField);

	pField->nX = 0;
	pField->nY = 0;

	return pField;
}

//D2Common.0x6FD521D0 (#11095)
void __stdcall DATATBLS_FreeField(D2FieldStrc* pField)
{
	D2_ASSERT(pField);

	D2_FREE_POOL(nullptr, pField);
}

//D2Common.0x6FD52210 (#11096)
void __stdcall DATATBLS_SetFieldCoordinates(D2FieldStrc* pField, int nX, int nY)
{
	D2_ASSERT(pField);

	pField->nX = nX;
	pField->nY = nY;
}

//D2Common.0x6FD52250 (#11097)
int __stdcall D2Common_11097(D2FieldStrc* pField, int nX, int nY)
{
	D2_ASSERT(pField);

	return *(&sgptDataTables->pFieldData[((nY - pField->nY) << 8) - pField->nX] + nX);
}

//D2Common.0x6FD522A0 (#11098)
int __stdcall D2Common_11098(D2FieldStrc* pField, int* pX, int* pY)
{
	
	D2_ASSERT(pField);

	int nIndex = *(&sgptDataTables->pFieldData[((sgptDataTables->pCollisionField.nCenterY + *pY - pField->nY) << 8) - pField->nX] + *pX + sgptDataTables->pCollisionField.nCenterX);

	*pX += gnFieldXOffsets[nIndex];
	*pY += gnFieldYOffsets[nIndex];

	return *(&sgptDataTables->pFieldData[((sgptDataTables->pCollisionField.nCenterY + *pY - pField->nY) << 8) - pField->nX] + *pX + sgptDataTables->pCollisionField.nCenterX) != 8;
}

//D2Common.0x6FD52360 (#11099)
BOOL __stdcall D2Common_11099(D2FieldStrc* pField, D2RoomStrc* pRoom, int nX, int nY, uint16_t fMask)
{
	if (pRoom)
	{
		if (COLLISION_CheckMask(pRoom, nX, nY, fMask))
		{
			return TRUE;
		}

		while (D2Common_11098(pField, &nX, &nY))
		{
			if (COLLISION_CheckMask(pRoom, nX, nY, fMask))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
