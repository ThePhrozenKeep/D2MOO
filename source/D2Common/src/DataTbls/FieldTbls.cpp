#include "D2DataTbls.h"

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
	int nSize = 0;

	wsprintfA(szPath, "%s\\expfield.d2", "DATA\\GLOBAL");
	pExpField = (char*)DATATBLS_GetBinaryData(pMemPool, szPath, &nSize, __FILE__, __LINE__);

	return DATATBLS_InitializeCollisionFieldTable(pExpField, nSize);
}

//D2Common.0x6FD52010 (#11089)
BOOL __stdcall DATATBLS_InitializeCollisionFieldTable(char* pExpField, int nSize)
{
	DWORD v2 = *(DWORD*)(pExpField + 2);
	DWORD v3 = *(DWORD*)(pExpField + 6);

	gpDataTables.pFieldData = (char*)FOG_AllocServerMemory(NULL, v2 * v3, __FILE__, __LINE__, 0);
	D2_ASSERT(gpDataTables.pFieldData);
	memcpy(gpDataTables.pFieldData, pExpField + 10, v2 * v3);

	gpDataTables.pCollisionField.nWidth = v3;
	gpDataTables.pCollisionField.nHeight = v2;
	gpDataTables.pCollisionField.nArea = v2 * v3;
	gpDataTables.pCollisionField.nCenterX = v3 >> 1;
	gpDataTables.pCollisionField.nCenterY = v2 >> 1;

	for (int i = 0; i < ARRAY_SIZE(gnFieldXOffsets); ++i)
	{
		gpDataTables.ExpFieldI[i] = gnFieldXOffsets[i] + v3 * gnFieldYOffsets[i];
	}

	FOG_FreeClientMemory(pExpField, __FILE__, __LINE__, 0);

	return TRUE;
}

//D2Common.0x6FD520F0 (#11090)
BOOL __stdcall DATATBLS_FreeCollisionFieldTable()
{
	if (gpDataTables.pFieldData)
	{
		FOG_FreeServerMemory(NULL, gpDataTables.pFieldData, __FILE__, __LINE__, 0);
	}

	gpDataTables.pFieldData = NULL;
	gpDataTables.pCollisionField.nWidth = 0;
	gpDataTables.pCollisionField.nHeight = 0;
	gpDataTables.pCollisionField.nArea = 0;
	gpDataTables.pCollisionField.nCenterX = 0;
	gpDataTables.pCollisionField.nCenterY = 0;

	return TRUE;
}

//D2Common.0x6FD52140 (#11091)
void __stdcall DATATBLS_GetCollisionFieldCenter(int* pCenterX, int* pCenterY)
{
	*pCenterX = gpDataTables.pCollisionField.nCenterX;
	*pCenterY = gpDataTables.pCollisionField.nCenterY;
}

//D2Common.0x6FD52160 (#11092)
int __stdcall DATATBLS_GetCollisionFieldWidth()
{
	return gpDataTables.pCollisionField.nWidth;
}

//D2Common.0x6FD52170 (#11093)
int __stdcall DATATBLS_GetCollisionFieldHeight()
{
	return gpDataTables.pCollisionField.nHeight;
}

//D2Common.0x6FD52180 (#11094)
D2FieldStrc* __stdcall DATATBLS_AllocField()
{
	D2FieldStrc* pField = (D2FieldStrc*)FOG_AllocServerMemory(NULL, sizeof(D2FieldStrc), __FILE__, __LINE__, 0);
	D2_ASSERT(pField);

	pField->nX = 0;
	pField->nY = 0;

	return pField;
}

//D2Common.0x6FD521D0 (#11095)
void __stdcall DATATBLS_FreeField(D2FieldStrc* pField)
{
	D2_ASSERT(pField);

	FOG_FreeServerMemory(NULL, pField, __FILE__, __LINE__, 0);
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

	return *(&gpDataTables.pFieldData[((nY - pField->nY) << 8) - pField->nX] + nX);
}

//D2Common.0x6FD522A0 (#11098)
int __stdcall D2Common_11098(D2FieldStrc* pField, int* pX, int* pY)
{
	int nIndex = 0;
	
	D2_ASSERT(pField);

	nIndex = *(&gpDataTables.pFieldData[((gpDataTables.pCollisionField.nCenterY + *pY - pField->nY) << 8) - pField->nX] + *pX + gpDataTables.pCollisionField.nCenterX);

	*pX += gnFieldXOffsets[nIndex];
	*pY += gnFieldYOffsets[nIndex];

	return *(&gpDataTables.pFieldData[((gpDataTables.pCollisionField.nCenterY + *pY - pField->nY) << 8) - pField->nX] + *pX + gpDataTables.pCollisionField.nCenterX) != 8;
}

//D2Common.0x6FD52360 (#11099)
BOOL __stdcall D2Common_11099(D2FieldStrc* pField, D2RoomStrc* pRoom, int nX, int nY, WORD fMask)
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
